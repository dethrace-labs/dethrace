#include "piping.h"

tPiped_registration_snapshot gRegistration_snapshots[5];
tPipe_reset_proc *gReset_procs[32];
float gWall_severity;
tPipe_smudge_data *gSmudge_space;
tU32 gOldest_time;
int gCurrent_snapshot_registration_index;
tPipe_chunk *gMr_chunky;
int gMax_rewind_chunks;
int gDisable_sound;
tCar_spec *gCar_ptr;
br_vector3 gZero_vector;
tPipe_chunk_type gReentrancy_array[5];
tU32 gLast_time;
tPipe_model_geometry_data *gModel_geometry_space;
tU32 gEnd_time;
tU32 gTrigger_time;
int gReentrancy_count;
int gDisable_advance;
br_vector3 gCar_pos;
br_vector3 gReference_pos;
br_scalar gMax_distance;
tU32 gLoop_abort_time;
br_vector3 gWall_impact_point;
tU8 *gPipe_buffer_working_end;
tU32 gYoungest_time;
tU8 *gPipe_buffer_phys_end;
tU8 *gLocal_buffer_record_ptr;
tU8 *gPipe_play_ptr;
tU8 *gEnd_of_session;
tU8 *gPipe_record_ptr;
tU8 *gPipe_buffer_oldest;
tU32 gPipe_buffer_size;
tU8 *gLocal_buffer;
tU32 gLocal_buffer_size;
tU8 *gPipe_buffer_start;

// Offset: 0
// Size: 139
// EAX: v
// EDX: p
void GetReducedPos(br_vector3 *v, tReduced_pos *p) {
}

// Offset: 140
// Size: 171
// EAX: p
// EDX: v
void SaveReducedPos(tReduced_pos *p, br_vector3 *v) {
    br_vector3 tv;
}

// Offset: 312
// Size: 185
int PipeSearchForwards() {
}

// Offset: 500
// Size: 68
int IsActionReplayAvailable() {
}

// Offset: 568
// Size: 105
int SomeReplayLeft() {
}

// Offset: 676
// Size: 44
void DisablePipedSounds() {
}

// Offset: 720
// Size: 44
void EnablePipedSounds() {
}

// Offset: 764
// Size: 1165
// EAX: pSession
tU32 LengthOfSession(tPipe_session *pSession) {
    int i;
    tU32 running_total;
    tPipe_chunk *the_chunk;
}

// Offset: 1932
// Size: 196
// EAX: pThe_type
// EDX: pMunge_reentrancy
void StartPipingSession2(tPipe_chunk_type pThe_type, int pMunge_reentrancy) {
}

// Offset: 2128
// Size: 50
// EAX: pThe_type
void StartPipingSession(tPipe_chunk_type pThe_type) {
}

// Offset: 2180
// Size: 390
// EAX: pMunge_reentrancy
void EndPipingSession2(int pMunge_reentrancy) {
    int a;
}

// Offset: 2572
// Size: 44
void EndPipingSession() {
}

// Offset: 2616
// Size: 168
// EAX: pSubject_index
// EDX: pData
// EBX: pData_length
void AddDataToSession(int pSubject_index, void *pData, tU32 pData_length) {
    tU32 temp_buffer_size;
    int variable_for_breaking_on;
}

// Offset: 2784
// Size: 140
// EAX: pCar_ID
// EDX: pModel_index
// EBX: pVertex_count
// ECX: pCoordinates
void AddModelGeometryToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex *pCoordinates) {
    tU32 data_size;
}

// Offset: 2924
// Size: 140
// EAX: pCar_ID
// EDX: pModel_index
// EBX: pVertex_count
// ECX: pCoordinates
void AddSmudgeToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tSmudged_vertex *pCoordinates) {
    tU32 data_size;
}

// Offset: 3064
// Size: 208
// EAX: pPedestrian_index
// EDX: pTrans
// EBX: pAction_index
// ECX: pFrame_index
void AddPedestrianToPipingSession(int pPedestrian_index, br_matrix34 *pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset) {
    tPipe_pedestrian_data data;
    tU32 data_size;
}

