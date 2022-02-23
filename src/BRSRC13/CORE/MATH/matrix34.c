#include "matrix34.h"
#include "harness/trace.h"
#include "vector.h"
#include <math.h>

br_matrix34 mattmp1;
br_matrix34 mattmp2;

#define M(x, y) mat->m[x][y]
#define A(x, y) A->m[x][y]
#define B(x, y) B->m[x][y]
#define C(x, y) C->m[x][y]
#define BR_MAC3(a, b, c, d, e, f) ((a) * (b) + (c) * (d) + (e) * (f))
#define BR_MAC4(a, b, c, d, e, f, g, h) ((a) * (b) + (c) * (d) + (e) * (f) + (g) * (h))

// IDA: void __cdecl BrMatrix34Copy(br_matrix34 *A, br_matrix34 *B)
void BrMatrix34Copy(br_matrix34* A, br_matrix34* B) {
    LOG_TRACE("(%p, %p)", A, B);

    A(0, 0) = B(0, 0);
    A(0, 1) = B(0, 1);
    A(0, 2) = B(0, 2);

    A(1, 0) = B(1, 0);
    A(1, 1) = B(1, 1);
    A(1, 2) = B(1, 2);

    A(2, 0) = B(2, 0);
    A(2, 1) = B(2, 1);
    A(2, 2) = B(2, 2);

    A(3, 0) = B(3, 0);
    A(3, 1) = B(3, 1);
    A(3, 2) = B(3, 2);
}

// IDA: void __cdecl BrMatrix34Mul(br_matrix34 *A, br_matrix34 *B, br_matrix34 *C)
void BrMatrix34Mul(br_matrix34* A, br_matrix34* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A(0, 0) = BR_MAC3(B(0, 0), C(0, 0), B(0, 1), C(1, 0), B(0, 2), C(2, 0));
    A(0, 1) = BR_MAC3(B(0, 0), C(0, 1), B(0, 1), C(1, 1), B(0, 2), C(2, 1));
    A(0, 2) = BR_MAC3(B(0, 0), C(0, 2), B(0, 1), C(1, 2), B(0, 2), C(2, 2));
    A(1, 0) = BR_MAC3(B(1, 0), C(0, 0), B(1, 1), C(1, 0), B(1, 2), C(2, 0));
    A(1, 1) = BR_MAC3(B(1, 0), C(0, 1), B(1, 1), C(1, 1), B(1, 2), C(2, 1));
    A(1, 2) = BR_MAC3(B(1, 0), C(0, 2), B(1, 1), C(1, 2), B(1, 2), C(2, 2));
    A(2, 0) = BR_MAC3(B(2, 0), C(0, 0), B(2, 1), C(1, 0), B(2, 2), C(2, 0));
    A(2, 1) = BR_MAC3(B(2, 0), C(0, 1), B(2, 1), C(1, 1), B(2, 2), C(2, 1));
    A(2, 2) = BR_MAC3(B(2, 0), C(0, 2), B(2, 1), C(1, 2), B(2, 2), C(2, 2));
    A(3, 0) = BR_MAC3(B(3, 0), C(0, 0), B(3, 1), C(1, 0), B(3, 2), C(2, 0)) + C(3, 0);
    A(3, 1) = BR_MAC3(B(3, 0), C(0, 1), B(3, 1), C(1, 1), B(3, 2), C(2, 1)) + C(3, 1);
    A(3, 2) = BR_MAC3(B(3, 0), C(0, 2), B(3, 1), C(1, 2), B(3, 2), C(2, 2)) + C(3, 2);
}

// IDA: void __cdecl BrMatrix34Identity(br_matrix34 *mat)
void BrMatrix34Identity(br_matrix34* mat) {
    // { 1, 0, 0},
    // { 0, 1, 0},
    // { 0, 0, 1}
    // ( 0, 0, 0 }
    M(0, 0) = 1.f;
    M(0, 1) = 0.f;
    M(0, 2) = 0.f;

    M(1, 0) = 0.f;
    M(1, 1) = 1.f;
    M(1, 2) = 0.f;

    M(2, 0) = 0.f;
    M(2, 1) = 0.f;
    M(2, 2) = 1.f;

    M(3, 0) = 0.f;
    M(3, 1) = 0.f;
    M(3, 2) = 0.f;
}

