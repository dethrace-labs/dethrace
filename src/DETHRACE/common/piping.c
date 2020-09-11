#include "piping.h"
#include <stdlib.h>

tPiped_registration_snapshot gRegistration_snapshots[5];
tPipe_reset_proc* gReset_procs[32];
float gWall_severity;
tPipe_smudge_data* gSmudge_space;
tU32 gOldest_time;
int gCurrent_snapshot_registration_index;
tPipe_chunk* gMr_chunky;
int gMax_rewind_chunks;
int gDisable_sound;
tCar_spec* gCar_ptr;
br_vector3 gZero_vector;
tPipe_chunk_type gReentrancy_array[5];
tU32 gLast_time;
tPipe_model_geometry_data* gModel_geometry_space;
tU32 gEnd_time;
tU32 gTrigger_time;
int gReentrancy_count;
int gDisable_advance;
br_vector3 gCar_pos;
br_vector3 gReference_pos;
br_scalar gMax_distance;
tU32 gLoop_abort_time;
br_vector3 gWall_impact_point;
tU8* gPipe_buffer_working_end;
tU32 gYoungest_time;
tU8* gPipe_buffer_phys_end;
tU8* gLocal_buffer_record_ptr;
tU8* gPipe_play_ptr;
tU8* gEnd_of_session;
tU8* gPipe_record_ptr;
tU8* gPipe_buffer_oldest;
tU32 gPipe_buffer_size;
tU8* gLocal_buffer;
tU32 gLocal_buffer_size;
tU8* gPipe_buffer_start;

void GetReducedPos(br_vector3* v, tReduced_pos* p) {
    NOT_IMPLEMENTED();
}

void SaveReducedPos(tReduced_pos* p, br_vector3* v) {
    br_vector3 tv;
    NOT_IMPLEMENTED();
}

int PipeSearchForwards() {
    NOT_IMPLEMENTED();
}

int IsActionReplayAvailable() {
    NOT_IMPLEMENTED();
}

int SomeReplayLeft() {
    NOT_IMPLEMENTED();
}

void DisablePipedSounds() {
    NOT_IMPLEMENTED();
}

void EnablePipedSounds() {
    NOT_IMPLEMENTED();
}

tU32 LengthOfSession(tPipe_session* pSession) {
    int i;
    tU32 running_total;
    tPipe_chunk* the_chunk;
    NOT_IMPLEMENTED();
}

void StartPipingSession2(tPipe_chunk_type pThe_type, int pMunge_reentrancy) {
    NOT_IMPLEMENTED();
}

void StartPipingSession(tPipe_chunk_type pThe_type) {
    NOT_IMPLEMENTED();
}

void EndPipingSession2(int pMunge_reentrancy) {
    int a;
    NOT_IMPLEMENTED();
}

void EndPipingSession() {
    NOT_IMPLEMENTED();
}

void AddDataToSession(int pSubject_index, void* pData, tU32 pData_length) {
    tU32 temp_buffer_size;
    int variable_for_breaking_on;
    NOT_IMPLEMENTED();
}

void AddModelGeometryToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex* pCoordinates) {
    tU32 data_size;
    NOT_IMPLEMENTED();
}

void AddSmudgeToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tSmudged_vertex* pCoordinates) {
    tU32 data_size;
    NOT_IMPLEMENTED();
}

void AddPedestrianToPipingSession(int pPedestrian_index, br_matrix34* pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset) {
    tPipe_pedestrian_data data;
    tU32 data_size;
    NOT_IMPLEMENTED();
}

void AddSparkToPipingSession(int pSpark_index, br_vector3* pPos, br_vector3* pV) {
    tPipe_spark_data data;
    NOT_IMPLEMENTED();
}

void AddShrapnelToPipingSession(int pShrapnel_index, br_vector3* pPos, tU16 pAge, br_material* pMaterial) {
    tPipe_shrapnel_data data;
    tU32 data_size;
    NOT_IMPLEMENTED();
}

