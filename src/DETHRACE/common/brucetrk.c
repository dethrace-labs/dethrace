#include "brucetrk.h"

#include "brender.h"
#include "errors.h"
#include "formats.h"
#include "globvars.h"
#include "globvrbm.h"
#include "harness/trace.h"
#include "init.h"
#include "pd/sys.h"
#include "utility.h"
#include "world.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// GLOBAL: CARM95 0x0053e488
br_actor* gMr_blendy;

// GLOBAL: CARM95 0x00550244
int gDefault_blend_pc;

// IDA: void __usercall AllocateActorMatrix(tTrack_spec *pTrack_spec@<EAX>, br_actor ****pDst@<EDX>)
// FUNCTION: CARM95 0x004a8a26
void AllocateActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pDst) {
    tU16 z;

    *pDst = BrMemAllocate(sizeof(br_actor***) * pTrack_spec->ncolumns_z, kMem_columns_z);
    for (z = 0; z != pTrack_spec->ncolumns_z; z++) {
        (*pDst)[z] = BrMemAllocate(sizeof(br_actor**) * pTrack_spec->ncolumns_x, kMem_columns_x);
        memset((*pDst)[z], 0, sizeof(br_actor**) * pTrack_spec->ncolumns_x);
    }
}

// IDA: void __usercall DisposeActorMatrix(tTrack_spec *pTrack_spec@<EAX>, br_actor ****pVictim@<EDX>, int pRemove_act_mod@<EBX>)
// FUNCTION: CARM95 0x004a8610
void DisposeActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pVictim, int pRemove_act_mod) {
    tU16 z;
    tU16 x;

    if (*pVictim != NULL) {
        for (z = 0; z != pTrack_spec->ncolumns_z; z++) {
            if (pRemove_act_mod != 0) {
                for (x = 0; x != pTrack_spec->ncolumns_x; x++) {
                    if ((*pVictim)[z][x] != NULL && (*pVictim)[z][x]->model != NULL) {
                        BrModelRemove((*pVictim)[z][x]->model);
                        BrModelFree((*pVictim)[z][x]->model);
                    }
                }
            }
            BrMemFree((*pVictim)[z]);
        }
        BrMemFree(*pVictim);
    }
}

// IDA: void __usercall DisposeColumns(tTrack_spec *pTrack_spec@<EAX>)
// FUNCTION: CARM95 0x004a8590
void DisposeColumns(tTrack_spec* pTrack_spec) {

    DisposeActorMatrix(pTrack_spec, &pTrack_spec->columns, 0);
    DisposeActorMatrix(pTrack_spec, &pTrack_spec->lollipops, 0);
    if (gAusterity_mode == 0) {
        DisposeActorMatrix(pTrack_spec, &pTrack_spec->blends, 1);
    }
    if (pTrack_spec->non_car_list != NULL && (0 < pTrack_spec->ampersand_digits)) {
        BrMemFree(pTrack_spec->non_car_list);
    }
}

// IDA: void __usercall XZToColumnXZ(tU8 *pColumn_x@<EAX>, tU8 *pColumn_z@<EDX>, br_scalar pX, br_scalar pZ, tTrack_spec *pTrack_spec)
// FUNCTION: CARM95 0x004a874b
void XZToColumnXZ(tU8* pColumn_x, tU8* pColumn_z, br_scalar pX, br_scalar pZ, tTrack_spec* pTrack_spec) {
    br_scalar x;
    br_scalar z;

    x = (pX - pTrack_spec->origin_x) / pTrack_spec->column_size_x;
    z = (pZ - pTrack_spec->origin_z) / pTrack_spec->column_size_z;
    if (x < 0.0f) {
        x = 0.0f;
    }
    if (x >= pTrack_spec->ncolumns_x) {
        x = pTrack_spec->ncolumns_x - 1.0f;
    }
    if (z < 0.0f) {
        z = 0.0f;
    }
    if (z >= pTrack_spec->ncolumns_z) {
        z = pTrack_spec->ncolumns_z - 1.0f;
    }
    *pColumn_x = x;
    *pColumn_z = z;
}

