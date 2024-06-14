#ifndef _OPPOPROC_H_
#define _OPPOPROC_H_

#include "dr_types.h"

int StraightestArcForCorner2D(br_vector2* pCent, br_scalar* pRadius, br_scalar* pEntry_length, int* pLeft_not_right, br_vector2* p1, br_vector2* p2, br_vector2* p3, br_scalar pWidth12, br_scalar pWidth23);

br_scalar CornerFudge(tCar_spec* pCar_spec);

br_scalar MaxCurvatureForCarSpeed(tCar_spec* pCar, br_scalar pSpeed);

br_scalar Vector2Cross(br_vector2* pA, br_vector2* pB);

tFollow_path_result EndOfPath(tOpponent_spec* pOpponent_spec);

int RoughlyColinear(br_vector2* p1, br_vector2* p2, br_vector2* p3);

int GetStraight(br_vector2* pStart, br_vector2* pFinish, br_scalar* pWidth, int section1, tOpponent_spec* pOpponent_spec, tFollow_path_data* data);

tFollow_path_result ProcessFollowPath(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand, int pPursuit_mode, int pIgnore_end, int pNever_struggle);

tFollow_path_result FollowCheatyPath(tOpponent_spec* pOpponent_spec);

#endif