void AddScreenWobbleToPipingSession(int pWobble_x, int pWobble_y) {
    tPipe_screen_shake_data data;
    NOT_IMPLEMENTED();
}

void AddGrooveStopToPipingSession(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    tPipe_groove_stop_data data;
    NOT_IMPLEMENTED();
}

void AddNonCarToPipingSession(int pIndex, br_actor* pActor) {
    tPipe_non_car_data data;
    NOT_IMPLEMENTED();
}

void AddSmokeToPipingSession(int pIndex, tU8 pType, br_vector3* pPos, br_scalar pRadius, br_scalar pStrength) {
    tPipe_smoke_data data;
    NOT_IMPLEMENTED();
}

void AddSmokeColumnToPipingSession(int pIndex, tCar_spec* pCar, int pVertex, int pColour) {
    tPipe_smoke_column_data data;
    NOT_IMPLEMENTED();
}

void AddFlameToPipingSession(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z) {
    tPipe_flame_data data;
    NOT_IMPLEMENTED();
}

void AddSplashToPipingSession(tCollision_info* pCar) {
    tPipe_splash_data data;
    NOT_IMPLEMENTED();
}

void AddOilSpillToPipingSession(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap) {
    tPipe_oil_spill_data data;
    NOT_IMPLEMENTED();
}

void AddFrameFinishToPipingSession(tU32 pThe_time) {
    tPipe_frame_boundary_data data;
    NOT_IMPLEMENTED();
}

void AddCarToPipingSession(int pCar_ID, br_matrix34* pCar_mat, br_vector3* pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag) {
    tPipe_car_data data;
    NOT_IMPLEMENTED();
}

void AddSoundToPipingSession(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3* pPos) {
    tPipe_sound_data data;
    NOT_IMPLEMENTED();
}

void AddDamageToPipingSession(int pCar_ID, tS8* pDifferences) {
    tPipe_damage_data data;
    int i;
    NOT_IMPLEMENTED();
}

void AddSpecialToPipingSession(tSpecial_type pType) {
    tPipe_special_data data;
    NOT_IMPLEMENTED();
}

void AddPedGibToPipingSession(int pIndex, br_matrix34* pTrans, int pSize, int pGib_index, int pPed_index) {
    tPipe_ped_gib_data data;
    NOT_IMPLEMENTED();
}

void AddCarIncidentToPipingSession(float pSeverity, tCar_spec* pCar, br_vector3* pImpact_point) {
    tPipe_incident_data data;
    NOT_IMPLEMENTED();
}

void AddPedIncidentToPipingSession(int pPed_index, br_actor* pActor) {
    tPipe_incident_data data;
    NOT_IMPLEMENTED();
}

void AddWallIncidentToPipingSession(float pSeverity, br_vector3* pImpact_point) {
    tPipe_incident_data data;
    NOT_IMPLEMENTED();
}

void AddProxRayToPipingSession(int pRay_index, tCar_spec* pCar, tU16 pPed_index, tU32 pTime) {
    tPipe_prox_ray_data data;
    NOT_IMPLEMENTED();
}

void AddSkidAdjustmentToPipingSession(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    tPipe_skid_adjustment adjustment;
    NOT_IMPLEMENTED();
}

void PipeSingleModelGeometry(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex* pCoordinates) {
    NOT_IMPLEMENTED();
}

void PipeSinglePedestrian(int pPedestrian_index, br_matrix34* pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset) {
    NOT_IMPLEMENTED();
}

void PipeSingleCar(int pCar_ID, br_matrix34* pCar_mat, br_vector3* pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag) {
    NOT_IMPLEMENTED();
}

void PipeSingleSound(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3* pPos) {
    NOT_IMPLEMENTED();
}

void PipeSingleOilSpill(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap) {
    NOT_IMPLEMENTED();
}

