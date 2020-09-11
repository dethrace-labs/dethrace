#include "transfrm.h"
#include "harness.h"

char rscid[52];
br_uint_8 _CombineTransforms[7][7];

void BrTransformToMatrix34(br_matrix34* mat, br_transform* xform) {
    NOT_IMPLEMENTED();
}

void BrMatrix34PreTransform(br_matrix34* mat, br_transform* xform) {
    br_matrix34 tmp;
    NOT_IMPLEMENTED();
}

void BrMatrix34PostTransform(br_matrix34* mat, br_transform* xform) {
    br_matrix34 tmp;
    NOT_IMPLEMENTED();
}

void BrMatrix4PreTransform(br_matrix4* mat, br_transform* xform) {
    br_matrix34 tmp;
    NOT_IMPLEMENTED();
}

void BrMatrix34ToTransform(br_transform* xform, br_matrix34* mat) {
    NOT_IMPLEMENTED();
}

void BrTransformToTransform(br_transform* dest, br_transform* src) {
    br_matrix34 temp;
    NOT_IMPLEMENTED();
}
