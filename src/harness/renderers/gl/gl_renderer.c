#include "gl_renderer.h"
#include "brender/brender.h"
#include "harness.h"
#include "harness/trace.h"
#include "resources/3d_frag.glsl.h"
#include "resources/3d_vert.glsl.h"
#include "resources/framebuffer_frag.glsl.h"
#include "resources/framebuffer_vert.glsl.h"
#include "stored_context.h"

#include <glad/glad.h>
#include <stdio.h>
#include <string.h>

static GLuint screen_buffer_vao, screen_buffer_ebo;
static GLuint fullscreen_quad_texture, palette_texture, depth_texture;

static GLuint shader_program_2d;
static GLuint shader_program_3d;
static GLuint framebuffer_id, framebuffer_texture = 0;

// holds the latest uploaded version of the colour_buffer. Available in shader for blending
static GLuint current_colourbuffer_texture;

static uint8_t gl_palette[4 * 256]; // RGBA
static uint8_t* screen_buffer_flip_pixels;
static uint16_t* depth_buffer_flip_pixels;

static int window_width, window_height, render_width, render_height;
static int vp_x, vp_y, vp_width, vp_height;

static br_pixelmap *last_colour_buffer, *last_depth_buffer;
static int dirty_buffers = 0;

static br_pixelmap *last_colour_buffer, *last_depth_buffer;

static tStored_material* current_material;
static br_pixelmap* current_shade_table;

// Increment flush_counter during flush, and upload_counter when we upload the colour_buffer texture.
// If the counters are equal, we can avoid re-uploading the same thing.
static unsigned int flush_counter = 0, colourbuffer_upload_counter = 0;

typedef struct gl_vertex {
    br_vector3 p;
    br_vector2 map;
    br_vector3 n;
    float colour_index; // float to allow interpolation
} gl_vertex;

struct {
    GLuint model, view, projection;
    GLuint clip_plane_count;
    GLuint clip_planes[6];
    GLuint colour_buffer_texture;
    GLuint viewport_height;

    GLuint material_flags;
    GLuint material_texture_enabled;
    GLuint material_texture_pixelmap;
    GLuint material_uv_transform;
    GLuint material_shade_table;
    GLuint material_blend_enabled;
    GLuint material_blend_table;
    GLuint material_index_base;
    GLuint material_index_range;
    GLuint material_shade_table_height;

} uniforms_3d;

struct {
    GLuint pixels, palette;
} uniforms_2d;

GLuint CreateShaderProgram(char* name, const char* vertex_shader, const int vertex_shader_len, const char* fragment_shader, const int fragment_shader_len) {
    int success;
    char log_buffer[1024];
    GLuint program;
    GLuint v_shader, f_shader;

    program = glCreateProgram();
    v_shader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertex_sources[] = { vertex_shader };
    glShaderSource(v_shader, 1, vertex_sources, &vertex_shader_len);
    glCompileShader(v_shader);
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(v_shader, 1024, NULL, log_buffer);
        LOG_PANIC("shader %s failed to compile: %s", name, log_buffer);
    }

    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragment_sources[] = { fragment_shader };
    glShaderSource(f_shader, 1, fragment_sources, &fragment_shader_len);
    glCompileShader(f_shader);
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log_buffer[1024];
        glGetShaderInfoLog(f_shader, 1024, NULL, log_buffer);
        LOG_PANIC("shader %s failed to compile: %s", name, log_buffer);
    }

    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    GLint link_ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        glGetShaderInfoLog(program, 1024, NULL, log_buffer);
        LOG_PANIC("shader program %s failed to link: %s", name, log_buffer);
    }
    return program;
}

GLint GetValidatedUniformLocation(GLuint program, char* uniform_name) {
    GLint location;
    location = glGetUniformLocation(program, uniform_name);
    if (location == -1) {
        LOG_PANIC("glGetUniformLocation(%d, %s) failed. Check the shader uniform names.", program, uniform_name);
    }
    return location;
}

