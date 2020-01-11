#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "br_types.h"

// Offset: 14
// Size: 276
void BrMatrix4Copy(br_matrix4* A, br_matrix4* B);

// Offset: 303
// Size: 1065
void BrMatrix4Mul(br_matrix4* A, br_matrix4* B, br_matrix4* C);

// Offset: 1386
// Size: 217
void BrMatrix4Identity(br_matrix4* mat);

// Offset: 1618
// Size: 214
void BrMatrix4Scale(br_matrix4* mat, br_scalar sx, br_scalar sy, br_scalar sz);

// Offset: 1849
// Size: 307
br_scalar BrMatrix4Inverse(br_matrix4* A, br_matrix4* B);

// Offset: 2169
// Size: 109
br_scalar Determinant3(br_scalar a1, br_scalar a2, br_scalar a3, br_scalar b1, br_scalar b2, br_scalar b3, br_scalar c1, br_scalar c2, br_scalar c3);

// Offset: 2299
// Size: 603
br_scalar BrMatrix4Determinant(br_matrix4* mat);

// Offset: 2919
// Size: 1722
void BrMatrix4Adjoint(br_matrix4* A, br_matrix4* B);

// Offset: 4662
// Size: 342
void BrMatrix4Perspective(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon);

// Offset: 5019
// Size: 390
void BrMatrix4Apply(br_vector4* A, br_vector4* B, br_matrix4* C);

// Offset: 5425
// Size: 366
void BrMatrix4ApplyP(br_vector4* A, br_vector3* B, br_matrix4* C);

// Offset: 5807
// Size: 334
void BrMatrix4ApplyV(br_vector4* A, br_vector3* B, br_matrix4* C);

// Offset: 6157
// Size: 390
void BrMatrix4TApply(br_vector4* A, br_vector4* B, br_matrix4* C);

// Offset: 6564
// Size: 366
void BrMatrix4TApplyP(br_vector4* A, br_vector3* B, br_matrix4* C);

// Offset: 6947
// Size: 334
void BrMatrix4TApplyV(br_vector4* A, br_vector3* B, br_matrix4* C);

// Offset: 7297
// Size: 274
void BrMatrix4Copy34(br_matrix4* A, br_matrix34* B);

// Offset: 7586
// Size: 882
void BrMatrix4Mul34(br_matrix4* A, br_matrix34* B, br_matrix4* C);

// Offset: 8483
// Size: 125
void BrMatrix4Pre34(br_matrix4* A, br_matrix34* B);

// Offset: 8624
// Size: 218
void BrMatrix4ShearZ(br_matrix4* mat, br_scalar sx, br_scalar sy);

#endif
