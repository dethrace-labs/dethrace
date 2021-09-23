#ifndef _QUAT_H_
#define _QUAT_H_

#include "brender/br_types.h"

br_quat* BrQuatMul(br_quat* q, br_quat* l, br_quat* r);

br_quat* BrQuatNormalise(br_quat* q, br_quat* qq);

br_quat* BrQuatInvert(br_quat* q, br_quat* qq);

br_quat* BrQuatSlerp(br_quat* q, br_quat* l, br_quat* r, br_scalar a, br_int_16 spins);

br_matrix34* BrQuatToMatrix34(br_matrix34* mat, br_quat* q);

br_quat* BrMatrix34ToQuat(br_quat* q, br_matrix34* mat);

br_matrix4* BrQuatToMatrix4(br_matrix4* mat, br_quat* q);

br_quat* BrMatrix4ToQuat(br_quat* q, br_matrix4* mat);

#endif
