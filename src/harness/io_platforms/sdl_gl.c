#include <glad/glad.h>

// this needs to be included after glad.h
#include <SDL.h>
#include <SDL_opengl.h>

#include "../renderers/gl/gl_renderer.h"
#include "../renderers/renderer.h"
#include "harness/trace.h"

#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))

int scancode_map[123];
const int scancodes_dethrace2sdl[123] = {
    -1, //   0 (LSHIFT || RSHIFT)
    -1, //   1 (LALT || RALT)
    -1, //   2 (LCTRL || RCTRL)
    -1, //   3 (LCTRL || RCTRL)
    SDL_SCANCODE_CAPSLOCK, //   4
    SDL_SCANCODE_RSHIFT, //   5
    SDL_SCANCODE_RALT, //   6
    SDL_SCANCODE_RCTRL, //   7
    SDL_SCANCODE_LSHIFT, //   8
    SDL_SCANCODE_LALT, //   9
    SDL_SCANCODE_LCTRL, //  10
    SDL_SCANCODE_0, //  11
    SDL_SCANCODE_1, //  12
    SDL_SCANCODE_2, //  13
    SDL_SCANCODE_3, //  14
    SDL_SCANCODE_4, //  15
    SDL_SCANCODE_5, //  16
    SDL_SCANCODE_6, //  17
    SDL_SCANCODE_7, //  18
    SDL_SCANCODE_8, //  19
    SDL_SCANCODE_9, //  20
    SDL_SCANCODE_A, //  21
    SDL_SCANCODE_B, //  22
    SDL_SCANCODE_C, //  23
    SDL_SCANCODE_D, //  24
    SDL_SCANCODE_E, //  25
    SDL_SCANCODE_F, //  26
    SDL_SCANCODE_G, //  27
    SDL_SCANCODE_H, //  28
    SDL_SCANCODE_I, //  29
    SDL_SCANCODE_J, //  30
    SDL_SCANCODE_K, //  31
    SDL_SCANCODE_L, //  32
    SDL_SCANCODE_M, //  33
    SDL_SCANCODE_N, //  34
    SDL_SCANCODE_O, //  35
    SDL_SCANCODE_P, //  36
    SDL_SCANCODE_Q, //  37
    SDL_SCANCODE_R, //  38
    SDL_SCANCODE_S, //  39
    SDL_SCANCODE_T, //  40
    SDL_SCANCODE_U, //  41
    SDL_SCANCODE_V, //  42
    SDL_SCANCODE_W, //  43
    SDL_SCANCODE_X, //  44
    SDL_SCANCODE_Y, //  45
    SDL_SCANCODE_Z, //  46
    SDL_SCANCODE_GRAVE, //  47
    SDL_SCANCODE_MINUS, //  48
    SDL_SCANCODE_EQUALS, //  49
    SDL_SCANCODE_BACKSPACE, //  50
    SDL_SCANCODE_RETURN, //  51
    SDL_SCANCODE_KP_ENTER, //  52
    SDL_SCANCODE_TAB, //  53
    SDL_SCANCODE_SLASH, //  54
    -1, //  55
    SDL_SCANCODE_SEMICOLON, //  56
    SDL_SCANCODE_APOSTROPHE, //  57
    SDL_SCANCODE_PERIOD, //  58
    SDL_SCANCODE_COMMA, //  59
    SDL_SCANCODE_LEFTBRACKET, //  60
    SDL_SCANCODE_RIGHTBRACKET, //  61
    SDL_SCANCODE_BACKSLASH, //  62
    SDL_SCANCODE_ESCAPE, //  63
    SDL_SCANCODE_INSERT, //  64
    SDL_SCANCODE_DELETE, //  65
    SDL_SCANCODE_HOME, //  66
    SDL_SCANCODE_END, //  67
    SDL_SCANCODE_PAGEUP, //  68
    SDL_SCANCODE_PAGEDOWN, //  69
    SDL_SCANCODE_LEFT, //  70
    SDL_SCANCODE_RIGHT, //  71
    SDL_SCANCODE_UP, //  72
    SDL_SCANCODE_DOWN, //  73
    SDL_SCANCODE_NUMLOCKCLEAR, //  74
    SDL_SCANCODE_KP_DIVIDE, //  75
    SDL_SCANCODE_KP_MULTIPLY, //  76
    SDL_SCANCODE_KP_MINUS, //  77
    SDL_SCANCODE_KP_PLUS, //  78
    SDL_SCANCODE_KP_PERIOD, //  79
    SDL_SCANCODE_KP_EQUALS, //  80
    SDL_SCANCODE_KP_0, //  81
    SDL_SCANCODE_KP_1, //  82
    SDL_SCANCODE_KP_2, //  83
    SDL_SCANCODE_KP_3, //  84
    SDL_SCANCODE_KP_4, //  85
    SDL_SCANCODE_KP_5, //  86
    SDL_SCANCODE_KP_6, //  87
    SDL_SCANCODE_KP_7, //  88
    SDL_SCANCODE_KP_8, //  89
    SDL_SCANCODE_KP_9, //  90
    SDL_SCANCODE_F1, //  91
    SDL_SCANCODE_F2, //  92
    SDL_SCANCODE_F3, //  93
    SDL_SCANCODE_F4, //  94
    SDL_SCANCODE_F5, //  95
    SDL_SCANCODE_F6, //  96
    SDL_SCANCODE_F7, //  97
    SDL_SCANCODE_F8, //  98
    SDL_SCANCODE_F9, //  99
    SDL_SCANCODE_F10, // 100
    SDL_SCANCODE_F11, // 101
    SDL_SCANCODE_F12, // 102
    SDL_SCANCODE_PRINTSCREEN, // 103
    SDL_SCANCODE_SCROLLLOCK, // 104
    SDL_SCANCODE_PAUSE, // 105
    SDL_SCANCODE_SPACE, // 106
};
int scancodes_sdl2dethrace[SDL_NUM_SCANCODES];

