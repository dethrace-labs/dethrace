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
    if (pCommand == ePOC_run) {

        if (pOpponent_spec->follow_path_data.cheating || pOpponent_spec->cheating) {
            return FollowCheatyPath(pOpponent_spec);
        }
        if (!pIgnore_end && !data->made_it && data->borrowed_time_start + 1000 < gTime_stamp_for_this_munging && gTime_stamp_for_this_munging < data->borrowed_time_start + 10000) {

            // float v59, v58, v57, v55, v54;
            // br_vector3 v56;

            // v59 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0];
            // section_dir.v[0] = v59 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0];
            // v58 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[1];
            // section_dir.v[1] = v58 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1];
            // v57 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2];
            // section_dir.v[2] = v57 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2];

            BrVector3Sub(&section_dir, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));

            // v56.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
            // goal_dir.v[0] = v56.v[0] - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0];
            // v55 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
            // goal_dir.v[1] = v55 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1];
            // v54 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
            // goal_dir.v[2] = v54 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2];

            BrVector3Sub(&goal_dir, &car_master_actor->t.t.translate.t, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));

            dist_along = BrVector3LengthSquared(&goal_dir) / BrVector3LengthSquared(&section_dir);
            //  (goal_dir.v[1] * section_dir.v[1]
            //                  + goal_dir.v[2] * section_dir.v[2]
            //                  + goal_dir.v[0] * section_dir.v[0])
            //     / (section_dir.v[1] * section_dir.v[1]
            //         + section_dir.v[2] * section_dir.v[2]
            //         + section_dir.v[0] * section_dir.v[0]);
            // section_v.v[0] = section_dir.v[0] * dist_along;
            // section_v.v[1] = section_dir.v[1] * dist_along;
            // section_v.v[2] = section_dir.v[2] * dist_along;
            BrVector3Scale(&section_v, &section_dir, dist_along);
            // wank.v[0] = goal_dir.v[0] - section_v.v[0];
            // wank.v[1] = goal_dir.v[1] - section_v.v[1];
            // wank.v[2] = goal_dir.v[2] - section_v.v[2];
            BrVector3Sub(&wank, &goal_dir, &section_v);
            goal_width = BrVector3Length(&wank); // sqrtf(wank.v[2] * wank.v[2] + wank.v[1] * wank.v[1] + wank.v[0] * wank.v[0]);
            if (GetOpponentsSectionWidth(pOpponent_spec, data->section_no) >= goal_width) {
                data->made_it = 1;
            }
        }
        if (data->borrowed_time_start + 10000 < gTime_stamp_for_this_munging && !data->made_it) {
            dr_dprintf("%s: ProcessFollowPath() giving up due to not making the corner", pOpponent_spec->car_spec->driver_name);
            return eFPR_given_up;
        }
        car_spec->keys.acc = 1;
        speed = BrVector3Length(&car_spec->v);
        //  sqrtf(
        //     car_spec->v.v[2] * car_spec->v.v[2]
        //     + car_spec->v.v[1] * car_spec->v.v[1]
        //     + car_spec->v.v[0] * car_spec->v.v[0]);

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
            car_spec->brake_force = 0.0f;
            car_spec->acc_force = 0.0f;
        } else {
            if (pIgnore_end) {
                stopped_speed = 0.07f;
            } else {
                stopped_speed = 0.2f;
            }
            if (!pNever_struggle && stopped_speed >= speed && data->last_finished_struggle_time + 2000 < gTime_stamp_for_this_munging && (pPursuit_mode || data->has_moved_during_this_task != 0)) {
                dr_dprintf("%s: 'Stopped,' section #%d, speed = %.2f m/s, about to start a-strugglin'", pOpponent_spec->car_spec->driver_name, data->section_no, speed);
                data->struggle_time = gTime_stamp_for_this_munging;
                if (pIgnore_end || data->section_no != data->last_struggle_section) {
                    data->last_struggle_section = data->section_no;
                    data->number_of_struggles = 0;
                } else {
                    if (data->number_of_struggles >= 3) {
                        car_spec->acc_force = 0.0;
                        car_spec->brake_force = 0.0;
                        dr_dprintf("%s: Giving up trying to follow path 'cos we've struggled too much", pOpponent_spec->car_spec->driver_name);
                        return eFPR_given_up;
                    }
                    data->number_of_struggles++;
                }
            }
        }
        // car_to_end.v[0] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0]
        //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
        // car_to_end.v[1] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[1]
        //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
        // car_to_end.v[2] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2]
        //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];

        BrVector3Sub(&car_to_end, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        car_to_end.v[1] = 0.0;
        dist_to_end = BrVector3Length(&car_to_end) * WORLD_SCALE;
        dist_to_goal = dist_to_end;
        if (dist_to_end > 15.0f) {
            // float v53_2 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0];
            // wank.v[0] = v53_2 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0];
            // float v52_1 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[1];
            // wank.v[1] = v52_1 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1];
            // float v51_0 = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2];
            // wank.v[2] = v51_0 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2];
            BrVector3Sub(&wank, GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no), GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));

            BrVector3Normalise(&a, &wank);
            // temp_2 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
            // wank.v[0] = temp_2 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0];
            // temp_1 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
            // wank.v[1] = temp_1 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1];
            // temp_0 = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
            // wank.v[2] = temp_0 - GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2];

            BrVector3Sub(&wank, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no));

            dot_a = BrVector3Dot(&a, &wank); // v62.v[2] * wank.v[2] + v62.v[1] * wank.v[1] + v62.v[0] * wank.v[0];
            wank2.v[0] = a.v[0] * dot_a;
            wank2.v[2] = a.v[2] * dot_a;
            car_to_end.v[0] = wank2.v[0] - wank.v[0];
            car_to_end.v[2] = wank2.v[2] - wank.v[2];
            car_to_end.v[1] = 0.0;
            // float v16 = BrVector3Length(&car_to_end); // sqrtf(car_to_end.v[0] * car_to_end.v[0] + 0.0 * 0.0 + car_to_end.v[2] * car_to_end.v[2]);
            dist_to_end = BrVector3Length(&car_to_end) * WORLD_SCALE;
            if (dist_to_end < 15.0f) {
                t = sqrtf(225.0 - dist_to_end * dist_to_end) / WORLD_SCALE;
                if (t + dot_a >= 0.0) {
                    wank.v[0] = a.v[0] * t;
                    wank.v[2] = a.v[2] * t;
                    wank.v[1] = 0.0;
                    BrVector3Accumulate(&car_to_end, &wank);
                    // car_to_end.v[0] = wank.v[0] + car_to_end.v[0];
                    // car_to_end.v[1] = car_to_end.v[1] + 0.0;
                    // car_to_end.v[2] = wank.v[2] + car_to_end.v[2];
                } else {
                    // car_to_end.v[0] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0]
                    //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
                    // car_to_end.v[1] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1]
                    //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
                    // car_to_end.v[2] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2]
                    //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
                    BrVector3Sub(&car_to_end, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no), &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                    dist_to_end = BrVector3Length(&car_to_end) * WORLD_SCALE;
                    //  sqrtf(
                    //                   car_to_end.v[0] * car_to_end.v[0]
                    //                   + car_to_end.v[1] * car_to_end.v[1]
                    //                   + car_to_end.v[2] * car_to_end.v[2])
                    //     * 6.9;
                    BrVector3Scale(&car_to_end, &car_to_end, 15.0f / dist_to_end);
                    // car_to_end.v[0] = 15.0 / dist_to_end * car_to_end.v[0];
                    // car_to_end.v[1] = 15.0 / dist_to_end * car_to_end.v[1];
                    // car_to_end.v[2] = 15.0 / dist_to_end * car_to_end.v[2];
                }
                dist_to_end = 15.0f;
            } else if (dot_a < 0.0f) {
                // car_to_end.v[0] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[0]
                //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
                // car_to_end.v[1] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[1]
                //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
                // car_to_end.v[2] = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no)->v[2]
                //     - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];
                // dist_to_end = sqrtf(
                //                   car_to_end.v[0] * car_to_end.v[0]
                //                   + car_to_end.v[1] * car_to_end.v[1]
                //                   + car_to_end.v[2] * car_to_end.v[2])
                //     * 6.9;
                // car_to_end.v[0] = 15.0 / dist_to_end * car_to_end.v[0];
                // car_to_end.v[1] = 15.0 / dist_to_end * car_to_end.v[1];
                // car_to_end.v[2] = 15.0 / dist_to_end * car_to_end.v[2];

                BrVector3Sub(&car_to_end, GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no), &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                dist_to_end = BrVector3Length(&car_to_end) * WORLD_SCALE;
                BrVector3Scale(&car_to_end, &car_to_end, 15.0f / dist_to_end);
            }
        }
        // float v20 = car_spec->v.v[1] * car_to_end.v[1]
        //     + car_spec->v.v[2] * car_to_end.v[2]
        //     + car_spec->v.v[0] * car_to_end.v[0];
        // v94 = v20 / sqrtf(v20);
        section_width = GetOpponentsSectionWidth(pOpponent_spec, data->section_no) * WORLD_SCALE;
        if (!pIgnore_end && speed * 1.5f > dist_to_goal) {
            dr_dprintf("%s: ProcessFollowPath() - *** CHANGING SECTIONS ***", pOpponent_spec->car_spec->driver_name);
            dr_dprintf("%s: ProcessFollowPath() - current section %d(#%d)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            data->section_no = GetOpponentsNextSection(pOpponent_spec, data->section_no);
            if (data->section_no == -1) {
                car_spec->acc_force = 0.0f;
                car_spec->brake_force = 0.0f;
                dr_dprintf("%s: ProcessFollowPath() - reached end of path", pOpponent_spec->car_spec->driver_name);
                return eFPR_end_of_path;
            }
            car_to_end.v[1] = 0.0f;
            real_section_no = GetOpponentsRealSection(pOpponent_spec, data->section_no);
            dr_dprintf("%s: ProcessFollowPath() - next section %d(#%d)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
            data->last_struggle_section = -1;
            data->borrowed_time_start = gTime_stamp_for_this_munging;
            data->made_it = 1;
            data->last_distance = 0.0f;
        }
        not_our_dir = (br_vector3*)&car_master_actor->t.t.mat.m[2];

        wank.v[0] = car_master_actor->t.t.mat.m[2][2] * car_to_end.v[1] - car_master_actor->t.t.mat.m[2][1] * car_to_end.v[2];
        wank.v[1] = car_master_actor->t.t.mat.m[2][0] * car_to_end.v[2] - car_master_actor->t.t.mat.m[2][2] * car_to_end.v[0];
        wank.v[2] = car_master_actor->t.t.mat.m[2][1] * car_to_end.v[0] - car_master_actor->t.t.mat.m[2][0] * car_to_end.v[1];

        BrVector3Cross(&wank, &car_to_end, not_our_dir);

        radius = GetOpponentsSectionWidth(pOpponent_spec, data->section_no) * 0.5f;
        just_fucking_brake = 0;
        dot_d = BrVector3Dot(&car_to_end, not_our_dir);
        if (dot_d <= 0.0f || speed <= 10.0f) {
            if ((wank.v[1] <= 0.0f || dot_d <= 0.0f) && GetOpponentsSectionWidth(pOpponent_spec, data->section_no) >= wank.v[1]) {
                if ((wank.v[1] >= 0.0 || dot_d <= 0.0) && -GetOpponentsSectionWidth(pOpponent_spec, data->section_no) <= wank.v[1]) {
                    if (wank.v[1] <= radius) {
                        if (-radius <= wank.v[1]) {
                            car_spec->curvature = 0.0f;
                        } else {
                            car_spec->curvature = -(MaxCurvatureForCarSpeed(car_spec, speed) * 0.05f);
                        }
                        data->desired_speed = 80.0f;
                    } else {
                        car_spec->curvature = MaxCurvatureForCarSpeed(car_spec, speed) * 0.05f;
                        data->desired_speed = 80.0f;
                    }
                } else {
                    car_spec->curvature = -MaxCurvatureForCarSpeed(car_spec, speed);
                    data->desired_speed = 6.0f;
                }
            } else {
                car_spec->curvature = MaxCurvatureForCarSpeed(car_spec, speed);
                data->desired_speed = 6.0f;
            }
        } else {
            data->desired_speed = 6.0f;
            car_spec->curvature = 0.0f;
            just_fucking_brake = 1;
        }
        if (just_fucking_brake) {
            car_spec->brake_force = car_spec->M * 15.0f;
            car_spec->acc_force = 0.0;
        } else {
            if (GetOpponentsNextSection(pOpponent_spec, data->section_no) != -1) {
                next_turning_radius = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[0] * -wank.v[2] + pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[2] * wank.v[0];
                next_corner_size = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[0] * -wank.v[2];
                next_turning_radius = next_turning_radius - (GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no)->v[2] * wank.v[0] + next_corner_size);
                float v104 = -wank.v[2] * not_our_dir->v[0] + not_our_dir->v[2] * wank.v[0];
                if (v104 * next_turning_radius > 0.0f) {
                    goal_width = 0.0f;
                    speed2d = speed * speed / 24.0f + speed * 1.5f;
                    p = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t;
                    // start.v[0] = -(next_turning_radius / v104) * not_our_dir->v[0];
                    // start.v[1] = -(next_turning_radius / v104) * not_our_dir->v[1];
                    // start.v[2] = -(next_turning_radius / v104) * not_our_dir->v[2];
                    BrVector3Scale(&start, not_our_dir, -(next_turning_radius / v104));
                    // start.v[0] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0] + start.v[0];
                    // start.v[1] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] + start.v[1];
                    // start.v[2] = pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2] + start.v[2];
                    BrVector3Accumulate(&start, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
                    section_no = data->section_no;
                    for (sx = 0; GetOpponentsNextSection(pOpponent_spec, section_no) != -1; sx++) {
                        if (sx >= 4) {
                            break;
                        }
                        // int nxt_section_temp = GetOpponentsNextSection(pOpponent_spec, section_no);
                        // next.v[0] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, nxt_section_temp)->v[0];
                        // // nxt_section_temp = GetOpponentsNextSection(pOpponent_spec, section_no);
                        // next.v[1] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, nxt_section_temp)->v[1];
                        // // nxt_section_temp = GetOpponentsNextSection(pOpponent_spec, section_no);
                        // next.v[2] = GetOpponentsSectionFinishNodePoint(pOpponent_spec, nxt_section_temp)->v[2];
                        // // nxt_section_temp = GetOpponentsNextSection(pOpponent_spec, data->section_no);
                        BrVector3Copy(&next, GetOpponentsSectionFinishNodePoint(pOpponent_spec, GetOpponentsNextSection(pOpponent_spec, section_no)));
                        next_width = GetOpponentsSectionWidth(pOpponent_spec, GetOpponentsNextSection(pOpponent_spec, section_no));
                        width = GetOpponentsSectionWidth(pOpponent_spec, data->section_no);
                        StraightestArcForCorner(
                            &corner_speed,
                            &corner_speed2,
                            &dot_a,
                            &p,
                            &start,
                            &next,
                            &p,
                            &start,
                            width,
                            next_width);
                        dot_a *= WORLD_SCALE;
                        goal_width = dot_a + goal_width;
                        if (goal_width > speed2d) {
                            break;
                        }
                        corner_speed *= WORLD_SCALE;
                        corner_speed2 *= WORLD_SCALE;
                        stopping_distance = CornerFudge(car_spec) * CornerFudge(car_spec) * (corner_speed * 10.0);
                        desired_speed = sqrtf(stopping_distance);
                        if (GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1) < desired_speed) {
                            desired_speed = GetOpponentsSectionMaxSpeed(pOpponent_spec, data->section_no, 1);
                            stopping_distance = desired_speed * desired_speed;
                        }
                        if (goal_width - corner_speed2 < (speed * speed - stopping_distance) / 24.0f + desired_speed * 1.5f && data->desired_speed > desired_speed) {
                            data->desired_speed = desired_speed;
                        }
                        p = start;
                        start = next;
                        section_no = GetOpponentsNextSection(pOpponent_spec, section_no);
                    }
                }
            }
            // if ((car_spec->car_ID & 0xFF00) == 768) {
            //     effective_speed_factor = gCop_speed_factor;
            // } else {
            //     effective_speed_factor = gOpponent_speed_factor;
            // }
            effective_speed_factor = CAR_SPEC_GET_SPEED_FACTOR(car_spec);
            // if (effective_speed_factor >= 1.0) {
            //     acc_factor = effective_speed_factor;
            // } else {
            //     acc_factor = 1.0;
            // }
            acc_factor = MAX(1.0f, effective_speed_factor);
            if (engine_damage <= 50 || engine_damage >= 98) {
                if (engine_damage >= 98) {
                    acc_factor -= 0.6f;
                }
            } else {
                acc_factor -= (engine_damage - 50) * 0.0125f;
            }
            if (trans_damage <= 50 || trans_damage >= 98) {
                if (trans_damage >= 98) {
                    acc_factor -= 0.3f;
                }
            } else {
                acc_factor -= (trans_damage - 50) * 0.00625f;
            }
            if (engine_damage >= 99 || trans_damage >= 99) {
                acc_factor = 0.0f;
            }
            max_acc = acc_factor * 10.0f;
            // TODO: ?? corner_speed2[10] = v30;
            error = data->desired_speed * effective_speed_factor - speed;
            acc = (error - data->prev_acc_error) * 1000.0f / gFrame_period_for_this_munging * 0.1f + error + data->prev_acc;
            if (acc > max_acc) {
                acc = max_acc;
            }
            if (acc < -max_acc) {
                acc = -max_acc;
            }
            data->prev_acc = acc;
            data->prev_acc_error = error;
            acc = car_spec->M * acc;
            if (acc <= 0.0f) {
                car_spec->acc_force = 0.0f;
                car_spec->brake_force = -acc;
            } else {
                car_spec->acc_force = acc;
                car_spec->brake_force = 0.0f;
            }
        }
        return eFPR_OK;
    } else if (pCommand == ePOC_start) {
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

    BrFatal("C:\\Msdev\\Projects\\DethRace\\OPPOPROC.C", 1420, "C:\\Msdev\\Projects\\DethRace\\OPPOPROC.C line %d", 140);
    return 0;
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

    data = &pOpponent_spec->follow_path_data;
    start = GetOpponentsSectionStartNodePoint(pOpponent_spec, pOpponent_spec->follow_path_data.section_no);
    finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, pOpponent_spec->follow_path_data.section_no);
    if ((pOpponent_spec->follow_path_data.cheating ^ pOpponent_spec->cheating) != 0) {
        data->cheating = pOpponent_spec->cheating;
        if (data->cheating) {
            dr_dprintf("%s: Dematerialising", pOpponent_spec->car_spec->driver_name);
            BrVector3Sub(&section_v, finish, start);
            BrVector3Sub(&car_to_end, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, start);
            distance_left = BrVector3Dot(&section_v, &car_to_end) / BrVector3LengthSquared(&section_v);
            if (distance_left < 0.0f) {
                BrVector3Copy(&data->cheaty_intersect, start);
            } else if (distance_left <= 1.0) {
                BrVector3Scale(&data->cheaty_intersect, &section_v, distance_left);
                BrVector3Accumulate(&data->cheaty_intersect, start);
            } else {
                BrVector3Copy(&data->cheaty_intersect, finish);
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
            desired_speed_BRU = MIN(7.0f, MAX(1.0f, distance_to_end * 2.0));
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
    while (frame_period_in_secs > 0.0) {
        BrVector3Sub(&a, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        distance_to_end = BrVector3Length(&a);
        if (distance_to_end < frame_period_in_secs) {
            BrVector3Accumulate(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &a);
            frame_period_in_secs = frame_period_in_secs - distance_to_end;
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
