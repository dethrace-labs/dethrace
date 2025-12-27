#ifndef DR_TYPES_H
#define DR_TYPES_H

#include "constants.h"
#include "macros.h"

#include "brender.h"
#include "s3/s3.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// This is a bit of a hack for compiling with MSVC 4.20
#if _MSC_VER == 1020
typedef unsigned int uintptr_t;
typedef int intptr_t;
#endif

#include "pc-all/net_types.h"

typedef unsigned char tU8;
typedef signed char tS8;
typedef unsigned short tU16;
typedef signed short tS16;
typedef unsigned int tU32;
typedef signed int tS32;
typedef double tF64;
typedef signed short tX88;
typedef signed int tX1616;
typedef tU8 tNet_message_type;
typedef struct tCar_spec_struct tCar_spec;
typedef struct tCar_spec_struct2 tCar_spec2;
typedef struct tPath_node_struct tPath_node;
typedef struct tPath_section_struct tPath_section;
typedef tU32 tPlayer_ID;
typedef void tPipe_reset_proc(void);
typedef struct tPowerup tPowerup;

#ifdef DETHRACE_FIX_BUGS
typedef int tGot_proc(tPowerup*, tCar_spec*);
typedef void tLose_proc(tPowerup*, tCar_spec*);
typedef void tPeriodic_proc(tPowerup*, tU32);
#else
typedef int* tGot_proc(tPowerup*, tCar_spec*);
typedef void* tLose_proc(tPowerup*, tCar_spec*);
typedef void* tPeriodic_proc(tPowerup*, tU32);
#endif
typedef char tPath_name[256];
typedef struct tFlic_descriptor* tFlic_descriptor_ptr;
typedef struct exception_struct* tException_list;
typedef struct exception_struct tException_node;
typedef int tKey_array[123];
typedef tS32 tJoy_array[8];
typedef void* tPMFM2CB(br_material*);
typedef struct v11face DR_FACE;
typedef struct fmt_vertex DR_VERTEX;

typedef enum tDriver {
    eDriver_non_car_unused_slot = 0,
    eDriver_non_car = 1,
    eDriver_oppo = 2,
    eDriver_net_human = 3,
    eDriver_local_human = 4
} tDriver;

typedef enum tBounds_type {
    eUndefined = 0,
    eBounds_ws = 1,
    eBounds_cs = 2
} tBounds_type;

typedef enum tImpact_location {
    eImpact_top = 0,
    eImpact_bottom = 1,
    eImpact_left = 2,
    eImpact_right = 3,
    eImpact_front = 4,
    eImpact_back = 5,
    eImpact_location_count = 6,
    eImpact_unknown = 7
} tImpact_location;

typedef enum tAxis_comp {
    eAxis_x = 0,
    eAxis_y = 1,
    eAxis_z = 2
} tAxis_comp;

typedef enum tCondition_operator {
    eCondition_less_than = 0,
    eCondition_greater_than = 1
} tCondition_operator;

typedef enum tDamage_type {
    eDamage_engine = 0,
    eDamage_transmission = 1,
    eDamage_driver = 2,
    eDamage_steering = 3,
    eDamage_lf_brake = 4,
    eDamage_rf_brake = 5,
    eDamage_lr_brake = 6,
    eDamage_rr_brake = 7,
    eDamage_lf_wheel = 8,
    eDamage_rf_wheel = 9,
    eDamage_lr_wheel = 10,
    eDamage_rr_wheel = 11,
    eDamage_type_count = 12
} tDamage_type;

typedef enum tJustification {
    eJust_left = 0,
    eJust_right = 1,
    eJust_centre = 2
} tJustification;

typedef enum tOpponent_objective_type {
    eOOT_none = 0,
    eOOT_complete_race = 1,
    eOOT_pursue_and_twat = 2,
    eOOT_run_away = 3,
    eOOT_get_near_player = 4,
    eOOT_levitate = 5,
    eOOT_knackered_and_freewheeling = 6,
    eOOT_frozen = 7,
    eOOT_wait_for_some_hapless_sod = 8,
    eOOT_rematerialise = 9,
    eOOT_return_to_start = 10
} tOpponent_objective_type;

typedef enum tFollow_path_result {
    eFPR_OK = 0,
    eFPR_end_of_path = 1,
    eFPR_given_up = 2
} tFollow_path_result;

typedef enum tPath_section_type_enum {
    ePST_normal = 0,
    ePST_race_path = 1,
    ePST_cheat_only = 2,
    ePST_count = 3
} tPath_section_type_enum;

typedef enum tPursue_car_state {
    ePCS_what_now = 0,
    ePCS_following_trail = 1,
    ePCS_following_line_of_sight = 2,
    ePCS_backing_up = 3
} tPursue_car_state;

typedef enum tProcess_objective_command {
    ePOC_start = 0,
    ePOC_run = 1,
    ePOC_die = 2
} tProcess_objective_command;

typedef enum tParts_category {
    eParts_armour = 0,
    eParts_power = 1,
    eParts_offensive = 2,
    eParts_count = 3
} tParts_category;

typedef enum tRace_over_reason {
    eRace_not_over_yet = -1,
    eRace_over_laps = 0,
    eRace_over_peds = 1,
    eRace_over_opponents = 2,
    eRace_over_abandoned = 3,
    eRace_over_out_of_time = 4,
    eRace_over_demo = 5,
    eRace_over_network_victory = 6,
    eRace_over_network_loss = 7,
    eRace_over_count = 8
} tRace_over_reason;

typedef enum tNet_mode {
    eNet_mode_none = 0,
    eNet_mode_thinking_about_it = 1,
    eNet_mode_host = 2,
    eNet_mode_client = 3
} tNet_mode;

typedef enum tPlayer_status {
    ePlayer_status_unknown = 0,
    ePlayer_status_ready = 1,
    ePlayer_status_loading = 2,
    ePlayer_status_wrecks_gallery = 3,
    ePlayer_status_summary = 4,
    ePlayer_status_not_responding = 5,
    ePlayer_status_racing = 6,
    ePlayer_status_main_menu = 7,
    ePlayer_status_recovering = 8,
    ePlayer_status_action_replay = 9
} tPlayer_status;

typedef enum tNet_game_type {
    eNet_game_type_fight_to_death = 0,
    eNet_game_type_car_crusher = 1,
    eNet_game_type_carnage = 2,
    eNet_game_type_checkpoint = 3,
    eNet_game_type_sudden_death = 4,
    eNet_game_type_tag = 5,
    eNet_game_type_foxy = 6,
    eNet_game_type_count = 7
} tNet_game_type;

typedef enum tNet_game_stage {
    eNet_game_starting = 0,
    eNet_game_ready = 1,
    eNet_game_playing = 2,
    eNet_game_status_count = 3
} tNet_game_stage;

typedef enum tCar_choice {
    eNet_car_frankie = 0,
    eNet_car_annie = 1,
    eNet_car_both = 2,
    eNet_car_all = 3
} tCar_choice;

typedef enum tNet_sequence_type {
    eNet_sequence_sequential = 0,
    eNet_sequence_random = 1
} tNet_sequence_type;

typedef enum tPowerup_event {
    ePowerup_gained = 0,
    ePowerup_ongoing = 1,
    ePowerup_lost = 2
} tPowerup_event;

typedef enum tNet_gameplay_mess {
    eNet_gameplay_checkpoint = 0,
    eNet_gameplay_wrong_checkpoint = 1,
    eNet_gameplay_suddenly_death = 2,
    eNet_gameplay_suicide = 3,
    eNet_gameplay_go_for_it = 4,
    eNet_gameplay_host_paused = 5,
    eNet_gameplay_host_unpaused = 6,
    eNet_gameplay_earn_credits = 7
} tNet_gameplay_mess;

typedef enum tCar_detail_ownership {
    eCar_owner_none = 0,
    eCar_owner_someone = 1,
    eCar_owner_self = 2,
    eCar_owner_not_allowed = 3
} tCar_detail_ownership;

typedef enum tPowerup_type {
    ePowerup_dummy = 0,
    ePowerup_instantaneous = 1,
    ePowerup_timed = 2,
    ePowerup_whole_race = 3
} tPowerup_type;

typedef enum tNet_powerup_type {
    eNet_powerup_local = 0,
    eNet_powerup_global = 1,
    eNet_powerup_inappropriate = 2
} tNet_powerup_type;

typedef enum tVehicle_type {
    eVehicle_self = 0,
    eVehicle_net_player = 1,
    eVehicle_opponent = 2,
    eVehicle_rozzer = 3,
    eVehicle_drone = 4,
    eVehicle_not_really = 5
} tVehicle_type;

typedef enum tDepth_effect_type {
    eDepth_effect_none = -1,
    eDepth_effect_darkness = 0,
    eDepth_effect_fog = 1
} tDepth_effect_type;

typedef enum tPlane_type {
    ePlane_pos_x = 0,
    ePlane_neg_x = 1,
    ePlane_pos_y = 2,
    ePlane_neg_y = 3,
    ePlane_pos_z = 4,
    ePlane_neg_z = 5,
    ePlane_general = 6
} tPlane_type;

typedef enum tNet_avail {
    eNet_avail_never = 0,
    eNet_avail_eagle = 1,
    eNet_avail_hawk = 2,
    eNet_avail_all = 3
} tNet_avail;

typedef enum tProg_status {
    eProg_intro = 0,
    eProg_opening = 1,
    eProg_idling = 2,
    eProg_demo = 3,
    eProg_game_starting = 4,
    eProg_game_ongoing = 5,
    eProg_quit = 6
} tProg_status;

typedef enum tMM_result {
    eMM_none = 0,
    eMM_continue = 1,
    eMM_end_game = 2,
    eMM_1_start = 3,
    eMM_n_start = 4,
    eMM_loaded = 5,
    eMM_save = 6,
    eMM_options = 7,
    eMM_quit = 8,
    eMM_timeout = 9,
    eMM_recover = 10,
    eMM_abort_race = 11
} tMM_result;

typedef enum tSO_result {
    eSO_main_menu_invoked = 0,
    eSO_game_over = 1,
    eSO_game_completed = 2,
    eSO_continue = 3
} tSO_result;

typedef enum tRace_result {
    eRace_game_abandonned = 0,
    eRace_aborted = 1,
    eRace_timed_out = 2,
    eRace_completed = 3
} tRace_result;

typedef enum tFrank_anne {
    eFrankie = 0,
    eAnnie = 1
} tFrank_anne;

typedef enum tRace_sel_view_type {
    eVT_Scene = 0,
    eVT_Info = 1,
    eVT_Opponents = 2
} tRace_sel_view_type;

typedef enum tAuto_parts_reply {
    eAP_auto = 0,
    eAP_manual = 1,
    eAP_piss_off = 2
} tAuto_parts_reply;

typedef enum tWhich_view {
    eView_undefined = 0,
    eView_left = 1,
    eView_forward = 2,
    eView_right = 3
} tWhich_view;

typedef enum tRolling_type {
    eRT_alpha = 0,
    eRT_numeric = 1,
    eRT_looping_random = 2,
    eRT_looping_single = 3
} tRolling_type;

typedef enum tCar_texturing_level {
    eCTL_none = 0,
    eCTL_transparent = 1,
    eCTL_full = 2,
    eCTL_count = 3
} tCar_texturing_level;

typedef enum tRoad_texturing_level {
    eRTL_none = 0,
    eRTL_full = 1,
    eRTL_count = 2
} tRoad_texturing_level;

typedef enum tWall_texturing_level {
    eWTL_none = 0,
    eWTL_linear = 1,
    eWTL_full = 2,
    eWTL_count = 3
} tWall_texturing_level;

typedef enum tAdd_to_storage_result {
    eStorage_not_enough_room = 0,
    eStorage_duplicate = 1,
    eStorage_allocated = 2
} tAdd_to_storage_result;

typedef enum tLollipop_mode {
    eLollipop_none = -1,
    eLollipop_x_match = 0,
    eLollipop_y_match = 1,
    eLollipop_z_match = 2
} tLollipop_mode;

typedef enum tSmear_type {
    eSmear_oil = 0,
    eSmear_blood = 1,
    eSmear_count = 2
} tSmear_type;

// Make gcc happy
typedef struct exception_ {
    int type;
    char* name;
    double arg1;
    double arg2;
    double retval;
} exception_;

typedef struct tTrack_spec {
    tU8 ncolumns_x;
    tU8 ncolumns_z;
    br_scalar column_size_x;
    br_scalar column_size_z;
    br_scalar origin_x;
    br_scalar origin_z;
    br_actor* the_actor;
    br_actor*** columns;
    br_actor*** lollipops;
    br_actor*** blends;
    int ampersand_digits;
    br_actor** non_car_list;
} tTrack_spec;

typedef struct tCrush_neighbour {
    br_uint_8 vertex_index;
    br_uint_8 factor;
} tCrush_neighbour;

typedef struct tCrush_point_spec {
    br_uint_16 vertex_index;
    br_uint_16 number_of_neighbours;
    br_vector3 limits_neg;
    br_vector3 limits_pos;
    br_vector3 softness_neg;
    br_vector3 softness_pos;
    tCrush_neighbour* neighbours;
} tCrush_point_spec;

typedef struct tCrush_data {
    int number_of_crush_points;
    float softness_factor;
    float min_fold_factor;
    float max_fold_factor;
    float wibble_factor;
    float limit_deviant;
    float split_chance;
    br_scalar min_y_fold_down;
    tCrush_point_spec* crush_points;
} tCrush_data;

typedef struct tSpecial_volume {
    br_matrix34 mat;
    br_matrix34 inv_mat;
    br_bounds bounds;
    br_scalar gravity_multiplier;
    br_scalar viscosity_multiplier;
    float car_damage_per_ms;
    float ped_damage_per_ms;
    int no_mat;
    int camera_special_effect_index;
    int sky_col;
    int entry_noise;
    int exit_noise;
    int engine_noise_index;
    br_material* screen_material;
    int material_modifier_index;
} tSpecial_volume;

