#include "piping.h"
#include "brender/brender.h"
#include "car.h"
#include "crush.h"
#include "errors.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/trace.h"
#include "oil.h"
#include "opponent.h"
#include "pedestrn.h"
#include "replay.h"
#include "skidmark.h"
#include "spark.h"
#include "sys.h"
#include "sound.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>
#include <string.h>

tU8* gPipe_buffer_start = NULL;
int gDisable_sound = 0;
int gDisable_advance = 0;
int gMax_rewind_chunks = 1000;
float gWall_severity = 0.f;
tPipe_reset_proc* gReset_procs[32] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ResetAllPedestrians,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ResetAllPedGibs,
    NULL,
    ResetSparks,
    ResetShrapnel,
    ResetScreenWobble,
    NULL,
    NULL,
    ResetSmoke,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ResetProxRay,
    NULL,
};
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
tPipe_chunk *gIncidentChunk; // FIXME: added by DethRace (really needed?)

#define LOCAL_BUFFER_SIZE 15000

#if DETHRACE_REPLAY_DEBUG
#define REPLAY_DEBUG_MAGIC1 0x1ed6ef85
#define REPLAY_DEBUG_ASSERT(test) assert(test)
#include <assert.h>
#else
#define REPLAY_DEBUG_ASSERT(test)
#endif

#if defined(DETHRACE_FIX_BUGS)
#define PIPE_ALIGN(V) (((V) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))
#endif

// IDA: void __usercall GetReducedPos(br_vector3 *v@<EAX>, tReduced_pos *p@<EDX>)
void GetReducedPos(br_vector3* v, tReduced_pos* p) {
    LOG_TRACE("(%p, %p)", v, p);

    v->v[0] = p->v[0] / 800.f;
    v->v[1] = p->v[1] / 800.f;
    v->v[2] = p->v[2] / 800.f;
    BrVector3Accumulate(v, &gProgram_state.current_car.car_master_actor->t.t.translate.t);
}

// IDA: void __usercall SaveReducedPos(tReduced_pos *p@<EAX>, br_vector3 *v@<EDX>)
void SaveReducedPos(tReduced_pos* p, br_vector3* v) {
    br_vector3 tv;
    LOG_TRACE("(%p, %p)", p, v);

    BrVector3Sub(&tv, v, &gProgram_state.current_car.car_master_actor->t.t.translate.t);
    p->v[0] = tv.v[0] * 800.f;
    p->v[1] = tv.v[1] * 800.f;
    p->v[2] = tv.v[2] * 800.f;
}

// IDA: int __cdecl PipeSearchForwards()
int PipeSearchForwards() {
    LOG_TRACE("()");

    if (gPipe_play_ptr == gPipe_record_ptr) {
        return 0;
    }
    if (gPipe_play_ptr == gPipe_buffer_oldest) {
        return 1;
    }
    if (GetReplayRate() == 0.f) {
        return GetReplayDirection() > 0;
    } else {
        return GetReplayRate() > 0.f;
    }
}

// IDA: int __cdecl IsActionReplayAvailable()
int IsActionReplayAvailable() {
    LOG_TRACE("()");

    return gPipe_buffer_start != NULL;
}

// IDA: int __cdecl SomeReplayLeft()
int SomeReplayLeft() {
    LOG_TRACE("()");

    return ((GetReplayDirection() >= 1 && gPipe_play_ptr != gPipe_record_ptr) ||
        (GetReplayDirection() <= -1 && gPipe_play_ptr != gPipe_buffer_oldest));
}

// IDA: void __cdecl DisablePipedSounds()
void DisablePipedSounds() {
    LOG_TRACE("()");

    gDisable_sound = 1;
}

// IDA: void __cdecl EnablePipedSounds()
void EnablePipedSounds() {
    LOG_TRACE("()");

    gDisable_sound = 0;
}

