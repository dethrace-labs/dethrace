#ifndef _CAR_H_
#define _CAR_H_

#include "br_types.h"
#include "dr_types.h"

#define CAR_MAX_SIMPLIFICATION_LEVEL 4

extern float gEngine_powerup_factor[6];
extern tCar_spec* gLast_car_to_skid[2];
extern tS3_sound_tag gSkid_tag[2];
extern tSave_camera gSave_camera[2];
extern tFace_ref gFace_list[150];
extern void (*ControlCar[6])(tCar_spec*, br_scalar);
extern tNon_car_spec* gActive_non_car_list[50];
extern float gCar_simplification_factor[2][CAR_MAX_SIMPLIFICATION_LEVEL + 1];
extern float gDefensive_powerup_factor[6];
extern float gOffensive_powerup_factor[6];
extern int gNet_player_to_view_index;
extern int gOver_shoot;
extern int gDouble_pling_water;
extern int gOpponent_viewing_mode;
extern br_scalar gMin_world_y;
extern int gMetal_crunch_sound_id[5];
extern int gFreeze_mechanics;
extern int gWoz_upside_down_at_all;
extern tU32 gQuite_wild_end;
extern tU32 gLast_mechanics_time;
extern tU32 gOn_me_wheels_start;
extern int gCar_car_collisions;
extern br_scalar gAccel;
extern br_vector3 gAverage_grid_position;
extern br_actor* gPed_actor;
extern tU32 gWild_start;
extern tU32 gLast_cunning_stunt;
extern int gCollision_count;
extern tU32 gQuite_wild_start;
extern int gCamera_frozen;
extern tU32 gSwitch_time;
extern int gMaterial_index;
extern int gEliminate_faces;
extern int gInTheSea;
extern int gMetal_scrape_sound_id[3];
extern br_vector3 gZero_v;
extern int gCamera_mode;
extern int gDoing_physics;
extern br_scalar gOur_yaw;
extern br_vector3 gCamera_pos_before_collide;
extern br_scalar gGravity;
extern br_vector3 gNew_ground_normal;
extern br_scalar gDt;
extern int gFace_num;
extern int gCollision_detection_on;
extern int gControl;
extern br_vector3 gGround_normal;
extern char gNon_car_spec_list[100];
extern int gCamera_has_collided;
extern int gCar_simplification_level;
extern int gNum_active_non_cars;
extern br_scalar gPanning_camera_height;
extern int gStop_opponents_moving;
extern tU32 gMechanics_time_sync;
extern int gNum_cars_and_non_cars;
extern int gFace_count;
extern br_angle gOld_yaw;
extern br_angle gOld_zoom;
extern br_angle gPanning_camera_angle;

// Offset: 0
// Size: 91
// EAX: pCar
// EDX: pUnit_type
// EBX: pDamage_amount
void DamageUnit(tCar_spec* pCar, int pUnit_type, int pDamage_amount);

// Offset: 92
// Size: 61
// EAX: pCar
// EDX: pUnit_type
// EBX: pDamage_amount
void DamageUnitWithSmoke(tCar_spec* pCar, int pUnit_type, int pDamage_amount);

// Offset: 156
// Size: 52
// EAX: pDamage_amount
void DamageEngine(int pDamage_amount);

// Offset: 208
// Size: 55
// EAX: pDamage_amount
void DamageTrans(int pDamage_amount);

// Offset: 264
// Size: 55
// EAX: pDamage_amount
void DamageSteering(int pDamage_amount);

// Offset: 320
// Size: 55
// EAX: pDamage_amount
void DamageLFWheel(int pDamage_amount);

// Offset: 376
// Size: 55
// EAX: pDamage_amount
void DamageLFBrake(int pDamage_amount);

// Offset: 432
// Size: 55
// EAX: pDamage_amount
void DamageLRBrake(int pDamage_amount);

// Offset: 488
// Size: 55
// EAX: pDamage_amount
void DamageLRWheel(int pDamage_amount);

// Offset: 544
// Size: 55
// EAX: pDamage_amount
void DamageRFWheel(int pDamage_amount);

// Offset: 600
// Size: 55
// EAX: pDamage_amount
void DamageRFBrake(int pDamage_amount);

// Offset: 656
// Size: 55
// EAX: pDamage_amount
void DamageRRBrake(int pDamage_amount);

