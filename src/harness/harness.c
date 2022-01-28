
#include "harness.h"
#include "brender_emu/renderer_impl.h"
#include "include/harness/config.h"
#include "platforms/null.h"
#include "platforms/sdl_gl.h"
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

// value between 0 and 1 - 0 is no fade, 1 is full black
float palette_fade_to_black_alpha = 0;

int rendered_scenes_this_frame = 0;
int rendered_scenes_last_frame = 0;

unsigned int last_frame_time = 0;

// Provided by game
extern void BrPixelmapFill(br_pixelmap* dst, br_uint_32 colour);
extern unsigned int GetTotalTime();
extern uint8_t gScan_code[123][2];

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

    Harness_ProcessCommandLine(argc, argv);

    install_signal_handler(argv[0]);
    platform->Init();

    int* keymap = platform->GetKeyMap();
    if (keymap != NULL) {
        for (int i = 0; i < 123; i++) {
            gScan_code[i][0] = keymap[i];
            //gScan_code[i][1] = keymap[i];
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

void Harness_Debug_PrintStack() {
#ifndef _WIN32
    posix_print_stack_trace();
#endif
}

int Harness_ProcessCommandLine(int* argc, char* argv[]) {
    char* platform_name = NULL;

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
        } else if (strstr(argv[i], "--platform=") != NULL) {
            platform_name = strstr(argv[i], "=") + 1;
            LOG_INFO("Platform set to: %s", platform_name);
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
        }

        if (handled) {
            // shift args downwards
            for (int j = i; j < *argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            (*argc)--;
        }
    }

    if (platform_name == NULL) {
        platform_name = "sdl_gl";
    }

    if (strcmp(platform_name, "sdl_gl") == 0) {
        platform = &sdl_gl_platform;
    } else if (strcmp(platform_name, "null") == 0) {
        platform = &null_platform;
    } else {
        LOG_PANIC("Invalid platform: %s", platform_name);
    }
    return 0;
}

void Harness_Hook_DOSGfxBegin() {
    platform->NewWindow("Dethrace", 640, 400);
}

void Harness_ConvertPalettedPixelmapTo32Bit(uint32_t** dst, br_pixelmap* src, int vflip) {
    uint8_t palette_index = 0;
    uint8_t* data = src->pixels;
    uint32_t* colors;
    int x;
    int y;
    int dest_y;

    if (!palette) {
        return;
    }
    colors = palette->pixels;
    if (*dst == NULL) {
        *dst = malloc(src->width * src->height * sizeof(uint32_t));
    }

    // generate 32 bit texture from src + palette
    if (vflip) {
        dest_y = src->height - 1;
        for (y = 0; y < src->height; y++) {
            for (x = 0; x < src->width; x++) {
                palette_index = (data[y * src->row_bytes + x]);
                (*dst)[dest_y * src->width + x] = colors[palette_index];
            }
            dest_y--;
        }
    } else {
        for (y = 0; y < src->height; y++) {
            for (x = 0; x < src->width; x++) {
                palette_index = (data[y * src->row_bytes + x]);
                (*dst)[y * src->width + x] = colors[palette_index];
            }
        }
    }
}

// Render 2d back buffer
void Harness_RenderScreen(br_pixelmap* dst, br_pixelmap* src) {
    Harness_ConvertPalettedPixelmapTo32Bit(&screen_buffer, src, 1);
    platform->RenderFullScreenQuad(screen_buffer, 320, 200);

    last_dst = dst;
    last_src = src;
}

void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm) {
    palette = pm;
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

// Begin 3d scene
void Harness_Hook_BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    rendered_scenes_this_frame++;

    // if this is the first 3d scene this frame, draw the current colour_buffer (2d screen) contents to framebuffer
    if (rendered_scenes_this_frame == 0) {
        Harness_RenderScreen(NULL, colour_buffer);
    }
    // clear to transparent ready for the game to render foreground elements which we will capture later in `Harness_Hook_BrPixelmapDoubleBuffer`
    BrPixelmapFill(colour_buffer, 0);

    platform->BeginScene(camera, colour_buffer);
}

void Harness_Hook_BrZbSceneRenderAdd(br_actor* tree) {
}

void Harness_Hook_renderFaces(br_model* model, br_material* material, br_token type) {
    platform->RenderModel(model, renderer_state->state.matrix.model_to_view);
}

void Harness_Hook_BrZbSceneRenderEnd() {
    platform->EndScene();
}

void Harness_Hook_MainGameLoop() {
    if (harness_game_config.fps == 0) {
        return;
    }

    if (last_frame_time) {
        unsigned int frame_time = GetTotalTime() - last_frame_time;

        if (frame_time < 100) {

            int sleep_time = (1000 / harness_game_config.fps) - frame_time;
            if (sleep_time > 5) {
                SDL_Delay(sleep_time);
            }
        }
    }

    last_frame_time = GetTotalTime();
}

// Called by game to swap buffers at end of frame rendering
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {

    // should we switch to fb 0 here ?

    // draw the current colour_buffer (2d screen) contents
    Harness_RenderScreen(dst, src);

    // draw the current 3d frame buffer
    if (rendered_scenes_this_frame) {
        platform->RenderFrameBuffer();
    }

    // if the game has faded the palette, we should respect that.
    // fixes screen flash during race start or recovery
    if (palette_fade_to_black_alpha != 0) {
        platform->RenderColorBlend(0, 0, 0, palette_fade_to_black_alpha);
    }

    platform->Swap();
    platform->PollEvents();

    // reset 3d render count
    rendered_scenes_last_frame = rendered_scenes_this_frame;
    rendered_scenes_this_frame = 0;
}

int Harness_Hook_KeyDown(unsigned char pScan_code) {
    return platform->IsKeyDown(pScan_code);
}

void Harness_Hook_PDServiceSystem() {
    platform->PollEvents();
}
void Harness_Hook_PDSetKeyArray() {
    platform->PollEvents();
}

void Harness_Hook_BrMaterialUpdate(br_material* mat, br_uint_16 flags) {
    //LOG_DEBUG("buffermat %s", mat->identifier);
    platform->BufferMaterial(mat);
}

void Harness_Hook_BrBufferUpdate(br_pixelmap* pm, br_token use, br_uint_16 flags) {
    if (use == BRT_COLOUR_MAP_O) {
        platform->BufferTexture(pm);
    }
}

void Harness_Hook_S3Service(int unk1, int unk2) {
    Sound_Service();
}

void Harness_Hook_S3StopAllOutletSounds() {
}

void Harness_Hook_SetFadedPalette(int pDegree) {

    // pDegree is 0-255 where 0 is full black and 255 is full original color.
    // we convert this to an alpha blend value
    palette_fade_to_black_alpha = (256 - pDegree) / 256.f;

    if (rendered_scenes_last_frame > 0) {
        platform->RenderFrameBuffer();
    }
    Harness_RenderScreen(last_dst, last_src);
    platform->RenderColorBlend(0, 0, 0, palette_fade_to_black_alpha);
    platform->Swap();
}