// Offset: 3272
// Size: 107
// EAX: pSpark_index
// EDX: pPos
// EBX: pV
void AddSparkToPipingSession(int pSpark_index, br_vector3 *pPos, br_vector3 *pV) {
    tPipe_spark_data data;
}

// Offset: 3380
// Size: 118
// EAX: pShrapnel_index
// EDX: pPos
// EBX: pAge
// ECX: pMaterial
void AddShrapnelToPipingSession(int pShrapnel_index, br_vector3 *pPos, tU16 pAge, br_material *pMaterial) {
    tPipe_shrapnel_data data;
    tU32 data_size;
}

// Offset: 3500
// Size: 65
// EAX: pWobble_x
// EDX: pWobble_y
void AddScreenWobbleToPipingSession(int pWobble_x, int pWobble_y) {
    tPipe_screen_shake_data data;
}

// Offset: 3568
// Size: 98
// EAX: pGroove_index
// EDX: pMatrix
// EBX: pPath_interrupt
// ECX: pObject_interrupt
void AddGrooveStopToPipingSession(int pGroove_index, br_matrix34 *pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    tPipe_groove_stop_data data;
}

// Offset: 3668
// Size: 79
// EAX: pIndex
// EDX: pActor
void AddNonCarToPipingSession(int pIndex, br_actor *pActor) {
    tPipe_non_car_data data;
}

// Offset: 3748
// Size: 127
// EAX: pIndex
// EDX: pType
// EBX: pPos
void AddSmokeToPipingSession(int pIndex, tU8 pType, br_vector3 *pPos, br_scalar pRadius, br_scalar pStrength) {
    tPipe_smoke_data data;
}

// Offset: 3876
// Size: 87
// EAX: pIndex
// EDX: pCar
// EBX: pVertex
// ECX: pColour
void AddSmokeColumnToPipingSession(int pIndex, tCar_spec *pCar, int pVertex, int pColour) {
    tPipe_smoke_column_data data;
}

// Offset: 3964
// Size: 87
// EAX: pIndex
// EDX: pFrame_count
void AddFlameToPipingSession(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z) {
    tPipe_flame_data data;
}

// Offset: 4052
// Size: 122
// EAX: pCar
void AddSplashToPipingSession(tCollision_info *pCar) {
    tPipe_splash_data data;
}

// Offset: 4176
// Size: 134
// EAX: pIndex
// EDX: pMat
void AddOilSpillToPipingSession(int pIndex, br_matrix34 *pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap) {
    tPipe_oil_spill_data data;
}

// Offset: 4312
// Size: 58
// EAX: pThe_time
void AddFrameFinishToPipingSession(tU32 pThe_time) {
    tPipe_frame_boundary_data data;
}

// Offset: 4372
// Size: 357
// EAX: pCar_ID
// EDX: pCar_mat
// EBX: pCar_velocity
void AddCarToPipingSession(int pCar_ID, br_matrix34 *pCar_mat, br_vector3 *pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag) {
    tPipe_car_data data;
}

// Offset: 4732
// Size: 129
// EAX: pOutlet
// EDX: pSound_index
// EBX: pL_volume
// ECX: pR_volume
void AddSoundToPipingSession(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3 *pPos) {
    tPipe_sound_data data;
}

// Offset: 4864
// Size: 94
// EAX: pCar_ID
// EDX: pDifferences
void AddDamageToPipingSession(int pCar_ID, tS8 *pDifferences) {
    tPipe_damage_data data;
    int i;
}

// Offset: 4960
// Size: 53
// EAX: pType
void AddSpecialToPipingSession(tSpecial_type pType) {
    tPipe_special_data data;
}

// Offset: 5016
// Size: 90
// EAX: pIndex
// EDX: pTrans
// EBX: pSize
// ECX: pGib_index
void AddPedGibToPipingSession(int pIndex, br_matrix34 *pTrans, int pSize, int pGib_index, int pPed_index) {
    tPipe_ped_gib_data data;
}

