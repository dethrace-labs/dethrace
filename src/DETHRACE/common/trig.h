#ifndef _TRIG_H_
#define _TRIG_H_

#include "br_types.h"
#include "dr_types.h"

extern float gFloat_sine_table[91];
extern br_fixed_ls gFixed_sine_table[91];
extern br_matrix23 mat23tmp1;
extern br_matrix23 mat23tmp2;
extern br_matrix34 mattmp1__trig; // suffix added to avoid duplicate symbol
extern br_matrix34 mattmp2__trig; // suffix added to avoid duplicate symbol

float FastFloatSin(int pAngle_in_degrees);

float FastFloatCos(int pAngle_in_degrees);

float FastFloatTan(int pAngle_in_degrees);

br_scalar FastScalarSin(int pAngle_in_degrees);

br_scalar FastScalarCos(int pAngle_in_degrees);

br_scalar FastScalarTan(int pAngle_in_degrees);

br_scalar FastScalarSinAngle(br_angle pBR_angle);

br_scalar FastScalarCosAngle(br_angle pBR_angle);

br_scalar FastScalarTanAngle(br_angle pBR_angle);

float FastFloatArcSin(float pValue);

float FastFloatArcCos(float pValue);

br_scalar FastScalarArcSin(br_scalar pValue);

br_scalar FastScalarArcCos(br_scalar pValue);

float FastFloatArcTan2(float pY, float pX);

br_scalar FastScalarArcTan2(br_scalar pY, br_scalar pX);

br_angle FastFloatArcTan2Angle(float pY, float pX);

br_angle FastScalarArcTan2Angle(br_scalar pY, br_scalar pX);

void DRMatrix34RotateX(br_matrix34* mat, br_angle rx);

void DRMatrix34RotateY(br_matrix34* mat, br_angle ry);

void DRMatrix34RotateZ(br_matrix34* mat, br_angle rz);

void DRMatrix34Rotate(br_matrix34* mat, br_angle r, br_vector3* a);

void DRMatrix34PreRotateX(br_matrix34* mat, br_angle rx);

void DRMatrix34PostRotateX(br_matrix34* mat, br_angle rx);

void DRMatrix34PreRotateY(br_matrix34* mat, br_angle ry);

void DRMatrix34PostRotateY(br_matrix34* mat, br_angle ry);

void DRMatrix34PreRotateZ(br_matrix34* mat, br_angle rz);

void DRMatrix34PostRotateZ(br_matrix34* mat, br_angle rz);

void DRMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis);

void DRMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis);

void DRMatrix23Rotate(br_matrix23* mat, br_angle rz);

void DRMatrix23PreRotate(br_matrix23* mat, br_angle rz);

void DRMatrix23PostRotate(br_matrix23* mat, br_angle rz);

#endif
