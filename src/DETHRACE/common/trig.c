#include "trig.h"
#include "brender.h"
#include <stdlib.h>

br_matrix23 mat23tmp1;
br_matrix23 mat23tmp2;
br_matrix34 mattmp1;
br_matrix34 mattmp2;
br_fixed_ls gFixed_sine_table[91];
float gFloat_sine_table[91] = {
    0.0,
    0.017452,
    0.034899,
    0.052336,
    0.069756001,
    0.087155998,
    0.104528,
    0.121869,
    0.139173,
    0.156434,
    0.173648,
    0.190809,
    0.207912,
    0.224951,
    0.24192201,
    0.25881901,
    0.275637,
    0.29237199,
    0.309017,
    0.32556799,
    0.34202,
    0.35836801,
    0.374607,
    0.39073101,
    0.406737,
    0.422618,
    0.438371,
    0.45399001,
    0.46947199,
    0.48480999,
    0.5,
    0.51503801,
    0.52991903,
    0.54463899,
    0.55919302,
    0.57357597,
    0.58778501,
    0.60181499,
    0.61566103,
    0.62932003,
    0.64278799,
    0.65605903,
    0.66913098,
    0.68199801,
    0.69465798,
    0.70710701,
    0.71934003,
    0.731354,
    0.74314499,
    0.75471002,
    0.76604402,
    0.77714598,
    0.78801101,
    0.79863602,
    0.809017,
    0.819152,
    0.82903802,
    0.83867103,
    0.84804797,
    0.85716701,
    0.86602497,
    0.87462002,
    0.88294798,
    0.89100701,
    0.898794,
    0.906308,
    0.91354501,
    0.92050499,
    0.92718399,
    0.93357998,
    0.93969297,
    0.94551897,
    0.95105702,
    0.95630503,
    0.96126199,
    0.96592599,
    0.97029603,
    0.97437,
    0.97814798,
    0.98162699,
    0.98480803,
    0.987688,
    0.99026799,
    0.99254602,
    0.99452198,
    0.99619502,
    0.99756402,
    0.99862999,
    0.99939102,
    0.99984801,
    1.0
};

// IDA: float __usercall FastFloatSin@<ST0>(int pAngle_in_degrees@<EAX>)
float FastFloatSin(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);

    if (pAngle_in_degrees >= 0) {
        pAngle_in_degrees = pAngle_in_degrees % 360;
    } else {
        pAngle_in_degrees = 360 - -pAngle_in_degrees % 360;
    }
    if (pAngle_in_degrees > 270) {
        return -gFloat_sine_table[360 - pAngle_in_degrees];
    }
    if (pAngle_in_degrees > 180) {
        return -gFloat_sine_table[pAngle_in_degrees - 180];
    }
    if (pAngle_in_degrees <= 90) {
        return gFloat_sine_table[pAngle_in_degrees];
    }
    return gFloat_sine_table[180 - pAngle_in_degrees];
}

// IDA: float __usercall FastFloatCos@<ST0>(int pAngle_in_degrees@<EAX>)
float FastFloatCos(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);

    if (pAngle_in_degrees >= 0) {
        pAngle_in_degrees = pAngle_in_degrees % 360;
    } else {
        pAngle_in_degrees = 360 - -pAngle_in_degrees % 360;
    }

    if (pAngle_in_degrees > 270) {
        return gFloat_sine_table[pAngle_in_degrees - 270];
    }
    if (pAngle_in_degrees > 180) {
        return -gFloat_sine_table[270 - pAngle_in_degrees];
    }
    if (pAngle_in_degrees <= 90) {
        return gFloat_sine_table[90 - pAngle_in_degrees];
    }
    return -gFloat_sine_table[pAngle_in_degrees - 90];
}

// IDA: float __usercall FastFloatTan@<ST0>(int pAngle_in_degrees@<EAX>)
float FastFloatTan(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FastScalarSin@<ST0>(int pAngle_in_degrees@<EAX>)
br_scalar FastScalarSin(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);

    return FastFloatSin(pAngle_in_degrees);
}

// IDA: br_scalar __usercall FastScalarCos@<ST0>(int pAngle_in_degrees@<EAX>)
br_scalar FastScalarCos(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);

    return FastFloatCos(pAngle_in_degrees);
}

