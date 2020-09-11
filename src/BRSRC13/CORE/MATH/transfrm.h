#ifndef _TRANSFRM_H_
#define _TRANSFRM_H_

#include "br_types.h"

void BrTransformToMatrix34(br_matrix34* mat, br_transform* xform);

void BrMatrix34PreTransform(br_matrix34* mat, br_transform* xform);

void BrMatrix34PostTransform(br_matrix34* mat, br_transform* xform);

void BrMatrix4PreTransform(br_matrix4* mat, br_transform* xform);

void BrMatrix34ToTransform(br_transform* xform, br_matrix34* mat);

void BrTransformToTransform(br_transform* dest, br_transform* src);

#endif