typedef struct tReduced_matrix {
    br_vector3 row1;
    br_vector3 row2;
    br_vector3 translation;
} tReduced_matrix;

typedef struct tCar_controls { // size: 0x4
    int joystick_acc : 8;
    int joystick_dec : 8;
    unsigned int left : 1;        // 0x10000   bit 17
    unsigned int right : 1;       // 0x20000   bit 18
    unsigned int acc : 1;         // 0x40000   bit 19
    unsigned int dec : 1;         // 0x80000   bit 20
    unsigned int brake : 1;       // 0x100000   bit 21
    unsigned int up : 1;          // 0x200000   bit 22
    unsigned int down : 1;        // 0x400000   bit 23
    unsigned int holdw : 1;       // 0x800000   bit 24
    unsigned int backwards : 1;   // 0x1000000   bit 25
    unsigned int change_up : 1;   // 0x2000000   bit 26
    unsigned int change_down : 1; // 0x4000000   bit 27
    unsigned int horn : 1;        // 0x8000000   bit 28
} tCar_controls;

typedef struct tNet_message_mechanics_info { // size: 0x84
    tU8 contents_size;                       // @0x0
    tNet_message_type type;                  // @0x1
    tU32 ID;                                 // @0x4
    tU32 time;                               // @0x8
    tReduced_matrix mat;                     // @0xc
    br_vector3 v;                            // @0x30
    br_vector3 omega;                        // @0x3c
    tU8 d[4];                                // @0x48
    tCar_controls keys;                      // @0x4c
    tU32 cc_coll_time;                       // @0x50
    tS16 curvature;                          // @0x54
    tU16 revs;                               // @0x56
    br_scalar front;                         // @0x58
    br_scalar back;                          // @0x5c
    tU32 repair_time;                        // @0x60
    tU8 damage[12];                          // @0x64
    tU16 powerups;                           // @0x70
    br_scalar wheel_dam_offset[4];           // @0x74
} tNet_message_mechanics_info;

typedef struct tDamage_unit {
    int x_coord;
    int y_coord;
    int damage_level;
    int last_level;
    int smoke_last_level;
    int periods[5];
    br_pixelmap* images;
} tDamage_unit;

typedef struct tDamage_condition {
    tAxis_comp axis_comp;
    tCondition_operator condition_operator;
    float comparitor;
} tDamage_condition;

typedef struct tDamage_effect {
    tDamage_type type;
    float weakness_factor;
} tDamage_effect;

typedef struct tDamage_clause {
    tDamage_condition conditions[2];
    int effect_count;
    int condition_count;
    tDamage_effect effects[4];
} tDamage_clause;

typedef struct tDamage_program {
    int clause_count;
    tDamage_clause* clauses;
} tDamage_program;

typedef struct tHeadup_slot {
    int x;
    int y;
    int colour;
    int cockpit_anchored;
    int dimmed_background;
    int dim_left;
    int dim_top;
    int dim_right;
    int dim_bottom;
    tJustification justification;
} tHeadup_slot;

typedef struct tPart_info {
    char part_name[14];
    tU8* data_ptr;
    tU32 data_length;
    int rank_required;
    int prices[3];
} tPart_info;

typedef struct tParts_spec {
    int number_of_parts;
    tPart_info info[6];
} tParts_spec;

typedef struct tCar_actor {
    br_actor* actor;
    br_scalar min_distance_squared;
    tCrush_data crush_data;
    br_vertex* undamaged_vertices;
} tCar_actor;

typedef struct tJoystick {
    tS32 left;
    tS32 right;
    tS32 acc;
    tS32 dec;
} tJoystick;

typedef struct tPursuee_trail {   // size: 0x144
    br_vector3 trail_nodes[25];   // @0x0
    br_vector3 base_heading;      // @0x12c
    tU32 time_of_next_recording;  // @0x138
    tU32 end_of_deviation;        // @0x13c
    tU8 number_of_nodes;          // @0x140
    tU8 has_deviated_recently;    // @0x141
    tU8 nodes_shifted_this_frame; // @0x142
} tPursuee_trail;

typedef struct tCar_spec_struct {              // size: 0x1a9c
    int index;                                 // @0x0
    int disabled;                              // @0x4
    tDriver driver;                            // @0x8
    br_actor* car_master_actor;                // @0xc
    br_scalar min_torque_squared;              // @0x10
    br_scalar break_off_radians_squared;       // @0x14
    br_vector3 v;                              // @0x18
    br_vector3 old_v;                          // @0x24
    br_vector3 velocity_car_space;             // @0x30
    br_matrix34 oldmat;                        // @0x3c
    br_matrix34 old_frame_mat;                 // @0x6c
    br_vector3 pos;                            // @0x9c
    br_vector3 omega;                          // @0xa8
    br_vector3 oldomega;                       // @0xb4
    br_scalar M;                               // @0xc0
    int infinite_mass;                         // @0xc4
    br_vector3 I;                              // @0xc8
    br_vector3 cmpos;                          // @0xd4
    int extra_point_num;                       // @0xe0
    br_bounds bounds[3];                       // @0xe4
    br_bounds max_bounds[2];                   // @0x12c
    br_vector3 extra_points[6];                // @0x15c
    br_scalar original_extra_points_z[6];      // @0x1a4
    br_vector3 old_point;                      // @0x1bc
    br_vector3 old_norm;                       // @0x1c8
    int box_face_start;                        // @0x1d4
    int box_face_end;                          // @0x1d8
    int box_face_ref;                          // @0x1dc
    br_matrix34 last_box_inv_mat;              // @0x1e0
    br_bounds last_box;                        // @0x210
    int doing_nothing_flag;                    // @0x228
    tSpecial_volume* last_special_volume;      // @0x22c
    tSpecial_volume* auto_special_volume;      // @0x230
    int frame_collision_flag;                  // @0x234
    int collision_flag;                        // @0x238
    int max_shrapnel_material;                 // @0x23c
    br_vector3 direction;                      // @0x240
    float speed;                               // @0x24c
    tU16 car_ID;                               // @0x250
    br_material* shrapnel_material[3];         // @0x254
    br_bounds bounds_world_space;              // @0x260
    tBounds_type bounds_ws_type;               // @0x278
    tU16 fire_vertex[12];                      // @0x27c
    tU16 num_smoke_columns;                    // @0x294
    br_vector3 water_normal;                   // @0x298
    br_scalar water_d;                         // @0x2a4
    br_scalar water_depth_factor;              // @0x2a8
    tNet_message_mechanics_info message;       // @0x2ac
    tU32 last_car_car_collision;               // @0x330
    br_scalar dt;                              // @0x334
    tCar_spec* who_last_hit_me;                // @0x338
    char name[32];                             // @0x33c
    char driver_name[32];                      // @0x35c
    char grid_icon_names[3][14];               // @0x37c
    tS8* cockpit_images[3];                    // @0x3a8
    br_pixelmap* prat_cam_left;                // @0x3b4
    br_pixelmap* prat_cam_top;                 // @0x3b8
    br_pixelmap* prat_cam_right;               // @0x3bc
    br_pixelmap* prat_cam_bottom;              // @0x3c0
    br_pixelmap* prat_cam_dummy;               // @0x3c4
    br_pixelmap* speedo_image[2];              // @0x3c8
    br_pixelmap* tacho_image[2];               // @0x3d0
    br_pixelmap* damage_background;            // @0x3d8
    br_pixelmap* lhands_images[7];             // @0x3dc
    br_pixelmap* rhands_images[7];             // @0x3f8
    br_pixelmap* grid_icon_image;              // @0x414
    br_pixelmap* gears_image;                  // @0x418
    int fg_index;                              // @0x41c
    int underwater_ability;                    // @0x420
    int invulnerable;                          // @0x424
    int wall_climber_mode;                     // @0x428
    int can_be_stolen;                         // @0x42c
    int has_been_stolen;                       // @0x430
    int active;                                // @0x434
    int knackered;                             // @0x438
    int pre_car_col_knackered;                 // @0x43c
    int render_left[3];                        // @0x440
    int render_top[3];                         // @0x44c
    int render_right[3];                       // @0x458
    int render_bottom[3];                      // @0x464
    int mirror_left;                           // @0x470
    int mirror_top;                            // @0x474
    int mirror_right;                          // @0x478
    int mirror_bottom;                         // @0x47c
    int prat_left;                             // @0x480
    int prat_top;                              // @0x484
    int prat_right;                            // @0x488
    int prat_bottom;                           // @0x48c
    int speedo_x[2];                           // @0x490
    int speedo_y[2];                           // @0x498
    int speedo_centre_x[2];                    // @0x4a0
    int speedo_centre_y[2];                    // @0x4a8
    int speedo_x_pitch[2];                     // @0x4b0
    int speedo_y_pitch[2];                     // @0x4b8
    int speedo_radius_1[2];                    // @0x4c0
    int speedo_radius_2[2];                    // @0x4c8
    int speedo_start_angle[2];                 // @0x4d0
    int speedo_end_angle[2];                   // @0x4d8
    int speedo_needle_colour[2];               // @0x4e0
    int tacho_x[2];                            // @0x4e8
    int tacho_y[2];                            // @0x4f0
    int tacho_centre_x[2];                     // @0x4f8
    int tacho_centre_y[2];                     // @0x500
    int tacho_x_pitch[2];                      // @0x508
    int tacho_y_pitch[2];                      // @0x510
    int tacho_radius_1[2];                     // @0x518
    int tacho_radius_2[2];                     // @0x520
    int tacho_start_angle[2];                  // @0x528
    int tacho_end_angle[2];                    // @0x530
    int tacho_needle_colour[2];                // @0x538
    int gear_x[2];                             // @0x540
    int gear_y[2];                             // @0x548
    int red_line;                              // @0x550
    int lhands_x[7];                           // @0x554
    int lhands_y[7];                           // @0x570
    int rhands_x[7];                           // @0x58c
    int rhands_y[7];                           // @0x5a8
    int number_of_hands_images;                // @0x5c4
    int max_speed;                             // @0x5c8
    int damage_x_offset;                       // @0x5cc
    int damage_y_offset;                       // @0x5d0
    int damage_background_x;                   // @0x5d4
    int damage_background_y;                   // @0x5d8
    int dim_count[2];                          // @0x5dc
    int dim_left[2][4];                        // @0x5e4
    int dim_top[2][4];                         // @0x604
    int dim_right[2][4];                       // @0x624
    int dim_bottom[2][4];                      // @0x644
    int car_actor_count;                       // @0x664
    int current_car_actor;                     // @0x668
    int principal_car_actor;                   // @0x66c
    int car_model_variable;                    // @0x670
    int number_of_steerable_wheels;            // @0x674
    int steering_ref[6];                       // @0x678
    int lf_sus_ref[4];                         // @0x690
    int rf_sus_ref[4];                         // @0x6a0
    int lr_sus_ref[2];                         // @0x6b0
    int rr_sus_ref[2];                         // @0x6b8
    int driven_wheels_spin_ref_1;              // @0x6c0
    int driven_wheels_spin_ref_2;              // @0x6c4
    int driven_wheels_spin_ref_3;              // @0x6c8
    int driven_wheels_spin_ref_4;              // @0x6cc
    int non_driven_wheels_spin_ref_1;          // @0x6d0
    int non_driven_wheels_spin_ref_2;          // @0x6d4
    int non_driven_wheels_spin_ref_3;          // @0x6d8
    int non_driven_wheels_spin_ref_4;          // @0x6dc
    int engine_noises[3];                      // @0x6e0
    float driver_x_offset;                     // @0x6ec
    float driver_y_offset;                     // @0x6f0
    float driver_z_offset;                     // @0x6f4
    float mirror_x_offset;                     // @0x6f8
    float mirror_y_offset;                     // @0x6fc
    float mirror_z_offset;                     // @0x700
    float rearview_camera_angle;               // @0x704
    float head_left_angle;                     // @0x708
    float head_right_angle;                    // @0x70c
    float steering_angle;                      // @0x710
    float speedo_speed;                        // @0x714
    float lf_sus_position;                     // @0x718
    float rf_sus_position;                     // @0x71c
    float lr_sus_position;                     // @0x720
    float rr_sus_position;                     // @0x724
    float driven_wheels_circum;                // @0x728
    float non_driven_wheels_circum;            // @0x72c
    float bounce_rate;                         // @0x730
    float bounce_amount;                       // @0x734
    float collision_mass_multiplier;           // @0x738
    float damage_multiplier;                   // @0x73c
    float grip_multiplier;                     // @0x740
    float engine_power_multiplier;             // @0x744
    tDamage_unit damage_units[12];             // @0x748
    tU8 frame_start_damage[12];                // @0x958
    tImpact_location last_impact_location;     // @0x964
    tDamage_program damage_programs[6];        // @0x968
    tHeadup_slot headup_slots[2][20];          // @0x998
    tParts_spec power_ups[3];                  // @0xfd8
    int car_actor_pipe_ref;                    // @0x12b4
    tCar_actor car_model_actors[5];            // @0x12b8
    br_material* screen_material;              // @0x13a8
    br_material* screen_material_source;       // @0x13ac
    br_matrix34 last_safe_positions[5];        // @0x13b0
    int wheel_slip;                            // @0x14a0
    br_scalar damping;                         // @0x14a4
    br_scalar sk[2];                           // @0x14a8
    br_scalar sb[2];                           // @0x14b0
    br_scalar susp_give[2];                    // @0x14b8
    br_scalar susp_height[2];                  // @0x14c0
    br_scalar ride_height;                     // @0x14c8
    br_vector3 wpos[4];                        // @0x14cc
    br_scalar curvature;                       // @0x14fc
    br_scalar maxcurve;                        // @0x1500
    br_scalar turn_speed;                      // @0x1504
    br_scalar oldd[4];                         // @0x1508
    int material_index[4];                     // @0x1518
    int dust_time[4];                          // @0x1528
    br_scalar mu[3];                           // @0x1538
    br_scalar friction_elipticity;             // @0x1544
    br_scalar down_force_speed;                // @0x1548
    int down_force_flag;                       // @0x154c
    br_scalar initial_brake;                   // @0x1550
    br_scalar brake_increase;                  // @0x1554
    br_scalar freduction;                      // @0x1558
    br_scalar acc_force;                       // @0x155c
    br_scalar torque;                          // @0x1560
    br_scalar brake_force;                     // @0x1564
    int traction_control;                      // @0x1568
    br_scalar rolling_r_front;                 // @0x156c
    br_scalar rolling_r_back;                  // @0x1570
    tCar_controls keys;                        // @0x1574
    tJoystick joystick;                        // @0x1578
    int pedals_xy;                             // @0x1588
    int number_of_wheels_on_ground;            // @0x158c
    br_actor* wheel_actors[6];                 // @0x1590
    float wheel_rot_pos[4];                    // @0x15a8
    br_scalar wheel_dam_offset[4];             // @0x15b8
    br_scalar damage_magnitude_accumulator;    // @0x15c8
    br_scalar revs;                            // @0x15cc
    br_scalar target_revs;                     // @0x15d0
    br_vector3 road_normal;                    // @0x15d4
    br_scalar max_force_front;                 // @0x15e0
    br_scalar max_force_rear;                  // @0x15e4
    int gear;                                  // @0x15e8
    int just_changed_gear;                     // @0x15ec
    int max_gear;                              // @0x15f0
    br_scalar speed_revs_ratio;                // @0x15f4
    br_scalar force_torque_ratio;              // @0x15f8
    tS3_sound_source_ptr sound_source;         // @0x15fc
    br_matrix34 pre_car_col_mat;               // @0x1600
    br_scalar pre_car_col_speed;               // @0x1630
    br_vector3 pre_car_col_direction;          // @0x1634
    br_vector3 pre_car_col_velocity;           // @0x1640
    br_vector3 pre_car_col_velocity_car_space; // @0x164c
    br_vector3 velocity_bu_per_sec;            // @0x1658
    float last_col_prop_x;                     // @0x1664
    float last_col_prop_y;                     // @0x1668
    float last_col_prop_z;                     // @0x166c
    tU32 time_last_hit;                        // @0x1670
    tU32 time_last_victim;                     // @0x1674
    struct tCar_spec_struct* last_hit_by;      // @0x1678
    struct tCar_spec_struct* last_culprit;     // @0x167c
    int no_of_processes_recording_my_trail;    // @0x1680
    tPursuee_trail my_trail;                   // @0x1684
    unsigned int grudge_raised_recently : 1;   // @0x17c8  // 0x1
    unsigned int big_bang : 1;                 // @0x17c8  // 0x2
    unsigned int scary_bang : 1;               // @0x17c8  // 0x4
    tU32 last_collision_time;                  // @0x17cc
    tU32 last_time_we_touched_a_player;        // @0x17d0
    tU32 end_steering_damage_effect;           // @0x17d4
    tU32 end_trans_damage_effect;              // @0x17d8
    int false_key_left;                        // @0x17dc
    int false_key_right;                       // @0x17e0
    tCar_spec* last_person_to_hit_us;          // @0x17e4
    tCar_spec* last_person_we_hit;             // @0x17e8
    br_vector3 engine_pos;                     // @0x17ec
    br_model* last_wheel_models[4];            // @0x17f8
    int last_wheel_faces[4];                   // @0x1808
    tU32 shadow_intersection_flags;            // @0x1818
    tU32 last_bounce;                          // @0x181c
    unsigned int new_skidding;                 // @0x1820
    unsigned int old_skidding;                 // @0x1824
    tU16 old_skid[4];                          // @0x1828
    br_vector3 prev_skid_pos[4];               // @0x1830
    br_vector3 skid_line_start[4];             // @0x1860
    br_vector3 skid_line_end[4];               // @0x1890
    br_vector3 nor[4];                         // @0x18c0
    br_vector3 prev_nor[4];                    // @0x18f0
    br_vector3 special_start[4];               // @0x1920
    br_scalar oil_remaining[4];                // @0x1950
    br_scalar blood_remaining[4];              // @0x1960
    br_scalar total_length[4];                 // @0x1970
    float proxy_ray_distance;                  // @0x1980
    tS32 powerups[64];                         // @0x1984
    tU32 time_to_recover;                      // @0x1a84
    tU32 repair_time;                          // @0x1a88
    int power_up_levels[3];                    // @0x1a8c
    tS3_sound_tag horn_sound_tag;              // @0x1a98
} tCar_spec;

