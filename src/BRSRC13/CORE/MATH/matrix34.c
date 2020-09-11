#include "matrix34.h"
#include "harness.h"

br_matrix34 mattmp1;
br_matrix34 mattmp2;
char rscid[52];

void BrMatrix34Copy(br_matrix34* A, br_matrix34* B) {
    NOT_IMPLEMENTED();
}

void BrMatrix34Mul(br_matrix34* A, br_matrix34* B, br_matrix34* C) {
    NOT_IMPLEMENTED();
}

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

void BrMatrix34RotateX(br_matrix34* mat, br_angle rx) {
    br_scalar s;
    br_scalar c;
    NOT_IMPLEMENTED();
}

void BrMatrix34RotateY(br_matrix34* mat, br_angle ry) {
    br_scalar s;
    br_scalar c;
    NOT_IMPLEMENTED();
}

void BrMatrix34RotateZ(br_matrix34* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void BrMatrix34Translate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34ShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34ShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34ShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    NOT_IMPLEMENTED();
}

br_scalar BrMatrix34Inverse(br_matrix34* B, br_matrix34* A) {
    float idet;
    float det;
    float pos;
    float neg;
    float temp;
    float AF[4][3];
    float BF[4][3];
    int i;
    NOT_IMPLEMENTED();
}

void BrMatrix34LPInverse(br_matrix34* A, br_matrix34* B) {
    NOT_IMPLEMENTED();
}

void BrMatrix34LPNormalise(br_matrix34* A, br_matrix34* B) {
    NOT_IMPLEMENTED();
}

void BrMatrix34RollingBall(br_matrix34* mat, int dx, int dy, int radius) {
    br_scalar nx;
    br_scalar ny;
    br_scalar ca;
    br_scalar sa;
    br_scalar dr;
    br_scalar h;
    NOT_IMPLEMENTED();
}

br_matrix34* BrBoundsToMatrix34(br_matrix34* mat, br_bounds* bounds) {
    int i;
    br_vector3 tr;
    br_vector3 sc;
    NOT_IMPLEMENTED();
}

void BrMatrix34Copy4(br_matrix34* A, br_matrix4* B) {
    NOT_IMPLEMENTED();
}

void BrMatrix34TApplyFV(br_vector3* A, br_fvector3* B, br_matrix34* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix34Apply(br_vector3* A, br_vector4* B, br_matrix34* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix34ApplyP(br_vector3* A, br_vector3* B, br_matrix34* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix34ApplyV(br_vector3* A, br_vector3* B, br_matrix34* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix34TApply(br_vector4* A, br_vector4* B, br_matrix34* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix34TApplyP(br_vector3* A, br_vector3* B, br_matrix34* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix34TApplyV(br_vector3* A, br_vector3* B, br_matrix34* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix34Pre(br_matrix34* mat, br_matrix34* A) {
    NOT_IMPLEMENTED();
}

void BrMatrix34Post(br_matrix34* mat, br_matrix34* A) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreRotateX(br_matrix34* mat, br_angle rx) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostRotateX(br_matrix34* mat, br_angle rx) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostRotateY(br_matrix34* mat, br_angle ry) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreRotateZ(br_matrix34* mat, br_angle rz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostRotateZ(br_matrix34* mat, br_angle rz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostShearX(br_matrix34* mat, br_scalar sy, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostShearY(br_matrix34* mat, br_scalar sx, br_scalar sz) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PostShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy) {
    NOT_IMPLEMENTED();
}
