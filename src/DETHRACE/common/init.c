#include "init.h"

#include <stdlib.h>
#include <time.h>

#include "car.h"
#include "controls.h"
#include "depth.h"
#include "displays.h"
#include "drfile.h"
#include "drmem.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "loading.h"
#include "loadsave.h"
#include "netgame.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "powerup.h"
#include "pratcam.h"
#include "raycast.h"
#include "replay.h"
#include "skidmark.h"
#include "sound.h"
#include "spark.h"
#include "trig.h"
#include "utility.h"
#include "world.h"

#include "brender/brender.h"

int gGame_initialized;
int gBr_initialized;
int gBrZb_initialized;
int gInitialisation_finished;
int gRender_indent;
tU32 gAustere_time;
int gInitial_rank;
int gCredits_per_rank[3];
int gInitial_credits[3];
int gNet_mode_of_last_game;
br_material* gDefault_track_material;

// IDA: void __cdecl AllocateSelf()
void AllocateSelf(void) {
    LOG_TRACE("()");

    gSelf = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (gSelf == NULL) {
        FatalError(kFatalError_AllocateSelf);
    }
    gSelf = BrActorAdd(gNon_track_actor, gSelf);
    if (gSelf == NULL) {
        FatalError(kFatalError_AllocateSelf);
    }
}

// IDA: void __cdecl AllocateCamera()
void AllocateCamera(void) {
    br_camera* camera_ptr;
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gCamera_list); i++) {
        gCamera_list[i] = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
        if (gCamera_list[i] == NULL) {
            FatalError(kFatalError_AllocateCamera);
        }

        camera_ptr = gCamera_list[i]->type_data;
        camera_ptr->type = BR_CAMERA_PERSPECTIVE_FOV;
        camera_ptr->field_of_view = BrDegreeToAngle(gCamera_angle);
        camera_ptr->hither_z = gCamera_hither;
        camera_ptr->yon_z = gCamera_yon;
        camera_ptr->aspect = (double)gWidth / (double)gHeight;
    }

    gCamera_list[0] = BrActorAdd(gSelf, gCamera_list[0]);
    if (gCamera_list[0] == NULL) {
        FatalError(kFatalError_AllocateCamera);
    }
    gCamera_list[1] = BrActorAdd(gUniverse_actor, gCamera_list[1]);
    if (gCamera_list[1] == NULL) {
        FatalError(kFatalError_AllocateCamera);
    }
    gCamera = gCamera_list[0];
    gRearview_camera = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    if (gRearview_camera == NULL) {
        FatalError(kFatalError_AllocateCamera);
    }

    gRearview_camera->t.t.mat.m[2][2] = -1.0f;
    camera_ptr = (br_camera*)gRearview_camera->type_data;
    camera_ptr->hither_z = gCamera_hither;
    camera_ptr->type = BR_CAMERA_PERSPECTIVE_FOV;
    camera_ptr->yon_z = gCamera_yon;
    camera_ptr->field_of_view = BrDegreeToAngle(gCamera_angle);
    camera_ptr->aspect = (double)gWidth / (double)gHeight;
    gRearview_camera = BrActorAdd(gSelf, gRearview_camera);
    if (gRearview_camera == NULL) {
        FatalError(kFatalError_AllocateCamera);
    }
    SetSightDistance(camera_ptr->yon_z);
}

