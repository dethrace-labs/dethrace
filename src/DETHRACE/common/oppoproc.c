#include "oppoproc.h"
#include <stdlib.h>

// Offset: 0
// Size: 724
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
    NOT_IMPLEMENTED();
}

// Offset: 724
// Size: 47
br_scalar CornerFudge(tCar_spec* pCar_spec) {
    NOT_IMPLEMENTED();
}

// Offset: 772
// Size: 92
br_scalar MaxCurvatureForCarSpeed(tCar_spec* pCar, br_scalar pSpeed) {
    br_scalar curv;
    NOT_IMPLEMENTED();
}

// Offset: 864
// Size: 70
br_scalar Vector2Cross(br_vector2* pA, br_vector2* pB) {
    NOT_IMPLEMENTED();
}

// Offset: 936
// Size: 108
tFollow_path_result EndOfPath(tOpponent_spec* pOpponent_spec) {
    tCar_spec* car_spec;
    NOT_IMPLEMENTED();
}

// Offset: 1044
// Size: 235
int RoughlyColinear(br_vector2* p1, br_vector2* p2, br_vector2* p3) {
    br_vector2 rel1;
    br_vector2 rel2;
    br_vector2 sum;
    br_scalar cross;
    br_scalar dot;
    br_scalar wibble;
    NOT_IMPLEMENTED();
}

// Offset: 1280
// Size: 296
int GetStraight(br_vector2* pStart, br_vector2* pFinish, br_scalar* pWidth, int section1, tOpponent_spec* pOpponent_spec, tFollow_path_data* data) {
    int section;
    br_vector2 next;
    br_scalar next_width;
    NOT_IMPLEMENTED();
}

// Offset: 1576
// Size: 5921
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
    NOT_IMPLEMENTED();
}

// Offset: 7500
// Size: 3018
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
    NOT_IMPLEMENTED();
}
