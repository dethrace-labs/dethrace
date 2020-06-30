#include "init.h"

#include <stdlib.h>
#include <time.h>

#include "common/depth.h"
#include "common/displays.h"
#include "common/drdebug.h"
#include "common/drfile.h"
#include "common/drmem.h"
#include "common/errors.h"
#include "common/flicplay.h"
#include "common/globvars.h"
#include "common/globvrkm.h"
#include "common/grafdata.h"
#include "common/graphics.h"
#include "common/loading.h"
#include "common/netgame.h"
#include "common/oil.h"
#include "common/pedestrn.h"
#include "common/powerup.h"
#include "common/raycast.h"
#include "common/replay.h"
#include "common/skidmark.h"
#include "common/sound.h"
#include "common/world.h"
#include "pc-dos/dossys.h"

#include "brender.h"

int gInitialisation_finished;
tU32 gAustere_time;
int gInitial_rank;
int gGame_initialized;
int gBr_initialized;
int gBrZb_initialized;
int gRender_indent;
int gCredits_per_rank[3];
int gInitial_credits[3];
int gNet_mode_of_last_game;
br_material* gDefault_track_material;

// Offset: 0
// Size: 115
void AllocateSelf() {
    NOT_IMPLEMENTED();
}

// Offset: 116
// Size: 514
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

// Offset: 632
// Size: 582
void ReinitialiseForwardCamera() {
    float the_angle;
    float d;
    float w;
    NOT_IMPLEMENTED();
}

// Offset: 1216
// Size: 217
void AllocateRearviewPixelmap() {
    NOT_IMPLEMENTED();
}

// Offset: 1436
// Size: 169
void ReinitialiseRearviewCamera() {
    NOT_IMPLEMENTED();
}

// Offset: 1608
// Size: 347
void ReinitialiseRenderStuff() {
    int y_diff;
    NOT_IMPLEMENTED();
}

// Offset: 1956
// Size: 34
void InstallFindFailedHooks() {
    NOT_IMPLEMENTED();
}

// Offset: 1992
// Size: 158
void AllocateStandardLamp() {
    br_actor* lamp;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 2152
// Size: 342
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

// Offset: 2496
// Size: 79
void InitBRFonts() {
    LOG_TRACE("()");
    gBig_font = LoadBRFont("BIGFONT.FNT");
    gFont_7 = LoadBRFont("FONT7.FNT");
    gHeadup_font = LoadBRFont("HEADUP.FNT");
}

// Offset: 2576
// Size: 196
void AustereWarning() {
    NOT_IMPLEMENTED();
}

// Offset: 2772
// Size: 309
void InitLineStuff() {
    NOT_IMPLEMENTED();
}

// Offset: 3084
// Size: 896
void InitSmokeStuff() {
    tPath_name path;
    NOT_IMPLEMENTED();
}

// Offset: 3980
// Size: 1350
void Init2DStuff() {
    static br_token_value fadealpha[3];
    tPath_name path;
    br_scalar prat_u;
    br_scalar prat_v;
    NOT_IMPLEMENTED();
}

// Offset: 5332
// Size: 671
// EAX: pArgc
// EDX: pArgv
void InitialiseApplication(int pArgc, char** pArgv) {

    gProgram_state.sausage_eater_mode = gSausage_override;
    DrDebugLog(gSausage_override, *pArgv);
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
    DrDebugLog(0, "AFTER APPLICATION INITIALISATION");
}

// Offset: 6004
// Size: 64
// EAX: pArgc
// EDX: pArgv
void InitialiseDeathRace(int pArgc, char** pArgv) {
    PDInitialiseSystem();

    InitialiseApplication(pArgc, pArgv);
    //dword_112DF8 = 1;  // never checked by game
}

// Offset: 6068
// Size: 427
// EAX: pStart_race
void InitGame(int pStart_race) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 6496
// Size: 191
void DisposeGameIfNecessary() {
    NOT_IMPLEMENTED();
}

// Offset: 6688
// Size: 54
void LoadInTrack() {
    NOT_IMPLEMENTED();
}

// Offset: 6744
// Size: 44
void DisposeTrack() {
    NOT_IMPLEMENTED();
}

// Offset: 6788
// Size: 97
// EAX: pMaterial
void CopyMaterialColourFromIndex(br_material* pMaterial) {
    NOT_IMPLEMENTED();
}

// Offset: 6888
// Size: 1230
void InitRace() {
    NOT_IMPLEMENTED();
}

// Offset: 8120
// Size: 208
void DisposeRace() {
    NOT_IMPLEMENTED();
}

// Offset: 8328
// Size: 45
int GetScreenSize() {
    NOT_IMPLEMENTED();
}

// Offset: 8376
// Size: 45
// EAX: pNew_size
void SetScreenSize(int pNew_size) {
    LOG_TRACE("(%d)", pNew_size);
    gRender_indent = pNew_size;
}
