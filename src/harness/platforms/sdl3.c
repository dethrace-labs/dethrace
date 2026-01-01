#include <SDL3/SDL.h>

#include "harness.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "sdl3_scancode_map.h"
#include "sdl3_syms.h"

SDL_COMPILE_TIME_ASSERT(sdl3_platform_requires_SDL3, SDL_MAJOR_VERSION == 3);

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* screen_texture;
static uint32_t converted_palette[256];
static br_pixelmap* last_screen_src;

static SDL_GLContext gl_context;

static int render_width, render_height;

static Uint32 last_frame_time;

static void (*gKeyHandler_func)(void);

// 32 bytes, 1 bit per key. Matches dos executable behavior
static br_uint_32 key_state[8];

static struct {
    int x, y;
    float scale_x, scale_y;
} viewport;

// Callbacks back into original game code
extern void QuitGame(void);
extern br_pixelmap* gBack_screen;

#ifdef DETHRACE_SDL_DYNAMIC
#ifdef _WIN32
static const char * const possible_locations[] = {
    "SDL3.dll",
};
#elif defined(__APPLE__)
#define SHARED_OBJECT_NAME "libSDL3"
#define SDL3_LIBNAME "libSDL3.dylib"
#define SDL3_FRAMEWORK "SDL3.framework/Versions/A/SDL3"
static const char * const possible_locations[] = {
    "@loader_path/" SDL3_LIBNAME, /* MyApp.app/Contents/MacOS/libSDL3_dylib */
    "@loader_path/../Frameworks/" SDL3_FRAMEWORK, /* MyApp.app/Contents/Frameworks/SDL3_framework */
    "@executable_path/" SDL3_LIBNAME, /* MyApp.app/Contents/MacOS/libSDL3_dylib */
    "@executable_path/../Frameworks/" SDL3_FRAMEWORK, /* MyApp.app/Contents/Frameworks/SDL3_framework */
    NULL,  /* /Users/username/Library/Frameworks/SDL3_framework */
    "/Library/Frameworks" SDL3_FRAMEWORK, /* /Library/Frameworks/SDL3_framework */
    SDL3_LIBNAME /* oh well, anywhere the system can see the .dylib (/usr/local/lib or whatever) */
};
#else
#include "elfdlopennote.h"
#ifdef ELF_NOTE_DLOPEN
ELF_NOTE_DLOPEN(
    "SDL3",
    "Platform-specific operations such as creating windows and handling events",
    ELF_NOTE_DLOPEN_PRIORITY_RECOMMENDED,
    "libSDL3.so.0",
    "libSDL3.so"
);
#endif
static const char * const possible_locations[] = {
    "libSDL3.so.0",
    "libSDL3.so",
};
#endif

static void *sdl3_so;
#endif

#define SDL_NAME "SDL3"
#define OBJECT_NAME sdl3_so
#define SYMBOL_PREFIX SDL3_
#define FOREACH_SDLX_SYM FOREACH_SDL3_SYM

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

static int SDL3_Harness_SetWindowPos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    // SDL_SetWindowPosition(hWnd, x, y);
    if (nWidth == 320 && nHeight == 200) {
        nWidth = 640;
        nHeight = 400;
    }
    SDL3_SetWindowSize(hWnd, nWidth, nHeight);
    return 0;
}

static void SDL3_Harness_DestroyWindow(void) {
    // SDL3_GL_DeleteContext(context);
    if (window != NULL) {
        SDL3_DestroyWindow(window);
    }
    SDL3_Quit();
    window = NULL;
}

// Checks whether the `flag_check` is the only modifier applied.
// e.g. is_only_modifier(event.key.keysym.mod, KMOD_ALT) returns true when only the ALT key was pressed
static int is_only_key_modifier(SDL_Keymod modifier_flags, SDL_Keymod flag_check) {
    return (modifier_flags & flag_check) && (modifier_flags & (SDL_KMOD_CTRL | SDL_KMOD_SHIFT | SDL_KMOD_ALT | SDL_KMOD_GUI)) == (modifier_flags & flag_check);
}