// IDA: void __cdecl BrMatrix34RotateX(br_matrix34 *mat, br_angle rx)
void BrMatrix34RotateX(br_matrix34* mat, br_angle rx) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rx);

    s = BR_SIN(rx);
    c = BR_COS(rx);

    M(0, 0) = 1;
    M(0, 1) = 0;
    M(0, 2) = 0;
    M(1, 0) = 0;
    M(1, 1) = c;
    M(1, 2) = s;
    M(2, 0) = 0;
    M(2, 1) = -s;
    M(2, 2) = c;
    M(3, 0) = 0;
    M(3, 1) = 0;
    M(3, 2) = 0;
}

// IDA: void __cdecl BrMatrix34RotateY(br_matrix34 *mat, br_angle ry)
void BrMatrix34RotateY(br_matrix34* mat, br_angle ry) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, ry);

    s = BR_SIN(ry);
    c = BR_COS(ry);

    M(0, 0) = c;
    M(0, 1) = 0;
    M(0, 2) = -s;
    M(1, 0) = 0;
    M(1, 1) = 1;
    M(1, 2) = 0;
    M(2, 0) = s;
    M(2, 1) = 0;
    M(2, 2) = c;
    M(3, 0) = 0;
    M(3, 1) = 0;
    M(3, 2) = 0;
}

// IDA: void __cdecl BrMatrix34RotateZ(br_matrix34 *mat, br_angle rz)
void BrMatrix34RotateZ(br_matrix34* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rz);

    s = BR_SIN(rz);
    c = BR_COS(rz);

    M(0, 0) = c;
    M(0, 1) = s;
    M(0, 2) = 0;
    M(1, 0) = -s;
    M(1, 1) = c;
    M(1, 2) = 0;
    M(2, 0) = 0;
    M(2, 1) = 0;
    M(2, 2) = 1;
    M(3, 0) = 0;
    M(3, 1) = 0;
    M(3, 2) = 0;
}

// IDA: void __cdecl BrMatrix34Rotate(br_matrix34 *mat, br_angle r, br_vector3 *a)
void BrMatrix34Rotate(br_matrix34* mat, br_angle r, br_vector3* a) {
    br_scalar t;
    br_scalar s;
    br_scalar c;
    br_scalar txy;
    br_scalar txz;
    br_scalar tyz;
    br_scalar sx;
    br_scalar sy;
    br_scalar sz;
    LOG_TRACE("(%p, %d, %p)", mat, r, a);

    s = BR_SIN(r);
    c = BR_COS(r);
    t = 1.0 - c;

    txy = t * a->v[0];
    txz = txy * a->v[2];
    txy = txy * a->v[1];
    tyz = t * a->v[1] * a->v[2];

    sx = s * a->v[0];
    sy = s * a->v[1];
    sz = s * a->v[2];

    M(0, 0) = t * a->v[0] * a->v[0] + c;
    M(0, 1) = txy + sz;
    M(0, 2) = txz - sy;
    M(1, 0) = txy - sz;
    M(1, 1) = t * a->v[1] * a->v[1] + c;
    M(1, 2) = tyz + sx;
    M(2, 0) = txz + sy;
    M(2, 1) = tyz - sx;
    M(2, 2) = t * a->v[2] * a->v[2] + c;

    M(3, 0) = M(3, 1) = M(3, 2) = 0.0;
}

// IDA: void __cdecl BrMatrix34Translate(br_matrix34 *mat, br_scalar dx, br_scalar dy, br_scalar dz)
void BrMatrix34Translate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, dx, dy, dz);

    M(0, 0) = 1.f;
    M(0, 1) = 0.f;
    M(0, 2) = 0.f;
    M(1, 0) = 0.f;
    M(1, 1) = 1.f;
    M(1, 2) = 0.f;
    M(2, 0) = 0.f;
    M(2, 1) = 0.f;
    M(2, 2) = 1.f;
    M(3, 0) = dx;
    M(3, 1) = dy;
    M(3, 2) = dz;
}

// IDA: void __cdecl BrMatrix34Scale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, sx, sy, sz);

    M(0, 0) = sx;
    M(0, 1) = 0.f;
    M(0, 2) = 0.f;
    M(1, 0) = 0.f;
    M(1, 1) = sy;
    M(1, 2) = 0.f;
    M(2, 0) = 0.f;
    M(2, 1) = 0.f;
    M(2, 2) = sz;
    M(3, 0) = 0.f;
    M(3, 1) = 0.f;
    M(3, 2) = 0.f;
}

