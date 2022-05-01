#ifndef MACROS_H
#define MACROS_H

#define VEHICLE_TYPE_FROM_ID(id) ((tVehicle_type)(id >> 8))
#define VEHICLE_INDEX_FROM_ID(id) (id & 0x00ff)

//#define VEC3_TRANSLATE(mat) (*(br_vector3*)(&mat->m[3][0]))

#define SLOBYTE(x) (*((signed char*)&(x)))

#define STR_STARTS_WITH(haystack, needle) strncmp(haystack, needle, strlen(needle))
#define STR_ENDS_WITH(haystack, needle) strcmp(haystack + strlen(haystack) - strlen(needle), needle)
#define MAX(a, b) ((a) > (b) ? a : b)
#define COUNT_OF(array) (sizeof((array)) / sizeof((array)[0]))
#define LEN(array) (sizeof((array)) / sizeof((array)[0]))

#define DEG_TO_RAD(degrees) ((degrees)*3.141592653589793 / 180.0)

#define V11MODEL(model) (((v11model*)model->prepared))
#define CAR(c) ((tCar_spec*)c)

#endif