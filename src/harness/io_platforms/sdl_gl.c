#include <glad/glad.h>

// this needs to be included after glad.h
#include <SDL.h>
#include <SDL_opengl.h>

#include "../renderers/gl/gl_renderer.h"
#include "../renderers/renderer.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "sdl2_scancode_to_dinput.h"

#include "globvars.h"
#include "grafdata.h"
#include "pd/sys.h"

#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))

SDL_Window* window;
SDL_GLContext context;
uint8_t directinput_key_state[SDL_NUM_SCANCODES];

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
    GLRenderer_FlushBuffers,
    GLRenderer_GetRenderSize,
    GLRenderer_GetWindowSize,
    GLRenderer_SetWindowSize,
    GLRenderer_GetViewport
};

void* CreateWindowAndRenderer(char* title, int x, int y, int width, int height /*, int pRender_width, int pRender_height*/) {
    int pRender_width = width;
    int pRender_height = height;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_PANIC("SDL_INIT_VIDEO error: %s", SDL_GetError());
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
        LOG_PANIC("Failed to set SDL_GL_CONTEXT_PROFILE_MASK attribute. %s", SDL_GetError());
    };
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetSwapInterval(1);

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        LOG_PANIC("Failed to create window. %s", SDL_GetError());
    }

    sdl_window_scale.x = ((float)pRender_width) / width;
    sdl_window_scale.y = ((float)pRender_height) / height;

    if (harness_game_config.start_full_screen) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        LOG_PANIC("Failed to call SDL_GL_CreateContext. %s", SDL_GetError());
    }

    // Load GL extensions using glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        LOG_PANIC("Failed to initialize the OpenGL context with GLAD.");
        exit(1);
    }

    GLRenderer_Init(width, height, pRender_width, pRender_height);

    return window;
}

void SetWindowPos_(HWND hWnd, int x, int y, int nWidth, int nHeight) {
    SDL_SetWindowPosition(hWnd, x, y);
    SDL_SetWindowSize(hWnd, nWidth, nHeight);
}

void CreateRenderer() {
    int width = 320;
    int height = 200;
    int pRender_width = width;
    int pRender_height = height;

    context = SDL_GL_CreateContext(window);
    if (!context) {
        LOG_PANIC("Failed to call SDL_GL_CreateContext. %s", SDL_GetError());
    }

    // Load GL extensions using glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        LOG_PANIC("Failed to initialize the OpenGL context with GLAD.");
        exit(1);
    }

    GLRenderer_Init(width, height, pRender_width, pRender_height);
}

void IOPlatform_Shutdown() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    window = NULL;
}

// Checks whether the `flag_check` is the only modifier applied.
// e.g. is_only_modifier(event.key.keysym.mod, KMOD_ALT) returns true when only the ALT key was pressed
static int is_only_key_modifier(int modifier_flags, int flag_check) {
    return (modifier_flags & flag_check) && (modifier_flags & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI)) == (modifier_flags & flag_check);
}

int GetAndHandleMessage(MSG* msg) {
    SDL_Event event;
    int dinput_key;
    int w_w, w_h;
    int vp_x, vp_y;
    int vp_w, vp_h;
    int r_w, r_h;

    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_RETURN) {
                if (event.key.type == SDL_KEYDOWN) {
                    if ((event.key.keysym.mod & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI))) {
                        // Ignore keydown of RETURN when used together with some modifier
                        return 0;
                    }
                } else if (event.key.type == SDL_KEYUP) {
                    if (is_only_key_modifier(event.key.keysym.mod, KMOD_ALT)) {
                        SDL_SetWindowFullscreen(window, (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                }
            }

            // Map incoming SDL scancode to DirectInput DIK_* key code.
            // https://github.com/DanielGibson/Snippets/blob/master/sdl2_scancode_to_dinput.h
            dinput_key = sdlScanCodeToDirectInputKeyNum[event.key.keysym.scancode];
            if (dinput_key == 0) {
                LOG_WARN("unexpected scan code %s (%d)", SDL_GetScancodeName(event.key.keysym.scancode), event.key.keysym.scancode);
                return 0;
            }
            // DInput expects high bit to be set if key is down
            // https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee418261(v=vs.85)
            directinput_key_state[dinput_key] = (event.type == SDL_KEYDOWN ? 0x80 : 0);
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SDL_GetWindowSize(window, &w_w, &w_h);
                gl_renderer.SetWindowSize(w_w, w_h);
                gl_renderer.GetViewport(&vp_x, &vp_y, &vp_w, &vp_h);
                gl_renderer.GetRenderSize(&r_w, &r_h);
                sdl_window_scale.x = (float)r_w / vp_w;
                sdl_window_scale.y = (float)r_h / vp_h;
                break;
            }
            break;

        case SDL_QUIT:
            msg->message = WM_QUIT;
            return 1;
        }
    }
    return 0;
}

