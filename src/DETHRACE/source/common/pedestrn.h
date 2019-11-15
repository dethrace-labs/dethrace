#ifndef _PEDESTRN_H_
#define _PEDESTRN_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 152
// EAX: pModel
void PedModelUpdate(br_model *pModel, br_scalar x0, br_scalar y0, br_scalar x1, br_scalar y1, br_scalar x2, br_scalar y2, br_scalar x3, br_scalar y3);

// Offset: 152
// Size: 98
// EAX: pActor
int ActorIsPedestrian(br_actor *pActor);

// Offset: 252
// Size: 79
// EAX: pActor
br_scalar PedHeightFromActor(br_actor *pActor);

// Offset: 332
// Size: 52
// EAX: pActor
int GetPedestrianValue(br_actor *pActor);

// Offset: 384
// Size: 88
// EAX: pActor
int PedestrianActorIsPerson(br_actor *pActor);

// Offset: 472
// Size: 87
// EAX: pIndex
br_actor* GetPedestrianActor(int pIndex);

// Offset: 560
// Size: 97
// EAX: pActor
// EDX: pFlipped
br_pixelmap* GetPedestrianTexture(br_actor *pActor, int *pFlipped);

// Offset: 660
// Size: 153
void TogglePedestrians();

// Offset: 816
// Size: 918
void InitPedGibs();

// Offset: 1736
// Size: 240
// EAX: pActor
void SetPedMaterialForRender(br_actor *pActor);

// Offset: 1976
// Size: 158
void PedCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, void *pRender_data, br_uint_8 pStyle, int pOn_screen);

// Offset: 2136
// Size: 516
void InitPeds();

// Offset: 2652
// Size: 275
// EAX: pActor
void MungeModelSize(br_actor *pActor, br_scalar pScaling_factor);

// Offset: 2928
// Size: 1468
// EAX: pPedestrian
int BurstPedestrian(tPedestrian_data *pPedestrian, float pSplattitudinalitude, int pAllow_explosion);

// Offset: 4396
// Size: 137
void ResetAllPedGibs();

// Offset: 4536
// Size: 200
// EAX: pIndex
// EDX: pSize
// EBX: pGib_index
// ECX: pPed_index
void AdjustPedGib(int pIndex, int pSize, int pGib_index, int pPed_index, br_matrix34 *pTrans);

// Offset: 4736
// Size: 810
// EAX: pFrame_period
void MungePedGibs(tU32 pFrame_period);

// Offset: 5548
// Size: 267
// EAX: pPedestrian
void KillPedestrian(tPedestrian_data *pPedestrian);

// Offset: 5816
// Size: 302
// EAX: pPedestrian
// EDX: pPixelmap
// EBX: pHeight
// ECX: pWidth
void CalcPedWidthNHeight(tPedestrian_data *pPedestrian, br_pixelmap *pPixelmap, br_scalar *pHeight, br_scalar *pWidth);

// Offset: 6120
// Size: 1942
// EAX: pPedestrian
int PedestrianNextInstruction(tPedestrian_data *pPedestrian, float pDanger_level, int pPosition_explicitly, int pMove_pc);

// Offset: 8064
// Size: 464
// EAX: pPedestrian
// EDX: pAction_changed
void MungePedestrianSequence(tPedestrian_data *pPedestrian, int pAction_changed);

// Offset: 8528
// Size: 166
// EAX: pPedestrian
void DetachPedFromCar(tPedestrian_data *pPedestrian);

// Offset: 8696
// Size: 117
// EAX: pPedestrian
void SetPedPos(tPedestrian_data *pPedestrian);

// Offset: 8816
// Size: 59
// EAX: pActor
void DetachPedActorFromCar(br_actor *pActor);

// Offset: 8876
// Size: 1317
// EAX: pPedestrian
void MungePedestrianFrames(tPedestrian_data *pPedestrian);

// Offset: 10196
// Size: 1288
// EAX: pPedestrian
void MungePedModel(tPedestrian_data *pPedestrian);

// Offset: 11484
// Size: 408
// EAX: pPedestrian
// EDX: pAction_index
// EBX: pRedo_frames_etc
void ChangeActionTo(tPedestrian_data *pPedestrian, int pAction_index, int pRedo_frames_etc);