// IDA: tU32 __usercall LengthOfSession@<EAX>(tPipe_session *pSession@<EAX>)
tU32 LengthOfSession(tPipe_session* pSession) {
    int i;
    tU32 running_total;
    tPipe_chunk* the_chunk;
    LOG_TRACE("(%p)", pSession);

#define SIZEOF_CHUNK(MEMBER) (offsetof(tPipe_chunk, chunk_data) + sizeof(pSession->chunks.chunk_data.MEMBER))
#define ROUND_UP(V, M) (((V) + (M) - 1) & (~((M) - 1)))

    switch (pSession->chunk_type) {
    case ePipe_chunk_actor_rstyle:
        running_total = SIZEOF_CHUNK(actor_rstyle_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_actor_translate:
        running_total = SIZEOF_CHUNK(actor_translate_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_actor_transform:
        running_total = SIZEOF_CHUNK(actor_transform_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_actor_create:
        running_total = 0;
        break;
    case ePipe_chunk_actor_destroy:
        running_total = 0;
        break;
    case ePipe_chunk_actor_relink:
        running_total = SIZEOF_CHUNK(actor_relink_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_actor_material:
        running_total = SIZEOF_CHUNK(actor_material_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_face_material:
        running_total = SIZEOF_CHUNK(face_material_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_material_trans:
        running_total = SIZEOF_CHUNK(material_trans_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_material_pixelmap:
        running_total = SIZEOF_CHUNK(material_pixelmap_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_model_geometry:
        running_total = 0;
        for (i = 0; i < pSession->number_of_chunks; i++) {
            the_chunk = (tPipe_chunk*)&((tU8*)&pSession->chunks)[running_total];
            running_total += the_chunk->chunk_data.model_geometry_data.vertex_count * sizeof(tChanged_vertex) + offsetof(tPipe_model_geometry_data, vertex_changes) + offsetof(tPipe_chunk, chunk_data);
        }
        break;
    case ePipe_chunk_pedestrian:
        running_total = 0;
        for (i = 0; i < pSession->number_of_chunks; i++) {
            the_chunk = (tPipe_chunk*)&(((tU8*)&pSession->chunks)[running_total]);
            if (the_chunk->chunk_data.pedestrian_data.hit_points <= 0) {
                running_total += SIZEOF_CHUNK(pedestrian_data);
            } else {
                running_total += offsetof(tPipe_pedestrian_data, spin_period) + offsetof(tPipe_chunk, chunk_data);
            }
        }
        break;
    case ePipe_chunk_frame_boundary:
        running_total = SIZEOF_CHUNK(frame_boundary_data);
        break;
    case ePipe_chunk_car:
        running_total = SIZEOF_CHUNK(car_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_sound:
        running_total = SIZEOF_CHUNK(sound_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_damage:
        running_total = SIZEOF_CHUNK(damage_data);
        break;
    case ePipe_chunk_special:
        running_total = SIZEOF_CHUNK(special_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_ped_gib:
        running_total = SIZEOF_CHUNK(ped_gib_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_incident:
        running_total = SIZEOF_CHUNK(incident_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_spark:
        running_total = SIZEOF_CHUNK(spark_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_shrapnel:
        running_total = 0;
        for (i = 0; i < pSession->number_of_chunks; i++) {
            the_chunk = (tPipe_chunk*)&((tU8*)&pSession->chunks)[running_total];
            if (the_chunk->subject_index & 0x8000) {
                running_total += SIZEOF_CHUNK(shrapnel_data);
            } else {
                running_total += offsetof(tPipe_shrapnel_data, age) + offsetof(tPipe_chunk, chunk_data);
            }
        }
        break;
    case ePipe_chunk_screen_shake:
        running_total = SIZEOF_CHUNK(screen_shake_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_groove_stop:
        running_total = SIZEOF_CHUNK(groove_stop_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_non_car:
        running_total = SIZEOF_CHUNK(non_car_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_smoke:
        running_total = SIZEOF_CHUNK(smoke_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_oil_spill:
        running_total = SIZEOF_CHUNK(oil_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_smoke_column:
        running_total = ROUND_UP(SIZEOF_CHUNK(smoke_column_data), 4) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_flame:
        running_total = SIZEOF_CHUNK(flame_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_smudge:
        running_total = 0;
        for (i = 0; i < pSession->number_of_chunks; i++) {
            the_chunk = (tPipe_chunk*)&((tU8*)&pSession->chunks)[running_total];
            running_total += the_chunk->chunk_data.smudge_data.vertex_count * sizeof(tSmudged_vertex) + offsetof(tPipe_smudge_data, vertex_changes) + offsetof(tPipe_chunk, chunk_data);
        }
        break;
    case ePipe_chunk_splash:
        running_total = SIZEOF_CHUNK(splash_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_prox_ray:
        running_total = SIZEOF_CHUNK(prox_ray_data) * pSession->number_of_chunks;
        break;
    case ePipe_chunk_skid_adjustment:
        running_total = SIZEOF_CHUNK(skid_adjustment) * pSession->number_of_chunks;
        break;
    default:
        running_total = 0;
        break;
    }
    running_total += offsetof(tPipe_session, chunks) + sizeof(tU16);
    if (running_total % 2 != 0) {
        FatalError(98);
    }
    return running_total;
}

// IDA: void __usercall StartPipingSession2(tPipe_chunk_type pThe_type@<EAX>, int pMunge_reentrancy@<EDX>)
void StartPipingSession2(tPipe_chunk_type pThe_type, int pMunge_reentrancy) {
    LOG_TRACE("(%d, %d)", pThe_type, pMunge_reentrancy);

    if (gPipe_buffer_start != NULL && !gAction_replay_mode && gProgram_state.racing) {
        if (pMunge_reentrancy) {
            if (gReentrancy_count != 0) {
                gReentrancy_array[gReentrancy_count - 1] = ((tPipe_session*)gLocal_buffer)->chunk_type;
                EndPipingSession2(0);
            }
            gReentrancy_count++;
        }
        ((tPipe_session*)gLocal_buffer)->chunk_type = pThe_type;
        ((tPipe_session*)gLocal_buffer)->number_of_chunks = 0;
#if defined(DETHRACE_REPLAY_DEBUG)
        ((tPipe_session*)gLocal_buffer)->magic1 = REPLAY_DEBUG_MAGIC1;
#endif
        gLocal_buffer_size = offsetof(tPipe_session, chunks);
        gMr_chunky = &((tPipe_session*)gLocal_buffer)->chunks;
    }
}

// IDA: void __usercall StartPipingSession(tPipe_chunk_type pThe_type@<EAX>)
void StartPipingSession(tPipe_chunk_type pThe_type) {
    LOG_TRACE("(%d)", pThe_type);

    StartPipingSession2(pThe_type, 1);
}

// IDA: void __usercall EndPipingSession2(int pMunge_reentrancy@<EAX>)
void EndPipingSession2(int pMunge_reentrancy) {
    int a;
    LOG_TRACE("(%d)", pMunge_reentrancy);

    if (gPipe_buffer_start != NULL && !gAction_replay_mode && gProgram_state.racing) {
        // Each session ends with a tU16, containing the session size
        *(tU16*)&gLocal_buffer[gLocal_buffer_size] = gLocal_buffer_size;
        a = gLocal_buffer_size;
        gLocal_buffer_size += sizeof(tU16);
        REPLAY_DEBUG_ASSERT(LengthOfSession((tPipe_session*)gLocal_buffer) == gLocal_buffer_size);
#if defined(DETHRACE_FIX_BUGS)
        gLocal_buffer_size = PIPE_ALIGN(gLocal_buffer_size);
        *(tU16*)&gLocal_buffer[gLocal_buffer_size - sizeof(tU16)] = gLocal_buffer_size - sizeof(tU16);
#endif
        if (((tPipe_session*)gLocal_buffer)->number_of_chunks != 0 && (gLocal_buffer_size < LOCAL_BUFFER_SIZE || a == LOCAL_BUFFER_SIZE - 2)) {
            if (gPipe_buffer_phys_end < gPipe_record_ptr + gLocal_buffer_size) {
                // Put session at begin of pipe, as no place at end
                gPipe_buffer_working_end = gPipe_record_ptr;
                gPipe_buffer_oldest = gPipe_buffer_start;
                gPipe_record_ptr = gPipe_buffer_start;
            }
            while (gPipe_record_ptr <= gPipe_buffer_oldest && gPipe_buffer_oldest < gPipe_record_ptr + gLocal_buffer_size) {
                // Remove older sessions
#if defined(DETHRACE_FIX_BUGS)
                gPipe_buffer_oldest += PIPE_ALIGN(LengthOfSession((tPipe_session*)gPipe_buffer_oldest));
#else
                gPipe_buffer_oldest += LengthOfSession((tPipe_session*)gPipe_buffer_oldest);
#endif
                if (gPipe_buffer_working_end <= gPipe_buffer_oldest) {
                    gPipe_buffer_working_end = gPipe_buffer_phys_end;
                    gPipe_buffer_oldest = gPipe_buffer_start;
                }
            }
            if (gPipe_buffer_oldest == NULL) {
                gPipe_buffer_oldest = gPipe_record_ptr;
            }
            memcpy(gPipe_record_ptr, gLocal_buffer, gLocal_buffer_size);
            gPipe_record_ptr += gLocal_buffer_size;
            if (gPipe_buffer_working_end < gPipe_record_ptr) {
                gPipe_buffer_working_end = gPipe_record_ptr;
            }
        }
        if (pMunge_reentrancy) {
            if (gReentrancy_count != 0) {
                gReentrancy_count--;
                if (gReentrancy_count != 0) {
                    StartPipingSession2(gReentrancy_array[gReentrancy_count - 1], 0);
                }
            }
        }
    }
}

// IDA: void __cdecl EndPipingSession()
void EndPipingSession() {
    LOG_TRACE("()");

    EndPipingSession2(1);
}

// IDA: void __usercall AddDataToSession(int pSubject_index@<EAX>, void *pData@<EDX>, tU32 pData_length@<EBX>)
void AddDataToSession(int pSubject_index, void* pData, tU32 pData_length) {
    tU32 temp_buffer_size;
    int variable_for_breaking_on;
    LOG_TRACE("(%d, %p, %d)", pSubject_index, pData, pData_length);

    if (gPipe_buffer_start != NULL && !gAction_replay_mode && gProgram_state.racing) {
        temp_buffer_size = gLocal_buffer_size + offsetof(tPipe_chunk, chunk_data) + pData_length;
        if (temp_buffer_size >= LOCAL_BUFFER_SIZE) {
            return;
        }
        ((tPipe_session*)gLocal_buffer)->number_of_chunks++;
        gMr_chunky->subject_index = pSubject_index;
        memcpy(&gMr_chunky->chunk_data, pData, pData_length);
        gMr_chunky = (tPipe_chunk*)(((tU8*)&gMr_chunky->chunk_data) + pData_length);
        gLocal_buffer_size = temp_buffer_size;
    }
}

// IDA: void __usercall AddModelGeometryToPipingSession(tU16 pCar_ID@<EAX>, int pModel_index@<EDX>, int pVertex_count@<EBX>, tChanged_vertex *pCoordinates@<ECX>)
void AddModelGeometryToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex* pCoordinates) {
    tU32 data_size;
    LOG_TRACE("(%d, %d, %d, %p)", pCar_ID, pModel_index, pVertex_count, pCoordinates);

    if (gModel_geometry_space != NULL) {
        if (pVertex_count > 600) {
            pVertex_count = 600;
        }
        data_size = offsetof(tPipe_model_geometry_data, vertex_changes) + pVertex_count * sizeof(tChanged_vertex);
        gModel_geometry_space->vertex_count = pVertex_count;
        gModel_geometry_space->model_index = pModel_index;
        memcpy(gModel_geometry_space->vertex_changes, pCoordinates, pVertex_count * sizeof(tChanged_vertex));
        AddDataToSession(pCar_ID, gModel_geometry_space, data_size);
    }
}

// IDA: void __usercall AddSmudgeToPipingSession(tU16 pCar_ID@<EAX>, int pModel_index@<EDX>, int pVertex_count@<EBX>, tSmudged_vertex *pCoordinates@<ECX>)
void AddSmudgeToPipingSession(tU16 pCar_ID, int pModel_index, int pVertex_count, tSmudged_vertex* pCoordinates) {
    tU32 data_size;
    LOG_TRACE("(%d, %d, %d, %p)", pCar_ID, pModel_index, pVertex_count, pCoordinates);

    if (gSmudge_space != NULL) {
        if (pVertex_count > 600) {
            pVertex_count = 600;
        }
        gSmudge_space->vertex_count = pVertex_count;
        gSmudge_space->model_index = pModel_index;
        memcpy(gSmudge_space->vertex_changes, pCoordinates, pVertex_count * sizeof(tSmudged_vertex));
        data_size = offsetof(tPipe_smudge_data, vertex_changes) + pVertex_count * sizeof(tSmudged_vertex);
        AddDataToSession(pCar_ID, gSmudge_space, data_size);
    }
}

// IDA: void __usercall AddPedestrianToPipingSession(int pPedestrian_index@<EAX>, br_matrix34 *pTrans@<EDX>, tU8 pAction_index@<EBX>, tU8 pFrame_index@<ECX>, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset)
void AddPedestrianToPipingSession(int pPedestrian_index, br_matrix34* pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset) {
    tPipe_pedestrian_data data;
    tU32 data_size;
    LOG_TRACE("(%d, %p, %d, %d, %d, %d, %d, %f, %f, %p)", pPedestrian_index, pTrans, pAction_index, pFrame_index, pHit_points, pDone_initial, pParent_ID, pSpin_period, pJump_magnitude, pOffset);

    if (pFrame_index == 0xff) {
        pFrame_index = 0;
    }
    data.action_and_frame_index = (pDone_initial ? 1 : 0) << 7 | pAction_index << 4 | pFrame_index;
    data.hit_points = pHit_points;
    data.new_translation.v[0] = pTrans->m[3][0];
    data.new_translation.v[1] = pTrans->m[3][1];
    data.new_translation.v[2] = pTrans->m[3][2];
    data.parent = pParent_ID;
    if (pHit_points <= 0) {
        data.spin_period = pSpin_period;
        data.parent_actor = GetPedestrianActor(pPedestrian_index)->parent;
        BrVector3Copy(&data.offset, pOffset);
        data.jump_magnitude = pJump_magnitude;
        data_size = sizeof(tPipe_pedestrian_data);
    } else {
        data_size = offsetof(tPipe_pedestrian_data, spin_period);
    }
    AddDataToSession(pPedestrian_index, &data, data_size);
}

// IDA: void __usercall AddSparkToPipingSession(int pSpark_index@<EAX>, br_vector3 *pPos@<EDX>, br_vector3 *pV@<EBX>)
void AddSparkToPipingSession(int pSpark_index, br_vector3* pPos, br_vector3* pV) {
    tPipe_spark_data data;
    LOG_TRACE("(%d, %p, %p)", pSpark_index, pPos, pV);

    BrVector3Copy(&data.pos, pPos);
    BrVector3Copy(&data.v, pV);
    AddDataToSession(pSpark_index, &data, sizeof(tPipe_spark_data));
}

// IDA: void __usercall AddShrapnelToPipingSession(int pShrapnel_index@<EAX>, br_vector3 *pPos@<EDX>, tU16 pAge@<EBX>, br_material *pMaterial@<ECX>)
void AddShrapnelToPipingSession(int pShrapnel_index, br_vector3* pPos, tU16 pAge, br_material* pMaterial) {
    tPipe_shrapnel_data data;
    tU32 data_size;
    LOG_TRACE("(%d, %p, %d, %p)", pShrapnel_index, pPos, pAge, pMaterial);

    BrVector3Copy(&data.pos, pPos);
    if ((pShrapnel_index & 0x8000) != 0) {
        data.age = pAge;
        data.material = pMaterial;
        data_size = sizeof(tPipe_shrapnel_data);
    } else {
        data_size = offsetof(tPipe_shrapnel_data, age);
    }
    AddDataToSession(pShrapnel_index, &data, data_size);
}

// IDA: void __usercall AddScreenWobbleToPipingSession(int pWobble_x@<EAX>, int pWobble_y@<EDX>)
void AddScreenWobbleToPipingSession(int pWobble_x, int pWobble_y) {
    tPipe_screen_shake_data data;
    LOG_TRACE("(%d, %d)", pWobble_x, pWobble_y);

    data.wobble_x = pWobble_x;
    data.wobble_y = pWobble_y;
    AddDataToSession(0, &data, sizeof(tPipe_screen_shake_data));
}

// IDA: void __usercall AddGrooveStopToPipingSession(int pGroove_index@<EAX>, br_matrix34 *pMatrix@<EDX>, int pPath_interrupt@<EBX>, int pObject_interrupt@<ECX>, float pPath_resumption, float pObject_resumption)
void AddGrooveStopToPipingSession(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    tPipe_groove_stop_data data;
    LOG_TRACE("(%d, %p, %d, %d, %f, %f)", pGroove_index, pMatrix, pPath_interrupt, pObject_interrupt, pPath_resumption, pObject_resumption);

    BrMatrix34Copy(&data.matrix, pMatrix);
    data.path_interrupt = pPath_interrupt;
    data.object_interrupt = pObject_interrupt;
    data.path_resumption = pPath_resumption;
    data.object_resumption = pObject_resumption;
    AddDataToSession(pGroove_index, &data, sizeof(tPipe_groove_stop_data));
}

// IDA: void __usercall AddNonCarToPipingSession(int pIndex@<EAX>, br_actor *pActor@<EDX>)
void AddNonCarToPipingSession(int pIndex, br_actor* pActor) {
    tPipe_non_car_data data;
    LOG_TRACE("(%d, %p)", pIndex, pActor);

    BrMatrix34Copy(&data.mat, &pActor->t.t.mat);
    data.actor = pActor;
    AddDataToSession(pIndex, &data, sizeof(tPipe_non_car_data));
}

// IDA: void __usercall AddSmokeToPipingSession(int pIndex@<EAX>, tU8 pType@<EDX>, br_vector3 *pPos@<EBX>, br_scalar pRadius, br_scalar pStrength)
void AddSmokeToPipingSession(int pIndex, tU8 pType, br_vector3* pPos, br_scalar pRadius, br_scalar pStrength) {
    tPipe_smoke_data data;
    LOG_TRACE("(%d, %d, %p, %f, %f)", pIndex, pType, pPos, pRadius, pStrength);

    SaveReducedPos(&data.pos, pPos);
    data.type = pType;
    data.radius = pRadius * 1024.f;
    data.strength = pStrength * 255.f;
    AddDataToSession(pIndex, &data, sizeof(tPipe_smoke_data));
}

// IDA: void __usercall AddSmokeColumnToPipingSession(int pIndex@<EAX>, tCar_spec *pCar@<EDX>, int pVertex@<EBX>, int pColour@<ECX>)
void AddSmokeColumnToPipingSession(int pIndex, tCar_spec* pCar, int pVertex, int pColour) {
    tPipe_smoke_column_data data;
    LOG_TRACE("(%d, %p, %d, %d)", pIndex, pCar, pVertex, pColour);

    data.car_ID = pCar->car_ID;
    data.vertex = pVertex;
    AddDataToSession(pColour << 14 | pIndex, &data, sizeof(tPipe_smoke_column_data));
}

// IDA: void __usercall AddFlameToPipingSession(int pIndex@<EAX>, int pFrame_count@<EDX>, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z)
void AddFlameToPipingSession(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z) {
    tPipe_flame_data data;
    LOG_TRACE("(%d, %d, %f, %f, %f, %f)", pIndex, pFrame_count, pScale_x, pScale_y, pOffset_x, pOffset_z);

    data.frame_count = pFrame_count;
    data.scale_x = pScale_x;
    data.scale_y = pScale_y;
    data.offset_x = pOffset_x;
    data.offset_z = pOffset_z;
    AddDataToSession(pIndex, &data, sizeof(tPipe_flame_data));
}

// IDA: void __usercall AddSplashToPipingSession(tCollision_info *pCar@<EAX>)
void AddSplashToPipingSession(tCollision_info* pCar) {
    tPipe_splash_data data;
    LOG_TRACE("(%p)", pCar);

    if (pCar->driver >= eDriver_oppo) {
        data.d = pCar->water_d;
        BrVector3Copy(&data.normal, &pCar->water_normal);
        AddDataToSession(pCar->car_ID, &data, sizeof(tPipe_splash_data));
    }
}

// IDA: void __usercall AddOilSpillToPipingSession(int pIndex@<EAX>, br_matrix34 *pMat@<EDX>, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap)
void AddOilSpillToPipingSession(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap) {
    tPipe_oil_spill_data data;
    LOG_TRACE("(%d, %p, %f, %f, %d, %d, %p, %p, %p)", pIndex, pMat, pFull_size, pGrow_rate, pSpill_time, pStop_time, pCar, pOriginal_pos, pPixelmap);

    BrMatrix34Copy(&data.mat, pMat);
    data.full_size = pFull_size;
    data.grow_rate = pGrow_rate;
    data.spill_time = pSpill_time;
    data.previous_stop_time = pStop_time;
    data.car = pCar;
    BrVector3Copy(&data.original_pos, pOriginal_pos);
    data.pixelmap = pPixelmap;
    AddDataToSession(pIndex, &data, sizeof(tPipe_oil_spill_data));
}

// IDA: void __usercall AddFrameFinishToPipingSession(tU32 pThe_time@<EAX>)
void AddFrameFinishToPipingSession(tU32 pThe_time) {
    tPipe_frame_boundary_data data;
    LOG_TRACE("(%d)", pThe_time);

    data.time = pThe_time;
    AddDataToSession(0, &data, sizeof(tPipe_frame_boundary_data));
}

// IDA: void __usercall AddCarToPipingSession(int pCar_ID@<EAX>, br_matrix34 *pCar_mat@<EDX>, br_vector3 *pCar_velocity@<EBX>, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag)
void AddCarToPipingSession(int pCar_ID, br_matrix34* pCar_mat, br_vector3* pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag) {
    tPipe_car_data data;
    LOG_TRACE("(%d, %p, %p, %f, %f, %f, %f, %f, %f, %f, %d, %d)", pCar_ID, pCar_mat, pCar_velocity, pSpeedo_speed, pLf_sus_position, pRf_sus_position, pLr_sus_position, pRr_sus_position, pSteering_angle, pRevs, pGear, pFrame_coll_flag);

    BrMatrix34Copy(&data.transformation, pCar_mat);
    BrVector3Copy(&data.velocity, pCar_velocity);
    data.speedo_speed = pSpeedo_speed * 32767.f / 0.07f;
    data.lf_sus_position = pLf_sus_position * 127.f / .15f;
    data.rf_sus_position = pRf_sus_position * 127.f / .15f;
    data.lr_sus_position = pLr_sus_position * 127.f / .15f;
    data.rr_sus_position = pRr_sus_position * 127.f / .15f;
    data.steering_angle = pSteering_angle * 32767.f / 60.f;
    data.revs_and_gear = (pGear + 1) << 12 | (pFrame_coll_flag ? 0 : 1) << 11 | ((((int)pRevs) / 10) & 0x7ff);
    AddDataToSession(pCar_ID, &data, sizeof(tPipe_car_data));
}

// IDA: void __usercall AddSoundToPipingSession(tS3_outlet_ptr pOutlet@<EAX>, int pSound_index@<EDX>, tS3_volume pL_volume@<EBX>, tS3_volume pR_volume@<ECX>, tS3_pitch pPitch, br_vector3 *pPos)
void AddSoundToPipingSession(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3* pPos) {
    tPipe_sound_data data;
    LOG_TRACE("(%d, %d, %d, %d, %d, %p)", pOutlet, pSound_index, pL_volume, pR_volume, pPitch, pPos);

    data.pitch = pPitch;
    if (pPos == NULL) {
        BrVector3Set(&data.position, 0.f, 0.f, 0.f);
    } else {
        BrVector3Copy(&data.position, pPos);
    }
    data.volume = (pR_volume << 8) | (pL_volume << 0);
    data.outlet_index = GetIndexFromOutlet(pOutlet);
    AddDataToSession(pSound_index, &data, sizeof(tPipe_sound_data));
}

// IDA: void __usercall AddDamageToPipingSession(int pCar_ID@<EAX>, tS8 *pDifferences@<EDX>)
void AddDamageToPipingSession(int pCar_ID, tS8* pDifferences) {
    tPipe_damage_data data;
    int i;
    LOG_TRACE("(%d, %p)", pCar_ID, pDifferences);

    for (i = 0; i < COUNT_OF(data.damage_delta); i++) {
        data.damage_delta[i] = pDifferences[i];
    }
    AddDataToSession(pCar_ID, &data, sizeof(tPipe_damage_data));
}

// IDA: void __usercall AddSpecialToPipingSession(tSpecial_type pType@<EAX>)
void AddSpecialToPipingSession(tSpecial_type pType) {
    tPipe_special_data data;
    LOG_TRACE("(%d)", pType);

    AddDataToSession(pType, &data, sizeof(tPipe_special_data));
}

// IDA: void __usercall AddPedGibToPipingSession(int pIndex@<EAX>, br_matrix34 *pTrans@<EDX>, int pSize@<EBX>, int pGib_index@<ECX>, int pPed_index)
void AddPedGibToPipingSession(int pIndex, br_matrix34* pTrans, int pSize, int pGib_index, int pPed_index) {
    tPipe_ped_gib_data data;
    LOG_TRACE("(%d, %p, %d, %d, %d)", pIndex, pTrans, pSize, pGib_index, pPed_index);

    data.ped_parent_index = pPed_index;
    data.size = pSize;
    data.gib_index = pGib_index;
    BrMatrix34Copy(&data.transform, pTrans);
    AddDataToSession(pIndex, &data, sizeof(tPipe_ped_gib_data));
}

// IDA: void __cdecl AddCarIncidentToPipingSession(float pSeverity, tCar_spec *pCar, br_vector3 *pImpact_point)
void AddCarIncidentToPipingSession(float pSeverity, tCar_spec* pCar, br_vector3* pImpact_point) {
    tPipe_incident_data data;
    LOG_TRACE("(%f, %p, %p)", pSeverity, pCar, pImpact_point);

    data.severity = pSeverity;
    data.info.car_info.car_ID = pCar->car_ID;
    BrVector3Copy(&data.info.car_info.impact_point, pImpact_point);
    AddDataToSession(1, &data, sizeof(tPipe_incident_data));
}

// IDA: void __usercall AddPedIncidentToPipingSession(int pPed_index@<EAX>, br_actor *pActor@<EDX>)
void AddPedIncidentToPipingSession(int pPed_index, br_actor* pActor) {
    tPipe_incident_data data;
    LOG_TRACE("(%d, %p)", pPed_index, pActor);

    data.severity = 0.f;
    data.info.ped_info.ped_index = pPed_index;
    data.info.ped_info.actor = pActor;
    AddDataToSession(0, &data, sizeof(tPipe_incident_data));
}

// IDA: void __cdecl AddWallIncidentToPipingSession(float pSeverity, br_vector3 *pImpact_point)
void AddWallIncidentToPipingSession(float pSeverity, br_vector3* pImpact_point) {
    tPipe_incident_data data;
    LOG_TRACE("(%f, %p)", pSeverity, pImpact_point);

    data.severity = pSeverity;
    BrVector3Copy(&data.info.wall_info.pos, pImpact_point);
    AddDataToSession(2, &data, sizeof(tPipe_incident_data));
}

// IDA: void __usercall AddProxRayToPipingSession(int pRay_index@<EAX>, tCar_spec *pCar@<EDX>, tU16 pPed_index@<EBX>, tU32 pTime@<ECX>)
void AddProxRayToPipingSession(int pRay_index, tCar_spec* pCar, tU16 pPed_index, tU32 pTime) {
    tPipe_prox_ray_data data;
    LOG_TRACE("(%d, %p, %d, %d)", pRay_index, pCar, pPed_index, pTime);

    data.ped_index = pPed_index;
    data.car_ID = pCar->car_ID;
    data.time = pTime;
    AddDataToSession(pRay_index, &data, sizeof(tPipe_prox_ray_data));
}

// IDA: void __usercall AddSkidAdjustmentToPipingSession(int pSkid_num@<EAX>, br_matrix34 *pMatrix@<EDX>, int pMaterial_index@<EBX>)
void AddSkidAdjustmentToPipingSession(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    tPipe_skid_adjustment adjustment;
    LOG_TRACE("(%d, %p, %d)", pSkid_num, pMatrix, pMaterial_index);

    BrMatrix34Copy(&adjustment.matrix, pMatrix);
    adjustment.material_index = pMaterial_index;
    AddDataToSession(pSkid_num, &adjustment, sizeof(tPipe_skid_adjustment));
}

// IDA: void __usercall PipeSingleModelGeometry(tU16 pCar_ID@<EAX>, int pModel_index@<EDX>, int pVertex_count@<EBX>, tChanged_vertex *pCoordinates@<ECX>)
void PipeSingleModelGeometry(tU16 pCar_ID, int pModel_index, int pVertex_count, tChanged_vertex* pCoordinates) {
    LOG_TRACE("(%d, %d, %d, %p)", pCar_ID, pModel_index, pVertex_count, pCoordinates);

    StartPipingSession(ePipe_chunk_model_geometry);
    AddModelGeometryToPipingSession(pCar_ID, pModel_index, pVertex_count, pCoordinates);
    EndPipingSession();
}

// IDA: void __usercall PipeSinglePedestrian(int pPedestrian_index@<EAX>, br_matrix34 *pTrans@<EDX>, tU8 pAction_index@<EBX>, tU8 pFrame_index@<ECX>, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset)
void PipeSinglePedestrian(int pPedestrian_index, br_matrix34* pTrans, tU8 pAction_index, tU8 pFrame_index, tS8 pHit_points, int pDone_initial, tU16 pParent_ID, float pSpin_period, br_scalar pJump_magnitude, br_vector3* pOffset) {
    LOG_TRACE("(%d, %p, %d, %d, %d, %d, %d, %f, %f, %p)", pPedestrian_index, pTrans, pAction_index, pFrame_index, pHit_points, pDone_initial, pParent_ID, pSpin_period, pJump_magnitude, pOffset);

    StartPipingSession(ePipe_chunk_pedestrian);
    AddPedestrianToPipingSession(pPedestrian_index, pTrans, pAction_index,
        pFrame_index, pHit_points, pDone_initial, pParent_ID, pSpin_period,
        pJump_magnitude, pOffset);
    EndPipingSession();
}

// IDA: void __usercall PipeSingleCar(int pCar_ID@<EAX>, br_matrix34 *pCar_mat@<EDX>, br_vector3 *pCar_velocity@<EBX>, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag)
void PipeSingleCar(int pCar_ID, br_matrix34* pCar_mat, br_vector3* pCar_velocity, float pSpeedo_speed, float pLf_sus_position, float pRf_sus_position, float pLr_sus_position, float pRr_sus_position, float pSteering_angle, br_scalar pRevs, int pGear, int pFrame_coll_flag) {
    LOG_TRACE("(%d, %p, %p, %f, %f, %f, %f, %f, %f, %f, %d, %d)", pCar_ID, pCar_mat, pCar_velocity, pSpeedo_speed, pLf_sus_position, pRf_sus_position, pLr_sus_position, pRr_sus_position, pSteering_angle, pRevs, pGear, pFrame_coll_flag);

    StartPipingSession(ePipe_chunk_car);
    AddCarToPipingSession(pCar_ID, pCar_mat, pCar_velocity, pSpeedo_speed,
        pLf_sus_position, pRf_sus_position, pLr_sus_position, pRr_sus_position,
        pSteering_angle, pRevs, pGear, pFrame_coll_flag);
    EndPipingSession();
}

// IDA: void __usercall PipeSingleSound(tS3_outlet_ptr pOutlet@<EAX>, int pSound_index@<EDX>, tS3_volume pL_volume@<EBX>, tS3_volume pR_volume@<ECX>, tS3_pitch pPitch, br_vector3 *pPos)
void PipeSingleSound(tS3_outlet_ptr pOutlet, int pSound_index, tS3_volume pL_volume, tS3_volume pR_volume, tS3_pitch pPitch, br_vector3* pPos) {
    LOG_TRACE("(%d, %d, %d, %d, %d, %p)", pOutlet, pSound_index, pL_volume, pR_volume, pPitch, pPos);

    if (!gAction_replay_mode && gProgram_state.racing) {
        StartPipingSession(ePipe_chunk_sound);
        AddSoundToPipingSession(pOutlet, pSound_index, pL_volume, pR_volume, pPitch, pPos);
        EndPipingSession();
    }
}

// IDA: void __usercall PipeSingleOilSpill(int pIndex@<EAX>, br_matrix34 *pMat@<EDX>, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap)
void PipeSingleOilSpill(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap) {
    LOG_TRACE("(%d, %p, %f, %f, %d, %d, %p, %p, %p)", pIndex, pMat, pFull_size, pGrow_rate, pSpill_time, pStop_time, pCar, pOriginal_pos, pPixelmap);

    StartPipingSession(ePipe_chunk_oil_spill);
    AddOilSpillToPipingSession(pIndex, pMat, pFull_size, pGrow_rate,
        pSpill_time, pStop_time, pCar, pOriginal_pos, pPixelmap);
    EndPipingSession();
}

// IDA: void __usercall PipeSingleDamage(int pCar_ID@<EAX>, tS8 *pDifferences@<EDX>)
void PipeSingleDamage(int pCar_ID, tS8* pDifferences) {
    LOG_TRACE("(%d, %p)", pCar_ID, pDifferences);

    StartPipingSession(ePipe_chunk_damage);
    AddDamageToPipingSession(pCar_ID, pDifferences);
    EndPipingSession();
}

// IDA: void __usercall PipeSingleSpecial(tSpecial_type pType@<EAX>)
void PipeSingleSpecial(tSpecial_type pType) {
    LOG_TRACE("(%d)", pType);

    StartPipingSession(ePipe_chunk_special);
    AddSpecialToPipingSession(pType);
    EndPipingSession();
}

// IDA: void __usercall PipeSinglePedGib(int pIndex@<EAX>, br_matrix34 *pTrans@<EDX>, int pSize@<EBX>, int pGib_index@<ECX>, int pPed_index)
void PipeSinglePedGib(int pIndex, br_matrix34* pTrans, int pSize, int pGib_index, int pPed_index) {
    LOG_TRACE("(%d, %p, %d, %d, %d)", pIndex, pTrans, pSize, pGib_index, pPed_index);

    StartPipingSession(ePipe_chunk_ped_gib);
    AddPedGibToPipingSession(pIndex, pTrans, pSize, pGib_index, pPed_index);
    EndPipingSession();
}

// IDA: void __cdecl PipeSingleCarIncident(float pSeverity, tCar_spec *pCar, br_vector3 *pImpact_point)
void PipeSingleCarIncident(float pSeverity, tCar_spec* pCar, br_vector3* pImpact_point) {
    LOG_TRACE("(%f, %p, %p)", pSeverity, pCar, pImpact_point);

    StartPipingSession(ePipe_chunk_incident);
    AddCarIncidentToPipingSession(pSeverity, pCar, pImpact_point);
    EndPipingSession();
}

// IDA: void __usercall PipeSinglePedIncident(int pPed_index@<EAX>, br_actor *pActor@<EDX>)
void PipeSinglePedIncident(int pPed_index, br_actor* pActor) {
    LOG_TRACE("(%d, %p)", pPed_index, pActor);

    StartPipingSession(ePipe_chunk_incident);
    AddPedIncidentToPipingSession(pPed_index, pActor);
    EndPipingSession();
    gWall_severity = 0.f;
}

// IDA: void __cdecl PipeSingleWallIncident(float pSeverity, br_vector3 *pImpact_point)
void PipeSingleWallIncident(float pSeverity, br_vector3* pImpact_point) {
    LOG_TRACE("(%f, %p)", pSeverity, pImpact_point);

    if (pSeverity > gWall_severity) {
        gWall_severity = pSeverity;
        BrVector3Copy(&gWall_impact_point, pImpact_point);
    }
}

// IDA: void __usercall PipeSingleScreenShake(int pWobble_x@<EAX>, int pWobble_y@<EDX>)
void PipeSingleScreenShake(int pWobble_x, int pWobble_y) {
    LOG_TRACE("(%d, %d)", pWobble_x, pWobble_y);

    StartPipingSession(ePipe_chunk_screen_shake);
    AddScreenWobbleToPipingSession(pWobble_x, pWobble_y);
    EndPipingSession();
}

// IDA: void __usercall PipeSingleGrooveStop(int pGroove_index@<EAX>, br_matrix34 *pMatrix@<EDX>, int pPath_interrupt@<EBX>, int pObject_interrupt@<ECX>, float pPath_resumption, float pObject_resumption)
void PipeSingleGrooveStop(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    LOG_TRACE("(%d, %p, %d, %d, %f, %f)", pGroove_index, pMatrix, pPath_interrupt, pObject_interrupt, pPath_resumption, pObject_resumption);

    StartPipingSession(ePipe_chunk_groove_stop);
    AddGrooveStopToPipingSession(pGroove_index, pMatrix, pPath_interrupt,
        pObject_interrupt, pPath_resumption, pObject_resumption);
    EndPipingSession();
}

// IDA: void __cdecl PipeFrameFinish()
void PipeFrameFinish() {
    LOG_TRACE("()");

    if (gWall_severity != 0.f) {
        StartPipingSession(ePipe_chunk_incident);
        AddWallIncidentToPipingSession(gWall_severity, &gWall_impact_point);
        EndPipingSession();
        gWall_severity = 0.f;
    }
    StartPipingSession(ePipe_chunk_frame_boundary);
    AddFrameFinishToPipingSession(GetTotalTime());
    EndPipingSession();
}

// IDA: void __cdecl PipingFrameReset()
void PipingFrameReset() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gReset_procs); i++) {
        if (gReset_procs[i] != NULL) {
            gReset_procs[i]();
        }
    }
}

// IDA: void __usercall PipeSingleSkidAdjustment(int pSkid_num@<EAX>, br_matrix34 *pMatrix@<EDX>, int pMaterial_index@<EBX>)
void PipeSingleSkidAdjustment(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    LOG_TRACE("(%d, %p, %d)", pSkid_num, pMatrix, pMaterial_index);

    StartPipingSession(ePipe_chunk_skid_adjustment);
    AddSkidAdjustmentToPipingSession(pSkid_num, pMatrix, pMaterial_index);
    EndPipingSession();
}

// IDA: void __cdecl ResetPiping()
void ResetPiping() {
    LOG_TRACE("()");

    gWall_severity = 0.f;
    gPipe_buffer_oldest = NULL;
    gPipe_record_ptr = gPipe_buffer_start;
    gPipe_buffer_working_end = gPipe_buffer_phys_end;
    gReentrancy_count = 0;
}

// IDA: void __cdecl InitialisePiping()
void InitialisePiping() {
    LOG_TRACE("()");

    if (!gAusterity_mode && gNet_mode == eNet_mode_none) {
        PDAllocateActionReplayBuffer((char**)&gPipe_buffer_start, &gPipe_buffer_size);
        gPipe_buffer_phys_end = gPipe_buffer_start + gPipe_buffer_size;
        gSmudge_space = BrMemAllocate(offsetof(tPipe_smudge_data, vertex_changes) + sizeof(tSmudged_vertex) * 2400, kMem_pipe_model_geometry);
        // DAT_00532008 = 0;
        BrVector3SetFloat(&gZero_vector, 0.f, 0.f, 0.f);
        gModel_geometry_space = (tPipe_model_geometry_data*)gSmudge_space;
        gLocal_buffer = BrMemAllocate(LOCAL_BUFFER_SIZE, kMem_pipe_model_geometry);
    } else {
        gPipe_buffer_start = NULL;
        gLocal_buffer = NULL;
        gModel_geometry_space = NULL;
        gSmudge_space = NULL;
    }
    ResetPiping();
}

// IDA: void __cdecl DisposePiping()
void DisposePiping() {
    LOG_TRACE("()");

    if (gPipe_buffer_start != NULL) {
        PDDisposeActionReplayBuffer((char*)gPipe_buffer_start);
    }
    gPipe_buffer_start = NULL;
    if (gModel_geometry_space != NULL) {
        BrMemFree(gModel_geometry_space);
        gModel_geometry_space = NULL;
    }
    if (gLocal_buffer != NULL) {
        BrMemFree(gLocal_buffer);
        gLocal_buffer = NULL;
    }
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
            car->car_ID = (cat << 8) | i;
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

    for (cat = eVehicle_self; cat < eVehicle_not_really; cat++) {
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
            car->active = 0;
        }
    }
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

    StartPipingSession(ePipe_chunk_car);
    for (cat = eVehicle_self; cat < eVehicle_not_really; cat++) {
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
            AddCarToPipingSession((cat << 8) | i,
                &car->car_master_actor->t.t.mat, &car->v, car->speedo_speed,
                car->lf_sus_position, car->rf_sus_position, car->lr_sus_position, car->rr_sus_position,
                car->steering_angle, car->revs, car->gear, car->frame_collision_flag);
        }
    }
    EndPipingSession();
    session_started = 0;
    for (cat = eVehicle_self; cat < eVehicle_net_player; cat++) {
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
            if (car->active) {
                difference_found = 0;
                for (j = 0; j < COUNT_OF(car->damage_units); j++) {
                    damage_deltas[j] = car->damage_units[j].damage_level - car->frame_start_damage[j];
                    difference_found |= damage_deltas[j];
                    car->frame_start_damage[j] = car->damage_units[j].damage_level;
                }
                if (difference_found) {
                    if (!session_started) {
                        StartPipingSession(ePipe_chunk_damage);
                        session_started = 1;
                    }
                    AddDamageToPipingSession((cat << 8) | i, damage_deltas);
                }
            }
        }
    }
    if (session_started) {
        EndPipingSession();
    }
}

// IDA: void __cdecl ResetPipePlayToEnd()
void ResetPipePlayToEnd() {
    LOG_TRACE("()");

    gPipe_play_ptr = gPipe_record_ptr;
}

// IDA: void __cdecl ResetPipePlayToStart()
void ResetPipePlayToStart() {
    LOG_TRACE("()");

    gPipe_play_ptr = gPipe_buffer_oldest;
}

// IDA: tU8* __cdecl GetPipePlayPtr()
tU8* GetPipePlayPtr() {
    LOG_TRACE("()");

    return gPipe_play_ptr;
}

// IDA: void __usercall SetPipePlayPtr(tU8 *pPtr@<EAX>)
void SetPipePlayPtr(tU8* pPtr) {
    LOG_TRACE("(%p)", pPtr);

    gPipe_play_ptr = pPtr;
}

// IDA: void __usercall AdvanceChunkPtr(tPipe_chunk **pChunk@<EAX>, tChunk_subject_index pType@<EDX>)
void AdvanceChunkPtr(tPipe_chunk** pChunk, tChunk_subject_index pType) {
    tPipe_chunk* old_chunk;
    LOG_TRACE("(%p, %d)", pChunk, pType);

    old_chunk = *pChunk;
    if (gDisable_advance) {
        return;
    }
    switch (pType) {
    case ePipe_chunk_model_geometry:
        *(tU8**)pChunk += offsetof(tPipe_model_geometry_data, vertex_changes) + (*pChunk)->chunk_data.model_geometry_data.vertex_count * sizeof(tChanged_vertex);
        break;
    case ePipe_chunk_pedestrian:
        *(tU8**)pChunk += (((*pChunk)->chunk_data.pedestrian_data.hit_points <= 0) ? sizeof(tPipe_pedestrian_data) : offsetof(tPipe_pedestrian_data, spin_period));
        break;
    case ePipe_chunk_frame_boundary:
        *(tU8**)pChunk += sizeof(tPipe_frame_boundary_data);
        break;
    case ePipe_chunk_car:
        *(tU8**)pChunk += sizeof(tPipe_car_data);
        break;
    case ePipe_chunk_sound:
        *(tU8**)pChunk += sizeof(tPipe_sound_data);
        break;
    case ePipe_chunk_damage:
        *(tU8**)pChunk += sizeof(tPipe_damage_data);
        break;
    case ePipe_chunk_special:
        *(tU8**)pChunk += sizeof(tPipe_special_data);
        break;
    case ePipe_chunk_ped_gib:
        *(tU8**)pChunk += sizeof(tPipe_ped_gib_data);
        break;
    case ePipe_chunk_incident:
        *(tU8**)pChunk += sizeof(tPipe_incident_data);
        break;
    case ePipe_chunk_spark:
        *(tU8**)pChunk += sizeof(tPipe_spark_data);
        break;
    case ePipe_chunk_shrapnel:
        *(tU8**)pChunk += (((*pChunk)->subject_index & 0x8000) ? sizeof(tPipe_shrapnel_data) : offsetof(tPipe_shrapnel_data, age));
        break;
    case ePipe_chunk_screen_shake:
        *(tU8**)pChunk += sizeof(tPipe_screen_shake_data);
        break;
    case ePipe_chunk_groove_stop:
        *(tU8**)pChunk += sizeof(tPipe_groove_stop_data);
        break;
    case ePipe_chunk_non_car:
        *(tU8**)pChunk += sizeof(tPipe_non_car_data);
        break;
    case ePipe_chunk_smoke:
        *(tU8**)pChunk += sizeof(tPipe_smoke_data);
        break;
    case ePipe_chunk_oil_spill:
        *(tU8**)pChunk += sizeof(tPipe_oil_spill_data);
        break;
    case ePipe_chunk_smoke_column:
        *(tU8**)pChunk += sizeof(tPipe_smoke_column_data);
        break;
    case ePipe_chunk_flame:
        *(tU8**)pChunk += sizeof(tPipe_flame_data);
        break;
    case ePipe_chunk_smudge:
        *(tU8**)pChunk += offsetof(tPipe_smudge_data, vertex_changes) + (*pChunk)->chunk_data.smudge_data.vertex_count * sizeof(tSmudged_vertex);
        break;
    case ePipe_chunk_splash:
        *(tU8**)pChunk += sizeof(tPipe_splash_data);
        break;
    case ePipe_chunk_prox_ray:
        *(tU8**)pChunk += sizeof(tPipe_prox_ray_data);
        break;
    case ePipe_chunk_skid_adjustment:
        *(tU8**)pChunk += sizeof(tPipe_skid_adjustment);
        break;
    }
    *(tU8**)pChunk += offsetof(tPipe_chunk, chunk_data);
    if (*(tU8**)pChunk == gEnd_of_session) {
        *pChunk = old_chunk;
    } else if (*(tU8**)pChunk > gEnd_of_session) {
        *pChunk = old_chunk;
    }
}

// IDA: void __usercall ApplyModelGeometry(tPipe_chunk **pChunk@<EAX>)
void ApplyModelGeometry(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    LOG_TRACE("(%p)", pChunk);

    if (((*pChunk)->subject_index & 0xff00) == 0) {
        car = &gProgram_state.current_car;
    } else {
        car = GetCarSpec((*pChunk)->subject_index >> 8, (*pChunk)->subject_index & 0xff);
    }
    model_ptr = car->car_model_actors[(*pChunk)->chunk_data.model_geometry_data.model_index].actor->model;
    for (i = 0; i < (*pChunk)->chunk_data.model_geometry_data.vertex_count; i++) {
        BrVector3Accumulate(&model_ptr->vertices[(*pChunk)->chunk_data.model_geometry_data.vertex_changes[i].vertex_index].p,
            &(*pChunk)->chunk_data.model_geometry_data.vertex_changes[i].delta_coordinates);
    }
    SetModelForUpdate(model_ptr, car, 0);
    AdvanceChunkPtr(pChunk, ePipe_chunk_model_geometry);
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

    if (((*pChunk)->subject_index & 0xff00) == 0) {
        car = &gProgram_state.current_car;
    } else {
        car = GetCarSpec((*pChunk)->subject_index >> 8, (*pChunk)->subject_index & 0xff);
    }
    model_ptr = car->car_model_actors[(*pChunk)->chunk_data.smudge_data.model_index].actor->model;
    for (i = 0; i < (*pChunk)->chunk_data.smudge_data.vertex_count; i++) {
        v = (*pChunk)->chunk_data.smudge_data.vertex_changes[i].vertex_index;
        inc = (*pChunk)->chunk_data.smudge_data.vertex_changes[i].light_index * pDir;
        V11MODEL(model_ptr)->groups->vertex_colours[v] = ((V11MODEL(model_ptr)->groups->vertex_colours[v] >> 24) + inc) << 24;
        if (model_ptr->flags & BR_MODF_UPDATEABLE) {
            model_ptr->vertices[V11MODEL(model_ptr)->groups->vertex_user[v]].index = (V11MODEL(model_ptr)->groups->vertex_colours[v] >> 24) + inc;
        }
    }
}

// IDA: void __usercall ApplySmudge(tPipe_chunk **pChunk@<EAX>)
void ApplySmudge(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    DoSmudge(pChunk, 1);
    AdvanceChunkPtr(pChunk, ePipe_chunk_smudge);
}

// IDA: void __usercall ApplyPedestrian(tPipe_chunk **pChunk@<EAX>)
void ApplyPedestrian(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustPedestrian(
        (*pChunk)->subject_index,
        ((*pChunk)->chunk_data.pedestrian_data.action_and_frame_index & 0x70) >> 4,
        (*pChunk)->chunk_data.pedestrian_data.action_and_frame_index & 0x0f,
        (*pChunk)->chunk_data.pedestrian_data.hit_points,
        (*pChunk)->chunk_data.pedestrian_data.action_and_frame_index >> 7,
        (*pChunk)->chunk_data.pedestrian_data.parent,
        (*pChunk)->chunk_data.pedestrian_data.parent_actor,
        (*pChunk)->chunk_data.pedestrian_data.spin_period,
        (*pChunk)->chunk_data.pedestrian_data.jump_magnitude,
        &(*pChunk)->chunk_data.pedestrian_data.offset,
        &(*pChunk)->chunk_data.pedestrian_data.new_translation);
    AdvanceChunkPtr(pChunk, ePipe_chunk_pedestrian);
}

// IDA: void __usercall ApplySpark(tPipe_chunk **pChunk@<EAX>)
void ApplySpark(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustSpark((*pChunk)->subject_index,
        &(*pChunk)->chunk_data.spark_data.pos,
        &(*pChunk)->chunk_data.spark_data.v);
    AdvanceChunkPtr(pChunk, ePipe_chunk_spark);
}

// IDA: void __usercall ApplyShrapnel(tPipe_chunk **pChunk@<EAX>)
void ApplyShrapnel(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustShrapnel((*pChunk)->subject_index,
        &(*pChunk)->chunk_data.shrapnel_data.pos,
        (*pChunk)->chunk_data.shrapnel_data.age,
        (*pChunk)->chunk_data.shrapnel_data.material);
    AdvanceChunkPtr(pChunk, ePipe_chunk_shrapnel);
}

// IDA: void __usercall ApplyScreenWobble(tPipe_chunk **pChunk@<EAX>)
void ApplyScreenWobble(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    SetScreenWobble((*pChunk)->chunk_data.screen_shake_data.wobble_x,
        (*pChunk)->chunk_data.screen_shake_data.wobble_y);
    AdvanceChunkPtr(pChunk, ePipe_chunk_screen_shake);
}

// IDA: void __usercall ApplyGrooveStop(tPipe_chunk **pChunk@<EAX>)
void ApplyGrooveStop(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    SetGrooveInterrupt((*pChunk)->subject_index,
        &(*pChunk)->chunk_data.groove_stop_data.matrix,
        (*pChunk)->chunk_data.groove_stop_data.path_interrupt,
        (*pChunk)->chunk_data.groove_stop_data.object_interrupt,
        (*pChunk)->chunk_data.groove_stop_data.path_resumption,
        (*pChunk)->chunk_data.groove_stop_data.object_resumption);
    AdvanceChunkPtr(pChunk, ePipe_chunk_groove_stop);
}

// IDA: void __usercall ApplyNonCar(tPipe_chunk **pChunk@<EAX>)
void ApplyNonCar(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustNonCar((*pChunk)->chunk_data.non_car_data.actor,
        &(*pChunk)->chunk_data.non_car_data.mat);
    AdvanceChunkPtr(pChunk, ePipe_chunk_non_car);
}

// IDA: void __usercall ApplySmoke(tPipe_chunk **pChunk@<EAX>)
void ApplySmoke(tPipe_chunk** pChunk) {
    br_vector3 pos;
    LOG_TRACE("(%p)", pChunk);

    GetReducedPos(&pos, &(*pChunk)->chunk_data.smoke_data.pos);
    AdjustSmoke((*pChunk)->subject_index,
        (*pChunk)->chunk_data.smoke_data.type,
        &pos,
        (*pChunk)->chunk_data.smoke_data.radius / 1024.f,
        (*pChunk)->chunk_data.smoke_data.strength / 256.f);
    AdvanceChunkPtr(pChunk, ePipe_chunk_smoke);
}

// IDA: void __usercall ApplySmokeColumn(tPipe_chunk **pChunk@<EAX>)
void ApplySmokeColumn(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustSmokeColumn((*pChunk)->subject_index & 0x3fff,
        ((((*pChunk)->chunk_data.smoke_column_data.car_ID) >> 8) == 0) ? &gProgram_state.current_car : GetCarSpec((*pChunk)->chunk_data.smoke_column_data.car_ID >> 8, (*pChunk)->chunk_data.smoke_column_data.car_ID & 0xff),
        (*pChunk)->chunk_data.smoke_column_data.vertex,
        (*pChunk)->subject_index >> 14);
    AdvanceChunkPtr(pChunk, ePipe_chunk_smoke_column);
}

// IDA: void __usercall ApplyFlame(tPipe_chunk **pChunk@<EAX>)
void ApplyFlame(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustFlame((*pChunk)->subject_index,
        (*pChunk)->chunk_data.flame_data.frame_count,
        (*pChunk)->chunk_data.flame_data.scale_x,
        (*pChunk)->chunk_data.flame_data.scale_y,
        (*pChunk)->chunk_data.flame_data.offset_x,
#if DETHRACE_FIX_BUGS
        (*pChunk)->chunk_data.flame_data.offset_z);
#else
        (*pChunk)->chunk_data.flame_data.offset_x);
#endif
    AdvanceChunkPtr(pChunk, ePipe_chunk_flame);
}

// IDA: void __usercall ApplySplash(tPipe_chunk **pChunk@<EAX>)
void ApplySplash(tPipe_chunk** pChunk) {
    tCar_spec* c;
    LOG_TRACE("(%p)", pChunk);

    if (((*pChunk)->subject_index & 0xff00) == 0) {
        c = &gProgram_state.current_car;
    } else {
        c = GetCarSpec((*pChunk)->subject_index >> 8, (*pChunk)->subject_index & 0xff);
    }
    c->water_d = (*pChunk)->chunk_data.splash_data.d;
    BrVector3Copy(&c->water_normal, &(*pChunk)->chunk_data.splash_data.normal);
    AdvanceChunkPtr(pChunk, ePipe_chunk_splash);
}

// IDA: void __usercall ApplyOilSpill(tPipe_chunk **pChunk@<EAX>, tU32 pStop_time@<EDX>)
void ApplyOilSpill(tPipe_chunk** pChunk, tU32 pStop_time) {
    LOG_TRACE("(%p, %d)", pChunk, pStop_time);

    AdjustOilSpill((*pChunk)->subject_index,
        &(*pChunk)->chunk_data.oil_data.mat,
        (*pChunk)->chunk_data.oil_data.full_size,
        (*pChunk)->chunk_data.oil_data.grow_rate,
        (*pChunk)->chunk_data.oil_data.spill_time,
        pStop_time,
        (*pChunk)->chunk_data.oil_data.car,
        &(*pChunk)->chunk_data.oil_data.original_pos,
        (*pChunk)->chunk_data.oil_data.pixelmap);
    AdvanceChunkPtr(pChunk, ePipe_chunk_oil_spill);
}

// IDA: void __usercall ApplyFrameBoundary(tPipe_chunk **pChunk@<EAX>)
void ApplyFrameBoundary(tPipe_chunk** pChunk) {
    tU32 result;
    LOG_TRACE("(%p)", pChunk);

    gLast_replay_frame_time = (*pChunk)->chunk_data.frame_boundary_data.time;
    // DAT_0054b244 = PDGetTotalTime();
    AdvanceChunkPtr(pChunk, ePipe_chunk_frame_boundary);
}

// IDA: void __usercall ApplySound(tPipe_chunk **pChunk@<EAX>)
void ApplySound(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    if (!gDisable_sound) {
        if ((*pChunk)->chunk_data.sound_data.volume == 0) {
            DRS3StartSound2(GetOutletFromIndex((*pChunk)->chunk_data.sound_data.outlet_index),
                (*pChunk)->subject_index,
                1,
                -1,
                -1,
                65535.f * GetReplayRate(),
                0x10000);
        } else if (BrVector3LengthSquared(&(*pChunk)->chunk_data.sound_data.position) == 0) {
            DRS3StartSound2(GetOutletFromIndex((*pChunk)->chunk_data.sound_data.outlet_index),
                (*pChunk)->subject_index,
                1,
                (*pChunk)->chunk_data.sound_data.volume & 0xff,
                (*pChunk)->chunk_data.sound_data.volume >> 8,
                (float)(*pChunk)->chunk_data.sound_data.pitch * fabsf(GetReplayRate()),
                0x10000);
        } else {
            DRS3StartSound3D(GetOutletFromIndex((*pChunk)->chunk_data.sound_data.outlet_index),
                (*pChunk)->subject_index,
                &(*pChunk)->chunk_data.sound_data.position,
                &gZero_vector,
                1,
                (*pChunk)->chunk_data.sound_data.volume,
                (float)(*pChunk)->chunk_data.sound_data.pitch * fabsf(GetReplayRate()),
                0x10000);
        }
    }
    AdvanceChunkPtr(pChunk, ePipe_chunk_sound);
}

// IDA: void __usercall ApplyCar(tPipe_chunk **pChunk@<EAX>)
void ApplyCar(tPipe_chunk** pChunk) {
    tCar_spec* car;
    br_vector3 com_offset_c;
    br_vector3 com_offset_w;
    LOG_TRACE("(%p)", pChunk);

    if (((*pChunk)->subject_index & 0xff00) == 0) {
        car = &gProgram_state.current_car;
    } else {
        car = GetCarSpec((*pChunk)->subject_index >> 8, (*pChunk)->subject_index & 0x00ff);
    }
    BrMatrix34Copy(&car->car_master_actor->t.t.mat, &(*pChunk)->chunk_data.car_data.transformation);
    BrVector3Copy(&car->v, &(*pChunk)->chunk_data.car_data.velocity);
    BrMatrix34TApplyV(&car->velocity_car_space, &car->v, &car->car_master_actor->t.t.mat);
    BrVector3InvScale(&car->velocity_car_space, &car->velocity_car_space, WORLD_SCALE);
    if (BrVector3LengthSquared(&car->velocity_car_space) >= .0001f) {
        BrVector3Normalise(&car->direction, &car->v);
    } else {
        BrVector3Negate(&car->direction, (br_vector3*)car->car_master_actor->t.t.mat.m[2]);
    }
    BrVector3Copy(&car->pos, &car->car_master_actor->t.t.translate.t);
    BrVector3InvScale(&com_offset_c, &car->cmpos, WORLD_SCALE);
    BrMatrix34ApplyV(&com_offset_w, &com_offset_c, &car->car_master_actor->t.t.mat);
    BrVector3Accumulate(&car->pos, &com_offset_w);
    car->speedo_speed = .07f * (*pChunk)->chunk_data.car_data.speedo_speed / 32767.f;
    car->lf_sus_position = 0.15f * (*pChunk)->chunk_data.car_data.lf_sus_position / 127.f;
    car->rf_sus_position = 0.15f * (*pChunk)->chunk_data.car_data.rf_sus_position / 127.f;
    car->lr_sus_position = 0.15f * (*pChunk)->chunk_data.car_data.lr_sus_position / 127.f;
    car->rr_sus_position = 0.15f * (*pChunk)->chunk_data.car_data.rr_sus_position / 127.f;
    car->steering_angle = 60.f * (*pChunk)->chunk_data.car_data.steering_angle / 32767.f;
    car->revs = 10 * ((*pChunk)->chunk_data.car_data.revs_and_gear & 0x7ff);
    car->gear = ((*pChunk)->chunk_data.car_data.revs_and_gear >> 12) - 1;
    car->frame_collision_flag = ((*pChunk)->chunk_data.car_data.revs_and_gear >> 11) & 0x1;
    AdvanceChunkPtr(pChunk, ePipe_chunk_car);
}

// IDA: void __usercall ApplyDamage(tPipe_chunk **pChunk@<EAX>)
void ApplyDamage(tPipe_chunk** pChunk) {
    tCar_spec* car;
    int i;
    LOG_TRACE("(%p)", pChunk);

    if (((*pChunk)->subject_index & 0xff00) == 0) {
        car = &gProgram_state.current_car;
    } else {
        car = GetCarSpec((*pChunk)->subject_index >> 8, (*pChunk)->subject_index & 0x00ff);
    }
    for (i = 0; i < COUNT_OF(car->damage_units); i++) {
        car->damage_units[i].damage_level += (*pChunk)->chunk_data.damage_data.damage_delta[i];
    }
    AdvanceChunkPtr(pChunk, ePipe_chunk_damage);
}

// IDA: void __usercall ApplySpecial(tPipe_chunk **pChunk@<EAX>)
void ApplySpecial(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    switch ((*pChunk)->subject_index) {
    case 0:
        if (fabsf(GetReplayRate()) <= 1.f) {
            FadePaletteDown();
        }
        break;
    case 1:
        gPed_scale_factor = 2.0f;
        break;
    case 2:
        gPed_scale_factor = 1.0f;
        break;
    case 3:
        gPed_scale_factor = 0.5f;
        break;
    case 4:
        gPed_scale_factor = 1.0f;
        break;
    }
    AdvanceChunkPtr(pChunk, ePipe_chunk_special);
}

// IDA: void __usercall ApplyPedGib(tPipe_chunk **pChunk@<EAX>)
void ApplyPedGib(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustPedGib((*pChunk)->subject_index,
        (*pChunk)->chunk_data.ped_gib_data.size,
        (*pChunk)->chunk_data.ped_gib_data.gib_index,
        (*pChunk)->chunk_data.ped_gib_data.ped_parent_index,
        &(*pChunk)->chunk_data.ped_gib_data.transform);
    AdvanceChunkPtr(pChunk, ePipe_chunk_ped_gib);
}

// IDA: void __usercall ApplyProxRay(tPipe_chunk **pChunk@<EAX>)
void ApplyProxRay(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustProxRay(
        (*pChunk)->subject_index,
        (*pChunk)->chunk_data.prox_ray_data.car_ID,
        (*pChunk)->chunk_data.prox_ray_data.ped_index,
        (*pChunk)->chunk_data.prox_ray_data.time);
    AdvanceChunkPtr(pChunk, ePipe_chunk_prox_ray);
}

// IDA: void __usercall ApplySkidAdjustment(tPipe_chunk **pChunk@<EAX>)
void ApplySkidAdjustment(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdjustSkid((*pChunk)->subject_index,
        &(*pChunk)->chunk_data.skid_adjustment.matrix,
        (*pChunk)->chunk_data.skid_adjustment.material_index);
    AdvanceChunkPtr(pChunk, ePipe_chunk_skid_adjustment);
}

// IDA: int __usercall ApplyPipedSession@<EAX>(tU8 **pPtr@<EAX>)
int ApplyPipedSession(tU8** pPtr) {
    int i;
    int return_value;
    tPipe_chunk* chunk_ptr;
    tPipe_chunk_type chunk_type;
    LOG_TRACE("(%p)", pPtr);

    if (*pPtr == gPipe_record_ptr) {
        return 1;
    }
    gEnd_of_session = *pPtr + (LengthOfSession((tPipe_session *)*pPtr) - sizeof(tU16));
    REPLAY_DEBUG_ASSERT(((tPipe_session *)*pPtr)->magic1 == REPLAY_DEBUG_MAGIC1);
    chunk_ptr = (tPipe_chunk *)(*pPtr + offsetof(tPipe_session, chunks));
    return_value = 0;
    chunk_type = ((tPipe_session *)*pPtr)->chunk_type;
    for (i = 0; i < ((tPipe_session *)*pPtr)->number_of_chunks; i++) {
        switch (chunk_type) {
        case ePipe_chunk_model_geometry:
            ApplyModelGeometry(&chunk_ptr);
            break;
        case ePipe_chunk_pedestrian:
            ApplyPedestrian(&chunk_ptr);
            break;
        case ePipe_chunk_frame_boundary:
            ApplyFrameBoundary(&chunk_ptr);
            return_value = 1;
            break;
        case ePipe_chunk_car:
            ApplyCar(&chunk_ptr);
            break;
        case ePipe_chunk_sound:
            ApplySound(&chunk_ptr);
            break;
        case ePipe_chunk_damage:
            ApplyDamage(&chunk_ptr);
            break;
        case ePipe_chunk_special:
            ApplySpecial(&chunk_ptr);
            break;
        case ePipe_chunk_ped_gib:
            ApplyPedGib(&chunk_ptr);
            break;
        case ePipe_chunk_incident:
            AdvanceChunkPtr(&chunk_ptr, ePipe_chunk_incident);
            break;
        case ePipe_chunk_spark:
            ApplySpark(&chunk_ptr);
            break;
        case ePipe_chunk_shrapnel:
            ApplyShrapnel(&chunk_ptr);
            break;
        case ePipe_chunk_screen_shake:
            ApplyScreenWobble(&chunk_ptr);
            break;
        case ePipe_chunk_groove_stop:
            ApplyGrooveStop(&chunk_ptr);
            break;
        case ePipe_chunk_non_car:
            ApplyNonCar(&chunk_ptr);
            break;
        case ePipe_chunk_smoke:
            ApplySmoke(&chunk_ptr);
            break;
        case ePipe_chunk_oil_spill:
            ApplyOilSpill(&chunk_ptr, 0);
            break;
        case ePipe_chunk_smoke_column:
            ApplySmokeColumn(&chunk_ptr);
            break;
        case ePipe_chunk_flame:
            ApplyFlame(&chunk_ptr);
            break;
        case ePipe_chunk_smudge:
            ApplySmudge(&chunk_ptr);
            break;
        case ePipe_chunk_splash:
            ApplySplash(&chunk_ptr);
            break;
        case ePipe_chunk_prox_ray:
            ApplyProxRay(&chunk_ptr);
            break;
        case ePipe_chunk_skid_adjustment:
            ApplySkidAdjustment(&chunk_ptr);
            break;
        default:
            break;
        }
    }
#if defined(DETHRACE_FIX_BUGS)
    *pPtr += PIPE_ALIGN(LengthOfSession((tPipe_session *)*pPtr));
#else
    *pPtr += LengthOfSession((tPipe_session *)*pPtr);
#endif
    if (*pPtr >= gPipe_buffer_working_end && *pPtr != gPipe_record_ptr) {
        *pPtr = gPipe_buffer_start;
    }
    return return_value;
}

// IDA: int __usercall MoveSessionPointerBackOne@<EAX>(tU8 **pPtr@<EAX>)
int MoveSessionPointerBackOne(tU8** pPtr) {
    LOG_TRACE("(%p)", pPtr);

    if (*pPtr == gPipe_buffer_oldest && *pPtr != gPipe_record_ptr) {
        return 1;
    }
    if (*pPtr == gPipe_buffer_start) {
        *pPtr = gPipe_buffer_working_end;
    }
    *pPtr -= sizeof(tU16);
    *pPtr -= *(tU16*)*pPtr;
    REPLAY_DEBUG_ASSERT(((tPipe_session*)*pPtr)->magic1 == REPLAY_DEBUG_MAGIC1);
    return 0;
}

// IDA: int __usercall MoveSessionPointerForwardOne@<EAX>(tU8 **pPtr@<EAX>)
int MoveSessionPointerForwardOne(tU8** pPtr) {
    LOG_TRACE("(%p)", pPtr);

    REPLAY_DEBUG_ASSERT(((tPipe_session*)*pPtr)->magic1 == REPLAY_DEBUG_MAGIC1);
#if defined(DETHRACE_FIX_BUGS)
    *pPtr += PIPE_ALIGN(LengthOfSession((tPipe_session*)*pPtr));
#else
    *pPtr += LengthOfSession((tPipe_session*)*pPtr);
#endif
    if (*pPtr >= gPipe_buffer_working_end && *pPtr != gPipe_record_ptr) {
        *pPtr = gPipe_buffer_start;
    }
    return *pPtr == gPipe_record_ptr;
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

    ptr = pPtr;
    chunk_counter = 0;
    masked_index = pIndex & 0x0fff;
    while (1) {
        if (!MoveSessionPointerBackOne(&ptr)) {
            reached_end = chunk_counter >= gMax_rewind_chunks;
            chunk_counter++;
        } else {
            reached_end = 1;
        }
        if (!reached_end) {
            gEnd_of_session = ptr + LengthOfSession((tPipe_session*)ptr) - sizeof(tU16);
            mr_chunky = &((tPipe_session*)ptr)->chunks;
            for (i = 0; i < ((tPipe_session*)ptr)->number_of_chunks && ((tPipe_session*)ptr)->chunk_type == pType; i++) {
                if ((mr_chunky->subject_index & 0xfff) == masked_index) {
                    return mr_chunky;
                }
                AdvanceChunkPtr(&mr_chunky, pType);
            }
        }
        if (reached_end) {
            return NULL;
        }
    }
}

// IDA: void __usercall UndoModelGeometry(tPipe_chunk **pChunk@<EAX>)
void UndoModelGeometry(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    LOG_TRACE("(%p)", pChunk);

    if (((*pChunk)->subject_index & 0xff00) == 0) {
        car = &gProgram_state.current_car;
    } else {
        car = GetCarSpec((*pChunk)->subject_index >> 8, (*pChunk)->subject_index & 0x00ff);
    }
    model_ptr = car->car_model_actors[(*pChunk)->chunk_data.model_geometry_data.model_index].actor->model;
    for (i = 0; i < (*pChunk)->chunk_data.model_geometry_data.vertex_count; i++) {
        BrVector3Sub(&model_ptr->vertices[(*pChunk)->chunk_data.model_geometry_data.vertex_changes[i].vertex_index].p,
            &model_ptr->vertices[(*pChunk)->chunk_data.model_geometry_data.vertex_changes[i].vertex_index].p,
            &(*pChunk)->chunk_data.model_geometry_data.vertex_changes[i].delta_coordinates);
    }
    SetModelForUpdate(model_ptr, car, 0);
    AdvanceChunkPtr(pChunk, ePipe_chunk_model_geometry);
}

// IDA: void __usercall UndoSmudge(tPipe_chunk **pChunk@<EAX>)
void UndoSmudge(tPipe_chunk** pChunk) {
    int i;
    br_model* model_ptr;
    tCar_spec* car;
    LOG_TRACE("(%p)", pChunk);

    DoSmudge(pChunk, -1);
    AdvanceChunkPtr(pChunk, ePipe_chunk_smudge);
}

// IDA: void __usercall UndoPedestrian(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoPedestrian(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    if (pPrev_chunk == NULL) {
        ApplyPedestrian(pChunk);
    }
    else {
        gDisable_advance = 1;
        ApplyPedestrian(&temp_prev_chunk);
        gDisable_advance = 0;
        AdvanceChunkPtr(pChunk, ePipe_chunk_pedestrian);
    }
}

// IDA: void __usercall UndoFrameBoundary(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoFrameBoundary(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    ApplyFrameBoundary(pChunk);
    AdvanceChunkPtr(pChunk, ePipe_chunk_frame_boundary);
}

// IDA: void __usercall UndoCar(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoCar(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    if (pPrev_chunk == NULL) {
        ApplyCar(pChunk);
    }
    else {
        gDisable_advance = 1;
        ApplyCar(&temp_prev_chunk);
        gDisable_advance = 0;
        AdvanceChunkPtr(pChunk, ePipe_chunk_car);
    }
}

// IDA: void __usercall UndoSound(tPipe_chunk **pChunk@<EAX>)
void UndoSound(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    AdvanceChunkPtr(pChunk,ePipe_chunk_sound);
}

// IDA: void __usercall UndoDamage(tPipe_chunk **pChunk@<EAX>)
void UndoDamage(tPipe_chunk** pChunk) {
    tCar_spec* car;
    int i;
    LOG_TRACE("(%p)", pChunk);

    if (((*pChunk)->subject_index & 0xff00) == 0) {
        car = &gProgram_state.current_car;
    }
    else {
        car = GetCarSpec((*pChunk)->subject_index >> 8, (*pChunk)->subject_index & 0xff);
    }
    for (i = 0; i < COUNT_OF(car->damage_units); i++) {
        car->damage_units[i].damage_level -= (*pChunk)->chunk_data.damage_data.damage_delta[i];
    }
    AdvanceChunkPtr(pChunk, ePipe_chunk_damage);
}

// IDA: void __usercall UndoSpecial(tPipe_chunk **pChunk@<EAX>)
void UndoSpecial(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    if ((*pChunk)->subject_index == 0) {
        ApplySpecial(pChunk);
    } else {
        switch ((*pChunk)->subject_index) {
        case 1:
            gPed_scale_factor = 1.0f;
            break;
        case 2:
            gPed_scale_factor = 2.0f;
            break;
        case 3:
            gPed_scale_factor = 1.0f;
            break;
        case 4:
            gPed_scale_factor = 0.5f;
            break;
        }
        AdvanceChunkPtr(pChunk, ePipe_chunk_special);
    }
}

// IDA: void __usercall UndoPedGib(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoPedGib(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk != NULL) {
        ApplyPedGib(&temp_prev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_ped_gib);
}

// IDA: void __usercall UndoSpark(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSpark(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk != NULL) {
        ApplySpark(&temp_prev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_spark);
}

// IDA: void __usercall UndoShrapnel(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoShrapnel(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk != NULL) {
        ApplyShrapnel(&temp_prev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_shrapnel);
}

// IDA: void __usercall UndoScreenWobble(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoScreenWobble(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk == NULL) {
        SetScreenWobble(0, 0);
    }
    else {
        ApplyScreenWobble(&temp_prev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_screen_shake);
}

// IDA: void __usercall UndoGrooveStop(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoGrooveStop(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk != NULL) {
        ApplyGrooveStop(&temp_prev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_groove_stop);
}

// IDA: void __usercall UndoNonCar(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoNonCar(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk != NULL) {
        ApplyNonCar(&temp_prev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_non_car);
}

// IDA: void __usercall UndoSmoke(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSmoke(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk != NULL) {
        ApplySmoke(&temp_prev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_smoke);
}

// IDA: void __usercall UndoSmokeColumn(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSmokeColumn(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    ApplySmokeColumn(pChunk);
}

