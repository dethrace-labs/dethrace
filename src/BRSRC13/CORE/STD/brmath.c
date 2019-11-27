#include "brmath.h"
#include <math.h>

char rscid[46];

// Offset: 13
// Size: 61
float BrFloatFloor(float f) {
    return floorf(f);
}

// Offset: 86
// Size: 61
float BrFloatCeil(float f) {
    return ceilf(f);
}

// Offset: 159
// Size: 61
float BrFloatSqrt(float f) {
    return sqrtf(f);
}

// Offset: 231
// Size: 70
float BrFloatPow(float a, float b) {
    return powf(a, b);
}

// Offset: 314
// Size: 70
float BrFloatAtan2(float x, float y) {
    return atan2f(x, y);
}