// Offset: 5108
// Size: 83
void AddCarIncidentToPipingSession(float pSeverity, tCar_spec *pCar, br_vector3 *pImpact_point) {
    tPipe_incident_data data;
}

// Offset: 5192
// Size: 73
// EAX: pPed_index
// EDX: pActor
void AddPedIncidentToPipingSession(int pPed_index, br_actor *pActor) {
    tPipe_incident_data data;
}

// Offset: 5268
// Size: 69
void AddWallIncidentToPipingSession(float pSeverity, br_vector3 *pImpact_point) {
    tPipe_incident_data data;
}

// Offset: 5340
// Size: 83
// EAX: pRay_index
// EDX: pCar
// EBX: pPed_index
// ECX: pTime
void AddProxRayToPipingSession(int pRay_index, tCar_spec *pCar, tU16 pPed_index, tU32 pTime) {
    tPipe_prox_ray_data data;
}

// Offset: 5424
// Size: 74
// EAX: pSkid_num
// EDX: pMatrix
// EBX: pMaterial_index
void AddSkidAdjustmentToPipingSession(int pSkid_num, br_matrix34 *pMatrix, int pMaterial_index) {
    tPipe_skid_adjustment adjustment;
}

// Offset: 5500
// Size: 75
// EAX: pCar_ID
// EDX: pModel_index
// EBX: pVertex_count
// ECX: pCoordinates
void PipeSingleModelGeometry(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex *pCoordinates) {
}

// Offset: 5576
// Size: 116
// EAX: pPedestrian_index
// EDX: pTrans
// EBX: pAction_index
// ECX: pFrame_index
void PipeSinglePedestrian(int pPedestrian_index, br_matrix34 *pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset) {
}

// Offset: 5692
// Size: 141
// EAX: pCar_ID
// EDX: pCar_mat
// EBX: pCar_velocity
void PipeSingleCar(int pCar_ID, br_matrix34 *pCar_mat, br_vector3 *pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag) {
}

// Offset: 5836
// Size: 102
// EAX: pOutlet
// EDX: pSound_index
// EBX: pL_volume
// ECX: pR_volume
void PipeSingleSound(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3 *pPos) {
}

// Offset: 5940
// Size: 104
// EAX: pIndex
// EDX: pMat
void PipeSingleOilSpill(int pIndex, br_matrix34 *pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap) {
}

// Offset: 6044
// Size: 64
// EAX: pCar_ID
// EDX: pDifferences
void PipeSingleDamage(int pCar_ID, tS8 *pDifferences) {
}

// Offset: 6108
// Size: 60
// EAX: pType
void PipeSingleSpecial(tSpecial_type pType) {
}

// Offset: 6168
// Size: 78
// EAX: pIndex
// EDX: pTrans
// EBX: pSize
// ECX: pGib_index
void PipeSinglePedGib(int pIndex, br_matrix34 *pTrans, int pSize, int pGib_index, int pPed_index) {
}

// Offset: 6248
// Size: 73
void PipeSingleCarIncident(float pSeverity, tCar_spec *pCar, br_vector3 *pImpact_point) {
}

// Offset: 6324
// Size: 74
// EAX: pPed_index
// EDX: pActor
void PipeSinglePedIncident(int pPed_index, br_actor *pActor) {
}

// Offset: 6400
// Size: 69
void PipeSingleWallIncident(float pSeverity, br_vector3 *pImpact_point) {
}

// Offset: 6472
// Size: 64
// EAX: pWobble_x
// EDX: pWobble_y
void PipeSingleScreenShake(int pWobble_x, int pWobble_y) {
}

// Offset: 6536
// Size: 92
// EAX: pGroove_index
// EDX: pMatrix
// EBX: pPath_interrupt
// ECX: pObject_interrupt
void PipeSingleGrooveStop(int pGroove_index, br_matrix34 *pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
}

// Offset: 6628
// Size: 113
void PipeFrameFinish() {
}

// Offset: 6744
// Size: 86
void PipingFrameReset() {
}

