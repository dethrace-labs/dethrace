#include "trig.h"
#include <stdlib.h>

br_matrix23 mat23tmp1;
br_matrix23 mat23tmp2;
br_matrix34 mattmp1;
br_matrix34 mattmp2;
br_fixed_ls gFixed_sine_table[91];
float gFloat_sine_table[91];

float FastFloatSin(int pAngle_in_degrees) {
    NOT_IMPLEMENTED();
}

float FastFloatCos(int pAngle_in_degrees) {
    NOT_IMPLEMENTED();
}

float FastFloatTan(int pAngle_in_degrees) {
    NOT_IMPLEMENTED();
}

br_scalar FastScalarSin(int pAngle_in_degrees) {
    NOT_IMPLEMENTED();
}

br_scalar FastScalarCos(int pAngle_in_degrees) {
    NOT_IMPLEMENTED();
}

br_scalar FastScalarTan(int pAngle_in_degrees) {
    NOT_IMPLEMENTED();
}

br_scalar FastScalarSinAngle(br_angle pBR_angle) {
    NOT_IMPLEMENTED();
}

br_scalar FastScalarCosAngle(br_angle pBR_angle) {
    NOT_IMPLEMENTED();
}

br_scalar FastScalarTanAngle(br_angle pBR_angle) {
    int angle_in_degrees;
    NOT_IMPLEMENTED();
}

float FastFloatArcSin(float pValue) {
    float low_limit;
    float high_limit;
    float mid_point;
    NOT_IMPLEMENTED();
}

float FastFloatArcCos(float pValue) {
    NOT_IMPLEMENTED();
}

br_scalar FastScalarArcSin(br_scalar pValue) {
    NOT_IMPLEMENTED();
}

br_scalar FastScalarArcCos(br_scalar pValue) {
    NOT_IMPLEMENTED();
}

float FastFloatArcTan2(float pY, float pX) {
    float abs_x;
    float abs_y;
    NOT_IMPLEMENTED();
}

br_scalar FastScalarArcTan2(br_scalar pY, br_scalar pX) {
    br_scalar abs_x;
    br_scalar abs_y;
    NOT_IMPLEMENTED();
}

br_angle FastFloatArcTan2Angle(float pY, float pX) {
    float abs_x;
    float abs_y;
    NOT_IMPLEMENTED();
}

br_angle FastScalarArcTan2Angle(br_scalar pY, br_scalar pX) {
    br_scalar abs_x;
    br_scalar abs_y;
    NOT_IMPLEMENTED();
}

void DRMatrix34RotateX(br_matrix34* mat, br_angle rx) {
    br_scalar s;
    br_scalar c;
    NOT_IMPLEMENTED();
}

void DRMatrix34RotateY(br_matrix34* mat, br_angle ry) {
    br_scalar s;
    br_scalar c;
    NOT_IMPLEMENTED();
}

void DRMatrix34RotateZ(br_matrix34* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    NOT_IMPLEMENTED();
}

void DRMatrix34Rotate(br_matrix34* mat, br_angle r, br_vector3* a) {
    br_scalar t;
    br_scalar s;
    br_scalar c;
    br_scalar txy;
    br_scalar txz;
    br_scalar tyz;
    br_scalar sx;
    br_scalar sy;
    br_scalar sz;
    NOT_IMPLEMENTED();
}

void DRMatrix34PreRotateX(br_matrix34* mat, br_angle rx) {
    NOT_IMPLEMENTED();
}

void DRMatrix34PostRotateX(br_matrix34* mat, br_angle rx) {
    NOT_IMPLEMENTED();
}

void DRMatrix34PreRotateY(br_matrix34* mat, br_angle ry) {
    NOT_IMPLEMENTED();
}

void DRMatrix34PostRotateY(br_matrix34* mat, br_angle ry) {
    NOT_IMPLEMENTED();
}

void DRMatrix34PreRotateZ(br_matrix34* mat, br_angle rz) {
    NOT_IMPLEMENTED();
}

void DRMatrix34PostRotateZ(br_matrix34* mat, br_angle rz) {
    NOT_IMPLEMENTED();
}

void DRMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    NOT_IMPLEMENTED();
}

void DRMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    NOT_IMPLEMENTED();
}

void DRMatrix23Rotate(br_matrix23* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    NOT_IMPLEMENTED();
}

void DRMatrix23PreRotate(br_matrix23* mat, br_angle rz) {
    NOT_IMPLEMENTED();
}

void DRMatrix23PostRotate(br_matrix23* mat, br_angle rz) {
    NOT_IMPLEMENTED();
}
