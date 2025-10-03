#include "opponent.h"
#include "brender.h"
#include "car.h"
#include "controls.h"
#include "crush.h"
#include "displays.h"
#include "errors.h"
#include "finteray.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrme.h"
#include "globvrpb.h"
#include "harness/trace.h"
#include "loading.h"
#include "oppoproc.h"
#include "pd/sys.h"
#include "skidmark.h"
#include "trig.h"
#include "utility.h"

#include <float.h>
#include <stdlib.h>

// GLOBAL: CARM95 0x005070d0
br_actor* gOppo_path_actor = NULL;

// GLOBAL: CARM95 0x005070d4
br_model* gOppo_path_model;

// GLOBAL: CARM95 0x005070d8
br_material* gMat_dk_yel;

// GLOBAL: CARM95 0x005070dc
br_material* gMat_md_yel;

// GLOBAL: CARM95 0x005070e0
br_material* gMat_lt_yel;

// GLOBAL: CARM95 0x005070e4
br_material* gMat_dk_red;

// GLOBAL: CARM95 0x005070e8
br_material* gMat_lt_red;

// GLOBAL: CARM95 0x005070ec
br_material* gMat_dk_grn;

// GLOBAL: CARM95 0x005070f0
br_material* gMat_lt_grn;

// GLOBAL: CARM95 0x005070f4
br_material* gMat_dk_blu;

// GLOBAL: CARM95 0x005070f8
br_material* gMat_lt_blu;

// GLOBAL: CARM95 0x005070fc
br_material* gMat_dk_turq;

// GLOBAL: CARM95 0x00507100
br_material* gMat_lt_turq;

// GLOBAL: CARM95 0x00507104
br_material* gMat_dk_gry;

// GLOBAL: CARM95 0x00507108
br_material* gMat_md_gry;

// GLOBAL: CARM95 0x0050710c
br_material* gMat_lt_gry;

// GLOBAL: CARM95 0x00507110
int gMellow_opponents;

// GLOBAL: CARM95 0x00507114
int gTest_toggle;

// GLOBAL: CARM95 0x00507118
int gAlready_elasticating;

// GLOBAL: CARM95 0x0050711c
int gVertices_used_in_non_edit_paths;

// GLOBAL: CARM95 0x00507120
int gFaces_used_in_non_edit_paths;

// GLOBAL: CARM95 0x00507124
int gMats_allocated;

// GLOBAL: CARM95 0x00507128
int gOppo_paths_shown;

// GLOBAL: CARM95 0x0050712c
int gMade_path_filename;

// GLOBAL: CARM95 0x00507130
int gBIG_APC_index = -1;

// GLOBAL: CARM95 0x00507138
char* gPath_section_type_names[3];

// GLOBAL: CARM95 0x00507144
int gMin_bangness = 100;

// GLOBAL: CARM95 0x00507148
int gMax_bangness;

// GLOBAL: CARM95 0x0050714c
tU32 gNext_elastication;

// GLOBAL: CARM95 0x00507150
tU32 gNext_write_during_elastication;

// GLOBAL: CARM95 0x00507154
char* gCop_name = "Faceless Cop";

// GLOBAL: CARM95 0x00507158
char* gDrone_name = "Innocent Civilian";

// GLOBAL: CARM95 0x0050715c
int gChallenger_index__opponent = -1; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00507160
int gSFS_count;

// GLOBAL: CARM95 0x00507164
int gSFS_total_cycles;

// GLOBAL: CARM95 0x00507168
int gSFS_max_cycles;

// GLOBAL: CARM95 0x0050716c
float gOpponent_nastyness_frigger = 1.f;

// GLOBAL: CARM95 0x00530ca8
char gOppo_path_filename[256];

// GLOBAL: CARM95 0x00530df0
br_scalar gIn_view_distance;

// GLOBAL: CARM95 0x00530de8
tU8* gBit_per_node;

// GLOBAL: CARM95 0x00530da8
int gGrudge_reduction_per_period;

// GLOBAL: CARM95 0x00530df8
int gSFS_cycles_this_time;

// GLOBAL: CARM95 0x00530dec
br_scalar gMinimum_yness_before_knackerisation;

// GLOBAL: CARM95 0x00530dd4
int gWanky_arse_tit_fuck;

// GLOBAL: CARM95 0x00530df4
br_scalar gHead_on_cos_value;

// GLOBAL: CARM95 0x00530de0
tU32 gNext_grudge_reduction;

// GLOBAL: CARM95 0x00530dd0
br_scalar gCop_pursuit_speed_percentage_multiplier;

// GLOBAL: CARM95 0x00530db8
br_scalar gDefinite_cop_pursuit_speed;

// GLOBAL: CARM95 0x00530dd8
int gAcknowledged_start;

// GLOBAL: CARM95 0x00530ca0
int gStart_jumped;

// GLOBAL: CARM95 0x00530dc8
int gNum_of_opponents_getting_near;

// GLOBAL: CARM95 0x00530dcc
int gNumber_of_cops_before_faffage;

// GLOBAL: CARM95 0x00530dc0
int gFirst_frame;

// GLOBAL: CARM95 0x00530dac
tU32 gAcme_frame_count;

// GLOBAL: CARM95 0x00530dbc
br_scalar gDefinite_no_cop_pursuit_speed;

// GLOBAL: CARM95 0x00530de4
int gNum_of_opponents_completing_race;

// GLOBAL: CARM95 0x00530c98
int gNum_of_opponents_pursuing;

// GLOBAL: CARM95 0x00530dc4
int gActive_car_list_rebuild_required;

// GLOBAL: CARM95 0x00530db4
br_scalar gFrame_period_for_this_munging_in_secs;

// GLOBAL: CARM95 0x00530c94
int gBig_bang;

// GLOBAL: CARM95 0x00530ddc
int gProcessing_opponents;

// GLOBAL: CARM95 0x005520e0
tU32 gFrame_period_for_this_munging;

// GLOBAL: CARM95 0x005520e4
tU32 gTime_stamp_for_this_munging;

// GLOBAL: CARM95 0x00530c90
tS16 gMobile_section;

// IDA: void __usercall PointActorAlongThisBloodyVector(br_actor *pThe_actor@<EAX>, br_vector3 *pThe_vector@<EDX>)
// FUNCTION: CARM95 0x00402390
void PointActorAlongThisBloodyVector(br_actor* pThe_actor, br_vector3* pThe_vector) {
    br_transform trans;

    trans.type = BR_TRANSFORM_LOOK_UP;
    BrVector3Copy(&trans.t.look_up.t, &pThe_actor->t.t.translate.t);
    BrVector3Copy(&trans.t.look_up.look, pThe_vector);
    BrVector3Set(&trans.t.look_up.up, 0.f, 1.f, 0.f);

    BrTransformToTransform(&pThe_actor->t, &trans);
}

// IDA: void __usercall ProcessCurrentObjective(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x004065e0
void ProcessCurrentObjective(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {

    switch (pOpponent_spec->current_objective) {
    case eOOT_complete_race:
        ProcessCompleteRace(pOpponent_spec, pCommand);
        break;
    case eOOT_pursue_and_twat:
        ProcessPursueAndTwat(pOpponent_spec, pCommand);
        break;
    case eOOT_run_away:
        ProcessRunAway(pOpponent_spec, pCommand);
        break;
    case eOOT_get_near_player:
        ProcessGetNearPlayer(pOpponent_spec, pCommand);
        break;
    case eOOT_levitate:
        ProcessLevitate(pOpponent_spec, pCommand);
        break;
    case eOOT_knackered_and_freewheeling:
        memset(&pOpponent_spec->car_spec->keys, 0, sizeof(pOpponent_spec->car_spec->keys));
        pOpponent_spec->car_spec->acc_force = 0.f;
        pOpponent_spec->car_spec->brake_force = 0.f;
        pOpponent_spec->car_spec->curvature = 0.f;
        break;
    case eOOT_frozen:
        ProcessFrozen(pOpponent_spec, pCommand);
        break;
    case eOOT_wait_for_some_hapless_sod:
        ProcessWaitForSomeHaplessSod(pOpponent_spec, pCommand);
        break;
    case eOOT_return_to_start:
        ProcessReturnToStart(pOpponent_spec, pCommand);
        break;
    default:
        break;
    }
}

// IDA: tS16 __usercall ReallocExtraPathNodes@<AX>(int pHow_many_then@<EAX>)
// FUNCTION: CARM95 0x004057a1
tS16 ReallocExtraPathNodes(int pHow_many_then) {
    tPath_node* new_nodes;
    tS16 first_new_node;

    first_new_node = -1;
    if (pHow_many_then != 0) {
        first_new_node = gProgram_state.AI_vehicles.number_of_path_nodes;
        new_nodes = BrMemAllocate(sizeof(tPath_node) * (pHow_many_then + gProgram_state.AI_vehicles.number_of_path_nodes), kMem_oppo_new_nodes);
        memcpy(new_nodes, gProgram_state.AI_vehicles.path_nodes, sizeof(tPath_node) * gProgram_state.AI_vehicles.number_of_path_nodes);
        if (gProgram_state.AI_vehicles.path_nodes != NULL) {
            BrMemFree(gProgram_state.AI_vehicles.path_nodes);
        }
        gProgram_state.AI_vehicles.number_of_path_nodes += pHow_many_then;
        gProgram_state.AI_vehicles.path_nodes = new_nodes;
    }
    dr_dprintf(
        "ReallocExtraPathNodes(): Allocated %d bytes for %d path nodes",
        sizeof(tPath_node) * (pHow_many_then + gProgram_state.AI_vehicles.number_of_path_nodes),
        pHow_many_then);
    return first_new_node;
}

// IDA: tS16 __usercall ReallocExtraPathSections@<AX>(int pHow_many_then@<EAX>)
// FUNCTION: CARM95 0x00405855
tS16 ReallocExtraPathSections(int pHow_many_then) {
    tPath_section* new_sections;
    tS16 first_new_section;

    first_new_section = -1;
    if (pHow_many_then != 0) {
        first_new_section = gProgram_state.AI_vehicles.number_of_path_sections;
        new_sections = BrMemAllocate(sizeof(tPath_section) * (pHow_many_then + gProgram_state.AI_vehicles.number_of_path_sections), kMem_oppo_new_sections);
        memcpy(new_sections, gProgram_state.AI_vehicles.path_sections, sizeof(tPath_section) * gProgram_state.AI_vehicles.number_of_path_sections);
        if (gProgram_state.AI_vehicles.path_sections != NULL) {
            BrMemFree(gProgram_state.AI_vehicles.path_sections);
        }
        gProgram_state.AI_vehicles.number_of_path_sections += pHow_many_then;
        gProgram_state.AI_vehicles.path_sections = new_sections;
    }
    dr_dprintf(
        "ReallocExtraPathSections(): Allocated %d bytes for %d path sections",
        sizeof(tPath_section) * (pHow_many_then + gProgram_state.AI_vehicles.number_of_path_sections),
        pHow_many_then);
    return first_new_section;
}

// IDA: int __usercall PointVisibleFromHere@<EAX>(br_vector3 *pFrom@<EAX>, br_vector3 *pTo@<EDX>)
// FUNCTION: CARM95 0x004063ac
int PointVisibleFromHere(br_vector3* pFrom, br_vector3* pTo) {
    br_vector3 from;
    br_vector3 dir;
    br_vector3 norm;
    br_scalar t;
    br_material* material;

    BrVector3Sub(&dir, pTo, pFrom);
    BrVector3Copy(&from, pFrom);
    from.v[1] += 0.15f;
    dir.v[1] += 0.15f;
    FindFace(&from, &dir, &norm, &t, &material);
    return t > 1.0f;
}

// IDA: tS16 __usercall FindNearestPathNode@<AX>(br_vector3 *pActor_coords@<EAX>, br_scalar *pDistance@<EDX>)
// FUNCTION: CARM95 0x0040f331
tS16 FindNearestPathNode(br_vector3* pActor_coords, br_scalar* pDistance) {
    int i;
    tS16 nearest_node;
    br_scalar distance;
    br_vector3 actor_to_node;

    nearest_node = -1;
    *pDistance = FLT_MAX;
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_path_nodes; i++) {
        BrVector3Sub(&actor_to_node, &gProgram_state.AI_vehicles.path_nodes[i].p, pActor_coords);
        distance = BrVector3Length(&actor_to_node);
        if (distance < *pDistance) {
            if (!gAlready_elasticating || gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1] != i) {
                *pDistance = distance;
                nearest_node = i;
            }
        }
    }
    return nearest_node;
}

// IDA: tS16 __usercall FindNearestPathSection@<AX>(br_vector3 *pActor_coords@<EAX>, br_vector3 *pPath_direction@<EDX>, br_vector3 *pIntersect@<EBX>, br_scalar *pDistance@<ECX>)
// FUNCTION: CARM95 0x0040294b
tS16 FindNearestPathSection(br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance) {

    return FindNearestGeneralSection(NULL, pActor_coords, pPath_direction, pIntersect, pDistance);
}

// IDA: tS16 __usercall FindNearestGeneralSection@<AX>(tCar_spec *pPursuee@<EAX>, br_vector3 *pActor_coords@<EDX>, br_vector3 *pPath_direction@<EBX>, br_vector3 *pIntersect@<ECX>, br_scalar *pDistance)
// FUNCTION: CARM95 0x00402975
tS16 FindNearestGeneralSection(tCar_spec* pPursuee, br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance) {
    int section_no;
    int no_sections;
    tS16 nearest_node_section_no;
    tS16 nearest_section;
    br_scalar nearest_node_distance_squared;
    br_scalar closest_distance_squared;
    br_scalar the_distance_squared;
    br_scalar t;
    br_scalar length_squared_a;
    br_vector3 a;
    br_vector3 p;
    br_vector3 wank;
    br_vector3 intersect;
    br_vector3* start;
    br_vector3* finish;
    br_vector3* nearest_node_v;
#if defined(DETHRACE_FIX_BUGS)
    br_vector3 zero_vector;
#endif

    nearest_node_section_no = -1;
    nearest_section = -1;
    nearest_node_distance_squared = BR_SCALAR_MAX;
    closest_distance_squared = BR_SCALAR_MAX;
#if defined(DETHRACE_FIX_BUGS)
    BrVector3Set(&zero_vector, 0.f, 0.f, 0.f);
    nearest_node_v = &zero_vector;
#endif

    if (pPursuee != NULL) {
        no_sections = pPursuee->my_trail.number_of_nodes - 1;
    } else {
        no_sections = gProgram_state.AI_vehicles.number_of_path_sections;
    }

    for (section_no = 0; section_no < no_sections; section_no++) {
        if (pPursuee != NULL) {
            start = &pPursuee->my_trail.trail_nodes[section_no];
            finish = &pPursuee->my_trail.trail_nodes[section_no + 1];
        } else {
            start = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].p;
            finish = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].p;
        }
        if (!gAlready_elasticating || gMobile_section != section_no) {
            BrVector3Sub(&a, finish, start);
            BrVector3Sub(&p, pActor_coords, start);
            BrVector3Sub(&wank, &p, &a);
            the_distance_squared = BrVector3LengthSquared(&wank);
            if (the_distance_squared < nearest_node_distance_squared) {
                nearest_node_distance_squared = the_distance_squared;
                nearest_node_section_no = section_no;
                nearest_node_v = finish;
            }
            the_distance_squared = BrVector3LengthSquared(&p);
            if (the_distance_squared < nearest_node_distance_squared) {
                nearest_node_distance_squared = the_distance_squared;
                nearest_node_section_no = section_no;
                nearest_node_v = start;
            }
            length_squared_a = BrVector3LengthSquared(&a);
            if (length_squared_a < 0.0001f) {
                continue;
            }
            t = BrVector3Dot(&p, &a) / length_squared_a;
            if (t >= 0 && t <= 1.f) {
                BrVector3Scale(&intersect, &a, t);
                BrVector3Sub(&wank, &p, &intersect);
                the_distance_squared = BrVector3LengthSquared(&wank);
                if (the_distance_squared < closest_distance_squared) {
                    closest_distance_squared = the_distance_squared;
                    nearest_section = section_no;
                    BrVector3Add(pIntersect, start, &intersect);
                    BrVector3NormaliseQuick(pPath_direction, &a);
                }
            }
        }
    }
    if (closest_distance_squared > nearest_node_distance_squared) {
        nearest_section = nearest_node_section_no;
        if (pPursuee != NULL) {
            start = &pPursuee->my_trail.trail_nodes[nearest_node_section_no];
            finish = &pPursuee->my_trail.trail_nodes[nearest_node_section_no + 1];
        } else {
            start = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[nearest_node_section_no].node_indices[0]].p;
            finish = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[nearest_node_section_no].node_indices[1]].p;
        }
        BrVector3Sub(&a, finish, start);
        BrVector3NormaliseQuick(pPath_direction, &a);
        BrVector3Copy(pIntersect, nearest_node_v);
        *pDistance = sqrt(nearest_node_distance_squared);
    } else {
        *pDistance = sqrt(closest_distance_squared);
    }
    if (pPursuee != NULL) {
        nearest_section += 15000;
    }
    return nearest_section;
}

// IDA: void __usercall DeadStopCar(tCar_spec *pCar_spec@<EAX>)
// FUNCTION: CARM95 0x00402481
void DeadStopCar(tCar_spec* pCar_spec) {

    pCar_spec->acc_force = 0.f;
    pCar_spec->brake_force = 0.f;
    pCar_spec->curvature = 0.f;
    pCar_spec->gear = 0;
    pCar_spec->revs = 0.f;
    BrVector3Set(&pCar_spec->omega, 0.f, 0.f, 0.f);
    BrVector3Set(&pCar_spec->v, 0.f, 0.f, 0.f);
}

// IDA: void __usercall TurnOpponentPhysicsOn(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00402401
void TurnOpponentPhysicsOn(tOpponent_spec* pOpponent_spec) {

    if (pOpponent_spec->physics_me == 0) {
        pOpponent_spec->physics_me = 1;
        gActive_car_list_rebuild_required = 1;
    }
}

// IDA: void __usercall TurnOpponentPhysicsOff(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x0040243f
void TurnOpponentPhysicsOff(tOpponent_spec* pOpponent_spec) {

    DeadStopCar(pOpponent_spec->car_spec);
    if (pOpponent_spec->physics_me) {
        pOpponent_spec->physics_me = 0;
        gActive_car_list_rebuild_required = 1;
    }
}

// IDA: void __cdecl NewObjective(tOpponent_spec *pOpponent_spec, tOpponent_objective_type pObjective_type, ...)
// FUNCTION: CARM95 0x0040694d
void NewObjective(tOpponent_spec* pOpponent_spec, tOpponent_objective_type pObjective_type, ...) {
    va_list marker;

    if (pOpponent_spec->current_objective != eOOT_none) {
        ProcessCurrentObjective(pOpponent_spec, ePOC_die);
    }
    pOpponent_spec->current_objective = pObjective_type;
    pOpponent_spec->time_this_objective_started = gTime_stamp_for_this_munging;
    pOpponent_spec->time_for_this_objective_to_finish = gTime_stamp_for_this_munging + IRandomBetween(30, 180) * 1000;
    if (pObjective_type == eOOT_pursue_and_twat) {
        pOpponent_spec->time_for_this_objective_to_finish += 90000;
    }
    va_start(marker, pObjective_type);
    switch (pObjective_type) {
    case eOOT_complete_race:
        gNum_of_opponents_completing_race++;
        break;
    case eOOT_get_near_player:
        gNum_of_opponents_getting_near++;
        break;
    case eOOT_pursue_and_twat:
        pOpponent_spec->pursue_car_data.pursuee = va_arg(marker, tCar_spec*);
        gNum_of_opponents_pursuing++;
        break;

    default:
        break;
    }
    dr_dprintf("%s: NewObjective() - type %d", pOpponent_spec->car_spec->driver_name, pObjective_type);
    ProcessCurrentObjective(pOpponent_spec, ePOC_start);
    va_end(marker);
}

// IDA: void __usercall CalcRaceRoute(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00402512
void CalcRaceRoute(tOpponent_spec* pOpponent_spec) {
    tS16 section_no;
    tS16 section_no_index;
    tS16 node_no;
    tS16 race_section_count;
    tS16 normal_section_ok_direction_count;
    tS16 normal_section_wrong_direction_count;
    tS16 temp_section_array[8];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    char str[256];
    char work_str[32];
    int i;

    if (pOpponent_spec->nnext_sections >= COUNT_OF(pOpponent_spec->next_sections)) {
        dr_dprintf("%s: CalcRaceRoute() - Pissing off 'cos projected route full up", pOpponent_spec->car_spec->driver_name);
        return;
    }
    if (pOpponent_spec->nnext_sections == 0) {
        dr_dprintf("%s: CalcRaceRoute() - Projected route empty; starting from nearest section", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->complete_race_data.finished_calcing_race_route = 0;
        pOpponent_spec->complete_race_data.found_race_section = 0;
        section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
        if (section_no < 0) {
            return;
        }
        AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
        if (gProgram_state.AI_vehicles.path_sections[section_no].type == ePST_race_path) {
            pOpponent_spec->complete_race_data.found_race_section = 1;
        }
    }
    while (pOpponent_spec->nnext_sections < COUNT_OF(pOpponent_spec->next_sections) && !pOpponent_spec->complete_race_data.finished_calcing_race_route) {
        node_no = gProgram_state.AI_vehicles.path_sections[pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no].node_indices[pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction];
        race_section_count = 0;
        normal_section_ok_direction_count = 0;
        normal_section_wrong_direction_count = 0;
        for (section_no_index = 0; section_no_index < gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections; section_no_index++) {
            section_no = gProgram_state.AI_vehicles.path_nodes[node_no].sections[section_no_index];
            if (pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no != section_no) {
                if (gProgram_state.AI_vehicles.path_sections[section_no].type == 1 && gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0] == node_no) {
                    pOpponent_spec->complete_race_data.found_race_section = 1;
                    temp_section_array[race_section_count] = section_no;
                    race_section_count++;
                } else if (race_section_count == 0 && gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0] == node_no) {
                    temp_section_array[normal_section_ok_direction_count] = section_no;
                    normal_section_ok_direction_count++;
                } else if (race_section_count == 0 && normal_section_ok_direction_count == 0 && (!gProgram_state.AI_vehicles.path_sections[section_no].one_way || gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no)) {
                    temp_section_array[normal_section_wrong_direction_count] = section_no;
                    normal_section_wrong_direction_count++;
                }
            }
        }
        if (race_section_count != 0) {
            AddToOpponentsProjectedRoute(pOpponent_spec, temp_section_array[IRandomBetween(0, race_section_count - 1)], 1);
        } else if (normal_section_ok_direction_count != 0) {
            AddToOpponentsProjectedRoute(pOpponent_spec, temp_section_array[IRandomBetween(0, normal_section_ok_direction_count - 1)], 1);
        } else if (normal_section_wrong_direction_count != 0) {
            AddToOpponentsProjectedRoute(pOpponent_spec, temp_section_array[IRandomBetween(0, normal_section_wrong_direction_count - 1)], 0);
        } else if (pOpponent_spec->complete_race_data.found_race_section) {
            pOpponent_spec->complete_race_data.finished_calcing_race_route = 1;
        } else {
            AddToOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no, pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction == 0);
        }
    }
}

// IDA: void __usercall TopUpRandomRoute(tOpponent_spec *pOpponent_spec@<EAX>, int pSections_to_add@<EDX>)
// FUNCTION: CARM95 0x00402e11
void TopUpRandomRoute(tOpponent_spec* pOpponent_spec, int pSections_to_add) {
    tS16 section_no;
    tS16 node_no;
    tS16 temp_section_array[8];
    int i;
    int target;
    int num_of_temp_sections;
    int direction;

    if (!pSections_to_add) {
        PDEnterDebugger("TopUpRandomRoute() called with no seed (woof, bark, etc.)");
    }
    if (pSections_to_add < 0) {
        target = 10;
    } else {
        target = MIN(pSections_to_add + pOpponent_spec->nnext_sections, 10);
    }
    while (pOpponent_spec->nnext_sections < target) {
        node_no = gProgram_state.AI_vehicles.path_sections[pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no].node_indices[pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction];
        if (gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections > 1) {
            num_of_temp_sections = 0;
            for (i = 0; i < gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections; i++) {
                section_no = gProgram_state.AI_vehicles.path_nodes[node_no].sections[i];
                if (pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no != section_no
                    && (!gProgram_state.AI_vehicles.path_sections[section_no].one_way || gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no)
                    && (pOpponent_spec->cheating || gProgram_state.AI_vehicles.path_sections[section_no].type != ePST_cheat_only)) {
                    temp_section_array[num_of_temp_sections] = section_no;
                    num_of_temp_sections++;
                }
            }

            if (num_of_temp_sections > 1) {
                section_no = temp_section_array[IRandomBetween(0, num_of_temp_sections - 1)];
                direction = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no;
            } else if (num_of_temp_sections == 1) {
                section_no = temp_section_array[0];
                direction = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no;
            } else {
                section_no = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no;
                direction = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction == 0;
            }
        } else {
            section_no = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no;
            direction = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction == 0;
        }
        AddToOpponentsProjectedRoute(pOpponent_spec, section_no, direction);
    }
}

// IDA: int __usercall SearchForSection@<EAX>(tRoute_section *pTemp_store@<EAX>, tRoute_section *pPerm_store@<EDX>, int *pNum_of_perm_store_sections@<EBX>, tS16 pTarget_section@<ECX>, int pDepth, br_scalar pDistance_so_far, tOpponent_spec *pOpponent_spec)
// FUNCTION: CARM95 0x00407f5d
int SearchForSection(tRoute_section* pTemp_store, tRoute_section* pPerm_store, int* pNum_of_perm_store_sections, tS16 pTarget_section, int pDepth, br_scalar pDistance_so_far, tOpponent_spec* pOpponent_spec) {
    // GLOBAL: CARM95 0x530c9c
    static br_scalar shortest_dist;
    // GLOBAL: CARM95 0x530db0
    static int routes_found;
    char depth_indent[32];
    int direction;
    tPath_node* node_ptr;
    tS16 node_no;
    tS16 section_no;
    tS16 section_no_index;
    br_scalar distance_so_far;

    gSFS_cycles_this_time++;
    if (pDepth == 1) {
        memset(gBit_per_node, 0, (gProgram_state.AI_vehicles.number_of_path_nodes + 7) / 8);
        shortest_dist = BR_SCALAR_MAX;
        routes_found = 0;
        *pNum_of_perm_store_sections = 0;
    }
    if (pDepth >= 10) {
        return 0;
    }
    node_no = gProgram_state.AI_vehicles.path_sections[pTemp_store[pDepth - 1].section_no].node_indices[pTemp_store[pDepth - 1].direction];
    node_ptr = &gProgram_state.AI_vehicles.path_nodes[node_no];
    gBit_per_node[node_no / 8] |= 1 << (node_no % 8);
    for (section_no_index = 0; section_no_index < node_ptr->number_of_sections; section_no_index++) {

        section_no = node_ptr->sections[section_no_index];
        direction = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no;

        if ((gBit_per_node[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[direction] / 8] & (1 << (gProgram_state.AI_vehicles.path_sections[section_no].node_indices[direction] % 8))) != 0) {
            continue;
        }
        if ((gProgram_state.AI_vehicles.path_sections[section_no].one_way && gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] == node_no)) {
            continue;
        }
        if (!pOpponent_spec->cheating && gProgram_state.AI_vehicles.path_sections[section_no].type == ePST_cheat_only) {
            continue;
        }

        pTemp_store[pDepth].section_no = section_no;
        pTemp_store[pDepth].direction = direction;
        distance_so_far = gProgram_state.AI_vehicles.path_sections[section_no].length + pDistance_so_far;
        if (pTarget_section == section_no && distance_so_far <= shortest_dist) {
            shortest_dist = distance_so_far;
            *pNum_of_perm_store_sections = pDepth + 1;
            memcpy(pPerm_store, pTemp_store, sizeof(tRoute_section) * *pNum_of_perm_store_sections);

            gWanky_arse_tit_fuck = routes_found + 1;
            routes_found++;
            if (routes_found >= 2) {
                return 1;
            } else {
                gBit_per_node[node_no / 8] &= ~(1 << (node_no % 8));
                return 0;
            }
        }
        if (pDepth < 9 && SearchForSection(pTemp_store, pPerm_store, pNum_of_perm_store_sections, pTarget_section, pDepth + 1, distance_so_far, pOpponent_spec)) {
            return 1;
        }
    }
    gBit_per_node[node_no / 8] &= ~(1 << (node_no % 8));
    return 0;
}

