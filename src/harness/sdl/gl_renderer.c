#include "gl_renderer.h"
#include "brender/brender.h"
#include "cameras/debug_camera.h"
#include "gl_renderer_shaders.h"
#include "harness.h"
#include "harness/trace.h"

#include <cglm/cglm.h>
#include <glad/glad.h>

// this needs to be included after glad.h
#include <SDL_opengl.h>

typedef struct tStored_model_context {
    GLuint vao_id, ebo_id;
} tStored_model_context;

typedef struct tStored_pixelmap {
    GLuint id;
} tStored_pixelmap;

typedef struct tStored_material {
    tStored_pixelmap* texture;
} tStored_material;

SDL_Window* window;
SDL_GLContext context;
GLuint screen_buffer_vao, screen_buffer_ebo;
GLuint screen_texture;
GLuint shader_program_2d;
GLuint shader_program_2d_trans;
GLuint shader_program_3d;

GLuint shader3d_texture_location;

int window_width, window_height;

void CompileShader(GLuint shader_id, const GLchar* source) {
    int success;
    char log[512];
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, log);
        LOG_PANIC("shader %d failed. %s", shader_id, log);
    }
}

void LoadShaders() {
    GLuint vs, fs;

    vs = glCreateShader(GL_VERTEX_SHADER);
    CompileShader(vs, vs_2d);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(fs, fs_2d);

    shader_program_2d = glCreateProgram();
    glAttachShader(shader_program_2d, vs);
    glAttachShader(shader_program_2d, fs);
    glLinkProgram(shader_program_2d);

    glDeleteShader(fs);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(fs, fs_2d_trans);
    shader_program_2d_trans = glCreateProgram();
    glAttachShader(shader_program_2d_trans, vs);
    glAttachShader(shader_program_2d_trans, fs);
    glLinkProgram(shader_program_2d_trans);
    glDeleteShader(vs);
    glDeleteShader(fs);

    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(fs, fs_3d);
    CompileShader(vs, vs_3d);
    shader_program_3d = glCreateProgram();
    glAttachShader(shader_program_3d, vs);
    glAttachShader(shader_program_3d, fs);
    glLinkProgram(shader_program_3d);

    glDeleteShader(vs);
    glDeleteShader(fs);

    //shader3d_texture_location = glGetUniformLocation(shader_program_3d, "texture");
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

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);

    if (!window) {
        LOG_PANIC("Failed to create window");
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);

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
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
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

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

void GLRenderer_BeginFrame(br_actor* camera, br_pixelmap* colour_buffer) {

    glViewport(colour_buffer->base_x * 2, colour_buffer->base_y * 2, colour_buffer->width * 2, colour_buffer->height * 2);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader_program_3d);

    GLuint lightpos_u = glGetUniformLocation(shader_program_3d, "lightPos");
    GLuint lightcolor_u = glGetUniformLocation(shader_program_3d, "lightColor");
    GLuint objectcolor_u = glGetUniformLocation(shader_program_3d, "objectColor");

    glUniform3f(lightpos_u, -0.0f, -1.0f, -0.3f);
    //glUniform3f(lightcolor_u, 0.97f, 0.84f, 0.11f);
    glUniform3f(lightcolor_u, 1, 1, 1);
    glUniform3f(objectcolor_u, 1.0f, 1.0f, 1.0f);

    GLuint view_u = glGetUniformLocation(shader_program_3d, "view");
    GLuint projection_u = glGetUniformLocation(shader_program_3d, "projection");

    if (gDebugCamera_active) {
        float m2[4][4];
        memcpy(m2, DebugCamera_View(), sizeof(float) * 16);
        // LOG_DEBUG("%f, %f, %f, %f", m2[0][0], m2[0][1], m2[0][2], m2[0][3]);
        // LOG_DEBUG("%f, %f, %f, %f", m2[1][0], m2[1][1], m2[1][2], m2[1][3]);
        // LOG_DEBUG("%f, %f, %f, %f", m2[2][0], m2[2][1], m2[2][2], m2[2][3]);
        // LOG_DEBUG("%f, %f, %f, %f", m2[3][0], m2[3][1], m2[3][2], m2[3][3]);
        glUniformMatrix4fv(view_u, 1, GL_FALSE, (float*)&m2[0]);
    } else {
        mat4 cam_matrix = {
            { camera->t.t.mat.m[0][0], camera->t.t.mat.m[0][1], camera->t.t.mat.m[0][2], 0 },
            { camera->t.t.mat.m[1][0], camera->t.t.mat.m[1][1], camera->t.t.mat.m[1][2], 0 },
            { camera->t.t.mat.m[2][0], camera->t.t.mat.m[2][1], camera->t.t.mat.m[2][2], 0 },
            { camera->t.t.mat.m[3][0], camera->t.t.mat.m[3][1], camera->t.t.mat.m[3][2], 1 }
        };
        mat4 cam_matrix_inv;
        glm_mat4_inv(cam_matrix, cam_matrix_inv);
        glUniformMatrix4fv(view_u, 1, GL_FALSE, &cam_matrix_inv[0][0]);
    }
    float* proj = DebugCamera_Projection();
    glUniformMatrix4fv(projection_u, 1, GL_FALSE, proj);

    DebugCamera_Update();
}

void GLRenderer_EndFrame() {
    glViewport(0, 0, window_width, window_height);
}

void GLRenderer_RenderFullScreenQuad(uint32_t* screen_buffer, int transparent) {

    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // TODO: remove fixed 320x200
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 320, 200, 0, GL_BGRA, GL_UNSIGNED_BYTE, screen_buffer);

    if (transparent) {
        glUseProgram(shader_program_2d_trans);
    } else {
        glUseProgram(shader_program_2d);
    }
    glBindVertexArray(screen_buffer_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_buffer_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLRenderer_Swap() {
    SDL_GL_SwapWindow(window);
    // glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void build_model(br_model* model) {
    tStored_model_context* ctx;
    v11model* v11;

    v11 = model->prepared;
    ctx = malloc(sizeof(tStored_model_context));

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

    LOG_DEBUG("model prepared for %s", model->identifier);

    if (v11 == NULL) {
        //LOG_WARN("No model prepared for %s", model->identifier);
        return;
    }

    if (ctx == NULL) {
        build_model(model);
        ctx = model->stored;
        //DebugCamera_SetPosition(model_matrix.m[3][0], model_matrix.m[3][1], model_matrix.m[3][2]);
    }

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader_program_3d);

    GLfloat m[16] = {
        model_matrix.m[0][0], model_matrix.m[0][1], model_matrix.m[0][2], 0,
        model_matrix.m[1][0], model_matrix.m[1][1], model_matrix.m[1][2], 0,
        model_matrix.m[2][0], model_matrix.m[2][1], model_matrix.m[2][2], 0,
        model_matrix.m[3][0], model_matrix.m[3][1], model_matrix.m[3][2], 1
    };

    GLuint model_u = glGetUniformLocation(shader_program_3d, "model");
    glUniformMatrix4fv(model_u, 1, GL_FALSE, m);

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
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
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
        LOG_DEBUG("trying to buffer mat");
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
        LOG_DEBUG("trying to buffer pix");
        uint32_t* full_color_texture = NULL;
        Harness_ConvertPalettedPixelmapTo32Bit(&full_color_texture, pm);
        if (full_color_texture == NULL) {
            return;
        }
        tStored_pixelmap* stored = malloc(sizeof(tStored_pixelmap));
        glGenTextures(1, &stored->id);
        glBindTexture(GL_TEXTURE_2D, stored->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pm->width, pm->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, full_color_texture);
        free(full_color_texture);
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