// IDA: void __cdecl BrMatrix34ShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
void BrMatrix34ShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sy, sz);

    M(0, 0) = 1.f;
    M(0, 1) = sy;
    M(0, 2) = sz;
    M(1, 0) = 0.f;
    M(1, 1) = 1.f;
    M(1, 2) = 0.f;
    M(2, 0) = 0.f;
    M(2, 1) = 0.f;
    M(2, 2) = 1.f;
    M(3, 0) = 0.f;
    M(3, 1) = 0.f;
    M(3, 2) = 0.f;
}

// IDA: void __cdecl BrMatrix34ShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
void BrMatrix34ShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sz);

    M(0, 0) = 1.f;
    M(0, 1) = 0.f;
    M(0, 2) = 0.f;
    M(1, 0) = sx;
    M(1, 1) = 1.f;
    M(1, 2) = sz;
    M(2, 0) = 0.f;
    M(2, 1) = 0.f;
    M(2, 2) = 1.f;
    M(3, 0) = 0.f;
    M(3, 1) = 0.f;
    M(3, 2) = 0.f;
}

// IDA: void __cdecl BrMatrix34ShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
void BrMatrix34ShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);

    M(0, 0) = 1.f;
    M(0, 1) = 0.f;
    M(0, 2) = 0.f;
    M(1, 0) = 0.f;
    M(1, 1) = 1.f;
    M(1, 2) = 0.f;
    M(2, 0) = sx;
    M(2, 1) = sy;
    M(2, 2) = 1.f;
    M(3, 0) = 0.f;
    M(3, 1) = 0.f;
    M(3, 2) = 0.f;
}

// IDA: br_scalar __cdecl BrMatrix34Inverse(br_matrix34 *B, br_matrix34 *A)
br_scalar BrMatrix34Inverse(br_matrix34* B, br_matrix34* A) {
    float idet;
    float det;
    float pos;
    float neg;
    float temp;
    float AF[4][3];
    float BF[4][3];
    int i;
    LOG_TRACE("(%p, %p)", B, A);

#define AF(x, y) (AF[x][y])
#define BF(x, y) (BF[x][y])

#define ACCUMULATE   \
    if (temp >= 0.f) \
        pos += temp; \
    else             \
        neg += temp;
#define PRECISION_LIMIT BR_SCALAR(1.0e-15)
#define ABS(a) (((a) < 0) ? -(a) : (a))

    for (i = 0; i < 4; i++) {
        AF(i, 0) = A(i, 0);
        AF(i, 1) = A(i, 1);
        AF(i, 2) = A(i, 2);
    }

    pos = neg = 0.0F;
    temp = AF(0, 0) * AF(1, 1) * AF(2, 2);
    ACCUMULATE
    temp = AF(0, 1) * AF(1, 2) * AF(2, 0);
    ACCUMULATE
    temp = AF(0, 2) * AF(1, 0) * AF(2, 1);
    ACCUMULATE
    temp = -AF(0, 2) * AF(1, 1) * AF(2, 0);
    ACCUMULATE
    temp = -AF(0, 1) * AF(1, 0) * AF(2, 2);
    ACCUMULATE
    temp = -AF(0, 0) * AF(1, 2) * AF(2, 1);
    ACCUMULATE
    det = pos + neg;

    if (ABS(det) <= PRECISION_LIMIT)
        return 0.f;

    if ((ABS(det / (pos - neg)) < PRECISION_LIMIT)) {
        return 0.f;
    }

    idet = 1.f / det;

    BF(0, 0) = (AF(1, 1) * AF(2, 2) - AF(1, 2) * AF(2, 1)) * idet;
    BF(1, 0) = -(AF(1, 0) * AF(2, 2) - AF(1, 2) * AF(2, 0)) * idet;
    BF(2, 0) = (AF(1, 0) * AF(2, 1) - AF(1, 1) * AF(2, 0)) * idet;
    BF(0, 1) = -(AF(0, 1) * AF(2, 2) - AF(0, 2) * AF(2, 1)) * idet;
    BF(1, 1) = (AF(0, 0) * AF(2, 2) - AF(0, 2) * AF(2, 0)) * idet;
    BF(2, 1) = -(AF(0, 0) * AF(2, 1) - AF(0, 1) * AF(2, 0)) * idet;
    BF(0, 2) = (AF(0, 1) * AF(1, 2) - AF(0, 2) * AF(1, 1)) * idet;
    BF(1, 2) = -(AF(0, 0) * AF(1, 2) - AF(0, 2) * AF(1, 0)) * idet;
    BF(2, 2) = (AF(0, 0) * AF(1, 1) - AF(0, 1) * AF(1, 0)) * idet;

    BF(3, 0) = -(AF(3, 0) * BF(0, 0) + AF(3, 1) * BF(1, 0) + AF(3, 2) * BF(2, 0));
    BF(3, 1) = -(AF(3, 0) * BF(0, 1) + AF(3, 1) * BF(1, 1) + AF(3, 2) * BF(2, 1));
    BF(3, 2) = -(AF(3, 0) * BF(0, 2) + AF(3, 1) * BF(1, 2) + AF(3, 2) * BF(2, 2));

    for (i = 0; i < 4; i++) {
        B(i, 0) = BF(i, 0);
        B(i, 1) = BF(i, 1);
        B(i, 2) = BF(i, 2);
    }

    return det;
}