// IDA: void __usercall CalcGetNearPlayerRoute(tOpponent_spec *pOpponent_spec@<EAX>, tCar_spec *pPlayer@<EDX>)
// FUNCTION: CARM95 0x004089bf
void CalcGetNearPlayerRoute(tOpponent_spec* pOpponent_spec, tCar_spec* pPlayer) {
    int i;
    int pass_2_depth;
    int sections_away;
    int num_of_perm_store_sections;
    int sections_to_copy;
    int fuck_it;
    tS16 section_no;
    tS16 players_section;
    br_vector3 section_v;
    br_vector3 intersect;
    br_scalar distance;
    tRoute_section temp_store[10];
    tRoute_section perm_store[10];
    char work_str[32];
    char str[256];

    fuck_it = 0;
    if (pOpponent_spec->nnext_sections >= COUNT_OF(pOpponent_spec->next_sections)) {
        dr_dprintf("%s: CalcGetNearPlayerRoute() - Quitting because route full up", pOpponent_spec->car_spec->driver_name);
        return;
    }
    players_section = FindNearestPathSection(&pPlayer->car_master_actor->t.t.translate.t, &section_v, &intersect, &distance);
    if (players_section < 0) {
        PDEnterDebugger("No path section near player. THIS CAN'T HAPPEN!");
        return;
    }
    if (pOpponent_spec->players_section_when_last_calced_full_path != players_section) {
        dr_dprintf("%s: CalcGetNearPlayerRoute() - Player has moved since last time (section #%d; was #%d)", pOpponent_spec->car_spec->driver_name, players_section, pOpponent_spec->players_section_when_last_calced_full_path);
        ClearOpponentsProjectedRoute(pOpponent_spec);
    }
    if (pOpponent_spec->nnext_sections == 0) {
        pOpponent_spec->players_section_when_last_calced_full_path = players_section;
        dr_dprintf("%s: CalcGetNearPlayerRoute() - Empty route; setting players_section_when_last_calced_full_path to #%d", pOpponent_spec->car_spec->driver_name, pOpponent_spec->players_section_when_last_calced_full_path);
        section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &section_v, &intersect, &distance);
        if (section_no < 0) {
            return;
        }
        AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
    }
    if (pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no == players_section) {
        dr_dprintf("%s: CalcGetNearPlayerRoute() - Last section is player's section (%d), so tack a random one on t'end", pOpponent_spec->car_spec->driver_name, players_section);
        TopUpRandomRoute(pOpponent_spec, 1);
    }
    while (pOpponent_spec->nnext_sections < 6 && !fuck_it) {
        temp_store[0].section_no = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no;
        temp_store[0].direction = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction;
        dr_dprintf("%s: CalcGetNearPlayerRoute() - In loop; our section #%d, player's section #%d", pOpponent_spec->car_spec->driver_name, temp_store[0].section_no, players_section);
        gSFS_count++;
        gSFS_cycles_this_time = 0;
        SearchForSection(temp_store, perm_store, &num_of_perm_store_sections, players_section, 1, 0.f, pOpponent_spec);
        gSFS_total_cycles += gSFS_cycles_this_time;
        if (gSFS_cycles_this_time > gSFS_max_cycles) {
            gSFS_max_cycles = gSFS_cycles_this_time;
        }
        dr_dprintf(">>>SearchForSection() - max %d, avg %.1f", gSFS_max_cycles, (float)gSFS_total_cycles / gSFS_count);
        if (num_of_perm_store_sections > 1) {
            sections_to_copy = MIN(COUNT_OF(pOpponent_spec->next_sections) - pOpponent_spec->nnext_sections, num_of_perm_store_sections - 1);
            memcpy(&pOpponent_spec->next_sections[pOpponent_spec->nnext_sections], &perm_store[1], sizeof(tRoute_section) * sections_to_copy);
            pOpponent_spec->nnext_sections += sections_to_copy;
            TopUpRandomRoute(pOpponent_spec, 1);
        } else {
            dr_dprintf("%s: CalcGetNearPlayerRoute() - SearchForSection() produced bugger all", pOpponent_spec->car_spec->driver_name);
            fuck_it = 1;
            if (pOpponent_spec->nnext_sections <= 4) {
                TopUpRandomRoute(pOpponent_spec, 4 - pOpponent_spec->nnext_sections + 4);
            }
        }
    }
}

// IDA: void __usercall CalcReturnToStartPointRoute(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00407d1e
void CalcReturnToStartPointRoute(tOpponent_spec* pOpponent_spec) {
    int i;
    int pass_2_depth;
    int sections_away;
    int num_of_perm_store_sections;
    int sections_to_copy;
    tS16 section_no;
    br_vector3 intersect;
    br_vector3 section_v;
    br_scalar distance;
    tRoute_section temp_store[10];
    tRoute_section perm_store[10];

    ClearOpponentsProjectedRoute(pOpponent_spec);
    section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &section_v, &intersect, &distance);
    BrVector3Normalise(&section_v, &section_v);

    if (BrVector3Dot(&pOpponent_spec->car_spec->direction, &section_v) > 0.0f) {
        AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
    } else {
        AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 0);
    }
    temp_store[0].section_no = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].section_no;
    temp_store[0].direction = pOpponent_spec->next_sections[pOpponent_spec->nnext_sections - 1].direction;
    gSFS_count++;
    gSFS_cycles_this_time = 0;
    SearchForSection(temp_store, perm_store, &num_of_perm_store_sections, pOpponent_spec->return_to_start_data.section_no, 1, 0.0f, pOpponent_spec);
    gSFS_total_cycles += gSFS_cycles_this_time;
    if (gSFS_cycles_this_time > gSFS_max_cycles) {
        gSFS_max_cycles = gSFS_cycles_this_time;
    }
    if (num_of_perm_store_sections > 1) {
        sections_to_copy = MIN(num_of_perm_store_sections - 1, 10 - pOpponent_spec->nnext_sections);
        memcpy(&pOpponent_spec->next_sections[pOpponent_spec->nnext_sections], &perm_store[1], sizeof(tRoute_section) * sections_to_copy);
        pOpponent_spec->nnext_sections += sections_to_copy;
        TopUpRandomRoute(pOpponent_spec, 1);
    } else {
        if (pOpponent_spec->nnext_sections <= 6) {
            TopUpRandomRoute(pOpponent_spec, 4 - pOpponent_spec->nnext_sections + 4);
        }
    }
}

// IDA: void __usercall ClearOpponentsProjectedRoute(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00404684
void ClearOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec) {

    pOpponent_spec->nnext_sections = 0;
}

// IDA: int __usercall AddToOpponentsProjectedRoute@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pSection_no@<EDX>, int pDirection@<EBX>)
// FUNCTION: CARM95 0x004030ef
int AddToOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, tS16 pSection_no, int pDirection) {

    if (pOpponent_spec->nnext_sections < COUNT_OF(pOpponent_spec->next_sections)) {
        pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].section_no = pSection_no;
        pOpponent_spec->next_sections[pOpponent_spec->nnext_sections].direction = pDirection;
        pOpponent_spec->nnext_sections++;
        return 1;
    } else {
        return 0;
    }
}

// IDA: int __usercall ShiftOpponentsProjectedRoute@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, int pPlaces@<EDX>)
// FUNCTION: CARM95 0x00404699
int ShiftOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, int pPlaces) {
    int i;

    if (pOpponent_spec->nnext_sections > pPlaces) {
        for (i = 0; i < COUNT_OF(pOpponent_spec->next_sections) - pPlaces; i++) {
            pOpponent_spec->next_sections[i].section_no = pOpponent_spec->next_sections[pPlaces + i].section_no;
            pOpponent_spec->next_sections[i].direction = pOpponent_spec->next_sections[pPlaces + i].direction;
        }
        pOpponent_spec->nnext_sections -= pPlaces;
        return 1;
    } else {
        return 0;
    }
}

// IDA: void __usercall StunTheBugger(tOpponent_spec *pOpponent_spec@<EAX>, int pMilliseconds@<EDX>)
// FUNCTION: CARM95 0x0040b12d
void StunTheBugger(tOpponent_spec* pOpponent_spec, int pMilliseconds) {

    pOpponent_spec->car_spec->acc_force = 0.f;
    pOpponent_spec->car_spec->brake_force = 0.f;
    pOpponent_spec->car_spec->curvature = 0.f;
    pOpponent_spec->stun_time_ends = MAX(gTime_stamp_for_this_munging + pMilliseconds, pOpponent_spec->stun_time_ends);
}

// IDA: void __usercall UnStunTheBugger(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00405e44
void UnStunTheBugger(tOpponent_spec* pOpponent_spec) {

    pOpponent_spec->stun_time_ends = 0;
}

// IDA: void __usercall ProcessCompleteRace(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x00406732
void ProcessCompleteRace(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3* initial_pos;
    br_actor* car_actor;
    tComplete_race_data* data;
    int res;
    char str[256];

    initial_pos = &gProgram_state.initial_position;
    car_actor = pOpponent_spec->car_spec->car_master_actor;
    data = &pOpponent_spec->complete_race_data;

    switch (pCommand) {
    case ePOC_start:
        dr_dprintf("%s: ProcessCompleteRace() - new objective started", pOpponent_spec->car_spec->driver_name);
        ClearOpponentsProjectedRoute(pOpponent_spec);
        CalcRaceRoute(pOpponent_spec);
        ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        break;
    case ePOC_run:
        if (pOpponent_spec->follow_path_data.section_no > 20000) {
            ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
            pOpponent_spec->follow_path_data.section_no = 20000;
        }
        if (pOpponent_spec->nnext_sections != 0) {
            res = ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0);
            if (res != eFPR_end_of_path) {
                goto skip_new_objective;
            }
        }

        dr_dprintf("%s: Giving up following race path because ran out of race path", pOpponent_spec->car_spec->driver_name);
        NewObjective(pOpponent_spec, eOOT_get_near_player);

    skip_new_objective:

        if (res != eFPR_OK) {
            if (res == eFPR_given_up) {
                dr_dprintf("%s: Giving up complete_race because ProcessFollowPath() gave up", pOpponent_spec->car_spec->driver_name);
            } else {
                dr_dprintf("%s: Giving up complete_race because reached end", pOpponent_spec->car_spec->driver_name);
            }
            ObjectiveComplete(pOpponent_spec);
        }
        if (gTime_stamp_for_this_munging > pOpponent_spec->time_this_objective_started + 20000) {
            dr_dprintf("%s: Time to give up complete_race. Might be back in a sec, though!", pOpponent_spec->car_spec->driver_name);
            ObjectiveComplete(pOpponent_spec);
        }
        if (pOpponent_spec->nnext_sections <= 4 && !data->finished_calcing_race_route) {
            CalcRaceRoute(pOpponent_spec);
        }
        break;
    }
}

// IDA: void __usercall StartRecordingTrail(tCar_spec *pPursuee@<EAX>)
// FUNCTION: CARM95 0x00405e59
void StartRecordingTrail(tCar_spec* pPursuee) {
    int i;

    if (pPursuee->no_of_processes_recording_my_trail == 0) {
        dr_dprintf("StartRecordingTrail - starting from scratch");
        pPursuee->no_of_processes_recording_my_trail = 1;
        pPursuee->my_trail.nodes_shifted_this_frame = 0;
        pPursuee->my_trail.has_deviated_recently = 0;
        pPursuee->my_trail.number_of_nodes = 2;
        pPursuee->my_trail.time_of_next_recording = gTime_stamp_for_this_munging + 500;
        BrVector3Copy(&pPursuee->my_trail.base_heading, &pPursuee->direction);
        BrVector3Copy(&pPursuee->my_trail.trail_nodes[0], &pPursuee->car_master_actor->t.t.translate.t);
        BrVector3Copy(&pPursuee->my_trail.trail_nodes[1], &pPursuee->car_master_actor->t.t.translate.t);
        pPursuee->my_trail.trail_nodes[0].v[2] += 0.2f;
    } else {
        dr_dprintf("StartRecordingTrail - another pursuer attaching");
        pPursuee->no_of_processes_recording_my_trail++;
    }
}

// IDA: void __usercall RecordNextTrailNode(tCar_spec *pPursuee@<EAX>)
// FUNCTION: CARM95 0x00405f97
void RecordNextTrailNode(tCar_spec* pPursuee) {
    tPursuee_trail* trail;
    br_vector3 start1;
    br_vector3 finish1;
    br_vector3 start2;
    br_vector3 finish2;
    br_vector3 offset_v;
    br_vector3 car_to_last_point_v;
    br_scalar length;
    int visible;

    trail = &pPursuee->my_trail;
    visible = 1;
    trail->nodes_shifted_this_frame = 0;
    if (trail->time_of_next_recording < gTime_stamp_for_this_munging) {

        trail->time_of_next_recording = gTime_stamp_for_this_munging + 500;
        if (BrVector3Dot(&pPursuee->direction, &trail->base_heading) < FastScalarCos(30)) {
            trail->has_deviated_recently = 1;
        }
        BrVector3Sub(&car_to_last_point_v, &trail->trail_nodes[trail->number_of_nodes - 2], &pPursuee->car_master_actor->t.t.translate.t);
        length = BrVector3Length(&car_to_last_point_v);
        if (length < 0.3f) {
            return;
        }
        CalcNegativeXVector(&offset_v, &trail->trail_nodes[trail->number_of_nodes - 2], &pPursuee->car_master_actor->t.t.translate.t, 0.5f);

        BrVector3Add(&start1, &trail->trail_nodes[trail->number_of_nodes - 2], &offset_v);
        BrVector3Add(&finish1, &pPursuee->car_master_actor->t.t.translate.t, &offset_v);
        BrVector3Sub(&start2, &trail->trail_nodes[trail->number_of_nodes - 2], &offset_v);
        BrVector3Sub(&finish2, &pPursuee->car_master_actor->t.t.translate.t, &offset_v);

        if ((trail->has_deviated_recently
                || !(visible = PointVisibleFromHere(&start1, &finish1))
                || !(visible = PointVisibleFromHere(&start2, &finish2))
                || !(visible = PointVisibleFromHere(&trail->trail_nodes[trail->number_of_nodes - 2], &pPursuee->car_master_actor->t.t.translate.t)))
            && ((visible && length > 2.0f) || (!visible && length > 1.5f))) {
            if (trail->number_of_nodes < COUNT_OF(trail->trail_nodes)) {
                trail->number_of_nodes++;
            } else {
                memmove(trail->trail_nodes, &trail->trail_nodes[1], (COUNT_OF(trail->trail_nodes) - 1) * sizeof(trail->trail_nodes[0]));
                trail->nodes_shifted_this_frame = 1;
            }
            trail->has_deviated_recently = 0;
            BrVector3Copy(&trail->base_heading, &pPursuee->direction);
        }
        BrVector3Copy(&trail->trail_nodes[trail->number_of_nodes - 1], &pPursuee->car_master_actor->t.t.translate.t);
    }
}

// IDA: tS16 __usercall FindNearestTrailSection@<AX>(tOpponent_spec *pOpponent_spec@<EAX>, tCar_spec *pPursuee@<EDX>, br_vector3 *pSection_v@<EBX>, br_vector3 *pIntersect@<ECX>, br_scalar *pDistance)
// FUNCTION: CARM95 0x0040769d
tS16 FindNearestTrailSection(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee, br_vector3* pSection_v, br_vector3* pIntersect, br_scalar* pDistance) {

    return FindNearestGeneralSection(pPursuee, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, pSection_v, pIntersect, pDistance);
}

// IDA: tS16 __usercall CalcNextTrailSection@<AX>(tOpponent_spec *pOpponent_spec@<EAX>, int pSection@<EDX>)
// FUNCTION: CARM95 0x00403145
tS16 CalcNextTrailSection(tOpponent_spec* pOpponent_spec, int pSection) {
    int section_no;
    tPursuee_trail* trail;

    section_no = pSection - 15000;
    trail = &pOpponent_spec->pursue_car_data.pursuee->my_trail;

    if (trail->number_of_nodes - 2 <= section_no) {
        return -1;
    }
    return section_no + 15001;
}

// IDA: void __usercall ProcessPursueAndTwat(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x00406a69
void ProcessPursueAndTwat(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    tPursue_car_data* data;
    br_vector3 wank;
    br_vector3 section_v;
    br_vector3 intersect;
    br_scalar d;
    br_scalar s;
    br_scalar t;
    br_scalar distance;
    tFollow_path_result res;
    char str[256];
    tS16 section_no;

    data = &pOpponent_spec->pursue_car_data;
    switch (pCommand) {
    case ePOC_start:
        dr_dprintf("%s: ProcessPursueAndTwat() - new objective started", pOpponent_spec->car_spec->driver_name);
        data->direct_line_nodes[0].number_of_sections = 1;
        data->direct_line_nodes[0].sections[0] = 10000;
        data->direct_line_nodes[1].number_of_sections = 1;
        data->direct_line_nodes[1].sections[0] = 10000;
        data->direct_line_section.node_indices[0] = 10000;
        data->direct_line_section.node_indices[1] = 10001;
        data->direct_line_section.min_speed[0] = 0;
        data->direct_line_section.min_speed[1] = 0;
        data->direct_line_section.max_speed[0] = -1;
        data->direct_line_section.max_speed[1] = -1;
        data->direct_line_section.width = 0.5f;
        data->direct_line_section.type = ePST_normal;
        data->start_backup_time = 0;
        data->time_of_next_visibility_check = 0;
        data->time_pursuee_last_visible = 0;
        data->time_last_twatted_em = 0;
        data->time_last_away_from_pursuee = gTime_stamp_for_this_munging;
        data->state = ePCS_what_now;
        return;

    case ePOC_run:

        if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec) && pOpponent_spec->distance_from_home > 75.0f) {
            dr_dprintf("%s: Completing pursuit objective because I'm out of my precinct", pOpponent_spec->car_spec->driver_name);
            NewObjective(pOpponent_spec, eOOT_return_to_start);
            return;
        }

        data->direct_line_section.length = MAX(pOpponent_spec->player_to_oppo_d, 3.0f);
        if (pOpponent_spec->player_to_oppo_d > 3.0f) {
            data->time_last_away_from_pursuee = gTime_stamp_for_this_munging;
        }
        if (gOpponents[pOpponent_spec->index].psyche.grudge_against_player < 15) {
            dr_dprintf("%s: Completing pursuit objective because I'm happy now", pOpponent_spec->car_spec->driver_name);
            ObjectiveComplete(pOpponent_spec);
            return;
        }
        if (data->state != ePCS_backing_up) {
            if (data->time_last_twatted_em + 1000 < gTime_stamp_for_this_munging && data->time_last_twatted_em + 3000 > gTime_stamp_for_this_munging && BrVector3Length(&data->pursuee->v) < 0.3f) {
                dr_dprintf("%s: Backing up because we're 'stationary' after colliding with pursuee", pOpponent_spec->car_spec->driver_name);
                data->start_backup_time = gTime_stamp_for_this_munging;
                data->state = ePCS_backing_up;
            } else {

                if (data->time_last_away_from_pursuee + 7000 < gTime_stamp_for_this_munging && data->time_last_twatted_em + 7000 < gTime_stamp_for_this_munging && data->start_backup_time + 10000 < gTime_stamp_for_this_munging) {
                    dr_dprintf("%s: Backing up because we're too close to pursuee without having twatted him", pOpponent_spec->car_spec->driver_name);
                    data->start_backup_time = gTime_stamp_for_this_munging;
                    data->state = ePCS_backing_up;
                } else {
                    if (pOpponent_spec->cheating) {
                        if (pOpponent_spec->player_to_oppo_d < 50.0f
                            && PointVisibleFromHere(&data->pursuee->car_master_actor->t.t.translate.t, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t)) {
                            data->time_pursuee_last_visible = gTime_stamp_for_this_munging;
                        } else {
                            data->time_pursuee_last_visible = 0;
                        }
                    } else if (pOpponent_spec->player_in_view_now || (data->time_of_next_visibility_check < gTime_stamp_for_this_munging && pOpponent_spec->player_to_oppo_d < 35.0f && PointVisibleFromHere(&data->pursuee->car_master_actor->t.t.translate.t, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t))) {
                        data->time_pursuee_last_visible = gTime_stamp_for_this_munging;
                        data->time_of_next_visibility_check = gTime_stamp_for_this_munging + 600;
                    }
                    if (data->time_pursuee_last_visible + 3000 > gTime_stamp_for_this_munging) {
                        if (data->state != ePCS_following_line_of_sight) {
                            dr_dprintf("%s: Commencing ePCS_following_line_of_sight state", pOpponent_spec->car_spec->driver_name);
                            data->state = ePCS_following_line_of_sight;
                            sprintf(str, "%s: I've spotted you!", pOpponent_spec->car_spec->driver_name);
                            ProcessFollowPath(pOpponent_spec, ePOC_start, 1, 1, 0);
                        }
                    } else {
                        if (data->pursuee->my_trail.number_of_nodes < 2) {
                            dr_dprintf("%s: Giving up pursuit - not visible & no trail yet", pOpponent_spec->car_spec->driver_name);
                            NewObjective(pOpponent_spec, eOOT_get_near_player);
                            return;
                        }
                        if (data->state != ePCS_following_trail) {
                            section_no = FindNearestTrailSection(pOpponent_spec, data->pursuee, &section_v, &intersect, &distance);
                            data->state = ePCS_following_trail;
                            if (distance > 20.0f || section_no == -1) {
                                dr_dprintf("%s: Giving up pursuit - not visible & trail ain't close enough (%f)", pOpponent_spec->car_spec->driver_name, distance);
                                NewObjective(pOpponent_spec, eOOT_get_near_player);
                                return;
                            }
                            dr_dprintf("%s: Commencing ePCS_following_trail state", pOpponent_spec->car_spec->driver_name);
                            pOpponent_spec->follow_path_data.section_no = section_no;
                            ProcessFollowPath(pOpponent_spec, ePOC_start, 1, 0, 0);
                        }
                    }
                }
            }
        }
        switch (data->state) {

        case ePCS_backing_up:
            if (data->start_backup_time + 2200 < gTime_stamp_for_this_munging) {
                pOpponent_spec->car_spec->acc_force = 0.0;
                pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->M * 15.0f;
                if (data->start_backup_time + 3000 < gTime_stamp_for_this_munging) {
                    pOpponent_spec->car_spec->brake_force = 0.0f;
                    data->state = ePCS_what_now;
                    dr_dprintf("%s: Finished backing up.", pOpponent_spec->car_spec->driver_name);
                }
            } else {
                pOpponent_spec->car_spec->curvature = 0.0f;
                pOpponent_spec->car_spec->brake_force = 0.0f;
                pOpponent_spec->car_spec->acc_force = pOpponent_spec->car_spec->M * -8.0f;
            }
            break;

        case ePCS_following_line_of_sight:
            BrVector3Copy(&data->direct_line_nodes[0].p, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
            BrVector3Sub(&wank, &data->pursuee->car_master_actor->t.t.translate.t, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
            d = BrVector3Length(&wank);
            BrVector3Sub(&wank, &data->pursuee->v, &pOpponent_spec->car_spec->v);
            s = BrVector3Length(&wank);
            if (s < 1.0f) {
                t = 0.0;
            } else {
                t = d / s / 2.0f;
            }
            BrVector3Scale(&data->direct_line_nodes[1].p, &data->pursuee->v, t);
            BrVector3Accumulate(&data->direct_line_nodes[1].p, &data->pursuee->car_master_actor->t.t.translate.t);
            if (d < 2.0f) {
                ProcessFollowPath(pOpponent_spec, ePOC_run, 1, 1, 1);
            } else {
                ProcessFollowPath(pOpponent_spec, ePOC_run, 1, 1, 0);
            }
            break;

        case ePCS_following_trail:
            if (data->pursuee->my_trail.nodes_shifted_this_frame) {
                if (pOpponent_spec->follow_path_data.section_no > 15000) {
                    pOpponent_spec->follow_path_data.section_no--;
                } else {
                    data->state = ePCS_following_trail;
                    section_no = FindNearestTrailSection(pOpponent_spec, data->pursuee, &section_v, &intersect, &distance);
                    dr_dprintf("%s: Trail got away; found new trail section %d", pOpponent_spec->car_spec->driver_name, section_no);
                    if (section_no == -1 || distance > 20.0f || !PointVisibleFromHere(&intersect, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t)) {
                        dr_dprintf("%s: ...which unfortunately is too far away (%fBRU) or not visible - end of pursuit", pOpponent_spec->car_spec->driver_name, distance);
                        NewObjective(pOpponent_spec, eOOT_get_near_player);
                        break;
                    }
                    pOpponent_spec->follow_path_data.section_no = section_no;
                    ProcessFollowPath(pOpponent_spec, ePOC_start, 1, 0, 0);
                }
                dr_dprintf("%s: Following re-jobbied section %d/%d", pOpponent_spec->car_spec->driver_name, pOpponent_spec->follow_path_data.section_no, data->pursuee->my_trail.number_of_nodes - 1);
            }
            sprintf(str, "%s: Trail section %d/%d", pOpponent_spec->car_spec->driver_name, pOpponent_spec->follow_path_data.section_no, data->pursuee->my_trail.number_of_nodes - 1);
            res = ProcessFollowPath(pOpponent_spec, ePOC_run, 1, 0, 0);
            if (res == eFPR_given_up) {
                NewObjective(pOpponent_spec, eOOT_get_near_player);
                return;
            }
            if (res == eFPR_end_of_path) {
                NewObjective(pOpponent_spec, eOOT_get_near_player);
                return;
            }
            break;

        case ePCS_what_now:
            PDEnterDebugger("ERROR: what_now state called in ProcessPursueAndTwat()");

        default:
            break;
        }

        break;

    default:
        return;
    }
}

