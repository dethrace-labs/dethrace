#ifndef _SKIDMARK_H_
#define _SKIDMARK_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 600
// EAX: pMark
// EDX: pFrom
// EBX: pTo
void StretchMark(tSkid *pMark, br_vector3 *pFrom, br_vector3 *pTo, br_scalar pTexture_start);

// Offset: 600
// Size: 81
// EAX: pIndex
br_material* MaterialFromIndex(int pIndex);

// Offset: 684
// Size: 116
// EAX: pSkid_num
// EDX: pMatrix
// EBX: pMaterial_index
void AdjustSkid(int pSkid_num, br_matrix34 *pMatrix, int pMaterial_index);

// Offset: 800
// Size: 182
// EAX: pPt
// EDX: pL1
// EBX: pL2
int FarFromLine2D(br_vector3 *pPt, br_vector3 *pL1, br_vector3 *pL2);

// Offset: 984
// Size: 137
// EAX: pPt
// EDX: pL1
// EBX: pL2
int Reflex2D(br_vector3 *pPt, br_vector3 *pL1, br_vector3 *pL2);

// Offset: 1124
// Size: 885
void InitSkids();

// Offset: 2012
// Size: 51
// EAX: pSkid_num
void HideSkid(int pSkid_num);

// Offset: 2064
// Size: 67
void HideSkids();

// Offset: 2132
// Size: 136
// EAX: pSkid
br_scalar SkidLen(int pSkid);

// Offset: 2268
// Size: 949
// EAX: pCar
// EDX: pWheel_num
// EBX: pPos
// ECX: pMaterial_index
void SkidSection(tCar_spec *pCar, int pWheel_num, br_vector3 *pPos, int pMaterial_index);

// Offset: 3220
// Size: 1175
// EAX: pCar
// EDX: pWheel_num
void SkidMark(tCar_spec *pCar, int pWheel_num);

// Offset: 4396
// Size: 130
// EAX: pCar
void InitCarSkidStuff(tCar_spec *pCar);

// Offset: 4528
// Size: 121
void SkidsPerFrame();

// Offset: 4652
// Size: 76
void RemoveMaterialsFromSkidmarks();

#endif