// Offset: 6832
// Size: 68
// EAX: pSkid_num
// EDX: pMatrix
// EBX: pMaterial_index
void PipeSingleSkidAdjustment(int pSkid_num, br_matrix34 *pMatrix, int pMaterial_index) {
}

// Offset: 6900
// Size: 84
void ResetPiping() {
}

// Offset: 6984
// Size: 234
void InitialisePiping() {
}

// Offset: 7220
// Size: 129
void DisposePiping() {
}

// Offset: 7352
// Size: 227
void InitLastDamageArrayEtc() {
    int j;
    int cat;
    int car_count;
    tCar_spec *car;
}

// Offset: 7580
// Size: 169
void ResetCars() {
    int cat;
    int i;
    int car_count;
}

// Offset: 7752
// Size: 601
void PipeCarPositions() {
    int cat;
    int i;
    int j;
    int car_count;
    int session_started;
    int difference_found;
    tS8 damage_deltas[12];
}

// Offset: 8356
// Size: 44
void ResetPipePlayToEnd() {
}

// Offset: 8400
// Size: 44
void ResetPipePlayToStart() {
}

// Offset: 8444
// Size: 45
tU8* GetPipePlayPtr() {
}

// Offset: 8492
// Size: 45
// EAX: pPtr
void SetPipePlayPtr(tU8 *pPtr) {
}

// Offset: 8540
// Size: 538
// EAX: pChunk
// EDX: pType
void AdvanceChunkPtr(tPipe_chunk **pChunk, tChunk_subject_index pType) {
    tPipe_chunk *old_chunk;
}

// Offset: 9080
// Size: 368
// EAX: pChunk
void ApplyModelGeometry(tPipe_chunk **pChunk) {
    int i;
    br_model *model_ptr;
    tCar_spec *car;
}

// Offset: 9448
// Size: 422
// EAX: pChunk
// EDX: pDir
void DoSmudge(tPipe_chunk **pChunk, int pDir) {
    int i;
    int v;
    tU8 inc;
    br_model *model_ptr;
    tCar_spec *car;
    int group;
}

// Offset: 9872
// Size: 63
// EAX: pChunk
void ApplySmudge(tPipe_chunk **pChunk) {
}

// Offset: 9936
// Size: 184
// EAX: pChunk
void ApplyPedestrian(tPipe_chunk **pChunk) {
}

// Offset: 10120
// Size: 84
// EAX: pChunk
void ApplySpark(tPipe_chunk **pChunk) {
}

// Offset: 10204
// Size: 99
// EAX: pChunk
void ApplyShrapnel(tPipe_chunk **pChunk) {
}

// Offset: 10304
// Size: 77
// EAX: pChunk
void ApplyScreenWobble(tPipe_chunk **pChunk) {
}

// Offset: 10384
// Size: 108
// EAX: pChunk
void ApplyGrooveStop(tPipe_chunk **pChunk) {
}

// Offset: 10492
// Size: 71
// EAX: pChunk
void ApplyNonCar(tPipe_chunk **pChunk) {
}

// Offset: 10564
// Size: 158
// EAX: pChunk
void ApplySmoke(tPipe_chunk **pChunk) {
    br_vector3 pos;
}

// Offset: 10724
// Size: 170
// EAX: pChunk
void ApplySmokeColumn(tPipe_chunk **pChunk) {
}

// Offset: 10896
// Size: 111
// EAX: pChunk
void ApplyFlame(tPipe_chunk **pChunk) {
}

// Offset: 11008
// Size: 192
// EAX: pChunk
void ApplySplash(tPipe_chunk **pChunk) {
    tCar_spec *c;
}

// Offset: 11200
// Size: 130
// EAX: pChunk
// EDX: pStop_time
void ApplyOilSpill(tPipe_chunk **pChunk, tU32 pStop_time) {
}

// Offset: 11332
// Size: 79
// EAX: pChunk
void ApplyFrameBoundary(tPipe_chunk **pChunk) {
    tU32 result;
}

// Offset: 11412
// Size: 441
// EAX: pChunk
void ApplySound(tPipe_chunk **pChunk) {
}

