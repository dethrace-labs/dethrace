#include "quat.h"
#include "harness_trace.h"

char rscid[48];

// IDA: br_quat* __cdecl BrQuatMul(br_quat *q, br_quat *l, br_quat *r)
br_quat* BrQuatMul(br_quat* q, br_quat* l, br_quat* r) {
    br_scalar x1;
    br_scalar x2;
    br_scalar x3;
    br_scalar x4;
    br_scalar x5;
    br_scalar x6;
    br_scalar x7;
    br_scalar x8;
    br_scalar s;
    br_scalar t;
    LOG_TRACE("(%p, %p, %p)", q, l, r);
    NOT_IMPLEMENTED();
}

// IDA: br_quat* __cdecl BrQuatNormalise(br_quat *q, br_quat *qq)
br_quat* BrQuatNormalise(br_quat* q, br_quat* qq) {
    br_scalar s;
    LOG_TRACE("(%p, %p)", q, qq);
    NOT_IMPLEMENTED();
}

// IDA: br_quat* __cdecl BrQuatInvert(br_quat *q, br_quat *qq)
br_quat* BrQuatInvert(br_quat* q, br_quat* qq) {
    LOG_TRACE("(%p, %p)", q, qq);
    NOT_IMPLEMENTED();
}

// IDA: br_quat* __cdecl BrQuatSlerp(br_quat *q, br_quat *l, br_quat *r, br_scalar a, br_int_16 spins)
br_quat* BrQuatSlerp(br_quat* q, br_quat* l, br_quat* r, br_scalar a, br_int_16 spins) {
    int omega;
    int omega_spin;
    br_scalar s_omega;
    br_scalar c_omega;
    br_scalar scale_l;
    br_scalar scale_r;
    br_quat t;
    LOG_TRACE("(%p, %p, %p, %f, %d)", q, l, r, a, spins);
    NOT_IMPLEMENTED();
}

// IDA: br_matrix34* __cdecl BrQuatToMatrix34(br_matrix34 *mat, br_quat *q)
br_matrix34* BrQuatToMatrix34(br_matrix34* mat, br_quat* q) {
    br_scalar xs;
    br_scalar ys;
    br_scalar zs;
    br_scalar wx;
    br_scalar wy;
    br_scalar wz;
    br_scalar xx;
    br_scalar xy;
    br_scalar xz;
    br_scalar yy;
    br_scalar yz;
    br_scalar zz;
    LOG_TRACE("(%p, %p)", mat, q);
    NOT_IMPLEMENTED();
}

// IDA: br_quat* __cdecl BrMatrix34ToQuat(br_quat *q, br_matrix34 *mat)
br_quat* BrMatrix34ToQuat(br_quat* q, br_matrix34* mat) {
    br_scalar tr;
    br_scalar s;
    int i;
    int j;
    int k;
    LOG_TRACE("(%p, %p)", q, mat);
    NOT_IMPLEMENTED();
}

// IDA: br_matrix4* __cdecl BrQuatToMatrix4(br_matrix4 *mat, br_quat *q)
br_matrix4* BrQuatToMatrix4(br_matrix4* mat, br_quat* q) {
    br_matrix34 tmp;
    LOG_TRACE("(%p, %p)", mat, q);
    NOT_IMPLEMENTED();
}

// IDA: br_quat* __cdecl BrMatrix4ToQuat(br_quat *q, br_matrix4 *mat)
br_quat* BrMatrix4ToQuat(br_quat* q, br_matrix4* mat) {
    br_matrix34 tmp;
    LOG_TRACE("(%p, %p)", q, mat);
    NOT_IMPLEMENTED();
}
