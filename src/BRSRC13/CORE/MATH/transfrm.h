#ifndef _TRANSFRM_H_
#define _TRANSFRM_H_

#include "brender/br_types.h"

extern br_uint_8 _CombineTransforms[7][7];
#define BrTransformCombineTypes(a, b) (_CombineTransforms[(a)][(b)])
#define BrTransformTypeIsLP(a) ((a) != BR_TRANSFORM_MATRIX34)
#define BrTransformTypeIsMatrix34(a) ((a) <= BR_TRANSFORM_MATRIX34_LP)

void BrTransformToMatrix34(br_matrix34* mat, br_transform* xform);

void BrMatrix34PreTransform(br_matrix34* mat, br_transform* xform);

void BrMatrix34PostTransform(br_matrix34* mat, br_transform* xform);

void BrMatrix4PreTransform(br_matrix4* mat, br_transform* xform);

void BrMatrix34ToTransform(br_transform* xform, br_matrix34* mat);

void BrTransformToTransform(br_transform* dest, br_transform* src);

#endif