// IDA: void __usercall ProcessRunAway(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x004076d2
void ProcessRunAway(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    int res;
    tS16 section_no;
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    char str[256];

    switch (pCommand) {

    case ePOC_start:
        dr_dprintf("%s: ProcessRunAway() - new objective started", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->run_away_data.time_to_stop = gTime_stamp_for_this_munging + 1000 * IRandomBetween(30, 90);
        ClearOpponentsProjectedRoute(pOpponent_spec);
        section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
        if (BrVector3Dot(&pOpponent_spec->car_spec->direction, &direction_v) >= 0.0f) {
            AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
        } else {
            AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 0);
        }
        TopUpRandomRoute(pOpponent_spec, -1);
        ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        sprintf(str, "%s: Shit! I'm out of here...", pOpponent_spec->car_spec->driver_name);
        break;

    case ePOC_run:
        if (pOpponent_spec->run_away_data.time_to_stop < gTime_stamp_for_this_munging) {
            ObjectiveComplete(pOpponent_spec);
            return;
        }
        if (pOpponent_spec->follow_path_data.section_no > 20000) {
            ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
            pOpponent_spec->follow_path_data.section_no = 20000;
        }
        if (pOpponent_spec->nnext_sections < 10) {
            TopUpRandomRoute(pOpponent_spec, 10 - pOpponent_spec->nnext_sections);
        }
        res = ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0);
        if (res == eFPR_given_up) {
            ClearOpponentsProjectedRoute(pOpponent_spec);
            section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
            if (BrVector3Dot(&pOpponent_spec->car_spec->direction, &direction_v) >= 0.0f) {
                AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 1);
            } else {
                AddToOpponentsProjectedRoute(pOpponent_spec, section_no, 0);
            }
            TopUpRandomRoute(pOpponent_spec, -1);
            ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        }

        break;

    case ePOC_die:
        return;
    }
}

// IDA: void __usercall ProcessWaitForSomeHaplessSod(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x004079da
void ProcessWaitForSomeHaplessSod(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {

    switch (pCommand) {
    case ePOC_start:
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->M;
        return;
    case ePOC_run:
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->M;
        return;

    case ePOC_die:
        return;
    }
}

// IDA: void __usercall ProcessReturnToStart(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x00407a66
void ProcessReturnToStart(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3 section_v;
    br_vector3 our_pos_xz;
    br_vector3 cop_to_start;
    br_scalar distance;
    int res;

    switch (pCommand) {
    case ePOC_start:
        dr_dprintf("%s: ProcessReturnToStart() - new objective started", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->return_to_start_data.waiting_near_start = 0;
        pOpponent_spec->return_to_start_data.section_no = FindNearestPathSection(&pOpponent_spec->start_pos, &section_v, &pOpponent_spec->return_to_start_data.nearest_path_point, &distance);
        pOpponent_spec->return_to_start_data.nearest_path_point.v[1] = 0.0;
        CalcReturnToStartPointRoute(pOpponent_spec);
        ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        break;
    case ePOC_run:
        if (TeleportCopToStart(pOpponent_spec)) {
            break;
        }
        if (!pOpponent_spec->return_to_start_data.waiting_near_start) {
            BrVector3Copy(&our_pos_xz, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
            our_pos_xz.v[1] = 0.0f;
            BrVector3Sub(&cop_to_start, &pOpponent_spec->start_pos, &our_pos_xz);
            if (BrVector3Length(&cop_to_start) < 10.f) {
                pOpponent_spec->return_to_start_data.waiting_near_start = 1;
                pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->M * 15.0f;
            } else {
                if (pOpponent_spec->follow_path_data.section_no > 20000) {
                    ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
                    pOpponent_spec->follow_path_data.section_no = 20000;
                }
                if (pOpponent_spec->nnext_sections <= 4) {
                    CalcReturnToStartPointRoute(pOpponent_spec);
                }
                res = ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0);
                if (res == eFPR_given_up || res == eFPR_end_of_path) {
                    if (res == eFPR_given_up) {
                        dr_dprintf("%s: Restarting return_to_start route because ProcessFollowPath() gave up.", pOpponent_spec->car_spec->driver_name);
                    } else {
                        dr_dprintf("%s: Restarting return_to_start route because ran out of path!", pOpponent_spec->car_spec->driver_name);
                    }
                    ClearOpponentsProjectedRoute(pOpponent_spec);
                    CalcReturnToStartPointRoute(pOpponent_spec);
                    ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
                }
            }
        } else {
            pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->M * 15.0f;
        }
        break;

    case ePOC_die:
        return;
    }
}

// IDA: void __usercall ProcessLevitate(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x00408349
void ProcessLevitate(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    float t;
    float terminal_time;
    float y;

    switch (pCommand) {
    case ePOC_start:
        dr_dprintf("%s: ProcessLevitate() - new objective started", pOpponent_spec->car_spec->driver_name);
        pOpponent_spec->levitate_data.waiting_to_levitate = 1;
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->M;
        pOpponent_spec->car_spec->acc_force = 0.f;
        pOpponent_spec->levitate_data.time_started = gTime_stamp_for_this_munging;
        break;

    case ePOC_run:
        if (pOpponent_spec->levitate_data.waiting_to_levitate) {
            if ((BrVector3Length(&pOpponent_spec->car_spec->v) < .01f && BrVector3Length(&pOpponent_spec->car_spec->omega) < 1.f) || gTime_stamp_for_this_munging - pOpponent_spec->levitate_data.time_started > 4000) {
                pOpponent_spec->levitate_data.waiting_to_levitate = 0;
                pOpponent_spec->levitate_data.time_started = gTime_stamp_for_this_munging;
                pOpponent_spec->levitate_data.initial_y = pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1];
                if (pOpponent_spec->car_spec->has_been_stolen) {
                    NewTextHeadupSlot(eHeadupSlot_misc, 250, 5000, -4, GetMiscString(kMiscString_CarAddedToChangeCarList));
                }
            } else {
                pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->M;
                pOpponent_spec->car_spec->acc_force = 0.f;
                BrVector3InvScale(&pOpponent_spec->car_spec->omega, &pOpponent_spec->car_spec->omega,
                    pow(2.f, gFrame_period_for_this_munging / 1000.0));
            }
        }
        if (!pOpponent_spec->levitate_data.waiting_to_levitate) {
            TurnOpponentPhysicsOff(pOpponent_spec);
            t = (gTime_stamp_for_this_munging - pOpponent_spec->levitate_data.time_started) / 1000.0;
            terminal_time = 20.f;
            if (t < terminal_time) {
                y = t * t * .5f / 2.f;
            } else {
                y = terminal_time * terminal_time * 0.5 / 2.0 + (t - terminal_time) * 10.0;
            }
            pOpponent_spec->car_spec->car_master_actor->t.t.translate.t.v[1] = pOpponent_spec->levitate_data.initial_y + y;
            if (y > 200.f) {
                pOpponent_spec->finished_for_this_race = 1;
            }
        }
        break;
    case ePOC_die:
        return;
    default:
        break;
    }
}

// IDA: void __usercall ProcessGetNearPlayer(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x004086fe
void ProcessGetNearPlayer(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3* initial_pos;
    br_actor* car_actor;
    int res;
    char str[256];

    initial_pos = &gProgram_state.initial_position;
    car_actor = pOpponent_spec->car_spec->car_master_actor;

    switch (pCommand) {
    case ePOC_start:
        dr_dprintf("%s: ProcessGetNearPlayer() - new objective started", pOpponent_spec->car_spec->driver_name);
        ClearOpponentsProjectedRoute(pOpponent_spec);
        CalcGetNearPlayerRoute(pOpponent_spec, &gProgram_state.current_car);
        ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        break;

    case ePOC_run:
        if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec) && pOpponent_spec->distance_from_home > 75.0f) {
            dr_dprintf("%s: Completing get_near objective because I'm out of my precinct", pOpponent_spec->car_spec->driver_name);
            NewObjective(pOpponent_spec, eOOT_return_to_start);
            return;
        }
        if (pOpponent_spec->follow_path_data.section_no > 20000) {
            if (pOpponent_spec->player_to_oppo_d < 10.0f || pOpponent_spec->follow_path_data.section_no == pOpponent_spec->players_section_when_last_calced_full_path) {
                dr_dprintf("%s: ProcessGetNearPlayer() - giving up 'cos got to player's section", pOpponent_spec->car_spec->driver_name);
                ObjectiveComplete(pOpponent_spec);
                return;
            }
            ShiftOpponentsProjectedRoute(pOpponent_spec, pOpponent_spec->follow_path_data.section_no - 20000);
            pOpponent_spec->follow_path_data.section_no = 20000;
        }
        if (pOpponent_spec->nnext_sections <= 4) {
            CalcGetNearPlayerRoute(pOpponent_spec, &gProgram_state.current_car);
        }
        res = ProcessFollowPath(pOpponent_spec, ePOC_run, 0, 0, 0);
        sprintf(str, "Get near: %d", GetOpponentsRealSection(pOpponent_spec, pOpponent_spec->follow_path_data.section_no));

        if (res == eFPR_given_up || res == eFPR_end_of_path) {
            if (res == eFPR_given_up) {
                NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                return;
            } else {
                dr_dprintf("%s: Restarting get_near_player route because ran out of path!", pOpponent_spec->car_spec->driver_name);
            }
            ClearOpponentsProjectedRoute(pOpponent_spec);
            CalcGetNearPlayerRoute(pOpponent_spec, &gProgram_state.current_car);
            ProcessFollowPath(pOpponent_spec, ePOC_start, 0, 0, 0);
        }

    default:
        break;
    }
}

// IDA: void __usercall ProcessFrozen(tOpponent_spec *pOpponent_spec@<EAX>, tProcess_objective_command pCommand@<EDX>)
// FUNCTION: CARM95 0x00408d47
void ProcessFrozen(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {

    switch (pCommand) {
    case ePOC_start:
        dr_dprintf("%d ProcessFrozen() - new task started", pOpponent_spec->index);
        dr_dprintf("%s: Rematerialising from ePOC_start in ProcessFrozen()...", pOpponent_spec->car_spec->driver_name);
        RematerialiseOpponentOnNearestSection(pOpponent_spec, 0.f);
        pOpponent_spec->car_spec->acc_force = 0.f;
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->M;
        break;
    case ePOC_run:
        pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->M;
        break;
    case ePOC_die:
        pOpponent_spec->car_spec->brake_force = 0.f;
        break;
    }
}

// IDA: int __usercall HeadOnWithPlayerPossible@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00409cd5
int HeadOnWithPlayerPossible(tOpponent_spec* pOpponent_spec) {
    br_vector3 oppo_to_player_norm;

    oppo_to_player_norm.v[0] = gProgram_state.current_car.car_master_actor->t.t.mat.m[3][0]
        - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][0];
    oppo_to_player_norm.v[1] = gProgram_state.current_car.car_master_actor->t.t.mat.m[3][1]
        - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1];
    oppo_to_player_norm.v[2] = gProgram_state.current_car.car_master_actor->t.t.mat.m[3][2]
        - pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][2];

    BrVector3Normalise(&oppo_to_player_norm, &oppo_to_player_norm);
    if (gHead_on_cos_value < BrVector3Dot(&pOpponent_spec->car_spec->direction, &oppo_to_player_norm)
        && -gHead_on_cos_value > BrVector3Dot(&gProgram_state.current_car.direction, &oppo_to_player_norm)) {

        dr_dprintf("HOORAY! Head-on imminent");
        return 1;
    }
    return 0;
}

// IDA: int __usercall AlreadyPursuingCar@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tCar_spec *pPursuee@<EDX>)
// FUNCTION: CARM95 0x00409e29
int AlreadyPursuingCar(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee) {

    return pOpponent_spec->current_objective == eOOT_pursue_and_twat && pOpponent_spec->pursue_car_data.pursuee == pPursuee;
}

// IDA: int __usercall LastTwatteeAPlayer@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00409e64
int LastTwatteeAPlayer(tOpponent_spec* pOpponent_spec) {

    return pOpponent_spec->car_spec->last_person_we_hit && pOpponent_spec->car_spec->last_person_we_hit->driver == eDriver_local_human;
}

// IDA: int __usercall LastTwatterAPlayer@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00409ea9
int LastTwatterAPlayer(tOpponent_spec* pOpponent_spec) {

    return pOpponent_spec->car_spec->last_person_to_hit_us && pOpponent_spec->car_spec->last_person_to_hit_us->driver == eDriver_local_human;
}

// IDA: void __usercall ObjectiveComplete(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00408e2d
void ObjectiveComplete(tOpponent_spec* pOpponent_spec) {

    dr_dprintf("%s: Objective Completed", pOpponent_spec->car_spec->driver_name);
    pOpponent_spec->new_objective_required = 1;
    switch (pOpponent_spec->current_objective) {
    case eOOT_complete_race:
        gNum_of_opponents_completing_race--;
        break;
    case eOOT_get_near_player:
        gNum_of_opponents_getting_near--;
        break;
    case eOOT_pursue_and_twat:
        gNum_of_opponents_pursuing--;
        break;
    default:
        break;
    }
}

// IDA: void __usercall TeleportOpponentToNearestSafeLocation(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00404464
void TeleportOpponentToNearestSafeLocation(tOpponent_spec* pOpponent_spec) {
    tS16 section_no;
    tU8 section_direction;
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    int section_counter;
    int found_safe_place;

    found_safe_place = 0;
    section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
    if (section_no < 0) {
        return;
    }
    pOpponent_spec->last_in_view = 0;
    TurnOpponentPhysicsOff(pOpponent_spec);
    section_direction = BrVector3Dot(&pOpponent_spec->car_spec->direction, &direction_v) < 0.0f;
    ClearOpponentsProjectedRoute(pOpponent_spec);
    AddToOpponentsProjectedRoute(pOpponent_spec, section_no, section_direction);
    TopUpRandomRoute(pOpponent_spec, -1);
    section_counter = 1;
    while (!found_safe_place) {
        BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[pOpponent_spec->next_sections[section_counter].section_no].node_indices[pOpponent_spec->next_sections[section_counter].direction]].p);
        CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(pOpponent_spec);
        if (pOpponent_spec->player_to_oppo_d > gIn_view_distance) {
            found_safe_place = 1;
        }
        section_counter++;
        if (pOpponent_spec->nnext_sections <= section_counter) {
            ShiftOpponentsProjectedRoute(pOpponent_spec, section_counter - 1);
            section_counter = 0;
            TopUpRandomRoute(pOpponent_spec, -1);
        }
    }
}

// IDA: void __usercall ChooseNewObjective(tOpponent_spec *pOpponent_spec@<EAX>, int pMust_choose_one@<EDX>)
// FUNCTION: CARM95 0x00408ec5
void ChooseNewObjective(tOpponent_spec* pOpponent_spec, int pMust_choose_one) {
    char str[255];
    tS16 players_section;
    br_vector3 wank;
    br_vector3 player_to_oppo_v;
    br_vector3 section_v;
    br_vector3 intersect;
    br_scalar dot;
    br_scalar distance;
    int do_it;
    int i;
    int j;
    int pursuit_percentage;
    int percentage;
    int general_grudge_increase;

    // v3 = pMust_choose_one;
    if (pOpponent_spec->current_objective == eOOT_knackered_and_freewheeling || pOpponent_spec->knackeredness_detected) {
        return;
    }
    if (gTime_stamp_for_this_munging > pOpponent_spec->next_out_of_world_check) {
        pOpponent_spec->next_out_of_world_check = gTime_stamp_for_this_munging + 500;
        if (HasCarFallenOffWorld(pOpponent_spec->car_spec)) {
            if (pOpponent_spec->car_spec->last_time_we_touched_a_player > gTime_stamp_for_this_munging - 7000) {
                TurnOpponentPhysicsOff(pOpponent_spec);
                pOpponent_spec->finished_for_this_race = 1;
                KnackerThisCar(pOpponent_spec->car_spec);
                pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[3][1] -= 1000.0f;
                return;
            } else {
                TeleportOpponentToNearestSafeLocation(pOpponent_spec);
                NewObjective(pOpponent_spec, eOOT_complete_race);
                return;
            }
        }
    }
    if (pOpponent_spec->car_spec->knackered && !pOpponent_spec->knackeredness_detected) {
        pOpponent_spec->knackeredness_detected = 1;
        dr_dprintf("%s: Knackered - dealing with appropriately", pOpponent_spec->car_spec->driver_name);
        if (pOpponent_spec->car_spec->has_been_stolen) {
            NewObjective(pOpponent_spec, eOOT_levitate);
        } else {
            NewObjective(pOpponent_spec, eOOT_knackered_and_freewheeling);
        }
        return;
    }
    if (pOpponent_spec->current_objective == eOOT_frozen) {
        if (CAR_SPEC_GET_SPEED_FACTOR(pOpponent_spec->car_spec) != 0.0f) {
            dr_dprintf("%s: Time to unfreeze", pOpponent_spec->car_spec->driver_name);
            if (pOpponent_spec->pursuing_player_before_freeze == 1) {
                NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
            } else {
                NewObjective(pOpponent_spec, eOOT_get_near_player);
            }
        }
    } else {
        if (CAR_SPEC_GET_SPEED_FACTOR(pOpponent_spec->car_spec) == 0.0f) {
            dr_dprintf("%s: Decided to freeze", pOpponent_spec->car_spec->driver_name);
            if (pOpponent_spec->current_objective == eOOT_pursue_and_twat && pOpponent_spec->pursue_car_data.pursuee == &gProgram_state.current_car) {
                pOpponent_spec->pursuing_player_before_freeze = 1;
            } else {
                pOpponent_spec->pursuing_player_before_freeze = 0;
            }
            NewObjective(pOpponent_spec, eOOT_frozen);
            return;
        }
        if (!gFirst_frame) {
            general_grudge_increase = (pOpponent_spec->nastiness * 40.0f + 10.0f);
            if (pOpponent_spec->car_spec->scary_bang && pOpponent_spec->player_to_oppo_d < 10.0f) {
                if (pOpponent_spec->current_objective == eOOT_pursue_and_twat) {
                    pursuit_percentage = 40;
                } else {
                    pursuit_percentage = 0;
                }
                if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec) && PercentageChance(20)) {
                    dr_dprintf("%s: Decided to run away", pOpponent_spec->car_spec->driver_name);
                    NewObjective(pOpponent_spec, eOOT_run_away);
                    return;
                } else if (PercentageChance((pursuit_percentage + 60) - pOpponent_spec->nastiness * 50.0f)) {
                    dr_dprintf("%s: Decided to run away", pOpponent_spec->car_spec->driver_name);
                    NewObjective(pOpponent_spec, eOOT_run_away);
                    return;
                }
            }
            if (!gMellow_opponents && (pOpponent_spec->current_objective != eOOT_run_away || pOpponent_spec->time_this_objective_started + 15000 <= gTime_stamp_for_this_munging)) {
                if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec) && pOpponent_spec->murder_reported && pOpponent_spec->player_to_oppo_d < 20.0f && !AlreadyPursuingCar(pOpponent_spec, &gProgram_state.current_car)) {
                    gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                    sprintf(str, "%s: Furderous melon!", pOpponent_spec->car_spec->driver_name);
                    dr_dprintf("%s: Decided to pursue after MURDER", pOpponent_spec->car_spec->driver_name);
                    NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                    return;
                }
                if (pOpponent_spec->car_spec->big_bang && LastTwatterAPlayer(pOpponent_spec) && !AlreadyPursuingCar(pOpponent_spec, pOpponent_spec->car_spec->last_person_to_hit_us)) {
                    gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                    sprintf(str, "%s: Christ! What was that?", pOpponent_spec->car_spec->driver_name);
                    dr_dprintf("%s: Decided to pursue after big bang; last person to twat us was %s", pOpponent_spec->car_spec->driver_name, pOpponent_spec->car_spec->last_person_to_hit_us->driver_name);
                    NewObjective(pOpponent_spec, eOOT_pursue_and_twat, pOpponent_spec->car_spec->last_person_to_hit_us);
                    return;
                }
                if (LastTwatteeAPlayer(pOpponent_spec) && !AlreadyPursuingCar(pOpponent_spec, pOpponent_spec->car_spec->last_person_we_hit)) {
                    gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                    sprintf(str, "%s: Ha! Bet you weren't expecting that!", pOpponent_spec->car_spec->driver_name);
                    dr_dprintf("%s: Decided to pursue %s after accidentally hitting them", pOpponent_spec->car_spec->driver_name, pOpponent_spec->car_spec->last_person_we_hit->driver_name);
                    NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                    return;
                }
                if (!AlreadyPursuingCar(pOpponent_spec, &gProgram_state.current_car)) {
                    if (pOpponent_spec->car_spec->grudge_raised_recently && (!CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec) || pOpponent_spec->player_to_oppo_d <= 20.f) && LastTwatterAPlayer(pOpponent_spec) && gOpponents[pOpponent_spec->index].psyche.grudge_against_player > 20) {
                        gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                        sprintf(str, "%s: Right! That's enough, %s!", pOpponent_spec->car_spec->driver_name, gProgram_state.current_car.driver_name);
                        dr_dprintf("%s: Decided to pursue after grudginess raised; last person to twat us was %s", pOpponent_spec->car_spec->driver_name, pOpponent_spec->car_spec->last_person_to_hit_us->driver_name);
                        NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                        return;
                    }

                    if (pOpponent_spec->player_in_view_now) {
                        do_it = 0;
                        if (pOpponent_spec->acknowledged_piv == 0) {

                            pOpponent_spec->acknowledged_piv = 1;
                            if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
                                percentage = (BrVector3Length(&gProgram_state.current_car.v) - gDefinite_no_cop_pursuit_speed) * gCop_pursuit_speed_percentage_multiplier;
                            } else if (gProgram_state.skill_level + 3 > gNum_of_opponents_pursuing) {
                                percentage = gOpponents[pOpponent_spec->index].psyche.grudge_against_player - 20 + pOpponent_spec->nastiness * 30.f;
                            } else {
                                percentage = 0;
                            }

                            percentage += 50 * HeadOnWithPlayerPossible(pOpponent_spec);
                            do_it = PercentageChance(percentage);
                            dr_dprintf("%s: Spotted player; chance of pursuing %d%%: %s", pOpponent_spec->car_spec->driver_name, percentage, do_it ? "YES, Decided to pursue" : "NO, Decided NOT to pursue");
                            if (do_it) {
                                gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100, MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + general_grudge_increase);
                                sprintf(str, "%s: I've decided to kill you for the fun of it", pOpponent_spec->car_spec->driver_name);
                                NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                                return;
                            }
                        }
                    }
                }
            }
        }
        if (pMust_choose_one) {
            dr_dprintf("%s: Choosing new objective because we have to...", pOpponent_spec->car_spec->driver_name, pOpponent_spec->car_spec->last_person_to_hit_us);
            if (pOpponent_spec->has_moved_at_some_point) {
                if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
                    NewObjective(pOpponent_spec, eOOT_return_to_start);
                    return;
                } else {
                    if (gNum_of_opponents_getting_near + gNum_of_opponents_pursuing < 3 && pOpponent_spec->player_to_oppo_d > 10.0f) {
                        dr_dprintf("%s: Choosing to get_near because not enough oppos are yet (%d/%d)", pOpponent_spec->car_spec->driver_name, gNum_of_opponents_getting_near + gNum_of_opponents_pursuing, 3);
                        NewObjective(pOpponent_spec, eOOT_get_near_player);
                        return;
                    }
                    if (gNum_of_opponents_completing_race < 2) {
                        dr_dprintf("%s: Choosing to complete_race because not enough oppos are yet (%d/%d)", pOpponent_spec->car_spec->driver_name, gNum_of_opponents_completing_race, 2);
                        NewObjective(pOpponent_spec, eOOT_complete_race);
                        return;
                    }

                    percentage = (int)pOpponent_spec->player_to_oppo_d - 15;
                    if (PercentageChance(percentage)) {
                        dr_dprintf("%s: Choosing to get_near because chance dictated it (%d%%)", pOpponent_spec->car_spec->driver_name, percentage);
                        NewObjective(pOpponent_spec, eOOT_get_near_player);
                        return;
                    } else {
                        dr_dprintf("%s: Choosing to complete_race because chance dictated it (%d%%)", pOpponent_spec->car_spec->driver_name, percentage);
                        NewObjective(pOpponent_spec, eOOT_complete_race);
                        return;
                    }
                }

            } else if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
                NewObjective(pOpponent_spec, eOOT_wait_for_some_hapless_sod);
                return;
            } else if (pOpponent_spec->pursue_from_start && !gMellow_opponents) {
                gOpponents[pOpponent_spec->index].psyche.grudge_against_player = MIN(100.f, pOpponent_spec->nastiness * 40.f + (MAX(20, gOpponents[pOpponent_spec->index].psyche.grudge_against_player) + 20));
                NewObjective(pOpponent_spec, eOOT_pursue_and_twat, &gProgram_state.current_car);
                return;

            } else {
                NewObjective(pOpponent_spec, eOOT_complete_race);
                return;
            }
        }
    }
}

