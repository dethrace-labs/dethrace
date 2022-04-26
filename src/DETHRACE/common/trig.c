#include "trig.h"
#include "brender/brender.h"
#include "harness/trace.h"
#include <math.h>
#include <stdlib.h>

float gFloat_sine_table[91] = {
    0.0f,
    0.017452f,
    0.034899f,
    0.052336f,
    0.069756f,
    0.087156f,
    0.104528f,
    0.121869f,
    0.139173f,
    0.156434f,
    0.173648f,
    0.190809f,
    0.207912f,
    0.224951f,
    0.241922f,
    0.258819f,
    0.275637f,
    0.292372f,
    0.309017f,
    0.325568f,
    0.342020f,
    0.358368f,
    0.374607f,
    0.390731f,
    0.406737f,
    0.422618f,
    0.438371f,
    0.453990f,
    0.469472f,
    0.484810f,
    0.5f,
    0.51503801f,
    0.52991903f,
    0.54463899f,
    0.55919302f,
    0.57357597f,
    0.58778501f,
    0.60181499f,
    0.61566103f,
    0.62932003f,
    0.64278799f,
    0.65605903f,
    0.66913098f,
    0.68199801f,
    0.69465798f,
    0.70710701f,
    0.71934003f,
    0.731354f,
    0.74314499f,
    0.75471002f,
    0.76604402f,
    0.77714598f,
    0.78801101f,
    0.79863602f,
    0.809017f,
    0.819152f,
    0.82903802f,
    0.83867103f,
    0.84804797f,
    0.85716701f,
    0.86602497f,
    0.87462002f,
    0.88294798f,
    0.89100701f,
    0.898794f,
    0.906308f,
    0.91354501f,
    0.92050499f,
    0.92718399f,
    0.93357998f,
    0.93969297f,
    0.94551897f,
    0.95105702f,
    0.95630503f,
    0.96126199f,
    0.96592599f,
    0.97029603f,
    0.97437f,
    0.97814798f,
    0.98162699f,
    0.98480803f,
    0.987688f,
    0.99026799f,
    0.99254602f,
    0.99452198f,
    0.99619502f,
    0.99756402f,
    0.99862999f,
    0.99939102f,
    0.99984801f,
    1.0f,
};
br_fixed_ls gFixed_sine_table[91];
br_matrix23 mat23tmp1;
br_matrix23 mat23tmp2;
br_matrix34 mattmp1__trig; // suffix added to avoid duplicate symbol
br_matrix34 mattmp2__trig; // suffix added to avoid duplicate symbol

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

    return FastFloatSin(pAngle_in_degrees) / FastFloatCos(pAngle_in_degrees);
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

    return FastScalarSin(pAngle_in_degrees) / FastScalarCos(pAngle_in_degrees);
}

// IDA: br_scalar __usercall FastScalarSinAngle@<ST0>(br_angle pBR_angle@<EAX>)
br_scalar FastScalarSinAngle(br_angle pBR_angle) {
    LOG_TRACE("(%d)", pBR_angle);

    return FastScalarSin(BrAngleToDegrees(pBR_angle));
}

// IDA: br_scalar __usercall FastScalarCosAngle@<ST0>(br_angle pBR_angle@<EAX>)
br_scalar FastScalarCosAngle(br_angle pBR_angle) {
    LOG_TRACE("(%d)", pBR_angle);

    return FastScalarCos(BrAngleToDegrees(pBR_angle));
}

// IDA: br_scalar __usercall FastScalarTanAngle@<ST0>(br_angle pBR_angle@<EAX>)
br_scalar FastScalarTanAngle(br_angle pBR_angle) {
    int angle_in_degrees;
    LOG_TRACE("(%d)", pBR_angle);

    angle_in_degrees = BrAngleToDegrees(pBR_angle);
    return FastScalarSin(angle_in_degrees) / FastScalarCos(angle_in_degrees);
}

