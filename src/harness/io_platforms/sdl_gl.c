#include <glad/glad.h>

// this needs to be included after glad.h
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_rect.h>

#include "../renderers/gl/gl_renderer.h"
#include "../renderers/renderer.h"
#include "harness/trace.h"

// Errol's keymap
int keymap[123] = {
    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_LALT,
    SDL_SCANCODE_LCTRL,
    -1,
    SDL_SCANCODE_CAPSLOCK,
    SDL_SCANCODE_RSHIFT,
    SDL_SCANCODE_RALT,
    SDL_SCANCODE_RCTRL,
    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_LALT,
    SDL_SCANCODE_LCTRL,
    SDL_SCANCODE_0,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_5,
    SDL_SCANCODE_6,
    SDL_SCANCODE_7,
    SDL_SCANCODE_8,
    SDL_SCANCODE_9,
    SDL_SCANCODE_A,
    SDL_SCANCODE_B,
    SDL_SCANCODE_C,
    SDL_SCANCODE_D,
    SDL_SCANCODE_E,
    SDL_SCANCODE_F,
    SDL_SCANCODE_G,
    SDL_SCANCODE_H,
    SDL_SCANCODE_I,
    SDL_SCANCODE_J,
    SDL_SCANCODE_K,
    SDL_SCANCODE_L,
    SDL_SCANCODE_M,
    SDL_SCANCODE_N,
    SDL_SCANCODE_O,
    SDL_SCANCODE_P,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_R,
    SDL_SCANCODE_S,
    SDL_SCANCODE_T,
    SDL_SCANCODE_U,
    SDL_SCANCODE_V,
    SDL_SCANCODE_W,
    SDL_SCANCODE_X,
    SDL_SCANCODE_Y,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_GRAVE,
    SDL_SCANCODE_MINUS,
    SDL_SCANCODE_EQUALS,
    SDL_SCANCODE_BACKSPACE,
    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_KP_ENTER,
    SDL_SCANCODE_TAB,
    SDL_SCANCODE_KP_DIVIDE,
    SDL_SCANCODE_SLASH,
    SDL_SCANCODE_SEMICOLON,
    SDL_SCANCODE_APOSTROPHE,
    SDL_SCANCODE_PERIOD,
    SDL_SCANCODE_COMMA,
    SDL_SCANCODE_LEFTBRACKET,
    SDL_SCANCODE_RIGHTBRACKET,
    -1,
    SDL_SCANCODE_ESCAPE,
    SDL_SCANCODE_INSERT,
    SDL_SCANCODE_DELETE,
    SDL_SCANCODE_HOME,
    SDL_SCANCODE_END,
    SDL_SCANCODE_PAGEUP,
    SDL_SCANCODE_PAGEDOWN,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_UP,
    SDL_SCANCODE_DOWN,
    -1,
    SDL_SCANCODE_KP_DIVIDE,
    SDL_SCANCODE_KP_MULTIPLY,
    SDL_SCANCODE_KP_MINUS,
    SDL_SCANCODE_KP_PLUS,
    SDL_SCANCODE_KP_PERIOD,
    SDL_SCANCODE_KP_EQUALS,
    SDL_SCANCODE_KP_0,
    SDL_SCANCODE_KP_1,
    SDL_SCANCODE_KP_2,
    SDL_SCANCODE_KP_3,
    SDL_SCANCODE_KP_4,
    SDL_SCANCODE_KP_5,
    SDL_SCANCODE_KP_6,
    SDL_SCANCODE_KP_7,
    SDL_SCANCODE_KP_8,
    SDL_SCANCODE_KP_9,
    SDL_SCANCODE_F1,
    SDL_SCANCODE_F2,
    SDL_SCANCODE_F3,
    SDL_SCANCODE_F4,
    SDL_SCANCODE_F5,
    SDL_SCANCODE_F6,
    SDL_SCANCODE_F7,
    SDL_SCANCODE_F8,
    SDL_SCANCODE_F9,
    SDL_SCANCODE_F10,
    SDL_SCANCODE_F11,
    SDL_SCANCODE_F12,
    SDL_SCANCODE_PRINTSCREEN,
    -1,
    SDL_SCANCODE_PAUSE,
    SDL_SCANCODE_SPACE
};

SDL_Window* window;
SDL_GLContext context;
uint8_t sdl_key_state[256];
SDL_FPoint sdl_window_scale;

tRenderer gl_renderer = {
    GLRenderer_Init,
    GLRenderer_BeginScene,
    GLRenderer_EndScene,
    GLRenderer_SetPalette,
    GLRenderer_FullScreenQuad,
    GLRenderer_Model,
    GLRenderer_ClearBuffers,
    GLRenderer_BufferTexture,
    GLRenderer_BufferMaterial,
    GLRenderer_FlushBuffers
};

tRenderer* Window_Create(char* title, int width, int height, int pRender_width, int pRender_height) {
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

    if (window == NULL) {
        LOG_PANIC("Failed to create window");
    }

    // Don't grab the mouse when a debugger is present
    if (!OS_IsDebuggerPresent()) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    sdl_window_scale.x = ((float)pRender_width) / width;
    sdl_window_scale.y = ((float)pRender_height) / height;

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

    gl_renderer.Init(width, height, pRender_width, pRender_height);
    return &gl_renderer;
}

void Window_PollEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (event.key.keysym.scancode > 122) {
                LOG_WARN("unexpected scan code %d", event.key.keysym.scancode);
                return;
            }
            if (event.key.type == SDL_KEYDOWN) {
                sdl_key_state[event.key.keysym.scancode] = 1;
                // LOG_DEBUG("key %d", key->keysym.scancode);
            } else {
                sdl_key_state[event.key.keysym.scancode] = 0;
            }

            break;

        case SDL_QUIT:
            LOG_PANIC("QuitGame");
            break;
        }
    }
}

void Window_Swap(int delay_ms_after_swap) {
    SDL_GL_SwapWindow(window);

    if (delay_ms_after_swap != 0) {
        SDL_Delay(delay_ms_after_swap);
    }
}

int* Input_GetKeyMap() {
    return (int*)keymap;
}

int Input_IsKeyDown(unsigned char scan_code) {
    return sdl_key_state[scan_code];
}

void Input_GetMousePosition(int* pX, int* pY) {
    SDL_GetMouseState(pX, pY);
    *pX *= sdl_window_scale.x;
    *pY *= sdl_window_scale.y;
}

void Input_GetMouseButtons(int* pButton1, int* pButton2) {
    int state = SDL_GetMouseState(NULL, NULL);
    *pButton1 = state & SDL_BUTTON_LMASK;
    *pButton2 = state & SDL_BUTTON_RMASK;
}