// Offset: 712
// Size: 55
// EAX: pDamage_amount
void DamageRRWheel(int pDamage_amount);

// Offset: 768
// Size: 508
// EAX: pP1
// EDX: pP2
// EBX: pP3
// ECX: pNormal
void CalculatePlaneNormal(br_vector3* pP1, br_vector3* pP2, br_vector3* pP3, br_vector3* pNormal);

// Offset: 1276
// Size: 143
// EAX: pThe_model
// EDX: pFace_index
void CalculateGroundNormal(br_model* pThe_model, int pFace_index);

// Offset: 1420
// Size: 399
void ChangeYCoordinate(br_scalar pNew_y, tU32 pTime_taken, br_model* pThe_model, int pFace_index);

// Offset: 1820
// Size: 127
// EAX: pCar_spec
// EDX: pModel_index
void SwitchCarActor(tCar_spec* pCar_spec, int pModel_index);

// Offset: 1948
// Size: 1602
// EAX: pCar
// EDX: pClear_disabled_flag
void InitialiseCar2(tCar_spec* pCar, int pClear_disabled_flag);

// Offset: 3552
// Size: 50
// EAX: pCar
void InitialiseCar(tCar_spec* pCar);

// Offset: 3604
// Size: 270
// EAX: pThe_race
void InitialiseCarsEtc(tRace_info* pThe_race);

// Offset: 3876
// Size: 231
// EAX: pThe_race
void GetAverageGridPosition(tRace_info* pThe_race);

// Offset: 4108
// Size: 1067
// EAX: pThe_race
// EDX: pCar_index
// EBX: pGrid_index
void SetInitialPosition(tRace_info* pThe_race, int pCar_index, int pGrid_index);

// Offset: 5176
// Size: 84
// EAX: pThe_race
void SetInitialPositions(tRace_info* pThe_race);

// Offset: 5260
// Size: 534
// EAX: non_car
void InitialiseNonCar(tNon_car_spec* non_car);

// Offset: 5796
// Size: 2073
// EAX: c
void GetFacesInBox(tCollision_info* c);

// Offset: 7872
// Size: 45
int IsCarInTheSea();

// Offset: 7920
// Size: 570
// EAX: car
// EDX: pTime
void RememberSafePosition(tCar_spec* car, tU32 pTime);

// Offset: 8492
// Size: 1055
// EAX: pTime_difference
void ControlOurCar(tU32 pTime_difference);

// Offset: 9548
// Size: 1603
// EAX: c
void CalcEngineForce(tCar_spec* c, br_scalar dt);

// Offset: 11152
// Size: 515
// EAX: pFrame_start_time
void PrepareCars(tU32 pFrame_start_time);

// Offset: 11668
// Size: 1539
// EAX: pLast_frame_time
// EDX: pTime
void FinishCars(tU32 pLast_frame_time, tU32 pTime);

// Offset: 13208
// Size: 337
// EAX: pLast_frame_time
// EDX: pTime
void InterpolateCars(tU32 pLast_frame_time, tU32 pTime);

// Offset: 13548
// Size: 218
void ResetOldmat();

// Offset: 13768
// Size: 118
void GetNonCars();

// Offset: 13888
// Size: 1003
// EAX: pCar
void GetNetPos(tCar_spec* pCar);

// Offset: 14892
// Size: 1298
// EAX: last_frame_time
// EDX: pTime_difference
void ApplyPhysicsToCars(tU32 last_frame_time, tU32 pTime_difference);

// Offset: 16192
// Size: 436
// EAX: pCar
void MungeSpecialVolume(tCollision_info* pCar);

// Offset: 16628
// Size: 199
// EAX: pCar
void ResetCarSpecialVolume(tCollision_info* pCar);

// Offset: 16828
// Size: 1052
// EAX: pCar
void TestAutoSpecialVolume(tCollision_info* pCar);

// Offset: 17880
// Size: 421
// EAX: car
void MoveAndCollideCar(tCar_spec* car, br_scalar dt);

// Offset: 18304
// Size: 271
// EAX: non_car
void MoveAndCollideNonCar(tNon_car_spec* non_car, br_scalar dt);

// Offset: 18576
// Size: 336
// EAX: car
int CollideCarWithWall(tCollision_info* car, br_scalar dt);

// Offset: 18912
// Size: 268
void ToggleControls();