// Offset: 11856
// Size: 962
// EAX: pChunk
void ApplyCar(tPipe_chunk **pChunk) {
    tCar_spec *car;
    br_vector3 com_offset_c;
    br_vector3 com_offset_w;
}

// Offset: 12820
// Size: 176
// EAX: pChunk
void ApplyDamage(tPipe_chunk **pChunk) {
    tCar_spec *car;
    int i;
}

// Offset: 12996
// Size: 166
// EAX: pChunk
void ApplySpecial(tPipe_chunk **pChunk) {
}

// Offset: 13164
// Size: 120
// EAX: pChunk
void ApplyPedGib(tPipe_chunk **pChunk) {
}

// Offset: 13284
// Size: 106
// EAX: pChunk
void ApplyProxRay(tPipe_chunk **pChunk) {
}

// Offset: 13392
// Size: 84
// EAX: pChunk
void ApplySkidAdjustment(tPipe_chunk **pChunk) {
}

// Offset: 13476
// Size: 610
// EAX: pPtr
int ApplyPipedSession(tU8 **pPtr) {
    int i;
    int return_value;
    tPipe_chunk *chunk_ptr;
    tPipe_chunk_type chunk_type;
}

// Offset: 14088
// Size: 129
// EAX: pPtr
int MoveSessionPointerBackOne(tU8 **pPtr) {
}

// Offset: 14220
// Size: 129
// EAX: pPtr
int MoveSessionPointerForwardOne(tU8 **pPtr) {
}

// Offset: 14352
// Size: 262
// EAX: pPtr
// EDX: pType
// EBX: pIndex
tPipe_chunk* FindPreviousChunk(tU8 *pPtr, tPipe_chunk_type pType, tChunk_subject_index pIndex) {
    tU8 *ptr;
    int i;
    int reached_end;
    int chunk_counter;
    tPipe_chunk *mr_chunky;
    tChunk_subject_index masked_index;
}

// Offset: 14616
// Size: 465
// EAX: pChunk
void UndoModelGeometry(tPipe_chunk **pChunk) {
    int i;
    br_model *model_ptr;
    tCar_spec *car;
}

// Offset: 15084
// Size: 63
// EAX: pChunk
void UndoSmudge(tPipe_chunk **pChunk) {
    int i;
    br_model *model_ptr;
    tCar_spec *car;
}

