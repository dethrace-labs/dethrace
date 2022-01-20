#include "matrix23.h"
#include "harness/trace.h"

#include <math.h>

#define A(x, y) A->m[x][y]
#define B(x, y) B->m[x][y]
#define C(x, y) C->m[x][y]
#define M(x, y) mat->m[x][y]

#define BR_MAC2(a, b, c, d) ((a) * (b) + (c) * (d))

br_matrix23 mattmp1_23;
br_matrix23 mattmp2_23;

// IDA: void __cdecl BrMatrix23Copy(br_matrix23 *A, br_matrix23 *B)
void BrMatrix23Copy(br_matrix23* A, br_matrix23* B) {
    LOG_TRACE("(%p, %p)", A, B);

    A(0, 0) = B(0, 0);
    A(0, 1) = B(0, 1);
    A(1, 0) = B(1, 0);
    A(1, 1) = B(1, 1);
    A(2, 0) = B(2, 0);
    A(2, 1) = B(2, 1);
}

// IDA: void __cdecl BrMatrix23Mul(br_matrix23 *A, br_matrix23 *B, br_matrix23 *C)
void BrMatrix23Mul(br_matrix23* A, br_matrix23* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A(0, 0) = BR_MAC2(B(0, 0), C(0, 0), B(0, 1), C(1, 0));
    A(0, 1) = BR_MAC2(B(0, 0), C(0, 1), B(0, 1), C(1, 1));
    A(1, 0) = BR_MAC2(B(1, 0), C(0, 0), B(1, 1), C(1, 0));
    A(1, 1) = BR_MAC2(B(1, 0), C(0, 1), B(1, 1), C(1, 1));
    A(2, 0) = BR_MAC2(B(2, 0), C(0, 0), B(2, 1), C(1, 0)) + C(2, 0);
    A(2, 1) = BR_MAC2(B(2, 0), C(0, 1), B(2, 1), C(1, 1)) + C(2, 1);
}

// IDA: void __cdecl BrMatrix23Identity(br_matrix23 *mat)
void BrMatrix23Identity(br_matrix23* mat) {
    LOG_TRACE("(%p)", mat);

    M(0, 0) = 1.f;
    M(0, 1) = 0.f;
    M(1, 0) = 0.f;
    M(1, 1) = 1.f;
    M(2, 0) = 0.f;
    M(2, 1) = 0.f;
}

// IDA: void __cdecl BrMatrix23Rotate(br_matrix23 *mat, br_angle rz)
void BrMatrix23Rotate(br_matrix23* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rz);

    c = BR_COS(rz);
    s = BR_SIN(rz);

    M(0, 0) = c;
    M(0, 1) = s;
    M(1, 0) = -s;
    M(1, 1) = c;
    M(2, 0) = BR_SCALAR(0.f);
    M(2, 1) = BR_SCALAR(0.f);
}

// IDA: void __cdecl BrMatrix23Translate(br_matrix23 *mat, br_scalar dx, br_scalar dy)
void BrMatrix23Translate(br_matrix23* mat, br_scalar dx, br_scalar dy) {
    LOG_TRACE("(%p, %f, %f)", mat, dx, dy);

    M(0, 0) = BR_SCALAR(1.f);
    M(0, 1) = BR_SCALAR(0.f);
    M(1, 0) = BR_SCALAR(0.f);
    M(1, 1) = BR_SCALAR(1.f);
    M(2, 0) = dx;
    M(2, 1) = dy;
}

// IDA: void __cdecl BrMatrix23Scale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
void BrMatrix23Scale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);

    M(0, 0) = sx;
    M(0, 1) = BR_SCALAR(0.f);
    M(1, 0) = BR_SCALAR(0.f);
    M(1, 1) = sy;
    M(2, 0) = BR_SCALAR(0.f);
    M(2, 1) = BR_SCALAR(0.f);
}

// IDA: void __cdecl BrMatrix23ShearX(br_matrix23 *mat, br_scalar sy)
void BrMatrix23ShearX(br_matrix23* mat, br_scalar sy) {
    LOG_TRACE("(%p, %f)", mat, sy);

    M(0, 0) = BR_SCALAR(1.f);
    M(0, 1) = sy;
    M(1, 0) = BR_SCALAR(0.f);
    M(1, 1) = BR_SCALAR(1.f);
    M(2, 0) = BR_SCALAR(0.f);
    M(2, 1) = BR_SCALAR(0.f);
}

