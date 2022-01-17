#include "cameras/debug_camera.h"
#include <SDL.h>

mat4 view, projection;
vec3 cam_pos = { 0, 0, 0 };
vec3 cam_front = { 0, 0, -1 };
vec3 cam_up = { 0, 1, 0 };
float cam_speed = 0.5f;

float vc[4][4] = {
    { 0.45397636, 0.0, 0.89101368, 0 }, { 0.1117821, 0.99209929, -0.056953594, 0 }, { -0.88397402, 0.12545498, 0.45038962, 0 }, { -69.239441, 20.735441, -52.417336, 1 }
};

float lastX = 400, lastY = 300;
int firstMouse = 1;
float yaw = 0, pitch = 0;

int gDebugCamera_active = 0;

void DebugCamera_Update() {
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
    if (state[SDL_SCANCODE_F12]) {
        gDebugCamera_active = !gDebugCamera_active;
        // if (!gDebugCamera_active) {
        //     printf("DEBUGCAMERAACTIVE\n");
        //     gDebugCamera_active = 1;
        // }
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

float* DebugCamera_Projection() {
    glm_perspective(glm_rad(55.55), 320.0f / 200.0f /*4.0f / 3.0f*/, 0.1f, 10000.f, projection);
    return (float*)&projection;
}

float* DebugCamera_View() {
    vec3 look;
    glm_vec3_add(cam_pos, cam_front, look);
    glm_lookat(cam_pos, look, cam_up, view);
    return (float*)&view;
}

void DebugCamera_SetPosition(float x, float y, float z) {
    if (cam_pos[0] == 0) {
        cam_pos[0] = x;
        cam_pos[1] = y;
        cam_pos[2] = z;
    }
}
