#include <glad/glad.h>

// this needs to be included after glad.h
#include <SDL.h>
#include <SDL_opengl.h>

#include "../renderers/gl/gl_renderer.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "sdl2_scancode_to_dinput.h"

#include "globvars.h"
#include "grafdata.h"
#include "pd/sys.h"

SDL_Window* window;
SDL_GLContext context;
uint8_t directinput_key_state[SDL_NUM_SCANCODES];
int render_width, render_height;
int window_width, window_height;
int vp_x, vp_y, vp_width, vp_height;

struct {
    float x;
    float y;
} sdl_window_scale;

static void update_viewport(void) {
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
    GLRenderer_SetViewport(vp_x, vp_y, vp_width, vp_height);
}

static void* create_window_and_renderer(char* title, int x, int y, int width, int height) {
    window_width = width;
    window_height = height;
    render_width = width;
    render_height = height;
    tOpenGL_profile opengl_profile;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_PANIC("SDL_INIT_VIDEO error: %s", SDL_GetError());
    }

    SDL_GL_SetSwapInterval(1);

    // prefer OpenGL core profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    opengl_profile = eOpenGL_profile_core;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        LOG_WARN("Failed to create OpenGL core profile: %s. Trying OpenGLES...", SDL_GetError());
        // fallback to OpenGL ES 3
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        opengl_profile = eOpenGL_profile_es;
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    }
    if (window == NULL) {
        LOG_PANIC("Failed to create window. %s", SDL_GetError());
    }

    sdl_window_scale.x = ((float)render_width) / width;
    sdl_window_scale.y = ((float)render_height) / height;

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

    GLRenderer_Init(opengl_profile, render_width, render_height);
    update_viewport();

    return window;
}

static int set_window_pos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    // SDL_SetWindowPosition(hWnd, x, y);
    if (nWidth == 320 && nHeight == 200) {
        nWidth = 640;
        nHeight = 400;
    }
    SDL_SetWindowSize(hWnd, nWidth, nHeight);
    return 0;
}

static void destroy_window(void* hWnd) {
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

static int get_and_handle_message(MSG_* msg) {
    SDL_Event event;
    int dinput_key;

    while (SDL_PollEvent(&event)) {
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
                SDL_GetWindowSize(window, &window_width, &window_height);
                update_viewport();
                sdl_window_scale.x = (float)render_width / vp_width;
                sdl_window_scale.y = (float)render_height / vp_height;
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

static void swap_window(void) {
    SDL_GL_SwapWindow(window);
}

static void get_keyboard_state(unsigned int count, uint8_t* buffer) {
    memcpy(buffer, directinput_key_state, count);
}

static int get_mouse_buttons(int* pButton1, int* pButton2) {
    int state = SDL_GetMouseState(NULL, NULL);
    *pButton1 = state & SDL_BUTTON_LMASK;
    *pButton2 = state & SDL_BUTTON_RMASK;
    return 0;
}

static int get_mouse_position(int* pX, int* pY) {
    SDL_GetMouseState(pX, pY);

    if (*pX < vp_x) {
        *pX = vp_x;
    } else if (*pX >= vp_x + vp_width) {
        *pX = vp_x + vp_width - 1;
    }
    if (*pY < vp_y) {
        *pY = vp_y;
    } else if (*pY >= vp_y + vp_height) {
        *pY = vp_y + vp_height - 1;
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
    return 0;
}

static void set_palette(PALETTEENTRY_* pal) {
    GLRenderer_SetPalette((uint8_t*)pal);
}

int show_error_message(void* window, char* text, char* caption) {
    fprintf(stderr, "%s", text);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, caption, text, window);
    return 0;
}

void Harness_Platform_Init(tHarness_platform* platform) {
    platform->ProcessWindowMessages = get_and_handle_message;
    platform->Sleep = SDL_Delay;
    platform->GetTicks = SDL_GetTicks;
    platform->CreateWindowAndRenderer = create_window_and_renderer;
    platform->ShowCursor = SDL_ShowCursor;
    platform->SetWindowPos = set_window_pos;
    platform->SwapWindow = swap_window;
    platform->DestroyWindow = destroy_window;
    platform->GetKeyboardState = get_keyboard_state;
    platform->GetMousePosition = get_mouse_position;
    platform->GetMouseButtons = get_mouse_buttons;
    platform->DestroyWindow = destroy_window;
    platform->ShowErrorMessage = show_error_message;

    platform->Renderer_BufferModel = GLRenderer_BufferModel;
    platform->Renderer_BufferMaterial = GLRenderer_BufferMaterial;
    platform->Renderer_BufferTexture = GLRenderer_BufferTexture;
    platform->Renderer_SetPalette = set_palette;
    platform->Renderer_FullScreenQuad = GLRenderer_FullScreenQuad;
    platform->Renderer_Model = GLRenderer_Model;
    platform->Renderer_ClearBuffers = GLRenderer_ClearBuffers;
    platform->Renderer_FlushBuffers = GLRenderer_FlushBuffers;
    platform->Renderer_BeginScene = GLRenderer_BeginScene;
    platform->Renderer_EndScene = GLRenderer_EndScene;
    platform->Renderer_SetViewport = GLRenderer_SetViewport;
}
