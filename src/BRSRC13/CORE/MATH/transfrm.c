#include "transfrm.h"
#include "CORE/MATH/angles.h"
#include "CORE/MATH/matrix34.h"
#include "CORE/MATH/quat.h"
#include "CORE/MATH/vector.h"
#include "harness.h"

char rscid[52];
br_uint_8 _CombineTransforms[7][7];

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

        mat->m[W][X] = xform->t.quat.t.v[X];
        mat->m[W][Y] = xform->t.quat.t.v[Y];
        mat->m[W][Z] = xform->t.quat.t.v[Z];
        break;

    case BR_TRANSFORM_EULER:

        BrEulerToMatrix34(mat, &xform->t.euler.e);

        mat->m[W][X] = xform->t.euler.t.v[X];
        mat->m[W][Y] = xform->t.euler.t.v[Y];
        mat->m[W][Z] = xform->t.euler.t.v[Z];

        break;

    case BR_TRANSFORM_TRANSLATION:
        BrMatrix34Translate(mat,
            xform->t.look_up.t.v[X],
            xform->t.look_up.t.v[Y],
            xform->t.look_up.t.v[Z]);

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

        mat->m[X][X] = vx.v[X];
        mat->m[X][Y] = vx.v[Y];
        mat->m[X][Z] = vx.v[Z];
        mat->m[Y][X] = vy.v[X];
        mat->m[Y][Y] = vy.v[Y];
        mat->m[Y][Z] = vy.v[Z];
        mat->m[Z][X] = vz.v[X];
        mat->m[Z][Y] = vz.v[Y];
        mat->m[Z][Z] = vz.v[Z];

        mat->m[W][X] = xform->t.look_up.t.v[X];
        mat->m[W][Y] = xform->t.look_up.t.v[Y];
        mat->m[W][Z] = xform->t.look_up.t.v[Z];
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrTransformToTransform(br_transform *dest, br_transform *src)
void BrTransformToTransform(br_transform* dest, br_transform* src) {
    br_matrix34 temp;
    LOG_TRACE("(%p, %p)", dest, src);
    NOT_IMPLEMENTED();
}
