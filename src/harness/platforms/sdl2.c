#include <SDL.h>

#include "harness.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "sdl2_syms.h"
#include "sdl_scancode_map.h"

SDL_COMPILE_TIME_ASSERT(sdl2_platform_requires_SDL2, SDL_MAJOR_VERSION == 2);

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* screen_texture;
static uint32_t converted_palette[256];
static br_pixelmap* last_screen_src;

static SDL_GLContext* gl_context;

static int render_width, render_height;

static Uint32 last_frame_time;

void (*gKeyHandler_func)(void);

// 32 bytes, 1 bit per key. Matches dos executable behavior
static uint32_t key_state[8];

static struct {
    int x, y;
    float scale_x, scale_y;
} viewport;

// Callbacks back into original game code
extern void QuitGame(void);
extern br_pixelmap* gBack_screen;

#ifdef DETHRACE_SDL_DYNAMIC
#ifdef _WIN32
static const char* const possible_locations[] = {
    "SDL2.dll",
};
#elif defined(__APPLE__)
#define SHARED_OBJECT_NAME "libSDL2"
#define SDL2_LIBNAME "libSDL2.dylib"
#define SDL2_FRAMEWORK "SDL2.framework/Versions/A/SDL2"
static const char* const possible_locations[] = {
    "@loader_path/" SDL2_LIBNAME,                     /* MyApp.app/Contents/MacOS/libSDL2_dylib */
    "@loader_path/../Frameworks/" SDL2_FRAMEWORK,     /* MyApp.app/Contents/Frameworks/SDL2_framework */
    "@executable_path/" SDL2_LIBNAME,                 /* MyApp.app/Contents/MacOS/libSDL2_dylib */
    "@executable_path/../Frameworks/" SDL2_FRAMEWORK, /* MyApp.app/Contents/Frameworks/SDL2_framework */
    NULL,                                             /* /Users/username/Library/Frameworks/SDL2_framework */
    "/Library/Frameworks" SDL2_FRAMEWORK,             /* /Library/Frameworks/SDL2_framework */
    SDL2_LIBNAME                                      /* oh well, anywhere the system can see the .dylib (/usr/local/lib or whatever) */
};
#else
static const char* const possible_locations[] = {
    "libSDL2-2.0.so.0",
    "libSDL2-2.0.so",
};
#endif
#endif

#ifdef DETHRACE_SDL_DYNAMIC
static void* sdl2_so;
#endif

#define OBJECT_NAME sdl2_so
#define SYMBOL_PREFIX SDL2_
#define FOREACH_SDLX_SYM FOREACH_SDL2_SYM

#include "sdl_dyn_common.h"

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

static int SDL2_Harness_SetWindowPos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    // SDL_SetWindowPosition(hWnd, x, y);
    if (nWidth == 320 && nHeight == 200) {
        nWidth = 640;
        nHeight = 400;
    }
    SDL2_SetWindowSize(hWnd, nWidth, nHeight);
    return 0;
}

static void SDL2_Harness_DestroyWindow(void) {
    // SDL2_GL_DeleteContext(context);
    if (window != NULL) {
        SDL2_DestroyWindow(window);
    }
    SDL2_Quit();
    window = NULL;
}

// Checks whether the `flag_check` is the only modifier applied.
// e.g. is_only_modifier(event.key.keysym.mod, KMOD_ALT) returns true when only the ALT key was pressed
static int is_only_key_modifier(int modifier_flags, int flag_check) {
    return (modifier_flags & flag_check) && (modifier_flags & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI)) == (modifier_flags & flag_check);
}

