#ifndef _MATRIX23_H_
#define _MATRIX23_H_

#include "br_types.h"

// Offset: 15
// Size: 156
void BrMatrix23Copy(br_matrix23 *A, br_matrix23 *B);

// Offset: 185
// Size: 316
void BrMatrix23Mul(br_matrix23 *A, br_matrix23 *B, br_matrix23 *C);

// Offset: 520
// Size: 117
void BrMatrix23Identity(br_matrix23 *mat);

// Offset: 654
// Size: 179
void BrMatrix23Rotate(br_matrix23 *mat, br_angle rz);

// Offset: 853
// Size: 115
void BrMatrix23Translate(br_matrix23 *mat, br_scalar dx, br_scalar dy);

// Offset: 984
// Size: 115
void BrMatrix23Scale(br_matrix23 *mat, br_scalar sx, br_scalar sy);

// Offset: 1116
// Size: 119
void BrMatrix23ShearX(br_matrix23 *mat, br_scalar sy);

// Offset: 1252
// Size: 119
void BrMatrix23ShearY(br_matrix23 *mat, br_scalar sx);

// Offset: 1389
// Size: 469
br_scalar BrMatrix23Inverse(br_matrix23 *B, br_matrix23 *A);

// Offset: 1878
// Size: 238
void BrMatrix23LPInverse(br_matrix23 *B, br_matrix23 *A);

// Offset: 2138
// Size: 246
void BrMatrix23LPNormalise(br_matrix23 *A, br_matrix23 *B);

// Offset: 2401
// Size: 232
void BrMatrix23ApplyP(br_vector2 *A, br_vector2 *B, br_matrix23 *C);

// Offset: 2650
// Size: 216
void BrMatrix23ApplyV(br_vector2 *A, br_vector2 *B, br_matrix23 *C);

// Offset: 2884
// Size: 216
void BrMatrix23TApplyP(br_vector2 *A, br_vector2 *B, br_matrix23 *C);

// Offset: 3118
// Size: 216
void BrMatrix23TApplyV(br_vector2 *A, br_vector2 *B, br_matrix23 *C);

// Offset: 3348
// Size: 132
void BrMatrix23Pre(br_matrix23 *mat, br_matrix23 *A);

// Offset: 3495
// Size: 132
void BrMatrix23Post(br_matrix23 *mat, br_matrix23 *A);

// Offset: 3647
// Size: 124
void BrMatrix23PreRotate(br_matrix23 *mat, br_angle rz);

// Offset: 3792
// Size: 124
void BrMatrix23PostRotate(br_matrix23 *mat, br_angle rz);

// Offset: 3939
// Size: 135
void BrMatrix23PreTranslate(br_matrix23 *mat, br_scalar x, br_scalar y);

// Offset: 4098
// Size: 85
void BrMatrix23PostTranslate(br_matrix23 *A, br_scalar x, br_scalar y);

// Offset: 4202
// Size: 135
void BrMatrix23PreScale(br_matrix23 *mat, br_scalar sx, br_scalar sy);

// Offset: 4357
// Size: 135
void BrMatrix23PostScale(br_matrix23 *mat, br_scalar sx, br_scalar sy);

// Offset: 4512
// Size: 126
void BrMatrix23PreShearX(br_matrix23 *mat, br_scalar sy);

// Offset: 4659
// Size: 126
void BrMatrix23PostShearX(br_matrix23 *mat, br_scalar sy);

// Offset: 4805
// Size: 126
void BrMatrix23PreShearY(br_matrix23 *mat, br_scalar sx);

// Offset: 4952
// Size: 126
void BrMatrix23PostShearY(br_matrix23 *mat, br_scalar sx);

#endif
