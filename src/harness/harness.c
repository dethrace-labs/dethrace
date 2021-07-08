
#include "harness.h"
#include "input/keyboard.h"
#include "rendering/renderer_state.h"
#include "sound/sound.h"
#include "stack_trace_handler.h"
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

SDL_Window* window;
tRenderer* current_renderer;
br_pixelmap* palette;
uint32_t* screen_buffer;
harness_br_renderer* renderer_state;

br_pixelmap* last_dst = NULL;
br_pixelmap* last_src = NULL;

// if true, disable the original CD check code
int harness_disable_cd_check = 1;

int harness_debug_level = 7;

int back_screen_is_transparent = 0;

extern void BrPixelmapFill(br_pixelmap* dst, br_uint_32 colour);

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
    int x;
    int y;

    if (screen_buffer == NULL) {
        screen_buffer = malloc(src->width * src->height * sizeof(uint32_t));
    }

    // generate 32 bit texture from src + palette
    for (y = 0; y < src->height; y++) {
        inc = 0;
        for (x = 0; x < src->width; x++) {
            palette_index = (data[y * src->row_bytes + x]);
            screen_buffer[y * src->width + x] = colors[palette_index];
        }
    }
    current_renderer->renderScreenBuffer(screen_buffer, back_screen_is_transparent);
    Harness_PumpEvents();

    last_dst = dst;
    last_src = src;
}

void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {
    Harness_RenderScreen(dst, src);
    current_renderer->swap(window);
    back_screen_is_transparent = 0;
}
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm) {
    palette = pm;
    if (last_src) {
        Harness_RenderScreen(last_dst, last_src);
        current_renderer->swap(window);
    }
}

void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour) {
    if (palette != NULL) {
        uint32_t* colors = palette->pixels;
        colors[i] = colour;
    }
}

void Harness_Hook_BrV1dbRendererBegin(br_v1db_state* v1db) {
    renderer_state = NewRendererState();
    v1db->renderer = (br_renderer*)renderer_state;
}

void Harness_Hook_renderFaces(v11model* model, br_material* material, br_token type) {
}

void Harness_Hook_BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    // splat current back_screen to framebuffer
    Harness_RenderScreen(NULL, colour_buffer);
    // clear to transparent ready for the game to render foreground bits
    BrPixelmapFill(colour_buffer, 0);
    back_screen_is_transparent = 1;

    current_renderer->renderFrameBegin();
}

void Harness_Hook_BrZbSceneRenderAdd(br_actor* tree) {
}

void Harness_Hook_BrZbSceneRenderEnd() {
}

void Harness_Hook_KeyBegin() {
    Keyboard_Init();
}

int Harness_Hook_KeyDown(unsigned char pScan_code) {
    return Keyboard_IsKeyDown(pScan_code);
}

void Harness_Hook_S3Service(int unk1, int unk2) {
    Sound_Service();
}

void Harness_Hook_S3StopAllOutletSounds() {
}