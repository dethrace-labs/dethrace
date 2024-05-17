#include "oil.h"
#include "finteray.h"
#include "globvars.h"
#include "globvrpb.h"
#include "harness/trace.h"
#include "loading.h"
#include "network.h"
#include "piping.h"
#include "utility.h"
#include <brender.h>
#include <math.h>
#include <stdlib.h>

char* gOil_pixie_names[1] = { "OIL.PIX" };
int gNext_oil_pixie = 0;
br_scalar gZ_buffer_diff;
br_scalar gMin_z_diff;
br_pixelmap* gOil_pixies[1];
tOil_spill_info gOily_spills[15];

// IDA: void __cdecl InitOilSpills()
void InitOilSpills(void) {
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
        the_material->power = 0.0f;
        the_material->index_base = 0;
        the_material->flags |= BR_MATF_LIGHT;
        the_material->flags |= BR_MATF_PERSPECTIVE;
        the_material->flags |= BR_MATF_SMOOTH;
        // TODO: added by dethrace, investigate why oil spills in OG do not need this flag set to render correctly
        the_material->flags |= BR_MATF_TWO_SIDED;
        the_material->index_range = 0;
        the_material->colour_map = NULL;
        BrMatrix23Identity(&the_material->map_transform);
        the_material->index_shade = BrTableFind("IDENTITY.TAB");
        BrMaterialUpdate(the_material, BR_MATU_ALL);
        the_model = BrModelAllocate(NULL, 4, 2);
        the_model->flags |= BR_MODF_KEEP_ORIGINAL;

        the_model->faces[0].vertices[0] = 2;
        the_model->faces[0].vertices[1] = 1;
        the_model->faces[0].vertices[2] = 0;
        the_model->faces[0].material = NULL;
        the_model->faces[0].smoothing = 1;
        the_model->faces[1].vertices[0] = 3;
        the_model->faces[1].vertices[1] = 2;
        the_model->faces[1].vertices[2] = 0;
        the_model->faces[1].material = NULL;
        the_model->faces[1].smoothing = 1;
        BrVector3Set(&the_model->vertices[0].p, -1.f, 0.f, -1.f);
        BrVector2Set(&the_model->vertices[0].map, 0.f, 1.f);
        BrVector3Set(&the_model->vertices[1].p, 1.f, 0.f, 1.f);
        BrVector2Set(&the_model->vertices[1].map, 0.f, 0.f);
        BrVector3Set(&the_model->vertices[2].p, 1.f, 0.f, -1.f);
        BrVector2Set(&the_model->vertices[2].map, 1.f, 0.f);
        BrVector3Set(&the_model->vertices[3].p, -1.f, 0.f, 1.f);
        BrVector2Set(&the_model->vertices[3].map, 1.f, 1.f);
        gOily_spills[i].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        gOily_spills[i].actor->model = the_model;
        gOily_spills[i].actor->render_style = BR_RSTYLE_NONE;
        gOily_spills[i].actor->material = the_material;
        BrActorAdd(gNon_track_actor, gOily_spills[i].actor);
    }
}

