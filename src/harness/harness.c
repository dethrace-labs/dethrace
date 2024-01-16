#include "harness.h"
#include "ascii_tables.h"
#include "brender_emu/renderer_impl.h"
#include "include/harness/config.h"
#include "include/harness/hooks.h"
#include "include/harness/os.h"
#include "platforms/null.h"
#include "sound/sound.h"
#include "version.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

br_pixelmap* palette;
uint32_t* screen_buffer;
harness_br_renderer* renderer_state;

br_pixelmap* last_dst = NULL;
br_pixelmap* last_src = NULL;

unsigned int last_frame_time = 0;
int force_null_platform = 0;

extern unsigned int GetTotalTime(void);

extern br_v1db_state v1db;
extern uint32_t gI_am_cheating;

// SplatPack or Carmageddon. This is where we represent the code differences between the two. For example, the intro smack file.
tHarness_game_info harness_game_info;

// Configuration options
tHarness_game_config harness_game_config;

// Platform hooks
tHarness_platform gHarness_platform;

extern void Harness_Platform_Init(tHarness_platform* platform);

int Harness_ProcessCommandLine(int* argc, char* argv[]);

static void Harness_DetectGameMode(void) {
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
            harness_game_info.defines.requires_ascii_table = 1;
            harness_game_info.defines.ascii_table = carmageddon_german_ascii_tables.ascii;
            harness_game_info.defines.ascii_shift_table = carmageddon_german_ascii_tables.ascii_shift;
            break;
        default:
            harness_game_info.defines.ascii_table = carmageddon_ascii_tables.ascii;
            harness_game_info.defines.ascii_shift_table = carmageddon_ascii_tables.ascii_shift;
            break;
        }
        break;
    case eGame_carmageddon_demo:
        harness_game_info.defines.ascii_table = demo_ascii_tables.ascii;
        harness_game_info.defines.ascii_shift_table = demo_ascii_tables.ascii_shift;
        break;
    case eGame_splatpack_demo:
    case eGame_splatpack_xmas_demo:
        harness_game_info.defines.ascii_table = xmas_ascii_tables.ascii;
        harness_game_info.defines.ascii_shift_table = xmas_ascii_tables.ascii_shift;
        break;
    default:
        break;
    }
}

void Harness_Init(int* argc, char* argv[]) {
    int result;

    printf("Dethrace version: %s\n", DETHRACE_VERSION);

    memset(&harness_game_info, 0, sizeof(harness_game_info));

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
    // Emulate DOS behavior
    harness_game_config.dos_mode = 0;
    // Skip binding socket to allow local network testing
    harness_game_config.no_bind = 0;

    // install signal handler by default
    harness_game_config.install_signalhandler = 1;

    Harness_ProcessCommandLine(argc, argv);

    if (harness_game_config.install_signalhandler) {
        OS_InstallSignalHandler(argv[0]);
    }

    char* root_dir = getenv("DETHRACE_ROOT_DIR");
    if (root_dir != NULL) {
        LOG_INFO("DETHRACE_ROOT_DIR is set to '%s'", root_dir);
    } else {
        root_dir = OS_Dirname(argv[0]);
    }
    printf("Using root directory: %s\n", root_dir);
    result = chdir(root_dir);
    if (result != 0) {
        LOG_PANIC("Failed to chdir. Error is %s", strerror(errno));
    }

    if (harness_game_info.mode == eGame_none) {
        Harness_DetectGameMode();
    }

    if (force_null_platform) {
        Null_Platform_Init(&gHarness_platform);
    } else {
        Harness_Platform_Init(&gHarness_platform);
    }
}

// used by unit tests
void Harness_ForceNullPlatform(void) {
    force_null_platform = 1;
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
        } else if (strcasecmp(argv[i], "--dos-mode") == 0) {
            harness_game_config.dos_mode = 1;
            handled = 1;
        } else if (strcasecmp(argv[i], "--no-bind") == 0) {
            harness_game_config.no_bind = 1;
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

// Render 2d back buffer
void Harness_RenderScreen(br_pixelmap* dst, br_pixelmap* src) {
    gHarness_platform.Renderer_FullScreenQuad((uint8_t*)src->pixels);

    last_dst = dst;
    last_src = src;
}

void Harness_Hook_BrV1dbRendererBegin(br_v1db_state* v1db) {
    renderer_state = NewHarnessBrRenderer();
    v1db->renderer = (br_renderer*)renderer_state;
}

static int Harness_CalculateFrameDelay(void) {
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

void Harness_Hook_renderActor(br_actor* actor, br_model* model, br_material* material, br_token type) {
    gHarness_platform.Renderer_Model(actor, model, material, type, renderer_state->state.matrix.model_to_view);
}

// Called by game to swap buffers at end of frame rendering
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {

    // draw the current colour_buffer (2d screen) contents
    Harness_RenderScreen(dst, src);

    int delay_ms = Harness_CalculateFrameDelay();
    gHarness_platform.SwapWindow();
    if (delay_ms > 0) {
        gHarness_platform.Sleep(delay_ms);
    }

    gHarness_platform.Renderer_ClearBuffers();
    last_frame_time = GetTotalTime();
}

void Harness_RenderLastScreen(void) {
    if (last_dst) {
        Harness_RenderScreen(last_dst, last_src);
        gHarness_platform.SwapWindow();
    }
}

// Filesystem hooks
FILE* Harness_Hook_fopen(const char* pathname, const char* mode) {
    return OS_fopen(pathname, mode);
}
