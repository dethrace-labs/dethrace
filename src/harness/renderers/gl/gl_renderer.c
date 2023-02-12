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

extern float gCamera_hither;
extern float gCamera_yon;

static GLuint screen_buffer_vao, screen_buffer_ebo;
static GLuint screen_texture, palette_texture, depth_texture;

static GLuint shader_program_2d;
static GLuint shader_program_3d;
static GLuint framebuffer_id, framebuffer_texture = 0;
static uint8_t gl_palette[4 * 256]; // RGBA
static uint8_t* screen_buffer_flip_pixels;
static uint16_t* depth_buffer_flip_pixels;

static int window_width, window_height, render_width, render_height;
static int vp_x, vp_y, vp_width, vp_height;

static br_pixelmap* last_shade_table = NULL;
static int dirty_buffers = 0;

tStored_material* current_material;

struct {
    GLuint pixels, uv_transform;
    GLuint shade_table;
    GLuint model, view, projection;
    GLuint palette_index_override;
    GLuint clip_plane_count;
    GLuint clip_planes[6];
    GLuint light_value;
} uniforms_3d;

struct {
    GLuint pixels, palette;
} uniforms_2d;

struct {
    GLuint pixels, palette;
} uniforms_2dpp;

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
    uniforms_3d.pixels = GetValidatedUniformLocation(shader_program_3d, "u_pixels");
    uniforms_3d.uv_transform = GetValidatedUniformLocation(shader_program_3d, "u_texture_coords_transform");
    uniforms_3d.shade_table = GetValidatedUniformLocation(shader_program_3d, "u_shade_table");
    uniforms_3d.projection = GetValidatedUniformLocation(shader_program_3d, "u_projection");
    uniforms_3d.palette_index_override = GetValidatedUniformLocation(shader_program_3d, "u_palette_index_override");
    uniforms_3d.view = GetValidatedUniformLocation(shader_program_3d, "u_view");
    uniforms_3d.light_value = GetValidatedUniformLocation(shader_program_3d, "u_light_value");

    // bind the uniform samplers to texture units. palette=1, shadetable=2
    glUniform1i(uniforms_3d.pixels, 0);
    glUniform1i(uniforms_3d.shade_table, 2);
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
    glGenTextures(1, &screen_texture);
    glGenTextures(1, &palette_texture);
    glGenTextures(1, &framebuffer_texture);
    glGenTextures(1, &depth_texture);

    // setup framebuffer
    glGenFramebuffers(1, &framebuffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

    // set pixel storage alignment to 1 byte
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, render_width, render_height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);

    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, gl_palette);

    // reset active texture back to default
    glActiveTexture(GL_TEXTURE0);

    CHECK_GL_ERROR("GLRenderer_SetPalette");
}

void GLRenderer_SetShadeTable(br_pixelmap* table) {

    if (last_shade_table == table) {
        return;
    }

    glActiveTexture(GL_TEXTURE2);
    tStored_pixelmap* stored = table->stored;
    glBindTexture(GL_TEXTURE_2D, stored->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // reset active texture back to default
    glActiveTexture(GL_TEXTURE0);

    last_shade_table = table;
}

extern br_v1db_state v1db;

void GLRenderer_BeginScene(br_actor* camera, br_pixelmap* colour_buffer) {
    glViewport(colour_buffer->base_x, render_height - colour_buffer->height - colour_buffer->base_y, colour_buffer->width, colour_buffer->height);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader_program_3d);

    current_material = NULL;
    last_shade_table = NULL;

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

    if (0 /*gDebugCamera_active*/) {
        // float m2[4][4];
        // memcpy(m2, DebugCamera_View(), sizeof(float) * 16);
        // glUniformMatrix4fv(uniforms_3d.view, 1, GL_FALSE, (GLfloat*)m2);
    } else {

        br_matrix4 cam44 = {
            { { v1db.camera_path[0].m.m[0][0], v1db.camera_path[0].m.m[0][1], v1db.camera_path[0].m.m[0][2], 0 },
                { v1db.camera_path[0].m.m[1][0], v1db.camera_path[0].m.m[1][1], v1db.camera_path[0].m.m[1][2], 0 },
                { v1db.camera_path[0].m.m[2][0], v1db.camera_path[0].m.m[2][1], v1db.camera_path[0].m.m[2][2], 0 },
                { v1db.camera_path[0].m.m[3][0], v1db.camera_path[0].m.m[3][1], v1db.camera_path[0].m.m[3][2], 1 } }
        };
        br_matrix4 cam44_inverse;
        BrMatrix4Inverse(&cam44_inverse, &cam44);

        glUniformMatrix4fv(uniforms_3d.view, 1, GL_FALSE, &cam44_inverse.m[0][0]);
    }
    br_matrix4 p;
    br_camera* cam = camera->type_data;
    BrMatrix4Perspective(&p, cam->field_of_view, cam->aspect, cam->hither_z, cam->yon_z);
    // hack: not sure why we have to do this, but this makes the result the same as `glm_perspective`
    p.m[2][2] *= -1;
    glUniformMatrix4fv(uniforms_3d.projection, 1, GL_FALSE, &p.m[0][0]);

    // DebugCamera_Update();

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
}

