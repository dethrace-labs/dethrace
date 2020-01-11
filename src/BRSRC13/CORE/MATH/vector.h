#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "br_types.h"

// Offset: 14
// Size: 108
void BrVector2Copy(br_vector2* v1, br_vector2* v2);

// Offset: 135
// Size: 75
void BrVector2Set(br_vector2* v1, br_scalar s1, br_scalar s2);

// Offset: 226
// Size: 75
void BrVector2SetInt(br_vector2* v1, int i1, int i2);

// Offset: 319
// Size: 75
void BrVector2SetFloat(br_vector2* v1, float f1, float f2);

// Offset: 410
// Size: 112
void BrVector2Negate(br_vector2* v1, br_vector2* v2);

// Offset: 535
// Size: 147
void BrVector2Add(br_vector2* v1, br_vector2* v2, br_vector2* v3);

// Offset: 702
// Size: 113
void BrVector2Accumulate(br_vector2* v1, br_vector2* v2);

// Offset: 828
// Size: 147
void BrVector2Sub(br_vector2* v1, br_vector2* v2, br_vector2* v3);

// Offset: 990
// Size: 114
void BrVector2Scale(br_vector2* v1, br_vector2* v2, br_scalar s);

// Offset: 1122
// Size: 120
void BrVector2InvScale(br_vector2* v1, br_vector2* v2, br_scalar s);

// Offset: 1255
// Size: 133
br_scalar BrVector2Dot(br_vector2* v1, br_vector2* v2);

// Offset: 1404
// Size: 107
br_scalar BrVector2Length(br_vector2* v1);

// Offset: 1534
// Size: 96
br_scalar BrVector2LengthSquared(br_vector2* v1);

// Offset: 1644
// Size: 126
void BrVector3Copy(br_vector3* v1, br_vector3* v2);

// Offset: 1783
// Size: 87
void BrVector3Set(br_vector3* v1, br_scalar s1, br_scalar s2, br_scalar s3);

// Offset: 1886
// Size: 87
void BrVector3SetInt(br_vector3* v1, int i1, int i2, int i3);

// Offset: 1991
// Size: 87
void BrVector3SetFloat(br_vector3* v1, float f1, float f2, float f3);

// Offset: 2094
// Size: 132
void BrVector3Negate(br_vector3* v1, br_vector3* v2);

// Offset: 2239
// Size: 174
void BrVector3Add(br_vector3* v1, br_vector3* v2, br_vector3* v3);

// Offset: 2433
// Size: 134
void BrVector3Accumulate(br_vector3* v1, br_vector3* v2);

// Offset: 2580
// Size: 174
void BrVector3Sub(br_vector3* v1, br_vector3* v2, br_vector3* v3);

// Offset: 2769
// Size: 135
void BrVector3Scale(br_vector3* v1, br_vector3* v2, br_scalar s);

// Offset: 2922
// Size: 135
void BrVector3InvScale(br_vector3* v1, br_vector3* v2, br_scalar s);

// Offset: 3070
// Size: 147
br_scalar BrVector3Dot(br_vector3* v1, br_vector3* v2);

// Offset: 3232
// Size: 280
void BrVector3Cross(br_vector3* v1, br_vector3* v2, br_vector3* v3);

// Offset: 3528
// Size: 118
br_scalar BrVector3Length(br_vector3* v1);

// Offset: 3669
// Size: 107
br_scalar BrVector3LengthSquared(br_vector3* v1);

// Offset: 3795
// Size: 226
void BrVector3Normalise(br_vector3* v1, br_vector3* v2);

// Offset: 4042
// Size: 191
void BrVector3NormaliseLP(br_vector3* v1, br_vector3* v2);

// Offset: 4246
// Size: 161
br_scalar BrVector4Dot(br_vector4* v1, br_vector4* v2);

// Offset: 4421
// Size: 138
void BrVector4Copy(br_vector4* v1, br_vector4* v2);

// Offset: 4573
// Size: 130
// EAX: v1
// EDX: v2
br_scalar BrFVector2Dot(br_fvector2* v1, br_vector2* v2);

// Offset: 4718
// Size: 134
// EAX: v1
// EDX: v2
void BrFVector3Copy(br_fvector3* v1, br_vector3* v2);

// Offset: 4868
// Size: 145
// EAX: v1
// EDX: v2
void BrVector3ScaleF(br_vector3* v1, br_fvector3* v2, br_scalar s);

// Offset: 5027
// Size: 144
// EAX: v1
// EDX: v2
br_scalar BrFVector3Dot(br_fvector3* v1, br_vector3* v2);

// Offset: 5191
// Size: 234
// EAX: v1
// EDX: v2
void BrFVector3Normalise(br_fvector3* v1, br_vector3* v2);

// Offset: 5447
// Size: 190
// EAX: v1
// EDX: v2
void BrFVector3NormaliseLP(br_fvector3* v1, br_vector3* v2);

// Offset: 5656
// Size: 190
void BrVector2Normalise(br_vector2* v1, br_vector2* v2);

#endif