// IDA: void __cdecl BrMatrix34LPInverse(br_matrix34 *A, br_matrix34 *B)
void BrMatrix34LPInverse(br_matrix34* A, br_matrix34* B) {
    LOG_TRACE("(%p, %p)", A, B);

    A(0, 0) = B(0, 0);
    A(0, 1) = B(1, 0);
    A(0, 2) = B(2, 0);

    A(1, 0) = B(0, 1);
    A(1, 1) = B(1, 1);
    A(1, 2) = B(2, 1);

    A(2, 0) = B(0, 2);
    A(2, 1) = B(1, 2);
    A(2, 2) = B(2, 2);

    A(3, 0) = -BR_MAC3(B(3, 0), A(0, 0), B(3, 1), A(1, 0), B(3, 2), A(2, 0));
    A(3, 1) = -BR_MAC3(B(3, 0), A(0, 1), B(3, 1), A(1, 1), B(3, 2), A(2, 1));
    A(3, 2) = -BR_MAC3(B(3, 0), A(0, 2), B(3, 1), A(1, 2), B(3, 2), A(2, 2));
}

// IDA: void __cdecl BrMatrix34LPNormalise(br_matrix34 *A, br_matrix34 *B)
void BrMatrix34LPNormalise(br_matrix34* A, br_matrix34* B) {
    LOG_TRACE("(%p, %p)", A, B);

    BrVector3Normalise((br_vector3*)A->m[2], (br_vector3*)B->m[2]);
    BrVector3Cross((br_vector3*)A->m[0], (br_vector3*)B->m[1], (br_vector3*)A->m[2]);
    BrVector3Normalise((br_vector3*)A->m[0], (br_vector3*)A->m[0]);
    BrVector3Cross((br_vector3*)A->m[1], (br_vector3*)A->m[2], (br_vector3*)A->m[0]);

    A(3, 0) = B(3, 0);
    A(3, 1) = B(3, 1);
    A(3, 2) = B(3, 2);
}

// IDA: void __cdecl BrMatrix34RollingBall(br_matrix34 *mat, int dx, int dy, int radius)
void BrMatrix34RollingBall(br_matrix34* mat, int dx, int dy, int radius) {
    br_scalar nx;
    br_scalar ny;
    br_scalar ca;
    br_scalar sa;
    br_scalar dr;
    br_scalar h;
    LOG_TRACE("(%p, %d, %d, %d)", mat, dx, dy, radius);

    // The rolling ball, Graphics Gems III (1993), pages 51-60, Academic Press

    dr = sqrtf(dx * dx + dy * dy);
    if (dr == BR_SCALAR(.0f)) {
        BrMatrix34Identity(mat);
        return;
    }
    h = sqrtf(dr * dr + radius * radius);
    ca = radius / h;
    sa = dr / h;
    nx = -dy / dr;
    ny = dx / dr;
    // nz = 0;

    h = (1 - ca);

    M(0, 0) = nx * nx * h + ca;
    M(0, 1) = nx * ny * h;
    M(0, 2) = ny * sa;
    M(1, 1) = ca + ny * ny * h;
    M(1, 2) = -nx * sa;
    M(2, 2) = ca;

    M(1, 0) = M(0, 1);
    M(2, 0) = -M(0, 2);
    M(2, 1) = -M(1, 2);

    M(3, 0) = BR_SCALAR(0.f);
    M(3, 1) = BR_SCALAR(0.f);
    M(3, 2) = BR_SCALAR(0.f);
}