void LoadShaders() {
    shader_program_2d = CreateShaderProgram("framebuffer", RESOURCES_FRAMEBUFFER_VERT_GLSL, sizeof(RESOURCES_FRAMEBUFFER_VERT_GLSL), RESOURCES_FRAMEBUFFER_FRAG_GLSL, sizeof(RESOURCES_FRAMEBUFFER_FRAG_GLSL));
    glUseProgram(shader_program_2d);
    uniforms_2d.pixels = GetValidatedUniformLocation(shader_program_2d, "u_pixels");
    uniforms_2d.palette = GetValidatedUniformLocation(shader_program_2d, "u_palette");

    // bind the uniform samplers to texture units:
    glUniform1i(uniforms_2d.pixels, 0);
    glUniform1i(uniforms_2d.palette, 1);

    shader_program_3d = CreateShaderProgram("3d", RESOURCES_3D_VERT_GLSL, sizeof(RESOURCES_3D_VERT_GLSL), RESOURCES_3D_FRAG_GLSL, sizeof(RESOURCES_3D_FRAG_GLSL));
    glUseProgram(shader_program_3d);
    uniforms_3d.clip_plane_count = GetValidatedUniformLocation(shader_program_3d, "u_clip_plane_count");
    for (int i = 0; i < 6; i++) {
        char name[32];
        sprintf(name, "u_clip_planes[%d]", i);
        uniforms_3d.clip_planes[i] = GetValidatedUniformLocation(shader_program_3d, name);
    }

    uniforms_3d.model = GetValidatedUniformLocation(shader_program_3d, "u_model");
    uniforms_3d.colour_buffer_texture = GetValidatedUniformLocation(shader_program_3d, "u_colour_buffer");
    uniforms_3d.projection = GetValidatedUniformLocation(shader_program_3d, "u_projection");
    uniforms_3d.view = GetValidatedUniformLocation(shader_program_3d, "u_view");
    uniforms_3d.viewport_height = GetValidatedUniformLocation(shader_program_3d, "u_viewport_height");

    uniforms_3d.material_flags = GetValidatedUniformLocation(shader_program_3d, "u_material_flags");
    uniforms_3d.material_texture_enabled = GetValidatedUniformLocation(shader_program_3d, "u_material_texture_enabled");
    uniforms_3d.material_texture_pixelmap = GetValidatedUniformLocation(shader_program_3d, "u_material_texture_pixelmap");
    uniforms_3d.material_uv_transform = GetValidatedUniformLocation(shader_program_3d, "u_material_uv_transform");
    uniforms_3d.material_shade_table = GetValidatedUniformLocation(shader_program_3d, "u_material_shade_table");
    uniforms_3d.material_shade_table_height = GetValidatedUniformLocation(shader_program_3d, "u_material_shade_table_height");
    uniforms_3d.material_blend_enabled = GetValidatedUniformLocation(shader_program_3d, "u_material_blend_enabled");
    uniforms_3d.material_blend_table = GetValidatedUniformLocation(shader_program_3d, "u_material_blend_table");
    uniforms_3d.material_index_base = GetValidatedUniformLocation(shader_program_3d, "u_material_index_base");
    uniforms_3d.material_index_range = GetValidatedUniformLocation(shader_program_3d, "u_material_index_range");

    // bind the uniform samplers to texture units
    glUniform1i(uniforms_3d.material_texture_pixelmap, 0);
    // palette occupies texture unit 1 but not required during 3d rendering
    glUniform1i(uniforms_3d.material_shade_table, 2);
    glUniform1i(uniforms_3d.material_blend_table, 3);
    glUniform1i(uniforms_3d.colour_buffer_texture, 4);
}

