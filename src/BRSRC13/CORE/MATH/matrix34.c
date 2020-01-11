#include "matrix34.h"

br_matrix34 mattmp1;
br_matrix34 mattmp2;
char rscid[52];

// Offset: 15
// Size: 228
void BrMatrix34Copy(br_matrix34* A, br_matrix34* B) {
}

// Offset: 257
// Size: 682
void BrMatrix34Mul(br_matrix34* A, br_matrix34* B, br_matrix34* C) {
}

// Offset: 958
// Size: 177
void BrMatrix34Identity(br_matrix34* mat) {
}

// Offset: 1153
// Size: 239
void BrMatrix34RotateX(br_matrix34* mat, br_angle rx) {
    br_scalar s;
    br_scalar c;
}

// Offset: 1410
// Size: 239
void BrMatrix34RotateY(br_matrix34* mat, br_angle ry) {
    br_scalar s;
    br_scalar c;
}

// Offset: 1667
// Size: 242
void BrMatrix34RotateZ(br_matrix34* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
}

// Offset: 1926
// Size: 418
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
}

// Offset: 2364
// Size: 177
void BrMatrix34Translate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz) {
}

// Offset: 2557
// Size: 177
void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
}

// Offset: 2751
// Size: 178
void BrMatrix34ShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
}

// Offset: 2946
// Size: 178
void BrMatrix34ShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
}

// Offset: 3141
// Size: 178
void BrMatrix34ShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
}

// Offset: 3337
// Size: 1008
br_scalar BrMatrix34Inverse(br_matrix34* B, br_matrix34* A) {
    float idet;
    float det;
    float pos;
    float neg;
    float temp;
    float AF[4][3];
    float BF[4][3];
    int i;
}

// Offset: 4365
// Size: 374
void BrMatrix34LPInverse(br_matrix34* A, br_matrix34* B) {
}

// Offset: 4761
// Size: 585
void BrMatrix34LPNormalise(br_matrix34* A, br_matrix34* B) {
}

// Offset: 5368
// Size: 354
void BrMatrix34RollingBall(br_matrix34* mat, int dx, int dy, int radius) {
    br_scalar nx;
    br_scalar ny;
    br_scalar ca;
    br_scalar sa;
    br_scalar dr;
    br_scalar h;
}

// Offset: 5741
// Size: 367
br_matrix34* BrBoundsToMatrix34(br_matrix34* mat, br_bounds* bounds) {
    int i;
    br_vector3 tr;
    br_vector3 sc;
}

// Offset: 6124
// Size: 234
void BrMatrix34Copy4(br_matrix34* A, br_matrix4* B) {
}

// Offset: 6377
// Size: 298
// EAX: A
// EDX: B
// EBX: C
void BrMatrix34TApplyFV(br_vector3* A, br_fvector3* B, br_matrix34* C) {
}

// Offset: 6691
// Size: 331
void BrMatrix34Apply(br_vector3* A, br_vector4* B, br_matrix34* C) {
}

// Offset: 7039
// Size: 379
void BrMatrix34ApplyP(br_vector3* A, br_vector3* B, br_matrix34* C) {
}

// Offset: 7435
// Size: 355
void BrMatrix34ApplyV(br_vector3* A, br_vector3* B, br_matrix34* C) {
}

// Offset: 7807
// Size: 408
void BrMatrix34TApply(br_vector4* A, br_vector4* B, br_matrix34* C) {
}

// Offset: 8233
// Size: 355
void BrMatrix34TApplyP(br_vector3* A, br_vector3* B, br_matrix34* C) {
}

// Offset: 8606
// Size: 355
void BrMatrix34TApplyV(br_vector3* A, br_vector3* B, br_matrix34* C) {
}

// Offset: 8975
// Size: 132
void BrMatrix34Pre(br_matrix34* mat, br_matrix34* A) {
}

// Offset: 9122
// Size: 132
void BrMatrix34Post(br_matrix34* mat, br_matrix34* A) {
}

// Offset: 9275
// Size: 124
void BrMatrix34PreRotateX(br_matrix34* mat, br_angle rx) {
}

// Offset: 9421
// Size: 124
void BrMatrix34PostRotateX(br_matrix34* mat, br_angle rx) {
}

// Offset: 9566
// Size: 124
void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry) {
}

// Offset: 9712
// Size: 124
void BrMatrix34PostRotateY(br_matrix34* mat, br_angle ry) {
}

// Offset: 9857
// Size: 124
void BrMatrix34PreRotateZ(br_matrix34* mat, br_angle rz) {
}

// Offset: 10003
// Size: 124
void BrMatrix34PostRotateZ(br_matrix34* mat, br_angle rz) {
}

// Offset: 10147
// Size: 128
void BrMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
}

// Offset: 10296
// Size: 128
void BrMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
}

// Offset: 10447
// Size: 144
void BrMatrix34PreTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z) {
}

// Offset: 10615
// Size: 144
void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z) {
}

// Offset: 10778
// Size: 144
void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
}

// Offset: 10942
// Size: 144
void BrMatrix34PostScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
}

// Offset: 11106
// Size: 135
void BrMatrix34PreShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
}

// Offset: 11262
// Size: 135
void BrMatrix34PostShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
}

// Offset: 11417
// Size: 135
void BrMatrix34PreShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
}

// Offset: 11573
// Size: 135
void BrMatrix34PostShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
}

// Offset: 11728
// Size: 135
void BrMatrix34PreShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
}

// Offset: 11884
// Size: 135
void BrMatrix34PostShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
}
