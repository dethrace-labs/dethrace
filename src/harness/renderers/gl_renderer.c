#include "gl_renderer.h"
#include "gl_renderer_shaders.h"
#include "harness_trace.h"

/*
#ifdef _WIN32
#include <windows.h>

#include <gl/gl.h>

#elif defined __unix__
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#elif defined __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#endif
*/

#include <glad/glad.h>

#include <SDL_opengl.h>
#include <cglm/cglm.h>

int Harness_GLRenderer_GetWindowFlags();
void Harness_GLRenderer_Init(SDL_Window* window);
void Harness_GLRenderer_Activate(SDL_Window* window);
void Harness_GLRenderer_RenderFrameBegin();
void Harness_GLRenderer_RenderFrameEnd();
void Harness_GLRenderer_RenderScreenBuffer(uint32_t* screen_buffer, int transparent);
void Harness_GLRenderer_Swap(SDL_Window* window);
void Harness_GLRenderer_RenderModel(br_model* model, br_matrix34 model_matrix);
void Harness_GLRenderer_RenderCube(float col, float x, float y, float z);

tRenderer OpenGLRenderer = {
    Harness_GLRenderer_GetWindowFlags,
    Harness_GLRenderer_Init,
    Harness_GLRenderer_RenderFrameBegin,
    Harness_GLRenderer_RenderFrameEnd,
    Harness_GLRenderer_RenderScreenBuffer,
    Harness_GLRenderer_Swap,
    Harness_GLRenderer_RenderModel,
    Harness_GLRenderer_RenderCube
};

typedef struct tModel_context {
    GLuint vao_id, ebo_id;
    int ngroups, nfaces;
} tModel_context;

SDL_GLContext context;
GLuint VBO, VAO, EBO;
GLuint screen_texture;
GLuint shader_program_2d;
GLuint shader_program_2d_trans;
GLuint shader_program_3d;

GLuint cube_vao;
GLuint cube_vertexbuffer;

GLuint MatrixID, ColorID;

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, -1.0f, // triangle 1 : begin
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f, -1.0f, // triangle 2 : begin
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f, // triangle 2 : end
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

float ang = 0;

vec3 cam_pos = { 0, 0, 0 };
vec3 lookat = { -67.514061, 20.485441, -52.174110 };
vec3 cam_front = { 0, 0, -1 };
vec3 cam_up = { 0, -1, 0 };
float cam_speed = 1.0f;

float lastX = 400, lastY = 300;
int firstMouse = 1;
float yaw = 0, pitch = 0;

