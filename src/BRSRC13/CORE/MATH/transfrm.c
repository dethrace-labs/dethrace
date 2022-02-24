#include "transfrm.h"
#include "CORE/MATH/angles.h"
#include "CORE/MATH/matrix34.h"
#include "CORE/MATH/quat.h"
#include "CORE/MATH/vector.h"
#include "harness/trace.h"

#include <string.h>

#define M34 BR_TRANSFORM_MATRIX34
#define MLP BR_TRANSFORM_MATRIX34_LP
#define QUT BR_TRANSFORM_QUAT
#define EUL BR_TRANSFORM_EULER
#define LUP BR_TRANSFORM_LOOK_UP
#define TRA BR_TRANSFORM_TRANSLATION
#define IDT BR_TRANSFORM_IDENTITY

br_uint_8 _CombineTransforms[7][7] = {

    /* M34 */ {
        M34,
        M34,
        M34,
        M34,
        M34,
        M34,
        M34,
    },
    /* MLP */ {
        M34,
        MLP,
        MLP,
        MLP,
        MLP,
        MLP,
        MLP,
    },
    /* QUT */ {
        M34,
        MLP,
        MLP,
        MLP,
        MLP,
        QUT,
        QUT,
    },
    /* EUL */ {
        M34,
        MLP,
        MLP,
        MLP,
        MLP,
        EUL,
        EUL,
    },
    /* LUP */ {
        M34,
        MLP,
        MLP,
        MLP,
        MLP,
        LUP,
        LUP,
    },
    /* TRA */ {
        M34,
        MLP,
        QUT,
        EUL,
        LUP,
        TRA,
        TRA,
    },
    /* IDT */ {
        M34,
        MLP,
        QUT,
        EUL,
        LUP,
        TRA,
        IDT,
    },
};

// IDA: void __cdecl BrTransformToMatrix34(br_matrix34 *mat, br_transform *xform)
void BrTransformToMatrix34(br_matrix34* mat, br_transform* xform) {
    LOG_TRACE("(%p, %p)", mat, xform);

    switch (xform->type) {
    case BR_TRANSFORM_MATRIX34:
    case BR_TRANSFORM_MATRIX34_LP:
        *mat = xform->t.mat;
        break;

    case BR_TRANSFORM_QUAT:

        BrQuatToMatrix34(mat, &xform->t.quat.q);

        mat->m[V_W][V_X] = xform->t.quat.t.v[V_X];
        mat->m[V_W][V_Y] = xform->t.quat.t.v[V_Y];
        mat->m[V_W][V_Z] = xform->t.quat.t.v[V_Z];
        break;

    case BR_TRANSFORM_EULER:

        BrEulerToMatrix34(mat, &xform->t.euler.e);

        mat->m[V_W][V_X] = xform->t.euler.t.v[V_X];
        mat->m[V_W][V_Y] = xform->t.euler.t.v[V_Y];
        mat->m[V_W][V_Z] = xform->t.euler.t.v[V_Z];

        break;

    case BR_TRANSFORM_TRANSLATION:
        BrMatrix34Translate(mat,
            xform->t.look_up.t.v[V_X],
            xform->t.look_up.t.v[V_Y],
            xform->t.look_up.t.v[V_Z]);

        break;

    case BR_TRANSFORM_LOOK_UP: {
        /*
			 * 1) Normalise Lookat vector to get Z component of matrix
			 * 2) Cross with up vector and normalise to get X component
			 * 3) Cross X & Z to get Y
			 */
        br_vector3 vx, vy, vz;

        BrVector3Normalise(&vz, &xform->t.look_up.look);
        BrVector3Negate(&vz, &vz);
        BrVector3Cross(&vx, &xform->t.look_up.up, &vz);
        BrVector3Normalise(&vx, &vx);
        BrVector3Cross(&vy, &vz, &vx);

        mat->m[V_X][V_X] = vx.v[V_X];
        mat->m[V_X][V_Y] = vx.v[V_Y];
        mat->m[V_X][V_Z] = vx.v[V_Z];
        mat->m[V_Y][V_X] = vy.v[V_X];
        mat->m[V_Y][V_Y] = vy.v[V_Y];
        mat->m[V_Y][V_Z] = vy.v[V_Z];
        mat->m[V_Z][V_X] = vz.v[V_X];
        mat->m[V_Z][V_Y] = vz.v[V_Y];
        mat->m[V_Z][V_Z] = vz.v[V_Z];

        mat->m[V_W][V_X] = xform->t.look_up.t.v[V_X];
        mat->m[V_W][V_Y] = xform->t.look_up.t.v[V_Y];
        mat->m[V_W][V_Z] = xform->t.look_up.t.v[V_Z];
    } break;

    case BR_TRANSFORM_IDENTITY:
        BrMatrix34Identity(mat);
        break;
    }
}

