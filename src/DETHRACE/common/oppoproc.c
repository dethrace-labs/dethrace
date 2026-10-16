#include "oppoproc.h"
#include "brender.h"
#include "errors.h"
#include "globvars.h"
#include "harness/trace.h"
#include "opponent.h"
#include "pd/sys.h"
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
    NOT_IMPLEMENTED();
}

// The DOS implementation uses 3D vectors; CARM95 calculates in the XZ plane.
// FUNCTION: CARM95 0x0049feaa
void StraightestArcForCorner(float* p1, float* p2, float* p3, br_vector3* p4, br_vector3* p5, br_vector3* p6, br_vector3* p7, br_vector3* p8, float p9, float p10) {
    br_scalar rel1; // z23, then dot product and cosine.
    br_scalar dz12;
    br_scalar tmp1;
    br_scalar rel3; // x23, then cross product and absolute sine.
    br_vector3 rot1;
    br_scalar tmp; // Tangent of half the corner angle.
    br_scalar tmp2;
    br_scalar tmp3; // x12.
    br_scalar tmp4;

    // Unused locals are retained to preserve the retail stack frame.
    dz12 = p6->v[2] - p5->v[2];
    tmp3 = p6->v[0] - p5->v[0];
    rel1 = p4->v[2] - p5->v[2];
    rel3 = p4->v[0] - p5->v[0];
    *p3 = sqrt(rel1 * rel1 + rel3 * rel3);
    if (*p3 <= 1.1920928955078125e-6f) {
        *p2 = 0.001f;
        *p3 = 0.001f;
    }
    tmp2 = dz12 * rel1 + tmp3 * rel3;
    rel3 = (-tmp3) * rel1 + rel3 * dz12;
    rel1 = tmp2;
    tmp2 = sqrt(rel1 * rel1 + rel3 * rel3);
    rel1 = rel1 / tmp2;
    rel3 = rel3 / tmp2;
    rel3 = fabs(rel3);
    tmp2 = (p10) * rel1 + p9;
    if ((rel3 < 1.f && fabs(tmp2) > rel3 * 1000.f) || rel3 < 0.001f) {
        *p1 = 1000.f;
        *p2 = 1000.f;
    } else {
        tmp2 = tmp2 / rel3;
        tmp = rel3 / (rel1 + 1.f);
        *p2 = tmp * p10 + sqrt(((tmp * tmp * p10) + 2.f * (tmp) * tmp2 - p10) * p10) + tmp2;
        *p1 = *p2 * tmp;
    }
}

// IDA: br_scalar __usercall CornerFudge@<ST0>(tCar_spec *pCar_spec@<EAX>)
// FUNCTION: CARM95 0x004a003e
br_scalar CornerFudge(tCar_spec* pCar_spec) {

    return 1.4f;
}

// IDA: br_scalar __usercall MaxCurvatureForCarSpeed@<ST0>(tCar_spec *pCar@<EAX>, br_scalar pSpeed)
// FUNCTION: CARM95 0x004a0054
br_scalar MaxCurvatureForCarSpeed(tCar_spec* pCar, br_scalar pSpeed) {
    br_scalar curv;

    if (pSpeed < 12.5f) {
        curv = pCar->maxcurve;
    } else {
        curv = pCar->maxcurve * 12.5f / pSpeed;
    }
    return curv;
}

// IDA: br_scalar __usercall Vector2Cross@<ST0>(br_vector2 *pA@<EAX>, br_vector2 *pB@<EDX>)
br_scalar Vector2Cross(br_vector2* pA, br_vector2* pB) {
    NOT_IMPLEMENTED();
}