// Offset: 15148
// Size: 101
// EAX: pChunk
// EDX: pPrev_chunk
void UndoPedestrian(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 15252
// Size: 65
// EAX: pChunk
// EDX: pPrev_chunk
void UndoFrameBoundary(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 15320
// Size: 101
// EAX: pChunk
// EDX: pPrev_chunk
void UndoCar(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 15424
// Size: 50
// EAX: pChunk
void UndoSound(tPipe_chunk **pChunk) {
}

// Offset: 15476
// Size: 176
// EAX: pChunk
void UndoDamage(tPipe_chunk **pChunk) {
    tCar_spec *car;
    int i;
}

// Offset: 15652
// Size: 156
// EAX: pChunk
void UndoSpecial(tPipe_chunk **pChunk) {
}

// Offset: 15808
// Size: 91
// EAX: pChunk
// EDX: pPrev_chunk
void UndoPedGib(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 15900
// Size: 91
// EAX: pChunk
// EDX: pPrev_chunk
void UndoSpark(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 15992
// Size: 91
// EAX: pChunk
// EDX: pPrev_chunk
void UndoShrapnel(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 16084
// Size: 102
// EAX: pChunk
// EDX: pPrev_chunk
void UndoScreenWobble(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 16188
// Size: 91
// EAX: pChunk
// EDX: pPrev_chunk
void UndoGrooveStop(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 16280
// Size: 91
// EAX: pChunk
// EDX: pPrev_chunk
void UndoNonCar(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 16372
// Size: 91
// EAX: pChunk
// EDX: pPrev_chunk
void UndoSmoke(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 16464
// Size: 52
// EAX: pChunk
// EDX: pPrev_chunk
void UndoSmokeColumn(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 16516
// Size: 46
// EAX: pChunk
// EDX: pPrev_chunk
void UndoFlame(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
}

// Offset: 16564
// Size: 174
// EAX: pChunk
// EDX: pPrev_chunk
void UndoSplash(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 16740
// Size: 99
// EAX: pChunk
// EDX: pPrev_chunk
void UndoOilSpill(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
    tPipe_chunk *temp_prev_chunk;
}

// Offset: 16840
// Size: 45
// EAX: pChunk
void UndoProxRay(tPipe_chunk **pChunk) {
}

// Offset: 16888
// Size: 105
// EAX: pChunk
// EDX: pPrev_chunk
void UndoSkidAdjustment(tPipe_chunk **pChunk, tPipe_chunk *pPrev_chunk) {
}

// Offset: 16996
// Size: 748
// EAX: pPtr
int UndoPipedSession(tU8 **pPtr) {
    tPipe_chunk *chunk_ptr;
    tPipe_chunk *prev_chunk;
    tU8 *temp_ptr;
    tU8 *pushed_end_of_session;
    int i;
    tPipe_chunk_type chunk_type;
}

// Offset: 17744
// Size: 84
// EAX: pPtr
tU32 FindPrevFrameTime(tU8 *pPtr) {
    tU8 *temp_ptr;
}

// Offset: 17828
// Size: 343
// EAX: pPtr
// EDX: pType
// EBX: pDefault_time
// ECX: pCall_back
void ScanBuffer(tU8 **pPtr, tPipe_chunk_type pType, tU32 pDefault_time, int (*pCall_back)(tPipe_chunk*, int, tU32), int (*pTime_check)(tU32)) {
    tPipe_chunk *chunk_ptr;
    tU32 the_time;
}

// Offset: 18172
// Size: 191
// EAX: pChunk_ptr
// EDX: pChunk_count
// EBX: pTime
int CheckSound(tPipe_chunk *pChunk_ptr, int pChunk_count, tU32 pTime) {
    int i;
    int sound_length;
    tPipe_chunk *temp_ptr;
}

// Offset: 18364
// Size: 88
// EAX: pTime
int SoundTimeout(tU32 pTime) {
}

// Offset: 18452
// Size: 126
// EAX: pPtr
// EDX: pOldest_time
// EBX: pYoungest_time
void ScanAndPlaySoundsToBe(tU8 *pPtr, tU32 pOldest_time, tU32 pYoungest_time) {
    tU8 *temp_ptr;
}

// Offset: 18580
// Size: 635
// EAX: pChunk_ptr
// EDX: pChunk_count
// EBX: pTime
int CheckCar(tPipe_chunk *pChunk_ptr, int pChunk_count, tU32 pTime) {
    int i;
    tCar_spec *car;
    br_vector3 com_offset_c;
    br_vector3 com_offset_w;
    br_vector3 difference;
    tPipe_chunk *temp_ptr;
}

// Offset: 19216
// Size: 237
// EAX: pTime
int CarTimeout(tU32 pTime) {
}

// Offset: 19456
// Size: 397
// EAX: pCar
// EDX: pSource_pos
void ScanCarsPositions(tCar_spec *pCar, br_vector3 *pSource_pos, br_scalar pMax_distance_sqr, tU32 pOffset_time, tU32 pTime_period, br_vector3 *pCar_pos, tU32 *pTime_returned) {
    tU8 *temp_ptr;
}

// Offset: 19856
// Size: 255
// EAX: pChunk_ptr
// EDX: pChunk_count
// EBX: pTime
int CheckIncident(tPipe_chunk *pChunk_ptr, int pChunk_count, tU32 pTime) {
}

// Offset: 20112
// Size: 595
// EAX: pOffset_time
// EDX: pIncident_type
// EBX: pSeverity
// ECX: pInfo
int GetNextIncident(tU32 pOffset_time, tIncident_type *pIncident_type, float *pSeverity, tIncident_info *pInfo, tU32 *pTime_away) {
    tU8 *temp_ptr;
}

// Offset: 20708
// Size: 87
tU32 GetARStartTime() {
}