// Offset: 19180
// Size: 675
// EAX: c
void ControlCar2(tCar_spec* c, br_scalar dt);

// Offset: 19856
// Size: 686
// EAX: c
void ControlCar3(tCar_spec* c, br_scalar dt);

// Offset: 20544
// Size: 1374
// EAX: c
void ControlCar4(tCar_spec* c, br_scalar dt);

// Offset: 21920
// Size: 1097
// EAX: c
void ControlCar5(tCar_spec* c, br_scalar dt);

// Offset: 23020
// Size: 530
// EAX: c
void ControlCar1(tCar_spec* c, br_scalar dt);

// Offset: 23552
// Size: 138
// EAX: wdt
// EDX: m
void setrotate(br_vector3* wdt, br_matrix34* m);

// Offset: 23692
// Size: 454
// EAX: c
void RotateCar2(tCollision_info* c, br_scalar dt);

// Offset: 24148
// Size: 673
// EAX: c
void RotateCarSecondOrder(tCollision_info* c, br_scalar dt);

// Offset: 24824
// Size: 486
// EAX: c
void RotateCarFirstOrder(tCollision_info* c, br_scalar dt);

// Offset: 25312
// Size: 218
// EAX: c
void SimpleRotate(tCollision_info* c, br_scalar dt);

// Offset: 25532
// Size: 305
// EAX: c
void RotateCar(tCollision_info* c, br_scalar dt);

// Offset: 25840
// Size: 434
// EAX: c
void SteeringSelfCentre(tCar_spec* c, br_scalar dt, br_vector3* n);

// Offset: 26276
// Size: 923
// EAX: nc
void NonCarCalcForce(tNon_car_spec* nc, br_scalar dt);

// Offset: 27200
// Size: 402
// EAX: c
void AddDrag(tCar_spec* c, br_scalar dt);

// Offset: 27604
// Size: 419
// EAX: c
// EDX: wheel_pos
// EBX: norm
// ECX: d
void DoBumpiness(tCar_spec* c, br_vector3* wheel_pos, br_vector3* norm, br_scalar* d, int n);

// Offset: 28024
// Size: 12038
// EAX: c
void CalcForce(tCar_spec* c, br_scalar dt);

// Offset: 40064
// Size: 1156
// EAX: c
void DoRevs(tCar_spec* c, br_scalar dt);

// Offset: 41220
// Size: 127
// EAX: c
// EDX: tdt
void ApplyTorque(tCar_spec* c, br_vector3* tdt);

// Offset: 41348
// Size: 120
// EAX: c
void TranslateCar(tCollision_info* c, br_scalar dt);

// Offset: 41468
// Size: 9029
// EAX: c
int CollCheck(tCollision_info* c, br_scalar dt);

// Offset: 50500
// Size: 916
// EAX: c
// EDX: vel
// EBX: normal_force
// ECX: pos
br_scalar AddFriction(tCollision_info* c, br_vector3* vel, br_vector3* normal_force, br_vector3* pos, br_scalar total_force, br_vector3* max_friction);

// Offset: 51416
// Size: 1179
// EAX: car1
// EDX: car2
// EBX: vel1
// ECX: vel2
void AddFrictionCarToCar(tCollision_info* car1, tCollision_info* car2, br_vector3* vel1, br_vector3* vel2, br_vector3* normal_force1, br_vector3* pos1, br_vector3* pos2, br_scalar total_force, br_vector3* max_friction);

// Offset: 52596
// Size: 233
void ScrapeNoise(br_scalar vel, br_vector3* position, int material);

// Offset: 52832
// Size: 747
// EAX: pC
// EDX: pWheel_num
void SkidNoise(tCar_spec* pC, int pWheel_num, br_scalar pV, int material);

// Offset: 53580
// Size: 77
// EAX: pC
void StopSkid(tCar_spec* pC);

// Offset: 53660
// Size: 255
// EAX: pForce
// EDX: position
// EBX: material
void CrashNoise(br_vector3* pForce, br_vector3* position, int material);

// Offset: 53916
// Size: 1541
// EAX: c
// EDX: pPosition
// EBX: pForce_car_space
// ECX: car2
void CrushAndDamageCar(tCar_spec* c, br_vector3* pPosition, br_vector3* pForce_car_space, tCar_spec* car2);