// IDA: void __usercall ProcessThisOpponent(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00406458
void ProcessThisOpponent(tOpponent_spec* pOpponent_spec) {
    int i;

    if ((!gMap_mode || !gShow_opponents) && (pOpponent_spec->last_in_view + 3000 < gTime_stamp_for_this_munging)) {
        if (pOpponent_spec->cheating == 0) {
            StartToCheat(pOpponent_spec);
        }
    } else {
        if (pOpponent_spec->cheating) {
            OiStopCheating(pOpponent_spec);
        }
    }

    ChooseNewObjective(pOpponent_spec, pOpponent_spec->new_objective_required);
    pOpponent_spec->new_objective_required = 0;
    if (gCountdown || gRace_finished) {
        pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->M * 10.f;
    }
    if (!pOpponent_spec->finished_for_this_race && !gStop_opponents_moving && !gRace_finished && pOpponent_spec->stun_time_ends < gTime_stamp_for_this_munging) {
        ProcessCurrentObjective(pOpponent_spec, ePOC_run);
    }
    if (pOpponent_spec->cheating) {
        BrVector3Copy(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
    }
}

// IDA: int __usercall IsNetCarActive@<EAX>(br_vector3 *pPoint@<EAX>)
// FUNCTION: CARM95 0x004034b7
int IsNetCarActive(br_vector3* pPoint) {
    br_vector3 tv;

    BrVector3Sub(&tv, &gProgram_state.current_car.car_master_actor->t.t.translate.t, pPoint);
    if (BrVector3LengthSquared(&tv) < 100.f) {
        return 1;
    }
    if (gCar_to_view != &gProgram_state.current_car) {
        BrVector3Sub(&tv, &gCar_to_view->car_master_actor->t.t.translate.t, pPoint);
        if (BrVector3LengthSquared(&tv) < 100.f) {
            return 1;
        }
    }
    return 0;
}

// IDA: void __cdecl RebuildActiveCarList()
// FUNCTION: CARM95 0x004031c3
void RebuildActiveCarList(void) {
    int i;
    tCar_spec* car_spec;

    if (gActive_car_list_rebuild_required) {
        gActive_car_list_rebuild_required = 0;
        gNum_active_cars = 0;

        for (i = 0; i < 1; i++) {
            if (i > 0) {
                PDFatalError("mGet_car_count( eVehicle_self ) > 1 - I didn't know this could happen!");
            }

            car_spec = &gProgram_state.current_car;
            if (!car_spec->disabled || gAction_replay_mode) {
                gActive_car_list[gNum_active_cars] = car_spec;
                gNum_active_cars++;
                car_spec->active = 1;
            }
        }

        if (gNet_mode == eNet_mode_host) {
            for (i = 0; i < GetCarCount(eVehicle_net_player); i++) {
                car_spec = GetCarSpec(eVehicle_net_player, i);
                if (!car_spec->disabled) {
                    gActive_car_list[gNum_active_cars] = car_spec;
                    gNum_active_cars++;
                    car_spec->active = 1;
                } else {
                    car_spec->active = 0;
                }
            }
        } else if (gNet_mode == eNet_mode_client) {
            for (i = 0; i < GetCarCount(eVehicle_net_player); i++) {
                car_spec = GetCarSpec(eVehicle_net_player, i);
                if (!car_spec->disabled && IsNetCarActive(&car_spec->car_master_actor->t.t.translate.t)) {
                    gActive_car_list[gNum_active_cars] = car_spec;
                    gNum_active_cars++;
                    car_spec->active = 1;
                } else {
                    car_spec->active = 0;
                }
            }
        }
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
            car_spec = GetCarSpec(eVehicle_opponent, i);
            if (gProgram_state.AI_vehicles.opponents[i].physics_me || gAction_replay_mode) {
                gActive_car_list[gNum_active_cars] = car_spec;
                gNum_active_cars++;
                car_spec->active = 1;
            } else {
                car_spec->active = 0;
            }
        }
        for (i = 0; i < gNumber_of_cops_before_faffage; i++) {
            car_spec = GetCarSpec(eVehicle_rozzer, i);
            if (gProgram_state.AI_vehicles.cops[i].physics_me || gAction_replay_mode) {
                gActive_car_list[gNum_active_cars] = car_spec;
                gNum_active_cars++;
                car_spec->active = 1;
            } else {
            }
        }
    }
}

// IDA: void __cdecl ForceRebuildActiveCarList()
// FUNCTION: CARM95 0x0040319c
void ForceRebuildActiveCarList(void) {

    gActive_car_list_rebuild_required = 1;
    if (gProgram_state.racing) {
        RebuildActiveCarList();
    }
}

// IDA: void __usercall StartToCheat(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00409eee
void StartToCheat(tOpponent_spec* pOpponent_spec) {

    dr_dprintf("%s: StartToCheat() - Starting to cheat", pOpponent_spec->car_spec->driver_name);
    pOpponent_spec->cheating = 1;
    if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
        dr_dprintf("%s: StartToCheat() - Turning physics OFF", pOpponent_spec->car_spec->driver_name);
        TurnOpponentPhysicsOff(pOpponent_spec);
        RebuildActiveCarList();
    }
}

// IDA: void __usercall OiStopCheating(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00409f78
void OiStopCheating(tOpponent_spec* pOpponent_spec) {

    dr_dprintf("%s: OiStopCheating() - End of cheating sesh", pOpponent_spec->car_spec->driver_name);
    pOpponent_spec->cheating = 0;
    if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
        dr_dprintf("%s: OiStopCheating() - Turning physics ON", pOpponent_spec->car_spec->driver_name);
        TurnOpponentPhysicsOn(pOpponent_spec);
        RebuildActiveCarList();
    }
}

// IDA: int __usercall TeleportCopToStart@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00409ff7
int TeleportCopToStart(tOpponent_spec* pOpponent_spec) {
    br_vector3 wank;

    if (!pOpponent_spec->cheating || !CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
        return 0;
    }
    BrVector3Sub(&wank, &gProgram_state.current_car.car_master_actor->t.t.translate.t, &pOpponent_spec->start_pos);
    if (BrVector3Length(&wank) > gIn_view_distance) {
        BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &pOpponent_spec->start_pos);
        PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &pOpponent_spec->start_direction);
        pOpponent_spec->physics_me = 0;
        RematerialiseOpponent(pOpponent_spec, 0.0);
        TurnOpponentPhysicsOff(pOpponent_spec);
        RebuildActiveCarList();
        NewObjective(pOpponent_spec, eOOT_wait_for_some_hapless_sod);
        return 1;
    }

    return 0;
}

// IDA: void __usercall CalcDistanceFromHome(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x0040a13d
void CalcDistanceFromHome(tOpponent_spec* pOpponent_spec) {
    br_vector3 wank;

    BrVector3Sub(&wank, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &pOpponent_spec->start_pos);
    pOpponent_spec->distance_from_home = BrVector3Length(&wank);
}

// IDA: int __usercall MassageOpponentPosition@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, int pMassage_count@<EDX>)
// FUNCTION: CARM95 0x0040472e
int MassageOpponentPosition(tOpponent_spec* pOpponent_spec, int pMassage_count) {
    br_matrix34* mat;
    br_vector3* car_trans;
    br_vector3 displacement;
    br_vector3 positive_y_vector;
    br_vector3 direction_v;

    mat = &pOpponent_spec->car_spec->car_master_actor->t.t.mat;
    car_trans = &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t;
    if (pMassage_count > 22) {
        return 0;
    }

    if (pMassage_count > 20) {
        car_trans->v[1] += (pMassage_count - 20) * 2.0f;
    } else {
        direction_v.v[0] = -mat->m[2][0];
        direction_v.v[1] = -mat->m[2][1];
        direction_v.v[2] = -mat->m[2][2];
        if (pMassage_count % 4 < 2) {
            BrVector3Normalise(&displacement, &direction_v);
            BrVector3Scale(&displacement, &displacement, (pMassage_count / 4) * 0.5f);
        } else {
            BrVector3Set(&positive_y_vector, 0.f, 1.f, 0.f);
            BrVector3Cross(&displacement, &positive_y_vector, &direction_v);
            BrVector3Normalise(&displacement, &displacement);
            BrVector3Scale(&displacement, &displacement, (pMassage_count / 4) * 0.1f);
        }
        if (pMassage_count % 2) {
            BrVector3Negate(&displacement, &displacement);
        }
        BrVector3Accumulate(car_trans, &displacement);
    }

    return 1;
}

// IDA: int __usercall RematerialiseOpponentOnThisSection@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, br_scalar pSpeed, tS16 pSection_no)
// FUNCTION: CARM95 0x004035b1
int RematerialiseOpponentOnThisSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed, tS16 pSection_no) {
    br_vector3* start;
    br_vector3* finish;
    br_vector3 a;
    br_vector3 p;
    br_vector3 section_v;
    br_vector3 car_to_end;
    br_vector3 intersect;
    br_scalar t;
    br_scalar distance_to_end;
    br_scalar length;

    if (pOpponent_spec->physics_me) {
        dr_dprintf("%s: Actually, we're already materialised", pOpponent_spec->car_spec->driver_name);
        return 1;
    }
    start = GetOpponentsSectionStartNodePoint(pOpponent_spec, pSection_no);
    finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, pSection_no);
    BrVector3Sub(&a, finish, start);
    length = BrVector3Length(&a);
    if (length != 0.f) {
        BrVector3Sub(&p, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, start);
        t = BrVector3Dot(&a, &p) / BrVector3LengthSquared(&a);
        if (t < 0.f) {
            BrVector3Copy(&intersect, start);
        } else if (t > 1.f) {
            BrVector3Copy(&intersect, finish);
        } else {
            BrVector3Scale(&intersect, &a, t);
            BrVector3Add(&intersect, &intersect, start);
        }
        BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &intersect);
        BrVector3Sub(&section_v, finish, start);
        PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &section_v);
    }
    if (RematerialiseOpponent(pOpponent_spec, pSpeed)) {
        BrVector3Sub(&car_to_end, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        distance_to_end = BrVector3Length(&car_to_end);
        pOpponent_spec->car_spec->brake_force = 0.f;
        pOpponent_spec->car_spec->acc_force = 0.f;
        if (distance_to_end < 5.f) {
            pOpponent_spec->car_spec->brake_force = 15.f * pOpponent_spec->car_spec->M;
        } else {
            pOpponent_spec->car_spec->acc_force = pOpponent_spec->car_spec->M / 2.f;
        }
        pOpponent_spec->last_in_view = gTime_stamp_for_this_munging;
        return 1;
    } else {
        return 0;
    }
}

// IDA: int __usercall RematerialiseOpponentOnNearestSection@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, br_scalar pSpeed)
// FUNCTION: CARM95 0x004038fe
int RematerialiseOpponentOnNearestSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed) {
    br_vector3 intersect;
    br_vector3 direction_v;
    br_vector3 car_to_end;
    br_vector3* start;
    br_vector3* finish;
    br_scalar distance;
    br_scalar distance_to_end;
    tS16 section_no;

    if (pOpponent_spec->physics_me) {
        dr_dprintf("%s: Actually, we're already materialised", pOpponent_spec->car_spec->driver_name);
        return 1;
    }
    section_no = FindNearestPathSection(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &direction_v, &intersect, &distance);
    start = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].p;
    finish = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].p;
    BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &intersect);
    PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &direction_v);
    BrVector3Sub(&car_to_end, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
    if (RematerialiseOpponent(pOpponent_spec, pSpeed)) {
        pOpponent_spec->car_spec->brake_force = 0.0f;
        pOpponent_spec->car_spec->acc_force = 0.0f;
        distance_to_end = BrVector3Length(&car_to_end);

        if (distance_to_end < 5.0f) {
            pOpponent_spec->car_spec->brake_force = pOpponent_spec->car_spec->M * 15.0f;
        } else {
            pOpponent_spec->car_spec->acc_force = pOpponent_spec->car_spec->M / 2.0f;
        }
    }
    return 0;
}

// IDA: int __usercall RematerialiseOpponent@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, br_scalar pSpeed)
// FUNCTION: CARM95 0x00403ad9
int RematerialiseOpponent(tOpponent_spec* pOpponent_spec, br_scalar pSpeed) {
    // GLOBAL: CARM95 0x507174
    static int count;
    // GLOBAL: CARM95 0x507170
    static int total;
    // GLOBAL: CARM95 0x507178
    static int highest;
    int this_total;
    br_matrix34* mat;
    br_matrix34 original_mat;
    br_vector3 a;
    br_vector3 b;
    br_vector3 norm;
    br_vector3 norm2;
    br_scalar dist;
    br_scalar dist2;
    br_scalar ts;
    br_angle phi;
    int i;
    int j;
    int massage_count;
    br_angle theta;
    int sensible_place;

    this_total = 0;
    mat = &pOpponent_spec->car_spec->car_master_actor->t.t.mat;
    massage_count = 0;
    theta = -BrRadianToAngle(atan2(mat->m[2][2], mat->m[2][0])) + BR_ANGLE_DEG(90);
    if (pOpponent_spec->physics_me) {
        dr_dprintf("%s: Actually, we're already materialised", pOpponent_spec->car_spec->driver_name);
        return 1;
    }
    total++;
    BrMatrix34Copy(&original_mat, mat);
    TurnOpponentPhysicsOn(pOpponent_spec);
    RebuildActiveCarList();
    do {
        count++;
        this_total++;
        BrVector3Scale((br_vector3*)mat->m[3], (br_vector3*)mat->m[3], WORLD_SCALE_D);
        BrVector3Copy(&b, (br_vector3*)mat->m[3]);
        BrMatrix34RotateY(mat, theta);
        BrVector3Copy((br_vector3*)mat->m[3], &b);
        BrVector3SetFloat(&b, 0.f, -100.f, 0.f);
        BrVector3Copy(&a, (br_vector3*)mat->m[3]);
        a.v[1] += 1.f;
        findfloor(&a, &b, &norm, &dist);
        a.v[1] += 100.f;
        findfloor(&a, &b, &norm2, &dist2);
        if (dist2 <= 1.f) {
            BrVector3SetFloat(&b, 0.f, -5.01f, 0.f);
            a.v[1] -= 100.f;
            for (i = 0; i < 20; i++) {
                a.v[1] += 5.f;
                findfloor(&a, &b, &norm2, &dist2);
                if (dist2 <= 1.f) {
                    break;
                }
            }
            dist2 = (i + 1) * 0.05f + -dist2 / 20.f;
        }
        if (dist2 < dist) {
            dist = -dist2;
            BrVector3Copy(&norm, &norm2);
        }
        if (BR_ABS(dist) <= 1.f) {
            mat->m[3][1] -= dist * 100.f - 2.f;
            ts = BrVector3Dot((br_vector3*)mat->m[2], &norm);
            phi = BrRadianToAngle(asin(ts));
            BrMatrix34PreRotateX(mat, phi);
            ts = BrVector3Dot((br_vector3*)mat->m[0], &norm);
            phi = -BrRadianToAngle(asin(ts));
            BrMatrix34PreRotateZ(mat, phi);
        }
        BrVector3Negate(&pOpponent_spec->car_spec->direction, (br_vector3*)mat->m[2]);
        BrMatrix34ApplyP(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->cmpos, mat);
        BrVector3InvScale(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->pos, WORLD_SCALE);
        BrVector3InvScale((br_vector3*)mat->m[3], (br_vector3*)mat->m[3], WORLD_SCALE_D);
        BrVector3Copy(&pOpponent_spec->car_spec->v, (br_vector3*)pOpponent_spec->car_spec->car_master_actor->t.t.mat.m[2]);
        BrVector3Negate(&pOpponent_spec->car_spec->v, &pOpponent_spec->car_spec->v);
        BrVector3Normalise(&pOpponent_spec->car_spec->v, &pOpponent_spec->car_spec->v);
        BrVector3Scale(&pOpponent_spec->car_spec->v, &pOpponent_spec->car_spec->v, pSpeed * WORLD_SCALE_D);
        BrVector3Set(&pOpponent_spec->car_spec->omega, 0.f, 0.f, 0.f);
        pOpponent_spec->car_spec->curvature = 0.0;
        BrMatrix34Copy(&pOpponent_spec->car_spec->oldmat, mat);
        BrMatrix34Copy(&pOpponent_spec->car_spec->old_frame_mat, mat);
        BrVector3Scale((br_vector3*)pOpponent_spec->car_spec->oldmat.m[3], (br_vector3*)pOpponent_spec->car_spec->oldmat.m[3], WORLD_SCALE);
        for (j = 0; j < COUNT_OF(pOpponent_spec->car_spec->oldd); j++) {
            pOpponent_spec->car_spec->oldd[j] = pOpponent_spec->car_spec->ride_height;
        }
        pOpponent_spec->car_spec->gear = 0;
        pOpponent_spec->car_spec->revs = 0.f;
        pOpponent_spec->car_spec->traction_control = 1;
        BrMatrix34ApplyP(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->cmpos, mat);
        BrVector3InvScale(&pOpponent_spec->car_spec->pos, &pOpponent_spec->car_spec->pos, WORLD_SCALE);
        BrVector3Negate(&pOpponent_spec->car_spec->direction, (br_vector3*)pOpponent_spec->car_spec->oldmat.m[3]);
        pOpponent_spec->car_spec->box_face_ref = gFace_num__car - 2;
        pOpponent_spec->car_spec->doing_nothing_flag = 0;
        sensible_place = TestForCarInSensiblePlace(pOpponent_spec->car_spec);
        if (!sensible_place) {
            BrMatrix34Copy(mat, &original_mat);
        }
    } while (!sensible_place && MassageOpponentPosition(pOpponent_spec, massage_count++));

    count--;
    this_total--;
    if (sensible_place) {
        dr_dprintf("%s: Rematerialised (took %d attempts, orig. pos. (%7.3f,%7.3f,%7.3f), actual pos. (%7.3f,%7.3f,%7.3f))",
            pOpponent_spec->car_spec->driver_name,
            this_total + 1,
            original_mat.m[3][0], original_mat.m[3][1], original_mat.m[3][2],
            mat->m[3][0], mat->m[3][1], mat->m[3][2]);
    }
    if (this_total > highest) {
        highest = this_total;
    }
    if (count != 0) {
        dr_dprintf("MassageOpponentPosition() called an avg of %.1f times (max %d) per ReMaterialisation",
            count / (float)total, highest);
    }
    if (!sensible_place) {
        TurnOpponentPhysicsOff(pOpponent_spec);
        RebuildActiveCarList();
        TeleportOpponentToNearestSafeLocation(pOpponent_spec);
        return 1;
    }
    ResetCarSpecialVolume((tCollision_info*)pOpponent_spec->car_spec);
    return 1;
}

// IDA: void __usercall CalcPlayerConspicuousness(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x0040a1ab
void CalcPlayerConspicuousness(tOpponent_spec* pOpponent_spec) {
    br_vector3 pos_in_cop_space;
    br_matrix34 inverse_transform;

    if (pOpponent_spec->next_player_visibility_check < gTime_stamp_for_this_munging) {
        pOpponent_spec->player_in_view_now = 0;
        if (CAR_SPEC_IS_ROZZER(pOpponent_spec->car_spec)) {
            pOpponent_spec->next_player_visibility_check = gTime_stamp_for_this_munging + IRandomBetween(0, 900) + 100;
            if (pOpponent_spec->player_to_oppo_d < 20.f) {
                BrMatrix34LPInverse(&inverse_transform, &pOpponent_spec->car_spec->car_master_actor->t.t.mat);
                BrMatrix34ApplyP(&pos_in_cop_space, &gProgram_state.current_car.car_master_actor->t.t.translate.t, &inverse_transform);
                if (pos_in_cop_space.v[2] < 0.f && PointVisibleFromHere(&gProgram_state.current_car.car_master_actor->t.t.translate.t, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t)) {
                    pOpponent_spec->player_in_view_now = 1;
                    pOpponent_spec->acknowledged_piv = 0;
                }
            }
        } else {
            pOpponent_spec->next_player_visibility_check = gTime_stamp_for_this_munging + IRandomBetween(0, 900) + 6000;
            dr_dprintf("%s: Time now: %9.2f; next vis check at %9.2f", pOpponent_spec->car_spec->driver_name, gTime_stamp_for_this_munging / 1000.0, pOpponent_spec->next_player_visibility_check / 1000.0);
            if (pOpponent_spec->player_to_oppo_d < 50.f && PointVisibleFromHere(&gProgram_state.current_car.car_master_actor->t.t.translate.t, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t)) {
                pOpponent_spec->player_in_view_now = 1;
                pOpponent_spec->acknowledged_piv = 0;
            }
        }
    }
}

// IDA: void __usercall CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x00404a0e
void CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(tOpponent_spec* pOpponent_spec) {

    BrVector3Sub(&pOpponent_spec->player_to_oppo_v, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &gProgram_state.current_car.car_master_actor->t.t.translate.t);
    pOpponent_spec->player_to_oppo_d = BrVector3Length(&pOpponent_spec->player_to_oppo_v);
    if (pOpponent_spec->player_to_oppo_d < gIn_view_distance) {
        pOpponent_spec->last_in_view = gTime_stamp_for_this_munging;
    }
}

// IDA: void __usercall ChallengeOccurred(int pChallenger_index@<EAX>, int pAccepted@<EDX>)
// FUNCTION: CARM95 0x00404abc
void ChallengeOccurred(int pChallenger_index, int pAccepted) {

    if (pAccepted) {
        gChallenger_index__opponent = pChallenger_index;
    }
}

// IDA: void __cdecl LoadCopCars()
// FUNCTION: CARM95 0x00404ad9
void LoadCopCars(void) {
    int i;

    if (gProgram_state.AI_vehicles.number_of_cops != 0) {
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            PossibleService();
            gProgram_state.AI_vehicles.cops[i].car_spec = BrMemAllocate(sizeof(tCar_spec), kMem_cop_car_spec);
            LoadCar(
                gBIG_APC_index == i ? "BIGAPC.TXT" : "APC.TXT",
                eDriver_oppo,
                gProgram_state.AI_vehicles.cops[i].car_spec,
                (gBIG_APC_index == i) ? 4 : 3,
                "The Cops",
                &gTheir_cars_storage_space);
        }
    }
}

// IDA: void __usercall LoadInOppoPaths(FILE *pF@<EAX>)
// FUNCTION: CARM95 0x00404b9b
void LoadInOppoPaths(FILE* pF) {
    char s[256];
    char* res;
    int data_errors;
    int section_no;
    int node_no;
    int i;
    br_scalar x;
    br_scalar y;
    br_scalar z;
    br_scalar scalars[8];
    br_scalar distance;
    tPath_node* node_ptr;
    br_vector3 section_v;
    br_vector3 positive_y_vector;
    br_vector3 intersect;
    br_vector3 cop_to_section;
    int j;
    int sections_to_delete;
    int delete_these[1024];

    data_errors = 0;
    sections_to_delete = 0;
    dr_dprintf("Start of LoadInOppoPaths()...");
    gProgram_state.AI_vehicles.number_of_path_nodes = 0;
    gProgram_state.AI_vehicles.number_of_path_sections = 0;
    gProgram_state.AI_vehicles.path_nodes = 0;
    gProgram_state.AI_vehicles.path_sections = 0;
    gBit_per_node = 0;
    gBIG_APC_index = -1;
    do {
        res = GetALineAndDontArgue(pF, s);
    } while (res && strcmp("START OF OPPONENT PATHS", s) != 0);
    if (res == 0) {
        // return;

    } else {
        if (gNet_mode != eNet_mode_none) {
        }
        ReallocExtraPathNodes(GetAnInt(pF));
        if (gProgram_state.AI_vehicles.number_of_path_nodes != 0) {
            for (node_no = 0; node_no < gProgram_state.AI_vehicles.number_of_path_nodes; node_no++) {
                ReadThreeFloats(pF, x, y, z);
                gProgram_state.AI_vehicles.path_nodes[node_no].p.v[0] = x;
                gProgram_state.AI_vehicles.path_nodes[node_no].p.v[1] = y;
                gProgram_state.AI_vehicles.path_nodes[node_no].p.v[2] = z;
                gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections = 0;
            }
        }
        ReallocExtraPathSections(GetAnInt(pF));
        if (gProgram_state.AI_vehicles.number_of_path_sections != 0) {
            for (section_no = 0; section_no < gProgram_state.AI_vehicles.number_of_path_sections; section_no++) {
                PossibleService();
                GetNScalars(pF, 8, scalars);
                gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0] = scalars[0];
                gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] = scalars[1];
                gProgram_state.AI_vehicles.path_sections[section_no].min_speed[0] = scalars[2];
                gProgram_state.AI_vehicles.path_sections[section_no].max_speed[0] = scalars[3];
                gProgram_state.AI_vehicles.path_sections[section_no].min_speed[1] = scalars[4];
                gProgram_state.AI_vehicles.path_sections[section_no].max_speed[1] = scalars[5];
                gProgram_state.AI_vehicles.path_sections[section_no].width = scalars[6];

                BrVector3Sub(&section_v,
                    &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].p,
                    &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].p);

                gProgram_state.AI_vehicles.path_sections[section_no].length = BrVector3Length(&section_v);
                if (scalars[7] >= 1000.0f) {
                    gProgram_state.AI_vehicles.path_sections[section_no].type = (tU8)(scalars[7] - 1000.0f);
                    gProgram_state.AI_vehicles.path_sections[section_no].one_way = 1;
                } else {
                    gProgram_state.AI_vehicles.path_sections[section_no].type = (tU8)scalars[7];
                    gProgram_state.AI_vehicles.path_sections[section_no].one_way = 0;
                }
                for (node_no = 0; node_no < 2; node_no++) {
                    node_ptr = &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[node_no]];
                    if (node_ptr->number_of_sections < 8) {
                        node_ptr->sections[node_ptr->number_of_sections] = (tS16)section_no;
                        node_ptr->number_of_sections++;
                    } else {
                        dr_dprintf("ERROR: Too many sections (including section #%d) attached to node #%d",
                            section_no,
                            gProgram_state.AI_vehicles.path_sections[section_no].node_indices[node_no]);
                        data_errors = 1;
                    }
                }
                BrVector3Sub(&section_v,
                    &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].p,
                    &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].p);

                if (BrVector3LengthSquared(&section_v) == 0.0f) {
                    dr_dprintf(
                        "ERROR: Opponent path section #%d has zero length (nodes #%d and #%d are in same position). Secti"
                        "on and one node will be deleted.",
                        section_no,
                        gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0],
                        gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]);
                    delete_these[sections_to_delete] = section_no;
                    sections_to_delete++;
                }
            }
        }

        if (data_errors) {
            PDFatalError("Opponent path data inconsistencies. Unable to rectumify them.");
        }
        if (sections_to_delete != 0) {
            for (i = 0; i < sections_to_delete; i++) {
                dr_dprintf("Deleting section #%d (was #%d)", delete_these[i], i + delete_these[i]);
                DeleteSection(delete_these[i]);
                DeleteOrphanNodes();
                for (j = i; j < sections_to_delete; j++) {
                    delete_these[i]--;
                }
            }
            WriteOutOppoPaths();
            sprintf(
                s,
                "Errors in opponent path data. All have been corrected and written out to '%s'. Refer to diagnostic file "
                "for more details.",
                gOppo_path_filename);
            PDFatalError(s);
        }
        if (gAusterity_mode || gNet_mode != eNet_mode_none) {
            gProgram_state.AI_vehicles.number_of_cops = GetAnInt(pF);
            for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
                GetALineAndDontArgue(pF, s);
            }
            gProgram_state.AI_vehicles.number_of_cops = 0;
        } else {
            gProgram_state.AI_vehicles.number_of_cops = GetAnInt(pF);
            if (gProgram_state.AI_vehicles.number_of_cops != 0) {
                for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
                    PossibleService();
                    GetNScalars(pF, 6, scalars);
                    BrVector3Set(&gProgram_state.AI_vehicles.cop_start_points[i], scalars[0], scalars[1], scalars[2]);

                    if (scalars[3] == 9.0f && scalars[4] == 9.0f && scalars[5] == 9.0f) {
                        gBIG_APC_index = i;
                    }

                    section_no = FindNearestPathSection(&gProgram_state.AI_vehicles.cop_start_points[i], &section_v, &intersect, &distance);
                    BrVector3Set(&positive_y_vector, 0.0, 1.0, 0.0);
                    BrVector3Cross(&gProgram_state.AI_vehicles.cop_start_vectors[i], &positive_y_vector, &section_v);
                    BrVector3Sub(&cop_to_section, &intersect, &gProgram_state.AI_vehicles.cop_start_points[i]);
                    if (BrVector3Dot(&gProgram_state.AI_vehicles.cop_start_vectors[i], &cop_to_section) < 0.0f) {
                        BrVector3Negate(&gProgram_state.AI_vehicles.cop_start_vectors[i], &gProgram_state.AI_vehicles.cop_start_vectors[i]);
                    }
                }
            }
        }
        do {
            GetALineAndDontArgue(pF, s);
        } while (strcmp("END OF OPPONENT PATHS", s) != 0);
        if (gProgram_state.AI_vehicles.number_of_path_sections != 0) {
            gBit_per_node = BrMemAllocate((gProgram_state.AI_vehicles.number_of_path_nodes + 7) / 8, kMem_oppo_bit_per_node);
        } else {
            gBit_per_node = NULL;
        }
        dr_dprintf("End of LoadInOppoPaths(), totals:");
        dr_dprintf("Nodes: %d", gProgram_state.AI_vehicles.number_of_path_nodes);
        dr_dprintf("Sections: %d", gProgram_state.AI_vehicles.number_of_path_sections);
        ConsistencyCheck();
    }
}

