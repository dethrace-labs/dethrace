#ifndef _PIPING_H_
#define _PIPING_H_

#include "br_types.h"
#include "dr_types.h"

extern tU8* gPipe_buffer_start;
extern int gDisable_sound;
extern int gDisable_advance;
extern int gMax_rewind_chunks;
extern float gWall_severity;
extern tPipe_reset_proc* gReset_procs[32];
extern tPiped_registration_snapshot gRegistration_snapshots[5];
extern tPipe_smudge_data* gSmudge_space;
extern tU32 gOldest_time;
extern int gCurrent_snapshot_registration_index;
extern tPipe_chunk* gMr_chunky;
extern tCar_spec* gCar_ptr;
extern br_vector3 gZero_vector;
extern tPipe_chunk_type gReentrancy_array[5];
extern tU32 gLast_time;
extern tPipe_model_geometry_data* gModel_geometry_space;
extern tU32 gEnd_time;
extern tU32 gTrigger_time;
extern int gReentrancy_count;
extern br_vector3 gCar_pos;
extern br_vector3 gReference_pos;
extern br_scalar gMax_distance;
extern tU32 gLoop_abort_time;
extern br_vector3 gWall_impact_point;
extern tU8* gPipe_buffer_working_end;
extern tU32 gYoungest_time;
extern tU8* gPipe_buffer_phys_end;
extern tU8* gLocal_buffer_record_ptr;
extern tU8* gPipe_play_ptr;
extern tU8* gEnd_of_session;
extern tU8* gPipe_record_ptr;
extern tU8* gPipe_buffer_oldest;
extern tU32 gPipe_buffer_size;
extern tU8* gLocal_buffer;
extern tU32 gLocal_buffer_size;

void GetReducedPos(br_vector3* v, tReduced_pos* p);

void SaveReducedPos(tReduced_pos* p, br_vector3* v);

int PipeSearchForwards();

int IsActionReplayAvailable();

int SomeReplayLeft();

void DisablePipedSounds();

void EnablePipedSounds();

tU32 LengthOfSession(tPipe_session* pSession);

void StartPipingSession2(tPipe_chunk_type pThe_type, int pMunge_reentrancy);

void StartPipingSession(tPipe_chunk_type pThe_type);

void EndPipingSession2(int pMunge_reentrancy);

void EndPipingSession();

void AddDataToSession(int pSubject_index, void* pData, tU32 pData_length);

void AddModelGeometryToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex* pCoordinates);

void AddSmudgeToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tSmudged_vertex* pCoordinates);

void AddPedestrianToPipingSession(int pPedestrian_index, br_matrix34* pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset);

void AddSparkToPipingSession(int pSpark_index, br_vector3* pPos, br_vector3* pV);

void AddShrapnelToPipingSession(int pShrapnel_index, br_vector3* pPos, tU16 pAge, br_material* pMaterial);

void AddScreenWobbleToPipingSession(int pWobble_x, int pWobble_y);

void AddGrooveStopToPipingSession(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption);

void AddNonCarToPipingSession(int pIndex, br_actor* pActor);

void AddSmokeToPipingSession(int pIndex, tU8 pType, br_vector3* pPos, br_scalar pRadius, br_scalar pStrength);

void AddSmokeColumnToPipingSession(int pIndex, tCar_spec* pCar, int pVertex, int pColour);

void AddFlameToPipingSession(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z);

void AddSplashToPipingSession(tCollision_info* pCar);

void AddOilSpillToPipingSession(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap);

void AddFrameFinishToPipingSession(tU32 pThe_time);

void AddCarToPipingSession(int pCar_ID, br_matrix34* pCar_mat, br_vector3* pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag);

void AddSoundToPipingSession(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3* pPos);

void AddDamageToPipingSession(int pCar_ID, tS8* pDifferences);

void AddSpecialToPipingSession(tSpecial_type pType);

void AddPedGibToPipingSession(int pIndex, br_matrix34* pTrans, int pSize, int pGib_index, int pPed_index);

void AddCarIncidentToPipingSession(float pSeverity, tCar_spec* pCar, br_vector3* pImpact_point);

void AddPedIncidentToPipingSession(int pPed_index, br_actor* pActor);

void AddWallIncidentToPipingSession(float pSeverity, br_vector3* pImpact_point);

void AddProxRayToPipingSession(int pRay_index, tCar_spec* pCar, tU16 pPed_index, tU32 pTime);

void AddSkidAdjustmentToPipingSession(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index);

void PipeSingleModelGeometry(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex* pCoordinates);

void PipeSinglePedestrian(int pPedestrian_index, br_matrix34* pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset);

void PipeSingleCar(int pCar_ID, br_matrix34* pCar_mat, br_vector3* pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag);

