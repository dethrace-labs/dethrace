
#include "harness.h"
#include "include/harness/config.h"
#include "platforms/sdl_gl.h"
#include "rendering/renderer_state.h"
#include "sound/sound.h"
#include "stack_trace_handler.h"

#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

tPlatform* platform;
br_pixelmap* palette;
uint32_t* screen_buffer;
harness_br_renderer* renderer_state;

br_pixelmap* last_dst = NULL;
br_pixelmap* last_src = NULL;

// if true, disable the original CD check code
int harness_disable_cd_check = 1;

int back_screen_is_transparent = 0;

extern void BrPixelmapFill(br_pixelmap* dst, br_uint_32 colour);
extern uint8_t gScan_code[123][2];

// SplatPack or Carmageddon. This is where we represent the code differences between the two. For example, the intro smack file.
tHarness_game_info harness_game_info;

void Harness_DetectGameMode() {

    if (access("DATA/RACES/CITY01.TXT", F_OK) == -1 && access("DATA/RACES/CITYA1.TXT", F_OK) == -1) {
        harness_game_info.intro_smk_file = "";
        harness_game_info.mode = eGame_carmageddon_demo;
        LOG_INFO("\"%s\"", "Carmageddon demo");
    } else if (access("DATA/CUTSCENE/SPLINTRO.SMK", F_OK) != -1) {
        harness_game_info.intro_smk_file = "SPLINTRO.SMK";
        LOG_INFO("\"%s\"", "Splat Pack");
    } else {
        harness_game_info.intro_smk_file = "MIX_INTR.SMK";
        LOG_INFO("\"%s\"", "Carmageddon");
    }
}

void Harness_Init(char* name, char* platform_name) {
    int result;

    if (strcmp(platform_name, "sdl_gl") == 0) {
        platform = &sdl_gl_platform;
    } else if (strcmp(platform_name, "null") == 0) {
        platform = NULL;
    } else {
        LOG_PANIC("Unknown platform name %s", platform_name);
    }

    screen_buffer = NULL;

    install_signal_handler(name);
    platform->Init();

    int* keymap = platform->GetKeyMap();
    for (int i = 0; i < 123; i++) {
        gScan_code[i][0] = keymap[i];
        //gScan_code[i][1] = keymap[i];
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
    platform->PollEvents();
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
    platform->CreateWindow("Dethrace", 640, 480);
}

void Harness_RenderScreen(br_pixelmap* dst, br_pixelmap* src) {
    uint8_t palette_index = 0;
    uint8_t* data = src->pixels;
    uint32_t* colors = palette->pixels;
    int x;
    int y;

    if (screen_buffer == NULL) {
        screen_buffer = malloc(src->width * src->height * sizeof(uint32_t));
    }

    // generate 32 bit texture from src + palette
    for (y = 0; y < src->height; y++) {
        for (x = 0; x < src->width; x++) {
            palette_index = (data[y * src->row_bytes + x]);
            screen_buffer[y * src->width + x] = colors[palette_index];
        }
    }

    platform->RenderFullScreenQuad(screen_buffer, back_screen_is_transparent);
    Harness_PumpEvents();

    last_dst = dst;
    last_src = src;
}

void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {
    Harness_RenderScreen(dst, src);
    platform->Swap();
    back_screen_is_transparent = 0;
}
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm) {
    palette = pm;
    if (last_src) {
        Harness_RenderScreen(last_dst, last_src);
        platform->Swap();
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

int col = 128;

void Harness_Hook_renderFaces(br_model* model, br_material* material, br_token type) {
    platform->RenderModel(model, renderer_state->state.matrix.model_to_view);
}

void Harness_Hook_BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    // splat current back_screen to framebuffer
    Harness_RenderScreen(NULL, colour_buffer);
    // clear to transparent ready for the game to render foreground bits
    BrPixelmapFill(colour_buffer, 0);
    back_screen_is_transparent = 1;

    //current_renderer->setViewport(colour_buffer->base_x * 2, colour_buffer->base_y * 2, colour_buffer->width * 2, colour_buffer->height * 2);
    platform->BeginFrame(camera, colour_buffer);
    col = 0;
}

void Harness_Hook_BrZbSceneRenderAdd(br_actor* tree) {
}

void Harness_Hook_BrZbSceneRenderEnd() {
    platform->EndFrame();
}

int Harness_Hook_KeyDown(unsigned char pScan_code) {
    return platform->IsKeyDown(pScan_code);
}

void PlatformHooks_PollEvents() {
    Harness_PumpEvents();
}

void Harness_Hook_S3Service(int unk1, int unk2) {
    Sound_Service();
}

void Harness_Hook_S3StopAllOutletSounds() {
}