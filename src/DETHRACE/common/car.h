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

void DamageUnit(tCar_spec* pCar, int pUnit_type, int pDamage_amount);

void DamageUnitWithSmoke(tCar_spec* pCar, int pUnit_type, int pDamage_amount);

void DamageEngine(int pDamage_amount);

void DamageTrans(int pDamage_amount);

void DamageSteering(int pDamage_amount);

void DamageLFWheel(int pDamage_amount);

void DamageLFBrake(int pDamage_amount);

void DamageLRBrake(int pDamage_amount);

void DamageLRWheel(int pDamage_amount);

void DamageRFWheel(int pDamage_amount);

void DamageRFBrake(int pDamage_amount);

void DamageRRBrake(int pDamage_amount);

void DamageRRWheel(int pDamage_amount);

void CalculatePlaneNormal(br_vector3* pP1, br_vector3* pP2, br_vector3* pP3, br_vector3* pNormal);

void CalculateGroundNormal(br_model* pThe_model, int pFace_index);

void ChangeYCoordinate(br_scalar pNew_y, tU32 pTime_taken, br_model* pThe_model, int pFace_index);

void SwitchCarActor(tCar_spec* pCar_spec, int pModel_index);

void InitialiseCar2(tCar_spec* pCar, int pClear_disabled_flag);

void InitialiseCar(tCar_spec* pCar);

void InitialiseCarsEtc(tRace_info* pThe_race);

void GetAverageGridPosition(tRace_info* pThe_race);

void SetInitialPosition(tRace_info* pThe_race, int pCar_index, int pGrid_index);

void SetInitialPositions(tRace_info* pThe_race);

void InitialiseNonCar(tNon_car_spec* non_car);

void GetFacesInBox(tCollision_info* c);

int IsCarInTheSea();

void RememberSafePosition(tCar_spec* car, tU32 pTime);

void ControlOurCar(tU32 pTime_difference);

void CalcEngineForce(tCar_spec* c, br_scalar dt);

void PrepareCars(tU32 pFrame_start_time);

void FinishCars(tU32 pLast_frame_time, tU32 pTime);

void InterpolateCars(tU32 pLast_frame_time, tU32 pTime);

void ResetOldmat();

void GetNonCars();

void GetNetPos(tCar_spec* pCar);

void ApplyPhysicsToCars(tU32 last_frame_time, tU32 pTime_difference);

void MungeSpecialVolume(tCollision_info* pCar);

void ResetCarSpecialVolume(tCollision_info* pCar);

void TestAutoSpecialVolume(tCollision_info* pCar);

void MoveAndCollideCar(tCar_spec* car, br_scalar dt);

void MoveAndCollideNonCar(tNon_car_spec* non_car, br_scalar dt);

int CollideCarWithWall(tCollision_info* car, br_scalar dt);

void ToggleControls();

void ControlCar2(tCar_spec* c, br_scalar dt);

void ControlCar3(tCar_spec* c, br_scalar dt);

void ControlCar4(tCar_spec* c, br_scalar dt);

void ControlCar5(tCar_spec* c, br_scalar dt);

void ControlCar1(tCar_spec* c, br_scalar dt);

void setrotate(br_vector3* wdt, br_matrix34* m);

void RotateCar2(tCollision_info* c, br_scalar dt);

void RotateCarSecondOrder(tCollision_info* c, br_scalar dt);

void RotateCarFirstOrder(tCollision_info* c, br_scalar dt);

void SimpleRotate(tCollision_info* c, br_scalar dt);

void RotateCar(tCollision_info* c, br_scalar dt);

void SteeringSelfCentre(tCar_spec* c, br_scalar dt, br_vector3* n);

void NonCarCalcForce(tNon_car_spec* nc, br_scalar dt);

void AddDrag(tCar_spec* c, br_scalar dt);

void DoBumpiness(tCar_spec* c, br_vector3* wheel_pos, br_vector3* norm, br_scalar* d, int n);

void CalcForce(tCar_spec* c, br_scalar dt);

