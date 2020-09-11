#ifndef _PEDESTRN_H_
#define _PEDESTRN_H_

#include "br_types.h"
#include "dr_types.h"

extern int gMin_respawn_time;
extern int gRespawn_variance;
extern float gZombie_factor;

void PedModelUpdate(br_model* pModel, br_scalar x0, br_scalar y0, br_scalar x1, br_scalar y1, br_scalar x2, br_scalar y2, br_scalar x3, br_scalar y3);

int ActorIsPedestrian(br_actor* pActor);

br_scalar PedHeightFromActor(br_actor* pActor);

int GetPedestrianValue(br_actor* pActor);

int PedestrianActorIsPerson(br_actor* pActor);

br_actor* GetPedestrianActor(int pIndex);

br_pixelmap* GetPedestrianTexture(br_actor* pActor, int* pFlipped);

void TogglePedestrians();

void InitPedGibs();

void SetPedMaterialForRender(br_actor* pActor);

void PedCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, void* pRender_data, br_uint_8 pStyle, int pOn_screen);

void InitPeds();

void MungeModelSize(br_actor* pActor, br_scalar pScaling_factor);

int BurstPedestrian(tPedestrian_data* pPedestrian, float pSplattitudinalitude, int pAllow_explosion);

void ResetAllPedGibs();

void AdjustPedGib(int pIndex, int pSize, int pGib_index, int pPed_index, br_matrix34* pTrans);

void MungePedGibs(tU32 pFrame_period);

void KillPedestrian(tPedestrian_data* pPedestrian);

void CalcPedWidthNHeight(tPedestrian_data* pPedestrian, br_pixelmap* pPixelmap, br_scalar* pHeight, br_scalar* pWidth);

int PedestrianNextInstruction(tPedestrian_data* pPedestrian, float pDanger_level, int pPosition_explicitly, int pMove_pc);

void MungePedestrianSequence(tPedestrian_data* pPedestrian, int pAction_changed);

void DetachPedFromCar(tPedestrian_data* pPedestrian);

void SetPedPos(tPedestrian_data* pPedestrian);

void DetachPedActorFromCar(br_actor* pActor);

void MungePedestrianFrames(tPedestrian_data* pPedestrian);

void MungePedModel(tPedestrian_data* pPedestrian);

void ChangeActionTo(tPedestrian_data* pPedestrian, int pAction_index, int pRedo_frames_etc);

int MungePedestrianAction(tPedestrian_data* pPedestrian, float pDanger_level);

void MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo();

void MungePedestrianPath(tPedestrian_data* pPedestrian, float pDanger_level, br_vector3* pDanger_direction);

float CalcPedestrianDangerLevel(tPedestrian_data* pPedestrian, br_vector3* pDanger_direction);

tPed_hit_position MoveToEdgeOfCar(tPedestrian_data* pPedestrian, tCollision_info* pCar, br_actor* pCar_actor, br_scalar pPed_x, br_scalar pPed_z, br_scalar pCar_bounds_min_x, br_scalar pCar_bounds_max_x, br_scalar pCar_bounds_min_z, br_scalar pCar_bounds_max_z, br_vector3* pMin_ped_bounds_car, br_vector3* pMax_ped_bounds_car);

void CheckLastPed();

int BloodyWheels(tCar_spec* pCar, br_vector3* pPed_car, br_scalar pSize, br_vector3* pPed_glob);

int FancyATossOffMate(tPedestrian_data* pPedestrian, tCollision_info* pCar, float pImpact_speed);

void CheckPedestrianDeathScenario(tPedestrian_data* pPedestrian);

void SendPedestrian(tPedestrian_data* pPedestrian, int pIndex);

void DoPedestrian(tPedestrian_data* pPedestrian, int pIndex);

void AdjustPedestrian(int pIndex, int pAction_index, int pFrame_index, int pHit_points, int pDone_initial, tU16 pParent, br_actor* pParent_actor, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset, br_vector3* pTrans);

void SquirtPathVertex(br_vertex* pFirst_vertex, br_vector3* pPoint);

void ResetAllPedestrians();

void GroundPedestrian(tPedestrian_data* pPedestrian);

void RevivePedestrian(tPedestrian_data* pPedestrian, int pAnimate);

void MungePedestrians(tU32 pFrame_period);

void RespawnPedestrians();

int GetPedCount();

int GetPedPosition(int pIndex, br_vector3* pPos);

void CreatePedestrian(FILE* pG, tPedestrian_instruction* pInstructions, int pInstruc_count, int pInit_instruc, int pRef_num, int pForce_read);

void ResetProxRay();

void PedMaterialFromHell();

void ResetPedMaterial();

void LoadInPedestrians(FILE* pF, int pSubs_count, tPed_subs* pSubs_array);

br_actor* BuildPedPaths(tPedestrian_instruction* pInstructions, int pInstruc_count, int pInit_instruc);

void WriteOutPeds();

void AddPed();

void NewPed(int pRef_num);

void RemoveCurrentPedPath();

void ScrubPedestrian();

void TogglePedDetect();

void NewPed0();

void NewPed1();

void NewPed2();

void NewPed3();

void NewPed4();

void NewPed5();

void NewPed6();

void NewPed7();

void NewPed8();

void NewPed9();

void NewPed0B();

void NewPed1B();

void NewPed2B();

void NewPed3B();

void NewPed4B();

void NewPed5B();

void NewPed6B();

void NewPed7B();

void NewPed8B();

void NewPed9B();

void MungeShowPedPath();

void DropPedPoint2();

void DropPedPoint();

void DropInitPedPoint();

void DropPedPointAir2();

void DropPedPointAir();

void DropInitPedPointAir();

br_uint_32 KillActorsModel(br_actor* pActor, void* pArg);

void DisposePedPaths();

void GetPedPos(int* pPed_index, int* pPoint_index);

void ShowPedPos();

void ShowPedPaths();

void PullPedPoint();

void PullPedPointAir();

void DeletePedPath();

void DeletePedPoint();

void DisposePedestrians();

void DoPedReport();

void RenderProximityRays(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, tU32 pTime);

void AdjustProxRay(int pRay_index, tU16 pCar_ID, tU16 pPed_index, tU32 pTime);

void ReceivedPedestrian(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time);

void SendAllPedestrianPositions(tPlayer_ID pPlayer);

#endif
