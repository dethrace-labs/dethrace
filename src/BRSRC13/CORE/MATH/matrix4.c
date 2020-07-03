#include "matrix4.h"
#include "harness.h"

char rscid[51];

// Offset: 14
// Size: 276
void BrMatrix4Copy(br_matrix4* A, br_matrix4* B) {
    NOT_IMPLEMENTED();
}

// Offset: 303
// Size: 1065
void BrMatrix4Mul(br_matrix4* A, br_matrix4* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

// Offset: 1386
// Size: 217
void BrMatrix4Identity(br_matrix4* mat) {
    NOT_IMPLEMENTED();
}

// Offset: 1618
// Size: 214
void BrMatrix4Scale(br_matrix4* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    NOT_IMPLEMENTED();
}

// Offset: 1849
// Size: 307
br_scalar BrMatrix4Inverse(br_matrix4* A, br_matrix4* B) {
    int i;
    int j;
    br_scalar det;
    br_scalar idet;
    NOT_IMPLEMENTED();
}

// Offset: 2169
// Size: 109
br_scalar Determinant3(br_scalar a1, br_scalar a2, br_scalar a3, br_scalar b1, br_scalar b2, br_scalar b3, br_scalar c1, br_scalar c2, br_scalar c3) {
    NOT_IMPLEMENTED();
}

// Offset: 2299
// Size: 603
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

// Offset: 2919
// Size: 1722
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

// Offset: 4662
// Size: 342
void BrMatrix4Perspective(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon) {
    br_scalar scale;
    NOT_IMPLEMENTED();
}

// Offset: 5019
// Size: 390
void BrMatrix4Apply(br_vector4* A, br_vector4* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

// Offset: 5425
// Size: 366
void BrMatrix4ApplyP(br_vector4* A, br_vector3* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

// Offset: 5807
// Size: 334
void BrMatrix4ApplyV(br_vector4* A, br_vector3* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

// Offset: 6157
// Size: 390
void BrMatrix4TApply(br_vector4* A, br_vector4* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

// Offset: 6564
// Size: 366
void BrMatrix4TApplyP(br_vector4* A, br_vector3* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

// Offset: 6947
// Size: 334
void BrMatrix4TApplyV(br_vector4* A, br_vector3* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

// Offset: 7297
// Size: 274
void BrMatrix4Copy34(br_matrix4* A, br_matrix34* B) {
    NOT_IMPLEMENTED();
}

// Offset: 7586
// Size: 882
void BrMatrix4Mul34(br_matrix4* A, br_matrix34* B, br_matrix4* C) {
    NOT_IMPLEMENTED();
}

// Offset: 8483
// Size: 125
void BrMatrix4Pre34(br_matrix4* A, br_matrix34* B) {
    br_matrix4 C;
    NOT_IMPLEMENTED();
}

// Offset: 8624
// Size: 218
void BrMatrix4ShearZ(br_matrix4* mat, br_scalar sx, br_scalar sy) {
    NOT_IMPLEMENTED();
}
