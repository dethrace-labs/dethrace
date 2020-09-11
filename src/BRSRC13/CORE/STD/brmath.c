#include "brmath.h"
#include "harness.h"
#include <math.h>

char rscid[46];

float BrFloatFloor(float f) {
    return floorf(f);
}

float BrFloatCeil(float f) {
    return ceilf(f);
}

float BrFloatSqrt(float f) {
    return sqrtf(f);
}

float BrFloatPow(float a, float b) {
    return powf(a, b);
}

float BrFloatAtan2(float x, float y) {
    return atan2f(x, y);
}
