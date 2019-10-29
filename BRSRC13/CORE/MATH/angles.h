#include "dr_types.h"
#include "br_types.h"
// Offset: 18
// Size: 998
br_matrix34* BrEulerToMatrix34(br_matrix34 *mat, br_euler *euler);

// Offset: 1034
// Size: 1213
br_euler* BrMatrix34ToEuler(br_euler *euler, br_matrix34 *mat);

// Offset: 2264
// Size: 102
br_matrix4* BrEulerToMatrix4(br_matrix4 *mat, br_euler *euler);

// Offset: 2383
// Size: 99
br_euler* BrMatrix4ToEuler(br_euler *dest, br_matrix4 *mat);

// Offset: 2496
// Size: 802
br_quat* BrEulerToQuat(br_quat *q, br_euler *euler);

// Offset: 3312
// Size: 68
br_euler* BrQuatToEuler(br_euler *euler, br_quat *q);

