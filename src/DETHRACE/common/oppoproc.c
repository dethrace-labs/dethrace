#include "oppoproc.h"
#include "errors.h"
#include "globvars.h"
#include "harness/trace.h"
#include "opponent.h"
#include <brender/brender.h>
#include <math.h>
#include <stdlib.h>

// IDA: int __usercall StraightestArcForCorner2D@<EAX>(br_vector2 *pCent@<EAX>, br_scalar *pRadius@<EDX>, br_scalar *pEntry_length@<EBX>, int *pLeft_not_right@<ECX>, br_vector2 *p1, br_vector2 *p2, br_vector2 *p3, br_scalar pWidth12, br_scalar pWidth23)
int StraightestArcForCorner2D(br_vector2* pCent, br_scalar* pRadius, br_scalar* pEntry_length, int* pLeft_not_right, br_vector2* p1, br_vector2* p2, br_vector2* p3, br_scalar pWidth12, br_scalar pWidth23) {
    br_vector2 rel1;
    br_vector2 rel3;
    br_vector2 rot1;
    br_vector2 rot1u;
    br_scalar len12_squared;
    br_scalar len23_squared;
    br_scalar c;
    br_scalar numerator;
    br_scalar x;
    br_scalar __block0___scale;
    LOG_TRACE("(%p, %p, %p, %p, %p, %p, %p, %f, %f)", pCent, pRadius, pEntry_length, pLeft_not_right, p1, p2, p3, pWidth12, pWidth23);
    NOT_IMPLEMENTED();
}

// FIXME: is this an older implementation?
// FIXME: is this function correct?
// FIXME: later iterations probably convert br_vector3 to br_vector2
static void StraightestArcForCorner(float* p1, float* p2, float* p3, br_vector3* p4, br_vector3* p5, br_vector3* p6, br_vector3* p7, br_vector3* p8, float p9, float p10) {
    br_vector3 rel1;
    br_vector3 rel3;
    br_vector3 rot1;
    br_scalar tmp;
    br_scalar tmp2;
    br_scalar tmp3;
    br_scalar tmp4;
    LOG_TRACE("(%p, %p, %p, %p, %p, %p, %p, %p, %f, %f)", p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);

    BrVector3Sub(&rel1, p6, p5);
    BrVector3Sub(&rel3, p4, p5);
    *p3 = BrVector3Length(&rel3);
    if (*p3 <= BR_SCALAR_EPSILON) {
        *p2 = 0.001f;
        *p3 = 0.001f;
    }
    tmp = BrVector3Dot(&rel1, &rel3);
    BrVector3Cross(&rot1, &rel1, &rel3);
    tmp2 = sqrtf(tmp * tmp + rot1.v[1] * rot1.v[1]);
    tmp3 = fabsf(rot1.v[1] / tmp2);
    tmp4 = p10 * tmp / tmp2 + p9;
    if ((tmp3 < 1.f && fabsf(tmp4) > tmp3 * 1000.f) || tmp3 < 0.001f) {
        *p1 = 1000.f;
        *p2 = 1000.f;
    } else {
        tmp4 = tmp4 / tmp3;
        tmp3 = tmp3 / (tmp / tmp2 + 1.f);
        *p2 = tmp3 * p10 + sqrtf(tmp3 * p10) + tmp4;
        *p1 = *p2 * tmp3;
    }
}

// IDA: br_scalar __usercall CornerFudge@<ST0>(tCar_spec *pCar_spec@<EAX>)
br_scalar CornerFudge(tCar_spec* pCar_spec) {
    LOG_TRACE("(%p)", pCar_spec);

    return 1.4f;
}

// IDA: br_scalar __usercall MaxCurvatureForCarSpeed@<ST0>(tCar_spec *pCar@<EAX>, br_scalar pSpeed)
br_scalar MaxCurvatureForCarSpeed(tCar_spec* pCar, br_scalar pSpeed) {
    br_scalar curv;
    LOG_TRACE("(%p, %f)", pCar, pSpeed);

    if (pSpeed >= 12.5f) {
        curv = pCar->maxcurve * 12.5f / pSpeed;
    } else {
        curv = pCar->maxcurve;
    }
    return curv;
}

