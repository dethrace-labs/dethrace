#ifndef _ANGLES_H_
#define _ANGLES_H_

#include "brender/br_types.h"
#include <math.h>

#define BR_EULER_XYZ_S 0
#define BR_EULER_XYX_S 8
#define BR_EULER_XZY_S 4
#define BR_EULER_XZX_S 12
#define BR_EULER_YZX_S 1
#define BR_EULER_YZY_S 9
#define BR_EULER_YXZ_S 5
#define BR_EULER_YXY_S 13
#define BR_EULER_ZXY_S 2
#define BR_EULER_ZXZ_S 10
#define BR_EULER_ZYX_S 6
#define BR_EULER_ZYZ_S 14
#define BR_EULER_ZYX_R 16
#define BR_EULER_XYX_R 24
#define BR_EULER_YZX_R 20
#define BR_EULER_XZX_R 28
#define BR_EULER_XZY_R 17
#define BR_EULER_YZY_R 25
#define BR_EULER_ZXY_R 21
#define BR_EULER_YXY_R 29
#define BR_EULER_YXZ_R 18
#define BR_EULER_ZXZ_R 26
#define BR_EULER_XYZ_R 22
#define BR_EULER_ZYZ_R 30

br_matrix34* BrEulerToMatrix34(br_matrix34* mat, br_euler* euler);

br_euler* BrMatrix34ToEuler(br_euler* euler, br_matrix34* mat);

br_matrix4* BrEulerToMatrix4(br_matrix4* mat, br_euler* euler);

br_euler* BrMatrix4ToEuler(br_euler* dest, br_matrix4* mat);

br_quat* BrEulerToQuat(br_quat* q, br_euler* euler);

br_euler* BrQuatToEuler(br_euler* euler, br_quat* q);

#endif