// IDA: void __usercall StripBlendedFaces(br_actor *pActor@<EAX>, br_model *pModel@<EDX>)
// FUNCTION: CARM95 0x004a8d47
void StripBlendedFaces(br_actor* pActor, br_model* pModel) {
    int i;
    br_face* face;
    int changed_one;
    char s[256];
    // GLOBAL: CARM95 0x53e47c
    static tU16 nfaces_allocated;

    changed_one = 0;

    for (i = 0; i < pModel->nfaces; i++) {
        face = &pModel->faces[i];
        if (face->material != NULL && face->material->identifier != NULL && ((face->material->identifier[0] == '!' && face->material->identifier[1] != '!' && gDefault_blend_pc != 0) || face->material->identifier[1] == '\\')) {
            if (gMr_blendy == NULL) {
                gMr_blendy = BrActorAllocate(BR_ACTOR_MODEL, NULL);
                gMr_blendy->render_style = BR_RSTYLE_NONE;
                gMr_blendy->model = BrModelAllocate(NULL, pModel->nvertices, pModel->nfaces);
                nfaces_allocated = pModel->nfaces;
                gMr_blendy->model->nfaces = 0;
                gMr_blendy->model->flags |= BR_MODF_UPDATEABLE;
                memcpy(gMr_blendy->model->vertices, pModel->vertices, pModel->nvertices * sizeof(br_vertex));
            }
            if (!AlreadyBlended(face->material)) {
                if (face->material->identifier[1] == '\\') {
                    if (face->material->identifier[2] == '2') {
                        BlendifyMaterial(face->material, 25);
                    } else if (face->material->identifier[2] == '7') {
                        BlendifyMaterial(face->material, 75);
                    } else {
                        BlendifyMaterial(face->material, 50);
                    }
                } else {
                    BlendifyMaterial(face->material, gDefault_blend_pc);
                }
                BrMaterialUpdate(face->material, BR_MATU_ALL);
            }
            if (nfaces_allocated <= gMr_blendy->model->nfaces) {
                PDFatalError("Perfectly understandable error by Batwick, thank you very much Bruce.");
            }
            memcpy(&gMr_blendy->model->faces[gMr_blendy->model->nfaces], face, sizeof(br_face));
            gMr_blendy->model->nfaces++;
            if (i < (pModel->nfaces - 1)) {
                memmove(&pModel->faces[i], &pModel->faces[i + 1], (pModel->nfaces - i - 1) * sizeof(br_face));
            }
            pModel->nfaces--;
            changed_one = 1;
            i--;
        }
    }
    if (changed_one) {
        if (pModel->nfaces != 0) {
            BrModelUpdate(pModel, BR_MODU_ALL);
        } else {
            pActor->model = NULL;
            pActor->type = BR_ACTOR_NONE;
        }
    }
}

// IDA: br_uint_32 __cdecl FindNonCarsCB(br_actor *pActor, tTrack_spec *pTrack_spec)
// FUNCTION: CARM95 0x004a90ff
br_uintptr_t FindNonCarsCB(br_actor* pActor, tTrack_spec* pTrack_spec) {
    int i;
    br_scalar r1;
    br_scalar r2;
    br_scalar r3;

    if (pActor->identifier != NULL && pActor->identifier[0] == '&' && pActor->identifier[1] >= '0' && pActor->identifier[1] <= '9') {
        i = (pActor->identifier[4] - '0') * 1000 + (pActor->identifier[5] - '0') * 100 + (pActor->identifier[6] - '0') * 10 + (pActor->identifier[7] - '0');
        if (i < 0 || pTrack_spec->ampersand_digits <= i) {
            return 1;
        }
        r1 = BR_SQR3(pActor->t.t.mat.m[0][0], pActor->t.t.mat.m[0][1], pActor->t.t.mat.m[0][2]);
        r2 = BR_SQR3(pActor->t.t.mat.m[1][0], pActor->t.t.mat.m[1][1], pActor->t.t.mat.m[1][2]);
        r3 = BR_SQR3(pActor->t.t.mat.m[2][0], pActor->t.t.mat.m[2][1], pActor->t.t.mat.m[2][2]);
        if (r1 < .999f || r2 < .999f || r3 < .999f) {
            dr_dprintf("non car was scaled down %s", pActor->identifier);
            pActor->t.t.translate.t.v[0] += 2000.f;
        }
        if (r1 > 1.001f || r2 > 1.001f || r3 > 1.001f) {
            r1 = 1.f / sqrt(r1);
            r2 = 1.f / sqrt(r2);
            r3 = 1.f / sqrt(r3);
            pActor->t.t.mat.m[0][0] *= r1;
            pActor->t.t.mat.m[0][1] *= r1;
            pActor->t.t.mat.m[0][2] *= r1;
            pActor->t.t.mat.m[1][0] *= r2;
            pActor->t.t.mat.m[1][1] *= r2;
            pActor->t.t.mat.m[1][2] *= r2;
            pActor->t.t.mat.m[2][0] *= r3;
            pActor->t.t.mat.m[2][1] *= r3;
            pActor->t.t.mat.m[2][2] *= r3;
            dr_dprintf("non car was scaled up %s", pActor->identifier);
        }
        pTrack_spec->non_car_list[i] = pActor;
        pActor->type_data = NULL;
        return 0;
    } else {
        if (pActor->model != NULL && !gAusterity_mode && pActor->identifier != NULL && pActor->identifier[0] != '&') {
            StripBlendedFaces(pActor, pActor->model);
        }
        return BrActorEnum(pActor, (br_actor_enum_cbfn*)FindNonCarsCB, pTrack_spec);
    }
}

