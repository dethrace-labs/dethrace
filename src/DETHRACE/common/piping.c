#include "piping.h"
#include "globvars.h"
#include "opponent.h"
#include <stdlib.h>

tU8* gPipe_buffer_start;
int gDisable_sound;
int gDisable_advance;
int gMax_rewind_chunks;
float gWall_severity;
tPipe_reset_proc* gReset_procs[32];
tPiped_registration_snapshot gRegistration_snapshots[5];
tPipe_smudge_data* gSmudge_space;
tU32 gOldest_time;
int gCurrent_snapshot_registration_index;
tPipe_chunk* gMr_chunky;
tCar_spec* gCar_ptr;
br_vector3 gZero_vector;
tPipe_chunk_type gReentrancy_array[5];
tU32 gLast_time;
tPipe_model_geometry_data* gModel_geometry_space;
tU32 gEnd_time;
tU32 gTrigger_time;
int gReentrancy_count;
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

// IDA: void __usercall GetReducedPos(br_vector3 *v@<EAX>, tReduced_pos *p@<EDX>)
void GetReducedPos(br_vector3* v, tReduced_pos* p) {
    LOG_TRACE("(%p, %p)", v, p);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SaveReducedPos(tReduced_pos *p@<EAX>, br_vector3 *v@<EDX>)
void SaveReducedPos(tReduced_pos* p, br_vector3* v) {
    br_vector3 tv;
    LOG_TRACE("(%p, %p)", p, v);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PipeSearchForwards()
int PipeSearchForwards() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl IsActionReplayAvailable()
int IsActionReplayAvailable() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl SomeReplayLeft()
int SomeReplayLeft() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisablePipedSounds()
void DisablePipedSounds() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EnablePipedSounds()
void EnablePipedSounds() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall LengthOfSession@<EAX>(tPipe_session *pSession@<EAX>)
tU32 LengthOfSession(tPipe_session* pSession) {
    int i;
    tU32 running_total;
    tPipe_chunk* the_chunk;
    LOG_TRACE("(%p)", pSession);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StartPipingSession2(tPipe_chunk_type pThe_type@<EAX>, int pMunge_reentrancy@<EDX>)
void StartPipingSession2(tPipe_chunk_type pThe_type, int pMunge_reentrancy) {
    LOG_TRACE("(%d, %d)", pThe_type, pMunge_reentrancy);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StartPipingSession(tPipe_chunk_type pThe_type@<EAX>)
void StartPipingSession(tPipe_chunk_type pThe_type) {
    LOG_TRACE("(%d)", pThe_type);

    SILENT_STUB();
}

// IDA: void __usercall EndPipingSession2(int pMunge_reentrancy@<EAX>)
void EndPipingSession2(int pMunge_reentrancy) {
    int a;
    LOG_TRACE("(%d)", pMunge_reentrancy);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EndPipingSession()
void EndPipingSession() {
    LOG_TRACE("()");

    SILENT_STUB();
}

// IDA: void __usercall AddDataToSession(int pSubject_index@<EAX>, void *pData@<EDX>, tU32 pData_length@<EBX>)
void AddDataToSession(int pSubject_index, void* pData, tU32 pData_length) {
    tU32 temp_buffer_size;
    int variable_for_breaking_on;
    LOG_TRACE("(%d, %p, %d)", pSubject_index, pData, pData_length);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddModelGeometryToPipingSession(tU16 pCar_ID@<EAX>, int pModel_index@<EDX>, int pVertex_count@<EBX>, tChanged_vertex *pCoordinates@<ECX>)
void AddModelGeometryToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex* pCoordinates) {
    tU32 data_size;
    LOG_TRACE("(%d, %d, %d, %p)", pCar_ID, pModel_index, pVertex_count, pCoordinates);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddSmudgeToPipingSession(tU16 pCar_ID@<EAX>, int pModel_index@<EDX>, int pVertex_count@<EBX>, tSmudged_vertex *pCoordinates@<ECX>)
void AddSmudgeToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tSmudged_vertex* pCoordinates) {
    tU32 data_size;
    LOG_TRACE("(%d, %d, %d, %p)", pCar_ID, pModel_index, pVertex_count, pCoordinates);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddPedestrianToPipingSession(int pPedestrian_index@<EAX>, br_matrix34 *pTrans@<EDX>, tU8 pAction_index@<EBX>, tU8 pFrame_index@<ECX>, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset)
void AddPedestrianToPipingSession(int pPedestrian_index, br_matrix34* pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset) {
    tPipe_pedestrian_data data;
    tU32 data_size;
    LOG_TRACE("(%d, %p, %d, %d, %d, %d, %d, %f, %f, %p)", pPedestrian_index, pTrans, pAction_index, pFrame_index, pHit_points, pDone_initial, pParent_ID, pSpin_period, pJump_magnitude, pOffset);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddSparkToPipingSession(int pSpark_index@<EAX>, br_vector3 *pPos@<EDX>, br_vector3 *pV@<EBX>)
void AddSparkToPipingSession(int pSpark_index, br_vector3* pPos, br_vector3* pV) {
    tPipe_spark_data data;
    LOG_TRACE("(%d, %p, %p)", pSpark_index, pPos, pV);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddShrapnelToPipingSession(int pShrapnel_index@<EAX>, br_vector3 *pPos@<EDX>, tU16 pAge@<EBX>, br_material *pMaterial@<ECX>)
void AddShrapnelToPipingSession(int pShrapnel_index, br_vector3* pPos, tU16 pAge, br_material* pMaterial) {
    tPipe_shrapnel_data data;
    tU32 data_size;
    LOG_TRACE("(%d, %p, %d, %p)", pShrapnel_index, pPos, pAge, pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddScreenWobbleToPipingSession(int pWobble_x@<EAX>, int pWobble_y@<EDX>)
void AddScreenWobbleToPipingSession(int pWobble_x, int pWobble_y) {
    tPipe_screen_shake_data data;
    LOG_TRACE("(%d, %d)", pWobble_x, pWobble_y);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddGrooveStopToPipingSession(int pGroove_index@<EAX>, br_matrix34 *pMatrix@<EDX>, int pPath_interrupt@<EBX>, int pObject_interrupt@<ECX>, float pPath_resumption, float pObject_resumption)
void AddGrooveStopToPipingSession(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    tPipe_groove_stop_data data;
    LOG_TRACE("(%d, %p, %d, %d, %f, %f)", pGroove_index, pMatrix, pPath_interrupt, pObject_interrupt, pPath_resumption, pObject_resumption);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddNonCarToPipingSession(int pIndex@<EAX>, br_actor *pActor@<EDX>)
void AddNonCarToPipingSession(int pIndex, br_actor* pActor) {
    tPipe_non_car_data data;
    LOG_TRACE("(%d, %p)", pIndex, pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddSmokeToPipingSession(int pIndex@<EAX>, tU8 pType@<EDX>, br_vector3 *pPos@<EBX>, br_scalar pRadius, br_scalar pStrength)
void AddSmokeToPipingSession(int pIndex, tU8 pType, br_vector3* pPos, br_scalar pRadius, br_scalar pStrength) {
    tPipe_smoke_data data;
    LOG_TRACE("(%d, %d, %p, %f, %f)", pIndex, pType, pPos, pRadius, pStrength);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddSmokeColumnToPipingSession(int pIndex@<EAX>, tCar_spec *pCar@<EDX>, int pVertex@<EBX>, int pColour@<ECX>)
void AddSmokeColumnToPipingSession(int pIndex, tCar_spec* pCar, int pVertex, int pColour) {
    tPipe_smoke_column_data data;
    LOG_TRACE("(%d, %p, %d, %d)", pIndex, pCar, pVertex, pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddFlameToPipingSession(int pIndex@<EAX>, int pFrame_count@<EDX>, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z)
void AddFlameToPipingSession(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z) {
    tPipe_flame_data data;
    LOG_TRACE("(%d, %d, %f, %f, %f, %f)", pIndex, pFrame_count, pScale_x, pScale_y, pOffset_x, pOffset_z);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddSplashToPipingSession(tCollision_info *pCar@<EAX>)
void AddSplashToPipingSession(tCollision_info* pCar) {
    tPipe_splash_data data;
    LOG_TRACE("(%p)", pCar);

    SILENT_STUB();
}

// IDA: void __usercall AddOilSpillToPipingSession(int pIndex@<EAX>, br_matrix34 *pMat@<EDX>, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap)
void AddOilSpillToPipingSession(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap) {
    tPipe_oil_spill_data data;
    LOG_TRACE("(%d, %p, %f, %f, %d, %d, %p, %p, %p)", pIndex, pMat, pFull_size, pGrow_rate, pSpill_time, pStop_time, pCar, pOriginal_pos, pPixelmap);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddFrameFinishToPipingSession(tU32 pThe_time@<EAX>)
void AddFrameFinishToPipingSession(tU32 pThe_time) {
    tPipe_frame_boundary_data data;
    LOG_TRACE("(%d)", pThe_time);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddCarToPipingSession(int pCar_ID@<EAX>, br_matrix34 *pCar_mat@<EDX>, br_vector3 *pCar_velocity@<EBX>, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag)
void AddCarToPipingSession(int pCar_ID, br_matrix34* pCar_mat, br_vector3* pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag) {
    tPipe_car_data data;
    LOG_TRACE("(%d, %p, %p, %f, %f, %f, %f, %f, %f, %f, %d, %d)", pCar_ID, pCar_mat, pCar_velocity, pSpeedo_speed, pLf_sus_position, pRf_sus_position, pLr_sus_position, pRr_sus_position, pSteering_angle, pRevs, pGear, pFrame_coll_flag);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddSoundToPipingSession(tS3_outlet_ptr pOutlet@<EAX>, int pSound_index@<EDX>, tS3_volume pL_volume@<EBX>, tS3_volume pR_volume@<ECX>, tS3_pitch pPitch, br_vector3 *pPos)
void AddSoundToPipingSession(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3* pPos) {
    tPipe_sound_data data;
    LOG_TRACE("(%d, %d, %d, %d, %d, %p)", pOutlet, pSound_index, pL_volume, pR_volume, pPitch, pPos);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddDamageToPipingSession(int pCar_ID@<EAX>, tS8 *pDifferences@<EDX>)
void AddDamageToPipingSession(int pCar_ID, tS8* pDifferences) {
    tPipe_damage_data data;
    int i;
    LOG_TRACE("(%d, %p)", pCar_ID, pDifferences);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddSpecialToPipingSession(tSpecial_type pType@<EAX>)
void AddSpecialToPipingSession(tSpecial_type pType) {
    tPipe_special_data data;
    LOG_TRACE("(%d)", pType);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddPedGibToPipingSession(int pIndex@<EAX>, br_matrix34 *pTrans@<EDX>, int pSize@<EBX>, int pGib_index@<ECX>, int pPed_index)
void AddPedGibToPipingSession(int pIndex, br_matrix34* pTrans, int pSize, int pGib_index, int pPed_index) {
    tPipe_ped_gib_data data;
    LOG_TRACE("(%d, %p, %d, %d, %d)", pIndex, pTrans, pSize, pGib_index, pPed_index);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AddCarIncidentToPipingSession(float pSeverity, tCar_spec *pCar, br_vector3 *pImpact_point)
void AddCarIncidentToPipingSession(float pSeverity, tCar_spec* pCar, br_vector3* pImpact_point) {
    tPipe_incident_data data;
    LOG_TRACE("(%f, %p, %p)", pSeverity, pCar, pImpact_point);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddPedIncidentToPipingSession(int pPed_index@<EAX>, br_actor *pActor@<EDX>)
void AddPedIncidentToPipingSession(int pPed_index, br_actor* pActor) {
    tPipe_incident_data data;
    LOG_TRACE("(%d, %p)", pPed_index, pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AddWallIncidentToPipingSession(float pSeverity, br_vector3 *pImpact_point)
void AddWallIncidentToPipingSession(float pSeverity, br_vector3* pImpact_point) {
    tPipe_incident_data data;
    LOG_TRACE("(%f, %p)", pSeverity, pImpact_point);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddProxRayToPipingSession(int pRay_index@<EAX>, tCar_spec *pCar@<EDX>, tU16 pPed_index@<EBX>, tU32 pTime@<ECX>)
void AddProxRayToPipingSession(int pRay_index, tCar_spec* pCar, tU16 pPed_index, tU32 pTime) {
    tPipe_prox_ray_data data;
    LOG_TRACE("(%d, %p, %d, %d)", pRay_index, pCar, pPed_index, pTime);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddSkidAdjustmentToPipingSession(int pSkid_num@<EAX>, br_matrix34 *pMatrix@<EDX>, int pMaterial_index@<EBX>)
void AddSkidAdjustmentToPipingSession(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    tPipe_skid_adjustment adjustment;
    LOG_TRACE("(%d, %p, %d)", pSkid_num, pMatrix, pMaterial_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleModelGeometry(tU16 pCar_ID@<EAX>, int pModel_index@<EDX>, int pVertex_count@<EBX>, tChanged_vertex *pCoordinates@<ECX>)
void PipeSingleModelGeometry(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex* pCoordinates) {
    LOG_TRACE("(%d, %d, %d, %p)", pCar_ID, pModel_index, pVertex_count, pCoordinates);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSinglePedestrian(int pPedestrian_index@<EAX>, br_matrix34 *pTrans@<EDX>, tU8 pAction_index@<EBX>, tU8 pFrame_index@<ECX>, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset)
void PipeSinglePedestrian(int pPedestrian_index, br_matrix34* pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset) {
    LOG_TRACE("(%d, %p, %d, %d, %d, %d, %d, %f, %f, %p)", pPedestrian_index, pTrans, pAction_index, pFrame_index, pHit_points, pDone_initial, pParent_ID, pSpin_period, pJump_magnitude, pOffset);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleCar(int pCar_ID@<EAX>, br_matrix34 *pCar_mat@<EDX>, br_vector3 *pCar_velocity@<EBX>, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag)
void PipeSingleCar(int pCar_ID, br_matrix34* pCar_mat, br_vector3* pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag) {
    LOG_TRACE("(%d, %p, %p, %f, %f, %f, %f, %f, %f, %f, %d, %d)", pCar_ID, pCar_mat, pCar_velocity, pSpeedo_speed, pLf_sus_position, pRf_sus_position, pLr_sus_position, pRr_sus_position, pSteering_angle, pRevs, pGear, pFrame_coll_flag);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleSound(tS3_outlet_ptr pOutlet@<EAX>, int pSound_index@<EDX>, tS3_volume pL_volume@<EBX>, tS3_volume pR_volume@<ECX>, tS3_pitch pPitch, br_vector3 *pPos)
void PipeSingleSound(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3* pPos) {
    LOG_TRACE("(%d, %d, %d, %d, %d, %p)", pOutlet, pSound_index, pL_volume, pR_volume, pPitch, pPos);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleOilSpill(int pIndex@<EAX>, br_matrix34 *pMat@<EDX>, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap)
void PipeSingleOilSpill(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap) {
    LOG_TRACE("(%d, %p, %f, %f, %d, %d, %p, %p, %p)", pIndex, pMat, pFull_size, pGrow_rate, pSpill_time, pStop_time, pCar, pOriginal_pos, pPixelmap);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleDamage(int pCar_ID@<EAX>, tS8 *pDifferences@<EDX>)
void PipeSingleDamage(int pCar_ID, tS8* pDifferences) {
    LOG_TRACE("(%d, %p)", pCar_ID, pDifferences);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleSpecial(tSpecial_type pType@<EAX>)
void PipeSingleSpecial(tSpecial_type pType) {
    LOG_TRACE("(%d)", pType);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSinglePedGib(int pIndex@<EAX>, br_matrix34 *pTrans@<EDX>, int pSize@<EBX>, int pGib_index@<ECX>, int pPed_index)
void PipeSinglePedGib(int pIndex, br_matrix34* pTrans, int pSize, int pGib_index, int pPed_index) {
    LOG_TRACE("(%d, %p, %d, %d, %d)", pIndex, pTrans, pSize, pGib_index, pPed_index);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PipeSingleCarIncident(float pSeverity, tCar_spec *pCar, br_vector3 *pImpact_point)
void PipeSingleCarIncident(float pSeverity, tCar_spec* pCar, br_vector3* pImpact_point) {
    LOG_TRACE("(%f, %p, %p)", pSeverity, pCar, pImpact_point);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSinglePedIncident(int pPed_index@<EAX>, br_actor *pActor@<EDX>)
void PipeSinglePedIncident(int pPed_index, br_actor* pActor) {
    LOG_TRACE("(%d, %p)", pPed_index, pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PipeSingleWallIncident(float pSeverity, br_vector3 *pImpact_point)
void PipeSingleWallIncident(float pSeverity, br_vector3* pImpact_point) {
    LOG_TRACE("(%f, %p)", pSeverity, pImpact_point);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleScreenShake(int pWobble_x@<EAX>, int pWobble_y@<EDX>)
void PipeSingleScreenShake(int pWobble_x, int pWobble_y) {
    LOG_TRACE("(%d, %d)", pWobble_x, pWobble_y);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleGrooveStop(int pGroove_index@<EAX>, br_matrix34 *pMatrix@<EDX>, int pPath_interrupt@<EBX>, int pObject_interrupt@<ECX>, float pPath_resumption, float pObject_resumption)
void PipeSingleGrooveStop(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    LOG_TRACE("(%d, %p, %d, %d, %f, %f)", pGroove_index, pMatrix, pPath_interrupt, pObject_interrupt, pPath_resumption, pObject_resumption);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PipeFrameFinish()
void PipeFrameFinish() {
    LOG_TRACE("()");
    SILENT_STUB();
}

// IDA: void __cdecl PipingFrameReset()
void PipingFrameReset() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PipeSingleSkidAdjustment(int pSkid_num@<EAX>, br_matrix34 *pMatrix@<EDX>, int pMaterial_index@<EBX>)
void PipeSingleSkidAdjustment(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    LOG_TRACE("(%d, %p, %d)", pSkid_num, pMatrix, pMaterial_index);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetPiping()
void ResetPiping() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitialisePiping()
void InitialisePiping() {
    LOG_TRACE("()");
    SILENT_STUB();
}

// IDA: void __cdecl DisposePiping()
void DisposePiping() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitLastDamageArrayEtc()
void InitLastDamageArrayEtc() {
    int i;
    int j;
    int cat;
    int car_count;
    tCar_spec* car;
    LOG_TRACE("()");

    for (cat = eVehicle_self; cat <= eVehicle_not_really; cat++) {
        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (i = 0; i < car_count; i++) {
            if (cat == eVehicle_self) {
                car = &gProgram_state.current_car;
            } else {
                car = GetCarSpec(cat, i);
            }
            if (cat != eVehicle_not_really) {
                for (j = 0; j < COUNT_OF(car->frame_start_damage); j++) {
                    car->frame_start_damage[j] = 0;
                }
            }
            car->car_ID = i + (cat * 256);
        }
    }
}

// IDA: void __cdecl ResetCars()
void ResetCars() {
    tCar_spec* car;
    int cat;
    int i;
    int car_count;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PipeCarPositions()
void PipeCarPositions() {
    tCar_spec* car;
    int cat;
    int i;
    int j;
    int car_count;
    int session_started;
    int difference_found;
    tS8 damage_deltas[12];
    LOG_TRACE("()");

    SILENT_STUB();
}

// IDA: void __cdecl ResetPipePlayToEnd()
void ResetPipePlayToEnd() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetPipePlayToStart()
void ResetPipePlayToStart() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tU8* __cdecl GetPipePlayPtr()
tU8* GetPipePlayPtr() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetPipePlayPtr(tU8 *pPtr@<EAX>)
void SetPipePlayPtr(tU8* pPtr) {
    LOG_TRACE("(%p)", pPtr);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdvanceChunkPtr(tPipe_chunk **pChunk@<EAX>, tChunk_subject_index pType@<EDX>)
void AdvanceChunkPtr(tPipe_chunk** pChunk, tChunk_subject_index pType) {
    tPipe_chunk* old_chunk;
    LOG_TRACE("(%p, %d)", pChunk, pType);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyModelGeometry(tPipe_chunk **pChunk@<EAX>)
void ApplyModelGeometry(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoSmudge(tPipe_chunk **pChunk@<EAX>, int pDir@<EDX>)
void DoSmudge(tPipe_chunk** pChunk, int pDir) {
    int i;
    int v;
    tU8 inc;
    br_model* model_ptr;
    tCar_spec* car;
    int group;
    LOG_TRACE("(%p, %d)", pChunk, pDir);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplySmudge(tPipe_chunk **pChunk@<EAX>)
void ApplySmudge(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyPedestrian(tPipe_chunk **pChunk@<EAX>)
void ApplyPedestrian(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplySpark(tPipe_chunk **pChunk@<EAX>)
void ApplySpark(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyShrapnel(tPipe_chunk **pChunk@<EAX>)
void ApplyShrapnel(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyScreenWobble(tPipe_chunk **pChunk@<EAX>)
void ApplyScreenWobble(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyGrooveStop(tPipe_chunk **pChunk@<EAX>)
void ApplyGrooveStop(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyNonCar(tPipe_chunk **pChunk@<EAX>)
void ApplyNonCar(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplySmoke(tPipe_chunk **pChunk@<EAX>)
void ApplySmoke(tPipe_chunk** pChunk) {
    br_vector3 pos;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplySmokeColumn(tPipe_chunk **pChunk@<EAX>)
void ApplySmokeColumn(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyFlame(tPipe_chunk **pChunk@<EAX>)
void ApplyFlame(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplySplash(tPipe_chunk **pChunk@<EAX>)
void ApplySplash(tPipe_chunk** pChunk) {
    tCar_spec* c;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyOilSpill(tPipe_chunk **pChunk@<EAX>, tU32 pStop_time@<EDX>)
void ApplyOilSpill(tPipe_chunk** pChunk, tU32 pStop_time) {
    LOG_TRACE("(%p, %d)", pChunk, pStop_time);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyFrameBoundary(tPipe_chunk **pChunk@<EAX>)
void ApplyFrameBoundary(tPipe_chunk** pChunk) {
    tU32 result;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplySound(tPipe_chunk **pChunk@<EAX>)
void ApplySound(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyCar(tPipe_chunk **pChunk@<EAX>)
void ApplyCar(tPipe_chunk** pChunk) {
    tCar_spec* car;
    br_vector3 com_offset_c;
    br_vector3 com_offset_w;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyDamage(tPipe_chunk **pChunk@<EAX>)
void ApplyDamage(tPipe_chunk** pChunk) {
    tCar_spec* car;
    int i;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplySpecial(tPipe_chunk **pChunk@<EAX>)
void ApplySpecial(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyPedGib(tPipe_chunk **pChunk@<EAX>)
void ApplyPedGib(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplyProxRay(tPipe_chunk **pChunk@<EAX>)
void ApplyProxRay(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ApplySkidAdjustment(tPipe_chunk **pChunk@<EAX>)
void ApplySkidAdjustment(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ApplyPipedSession@<EAX>(tU8 **pPtr@<EAX>)
int ApplyPipedSession(tU8** pPtr) {
    int i;
    int return_value;
    tPipe_chunk* chunk_ptr;
    tPipe_chunk_type chunk_type;
    LOG_TRACE("(%p)", pPtr);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall MoveSessionPointerBackOne@<EAX>(tU8 **pPtr@<EAX>)
int MoveSessionPointerBackOne(tU8** pPtr) {
    LOG_TRACE("(%p)", pPtr);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall MoveSessionPointerForwardOne@<EAX>(tU8 **pPtr@<EAX>)
int MoveSessionPointerForwardOne(tU8** pPtr) {
    LOG_TRACE("(%p)", pPtr);
    NOT_IMPLEMENTED();
}

// IDA: tPipe_chunk* __usercall FindPreviousChunk@<EAX>(tU8 *pPtr@<EAX>, tPipe_chunk_type pType@<EDX>, tChunk_subject_index pIndex@<EBX>)
tPipe_chunk* FindPreviousChunk(tU8* pPtr, tPipe_chunk_type pType, tChunk_subject_index pIndex) {
    tU8* ptr;
    int i;
    int reached_end;
    int chunk_counter;
    tPipe_chunk* mr_chunky;
    tChunk_subject_index masked_index;
    LOG_TRACE("(%p, %d, %d)", pPtr, pType, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoModelGeometry(tPipe_chunk **pChunk@<EAX>)
void UndoModelGeometry(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoSmudge(tPipe_chunk **pChunk@<EAX>)
void UndoSmudge(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoPedestrian(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoPedestrian(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoFrameBoundary(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoFrameBoundary(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoCar(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoCar(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoSound(tPipe_chunk **pChunk@<EAX>)
void UndoSound(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoDamage(tPipe_chunk **pChunk@<EAX>)
void UndoDamage(tPipe_chunk** pChunk) {
    tCar_spec* car;
    int i;
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoSpecial(tPipe_chunk **pChunk@<EAX>)
void UndoSpecial(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoPedGib(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoPedGib(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoSpark(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSpark(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoShrapnel(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoShrapnel(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoScreenWobble(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoScreenWobble(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoGrooveStop(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoGrooveStop(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoNonCar(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoNonCar(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoSmoke(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSmoke(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoSmokeColumn(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSmokeColumn(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoFlame(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoFlame(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoSplash(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSplash(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoOilSpill(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoOilSpill(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoProxRay(tPipe_chunk **pChunk@<EAX>)
void UndoProxRay(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UndoSkidAdjustment(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSkidAdjustment(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall UndoPipedSession@<EAX>(tU8 **pPtr@<EAX>)
int UndoPipedSession(tU8** pPtr) {
    tPipe_chunk* chunk_ptr;
    tPipe_chunk* prev_chunk;
    tU8* temp_ptr;
    tU8* pushed_end_of_session;
    int i;
    tPipe_chunk_type chunk_type;
    LOG_TRACE("(%p)", pPtr);
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall FindPrevFrameTime@<EAX>(tU8 *pPtr@<EAX>)
tU32 FindPrevFrameTime(tU8* pPtr) {
    tU8* temp_ptr;
    LOG_TRACE("(%p)", pPtr);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ScanBuffer(tU8 **pPtr@<EAX>, tPipe_chunk_type pType@<EDX>, tU32 pDefault_time@<EBX>, int (*pCall_back)(tPipe_chunk*, int, tU32)@<ECX>, int (*pTime_check)(tU32))
void ScanBuffer(tU8** pPtr, tPipe_chunk_type pType, tU32 pDefault_time, int (*pCall_back)(tPipe_chunk*, int, tU32), int (*pTime_check)(tU32)) {
    tPipe_chunk* chunk_ptr;
    tU32 the_time;
    LOG_TRACE("(%p, %d, %d, %p, %p)", pPtr, pType, pDefault_time, pCall_back, pTime_check);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CheckSound@<EAX>(tPipe_chunk *pChunk_ptr@<EAX>, int pChunk_count@<EDX>, tU32 pTime@<EBX>)
int CheckSound(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime) {
    int i;
    int sound_length;
    tPipe_chunk* temp_ptr;
    LOG_TRACE("(%p, %d, %d)", pChunk_ptr, pChunk_count, pTime);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SoundTimeout@<EAX>(tU32 pTime@<EAX>)
int SoundTimeout(tU32 pTime) {
    LOG_TRACE("(%d)", pTime);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ScanAndPlaySoundsToBe(tU8 *pPtr@<EAX>, tU32 pOldest_time@<EDX>, tU32 pYoungest_time@<EBX>)
void ScanAndPlaySoundsToBe(tU8* pPtr, tU32 pOldest_time, tU32 pYoungest_time) {
    tU8* temp_ptr;
    LOG_TRACE("(%p, %d, %d)", pPtr, pOldest_time, pYoungest_time);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CheckCar@<EAX>(tPipe_chunk *pChunk_ptr@<EAX>, int pChunk_count@<EDX>, tU32 pTime@<EBX>)
int CheckCar(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime) {
    int i;
    tCar_spec* car;
    br_vector3 com_offset_c;
    br_vector3 com_offset_w;
    br_vector3 difference;
    tPipe_chunk* temp_ptr;
    LOG_TRACE("(%p, %d, %d)", pChunk_ptr, pChunk_count, pTime);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CarTimeout@<EAX>(tU32 pTime@<EAX>)
int CarTimeout(tU32 pTime) {
    LOG_TRACE("(%d)", pTime);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ScanCarsPositions(tCar_spec *pCar@<EAX>, br_vector3 *pSource_pos@<EDX>, br_scalar pMax_distance_sqr, tU32 pOffset_time, tU32 pTime_period, br_vector3 *pCar_pos, tU32 *pTime_returned)
void ScanCarsPositions(tCar_spec* pCar, br_vector3* pSource_pos, br_scalar pMax_distance_sqr, tU32 pOffset_time, tU32 pTime_period, br_vector3* pCar_pos, tU32* pTime_returned) {
    tU8* temp_ptr;
    LOG_TRACE("(%p, %p, %f, %d, %d, %p, %p)", pCar, pSource_pos, pMax_distance_sqr, pOffset_time, pTime_period, pCar_pos, pTime_returned);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CheckIncident@<EAX>(tPipe_chunk *pChunk_ptr@<EAX>, int pChunk_count@<EDX>, tU32 pTime@<EBX>)
int CheckIncident(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime) {
    LOG_TRACE("(%p, %d, %d)", pChunk_ptr, pChunk_count, pTime);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetNextIncident@<EAX>(tU32 pOffset_time@<EAX>, tIncident_type *pIncident_type@<EDX>, float *pSeverity@<EBX>, tIncident_info *pInfo@<ECX>, tU32 *pTime_away)
int GetNextIncident(tU32 pOffset_time, tIncident_type* pIncident_type, float* pSeverity, tIncident_info* pInfo, tU32* pTime_away) {
    tU8* temp_ptr;
    LOG_TRACE("(%d, %p, %p, %p, %p)", pOffset_time, pIncident_type, pSeverity, pInfo, pTime_away);
    NOT_IMPLEMENTED();
}

// IDA: tU32 __cdecl GetARStartTime()
tU32 GetARStartTime() {
    tU8* temp_ptr;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