// IDA: tFollow_path_result __usercall EndOfPath@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
tFollow_path_result EndOfPath(tOpponent_spec* pOpponent_spec) {
    tCar_spec* car_spec;
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
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetStraight@<EAX>(br_vector2 *pStart@<EAX>, br_vector2 *pFinish@<EDX>, br_scalar *pWidth@<EBX>, int section1@<ECX>, tOpponent_spec *pOpponent_spec, tFollow_path_data *data)
int GetStraight(br_vector2* pStart, br_vector2* pFinish, br_scalar* pWidth, int section1, tOpponent_spec* pOpponent_spec, tFollow_path_data* data) {
    int section;
    br_vector2 next;
    br_scalar next_width;
    NOT_IMPLEMENTED();
}

// IDA: tFollow_path_result __usercall ProcessFollowPath@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>, int pPursuit_mode@<EBX>, int pIgnore_end@<ECX>, int pNever_struggle)
// FUNCTION: CARM95 0x0049e3a0
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


    car_spec = pOpponent_spec->car_spec;
    engine_damage = car_spec->damage_units[0].damage_level;
    trans_damage = car_spec->damage_units[1].damage_level;
    data = &pOpponent_spec->follow_path_data;
    car_master_actor = car_spec->car_master_actor;

    switch (pCommand) {
    case ePOC_start:
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
    case ePOC_run:
        if (data->cheating || pOpponent_spec->cheating != data->cheating) {
            return FollowCheatyPath(pOpponent_spec);
        }
        if (!pIgnore_end && !data->made_it && data->borrowed_time_start + 1000 < gTime_stamp_for_this_munging && gTime_stamp_for_this_munging < data->borrowed_time_start + 10000) {
            BrVector3Sub(&a, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            BrVector3Sub(&p, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            t = BrVector3Dot(&p, &a) / BrVector3LengthSquared(&a);
            BrVector3Scale(&intersect, &a, t);
            BrVector3Sub(&wank, &p, &intersect);
            distance = BrVector3Length(&wank);
            if (GetOpponentsSectionWidth(pOpponent_spec, data->section_no) >= distance) {
                data->made_it = 1;
            }
        }
        if (data->borrowed_time_start + 10000 < gTime_stamp_for_this_munging && !data->made_it) {
            dr_dprintf("%s: ProcessFollowPath() giving up due to not making the corner", pOpponent_spec->car_spec->driver_name);
            return eFPR_given_up;
        }
        car_spec->keys.acc = 1;
        speed = BrVector3Length(&car_spec->v);
        if (speed > 0.2f) {
            data->has_moved_during_this_task = 1;
            pOpponent_spec->has_moved_at_some_point = 1;
        }
        if (data->struggle_time) {
            if (data->struggle_time + 750 * (data->number_of_struggles - 1) + 2750 < gTime_stamp_for_this_munging) {
                dr_dprintf("%s: done struggling. speed = %.2f m/s", pOpponent_spec->car_spec->driver_name, speed);
                data->made_it = 0;
                data->borrowed_time_start = gTime_stamp_for_this_munging;
                data->struggle_time = 0;
                data->last_finished_struggle_time = gTime_stamp_for_this_munging;
                car_spec->brake_force = 0.0f;
                car_spec->acc_force = 0.0f;
            } else {
                if (data->struggle_time + 750 * (data->number_of_struggles - 1) + 2000 < gTime_stamp_for_this_munging) {
                    car_spec->acc_force = 0.0f;
                    car_spec->brake_force = car_spec->M * 15.0f;
                    car_spec->curvature = 0.0f;
                    return eFPR_OK;
                } else {
                    car_spec->brake_force = 0.0f;
                    car_spec->acc_force = car_spec->M * -6.0f;
                    car_spec->curvature = 0.0f;
                    return eFPR_OK;
                }
            }
        } else {
            if (pIgnore_end) {
                stopped_speed = 0.06666667f;
            } else {
                stopped_speed = 0.2f;
            }
            if (!pNever_struggle && (stopped_speed) >= speed && data->last_finished_struggle_time + 2000 < gTime_stamp_for_this_munging && (pPursuit_mode || data->has_moved_during_this_task != 0)) {
                dr_dprintf("%s: 'Stopped,' section #%d, speed = %.2f m/s, about to start a-strugglin'", pOpponent_spec->car_spec->driver_name, data->section_no, speed);
                data->struggle_time = gTime_stamp_for_this_munging;
                if (!pIgnore_end && data->section_no == data->last_struggle_section) {
                    if (data->number_of_struggles >= 3) {
                        car_spec->acc_force = 0.0f;
                        car_spec->brake_force = 0.0f;
                        dr_dprintf("%s: Giving up trying to follow path 'cos we've struggled too much", pOpponent_spec->car_spec->driver_name);
                        return eFPR_given_up;
                    } else {
                        data->number_of_struggles++;
                    }
                } else {
                    data->last_struggle_section = data->section_no;
                    data->number_of_struggles = 0;
                }
            }
        }
        BrVector3Sub(&goal_dir, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        goal_dir.v[1] = 0.0f;
        dist_to_goal = BrVector3Length(&goal_dir) * WORLD_SCALE_D;
        dist_to_end = dist_to_goal;
        if (dist_to_goal > 15.0f) {
            BrVector3Sub(&wank, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));
            BrVector3Normalise(&section_dir, &wank);
            BrVector3Sub(&wank, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));

            dist_along = BrVector3Dot(&section_dir, &wank); // v62.v[2] * wank.v[2] + v62.v[1] * wank.v[1] + v62.v[0] * wank.v[0];
            BrVector3Scale(&wank2, &section_dir, dist_along);
            BrVector3Sub(&goal_dir, &wank2, &wank);
            goal_dir.v[1] = 0.0f;
            dist_to_goal = BrVector3Length(&goal_dir) * WORLD_SCALE_D;
            if (dist_to_goal < 15.0f) {
                further_along = sqrt(225.0f - dist_to_goal * dist_to_goal) / WORLD_SCALE_D;
                if (further_along + dist_along < 0.0f) {
                    BrVector3Sub(&goal_dir, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no), &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                    dist_to_goal = BrVector3Length(&goal_dir) * WORLD_SCALE_D;
                    BrVector3Scale(&goal_dir, &goal_dir, 15.0f / dist_to_goal);
                } else {
                    wank.v[0] = section_dir.v[0] * further_along;
                    wank.v[1] = section_dir.v[1] * further_along;
                    wank.v[2] = section_dir.v[2] * further_along;
                    wank.v[1] = 0.0f;
                    BrVector3Accumulate(&goal_dir, &wank);
                }
                dist_to_goal = 15.0f;
            } else if (dist_along < 0.0f) {
                BrVector3Sub(&goal_dir, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no), &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                dist_to_goal = BrVector3Length(&goal_dir) * WORLD_SCALE_D;
                BrVector3Scale(&goal_dir, &goal_dir, 15.0f / dist_to_goal);
            }
        }
        speed_to_goal = BrVector3Dot(&car_spec->v, &goal_dir) / BrVector3Length(&goal_dir);
        section_width = GetOpponentsSectionWidth(pOpponent_spec, data->section_no);
        section_width *= WORLD_SCALE_D;
        if (!pIgnore_end && speed * 1.5f > dist_to_end) {
            dr_dprintf("%s: ProcessFollowPath() - *** CHANGING SECTIONS ***", pOpponent_spec->car_spec->driver_name);
            dr_dprintf("%s: ProcessFollowPath() - current section %d(#%d)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            data->section_no = GetOpponentsNextSection(pOpponent_spec, data->section_no);
            if (data->section_no == -1) {
                car_spec->acc_force = 0.0f;
                car_spec->brake_force = 0.0f;
                dr_dprintf("%s: ProcessFollowPath() - reached end of path", pOpponent_spec->car_spec->driver_name);
                return eFPR_end_of_path;
            }
            goal_dir.v[1] = 0.0f;
            real_section_no = GetOpponentsRealSection(pOpponent_spec, data->section_no);
            dr_dprintf("%s: ProcessFollowPath() - next section %d(#%d)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            data->last_struggle_section = -1;
            data->borrowed_time_start = gTime_stamp_for_this_munging;
            data->made_it = 0;
            data->last_distance = 0.0f;
        }
        not_our_dir = (br_vector3*)&car_spec->car_master_actor->t.t.mat.m[2];

        BrVector3Cross(&wank, &goal_dir, not_our_dir);

        goal_width = GetOpponentsSectionWidth(pOpponent_spec, data->section_no) * 0.5f;
        just_fucking_brake = 0;
        if (BrVector3Dot(&goal_dir, not_our_dir) > 0.0f && speed > 10.0f) {
            data->desired_speed = 6.0f;
            car_spec->curvature = 0.0f;
            just_fucking_brake = 1;
        } else {
            if ((wank.v[1] > 0.0f && BrVector3Dot(&goal_dir, not_our_dir) > 0.0f) || GetOpponentsSectionWidth(pOpponent_spec, data->section_no) < wank.v[1]) {
                car_spec->curvature = MaxCurvatureForCarSpeed(car_spec, speed);
                data->desired_speed = 6.0f;
            } else if ((wank.v[1] < 0.0f && BrVector3Dot(&goal_dir, not_our_dir) > 0.0f) || -GetOpponentsSectionWidth(pOpponent_spec, data->section_no) > wank.v[1]) {
                car_spec->curvature = -MaxCurvatureForCarSpeed(car_spec, speed);
                data->desired_speed = 6.0f;
            } else if (wank.v[1] > goal_width) {
                car_spec->curvature = MaxCurvatureForCarSpeed(car_spec, speed) * 0.05f;
                data->desired_speed = 80.0f;
            } else if (-goal_width > wank.v[1]) {
                car_spec->curvature = -(MaxCurvatureForCarSpeed(car_spec, speed) * 0.05f);
                data->desired_speed = 80.0f;
            } else {
                car_spec->curvature = 0.0f;
                data->desired_speed = 80.0f;
            }
        }
        if (just_fucking_brake) {
            car_spec->brake_force = car_spec->M * 15.0f;
            car_spec->acc_force = 0.0f;
        } else {
            if (GetOpponentsNextSection(pOpponent_spec, data->section_no) != -1) {
                dot_a = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] * (-wank.v[2]) + pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] * wank.v[0];
                dot_a = dot_a - (GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2] * wank.v[0] + GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0] * (-wank.v[2]));
                // FIXME: added temporary variable
                dot_d = (-wank.v[2]) * not_our_dir->v[0] + not_our_dir->v[2] * wank.v[0];
                if (dot_d * dot_a > 0.0f) {
                    distance = 0.0f;
                    stopping_distance = speed * speed / 24.0f + speed * 1.5f;
                    BrVector3Copy(&start, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                    BrVector3Scale(&corner, not_our_dir, -(dot_a / dot_d));
                    BrVector3Accumulate(&corner, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                    section_no = data->section_no;
                    for (sx = 0; GetOpponentsNextSection(pOpponent_spec, section_no) != -1 && sx < 4; section_no = GetOpponentsNextSection(pOpponent_spec, section_no), sx++) {
                        BrVector3Copy(&next, GetOpponentsSectionFinishNodePoint(pOpponent_spec, GetOpponentsNextSection(pOpponent_spec, section_no)));
                        StraightestArcForCorner(&radius, &entry_len, &dist_along, &start, &corner, &next, &start, &corner,
                            GetOpponentsSectionWidth(pOpponent_spec, data->section_no),
                            GetOpponentsSectionWidth(pOpponent_spec, GetOpponentsNextSection(pOpponent_spec, data->section_no)));
                        dist_along *= WORLD_SCALE_D;
                        distance += dist_along;
                        if (distance > stopping_distance) {
                            break;
                        }
                        radius *= WORLD_SCALE_D;
                        entry_len *= WORLD_SCALE_D;
                        corner_speed2 = CornerFudge(car_spec) * CornerFudge(car_spec) * (radius * 10.0f);
                        corner_speed = sqrt(corner_speed2);
                        if (GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1) < corner_speed) {
                            corner_speed = GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1);
                            corner_speed2 = corner_speed * corner_speed;
                        } else {
                        }
                        if (distance - entry_len < (speed * speed - corner_speed2) / 24.0f + corner_speed * 1.5f && data->desired_speed > corner_speed) {
                            data->desired_speed = corner_speed;
                        }
                        BrVector3Copy(&start, &corner);
                        BrVector3Copy(&corner, &next);
                    }
                }
            }
            if (CAR_SPEC_IS_ROZZER(car_spec)) {
                effective_speed_factor = gCop_speed_factor;
            } else {
                effective_speed_factor = gOpponent_speed_factor;
            }
            if (effective_speed_factor < 1.0f) {
                acc_factor = 1.0f;
            } else {
                acc_factor = effective_speed_factor;
            }
            if (engine_damage > 50 && engine_damage < 98) {
                acc_factor -= (engine_damage - 50) * 0.0125f;
            } else if (engine_damage >= 98) {
                acc_factor -= 0.6f;
            }
            if (trans_damage > 50 && trans_damage < 98) {
                acc_factor -= (trans_damage - 50) * 0.00625f;
            } else if (trans_damage >= 98) {
                acc_factor -= 0.3f;
            }
            if (engine_damage >= 99 || trans_damage >= 99) {
                acc_factor = 0.0f;
            }
            max_acc = acc_factor * 10.0f;
            // TODO: ?? corner_speed2[10] = v30;
            desired_speed = data->desired_speed * effective_speed_factor;
            error = desired_speed - speed;
            if ((acc = data->prev_acc + (error + (error - data->prev_acc_error) * 1000.0f / gFrame_period_for_this_munging * 0.1f)) > max_acc) {
                acc = max_acc;
            }
            if (acc < -max_acc) {
                acc = -max_acc;
            }
            data->prev_acc = acc;
            data->prev_acc_error = error;
            acc = car_spec->M * acc;
            if (acc > 0.0f) {
                car_spec->acc_force = acc;
                car_spec->brake_force = 0.0f;
            } else {
                car_spec->acc_force = 0.0f;
                car_spec->brake_force = -acc;
            }
        }
        return eFPR_OK;

    default:
        BrFatal("C:\\Msdev\\Projects\\DethRace\\OPPOPROC.C", 1420, "C:\\Msdev\\Projects\\DethRace\\OPPOPROC.C line %d", 1420);
        return eFPR_OK;
    }
}