void DoRevs(tCar_spec* c, br_scalar dt);

void ApplyTorque(tCar_spec* c, br_vector3* tdt);

void TranslateCar(tCollision_info* c, br_scalar dt);

int CollCheck(tCollision_info* c, br_scalar dt);

br_scalar AddFriction(tCollision_info* c, br_vector3* vel, br_vector3* normal_force, br_vector3* pos, br_scalar total_force, br_vector3* max_friction);

void AddFrictionCarToCar(tCollision_info* car1, tCollision_info* car2, br_vector3* vel1, br_vector3* vel2, br_vector3* normal_force1, br_vector3* pos1, br_vector3* pos2, br_scalar total_force, br_vector3* max_friction);

void ScrapeNoise(br_scalar vel, br_vector3* position, int material);

void SkidNoise(tCar_spec* pC, int pWheel_num, br_scalar pV, int material);

void StopSkid(tCar_spec* pC);

void CrashNoise(br_vector3* pForce, br_vector3* position, int material);

void CrushAndDamageCar(tCar_spec* c, br_vector3* pPosition, br_vector3* pForce_car_space, tCar_spec* car2);

int ExpandBoundingBox(tCar_spec* c);

void CrushBoundingBox(tCar_spec* c, int crush_only);

void AddCollPoint(br_scalar dist, br_vector3* p, br_vector3* norm, br_vector3* r, br_vector3* n, br_vector3* dir, int num, tCollision_info* c);

br_scalar SinglePointColl(br_scalar* f, br_matrix4* m, br_scalar* d);

br_scalar TwoPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n);

br_scalar DrMatrix4Inverse(br_matrix4* mi, br_matrix4* mc);

br_scalar ThreePointColl(br_scalar* f, br_matrix4* m, br_scalar* d);

br_scalar ThreePointCollRec(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c);

br_scalar FourPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c);

void MultiFindFloorInBoxM(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref);

void MultiFindFloorInBoxBU(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref);

void findfloor(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d);

int FindFloorInBoxM(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c);

int FindFloorInBoxBU(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c);

int FindFloorInBoxBU2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c);

int FindFloorInBoxM2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c);

int BoxFaceIntersect(br_bounds* pB, br_matrix34* pM, br_matrix34* pMold, br_vector3* pPoint_list, br_vector3* pNorm_list, br_scalar* pDist_list, int pMax_pnts, tCollision_info* c);

int AddEdgeCollPoints(br_vector3* p1, br_vector3* p2, br_bounds* pB, br_matrix34* pMold, br_vector3* pPoint_list, br_vector3* pNorm_list, int n, int pMax_pnts, tCollision_info* c);

void GetPlaneNormal(br_vector3* n, int p);

int GetBoundsEdge(br_vector3* pos, br_vector3* edge, br_bounds* pB, int plane1, int plane2, br_vector3* a, br_vector3* b, br_vector3* c, int flag);

void oldMoveOurCar(tU32 pTime_difference);

void ToggleCollisionDetection();

void CancelPendingCunningStunt();

float frac(float pN);

void SetAmbientPratCam(tCar_spec* pCar);

void MungeCarGraphics(tU32 pFrame_period);

void ResetCarScreens();

tCar_spec* GetRaceLeader();

void AmIGettingBoredWatchingCameraSpin();

void ViewNetPlayer();

void ViewOpponent();

void ToggleCarToCarCollisions();

void SwapCar();

void AdjustDownForce();

void FreezeMechanics();

void PutOpponentsInNeutral();

void SetPanningFieldOfView();

void CheckDisablePlingMaterials(tCar_spec* pCar);

void PositionExternalCamera(tCar_spec* c, tU32 pTime);

void CameraBugFix(tCar_spec* c, tU32 pTime);

int PossibleRemoveNonCarFromWorld(br_actor* pActor);

void PutNonCarBackInWorld(br_actor* pActor);

int IncidentCam(tCar_spec* c, tU32 pTime);

