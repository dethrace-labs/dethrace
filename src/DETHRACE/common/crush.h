#ifndef _CRUSH_H_
#define _CRUSH_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern float gWobble_spam_y[8];
extern float gWobble_spam_z[8];
extern br_scalar gWheel_circ_to_width;
extern tU8 gSmoke_damage_step[12];
extern int gSteal_ranks[5];

int ReadCrushData(FILE* pF, tCrush_data* pCrush_data);

float SkipCrushData(FILE* pF);

int WriteCrushData(FILE* pF, tCrush_data* pCrush_data);

void DisposeCrushData(tCrush_data* pCrush_data);

void CrushModelPoint(tCar_spec* pCar, int pModel_index, br_model* pModel, int pCrush_point_index, br_vector3* pEnergy_vector, br_scalar total_energy, tCrush_data* pCrush_data);

void CrushModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, br_vector3* pImpact_point, br_vector3* pEnergy_vector, tCrush_data* pCrush_data);

void JitModelUpdate(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen);

void SetModelForUpdate(br_model* pModel, tCar_spec* pCar, int crush_only);

void TotallySpamTheModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, tCrush_data* pCrush_data, br_scalar pMagnitude);

br_scalar RepairModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, br_vertex* pUndamaged_vertices, br_scalar pAmount, br_scalar* pTotal_deflection);

float RepairCar2(tCar_spec* pCar, tU32 pFrame_period, br_scalar* pTotal_deflection);

float RepairCar(tU16 pCar_ID, tU32 pFrame_period, br_scalar* pTotal_deflection);

void TotallyRepairACar(tCar_spec* pCar);

void TotallyRepairCar(void);

void CheckLastCar(void);

void KnackerThisCar(tCar_spec* pCar);

void SetKnackeredFlag(tCar_spec* pCar);

void DamageUnit2(tCar_spec* pCar, int pUnit_type, int pDamage_amount);

void RecordLastDamage(tCar_spec* pCar);

void DoDamage(tCar_spec* pCar, tDamage_type pDamage_type, float pMagnitude, float pNastiness);

void CheckPiledriverBonus(tCar_spec* pCar, br_vector3* pImpact_point, br_vector3* pEnergy);

tImpact_location CalcModifiedLocation(tCar_spec* pCar);

void DoPratcamHit(br_vector3* pHit_vector);

void DamageSystems(tCar_spec* pCar, br_vector3* pImpact_point, br_vector3* pEnergy_vector, int pWas_hitting_a_car);

tImpact_location GetDirection(br_vector3* pVelocity);

void SetSmokeLastDamageLevel(tCar_spec* pCar);

void SortOutSmoke(tCar_spec* pCar);

void StealCar(tCar_spec* pCar);

int DoCrashEarnings(tCar_spec* pCar1, tCar_spec* pCar2);

void DoWheelDamage(tU32 pFrame_period);

void CrashEarnings(tCar_spec* pCar1, tCar_spec* pCar2);

#endif
