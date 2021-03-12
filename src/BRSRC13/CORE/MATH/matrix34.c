#include "matrix34.h"
#include "harness.h"

br_matrix34 mattmp1;
br_matrix34 mattmp2;
char rscid[52];

#define A(x, y) A->m[x][y]
#define B(x, y) B->m[x][y]
#define C(x, y) C->m[x][y]
#define BR_MAC3(a, b, c, d, e, f) ((a) * (b) + (c) * (d) + (e) * (f))

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

    A->m[0][0] = C->m[0][0] * B->m[0][0] + C->m[2][0] * B->m[0][2] + B->m[0][1] * C->m[1][0];
    A->m[0][1] = B->m[0][0] * C->m[0][1] + B->m[0][2] * C->m[2][1] + B->m[0][1] * C->m[1][1];
    A->m[0][2] = B->m[0][0] * C->m[0][2] + C->m[1][2] * B->m[0][1] + B->m[0][2] * C->m[2][2];
    A->m[1][0] = C->m[0][0] * B->m[1][0] + B->m[1][2] * C->m[2][0] + C->m[1][0] * B->m[1][1];
    A->m[1][1] = B->m[1][2] * C->m[2][1] + C->m[0][1] * B->m[1][0] + C->m[1][1] * B->m[1][1];
    A->m[1][2] = B->m[1][2] * C->m[2][2] + C->m[1][2] * B->m[1][1] + C->m[0][2] * B->m[1][0];
    A->m[2][0] = C->m[0][0] * B->m[2][0] + C->m[2][0] * B->m[2][2] + B->m[2][1] * C->m[1][0];
    A->m[2][1] = C->m[0][1] * B->m[2][0] + C->m[2][1] * B->m[2][2] + B->m[2][1] * C->m[1][1];
    A->m[2][2] = C->m[1][2] * B->m[2][1] + B->m[2][0] * C->m[0][2] + C->m[2][2] * B->m[2][2];
    A->m[3][0] = C->m[0][0] * B->m[3][0] + C->m[2][0] * B->m[3][2] + B->m[3][1] * C->m[1][0] + C->m[3][0];
    A->m[3][1] = B->m[3][0] * C->m[0][1] + B->m[3][1] * C->m[1][1] + B->m[3][2] * C->m[2][1] + C->m[3][1];
    A->m[3][2] = C->m[1][2] * B->m[3][1] + B->m[3][2] * C->m[2][2] + B->m[3][0] * C->m[0][2] + C->m[3][2];
}

// IDA: void __cdecl BrMatrix34Identity(br_matrix34 *mat)
void BrMatrix34Identity(br_matrix34* mat) {
    // { 1, 0, 0},
    // { 0, 1, 0},
    // { 0, 0, 1}
    // ( 0, 0, 0 }
    mat->m[0][0] = 1.0;
    mat->m[0][1] = 0;
    mat->m[0][2] = 0;

    mat->m[1][0] = 0;
    mat->m[1][1] = 1.0;
    mat->m[1][2] = 0;

    mat->m[2][0] = 0;
    mat->m[2][1] = 0;
    mat->m[2][2] = 1.0;

    mat->m[3][0] = 0;
    mat->m[3][1] = 0;
    mat->m[3][2] = 0;
}

