#include "transfrm.h"
#include "harness.h"

char rscid[52];
br_uint_8 _CombineTransforms[7][7];

// IDA: void __cdecl BrTransformToMatrix34(br_matrix34 *mat, br_transform *xform)
void BrTransformToMatrix34(br_matrix34* mat, br_transform* xform) {
    LOG_TRACE("(%p, %p)", mat, xform);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrMatrix34PreTransform(br_matrix34 *mat, br_transform *xform)
void BrMatrix34PreTransform(br_matrix34* mat, br_transform* xform) {
    br_matrix34 tmp;
    LOG_TRACE("(%p, %p)", mat, xform);
    NOT_IMPLEMENTED();
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
