#ifndef _MATRIX23_H_
#define _MATRIX23_H_

#include "brender/br_types.h"

void BrMatrix23Copy(br_matrix23* A, br_matrix23* B);

void BrMatrix23Mul(br_matrix23* A, br_matrix23* B, br_matrix23* C);

void BrMatrix23Identity(br_matrix23* mat);

void BrMatrix23Rotate(br_matrix23* mat, br_angle rz);

void BrMatrix23Translate(br_matrix23* mat, br_scalar dx, br_scalar dy);

void BrMatrix23Scale(br_matrix23* mat, br_scalar sx, br_scalar sy);

void BrMatrix23ShearX(br_matrix23* mat, br_scalar sy);

void BrMatrix23ShearY(br_matrix23* mat, br_scalar sx);

br_scalar BrMatrix23Inverse(br_matrix23* B, br_matrix23* A);

void BrMatrix23LPInverse(br_matrix23* B, br_matrix23* A);

void BrMatrix23LPNormalise(br_matrix23* A, br_matrix23* B);

void BrMatrix23ApplyP(br_vector2* A, br_vector2* B, br_matrix23* C);

void BrMatrix23ApplyV(br_vector2* A, br_vector2* B, br_matrix23* C);

void BrMatrix23TApplyP(br_vector2* A, br_vector2* B, br_matrix23* C);

void BrMatrix23TApplyV(br_vector2* A, br_vector2* B, br_matrix23* C);

void BrMatrix23Pre(br_matrix23* mat, br_matrix23* A);

void BrMatrix23Post(br_matrix23* mat, br_matrix23* A);

void BrMatrix23PreRotate(br_matrix23* mat, br_angle rz);

void BrMatrix23PostRotate(br_matrix23* mat, br_angle rz);

void BrMatrix23PreTranslate(br_matrix23* mat, br_scalar x, br_scalar y);

void BrMatrix23PostTranslate(br_matrix23* A, br_scalar x, br_scalar y);

void BrMatrix23PreScale(br_matrix23* mat, br_scalar sx, br_scalar sy);

void BrMatrix23PostScale(br_matrix23* mat, br_scalar sx, br_scalar sy);

void BrMatrix23PreShearX(br_matrix23* mat, br_scalar sy);

void BrMatrix23PostShearX(br_matrix23* mat, br_scalar sy);

void BrMatrix23PreShearY(br_matrix23* mat, br_scalar sx);

void BrMatrix23PostShearY(br_matrix23* mat, br_scalar sx);

#endif