// Offset: 11892
// Size: 589
// EAX: pPedestrian
int MungePedestrianAction(tPedestrian_data *pPedestrian, float pDanger_level);

// Offset: 12484
// Size: 105
void MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo();

// Offset: 12592
// Size: 2334
// EAX: pPedestrian
void MungePedestrianPath(tPedestrian_data *pPedestrian, float pDanger_level, br_vector3 *pDanger_direction);

// Offset: 14928
// Size: 764
// EAX: pPedestrian
// EDX: pDanger_direction
float CalcPedestrianDangerLevel(tPedestrian_data *pPedestrian, br_vector3 *pDanger_direction);

// Offset: 15692
// Size: 972
// EAX: pPedestrian
// EDX: pCar
// EBX: pCar_actor
tPed_hit_position MoveToEdgeOfCar(tPedestrian_data *pPedestrian, tCollision_info *pCar, br_actor *pCar_actor, br_scalar pPed_x, br_scalar pPed_z, br_scalar pCar_bounds_min_x, br_scalar pCar_bounds_max_x, br_scalar pCar_bounds_min_z, br_scalar pCar_bounds_max_z, br_vector3 *pMin_ped_bounds_car, br_vector3 *pMax_ped_bounds_car);

// Offset: 16664
// Size: 101
void CheckLastPed();

// Offset: 16768
// Size: 319
// EAX: pCar
// EDX: pPed_car
int BloodyWheels(tCar_spec *pCar, br_vector3 *pPed_car, br_scalar pSize, br_vector3 *pPed_glob);

// Offset: 17088
// Size: 142
// EAX: pPedestrian
// EDX: pCar
int FancyATossOffMate(tPedestrian_data *pPedestrian, tCollision_info *pCar, float pImpact_speed);

// Offset: 17232
// Size: 5870
// EAX: pPedestrian
void CheckPedestrianDeathScenario(tPedestrian_data *pPedestrian);

// Offset: 23104
// Size: 769
// EAX: pPedestrian
// EDX: pIndex
void SendPedestrian(tPedestrian_data *pPedestrian, int pIndex);

// Offset: 23876
// Size: 1162
// EAX: pPedestrian
// EDX: pIndex
void DoPedestrian(tPedestrian_data *pPedestrian, int pIndex);

// Offset: 25040
// Size: 767
// EAX: pIndex
// EDX: pAction_index
// EBX: pFrame_index
// ECX: pHit_points
void AdjustPedestrian(int pIndex, int pAction_index, int pFrame_index, int pHit_points, int pDone_initial, tU16 pParent, br_actor *pParent_actor, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset, br_vector3 *pTrans);

// Offset: 25808
// Size: 194
// EAX: pFirst_vertex
// EDX: pPoint
void SquirtPathVertex(br_vertex *pFirst_vertex, br_vector3 *pPoint);

// Offset: 26004
// Size: 92
void ResetAllPedestrians();

// Offset: 26096
// Size: 476
// EAX: pPedestrian
void GroundPedestrian(tPedestrian_data *pPedestrian);

// Offset: 26572
// Size: 483
// EAX: pPedestrian
// EDX: pAnimate
void RevivePedestrian(tPedestrian_data *pPedestrian, int pAnimate);

// Offset: 27056
// Size: 1078
// EAX: pFrame_period
void MungePedestrians(tU32 pFrame_period);

// Offset: 28136
// Size: 201
void RespawnPedestrians();

// Offset: 28340
// Size: 45
int GetPedCount();

// Offset: 28388
// Size: 260
// EAX: pIndex
// EDX: pPos
int GetPedPosition(int pIndex, br_vector3 *pPos);

// Offset: 28648
// Size: 2930
// EAX: pG
// EDX: pInstructions
// EBX: pInstruc_count
// ECX: pInit_instruc
void CreatePedestrian(FILE *pG, tPedestrian_instruction *pInstructions, int pInstruc_count, int pInit_instruc, int pRef_num, int pForce_read);

// Offset: 31580
// Size: 73
void ResetProxRay();

// Offset: 31656
// Size: 34
void PedMaterialFromHell();

// Offset: 31692
// Size: 34
void ResetPedMaterial();

