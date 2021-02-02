#include "init.h"

#include <stdlib.h>
#include <time.h>

#include "common/controls.h"
#include "common/depth.h"
#include "common/displays.h"
#include "common/drfile.h"
#include "common/drmem.h"
#include "common/errors.h"
#include "common/flicplay.h"
#include "common/globvars.h"
#include "common/globvrkm.h"
#include "common/globvrpb.h"
#include "common/grafdata.h"
#include "common/graphics.h"
#include "common/loading.h"
#include "common/netgame.h"
#include "common/network.h"
#include "common/oil.h"
#include "common/pedestrn.h"
#include "common/powerup.h"
#include "common/raycast.h"
#include "common/replay.h"
#include "common/skidmark.h"
#include "common/sound.h"
#include "common/utility.h"
#include "common/world.h"
#include "pd/sys.h"

#include "brender.h"

int gInitialisation_finished = 0;
tU32 gAustere_time;
int gInitial_rank;
int gGame_initialized = 0;
int gBr_initialized = 0;
int gBrZb_initialized = 0;
int gRender_indent = 0;
int gCredits_per_rank[3];
int gInitial_credits[3];
int gNet_mode_of_last_game;
br_material* gDefault_track_material;

// IDA: void __cdecl AllocateSelf()
void AllocateSelf() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AllocateCamera()
void AllocateCamera() {
    br_camera* camera_ptr;
    int i;
    LOG_TRACE("()");

    for (i = 0; i < 2; i++) {
        gCamera_list[i] = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
        if (!gCamera_list[i]) {
            FatalError(5);
        }

        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->type = 1;
        camera_ptr->field_of_view = BR_ANGLE_DEG(gCamera_angle);
        camera_ptr->hither_z = gCamera_hither;
        camera_ptr->yon_z = gCamera_yon;
        camera_ptr->aspect = (double)gWidth / (double)gHeight;
    }

    gCamera_list[0] = BrActorAdd(gSelf, gCamera_list[0]);
    if (!gCamera_list[0]) {
        FatalError(5);
    }
    gCamera_list[1] = BrActorAdd(gUniverse_actor, gCamera_list[1]);
    if (!gCamera_list[1]) {
        FatalError(5);
    }
    gCamera = gCamera_list[0];
    gRearview_camera = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    if (!gRearview_camera) {
        FatalError(5);
    }

    gRearview_camera->t.t.mat.m[2][2] = -1.0f;
    camera_ptr = (br_camera*)gRearview_camera->type_data;
    camera_ptr->hither_z = gCamera_hither;
    camera_ptr->type = 1;
    camera_ptr->yon_z = gCamera_yon;
    camera_ptr->field_of_view = BR_ANGLE_DEG(gCamera_angle);
    camera_ptr->aspect = (double)gWidth / (double)gHeight;
    gRearview_camera = BrActorAdd(gSelf, gRearview_camera);
    if (!gRearview_camera) {
        FatalError(5);
    }
    SetSightDistance(camera_ptr->yon_z);
}

// IDA: void __cdecl ReinitialiseForwardCamera()
void ReinitialiseForwardCamera() {
    br_camera* camera_ptr;
    float the_angle;
    float d;
    float w;
    LOG_TRACE("()");

    STUB();
}

