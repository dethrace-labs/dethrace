// car.c + stuff.c

#include "car.h"
#include "brender.h"
#include "brucetrk.h"
#include "car.h"
#include "constants.h"
#include "controls.h"
#include "crush.h"
#include "displays.h"
#include "finteray.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrme.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "netgame.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "pratcam.h"
#include "raycast.h"
#include "replay.h"
#include "skidmark.h"
#include "sound.h"
#include "spark.h"
#include "structur.h"
#include "trig.h"
#include "utility.h"
#include "world.h"
#include <math.h>
#include <stdlib.h>

// GLOBAL: CARM95 0x00514cb0
int gDoing_physics = 0;

// GLOBAL: CARM95 0x00514cb4
br_scalar gDt = 0.f;
// suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00514cb8
int gCollision_detection_on__car = 1;
// suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00514cc0
br_vector3 gGround_normal__car = { { 0.0f, 1.0f, 0.0f } };

// GLOBAL: CARM95 0x0000514cd0
void (*ControlCar[6])(tCar_spec*, br_scalar) = {
    &ControlCar1,
    &ControlCar2,
    &ControlCar3,
    &ControlCar4,
    &ControlCar5,
    NULL,
};

// GLOBAL: CARM95 0x00514ce8
int gControl__car = 3; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00514df8
int gFace_num__car = 1; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00514cf0
br_angle gOld_yaw__car = 0; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00514cf4
br_angle gOld_zoom = 0;

// GLOBAL: CARM95 0x00514cf8
br_vector3 gCamera_pos_before_collide = { { 0 } };

// GLOBAL: CARM95 0x00514d08
int gMetal_crunch_sound_id__car[5] = {
    // suffix added to avoid duplicate symbol
    5000,
    5001,
    5002,
    5003,
    5004,
};

// GLOBAL: CARM95 0x00514d20
int gMetal_scrape_sound_id__car[3] = {
    // suffix added to avoid duplicate symbol
    5010,
    5011,
    5012,
};

// GLOBAL: CARM95 0x00514d2c
int gCar_car_collisions = 1;

// GLOBAL: CARM95 0x00514d30
int gFreeze_mechanics = 0;

// GLOBAL: CARM95 0x00514d34
tU32 gLast_cunning_stunt = 0;

// GLOBAL: CARM95 0x00514d38
tU32 gWild_start = 0;

// GLOBAL: CARM95 0x00514d3c
tU32 gQuite_wild_start = 0;

// GLOBAL: CARM95 0x00514d40
tU32 gQuite_wild_end = 0;

// GLOBAL: CARM95 0x00514d44
tU32 gOn_me_wheels_start = 0;

// GLOBAL: CARM95 0x00514d48
int gWoz_upside_down_at_all = 0;

// GLOBAL: CARM95 0x00514d50
tS3_sound_tag gSkid_tag[2] = { 0, 0 };

// GLOBAL: CARM95 0x00514d58
tCar_spec* gLast_car_to_skid[2] = { NULL, NULL };

// GLOBAL: CARM95 0x00514d60
int gEliminate_faces = 0;

// GLOBAL: CARM95 0x00514d68
br_vector3 gZero_v__car = { { 0 } }; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00514d74
tU32 gSwitch_time = 0;

// GLOBAL: CARM95 0x00514d78
tSave_camera gSave_camera[2];

// GLOBAL: CARM95 0x00514d90
tU32 gLast_mechanics_time;

// GLOBAL: CARM95 0x00514d94
int gOpponent_viewing_mode;

// GLOBAL: CARM95 0x00514d98
int gNet_player_to_view_index = -1;

// GLOBAL: CARM95 0x00514d9c
int gDouble_pling_water = 0;

// GLOBAL: CARM95 0x00514da0
int gStop_opponents_moving = 0;

// GLOBAL: CARM95 0x00514da8
float gDefensive_powerup_factor[6] = { 1.0f, 0.825f, 0.65f, 0.475f, 0.3f, 0.01f };

// GLOBAL: CARM95 0x00514dc0
float gOffensive_powerup_factor[6] = { 1.0f, 1.5f, 2.0f, 3.0f, 5.0f, 10.0f };

// GLOBAL: CARM95 0x00514dd8
float gEngine_powerup_factor[6] = { 1.3f, 1.9f, 2.5f, 3.2f, 4.0f, 10.0f };

// GLOBAL: CARM95 0x00514df0
br_angle gPanning_camera_angle;

// GLOBAL: CARM95 0x00514df2
br_scalar gPanning_camera_height;

// GLOBAL: CARM95 0x00514cec
int gFace_count;

// GLOBAL: CARM95 0x00514e00
float gCar_simplification_factor[2][5] = {
    { 10.0f, 3.0f, 1.5f, 0.75f, 0.0f },
    { 10.0f, 5.0f, 2.5f, 1.5f, 0.0f }
};

// GLOBAL: CARM95 0x00514e28
int gCar_simplification_level = 0;

// GLOBAL: CARM95 0x00514e2c
int gNum_active_non_cars = 0;

// GLOBAL: CARM95 0x00514e30
int gCamera_has_collided = 0;

// GLOBAL: CARM95 0x0053a5b8
tFace_ref gFace_list__car[150]; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00550680
tNon_car_spec* gActive_non_car_list[50];

// GLOBAL: CARM95 0x0053a57c
int gOver_shoot;

// GLOBAL: CARM95 0x0053a574
br_scalar gMin_world_y;
br_scalar gAccel;

// GLOBAL: CARM95 0x0053a518
br_vector3 gAverage_grid_position;

// GLOBAL: CARM95 0x0053cfe8
br_actor* gPed_actor;
int gCollision_count;

// GLOBAL: CARM95 0x0053d000
int gCamera_frozen;

// GLOBAL: CARM95 0x0053a584
int gMaterial_index;

// GLOBAL: CARM95 0x0053a578
int gInTheSea;

// GLOBAL: CARM95 0x0053a524
int gCamera_mode;
br_scalar gOur_yaw__car;            // suffix added to avoid duplicate symbol
br_scalar gGravity__car;            // suffix added to avoid duplicate symbol
br_vector3 gNew_ground_normal__car; // suffix added to avoid duplicate symbol
// GLOBAL: CARM95 0x00550750
char gNon_car_spec_list[100];

// GLOBAL: CARM95 0x00550670
tU32 gMechanics_time_sync;

// GLOBAL: CARM95 0x00550748
int gNum_cars_and_non_cars;

// IDA: void __usercall DamageUnit(tCar_spec *pCar@<EAX>, int pUnit_type@<EDX>, int pDamage_amount@<EBX>)
// FUNCTION: CARM95 0x004751c0
void DamageUnit(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {
    tDamage_unit* the_damage;

    if (pDamage_amount <= 0) {
        return;
    }
    the_damage = &pCar->damage_units[pUnit_type];
    the_damage->damage_level += pDamage_amount;
    if (the_damage->damage_level >= 100) {
        the_damage->damage_level = 99;
    }
}

// IDA: void __usercall DamageUnitWithSmoke(tCar_spec *pCar@<EAX>, int pUnit_type@<EDX>, int pDamage_amount@<EBX>)
// FUNCTION: CARM95 0x00475239
void DamageUnitWithSmoke(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {

    DamageUnit(pCar, pUnit_type, pDamage_amount);
    SortOutSmoke(pCar);
}

// IDA: void __usercall DamageEngine(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x00475215
void DamageEngine(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_engine, pDamage_amount);
}

// IDA: void __usercall DamageTrans(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x00475264
void DamageTrans(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_transmission, pDamage_amount);
}

// IDA: void __usercall DamageSteering(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x00475288
void DamageSteering(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_steering, pDamage_amount);
}

// IDA: void __usercall DamageLFWheel(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x004752ac
void DamageLFWheel(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_lf_wheel, pDamage_amount);
}

// IDA: void __usercall DamageLFBrake(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x004752d0
void DamageLFBrake(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_lf_brake, pDamage_amount);
}

// IDA: void __usercall DamageLRBrake(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x004752f4
void DamageLRBrake(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_lr_brake, pDamage_amount);
}

// IDA: void __usercall DamageLRWheel(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x00475318
void DamageLRWheel(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_lr_wheel, pDamage_amount);
}

// IDA: void __usercall DamageRFWheel(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x0047533c
void DamageRFWheel(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_rf_wheel, pDamage_amount);
}

// IDA: void __usercall DamageRFBrake(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x00475360
void DamageRFBrake(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_rf_brake, pDamage_amount);
}

// IDA: void __usercall DamageRRBrake(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x00475384
void DamageRRBrake(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_rr_brake, pDamage_amount);
}

// IDA: void __usercall DamageRRWheel(int pDamage_amount@<EAX>)
// FUNCTION: CARM95 0x004753a8
void DamageRRWheel(int pDamage_amount) {

    DamageUnitWithSmoke(&gProgram_state.current_car, eDamage_rr_wheel, pDamage_amount);
}

// IDA: void __usercall CalculatePlaneNormal(br_vector3 *pP1@<EAX>, br_vector3 *pP2@<EDX>, br_vector3 *pP3@<EBX>, br_vector3 *pNormal@<ECX>)
void CalculatePlaneNormal(br_vector3* pP1, br_vector3* pP2, br_vector3* pP3, br_vector3* pNormal) {
    br_vector3 p0;
    br_vector3 p1;
    br_vector3 cross_product;
    br_vector3 temp_vector;
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CalculateGroundNormal(br_model *pThe_model@<EAX>, int pFace_index@<EDX>)
void CalculateGroundNormal(br_model* pThe_model, int pFace_index) {
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ChangeYCoordinate(br_scalar pNew_y, tU32 pTime_taken, br_model *pThe_model, int pFace_index)
void ChangeYCoordinate(br_scalar pNew_y, tU32 pTime_taken, br_model* pThe_model, int pFace_index) {
    br_scalar y_change;
    br_transform new_transform;
    br_vector3 side_window;
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SwitchCarActor(tCar_spec *pCar_spec@<EAX>, int pModel_index@<EDX>)
// FUNCTION: CARM95 0x004753cc
void SwitchCarActor(tCar_spec* pCar_spec, int pModel_index) {
    int i;

    for (i = 0; i < pCar_spec->car_actor_count; i++) {
        if (i == pModel_index) {
            pCar_spec->car_model_actors[i].actor->render_style = BR_RSTYLE_FACES;
        } else {
            pCar_spec->car_model_actors[i].actor->render_style = BR_RSTYLE_NONE;
        }
    }
    pCar_spec->current_car_actor = pModel_index;
}

// IDA: void __usercall InitialiseCar2(tCar_spec *pCar@<EAX>, int pClear_disabled_flag@<EDX>)
// FUNCTION: CARM95 0x0047544b
void InitialiseCar2(tCar_spec* pCar, int pClear_disabled_flag) {
    int index;
    int j;
    int cat;
    int car_count;
    br_actor* car_actor;
    br_angle initial_yaw;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    br_scalar speed;
    int below_face_index;
    int above_face_index;
    br_model* below_model;
    br_model* above_model;
    br_vector3 grid_offset;
    br_matrix34 initial_yaw_matrix;
    br_matrix34 safe_position;

    PossibleService();
    if (pCar->disabled && pClear_disabled_flag) {
        EnableCar(pCar);
    }
    car_actor = pCar->car_master_actor;
    InitCarSkidStuff(pCar);
    if (pCar->current_car_actor >= 0) {
        pCar->car_model_actors[pCar->current_car_actor].actor->render_style = BR_RSTYLE_NONE;
    }
    SwitchCarActor(pCar, pCar->current_car_actor);
    if (strcmp(pCar->name, "STELLA.TXT") != 0) {
        pCar->proxy_ray_distance = 0.0f;
    } else {
        pCar->proxy_ray_distance = 6.0f;
    }
    pCar->last_special_volume = NULL;
    pCar->auto_special_volume = NULL;
    pCar->num_smoke_columns = 0;
    pCar->who_last_hit_me = NULL;
    pCar->screen_material_source = NULL;
    if (pCar->screen_material != NULL) {
        pCar->screen_material->colour_map = NULL;
        pCar->screen_material->index_shade = gRender_shade_table;
        BrMaterialUpdate(pCar->screen_material, BR_MATU_ALL);
    }
    if (pCar->driver == eDriver_local_human) {
        ResetRecoveryVouchers();
    }
    BrVector3SetFloat(&pCar->v, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&pCar->omega, 0.0f, 0.0f, 0.0f);
    pCar->curvature = 0.0f;
    BrMatrix34Copy(&safe_position, &pCar->car_master_actor->t.t.mat);
    if (safe_position.m[3][0] > 500.0f) {
        safe_position.m[3][0] -= gDisabled_vector.v[0];
        safe_position.m[3][1] -= gDisabled_vector.v[1];
        safe_position.m[3][2] -= gDisabled_vector.v[2];
    }
    BrMatrix34Copy(&pCar->old_frame_mat, &safe_position);
    BrMatrix34Copy(&pCar->oldmat, &safe_position);
    BrVector3Scale((br_vector3*)pCar->oldmat.m[3], (br_vector3*)pCar->oldmat.m[3], WORLD_SCALE);
    BrMatrix34ApplyP(&pCar->pos, &pCar->cmpos, &pCar->oldmat);
    BrVector3InvScale(&pCar->pos, &pCar->pos, WORLD_SCALE);
    for (j = 0; j < COUNT_OF(pCar->oldd); j++) {
        pCar->oldd[j] = pCar->ride_height;
    }
    pCar->gear = 0;
    pCar->revs = 0.f;
    pCar->traction_control = 1;
    BrVector3Negate(&pCar->direction, (br_vector3*)&pCar->car_master_actor->t.t.mat.m[2]);
    for (j = 0; j < COUNT_OF(pCar->last_safe_positions); j++) {
        BrMatrix34Copy(&pCar->last_safe_positions[j], &safe_position);
    }
    pCar->message.type = 0;
    pCar->message.time = 0;
    pCar->dt = -1.f;
    pCar->last_car_car_collision = 1;
    pCar->time_to_recover = 0;
    pCar->repair_time = 0;

    switch (pCar->driver) {

    case eDriver_oppo:
        index = 0;
        for (j = 0; j < gCurrent_race.number_of_racers; j++) {
            if (gCurrent_race.opponent_list[j].car_spec->driver == eDriver_oppo) {
                if (gCurrent_race.opponent_list[j].car_spec == pCar) {
                    pCar->car_ID = index + 512;
                }
                index++;
            }
        }
        break;

    case eDriver_net_human:
        index = 0;
        for (j = 0; j < gCurrent_race.number_of_racers; j++) {
            if (gCurrent_race.opponent_list[j].car_spec
                && gCurrent_race.opponent_list[j].car_spec->driver == eDriver_net_human) {
                if (gCurrent_race.opponent_list[j].car_spec == pCar) {
                    pCar->car_ID = index + 256;
                }
                index++;
            }
        }
        break;

    case eDriver_local_human:
        pCar->car_ID = 0;
        break;
#ifdef DETHRACE_FIX_BUGS
    default:
        break;
#endif
    }
    PossibleService();
    pCar->box_face_ref = gFace_num__car - 2;
    pCar->doing_nothing_flag = 0;
    pCar->end_steering_damage_effect = 0;
    pCar->end_trans_damage_effect = 0;
    pCar->wheel_dam_offset[0] = 0.f;
    pCar->wheel_dam_offset[1] = 0.f;
    pCar->wheel_dam_offset[2] = 0.f;
    pCar->wheel_dam_offset[3] = 0.f;
    pCar->shadow_intersection_flags = 0;
    pCar->underwater_ability = 0;
    pCar->invulnerable = 0;
    pCar->wall_climber_mode = 0;
    pCar->grip_multiplier = 1.f;
    pCar->damage_multiplier = 1.f;
    pCar->collision_mass_multiplier = 1.f;
    pCar->engine_power_multiplier = 1.f;
    pCar->bounce_rate = 0.f;
    pCar->bounce_amount = 0.f;
    pCar->knackered = 0;
    TotallyRepairACar(pCar);
    SetCarSuspGiveAndHeight(pCar, 1.f, 1.f, 1.f, 0.f, 0.f);
    for (j = 0; j < COUNT_OF(pCar->powerups); ++j) {
        pCar->powerups[j] = 0;
    }
    if (gNet_mode != eNet_mode_none) {
        for (j = 0; j < COUNT_OF(pCar->power_up_levels); j++) {
            pCar->power_up_levels[j] = 0;
        }
    }
}

// IDA: void __usercall InitialiseCar(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x00475b63
void InitialiseCar(tCar_spec* pCar) {
    InitialiseCar2(pCar, 1);
}

// IDA: void __usercall InitialiseCarsEtc(tRace_info *pThe_race@<EAX>)
// FUNCTION: CARM95 0x00475b7c
void InitialiseCarsEtc(tRace_info* pThe_race) {
    int i;
    int cat;
    int car_count;
    tCar_spec* car;
    br_bounds bnds;

    gProgram_state.initial_position = pThe_race->initial_position;
    gProgram_state.initial_yaw = pThe_race->initial_yaw;
    BrActorToBounds(&bnds, gProgram_state.track_spec.the_actor);
    gMin_world_y = bnds.min.v[1];
    gNum_active_non_cars = 0;
    for (cat = eVehicle_self; cat <= eVehicle_not_really; cat++) {
        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (i = 0; i < car_count; i++) {
            PossibleService();
            if (cat == eVehicle_self) {
                car = &gProgram_state.current_car;
            } else {
                car = GetCarSpec(cat, i);
            }
            if (cat != eVehicle_not_really) {
                InitialiseCar(car);
            }
        }
    }
    gCamera_yaw = 0;
    InitialiseExternalCamera();
    gLast_mechanics_time = 0;
}

// IDA: void __usercall GetAverageGridPosition(tRace_info *pThe_race@<EAX>)
// FUNCTION: CARM95 0x00475ca4
void GetAverageGridPosition(tRace_info* pThe_race) {
    int i;
    br_scalar total_cars;
    tCar_spec* car;

    total_cars = 0.0f;
    BrVector3SetFloat(&gAverage_grid_position, 0.0f, 0.0f, 0.0f);
    for (i = 0; i < pThe_race->number_of_racers; i++) {
        car = pThe_race->opponent_list[i].car_spec;
        BrVector3Accumulate(&gAverage_grid_position, &car->pos);
        total_cars += 1.0f;
    }
    BrVector3InvScale(&gAverage_grid_position, &gAverage_grid_position, total_cars);
}

// IDA: void __usercall SetInitialPosition(tRace_info *pThe_race@<EAX>, int pCar_index@<EDX>, int pGrid_index@<EBX>)
// FUNCTION: CARM95 0x00475d7d
void SetInitialPosition(tRace_info* pThe_race, int pCar_index, int pGrid_index) {
    int place_on_grid;
    int i;
    int start_i;
    int j;
    br_actor* car_actor;
    br_angle initial_yaw;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    br_scalar speed;
    int below_face_index;
    int above_face_index;
    br_model* below_model;
    br_model* above_model;
    tCar_spec* car;
    br_vector3 grid_offset;
    br_vector3 dist;
    br_vector3 real_pos;
    br_matrix34 initial_yaw_matrix;
    br_bounds bnds;

#ifdef DETHRACE_FIX_BUGS
    initial_yaw = 0;
#endif
    car_actor = pThe_race->opponent_list[pCar_index].car_spec->car_master_actor;
    car = pThe_race->opponent_list[pCar_index].car_spec;
    BrMatrix34Identity(&car_actor->t.t.mat);
    place_on_grid = 1;
    if (gNet_mode != eNet_mode_none && !gCurrent_net_game->options.grid_start && pThe_race->number_of_net_start_points != 0) {
        start_i = i = IRandomBetween(0, pThe_race->number_of_net_start_points - 1);
        do {
            PossibleService();
            for (j = 0; j < gNumber_of_net_players; j++) {
                if (j != pCar_index) {
                    BrVector3Copy(&real_pos, &pThe_race->opponent_list[j].car_spec->car_master_actor->t.t.translate.t);
                    if (real_pos.v[0] > 500.f) {
                        real_pos.v[0] -= gDisabled_vector.v[0];
                        real_pos.v[1] -= gDisabled_vector.v[1];
                        real_pos.v[2] -= gDisabled_vector.v[2];
                    }
                    BrVector3Sub(&dist, &real_pos, &pThe_race->net_starts[i].pos);
                    if (BrVector3LengthSquared(&dist) < 16.f) {
                        break;
                    }
                }
            }
            if (j == gNumber_of_net_players) {
                BrVector3Copy(&car_actor->t.t.translate.t, &pThe_race->net_starts[i].pos);
                initial_yaw = BrDegreeToAngle(pThe_race->net_starts[i].yaw);
                place_on_grid = 0;
                break;
            }
            i++;
            if (i == pThe_race->number_of_net_start_points) {
                i = 0;
            }
        } while (start_i != i);
    }
    if (place_on_grid) {
        initial_yaw = BrDegreeToAngle(pThe_race->initial_yaw);
        BrMatrix34RotateY(&initial_yaw_matrix, initial_yaw);
        grid_offset.v[0] = 0.0f - pGrid_index % 2;
        grid_offset.v[1] = 0.0f;
        grid_offset.v[2] = (br_scalar)(pGrid_index / 2) * 2.0f + (br_scalar)(pGrid_index % 2) * 0.4f;
        BrMatrix34ApplyV(&car_actor->t.t.translate.t, &grid_offset, &initial_yaw_matrix);
        BrVector3Accumulate(&car_actor->t.t.translate.t, &pThe_race->initial_position);
    }
    FindBestY(
        &car_actor->t.t.translate.t,
        gTrack_actor,
        10.0f,
        &nearest_y_above,
        &nearest_y_below,
        &above_model,
        &below_model,
        &above_face_index,
        &below_face_index);
    if (nearest_y_above != 30000.0f) {
        car_actor->t.t.translate.t.v[1] = nearest_y_above;
    } else if (nearest_y_below != -30000.0f) {
        car_actor->t.t.translate.t.v[1] = nearest_y_below;
    } else {
        car_actor->t.t.translate.t.v[1] = 0.0f;
    }
    BrMatrix34PreRotateY(&car_actor->t.t.mat, initial_yaw);
    if (gNet_mode) {
        BrMatrix34Copy(
            &gNet_players[pThe_race->opponent_list[pCar_index].net_player_index].initial_position,
            &car->car_master_actor->t.t.mat);
    }
    if (gNet_mode != eNet_mode_none && car->disabled && car_actor->t.t.translate.t.v[0] < 500.0f) {
        DisableCar(car);
    }
    // Enable to start all opponent cars upside down ;)
    // if (strstr(car->name, "EAGLE") == 0) {
    //     car_actor->t.t.translate.t.v[1] += 2;
    //     car_actor->t.t.look_up.up.v[1] = -1;
    // }
}

// IDA: void __usercall SetInitialPositions(tRace_info *pThe_race@<EAX>)
// FUNCTION: CARM95 0x00476279
void SetInitialPositions(tRace_info* pThe_race) {
    int i;

    for (i = 0; i < pThe_race->number_of_racers; i++) {
        SetInitialPosition(pThe_race, i, i);
    }
}

// IDA: void __usercall InitialiseNonCar(tNon_car_spec *non_car@<EAX>)
// FUNCTION: CARM95 0x004762c1
void InitialiseNonCar(tNon_car_spec* non_car) {
    tCollision_info* c;

    c = &non_car->collision_info;
    BrMatrix34Copy(&c->oldmat, &c->car_master_actor->t.t.mat);
    c->box_face_ref = gFace_num__car - 2;
    c->doing_nothing_flag = 1;
    c->disabled = 0;
    BrVector3SetFloat(&c->v, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&c->omega, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&c->oldomega, 0.0f, 0.0f, 0.0f);
    c->box_face_ref = gFace_num__car - 2;
    c->collision_flag = 0;
    c->who_last_hit_me = NULL;
    if (c->car_master_actor->identifier[3] != '!') {
        c->M = non_car->attached_mass;
        BrVector3Copy(&c->cmpos, &non_car->attached_cmpos);
        c->min_torque_squared = non_car->min_torque_squared;
    } else {
        c->M = non_car->free_mass;
        c->min_torque_squared = 0.0f;
        BrVector3Copy(&c->cmpos, &non_car->free_cmpos);
    }
    BrVector3Scale(&c->I, &non_car->I_over_M, c->M);
    c->message.type = 0;
    c->message.time = 0;
    c->dt = -1.0f;
    c->last_car_car_collision = 1;
}

// IDA: void __usercall GetFacesInBox(tCollision_info *c@<EAX>)
// FUNCTION: CARM95 0x004764ca
void GetFacesInBox(tCollision_info* c) {
    tBounds bnds;
    br_bounds new_in_old;
    br_bounds predicted_bounds;
    br_matrix34 mat;
    br_matrix34 mat2;
    br_matrix34 mat3;
    br_matrix34 mat4;
    br_matrix34 mat5;
    br_matrix34 mat6;
    br_scalar old_d;
    int i;
    br_bounds current_bounds;

    BrMatrix34Copy(&mat, &c->car_master_actor->t.t.mat);
    BrMatrix34Copy(&mat2, &c->oldmat);
    BrVector3InvScale((br_vector3*)mat.m[3], (br_vector3*)mat.m[3], WORLD_SCALE_D);
    BrVector3InvScale((br_vector3*)mat2.m[3], (br_vector3*)mat2.m[3], WORLD_SCALE_D);
    BrMatrix34LPInverse(&mat3, &mat);
    BrMatrix34Mul(&mat4, &mat2, &mat3);
    GetNewBoundingBox(&bnds.original_bounds, c->bounds, &mat4);
    for (i = 0; i < 3; i++) {
        bnds.original_bounds.min.v[i] = MIN(bnds.original_bounds.min.v[i], c->bounds[0].min.v[i]);
        bnds.original_bounds.max.v[i] = MAX(bnds.original_bounds.max.v[i], c->bounds[0].max.v[i]);
        bnds.original_bounds.min.v[i] -= 0.002f;
        bnds.original_bounds.max.v[i] += 0.002f;
    }
    GetNewBoundingBox(&c->bounds_world_space, &bnds.original_bounds, &mat);
    c->bounds_ws_type = eBounds_ws;

    if (c->box_face_ref != gFace_num__car && (c->box_face_ref != gFace_num__car - 1 || c->box_face_start <= gFace_count)) {
        goto condition_met;
    }

    /* Second group (was comma-expression) */
    BrMatrix34Mul(&mat5, &mat, &c->last_box_inv_mat);
    GetNewBoundingBox(&new_in_old, &bnds.original_bounds, &mat5);

    if (c->last_box.max.v[0] > new_in_old.max.v[0] && c->last_box.max.v[1] > new_in_old.max.v[1] && c->last_box.max.v[2] > new_in_old.max.v[2] && c->last_box.min.v[0] < new_in_old.min.v[0] && c->last_box.min.v[1] < new_in_old.min.v[1] && c->last_box.min.v[2] < new_in_old.min.v[2]) {
        return;
    }

condition_met:

    BrMatrix34Mul(&mat5, &mat4, &mat4);
    BrMatrix34Mul(&mat6, &mat5, &mat4);
    BrMatrix34LPInverse(&mat5, &mat6);
    GetNewBoundingBox(&predicted_bounds, c->bounds, &mat5);
    for (i = 0; i < 3; i++) {
        bnds.original_bounds.min.v[i] = MIN(bnds.original_bounds.min.v[i], predicted_bounds.min.v[i]);
        bnds.original_bounds.max.v[i] = MAX(bnds.original_bounds.max.v[i], predicted_bounds.max.v[i]);

        bnds.original_bounds.min.v[i] -= 0.02f;
        bnds.original_bounds.max.v[i] += 0.02f;
    }
    c->last_box = bnds.original_bounds;
    BrMatrix34Copy(&c->last_box_inv_mat, &mat3);
    bnds.mat = &mat;
    c->box_face_start = gFace_count;
    gPling_face = NULL;
    gFace_count += FindFacesInBox(&bnds, &gFace_list__car[gFace_count], COUNT_OF(gFace_list__car) - gFace_count);
    if (gFace_count >= COUNT_OF(gFace_list__car)) {
        c->box_face_start = 0;
        gFace_count = FindFacesInBox(&bnds, gFace_list__car, COUNT_OF(gFace_list__car));
        gFace_num__car++;
    }
    old_d = c->water_d;
    if (c->driver == eDriver_local_human
        && c->water_d != 10000.f
        && gDouble_pling_water
        && BrVector3Dot(&c->bounds_world_space.max, &c->water_normal) - c->water_d <= 0.f) {
        gInTheSea = 1;
        FreezeCamera();
    }
    if (gPling_face != NULL && (float)fabs(gPling_face->normal.v[1]) > 0.9f) {
        BrVector3Copy(&c->water_normal, &gPling_face->normal);
        if (c->water_normal.v[1] < 0.f) {
            BrVector3Negate(&c->water_normal, &c->water_normal);
        }
        c->water_d = BrVector3Dot(&gPling_face->v[0], &c->water_normal);
        if (c->driver == eDriver_local_human) {
            if (gPling_face->material->identifier[1] == '!') {
                if (BrVector3Dot(&c->bounds_world_space.min, &c->water_normal) - c->water_d < 0.0f) {
                    GetNewBoundingBox(&current_bounds, &c->bounds[1], &c->car_master_actor->t.t.mat);
                    if (BrVector3Dot(&current_bounds.min, &c->water_normal) / WORLD_SCALE_D - c->water_d < 0.0) {
                        gInTheSea = 1;
                        FreezeCamera();
                    }
                }
                gDouble_pling_water = 1;
            } else {
                gDouble_pling_water = 0;
            }
        }
    } else {
        c->water_d = 10000.0;
        if (c->driver == eDriver_local_human) {
            if (gInTheSea == 1) {
                gInTheSea = 2;
            } else {
                gInTheSea = 0;
            }
        }
    }
    if (c->water_d != old_d) {
        StartPipingSession(ePipe_chunk_splash);
        AddSplashToPipingSession(c);
        EndPipingSession();
    }
    c->box_face_end = gFace_count;
    c->box_face_ref = gFace_num__car;
}

// IDA: int __cdecl IsCarInTheSea()
// FUNCTION: CARM95 0x00476cf4
int IsCarInTheSea(void) {

    return gInTheSea;
}

// IDA: void __usercall RememberSafePosition(tCar_spec *car@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x004771a2
void RememberSafePosition(tCar_spec* car, tU32 pTime) {
    // GLOBAL: CARM95 0x514e34
    static tU32 time_count;
    int j;
    br_vector3 r;
    br_scalar ts;

    if (car->disabled) {
        return;
    }
    time_count += pTime;
    if (time_count < 5000) {
        return;
    }
    time_count = 4000;
    for (j = 0; j < 4; j++) {
        if (car->susp_height[j >> 1] <= car->oldd[j]) {
            return;
        }
    }
    if ((car->last_special_volume != NULL && car->last_special_volume->gravity_multiplier != 1.0)
        || gCurrent_race.material_modifiers[car->material_index[0]].tyre_road_friction < 0.1
        || gCurrent_race.material_modifiers[car->material_index[1]].tyre_road_friction < 0.1
        || gCurrent_race.material_modifiers[car->material_index[2]].tyre_road_friction < 0.1
        || gCurrent_race.material_modifiers[car->material_index[3]].tyre_road_friction < 0.1) {
        return;
    }
    if (car->car_master_actor->t.t.mat.m[1][1] < 0.8f) {
        return;
    }
    for (j = 0; j < 5; j++) {
        BrVector3Sub(&r, &car->car_master_actor->t.t.translate.t, (br_vector3*)car->last_safe_positions[j].m[3]);

        if (BrVector3LengthSquared(&r) < 8.4015961f) {
            return;
        }
    }
    for (j = 3; j > 0; j--) {
        BrMatrix34Copy(&car->last_safe_positions[j], &car->last_safe_positions[j - 1]);
    }
    BrMatrix34Copy(&car->last_safe_positions[0], &car->car_master_actor->t.t.mat);
    time_count = 0;
}

// IDA: void __usercall ControlOurCar(tU32 pTime_difference@<EAX>)
// FUNCTION: CARM95 0x00476d09
void ControlOurCar(tU32 pTime_difference) {
    br_scalar ts;
    br_vector3 minus_k;
    tCar_spec* car;
    static int steering_locked;
    int i;
    tU32 time;

    car = &gProgram_state.current_car;
    if (gCar_flying) {
        if (!gNet_mode) {
            BrVector3Scale(&car->car_master_actor->t.t.translate.t, &car->car_master_actor->t.t.translate.t, WORLD_SCALE);
            FlyCar(car, pTime_difference / 1000.f);
            BrVector3InvScale(&car->car_master_actor->t.t.translate.t, &car->car_master_actor->t.t.translate.t, WORLD_SCALE);
        } else {
            gCar_flying = 0;
        }
    } else {
        time = GetTotalTime();
        if (car->damage_units[eDamage_steering].damage_level > 40) {
            if (car->end_steering_damage_effect) {
                if (time < car->end_steering_damage_effect || car->damage_units[eDamage_steering].damage_level == 99) {
                    car->keys.left = car->false_key_left;
                    car->keys.right = car->false_key_right;
                } else {
                    car->end_steering_damage_effect = 0;
                }
            } else if (PercentageChance(pTime_difference * (car->damage_units[eDamage_steering].damage_level - 40) * 0.0045) && fabs(car->velocity_car_space.v[2]) > 0.0001) {
                if (car->keys.left == 0 && car->keys.right == 0) {
                    if (PercentageChance(50)) {
                        car->false_key_left = 1;
                    } else {
                        car->false_key_right = 1;
                    }
                } else {
                    car->false_key_left = !car->keys.left;
                    car->false_key_right = !car->keys.right;
                }
                car->end_steering_damage_effect = FRandomBetween(0.0f, 25 * (car->damage_units[eDamage_steering].damage_level - 40)) + time;
            }
        }

        if (car->damage_units[eDamage_transmission].damage_level > 40) {
            if (car->end_trans_damage_effect) {
                if (time < car->end_trans_damage_effect || car->damage_units[eDamage_transmission].damage_level == 99) {
                    car->gear = 0;
                    car->just_changed_gear = 1;
                } else {
                    car->end_trans_damage_effect = 0;
                }
            } else {
                if (PercentageChance(pTime_difference * (car->damage_units[eDamage_transmission].damage_level - 40) * 0.006) != 0) {
                    car->end_trans_damage_effect = FRandomBetween(0.f, 50 * (car->damage_units[eDamage_transmission].damage_level - 40)) + time;
                }
            }
        }
        ControlCar[gControl__car](car, pTime_difference / 1000.0f);
        RememberSafePosition(car, pTime_difference);
        if (gCamera_reset) {
            BrVector3SetFloat(&minus_k, 0.0f, 0.0f, -1.0f);
            gCamera_sign = 0;
            BrMatrix34ApplyV(&car->direction, &minus_k, &car->car_master_actor->t.t.mat);
        }
    }
}

// IDA: void __usercall CalcEngineForce(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x00477433
void CalcEngineForce(tCar_spec* c, br_scalar dt) {
    br_scalar torque;
    br_scalar ts;
    br_scalar ts2;
    br_scalar brake_temp;
    int sign;
    tS32 temp_for_swap;

    c->acc_force = 0.0f;
    if (c->revs == 0.0f) {
        c->gear = 0;
    }

    if (c->keys.backwards != (c->gear < 0 || (c->gear == 0 && c->velocity_car_space.v[2] > 0.5))) {
        c->keys.backwards = !c->keys.backwards;

        // ye olde XOR swap
        c->keys.acc = c->keys.acc ^ c->keys.dec;
        c->keys.dec = c->keys.acc ^ c->keys.dec;
        c->keys.acc = c->keys.acc ^ c->keys.dec;

        temp_for_swap = c->joystick.acc;
        c->joystick.acc = c->joystick.dec;
        c->joystick.dec = temp_for_swap;
    }
    if (!c->gear && !c->keys.acc && c->joystick.acc <= 0 && (c->keys.dec || c->joystick.dec > 0) && !c->keys.backwards && (float)fabs(c->velocity_car_space.v[2]) < 1.0f) {
        c->keys.backwards = 1;
        c->keys.acc = c->keys.dec;
        c->keys.dec = 0;
        temp_for_swap = c->joystick.acc;
        c->joystick.acc = c->joystick.dec;
        c->joystick.dec = temp_for_swap;
    }
    c->torque = -(c->revs * c->revs / 100000000.0f) - 0.2;
    if (c->keys.acc || c->joystick.acc >= 0) {
        if (fabs(c->curvature) > c->maxcurve / 2.0f && c->gear < 2 && c->gear && c->traction_control) {
            ts = 0.7f;
        } else if (c->joystick.acc >= 0) {
            ts = c->joystick.acc / 54613.0;
        } else {
            ts = 1.2;
        }

        ts = (c->engine_power_multiplier * ts) * gEngine_powerup_factor[c->power_up_levels[1]];
        if (c->damage_units[0].damage_level > 10) {
            ts = (1.0f - (float)(c->damage_units[0].damage_level - 10) / 100.0f) * ts;
        }
        c->torque += ts;
    } else {
        c->traction_control = 1;
    }
    if (c->keys.dec || (c->keys.acc && !c->gear) || c->joystick.dec > 0 || (c->joystick.acc > 0 && !c->gear)) {
        if (c->joystick.dec > 0) {
            brake_temp = (double)(c->joystick.dec / 0x10000) * c->brake_increase;
            c->brake_force = brake_temp + c->initial_brake;
        }
        if (c->brake_force == 0.0f) {
            c->brake_force = c->initial_brake;
        } else {
            c->brake_force += c->brake_increase * dt;
            if (c->initial_brake + c->brake_increase < c->brake_force) {
                c->brake_force = c->initial_brake + c->brake_increase;
            }
        }
    } else {
        c->brake_force = 0.0f;
    }
    if (c->gear == 0) {
        return;
    }
    c->acc_force = c->force_torque_ratio * c->torque / (float)c->gear;
    if (c->gear >= 0) {
        sign = 1;
    } else {
        sign = -1;
    }
    if (c->brake_force != 0.0f) {
        c->revs = c->target_revs;
    } else {
        if (c->revs - 1.0f > c->target_revs || c->revs + 1.0f < c->target_revs) {
            ts = (c->torque * dt / 0.0002) + (c->revs - c->target_revs);
            ts2 = (1.0 / (c->speed_revs_ratio * c->M) / (float)c->gear + 1.0 / (c->force_torque_ratio * 0.0002) * (double)c->gear) * dt;
            c->acc_force += ts / ts2;
        }
    }
}

// IDA: void __usercall PrepareCars(tU32 pFrame_start_time@<EAX>)
// FUNCTION: CARM95 0x00477b11
void PrepareCars(tU32 pFrame_start_time) {
    tCar_spec* car;
    int i;
    // GLOBAL: CARM95 0x514e3c
    static tU32 last_frame_start;

    last_frame_start = pFrame_start_time;
    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        car = gActive_car_list[i];
        BrVector3Scale(&car->car_master_actor->t.t.translate.t, &car->car_master_actor->t.t.translate.t, WORLD_SCALE);
        BrVector3Scale(&car->velocity_car_space, &car->velocity_car_space, WORLD_SCALE * 1000.f);
        car->frame_collision_flag = gOver_shoot && car->collision_flag;
        if (car->driver > eDriver_non_car) {
            RecordLastDamage(car);
            if (car->driver == eDriver_oppo && gStop_opponents_moving) {
                car->acc_force = 0.0f;
                car->brake_force = 0.0f;
                car->keys.acc = 0;
                car->keys.dec = 0;
                car->joystick.acc = -1;
                car->joystick.dec = -1;
            }
            if (!car->wheel_slip) {
                StopSkid(car);
            }
            if (car->driver == eDriver_net_human && car->message.time < pFrame_start_time - 1000) {
                car->keys.acc = 0;
                car->keys.dec = 0;
                car->joystick.acc = -1;
                car->joystick.dec = -1;
                car->keys.horn = 0;
            }
            SetSmokeLastDamageLevel(car);
        }
        BrMatrix34Copy(&car->car_master_actor->t.t.mat, &car->oldmat);
    }
}

// IDA: void __usercall FinishCars(tU32 pLast_frame_time@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x00477d38
void FinishCars(tU32 pLast_frame_time, tU32 pTime) {
    tCar_spec* car;
    br_vector3 minus_k;
    int i;
    int wheel;

    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        car = gActive_car_list[i];
        if ((float)fabs(car->omega.v[0]) > 10000.f
            || (float)fabs(car->omega.v[1]) > 10000.f
            || (float)fabs(car->omega.v[2]) > 10000.f
            || (float)fabs(car->omega.v[0]) > 10000.f
            || (float)fabs(car->omega.v[1]) > 10000.f
            || (float)fabs(car->omega.v[2]) > 10000.f) {
            BrVector3SetFloat(&car->omega, 0.f, 0.f, 0.f);
            BrVector3SetFloat(&car->v, 0.f, 0.f, 0.f);
        }
        BrVector3InvScale(&car->velocity_car_space, &car->velocity_car_space, WORLD_SCALE * 1000.0f);
        BrVector3InvScale(&car->car_master_actor->t.t.translate.t, &car->car_master_actor->t.t.translate.t, WORLD_SCALE);

        car->speed = BR_LENGTH2(car->v.v[0], car->v.v[2]) / (WORLD_SCALE * 1000.0f);
        BrVector3Negate(&minus_k, (br_vector3*)car->car_master_actor->t.t.mat.m[2]);
        if (car->speed > 0.0001f) {
            if (gLast_mechanics_time > pLast_frame_time && gCar_to_view == car) {
                BrVector3Sub(&car->old_v, &car->old_v, &car->v);
#ifdef DETHRACE_FIX_BUGS
                BrVector3Scale(&car->old_v, &car->old_v, (gLast_mechanics_time - pLast_frame_time) / harness_game_config.physics_step_time);
#else
                BrVector3Scale(&car->old_v, &car->old_v, (gLast_mechanics_time - pLast_frame_time) / 40.f);
#endif
                BrVector3Accumulate(&car->old_v, &car->v);
                BrVector3Normalise(&car->direction, &car->old_v);
            } else {
                BrVector3Normalise(&car->direction, &car->v);
            }
        } else {
            if (BrVector3Dot(&car->direction, &minus_k) >= 0.f) {
                BrVector3SetFloat(&minus_k, 0.f, 0.f, -1.f);
            } else {
                BrVector3SetFloat(&minus_k, 0.f, 0.f, 1.f);
            }
            BrMatrix34ApplyV(&car->direction, &minus_k, &car->car_master_actor->t.t.mat);
        }
        if (car->driver > eDriver_non_car) {
            car->speedo_speed = BrVector3Dot(&minus_k, &car->v) / (WORLD_SCALE * 1000.0f);

            car->steering_angle = BrRadianToDegree(atan((car->wpos[0].v[2] - car->wpos[2].v[2]) * car->curvature));

            car->lr_sus_position = (car->ride_height - car->oldd[0]) / WORLD_SCALE_D;
            car->rr_sus_position = (car->ride_height - car->oldd[1]) / WORLD_SCALE_D;
            car->lf_sus_position = (car->ride_height - car->oldd[2]) / WORLD_SCALE_D;
            car->rf_sus_position = (car->ride_height - car->oldd[3]) / WORLD_SCALE_D;
            for (wheel = 0; wheel < 4; wheel++) {
                if (car->oldd[wheel] < car->susp_height[wheel >> 1] && gCurrent_race.material_modifiers[car->material_index[wheel]].smoke_type >= 2 && !car->doing_nothing_flag) {
                    GenerateContinuousSmoke(car, wheel, pTime);
                }
            }
        }
    }
}

// IDA: void __usercall InterpolateCars(tU32 pLast_frame_time@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x00478928
void InterpolateCars(tU32 pLast_frame_time, tU32 pTime) {
    br_scalar dt;
    tCar_spec* car;
    int i;

    dt = ((int)(gLast_mechanics_time - pLast_frame_time)) / 1000.0;
    if (dt > 0.04 || dt < 0)
        dt = 0;

    gOver_shoot = dt > 0.0f;

    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        car = gActive_car_list[i];
        BrMatrix34Copy(&car->oldmat, &car->car_master_actor->t.t.mat);
        SimpleRotate((tCollision_info*)car, -dt);
        TranslateCar((tCollision_info*)car, -dt);
        BrMatrix34ApplyP(&car->pos, &car->cmpos, &car->car_master_actor->t.t.mat);
        BrVector3InvScale(&car->pos, &car->pos, WORLD_SCALE);
    }
}

// IDA: void __cdecl ResetOldmat()
// FUNCTION: CARM95 0x00478a88
void ResetOldmat(void) {
    tCar_spec* car;
    int i;
    br_matrix34 mat;
    // GLOBAL: CARM95 0x514e40
    static int normalise_count = 0;

    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        car = gActive_car_list[i];
        BrMatrix34Copy(&car->oldmat, &car->car_master_actor->t.t.mat);
    }
    normalise_count++;
    if (normalise_count > 50) {
        normalise_count = 0;
        for (i = 0; i < gNum_cars_and_non_cars; i++) {
            car = gActive_car_list[i];
            BrMatrix34LPNormalise(&mat, &car->car_master_actor->t.t.mat);
            BrMatrix34Copy(&car->car_master_actor->t.t.mat, &mat);
        }
    }
}

// IDA: void __cdecl GetNonCars()
// FUNCTION: CARM95 0x00478b58
void GetNonCars(void) {
    int i;
    int j;

    gNum_cars_and_non_cars = gNum_active_cars + gNum_active_non_cars;

    for (i = gNum_active_cars, j = 0; i < gNum_cars_and_non_cars; i++, j++) {
        gActive_car_list[i] = (tCar_spec*)gActive_non_car_list[j];
    }
}

// IDA: void __usercall GetNetPos(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x00478bb7
void GetNetPos(tCar_spec* pCar) {
    int j;
    float amount;
    br_scalar total_deflection;

    if (gNet_mode == eNet_mode_host && pCar->last_car_car_collision > pCar->message.cc_coll_time) {
        pCar->message.type = 0;
        pCar->dt = -1.0f;
        return;
    }
    if ((float)fabs(pCar->message.omega.v[0]) > 10000.0f
        || (float)fabs(pCar->message.omega.v[1]) > 10000.0f
        || (float)fabs(pCar->message.omega.v[2]) > 10000.0f
        || (float)fabs(pCar->message.omega.v[0]) > 10000.0f
        || (float)fabs(pCar->message.omega.v[1]) > 10000.0f
        || (float)fabs(pCar->message.omega.v[2]) > 10000.0f) {
        BrVector3SetFloat(&pCar->message.omega, 0.0, 0.0, 0.0);
        BrVector3SetFloat(&pCar->message.v, 0.0, 0.0, 0.0);
    }
    GetExpandedMatrix(&pCar->car_master_actor->t.t.mat, &pCar->message.mat);
    if (gNet_mode == eNet_mode_client) {
        BrMatrix34Copy(&pCar->oldmat, &pCar->car_master_actor->t.t.mat);
    }
    BrVector3Copy(&pCar->v, &pCar->message.v);
    BrVector3Copy(&pCar->omega, &pCar->message.omega);

    if (pCar->driver > eDriver_non_car) {
        pCar->curvature = pCar->message.curvature * pCar->maxcurve / 32767.0f;

        for (j = 0; j < COUNT_OF(pCar->oldd); j++) {
            pCar->oldd[j] = (float)pCar->message.d[j] * pCar->susp_height[j >> 1] / 255.0f;
        }
        if (pCar->driver >= eDriver_net_human && pCar->repair_time < pCar->message.repair_time) {
            if (pCar->message.repair_time - pCar->repair_time >= 100000) {
                TotallyRepairACar(pCar);
                pCar->repair_time = pCar->message.repair_time;
            } else {
                amount = RepairCar2(pCar, pCar->message.repair_time - pCar->repair_time, &total_deflection);
            }
            for (j = 0; j < COUNT_OF(pCar->damage_units); j++) {
                pCar->damage_units[j].damage_level = pCar->message.damage[j];
            }
            SetSmokeLastDamageLevel(pCar);
            StopCarSmoking(pCar);
        } else {
            for (j = 0; j < COUNT_OF(pCar->damage_units); j++) {
                pCar->damage_units[j].damage_level = pCar->message.damage[j];
            }
            SortOutSmoke(pCar);
        }
        if (pCar->driver == eDriver_net_human) {
            pCar->revs = pCar->message.revs;
        }
        if (pCar->driver >= eDriver_net_human) {
            pCar->bounds[1].min.v[2] = pCar->message.front;
            pCar->bounds[1].max.v[2] = pCar->message.back;
        }
        if (pCar->driver != eDriver_local_human) {
            for (j = 0; j < COUNT_OF(pCar->wheel_dam_offset); j++) {
                pCar->wheel_dam_offset[j] = pCar->message.wheel_dam_offset[j];
            }
        }
        GetFacesInBox((tCollision_info*)pCar);
    }

    pCar->message.type = 0;
    pCar->last_car_car_collision = pCar->message.cc_coll_time;
}

// IDA: void __usercall ApplyPhysicsToCars(tU32 last_frame_time@<EAX>, tU32 pTime_difference@<EDX>)
// FUNCTION: CARM95 0x0047839b
void ApplyPhysicsToCars(tU32 last_frame_time, tU32 pTime_difference) {
    br_vector3 minus_k;
    int i;
    int old_num_cars;
    int step_number;
    int dam_index;
    static int steering_locked;
    tCar_spec* car;
    tCollision_info* car_info;
    tNon_car_spec* non_car;
    tU32 time_step;
    tU32 frame_end_time;

    step_number = 0;
    frame_end_time = last_frame_time + pTime_difference;
    if (gFreeze_mechanics) {
        return;
    }
    if (gNet_mode == eNet_mode_client) {
        ForceRebuildActiveCarList();
    }
    if (gLast_mechanics_time < last_frame_time) {
#ifdef DETHRACE_FIX_BUGS
        gLast_mechanics_time = harness_game_config.physics_step_time * (last_frame_time / harness_game_config.physics_step_time);
#else
        gLast_mechanics_time = 40 * (last_frame_time / 40);
#endif
    }
    GetNonCars();
    if (frame_end_time <= gLast_mechanics_time) {
        PrepareCars(last_frame_time);
        InterpolateCars(frame_end_time, pTime_difference);
        FinishCars(frame_end_time, pTime_difference);
        return;
    }

    gDoing_physics = 1;
    PrepareCars(last_frame_time);

#ifdef DETHRACE_FIX_BUGS
    time_step = harness_game_config.physics_step_time;
    gDt = harness_game_config.physics_step_time / 1000.0;
#else
    time_step = 40;
    gDt = 40 / 1000.0;
#endif

    gMechanics_time_sync = pTime_difference - (gLast_mechanics_time - last_frame_time);
    while (gLast_mechanics_time < frame_end_time && step_number < 5) {
        step_number++;
        ResetOldmat();
        BrVector3Copy(&gProgram_state.current_car.old_v, &gProgram_state.current_car.v);
        if (&gProgram_state.current_car != gCar_to_view) {
            BrVector3Copy(&gCar_to_view->old_v, &gCar_to_view->v);
        }
        for (i = 0; i < gNum_active_cars; i++) {
            car = gActive_car_list[i];
            car_info = (tCollision_info*)car;
            car->dt = -1.f;
            if (car->message.type == NETMSGID_MECHANICS && car->message.time >= gLast_mechanics_time && car->message.time <= gLast_mechanics_time + time_step) {
                // time between car message and next mechanics
                car->dt = (gLast_mechanics_time + time_step - car->message.time) / 1000.0f;
                // if the time between car message and next mechanics is about equal to timestep
                if (car->dt < gDt - 0.0001) {
                    if (gNet_mode != eNet_mode_host) {
                        for (dam_index = 0; dam_index < COUNT_OF(car->damage_units); dam_index++) {
                            if (car->damage_units[dam_index].damage_level < car->message.damage[dam_index]) {
                                car->dt = -1.f;
                                break;
                            }
                        }
                        if (car->dt >= 0.f) {
                            GetNetPos(car);
                        }
                    } else {
                        car->dt = -1.f;
                    }
                } else {
                    GetNetPos(car);
                }
            }
            if (car->disabled
                || (car->doing_nothing_flag
                    && (car->driver < eDriver_net_human
                        || (gPalette_fade_time && car->driver == eDriver_local_human)))) {
                continue;
            }
            if (car->box_face_ref != gFace_num__car
                && (car->box_face_ref != gFace_num__car - 1 || car->box_face_start <= gFace_count)) {
                GetFacesInBox(car_info);
            }
            if (car->dt != 0.f) {
                MoveAndCollideCar(car, gDt);
            }
        }
        for (i = 0; i < gNum_active_non_cars; i++) {
            non_car = gActive_non_car_list[i];
            if (non_car->collision_info.doing_nothing_flag) {
                continue;
            }
            car_info = (tCollision_info*)non_car;
            car_info->dt = -1.f;
            if (car_info->message.type == NETMSGID_NONCAR_INFO && car_info->message.time >= gLast_mechanics_time && gLast_mechanics_time + time_step >= car_info->message.time) {
                car_info->dt = (gLast_mechanics_time + time_step - car_info->message.time) / 1000.0f;
                GetNetPos((tCar_spec*)car_info);
            }
            if (car_info->box_face_ref != gFace_num__car
                && (car_info->box_face_ref != gFace_num__car - 1
                    || car_info->box_face_start <= gFace_count)) {
                GetFacesInBox(car_info);
            }
            if (car_info->dt != 0.0f) {
                MoveAndCollideNonCar(non_car, gDt);
            }
        }
        do {
            old_num_cars = gNum_cars_and_non_cars;
            CrashCarsTogether(gDt);
        } while (old_num_cars < gNum_cars_and_non_cars);
        gMechanics_time_sync -= time_step;
        gLast_mechanics_time += time_step;
    }
    gMechanics_time_sync = 1;
    SendCarData(gLast_mechanics_time);
    InterpolateCars(frame_end_time, pTime_difference);
    FinishCars(frame_end_time, pTime_difference);
    gDoing_physics = 0;
    CheckForDeAttachmentOfNonCars(pTime_difference);
}

// IDA: void __usercall MungeSpecialVolume(tCollision_info *pCar@<EAX>)
// FUNCTION: CARM95 0x004792d0
void MungeSpecialVolume(tCollision_info* pCar) {
    tSpecial_volume* new_special_volume;
    tCar_spec* car;

    new_special_volume = FindSpecialVolume(&pCar->pos, pCar->last_special_volume);
    if (pCar->auto_special_volume != NULL && (new_special_volume == NULL || new_special_volume->gravity_multiplier == 1.f)) {
        if (pCar->water_d == 10000.f && pCar->water_depth_factor != 1.f) {
            pCar->auto_special_volume = NULL;
        } else {
            new_special_volume = pCar->auto_special_volume;
        }
    }
    if (pCar->last_special_volume != new_special_volume && pCar->driver == eDriver_local_human) {
        if (pCar->last_special_volume != NULL && pCar->last_special_volume->exit_noise >= 0 && (new_special_volume == NULL || pCar->last_special_volume->exit_noise != new_special_volume->exit_noise)) {
            DRS3StartSound(gCar_outlet, pCar->last_special_volume->exit_noise);
        }
        if (new_special_volume != NULL && new_special_volume->entry_noise >= 0 && (pCar->last_special_volume == NULL || pCar->last_special_volume->entry_noise != new_special_volume->entry_noise)) {
            DRS3StartSound(gCar_outlet, new_special_volume->entry_noise);
        }
    }
    pCar->last_special_volume = new_special_volume;
    if (new_special_volume != NULL && pCar->num_smoke_columns != 0 && pCar->last_special_volume != NULL && pCar->last_special_volume->gravity_multiplier < 1.f) {
        StopCarSmoking((tCar_spec*)pCar);
    }
}

// IDA: void __usercall ResetCarSpecialVolume(tCollision_info *pCar@<EAX>)
// FUNCTION: CARM95 0x0047901e
void ResetCarSpecialVolume(tCollision_info* pCar) {
    br_vector3 cast_v;
    br_vector3 norm;
    br_scalar t;
    int id_len;
    char* mat_id;
    tSpecial_volume* new_special_volume;
    br_material* material;

    new_special_volume = NULL;
    BrVector3Set(&cast_v, 0.f, 200.f, 0.f);
    DisablePlingMaterials();
    FindFace(&pCar->car_master_actor->t.t.translate.t, &cast_v, &norm, &t, &material);
    EnablePlingMaterials();
    if (t < 100.0f && material != NULL) {
        mat_id = material->identifier;
        if (mat_id) {
            id_len = strlen(mat_id);
            if (id_len > 0 && (*mat_id == '!' || *mat_id == '#')) {
                new_special_volume = GetDefaultSpecialVolumeForWater();
            }
        }
    }
    pCar->auto_special_volume = new_special_volume;
    pCar->water_depth_factor = 1.0f;
}

// IDA: void __usercall TestAutoSpecialVolume(tCollision_info *pCar@<EAX>)
// FUNCTION: CARM95 0x004794d3
void TestAutoSpecialVolume(tCollision_info* pCar) {
    br_vector3 pos;
    br_scalar car_d;
    br_scalar d;
    br_scalar d2;
    br_vector3 dir;
    br_vector3 tv;
    br_vector3 lp;
    br_vector3 hp;
    tSpecial_volume* vol;
    br_matrix34* mat;
    br_scalar lowest_p;
    br_scalar highest_p;
    br_scalar val;
    int i;

    mat = &pCar->car_master_actor->t.t.mat;
    highest_p = 0.f;
    for (i = 0; i < 3; i++) {
        highest_p += BrVector3Dot((br_vector3*)mat->m[i], &pCar->water_normal) * pCar->bounds[0].min.v[i];
    }
    highest_p += BrVector3Dot((br_vector3*)mat->m[3], &pCar->water_normal) / WORLD_SCALE;
    lowest_p = highest_p;
    for (i = 0; i < 3; i++) {
        val = (pCar->bounds[0].max.v[i] - pCar->bounds[0].min.v[i]) * BrVector3Dot((br_vector3*)mat->m[i], &pCar->water_normal);
        if (val >= 0.f) {
            highest_p += val;
        } else {
            lowest_p += val;
        }
    }

    if (pCar->water_d > lowest_p) {
        if (pCar->water_d >= highest_p) {
            pCar->water_depth_factor = 1.f;
        } else {
            pCar->water_depth_factor = (pCar->water_d - lowest_p) / (highest_p - lowest_p);
        }
        if (pCar->auto_special_volume == NULL) {
            vol = GetDefaultSpecialVolumeForWater();
            if (vol == NULL) {
                pCar->water_depth_factor = 1.f;
                pCar->auto_special_volume = NULL;
            } else {
                BrVector3Scale(&tv, &pCar->bounds[0].min, WORLD_SCALE);
                BrMatrix34ApplyP(&lp, &tv, mat);
                BrVector3InvScale(&lp, &lp, WORLD_SCALE);
                BrVector3Copy(&hp, &lp);
                for (i = 0; i < 3; i++) {
                    val = pCar->bounds[0].max.v[i] - pCar->bounds[0].min.v[i];
                    BrVector3Scale(&tv, (br_vector3*)mat->m[i], val);
                    if (BrVector3Dot(&pCar->water_normal, &tv) > 0.f) {
                        BrVector3Accumulate(&hp, &tv);
                    } else {
                        BrVector3Accumulate(&lp, &tv);
                    }
                }
                BrVector3Sub(&dir, &hp, &lp);
                DisablePlingMaterials();
                FindFloorInBoxBU(&lp, &dir, &tv, &d, pCar);
                EnablePlingMaterials();
                FindFloorInBoxBU(&pos, &dir, &tv, &d2, pCar);
                if (d2 <= d) {
                    pCar->water_depth_factor = 1.f;
                    pCar->auto_special_volume = NULL;
                } else {
                    pCar->auto_special_volume = vol;
                }
            }
        }
    } else {
        pCar->auto_special_volume = NULL;
        pCar->water_depth_factor = 1.f;
    }
}

// IDA: void __usercall MoveAndCollideCar(tCar_spec *car@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x004790fd
void MoveAndCollideCar(tCar_spec* car, br_scalar dt) {
    tCollision_info* car_info;
    int wheel;

    if (car->dt >= 0.f) {
        dt = car->dt;
    }
    if (dt != 0.f && (!gCar_flying || &gProgram_state.current_car != car)) {
        car_info = (tCollision_info*)car;
        car->new_skidding = 0;
        if (car->water_d != 10000.0f) {
            TestAutoSpecialVolume(car_info);
        }
        MungeSpecialVolume(car_info);
        if (car->driver <= eDriver_oppo) {
            CalcForce(car, dt);
        } else {
            CalcEngineForce(car, dt);
            CalcForce(car, dt);
            DoRevs(car, dt);
        }
        RotateCar(car_info, dt);
        TranslateCar(car_info, dt);
        CollideCarWithWall(car_info, dt);
        BrMatrix34ApplyP(&car->pos, &car->cmpos, &car->car_master_actor->t.t.mat);
        BrVector3InvScale(&car->pos, &car->pos, WORLD_SCALE);
        for (wheel = 0; wheel < 4; wheel++) {
            SkidMark(car, wheel);
        }
    }
}

// IDA: void __usercall MoveAndCollideNonCar(tNon_car_spec *non_car@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x00479914
void MoveAndCollideNonCar(tNon_car_spec* non_car, br_scalar dt) {
    tCollision_info* car_info;

    car_info = &non_car->collision_info;
    if (car_info->water_d != 10000.f) {
        TestAutoSpecialVolume(&non_car->collision_info);
    }
    MungeSpecialVolume(&non_car->collision_info);
    if (car_info->dt >= 0.f) {
        dt = car_info->dt;
    }
    NonCarCalcForce(non_car, dt);
    RotateCar(&non_car->collision_info, dt);
    TranslateCar(&non_car->collision_info, dt);
    CollideCarWithWall(&non_car->collision_info, dt);
    BrMatrix34ApplyP(&car_info->pos, &car_info->cmpos, &car_info->car_master_actor->t.t.mat);
    BrVector3InvScale(&car_info->pos, &car_info->pos, WORLD_SCALE);
}

// IDA: int __usercall CollideCarWithWall@<EAX>(tCollision_info *car@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x00479a2c
int CollideCarWithWall(tCollision_info* car, br_scalar dt) {

    GetFacesInBox(car);
    if (gCollision_detection_on__car) {
        car->collision_flag = 0;
        while (CollCheck(car, dt)) {
            car->collision_flag++;
            if (car->collision_flag - 1 > 20) {
                car->collision_flag = 1;
                BrVector3Set(&car->v, 0.f, 0.f, 0.f);
                BrVector3Set(&car->omega, 0.f, 0.f, 0.f);
                break;
            }
            RotateCar(car, dt);
            TranslateCar(car, dt);
            GetFacesInBox(car);
        }
        if (car->collision_flag) {
            CrashEarnings(CAR(car), NULL);
        }
        BrMatrix34TApplyV(&car->velocity_car_space, &car->v, &car->oldmat);
        car->frame_collision_flag += car->collision_flag;
    }
    return car->collision_flag;
}

// IDA: void __cdecl ToggleControls()
// FUNCTION: CARM95 0x00479b7a
void ToggleControls(void) {

    gControl__car++;
    if (ControlCar[gControl__car] == NULL) {
        gControl__car = 0;
    }
    switch (gControl__car) {
    case 0:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -1, "Original Controls");
        break;
    case 1:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -1, "Accelerated steering");
        break;
    case 2:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -1, "0.75 Accelerated");
        break;
    case 3:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -1, "0.5 Accelerated");
        break;
    default:
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -1, "New controls");
        break;
    }
}

// IDA: void __usercall ControlCar2(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x00479c6d
void ControlCar2(tCar_spec* c, br_scalar dt) {

    c->acc_force = 0.f;
    if (c->keys.acc) {
        c->acc_force = 7.f * c->M;
    }
    if (c->keys.dec) {
        c->acc_force = -7.f * c->M;
    }
    if (c->keys.left) {
        if (c->turn_speed < 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->curvature >= 0.f) {
            c->turn_speed += dt / 0.04f * (0.05f / (5.f + BrVector3Length(&c->v)) / 2.f);
        } else {
            c->turn_speed += 0.01f * dt / 0.04f / 2.f;
        }
    }
    if (c->keys.right) {
        if (c->turn_speed > 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->curvature <= 0.f) {
            c->turn_speed -= dt / 0.04f * (0.05f / (5.f + BrVector3Length(&c->v)) / 2.f);
        } else {
            c->turn_speed -= 0.01f * dt / 0.04f / 2.f;
        }
    }
    if (!c->keys.left && !c->keys.right) {
        c->turn_speed = 0.f;
    }
    c->curvature += c->turn_speed;
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
}

// IDA: void __usercall ControlCar3(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x00479f4b
void ControlCar3(tCar_spec* c, br_scalar dt) {

    if (c->keys.left) {
        if (c->turn_speed < 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->curvature >= 0.f && c->omega.v[1] >= 0.f) {
            c->turn_speed += dt / 0.04f * (0.05f / (5.f + BrVector3Length(&c->v)) / 2.f) * 0.75f;
        } else {
            c->turn_speed += 0.01f * dt / 0.04f / 2.f * 3.f;
        }
    }

    if (c->keys.right) {
        if (c->turn_speed > 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->curvature <= 0.f && c->omega.v[1] <= 0.f) {
            c->turn_speed -= dt / 0.04f * (0.05f / (5.f + BrVector3Length(&c->v)) / 2.f) * 0.75f;
        } else {
            c->turn_speed -= 0.01f * dt / 0.04f / 2.f * 3.f;
        }
    }
    if (!c->keys.left && !c->keys.right) {
        c->turn_speed = 0.f;
    }
    c->curvature += c->turn_speed;
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
}

// IDA: void __usercall ControlCar4(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047a20e
void ControlCar4(tCar_spec* c, br_scalar dt) {
    br_scalar ts;

    if (c->keys.left) {
        if (c->turn_speed < 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->velocity_car_space.v[2] > 0.f) {
            c->turn_speed += dt * 0.01f / .04f / 2.f * 2.f;
        } else if ((c->curvature >= 0.f && c->omega.v[1] >= -.001f) || c->turn_speed != 0.f) {
            c->turn_speed += dt / .04f * (0.05f / (BrVector3Length(&c->v) + 5.f)) / 2.f * .5f;
        } else {
            c->turn_speed = dt / .04f * (.05f / (BrVector3Length(&c->v) + 5.f)) * 4.f / 2.f * .5f;
            if (c->omega.v[1] < -.01f) {
                c->turn_speed -= dt * .01f / (harness_game_config.physics_step_time / 1000.f) / 2.f * c->omega.v[1] * 2.f;
            }
        }
    }
    if (c->keys.right) {
        if (c->turn_speed > 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->velocity_car_space.v[2] > 0.f) {
            c->turn_speed -= dt * .01f / .04f / 2.f * 2.f;
        } else if ((c->curvature <= 0.f && c->omega.v[1] <= .001f) || c->turn_speed != 0.f) {
            c->turn_speed -= dt / .04f * (.05f / (BrVector3Length(&c->v) + 5.f)) / 2.f * .5f;
        } else {
            c->turn_speed = dt / .04f * (.05f / (BrVector3Length(&c->v) + 5.f)) * -4.f / 2.f * .5f;
            if (c->omega.v[1] < -.01f) {
                c->turn_speed -= dt * .01f / (harness_game_config.physics_step_time / 1000.f) / 2.f * c->omega.v[1] * 2.f;
            }
        }
    }
    if (!c->keys.left && !c->keys.right) {
        c->turn_speed = 0.f;
    } else if (fabs(c->turn_speed) < fabs(dt * 2.f * c->curvature) && c->curvature * c->turn_speed < 0.f) {
        c->turn_speed = -(dt * 2.f * c->curvature);
    }
    c->curvature += c->turn_speed;
    if (c->joystick.left > 0) {
        c->curvature = (float)c->joystick.left * (float)c->joystick.left / 4294967300.f * c->maxcurve;
    } else if (c->joystick.right >= 0) {
        c->curvature = -((float)c->joystick.right * (float)c->joystick.right / 4294967300.f) * c->maxcurve;
    }
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
}

// IDA: void __usercall ControlCar5(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047a7b1
void ControlCar5(tCar_spec* c, br_scalar dt) {

    c->acc_force = 0.f;
    if (c->keys.acc) {
        c->acc_force = 7.f * c->M;
    }
    if (c->keys.dec) {
        c->acc_force = -7.f * c->M;
    }
    if (c->keys.left) {
        if (c->turn_speed < 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->curvature >= 0) {
            c->turn_speed += dt / 0.04f * (0.05f / (5.f + BrVector3Length(&c->v)) / 2.f) * 0.5f;
        } else {
            c->turn_speed += 0.01f * dt / 0.04f / 2.f * .5f;
        }
    }
    if (c->keys.right) {
        if (c->turn_speed > 0.f) {
            c->turn_speed = 0.f;
        }
        if (c->curvature <= 0) {
            c->turn_speed -= dt / 0.04f * (0.05f / (5.f + BrVector3Length(&c->v)) / 2.f) * 0.5f;
        } else {
            c->turn_speed -= 0.01f * dt / 0.04f / 2.f * .5f;
        }
    }
    if (!c->keys.left && !c->keys.right) {
        c->turn_speed = 0.f;
        if (c->curvature < 0.f && !c->keys.holdw) {
            c->curvature += dt / 0.04f * 0.05f / (5.f + BrVector3Length(&c->v)) / 2.f * 4.f;
            if (c->curvature > 0.f) {
                c->curvature = 0.f;
            }
        } else if (c->curvature > 0.f && !c->keys.holdw) {
            c->curvature -= dt / 0.04f * 0.05f / (5.f + BrVector3Length(&c->v)) / 2.f * 4.f;
            if (c->curvature < 0.f) {
                c->curvature = 0.f;
            }
        }
    }
    c->curvature += c->turn_speed;
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
    c->keys.left = 1;
}

// IDA: void __usercall ControlCar1(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047ac31
void ControlCar1(tCar_spec* c, br_scalar dt) {

    c->acc_force = 0.f;
    if (c->keys.acc) {
        c->acc_force = 7.f * c->M;
    }
    if (c->keys.dec) {
        c->acc_force = -7.f * c->M;
    }
    if (c->keys.left) {
        if (c->curvature >= 0.f) {
            c->curvature += dt / 0.04f * 0.05f / (5.f + BrVector3Length(&c->v));
        } else {
            c->curvature += 0.01f * dt / 0.04f;
        }
    }
    if (c->keys.right) {
        if (c->curvature <= 0.f) {
            c->curvature -= dt / 0.04f * 0.05f / (5.f + BrVector3Length(&c->v));
        } else {
            c->curvature -= 0.01f * dt / 0.04f;
        }
    }
    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (c->curvature < -c->maxcurve) {
        c->curvature = -c->maxcurve;
    }
}

// IDA: void __usercall setrotate(br_vector3 *wdt@<EAX>, br_matrix34 *m@<EDX>)
// FUNCTION: CARM95 0x0047ae59
void setrotate(br_vector3* wdt, br_matrix34* m) {
    br_euler e;

    e.a = BR_ANGLE_RAD(wdt->v[0]);
    e.b = BR_ANGLE_RAD(wdt->v[1]);
    e.c = BR_ANGLE_RAD(wdt->v[2]);
    e.order = 0;
    BrEulerToMatrix34(m, &e);
}

// IDA: void __usercall RotateCar2(tCollision_info *c@<EAX>, br_scalar dt)
void RotateCar2(tCollision_info* c, br_scalar dt) {
    br_vector3 wdt;
    br_vector3 wdt2;
    br_vector3 L;
    br_vector3 L2;
    br_matrix34 m;

    BrVector3Scale(&wdt, &c->omega, dt);
    BrVector3Negate(&wdt2, &wdt);
    BrVector3Mul(&L, &c->I, &c->omega);
    setrotate(&wdt2, &m);
    BrMatrix34ApplyV(&L2, &L, &m);
    setrotate(&wdt, &m);
    BrMatrix34PreTranslate(&m, -c->cmpos.v[0], -c->cmpos.v[1], -c->cmpos.v[2]);
    BrMatrix34PostTranslate(&m, c->cmpos.v[0], c->cmpos.v[1], c->cmpos.v[2]);
    BrMatrix34Pre(&c->car_master_actor->t.t.mat, &m);
    BrVector3Copy(&c->oldomega, &c->omega);
    Vector3Div(&c->omega, &L2, &c->I);
}

// IDA: void __usercall RotateCarSecondOrder(tCollision_info *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047b434
void RotateCarSecondOrder(tCollision_info* c, br_scalar dt) {
    br_vector3 L;
    br_vector3 L2;
    br_vector3 axis;
    br_vector3 omega;
    br_scalar rad;
    br_scalar rad_rate;
    br_matrix34 m;

    rad_rate = BrVector3Length(&c->omega);
    rad = rad_rate * dt;

    BrVector3InvScale(&axis, &c->omega, rad_rate);
    BrVector3Mul(&L, &c->I, &c->omega);

    BrMatrix34Rotate(&m, BrRadianToAngle(rad) / 2, &axis);
    BrMatrix34TApplyV(&L2, &L, &m);
    omega.v[0] = L2.v[0] / c->I.v[0];
    omega.v[1] = L2.v[1] / c->I.v[1];
    omega.v[2] = L2.v[2] / c->I.v[2];

    rad_rate = BrVector3Length(&omega);
    rad = rad_rate * dt;

    BrVector3InvScale(&axis, &omega, rad_rate);
    BrMatrix34Rotate(&m, BrRadianToAngle(rad), &axis);
    BrMatrix34PreTranslate(&m, -c->cmpos.v[0], -c->cmpos.v[1], -c->cmpos.v[2]);
    BrMatrix34PostTranslate(&m, c->cmpos.v[0], c->cmpos.v[1], c->cmpos.v[2]);
    BrMatrix34Pre(&c->car_master_actor->t.t.mat, &m);
    BrMatrix34TApplyV(&L2, &L, &m);
    c->omega.v[0] = L2.v[0] / c->I.v[0];
    c->omega.v[1] = L2.v[1] / c->I.v[1];
    c->omega.v[2] = L2.v[2] / c->I.v[2];
}

// IDA: void __usercall RotateCarFirstOrder(tCollision_info *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047b073
void RotateCarFirstOrder(tCollision_info* c, br_scalar dt) {
    br_vector3 axis;
    br_vector3 L;
    br_vector3 L2;
    br_matrix34 m;
    br_scalar rad_rate;
    br_scalar rad;
    br_scalar e1;
    br_scalar e2;
    static br_scalar max_rad;

    rad_rate = BrVector3Length(&c->omega);
    rad = rad_rate * dt;

    if (rad < .0001f) {
        return;
    }
    BrVector3InvScale(&axis, &c->omega, rad_rate);
    BrVector3Mul(&L, &c->I, &c->omega);
    BrMatrix34Rotate(&m, BrRadianToAngle(rad), &axis);
    BrMatrix34TApplyV(&L2, &L, &m);
    BrMatrix34PreTranslate(&m, -c->cmpos.v[0], -c->cmpos.v[1], -c->cmpos.v[2]);
    BrMatrix34PostTranslate(&m, c->cmpos.v[0], c->cmpos.v[1], c->cmpos.v[2]);
    BrMatrix34Pre(&c->car_master_actor->t.t.mat, &m);
    c->omega.v[0] = L2.v[0] / c->I.v[0];
    c->omega.v[1] = L2.v[1] / c->I.v[1];
    c->omega.v[2] = L2.v[2] / c->I.v[2];
}

// IDA: void __usercall SimpleRotate(tCollision_info *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047b23c
void SimpleRotate(tCollision_info* c, br_scalar dt) {
    br_vector3 axis;
    br_scalar rad_rate;
    br_scalar rad;

    rad_rate = BrVector3Length(&c->omega);
    BrVector3InvScale(&axis, &c->omega, rad_rate);
    rad = rad_rate * dt;
    if (rad >= 0.0001) {
        BrMatrix34PreRotate(&c->car_master_actor->t.t.mat, BrRadianToAngle(rad), &axis);
    }
}

// IDA: void __usercall RotateCar(tCollision_info *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047b2fd
void RotateCar(tCollision_info* c, br_scalar dt) {
    br_scalar rad_squared;
    int steps;
    int i;

    rad_squared = BrVector3LengthSquared(&c->omega) * dt;
    BrVector3Copy(&c->oldomega, &c->omega);

    if (rad_squared < .0000001f) {
        return;
    }

    if (rad_squared > .008f) {
        steps = sqrt(rad_squared / .032f) + 1;
        dt = dt / steps;

        for (i = 0; i < steps && i < 20; i++) {
            RotateCarSecondOrder(c, dt);
        }
    } else {
        RotateCarFirstOrder(c, dt);
    }
}

// IDA: void __usercall SteeringSelfCentre(tCar_spec *c@<EAX>, br_scalar dt, br_vector3 *n)
// FUNCTION: CARM95 0x0047ea86
void SteeringSelfCentre(tCar_spec* c, br_scalar dt, br_vector3* n) {
    br_scalar ts;
    br_scalar ts2;

    if (c->curvature > c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (-c->maxcurve > c->curvature) {
        c->curvature = -c->maxcurve;
    }
    if (!c->keys.left && c->joystick.left <= 0 && !c->keys.right && c->joystick.right <= 0 && !c->keys.holdw) {
        if (c->susp_height[1] > c->oldd[2] || c->susp_height[1] > c->oldd[3]) {
            ts = -((c->omega.v[2] * n->v[2] + c->omega.v[1] * n->v[1] + c->omega.v[0] * n->v[0]) * (dt / (c->wpos[0].v[2] - c->wpos[2].v[2])));
            ts2 = -(c->curvature * dt);
            if (fabs(ts) < fabs(ts2) || (ts * ts2 < 0.0)) {
                ts = ts2;
            }
            c->curvature = c->curvature + ts;
            if (c->curvature * ts > 0.0) {
                c->curvature = 0.0;
            }
        }
    }
}

// IDA: void __usercall NonCarCalcForce(tNon_car_spec *nc@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047b68d
void NonCarCalcForce(tNon_car_spec* nc, br_scalar dt) {
    tCollision_info* c;
    tSpecial_volume* vol;
    br_scalar ts;
    br_vector3 tv;
    br_vector3 v;

    c = &nc->collision_info;
    vol = nc->collision_info.last_special_volume;
    if (nc->collision_info.car_master_actor->identifier[3] != '!') {
        if (c->car_master_actor->t.t.mat.m[1][1] < nc->snap_off_cosine || c->min_torque_squared == 0.0f) {
            c->car_master_actor->identifier[3] = '!';
            c->M = nc->free_mass;
            c->min_torque_squared = 0.0f;
            BrVector3Sub(&v, &nc->free_cmpos, &c->cmpos);
            BrVector3Cross(&tv, &c->omega, &v);
            BrMatrix34ApplyV(&v, &tv, &c->car_master_actor->t.t.mat);
            BrVector3Accumulate(&c->v, &v);
            c->cmpos = nc->free_cmpos;
        } else {
            BrVector3SetFloat(&c->v, 0.0f, 0.0f, 0.0f);
            ts = BrVector3LengthSquared(&c->omega);
            BrVector3SetFloat(&c->omega, 0.0f, 0.0f, 0.0f);
            c->doing_nothing_flag = 1;
        }
    }
    if (c->car_master_actor->identifier[3] == '!') {
        if (vol != NULL) {
            c->v.v[1] = c->v.v[1] - dt * 10.0f * vol->gravity_multiplier;
        } else {
            c->v.v[1] = c->v.v[1] - dt * 10.0f;
        }
        ts = BrVector3Length(&c->v);
        if (vol != NULL) {
            ts = vol->viscosity_multiplier * ts;
        }
        ts = -(dt * 0.0005f * ts) / c->M;
        BrVector3Scale(&v, &c->v, ts);
        BrVector3Accumulate(&c->v, &v);
        ts = BrVector3Length(&c->omega);
        if (vol != NULL) {
            ts = vol->viscosity_multiplier * ts;
        }
        ts = -(dt * 0.0005 * ts);
        BrVector3Scale(&v, &c->omega, ts);
        ApplyTorque(CAR(c), &v);
    }
}

// IDA: void __usercall AddDrag(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047ec63
void AddDrag(tCar_spec* c, br_scalar dt) {
    br_scalar drag_multiplier;
    br_scalar ts;
    tSpecial_volume* vol;
    br_vector3 b;

    vol = c->last_special_volume;
    drag_multiplier = -(dt * TIME_CONV_THING);
    if (vol != NULL) {
        if (c->underwater_ability) {
            drag_multiplier = vol->viscosity_multiplier * drag_multiplier * .6f;
        } else {
            drag_multiplier = vol->viscosity_multiplier * drag_multiplier;
        }
        drag_multiplier = c->water_depth_factor * drag_multiplier;
    }
    ts = BrVector3Length(&c->v) * drag_multiplier / c->M;
    BrVector3Scale(&b, &c->v, ts);
    BrVector3Accumulate(&c->v, &b);
    ts = BrVector3Length(&c->omega) * drag_multiplier;
    BrVector3Scale(&b, &c->omega, ts);
    ApplyTorque(c, &b);
}

// IDA: void __usercall DoBumpiness(tCar_spec *c@<EAX>, br_vector3 *wheel_pos@<EDX>, br_vector3 *norm@<EBX>, br_scalar *d@<ECX>, int n)
// FUNCTION: CARM95 0x0047ede9
void DoBumpiness(tCar_spec* c, br_vector3* wheel_pos, br_vector3* norm, br_scalar* d, int n) {
    br_vector3 tv;
    int delta;
    int x;
    int y;
    tMaterial_modifiers* mat_list;

    tv.v[0] = c->nor[n].v[0] * d[n] + wheel_pos[n].v[0];
    tv.v[2] = c->nor[n].v[2] * d[n] + wheel_pos[n].v[2];

    x = abs((int)(512.0f * tv.v[0])) % 2048;
    y = abs((int)(512.0f * tv.v[2])) % 2048;

    if (x > 1024) {
        x = 2048 - x;
    }
    if (y > 1024) {
        y = 2048 - y;
    }
    if (x + y <= 1024) {
        delta = x + y;
    } else {
        delta = 2048 - x - y;
    }
    delta -= 400;
    if (delta < 0) {
        delta = 0;
    }
    mat_list = gCurrent_race.material_modifiers;
    d[n] = delta * mat_list[c->material_index[n]].bumpiness / 42400.0f * norm[n].v[1] + d[n];
}

// IDA: void __usercall CalcForce(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047ba5d
void CalcForce(tCar_spec* c, br_scalar dt) {
    int n;
    int normnum;
    int i;
    int x;
    int y;
    br_scalar force[4];
    br_scalar d[4];
    br_scalar ts;
    br_scalar ts2;
    br_scalar ts3;
    br_scalar ts4;
    br_scalar ts5;
    br_scalar ts6;
    br_scalar deltaomega;
    br_scalar wheelratio;
    br_scalar modf;
    br_scalar maxfl;
    br_scalar maxfr;
    br_scalar max_retardation;
    br_scalar front_retardation;
    br_scalar friction_number;
    br_vector3 a;
    br_vector3 b;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 norm[4];
    br_vector3 v_batwick;
    br_vector3 vplane;
    br_vector3 rightplane;
    br_vector3 t;
    br_vector3 f;
    br_vector3 ff;
    br_vector3 fb;
    br_scalar ffi;
    br_scalar ffk;
    br_scalar fbi;
    br_scalar fbk;
    br_vector3 wheel_pos[4];
    br_scalar direction;
    br_scalar wheel_spin_force;
    br_scalar d_damage;
    br_scalar fl_oil_factor;
    br_scalar fr_oil_factor;
    br_scalar rl_oil_factor;
    br_scalar rr_oil_factor;
    br_matrix34* mat;
    tMaterial_modifiers* mat_list;
    // GLOBAL: CARM95 0x514e4c
    static br_scalar stop_timer;
    // GLOBAL: CARM95 0x514e50
    static br_scalar slide_dist;
    tDamage_type dam;
    br_scalar v;
    tSpecial_volume* vol;
    br_scalar scale;

    int v72;         // [esp+24h] [ebp-1C8h]
    double v73;      // [esp+2Ch] [ebp-1C0h]
    float v74;       // [esp+34h] [ebp-1B8h]
    float v75;       // [esp+38h] [ebp-1B4h]
    float v76;       // [esp+3Ch] [ebp-1B0h]
    float v77;       // [esp+40h] [ebp-1ACh]
    float v78;       // [esp+44h] [ebp-1A8h]
    float v79;       // [esp+48h] [ebp-1A4h]
    float v80;       // [esp+4Ch] [ebp-1A0h] MAPDST
    float v82;       // [esp+50h] [ebp-19Ch] MAPDST
    float v84;       // [esp+54h] [ebp-198h]
    float v85;       // [esp+58h] [ebp-194h] MAPDST
    float v87;       // [esp+5Ch] [ebp-190h] MAPDST
    float v98;       // [esp+88h] [ebp-164h]
    float v99;       // [esp+8Ch] [ebp-160h]
    br_vector3 v102; // [esp+98h] [ebp-154h]
    br_vector3 v103; // [esp+A4h] [ebp-148h]
    int v105;        // [esp+B8h] [ebp-134h]
    float v106;      // [esp+C0h] [ebp-12Ch]
    br_vector3 v107; // [esp+C4h] [ebp-128h]
    float v108;      // [esp+D0h] [ebp-11Ch]
    float v109;      // [esp+D4h] [ebp-118h]
    float v116;      // [esp+F8h] [ebp-F4h]
    br_vector3 B;    // [esp+FCh] [ebp-F0h] BYREF
    br_scalar pV;    // [esp+10Ch] [ebp-E0h]
    br_vector3 v123; // [esp+130h] [ebp-BCh]
    float v125;      // [esp+16Ch] [ebp-80h]
    float v128;      // [esp+18Ch] [ebp-60h]
    float v129;      // [esp+190h] [ebp-5Ch]
    float v134;      // [esp+1D8h] [ebp-14h]
    float v135;      // [esp+1DCh] [ebp-10h]
    br_vector3 v136; // [esp+1E0h] [ebp-Ch]

    BrVector3Set(&v136, 0, 0, 0);
    normnum = 0;
    BrVector3Set(&f, 0, 0, 0);
    BrVector3Set(&B, 0, 0, 0);
    mat = &c->car_master_actor->t.t.mat;
    mat_list = gCurrent_race.material_modifiers;
    vol = c->last_special_volume;
    b.v[0] = -mat->m[1][0];
    b.v[1] = -mat->m[1][1];
    b.v[2] = -mat->m[1][2];
    c->material_index[0] = 0;
    c->material_index[1] = 0;
    c->material_index[2] = 0;
    c->material_index[3] = 0;
    wheelratio = (c->wpos[2].v[2] - c->cmpos.v[2]) / (c->wpos[0].v[2] - c->cmpos.v[2]);
    BrVector3Set(&c->road_normal, 0, 0, 0);
    for (i = 0; i < 4; ++i) {
        BrMatrix34ApplyP(&wheel_pos[i], &c->wpos[i], mat);
    }
    MultiFindFloorInBoxM(4, wheel_pos, &b, c->nor, d, c, c->material_index);
    if (c->last_special_volume && c->last_special_volume->material_modifier_index) {
        c->material_index[0] = c->last_special_volume->material_modifier_index;
        c->material_index[1] = c->material_index[0];
        c->material_index[2] = c->material_index[1];
        c->material_index[3] = c->material_index[2];
    }
    for (i = 0; i < 4; ++i) {
        BrMatrix34TApplyV(&norm[i], &c->nor[i], mat);
        if (mat_list[c->material_index[i]].bumpiness != 0.0) {
            DoBumpiness(c, wheel_pos, norm, d, i);
        }
        if (d[i] < -0.5 || c->wheel_dam_offset[i ^ 2] * 6.9 + c->susp_height[i / 2] < d[i]) {
            force[i] = 0.0;
            d[i] = c->susp_height[i / 2];
        } else {
            BrVector3Accumulate(&c->road_normal, &norm[i]);
            normnum++;
            d[i] = d[i] - c->wheel_dam_offset[i ^ 2] * 6.9;
            force[i] = (c->susp_height[i / 2] - d[i]) * c->sk[1 / 2];
            force[i] = force[i] - (d[i] - c->oldd[i]) / dt * c->sb[i / 2];
            if (c->susp_height[i / 2] == c->oldd[i]
                && c->nor[i].v[2] * c->v.v[2] + c->nor[i].v[1] * c->v.v[1] + c->nor[i].v[0] * c->v.v[0] > -0.0099999998
                && c->M * 20.0 / 4.0 < force[i]) {
                d[i] = c->susp_height[i / 2];
                force[i] = c->M * 20.0 / 4.0;
            }
            if (force[i] < 0.0) {
                force[i] = 0.0;
            }
            B.v[1] = force[i] + B.v[1];
            f.v[0] = f.v[0] - (c->wpos[i].v[2] - c->cmpos.v[2]) * force[i];
            f.v[2] = (c->wpos[i].v[0] - c->cmpos.v[0]) * force[i] + f.v[2];
        }
        c->oldd[i] = d[i];
    }
    if (c->driver <= eDriver_non_car || !c->wall_climber_mode || (c->road_normal.v[0] == 0.0 && c->road_normal.v[1] == 0.0 && c->road_normal.v[2] == 0.0)) {
        if (vol) {
            friction_number = (1.0 - vol->gravity_multiplier) * c->water_depth_factor;
            if (c->underwater_ability) {
                friction_number = friction_number * 0.6;
            }
            friction_number = (1.0 - friction_number) * c->M;
        } else {
            friction_number = c->M;
        }
        friction_number = friction_number * gGravity_multiplier * 10.0;
        B.v[0] = B.v[0] - mat->m[0][1] * friction_number;
        B.v[1] = B.v[1] - mat->m[1][1] * friction_number;
        B.v[2] = B.v[2] - mat->m[2][1] * friction_number;
    } else {
        BrVector3Normalise(&v107, &c->road_normal);
        BrVector3Scale(&v107, &v107, -(c->M * 10.0));
        BrVector3Accumulate(&B, &v107);
    }
    if (c->driver >= eDriver_net_human) {
        SteeringSelfCentre(c, dt, &c->road_normal);
    }
    if (normnum) {
        // ts = 1.0 / sqrt(1.0); <- looked like this in the windows build definitely wrong
        // ts = 1.0 / sqrt(c->road_normal.v[0] * c->road_normal.v[0] + c->road_normal.v[1] * c->road_normal.v[1] + c->road_normal.v[2] * c->road_normal.v[2]);
        // c->road_normal.v[0] = c->road_normal.v[0] * ts;
        // c->road_normal.v[1] = c->road_normal.v[1] * ts;
        // c->road_normal.v[2] = c->road_normal.v[2] * ts;
        BrVector3NormaliseQuick(&c->road_normal, &c->road_normal);

        friction_number = c->road_normal.v[1] * mat->m[1][1] + c->road_normal.v[2] * mat->m[2][1] + c->road_normal.v[0] * mat->m[0][1];
        if (c->driver > eDriver_non_car && c->wall_climber_mode) {
            friction_number = 1.0;
        }
        friction_number = gCurrent_race.material_modifiers[c->material_index[0]].down_force * friction_number;
        if (friction_number > 0.0f) {
            friction_number = fabs(c->velocity_car_space.v[2]) * c->M * 10.0 * friction_number / c->down_force_speed;
            if (c->M * 10.0 < friction_number) {
                friction_number = c->M * 10.0;
            }
            if (c->number_of_wheels_on_ground == 4 && c->oldd[2] == c->susp_height[1] && c->oldd[3] == c->susp_height[1]) {
                a.v[0] = c->wpos[2].v[2] * mat->m[2][0];
                a.v[1] = c->wpos[2].v[2] * mat->m[2][1];
                a.v[2] = c->wpos[2].v[2] * mat->m[2][2];
                a.v[0] = mat->m[3][0] + a.v[0];
                a.v[1] = mat->m[3][1] + a.v[1];
                a.v[2] = mat->m[3][2] + a.v[2];
                BrVector3Scale(&b, &b, (c->wpos[0].v[2] - c->wpos[2].v[2]));
                findfloor(&a, &b, norm, &ts2);
                if (ts2 > 1.0) {
                    c->down_force_flag = 1;
                }
            } else if (c->down_force_flag && (c->oldd[2] < c->susp_height[1] || c->oldd[3] < c->susp_height[1])) {
                c->down_force_flag = 0;
            }
            if (c->down_force_flag) {
                friction_number = (c->wpos[2].v[2] - c->cmpos.v[2]) / (c->wpos[2].v[2] - c->wpos[0].v[2]) * friction_number;
                f.v[0] = (c->wpos[0].v[2] - c->cmpos.v[2]) * friction_number + f.v[0];
            }
            B.v[1] = B.v[1] - friction_number;
        }
        vplane.v[0] = BrVector3Dot(&c->velocity_car_space, &c->road_normal) * c->road_normal.v[0];
        vplane.v[1] = BrVector3Dot(&c->velocity_car_space, &c->road_normal) * c->road_normal.v[1];
        vplane.v[2] = BrVector3Dot(&c->velocity_car_space, &c->road_normal) * c->road_normal.v[2];
        BrVector3Sub(&vplane, &c->velocity_car_space, &vplane);
        if (vplane.v[2] < 0.0) {
            ts = 1.0;
        } else {
            ts = -1.0;
        }
        ts3 = BrVector3Length(&vplane);
        deltaomega = ts3 * c->curvature * ts;
        deltaomega = deltaomega - BrVector3Dot(&c->omega, &c->road_normal);
        BrVector3Set(&v103, c->road_normal.v[1], -c->road_normal.v[0], 0);
        BrVector3Normalise(&v103, &v103);

        friction_number = c->I.v[1] / dt * deltaomega;
        ts = friction_number / (c->wpos[2].v[2] - c->wpos[0].v[2]);
        v108 = ts;
        v109 = -ts;
        BrVector3Set(&rightplane, 0, c->road_normal.v[2], -c->road_normal.v[1]);
        BrVector3Normalise(&rightplane, &rightplane);
        v99 = c->acc_force;
        friction_number = BrVector3Dot(&rightplane, &vplane);
        v87 = BrVector3Dot(&v103, &vplane);
        ts2 = fabs(v87);
        friction_number = (c->wpos[0].v[2] - c->cmpos.v[2]) * friction_number * fabs(c->curvature);
        if (c->curvature <= 0.0) {
            friction_number = v87 - friction_number;
        } else {
            friction_number = v87 + friction_number;
        }
        friction_number = -(c->M / dt * friction_number);
        friction_number = friction_number - BrVector3Dot(&B, &v103);

        friction_number = friction_number / (1.0 - wheelratio);
        v108 = friction_number + v108;
        v109 = -wheelratio * friction_number + v109;
        friction_number = (c->wpos[0].v[2] - c->wpos[2].v[2]) * v108;
        v98 = friction_number * c->curvature;
        friction_number = BrVector3Dot(&c->velocity_car_space, &rightplane) * c->M / dt;
        v129 = BrVector3Dot(&rightplane, &B) + friction_number;
        v128 = c->mu[0] * c->brake_force / (c->mu[1] / c->friction_elipticity + c->mu[0]);
        v125 = c->brake_force - v128;
        v105 = (c->damage_units[7].damage_level + c->damage_units[6].damage_level) / 2;
        if (v105 > 20) {
            v128 = (1.0 - (double)(v105 - 20) / 80.0) * (1.0 - (double)(v105 - 20) / 80.0) * v128;
        }
        v105 = (c->damage_units[5].damage_level + c->damage_units[4].damage_level) / 2;
        if (v105 > 20) {
            v125 = (1.0 - (double)(v105 - 20) / 80.0) * (1.0 - (double)(v105 - 20) / 80.0) * v125;
        }
        ts2 = (force[1] + force[0]) * c->rolling_r_back + v128;
        v87 = (force[2] + force[3]) * c->rolling_r_front + v125;
        v128 = c->wpos[0].v[2] - c->wpos[2].v[2];
        v128 = sqrt(v128 * v128 * c->curvature * c->curvature + 1.0);
        v106 = v87 / v128;
        v134 = v106 + ts2;
        if (fabs(v129) < fabs(v134)) {
            ts2 = v129 / v134 * ts2;
            v106 = v129 / v134 * v106;
        }
        if ((v87 + ts2) * v129 < 0.0) {
            ts2 = -ts2;
            v106 = -v106;
        }
        v129 = v129 - (ts2 + v106);
        v99 = v99 - ts2;
        if (c->keys.brake && c->damage_units[eDamage_lr_brake].damage_level < 60 && c->damage_units[eDamage_rr_brake].damage_level < 60) {
            v99 = v99 - v129;
            c->gear = 0;
        }
        v99 = v99 / c->friction_elipticity;
        v135 = sqrt(v99 * v99 + v109 * v109) / 2.0;

        GetOilFrictionFactors(c, &fl_oil_factor, &fr_oil_factor, &rl_oil_factor, &rr_oil_factor);
        if (c->driver <= eDriver_non_car) {
            v116 = 1.0;
        } else {
            v116 = c->grip_multiplier;
        }
        BrVector3Sub(&v102, &c->wpos[0], &c->cmpos);
        BrVector3Cross(&a, &c->omega, &v102);
        BrVector3Accumulate(&a, &c->velocity_car_space);
        if (c->driver >= eDriver_net_human
            && (((c->keys.left || c->joystick.left > 0x8000) && c->curvature > 0.0 && deltaomega > 0.1 && a.v[0] > 0.0)
                || ((c->keys.right || c->joystick.right > 0x8000) && c->curvature < 0.0 && deltaomega < 0.1 && a.v[0] < 0.0))
            && ts > 0.0) {
            friction_number = c->mu[0];
        } else {
            friction_number = c->mu[2];
            ts2 = fabs(a.v[0]) / 10.0;
            if (ts2 > 1) {
                ts2 = 1.0;
            }
            friction_number = (c->mu[2] - c->mu[0]) * ts2 + friction_number;
        }

        maxfl = sqrt(force[0]) * friction_number * (rl_oil_factor * v116) * mat_list[c->material_index[0]].tyre_road_friction;
        maxfr = sqrt(force[1]) * friction_number * (rr_oil_factor * v116) * mat_list[c->material_index[1]].tyre_road_friction;
        c->max_force_rear = maxfr + maxfl;
        if (rl_oil_factor == 1.0 && rr_oil_factor == 1.0 && c->traction_control && v135 * 2.0 > c->max_force_rear && c->acc_force > 0.0
            && (c->driver < eDriver_net_human || (c->target_revs > 1000.0 && c->gear > 0))) {
            ts2 = v99;
            if (v99 * v99 <= v135 * v135 * 4.0) {
                v87 = sqrt(v135 * v135 * 4.0 - v99 * v99);
            } else {
                v87 = 0.0;
            }
            if (c->max_force_rear <= v87) {
                c->torque = -(c->revs * c->revs / 100000000.0) - 0.1;
            } else {
                float v177 = sqrt(c->max_force_rear * c->max_force_rear - v87 * v87);
                ts3 = ts2 < 0.0 ? -1.0 : 1.0;
                // ts4 = (ts2 - ts3 * sqrt(ts3)) * 1.01;
                // if (fabs(ts2) > fabs(ts4)) {
                //     v87 = ts4;
                //     ts2 = v87;
                // }

                ts4 = (ts2 - ts3 * v177) * 1.01;
                if (fabs(ts2) > fabs(ts4)) {
                    ts2 = ts4;
                }
            }
            v99 = v99 - ts2;
            v135 = sqrt(v99 * v99 + v109 * v109) / 2.0;

        } else if (c->driver >= eDriver_net_human && c->gear > 0 && c->revs > c->target_revs && !c->traction_control) {
            if (!c->keys.change_down) {
                c->traction_control = 1;
            }
            friction_number = 1.0 - (c->revs - c->target_revs) / (double)(400 * c->gear);
            if (friction_number < 0.40000001) {
                friction_number = 0.40000001;
            }
            maxfl = friction_number * maxfl;
            maxfr = friction_number * maxfr;
        }
        if (fabs(v109) > maxfr + maxfl && maxfr + maxfl > 0.1) {
            v87 = (maxfr + maxfl) / fabs(v109) * dt;
            v109 = v87 * v109;
            v99 = c->friction_elipticity * v87 * v99;
            friction_number = -((c->wpos[2].v[2] - c->cmpos.v[2]) * v108 * ((c->wpos[2].v[2] - c->cmpos.v[2]) * v108) / c->I.v[1] + (v98 * v98 + v108 * v108) / c->M);
            ts2 = (BrVector3Dot(&v103, &vplane) + v109 / c->M) * v108;
            ts2 = BrVector3Dot(&rightplane, &vplane) * v98 + ts2;
            ts2 = BrVector3Dot(&c->omega, &c->road_normal) * (c->wpos[2].v[2] - c->cmpos.v[2]) * v108 + ts2;
            ts2 = (c->wpos[0].v[2] - c->cmpos.v[2]) * (c->wpos[2].v[2] - c->cmpos.v[2]) * v109 / c->I.v[1] * v108 + ts2;
            if (fabs(friction_number) > 0.1) {
                friction_number = ts2 / (friction_number * dt);
                v108 = friction_number * v108;
                v98 = friction_number * v98;
            }
            v109 = v109 / v87;
            v99 = v99 / (c->friction_elipticity * v87);
        }
        v98 = v98 - v106;
        v108 = (c->wpos[0].v[2] - c->wpos[2].v[2]) * c->curvature * v106 + v108;
        if (v135 > 0.000099999997) {
            v109 = v109 / (v135 * 2.0);
            v99 = v99 / (v135 * 2.0);
        }
        v99 = c->friction_elipticity * v99;
        force[0] = v135;
        force[1] = v135;
        c->wheel_slip = 0;
        switch ((force[0] > maxfl) + 2 * (force[1] > maxfr)) {
        case 0:
            slide_dist = 0;
            break;
        case 1:
            force[0] = c->freduction * maxfl;
            force[1] = v135 - force[0] + force[1];
            if (force[1] <= maxfr) {
                slide_dist = 0;
            } else {
                if (maxfr > 0.1) {
                    pV = (force[1] - maxfr) / maxfr;
                    if (&gProgram_state.current_car == c) {
                        v78 = 20.0;
                    } else {
                        v78 = 60.0;
                    }
                    if (v78 <= pV) {
                        c->new_skidding |= 2u;
                    }
                    SkidNoise(c, 1, pV, c->material_index[1]);
                }
                force[1] = c->freduction * maxfr;
                c->wheel_slip |= 2u;
            }
            break;
        case 2:
            force[1] = c->freduction * maxfr;
            force[0] = v135 - force[1] + force[0];
            if (force[0] <= maxfl) {
                slide_dist = 0;
            } else {
                if (maxfl > 0.1) {
                    pV = (force[0] - maxfl) / maxfl;
                    if (&gProgram_state.current_car == c) {
                        v77 = 20.0;
                    } else {
                        v77 = 60.0;
                    }
                    if (v77 <= pV) {
                        c->new_skidding |= 1u;
                    }
                    SkidNoise(c, 0, pV, c->material_index[0]);
                }
                force[0] = c->freduction * maxfl;
                c->wheel_slip |= 2u;
            }
            break;
        case 3:
            force[0] = c->freduction * maxfl;
            force[1] = c->freduction * maxfr;
            c->wheel_slip |= 2u;
            pV = (v135 * 2.0 - maxfl - maxfr) / (maxfr + maxfl);
            if (&gProgram_state.current_car == c) {
                v79 = 20.0;
            } else {
                v79 = 60.0;
            }
            if (v79 <= pV) {
                if (maxfl > 0.1) {
                    c->new_skidding |= 1u;
                }
                if (maxfr > 0.1) {
                    c->new_skidding |= 2u;
                }
            }
            if (IRandomBetween(0, 1)) {
                if (maxfl > 0.1) {
                    SkidNoise(c, 0, pV, c->material_index[0]);
                }
            } else if (maxfr > 0.1) {
                SkidNoise(c, 1, pV, c->material_index[1]);
            }
            break;
        default:
            break;
        }
        if (c->wheel_slip && c->curvature * c->turn_speed > 0.0 && fabs(v109) > 0.0099999998 && c->curvature * v109 < 0.0 && !c->keys.brake && !c->keys.change_down) {
            c->turn_speed = 0.0;
        }
        v135 = sqrt(v108 * v108 + v98 * v98) / 2.0;
        if (v135 > 0.000099999997) {
            v108 = v108 / (v135 * 2.0);
            v98 = v98 / (v135 * 2.0);
        }
        maxfl = sqrt(force[2]) * c->mu[1] * (fl_oil_factor * v116) * mat_list[c->material_index[2]].tyre_road_friction;
        maxfr = sqrt(force[3]) * c->mu[1] * (fr_oil_factor * v116) * mat_list[c->material_index[3]].tyre_road_friction;
        c->max_force_front = maxfr + maxfl;
        force[2] = v135;
        force[3] = v135;
        v72 = (v135 > maxfl) + 2 * (v135 > maxfr);
        switch (v72) {
        case 1:
            force[2] = c->freduction * maxfl;
            force[3] = v135 - force[2] + force[3];
            if (force[3] > maxfr) {
                if (maxfr > 0.1) {
                    pV = (force[3] - maxfr) / maxfr;
                    if (&gProgram_state.current_car == c) {
                        v75 = 20.0;
                    } else {
                        v75 = 60.0;
                    }
                    if (v75 <= pV) {
                        c->new_skidding |= 8u;
                    }
                    SkidNoise(c, 3, pV, c->material_index[3]);
                }
                force[3] = c->freduction * maxfr;
                c->wheel_slip |= 1u;
            }
            break;
        case 2:
            force[3] = c->freduction * maxfr;
            force[2] = v135 - force[3] + force[2];
            if (force[2] > maxfl) {
                if (maxfl > 0.1) {
                    pV = (force[2] - maxfl) / maxfl;
                    if (&gProgram_state.current_car == c) {
                        v74 = 20.0;
                    } else {
                        v74 = 60.0;
                    }
                    if (v74 <= pV) {
                        c->new_skidding |= 4u;
                    }
                    SkidNoise(c, 2, pV, c->material_index[2]);
                }
                force[2] = c->freduction * maxfl;
                c->wheel_slip |= 1u;
            }
            break;
        case 3:
            force[2] = c->freduction * maxfl;
            force[3] = c->freduction * maxfr;
            c->wheel_slip |= 1u;
            pV = (v135 * 2.0 - maxfl - maxfr) / (maxfr + maxfl);
            if (&gProgram_state.current_car == c) {
                v76 = 20.0;
            } else {
                v76 = 60.0;
            }
            if (v76 <= pV) {
                if (maxfl > 0.1) {
                    c->new_skidding |= 4u;
                }
                if (maxfr > 0.1) {
                    c->new_skidding |= 8u;
                }
            }
            if (IRandomBetween(0, 1)) {
                if (maxfl > 0.1) {
                    SkidNoise(c, 2, pV, c->material_index[2]);
                }
            } else if (maxfr > 0.1) {
                SkidNoise(c, 3, pV, c->material_index[3]);
            }
            break;
        }
        BrVector3Scale(&v136, &rightplane, v99);
        BrVector3Scale(&a, &v103, v109);
        BrVector3Accumulate(&v136, &a);
        BrVector3Scale(&v123, &rightplane, v98);
        BrVector3Scale(&a, &v103, v108);
        BrVector3Accumulate(&v123, &a);

        rightplane = c->wpos[0];
        rightplane.v[1] = rightplane.v[1] - c->oldd[0];
        BrVector3Sub(&rightplane, &rightplane, &c->cmpos);
        BrVector3Scale(&b, &v136, force[0]);
        BrVector3Accumulate(&B, &b);
        BrVector3Cross(&a, &rightplane, &b);
        BrVector3Accumulate(&f, &a);

        rightplane = c->wpos[1];
        rightplane.v[1] = rightplane.v[1] - c->oldd[1];
        BrVector3Sub(&rightplane, &rightplane, &c->cmpos);
        BrVector3Scale(&b, &v136, force[1]);
        BrVector3Accumulate(&B, &b);
        BrVector3Cross(&a, &rightplane, &b);
        BrVector3Accumulate(&f, &a);

        rightplane = c->wpos[2];
        rightplane.v[1] = rightplane.v[1] - c->oldd[2];
        BrVector3Sub(&rightplane, &rightplane, &c->cmpos);
        BrVector3Scale(&b, &v123, force[2]);
        BrVector3Accumulate(&B, &b);
        BrVector3Cross(&a, &rightplane, &b);
        BrVector3Accumulate(&f, &a);

        rightplane = c->wpos[3];
        rightplane.v[1] = rightplane.v[1] - c->oldd[3];
        BrVector3Sub(&rightplane, &rightplane, &c->cmpos);
        BrVector3Scale(&b, &v123, force[3]);
        BrVector3Accumulate(&B, &b);
        BrVector3Cross(&a, &rightplane, &b);
        BrVector3Accumulate(&f, &a);

    } else {
        c->max_force_front = 0.0;
        c->max_force_rear = 0.0;
        StopSkid(c);
    }
    c->number_of_wheels_on_ground = normnum;
    BrMatrix34ApplyV(&b, &B, mat);
    BrVector3Scale(&rightplane, &f, dt);
    ApplyTorque(c, &rightplane);
    BrVector3Scale(&rightplane, &b, dt / c->M);
    BrVector3Accumulate(&c->v, &rightplane);
    if (c->speed < 0.0001f
        && ((!c->keys.acc && c->joystick.acc <= 0) || !c->gear)
        && !c->keys.dec
        && c->joystick.dec <= 0
        && c->bounce_rate == 0.0
        && BrVector3Length(&c->omega) < 0.05) {
        if (vol) {
            v73 = c->driver > eDriver_non_car && c->underwater_ability ? 1.0 - (1.0 - vol->gravity_multiplier) * 0.6 : vol->gravity_multiplier;
            friction_number = BrVector3Length(&b) / v73 / gGravity_multiplier;
        } else {
            friction_number = BrVector3Length(&b);
        }
        if (c->M > friction_number || (c->keys.brake && normnum >= 3)) {
            if (stop_timer == 100.0) {
                stop_timer = 0.0;
            }
            if (stop_timer > 0.5) {
                BrVector3SetFloat(&c->v, 0.0, 0.0, 0.0);
                BrVector3SetFloat(&c->omega, 0.0, 0.0, 0.0);
                stop_timer = 0.5;
            }
        }
    }
    stop_timer = dt + stop_timer;
    if (stop_timer > 1.0) {
        stop_timer = 100.0;
    }
    AddDrag(c, dt);
    if (c->driver >= eDriver_net_human) {
        c->acc_force = -(v136.v[2] * force[0]) - v136.v[2] * force[1];
        // LOG_DEBUG("old %f new %f", old, c->acc_force);
    }
}

// IDA: void __usercall DoRevs(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047ef8e
void DoRevs(tCar_spec* c, br_scalar dt) {
    br_scalar wheel_spin_force;
    br_scalar ts;
    int revs_increase;

    ts = -BrVector3Dot((br_vector3*)c->car_master_actor->t.t.mat.m[2], &c->v);

    if (c->gear) {
        c->target_revs = ts / c->speed_revs_ratio / (double)c->gear;
    } else {
        c->target_revs = 0.0;
    }
    if (c->target_revs < 0.0) {
        c->target_revs = 0.0;
        c->gear = 0;
    }
    if (!c->number_of_wheels_on_ground || ((c->wheel_slip & 2) + 1) != 0 || !c->gear) {
        if (c->number_of_wheels_on_ground) {
            wheel_spin_force = c->force_torque_ratio * c->torque - (double)c->gear * c->acc_force;
        } else {
            wheel_spin_force = c->force_torque_ratio * c->torque;
        }
        if (c->gear) {
            if (c->gear < 2 && (c->keys.dec || c->joystick.dec > 0) && fabs(ts) < 1.0 && c->revs > 1000.0) {
                c->gear = -c->gear;
            }
        } else {
            if (c->revs > 1000.0 && !c->keys.brake && (c->keys.acc || c->joystick.acc > 0) && !gCountdown) {
                if (c->keys.backwards) {
                    c->gear = -1;
                } else {
                    c->gear = 1;
                }
            }
            wheel_spin_force = c->force_torque_ratio * c->torque;
        }
        c->revs = wheel_spin_force / c->force_torque_ratio * dt / 0.0002 + c->revs;

        if (c->traction_control && wheel_spin_force > 0.0 && c->revs > c->target_revs && c->gear && c->target_revs > 1000.0) {
            c->revs = c->target_revs;
        }
        if (c->revs <= 0.0) {
            c->revs = 0.0;
        }
    }
    if ((c->wheel_slip & 2) == 0 && c->target_revs > 6000.0 && c->revs > 6000.0 && c->gear < c->max_gear && c->gear > 0 && !c->just_changed_gear) {
        c->gear++;
    }
    if (c->gear > 1 && c->target_revs < 3000.0 && !c->just_changed_gear) {
        c->gear--;
    }
    if (c->revs < 200.0 && c->target_revs < 200.0 && c->gear <= 1 && !c->keys.acc && c->joystick.acc <= 0 && !c->just_changed_gear) {
        c->gear = 0;
    }
    if (c->just_changed_gear && c->revs < 6000.0 && c->revs > 200.0 && (c->gear < 2 || c->revs >= 3000.0)) {
        c->just_changed_gear = 0;
    }
    if (c->revs >= 6000.0 && (c->keys.acc || c->joystick.acc > 0)) {
        c->just_changed_gear = 0;
    }
}

// IDA: void __usercall ApplyTorque(tCar_spec *c@<EAX>, br_vector3 *tdt@<EDX>)
// FUNCTION: CARM95 0x0047f4f2
void ApplyTorque(tCar_spec* c, br_vector3* tdt) {

    c->omega.v[0] = tdt->v[0] / c->I.v[0] + c->omega.v[0];
    c->omega.v[1] = tdt->v[1] / c->I.v[1] + c->omega.v[1];
    c->omega.v[2] = tdt->v[2] / c->I.v[2] + c->omega.v[2];
}

// IDA: void __usercall TranslateCar(tCollision_info *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047f55f
void TranslateCar(tCollision_info* c, br_scalar dt) {
    br_vector3 t;

    t.v[0] = c->v.v[0] * dt;
    t.v[1] = c->v.v[1] * dt;
    t.v[2] = c->v.v[2] * dt;
    c->car_master_actor->t.t.mat.m[3][0] = c->car_master_actor->t.t.mat.m[3][0] + t.v[0];
    c->car_master_actor->t.t.mat.m[3][1] = c->car_master_actor->t.t.mat.m[3][1] + t.v[1];
    c->car_master_actor->t.t.mat.m[3][2] = c->car_master_actor->t.t.mat.m[3][2] + t.v[2];
}

// IDA: int __usercall CollCheck@<EAX>(tCollision_info *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0047f5d0
int CollCheck(tCollision_info* c, br_scalar dt) {
    br_vector3 a;
    br_vector3 a1;
    br_vector3 aa;
    br_vector3 bb;
    br_vector3 cc;
    br_vector3 pos;
    br_vector3 r[8];
    br_vector3 norm;
    br_vector3 n[8];
    br_vector3 dir;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 tau[4];
    br_vector3 old_val;
    br_vector3 ftau;
    br_vector3 max_friction;
    br_vector3 vel;
    br_vector3 p_vel;
    br_vector3 normal_force;
    br_vector3 friction_force;
    br_scalar d[4];
    br_scalar f[4];
    br_scalar total_force;
    br_scalar point_vel;
    br_scalar batwick_length;
    br_matrix4 M;
    br_scalar dist;
    br_scalar min_d;
    br_scalar ts;
    br_scalar ts2;
    br_scalar v_diff;
    br_matrix34* mat;
    br_matrix34* oldmat;
    br_matrix34 mat_to_oldmat;
    br_matrix34 oldmat_to_mat;
    br_matrix34 tm;
    int collision;
    br_bounds bnds;
    tFace_ref* f_ref;
    int i;
    int j;
    int l;
    int k;
    int material;
    int noise_defeat;
    // GLOBAL: CARM95 0x53a52c
    static int oldk;
    br_scalar min;
    br_scalar max;
    br_vector3 edges[3];
    br_vector3 corner;
    br_vector3 test_dir;
    br_scalar min_acc;
    br_scalar max_acc;
    br_matrix34 message_mat;

    // v34 = 0;
    // v35 = 0;
    // v36 = 0x3F800000;
    // v48 = 0x3F800347;
    mat = &c->car_master_actor->t.t.mat;
    oldmat = &c->oldmat;
    k = 0;
    gMaterial_index = 0;
    if (c->dt >= 0.0f && gNet_mode == eNet_mode_host) {
        oldmat = &message_mat;
        GetExpandedMatrix(&message_mat, &c->message.mat);
    }
    if (dt < 0.0f) {
        mat = oldmat;
    }
    BrMatrix34LPInverse(&tm, mat);
    BrMatrix34Mul(&oldmat_to_mat, oldmat, &tm);

    oldmat_to_mat.m[3][0] = oldmat_to_mat.m[3][0] / WORLD_SCALE;
    oldmat_to_mat.m[3][1] = oldmat_to_mat.m[3][1] / WORLD_SCALE;
    oldmat_to_mat.m[3][2] = oldmat_to_mat.m[3][2] / WORLD_SCALE;
    GetNewBoundingBox(&bnds, &c->bounds[2], &oldmat_to_mat);
    for (i = 0; i < 3; ++i) {
        if (c->bounds[2].min.v[i] < bnds.min.v[i]) {
            bnds.min.v[i] = c->bounds[2].min.v[i];
        }
        if (c->bounds[2].max.v[i] > bnds.max.v[i]) {
            bnds.max.v[i] = c->bounds[2].max.v[i];
        }
    }
    a1.v[0] = mat->m[3][0] / WORLD_SCALE;
    a1.v[1] = mat->m[3][1] / WORLD_SCALE;
    a1.v[2] = mat->m[3][2] / WORLD_SCALE;
    BrMatrix34ApplyV(&aa, &bnds.min, mat);
    BrVector3Accumulate(&aa, &a1);
    for (j = 0; j < 3; ++j) {
        edges[j].v[0] = (bnds.max.v[j] - bnds.min.v[j]) * mat->m[j][0];
        edges[j].v[1] = (bnds.max.v[j] - bnds.min.v[j]) * mat->m[j][1];
        edges[j].v[2] = (bnds.max.v[j] - bnds.min.v[j]) * mat->m[j][2];
    }
    for (i = 0; i < 50 && i < c->box_face_end - c->box_face_start; i++) {
        f_ref = &gFace_list__car[c->box_face_start + i];
        BrVector3Sub(&bb, &aa, &f_ref->v[0]);
        max = BrVector3Dot(&bb, &f_ref->normal);
        min = max;
        for (j = 0; j < 3; ++j) {
            ts = BrVector3Dot(&edges[j], &f_ref->normal);
            if (ts >= 0) {
                max = max + ts;
            } else {
                min = min + ts;
            }
        }
        if ((max <= 0.001f || min <= 0.001f) && (max >= -0.001f || min >= -0.001f)) {
            f_ref->flags &= ~0x80u;
            k++;
        } else {
            f_ref->flags |= 0x80u;
        }
    }
    if (k == 0) {
        return 0;
    }
    k = 0;
    BrMatrix34LPInverse(&tm, oldmat);
    BrMatrix34Mul(&mat_to_oldmat, mat, &tm);
    gEliminate_faces = 1;
    for (i = 0; i < 8 + c->extra_point_num; i++) {
        if (i >= 8) {
            tv = c->extra_points[i - 8];
        } else {
            tv.v[0] = ((i & 2) == 0) * c->bounds[1].min.v[0] + ((i & 2) >> 1) * c->bounds[1].max.v[0];
            tv.v[1] = ((i & 1) == 0) * c->bounds[1].min.v[1] + ((i & 1) >> 0) * c->bounds[1].max.v[1];
            tv.v[2] = ((i & 4) == 0) * c->bounds[1].max.v[2] + ((i & 4) >> 2) * c->bounds[1].min.v[2];
        }
        BrMatrix34ApplyP(&dir, &tv, mat);
        if (dt >= 0.0f) {
            BrMatrix34ApplyP(&a, &tv, oldmat);
        } else {
            BrVector3Scale(&a, &c->pos, WORLD_SCALE);
        }
        BrVector3Sub(&dir, &dir, &a);
        BrVector3Normalise(&normal_force, &dir);
        BrVector3Scale(&normal_force, &normal_force, 0.0072463769f);
        BrVector3Accumulate(&dir, &normal_force);
        material = FindFloorInBoxM2(&a, &dir, &norm, &dist, c);
        if (dist >= 0.0f && dist < 1.0001f) {
            BrVector3Scale(&cc, &c->pos, WORLD_SCALE);
            BrVector3Sub(&cc, &cc, &a);
            FindFloorInBoxM(&a, &cc, &bb, &ts, c);
            if (i < 8 || ts > 1.0f) {
                BrMatrix34TApplyV(&a, &norm, oldmat);
                AddCollPoint(dist, &tv, &a, r, n, &dir, k, c);
                k++;
                if (gMaterial_index == 0) {
                    gMaterial_index = material;
                }
            }
        }
    }
    gEliminate_faces = 0;
    if (k < 1) {
        k += BoxFaceIntersect(&c->bounds[1], mat, &mat_to_oldmat, &r[k], &n[k], &d[k], 8 - k, c);
    }
    if (k > 4) {
        k = 4;
    }
    for (i = 0; i < k; i++) {
        if (fabs(r[i].v[1]) + fabs(r[i].v[2]) + fabs(r[i].v[0]) > 500.0f) {
            for (j = i + 1; j < k; j++) {
                if (fabs(r[j].v[1]) + fabs(r[j].v[2]) + fabs(r[j].v[0]) < 500.0f) {
                    r[i] = r[j];
                    n[i] = n[j];
                    i++;
                }
            }
            k = i;
            break;
        }
    }
    if (dt >= 0.0f) {
        if (k > 0 && c->collision_flag && k < 4
            && (fabs(r[0].v[0] - c->old_point.v[0]) > 0.05f
                || fabs(r[0].v[1] - c->old_point.v[1]) > 0.05f
                || fabs(r[0].v[2] - c->old_point.v[2]) > 0.05f)) {
            r[k] = c->old_point;
            n[k] = c->old_norm;
            k++;
        }
        if (k > 0) {
            c->old_point = r[0];
            c->old_norm = n[0];
            BrMatrix34Copy(mat, oldmat);
            c->omega = c->oldomega;
            BrMatrix34TApplyV(&c->velocity_car_space, &c->v, mat);
            memset(&norm, 0, sizeof(norm));
            collision = 0;
            for (i = 0; i < k; i++) {
                BrVector3Cross(&tau[i], &r[i], &n[i]);
                tau[i].v[0] = tau[i].v[0] / c->I.v[0];
                tau[i].v[1] = tau[i].v[1] / c->I.v[1];
                tau[i].v[2] = tau[i].v[2] / c->I.v[2];
                BrVector3Cross(&normal_force, &c->omega, &r[i]);
                BrVector3Accumulate(&normal_force, &c->velocity_car_space);
                d[i] = -(BrVector3Dot(&n[i], &normal_force));
                BrVector3Add(&normal_force, &r[i], &c->cmpos);
                BrMatrix34ApplyP(&dir, &normal_force, &mat_to_oldmat);
                BrVector3Sub(&dir, &dir, &normal_force);
                ts = -(BrVector3Dot(&n[i], &dir) / dt);
                if (ts > d[i]) {
                    d[i] = ts;
                }
                if (d[i] > 0.0f) {
                    collision = 1;
                }
            }
            if (!collision) {
                d[0] = 0.5f;
            }
            for (i = 0; i < k; i++) {
                for (j = 0; j < k; j++) {
                    BrVector3Cross(&normal_force, &tau[j], &r[i]);
                    BrVector3InvScale(&norm, &n[j], c->M);
                    BrVector3Accumulate(&normal_force, &norm);
                    M.m[i][j] = BrVector3Dot(&n[i], &normal_force);
                }
            }
            switch (k) {
            case 1:
                ts = SinglePointColl(f, &M, d);
                break;
            case 2:
                ts = TwoPointColl(f, &M, d, tau, n);
                break;
            case 3:
                d[3] = 0.0f;
                ts = ThreePointCollRec(f, &M, d, tau, n, c);
                break;
            case 4:
                ts = FourPointColl(f, &M, d, tau, n, c);
                break;
            default:
                break;
            }
            if (k > 3) {
                k = 3;
            }
            // if (f[0] > 10.0f || f[1] > 10.0f || f[2] > 10.0f) {
            //     v31 = 0;
            // }
            if (fabs(ts) <= 0.000001f) {
                BrVector3Set(&c->v, 0.f, 0.f, 0.f);
                BrVector3Set(&c->omega, 0.f, 0.f, 0.f);
                BrVector3Set(&c->oldomega, 0.f, 0.f, 0.f);
                return k;
            }
            BrVector3Set(&p_vel, 0.f, 0.f, 0.f);
            BrVector3Set(&dir, 0.f, 0.f, 0.f);
            BrVector3Set(&friction_force, 0.f, 0.f, 0.f);
            total_force = 0.f;
            for (i = 0; i < k; i++) {
                if (f[i] < 0.001f) {
                    f[i] = 0.001f;
                }
                f[i] = f[i] * 1.001f;
                BrVector3Scale(&tau[i], &tau[i], f[i]);
                BrVector3Accumulate(&c->omega, &tau[i]);
                f[i] = f[i] / c->M;
                BrVector3Scale(&n[i], &n[i], f[i]);
                BrVector3Accumulate(&p_vel, &n[i]);
                BrVector3Add(&bb, &r[i], &c->cmpos);
                BrVector3Scale(&bb, &bb, f[i]);
                BrVector3Accumulate(&dir, &bb);
                total_force = f[i] + total_force;
            }
            if (gPinball_factor != 0.0f) {
                BrVector3Scale(&p_vel, &p_vel, gPinball_factor);
                point_vel = BrVector3LengthSquared(&p_vel);
                if (point_vel > 10.0f) {
                    noise_defeat = 1;
                    if (c->driver == eDriver_local_human) {
                        DRS3StartSound(gCar_outlet, 9011);
                    } else {
                        DRS3StartSound3D(gCar_outlet, 9011, &c->pos, &gZero_v__car, 1, 255, 0x10000, 0x10000);
                    }
                    if (point_vel > 10000.0f) {
                        BrVector3Normalise(&p_vel, &p_vel);
                        BrVector3Scale(&p_vel, &p_vel, 100);
                    }
                }
            }
            BrVector3Accumulate(&c->velocity_car_space, &p_vel);
            BrVector3InvScale(&dir, &dir, total_force);
            BrVector3Cross(&tv, &c->omega, &dir);
            BrVector3Accumulate(&tv, &c->velocity_car_space);
            batwick_length = BrVector3Length(&tv);
            if (!c->collision_flag || (c->collision_flag == 1 && oldk < k)) {
                for (i = 0; i < k; i++) {
                    BrVector3Cross(&vel, &c->omega, &r[i]);
                    BrVector3Accumulate(&vel, &c->velocity_car_space);
                    AddFriction(c, &vel, &n[i], &r[i], f[i], &max_friction);
                    BrVector3Accumulate(&friction_force, &max_friction);
                    BrVector3Accumulate(&c->velocity_car_space, &max_friction);
                }
            }
            oldk = k;
            BrMatrix34ApplyP(&pos, &dir, &c->car_master_actor->t.t.mat);
            BrVector3InvScale(&pos, &pos, WORLD_SCALE);
            noise_defeat = 0;
            BrVector3Add(&normal_force, &friction_force, &p_vel);
            BrMatrix34ApplyV(&norm, &normal_force, mat);
            min = dt * 90.0f / 10.0f;
            max = dt * 110.0f / 10.0f;
            if (c->last_special_volume != NULL) {
                min *= c->last_special_volume->gravity_multiplier;
                max *= c->last_special_volume->gravity_multiplier;
            }
            if (BrVector3LengthSquared(&c->velocity_car_space) < 0.05f
                && 0.1f * total_force > BrVector3Dot(&c->omega, &tv)
                && k >= 3
                && norm.v[1] > min
                && norm.v[1] < max) {
                if (c->driver <= eDriver_non_car || fabs(normal_force.v[2]) <= total_force * 0.9f) {
                    BrVector3Set(&c->v, 0.f, 0.f, 0.f);
                    BrVector3Set(&norm, 0.f, 0.f, 0.f);
                    BrVector3Set(&normal_force, 0.f, 0.f, 0.f);
                    BrVector3Set(&c->omega, 0.f, 0.f, 0.f);
                    BrVector3Set(&c->oldomega, 0.f, 0.f, 0.f);
                    if (c->driver <= eDriver_non_car || CAR(c)->max_force_rear == 0.0f) {
                        if (c->driver <= eDriver_non_car) {
                            PipeSingleNonCar(c);
                        }
                        c->doing_nothing_flag = 1;
                    }
                } else {
                    BrVector3SetFloat(&tv2, 0.0f, -1.0f, 0.0f);
                    bb.v[0] = mat->m[1][2] * tv2.v[1] - mat->m[1][1] * tv2.v[2];
                    bb.v[1] = mat->m[1][0] * tv2.v[2] - mat->m[1][2] * tv2.v[0];
                    bb.v[2] = mat->m[1][1] * tv2.v[0] - mat->m[1][0] * tv2.v[1];
                    if (BrVector3Dot(&bb, (br_vector3*)&mat->m[0][1]) <= 0.0f) {
                        c->omega.v[0] = -0.5f;
                    } else {
                        c->omega.v[0] = 0.5f;
                    }
                }
            }
            BrVector3Accumulate(&c->v, &norm);
            if (c->driver >= eDriver_net_human) {
                BrVector3Scale(&normal_force, &normal_force, gDefensive_powerup_factor[CAR(c)->power_up_levels[0]]);
            }
            if (c->driver < eDriver_net_human) {
                BrVector3Scale(&normal_force, &normal_force, 0.01f);
            } else {
                BrVector3Scale(&normal_force, &normal_force, 0.75f);
            }
            if (
#if defined(DETHRACE_FIX_BUGS)
                // `c` is only a `tCar_spec*` if the driver is an opponent or human, otherwise, it will be a `tNon_car_spec*`. The following code
                // assumes `c` is a `tCar_spec*`, causing invalid memory accesses
                c->driver >= eDriver_oppo &&
#endif
                (CAR(c)->invulnerable
                    || (c->driver < eDriver_net_human && (c->driver != eDriver_oppo || PointOutOfSight(&c->pos, 150.0f)))
                    || ((v_diff = (CAR(c)->pre_car_col_velocity.v[1] - c->v.v[1]) * gDefensive_powerup_factor[CAR(c)->power_up_levels[0]]) >= -20.0f)
                    || CAR(c)->number_of_wheels_on_ground >= 3)) {
                CrushAndDamageCar(CAR(c), &dir, &normal_force, NULL);
            } else {
                // Cops Special Forces is always stolen if destroyed!
                if (c->driver == eDriver_oppo && c->index == 4 && v_diff < -40.0f) {
                    KnackerThisCar(CAR(c));
                    StealCar(CAR(c));
                    v_diff = v_diff * 5.0f;
                }
#if defined(DETHRACE_FIX_BUGS)
                // `c` is only a `tCar_spec*` if the driver is an opponent or human, otherwise, it will be a `tNon_car_spec*`. The following code
                // assumes `c` is a `tCar_spec*`, causing invalid memory accesses
                if (c->driver >= eDriver_oppo)
#endif
                {
                    for (i = 0; i < CAR(c)->car_actor_count; i++) {
                        ts2 = (v_diff + 20.0f) * -0.01f;
                        TotallySpamTheModel(CAR(c), i, CAR(c)->car_model_actors[i].actor, &CAR(c)->car_model_actors[i].crush_data, ts2);
                    }
                    for (i = 0; i < COUNT_OF(CAR(c)->damage_units); i++) {
                        DamageUnit(CAR(c), i, IRandomPosNeg(5) + (v_diff + 20.0f) * -1.5f);
                    }
                }
            }
            if (!noise_defeat) {
                CrashNoise(&norm, &pos, gMaterial_index);
                ScrapeNoise(batwick_length, &pos, gMaterial_index);
            }
            BrVector3InvScale(&tv, &tv, WORLD_SCALE);
            BrMatrix34ApplyV(&bb, &tv, &c->car_master_actor->t.t.mat);
            BrMatrix34ApplyV(&norm, &p_vel, &c->car_master_actor->t.t.mat);
            CreateSparks(&pos, &bb, &norm, gCurrent_race.material_modifiers[gMaterial_index].sparkiness, CAR(c));
        }
        return k;
    } else {
        if (k != 0) {
            c->old_point = r[0];
            c->old_norm = n[0];
        }
        return k;
    }
}

// IDA: br_scalar __usercall AddFriction@<ST0>(tCollision_info *c@<EAX>, br_vector3 *vel@<EDX>, br_vector3 *normal_force@<EBX>, br_vector3 *pos@<ECX>, br_scalar total_force, br_vector3 *max_friction)
// FUNCTION: CARM95 0x00481cb9
br_scalar AddFriction(tCollision_info* c, br_vector3* vel, br_vector3* normal_force, br_vector3* pos, br_scalar total_force, br_vector3* max_friction) {
    br_vector3 norm;
    br_vector3 tv;
    br_vector3 ftau;
    br_scalar ts;
    br_scalar point_vel;

    ts = BrVector3Dot(normal_force, vel) / BrVector3Dot(normal_force, normal_force);
    BrVector3Scale(&tv, normal_force, ts);
    BrVector3Sub(vel, vel, &tv);
    point_vel = total_force * 0.35f * gCurrent_race.material_modifiers[gMaterial_index].car_wall_friction;
    ts = BrVector3Length(vel);
    if (ts < 0.0001f) {
        BrVector3Set(max_friction, 0.f, 0.f, 0.f);
        return 0.0;
    }
    BrVector3InvScale(max_friction, vel, -ts);
    BrVector3Cross(&ftau, pos, max_friction);
    BrVector3Scale(&ftau, &ftau, c->M);
    ftau.v[0] = ftau.v[0] / c->I.v[0];
    ftau.v[1] = ftau.v[1] / c->I.v[1];
    ftau.v[2] = ftau.v[2] / c->I.v[2];
    ts = 1.0 / c->M;
    norm.v[0] = pos->v[2] * ftau.v[1] - pos->v[1] * ftau.v[2];
    norm.v[1] = pos->v[0] * ftau.v[2] - pos->v[2] * ftau.v[0];
    norm.v[2] = pos->v[1] * ftau.v[0] - pos->v[0] * ftau.v[1];
    ts = max_friction->v[0] * norm.v[0] + max_friction->v[1] * norm.v[1] + max_friction->v[2] * norm.v[2] + ts;
    if (fabs(ts) <= 0.0001f) {
        ts = 0.0f;
    } else {
        ts = -BrVector3Dot(max_friction, vel) / ts;
    }
    if (ts > point_vel) {
        ts = point_vel;
    }
    BrVector3Scale(max_friction, max_friction, ts);
    BrVector3Cross(&tv, pos, max_friction);
    BrVector3Scale(&tv, &tv, c->M);
    ApplyTorque((tCar_spec*)c, &tv);
    return point_vel;
}

// IDA: void __usercall AddFrictionCarToCar(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_vector3 *vel1@<EBX>, br_vector3 *vel2@<ECX>, br_vector3 *normal_force1, br_vector3 *pos1, br_vector3 *pos2, br_scalar total_force, br_vector3 *max_friction)
// FUNCTION: CARM95 0x0049231c
void AddFrictionCarToCar(tCollision_info* car1, tCollision_info* car2, br_vector3* vel1, br_vector3* vel2, br_vector3* normal_force1, br_vector3* pos1, br_vector3* pos2, br_scalar total_force, br_vector3* max_friction) {
    br_vector3 v_diff1;
    br_vector3 v_diff2;
    br_vector3 tau1;
    br_vector3 tau2;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 vel2_in_frame_1;
    br_scalar ts;
    br_scalar ts2;
    br_scalar v_diff;
    br_scalar stopping_impulse;
    br_scalar total_friction;
    int i;

    BrMatrix34TApplyV(&tv, vel2, &car2->oldmat);
    BrMatrix34ApplyV(&vel2_in_frame_1, &tv, &car1->oldmat);
    BrVector3Sub(&v_diff1, &vel2_in_frame_1, vel1);
    ts = BrVector3LengthSquared(normal_force1);
    ts2 = BrVector3Dot(normal_force1, &v_diff1) / ts;
    BrVector3Scale(&tv, normal_force1, ts2);
    BrVector3Sub(&v_diff1, &v_diff1, &tv);
    v_diff = BrVector3Length(&v_diff1);
    if (v_diff < 0.01f) {
        BrVector3Set(max_friction, 0.f, 0.f, 0.f);
    } else {
        BrVector3InvScale(&v_diff1, &v_diff1, v_diff);
        BrMatrix34ApplyV(&tv, &v_diff1, &car1->oldmat);
        BrMatrix34TApplyV(&v_diff2, &tv, &car2->oldmat);
        BrVector3Negate(&v_diff2, &v_diff2);
        BrVector3Cross(&tau1, pos1, &v_diff1);
        BrVector3Cross(&tau2, pos2, &v_diff2);
        for (i = 0; i < 3; ++i) {
            tau1.v[i] /= car1->I.v[i];
            tau2.v[i] /= car2->I.v[i];
        }
        BrVector3Cross(&tv, &tau1, pos1);
        BrVector3Cross(&tv2, &tau2, pos2);
        ts = BrVector3Dot(&tv, &v_diff1) + BrVector3Dot(&tv2, &v_diff2) + 1.f / car2->M + 1.f / car1->M;
        if (ts < 0.0001f) {
            BrVector3Set(max_friction, 0.f, 0.f, 0.f);
        } else {
            stopping_impulse = v_diff / ts;
            total_friction = total_force * 0.35f;
            if (stopping_impulse < total_friction) {
                total_friction = stopping_impulse;
            }
            if (!car1->infinite_mass) {
                BrVector3Scale(&tau1, &tau1, total_friction);
                BrVector3Accumulate(&car1->omega, &tau1);
            }
            if (!car2->infinite_mass) {
                BrVector3Scale(&tau2, &tau2, total_friction);
                BrVector3Accumulate(&car2->omega, &tau2);
            }
            BrVector3Scale(max_friction, &v_diff1, total_friction);
        }
    }
}

// IDA: void __cdecl ScrapeNoise(br_scalar vel, br_vector3 *position, int material)
// FUNCTION: CARM95 0x00482070
void ScrapeNoise(br_scalar vel, br_vector3* position, int material) {
    tS3_volume vol;
    // GLOBAL: CARM95 0x514e54
    static tS3_sound_tag scrape_tag;
    // GLOBAL: CARM95 0x53a53c
    static tS3_volume last_scrape_vol;
    br_vector3 velocity;
    br_vector3 position_in_br;

    vol = vel * 7.0;
    if (gCurrent_race.material_modifiers[material].scrape_noise_index == -1) {
        return;
    }
    if ((scrape_tag && DRS3SoundStillPlaying(scrape_tag)) || vol <= 30) {
        if (last_scrape_vol < vol) {
            DRS3ChangeVolume(scrape_tag, vol);
            last_scrape_vol = vol;
        }
    } else {
        BrVector3Set(&velocity, 0.f, 0.f, 0.f);
        scrape_tag = DRS3StartSound3D(
            gCar_outlet,
            gMetal_scrape_sound_id__car[IRandomBetween(0, COUNT_OF(gMetal_scrape_sound_id__car) - 1)],
            position,
            &velocity,
            1,
            vol,
            IRandomBetween(49152, 81920),
            0x10000);
        last_scrape_vol = vol;
    }
}

// IDA: void __usercall SkidNoise(tCar_spec *pC@<EAX>, int pWheel_num@<EDX>, br_scalar pV, int material)
// FUNCTION: CARM95 0x0048216d
void SkidNoise(tCar_spec* pC, int pWheel_num, br_scalar pV, int material) {
    br_vector3 pos;
    br_vector3 world_pos;
    br_vector3 wv;
    br_vector3 wvw;
    br_scalar ts;
    static tS3_volume last_skid_vol[2];
    int i;

    i = IRandomBetween(0, 1);
    if (gCurrent_race.material_modifiers[material].tyre_noise_index == -1) {
        return;
    }
    if (IRandomBetween(0, 4) != 0) {
        return;
    }

    last_skid_vol[i] = pV * 10.0;
    if ((pWheel_num & 1) != 0) {
        pos.v[0] = pC->bounds[1].max.v[0];
    } else {
        pos.v[0] = pC->bounds[1].min.v[0];
    }
    pos.v[1] = pC->wpos[pWheel_num].v[1] - pC->oldd[pWheel_num];
    pos.v[2] = pC->wpos[pWheel_num].v[2];
    BrMatrix34ApplyP(&world_pos, &pos, &pC->car_master_actor->t.t.mat);
    BrVector3InvScale(&world_pos, &world_pos, WORLD_SCALE);
    if (!DRS3SoundStillPlaying(gSkid_tag[i]) || (pC->driver == eDriver_local_human && gLast_car_to_skid[i] != pC)) {
        gSkid_tag[i] = DRS3StartSound3D(
            gCar_outlet,
            IRandomBetween(0, 4) + 9000,
            &world_pos,
            &pC->velocity_bu_per_sec,
            1,
            last_skid_vol[i],
            IRandomBetween(49152, 81920),
            0x10000);
        gLast_car_to_skid[i] = pC;
    }
    if (gCurrent_race.material_modifiers[material].smoke_type == 1) {
        BrVector3Cross(&wv, &pC->omega, &pos);
        BrVector3Add(&wv, &wv, &pC->velocity_car_space);
        ts = -(BrVector3Dot(&wv, &pC->road_normal));
        BrVector3Scale(&wvw, &pC->road_normal, ts);
        BrVector3Add(&wv, &wv, &wvw);
        BrMatrix34ApplyV(&wvw, &wv, &pC->car_master_actor->t.t.mat);
        CreatePuffOfSmoke(&world_pos, &wvw, pV / 25.0, 1.0, 4, pC);
    }
}

// IDA: void __usercall StopSkid(tCar_spec *pC@<EAX>)
// FUNCTION: CARM95 0x00482458
void StopSkid(tCar_spec* pC) {

    if (gLast_car_to_skid[0] == pC) {
        DRS3StopSound(gSkid_tag[0]);
    }
    if (gLast_car_to_skid[1] == pC) {
        DRS3StopSound(gSkid_tag[1]);
    }
}

// IDA: void __usercall CrashNoise(br_vector3 *pForce@<EAX>, br_vector3 *position@<EDX>, int material@<EBX>)
// FUNCTION: CARM95 0x0048249d
void CrashNoise(br_vector3* pForce, br_vector3* position, int material) {
    // GLOBAL: CARM95 0x514e60
    static tS3_sound_tag crunch_tag;
    // GLOBAL: CARM95 0x53a540
    static tS3_volume last_crunch_vol;
    tS3_volume vol;
    br_vector3 velocity;

    vol = 60.f * BrVector3Length(pForce);
    if (gCurrent_race.material_modifiers[material].crash_noise_index != -1) {
        if (vol >= 256) {
            vol = 255;
        }
        if (crunch_tag == 0 || (!DRS3SoundStillPlaying(crunch_tag) && vol > 30)) {
            last_crunch_vol = vol;
            (void)last_crunch_vol;
            BrVector3Set(&velocity, 0.f, 0.f, 0.f);
            crunch_tag = DRS3StartSound3D(gCar_outlet,
                gMetal_crunch_sound_id__car[IRandomBetween(0, COUNT_OF(gMetal_crunch_sound_id__car) - 1)],
                position, &velocity, 1, vol, IRandomBetween(49152, 81920), 0x10000);
        }
    }
}

// IDA: void __usercall CrushAndDamageCar(tCar_spec *c@<EAX>, br_vector3 *pPosition@<EDX>, br_vector3 *pForce_car_space@<EBX>, tCar_spec *car2@<ECX>)
// FUNCTION: CARM95 0x004825a8
void CrushAndDamageCar(tCar_spec* c, br_vector3* pPosition, br_vector3* pForce_car_space, tCar_spec* car2) {
    br_vector3 force;
    br_vector3 force2;
    br_vector3 position;
    br_vector3 pos_w;
    br_vector3 car_to_cam;
    br_vector3 force_for_bodywork;
    br_scalar ts;
    int i;
    br_matrix34 m;
    br_scalar fudge_multiplier;

    if (car2 != NULL) {
        car2->who_last_hit_me = c;
        c->who_last_hit_me = car2;
    }

    if (c->driver == eDriver_non_car_unused_slot || c->driver == eDriver_non_car) {
        return;
    }
    fudge_multiplier = gNet_mode == eNet_mode_none || gNet_softness[gCurrent_net_game->type] == 1.0f ? 1.0f : gNet_softness[gCurrent_net_game->type];
    BrVector3Sub(&car_to_cam, &c->pos, (br_vector3*)gCamera_to_world.m[3]);
    ts = BrVector3LengthSquared(&car_to_cam);
    if (c->driver == eDriver_oppo && ts > 200.0f) {
        return;
    }
    if (car2 != NULL) {
        if (car2->driver > eDriver_non_car) {
            TwoCarsHitEachOther(c, car2);
        }
        if (c->driver >= eDriver_net_human) {
            fudge_multiplier = gDefensive_powerup_factor[c->power_up_levels[0]] * 1.2f * fudge_multiplier;
        }
        if (car2->driver >= eDriver_net_human) {
            if (gNet_mode != eNet_mode_none
                && (gCurrent_net_game->type == eNet_game_type_fight_to_death || gCurrent_net_game->type == eNet_game_type_car_crusher)) {
                fudge_multiplier = gOffensive_powerup_factor[car2->power_up_levels[2]] * gNet_offensive[gCurrent_net_game->type] * car2->damage_multiplier * fudge_multiplier;
            } else {
                fudge_multiplier = gOffensive_powerup_factor[car2->power_up_levels[2]] * car2->damage_multiplier * fudge_multiplier;
            }
        }
        if (c->driver == eDriver_oppo && car2->driver == eDriver_oppo) {
            fudge_multiplier = fudge_multiplier * 0.2f;
        }
        if (car2->driver <= eDriver_non_car) {
            car2 = NULL;
        } else {
            fudge_multiplier /= ((car2->car_model_actors[car2->principal_car_actor].crush_data.softness_factor + 0.7f) / 0.7f);
        }
    }
    BrVector3InvScale(&position, pPosition, WORLD_SCALE);
    BrVector3Scale(&force, pForce_car_space, fudge_multiplier * 0.03f);
    ts = BrVector3LengthSquared(&force);
    if (c->driver <= eDriver_non_car || !c->invulnerable) {
        c->damage_magnitude_accumulator += ts;
    }
    if (c->driver < eDriver_net_human) {
        BrVector3Scale(&force_for_bodywork, &force, 1.5f);
    } else {
        if (c->collision_mass_multiplier != 1.0) {
            BrVector3InvScale(&force, &force, c->collision_mass_multiplier);
        }
        BrVector3Scale(&force_for_bodywork, &force, 0.5f);
        if (c->driver == eDriver_local_human) {
            DoPratcamHit(&force);
        }
    }
    if (gNet_mode == eNet_mode_host && (gCurrent_net_game->type == eNet_game_type_tag || gCurrent_net_game->type == eNet_game_type_foxy) && car2 != NULL
        && c->driver >= eDriver_net_human && car2->driver >= eDriver_net_human) {
        if (gNet_players[gIt_or_fox].car == c && car2->knackered) {
            CarInContactWithItOrFox(NetPlayerFromCar(car2));
        } else if (gNet_players[gIt_or_fox].car == car2 && !c->knackered) {
            CarInContactWithItOrFox(NetPlayerFromCar(c));
        }
    }
    if (gNet_mode != eNet_mode_client || car2 == NULL) {
        DamageSystems(c, &position, &force, car2 != NULL);
    }
    if (c->driver <= eDriver_non_car || !c->invulnerable) {
        for (i = 0; i < c->car_actor_count; i++) {
            if (c->car_model_actors[i].min_distance_squared != -1.0f || (pForce_car_space->v[1] >= 0.0f && pForce_car_space->v[2] >= 0.0f)) {
                CrushModel(c, i, c->car_model_actors[i].actor, &position, &force_for_bodywork, &c->car_model_actors[i].crush_data);
            }
        }
        if (car2 && car2->driver == eDriver_local_human && ts > 0.003f) {
            PipeSingleCarIncident(ts, c, &position);
        }
        if (!car2 && c->driver == eDriver_local_human && ts > 0.003f) {
            BrMatrix34Copy(&m, &c->car_master_actor->t.t.mat);
            m.m[3][0] /= WORLD_SCALE;
            m.m[3][1] /= WORLD_SCALE;
            m.m[3][2] /= WORLD_SCALE;
            BrMatrix34ApplyP(&pos_w, &position, &m);
            PipeSingleWallIncident(ts, &pos_w);
        }
    }
    if (car2 != NULL && car2->driver == eDriver_local_human && ts > 0.003f) {
        PipeSingleCarIncident(ts, c, &position);
    }
    if (car2 == NULL && c->driver == eDriver_local_human && ts > 0.003f) {
        BrMatrix34Copy(&m, &c->car_master_actor->t.t.mat);
        m.m[3][0] /= WORLD_SCALE;
        m.m[3][1] /= WORLD_SCALE;
        m.m[3][2] /= WORLD_SCALE;
        BrMatrix34ApplyP(&pos_w, &position, &m);
        PipeSingleWallIncident(ts, &pos_w);
    }
}

// IDA: int __usercall ExpandBoundingBox@<EAX>(tCar_spec *c@<EAX>)
// FUNCTION: CARM95 0x00482c00
int ExpandBoundingBox(tCar_spec* c) {
    br_scalar min_z;
    br_scalar max_z;
    br_scalar dist;
    br_vector3 tv;
    br_vector3 old_pos;
    int l;
    br_matrix34 mat;

    l = 0;
    min_z = c->bounds[1].min.v[2];
    max_z = c->bounds[1].max.v[2];
    old_pos = *(br_vector3*)&c->oldmat.m[3][0];
    CrushBoundingBox(c, 0);
    for (l = 0; l < 5; l++) {
        if (TestForCarInSensiblePlace(c)) {
            break;
        }
        if (c->old_point.v[2] <= 0.0f) {
            dist = min_z - c->bounds[1].min.v[2];
        } else {
            dist = c->bounds[1].max.v[2] - max_z;
        }
        if (dist >= 0.0f) {
            dist += 0.005f;
            BrVector3Scale(&c->old_norm, &c->old_norm, dist);
            BrMatrix34ApplyV(&tv, &c->old_norm, &c->car_master_actor->t.t.mat);
            c->oldmat.m[3][0] += tv.v[0];
            c->oldmat.m[3][1] += tv.v[1];
            c->oldmat.m[3][2] += tv.v[2];
            l++;
        } else {
            l = 5;
        }
    }
    if (l < 5) {
        return 1;
    }
    *(br_vector3*)&c->oldmat.m[3][0] = old_pos;
    c->bounds[1].min.v[2] = min_z;
    c->bounds[1].max.v[2] = max_z;
    if (c->driver == eDriver_local_human) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_RepairObstructed));
    }
    return 0;
}

// IDA: void __usercall CrushBoundingBox(tCar_spec *c@<EAX>, int crush_only@<EDX>)
// FUNCTION: CARM95 0x00482dee
void CrushBoundingBox(tCar_spec* c, int crush_only) {
    br_vector3 min;
    br_vector3 max;
    int i;
    br_actor* actor;

    if (c == NULL) {
        return;
    }
    actor = c->car_model_actors[c->principal_car_actor].actor;
    max.v[0] = c->wpos[2].v[2] - c->non_driven_wheels_circum / 6.2f;
    min.v[0] = c->driven_wheels_circum / 6.2f + c->wpos[0].v[2];
    max.v[0] /= WORLD_SCALE;
    min.v[0] /= WORLD_SCALE;
    for (i = 0; i < actor->model->nvertices; i++) {
        if (actor->model->vertices[i].p.v[2] < max.v[0]) {
            max.v[0] = actor->model->vertices[i].p.v[2];
        }
        if (actor->model->vertices[i].p.v[2] > min.v[0]) {
            min.v[0] = actor->model->vertices[i].p.v[2];
        }
    }
    max.v[0] *= WORLD_SCALE;
    min.v[0] *= WORLD_SCALE;
    if (crush_only) {
        if (c->bounds[1].min.v[2] > max.v[0]) {
            max.v[0] = c->bounds[1].min.v[2];
        }
        if (c->bounds[1].max.v[2] < min.v[0]) {
            min.v[0] = c->bounds[1].max.v[2];
        }
    } else {
        if (c->max_bounds[1].min.v[2] > max.v[0]) {
            max.v[0] = c->max_bounds[1].min.v[2];
        }
        if (c->max_bounds[1].max.v[2] < min.v[0]) {
            min.v[0] = c->max_bounds[1].max.v[2];
        }
    }
    c->bounds[1].min.v[2] = max.v[0];
    c->bounds[1].max.v[2] = min.v[0];
    for (i = 0; i < c->extra_point_num; i++) {
        if (c->max_bounds[1].max.v[2] + 0.01f >= c->original_extra_points_z[i] && c->max_bounds[1].min.v[2] - 0.01f <= c->original_extra_points_z[i]) {
            if (c->original_extra_points_z[i] > min.v[0]) {
                c->extra_points[i].v[2] = min.v[0];
            } else if (c->original_extra_points_z[i] >= max.v[0]) {
                c->extra_points[i].v[2] = c->original_extra_points_z[i];
            } else {
                c->extra_points[i].v[2] = max.v[0];
            }
            if (c->extra_points[i].v[2] > min.v[0]) {
                c->extra_points[i].v[2] = min.v[0];
            }
            if (c->extra_points[i].v[2] < max.v[0]) {
                c->extra_points[i].v[2] = max.v[0];
            }
        } else {
            c->extra_points[i].v[2] = c->original_extra_points_z[i];
        }
    }
}

// IDA: void __cdecl AddCollPoint(br_scalar dist, br_vector3 *p, br_vector3 *norm, br_vector3 *r, br_vector3 *n, br_vector3 *dir, int num, tCollision_info *c)
// FUNCTION: CARM95 0x00483152
void AddCollPoint(br_scalar dist, br_vector3* p, br_vector3* norm, br_vector3* r, br_vector3* n, br_vector3* dir, int num, tCollision_info* c) {
    static br_scalar d[4];
    int i;
    int furthest;

    if (num < 4) {
        d[num] = dist;
        n[num] = *norm;
        BrVector3Sub(&r[num], p, &c->cmpos);
        return;
    }
    furthest = 0;
    for (i = 1; i < 4; i++) {
        if (d[furthest] < d[i]) {
            furthest = i;
        }
    }
    if (d[furthest] >= dist) {
        num = furthest;
        d[num] = dist;
        n[num] = *norm;
        BrVector3Sub(&r[num], p, &c->cmpos);
    }
}

// IDA: br_scalar __usercall SinglePointColl@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>)
// FUNCTION: CARM95 0x0048326d
br_scalar SinglePointColl(br_scalar* f, br_matrix4* m, br_scalar* d) {

    f[0] = d[0] / m->m[0][0];
    if (f[0] < 0.0f) {
        f[0] = 0.f;
    }
    return fabs(m->m[0][0]);
}

// IDA: br_scalar __usercall TwoPointColl@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n)
// FUNCTION: CARM95 0x004832b2
br_scalar TwoPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    br_scalar ts;

    ts = m->m[1][1] * m->m[0][0] - m->m[0][1] * m->m[1][0];

    if (fabs(ts) >= 0.000001f) {
        f[0] = (m->m[1][1] * d[0] - m->m[0][1] * d[1]) / ts;
        f[1] = (m->m[1][0] * d[0] - m->m[0][0] * d[1]) / -ts;
    }
    if (f[1] < 0.0f || fabs(ts) < 0.000001f) {
        ts = SinglePointColl(f, m, d);
        f[1] = 0.0f;
    } else if (f[0] < 0.0f) {
        m->m[0][0] = m->m[1][1];
        tau[0] = tau[1];
        n[0] = n[1];
        d[0] = d[1];
        ts = SinglePointColl(f, m, d);
        f[1] = 0.0;
    }
    return fabs(ts);
}

// IDA: br_scalar __usercall DrMatrix4Inverse@<ST0>(br_matrix4 *mi@<EAX>, br_matrix4 *mc@<EDX>)
// FUNCTION: CARM95 0x0048377f
br_scalar DrMatrix4Inverse(br_matrix4* mi, br_matrix4* mc) {

    return BrMatrix4Inverse(mi, mc);
}

// IDA: br_scalar __usercall ThreePointColl@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>)
// FUNCTION: CARM95 0x004836f1
br_scalar ThreePointColl(br_scalar* f, br_matrix4* m, br_scalar* d) {
    br_matrix4 mc;
    br_matrix4 mi;
    br_scalar ts;

    BrMatrix4Copy(&mc, m);
    memset(&mc.m[2][3], 0, 16);
    mc.m[1][3] = 0.0f;
    mc.m[0][3] = 0.0f;
    mc.m[3][3] = 1.0f;
    ts = DrMatrix4Inverse(&mi, &mc);
    BrMatrix4TApply((br_vector4*)f, (br_vector4*)d, &mi);
    f[3] = 0.0f;
    return fabs(ts);
}

// IDA: br_scalar __usercall ThreePointCollRec@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n, tCollision_info *c)
// FUNCTION: CARM95 0x00483425
br_scalar ThreePointCollRec(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c) {
    int i;
    int j;
    br_scalar ts;

    ts = ThreePointColl(f, m, d);
    if (f[0] >= 0.0f && f[1] >= 0.0f && f[2] >= 0.0f && ts >= 0.000001f) {
        c->infinite_mass = 256;
        return ts;
    }
    if (ts < 0.000001f) {
        i = 0;
        j = 1;
    } else if (f[0] < 0.0f) {
        i = 1;
        j = 2;
    } else if (f[1] < 0.0f) {
        i = 0;
        j = 2;
    } else if (f[2] < 0.0f) {
        i = 0;
        j = 1;
    } else {
        return 0.0f;
    }
    m->m[0][0] = ((float*)m->m)[5 * i];
    m->m[1][0] = m->m[j][i];
    m->m[0][1] = m->m[i][j];
    m->m[1][1] = ((float*)m->m)[5 * j];
    tau[0] = tau[i];
    tau[1] = tau[j];
    n[0] = n[i];
    n[1] = n[j];
    d[0] = d[i];
    d[1] = d[j];
    ts = TwoPointColl(f, m, d, tau, n);
    f[2] = 0.0f;
    return ts;
}

// IDA: br_scalar __usercall FourPointColl@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n, tCollision_info *c)
// FUNCTION: CARM95 0x0048379f
br_scalar FourPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c) {
    int i;
    int j;
    int l;
    br_scalar ts;

    ts = ThreePointColl(f, m, d);
    if (f[0] < 0.0 || f[1] < 0.0 || f[2] < 0.0 || ts < 0.000001) {
        if (ts < 0.000001) {
            j = 3;
        } else if (f[0] < 0.0) {
            j = 0;
        } else if (f[1] >= 0.0) {
            j = 2;
        } else {
            j = 1;
        }
        for (i = j; i < 3; ++i) {
            for (l = 0; l < 4; ++l) {
                m->m[i][l] = m->m[i + 1][l];
            }
            d[i] = d[i + 1];
            tau[i] = tau[i + 1];
            n[i] = n[i + 1];
            d[i] = d[i + 1];
        }
        for (i = j; i < 3; ++i) {
            for (l = 0; l < 3; ++l) {
                m->m[l][i] = m->m[l][i + 1];
            }
        }
        return ThreePointCollRec(f, m, d, tau, n, c);
    } else {
        c->infinite_mass = 256;
        return ts;
    }
}

// IDA: void __usercall MultiFindFloorInBoxM(int pNum_rays@<EAX>, br_vector3 *a@<EDX>, br_vector3 *b@<EBX>, br_vector3 *nor@<ECX>, br_scalar *d, tCar_spec *c, int *mat_ref)
// FUNCTION: CARM95 0x00483a49
void MultiFindFloorInBoxM(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref) {
    br_vector3 aa[4];
    br_vector3 bb;
    int i;

    for (i = 0; i < pNum_rays; i++) {
        aa[i].v[0] = a[i].v[0] / WORLD_SCALE;
        aa[i].v[1] = a[i].v[1] / WORLD_SCALE;
        aa[i].v[2] = a[i].v[2] / WORLD_SCALE;
        d[i] = 2.0;
    }
    bb.v[0] = b->v[0] / WORLD_SCALE;
    bb.v[1] = b->v[1] / WORLD_SCALE;
    bb.v[2] = b->v[2] / WORLD_SCALE;
    MultiFindFloorInBoxBU(pNum_rays, aa, &bb, nor, d, c, mat_ref);
}

// IDA: void __usercall MultiFindFloorInBoxBU(int pNum_rays@<EAX>, br_vector3 *a@<EDX>, br_vector3 *b@<EBX>, br_vector3 *nor@<ECX>, br_scalar *d, tCar_spec *c, int *mat_ref)
// FUNCTION: CARM95 0x00483b2a
void MultiFindFloorInBoxBU(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref) {
    br_vector3 nor2;
    int i;
    int j;
    int l;
    br_scalar dist[4];
    tFace_ref* face_ref;

    for (i = c->box_face_start; i < c->box_face_end; i++) {
        face_ref = &gFace_list__car[i];
        if (!gEliminate_faces || (face_ref->flags & 0x80) == 0x0) {
            MultiRayCheckSingleFace(pNum_rays, face_ref, a, b, &nor2, dist);
            for (j = 0; j < pNum_rays; ++j) {
                if (d[j] > dist[j]) {
                    d[j] = dist[j];
                    nor[j] = nor2;
                    l = *gFace_list__car[i].material->identifier - 47;
                    if (l >= 0 && l < 11) {
                        mat_ref[j] = l;
                    }
                }
            }
        }
    }
}

// IDA: void __usercall findfloor(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>)
// FUNCTION: CARM95 0x00483c6a
void findfloor(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d) {
    br_material* material;
    br_vector3 aa;
    br_vector3 bb;

    BrVector3InvScale(&aa, a, WORLD_SCALE);
    BrVector3InvScale(&bb, b, WORLD_SCALE);
    FindFace(&aa, &bb, nor, d, &material);
}

// IDA: int __usercall FindFloorInBoxM@<EAX>(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>, tCollision_info *c)
// FUNCTION: CARM95 0x00483cec
int FindFloorInBoxM(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 aa;
    br_vector3 bb;

    aa.v[0] = a->v[0] / WORLD_SCALE;
    aa.v[1] = a->v[1] / WORLD_SCALE;
    aa.v[2] = a->v[2] / WORLD_SCALE;
    bb.v[0] = b->v[0] / WORLD_SCALE;
    bb.v[1] = b->v[1] / WORLD_SCALE;
    bb.v[2] = b->v[2] / WORLD_SCALE;
    return FindFloorInBoxBU(&aa, &bb, nor, d, c);
}

// IDA: int __usercall FindFloorInBoxBU@<EAX>(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>, tCollision_info *c)
// FUNCTION: CARM95 0x00483d73
int FindFloorInBoxBU(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 nor2;
    int i;
    int j;
    br_scalar dist;
    tFace_ref* face_ref;

#if defined(DETHRACE_FIX_BUGS)
    j = 0; // added to keep compiler happy
#endif
    *d = 2.0;
    for (i = c->box_face_start; i < c->box_face_end; i++) {
        face_ref = &gFace_list__car[i];
        if (!gEliminate_faces || SLOBYTE(face_ref->flags) >= 0) {
            CheckSingleFace(face_ref, a, b, &nor2, &dist);
            if (*d > dist) {
                *d = dist;
                j = i;
                BrVector3Copy(nor, &nor2);
            }
        }
    }
    if (*d >= 2.f) {
        return 0;
    }
    i = gFace_list__car[j].material->identifier[0] - ('0' - 1);
    if (i < 0 || i >= 11) {
        return 0;
    } else {
        return i;
    }
}

// IDA: int __usercall FindFloorInBoxBU2@<EAX>(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>, tCollision_info *c)
// FUNCTION: CARM95 0x00483e92
int FindFloorInBoxBU2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 nor2;
    br_vector3 tv;
    int i;
    int j;
    br_scalar dist;
    tFace_ref* face_ref;

#if defined(DETHRACE_FIX_BUGS)
    j = 0; // added to keep compiler happy
#endif
    *d = 2.f;
    for (i = c->box_face_start; i < c->box_face_end; i++) {
        face_ref = &gFace_list__car[i];
        if (!gEliminate_faces || SLOBYTE(face_ref->flags) >= 0) {
            CheckSingleFace(face_ref, a, b, &nor2, &dist);
            if (*d > dist) {
                if (face_ref->material->user == DOUBLESIDED_USER_FLAG || (face_ref->material->flags & (BR_MATF_ALWAYS_VISIBLE | BR_MATF_TWO_SIDED)) != 0) {
                    BrVector3Sub(&tv, &c->pos, a);
                    if (BrVector3Dot(&tv, &nor2) >= 0.f) {
                        *d = dist;
                        j = i;
                        BrVector3Copy(nor, &nor2);
                    }
                } else {
                    *d = dist;
                    j = i;
                    BrVector3Copy(nor, &nor2);
                }
            }
        }
        face_ref++;
    }
    if (*d >= 2.f) {
        return 0;
    }
    i = gFace_list__car[j].material->identifier[0] - ('0' - 1);
    if (i < 0 || i >= 11) {
        return 0;
    } else {
        return i;
    }
}

// IDA: int __usercall FindFloorInBoxM2@<EAX>(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>, tCollision_info *c)
// FUNCTION: CARM95 0x0048405b
int FindFloorInBoxM2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 aa;
    br_vector3 bb;

    aa.v[0] = a->v[0] / WORLD_SCALE;
    aa.v[1] = a->v[1] / WORLD_SCALE;
    aa.v[2] = a->v[2] / WORLD_SCALE;
    bb.v[0] = b->v[0] / WORLD_SCALE;
    bb.v[1] = b->v[1] / WORLD_SCALE;
    bb.v[2] = b->v[2] / WORLD_SCALE;
    return FindFloorInBoxBU2(&aa, &bb, nor, d, c);
}

// IDA: int __usercall BoxFaceIntersect@<EAX>(br_bounds *pB@<EAX>, br_matrix34 *pM@<EDX>, br_matrix34 *pMold@<EBX>, br_vector3 *pPoint_list@<ECX>, br_vector3 *pNorm_list, br_scalar *pDist_list, int pMax_pnts, tCollision_info *c)
// FUNCTION: CARM95 0x004840e2
int BoxFaceIntersect(br_bounds* pB, br_matrix34* pM, br_matrix34* pMold, br_vector3* pPoint_list, br_vector3* pNorm_list, br_scalar* pDist_list, int pMax_pnts, tCollision_info* c) {
    br_vector3 p[3];
    br_vector3 tv;
    br_vector3 pos;
    br_bounds bnds;
    int i;
    int j;
    int n;
    int flag;
    int m;
    tFace_ref* f_ref;
    br_face* face;

    n = 0;
    BrVector3InvScale(&bnds.min, &pB->min, WORLD_SCALE);
    BrVector3InvScale(&bnds.max, &pB->max, WORLD_SCALE);
    BrVector3InvScale(&pos, (br_vector3*)pM->m[3], WORLD_SCALE);
    BrVector3InvScale((br_vector3*)pMold->m[3], (br_vector3*)pMold->m[3], WORLD_SCALE);

    for (i = c->box_face_start; i < c->box_face_end && i < c->box_face_start + 50; i++) {
        f_ref = &gFace_list__car[i];
        if (SLOBYTE(f_ref->flags) >= 0 && f_ref->material->identifier[0] != '!') {
            BrVector3Sub(&tv, &f_ref->v[0], &pos);
            BrMatrix34TApplyV(&p[0], &tv, pM);
            BrVector3Sub(&tv, &f_ref->v[1], &pos);
            BrMatrix34TApplyV(&p[1], &tv, pM);
            BrVector3Sub(&tv, &f_ref->v[2], &pos);
            BrMatrix34TApplyV(&p[2], &tv, pM);
            j = n;
            if ((f_ref->flags & 1) == 0) {
                n += AddEdgeCollPoints(&p[0], &p[1], &bnds, pMold, pPoint_list, pNorm_list, n, pMax_pnts, c);
            }
            if ((f_ref->flags & 2) == 0) {
                n += AddEdgeCollPoints(&p[1], &p[2], &bnds, pMold, pPoint_list, pNorm_list, n, pMax_pnts, c);
            }
            if ((f_ref->flags & 4) == 0) {
                n += AddEdgeCollPoints(&p[2], &p[0], &bnds, pMold, pPoint_list, pNorm_list, n, pMax_pnts, c);
            }
            if (n > j) {
                if (gMaterial_index == 0) {
                    m = f_ref->material->identifier[0] - '/';
                    if (m > 0 && m < 11) {
                        gMaterial_index = m;
                    }
                }
                while (j < n) {
                    BrVector3Scale(&pPoint_list[j], &pPoint_list[j], WORLD_SCALE);
                    BrVector3Sub(&pPoint_list[j], &pPoint_list[j], &c->cmpos);
                    j++;
                }
            }
        }
    }
    if (n) {
        m = 0;
        for (i = 0; i < n - 1; i++) {
            flag = 1;
            for (j = i + 1; j < n; j++) {
                if (fabs(pPoint_list[i].v[0] - pPoint_list[j].v[0]) <= 0.001f
                    && fabs(pPoint_list[i].v[1] - pPoint_list[j].v[1]) <= 0.001f
                    && fabs(pPoint_list[i].v[2] - pPoint_list[j].v[2]) <= 0.001f) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {
                BrVector3Copy(&pPoint_list[m], &pPoint_list[i]);
                m++;
            }
        }
        BrVector3Copy(&pPoint_list[m], &pPoint_list[n - 1]);
        n = m + 1;
    }
    BrVector3Scale((br_vector3*)pMold->m[3], (br_vector3*)pMold->m[3], WORLD_SCALE);
    return n;
}

// IDA: int __usercall AddEdgeCollPoints@<EAX>(br_vector3 *p1@<EAX>, br_vector3 *p2@<EDX>, br_bounds *pB@<EBX>, br_matrix34 *pMold@<ECX>, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int n, int pMax_pnts, tCollision_info *c)
// FUNCTION: CARM95 0x0048469b
int AddEdgeCollPoints(br_vector3* p1, br_vector3* p2, br_bounds* pB, br_matrix34* pMold, br_vector3* pPoint_list, br_vector3* pNorm_list, int n, int pMax_pnts, tCollision_info* c) {
    br_vector3 op1;
    br_vector3 op2;
    br_vector3 a;
    br_vector3 b;
    br_vector3 edge;
    br_vector3 hp1;
    br_vector3 hp2;
    br_vector3 hp3;
    int plane1;
    int plane2;
    int plane3;
    int d;

    float scale;

    plane1 = LineBoxColl(p1, p2, pB, &hp1);
    if (plane1 == 0) {
        return 0;
    }
    if (n + 2 > pMax_pnts) {
        return 0;
    }
    plane2 = LineBoxColl(p2, p1, pB, &hp2);
    if (plane2 == 0) {
        return 0;
    }
    if (plane1 != 8 && plane2 != 8 && (plane1 ^ plane2) == 4) {
        BrVector3Add(&op1, &hp2, &hp1);
        BrVector3Scale(&op1, &op1, .5f);
        BrMatrix34ApplyP(&op2, &op1, pMold);
        plane3 = LineBoxColl(&op2, &op1, pB, &hp3);
        if (plane3 == 8) {
            return 0;
        }
        GetBoundsEdge(&pPoint_list[n], &edge, pB, plane1, plane3, &op2, &hp1, &hp2, c->collision_flag);
        GetBoundsEdge(&pPoint_list[n + 1], &edge, pB, plane2, plane3, &op2, &hp1, &hp2, c->collision_flag);
        GetPlaneNormal(&pNorm_list[n], plane3);
        BrVector3Copy(&pNorm_list[n + 1], &pNorm_list[n]);
        return 2;
    } else if (plane1 == 8 && plane2 != 8) {
        BrMatrix34ApplyP(&a, p1, pMold);
        plane3 = LineBoxColl(&a, p1, pB, &hp3);
        if (plane3 == 8) {
            return 0;
        }
        BrVector3Copy(&pPoint_list[n], &hp3);
        GetPlaneNormal(&pNorm_list[n], plane2);
        if (plane2 == plane3 || (plane3 ^ plane2) == 4) {
            return 1;
        }
        GetBoundsEdge(&pPoint_list[n + 1], &edge, pB, plane2, plane3, p1, &hp2, &hp3, c->collision_flag);
        BrVector3Sub(&op1, p1, p2);
        BrVector3Cross(&pNorm_list[n + 1], &edge, &op1);
        BrVector3Normalise(&pNorm_list[n + 1], &pNorm_list[n + 1]);
        d = (plane2 - 1) & 3;
        if ((pNorm_list[n + 1].v[d] < 0.f) == (plane2 & 4) >> 2) {
            BrVector3Negate(&pNorm_list[n + 1], &pNorm_list[n + 1]);
        }
        BrVector3Copy(&op1, &pNorm_list[n + 1]);
        BrMatrix34ApplyV(&pNorm_list[n + 1], &op1, pMold);
        return 2;
    } else if (plane2 == 8 && plane1 != 8) {
        BrMatrix34ApplyP(&b, p2, pMold);
        plane3 = LineBoxColl(&b, p2, pB, &hp3);
        if (plane3 == 8) {
            return 0;
        }
        pPoint_list[n] = hp3;
        GetPlaneNormal(&pNorm_list[n], plane1);
        if (plane1 == plane3 || (plane3 ^ plane1) == 4) {
            return 1;
        }
        GetBoundsEdge(&pPoint_list[n + 1], &edge, pB, plane1, plane3, p2, &hp1, &hp3, c->collision_flag);
        BrVector3Sub(&op1, p1, p2);
        BrVector3Cross(&pNorm_list[n + 1], &edge, &op1);
        BrVector3Normalise(&pNorm_list[n + 1], &pNorm_list[n + 1]);
        d = (plane1 - 1) & 3;
        if ((pNorm_list[n + 1].v[d] < 0.f) == (plane1 & 4) >> 2) {
            BrVector3Negate(&pNorm_list[n + 1], &pNorm_list[n + 1]);
        }
        BrVector3Copy(&op1, &pNorm_list[n + 1]);
        BrMatrix34ApplyV(&pNorm_list[n + 1], &op1, pMold);
        return 2;
    } else if (plane1 != 8 && plane2 != 8) {
        BrVector3Add(&op1, &hp2, &hp1);
        BrVector3Scale(&op1, &op1, .5f);
        BrMatrix34ApplyP(&op2, &op1, pMold);
        plane3 = LineBoxColl(&op2, &op1, pB, &hp3);
        if (plane3 == 8 || plane3 == 0) {
            return 0;
        }
        if (plane1 == plane3 || plane2 == plane3) {
            GetBoundsEdge(&pPoint_list[n], &edge, pB, plane1, plane2, &op2, &hp1, &hp2, c->collision_flag);
            BrVector3Sub(&op1, &hp1, &hp2);
            BrVector3Cross(&op2, &edge, &op1);
            BrVector3Normalise(&pNorm_list[n], &op2);
            BrVector3Add(&op1, &pB->max, &pB->min);
            BrVector3Scale(&op1, &op1, .5f);
            BrVector3Sub(&op1, &pPoint_list[n], &op1);
            if (BrVector3Dot(&pNorm_list[n], &op1) > 0.f) {
                BrVector3Negate(&pNorm_list[n], &pNorm_list[n]);
            }
            BrVector3Copy(&op1, &pNorm_list[n]);
            BrMatrix34ApplyV(&pNorm_list[n], &op1, pMold);
            return 1;
        } else {
            GetBoundsEdge(&pPoint_list[n], &edge, pB, plane1, plane3, &hp3, &hp1, &hp2, c->collision_flag);
            GetBoundsEdge(&pPoint_list[n + 1], &edge, pB, plane2, plane3, &hp3, &hp1, &hp2, c->collision_flag);
            GetPlaneNormal(&pNorm_list[n], plane3);
            BrVector3Copy(&pNorm_list[n + 1], &pNorm_list[n]);
            return 2;
        }
    } else if (plane1 == 8 && plane2 == 8) {
        BrMatrix34ApplyP(&op1, p1, pMold);
        plane3 = LineBoxColl(&op1, p1, pB, &pPoint_list[n]);
        GetPlaneNormal(&pNorm_list[n], plane3);
        d = n + (plane3 != 8);
        BrMatrix34ApplyP(&op1, p2, pMold);
        plane3 = LineBoxColl(&op1, p2, pB, &pPoint_list[d]);
        GetPlaneNormal(&pNorm_list[d], plane3);
        return (n != d) + (plane3 != 8);
    } else {
        return 0;
    }
}

// IDA: void __usercall GetPlaneNormal(br_vector3 *n@<EAX>, int p@<EDX>)
// FUNCTION: CARM95 0x004854c1
void GetPlaneNormal(br_vector3* n, int p) {
    int d;

    d = (p - 1) & 3;
    BrVector3Set(n, 0.f, 0.f, 0.f);
    if ((p & 4) != 0) {
        n->v[d] = 1.0f;
    } else {
        n->v[d] = -1.0f;
    }
}

// IDA: int __usercall GetBoundsEdge@<EAX>(br_vector3 *pos@<EAX>, br_vector3 *edge@<EDX>, br_bounds *pB@<EBX>, int plane1@<ECX>, int plane2, br_vector3 *a, br_vector3 *b, br_vector3 *c, int flag)
// FUNCTION: CARM95 0x0048551f
int GetBoundsEdge(br_vector3* pos, br_vector3* edge, br_bounds* pB, int plane1, int plane2, br_vector3* a, br_vector3* b, br_vector3* c, int flag) {
    int d1;
    int d2;
    int d3;
    br_vector3 n;
    br_vector3 p;
    br_vector3 q;

    d1 = (plane1 - 1) & 3;
    d2 = (plane2 - 1) & 3;
    BrVector3Sub(&n, b, a);
    BrVector3Sub(&p, c, a);
    BrVector3Cross(&q, &n, &p);
    if ((plane1 & 4) != 0) {
        pos->v[d1] = pB->min.v[d1];
    } else {
        pos->v[d1] = pB->max.v[d1];
    }
    if ((plane2 & 4) != 0) {
        pos->v[d2] = pB->min.v[d2];
    } else {
        pos->v[d2] = pB->max.v[d2];
    }
    d3 = 3 - d1 - d2;
    edge->v[d1] = 0.f;
    edge->v[d2] = 0.f;
    edge->v[d3] = 1.f;
    if ((flag & 1) != 0) {
        pos->v[d3] = (c->v[d3] + b->v[d3]) / 2.f;
    } else {
        pos->v[d3] = a->v[d3] - ((pos->v[d2] - a->v[d2]) * q.v[d2] + (pos->v[d1] - a->v[d1]) * q.v[d1]) / q.v[d3];
    }
    return 1;
}

// IDA: void __usercall oldMoveOurCar(tU32 pTime_difference@<EAX>)
// FUNCTION: CARM95 0x00485bea
void oldMoveOurCar(tU32 pTime_difference) {
    br_vector3 thrust_vector;
    br_matrix34 direction_matrix;
    br_matrix34 old_mat;
    double rotate_amount;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    br_scalar speed;
    int below_face_index;
    int above_face_index;
    br_model* below_model;
    br_model* above_model;
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleCollisionDetection()
void ToggleCollisionDetection(void) {
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CancelPendingCunningStunt()
// FUNCTION: CARM95 0x00485d8b
void CancelPendingCunningStunt(void) {

    gQuite_wild_end = 0;
    gQuite_wild_start = 0;
    gOn_me_wheels_start = 0;
    gWoz_upside_down_at_all = 0;
    gWild_start = 0;
}

// IDA: float __cdecl frac(float pN)
// FUNCTION: CARM95 0x00485dc8
float frac(float pN) {

    return pN - (float)(int)pN;
}

// IDA: void __usercall SetAmbientPratCam(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x00486d06
void SetAmbientPratCam(tCar_spec* pCar) {
    br_scalar vcs_x;
    br_scalar vcs_y;
    br_scalar vcs_z;
    br_scalar abs_vcs_x;
    br_scalar abs_vcs_y;
    br_scalar abs_vcs_z;
    br_scalar abs_omega_x;
    br_scalar abs_omega_y;
    br_scalar abs_omega_z;
    tU32 the_time;
    // GLOBAL: CARM95 0x53a598
    static tU32 last_time_on_ground;

    if (gRace_finished) {
        return;
    }
    the_time = GetTotalTime();
    if (pCar->number_of_wheels_on_ground != 0) {
        last_time_on_ground = the_time;
    }
    vcs_x = pCar->velocity_car_space.v[0];
    vcs_y = pCar->velocity_car_space.v[1];
    vcs_z = pCar->velocity_car_space.v[2];
    abs_vcs_x = fabs(vcs_x);
    abs_vcs_y = fabs(vcs_y);
    abs_vcs_z = fabs(vcs_z);
    abs_omega_x = fabs(pCar->omega.v[0]);
    abs_omega_y = fabs(pCar->omega.v[1]);
    abs_omega_z = fabs(pCar->omega.v[2]);

    if (abs_omega_x > 4.5f || abs_omega_z > 4.5f) {
        ChangeAmbientPratcam(kPratcam_rolling_violent);
    } else if (abs_omega_y > 4.5f) {
        ChangeAmbientPratcam(kPratcam_spinning_violent);
    } else if (abs_omega_x > 3.f || abs_omega_z > 3.f) {
        ChangeAmbientPratcam(kPratcam_rolling_medium);
    } else if (abs_omega_y > 3.f) {
        ChangeAmbientPratcam(kPratcam_spinning_medium);
    } else if (pCar->car_master_actor->t.t.mat.m[1][1] < 0.1f) {
        ChangeAmbientPratcam(kPratcam_upside_down);
    } else if (abs_vcs_y > abs_vcs_z && abs_vcs_y > abs_vcs_x && vcs_y < -.004f) {
        ChangeAmbientPratcam(kPratcam_falling);
    } else if (the_time - last_time_on_ground > 500) {
        ChangeAmbientPratcam(kPratcam_flying);
    } else if (abs_vcs_x > abs_vcs_z && vcs_x > .001f) {
        ChangeAmbientPratcam(kPratcam_skidding_right);
    } else if (abs_vcs_x > abs_vcs_z && vcs_x < -.001f) {
        ChangeAmbientPratcam(kPratcam_skidding_left);
    } else if (abs_omega_x > 1.5f || abs_omega_z > 1.5f) {
        ChangeAmbientPratcam(kPratcam_rolling_gently);
    } else if (abs_omega_y > 1.5f) {
        ChangeAmbientPratcam(kPratcam_spinning_gently);
    } else if (abs_vcs_z > .01f) {
        ChangeAmbientPratcam(kPratcam_over_137mph);
    } else if (abs_vcs_z > .004f) {
        ChangeAmbientPratcam(kPratcam_between_67_and_167mph);
    } else if (abs_vcs_z > .0015f) {
        ChangeAmbientPratcam(kPratcam_between_25_and_67mph);
    } else {
        ChangeAmbientPratcam(kPratcam_stationary_or_below_25mph);
    }
}

// IDA: void __usercall MungeCarGraphics(tU32 pFrame_period@<EAX>)
// FUNCTION: CARM95 0x00485dee
void MungeCarGraphics(tU32 pFrame_period) {
    int i;
    int j;
    int update_mat;
    int spinning_wildly;
    int spinning_mildly;
    int car_count;
    int oily_count;
    int car;
    int cat;
    int new_special_screen;
    tCar_spec* the_car;
    br_scalar distance_from_camera;
    br_scalar car_x;
    br_scalar car_z;
    br_scalar oily_size;
    br_scalar car_radius;
    br_scalar abs_omega_x;
    br_scalar abs_omega_y;
    br_scalar abs_omega_z;
    float wheel_speed;
    float speed_mph;
    float rev_angle;
    float sine_angle;
    float raw_revs;
    float rev_reducer;
    tSpecial_screen* the_special_screen;
    br_material* the_material;
    tU32 the_time;
    br_actor* oily_actor;

    if (gNet_mode != eNet_mode_none
        && ((gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox)
            || (gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox))) {
        gProgram_state.current_car.power_up_levels[1] = 0;
    }
    SetAmbientPratCam(&gProgram_state.current_car);
    if (gProgram_state.cockpit_on) {
        SwitchCarActor(&gProgram_state.current_car, gProgram_state.current_car.car_actor_count - 1);
    } else {
        SwitchCarActor(&gProgram_state.current_car, gProgram_state.current_car.car_actor_count - 2);
    }

    the_time = PDGetTotalTime();
    for (cat = eVehicle_self; cat <= eVehicle_rozzer; cat++) {
        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (car = 0; car < car_count; car++) {
            if (cat == eVehicle_self) {
                the_car = &gProgram_state.current_car;
            } else {
                the_car = GetCarSpec(cat, car);
            }
            the_car->car_master_actor->render_style = (the_car->driver == eDriver_local_human || !PointOutOfSight(&the_car->pos, gYon_squared)) ? BR_RSTYLE_DEFAULT : BR_RSTYLE_NONE;
        }
    }
    for (car = 0; car < gNum_active_cars; car++) {
        the_car = gActive_car_list[car];
        if (the_car->car_master_actor->render_style != BR_RSTYLE_NONE) {
            car_x = the_car->car_master_actor->t.t.translate.t.v[0];
            car_z = the_car->car_master_actor->t.t.translate.t.v[2];
            the_car->shadow_intersection_flags = 0;
            oily_count = GetOilSpillCount();
            for (i = 0; i < oily_count; i++) {
                GetOilSpillDetails(i, &oily_actor, &oily_size);
                if (oily_actor != NULL) {
                    car_radius = the_car->bounds[1].max.v[2] / WORLD_SCALE * 1.5f;
                    if (oily_actor->t.t.translate.t.v[0] - oily_size < car_x + car_radius
                        && oily_actor->t.t.translate.t.v[0] + oily_size > car_x - car_radius
                        && oily_actor->t.t.translate.t.v[2] - oily_size < car_z + car_radius
                        && oily_actor->t.t.translate.t.v[2] + oily_size > car_z - car_radius) {
                        the_car->shadow_intersection_flags |= 1 << i;
                    }
                }
            }
            if (the_car->driver < eDriver_net_human && (!gAction_replay_mode || !ReplayIsPaused())) {
                if (gCountdown) {
                    sine_angle = FRandomBetween(0.4f, 1.6f) * ((double)GetTotalTime() / ((double)gCountdown * 100.0f));
                    sine_angle = frac(sine_angle) * 360.0f;
                    sine_angle = FastScalarSin(sine_angle);
                    raw_revs = the_car->red_line * fabs(sine_angle);
                    rev_reducer = (11.0 - (double)gCountdown) / 10.0;
                    the_car->revs = rev_reducer * raw_revs;
                } else {
                    the_car->revs = (the_car->speedo_speed / 0.003
                                        - (double)(int)(the_car->speedo_speed / 0.003))
                            * (double)(the_car->red_line - 800)
                        + 800.0;
                }
            }
            for (i = 0; i < the_car->number_of_steerable_wheels; i++) {
                ControlBoundFunkGroove(the_car->steering_ref[i], the_car->steering_angle);
            }
            for (i = 0; i < COUNT_OF(the_car->rf_sus_ref); i++) {
                ControlBoundFunkGroove(the_car->rf_sus_ref[i], the_car->rf_sus_position);
                if ((i & 1) != 0) {
                    ControlBoundFunkGroove(the_car->lf_sus_ref[i], -the_car->lf_sus_position);
                } else {
                    ControlBoundFunkGroove(the_car->lf_sus_ref[i], the_car->lf_sus_position);
                }
            }
            for (i = 0; i < COUNT_OF(the_car->rr_sus_ref); i++) {
                ControlBoundFunkGroove(the_car->rr_sus_ref[i], the_car->rr_sus_position);
                if ((i & 1) != 0) {
                    ControlBoundFunkGroove(the_car->lr_sus_ref[i], -the_car->lr_sus_position);
                } else {
                    ControlBoundFunkGroove(the_car->lr_sus_ref[i], the_car->lr_sus_position);
                }
            }
            if (!gAction_replay_mode || !ReplayIsPaused()) {
                wheel_speed = -(the_car->speedo_speed / the_car->non_driven_wheels_circum * (float)gFrame_period);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_1, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_2, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_3, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_4, wheel_speed);
                if (the_car->driver >= eDriver_net_human) {
                    if (the_car->gear) {
                        wheel_speed = -(the_car->revs
                            * the_car->speed_revs_ratio
                            / 6900.f
                            * (double)the_car->gear
                            / the_car->driven_wheels_circum
                            * (double)gFrame_period);
                    } else if (the_car->keys.brake) {
                        wheel_speed = 0.0;
                    } else {
                        wheel_speed = -(the_car->speedo_speed / the_car->driven_wheels_circum * (double)gFrame_period);
                    }
                }
                ControlBoundFunkGroovePlus(the_car->driven_wheels_spin_ref_1, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->driven_wheels_spin_ref_2, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->driven_wheels_spin_ref_3, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->driven_wheels_spin_ref_4, wheel_speed);
            }
            if (gAction_replay_mode) {
                MungeSpecialVolume((tCollision_info*)the_car);
            } else if (the_car->driver == eDriver_local_human) {
                abs_omega_x = (fabs(the_car->I.v[0]) + 3.3f) / 2.0f * fabs(the_car->omega.v[0]);
                abs_omega_y = (fabs(the_car->I.v[1]) + 3.57f) / 2.0f * fabs(the_car->omega.v[1]);
                abs_omega_z = (fabs(the_car->I.v[2]) + 0.44f) / 2.0f * fabs(the_car->omega.v[2]);
                spinning_wildly = abs_omega_x > 26.4f || abs_omega_y > 49.98f || abs_omega_z > 3.52f;
                if (spinning_wildly && the_time - gLast_cunning_stunt > 10000) {
                    if (!gWild_start
                        || (the_car->last_special_volume != NULL && the_car->last_special_volume->gravity_multiplier != 1.f)) {
                        gWild_start = the_time;
                    } else if (the_time - gWild_start >= 500) {
                        DoFancyHeadup(kFancyHeadupCunningStuntBonus);
                        EarnCredits(gCunning_stunt_bonus[gProgram_state.skill_level]);
                        gLast_cunning_stunt = the_time;
                        gOn_me_wheels_start = 0;
                        gQuite_wild_end = 0;
                        gQuite_wild_start = 0;
                        gWoz_upside_down_at_all = 0;
                    }
                } else {
                    gWild_start = 0;
                    spinning_mildly = abs_omega_x > 1.65f || abs_omega_z > 0.22f;
                    if (the_car->number_of_wheels_on_ground <= 3) {
                        gOn_me_wheels_start = 0;
                        if (the_car->number_of_wheels_on_ground || !spinning_mildly) {
                            gQuite_wild_end = the_time;
                        } else {
                            if (!gQuite_wild_start) {
                                gQuite_wild_start = the_time;
                            }
                            if (the_car->car_master_actor->t.t.mat.m[1][1] < -0.8f) {
                                gWoz_upside_down_at_all = the_time;
                            }
                        }
                    } else {
                        if (!gQuite_wild_end) {
                            gQuite_wild_end = the_time;
                        }
                        if (!gQuite_wild_start
                            || the_time - gLast_cunning_stunt <= 10000
                            || gQuite_wild_end - gQuite_wild_start < 2000
                            || gWoz_upside_down_at_all < gQuite_wild_start
                            || gWoz_upside_down_at_all > gQuite_wild_end
                            || (!gOn_me_wheels_start && the_time - gQuite_wild_end >= 300)) {
                            gQuite_wild_end = 0;
                            gQuite_wild_start = 0;
                            gOn_me_wheels_start = 0;
                            gWoz_upside_down_at_all = 0;
                        } else if (!gOn_me_wheels_start) {
                            gOn_me_wheels_start = the_time;
                        } else if (the_time - gOn_me_wheels_start > 500
                            && (the_car->last_special_volume == NULL
                                || the_car->last_special_volume->gravity_multiplier == 1.0f)) {
                            DoFancyHeadup(kFancyHeadupCunningStuntBonus);
                            EarnCredits(gCunning_stunt_bonus[gProgram_state.skill_level]);
                            gLast_cunning_stunt = PDGetTotalTime();
                            gQuite_wild_end = 0;
                            gQuite_wild_start = 0;
                            gOn_me_wheels_start = 0;
                            gWoz_upside_down_at_all = 0;
                        }
                    }
                }
            }
            if (the_car->driver != eDriver_local_human && the_car->car_model_variable) {
                distance_from_camera = Vector3DistanceSquared(&the_car->car_master_actor->t.t.translate.t,
                                           (br_vector3*)gCamera_to_world.m[3])
                    / gCar_simplification_factor[gGraf_spec_index][gCar_simplification_level];
                if (gNet_mode != eNet_mode_none && gNet_players[gIt_or_fox].car == the_car) {
                    distance_from_camera = 0.f;
                }
                for (i = 0; i < the_car->car_actor_count; i++) {
                    if (the_car->car_model_actors[i].min_distance_squared <= distance_from_camera) {
                        SwitchCarActor(the_car, i);
                        break;
                    }
                }
            }
            if (the_car->screen_material != NULL) {
                the_material = NULL;
                if (the_car->last_special_volume != NULL && the_car->last_special_volume->screen_material != NULL) {
                    if (!gAction_replay_mode && the_car->last_special_volume != gDefault_water_spec_vol) {
                        the_material = the_car->last_special_volume->screen_material;
                    } else if (gProgram_state.current_depth_effect.type == eDepth_effect_fog) {
                        the_material = gProgram_state.standard_screen_fog;
                    } else if (gProgram_state.current_depth_effect.sky_texture != NULL) {
                        the_material = gProgram_state.standard_screen;
                    } else {
                        the_material = gProgram_state.standard_screen_dark;
                    }
                } else {
                    if (gProgram_state.current_depth_effect.type == eDepth_effect_fog) {
                        the_material = gProgram_state.standard_screen_fog;
                    } else if (gProgram_state.current_depth_effect.sky_texture != NULL) {
                        the_material = gProgram_state.standard_screen;
                    } else {
                        the_material = gProgram_state.standard_screen_dark;
                    }
                }
                update_mat = 0;
                if (the_material != NULL && the_car->screen_material_source != the_material) {
                    the_car->screen_material->flags = the_material->flags;
                    the_car->screen_material->ka = the_material->ka;
                    the_car->screen_material->kd = the_material->kd;
                    the_car->screen_material->ks = the_material->ks;
                    the_car->screen_material->power = the_material->power;
                    the_car->screen_material->index_base = the_material->index_base;
                    the_car->screen_material->index_range = the_material->index_range;
                    the_car->screen_material->colour_map = the_material->colour_map;

                    the_car->screen_material->map_transform = the_material->map_transform;
                    the_car->screen_material->index_shade = gRender_shade_table;
                    the_car->screen_material_source = the_material;
                    update_mat = 1;
                }
                if (the_car->screen_material->colour_map != NULL) {
                    the_car->screen_material->map_transform.m[2][0] = fmod(car_x, 1.f);
                    the_car->screen_material->map_transform.m[2][1] = fmod(car_z, 1.f);
                    if (!update_mat) {
                        BrMaterialUpdate(the_car->screen_material, BR_MATU_MAP_TRANSFORM);
                    }
                }
                if (update_mat) {
                    BrMaterialUpdate(the_car->screen_material, BR_MATU_ALL);
                }
            }
        }
    }
}

// IDA: void __cdecl ResetCarScreens()
// FUNCTION: CARM95 0x0048701d
void ResetCarScreens(void) {
    int cat;
    int car_count;
    int i;
    tCar_spec* the_car;

    for (cat = eVehicle_self; cat <= eVehicle_rozzer; cat++) {
        car_count = (cat == eVehicle_self) ? 1 : GetCarCount(cat);
        for (i = 0; i < car_count; i++) {
            the_car = (cat == eVehicle_self) ? &gProgram_state.current_car : GetCarSpec(cat, i);
            the_car->last_special_volume = NULL;
        }
    }
    MungeCarGraphics(gFrame_period);
}

// IDA: tCar_spec* __cdecl GetRaceLeader()
// FUNCTION: CARM95 0x00487179
tCar_spec* GetRaceLeader(void) {
    int i;
    int score;
    tCar_spec* car;

    if ((gCurrent_net_game->type == eNet_game_type_foxy || gCurrent_net_game->type == eNet_game_type_tag) && gIt_or_fox >= 0 && gIt_or_fox < gNumber_of_net_players) {
        car = gNet_players[gIt_or_fox].car;
    } else {
        car = gNet_players[0].car;
        score = gNet_players[0].last_score_index;
        for (i = 1; i < gNumber_of_net_players; i++) {
            if (score > gNet_players[i].last_score_index) {
                score = gNet_players[i].last_score_index;
                car = gNet_players[i].car;
            }
        }
    }
    return car;
}

// IDA: void __cdecl AmIGettingBoredWatchingCameraSpin()
// FUNCTION: CARM95 0x004876f1
void AmIGettingBoredWatchingCameraSpin(void) {
    // GLOBAL: CARM95 0x53a528
    static tU32 time_of_death;
    // GLOBAL: CARM95 0x514e64
    static tU32 headup_timer;
    tCar_spec* car;
    char s[256];

    if (gNet_mode == eNet_mode_none
        || (gCurrent_net_game->type != eNet_game_type_sudden_death
            && gCurrent_net_game->type != eNet_game_type_tag
            && gCurrent_net_game->type != eNet_game_type_fight_to_death)) {
        gOpponent_viewing_mode = 0;
    } else if (!gRace_finished) {
        time_of_death = 0;
        gOpponent_viewing_mode = 0;
    } else if (time_of_death == 0) {
        time_of_death = GetRaceTime();
    } else {
        if (GetRaceTime() >= time_of_death + 10000) {
            if (gOpponent_viewing_mode == 0) {
                gOpponent_viewing_mode = 1;
                gNet_player_to_view_index = -2;
                ViewNetPlayer();
            }
            if (gNet_player_to_view_index >= gNumber_of_net_players) {
                gNet_player_to_view_index = -2;
                ViewNetPlayer();
            }
            if (gNet_player_to_view_index < 0 && gCar_to_view != GetRaceLeader()) {
                gNet_player_to_view_index = -2;
                ViewNetPlayer();
            }
            if ((GetRaceTime() > headup_timer + 1000 || headup_timer > GetRaceTime()) && gRace_over_reason == eRace_not_over_yet) {
                strcpy(s, GetMiscString(kMiscString_WATCHING));
                strcat(s, " ");
                if (gNet_player_to_view_index >= 0) {
                    strcat(s, gNet_players[gNet_player_to_view_index].player_name);
                } else if (gCurrent_net_game->type == eNet_game_type_tag) {
                    strcat(s, GetMiscString(kMiscString_QUOTE_IT_QUOTE));
                } else {
                    strcat(s, GetMiscString(kMiscString_RACE_LEADER));
                }
                headup_timer = GetRaceTime();
                NewTextHeadupSlot(eHeadupSlot_fancies, 0, 500, -4, s);
            }
        }
    }
}

// IDA: void __cdecl ViewNetPlayer()
// FUNCTION: CARM95 0x004870d8
void ViewNetPlayer(void) {

    if (gOpponent_viewing_mode) {
        if (gProgram_state.cockpit_on) {
            ToggleCockpit();
        }
        gNet_player_to_view_index++;
        if (gNumber_of_net_players <= gNet_player_to_view_index) {
            gNet_player_to_view_index = -1;
        }
        if (gNet_player_to_view_index < 0) {
            gCar_to_view = GetRaceLeader();
        } else {
            gCar_to_view = gNet_players[gNet_player_to_view_index].car;
        }
        gCamera_yaw = 0;
        InitialiseExternalCamera();
        PositionExternalCamera(gCar_to_view, 200u);
    }
}

// IDA: void __cdecl ViewOpponent()
// FUNCTION: CARM95 0x00487248
void ViewOpponent(void) {
    // GLOBAL: CARM95 0x514e68
    static int n;

    n++;
    if (gNet_mode != eNet_mode_none) {
        if (n >= gNumber_of_net_players) {
            n = 0;
        }
        gCar_to_view = gNet_players[n].car;
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -3, gNet_players[n].player_name);
    } else {
        if (n >= gNum_viewable_cars) {
            n = 0;
        }
        gCar_to_view = gViewable_car_list[n];
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -3, gViewable_car_list[n]->driver_name);
    }
    gCamera_yaw = 0;
    InitialiseExternalCamera();
    PositionExternalCamera(gCar_to_view, 200);
}

// IDA: void __cdecl ToggleCarToCarCollisions()
// FUNCTION: CARM95 0x00487333
void ToggleCarToCarCollisions(void) {

    gCar_car_collisions = !gCar_car_collisions;
    if (gCar_car_collisions) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, "Car Car Collisions");
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, "Ghost Cars");
    }
}

// IDA: void __cdecl SwapCar()
// FUNCTION: CARM95 0x004873a6
void SwapCar(void) {
}

// IDA: void __cdecl AdjustDownForce()
// FUNCTION: CARM95 0x004873b1
void AdjustDownForce(void) {
    char s[100];
    tCar_spec* c;

    c = gCar_to_view;
    c->down_force_speed += 50.f;
    if (c->down_force_speed > 2000.f) {
        c->down_force_speed = 50.f;
    }
    if (c->down_force_speed > 300.f) {
        c->down_force_speed = 2000.0;
    }
    sprintf(s, "DownForceSpeed %f", c->down_force_speed);
    NewTextHeadupSlot(eHeadupSlot_misc, 0, 1500, -4, s);
}

// IDA: void __cdecl FreezeMechanics()
// FUNCTION: CARM95 0x00487464
void FreezeMechanics(void) {

    gFreeze_mechanics = !gFreeze_mechanics;
    if (gFreeze_mechanics) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, "Mechanics Frozen");
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, "Thawed Mechanics");
    }
}

// IDA: void __cdecl PutOpponentsInNeutral()
// FUNCTION: CARM95 0x004874d7
void PutOpponentsInNeutral(void) {

    gStop_opponents_moving = !gStop_opponents_moving;
    if (gStop_opponents_moving) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, "Opponents in neutral");
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, "Back in gear");
    }
}

// IDA: void __cdecl SetPanningFieldOfView()
// FUNCTION: CARM95 0x004879d8
void SetPanningFieldOfView(void) {
    br_camera* camera_ptr;

    camera_ptr = gCamera->type_data;
    if (gPanning_camera_angle == 0) {
        gPanning_camera_angle = BrDegreeToAngle(gCamera_angle) * 0.7f;
    }
    camera_ptr->field_of_view = gPanning_camera_angle;
}

// IDA: void __usercall CheckDisablePlingMaterials(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x00487a3b
void CheckDisablePlingMaterials(tCar_spec* pCar) {
    br_matrix34* mat;
    br_scalar height;
    int i;

    height = 0.f;
    if (pCar->water_d == 10000.f) {
        DisablePlingMaterials();
    } else {
        mat = &pCar->car_master_actor->t.t.mat;
        for (i = 0; i < 3; i++) {
            if (mat->m[i][1] > 0.f) {
                height += pCar->bounds[0].max.v[i] * mat->m[i][1];
            } else {
                height += pCar->bounds[0].min.v[i] * mat->m[i][1];
            }
        }
        if (mat->m[3][1] / WORLD_SCALE + height < pCar->water_d) {
            DisablePlingMaterials();
        }
    }
}

// IDA: void __usercall PositionExternalCamera(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x0048754a
void PositionExternalCamera(tCar_spec* c, tU32 pTime) {
    // GLOBAL: CARM95 0x514e6c
    static int old_camera_mode;
    br_camera* camera_ptr;

    camera_ptr = (br_camera*)gCamera->type_data;
    CheckCameraHither();
    AmIGettingBoredWatchingCameraSpin();
    if ((!gAction_replay_mode || gAction_replay_camera_mode == eAction_replay_standard) && old_camera_mode != -1) {
        camera_ptr->field_of_view = BrDegreeToAngle(gCamera_angle);
        old_camera_mode = -1;
    }
    if (!gProgram_state.cockpit_on) {
        if (gOpponent_viewing_mode && gAction_replay_mode) {
            c = &gProgram_state.current_car;
        } else {
            c = gCar_to_view;
        }
        if (c->car_master_actor->t.t.translate.t.v[0] <= 500.0) {
            if (gAction_replay_mode && gAction_replay_camera_mode) {
                if (gAction_replay_camera_mode == eAction_replay_action) {
                    CheckDisablePlingMaterials(c);
                    if (IncidentCam(c, pTime)) {
                        SetPanningFieldOfView();
                        EnablePlingMaterials();
                        old_camera_mode = gAction_replay_camera_mode;
                        return;
                    }
                }
                CheckDisablePlingMaterials(c);
                SetPanningFieldOfView();
                if (gAction_replay_camera_mode != old_camera_mode) {
                    SetUpPanningCamera(c);
                    old_camera_mode = gAction_replay_camera_mode;
                }
                PanningExternalCamera(c, pTime);
                EnablePlingMaterials();
            } else {
                NormalPositionExternalCamera(c, pTime);
            }
        }
    }
}

// IDA: void __usercall CameraBugFix(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x00487b2c
void CameraBugFix(tCar_spec* c, tU32 pTime) {
    br_matrix34 mat;
    br_matrix34* m2;
    br_vector3 tv;

    if (gAction_replay_mode && gAction_replay_camera_mode != eAction_replay_standard && gPed_actor != NULL && !gProgram_state.cockpit_on) {
        IncidentCam(c, pTime);
    }
}
// IDA: int __usercall PossibleRemoveNonCarFromWorld@<EAX>(br_actor *pActor@<EAX>)
// FUNCTION: CARM95 0x0048876d
int PossibleRemoveNonCarFromWorld(br_actor* pActor) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;

    track_spec = &gProgram_state.track_spec;
    XZToColumnXZ(&cx, &cz, pActor->t.t.translate.t.v[0], pActor->t.t.translate.t.v[2], track_spec);
    if (track_spec->columns[cz][cx] == pActor->parent) {
        BrActorRemove(pActor);
        return 1;
    }
    return 0;
}

// IDA: void __usercall PutNonCarBackInWorld(br_actor *pActor@<EAX>)
// FUNCTION: CARM95 0x004887e9
void PutNonCarBackInWorld(br_actor* pActor) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;

    track_spec = &gProgram_state.track_spec;
    XZToColumnXZ(&cx, &cz, pActor->t.t.translate.t.v[0], pActor->t.t.translate.t.v[2], track_spec);
    BrActorAdd(track_spec->columns[cz][cx], pActor);
}

// IDA: int __usercall IncidentCam@<EAX>(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x00487b89
int IncidentCam(tCar_spec* c, tU32 pTime) {
    br_matrix34* m2;
    br_matrix34 mat;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 perp;
    br_vector3 vertical;
    br_vector3 murderer_pos;
    br_scalar ts;
    tCar_spec* car2;
    // GLOBAL: CARM95 0x514e70
    static tU32 next_incident_time = 0;
    // GLOBAL: CARM95 0x514e74
    static tIncident_type type = eNo_incident;
    // GLOBAL: CARM95 0x53a5b0
    static float severity;
    // GLOBAL: CARM95 0x53a588
    static tIncident_info info;
    // GLOBAL: CARM95 0x514e78
    static int random = 1;
    // GLOBAL: CARM95 0x514e7c
    static int count = 0;
    br_scalar temp;
    br_vector3 old_cam_pos;
    int removed;

    gPed_actor = NULL;
    m2 = &gCamera->t.t.mat;
    if (type == eNo_incident) {
        MoveCamToIncident(c, &type, &severity, &info, &next_incident_time);
    }
    if (type == eNo_incident) {
        return 0;
    }
    if (type == eIncident_ped) {
        BrVector3Copy(&old_cam_pos, &gCamera->t.t.translate.t);
        gPed_actor = info.ped_info.ped_actor;
        removed = PossibleRemoveNonCarFromWorld(info.ped_info.murderer_actor);
        BrMatrix34Mul(&mat, &gPed_actor->t.t.mat, &gPed_actor->parent->t.t.mat);
        info.ped_info.murderer_actor = c->car_master_actor;
        if (info.ped_info.murderer_actor != NULL) {
            BrVector3Copy(&murderer_pos, &c->pos);
        } else if (info.ped_info.murderer_actor->model != NULL) {
            BrVector3Add(&murderer_pos, &info.ped_info.murderer_actor->model->bounds.max, &info.ped_info.murderer_actor->model->bounds.min);
            BrVector3Scale(&murderer_pos, &murderer_pos, 0.5f);
            BrMatrix34ApplyP(&murderer_pos, &murderer_pos, &info.ped_info.murderer_actor->t.t.mat);
        } else {
            BrVector3Copy(&murderer_pos, &info.ped_info.murderer_actor->t.t.translate.t);
        }
        BrVector3Normalise(&vertical, (br_vector3*)mat.m[1]);
        BrVector3Scale(&vertical, &vertical, PedHeightFromActor(info.ped_info.ped_actor) / 2.f);
        BrVector3Accumulate((br_vector3*)mat.m[3], &vertical);
        if (next_incident_time > GetTotalTime() || !PipeSearchForwards()) {
            BrVector3Sub(&tv, (br_vector3*)mat.m[3], &murderer_pos);
            tv.v[1] = 0.f;
            BrVector3Normalise(&tv, &tv);
            BrVector3Set(&vertical, .0f, .4f, .0f);
            BrVector3Cross(&perp, &tv, &vertical);
            if (random) {
                BrVector3Negate(&perp, &perp);
            }
            if (PipeSearchForwards()) {
                BrVector3Accumulate(&perp, &tv);
            }
            BrVector3Add(&gCamera->t.t.translate.t, (br_vector3*)mat.m[3], &perp);
            CollideCamera2(&murderer_pos, &gCamera->t.t.translate.t, NULL, 1);
        }
        PointCamera((br_vector3*)mat.m[3], m2);
        BrVector3Sub(&tv, &gCamera->t.t.translate.t, &info.ped_info.murderer_actor->t.t.translate.t);
        ts = BrVector3LengthSquared(&tv);
        if (abs(GetTotalTime() - next_incident_time) > 2500) {
            type = eNo_incident;
        }
        if ((PipeSearchForwards() ? (next_incident_time < GetTotalTime()) : (next_incident_time > GetTotalTime()))
            && (ts > 25.f || CheckForWall(&info.ped_info.murderer_actor->t.t.translate.t, &gCamera->t.t.translate.t))) {
            type = eNo_incident;
        }
        if (removed) {
            PutNonCarBackInWorld(info.ped_info.murderer_actor);
        }
        if (Vector3DistanceSquared((br_vector3*)mat.m[3], &gCamera->t.t.translate.t) < .15f * .15f) {
            BrVector3Copy(&gCamera->t.t.translate.t, &old_cam_pos);
            gPed_actor = NULL;
            return 0;
        }
    } else if (type == eIncident_car) {
        BrVector3Sub(&tv, &info.car_info.car->pos, &c->pos);
        tv.v[1] = 0.f;
        BrVector3Normalise(&tv, &tv);
        BrVector3Scale(&tv, &tv, 2.f);
        BrVector3Add(&gCamera->t.t.translate.t, &info.car_info.car->pos, &tv);
        gCamera->t.t.translate.t.v[1] += 1.f;
        CollideCamera2(&info.car_info.car->pos, &gCamera->t.t.translate.t, NULL, 1);
        PointCamera(&info.car_info.car->pos, m2);
        BrVector3Sub(&tv, &gCamera->t.t.translate.t, &c->pos);
        ts = BrVector3LengthSquared(&tv);
        if (abs(GetTotalTime() - next_incident_time) > 2500) {
            type = eNo_incident;
        }
        if ((PipeSearchForwards() ? (next_incident_time < GetTotalTime()) : (next_incident_time > GetTotalTime()))
            && (ts > 25.f || CheckForWall(&c->pos, &gCamera->t.t.translate.t))) {
            type = eNo_incident;
        }
    } else if (type == eIncident_wall) {
        PointCamera(&c->pos, m2);
        BrVector3Sub(&tv, &gCamera->t.t.translate.t, &c->pos);
        ts = BrVector3LengthSquared(&tv);
        if (abs(GetTotalTime() - next_incident_time) > 2500) {
            type = eNo_incident;
        }
        if ((PipeSearchForwards() ? (next_incident_time < GetTotalTime()) : (next_incident_time > GetTotalTime()))
            && (ts > 25.f || CheckForWall(&c->pos, &gCamera->t.t.translate.t))) {
            type = eNo_incident;
        }
    } else {
        type = eNo_incident;
    }
    if (type == eNo_incident) {
        if (count > 1) {
            SetUpPanningCamera(c);
            return 0;
        } else {
            count++;
            if (IncidentCam(c, pTime)) {
                count--;
                return 1;
            } else {
                count--;
                return 0;
            }
        }
    } else {
        return 1;
    }
}

// IDA: int __usercall MoveCamToIncident@<EAX>(tCar_spec *c@<EAX>, tIncident_type *type@<EDX>, float *severity@<EBX>, tIncident_info *info@<ECX>, tU32 *next_incident_time)
// FUNCTION: CARM95 0x00488849
int MoveCamToIncident(tCar_spec* c, tIncident_type* type, float* severity, tIncident_info* info, tU32* next_incident_time) {
    tU32 next_incident_time2;
    tU32 t;
    tIncident_type type2;
    float severity2;
    tIncident_info info2;
    br_vector3 pos;
    br_vector3 left;
    br_vector3 right;
    br_vector3 vertical;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 perp;
    int test;

    test = 0;
    if (!GetNextIncident(-1, type, severity, info, next_incident_time)) {
        *type = eNo_incident;
    } else {
        if (abs(*next_incident_time) > 2500) {
            *type = eNo_incident;
        } else {
            t = *next_incident_time;
            for (test = 0; GetNextIncident(abs(t), &type2, &severity2, &info2, &next_incident_time2) && test <= 10 && abs(next_incident_time2) <= 3500; test++) {
                if ((*type != type2 && type2 < *type) || (*type == type2 && *severity <= severity2)) {
                    *info = info2;
                    *severity = severity2;
                    *type = type2;
                    *next_incident_time = next_incident_time2;
                }
                t = next_incident_time2;
            }
            if (abs(*next_incident_time) > 2500) {
                *type = eNo_incident;
            } else {
                if (*type == eIncident_wall) {
                    if (*severity < 0.1f) {
                        *type = eNo_incident;
                        return 0;
                    }
                    ScanCarsPositions(c, &c->pos, 100000.f, -1, abs(*next_incident_time), &pos, &t);
                    if (t == 0) {
                        *type = eNo_incident;
                    } else {
                        BrVector3Sub(&tv, &pos, &c->pos);
                        if (BrVector3LengthSquared(&tv) > 102.91955471539592f) {
                            *type = eNo_incident;
                        } else {
                            BrVector3Sub(&tv, &pos, &info->wall_info.pos);
                            BrVector3Normalise(&tv, &tv);
                            BrVector3Scale(&tv, &tv, 2.f);
                            BrVector3Set(&vertical, 0.f, 1.f, 0.f);
                            BrVector3Cross(&perp, &vertical, &tv);
                            BrVector3Add(&left, &pos, &tv);
                            BrVector3Add(&left, &left, &perp);
                            left.v[1] += 2.f;
                            BrVector3Add(&right, &pos, &tv);
                            BrVector3Sub(&right, &right, &perp);
                            right.v[1] += 2.f;
                            CollideCamera2(&pos, &left, NULL, 1);
                            CollideCamera2(&pos, &right, NULL, 1);
                            if (Vector3DistanceSquared(&left, &pos) <= Vector3DistanceSquared(&right, &pos)) {
                                BrVector3Copy(&tv, &right);
                            } else {
                                BrVector3Copy(&tv, &left);
                            }
                            BrVector3Copy(&gCamera->t.t.translate.t, &tv);
                        }
                    }
                }
                *next_incident_time += GetTotalTime();
            }
        }
    }
    return 0;
}

// IDA: void __usercall PanningExternalCamera(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x00488d45
void PanningExternalCamera(tCar_spec* c, tU32 pTime) {
    br_matrix34* m2;
    br_matrix34* m1;
    br_vector3 tv;
    br_scalar ts;
    // GLOBAL: CARM95 0x514e80
    static int inside_camera_zone = 1;

    BrVector3Sub(&tv, &gCamera->t.t.translate.t, &c->pos);
    ts = BrVector3LengthSquared(&tv);
    if (ts > 102.91955471539592f || (gSwitch_time != 0 && (PipeSearchForwards() ? (gSwitch_time <= GetTotalTime()) : (gSwitch_time >= GetTotalTime())))) {
        if ((inside_camera_zone || ts > 205.83910943079184f) && (ts > 25.f || CheckForWall(&c->pos, &gCamera->t.t.translate.t))) {
            SetUpPanningCamera(c);
            inside_camera_zone = 0;
        }
    } else {
        inside_camera_zone = 1;
    }
    m1 = &c->car_master_actor->t.t.mat;
    m2 = &gCamera->t.t.mat;
    PointCameraAtCar(c, m1, m2);
}

// IDA: int __usercall CheckForWall@<EAX>(br_vector3 *start@<EAX>, br_vector3 *end@<EDX>)
// FUNCTION: CARM95 0x00488ed6
int CheckForWall(br_vector3* start, br_vector3* end) {
    br_vector3 dir;
    br_material* material;
    br_vector3 normal;
    br_scalar d;

    BrVector3Sub(&dir, end, start);
    FindFace(start, &dir, &normal, &d, &material);
    return d <= 1.f;
}

// IDA: void __usercall SetUpPanningCamera(tCar_spec *c@<EAX>)
// FUNCTION: CARM95 0x00488f50
void SetUpPanningCamera(tCar_spec* c) {
    br_vector3 pos;
    br_vector3 perp;
    br_vector3 dir;
    br_vector3 tv;
    br_vector3 tv2;
    br_scalar ts;
    tU32 time;
    tU32 t;
    tU32 t2;
    tU32 time_step;
    br_matrix34* m2;
    br_matrix34* m1;
    br_vector3 left;
    br_vector3 right;
    br_vector3 car_centre;
    int left_score;
    int right_score;

    ScanCarsPositions(c, &c->pos, 411.6782f, -1, 5000, &car_centre, &t);
    BrVector3Sub(&dir, &car_centre, &c->pos);
    time_step = ((t > GetTotalTime()) ? t - GetTotalTime() : GetTotalTime() - t) * SRandomBetween(0.8f, 1.5f);
    if (BrVector3LengthSquared(&dir) >= .01f && t != 0) {
        ScanCarsPositions(c, &c->pos, 102.9196f, -1, time_step / 2, &pos, &t2);
        if (t2 == 0) {
            BrVector3Copy(&pos, &c->pos);
        }
    } else {
        BrVector3Negate(&dir, (br_vector3*)&c->car_master_actor->t.t.mat.m[2]);
        BrVector3Copy(&pos, &c->pos);
        time_step = 0;
    }
    BrVector3SetFloat(&tv, 0.f, 1.f, 0.f);
    BrVector3Cross(&perp, &tv, &dir);
    ts = BrVector3Length(&perp);
    if (ts >= .1f) {
        BrVector3Scale(&perp, &perp, 2.f / ts * SRandomBetween(0.3333333f, 1.f));
        BrVector3Set(&tv2, 0.f, 2 * SRandomBetween(0.3333333f, 1.f), 0.f);
        BrVector3Add(&tv, &pos, &tv2);
        BrVector3Add(&left, &tv, &perp);
        BrVector3Sub(&right, &tv, &perp);
        CollideCamera2(&pos, &left, NULL, 1);
        CollideCamera2(&pos, &right, NULL, 1);
        BrVector3Sub(&tv, &left, &pos);
        BrVector3Sub(&tv2, &right, &pos);
        if (BrVector3LengthSquared(&tv) + SRandomPosNeg(.01f) <= BrVector3LengthSquared(&tv2)) {
            BrVector3Copy(&gCamera->t.t.translate.t, &right);
        } else {
            BrVector3Copy(&gCamera->t.t.translate.t, &left);
        }
        if (t != 0 && CheckForWall(&c->pos, &gCamera->t.t.translate.t)) {
            ScanCarsPositions(c, &c->pos, 10000.f, -1, 1000, &tv, &time);
            CollideCamera2(&tv, &gCamera->t.t.translate.t, NULL, 1);
        }
        if (t != 0 && CheckForWall(&car_centre, &gCamera->t.t.translate.t)) {
            time_step = time_step / 16;
            BrVector3Copy(&tv, &pos);
            while (1) {
                ScanCarsPositions(c, &tv, 10000.f, abs(t2 - GetTotalTime()), time_step, &tv2, &time);
                t2 += (GetReplayDirection() ? 1 : -1) * time_step;
                BrVector3Copy(&tv, &tv2);
                if (CheckForWall(&tv, &gCamera->t.t.translate.t)) {
                    break;
                }
                if (t2 >= GetTotalTime() + 5000) {
                    break;
                }
            }
            gSwitch_time = t2;
        } else {
            if (t == 0) {
                t = 5000;
            }
            gSwitch_time = t;
        }
    }
}

// IDA: void __usercall SaveCameraPosition(int i@<EAX>)
// FUNCTION: CARM95 0x0048949c
void SaveCameraPosition(int i) {

    if (gSave_camera[i].saved != 1) {
        gSave_camera[i].zoom = gCamera_zoom;
        gSave_camera[i].yaw = gCamera_yaw;
        gSave_camera[i].saved = 1;
    }
}

// IDA: void __usercall RestoreCameraPosition(int i@<EAX>)
// FUNCTION: CARM95 0x004894f8
void RestoreCameraPosition(int i) {

    if (gSave_camera[i].saved != 0) {
        gCamera_zoom = gSave_camera[i].zoom;
        gCamera_yaw = gSave_camera[i].yaw;
        gSave_camera[i].saved = 0;
    }
}

// IDA: void __usercall NormalPositionExternalCamera(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x00489553
void NormalPositionExternalCamera(tCar_spec* c, tU32 pTime) {
    br_matrix34* m2;
    br_matrix34* m1;
    br_scalar time;
    br_scalar ts;
    br_scalar ts2;
    br_scalar dist;
    br_scalar height_inc;
    br_scalar l;
    br_scalar frac;
    br_vector3 vn;
    br_vector3 a;
    br_vector3 b;
    br_vector3 tv;
    br_angle yaw;
    br_angle theta;
    br_scalar d;
    int face;
    int i;
    int swoop;
    int manual_swing;
    int manual_zoom;
    br_vector3 old_camera_pos;
    br_scalar scale;

    m1 = &gCamera->t.t.mat;
    m2 = &c->car_master_actor->t.t.mat;
    swoop = gCountdown && c->pos.v[1] + 0.001f < gCamera_height;
    manual_swing = gOld_yaw__car != gCamera_yaw || swoop;
    manual_zoom = (double)gOld_zoom != gCamera_zoom;
    BrVector3Copy(&old_camera_pos, &gCamera->t.t.translate.t);

    if (!gProgram_state.cockpit_on) {
        if (swoop) {
            gCamera_yaw = 0;
            manual_swing = 1;
        }
        if (fabs(c->speedo_speed) > 0.0006f && gCamera_mode > 0) {
            gCamera_mode = -1;
            gCamera_sign = BrVector3Dot((br_vector3*)m2->m[2], &c->direction) > 0.0f;
        }
        if (c->frame_collision_flag && gCamera_mode != -2) {
            gCamera_mode = 1;
        }
        if (gCar_flying || gCamera_reset || gCamera_mode == -2) {
            gCamera_mode = 0;
        }
        d = sqrt(gCamera_zoom) + 4.f / WORLD_SCALE;
        if (!gCamera_mode || gCamera_mode == -1) {
            BrVector3Copy(&vn, &c->direction);
            MoveWithWheels(c, &vn, manual_swing);
            vn.v[1] = 0.0f;
            BrVector3Normalise(&vn, &vn);
            if (gCar_flying) {
                gCamera_sign = 0;
            }
            SwingCamera(c, m2, m1, &vn, pTime);
            BrVector3Scale(&a, &vn, d);
            BrVector3Sub(&gCamera->t.t.translate.t, &c->pos, &a);
            BrVector3Copy(&gView_direction, &vn);
        }
        if (gCamera_mode == 1) {
            if (manual_swing || manual_zoom) {
                BrVector3Copy(&old_camera_pos, &gCamera_pos_before_collide);
            }
            BrVector3Sub(&a, &c->pos, &old_camera_pos);
            a.v[1] = 0.0f;
            if (manual_swing) {
                DrVector3RotateY(&a, (gCamera_sign == 0 ? 1 : -1) * (gCamera_yaw - gOld_yaw__car));
                gCamera_yaw = gOld_yaw__car;
            }
            BrVector3Normalise(&vn, &a);
            BrVector3Copy(&gView_direction, &vn);
            BrVector3Scale(&vn, &vn, -d);
            BrVector3Accumulate(&a, &vn);
            dist = BrVector3Length(&a);
            l = (float)pTime / 1000.0f * (dist + 1.0f) / dist;
            if (l < 1.0f && BrVector3Dot(&a, &vn) > 0.0f) {
                BrVector3Scale(&a, &a, (l - 1.f));
                BrVector3Accumulate(&vn, &a);
            }
            BrVector3Add(&gCamera->t.t.translate.t, &c->pos, &vn);
        }
        height_inc = gCamera_zoom * gCamera_zoom + 0.3f;
        time = pTime * 0.001f;
        if (!gCamera_frozen || gAction_replay_mode) {
            if (pTime >= 5000) {
                gCamera_height = c->pos.v[1];
            } else if (swoop) {
                if (time > 0.2f) {
                    time = 0.2f;
                }
                gCamera_height -= time * 5.0f;
                if (gCamera_height < c->pos.v[1]) {
                    gCamera_height = c->pos.v[1];
                }
            } else {
                gCamera_height = time * 5.0f * c->pos.v[1] + gCamera_height;
                gCamera_height = gCamera_height / (time * 5.0f + 1.0f);
            }
        }
        l = c->direction.v[1] * d;
        if (l > 0) {
            if (c->pos.v[1] - l - height_inc / 2.0f > gCamera_height) {
                gCamera_height = c->pos.v[1] - l - height_inc / 2.0f;
            }
        }

        gCamera->t.t.translate.t.v[1] = height_inc + gCamera_height;
        BrVector3Copy(&gCamera_pos_before_collide, &gCamera->t.t.translate.t);
        CollideCameraWithOtherCars(&c->pos, &gCamera->t.t.translate.t);
        CollideCamera2(&c->pos, &gCamera->t.t.translate.t, &old_camera_pos, manual_swing || manual_zoom);
        if (gCamera_has_collided && swoop) {
            gCamera_height = c->pos.v[1];
        }
        PointCameraAtCar(c, m2, m1);
    }
    gOld_yaw__car = gCamera_yaw;
    gOld_zoom = (br_angle)gCamera_zoom;
}

// IDA: void __usercall MoveWithWheels(tCar_spec *c@<EAX>, br_vector3 *vn@<EDX>, int manual_swing@<EBX>)
// FUNCTION: CARM95 0x00489dc2
void MoveWithWheels(tCar_spec* c, br_vector3* vn, int manual_swing) {
    br_angle yaw;
    br_angle theta;
    // GLOBAL: CARM95 0x514e84
    static int move_with_wheels;

    if (c->speed < 0.0001f && !gCamera_mode) {
        if (manual_swing) {
            if (gCamera_yaw <= 32760u) {
                yaw = gCamera_yaw;
            } else {
                yaw = gCamera_yaw - 32760;
            }
            if (yaw <= BrDegreeToAngle(45) || yaw >= BrDegreeToAngle(135)) {
                if (!move_with_wheels) {
                    theta = BrRadianToAngle(atan2(c->wpos[0].v[2] * c->curvature, 1.0f));
                    gCamera_yaw -= (-2 * gCamera_sign + 1) * theta;
                    move_with_wheels = 1;
                }
            } else if (move_with_wheels) {
                theta = BrRadianToAngle(atan2(c->wpos[0].v[2] * c->curvature, 1.0));
                gCamera_yaw += (-2 * gCamera_sign + 1) * theta;
                move_with_wheels = 0;
            }
        }
        if (move_with_wheels) {
            if (!gCar_flying) {
                theta = BrRadianToAngle(atan2(c->wpos[0].v[2] * c->curvature, 1.0));
                DrVector3RotateY(vn, theta);
            }
        }
    }
}

// IDA: void __usercall SwingCamera(tCar_spec *c@<EAX>, br_matrix34 *m1@<EDX>, br_matrix34 *m2@<EBX>, br_vector3 *vn@<ECX>, tU32 pTime)
// FUNCTION: CARM95 0x00489f7c
void SwingCamera(tCar_spec* c, br_matrix34* m1, br_matrix34* m2, br_vector3* vn, tU32 pTime) {
    int i;
    br_scalar ts;
    br_angle yaw;
    br_angle theta;
    br_angle alpha;
    br_scalar sin_dtheta;
    br_scalar cos_dtheta;
    br_scalar sign;
    int manual_swing;
    // GLOBAL: CARM95 0x514e88
    static br_angle omega = 0;
    // GLOBAL: CARM95 0x514e8c
    static int elapsed_time = -1;
    // GLOBAL: CARM95 0x514e90
    static br_vector3 old_vn;

    br_scalar v16, v17;
    br_angle v8;

    manual_swing = gOld_yaw__car != gCamera_yaw;
    if (elapsed_time > 500) {
        elapsed_time = -1;
    }
    if (elapsed_time >= 0) {
        elapsed_time += pTime;
    }
    sign = -BrVector3Dot((br_vector3*)m1->m[2], vn);
    ts = BrVector3Dot(vn, &old_vn);

    BrVector3Copy(&old_vn, vn);
    if ((sign < 0.0f) == gCamera_sign) {
        elapsed_time = -1;
    } else if (ts <= 0.0 || elapsed_time >= 0) {
        if (elapsed_time < 0) {
            elapsed_time = 0;
        }
        if (elapsed_time < 500 && sign <= 0.0f) {
            BrVector3Negate(vn, vn);
        } else {
            elapsed_time = 500;
            if (sign <= 0.0) {
                ts = 0.0006f;
            } else {
                ts = 0.0001f;
            }
            if (fabs(c->speedo_speed) <= ts || gCar_flying) {
                BrVector3Negate(vn, vn);
            } else {
                gCamera_sign = gCamera_sign == 0;
                omega = BrDegreeToAngle(pTime * 0.03f);
                if (gCamera_yaw <= 32760) {
                    yaw = gCamera_yaw;
                } else {
                    yaw = gCamera_yaw - 32760;
                }
                if ((gCamera_yaw + 16380) <= 32760) {
                    if (yaw > 8190 && yaw < 24570) {
                        gCamera_yaw = 32760 - gCamera_yaw;
                    }
                } else {
                    gCamera_yaw = 32760 - gCamera_yaw;
                }
            }
        }
    } else {
        gCamera_sign = gCamera_sign == 0;
        if (gCamera_yaw <= 32760) {
            yaw = gCamera_yaw;
        } else {
            yaw = gCamera_yaw - 32760;
        }
        if (yaw > 8190 && yaw < 24570) {
            gCamera_yaw = -gCamera_yaw;
        }
    }
    if (gCamera_sign) {
        yaw = -gCamera_yaw;
    } else {
        yaw = gCamera_yaw;
    }
    if (!gCar_flying) {
        DrVector3RotateY(vn, yaw);
    }
    sin_dtheta = 0.0;
    v16 = vn->v[0] * gView_direction.v[2] - vn->v[2] * gView_direction.v[0];
    v17 = vn->v[0] * gView_direction.v[0] + vn->v[2] * gView_direction.v[2];

    v8 = BrRadianToAngle(sqrt(c->omega.v[2] * c->omega.v[2] + c->omega.v[0] * c->omega.v[0] + c->omega.v[1] * c->omega.v[1]) * pTime / 1000.0);
    sin_dtheta = sin(BrAngleToRadian(v8)) + 0.1;

    if (omega || gCamera_reset || (c->speed < 0.0001f && !manual_swing) || gCamera_mode == -1 || (v17 > 0.0 && !manual_swing && fabs(v16) > sin_dtheta)) {
        if (!gCar_flying) {
            theta = BrRadianToAngle(asin(sin_dtheta));
            if (omega < theta) {
                omega = theta;
            }
            if (!omega) {
                omega = BrDegreeToAngle(pTime * 0.03); // (__int64)((double)(int)pTime * 0.03 * 182.0444444444445);
            }
            cos_dtheta = cos(BrAngleToRadian(omega));
            if (cos_dtheta <= v17) {
                omega = 0;
                gCamera_mode = 0;
            } else {
                ts = BrAngleToRadian(omega);
                if (v16 <= 0.0) {
                    vn->v[0] = cos(ts) * gView_direction.v[0] - sin(ts) * gView_direction.v[2];
                    vn->v[2] = sin(ts) * gView_direction.v[0] + cos(ts) * gView_direction.v[2];
                } else {
                    vn->v[0] = sin(ts) * gView_direction.v[2] + cos(ts) * gView_direction.v[0];
                    vn->v[2] = cos(ts) * gView_direction.v[2] - sin(ts) * gView_direction.v[0];
                }
                omega += BrDegreeToAngle(pTime * 0.03f);
                if (BrDegreeToAngle(pTime * 0.1f) < omega) {
                    omega = BrDegreeToAngle(pTime * 0.1f);
                }
                if (omega < theta) {
                    omega = theta;
                }
            }
        }
    }
}

// IDA: void __usercall PointCameraAtCar(tCar_spec *c@<EAX>, br_matrix34 *m1@<EDX>, br_matrix34 *m2@<EBX>)
// FUNCTION: CARM95 0x0048a73b
void PointCameraAtCar(tCar_spec* c, br_matrix34* m1, br_matrix34* m2) {
    br_vector3 vn;
    br_vector3 tv;
    br_vector3 tv2;
    br_scalar dist;
    br_scalar frac;
    br_angle theta;
    br_vector3* pos;
    br_camera* camera_ptr;
    br_angle off_centre_angle;
    int swoop;
    br_scalar scale;

    camera_ptr = gCamera->type_data;
    theta = camera_ptr->field_of_view / 5;
    swoop = gCountdown && c->pos.v[1] + 0.01f < gCamera_height;
    if (0) {
        BrVector3Sub(&tv, &gAverage_grid_position, &c->pos);
        frac = (gCamera_height - c->pos.v[1]) / 10.0f;
        BrVector3Scale(&tv, &tv, frac);
        BrVector3Accumulate(&tv, &c->pos);
        pos = &tv;
        theta = (1.0f - frac) * (float)theta;
    } else {
        pos = &c->pos;
    }
    BrVector3Set(&vn, c->pos.v[0] - m2->m[3][0], 0.f, c->pos.v[2] - m2->m[3][2]);
    BrVector3Normalise(&vn, &vn);
    m2->m[0][0] = -vn.v[2];
    m2->m[0][1] = 0.0f;
    m2->m[0][2] = vn.v[0];
    m2->m[1][0] = 0.0f;
    m2->m[1][1] = 1.0f;
    m2->m[1][2] = 0.0f;
    m2->m[2][0] = -vn.v[0];
    m2->m[2][1] = 0.0f;
    m2->m[2][2] = -vn.v[2];
    BrVector3Sub(&tv2, pos, (br_vector3*)m2->m[3]);
    dist = BrVector3Dot(&tv2, &vn);
    BrMatrix34PreRotateX(m2, theta - BrRadianToAngle(atan2(m2->m[3][1] - pos->v[1], dist)));
}

// IDA: void __usercall PointCamera(br_vector3 *pos@<EAX>, br_matrix34 *m2@<EDX>)
// FUNCTION: CARM95 0x0048aa19
void PointCamera(br_vector3* pos, br_matrix34* m2) {
    br_vector3 vn;
    br_scalar dist;
    br_angle theta;
    br_camera* camera_ptr;

    camera_ptr = gCamera->type_data;
    BrVector3Sub(&vn, pos, (br_vector3*)m2->m[3]);
    vn.v[1] = 0.f;
    BrVector3Normalise(&vn, &vn);
    m2->m[0][0] = -vn.v[2];
    m2->m[0][1] = 0.f;
    m2->m[0][2] = vn.v[0];
    m2->m[1][0] = 0.f;
    m2->m[1][1] = 1.f;
    m2->m[1][2] = 0.f;
    m2->m[2][0] = -vn.v[0];
    m2->m[2][1] = 0.f;
    m2->m[2][2] = -vn.v[2];
    dist = BrVector3LengthSquared(&vn);
    theta = BR_ATAN2(m2->m[3][1] - pos->v[1], dist);
    BrMatrix34PreRotateX(m2, camera_ptr->field_of_view / 5 - theta);
}

// IDA: int __usercall CollideCamera2@<EAX>(br_vector3 *car_pos@<EAX>, br_vector3 *cam_pos@<EDX>, br_vector3 *old_camera_pos@<EBX>, int manual_move@<ECX>)
// FUNCTION: CARM95 0x0048abbb
int CollideCamera2(br_vector3* car_pos, br_vector3* cam_pos, br_vector3* old_camera_pos, int manual_move) {
    int i;
    int k;
    br_vector3 a;
    br_vector3 b;
    br_vector3 vn;
    br_vector3 tv;
    br_vector3 tv2;
    br_scalar l;
    br_scalar d;
    br_scalar ts;
    br_scalar ts2;
    br_scalar dist;
    br_scalar hither;
    br_angle theta;
    tBounds bnds;
    br_matrix34 mat;
    br_material* material;
    br_scalar alpha;
    br_scalar sa;
    br_scalar sb;
    br_scalar sc;
    tFace_ref face_list[3];

#ifdef DETHRACE_FIX_BUGS
    ts2 = 0.f;
#endif
    hither = ((br_camera*)gCamera->type_data)->hither_z * 3.0f;
    gCamera_has_collided = 0;
    for (i = 0; i < 1; i++) {
        BrVector3Sub(&tv, cam_pos, car_pos);
        dist = BrVector3Length(&tv);
        BrVector3Scale(&tv, &tv, 1.2f);
        FindFace(car_pos, &tv, &a, &ts, &material);
        if (ts <= 1.0) {
            gCamera_has_collided = 1;
            if (BrVector3Dot(&a, &tv) > 0.0f) {
                BrVector3Negate(&a, &a);
            }
            if (gCamera_mode == 1 && !manual_move) {
                BrVector3Sub(&tv2, car_pos, old_camera_pos);
                FindFace(old_camera_pos, &tv2, &b, &ts2, &material);
                if (ts2 > 1.0f) {
                    BrVector3Copy(cam_pos, old_camera_pos);
                    return i;
                }
            }
            BrVector3Scale(&tv, &tv, ts);
            BrVector3Scale(&tv2, &a, hither);
            BrVector3Accumulate(&tv, &tv2);
            dist = BrVector3Length(&tv);
            BrVector3Add(cam_pos, car_pos, &tv);
            if (gMin_camera_car_distance > dist && i == 0 && a.v[1] > -0.7) {
                BrVector3Scale(&tv2, &a, -a.v[1]);
                tv2.v[1] += 1.f;
                if (gProgram_state.current_car.car_master_actor->t.t.mat.m[1][1] < 0.f) {
                    BrVector3Negate(&tv2, &tv2);
                }
                d = BrVector3LengthSquared(&tv2);
                l = BrVector3Dot(&tv2, &tv);
                alpha = BrVector3LengthSquared(&tv) - gMin_camera_car_distance * gMin_camera_car_distance;
                ts2 = l * l - alpha * d * 4.0;
                if (ts2 >= 0.f && d != 0.f) {
                    sa = (sqrt(ts2) - l) / (d * 2.0f);
                    BrVector3Scale(&tv2, &tv2, sa);
                    FindFace(cam_pos, &tv2, &a, &ts, &material);
                    if (ts < 1.0f) {
                        BrVector3Scale(&tv2, &tv2, ts);
                    }
                    BrVector3Set(&b, tv.v[0], 0.f, tv.v[2]);
                    BrVector3Normalise(&b, &b);
                    BrVector3Accumulate(&tv, &tv2);
                    ts2 = BrVector3Dot(&tv, &b);
                    if (ts2 < 0.03f && !gAction_replay_mode) {
                        BrVector3Normalise(&tv2, &tv2);
                        alpha = BrVector3Dot(&tv2, &b);
                        if (alpha < -0.03f) {
                            alpha = (0.03f - ts2) / alpha;
                            BrVector3Scale(&tv2, &tv2, alpha);
                            BrVector3Add(&tv, &tv2, &tv);
                        }
                    }
                }
                BrVector3Add(cam_pos, car_pos, &tv);
            }
        }

        bnds.mat = &mat;
        BrMatrix34Identity(&mat);
        BrVector3Set(&tv2, hither, hither, hither);
        bnds.original_bounds.min.v[0] = cam_pos->v[0] - hither;
        bnds.original_bounds.min.v[1] = cam_pos->v[1] - hither;
        bnds.original_bounds.min.v[2] = cam_pos->v[2] - hither;
        bnds.original_bounds.max.v[0] = cam_pos->v[0] + hither;
        bnds.original_bounds.max.v[1] = cam_pos->v[1] + hither;
        bnds.original_bounds.max.v[2] = cam_pos->v[2] + hither;
        k = FindFacesInBox(&bnds, face_list, 3);
        if (k > 0) {
            BrVector3Sub(&tv2, cam_pos, &face_list[0].v[0]);
            sa = BrVector3Dot(&face_list[0].normal, &tv2);
            // ts2 = sa;
            if (sa < hither && sa >= 0.0) {
                BrVector3Scale(&tv2, &face_list[0].normal, hither - sa);
                BrVector3Accumulate(cam_pos, &tv2);
            }
            if (k > 1) {
                sb = BrVector3Dot(&face_list[0].normal, &face_list[1].normal);
                if (sb > 0.95f && k > 2) {
                    BrVector3Copy(&face_list[1].normal, &face_list[2].normal);
                    BrVector3Copy(&face_list[1].v[0], &face_list[2].v[0]);
                    sb = BrVector3Dot(&face_list[0].normal, &face_list[2].normal);
                    k = 2;
                }
                if (sb <= 0.95f) {
                    BrVector3Sub(&tv2, cam_pos, &face_list[1].v[0]);
                    sc = BrVector3Dot(&face_list[1].normal, &tv2);
                    if (sc < hither && sc >= 0.0) {
                        sc = BrVector3Dot(&face_list[0].normal, &face_list[1].normal);
                        BrVector3Scale(&b, &face_list[0].normal, sc);
                        BrVector3Sub(&face_list[1].normal, &face_list[1].normal, &b);
                        BrVector3Scale(&tv2, &face_list[1].normal, hither - ts2);
                        BrVector3Accumulate(cam_pos, &tv2);
                    }
                }
            }
        }
        i += k;
    }
    return i;
}

// IDA: int __usercall BoundsTest@<EAX>(br_bounds *bnds@<EAX>, br_vector3 *p@<EDX>)
// FUNCTION: CARM95 0x0048ba12
int BoundsTest(br_bounds* bnds, br_vector3* p) {
    int j;

    for (j = 0; j < 3; j++) {
        if (p->v[j] > bnds->max.v[j] || p->v[j] < bnds->min.v[j]) {
            return 0;
        }
    }
    return 1;
}

// IDA: int __usercall CollideCameraWithOtherCars@<EAX>(br_vector3 *car_pos@<EAX>, br_vector3 *cam_pos@<EDX>)
// FUNCTION: CARM95 0x0048b820
int CollideCameraWithOtherCars(br_vector3* car_pos, br_vector3* cam_pos) {
    int i;
    int plane;
    br_scalar ts;
    tCar_spec* c;
    br_vector3 tv;
    br_vector3 pos_car_space;
    br_vector3 dir;
    br_vector3 p;
    br_vector3 n;
    br_bounds bnds;

    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        if (BoundsTest(&gActive_car_list[i]->bounds_world_space, cam_pos)) {
            c = gActive_car_list[i];
            BrVector3Sub(&tv, cam_pos, &c->car_master_actor->t.t.translate.t);
            BrMatrix34TApplyV(&p, &tv, &c->car_master_actor->t.t.mat);
            if (BoundsTest(&c->bounds[0], &p)) {
                BrVector3Sub(&tv, cam_pos, car_pos);
                BrMatrix34TApplyV(&dir, &tv, &c->car_master_actor->t.t.mat);
                BrVector3Add(&pos_car_space, &p, &dir);
                BrVector3SetFloat(&tv, 0.03f, 0.03f, 0.03f);
                BrVector3Sub(&bnds.min, &c->bounds[0].min, &tv);
                BrVector3Add(&bnds.max, &c->bounds[0].max, &tv);
                plane = LineBoxColl(&pos_car_space, &p, &bnds, &tv);
                BrMatrix34ApplyP(cam_pos, &tv, &c->car_master_actor->t.t.mat);
                return 1;
            }
        }
    }
    return 0;
}

// IDA: void __cdecl InitialiseExternalCamera()
// FUNCTION: CARM95 0x0048ba8a
void InitialiseExternalCamera(void) {
    br_scalar ts;
    tCar_spec* c;
    br_vector3 r;
    br_angle yaw;

    c = gCar_to_view;
    if (!gProgram_state.racing) {
        c = &gProgram_state.current_car;
    }
    gCamera_height = c->pos.v[1];
    BrVector3Set(&gView_direction, c->direction.v[0], 0.0f, c->direction.v[2]);
    BrVector3Normalise(&gView_direction, &gView_direction);
    ts = -BrVector3Dot(&gView_direction, (br_vector3*)c->car_master_actor->t.t.mat.m[2]);
    gCamera_sign = ts < 0;
    gCamera_mode = 0;
    if (gCamera_sign) {
        yaw = -gCamera_yaw;
    } else {
        yaw = gCamera_yaw;
    }
    DrVector3RotateY(&gView_direction, yaw);
    gMin_camera_car_distance = 0.6f;
    gCamera_frozen = 0;
    gCamera_mode = -2;
    if (gCountdown && (gNet_mode == eNet_mode_none || gCurrent_net_game->options.grid_start) && gCountdown > 4) {
        gCamera_height = gCamera_height + 10.0f;
    }
}

// IDA: void __cdecl FreezeCamera()
// FUNCTION: CARM95 0x0048bc93
void FreezeCamera(void) {

    gCamera_frozen = 1;
}

// IDA: void __usercall FlyCar(tCar_spec *c@<EAX>, br_scalar dt)
// FUNCTION: CARM95 0x0048bca8
void FlyCar(tCar_spec* c, br_scalar dt) {
    int accflag;
    int turnflag;
    br_vector3 step;
    br_matrix34* mat;
    br_angle theta;
    // GLOBAL: CARM95 0x514e9c
    static br_scalar vel = 0.f;
    tFace_ref faces[20];
    tBounds bnds;
    int i;                 // Added by DethRace
    br_scalar tmp_scalar1; // Added by DethRace
    br_scalar tmp_scalar2; // Added by DethRace
    br_vector3 tmp1;       // Added by DethRace
    br_vector3 tmp2;       // Added by DethRace
    br_vector3 tmp3;       // Added by DethRace

    accflag = 0;
    turnflag = 0;
    mat = &c->car_master_actor->t.t.mat;
    bnds.mat = mat;
    BrVector3InvScale(&bnds.original_bounds.min, &c->bounds[1].min, WORLD_SCALE);
    BrVector3InvScale(&bnds.original_bounds.max, &c->bounds[1].max, WORLD_SCALE);
    BrVector3InvScale((br_vector3*)bnds.mat->m[3], (br_vector3*)bnds.mat->m[3], WORLD_SCALE);
    FindFacesInBox(&bnds, faces, COUNT_OF(faces));
    BrVector3Scale((br_vector3*)bnds.mat->m[3], (br_vector3*)bnds.mat->m[3], WORLD_SCALE);
    if (c->keys.acc || c->joystick.acc > 0) {
        vel += 10.f * dt;
        accflag = 1;
    }
    if (c->keys.dec || c->joystick.dec > 0) {
        vel -= 10.f * dt;
        accflag = 1;
    }
    if (!accflag) {
        if (vel >= 20.f * dt || vel <= -20.f * dt) {
            vel -= 20.f * vel / fabs(vel) * dt;
        } else {
            vel = 0.f;
        }
    }
    BrVector3Scale(&step, &c->v, dt);
    BrVector3Accumulate((br_vector3*)mat->m[3], &step);
    BrVector3Scale(&step, (br_vector3*)mat->m[1], dt);
    if (c->keys.left || c->joystick.left > 0) {
        BrVector3Accumulate(&c->omega, &step);
        turnflag = 1;
    }
    if (c->keys.right || c->joystick.right > 0) {
        BrVector3Sub(&c->omega, &c->omega, &step);
        turnflag = 1;
    }
    if (!turnflag) {
        BrVector3SetFloat(&c->omega, 0.f, 0.f, 0.f);
    }
    theta = BrRadianToAngle(c->omega.v[1] * dt);
    DrVector3RotateY(&c->v, theta);
    BrMatrix34PreRotateY(mat, theta);
    if (c->keys.up) {
        BrMatrix34PreRotateX(mat, BrDegreeToAngle(5));
    }
    if (c->keys.down) {
        BrMatrix34PreRotateX(mat, BrDegreeToAngle(360 - 5));
    }
    BrVector3Scale(&c->v, (br_vector3*)mat->m[2], -vel);
    BrVector3Scale(&step, &c->v, dt);
    BrVector3Accumulate((br_vector3*)&mat->m[3], &step);
    if (c->keys.holdw) {
        BrVector3Copy(&step, (br_vector3*)mat->m[3]);
        BrMatrix34RotateY(mat, BrDegreeToAngle(90) - BrRadianToAngle(atan2(mat->m[2][2], mat->m[2][0])));
        BrVector3Copy((br_vector3*)mat->m[3], &step);
        BrVector3SetFloat(&step, 0.f, -100.f, 0.f);
        BrVector3Copy(&tmp1, (br_vector3*)&mat->m[3]);
        findfloor(&tmp1, &step, &tmp2, &tmp_scalar1);
        tmp1.v[1] += 100.f;
        findfloor(&tmp1, &step, &tmp3, &tmp_scalar2);
        if (tmp_scalar2 <= 1.f) {
            BrVector3SetFloat(&step, 0.f, -5.01f, 0.f);
            tmp1.v[1] -= 20 * 5.f;
            for (i = 0; i < 20; i++) {
                tmp1.v[1] += 5.f;
                findfloor(&tmp1, &step, &tmp3, &tmp_scalar2);
                if (tmp_scalar2 <= 1.f) {
                    break;
                }
            }
            tmp_scalar2 = -tmp_scalar2 / 20.f + (i + 1) * 0.05f;
        }
        if (tmp_scalar2 < tmp_scalar1) {
            tmp_scalar1 = -tmp_scalar2;
            BrVector3Copy(&tmp2, &tmp3);
        }
        if (tmp_scalar1 <= 1.f) {
            mat->m[3][1] -= tmp_scalar1 * 100.f;
            BrMatrix34PreRotateX(mat, BrRadianToAngle(asin(BrVector3Dot((br_vector3*)mat->m[2], &tmp2))));
            BrMatrix34PreRotateZ(mat, -BrRadianToAngle(asin(BrVector3Dot((br_vector3*)mat->m[0], &tmp2))));
        }
    }
    BrVector3Negate((br_vector3*)&c->direction, (br_vector3*)mat->m[2]);
    BrMatrix34Copy(&c->oldmat, mat);
    BrMatrix34ApplyP(&c->pos, &c->cmpos, mat);
    BrVector3InvScale(&c->pos, &c->pos, WORLD_SCALE);
    BrVector3InvScale((br_vector3*)bnds.mat->m[3], (br_vector3*)bnds.mat->m[3], WORLD_SCALE);
    GetNewBoundingBox(&c->bounds_world_space, c->bounds, bnds.mat);
    BrVector3Scale((br_vector3*)bnds.mat->m[3], (br_vector3*)bnds.mat->m[3], WORLD_SCALE);
}

// IDA: void __usercall DrVector3RotateY(br_vector3 *v@<EAX>, br_angle t@<EDX>)
// FUNCTION: CARM95 0x0048c718
void DrVector3RotateY(br_vector3* v, br_angle t) {
    br_scalar c;
    br_scalar s;
    br_scalar ts;

    c = cos(BrAngleToRadian(t));
    s = sin(BrAngleToRadian(t));
    ts = v->v[0] * c + v->v[2] * s;
    v->v[2] = v->v[2] * c - v->v[0] * s;
    v->v[0] = ts;
}

// IDA: void __cdecl CrashCarsTogether(br_scalar dt)
// FUNCTION: CARM95 0x0048c795
void CrashCarsTogether(br_scalar dt) {
    int pass;
    int k;
    int i;
    tCollison_data collide_list[32];

    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        collide_list[i].car = NULL;
        collide_list[i].ref = gNum_cars_and_non_cars - 1;
        gActive_car_list[i]->infinite_mass = 0;
    }
    for (pass = 0; pass < 5; pass++) {
        k = CrashCarsTogetherSinglePass(dt, pass, collide_list);
        if (k <= 0) {
            break;
        }
    }
    if (k > 0) {
        for (i = 0; i < gNum_cars_and_non_cars; i++) {
            BringCarToAGrindingHalt((tCollision_info*)gActive_car_list[i]);
        }
    }
}

// IDA: int __cdecl CrashCarsTogetherSinglePass(br_scalar dt, int pPass, tCollison_data *collide_list)
// FUNCTION: CARM95 0x0048c8af
int CrashCarsTogetherSinglePass(br_scalar dt, int pPass, tCollison_data* collide_list) {
    int i;
    int j;
    int l;
    int m;
    int n;
    int collided;
    int k;
    int ref1;
    int ref2;
    int c1im;
    int c2im;
    tCollision_info* car_1;
    tCollision_info* car_2;
    tCollision_info* car_3;
    tCollision_info* car_in_middle;
    tCollision_info* car_on_wall;

    collided = 0;
    for (i = 0; i < gNum_cars_and_non_cars - 1; i++) {
        car_1 = (tCollision_info*)gActive_car_list[i];
        for (j = i + 1; j < gNum_cars_and_non_cars; j++) {
            car_2 = (tCollision_info*)gActive_car_list[j];
            if (collide_list[i].ref > 0 || collide_list[j].ref > 0) {
                collide_list[i].ref--;
                collide_list[j].ref--;
                if (SimpleCarCarCollisionTest(car_1, car_2)) {
                    if (car_1->infinite_mass == -1 && car_2->infinite_mass > 0) {
                        if (CollideTwoCars(car_1, car_2, -1)) {
                            if (car_2->infinite_mass >= 256 || pPass >= 4) {
                                BringCarToAGrindingHalt(car_2);
                            } else {
                                car_2->infinite_mass = 0;
                                k = CollideTwoCarsWithWalls(car_1, car_2, dt);
                                car_2->infinite_mass = 256;
                                if (k < 0) {
                                    BringCarToAGrindingHalt(car_2);
                                }
                            }
                            collide_list[j].ref = gNum_cars_and_non_cars - 2;
                            collide_list[j].car = car_1;
                            collided++;
                        }
                    } else if (car_2->infinite_mass == -1 && car_1->infinite_mass > 0) {
                        if (CollideTwoCars(car_1, car_2, -1)) {
                            if (car_1->infinite_mass >= 256 || pPass >= 4) {
                                BringCarToAGrindingHalt(car_1);
                            } else {
                                car_1->infinite_mass = 0;
                                k = CollideTwoCarsWithWalls(car_1, car_2, dt);
                                car_1->infinite_mass = 256;
                                if (k < 0) {
                                    BringCarToAGrindingHalt(car_1);
                                }
                            }
                            collide_list[i].ref = gNum_cars_and_non_cars - 2;
                            collide_list[i].car = car_2;
                            collided++;
                        }
                    } else if (collide_list[i].car || collide_list[j].car) {
                        if ((collide_list[j].car == NULL) == (collide_list[i].car == NULL)) {
                            if (collide_list[j].car != collide_list[i].car || (car_1->infinite_mass && car_2->infinite_mass)) {
                                if (collide_list[i].car && collide_list[j].car) {
                                    if (car_1->infinite_mass && car_2->infinite_mass) {
                                        if ((car_1->infinite_mass != -1 || car_2->infinite_mass != -1) && CollideTwoCars(car_1, car_2, -1)) {
                                            collide_list[i].ref = gNum_cars_and_non_cars - 2;
                                            collide_list[j].ref = gNum_cars_and_non_cars - 2;
                                            if (car_1->infinite_mass && car_2->infinite_mass) {
                                                BringCarToAGrindingHalt(car_1);
                                                BringCarToAGrindingHalt(car_2);
                                            } else {
                                                k = CollideTwoCarsWithWalls(car_1, car_2, dt);
                                                if (k >= 0) {
                                                    if (k) {
                                                        if (car_1->infinite_mass >= 0) {
                                                            car_1->infinite_mass++;
                                                        }
                                                        if (car_2->infinite_mass >= 0) {
                                                            car_2->infinite_mass++;
                                                        }
                                                        collided++;
                                                    }
                                                } else {
                                                    BringCarToAGrindingHalt(car_1);
                                                    BringCarToAGrindingHalt(car_2);
                                                    collided++;
                                                }
                                            }
                                            collided++;
                                        }
                                    } else {
                                        c1im = car_1->infinite_mass;
                                        c2im = car_2->infinite_mass;
                                        k = CollideTwoCarsWithWalls(car_1, car_2, dt);
                                        if (k > -1) {
                                            if (k) {
                                                if (!c2im) {
                                                    collide_list[j].ref = gNum_cars_and_non_cars - 2;
                                                }
                                                if (!c1im) {
                                                    collide_list[i].ref = gNum_cars_and_non_cars - 2;
                                                }
                                                collided++;
                                                if (car_1->infinite_mass >= 0) {
                                                    car_1->infinite_mass++;
                                                }
                                                if (car_2->infinite_mass >= 0) {
                                                    car_2->infinite_mass++;
                                                }
                                            }
                                        } else {
                                            BringCarToAGrindingHalt(car_1);
                                            BringCarToAGrindingHalt(car_2);
                                            if (c1im >= 0) {
                                                collide_list[i].ref = gNum_cars_and_non_cars - 2;
                                            }
                                            if (c2im >= 0) {
                                                collide_list[j].ref = gNum_cars_and_non_cars - 2;
                                            }
                                            collided++;
                                        }
                                    }
                                }
                            } else {
                                k = CollideTwoCarsWithWalls(car_1, car_2, dt);
                                if (k) {
                                    collide_list[i].ref = gNum_cars_and_non_cars - 2;
                                    collide_list[j].ref = gNum_cars_and_non_cars - 2;
                                    if (k == -1) {
                                        BringCarToAGrindingHalt(car_1);
                                        BringCarToAGrindingHalt(car_2);
                                    }
                                    collided++;
                                    if (car_1->infinite_mass >= 0) {
                                        car_1->infinite_mass++;
                                    }
                                    if (car_2->infinite_mass >= 0) {
                                        car_2->infinite_mass++;
                                    }
                                }
                            }
                        } else {
                            c1im = car_1->infinite_mass;
                            c2im = car_2->infinite_mass;
                            k = CollideTwoCarsWithWalls(car_1, car_2, dt);
                            if (k == -1) {
                                if (collide_list[i].car) {
                                    car_3 = collide_list[i].car;
                                } else {
                                    car_3 = collide_list[j].car;
                                }
                                if (collide_list[i].car) {
                                    car_in_middle = car_1;
                                } else {
                                    car_in_middle = car_2;
                                }
                                if (car_3->infinite_mass > 1 || car_3->infinite_mass == -1 || (tU8)(car_in_middle->infinite_mass) > 1 || car_in_middle->infinite_mass == -1) {
                                    BringCarToAGrindingHalt(car_1);
                                    BringCarToAGrindingHalt(car_2);
                                    collide_list[i].ref = gNum_cars_and_non_cars - 2;
                                    collide_list[j].ref = gNum_cars_and_non_cars - 2;
                                } else {
                                    if (collide_list[i].car) {
                                        car_on_wall = car_2;
                                    } else {
                                        car_on_wall = car_1;
                                    }
                                    car_in_middle->infinite_mass = 0;
                                    car_3->infinite_mass = 0;
                                    k = CollideTwoCarsWithWalls(car_on_wall, car_in_middle, dt);
                                    car_in_middle->infinite_mass = 2;
                                    car_on_wall->infinite_mass++;
                                    collide_list[i].ref = gNum_cars_and_non_cars - 2;
                                    collide_list[j].ref = gNum_cars_and_non_cars - 2;
                                    if (k < 0) {
                                        BringCarToAGrindingHalt(car_1);
                                        BringCarToAGrindingHalt(car_2);
                                    }
                                }
                                collide_list[i].car = car_2;
                                collide_list[j].car = car_1;
                                collided++;
                            } else if (k) {
                                collide_list[i].car = car_2;
                                collide_list[j].car = car_1;
                                if (!c2im) {
                                    collide_list[j].ref = gNum_cars_and_non_cars - 2;
                                }
                                if (!c1im) {
                                    collide_list[i].ref = gNum_cars_and_non_cars - 2;
                                }
                                collided++;
                                if (car_1->infinite_mass >= 0) {
                                    car_1->infinite_mass++;
                                }
                                if (car_2->infinite_mass >= 0) {
                                    car_2->infinite_mass++;
                                }
                            }
                        }
                    } else {
                        k = CollideTwoCarsWithWalls(car_1, car_2, dt);
                        if (k) {
                            collide_list[i].car = car_2;
                            collide_list[j].car = car_1;
                            collide_list[i].ref = gNum_cars_and_non_cars - 2;
                            collide_list[j].ref = gNum_cars_and_non_cars - 2;
                            if (k == -1) {
                                BringCarToAGrindingHalt(car_1);
                                BringCarToAGrindingHalt(car_2);
                                collide_list[i].ref = gNum_cars_and_non_cars - 2;
                                collide_list[j].ref = gNum_cars_and_non_cars - 2;
                            }
                            collided++;
                        }
                    }
                    CrashEarnings((tCar_spec*)car_1, (tCar_spec*)car_2);
                }
            }
        }
    }
    return collided;
}

// IDA: void __usercall BringCarToAGrindingHalt(tCollision_info *car@<EAX>)
// FUNCTION: CARM95 0x0048d21b
void BringCarToAGrindingHalt(tCollision_info* car) {

    BrVector3SetFloat(&car->v, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&car->omega, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&car->oldomega, 0.0f, 0.0f, 0.0f);
    BrMatrix34Copy(&car->car_master_actor->t.t.mat, &car->oldmat);
    car->infinite_mass = -1;
}

// IDA: int __usercall BoundsOverlapTest@<EAX>(br_bounds *b1@<EAX>, br_bounds *b2@<EDX>)
// FUNCTION: CARM95 0x0048d2e3
int BoundsOverlapTest_car(br_bounds* b1, br_bounds* b2) {

    return b2->max.v[0] >= b1->min.v[0]
        && b1->max.v[0] >= b2->min.v[0]
        && b2->max.v[1] >= b1->min.v[1]
        && b1->max.v[1] >= b2->min.v[1]
        && b2->max.v[2] >= b1->min.v[2]
        && b1->max.v[2] >= b2->min.v[2];
}

// IDA: int __usercall SimpleCarCarCollisionTest@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>)
// FUNCTION: CARM95 0x0048d28f
int SimpleCarCarCollisionTest(tCollision_info* car1, tCollision_info* car2) {

    if (car1->bounds_ws_type == eBounds_ws && car2->bounds_ws_type == eBounds_ws) {
        return BoundsOverlapTest_car(&car1->bounds_world_space, &car2->bounds_world_space);
    } else {
        return 1;
    }
}

// IDA: int __usercall CollideTwoCarsWithWalls@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_scalar dt)
// FUNCTION: CARM95 0x0048d38c
int CollideTwoCarsWithWalls(tCollision_info* car1, tCollision_info* car2, br_scalar dt) {
    br_vector3 mom1;
    br_vector3 mom2;
    int l;
    int m;
    int n;
    int p;
    int im1;
    int im2;

    l = 0;
    m = 0;
    p = 0;
    im1 = car1->infinite_mass;
    im2 = car2->infinite_mass;
    do {
        n = CollideTwoCarsRepeatedly(car1, car2, dt);
        if (n <= 0) {
            if (n == -1) {
                return -1;
            }
            l = 0;
            m = 0;
        } else {
            ++p;
            if (n >= 5) {
                if (p >= 10 || car1->infinite_mass || car2->infinite_mass) {
                    return -1;
                }
                BrVector3Set(&car1->omega, 0.0f, 0.0f, 0.0f);
                BrVector3Set(&car2->omega, 0.0f, 0.0f, 0.0f);
                BrVector3Scale(&mom1, &car1->v, car1->M);
                BrVector3Scale(&mom2, &car2->v, car2->M);
                BrVector3Accumulate(&mom1, &mom2);
                BrVector3InvScale(&car1->v, &mom1, car2->M + car1->M);
                car2->v = car1->v;
                RotateCar(car1, dt);
                TranslateCar(car1, dt);
                RotateCar(car2, dt);
                TranslateCar(car2, dt);
                if (CollideTwoCars(car1, car2, -1)) {
                    return -1;
                }
                if (im1 || im2) {
                    return -1;
                }
            }
            if (!im1) {
                l = CollideCarWithWall(car1, dt);
            }
            if (!im2) {
                m = CollideCarWithWall(car2, dt);
            }
            if (p < 3) {
                car1->infinite_mass = im1;
                car2->infinite_mass = im2;
            }
            if (p > 5) {
                if (l) {
                    car1->infinite_mass |= 0x100u;
                }
                if (m) {
                    car2->infinite_mass |= 0x100u;
                }
            }
            if (car1->infinite_mass && car2->infinite_mass) {
                return -1;
            }
        }
    } while ((l || m) && p < 10);
    if (p < 10) {
        return p;
    } else {
        return -1;
    }
}

// IDA: int __usercall CollideTwoCarsRepeatedly@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_scalar dt)
// FUNCTION: CARM95 0x0048d719
int CollideTwoCarsRepeatedly(tCollision_info* car1, tCollision_info* car2, br_scalar dt) {
    int l;
    int collide;
    br_scalar ts;

    l = 0;
    ModifyCarsMass(car1, car2);
    while (1) {
        collide = CollideTwoCars(car1, car2, l);
        if (!collide) {
            break;
        }
        if (collide == -1) {
            ResetCarsMass(car1, car2);
            return -1;
        }
        if (++l >= 5) {
            break;
        }
        RotateCar(car1, dt);
        TranslateCar(car1, dt);
        RotateCar(car2, dt);
        TranslateCar(car2, dt);
    }
    if (l > 0) {
        car1->frame_collision_flag += 256;
        car2->frame_collision_flag += 256;
        if (gNet_mode == eNet_mode_host) {
            car1->last_car_car_collision = gLast_mechanics_time + harness_game_config.physics_step_time;
            car2->last_car_car_collision = gLast_mechanics_time + harness_game_config.physics_step_time;
        }
    }
    ResetCarsMass(car1, car2);
    return l;
}

// IDA: int __usercall CollideTwoCars@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, int pPass@<EBX>)
// FUNCTION: CARM95 0x0048d84a
int CollideTwoCars(tCollision_info* car1, tCollision_info* car2, int pPass) {
    int k;
    int old_k;
    int i;
    int j;
    br_scalar dist;
    br_scalar ts;
    br_bounds new_car1_bnds;
    br_bounds new_car2_bnds;
    br_bounds bnds;
    br_matrix34* mat1;
    br_matrix34* mat2;
    br_matrix34* oldmat1;
    br_matrix34* oldmat2;
    br_matrix34 inv_mat1;
    br_matrix34 inv_mat2;
    br_matrix34 inv_oldmat1;
    br_matrix34 inv_oldmat2;
    br_matrix34 car2_to_car1;
    br_matrix34 car1_to_car2;
    br_matrix34 old_car2_to_car1;
    br_matrix34 old_car1_to_car2;
    br_matrix34 car1_to_old_car1;
    br_matrix34 car2_to_old_car2;
    br_vector3 r[16];
    br_vector3 n[16];
    br_vector3 sep;
    br_vector3 tv;
    int add_point;
    // GLOBAL: CARM95 0x53cff0
    static br_vector3 oldr1;
    // GLOBAL: CARM95 0x53a568
    static br_vector3 oldr2;
    // GLOBAL: CARM95 0x53a558
    static br_vector3 oldn1;
    // GLOBAL: CARM95 0x53a548
    static br_vector3 oldn2;
    static int is_old_point_available;

    if (!gCar_car_collisions) {
        return 0;
    }
    if (pPass < 0 && !SimpleCarCarCollisionTest(car1, car2)) {
        return 0;
    }

    add_point = pPass;
    mat1 = &car1->car_master_actor->t.t.mat;
    mat2 = &car2->car_master_actor->t.t.mat;
    oldmat1 = &car1->oldmat;
    oldmat2 = &car2->oldmat;
    BrMatrix34LPInverse(&inv_oldmat1, &car1->oldmat);
    BrMatrix34LPInverse(&inv_oldmat2, &car2->oldmat);
    BrMatrix34Mul(&car1_to_old_car1, mat1, &inv_oldmat1);
    BrMatrix34Mul(&car2_to_old_car2, mat2, &inv_oldmat2);
    BrMatrix34Mul(&old_car2_to_car1, oldmat2, &inv_oldmat1);
    GetNewBoundingBox(&new_car1_bnds, &car1->bounds[1], &car1_to_old_car1);
    GetNewBoundingBox(&new_car2_bnds, &car2->bounds[1], &car2_to_old_car2);

    for (i = 0; i < 3; ++i) {
        new_car1_bnds.min.v[i] = MIN(car1->bounds[1].min.v[i], new_car1_bnds.min.v[i]);
        new_car1_bnds.max.v[i] = MAX(car1->bounds[1].max.v[i], new_car1_bnds.max.v[i]);

        new_car2_bnds.min.v[i] = MIN(car2->bounds[1].min.v[i], new_car2_bnds.min.v[i]);
        new_car2_bnds.max.v[i] = MAX(car2->bounds[1].max.v[i], new_car2_bnds.max.v[i]);
    }
    GetNewBoundingBox(&bnds, &new_car2_bnds, &old_car2_to_car1);

    if (new_car1_bnds.max.v[0] < bnds.min.v[0]
        || bnds.max.v[0] < new_car1_bnds.min.v[0]
        || new_car1_bnds.max.v[1] < bnds.min.v[1]
        || bnds.max.v[1] < new_car1_bnds.min.v[1]
        || new_car1_bnds.max.v[2] < bnds.min.v[2]
        || bnds.max.v[2] < new_car1_bnds.min.v[2]) {
        return 0;
    }
    BrMatrix34LPInverse(&inv_mat1, mat1);
    BrMatrix34LPInverse(&inv_mat2, mat2);
    BrMatrix34Mul(&car2_to_car1, mat2, &inv_mat1);
    BrMatrix34Mul(&car1_to_car2, mat1, &inv_mat2);
    BrMatrix34Mul(&old_car2_to_car1, oldmat2, &inv_oldmat1);
    BrMatrix34Mul(&old_car1_to_car2, oldmat1, &inv_oldmat2);
    BrMatrix34Mul(&car1_to_old_car1, mat1, &inv_oldmat1);
    BrMatrix34Mul(&car2_to_old_car2, mat2, &inv_oldmat2);
    do {
        k = 0;
        k += FacePointCarCarCollide(car1, car2, &car2_to_car1, &old_car2_to_car1, &car1_to_old_car1, r, n, 8, 0);
        k += FacePointCarCarCollide(car2, car1, &car1_to_car2, &old_car1_to_car2, &car2_to_old_car2, &r[2 * k], &n[2 * k], 8 - k, 1);
        old_k = k;

        if (k < 3 || add_point) {
            k += GetEdgeEdgeCollisions(&car1->bounds[1], &car2->bounds[1], &car2_to_car1, &car1_to_car2, &old_car2_to_car1, &old_car1_to_car2, &car1_to_old_car1, &r[2 * k], &n[2 * k], 8 - k);
        }
        if (k == -1) {
            TestOldMats(car1, car2, 1);
        }
        if (!k) {
            return 0;
        }
        if (k > 4) {
            i = old_k;
            j = old_k;
            while (i < k) {
                if (BrVector3Dot(&n[2 * i + 1], &r[2 * i + 1]) <= 0.0f || i - j >= k - 4) {
                    if (j != i) {
                        r[2 * j] = r[2 * i];
                    }
                    n[2 * j] = n[2 * i];
                    r[2 * j + 1] = r[2 * i + 1];
                    n[2 * j + 1] = n[2 * i + 1];
                } else {
                    j--;
                }
                i++;
                j++;
            }
            k = j;
        }
        for (i = 0; i < k; ++i) {
            BrVector3Sub(&r[2 * i], &r[2 * i], &car1->cmpos);
            BrVector3Sub(&r[2 * i + 1], &r[2 * i + 1], &car2->cmpos);
        }
        if (add_point == -1) {
            return k;
        }
        car1->doing_nothing_flag = 0;
        car2->doing_nothing_flag = 0;
        if (k < 3 && add_point) {
            for (i = 0; i < k; i++) {
                BrVector3Sub(&tv, &r[2 * i], &oldr1);
                if (BrVector3LengthSquared(&tv) < 0.01f) {
                    add_point = 0;
                }
            }
            if (add_point) {
                r[2 * k] = oldr1;
                r[2 * k + 1] = oldr2;
                n[2 * k] = oldn1;
                n[2 * k + 1] = oldn2;
                k++;
            }
        }
        oldr1 = r[0];
        oldr2 = r[1];
        oldn1 = n[0];
        oldn2 = n[1];
        if (k < 3) {
            if (car1->collision_flag && !car1->infinite_mass) {

                for (i = 0; i < k; i++) {
                    if (BrVector3Dot(&n[2 * i], &car1->old_norm) < -0.9f) {
                        car1->infinite_mass |= 0x100u;
                    }
                }
                if (!car1->infinite_mass) {
                    r[2 * k] = car1->old_point;
                    n[2 * k] = car1->old_norm;
                    BrVector3SetFloat(&n[2 * k + 1], 0.0f, 0.0f, 0.0f);
                    BrVector3SetFloat(&r[2 * k + 1], 0.0f, 0.0f, 0.0f);
                    k++;
                }
            }
            if (car2->collision_flag && !car2->infinite_mass) {
                for (i = 0; i < k; i++) {
                    if (BrVector3Dot(&n[2 * i + 1], &car2->old_norm) < -0.9f) {
                        car2->infinite_mass |= 0x100u;
                    }
                }
                if (!car2->infinite_mass) {
                    r[2 * k + 1] = car2->old_point;
                    n[2 * k + 1] = car2->old_norm;
                    BrVector3SetFloat(&n[2 * k], 0.0f, 0.0f, 0.0f);
                    BrVector3SetFloat(&r[2 * k], 0.0f, 0.0f, 0.0f);
                    k++;
                }
            }
        }
        if (car1->infinite_mass && car2->infinite_mass) {
            return -1;
        }
    } while (DoCollide(car1, car2, r, n, k, pPass, &car1_to_car2));
    return k;
}

// IDA: int __usercall GetEdgeEdgeCollisions@<EAX>(br_bounds *pB1@<EAX>, br_bounds *pB2@<EDX>, br_matrix34 *pM21@<EBX>, br_matrix34 *pM12@<ECX>, br_matrix34 *pMo21, br_matrix34 *pMo12, br_matrix34 *pM1o1, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int pMax)
// FUNCTION: CARM95 0x0048e82a
int GetEdgeEdgeCollisions(br_bounds* pB1, br_bounds* pB2, br_matrix34* pM21, br_matrix34* pM12, br_matrix34* pMo21, br_matrix34* pMo12, br_matrix34* pM1o1, br_vector3* pPoint_list, br_vector3* pNorm_list, int pMax) {
    br_vector3 p1;
    br_vector3 p2;
    br_vector3 tp1;
    br_vector3 tp2;
    br_vector3 tp3;
    br_vector3 hp1;
    br_vector3 hp2;
    br_vector3 hp3;
    br_vector3 shp1;
    br_vector3 shp2;
    br_vector3 edge;
    int plane1;
    int plane2;
    int plane3;
    br_scalar ts;
    int i;
    int j;
    int n;

    // EdgeEdge final version
    n = 0;
    if (pMax < 1) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (i == 3) {
            tp1 = pB2->min;
        } else {
            tp1 = pB2->max;
            tp1.v[i] = pB2->min.v[i];
        }
        for (j = 0; j < 3; j++) {
            tp2 = tp1;
            if (pB2->max.v[j] == tp2.v[j]) {
                tp2.v[j] = pB2->min.v[j];
            } else {
                tp2.v[j] = pB2->max.v[j];
            }

            BrMatrix34ApplyP(&p1, &tp1, pM21);
            BrMatrix34ApplyP(&p2, &tp2, pM21);
            plane1 = LineBoxColl(&p1, &p2, pB1, &hp1);
            if (plane1 == 0) {
                continue;
            }
            plane2 = LineBoxColl(&p2, &p1, pB1, &hp2);
            if (plane1 == 8 || plane2 == 8 || plane2 == 0) {
                continue;
            }
            BrVector3Add(&p1, &hp1, &hp2);
            BrVector3Scale(&p1, &p1, 0.5f);
            BrMatrix34ApplyP(&tp3, &p1, pM12);
            BrMatrix34ApplyP(&p2, &tp3, pMo21);
            plane3 = LineBoxColl(&p2, &p1, pB1, &hp3);
            // if (plane3 != 8 && plane3 != 0) {
            //     goto LABEL_25;
            // }
            if (plane3 == 8 || plane3 == 0) {
                BrVector3Sub(&tp3, &p2, &p1);
                ts = BrVector3Length(&tp3);
                ts = ts / 0.01f;
                if (ts == 0.0f) {
                    continue;
                }
                BrVector3InvScale(&tp3, &tp3, ts);
                BrVector3Accumulate(&p2, &tp3);
                plane3 = LineBoxColl(&p2, &p1, pB1, &hp3);
                if (plane3 == 8) {
                    continue;
                }
            }
            if (plane3 == 0) {
                continue;
            }

            BrMatrix34ApplyP(&shp1, &hp1, pM12);
            BrMatrix34ApplyP(&shp2, &hp2, pM12);
            if ((plane1 ^ plane2) != 4 && (plane3 == plane1 || plane3 == plane2)) {
                if (n >= pMax) {
                    return n;
                }
                GetBoundsEdge(&pPoint_list[2 * n], &edge, pB1, plane1, plane2, &p2, &hp1, &hp2, 0);
                BrVector3Accumulate(&shp1, &shp2);
                BrVector3Scale(&pPoint_list[2 * n + 1], &shp1, 0.5f);
                BrVector3Sub(&p1, &hp1, &hp2);
                BrVector3Cross(&p2, &edge, &p1);
                BrVector3Normalise(&p2, &p2);
                BrVector3Add(&p1, &pB1->max, &pB1->min);
                BrVector3Scale(&p1, &p1, 0.5f);
                BrVector3Sub(&p1, &pPoint_list[2 * n], &p1);
                if (BrVector3Dot(&p1, &p2) > 0.0) {
                    BrVector3Negate(&p2, &p2);
                }
                BrMatrix34ApplyV(&p1, &p2, pM12);
                BrMatrix34TApplyV(&pNorm_list[2 * n], &p1, pMo12);
                BrMatrix34TApplyV(&pNorm_list[2 * n + 1], &p2, pMo21);
                BrVector3Negate(&pNorm_list[2 * n + 1], &pNorm_list[2 * n + 1]);
                n++;
            } else if ((plane1 ^ plane2) == 4) {
                if (pMax - 1 <= n) {
                    return n;
                }
                GetBoundsEdge(&pPoint_list[2 * n], &edge, pB1, plane1, plane3, &p2, &hp1, &hp2, 0);
                GetBoundsEdge(&pPoint_list[2 * n + 2], &edge, pB1, plane2, plane3, &p2, &hp1, &hp2, 0);
                pPoint_list[2 * n + 1] = shp1;
                pPoint_list[2 * n + 3] = shp2;
                BrVector3Sub(&p1, &hp1, &hp2);
                BrMatrix34ApplyV(&p2, &p1, pM12);
                BrMatrix34ApplyV(&p1, &p2, pMo21);
                BrVector3Cross(&p2, &edge, &p1);
                BrVector3Normalise(&pNorm_list[2 * n], &p2);
                GetPlaneNormal(&edge, plane3);
                if (BrVector3Dot(&pNorm_list[2 * n], &edge) < 0.0f) {
                    BrVector3Negate(&pNorm_list[2 * n], &pNorm_list[2 * n]);
                }
                BrMatrix34ApplyV(&pNorm_list[2 * n + 1], &pNorm_list[2 * n], pMo12);
                BrVector3Negate(&pNorm_list[2 * n + 1], &pNorm_list[2 * n + 1]);
                BrMatrix34ApplyV(&tp3, &pNorm_list[2 * n], pM12);
                BrMatrix34ApplyV(&pNorm_list[2 * n], &tp3, pMo21);
                pNorm_list[2 * n + 2] = pNorm_list[2 * n];
                pNorm_list[2 * n + 3] = pNorm_list[2 * n + 1];
                n += 2;
            } else {
                if (pMax - 1 <= n) {
                    return n;
                }
                GetBoundsEdge(&pPoint_list[2 * n], &edge, pB1, plane1, plane3, &p2, &hp1, &hp2, 0);
                GetBoundsEdge(&pPoint_list[2 * n + 2], &edge, pB1, plane2, plane3, &p2, &hp1, &hp2, 0);
                BrMatrix34ApplyP(&pPoint_list[2 * n + 1], &hp1, pM12);
                BrMatrix34ApplyP(&pPoint_list[2 * n + 3], &hp2, pM12);
                GetPlaneNormal(&pNorm_list[2 * n], plane3);
                BrMatrix34ApplyV(&pNorm_list[2 * n + 2], &pNorm_list[2 * n], pM1o1);
                BrMatrix34ApplyV(&pNorm_list[2 * n + 1], &pNorm_list[2 * n + 2], pMo12);
                pNorm_list[2 * n] = pNorm_list[2 * n + 2];
                BrVector3Negate(&pNorm_list[2 * n + 1], &pNorm_list[2 * n + 1]);
                pNorm_list[2 * n + 3] = pNorm_list[2 * n + 1];
                n += 2;
            }
        }
    }
    return n;
}

// IDA: int __usercall FacePointCarCarCollide@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_matrix34 *pMms@<EBX>, br_matrix34 *pMoms@<ECX>, br_matrix34 *pMsos, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int pMax, int order)
// FUNCTION: CARM95 0x0048f686
int FacePointCarCarCollide(tCollision_info* car1, tCollision_info* car2, br_matrix34* pMms, br_matrix34* pMoms, br_matrix34* pMsos, br_vector3* pPoint_list, br_vector3* pNorm_list, int pMax, int order) {
    int k;
    int i;
    int j;
    int l;
    int plane;
    br_vector3 a;
    br_vector3 a1;
    br_vector3 aa;
    br_vector3 bb;
    br_vector3 norm;
    br_vector3 hp;
    br_vector3 centre;
    br_scalar dist;
    br_bounds* pStat_box;
    br_bounds* pMove_box;

    k = 0;
    pStat_box = &car2->bounds[1];
    BrVector3Add(&centre, &car2->bounds[1].max, &car2->bounds[1].min);
    BrVector3InvScale(&centre, &centre, 2.0f);
    for (i = 0; i < car2->extra_point_num + 8; i++) {
        if (i >= 8) {
            a = car2->extra_points[i - 8];
        } else {
            a.v[0] = ((i & 2) == 0) * pStat_box->min.v[0] + ((i & 2) >> 1) * pStat_box->max.v[0];
            a.v[1] = ((i & 1) == 0) * pStat_box->min.v[1] + (i & 1) * pStat_box->max.v[1];
            a.v[2] = ((i & 4) == 0) * pStat_box->max.v[2] + ((i & 4) >> 2) * pStat_box->min.v[2];
        }
        BrMatrix34ApplyP(&aa, &a, pMms);
        BrMatrix34ApplyP(&bb, &a, pMoms);
        BrVector3Sub(&aa, &aa, &bb);
        dist = BrVector3Length(&aa);
        if (dist >= 0.00001f) {
            BrVector3Scale(&a1, &aa, (0.05f / WORLD_SCALE) / dist); // 0.0072463769 * 6.9 = 0.05
            BrVector3Accumulate(&aa, &a1);
            BrVector3Accumulate(&aa, &bb);
            plane = LineBoxCollWithSphere(&bb, &aa, &car1->bounds[1], &hp);
            if ((plane & 7) != 0) {
                GetPlaneNormal(&norm, plane);

                j = 2 * k + order;
                l = 2 * k + (order == 0);
                BrMatrix34ApplyV(&pNorm_list[j], &norm, pMsos);
                BrMatrix34TApplyV(&pNorm_list[l], &pNorm_list[j], pMoms);
                BrVector3Negate(&pNorm_list[l], &pNorm_list[l]);
                if ((pNorm_list[l].v[0] >= 0.0f) != (centre.v[0] <= a.v[0]) || (pNorm_list[l].v[1] >= 0.0f) != (centre.v[1] <= a.v[1]) || (pNorm_list[l].v[2] >= 0.0f) != (a.v[2] >= centre.v[2])
                    || !TestOldMats(car1, car2, 0)) {
                    pPoint_list[l] = a;
                    pPoint_list[j] = hp;
                    k++;
                    if (pMax == k) {
                        return k;
                    }
                }
            }
        }
    }
    return k;
}

// IDA: void __usercall MungeCarsMass(tCollision_info *pCar@<EAX>, br_scalar pFactor)
// FUNCTION: CARM95 0x0048fc1c
void MungeCarsMass(tCollision_info* pCar, br_scalar pFactor) {

    pCar->M = pCar->M * pFactor;
    BrVector3Scale(&pCar->I, &pCar->I, pFactor);
}

// IDA: void __usercall ModifyCarsMass(tCollision_info *pCar_1@<EAX>, tCollision_info *pCar_2@<EDX>)
// FUNCTION: CARM95 0x0048fb97
void ModifyCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2) {

    if (pCar_1->driver > eDriver_non_car && ((tCar_spec*)pCar_1)->collision_mass_multiplier != 1.0f) {
        MungeCarsMass(pCar_1, ((tCar_spec*)pCar_1)->collision_mass_multiplier);
    }
    if (pCar_2->driver > eDriver_non_car && ((tCar_spec*)pCar_2)->collision_mass_multiplier != 1.0f) {
        MungeCarsMass(pCar_2, ((tCar_spec*)pCar_2)->collision_mass_multiplier);
    }
}

// IDA: void __usercall ResetCarsMass(tCollision_info *pCar_1@<EAX>, tCollision_info *pCar_2@<EDX>)
// FUNCTION: CARM95 0x0048fc7b
void ResetCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2) {

    if (pCar_1->driver > eDriver_non_car && ((tCar_spec*)pCar_1)->collision_mass_multiplier != 1.0f) {
        MungeCarsMass(pCar_1, 1.0f / ((tCar_spec*)pCar_1)->collision_mass_multiplier);
    }
    if (pCar_2->driver > eDriver_non_car && ((tCar_spec*)pCar_2)->collision_mass_multiplier != 1.0f) {
        MungeCarsMass(pCar_2, 1.0f / ((tCar_spec*)pCar_2)->collision_mass_multiplier);
    }
}

// IDA: int __usercall DoCollide@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_vector3 *r@<EBX>, br_vector3 *n@<ECX>, int k, int pPass, br_matrix34 *mat1_to_mat2)
// FUNCTION: CARM95 0x0048fd16
int DoCollide(tCollision_info* car1, tCollision_info* car2, br_vector3* r, br_vector3* n, int k, int pPass, br_matrix34* mat1_to_mat2) {
    br_matrix34* mat1;
    br_matrix34* mat2;
    br_matrix34* oldmat1;
    br_matrix34* oldmat2;
    br_matrix4 M;
    br_vector3 tau1[8];
    br_vector3 a;
    br_vector3 norm;
    br_vector3 f1;
    br_vector3 f2;
    br_vector3 pos1;
    br_vector3 pos2;
    br_vector3 max_friction;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3* tau2;
    br_vector3 torque1;
    br_vector3 torque2;
    br_scalar f[4];
    br_scalar d[4];
    br_scalar ts;
    br_scalar tforce;
    int i;
    int j;
    int car1_point;
    int car2_point;
    int plane;
    int move_car1;
    int move_car2;
    br_vector3 a2;
    br_vector3 f12;
    br_vector3 f22;
    br_vector3 point_vel1;
    br_vector3 point_vel2;
    br_scalar fudge_multiplier;
    br_scalar factor;
    int need_to_fudge;

    mat1 = &car1->car_master_actor->t.t.mat;
    mat2 = &car2->car_master_actor->t.t.mat;
    oldmat1 = &car1->oldmat;
    oldmat2 = &car2->oldmat;
    tau2 = &tau1[4];
    move_car1 = !car1->infinite_mass && car1->min_torque_squared == 0.0f;
    move_car2 = !car2->infinite_mass && car2->min_torque_squared == 0.0f;
    if (k > 4) {
        k = 4;
    }
    BrMatrix34Copy(mat1, oldmat1);
    BrMatrix34Copy(mat2, oldmat2);
    BrVector3Copy(&car1->omega, &car1->oldomega);
    BrVector3Copy(&car2->omega, &car2->oldomega);
    BrMatrix34TApplyV(&car1->velocity_car_space, &car1->v, mat1);
    BrMatrix34TApplyV(&car2->velocity_car_space, &car2->v, mat2);
    need_to_fudge = 1;
    for (i = 0; i < k; ++i) {
        BrVector3Cross(&tau1[i], &r[2 * i], &n[2 * i]);
        BrVector3Cross(&tau2[i], &r[2 * i + 1], &n[2 * i + 1]);
        Vector3Div(&tau1[i], &tau1[i], &car1->I);
        Vector3Div(&tau2[i], &tau2[i], &car2->I);
        BrVector3Cross(&tv, &car1->omega, &r[2 * i]);
        BrVector3Accumulate(&tv, &car1->velocity_car_space);
        d[i] = -BrVector3Dot(&n[2 * i], &tv);
        BrVector3Cross(&tv, &car2->omega, &r[2 * i + 1]);
        BrVector3Accumulate(&tv, &car2->velocity_car_space);
        d[i] -= BrVector3Dot(&n[2 * i + 1], &tv);
        if (d[i] > 0.0f) {
            need_to_fudge = 0;
        }
    }
    if (need_to_fudge) {
        d[0] = 0.5f;
    }
    factor = 0.0f;
    if (move_car1) {
        factor = car1->M + factor;
    }
    if (move_car2) {
        factor = car2->M + factor;
    }
    for (i = 0; i < k; i++) {
        for (j = 0; j < k; j++) {
            if (move_car1) {
                BrVector3Cross(&tv, &tau1[j], &r[2 * i]);
                BrVector3InvScale(&norm, &n[2 * j], car1->M);
                BrVector3Accumulate(&tv, &norm);
                M.m[i][j] = BrVector3Dot(&n[2 * i], &tv);
            } else {
                M.m[i][j] = 0.0f;
            }
            if (move_car2) {
                BrVector3Cross(&tv, &tau2[j], &r[2 * i + 1]);
                BrVector3InvScale(&norm, &n[2 * j + 1], car2->M);
                BrVector3Accumulate(&tv, &norm);
                M.m[i][j] += BrVector3Dot(&n[2 * i + 1], &tv);
            }
            M.m[i][j] *= factor;
        }
    }
    switch (k) {
    case 1:
        ts = SinglePointColl(f, &M, d);
        break;
    case 2:
        ts = TwoPointCollB(f, &M, d, tau1, n);
        break;
    case 3:
        d[3] = 0.f;
        ts = ThreePointCollRecB(f, &M, d, tau1, n);
        break;
    case 4:
        ts = FourPointCollB(f, &M, d, tau1, n);
        break;
    default:
        ts = 0;
        break;
    }
    if (k > 3) {
        k = 3;
    }
    if (fabs(ts) <= 0.000001f) {
        return 0;
    }
    BrVector3SetFloat(&f1, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&f2, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&pos1, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&pos2, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&torque1, 0.0f, 0.0f, 0.0f);
    BrVector3SetFloat(&torque2, 0.0f, 0.0f, 0.0f);
    tforce = 0.0f;
    car1_point = -1;
    car2_point = -1;
    for (i = 0; i < k; i++) {
        f[i] = f[i] * factor;
        if (f[i] == 0.0f && k != 0) {
            break;
        }
        if (f[i] < 0.001f) {
            f[i] = 0.001f;
        }
        if (f[i] > 10.0f) {
            ts = 0.0f;
        }
        f[i] += 0.1f;
        BrVector3Scale(&tau1[i], &tau1[i], f[i]);
        BrVector3Scale(&tau2[i], &tau2[i], f[i]);
        BrVector3Accumulate(&torque1, &tau1[i]);
        BrVector3Accumulate(&torque2, &tau2[i]);
        if (!pPass && Vector3IsZero(&n[2 * i])) {
            car2_point = i;
        } else if (!pPass && Vector3IsZero(&n[2 * i + 1])) {
            car1_point = i;
        } else {
            ts = f[i] / car1->M;
            BrVector3Scale(&tv2, &n[2 * i], ts);
            BrVector3Accumulate(&f1, &tv2);
            ts = f[i] / car2->M;
            BrVector3Scale(&tv2, &n[2 * i + 1], ts);
            BrVector3Accumulate(&f2, &tv2);
            BrVector3Scale(&tv2, &r[2 * i], f[i]);
            BrVector3Accumulate(&pos1, &tv2);
            BrVector3Scale(&tv2, &r[2 * i + 1], f[i]);
            BrVector3Accumulate(&pos2, &tv2);
            tforce += f[i];
        }
    }
    if (car1->min_torque_squared != 0.0f && !car1->infinite_mass && BrVector3LengthSquared(&torque1) > car1->min_torque_squared) {
        BrVector3Scale(&car1->omega, &car1->omega, gDt);
        car1->omega.v[0] += torque1.v[0] * 0.04f;
        car1->omega.v[2] += torque1.v[2] * 0.04f;
        if (BrVector3LengthSquared(&car1->omega) > car1->break_off_radians_squared) {
            car1->min_torque_squared = 0.0f;
            return 1;
        }
        BrVector3InvScale(&car1->omega, &car1->omega, gDt);
    }
    if (car2->min_torque_squared != 0.0f && !car2->infinite_mass && BrVector3LengthSquared(&torque2) > car2->min_torque_squared) {
        BrVector3Scale(&car2->omega, &car2->omega, gDt);
        car2->omega.v[0] += torque2.v[0] * 0.04f;
        car2->omega.v[2] += torque2.v[2] * 0.04f;
        if (BrVector3LengthSquared(&car2->omega) > car2->break_off_radians_squared) {
            car2->min_torque_squared = 0.0f;
            return 1;
        }
        BrVector3InvScale(&car2->omega, &car2->omega, gDt);
    }
    if (move_car1) {
        BrVector3Accumulate(&car1->omega, &torque1);
    }
    if (move_car2) {
        BrVector3Accumulate(&car2->omega, &torque2);
    }
    BrVector3InvScale(&pos1, &pos1, tforce);
    BrVector3InvScale(&pos2, &pos2, tforce);
    if (pPass == 0) {
        if (car1_point >= 0 && move_car1) {
            f[car1_point] = f[car1_point] / car1->M;
            BrVector3Scale(&n[2 * car1_point], &n[2 * car1_point], f[car1_point]);
            BrVector3Cross(&tv2, &car1->oldomega, &r[2 * car1_point]);
            BrVector3Accumulate(&tv2, &car1->velocity_car_space);
            ts = BrVector3Length(&tv2);
            if (ts > 0.0001f && (car1->driver <= eDriver_non_car || !CAR(car1)->wall_climber_mode)) {
                AddFriction(car1, &tv2, &n[2 * car1_point], &r[2 * car1_point], f[car1_point], &max_friction);
                BrVector3Accumulate(&n[2 * car1_point], &max_friction);
            }
            BrMatrix34ApplyV(&tv, &n[2 * car1_point], mat1);
            BrVector3Accumulate(&car1->v, &tv);
        }
        if (car2_point >= 0 && move_car2) {
            f[car2_point] = f[car2_point] / car2->M;
            BrVector3Scale(&n[2 * car2_point + 1], &n[2 * car2_point + 1], f[car2_point]);
            BrVector3Cross(&tv2, &car2->oldomega, &r[2 * car2_point + 1]);
            BrVector3Accumulate(&tv2, &car2->velocity_car_space);
            ts = BrVector3Length(&tv2);

            if (ts > 0.0001f && (car1->driver <= eDriver_non_car || !(CAR(car1)->wall_climber_mode))) {
                AddFriction(car2, &tv2, &n[2 * car2_point + 1], &r[2 * car2_point + 1], f[car2_point], &max_friction);
                BrVector3Accumulate(&n[2 * car2_point + 1], &max_friction);
            }
            BrMatrix34ApplyV(&tv, &n[2 * car2_point + 1], mat2);
            BrVector3Accumulate(&car2->v, &tv);
        }
        if (tforce != 0.0f) {
            BrVector3Cross(&point_vel1, &car1->oldomega, &pos1);
            BrVector3Sub(&a, &car1->v, &car2->v);
            BrMatrix34TApplyV(&tv2, &a, mat1);
            BrVector3Accumulate(&point_vel1, &tv2);
            BrVector3Cross(&point_vel2, &car2->oldomega, &pos2);
            AddFrictionCarToCar(car1, car2, &point_vel1, &point_vel2, &f1, &pos1, &pos2, tforce, &max_friction);
            if (TestForNan(max_friction.v) || TestForNan(&max_friction.v[1]) || TestForNan(&max_friction.v[2])) {
                BrVector3SetFloat(&max_friction, 0.0f, 0.0f, 0.0f);
            }
            BrVector3InvScale(&tv2, &max_friction, car1->M);
            BrVector3Accumulate(&f1, &tv2);
            BrMatrix34ApplyV(&tv2, &max_friction, mat1);
            BrMatrix34TApplyV(&max_friction, &tv2, mat2);
            BrVector3InvScale(&tv2, &max_friction, car2->M);
            BrVector3Accumulate(&f2, &tv2);
        }
    }
    if (tforce == 0.0f) {
        return 0;
    }
    BrMatrix34ApplyV(&tv, &f1, mat1);
    if (move_car1) {
        BrVector3Accumulate(&car1->v, &tv);
    }
    CrushAndDamageCar(CAR(car1), &pos1, &f1, CAR(car2));
    if ((car1->infinite_mass & 0x100) != 0) {
        BrVector3Sub(&tv2, &car1->cmpos, &pos1);
        BrVector3Accumulate(&tv2, &car1->cmpos);
        ts = BrVector3Length(&f1);
        if (ts > 0.0001f) {
            ts = 5.0f / ts;
            BrVector3Scale(&a, &f1, ts);
            BrVector3Accumulate(&tv2, &a);
            plane = LineBoxColl(&tv2, &pos1, &car1->bounds[1], &a);
            if (plane) {
                BrVector3Negate(&f12, &f1);
                CrushAndDamageCar(CAR(car1), &a, &f12, CAR(car2));
            }
        }
    }
    BrMatrix34ApplyV(&tv, &f2, mat2);
    if (move_car2) {
        BrVector3Accumulate(&car2->v, &tv);
    }
    CrushAndDamageCar(CAR(car2), &pos2, &f2, CAR(car1));
    if ((car2->infinite_mass & 0x100) != 0) {
        BrVector3Sub(&tv2, &car2->cmpos, &pos2);
        BrVector3Accumulate(&tv2, &car2->cmpos);
        ts = BrVector3Length(&f2);
        if (ts > 0.0001f) {
            ts = 5.0f / ts;
            BrVector3Scale(&a, &f2, ts);
            BrVector3Accumulate(&tv2, &a);
            plane = LineBoxColl(&tv2, &pos2, &car2->bounds[1], &a);
            if (plane) {
                BrVector3Negate(&f22, &f2);
                CrushAndDamageCar(CAR(car2), &a, &f22, CAR(car1));
            }
        }
    }
    BrMatrix34ApplyP(&tv2, &pos1, mat1);
    BrVector3InvScale(&tv2, &tv2, WORLD_SCALE);
    BrVector3Scale(&f1, &f1, 5.0f);
    CrashNoise(&f1, &tv2, 0);
    BrVector3Add(&a, &car2->v, &car1->v);
    BrVector3Scale(&a, &a, 0.25f / WORLD_SCALE);
    BrVector3Scale(&tv, &tv, car2->M * 3.0f);
    CreateSparkShower(&tv2, &a, &tv, CAR(car1), CAR(car2));
    return 0;
}

// IDA: br_scalar __usercall TwoPointCollB@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n)
// FUNCTION: CARM95 0x004927be
br_scalar TwoPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    br_scalar ts;

    ts = m->m[1][1] * m->m[0][0] - m->m[0][1] * m->m[1][0];
    if (fabs(ts) > 0.000001) {
        f[0] = (m->m[1][1] * d[0] - m->m[0][1] * d[1]) / ts;
        f[1] = (m->m[1][0] * d[0] - m->m[0][0] * d[1]) / (-ts);
    }
    if (f[1] < 0.0f) {
        ts = SinglePointColl(f, m, d);
        f[1] = 0.f;
    } else if (f[0] < 0.f) {
        m->m[0][0] = m->m[1][1];
        BrVector3Copy(&tau[0], &tau[1]);
        BrVector3Copy(&tau[4], &tau[5]);
        BrVector3Copy(&n[0], &n[2]);
        BrVector3Copy(&n[1], &n[3]);
        d[0] = d[1];
        ts = SinglePointColl(f, m, d);
        f[1] = 0.0f;
    }
    return ts;
}

// IDA: br_scalar __usercall ThreePointCollRecB@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n)
// FUNCTION: CARM95 0x00492961
br_scalar ThreePointCollRecB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    int i;
    int j;
    br_scalar ts;

    ts = ThreePointColl(f, m, d);
    if (f[0] >= 0.0f && f[1] >= 0.0f && f[2] >= 0.0f) {
        return ts;
    }
    if (f[2] < 0.f) {
        i = 0;
        j = 1;
    } else if (f[1] < 0.f) {
        i = 0;
        j = 2;
    } else if (f[0] < 0.f) {
        i = 1;
        j = 2;
    } else {
        return 0.0f;
    }
    m->m[0][0] = m->m[i][i];
    m->m[1][0] = m->m[j][i];
    m->m[0][1] = m->m[i][j];
    m->m[1][1] = m->m[j][j];
    BrVector3Copy(&tau[0], &tau[i]);
    BrVector3Copy(&tau[1], &tau[j]);
    BrVector3Copy(&tau[4], &tau[i + 4]);
    BrVector3Copy(&tau[5], &tau[j + 4]);
    BrVector3Copy(&n[0], &n[2 * i]);
    BrVector3Copy(&n[2], &n[2 * j]);
    BrVector3Copy(&n[1], &n[2 * i + 1]);
    BrVector3Copy(&n[3], &n[2 * j + 1]);
    d[0] = d[i];
    d[1] = d[j];
    ts = TwoPointCollB(f, m, d, tau, n);
    f[2] = 0.0;
    return ts;
}

// IDA: br_scalar __usercall FourPointCollB@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n)
// FUNCTION: CARM95 0x00492cdd
br_scalar FourPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    int i;
    int j;
    int l;
    br_scalar ts;

    ts = ThreePointColl(f, m, d);
    if (f[0] >= 0.0f && f[1] >= 0.0f && f[2] >= 0.0f) {
        return ts;
    }
    if (f[0] < 0.0f) {
        l = 0;
    } else if (f[1] < 0.0f) {
        l = 1;
    } else {
        l = 2;
    }
    for (i = l; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            m->m[i][j] = m->m[i + 1][j];
        }
        d[i] = d[i + 1];
        tau[i] = tau[i + 1];
        tau[i + 4] = tau[i + 5];
        n[2 * i] = n[2 * i + 2];
        n[2 * i + 1] = n[2 * i + 3];
        d[i] = d[i + 1];
    }
    for (i = l; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m->m[j][i] = m->m[j][i + 1];
        }
    }
    return ThreePointCollRecB(f, m, d, tau, n);
}

// IDA: int __usercall TestForNan@<EAX>(float *f@<EAX>)
// FUNCTION: CARM95 0x00492ff8
int TestForNan(float* f) {
    tU32 i;

#ifdef DETHRACE_FIX_BUGS
    return isnan(*f);
#else
    memcpy(&i, f, sizeof i);
    i = ~i & 0x7F800000;
    return i == 0;
#endif
}

// IDA: void __cdecl CheckCameraHither()
// FUNCTION: CARM95 0x00493036
void CheckCameraHither(void) {
    br_camera* cam;
    // GLOBAL: CARM95 0x514ea4
    static int old_hither;

    cam = (br_camera*)gCamera->type_data;
    if (TestForNan(&cam->hither_z)) {
        cam->hither_z = (float)old_hither;
    }
    old_hither = (int)cam->hither_z;
}

// IDA: void __usercall SetCarSuspGiveAndHeight(tCar_spec *pCar@<EAX>, br_scalar pFront_give_factor, br_scalar pRear_give_factor, br_scalar pDamping_factor, br_scalar pExtra_front_height, br_scalar pExtra_rear_height)
// FUNCTION: CARM95 0x00493087
void SetCarSuspGiveAndHeight(tCar_spec* pCar, br_scalar pFront_give_factor, br_scalar pRear_give_factor, br_scalar pDamping_factor, br_scalar pExtra_front_height, br_scalar pExtra_rear_height) {
    br_scalar front_give;
    br_scalar rear_give;
    br_scalar damping;
    br_scalar ratio;
    int i;

#define UNK_SUSPENION_FACTOR 5.0f

    front_give = pCar->susp_give[1] * pFront_give_factor * WORLD_SCALE;
    rear_give = pCar->susp_give[0] * pRear_give_factor * WORLD_SCALE;
    damping = pCar->damping * pDamping_factor;
    ratio = fabs((pCar->wpos[0].v[2] - pCar->cmpos.v[2]) / (pCar->wpos[2].v[2] - pCar->cmpos.v[2]));
    pCar->sk[0] = pCar->M / (ratio + 1.0f) * UNK_SUSPENION_FACTOR / rear_give;
    pCar->sb[0] = pCar->M / (ratio + 1.0f) * sqrt(UNK_SUSPENION_FACTOR) / sqrt(rear_give);
    ratio = 1.0f / ratio;
    pCar->sk[1] = pCar->M / (ratio + 1.0f) * UNK_SUSPENION_FACTOR / front_give;
    pCar->sb[1] = pCar->M / (ratio + 1.0f) * sqrt(UNK_SUSPENION_FACTOR) / sqrt(front_give);

    pCar->sb[0] *= damping;
    pCar->sb[1] *= damping;
    pCar->susp_height[0] = pCar->ride_height + rear_give + pExtra_rear_height;
    pCar->susp_height[1] = pCar->ride_height + front_give + pExtra_front_height;

    pCar->bounds[0].min.v[1] = -MAX(rear_give, front_give) + -MAX(pExtra_rear_height, pExtra_front_height);
    pCar->bounds[0].min.v[1] /= WORLD_SCALE;

#undef UNK_SUSPENION_FACTOR
}

// IDA: int __usercall TestForCarInSensiblePlace@<EAX>(tCar_spec *car@<EAX>)
// FUNCTION: CARM95 0x0049327e
int TestForCarInSensiblePlace(tCar_spec* car) {
    br_bounds bnds;
    br_matrix34 mat;
    br_matrix34* mat1;
    br_matrix34* mat2;
    int i;
    int j;
    int k;
    tCollision_info* c2;
    tCollision_info* car_info;
    br_vector3 sep;
    br_vector3 tv;
    br_vector3 tv2;

    car_info = (tCollision_info*)car;
    if (!gProgram_state.racing) {
        return 1;
    }
    mat1 = &car_info->car_master_actor->t.t.mat;
    if (!gDoing_physics) {
        BrVector3Scale((br_vector3*)mat1->m[3], (br_vector3*)mat1->m[3], WORLD_SCALE);
    }
    GetFacesInBox(car_info);
    BrMatrix34ApplyP(&car_info->pos, &car_info->cmpos, mat1);
    BrVector3InvScale(&car_info->pos, &car_info->pos, WORLD_SCALE);
    k = CollCheck(car_info, -2.f);
    if (!k) {
        if (gNum_active_non_cars + gNum_active_cars != gNum_cars_and_non_cars) {
            GetNonCars();
        }
        for (i = 0; i < gNum_cars_and_non_cars; i++) {
            c2 = (tCollision_info*)gActive_car_list[i];
            if (car_info != c2) {
                mat2 = &c2->car_master_actor->t.t.mat;
                BrVector3Scale((br_vector3*)mat2->m[3], (br_vector3*)mat2->m[3], WORLD_SCALE);
                BrVector3Sub(&sep, (br_vector3*)mat1->m[3], (br_vector3*)mat2->m[3]);
                if (BrVector3LengthSquared(&sep) <= 100.0) {
                    k += TestOldMats(car_info, c2, 0);
                    k += TestOldMats(c2, car_info, 0);
                }
                if (k != 0) {
                    BrMatrix34ApplyP(&tv, &car_info->cmpos, mat1);
                    BrMatrix34ApplyP(&tv2, &c2->cmpos, mat2);
                    BrVector3Sub(&tv, &tv2, &tv);
                    BrMatrix34TApplyV(&car_info->old_point, &tv, mat1);
                    BrVector3Normalise(&car_info->old_norm, &car_info->old_point);
                    BrVector3Negate(&car_info->old_norm, &car_info->old_norm);
                }
                BrVector3InvScale((br_vector3*)mat2->m[3], (br_vector3*)mat2->m[3], WORLD_SCALE);
                if (k != 0) {
                    break;
                }
            }
        }
    }
    if (!gDoing_physics) {
        BrVector3InvScale((br_vector3*)mat1->m[3], (br_vector3*)mat1->m[3], WORLD_SCALE);
    }
    if (k != 0) {
        return 0;
    } else {
        return -1;
    }
}

// IDA: int __usercall TestOldMats@<EAX>(tCollision_info *c1@<EAX>, tCollision_info *c2@<EDX>, int newmats@<EBX>)
// FUNCTION: CARM95 0x004936e6
int TestOldMats(tCollision_info* c1, tCollision_info* c2, int newmats) {
    br_vector3 p1;
    br_vector3 p2;
    br_vector3 tp1;
    br_vector3 tp2;
    br_vector3 tp3;
    br_vector3 hp1;
    br_vector3 hp2;
    br_vector3 hp3;
    br_vector3 shp1;
    br_vector3 shp2;
    br_vector3 edge;
    int plane1;
    int plane2;
    int plane3;
    br_scalar ts;
    int i;
    int j;
    int n;
    br_bounds* b1;
    br_bounds* b2;
    br_matrix34 invmat1;
    br_matrix34 mat21;

    n = 0;
    b1 = &c1->bounds[1];
    b2 = &c2->bounds[1];
    if (newmats) {
        BrMatrix34LPInverse(&invmat1, &c1->car_master_actor->t.t.mat);
        BrMatrix34Mul(&mat21, &c2->car_master_actor->t.t.mat, &invmat1);
    } else {
        BrMatrix34LPInverse(&invmat1, &c1->oldmat);
        BrMatrix34Mul(&mat21, &c2->oldmat, &invmat1);
    }
    for (i = 0; i < 4; i++) {
        if (i == 3) {
            BrVector3Copy(&edge, &b2->min);
        } else {
            BrVector3Copy(&edge, &b2->max);
            edge.v[i] = b2->min.v[i];
        }
        for (j = 0; j < 3; j++) {
            BrVector3Copy(&tp1, &edge);
            if (b2->max.v[j] == tp1.v[j]) {
                tp1.v[j] = b2->min.v[j];
            } else {
                tp1.v[j] = b2->max.v[j];
            }
            BrMatrix34ApplyP(&p1, &edge, &mat21);
            BrMatrix34ApplyP(&p2, &tp1, &mat21);
            if (LineBoxColl(&p1, &p2, b1, &hp1)) {
                n++;
            }
        }
    }
    return n;
}

// IDA: int __usercall PullActorFromWorld@<EAX>(br_actor *pActor@<EAX>)
// FUNCTION: CARM95 0x0049393a
int PullActorFromWorld(br_actor* pActor) {

    if (gDoing_physics) {
        return DoPullActorFromWorld(pActor);
    }
    return 0;
}

// IDA: int __usercall DoPullActorFromWorld@<EAX>(br_actor *pActor@<EAX>)
// FUNCTION: CARM95 0x0049396a
int DoPullActorFromWorld(br_actor* pActor) {
    int num;
    int i;
    tCollision_info* c;
    tNon_car_spec* non_car;

    non_car = NULL;
    num = 10 * (pActor->identifier[1] - '0') + 1 * (pActor->identifier[2] - '0');
    if (gNon_car_spec_list[num]) {
        non_car = &gProgram_state.non_cars[gNon_car_spec_list[num] + 4];
    }
    if (non_car && non_car->collision_info.driver == eDriver_non_car) {
        non_car = gProgram_state.non_cars;
        for (i = 0; i < NONCAR_UNUSED_SLOTS; i++) {
            if (non_car->collision_info.driver == eDriver_non_car_unused_slot) {
                break;
            }
            non_car++;
        }
        if (i == NONCAR_UNUSED_SLOTS) {
            non_car = NULL;
        } else {
            memcpy(non_car, &gProgram_state.non_cars[gNon_car_spec_list[num] + NONCAR_UNUSED_SLOTS - 1], sizeof(tNon_car_spec));
        }
    }
    if (non_car != NULL) {
        pActor->type_data = non_car;
        c = &non_car->collision_info;
        c->driver = eDriver_non_car;
        c->doing_nothing_flag = 1;
        BrActorRemove(pActor);
        BrActorAdd(gNon_track_actor, pActor);
        c->car_master_actor = pActor;
        c->car_ID = 100 * (pActor->identifier[5] - '0') + 10 * (pActor->identifier[6] - '0') + 1 * (pActor->identifier[7] - '0');
        gActive_non_car_list[gNum_active_non_cars] = non_car;
        gNum_active_non_cars++;
        gActive_car_list[gNum_cars_and_non_cars] = (tCar_spec*)non_car;
        gNum_cars_and_non_cars++;
        GetNewBoundingBox(&c->bounds_world_space, c->bounds, &pActor->t.t.mat);
        non_car->collision_info.bounds_ws_type = eBounds_ws;
        InitialiseNonCar(non_car);
        ResetCarSpecialVolume((tCollision_info*)non_car);
        if (gDoing_physics) {
            BrVector3Scale((br_vector3*)&pActor->t.t.mat.m[3][0], (br_vector3*)&pActor->t.t.mat.m[3][0], WORLD_SCALE);
        }
        BrMatrix34Copy(&c->oldmat, &pActor->t.t.mat);
        if (!gDoing_physics) {
            BrVector3Scale((br_vector3*)&c->oldmat.m[3][0], (br_vector3*)&c->oldmat.m[3][0], WORLD_SCALE);
        }
        PipeSingleNonCar((tCollision_info*)non_car);
        return 1;
    } else {
        pActor->identifier[1] = 'x';
        return 0;
    }
}

// IDA: void __usercall CheckForDeAttachmentOfNonCars(tU32 pTime@<EAX>)
// FUNCTION: CARM95 0x00493c46
void CheckForDeAttachmentOfNonCars(tU32 pTime) {
    // GLOBAL: CARM95 0x514ea8
    static tU32 total_time;
    br_bounds bnds;
    int i;
    int j;
    int last_free_slot;
    int drop;
    tCollision_info* c;
    tCollision_info* c2;
    br_actor* actor;
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    br_matrix34 mat;

    if (gNum_active_non_cars == 0) {
        return;
    }

    last_free_slot = 0;
    track_spec = &gProgram_state.track_spec;

    StartPipingSession(ePipe_chunk_non_car);
    for (i = 0; i < gNum_active_non_cars; i++) {
        if (!gActive_non_car_list[i]->collision_info.doing_nothing_flag) {
            AddNonCarToPipingSession(gActive_non_car_list[i]->collision_info.car_ID, gActive_non_car_list[i]->collision_info.car_master_actor);
        }
    }
    EndPipingSession();

    total_time += pTime;
    if (total_time < 1000) {
        return;
    }
    total_time = 0;
    for (i = 0; i < gNum_active_non_cars; i++) {
        c = &gActive_non_car_list[i]->collision_info;
        if (c->car_master_actor->t.t.translate.t.v[1] < gMin_world_y) {
            c->doing_nothing_flag = 1;
        }
        if (TestForNan(&c->car_master_actor->t.t.mat.m[3][1])) {
            BrVector3Set(&c->omega, 0.0f, 0.0f, 0.0f);
            BrMatrix34Identity(&c->car_master_actor->t.t.mat);
            BrVector3Set(&c->car_master_actor->t.t.translate.t, 2000.f, 0.f, 0.f);
            c->doing_nothing_flag = 1;
        }
        actor = c->car_master_actor;
        gActive_non_car_list[last_free_slot] = gActive_non_car_list[i];
        if (c->doing_nothing_flag) {
            drop = 1;
            for (j = 0; j < gNum_cars_and_non_cars; j++) {
                c2 = (tCollision_info*)gActive_car_list[j];
                if (c2 != c && !c2->doing_nothing_flag) {
                    BrMatrix34Mul(&mat, &actor->t.t.mat, &c2->last_box_inv_mat);
                    GetNewBoundingBox(&bnds, &actor->model->bounds, &mat);
                    if (c2->last_box.max.v[0] >= bnds.min.v[0]
                        && c2->last_box.max.v[1] >= bnds.min.v[1]
                        && c2->last_box.max.v[2] >= bnds.min.v[2]
                        && c2->last_box.min.v[0] <= bnds.max.v[0]
                        && c2->last_box.min.v[1] <= bnds.max.v[1]
                        && c2->last_box.min.v[2] <= bnds.max.v[2]) {
                        drop = 0;
                        break;
                    }
                }
            }
            if (drop) {
                BrActorRemove(actor);
                c->driver = eDriver_non_car_unused_slot;
                last_free_slot--;
                XZToColumnXZ(&cx, &cz, actor->t.t.mat.m[3][0], actor->t.t.mat.m[3][2], track_spec);
                if (track_spec->columns[cz][cx] != NULL) {
                    BrActorAdd(track_spec->columns[cz][cx], actor);
                } else {
                    BrActorAdd(gTrack_actor, actor);
                }
            }
        }
        last_free_slot++;
    }
    gNum_active_non_cars = last_free_slot;
}

// IDA: void __usercall AdjustNonCar(br_actor *pActor@<EAX>, br_matrix34 *pMat@<EDX>)
// FUNCTION: CARM95 0x00493ff2
void AdjustNonCar(br_actor* pActor, br_matrix34* pMat) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;

    track_spec = &gProgram_state.track_spec;
    BrMatrix34Copy(&pActor->t.t.mat, pMat);
    if (pActor->parent != gNon_track_actor) {
        XZToColumnXZ(&cx, &cz, pActor->t.t.translate.t.v[0], pActor->t.t.translate.t.v[2], track_spec);
        if (track_spec->columns[cz][cx] != pActor->parent && track_spec->columns[cz][cx] != NULL) {
            BrActorRemove(pActor);
            BrActorAdd(track_spec->columns[cz][cx], pActor);
        }
    }
}

// IDA: void __usercall PipeSingleNonCar(tCollision_info *c@<EAX>)
// FUNCTION: CARM95 0x004940c2
void PipeSingleNonCar(tCollision_info* c) {

    StartPipingSession(ePipe_chunk_non_car);
    if (gDoing_physics) {
        BrVector3InvScale(&c->car_master_actor->t.t.translate.t, &c->car_master_actor->t.t.translate.t, WORLD_SCALE);
    }
    AddNonCarToPipingSession(c->car_ID, c->car_master_actor);
    if (gDoing_physics) {
        BrVector3Scale(&c->car_master_actor->t.t.translate.t, &c->car_master_actor->t.t.translate.t, WORLD_SCALE);
    }
    EndPipingSession();
}

// IDA: int __usercall GetPrecalculatedFacesUnderCar@<EAX>(tCar_spec *pCar@<EAX>, tFace_ref **pFace_refs@<EDX>)
// FUNCTION: CARM95 0x004941a2
int GetPrecalculatedFacesUnderCar(tCar_spec* pCar, tFace_ref** pFace_refs) {

    if (pCar->box_face_ref == gFace_num__car
        || (pCar->box_face_ref == gFace_num__car - 1 && pCar->box_face_start > gFace_count)) {
        *pFace_refs = &gFace_list__car[pCar->box_face_start];
        return pCar->box_face_end - pCar->box_face_start;
    }
    return 0;
}

// IDA: br_material* __cdecl SomeNearbyMaterial()
br_material* SomeNearbyMaterial(void) {

    return gFace_list__car[gProgram_state.current_car.box_face_start].material;
}