void GLRenderer_EndScene() {

    //  switch back to default fb
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    CHECK_GL_ERROR("GLRenderer_RenderFullScreenQuad");
}

void GLRenderer_FullScreenQuad(uint8_t* screen_buffer) {
    glViewport(vp_x, vp_y, vp_width, vp_height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glUseProgram(shader_program_2d);
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, render_width, render_height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, screen_buffer);
    glBindVertexArray(screen_buffer_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_buffer_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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

    fmt_vertex* verts = malloc(sizeof(fmt_vertex) * total_verts);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(fmt_vertex) * total_verts, verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(fmt_vertex), (void*)offsetof(fmt_vertex, p));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(fmt_vertex), (void*)offsetof(fmt_vertex, n));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(fmt_vertex), (void*)offsetof(fmt_vertex, map));
    glEnableVertexAttribArray(2);

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

    glUniformMatrix2x3fv(uniforms_3d.uv_transform, 1, GL_TRUE, &material->map_transform.m[0][0]);
    glUniform1i(uniforms_3d.palette_index_override, material->index_base);
    if (material->shade_table) {
        GLRenderer_SetShadeTable(material->shade_table);
    }

    if ((material->flags & BR_MATF_LIGHT) && !(material->flags & BR_MATF_PRELIT) && material->shade_table) {
        // TODO: light value shouldn't always be 0? Works for shadows, not sure about other things.
        glUniform1i(uniforms_3d.light_value, 0);
    } else {
        glUniform1i(uniforms_3d.light_value, -1);
    }

    if (material->flags & (BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE)) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
    }

    if (material->pixelmap) {
        tStored_pixelmap* stored_px = material->pixelmap->stored;
        if (stored_px != NULL) {
            glBindTexture(GL_TEXTURE_2D, stored_px->id);
            glUniform1i(uniforms_3d.palette_index_override, -1);
        }
    }
}

void GLRenderer_Model(br_actor* actor, br_model* model, br_matrix34 model_matrix, br_token render_type) {
    tStored_model_context* ctx;
    ctx = model->stored;
    v11model* v11 = model->prepared;

    // LOG_DEBUG("model rendering %s", model->identifier);
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

    // br_actor can have a material too, which is applied to the faces if the face doesn't have a texture
    if (actor->material) {
        setActiveMaterial(actor->material->stored);
    } else {
        // TODO: set defaults for now. This fixes missing curb materials but probably isn't the right fix.
        LOG_WARN_ONCE("set default palette override for missing actor material")
        glUniform1i(uniforms_3d.palette_index_override, 227);
        glUniform1i(uniforms_3d.light_value, -1);
    }

    switch (render_type) {
    case BRT_TRIANGLE:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case BRT_LINE:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform1i(uniforms_3d.palette_index_override, 255);
        glUniform1i(uniforms_3d.light_value, -1);
        break;
    default:
        LOG_PANIC("render_type %d is not supported?!", render_type);
    }

    v11group* group;
    int element_index = 0;
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

void GLRenderer_FlushBuffers(br_pixelmap* color_buffer, br_pixelmap* depth_buffer) {

    if (!dirty_buffers) {
        return;
    }

    // pull framebuffer into cpu memory to emulate BRender behavior
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, screen_buffer_flip_pixels);

    // flip texture to match the expected orientation
    int dest_y = render_height;
    uint8_t* pm_pixels = color_buffer->pixels;
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

    // pull depthbuffer into cpu memory to emulate BRender behavior
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, depth_buffer_flip_pixels);

    dest_y = color_buffer->height;
    int src_y = render_height - color_buffer->base_y - color_buffer->height;
    uint16_t* depth_pixels = depth_buffer->pixels;
    for (int y = 0; y < color_buffer->height; y++) {
        dest_y--;
        for (int x = 0; x < color_buffer->width; x++) {
            depth_pixels[dest_y * render_width + x] = depth_buffer_flip_pixels[src_y * render_width + color_buffer->base_x + x];
        }
        src_y++;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
    glClear(GL_COLOR_BUFFER_BIT);
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