// IDA: void __cdecl DisposeOpponentPaths()
// FUNCTION: CARM95 0x00405912
void DisposeOpponentPaths(void) {

    if (gProgram_state.AI_vehicles.path_nodes != NULL) {
        BrMemFree(gProgram_state.AI_vehicles.path_nodes);
    }
    if (gProgram_state.AI_vehicles.path_sections != NULL) {
        BrMemFree(gProgram_state.AI_vehicles.path_sections);
    }
    if (gBit_per_node != NULL) {
        BrMemFree(gBit_per_node);
    }

    gProgram_state.AI_vehicles.number_of_path_nodes = 0;
    gProgram_state.AI_vehicles.number_of_path_sections = 0;
    gProgram_state.AI_vehicles.path_nodes = NULL;
    gProgram_state.AI_vehicles.path_sections = NULL;
    gBit_per_node = NULL;
}

// IDA: void __usercall MungeOpponents(tU32 pFrame_period@<EAX>)
// FUNCTION: CARM95 0x004059a0
void MungeOpponents(tU32 pFrame_period) {
    int i;
    int un_stun_flag;

    un_stun_flag = 0;

    if (gProgram_state.AI_vehicles.number_of_opponents == 0 && gNumber_of_cops_before_faffage == 0) {
        return;
    }
    gAcme_frame_count++;
    gTime_stamp_for_this_munging = GetTotalTime();
    gFrame_period_for_this_munging = pFrame_period;
    gFrame_period_for_this_munging_in_secs = gFrame_period_for_this_munging / 1000.f;
    if (!gAcknowledged_start && !gCountdown) {
        gAcknowledged_start = 1;
        if (!gStart_jumped) {
            un_stun_flag = 1;
        }
    }
    if (gProgram_state.current_car.no_of_processes_recording_my_trail != 0) {
        RecordNextTrailNode(&gProgram_state.current_car);
    } else {
        StartRecordingTrail(&gProgram_state.current_car);
    }
    TrackElasticateyPath();
    if (!gProcessing_opponents) {
        return;
    }
    gNum_of_opponents_pursuing = 0;
    gNum_of_opponents_getting_near = 0;
    gNum_of_opponents_completing_race = 0;
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        if (!gProgram_state.AI_vehicles.opponents[i].finished_for_this_race) {
            if (gProgram_state.AI_vehicles.opponents[i].current_objective == eOOT_pursue_and_twat) {
                gNum_of_opponents_pursuing++;
            } else if (gProgram_state.AI_vehicles.opponents[i].current_objective == eOOT_get_near_player) {

                gNum_of_opponents_getting_near++;

            } else if (gProgram_state.AI_vehicles.opponents[i].current_objective == eOOT_complete_race) {
                gNum_of_opponents_completing_race++;
            }
        }
    }
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        if (!gProgram_state.AI_vehicles.opponents[i].finished_for_this_race) {
            if (un_stun_flag) {
                UnStunTheBugger(&gProgram_state.AI_vehicles.opponents[i]);
            }
            CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(&gProgram_state.AI_vehicles.opponents[i]);
            CalcPlayerConspicuousness(&gProgram_state.AI_vehicles.opponents[i]);
            ProcessThisOpponent(&gProgram_state.AI_vehicles.opponents[i]);
            ClearTwattageOccurrenceVariables(&gProgram_state.AI_vehicles.opponents[i]);
        }
    }
    for (i = 0; i < gNumber_of_cops_before_faffage; i++) {
        if (!gProgram_state.AI_vehicles.cops[i].finished_for_this_race) {
            if (un_stun_flag) {
                UnStunTheBugger(&gProgram_state.AI_vehicles.cops[i]);
            }
            CalcDistanceFromHome(&gProgram_state.AI_vehicles.cops[i]);
            CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(&gProgram_state.AI_vehicles.cops[i]);
            CalcPlayerConspicuousness(&gProgram_state.AI_vehicles.cops[i]);
            ProcessThisOpponent(&gProgram_state.AI_vehicles.cops[i]);
            ClearTwattageOccurrenceVariables(&gProgram_state.AI_vehicles.cops[i]);
            gProgram_state.AI_vehicles.cops[i].murder_reported = 0;
        }
    }
    if (gNext_grudge_reduction < gTime_stamp_for_this_munging) {
        gNext_grudge_reduction = gTime_stamp_for_this_munging + 3000;
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
            if (!gProgram_state.AI_vehicles.opponents[i].finished_for_this_race) {
                gOpponents[gProgram_state.AI_vehicles.opponents[i].index].psyche.grudge_against_player -= MIN(gOpponents[gProgram_state.AI_vehicles.opponents[i].index].psyche.grudge_against_player, gGrudge_reduction_per_period);
            }
        }
    }
    RebuildActiveCarList();
    gFirst_frame = 0;
}

// IDA: void __cdecl SetInitialCopPositions()
// FUNCTION: CARM95 0x0040a3a5
void SetInitialCopPositions(void) {
    int i;

    for (i = 0; i < GetCarCount(eVehicle_rozzer); i++) {
        PossibleService();
        BrVector3Copy(&gProgram_state.AI_vehicles.cops[i].car_spec->car_master_actor->t.t.translate.t, &gProgram_state.AI_vehicles.cop_start_points[i]);
        PointActorAlongThisBloodyVector(gProgram_state.AI_vehicles.cops[i].car_spec->car_master_actor, &gProgram_state.AI_vehicles.cop_start_vectors[i]);
        gProgram_state.AI_vehicles.cops[i].physics_me = 0;
        RematerialiseOpponent(&gProgram_state.AI_vehicles.cops[i], 0.f);
        InitCarSkidStuff(gProgram_state.AI_vehicles.cops[i].car_spec);
    }
}

// IDA: void __usercall InitOpponents(tRace_info *pRace_info@<EAX>)
// FUNCTION: CARM95 0x0040a4ce
void InitOpponents(tRace_info* pRace_info) {
    int i;
    int opponent_number;
    int rank_dependent_difficulty;
    int skill_dependent_difficulty;
    br_bounds bounds;

    gNext_grudge_reduction = gTime_stamp_for_this_munging + 8000;
    gGrudge_reduction_per_period = 3 - gProgram_state.skill_level;
    gProcessing_opponents = 1;
    gFirst_frame = 1;
    gAcknowledged_start = 0;
    gStart_jumped = 0;
    gNum_viewable_cars = 0;
    gViewable_car_list[gNum_viewable_cars] = &gProgram_state.current_car;
    gNum_viewable_cars++;
    BrActorToBounds(&bounds, gProgram_state.track_spec.the_actor);
    gMinimum_yness_before_knackerisation = bounds.min.v[1] - 2.f;
    gDefinite_no_cop_pursuit_speed = 17.8788f;
    gDefinite_cop_pursuit_speed = 44.697f;
    gCop_pursuit_speed_percentage_multiplier = 100.f / (gDefinite_cop_pursuit_speed - gDefinite_no_cop_pursuit_speed);
    gHead_on_cos_value = cos(0.5235668420791626f);
    gAcme_frame_count = 0;
    gProgram_state.current_car.no_of_processes_recording_my_trail = 0;
    rank_dependent_difficulty = (101.0 - (gCurrent_race.suggested_rank < 10 ? 0.5 : gCurrent_race.suggested_rank)) / 10.0;
    skill_dependent_difficulty = gProgram_state.skill_level * 5;
    gBig_bang = 70.f - (2 * skill_dependent_difficulty + 3 * rank_dependent_difficulty) * gOpponent_nastyness_frigger;
    gIn_view_distance = MAX(gCamera_yon + 10.f, 45.f);
    gTime_stamp_for_this_munging = GetTotalTime();
    gFrame_period_for_this_munging = 1;
    gFrame_period_for_this_munging_in_secs = gFrame_period_for_this_munging / 1000.f;
    if (gNet_mode != eNet_mode_none) {
        gProgram_state.AI_vehicles.number_of_opponents = 0;
    } else {
        gProgram_state.AI_vehicles.number_of_opponents = pRace_info->number_of_racers - 1;
    }
    gNumber_of_cops_before_faffage = gProgram_state.AI_vehicles.number_of_cops;
    for (i = 0, opponent_number = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++, opponent_number++) {
        PossibleService();
        if (pRace_info->opponent_list[opponent_number].index < 0) {
            opponent_number++;
        }
        gProgram_state.AI_vehicles.opponents[i].car_spec = pRace_info->opponent_list[opponent_number].car_spec;
        gProgram_state.AI_vehicles.opponents[i].car_spec->car_ID = i + 0x200;
        dr_dprintf("Car '%s', car_ID %x",
            gProgram_state.AI_vehicles.opponents[i].car_spec->driver_name,
            gProgram_state.AI_vehicles.opponents[i].car_spec->car_ID);
        gProgram_state.AI_vehicles.opponents[i].index = pRace_info->opponent_list[opponent_number].index;
        gProgram_state.AI_vehicles.opponents[i].time_last_processed = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.opponents[i].time_this_objective_started = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.opponents[i].last_moved_ok = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.opponents[i].last_in_view = 0;
        gProgram_state.AI_vehicles.opponents[i].stun_time_ends = 0;
        gProgram_state.AI_vehicles.opponents[i].next_player_visibility_check = gTime_stamp_for_this_munging + IRandomBetween(0, 900) + 2000;
        gProgram_state.AI_vehicles.opponents[i].next_out_of_world_check = gTime_stamp_for_this_munging + 500;
        gProgram_state.AI_vehicles.opponents[i].cunting_buttfuck_timer = 0;
        gProgram_state.AI_vehicles.opponents[i].finished_for_this_race = 0;
        gProgram_state.AI_vehicles.opponents[i].physics_me = 1;
        gProgram_state.AI_vehicles.opponents[i].pursue_from_start = 0;
        gProgram_state.AI_vehicles.opponents[i].cheating = 0;
        gProgram_state.AI_vehicles.opponents[i].knackeredness_detected = 0;
        gProgram_state.AI_vehicles.opponents[i].players_section_when_last_calced_full_path = -1;
        gProgram_state.AI_vehicles.opponents[i].car_spec->last_person_to_hit_us = NULL;
        gProgram_state.AI_vehicles.opponents[i].car_spec->last_person_we_hit = NULL;
        gProgram_state.AI_vehicles.opponents[i].car_spec->last_collision_time = 0;
        gProgram_state.AI_vehicles.opponents[i].car_spec->last_time_we_touched_a_player = 0;
        gProgram_state.AI_vehicles.opponents[i].car_spec->grudge_raised_recently = 1;
        gProgram_state.AI_vehicles.opponents[i].car_spec->no_of_processes_recording_my_trail = 0;
        gProgram_state.AI_vehicles.opponents[i].nnext_sections = 0;
        gProgram_state.AI_vehicles.opponents[i].new_objective_required = 1;
        gProgram_state.AI_vehicles.opponents[i].current_objective = eOOT_none;
        gProgram_state.AI_vehicles.opponents[i].has_moved_at_some_point = 0;
        gProgram_state.AI_vehicles.opponents[i].player_in_view_now = 0;
        gProgram_state.AI_vehicles.opponents[i].acknowledged_piv = 0;

        gProgram_state.AI_vehicles.opponents[i].nastiness = ((99.0 - (gCurrent_race.suggested_rank < 10 ? 0.5 : gCurrent_race.suggested_rank)) / 98.0
                                                                + (gOpponents[gProgram_state.AI_vehicles.opponents[i].index].strength_rating - 1) / 4.0
                                                                + gProgram_state.skill_level / 2.0)
            / 3.0 * gOpponent_nastyness_frigger;
        BrVector3Set(&gProgram_state.AI_vehicles.opponents[i].pos_last_frame, 0.f, 0.f, 0.f);
        gOpponents[gProgram_state.AI_vehicles.opponents[i].index].psyche.grudge_against_player = 10;
        gViewable_car_list[gNum_viewable_cars] = gProgram_state.AI_vehicles.opponents[i].car_spec;
        gNum_viewable_cars++;
        StunTheBugger(&gProgram_state.AI_vehicles.opponents[i], 10000);
    }
    if (gChallenger_index__opponent >= 0) {
        tCar_spec* car_spec;
        tOpponent_spec* opponent_spec;
        car_spec = GetCarSpecFromGlobalOppoIndex(gChallenger_index__opponent);
        opponent_spec = GetOpponentSpecFromCarSpec(car_spec);
        if (opponent_spec != NULL) {
            opponent_spec->pursue_from_start = 1;
        } else {
            dr_dprintf("ERROR - can't record dare - no opponent_spec for car_spec");
        }
        gChallenger_index__opponent = -1;
    }
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
        PossibleService();
        gProgram_state.AI_vehicles.cops[i].car_spec->car_ID = i + 0x300;
        gProgram_state.AI_vehicles.cops[i].index = 3;
        gProgram_state.AI_vehicles.cops[i].time_last_processed = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.cops[i].time_this_objective_started = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.cops[i].last_moved_ok = gTime_stamp_for_this_munging;
        gProgram_state.AI_vehicles.cops[i].last_in_view = 0;
        gProgram_state.AI_vehicles.cops[i].stun_time_ends = 0;
        gProgram_state.AI_vehicles.cops[i].next_player_visibility_check = gTime_stamp_for_this_munging + IRandomBetween(0, 900) + 5000;
        gProgram_state.AI_vehicles.cops[i].next_out_of_world_check = gTime_stamp_for_this_munging + 500;
        gProgram_state.AI_vehicles.cops[i].cunting_buttfuck_timer = 0;
        gProgram_state.AI_vehicles.cops[i].finished_for_this_race = 0;
        gProgram_state.AI_vehicles.cops[i].physics_me = 1;
        gProgram_state.AI_vehicles.cops[i].pursue_from_start = 0;
        gProgram_state.AI_vehicles.cops[i].cheating = 0;
        gProgram_state.AI_vehicles.cops[i].murder_reported = 0;
        gProgram_state.AI_vehicles.cops[i].knackeredness_detected = 0;
        gProgram_state.AI_vehicles.cops[i].players_section_when_last_calced_full_path = -1;
        gProgram_state.AI_vehicles.cops[i].nnext_sections = 0;
        gProgram_state.AI_vehicles.cops[i].new_objective_required = 1;
        gProgram_state.AI_vehicles.cops[i].current_objective = eOOT_none;
        gProgram_state.AI_vehicles.cops[i].has_moved_at_some_point = 0;
        gProgram_state.AI_vehicles.cops[i].player_in_view_now = 0;
        gProgram_state.AI_vehicles.cops[i].acknowledged_piv = 0;
#ifdef DETHRACE_FIX_BUGS
        gProgram_state.AI_vehicles.cops[i].nastiness =
#else
        gProgram_state.AI_vehicles.opponents[i].nastiness =
#endif
            ((99.0 - (gCurrent_race.suggested_rank < 10 ? 0.5 : gCurrent_race.suggested_rank)) / 98.0
                + gProgram_state.skill_level / 2.0 + 2.25)
            / 3.f * gOpponent_nastyness_frigger;

        BrVector3Copy(&gProgram_state.AI_vehicles.cops[i].start_pos, &gProgram_state.AI_vehicles.cop_start_points[i]);
        BrVector3Copy(&gProgram_state.AI_vehicles.cops[i].start_direction, &gProgram_state.AI_vehicles.cop_start_vectors[i]);
        BrVector3Set(&gProgram_state.AI_vehicles.cops[i].pos_last_frame, 0.f, 0.f, 0.f);
        gViewable_car_list[gNum_viewable_cars] = gProgram_state.AI_vehicles.cops[i].car_spec;
        gNum_viewable_cars++;
        gProgram_state.AI_vehicles.cops[i].car_spec->last_person_to_hit_us = NULL;
        gProgram_state.AI_vehicles.cops[i].car_spec->last_person_we_hit = NULL;
        gProgram_state.AI_vehicles.cops[i].car_spec->last_collision_time = 0;
        gProgram_state.AI_vehicles.cops[i].car_spec->last_time_we_touched_a_player = 0;
        gProgram_state.AI_vehicles.cops[i].car_spec->no_of_processes_recording_my_trail = 0;
        gProgram_state.AI_vehicles.cops[i].car_spec->grudge_raised_recently = 1;
        gOpponents[gProgram_state.AI_vehicles.cops[i].index].psyche.grudge_against_player = 10;
        StunTheBugger(&gProgram_state.AI_vehicles.cops[i], 10000);
    }
    gActive_car_list_rebuild_required = 1;
    RebuildActiveCarList();
}

// IDA: void __cdecl DisposeOpponents()
// FUNCTION: CARM95 0x0040b186
void DisposeOpponents(void) {
    int i;
    if (gProgram_state.AI_vehicles.number_of_cops != 0) {
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            DisposeCar(gProgram_state.AI_vehicles.cops[i].car_spec, (i == gBIG_APC_index) ? 4 : 3);
            BrMemFree(gProgram_state.AI_vehicles.cops[i].car_spec);
        }
    }
}

// IDA: void __usercall WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(int pWhat_the_countdown_was@<EAX>)
// FUNCTION: CARM95 0x0040b20f
void WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(int pWhat_the_countdown_was) {
    int i;

    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        UnStunTheBugger(&gProgram_state.AI_vehicles.opponents[i]);
        StunTheBugger(&gProgram_state.AI_vehicles.opponents[i], MIN(IRandomBetween(1000, 2500), 1000 * pWhat_the_countdown_was));
    }
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
        UnStunTheBugger(&gProgram_state.AI_vehicles.cops[i]);
        StunTheBugger(&gProgram_state.AI_vehicles.cops[i], MIN(IRandomBetween(1000, 2500), 1000 * pWhat_the_countdown_was));
    }
    gAcknowledged_start = 1;
    gStart_jumped = 1;
}

// IDA: void __usercall ReportMurderToPoliceDepartment(tCar_spec *pCar_spec@<EAX>)
// FUNCTION: CARM95 0x0040b3a5
void ReportMurderToPoliceDepartment(tCar_spec* pCar_spec) {
    int i;

    if (pCar_spec == &gProgram_state.current_car) {
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            gProgram_state.AI_vehicles.cops[i].murder_reported = 1;
        }
    }
}

// IDA: int __usercall GetCarCount@<EAX>(tVehicle_type pCategory@<EAX>)
// FUNCTION: CARM95 0x0040b412
int GetCarCount(tVehicle_type pCategory) {

    switch (pCategory) {
    case eVehicle_self:
        return 1;

    case eVehicle_opponent:
        return gProgram_state.AI_vehicles.number_of_opponents;

    case eVehicle_net_player:
        return gNet_mode ? gNumber_of_net_players - 1 : 0;

    case eVehicle_rozzer:
        return gNumber_of_cops_before_faffage;

    case eVehicle_drone:
        return 0;

    case eVehicle_not_really:
        return gNum_active_non_cars;
    }

    return 0;
}

// IDA: tCar_spec* __usercall GetCarSpec@<EAX>(tVehicle_type pCategory@<EAX>, int pIndex@<EDX>)
// FUNCTION: CARM95 0x0040b4b1
tCar_spec* GetCarSpec(tVehicle_type pCategory, int pIndex) {

    switch (pCategory) {

    case eVehicle_self:
        return &gProgram_state.current_car;

    case eVehicle_net_player:
        return pIndex >= gThis_net_player_index ? gNet_players[pIndex + 1].car : gNet_players[pIndex].car;

    case eVehicle_opponent:
        return gProgram_state.AI_vehicles.opponents[pIndex].car_spec;

    case eVehicle_rozzer:
        return gProgram_state.AI_vehicles.cops[pIndex].car_spec;

    case eVehicle_drone:
        PDEnterDebugger("OPPONENT.C: GetCarSpec() can't return drone car_specs");
        return 0;

    case eVehicle_not_really:
        return (tCar_spec*)gActive_non_car_list[pIndex];
    }
    return 0;
}

// IDA: char* __usercall GetDriverName@<EAX>(tVehicle_type pCategory@<EAX>, int pIndex@<EDX>)
// FUNCTION: CARM95 0x0040b592
char* GetDriverName(tVehicle_type pCategory, int pIndex) {

    switch (pCategory) {
    case eVehicle_self:
        return gProgram_state.player_name[gProgram_state.frank_or_anniness];
    case eVehicle_opponent:
        return gOpponents[gProgram_state.AI_vehicles.opponents[pIndex].index].name;
    case eVehicle_rozzer:
        return gCop_name;
    case eVehicle_drone:
        return gDrone_name;
    case eVehicle_not_really:
        return NULL;
    }
    return NULL;
}

// IDA: tOpponent_spec* __usercall GetOpponentSpecFromCarSpec@<EAX>(tCar_spec *pCar_spec@<EAX>)
// FUNCTION: CARM95 0x0040b639
tOpponent_spec* GetOpponentSpecFromCarSpec(tCar_spec* pCar_spec) {
    int i;

    if (CAR_SPEC_IS_OPPONENT(pCar_spec)) {
        for (i = 0; i < GetCarCount(eVehicle_opponent); i++) {
            if (gProgram_state.AI_vehicles.opponents[i].car_spec == pCar_spec) {
                return &gProgram_state.AI_vehicles.opponents[i];
            }
        }
    } else if (CAR_SPEC_IS_ROZZER(pCar_spec)) {
        for (i = 0; i < GetCarCount(eVehicle_rozzer); i++) {
            if (gProgram_state.AI_vehicles.cops[i].car_spec == pCar_spec) {
                return &gProgram_state.AI_vehicles.cops[i];
            }
        }
    }
    return NULL;
}

// IDA: tCar_spec* __usercall GetCarSpecFromGlobalOppoIndex@<EAX>(int pIndex@<EAX>)
// FUNCTION: CARM95 0x0040b74d
tCar_spec* GetCarSpecFromGlobalOppoIndex(int pIndex) {
    int i;

    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        if (gProgram_state.AI_vehicles.opponents[i].index == pIndex) {
            return gProgram_state.AI_vehicles.opponents[i].car_spec;
        }
    }
    return NULL;
}

// IDA: int __usercall GetOpponentsRealSection@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, int pSection_no@<EDX>)
// FUNCTION: CARM95 0x0040b7b7
int GetOpponentsRealSection(tOpponent_spec* pOpponent_spec, int pSection_no) {

    if (pSection_no >= 20000) {
        return pOpponent_spec->next_sections[pSection_no - 20000].section_no;
    } else if (pSection_no < 10000) {
        return pSection_no;
    }
    return -1;
}

// IDA: int __usercall GetOpponentsFirstSection@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x0040b806
int GetOpponentsFirstSection(tOpponent_spec* pOpponent_spec) {

    if (pOpponent_spec->current_objective == eOOT_pursue_and_twat) {
        if (pOpponent_spec->pursue_car_data.state == ePCS_following_trail) {
            return pOpponent_spec->follow_path_data.section_no;
        } else if (pOpponent_spec->pursue_car_data.state == ePCS_following_line_of_sight) {
            return 10000;
        }
    }
    return 20000;
}

// IDA: int __usercall GetOpponentsNextSection@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pCurrent_section@<EDX>)
// FUNCTION: CARM95 0x0040b86e
int GetOpponentsNextSection(tOpponent_spec* pOpponent_spec, tS16 pCurrent_section) {

    if (pCurrent_section < 20000) {
        if (pCurrent_section < 15000) {
            return -1;
        } else {
            return CalcNextTrailSection(pOpponent_spec, pCurrent_section);
        }
    } else if (pCurrent_section - 19999 >= pOpponent_spec->nnext_sections || (!pOpponent_spec->cheating && gProgram_state.AI_vehicles.path_sections[pCurrent_section - 19999].type == ePST_cheat_only)) {
        return -1;
    } else {
        return pCurrent_section + 1;
    }
}

// IDA: tS16 __usercall GetOpponentsSectionStartNode@<AX>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pSection@<EDX>)
// FUNCTION: CARM95 0x0040b91f
tS16 GetOpponentsSectionStartNode(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    int node_index_index;

    if (pSection >= 20000 && pSection - 20000 < pOpponent_spec->nnext_sections) {
        node_index_index = pOpponent_spec->next_sections[pSection - 20000].direction == 0;
        if (pSection - 20000 > pOpponent_spec->nnext_sections) {
            section_no = -1;
        } else {
            section_no = gProgram_state.AI_vehicles.path_sections[pOpponent_spec->next_sections[pSection - 20000].section_no].node_indices[node_index_index];
            return section_no;
        }
    }
    dr_dprintf("BIG ERROR - GetOpponentsSectionStartNode() - section not found in next_section array for opponent %s",
        pOpponent_spec->car_spec->driver_name);
    PDEnterDebugger("BIG ERROR - GetOpponentsSectionStartNode()");
    return -1;
}

// IDA: tS16 __usercall GetOpponentsSectionFinishNode@<AX>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pSection@<EDX>)
// FUNCTION: CARM95 0x0040b9fb
tS16 GetOpponentsSectionFinishNode(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    int node_index_index;

    if (pSection >= 20000 && pSection - 20000 < pOpponent_spec->nnext_sections) {
        return gProgram_state.AI_vehicles.path_sections[pOpponent_spec->next_sections[pSection - 20000].section_no].node_indices[pOpponent_spec->next_sections[pSection - 20000].direction];
    }
    dr_dprintf("BIG ERROR - GetOpponentsSectionFinishNode() - section not found in next_section array for opponent %s",
        pOpponent_spec->car_spec->driver_name);
    PDEnterDebugger("BIG ERROR - GetOpponentsSectionFinishNode()");
    return 0;
}