// IDA: void __cdecl ReinitialiseForwardCamera()
void ReinitialiseForwardCamera(void) {
    br_camera* camera_ptr;
    float the_angle;
    float d;
    float w;
    LOG_TRACE("()");

    camera_ptr = (br_camera*)gCamera->type_data;
    if (gProgram_state.cockpit_on) {
        the_angle = gCamera_angle / 2.0;

        d = atan(
                tandeg(the_angle)
                * (double)gRender_screen->height
                / (double)(gProgram_state.current_car.render_bottom[0] - gProgram_state.current_car.render_top[0]))
            * 114.5915590261646;
        camera_ptr->field_of_view = BrDegreeToAngle(d);
        BrMatrix34Identity(&gCamera->t.t.mat);
        gCamera->t.t.mat.m[3][0] = gProgram_state.current_car.driver_x_offset;
        gCamera->t.t.mat.m[3][1] = gProgram_state.current_car.driver_y_offset;
        gCamera->t.t.mat.m[3][2] = gProgram_state.current_car.driver_z_offset;
        w = (float)(gRender_screen->base_y
            + (gRender_screen->height / 2)
            - (gProgram_state.current_car.render_bottom[0] + gProgram_state.current_car.render_top[0]) / 2);

        gCamera->t.t.mat.m[2][1] = tandeg(d / 2.0) * w * 2.0 / (float)gRender_screen->height;
        camera_ptr->aspect = (float)gWidth / gHeight;
        camera_ptr->yon_z = gYon_multiplier * gCamera_yon;
        if (gProgram_state.which_view == eView_left) {
            DRMatrix34PostRotateY(
                &gCamera->t.t.mat,
                BrDegreeToAngle(gProgram_state.current_car.head_left_angle));
        } else if (gProgram_state.which_view == eView_right) {
            DRMatrix34PostRotateY(
                &gCamera->t.t.mat,
                BrDegreeToAngle(gProgram_state.current_car.head_right_angle));
        }
        gCamera->t.t.mat.m[3][0] = gProgram_state.current_car.driver_x_offset;
        gCamera->t.t.mat.m[3][1] = gProgram_state.current_car.driver_y_offset;
        gCamera->t.t.mat.m[3][2] = gProgram_state.current_car.driver_z_offset;
        SetSightDistance(camera_ptr->yon_z);
        MungeForwardSky();
    }
    AssertYons();
}

