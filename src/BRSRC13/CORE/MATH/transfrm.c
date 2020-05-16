#include "transfrm.h"
#include "debug.h"

char rscid[52];
br_uint_8 _CombineTransforms[7][7];

// Offset: 22
// Size: 832
void BrTransformToMatrix34(br_matrix34* mat, br_transform* xform) {
    NOT_IMPLEMENTED();
}

// Offset: 877
// Size: 127
void BrMatrix34PreTransform(br_matrix34* mat, br_transform* xform) {
    br_matrix34 tmp;
    NOT_IMPLEMENTED();
}

// Offset: 1028
// Size: 133
void BrMatrix34PostTransform(br_matrix34* mat, br_transform* xform) {
    br_matrix34 tmp;
    NOT_IMPLEMENTED();
}

// Offset: 1183
// Size: 133
void BrMatrix4PreTransform(br_matrix4* mat, br_transform* xform) {
    br_matrix34 tmp;
    NOT_IMPLEMENTED();
}

// Offset: 1338
// Size: 560
void BrMatrix34ToTransform(br_transform* xform, br_matrix34* mat) {
    NOT_IMPLEMENTED();
}

// Offset: 1921
// Size: 214
void BrTransformToTransform(br_transform* dest, br_transform* src) {
    br_matrix34 temp;
    NOT_IMPLEMENTED();
}
