#ifndef BR_DEFS_H
#define BR_DEFS_H

// Inspired by BRender SDK includes  (https://rr2000.cwaboard.co.uk/R4/BRENDER/BRENDER.zip)

#define PI 3.14159265358979323846

#define BR_MAC3(a, b, c, d, e, f) ((a) * (b) + (c) * (d) + (e) * (f))
#define BR_SQR3(a, b, c) ((a) * (a) + (b) * (b) + (c) * (c))

#define BrAngleToRadian(a) ((br_scalar)((a) * (PI / 32768.0))) // a * 0.00009587379924285257
#define BrRadianToAngle(r) ((br_angle)(long)((r) * (32768.0 / PI))) // r * 10430.378350470453

#define BR_VECTOR3(a, b, c) \
    { a, b, c }

#define BR_VECTOR4(a, b, c, d) \
    { a, b, c, d }

#define BR_COLOUR_RGB(r, g, b) \
    ((((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))

#define BR_ANGLE_DEG(deg) ((br_angle)((deg)*182))
#define BR_ANGLE_RAD(rad) ((br_angle)((rad)*10430))
#define BrDegreeToRadian(d) ((br_scalar)((d) * (PI / 180.0)))
#define BrDegreeToAngle(d) ((br_angle)(long)((d) * (32768.0 / 180.0))) // "d * 182.044444444"

#define BR_SCALAR(x) ((br_scalar)(x))

#define BR_COLOUR_RGBA(r, g, b, a) \
    ((((unsigned int)(a)) << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))

#define BR_LENGTH2(a, b) ((br_scalar)sqrt((a) * (a) + (b) * (b)))
#define BR_LENGTH3(a, b, c) ((br_scalar)sqrt((a) * (a) + (b) * (b) + (c) * (c)))
#define BR_SCALAR_EPSILON 1.192092896e-7f

#define BR_SCALAR_MAX 3.402823466e+38f
#define BR_SCALAR_MIN (-3.402823466e+38f)

#define BR_SIMPLEHEAD(l) (void*)(((br_simple_list*)(l))->head)
#define BR_SIMPLENEXT(n) (void*)(((br_simple_node*)(n))->next)
#define BR_FOR_SIMPLELIST(list, ptr) for ((ptr) = BR_SIMPLEHEAD(list); (ptr); (ptr) = BR_SIMPLENEXT(ptr))
#define BR_SIMPLEREMOVE(n) ((void*)BrSimpleRemove((br_simple_node*)(n)))

#define V_X 0
#define V_Y 1
#define V_Z 2
#define V_W 3

#define BR_FONTF_PROPORTIONAL 1

#define BR_SIN(a) ((br_scalar)sin(BrAngleToRadian(a)))
#define BR_COS(a) ((br_scalar)cos(BrAngleToRadian(a)))
#define BR_TAN(a) ((br_scalar)tan(BrAngleToRadian(a)))
#define BR_ASIN(a) BrRadianToAngle(asin(a))
#define BR_ACOS(a) BrRadianToAngle(acos(a))
#define BR_ATAN2(a, b) BrRadianToAngle(atan2((a), (b)))
#define BR_ATAN2FAST(a, b) BrRadianToAngle(atan2((a), (b)))

#define BR_ASIZE(a) (sizeof(a) / sizeof((a)[0]))

#endif /* BR_DEFS_H */