int MoveCamToIncident(tCar_spec* c, tIncident_type* type, float* severity, tIncident_info* info, tU32* next_incident_time);

void PanningExternalCamera(tCar_spec* c, tU32 pTime);

int CheckForWall(br_vector3* start, br_vector3* end);

void SetUpPanningCamera(tCar_spec* c);

void SaveCameraPosition(int i);

void RestoreCameraPosition(int i);

void NormalPositionExternalCamera(tCar_spec* c, tU32 pTime);

void MoveWithWheels(tCar_spec* c, br_vector3* vn, int manual_swing);

void SwingCamera(tCar_spec* c, br_matrix34* m1, br_matrix34* m2, br_vector3* vn, tU32 pTime);

void PointCameraAtCar(tCar_spec* c, br_matrix34* m1, br_matrix34* m2);

void PointCamera(br_vector3* pos, br_matrix34* m2);

int CollideCamera2(br_vector3* car_pos, br_vector3* cam_pos, br_vector3* old_camera_pos, int manual_move);

int BoundsTest(br_bounds* bnds, br_vector3* p);

int CollideCameraWithOtherCars(br_vector3* car_pos, br_vector3* cam_pos);

void InitialiseExternalCamera();

void FreezeCamera();

void FlyCar(tCar_spec* c, br_scalar dt);

void DrVector3RotateY(br_vector3* v, br_angle t);

void CrashCarsTogether(br_scalar dt);

int CrashCarsTogetherSinglePass(br_scalar dt, int pPass, tCollison_data* collide_list);

void BringCarToAGrindingHalt(tCollision_info* car);

int BoundsOverlapTest_car(br_bounds* b1, br_bounds* b2);

int SimpleCarCarCollisionTest(tCollision_info* car1, tCollision_info* car2);

int CollideTwoCarsWithWalls(tCollision_info* car1, tCollision_info* car2, br_scalar dt);

int CollideTwoCarsRepeatedly(tCollision_info* car1, tCollision_info* car2, br_scalar dt);

int CollideTwoCars(tCollision_info* car1, tCollision_info* car2, int pPass);

int GetEdgeEdgeCollisions(br_bounds* pB1, br_bounds* pB2, br_matrix34* pM21, br_matrix34* pM12, br_matrix34* pMo21, br_matrix34* pMo12, br_matrix34* pM1o1, br_vector3* pPoint_list, br_vector3* pNorm_list, int pMax);

int FacePointCarCarCollide(tCollision_info* car1, tCollision_info* car2, br_matrix34* pMms, br_matrix34* pMoms, br_matrix34* pMsos, br_vector3* pPoint_list, br_vector3* pNorm_list, int pMax, int order);

void MungeCarsMass(tCollision_info* pCar, br_scalar pFactor);

void ModifyCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2);

void ResetCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2);

int DoCollide(tCollision_info* car1, tCollision_info* car2, br_vector3* r, br_vector3* n, int k, int pPass, br_matrix34* mat1_to_mat2);

br_scalar TwoPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n);

br_scalar ThreePointCollRecB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n);

br_scalar FourPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n);

int TestForNan(float* f);

void CheckCameraHither();

void SetCarSuspGiveAndHeight(tCar_spec* pCar, br_scalar pFront_give_factor, br_scalar pRear_give_factor, br_scalar pDamping_factor, br_scalar pExtra_front_height, br_scalar pExtra_rear_height);

int TestForCarInSensiblePlace(tCar_spec* car);

int TestOldMats(tCollision_info* c1, tCollision_info* c2, int newmats);

int PullActorFromWorld(br_actor* pActor);

int DoPullActorFromWorld(br_actor* pActor);

void CheckForDeAttachmentOfNonCars(tU32 pTime);

void AdjustNonCar(br_actor* pActor, br_matrix34* pMat);

void PipeSingleNonCar(tCollision_info* c);

int GetPrecalculatedFacesUnderCar(tCar_spec* pCar, tFace_ref** pFace_refs);

br_material* SomeNearbyMaterial();

#endif