static void SDL3_Harness_ProcessWindowMessages(void) {
    SDL_Event event;

    while (SDL3_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            if (event.key.windowID != SDL3_GetWindowID(window)) {
                continue;
            }
            if (event.key.key == SDLK_RETURN) {
                if (event.key.type == SDL_EVENT_KEY_DOWN) {
                    if ((event.key.mod & (SDL_KMOD_CTRL | SDL_KMOD_SHIFT | SDL_KMOD_ALT | SDL_KMOD_GUI))) {
                        // Ignore keydown of RETURN when used together with some modifier
                        return;
                    }
                } else if (event.key.type == SDL_EVENT_KEY_UP) {
                    if (is_only_key_modifier(event.key.mod, SDL_KMOD_ALT)) {
                        SDL3_SetWindowFullscreen(window, (SDL3_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) ? 0 : SDL_WINDOW_FULLSCREEN);
                    }
                }
            }

            // Map incoming SDL scancode to PC scan code as used by game code
            int dethrace_scancode = sdl_scancode_map[event.key.scancode];
            if (dethrace_scancode == 0) {
                LOG_WARN3("unexpected scan code %s (%d)", SDL3_GetScancodeName(event.key.scancode), event.key.scancode);
                return;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                key_state[dethrace_scancode >> 5] |= (1 << (dethrace_scancode & 0x1F));
            } else {
                key_state[dethrace_scancode >> 5] &= ~(1 << (dethrace_scancode & 0x1F));
            }
            gKeyHandler_func();
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            calculate_viewport(event.window.data1, event.window.data2);
            break;

        case SDL_EVENT_QUIT:
            QuitGame();
        }
    }
}

static void SDL3_Harness_SetKeyHandler(void (*handler_func)(void)) {
    gKeyHandler_func = handler_func;
}

static void SDL3_Harness_GetKeyboardState(br_uint_32* buffer) {
    memcpy(buffer, key_state, sizeof(key_state));
}

static int SDL3_Harness_GetMouseButtons(int* pButton1, int* pButton2) {
    if (SDL3_GetMouseFocus() != window) {
        *pButton1 = 0;
        *pButton2 = 0;
        return 0;
    }
    int state = SDL3_GetMouseState(NULL, NULL);
    *pButton1 = state & SDL_BUTTON_LMASK;
    *pButton2 = state & SDL_BUTTON_RMASK;
    return 0;
}

static int SDL3_Harness_GetMousePosition(int* pX, int* pY) {
    int window_width, window_height;
    float fWX, fWY;
    float fX, fY;

    if (SDL3_GetMouseFocus() != window) {
        return 0;
    }
    SDL3_GetWindowSize(window, &window_width, &window_height);

    SDL3_GetMouseState(&fWX, &fWY);
    if (renderer != NULL) {
        // software renderer
        SDL3_RenderCoordinatesFromWindow(renderer, fWX, fWY, &fX, &fY);
    } else {
        // hardware renderer
        // handle case where window is stretched larger than the pixel size
        fX = fWX * (640.0f / window_width);
        fY = fWY * (480.0f / window_height);
    }
    *pX = (int)fX;
    *pY = (int)fY;
    return 0;
}

static void limit_fps(void) {
    Uint32 now = SDL3_GetTicks();
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
    last_frame_time = SDL3_GetTicks();
}

static int SDL3_Harness_ShowErrorMessage(char* text, char* caption) {
    fprintf(stderr, "%s", text);
    SDL3_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, caption, text, window);
    return 0;
}

