#ifndef _QUAT_H_
#define _QUAT_H_

#include "br_types.h"

// Offset: 10
// Size: 433
br_quat* BrQuatMul(br_quat* q, br_quat* l, br_quat* r);

// Offset: 459
// Size: 211
br_quat* BrQuatNormalise(br_quat* q, br_quat* qq);

// Offset: 683
// Size: 147
br_quat* BrQuatInvert(br_quat* q, br_quat* qq);

// Offset: 842
// Size: 612
br_quat* BrQuatSlerp(br_quat* q, br_quat* l, br_quat* r, br_scalar a, br_int_16 spins);

// Offset: 1471
// Size: 399
br_matrix34* BrQuatToMatrix34(br_matrix34* mat, br_quat* q);

// Offset: 1887
// Size: 589
br_quat* BrMatrix34ToQuat(br_quat* q, br_matrix34* mat);

// Offset: 2492
// Size: 133
br_matrix4* BrQuatToMatrix4(br_matrix4* mat, br_quat* q);

// Offset: 2641
// Size: 130
br_quat* BrMatrix4ToQuat(br_quat* q, br_matrix4* mat);

#endif