// IDA: void __cdecl BrMatrix23ShearY(br_matrix23 *mat, br_scalar sx)
void BrMatrix23ShearY(br_matrix23* mat, br_scalar sx) {
    LOG_TRACE("(%p, %f)", mat, sx);

    M(0, 0) = BR_SCALAR(1.f);
    M(0, 1) = BR_SCALAR(0.f);
    M(1, 0) = sx;
    M(1, 1) = BR_SCALAR(1.f);
    M(2, 0) = BR_SCALAR(0.f);
    M(2, 1) = BR_SCALAR(0.f);
}

// IDA: br_scalar __cdecl BrMatrix23Inverse(br_matrix23 *B, br_matrix23 *A)
br_scalar BrMatrix23Inverse(br_matrix23* B, br_matrix23* A) {
    br_scalar det;
    br_scalar idet;
    br_scalar pos;
    br_scalar neg;
    LOG_TRACE("(%p, %p)", B, A);

    idet = A(0, 0) * A(1, 1);
    if (idet < BR_SCALAR(0.f)) {
        pos = BR_SCALAR(0.f);
        neg = idet;
    } else {
        pos = idet;
        neg = BR_SCALAR(0.f);
    }
    idet = -A(0, 1) * A(1, 0);
    if (idet < BR_SCALAR(0.f)) {
        neg = neg + idet;
    } else {
        pos = pos + idet;
    }
    det = pos + neg;

    if (fabs(det) < 2.384186e-07f) {
        return BR_SCALAR(0.f);
    }
    idet = 1 / det;
    B(0, 0) = A(1, 1) * idet;
    B(0, 1) = -A(0, 1) * idet;
    B(1, 0) = -A(1, 0) * idet;
    B(1, 1) = A(0, 0) * idet;
    B(2, 0) = BR_MAC2(A(1, 0), A(2, 1), -A(2, 0), A(1, 1)) * idet;
    B(2, 1) = BR_MAC2(-A(2, 1), A(0, 0), A(0, 1), A(2, 0)) * idet;
    return det;
}

// IDA: void __cdecl BrMatrix23LPInverse(br_matrix23 *B, br_matrix23 *A)
void BrMatrix23LPInverse(br_matrix23* B, br_matrix23* A) {
    LOG_TRACE("(%p, %p)", B, A);

    B(0, 0) = A(1, 1);
    B(0, 1) = -A(0, 1);
    B(1, 0) = -A(1, 0);
    B(1, 1) = A(0, 0);
    B(2, 0) = BR_MAC2(A(1, 0), A(2, 1), -A(2, 0), A(1, 1));
    B(2, 1) = BR_MAC2(-A(2, 1), A(0, 0), A(0, 1), A(2, 0));
}

// IDA: void __cdecl BrMatrix23LPNormalise(br_matrix23 *A, br_matrix23 *B)
void BrMatrix23LPNormalise(br_matrix23* A, br_matrix23* B) {
    LOG_TRACE("(%p, %p)", A, B);

    br_scalar norm = sqrtf(BR_MAC2(B(1, 0), B(1, 0), B(1, 1), B(1,1)));
    if (norm < 2.384186e-07f) {
        A(1, 0) = BR_SCALAR(1.f);
        A(1, 1) = BR_SCALAR(0.f);
    } else {
        A(1, 0) = B(1, 0) / norm;
        A(1, 1) = B(1, 1) / norm;
    }
    A(0, 0) = A(1, 1);
    A(0, 1) = -A(1, 0);
    A(2, 0) = B(2, 0);
    A(2, 1) = B(2, 1);
}

// IDA: void __cdecl BrMatrix23ApplyP(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
void BrMatrix23ApplyP(br_vector2* A, br_vector2* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->v[0] = BR_MAC2(B->v[0], C(0, 0), B->v[1], C(1, 0)) + C(2, 0);
    A->v[1] = BR_MAC2(B->v[0], C(0, 1), B->v[1], C(1, 1)) + C(2, 1);
}

// IDA: void __cdecl BrMatrix23ApplyV(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
void BrMatrix23ApplyV(br_vector2* A, br_vector2* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->v[0] = BR_MAC2(B->v[0], C(0, 0), B->v[1], C(1, 0));
    A->v[1] = BR_MAC2(B->v[0], C(0, 1), B->v[1], C(1, 1));
}

// IDA: void __cdecl BrMatrix23TApplyP(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
void BrMatrix23TApplyP(br_vector2* A, br_vector2* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->v[0] = BR_MAC2(B->v[0], C(0, 0), B->v[1], C(0, 1));
    A->v[1] = BR_MAC2(B->v[0], C(1, 0), B->v[1], C(1, 1));
}