void SetupFullScreenRectGeometry() {
    float vertices[] = {
        // positions          // colors           // texture coords
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    GLuint vbo;
    glGenVertexArrays(1, &screen_buffer_vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &screen_buffer_ebo);

    glBindVertexArray(screen_buffer_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_buffer_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

static void update_viewport() {
    const float target_aspect_ratio = (float)render_width / render_height;
    const float aspect_ratio = (float)window_width / window_height;

    vp_width = window_width;
    vp_height = window_height;
    if (aspect_ratio != target_aspect_ratio) {
        if (aspect_ratio > target_aspect_ratio) {
            vp_width = window_height * target_aspect_ratio + .5f;
        } else {
            vp_height = window_width / target_aspect_ratio + .5f;
        }
    }
    vp_x = (window_width - vp_width) / 2;
    vp_y = (window_height - vp_height) / 2;
}

void GLRenderer_Init(int width, int height, int pRender_width, int pRender_height) {
    window_width = width;
    window_height = height;
    render_width = pRender_width;
    render_height = pRender_height;
    update_viewport();

    LOG_INFO("OpenGL vendor string: %s", glGetString(GL_VENDOR));
    LOG_INFO("OpenGL renderer string: %s", glGetString(GL_RENDERER));
    LOG_INFO("OpenGL version string: %s", glGetString(GL_VERSION));
    LOG_INFO("OpenGL shading language version string: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    int maxTextureImageUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits);
    if (maxTextureImageUnits < 3) {
        LOG_PANIC("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS is %d. Need at least 3", maxTextureImageUnits);
    }

    LoadShaders();
    SetupFullScreenRectGeometry();

    // config
    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // textures
    glGenTextures(1, &fullscreen_quad_texture);
    glGenTextures(1, &palette_texture);
    glGenTextures(1, &framebuffer_texture);
    glGenTextures(1, &depth_texture);
    glGenTextures(1, &current_colourbuffer_texture);

    // setup framebuffer
    glGenFramebuffers(1, &framebuffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

    // set pixel storage alignment to 1 byte
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, fullscreen_quad_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, palette_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, render_width, render_height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);

    glBindTexture(GL_TEXTURE_2D, current_colourbuffer_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, render_width, render_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_PANIC("Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    screen_buffer_flip_pixels = malloc(sizeof(uint8_t) * render_width * render_height);
    depth_buffer_flip_pixels = malloc(sizeof(uint16_t) * render_width * render_height);

    CHECK_GL_ERROR("initializeOpenGLContext");
}

void GLRenderer_SetPalette(uint8_t* rgba_colors) {
    for (int i = 0; i < 256; i++) {
        gl_palette[i * 4] = rgba_colors[i * 4 + 2];
        gl_palette[i * 4 + 1] = rgba_colors[i * 4 + 1];
        gl_palette[i * 4 + 2] = rgba_colors[i * 4];
        // palette index 0 is transparent, so set alpha to 0
        if (i == 0) {
            gl_palette[i * 4 + 3] = 0;
        } else {
            gl_palette[i * 4 + 3] = 0xff;
        }
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, palette_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, gl_palette);

    // reset active texture back to default
    glActiveTexture(GL_TEXTURE0);

    CHECK_GL_ERROR("GLRenderer_SetPalette");
}

void GLRenderer_SetShadeTable(br_pixelmap* table) {
    if (current_shade_table == table) {
        return;
    }

    // shade table uses texture unit 2
    glActiveTexture(GL_TEXTURE2);
    tStored_pixelmap* stored = table->stored;
    glBindTexture(GL_TEXTURE_2D, stored->id);

    // reset active texture back to default
    glActiveTexture(GL_TEXTURE0);
    current_shade_table = table;
}

void GLRenderer_SetBlendTable(br_pixelmap* table) {

    if (flush_counter != colourbuffer_upload_counter) {
        GLRenderer_FlushBuffers(eFlush_color_buffer);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, current_colourbuffer_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, render_width, render_height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, last_colour_buffer->pixels);
        colourbuffer_upload_counter = flush_counter;
    }

    // blend table uses texture unit 3
    glActiveTexture(GL_TEXTURE3);
    tStored_pixelmap* stored = table->stored;
    glBindTexture(GL_TEXTURE_2D, stored->id);

    // reset active texture back to default
    glActiveTexture(GL_TEXTURE0);
}

extern br_v1db_state v1db;

void GLRenderer_BeginScene(br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    last_colour_buffer = colour_buffer;
    last_depth_buffer = depth_buffer;
    glViewport(colour_buffer->base_x, render_height - colour_buffer->height - colour_buffer->base_y, colour_buffer->width, colour_buffer->height);
    glUseProgram(shader_program_3d);
    glUniform1ui(uniforms_3d.viewport_height, render_height);

    current_material = NULL;
    current_shade_table = NULL;

    int enabled_clip_planes = 0;
    for (int i = 0; i < v1db.enabled_clip_planes.max; i++) {
        if (v1db.enabled_clip_planes.enabled == NULL || !v1db.enabled_clip_planes.enabled[i]) {
            continue;
        }
        br_vector4* v4 = v1db.enabled_clip_planes.enabled[i]->type_data;
        glUniform4f(uniforms_3d.clip_planes[enabled_clip_planes], v4->v[0], v4->v[1], v4->v[2], v4->v[3]);
        enabled_clip_planes++;
    }
    glUniform1i(uniforms_3d.clip_plane_count, enabled_clip_planes);

    br_matrix4 cam44 = {
        { { v1db.camera_path[0].m.m[0][0], v1db.camera_path[0].m.m[0][1], v1db.camera_path[0].m.m[0][2], 0 },
            { v1db.camera_path[0].m.m[1][0], v1db.camera_path[0].m.m[1][1], v1db.camera_path[0].m.m[1][2], 0 },
            { v1db.camera_path[0].m.m[2][0], v1db.camera_path[0].m.m[2][1], v1db.camera_path[0].m.m[2][2], 0 },
            { v1db.camera_path[0].m.m[3][0], v1db.camera_path[0].m.m[3][1], v1db.camera_path[0].m.m[3][2], 1 } }
    };
    br_matrix4 cam44_inverse;
    BrMatrix4Inverse(&cam44_inverse, &cam44);

    glUniformMatrix4fv(uniforms_3d.view, 1, GL_FALSE, &cam44_inverse.m[0][0]);

    br_matrix4 p;
    br_camera* cam = camera->type_data;
    BrMatrix4Perspective(&p, cam->field_of_view, cam->aspect, cam->hither_z, cam->yon_z);
    // hack: not sure why we have to do this, but this makes the result the same as `glm_perspective`
    p.m[2][2] *= -1;
    glUniformMatrix4fv(uniforms_3d.projection, 1, GL_FALSE, &p.m[0][0]);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
}

void GLRenderer_EndScene() {
    //  switch back to default fb and reset state
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDepthMask(GL_TRUE);
    glDepthMask(GL_TRUE);
    CHECK_GL_ERROR("GLRenderer_EndScene");
}

void GLRenderer_FullScreenQuad(uint8_t* screen_buffer) {

    glViewport(vp_x, vp_y, vp_width, vp_height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindTexture(GL_TEXTURE_2D, fullscreen_quad_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, render_width, render_height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, screen_buffer);

    glBindVertexArray(screen_buffer_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_buffer_ebo);
    glUseProgram(shader_program_2d);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    CHECK_GL_ERROR("GLRenderer_RenderFullScreenQuad");
}

void GLRenderer_ClearBuffers() {
    // clear our virtual framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // clear real framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CHECK_GL_ERROR("GLRenderer_ClearBuffers");
}

void GLRenderer_BufferModel(br_model* model) {
    tStored_model_context* ctx;
    v11model* v11;

    if (model->stored != NULL) {
        LOG_PANIC("trying to build a stored model");
    }

    v11 = model->prepared;
    ctx = NewStoredModelContext();

    int total_verts = 0, total_faces = 0;
    for (int i = 0; i < v11->ngroups; i++) {
        total_verts += v11->groups[i].nvertices;
        total_faces += v11->groups[i].nfaces;
    }

    // override normals
    br_vector3 v3 = { { 0, 0, 0 } };
    for (int g = 0; g < v11->ngroups; g++) {
        for (int i = 0; i < v11->groups[g].nvertices; i++) {
            v11->groups[g].vertices[i].n = v3;
        }
    }
    br_vector3 v0v1, v2v1, normal;
    for (int g = 0; g < v11->ngroups; g++) {
        v11group* group = &v11->groups[g];
        for (int i = 0; i < group->nfaces; i++) {
            v11face* f = &group->faces[i];
            fmt_vertex* v0 = &group->vertices[f->vertices[0]];
            fmt_vertex* v1 = &group->vertices[f->vertices[1]];
            fmt_vertex* v2 = &group->vertices[f->vertices[2]];
            BrVector3Sub(&v0v1, &v0->p, &v1->p);
            BrVector3Sub(&v2v1, &v2->p, &v1->p);
            BrVector3Cross(&normal, &v0v1, &v2v1);
            BrVector3Accumulate(&v0->n, &normal);
            BrVector3Accumulate(&v1->n, &normal);
            BrVector3Accumulate(&v2->n, &normal);
        }
    }
    for (int g = 0; g < v11->ngroups; g++) {
        for (int i = 0; i < v11->groups[g].nvertices; i++) {
            BrVector3Normalise(&v11->groups[g].vertices[i].n, &v11->groups[g].vertices[i].n);
        }
    }

    gl_vertex* verts = malloc(sizeof(gl_vertex) * total_verts);
    unsigned int* indices = malloc(sizeof(int) * 3 * total_faces);

    int v_index = 0;
    int i_index = 0;
    int face_offset = 0;
    for (int g = 0; g < v11->ngroups; g++) {
        for (int i = 0; i < v11->groups[g].nvertices; i++) {
            fmt_vertex* v = &v11->groups[g].vertices[i];
            verts[v_index].p = v->p;
            verts[v_index].n = v->n;
            verts[v_index].map = v->map;
            verts[v_index].colour_index = BR_ALPHA(v11->groups[g].vertex_colours[i]);
            v_index++;
        }
        for (int i = 0; i < v11->groups[g].nfaces; i++) {
            v11face* f = &v11->groups[g].faces[i];
            indices[i_index++] = f->vertices[0] + face_offset;
            indices[i_index++] = f->vertices[1] + face_offset;
            indices[i_index++] = f->vertices[2] + face_offset;
        }
        face_offset += v11->groups[g].nvertices;
    }

    glGenVertexArrays(1, &ctx->vao_id);
    glGenBuffers(1, &ctx->vbo_id);
    glGenBuffers(1, &ctx->ebo_id);

    // Vertices
    glBindVertexArray(ctx->vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gl_vertex) * total_verts, verts, GL_STATIC_DRAW);
    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gl_vertex), (void*)offsetof(gl_vertex, p));
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gl_vertex), (void*)offsetof(gl_vertex, n));
    glEnableVertexAttribArray(1);
    // uv coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gl_vertex), (void*)offsetof(gl_vertex, map));
    glEnableVertexAttribArray(2);
    // color
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(gl_vertex), (void*)offsetof(gl_vertex, colour_index));
    glEnableVertexAttribArray(3);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * total_faces, indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    free(verts);
    free(indices);

    model->stored = ctx;

    CHECK_GL_ERROR("after build model");
}

