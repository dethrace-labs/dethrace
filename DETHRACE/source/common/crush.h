#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 620
// EAX: pF
// EDX: pCrush_data
int ReadCrushData(FILE *pF, tCrush_data *pCrush_data);

// Offset: 620
// Size: 230
// EAX: pF
float SkipCrushData(FILE *pF);

// Offset: 852
// Size: 702
// EAX: pF
// EDX: pCrush_data
int WriteCrushData(FILE *pF, tCrush_data *pCrush_data);

// Offset: 1556
// Size: 130
// EAX: pCrush_data
void DisposeCrushData(tCrush_data *pCrush_data);

// Offset: 1688
// Size: 1656
// EAX: pCar
// EDX: pModel_index
// EBX: pModel
// ECX: pCrush_point_index
void CrushModelPoint(tCar_spec *pCar, int pModel_index, br_model *pModel, int pCrush_point_index, br_vector3 *pEnergy_vector, br_scalar total_energy);

// Offset: 3344
// Size: 527
// EAX: pCar
// EDX: pModel_index
// EBX: pActor
// ECX: pImpact_point
void CrushModel(tCar_spec *pCar, int pModel_index, br_actor *pActor, br_vector3 *pImpact_point, br_vector3 *pEnergy_vector);

// Offset: 3872
// Size: 99
void JitModelUpdate(br_actor *actor, br_model *model, br_material *material, void *render_data);

// Offset: 3972
// Size: 131
// EAX: pModel
// EDX: pCar
// EBX: crush_only
void SetModelForUpdate(br_model *pModel, tCar_spec *pCar);

// Offset: 4104
// Size: 380
// EAX: pCar
// EDX: pModel_index
// EBX: pActor
// ECX: pCrush_data
void TotallySpamTheModel(tCar_spec *pCar, int pModel_index, br_actor *pActor, tCrush_data *pCrush_data);

// Offset: 4484
// Size: 662
// EAX: pCar
// EDX: pModel_index
// EBX: pActor
// ECX: pUndamaged_vertices
br_scalar RepairModel(tCar_spec *pCar, int pModel_index, br_actor *pActor, br_vertex *pUndamaged_vertices, br_scalar pAmount);

// Offset: 5148
// Size: 298
// EAX: pCar
// EDX: pFrame_period
// EBX: pTotal_deflection
float RepairCar2(tCar_spec *pCar, tU32 pFrame_period, br_scalar *pTotal_deflection);

// Offset: 5448
// Size: 120
// EAX: pCar_ID
// EDX: pFrame_period
// EBX: pTotal_deflection
float RepairCar(tU16 pCar_ID, tU32 pFrame_period, br_scalar *pTotal_deflection);

// Offset: 5568
// Size: 710
// EAX: pCar
void TotallyRepairACar(tCar_spec *pCar);

// Offset: 6280
// Size: 53
void TotallyRepairCar();

// Offset: 6336
// Size: 111
void CheckLastCar();

// Offset: 6448
// Size: 102
// EAX: pCar
void KnackerThisCar(tCar_spec *pCar);

// Offset: 6552
// Size: 278
// EAX: pCar
void SetKnackeredFlag(tCar_spec *pCar);

// Offset: 6832
// Size: 270
// EAX: pCar
// EDX: pUnit_type
// EBX: pDamage_amount
void DamageUnit2(tCar_spec *pCar, int pUnit_type);

// Offset: 7104
// Size: 236
// EAX: pCar
void RecordLastDamage(tCar_spec *pCar);

// Offset: 7340
// Size: 249
// EAX: pCar
// EDX: pDamage_type
void DoDamage(tCar_spec *pCar, tDamage_type pDamage_type, float pMagnitude);

// Offset: 7592
// Size: 650
// EAX: pCar
// EDX: pImpact_point
// EBX: pEnergy
void CheckPiledriverBonus(tCar_spec *pCar, br_vector3 *pImpact_point, br_vector3 *pEnergy);

// Offset: 8244
// Size: 152
// EAX: pCar
tImpact_location CalcModifiedLocation(tCar_spec *pCar);

// Offset: 8396
// Size: 222
// EAX: pHit_vector
void DoPratcamHit(br_vector3 *pHit_vector);

// Offset: 8620
// Size: 2217
// EAX: pCar
// EDX: pImpact_point
// EBX: pEnergy_vector
// ECX: pWas_hitting_a_car
void DamageSystems(tCar_spec *pCar, br_vector3 *pImpact_point, br_vector3 *pEnergy_vector);

// Offset: 10840
// Size: 210
// EAX: pVelocity
tImpact_location GetDirection(br_vector3 *pVelocity);

// Offset: 11052
// Size: 92
// EAX: pCar
void SetSmokeLastDamageLevel(tCar_spec *pCar);

// Offset: 11144
// Size: 318
// EAX: pCar
void SortOutSmoke(tCar_spec *pCar);

// Offset: 11464
// Size: 103
// EAX: pCar
void StealCar(tCar_spec *pCar);

// Offset: 11568
// Size: 5298
// EAX: pCar1
// EDX: pCar2
int DoCrashEarnings(tCar_spec *pCar1, tCar_spec *pCar2);

// Offset: 16868
// Size: 1028
// EAX: pFrame_period
void DoWheelDamage(tU32 pFrame_period);

// Offset: 17896
// Size: 69
// EAX: pCar1
// EDX: pCar2
void CrashEarnings(tCar_spec *pCar1, tCar_spec *pCar2);