// IDA: br_vector3* __usercall GetOpponentsSectionStartNodePoint@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pSection@<EDX>)
// FUNCTION: CARM95 0x0040baa1
br_vector3* GetOpponentsSectionStartNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    tS16 node_no;
    int node_index_index;

    if (pSection >= 20000 && pOpponent_spec->nnext_sections > pSection - 20000) {
        section_no = pOpponent_spec->next_sections[pSection - 20000].section_no;
        node_index_index = pOpponent_spec->next_sections[pSection - 20000].direction;
        node_no = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[node_index_index == 0];
        return &gProgram_state.AI_vehicles.path_nodes[node_no].p;
    }

    if (pSection >= 15000) {
        return &pOpponent_spec->pursue_car_data.pursuee->my_trail.trail_nodes[pSection - 15000];
    }
    if (pSection == 10000) {
        return &pOpponent_spec->pursue_car_data.direct_line_nodes[0].p;
    }
    dr_dprintf("BIG ERROR - GetOpponentsSectionStartNodePoint() - section not found in next_section array for opponent %s", pOpponent_spec->car_spec->driver_name);
    PDEnterDebugger("BIG ERROR - GetOpponentsSectionStartNodePoint()");
    return 0;
}

// IDA: br_vector3* __usercall GetOpponentsSectionFinishNodePoint@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pSection@<EDX>)
// FUNCTION: CARM95 0x0040bbc3
br_vector3* GetOpponentsSectionFinishNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    tS16 node_no;
    int node_index_index;

    if (pSection >= 20000 && pOpponent_spec->nnext_sections > pSection - 20000) {
        section_no = pOpponent_spec->next_sections[pSection - 20000].section_no;
        node_index_index = pOpponent_spec->next_sections[pSection - 20000].direction;
        node_no = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[node_index_index];
        return &gProgram_state.AI_vehicles.path_nodes[node_no].p;
    } else if (pSection >= 15000) {
        return &pOpponent_spec->pursue_car_data.pursuee->my_trail.trail_nodes[(pSection + 1) - 15000];
    } else if (pSection == 10000) {
        return &pOpponent_spec->pursue_car_data.direct_line_nodes[1].p;
    } else {
        dr_dprintf("BIG ERROR - GetOpponentsSectionFinishNodePoint() - section not found in next_section array for opponent %s",
            pOpponent_spec->car_spec->driver_name);
        PDEnterDebugger("BIG ERROR - GetOpponentsSectionFinishNodePoint()");
        return NULL;
    }
}

// IDA: br_scalar __usercall GetOpponentsSectionWidth@<ST0>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pSection@<EDX>)
// FUNCTION: CARM95 0x0040bccd
br_scalar GetOpponentsSectionWidth(tOpponent_spec* pOpponent_spec, tS16 pSection) {

    if (pSection >= 20000 && pSection - 20000 < pOpponent_spec->nnext_sections) {
        return gProgram_state.AI_vehicles.path_sections[pOpponent_spec->next_sections[pSection - 20000].section_no].width;
    }
    if (pSection >= 15000) {
        return 0.5f;
    }
    if (pSection == 10000) {
        return pOpponent_spec->pursue_car_data.direct_line_section.width;
    }
    return gProgram_state.AI_vehicles.path_sections[pSection].width;
}

// IDA: int __usercall GetOpponentsSectionMinSpeed@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pSection@<EDX>, int pTowards_finish@<EBX>)
// FUNCTION: CARM95 0x0040bd7a
int GetOpponentsSectionMinSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish) {
    tS16 section_no;
    int direction;

    if (pSection >= 20000 && pSection - 20000 < pOpponent_spec->nnext_sections) {
        section_no = pOpponent_spec->next_sections[pSection - 20000].section_no;
        direction = pOpponent_spec->next_sections[pSection - 20000].direction;
        return gProgram_state.AI_vehicles.path_sections[section_no].min_speed[pTowards_finish == direction];
    }
    if (pSection >= 15000) {
        return 0;
    }
    if (pSection == 10000) {
        return pOpponent_spec->pursue_car_data.direct_line_section.min_speed[pTowards_finish];
    }
    dr_dprintf("WARNING - GetOpponentsSectionMinSpeed() - section not found in next_section array for opponent %s", pOpponent_spec->car_spec->driver_name);
    PDEnterDebugger("WARNING - GetOpponentsSectionMinSpeed()");
    return 0;
}

// IDA: int __usercall GetOpponentsSectionMaxSpeed@<EAX>(tOpponent_spec *pOpponent_spec@<EAX>, tS16 pSection@<EDX>, int pTowards_finish@<EBX>)
// FUNCTION: CARM95 0x0040be6e
int GetOpponentsSectionMaxSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish) {
    tS16 section_no;
    int direction;

    if (pSection >= 20000 && pSection - 20000 < pOpponent_spec->nnext_sections) {
        section_no = pOpponent_spec->next_sections[pSection - 20000].section_no;
        direction = pOpponent_spec->next_sections[pSection - 20000].direction;
        return gProgram_state.AI_vehicles.path_sections[section_no].max_speed[pTowards_finish == direction];
    }
    if (pSection >= 15000) {
        return 255;
    }
    if (pSection == 10000) {
        return pOpponent_spec->pursue_car_data.direct_line_section.max_speed[pTowards_finish];
    }
    dr_dprintf("WARNING - GetOpponentsSectionMaxSpeed() - section not found in next_section array for opponent %s", pOpponent_spec->car_spec->driver_name);
    PDEnterDebugger("WARNING - GetOpponentsSectionMaxSpeed()");
    return 255;
}

// IDA: void __usercall InitOpponentPsyche(int pOpponent_index@<EAX>)
// FUNCTION: CARM95 0x0040bf68
void InitOpponentPsyche(int pOpponent_index) {
    gOpponents[pOpponent_index].psyche.grudge_against_player = 0;
}

// IDA: void __usercall ClearTwattageOccurrenceVariables(tOpponent_spec *pOpponent_spec@<EAX>)
// FUNCTION: CARM95 0x0040bf8d
void ClearTwattageOccurrenceVariables(tOpponent_spec* pOpponent_spec) {

    pOpponent_spec->car_spec->big_bang = 0;
    pOpponent_spec->car_spec->scary_bang = 0;
    pOpponent_spec->car_spec->grudge_raised_recently = 0;
    pOpponent_spec->car_spec->last_person_to_hit_us = NULL;
    pOpponent_spec->car_spec->last_person_we_hit = NULL;
}

// IDA: void __usercall TwoCarsHitEachOther(tCar_spec *pA_car@<EAX>, tCar_spec *pAnother_car@<EDX>)
// FUNCTION: CARM95 0x0040bfdf
void TwoCarsHitEachOther(tCar_spec* pA_car, tCar_spec* pAnother_car) {

    if (pA_car->driver == eDriver_local_human) {
        pAnother_car->last_time_we_touched_a_player = gTime_stamp_for_this_munging;
    }
    if (pAnother_car->driver == eDriver_local_human) {
        pA_car->last_time_we_touched_a_player = gTime_stamp_for_this_munging;
    }
}

// IDA: void __usercall RecordOpponentTwattageOccurrence(tCar_spec *pTwatter@<EAX>, tCar_spec *pTwattee@<EDX>)
// FUNCTION: CARM95 0x0040c020
void RecordOpponentTwattageOccurrence(tCar_spec* pTwatter, tCar_spec* pTwattee) {
    int bangness;
    int twatter_index;
    int twattee_index;
    int grudginess_caused_by_damage;
    int new_grudge_value;
    float damage;
    char str[256];
    tOpponent_spec* twattee_opponent_spec;
    tOpponent_spec* twatter_opponent_spec;

    if (pTwatter->driver != eDriver_oppo && pTwattee->driver != eDriver_oppo) {
        return;
    }
    damage = pTwattee->damage_magnitude_accumulator;
    bangness = MIN(sqrt(damage * 300000.0f), 100);
    grudginess_caused_by_damage = bangness / 10 + 50 * CAR_SPEC_IS_ROZZER(pTwattee);
    dr_dprintf("Frame %0.6d: %s hit %s, damage %f, bangness %d, gBig_bang %d, grudginess %d",
        gAcme_frame_count,
        pTwatter->driver_name,
        pTwattee->driver_name,
        damage,
        bangness,
        gBig_bang,
        grudginess_caused_by_damage);
    if (gMin_bangness <= bangness) {
        if (gMax_bangness < bangness) {
            gMax_bangness = bangness;
            dr_dprintf("(New gMax_bangness - %d)", bangness);
        }
    } else {
        gMin_bangness = bangness;
        dr_dprintf("(New gMin_bangness - %d)", bangness);
    }
    if (bangness >= 5) {
        pTwatter->last_collision_time = gTime_stamp_for_this_munging;
        pTwatter->last_person_we_hit = pTwattee;
        pTwattee->last_collision_time = gTime_stamp_for_this_munging;
        pTwattee->last_person_to_hit_us = pTwatter;
        pTwattee->grudge_raised_recently = 1;
        if (bangness >= gBig_bang || CAR_SPEC_IS_ROZZER(pTwattee)) {
            pTwattee->big_bang = 1;
        }
        if (bangness >= 80) {
            pTwattee->scary_bang = 1;
        }
        if (pTwatter->driver == eDriver_local_human) {
            twattee_opponent_spec = GetOpponentSpecFromCarSpec(pTwattee);
            if (pTwattee->scary_bang) {
                StunTheBugger(twattee_opponent_spec, 30 * bangness + 1000);
            }
            new_grudge_value = grudginess_caused_by_damage + gOpponents[twattee_opponent_spec->index].psyche.grudge_against_player;
            if (new_grudge_value > 100) {
                new_grudge_value = 100;
            }
            gOpponents[twattee_opponent_spec->index].psyche.grudge_against_player = new_grudge_value;
        } else if (pTwattee->driver == eDriver_local_human) {
            twatter_opponent_spec = GetOpponentSpecFromCarSpec(pTwatter);
            if (twatter_opponent_spec->current_objective == eOOT_pursue_and_twat && twatter_opponent_spec->pursue_car_data.pursuee == pTwattee) {
                twatter_opponent_spec->pursue_car_data.time_last_twatted_em = gTime_stamp_for_this_munging;
            }
            twatter_index = twatter_opponent_spec->index;
            new_grudge_value = gOpponents[twatter_index].psyche.grudge_against_player - (twatter_opponent_spec->current_objective == eOOT_pursue_and_twat ? 0 : 2 * grudginess_caused_by_damage);
            if (new_grudge_value < 0) {
                new_grudge_value = 0;
            }
            gOpponents[twatter_index].psyche.grudge_against_player = new_grudge_value;
        } else {
            twatter_opponent_spec = GetOpponentSpecFromCarSpec(pTwatter);
            twattee_opponent_spec = GetOpponentSpecFromCarSpec(pTwattee);
            if (pTwattee->scary_bang) {
                StunTheBugger(twattee_opponent_spec, 30 * bangness + 1000);
            }
            twattee_index = twattee_opponent_spec->index;
            if (twatter_opponent_spec->current_objective == eOOT_pursue_and_twat && twatter_opponent_spec->pursue_car_data.pursuee == pTwattee) {
                twatter_opponent_spec->pursue_car_data.time_last_twatted_em = gTime_stamp_for_this_munging;
            }
            if (CAR_SPEC_IS_OPPONENT(pTwatter) && CAR_SPEC_IS_ROZZER(pTwattee)) {
                new_grudge_value = grudginess_caused_by_damage + gOpponents[twattee_index].psyche.grudge_against_player;
                if (new_grudge_value > 100) {
                    new_grudge_value = 100;
                }
                gOpponents[twattee_index].psyche.grudge_against_player = new_grudge_value;
            }
        }
    }
}

// IDA: void __cdecl ToggleOpponentTest()
// FUNCTION: CARM95 0x0040c537
void ToggleOpponentTest(void) {

    gTest_toggle = !gTest_toggle;
}

// IDA: void __cdecl ToggleOpponentProcessing()
// FUNCTION: CARM95 0x0040c568
void ToggleOpponentProcessing(void) {
    int i;

    gProcessing_opponents = !gProcessing_opponents;
    if (gProcessing_opponents) {
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
            ObjectiveComplete(&gProgram_state.AI_vehicles.opponents[i]);
        }
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            ObjectiveComplete(&gProgram_state.AI_vehicles.cops[i]);
        }
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "OPPONENTS SWITCHED ON");
    } else {
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
            gProgram_state.AI_vehicles.opponents[i].physics_me = 0;
        }
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            gProgram_state.AI_vehicles.cops[i].physics_me = 0;
        }
        gActive_car_list_rebuild_required = 1;
        RebuildActiveCarList();
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "OPPONENTS SWITCHED OFF");
    }
}

// IDA: void __cdecl ToggleMellowOpponents()
// FUNCTION: CARM95 0x0040c6dd
void ToggleMellowOpponents(void) {
    int i;

    gMellow_opponents = !gMellow_opponents;
    if (gMellow_opponents) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -1, "Opponents all nice and fluffy");
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
            ObjectiveComplete(&gProgram_state.AI_vehicles.opponents[i]);
        }
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -1, "Opponents hostile again");
    }
}

// IDA: void __cdecl RepairOpponentsSystems()
// FUNCTION: CARM95 0x0040c795
void RepairOpponentsSystems(void) {
    int i;

    for (i = 0; i < gProgram_state.AI_vehicles.number_of_opponents; i++) {
        if (!gProgram_state.AI_vehicles.opponents[i].pursue_from_start) {
            TotallyRepairACar(gProgram_state.AI_vehicles.opponents[i].car_spec);
            TurnOpponentPhysicsOff(&gProgram_state.AI_vehicles.opponents[i]);
            gProgram_state.AI_vehicles.opponents[i].knackeredness_detected = 0;
        }
    }
    NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -1, "Opponents systems repaired (but not bodywork)");
}

// IDA: void __usercall CopyVertex(br_vertex *pDest_vertex@<EAX>, br_vertex *pSrc_vertex@<EDX>)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x0040e205
void CopyVertex__opponent(br_vertex* pDest_vertex, br_vertex* pSrc_vertex) {

    BrVector3Copy(&pDest_vertex->p, &pSrc_vertex->p);
    pDest_vertex->map.v[0] = pSrc_vertex->map.v[0];
    pDest_vertex->map.v[1] = pSrc_vertex->map.v[1];
    pDest_vertex->index = pSrc_vertex->index;
    pDest_vertex->red = pSrc_vertex->red;
    pDest_vertex->grn = pSrc_vertex->grn;
    pDest_vertex->blu = pSrc_vertex->blu;
}

// IDA: void __usercall CopyFace(br_face *pDest_face@<EAX>, br_face *pSrc_face@<EDX>)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x0040e27a
void CopyFace__opponent(br_face* pDest_face, br_face* pSrc_face) {

    pDest_face->vertices[0] = pSrc_face->vertices[0];
    pDest_face->vertices[1] = pSrc_face->vertices[1];
    pDest_face->vertices[2] = pSrc_face->vertices[2];
    pDest_face->material = pSrc_face->material;
    pDest_face->smoothing = pSrc_face->smoothing;
    pDest_face->flags = pSrc_face->flags;
}

// IDA: void __usercall DeleteSection(tS16 pSection_to_delete@<EAX>)
// FUNCTION: CARM95 0x0040c868
void DeleteSection(tS16 pSection_to_delete) {
    tS16 section_no;
    tS16 section_no_index;
    tS16 node_no;
    tS16 node_no_index;
    tS16 found_it;

    for (node_no = 0; node_no < 2; node_no++) {
        node_no_index = gProgram_state.AI_vehicles.path_sections[pSection_to_delete].node_indices[node_no];
        if (node_no_index >= 0) {
            found_it = 0;
            for (section_no = 0; section_no < (gProgram_state.AI_vehicles.path_nodes[node_no_index].number_of_sections - 1); section_no++) {
                if (gProgram_state.AI_vehicles.path_nodes[node_no_index].sections[section_no] == pSection_to_delete) {
                    found_it = 1;
                }
                if (found_it) {
                    gProgram_state.AI_vehicles.path_nodes[node_no_index].sections[section_no] = gProgram_state.AI_vehicles.path_nodes[node_no_index].sections[section_no + 1];
                }
            }
            if (gProgram_state.AI_vehicles.path_nodes[node_no_index].number_of_sections != 0) {
                gProgram_state.AI_vehicles.path_nodes[node_no_index].number_of_sections--;
            }
        }
    }
    for (section_no = pSection_to_delete; section_no < (gProgram_state.AI_vehicles.number_of_path_sections - 1); section_no++) {
        gProgram_state.AI_vehicles.path_sections[section_no] = gProgram_state.AI_vehicles.path_sections[section_no + 1];
    }
    gProgram_state.AI_vehicles.number_of_path_sections--;
    for (node_no = 0; node_no < gProgram_state.AI_vehicles.number_of_path_nodes; node_no++) {
        for (section_no_index = 0; section_no_index < gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections; section_no_index++) {
            if (pSection_to_delete < gProgram_state.AI_vehicles.path_nodes[node_no].sections[section_no_index]) {
                gProgram_state.AI_vehicles.path_nodes[node_no].sections[section_no_index]--;
            }
        }
    }
}

// IDA: void __usercall DeleteNode(tS16 pNode_to_delete@<EAX>, int pAnd_sections@<EDX>)
// FUNCTION: CARM95 0x0040caf4
void DeleteNode(tS16 pNode_to_delete, int pAnd_sections) {
    tS16 node_no;
    tS16 section_no;
    tS16 section1;
    tS16 section2;

    dr_dprintf("Node to be deleted #%d", pNode_to_delete);
    if (pAnd_sections) {
        while (gProgram_state.AI_vehicles.path_nodes[pNode_to_delete].number_of_sections != 0) {
            DeleteSection(gProgram_state.AI_vehicles.path_nodes[pNode_to_delete].sections[0]);
        }
    } else {
        if (gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[pNode_to_delete].sections[0]].node_indices[0] == pNode_to_delete) {
            section1 = gProgram_state.AI_vehicles.path_nodes[pNode_to_delete].sections[1];
            section2 = gProgram_state.AI_vehicles.path_nodes[pNode_to_delete].sections[0];
        } else {
            section1 = gProgram_state.AI_vehicles.path_nodes[pNode_to_delete].sections[0];
            section2 = gProgram_state.AI_vehicles.path_nodes[pNode_to_delete].sections[1];
        }
        dr_dprintf("Section 1#%d(#%d,#%d), section 2#%d(#%d,#%d)", section1,
            gProgram_state.AI_vehicles.path_sections[section1].node_indices[0],
            gProgram_state.AI_vehicles.path_sections[section1].node_indices[1],
            section2,
            gProgram_state.AI_vehicles.path_sections[section2].node_indices[0],
            gProgram_state.AI_vehicles.path_sections[section2].node_indices[1]);
        gProgram_state.AI_vehicles.path_sections[section1].min_speed[1] = gProgram_state.AI_vehicles.path_sections[section2].min_speed[1];
        gProgram_state.AI_vehicles.path_sections[section1].max_speed[1] = gProgram_state.AI_vehicles.path_sections[section2].max_speed[1];
        node_no = gProgram_state.AI_vehicles.path_sections[section2].node_indices[1];
        gProgram_state.AI_vehicles.path_sections[section1].node_indices[1] = node_no;
        dr_dprintf("Section 1's new end node is #%d", node_no);
        if (gProgram_state.AI_vehicles.path_nodes[node_no].sections[0] == section2) {
            gProgram_state.AI_vehicles.path_nodes[node_no].sections[0] = section1;
        } else {
            gProgram_state.AI_vehicles.path_nodes[node_no].sections[1] = section1;
        }
        gProgram_state.AI_vehicles.path_nodes[pNode_to_delete].number_of_sections = 0;
        gProgram_state.AI_vehicles.path_sections[section2].node_indices[0] = -1;
        gProgram_state.AI_vehicles.path_sections[section2].node_indices[1] = -1;
        DeleteSection(section2);
    }
    for (node_no = pNode_to_delete; node_no < (gProgram_state.AI_vehicles.number_of_path_nodes - 1); node_no++) {
        gProgram_state.AI_vehicles.path_nodes[node_no] = gProgram_state.AI_vehicles.path_nodes[node_no + 1];
    }
    for (section_no = 0; section_no < gProgram_state.AI_vehicles.number_of_path_sections; section_no++) {
        if (pNode_to_delete < gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]) {
            gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]--;
        }
        if (pNode_to_delete < gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]) {
            gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]--;
        }
    }
    gProgram_state.AI_vehicles.number_of_path_nodes--;
}

// IDA: void __cdecl DeleteOrphanNodes()
// FUNCTION: CARM95 0x0040ca84
void DeleteOrphanNodes(void) {
    tS16 node_no;

    for (node_no = 0; node_no < gProgram_state.AI_vehicles.number_of_path_nodes; node_no++) {
        while (node_no < gProgram_state.AI_vehicles.number_of_path_nodes && gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections == 0) {
            DeleteNode(node_no, 1);
        }
    }
}

// IDA: void __usercall InsertThisNodeInThisSectionHere(tS16 pInserted_node@<EAX>, tS16 pSection_no@<EDX>, br_vector3 *pWhere@<EBX>)
// FUNCTION: CARM95 0x0040fb06
void InsertThisNodeInThisSectionHere(tS16 pInserted_node, tS16 pSection_no, br_vector3* pWhere) {
    tS16 new_section;
    tS16 section_no_index;
    tS16 node1;
    tS16 node2;
    tS16 node3;

    section_no_index = gProgram_state.AI_vehicles.path_sections[pSection_no].node_indices[1];
    new_section = ReallocExtraPathSections(1);
    gProgram_state.AI_vehicles.path_sections[new_section].node_indices[0] = pInserted_node;
    gProgram_state.AI_vehicles.path_sections[new_section].node_indices[1] = section_no_index;
    gProgram_state.AI_vehicles.path_sections[new_section].min_speed[0] = 0;
    gProgram_state.AI_vehicles.path_sections[new_section].max_speed[0] = 255;
    gProgram_state.AI_vehicles.path_sections[new_section].min_speed[1] = gProgram_state.AI_vehicles.path_sections[pSection_no].min_speed[1];
    gProgram_state.AI_vehicles.path_sections[new_section].max_speed[1] = gProgram_state.AI_vehicles.path_sections[pSection_no].max_speed[1];
    gProgram_state.AI_vehicles.path_sections[new_section].width = gProgram_state.AI_vehicles.path_sections[pSection_no].width;
    gProgram_state.AI_vehicles.path_sections[new_section].type = gProgram_state.AI_vehicles.path_sections[pSection_no].type;
    gProgram_state.AI_vehicles.path_sections[new_section].one_way = gProgram_state.AI_vehicles.path_sections[pSection_no].one_way;
    gProgram_state.AI_vehicles.path_sections[pSection_no].node_indices[1] = pInserted_node;
    gProgram_state.AI_vehicles.path_sections[pSection_no].min_speed[1] = 0;
    gProgram_state.AI_vehicles.path_sections[pSection_no].max_speed[1] = 255;
    BrVector3Copy(&gProgram_state.AI_vehicles.path_nodes[pInserted_node].p, pWhere);
    gProgram_state.AI_vehicles.path_nodes[pInserted_node].sections
        [gProgram_state.AI_vehicles.path_nodes[pInserted_node].number_of_sections]
        = pSection_no;
    gProgram_state.AI_vehicles.path_nodes[pInserted_node].number_of_sections = gProgram_state.AI_vehicles.path_nodes[pInserted_node].number_of_sections + 1;
    gProgram_state.AI_vehicles.path_nodes[pInserted_node].sections
        [gProgram_state.AI_vehicles.path_nodes[pInserted_node].number_of_sections]
        = new_section;
    gProgram_state.AI_vehicles.path_nodes[pInserted_node].number_of_sections = gProgram_state.AI_vehicles.path_nodes[pInserted_node].number_of_sections + 1;
    for (node1 = 0; node1 < gProgram_state.AI_vehicles.path_nodes[section_no_index].number_of_sections; node1++) {
        if (gProgram_state.AI_vehicles.path_nodes[section_no_index].sections[node1] == pSection_no) {
            gProgram_state.AI_vehicles.path_nodes[section_no_index].sections[node1] = new_section;
        }
    }
}

// IDA: void __cdecl TrackElasticateyPath()
// FUNCTION: CARM95 0x0040ce66
void TrackElasticateyPath(void) {

    if (gAlready_elasticating && gNext_elastication < gTime_stamp_for_this_munging) {
        gNext_elastication = gTime_stamp_for_this_munging + 2000;
        BrVector3Copy(&gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1]].p, &gSelf->t.t.translate.t);
        RebuildOppoPathModel();
        if (gNext_write_during_elastication < gTime_stamp_for_this_munging) {
            gNext_write_during_elastication = gTime_stamp_for_this_munging + 10000;
            WriteOutOppoPaths();
        }
    }
}

