#include <SDL.h>

#include "harness.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "sdl1_scancode_to_dinput.h"
#include "sdl1_syms.h"

SDL_COMPILE_TIME_ASSERT(sdl1_platform_requires_SDL1, SDL_MAJOR_VERSION == 1);

static SDL_Surface* screen;
static uint32_t converted_palette[256];
static br_pixelmap* last_screen_src;

static Uint32 last_frame_time;

static void (*gKeyHandler_func)(void);

// 32 bytes, 1 bit per key. Matches dos executable behavior
static uint32_t key_state[8];

static struct {
    int x, y;
    float scale_x, scale_y;
} viewport;

// Callbacks back into original game code
extern void QuitGame(void);
extern br_pixelmap* gBack_screen;
static int window_width, window_height;

#ifdef DETHRACE_SDL_DYNAMIC
#ifdef _WIN32
static const char* const possible_locations[] = {
    "SDL.dll",
};
#elif defined(__APPLE__)
#define SHARED_OBJECT_NAME "libSDL"
#define SDL1_LIBNAME "libSDL.dylib"
static const char* const possible_locations[] = {
    "@loader_path/" SDL1_LIBNAME,     /* MyApp.app/Contents/MacOS/libSDL2_dylib */
    "@executable_path/" SDL1_LIBNAME, /* MyApp.app/Contents/MacOS/libSDL2_dylib */
    SDL1_LIBNAME                      /* oh well, anywhere the system can see the .dylib (/usr/local/lib or whatever) */
};
#else
static const char* const possible_locations[] = {
    "libSDL-1.2.so.0",
    "libSDL-1.2.so",
};
#endif
#endif

#ifdef DETHRACE_SDL_DYNAMIC
static void* sdl1_so;
#endif

#define OBJECT_NAME sdl1_so
#define SYMBOL_PREFIX SDL1_
#define FOREACH_SDLX_SYM FOREACH_SDL1_SYM

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

static void recreate_screen(void) {
    int video_flags = SDL_RESIZABLE | SDL_HWSURFACE | SDL_DOUBLEBUF;

    if (harness_game_config.start_full_screen) {
        video_flags |= SDL_FULLSCREEN;
    }
    screen = SDL1_SetVideoMode(window_width, window_height, 32, video_flags);
    if (screen == NULL) {
        LOG_PANIC("SDL_SetVideoMode failed (%s)", SDL1_GetError());
    }
}

static void SDL1_Harness_CreateWindow(const char* title, int width, int height, tHarness_window_type window_type) {
    window_width = width;
    window_height = height;

    initializeSDL1KeyNums();

    if (SDL1_Init(SDL_INIT_VIDEO) != 0) {
        LOG_PANIC("SDL_INIT_VIDEO error: %s", SDL1_GetError());
    }

    if (window_type == eWindow_type_software) {
        recreate_screen();
    } else {
        LOG_PANIC("Unsupported window type (%d)", window_type);
    }

    SDL1_WM_SetCaption("Carmageddon", NULL);

    SDL1_ShowCursor(SDL_DISABLE);
}

static int SDL1_Harness_SetWindowPos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    // SDL_SetWindowPosition(hWnd, x, y);
    if (nWidth == 320 && nHeight == 200) {
        nWidth = 640;
        nHeight = 400;
    }
    window_width = nWidth;
    window_height = nHeight;
    recreate_screen();
    SDL1_UpdateRect(hWnd, x, y, nWidth, nHeight);
    return 0;
}

static void SDL1_Harness_DestroyWindow(void) {
    SDL1_FreeSurface(screen);
    SDL1_Quit();
    screen = NULL;
}

// Checks whether the `flag_check` is the only modifier applied.
// e.g. is_only_modifier(event.key.keysym.mod, KMOD_ALT) returns true when only the ALT key was pressed
static int is_only_key_modifier(int modifier_flags, int flag_check) {
    return (modifier_flags & flag_check) && (modifier_flags & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_META)) == (modifier_flags & flag_check);
}

static void SDL1_Harness_ProcessWindowMessages(void) {
    SDL_Event event;
    int dethrace_scancode;

    while (SDL1_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_RETURN) {
                if (event.key.type == SDL_KEYDOWN) {
                    if ((event.key.keysym.mod & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_META))) {
                        // Ignore keydown of RETURN when used together with some modifier
                        return;
                    }
                } else if (event.key.type == SDL_KEYUP) {
                    if (is_only_key_modifier(event.key.keysym.mod, KMOD_ALT)) {
                        SDL1_WM_ToggleFullScreen(screen);
                    }
                }
            }

            // Map incoming SDL key to PC scan code as used by game code
            dethrace_scancode = sdl1KeyToDirectInputKeyNum[event.key.keysym.sym];
            if (dethrace_scancode == 0) {
                LOG_WARN("unexpected key \"%s\" (0x%x)", SDL1_GetKeyName(event.key.keysym.sym), event.key.keysym.sym);
                return;
            }
            if (event.type == SDL_KEYDOWN) {
                key_state[dethrace_scancode >> 5] |= (1 << (dethrace_scancode & 0x1F));
            } else {
                key_state[dethrace_scancode >> 5] &= ~(1 << (dethrace_scancode & 0x1F));
            }
            gKeyHandler_func();
            break;

        case SDL_VIDEORESIZE:
            calculate_viewport(event.resize.w, event.resize.h);
            break;

        case SDL_QUIT:
            QuitGame();
            return;
        }
    }
    return;
}

