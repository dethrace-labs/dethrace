#ifndef BR_INLINE_FUNCS_H
#define BR_INLINE_FUNCS_H

#include "br_defs.h"

#define BrVector3Length(v1) BR_LENGTH3((v1)->v[0], (v1)->v[1], (v1)->v[2])
#define BrVector3LengthSquared(v1) BR_SQR3((v1)->v[0], (v1)->v[1], (v1)->v[2])
#define BrVector3Dot(v1, v2) BR_MAC3((v1)->v[0], (v2)->v[0], (v1)->v[1], (v2)->v[1], (v1)->v[2], (v2)->v[2])

#define BrVector3Copy(v1, v2)    \
    do {                         \
        (v1)->v[0] = (v2)->v[0]; \
        (v1)->v[1] = (v2)->v[1]; \
        (v1)->v[2] = (v2)->v[2]; \
    } while (0)

#define BrVector3Negate(v1, v2)   \
    do {                          \
        (v1)->v[0] = -(v2)->v[0]; \
        (v1)->v[1] = -(v2)->v[1]; \
        (v1)->v[2] = -(v2)->v[2]; \
    } while (0)

#define BrVector3Set(v1, s1, s2, s3) \
    do {                             \
        (v1)->v[0] = (s1);           \
        (v1)->v[1] = (s2);           \
        (v1)->v[2] = (s3);           \
    } while (0)

// v2[1] * v3[2]
#define BrVector3Cross(v1, v2, v3)                                      \
    do {                                                                \
        (v1)->v[0] = (v2)->v[1] * (v3)->v[2] - (v2)->v[2] * (v3)->v[1]; \
        (v1)->v[1] = (v2)->v[2] * (v3)->v[0] - (v2)->v[0] * (v3)->v[2]; \
        (v1)->v[2] = (v2)->v[0] * (v3)->v[1] - (v2)->v[1] * (v3)->v[0]; \
    } while (0)

#define BrVector3Add(v1, v2, v3)              \
    do {                                      \
        (v1)->v[0] = (v2)->v[0] + (v3)->v[0]; \
        (v1)->v[1] = (v2)->v[1] + (v3)->v[1]; \
        (v1)->v[2] = (v2)->v[2] + (v3)->v[2]; \
    } while (0)

#define BrVector3Sub(v1, v2, v3)              \
    do {                                      \
        (v1)->v[0] = (v2)->v[0] - (v3)->v[0]; \
        (v1)->v[1] = (v2)->v[1] - (v3)->v[1]; \
        (v1)->v[2] = (v2)->v[2] - (v3)->v[2]; \
    } while (0)

#define BrVector3Accumulate(v1, v2) \
    do {                            \
        (v1)->v[0] += (v2)->v[0];   \
        (v1)->v[1] += (v2)->v[1];   \
        (v1)->v[2] += (v2)->v[2];   \
    } while (0)

#define BrVector3Scale(v1, v2, s)    \
    do {                             \
        (v1)->v[0] = (v2)->v[0] * s; \
        (v1)->v[1] = (v2)->v[1] * s; \
        (v1)->v[2] = (v2)->v[2] * s; \
    } while (0)

#define BrVector3InvScale(v1, v2, s)       \
    do {                                   \
        br_scalar __scale = 1.0f / (s);    \
        (v1)->v[0] = (v2)->v[0] * __scale; \
        (v1)->v[1] = (v2)->v[1] * __scale; \
        (v1)->v[2] = (v2)->v[2] * __scale; \
    } while (0)

#define BrVector3Normalise(v1, v2)                               \
    do {                                                         \
        br_scalar _scale;                                        \
        _scale = BR_LENGTH3((v2)->v[0], (v2)->v[1], (v2)->v[2]); \
        if (_scale > BR_SCALAR_EPSILON * 2) {                    \
            _scale = 1.0 / _scale;                               \
            (v1)->v[0] = (v2)->v[0] * _scale;                    \
            (v1)->v[1] = (v2)->v[1] * _scale;                    \
            (v1)->v[2] = (v2)->v[2] * _scale;                    \
        } else {                                                 \
            (v1)->v[0] = 1.0;                                    \
            (v1)->v[1] = 0.0;                                    \
            (v1)->v[2] = 0.0;                                    \
        }                                                        \
    } while (0)

#define BrVector3NormaliseQuick(v1, v2)                                  \
    do {                                                                 \
        br_scalar _scale;                                                \
        _scale = 1.0 / (BR_LENGTH3((v2)->v[0], (v2)->v[1], (v2)->v[2])); \
        BrVector3Scale(v1, v2, _scale);                                  \
    } while (0)

#endif