// IDA: br_scalar __usercall Vector2Cross@<ST0>(br_vector2 *pA@<EAX>, br_vector2 *pB@<EDX>)
br_scalar Vector2Cross(br_vector2* pA, br_vector2* pB) {
    LOG_TRACE("(%p, %p)", pA, pB);
    NOT_IMPLEMENTED();
}

// IDA: tFollow_path_result __usercall EndOfPath@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
tFollow_path_result EndOfPath(tOpponent_spec* pOpponent_spec) {
    tCar_spec* car_spec;
    LOG_TRACE("(%p)", pOpponent_spec);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall RoughlyColinear@<EAX>(br_vector2 *p1@<EAX>, br_vector2 *p2@<EDX>, br_vector2 *p3@<EBX>)
int RoughlyColinear(br_vector2* p1, br_vector2* p2, br_vector2* p3) {
    br_vector2 rel1;
    br_vector2 rel2;
    br_vector2 sum;
    br_scalar cross;
    br_scalar dot;
    br_scalar wibble;
    LOG_TRACE("(%p, %p, %p)", p1, p2, p3);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetStraight@<EAX>(br_vector2 *pStart@<EAX>, br_vector2 *pFinish@<EDX>, br_scalar *pWidth@<EBX>, int section1@<ECX>, tOpponent_spec *pOpponent_spec, tFollow_path_data *data)
int GetStraight(br_vector2* pStart, br_vector2* pFinish, br_scalar* pWidth, int section1, tOpponent_spec* pOpponent_spec, tFollow_path_data* data) {
    int section;
    br_vector2 next;
    br_scalar next_width;
    LOG_TRACE("(%p, %p, %p, %d, %p, %p)", pStart, pFinish, pWidth, section1, pOpponent_spec, data);
    NOT_IMPLEMENTED();
}

// IDA: tFollow_path_result __usercall ProcessFollowPath@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>, int pPursuit_mode@<EBX>, int pIgnore_end@<ECX>, int pNever_struggle)
tFollow_path_result ProcessFollowPath(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand, int pPursuit_mode, int pIgnore_end, int pNever_struggle) {
    tS16 real_section_no;
    tFollow_path_data* data;
    br_vector3 wank;
    br_vector3 wank2;
    br_vector3* not_our_dir;
    br_vector3 section_dir;
    br_vector3 section_v;
    br_vector3 start;
    br_vector3 corner;
    br_vector3 next;
    br_vector3 goal_dir;
    br_vector3 intersect;
    br_vector3 a;
    br_vector3 p;
    br_vector3 car_to_end;
    br_actor* car_master_actor;
    br_scalar stopped_speed;
    br_scalar dist_to_end;
    br_scalar t;
    br_scalar acc;
    br_scalar speed;
    br_scalar dist_to_goal;
    br_scalar section_width;
    br_scalar goal_width;
    br_scalar desired_speed;
    br_scalar dist_along;
    br_scalar acc_factor;
    br_scalar max_acc;
    br_scalar speed_to_goal;
    br_scalar error;
    br_scalar radius;
    br_scalar entry_len;
    br_scalar corner_speed2;
    br_scalar distance;
    br_scalar stopping_distance;
    br_scalar corner_speed;
    br_scalar dot_a;
    br_scalar dot_d;
    br_scalar further_along;
    tCar_spec* car_spec;
    int engine_damage;
    int trans_damage;
    int section_no;
    int sx;
    int just_fucking_brake;
    br_vector2 oppo_pos2d;
    br_vector2 start2d;
    br_vector2 finish2d;
    br_vector2 next2d;
    br_vector2 oppo_pos_rel;
    br_vector2 oppo_pos_rel_next;
    br_vector2 section_rel;
    br_vector2 section_rel_next;
    br_vector2 v2d;
    br_vector2 corner2d;
    br_vector2 after_corner2d;
    br_vector2 next_turning_cent;
    br_vector2 temp2d;
    br_scalar section_len;
    br_scalar section_len_next;
    br_scalar pos_error;
    br_scalar pos_error_next;
    br_scalar pos_error_factor;
    br_scalar sin_error;
    br_scalar corner_radius;
    br_scalar corner_entry_length;
    br_scalar corner_distance;
    br_scalar speed2d;
    br_scalar stemp1;
    br_scalar width;
    br_scalar next_width;
    br_scalar next_turning_radius;
    br_scalar next_corner_size;
    br_scalar later_width;
    br_scalar effective_speed_factor;
    int first_straight;
    int next_straight;
    int left_not_right;
    int later_straight;
    int next_left_not_right;
    LOG_TRACE("(%p, %d, %d, %d, %d)", pOpponent_spec, pCommand, pPursuit_mode, pIgnore_end, pNever_struggle);

    car_spec = pOpponent_spec->car_spec;
    engine_damage = car_spec->damage_units[0].damage_level;
    trans_damage = car_spec->damage_units[1].damage_level;
    data = &pOpponent_spec->follow_path_data;
    car_master_actor = car_spec->car_master_actor;
    if (pCommand == ePOC_start) {
        data->first_section_no = GetOpponentsFirstSection(pOpponent_spec);
        data->section_no = data->first_section_no;
        dr_dprintf("%s: ProcessFollowPath() - new task started, first real section #%d", pOpponent_spec->car_spec->driver_name, GetOpponentsRealSection(pOpponent_spec, data->first_section_no));
        data->has_moved_during_this_task = 0;
        data->struggle_time = 0;
        data->last_finished_struggle_time = gTime_stamp_for_this_munging;
        data->prev_acc = 0.f;
        data->prev_acc_error = 0.f;
        data->borrowed_time_start = gTime_stamp_for_this_munging;
        data->last_struggle_section = -1;
        data->made_it = 1;
        data->cheating = 0;
        data->cornering = 0;
        if (!pOpponent_spec->cheating && !pOpponent_spec->physics_me) {
            dr_dprintf("%s: Rematerialising from ePOC_start in ProcessFollowPath()...", pOpponent_spec->car_spec->driver_name);
            RematerialiseOpponentOnNearestSection(pOpponent_spec, BrVector3Length(&car_spec->v));
        }
        return eFPR_OK;
    } else if (pCommand == ePOC_run) {
        if (pOpponent_spec->follow_path_data.cheating || pOpponent_spec->cheating) {
            return FollowCheatyPath(pOpponent_spec);
        }
        if (!pIgnore_end && !data->made_it && data->borrowed_time_start + 1000 < gTime_stamp_for_this_munging && gTime_stamp_for_this_munging < data->borrowed_time_start + 10000) {
            BrVector3Sub(&section_dir, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            // actually car_to_start
            BrVector3Sub(&car_to_end, &car_master_actor->t.t.translate.t, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            width = Vector3Distance(&section_dir, &car_to_end);
            section_width = GetOpponentsSectionWidth(pOpponent_spec, data->section_no);
            if (width <= section_width) {
                data->made_it = 1;
            }
        }
        if (data->borrowed_time_start + 10000 < gTime_stamp_for_this_munging && !data->made_it) {
            dr_dprintf("%s: ProcessFollowPath() giving up due to not making the corner", car_spec->driver_name);
            return eFPR_given_up;
        }
        car_spec->keys.acc = 1;
        speed = BrVector3Length(&car_spec->v);
        if (speed > 0.2f) {
            data->has_moved_during_this_task = 1;
            pOpponent_spec->has_moved_at_some_point = 1;
        }
        if (data->struggle_time) {
            if (data->struggle_time + (data->number_of_struggles - 1) * 750 + 2750 >= gTime_stamp_for_this_munging) {
                if (data->struggle_time + (data->number_of_struggles - 1) * 750 + 2000 >= gTime_stamp_for_this_munging) {
                    car_spec->brake_force = 0.0;
                    car_spec->acc_force = car_spec->M * -6.0;
                } else {
                    car_spec->acc_force = 0.0;
                    car_spec->brake_force = car_spec->M * 15.0;
                }
                car_spec->curvature = 0.0;
                return 0;
            }
            dr_dprintf("%s: done struggling. speed = %.2f m/s", pOpponent_spec->car_spec->driver_name, speed);
            data->made_it = 0;
            data->borrowed_time_start = gTime_stamp_for_this_munging;
            data->struggle_time = 0;
            data->last_finished_struggle_time = gTime_stamp_for_this_munging;
            car_spec->brake_force = 0.0;
            car_spec->acc_force = 0.0;
        } else {
            if (pIgnore_end) {
                stopped_speed = 0.06666667f;
            } else {
                stopped_speed = 0.2f;
            }
            if (!pNever_struggle && stopped_speed >= speed && data->last_finished_struggle_time + 2000 < gTime_stamp_for_this_munging && (pPursuit_mode || data->has_moved_during_this_task)) {
                dr_dprintf("%s: 'Stopped,' section #%d, speed = %.2f m/s, about to start a-strugglin'", pOpponent_spec->car_spec->driver_name, data->section_no, speed);
                data->struggle_time = gTime_stamp_for_this_munging;
                if (pIgnore_end || data->section_no != data->last_struggle_section) {
                    data->last_struggle_section = data->section_no;
                    data->number_of_struggles = 0;
                } else {
                    if (data->number_of_struggles >= 3) {
                        car_spec->acc_force = 0.0f;
                        car_spec->brake_force = 0.0f;
                        dr_dprintf("%s: Giving up trying to follow path 'cos we've struggled too much", pOpponent_spec->car_spec->driver_name);
                        return eFPR_given_up;
                    }
                    data->number_of_struggles++;
                }
            }
        }

        BrVector3Sub(&car_to_end, &car_master_actor->t.t.translate.t, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no));
        car_to_end.v[1] = 0.f;
        dist_to_end = BrVector3Length(&car_to_end) * WORLD_SCALE;
        if (dist_to_end > 15.f) {
            BrVector3Sub(&section_dir, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            BrVector3Normalise(&section_dir, &section_dir);
            BrVector3Sub(&car_to_end, &car_master_actor->t.t.translate.t, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            pos_error_factor = BrVector3Dot(&section_dir, &car_to_end);
            BrVector3Scale(&wank, &section_dir, pos_error_factor);
            BrVector3Sub(&wank, &wank, &car_to_end);
            wank.v[1] = 0.f;
            pos_error = BrVector3Length(&wank) * WORLD_SCALE;
            if (pos_error < 15.f) {
                pos_error_next = sqrtf(15.f * 15.f - pos_error * pos_error) / WORLD_SCALE;
                if (pos_error_next + pos_error_factor >= 0.f) {
                    BrVector3Scale(&wank2, &section_dir, pos_error_next);
                    wank2.v[1] = 0.f;
                    BrVector3Accumulate(&wank, &wank2);
                } else {
                    BrVector3Sub(&wank, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no), &car_master_actor->t.t.translate.t);
                    error = BrVector3Length(&wank);
                    BrVector3Scale(&wank, &wank, 15.f / error);
                }
                pos_error = 15.f;
            } else if (pos_error_factor < 0.f) {
                BrVector3Sub(&wank, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no), &car_master_actor->t.t.translate.t);
                pos_error = BrVector3Length(&wank) * WORLD_SCALE;
                BrVector3Scale(&wank, &wank, 15.f / pos_error);
            }
        }
        // 1 / BrVector3Length(&wank);
        width = GetOpponentsSectionWidth(pOpponent_spec, data->section_no) * WORLD_SCALE;
        if (!pIgnore_end && speed * 1.5f > dist_to_end) {
            dr_dprintf("%s: ProcessFollowPath() - *** CHANGING SECTIONS ***", car_spec->driver_name);
            dr_dprintf("%s: ProcessFollowPath() - current section %d(#%d)", car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            data->section_no = GetOpponentsNextSection(pOpponent_spec, data->section_no);
            if (data->section_no == -1) {
                car_spec->acc_force = 0.f;
                car_spec->brake_force = 0.f;
                dr_dprintf("%s: ProcessFollowPath() - reached end of path", car_spec->driver_name);
                return eFPR_end_of_path;
            }
            wank.v[1] = 0.f;
            dr_dprintf("%s: ProcessFollowPath() - next section %d(#%d)", car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            data->last_struggle_section = -1;
            data->borrowed_time_start = gTime_stamp_for_this_munging;
            data->made_it = 1;
            data->last_distance = 0.f;
        }
        BrVector3Cross(&corner, &wank, (br_vector3*)car_master_actor->t.t.mat.m[2]);

        first_straight = 0;
        if (BrVector3Dot(&wank, (br_vector3*)car_master_actor->t.t.mat.m[2]) > 0.f && speed > 10.f) {
            data->desired_speed = 6.f;
            car_spec->curvature = 0.f;
            first_straight = 1;
        } else if ((corner.v[1] > 0.f && BrVector3Dot(&wank, (br_vector3*)car_master_actor->t.t.mat.m[2]) > 0.f) || GetOpponentsSectionWidth(pOpponent_spec, data->section_no) < corner.v[1]) {
            car_spec->curvature = MaxCurvatureForCarSpeed(car_spec, speed);
            data->desired_speed = 6.f;
        } else if ((corner.v[1] < 0.f && BrVector3Dot(&wank, (br_vector3*)car_master_actor->t.t.mat.m[2]) > 0.f) || -GetOpponentsSectionWidth(pOpponent_spec, data->section_no) > corner.v[1]) {
            car_spec->curvature = -MaxCurvatureForCarSpeed(car_spec, speed);
            data->desired_speed = 6.f;
        } else if (corner.v[1] > .5f * GetOpponentsSectionWidth(pOpponent_spec, data->section_no)) {
            car_spec->curvature = 0.05f * MaxCurvatureForCarSpeed(car_spec, speed);
            data->desired_speed = 80.f;
        } else if (corner.v[1] < -.5f * GetOpponentsSectionWidth(pOpponent_spec, data->section_no)) {
            car_spec->curvature = -0.05f * MaxCurvatureForCarSpeed(car_spec, speed);
            data->desired_speed = 80.f;
        } else {
            car_spec->curvature = 0.f;
            data->desired_speed = 80.f;
        }
        if (first_straight) {
            car_spec->brake_force = 15.f * car_spec->M;
            car_spec->acc_force = 0.f;
            return eFPR_OK;
        }
        if (GetOpponentsNextSection(pOpponent_spec, data->section_no) != -1) {
            next_turning_radius = (car_master_actor->t.t.translate.t.v[2] * corner.v[0] - car_master_actor->t.t.translate.t.v[0] * corner.v[2])
                - (GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2] * corner.v[0] - GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0] * corner.v[2]);
            next_corner_size = car_master_actor->t.t.mat.m[2][2] * corner.v[0] - car_master_actor->t.t.mat.m[2][0] * corner.v[2];
            if (next_corner_size * next_turning_radius > 0.f) {
                speed_to_goal = 0.f;
                BrVector3Copy(&a, &car_master_actor->t.t.translate.t);
                BrVector3Scale(&p, (br_vector3*)car_master_actor->t.t.mat.m[2], -next_turning_radius / next_corner_size);
                BrVector3Sub(&p, &car_master_actor->t.t.translate.t, &p);
                real_section_no = data->section_no;
                section_no = 0;
                while (GetOpponentsNextSection(pOpponent_spec, real_section_no) != -1 && section_no < 4) {
                    BrVector3Copy(&next, GetOpponentsSectionFinishNodePoint(pOpponent_spec, GetOpponentsNextSection(pOpponent_spec, real_section_no)));
                    StraightestArcForCorner(&corner_speed, &corner_speed2, &pos_error, &a, &p, &next, &a, &p,
                        GetOpponentsSectionWidth(pOpponent_spec, data->section_no), GetOpponentsSectionWidth(pOpponent_spec, GetOpponentsNextSection(pOpponent_spec, data->section_no)));
                    pos_error *= WORLD_SCALE;
                    speed_to_goal += pos_error;
                    if (speed_to_goal > speed * (1.5f + speed / 24.f)) {
                        break;
                    }
                    corner_speed *= WORLD_SCALE;
                    corner_speed2 *= WORLD_SCALE;
                    stopping_distance = 10.f * corner_speed * CornerFudge(car_spec) * CornerFudge(car_spec);
                    desired_speed = sqrtf(stopping_distance);
                    if (desired_speed > GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1)) {
                        desired_speed = sqrtf(GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1));
                        stopping_distance = desired_speed * desired_speed;
                    }
                    if (speed_to_goal - corner_speed2 < desired_speed * 1.5f * (speed * speed - stopping_distance) / 24.f
                        && data->desired_speed > desired_speed) {
                        data->desired_speed = desired_speed;
                    }
                    BrVector3Copy(&a, &p);
                    BrVector3Copy(&p, &next);
                }
            }
        }
        effective_speed_factor = CAR_SPEC_GET_SPEED_FACTOR(car_spec);
        max_acc = MAX(effective_speed_factor, 1.f);
        if (engine_damage > 50 && engine_damage <= 97) {
            max_acc -= (engine_damage - 50) * .0125f;
        } else if (engine_damage > 97) {
            max_acc -= .6f;
        }
        if (trans_damage > 50 && trans_damage <= 97) {
            max_acc -= (trans_damage - 50) * .00625f;
        } else if (trans_damage > 97) {
            max_acc -= .3f;
        }
        max_acc *= 10.f;

        error = data->desired_speed * effective_speed_factor - speed;
        acc = (error - data->prev_acc_error) * 1000.f / gFrame_period_for_this_munging * .1f + error + data->prev_acc;
        if (acc > max_acc) {
            acc = max_acc;
        }
        if (acc < -max_acc) {
            acc = -max_acc;
        }
        data->prev_acc = acc;
        data->prev_acc_error = error;

        car_spec->acc_force = MAX(car_spec->M * acc, 0.f);
        car_spec->brake_force = -MIN(car_spec->M * acc, 0.f);
        return eFPR_OK;
    } else {
        BrFatal("C:\\Msdev\\Projects\\DethRace\\OPPOPROC.C", 1420, "C:\\Msdev\\Projects\\DethRace\\OPPOPROC.C line %d");
        return eFPR_OK;
    }
}

// IDA: tFollow_path_result __usercall FollowCheatyPath@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
tFollow_path_result FollowCheatyPath(tOpponent_spec* pOpponent_spec) {
    tFollow_path_data* data;
    br_vector3 a;
    br_vector3 p;
    br_vector3 section_v;
    br_vector3 car_to_end;
    br_vector3 car_to_intersect;
    br_vector3* start;
    br_vector3* finish;
    br_scalar t;
    br_scalar frame_period_in_secs;
    br_scalar distance_left;
    br_scalar distance_to_end;
    br_scalar distance_to_intersect;
    br_scalar section_min;
    br_scalar section_max;
    br_scalar desired_speed_BRU;
    LOG_TRACE("(%p)", pOpponent_spec);
    NOT_IMPLEMENTED();
}
