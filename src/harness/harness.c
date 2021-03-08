
#include "harness.h"
#include "input/keyboard.h"
#include "stack_trace_handler.h"
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>

SDL_Window* window;
tRenderer* current_renderer;
br_pixelmap* palette;
uint32_t* screen_buffer;

br_pixelmap* last_dst = NULL;
br_pixelmap* last_src = NULL;

// if true, disable the original CD check code
int harness_disable_cd_check = 1;

int harness_debug_level = 7;

// SplatPack or Carmageddon. This is where we represent the code differences between the two. For example, the intro smack file.
tHarness_GameMode harness_game_mode;

void Harness_DetectGameMode() {
    if (access("DATA/CUTSCENE/SPLINTRO.SMK", F_OK) != -1) {
        harness_game_mode.name = "Splat Pack";
        harness_game_mode.intro_smk_file = "SPLINTRO.SMK";
    } else {
        harness_game_mode.name = "Carmageddon";
        harness_game_mode.intro_smk_file = "MIX_INTR.SMK";
    }
    LOG_INFO("\"%s\"", harness_game_mode.name);
}

void Harness_Init(char* name, tRenderer* renderer) {
    int result;

    install_signal_handler(name);
    current_renderer = renderer;
    screen_buffer = NULL;

    if (SDL_Init(SDL_INIT_TIMER) != 0) {
        LOG_PANIC("SDL_INIT_TIMER error: %s", SDL_GetError());
    }

    char* root_dir = getenv("DETHRACE_ROOT_DIR");
    if (!root_dir) {
        LOG_INFO("DETHRACE_ROOT_DIR is not set, assuming '.'");
    } else {
        printf("DETHRACE_ROOT_DIR: %s\n", root_dir);
        result = chdir(root_dir);
        if (result != 0) {
            LOG_PANIC("Failed to chdir. Error is %s", strerror(errno));
        }
    }

    Harness_DetectGameMode();
}

void Harness_Debug_PrintStack() {
#ifndef _WIN32
    posix_print_stack_trace();
#endif
}

void Harness_PumpEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            Keyboard_HandleEvent(&event.key);
            break;

        case SDL_QUIT:
            LOG_PANIC("QuitGame");
            break;
        }
    }
}

int Harness_Hook_HandleCommandLineArg(char* arg) {
    if (strcasecmp(arg, "-cdcheck") == 0) {
        harness_disable_cd_check = 0;
        return 1;
    } else if (strstr(arg, "-debug=") != NULL) {
        char* s = strstr(arg, "=");
        harness_debug_level = atoi(s + 1);
        LOG_INFO("debug level set to %d", harness_debug_level);
        return 1;
    }
    return 0;
}

void Harness_Hook_DOSGfxBegin() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_PANIC("SDL_INIT_VIDEO error: %s", SDL_GetError());
    }

    window = SDL_CreateWindow("Dethrace",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        680, 480,
        current_renderer->get_window_flags());

    if (!window) {
        LOG_PANIC("Failed to create window");
    }
    current_renderer->init(window);
}

void Harness_Hook_PDServiceSystem(int pTime_since_last_call) {
    Harness_PumpEvents();
}

void Harness_RenderScreen(br_pixelmap* dst, br_pixelmap* src) {
    uint8_t palette_index = 0;
    int inc = 0;
    uint8_t* data = src->pixels;
    uint32_t* colors = palette->pixels;
    uint32_t true_color;
    int x;
    int y;

    if (screen_buffer == NULL) {
        screen_buffer = malloc(src->width * src->height * sizeof(uint32_t));
        memset(screen_buffer, 0, src->width * src->height * sizeof(uint32_t));
    }

    // generate 32 bit texture from src + palette
    for (y = 0; y < src->height; y++) {
        inc = 0;
        for (x = 0; x < src->width; x++) {

            palette_index = (data[y * src->row_bytes + x]);
            true_color = colors[palette_index];
            screen_buffer[y * src->width + x] = true_color;
        }
    }
    current_renderer->doubleBuffer(screen_buffer, window);
    Harness_PumpEvents();
}

void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {
    last_dst = dst;
    last_src = src;
    Harness_RenderScreen(dst, src);
}
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm) {
    palette = pm;
    if (last_dst && last_src) {
        Harness_RenderScreen(last_dst, last_src);
    }
}

void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour) {
    if (palette != NULL) {
        uint32_t* colors = palette->pixels;
        colors[i] = colour;
    }
}

void Harness_Hook_KeyBegin() {
    Keyboard_Init();
}

int Harness_Hook_KeyDown(unsigned char pScan_code) {
    return Keyboard_IsKeyDown(pScan_code);
}