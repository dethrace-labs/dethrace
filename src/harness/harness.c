
#include "harness.h"
#include "brender_emu/renderer_impl.h"
#include "include/harness/config.h"
#include "io_platforms/io_platform.h"
#include "os/os.h"
#include "renderers/null.h"
#include "sound/sound.h"

#include <errno.h>
#include <stdio.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

tRenderer* renderer;
br_pixelmap* palette;
uint32_t* screen_buffer;
harness_br_renderer* renderer_state;

br_pixelmap* last_dst = NULL;
br_pixelmap* last_src = NULL;

br_pixelmap *last_colour_buffer, *last_depth_buffer;

unsigned int last_frame_time = 0;
int force_nullrenderer = 0;

extern unsigned int GetTotalTime();
extern uint8_t gScan_code[123][2];
extern br_v1db_state v1db;

// SplatPack or Carmageddon. This is where we represent the code differences between the two. For example, the intro smack file.
tHarness_game_info harness_game_info;

// Configuration options
tHarness_game_config harness_game_config;

int Harness_ProcessCommandLine(int* argc, char* argv[]);

void Harness_DetectGameMode() {
    if (access("DATA/RACES/CITY01.TXT", F_OK) == -1 && access("DATA/RACES/CITYA1.TXT", F_OK) == -1) {
        harness_game_info.defines.INTRO_SMK_FILE = "";
        harness_game_info.defines.GERMAN_LOADSCRN = "COWLESS.PIX";
        harness_game_info.mode = eGame_carmageddon_demo;
        LOG_INFO("\"%s\"", "Carmageddon demo");
    } else if (access("DATA/CUTSCENE/SPLINTRO.SMK", F_OK) != -1) {
        harness_game_info.defines.INTRO_SMK_FILE = "SPLINTRO.SMK";
        harness_game_info.defines.GERMAN_LOADSCRN = "LOADSCRN.PIX";
        harness_game_info.mode = eGame_splatpack;
        LOG_INFO("\"%s\"", "Splat Pack");
    } else {
        harness_game_info.defines.INTRO_SMK_FILE = "MIX_INTR.SMK";
        harness_game_info.defines.GERMAN_LOADSCRN = "LOADSCRN.PIX";
        harness_game_info.mode = eGame_carmageddon;
        LOG_INFO("\"%s\"", "Carmageddon");
    }
}

void Harness_Init(int* argc, char* argv[]) {
    int result;

    // disable the original CD check code
    harness_game_config.disable_cd_check = 1;
    // original physics time step. Lower values seem to work better at 30+ fps
    harness_game_config.physics_step_time = 40;
    // do not limit fps by default
    harness_game_config.fps = 0;
    // do not freeze timer
    harness_game_config.freeze_timer = 0;

    // install signal handler by default
    harness_game_config.install_signalhandler = 1;

    Harness_ProcessCommandLine(argc, argv);

    if (harness_game_config.install_signalhandler) {
        OS_InstallSignalHandler(argv[0]);
    }

    int* keymap = Input_GetKeyMap();
    if (keymap != NULL) {
        for (int i = 0; i < 123; i++) {
            gScan_code[i][0] = keymap[i];
            // gScan_code[i][1] = keymap[i];
        }
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
    if (harness_game_info.mode == eGame_none) {
        Harness_DetectGameMode();
    }
}

void Harness_ForceNullRenderer() {
    force_nullrenderer = 1;
    renderer = &null_renderer;
}

void Harness_Debug_PrintStack() {
    OS_PrintStacktrace();
}

int Harness_ProcessCommandLine(int* argc, char* argv[]) {
    for (int i = 1; i < *argc; i++) {
        int handled = 0;

        if (strcasecmp(argv[i], "--cdcheck") == 0) {
            harness_game_config.disable_cd_check = 0;
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

void Harness_Hook_DOSGfxBegin() {
    if (force_nullrenderer) {
        return;
    }
    renderer = Window_Create("Dethrace", 640, 400, 320, 200);
}

// Render 2d back buffer
void Harness_RenderScreen(br_pixelmap* dst, br_pixelmap* src) {
    renderer->FullScreenQuad((uint8_t*)src->pixels, 320, 200);

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
    last_colour_buffer = colour_buffer;
    last_depth_buffer = depth_buffer;
    renderer->BeginScene(camera, colour_buffer);
}

void Harness_Hook_BrZbSceneRenderAdd(br_actor* tree) {
}

void Harness_Hook_renderFaces(br_actor* actor, br_model* model, br_material* material, br_token type) {
    renderer->Model(actor, model, renderer_state->state.matrix.model_to_view);
}

void Harness_Hook_BrZbSceneRenderEnd() {
    renderer->EndScene();
}

// Called by game to swap buffers at end of frame rendering
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {

    // draw the current colour_buffer (2d screen) contents
    renderer->FlushBuffers(last_colour_buffer, last_depth_buffer);
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

void Harness_Hook_S3Service(int unk1, int unk2) {
    Sound_Service();
}

void Harness_Hook_S3StopAllOutletSounds() {
}

void Harness_Hook_FlushRenderer() {
    renderer->FlushBuffers(last_colour_buffer, last_depth_buffer);
}
