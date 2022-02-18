#include "gl_renderer.h"
#include "brender/brender.h"
#include "cameras/debug_camera.h"
#include "gl_brender_stored_context.h"
#include "gl_renderer_shaders.h"
#include "harness.h"
#include "harness/trace.h"

#include <cglm/cglm.h>
#include <glad/glad.h>

// this needs to be included after glad.h
#include <SDL_opengl.h>

SDL_Window* window;
SDL_GLContext context;
GLuint screen_buffer_vao, screen_buffer_ebo;
GLuint screen_texture, palette_texture;

GLuint shader_program_2d;
GLuint shader_program_2d_pp;
GLuint shader_program_3d;
GLuint framebuffer_id, framebuffer_texture = 0;
unsigned int rbo;
uint8_t gl_palette[4 * 256]; // RGBA

int window_width, window_height;

struct {
    GLuint textureSample;
    GLuint pixels, palette;
    GLuint model, view, projection;
    GLuint clip_plane_count;
    GLuint clip_planes[6];
} uniforms_3d;

struct {
    GLuint pixels, palette;
} uniforms_2d;

GLuint CreateShaderFromFile(const char* filename, const char* fallback, GLenum type) {
    int success;

    GLuint res = glCreateShader(type);

    FILE* f = fopen(filename, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);
        char* file_bytes = malloc(fsize + 1);
        fread(file_bytes, fsize, 1, f);
        fclose(f);
        file_bytes[fsize] = 0;
        const GLchar* sources[] = { file_bytes };
        glShaderSource(res, 1, sources, NULL);
    } else {
        const GLchar* sources[] = { fallback };
        glShaderSource(res, 1, sources, NULL);
    }

    glCompileShader(res);
    glGetShaderiv(res, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log_buffer[1024];
        glGetShaderInfoLog(res, 1024, NULL, log_buffer);
        LOG_PANIC("shader %s failed to compile: %s", filename, log_buffer);
    }
    return res;
}

GLuint CreateShaderProgram(const char* vertex_file, const char* fragment_file, const char* vertex_fallback, const char* fragment_fallback) {
    GLuint program = glCreateProgram();
    GLuint v_shader, f_shader;

    v_shader = CreateShaderFromFile(vertex_file, vertex_fallback, GL_VERTEX_SHADER);
    if (!v_shader)
        return 0;
    glAttachShader(program, v_shader);

    f_shader = CreateShaderFromFile(fragment_file, fragment_fallback, GL_FRAGMENT_SHADER);
    if (!f_shader)
        return 0;
    glAttachShader(program, f_shader);

    glLinkProgram(program);

    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    GLint link_ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        char log_buffer[1024];
        glGetShaderInfoLog(program, 1024, NULL, log_buffer);
        LOG_PANIC("shader program %s:%s failed to link: %s", vertex_file, fragment_file, log_buffer);
    }
    return program;
}

void LoadShaders() {
    shader_program_2d = CreateShaderProgram("vertex_shader_2d.glsl", "fragment_shader_2d.glsl", vs_2d, fs_2d);
    glUseProgram(shader_program_2d);
    uniforms_2d.pixels = glGetUniformLocation(shader_program_2d, "pixels");
    uniforms_2d.palette = glGetUniformLocation(shader_program_2d, "palette");

    // bind the uniform samplers to texture units:
    glUniform1i(uniforms_2d.pixels, 0);
    glUniform1i(uniforms_2d.palette, 1);

    shader_program_2d_pp = CreateShaderProgram("vertex_shader_postprocess.glsl", "fragment_shader_postprocess.glsl", vs_2d, fs_postprocess);

    shader_program_3d = CreateShaderProgram("vertex_shader_3d.glsl", "fragment_shader_3d.glsl", vs_3d, fs_3d);
    glUseProgram(shader_program_3d);
    uniforms_3d.clip_plane_count = glGetUniformLocation(shader_program_3d, "clip_plane_count");
    for (int i = 0; i < 6; i++) {
        char name[32];
        sprintf(name, "clip_planes[%d]", i);
        uniforms_3d.clip_planes[i] = glGetUniformLocation(shader_program_3d, name);
        LOG_DEBUG("%d", uniforms_3d.clip_planes[i]);
    }
    uniforms_3d.model = glGetUniformLocation(shader_program_3d, "model");
    uniforms_3d.palette = glGetUniformLocation(shader_program_3d, "palette");
    uniforms_3d.pixels = glGetUniformLocation(shader_program_3d, "pixels");
    uniforms_3d.projection = glGetUniformLocation(shader_program_3d, "projection");
    uniforms_3d.textureSample = glGetUniformLocation(shader_program_3d, "textureSample");
    uniforms_3d.view = glGetUniformLocation(shader_program_3d, "view");

    // bind the uniform samplers to texture units:
    glUniform1i(uniforms_2d.pixels, 0);
    glUniform1i(uniforms_2d.palette, 1);
}