// IDA: br_scalar __usercall FastScalarTan@<ST0>(int pAngle_in_degrees@<EAX>)
br_scalar FastScalarTan(int pAngle_in_degrees) {
    LOG_TRACE("(%d)", pAngle_in_degrees);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FastScalarSinAngle@<ST0>(br_angle pBR_angle@<EAX>)
br_scalar FastScalarSinAngle(br_angle pBR_angle) {
    LOG_TRACE("(%d)", pBR_angle);

    return FastScalarSin(pBR_angle * 0.0054931640625);
}

// IDA: br_scalar __usercall FastScalarCosAngle@<ST0>(br_angle pBR_angle@<EAX>)
br_scalar FastScalarCosAngle(br_angle pBR_angle) {
    LOG_TRACE("(%d)", pBR_angle);

    return FastScalarCos(pBR_angle * 0.0054931640625);
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

    abs_x = fabs(pX);
    abs_y = fabs(pY);
    if (pX == 0.0) {
        if (pY >= 0.0) {
            if (pY <= 0.0) {
                return 0.0;
            } else {
                return 90.0;
            }
        } else {
            return 270.0;
        }
    } else if (pX >= 0.0) {
        if (pY >= 0.0) {
            if (abs_y <= (double)abs_x) {
                return abs_y / abs_x * 45.0;
            } else {
                return (2.0 - abs_x / abs_y) * 45.0;
            }
        } else if (abs_y <= (double)abs_x) {
            return (8.0 - abs_y / abs_x) * 45.0;
        } else {
            return (abs_x / abs_y + 6.0) * 45.0;
        }
    } else if (pY >= 0.0) {
        if (abs_y <= (double)abs_x) {
            return (4.0 - abs_y / abs_x) * 45.0;
        } else {
            return (abs_x / abs_y + 2.0) * 45.0;
        }
    } else if (abs_y <= (double)abs_x) {
        return (abs_y / abs_x + 4.0) * 45.0;
    } else {
        return (6.0 - abs_x / abs_y) * 45.0;
    }
}

// IDA: br_scalar __cdecl FastScalarArcTan2(br_scalar pY, br_scalar pX)
br_scalar FastScalarArcTan2(br_scalar pY, br_scalar pX) {
    br_scalar abs_x;
    br_scalar abs_y;
    LOG_TRACE("(%f, %f)", pY, pX);

    return FastFloatArcTan2(pY, pX);
}

// IDA: br_angle __cdecl FastFloatArcTan2Angle(float pY, float pX)
br_angle FastFloatArcTan2Angle(float pY, float pX) {
    float abs_x;
    float abs_y;
    LOG_TRACE("(%f, %f)", pY, pX);

    abs_x = fabs(pX);
    abs_y = fabs(pY);
    if (pX == 0.0) {
        if (pY >= 0.0) {
            if (pY <= 0.0) {
                return 0;
            } else {
                return 16380;
            }
        } else {
            return -16396;
        }
    } else if (pX >= 0.0) {
        if (pY >= 0.0) {
            if (abs_y <= (double)abs_x) {
                return (abs_y / abs_x * 8192.0);
            } else {
                return ((2.0 - abs_x / abs_y) * 8192.0);
            }
        } else if (abs_y <= abs_x) {
            return ((8.0 - abs_y / abs_x) * 8192.0);
        } else {
            return ((abs_x / abs_y + 6.0) * 8192.0);
        }
    } else if (pY >= 0.0) {
        if (abs_y <= abs_x) {
            return ((4.0 - abs_y / abs_x) * 8192.0);
        } else {
            return ((abs_x / abs_y + 2.0) * 8192.0);
        }
    } else if (abs_y <= abs_x) {
        return ((abs_y / abs_x + 4.0) * 8192.0);
    } else {
        return ((6.0 - abs_x / abs_y) * 8192.0);
    }
}

// IDA: br_angle __cdecl FastScalarArcTan2Angle(br_scalar pY, br_scalar pX)
br_angle FastScalarArcTan2Angle(br_scalar pY, br_scalar pX) {
    br_scalar abs_x;
    br_scalar abs_y;
    LOG_TRACE("(%f, %f)", pY, pX);

    return FastFloatArcTan2Angle(pY, pX);
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

    s = FastScalarSinAngle(ry);
    c = FastScalarCosAngle(ry);
    mat->m[0][1] = 0.0;
    mat->m[1][0] = 0.0;
    mat->m[1][1] = 1.0;
    mat->m[1][2] = 0.0;
    mat->m[2][1] = 0.0;
    mat->m[3][0] = 0.0;
    mat->m[3][1] = 0.0;
    mat->m[3][2] = 0.0;
    mat->m[0][0] = c;
    mat->m[2][0] = s;
    mat->m[0][2] = -s;
    mat->m[2][2] = c;
}

// IDA: void __usercall DRMatrix34RotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34RotateZ(br_matrix34* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rz);

    s = FastScalarSinAngle(rz);
    c = FastScalarCosAngle(rz);
    mat->m[0][2] = 0.0;
    mat->m[1][2] = 0.0;
    mat->m[2][0] = 0.0;
    mat->m[2][1] = 0.0;
    mat->m[2][2] = 1.0;
    mat->m[3][0] = 0.0;
    mat->m[3][1] = 0.0;
    mat->m[3][2] = 0.0;
    mat->m[0][0] = c;
    mat->m[0][1] = s;
    mat->m[1][0] = -s;
    mat->m[1][1] = c;
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

    DRMatrix34RotateY(&mattmp2, ry);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
}

// IDA: void __usercall DRMatrix34PreRotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34PreRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRMatrix34PostRotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34PostRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    DRMatrix34RotateZ(&mattmp2, rz);
    BrMatrix34Mul(&mattmp1, mat, &mattmp2);
    BrMatrix34Copy(mat, &mattmp1);
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