// Offset: 31728
// Size: 1879
// EAX: pF
// EDX: pSubs_count
// EBX: pSubs_array
void LoadInPedestrians(FILE *pF, int pSubs_count, tPed_subs *pSubs_array);

// Offset: 33608
// Size: 1313
// EAX: pInstructions
// EDX: pInstruc_count
// EBX: pInit_instruc
br_actor* BuildPedPaths(tPedestrian_instruction *pInstructions, int pInstruc_count, int pInit_instruc);

// Offset: 34924
// Size: 1161
void WriteOutPeds();

// Offset: 36088
// Size: 213
void AddPed();

// Offset: 36304
// Size: 296
// EAX: pRef_num
void NewPed(int pRef_num);

// Offset: 36600
// Size: 113
void RemoveCurrentPedPath();

// Offset: 36716
// Size: 89
void ScrubPedestrian();

// Offset: 36808
// Size: 134
void TogglePedDetect();

// Offset: 36944
// Size: 41
void NewPed0();

// Offset: 36988
// Size: 44
void NewPed1();

// Offset: 37032
// Size: 44
void NewPed2();

// Offset: 37076
// Size: 44
void NewPed3();

// Offset: 37120
// Size: 44
void NewPed4();

// Offset: 37164
// Size: 44
void NewPed5();

// Offset: 37208
// Size: 44
void NewPed6();

// Offset: 37252
// Size: 44
void NewPed7();

// Offset: 37296
// Size: 44
void NewPed8();

// Offset: 37340
// Size: 44
void NewPed9();

// Offset: 37384
// Size: 44
void NewPed0B();

// Offset: 37428
// Size: 44
void NewPed1B();

// Offset: 37472
// Size: 44
void NewPed2B();

// Offset: 37516
// Size: 44
void NewPed3B();

// Offset: 37560
// Size: 44
void NewPed4B();

// Offset: 37604
// Size: 44
void NewPed5B();

// Offset: 37648
// Size: 44
void NewPed6B();

// Offset: 37692
// Size: 44
void NewPed7B();

// Offset: 37736
// Size: 44
void NewPed8B();

// Offset: 37780
// Size: 44
void NewPed9B();

// Offset: 37824
// Size: 100
void MungeShowPedPath();

// Offset: 37924
// Size: 101
void DropPedPoint2();

// Offset: 38028
// Size: 67
void DropPedPoint();

// Offset: 38096
// Size: 77
void DropInitPedPoint();

// Offset: 38176
// Size: 126
void DropPedPointAir2();

// Offset: 38304
// Size: 67
void DropPedPointAir();

// Offset: 38372
// Size: 77
void DropInitPedPointAir();

// Offset: 38452
// Size: 77
br_uint_32 KillActorsModel(br_actor *pActor, void *pArg);

// Offset: 38532
// Size: 154
void DisposePedPaths();

// Offset: 38688
// Size: 417
// EAX: pPed_index
// EDX: pPoint_index
void GetPedPos(int *pPed_index, int *pPoint_index);

// Offset: 39108
// Size: 105
void ShowPedPos();

// Offset: 39216
// Size: 381
void ShowPedPaths();

// Offset: 39600
// Size: 104
void PullPedPoint();

// Offset: 39704
// Size: 141
void PullPedPointAir();

// Offset: 39848
// Size: 135
void DeletePedPath();

// Offset: 39984
// Size: 89
void DeletePedPoint();

// Offset: 40076
// Size: 589
void DisposePedestrians();

// Offset: 40668
// Size: 364
void DoPedReport();

// Offset: 41032
// Size: 1223
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void RenderProximityRays(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world, tU32 pTime);

// Offset: 42256
// Size: 145
// EAX: pRay_index
// EDX: pCar_ID
// EBX: pPed_index
// ECX: pTime
void AdjustProxRay(int pRay_index, tU16 pCar_ID, tU16 pPed_index, tU32 pTime);

// Offset: 42404
// Size: 1200
// EAX: pContents
// EDX: pMessage
// EBX: pReceive_time
void ReceivedPedestrian(tNet_contents *pContents, tNet_message *pMessage, tU32 pReceive_time);

// Offset: 43604
// Size: 115
// EAX: pPlayer
void SendAllPedestrianPositions(tPlayer_ID pPlayer);

#endif