void GLRenderer_CreateWindow(char* title, int width, int height) {
    window_width = width;
    window_height = height;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_PANIC("SDL_INIT_VIDEO error: %s", SDL_GetError());
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
        LOG_PANIC("Failed to set SDL_GL_CONTEXT_PROFILE_MASK attribute. %s", SDL_GetError());
    };
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetSwapInterval(1);

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);

    if (!window) {
        LOG_PANIC("Failed to create window");
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    context = SDL_GL_CreateContext(window);
    if (!context) {
        LOG_PANIC("Failed to call SDL_GL_CreateContext. %s", SDL_GetError());
    }

    // Load GL extensions using glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        LOG_PANIC("Failed to initialize the OpenGL context with GLAD.");
        exit(1);
    }

    LoadShaders();

    float vertices[] = {
        // positions          // colors           // texture coords
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top right
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // bottom right
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom left
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &screen_texture);
    glGenTextures(1, &palette_texture);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glGenFramebuffers(1, &framebuffer_id);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glGenTextures(1, &framebuffer_texture);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_PANIC("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
}

extern br_v1db_state v1db;

void GLRenderer_BeginScene(br_actor* camera, br_pixelmap* colour_buffer) {

    glViewport(colour_buffer->base_x * 2, colour_buffer->base_y * 2, colour_buffer->width * 2, colour_buffer->height * 2);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader_program_3d);

    int enabled_clip_planes = 0;
    for (int i = 0; i < v1db.enabled_clip_planes.max; i++) {
        if (!v1db.enabled_clip_planes.enabled || !v1db.enabled_clip_planes.enabled[i]) {
            continue;
        }
        br_vector4* v4 = v1db.enabled_clip_planes.enabled[i]->type_data;
        glUniform4f(uniforms_3d.clip_planes[enabled_clip_planes], v4->v[0], v4->v[1], v4->v[2], v4->v[3]);
        enabled_clip_planes++;
    }

    glUniform1i(uniforms_3d.clip_plane_count, enabled_clip_planes);

    if (gDebugCamera_active) {
        float m2[4][4];
        memcpy(m2, DebugCamera_View(), sizeof(float) * 16);
        // LOG_DEBUG("%f, %f, %f, %f", m2[0][0], m2[0][1], m2[0][2], m2[0][3]);
        // LOG_DEBUG("%f, %f, %f, %f", m2[1][0], m2[1][1], m2[1][2], m2[1][3]);
        // LOG_DEBUG("%f, %f, %f, %f", m2[2][0], m2[2][1], m2[2][2], m2[2][3]);
        // LOG_DEBUG("%f, %f, %f, %f", m2[3][0], m2[3][1], m2[3][2], m2[3][3]);
        glUniformMatrix4fv(uniforms_3d.view, 1, GL_FALSE, (float*)&m2[0]);
    } else {
        mat4 cam_matrix = {
            { camera->t.t.mat.m[0][0], camera->t.t.mat.m[0][1], camera->t.t.mat.m[0][2], 0 },
            { camera->t.t.mat.m[1][0], camera->t.t.mat.m[1][1], camera->t.t.mat.m[1][2], 0 },
            { camera->t.t.mat.m[2][0], camera->t.t.mat.m[2][1], camera->t.t.mat.m[2][2], 0 },
            { camera->t.t.mat.m[3][0], camera->t.t.mat.m[3][1], camera->t.t.mat.m[3][2], 1 }
        };
        mat4 cam_matrix_inv;
        glm_mat4_inv(cam_matrix, cam_matrix_inv);
        glUniformMatrix4fv(uniforms_3d.view, 1, GL_FALSE, &cam_matrix_inv[0][0]);
    }
    float* proj = DebugCamera_Projection();
    glUniformMatrix4fv(uniforms_3d.projection, 1, GL_FALSE, proj);

    DebugCamera_Update();

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer_EndScene() {
}