// IDA: br_uint_32 __cdecl ProcessModelsCB(br_actor *pActor, tTrack_spec *pTrack_spec)
// FUNCTION: CARM95 0x004a8afc
br_uintptr_t ProcessModelsCB(br_actor* pActor, tTrack_spec* pTrack_spec) {
    unsigned int x;
    unsigned int z;
    int group;

    if (sscanf(pActor->identifier, "%u%u", &x, &z) == 2 && pTrack_spec->ncolumns_x > x && pTrack_spec->ncolumns_z > z) {
        pActor->material = gDefault_track_material;
        pTrack_spec->columns[z][x] = pActor;
        gMr_blendy = NULL;
        if (pActor->model && !gAusterity_mode) {
            StripBlendedFaces(pActor, pActor->model);
        }
        BrActorEnum(pActor, (br_actor_enum_cbfn*)FindNonCarsCB, pTrack_spec);
        if (gMr_blendy) {
            BrActorAdd(pActor, gMr_blendy);
            BrModelAdd(gMr_blendy->model);
            for (group = 0; group < V11MODEL(gMr_blendy->model)->ngroups; group++) {
                V11MODEL(gMr_blendy->model)->groups[group].user = gMr_blendy->model->faces[*V11MODEL(gMr_blendy->model)->groups[group].face_user].material;
            }
            gMr_blendy->model->flags &= ~BR_MODF_UPDATEABLE;
            DodgyModelUpdate(gMr_blendy->model);
            pTrack_spec->blends[z][x] = gMr_blendy;
        }
    } else if (*pActor->identifier == '%' && sscanf((const char*)pActor->identifier + 1, "%u%u", &x, &z) == 2 && pTrack_spec->ncolumns_x > x && pTrack_spec->ncolumns_z > z) {
        pTrack_spec->lollipops[z][x] = pActor;
    } else {
        BrActorEnum(pActor, (br_actor_enum_cbfn*)ProcessModelsCB, pTrack_spec);
    }
    return 0;
}

// IDA: void __usercall ProcessModels(tTrack_spec *pTrack_spec@<EAX>)
// FUNCTION: CARM95 0x004a8ad9
void ProcessModels(tTrack_spec* pTrack_spec) {

    BrActorEnum(pTrack_spec->the_actor, (br_actor_enum_cbfn*)ProcessModelsCB, pTrack_spec);
}

