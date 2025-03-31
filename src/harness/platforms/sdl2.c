#include <SDL.h>

#include "harness.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "sdl2_scancode_to_dinput.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* screen_texture;
static uint32_t converted_palette[256];
static br_pixelmap* last_screen_src;

static SDL_GLContext* gl_context;

static int render_width, render_height;

static Uint32 last_frame_time;

static uint8_t directinput_key_state[SDL_NUM_SCANCODES];

static struct {
    int x, y;
    float scale_x, scale_y;
} viewport;

// Callbacks back into original game code
extern void QuitGame(void);
extern uint32_t gKeyboard_bits[8];
extern br_pixelmap* gBack_screen;

static void calculate_viewport(int window_width, int window_height) {
    int vp_width, vp_height;
    float target_aspect_ratio;
    float aspect_ratio;

    aspect_ratio = (float)window_width / window_height;
    target_aspect_ratio = (float)gBack_screen->width / gBack_screen->height;

    vp_width = window_width;
    vp_height = window_height;
    if (aspect_ratio != target_aspect_ratio) {
        if (aspect_ratio > target_aspect_ratio) {
            vp_width = window_height * target_aspect_ratio + .5f;
        } else {
            vp_height = window_width / target_aspect_ratio + .5f;
        }
    }
    viewport.x = (window_width - vp_width) / 2;
    viewport.y = (window_height - vp_height) / 2;
    viewport.scale_x = (float)vp_width / gBack_screen->width;
    viewport.scale_y = (float)vp_height / gBack_screen->height;
}

static int SDL2_SetWindowPos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    // SDL_SetWindowPosition(hWnd, x, y);
    if (nWidth == 320 && nHeight == 200) {
        nWidth = 640;
        nHeight = 400;
    }
    SDL_SetWindowSize(hWnd, nWidth, nHeight);
    return 0;
}

static void SDL2_DestroyWindow(void* hWnd) {
    // SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    window = NULL;
}

// Checks whether the `flag_check` is the only modifier applied.
// e.g. is_only_modifier(event.key.keysym.mod, KMOD_ALT) returns true when only the ALT key was pressed
static int is_only_key_modifier(int modifier_flags, int flag_check) {
    return (modifier_flags & flag_check) && (modifier_flags & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI)) == (modifier_flags & flag_check);
}

static void SDL2_ProcessWindowMessages(MSG_* msg) {
    SDL_Event event;
    int dinput_key;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (event.key.windowID != SDL_GetWindowID(window)) {
                continue;
            }
            if (event.key.keysym.sym == SDLK_RETURN) {
                if (event.key.type == SDL_KEYDOWN) {
                    if ((event.key.keysym.mod & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI))) {
                        // Ignore keydown of RETURN when used together with some modifier
                        return;
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
                return;
            }
            // DInput expects high bit to be set if key is down
            // https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee418261(v=vs.85)
            directinput_key_state[dinput_key] = (event.type == SDL_KEYDOWN ? 0x80 : 0);
            if (event.type == SDL_KEYDOWN) {
                gKeyboard_bits[dinput_key >> 5] |= (1 << (dinput_key & 0x1F));
            } else {
                gKeyboard_bits[dinput_key >> 5] &= ~(1 << (dinput_key & 0x1F));
            }
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                calculate_viewport(event.window.data1, event.window.data2);
            }
            break;

        case SDL_QUIT:
            QuitGame();
        }
    }
}

static void SDL2_GetKeyboardState(unsigned int count, uint8_t* buffer) {
    memcpy(buffer, directinput_key_state, count);
}

static int SDL2_GetMouseButtons(int* pButton1, int* pButton2) {
    if (SDL_GetMouseFocus() != window) {
        *pButton1 = 0;
        *pButton2 = 0;
        return 0;
    }
    int state = SDL_GetMouseState(NULL, NULL);
    *pButton1 = state & SDL_BUTTON_LMASK;
    *pButton2 = state & SDL_BUTTON_RMASK;
    return 0;
}

static int SDL2_GetMousePosition(int* pX, int* pY) {
    int window_width, window_height;
    float lX, lY;

    if (SDL_GetMouseFocus() != window) {
        return 0;
    }
    SDL_GetWindowSize(window, &window_width, &window_height);

    SDL_GetMouseState(pX, pY);
    if (renderer != NULL) {
        // software renderer
        SDL_RenderWindowToLogical(renderer, *pX, *pY, &lX, &lY);
    } else {
        // hardware renderer
        // handle case where window is stretched larger than the pixel size
        lX = *pX * (640.0f / window_width);
        lY = *pY * (480.0f / window_height);
    }
    *pX = (int)lX;
    *pY = (int)lY;
    return 0;
}

static void limit_fps(void) {
    Uint32 now = SDL_GetTicks();
    if (last_frame_time != 0) {
        unsigned int frame_time = now - last_frame_time;
        last_frame_time = now;
        if (frame_time < 100) {
            int sleep_time = (1000 / harness_game_config.fps) - frame_time;
            if (sleep_time > 5) {
                gHarness_platform.Sleep(sleep_time);
            }
        }
    }
    last_frame_time = SDL_GetTicks();
}

