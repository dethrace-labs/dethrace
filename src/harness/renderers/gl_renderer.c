#include "gl_renderer.h"
#include "gl_renderer_shaders.h"
#include "harness.h"

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

tRenderer OpenGLRenderer = {
    Harness_GLRenderer_GetWindowFlags,
    Harness_GLRenderer_Init,
    Harness_GLRenderer_RenderFrameBegin,
    Harness_GLRenderer_RenderFrameEnd,
    Harness_GLRenderer_RenderScreenBuffer,
    Harness_GLRenderer_Swap
};

SDL_GLContext context;
GLuint VBO, VAO, EBO;
GLuint screen_texture;
GLuint shader_program_2d;
GLuint shader_program_2d_trans;
GLuint shader_program_3d;

GLuint cube_vao;
GLuint cube_vertexbuffer;
GLuint cube_colorbuffer;

GLuint MatrixID;

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
// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f, 0.771f, 0.014f,
    0.609f, 0.115f, 0.436f,
    0.327f, 0.483f, 0.844f,
    0.822f, 0.569f, 0.201f,
    0.435f, 0.602f, 0.223f,
    0.310f, 0.747f, 0.185f,
    0.597f, 0.770f, 0.761f,
    0.559f, 0.436f, 0.730f,
    0.359f, 0.583f, 0.152f,
    0.483f, 0.596f, 0.789f,
    0.559f, 0.861f, 0.639f,
    0.195f, 0.548f, 0.859f,
    0.014f, 0.184f, 0.576f,
    0.771f, 0.328f, 0.970f,
    0.406f, 0.615f, 0.116f,
    0.676f, 0.977f, 0.133f,
    0.971f, 0.572f, 0.833f,
    0.140f, 0.616f, 0.489f,
    0.997f, 0.513f, 0.064f,
    0.945f, 0.719f, 0.592f,
    0.543f, 0.021f, 0.978f,
    0.279f, 0.317f, 0.505f,
    0.167f, 0.620f, 0.077f,
    0.347f, 0.857f, 0.137f,
    0.055f, 0.953f, 0.042f,
    0.714f, 0.505f, 0.345f,
    0.783f, 0.290f, 0.734f,
    0.722f, 0.645f, 0.174f,
    0.302f, 0.455f, 0.848f,
    0.225f, 0.587f, 0.040f,
    0.517f, 0.713f, 0.338f,
    0.053f, 0.959f, 0.120f,
    0.393f, 0.621f, 0.362f,
    0.673f, 0.211f, 0.457f,
    0.820f, 0.883f, 0.371f,
    0.982f, 0.099f, 0.879f
};

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

    glGenBuffers(1, &cube_colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cube_colorbuffer);
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, // attribute. No particular reason for 1, but must match the layout in the shader.
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    MatrixID = glGetUniformLocation(shader_program_3d, "MVP");
    LOG_DEBUG("found matrixid %d", MatrixID);

    glBindVertexArray(0);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

float ang = 0;
void Harness_GLRenderer_RenderFrameBegin() {

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader_program_3d);

    mat4 MVP, model, view, proj;
    glm_perspective(glm_rad(45), 4.0f / 3.0f, 0.1f, 100.f, proj);
    glm_lookat((vec3){ -70, 25, -60 }, (vec3){ -67, 20, -52 }, (vec3){ 0, 1, 0 }, view);

    glm_mat4_identity(model);
    //glm_rotate_y(model, ang, model);
    ang += 0.01f;
    glm_translate(model, (vec3){ -67.514, 20.345, -52.174 });
    glm_mat4_mulN((mat4*[]){ &proj, &view, &model }, 3, MVP);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glBindVertexArray(cube_vao);

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
    glBindVertexArray(0);
}

void Harness_GLRenderer_RenderFrameEnd() {
}

void Harness_GLRenderer_RenderScreenBuffer(uint32_t* screen_buffer, int transparent) {

    //return;

    glDisable(GL_DEPTH_TEST);

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
}