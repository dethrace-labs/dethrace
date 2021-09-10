#include "matrix23.h"
#include "harness_trace.h"

br_matrix23 mattmp1;
br_matrix23 mattmp2;

// IDA: void __cdecl BrMatrix23Copy(br_matrix23 *A, br_matrix23 *B)
void BrMatrix23Copy(br_matrix23* A, br_matrix23* B) {
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23Mul(br_matrix23 *A, br_matrix23 *B, br_matrix23 *C)
void BrMatrix23Mul(br_matrix23* A, br_matrix23* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23Identity(br_matrix23 *mat)
void BrMatrix23Identity(br_matrix23* mat) {
    mat->m[0][0] = 1.0;
    mat->m[0][1] = 0.0;
    mat->m[1][0] = 0.0;
    mat->m[1][1] = 1.0;
    mat->m[2][0] = 0.0;
    mat->m[2][1] = 0.0;
}

// IDA: void __cdecl BrMatrix23Rotate(br_matrix23 *mat, br_angle rz)
void BrMatrix23Rotate(br_matrix23* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23Translate(br_matrix23 *mat, br_scalar dx, br_scalar dy)
void BrMatrix23Translate(br_matrix23* mat, br_scalar dx, br_scalar dy) {
    LOG_TRACE("(%p, %f, %f)", mat, dx, dy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23Scale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
void BrMatrix23Scale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23ShearX(br_matrix23 *mat, br_scalar sy)
void BrMatrix23ShearX(br_matrix23* mat, br_scalar sy) {
    LOG_TRACE("(%p, %f)", mat, sy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23ShearY(br_matrix23 *mat, br_scalar sx)
void BrMatrix23ShearY(br_matrix23* mat, br_scalar sx) {
    LOG_TRACE("(%p, %f)", mat, sx);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl BrMatrix23Inverse(br_matrix23 *B, br_matrix23 *A)
br_scalar BrMatrix23Inverse(br_matrix23* B, br_matrix23* A) {
    br_scalar det;
    br_scalar idet;
    br_scalar pos;
    br_scalar neg;
    LOG_TRACE("(%p, %p)", B, A);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23LPInverse(br_matrix23 *B, br_matrix23 *A)
void BrMatrix23LPInverse(br_matrix23* B, br_matrix23* A) {
    LOG_TRACE("(%p, %p)", B, A);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23LPNormalise(br_matrix23 *A, br_matrix23 *B)
void BrMatrix23LPNormalise(br_matrix23* A, br_matrix23* B) {
    LOG_TRACE("(%p, %p)", A, B);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23ApplyP(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
void BrMatrix23ApplyP(br_vector2* A, br_vector2* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23ApplyV(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
void BrMatrix23ApplyV(br_vector2* A, br_vector2* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23TApplyP(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
void BrMatrix23TApplyP(br_vector2* A, br_vector2* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23TApplyV(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
void BrMatrix23TApplyV(br_vector2* A, br_vector2* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23Pre(br_matrix23 *mat, br_matrix23 *A)
void BrMatrix23Pre(br_matrix23* mat, br_matrix23* A) {
    LOG_TRACE("(%p, %p)", mat, A);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23Post(br_matrix23 *mat, br_matrix23 *A)
void BrMatrix23Post(br_matrix23* mat, br_matrix23* A) {
    LOG_TRACE("(%p, %p)", mat, A);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PreRotate(br_matrix23 *mat, br_angle rz)
void BrMatrix23PreRotate(br_matrix23* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PostRotate(br_matrix23 *mat, br_angle rz)
void BrMatrix23PostRotate(br_matrix23* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PreTranslate(br_matrix23 *mat, br_scalar x, br_scalar y)
void BrMatrix23PreTranslate(br_matrix23* mat, br_scalar x, br_scalar y) {
    LOG_TRACE("(%p, %f, %f)", mat, x, y);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PostTranslate(br_matrix23 *A, br_scalar x, br_scalar y)
void BrMatrix23PostTranslate(br_matrix23* A, br_scalar x, br_scalar y) {
    LOG_TRACE("(%p, %f, %f)", A, x, y);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PreScale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
void BrMatrix23PreScale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PostScale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
void BrMatrix23PostScale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PreShearX(br_matrix23 *mat, br_scalar sy)
void BrMatrix23PreShearX(br_matrix23* mat, br_scalar sy) {
    LOG_TRACE("(%p, %f)", mat, sy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PostShearX(br_matrix23 *mat, br_scalar sy)
void BrMatrix23PostShearX(br_matrix23* mat, br_scalar sy) {
    LOG_TRACE("(%p, %f)", mat, sy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PreShearY(br_matrix23 *mat, br_scalar sx)
void BrMatrix23PreShearY(br_matrix23* mat, br_scalar sx) {
    LOG_TRACE("(%p, %f)", mat, sx);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix23PostShearY(br_matrix23 *mat, br_scalar sx)
void BrMatrix23PostShearY(br_matrix23* mat, br_scalar sx) {
    LOG_TRACE("(%p, %f)", mat, sx);
    NOT_IMPLEMENTED();
}