static int SDL2_ShowErrorMessage(void* window, char* text, char* caption) {
    fprintf(stderr, "%s", text);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, caption, text, window);
    return 0;
}

static void SDL2_CreateWindow(char* title, int width, int height, tHarness_window_type window_type) {
    int window_width, window_height;

    render_width = width;
    render_height = height;

    window_width = width;
    window_height = height;

    // special case lores and make a bigger window
    if (width == 320 && height == 200) {
        window_width = 640;
        window_height = 480;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_PANIC("SDL_INIT_VIDEO error: %s", SDL_GetError());
    }

    if (window_type == eWindow_type_opengl) {

        window = SDL_CreateWindow(title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width, window_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        if (window == NULL) {
            LOG_PANIC("Failed to create window: %s", SDL_GetError());
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        gl_context = SDL_GL_CreateContext(window);

        if (gl_context == NULL) {
            LOG_WARN("Failed to create OpenGL core profile: %s. Trying OpenGLES...", SDL_GetError());
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            gl_context = SDL_GL_CreateContext(window);
        }
        if (gl_context == NULL) {
            LOG_PANIC("Failed to create OpenGL context: %s", SDL_GetError());
        }
        SDL_GL_SetSwapInterval(1);

    } else {
        window = SDL_CreateWindow(title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width, window_height,
            SDL_WINDOW_RESIZABLE);
        if (window == NULL) {
            LOG_PANIC("Failed to create window: %s", SDL_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        if (renderer == NULL) {
            LOG_PANIC("Failed to create renderer: %s", SDL_GetError());
        }
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_RenderSetLogicalSize(renderer, render_width, render_height);

        screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (screen_texture == NULL) {
            SDL_RendererInfo info;
            SDL_GetRendererInfo(renderer, &info);
            for (Uint32 i = 0; i < info.num_texture_formats; i++) {
                LOG_INFO("%s\n", SDL_GetPixelFormatName(info.texture_formats[i]));
            }
            LOG_PANIC("Failed to create screen_texture: %s", SDL_GetError());
        }
    }

    SDL_ShowCursor(SDL_DISABLE);

    viewport.x = 0;
    viewport.y = 0;
    viewport.scale_x = 1;
    viewport.scale_y = 1;

    if (harness_game_config.start_full_screen) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
}

static void SDL2_Swap(br_pixelmap* back_buffer) {
    uint8_t* src_pixels = back_buffer->pixels;
    uint32_t* dest_pixels;
    int dest_pitch;

    SDL2_ProcessWindowMessages(NULL);

    if (gl_context != NULL) {
        SDL_GL_SwapWindow(window);
    } else {
        SDL_LockTexture(screen_texture, NULL, (void**)&dest_pixels, &dest_pitch);
        for (int i = 0; i < back_buffer->height * back_buffer->width; i++) {
            *dest_pixels = converted_palette[*src_pixels];
            dest_pixels++;
            src_pixels++;
        }
        SDL_UnlockTexture(screen_texture);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        last_screen_src = back_buffer;
    }

    if (harness_game_config.fps != 0) {
        limit_fps();
    }
}

static void SDL2_PaletteChanged(br_colour entries[256]) {
    for (int i = 0; i < 256; i++) {
        converted_palette[i] = (0xff << 24 | BR_RED(entries[i]) << 16 | BR_GRN(entries[i]) << 8 | BR_BLU(entries[i]));
    }
    if (last_screen_src != NULL) {
        SDL2_Swap(last_screen_src);
    }
}

static void SDL2_GetViewport(int* x, int* y, float* width_multipler, float* height_multiplier) {
    *x = viewport.x;
    *y = viewport.y;
    *width_multipler = viewport.scale_x;
    *height_multiplier = viewport.scale_y;
}

static int SDL2_Harness_Platform_Init(tHarness_platform* platform) {
    platform->ProcessWindowMessages = SDL2_ProcessWindowMessages;
    platform->Sleep = SDL_Delay;
    platform->GetTicks = SDL_GetTicks;
    platform->ShowCursor = SDL_ShowCursor;
    platform->SetWindowPos = SDL2_SetWindowPos;
    platform->DestroyWindow = SDL2_DestroyWindow;
    platform->GetKeyboardState = SDL2_GetKeyboardState;
    platform->GetMousePosition = SDL2_GetMousePosition;
    platform->GetMouseButtons = SDL2_GetMouseButtons;
    platform->ShowErrorMessage = SDL2_ShowErrorMessage;

    platform->CreateWindow_ = SDL2_CreateWindow;
    platform->Swap = SDL2_Swap;
    platform->PaletteChanged = SDL2_PaletteChanged;
    platform->GL_GetProcAddress = SDL_GL_GetProcAddress;
    platform->GetViewport = SDL2_GetViewport;
    return 0;
};

const tPlatform_bootstrap SDL2_bootstrap = {
    "sdl2",
    "SDL2 video backend (libsdl.org)",
    ePlatform_cap_software | ePlatform_cap_opengl,
    SDL2_Harness_Platform_Init,
};