// IDA: br_matrix34* __cdecl BrBoundsToMatrix34(br_matrix34 *mat, br_bounds *bounds)
br_matrix34* BrBoundsToMatrix34(br_matrix34* mat, br_bounds* bounds) {
    int i;
    br_vector3 tr;
    br_vector3 sc;
    LOG_TRACE("(%p, %p)", mat, bounds);

    for (i = 0; i < 3; ++i) {
        tr.v[i] = 0.5f * bounds->min.v[i] + 0.5f * bounds->max.v[i];
        if (bounds->min.v[i] == bounds->max.v[i]) {
            sc.v[i] = 1.f;
        } else {
            sc.v[i] = 0.5f * bounds->max.v[i] - 0.5f * bounds->min.v[i];
        }
    }

    M(0, 0) = sc.v[0];
    M(0, 1) = 0.f;
    M(0, 2) = 0.f;
    M(1, 0) = 0.f;
    M(1, 1) = sc.v[1];
    M(1, 2) = 0.f;
    M(2, 0) = 0.f;
    M(2, 1) = 0.f;
    M(2, 2) = sc.v[2];
    M(3, 0) = tr.v[0];
    M(3, 1) = tr.v[1];
    M(3, 2) = tr.v[2];
    return mat;
}

// IDA: void __cdecl BrMatrix34Copy4(br_matrix34 *A, br_matrix4 *B)
void BrMatrix34Copy4(br_matrix34* A, br_matrix4* B) {
    LOG_TRACE("(%p, %p)", A, B);

    A(0, 0) = B(0, 0);
    A(0, 1) = B(0, 1);
    A(0, 2) = B(0, 2);

    A(1, 0) = B(1, 0);
    A(1, 1) = B(1, 1);
    A(1, 2) = B(1, 2);

    A(2, 0) = B(2, 0);
    A(2, 1) = B(2, 1);
    A(2, 2) = B(2, 2);

    A(3, 0) = B(3, 0);
    A(3, 1) = B(3, 1);
    A(3, 2) = B(3, 2);
}

// IDA: void __usercall BrMatrix34TApplyFV(br_vector3 *A@<EAX>, br_fvector3 *B@<EDX>, br_matrix34 *C@<EBX>)
void BrMatrix34TApplyFV(br_vector3* A, br_fvector3* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
}

// IDA: void __cdecl BrMatrix34Apply(br_vector3 *A, br_vector4 *B, br_matrix34 *C)
void BrMatrix34Apply(br_vector3* A, br_vector4* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->v[0] = BR_MAC4(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0), B->v[3], C(3, 0));
    A->v[1] = BR_MAC4(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1), B->v[3], C(3, 1));
    A->v[2] = BR_MAC4(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2), B->v[3], C(3, 2));
}

// IDA: void __cdecl BrMatrix34ApplyP(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
void BrMatrix34ApplyP(br_vector3* A, br_vector3* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0)) + C(3, 0);
    A->v[1] = BR_MAC3(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1)) + C(3, 1);
    A->v[2] = BR_MAC3(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2)) + C(3, 2);
}

// IDA: void __cdecl BrMatrix34ApplyV(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
void BrMatrix34ApplyV(br_vector3* A, br_vector3* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0));
    A->v[1] = BR_MAC3(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1));
    A->v[2] = BR_MAC3(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2));
}

// IDA: void __cdecl BrMatrix34TApply(br_vector4 *A, br_vector4 *B, br_matrix34 *C)
void BrMatrix34TApply(br_vector4* A, br_vector4* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C)

    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
    A->v[3] = BR_MAC3(B->v[0], C(3, 0), B->v[1], C(3, 1), B->v[2], C(3, 2)) + B->v[3];
}

// IDA: void __cdecl BrMatrix34TApplyP(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
void BrMatrix34TApplyP(br_vector3* A, br_vector3* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    // translation elements are presumed zero or irrelevant
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
}

