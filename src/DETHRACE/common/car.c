#include "car.h"
#include "brender.h"
#include "constants.h"
#include "controls.h"
#include "crush.h"
#include "displays.h"
#include "finteray.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "netgame.h"
#include "oil.h"
#include "opponent.h"
#include "pd/sys.h"
#include "piping.h"
#include "raycast.h"
#include "replay.h"
#include "skidmark.h"
#include "spark.h"
#include "trig.h"
#include "utility.h"
#include "world.h"
#include <math.h>
#include <stdlib.h>

int gDoing_physics;
br_scalar gDt;
// suffix added to avoid duplicate symbol
int gCollision_detection_on__car = 1;
// suffix added to avoid duplicate symbol
br_vector3 gGround_normal__car;
// suffix added to avoid duplicate symbol
void (*ControlCar[6])(tCar_spec*, br_scalar) = {
    &ControlCar1,
    &ControlCar2,
    &ControlCar3,
    &ControlCar4,
    &ControlCar5,
    NULL
};
int gControl__car = 3; // suffix added to avoid duplicate symbol
int gFace_num__car = 1; // suffix added to avoid duplicate symbol
br_angle gOld_yaw__car; // suffix added to avoid duplicate symbol
br_angle gOld_zoom;
br_vector3 gCamera_pos_before_collide;
int gMetal_crunch_sound_id__car[5]; // suffix added to avoid duplicate symbol
int gMetal_scrape_sound_id__car[3]; // suffix added to avoid duplicate symbol
int gCar_car_collisions;
int gFreeze_mechanics;
tU32 gLast_cunning_stunt;
tU32 gWild_start;
tU32 gQuite_wild_start;
tU32 gQuite_wild_end;
tU32 gOn_me_wheels_start;
int gWoz_upside_down_at_all;
tS3_sound_tag gSkid_tag[2];
tCar_spec* gLast_car_to_skid[2];
int gEliminate_faces;
br_vector3 gZero_v__car; // suffix added to avoid duplicate symbol
tU32 gSwitch_time;
tSave_camera gSave_camera[2];
tU32 gLast_mechanics_time;
int gOpponent_viewing_mode;
int gNet_player_to_view_index;
int gDouble_pling_water;
int gStop_opponents_moving;
float gDefensive_powerup_factor[6];
float gOffensive_powerup_factor[6];
float gEngine_powerup_factor[6] = { 1.3, 1.9, 2.5, 3.2, 4.0, 10.0 };
br_angle gPanning_camera_angle;
br_scalar gPanning_camera_height;
int gFace_count;
float gCar_simplification_factor[2][5] = {
    { 10.0, 3.0, 1.5, 0.75, 0.0 },
    { 10.0, 5.0, 2.5, 1.5, 0.0 }
};
int gCar_simplification_level;
int gNum_active_non_cars;
int gCamera_has_collided;
tFace_ref gFace_list__car[150]; // suffix added to avoid duplicate symbol
tNon_car_spec* gActive_non_car_list[50];
int gOver_shoot;
br_scalar gMin_world_y;
br_scalar gAccel;
br_vector3 gAverage_grid_position;
br_actor* gPed_actor;
int gCollision_count;
int gCamera_frozen;
int gMaterial_index;
int gInTheSea;
int gCamera_mode;
br_scalar gOur_yaw__car; // suffix added to avoid duplicate symbol
br_scalar gGravity__car; // suffix added to avoid duplicate symbol
br_vector3 gNew_ground_normal__car; // suffix added to avoid duplicate symbol
char gNon_car_spec_list[100];
tU32 gMechanics_time_sync;
int gNum_cars_and_non_cars;