void setActiveMaterial(tStored_material* material) {
    if (material == NULL || material == current_material) {
        return;
    }

    glUniformMatrix2x3fv(uniforms_3d.material_uv_transform, 1, GL_TRUE, &material->map_transform.m[0][0]);

    if (material->pixelmap) {
        tStored_pixelmap* stored_px = material->pixelmap->stored;
        if (stored_px == NULL) {
            LOG_PANIC("stored_px is null for pixelmap %s", material->pixelmap->identifier);
        }
        glBindTexture(GL_TEXTURE_2D, stored_px->id);
        glUniform1ui(uniforms_3d.material_texture_enabled, 1);
    } else {
        glUniform1ui(uniforms_3d.material_texture_enabled, 0);

        // index_base and index_range are only used for untextured materials
        glUniform1ui(uniforms_3d.material_index_base, material->index_base);
        glUniform1ui(uniforms_3d.material_index_range, material->index_range);
    }

    if (material->shade_table) {
        glUniform1ui(uniforms_3d.material_shade_table_height, material->shade_table->height);
        GLRenderer_SetShadeTable(material->shade_table);
    }

    if (material->index_blend) {
        glUniform1ui(uniforms_3d.material_blend_enabled, 1);
        GLRenderer_SetBlendTable(material->index_blend);
        // materials with index_blend do not write to depth buffer (https://www.cwaboard.co.uk/viewtopic.php?p=105846&sid=58ad8910238000ca14b01dad85117175#p105846)
        glDepthMask(GL_FALSE);
    } else {
        glUniform1ui(uniforms_3d.material_blend_enabled, 0);
        glDepthMask(GL_TRUE);
    }

    glUniform1ui(uniforms_3d.material_flags, material->flags);

    if (material->flags & (BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE)) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
    }

    CHECK_GL_ERROR("GLRenderer_RenderModelxx");
}