void PipeSingleDamage(int pCar_ID, tS8* pDifferences) {
    NOT_IMPLEMENTED();
}

void PipeSingleSpecial(tSpecial_type pType) {
    NOT_IMPLEMENTED();
}

void PipeSinglePedGib(int pIndex, br_matrix34* pTrans, int pSize, int pGib_index, int pPed_index) {
    NOT_IMPLEMENTED();
}

void PipeSingleCarIncident(float pSeverity, tCar_spec* pCar, br_vector3* pImpact_point) {
    NOT_IMPLEMENTED();
}

void PipeSinglePedIncident(int pPed_index, br_actor* pActor) {
    NOT_IMPLEMENTED();
}

void PipeSingleWallIncident(float pSeverity, br_vector3* pImpact_point) {
    NOT_IMPLEMENTED();
}

void PipeSingleScreenShake(int pWobble_x, int pWobble_y) {
    NOT_IMPLEMENTED();
}

void PipeSingleGrooveStop(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    NOT_IMPLEMENTED();
}

void PipeFrameFinish() {
    NOT_IMPLEMENTED();
}

void PipingFrameReset() {
    NOT_IMPLEMENTED();
}

void PipeSingleSkidAdjustment(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    NOT_IMPLEMENTED();
}

void ResetPiping() {
    NOT_IMPLEMENTED();
}

void InitialisePiping() {
    NOT_IMPLEMENTED();
}

void DisposePiping() {
    NOT_IMPLEMENTED();
}

