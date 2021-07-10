#include "plane.h"
#include "brmath.h"
#include "harness_trace.h"
#include <math.h>

char rscid[49];

// IDA: br_int_32 __cdecl BrPlaneEquation(br_vector4 *eqn, br_vector3 *v0, br_vector3 *v1, br_vector3 *v2)
br_int_32 BrPlaneEquation(br_vector4* eqn, br_vector3* v0, br_vector3* v1, br_vector3* v2) {
    br_vector3 a;
    br_vector3 b;
    float ax;
    float ay;
    float az;
    float bx;
    float by;
    float bz;
    float nx;
    float ny;
    float nz;
    float d;
    float l;

    ay = v1->v[1] - v0->v[1];
    bz = v2->v[2] - v0->v[2];
    az = v1->v[2] - v0->v[2];
    bx = v2->v[0] - v0->v[0];
    ax = v1->v[0] - v0->v[0];
    by = v2->v[1] - v0->v[1];
    nx = ay * bz - az * by;
    ny = az * bx - ax * bz;
    nz = ax * by - ay * bx;
    l = nx * nx + ny * ny + nz * nz;
    l = BrFloatSqrt(l);
    if (isnan(l)) {
        nx = 0.0;
        ny = 0.0;
        nz = 1.0;
    } else {
        nx = nx * (1.0f / l);
        ny = ny * (1.0f / l);
        nz = nz * (1.0f / l);
    }
    d = nx * v0->v[0] + ny * v0->v[1] + nz * v0->v[2];
    eqn->v[0] = nx;
    eqn->v[1] = ny;
    eqn->v[2] = nz;
    eqn->v[3] = d;
    return !isnan(l);
}