// IDA: void __usercall ExtractColumns(tTrack_spec *pTrack_spec@<EAX>)
// FUNCTION: CARM95 0x004a884d
void ExtractColumns(tTrack_spec* pTrack_spec) {
    unsigned int x;
    unsigned int z;
    int ad;
    int unsplit;
    float e;
    br_scalar extra_room;
    br_bounds bounds;

    unsplit = 0;
    switch (sscanf(pTrack_spec->the_actor->identifier, "%u%u%f%d", &x, &z, &extra_room, &ad)) {
    case 3:
        BrFailure(
            "Attempt to extract columns from invalid track\n"
            "(It might have been produced by an ancient preproc.\n"
            "This is no longer supported.\n");
        break;

    case 4:
        pTrack_spec->ampersand_digits = ad;
        break;

    default:
        unsplit = 1;
        x = 1;
        z = 1;
        extra_room = 0.0;
        pTrack_spec->ampersand_digits = 0;
    }
    pTrack_spec->ncolumns_x = x;
    pTrack_spec->ncolumns_z = z;

    BrActorToBounds(&bounds, pTrack_spec->the_actor);
    pTrack_spec->column_size_x = (bounds.max.v[0] - bounds.min.v[0] + extra_room * 2.0) / (double)pTrack_spec->ncolumns_x;
    pTrack_spec->column_size_z = (bounds.max.v[2] - bounds.min.v[2] + extra_room * 2.0) / (double)pTrack_spec->ncolumns_z;
    pTrack_spec->origin_x = bounds.min.v[0] - extra_room;
    pTrack_spec->origin_z = bounds.min.v[2] - extra_room;
    AllocateActorMatrix(pTrack_spec, &pTrack_spec->columns);
    AllocateActorMatrix(pTrack_spec, &pTrack_spec->lollipops);
    AllocateActorMatrix(pTrack_spec, &pTrack_spec->blends);
    if (pTrack_spec->ampersand_digits <= 0) {
        pTrack_spec->non_car_list = NULL;
    } else {
        pTrack_spec->non_car_list = BrMemAllocate(sizeof(br_actor*) * pTrack_spec->ampersand_digits, kMem_non_car_list);
    }
    if (unsplit) {
        **pTrack_spec->columns = pTrack_spec->the_actor;
    } else {
        ProcessModels(pTrack_spec);
    }
}

// IDA: void __usercall LollipopizeActor4(br_actor *pActor@<EAX>, br_matrix34 *pRef_to_world@<EDX>, br_actor *pCamera@<EBX>)
// FUNCTION: CARM95 0x004a9dc1
void LollipopizeActor4(br_actor* pActor, br_matrix34* pRef_to_world, br_actor* pCamera) {

    pActor->t.t.mat.m[1][0] = 0.0;
    pActor->t.t.mat.m[1][1] = 1.0;
    pActor->t.t.mat.m[1][2] = 0.0;
    pActor->t.t.mat.m[2][0] = pRef_to_world->m[2][0];
    pActor->t.t.mat.m[2][1] = pRef_to_world->m[2][1];
    pActor->t.t.mat.m[2][2] = pRef_to_world->m[2][2];
    pActor->t.t.mat.m[0][0] = pActor->t.t.mat.m[1][1] * pActor->t.t.mat.m[2][2]
        - pActor->t.t.mat.m[1][2] * pActor->t.t.mat.m[2][1];
    pActor->t.t.mat.m[0][1] = pActor->t.t.mat.m[1][2] * pActor->t.t.mat.m[2][0]
        - pActor->t.t.mat.m[1][0] * pActor->t.t.mat.m[2][2];
    pActor->t.t.mat.m[0][2] = pActor->t.t.mat.m[2][1] * pActor->t.t.mat.m[1][0]
        - pActor->t.t.mat.m[1][1] * pActor->t.t.mat.m[2][0];
}

// IDA: br_uint_32 __cdecl LollipopizeChildren(br_actor *pActor, void *pArg)
// FUNCTION: CARM95 0x004a9d8d
br_uintptr_t LollipopizeChildren(br_actor* pActor, void* pArg) {
    tMatrix_and_actor* maa;

    maa = pArg;
    LollipopizeActor4(pActor, maa->m, maa->a);
    return 0;
}

