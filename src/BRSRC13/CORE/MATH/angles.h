#ifndef _ANGLES_H_
#define _ANGLES_H_

#include "brender/br_types.h"
#include <math.h>

br_matrix34* BrEulerToMatrix34(br_matrix34* mat, br_euler* euler);

br_euler* BrMatrix34ToEuler(br_euler* euler, br_matrix34* mat);

br_matrix4* BrEulerToMatrix4(br_matrix4* mat, br_euler* euler);

br_euler* BrMatrix4ToEuler(br_euler* dest, br_matrix4* mat);

br_quat* BrEulerToQuat(br_quat* q, br_euler* euler);

br_euler* BrQuatToEuler(br_euler* euler, br_quat* q);

#endif