typedef struct tOppo_psyche {
    tU8 grudge_against_player;
} tOppo_psyche;

typedef struct tComplete_race_data {
    tU8 finished_calcing_race_route;
    tU8 found_race_section;
} tComplete_race_data;

typedef struct tReturn_to_start_data {
    br_vector3 nearest_path_point;
    tS16 section_no;
    tU8 waiting_near_start;
} tReturn_to_start_data;

typedef struct tPath_node_struct {
    br_vector3 p;
    tS16 sections[8];
    tU8 number_of_sections;
} tPath_node;

typedef struct tPath_section_struct { // size: 0x14
    tS16 node_indices[2];             // @0x0
    tU8 min_speed[2];                 // @0x4
    tU8 max_speed[2];                 // @0x6
    br_scalar width;                  // @0x8
    br_scalar length;                 // @0xc
    tU8 type;                         // @0x10
    tU8 one_way;                      // @0x11
} tPath_section;

typedef struct tPursue_car_data {       // size: 0x70
    tCar_spec* pursuee;                 // @0x0
    tU32 time_of_next_visibility_check; // @0x4
    tU32 start_backup_time;             // @0x8
    tU32 time_last_twatted_em;          // @0xc
    tU32 time_pursuee_last_visible;     // @0x10
    tU32 time_last_away_from_pursuee;   // @0x14
    tPath_node direct_line_nodes[2];    // @0x18
    tPath_section direct_line_section;  // @0x58
    tU8 state;                          // @0x6c
} tPursue_car_data;

typedef struct tFollow_path_data {               // size: 0x58
    tU32 struggle_time;                          // @0x0
    tU32 last_finished_struggle_time;            // @0x4
    tU32 toggle_time;                            // @0x8
    tU32 borrowed_time_start;                    // @0xc
    br_scalar prev_acc;                          // @0x10
    br_scalar prev_acc_error;                    // @0x14
    br_scalar desired_speed;                     // @0x18
    br_scalar last_distance;                     // @0x1c
    br_vector3 cheaty_intersect;                 // @0x20
    tS16 section_no;                             // @0x2c
    tS16 first_section_no;                       // @0x2e
    tS16 last_struggle_section;                  // @0x30
    unsigned int number_of_struggles : 8;        // @0x34
    unsigned int has_moved_during_this_task : 1; // @0x34  // 0x100
    unsigned int made_it : 1;                    // @0x34  // 0x200
    unsigned int cheating : 1;                   // @0x34  // 0x400
    unsigned int cornering : 1;                  // @0x34  // 0x800
    unsigned int left_not_right : 1;             // @0x34  // 0x1000
    unsigned int off_path_toggle;                // @0x38
    unsigned int moving_to_intersect;            // @0x3c
    br_vector2 turning_cent;                     // @0x40
    br_scalar turning_radius;                    // @0x48
    br_scalar corner_size;                       // @0x4c
    br_scalar corner_width;                      // @0x50
    int section_after;                           // @0x54
} tFollow_path_data;

typedef struct tLevitate_data {
    br_scalar initial_y;
    tU32 time_started;
    unsigned int waiting_to_levitate : 1;
} tLevitate_data;

typedef struct tRun_away_data {
    tU32 time_to_stop;
} tRun_away_data;

typedef struct tRoute_section { // size: 0x4
    tS16 section_no;            // @0x0
    tU8 direction;              // @0x2
} tRoute_section;

typedef struct tOpponent_spec {                      // size: 0x190
    int index;                                       // @0x0
    tOpponent_objective_type current_objective;      // @0x4
    tCar_spec* car_spec;                             // @0x8
    float nastiness;                                 // @0xc
    br_scalar distance_to_camera;                    // @0x10
    br_scalar distance_from_home;                    // @0x14
    br_scalar player_to_oppo_d;                      // @0x18
    br_vector3 start_pos;                            // @0x1c
    br_vector3 start_direction;                      // @0x28
    br_vector3 pos_last_frame;                       // @0x34
    br_vector3 player_to_oppo_v;                     // @0x40
    tU32 next_out_of_world_check;                    // @0x4c
    tU32 stun_time_ends;                             // @0x50
    tU32 next_player_visibility_check;               // @0x54
    tU32 last_moved_ok;                              // @0x58
    tU32 last_in_view;                               // @0x5c
    tU32 time_last_processed;                        // @0x60
    tU32 time_this_objective_started;                // @0x64
    tU32 time_for_this_objective_to_finish;          // @0x68
    tU32 cunting_buttfuck_timer;                     // @0x6c
    tS16 players_section_when_last_calced_full_path; // @0x70
    int nnext_sections;                              // @0x74
    tRoute_section next_sections[10];                // @0x78
    unsigned int new_objective_required : 1;         // @0xa0  // 0x1
    unsigned int finished_for_this_race : 1;         // @0xa0  // 0x2
    unsigned int knackeredness_detected : 1;         // @0xa0  // 0x4
    unsigned int physics_me : 1;                     // @0xa0  // 0x8
    unsigned int pursue_from_start : 1;              // @0xa0  // 0x10
    unsigned int cheating : 1;                       // @0xa0  // 0x20
    unsigned int last_cheating_value : 1;            // @0xa0  // 0x40
    unsigned int pursuing_player_before_freeze : 1;  // @0xa0  // 0x80
    unsigned int has_moved_at_some_point : 1;        // @0xa0  // 0x100
    unsigned int player_in_view_now : 1;             // @0xa0  // 0x200
    unsigned int acknowledged_piv : 1;               // @0xa0  // 0x400
    unsigned int murder_reported : 1;                // @0xa0  // 0x800
    tComplete_race_data complete_race_data;          // @0xa4
    tFollow_path_data follow_path_data;              // @0xa8
    tPursue_car_data pursue_car_data;                // @0x100
    tLevitate_data levitate_data;                    // @0x170
    tRun_away_data run_away_data;                    // @0x17c
    tReturn_to_start_data return_to_start_data;      // @0x180
} tOpponent_spec;

typedef struct tIntelligent_vehicles {
    int number_of_opponents;
    int number_of_cops;
    int number_of_path_nodes;
    int number_of_path_sections;
    br_vector3 cop_start_points[10];
    br_vector3 cop_start_vectors[10];
    tOpponent_spec opponents[5];
    tOpponent_spec cops[10];
    tPath_node* path_nodes;
    tPath_section* path_sections;
} tIntelligent_vehicles;

typedef struct tBounds {
    br_matrix34* mat;
    br_bounds original_bounds;
    br_vector3 box_centre;
    br_scalar radius;
    br_bounds real_bounds;
} tBounds;

typedef struct tFace_ref {
    br_material* material;
    br_vector3 v[3];
    br_vector2* map[3];
    br_vector3 normal;
    int flags;
    br_scalar d;
} tFace_ref;

#pragma pack(push, 4)
typedef struct tNet_game_player_info { // size: 0xc0
    tPD_net_player_info pd_net_info;   // @0x0
    tU32 this_players_time_stamp;      // @0x10
    tU32 last_heard_from_him;          // @0x14
    tU32 reposition_time;              // @0x18
    tU32 last_waste_message;           // @0x1c
    int host;                          // @0x20
    tPlayer_ID ID;                     // @0x24
    char player_name[32];              // @0x28
    tPlayer_status player_status;      // @0x48
    int car_index;                     // @0x4c
    int grid_index;                    // @0x50
    int grid_position_set;             // @0x54
    int opponent_list_index;           // @0x58
    int awaiting_confirmation;         // @0x5c
    int score;                         // @0x60
    int credits;                       // @0x64
    int wasted;                        // @0x68
    int wasteage_attributed;           // @0x6c
    int name_not_clipped;              // @0x70
    int race_stuff_initialised;        // @0x74
    int played;                        // @0x78
    int won;                           // @0x7c
    int next_car_index;                // @0x80
    int games_score;                   // @0x84
    int last_score_index;              // @0x88
    br_matrix34 initial_position;      // @0x8c
    tCar_spec* car;                    // @0xbc
} tNet_game_player_info;
#pragma pack(pop)

typedef struct tNet_game_options {         // size: 0x30
    int show_players_on_map;               // @0x0
    int show_peds_on_map;                  // @0x4
    int enable_text_messages;              // @0x8
    int show_powerups_on_map;              // @0xc
    int powerup_respawn;                   // @0x10
    int open_game;                         // @0x14
    int starting_money_index;              // @0x18
    int grid_start;                        // @0x1c
    int race_end_target;                   // @0x20
    int random_car_choice;                 // @0x24
    tNet_sequence_type race_sequence_type; // @0x28
    tCar_choice car_choice;                // @0x2c
} tNet_game_options;

typedef struct tNet_game_status { // size: 0x4
    tNet_game_stage stage;        // @0x0
} tNet_game_status;

typedef struct tNet_game_details {   // size: 0x78
    tPD_net_player_info pd_net_info; // @0x0
    char host_name[32];              // @0x10
    tPlayer_ID host_ID;              // @0x30
    int num_players;                 // @0x34
    int start_race;                  // @0x38
    int no_races_yet;                // @0x3c
    tNet_game_status status;         // @0x40
    tNet_game_options options;       // @0x44
    tNet_game_type type;             // @0x74
} tNet_game_details;

typedef struct tNet_message_send_me_details {
    tU8 contents_size;
    tNet_message_type type;
} tNet_message_send_me_details;

typedef struct tNet_message_my_details {
    tU8 contents_size;
    tNet_message_type type;
    tNet_game_details details;
} tNet_message_my_details;