int Harness_GLRenderer_GetWindowFlags() {
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
        LOG_PANIC("Failed to set SDL_GL_CONTEXT_PROFILE_MASK attribute. %s", SDL_GetError());
    };
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    return SDL_WINDOW_OPENGL;
}

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

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("0 gl error: %d\n", err);
    }

    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(fs, fs_3d);
    CompileShader(vs, vs_3d);
    shader_program_3d = glCreateProgram();
    glAttachShader(shader_program_3d, vs);
    glAttachShader(shader_program_3d, fs);
    glLinkProgram(shader_program_3d);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("1 gl error: %d\n", err);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Harness_GLRenderer_Init(SDL_Window* window) {
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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

    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);

    glGenBuffers(1, &cube_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vertexbuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, // attribute. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

void Harness_GLRenderer_RenderFrameBegin() {

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader_program_3d);

    GLuint lightpos_u = glGetUniformLocation(shader_program_3d, "lightPos");
    GLuint lightcolor_u = glGetUniformLocation(shader_program_3d, "lightColor");
    GLuint objectcolor_u = glGetUniformLocation(shader_program_3d, "objectColor");

    glUniform3f(lightpos_u, -0.2f, 1.0f, -0.3f);
    glUniform3f(lightcolor_u, 1.0f, 1.0f, 1.0f);
    glUniform3f(objectcolor_u, 1.0f, 1.0f, 1.0f);

    GLuint view_u = glGetUniformLocation(shader_program_3d, "view");
    GLuint projection_u = glGetUniformLocation(shader_program_3d, "projection");
    mat4 view, proj;
    glm_perspective(glm_rad(45), 4.0f / 3.0f, 0.1f, 10000.f, proj);
    vec3 look2;
    glm_vec3_add(cam_pos, cam_front, look2);
    glm_lookat(cam_pos, look2, (vec3){ 0, 1, 0 }, view);

    glUniformMatrix4fv(projection_u, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(view_u, 1, GL_FALSE, &view[0][0]);
}

void Harness_GLRenderer_RenderFrameEnd() {
}

void Harness_GLRenderer_RenderScreenBuffer(uint32_t* screen_buffer, int transparent) {

    //return;

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
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Harness_GLRenderer_Swap(SDL_Window* window) {
    SDL_GL_SwapWindow(window);
    // glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ang += 0.01f;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        vec3 s;
        glm_vec3_scale(cam_front, cam_speed, s);
        glm_vec3_add(cam_pos, s, cam_pos);
    }
    if (state[SDL_SCANCODE_DOWN]) {
        vec3 s;
        glm_vec3_scale(cam_front, -cam_speed, s);
        glm_vec3_add(cam_pos, s, cam_pos);
    }
    if (state[SDL_SCANCODE_LEFT]) {
        vec3 cr;
        glm_cross(cam_front, cam_up, cr);
        glm_normalize(cr);
        glm_vec3_scale(cr, cam_speed, cr);
        glm_vec3_add(cam_pos, cr, cam_pos);
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        vec3 cr;
        glm_cross(cam_front, cam_up, cr);
        glm_normalize(cr);
        glm_vec3_scale(cr, -cam_speed, cr);
        glm_vec3_add(cam_pos, cr, cam_pos);
    }

    int xpos, ypos;
    SDL_GetMouseState(&xpos, &ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.7f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction;
    direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    direction[1] = sin(glm_rad(pitch));
    direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_normalize_to(direction, cam_front);
}

void Harness_GLRenderer_RenderModel(br_model* model, br_matrix34 model_matrix) {
    tModel_context* ctx;
    ctx = model->harness_user_data;
    v11model* v11 = model->prepared;

    if (v11 == NULL) {
        LOG_WARN("No model prepared for %s", model->identifier);
        return;
    }

    if (ctx == NULL) {
        ctx = malloc(sizeof(tModel_context));

        GLuint vbo_id;

        int total_verts = 0, total_faces = 0;
        for (int i = 0; i < v11->ngroups; i++) {
            //LOG_DEBUG("group %d: %d, %d", i, v11->groups[i].nvertices, v11->groups[i].nfaces);
            total_verts += v11->groups[i].nvertices;
            total_faces += v11->groups[i].nfaces;
        }

        LOG_DEBUG("prepping model %d %d, %d %d", model->nvertices, model->nfaces, total_verts, total_faces);

        glGenVertexArrays(1, &ctx->vao_id);
        glGenBuffers(1, &vbo_id);
        glGenBuffers(1, &ctx->ebo_id);

        glBindVertexArray(ctx->vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

        int stride = 6;
        float* verts = malloc(sizeof(float) * stride * total_verts);
        int index = 0;
        for (int g = 0; g < v11->ngroups; g++) {
            for (int i = 0; i < v11->groups[g].nvertices; i++) {
                fmt_vertex* v = &v11->groups[g].vertices[i];
                verts[index++] = v->p.v[0];
                verts[index++] = v->p.v[1];
                verts[index++] = v->p.v[2];
                verts[index++] = v->n.v[0];
                verts[index++] = v->n.v[1];
                verts[index++] = v->n.v[2];
                //LOG_DEBUG("vert: %d: %f, %f, %f", i, model->vertices[i].p.v[0], model->vertices[i].p.v[1], model->vertices[i].p.v[2]);
            }
        }
        LOG_DEBUG("create 1 %d", index);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stride * total_verts, verts, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->ebo_id);
        unsigned int* ind = malloc(sizeof(int) * 3 * total_faces);
        int face_offset = 0;
        index = 0;
        for (int g = 0; g < v11->ngroups; g++) {
            for (int i = 0; i < v11->groups[g].nfaces; i++) {
                v11face* f = &v11->groups[g].faces[i];
                ind[index++] = f->vertices[0] + face_offset;
                ind[index++] = f->vertices[1] + face_offset;
                ind[index++] = f->vertices[2] + face_offset;
                //LOG_DEBUG("face: %d: %d, %d, %d", i, model->faces[i].vertices[0], model->faces[i].vertices[1], model->faces[i].vertices[2]);
                //LOG_DEBUG("face: [%d, %d, %d]", ind[i * 3], ind[i * 3 + 1], ind[i * 3 + 2]);
            }
            face_offset += v11->groups[g].nvertices;
        }
        LOG_DEBUG("create 2 %d", index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * total_faces, ind, GL_STATIC_DRAW);
        glBindVertexArray(0);

        free(verts);
        free(ind);

        ctx->nfaces = total_faces;
        model->harness_user_data = ctx;

        if (cam_pos[0] == 0) {
            cam_pos[0] = model_matrix.m[3][0];
            cam_pos[1] = model_matrix.m[3][1];
            cam_pos[2] = model_matrix.m[3][2];
        }
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

    ctx = model->harness_user_data;
    glBindVertexArray(ctx->vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->ebo_id);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    int element_index = 0;
    for (int g = 0; g < v11->ngroups; g++) {
        glDrawElements(GL_TRIANGLES, v11->groups[g].nfaces * 3, GL_UNSIGNED_INT, (void*)(element_index * sizeof(int)));
        element_index += v11->groups[g].nfaces * 3;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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