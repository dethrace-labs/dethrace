#ifndef _PEDESTRN_H_
#define _PEDESTRN_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern int gDetect_peds;
extern int gReally_stupid_ped_bug_enable;
extern int gPed_sound_disable;
extern int gVesuvians_last_time;
extern int gSend_peds;
extern tU32 gLast_ped_message_send;
extern tPedestrian_instruction* gInitial_instruction;
extern char* gRate_commands[3];
extern char* gCollide_commands[1];
extern char* gInstruc_commands[10];
extern float gMin_ped_gib_speeds[4];
extern float gPed_gib_distrib[4];
extern float gPed_gib_speeds[4];
extern int gPed_size_counts[4];
extern char* gPed_gib_names[4][5];
extern char* gPed_geb_names[4][5];
extern int gPed_gib_maxes[4][5];
extern br_scalar gExploding_ped_scale[3];
extern br_vector3 gZero_v__pedestrn; // suffix added to avoid duplicate symbol
extern int gPed_instruc_count;
extern int gPed_count;
extern br_actor* gPath_actor;
extern br_actor* gCurrent_ped_path_actor;
extern int gPedestrians_on;
extern int gVesuvian_corpses;
extern br_material* gPed_material;
extern int gPed_gib_counts[4][5];
extern tPedestrian_instruction gPed_instrucs[100];
extern tPed_gib gPed_gibs[30];
extern tPed_gib_materials gPed_gib_materials[4];
extern tProximity_ray gProximity_rays[20];
extern int gPed_colliding;
extern float gZombie_factor;
extern int gRespawn_variance;
extern br_scalar gPed_scale_factor;
extern int gTotal_peds;
extern int gPedestrian_harvest;
extern br_vector3 gPed_pos_camera;
extern int gMin_respawn_time;
extern br_material* gPath_mat_calc;
extern float gPedestrian_speed_factor;
extern int gExploding_pedestrians;
extern int gBlind_pedestrians;
extern br_material* gPath_mat_normal;
extern br_material* gInit_pos_mat_calc;
extern int gPed_other;
extern int gAttracted_pedestrians;
extern int gPed_ref_num;
extern br_scalar gMax_distance_squared;
extern br_model* gPed_model;
extern float gDanger_level;
extern br_vector3 gDanger_direction;
extern int gInit_ped_instruc;
extern int gCurrent_lollipop_index;
extern int gVesuvians_this_time;
extern int gNumber_of_ped_gibs;
extern tPedestrian_data* gFlag_waving_bastard;
extern int gNumber_of_pedestrians;
extern br_pixelmap* gProx_ray_shade_table;
extern tPedestrian_data* gPedestrian_array;
extern tU32 gLast_ped_splat_time;
extern int gCurrent_ped_multiplier;

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

intptr_t KillActorsModel(br_actor* pActor, void* pArg);

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