// IDA: void __cdecl ResetOilSpills()
void ResetOilSpills(void) {
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
        if (gOily_spills[i].car == pCar && the_time < gOily_spills[i].spill_time + 5000) {
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
    gOily_spills[oily_index].grow_rate = SRandomBetween(3e-5f, 10e-5f);
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

    car = pOil->car;
    if (car->driver >= eDriver_net_human && car->damage_units[eDamage_engine].damage_level <= 98 && car->damage_units[eDamage_transmission].damage_level <= 98) {
        return 0;
    }
    angle_to_rotate_by = IRandomBetween(0, 0xffff);
    kev_bounds.original_bounds.min.v[0] = -pOil->full_size;
    kev_bounds.original_bounds.min.v[1] = 1.5f * car->car_model_actors[car->principal_car_actor].actor->model->bounds.min.v[1];
    kev_bounds.original_bounds.min.v[2] = -pOil->full_size;
    kev_bounds.original_bounds.max.v[0] = pOil->full_size;
    kev_bounds.original_bounds.max.v[1] = car->car_model_actors[car->principal_car_actor].actor->model->bounds.max.v[1];
    kev_bounds.original_bounds.max.v[2] = pOil->full_size;
    BrMatrix34PreRotateY(&pOil->actor->t.t.mat, angle_to_rotate_by);
    kev_bounds.mat = &car->car_master_actor->t.t.mat;
    face_count = FindFacesInBox(&kev_bounds, the_list, COUNT_OF(the_list));
    BrVector3Set(&v, .0f, .2f, .0f);
    BrMatrix34ApplyP(&ray_pos, &v, &car->car_master_actor->t.t.mat);
    BrVector3Set(&ray_dir, 0.f, kev_bounds.original_bounds.min.v[1] - kev_bounds.original_bounds.max.v[1], 0.f);
    if (face_count == 0) {
        return 0;
    }
    found_one = 0;
    for (i = 0; i < face_count; i++) {
        face_ref = &the_list[i];
        if (!found_one) {
            CheckSingleFace(face_ref, &ray_pos, &ray_dir, &normal, &distance);
            if (distance < 100.f) {
                found_one = 1;
                BrVector3Copy((br_vector3*)pOil->actor->t.t.mat.m[1], &normal);
                BrVector3Set(&v, 0.f, 0.f, 1.f);
                BrVector3Cross((br_vector3*)pOil->actor->t.t.mat.m[0], &normal, &v);
                BrVector3Set(&v, 1.f, 0.f, 0.f);
                BrVector3Cross((br_vector3*)pOil->actor->t.t.mat.m[2], &normal, &v);
                BrVector3Scale(&v, &ray_dir, distance);
                BrVector3Add(&pOil->pos, &ray_pos, &v);
                BrMatrix34PreRotateY(&pOil->actor->t.t.mat, angle_to_rotate_by);
            }
        }
    }
    if (!found_one || normal.v[1] < .97f) {
        return 0;
    }
    for (i = 0; i < face_count; i++) {
        face_ref = &the_list[i];
        mr_dotty = BrVector3Dot(&face_ref->normal, &normal);
        if (mr_dotty < .98f && (mr_dotty > .8f || !NormalSideOfPlane(&pOil->actor->t.t.translate.t, &face_ref->normal, face_ref->d))) {
            return 0;
        }
    }
    return 1;
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
        if (fabsf(factor) <= 0.01f) {
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
    pModel->vertices[1].p.v[0] = 0.1f;
    pModel->vertices[1].p.v[2] = -0.1f;
    pModel->vertices[2].p.v[0] = 0.1f;
    pModel->vertices[2].p.v[2] = 0.1f;
    pModel->vertices[3].p.v[0] = -0.1f;
    pModel->vertices[3].p.v[2] = 0.1f;
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

    time = GetTotalTime();
    for (i = 0; i < COUNT_OF(gOily_spills); i++) {
        if (gOily_spills[i].car == NULL) {
            gOily_spills[i].actor->render_style = BR_RSTYLE_NONE;
        } else {
            the_model = gOily_spills[i].actor->model;
            if (gOily_spills[i].actor->render_style == BR_RSTYLE_NONE && gOily_spills[i].spill_time <= time && fabsf(gOily_spills[i].car->v.v[0]) < .01f && fabsf(gOily_spills[i].car->v.v[1]) < .01f && fabsf(gOily_spills[i].car->v.v[2]) < .01f) {
                if (gAction_replay_mode) {
                    SetInitialOilStuff(&gOily_spills[i], the_model);
                } else {
                    if (!OKToSpillOil(&gOily_spills[i])) {
                        gOily_spills[i].car = NULL;
                    } else {
                        gOily_spills[i].spill_time = time;
                        gOily_spills[i].actor->material->colour_map = gOil_pixies[gNext_oil_pixie];
                        gNext_oil_pixie++;
                        if (gNext_oil_pixie >= COUNT_OF(gOil_pixies)) {
                            gNext_oil_pixie = 0;
                        }
                        BrVector3Copy(&gOily_spills[i].original_pos, &gOily_spills[i].car->pos);
                        PipeSingleOilSpill(i,
                            &gOily_spills[i].actor->t.t.mat,
                            gOily_spills[i].full_size,
                            gOily_spills[i].grow_rate,
                            gOily_spills[i].spill_time,
                            gOily_spills[i].stop_time,
                            gOily_spills[i].car,
                            &gOily_spills[i].original_pos,
                            gOily_spills[i].actor->material->colour_map);
                        gOily_spills[i].stop_time = 0;
                        SetInitialOilStuff(&gOily_spills[i], the_model);
                        if (gNet_mode != eNet_mode_none) {
                            message = NetBuildMessage(30, 0);
                            message->contents.data.oil_spill.player = NetPlayerFromCar(gOily_spills[i].car)->ID;
                            message->contents.data.oil_spill.full_size = gOily_spills[i].full_size;
                            message->contents.data.oil_spill.grow_rate = gOily_spills[i].grow_rate;
                            message->contents.data.oil_spill.current_size = gOily_spills[i].current_size;
                            NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, NULL);
                        }
                    }
                }
            } else {
                if (gOily_spills[i].actor->render_style == BR_RSTYLE_FACES && (gOily_spills[i].stop_time == 0 || time < gOily_spills[i].stop_time)) {
                    BrVector3Sub(&v, &gOily_spills[i].original_pos, &gOily_spills[i].car->pos);
                    grow_amount = BrVector3LengthSquared(&v);
                    if (gOily_spills[i].stop_time != 0 || grow_amount <= 0.2f) {
                        this_size = 0.1f + (time - gOily_spills[i].spill_time) * gOily_spills[i].grow_rate;
                        if (this_size >= 0.1f) {
                            gOily_spills[i].actor->render_style = BR_RSTYLE_FACES;
                            if (this_size <= gOily_spills[i].full_size) {
                                the_model->vertices[0].p.v[0] = -this_size;
                                the_model->vertices[0].p.v[2] = -this_size;
                                the_model->vertices[1].p.v[0] = this_size;
                                the_model->vertices[1].p.v[2] = -this_size;
                                the_model->vertices[2].p.v[0] = this_size;
                                the_model->vertices[2].p.v[2] = this_size;
                                the_model->vertices[3].p.v[0] = -this_size;
                                the_model->vertices[3].p.v[2] = this_size;
                                gOily_spills[i].current_size = this_size;
                            } else {
                                the_model->vertices[0].p.v[0] = -gOily_spills[i].full_size;
                                the_model->vertices[0].p.v[2] = -gOily_spills[i].full_size;
                                the_model->vertices[1].p.v[0] = gOily_spills[i].full_size;
                                the_model->vertices[1].p.v[2] = -gOily_spills[i].full_size;
                                the_model->vertices[2].p.v[0] = gOily_spills[i].full_size;
                                the_model->vertices[2].p.v[2] = gOily_spills[i].full_size;
                                the_model->vertices[3].p.v[0] = -gOily_spills[i].full_size;
                                the_model->vertices[3].p.v[2] = gOily_spills[i].full_size;
                                gOily_spills[i].current_size = gOily_spills[i].full_size;
                            }
                            BrModelUpdate(the_model, BR_MODU_ALL);
                        } else {
                            gOily_spills[i].actor->render_style = BR_RSTYLE_NONE;
                        }
                    } else {
                        gOily_spills[i].stop_time = time;
                        continue;
                    }
                }
            }
        }
        if (gOily_spills[i].actor->render_style == BR_RSTYLE_FACES) {
            MungeOilsHeightAboveGround(&gOily_spills[i]);
        }
    }
}

// IDA: int __cdecl GetOilSpillCount()
int GetOilSpillCount(void) {
    // LOG_TRACE("()");

    return COUNT_OF(gOily_spills);
}

// IDA: void __usercall GetOilSpillDetails(int pIndex@<EAX>, br_actor **pActor@<EDX>, br_scalar *pSize@<EBX>)
void GetOilSpillDetails(int pIndex, br_actor** pActor, br_scalar* pSize) {
    LOG_TRACE("(%d, %p, %p)", pIndex, pActor, pSize);

    if (gOily_spills[pIndex].car != NULL) {
        *pActor = gOily_spills[pIndex].actor;
        *pSize = gOily_spills[pIndex].full_size;
    } else {
        *pActor = NULL;
    }
}

#define SQR(V) ((V) * (V))

// IDA: int __usercall PointInSpill@<EAX>(br_vector3 *pV@<EAX>, int pSpill@<EDX>)
int PointInSpill(br_vector3* pV, int pSpill) {
    LOG_TRACE("(%p, %d)", pV, pSpill);

    return gOily_spills[pSpill].current_size * gOily_spills[pSpill].current_size * 0.8f > SQR(pV->v[0] / WORLD_SCALE - gOily_spills[pSpill].actor->t.t.translate.t.v[0])
        && gOily_spills[pSpill].current_size * gOily_spills[pSpill].current_size * 0.8f > SQR(pV->v[2] / WORLD_SCALE - gOily_spills[pSpill].actor->t.t.translate.t.v[2])
        && fabsf(pV->v[1] / WORLD_SCALE - gOily_spills[pSpill].actor->t.t.translate.t.v[1]) < 0.1f;
}

// IDA: void __usercall GetOilFrictionFactors(tCar_spec *pCar@<EAX>, br_scalar *pFl_factor@<EDX>, br_scalar *pFr_factor@<EBX>, br_scalar *pRl_factor@<ECX>, br_scalar *pRr_factor)
void GetOilFrictionFactors(tCar_spec* pCar, br_scalar* pFl_factor, br_scalar* pFr_factor, br_scalar* pRl_factor, br_scalar* pRr_factor) {
    int i;
    br_vector3 wheel_world;
    LOG_TRACE("(%p, %p, %p, %p, %p)", pCar, pFl_factor, pFr_factor, pRl_factor, pRr_factor);

    *pFl_factor = 1.0f;
    *pFr_factor = 1.0f;
    *pRl_factor = 1.0f;
    *pRr_factor = 1.0f;
    switch (pCar->driver) {
    case eDriver_non_car_unused_slot:
    case eDriver_non_car:
        return;
    default:
        break;
    }
    if (pCar->shadow_intersection_flags != 0) {
        for (i = 0; i < COUNT_OF(gOily_spills); i++) {
            if (((1 << i) & pCar->shadow_intersection_flags) != 0 && gOily_spills[i].car != NULL) {
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

    BrMatrix34Copy(&gOily_spills[pIndex].actor->t.t.mat, pMat);
    gOily_spills[pIndex].full_size = pFull_size;
    gOily_spills[pIndex].grow_rate = pGrow_rate;
    gOily_spills[pIndex].spill_time = pSpill_time;
    gOily_spills[pIndex].stop_time = pStop_time;
    gOily_spills[pIndex].car = pCar;
    BrVector3Copy(&gOily_spills[pIndex].original_pos, pOriginal_pos);
    gOily_spills[pIndex].actor->material->colour_map = pPixelmap;
    gOily_spills[pIndex].actor->render_style = BR_RSTYLE_NONE;
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

    oldest_one = 0;
    car = NetCarFromPlayerID(pContents->data.oil_spill.player);
    if (car == NULL) {
        return;
    }
    oily_index = -1;
    the_time = GetTotalTime();
    oldest_time = GetTotalTime();
    for (i = 0; i < COUNT_OF(gOily_spills); i++) {
        if (gOily_spills[i].car == car && the_time < gOily_spills[i].spill_time + 5000) {
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
    gOily_spills[oily_index].car = car;
    gOily_spills[oily_index].spill_time = the_time;
    gOily_spills[oily_index].full_size = pContents->data.oil_spill.full_size;
    gOily_spills[oily_index].grow_rate = pContents->data.oil_spill.grow_rate;
    gOily_spills[oily_index].current_size = pContents->data.oil_spill.current_size;
    gOily_spills[oily_index].actor->render_style = BR_RSTYLE_NONE;
}