// IDA: void __usercall DamageUnit(tCar_spec *pCar@<EAX>, int pUnit_type@<EDX>, int pDamage_amount@<EBX>)
void DamageUnit(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {
    tDamage_unit* the_damage;
    LOG_TRACE("(%p, %d, %d)", pCar, pUnit_type, pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageUnitWithSmoke(tCar_spec *pCar@<EAX>, int pUnit_type@<EDX>, int pDamage_amount@<EBX>)
void DamageUnitWithSmoke(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {
    LOG_TRACE("(%p, %d, %d)", pCar, pUnit_type, pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageEngine(int pDamage_amount@<EAX>)
void DamageEngine(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageTrans(int pDamage_amount@<EAX>)
void DamageTrans(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageSteering(int pDamage_amount@<EAX>)
void DamageSteering(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageLFWheel(int pDamage_amount@<EAX>)
void DamageLFWheel(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageLFBrake(int pDamage_amount@<EAX>)
void DamageLFBrake(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageLRBrake(int pDamage_amount@<EAX>)
void DamageLRBrake(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageLRWheel(int pDamage_amount@<EAX>)
void DamageLRWheel(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageRFWheel(int pDamage_amount@<EAX>)
void DamageRFWheel(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageRFBrake(int pDamage_amount@<EAX>)
void DamageRFBrake(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageRRBrake(int pDamage_amount@<EAX>)
void DamageRRBrake(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageRRWheel(int pDamage_amount@<EAX>)
void DamageRRWheel(int pDamage_amount) {
    LOG_TRACE("(%d)", pDamage_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CalculatePlaneNormal(br_vector3 *pP1@<EAX>, br_vector3 *pP2@<EDX>, br_vector3 *pP3@<EBX>, br_vector3 *pNormal@<ECX>)
void CalculatePlaneNormal(br_vector3* pP1, br_vector3* pP2, br_vector3* pP3, br_vector3* pNormal) {
    br_vector3 p0;
    br_vector3 p1;
    br_vector3 cross_product;
    br_vector3 temp_vector;
    LOG_TRACE("(%p, %p, %p, %p)", pP1, pP2, pP3, pNormal);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CalculateGroundNormal(br_model *pThe_model@<EAX>, int pFace_index@<EDX>)
void CalculateGroundNormal(br_model* pThe_model, int pFace_index) {
    LOG_TRACE("(%p, %d)", pThe_model, pFace_index);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ChangeYCoordinate(br_scalar pNew_y, tU32 pTime_taken, br_model *pThe_model, int pFace_index)
void ChangeYCoordinate(br_scalar pNew_y, tU32 pTime_taken, br_model* pThe_model, int pFace_index) {
    br_scalar y_change;
    br_transform new_transform;
    br_vector3 side_window;
    LOG_TRACE("(%f, %d, %p, %d)", pNew_y, pTime_taken, pThe_model, pFace_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SwitchCarActor(tCar_spec *pCar_spec@<EAX>, int pModel_index@<EDX>)
void SwitchCarActor(tCar_spec* pCar_spec, int pModel_index) {
    int i;
    LOG_TRACE("(%p, %d)", pCar_spec, pModel_index);

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
    LOG_TRACE("(%p, %d)", pCar, pClear_disabled_flag);

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
    if (!strcmp(pCar->name, "STELLA.TXT")) {
        pCar->proxy_ray_distance = 6.0;
    } else {
        pCar->proxy_ray_distance = 0.0;
    }
    pCar->last_special_volume = 0;
    pCar->auto_special_volume = 0;
    pCar->num_smoke_columns = 0;
    pCar->who_last_hit_me = 0;
    pCar->screen_material_source = 0;
    if (pCar->screen_material) {
        pCar->screen_material->colour_map = 0;
        pCar->screen_material->index_shade = gRender_shade_table;
        BrMaterialUpdate(pCar->screen_material, 0x7FFFu);
    }
    if (pCar->driver == eDriver_local_human) {
        ResetRecoveryVouchers();
    }
    BrVector3SetFloat(&pCar->v, 0.0, 0.0, 0.0);
    BrVector3SetFloat(&pCar->omega, 0.0, 0.0, 0.0);
    pCar->curvature = 0.0;
    BrMatrix34Copy(&safe_position, &car_actor->t.t.mat);
    if (safe_position.m[3][0] > 500.0) {
        safe_position.m[3][0] -= 1000.0f;
        safe_position.m[3][1] -= 1000.0f;
        safe_position.m[3][2] -= 1000.0f;
    }
    BrMatrix34Copy(&pCar->old_frame_mat, &safe_position);
    BrMatrix34Copy(&pCar->oldmat, &safe_position);
    pCar->oldmat.m[3][0] *= 6.9000001;
    pCar->oldmat.m[3][1] *= 6.9000001;
    pCar->oldmat.m[3][2] *= 6.9000001;
    BrMatrix34ApplyP(&pCar->pos, &pCar->cmpos, &pCar->oldmat);
    pCar->pos.v[0] /= 6.9000001;
    pCar->pos.v[1] /= 6.9000001;
    pCar->pos.v[2] /= 6.9000001;
    for (j = 0; j < COUNT_OF(pCar->oldd); j++) {
        pCar->oldd[j] = pCar->ride_height;
    }
    pCar->gear = 0;
    pCar->revs = 0.0;
    pCar->traction_control = 1;
    pCar->direction.v[0] = -car_actor->t.t.mat.m[2][0];
    pCar->direction.v[1] = -car_actor->t.t.mat.m[2][1];
    pCar->direction.v[2] = -car_actor->t.t.mat.m[2][2];
    for (j = 0; j < COUNT_OF(pCar->last_safe_positions); j++) {
        BrMatrix34Copy(&pCar->last_safe_positions[j], &safe_position);
    }
    pCar->message.type = 0;
    pCar->message.time = 0;
    pCar->dt = -1.0;
    pCar->last_car_car_collision = 1;
    pCar->time_to_recover = 0;
    pCar->repair_time = 0;

    switch (pCar->driver) {

    case eDriver_oppo:
        index = 0;
        for (j = 0; gCurrent_race.number_of_racers > j; ++j) {
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
        for (j = 0; gCurrent_race.number_of_racers > j; ++j) {
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

    default:
        LOG_WARN("Case %d not handled", pCar->driver);
        break;
    }
    PossibleService();
    pCar->box_face_ref = gFace_num__car - 2;
    pCar->doing_nothing_flag = 0;
    pCar->end_steering_damage_effect = 0;
    pCar->end_trans_damage_effect = 0;
    pCar->wheel_dam_offset[0] = 0.0;
    pCar->wheel_dam_offset[1] = 0.0;
    pCar->wheel_dam_offset[2] = 0.0;
    pCar->wheel_dam_offset[3] = 0.0;
    pCar->shadow_intersection_flags = 0;
    pCar->underwater_ability = 0;
    pCar->invulnerable = 0;
    pCar->wall_climber_mode = 0;
    pCar->grip_multiplier = 1.0;
    pCar->damage_multiplier = 1.0;
    pCar->collision_mass_multiplier = 1.0;
    pCar->engine_power_multiplier = 1.0;
    pCar->bounce_rate = 0.0;
    pCar->bounce_amount = 0.0;
    pCar->knackered = 0;
    TotallyRepairACar(pCar);
    SetCarSuspGiveAndHeight(pCar, 1.0, 1.0, 1.0, 0.0, 0.0);
    for (j = 0; j < 64; ++j) {
        pCar->powerups[j] = 0;
    }
    if (gNet_mode) {
        for (j = 0; j < 3; ++j) {
            pCar->power_up_levels[j] = 0;
        }
    }
}

// IDA: void __usercall InitialiseCar(tCar_spec *pCar@<EAX>)
void InitialiseCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    InitialiseCar2(pCar, 1);
}

// IDA: void __usercall InitialiseCarsEtc(tRace_info *pThe_race@<EAX>)
void InitialiseCarsEtc(tRace_info* pThe_race) {
    int i;
    int cat;
    int car_count;
    tCar_spec* car;
    br_bounds bnds;
    LOG_TRACE("(%p)", pThe_race);

    gProgram_state.initial_position = pThe_race->initial_position;
    gProgram_state.initial_yaw = pThe_race->initial_yaw;
    BrActorToBounds(&bnds, gProgram_state.track_spec.the_actor);
    gMin_world_y = bnds.min.v[1];
    gNum_active_non_cars = 0;
    for (cat = eVehicle_self; cat <= eVehicle_not_really; ++cat) {
        if (cat) {
            car_count = GetCarCount(cat);
        } else {
            car_count = 1;
        }
        for (i = 0; car_count > i; i++) {
            PossibleService();
            if (cat) {
                car = GetCarSpec(cat, i);
            } else {
                car = &gProgram_state.current_car;
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
void GetAverageGridPosition(tRace_info* pThe_race) {
    int i;
    br_scalar total_cars;
    tCar_spec* car;
    LOG_TRACE("(%p)", pThe_race);

    total_cars = 0.0;
    BrVector3SetFloat(&gAverage_grid_position, 0.0, 0.0, 0.0);
    for (i = 0; pThe_race->number_of_racers > i; i++) {
        car = pThe_race->opponent_list[i].car_spec;
        gAverage_grid_position.v[0] = car->pos.v[0] + gAverage_grid_position.v[0];
        gAverage_grid_position.v[1] = car->pos.v[1] + gAverage_grid_position.v[1];
        gAverage_grid_position.v[2] = car->pos.v[2] + gAverage_grid_position.v[2];
        total_cars = total_cars + 1.0;
    }
    gAverage_grid_position.v[0] = gAverage_grid_position.v[0] / total_cars;
    gAverage_grid_position.v[1] = gAverage_grid_position.v[1] / total_cars;
    gAverage_grid_position.v[2] = gAverage_grid_position.v[2] / total_cars;
}

// IDA: void __usercall SetInitialPosition(tRace_info *pThe_race@<EAX>, int pCar_index@<EDX>, int pGrid_index@<EBX>)
void SetInitialPosition(tRace_info* pThe_race, int pCar_index, int pGrid_index) {
    int place_on_grid;
    int i;
    int start_i;
    int j;
    br_actor* car_actor;
    br_angle initial_yaw = 0;
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
    LOG_TRACE("(%p, %d, %d)", pThe_race, pCar_index, pGrid_index);

    car_actor = pThe_race->opponent_list[pCar_index].car_spec->car_master_actor;
    car = pThe_race->opponent_list[pCar_index].car_spec;
    BrMatrix34Identity(&car_actor->t.t.mat);
    place_on_grid = 1;
    if (gNet_mode && !gCurrent_net_game->options.grid_start && pThe_race->number_of_net_start_points) {
        TELL_ME_IF_WE_PASS_THIS_WAY();
        // start_i = IRandomBetween(0, pThe_race->number_of_net_start_points - 1);
        // i = start_i;
        // while (1) {
        //     PossibleService();
        //     for (j = 0; gNumber_of_net_players > j; ++j) {
        //         if (j != pCar_index) {
        //             v19 = pThe_race->opponent_list[j].car_spec->car_master_actor->t.t.translate.t.v[0];
        //             v20 = pThe_race->opponent_list[j].car_spec->car_master_actor->t.t.translate.t.v[1];
        //             v21 = pThe_race->opponent_list[j].car_spec->car_master_actor->t.t.translate.t.v[2];
        //             if (v19 > 500.0) {
        //                 v19 = v19 - 1000.0f;
        //                 v20 = v20 - 1000.0f;
        //                 v21 = v21 - 1000.0f;
        //             }
        //             dist.v[0] = v19 - pThe_race->net_starts[start_i].pos.v[0];
        //             dist.v[1] = v20 - pThe_race->net_starts[start_i].pos.v[1];
        //             dist.v[2] = v21 - pThe_race->net_starts[start_i].pos.v[2];
        //             if (dist.v[1] * dist.v[1] + dist.v[2] * dist.v[2] + dist.v[0] * dist.v[0] < 16.0) {
        //                 break;
        //             }
        //         }
        //     }
        //     if (gNumber_of_net_players == j) {
        //         break;
        //     }
        //     if (pThe_race->number_of_net_start_points == ++start_i) {
        //         start_i = 0;
        //     }
        //     if (i == start_i) {
        //         goto LABEL_17;
        //     }
        // }
        // car_actor->t.t.translate.t.v[0] = pThe_race->net_starts[start_i].pos.v[0];
        // car_actor->t.t.translate.t.v[1] = pThe_race->net_starts[start_i].pos.v[1];
        // car_actor->t.t.translate.t.v[2] = pThe_race->net_starts[start_i].pos.v[2];
        // initial_yaw[0] = (__int64)(pThe_race->net_starts[start_i].yaw * 182.0444444444445);
        // place_on_grid = 0;
    }
    if (place_on_grid) {
        initial_yaw = (pThe_race->initial_yaw * 182.0444444444445);
        BrMatrix34RotateY(&initial_yaw_matrix, initial_yaw);
        grid_offset.v[0] = 0.0 - pGrid_index % 2;
        grid_offset.v[1] = 0.0;
        grid_offset.v[2] = (double)(pGrid_index / 2) * 2.0 + (double)(pGrid_index % 2) * 0.40000001;
        LOG_DEBUG("grid offset: %f, %f, %f", grid_offset.v[0], grid_offset.v[1], grid_offset.v[2]);
        BrMatrix34ApplyV(&car_actor->t.t.translate.t, &grid_offset, &initial_yaw_matrix);
        car_actor->t.t.translate.t.v[0] += pThe_race->initial_position.v[0];
        car_actor->t.t.translate.t.v[1] += pThe_race->initial_position.v[1];
        car_actor->t.t.translate.t.v[2] += pThe_race->initial_position.v[2];
        LOG_DEBUG("yaw: %f, pos: %f, %f, %f", pThe_race->initial_yaw, pThe_race->initial_position.v[0], pThe_race->initial_position.v[1], pThe_race->initial_position.v[2]);
    }
    LOG_DEBUG("grid pos: %d, pos: x=%f, z=%f", pGrid_index, car_actor->t.t.translate.t.v[0], car_actor->t.t.translate.t.v[2]);
    FindBestY(
        &car_actor->t.t.translate.t,
        gTrack_actor,
        10.0,
        &nearest_y_above,
        &nearest_y_below,
        &above_model,
        &below_model,
        &above_face_index,
        &below_face_index);
    if (nearest_y_above == 30000.0) {
        if (nearest_y_below == -30000.0) {
            LOG_DEBUG("found pos 1: %f", 0);
            car_actor->t.t.translate.t.v[1] = 0.0;
        } else {
            LOG_DEBUG("found pos 2: %f", nearest_y_below);
            car_actor->t.t.translate.t.v[1] = nearest_y_below;
        }
    } else {
        // 20.345
        LOG_DEBUG("found pos 3: %f, x: %f, z: %f", nearest_y_above, car_actor->t.t.translate.t.v[0], car_actor->t.t.translate.t.v[2]);
        car_actor->t.t.translate.t.v[1] = nearest_y_above;
    }
    BrMatrix34PreRotateY(&car_actor->t.t.mat, initial_yaw);
    if (gNet_mode) {
        BrMatrix34Copy(
            &gNet_players[pThe_race->opponent_list[pCar_index].net_player_index].initial_position,
            &car->car_master_actor->t.t.mat);
    }
    if (gNet_mode && car->disabled && car_actor->t.t.translate.t.v[0] < 500.0) {
        DisableCar(car);
    }
}

// IDA: void __usercall SetInitialPositions(tRace_info *pThe_race@<EAX>)
void SetInitialPositions(tRace_info* pThe_race) {
    int i;
    LOG_TRACE("(%p)", pThe_race);

    for (i = 0; i < pThe_race->number_of_racers; i++) {
        SetInitialPosition(pThe_race, i, i);
    }
}

// IDA: void __usercall InitialiseNonCar(tNon_car_spec *non_car@<EAX>)
void InitialiseNonCar(tNon_car_spec* non_car) {
    tCollision_info* c;
    LOG_TRACE("(%p)", non_car);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetFacesInBox(tCollision_info *c@<EAX>)
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
    LOG_TRACE("(%p)", c);

    BrMatrix34Copy(&mat, &c->car_master_actor->t.t.mat);
    BrMatrix34Copy(&mat2, &c->oldmat);
    mat.m[3][0] = mat.m[3][0] / 6.9;
    mat.m[3][1] = mat.m[3][1] / 6.9;
    mat.m[3][2] = mat.m[3][2] / 6.9;
    mat2.m[3][0] = mat2.m[3][0] / 6.9;
    mat2.m[3][1] = mat2.m[3][1] / 6.9;
    mat2.m[3][2] = mat2.m[3][2] / 6.9;
    BrMatrix34LPInverse(&mat3, &mat);
    BrMatrix34Mul(&mat4, &mat2, &mat3);
    GetNewBoundingBox(&bnds.original_bounds, c->bounds, &mat4);
    for (i = 0; i < 3; ++i) {
        if (bnds.original_bounds.min.v[i] > c->bounds[0].min.v[i]) {
            bnds.original_bounds.min.v[i] = c->bounds[0].min.v[i];
        }
        if (bnds.original_bounds.max.v[i] < c->bounds[0].max.v[i]) {
            bnds.original_bounds.max.v[i] = c->bounds[0].max.v[i];
        }
        bnds.original_bounds.min.v[i] = bnds.original_bounds.min.v[i] - 0.0020000001;
        bnds.original_bounds.max.v[i] = bnds.original_bounds.max.v[i] + 0.0020000001;
    }
    GetNewBoundingBox(&c->bounds_world_space, &bnds.original_bounds, &mat);
    c->bounds_ws_type = eBounds_ws;
    if ((c->box_face_ref != gFace_num__car && (c->box_face_ref != gFace_num__car - 1 || c->box_face_start <= gFace_count))
        || (BrMatrix34Mul(&mat5, &mat, &c->last_box_inv_mat),
            GetNewBoundingBox(&new_in_old, &bnds.original_bounds, &mat5),
            c->last_box.max.v[0] <= new_in_old.max.v[0])
        || c->last_box.max.v[1] <= new_in_old.max.v[1]
        || c->last_box.max.v[2] <= new_in_old.max.v[2]
        || c->last_box.min.v[0] >= new_in_old.min.v[0]
        || c->last_box.min.v[1] >= new_in_old.min.v[1]
        || c->last_box.min.v[2] >= new_in_old.min.v[2]) {
        BrMatrix34Mul(&mat5, &mat4, &mat4);
        BrMatrix34Mul(&mat6, &mat5, &mat4);
        BrMatrix34LPInverse(&mat5, &mat6);
        GetNewBoundingBox(&predicted_bounds, c->bounds, &mat5);
        for (i = 0; i < 3; ++i) {
            if (bnds.original_bounds.min.v[i] > predicted_bounds.min.v[i]) {
                bnds.original_bounds.min.v[i] = predicted_bounds.min.v[i];
            }
            if (bnds.original_bounds.max.v[i] < predicted_bounds.max.v[i]) {
                bnds.original_bounds.max.v[i] = predicted_bounds.max.v[i];
            }
            bnds.original_bounds.min.v[i] = bnds.original_bounds.min.v[i] - 0.02;
            bnds.original_bounds.max.v[i] = bnds.original_bounds.max.v[i] + 0.02;
        }
        c->last_box = bnds.original_bounds;
        BrMatrix34Copy(&c->last_box_inv_mat, &mat3);
        bnds.mat = &mat;
        c->box_face_start = gFace_count;
        gPling_face = NULL;
        gFace_count += FindFacesInBox(&bnds, &gFace_list__car[gFace_count], 150 - gFace_count);
        if (gFace_count >= 150) {
            c->box_face_start = 0;
            gFace_count = FindFacesInBox(&bnds, gFace_list__car, 150);
            ++gFace_num__car;
        }
        old_d = c->water_d;
        if (c->driver == eDriver_local_human
            && c->water_d != 10000.0
            && gDouble_pling_water
            && c->bounds_world_space.max.v[2] * c->water_normal.v[2] + c->bounds_world_space.max.v[1] * c->water_normal.v[1] + c->water_normal.v[0] * c->bounds_world_space.max.v[0] - c->water_d <= 0.0) {
            gInTheSea = 1;
            FreezeCamera();
        }
        if (gPling_face && fabs(gPling_face->normal.v[1]) > 0.89999998) {
            c->water_normal.v[0] = gPling_face->normal.v[0];
            c->water_normal.v[1] = gPling_face->normal.v[1];
            c->water_normal.v[2] = gPling_face->normal.v[2];
            if (c->water_normal.v[1] < 0.0) {
                c->water_normal.v[0] = -c->water_normal.v[0];
                c->water_normal.v[1] = -c->water_normal.v[1];
                c->water_normal.v[2] = -c->water_normal.v[2];
            }
            c->water_d = gPling_face->v[0].v[1] * c->water_normal.v[1] + gPling_face->v[0].v[2] * c->water_normal.v[2] + gPling_face->v[0].v[0] * c->water_normal.v[0];
            if (c->driver == eDriver_local_human) {
                if (gPling_face->material->identifier[1] == '!') {
                    if (c->bounds_world_space.min.v[1] * c->water_normal.v[1] + c->bounds_world_space.min.v[2] * c->water_normal.v[2] + c->water_normal.v[0] * c->bounds_world_space.min.v[0] - c->water_d < 0.0) {
                        GetNewBoundingBox(&current_bounds, &c->bounds[1], &c->car_master_actor->t.t.mat);
                        if ((c->water_normal.v[1] * current_bounds.min.v[1] + c->water_normal.v[2] * current_bounds.min.v[2] + c->water_normal.v[0] * current_bounds.min.v[0]) / 6.9 - c->water_d < 0.0) {
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
}

// IDA: int __cdecl IsCarInTheSea()
int IsCarInTheSea() {
    LOG_TRACE("()");

    return gInTheSea;
}

// IDA: void __usercall RememberSafePosition(tCar_spec *car@<EAX>, tU32 pTime@<EDX>)
void RememberSafePosition(tCar_spec* car, tU32 pTime) {
    static tU32 time_count;
    int j;
    br_vector3 r;
    br_scalar ts;
    LOG_TRACE("(%p, %d)", car, pTime);

    STUB();
}

// IDA: void __usercall ControlOurCar(tU32 pTime_difference@<EAX>)
void ControlOurCar(tU32 pTime_difference) {
    br_scalar ts;
    br_vector3 minus_k;
    tCar_spec* car;
    static int steering_locked;
    int i;
    tU32 time;
    LOG_TRACE("(%d)", pTime_difference);

    car = &gProgram_state.current_car;
    if (gCar_flying) {
        if (gNet_mode) {
            gCar_flying = 0;
        } else {
            car->car_master_actor->t.t.mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * 6.9000001;
            car->car_master_actor->t.t.mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * 6.9000001;
            car->car_master_actor->t.t.mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * 6.9000001;
            ts = (double)pTime_difference / 1000.0;
            FlyCar(car, ts);
            car->car_master_actor->t.t.mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] / 6.9000001;
            car->car_master_actor->t.t.mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] / 6.9000001;
            car->car_master_actor->t.t.mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] / 6.9000001;
        }
        return;
    }
    time = GetTotalTime();
    if (car->damage_units[eDamage_steering].damage_level > 40) {
        LOG_PANIC("this is not expected");
        if (car->end_steering_damage_effect) {
            if (car->end_steering_damage_effect > time || car->damage_units[eDamage_steering].damage_level == 99) {
                car->keys.left = car->false_key_left;
                car->keys.right = car->false_key_right;
            } else {
                car->end_steering_damage_effect = 0;
            }
        } else {
            ts = pTime_difference * (car->damage_units[eDamage_steering].damage_level - 40) * 0.0045;
            if (PercentageChance(ts) && fabs(car->velocity_car_space.v[2]) > 0.0001) {
                if (car->keys.left || car->keys.right) {
                    car->false_key_left = !car->keys.left;
                    car->false_key_right = !car->keys.right;
                } else if (PercentageChance(50)) {
                    car->false_key_left = 1;
                } else {
                    car->false_key_right = 1;
                }
                ts = 5 * (5 * car->damage_units[eDamage_steering].damage_level - 200);
                car->end_steering_damage_effect = FRandomBetween(0.0, ts) + time;
            }
        }
    }
    if (car->damage_units[eDamage_transmission].damage_level > 40) {
        LOG_PANIC("this is not expected");
        if (car->end_trans_damage_effect) {
            if (car->end_trans_damage_effect > time || car->damage_units[eDamage_transmission].damage_level == 99) {
                car->gear = 0;
                car->just_changed_gear = 1;
            } else {
                car->end_trans_damage_effect = 0;
            }
        } else {
            ts = pTime_difference * (car->damage_units[eDamage_transmission].damage_level - 40);
            if (PercentageChance(ts) * 0.006 != 0) {
                ts = 10 * (5 * car->damage_units[eDamage_transmission].damage_level - 200);
                car->end_trans_damage_effect = FRandomBetween(0.0, ts) + time;
            }
        }
    }
    ts = (double)pTime_difference / 1000.0;
    ControlCar[gControl__car](car, ts);
    RememberSafePosition(car, pTime_difference);
    if (gCamera_reset) {
        BrVector3SetFloat(&minus_k, 0.0, 0.0, -1.0);
        gCamera_sign = 0;
        BrMatrix34ApplyV(&car->direction, &minus_k, &car->car_master_actor->t.t.mat);
    }
}

// IDA: void __usercall CalcEngineForce(tCar_spec *c@<EAX>, br_scalar dt)
void CalcEngineForce(tCar_spec* c, br_scalar dt) {
    br_scalar torque;
    br_scalar ts;
    br_scalar ts2;
    br_scalar brake_temp;
    int sign;
    tS32 temp_for_swap;
    LOG_TRACE("(%p, %f)", c, dt);

    c->acc_force = 0.0;
    if (c->revs == 0.0) {
        //LOG_DEBUG("changedown1");
        c->gear = 0;
    }
    sign = c->gear < 0 || (!c->gear && c->velocity_car_space.v[2] > 0.5);
    if (c->keys.backwards != sign) {
        c->keys.backwards = !c->keys.backwards;
        //LOG_PANIC("not expected");

        temp_for_swap = c->keys.acc;
        c->keys.acc = c->keys.dec;
        c->keys.dec = temp_for_swap;

        temp_for_swap = c->joystick.acc;
        c->joystick.acc = c->joystick.dec;
        c->joystick.dec = temp_for_swap;
    }
    if (!c->gear && !c->keys.acc && c->joystick.acc <= 0 && (c->keys.dec || c->joystick.dec > 0) && !c->keys.backwards && fabs(c->velocity_car_space.v[2]) < 1.0) {
        c->keys.backwards = 1;
        c->keys.acc = c->keys.dec;
        c->keys.dec = 0;
        temp_for_swap = c->joystick.acc;
        c->joystick.acc = c->joystick.dec;
        c->joystick.dec = temp_for_swap;
    }
    c->torque = -(c->revs * c->revs / 100000000.0) - 0.2;
    if (c->keys.acc || c->joystick.acc >= 0) {
        if (fabs(c->curvature) > c->maxcurve / 2.0 && c->gear < 2 && c->gear && c->traction_control) {
            ts = 0.69999999;
        } else if (c->joystick.acc < 0) {
            ts = 1.2;
        } else {
            ts = c->joystick.acc / 54613.0;
            LOG_PANIC("ooo");
        }

        torque = c->engine_power_multiplier * ts * gEngine_powerup_factor[c->power_up_levels[1]];
        if (c->damage_units[0].damage_level > 10) {
            torque = (1.0 - (double)(c->damage_units[0].damage_level - 10) / 100.0) * torque;
        }
        c->torque = c->torque + torque;
    } else {
        c->traction_control = 1;
    }
    if (!c->keys.dec && (!c->keys.acc || c->gear) && c->joystick.dec <= 0 && (c->joystick.acc <= 0 || c->gear)) {
        c->brake_force = 0.0;
    } else {
        if (c->joystick.dec > 0) {
            c->brake_force = (double)(c->joystick.dec / 0x10000) * c->brake_increase + c->initial_brake;
        }
        if (c->brake_force == 0.0) {
            c->brake_force = c->initial_brake;
        } else {
            c->brake_force = c->brake_increase * dt + c->brake_force;
            if (c->initial_brake + c->brake_increase < c->brake_force) {
                c->brake_force = c->initial_brake + c->brake_increase;
            }
        }
    }
    if (c->gear) {
        c->acc_force = c->force_torque_ratio * c->torque / (double)c->gear;
        //printf("force_torque_ratio %f, torque %f, dt %f, revs %f, target %f\n", c->force_torque_ratio, c->torque, dt, c->revs, c->target_revs);
        if (c->brake_force == 0.0) {
            if (c->revs - 1.0 > c->target_revs || c->revs + 1.0 < c->target_revs) {
                ts2 = c->torque * dt / 0.0002 + c->revs - c->target_revs;
                //LOG_DEBUG("revs_diff %f", c->revs - c->target_revs);
                c->acc_force = ts2 / ((1.0 / (c->speed_revs_ratio * c->M) / (double)c->gear + 1.0 / (c->force_torque_ratio * 0.0002) * (double)c->gear) * dt) + c->acc_force;
                //LOG_DEBUG("updating force from %f to %f, delta %f", f2, c->acc_force, ts2 / ((1.0 / (c->speed_revs_ratio * c->M) / (double)c->gear + 1.0 / (c->force_torque_ratio * 0.0002) * (double)c->gear) * dt));
            }
        } else {
            c->revs = c->target_revs;
        }
    }
    //LOG_DEBUG("force 1 %f, 2 %f, 3 %f, %f", f1, f2, f3, c->target_revs);
}

// IDA: void __usercall PrepareCars(tU32 pFrame_start_time@<EAX>)
void PrepareCars(tU32 pFrame_start_time) {
    tCar_spec* car;
    int i;
    static tU32 last_frame_start;
    LOG_TRACE("(%d)", pFrame_start_time);

    last_frame_start = pFrame_start_time;
    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        car = gActive_car_list[i];
        car->car_master_actor->t.t.mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * 6.9000001;
        car->car_master_actor->t.t.mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * 6.9000001;
        car->car_master_actor->t.t.mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * 6.9000001;
        car->velocity_car_space.v[0] = car->velocity_car_space.v[0] * 6900.0;
        car->velocity_car_space.v[1] = car->velocity_car_space.v[1] * 6900.0;
        car->velocity_car_space.v[2] = car->velocity_car_space.v[2] * 6900.0;
        car->frame_collision_flag = gCar_car_collisions && car->collision_flag;
        if (car->driver > eDriver_non_car) {
            RecordLastDamage(car);
            if (car->driver == eDriver_oppo && gStop_opponents_moving) {
                car->acc_force = 0.0;
                car->brake_force = 0.0;
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
                car->keys.horn = 0;
                car->joystick.acc = -1;
                car->joystick.dec = -1;
            }
            SetSmokeLastDamageLevel(car);
        }
        BrMatrix34Copy(&car->car_master_actor->t.t.mat, &car->oldmat);
    }
}

// IDA: void __usercall FinishCars(tU32 pLast_frame_time@<EAX>, tU32 pTime@<EDX>)
void FinishCars(tU32 pLast_frame_time, tU32 pTime) {
    tCar_spec* car;
    br_vector3 minus_k;
    int i;
    int wheel;
    br_scalar scale;
    LOG_TRACE("(%d, %d)", pLast_frame_time, pTime);

    for (i = 0; gNum_cars_and_non_cars > i; ++i) {
        car = gActive_car_list[i];
        if (fabs(car->omega.v[0]) > 10000.0
            || fabs(car->omega.v[1]) > 10000.0
            || fabs(car->omega.v[2]) > 10000.0) {
            BrVector3SetFloat(&car->omega, 0.0, 0.0, 0.0);
            BrVector3SetFloat(&car->v, 0.0, 0.0, 0.0);
        }
        car->velocity_car_space.v[0] = car->velocity_car_space.v[0] / 6900.0;
        car->velocity_car_space.v[1] = car->velocity_car_space.v[1] / 6900.0;
        car->velocity_car_space.v[2] = car->velocity_car_space.v[2] / 6900.0;
        car->car_master_actor->t.t.mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] / 6.9000001;
        car->car_master_actor->t.t.mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] / 6.9000001;
        car->car_master_actor->t.t.mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] / 6.9000001;

        car->speed = sqrt(car->v.v[2] * car->v.v[2] + car->v.v[0] * car->v.v[0]) / 6900.0;
        minus_k.v[0] = -car->car_master_actor->t.t.mat.m[2][0];
        minus_k.v[1] = -car->car_master_actor->t.t.mat.m[2][1];
        minus_k.v[2] = -car->car_master_actor->t.t.mat.m[2][2];
        if (car->speed <= 0.000099999997) {
            if (car->direction.v[1] * minus_k.v[1] + car->direction.v[2] * minus_k.v[2] + car->direction.v[0] * minus_k.v[0] < 0.0) {
                BrVector3SetFloat(&minus_k, 0.0, 0.0, 1.0);
            } else {
                BrVector3SetFloat(&minus_k, 0.0, 0.0, -1.0);
            }
            BrMatrix34ApplyV(&car->direction, &minus_k, &car->car_master_actor->t.t.mat);
        } else if (gLast_mechanics_time > pLast_frame_time && gCar_to_view == car) {
            car->old_v.v[0] = car->old_v.v[0] - car->v.v[0];
            car->old_v.v[1] = car->old_v.v[1] - car->v.v[1];
            car->old_v.v[2] = car->old_v.v[2] - car->v.v[2];
            car->old_v.v[0] = (gLast_mechanics_time - pLast_frame_time) / (float)PHYSICS_STEP_TIME * car->old_v.v[0];
            car->old_v.v[1] = (gLast_mechanics_time - pLast_frame_time) / (float)PHYSICS_STEP_TIME * car->old_v.v[1];
            car->old_v.v[2] = (gLast_mechanics_time - pLast_frame_time) / (float)PHYSICS_STEP_TIME * car->old_v.v[2];
            car->old_v.v[0] = car->old_v.v[0] + car->v.v[0];
            car->old_v.v[1] = car->old_v.v[1] + car->v.v[1];
            car->old_v.v[2] = car->old_v.v[2] + car->v.v[2];
            scale = sqrt(
                car->old_v.v[1] * car->old_v.v[1]
                + car->old_v.v[2] * car->old_v.v[2]
                + car->old_v.v[0] * car->old_v.v[0]);
            if (scale <= 2.3841858e-7) {
                car->direction.v[0] = 1.0;
                car->direction.v[1] = 0.0;
                car->direction.v[2] = 0.0;
            } else {
                scale = 1.0 / scale;
                car->direction.v[0] = car->old_v.v[0] * scale;
                car->direction.v[1] = car->old_v.v[1] * scale;
                car->direction.v[2] = car->old_v.v[2] * scale;
            }
        } else {
            scale = sqrt(car->v.v[2] * car->v.v[2] + car->v.v[1] * car->v.v[1] + car->v.v[0] * car->v.v[0]);
            if (scale <= 2.3841858e-7) {
                car->direction.v[0] = 1.0;
                car->direction.v[1] = 0.0;
                car->direction.v[2] = 0.0;
            } else {
                scale = 1.0 / scale;
                car->direction.v[0] = car->v.v[0] * scale;
                car->direction.v[1] = car->v.v[1] * scale;
                car->direction.v[2] = car->v.v[2] * scale;
            }
        }
        if (car->driver > eDriver_non_car) {
            car->speedo_speed = (car->v.v[2] * minus_k.v[2] + car->v.v[1] * minus_k.v[1] + car->v.v[0] * minus_k.v[0]) / 6900.0;
            car->steering_angle = atan((car->wpos[0].v[2] - car->wpos[2].v[2]) * car->curvature) * 57.29577951308232;
            car->lr_sus_position = (car->ride_height - car->oldd[0]) / 6.9;
            car->rr_sus_position = (car->ride_height - car->oldd[1]) / 6.9;
            car->lf_sus_position = (car->ride_height - car->oldd[2]) / 6.9;
            car->rf_sus_position = (car->ride_height - car->oldd[3]) / 6.9;
            for (wheel = 0; wheel < 4; ++wheel) {
                if (*(float*)((char*)car->susp_height + 2 * (wheel & 0xFFFFFFFE)) > (double)car->oldd[wheel]
                    && gCurrent_race.material_modifiers[car->material_index[wheel]].smoke_type >= 2
                    && !car->doing_nothing_flag) {
                    GenerateContinuousSmoke(car, wheel, pTime);
                }
            }
        }
    }
}

// IDA: void __usercall InterpolateCars(tU32 pLast_frame_time@<EAX>, tU32 pTime@<EDX>)
void InterpolateCars(tU32 pLast_frame_time, tU32 pTime) {
    br_scalar dt;
    tCar_spec* car;
    int i;
    LOG_TRACE("(%d, %d)", pLast_frame_time, pTime);

    dt = (gLast_mechanics_time - pLast_frame_time) / 1000.0;
    if (dt > 0.04 || dt < 0.0) {
        dt = 0.0;
    }
    gCar_car_collisions = dt > 0.0;
    for (i = 0; i < gNum_cars_and_non_cars; ++i) {
        car = gActive_car_list[i];
        BrMatrix34Copy(&car->oldmat, &car->car_master_actor->t.t.mat);
        SimpleRotate((tCollision_info*)car, -dt);
        TranslateCar((tCollision_info*)car, -dt);
        BrMatrix34ApplyP(&car->pos, &car->cmpos, &car->car_master_actor->t.t.mat);
        car->pos.v[0] = car->pos.v[0] / 6.9000001;
        car->pos.v[1] = car->pos.v[1] / 6.9000001;
        car->pos.v[2] = car->pos.v[2] / 6.9000001;
    }
}

// IDA: void __cdecl ResetOldmat()
void ResetOldmat() {
    tCar_spec* car;
    int i;
    br_matrix34 mat;
    static int normalise_count = 0;
    LOG_TRACE("()");

    for (i = 0; i < gNum_cars_and_non_cars; i++) {
        BrMatrix34Copy(&gActive_car_list[i]->oldmat, &gActive_car_list[i]->car_master_actor->t.t.mat);
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
void GetNonCars() {
    int i;
    int j;
    LOG_TRACE("()");

    gNum_cars_and_non_cars = gNum_active_non_cars + gNum_active_cars;
    j = 0;
    for (i = gNum_active_cars; i < gNum_cars_and_non_cars; i++) {
        gActive_car_list[i] = (tCar_spec*)gActive_non_car_list[j];
        j++;
    }
}

// IDA: void __usercall GetNetPos(tCar_spec *pCar@<EAX>)
void GetNetPos(tCar_spec* pCar) {
    int j;
    float amount;
    br_scalar total_deflection;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyPhysicsToCars(tU32 last_frame_time@<EAX>, tU32 pTime_difference@<EDX>)
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
    LOG_TRACE("(%d, %d)", last_frame_time, pTime_difference);

    step_number = 0;
    time_step = last_frame_time + pTime_difference;
    if (gFreeze_mechanics) {
        return;
    }
    if (gNet_mode == eNet_mode_client) {
        ForceRebuildActiveCarList();
    }
    if (gLast_mechanics_time < last_frame_time) {
        gLast_mechanics_time = PHYSICS_STEP_TIME * (last_frame_time / PHYSICS_STEP_TIME);
    }
    GetNonCars();
    if (gLast_mechanics_time < time_step) {
        gDoing_physics = 1;
        PrepareCars(last_frame_time);
        gDt = 0.039999999;
        gMechanics_time_sync = pTime_difference - (gLast_mechanics_time - last_frame_time);
        while (gLast_mechanics_time < time_step && step_number < 5) {
            step_number++;
            ResetOldmat();
            gProgram_state.current_car.old_v = gProgram_state.current_car.v;
            if (&gProgram_state.current_car != gCar_to_view) {
                gCar_to_view->old_v.v[0] = gCar_to_view->v.v[0];
                gCar_to_view->old_v.v[1] = gCar_to_view->v.v[1];
                gCar_to_view->old_v.v[2] = gCar_to_view->v.v[2];
            }
            for (i = 0; gNum_active_cars > i; ++i) {
                car = gActive_car_list[i];
                car->dt = -1.0;
                if (car->message.type == 15 && car->message.time >= gLast_mechanics_time && gLast_mechanics_time + PHYSICS_STEP_TIME >= car->message.time) {
                    car->dt = (double)(gLast_mechanics_time + PHYSICS_STEP_TIME - car->message.time) / 1000.0;
                    if (gDt - 0.0001 <= car->dt) {
                        GetNetPos(car);
                    } else if (gNet_mode == eNet_mode_host) {
                        car->dt = -1.0;
                    } else {
                        for (dam_index = 0; dam_index < 12; ++dam_index) {
                            if (car->damage_units[dam_index].damage_level < car->message.damage[dam_index]) {
                                car->dt = -1.0;
                                break;
                            }
                        }
                        if (car->dt >= 0.0) {
                            GetNetPos(car);
                        }
                    }
                }
                if (!car->disabled
                    && (!car->doing_nothing_flag || (car->driver >= eDriver_net_human && (!gRecover_timer || car->driver != eDriver_local_human)))) {
                    if (car->box_face_ref != gFace_num__car
                        && (car->box_face_ref != gFace_num__car - 1 || car->box_face_start <= gFace_count)) {
                        car_info = (tCollision_info*)car;
                        GetFacesInBox(car_info);
                    }
                    if (car->dt != 0.0) {
                        MoveAndCollideCar(car, gDt);
                    }
                }
            }
            for (i = 0; i < gNum_active_non_cars; ++i) {
                non_car = gActive_non_car_list[i];
                if (!non_car->collision_info.doing_nothing_flag) {
                    non_car->collision_info.dt = -1.0;
                    if (non_car->collision_info.message.type == 16 && non_car->collision_info.message.time >= gLast_mechanics_time && gLast_mechanics_time + PHYSICS_STEP_TIME >= non_car->collision_info.message.time) {
                        non_car->collision_info.dt = (double)(gLast_mechanics_time + PHYSICS_STEP_TIME - non_car->collision_info.message.time) / 1000.0;
                        GetNetPos((tCar_spec*)non_car);
                    }
                    if (non_car->collision_info.box_face_ref != gFace_num__car
                        && (non_car->collision_info.box_face_ref != gFace_num__car - 1
                            || non_car->collision_info.box_face_start <= gFace_count)) {
                        GetFacesInBox(&non_car->collision_info);
                    }
                    if (non_car->collision_info.dt != 0.0) {
                        MoveAndCollideNonCar(non_car, gDt);
                    }
                }
            }
            do {
                old_num_cars = gNum_cars_and_non_cars;
                CrashCarsTogether(gDt);
            } while (old_num_cars < gNum_cars_and_non_cars);
            gMechanics_time_sync -= PHYSICS_STEP_TIME;
            gLast_mechanics_time += PHYSICS_STEP_TIME;
        }
        gMechanics_time_sync = 1;
        SendCarData(gLast_mechanics_time);
        InterpolateCars(time_step, pTime_difference);
        FinishCars(time_step, pTime_difference);
        gDoing_physics = 0;
        CheckForDeAttachmentOfNonCars(pTime_difference);
    } else {
        //LOG_DEBUG("skipped physics");
        PrepareCars(last_frame_time);
        InterpolateCars(time_step, pTime_difference);
        FinishCars(time_step, pTime_difference);
    }
}

// IDA: void __usercall MungeSpecialVolume(tCollision_info *pCar@<EAX>)
void MungeSpecialVolume(tCollision_info* pCar) {
    tSpecial_volume* new_special_volume;
    tCar_spec* car;
    LOG_TRACE("(%p)", pCar);

    SILENT_STUB();
}

// IDA: void __usercall ResetCarSpecialVolume(tCollision_info *pCar@<EAX>)
void ResetCarSpecialVolume(tCollision_info* pCar) {
    br_vector3 cast_v;
    br_vector3 norm;
    br_scalar t;
    int id_len;
    char* mat_id;
    tSpecial_volume* new_special_volume;
    br_material* material;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall TestAutoSpecialVolume(tCollision_info *pCar@<EAX>)
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
    LOG_TRACE("(%p)", pCar);

    SILENT_STUB();
}

// IDA: void __usercall MoveAndCollideCar(tCar_spec *car@<EAX>, br_scalar dt)
void MoveAndCollideCar(tCar_spec* car, br_scalar dt) {
    tCollision_info* car_info;
    int wheel;
    LOG_TRACE("(%p, %f)", car, dt);

    if (car->dt >= 0.0) {
        dt = car->dt;
    }
    if (dt != 0.0 && (!gCar_flying || &gProgram_state.current_car != car)) {
        car_info = (tCollision_info*)car;
        car->new_skidding = 0;
        if (car->water_d != 10000.0) {
            TestAutoSpecialVolume(car_info);
        }
        MungeSpecialVolume(car_info);
        if (car->driver < eDriver_net_human) {
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
        car->pos.v[0] = car->pos.v[0] / 6.9000001;
        car->pos.v[1] = car->pos.v[1] / 6.9000001;
        car->pos.v[2] = car->pos.v[2] / 6.9000001;
        for (wheel = 0; wheel < 4; wheel++) {
            SkidMark(car, wheel);
        }
    }
}

// IDA: void __usercall MoveAndCollideNonCar(tNon_car_spec *non_car@<EAX>, br_scalar dt)
void MoveAndCollideNonCar(tNon_car_spec* non_car, br_scalar dt) {
    tCollision_info* car_info;
    LOG_TRACE("(%p, %f)", non_car, dt);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CollideCarWithWall@<EAX>(tCollision_info *car@<EAX>, br_scalar dt)
int CollideCarWithWall(tCollision_info* car, br_scalar dt) {
    LOG_TRACE("(%p, %f)", car, dt);

    GetFacesInBox(car);
    if (gCollision_detection_on__car) {
        car->collision_flag = 0;
        while (CollCheck(car, dt)) {
            car->collision_flag++;
            if (car->collision_flag - 1 > 20) {
                car->collision_flag = 1;
                car->v.v[0] = 0.0;
                car->v.v[1] = 0.0;
                car->v.v[2] = 0.0;
                car->omega.v[0] = 0.0;
                car->omega.v[1] = 0.0;
                car->omega.v[2] = 0.0;
                break;
            }
            RotateCar(car, dt);
            TranslateCar(car, dt);
            GetFacesInBox(car);
        }
        if (car->collision_flag) {
            CrashEarnings((tCar_spec*)car, 0);
        }
        BrMatrix34TApplyV(&car->velocity_car_space, &car->v, &car->oldmat);
        car->frame_collision_flag += car->collision_flag;
    }
    return car->collision_flag;
}

// IDA: void __cdecl ToggleControls()
void ToggleControls() {
    LOG_TRACE("()");

    if (!ControlCar[++gControl__car]) {
        gControl__car = 0;
    }
    switch (gControl__car) {
    case 0:
        NewTextHeadupSlot(4, 0, 500, -1, "Original Controls");
        break;
    case 1:
        NewTextHeadupSlot(4, 0, 500, -1, "Accelerated steering");
        break;
    case 2:
        NewTextHeadupSlot(4, 0, 500, -1, "0.75 Accelerated");
        break;
    case 3:
        NewTextHeadupSlot(4, 0, 500, -1, "0.5 Accelerated");
        break;
    default:
        NewTextHeadupSlot(4, 0, 500, -1, "New controls");
        break;
    }
}

// IDA: void __usercall ControlCar2(tCar_spec *c@<EAX>, br_scalar dt)
void ControlCar2(tCar_spec* c, br_scalar dt) {
    LOG_TRACE("(%p, %f)", c, dt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ControlCar3(tCar_spec *c@<EAX>, br_scalar dt)
void ControlCar3(tCar_spec* c, br_scalar dt) {
    LOG_TRACE("(%p, %f)", c, dt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ControlCar4(tCar_spec *c@<EAX>, br_scalar dt)
void ControlCar4(tCar_spec* c, br_scalar dt) {
    br_scalar ts;
    LOG_TRACE("(%p, %f)", c, dt);

    if (c->keys.left) {
        if (c->turn_speed < 0.0) {
            c->turn_speed = 0.0;
        }
        if (c->velocity_car_space.v[2] <= 0.0) {
            if ((c->curvature < 0.0 || c->omega.v[1] < -0.001) && c->turn_speed == 0.0) {
                c->turn_speed = 0.050000001
                    / (sqrt(c->v.v[1] * c->v.v[1] + c->v.v[2] * c->v.v[2] + c->v.v[0] * c->v.v[0]) + 5.0)
                    * (dt
                        * 25.0)
                    * 4.0
                    / 2.0
                    * 0.5;
                if (c->omega.v[1] < -0.01) {
                    c->turn_speed = c->turn_speed - dt * 0.01 / 0.04 / 2.0 * c->omega.v[1] * 2.0;
                }
            } else {
                c->turn_speed = 0.050000001
                        / (sqrt(c->v.v[1] * c->v.v[1] + c->v.v[2] * c->v.v[2] + c->v.v[0] * c->v.v[0]) + 5.0)
                        * (dt
                            * 25.0)
                        / 2.0
                        * 0.5
                    + c->turn_speed;
            }
        } else {
            c->turn_speed = dt * 0.01 / 0.04 / 2.0 * 2.0 + c->turn_speed;
        }
    }
    if (c->keys.right) {
        if (c->turn_speed > 0.0) {
            c->turn_speed = 0.0;
        }
        if (c->velocity_car_space.v[2] <= 0.0) {
            if ((c->curvature > 0.0 || c->omega.v[1] > 0.001) && c->turn_speed == 0.0) {
                c->turn_speed = 0.050000001
                    / (sqrt(c->v.v[1] * c->v.v[1] + c->v.v[2] * c->v.v[2] + c->v.v[0] * c->v.v[0]) + 5.0)
                    * (dt
                        * 25.0)
                    * -4.0
                    / 2.0
                    * 0.5;
                if (c->omega.v[1] < -0.01) {
                    c->turn_speed = c->turn_speed - dt * 0.01 / 0.04 / 2.0 * c->omega.v[1] * 2.0;
                }
            } else {
                c->turn_speed = c->turn_speed
                    - 0.050000001
                        / (sqrt(c->v.v[1] * c->v.v[1] + c->v.v[2] * c->v.v[2] + c->v.v[0] * c->v.v[0]) + 5.0)
                        * (dt
                            * 25.0)
                        / 2.0
                        * 0.5;
            }
        } else {
            c->turn_speed = c->turn_speed - dt * 0.01 / 0.04 / 2.0 * 2.0;
        }
    }
    if (c->keys.left || c->keys.right) {
        if (fabs(c->turn_speed) < fabs(dt * 2.0 * c->curvature) && c->curvature * c->turn_speed < 0.0) {
            c->turn_speed = -(dt * 2.0 * c->curvature);
        }
    } else {
        c->turn_speed = 0.0;
    }
    c->curvature = c->curvature + c->turn_speed;
    if (c->joystick.left <= 0) {
        if (c->joystick.right >= 0) {
            ts = (double)c->joystick.right * (double)c->joystick.right / 4294967300.0;
            c->curvature = c->maxcurve * -ts;
        }
    } else {
        c->curvature = (double)c->joystick.left * (double)c->joystick.left / 4294967300.0 * c->maxcurve;
    }
    if (c->curvature > (double)c->maxcurve) {
        c->curvature = c->maxcurve;
    }
    if (-c->maxcurve > c->curvature) {
        c->curvature = -c->maxcurve;
    }
}

// IDA: void __usercall ControlCar5(tCar_spec *c@<EAX>, br_scalar dt)
void ControlCar5(tCar_spec* c, br_scalar dt) {
    LOG_TRACE("(%p, %f)", c, dt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ControlCar1(tCar_spec *c@<EAX>, br_scalar dt)
void ControlCar1(tCar_spec* c, br_scalar dt) {
    LOG_TRACE("(%p, %f)", c, dt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall setrotate(br_vector3 *wdt@<EAX>, br_matrix34 *m@<EDX>)
void setrotate(br_vector3* wdt, br_matrix34* m) {
    br_euler e;
    LOG_TRACE("(%p, %p)", wdt, m);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RotateCar2(tCollision_info *c@<EAX>, br_scalar dt)
void RotateCar2(tCollision_info* c, br_scalar dt) {
    br_vector3 wdt;
    br_vector3 wdt2;
    br_vector3 L;
    br_vector3 L2;
    br_matrix34 m;
    LOG_TRACE("(%p, %f)", c, dt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RotateCarSecondOrder(tCollision_info *c@<EAX>, br_scalar dt)
void RotateCarSecondOrder(tCollision_info* c, br_scalar dt) {
    br_vector3 L;
    br_vector3 L2;
    br_vector3 axis;
    br_vector3 omega;
    br_scalar rad;
    br_scalar rad_rate;
    br_matrix34 m;
    LOG_TRACE("(%p, %f)", c, dt);

    rad = sqrt(c->omega.v[1] * c->omega.v[1] + c->omega.v[2] * c->omega.v[2] + c->omega.v[0] * c->omega.v[0]);
    rad_rate = rad * dt;
    omega.v[0] = c->omega.v[0] / rad;
    omega.v[1] = c->omega.v[1] / rad;
    omega.v[2] = c->omega.v[2] / rad;
    L.v[0] = c->I.v[0] * c->omega.v[0];
    L.v[1] = c->I.v[1] * c->omega.v[1];
    L.v[2] = c->I.v[2] * c->omega.v[2];
    BrMatrix34Rotate(&m, BrRadianToAngle(rad_rate) >> 1, &omega);
    BrMatrix34TApplyV(&L2, &L, &m);
    axis.v[0] = L2.v[0] / c->I.v[0];
    axis.v[1] = L2.v[1] / c->I.v[1];
    axis.v[2] = L2.v[2] / c->I.v[2];
    rad = sqrt(axis.v[1] * axis.v[1] + axis.v[2] * axis.v[2] + axis.v[0] * axis.v[0]);
    rad_rate = rad * dt;
    omega.v[0] = axis.v[0] / rad;
    omega.v[1] = axis.v[1] / rad;
    omega.v[2] = axis.v[2] / rad;
    BrMatrix34Rotate(&m, BrRadianToAngle(rad_rate), &omega);
    omega.v[2] = -c->cmpos.v[2];
    omega.v[1] = -c->cmpos.v[1];
    omega.v[0] = -c->cmpos.v[0];
    BrMatrix34PreTranslate(&m, omega.v[0], omega.v[1], omega.v[2]);
    BrMatrix34PostTranslate(&m, c->cmpos.v[0], c->cmpos.v[1], c->cmpos.v[2]);
    BrMatrix34Pre(&c->car_master_actor->t.t.mat, &m);
    BrMatrix34TApplyV(&L2, &L, &m);
    c->omega.v[0] = L2.v[0] / c->I.v[0];
    c->omega.v[1] = L2.v[1] / c->I.v[1];
    c->omega.v[2] = L2.v[2] / c->I.v[2];
}

// IDA: void __usercall RotateCarFirstOrder(tCollision_info *c@<EAX>, br_scalar dt)
void RotateCarFirstOrder(tCollision_info* c, br_scalar dt) {
    br_vector3 axis;
    br_vector3 L;
    br_vector3 L2;
    br_matrix34 m;
    br_scalar rad_rate;
    br_scalar rad;
    br_scalar e1;
    br_scalar e2;
    static br_scalar max_rad = 0.0001;
    LOG_TRACE("(%p, %f)", c, dt);

    rad = sqrt(c->omega.v[1] * c->omega.v[1] + c->omega.v[2] * c->omega.v[2] + c->omega.v[0] * c->omega.v[0]);
    rad_rate = rad * dt;
    if (rad_rate >= max_rad) {
        axis.v[0] = c->omega.v[0] / rad;
        axis.v[1] = c->omega.v[1] / rad;
        axis.v[2] = c->omega.v[2] / rad;
        L.v[0] = c->I.v[0] * c->omega.v[0];
        L.v[1] = c->I.v[1] * c->omega.v[1];
        L.v[2] = c->I.v[2] * c->omega.v[2];
        BrMatrix34Rotate(&m, BrRadianToAngle(rad_rate), &axis);
        BrMatrix34TApplyV(&L2, &L, &m);
        BrMatrix34PreTranslate(&m, -c->cmpos.v[0], -c->cmpos.v[1], -c->cmpos.v[2]);
        BrMatrix34PostTranslate(&m, c->cmpos.v[0], c->cmpos.v[1], c->cmpos.v[2]);
        BrMatrix34Pre(&c->car_master_actor->t.t.mat, &m);
        c->omega.v[0] = L2.v[0] / c->I.v[0];
        c->omega.v[1] = L2.v[1] / c->I.v[1];
        c->omega.v[2] = L2.v[2] / c->I.v[2];
    }
}

// IDA: void __usercall SimpleRotate(tCollision_info *c@<EAX>, br_scalar dt)
void SimpleRotate(tCollision_info* c, br_scalar dt) {
    br_vector3 axis;
    br_scalar rad_rate;
    br_scalar rad;
    LOG_TRACE("(%p, %f)", c, dt);

    rad = sqrt(c->omega.v[1] * c->omega.v[1] + c->omega.v[2] * c->omega.v[2] + c->omega.v[0] * c->omega.v[0]);
    axis.v[0] = c->omega.v[0] / rad;
    axis.v[1] = c->omega.v[1] / rad;
    axis.v[2] = c->omega.v[2] / rad;
    rad_rate = rad * dt;
    if (rad_rate >= 0.0001) {
        BrMatrix34PreRotate(&c->car_master_actor->t.t.mat, BrRadianToAngle(rad_rate), &axis);
    }
}

// IDA: void __usercall RotateCar(tCollision_info *c@<EAX>, br_scalar dt)
void RotateCar(tCollision_info* c, br_scalar dt) {
    br_scalar rad_squared;
    int steps;
    int i;
    LOG_TRACE("(%p, %f)", c, dt);

    rad_squared = (c->omega.v[1] * c->omega.v[1] + c->omega.v[2] * c->omega.v[2] + c->omega.v[0] * c->omega.v[0]) * dt;
    c->oldomega = c->omega;
    if (rad_squared >= 0.0000001) {
        if (rad_squared <= 0.0080000004) {
            RotateCarFirstOrder(c, dt);
        } else {
            steps = sqrt(rad_squared / 0.032000002) + 1;
            dt = dt / (double)steps;
            for (i = 0; i < steps && i < 20; ++i) {
                RotateCarSecondOrder(c, dt);
            }
        }
    }
}

// IDA: void __usercall SteeringSelfCentre(tCar_spec *c@<EAX>, br_scalar dt, br_vector3 *n)
void SteeringSelfCentre(tCar_spec* c, br_scalar dt, br_vector3* n) {
    br_scalar ts;
    br_scalar ts2;
    LOG_TRACE("(%p, %f, %p)", c, dt, n);

    if (c->curvature > (double)c->maxcurve) {
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
            // char s[256];
            // sprintf(s, "curve: %f", c->curvature);
            // ChangeHeadupText(gProgram_state.frame_rate_headup, s);
        }
    }
}

// IDA: void __usercall NonCarCalcForce(tNon_car_spec *nc@<EAX>, br_scalar dt)
void NonCarCalcForce(tNon_car_spec* nc, br_scalar dt) {
    tCollision_info* c;
    tSpecial_volume* vol;
    br_scalar ts;
    br_vector3 tv;
    br_vector3 v;
    LOG_TRACE("(%p, %f)", nc, dt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddDrag(tCar_spec *c@<EAX>, br_scalar dt)
void AddDrag(tCar_spec* c, br_scalar dt) {
    br_scalar drag_multiplier;
    br_scalar ts;
    tSpecial_volume* vol;
    br_vector3 b;
    LOG_TRACE("(%p, %f)", c, dt);

    vol = c->last_special_volume;
    drag_multiplier = -(dt * TIME_CONV_THING);
    if (vol) {
        if (c->underwater_ability) {
            drag_multiplier = vol->viscosity_multiplier * drag_multiplier * 0.6;
        } else {
            drag_multiplier = vol->viscosity_multiplier * drag_multiplier;
        }
        drag_multiplier = c->water_depth_factor * drag_multiplier;
    }
    ts = sqrt(c->v.v[1] * c->v.v[1] + c->v.v[2] * c->v.v[2] + c->v.v[0] * c->v.v[0]) * drag_multiplier / c->M;
    b.v[0] = c->v.v[0] * ts;
    b.v[1] = c->v.v[1] * ts;
    b.v[2] = c->v.v[2] * ts;
    c->v.v[0] = c->v.v[0] + b.v[0];
    c->v.v[1] = c->v.v[1] + b.v[1];
    c->v.v[2] = c->v.v[2] + b.v[2];
    ts = sqrt(c->omega.v[1] * c->omega.v[1] + c->omega.v[2] * c->omega.v[2] + c->omega.v[0] * c->omega.v[0]) * drag_multiplier;
    b.v[0] = c->omega.v[0] * ts;
    b.v[1] = c->omega.v[1] * ts;
    b.v[2] = c->omega.v[2] * ts;
    ApplyTorque(c, &b);
}

// IDA: void __usercall DoBumpiness(tCar_spec *c@<EAX>, br_vector3 *wheel_pos@<EDX>, br_vector3 *norm@<EBX>, br_scalar *d@<ECX>, int n)
void DoBumpiness(tCar_spec* c, br_vector3* wheel_pos, br_vector3* norm, br_scalar* d, int n) {
    br_vector3 tv;
    int delta;
    int x;
    int y;
    tMaterial_modifiers* mat_list;
    LOG_TRACE("(%p, %p, %p, %p, %d)", c, wheel_pos, norm, d, n);

    SILENT_STUB();
}

// IDA: void __usercall CalcForce(tCar_spec *c@<EAX>, br_scalar dt)
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
    static br_scalar stop_timer;
    static br_scalar slide_dist;
    tDamage_type dam;
    br_scalar v;
    tSpecial_volume* vol;
    br_scalar scale;
    LOG_TRACE("(%p, %f)", c, dt);

    int v72; // [esp+24h] [ebp-1C8h]
    double v73; // [esp+2Ch] [ebp-1C0h]
    float v74; // [esp+34h] [ebp-1B8h]
    float v75; // [esp+38h] [ebp-1B4h]
    float v76; // [esp+3Ch] [ebp-1B0h]
    float v77; // [esp+40h] [ebp-1ACh]
    float v78; // [esp+44h] [ebp-1A8h]
    float v79; // [esp+48h] [ebp-1A4h]
    float v80; // [esp+4Ch] [ebp-1A0h] MAPDST
    float v82; // [esp+50h] [ebp-19Ch] MAPDST
    float v84; // [esp+54h] [ebp-198h]
    float v85; // [esp+58h] [ebp-194h] MAPDST
    float v87; // [esp+5Ch] [ebp-190h] MAPDST
    float v98; // [esp+88h] [ebp-164h]
    float v99; // [esp+8Ch] [ebp-160h]
    br_vector3 v102; // [esp+98h] [ebp-154h]
    br_vector3 v103; // [esp+A4h] [ebp-148h]
    int v105; // [esp+B8h] [ebp-134h]
    float v106; // [esp+C0h] [ebp-12Ch]
    br_vector3 v107; // [esp+C4h] [ebp-128h]
    float v108; // [esp+D0h] [ebp-11Ch]
    float v109; // [esp+D4h] [ebp-118h]
    float v116; // [esp+F8h] [ebp-F4h]
    br_vector3 B; // [esp+FCh] [ebp-F0h] BYREF
    br_scalar pV; // [esp+10Ch] [ebp-E0h]
    br_vector3 v123; // [esp+130h] [ebp-BCh]
    float v125; // [esp+16Ch] [ebp-80h]
    float v128; // [esp+18Ch] [ebp-60h]
    float v129; // [esp+190h] [ebp-5Ch]
    float v134; // [esp+1D8h] [ebp-14h]
    float v135; // [esp+1DCh] [ebp-10h]
    br_vector3 v136; // [esp+1E0h] [ebp-Ch]

    v136.v[0] = 0;
    v136.v[1] = 0;
    v136.v[2] = 0;

    normnum = 0;
    f.v[0] = 0.0;
    f.v[1] = 0.0;
    f.v[2] = 0.0;
    B.v[0] = 0.0;
    B.v[1] = 0.0;
    B.v[2] = 0.0;
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
    c->road_normal.v[0] = 0.0;
    c->road_normal.v[1] = 0.0;
    c->road_normal.v[2] = 0.0;
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
            c->road_normal.v[0] = norm[i].v[0] + c->road_normal.v[0];
            c->road_normal.v[1] = norm[i].v[1] + c->road_normal.v[1];
            c->road_normal.v[2] = norm[i].v[2] + c->road_normal.v[2];
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
        scale = sqrt(
            c->road_normal.v[1] * c->road_normal.v[1]
            + c->road_normal.v[2] * c->road_normal.v[2]
            + c->road_normal.v[0] * c->road_normal.v[0]);
        if (scale <= 2.3841858e-7) {
            v107.v[0] = 1.0;
            v107.v[1] = 0.0;
            v107.v[2] = 0.0;
        } else {
            scale = 1.0 / scale;
            v107.v[0] = c->road_normal.v[0] * scale;
            v107.v[1] = c->road_normal.v[1] * scale;
            v107.v[2] = c->road_normal.v[2] * scale;
        }
        v107.v[0] = -(c->M * 10.0) * v107.v[0];
        v107.v[1] = -(c->M * 10.0) * v107.v[1];
        v107.v[2] = -(c->M * 10.0) * v107.v[2];
        B.v[0] = B.v[0] + v107.v[0];
        B.v[1] = B.v[1] + v107.v[1];
        B.v[2] = B.v[2] + v107.v[2];
    }
    if (c->driver >= eDriver_net_human) {
        SteeringSelfCentre(c, dt, &c->road_normal);
    }
    if (normnum) {
        // ts = 1.0 / sqrt(1.0); <- looked like this in the windows build definitely wrong
        ts = 1.0 / sqrt(c->road_normal.v[0] * c->road_normal.v[0] + c->road_normal.v[1] * c->road_normal.v[1] + c->road_normal.v[2] * c->road_normal.v[2]);
        c->road_normal.v[0] = c->road_normal.v[0] * ts;
        c->road_normal.v[1] = c->road_normal.v[1] * ts;
        c->road_normal.v[2] = c->road_normal.v[2] * ts;
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
                b.v[0] = (c->wpos[0].v[2] - c->wpos[2].v[2]) * b.v[0];
                b.v[1] = (c->wpos[0].v[2] - c->wpos[2].v[2]) * b.v[1];
                b.v[2] = (c->wpos[0].v[2] - c->wpos[2].v[2]) * b.v[2];
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
        vplane.v[0] = (c->velocity_car_space.v[1] * c->road_normal.v[1]
                          + c->road_normal.v[2] * c->velocity_car_space.v[2]
                          + c->road_normal.v[0] * c->velocity_car_space.v[0])
            * c->road_normal.v[0];
        vplane.v[1] = (c->velocity_car_space.v[1] * c->road_normal.v[1]
                          + c->road_normal.v[2] * c->velocity_car_space.v[2]
                          + c->road_normal.v[0] * c->velocity_car_space.v[0])
            * c->road_normal.v[1];
        vplane.v[2] = (c->velocity_car_space.v[1] * c->road_normal.v[1]
                          + c->road_normal.v[2] * c->velocity_car_space.v[2]
                          + c->road_normal.v[0] * c->velocity_car_space.v[0])
            * c->road_normal.v[2];
        vplane.v[0] = c->velocity_car_space.v[0] - vplane.v[0];
        vplane.v[1] = c->velocity_car_space.v[1] - vplane.v[1];
        vplane.v[2] = c->velocity_car_space.v[2] - vplane.v[2];
        if (vplane.v[2] < 0.0) {
            ts = 1.0;
        } else {
            ts = -1.0;
        }
        ts3 = sqrt(vplane.v[2] * vplane.v[2] + vplane.v[1] * vplane.v[1] + vplane.v[0] * vplane.v[0]);
        deltaomega = ts3 * c->curvature * ts;
        deltaomega = deltaomega - (c->omega.v[1] * c->road_normal.v[1] + c->road_normal.v[2] * c->omega.v[2] + c->road_normal.v[0] * c->omega.v[0]);
        v103.v[0] = c->road_normal.v[1];
        v103.v[1] = -c->road_normal.v[0];
        v103.v[2] = 0.0;
        scale = sqrt(0.0 * 0.0 + v103.v[1] * v103.v[1] + v103.v[0] * v103.v[0]);
        if (scale <= 2.3841858e-7) {
            v103.v[0] = 1.0;
            v103.v[1] = 0.0;
            v103.v[2] = 0.0;
        } else {
            scale = 1.0 / scale;
            v103.v[0] = v103.v[0] * scale;
            v103.v[1] = v103.v[1] * scale;
            v103.v[2] = v103.v[2] * scale;
        }

        friction_number = c->I.v[1] / dt * deltaomega;
        ts = friction_number / (c->wpos[2].v[2] - c->wpos[0].v[2]);
        v108 = ts;
        v109 = -ts;
        rightplane.v[0] = 0.0;
        rightplane.v[1] = c->road_normal.v[2];
        rightplane.v[2] = -c->road_normal.v[1];
        scale = sqrt(rightplane.v[1] * rightplane.v[1] + rightplane.v[2] * rightplane.v[2] + 0.0 * 0.0);
        if (scale <= 2.3841858e-7) {
            rightplane.v[0] = 1.0;
            rightplane.v[1] = 0.0;
            rightplane.v[2] = 0.0;
        } else {
            scale = 1.0 / scale;
            rightplane.v[0] = rightplane.v[0] * scale;
            rightplane.v[1] = rightplane.v[1] * scale;
            rightplane.v[2] = rightplane.v[2] * scale;
        }
        v99 = c->acc_force;
        friction_number = rightplane.v[1] * vplane.v[1] + rightplane.v[2] * vplane.v[2] + vplane.v[0] * rightplane.v[0];
        v87 = v103.v[2] * vplane.v[2] + v103.v[1] * vplane.v[1] + v103.v[0] * vplane.v[0];
        ts2 = fabs(v87);
        friction_number = (c->wpos[0].v[2] - c->cmpos.v[2]) * friction_number * fabs(c->curvature);
        if (c->curvature <= 0.0) {
            friction_number = v87 - friction_number;
        } else {
            friction_number = v87 + friction_number;
        }
        friction_number = -(c->M / dt * friction_number);
        friction_number = friction_number - (B.v[1] * v103.v[1] + B.v[2] * v103.v[2] + B.v[0] * v103.v[0]);

        friction_number = friction_number / (1.0 - wheelratio);
        v108 = friction_number + v108;
        v109 = -wheelratio * friction_number + v109;
        friction_number = (c->wpos[0].v[2] - c->wpos[2].v[2]) * v108;
        v98 = friction_number * c->curvature;
        friction_number = (c->velocity_car_space.v[1] * rightplane.v[1] + rightplane.v[2] * c->velocity_car_space.v[2] + c->velocity_car_space.v[0] * rightplane.v[0]) * c->M / dt;
        v129 = rightplane.v[1] * B.v[1] + rightplane.v[2] * B.v[2] + B.v[0] * rightplane.v[0] + friction_number;
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
        v102.v[0] = c->wpos[0].v[0] - c->cmpos.v[0];
        v102.v[1] = c->wpos[0].v[1] - c->cmpos.v[1];
        v102.v[2] = c->wpos[0].v[2] - c->cmpos.v[2];
        a.v[0] = c->omega.v[1] * v102.v[2] - c->omega.v[2] * v102.v[1];
        a.v[1] = c->omega.v[2] * v102.v[0] - v102.v[2] * c->omega.v[0];
        a.v[2] = v102.v[1] * c->omega.v[0] - c->omega.v[1] * v102.v[0];
        a.v[0] = c->velocity_car_space.v[0] + a.v[0];
        a.v[1] = c->velocity_car_space.v[1] + a.v[1];
        a.v[2] = c->velocity_car_space.v[2] + a.v[2];
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
        // printf("outside of check. rpm %f, target %f\n", c->revs, c->target_revs);
        if (rl_oil_factor == 1.0 && rr_oil_factor == 1.0 && c->traction_control && v135 * 2.0 > c->max_force_rear && c->acc_force > 0.0
            && (c->driver < eDriver_net_human || (c->target_revs > 1000.0 && c->gear > 0))) {
            // printf(">>> inside check v99=%f, v135=%f\n", v99, v135);
            ts2 = v99;
            if (v99 * v99 <= v135 * v135 * 4.0) {
                v87 = sqrt(v135 * v135 * 4.0 - v99 * v99);
            } else {
                v87 = 0.0;
            }
            if (c->max_force_rear <= v87) {
                printf("affecting torque ***** \n");
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

            // v99 = 8.8f;
            // v135 = 4.4f;
            printf("final values v99=%f v135=%f ts2=%f\n", v99, v135, ts2);
        } else if (c->driver >= eDriver_net_human && c->gear > 0 && c->revs > c->target_revs && !c->traction_control) {
            if (!c->keys.change_down) {
                c->traction_control = 1;
            }
            LOG_DEBUG("revs > target_revs");
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
            ts2 = (v103.v[2] * vplane.v[2] + v103.v[1] * vplane.v[1] + v103.v[0] * vplane.v[0] + v109 / c->M) * v108;
            ts2 = (rightplane.v[1] * vplane.v[1] + rightplane.v[2] * vplane.v[2] + vplane.v[0] * rightplane.v[0]) * v98 + ts2;
            ts2 = (c->omega.v[1] * c->road_normal.v[1] + c->road_normal.v[2] * c->omega.v[2] + c->road_normal.v[0] * c->omega.v[0]) * (c->wpos[2].v[2] - c->cmpos.v[2]) * v108 + ts2;
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
        if (c->wheel_slip
            && c->curvature * c->turn_speed > 0.0
            && fabs(v109) > 0.0099999998
            && c->curvature * v109 < 0.0
            && !c->keys.brake
            && !c->keys.change_down) {
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
        v136.v[0] = rightplane.v[0] * v99;
        v136.v[1] = rightplane.v[1] * v99;
        v136.v[2] = rightplane.v[2] * v99;
        a.v[0] = v103.v[0] * v109;
        a.v[1] = v103.v[1] * v109;
        a.v[2] = v103.v[2] * v109;
        v136.v[0] = a.v[0] + v136.v[0];
        v136.v[1] = v136.v[1] + a.v[1];
        v136.v[2] = v136.v[2] + a.v[2];
        v123.v[0] = rightplane.v[0] * v98;
        v123.v[1] = rightplane.v[1] * v98;
        v123.v[2] = rightplane.v[2] * v98;
        a.v[0] = v103.v[0] * v108;
        a.v[1] = v103.v[1] * v108;
        a.v[2] = v103.v[2] * v108;
        v123.v[0] = v123.v[0] + a.v[0];
        v123.v[1] = v123.v[1] + a.v[1];
        v123.v[2] = v123.v[2] + a.v[2];
        rightplane = c->wpos[0];
        rightplane.v[1] = rightplane.v[1] - c->oldd[0];
        rightplane.v[0] = rightplane.v[0] - c->cmpos.v[0];
        rightplane.v[1] = rightplane.v[1] - c->cmpos.v[1];
        rightplane.v[2] = rightplane.v[2] - c->cmpos.v[2];
        b.v[0] = v136.v[0] * force[0];
        b.v[1] = v136.v[1] * force[0];
        b.v[2] = v136.v[2] * force[0];
        B.v[0] = B.v[0] + b.v[0];
        B.v[1] = B.v[1] + b.v[1];
        B.v[2] = B.v[2] + b.v[2];
        a.v[0] = rightplane.v[1] * b.v[2] - b.v[1] * rightplane.v[2];
        a.v[1] = rightplane.v[2] * b.v[0] - b.v[2] * rightplane.v[0];
        a.v[2] = b.v[1] * rightplane.v[0] - rightplane.v[1] * b.v[0];
        f.v[0] = f.v[0] + a.v[0];
        f.v[1] = f.v[1] + a.v[1];
        f.v[2] = f.v[2] + a.v[2];
        rightplane = c->wpos[1];
        rightplane.v[1] = rightplane.v[1] - c->oldd[1];
        rightplane.v[0] = rightplane.v[0] - c->cmpos.v[0];
        rightplane.v[1] = rightplane.v[1] - c->cmpos.v[1];
        rightplane.v[2] = rightplane.v[2] - c->cmpos.v[2];
        b.v[0] = force[1] * v136.v[0];
        b.v[1] = force[1] * v136.v[1];
        b.v[2] = force[1] * v136.v[2];
        B.v[0] = B.v[0] + b.v[0];
        B.v[1] = B.v[1] + b.v[1];
        B.v[2] = B.v[2] + b.v[2];
        a.v[0] = rightplane.v[1] * b.v[2] - b.v[1] * rightplane.v[2];
        a.v[1] = rightplane.v[2] * b.v[0] - b.v[2] * rightplane.v[0];
        a.v[2] = b.v[1] * rightplane.v[0] - rightplane.v[1] * b.v[0];
        f.v[0] = f.v[0] + a.v[0];
        f.v[1] = f.v[1] + a.v[1];
        f.v[2] = f.v[2] + a.v[2];
        rightplane = c->wpos[2];
        rightplane.v[1] = rightplane.v[1] - c->oldd[2];
        rightplane.v[0] = rightplane.v[0] - c->cmpos.v[0];
        rightplane.v[1] = rightplane.v[1] - c->cmpos.v[1];
        rightplane.v[2] = rightplane.v[2] - c->cmpos.v[2];
        b.v[0] = force[2] * v123.v[0];
        b.v[1] = force[2] * v123.v[1];
        b.v[2] = force[2] * v123.v[2];
        B.v[0] = B.v[0] + b.v[0];
        B.v[1] = B.v[1] + b.v[1];
        B.v[2] = B.v[2] + b.v[2];
        a.v[0] = rightplane.v[1] * b.v[2] - b.v[1] * rightplane.v[2];
        a.v[1] = rightplane.v[2] * b.v[0] - b.v[2] * rightplane.v[0];
        a.v[2] = b.v[1] * rightplane.v[0] - rightplane.v[1] * b.v[0];
        f.v[0] = f.v[0] + a.v[0];
        f.v[1] = f.v[1] + a.v[1];
        f.v[2] = f.v[2] + a.v[2];
        rightplane = c->wpos[3];
        rightplane.v[1] = rightplane.v[1] - c->oldd[3];
        rightplane.v[0] = rightplane.v[0] - c->cmpos.v[0];
        rightplane.v[1] = rightplane.v[1] - c->cmpos.v[1];
        rightplane.v[2] = rightplane.v[2] - c->cmpos.v[2];
        b.v[0] = v123.v[0] * force[3];
        b.v[1] = v123.v[1] * force[3];
        b.v[2] = v123.v[2] * force[3];
        B.v[0] = B.v[0] + b.v[0];
        B.v[1] = B.v[1] + b.v[1];
        B.v[2] = B.v[2] + b.v[2];
        a.v[0] = rightplane.v[1] * b.v[2] - b.v[1] * rightplane.v[2];
        a.v[1] = rightplane.v[2] * b.v[0] - b.v[2] * rightplane.v[0];
        a.v[2] = b.v[1] * rightplane.v[0] - rightplane.v[1] * b.v[0];
        f.v[0] = f.v[0] + a.v[0];
        f.v[1] = f.v[1] + a.v[1];
        f.v[2] = f.v[2] + a.v[2];
    } else {
        c->max_force_front = 0.0;
        c->max_force_rear = 0.0;
        StopSkid(c);
    }
    c->number_of_wheels_on_ground = normnum;
    BrMatrix34ApplyV(&b, &B, mat);
    rightplane.v[0] = f.v[0] * dt;
    rightplane.v[1] = f.v[1] * dt;
    rightplane.v[2] = f.v[2] * dt;
    ApplyTorque(c, &rightplane);
    rightplane.v[0] = dt / c->M * b.v[0];
    rightplane.v[1] = dt / c->M * b.v[1];
    rightplane.v[2] = dt / c->M * b.v[2];
    c->v.v[0] = c->v.v[0] + rightplane.v[0];
    c->v.v[1] = c->v.v[1] + rightplane.v[1];
    c->v.v[2] = c->v.v[2] + rightplane.v[2];
    if (c->speed < 0.000099999997
        && ((!c->keys.acc && c->joystick.acc <= 0) || !c->gear)
        && !c->keys.dec
        && c->joystick.dec <= 0
        && c->bounce_rate == 0.0
        && sqrt(c->omega.v[1] * c->omega.v[1] + c->omega.v[2] * c->omega.v[2] + c->omega.v[0] * c->omega.v[0]) < 0.05) {
        if (vol) {
            v73 = c->driver > eDriver_non_car && c->underwater_ability ? 1.0 - (1.0 - vol->gravity_multiplier) * 0.6 : vol->gravity_multiplier;
            friction_number = sqrt(b.v[2] * b.v[2] + b.v[1] * b.v[1] + b.v[0] * b.v[0]) / v73 / gGravity_multiplier;
        } else {
            friction_number = sqrt(b.v[2] * b.v[2] + b.v[1] * b.v[1] + b.v[0] * b.v[0]);
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
        //LOG_DEBUG("old %f new %f", old, c->acc_force);
    }
}

// IDA: void __usercall DoRevs(tCar_spec *c@<EAX>, br_scalar dt)
void DoRevs(tCar_spec* c, br_scalar dt) {
    br_scalar wheel_spin_force;
    br_scalar ts;
    int revs_increase;
    LOG_TRACE("(%p, %f)", c, dt);

    ts = -(c->car_master_actor->t.t.mat.m[2][2] * c->v.v[2]
        + c->car_master_actor->t.t.mat.m[2][1] * c->v.v[1]
        + c->car_master_actor->t.t.mat.m[2][0] * c->v.v[0]);

    //LOG_DEBUG("ts %f, acc %f", ts, c->acc_force);
    if (c->gear) {
        c->target_revs = ts / c->speed_revs_ratio / (double)c->gear;
    } else {
        c->target_revs = 0.0;
    }
    if (c->target_revs < 0.0) {
        c->target_revs = 0.0;
        c->gear = 0;
        LOG_DEBUG("changedown2");
    }
    if (!c->number_of_wheels_on_ground || ((c->wheel_slip & 2) + 1) != 0 || !c->gear) {
        if (c->number_of_wheels_on_ground) {
            //LOG_DEBUG("spin1");
            wheel_spin_force = c->force_torque_ratio * c->torque - (double)c->gear * c->acc_force;
        } else {
            wheel_spin_force = c->force_torque_ratio * c->torque;
            LOG_DEBUG("spin2");
        }
        if (c->gear) {
            if (c->gear < 2 && (c->keys.dec || c->joystick.dec > 0) && fabs(ts) < 1.0 && c->revs > 1000.0) {
                LOG_DEBUG("something 2");
                c->gear = -c->gear;
            }
        } else {
            if (c->revs > 1000.0 && !c->keys.brake && (c->keys.acc || c->joystick.acc > 0) && !gCountdown) {
                if (c->keys.backwards) {
                    LOG_DEBUG("change_down3");
                    c->gear = -1;
                } else {
                    LOG_DEBUG("change_up2");
                    c->gear = 1;
                }
            }
            wheel_spin_force = c->force_torque_ratio * c->torque;
        }
        //LOG_DEBUG("setting revs %f. torque %f", wheel_spin_force, c->torque);
        //printf("revs 0 %f, torque %f acc_force %f\n", c->revs, c->torque, c->acc_force);
        c->revs = wheel_spin_force / c->force_torque_ratio * dt / 0.0002 + c->revs;
        //printf("revs 1 %f\n", c->revs);
        if (c->traction_control && wheel_spin_force > 0.0 && c->revs > c->target_revs && c->gear && c->target_revs > 1000.0) {
            LOG_DEBUG("revs = target_revs");
            c->revs = c->target_revs;
        }
        if (c->revs <= 0.0) {
            c->revs = 0.0;
        }
    }
    if ((c->wheel_slip & 2) == 0 && c->target_revs > 6000.0 && c->revs > 6000.0 && c->gear < c->max_gear && c->gear > 0 && !c->just_changed_gear) {
        c->gear++;
        LOG_DEBUG("gear_up");
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

    //LOG_DEBUG("target_revs %f, %f, ac %d", c->target_revs, c->revs, c->keys.acc);

    // if (c->driver == eDriver_local_human) {
    //     char s[256];
    //     sprintf(s, "gear %d, speed %f, rpm: %f", c->gear, c->speed, c->revs);
    //     ChangeHeadupText(gProgram_state.frame_rate_headup, s);
    // }
}

// IDA: void __usercall ApplyTorque(tCar_spec *c@<EAX>, br_vector3 *tdt@<EDX>)
void ApplyTorque(tCar_spec* c, br_vector3* tdt) {
    LOG_TRACE("(%p, %p)", c, tdt);

    c->omega.v[0] = tdt->v[0] / c->I.v[0] + c->omega.v[0];
    c->omega.v[1] = tdt->v[1] / c->I.v[1] + c->omega.v[1];
    c->omega.v[2] = tdt->v[2] / c->I.v[2] + c->omega.v[2];
}

// IDA: void __usercall TranslateCar(tCollision_info *c@<EAX>, br_scalar dt)
void TranslateCar(tCollision_info* c, br_scalar dt) {
    br_vector3 t;
    LOG_TRACE("(%p, %f)", c, dt);

    t.v[0] = c->v.v[0] * dt;
    t.v[1] = c->v.v[1] * dt;
    t.v[2] = c->v.v[2] * dt;
    c->car_master_actor->t.t.mat.m[3][0] = c->car_master_actor->t.t.mat.m[3][0] + t.v[0];
    c->car_master_actor->t.t.mat.m[3][1] = c->car_master_actor->t.t.mat.m[3][1] + t.v[1];
    c->car_master_actor->t.t.mat.m[3][2] = c->car_master_actor->t.t.mat.m[3][2] + t.v[2];
}

// IDA: int __usercall CollCheck@<EAX>(tCollision_info *c@<EAX>, br_scalar dt)
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
    static int oldk;
    br_scalar min;
    br_scalar max;
    br_vector3 edges[3];
    br_vector3 corner;
    br_vector3 test_dir;
    br_scalar min_acc;
    br_scalar max_acc;
    br_matrix34 message_mat;
    LOG_TRACE("(%p, %f)", c, dt);

    SILENT_STUB();
    return 0;
}

// IDA: br_scalar __usercall AddFriction@<ST0>(tCollision_info *c@<EAX>, br_vector3 *vel@<EDX>, br_vector3 *normal_force@<EBX>, br_vector3 *pos@<ECX>, br_scalar total_force, br_vector3 *max_friction)
br_scalar AddFriction(tCollision_info* c, br_vector3* vel, br_vector3* normal_force, br_vector3* pos, br_scalar total_force, br_vector3* max_friction) {
    br_vector3 norm;
    br_vector3 tv;
    br_vector3 ftau;
    br_scalar ts;
    br_scalar point_vel;
    LOG_TRACE("(%p, %p, %p, %p, %f, %p)", c, vel, normal_force, pos, total_force, max_friction);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddFrictionCarToCar(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_vector3 *vel1@<EBX>, br_vector3 *vel2@<ECX>, br_vector3 *normal_force1, br_vector3 *pos1, br_vector3 *pos2, br_scalar total_force, br_vector3 *max_friction)
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
    LOG_TRACE("(%p, %p, %p, %p, %p, %p, %p, %f, %p)", car1, car2, vel1, vel2, normal_force1, pos1, pos2, total_force, max_friction);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScrapeNoise(br_scalar vel, br_vector3 *position, int material)
void ScrapeNoise(br_scalar vel, br_vector3* position, int material) {
    tS3_volume vol;
    static tS3_sound_tag scrape_tag;
    static tS3_volume last_scrape_vol;
    br_vector3 velocity;
    br_vector3 position_in_br;
    LOG_TRACE("(%f, %p, %d)", vel, position, material);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SkidNoise(tCar_spec *pC@<EAX>, int pWheel_num@<EDX>, br_scalar pV, int material)
void SkidNoise(tCar_spec* pC, int pWheel_num, br_scalar pV, int material) {
    br_vector3 pos;
    br_vector3 world_pos;
    br_vector3 wv;
    br_vector3 wvw;
    br_scalar ts;
    static tS3_volume last_skid_vol[2];
    int i;
    LOG_TRACE("(%p, %d, %f, %d)", pC, pWheel_num, pV, material);

    STUB();
}

// IDA: void __usercall StopSkid(tCar_spec *pC@<EAX>)
void StopSkid(tCar_spec* pC) {
    LOG_TRACE("(%p)", pC);

    STUB();
}

// IDA: void __usercall CrashNoise(br_vector3 *pForce@<EAX>, br_vector3 *position@<EDX>, int material@<EBX>)
void CrashNoise(br_vector3* pForce, br_vector3* position, int material) {
    static tS3_sound_tag crunch_tag;
    static tS3_volume last_crunch_vol;
    tS3_volume vol;
    br_vector3 velocity;
    LOG_TRACE("(%p, %p, %d)", pForce, position, material);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CrushAndDamageCar(tCar_spec *c@<EAX>, br_vector3 *pPosition@<EDX>, br_vector3 *pForce_car_space@<EBX>, tCar_spec *car2@<ECX>)
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
    LOG_TRACE("(%p, %p, %p, %p)", c, pPosition, pForce_car_space, car2);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ExpandBoundingBox@<EAX>(tCar_spec *c@<EAX>)
int ExpandBoundingBox(tCar_spec* c) {
    br_scalar min_z;
    br_scalar max_z;
    br_scalar dist;
    br_vector3 tv;
    br_vector3 old_pos;
    int l;
    br_matrix34 mat;
    LOG_TRACE("(%p)", c);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CrushBoundingBox(tCar_spec *c@<EAX>, int crush_only@<EDX>)
void CrushBoundingBox(tCar_spec* c, int crush_only) {
    br_vector3 min;
    br_vector3 max;
    int i;
    br_actor* actor;
    LOG_TRACE("(%p, %d)", c, crush_only);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AddCollPoint(br_scalar dist, br_vector3 *p, br_vector3 *norm, br_vector3 *r, br_vector3 *n, br_vector3 *dir, int num, tCollision_info *c)
void AddCollPoint(br_scalar dist, br_vector3* p, br_vector3* norm, br_vector3* r, br_vector3* n, br_vector3* dir, int num, tCollision_info* c) {
    static br_scalar d[4];
    int i;
    int furthest;
    LOG_TRACE("(%f, %p, %p, %p, %p, %p, %d, %p)", dist, p, norm, r, n, dir, num, c);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall SinglePointColl@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>)
br_scalar SinglePointColl(br_scalar* f, br_matrix4* m, br_scalar* d) {
    LOG_TRACE("(%p, %p, %p)", f, m, d);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall TwoPointColl@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n)
br_scalar TwoPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %p, %p)", f, m, d, tau, n);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall DrMatrix4Inverse@<ST0>(br_matrix4 *mi@<EAX>, br_matrix4 *mc@<EDX>)
br_scalar DrMatrix4Inverse(br_matrix4* mi, br_matrix4* mc) {
    LOG_TRACE("(%p, %p)", mi, mc);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall ThreePointColl@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>)
br_scalar ThreePointColl(br_scalar* f, br_matrix4* m, br_scalar* d) {
    br_matrix4 mc;
    br_matrix4 mi;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p)", f, m, d);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall ThreePointCollRec@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n, tCollision_info *c)
br_scalar ThreePointCollRec(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c) {
    int i;
    int j;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %p, %p, %p)", f, m, d, tau, n, c);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FourPointColl@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n, tCollision_info *c)
br_scalar FourPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c) {
    int i;
    int j;
    int l;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %p, %p, %p)", f, m, d, tau, n, c);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MultiFindFloorInBoxM(int pNum_rays@<EAX>, br_vector3 *a@<EDX>, br_vector3 *b@<EBX>, br_vector3 *nor@<ECX>, br_scalar *d, tCar_spec *c, int *mat_ref)
void MultiFindFloorInBoxM(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref) {
    br_vector3 aa[4];
    br_vector3 bb;
    int i;
    LOG_TRACE("(%d, %p, %p, %p, %p, %p, %p)", pNum_rays, a, b, nor, d, c, mat_ref);

    for (i = 0; i < pNum_rays; i++) {
        aa[i].v[0] = a[i].v[0] / 6.9000001;
        aa[i].v[1] = a[i].v[1] / 6.9000001;
        aa[i].v[2] = a[i].v[2] / 6.9000001;
        d[i] = 2.0;
    }
    bb.v[0] = b->v[0] / 6.9000001;
    bb.v[1] = b->v[1] / 6.9000001;
    bb.v[2] = b->v[2] / 6.9000001;
    MultiFindFloorInBoxBU(pNum_rays, aa, &bb, nor, d, c, mat_ref);
}

// IDA: void __usercall MultiFindFloorInBoxBU(int pNum_rays@<EAX>, br_vector3 *a@<EDX>, br_vector3 *b@<EBX>, br_vector3 *nor@<ECX>, br_scalar *d, tCar_spec *c, int *mat_ref)
void MultiFindFloorInBoxBU(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref) {
    br_vector3 nor2;
    int i;
    int j;
    int l;
    br_scalar dist[4];
    tFace_ref* face_ref;
    LOG_TRACE("(%d, %p, %p, %p, %p, %p, %p)", pNum_rays, a, b, nor, d, c, mat_ref);

    for (i = 0; i < c->box_face_end; i++) {
        face_ref = &gFace_list__car[i];
        if (!gEliminate_faces || (face_ref->flags & 0x80) == 0) {
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
void findfloor(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d) {
    br_material* material;
    br_vector3 aa;
    br_vector3 bb;
    LOG_TRACE("(%p, %p, %p, %p)", a, b, nor, d);

    aa.v[0] = a->v[0] / 6.9000001;
    aa.v[1] = a->v[1] / 6.9000001;
    aa.v[2] = a->v[2] / 6.9000001;
    bb.v[0] = b->v[0] / 6.9000001;
    bb.v[1] = b->v[1] / 6.9000001;
    bb.v[2] = b->v[2] / 6.9000001;
    FindFace(&aa, &bb, nor, d, &material);
}

// IDA: int __usercall FindFloorInBoxM@<EAX>(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>, tCollision_info *c)
int FindFloorInBoxM(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 aa;
    br_vector3 bb;
    LOG_TRACE("(%p, %p, %p, %p, %p)", a, b, nor, d, c);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FindFloorInBoxBU@<EAX>(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>, tCollision_info *c)
int FindFloorInBoxBU(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 nor2;
    int i;
    int j;
    br_scalar dist;
    tFace_ref* face_ref;
    LOG_TRACE("(%p, %p, %p, %p, %p)", a, b, nor, d, c);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FindFloorInBoxBU2@<EAX>(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>, tCollision_info *c)
int FindFloorInBoxBU2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 nor2;
    br_vector3 tv;
    int i;
    int j;
    br_scalar dist;
    tFace_ref* face_ref;
    LOG_TRACE("(%p, %p, %p, %p, %p)", a, b, nor, d, c);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FindFloorInBoxM2@<EAX>(br_vector3 *a@<EAX>, br_vector3 *b@<EDX>, br_vector3 *nor@<EBX>, br_scalar *d@<ECX>, tCollision_info *c)
int FindFloorInBoxM2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 aa;
    br_vector3 bb;
    LOG_TRACE("(%p, %p, %p, %p, %p)", a, b, nor, d, c);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall BoxFaceIntersect@<EAX>(br_bounds *pB@<EAX>, br_matrix34 *pM@<EDX>, br_matrix34 *pMold@<EBX>, br_vector3 *pPoint_list@<ECX>, br_vector3 *pNorm_list, br_scalar *pDist_list, int pMax_pnts, tCollision_info *c)
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
    LOG_TRACE("(%p, %p, %p, %p, %p, %p, %d, %p)", pB, pM, pMold, pPoint_list, pNorm_list, pDist_list, pMax_pnts, c);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall AddEdgeCollPoints@<EAX>(br_vector3 *p1@<EAX>, br_vector3 *p2@<EDX>, br_bounds *pB@<EBX>, br_matrix34 *pMold@<ECX>, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int n, int pMax_pnts, tCollision_info *c)
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
    LOG_TRACE("(%p, %p, %p, %p, %p, %p, %d, %d, %p)", p1, p2, pB, pMold, pPoint_list, pNorm_list, n, pMax_pnts, c);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetPlaneNormal(br_vector3 *n@<EAX>, int p@<EDX>)
void GetPlaneNormal(br_vector3* n, int p) {
    int d;
    LOG_TRACE("(%p, %d)", n, p);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetBoundsEdge@<EAX>(br_vector3 *pos@<EAX>, br_vector3 *edge@<EDX>, br_bounds *pB@<EBX>, int plane1@<ECX>, int plane2, br_vector3 *a, br_vector3 *b, br_vector3 *c, int flag)
int GetBoundsEdge(br_vector3* pos, br_vector3* edge, br_bounds* pB, int plane1, int plane2, br_vector3* a, br_vector3* b, br_vector3* c, int flag) {
    int d1;
    int d2;
    int d3;
    br_vector3 n;
    br_vector3 p;
    br_vector3 q;
    LOG_TRACE("(%p, %p, %p, %d, %d, %p, %p, %p, %d)", pos, edge, pB, plane1, plane2, a, b, c, flag);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall oldMoveOurCar(tU32 pTime_difference@<EAX>)
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
    LOG_TRACE("(%d)", pTime_difference);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleCollisionDetection()
void ToggleCollisionDetection() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CancelPendingCunningStunt()
void CancelPendingCunningStunt() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: float __cdecl frac(float pN)
float frac(float pN) {
    LOG_TRACE("(%f)", pN);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetAmbientPratCam(tCar_spec *pCar@<EAX>)
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
    static tU32 last_time_on_ground;
    LOG_TRACE("(%p)", pCar);

    SILENT_STUB();
}

// IDA: void __usercall MungeCarGraphics(tU32 pFrame_period@<EAX>)
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
    LOG_TRACE("(%d)", pFrame_period);

    if (gNet_mode
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
        if (cat) {
            car_count = GetCarCount(cat);
        } else {
            car_count = 1;
        }
        for (car = 0; car_count > car; car++) {
            if (cat) {
                the_car = GetCarSpec(cat, car);
            } else {
                the_car = &gProgram_state.current_car;
            }
            the_car->car_master_actor->render_style = (the_car->driver != eDriver_local_human) && PointOutOfSight(&the_car->pos, gYon_squared);
        }
    }
    for (car = 0; car < gNum_active_cars; car++) {
        the_car = gActive_car_list[car];
        if (the_car->car_master_actor->render_style != BR_RSTYLE_NONE) {
            car_x = the_car->car_master_actor->t.t.mat.m[3][0];
            car_z = the_car->car_master_actor->t.t.mat.m[3][2];
            the_car->shadow_intersection_flags = 0;
            oily_count = GetOilSpillCount();
            for (i = 0; i < oily_count; i++) {
                GetOilSpillDetails(i, &oily_actor, &oily_size);
                if (oily_actor) {
                    car_radius = the_car->bounds[1].max.v[2] / 6.9000001 * 1.5;
                    if (oily_actor->t.t.mat.m[3][0] - oily_size < car_x + car_radius
                        && oily_actor->t.t.mat.m[3][0] + oily_size > car_x - car_radius
                        && oily_actor->t.t.mat.m[3][2] - oily_size < car_z + car_radius
                        && oily_actor->t.t.mat.m[3][2] + oily_size > car_z - car_radius) {
                        the_car->shadow_intersection_flags |= 1 << i;
                    }
                }
            }
            if (the_car->driver < eDriver_net_human && (!gAction_replay_mode || !ReplayIsPaused())) {
                if (gCountdown) {
                    sine_angle = FRandomBetween(0.40000001, 1.6) * ((double)GetTotalTime() / ((double)gCountdown * 100.0));
                    sine_angle = frac(sine_angle) * 360.0;
                    sine_angle = FastScalarSin(sine_angle);
                    raw_revs = (double)the_car->red_line * fabs(sine_angle);
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
            for (i = 0; i < 4; i++) {
                ControlBoundFunkGroove(the_car->rf_sus_ref[i], the_car->rf_sus_position);
                if ((i & 1) != 0) {
                    ControlBoundFunkGroove(the_car->lf_sus_ref[i], -the_car->lf_sus_position);
                } else {
                    ControlBoundFunkGroove(the_car->lf_sus_ref[i], the_car->lf_sus_position);
                }
            }
            for (i = 0; i < 2; i++) {
                ControlBoundFunkGroove(the_car->rr_sus_ref[i], the_car->rr_sus_position);
                if ((i & 1) != 0) {
                    ControlBoundFunkGroove(the_car->lr_sus_ref[i], -the_car->lr_sus_position);
                } else {
                    ControlBoundFunkGroove(the_car->lr_sus_ref[i], the_car->lr_sus_position);
                }
            }
            if (!gAction_replay_mode || !ReplayIsPaused()) {
                wheel_speed = -(the_car->speedo_speed / the_car->non_driven_wheels_circum * (double)gFrame_period);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_1, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_2, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_3, wheel_speed);
                ControlBoundFunkGroovePlus(the_car->non_driven_wheels_spin_ref_4, wheel_speed);
                if (the_car->driver >= eDriver_net_human) {
                    if (the_car->gear) {
                        wheel_speed = -(the_car->revs
                            * the_car->speed_revs_ratio
                            / 6900.0
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
                abs_omega_x = (fabs(the_car->I.v[0]) + 3.3) / 2.0 * fabs(the_car->omega.v[0]);
                abs_omega_y = (fabs(the_car->I.v[1]) + 3.57) / 2.0 * fabs(the_car->omega.v[1]);
                abs_omega_z = (fabs(the_car->I.v[2]) + 0.44) / 2.0 * fabs(the_car->omega.v[2]);
                spinning_wildly = abs_omega_x > 26.4 || abs_omega_y > 49.98 || abs_omega_z > 3.52;
                if (spinning_wildly && the_time - gLast_cunning_stunt > 10000) {
                    if (gWild_start
                        && (!the_car->last_special_volume || the_car->last_special_volume->gravity_multiplier == 1.0)) {
                        if (the_time - gWild_start >= 500) {
                            DoFancyHeadup(9);
                            EarnCredits(gCunning_stunt_bonus[gProgram_state.skill_level]);
                            gLast_cunning_stunt = the_time;
                            gOn_me_wheels_start = 0;
                            gQuite_wild_end = 0;
                            gQuite_wild_start = 0;
                            gWoz_upside_down_at_all = 0;
                        }
                    } else {
                        gWild_start = the_time;
                    }
                } else {
                    gWild_start = 0;
                    spinning_mildly = abs_omega_x > 1.65 || abs_omega_z > 0.22;
                    if (the_car->number_of_wheels_on_ground <= 3) {
                        gOn_me_wheels_start = 0;
                        if (the_car->number_of_wheels_on_ground || !spinning_mildly) {
                            gQuite_wild_end = the_time;
                        } else {
                            if (!gQuite_wild_start) {
                                gQuite_wild_start = the_time;
                            }
                            if (the_car->car_master_actor->t.t.mat.m[1][1] < -0.80000001) {
                                gWoz_upside_down_at_all = the_time;
                            }
                        }
                    } else {
                        if (!gQuite_wild_end) {
                            gQuite_wild_end = the_time;
                        }
                        if (gQuite_wild_start
                            && the_time - gLast_cunning_stunt > 10000
                            && gQuite_wild_end - gQuite_wild_start >= 2000
                            && gWoz_upside_down_at_all >= gQuite_wild_start
                            && gWoz_upside_down_at_all <= gQuite_wild_end
                            && (gOn_me_wheels_start || the_time - gQuite_wild_end < 300)) {
                            if (gOn_me_wheels_start) {
                                if (the_time - gOn_me_wheels_start > 500
                                    && (!the_car->last_special_volume
                                        || the_car->last_special_volume->gravity_multiplier == 1.0)) {
                                    DoFancyHeadup(9);
                                    EarnCredits(gCunning_stunt_bonus[gProgram_state.skill_level]);
                                    gLast_cunning_stunt = PDGetTotalTime();
                                    gQuite_wild_end = 0;
                                    gQuite_wild_start = 0;
                                    gOn_me_wheels_start = 0;
                                    gWoz_upside_down_at_all = 0;
                                }
                            } else {
                                gOn_me_wheels_start = the_time;
                            }
                        } else {
                            gQuite_wild_end = 0;
                            gQuite_wild_start = 0;
                            gOn_me_wheels_start = 0;
                            gWoz_upside_down_at_all = 0;
                        }
                    }
                }
            }
            if (the_car->driver != eDriver_local_human && the_car->car_model_variable) {
                distance_from_camera = (car_x - gCamera_to_world.m[3][0])
                        * (car_x - gCamera_to_world.m[3][0])
                    + (car_z - gCamera_to_world.m[3][2])
                        * (car_z - gCamera_to_world.m[3][2])
                    + (the_car->car_master_actor->t.t.mat.m[3][1] - gCamera_to_world.m[3][1])
                        * (the_car->car_master_actor->t.t.mat.m[3][1] - gCamera_to_world.m[3][1]);

                distance_from_camera = distance_from_camera / gCar_simplification_factor[gGraf_spec_index][gCar_simplification_level];
                if (gNet_mode && gNet_players[gIt_or_fox].car == the_car) {
                    distance_from_camera = 0.0;
                }
                for (i = 0; the_car->car_actor_count > i; i++) {
                    if (the_car->car_model_actors[i].min_distance_squared <= distance_from_camera) {
                        SwitchCarActor(the_car, i);
                        break;
                    }
                }
            }
            if (the_car->screen_material) {
                the_material = NULL;
                if (the_car->last_special_volume && the_car->last_special_volume->screen_material) {
                    if (gAction_replay_mode && the_car->last_special_volume == gDefault_water_spec_vol) {
                        if (gProgram_state.current_depth_effect.type == eDepth_effect_fog) {
                            the_material = gProgram_state.standard_screen_fog;
                        } else if (gProgram_state.current_depth_effect.sky_texture) {
                            the_material = gProgram_state.standard_screen;
                        } else {
                            the_material = gProgram_state.standard_screen_dark;
                        }
                    } else {
                        the_material = the_car->last_special_volume->screen_material;
                    }
                } else if (gProgram_state.current_depth_effect.type == eDepth_effect_fog) {
                    the_material = gProgram_state.standard_screen_fog;
                } else if (gProgram_state.current_depth_effect.sky_texture) {
                    the_material = gProgram_state.standard_screen;
                } else {
                    the_material = gProgram_state.standard_screen_dark;
                }
                update_mat = 0;
                if (the_material && the_car->screen_material_source != the_material) {
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
                if (the_car->screen_material->colour_map) {
                    the_car->screen_material->map_transform.m[2][0] = fmod(car_x, 1.0);
                    the_car->screen_material->map_transform.m[2][1] = fmod(car_z, 1.0);
                    if (!update_mat) {
                        BrMaterialUpdate(the_car->screen_material, 1u);
                    }
                }
                if (update_mat) {
                    BrMaterialUpdate(the_car->screen_material, 0x7FFFu);
                }
            }
        }
    }
}

// IDA: void __cdecl ResetCarScreens()
void ResetCarScreens() {
    int cat;
    int car_count;
    int i;
    tCar_spec* the_car;
    LOG_TRACE("()");
    STUB();
}

// IDA: tCar_spec* __cdecl GetRaceLeader()
tCar_spec* GetRaceLeader() {
    int i;
    int score;
    tCar_spec* car;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AmIGettingBoredWatchingCameraSpin()
void AmIGettingBoredWatchingCameraSpin() {
    static tU32 time_of_death;
    static tU32 headup_timer;
    tCar_spec* car;
    char s[256];
    LOG_TRACE("()");

    SILENT_STUB();
}

// IDA: void __cdecl ViewNetPlayer()
void ViewNetPlayer() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ViewOpponent()
void ViewOpponent() {
    static int n;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleCarToCarCollisions()
void ToggleCarToCarCollisions() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SwapCar()
void SwapCar() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AdjustDownForce()
void AdjustDownForce() {
    char s[100];
    tCar_spec* c;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl FreezeMechanics()
void FreezeMechanics() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PutOpponentsInNeutral()
void PutOpponentsInNeutral() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetPanningFieldOfView()
void SetPanningFieldOfView() {
    br_camera* camera_ptr;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CheckDisablePlingMaterials(tCar_spec *pCar@<EAX>)
void CheckDisablePlingMaterials(tCar_spec* pCar) {
    br_matrix34* mat;
    br_scalar height;
    int i;
    LOG_TRACE("(%p)", pCar);
    SILENT_STUB();
}

// IDA: void __usercall PositionExternalCamera(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
void PositionExternalCamera(tCar_spec* c, tU32 pTime) {
    static int old_camera_mode;
    br_camera* camera_ptr;
    LOG_TRACE("(%p, %d)", c, pTime);

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
        if (c->car_master_actor->t.t.mat.m[3][0] <= 500.0) {
            if (gAction_replay_mode && gAction_replay_camera_mode) {
                LOG_PANIC("%d, %d", gAction_replay_mode, gAction_replay_camera_mode);
                if (gAction_replay_camera_mode == eAction_replay_action && (CheckDisablePlingMaterials(c), IncidentCam(c, pTime))) {
                    SetPanningFieldOfView();
                    EnablePlingMaterials();
                    old_camera_mode = gAction_replay_camera_mode;
                } else {
                    CheckDisablePlingMaterials(c);
                    SetPanningFieldOfView();
                    if (gAction_replay_camera_mode != old_camera_mode) {
                        SetUpPanningCamera(c);
                        old_camera_mode = gAction_replay_camera_mode;
                    }
                    PanningExternalCamera(c, pTime);
                    EnablePlingMaterials();
                }
            } else {
                NormalPositionExternalCamera(c, pTime);
            }
        }
    }
}

// IDA: void __usercall CameraBugFix(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
void CameraBugFix(tCar_spec* c, tU32 pTime) {
    br_matrix34 mat;
    br_matrix34* m2;
    br_vector3 tv;
    LOG_TRACE("(%p, %d)", c, pTime);
    SILENT_STUB();
}

// IDA: int __usercall PossibleRemoveNonCarFromWorld@<EAX>(br_actor *pActor@<EAX>)
int PossibleRemoveNonCarFromWorld(br_actor* pActor) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PutNonCarBackInWorld(br_actor *pActor@<EAX>)
void PutNonCarBackInWorld(br_actor* pActor) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall IncidentCam@<EAX>(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
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
    static tU32 next_incident_time;
    static tIncident_type type;
    static float severity;
    static tIncident_info info;
    static int random;
    static int count;
    br_scalar temp;
    br_vector3 old_cam_pos;
    int removed;
    LOG_TRACE("(%p, %d)", c, pTime);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall MoveCamToIncident@<EAX>(tCar_spec *c@<EAX>, tIncident_type *type@<EDX>, float *severity@<EBX>, tIncident_info *info@<ECX>, tU32 *next_incident_time)
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
    LOG_TRACE("(%p, %p, %p, %p, %p)", c, type, severity, info, next_incident_time);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PanningExternalCamera(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
void PanningExternalCamera(tCar_spec* c, tU32 pTime) {
    br_matrix34* m2;
    br_matrix34* m1;
    br_vector3 tv;
    br_scalar ts;
    static int inside_camera_zone;
    LOG_TRACE("(%p, %d)", c, pTime);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CheckForWall@<EAX>(br_vector3 *start@<EAX>, br_vector3 *end@<EDX>)
int CheckForWall(br_vector3* start, br_vector3* end) {
    br_vector3 dir;
    br_material* material;
    br_vector3 normal;
    br_scalar d;
    LOG_TRACE("(%p, %p)", start, end);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetUpPanningCamera(tCar_spec *c@<EAX>)
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
    LOG_TRACE("(%p)", c);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SaveCameraPosition(int i@<EAX>)
void SaveCameraPosition(int i) {
    LOG_TRACE("(%d)", i);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RestoreCameraPosition(int i@<EAX>)
void RestoreCameraPosition(int i) {
    LOG_TRACE("(%d)", i);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NormalPositionExternalCamera(tCar_spec *c@<EAX>, tU32 pTime@<EDX>)
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
    LOG_TRACE("(%p, %d)", c, pTime);

    m1 = &gCamera->t.t.mat;
    m2 = &c->car_master_actor->t.t.mat;
    swoop = gCountdown && c->pos.v[1] + 0.001 < gCamera_height;
    manual_swing = gOld_yaw__car != gCamera_yaw || swoop;
    manual_zoom = (double)gOld_zoom != gCamera_zoom;
    old_camera_pos = *(br_vector3*)&m1->m[3][0];
    if (!gProgram_state.cockpit_on) {
        if (swoop) {
            gCamera_yaw = 0;
            manual_swing = 1;
        }
        if (fabs(c->speedo_speed) > 0.0005999999999999999 && gCamera_mode > 0) {
            gCamera_mode = -1;
            gCamera_sign = m2->m[2][1] * c->direction.v[1]
                    + m2->m[2][2] * c->direction.v[2]
                    + m2->m[2][0] * c->direction.v[0]
                > 0.0;
        }
        if (c->frame_collision_flag && gCamera_mode != -2) {
            gCamera_mode = 1;
        }
        if (gCar_flying || gCamera_reset || gCamera_mode == -2) {
            gCamera_mode = 0;
        }
        d = sqrt(gCamera_zoom) + 0.57971013;
        if (!gCamera_mode || gCamera_mode == -1) {
            vn = c->direction;
            MoveWithWheels(c, &vn, manual_swing);
            vn.v[1] = 0.0;
            scale = sqrt(vn.v[2] * vn.v[2] + 0.0 * 0.0 + vn.v[0] * vn.v[0]);
            if (scale <= 2.3841858e-7) {
                vn.v[0] = 1.0;
                vn.v[1] = 0.0;
                vn.v[2] = 0.0;
            } else {
                scale = 1.0 / scale;
                vn.v[0] = vn.v[0] * scale;
                vn.v[1] = vn.v[1] * scale;
                vn.v[2] = vn.v[2] * scale;
            }
            if (gCar_flying) {
                gCamera_sign = 0;
            }
            SwingCamera(c, m2, m1, &vn, pTime);
            a.v[0] = vn.v[0] * d;
            a.v[1] = vn.v[1] * d;
            a.v[2] = vn.v[2] * d;
            m1->m[3][0] = c->pos.v[0] - a.v[0];
            m1->m[3][1] = c->pos.v[1] - a.v[1];
            m1->m[3][2] = c->pos.v[2] - a.v[2];
            gView_direction = vn;
        }
        if (gCamera_mode == 1) {
            if (manual_swing || manual_zoom) {
                old_camera_pos = gCamera_pos_before_collide;
            }
            a.v[0] = c->pos.v[0] - old_camera_pos.v[0];
            a.v[1] = c->pos.v[1] - old_camera_pos.v[1];
            a.v[2] = c->pos.v[2] - old_camera_pos.v[2];
            a.v[1] = 0.0;
            if (manual_swing) {
                DrVector3RotateY(&a, (gCamera_sign == 0 ? 1 : -1) * (gCamera_yaw - gOld_yaw__car));
                gCamera_yaw = gOld_yaw__car;
            }
            scale = sqrt(a.v[1] * a.v[1] + a.v[2] * a.v[2] + a.v[0] * a.v[0]);
            if (scale <= 2.3841858e-7) {
                vn.v[0] = 1.0;
                vn.v[1] = 0.0;
                vn.v[2] = 0.0;
            } else {
                scale = 1.0 / scale;
                vn.v[0] = a.v[0] * scale;
                vn.v[1] = a.v[1] * scale;
                vn.v[2] = a.v[2] * scale;
            }
            gView_direction = vn;
            vn.v[0] = -d * vn.v[0];
            vn.v[1] = vn.v[1] * -d;
            vn.v[2] = vn.v[2] * -d;
            a.v[0] = a.v[0] + vn.v[0];
            a.v[1] = a.v[1] + vn.v[1];
            a.v[2] = a.v[2] + vn.v[2];
            dist = sqrt(a.v[1] * a.v[1] + a.v[2] * a.v[2] + a.v[0] * a.v[0]);
            l = (double)pTime / 1000.0 * (dist + 1.0) / dist;
            if (l < 1.0f && a.v[2] * vn.v[2] + a.v[1] * vn.v[1] + a.v[0] * vn.v[0] > 0.0) {
                a.v[0] = (l - 1.0) * a.v[0];
                a.v[1] = (l - 1.0) * a.v[1];
                a.v[2] = (l - 1.0) * a.v[2];
                vn.v[0] = a.v[0] + vn.v[0];
                vn.v[1] = a.v[1] + vn.v[1];
                vn.v[2] = a.v[2] + vn.v[2];
            }
            m1->m[3][0] = c->pos.v[0] + vn.v[0];
            m1->m[3][1] = c->pos.v[1] + vn.v[1];
            m1->m[3][2] = c->pos.v[2] + vn.v[2];
        }
        height_inc = gCamera_zoom * gCamera_zoom + 0.30000001;
        time = (double)pTime * 0.001;
        if (!gCamera_frozen || gAction_replay_mode) {
            if (pTime < 5000) {
                if (swoop) {
                    if (time > 0.2) {
                        time = 0.2;
                    }
                    gCamera_height -= time * 5.0;
                    if (gCamera_height < c->pos.v[1]) {
                        gCamera_height = c->pos.v[1];
                    }
                } else {
                    gCamera_height = time * 5.0 * c->pos.v[1] + gCamera_height;
                    gCamera_height = gCamera_height / (time * 5.0 + 1.0);
                }
            } else {
                gCamera_height = c->pos.v[1];
            }
        }
        l = c->direction.v[1] * d;
        if (l > 0) {
            if (c->pos.v[1] - l - height_inc / 2.0 > gCamera_height) {
                gCamera_height = c->pos.v[1] - l - height_inc / 2.0;
            }
        }

        m1->m[3][1] = height_inc + gCamera_height;
        gCamera_pos_before_collide = *(br_vector3*)&m1->m[3][0];
        CollideCameraWithOtherCars(&c->pos, (br_vector3*)m1->m[3]);
        if (manual_swing || manual_zoom) {
            CollideCamera2(&c->pos, (br_vector3*)m1->m[3], &old_camera_pos, 1);
        } else {
            CollideCamera2(&c->pos, (br_vector3*)m1->m[3], &old_camera_pos, 0);
        }
        if (gCamera_has_collided && swoop) {
            gCamera_height = c->pos.v[1];
        }
        PointCameraAtCar(c, m2, m1);
    }
    gOld_yaw__car = gCamera_yaw;
    gOld_zoom = (br_angle)gCamera_zoom;
}

// IDA: void __usercall MoveWithWheels(tCar_spec *c@<EAX>, br_vector3 *vn@<EDX>, int manual_swing@<EBX>)
void MoveWithWheels(tCar_spec* c, br_vector3* vn, int manual_swing) {
    br_angle yaw;
    br_angle theta;
    static int move_with_wheels;
    LOG_TRACE("(%p, %p, %d)", c, vn, manual_swing);

    if (c->speed <= 0.000099999997 && !gCamera_mode) {
        if (manual_swing) {
            if (gCamera_yaw <= 32760u) {
                yaw = gCamera_yaw;
            } else {
                yaw = gCamera_yaw - 32760;
            }
            if (yaw <= 8190u || yaw >= 24570u) {
                if (!move_with_wheels) {
                    theta = BrRadianToAngle(atan2(c->wpos[0].v[2] * c->curvature, 1.0));
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
    static br_angle omega = 0;
    static int elapsed_time = -1;
    static br_vector3 old_vn;
    LOG_TRACE("(%p, %p, %p, %p, %d)", c, m1, m2, vn, pTime);

    manual_swing = gOld_yaw__car != gCamera_yaw;
    if (elapsed_time > 500) {
        elapsed_time = -1;
    }
    if (elapsed_time >= 0) {
        elapsed_time += pTime;
    }
    sign = -(m1->m[2][0] * vn->v[0] + m1->m[2][1] * vn->v[1] + m1->m[2][2] * vn->v[2]);
    ts = vn->v[1] * old_vn.v[1] + vn->v[2] * old_vn.v[2] + vn->v[0] * old_vn.v[0];

    old_vn = *vn;
    if ((sign < 0.0) == gCamera_sign) {
        elapsed_time = -1;
    } else if (ts <= 0.0 || elapsed_time >= 0) {
        if (elapsed_time < 0) {
            elapsed_time = 0;
        }
        if (elapsed_time < 500 && sign <= 0.0) {
            vn->v[0] = -vn->v[0];
            vn->v[1] = -vn->v[1];
            vn->v[2] = -vn->v[2];
        } else {
            elapsed_time = 500;
            if (sign <= 0.0) {
                ts = 0.0005999999999999999;
            } else {
                ts = 0.0001;
            }
            if (fabs(c->speedo_speed) <= ts || gCar_flying) {
                vn->v[0] = -vn->v[0];
                vn->v[1] = -vn->v[1];
                vn->v[2] = -vn->v[2];
            } else {
                gCamera_sign = gCamera_sign == 0;
                omega = BrDegreeToAngle(pTime * 0.03);
                if (gCamera_yaw <= 32760) {
                    yaw = gCamera_yaw;
                } else {
                    yaw = gCamera_yaw - 32760;
                }
                if ((uint16_t)(gCamera_yaw + 16380) <= 32760) {
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
    br_scalar v16 = vn->v[0] * gView_direction.v[2] - vn->v[2] * gView_direction.v[0];
    br_scalar v17 = vn->v[0] * gView_direction.v[0] + vn->v[2] * gView_direction.v[2];

    br_angle v8 = BrRadianToAngle(sqrt(c->omega.v[2] * c->omega.v[2] + c->omega.v[0] * c->omega.v[0] + c->omega.v[1] * c->omega.v[1]) * pTime / 1000.0);
    sin_dtheta = sin(BrAngleToRadian(v8)) + 0.1;

    if (omega || gCamera_reset || (c->speed < 0.000099999997 && !manual_swing) || gCamera_mode == -1 || (v17 > 0.0 && !manual_swing && fabs(v16) > sin_dtheta)) {
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
                omega += BrDegreeToAngle(pTime * 0.03);
                if (BrDegreeToAngle(pTime * 0.1) < omega) {
                    omega = BrDegreeToAngle(pTime * 0.1);
                }
                if (omega < theta) {
                    omega = theta;
                }
            }
        }
    }
}

// IDA: void __usercall PointCameraAtCar(tCar_spec *c@<EAX>, br_matrix34 *m1@<EDX>, br_matrix34 *m2@<EBX>)
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
    LOG_TRACE("(%p, %p, %p)", c, m1, m2);

    camera_ptr = (br_camera*)gCamera->type_data;
    theta = camera_ptr->field_of_view / 5;
    swoop = gCountdown && c->pos.v[1] + 0.0099999998 < gCamera_height;
    if (swoop) {
        tv.v[0] = gAverage_grid_position.v[0] - c->pos.v[0];
        tv.v[1] = gAverage_grid_position.v[1] - c->pos.v[1];
        tv.v[2] = gAverage_grid_position.v[2] - c->pos.v[2];
        frac = (gCamera_height - c->pos.v[1]) / 10.0;
        tv.v[0] = tv.v[0] * frac;
        tv.v[1] = tv.v[1] * frac;
        tv.v[2] = tv.v[2] * frac;
        tv.v[0] = c->pos.v[0] + tv.v[0];
        tv.v[1] = c->pos.v[1] + tv.v[1];
        tv.v[2] = c->pos.v[2] + tv.v[2];
        pos = &tv;
        theta = (1.0 - frac) * (double)theta;
    } else {
        pos = &c->pos;
    }
    vn.v[0] = c->pos.v[0] - m2->m[3][0];
    vn.v[2] = c->pos.v[2] - m2->m[3][2];
    scale = sqrt(vn.v[2] * vn.v[2] + 0.0 * 0.0 + vn.v[0] * vn.v[0]);
    if (scale <= 2.3841858e-7) {
        vn.v[0] = 1.0;
        vn.v[1] = 0.0;
        vn.v[2] = 0.0;
    } else {
        scale = 1.0 / scale;
        vn.v[0] *= scale;
        vn.v[1] = 0.0;
        vn.v[2] *= scale;
    }
    m2->m[0][0] = -vn.v[2];
    m2->m[0][1] = 0.0;
    m2->m[0][2] = vn.v[0];
    m2->m[1][0] = 0.0;
    m2->m[1][1] = 1.0;
    m2->m[1][2] = 0.0;
    m2->m[2][0] = -vn.v[0];
    m2->m[2][1] = 0.0;
    m2->m[2][2] = -vn.v[2];
    tv2.v[0] = pos->v[0] - m2->m[3][0];
    tv2.v[1] = pos->v[1] - m2->m[3][1];
    tv2.v[2] = pos->v[2] - m2->m[3][2];
    dist = tv2.v[2] * vn.v[2] + tv2.v[1] * vn.v[1] + tv2.v[0] * vn.v[0];
    BrMatrix34PreRotateX(m2, theta - BrRadianToAngle(atan2(m2->m[3][1] - pos->v[1], dist)));
}

// IDA: void __usercall PointCamera(br_vector3 *pos@<EAX>, br_matrix34 *m2@<EDX>)
void PointCamera(br_vector3* pos, br_matrix34* m2) {
    br_vector3 vn;
    br_scalar dist;
    br_angle theta;
    br_camera* camera_ptr;
    LOG_TRACE("(%p, %p)", pos, m2);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CollideCamera2@<EAX>(br_vector3 *car_pos@<EAX>, br_vector3 *cam_pos@<EDX>, br_vector3 *old_camera_pos@<EBX>, int manual_move@<ECX>)
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
    LOG_TRACE("(%p, %p, %p, %d)", car_pos, cam_pos, old_camera_pos, manual_move);

    hither = ((br_camera*)gCamera->type_data)->hither_z * 3.0;
    gCamera_has_collided = 0;
    for (i = 0; i < 1; i++) {
        tv.v[0] = cam_pos->v[0] - car_pos->v[0];
        tv.v[1] = cam_pos->v[1] - car_pos->v[1];
        tv.v[2] = cam_pos->v[2] - car_pos->v[2];
        dist = sqrt(tv.v[1] * tv.v[1] + tv.v[2] * tv.v[2] + tv.v[0] * tv.v[0]);
        tv.v[0] = tv.v[0] * 1.2;
        tv.v[1] = tv.v[1] * 1.2;
        tv.v[2] = tv.v[2] * 1.2;
        FindFace(car_pos, &tv, &a, &ts, &material);
        if (ts <= 1.0) {
            gCamera_has_collided = 1;
            if (a.v[1] * tv.v[1] + a.v[2] * tv.v[2] + a.v[0] * tv.v[0] > 0.0) {
                a.v[0] = -a.v[0];
                a.v[1] = -a.v[1];
                a.v[2] = -a.v[2];
            }
            if (gCamera_mode == 1 && !manual_move) {
                tv2.v[0] = car_pos->v[0] - old_camera_pos->v[0];
                tv2.v[1] = car_pos->v[1] - old_camera_pos->v[1];
                tv2.v[2] = car_pos->v[2] - old_camera_pos->v[2];
                FindFace(old_camera_pos, &tv2, &b, &ts2, &material);
                if (ts2 > 1.0) {
                    *cam_pos = *old_camera_pos;
                    return i;
                }
            }
            tv.v[0] = tv.v[0] * ts;
            tv.v[1] = tv.v[1] * ts;
            tv.v[2] = tv.v[2] * ts;
            tv2.v[0] = a.v[0] * hither;
            tv2.v[1] = a.v[1] * hither;
            tv2.v[2] = a.v[2] * hither;
            tv.v[0] = tv2.v[0] + tv.v[0];
            tv.v[1] = tv2.v[1] + tv.v[1];
            tv.v[2] = tv2.v[2] + tv.v[2];
            dist = sqrt(tv.v[1] * tv.v[1] + tv.v[2] * tv.v[2] + tv.v[0] * tv.v[0]);
            cam_pos->v[0] = car_pos->v[0] + tv.v[0];
            cam_pos->v[1] = car_pos->v[1] + tv.v[1];
            cam_pos->v[2] = car_pos->v[2] + tv.v[2];
            if (gMin_camera_car_distance > dist && !i && a.v[1] > -0.7) {
                tv2.v[0] = -a.v[1] * a.v[0];
                tv2.v[1] = -a.v[1] * a.v[1];
                tv2.v[2] = -a.v[1] * a.v[2];
                tv2.v[1] = tv2.v[1] + 1.0;
                if (gProgram_state.current_car.car_master_actor->t.t.mat.m[1][1] < 0.0) {
                    tv2.v[0] = -tv2.v[0];
                    tv2.v[1] = -tv2.v[1];
                    tv2.v[2] = -tv2.v[2];
                }
                d = tv2.v[1] * tv2.v[1] + tv2.v[2] * tv2.v[2] + tv2.v[0] * tv2.v[0];
                l = tv2.v[1] * tv.v[1] + tv2.v[2] * tv.v[2] + tv2.v[0] * tv.v[0];
                alpha = tv.v[1] * tv.v[1]
                    + tv.v[2] * tv.v[2]
                    + tv.v[0] * tv.v[0]
                    - gMin_camera_car_distance * gMin_camera_car_distance;
                ts2 = l * l - alpha * d * 4.0;
                if (alpha >= 0 && d != 0.0) {
                    sa = (sqrt(ts2) - l) / (d * 2.0);
                    tv2.v[0] = tv2.v[0] * sa;
                    tv2.v[1] = tv2.v[1] * sa;
                    tv2.v[2] = tv2.v[2] * sa;
                    FindFace(cam_pos, &tv2, &a, &ts, &material);
                    if (ts < 1.0) {
                        tv2.v[0] = tv2.v[0] * ts;
                        tv2.v[1] = tv2.v[1] * ts;
                        tv2.v[2] = tv2.v[2] * ts;
                    }
                    b.v[0] = tv.v[0];
                    b.v[1] = 0.0;
                    b.v[2] = tv.v[2];
                    dist = sqrt(0.0 * 0.0 + tv.v[2] * tv.v[2] + tv.v[0] * tv.v[0]);
                    if (alpha <= 2.3841858e-7) {
                        b.v[0] = 1.0;
                        b.v[1] = 0.0;
                        b.v[2] = 0.0;
                    } else {
                        alpha = 1.0 / dist;
                        b.v[0] = b.v[0] * alpha;
                        b.v[1] = b.v[1] * alpha;
                        b.v[2] = b.v[2] * alpha;
                    }
                    tv.v[0] = tv2.v[0] + tv.v[0];
                    tv.v[1] = tv2.v[1] + tv.v[1];
                    tv.v[2] = tv2.v[2] + tv.v[2];
                    ts2 = tv.v[1] * b.v[1] + tv.v[2] * b.v[2] + b.v[0] * tv.v[0];
                    if (ts2 < 0.029999999 && !gAction_replay_mode) {
                        dist = sqrt(tv2.v[1] * tv2.v[1] + tv2.v[2] * tv2.v[2] + tv2.v[0] * tv2.v[0]);
                        if (dist <= 2.3841858e-7) {
                            tv2.v[0] = 1.0;
                            tv2.v[1] = 0.0;
                            tv2.v[2] = 0.0;
                        } else {
                            alpha = 1.0 / dist;
                            tv2.v[0] = tv2.v[0] * alpha;
                            tv2.v[1] = tv2.v[1] * alpha;
                            tv2.v[2] = tv2.v[2] * alpha;
                        }
                        if (tv2.v[2] * b.v[2] + tv2.v[0] * b.v[0] + tv2.v[1] * b.v[1] < -0.029999999)
                            alpha = tv2.v[1] * b.v[1] + tv2.v[2] * b.v[2] + b.v[0] * tv2.v[0];
                        alpha = (0.029999999 - ts2) / alpha;
                        tv2.v[0] = tv2.v[0] * alpha;
                        tv2.v[1] = tv2.v[1] * alpha;
                        tv2.v[2] = tv2.v[2] * alpha;
                        tv.v[0] = tv2.v[0] + tv.v[0];
                        tv.v[1] = tv2.v[1] + tv.v[1];
                        tv.v[2] = tv2.v[2] + tv.v[2];
                    }
                }
            }
            cam_pos->v[0] = car_pos->v[0] + tv.v[0];
            cam_pos->v[1] = car_pos->v[1] + tv.v[1];
            cam_pos->v[2] = car_pos->v[2] + tv.v[2];
        }

        bnds.mat = &mat;
        BrMatrix34Identity(&mat);
        tv2.v[0] = hither;
        tv2.v[1] = hither;
        tv2.v[2] = hither;
        bnds.original_bounds.min.v[0] = cam_pos->v[0] - hither;
        bnds.original_bounds.min.v[1] = cam_pos->v[1] - hither;
        bnds.original_bounds.min.v[2] = cam_pos->v[2] - hither;
        bnds.original_bounds.max.v[0] = cam_pos->v[0] + hither;
        bnds.original_bounds.max.v[1] = cam_pos->v[1] + hither;
        bnds.original_bounds.max.v[2] = cam_pos->v[2] + hither;
        k = FindFacesInBox(&bnds, face_list, 3);
        if (k > 0) {
            tv2.v[0] = cam_pos->v[0] - face_list[0].v[0].v[0];
            tv2.v[1] = cam_pos->v[1] - face_list[0].v[0].v[1];
            tv2.v[2] = cam_pos->v[2] - face_list[0].v[0].v[2];
            sa = face_list[0].normal.v[2] * tv2.v[2]
                + face_list[0].normal.v[1] * tv2.v[1]
                + face_list[0].normal.v[0] * tv2.v[0];
            //ts2 = sa;
            if (sa < hither && sa >= 0.0) {
                tv2.v[0] = (hither - sa) * face_list[0].normal.v[0];
                tv2.v[1] = (hither - sa) * face_list[0].normal.v[1];
                tv2.v[2] = (hither - sa) * face_list[0].normal.v[2];
                cam_pos->v[0] = cam_pos->v[0] + tv2.v[0];
                cam_pos->v[1] = cam_pos->v[1] + tv2.v[1];
                cam_pos->v[2] = cam_pos->v[2] + tv2.v[2];
            }
            if (k > 1) {
                sb = face_list[1].normal.v[2] * face_list[0].normal.v[2]
                    + face_list[1].normal.v[1] * face_list[0].normal.v[1]
                    + face_list[1].normal.v[0] * face_list[0].normal.v[0];
                if (sb > 0.94999999 && k > 2) {
                    face_list[1].normal.v[0] = face_list[2].normal.v[0];
                    face_list[1].normal.v[1] = face_list[2].normal.v[1];
                    face_list[1].normal.v[2] = face_list[2].normal.v[2];
                    face_list[1].v[0].v[0] = face_list[2].v[0].v[0];
                    face_list[1].v[0].v[1] = face_list[2].v[0].v[1];
                    face_list[1].v[0].v[2] = face_list[2].v[0].v[2];
                    sb = face_list[2].normal.v[2] * face_list[0].normal.v[2]
                        + face_list[2].normal.v[1] * face_list[0].normal.v[1]
                        + face_list[2].normal.v[0] * face_list[0].normal.v[0];
                    k = 2;
                }
                if (sb <= 0.94999999) {
                    tv2.v[0] = cam_pos->v[0] - face_list[1].v[0].v[0];
                    tv2.v[1] = cam_pos->v[1] - face_list[1].v[0].v[1];
                    tv2.v[2] = cam_pos->v[2] - face_list[1].v[0].v[2];
                    sc = face_list[1].normal.v[2] * tv2.v[2]
                        + face_list[1].normal.v[1] * tv2.v[1]
                        + face_list[1].normal.v[0] * tv2.v[0];
                    if (sc < hither && sc >= 0.0) {
                        sc = face_list[1].normal.v[2] * face_list[0].normal.v[2]
                            + face_list[1].normal.v[1] * face_list[0].normal.v[1]
                            + face_list[1].normal.v[0] * face_list[0].normal.v[0];
                        b.v[0] = face_list[0].normal.v[0] * sc;
                        b.v[1] = face_list[0].normal.v[1] * sc;
                        b.v[2] = face_list[0].normal.v[2] * sc;
                        face_list[1].normal.v[0] = face_list[1].normal.v[0] - b.v[0];
                        face_list[1].normal.v[1] = face_list[1].normal.v[1] - b.v[1];
                        face_list[1].normal.v[2] = face_list[1].normal.v[2] - b.v[2];
                        tv2.v[0] = (hither - ts2) * face_list[1].normal.v[0];
                        tv2.v[1] = (hither - ts2) * face_list[1].normal.v[1];
                        tv2.v[2] = (hither - ts2) * face_list[1].normal.v[2];
                        cam_pos->v[0] = cam_pos->v[0] + tv2.v[0];
                        cam_pos->v[1] = cam_pos->v[1] + tv2.v[1];
                        cam_pos->v[2] = cam_pos->v[2] + tv2.v[2];
                    }
                }
            }
        }
        i += k;
    }
    return i;
}

// IDA: int __usercall BoundsTest@<EAX>(br_bounds *bnds@<EAX>, br_vector3 *p@<EDX>)
int BoundsTest(br_bounds* bnds, br_vector3* p) {
    int j;
    LOG_TRACE("(%p, %p)", bnds, p);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CollideCameraWithOtherCars@<EAX>(br_vector3 *car_pos@<EAX>, br_vector3 *cam_pos@<EDX>)
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
    LOG_TRACE("(%p, %p)", car_pos, cam_pos);

    SILENT_STUB();
    return 0;
}

// IDA: void __cdecl InitialiseExternalCamera()
void InitialiseExternalCamera() {
    br_scalar ts;
    tCar_spec* c;
    br_vector3 r;
    br_angle yaw;
    LOG_TRACE("()");

    c = gCar_to_view;
    if (!gProgram_state.racing) {
        c = &gProgram_state.current_car;
    }
    gCamera_height = c->pos.v[1];
    gView_direction.v[0] = c->direction.v[0];
    gView_direction.v[1] = 0.0;
    gView_direction.v[2] = c->direction.v[2];
    ts = sqrt(gView_direction.v[0] * gView_direction.v[0] + gView_direction.v[2] * gView_direction.v[2]);
    if (ts <= 2.3841858e-7) {
        gView_direction.v[0] = 1.0;
        gView_direction.v[1] = 0.0;
        gView_direction.v[2] = 0.0;
    } else {
        gView_direction.v[0] *= (1.0 / ts);
        gView_direction.v[1] *= (1.0 / ts);
        gView_direction.v[2] *= (1.0 / ts);
    }
    ts = -(gView_direction.v[1] * c->car_master_actor->t.t.mat.m[2][1]
        + gView_direction.v[0] * c->car_master_actor->t.t.mat.m[2][0]
        + gView_direction.v[2] * c->car_master_actor->t.t.mat.m[2][2]);
    gCamera_sign = ts < 0;
    gCamera_mode = 0;
    if (ts >= 0.0) {
        yaw = -gCamera_yaw;
    } else {
        yaw = gCamera_yaw;
    }
    DrVector3RotateY(&gView_direction, yaw);
    gMin_camera_car_distance = 0.60000002;
    gCamera_frozen = 0;
    gCamera_mode = -2;
    if (gCountdown && (!gNet_mode || gCurrent_net_game->options.grid_start) && gCountdown > 4) {
        gCamera_height = gCamera_height + 10.0;
    }
    LOG_MATRIX("c->car_master_actor->t.t.mat", &c->car_master_actor->t.t.mat);
    LOG_VEC("gView_direction", &gView_direction);
}

// IDA: void __cdecl FreezeCamera()
void FreezeCamera() {
    LOG_TRACE("()");

    gCamera_frozen = 1;
}

// IDA: void __usercall FlyCar(tCar_spec *c@<EAX>, br_scalar dt)
void FlyCar(tCar_spec* c, br_scalar dt) {
    int accflag;
    int turnflag;
    br_vector3 step;
    br_matrix34* mat;
    br_angle theta;
    static br_scalar vel;
    tFace_ref faces[20];
    tBounds bnds;
    LOG_TRACE("(%p, %f)", c, dt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrVector3RotateY(br_vector3 *v@<EAX>, br_angle t@<EDX>)
void DrVector3RotateY(br_vector3* v, br_angle t) {
    br_scalar c;
    br_scalar s;
    br_scalar ts;
    LOG_TRACE("(%p, %d)", v, t);

    c = cos(BrAngleToRadian(t));
    s = sin(BrAngleToRadian(t));
    ts = v->v[0] * c + v->v[2] * s;
    v->v[2] = v->v[2] * c - v->v[0] * s;
    v->v[0] = ts;
}

// IDA: void __cdecl CrashCarsTogether(br_scalar dt)
void CrashCarsTogether(br_scalar dt) {
    int pass;
    int k;
    int i;
    tCollison_data collide_list[32];
    LOG_TRACE("(%f)", dt);

    STUB();
}

// IDA: int __cdecl CrashCarsTogetherSinglePass(br_scalar dt, int pPass, tCollison_data *collide_list)
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
    LOG_TRACE("(%f, %d, %p)", dt, pPass, collide_list);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BringCarToAGrindingHalt(tCollision_info *car@<EAX>)
void BringCarToAGrindingHalt(tCollision_info* car) {
    LOG_TRACE("(%p)", car);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall BoundsOverlapTest@<EAX>(br_bounds *b1@<EAX>, br_bounds *b2@<EDX>)
int BoundsOverlapTest_car(br_bounds* b1, br_bounds* b2) {
    LOG_TRACE("(%p, %p)", b1, b2);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SimpleCarCarCollisionTest@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>)
int SimpleCarCarCollisionTest(tCollision_info* car1, tCollision_info* car2) {
    LOG_TRACE("(%p, %p)", car1, car2);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CollideTwoCarsWithWalls@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_scalar dt)
int CollideTwoCarsWithWalls(tCollision_info* car1, tCollision_info* car2, br_scalar dt) {
    br_vector3 mom1;
    br_vector3 mom2;
    int l;
    int m;
    int n;
    int p;
    int im1;
    int im2;
    LOG_TRACE("(%p, %p, %f)", car1, car2, dt);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CollideTwoCarsRepeatedly@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_scalar dt)
int CollideTwoCarsRepeatedly(tCollision_info* car1, tCollision_info* car2, br_scalar dt) {
    int l;
    int collide;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %f)", car1, car2, dt);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CollideTwoCars@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, int pPass@<EBX>)
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
    static br_vector3 oldr1;
    static br_vector3 oldr2;
    static br_vector3 oldn1;
    static br_vector3 oldn2;
    static int is_old_point_available;
    LOG_TRACE("(%p, %p, %d)", car1, car2, pPass);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetEdgeEdgeCollisions@<EAX>(br_bounds *pB1@<EAX>, br_bounds *pB2@<EDX>, br_matrix34 *pM21@<EBX>, br_matrix34 *pM12@<ECX>, br_matrix34 *pMo21, br_matrix34 *pMo12, br_matrix34 *pM1o1, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int pMax)
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
    LOG_TRACE("(%p, %p, %p, %p, %p, %p, %p, %p, %p, %d)", pB1, pB2, pM21, pM12, pMo21, pMo12, pM1o1, pPoint_list, pNorm_list, pMax);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FacePointCarCarCollide@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_matrix34 *pMms@<EBX>, br_matrix34 *pMoms@<ECX>, br_matrix34 *pMsos, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int pMax, int order)
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
    LOG_TRACE("(%p, %p, %p, %p, %p, %p, %p, %d, %d)", car1, car2, pMms, pMoms, pMsos, pPoint_list, pNorm_list, pMax, order);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MungeCarsMass(tCollision_info *pCar@<EAX>, br_scalar pFactor)
void MungeCarsMass(tCollision_info* pCar, br_scalar pFactor) {
    LOG_TRACE("(%p, %f)", pCar, pFactor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ModifyCarsMass(tCollision_info *pCar_1@<EAX>, tCollision_info *pCar_2@<EDX>)
void ModifyCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2) {
    LOG_TRACE("(%p, %p)", pCar_1, pCar_2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetCarsMass(tCollision_info *pCar_1@<EAX>, tCollision_info *pCar_2@<EDX>)
void ResetCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2) {
    LOG_TRACE("(%p, %p)", pCar_1, pCar_2);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DoCollide@<EAX>(tCollision_info *car1@<EAX>, tCollision_info *car2@<EDX>, br_vector3 *r@<EBX>, br_vector3 *n@<ECX>, int k, int pPass, br_matrix34 *mat1_to_mat2)
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
    LOG_TRACE("(%p, %p, %p, %p, %d, %d, %p)", car1, car2, r, n, k, pPass, mat1_to_mat2);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall TwoPointCollB@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n)
br_scalar TwoPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %p, %p)", f, m, d, tau, n);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall ThreePointCollRecB@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n)
br_scalar ThreePointCollRecB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    int i;
    int j;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %p, %p)", f, m, d, tau, n);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FourPointCollB@<ST0>(br_scalar *f@<EAX>, br_matrix4 *m@<EDX>, br_scalar *d@<EBX>, br_vector3 *tau@<ECX>, br_vector3 *n)
br_scalar FourPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    int i;
    int j;
    int l;
    br_scalar ts;
    LOG_TRACE("(%p, %p, %p, %p, %p)", f, m, d, tau, n);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall TestForNan@<EAX>(float *f@<EAX>)
int TestForNan(float* f) {
    tU32 i;
    LOG_TRACE("(%p)", f);
    i = *f;
    return (~i & 0x7F800000) == 0;
}

// IDA: void __cdecl CheckCameraHither()
void CheckCameraHither() {
    br_camera* cam;
    static int old_hither;
    LOG_TRACE("()");

    cam = (br_camera*)gCamera->type_data;
    if (TestForNan(&cam->hither_z)) {
        cam->hither_z = (float)old_hither;
    }
    old_hither = (int)cam->hither_z;
}

// IDA: void __usercall SetCarSuspGiveAndHeight(tCar_spec *pCar@<EAX>, br_scalar pFront_give_factor, br_scalar pRear_give_factor, br_scalar pDamping_factor, br_scalar pExtra_front_height, br_scalar pExtra_rear_height)
void SetCarSuspGiveAndHeight(tCar_spec* pCar, br_scalar pFront_give_factor, br_scalar pRear_give_factor, br_scalar pDamping_factor, br_scalar pExtra_front_height, br_scalar pExtra_rear_height) {
    br_scalar front_give;
    br_scalar rear_give;
    br_scalar damping;
    br_scalar ratio;
    int i;
    LOG_TRACE("(%p, %f, %f, %f, %f, %f)", pCar, pFront_give_factor, pRear_give_factor, pDamping_factor, pExtra_front_height, pExtra_rear_height);

    front_give = pCar->susp_give[1] * pFront_give_factor * 6.9000001;
    rear_give = pCar->susp_give[0] * pRear_give_factor * 6.9000001;
    damping = pCar->damping * pDamping_factor;
    ratio = fabs((pCar->wpos[0].v[2] - pCar->cmpos.v[2]) / (pCar->wpos[2].v[2] - pCar->cmpos.v[2]));
    pCar->sk[0] = pCar->M / (ratio + 1.0) * 5.0 / rear_give;
    pCar->sb[0] = pCar->M / (ratio + 1.0) * sqrt(5.0) / sqrt(rear_give);
    ratio = 1.0 / ratio;
    pCar->sk[1] = pCar->M / (ratio + 1.0) * 5.0 / front_give;
    pCar->sb[1] = pCar->M / (ratio + 1.0) * sqrt(5.0) / sqrt(front_give);
    pCar->sb[0] = pCar->sb[0] * damping;
    pCar->sb[1] = pCar->sb[1] * damping;
    pCar->susp_height[0] = pCar->ride_height + rear_give + pExtra_rear_height;
    pCar->susp_height[1] = pCar->ride_height + front_give + pExtra_front_height;

    if (rear_give >= front_give) {
        i = -rear_give;
    } else {
        i = -front_give;
    }
    if (pExtra_rear_height >= pExtra_front_height) {
        i -= pExtra_rear_height;
    } else {
        i -= pExtra_front_height;
    }
    pCar->bounds[0].min.v[1] = i;
    pCar->bounds[0].min.v[1] = pCar->bounds[0].min.v[1] / 6.9000001;
}

// IDA: int __usercall TestForCarInSensiblePlace@<EAX>(tCar_spec *car@<EAX>)
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
    LOG_TRACE("(%p)", car);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall TestOldMats@<EAX>(tCollision_info *c1@<EAX>, tCollision_info *c2@<EDX>, int newmats@<EBX>)
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
    LOG_TRACE("(%p, %p, %d)", c1, c2, newmats);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PullActorFromWorld@<EAX>(br_actor *pActor@<EAX>)
int PullActorFromWorld(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);

    if (gDoing_physics) {
        return DoPullActorFromWorld(pActor);
    }
    return 0;
}

// IDA: int __usercall DoPullActorFromWorld@<EAX>(br_actor *pActor@<EAX>)
int DoPullActorFromWorld(br_actor* pActor) {
    int num;
    int i;
    tCollision_info* c;
    tNon_car_spec* non_car;
    LOG_TRACE("(%p)", pActor);

    STUB();
    return 0;
}

// IDA: void __usercall CheckForDeAttachmentOfNonCars(tU32 pTime@<EAX>)
void CheckForDeAttachmentOfNonCars(tU32 pTime) {
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
    LOG_TRACE("(%d)", pTime);

    SILENT_STUB();
}

// IDA: void __usercall AdjustNonCar(br_actor *pActor@<EAX>, br_matrix34 *pMat@<EDX>)
void AdjustNonCar(br_actor* pActor, br_matrix34* pMat) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    LOG_TRACE("(%p, %p)", pActor, pMat);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleNonCar(tCollision_info *c@<EAX>)
void PipeSingleNonCar(tCollision_info* c) {
    LOG_TRACE("(%p)", c);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetPrecalculatedFacesUnderCar@<EAX>(tCar_spec *pCar@<EAX>, tFace_ref **pFace_refs@<EDX>)
int GetPrecalculatedFacesUnderCar(tCar_spec* pCar, tFace_ref** pFace_refs) {
    LOG_TRACE("(%p, %p)", pCar, pFace_refs);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __cdecl SomeNearbyMaterial()
br_material* SomeNearbyMaterial() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