typedef struct tNet_message_join {
    tU8 contents_size;
    tNet_message_type type;
    tNet_game_player_info player_info;
} tNet_message_join;

typedef struct tNet_message_leave {
    tU8 contents_size;
    tNet_message_type type;
} tNet_message_leave;

typedef struct tNet_message_host_pissing_off {
    tU8 contents_size;
    tNet_message_type type;
} tNet_message_host_pissing_off;

typedef struct tNet_message_new_player_list {
    tU8 contents_size;
    tNet_message_type type;
    int number_of_players;
    int this_index;
    int batch_number;
    tNet_game_player_info player;
} tNet_message_new_player_list;

typedef struct tNet_message_race_over {
    tU8 contents_size;
    tNet_message_type type;
    tRace_over_reason reason;
} tNet_message_race_over;

typedef struct tNet_message_status_report {
    tU8 contents_size;
    tNet_message_type type;
    tPlayer_status status;
} tNet_message_status_report;

typedef struct tGrid_spec {
    int index;
    int next_car_index;
    br_matrix34 mat;
} tGrid_spec;

typedef struct tNet_message_start_race {
    tU8 contents_size;
    tNet_message_type type;
    int car_count;
    int racing;
    int next_race;
    tGrid_spec car_list[6];
} tNet_message_start_race;

typedef struct tNet_message_guarantee_reply {
    tU8 contents_size;
    tNet_message_type type;
    tU32 guarantee_number;
} tNet_message_guarantee_reply;

typedef struct tNet_message_headup {
    tU8 contents_size;
    tNet_message_type type;
    char text[128];
} tNet_message_headup;

typedef struct tNet_message_host_query {
    tU8 contents_size;
    tNet_message_type type;
} tNet_message_host_query;

typedef struct tNet_message_host_reply {
    tU8 contents_size;
    tNet_message_type type;
    int race_has_started;
    int race_index;
    int pending_race;
} tNet_message_host_reply;

typedef struct tNet_message_cop_info {
    tU8 contents_size;
    tNet_message_type type;
    tU32 ID;
    tU32 time;
    tReduced_matrix mat;
    br_vector3 v;
    br_vector3 omega;
    br_scalar curvature;
    br_scalar d[4];
    tU8 damage[12];
} tNet_message_cop_info;

typedef struct tNet_message_non_car_info {
    tU8 contents_size;
    tNet_message_type type;
    tU32 ID;
    tU32 time;
    tReduced_matrix mat;
    br_vector3 v;
    br_vector3 omega;
    tU32 cc_coll_time;
    tU16 flags;
} tNet_message_non_car_info;

typedef struct tNet_message_non_car_position {
    tU8 contents_size;
    tNet_message_type type;
    tU32 ID;
    br_matrix34 mat;
    tU16 flags;
} tNet_message_non_car_position;

typedef struct tNet_message_time_sync {
    tU8 contents_size;
    tNet_message_type type;
    int race_start_time;
} tNet_message_time_sync;

typedef struct tNet_message_players_confirm {
    tU8 contents_size;
    tNet_message_type type;
    tPlayer_ID player;
} tNet_message_players_confirm;

typedef struct tNet_message_disable_car {
    tU8 contents_size;
    tNet_message_type type;
    tPlayer_ID player;
} tNet_message_disable_car;

typedef struct tNet_message_enable_car {
    tU8 contents_size;
    tNet_message_type type;
    tPlayer_ID player;
} tNet_message_enable_car;

typedef struct tNet_message_powerup {
    tU8 contents_size;
    tNet_message_type type;
    tPlayer_ID player;
    int powerup_index;
    tU32 time_left;
    tPowerup_event event;
} tNet_message_powerup;

typedef struct tNet_message_recover {
    tU8 contents_size;
    tNet_message_type type;
    tU32 ID;
    int time_to_recover;
} tNet_message_recover;

typedef struct tNet_message_scores {
    tU8 contents_size;
    tNet_message_type type;
    int general_score;
    int scores[6];
} tNet_message_scores;

typedef struct tNet_message_wasted {
    tU8 contents_size;
    tNet_message_type type;
    tPlayer_ID victim;
    tPlayer_ID culprit;
} tNet_message_wasted;

typedef struct tNet_message_gameplay {
    tU8 contents_size;
    tNet_message_type type;
    tNet_gameplay_mess mess;
    tU32 param_1;
    tU32 param_2;
    tU32 param_3;
    tU32 param_4;
} tNet_message_gameplay;

typedef struct tNet_message_pedestrian {
    tU8 contents_size;
    tNet_message_type type;
    tS8 action_instruction;
    tS8 flags;
    tS16 index;
    br_vector3 pos;
    br_scalar speed;
    br_vector3 to_pos;
    br_vector3 offset;
    tPlayer_ID murderer;
    tU32 respawn_time_or_spin_period;
    tS8 frame;
} tNet_message_pedestrian;

typedef struct tNet_message_car_details_req {
    tU8 contents_size;
    tNet_message_type type;
} tNet_message_car_details_req;

typedef struct tCar_details {
    int car_index;
    char owner[16];
} tCar_details;

typedef struct tNet_message_car_details {
    tU8 contents_size;
    tNet_message_type type;
    int count;
    tCar_details details[6];
} tNet_message_car_details;

typedef struct tGame_scores {
    int played;
    int won;
    int score;
} tGame_scores;

typedef struct tNet_message_game_scores {
    tU8 contents_size;
    tNet_message_type type;
    tGame_scores scores[6];
} tNet_message_game_scores;

typedef struct tNet_message_oil_spill {
    tU8 contents_size;
    tNet_message_type type;
    tPlayer_ID player;
    br_scalar full_size;
    br_scalar grow_rate;
    br_scalar current_size;
} tNet_message_oil_spill;

typedef struct tNet_message_crush_point {
    tU8 contents_size;
    tNet_message_type type;
    tPlayer_ID id;
    tU16 vertex;
    br_vector3 energy_vector;
} tNet_message_crush_point;

typedef union tNet_contents {                           // size: 0x160
    struct {                                            // size: 0x2
        tU8 contents_size;                              // @0x0
        tNet_message_type type;                         // @0x1
    } header;                                           // @0x0
    union {                                             // size: 0x160
        tNet_message_send_me_details send_details;      // @0x0
        tNet_message_my_details details;                // @0x0
        tNet_message_join join;                         // @0x0
        tNet_message_leave leave;                       // @0x0
        tNet_message_host_pissing_off hosticide;        // @0x0
        tNet_message_new_player_list player_list;       // @0x0
        tNet_message_race_over race_over;               // @0x0
        tNet_message_status_report report;              // @0x0
        tNet_message_start_race start_race;             // @0x0
        tNet_message_guarantee_reply reply;             // @0x0
        tNet_message_headup headup;                     // @0x0
        tNet_message_host_query where_we_at;            // @0x0
        tNet_message_host_reply heres_where_we_at;      // @0x0
        tNet_message_mechanics_info mech;               // @0x0
        tNet_message_non_car_info non_car;              // @0x0
        tNet_message_time_sync time_sync;               // @0x0
        tNet_message_players_confirm confirm;           // @0x0
        tNet_message_disable_car disable_car;           // @0x0
        tNet_message_enable_car enabled_car;            // @0x0
        tNet_message_powerup powerup;                   // @0x0
        tNet_message_recover recover;                   // @0x0
        tNet_message_scores scores;                     // @0x0
        tNet_message_wasted wasted;                     // @0x0
        tNet_message_pedestrian pedestrian;             // @0x0
        tNet_message_gameplay gameplay;                 // @0x0
        tNet_message_non_car_position non_car_position; // @0x0
        tNet_message_cop_info cop_info;                 // @0x0
        tNet_message_car_details_req car_details_req;   // @0x0
        tNet_message_car_details car_details;           // @0x0
        tNet_message_game_scores game_scores;           // @0x0
        tNet_message_oil_spill oil_spill;               // @0x0
        tNet_message_crush_point crush;                 // @0x0
    } data;                                             // @0x0
} tNet_contents;

#pragma pack(push, 4)
typedef struct tNet_message {    // size: 0x17c
    tU32 pd_stuff_so_DO_NOT_USE; // @0x0
    tU32 magic_number;           // @0x4
    tU32 guarantee_number;       // @0x8
    tPlayer_ID sender;           // @0xc
    int version;                 // @0x10
    tU32 senders_time_stamp;     // @0x14
    tU16 num_contents;           // @0x18
    tU16 overall_size;           // @0x1a
    tNet_contents contents;      // @0x1c
} tNet_message;
#pragma pack(pop)

typedef struct tCar_detail_info {
    tCar_detail_ownership ownership;
    char name[16];
} tCar_detail_info;

typedef struct tWav_header {
    char quote_RIFF[4];
    tU32 format_length;
    tU8 wave[4];
    tU8 fmt[4];
    tU32 wave_format_length;
    tU16 format_tag;
    tU16 channels;
    tU32 samples_per_sec;
    tU32 avg_bytes_per_sec;
    tU16 block_align;
    tU16 bits_per_sample;
    tU8 data[4];
    tU32 data_length;
} tWav_header;

typedef struct tPowerup {
    tPowerup_type type;
    tU32 got_time;
    tU32 duration;
    tU32 lose_time;
    tU16 group_inclusion;
    br_pixelmap* icon;
    int fizzle_type;
    int number_of_float_params;
    int number_of_integer_params;
    int* integer_params;
    int current_value;
    int prat_cam_event;
    tNet_powerup_type net_type;
    tGot_proc* got_proc;
    tLose_proc* lose_proc;
    tPeriodic_proc* periodic_proc;
    float* float_params;
    tCar_spec* car;
    char message[64];
} tPowerup;

typedef struct tDepth_effect {
    tDepth_effect_type type;
    int start;
    int end;
    br_pixelmap* sky_texture;
} tDepth_effect;

typedef struct tPlane_spec {
    tPlane_type plane_type;
    br_fvector3 n;
    br_scalar d;
} tPlane_spec;

typedef struct tMaterial_modifiers {
    br_scalar car_wall_friction;
    br_scalar tyre_road_friction;
    br_scalar down_force;
    br_scalar bumpiness;
    int tyre_noise_index;
    int crash_noise_index;
    int scrape_noise_index;
    br_scalar sparkiness;
    int smoke_type;
    br_material* skid_mark_material;
} tMaterial_modifiers;

typedef struct tSpecial_screen {
    br_material* material;
    br_scalar min_x;
    br_scalar min_z;
    br_scalar max_x;
    br_scalar max_z;
} tSpecial_screen;

typedef struct tRace_list_spec {
    char name[32];
    int rank_required;
    int best_rank;
    int suggested_rank;
    int been_there_done_that;
} tRace_list_spec;

typedef struct tRace_save_info {
    tU32 been_there_done_that;
} tRace_save_info;

typedef struct tGraf_spec {
    int depth_bits;
    int depth_bytes;
    int doubled;
    int total_width;
    int total_height;
    long black_value;
    int colour_index;
    char* data_dir_name;
    char* gfx_init_string;
    int row_bytes;
    int phys_width;
    int phys_height;
    void* base_addr;
} tGraf_spec;

typedef struct tCollision_info {          // size: 0x33c
    int index;                            // @0x0
    int disabled;                         // @0x4
    tDriver driver;                       // @0x8
    br_actor* car_master_actor;           // @0xc
    br_scalar min_torque_squared;         // @0x10
    br_scalar break_off_radians_squared;  // @0x14
    br_vector3 v;                         // @0x18
    br_vector3 old_v;                     // @0x24
    br_vector3 velocity_car_space;        // @0x30
    br_matrix34 oldmat;                   // @0x3c
    br_matrix34 old_frame_mat;            // @0x6c
    br_vector3 pos;                       // @0x9c
    br_vector3 omega;                     // @0xa8
    br_vector3 oldomega;                  // @0xb4
    br_scalar M;                          // @0xc0
    int infinite_mass;                    // @0xc4
    br_vector3 I;                         // @0xc8
    br_vector3 cmpos;                     // @0xd4
    int extra_point_num;                  // @0xe0
    br_bounds bounds[3];                  // @0xe4
    br_bounds max_bounds[2];              // @0x12c
    br_vector3 extra_points[6];           // @0x15c
    br_scalar original_extra_points_z[6]; // @0x1a4
    br_vector3 old_point;                 // @0x1bc
    br_vector3 old_norm;                  // @0x1c8
    int box_face_start;                   // @0x1d4
    int box_face_end;                     // @0x1d8
    int box_face_ref;                     // @0x1dc
    br_matrix34 last_box_inv_mat;         // @0x1e0
    br_bounds last_box;                   // @0x210
    int doing_nothing_flag;               // @0x228
    tSpecial_volume* last_special_volume; // @0x22c
    tSpecial_volume* auto_special_volume; // @0x230
    int frame_collision_flag;             // @0x234
    int collision_flag;                   // @0x238
    int max_shrapnel_material;            // @0x23c
    br_vector3 direction;                 // @0x240
    float speed;                          // @0x24c
    tU16 car_ID;                          // @0x250
    br_material* shrapnel_material[3];    // @0x254
    br_bounds bounds_world_space;         // @0x260
    tBounds_type bounds_ws_type;          // @0x278
    tU16 fire_vertex[12];                 // @0x27c
    tU16 num_smoke_columns;               // @0x294
    br_vector3 water_normal;              // @0x298
    br_scalar water_d;                    // @0x2a4
    br_scalar water_depth_factor;         // @0x2a8
    tNet_message_mechanics_info message;  // @0x2ac
    tU32 last_car_car_collision;          // @0x330
    br_scalar dt;                         // @0x334
    tCar_spec* who_last_hit_me;           // @0x338
} tCollision_info;

typedef struct tNon_car_spec {      // size: 0x370
    tCollision_info collision_info; // @0x0
    br_scalar free_mass;            // @0x33c
    br_scalar attached_mass;        // @0x340
    br_vector3 free_cmpos;          // @0x344
    br_vector3 attached_cmpos;      // @0x350
    br_scalar min_torque_squared;   // @0x35c
    br_scalar snap_off_cosine;      // @0x360
    br_vector3 I_over_M;            // @0x364
} tNon_car_spec;

