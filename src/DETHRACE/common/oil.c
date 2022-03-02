#include "oil.h"
#include "brender/brender.h"
#include "globvars.h"
#include "harness/trace.h"
#include "loading.h"
#include "utility.h"
#include <math.h>
#include <stdlib.h>

char* gOil_pixie_names[1] = { "OIL.PIX" };
int gNext_oil_pixie;
br_scalar gZ_buffer_diff;
br_scalar gMin_z_diff;
br_pixelmap* gOil_pixies[1];
tOil_spill_info gOily_spills[15];

// IDA: void __cdecl InitOilSpills()
void InitOilSpills() {
    int i;
    br_model* the_model;
    br_material* the_material;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gOil_pixie_names); i++) {
        gOil_pixies[i] = LoadPixelmap(gOil_pixie_names[i]);
        BrMapAdd(gOil_pixies[i]);
    }

    for (i = 0; i < COUNT_OF(gOily_spills); i++) {
        the_material = BrMaterialAllocate(NULL);
        BrMaterialAdd(the_material);
        the_material->ka = 0.99f;
        the_material->kd = 0.0f;
        the_material->ks = 0.0f;
        the_material->power = 0.0;
        the_material->index_base = 0;
        the_material->flags |= BR_MATF_LIGHT;
        the_material->flags |= BR_MATF_PERSPECTIVE;
        the_material->flags |= 0x00000004;
        the_material->index_range = 0;
        the_material->colour_map = NULL;
        BrMatrix23Identity(&the_material->map_transform);
        the_material->index_shade = BrTableFind("IDENTITY.TAB");
        BrMaterialUpdate(the_material, BR_MATU_ALL);
        the_model = BrModelAllocate(NULL, 4, 2);
        the_model->flags |= BR_MODF_KEEP_ORIGINAL;

        the_model->faces->vertices[0] = 2;
        the_model->faces->vertices[1] = 1;
        the_model->faces->vertices[2] = 0;
        the_model->faces->material = NULL;
        the_model->faces->smoothing = 1;
        the_model->faces[1].vertices[0] = 3;
        the_model->faces[1].vertices[1] = 2;
        the_model->faces[1].vertices[2] = 0;
        the_model->faces[1].material = NULL;
        the_model->faces[1].smoothing = 1;
        the_model->vertices[0].p.v[0] = -1.0f;
        the_model->vertices[0].p.v[1] = 0.0f;
        the_model->vertices[0].p.v[2] = -1.0f;
        the_model->vertices->map.v[0] = 0.0f;
        the_model->vertices->map.v[1] = 1.0f;
        the_model->vertices[1].p.v[0] = 1.0f;
        the_model->vertices[1].p.v[1] = 0.0f;
        the_model->vertices[1].p.v[2] = 1.0f;
        the_model->vertices[1].map.v[0] = 0.0f;
        the_model->vertices[1].map.v[1] = 0.0f;
        the_model->vertices[2].p.v[0] = 1.0f;
        the_model->vertices[2].p.v[1] = 0.0f;
        the_model->vertices[2].p.v[2] = -1.0f;
        the_model->vertices[2].map.v[0] = 1.0f;
        the_model->vertices[2].map.v[1] = 0.0f;
        the_model->vertices[3].p.v[0] = -1.0f;
        the_model->vertices[3].p.v[1] = 0.0f;
        the_model->vertices[3].p.v[2] = 1.0f;
        the_model->vertices[3].map.v[0] = 1.0f;
        the_model->vertices[3].map.v[1] = 1.0f;
        gOily_spills[i].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        gOily_spills[i].actor->model = the_model;
        gOily_spills[i].actor->render_style = BR_RSTYLE_NONE;
        gOily_spills[i].actor->material = the_material;
        BrActorAdd(gNon_track_actor, gOily_spills[i].actor);
    }
}

// IDA: void __cdecl ResetOilSpills()
void ResetOilSpills() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gOily_spills); i++) {
        gOily_spills[i].actor->render_style = BR_RSTYLE_NONE;
        gOily_spills[i].car = NULL;
        gOily_spills[i].stop_time = 0;
    }
}

