#ifndef _SKIDMARK_H_
#define _SKIDMARK_H_

#include "br_types.h"
#include "dr_types.h"

void StretchMark(tSkid* pMark, br_vector3* pFrom, br_vector3* pTo, br_scalar pTexture_start);

br_material* MaterialFromIndex(int pIndex);

void AdjustSkid(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index);

int FarFromLine2D(br_vector3* pPt, br_vector3* pL1, br_vector3* pL2);

int Reflex2D(br_vector3* pPt, br_vector3* pL1, br_vector3* pL2);

void InitSkids();

void HideSkid(int pSkid_num);

void HideSkids();

br_scalar SkidLen(int pSkid);

void SkidSection(tCar_spec* pCar, int pWheel_num, br_vector3* pPos, int pMaterial_index);

void SkidMark(tCar_spec* pCar, int pWheel_num);

void InitCarSkidStuff(tCar_spec* pCar);

void SkidsPerFrame();

void RemoveMaterialsFromSkidmarks();

#endif