typedef struct tOpp_spec { // size: 0x10
    int index;             // @0x0
    int ranking;           // @0x4
    int net_player_index;  // @0x8
    tCar_spec* car_spec;   // @0xc
} tOpp_spec;

typedef struct tCheckpoint {
    int time_value[3];
    int quad_count;
    br_vector3 vertices[4][4];
    br_vector3 normal[4];
    int map_left[2];
    int map_top[2];
    int map_right[2];
    int map_bottom[2];
} tCheckpoint;

typedef struct tNet_starts {
    br_vector3 pos;
    br_scalar yaw;
} tNet_starts;

typedef struct tText_chunk {
    int frame_cue;
    int frame_end;
    int x_coord;
    int y_coord;
    int line_count;
    char* text[8];
} tText_chunk;

typedef struct tRace_info {
    char name[32];
    char track_file_name[14];
    int rank_required;
    int best_rank;
    int suggested_rank;
    int total_laps;
    int check_point_count;
    int initial_timer[3];
    int bonus_score[8][3];
    int number_of_racers;
    int number_of_net_start_points;
    int text_chunk_count;
    tNet_starts net_starts[24];
    tCheckpoint checkpoints[10];
    tOpp_spec opponent_list[30];
    tU8* scene_image_data;
    tU8* map_image_data;
    tU8* info_image_data;
    tU32 scene_image_data_length;
    tU32 map_image_data_length;
    tU32 info_image_data_length;
    br_vector3 initial_position;
    br_scalar initial_yaw;
    br_pixelmap* map_image;
    br_matrix34 map_transformation;
    tText_chunk* text_chunks;
    tMaterial_modifiers material_modifiers[11];
} tRace_info;

typedef struct tOpponent_save_info {
    tU32 dead;
} tOpponent_save_info;

typedef struct tOpponent {
    char name[24];
    char abbrev_name[24];
    char mug_shot_name[14];
    char car_file_name[14];
    char stolen_car_flic_name[14];
    tU8* mug_shot_image_data;
    tU8* stolen_car_image_data;
    tU32 mug_shot_image_data_length;
    tU32 stolen_car_image_data_length;
    int car_number;
    int strength_rating;
    int picked;
    int dead;
    int text_chunk_count;
    tNet_avail network_availability;
    br_pixelmap* grid_icon_image;
    tOppo_psyche psyche;
    tText_chunk* text_chunks;
} tOpponent;

typedef struct tProgram_state {         // size: 0x356c
    tS32 credits;                       // @0x0
    tS32 credits_earned;                // @0x4
    tS32 credits_lost;                  // @0x8
    tU32 view_change_start;             // @0xc
    tU32 pratcam_move_start;            // @0x10
    int peds_killed;                    // @0x14
    int sausage_eater_mode;             // @0x18
    int rank;                           // @0x1c
    int loaded;                         // @0x20
    int last_slot;                      // @0x24
    int skill_level;                    // @0x28
    int parts_shop_visited;             // @0x2c
    int racing;                         // @0x30
    int cut_scene;                      // @0x34
    int saving;                         // @0x38
    int loading;                        // @0x3c
    int dont_save_or_load;              // @0x40
    int dont_load;                      // @0x44
    int mirror_on;                      // @0x48
    int prat_cam_on;                    // @0x4c
    int cockpit_on;                     // @0x50
    int cockpit_image_index;            // @0x54
    int current_render_left;            // @0x58
    int current_render_top;             // @0x5c
    int current_render_right;           // @0x60
    int current_render_bottom;          // @0x64
    int frame_rate_headup;              // @0x68
    int revs;                           // @0x6c
    int music_volume;                   // @0x70
    int effects_volume;                 // @0x74
    int current_race_index;             // @0x78
    int redo_race_index;                // @0x7c
    int credits_per_rank;               // @0x80
    int game_completed;                 // @0x84
    int number_of_cars;                 // @0x88
    int current_car_index;              // @0x8c
    tWhich_view which_view;             // @0x90
    tWhich_view new_view;               // @0x94
    tWhich_view pending_view;           // @0x98
    tWhich_view old_view;               // @0x9c
    tRace_sel_view_type view_type;      // @0xa0
    tProg_status prog_status;           // @0xa4
    tFrank_anne frank_or_anniness;      // @0xa8
    tAuto_parts_reply auto_parts_reply; // @0xac
    tCar_spec current_car;              // @0xb0
    char player_name[2][14];            // @0x1b4c
    char track_file_name[14];           // @0x1b68
    char car_name[14];                  // @0x1b76
    int cars_available[60];             // @0x1b84
    br_vector3 initial_position;        // @0x1c74
    br_scalar initial_yaw;              // @0x1c80
    tTrack_spec track_spec;             // @0x1c84
    tDepth_effect default_depth_effect; // @0x1cb0
    tDepth_effect current_depth_effect; // @0x1cc0
    int special_volume_count;           // @0x1cd0
    tSpecial_volume* special_volumes;   // @0x1cd4
    br_material* standard_screen;       // @0x1cd8
    br_material* standard_screen_dark;  // @0x1cdc
    br_material* standard_screen_fog;   // @0x1ce0
    int special_screens_count;          // @0x1ce4
    tSpecial_screen* special_screens;   // @0x1ce8
    tIntelligent_vehicles AI_vehicles;  // @0x1cec
    tNon_car_spec* non_cars;            // @0x3564
    int num_non_car_spaces;             // @0x3568
} tProgram_state;

typedef struct tDR_font {
    br_pixelmap* images;
    int file_read_once;
    int height;
    int width;
    int spacing;
    int offset;
    int num_entries;
    int width_table[224];
} tDR_font;

typedef struct tGraf_data {
    int width;
    int height;
    int rolling_letter_y_pitch;
    int save_slot_y_offset;
    int rolling_letter_x_pitch;
    int save_slot_x_offset;
    int save_slot_rank_x_offset;
    int save_slot_credits_x_offset;
    int save_slot_height;
    int save_slot_letter_height;
    int save_slot_table[41];
    int player_name_y;
    int player_name_x[2];
    int enter_name_x[2];
    int enter_name_y;
    int frank_panel_left;
    int frank_panel_top;
    int frank_panel_right;
    int frank_panel_bottom;
    int anne_panel_left;
    int anne_panel_top;
    int anne_panel_right;
    int anne_panel_bottom;
    int cock_margin_x;
    int cock_margin_y;
    int total_cock_width;
    int total_cock_height;
    int dial__x[2];
    int dial__y[2];
    int dial__x_centre;
    int dial__y_centre;
    int start_race_panel_left;
    int start_race_panel_top;
    int start_race_panel_right;
    int start_race_panel_bottom;
    int start_race_panel_top_clip;
    int start_race_panel_bottom_clip;
    int choose_race_rank_right;
    int choose_race_name_left;
    int choose_race_bullet_left;
    int choose_race_left;
    int choose_race_right;
    int choose_race_y_top;
    int choose_race_y_bottom;
    int choose_race_y_pitch;
    int choose_race_curr_y;
    int choose_race_box_left;
    int choose_race_box_top;
    int choose_race_line_y;
    int choose_race_current_text_x;
    int choose_race_current_text_y;
    int choose_race_current_num_x;
    int grid_x_pitch;
    int grid_y_pitch;
    int grid_x_stagger;
    int grid_left_x;
    int grid_top_y;
    int grid_left_clip;
    int grid_top_clip;
    int grid_right_clip;
    int grid_bottom_clip;
    int grid_numbers_left;
    int grid_numbers_top;
    int grid_numbers_right;
    int grid_marker_margin;
    int grid_marker_x_len;
    int dare_mugshot_left;
    int dare_mugshot_top;
    int dare_mugshot_width;
    int dare_mugshot_height;
    int dare_text_left;
    int dare_text_width;
    int dare_mug_left_margin;
    int dare_mug_top_margin;
    int dare_y_adjust;
    int parts_image_x;
    int parts_image_y;
    int parts_image_width;
    int parts_image_height;
    int parts_cost_x;
    int parts_cost_y;
    int parts_total_x;
    int parts_total_y;
    int parts_net_x;
    int parts_net_y;
    int parts_numbers_x;
    int parts_top_clip;
    int parts_bottom_clip;
    int parts_label_x;
    int parts_label_y;
    int summ1_credits_box_left;
    int summ1_credits_left;
    int summ1_credits_right;
    int summ1_earned_top;
    int summ1_earned_bottom;
    int summ1_lost_top;
    int summ1_lost_bottom;
    int summ1_total_top;
    int summ1_total_bottom;
    int summ1_rank_x_pitch;
    int summ1_rank_inc_l;
    int summ1_rank_inc_c;
    int summ1_rank_total_l;
    int summ1_rank_total_c;
    int summ1_rank_inc_left;
    int summ1_rank_inc_right;
    int summ1_rank_total_left;
    int summ1_rank_total_right;
    int summ1_rank_top;
    int summ1_rank_bot;
    int summ1_rank_y;
    int wreck_name_left;
    int wreck_name_right;
    int wreck_name_top;
    int wreck_name_bottom;
    int wreck_name_base_line;
    int wreck_render_x;
    int wreck_render_y;
    int wreck_render_w;
    int wreck_render_h;
    int power_up_icon_x;
    int power_up_icon_countdown_x;
    int power_up_icon_y;
    int power_up_icon_y_pitch;
    int power_up_icon_countdown_y_offset;
    int change_car_text_y;
    int change_car_line_left;
    int change_car_line_y;
    int change_car_line_right;
    int change_car_panel_left;
    int change_car_panel_top;
    int change_car_panel_right;
    int change_car_panel_bottom;
    int change_car_panel_top_clip;
    int change_car_panel_bottom_clip;
    int map_timer_text_x;
    int map_timer_text_y;
    int map_timer_border_x;
    int map_timer_border_y;
    int action_replay_R_x;
    int action_replay_R_y;
    int action_replay_controls_x;
    int action_replay_controls_y;
    int action_replay_hilite_y;
    int action_replay_rew_start_x;
    int action_replay_rew_x;
    int action_replay_rev_play_x;
    int action_replay_pause_x;
    int action_replay_play_x;
    int action_replay_ffwd_x;
    int action_replay_fwd_end_x;
    int action_replay_camera_x;
    int action_replay_cam_text_x;
    int action_replay_cam_text_y;
    int joinable_games_x_1;
    int joinable_games_x_2;
    int joinable_games_x_3;
    int joinable_games_x_4;
    int joinable_games_x_r;
    int joinable_games_y;
    int joinable_games_y_pitch;
    int joinable_games_sel_left;
    int joinable_games_sel_right;
    int joinable_games_sel_top_marg;
    int joinable_games_sel_bot_marg;
    int net_player_name_x;
    int net_player_name_y;
    int error_box_left;
    int error_box_top;
    int error_box_right;
    int error_box_bottom;
    int start_synch_x_0;
    int start_synch_x_1;
    int start_synch_x_2;
    int start_synch_x_r;
    int start_synch_top;
    int start_synch_y_pitch;
    int start_synch_start_x;
    int start_synch_start_y;
    int key_assign_col_1;
    int key_assign_col_1_a;
    int key_assign_col_2;
    int key_assign_col_2_a;
    int key_assign_y_pitch;
    int key_assign_y;
    int key_assign_key_map_y;
    int key_assign_flic_x;
    int key_assign_flic_y;
    int map_render_x_marg;
    int map_render_y_marg;
    int net_head_box_x;
    int net_head_box_pitch;
    int net_head_box_width;
    int net_head_box_top;
    int net_head_box_bot;
    int net_head_name_x_marg;
    int net_head_name_y;
    int net_head_num_x;
    int net_head_num_y;
    int net_head_num_height;
    int net_head_icon_x;
    int net_head_icon_y;
    int net_head_icon_height;
    int net_head_score_x;
    int net_head_score_y;
    int armour_headup_y[2];
    int power_headup_y[2];
    int offense_headup_y[2];
    int ps_dim_left;
    int ps_dim_right;
    int ps_dim_height;
    int ps_name_left;
    int ps_name_top_border;
    int ps_bars_per_level;
    int ps_x_pitch;
    int ps_bar_top_border;
    int ps_bar_left;
    int ps_bar_height;
    int net_choose_race_x;
    int net_choose_race_y;
    int net_descr_race_l;
    int net_descr_race_r;
    int net_descr_race_top;
    int net_descr_race_bot;
    int net_sum_x_1;
    int net_sum_x_2;
    int net_sum_x_3;
    int net_sum_x_4;
    int net_sum_x_5;
    int net_sum_headings_y;
    int net_sum_y_pitch;
    int graph_opt_disable_x;
    int graph_opt_disable_y;
    int sound_opt_disable_x;
    int sound_opt_disable_y;
    int net_message_enter_x;
    int net_message_enter_y;
    int eval_x;
    int eval_y;
} tGraf_data;

typedef struct tRolling_letter {
    int letters[9];
    int x_coord;
    int y_coord;
    int number_of_letters;
    tRolling_type rolling_type;
    float current_offset;
} tRolling_letter;

typedef struct tFlic_descriptor {
    char* data;
    char* data_start;
    char file_name[32];
    tU8* first_pixel;
    tU32 bytes_remaining;
    tU32 frame_period;
    tU32 last_frame;
    br_pixelmap* the_pixelmap;
    int x_offset;
    int y_offset;
    int width;
    int height;
    int frames_left;
    int current_frame;
    int the_index;
    int new_format;
    int must_finish;
    int bytes_still_to_be_read;
    int bytes_in_buffer;
    FILE* f;
    tFlic_descriptor_ptr next;
} tFlic_descriptor;

typedef struct tBrender_storage {
    int models_count;
    int materials_count;
    int shade_tables_count;
    int pixelmaps_count;
    int max_pixelmaps;
    int max_shade_tables;
    int max_materials;
    int max_models;
    br_model** models;
    br_pixelmap** pixelmaps;
    br_pixelmap** shade_tables;
    br_material** materials;
    br_pixelmap** saved_colour_maps;
} tBrender_storage;