// Offset: 55460
// Size: 472
// EAX: c
int ExpandBoundingBox(tCar_spec* c);

// Offset: 55932
// Size: 795
// EAX: c
// EDX: crush_only
void CrushBoundingBox(tCar_spec* c, int crush_only);

// Offset: 56728
// Size: 279
void AddCollPoint(br_scalar dist, br_vector3* p, br_vector3* norm, br_vector3* r, br_vector3* n, br_vector3* dir, int num, tCollision_info* c);

// Offset: 57008
// Size: 88
// EAX: f
// EDX: m
// EBX: d
br_scalar SinglePointColl(br_scalar* f, br_matrix4* m, br_scalar* d);

// Offset: 57096
// Size: 359
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar TwoPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n);

// Offset: 57456
// Size: 62
// EAX: mi
// EDX: mc
br_scalar DrMatrix4Inverse(br_matrix4* mi, br_matrix4* mc);

// Offset: 57520
// Size: 172
// EAX: f
// EDX: m
// EBX: d
br_scalar ThreePointColl(br_scalar* f, br_matrix4* m, br_scalar* d);

// Offset: 57692
// Size: 638
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar ThreePointCollRec(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c);

// Offset: 58332
// Size: 639
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar FourPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c);

// Offset: 58972
// Size: 228
// EAX: pNum_rays
// EDX: a
// EBX: b
// ECX: nor
void MultiFindFloorInBoxM(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref);

// Offset: 59200
// Size: 338
// EAX: pNum_rays
// EDX: a
// EBX: b
// ECX: nor
void MultiFindFloorInBoxBU(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref);

// Offset: 59540
// Size: 149
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
void findfloor(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d);

// Offset: 59692
// Size: 157
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
int FindFloorInBoxM(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c);

// Offset: 59852
// Size: 278
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
int FindFloorInBoxBU(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c);

// Offset: 60132
// Size: 443
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
int FindFloorInBoxBU2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c);

// Offset: 60576
// Size: 157
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
int FindFloorInBoxM2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c);

// Offset: 60736
// Size: 1324
// EAX: pB
// EDX: pM
// EBX: pMold
// ECX: pPoint_list
int BoxFaceIntersect(br_bounds* pB, br_matrix34* pM, br_matrix34* pMold, br_vector3* pPoint_list, br_vector3* pNorm_list, br_scalar* pDist_list, int pMax_pnts, tCollision_info* c);

// Offset: 62060
// Size: 3591
// EAX: p1
// EDX: p2
// EBX: pB
// ECX: pMold
int AddEdgeCollPoints(br_vector3* p1, br_vector3* p2, br_bounds* pB, br_matrix34* pMold, br_vector3* pPoint_list, br_vector3* pNorm_list, int n, int pMax_pnts, tCollision_info* c);

// Offset: 65652
// Size: 118
// EAX: n
// EDX: p
void GetPlaneNormal(br_vector3* n, int p);

// Offset: 65772
// Size: 534
// EAX: pos
// EDX: edge
// EBX: pB
// ECX: plane1
int GetBoundsEdge(br_vector3* pos, br_vector3* edge, br_bounds* pB, int plane1, int plane2, br_vector3* a, br_vector3* b, br_vector3* c, int flag);

// Offset: 66308
// Size: 603
// EAX: pTime_difference
void oldMoveOurCar(tU32 pTime_difference);

// Offset: 66912
// Size: 34
void ToggleCollisionDetection();

// Offset: 66948
// Size: 84
void CancelPendingCunningStunt();

// Offset: 67032
// Size: 61
float frac(float pN);

// Offset: 67096
// Size: 601
// EAX: pCar
void SetAmbientPratCam(tCar_spec* pCar);

// Offset: 67700
// Size: 3599
// EAX: pFrame_period
void MungeCarGraphics(tU32 pFrame_period);

// Offset: 71300
// Size: 179
void ResetCarScreens();

// Offset: 71480
// Size: 208
tCar_spec* GetRaceLeader();

// Offset: 71688
// Size: 555
void AmIGettingBoredWatchingCameraSpin();

// Offset: 72244
// Size: 161
void ViewNetPlayer();

// Offset: 72408
// Size: 251
void ViewOpponent();

// Offset: 72660
// Size: 134
void ToggleCarToCarCollisions();

