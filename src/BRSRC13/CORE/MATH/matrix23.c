#include "matrix23.h"
#include "harness.h"

br_matrix23 mattmp1;
br_matrix23 mattmp2;
char rscid[52];

void BrMatrix23Copy(br_matrix23* A, br_matrix23* B) {
    NOT_IMPLEMENTED();
}

void BrMatrix23Mul(br_matrix23* A, br_matrix23* B, br_matrix23* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix23Identity(br_matrix23* mat) {
    mat->m[0][0] = 1.0;
    mat->m[0][1] = 0.0;
    mat->m[1][0] = 0.0;
    mat->m[1][1] = 1.0;
    mat->m[2][0] = 0.0;
    mat->m[2][1] = 0.0;
}

void BrMatrix23Rotate(br_matrix23* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    NOT_IMPLEMENTED();
}

void BrMatrix23Translate(br_matrix23* mat, br_scalar dx, br_scalar dy) {
    NOT_IMPLEMENTED();
}

void BrMatrix23Scale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    NOT_IMPLEMENTED();
}

void BrMatrix23ShearX(br_matrix23* mat, br_scalar sy) {
    NOT_IMPLEMENTED();
}

void BrMatrix23ShearY(br_matrix23* mat, br_scalar sx) {
    NOT_IMPLEMENTED();
}

br_scalar BrMatrix23Inverse(br_matrix23* B, br_matrix23* A) {
    br_scalar det;
    br_scalar idet;
    br_scalar pos;
    br_scalar neg;
    NOT_IMPLEMENTED();
}

void BrMatrix23LPInverse(br_matrix23* B, br_matrix23* A) {
    NOT_IMPLEMENTED();
}

void BrMatrix23LPNormalise(br_matrix23* A, br_matrix23* B) {
    NOT_IMPLEMENTED();
}

void BrMatrix23ApplyP(br_vector2* A, br_vector2* B, br_matrix23* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix23ApplyV(br_vector2* A, br_vector2* B, br_matrix23* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix23TApplyP(br_vector2* A, br_vector2* B, br_matrix23* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix23TApplyV(br_vector2* A, br_vector2* B, br_matrix23* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix23Pre(br_matrix23* mat, br_matrix23* A) {
    NOT_IMPLEMENTED();
}

void BrMatrix23Post(br_matrix23* mat, br_matrix23* A) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PreRotate(br_matrix23* mat, br_angle rz) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PostRotate(br_matrix23* mat, br_angle rz) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PreTranslate(br_matrix23* mat, br_scalar x, br_scalar y) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PostTranslate(br_matrix23* A, br_scalar x, br_scalar y) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PreScale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PostScale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PreShearX(br_matrix23* mat, br_scalar sy) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PostShearX(br_matrix23* mat, br_scalar sy) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PreShearY(br_matrix23* mat, br_scalar sx) {
    NOT_IMPLEMENTED();
}

void BrMatrix23PostShearY(br_matrix23* mat, br_scalar sx) {
    NOT_IMPLEMENTED();
}