// IDA: void __usercall RecalcNearestPathSectionSpeed(int pMax_not_min@<EAX>, int pAdjustment@<EDX>)
// FUNCTION: CARM95 0x004105c6
void RecalcNearestPathSectionSpeed(int pMax_not_min, int pAdjustment) {
    tS16 section_no;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_vector3 wank;
    br_scalar distance;
    br_scalar dist_to_start;
    br_scalar dist_to_finish;
    char str[128];
    int new_speed;
    int nearest_end;

    if (gOppo_paths_shown) {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        if (!gAlready_elasticating && distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any paths close enough");
        } else {
            BrVector3Sub(&wank, &gSelf->t.t.translate.t, &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].p);
            dist_to_start = BrVector3Length(&wank);
            BrVector3Sub(&wank, &gSelf->t.t.translate.t, &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].p);
            dist_to_finish = BrVector3Length(&wank);
            nearest_end = dist_to_finish < dist_to_start ? 1 : 0;
            if (pMax_not_min) {
                new_speed = gProgram_state.AI_vehicles.path_sections[section_no].max_speed[nearest_end];
            } else {
                new_speed = gProgram_state.AI_vehicles.path_sections[section_no].min_speed[nearest_end];
            }
            new_speed += 5 * pAdjustment;
            if (5 * pAdjustment < 0 && new_speed > 100) {
                new_speed = 100;
            } else if (5 * pAdjustment > 0 && new_speed > 100) {
                new_speed = 255;
            }
            if (new_speed < 0) {
                new_speed = 0;
            } else if (new_speed > 255) {
                new_speed = 255;
            }
            if (pMax_not_min) {
                gProgram_state.AI_vehicles.path_sections[section_no].max_speed[nearest_end] = new_speed;
            } else {
                gProgram_state.AI_vehicles.path_sections[section_no].min_speed[nearest_end] = new_speed;
            }
            if (nearest_end != 0) {
                sprintf(str, "Towards section finish - Min Speed %d mph - Max speed %d mph",
                    (int)(2.2f * gProgram_state.AI_vehicles.path_sections[section_no].min_speed[nearest_end]),
                    (int)(2.2f * gProgram_state.AI_vehicles.path_sections[section_no].max_speed[nearest_end]));
            } else {
                sprintf(str, "Towards section start - Min Speed %d mph - Max speed %d mph",
                    (int)(2.2f * gProgram_state.AI_vehicles.path_sections[section_no].min_speed[0]),
                    (int)(2.2f * gProgram_state.AI_vehicles.path_sections[section_no].max_speed[0]));
            }
            ShowOppoPaths();
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl RecalcNearestPathSectionWidth(br_scalar pAdjustment)
// FUNCTION: CARM95 0x004103ef
void RecalcNearestPathSectionWidth(br_scalar pAdjustment) {
    tS16 section_no;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_scalar distance;
    char str[128];

    if (gOppo_paths_shown) {
        if (!gAlready_elasticating) {
            section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
            if (distance > 10.f) {
                NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any paths close enough");
                return;
            }
        } else {
            section_no = gMobile_section;
        }
        gProgram_state.AI_vehicles.path_sections[section_no].width += (int)pAdjustment * pAdjustment + pAdjustment;
        if (gProgram_state.AI_vehicles.path_sections[section_no].width < .05f) {
            gProgram_state.AI_vehicles.path_sections[section_no].width = .05f;
        }
        ShowOppoPaths();
        sprintf(str, "Width %2.1f BRU", 2.f * gProgram_state.AI_vehicles.path_sections[section_no].width);
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
    }
}

// IDA: void __usercall CalcNegativeXVector(br_vector3 *pNegative_x_vector@<EAX>, br_vector3 *pStart@<EDX>, br_vector3 *pFinish@<EBX>, br_scalar pLength)
// FUNCTION: CARM95 0x0040cf4e
void CalcNegativeXVector(br_vector3* pNegative_x_vector, br_vector3* pStart, br_vector3* pFinish, br_scalar pLength) {
    br_vector3 positive_y_vector;
    br_vector3 path_vector;

    positive_y_vector.v[0] = pFinish->v[0] - pStart->v[0];
    positive_y_vector.v[1] = pFinish->v[1] - pStart->v[1];
    positive_y_vector.v[2] = pFinish->v[2] - pStart->v[2];
    pNegative_x_vector->v[0] = 1.0 * positive_y_vector.v[2] - positive_y_vector.v[1] * 0.0;
    pNegative_x_vector->v[1] = 0.0 * positive_y_vector.v[0] - positive_y_vector.v[2] * 0.0;
    pNegative_x_vector->v[2] = positive_y_vector.v[1] * 0.0 - 1.0 * positive_y_vector.v[0];

    BrVector3Normalise(pNegative_x_vector, pNegative_x_vector);
    BrVector3Scale(pNegative_x_vector, pNegative_x_vector, pLength);
}

// IDA: void __usercall MakeVertexAndOffsetIt(br_model *pModel@<EAX>, int pVertex_num@<EDX>, br_scalar pX, br_scalar pY, br_scalar pZ, br_vector3 *pOffset)
// FUNCTION: CARM95 0x0040d9d3
void MakeVertexAndOffsetIt(br_model* pModel, int pVertex_num, br_scalar pX, br_scalar pY, br_scalar pZ, br_vector3* pOffset) {

    BrVector3Set(&pModel->vertices[pVertex_num].p, pX, pY, pZ);
    BrVector3Accumulate(&pModel->vertices[pVertex_num].p, pOffset);
}

// IDA: void __usercall MakeFaceAndTextureIt(br_model *pModel@<EAX>, int pFace_num@<EDX>, int pV0@<EBX>, int pV1@<ECX>, int pV2, br_material *pMaterial)
// FUNCTION: CARM95 0x0040da85
void MakeFaceAndTextureIt(br_model* pModel, int pFace_num, int pV0, int pV1, int pV2, br_material* pMaterial) {

    pModel->faces[pFace_num].vertices[0] = pV0;
    pModel->faces[pFace_num].vertices[1] = pV1;
    pModel->faces[pFace_num].vertices[2] = pV2;
    pModel->faces[pFace_num].smoothing = -1;
    pModel->faces[pFace_num].material = pMaterial;
}

// IDA: void __usercall MakeSection(br_uint_16 pFirst_vertex@<EAX>, br_uint_16 pFirst_face@<EDX>, br_vector3 *pStart@<EBX>, br_vector3 *pFinish@<ECX>, br_scalar pWidth, br_material *pMaterial_centre_lt, br_material *pMaterial_centre_dk, br_material *pMaterial_edges_start_lt, br_material *pMaterial_edges_start_dk, br_material *pMaterial_edges_finish_lt, br_material *pMaterial_edges_finish_dk)
// FUNCTION: CARM95 0x0040d59f
void MakeSection(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pStart, br_vector3* pFinish, br_scalar pWidth, br_material* pMaterial_centre_lt, br_material* pMaterial_centre_dk, br_material* pMaterial_edges_start_lt, br_material* pMaterial_edges_start_dk, br_material* pMaterial_edges_finish_lt, br_material* pMaterial_edges_finish_dk) {
    int i;
    br_vector3 offset_v;
    br_vector3 centre_length_v;
    br_material* the_material_start_lt;
    br_material* the_material_start_dk;
    br_material* the_material_finish_lt;
    br_material* the_material_finish_dk;
    br_scalar height;

    CalcNegativeXVector(&offset_v, pStart, pFinish, pWidth);
    for (i = 0; i < 3; i++) {
        the_material_start_lt = pMaterial_edges_start_lt;
        the_material_start_dk = pMaterial_edges_start_dk;
        the_material_finish_lt = pMaterial_edges_finish_lt;
        the_material_finish_dk = pMaterial_edges_finish_dk;
        height = .15f;
        if (i == 1) {
            BrVector3Negate(&offset_v, &offset_v);
        } else if (i == 2) {
            height = .3f;
            BrVector3Set(&offset_v, 0.f, 0.f, 0.f);
            the_material_finish_lt = pMaterial_centre_lt;
            the_material_start_lt = pMaterial_centre_lt;
            the_material_finish_dk = pMaterial_centre_dk;
            the_material_start_dk = pMaterial_centre_dk;
        }
        centre_length_v.v[0] = pStart->v[0] + (pFinish->v[0] - pStart->v[0]) / 2.f;
        centre_length_v.v[1] = pStart->v[1] + (pFinish->v[1] - pStart->v[1]) / 2.f;
        centre_length_v.v[2] = pStart->v[2] + (pFinish->v[2] - pStart->v[2]) / 2.f;

        MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 6 * i + 0, pStart->v[0], pStart->v[1], pStart->v[2], &offset_v);
        MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 6 * i + 1, pStart->v[0], pStart->v[1] + height, pStart->v[2], &offset_v);
        MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 6 * i + 2, centre_length_v.v[0], centre_length_v.v[1] + height, centre_length_v.v[2], &offset_v);
        MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 6 * i + 3, centre_length_v.v[0], centre_length_v.v[1], centre_length_v.v[2], &offset_v);
        MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 6 * i + 4, pFinish->v[0], pFinish->v[1] + height, pFinish->v[2], &offset_v);
        MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 6 * i + 5, pFinish->v[0], pFinish->v[1], pFinish->v[2], &offset_v);

        MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 4 * i + 0, pFirst_vertex + 6 * i + 0, pFirst_vertex + 6 * i + 1, pFirst_vertex + 6 * i + 2, the_material_start_lt);
        MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 4 * i + 1, pFirst_vertex + 6 * i + 0, pFirst_vertex + 6 * i + 2, pFirst_vertex + 6 * i + 3, the_material_start_dk);
        MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 4 * i + 2, pFirst_vertex + 6 * i + 3, pFirst_vertex + 6 * i + 2, pFirst_vertex + 6 * i + 4, the_material_finish_lt);
        MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 4 * i + 3, pFirst_vertex + 6 * i + 3, pFirst_vertex + 6 * i + 4, pFirst_vertex + 6 * i + 5, the_material_finish_dk);
    }
}

// IDA: void __usercall MakeCube(br_uint_16 pFirst_vertex@<EAX>, br_uint_16 pFirst_face@<EDX>, br_vector3 *pPoint@<EBX>, br_material *pMaterial_1@<ECX>, br_material *pMaterial_2, br_material *pMaterial_3)
// FUNCTION: CARM95 0x0040daf1
void MakeCube(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pPoint, br_material* pMaterial_1, br_material* pMaterial_2, br_material* pMaterial_3) {
    br_vector3 offset_v;
    br_vector3 point;

    point.v[0] = pPoint->v[0];
    point.v[1] = pPoint->v[1];
    point.v[1] += .15f;
    point.v[2] = pPoint->v[2];

    BrVector3Set(&offset_v, .1f, .1f, .1f);
    MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 0, point.v[0], point.v[1], point.v[2], &offset_v);
    BrVector3Set(&offset_v, .1f, -.1f, .1f);
    MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 1, point.v[0], point.v[1], point.v[2], &offset_v);
    BrVector3Set(&offset_v, -.1f, -.1f, .1f);
    MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 2, point.v[0], point.v[1], point.v[2], &offset_v);
    BrVector3Set(&offset_v, -.1f, .1f, .1f);
    MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 3, point.v[0], point.v[1], point.v[2], &offset_v);
    BrVector3Set(&offset_v, .1f, .1f, -.1f);
    MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 4, point.v[0], point.v[1], point.v[2], &offset_v);
    BrVector3Set(&offset_v, .1f, -.1f, -.1f);
    MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 5, point.v[0], point.v[1], point.v[2], &offset_v);
    BrVector3Set(&offset_v, -.1f, -.1f, -.1f);
    MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 6, point.v[0], point.v[1], point.v[2], &offset_v);
    BrVector3Set(&offset_v, -.1f, .1f, -.1f);
    MakeVertexAndOffsetIt(gOppo_path_model, pFirst_vertex + 7, point.v[0], point.v[1], point.v[2], &offset_v);

    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 0, pFirst_vertex + 3, pFirst_vertex + 2, pFirst_vertex + 1, pMaterial_1);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 1, pFirst_vertex + 0, pFirst_vertex + 3, pFirst_vertex + 1, pMaterial_1);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 2, pFirst_vertex + 1, pFirst_vertex + 5, pFirst_vertex + 4, pMaterial_2);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 3, pFirst_vertex + 1, pFirst_vertex + 4, pFirst_vertex + 0, pMaterial_2);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 4, pFirst_vertex + 0, pFirst_vertex + 4, pFirst_vertex + 3, pMaterial_3);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 5, pFirst_vertex + 3, pFirst_vertex + 4, pFirst_vertex + 7, pMaterial_3);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 6, pFirst_vertex + 4, pFirst_vertex + 5, pFirst_vertex + 7, pMaterial_1);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 7, pFirst_vertex + 5, pFirst_vertex + 6, pFirst_vertex + 7, pMaterial_1);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 8, pFirst_vertex + 2, pFirst_vertex + 7, pFirst_vertex + 6, pMaterial_2);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 9, pFirst_vertex + 2, pFirst_vertex + 3, pFirst_vertex + 7, pMaterial_2);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 10, pFirst_vertex + 1, pFirst_vertex + 2, pFirst_vertex + 6, pMaterial_3);
    MakeFaceAndTextureIt(gOppo_path_model, pFirst_face + 11, pFirst_vertex + 1, pFirst_vertex + 6, pFirst_vertex + 5, pMaterial_3);
}

// IDA: void __usercall CalcNumberOfFacesAndVerticesForOppoPathModel(br_uint_16 *pFace_index_ptr@<EAX>, br_uint_16 *pVertex_index_ptr@<EDX>)
// FUNCTION: CARM95 0x0040e016
void CalcNumberOfFacesAndVerticesForOppoPathModel(br_uint_16* pFace_index_ptr, br_uint_16* pVertex_index_ptr) {

    *pFace_index_ptr = gProgram_state.AI_vehicles.number_of_path_sections * 12 + gProgram_state.AI_vehicles.number_of_cops * 12;
    *pVertex_index_ptr = gProgram_state.AI_vehicles.number_of_path_sections * 18 + gProgram_state.AI_vehicles.number_of_cops * 8;
}

// IDA: void __usercall ReallocModelFacesAndVertices(br_model *pModel@<EAX>, int pNum_faces@<EDX>, int pNum_vertices@<EBX>)
// FUNCTION: CARM95 0x0040e057
void ReallocModelFacesAndVertices(br_model* pModel, int pNum_faces, int pNum_vertices) {
    br_vertex* new_vertices;
    br_face* new_faces;
    int i;

    new_vertices = BrResAllocate(pModel, pNum_vertices * sizeof(br_vertex), BR_MEMORY_VERTICES);
    memset(new_vertices, 0, pNum_vertices * sizeof(br_vertex));
    if (pModel->nvertices != 0) {
        for (i = 0; i < ((pNum_vertices <= pModel->nvertices) ? pNum_vertices : pModel->nvertices); i++) {
            CopyVertex__opponent(&new_vertices[i], &pModel->vertices[i]);
        }
        BrResRemove(pModel->vertices);
        BrResFree(pModel->vertices);
    }
    pModel->vertices = new_vertices;
    pModel->nvertices = pNum_vertices;

    new_faces = BrResAllocate(pModel, pNum_faces * sizeof(br_face), BR_MEMORY_FACES);
    memset(new_faces, 0, pNum_faces * sizeof(br_face));
    if (pModel->nfaces != 0) {
        for (i = 0; i < ((pNum_faces <= pModel->nfaces) ? pNum_faces : pModel->nfaces); i++) {
            CopyFace__opponent(&new_faces[i], &pModel->faces[i]);
        }
        BrResRemove(pModel->faces);
        BrResFree(pModel->faces);
    }
    pModel->faces = new_faces;
    pModel->nfaces = pNum_faces;
}

// IDA: br_material* __usercall CreateSimpleMaterial@<EAX>(int pColour_index@<EAX>)
// FUNCTION: CARM95 0x0040e3cf
br_material* CreateSimpleMaterial(int pColour_index) {
    br_material* return_me;

    return_me = BrMaterialAllocate(NULL);
    return_me->index_base = pColour_index;
    return_me->index_range = 1;
    return_me->flags = BR_MATF_TWO_SIDED;
    return_me->index_shade = NULL;
    return_me->colour_map = NULL;
    return_me->identifier = NULL;
    BrMaterialAdd(return_me);
    return return_me;
}

// IDA: void __cdecl AllocateMatsForOppoPathModel()
// FUNCTION: CARM95 0x0040e2d3
void AllocateMatsForOppoPathModel(void) {

    gMat_dk_yel = CreateSimpleMaterial(50);
    gMat_md_yel = CreateSimpleMaterial(51);
    gMat_lt_yel = CreateSimpleMaterial(52);
    gMat_dk_red = CreateSimpleMaterial(3);
    gMat_lt_red = CreateSimpleMaterial(4);
    gMat_dk_grn = CreateSimpleMaterial(66);
    gMat_lt_grn = CreateSimpleMaterial(68);
    gMat_dk_blu = CreateSimpleMaterial(162);
    gMat_lt_blu = CreateSimpleMaterial(164);
    gMat_dk_turq = CreateSimpleMaterial(130);
    gMat_lt_turq = CreateSimpleMaterial(132);
    gMat_dk_gry = CreateSimpleMaterial(253);
    gMat_md_gry = CreateSimpleMaterial(254);
    gMat_lt_gry = CreateSimpleMaterial(255);

    gMats_allocated = 1;
}

// IDA: void __cdecl RebuildOppoPathModel()
// FUNCTION: CARM95 0x0040d097
void RebuildOppoPathModel(void) {
    // GLOBAL: CARM95 0x50717c
    static int nvertices_last_time = 0;
    // GLOBAL: CARM95 0x507180
    static int nfaces_last_time = 0;
    int i;
    int at_least_one;
    br_uint_16 nfaces;
    br_uint_16 nvertices;
    br_uint_16 first_face;
    br_uint_16 first_vertex;
    br_material* centre_mat_lt;
    br_material* centre_mat_dk;
    br_material* edge_mat_start_lt;
    br_material* edge_mat_start_dk;
    br_material* edge_mat_finish_lt;
    br_material* edge_mat_finish_dk;

    if (gProgram_state.AI_vehicles.number_of_path_nodes < 2) {
        if (gOppo_path_model != NULL) {
            BrModelRemove(gOppo_path_model);
            BrModelFree(gOppo_path_model);
            gOppo_path_model = NULL;
        }
        if (gOppo_path_actor != NULL) {
            gOppo_path_actor->type = BR_ACTOR_NONE;
            gOppo_path_actor->render_style = BR_RSTYLE_NONE;
        }
    } else {
        if (!gMats_allocated) {
            AllocateMatsForOppoPathModel();
        }
        if (gOppo_path_actor == NULL) {
            gOppo_path_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
            BrActorAdd(gNon_track_actor, gOppo_path_actor);
        }
        if (gOppo_path_model == NULL) {
            gOppo_path_actor->model = BrModelAllocate("OppoPathModel", 3, 1);
            gOppo_path_model = gOppo_path_actor->model;
            gOppo_path_model->flags |= BR_MODF_DONT_WELD | BR_MODF_KEEP_ORIGINAL | BR_MODF_GENERATE_TAGS;
            BrModelAdd(gOppo_path_model);
        }
        gOppo_path_actor->model = gOppo_path_model;
        gOppo_path_actor->type = BR_ACTOR_MODEL;
        gOppo_path_actor->render_style = BR_RSTYLE_FACES;
        CalcNumberOfFacesAndVerticesForOppoPathModel(&nfaces, &nvertices);
        if (nvertices_last_time < nvertices || nfaces_last_time < nfaces) {
            ReallocModelFacesAndVertices(gOppo_path_model, nfaces, nvertices);
            nvertices_last_time = nvertices;
            nfaces_last_time = nfaces;
        } else {
            gOppo_path_model->nvertices = nvertices;
            gOppo_path_model->nfaces = nfaces;
        }
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_path_sections; i++) {
            centre_mat_lt = gMat_lt_grn;
            centre_mat_dk = gMat_dk_grn;
            edge_mat_start_lt = gMat_lt_grn;
            edge_mat_start_dk = gMat_dk_grn;
            edge_mat_finish_lt = gMat_lt_grn;
            edge_mat_finish_dk = gMat_dk_grn;
            if (gProgram_state.AI_vehicles.path_sections[i].type == 1) {
                centre_mat_lt = gMat_lt_red;
                centre_mat_dk = gMat_dk_red;
            } else if (gProgram_state.AI_vehicles.path_sections[i].type == 2) {
                centre_mat_lt = gMat_lt_blu;
                centre_mat_dk = gMat_dk_blu;
            }
            if (gProgram_state.AI_vehicles.path_sections[i].one_way) {
                centre_mat_lt = gMat_lt_yel;
            }
            if ((gProgram_state.AI_vehicles.path_sections[i].min_speed[0] != 0) || (gProgram_state.AI_vehicles.path_sections[i].max_speed[0] != 255)) {
                edge_mat_start_lt = gMat_lt_turq;
                edge_mat_start_dk = gMat_dk_turq;
            }
            if ((gProgram_state.AI_vehicles.path_sections[i].min_speed[1] != 0) || (gProgram_state.AI_vehicles.path_sections[i].max_speed[1] != 255)) {
                edge_mat_finish_lt = gMat_lt_turq;
                edge_mat_finish_dk = gMat_dk_turq;
            }
            if (gAlready_elasticating && gMobile_section == i) {
                BrVector3Copy(&gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[i].node_indices[1]].p,
                    &gSelf->t.t.translate.t);
            }
            MakeSection(18 * i, 12 * i,
                &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[i].node_indices[0]].p,
                &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[i].node_indices[1]].p,
                gProgram_state.AI_vehicles.path_sections[i].width,
                centre_mat_lt, centre_mat_dk,
                edge_mat_start_lt, edge_mat_start_dk,
                edge_mat_finish_lt, edge_mat_finish_dk);
        }
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            MakeCube(18 * gProgram_state.AI_vehicles.number_of_path_sections + 8 * i,
                12 * gProgram_state.AI_vehicles.number_of_path_sections + 12 * i,
                gProgram_state.AI_vehicles.cop_start_points + i,
                gMat_lt_turq,
                gMat_lt_turq,
                gMat_dk_turq);
        }
        BrModelUpdate(gOppo_path_model, BR_MODU_ALL);
    }
}

// IDA: int __cdecl ConsistencyCheck()
// FUNCTION: CARM95 0x0040e436
int ConsistencyCheck(void) {
    tS16 node_no;
    tS16 section_no;
    tS16 start_node;
    tS16 finish_node;
    tS16 section_no_index;
    tS16 section_no_index1;
    int found_how_many;
    int failed;
    tU8* nodes_referenced_by_sections_array = NULL;
    tU8* sections_referenced_by_nodes_array = NULL;

    failed = 0;
    if (gProgram_state.AI_vehicles.number_of_path_nodes != 0) {
        nodes_referenced_by_sections_array = BrMemAllocate(gProgram_state.AI_vehicles.number_of_path_nodes, kMem_nodes_array);
        memset(nodes_referenced_by_sections_array, 0, gProgram_state.AI_vehicles.number_of_path_nodes);
    }
    if (gProgram_state.AI_vehicles.number_of_path_sections != 0) {
        sections_referenced_by_nodes_array = BrMemAllocate(gProgram_state.AI_vehicles.number_of_path_sections, kMem_sections_array);
        memset(sections_referenced_by_nodes_array, 0, gProgram_state.AI_vehicles.number_of_path_sections);
    }
    for (section_no_index = 0; section_no_index < gProgram_state.AI_vehicles.number_of_path_sections; section_no_index++) {
        start_node = gProgram_state.AI_vehicles.path_sections[section_no_index].node_indices[0];
        finish_node = gProgram_state.AI_vehicles.path_sections[section_no_index].node_indices[1];
        if (finish_node == start_node) {
            dr_dprintf("CONSISTENCY FAILURE: Section #%d has both ends attached to same node!", section_no_index);
            failed = 1;
        }
        if (start_node >= 0 && gProgram_state.AI_vehicles.number_of_path_nodes - 1 >= start_node) {
            nodes_referenced_by_sections_array[start_node] = 1;
            nodes_referenced_by_sections_array[finish_node] = 1;
            found_how_many = 0;
            for (section_no_index1 = 0; section_no_index1 < gProgram_state.AI_vehicles.path_nodes[start_node].number_of_sections; section_no_index1++) {
                if (gProgram_state.AI_vehicles.path_nodes[start_node].sections[section_no_index1] == section_no_index) {
                    found_how_many++;
                }
            }
            if (found_how_many == 0) {
                dr_dprintf(
                    "CONSISTENCY FAILURE: Section #%d references node #%d but not vice-versa",
                    section_no_index,
                    start_node);
                failed = 1;
            }
        } else {
            dr_dprintf(
                "CONSISTENCY FAILURE: Section #%d references invalid node (#%d) - should be in range 0..%d",
                section_no_index,
                start_node,
                gProgram_state.AI_vehicles.number_of_path_nodes - 1);
            failed = 1;
        }
        if (finish_node >= 0 && gProgram_state.AI_vehicles.number_of_path_nodes - 1 >= finish_node) {
            found_how_many = 0;
            for (section_no_index1 = 0; section_no_index1 < gProgram_state.AI_vehicles.path_nodes[finish_node].number_of_sections; section_no_index1++) {
                if (gProgram_state.AI_vehicles.path_nodes[finish_node].sections[section_no_index1] == section_no_index) {
                    found_how_many++;
                }
            }
            if (found_how_many == 0) {
                dr_dprintf(
                    "CONSISTENCY FAILURE: Section #%d references node #%d but not vice-versa",
                    section_no_index,
                    finish_node);
                failed = 1;
            }
        } else {
            dr_dprintf(
                "CONSISTENCY FAILURE: Section #%d references invalid node (#%d) - should be in range 0..%d",
                section_no_index,
                finish_node,
                gProgram_state.AI_vehicles.number_of_path_nodes - 1);
            failed = 1;
        }
    }
    for (node_no = 0; node_no < gProgram_state.AI_vehicles.number_of_path_nodes; node_no++) {
        for (section_no_index = 0; section_no_index < gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections; section_no_index++) {
            section_no = gProgram_state.AI_vehicles.path_nodes[node_no].sections[section_no_index];
            if (section_no >= 0 && gProgram_state.AI_vehicles.number_of_path_sections - 1 >= section_no) {
                sections_referenced_by_nodes_array[section_no] = 1;
                if (gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0] != node_no
                    && gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] != node_no) {
                    dr_dprintf(
                        "CONSISTENCY FAILURE: Node #%d references section #%d but not vice-versa",
                        node_no,
                        section_no);
                    failed = 1;
                }
            } else {
                dr_dprintf(
                    "CONSISTENCY FAILURE: Node #%d references invalid section (#%d) - should be in range 0..%d",
                    node_no,
                    section_no,
                    gProgram_state.AI_vehicles.number_of_path_sections - 1);
                failed = 1;
            }
            found_how_many = 0;
            for (section_no_index1 = section_no; section_no_index1 < gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections; section_no_index1++) {
                if (gProgram_state.AI_vehicles.path_nodes[node_no].sections[section_no_index1] == section_no) {
                    found_how_many++;
                }
            }
            if (found_how_many > 1) {
                dr_dprintf(
                    "CONSISTENCY FAILURE: Node #%d references section #%d multiple times",
                    node_no,
                    section_no);
                failed = 1;
            }
        }
    }
    for (section_no = 0; section_no < gProgram_state.AI_vehicles.number_of_path_sections; section_no++) {
        if (!sections_referenced_by_nodes_array[section_no]) {
            dr_dprintf("CONSISTENCY FAILURE: Section #%d not referenced by any nodes", section_no);
            failed = 1;
        }
    }
    for (node_no = 0; node_no < gProgram_state.AI_vehicles.number_of_path_nodes; node_no++) {
        if (!nodes_referenced_by_sections_array[node_no]) {
            dr_dprintf("CONSISTENCY FAILURE: Node #%d not referenced by any sections", node_no);
            failed = 1;
        }
    }
    if (gProgram_state.AI_vehicles.number_of_path_nodes != 0) {
        BrMemFree(nodes_referenced_by_sections_array);
    }
    if (gProgram_state.AI_vehicles.number_of_path_sections != 0) {
        BrMemFree(sections_referenced_by_nodes_array);
    }
    if (failed) {
        dr_dprintf(
            "CONSISTENCY FAILURE INFORMATION: Allegedly %d sections and %d nodes",
            gProgram_state.AI_vehicles.number_of_path_sections,
            gProgram_state.AI_vehicles.number_of_path_nodes);
        dr_dprintf("^^^ CONSISTENCY FAILURE ^^^");
        PDEnterDebugger("OPPONENT PATH CONSISTENCY FAILURE - refer to DIAGNOST.TXT");
    }
    return !failed;
}

// IDA: void __cdecl ShowOppoPaths()
// FUNCTION: CARM95 0x0040ed8e
void ShowOppoPaths(void) {
    char str[256];

    if (!gOppo_paths_shown) {
        if (gOppo_path_actor != NULL) {
            gOppo_path_actor->render_style = BR_RSTYLE_NONE;
        }
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -1, "Not displaying any paths");
    } else {
        RebuildOppoPathModel();
        sprintf(str, "Total %d nodes, %d sections",
            gProgram_state.AI_vehicles.number_of_path_nodes,
            gProgram_state.AI_vehicles.number_of_path_sections);
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -1, str);
    }
    if (ConsistencyCheck()) {
        WriteOutOppoPaths();
    }
}

