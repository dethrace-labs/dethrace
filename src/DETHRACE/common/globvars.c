#include "globvars.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x5212f0
int gTyping;
// GLOBAL: CARM95 0x5212f4
int gArrow_mode;
// GLOBAL: CARM95 0x5212f8
int gInfo_on;
// GLOBAL: CARM95 0x5212fc
int gHost_abandon_game;
// GLOBAL: CARM95 0x521300
int gRecover_car;
// GLOBAL: CARM95 0x521304
int gDisallow_abandon_race;
int gPipe_reg_snap_before_new_game;
int gPipe_reg_snap_before_our_car;
int gPipe_reg_snap_after_our_car;
// GLOBAL: CARM95 0x521314
int gAction_replay_mode;
// GLOBAL: CARM95 0x521318
int gDefault_engine_noise_index;
// GLOBAL: CARM95 0x52131c
int gAusterity_mode;
// GLOBAL: CARM95 0x521320
int gSausage_override;
// GLOBAL: CARM95 0x521324
int gAustere_override;
// GLOBAL: CARM95 0x521328
int gSound_override;
// GLOBAL: CARM95 0x52132c
int gKey_map_index;
// GLOBAL: CARM95 0x521330
int gInterface_within_race_mode;
// GLOBAL: CARM95 0x521334
int gCut_scene_override;
// GLOBAL: CARM95 0x521338
int gGame_to_load = -1;
// GLOBAL: CARM95 0x52133c
int gCD_is_in_drive;
// GLOBAL: CARM95 0x521340
br_pixelmap* gRearview_screen;
// GLOBAL: CARM95 0x521344
br_scalar gCut_delay_1;
// GLOBAL: CARM95 0x521348
br_scalar gCut_delay_2;
// GLOBAL: CARM95 0x52134c
br_scalar gCut_delay_3;
// GLOBAL: CARM95 0x521350
br_scalar gCut_delay_4;
// GLOBAL: CARM95 0x521354
br_scalar gYon_multiplier;
// GLOBAL: CARM95 0x521358
tSave_game* gPre_race_saved_game;
FILE* gDiagnostic_file;
br_vector3 gDisabled_vector;
// GLOBAL: CARM95 0x52136c
tU32 gI_am_cheating;
char gNet_player_name[32];
// GLOBAL: CARM95 0x545440
br_matrix34 gRearview_camera_to_world;
// GLOBAL: CARM95 0x549eb0
tSpecial_volume gDefault_default_water_spec_vol;
// GLOBAL: CARM95 0x545400
br_matrix34 gCamera_to_world;
tRace_list_spec gRace_list[100];
// GLOBAL: CARM95 0x546820
tPath_name gApplication_path;
// GLOBAL: CARM95 0x54a068
float gPinball_factor;
// GLOBAL: CARM95 0x54a064
float gOpponent_speed_factor;
// GLOBAL: CARM95 0x54b250
float gCar_crush_min_fold;
float gNet_recovery_cost[7];
// GLOBAL: CARM95 0x545474
float gCar_crush_softness;
// GLOBAL: CARM95 0x54b248
br_vector3* gOur_pos;
// GLOBAL: CARM95 0x545368
float gCar_crush_max_fold;
// GLOBAL: CARM95 0x54a020
tBrender_storage gNet_cars_storage_space;
float gNet_softness[7];
// GLOBAL: CARM95 0x54a094
tS32 gJoystick_deadzone;
// GLOBAL: CARM95 0x54b270
float gCar_crush_limit_deviant;
float gNet_offensive[7];
// GLOBAL: CARM95 0x545470
float gCar_crush_split_chance;
// GLOBAL: CARM95 0x54532c
float gGravity_multiplier;
// GLOBAL: CARM95 0x545364
float gCar_crush_wibble;
float gRepair_cost[3];
// GLOBAL: CARM95 0x5454c0
float gCop_speed_factor;
// GLOBAL: CARM95 0x54535c
float gDefault_gravity;
// GLOBAL: CARM95 0x545334
tU32 gKeys_pressed;
// GLOBAL: CARM95 0x54a054
tSpecial_volume* gDefault_water_spec_vol;
float gNet_repair_cost[7];
// GLOBAL: CARM95 0x54537c
br_font* gFont_7;
// GLOBAL: CARM95 0x54a00c
tU32 gLast_replay_frame_time;
// GLOBAL: CARM95 0x5453f8
tU32 gTag_start_time;
// GLOBAL: CARM95 0x54b254
tOpponent* gOpponents;
tU32 gLast_credit_tick;
// GLOBAL: CARM95 0x549f80
tBrender_storage gTheir_cars_storage_space;
// GLOBAL: CARM95 0x5452fc
br_font* gHeadup_font;
// GLOBAL: CARM95 0x5454b8
tU32 gRace_finished;
tU32 gFrame_start_time;
// GLOBAL: CARM95 0x546930
tProgram_state gProgram_state;
tU32 gCredit_period_single[3];
// GLOBAL: CARM95 0x54b240
tU32 gRace_start;
// GLOBAL: CARM95 0x5453c0
tBrender_storage gTrack_storage_space;
// GLOBAL: CARM95 0x5453b0
tU32 gFrame_period;
// GLOBAL: CARM95 0x5467ac
tU32 gCredit_period;
tU32 gCredit_period_network[7];
// GLOBAL: CARM95 0x5467c0
tBrender_storage gOur_car_storage_space;
// GLOBAL: CARM95 0x54531c
tU32 gFox_start_time;
// GLOBAL: CARM95 0x546810
br_font* gBig_font;
// GLOBAL: CARM95 0x545480
tBrender_storage gPedestrians_storage_space;
// GLOBAL: CARM95 0x54a0a0
tRace_info gCurrent_race;
// GLOBAL: CARM95 0x54b26c
br_actor* gSelf;
// GLOBAL: CARM95 0x549fb4
br_actor* gDont_render_actor;
// GLOBAL: CARM95 0x5453ac
br_pixelmap* gScreen;
// GLOBAL: CARM95 0x54a06c
br_pixelmap* gRender_screen;
// GLOBAL: CARM95 0x5467b4
br_actor* gRearview_camera;
// GLOBAL: CARM95 0x5453b4
br_scalar gCamera_hither;
// GLOBAL: CARM95 0x545340
br_pixelmap* gBack_screen;
// GLOBAL: CARM95 0x545338
br_scalar gCamera_angle;
// GLOBAL: CARM95 0x546798
br_pixelmap* gDigits_pix;
// GLOBAL: CARM95 0x5454b4
br_pixelmap* gRender_shade_table;
// GLOBAL: CARM95 0x5467b0
br_pixelmap* gRearview_depth_buffer;
br_pixelmap* gWhole_screen;
// GLOBAL: CARM95 0x54a058
br_pixelmap* gIcons_pix;
// GLOBAL: CARM95 0x54a060
br_scalar gCamera_yon;
// GLOBAL: CARM95 0x54533c
br_actor* gCamera;
// GLOBAL: CARM95 0x545360
br_actor* gArrow_actor;
br_pixelmap* gIcons_pix_low_res;
// GLOBAL: CARM95 0x545324
br_pixelmap* gDepth_buffer;
int gInitial_net_credits[5];
float gCar_cred_value[3];
// GLOBAL: CARM95 0x545430
int gNo_races_yet;
int gJump_start_fine[3];
// GLOBAL: CARM95 0x5454bc
int gReal_graf_data_index;
float gCar_time_value[3];
// GLOBAL: CARM95 0x54679c
int gTyping_slot;
int gCunning_stunt_bonus[3];
// GLOBAL: CARM95 0x54b258
int gKnobbled_frame_period;
// GLOBAL: CARM95 0x54b24c
br_actor* gNon_track_actor;
// GLOBAL: CARM95 0x546790
int gWait_for_it;
int gCheck_point_cash[3];
int gPoints_per_second[3];
// GLOBAL: CARM95 0x5453fc
br_actor* gUniverse_actor;
// GLOBAL: CARM95 0x546920
br_actor* gTrack_actor;
// GLOBAL: CARM95 0x5467f4
int gMap_mode;
int gPed_time_value[3];
// GLOBAL: CARM95 0x545328
int gAbandon_game;
// GLOBAL: CARM95 0x5467b8
int gLap;
// GLOBAL: CARM95 0x54b274
int gShow_opponents;
// GLOBAL: CARM95 0x54680c
int gTotal_laps;
// GLOBAL: CARM95 0x546794
int gFree_repairs;
// GLOBAL: CARM95 0x546924
int gFreeze_timer;
int gRoll_over_creds[3];
// GLOBAL: CARM95 0x545320
int gShow_peds_on_map;
// GLOBAL: CARM95 0x5453f4
int gOn_drugs;
int gWasted_time[3];
// GLOBAL: CARM95 0x54a008
int gCheckpoint;
int gRoll_over_time[3];
int gWasted_creds[3];
// GLOBAL: CARM95 0x54a004
int gTimer;
// GLOBAL: CARM95 0x549fb8
int gCheckpoint_count;
// GLOBAL: CARM95 0x54a05c
int gCountdown;
// GLOBAL: CARM95 0x549ffc
int gFrame_rate;
// GLOBAL: CARM95 0x545330
int gNumber_of_racers;
// GLOBAL: CARM95 0x54a09c
int gSound_enabled;
// GLOBAL: CARM95 0x54a098
int gGraf_spec_index;
// GLOBAL: CARM95 0x54a000
int gSound_available;
// GLOBAL: CARM95 0x545434
int gNumber_of_races;
char gDir_separator[3] = "";