// IDA: void __cdecl AllocateRearviewPixelmap()
void AllocateRearviewPixelmap() {
    char* rear_screen_pixels;
    LOG_TRACE("()");

    if (gRearview_screen) {
        BrMemFree(gRearview_screen->pixels);
        BrPixelmapFree(gRearview_screen);
        BrPixelmapFree(gRearview_depth_buffer);
        gRearview_screen = NULL;
    }
    if (gProgram_state.mirror_on) {
        rear_screen_pixels = BrMemAllocate(
            (gProgram_state.current_car.mirror_bottom - gProgram_state.current_car.mirror_top + 1)
                * (gProgram_state.current_car.mirror_right - gProgram_state.current_car.mirror_left + 4)
                * gGraf_specs[gGraf_spec_index].depth_bytes,
            kMem_rear_screen_pixels);
        if (gScreen->row_bytes < 0) {
            BrFatal("..\\..\\source\\common\\init.c", 260, "Bruce bug at line %d, file ..\\..\\source\\common\\init.c", 4);
        }
        gRearview_screen = DRPixelmapAllocate(
            gScreen->type,
            gProgram_state.current_car.mirror_right - gProgram_state.current_car.mirror_left,
            gProgram_state.current_car.mirror_bottom - gProgram_state.current_car.mirror_top,
            rear_screen_pixels,
            0);

        gRearview_screen->origin_x = gRearview_screen->width / 2;
        gRearview_screen->origin_y = gRearview_screen->height / 2;
        gRearview_depth_buffer = BrPixelmapMatch(gRearview_screen, BR_PMMATCH_DEPTH_16);
    }
}

// IDA: void __cdecl ReinitialiseRearviewCamera()
void ReinitialiseRearviewCamera() {
    br_camera* camera_ptr;
    LOG_TRACE("()");
    STUB();
}