// IDA: void __cdecl BrMatrix34TApplyV(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
void BrMatrix34TApplyV(br_vector3* A, br_vector3* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    // translation elements are presumed zero or irrelevant
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
}

// IDA: void __cdecl BrMatrix34Pre(br_matrix34 *mat, br_matrix34 *A)
void BrMatrix34Pre(br_matrix34* mat, br_matrix34* A) {
    LOG_TRACE("(%p, %p)", mat, A);

    BrMatrix34Mul(&mattmp1, A, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34Post(br_matrix34 *mat, br_matrix34 *A)
void BrMatrix34Post(br_matrix34* mat, br_matrix34* A) {
    LOG_TRACE("(%p, %p)", mat, A);

    BrMatrix34Mul(&mattmp1, mat, A);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PreRotateX(br_matrix34 *mat, br_angle rx)
void BrMatrix34PreRotateX(br_matrix34* mat, br_angle rx) {
    LOG_TRACE("(%p, %d)", mat, rx);

    BrMatrix34RotateX(&mattmp2, rx);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostRotateX(br_matrix34 *mat, br_angle rx)
void BrMatrix34PostRotateX(br_matrix34* mat, br_angle rx) {
    LOG_TRACE("(%p, %d)", mat, rx);

    BrMatrix34RotateX(&mattmp2, rx);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PreRotateY(br_matrix34 *mat, br_angle ry)
void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry) {
    LOG_TRACE("(%p, %d)", mat, ry);

    BrMatrix34RotateY(&mattmp2, ry);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostRotateY(br_matrix34 *mat, br_angle ry)
void BrMatrix34PostRotateY(br_matrix34* mat, br_angle ry) {
    LOG_TRACE("(%p, %d)", mat, ry);

    BrMatrix34RotateY(&mattmp2, ry);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PreRotateZ(br_matrix34 *mat, br_angle rz)
void BrMatrix34PreRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    BrMatrix34RotateZ(&mattmp2, rz);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostRotateZ(br_matrix34 *mat, br_angle rz)
void BrMatrix34PostRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    BrMatrix34RotateZ(&mattmp2, rz);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PreRotate(br_matrix34 *mat, br_angle r, br_vector3 *axis)
void BrMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    LOG_TRACE("(%p, %d, %p)", mat, r, axis);

    BrMatrix34Rotate(&mattmp2, r, axis);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostRotate(br_matrix34 *mat, br_angle r, br_vector3 *axis)
void BrMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    LOG_TRACE("(%p, %d, %p)", mat, r, axis);

    BrMatrix34Rotate(&mattmp2, r, axis);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PreTranslate(br_matrix34 *mat, br_scalar x, br_scalar y, br_scalar z)
void BrMatrix34PreTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, x, y, z);

    BrMatrix34Translate(&mattmp2, x, y, z);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostTranslate(br_matrix34 *mat, br_scalar x, br_scalar y, br_scalar z)
void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, x, y, z);

    M(3, 0) += x;
    M(3, 1) += y;
    M(3, 2) += z;
}

// IDA: void __cdecl BrMatrix34PreScale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, sx, sy, sz);

    BrMatrix34Scale(&mattmp2, sx, sy, sz);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostScale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
void BrMatrix34PostScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, sx, sy, sz);

    BrMatrix34Scale(&mattmp1, sx, sy, sz);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

// IDA: void __cdecl BrMatrix34PreShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
void BrMatrix34PreShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sy, sz);

    BrMatrix34ShearX(&mattmp2, sy, sz);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
void BrMatrix34PostShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sy, sz);

    BrMatrix34ShearX(&mattmp2, sy, sz);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PreShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
void BrMatrix34PreShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sz);

    BrMatrix34ShearY(&mattmp2, sx, sz);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
void BrMatrix34PostShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sz);

    BrMatrix34ShearY(&mattmp2, sx, sz);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PreShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
void BrMatrix34PreShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);

    BrMatrix34ShearZ(&mattmp2, sx, sy);
    BrMatrix34Mul(&mattmp1, &mattmp2, mat);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __cdecl BrMatrix34PostShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
void BrMatrix34PostShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);

    BrMatrix34ShearZ(&mattmp2, sx, sy);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
}