// IDA: void __usercall QueueOilSpill(tCar_spec *pCar@<EAX>)
void QueueOilSpill(tCar_spec* pCar) {
    int i;
    int oily_index;
    int oldest_one;
    tU32 the_time;
    tU32 oldest_time;
    LOG_TRACE("(%p)", pCar);

    oldest_one = 0;
    oily_index = -1;
    the_time = GetTotalTime();
    oldest_time = GetTotalTime();

    for (i = 0; i < COUNT_OF(gOily_spills); i++) {
        if (gOily_spills[i].car == pCar && the_time < (gOily_spills[i].spill_time + 5000)) {
            return;
        }
    }

    for (i = 0; i < COUNT_OF(gOily_spills); i++) {
        if (gOily_spills[i].car == NULL) {
            oily_index = i;
            break;
        }
        if (gOily_spills[i].spill_time < oldest_time) {
            oldest_time = gOily_spills[i].spill_time;
            oldest_one = i;
        }
    }

    if (oily_index < 0) {
        oily_index = oldest_one;
    }
    gOily_spills[oily_index].car = pCar;
    gOily_spills[oily_index].spill_time = the_time + 500;
    gOily_spills[oily_index].full_size = SRandomBetween(.35f, .6f);
    gOily_spills[oily_index].grow_rate = SRandomBetween(30e-4f, 1e-4f);
    gOily_spills[oily_index].current_size = .1f;
    gOily_spills[oily_index].actor->render_style = BR_RSTYLE_NONE;
}

