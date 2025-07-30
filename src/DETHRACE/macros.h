#ifndef MACROS_H
#define MACROS_H

#define DR_JOIN2(A,B) A##B
#define DR_JOIN(A,B) DR_JOIN2(A, B)
#define DR_STATIC_ASSERT(V) typedef int DR_JOIN(dr_static_assert_, __COUNTER__)[(V)?1:-1]

#define VEHICLE_TYPE_FROM_ID(id) ((tVehicle_type)(id >> 8))
#define VEHICLE_INDEX_FROM_ID(id) ((id)&0x00ff)

// #define VEC3_TRANSLATE(mat) (*(br_vector3*)(&mat->m[3][0]))

#define SLOBYTE(x) (*((signed char*)&(x)))

#define STR_STARTS_WITH(haystack, needle) strncmp(haystack, needle, strlen(needle))
#define STR_ENDS_WITH(haystack, needle) strcmp(haystack + strlen(haystack) - strlen(needle), needle)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CONSTRAIN_BETWEEN(lowerbound, upperbound, val) (MIN((upperbound), MAX((lowerbound), (val))))

#define COUNT_OF(array) (sizeof((array)) / sizeof((array)[0]))
#define LEN(array) (sizeof((array)) / sizeof((array)[0]))

#define DEG_TO_RAD(degrees) ((degrees)*3.141592653589793 / 180.0)

#define V11MODEL(model) (((struct v11model*)model->prepared))
#define CAR(c) ((tCar_spec*)c)

#define Vector3Div(v1, v2, v3)                \
    do {                                      \
        (v1)->v[0] = (v2)->v[0] / (v3)->v[0]; \
        (v1)->v[1] = (v2)->v[1] / (v3)->v[1]; \
        (v1)->v[2] = (v2)->v[2] / (v3)->v[2]; \
    } while (0)

#define Vector3DistanceSquared(V1, V2) \
    ((((V1)->v[0] - (V2)->v[0])) * (((V1)->v[0] - (V2)->v[0])) + (((V1)->v[1] - (V2)->v[1])) * (((V1)->v[1] - (V2)->v[1])) + (((V1)->v[2] - (V2)->v[2])) * (((V1)->v[2] - (V2)->v[2])))

#define Vector3Distance(V1, V2) sqrt(Vector3DistanceSquared((V1), (V2)))
#define Vector3AreEqual(V1, V2) \
    ((V1)->v[0] == (V2)->v[0] && (V1)->v[1] == (V2)->v[1] && (V1)->v[2] == (V2)->v[2])
#define Vector3EqualElements(V, A, B, C) \
    ((V)->v[0] == (A) && (V)->v[1] == (B) && (V)->v[2] == (C))
#define Vector3IsZero(V) Vector3EqualElements((V), 0.f, 0.f, 0.f)
#define Vector3AddFloats(V1, V2, X, Y, Z) \
    do {                                  \
        (V1)->v[0] = (V2)->v[0] + (X);    \
        (V1)->v[1] = (V2)->v[1] + (Y);    \
        (V1)->v[2] = (V2)->v[2] + (Z);    \
    } while (0)
#define SwapValuesUsingTemporary(V1, V2, T) \
    do {                                    \
        (T) = (V1);                         \
        (V1) = (V2);                        \
        (V2) = (T);                         \
    } while (0)

#endif
