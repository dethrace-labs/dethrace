#include "globvars.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x005212f0
int gTyping;

// GLOBAL: CARM95 0x005212f4
int gArrow_mode;

// GLOBAL: CARM95 0x005212f8
int gInfo_on;

// GLOBAL: CARM95 0x005212fc
int gHost_abandon_game;

// GLOBAL: CARM95 0x00521300
int gRecover_car;

// GLOBAL: CARM95 0x00521304
int gDisallow_abandon_race;

int gPipe_reg_snap_before_new_game;

int gPipe_reg_snap_before_our_car;

int gPipe_reg_snap_after_our_car;

// GLOBAL: CARM95 0x00521314
int gAction_replay_mode;

// GLOBAL: CARM95 0x00521318
int gDefault_engine_noise_index;

// GLOBAL: CARM95 0x0052131c
int gAusterity_mode;

// GLOBAL: CARM95 0x00521320
int gSausage_override;

// GLOBAL: CARM95 0x00521324
int gAustere_override;

// GLOBAL: CARM95 0x00521328
int gSound_override;

// GLOBAL: CARM95 0x0052132c
int gKey_map_index;

// GLOBAL: CARM95 0x00521330
int gInterface_within_race_mode;

// GLOBAL: CARM95 0x00521334
int gCut_scene_override;

// GLOBAL: CARM95 0x00521338
int gGame_to_load = -1;

// GLOBAL: CARM95 0x0052133c
int gCD_is_in_drive;

// GLOBAL: CARM95 0x00521340
br_pixelmap* gRearview_screen;

// GLOBAL: CARM95 0x00521344
br_scalar gCut_delay_1;

// GLOBAL: CARM95 0x00521348
br_scalar gCut_delay_2;

// GLOBAL: CARM95 0x0052134c
br_scalar gCut_delay_3;

// GLOBAL: CARM95 0x00521350
br_scalar gCut_delay_4;

// GLOBAL: CARM95 0x00521354
br_scalar gYon_multiplier;

// GLOBAL: CARM95 0x00521358
tSave_game* gPre_race_saved_game;

FILE* gDiagnostic_file;

// GLOBAL: CARM95 0x00521360
br_vector3 gDisabled_vector = { 1000.f, 1000.f, 1000.f };

// GLOBAL: CARM95 0x0052136c
tU32 gI_am_cheating;

// GLOBAL: CARM95 0x0054b220
char gNet_player_name[32];

// GLOBAL: CARM95 0x00545440
br_matrix34 gRearview_camera_to_world;

// GLOBAL: CARM95 0x00549eb0
tSpecial_volume gDefault_default_water_spec_vol;

// GLOBAL: CARM95 0x00545400
br_matrix34 gCamera_to_world;

// GLOBAL: CARM95 0x005454d0
tRace_list_spec gRace_list[100];

// GLOBAL: CARM95 0x00546820
tPath_name gApplication_path;

// GLOBAL: CARM95 0x0054a068
float gPinball_factor;

// GLOBAL: CARM95 0x0054a064
float gOpponent_speed_factor;

// GLOBAL: CARM95 0x0054b250
float gCar_crush_min_fold;

// GLOBAL: CARM95 0x00549fc0
float gNet_recovery_cost[7];

// GLOBAL: CARM95 0x00545474
float gCar_crush_softness;

// GLOBAL: CARM95 0x0054b248
br_vector3* gOur_pos;

// GLOBAL: CARM95 0x00545368
float gCar_crush_max_fold;

// GLOBAL: CARM95 0x0054a020
tBrender_storage gNet_cars_storage_space;

// GLOBAL: CARM95 0x00545380
float gNet_softness[7];

// GLOBAL: CARM95 0x0054a094
tS32 gJoystick_deadzone;

// GLOBAL: CARM95 0x0054b270
float gCar_crush_limit_deviant;

// GLOBAL: CARM95 0x005452e0
float gNet_offensive[7];

// GLOBAL: CARM95 0x00545470
float gCar_crush_split_chance;

// GLOBAL: CARM95 0x0054532c
float gGravity_multiplier;

// GLOBAL: CARM95 0x00545364
float gCar_crush_wibble;

// GLOBAL: CARM95 0x00549fe0
float gRepair_cost[3];

// GLOBAL: CARM95 0x005454c0
float gCop_speed_factor;

// GLOBAL: CARM95 0x0054535c
float gDefault_gravity;

// GLOBAL: CARM95 0x00545334
tU32 gKeys_pressed;

// GLOBAL: CARM95 0x0054a054
tSpecial_volume* gDefault_water_spec_vol;

// GLOBAL: CARM95 0x00549f60
float gNet_repair_cost[7];

// GLOBAL: CARM95 0x0054537c
br_font* gFont_7;

// GLOBAL: CARM95 0x0054a00c
tU32 gLast_replay_frame_time;

// GLOBAL: CARM95 0x005453f8
tU32 gTag_start_time;

// GLOBAL: CARM95 0x0054b254
tOpponent* gOpponents;

tU32 gLast_credit_tick;

// GLOBAL: CARM95 0x00549f80
tBrender_storage gTheir_cars_storage_space;

// GLOBAL: CARM95 0x005452fc
br_font* gHeadup_font;

// GLOBAL: CARM95 0x005454b8
tU32 gRace_finished;

// GLOBAL: CARM95 0x0054B244
tU32 gFrame_start_time;

// GLOBAL: CARM95 0x00546930
tProgram_state gProgram_state;

// GLOBAL: CARM95 0x00545370
tU32 gCredit_period_single[3];