void GLRenderer_Model(br_actor* actor, br_model* model, br_material* material, br_token render_type, br_matrix34 model_matrix) {
    tStored_model_context* ctx;
    v11group* group;
    int element_index = 0;

    if (model->flags & BR_MODF_DETHRACE_FORCE_BUFFER_UPDATE) {
        if (model->stored) {
            ((br_object*)model->stored)->dispatch->_free((br_object*)model->stored);
            model->stored = NULL;
        }
        GLRenderer_BufferModel(model);
        model->flags &= ~BR_MODF_DETHRACE_FORCE_BUFFER_UPDATE;
    }

    ctx = model->stored;
    v11model* v11 = model->prepared;

    if (v11 == NULL) {
        // LOG_WARN("No model prepared for %s", model->identifier);
        return;
    }

    GLfloat m[16] = {
        model_matrix.m[0][0], model_matrix.m[0][1], model_matrix.m[0][2], 0,
        model_matrix.m[1][0], model_matrix.m[1][1], model_matrix.m[1][2], 0,
        model_matrix.m[2][0], model_matrix.m[2][1], model_matrix.m[2][2], 0,
        model_matrix.m[3][0], model_matrix.m[3][1], model_matrix.m[3][2], 1
    };

    glUniformMatrix4fv(uniforms_3d.model, 1, GL_FALSE, m);
    glBindVertexArray(ctx->vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->ebo_id);

    // set default material for this actor/model
    setActiveMaterial(material->stored);

    switch (render_type) {
    case BRT_TRIANGLE:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case BRT_LINE:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform1ui(uniforms_3d.material_index_base, 255);
        glUniform1ui(uniforms_3d.material_flags, 0);
        break;
    default:
        LOG_PANIC("render_type %d is not supported?!", render_type);
    }

    for (int g = 0; g < v11->ngroups; g++) {
        group = &v11->groups[g];
        setActiveMaterial(group->stored);
        glDrawElements(GL_TRIANGLES, group->nfaces * 3, GL_UNSIGNED_INT, (void*)(element_index * sizeof(int)));
        element_index += group->nfaces * 3;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    dirty_buffers = 1;

    CHECK_GL_ERROR("GLRenderer_RenderModel");
}

void GLRenderer_BufferMaterial(br_material* mat) {
    tStored_material* stored = mat->stored;
    if (!stored) {
        stored = NewStoredMaterial();
        mat->stored = stored;
        if (mat->identifier) {
            strcpy(stored->identifier, mat->identifier);
        }
    }
    BrMatrix23Copy(&stored->map_transform, &mat->map_transform);
    stored->pixelmap = mat->colour_map;
    stored->flags = mat->flags;
    stored->shade_table = mat->index_shade;
    stored->index_base = mat->index_base;
    stored->index_range = mat->index_range;
    stored->index_blend = mat->index_blend;
}

void GLRenderer_BufferTexture(br_pixelmap* pm) {
    tStored_pixelmap* stored = pm->stored;
    if (pm->stored) {
    } else {
        stored = NewStoredPixelmap();
        glGenTextures(1, &stored->id);
        pm->stored = stored;
    }

    // sometimes the pixelmap has row_bytes > width. OpenGL expects linear pixels, so flatten it out
    uint8_t* linear_pixels = malloc(sizeof(uint8_t) * pm->width * pm->height);
    uint8_t* original_pixels = pm->pixels;
    for (int y = 0; y < pm->height; y++) {
        for (int x = 0; x < pm->width; x++) {
            linear_pixels[y * pm->width + x] = original_pixels[y * pm->row_bytes + x];
        }
    }

    glBindTexture(GL_TEXTURE_2D, stored->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, pm->width, pm->height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, linear_pixels);
    free(linear_pixels);

    CHECK_GL_ERROR("GLRenderer_BufferTexture");
}

void GLRenderer_FlushBuffers(tRenderer_flush_type flush_type) {

    if (!dirty_buffers) {
        return;
    }

    // pull framebuffer into cpu memory to emulate BRender behavior
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, screen_buffer_flip_pixels);

    // flip texture to match the expected orientation
    int dest_y = render_height;
    uint8_t* pm_pixels = last_colour_buffer->pixels;
    uint8_t new_pixel;
    for (int y = 0; y < render_height; y++) {
        dest_y--;
        for (int x = 0; x < render_width; x++) {
            new_pixel = screen_buffer_flip_pixels[y * render_width + x];
            if (new_pixel != 0) {
                pm_pixels[dest_y * render_width + x] = new_pixel;
            }
        }
    }

    if (flush_type == eFlush_all) {

        // pull depthbuffer into cpu memory to emulate BRender behavior
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, depth_buffer_flip_pixels);

        dest_y = last_colour_buffer->height;
        int src_y = render_height - last_colour_buffer->base_y - last_colour_buffer->height;
        uint16_t* depth_pixels = last_depth_buffer->pixels;
        for (int y = 0; y < last_colour_buffer->height; y++) {
            dest_y--;
            for (int x = 0; x < last_colour_buffer->width; x++) {
                uint16_t new_depth = depth_buffer_flip_pixels[src_y * render_width + last_colour_buffer->base_x + x];
                depth_pixels[dest_y * render_width + x] = new_depth;
            }
            src_y++;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
    glClear(GL_COLOR_BUFFER_BIT);
    flush_counter++;
    dirty_buffers = 0;
}

void GLRenderer_GetRenderSize(int* width, int* height) {
    *width = render_width;
    *height = render_height;
}

void GLRenderer_GetWindowSize(int* width, int* height) {
    *width = window_width;
    *height = window_height;
}

void GLRenderer_SetWindowSize(int width, int height) {
    window_width = width;
    window_height = height;
    update_viewport();
}

void GLRenderer_GetViewport(int* x, int* y, int* width, int* height) {
    *x = vp_x;
    *y = vp_y;
    *width = vp_width;
    *height = vp_height;
}