// IDA: float __cdecl FastFloatArcSin(float pValue)
float FastFloatArcSin(float pValue) {
    float low_limit;
    float high_limit;
    float mid_point;
    LOG_TRACE("(%f)", pValue);
    
    if (pValue < 0.f) {
        return -FastFloatArcSin(-pValue);
    }
    high_limit = 90.f * pValue;
    low_limit = high_limit - 19.f;
    if (low_limit < 0.f) {
        low_limit = 0.f;
    }
    while (high_limit - low_limit >= 1.f) {
        mid_point = (low_limit + high_limit) / 2.f;

        if (gFloat_sine_table[(int)mid_point] > pValue) {
            high_limit = mid_point;
        } else {
            low_limit = mid_point;
        }
    }
    return low_limit;
}

// IDA: float __cdecl FastFloatArcCos(float pValue)
float FastFloatArcCos(float pValue) {
    LOG_TRACE("(%f)", pValue);

    return 90.f - FastFloatArcSin(pValue);
}

// IDA: br_scalar __cdecl FastScalarArcSin(br_scalar pValue)
br_scalar FastScalarArcSin(br_scalar pValue) {
    LOG_TRACE("(%f)", pValue);

    return FastFloatArcSin(pValue);
}

// IDA: br_scalar __cdecl FastScalarArcCos(br_scalar pValue)
br_scalar FastScalarArcCos(br_scalar pValue) {
    LOG_TRACE("(%f)", pValue);

    return 90.f - FastScalarArcSin(pValue);
}

// IDA: float __cdecl FastFloatArcTan2(float pY, float pX)
float FastFloatArcTan2(float pY, float pX) {
    float abs_x;
    float abs_y;
    LOG_TRACE("(%f, %f)", pY, pX);

    abs_x = fabsf(pX);
    abs_y = fabsf(pY);
    if (pX == 0.0f) {
        if (pY < 0.0f) {
            return 270.0;
        } else if (pY == 0.0f) {
            return 0.0f;
        } else {
            return 90.0f;
        }
    } else if (pX > 0.0f) {
        if (pY >= 0.0) {
            if (abs_y <= abs_x) {
                return abs_y / abs_x * 45.0f;
            } else {
                return (2.0f - abs_x / abs_y) * 45.0f;
            }
        } else {
            if (abs_y <= abs_x) {
                return (8.0f - abs_y / abs_x) * 45.0f;
            } else {
                return (abs_x / abs_y + 6.0f) * 45.0f;
            }
        }
    } else {
        if (pY >= 0.0f) {
            if (abs_y <= abs_x) {
                return (4.0f - abs_y / abs_x) * 45.0f;
            } else {
                return (abs_x / abs_y + 2.0f) * 45.0f;
            }
        } else {
            if (abs_y <= abs_x) {
                return (abs_y / abs_x + 4.0f) * 45.0f;
            } else {
                return (6.0f - abs_x / abs_y) * 45.0f;
            }
        }
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

    abs_x = fabsf(pX);
    abs_y = fabsf(pY);
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
                return ((2.0 - abs_x / abs_y) * 8192.0f);
            }
        } else if (abs_y <= abs_x) {
            return ((8.0 - abs_y / abs_x) * 8192.0f);
        } else {
            return ((abs_x / abs_y + 6.0) * 8192.0f);
        }
    } else if (pY >= 0.0) {
        if (abs_y <= abs_x) {
            return ((4.0 - abs_y / abs_x) * 8192.0f);
        } else {
            return ((abs_x / abs_y + 2.0) * 8192.0f);
        }
    } else if (abs_y <= abs_x) {
        return ((abs_y / abs_x + 4.0) * 8192.0f);
    } else {
        return ((6.0 - abs_x / abs_y) * 8192.0f);
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

    s = FastScalarSinAngle(rx);
    c = FastScalarCosAngle(rx);
    mat->m[0][0] = 1.0;
    mat->m[0][1] = 0.0;
    mat->m[0][2] = 0.0;
    mat->m[1][0] = 0.0;
    mat->m[1][1] = c;
    mat->m[1][2] = s;
    mat->m[2][0] = 0.0;
    mat->m[2][1] = -s;
    mat->m[2][2] = c;
    mat->m[3][0] = 0.0;
    mat->m[3][1] = 0.0;
    mat->m[3][2] = 0.0;
}