// GLOBAL: CARM95 0x0054b240
tU32 gRace_start;

// GLOBAL: CARM95 0x005453c0
tBrender_storage gTrack_storage_space;

// GLOBAL: CARM95 0x005453b0
tU32 gFrame_period;

// GLOBAL: CARM95 0x005467ac
tU32 gCredit_period;

// GLOBAL: CARM95 0x00545300
tU32 gCredit_period_network[7];

// GLOBAL: CARM95 0x005467c0
tBrender_storage gOur_car_storage_space;

// GLOBAL: CARM95 0x0054531c
tU32 gFox_start_time;

// GLOBAL: CARM95 0x00546810
br_font* gBig_font;

// GLOBAL: CARM95 0x00545480
tBrender_storage gPedestrians_storage_space;

// GLOBAL: CARM95 0x0054a0a0
tRace_info gCurrent_race;

// GLOBAL: CARM95 0x0054b26c
br_actor* gSelf;

// GLOBAL: CARM95 0x00549fb4
br_actor* gDont_render_actor;

// GLOBAL: CARM95 0x005453ac
br_pixelmap* gScreen;

// GLOBAL: CARM95 0x0054a06c
br_pixelmap* gRender_screen;

// GLOBAL: CARM95 0x005467b4
br_actor* gRearview_camera;

// GLOBAL: CARM95 0x005453b4
br_scalar gCamera_hither;

// GLOBAL: CARM95 0x00545340
br_pixelmap* gBack_screen;

// GLOBAL: CARM95 0x00545338
br_scalar gCamera_angle;

// GLOBAL: CARM95 0x00546798
br_pixelmap* gDigits_pix;

// GLOBAL: CARM95 0x005454b4
br_pixelmap* gRender_shade_table;

// GLOBAL: CARM95 0x005467b0
br_pixelmap* gRearview_depth_buffer;

br_pixelmap* gWhole_screen;

// GLOBAL: CARM95 0x0054a058
br_pixelmap* gIcons_pix;

// GLOBAL: CARM95 0x0054a060
br_scalar gCamera_yon;

// GLOBAL: CARM95 0x0054533c
br_actor* gCamera;

// GLOBAL: CARM95 0x00545360
br_actor* gArrow_actor;

br_pixelmap* gIcons_pix_low_res;

// GLOBAL: CARM95 0x00545324
br_pixelmap* gDepth_buffer;

// GLOBAL: CARM95 0x0054a080
int gInitial_net_credits[5];

// GLOBAL: CARM95 0x005467a0
float gCar_cred_value[3];

// GLOBAL: CARM95 0x00545430
int gNo_races_yet;

// GLOBAL: CARM95 0x005453a0
int gJump_start_fine[3];

// GLOBAL: CARM95 0x005454bc
int gReal_graf_data_index;

// GLOBAL: CARM95 0x00549ff0
float gCar_time_value[3];

// GLOBAL: CARM95 0x0054679c
int gTyping_slot;

// GLOBAL: CARM95 0x0054b260
int gCunning_stunt_bonus[3];

// GLOBAL: CARM95 0x0054b258
int gKnobbled_frame_period;

// GLOBAL: CARM95 0x0054b24c
br_actor* gNon_track_actor;

// GLOBAL: CARM95 0x00546790
int gWait_for_it;

// GLOBAL: CARM95 0x0054a070
int gCheck_point_cash[3];

// GLOBAL: CARM95 0x0054a010
int gPoints_per_second[3];

// GLOBAL: CARM95 0x005453fc
br_actor* gUniverse_actor;

// GLOBAL: CARM95 0x00546920
br_actor* gTrack_actor;

// GLOBAL: CARM95 0x005467f4
int gMap_mode;

// GLOBAL: CARM95 0x0054b290
int gPed_time_value[3];

// GLOBAL: CARM95 0x00545328
int gAbandon_game;

// GLOBAL: CARM95 0x005467b8
int gLap;

// GLOBAL: CARM95 0x0054b274
int gShow_opponents;

// GLOBAL: CARM95 0x0054680c
int gTotal_laps;

// GLOBAL: CARM95 0x00546794
int gFree_repairs;

// GLOBAL: CARM95 0x00546924
int gFreeze_timer;

// GLOBAL: CARM95 0x00549ea0
int gRoll_over_creds[3];

// GLOBAL: CARM95 0x00545320
int gShow_peds_on_map;

// GLOBAL: CARM95 0x005453f4
int gOn_drugs;

// GLOBAL: CARM95 0x0054b280
int gWasted_time[3];

// GLOBAL: CARM95 0x0054a008
int gCheckpoint;

// GLOBAL: CARM95 0x00546800
int gRoll_over_time[3];

// GLOBAL: CARM95 0x00545350
int gWasted_creds[3];

// GLOBAL: CARM95 0x0054a004
int gTimer;

// GLOBAL: CARM95 0x00549fb8
int gCheckpoint_count;

// GLOBAL: CARM95 0x0054a05c
int gCountdown;

// GLOBAL: CARM95 0x00549ffc
int gFrame_rate;

// GLOBAL: CARM95 0x00545330
int gNumber_of_racers;

// GLOBAL: CARM95 0x0054a09c
int gSound_enabled;

// GLOBAL: CARM95 0x0054a098
int gGraf_spec_index;

// GLOBAL: CARM95 0x0054a000
int gSound_available;

// GLOBAL: CARM95 0x00545434
int gNumber_of_races;

// GLOBAL: CARM95 0x00549eac
char gDir_separator[3];