// Offset: 72796
// Size: 34
void SwapCar();

// Offset: 72832
// Size: 178
void AdjustDownForce();

// Offset: 73012
// Size: 134
void FreezeMechanics();

// Offset: 73148
// Size: 134
void PutOpponentsInNeutral();

// Offset: 73284
// Size: 133
void SetPanningFieldOfView();

// Offset: 73420
// Size: 232
// EAX: pCar
void CheckDisablePlingMaterials(tCar_spec* pCar);

// Offset: 73652
// Size: 346
// EAX: c
// EDX: pTime
void PositionExternalCamera(tCar_spec* c, tU32 pTime);

// Offset: 74000
// Size: 102
// EAX: c
// EDX: pTime
void CameraBugFix(tCar_spec* c, tU32 pTime);

// Offset: 74104
// Size: 141
// EAX: pActor
int PossibleRemoveNonCarFromWorld(br_actor* pActor);

// Offset: 74248
// Size: 117
// EAX: pActor
void PutNonCarBackInWorld(br_actor* pActor);

// Offset: 74368
// Size: 2633
// EAX: c
// EDX: pTime
int IncidentCam(tCar_spec* c, tU32 pTime);

// Offset: 77004
// Size: 1267
// EAX: c
// EDX: type
// EBX: severity
// ECX: info
int MoveCamToIncident(tCar_spec* c, tIncident_type* type, float* severity, tIncident_info* info, tU32* next_incident_time);

// Offset: 78272
// Size: 368
// EAX: c
// EDX: pTime
void PanningExternalCamera(tCar_spec* c, tU32 pTime);

// Offset: 78640
// Size: 136
// EAX: start
// EDX: end
int CheckForWall(br_vector3* start, br_vector3* end);

// Offset: 78776
// Size: 1383
// EAX: c
void SetUpPanningCamera(tCar_spec* c);

// Offset: 80160
// Size: 97
// EAX: i
void SaveCameraPosition(int i);

// Offset: 80260
// Size: 97
// EAX: i
void RestoreCameraPosition(int i);

// Offset: 80360
// Size: 2107
// EAX: c
// EDX: pTime
void NormalPositionExternalCamera(tCar_spec* c, tU32 pTime);

// Offset: 82468
// Size: 446
// EAX: c
// EDX: vn
// EBX: manual_swing
void MoveWithWheels(tCar_spec* c, br_vector3* vn, int manual_swing);

// Offset: 82916
// Size: 1828
// EAX: c
// EDX: m1
// EBX: m2
// ECX: vn
void SwingCamera(tCar_spec* c, br_matrix34* m1, br_matrix34* m2, br_vector3* vn, tU32 pTime);

// Offset: 84744
// Size: 776
// EAX: c
// EDX: m1
// EBX: m2
void PointCameraAtCar(tCar_spec* c, br_matrix34* m1, br_matrix34* m2);

// Offset: 85520
// Size: 465
// EAX: pos
// EDX: m2
void PointCamera(br_vector3* pos, br_matrix34* m2);

// Offset: 85988
// Size: 2411
// EAX: car_pos
// EDX: cam_pos
// EBX: old_camera_pos
// ECX: manual_move
int CollideCamera2(br_vector3* car_pos, br_vector3* cam_pos, br_vector3* old_camera_pos, int manual_move);

// Offset: 88400
// Size: 141
// EAX: bnds
// EDX: p
int BoundsTest(br_bounds* bnds, br_vector3* p);

// Offset: 88544
// Size: 494
// EAX: car_pos
// EDX: cam_pos
int CollideCameraWithOtherCars(br_vector3* car_pos, br_vector3* cam_pos);

// Offset: 89040
// Size: 511
void InitialiseExternalCamera();

// Offset: 89552
// Size: 44
void FreezeCamera();

// Offset: 89596
// Size: 2248
// EAX: c
void FlyCar(tCar_spec* c, br_scalar dt);

// Offset: 91844
// Size: 157
// EAX: v
// EDX: t
void DrVector3RotateY(br_vector3* v, br_angle t);

// Offset: 92004
// Size: 237
void CrashCarsTogether(br_scalar dt);

// Offset: 92244
// Size: 2384
int CrashCarsTogetherSinglePass(br_scalar dt, int pPass, tCollison_data* collide_list);

