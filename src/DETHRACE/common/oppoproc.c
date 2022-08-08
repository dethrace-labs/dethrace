#include "oppoproc.h"
#include "errors.h"
#include "globvars.h"
#include "harness/trace.h"
#include "opponent.h"
#include "pd/sys.h"
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
    if (pCommand) {
        if (pCommand != ePOC_run) {
            BrFatal("C:\\Msdev\\Projects\\DethRace\\OPPOPROC.C", 1420, "C:\\Msdev\\Projects\\DethRace\\OPPOPROC.C line %d", 140);
            return 0;
        }
        if (pOpponent_spec->follow_path_data.cheating || pOpponent_spec->cheating) {
            return FollowCheatyPath(pOpponent_spec);
        }
        if (!pIgnore_end && !data->made_it && data->borrowed_time_start + 1000 < gTime_stamp_for_this_munging && gTime_stamp_for_this_munging < data->borrowed_time_start + 10000) {

            float v59, v58, v57, v55, v54;
            br_vector3 v56;

            v59 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0];
            section_dir.v[0] = v59 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0];
            v58 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[1];
            section_dir.v[1] = v58 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1];
            v57 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2];
            section_dir.v[2] = v57 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2];
            v56.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
            goal_dir.v[0] = v56.v[0] - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0];
            v55 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
            goal_dir.v[1] = v55 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1];
            v54 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
            goal_dir.v[2] = v54 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2];
            dist_along = (goal_dir.v[1] * section_dir.v[1]
                             + goal_dir.v[2] * section_dir.v[2]
                             + goal_dir.v[0] * section_dir.v[0])
                / (section_dir.v[1] * section_dir.v[1]
                    + section_dir.v[2] * section_dir.v[2]
                    + section_dir.v[0] * section_dir.v[0]);
            section_v.v[0] = section_dir.v[0] * dist_along;
            section_v.v[1] = section_dir.v[1] * dist_along;
            section_v.v[2] = section_dir.v[2] * dist_along;
            wank.v[0] = goal_dir.v[0] - section_v.v[0];
            wank.v[1] = goal_dir.v[1] - section_v.v[1];
            wank.v[2] = goal_dir.v[2] - section_v.v[2];
            goal_width = sqrtf(wank.v[2] * wank.v[2] + wank.v[1] * wank.v[1] + wank.v[0] * wank.v[0]);
            if (GetOpponentsSectionWidth(pOpponent_spec, data->section_no) >= (double)goal_width) {
                data->made_it = 1;
            }
        }
        if (data->borrowed_time_start + 10000 < gTime_stamp_for_this_munging && !data->made_it) {
            dr_dprintf("%s: ProcessFollowPath() giving up due to not making the corner", pOpponent_spec->car_spec->driver_name);
            return 2;
        }
        car_spec->keys.acc = 1;

        speed = sqrtf(
            car_spec->v.v[2] * car_spec->v.v[2]
            + car_spec->v.v[1] * car_spec->v.v[1]
            + car_spec->v.v[0] * car_spec->v.v[0]);

        if (speed > 0.2f) {
            data->has_moved_during_this_task = 1;
            pOpponent_spec->has_moved_at_some_point = 1;
        }
        if (data->struggle_time) {
            if (data->struggle_time + 150 * (5 * data->number_of_struggles - 5) + 2750 >= gTime_stamp_for_this_munging) {
                if (data->struggle_time + 150 * (5 * data->number_of_struggles - 5) + 2000 >= gTime_stamp_for_this_munging) {
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
                stopped_speed = 0.06666667;
            } else {
                stopped_speed = 0.2;
            }
            if (!pNever_struggle
                && stopped_speed >= (double)speed
                && data->last_finished_struggle_time + 2000 < gTime_stamp_for_this_munging
                && (pPursuit_mode || data->has_moved_during_this_task != 0)) {
                dr_dprintf(
                    "%s: 'Stopped,' section #%d, speed = %.2f m/s, about to start a-strugglin'",
                    pOpponent_spec->car_spec->driver_name,
                    data->section_no,
                    speed);
                data->struggle_time = gTime_stamp_for_this_munging;
                if (pIgnore_end || data->section_no != data->last_struggle_section) {
                    data->last_struggle_section = data->section_no;
                    data->number_of_struggles = 0;
                } else {
                    if (data->number_of_struggles >= 3u) {
                        car_spec->acc_force = 0.0;
                        car_spec->brake_force = 0.0;
                        dr_dprintf(
                            "%s: Giving up trying to follow path 'cos we've struggled too much",
                            pOpponent_spec->car_spec->driver_name);
                        return 2;
                    }
                    data->number_of_struggles++;
                }
            }
        }
        car_to_end.v[0] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0]
            - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
        car_to_end.v[1] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[1]
            - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
        car_to_end.v[2] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2]
            - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
        car_to_end.v[1] = 0.0;
        dist_to_end = sqrtf(car_to_end.v[0] * car_to_end.v[0] + 0.0 * 0.0 + car_to_end.v[2] * car_to_end.v[2]) * 6.9;
        float v80 = dist_to_end;
        if (dist_to_end > 15.0) {
            float v53_2 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0];
            wank.v[0] = v53_2 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0];
            float v52_1 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[1];
            wank.v[1] = v52_1 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1];
            float v51_0 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2];
            wank.v[2] = v51_0 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2];
            float v11 = sqrtf(wank.v[2] * wank.v[2] + wank.v[1] * wank.v[1] + wank.v[0] * wank.v[0]);
            // if (v13 | v14) {
            //     v62.v[0] = 1.0;
            //     v62.v[1] = 0.0;
            //     v62.v[2] = 0.0;
            // } else {
            //     v60 = v11;
            //     v15 = 1.0 / v60;
            //     v60 = v15;
            //     v62.v[0] = v15 * wank.v[0];
            //     v62.v[1] = wank.v[1] * v60;
            //     v62.v[2] = wank.v[2] * v60;
            // }
            float temp_0, temp_1, temp_2;
            br_vector3 v62;
            BrVector3Normalise(&v62, &wank);
            temp_2 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
            wank.v[0] = temp_2 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0];
            temp_1 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
            wank.v[1] = temp_1 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1];
            temp_0 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
            wank.v[2] = temp_0 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2];
            pos_error = v62.v[2] * wank.v[2] + v62.v[1] * wank.v[1] + v62.v[0] * wank.v[0];
            br_vector3 v64;
            v64.v[0] = v62.v[0] * pos_error;
            v64.v[2] = v62.v[2] * pos_error;
            car_to_end.v[0] = v64.v[0] - wank.v[0];
            car_to_end.v[2] = v64.v[2] - wank.v[2];
            car_to_end.v[1] = 0.0;
            float v16 = sqrtf(car_to_end.v[0] * car_to_end.v[0] + 0.0 * 0.0 + car_to_end.v[2] * car_to_end.v[2]);
            float v63;
            dist_to_end = v16 * 6.9;
            if (dist_to_end < 15.0f) {
                v63 = sqrtf(225.0 - dist_to_end * dist_to_end) / 6.9;
                if (v63 + pos_error >= 0.0) {
                    wank.v[0] = v62.v[0] * v63;
                    wank.v[2] = v62.v[2] * v63;
                    wank.v[1] = 0.0;
                    car_to_end.v[0] = wank.v[0] + car_to_end.v[0];
                    car_to_end.v[1] = car_to_end.v[1] + 0.0;
                    car_to_end.v[2] = wank.v[2] + car_to_end.v[2];
                } else {
                    car_to_end.v[0] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0]
                        - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
                    car_to_end.v[1] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1]
                        - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
                    car_to_end.v[2] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2]
                        - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
                    dist_to_end = sqrtf(
                                      car_to_end.v[0] * car_to_end.v[0]
                                      + car_to_end.v[1] * car_to_end.v[1]
                                      + car_to_end.v[2] * car_to_end.v[2])
                        * 6.9;
                    car_to_end.v[0] = 15.0 / dist_to_end * car_to_end.v[0];
                    car_to_end.v[1] = 15.0 / dist_to_end * car_to_end.v[1];
                    car_to_end.v[2] = 15.0 / dist_to_end * car_to_end.v[2];
                }
                dist_to_end = 15.0;
            } else if (pos_error < 0.0) {
                car_to_end.v[0] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0]
                    - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
                car_to_end.v[1] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1]
                    - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
                car_to_end.v[2] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2]
                    - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
                dist_to_end = sqrtf(
                                  car_to_end.v[0] * car_to_end.v[0]
                                  + car_to_end.v[1] * car_to_end.v[1]
                                  + car_to_end.v[2] * car_to_end.v[2])
                    * 6.9;
                car_to_end.v[0] = 15.0 / dist_to_end * car_to_end.v[0];
                car_to_end.v[1] = 15.0 / dist_to_end * car_to_end.v[1];
                car_to_end.v[2] = 15.0 / dist_to_end * car_to_end.v[2];
            }
        }
        // float v20 = car_spec->v.v[1] * car_to_end.v[1]
        //     + car_spec->v.v[2] * car_to_end.v[2]
        //     + car_spec->v.v[0] * car_to_end.v[0];
        // v94 = v20 / sqrtf(v20);
        section_width = GetOpponentsSectionWidth(pOpponent_spec, data->section_no) * 6.9;
        if (!pIgnore_end && speed * 1.5 > v80) {
            dr_dprintf("%s: ProcessFollowPath() - *** CHANGING SECTIONS ***", pOpponent_spec->car_spec->driver_name);
            int OpponentsRealSection = GetOpponentsRealSection(pOpponent_spec, data->section_no);
            dr_dprintf(
                "%s: ProcessFollowPath() - current section %d(#%d)",
                pOpponent_spec->car_spec->driver_name,
                data->section_no,
                OpponentsRealSection);
            data->section_no = GetOpponentsNextSection(pOpponent_spec, data->section_no);
            if (data->section_no == -1) {
                car_spec->acc_force = 0.0;
                car_spec->brake_force = 0.0;
                dr_dprintf("%s: ProcessFollowPath() - reached end of path", pOpponent_spec->car_spec->driver_name);
                return 1;
            }
            car_to_end.v[1] = 0.0;
            real_section_no = GetOpponentsRealSection(pOpponent_spec, data->section_no);
            int v22 = GetOpponentsRealSection(pOpponent_spec, data->section_no);
            dr_dprintf(
                "%s: ProcessFollowPath() - next section %d(#%d)",
                pOpponent_spec->car_spec->driver_name,
                data->section_no,
                v22);
            data->last_struggle_section = -1;
            data->borrowed_time_start = gTime_stamp_for_this_munging;
            data->made_it = 1;
            data->last_distance = 0.0;
        }
        br_actor* cma = car_spec->car_master_actor;
        br_vector3* v96 = (br_vector3*)&cma->t.t.mat.m[2];
        wank.v[0] = cma->t.t.mat.m[2][2] * car_to_end.v[1] - cma->t.t.mat.m[2][1] * car_to_end.v[2];
        wank.v[1] = cma->t.t.mat.m[2][0] * car_to_end.v[2] - cma->t.t.mat.m[2][2] * car_to_end.v[0];
        wank.v[2] = cma->t.t.mat.m[2][1] * car_to_end.v[0] - cma->t.t.mat.m[2][0] * car_to_end.v[1];

        float width_1 = GetOpponentsSectionWidth(pOpponent_spec, data->section_no) * 0.5;
        just_fucking_brake = 0;
        if (car_to_end.v[1] * v96->v[1] + v96->v[2] * car_to_end.v[2] + v96->v[0] * car_to_end.v[0] <= 0.0
            || speed <= 10.0) {
            if ((wank.v[1] <= 0.0
                    || car_to_end.v[1] * v96->v[1] + v96->v[2] * car_to_end.v[2] + v96->v[0] * car_to_end.v[0] <= 0.0)
                && GetOpponentsSectionWidth(pOpponent_spec, data->section_no) >= (double)wank.v[1]) {
                if ((wank.v[1] >= 0.0
                        || car_to_end.v[1] * v96->v[1] + v96->v[2] * car_to_end.v[2] + v96->v[0] * car_to_end.v[0] <= 0.0)
                    && -GetOpponentsSectionWidth(pOpponent_spec, data->section_no) <= wank.v[1]) {
                    if (wank.v[1] <= (double)width_1) {
                        if (-width_1 <= wank.v[1]) {
                            car_spec->curvature = 0.0;
                        } else {
                            car_spec->curvature = -(MaxCurvatureForCarSpeed(car_spec, speed) * 0.050000001);
                        }
                        data->desired_speed = 80.0;
                    } else {
                        car_spec->curvature = MaxCurvatureForCarSpeed(car_spec, speed) * 0.050000001;
                        data->desired_speed = 80.0;
                    }
                } else {
                    car_spec->curvature = -MaxCurvatureForCarSpeed(car_spec, speed);
                    data->desired_speed = 6.0;
                }
            } else {
                car_spec->curvature = MaxCurvatureForCarSpeed(car_spec, speed);
                data->desired_speed = 6.0;
            }
        } else {
            data->desired_speed = 6.0;
            car_spec->curvature = 0.0;
            just_fucking_brake = 1;
        }
        if (just_fucking_brake) {
            car_spec->brake_force = car_spec->M * 15.0;
            car_spec->acc_force = 0.0;
        } else {
            if (GetOpponentsNextSection(pOpponent_spec, data->section_no) != -1) {
                next_turning_radius = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] * -wank.v[2]
                    + pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] * wank.v[0];
                next_corner_size = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0]
                    * -wank.v[2];
                next_turning_radius = next_turning_radius
                    - (GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2]
                            * wank.v[0]
                        + next_corner_size);
                float v104 = -wank.v[2] * v96->v[0] + v96->v[2] * wank.v[0];
                if (v104 * next_turning_radius > 0.0) {
                    goal_width = 0.0;
                    float v92 = speed * speed / 24.0 + speed * 1.5;
                    p = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t;
                    start.v[0] = -(next_turning_radius / v104) * v96->v[0];
                    start.v[1] = -(next_turning_radius / v104) * v96->v[1];
                    start.v[2] = -(next_turning_radius / v104) * v96->v[2];
                    start.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] + start.v[0];
                    start.v[1] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] + start.v[1];
                    start.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] + start.v[2];
                    section_no = data->section_no;
                    for (sx = 0; GetOpponentsNextSection(pOpponent_spec, section_no) != -1; ++sx) {
                        if (sx >= 4) {
                            break;
                        }
                        int nxt_section_temp = GetOpponentsNextSection(pOpponent_spec, section_no);
                        next.v[0] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, nxt_section_temp)->v[0];
                        nxt_section_temp = GetOpponentsNextSection(pOpponent_spec, section_no);
                        next.v[1] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, nxt_section_temp)->v[1];
                        nxt_section_temp = GetOpponentsNextSection(pOpponent_spec, section_no);
                        next.v[2] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, nxt_section_temp)->v[2];
                        nxt_section_temp = GetOpponentsNextSection(pOpponent_spec, data->section_no);
                        next_width = GetOpponentsSectionWidth(pOpponent_spec, nxt_section_temp);
                        width = GetOpponentsSectionWidth(pOpponent_spec, data->section_no);
                        StraightestArcForCorner(
                            &corner_speed,
                            &corner_speed2,
                            &pos_error,
                            &p,
                            &start,
                            &next,
                            &p,
                            &start,
                            width,
                            next_width);
                        pos_error = pos_error * 6.9;
                        goal_width = pos_error + goal_width;
                        if (goal_width > v92) {
                            break;
                        }
                        corner_speed = corner_speed * 6.9;
                        corner_speed2 = corner_speed2 * 6.9;
                        float v83 = CornerFudge(car_spec) * CornerFudge(car_spec) * (corner_speed * 10.0);
                        desired_speed = sqrtf(v83);
                        float OpponentsSectionMaxSpeed = GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1);
                        if ((double)OpponentsSectionMaxSpeed < desired_speed) {
                            desired_speed = (double)GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1);
                            v83 = desired_speed * desired_speed;
                        }
                        if (goal_width - corner_speed2 < (speed * speed - v83) / 24.0 + desired_speed * 1.5
                            && data->desired_speed > (double)desired_speed) {
                            data->desired_speed = desired_speed;
                        }
                        p = start;
                        start = next;
                        section_no = GetOpponentsNextSection(pOpponent_spec, section_no);
                    }
                }
            }
            if ((car_spec->car_ID & 0xFF00) == 768) {
                effective_speed_factor = gCop_speed_factor;
            } else {
                effective_speed_factor = gOpponent_speed_factor;
            }
            if (effective_speed_factor >= 1.0) {
                acc_factor = effective_speed_factor;
            } else {
                acc_factor = 1.0;
            }
            if (engine_damage <= 50 || engine_damage >= 98) {
                if (engine_damage >= 98) {
                    acc_factor = acc_factor - 0.60000002;
                }
            } else {
                acc_factor = acc_factor - (double)(engine_damage - 50) * 0.0125;
            }
            if (trans_damage <= 50 || trans_damage >= 98) {
                if (trans_damage >= 98) {
                    acc_factor = acc_factor - 0.30000001;
                }
            } else {
                acc_factor = acc_factor - (double)(trans_damage - 50) * 0.0062500001;
            }
            if (engine_damage >= 99 || trans_damage >= 99) {
                acc_factor = 0.0;
            }
            max_acc = acc_factor * 10.0;
            float v30 = data->desired_speed * effective_speed_factor;
            // TODO: ?? corner_speed2[10] = v30;
            error = v30 - speed;
            acc = (error - data->prev_acc_error) * 1000.0 / (double)(unsigned int)gFrame_period_for_this_munging * 0.1
                + error
                + data->prev_acc;
            if (acc > max_acc) {
                acc = max_acc;
            }
            if (acc < -max_acc) {
                acc = -max_acc;
            }
            data->prev_acc = acc;
            data->prev_acc_error = error;
            acc = car_spec->M * acc;
            if (acc <= 0.0) {
                car_spec->acc_force = 0.0;
                car_spec->brake_force = -acc;
            } else {
                car_spec->acc_force = acc;
                car_spec->brake_force = 0.0;
            }
        }
        return 0;
    } else {
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
    }
}

