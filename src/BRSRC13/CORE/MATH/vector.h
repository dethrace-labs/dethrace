#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "br_types.h"

void BrVector2Copy(br_vector2* v1, br_vector2* v2);

void BrVector2Set(br_vector2* v1, br_scalar s1, br_scalar s2);

void BrVector2SetInt(br_vector2* v1, int i1, int i2);

void BrVector2SetFloat(br_vector2* v1, float f1, float f2);

void BrVector2Negate(br_vector2* v1, br_vector2* v2);

void BrVector2Add(br_vector2* v1, br_vector2* v2, br_vector2* v3);

void BrVector2Accumulate(br_vector2* v1, br_vector2* v2);

void BrVector2Sub(br_vector2* v1, br_vector2* v2, br_vector2* v3);

void BrVector2Scale(br_vector2* v1, br_vector2* v2, br_scalar s);

void BrVector2InvScale(br_vector2* v1, br_vector2* v2, br_scalar s);

br_scalar BrVector2Dot(br_vector2* v1, br_vector2* v2);

br_scalar BrVector2Length(br_vector2* v1);

br_scalar BrVector2LengthSquared(br_vector2* v1);

void BrVector3Copy(br_vector3* v1, br_vector3* v2);

void BrVector3Set(br_vector3* v1, br_scalar s1, br_scalar s2, br_scalar s3);

void BrVector3SetInt(br_vector3* v1, int i1, int i2, int i3);

void BrVector3SetFloat(br_vector3* v1, float f1, float f2, float f3);

void BrVector3Negate(br_vector3* v1, br_vector3* v2);

void BrVector3Add(br_vector3* v1, br_vector3* v2, br_vector3* v3);

void BrVector3Accumulate(br_vector3* v1, br_vector3* v2);

void BrVector3Sub(br_vector3* v1, br_vector3* v2, br_vector3* v3);

void BrVector3Scale(br_vector3* v1, br_vector3* v2, br_scalar s);

void BrVector3InvScale(br_vector3* v1, br_vector3* v2, br_scalar s);

br_scalar BrVector3Dot(br_vector3* v1, br_vector3* v2);

void BrVector3Cross(br_vector3* v1, br_vector3* v2, br_vector3* v3);

br_scalar BrVector3Length(br_vector3* v1);

br_scalar BrVector3LengthSquared(br_vector3* v1);

void BrVector3Normalise(br_vector3* v1, br_vector3* v2);

void BrVector3NormaliseLP(br_vector3* v1, br_vector3* v2);

br_scalar BrVector4Dot(br_vector4* v1, br_vector4* v2);

void BrVector4Copy(br_vector4* v1, br_vector4* v2);

br_scalar BrFVector2Dot(br_fvector2* v1, br_vector2* v2);

void BrFVector3Copy(br_fvector3* v1, br_vector3* v2);

void BrVector3ScaleF(br_vector3* v1, br_fvector3* v2, br_scalar s);

br_scalar BrFVector3Dot(br_fvector3* v1, br_vector3* v2);

void BrFVector3Normalise(br_fvector3* v1, br_vector3* v2);

void BrFVector3NormaliseLP(br_fvector3* v1, br_vector3* v2);

void BrVector2Normalise(br_vector2* v1, br_vector2* v2);

#endif