// IDA: tFollow_path_result __usercall FollowCheatyPath@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x004a00a4
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

    data = &pOpponent_spec->follow_path_data;
    start = GetOpponentsSectionStartNodePoint(pOpponent_spec, pOpponent_spec->follow_path_data.section_no);
    finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, pOpponent_spec->follow_path_data.section_no);
    if ((pOpponent_spec->follow_path_data.cheating ^ pOpponent_spec->cheating) != 0) {
        data->cheating = pOpponent_spec->cheating;
        if (data->cheating) {
            dr_dprintf("%s: Dematerialising", pOpponent_spec->car_spec->driver_name);
            BrVector3Sub(&section_v, finish, start);
            BrVector3Sub(&car_to_end, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, start);
            t = BrVector3Dot(&section_v, &car_to_end) / BrVector3LengthSquared(&section_v);
            if (t < 0.0f) {
                BrVector3Copy(&data->cheaty_intersect, start);
            } else if (t > 1.f) {
                BrVector3Copy(&data->cheaty_intersect, finish);
            } else {
                BrVector3Scale(&data->cheaty_intersect, &section_v, t);
                BrVector3Accumulate(&data->cheaty_intersect, start);
            }
            BrVector3Sub(&car_to_intersect, &data->cheaty_intersect, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
            distance_to_intersect = BrVector3Length(&car_to_intersect);
            frame_period_in_secs = gFrame_period_for_this_munging / 1000.0f * 20.0f;
            TurnOpponentPhysicsOff(pOpponent_spec);
            if (distance_to_intersect >= frame_period_in_secs) {
                data->moving_to_intersect = 1;
                BrVector3Normalise(&car_to_intersect, &car_to_intersect);
                BrVector3Scale(&car_to_intersect, &car_to_intersect, frame_period_in_secs);
                BrVector3Accumulate(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &car_to_intersect);
                return eFPR_OK;
            } else {
                data->moving_to_intersect = 0;
                BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &data->cheaty_intersect);
                return eFPR_OK;
            }
        }
        BrVector3Sub(&p, finish, start);
        PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &p);
        BrVector3Sub(&a, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        distance_to_end = BrVector3Length(&a);
        dr_dprintf("%s: Rematerialising from FollowCheatyPath()...", pOpponent_spec->car_spec->driver_name);
        if (data->section_no < 0) {
            PDEnterDebugger("No useful section number in ProcessCheatyPath()");
        }

        section_max = GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1);
        section_min = GetOpponentsSectionMinSpeed(pOpponent_spec, data->section_no, 1);

        if (section_max < 255) {
            desired_speed_BRU = section_max / WORLD_SCALE;
        } else if (section_min > 0) {
            desired_speed_BRU = section_min / WORLD_SCALE;
        } else {
            desired_speed_BRU = MIN(7.0f, MAX(1.0f, distance_to_end * 2.0f));
        }
        if (RematerialiseOpponentOnNearestSection(pOpponent_spec, desired_speed_BRU)) {
            pOpponent_spec->car_spec->brake_force = 0.0f;
            pOpponent_spec->car_spec->acc_force = 0.0f;
            if (distance_to_end >= 5.0f) {
                pOpponent_spec->car_spec->acc_force = pOpponent_spec->car_spec->M / 2.0f;
            } else {
                pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->M * 15.0f;
            }
            return eFPR_OK;
        }
        data->cheating = 1;
    }
    frame_period_in_secs = gFrame_period_for_this_munging / 1000.0f * 20.0f;
    if (data->moving_to_intersect) {
        BrVector3Sub(&car_to_intersect, &data->cheaty_intersect, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        distance_to_intersect = BrVector3Length(&car_to_intersect);
        if (distance_to_intersect < frame_period_in_secs) {
            data->moving_to_intersect = 0;
            BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &data->cheaty_intersect);
        } else {
            BrVector3Normalise(&car_to_intersect, &car_to_intersect);
            BrVector3Scale(&car_to_intersect, &car_to_intersect, frame_period_in_secs);
            BrVector3Accumulate(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &car_to_intersect);
        }
        return eFPR_OK;
    }

    BrVector3Sub(&p, finish, start);
    BrVector3Normalise(&p, &p);
    while (frame_period_in_secs > 0.0f) {
        BrVector3Sub(&a, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        distance_to_end = BrVector3Length(&a);
        if (distance_to_end < frame_period_in_secs) {
            BrVector3Accumulate(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &a);
            frame_period_in_secs -= distance_to_end;
            dr_dprintf("%s: ProcessFollowPath() - current section %d(#%d) (cheating)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            data->section_no = GetOpponentsNextSection(pOpponent_spec, data->section_no);
            if (data->section_no == -1) {
                pOpponent_spec->car_spec->acc_force = 0.0f;
                pOpponent_spec->car_spec->brake_force = 0.0f;
                dr_dprintf("%s: ProcessFollowPath() - reached end of path while cheating", pOpponent_spec->car_spec->driver_name);
                return eFPR_end_of_path;
            }
            dr_dprintf("%s: ProcessFollowPath() - next section %d(#%d) (cheating)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            start = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no);
            finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no);
            BrVector3Sub(&p, finish, start);
            BrVector3Normalise(&p, &p);
        } else {
            BrVector3Scale(&p, &p, frame_period_in_secs);
            BrVector3Accumulate(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &p);
            frame_period_in_secs = 0.0;
        }
    }
    return eFPR_OK;
}