// IDA: void __cdecl AllocateRearviewPixelmap()
void AllocateRearviewPixelmap(void) {
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
void ReinitialiseRearviewCamera(void) {
    br_camera* camera_ptr;
    LOG_TRACE("()");

    camera_ptr = gRearview_camera->type_data;
    camera_ptr->field_of_view = BrDegreeToAngle(gProgram_state.current_car.rearview_camera_angle);
    camera_ptr->aspect = (gProgram_state.current_car.mirror_right - gProgram_state.current_car.mirror_left) / (float)(gProgram_state.current_car.mirror_bottom - gProgram_state.current_car.mirror_top);
    gRearview_camera->t.t.translate.t.v[0] = gProgram_state.current_car.mirror_x_offset;
    gRearview_camera->t.t.translate.t.v[1] = gProgram_state.current_car.mirror_y_offset;
    gRearview_camera->t.t.translate.t.v[2] = gProgram_state.current_car.mirror_z_offset;
    AllocateRearviewPixelmap();
    MungeRearviewSky();
}

// IDA: void __cdecl ReinitialiseRenderStuff()
void ReinitialiseRenderStuff(void) {
    int x_diff;
    int y_diff;
    LOG_TRACE("()");

    if (gProgram_state.cockpit_on) {
        gProgram_state.current_render_left = gProgram_state.current_car.render_left[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_top = gProgram_state.current_car.render_top[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_right = gProgram_state.current_car.render_right[gProgram_state.cockpit_image_index];
        gProgram_state.current_render_bottom = gProgram_state.current_car.render_bottom[gProgram_state.cockpit_image_index];
    } else {
        gProgram_state.current_render_top = (gGraf_specs[gGraf_spec_index].total_height / 18 & ~1) * gRender_indent;
        gProgram_state.current_render_left = (gGraf_specs[gGraf_spec_index].total_width / 18 & ~3) * gRender_indent;
        x_diff = gGraf_specs[gGraf_spec_index].total_width - gProgram_state.current_render_left;
        y_diff = gGraf_specs[gGraf_spec_index].total_height - gProgram_state.current_render_top;
        gProgram_state.current_render_right = x_diff;
        gProgram_state.current_render_bottom = y_diff;
    }
}

// IDA: void __cdecl InstallFindFailedHooks()
void InstallFindFailedHooks(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AllocateStandardLamp()
void AllocateStandardLamp(void) {
    br_actor* lamp;
    int i;
    STUB();
}

// IDA: void __cdecl InitializeBRenderEnvironment()
void InitializeBRenderEnvironment(void) {
    br_model* arrow_model;
    LOG_TRACE("()");

    gBr_initialized = 1;
    InstallDRMemCalls();
    InstallDRFileCalls();
    SetBRenderScreenAndBuffers(0, 0, 0, 0);
    gUniverse_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (gUniverse_actor == NULL) {
        FatalError(kFatalError_AllocateRootActor);
    }
    gUniverse_actor->identifier = BrResStrDup(gUniverse_actor, "Root");
    BrEnvironmentSet(gUniverse_actor);
    gNon_track_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    if (gNon_track_actor == NULL) {
        FatalError(kFatalError_AllocateRootActor);
    }
    BrActorAdd(gUniverse_actor, gNon_track_actor);
    gDont_render_actor = BrActorAllocate(BR_ACTOR_NONE, 0);
    if (gDont_render_actor == NULL) {
        FatalError(kFatalError_AllocateRootActor);
    }
    gDont_render_actor->render_style = BR_RSTYLE_NONE;
    BrActorAdd(gUniverse_actor, gDont_render_actor);
    AllocateSelf();
    AllocateCamera();
    arrow_model = LoadModel("CPOINT.DAT");
    BrModelAdd(arrow_model);
    gArrow_actor = LoadActor("CPOINT.ACT");
    gArrow_actor->model = arrow_model;
}

// IDA: void __cdecl InitBRFonts()
void InitBRFonts(void) {
    LOG_TRACE("()");
    gBig_font = LoadBRFont("BIGFONT.FNT");
    gFont_7 = LoadBRFont("FONT7.FNT");
    gHeadup_font = LoadBRFont("HEADUP.FNT");
}

// IDA: void __cdecl AustereWarning()
void AustereWarning(void) {
    LOG_TRACE("()");

    ClearEntireScreen();

    if (gAusterity_mode) {
        LoadFont(FONT_GRNLIT);
        TransDRPixelmapText(gBack_screen,
            (gBack_screen->width - DRTextWidth(&gFonts[FONT_GRNLIT], GetMiscString(kMiscString_LOW_MEMORY_MODE_INVOKED))) / 2,
            0,
            &gFonts[FONT_GRNLIT],
            GetMiscString(kMiscString_LOW_MEMORY_MODE_INVOKED),
            gBack_screen->width);
        PDScreenBufferSwap(0);
        gAustere_time = PDGetTotalTime();
        DisposeFont(FONT_GRNLIT);
    } else {
        gAustere_time = gAusterity_mode;
    }
}

// IDA: void __cdecl InitLineStuff()
void InitLineStuff(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitSmokeStuff()
void InitSmokeStuff(void) {
    static br_token_value fadealpha[3];
    tPath_name path;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl Init2DStuff()
void Init2DStuff(void) {
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

    if (harness_game_config.gore_check) {
        gProgram_state.sausage_eater_mode = gSausage_override ? 1 : (PDGetGorePassword() ? 0 : 1);
        PDDisplayGoreworthiness(!gProgram_state.sausage_eater_mode);
    } else {
        gProgram_state.sausage_eater_mode = gSausage_override;
    }

    MAMSInitMem();
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
        FatalError(kFatalError_UnsupportedScreenDepth);
    }
    CalcGrafDataIndex();
    PDInitScreen();
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
    InitPaletteAnimate();
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
    // dword_112DF8 = 1;  // never checked by game
}

// IDA: void __usercall InitGame(int pStart_race@<EAX>)
void InitGame(int pStart_race) {
    int i;
    LOG_TRACE("(%d)", pStart_race);

    gWaiting_for_unpause = 1;
    gWait_for_it = 1;
    if (gNet_mode != eNet_mode_none) {
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
    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        gProgram_state.current_car.power_up_levels[0] = gDemo_armour;
        gProgram_state.current_car.power_up_levels[1] = gDemo_power;
        gProgram_state.current_car.power_up_levels[2] = gDemo_offensive;
    } else {
        for (i = 0; i < COUNT_OF(gProgram_state.current_car.power_up_levels); i++) {
            gProgram_state.current_car.power_up_levels[i] = 0;
        }
    }
    for (i = 0; i < gNumber_of_races; i++) {
        gRace_list[i].been_there_done_that = 0;
    }
    for (i = 0; i < gNumber_of_racers; i++) {
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
    SwitchToLoresMode();
}

// IDA: void __cdecl DisposeGameIfNecessary()
void DisposeGameIfNecessary(void) {
    int i;
    LOG_TRACE("()");

    if (gNet_mode != eNet_mode_none) {
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
void LoadInTrack(void) {
    LOG_TRACE("()");

    LoadTrack(gProgram_state.track_file_name, &gProgram_state.track_spec, &gCurrent_race);
}

// IDA: void __cdecl DisposeTrack()
void DisposeTrack(void) {
    LOG_TRACE("()");

    FreeTrack(&gProgram_state.track_spec);
}

// IDA: void __usercall CopyMaterialColourFromIndex(br_material *pMaterial@<EAX>)
void CopyMaterialColourFromIndex(br_material* pMaterial) {
    LOG_TRACE("(%p)", pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitRace()
void InitRace(void) {
    LOG_TRACE("()");

    SwitchToRealResolution();
    // TODO: dword_5454C4 = 0;
    ClearConcussion();
    ClearWobbles();
    ClearHeadups();
    ResetOilSpills();
    HideSkids();
    PossibleService();
    BuildColourTable(gRender_palette);
    PossibleService();
    // TODO: dword_55142C = 0;
    gStart_race_sent = 0;
    gProgram_state.frame_rate_headup = NewTextHeadupSlot(0, 0, 0, -1, "");
    if (TranslationMode()) {
        if (gAusterity_mode) {
            FlushInterfaceFonts();
        }
    } else {
        LoadFont(1);
        LoadFont(2);
        LoadFont(3);
    }
    LoadFont(4);
    LoadFont(5);
    PossibleService();
    LoadFont(6);
    LoadFont(7);
    LoadFont(8);
    PossibleService();
    ResetRecoveryVouchers();
    gMap_mode = 0;
    gProgram_state.cockpit_image_index = 0;
    if (gNet_mode != eNet_mode_none) {
        gNet_cash_headup = NewTextHeadupSlot(13, 0, 0, -6, "");
        gNet_ped_headup = NewTextHeadupSlot(14, 0, 0, -6, "");
    } else {
        gCredits_won_headup = NewTextHeadupSlot(1, 0, 0, -6, "");
        gPed_kill_count_headup = NewTextHeadupSlot(2, 0, 0, -6, "");
        gCar_kill_count_headup = NewTextHeadupSlot(12, 0, 0, -6, "");
        gTimer_headup = NewTextHeadupSlot(7, 0, 0, -5, "");
        gTime_awarded_headup = NewTextHeadupSlot(11, 0, 0, -2, "");
        gLaps_headup = NewTextHeadupSlot(8, 0, 0, -6, "");
    }
    PossibleService();
    gProgram_state.which_view = eView_forward;
    gProgram_state.new_view = eView_undefined;
    gProgram_state.pratcam_move_start = 0;
    gAction_replay_mode = 0;
    SwitchToRealResolution();
    InitPratcam();
    SwitchToLoresMode();
    gProgram_state.credits_earned = 0;
    gProgram_state.credits_lost = 0;
    gProgram_state.peds_killed = 0;
    gProgram_state.revs = 2000;
    gProgram_state.current_car.speed = 0.0;
    gProgram_state.current_car.steering_angle = 0.0;
    gProgram_state.current_car.lf_sus_position = 0.0;
    gProgram_state.current_car.rf_sus_position = 0.0;
    gProgram_state.current_car.lr_sus_position = 0.0;
    gProgram_state.current_car.rr_sus_position = 0.0;
    PossibleService();
    gAuto_repair = 0;
    SetIntegerMapRenders();
    AdjustRenderScreenSize();
    PrintMemoryDump(0, "DIRECTLY BEFORE LOADING IN TRACK");
    LoadInTrack();
    if (gYon_multiplier != 1.0) {
        AdjustRenderScreenSize();
    }
    PrintMemoryDump(0, "DIRECTLY AFTER LOADING IN TRACK");
    LoadCopCars();
    PrintMemoryDump(0, "AFTER LOADING IN COPS");
    SaveShadeTables();
    gCountdown = 7;
    gTimer = 1000 * gCurrent_race.initial_timer[gProgram_state.skill_level];
    gLap = 1;
    gTotal_laps = gCurrent_race.total_laps;
    gCheckpoint = 1;
    gCheckpoint_count = gCurrent_race.check_point_count;
    gFreeze_timer = 0;
    gFree_repairs = 0;
    if (gNet_mode) {
        gShow_opponents = gCurrent_net_game->options.show_players_on_map;
    } else {
        gShow_opponents = 1;
    }
    gOn_drugs = 0;
    gRace_finished = 0;
    gOpponent_speed_factor = 1.0f;
    gCop_speed_factor = 1.0f;
    gGravity_multiplier = gDefault_gravity;
    gPinball_factor = 0;
    gInstant_handbrake = 0;
    if (gNet_mode) {
        gShow_peds_on_map = gCurrent_net_game->options.show_peds_on_map;
    } else {
        gShow_peds_on_map = 0;
    }
    PossibleService();
    SetCarSuspGiveAndHeight(&gProgram_state.current_car, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
    InitPowerups();
    PossibleService();
    ResetSparks();
    PossibleService();
    MakeSavedGame(&gPre_race_saved_game);
    PossibleService();
    InitPlayers();
    if (gNet_mode) {
        gProgram_state.credits_earned = gInitial_net_credits[gCurrent_net_game->options.starting_money_index];
        InitNetGameplayStuff();
    }
    gInitialised_grid = 0;
    SwitchToLoresMode();
}

// IDA: void __cdecl DisposeRace()
void DisposeRace(void) {
    LOG_TRACE("()");

    PossibleService();
    DisposePiping();
    PossibleService();
    CloseDownPowerUps();
    PossibleService();
    if (!TranslationMode()) {
        DisposeFont(1);
        DisposeFont(2);
        DisposeFont(3);
    }
    PossibleService();
    DisposeFont(6);
    DisposeFont(7);
    DisposeFont(8);
    DisposeFont(4);
    DisposeFont(5);
    PossibleService();
    DisposeSavedShadeTables();
    PossibleService();
    DisposeSoundSources();
    PossibleService();
    DisposeOpponents();
    PossibleService();
    DisposePedestrians();
    PossibleService();
    DisposePratcam();
    PossibleService();
}

// IDA: int __cdecl GetScreenSize()
int GetScreenSize(void) {
    LOG_TRACE("()");

    return gRender_indent;
}

// IDA: void __usercall SetScreenSize(int pNew_size@<EAX>)
void SetScreenSize(int pNew_size) {
    LOG_TRACE("(%d)", pNew_size);

    gRender_indent = pNew_size;
}