SDL_Window* window;
SDL_GLContext context;
uint8_t sdl_key_state[256];
struct {
    float x;
    float y;
} sdl_window_scale;

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
    GLRenderer_BufferModel,
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
    int dethrace_key;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            dethrace_key = scancodes_sdl2dethrace[event.key.keysym.scancode];
            if (dethrace_key == -1) {
                LOG_WARN("unexpected scan code %s (%d)", SDL_GetScancodeName(event.key.keysym.scancode), event.key.keysym.scancode);
                return;
            }
            if (event.key.type == SDL_KEYDOWN) {
                sdl_key_state[event.key.keysym.scancode] = 1;
            } else {
                sdl_key_state[event.key.keysym.scancode] = 0;
            }

            sdl_key_state[0] = sdl_key_state[scancodes_sdl2dethrace[SDL_SCANCODE_LSHIFT]] || sdl_key_state[scancodes_sdl2dethrace[SDL_SCANCODE_RSHIFT]];
            sdl_key_state[1] = sdl_key_state[scancodes_sdl2dethrace[SDL_SCANCODE_LALT]] || sdl_key_state[scancodes_sdl2dethrace[SDL_SCANCODE_RALT]];
            sdl_key_state[2] = sdl_key_state[scancodes_sdl2dethrace[SDL_SCANCODE_LCTRL]] || sdl_key_state[scancodes_sdl2dethrace[SDL_SCANCODE_RCTRL]];
            sdl_key_state[3] = sdl_key_state[2];
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

void Input_Init() {
    for (size_t i = 0; i < ARRAY_LEN(scancodes_sdl2dethrace); i++) {
        scancodes_sdl2dethrace[i] = -1;
    }
    for (size_t i = 0; i < ARRAY_LEN(scancodes_dethrace2sdl); i++) {
        if (scancodes_dethrace2sdl[i] != -1) {
            scancodes_sdl2dethrace[scancodes_dethrace2sdl[i]] = i;
        }
    }
    for (size_t i = 0; i < ARRAY_LEN(scancode_map); i++) {
        scancode_map[i] = i;
    }
}

int* Input_GetKeyMap() {
    return (int*)scancode_map;
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
