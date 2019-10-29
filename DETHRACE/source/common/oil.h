#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 811
void InitOilSpills();

// Offset: 812
// Size: 101
void ResetOilSpills();

// Offset: 916
// Size: 335
// EAX: pCar
void QueueOilSpill(tCar_spec *pCar);

// Offset: 1252
// Size: 967
// EAX: pOil
int OKToSpillOil(tOil_spill_info *pOil);

// Offset: 2220
// Size: 124
// EAX: pDst
// EDX: pFrom
// EBX: pTo
void Vector3Interpolate(br_vector3 *pDst, br_vector3 *pFrom, br_vector3 *pTo, br_scalar pP);

// Offset: 2344
// Size: 252
// EAX: pNew_pos
// EDX: pGround_normal
// EBX: pOld_pos
void EnsureGroundDetailVisible(br_vector3 *pNew_pos, br_vector3 *pGround_normal, br_vector3 *pOld_pos);

// Offset: 2596
// Size: 64
// EAX: pOil
void MungeOilsHeightAboveGround(tOil_spill_info *pOil);

// Offset: 2660
// Size: 53
// EAX: pIndex
void MungeIndexedOilsHeightAboveGround(int pIndex);

// Offset: 2716
// Size: 193
// EAX: pOil
// EDX: pModel
void SetInitialOilStuff(tOil_spill_info *pOil, br_model *pModel);

// Offset: 2912
// Size: 1329
// EAX: pFrame_period
void ProcessOilSpills(tU32 pFrame_period);

// Offset: 4244
// Size: 44
int GetOilSpillCount();

// Offset: 4288
// Size: 93
// EAX: pIndex
// EDX: pActor
// EBX: pSize
void GetOilSpillDetails(int pIndex, br_actor **pActor, br_scalar *pSize, short __unk3__, br_memory_classes __unk4__);

// Offset: 4384
// Size: 273
// EAX: pV
// EDX: pSpill
int PointInSpill(br_vector3 *pV);

// Offset: 4660
// Size: 597
// EAX: pCar
// EDX: pFl_factor
// EBX: pFr_factor
// ECX: pRl_factor
void GetOilFrictionFactors(tCar_spec *pCar, br_scalar *pFl_factor, br_scalar *pFr_factor, br_scalar *pRl_factor, br_scalar *pRr_factor);

// Offset: 5260
// Size: 211
// EAX: pIndex
// EDX: pMat
void AdjustOilSpill(int pIndex, br_matrix34 *pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap, char __unk9__, brfile_getchr_cbfn __unk10__, tNet_contents __unk11__, unsigned char __unk12__, br_memory_classes __unk13__, struct __unk15__, tNet_contents *__unk16__);

// Offset: 5472
// Size: 338
// EAX: pContents
void ReceivedOilSpill(tNet_contents *pContents);