void GLRenderer_RenderFramebuffer() {

    glViewport(0, 0, window_width, window_height);
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glUseProgram(shader_program_2d_pp);

    glBindVertexArray(screen_buffer_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_buffer_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // this wasn't here before...
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderer_RenderFullScreenQuad(uint8_t* screen_buffer, int width, int height) {

    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    GLenum e = glGetError();
    if (e != 0) {
        LOG_DEBUG("error %d", e);
    }

    // e = glGetError();
    // LOG_DEBUG("error1x %d, text %d", e, screen_texture);

    glUseProgram(shader_program_2d);
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    // e = glGetError();
    // LOG_DEBUG("error2 %d, text %d", e, screen_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // e = glGetError();
    // LOG_DEBUG("error3 %d", e);

    //    memset(screen_buffer, 0x30, 320 * 200);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, screen_buffer);
    // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_R8, GL_UNSIGNED_BYTE, screen_buffer);
    e = glGetError();
    LOG_DEBUG("error4 %d", e);
    glBindVertexArray(screen_buffer_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_buffer_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLRenderer_Swap() {
    SDL_GL_SwapWindow(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void build_model(br_model* model) {
    tStored_model_context* ctx;
    v11model* v11;

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
    GLuint vbo_id;
    glGenBuffers(1, &vbo_id);
    glGenBuffers(1, &ctx->ebo_id);

    // Vertices }
    glBindVertexArray(ctx->vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
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
}

void GLRenderer_RenderModel(br_model* model, br_matrix34 model_matrix) {
    tStored_model_context* ctx;
    ctx = model->stored;
    v11model* v11 = model->prepared;

    if (v11 == NULL) {
        // LOG_WARN("No model prepared for %s", model->identifier);
        return;
    }

    if (ctx == NULL) {
        build_model(model);
        ctx = model->stored;
        // DebugCamera_SetPosition(model_matrix.m[3][0], model_matrix.m[3][1], model_matrix.m[3][2]);
    }

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader_program_3d);

    GLfloat m[16] = {
        model_matrix.m[0][0], model_matrix.m[0][1], model_matrix.m[0][2], 0,
        model_matrix.m[1][0], model_matrix.m[1][1], model_matrix.m[1][2], 0,
        model_matrix.m[2][0], model_matrix.m[2][1], model_matrix.m[2][2], 0,
        model_matrix.m[3][0], model_matrix.m[3][1], model_matrix.m[3][2], 1
    };

    glUniformMatrix4fv(uniforms_3d.model, 1, GL_FALSE, m);

    glBindVertexArray(ctx->vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->ebo_id);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    int element_index = 0;

    for (int g = 0; g < v11->ngroups; g++) {
        tStored_material* mat = (tStored_material*)v11->groups[g].stored;
        if (mat && mat->texture) {
            glBindTexture(GL_TEXTURE_2D, mat->texture->id);
            glUniform1i(uniforms_3d.textureSample, 1);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform1i(uniforms_3d.textureSample, 0);
        }

        glDrawElements(GL_TRIANGLES, v11->groups[g].nfaces * 3, GL_UNSIGNED_INT, (void*)(element_index * sizeof(int)));
        element_index += v11->groups[g].nfaces * 3;
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    // element_index = 0;
    // for (int g = 0; g < v11->ngroups; g++) {
    //     glDrawElements(GL_TRIANGLES, v11->groups[g].nfaces * 3, GL_UNSIGNED_INT, (void*)(element_index * sizeof(int)));
    //     element_index += v11->groups[g].nfaces * 3;
    // }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLRenderer_BufferMaterial(br_material* mat) {
    if (!mat->stored) {
        if (!mat->colour_map) {
        }
        if (mat->colour_map) {
            GLRenderer_BufferTexture(mat->colour_map);
            if (mat->colour_map->stored) {
                tStored_material* stored = malloc(sizeof(tStored_material));
                stored->texture = mat->colour_map->stored;
                mat->stored = stored;
            }
        }
    }
}

void GLRenderer_BufferTexture(br_pixelmap* pm) {
    if (!pm->stored) {
        // sometimes the pixelmap has row_bytes > width. OpenGL expects linear pixels, so flatten it out
        uint8_t* linear_pixels = malloc(sizeof(uint8_t) * pm->width * pm->height);
        uint8_t* original_pixels = pm->pixels;
        for (int y = 0; y < pm->height; y++) {
            for (int x = 0; x < pm->width; x++) {
                linear_pixels[y * pm->width + x] = original_pixels[y * pm->row_bytes + x];
            }
        }

        tStored_pixelmap* stored = malloc(sizeof(tStored_pixelmap));
        glGenTextures(1, &stored->id);
        glBindTexture(GL_TEXTURE_2D, stored->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, pm->width, pm->height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, linear_pixels);
        free(linear_pixels);
        pm->stored = stored;
    }
}

void Harness_GLRenderer_RenderCube(float col, float x, float y, float z) {

    // // Enable depth test
    // glEnable(GL_DEPTH_TEST);
    // glUseProgram(shader_program_3d);

    // mat4 MVP, model, view, proj;
    // glm_perspective(glm_rad(45), 4.0f / 3.0f, 0.1f, 10000.f, proj);
    // glm_lookat((vec3){ -80, 25, -60 }, (vec3){ -67.514061, 20.485441, -52.174110 }, (vec3){ 0, 1, 0 }, view);

    // glm_mat4_identity(model);
    // //glm_rotate_y(model, ang, model);
    // ang += 0.01f;
    // glm_translate(model, (vec3){ x, y, z });
    // glm_mat4_mulN((mat4*[]){ &proj, &view, &model }, 3, MVP);
    // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // //const float c[3] = { 1, 1, 1 };

    // glUniform3f(ColorID, col, col, col);

    // glBindVertexArray(cube_vao);

    // // Draw the triangle !
    // glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
    // glBindVertexArray(0);
}