typedef struct tSave_game {
    char slot_name[16];
    char car_name[16];
    char player_name[2][14];
    tRace_save_info race_info[100];
    tOpponent_save_info opponent_info[48];
    tU32 credits;
    tU32 rank;
    tU32 skill_level;
    tU32 game_completed;
    tU32 number_of_cars;
    tU32 cars_available[60];
    tU32 current_car_index;
    tU32 current_race_index;
    tU32 redo_race_index;
    tU32 frank_or_annitude;
    tU32 power_up_levels[3];
    tU32 version;
    tU32 checksum;
} tSave_game;

typedef struct tPed_subs {
    int orig;
    int subs;
} tPed_subs;

typedef struct tRadio_bastards {
    int count;
    int top;
    int current_value;
    int left[5];
} tRadio_bastards;

typedef struct exception_struct {
    tException_list next;
    char* name;
    int flags;
} tException_node;

typedef struct tPixelmap_user_data {
    tU16 orig_width;
    tU16 orig_height;
} tPixelmap_user_data;

typedef struct tMatrix_and_actor {
    br_matrix34* m;
    br_actor* a;
} tMatrix_and_actor;

typedef tU16 tChunk_subject_index;

typedef enum tShadow_level {
    eShadow_none = 0,
    eShadow_us_only = 1,
    eShadow_us_and_opponents = 2,
    eShadow_everyone = 3
} tShadow_level;

typedef enum tPipe_chunk_type {
    ePipe_chunk_actor_rstyle = 0,
    ePipe_chunk_actor_translate = 1,
    ePipe_chunk_actor_transform = 2,
    ePipe_chunk_actor_create = 3,
    ePipe_chunk_actor_destroy = 4,
    ePipe_chunk_actor_relink = 5,
    ePipe_chunk_actor_material = 6,
    ePipe_chunk_face_material = 7,
    ePipe_chunk_material_trans = 8,
    ePipe_chunk_material_pixelmap = 9,
    ePipe_chunk_model_geometry = 10,
    ePipe_chunk_pedestrian = 11,
    ePipe_chunk_frame_boundary = 12,
    ePipe_chunk_car = 13,
    ePipe_chunk_sound = 14,
    ePipe_chunk_damage = 15,
    ePipe_chunk_special = 16,
    ePipe_chunk_ped_gib = 17,
    ePipe_chunk_incident = 18,
    ePipe_chunk_spark = 19,
    ePipe_chunk_shrapnel = 20,
    ePipe_chunk_screen_shake = 21,
    ePipe_chunk_groove_stop = 22,
    ePipe_chunk_non_car = 23,
    ePipe_chunk_smoke = 24,
    ePipe_chunk_oil_spill = 25,
    ePipe_chunk_smoke_column = 26,
    ePipe_chunk_flame = 27,
    ePipe_chunk_smudge = 28,
    ePipe_chunk_splash = 29,
    ePipe_chunk_prox_ray = 30,
    ePipe_chunk_skid_adjustment = 31,
    ePipe_chunk_enum_count = 32
} tPipe_chunk_type;

typedef enum tSpecial_type {
    ePipe_special_fade = 0,
    ePipe_special_giant_ped_on = 1,
    ePipe_special_giant_ped_off = 2,
    ePipe_special_min_ped_on = 3,
    ePipe_special_min_ped_off = 4
} tSpecial_type;

typedef enum tIncident_type {
    eIncident_ped = 0,
    eIncident_car = 1,
    eIncident_wall = 2,
    eNo_incident = 3
} tIncident_type;

typedef enum tAction_replay_camera_type {
    eAction_replay_standard = 0,
    eAction_replay_tv = 1,
    eAction_replay_action = 2
} tAction_replay_camera_type;

typedef enum tKey_down_result {
    tKey_down_no = 0,
    tKey_down_yes = 1,
    tKey_down_still = 2,
    tKey_down_repeat = 3
} tKey_down_result;
typedef int dr_pick2d_cbfn(br_actor*, br_model*, br_material*, br_vector3*, br_vector3*, br_scalar, br_scalar, void*);
typedef struct tReduced_pos {
    tS16 v[3];
} tReduced_pos;

typedef union tIncident_info {    // size: 0x10
    struct {                      // size: 0x10
        tCar_spec* car;           // @0x0
        br_vector3 impact_point;  // @0x4
    } car_info;                   // @0x0
    struct {                      // size: 0x8
        br_actor* ped_actor;      // @0x0
        br_actor* murderer_actor; // @0x4
    } ped_info;                   // @0x0
    struct {                      // size: 0xc
        br_vector3 pos;           // @0x0
    } wall_info;                  // @0x0
} tIncident_info;

typedef struct tChanged_vertex {
    tU16 vertex_index;
    br_vector3 delta_coordinates;
} tChanged_vertex;

typedef struct tPipe_actor_rstyle_data {
    tU8 new_rstyle;
} tPipe_actor_rstyle_data;

typedef struct tPipe_actor_translate_data {
    br_vector3 new_translation;
} tPipe_actor_translate_data;

typedef struct tPipe_actor_transform_data {
    br_matrix34 new_transformation;
} tPipe_actor_transform_data;

typedef struct tPipe_actor_create_data {
    tU8 dummy;
} tPipe_actor_create_data;

typedef struct tPipe_actor_destroy_data {
    tU8 dummy;
} tPipe_actor_destroy_data;

typedef struct tPipe_actor_relink_data {
    tChunk_subject_index new_parent;
} tPipe_actor_relink_data;

typedef struct tPipe_actor_material_data {
    tChunk_subject_index new_material;
} tPipe_actor_material_data;

typedef struct tPipe_face_material_data {
    tU16 face_index;
    tChunk_subject_index new_material;
} tPipe_face_material_data;

typedef struct tPipe_material_trans_data {
    br_matrix23 new_transformation;
} tPipe_material_trans_data;

typedef struct tPipe_material_pixelmap_data {
    tChunk_subject_index new_pixelmap;
} tPipe_material_pixelmap_data;

typedef struct tPipe_model_geometry_data {
    tU16 vertex_count;
    int model_index;
    tChanged_vertex vertex_changes[1];
} tPipe_model_geometry_data;

typedef struct tSmudged_vertex {
    tU16 vertex_index;
    tU16 light_index;
} tSmudged_vertex;

typedef struct tPipe_smudge_data {
    tU16 vertex_count;
    int model_index;
    tSmudged_vertex vertex_changes[1];
} tPipe_smudge_data;

typedef struct tPipe_pedestrian_data { // size: 0x28
    tU8 action_and_frame_index;        // @0x0
    tS8 hit_points;                    // @0x1
    tU16 parent;                       // @0x2
    br_vector3 new_translation;        // @0x4
    float spin_period;                 // @0x10
    br_actor* parent_actor;            // @0x14
    br_vector3 offset;                 // @0x18
    br_scalar jump_magnitude;          // @0x24
} tPipe_pedestrian_data;

typedef struct tPipe_frame_boundary_data {
    tU32 time;
} tPipe_frame_boundary_data;

typedef struct tPipe_car_data {
    br_matrix34 transformation;
    br_vector3 velocity;
    tS16 speedo_speed;
    tS16 steering_angle;
    tU16 revs_and_gear;
    tS8 lf_sus_position;
    tS8 rf_sus_position;
    tS8 lr_sus_position;
    tS8 rr_sus_position;
} tPipe_car_data;

typedef struct tPipe_sound_data {
    tS3_pitch pitch;
    br_vector3 position;
    tU16 outlet_index;
    tU16 volume;
} tPipe_sound_data;

typedef struct tPipe_damage_data {
    tS8 damage_delta[12];
} tPipe_damage_data;

typedef struct tPipe_special_data {
    tU8 dummy;
    tS8 padding;
} tPipe_special_data;

typedef struct tPipe_ped_gib_data {
    tU16 ped_parent_index;
    tU8 size;
    tU8 gib_index;
    br_matrix34 transform;
} tPipe_ped_gib_data;

typedef struct tPipe_incident_data { // size: 0x14
    float severity;                  // @0x0
    union {                          // size: 0x10
        struct {                     // size: 0x10
            tU16 car_ID;             // @0x0
            br_vector3 impact_point; // @0x4
        } car_info;                  // @0x0
        struct {                     // size: 0x8
            tU16 ped_index;          // @0x0
            br_actor* actor;         // @0x4
        } ped_info;                  // @0x0
        struct {                     // size: 0xc
            br_vector3 pos;          // @0x0
        } wall_info;                 // @0x0
    } info;                          // @0x4
} tPipe_incident_data;

typedef struct tPipe_spark_data {
    br_vector3 pos;
    br_vector3 v;
} tPipe_spark_data;

typedef struct tPipe_shrapnel_data {
    br_vector3 pos;
    tU16 age;
    br_material* material;
} tPipe_shrapnel_data;

typedef struct tPipe_screen_shake_data {
    tS8 wobble_x;
    tS8 wobble_y;
} tPipe_screen_shake_data;

typedef struct tPipe_non_car_data {
    br_actor* actor;
    br_matrix34 mat;
} tPipe_non_car_data;

typedef struct tPipe_smoke_data {
    tReduced_pos pos;
    tU16 radius;
    tU8 strength;
    tU8 type;
} tPipe_smoke_data;

typedef struct tPipe_smoke_column_data {
    int car_ID;
    tU16 vertex;
} tPipe_smoke_column_data;

typedef struct tPipe_flame_data {
    tS16 frame_count;
    br_scalar scale_x;
    br_scalar scale_y;
    br_scalar offset_x;
    br_scalar offset_z;
} tPipe_flame_data;

typedef struct tPipe_splash_data {
    br_scalar d;
    br_vector3 normal;
} tPipe_splash_data;

typedef struct tPipe_prox_ray_data {
    tU16 ped_index;
    tU16 car_ID;
    tU32 time;
} tPipe_prox_ray_data;

typedef struct tPipe_oil_spill_data {
    br_matrix34 mat;
    br_scalar full_size;
    br_scalar grow_rate;
    tU32 spill_time;
    tU32 previous_stop_time;
    tCar_spec* car;
    br_vector3 original_pos;
    br_pixelmap* pixelmap;
} tPipe_oil_spill_data;

typedef struct tPipe_groove_stop_data {
    int path_interrupt;
    int object_interrupt;
    float path_resumption;
    float object_resumption;
    br_matrix34 matrix;
} tPipe_groove_stop_data;

typedef struct tPipe_skid_adjustment {
    br_matrix34 matrix;
    int material_index;
} tPipe_skid_adjustment;

typedef struct tPipe_chunk {            // size: 0x58
    tChunk_subject_index subject_index; // @0x0
#if defined(DETHRACE_REPLAY_DEBUG)
    int chunk_magic1;
#endif
    union {                                                  // size: 0x54
        tPipe_actor_rstyle_data actor_rstyle_data;           // @0x0
        tPipe_actor_translate_data actor_translate_data;     // @0x0
        tPipe_actor_transform_data actor_transform_data;     // @0x0
        tPipe_actor_create_data actor_create_data;           // @0x0
        tPipe_actor_destroy_data actor_destroy_data;         // @0x0
        tPipe_actor_relink_data actor_relink_data;           // @0x0
        tPipe_actor_material_data actor_material_data;       // @0x0
        tPipe_face_material_data face_material_data;         // @0x0
        tPipe_material_trans_data material_trans_data;       // @0x0
        tPipe_material_pixelmap_data material_pixelmap_data; // @0x0
        tPipe_model_geometry_data model_geometry_data;       // @0x0
        tPipe_pedestrian_data pedestrian_data;               // @0x0
        tPipe_frame_boundary_data frame_boundary_data;       // @0x0
        tPipe_car_data car_data;                             // @0x0
        tPipe_sound_data sound_data;                         // @0x0
        tPipe_damage_data damage_data;                       // @0x0
        tPipe_special_data special_data;                     // @0x0
        tPipe_ped_gib_data ped_gib_data;                     // @0x0
        tPipe_incident_data incident_data;                   // @0x0
        tPipe_spark_data spark_data;                         // @0x0
        tPipe_shrapnel_data shrapnel_data;                   // @0x0
        tPipe_screen_shake_data screen_shake_data;           // @0x0
        tPipe_groove_stop_data groove_stop_data;             // @0x0
        tPipe_non_car_data non_car_data;                     // @0x0
        tPipe_smoke_data smoke_data;                         // @0x0
        tPipe_oil_spill_data oil_data;                       // @0x0
        tPipe_smoke_column_data smoke_column_data;           // @0x0
        tPipe_flame_data flame_data;                         // @0x0
        tPipe_smudge_data smudge_data;                       // @0x0
        tPipe_splash_data splash_data;                       // @0x0
        tPipe_prox_ray_data prox_ray_data;                   // @0x0
        tPipe_skid_adjustment skid_adjustment;               // @0x0
    } chunk_data;                                            // @0x4
} tPipe_chunk;

typedef struct tPipe_session {   // size: 0x60
    tPipe_chunk_type chunk_type; // @0x0
    tU8 number_of_chunks;        // @0x4
#if defined(DETHRACE_REPLAY_DEBUG)
    int pipe_magic1;
#endif
    tPipe_chunk chunks; // @0x8
} tPipe_session;

typedef struct tCollison_data {
    int ref;
    tCollision_info* car;
} tCollison_data;

typedef struct tSave_camera {
    int saved;
    br_scalar zoom;
    br_angle yaw;
} tSave_camera;

#ifdef DETHRACE_FIX_BUGS
typedef void tEdit_func(void);
#else
typedef void* tEdit_func(void);
#endif

