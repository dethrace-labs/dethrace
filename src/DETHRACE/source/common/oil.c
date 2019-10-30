#include "oil.h"

// Global variables
int gNext_oil_pixie;
char *gOil_pixie_names[1];
br_scalar gZ_buffer_diff;
br_scalar gMin_z_diff;
br_pixelmap *gOil_pixies[1];
tOil_spill_info gOily_spills[15];

// Offset: 0
// Size: 811
void InitOilSpills() {
  br_model *the_model;
  br_material *the_material;
}

// Offset: 812
// Size: 101
void ResetOilSpills() {
}

// Offset: 916
// Size: 335
// EAX: pCar
void QueueOilSpill(tCar_spec *pCar) {
  int i;
  int oily_index;
  int oldest_one;
  tU32 the_time;
  tU32 oldest_time;
}

// Offset: 1252
// Size: 967
// EAX: pOil
int OKToSpillOil(tOil_spill_info *pOil) {
  br_scalar temp;
  br_scalar size_with_margin;
  br_scalar distance;
  br_scalar mr_dotty;
  br_vector3 v;
  br_vector3 ray_pos;
  br_vector3 ray_dir;
  br_vector3 normal;
  tCar_spec *car;
  int i;
  int face_count;
  int found_one;
  br_angle angle_to_rotate_by;
  tBounds kev_bounds;
  tFace_ref the_list[10];
  tFace_ref *face_ref;
}

// Offset: 2220
// Size: 124
// EAX: pDst
// EDX: pFrom
// EBX: pTo
void Vector3Interpolate(br_vector3 *pDst, br_vector3 *pFrom, br_vector3 *pTo, br_scalar pP) {
}

// Offset: 2344
// Size: 252
// EAX: pNew_pos
// EDX: pGround_normal
// EBX: pOld_pos
void EnsureGroundDetailVisible(br_vector3 *pNew_pos, br_vector3 *pGround_normal, br_vector3 *pOld_pos) {
  br_scalar factor;
  br_scalar s;
  br_scalar dist;
  br_vector3 to_camera;
}

// Offset: 2596
// Size: 64
// EAX: pOil
void MungeOilsHeightAboveGround(tOil_spill_info *pOil) {
}

// Offset: 2660
// Size: 53
// EAX: pIndex
void MungeIndexedOilsHeightAboveGround(int pIndex) {
}

// Offset: 2716
// Size: 193
// EAX: pOil
// EDX: pModel
void SetInitialOilStuff(tOil_spill_info *pOil, br_model *pModel) {
}

// Offset: 2912
// Size: 1329
// EAX: pFrame_period
void ProcessOilSpills(tU32 pFrame_period) {
  int i;
  tU32 time;
  br_model *the_model;
  br_scalar grow_amount;
  br_scalar initial_size;
  br_scalar this_size;
  br_vector3 v;
  tNet_message *message;
}

// Offset: 4244
// Size: 44
int GetOilSpillCount() {
}

// Offset: 4288
// Size: 93
// EAX: pIndex
// EDX: pActor
// EBX: pSize
void GetOilSpillDetails(int pIndex, br_actor **pActor, br_scalar *pSize, short __unk3__, br_memory_classes __unk4__) {
}

// Offset: 4384
// Size: 273
// EAX: pV
// EDX: pSpill
int PointInSpill(br_vector3 *pV) {
  int pSpill;
}

// Offset: 4660
// Size: 597
// EAX: pCar
// EDX: pFl_factor
// EBX: pFr_factor
// ECX: pRl_factor
void GetOilFrictionFactors(tCar_spec *pCar, br_scalar *pFl_factor, br_scalar *pFr_factor, br_scalar *pRl_factor, br_scalar *pRr_factor) {
  int i;
  br_vector3 wheel_world;
}

// Offset: 5260
// Size: 211
// EAX: pIndex
// EDX: pMat
void AdjustOilSpill(int pIndex, br_matrix34 *pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap, char __unk9__, brfile_getchr_cbfn __unk10__, tNet_contents __unk11__, unsigned char __unk12__, br_memory_classes __unk13__, struct __unk15__, tNet_contents *__unk16__) {
}

// Offset: 5472
// Size: 338
// EAX: pContents
void ReceivedOilSpill(tNet_contents *pContents) {
  int i;
  int oily_index;
  int oldest_one;
  tU32 the_time;
  tU32 oldest_time;
  tCar_spec *car;
}