#include <errno.h>
#include <string.h>
// IDA: void __cdecl WriteOutOppoPaths()
// FUNCTION: CARM95 0x0040e9d4
void WriteOutOppoPaths(void) {
    char the_path[256];
    char str[13];
    FILE* f;
    int i;

    if (!gMade_path_filename) {
        for (i = 0; 1; i++) {
#ifdef DETHRACE_FIX_BUGS
            sprintf(str, "OPATH%03d.TXT", i);
#else
            sprintf(str, "OPATH%0.3d.TXT", i);
#endif
            PathCat(the_path, gApplication_path, str);
#ifdef DETHRACE_FIX_BUGS
            // OldDRfopen refuses to open unknown .TXT files
            f = fopen(the_path, "r");
#else
            f = DRfopen(the_path, "r+");
#endif
            if (f == NULL) {
                break;
            }
            fclose(f);
        }
        strcpy(gOppo_path_filename, the_path);
        gMade_path_filename = 1;
    }
#ifdef DETHRACE_FIX_BUGS
    f = fopen(gOppo_path_filename, "w");
#else
    f = DRfopen(gOppo_path_filename, "wt");
#endif
    if (f == NULL) {
        printf("f is NULL, errno=%d, msg=\"%s\"\n", errno, strerror(errno));
    }
    fprintf(f, "%s\n", "START OF OPPONENT PATHS");
    fprintf(f, "\n%-3d                             // Number of path nodes\n",
        gProgram_state.AI_vehicles.number_of_path_nodes);
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_path_nodes; i++) {
        fprintf(f, "%9.3f,%9.3f,%9.3f   // Node #%d\n",
            gProgram_state.AI_vehicles.path_nodes[i].p.v[0],
            gProgram_state.AI_vehicles.path_nodes[i].p.v[1],
            gProgram_state.AI_vehicles.path_nodes[i].p.v[2],
            i);
    }
    fprintf(f, "\n%-3d                                           // Number of path sections\n",
        gProgram_state.AI_vehicles.number_of_path_sections);
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_path_sections; i++) {
        fprintf(f, "%4d,%4d,%4d,%4d,%4d,%4d,%7.1f,%5d   // Section #%d\n",
            gProgram_state.AI_vehicles.path_sections[i].node_indices[0],
            gProgram_state.AI_vehicles.path_sections[i].node_indices[1],
            gProgram_state.AI_vehicles.path_sections[i].min_speed[0],
            gProgram_state.AI_vehicles.path_sections[i].max_speed[0],
            gProgram_state.AI_vehicles.path_sections[i].min_speed[1],
            gProgram_state.AI_vehicles.path_sections[i].max_speed[1],
            gProgram_state.AI_vehicles.path_sections[i].width,
            gProgram_state.AI_vehicles.path_sections[i].one_way ? (gProgram_state.AI_vehicles.path_sections[i].type + 1000) : gProgram_state.AI_vehicles.path_sections[i].type,
            i);
    }
    fprintf(f, "\n%-2d                                                            // Number of cop start points\n",
        gProgram_state.AI_vehicles.number_of_cops);
    for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
        fprintf(f, "%9.3f,%9.3f,%9.3f,%9.3f,%9.3f,%9.3f   // Cop start point #%d\n",
            gProgram_state.AI_vehicles.cop_start_points[i].v[0],
            gProgram_state.AI_vehicles.cop_start_points[i].v[1],
            gProgram_state.AI_vehicles.cop_start_points[i].v[2],
            0.f, 0.f, 0.f, i);
    }
    fprintf(f, "END OF OPPONENT PATHS");
    fclose(f);
}

// IDA: int __cdecl NewNodeOKHere()
// FUNCTION: CARM95 0x0040f42b
int NewNodeOKHere(void) {
    br_vector3 last_node_to_this;

    if (gAlready_elasticating) {
        BrVector3Sub(&last_node_to_this,
            &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1]].p,
            &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[0]].p);
        return BrVector3Length(&last_node_to_this) != 0.f;
    }
    return 1;
}

// IDA: void __cdecl ShowHideOppoPaths()
// FUNCTION: CARM95 0x0040ed4b
void ShowHideOppoPaths(void) {

    if (!gAlready_elasticating) {
        gOppo_paths_shown = !gOppo_paths_shown;
        ShowOppoPaths();
    }
}

// IDA: void __cdecl DropElasticateyNode()
// FUNCTION: CARM95 0x0040ee30
void DropElasticateyNode(void) {
    char str[256];
    tS16 old_node;
    tS16 new_node;
    tS16 section_no_index;
    br_scalar distance;
    int all_the_same_type;
    int one_wayness;
    tPath_section_type_enum section_type;
    tPath_section_type_enum original_type;

    all_the_same_type = 1;
    if (!NewNodeOKHere()) {
        return;
    }
    if (gAlready_elasticating) {
        old_node = gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1];
        BrVector3Copy(&gProgram_state.AI_vehicles.path_nodes[old_node].p,
            &gProgram_state.current_car.car_master_actor->t.t.translate.t);
        original_type = gProgram_state.AI_vehicles.path_sections[gMobile_section].type;
        one_wayness = gProgram_state.AI_vehicles.path_sections[gMobile_section].one_way;
        new_node = ReallocExtraPathNodes(1);
        gMobile_section = ReallocExtraPathSections(1);
    } else {
        if (!gOppo_paths_shown) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "You must show paths before adding to them (F5)");
            return;
        }
        if (gProgram_state.AI_vehicles.number_of_path_nodes == 0) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not implemented yet. Go away.");
            return;
        }
        old_node = FindNearestPathNode(&gSelf->t.t.translate.t, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any nodes close enough");
            return;
        }
        original_type = 0;
        if (gProgram_state.AI_vehicles.path_nodes[old_node].number_of_sections != 0) {
            for (section_no_index = 1; section_no_index < gProgram_state.AI_vehicles.path_nodes[old_node].number_of_sections; section_no_index++) {
                if (gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[old_node].sections[section_no_index]].type != gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[old_node].sections[0]].type) {
                    all_the_same_type = 0;
                }
            }
            if (all_the_same_type) {
                original_type = gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[old_node].sections[0]].type;
            }
        }
        gAlready_elasticating = 1;
        new_node = ReallocExtraPathNodes(1);
        gMobile_section = ReallocExtraPathSections(1);
        one_wayness = 0;
    }
    gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[0] = old_node;
    gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1] = new_node;
    gProgram_state.AI_vehicles.path_sections[gMobile_section].min_speed[0] = 0;
    gProgram_state.AI_vehicles.path_sections[gMobile_section].min_speed[1] = 0;
    gProgram_state.AI_vehicles.path_sections[gMobile_section].max_speed[0] = 255;
    gProgram_state.AI_vehicles.path_sections[gMobile_section].max_speed[1] = 255;
    gProgram_state.AI_vehicles.path_sections[gMobile_section].type = original_type;
    gProgram_state.AI_vehicles.path_sections[gMobile_section].one_way = one_wayness;
    if (gProgram_state.AI_vehicles.path_nodes[old_node].number_of_sections == 0) {
        gProgram_state.AI_vehicles.path_sections[gMobile_section].width = 1.f;
    } else {
        gProgram_state.AI_vehicles.path_sections[gMobile_section].width = gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[old_node].sections[0]].width;
    }
    gProgram_state.AI_vehicles.path_nodes[new_node].number_of_sections = 0;
    gProgram_state.AI_vehicles.path_nodes[new_node].sections[gProgram_state.AI_vehicles.path_nodes[new_node].number_of_sections] = gMobile_section;
    gProgram_state.AI_vehicles.path_nodes[new_node].number_of_sections += 1;
    gProgram_state.AI_vehicles.path_nodes[old_node].sections[gProgram_state.AI_vehicles.path_nodes[old_node].number_of_sections] = gMobile_section;
    gProgram_state.AI_vehicles.path_nodes[old_node].number_of_sections += 1;
    ShowOppoPaths();
    sprintf(str, "New section #%d, new node #%d", gMobile_section, new_node);
    NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
}

// IDA: void __cdecl InsertAndElasticate()
// FUNCTION: CARM95 0x0040f565
void InsertAndElasticate(void) {
    tS16 inserted_node;
    tS16 elasticatey_node;
    tS16 section_no;
    tS16 new_section;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_vector3 wank;
    br_scalar distance;
    int not_perp;
    int one_wayness;
    char str[256];
    tPath_section_type_enum section_type;

    not_perp = 0;
    if (NewNodeOKHere()) {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        BrVector3Sub(&wank,
            &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].p,
            &intersect);
        if (BrVector3Length(&wank) == 0.f) {
            not_perp = 1;
        }
        BrVector3Sub(&wank,
            &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].p,
            &intersect);
        if (BrVector3Length(&wank) == 0.f) {
            not_perp = 1;
        }
        if (not_perp || distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Get nearer to the section");
        } else {
            new_section = ReallocExtraPathSections(1);
            if (gAlready_elasticating) {
                inserted_node = gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1];
                section_type = gProgram_state.AI_vehicles.path_sections[gMobile_section].type;
                one_wayness = gProgram_state.AI_vehicles.path_sections[gMobile_section].one_way;
                elasticatey_node = ReallocExtraPathNodes(1);
                gProgram_state.AI_vehicles.path_nodes[elasticatey_node].number_of_sections = 0;
                gProgram_state.AI_vehicles.path_sections[new_section].width = gProgram_state.AI_vehicles.path_sections[gMobile_section].width;
            } else {
                inserted_node = ReallocExtraPathNodes(2);
                gProgram_state.AI_vehicles.path_nodes[inserted_node].number_of_sections = 0;
                elasticatey_node = inserted_node + 1;
                gProgram_state.AI_vehicles.path_nodes[elasticatey_node].number_of_sections = 0;
                gProgram_state.AI_vehicles.path_sections[new_section].width = gProgram_state.AI_vehicles.path_sections[section_no].width;
                section_type = gProgram_state.AI_vehicles.path_sections[section_no].type;
                one_wayness = gProgram_state.AI_vehicles.path_sections[section_no].one_way;
            }
            InsertThisNodeInThisSectionHere(inserted_node, section_no, &gSelf->t.t.translate.t);
            gMobile_section = new_section;
            gProgram_state.AI_vehicles.path_sections[new_section].node_indices[0] = inserted_node;
            gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1] = elasticatey_node;
            gProgram_state.AI_vehicles.path_sections[gMobile_section].min_speed[0] = 0;
            gProgram_state.AI_vehicles.path_sections[gMobile_section].min_speed[1] = 0;
            gProgram_state.AI_vehicles.path_sections[gMobile_section].max_speed[0] = 255;
            gProgram_state.AI_vehicles.path_sections[gMobile_section].max_speed[1] = 255;
            gProgram_state.AI_vehicles.path_sections[gMobile_section].type = section_type;
            gProgram_state.AI_vehicles.path_sections[gMobile_section].one_way = one_wayness;
            gProgram_state.AI_vehicles.path_nodes[inserted_node].sections[gProgram_state.AI_vehicles.path_nodes[inserted_node].number_of_sections] = gMobile_section;
            gProgram_state.AI_vehicles.path_nodes[inserted_node].number_of_sections += 1;
            gProgram_state.AI_vehicles.path_nodes[elasticatey_node].sections[gProgram_state.AI_vehicles.path_nodes[elasticatey_node].number_of_sections] = gMobile_section;
            gProgram_state.AI_vehicles.path_nodes[elasticatey_node].number_of_sections += 1;
            gAlready_elasticating = 1;
            ShowOppoPaths();
            sprintf(str, "New section %d, new node #%d inserted into section #%d",
                gMobile_section, inserted_node, section_no);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl InsertAndDontElasticate()
// FUNCTION: CARM95 0x0040fdf3
void InsertAndDontElasticate(void) {
    tS16 inserted_node;
    tS16 section_no;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_vector3 wank;
    br_scalar distance;
    int not_perp;
    char str[256];

    not_perp = 0;
    if (NewNodeOKHere()) {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        BrVector3Sub(&wank, &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0]].p, &intersect);
        if (BrVector3Length(&wank) == 0.f) {
            not_perp = 1;
        }
        BrVector3Sub(&wank, &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1]].p, &intersect);
        if (BrVector3Length(&wank) == 0.f) {
            not_perp = 1;
        }
        if (not_perp || distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Get nearer to the section");
        } else {
            if (gAlready_elasticating) {
                gAlready_elasticating = 0;
                inserted_node = gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1];
            } else {
                inserted_node = ReallocExtraPathNodes(1);
                gProgram_state.AI_vehicles.path_nodes[inserted_node].number_of_sections = 0;
            }
            InsertThisNodeInThisSectionHere(inserted_node, section_no, &gSelf->t.t.translate.t);
            ShowOppoPaths();
            sprintf(str, "New node #%d inserted into section #%d", inserted_node, section_no);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl DropDeadEndNode()
// FUNCTION: CARM95 0x004100f3
void DropDeadEndNode(void) {
    char str[256];

    if (NewNodeOKHere() && gAlready_elasticating) {
        gAlready_elasticating = 0;
        BrVector3Copy(
            &gProgram_state.AI_vehicles.path_nodes[gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1]].p,
            &gSelf->t.t.translate.t);
        ShowOppoPaths();
        sprintf(str, "New section #%d, finish node #%d",
            gMobile_section,
            gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1]);
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 4000, -1, str);
    }
}

// IDA: void __cdecl DropNodeOnNodeAndStopElasticating()
// FUNCTION: CARM95 0x0041020c
void DropNodeOnNodeAndStopElasticating(void) {
    int node_no;
    char str[256];
    br_scalar distance;

    if (gAlready_elasticating) {
        node_no = FindNearestPathNode(&gSelf->t.t.translate.t, &distance);
        if (gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[0] == node_no || distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any nodes close enough");
        } else if (gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections >= COUNT_OF(gProgram_state.AI_vehicles.path_nodes[node_no].sections)) {
            sprintf(str, "Sorry, node #%d already has %d sections attached", node_no, (int)COUNT_OF(gProgram_state.AI_vehicles.path_nodes[node_no].sections));
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        } else {
            gAlready_elasticating = 0;
            gProgram_state.AI_vehicles.number_of_path_nodes -= 1;
            gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1] = node_no;
            gProgram_state.AI_vehicles.path_nodes[node_no].sections[gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections] = gMobile_section;
            gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections += 1;
            ShowOppoPaths();
            sprintf(str, "New section #%d, attached to existing node #%d",
                gMobile_section,
                gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1]);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 4000, -1, str);
        }
    }
}

// IDA: void __cdecl WidenOppoPathSection()
// FUNCTION: CARM95 0x004103c5
void WidenOppoPathSection(void) {

    if (gOppo_paths_shown) {
        RecalcNearestPathSectionWidth(.05f);
    }
}

// IDA: void __cdecl NarrowOppoPathSection()
// FUNCTION: CARM95 0x00410573
void NarrowOppoPathSection(void) {

    if (gOppo_paths_shown) {
        RecalcNearestPathSectionWidth(-.05f);
    }
}

// IDA: void __cdecl IncreaseSectionMinSpeed()
// FUNCTION: CARM95 0x0041059d
void IncreaseSectionMinSpeed(void) {

    if (gOppo_paths_shown) {
        RecalcNearestPathSectionSpeed(0, 1);
    }
}

// IDA: void __cdecl DecreaseSectionMinSpeed()
// FUNCTION: CARM95 0x00410a7f
void DecreaseSectionMinSpeed(void) {

    if (gOppo_paths_shown) {
        RecalcNearestPathSectionSpeed(0, -1);
    }
}

// IDA: void __cdecl IncreaseSectionMaxSpeed()
// FUNCTION: CARM95 0x00410aa8
void IncreaseSectionMaxSpeed(void) {

    if (gOppo_paths_shown) {
        RecalcNearestPathSectionSpeed(1, 1);
    }
}

// IDA: void __cdecl DecreaseSectionMaxSpeed()
// FUNCTION: CARM95 0x00410ad1
void DecreaseSectionMaxSpeed(void) {

    if (gOppo_paths_shown) {
        RecalcNearestPathSectionSpeed(1, -1);
    }
}

// IDA: void __cdecl PullOppoPoint()
// FUNCTION: CARM95 0x00410afa
void PullOppoPoint(void) {
    tS16 node_no;
    br_scalar distance;

    if (gOppo_paths_shown) {
        if (gAlready_elasticating) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're elasticating");
        } else {
            node_no = FindNearestPathNode(&gSelf->t.t.translate.t, &distance);
            if (distance > 10.f) {
                NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any paths close enough");
            } else {
                BrVector3Copy(&gProgram_state.AI_vehicles.path_nodes[node_no].p, &gSelf->t.t.translate.t);
                ShowOppoPaths();
                NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Bing!");
            }
        }
    }
}

// IDA: void __cdecl ShowNodeInfo()
// FUNCTION: CARM95 0x00410bfa
void ShowNodeInfo(void) {
    tS16 node_no;
    char str[256];
    br_scalar distance;

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        sprintf(str, "Next point will be #%d",
            gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1]);
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
    } else {
        node_no = FindNearestPathNode(&gSelf->t.t.translate.t, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any nodes close enough");
        } else {
            sprintf(str, "Nearest node #%d has %d attached sections",
                node_no,
                gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl ShowSectionInfo1()
// FUNCTION: CARM95 0x00410d28
void ShowSectionInfo1(void) {
    tS16 section_no;
    char str[256];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        sprintf(str, "This section will be #%d attached to nodes #%d and #%d",
            gMobile_section,
            gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[0],
            gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1]);
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
    } else {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any sections close enough");
        } else {
            sprintf(str, "Nearest section #%d, start node #%d, finish node #%d",
                section_no,
                gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[0],
                gProgram_state.AI_vehicles.path_sections[gMobile_section].node_indices[1]);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl ShowSectionInfo2()
// FUNCTION: CARM95 0x00410e9b
void ShowSectionInfo2(void) {
    tS16 section_no;
    char str[256];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        sprintf(str, "Towards start - min %d max %d, finish - min %d, max %d mph",
            (int)(2.2f * gProgram_state.AI_vehicles.path_sections[gMobile_section].min_speed[0]),
            (int)(2.2f * gProgram_state.AI_vehicles.path_sections[gMobile_section].max_speed[0]),
            (int)(2.2f * gProgram_state.AI_vehicles.path_sections[gMobile_section].min_speed[1]),
            (int)(2.2f * gProgram_state.AI_vehicles.path_sections[gMobile_section].max_speed[1]));
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
    } else {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any sections close enough");
        } else {
            sprintf(str, "Towards start - min %d max %d, finish - min %d, max %d mph",
                (int)(2.2f * gProgram_state.AI_vehicles.path_sections[section_no].min_speed[0]),
                (int)(2.2f * gProgram_state.AI_vehicles.path_sections[section_no].max_speed[0]),
                (int)(2.2f * gProgram_state.AI_vehicles.path_sections[section_no].min_speed[1]),
                (int)(2.2f * gProgram_state.AI_vehicles.path_sections[section_no].max_speed[1]));
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl DeleteOppoPathSection()
// FUNCTION: CARM95 0x00411124
void DeleteOppoPathSection(void) {
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;

    if (gOppo_paths_shown == 0) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're creating a new section");
    } else {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any sections close enough");
        } else {
            DeleteSection(section_no);
            DeleteOrphanNodes();
            ShowOppoPaths();
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Pop!");
        }
    }
}

// IDA: void __cdecl DeleteOppoPathNodeAndSections()
// FUNCTION: CARM95 0x00411206
void DeleteOppoPathNodeAndSections(void) {
    br_scalar distance;
    tS16 node_no;

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're creating a new section");
    } else {
        node_no = FindNearestPathNode(&gSelf->t.t.translate.t, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any nodes close enough");
        } else {
            DeleteNode(node_no, 1);
            DeleteOrphanNodes();
            ShowOppoPaths();
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Blam!");
        }
    }
}

// IDA: void __cdecl DeleteOppoPathNodeAndJoin()
// FUNCTION: CARM95 0x004112e2
void DeleteOppoPathNodeAndJoin(void) {
    br_scalar distance;
    tS16 node_no;

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're creating a new section");
    } else {
        node_no = FindNearestPathNode(&gSelf->t.t.translate.t, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any nodes close enough");
        } else if (gProgram_state.AI_vehicles.path_nodes[node_no].number_of_sections != 2) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Node must have exactly 2 sections attached");
        } else if ((gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[node_no].sections[0]].node_indices[0] == node_no
                       && gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[node_no].sections[1]].node_indices[1] == node_no)
            || (gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[node_no].sections[1]].node_indices[0] == node_no
                && gProgram_state.AI_vehicles.path_sections[gProgram_state.AI_vehicles.path_nodes[node_no].sections[0]].node_indices[1] == node_no)) {
            ConsistencyCheck();
            DeleteNode(node_no, 0);
            ConsistencyCheck();
            DeleteOrphanNodes();
            ConsistencyCheck();
            ShowOppoPaths();
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Blam!");
        } else {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Sections must point in same direction");
        }
    }
}

// IDA: void __cdecl ReverseSectionDirection()
// FUNCTION: CARM95 0x004114dd
void ReverseSectionDirection(void) {
    tS16 temp;
    tU8 speed_temp;
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're creating a new section");
    } else {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any sections close enough");
        } else {
            temp = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0];
            gProgram_state.AI_vehicles.path_sections[section_no].node_indices[0] = gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1];
            gProgram_state.AI_vehicles.path_sections[section_no].node_indices[1] = temp;

            speed_temp = gProgram_state.AI_vehicles.path_sections[section_no].min_speed[0];
            gProgram_state.AI_vehicles.path_sections[section_no].min_speed[0] = gProgram_state.AI_vehicles.path_sections[section_no].min_speed[1];
            gProgram_state.AI_vehicles.path_sections[section_no].min_speed[1] = speed_temp;

            speed_temp = gProgram_state.AI_vehicles.path_sections[section_no].max_speed[0];
            gProgram_state.AI_vehicles.path_sections[section_no].max_speed[0] = gProgram_state.AI_vehicles.path_sections[section_no].max_speed[1];
            gProgram_state.AI_vehicles.path_sections[section_no].max_speed[1] = speed_temp;

            ShowOppoPaths();
        }
    }
}

// IDA: void __cdecl CycleSectionType()
// FUNCTION: CARM95 0x0041169c
void CycleSectionType(void) {
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
    char str[256];

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're creating a new section");
    } else {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any sections close enough");
        } else {
            gProgram_state.AI_vehicles.path_sections[section_no].type = (gProgram_state.AI_vehicles.path_sections[section_no].type + 1) % 3;
            sprintf(str, "%s section", gPath_section_type_names[gProgram_state.AI_vehicles.path_sections[section_no].type]);
            ShowOppoPaths();
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl ToggleOneWayNess()
// FUNCTION: CARM95 0x004117ee
void ToggleOneWayNess(void) {
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're creating a new section");
    } else {
        section_no = FindNearestPathSection(&gSelf->t.t.translate.t, &direction_v, &intersect, &distance);
        if (distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Can't find any sections close enough");
        } else {
            if (gProgram_state.AI_vehicles.path_sections[section_no].one_way) {
                gProgram_state.AI_vehicles.path_sections[section_no].one_way = 0;
            } else {
                gProgram_state.AI_vehicles.path_sections[section_no].one_way = 1;
            }
            ShowOppoPaths();
            if (gProgram_state.AI_vehicles.path_sections[section_no].one_way) {
                NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "ONE-WAY");
            } else {
                NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "TWO-WAY");
            }
        }
    }
}

// IDA: void __cdecl CopStartPointInfo()
// FUNCTION: CARM95 0x00411947
void CopStartPointInfo(void) {
    char str[256];
    int i;
    int closest;
    br_scalar closest_distance;
    br_scalar distance;
    br_vector3 car_to_point;

    closest = -1;
    closest_distance = FLT_MAX;
    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else {
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            BrVector3Sub(&car_to_point, &gSelf->t.t.translate.t, &gProgram_state.AI_vehicles.cop_start_points[i]);
            distance = BrVector3LengthSquared(&car_to_point);
            if (distance < closest_distance) {
                closest = i;
                closest_distance = distance;
            }
        }
        if (closest < 0 || closest_distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "No cop start points close enough");
        } else {
            sprintf(str, "Nearest cop start point #%d", closest);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl DropCopStartPoint()
// FUNCTION: CARM95 0x00411ae8
void DropCopStartPoint(void) {
    char str[256];

    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're creating a new section");
    } else {
        if (gProgram_state.AI_vehicles.number_of_cops < COUNT_OF(gProgram_state.AI_vehicles.cop_start_points)) {
            BrVector3Copy(&gProgram_state.AI_vehicles.cop_start_points[gProgram_state.AI_vehicles.number_of_cops], &gSelf->t.t.translate.t);
            gProgram_state.AI_vehicles.number_of_cops += 1;
            ShowOppoPaths();
            sprintf(str, "New cop start point dropped (%d of %d)", gProgram_state.AI_vehicles.number_of_cops, (int)COUNT_OF(gProgram_state.AI_vehicles.cop_start_points));
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        } else {
            sprintf(str, "Sorry, no more than %d cop start points", (int)COUNT_OF(gProgram_state.AI_vehicles.cop_start_points));
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl DeleteCopStartPoint()
// FUNCTION: CARM95 0x00411c18
void DeleteCopStartPoint(void) {
    char str[256];
    int i;
    int closest;
    br_scalar closest_distance;
    br_scalar distance;
    br_vector3 car_to_point;

    closest = -1;
    closest_distance = FLT_MAX;
    if (!gOppo_paths_shown) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Show paths first (F5)");
    } else if (gAlready_elasticating) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "Not while you're creating a new section");
    } else {
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            BrVector3Sub(&car_to_point, &gSelf->t.t.translate.t, &gProgram_state.AI_vehicles.cop_start_points[i]);
            distance = BrVector3Length(&car_to_point);
            if (distance < closest_distance) {
                closest_distance = distance;
                closest = i;
            }
        }
        if (closest < 0 || closest_distance > 10.f) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, "No cop start points close enough");
        } else {
            for (i = closest; i < gProgram_state.AI_vehicles.number_of_cops - 1; i++) {
                BrVector3Copy(&gProgram_state.AI_vehicles.cop_start_points[i],
                    &gProgram_state.AI_vehicles.cop_start_points[i + 1]);
            }
            gProgram_state.AI_vehicles.number_of_cops -= 1;
            ShowOppoPaths();
            sprintf(str, "Deleted cop start point #%d", closest);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -1, str);
        }
    }
}

// IDA: void __cdecl CycleCopStartPointType()
// FUNCTION: CARM95 0x00411e7f
void CycleCopStartPointType(void) {
}