typedef enum tEdit_mode {
    eEdit_mode_cheat = 0,
    eEdit_mode_acc = 1,
    eEdit_mode_spec_vol = 2,
    eEdit_mode_ped = 3,
    eEdit_mode_opp = 4,
    eEdit_mode_pratcam = 5,
    eEdit_mode_depth = 6,
    eEdit_mode_damage = 7,
    eEdit_mode_bonnet = 8,
    eEdit_mode_options = 9,
    eEdit_mode_count = 10
} tEdit_mode;
typedef struct tToggle_element {
    int key1;
    int key2;
    int in_game_only;
    int exact_modifiers;
    int on_last_time;
    void (*action_proc)(void);
} tToggle_element;

typedef struct tCheat {
    tU32 code;
    tU32 code2;
    void (*action_proc)(int);
    int num;
} tCheat;

typedef enum tSpec_vol_depth_effect {
    eSpec_dep_acid = 0,
    eSpec_dep_water = 1,
    eSpec_dep_slight_fog = 2,
    eSpec_dep_med_fog = 3,
    eSpec_dep_thick_fog = 4
} tSpec_vol_depth_effect;

typedef enum tSpec_vol_cam_effect {
    eSpec_cam_wobble = 0
} tSpec_vol_cam_effect;

typedef enum tHeadup_type {
    eHeadup_unused = 0,
    eHeadup_text = 1,
    eHeadup_coloured_text = 2,
    eHeadup_image = 3,
    eHeadup_fancy = 4,
    eHeadup_box_text = 5
} tHeadup_type;

typedef enum tFancy_stage {
    eFancy_stage_incoming = 0,
    eFancy_stage_halting = 1,
    eFancy_stage_waiting = 2,
    eFancy_stage_readying = 3,
    eFancy_stage_leaving = 4
} tFancy_stage;

typedef struct tHeadup {              // size: 0x14c
    tHeadup_type type;                // @0x0
    int x;                            // @0x4
    int y;                            // @0x8
    int original_x;                   // @0xc
    int right_edge;                   // @0x10
    int flash_period;                 // @0x14
    int slot_index;                   // @0x18
    int dimmed_background;            // @0x1c
    int dim_left;                     // @0x20
    int dim_top;                      // @0x24
    int dim_right;                    // @0x28
    int dim_bottom;                   // @0x2c
    int clever;                       // @0x30
    int cockpit_anchored;             // @0x34
    int flash_state;                  // @0x38
    tJustification justification;     // @0x3c
    tU32 end_time;                    // @0x40
    tU32 last_flash;                  // @0x44
    union {                           // size: 0x104
        struct {                      // size: 0x104
            char text[250];           // @0x0
            int colour;               // @0xfc
            br_font* font;            // @0x100
        } text_info;                  // @0x0
        struct {                      // size: 0x100
            char text[250];           // @0x0
            tDR_font* coloured_font;  // @0xfc
        } coloured_text_info;         // @0x0
        struct {                      // size: 0x4
            br_pixelmap* image;       // @0x0
        } image_info;                 // @0x0
        struct {                      // size: 0x18
            br_pixelmap* image;       // @0x0
            int offset;               // @0x4
            int shear_amount;         // @0x8
            int end_offset;           // @0xc
            tFancy_stage fancy_stage; // @0x10
            tU32 start_time;          // @0x14
        } fancy_info;                 // @0x0
    } data;                           // @0x48
} tHeadup;

typedef struct tQueued_headup {
    int flash_rate;
    int lifetime;
    int font_index;
    char text[256];
} tQueued_headup;

typedef struct tFlicette {
    int flic_index;
    int x[2];
    int y[2];
} tFlicette;

typedef struct tMouse_area {
    int left[2];
    int top[2];
    int right[2];
    int bottom[2];
    int new_choice;
    int new_mode;
    int available_when_typing;
    int (*mouse_click)(int*, int*, int, int);
} tMouse_area;

typedef struct tRectile {
    int left[2];
    int top[2];
    int right[2];
    int bottom[2];
} tRectile;

typedef struct tInterface_spec {
    int initial_imode;
    int first_opening_flic;
    int second_opening_flic;
    int end_flic_go_ahead;
    int end_flic_escaped;
    int end_flic_otherwise;
    int flic_bunch_to_load;
    int move_left_new_mode[2];
    int move_left_delta[2];
    int move_left_min[2];
    int move_left_max[2];
    int (*move_left_proc[2])(int*, int*);
    int move_right_new_mode[2];
    int move_right_delta[2];
    int move_right_min[2];
    int move_right_max[2];
    int (*move_right_proc[2])(int*, int*);
    int move_up_new_mode[2];
    int move_up_delta[2];
    int move_up_min[2];
    int move_up_max[2];
    int (*move_up_proc[2])(int*, int*);
    int move_down_new_mode[2];
    int move_down_delta[2];
    int move_down_min[2];
    int move_down_max[2];
    int (*move_down_proc[2])(int*, int*);
    int go_ahead_allowed[2];
    int (*go_ahead_proc[2])(int*, int*);
    int escape_allowed[2];
    int (*escape_proc[2])(int*, int*);
    int (*exit_proc)(int*, int*);
    void (*draw_proc)(int, int);
    tU32 time_out;
    void (*start_proc1)(void);
    void (*start_proc2)(void);
    int (*done_proc)(int, int, int, int, int);
    int font_needed;
    int typeable[2];
    void (*get_original_string)(int, int, char*, int*);
    int escape_code;
    int dont_save_or_load;
    int number_of_button_flics;
    tFlicette* flicker_on_flics;
    tFlicette* flicker_off_flics;
    tFlicette* pushed_flics;
    int number_of_mouse_areas;
    tMouse_area* mouse_areas;
    int number_of_recopy_areas;
    tRectile* recopy_areas;
} tInterface_spec;

typedef int dr_modelpick2d_cbfn(br_model*, br_material*, br_vector3*, br_vector3*, br_scalar, int, int, int, br_vector3*, br_vector2*, void*);
typedef int dr_pick3d_cbfn(br_actor*, br_model*, br_material*, br_matrix34*, br_bounds*, void*);

typedef struct tFlic_spec {
    char* file_name;
    int interruptable;
    int queued;
    int frame_rate;
    int repeat;
    int x_offset;
    int y_offset;
    tS8* data_ptr;
    tU32 the_size;
} tFlic_spec;

typedef struct tFlic_bunch {
    char count;
    int* indexes;
} tFlic_bunch;

typedef struct tTranslation_record {
    int flic_index;
    int x;
    int y;
    int font_index;
    int global;
    int every_frame;
    tJustification justification;
    char* text;
} tTranslation_record;

/* Changed due to conflict with tVehicle_type enum */
typedef enum tVehicle_category {
    eVehiclecat_self = 0,
    eVehiclecat_opponent = 1,
    eVehiclecat_rozzer = 2,
    eVehiclecat_drone = 3
} tVehicle_category;

typedef struct tTransient_bm {
    br_pixelmap* pixmap;
    int in_use;
    int x_coord;
    int y_coord;
    int order_number;
    int user_data;
} tTransient_bm;

typedef struct tCursor_giblet {
    int current_giblet;
    int sequence_index;
    int landed;
    int transient_index;
    float x_speed;
    float y_speed;
    float x_coord;
    float y_coord;
    float the_speed;
    tU32 last_giblet_change;
    tU32 giblet_change_period;
    tU32 e_t_a;
} tCursor_giblet;

typedef struct tWobble_spec {
    float amplitude_x;
    float amplitude_y;
    float period;
    int time_started;
} tWobble_spec;

typedef struct tConcussion {
    int concussed;
    br_matrix34 amplitudes;
    br_matrix34 periods;
    tU32 time_started;
} tConcussion;

typedef struct tSaved_table {
    br_pixelmap* original;
    br_pixelmap* copy;
} tSaved_table;

typedef struct tClip_details {
    br_actor* clip;
    br_scalar length;
} tClip_details;

typedef enum tNet_head_avail {
    eNet_or_otherwise = 0,
    eNet_only = 1,
    eNot_net = 2,
    eNot_ever_ever_ever = 3
} tNet_head_avail;
typedef struct tModel_pool {
    br_model** model_array;
    int model_count;
} tModel_pool;

typedef struct tHeadup_info {
    char* name;
    tNet_head_avail avail;
} tHeadup_info;

typedef enum tTime_bonus_state {
    eTime_bonus_none = 0,
    eTime_bonus_initial_pause = 1,
    eTime_bonus_race_bonus = 2,
    eTime_bonus_tb_up = 3,
    eTime_bonus_tb_pause = 4,
    eTime_bonus_tb_down = 5,
    eTime_bonus_end_pause = 6
} tTime_bonus_state;

typedef enum tInfo_mode {
    eInfo_regular = 0,
    eInfo_pcam_car = 1,
    eInfo_nearby_mat = 2,
    eInfo_count = 3
} tInfo_mode;
typedef struct tHeadup_pair {
    int player_index;
    int score;
    int out_of_game;
} tHeadup_pair;

typedef struct tMax_message {
    // char buffer[516];  // 512 + sizeof(void*)
    char buffer[520];
} tMax_message;

typedef struct tMid_message {
    // char buffer[132];  // 128 + sizeof(void*)
    char buffer[136];
} tMid_message;

typedef struct tMin_message {
    // char buffer[36];   // 32 + sizeof(void*)
    char buffer[40];
} tMin_message;

typedef struct tGuaranteed_message {        // size: 0x2c
    tNet_message* message;                  // @0x0
    tU32 send_time;                         // @0x4
    tU32 next_resend_time;                  // @0x8
    tU32 resend_period;                     // @0xc
    int recieved;                           // @0x10
    tPD_net_player_info pd_address;         // @0x14
    int (*NotifyFail)(tU32, tNet_message*); // @0x24
    tU32 guarantee_number;                  // @0x28
} tGuaranteed_message;

typedef enum tJoin_or_host_result {
    eJoin_or_host_cancel = 0,
    eJoin_or_host_join = 1,
    eJoin_or_host_host = 2
} tJoin_or_host_result;
typedef struct tJoinable_game {
    tNet_game_details* game;
    tU32 time;
} tJoinable_game;

typedef struct tOil_spill_info {
    br_actor* actor;
    tCar_spec* car;
    tU32 spill_time;
    tU32 stop_time;
    br_scalar full_size;
    br_scalar current_size;
    br_scalar grow_rate;
    br_vector3 original_pos;
    br_vector3 pos;
} tOil_spill_info;

typedef enum tJoy_calib_stage {
    eJoy_calib_tl_1 = 0,
    eJoy_calib_br_1 = 1,
    eJoy_calib_cn_1 = 2,
    eJoy_calib_tl_2 = 3,
    eJoy_calib_br_2 = 4,
    eJoy_calib_cn_2 = 5
} tJoy_calib_stage;

typedef enum tPed_instruc_type {
    ePed_instruc_point = 0,
    ePed_instruc_xpoint = 1,
    ePed_instruc_bchoice = 2,
    ePed_instruc_fchoice = 3,
    ePed_instruc_dead = 4,
    ePed_instruc_bmarker = 5,
    ePed_instruc_fmarker = 6,
    ePed_instruc_baction = 7,
    ePed_instruc_faction = 8,
    ePed_instruc_reverse = 9
} tPed_instruc_type;

typedef enum tPed_frame_rate_type {
    ePed_frame_fixed = 0,
    ePed_frame_speed = 1,
    ePed_frame_variable = 2
} tPed_frame_rate_type;

typedef enum tPed_collide_type {
    ePed_collide_none = -1,
    ePed_collide_on = 0
} tPed_collide_type;

typedef enum tPed_hit_position {
    ePed_hit_unknown = 0,
    ePed_hit_front = 1,
    ePed_hit_back = 2,
    ePed_hit_rside = 3,
    ePed_hit_lside = 4
} tPed_hit_position;
typedef struct tPed_choice {
    tU16 danger_level;
    tU8 percentage_chance;
    tU8 marker_ref;
} tPed_choice;

typedef struct tPedestrian_instruction { // size: 0x14
    tPed_instruc_type type;              // @0x0
    union {                              // size: 0x10
        struct {                         // size: 0x10
            br_vector3 position;         // @0x0
            int irreversable;            // @0xc
        } point_data;                    // @0x0
        struct {                         // size: 0xc
            int number_of_choices;       // @0x0
            tPed_choice choices[2];      // @0x4
        } choice_data;                   // @0x0
        struct {                         // size: 0x4
            int death_sequence;          // @0x0
        } death_data;                    // @0x0
        struct {                         // size: 0x4
            int marker_ref;              // @0x0
        } marker_data;                   // @0x0
        struct {                         // size: 0x4
            int action_index;            // @0x0
        } action_data;                   // @0x0
    } data;                              // @0x4
} tPedestrian_instruction;

typedef struct tBearing_sequence {
    int sequence_index;
    float max_bearing;
} tBearing_sequence;

typedef struct tPedestrian_action {
    float danger_level;
    float percentage_chance;
    int number_of_bearings;
    int number_of_sounds;
    int sounds[3];
    tBearing_sequence sequences[7];
    float initial_speed;
    float looping_speed;
    tU32 reaction_time;
} tPedestrian_action;

typedef struct tPed_frame_info {
    br_vector2 offset;
    int flipped;
    br_pixelmap* pixelmap;
} tPed_frame_info;

typedef struct tPedestrian_sequence {
    tPed_frame_rate_type frame_rate_type;
    float frame_rate_factor1;
    float frame_rate_factor2;
    int number_of_frames;
    int looping_frame_start;
    tPed_frame_info frames[10];
} tPedestrian_sequence;

