#ifndef _MATRIX34_H_
#define _MATRIX34_H_

#include "br_types.h"

void BrMatrix34Copy(br_matrix34* A, br_matrix34* B);

void BrMatrix34Mul(br_matrix34* A, br_matrix34* B, br_matrix34* C);

void BrMatrix34Identity(br_matrix34* mat);

void BrMatrix34RotateX(br_matrix34* mat, br_angle rx);

void BrMatrix34RotateY(br_matrix34* mat, br_angle ry);

void BrMatrix34RotateZ(br_matrix34* mat, br_angle rz);

void BrMatrix34Rotate(br_matrix34* mat, br_angle r, br_vector3* a);

void BrMatrix34Translate(br_matrix34* mat, br_scalar dx, br_scalar dy, br_scalar dz);

void BrMatrix34Scale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);

void BrMatrix34ShearX(br_matrix34* mat, br_scalar sy, br_scalar sz);

void BrMatrix34ShearY(br_matrix34* mat, br_scalar sx, br_scalar sz);

void BrMatrix34ShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy);

br_scalar BrMatrix34Inverse(br_matrix34* B, br_matrix34* A);

void BrMatrix34LPInverse(br_matrix34* A, br_matrix34* B);

void BrMatrix34LPNormalise(br_matrix34* A, br_matrix34* B);

void BrMatrix34RollingBall(br_matrix34* mat, int dx, int dy, int radius);

br_matrix34* BrBoundsToMatrix34(br_matrix34* mat, br_bounds* bounds);

void BrMatrix34Copy4(br_matrix34* A, br_matrix4* B);

void BrMatrix34TApplyFV(br_vector3* A, br_fvector3* B, br_matrix34* C);

void BrMatrix34Apply(br_vector3* A, br_vector4* B, br_matrix34* C);

void BrMatrix34ApplyP(br_vector3* A, br_vector3* B, br_matrix34* C);

void BrMatrix34ApplyV(br_vector3* A, br_vector3* B, br_matrix34* C);

void BrMatrix34TApply(br_vector4* A, br_vector4* B, br_matrix34* C);

void BrMatrix34TApplyP(br_vector3* A, br_vector3* B, br_matrix34* C);

void BrMatrix34TApplyV(br_vector3* A, br_vector3* B, br_matrix34* C);

void BrMatrix34Pre(br_matrix34* mat, br_matrix34* A);

void BrMatrix34Post(br_matrix34* mat, br_matrix34* A);

void BrMatrix34PreRotateX(br_matrix34* mat, br_angle rx);

void BrMatrix34PostRotateX(br_matrix34* mat, br_angle rx);

void BrMatrix34PreRotateY(br_matrix34* mat, br_angle ry);

void BrMatrix34PostRotateY(br_matrix34* mat, br_angle ry);

void BrMatrix34PreRotateZ(br_matrix34* mat, br_angle rz);

void BrMatrix34PostRotateZ(br_matrix34* mat, br_angle rz);

void BrMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis);

void BrMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis);

void BrMatrix34PreTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z);

void BrMatrix34PostTranslate(br_matrix34* mat, br_scalar x, br_scalar y, br_scalar z);

void BrMatrix34PreScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);

void BrMatrix34PostScale(br_matrix34* mat, br_scalar sx, br_scalar sy, br_scalar sz);

void BrMatrix34PreShearX(br_matrix34* mat, br_scalar sy, br_scalar sz);

void BrMatrix34PostShearX(br_matrix34* mat, br_scalar sy, br_scalar sz);

void BrMatrix34PreShearY(br_matrix34* mat, br_scalar sx, br_scalar sz);

void BrMatrix34PostShearY(br_matrix34* mat, br_scalar sx, br_scalar sz);

void BrMatrix34PreShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy);

void BrMatrix34PostShearZ(br_matrix34* mat, br_scalar sx, br_scalar sy);

#endif
