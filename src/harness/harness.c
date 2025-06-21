#include "harness.h"
#include "ascii_tables.h"
#include "include/harness/config.h"
#include "include/harness/hooks.h"
#include "include/harness/os.h"
#include "platforms/null.h"
#include "version.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

extern br_uint_32 gI_am_cheating;

extern void Harness_Platform_Init(tHarness_platform* platform);

extern const tPlatform_bootstrap SDL1_bootstrap;
extern const tPlatform_bootstrap SDL2_bootstrap;

static const tPlatform_bootstrap* platform_bootstraps[] = {
#ifdef DETHRACE_PLATFORM_SDL2
    &SDL2_bootstrap,
#endif
#ifdef DETHRACE_PLATFORM_SDL1
    &SDL1_bootstrap,
#endif
};

// SplatPack or Carmageddon. This is where we represent the code differences between the two. For example, the intro smack file.
tHarness_game_info harness_game_info;

// Configuration options
tHarness_game_config harness_game_config;

// Platform hooks
tHarness_platform gHarness_platform;

static int force_null_platform = 0;

typedef struct {
    const char* platform_name;
    int platform_capabilityies;
    int install_signalhandler;
} tArgument_config;

static int Harness_ProcessCommandLine(tArgument_config* argument_config, int* argc, char* argv[]);

