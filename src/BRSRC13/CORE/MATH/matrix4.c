#include "matrix4.h"
#include "harness/trace.h"
#include <math.h>

#define A(x, y) A->m[x][y]
#define B(x, y) B->m[x][y]
#define C(x, y) C->m[x][y]
#define M(x, y) mat->m[x][y]

#define BR_MAC2(a, b, c, d) ((a) * (b) + (c) * (d))
#define BR_MAC3(a, b, c, d, e, f) ((a) * (b) + (c) * (d) + (e) * (f))
#define BR_MAC4(a, b, c, d, e, f, g, h) ((a) * (b) + (c) * (d) + (e) * (f) + (g) * (h))

// IDA: void __cdecl BrMatrix4Copy(br_matrix4 *A, br_matrix4 *B)
void BrMatrix4Copy(br_matrix4* A, br_matrix4* B) {
    LOG_TRACE("(%p, %p)", A, B);

    A(0, 0) = B(0, 0);
    A(0, 1) = B(0, 1);
    A(0, 2) = B(0, 2);
    A(0, 3) = B(0, 3);
    A(1, 0) = B(1, 0);
    A(1, 1) = B(1, 1);
    A(1, 2) = B(1, 2);
    A(1, 3) = B(1, 3);
    A(2, 0) = B(2, 0);
    A(2, 1) = B(2, 1);
    A(2, 2) = B(2, 2);
    A(2, 3) = B(2, 3);
    A(3, 0) = B(3, 0);
    A(3, 1) = B(3, 1);
    A(3, 2) = B(3, 2);
    A(3, 3) = B(3, 3);
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

    BrMatrix4Adjoint(A, B);
    det = BrMatrix4Determinant(B);

    if (fabs(det) < BR_SCALAR_EPSILON * 2)
        return 0;

    idet = 1.0 / det;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            A(i, j) = A(i, j) * idet;
        }
    }

    return det;
}

// IDA: br_scalar __cdecl Determinant3(br_scalar a1, br_scalar a2, br_scalar a3, br_scalar b1, br_scalar b2, br_scalar b3, br_scalar c1, br_scalar c2, br_scalar c3)
br_scalar Determinant3(br_scalar a1, br_scalar a2, br_scalar a3, br_scalar b1, br_scalar b2, br_scalar b3, br_scalar c1, br_scalar c2, br_scalar c3) {
    LOG_TRACE("(%f, %f, %f, %f, %f, %f, %f, %f, %f)", a1, a2, a3, b1, b2, b3, c1, c2, c3);

    return BR_MAC3(a1, BR_MAC2(b2, c3, -b3, c2),
        -b1, BR_MAC2(a2, c3, -a3, c2),
        c1, BR_MAC2(a2, b3, -a3, b2));
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

    a1 = M(0, 0);
    b1 = M(0, 1);
    c1 = M(0, 2);
    d1 = M(0, 3);

    a2 = M(1, 0);
    b2 = M(1, 1);
    c2 = M(1, 2);
    d2 = M(1, 3);

    a3 = M(2, 0);
    b3 = M(2, 1);
    c3 = M(2, 2);
    d3 = M(2, 3);

    a4 = M(3, 0);
    b4 = M(3, 1);
    c4 = M(3, 2);
    d4 = M(3, 3);

    return BR_MAC4(a1, Determinant3(b2, b3, b4, c2, c3, c4, d2, d3, d4),
        -b1, Determinant3(a2, a3, a4, c2, c3, c4, d2, d3, d4),
        c1, Determinant3(a2, a3, a4, b2, b3, b4, d2, d3, d4),
        -d1, Determinant3(a2, a3, a4, b2, b3, b4, c2, c3, c4));
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

    a1 = B(0, 0);
    b1 = B(0, 1);
    c1 = B(0, 2);
    d1 = B(0, 3);

    a2 = B(1, 0);
    b2 = B(1, 1);
    c2 = B(1, 2);
    d2 = B(1, 3);

    a3 = B(2, 0);
    b3 = B(2, 1);
    c3 = B(2, 2);
    d3 = B(2, 3);

    a4 = B(3, 0);
    b4 = B(3, 1);
    c4 = B(3, 2);
    d4 = B(3, 3);

    /* row column labeling reversed since we transpose rows & columns */

    A(0, 0) = Determinant3(b2, b3, b4, c2, c3, c4, d2, d3, d4);
    A(1, 0) = -Determinant3(a2, a3, a4, c2, c3, c4, d2, d3, d4);
    A(2, 0) = Determinant3(a2, a3, a4, b2, b3, b4, d2, d3, d4);
    A(3, 0) = -Determinant3(a2, a3, a4, b2, b3, b4, c2, c3, c4);

    A(0, 1) = -Determinant3(b1, b3, b4, c1, c3, c4, d1, d3, d4);
    A(1, 1) = Determinant3(a1, a3, a4, c1, c3, c4, d1, d3, d4);
    A(2, 1) = -Determinant3(a1, a3, a4, b1, b3, b4, d1, d3, d4);
    A(3, 1) = Determinant3(a1, a3, a4, b1, b3, b4, c1, c3, c4);

    A(0, 2) = Determinant3(b1, b2, b4, c1, c2, c4, d1, d2, d4);
    A(1, 2) = -Determinant3(a1, a2, a4, c1, c2, c4, d1, d2, d4);
    A(2, 2) = Determinant3(a1, a2, a4, b1, b2, b4, d1, d2, d4);
    A(3, 2) = -Determinant3(a1, a2, a4, b1, b2, b4, c1, c2, c4);

    A(0, 3) = -Determinant3(b1, b2, b3, c1, c2, c3, d1, d2, d3);
    A(1, 3) = Determinant3(a1, a2, a3, c1, c2, c3, d1, d2, d3);
    A(2, 3) = -Determinant3(a1, a2, a3, b1, b2, b3, d1, d2, d3);
    A(3, 3) = Determinant3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
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

    A->v[0] = BR_MAC4(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2), B->v[3], C(0, 3));
    A->v[1] = BR_MAC4(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2), B->v[3], C(1, 3));
    A->v[2] = BR_MAC4(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2), B->v[3], C(2, 3));
    A->v[3] = BR_MAC4(B->v[0], C(3, 0), B->v[1], C(3, 1), B->v[2], C(3, 2), B->v[3], C(3, 3));
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