// IDA: void __usercall UndoFlame(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoFlame(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    ApplyFlame(pChunk);
}

// IDA: void __usercall UndoSplash(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSplash(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk == NULL) {
        ((((*pChunk)->subject_index & 0xff00) == 0) ? &gProgram_state.current_car : GetCarSpec((*pChunk)->subject_index >> 8, (*pChunk)->subject_index & 0xff))->water_d = 10000.f;
    }
    else {
        ApplySplash(&temp_prev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_splash);
}

// IDA: void __usercall UndoOilSpill(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoOilSpill(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    tPipe_chunk* temp_prev_chunk;
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    temp_prev_chunk = pPrev_chunk;
    gDisable_advance = 1;
    if (pPrev_chunk != NULL) {
        ApplyOilSpill(&temp_prev_chunk, (*pChunk)->chunk_data.oil_data.previous_stop_time);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_oil_spill);
}

// IDA: void __usercall UndoProxRay(tPipe_chunk **pChunk@<EAX>)
void UndoProxRay(tPipe_chunk** pChunk) {
    LOG_TRACE("(%p)", pChunk);

    ApplyProxRay(pChunk);
}

// IDA: void __usercall UndoSkidAdjustment(tPipe_chunk **pChunk@<EAX>, tPipe_chunk *pPrev_chunk@<EDX>)
void UndoSkidAdjustment(tPipe_chunk** pChunk, tPipe_chunk* pPrev_chunk) {
    LOG_TRACE("(%p, %p)", pChunk, pPrev_chunk);

    gDisable_advance = 1;
    if (pPrev_chunk == NULL) {
        HideSkid((*pChunk)->subject_index);
    }
    else {
        ApplySkidAdjustment(&pPrev_chunk);
    }
    gDisable_advance = 0;
    AdvanceChunkPtr(pChunk, ePipe_chunk_skid_adjustment);
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

    if (MoveSessionPointerBackOne(pPtr)) {
        return 1;
    }
    REPLAY_DEBUG_ASSERT(((tPipe_session*)*pPtr)->magic1 == REPLAY_DEBUG_MAGIC1);
    gEnd_of_session = *pPtr + LengthOfSession((tPipe_session*)*pPtr) - sizeof(tU16);
    chunk_ptr = &((tPipe_session*)*pPtr)->chunks;
    chunk_type = ((tPipe_session*)*pPtr)->chunk_type;
    pushed_end_of_session = gEnd_of_session;
    for (i = 0; i < ((tPipe_session*)pPtr)->number_of_chunks; i++) {
        if (!(chunk_type == ePipe_chunk_model_geometry || chunk_type == ePipe_chunk_sound || chunk_type == ePipe_chunk_damage || chunk_type == ePipe_chunk_special || chunk_type == ePipe_chunk_incident || chunk_type == ePipe_chunk_prox_ray || chunk_type == ePipe_chunk_smudge)) {
            prev_chunk = FindPreviousChunk(*pPtr, ((tPipe_session*)*pPtr)->chunk_type, chunk_ptr->subject_index);
        }
        gEnd_of_session = pushed_end_of_session;
        switch (chunk_type) {
        case ePipe_chunk_model_geometry:
            UndoModelGeometry(&chunk_ptr);
            break;
        case ePipe_chunk_pedestrian:
            UndoPedestrian(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_frame_boundary:
            UndoFrameBoundary(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_car:
            UndoCar(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_sound:
            UndoSound(&chunk_ptr);
            break;
        case ePipe_chunk_damage:
            UndoDamage(&chunk_ptr);
            break;
        case ePipe_chunk_special:
            UndoSpecial(&chunk_ptr);
            break;
        case ePipe_chunk_ped_gib:
            UndoPedGib(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_incident:
            AdvanceChunkPtr(&chunk_ptr, ePipe_chunk_incident);
            break;
        case ePipe_chunk_spark:
            UndoSpark(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_shrapnel:
            UndoShrapnel(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_screen_shake:
            UndoScreenWobble(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_groove_stop:
            UndoGrooveStop(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_non_car:
            UndoNonCar(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_smoke:
            UndoSmoke(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_oil_spill:
            UndoOilSpill(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_smoke_column:
            UndoSmokeColumn(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_flame:
            UndoFlame(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_smudge:
            UndoSmudge(&chunk_ptr);
            break;
        case ePipe_chunk_splash:
            UndoSplash(&chunk_ptr, prev_chunk);
            break;
        case ePipe_chunk_prox_ray:
            UndoProxRay(&chunk_ptr);
            break;
        case ePipe_chunk_skid_adjustment:
            UndoSkidAdjustment(&chunk_ptr, prev_chunk);
            break;
        default:
            break;
        }
    }
    temp_ptr = *pPtr;
    if (MoveSessionPointerBackOne(&temp_ptr)) {
        return 1;
    }
    return ((tPipe_session*)temp_ptr)->chunk_type == ePipe_chunk_frame_boundary;
}

// IDA: tU32 __usercall FindPrevFrameTime@<EAX>(tU8 *pPtr@<EAX>)
tU32 FindPrevFrameTime(tU8* pPtr) {
    tU8* temp_ptr;
    LOG_TRACE("(%p)", pPtr);

    temp_ptr = pPtr;
    do {
        if (MoveSessionPointerBackOne(&temp_ptr)) {
          return 0;
        }
    } while (((tPipe_session*)temp_ptr)->chunk_type != ePipe_chunk_frame_boundary);
    return ((tPipe_session*)temp_ptr)->chunks.chunk_data.frame_boundary_data.time;
}

// IDA: void __usercall ScanBuffer(tU8 **pPtr@<EAX>, tPipe_chunk_type pType@<EDX>, tU32 pDefault_time@<EBX>, int (*pCall_back)(tPipe_chunk*, int, tU32)@<ECX>, int (*pTime_check)(tU32))
void ScanBuffer(tU8** pPtr, tPipe_chunk_type pType, tU32 pDefault_time, int (*pCall_back)(tPipe_chunk*, int, tU32), int (*pTime_check)(tU32)) {
    tPipe_chunk* chunk_ptr;
    tU32 the_time;
    LOG_TRACE("(%p, %d, %d, %p, %p)", pPtr, pType, pDefault_time, pCall_back, pTime_check);

    the_time = pDefault_time;
    while (1) {
        if (PipeSearchForwards() ? MoveSessionPointerForwardOne(pPtr) : MoveSessionPointerBackOne(pPtr)) {
            return;
        }
        gEnd_of_session = *pPtr + LengthOfSession((tPipe_session*)*pPtr) - sizeof(tU16);
        if (((tPipe_session*)*pPtr)->chunk_type == ePipe_chunk_frame_boundary) {
            the_time = ((tPipe_session*)*pPtr)->chunks.chunk_data.frame_boundary_data.time;
        } else if (((tPipe_session*)*pPtr)->chunk_type == pType) {
            if (pCall_back(&((tPipe_session*)*pPtr)->chunks, ((tPipe_session*)*pPtr)->number_of_chunks, the_time)) {
                return;
            }
        }
        if (pTime_check != NULL) {
            if (!pTime_check(the_time)) {
                return;
            }
        }
    }
}

// IDA: int __usercall CheckSound@<EAX>(tPipe_chunk *pChunk_ptr@<EAX>, int pChunk_count@<EDX>, tU32 pTime@<EBX>)
int CheckSound(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime) {
    int i;
    int sound_length;
    tPipe_chunk* temp_ptr;
    LOG_TRACE("(%p, %d, %d)", pChunk_ptr, pChunk_count, pTime);

    STUB_ONCE();
    return 1;
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

    temp_ptr = pChunk_ptr;
    if (PipeSearchForwards()) {
        if (pTime <= gOldest_time) {
            return 0;
        }
    } else {
        if (pTime >= gOldest_time) {
            return 0;
        }
    }
    for (i = 0; i < pChunk_count; i++) {
        if ((temp_ptr->subject_index & 0xff00) == 0) {
            car = &gProgram_state.current_car;
        } else {
            car = GetCarSpec(temp_ptr->subject_index >> 8, temp_ptr->subject_index & 0xff);
        }
        if (car == gCar_ptr) {
            BrVector3Copy(&gCar_pos, (br_vector3*)temp_ptr->chunk_data.car_data.transformation.m[3]);
            BrVector3InvScale(&com_offset_c, &car->cmpos, WORLD_SCALE);
            BrMatrix34ApplyV(&com_offset_w, &com_offset_c, &temp_ptr->chunk_data.car_data.transformation);
            BrVector3Accumulate(&gCar_pos, &com_offset_w);
            BrVector3Sub(&difference, &gCar_pos, &gReference_pos);
            if (BrVector3LengthSquared(&difference) <= gMax_distance) {
                gTrigger_time = pTime;
                return 0;
            } else {
                gTrigger_time = pTime;
                return 1;
            }
        }
        AdvanceChunkPtr(&temp_ptr, ePipe_chunk_car);
    }
    return 0;
}

// IDA: int __usercall CarTimeout@<EAX>(tU32 pTime@<EAX>)
int CarTimeout(tU32 pTime) {
    LOG_TRACE("(%d)", pTime);

    if (PipeSearchForwards()) {
        if (pTime > gYoungest_time) {
            return 0;
        }
    } else {
        if (pTime < gYoungest_time) {
            return 0;
        }
    }
    return 1;
}

// IDA: void __usercall ScanCarsPositions(tCar_spec *pCar@<EAX>, br_vector3 *pSource_pos@<EDX>, br_scalar pMax_distance_sqr, tU32 pOffset_time, tU32 pTime_period, br_vector3 *pCar_pos, tU32 *pTime_returned)
void ScanCarsPositions(tCar_spec* pCar, br_vector3* pSource_pos, br_scalar pMax_distance_sqr, tU32 pOffset_time, tU32 pTime_period, br_vector3* pCar_pos, tU32* pTime_returned) {
    tU8* temp_ptr;
    LOG_TRACE("(%p, %p, %f, %d, %d, %p, %p)", pCar, pSource_pos, pMax_distance_sqr, pOffset_time, pTime_period, pCar_pos, pTime_returned);

    temp_ptr = gPipe_play_ptr;
    gTrigger_time = 0;
    gMax_distance = pMax_distance_sqr;
    BrVector3Copy(&gReference_pos, pSource_pos);
    gCar_ptr = pCar;

    if (PipeSearchForwards()) {
        gOldest_time = GetTotalTime() + pOffset_time;
        gYoungest_time = gOldest_time + pTime_period;
    } else {
        gOldest_time = GetTotalTime() - pOffset_time;
        gYoungest_time = gOldest_time - pTime_period;
    }

    ScanBuffer(&temp_ptr, ePipe_chunk_car, GetTotalTime(), CheckCar, CarTimeout);
    BrVector3Copy(pCar_pos, &gCar_pos);
    if (pCar_pos->v[0] > 500.f) {
        Vector3AddFloats(pCar_pos, pCar_pos, -1000.f, -1000.f, -1000.f);
    }
    *pTime_returned = gTrigger_time;
}

// IDA: int __usercall CheckIncident@<EAX>(tPipe_chunk *pChunk_ptr@<EAX>, int pChunk_count@<EDX>, tU32 pTime@<EBX>)
int CheckIncident(tPipe_chunk* pChunk_ptr, int pChunk_count, tU32 pTime) {
    LOG_TRACE("(%p, %d, %d)", pChunk_ptr, pChunk_count, pTime);

    if (PipeSearchForwards()) {
        if (pTime <= gOldest_time) {
            return 0;
        }
    } else {
        if (gOldest_time <= pTime) {
            return 0;
        }
    }
    gIncidentChunk = pChunk_ptr;
    gTrigger_time = pTime;
    return 1;
}

// IDA: int __usercall GetNextIncident@<EAX>(tU32 pOffset_time@<EAX>, tIncident_type *pIncident_type@<EDX>, float *pSeverity@<EBX>, tIncident_info *pInfo@<ECX>, tU32 *pTime_away)
int GetNextIncident(tU32 pOffset_time, tIncident_type* pIncident_type, float* pSeverity, tIncident_info* pInfo, tU32* pTime_away) {
    tU8* temp_ptr;
    LOG_TRACE("(%d, %p, %p, %p, %p)", pOffset_time, pIncident_type, pSeverity, pInfo, pTime_away);

    temp_ptr = gPipe_play_ptr;
    gTrigger_time = 0;
    if (PipeSearchForwards()) {
        gOldest_time = GetTotalTime() + pOffset_time;
    } else {
        gOldest_time = GetTotalTime() - pOffset_time;
    }
    ScanBuffer(&temp_ptr, ePipe_chunk_incident, GetTotalTime(), CheckIncident, NULL);
    if (gTrigger_time != 0) {
        *pTime_away = gTrigger_time - GetTotalTime();
        *pIncident_type = gIncidentChunk->subject_index;
        *pSeverity = gIncidentChunk->chunk_data.incident_data.severity;
        if (*pIncident_type == eIncident_ped) {
            pInfo->ped_info.ped_actor = GetPedestrianActor(gIncidentChunk->chunk_data.incident_data.info.ped_info.ped_index);
            pInfo->ped_info.murderer_actor = gIncidentChunk->chunk_data.incident_data.info.ped_info.actor;
        } else if (*pIncident_type == eIncident_car) {
            if ((gIncidentChunk->chunk_data.incident_data.info.car_info.car_ID & 0xff00) == 0) {
                pInfo->car_info.car = &gProgram_state.current_car;
            } else {
                pInfo->car_info.car = GetCarSpec(gIncidentChunk->chunk_data.incident_data.info.car_info.car_ID >> 8,
                    gIncidentChunk->chunk_data.incident_data.info.car_info.car_ID & 0xff);
            }
            BrVector3Copy(&pInfo->car_info.impact_point, &gIncidentChunk->chunk_data.incident_data.info.car_info.impact_point);
        } else if (*pIncident_type == eIncident_wall) {
            BrVector3Copy(&pInfo->wall_info.pos, &gIncidentChunk->chunk_data.incident_data.info.wall_info.pos);
        }
    }
    return gTrigger_time;
}

// IDA: tU32 __cdecl GetARStartTime()
tU32 GetARStartTime() {
    tU8* temp_ptr;
    LOG_TRACE("()");

    temp_ptr = gPipe_buffer_oldest;
    do {
      if (MoveSessionPointerForwardOne(&temp_ptr)) {
          return 0;
      }
    } while (((tPipe_session*)temp_ptr)->chunk_type != ePipe_chunk_frame_boundary);
    return ((tPipe_session*)temp_ptr)->chunks.chunk_data.frame_boundary_data.time;
}