typedef struct tPedestrian_data {
    int magic_number;
    br_scalar height;
    br_scalar jump_magnitude;
    br_scalar car_to_ped;
    br_scalar min_x;
    br_scalar max_x;
    int credits_value;
    int hit_points;
    int number_of_exploding_sounds;
    int exploding_sounds[3];
    int falling_sound;
    int last_sound_action;
    tU16 killers_ID;
    br_scalar width;
    br_scalar height2;
    tCar_spec* fate;
    br_actor* actor;
    tPlayer_ID murderer;
    tPedestrian_instruction* instruction_list;
    tPedestrian_action* action_list;
    tU8 ref_number;
    tU8 active;
    tU8 munged;
    tU8 collided_last_time;
    tU8 flipped;
    tU8 reverse_frames;
    tU8 sent_dead_message;
    tS8 image_index;
    tS8 number_of_actions;
    tS8 number_of_instructions;
    tS8 first_instruction;
    tS8 current_instruction;
    tS8 fatal_car_impact_action;
    tS8 non_fatal_car_impact_action;
    tS8 after_impact_action;
    tS8 fatal_ground_impact_action;
    tS8 non_fatal_ground_impact_action;
    tS8 giblets_action;
    tS8 current_sequence;
    tS8 current_action;
    tS8 current_action_mode;
    tS8 current_frame;
    tS8 number_of_sequences;
    tS8 done_initial;
    tS8 giblets_being_sat_upon;
    tS8 mid_air;
    tS8 instruction_direction;
    tS8 irreversable;
    tS8 frightened_of_us;
    tS8 cloned;
    float current_speed;
    float acceleration;
    float spin_period;
    tU32 last_frame;
    tU32 last_action_change;
    tU32 last_sound_make;
    tS32 respawn_time;
    br_vector3 pos;
    br_vector3 direction;
    br_vector3 from_pos;
    br_vector3 to_pos;
    br_vector3 offset;
    float falling_speed;
    tSpecial_volume* last_special_volume;
    tPedestrian_sequence* sequences;
    tS3_sound_tag last_sound;
    br_pixelmap* colour_map;
} tPedestrian_data;

typedef struct tPed_gib {
    br_actor* actor;
    br_scalar x_speed;
    br_scalar y_speed;
    int size;
    int last_frame;
    int gib_index;
    int parent_index;
    tU32 start_time;
    tU32 end_time;
    tU32 flip_period;
} tPed_gib;

typedef struct tPed_gib_materials {
    int count;
    br_material* materials[5];
} tPed_gib_materials;

typedef struct tProximity_ray {
    tCar_spec* car;
    tPedestrian_data* ped;
    tU32 start_time;
} tProximity_ray;

typedef struct tPiped_registration_snapshot {
    int piped_actors_count;
    int piped_models_count;
    int piped_materials_count;
    int piped_pixelmaps_count;
} tPiped_registration_snapshot;

typedef struct tHeadup_icon {
    tPowerup* powerup;
    int fizzle_stage;
    int fizzle_direction;
    tU32 fizzle_start;
} tHeadup_icon;

typedef struct tPrat_flic_spec {
    char* data;
    tU32 data_length;
} tPrat_flic_spec;

typedef struct tPrat_alternative {
    int ref;
    int chance;
    int number_of_sounds;
    int sound_ids[8];
    int sound_chance;
} tPrat_alternative;

typedef struct tPrat_flic_chunk {
    int number_of_alternatives;
    tPrat_alternative alternatives[6];
} tPrat_flic_chunk;

typedef struct tPrat_sequence {
    int number_of_chunks;
    int precedence;
    int repeat_chunk;
    tPrat_flic_chunk chunks[6];
} tPrat_sequence;

typedef enum tGrid_draw {
    eGrid_draw_none = 0,
    eGrid_draw_icons_only = 1,
    eGrid_draw_all = 2
} tGrid_draw;

typedef enum tNet_synch_mode {
    eNet_synch_host_first = 0,
    eNet_synch_host_subsequent = 1,
    eNet_synch_client = 2
} tNet_synch_mode;
typedef struct tWreck_info {
    br_actor* actor;
    br_matrix34 rotation;
    br_matrix34 original_matrix;
    int customised;
    int car_index;
    float pos_x;
    float pos_y;
    tVehicle_type car_type;
    br_scalar scaling_factor;
} tWreck_info;

typedef struct tRectangle {
    int left;
    int top;
    int right;
    int bottom;
} tRectangle;

typedef struct tSkid {
    br_actor* actor;
    br_vector3 normal;
    br_vector3 pos;
} tSkid;

typedef struct tSpark {
    int count;
    br_vector3 pos;
    br_vector3 v;
    br_vector3 length;
    br_vector3 normal;
    tU32 time_sync;
    tCar_spec* car;
    tU8 colour;
} tSpark;

typedef struct tShrapnel {
    br_actor* actor;
    br_vector3 v;
    tU32 time_sync;
    tU32 age;
    br_scalar shear1;
    br_scalar shear2;
    br_vector3 axis;
} tShrapnel;

typedef struct tSmoke {
    tU32 time_sync;
    br_vector3 pos;
    br_vector3 v;
    br_scalar radius;
    br_scalar strength;
    br_scalar decay_factor;
    tU8 type;
    tU8 pipe_me;
} tSmoke;

typedef struct tSmoke_column {
    tCar_spec* car;
    tU32 time;
    tU32 lifetime;
    tU32 count;
    int colour;
    int whiter;
    br_actor* flame_actor;
    int frame_count[3];
    br_vector3 pos;
    br_scalar scale_x[3];
    br_scalar scale_y[3];
    br_scalar offset_x[3];
    br_scalar offset_z[3];
    tU32 smudge_timer;
    int vertex_index;
    int upright;

#ifdef DETHRACE_FIX_BUGS
    // Flames textures are animated per frame. At higher FPS, this happens too quickly
    br_scalar frame_time[3];
#endif
} tSmoke_column;

typedef struct tSplash {
    br_actor* actor;
    br_vector3 v;
    int just_done;
    br_scalar size;
    br_scalar scale_x;
} tSplash;

typedef struct tBRender_smoke {
    br_vector3 pos;
    br_scalar r;
    br_scalar strength;
    br_colour col;
    br_scalar aspect;
} tBRender_smoke;

typedef struct tRGB_colour {
    int red;
    int green;
    int blue;
} tRGB_colour;

#ifdef DETHRACE_FIX_BUGS
typedef br_material* tPMFMCB(br_model*, tU16);
#else
typedef br_material** tPMFMCB(br_model*, tU16);
#endif

typedef enum tMatrix_mod_type {
    eMatrix_mod_none = -1,
    eMatrix_mod_spin = 0,
    eMatrix_mod_rock = 1,
    eMatrix_mod_throb = 2,
    eMatrix_mod_slither = 3,
    eMatrix_mod_roll = 4
} tMatrix_mod_type;

typedef enum tTexture_animation_type {
    eTexture_animation_none = -1,
    eTexture_animation_frames = 0,
    eTexture_animation_flic = 1
} tTexture_animation_type;

typedef enum tMove_mode {
    eMove_none = -1,
    eMove_linear = 0,
    eMove_harmonic = 1,
    eMove_flash = 2,
    eMove_controlled = 3,
    eMove_absolute = 4,
    eMove_continuous = 5
} tMove_mode;

typedef enum tFunk_trigger_mode {
    eFunk_mode_constant = 0,
    eFunk_mode_distance = 1,
    eFunk_mode_last_lap_only = 2,
    eFunk_mode_all_laps_but_last = 3
} tFunk_trigger_mode;

typedef enum tGroove_trigger_mode {
    eGroove_mode_constant = 0,
    eGroove_mode_distance = 1
} tGroove_trigger_mode;

typedef enum tGroove_path_mode {
    eGroove_path_none = -1,
    eGroove_path_straight = 0,
    eGroove_path_circular = 1
} tGroove_path_mode;

typedef enum tGroove_object_mode {
    eGroove_object_none = -1,
    eGroove_object_spin = 0,
    eGroove_object_rock = 1,
    eGroove_object_throb = 2,
    eGroove_object_shear = 3
} tGroove_object_mode;

typedef enum tGroove_axis_mode {
    eGroove_axis_x = 0,
    eGroove_axis_y = 1,
    eGroove_axis_z = 2
} tGroove_axis_mode;

typedef enum tAnimation_time_mode {
    eTime_mode_approximate = 0,
    eTime_mode_accurate = 1
} tAnimation_time_mode;

typedef enum tInterrupt_status {
    eInterrupt_none = 0,
    eInterrupt_less_than = 1,
    eInterrupt_greater_than = 2
} tInterrupt_status;

typedef enum tRotate_mode {
    eRotate_mode_x = 0,
    eRotate_mode_y = 1,
    eRotate_mode_z = 2
} tRotate_mode;

typedef enum tScale_mode {
    eScale_mode_all = 0,
    eScale_mode_x = 1,
    eScale_mode_y = 2,
    eScale_mode_z = 3
} tScale_mode;

typedef struct tFunkotronic_spec {                  // size: 0xd8
    int owner;                                      // @0x0
    br_material* material;                          // @0x4
    tFunk_trigger_mode mode;                        // @0x8
    tMatrix_mod_type matrix_mod_type;               // @0xc
    tMove_mode matrix_mode;                         // @0x10
    union {                                         // size: 0x18
        struct {                                    // size: 0x4
            float period;                           // @0x0
        } spin_info;                                // @0x0
        struct {                                    // size: 0x10
            float period;                           // @0x0
            br_scalar x_centre;                     // @0x4
            br_scalar y_centre;                     // @0x8
            float rock_angle;                       // @0xc
        } rock_info;                                // @0x0
        struct {                                    // size: 0x18
            float x_period;                         // @0x0
            float y_period;                         // @0x4
            br_scalar x_centre;                     // @0x8
            br_scalar y_centre;                     // @0xc
            float x_magnitude;                      // @0x10
            float y_magnitude;                      // @0x14
        } throb_info;                               // @0x0
        struct {                                    // size: 0x10
            float x_period;                         // @0x0
            float y_period;                         // @0x4
            float x_magnitude;                      // @0x8
            float y_magnitude;                      // @0xc
        } slither_info;                             // @0x0
        struct {                                    // size: 0x8
            float x_period;                         // @0x0
            float y_period;                         // @0x4
        } roll_info;                                // @0x0
    } matrix_mod_data;                              // @0x14
    tMove_mode lighting_animation_type;             // @0x2c
    float lighting_animation_period;                // @0x30
    float ambient_base;                             // @0x34
    float ambient_delta;                            // @0x38
    float direct_base;                              // @0x3c
    float direct_delta;                             // @0x40
    float specular_base;                            // @0x44
    float specular_delta;                           // @0x48
    tTexture_animation_type texture_animation_type; // @0x4c
    tAnimation_time_mode time_mode;                 // @0x50
    float last_frame;                               // @0x54
    union {                                         // size: 0x78
        struct {                                    // size: 0x30
            tMove_mode mode;                        // @0x0
            float period;                           // @0x4
            int texture_count;                      // @0x8
            int current_frame;                      // @0xc
            br_pixelmap* textures[8];               // @0x10
        } frames_info;                              // @0x0
        struct {                                    // size: 0x78
            tU8* flic_data;                         // @0x0
            tU32 flic_data_length;                  // @0x4
            tFlic_descriptor flic_descriptor;       // @0x8
        } flic_info;                                // @0x0
    } texture_animation_data;                       // @0x58
    int proximity_count;                            // @0xd0
    br_vector3* proximity_array;                    // @0xd4
} tFunkotronic_spec;

typedef struct tGroovidelic_spec {             // size: 0x80
    int owner;                                 // @0x0
    int done_this_frame;                       // @0x4
    br_actor* actor;                           // @0x8
    tLollipop_mode lollipop_mode;              // @0xc
    tGroove_trigger_mode mode;                 // @0x10
    tGroove_path_mode path_type;               // @0x14
    tMove_mode path_mode;                      // @0x18
    tInterrupt_status path_interrupt_status;   // @0x1c
    float path_resumption_value;               // @0x20
    union {                                    // size: 0x1c
        struct {                               // size: 0x1c
            float period;                      // @0x0
            float x_delta;                     // @0x4
            float y_delta;                     // @0x8
            float z_delta;                     // @0xc
            br_vector3 centre;                 // @0x10
        } straight_info;                       // @0x0
        struct {                               // size: 0x18
            float period;                      // @0x0
            float radius;                      // @0x4
            br_vector3 centre;                 // @0x8
            tGroove_axis_mode axis;            // @0x14
        } circular_info;                       // @0x0
    } path_data;                               // @0x24
    br_vector3 object_centre;                  // @0x40
    br_vector3 object_position;                // @0x4c
    tGroove_object_mode object_type;           // @0x58
    tMove_mode object_mode;                    // @0x5c
    tInterrupt_status object_interrupt_status; // @0x60
    float object_resumption_value;             // @0x64
    union {                                    // size: 0x18
        struct {                               // size: 0x8
            float period;                      // @0x0
            tGroove_axis_mode axis;            // @0x4
        } spin_info;                           // @0x0
        struct {                               // size: 0x10
            float period;                      // @0x0
            float max_angle;                   // @0x4
            float current_angle;               // @0x8
            tGroove_axis_mode axis;            // @0xc
        } rock_info;                           // @0x0
        struct {                               // size: 0x18
            float x_period;                    // @0x0
            float y_period;                    // @0x4
            float z_period;                    // @0x8
            float x_magnitude;                 // @0xc
            float y_magnitude;                 // @0x10
            float z_magnitude;                 // @0x14
        } throb_info;                          // @0x0
        struct {                               // size: 0x18
            float x_period;                    // @0x0
            float y_period;                    // @0x4
            float z_period;                    // @0x8
            float x_magnitude;                 // @0xc
            float y_magnitude;                 // @0x10
            float z_magnitude;                 // @0x14
        } shear_info;                          // @0x0
    } object_data;                             // @0x68
} tGroovidelic_spec;

typedef struct tMem_info {
    unsigned int largest_block_avail;
    unsigned int max_unlocked_page;
    unsigned int largest_lockable_page;
    unsigned int lin_addr_space;
    unsigned int num_free_pages_avail;
    unsigned int num_physical_pages_free;
    unsigned int total_physical_pages;
    unsigned int free_lin_addr_space;
    unsigned int size_of_page_file;
    unsigned int reserved[3];
} tMem_info;

typedef struct tIPX_netnum {
    unsigned char bNetwork[4];
} tIPX_netnum;

#endif
