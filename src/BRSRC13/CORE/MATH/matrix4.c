#include "matrix4.h"
#include "harness_trace.h"

char rscid[51];

// IDA: void __cdecl BrMatrix4Copy(br_matrix4 *A, br_matrix4 *B)
void BrMatrix4Copy(br_matrix4* A, br_matrix4* B) {
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Mul(br_matrix4 *A, br_matrix4 *B, br_matrix4 *C)
void BrMatrix4Mul(br_matrix4* A, br_matrix4* B, br_matrix4* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Identity(br_matrix4 *mat)
void BrMatrix4Identity(br_matrix4* mat) {
    LOG_TRACE("(%p)", mat);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Scale(br_matrix4 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
void BrMatrix4Scale(br_matrix4* mat, br_scalar sx, br_scalar sy, br_scalar sz) {
    LOG_TRACE("(%p, %f, %f, %f)", mat, sx, sy, sz);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl BrMatrix4Inverse(br_matrix4 *A, br_matrix4 *B)
br_scalar BrMatrix4Inverse(br_matrix4* A, br_matrix4* B) {
    int i;
    int j;
    br_scalar det;
    br_scalar idet;
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl Determinant3(br_scalar a1, br_scalar a2, br_scalar a3, br_scalar b1, br_scalar b2, br_scalar b3, br_scalar c1, br_scalar c2, br_scalar c3)
br_scalar Determinant3(br_scalar a1, br_scalar a2, br_scalar a3, br_scalar b1, br_scalar b2, br_scalar b3, br_scalar c1, br_scalar c2, br_scalar c3) {
    LOG_TRACE("(%f, %f, %f, %f, %f, %f, %f, %f, %f)", a1, a2, a3, b1, b2, b3, c1, c2, c3);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl BrMatrix4Determinant(br_matrix4 *mat)
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
    LOG_TRACE("(%p)", mat);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Adjoint(br_matrix4 *A, br_matrix4 *B)
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
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Perspective(br_matrix4 *mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon)
void BrMatrix4Perspective(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon) {
    br_scalar scale;
    LOG_TRACE("(%p, %d, %f, %f, %f)", mat, field_of_view, aspect, hither, yon);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Apply(br_vector4 *A, br_vector4 *B, br_matrix4 *C)
void BrMatrix4Apply(br_vector4* A, br_vector4* B, br_matrix4* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4ApplyP(br_vector4 *A, br_vector3 *B, br_matrix4 *C)
void BrMatrix4ApplyP(br_vector4* A, br_vector3* B, br_matrix4* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4ApplyV(br_vector4 *A, br_vector3 *B, br_matrix4 *C)
void BrMatrix4ApplyV(br_vector4* A, br_vector3* B, br_matrix4* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4TApply(br_vector4 *A, br_vector4 *B, br_matrix4 *C)
void BrMatrix4TApply(br_vector4* A, br_vector4* B, br_matrix4* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4TApplyP(br_vector4 *A, br_vector3 *B, br_matrix4 *C)
void BrMatrix4TApplyP(br_vector4* A, br_vector3* B, br_matrix4* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4TApplyV(br_vector4 *A, br_vector3 *B, br_matrix4 *C)
void BrMatrix4TApplyV(br_vector4* A, br_vector3* B, br_matrix4* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Copy34(br_matrix4 *A, br_matrix34 *B)
void BrMatrix4Copy34(br_matrix4* A, br_matrix34* B) {
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Mul34(br_matrix4 *A, br_matrix34 *B, br_matrix4 *C)
void BrMatrix4Mul34(br_matrix4* A, br_matrix34* B, br_matrix4* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4Pre34(br_matrix4 *A, br_matrix34 *B)
void BrMatrix4Pre34(br_matrix4* A, br_matrix34* B) {
    br_matrix4 C;
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix4ShearZ(br_matrix4 *mat, br_scalar sx, br_scalar sy)
void BrMatrix4ShearZ(br_matrix4* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);
    NOT_IMPLEMENTED();
}