void PipeSingleSound(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3* pPos);

void PipeSingleOilSpill(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap);

void PipeSingleDamage(int pCar_ID, tS8* pDifferences);

void PipeSingleSpecial(tSpecial_type pType);

void PipeSinglePedGib(int pIndex, br_matrix34* pTrans, int pSize, int pGib_index, int pPed_index);

void PipeSingleCarIncident(float pSeverity, tCar_spec* pCar, br_vector3* pImpact_point);

void PipeSinglePedIncident(int pPed_index, br_actor* pActor);

void PipeSingleWallIncident(float pSeverity, br_vector3* pImpact_point);

void PipeSingleScreenShake(int pWobble_x, int pWobble_y);

void PipeSingleGrooveStop(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption);

void PipeFrameFinish();

void PipingFrameReset();

void PipeSingleSkidAdjustment(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index);

void ResetPiping();

void InitialisePiping();

void DisposePiping();

void InitLastDamageArrayEtc();

void ResetCars();

void PipeCarPositions();

void ResetPipePlayToEnd();

void ResetPipePlayToStart();

tU8* GetPipePlayPtr();

void SetPipePlayPtr(tU8* pPtr);

void AdvanceChunkPtr(tPipe_chunk** pChunk, tChunk_subject_index pType);

void ApplyModelGeometry(tPipe_chunk** pChunk);

void DoSmudge(tPipe_chunk** pChunk, int pDir);

void ApplySmudge(tPipe_chunk** pChunk);

void ApplyPedestrian(tPipe_chunk** pChunk);

void ApplySpark(tPipe_chunk** pChunk);

void ApplyShrapnel(tPipe_chunk** pChunk);

void ApplyScreenWobble(tPipe_chunk** pChunk);

void ApplyGrooveStop(tPipe_chunk** pChunk);

void ApplyNonCar(tPipe_chunk** pChunk);

void ApplySmoke(tPipe_chunk** pChunk);

void ApplySmokeColumn(tPipe_chunk** pChunk);

void ApplyFlame(tPipe_chunk** pChunk);

void ApplySplash(tPipe_chunk** pChunk);

void ApplyOilSpill(tPipe_chunk** pChunk, tU32 pStop_time);

void ApplyFrameBoundary(tPipe_chunk** pChunk);

void ApplySound(tPipe_chunk** pChunk);

void ApplyCar(tPipe_chunk** pChunk);

void ApplyDamage(tPipe_chunk** pChunk);

void ApplySpecial(tPipe_chunk** pChunk);

void ApplyPedGib(tPipe_chunk** pChunk);

void ApplyProxRay(tPipe_chunk** pChunk);

void ApplySkidAdjustment(tPipe_chunk** pChunk);

int ApplyPipedSession(tU8** pPtr);

int MoveSessionPointerBackOne(tU8** pPtr);

int MoveSessionPointerForwardOne(tU8** pPtr);

tPipe_chunk* FindPreviousChunk(tU8* pPtr, tPipe_chunk_type pType, tChunk_subject_index pIndex);

void UndoModelGeometry(tPipe_chunk** pChunk);

void UndoSmudge(tPipe_chunk** pChunk);

void UndoPedestrian(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoFrameBoundary(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoCar(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoSound(tPipe_chunk** pChunk);

void UndoDamage(tPipe_chunk** pChunk);

void UndoSpecial(tPipe_chunk** pChunk);

void UndoPedGib(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoSpark(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoShrapnel(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoScreenWobble(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoGrooveStop(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoNonCar(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoSmoke(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoSmokeColumn(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoFlame(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoSplash(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoOilSpill(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

void UndoProxRay(tPipe_chunk** pChunk);

void UndoSkidAdjustment(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk);

int UndoPipedSession(tU8** pPtr);

tU32 FindPrevFrameTime(tU8* pPtr);

void ScanBuffer(tU8** pPtr, tPipe_chunk_type pType, tU32 pDefault_time, int (*pCall_back)(tPipe_chunk*, int, tU32), int (*pTime_check)(tU32));

int CheckSound(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime);

int SoundTimeout(tU32 pTime);

void ScanAndPlaySoundsToBe(tU8* pPtr, tU32 pOldest_time, tU32 pYoungest_time);

int CheckCar(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime);

int CarTimeout(tU32 pTime);

void ScanCarsPositions(tCar_spec* pCar, br_vector3* pSource_pos, br_scalar pMax_distance_sqr, tU32 pOffset_time, tU32 pTime_period, br_vector3* pCar_pos, tU32* pTime_returned);

int CheckIncident(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime);

int GetNextIncident(tU32 pOffset_time, tIncident_type* pIncident_type, float* pSeverity, tIncident_info* pInfo, tU32* pTime_away);

tU32 GetARStartTime();

#endif
