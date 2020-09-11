#include "matrix4.h"
#include "harness.h"

char rscid[51];

void BrMatrix4Copy(br_matrix4* A, br_matrix4* B) {
    NOT_IMPLEMENTED();
}

void BrMatrix4Mul(br_matrix4* A, br_matrix4* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix4Identity(br_matrix4* mat) {
    NOT_IMPLEMENTED();
}

void BrMatrix4Scale(br_matrix4* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    NOT_IMPLEMENTED();
}

br_scalar BrMatrix4Inverse(br_matrix4* A, br_matrix4* B) {
    int i;
    int j;
    br_scalar det;
    br_scalar idet;
    NOT_IMPLEMENTED();
}

br_scalar Determinant3(br_scalar a1, br_scalar a2, br_scalar a3, br_scalar b1, br_scalar b2, br_scalar b3, br_scalar c1, br_scalar c2, br_scalar c3) {
    NOT_IMPLEMENTED();
}

br_scalar BrMatrix4Determinant(br_matrix4* mat) {
    br_scalar a1;
    br_scalar a2;
    br_scalar a3;
    br_scalar a4;
    br_scalar b1;
    br_scalar b2;
    br_scalar b3;
    br_scalar b4;
    br_scalar c1;
    br_scalar c2;
    br_scalar c3;
    br_scalar c4;
    br_scalar d1;
    br_scalar d2;
    br_scalar d3;
    br_scalar d4;
    NOT_IMPLEMENTED();
}

void BrMatrix4Adjoint(br_matrix4* A, br_matrix4* B) {
    br_scalar a1;
    br_scalar a2;
    br_scalar a3;
    br_scalar a4;
    br_scalar b1;
    br_scalar b2;
    br_scalar b3;
    br_scalar b4;
    br_scalar c1;
    br_scalar c2;
    br_scalar c3;
    br_scalar c4;
    br_scalar d1;
    br_scalar d2;
    br_scalar d3;
    br_scalar d4;
    NOT_IMPLEMENTED();
}

void BrMatrix4Perspective(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon) {
    br_scalar scale;
    NOT_IMPLEMENTED();
}

void BrMatrix4Apply(br_vector4* A, br_vector4* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix4ApplyP(br_vector4* A, br_vector3* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix4ApplyV(br_vector4* A, br_vector3* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix4TApply(br_vector4* A, br_vector4* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix4TApplyP(br_vector4* A, br_vector3* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix4TApplyV(br_vector4* A, br_vector3* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix4Copy34(br_matrix4* A, br_matrix34* B) {
    NOT_IMPLEMENTED();
}

void BrMatrix4Mul34(br_matrix4* A, br_matrix34* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

void BrMatrix4Pre34(br_matrix4* A, br_matrix34* B) {
    br_matrix4 C;
    NOT_IMPLEMENTED();
}

void BrMatrix4ShearZ(br_matrix4* mat, br_scalar sx, br_scalar sy) {
    NOT_IMPLEMENTED();
}