static void SDL3_Harness_CreateWindow(const char* title, int width, int height, tHarness_window_type window_type) {
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

    if (!SDL3_Init(SDL_INIT_VIDEO)) {
        LOG_PANIC2("SDL_INIT_VIDEO error: %s", SDL3_GetError());
    }

    if (window_type == eWindow_type_opengl) {

        window = SDL3_CreateWindow(title,
            window_width, window_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        if (window == NULL) {
            LOG_PANIC2("Failed to create window: %s", SDL3_GetError());
        }

        SDL3_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL3_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL3_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        gl_context = SDL3_GL_CreateContext(window);

        if (gl_context == NULL) {
            LOG_WARN2("Failed to create OpenGL core profile: %s. Trying OpenGLES...", SDL3_GetError());
            SDL3_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            SDL3_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL3_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            gl_context = SDL3_GL_CreateContext(window);
        }
        if (gl_context == NULL) {
            LOG_PANIC2("Failed to create OpenGL context: %s", SDL3_GetError());
        }
        SDL3_GL_SetSwapInterval(1);

    } else {
        window = SDL3_CreateWindow(title,
            window_width, window_height,
            SDL_WINDOW_RESIZABLE);
        if (window == NULL) {
            LOG_PANIC2("Failed to create window: %s", SDL3_GetError());
        }

        renderer = SDL3_CreateRenderer(window, NULL);
        if (renderer == NULL) {
            LOG_PANIC2("Failed to create renderer: %s", SDL3_GetError());
        }
        SDL3_SetRenderVSync(renderer, 1);
        SDL3_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL3_SetRenderLogicalPresentation(renderer, render_width, render_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        screen_texture = SDL3_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (screen_texture == NULL) {
            const SDL_PixelFormat *renderer_formats = NULL;
            SDL_PropertiesID renderer_props = SDL3_GetRendererProperties(renderer);
            if (renderer_props) {
                renderer_formats = SDL3_GetPointerProperty(renderer_props, SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER, NULL);
                if (renderer_formats) {
                    for (Uint32 i = 0; renderer_formats[i] != SDL_PIXELFORMAT_UNKNOWN; i++) {
                        LOG_INFO2("%s\n", SDL3_GetPixelFormatName(renderer_formats[i]));
                    }
                }
            }
            LOG_PANIC2("Failed to create renderer texture (%s)", SDL3_GetError());
        }
    }

    SDL3_HideCursor();

    viewport.x = 0;
    viewport.y = 0;
    viewport.scale_x = 1;
    viewport.scale_y = 1;

    if (harness_game_config.start_full_screen) {
        SDL3_SetWindowFullscreen(window, true);
    }
}

static void SDL3_Harness_Swap(br_pixelmap* back_buffer) {

    SDL3_Harness_ProcessWindowMessages();

    if (gl_context != NULL) {
        SDL3_GL_SwapWindow(window);
    } else {
        uint8_t* src_pixels = back_buffer->pixels;
        uint32_t* dest_pixels;
        int dest_pitch;

        SDL3_LockTexture(screen_texture, NULL, (void**)&dest_pixels, &dest_pitch);
        for (int i = 0; i < back_buffer->height * back_buffer->width; i++) {
            *dest_pixels = converted_palette[*src_pixels];
            dest_pixels++;
            src_pixels++;
        }
        SDL3_UnlockTexture(screen_texture);
        SDL3_RenderClear(renderer);
        SDL3_RenderTexture(renderer, screen_texture, NULL, NULL);
        SDL3_RenderPresent(renderer);
        last_screen_src = back_buffer;
    }

    if (harness_game_config.fps != 0) {
        limit_fps();
    }
}

static void SDL3_Harness_PaletteChanged(br_colour entries[256]) {
    for (int i = 0; i < 256; i++) {
        converted_palette[i] = (0xff << 24 | BR_RED(entries[i]) << 16 | BR_GRN(entries[i]) << 8 | BR_BLU(entries[i]));
    }
    if (last_screen_src != NULL) {
        SDL3_Harness_Swap(last_screen_src);
    }
}

static void SDL3_Harness_GetViewport(int* x, int* y, float* width_multipler, float* height_multiplier) {
    *x = viewport.x;
    *y = viewport.y;
    *width_multipler = viewport.scale_x;
    *height_multiplier = viewport.scale_y;
}

static void SDL3_Harness_GetPrefPath(char* path, char* app_name) {
    char* sdl_path = SDL3_GetPrefPath(NULL, app_name);
    if (sdl_path == NULL) {
        LOG_PANIC("Failed to get preferences path (%s)", SDL3_GetError());
    }
    strcpy(path, sdl_path);
    SDL3_free(sdl_path);
}

static uint32_t SDL3_Harness_GetTicks(void) {
    return SDL3_GetTicks();
}

static int SDL3_Harness_ShowCursor(int show) {
    if (show) {
        SDL3_ShowCursor();
    } else {
        SDL3_HideCursor();
    }
    return 0;
}

static void* SDL3_Harness_GL_GetProcAddress(const char* name) {
    return SDL3_GL_GetProcAddress(name);
}

static int SDL3_Harness_Platform_Init(tHarness_platform* platform) {
    if (SDL3_LoadSymbols() != 0) {
        return 1;
    }
    platform->ProcessWindowMessages = SDL3_Harness_ProcessWindowMessages;
    platform->Sleep = SDL3_Delay;
    platform->GetTicks = SDL3_Harness_GetTicks;
    platform->ShowCursor = SDL3_Harness_ShowCursor;
    platform->SetWindowPos = SDL3_Harness_SetWindowPos;
    platform->DestroyWindow = SDL3_Harness_DestroyWindow;
    platform->SetKeyHandler = SDL3_Harness_SetKeyHandler;
    platform->GetKeyboardState = SDL3_Harness_GetKeyboardState;
    platform->GetMousePosition = SDL3_Harness_GetMousePosition;
    platform->GetMouseButtons = SDL3_Harness_GetMouseButtons;
    platform->ShowErrorMessage = SDL3_Harness_ShowErrorMessage;

    platform->CreateWindow_ = SDL3_Harness_CreateWindow;
    platform->Swap = SDL3_Harness_Swap;
    platform->PaletteChanged = SDL3_Harness_PaletteChanged;
    platform->GL_GetProcAddress = SDL3_Harness_GL_GetProcAddress;
    platform->GetViewport = SDL3_Harness_GetViewport;
    platform->GetPrefPath = SDL3_Harness_GetPrefPath;
    return 0;
};

const tPlatform_bootstrap SDL3_bootstrap = {
    "sdl3",
    "SDL3 video backend (libsdl.org)",
    ePlatform_cap_software | ePlatform_cap_opengl,
    SDL3_Harness_Platform_Init,
};