void InitLastDamageArrayEtc() {
    int j;
    int cat;
    int car_count;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void ResetCars() {
    int cat;
    int i;
    int car_count;
    NOT_IMPLEMENTED();
}

void PipeCarPositions() {
    int cat;
    int i;
    int j;
    int car_count;
    int session_started;
    int difference_found;
    tS8 damage_deltas[12];
    NOT_IMPLEMENTED();
}

void ResetPipePlayToEnd() {
    NOT_IMPLEMENTED();
}

void ResetPipePlayToStart() {
    NOT_IMPLEMENTED();
}

tU8* GetPipePlayPtr() {
    NOT_IMPLEMENTED();
}

void SetPipePlayPtr(tU8* pPtr) {
    NOT_IMPLEMENTED();
}

void AdvanceChunkPtr(tPipe_chunk** pChunk, tChunk_subject_index pType) {
    tPipe_chunk* old_chunk;
    NOT_IMPLEMENTED();
}

void ApplyModelGeometry(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void DoSmudge(tPipe_chunk** pChunk, int pDir) {
    int i;
    int v;
    tU8 inc;
    br_model* model_ptr;
    tCar_spec* car;
    int group;
    NOT_IMPLEMENTED();
}

void ApplySmudge(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyPedestrian(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplySpark(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyShrapnel(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyScreenWobble(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyGrooveStop(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyNonCar(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplySmoke(tPipe_chunk** pChunk) {
    br_vector3 pos;
    NOT_IMPLEMENTED();
}

void ApplySmokeColumn(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyFlame(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplySplash(tPipe_chunk** pChunk) {
    tCar_spec* c;
    NOT_IMPLEMENTED();
}

void ApplyOilSpill(tPipe_chunk** pChunk, tU32 pStop_time) {
    NOT_IMPLEMENTED();
}

void ApplyFrameBoundary(tPipe_chunk** pChunk) {
    tU32 result;
    NOT_IMPLEMENTED();
}

void ApplySound(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyCar(tPipe_chunk** pChunk) {
    tCar_spec* car;
    br_vector3 com_offset_c;
    br_vector3 com_offset_w;
    NOT_IMPLEMENTED();
}

void ApplyDamage(tPipe_chunk** pChunk) {
    tCar_spec* car;
    int i;
    NOT_IMPLEMENTED();
}

void ApplySpecial(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyPedGib(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplyProxRay(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void ApplySkidAdjustment(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

int ApplyPipedSession(tU8** pPtr) {
    int i;
    int return_value;
    tPipe_chunk* chunk_ptr;
    tPipe_chunk_type chunk_type;
    NOT_IMPLEMENTED();
}

int MoveSessionPointerBackOne(tU8** pPtr) {
    NOT_IMPLEMENTED();
}

int MoveSessionPointerForwardOne(tU8** pPtr) {
    NOT_IMPLEMENTED();
}

tPipe_chunk* FindPreviousChunk(tU8* pPtr, tPipe_chunk_type pType, tChunk_subject_index pIndex) {
    tU8* ptr;
    int i;
    int reached_end;
    int chunk_counter;
    tPipe_chunk* mr_chunky;
    tChunk_subject_index masked_index;
    NOT_IMPLEMENTED();
}

void UndoModelGeometry(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void UndoSmudge(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void UndoPedestrian(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoFrameBoundary(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoCar(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoSound(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void UndoDamage(tPipe_chunk** pChunk) {
    tCar_spec* car;
    int i;
    NOT_IMPLEMENTED();
}

void UndoSpecial(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void UndoPedGib(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoSpark(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoShrapnel(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoScreenWobble(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoGrooveStop(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoNonCar(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoSmoke(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoSmokeColumn(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoFlame(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    NOT_IMPLEMENTED();
}

void UndoSplash(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoOilSpill(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    NOT_IMPLEMENTED();
}

void UndoProxRay(tPipe_chunk** pChunk) {
    NOT_IMPLEMENTED();
}

void UndoSkidAdjustment(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    NOT_IMPLEMENTED();
}

int UndoPipedSession(tU8** pPtr) {
    tPipe_chunk* chunk_ptr;
    tPipe_chunk* prev_chunk;
    tU8* temp_ptr;
    tU8* pushed_end_of_session;
    int i;
    tPipe_chunk_type chunk_type;
    NOT_IMPLEMENTED();
}

tU32 FindPrevFrameTime(tU8* pPtr) {
    tU8* temp_ptr;
    NOT_IMPLEMENTED();
}

void ScanBuffer(tU8** pPtr, tPipe_chunk_type pType, tU32 pDefault_time, int (*pCall_back)(tPipe_chunk*, int, tU32), int (*pTime_check)(tU32)) {
    tPipe_chunk* chunk_ptr;
    tU32 the_time;
    NOT_IMPLEMENTED();
}

int CheckSound(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime) {
    int i;
    int sound_length;
    tPipe_chunk* temp_ptr;
    NOT_IMPLEMENTED();
}

int SoundTimeout(tU32 pTime) {
    NOT_IMPLEMENTED();
}

void ScanAndPlaySoundsToBe(tU8* pPtr, tU32 pOldest_time, tU32 pYoungest_time) {
    tU8* temp_ptr;
    NOT_IMPLEMENTED();
}

int CheckCar(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime) {
    int i;
    tCar_spec* car;
    br_vector3 com_offset_c;
    br_vector3 com_offset_w;
    br_vector3 difference;
    tPipe_chunk* temp_ptr;
    NOT_IMPLEMENTED();
}

int CarTimeout(tU32 pTime) {
    NOT_IMPLEMENTED();
}

void ScanCarsPositions(tCar_spec* pCar, br_vector3* pSource_pos, br_scalar pMax_distance_sqr, tU32 pOffset_time, tU32 pTime_period, br_vector3* pCar_pos, tU32* pTime_returned) {
    tU8* temp_ptr;
    NOT_IMPLEMENTED();
}

int CheckIncident(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime) {
    NOT_IMPLEMENTED();
}

int GetNextIncident(tU32 pOffset_time, tIncident_type* pIncident_type, float* pSeverity, tIncident_info* pInfo, tU32* pTime_away) {
    tU8* temp_ptr;
    NOT_IMPLEMENTED();
}

tU32 GetARStartTime() {
    NOT_IMPLEMENTED();
}