static void Harness_DetectGameMode(void) {
    FILE* f;
    int filesize;
    char* buffer;
    int nb;

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
        f = fopen("DATA/TRNSLATE.TXT", "rb");
        fseek(f, 0, SEEK_END);
        filesize = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(filesize + 1);
        nb = fread(buffer, 1, filesize, f);
        if (nb != filesize) {
            LOG_PANIC("Unable to read DATA/TRNSLATE.TXT");
        }
        buffer[filesize] = '\0';
        fclose(f);
        if (strstr(buffer, "NEUES SPIEL") != NULL) {
            harness_game_info.localization = eGameLocalization_german;
            LOG_INFO2("Language: \"%s\"", "German");
        } else if (strstr(buffer, "NOWA GRA") != NULL) {
            harness_game_info.localization = eGameLocalization_polish;
            LOG_INFO2("Language: \"%s\"", "Polish");
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

    // 3dfx code paths require at least smoke.pix which is used instead of writing smoke directly to framebuffer
    if (access("DATA/PIXELMAP/SMOKE.PIX", F_OK) != -1) {
        harness_game_info.data_dir_has_3dfx_assets = 1;
    }
}

int Harness_Init(int* argc, char* argv[]) {
    int result;
    char* root_dir;
    tArgument_config argument_config;

    printf("Dethrace version: %s\n", DETHRACE_VERSION);

    memset(&harness_game_info, 0, sizeof(harness_game_info));

    // disable the original CD check code
    harness_game_config.enable_cd_check = 0;
    // original physics time step. Lower values seem to work better at 30+ fps
    harness_game_config.physics_step_time = 40;
    // limit to 60 fps by default
    harness_game_config.fps = 60;
    // do not freeze timer
    harness_game_config.freeze_timer = 0;
    // default demo time out is 240s
    harness_game_config.demo_timeout = 240000;
    // disable developer diagnostics by default
    harness_game_config.enable_diagnostics = 0;
    // no volume multiplier
    harness_game_config.volume_multiplier = 1.0f;
    // start window in windowed mode
    harness_game_config.start_full_screen = 1;
    // Disable gore check emulation
    harness_game_config.gore_check = 0;
    // Disable "Sound Options" menu
    harness_game_config.sound_options = 0;
    // Skip binding socket to allow local network testing
    harness_game_config.no_bind = 0;
    // Disable verbose logging
    harness_game_config.verbose = 0;

    // don't require a particular platform
    argument_config.platform_name = NULL;
    // request software renderer capability
    argument_config.platform_capabilityies = ePlatform_cap_software;
    // install signal handler
    argument_config.install_signalhandler = 1;

    if (Harness_ProcessCommandLine(&argument_config, argc, argv) != 0) {
        fprintf(stderr, "Failed to parse harness command line\n");
        return 1;
    }

    if (argument_config.install_signalhandler) {
        OS_InstallSignalHandler(argv[0]);
    }

    root_dir = getenv("DETHRACE_ROOT_DIR");
    if (root_dir != NULL) {
        LOG_INFO2("DETHRACE_ROOT_DIR is set to '%s'", root_dir);
    } else {
        root_dir = OS_GetWorkingDirectory(argv[0]);
    }
    // if root_dir is null or empty, no need to chdir
    if (root_dir != NULL && root_dir[0] != '\0') {
        printf("Using root directory: %s\n", root_dir);
        result = chdir(root_dir);
        if (result != 0) {
            LOG_PANIC2("Failed to chdir. Error is %s", strerror(errno));
        }
    }

    if (harness_game_info.mode == eGame_none) {
        Harness_DetectGameMode();
    }

    if (harness_game_config.opengl_3dfx_mode && !harness_game_info.data_dir_has_3dfx_assets) {
        printf("Error: data directory does not contain 3dfx assets so opengl mode cannot be used\n");
        exit(1);
    }

    if (force_null_platform) {
        Null_Platform_Init(&gHarness_platform);
    } else {
        const tPlatform_bootstrap* selected_bootstrap = NULL;

        if (argument_config.platform_name != NULL) {
            size_t i;
            for (i = 0; i < BR_ASIZE(platform_bootstraps); i++) {
                if (strcasecmp(platform_bootstraps[i]->name, argument_config.platform_name) == 0) {
                    if ((platform_bootstraps[i]->capabilities & argument_config.platform_capabilityies) != argument_config.platform_capabilityies) {
                        fprintf(stderr, "Platform \"%s\" does not support requested capabilities. Try another video driver and/or add/remove --opengl\n", selected_bootstrap->name);
                        return 1;
                    }
                    selected_bootstrap = platform_bootstraps[i];
                    break;
                }
            }
            if (selected_bootstrap == NULL) {
                fprintf(stderr, "Could not find a platform named \"%s\"\n", argument_config.platform_name);
                return 1;
            }
            if (selected_bootstrap->init(&gHarness_platform) != 0) {
                fprintf(stderr, "%s initialization failed\n", selected_bootstrap->name);
                return 1;
            }
        } else {
            size_t i;
            for (i = 0; i < BR_ASIZE(platform_bootstraps); i++) {
                LOG_DEBUG2("Attempting video driver \"%s\"", platform_bootstraps[i]->name);
                if ((platform_bootstraps[i]->capabilities & argument_config.platform_capabilityies) != argument_config.platform_capabilityies) {
                    fprintf(stderr, "Skipping platform \"%s\". Does not support required capabilities.\n", platform_bootstraps[i]->name);
                    continue;
                }
                LOG_DEBUG2("Try platform \"%s\"...", platform_bootstraps[i]->name);
                if (platform_bootstraps[i]->init(&gHarness_platform) == 0) {
                    selected_bootstrap = platform_bootstraps[i];
                    break;
                }
            }
            if (selected_bootstrap == NULL) {
                fprintf(stderr, "Could not find a supported platform\n");
                return 1;
            }
        }
        if (selected_bootstrap == NULL) {
            fprintf(stderr, "Could not find a supported platform\n");
            return 1;
        }
        LOG_INFO3("Platform: %s (%s)", selected_bootstrap->name, selected_bootstrap->description);
    }
    return 0;
}

// used by unit tests
void Harness_ForceNullPlatform(void) {
    force_null_platform = 1;
}

int Harness_ProcessCommandLine(tArgument_config* config, int* argc, char* argv[]) {
    int i, j;
    for (i = 1; i < *argc;) {
        int consumed = -1;

        if (strcasecmp(argv[i], "--cdcheck") == 0) {
            harness_game_config.enable_cd_check = 1;
            consumed = 1;
        } else if (strstr(argv[i], "--debug=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_debug_level = atoi(s + 1);
            LOG_INFO2("debug level set to %d", harness_debug_level);
            consumed = 1;
        } else if (strstr(argv[i], "--physics-step-time=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_game_config.physics_step_time = atoi(s + 1);
            LOG_INFO2("Physics step time set to %d", harness_game_config.physics_step_time);
            consumed = 1;
        } else if (strstr(argv[i], "--fps=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_game_config.fps = atoi(s + 1);
            LOG_INFO2("FPS limiter set to %f", harness_game_config.fps);
            consumed = 1;
        } else if (strcasecmp(argv[i], "--freeze-timer") == 0) {
            LOG_INFO("Timer frozen");
            harness_game_config.freeze_timer = 1;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--no-signal-handler") == 0) {
            LOG_INFO("Don't install the signal handler");
            config->install_signalhandler = 0;
            consumed = 1;
        } else if (strstr(argv[i], "--demo-timeout=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_game_config.demo_timeout = atoi(s + 1) * 1000;
            LOG_INFO2("Demo timeout set to %d milliseconds", harness_game_config.demo_timeout);
            consumed = 1;
        } else if (strcasecmp(argv[i], "--i-am-cheating") == 0) {
            gI_am_cheating = 0xa11ee75d;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--enable-diagnostics") == 0) {
            harness_game_config.enable_diagnostics = 1;
            consumed = 1;
        } else if (strstr(argv[i], "--volume-multiplier=") != NULL) {
            char* s = strstr(argv[i], "=");
            harness_game_config.volume_multiplier = atof(s + 1);
            LOG_INFO2("Volume multiplier set to %f", harness_game_config.volume_multiplier);
            consumed = 1;
        } else if (strcasecmp(argv[i], "--full-screen") == 0) {
            // option left for backwards compatibility
            harness_game_config.start_full_screen = 1;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--window") == 0) {
            harness_game_config.start_full_screen = 0;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--gore-check") == 0) {
            harness_game_config.gore_check = 1;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--sound-options") == 0) {
            harness_game_config.sound_options = 1;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--opengl") == 0) {
            config->platform_capabilityies &= ~ePlatform_cap_video_mask;
            config->platform_capabilityies |= ePlatform_cap_opengl;
            harness_game_config.opengl_3dfx_mode = 1;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--no-music") == 0) {
            harness_game_config.no_music = 1;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--game-completed") == 0) {
            harness_game_config.game_completed = 1;
            consumed = 1;
        } else if (strcasecmp(argv[i], "--no-bind") == 0) {
            harness_game_config.no_bind = 1;
            consumed = 1;
        } else if (strstr(argv[i], "--network-adapter-name=") != NULL) {
            char* s = strstr(argv[i], "=");
            strcpy(harness_game_config.network_adapter_name, s + 1);
            consumed = 1;
        } else if (strcmp(argv[i], "--platform") == 0) {
            if (i < *argc + 1) {
                config->platform_name = argv[i + 1];
                consumed = 2;
            }
        }

        if (consumed > 0) {
            // shift args downwards
            for (j = i; j < *argc - consumed; j++) {
                argv[j] = argv[j + consumed];
            }
            *argc -= consumed;
        } else {
            i += 1;
        }
    }

    return 0;
}

// Filesystem hooks
FILE* Harness_Hook_fopen(const char* pathname, const char* mode) {
    return OS_fopen(pathname, mode);
}

// Localization
int Harness_Hook_isalnum(int c) {
    int i;
    if (harness_game_info.localization == eGameLocalization_polish) {
        // Polish diacritic letters in Windows-1250
        unsigned char letters[] = { 140, 143, 156, 159, 163, 165, 175, 179, 185, 191, 198, 202, 209, 211, 230, 234, 241, 243 };
        for (i = 0; i < (int)sizeof(letters); i++) {
            if ((unsigned char)c == letters[i]) {
                return 1;
            }
        }
    }

    return isalnum(c);
}
