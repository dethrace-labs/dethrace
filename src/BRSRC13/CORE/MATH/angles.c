#include "angles.h"
#include "harness.h"

char rscid[48];
order_info OrderAxes[32];

br_matrix34* BrEulerToMatrix34(br_matrix34* mat, br_euler* euler) {
    br_uint_8 o;
    br_angle ti;
    br_angle tj;
    br_angle th;
    br_scalar ci;
    br_scalar cj;
    br_scalar ch;
    br_scalar si;
    br_scalar sj;
    br_scalar sh;
    br_scalar cc;
    br_scalar cs;
    br_scalar sc;
    br_scalar ss;
    int a0;
    int a1;
    int a2;
    NOT_IMPLEMENTED();
}

br_euler* BrMatrix34ToEuler(br_euler* euler, br_matrix34* mat) {
    br_uint_8 o;
    int a0;
    int a1;
    int a2;
    NOT_IMPLEMENTED();
}

br_matrix4* BrEulerToMatrix4(br_matrix4* mat, br_euler* euler) {
    br_matrix34 tmp;
    NOT_IMPLEMENTED();
}

br_euler* BrMatrix4ToEuler(br_euler* dest, br_matrix4* mat) {
    br_matrix34 tmp;
    NOT_IMPLEMENTED();
}

br_quat* BrEulerToQuat(br_quat* q, br_euler* euler) {
    br_uint_8 o;
    br_angle ti;
    br_angle tj;
    br_angle th;
    br_scalar ci;
    br_scalar cj;
    br_scalar ch;
    br_scalar si;
    br_scalar sj;
    br_scalar sh;
    br_scalar cc;
    br_scalar cs;
    br_scalar sc;
    br_scalar ss;
    int a0;
    int a1;
    int a2;
    NOT_IMPLEMENTED();
}

br_euler* BrQuatToEuler(br_euler* euler, br_quat* q) {
    br_matrix34 mat;
    NOT_IMPLEMENTED();
}