static void SDL1_Harness_SetKeyHandler(void (*handler_func)(void)) {
    gKeyHandler_func = handler_func;
}

static void SDL1_Harness_GetKeyboardState(uint32_t* buffer) {
    memcpy(buffer, key_state, sizeof(key_state));
}

static int SDL1_Harness_GetMouseButtons(int* pButton1, int* pButton2) {
    int state = SDL1_GetMouseState(NULL, NULL);
    *pButton1 = state & SDL_BUTTON_LMASK;
    *pButton2 = state & SDL_BUTTON_RMASK;
    return 0;
}

static int SDL1_Harness_GetMousePosition(int* pX, int* pY) {
    SDL1_GetMouseState(pX, pY);
    return 0;
}

static void limit_fps(void) {
    Uint32 now = SDL1_GetTicks();
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
    last_frame_time = SDL1_GetTicks();
}

static void SDL1_Renderer_Present(br_pixelmap* src) {
    // fastest way to convert 8 bit indexed to 32 bit
    uint8_t* src_pixels = src->pixels;
    uint32_t* dest_pixels;

    SDL1_LockSurface(screen);
    dest_pixels = screen->pixels;
    for (int i = 0; i < src->height * src->width; i++) {
        *dest_pixels = converted_palette[*src_pixels];
        dest_pixels++;
        src_pixels++;
    }
    SDL1_UnlockSurface(screen);

    SDL1_Flip(screen);

    last_screen_src = src;

    if (harness_game_config.fps != 0) {
        limit_fps();
    }
}

static void SDL1_Harness_Swap(br_pixelmap* back_buffer) {

    SDL1_Harness_ProcessWindowMessages();

    if (0) {
        SDL1_GL_SwapBuffers();
    } else {
        SDL1_Renderer_Present(back_buffer);

        if (harness_game_config.fps != 0) {
            limit_fps();
        }
    }
}

static void SDL1_Harness_PaletteChanged(br_colour entries[256]) {
    for (int i = 0; i < 256; i++) {
        converted_palette[i] = (0xff << 24 | BR_RED(entries[i]) << 16 | BR_GRN(entries[i]) << 8 | BR_BLU(entries[i]));
    }
    if (last_screen_src != NULL) {
        SDL1_Harness_Swap(last_screen_src);
    }
}

static int SDL1_Harness_ShowErrorMessage(char* text, char* caption) {
    fprintf(stderr, "%s", text);
#ifdef _WIN32
    MessageBoxA(NULL, text, caption, MB_ICONERROR);
#endif
    return 0;
}

static void SDL1_Harness_GetViewport(int* x, int* y, float* width_multipler, float* height_multiplier) {
    *x = viewport.x;
    *y = viewport.y;
    *width_multipler = viewport.scale_x;
    *height_multiplier = viewport.scale_y;
}

static void SDL1_Harness_GetPrefPath(char* app_name, char* path) {
    // SDL_GetPrefPath not in SDL1. We could implement it if we really needed to.
    // for now, just return the current path
    strcpy(path, ".");
}

static int SDL1_Harness_Platform_Init(tHarness_platform* platform) {
    if (SDL1_LoadSymbols() != 0) {
        return 1;
    }
    platform->ProcessWindowMessages = SDL1_Harness_ProcessWindowMessages;
    platform->Sleep = SDL1_Delay;
    platform->GetTicks = SDL1_GetTicks;
    platform->ShowCursor = SDL1_ShowCursor;
    platform->SetWindowPos = SDL1_Harness_SetWindowPos;
    platform->DestroyWindow = SDL1_Harness_DestroyWindow;
    platform->SetKeyHandler = SDL1_Harness_SetKeyHandler;
    platform->GetKeyboardState = SDL1_Harness_GetKeyboardState;
    platform->GetMousePosition = SDL1_Harness_GetMousePosition;
    platform->GetMouseButtons = SDL1_Harness_GetMouseButtons;
    platform->ShowErrorMessage = SDL1_Harness_ShowErrorMessage;

    platform->CreateWindow_ = SDL1_Harness_CreateWindow;
    platform->Swap = SDL1_Harness_Swap;
    platform->PaletteChanged = SDL1_Harness_PaletteChanged;
    platform->GL_GetProcAddress = SDL1_GL_GetProcAddress;
    platform->GetViewport = SDL1_Harness_GetViewport;
    platform->GetPrefPath = SDL1_Harness_GetPrefPath;
    return 0;
}

const tPlatform_bootstrap SDL1_bootstrap = {
    "sdl1",
    "SDL1 video backend (libsdl.org)",
    ePlatform_cap_software,
    SDL1_Harness_Platform_Init,
};