// IDA: void __cdecl ReinitialiseRenderStuff()
void ReinitialiseRenderStuff() {
    int x_diff;
    int y_diff;
    LOG_TRACE("()");

    if (gProgram_state.cockpit_on) {
        gProgram_state.current_render_left = gProgram_state.current_car.render_left[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_top = gProgram_state.current_car.render_top[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_right = gProgram_state.current_car.render_right[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_bottom = gProgram_state.current_car.render_bottom[gProgram_state.cockpit_image_index];
    } else {
        y_diff = (gGraf_specs[gGraf_spec_index].total_height / 18 & 0xFFFFFFFE) * gRender_indent;
        x_diff = (gGraf_specs[gGraf_spec_index].total_width / 18 & 0xFFFFFFFC) * gRender_indent;
        gProgram_state.current_render_left = x_diff;
        gProgram_state.current_render_top = y_diff;
        gProgram_state.current_render_right = gGraf_specs[gGraf_spec_index].total_width - x_diff;
        gProgram_state.current_render_bottom = gGraf_specs[gGraf_spec_index].total_height - y_diff;
    }
}

// IDA: void __cdecl InstallFindFailedHooks()
void InstallFindFailedHooks() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AllocateStandardLamp()
void AllocateStandardLamp() {
    br_actor* lamp;
    int i;
    STUB();
}

// IDA: void __cdecl InitializeBRenderEnvironment()
void InitializeBRenderEnvironment() {
    br_model* arrow_model;
    LOG_TRACE("()");

    gBr_initialized = 1;
    InstallDRMemCalls();
    InstallDRFileCalls();
    SetBRenderScreenAndBuffers(0, 0, 0, 0);
    gUniverse_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (!gUniverse_actor) {
        FatalError(3);
    }
    gUniverse_actor->identifier = BrResStrDup(gUniverse_actor, "Root");
    BrEnvironmentSet(gUniverse_actor);
    gNon_track_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (!gNon_track_actor) {
        FatalError(3);
    }
    BrActorAdd(gUniverse_actor, gNon_track_actor);
    gDont_render_actor = BrActorAllocate(BR_ACTOR_NONE, 0);
    if (!gDont_render_actor) {
        FatalError(3);
    }
    gDont_render_actor->render_style = BR_RSTYLE_NONE;
    BrActorAdd(gUniverse_actor, gDont_render_actor);
    gSelf = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (!gSelf) {
        FatalError(6);
    }
    gSelf = BrActorAdd(gNon_track_actor, gSelf);
    if (!gSelf) {
        FatalError(6);
    }
    AllocateCamera();
    arrow_model = LoadModel("CPOINT.DAT");
    BrModelAdd(arrow_model);
    gArrow_actor = LoadActor("CPOINT.ACT");
    gArrow_actor->model = arrow_model;
}

// IDA: void __cdecl InitBRFonts()
void InitBRFonts() {
    LOG_TRACE("()");
    gBig_font = LoadBRFont("BIGFONT.FNT");
    gFont_7 = LoadBRFont("FONT7.FNT");
    gHeadup_font = LoadBRFont("HEADUP.FNT");
}

// IDA: void __cdecl AustereWarning()
void AustereWarning() {
    LOG_TRACE("()");

    ClearEntireScreen();

    if (gAusterity_mode) {
        LoadFont(FONT_GRNLIT);
        TransDRPixelmapText(gBack_screen,
            (gBack_screen->width - DRTextWidth(&gFonts[FONT_GRNLIT], GetMiscString(193))) / 2,
            0,
            &gFonts[FONT_GRNLIT],
            GetMiscString(193),
            gBack_screen->width);
        PDScreenBufferSwap(0);
        gAustere_time = PDGetTotalTime();
        DisposeFont(FONT_GRNLIT);
    } else {
        gAustere_time = gAusterity_mode;
    }
}

// IDA: void __cdecl InitLineStuff()
void InitLineStuff() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitSmokeStuff()
void InitSmokeStuff() {
    static br_token_value fadealpha[3];
    tPath_name path;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl Init2DStuff()
void Init2DStuff() {
    br_camera* camera;
    static br_token_value fadealpha[3];
    tPath_name path;
    br_scalar prat_u;
    br_scalar prat_v;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall InitialiseApplication(int pArgc@<EAX>, char **pArgv@<EDX>)
void InitialiseApplication(int pArgc, char** pArgv) {

    gProgram_state.sausage_eater_mode = gSausage_override;
    PrintMemoryDump(gSausage_override, *pArgv);
    if (gAustere_override || PDDoWeLeadAnAustereExistance() != 0) {
        gAusterity_mode = 1;
    }

    srand(time(NULL));
    BrV1dbBeginWrapper_Float();
    CreateStainlessClasses();
    InitWobbleStuff();
    LoadGeneralParameters();
    DefaultNetName();
    strcpy(gProgram_state.player_name[0], "MAX DAMAGE");
    strcpy(gProgram_state.player_name[1], "DIE ANNA");
    RestoreOptions();
    LoadKeyMapping();
    if (!PDInitScreenVars(pArgc, pArgv)) {
        FatalError(0);
    }
    CalcGrafDataIndex();
    InitializeBRenderEnvironment();
    InitDRFonts();
    InitBRFonts();
    LoadMiscStrings();
    LoadInRegistees();
    FinishLoadingGeneral();
    InitializePalettes();
    AustereWarning();
    LoadInterfaceStrings();
    InitializeActionReplay();
    FlicPaletteAllocate();
    InitInterfaceLoadState();
    InitTransientBitmaps();
    InitSound();
    InitHeadups();
    gDefault_track_material = BrMaterialAllocate("gDefault_track_material");
    gDefault_track_material->index_base = 227;
    gDefault_track_material->index_range = 1;
    BrMaterialAdd(gDefault_track_material);
    InitShadow();
    InitFlics();
    AllocateStandardLamp();
    InitAmbience();
    LoadOpponents();
    LoadPowerups();
    LoadRaces(gRace_list, &gNumber_of_races, -1);
    RevertPalette();
    InitRayCasting();
    InitDepthEffects();
    InitialiseStorageSpace(&gOur_car_storage_space, 40, 2, 40, 30);
    InitialiseStorageSpace(&gTheir_cars_storage_space, 300, 50, 500, 200);
    InitialiseStorageSpace(&gPedestrians_storage_space, 500, 10, 0, 0);
    InitialiseStorageSpace(&gTrack_storage_space, 400, 50, 400, 1000);
    InitOilSpills();
    if (gAustere_time) {
        while (PDGetTotalTime() - gAustere_time < 2000) {
        }
    }
    ClearEntireScreen();
    InitSkids();
    InitPeds();
    gProgram_state.track_spec.the_actor = NULL;
    gCD_is_in_drive = TestForOriginalCarmaCDinDrive();
    SwitchToLoresMode();
    PrintMemoryDump(0, "AFTER APPLICATION INITIALISATION");
}

// IDA: void __usercall InitialiseDeathRace(int pArgc@<EAX>, char **pArgv@<EDX>)
void InitialiseDeathRace(int pArgc, char** pArgv) {
    PDInitialiseSystem();

    InitialiseApplication(pArgc, pArgv);
    //dword_112DF8 = 1;  // never checked by game
}

// IDA: void __usercall InitGame(int pStart_race@<EAX>)
void InitGame(int pStart_race) {
    int i;
    LOG_TRACE("(%d)", pStart_race);

    gWaiting_for_unpause = 1;
    gWait_for_it = 1;
    if (gNet_mode) {
        gCredit_period = gCredit_period_network[gCurrent_net_game->type];
    } else {
        gCredit_period = gCredit_period_single[gProgram_state.skill_level];
    }
    SwitchToRealResolution();
    gGame_to_load = -1;
    gCurrent_race.number_of_racers = 0;
    LoadHeadupImages();
    gGame_initialized = 1;
    gNet_mode_of_last_game = gNet_mode;
    gNo_races_yet = 1;
    NetPlayerStatusChanged(ePlayer_status_loading);
    gProgram_state.current_race_index = pStart_race;
    for (i = 0; i < COUNT_OF(gProgram_state.current_car.power_up_levels); i++) {
        gProgram_state.current_car.power_up_levels[i] = 0;
    }
    for (i = 0; gNumber_of_races > i; ++i) {
        gRace_list[i].been_there_done_that = 0;
    }
    for (i = 0; gNumber_of_racers > i; ++i) {
        gOpponents[i].dead = 0;
    }
    gProgram_state.rank = gInitial_rank;
    gProgram_state.credits = gInitial_credits[gProgram_state.skill_level];
    gProgram_state.credits_per_rank = gCredits_per_rank[gProgram_state.skill_level];
    gProgram_state.number_of_cars = 1;
    gProgram_state.cars_available[0] = gProgram_state.frank_or_anniness;
    gProgram_state.game_completed = 0;
    gProgram_state.redo_race_index = -1;
    gWait_for_it = 0;
    return SwitchToLoresMode();
}

// IDA: void __cdecl DisposeGameIfNecessary()
void DisposeGameIfNecessary() {
    int i;
    LOG_TRACE("()");

    if (gNet_mode) {
        NetLeaveGame(gCurrent_net_game);
    }
    if (gGame_initialized) {
        DisposeHeadupImages();
        gGame_initialized = 0;
        if (gNet_mode_of_last_game) {
            for (i = 0; i < gNumber_of_net_players; i++) {
                DisposeCar(gNet_players[i].car, gNet_players[i].car_index);
            }
            PossibleService();
            LoadRaces(gRace_list, &gNumber_of_races, -1);
            DisposeStorageSpace(&gNet_cars_storage_space);
        }
    }
    ShutdownNetIfRequired();
}

// IDA: void __cdecl LoadInTrack()
void LoadInTrack() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeTrack()
void DisposeTrack() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyMaterialColourFromIndex(br_material *pMaterial@<EAX>)
void CopyMaterialColourFromIndex(br_material* pMaterial) {
    LOG_TRACE("(%p)", pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitRace()
void InitRace() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeRace()
void DisposeRace() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetScreenSize()
int GetScreenSize() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetScreenSize(int pNew_size@<EAX>)
void SetScreenSize(int pNew_size) {
    LOG_TRACE("(%d)", pNew_size);
    gRender_indent = pNew_size;
}
