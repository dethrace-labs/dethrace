#ifndef _OIL_H_
#define _OIL_H_

#include "br_types.h"
#include "dr_types.h"

extern char *gOil_pixie_names[1];
extern int gNext_oil_pixie;
extern br_scalar gZ_buffer_diff;
extern br_scalar gMin_z_diff;
extern br_pixelmap *gOil_pixies[1];
extern tOil_spill_info gOily_spills[15];

void InitOilSpills();

void ResetOilSpills();

void QueueOilSpill(tCar_spec* pCar);

int OKToSpillOil(tOil_spill_info* pOil);

void Vector3Interpolate(br_vector3* pDst, br_vector3* pFrom, br_vector3* pTo, br_scalar pP);

void EnsureGroundDetailVisible(br_vector3* pNew_pos, br_vector3* pGround_normal, br_vector3* pOld_pos);

void MungeOilsHeightAboveGround(tOil_spill_info* pOil);

void MungeIndexedOilsHeightAboveGround(int pIndex);

void SetInitialOilStuff(tOil_spill_info* pOil, br_model* pModel);

void ProcessOilSpills(tU32 pFrame_period);

int GetOilSpillCount();

void GetOilSpillDetails(int pIndex, br_actor** pActor, br_scalar* pSize);

int PointInSpill(br_vector3* pV, int pSpill);

void GetOilFrictionFactors(tCar_spec* pCar, br_scalar* pFl_factor, br_scalar* pFr_factor, br_scalar* pRl_factor, br_scalar* pRr_factor);

void AdjustOilSpill(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap);

void ReceivedOilSpill(tNet_contents* pContents);

#endif