static void SDL2_Harness_ProcessWindowMessages(void) {
    SDL_Event event;

    while (SDL2_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (event.key.windowID != SDL2_GetWindowID(window)) {
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
#ifndef __EMSCRIPTEN__
                        SDL2_SetWindowFullscreen(window, (SDL2_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
                    }
                }
            }

            // Map incoming SDL scancode to PC scan code as used by game code
            int dethrace_scancode = sdl_scancode_map[event.key.keysym.scancode];
            if (dethrace_scancode == 0) {
                LOG_WARN("unexpected scan code %s (%d)", SDL2_GetScancodeName(event.key.keysym.scancode), event.key.keysym.scancode);
                return;
            }

            if (event.type == SDL_KEYDOWN) {
                key_state[dethrace_scancode >> 5] |= (1 << (dethrace_scancode & 0x1F));
            } else {
                key_state[dethrace_scancode >> 5] &= ~(1 << (dethrace_scancode & 0x1F));
            }
            gKeyHandler_func();
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

static void SDL2_Harness_SetKeyHandler(void (*handler_func)(void)) {
    gKeyHandler_func = handler_func;
}

static void SDL2_Harness_GetKeyboardState(uint32_t* buffer) {
    memcpy(buffer, key_state, sizeof(key_state));
}

static int SDL2_Harness_GetMouseButtons(int* pButton1, int* pButton2) {
    if (SDL2_GetMouseFocus() != window) {
        *pButton1 = 0;
        *pButton2 = 0;
        return 0;
    }
    int state = SDL2_GetMouseState(NULL, NULL);
    *pButton1 = state & SDL_BUTTON_LMASK;
    *pButton2 = state & SDL_BUTTON_RMASK;
    return 0;
}

static int SDL2_Harness_GetMousePosition(int* pX, int* pY) {
    int window_width, window_height;
    float lX, lY;

    if (SDL2_GetMouseFocus() != window) {
        return 0;
    }
    SDL2_GetWindowSize(window, &window_width, &window_height);

    SDL2_GetMouseState(pX, pY);
    if (renderer != NULL) {
        // software renderer
        SDL2_RenderWindowToLogical(renderer, *pX, *pY, &lX, &lY);
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
    Uint32 now = SDL2_GetTicks();
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
    last_frame_time = SDL2_GetTicks();
}

static int SDL2_Harness_ShowErrorMessage(char* title, char* message) {
    fprintf(stderr, "%s", message);
    SDL2_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, window);
    return 0;
}

static void SDL2_Harness_CreateWindow(const char* title, int width, int height, tHarness_window_type window_type) {
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

    if (SDL2_Init(SDL_INIT_VIDEO) != 0) {
        LOG_PANIC("SDL_INIT_VIDEO error: %s", SDL2_GetError());
    }

    if (window_type == eWindow_type_opengl) {

        window = SDL2_CreateWindow(title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width, window_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        if (window == NULL) {
            LOG_PANIC("Failed to create window: %s", SDL2_GetError());
        }

        SDL2_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL2_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL2_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        gl_context = SDL2_GL_CreateContext(window);

        if (gl_context == NULL) {
            LOG_WARN("Failed to create OpenGL core profile: %s. Trying OpenGLES...", SDL2_GetError());
            SDL2_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            SDL2_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL2_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            gl_context = SDL2_GL_CreateContext(window);
        }
        if (gl_context == NULL) {
            LOG_PANIC("Failed to create OpenGL context: %s", SDL2_GetError());
        }
        SDL2_GL_SetSwapInterval(1);

    } else {
        window = SDL2_CreateWindow(title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width, window_height,
            SDL_WINDOW_RESIZABLE);
        if (window == NULL) {
            LOG_PANIC("Failed to create window: %s", SDL2_GetError());
        }

        renderer = SDL2_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        if (renderer == NULL) {
            LOG_PANIC("Failed to create renderer: %s", SDL2_GetError());
        }
        SDL2_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL2_RenderSetLogicalSize(renderer, render_width, render_height);

        screen_texture = SDL2_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (screen_texture == NULL) {
            SDL_RendererInfo info;
            SDL2_GetRendererInfo(renderer, &info);
            for (Uint32 i = 0; i < info.num_texture_formats; i++) {
                LOG_INFO("%s\n", SDL2_GetPixelFormatName(info.texture_formats[i]));
            }
            LOG_PANIC("Failed to create screen_texture: %s", SDL2_GetError());
        }
    }

    SDL2_ShowCursor(SDL_DISABLE);

    viewport.x = 0;
    viewport.y = 0;
    viewport.scale_x = 1;
    viewport.scale_y = 1;

    if (harness_game_config.start_full_screen) {
#ifndef __EMSCRIPTEN__
        SDL2_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
    }
}

static void SDL2_Harness_Swap(br_pixelmap* back_buffer) {

    SDL2_Harness_ProcessWindowMessages();

    if (gl_context != NULL) {
        SDL2_GL_SwapWindow(window);
    } else {
        uint8_t* src_pixels = back_buffer->pixels;
        uint32_t* dest_pixels;
        int dest_pitch;

        SDL2_LockTexture(screen_texture, NULL, (void**)&dest_pixels, &dest_pitch);
        for (int i = 0; i < back_buffer->height * back_buffer->width; i++) {
            *dest_pixels = converted_palette[*src_pixels];
            dest_pixels++;
            src_pixels++;
        }
        SDL2_UnlockTexture(screen_texture);
        SDL2_RenderClear(renderer);
        SDL2_RenderCopy(renderer, screen_texture, NULL, NULL);
        SDL2_RenderPresent(renderer);
        last_screen_src = back_buffer;
    }

    if (harness_game_config.fps != 0) {
        limit_fps();
    }
}

static void SDL2_Harness_PaletteChanged(br_colour entries[256]) {
    for (int i = 0; i < 256; i++) {
        converted_palette[i] = (0xff << 24 | BR_RED(entries[i]) << 16 | BR_GRN(entries[i]) << 8 | BR_BLU(entries[i]));
    }
    if (last_screen_src != NULL) {
        SDL2_Harness_Swap(last_screen_src);
    }
}

static void SDL2_Harness_GetViewport(int* x, int* y, float* width_multipler, float* height_multiplier) {
    *x = viewport.x;
    *y = viewport.y;
    *width_multipler = viewport.scale_x;
    *height_multiplier = viewport.scale_y;
}

static int SDL2_Harness_Platform_Init(tHarness_platform* platform) {
    if (SDL2_LoadSymbols() != 0) {
        return 1;
    }
    platform->ProcessWindowMessages = SDL2_Harness_ProcessWindowMessages;
    platform->Sleep = SDL2_Delay;
    platform->GetTicks = SDL2_GetTicks;
    platform->ShowCursor = SDL2_ShowCursor;
    platform->SetWindowPos = SDL2_Harness_SetWindowPos;
    platform->DestroyWindow = SDL2_Harness_DestroyWindow;
    platform->SetKeyHandler = SDL2_Harness_SetKeyHandler;
    platform->GetKeyboardState = SDL2_Harness_GetKeyboardState;
    platform->GetMousePosition = SDL2_Harness_GetMousePosition;
    platform->GetMouseButtons = SDL2_Harness_GetMouseButtons;
    platform->ShowErrorMessage = SDL2_Harness_ShowErrorMessage;

    platform->CreateWindow_ = SDL2_Harness_CreateWindow;
    platform->Swap = SDL2_Harness_Swap;
    platform->PaletteChanged = SDL2_Harness_PaletteChanged;
    platform->GL_GetProcAddress = SDL2_GL_GetProcAddress;
    platform->GetViewport = SDL2_Harness_GetViewport;
    return 0;
};

const tPlatform_bootstrap SDL2_bootstrap = {
    "sdl2",
    "SDL2 video backend (libsdl.org)",
    ePlatform_cap_software | ePlatform_cap_opengl,
    SDL2_Harness_Platform_Init,
};