// IDA: void __usercall DRMatrix34RotateY(br_matrix34 *mat@<EAX>, br_angle ry@<EDX>)
void DRMatrix34RotateY(br_matrix34* mat, br_angle ry) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE8("(%p, %d)", mat, ry);

    s = FastScalarSinAngle(ry);
    c = FastScalarCosAngle(ry);
    mat->m[0][0] = c;
    mat->m[0][1] = 0.0;
    mat->m[0][2] = -s;
    mat->m[1][0] = 0.0;
    mat->m[1][1] = 1.0;
    mat->m[1][2] = 0.0;
    mat->m[2][0] = s;
    mat->m[2][1] = 0.0;
    mat->m[2][2] = c;
    mat->m[3][0] = 0.0;
    mat->m[3][1] = 0.0;
    mat->m[3][2] = 0.0;
}

// IDA: void __usercall DRMatrix34RotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34RotateZ(br_matrix34* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE8("(%p, %d)", mat, rz);

    s = FastScalarSinAngle(rz);
    c = FastScalarCosAngle(rz);
    mat->m[0][0] = c;
    mat->m[0][1] = s;
    mat->m[0][2] = 0.0;
    mat->m[1][0] = -s;
    mat->m[1][1] = c;
    mat->m[1][2] = 0.0;
    mat->m[2][0] = 0.0;
    mat->m[2][1] = 0.0;
    mat->m[2][2] = 1.0;
    mat->m[3][0] = 0.0;
    mat->m[3][1] = 0.0;
    mat->m[3][2] = 0.0;
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

    s = FastScalarSinAngle(r);
    c = FastScalarCosAngle(r);
    t = 1.f - c;
    txy = t * a->v[0] * a->v[1];
    txz = t * a->v[0] * a->v[2];
    tyz = t * a->v[1] * a->v[2];
    sx = s * a->v[0];
    sy = s * a->v[1];
    sz = s * a->v[2];

    mat->m[0][0] = a->v[0] * a->v[0] * t + c;
    mat->m[0][1] = sx + txy;
    mat->m[0][2] = txz - sy;
    mat->m[1][0] = txy - sz;
    mat->m[1][1] = a->v[1] * a->v[1] * t + c;
    mat->m[1][2] = sx + tyz;
    mat->m[2][0] = sy + txz;
    mat->m[2][1] = tyz - sx;
    mat->m[2][2] = a->v[2] * a->v[2] * t + c;
    mat->m[3][2] = 0.f;
    mat->m[3][1] = 0.f;
    mat->m[3][0] = 0.f;
}

// IDA: void __usercall DRMatrix34PreRotateX(br_matrix34 *mat@<EAX>, br_angle rx@<EDX>)
void DRMatrix34PreRotateX(br_matrix34* mat, br_angle rx) {
    LOG_TRACE("(%p, %d)", mat, rx);

    DRMatrix34RotateX(&mattmp2__trig, rx);
    BrMatrix34Mul(&mattmp1__trig, &mattmp2__trig, mat);
    BrMatrix34Copy(mat, &mattmp1__trig);
}

// IDA: void __usercall DRMatrix34PostRotateX(br_matrix34 *mat@<EAX>, br_angle rx@<EDX>)
void DRMatrix34PostRotateX(br_matrix34* mat, br_angle rx) {
    LOG_TRACE("(%p, %d)", mat, rx);

    DRMatrix34RotateX(&mattmp2__trig, rx);
    BrMatrix34Mul(&mattmp1__trig, mat, &mattmp2__trig);
    BrMatrix34Copy(mat, &mattmp1__trig);
}