// IDA: void __usercall DrawColumns(int pDraw_blends@<EAX>, tTrack_spec *pTrack_spec@<EDX>, int pMin_x@<EBX>, int pMax_x@<ECX>, int pMin_z, int pMax_z, br_matrix34 *pCamera_to_world)
// FUNCTION: CARM95 0x004a9a01
void DrawColumns(int pDraw_blends, tTrack_spec* pTrack_spec, int pMin_x, int pMax_x, int pMin_z, int pMax_z, br_matrix34* pCamera_to_world) {
    tU8 column_x;
    tU8 column_z;
    tU8 column_x2;
    tU8 column_z2;
    tMatrix_and_actor maa;
    br_actor* blended_polys;

    maa.m = pCamera_to_world;
    if (fabs(pCamera_to_world->m[2][2]) >= fabs(pCamera_to_world->m[2][0])) {
        for (column_z = pMin_z; column_z <= pMax_z; ++column_z) {
            for (column_x = pMin_x; column_x <= pMax_x; ++column_x) {
                if (pCamera_to_world->m[2][0] <= 0.0) {
                    column_x2 = pMin_x + pMax_x - column_x;
                } else {
                    column_x2 = column_x;
                }
                if (pCamera_to_world->m[2][2] <= 0.0) {
                    column_z2 = pMax_z + pMin_z - column_z;
                } else {
                    column_z2 = column_z;
                }
                if (pDraw_blends) {
                    blended_polys = pTrack_spec->blends[column_z2][column_x2];
                    if (blended_polys) {
                        blended_polys->render_style = BR_RSTYLE_FACES;
                        BrZbSceneRenderAdd(blended_polys);
                        blended_polys->render_style = BR_RSTYLE_NONE;
                    }
                } else {
                    if (pTrack_spec->columns[column_z2][column_x2]) {
                        BrZbSceneRenderAdd(pTrack_spec->columns[column_z2][column_x2]);
                    }
                    if (pTrack_spec->lollipops[column_z2][column_x2]) {
                        maa.a = pTrack_spec->lollipops[column_z2][column_x2];
                        BrActorEnum(pTrack_spec->lollipops[column_z2][column_x2], LollipopizeChildren, &maa);
                        BrZbSceneRenderAdd(pTrack_spec->lollipops[column_z2][column_x2]);
                    }
                }
            }
        }
    } else {
        for (column_x = pMin_x; column_x <= pMax_x; ++column_x) {
            for (column_z = pMin_z; column_z <= pMax_z; ++column_z) {
                if (pCamera_to_world->m[2][0] <= 0.0) {
                    column_x2 = pMin_x + pMax_x - column_x;
                } else {
                    column_x2 = column_x;
                }
                if (pCamera_to_world->m[2][2] <= 0.0) {
                    column_z2 = pMax_z + pMin_z - column_z;
                } else {
                    column_z2 = column_z;
                }
                if (pDraw_blends) {
                    blended_polys = pTrack_spec->blends[column_z2][column_x2];
                    if (blended_polys) {
                        blended_polys->render_style = BR_RSTYLE_FACES;
                        BrZbSceneRenderAdd(blended_polys);
                        blended_polys->render_style = BR_RSTYLE_NONE;
                    }
                } else {
                    if (pTrack_spec->columns[column_z2][column_x2]) {
                        BrZbSceneRenderAdd(pTrack_spec->columns[column_z2][column_x2]);
                    }
                    if (pTrack_spec->lollipops[column_z2][column_x2]) {
                        maa.a = pTrack_spec->lollipops[column_z2][column_x2];
                        BrActorEnum(pTrack_spec->lollipops[column_z2][column_x2], LollipopizeChildren, &maa);
                        BrZbSceneRenderAdd(pTrack_spec->lollipops[column_z2][column_x2]);
                    }
                }
            }
        }
    }
}