// IDA: int __usercall OKToSpillOil@<EAX>(tOil_spill_info *pOil@<EAX>)
int OKToSpillOil(tOil_spill_info* pOil) {
    br_scalar temp;
    br_scalar size_with_margin;
    br_scalar distance;
    br_scalar mr_dotty;
    br_vector3 v;
    br_vector3 ray_pos;
    br_vector3 ray_dir;
    br_vector3 normal;
    tCar_spec* car;
    int i;
    int face_count;
    int found_one;
    br_angle angle_to_rotate_by;
    tBounds kev_bounds;
    tFace_ref the_list[10];
    tFace_ref* face_ref;
    LOG_TRACE("(%p)", pOil);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Vector3Interpolate(br_vector3 *pDst@<EAX>, br_vector3 *pFrom@<EDX>, br_vector3 *pTo@<EBX>, br_scalar pP)
void Vector3Interpolate(br_vector3* pDst, br_vector3* pFrom, br_vector3* pTo, br_scalar pP) {
    LOG_TRACE("(%p, %p, %p, %f)", pDst, pFrom, pTo, pP);

    pDst->v[0] = (pTo->v[0] - pFrom->v[0]) * pP + pFrom->v[0];
    pDst->v[1] = (pTo->v[1] - pFrom->v[1]) * pP + pFrom->v[1];
    pDst->v[2] = (pTo->v[2] - pFrom->v[2]) * pP + pFrom->v[2];
}

// IDA: void __usercall EnsureGroundDetailVisible(br_vector3 *pNew_pos@<EAX>, br_vector3 *pGround_normal@<EDX>, br_vector3 *pOld_pos@<EBX>)
void EnsureGroundDetailVisible(br_vector3* pNew_pos, br_vector3* pGround_normal, br_vector3* pOld_pos) {
    br_scalar factor;
    br_scalar s;
    br_scalar dist;
    br_vector3 to_camera;
    LOG_TRACE("(%p, %p, %p)", pNew_pos, pGround_normal, pOld_pos);

    to_camera.v[0] = gCamera_to_world.m[3][0] - pOld_pos->v[0];
    to_camera.v[1] = gCamera_to_world.m[3][1] - pOld_pos->v[1];
    to_camera.v[2] = gCamera_to_world.m[3][2] - pOld_pos->v[2];
    dist = BrVector3Length(&to_camera);
    if (dist > BR_SCALAR_EPSILON) {
        factor = BrVector3Dot(pGround_normal, &to_camera) / dist;
        if (fabs(factor) <= 0.01f) {
            s = 0.01f;
        } else {
            s = 0.01f / factor;
            if (s > 0.1f) {
                s = 0.1f;
            }
        }
        Vector3Interpolate(pNew_pos, pOld_pos, (br_vector3*)gCamera_to_world.m[3], s);
    }
}

// IDA: void __usercall MungeOilsHeightAboveGround(tOil_spill_info *pOil@<EAX>)
void MungeOilsHeightAboveGround(tOil_spill_info* pOil) {
    LOG_TRACE("(%p)", pOil);

    EnsureGroundDetailVisible(&pOil->actor->t.t.look_up.t, &pOil->actor->t.t.look_up.up, &pOil->pos);
}

// IDA: void __usercall MungeIndexedOilsHeightAboveGround(int pIndex@<EAX>)
void MungeIndexedOilsHeightAboveGround(int pIndex) {
    LOG_TRACE("(%d)", pIndex);

    MungeOilsHeightAboveGround(&gOily_spills[pIndex]);
}

// IDA: void __usercall SetInitialOilStuff(tOil_spill_info *pOil@<EAX>, br_model *pModel@<EDX>)
void SetInitialOilStuff(tOil_spill_info* pOil, br_model* pModel) {
    LOG_TRACE("(%p, %p)", pOil, pModel);

    pModel->vertices[0].p.v[0] = -0.1f;
    pModel->vertices[0].p.v[2] = -0.1f;
    pModel->vertices[1].p.v[0] =  0.1f;
    pModel->vertices[1].p.v[2] = -0.1f;
    pModel->vertices[2].p.v[0] =  0.1f;
    pModel->vertices[2].p.v[2] =  0.1f;
    pModel->vertices[3].p.v[0] = -0.1f;
    pModel->vertices[3].p.v[2] =  0.1f;
    pOil->actor->render_style = BR_RSTYLE_FACES;
    BrMaterialUpdate(pOil->actor->material, BR_MATU_ALL);
    BrModelUpdate(pModel, BR_MODU_ALL);
}

// IDA: void __usercall ProcessOilSpills(tU32 pFrame_period@<EAX>)
void ProcessOilSpills(tU32 pFrame_period) {
    int i;
    tU32 time;
    br_model* the_model;
    br_scalar grow_amount;
    br_scalar initial_size;
    br_scalar this_size;
    br_vector3 v;
    tNet_message* message;
    LOG_TRACE("(%d)", pFrame_period);
    STUB_ONCE();
}

// IDA: int __cdecl GetOilSpillCount()
int GetOilSpillCount() {
    //LOG_TRACE("()");

    return COUNT_OF(gOily_spills);
}

// IDA: void __usercall GetOilSpillDetails(int pIndex@<EAX>, br_actor **pActor@<EDX>, br_scalar *pSize@<EBX>)
void GetOilSpillDetails(int pIndex, br_actor** pActor, br_scalar* pSize) {
    LOG_TRACE("(%d, %p, %p)", pIndex, pActor, pSize);

    if (gOily_spills[pIndex].car != NULL) {
        *pActor = gOily_spills[pIndex].actor;
        *pSize = gOily_spills[pIndex].full_size;
    } else {
        *pActor = 0;
    }
}

// IDA: int __usercall PointInSpill@<EAX>(br_vector3 *pV@<EAX>, int pSpill@<EDX>)
int PointInSpill(br_vector3* pV, int pSpill) {
    LOG_TRACE("(%p, %d)", pV, pSpill);

    return gOily_spills[pSpill].current_size * gOily_spills[pSpill].current_size * 0.8f > (pV->v[0] / WORLD_SCALE - gOily_spills[pSpill].actor->t.t.mat.m[3][0]) * (pV->v[0] / WORLD_SCALE - gOily_spills[pSpill].actor->t.t.mat.m[3][0])
        && gOily_spills[pSpill].current_size * gOily_spills[pSpill].current_size * 0.8f > (pV->v[2] / WORLD_SCALE - gOily_spills[pSpill].actor->t.t.mat.m[3][2]) * (pV->v[2] / WORLD_SCALE - gOily_spills[pSpill].actor->t.t.mat.m[3][2])
        && fabs(pV->v[1] / WORLD_SCALE - gOily_spills[pSpill].actor->t.t.mat.m[3][1]) < 0.1f;
}

// IDA: void __usercall GetOilFrictionFactors(tCar_spec *pCar@<EAX>, br_scalar *pFl_factor@<EDX>, br_scalar *pFr_factor@<EBX>, br_scalar *pRl_factor@<ECX>, br_scalar *pRr_factor)
void GetOilFrictionFactors(tCar_spec* pCar, br_scalar* pFl_factor, br_scalar* pFr_factor, br_scalar* pRl_factor, br_scalar* pRr_factor) {
    int i;
    br_vector3 wheel_world;
    LOG_TRACE("(%p, %p, %p, %p, %p)", pCar, pFl_factor, pFr_factor, pRl_factor, pRr_factor);

    *pFl_factor = 1.0;
    *pFr_factor = 1.0;
    *pRl_factor = 1.0;
    *pRr_factor = 1.0;
    switch (pCar->driver) {
    case eDriver_non_car_unused_slot:
    case eDriver_non_car:
        return;
    default:
        break;
    }
    if (pCar->shadow_intersection_flags != 0) {
        for (i = 0; i < COUNT_OF(gOily_spills); i++) {
            if (((1 << i) & pCar->shadow_intersection_flags) != 0 && gOily_spills[i].car) {
                BrMatrix34ApplyP(&wheel_world, &pCar->wpos[2], &pCar->car_master_actor->t.t.mat);
                if (PointInSpill(&wheel_world, i)) {
                    pCar->oil_remaining[2] = SRandomBetween(1.5f, 2.5f);
                }
                BrMatrix34ApplyP(&wheel_world, &pCar->wpos[3], &pCar->car_master_actor->t.t.mat);
                if (PointInSpill(&wheel_world, i)) {
                    pCar->oil_remaining[3] = SRandomBetween(1.5f, 2.5f);
                }
                BrMatrix34ApplyP(&wheel_world, &pCar->wpos[0], &pCar->car_master_actor->t.t.mat);
                if (PointInSpill(&wheel_world, i)) {
                    pCar->oil_remaining[0] = SRandomBetween(1.5f, 2.5f);
                }
                BrMatrix34ApplyP(&wheel_world, &pCar->wpos[1], &pCar->car_master_actor->t.t.mat);
                if (PointInSpill(&wheel_world, i)) {
                    pCar->oil_remaining[1] = SRandomBetween(1.5f, 2.5f);
                }
            }
        }
    }
    if (pCar->oil_remaining[2] != 0.0f) {
        *pFl_factor = SRandomBetween(0.01f, 0.15f);
    }
    if (pCar->oil_remaining[3] != 0.0f) {
        *pFr_factor = SRandomBetween(0.01f, 0.15f);
    }
    if (pCar->oil_remaining[0] != 0.0f) {
        *pRl_factor = SRandomBetween(0.01f, 0.15f);
    }
    if (pCar->oil_remaining[1] != 0.0f) {
        *pRr_factor = SRandomBetween(0.01f, 0.15f);
    }
}

// IDA: void __usercall AdjustOilSpill(int pIndex@<EAX>, br_matrix34 *pMat@<EDX>, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec *pCar, br_vector3 *pOriginal_pos, br_pixelmap *pPixelmap)
void AdjustOilSpill(int pIndex, br_matrix34* pMat, br_scalar pFull_size, br_scalar pGrow_rate, tU32 pSpill_time, tU32 pStop_time, tCar_spec* pCar, br_vector3* pOriginal_pos, br_pixelmap* pPixelmap) {
    LOG_TRACE("(%d, %p, %f, %f, %d, %d, %p, %p, %p)", pIndex, pMat, pFull_size, pGrow_rate, pSpill_time, pStop_time, pCar, pOriginal_pos, pPixelmap);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedOilSpill(tNet_contents *pContents@<EAX>)
void ReceivedOilSpill(tNet_contents* pContents) {
    int i;
    int oily_index;
    int oldest_one;
    tU32 the_time;
    tU32 oldest_time;
    tCar_spec* car;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}
