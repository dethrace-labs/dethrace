#include "harness.h"
#include "brender_emu/renderer_impl.h"
#include "include/harness/config.h"
#include "include/harness/os.h"
#include "io_platforms/io_platform.h"
#include "renderers/null.h"
#include "sound/sound.h"
#include "version.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

tRenderer* renderer;
br_pixelmap* palette;
uint32_t* screen_buffer;
harness_br_renderer* renderer_state;

br_pixelmap* last_dst = NULL;
br_pixelmap* last_src = NULL;

unsigned int last_frame_time = 0;
int force_nullrenderer = 0;

extern unsigned int GetTotalTime();
extern uint8_t gScan_code[123][2];
extern br_v1db_state v1db;
extern uint32_t gI_am_cheating;

// SplatPack or Carmageddon. This is where we represent the code differences between the two. For example, the intro smack file.
tHarness_game_info harness_game_info;

// Configuration options
tHarness_game_config harness_game_config;

/* clang-format off */
// German ASCII codes
static int carmageddon_german_ascii_table[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
    108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 122, 121, 94, -33, -76, 8, 13, 13, 0, 45, 60, -10, -28, 46, 44, -4, 43, 35, 27,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4, 56, -33, -76, 46, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static int carmageddon_german_ascii_shift_table[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 61, 33, 34, -89, 36, 37, 38, 47, 40, 41, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 90, 89, -80, 63, 96, 8, 13, 13, 0, 95, 62, -42, -60, 58, 44, -36, 42, 39, 27,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4, 56, -33, -76, 46, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Demo ASCII codes
static int demo_ascii_table[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
    108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 122, 121, 94, -33, -76, 8, 13, 13, 0, 45, 60, -10, -28, 46, 44, -4, 43, 35, 27,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4, 56, -33, -76, 46, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static int demo_ascii_shift_table[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 61, 33, 34, -89, 36, 37, 38, 47, 40, 41, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 90, 89, -80, 63, 96, 8, 13, 13, 0, 95, 62, -42, -60, 58, 44, -36, 42, 39, 27,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4, 56, -33, -76, 46, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Splatpack Demo ASCII codes
static int splatpack_xmasdemo_ascii_table[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 96, 45, 61, 8, 13, 3, 9, 47, 92, 59, 39, 46, 44, 91, 93, 35, 27,
    0, 127, 0, 0, 0, 0, 28, 29, 30, 31, 0, 47, 42, 45, 43, 46, 61, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
};
static int splatpack_xmasdemo_ascii_shift_table[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 33, 34, 163, 36, 37, 94, 38, 42, 40, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 172, 95, 43, 8, 13, 13, 0, 63, 124, 58, 64, 62, 44, 123, 125, 126, 27,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 42, 45, 43, 46, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 8,
};
/* clang-format on */

int Harness_ProcessCommandLine(int* argc, char* argv[]);

void Harness_DetectGameMode() {
    if (access("DATA/RACES/CASTLE.TXT", F_OK) != -1) {
        // All splatpack edition have the castle track
        if (access("DATA/RACES/CASTLE2.TXT", F_OK) != -1) {
            // Only the full splat release has the castle2 track
            harness_game_info.defines.INTRO_SMK_FILE = "SPLINTRO.SMK";
            harness_game_info.defines.GERMAN_LOADSCRN = "LOADSCRN.PIX";
            harness_game_info.mode = eGame_splatpack;
            printf("Game mode: Splat Pack\n");
        } else if (access("DATA/RACES/TINSEL.TXT", F_OK) != -1) {
            // Only the the splat x-mas demo has the tinsel track
            harness_game_info.defines.INTRO_SMK_FILE = "MIX_INTR.SMK";
            harness_game_info.defines.GERMAN_LOADSCRN = "";
            harness_game_info.mode = eGame_splatpack_xmas_demo;
            printf("Game mode: Splat Pack X-mas demo\n");
        } else {
            // Assume we're using the splatpack demo
            harness_game_info.defines.INTRO_SMK_FILE = "MIX_INTR.SMK";
            harness_game_info.defines.GERMAN_LOADSCRN = "";
            harness_game_info.mode = eGame_splatpack_demo;
            printf("Game mode: Splat Pack demo\n");
        }
    } else if (access("DATA/RACES/CITYB3.TXT", F_OK) != -1) {
        // All non-splatpack edition have the cityb3 track
        if (access("DATA/RACES/CITYA1.TXT", F_OK) == -1) {
            // The demo does not have the citya1 track
            harness_game_info.defines.INTRO_SMK_FILE = "";
            harness_game_info.defines.GERMAN_LOADSCRN = "COWLESS.PIX";
            harness_game_info.mode = eGame_carmageddon_demo;
            printf("Game mode: Carmageddon demo\n");
        } else {
            goto carmageddon;
        }
    } else {
    carmageddon:
        if (access("DATA/CUTSCENE/Mix_intr.smk", F_OK) == -1) {
            harness_game_info.defines.INTRO_SMK_FILE = "Mix_intr.smk";
        } else {
            harness_game_info.defines.INTRO_SMK_FILE = "MIX_INTR.SMK";
        }
        harness_game_info.defines.GERMAN_LOADSCRN = "LOADSCRN.PIX";
        harness_game_info.mode = eGame_carmageddon;
        printf("Game mode: Carmageddon\n");
    }

    harness_game_info.localization = eGameLocalization_none;
    if (access("DATA/TRNSLATE.TXT", F_OK) != -1) {
        FILE* f = fopen("DATA/TRNSLATE.TXT", "rb");
        fseek(f, 0, SEEK_END);
        int filesize = ftell(f);
        fseek(f, 0, SEEK_SET);
        char* buffer = malloc(filesize + 1);
        int nb = fread(buffer, 1, filesize, f);
        if (nb != filesize) {
            LOG_PANIC("Unable to read DATA/TRNSLATE.TXT");
        }
        buffer[filesize] = '\0';
        fclose(f);
        if (strstr(buffer, "NEUES SPIEL") != NULL) {
            harness_game_info.localization = eGameLocalization_german;
            LOG_INFO("Language: \"%s\"", "German");
        } else {
            LOG_INFO("Language: unrecognized");
        }
        free(buffer);
    }

    switch (harness_game_info.mode) {
    case eGame_carmageddon:
        switch (harness_game_info.localization) {
        case eGameLocalization_german:
            harness_game_info.defines.ascii_table = carmageddon_german_ascii_table;
            harness_game_info.defines.ascii_shift_table = carmageddon_german_ascii_shift_table;
            break;
        default:
            break;
        }
        break;
    case eGame_carmageddon_demo:
        harness_game_info.defines.ascii_table = demo_ascii_table;
        harness_game_info.defines.ascii_shift_table = demo_ascii_shift_table;
        break;
    case eGame_splatpack_demo:
    case eGame_splatpack_xmas_demo:
        harness_game_info.defines.ascii_table = splatpack_xmasdemo_ascii_table;
        harness_game_info.defines.ascii_shift_table = splatpack_xmasdemo_ascii_shift_table;
        break;
    default:
        break;
    }
}

void Harness_Init(int* argc, char* argv[]) {
    int result;

    printf("Dethrace version: %s\n", DETHRACE_VERSION);

    // disable the original CD check code
    harness_game_config.enable_cd_check = 0;
    // original physics time step. Lower values seem to work better at 30+ fps
    harness_game_config.physics_step_time = 40;
    // do not limit fps by default
    harness_game_config.fps = 0;
    // do not freeze timer
    harness_game_config.freeze_timer = 0;
    // default demo time out is 240s
    harness_game_config.demo_timeout = 240000;
    // disable developer diagnostics by default
    harness_game_config.enable_diagnostics = 0;
    // no volume multiplier
    harness_game_config.volume_multiplier = 1.0f;
    // start window in windowed mode
    harness_game_config.start_full_screen = 0;
    // disable replay by default
    harness_game_config.enable_replay = 0;

    // install signal handler by default
    harness_game_config.install_signalhandler = 1;

    Harness_ProcessCommandLine(argc, argv);

    if (harness_game_config.install_signalhandler) {
        OS_InstallSignalHandler(argv[0]);
    }

    char* root_dir = getenv("DETHRACE_ROOT_DIR");
    if (root_dir == NULL) {
        LOG_INFO("DETHRACE_ROOT_DIR is not set, assuming '.'");
    } else {
        printf("Data directory: %s\n", root_dir);
        result = chdir(root_dir);
        if (result != 0) {
            LOG_PANIC("Failed to chdir. Error is %s", strerror(errno));
        }
    }
    if (harness_game_info.mode == eGame_none) {
        Harness_DetectGameMode();
    }

    Input_Init();
    int* keymap = Input_GetKeyMap();
    if (keymap != NULL) {
        for (int i = 0; i < 123; i++) {
            gScan_code[i][0] = keymap[i];
            // gScan_code[i][1] = keymap[i];
        }
    }
}

// used by unit tests
void Harness_ForceNullRenderer() {
    force_nullrenderer = 1;
    renderer = &null_renderer;
}

int Harness_ProcessCommandLine(int* argc, char* argv[]) {
    for (int i = 1; i < *argc; i++) {
        int handled = 0;

        if (strcasecmp(argv[i], "--cdcheck") == 0) {
            harness_game_config.enable_cd_check = 1;
            handled = 1;
        } else if (strstr(argv[i], "--debug=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_debug_level = atoi(s + 1);
            LOG_INFO("debug level set to %d", harness_debug_level);
            handled = 1;
        } else if (strstr(argv[i], "--physics-step-time=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_game_config.physics_step_time = atof(s + 1);
            LOG_INFO("Physics step time set to %f", harness_game_config.physics_step_time);
            handled = 1;
        } else if (strstr(argv[i], "--fps=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_game_config.fps = atoi(s + 1);
            LOG_INFO("FPS limiter set to %f", harness_game_config.fps);
            handled = 1;
        } else if (strcasecmp(argv[i], "--freeze-timer") == 0) {
            LOG_INFO("Timer frozen");
            harness_game_config.freeze_timer = 1;
            handled = 1;
        } else if (strcasecmp(argv[i], "--no-signal-handler") == 0) {
            LOG_INFO("Don't install the signal handler");
            harness_game_config.install_signalhandler = 0;
            handled = 1;
        } else if (strstr(argv[i], "--demo-timeout=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_game_config.demo_timeout = atoi(s + 1) * 1000;
            LOG_INFO("Demo timeout set to %d milliseconds", harness_game_config.demo_timeout);
            handled = 1;
        } else if (strcasecmp(argv[i], "--i-am-cheating") == 0) {
            gI_am_cheating = 0xa11ee75d;
            handled = 1;
        } else if (strcasecmp(argv[i], "--enable-diagnostics") == 0) {
            harness_game_config.enable_diagnostics = 1;
            handled = 1;
        } else if (strstr(argv[i], "--volume-multiplier=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_game_config.volume_multiplier = atof(s + 1);
            LOG_INFO("Volume multiplier set to %f", harness_game_config.volume_multiplier);
            handled = 1;
        } else if (strcasecmp(argv[i], "--full-screen") == 0) {
            harness_game_config.start_full_screen = 1;
            handled = 1;
        } else if (strcasecmp(argv[i], "--enable-replay") == 0) {
            harness_game_config.enable_replay = 1;
            handled = 1;
        }

        if (handled) {
            // shift args downwards
            for (int j = i; j < *argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            (*argc)--;
            i--;
        }
    }

    return 0;
}

void Harness_Hook_GraphicsInit(int render_width, int render_height) {
    int window_width, window_height;
    if (force_nullrenderer) {
        return;
    }
    if (render_width == 320) {
        window_width = render_width * 2;
        window_height = render_height * 2;
    } else {
        window_width = render_width;
        window_height = render_height;
    }
    renderer = Window_Create("Dethrace", window_width, window_height, render_width, render_height);
}

void Harness_Hook_PDShutdownSystem() {
    IOPlatform_Shutdown();
}

// Render 2d back buffer
void Harness_RenderScreen(br_pixelmap* dst, br_pixelmap* src) {
    renderer->FullScreenQuad((uint8_t*)src->pixels);

    last_dst = dst;
    last_src = src;
}

void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm) {
    renderer->SetPalette((uint8_t*)pm->pixels);
    palette = pm;

    if (last_dst) {
        Harness_RenderScreen(last_dst, last_src);
        Window_Swap(0);
    }
}

void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour) {
    if (palette != NULL) {
        uint32_t* colors = palette->pixels;
        colors[i] = colour;
    }
}

void Harness_Hook_BrV1dbRendererBegin(br_v1db_state* v1db) {
    renderer_state = NewHarnessBrRenderer();
    v1db->renderer = (br_renderer*)renderer_state;
}

int Harness_CalculateFrameDelay() {
    if (harness_game_config.fps == 0) {
        return 0;
    }

    unsigned int now = GetTotalTime();

    if (last_frame_time != 0) {
        unsigned int frame_time = now - last_frame_time;
        last_frame_time = now;
        if (frame_time < 100) {
            int sleep_time = (1000 / harness_game_config.fps) - frame_time;
            if (sleep_time > 5) {
                return sleep_time;
            }
        }
    }
    return 0;
}

// Begin 3d scene
void Harness_Hook_BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    renderer->BeginScene(camera, colour_buffer, depth_buffer);
}

void Harness_Hook_renderActor(br_actor* actor, br_model* model, br_material* material, br_token type) {
    renderer->Model(actor, model, renderer_state->state.matrix.model_to_view, type);
}

void Harness_Hook_BrZbSceneRenderEnd() {
    renderer->FlushBuffers(eFlush_all);
    renderer->EndScene();
}

// Called by game to swap buffers at end of frame rendering
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {

    // draw the current colour_buffer (2d screen) contents
    Harness_RenderScreen(dst, src);

    int delay_ms = Harness_CalculateFrameDelay();
    Window_Swap(delay_ms);

    renderer->ClearBuffers();
    Window_PollEvents();

    last_frame_time = GetTotalTime();
}

int Harness_Hook_KeyDown(unsigned char pScan_code) {
    return Input_IsKeyDown(pScan_code);
}

void Harness_Hook_PDServiceSystem() {
    Window_PollEvents();
}
void Harness_Hook_PDSetKeyArray() {
    Window_PollEvents();
}

void Harness_Hook_FlushRenderer() {
    renderer->FlushBuffers(eFlush_all);
}

void Harness_Hook_BrMaterialUpdate(br_material* mat, br_uint_16 flags) {
    renderer->BufferMaterial(mat);
}

void Harness_Hook_BrBufferUpdate(br_pixelmap* pm, br_token use, br_uint_16 flags) {
    if (use == BRT_COLOUR_MAP_O || use == BRT_UNKNOWN) {
        renderer->BufferTexture(pm);
    } else {
        LOG_PANIC("use %d", use);
    }
}

void Harness_Hook_BrModelUpdate(br_model* model) {
    renderer->BufferModel(model);
}

// Input hooks
void Harness_Hook_GetMousePosition(int* pX, int* pY) {
    Input_GetMousePosition(pX, pY);
}

void Harness_Hook_GetMouseButtons(int* pButton1, int* pButton2) {
    Input_GetMouseButtons(pButton1, pButton2);
}

// Sound hooks
void Harness_Hook_S3Service(int unk1, int unk2) {
    Sound_Service();
}

void Harness_Hook_S3StopAllOutletSounds() {
}

// Filesystem hooks
FILE* Harness_Hook_fopen(const char* pathname, const char* mode) {
    return OS_fopen(pathname, mode);
}