// IDA: void __usercall RenderTrack(br_actor *pWorld@<EAX>, tTrack_spec *pTrack_spec@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>, int pRender_blends)
// FUNCTION: CARM95 0x004a944a
void RenderTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world, int pRender_blends) {
    // GLOBAL: CARM95 0x53e4ac
    static tU8 column_x;
    // GLOBAL: CARM95 0x53e490
    static tU8 column_z;
    // GLOBAL: CARM95 0x53e48c
    static tU8 min_x;
    // GLOBAL: CARM95 0x53e480
    static tU8 max_x;
    // GLOBAL: CARM95 0x53e494
    static tU8 min_z;
    // GLOBAL: CARM95 0x53e484
    static tU8 max_z;
    // GLOBAL: CARM95 0x53e470
    static br_vector3 edge_before;
    // GLOBAL: CARM95 0x53e4a0
    static br_vector3 edge_after;
    // GLOBAL: CARM95 0x53e498
    static br_camera* camera;
    // GLOBAL: CARM95 0x53e49c
    static br_scalar tan_fov_ish;
    static br_actor* result;

    if (pTrack_spec->columns != NULL) {
        if (pRender_blends) {
            DrawColumns(1, pTrack_spec, min_x, max_x, min_z, max_z, pCamera_to_world);
        } else {
            camera = (br_camera*)pCamera->type_data;
            XZToColumnXZ(&column_x, &column_z, pCamera_to_world->m[3][0], pCamera_to_world->m[3][2], pTrack_spec);
            min_x = column_x;
            max_x = column_x;
            min_z = column_z;
            max_z = column_z;
            tan_fov_ish = sin(BrAngleToRadian(camera->field_of_view / 2)) / cos(BrAngleToRadian(camera->field_of_view / 2));
            edge_after.v[0] = camera->aspect * tan_fov_ish;
            edge_after.v[1] = tan_fov_ish;
            edge_after.v[2] = -1.0;
            edge_before.v[0] = camera->yon_z * gYon_factor * edge_after.v[0];
            edge_before.v[1] = camera->yon_z * gYon_factor * tan_fov_ish;
            edge_before.v[2] = camera->yon_z * gYon_factor * -1.0;
            BrMatrix34ApplyV(&edge_after, &edge_before, pCamera_to_world);
            XZToColumnXZ(&column_x, &column_z, pCamera_to_world->m[3][0] + edge_after.v[0], pCamera_to_world->m[3][2] + edge_after.v[2], pTrack_spec);
            if (column_x < min_x) {
                min_x = column_x;
            } else if (column_x > max_x) {
                max_x = column_x;
            }
            if (column_z < min_z) {
                min_z = column_z;
            } else if (column_z > max_z) {
                max_z = column_z;
            }
            edge_before.v[0] = -edge_before.v[0];
            BrMatrix34ApplyV(&edge_after, &edge_before, pCamera_to_world);
            XZToColumnXZ(&column_x, &column_z, pCamera_to_world->m[3][0] + edge_after.v[0], pCamera_to_world->m[3][2] + edge_after.v[2], pTrack_spec);
            if (column_x < min_x) {
                min_x = column_x;
            } else if (column_x > max_x) {
                max_x = column_x;
            }
            if (column_z >= min_z) {
                if (column_z > max_z) {
                    max_z = column_z;
                }
            } else {
                min_z = column_z;
            }
            edge_before.v[1] = -edge_before.v[1];
            BrMatrix34ApplyV(&edge_after, &edge_before, pCamera_to_world);
            XZToColumnXZ(&column_x, &column_z, pCamera_to_world->m[3][0] + edge_after.v[0], pCamera_to_world->m[3][2] + edge_after.v[2], pTrack_spec);
            if (column_x < min_x) {
                min_x = column_x;
            } else if (column_x > max_x) {
                max_x = column_x;
            }
            if (column_z < min_z) {
                min_z = column_z;
            } else if (column_z > max_z) {
                max_z = column_z;
            }
            edge_before.v[0] = -edge_before.v[0];
            BrMatrix34ApplyV(&edge_after, &edge_before, pCamera_to_world);
            XZToColumnXZ(&column_x, &column_z, pCamera_to_world->m[3][0] + edge_after.v[0], pCamera_to_world->m[3][2] + edge_after.v[2], pTrack_spec);
            if (column_x < min_x) {
                min_x = column_x;
            } else if (column_x > max_x) {
                max_x = column_x;
            }
            if (column_z < min_z) {
                min_z = column_z;
            } else if (column_z > max_z) {
                max_z = column_z;
            }
            if (min_x != 0) {
                min_x--;
            }
            if (pTrack_spec->ncolumns_x - 1 > max_x) {
                max_x++;
            }
            if (min_z != 0) {
                min_z--;
            }
            if (pTrack_spec->ncolumns_z - 1 > max_z) {
                max_z++;
            }
            DrawColumns(0, pTrack_spec, min_x, max_x, min_z, max_z, pCamera_to_world);
        }
    } else {
        BrZbSceneRenderAdd(pWorld);
    }
}

// IDA: br_scalar __cdecl GetYonFactor()
// FUNCTION: CARM95 0x004a9e6e
br_scalar GetYonFactor(void) {

    return gYon_factor;
}

// IDA: void __cdecl SetYonFactor(br_scalar pNew)
// FUNCTION: CARM95 0x004a9e84
void SetYonFactor(br_scalar pNew) {

    gYon_factor = pNew;
}