// IDA: tFollow_path_result __usercall FollowCheatyPath@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
tFollow_path_result FollowCheatyPath(tOpponent_spec* pOpponent_spec) {
    tFollow_path_data* data;
    br_vector3 a;
    br_vector3 p;                //
    br_vector3 section_v;        //
    br_vector3 car_to_end;       //
    br_vector3 car_to_intersect; //
    br_vector3* start;           //
    br_vector3* finish;          //
    br_scalar t;
    br_scalar frame_period_in_secs;
    br_scalar distance_left;
    br_scalar distance_to_end;
    br_scalar distance_to_intersect;
    br_scalar section_min;
    br_scalar section_max;
    br_scalar desired_speed_BRU;
    LOG_TRACE("(%p)", pOpponent_spec);

    data = &pOpponent_spec->follow_path_data;
    start = GetOpponentsSectionStartNodePoint(pOpponent_spec, pOpponent_spec->follow_path_data.section_no);
    finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, pOpponent_spec->follow_path_data.section_no);
    if (pOpponent_spec->follow_path_data.cheating || pOpponent_spec->cheating) {
        // if (((((*((_DWORD*)&pOpponent_spec->follow_path_data + 13) & 0x400) != 0) ^ (unsigned __int8)(*((_DWORD*)pOpponent_spec + 40) >> 5)) & 1) != 0) {
        //*((_DWORD*)data + 13) = *((_DWORD*)data + 13) & 0xFFFFFBFF | (((*((_DWORD*)pOpponent_spec + 40) >> 5) & 1) << 10);
        data->cheating = pOpponent_spec->cheating;
        if (data->cheating) {
            dr_dprintf("%s: Dematerialising", pOpponent_spec->car_spec->driver_name);
            section_v.v[0] = finish->v[0] - start->v[0];
            section_v.v[1] = finish->v[1] - start->v[1];
            section_v.v[2] = finish->v[2] - start->v[2];
            car_to_end.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] - start->v[0];
            car_to_end.v[1] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] - start->v[1];
            car_to_end.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] - start->v[2];
            distance_left = (section_v.v[2] * car_to_end.v[2]
                                + car_to_end.v[1] * section_v.v[1]
                                + car_to_end.v[0] * section_v.v[0])
                / (section_v.v[2] * section_v.v[2]
                    + section_v.v[1] * section_v.v[1]
                    + section_v.v[0] * section_v.v[0]);
            if (distance_left < 0.0f) {
                data->cheaty_intersect.v[0] = start->v[0];
                data->cheaty_intersect.v[1] = start->v[1];
                data->cheaty_intersect.v[2] = start->v[2];
            } else if (distance_left <= 1.0) {
                data->cheaty_intersect.v[0] = section_v.v[0] * distance_left;
                data->cheaty_intersect.v[1] = section_v.v[1] * distance_left;
                data->cheaty_intersect.v[2] = section_v.v[2] * distance_left;
                data->cheaty_intersect.v[0] = data->cheaty_intersect.v[0] + start->v[0];
                data->cheaty_intersect.v[1] = data->cheaty_intersect.v[1] + start->v[1];
                data->cheaty_intersect.v[2] = data->cheaty_intersect.v[2] + start->v[2];
            } else {
                data->cheaty_intersect.v[0] = finish->v[0];
                data->cheaty_intersect.v[1] = finish->v[1];
                data->cheaty_intersect.v[2] = finish->v[2];
            }
            car_to_intersect.v[0] = data->cheaty_intersect.v[0]
                - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
            car_to_intersect.v[1] = data->cheaty_intersect.v[1]
                - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
            car_to_intersect.v[2] = data->cheaty_intersect.v[2]
                - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
            distance_to_intersect = sqrtf(
                car_to_intersect.v[1] * car_to_intersect.v[1]
                + car_to_intersect.v[2] * car_to_intersect.v[2]
                + car_to_intersect.v[0] * car_to_intersect.v[0]);
            frame_period_in_secs = (double)(unsigned int)gFrame_period_for_this_munging / 1000.0 * 20.0;
            TurnOpponentPhysicsOff(pOpponent_spec);
            if (distance_to_intersect >= (double)frame_period_in_secs) {
                data->moving_to_intersect = 1;
                // v4 = sqrtf(
                //     car_to_intersect_1 * car_to_intersect_1
                //     + car_to_intersect_2 * car_to_intersect_2
                //     + car_to_intersect_0 * car_to_intersect_0);
                // if (v6 | v7) {
                //     car_to_intersect_0 = 1.0;
                //     car_to_intersect_1 = 0.0;
                //     car_to_intersect_2 = 0.0;
                // } else {
                //     v41 = v4;
                //     v8 = 1.0 / v41;
                //     v41 = v8;
                //     car_to_intersect_0 = v8 * car_to_intersect_0;
                //     car_to_intersect_1 = car_to_intersect_1 * v41;
                //     car_to_intersect_2 = car_to_intersect_2 * v41;
                // }
                BrVector3Normalise(&car_to_intersect, &car_to_intersect);
                BrVector3Scale(&car_to_intersect, &car_to_intersect, frame_period_in_secs);
                // car_to_intersect_0 = car_to_intersect_0 * frame_period_in_secs;
                // car_to_intersect_1 = car_to_intersect_1 * frame_period_in_secs;
                // car_to_intersect_2 = car_to_intersect_2 * frame_period_in_secs;
                pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0]
                    + car_to_intersect.v[0];
                pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1]
                    + car_to_intersect.v[1];
                pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2]
                    + car_to_intersect.v[2];
                return 0;
            } else {
                data->moving_to_intersect = 0;
                pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] = data->cheaty_intersect.v[0];
                pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] = data->cheaty_intersect.v[1];
                pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] = data->cheaty_intersect.v[2];
                return 0;
            }
        }
        p.v[0] = finish->v[0] - start->v[0];
        p.v[1] = finish->v[1] - start->v[1];
        p.v[2] = finish->v[2] - start->v[2];
        PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &p);
        a.v[0] = finish->v[0] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
        a.v[1] = finish->v[1] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
        a.v[2] = finish->v[2] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
        distance_to_end = BrVector3Length(&a); // sqrtf(.v[0] * a_vec11 + a_vec12 * a_vec12 + a_vec10 * a_vec10);
        dr_dprintf("%s: Rematerialising from FollowCheatyPath()...", pOpponent_spec->car_spec->driver_name);
        if (data->section_no < 0) {
            PDEnterDebugger("No useful section number in ProcessCheatyPath()");
        }
        float v10;
        int v9;
        if (GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1) >= 255) {
            if (GetOpponentsSectionMinSpeed(pOpponent_spec, data->section_no, 1) <= 0) {
                v10 = distance_to_end * 2.0;
                if (v10 >= 7.0) {
                    v10 = 7.0;
                }
                if (v10 <= 1.0) {
                    v10 = 1.0;
                }
                desired_speed_BRU = v10;
                v9 = RematerialiseOpponentOnNearestSection(pOpponent_spec, desired_speed_BRU);
            } else {
                section_min = (double)GetOpponentsSectionMinSpeed(pOpponent_spec, data->section_no, 1) / 6.9;
                v9 = RematerialiseOpponentOnNearestSection(pOpponent_spec, section_min);
            }
        } else {
            section_max = (double)GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1) / 6.9;
            v9 = RematerialiseOpponentOnNearestSection(pOpponent_spec, section_max);
        }
        if (v9) {
            pOpponent_spec->car_spec->brake_force = 0.0;
            pOpponent_spec->car_spec->acc_force = 0.0;
            if (distance_to_end >= 5.0) {
                pOpponent_spec->car_spec->acc_force = pOpponent_spec->car_spec->M / 2.0;
            } else {
                pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->M * 15.0;
            }
            return 0;
        }
        data->cheating = 1;
    }
    frame_period_in_secs = (double)(unsigned int)gFrame_period_for_this_munging / 1000.0 * 20.0;
    if (data->moving_to_intersect) {
        car_to_intersect.v[0] = data->cheaty_intersect.v[0] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
        car_to_intersect.v[1] = data->cheaty_intersect.v[1] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
        car_to_intersect.v[2] = data->cheaty_intersect.v[2] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
        // v11 = sqrtf(
        //     car_to_intersect_1 * car_to_intersect_1
        //     + car_to_intersect_2 * car_to_intersect_2
        //     + car_to_intersect_0 * car_to_intersect_0);
        distance_to_intersect = BrVector3Length(&car_to_intersect);
        if (distance_to_intersect < frame_period_in_secs) {
            data->moving_to_intersect = 0;
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] = data->cheaty_intersect.v[0];
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] = data->cheaty_intersect.v[1];
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] = data->cheaty_intersect.v[2];
        } else {
            // v14 = sqrtf(
            //     car_to_intersect_1 * car_to_intersect_1
            //     + car_to_intersect_2 * car_to_intersect_2
            //     + car_to_intersect_0 * car_to_intersect_0);
            // if (v16 | v17) {
            //     car_to_intersect_0 = 1.0;
            //     car_to_intersect_1 = 0.0;
            //     car_to_intersect_2 = 0.0;
            // } else {
            //     v39 = v14;
            //     v18 = 1.0 / v39;
            //     v39 = v18;
            //     car_to_intersect_0 = v18 * car_to_intersect_0;
            //     car_to_intersect_1 = car_to_intersect_1 * v39;
            //     car_to_intersect_2 = car_to_intersect_2 * v39;
            // }
            BrVector3Normalise(&car_to_intersect, &car_to_intersect);
            BrVector3Scale(&car_to_intersect, &car_to_intersect, frame_period_in_secs);
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0]
                + car_to_intersect.v[0];
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1]
                + car_to_intersect.v[1];
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2]
                + car_to_intersect.v[2];
        }
        return 0;
    }

    p.v[0] = finish->v[0] - start->v[0];
    p.v[1] = finish->v[1] - start->v[1];
    p.v[2] = finish->v[2] - start->v[2];
    // v19 = sqrtf(p.v[2] * p.v[2] + p.v[1] * p.v[1] + p.v[0] * p.v[0]);
    // if (v21 | v22) {
    //     p.v[0] = 1.0;
    //     p.v[1] = 0.0;
    //     p.v[2] = 0.0;
    // } else {
    //     v37 = v19;
    //     v23 = 1.0 / v37;
    //     v37 = v23;
    //     p.v[0] = v23 * p.v[0];
    //     p.v[1] = p.v[1] * v37;
    //     p.v[2] = p.v[2] * v37;
    // }
    BrVector3Normalise(&p, &p);
    while (frame_period_in_secs > 0.0) {
        a.v[0] = finish->v[0] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
        a.v[1] = finish->v[1] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
        a.v[2] = finish->v[2] - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
        distance_to_end = BrVector3Length(&a);
        if (distance_to_end < frame_period_in_secs) {
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0]
                + a.v[0];
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1]
                + a.v[1];
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2]
                + a.v[2];
            frame_period_in_secs = frame_period_in_secs - distance_to_end;

            dr_dprintf("%s: ProcessFollowPath() - current section %d(#%d) (cheating)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            data->section_no = GetOpponentsNextSection(pOpponent_spec, data->section_no);
            if (data->section_no == -1) {
                pOpponent_spec->car_spec->acc_force = 0.0;
                pOpponent_spec->car_spec->brake_force = 0.0;
                dr_dprintf("%s: ProcessFollowPath() - reached end of path while cheating", pOpponent_spec->car_spec->driver_name);
                return 1;
            }
            dr_dprintf("%s: ProcessFollowPath() - next section %d(#%d) (cheating)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            start = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no);
            finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no);
            p.v[0] = finish->v[0] - start->v[0];
            p.v[1] = finish->v[1] - start->v[1];
            p.v[2] = finish->v[2] - start->v[2];
            BrVector3Normalise(&p, &p);
        } else {
            p.v[0] = p.v[0] * frame_period_in_secs;
            p.v[1] = p.v[1] * frame_period_in_secs;
            p.v[2] = p.v[2] * frame_period_in_secs;
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0]
                + p.v[0];
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1]
                + p.v[1];
            pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2]
                + p.v[2];
            frame_period_in_secs = 0.0;
        }
    }
    return 0;
}