// Offset: 94628
// Size: 142
// EAX: car
void BringCarToAGrindingHalt(tCollision_info* car);

// Offset: 94940
// Size: 99
// EAX: car1
// EDX: car2
int SimpleCarCarCollisionTest(tCollision_info* car1, tCollision_info* car2);

// Offset: 95040
// Size: 877
// EAX: car1
// EDX: car2
int CollideTwoCarsWithWalls(tCollision_info* car1, tCollision_info* car2, br_scalar dt);

// Offset: 95920
// Size: 298
// EAX: car1
// EDX: car2
int CollideTwoCarsRepeatedly(tCollision_info* car1, tCollision_info* car2, br_scalar dt);

// Offset: 96220
// Size: 3499
// EAX: car1
// EDX: car2
// EBX: pPass
int CollideTwoCars(tCollision_info* car1, tCollision_info* car2, int pPass);

// Offset: 99720
// Size: 3598
// EAX: pB1
// EDX: pB2
// EBX: pM21
// ECX: pM12
int GetEdgeEdgeCollisions(br_bounds* pB1, br_bounds* pB2, br_matrix34* pM21, br_matrix34* pM12, br_matrix34* pMo21, br_matrix34* pMo12, br_matrix34* pM1o1, br_vector3* pPoint_list, br_vector3* pNorm_list, int pMax);

// Offset: 103320
// Size: 1920
// EAX: car1
// EDX: car2
// EBX: pMms
// ECX: pMoms
int FacePointCarCarCollide(tCollision_info* car1, tCollision_info* car2, br_matrix34* pMms, br_matrix34* pMoms, br_matrix34* pMsos, br_vector3* pPoint_list, br_vector3* pNorm_list, int pMax, int order);

// Offset: 105240
// Size: 111
// EAX: pCar
void MungeCarsMass(tCollision_info* pCar, br_scalar pFactor);

// Offset: 105352
// Size: 124
// EAX: pCar_1
// EDX: pCar_2
void ModifyCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2);

// Offset: 105476
// Size: 144
// EAX: pCar_1
// EDX: pCar_2
void ResetCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2);

// Offset: 105620
// Size: 8941
// EAX: car1
// EDX: car2
// EBX: r
// ECX: n
int DoCollide(tCollision_info* car1, tCollision_info* car2, br_vector3* r, br_vector3* n, int k, int pPass, br_matrix34* mat1_to_mat2);

// Offset: 114564
// Size: 417
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar TwoPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n);

// Offset: 114984
// Size: 827
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar ThreePointCollRecB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n);

// Offset: 115812
// Size: 789
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar FourPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n);

// Offset: 116604
// Size: 89
// EAX: f
int TestForNan(float* f);

// Offset: 116696
// Size: 89
void CheckCameraHither();

// Offset: 116788
// Size: 525
// EAX: pCar
void SetCarSuspGiveAndHeight(tCar_spec* pCar, br_scalar pFront_give_factor, br_scalar pRear_give_factor, br_scalar pDamping_factor, br_scalar pExtra_front_height, br_scalar pExtra_rear_height);

// Offset: 117316
// Size: 1023
// EAX: car
int TestForCarInSensiblePlace(tCar_spec* car);

// Offset: 118340
// Size: 528
// EAX: c1
// EDX: c2
// EBX: newmats
int TestOldMats(tCollision_info* c1, tCollision_info* c2, int newmats);

// Offset: 118868
// Size: 69
// EAX: pActor
int PullActorFromWorld(br_actor* pActor);

// Offset: 118940
// Size: 681
// EAX: pActor
int DoPullActorFromWorld(br_actor* pActor);

// Offset: 119624
// Size: 904
// EAX: pTime
void CheckForDeAttachmentOfNonCars(tU32 pTime);

// Offset: 120528
// Size: 248
// EAX: pActor
// EDX: pMat
void AdjustNonCar(br_actor* pActor, br_matrix34* pMat);

// Offset: 120776
// Size: 240
// EAX: c
void PipeSingleNonCar(tCollision_info* c);

// Offset: 121016
// Size: 169
// EAX: pCar
// EDX: pFace_refs
int GetPrecalculatedFacesUnderCar(tCar_spec* pCar, tFace_ref** pFace_refs);

// Offset: 121188
// Size: 53
br_material* SomeNearbyMaterial();

#endif