// IDA: void __cdecl BrMatrix34RotateX(br_matrix34 *mat, br_angle rx)
void BrMatrix34RotateX(br_matrix34* mat, br_angle rx) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rx);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34RotateY(br_matrix34 *mat, br_angle ry)
void BrMatrix34RotateY(br_matrix34* mat, br_angle ry) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, ry);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34RotateZ(br_matrix34 *mat, br_angle rz)
void BrMatrix34RotateZ(br_matrix34* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34Translate(br_matrix34 *mat, br_scalar dx, br_scalar dy, br_scalar dz)
void BrMatrix34Translate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, dx, dy, dz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34Scale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, sx, sy, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34ShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
void BrMatrix34ShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sy, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34ShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
void BrMatrix34ShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34ShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
void BrMatrix34ShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34LPInverse(br_matrix34 *A, br_matrix34 *B)
void BrMatrix34LPInverse(br_matrix34* A, br_matrix34* B) {
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34LPNormalise(br_matrix34 *A, br_matrix34 *B)
void BrMatrix34LPNormalise(br_matrix34* A, br_matrix34* B) {
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: br_matrix34* __cdecl BrBoundsToMatrix34(br_matrix34 *mat, br_bounds *bounds)
br_matrix34* BrBoundsToMatrix34(br_matrix34* mat, br_bounds* bounds) {
    int i;
    br_vector3 tr;
    br_vector3 sc;
    LOG_TRACE("(%p, %p)", mat, bounds);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34Copy4(br_matrix34 *A, br_matrix4 *B)
void BrMatrix34Copy4(br_matrix34* A, br_matrix4* B) {
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BrMatrix34TApplyFV(br_vector3 *A@<EAX>, br_fvector3 *B@<EDX>, br_matrix34 *C@<EBX>)
void BrMatrix34TApplyFV(br_vector3* A, br_fvector3* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34Apply(br_vector3 *A, br_vector4 *B, br_matrix34 *C)
void BrMatrix34Apply(br_vector3* A, br_vector4* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34TApply(br_vector4 *A, br_vector4 *B, br_matrix34 *C)
void BrMatrix34TApply(br_vector4* A, br_vector4* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34TApplyP(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
void BrMatrix34TApplyP(br_vector3* A, br_vector3* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34TApplyV(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
void BrMatrix34TApplyV(br_vector3* A, br_vector3* B, br_matrix34* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34Pre(br_matrix34 *mat, br_matrix34 *A)
void BrMatrix34Pre(br_matrix34* mat, br_matrix34* A) {
    br_matrix34 mattmp;
    LOG_TRACE("(%p, %p)", mat, A);

    BrMatrix34Mul(&mattmp, A, mat);
    BrMatrix34Copy(mat, &mattmp);
}

// IDA: void __cdecl BrMatrix34Post(br_matrix34 *mat, br_matrix34 *A)
void BrMatrix34Post(br_matrix34* mat, br_matrix34* A) {
    LOG_TRACE("(%p, %p)", mat, A);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreRotateX(br_matrix34 *mat, br_angle rx)
void BrMatrix34PreRotateX(br_matrix34* mat, br_angle rx) {
    LOG_TRACE("(%p, %d)", mat, rx);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostRotateX(br_matrix34 *mat, br_angle rx)
void BrMatrix34PostRotateX(br_matrix34* mat, br_angle rx) {
    LOG_TRACE("(%p, %d)", mat, rx);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreRotateY(br_matrix34 *mat, br_angle ry)
void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry) {
    LOG_TRACE("(%p, %d)", mat, ry);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostRotateY(br_matrix34 *mat, br_angle ry)
void BrMatrix34PostRotateY(br_matrix34* mat, br_angle ry) {
    LOG_TRACE("(%p, %d)", mat, ry);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreRotateZ(br_matrix34 *mat, br_angle rz)
void BrMatrix34PreRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostRotateZ(br_matrix34 *mat, br_angle rz)
void BrMatrix34PostRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreRotate(br_matrix34 *mat, br_angle r, br_vector3 *axis)
void BrMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    LOG_TRACE("(%p, %d, %p)", mat, r, axis);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostRotate(br_matrix34 *mat, br_angle r, br_vector3 *axis)
void BrMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    LOG_TRACE("(%p, %d, %p)", mat, r, axis);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreTranslate(br_matrix34 *mat, br_scalar x, br_scalar y, br_scalar z)
void BrMatrix34PreTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, x, y, z);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostTranslate(br_matrix34 *mat, br_scalar x, br_scalar y, br_scalar z)
void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, x, y, z);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreScale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, sx, sy, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostScale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
void BrMatrix34PostScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, sx, sy, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
void BrMatrix34PreShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sy, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
void BrMatrix34PostShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sy, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
void BrMatrix34PreShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
void BrMatrix34PostShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
void BrMatrix34PreShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PostShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
void BrMatrix34PostShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);
    NOT_IMPLEMENTED();
}
