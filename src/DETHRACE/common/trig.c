#include "trig.h"
#include <stdlib.h>

br_matrix23 mat23tmp1;
br_matrix23 mat23tmp2;
br_matrix34 mattmp1;
br_matrix34 mattmp2;
br_fixed_ls gFixed_sine_table[91];
float gFloat_sine_table[91];

// IDA: float __usercall FastFloatSin@<ST0>(int pAngle_in_degrees@<EAX>)
float FastFloatSin(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);
    NOT_IMPLEMENTED();
}

// IDA: float __usercall FastFloatCos@<ST0>(int pAngle_in_degrees@<EAX>)
float FastFloatCos(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);
    NOT_IMPLEMENTED();
}

// IDA: float __usercall FastFloatTan@<ST0>(int pAngle_in_degrees@<EAX>)
float FastFloatTan(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FastScalarSin@<ST0>(int pAngle_in_degrees@<EAX>)
br_scalar FastScalarSin(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FastScalarCos@<ST0>(int pAngle_in_degrees@<EAX>)
br_scalar FastScalarCos(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FastScalarTan@<ST0>(int pAngle_in_degrees@<EAX>)
br_scalar FastScalarTan(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FastScalarSinAngle@<ST0>(br_angle pBR_angle@<EAX>)
br_scalar FastScalarSinAngle(br_angle pBR_angle) {
    LOG_TRACE("(%d)", pBR_angle);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FastScalarCosAngle@<ST0>(br_angle pBR_angle@<EAX>)
br_scalar FastScalarCosAngle(br_angle pBR_angle) {
    LOG_TRACE("(%d)", pBR_angle);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FastScalarTanAngle@<ST0>(br_angle pBR_angle@<EAX>)
br_scalar FastScalarTanAngle(br_angle pBR_angle) {
    int angle_in_degrees;
    LOG_TRACE("(%d)", pBR_angle);
    NOT_IMPLEMENTED();
}

// IDA: float __cdecl FastFloatArcSin(float pValue)
float FastFloatArcSin(float pValue) {
    float low_limit;
    float high_limit;
    float mid_point;
    LOG_TRACE("(%f)", pValue);
    NOT_IMPLEMENTED();
}

// IDA: float __cdecl FastFloatArcCos(float pValue)
float FastFloatArcCos(float pValue) {
    LOG_TRACE("(%f)", pValue);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl FastScalarArcSin(br_scalar pValue)
br_scalar FastScalarArcSin(br_scalar pValue) {
    LOG_TRACE("(%f)", pValue);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl FastScalarArcCos(br_scalar pValue)
br_scalar FastScalarArcCos(br_scalar pValue) {
    LOG_TRACE("(%f)", pValue);
    NOT_IMPLEMENTED();
}

// IDA: float __cdecl FastFloatArcTan2(float pY, float pX)
float FastFloatArcTan2(float pY, float pX) {
    float abs_x;
    float abs_y;
    LOG_TRACE("(%f, %f)", pY, pX);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl FastScalarArcTan2(br_scalar pY, br_scalar pX)
br_scalar FastScalarArcTan2(br_scalar pY, br_scalar pX) {
    br_scalar abs_x;
    br_scalar abs_y;
    LOG_TRACE("(%f, %f)", pY, pX);
    NOT_IMPLEMENTED();
}

// IDA: br_angle __cdecl FastFloatArcTan2Angle(float pY, float pX)
br_angle FastFloatArcTan2Angle(float pY, float pX) {
    float abs_x;
    float abs_y;
    LOG_TRACE("(%f, %f)", pY, pX);
    NOT_IMPLEMENTED();
}

// IDA: br_angle __cdecl FastScalarArcTan2Angle(br_scalar pY, br_scalar pX)
br_angle FastScalarArcTan2Angle(br_scalar pY, br_scalar pX) {
    br_scalar abs_x;
    br_scalar abs_y;
    LOG_TRACE("(%f, %f)", pY, pX);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34RotateX(br_matrix34 *mat@<EAX>, br_angle rx@<EDX>)
void DRMatrix34RotateX(br_matrix34* mat, br_angle rx) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rx);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34RotateY(br_matrix34 *mat@<EAX>, br_angle ry@<EDX>)
void DRMatrix34RotateY(br_matrix34* mat, br_angle ry) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, ry);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34RotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34RotateZ(br_matrix34* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34Rotate(br_matrix34 *mat@<EAX>, br_angle r@<EDX>, br_vector3 *a@<EBX>)
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
    LOG_TRACE("(%p, %d, %p)", mat, r, a);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PreRotateX(br_matrix34 *mat@<EAX>, br_angle rx@<EDX>)
void DRMatrix34PreRotateX(br_matrix34* mat, br_angle rx) {
    LOG_TRACE("(%p, %d)", mat, rx);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PostRotateX(br_matrix34 *mat@<EAX>, br_angle rx@<EDX>)
void DRMatrix34PostRotateX(br_matrix34* mat, br_angle rx) {
    LOG_TRACE("(%p, %d)", mat, rx);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PreRotateY(br_matrix34 *mat@<EAX>, br_angle ry@<EDX>)
void DRMatrix34PreRotateY(br_matrix34* mat, br_angle ry) {
    LOG_TRACE("(%p, %d)", mat, ry);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PostRotateY(br_matrix34 *mat@<EAX>, br_angle ry@<EDX>)
void DRMatrix34PostRotateY(br_matrix34* mat, br_angle ry) {
    LOG_TRACE("(%p, %d)", mat, ry);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PreRotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34PreRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PostRotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34PostRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PreRotate(br_matrix34 *mat@<EAX>, br_angle r@<EDX>, br_vector3 *axis@<EBX>)
void DRMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    LOG_TRACE("(%p, %d, %p)", mat, r, axis);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PostRotate(br_matrix34 *mat@<EAX>, br_angle r@<EDX>, br_vector3 *axis@<EBX>)
void DRMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    LOG_TRACE("(%p, %d, %p)", mat, r, axis);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix23Rotate(br_matrix23 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix23Rotate(br_matrix23* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix23PreRotate(br_matrix23 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix23PreRotate(br_matrix23* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix23PostRotate(br_matrix23 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix23PostRotate(br_matrix23* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}