// IDA: void __cdecl BrMatrix34PreTransform(br_matrix34 *mat, br_transform *xform)
void BrMatrix34PreTransform(br_matrix34* mat, br_transform* xform) {
    br_matrix34 tmp;
    LOG_TRACE("(%p, %p)", mat, xform);

    if (xform->type == BR_TRANSFORM_IDENTITY) {
        return;
    }

    BrTransformToMatrix34(&tmp, xform);
    BrMatrix34Pre(mat, &tmp);
}

// IDA: void __cdecl BrMatrix34PostTransform(br_matrix34 *mat, br_transform *xform)
void BrMatrix34PostTransform(br_matrix34* mat, br_transform* xform) {
    br_matrix34 tmp;
    LOG_TRACE("(%p, %p)", mat, xform);

    if (xform->type == BR_TRANSFORM_IDENTITY) {
        return;
    }

    BrTransformToMatrix34(&tmp, xform);
    BrMatrix34Post(mat, &tmp);
}

// IDA: void __cdecl BrMatrix4PreTransform(br_matrix4 *mat, br_transform *xform)
void BrMatrix4PreTransform(br_matrix4* mat, br_transform* xform) {
    br_matrix34 tmp;
    LOG_TRACE("(%p, %p)", mat, xform);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34ToTransform(br_transform *xform, br_matrix34 *mat)
void BrMatrix34ToTransform(br_transform* xform, br_matrix34* mat) {
    LOG_TRACE("(%p, %p)", xform, mat);

    switch (xform->type) {
    case BR_TRANSFORM_MATRIX34:
        memcpy(&xform->t.mat, mat, sizeof(br_matrix34));
        break;
    case BR_TRANSFORM_MATRIX34_LP:
        memcpy(&xform->t.mat, mat, sizeof(br_matrix34));
        BrMatrix34LPNormalise(&xform->t.mat, &xform->t.mat);
        break;
    case BR_TRANSFORM_QUAT:
        BrMatrix34ToQuat(&xform->t.quat.q, mat);
        BrVector3Copy(&xform->t.quat.t, (br_vector3*)mat->m[3]);
        break;
    case BR_TRANSFORM_EULER:
        BrMatrix34ToEuler(&xform->t.euler.e, mat);
        BrVector3Copy(&xform->t.quat.t, (br_vector3*)mat->m[3]);
        break;
    case BR_TRANSFORM_LOOK_UP:
        NOT_IMPLEMENTED();
        break;
    case BR_TRANSFORM_TRANSLATION:
        BrVector3Copy(&xform->t.quat.t, (br_vector3*)mat->m[3]);
        break;
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
}

// IDA: void __cdecl BrTransformToTransform(br_transform *dest, br_transform *src)
void BrTransformToTransform(br_transform* dest, br_transform* src) {
    br_matrix34 temp;
    LOG_TRACE("(%p, %p)", dest, src);

    if (src->type == dest->type) {
        memcpy(dest, src, sizeof(br_transform));
        return;
    }
    switch (dest->type) {
    case BR_TRANSFORM_MATRIX34:
        BrTransformToMatrix34(&dest->t.mat, src);
        break;
    case BR_TRANSFORM_MATRIX34_LP:
        BrTransformToMatrix34(&dest->t.mat, src);
        BrMatrix34LPNormalise(&dest->t.mat, &dest->t.mat);
        break;
    default:
        BrTransformToMatrix34(&temp, src);
        BrMatrix34ToTransform(dest, &temp);
    }
}