// IDA: void __cdecl BrMatrix23TApplyV(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
void BrMatrix23TApplyV(br_vector2* A, br_vector2* B, br_matrix23* C) {
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->v[0] = BR_MAC2(B->v[0], C(0, 0), B->v[1], C(0, 1));
    A->v[1] = BR_MAC2(B->v[0], C(1, 0), B->v[1], C(1, 1));
}

// IDA: void __cdecl BrMatrix23Pre(br_matrix23 *mat, br_matrix23 *A)
void BrMatrix23Pre(br_matrix23* mat, br_matrix23* A) {
    LOG_TRACE("(%p, %p)", mat, A);

    BrMatrix23Mul(&mattmp1_23, A, mat);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23Post(br_matrix23 *mat, br_matrix23 *A)
void BrMatrix23Post(br_matrix23* mat, br_matrix23* A) {
    LOG_TRACE("(%p, %p)", mat, A);

    BrMatrix23Mul(&mattmp1_23, mat, A);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PreRotate(br_matrix23 *mat, br_angle rz)
void BrMatrix23PreRotate(br_matrix23* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    BrMatrix23Rotate(&mattmp2_23, rz);
    BrMatrix23Mul(&mattmp1_23, &mattmp2_23, mat);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PostRotate(br_matrix23 *mat, br_angle rz)
void BrMatrix23PostRotate(br_matrix23* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    BrMatrix23Rotate(&mattmp2_23, rz);
    BrMatrix23Mul(&mattmp1_23, mat, &mattmp2_23);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PreTranslate(br_matrix23 *mat, br_scalar x, br_scalar y)
void BrMatrix23PreTranslate(br_matrix23* mat, br_scalar x, br_scalar y) {
    LOG_TRACE("(%p, %f, %f)", mat, x, y);

    BrMatrix23Translate(&mattmp2_23, x, y);
    BrMatrix23Mul(&mattmp1_23, &mattmp2_23, mat);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PostTranslate(br_matrix23 *A, br_scalar x, br_scalar y)
void BrMatrix23PostTranslate(br_matrix23* A, br_scalar x, br_scalar y) {
    LOG_TRACE("(%p, %f, %f)", A, x, y);

    A(2, 0) += x;
    A(2, 1) += y;
}

// IDA: void __cdecl BrMatrix23PreScale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
void BrMatrix23PreScale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);

    BrMatrix23Scale(&mattmp2_23, sx, sy);
    BrMatrix23Mul(&mattmp1_23, &mattmp2_23, mat);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PostScale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
void BrMatrix23PostScale(br_matrix23* mat, br_scalar sx, br_scalar sy) {
    LOG_TRACE("(%p, %f, %f)", mat, sx, sy);

    BrMatrix23Scale(&mattmp2_23, sx, sy);
    BrMatrix23Mul(&mattmp1_23, mat, &mattmp2_23);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PreShearX(br_matrix23 *mat, br_scalar sy)
void BrMatrix23PreShearX(br_matrix23* mat, br_scalar sy) {
    LOG_TRACE("(%p, %f)", mat, sy);

    BrMatrix23ShearX(&mattmp2_23, sy);
    BrMatrix23Mul(&mattmp1_23, &mattmp2_23, mat);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PostShearX(br_matrix23 *mat, br_scalar sy)
void BrMatrix23PostShearX(br_matrix23* mat, br_scalar sy) {
    LOG_TRACE("(%p, %f)", mat, sy);

    BrMatrix23ShearX(&mattmp2_23, sy);
    BrMatrix23Mul(&mattmp1_23, mat, &mattmp2_23);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PreShearY(br_matrix23 *mat, br_scalar sx)
void BrMatrix23PreShearY(br_matrix23* mat, br_scalar sx) {
    LOG_TRACE("(%p, %f)", mat, sx);

    BrMatrix23ShearY(&mattmp2_23, sx);
    BrMatrix23Mul(&mattmp1_23, &mattmp2_23, mat);
    BrMatrix23Copy(mat, &mattmp1_23);
}

// IDA: void __cdecl BrMatrix23PostShearY(br_matrix23 *mat, br_scalar sx)
void BrMatrix23PostShearY(br_matrix23* mat, br_scalar sx) {
    LOG_TRACE("(%p, %f)", mat, sx);

    BrMatrix23ShearY(&mattmp2_23, sx);
    BrMatrix23Mul(&mattmp1_23, mat, &mattmp2_23);
    BrMatrix23Copy(mat, &mattmp1_23);
}
