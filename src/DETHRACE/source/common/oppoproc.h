#ifndef _OPPOPROC_H_
#define _OPPOPROC_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 724
// EAX: pCent
// EDX: pRadius
// EBX: pEntry_length
// ECX: pLeft_not_right
int StraightestArcForCorner2D(br_vector2 *pCent, br_scalar *pRadius, br_scalar *pEntry_length, int *pLeft_not_right, br_vector2 *p1, br_vector2 *p2, br_vector2 *p3, br_scalar pWidth12, br_scalar pWidth23);

// Offset: 724
// Size: 47
// EAX: pCar_spec
br_scalar CornerFudge(tCar_spec *pCar_spec);

// Offset: 772
// Size: 92
// EAX: pCar
br_scalar MaxCurvatureForCarSpeed(tCar_spec *pCar, br_scalar pSpeed);

// Offset: 864
// Size: 70
// EAX: pA
// EDX: pB
br_scalar Vector2Cross(br_vector2 *pA, br_vector2 *pB);

// Offset: 936
// Size: 108
// EAX: pOpponent_spec
tFollow_path_result EndOfPath(tOpponent_spec *pOpponent_spec);

// Offset: 1044
// Size: 235
// EAX: p1
// EDX: p2
// EBX: p3
int RoughlyColinear(br_vector2 *p1, br_vector2 *p2, br_vector2 *p3);

// Offset: 1280
// Size: 296
// EAX: pStart
// EDX: pFinish
// EBX: pWidth
// ECX: section1
int GetStraight(br_vector2 *pStart, br_vector2 *pFinish, br_scalar *pWidth, int section1, tOpponent_spec *pOpponent_spec, tFollow_path_data *data);

// Offset: 1576
// Size: 5921
// EAX: pOpponent_spec
// EDX: pCommand
// EBX: pPursuit_mode
// ECX: pIgnore_end
tFollow_path_result ProcessFollowPath(tOpponent_spec *pOpponent_spec, tProcess_objective_command pCommand, int pPursuit_mode, int pIgnore_end, int pNever_struggle);

// Offset: 7500
// Size: 3018
// EAX: pOpponent_spec
tFollow_path_result FollowCheatyPath(tOpponent_spec *pOpponent_spec);

#endif