// IDA: void __usercall DRMatrix34PreRotateY(br_matrix34 *mat@<EAX>, br_angle ry@<EDX>)
void DRMatrix34PreRotateY(br_matrix34* mat, br_angle ry) {
    LOG_TRACE("(%p, %d)", mat, ry);

    DRMatrix34RotateY(&mattmp2__trig, ry);
    BrMatrix34Mul(&mattmp1__trig, &mattmp2__trig, mat);
    BrMatrix34Copy(mat, &mattmp1__trig);
}

// IDA: void __usercall DRMatrix34PostRotateY(br_matrix34 *mat@<EAX>, br_angle ry@<EDX>)
void DRMatrix34PostRotateY(br_matrix34* mat, br_angle ry) {
    LOG_TRACE("(%p, %d)", mat, ry);

    DRMatrix34RotateY(&mattmp2__trig, ry);
    BrMatrix34Mul(&mattmp1__trig, mat, &mattmp2__trig);
    BrMatrix34Copy(mat, &mattmp1__trig);
}

// IDA: void __usercall DRMatrix34PreRotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34PreRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    DRMatrix34RotateZ(&mattmp2__trig, rz);
    BrMatrix34Mul(&mattmp1__trig, &mattmp2__trig, mat);
    BrMatrix34Copy(mat, &mattmp1__trig);
}

// IDA: void __usercall DRMatrix34PostRotateZ(br_matrix34 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix34PostRotateZ(br_matrix34* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    DRMatrix34RotateZ(&mattmp2__trig, rz);
    BrMatrix34Mul(&mattmp1__trig, mat, &mattmp2__trig);
    BrMatrix34Copy(mat, &mattmp1__trig);
}

// IDA: void __usercall DRMatrix34PreRotate(br_matrix34 *mat@<EAX>, br_angle r@<EDX>, br_vector3 *axis@<EBX>)
void DRMatrix34PreRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    LOG_TRACE("(%p, %d, %p)", mat, r, axis);

    DRMatrix34Rotate(&mattmp2__trig, r, axis);
    BrMatrix34Mul(&mattmp1__trig, &mattmp2__trig, mat);
    BrMatrix34Copy(mat, &mattmp1__trig);
}

// IDA: void __usercall DRMatrix34PostRotate(br_matrix34 *mat@<EAX>, br_angle r@<EDX>, br_vector3 *axis@<EBX>)
void DRMatrix34PostRotate(br_matrix34* mat, br_angle r, br_vector3* axis) {
    LOG_TRACE("(%p, %d, %p)", mat, r, axis);

    DRMatrix34Rotate(&mattmp2__trig, r, axis);
    BrMatrix34Mul(&mattmp1__trig, mat, &mattmp2__trig);
    BrMatrix34Copy(mat, &mattmp1__trig);
}

// IDA: void __usercall DRMatrix23Rotate(br_matrix23 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix23Rotate(br_matrix23* mat, br_angle rz) {
    br_scalar s;
    br_scalar c;
    LOG_TRACE("(%p, %d)", mat, rz);

    s = FastScalarSinAngle(rz);
    c = FastScalarCosAngle(rz);

    mat->m[0][0] = c;
    mat->m[0][1] = s;
    mat->m[1][0] = -s;
    mat->m[1][1] = c;
    mat->m[2][0] = 0.f;
    mat->m[2][1] = 0.f;
}

// IDA: void __usercall DRMatrix23PreRotate(br_matrix23 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix23PreRotate(br_matrix23* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    DRMatrix23Rotate(&mat23tmp2, rz);
    BrMatrix23Mul(&mat23tmp1, &mat23tmp2, mat);
    BrMatrix23Copy(mat, &mat23tmp1);
}

// IDA: void __usercall DRMatrix23PostRotate(br_matrix23 *mat@<EAX>, br_angle rz@<EDX>)
void DRMatrix23PostRotate(br_matrix23* mat, br_angle rz) {
    LOG_TRACE("(%p, %d)", mat, rz);

    DRMatrix23Rotate(&mat23tmp2, rz);
    BrMatrix23Mul(&mat23tmp1, mat, &mat23tmp2);
    BrMatrix23Copy(mat, &mat23tmp1);
}