void SwapWindow(int delay_ms_after_swap) {
    SDL_GL_SwapWindow(window);

    if (delay_ms_after_swap != 0) {
        SDL_Delay(delay_ms_after_swap);
    }
}

void GetKeyboardState(unsigned int count, uint8_t* buffer) {
    memcpy(buffer, directinput_key_state, count);
}

void IOPlatform_GetMousePosition(int* pX, int* pY) {
    int vp_x, vp_y, vp_w, vp_h;

    SDL_GetMouseState(pX, pY);
    gl_renderer.GetViewport(&vp_x, &vp_y, &vp_w, &vp_h);
    if (*pX < vp_x) {
        *pX = vp_x;
    } else if (*pX >= vp_x + vp_w) {
        *pX = vp_x + vp_w - 1;
    }
    if (*pY < vp_y) {
        *pY = vp_y;
    } else if (*pY >= vp_y + vp_h) {
        *pY = vp_y + vp_h - 1;
    }
    *pX -= vp_x;
    *pY -= vp_y;
    *pX *= sdl_window_scale.x;
    *pY *= sdl_window_scale.y;

#if defined(DETHRACE_FIX_BUGS)
    // In hires mode (640x480), the menus are still rendered at (320x240),
    // so prescale the cursor coordinates accordingly.
    *pX *= gGraf_specs[gGraf_data_index].phys_width;
    *pX /= gGraf_specs[gReal_graf_data_index].phys_width;
    *pY *= gGraf_specs[gGraf_data_index].phys_height;
    *pY /= gGraf_specs[gReal_graf_data_index].phys_height;
#endif
}

void IOPlatform_GetMouseButtons(int* pButton1, int* pButton2) {
    int state = SDL_GetMouseState(NULL, NULL);
    *pButton1 = state & SDL_BUTTON_LMASK;
    *pButton2 = state & SDL_BUTTON_RMASK;
}

void GetMousePosition_(LPPOINT point) {
    SDL_GetMouseState(&point->x, &point->y);
}

void IOPlatform_Init(tPlatform_hooks* platform) {
    platform->GetMessage = GetAndHandleMessage;
    platform->Sleep = SDL_Delay;
    platform->GetTicks = SDL_GetTicks;
    platform->CreateWindow = CreateWindowAndRenderer;
    platform->ShowCursor = SDL_ShowCursor;
    platform->SetWindowPos = SetWindowPos_;
    platform->SwapWindow = SwapWindow;
    platform->BufferModel = GLRenderer_BufferModel;
    platform->BufferMaterial = GLRenderer_BufferMaterial;
    platform->BufferTexture = GLRenderer_BufferTexture;
    platform->SetPalette = GLRenderer_SetPalette;
    platform->RenderFullScreenQuad = GLRenderer_FullScreenQuad;
    platform->ClearBuffers = GLRenderer_ClearBuffers;
    platform->GetKeyboardState = GetKeyboardState;
    platform->GetCursorPos = GetMousePosition_;
    platform->DestroyWindow = SDL_DestroyWindow;
}
