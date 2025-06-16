#include "skidmark.h"
#include "brender.h"
#include "globvars.h"
#include "globvrbm.h"
#include "harness/trace.h"
#include "loading.h"
#include "oil.h"
#include "piping.h"
#include "utility.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

char* gBoring_material_names[2] = { "OILSMEAR.MAT", "ROBSMEAR.MAT" };
char* gMaterial_names[2] = { "OILSMEAR.MAT", "GIBSMEAR.MAT" };
tSkid gSkids[100];

// IDA: void __usercall StretchMark(tSkid *pMark@<EAX>, br_vector3 *pFrom@<EDX>, br_vector3 *pTo@<EBX>, br_scalar pTexture_start)
// FUNCTION: CARM95 0x401e7c
void StretchMark(tSkid* pMark, br_vector3* pFrom, br_vector3* pTo, br_scalar pTexture_start) {
    br_vector3 temp;
    br_vector3* rows;
    br_scalar len;
    br_model* model;
    LOG_TRACE("(%p, %p, %p, %f)", pMark, pFrom, pTo, pTexture_start);

    rows = (br_vector3*)&pMark->actor->t.t.mat;
    BrVector3Sub(&temp, pTo, pFrom);
    len = BrVector3Length(&temp);

    rows[2].v[0] = pMark->normal.v[2] * temp.v[1] - pMark->normal.v[1] * temp.v[2];
    rows[2].v[1] = pMark->normal.v[0] * temp.v[2] - pMark->normal.v[2] * temp.v[0];
    rows[2].v[2] = pMark->normal.v[1] * temp.v[0] - pMark->normal.v[0] * temp.v[1];

    if (len > BR_SCALAR_EPSILON) {
        rows[2].v[0] = 0.05f / len * rows[2].v[0];
        rows[2].v[1] = 0.05f / len * rows[2].v[1];
        rows[2].v[2] = 0.05f / len * rows[2].v[2];
        rows->v[0] = len / len * temp.v[0];
        rows->v[1] = len / len * temp.v[1];
        rows->v[2] = len / len * temp.v[2];
        BrVector3Add(&temp, pTo, pFrom);
        BrVector3Scale(&pMark->pos, &temp, 0.5f);
        rows[3] = pMark->pos;
        model = pMark->actor->model;
        model->vertices[1].map.v[0] = pTexture_start / 0.05f;
        model->vertices[0].map.v[0] = model->vertices[1].map.v[0];
        model->vertices[3].map.v[0] = (pTexture_start + len) / 0.05f;
        model->vertices[2].map.v[0] = model->vertices[3].map.v[0];
        BrModelUpdate(model, BR_MODU_ALL);
    }
}

// IDA: br_material* __usercall MaterialFromIndex@<EAX>(int pIndex@<EAX>)
// FUNCTION: CARM95 0x401088
br_material* MaterialFromIndex(int pIndex) {
    LOG_TRACE("(%d)", pIndex);

    if (pIndex > -2) {
        return gCurrent_race.material_modifiers[pIndex].skid_mark_material;
    } else {
        return gMaterial[-2 - pIndex];
    }
}

// IDA: void __usercall AdjustSkid(int pSkid_num@<EAX>, br_matrix34 *pMatrix@<EDX>, int pMaterial_index@<EBX>)
// FUNCTION: CARM95 0x401000
void AdjustSkid(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    LOG_TRACE("(%d, %p, %d)", pSkid_num, pMatrix, pMaterial_index);

    gSkids[pSkid_num].actor->t.t.mat = *pMatrix;
    gSkids[pSkid_num].pos.v[0] = pMatrix->m[3][0];
    gSkids[pSkid_num].pos.v[1] = pMatrix->m[3][1];
    gSkids[pSkid_num].pos.v[2] = pMatrix->m[3][2];
    gSkids[pSkid_num].actor->material = MaterialFromIndex(pMaterial_index);
    gSkids[pSkid_num].actor->render_style = BR_RSTYLE_DEFAULT;
}

// IDA: int __usercall FarFromLine2D@<EAX>(br_vector3 *pPt@<EAX>, br_vector3 *pL1@<EDX>, br_vector3 *pL2@<EBX>)
// FUNCTION: CARM95 0x4020dc
int FarFromLine2D(br_vector3* pPt, br_vector3* pL1, br_vector3* pL2) {
    br_vector2 line;
    br_vector2 to_pt;
    br_scalar line_len;
    br_scalar cross;
    LOG_TRACE("(%p, %p, %p)", pPt, pL1, pL2);

    line.v[0] = pL2->v[0] - pL1->v[0];
    line.v[1] = pL2->v[2] - pL1->v[2];
    to_pt.v[0] = pPt->v[0] - pL2->v[0];
    to_pt.v[1] = pPt->v[2] - pL2->v[2];
    cross = -line.v[0] * to_pt.v[1] + to_pt.v[0] * line.v[1];
    line_len = sqrtf(line.v[0] * line.v[0] + line.v[1] * line.v[1]);
    return fabs(cross) > line_len * 0.050000001;
}

// IDA: int __usercall Reflex2D@<EAX>(br_vector3 *pPt@<EAX>, br_vector3 *pL1@<EDX>, br_vector3 *pL2@<EBX>)
// FUNCTION: CARM95 0x402179
int Reflex2D(br_vector3* pPt, br_vector3* pL1, br_vector3* pL2) {
    br_vector2 line;
    br_vector2 to_pt;
    LOG_TRACE("(%p, %p, %p)", pPt, pL1, pL2);

    line.v[0] = pL2->v[0] - pL1->v[0];
    line.v[1] = pL2->v[2] - pL1->v[2];
    to_pt.v[0] = pPt->v[0] - pL2->v[0];
    to_pt.v[1] = pPt->v[2] - pL2->v[2];
    return to_pt.v[1] * line.v[1] + to_pt.v[0] * line.v[0] < 0.0;
}

// IDA: void __cdecl InitSkids()
// FUNCTION: CARM95 0x4010c8
void InitSkids(void) {
    int skid;
    int mat;
    int sl;
    br_model* square;
    char* str;
#if defined(DETHRACE_FIX_BUGS)
    char mat_name[32];
#endif
    LOG_TRACE("()");

    for (mat = 0; mat < COUNT_OF(gMaterial_names); mat++) {
        if (gProgram_state.sausage_eater_mode) {
            str = gBoring_material_names[mat];
        } else {
            str = gMaterial_names[mat];
        }
        gMaterial[mat] = BrMaterialFind(str);
        if (gMaterial[mat] == NULL) {
            if (gProgram_state.sausage_eater_mode) {
                str = gBoring_material_names[mat];
            } else {
                str = gMaterial_names[mat];
            }

#if defined(DETHRACE_FIX_BUGS)
            // Avoid modification of read-only data by strtok.
            strcpy(mat_name, str);
            str = mat_name;
#endif
            sl = strlen(strtok(str, "."));
            strcpy(str + sl, ".PIX");
            BrMapAdd(LoadPixelmap(str));
            strcpy(str + sl, ".MAT");
            gMaterial[mat] = LoadMaterial(str);
            if (gMaterial[mat] == NULL) {
                BrFatal("..\\..\\source\\common\\skidmark.c", 207, "Couldn't find %s", gMaterial_names[mat]);
            }
#ifdef DETHRACE_3DFX_PATCH
            GlorifyMaterial(&gMaterial[mat], 1);
#endif
            BrMaterialAdd(gMaterial[mat]);
        }
#ifdef DETHRACE_3DFX_PATCH
        else {

            BrMapRemove(gMaterial[mat]->colour_map);
            gMaterial[mat]->colour_map = PurifiedPixelmap(gMaterial[mat]->colour_map);
            BrMapAdd(gMaterial[mat]->colour_map);
            GlorifyMaterial(&gMaterial[mat], 1);
            BrMaterialUpdate(gMaterial[mat], BR_MATU_ALL);
        }
#endif
    }

    for (skid = 0; skid < COUNT_OF(gSkids); skid++) {
        gSkids[skid].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        BrActorAdd(gNon_track_actor, gSkids[skid].actor);
        gSkids[skid].actor->t.t.mat.m[1][1] = 0.01f;
        gSkids[skid].actor->render_style = BR_RSTYLE_NONE;
        square = BrModelAllocate(NULL, 4, 2);
        BrVector3Set(&square->vertices[0].p, -0.5f, 1.0f, -0.5f);
        BrVector3Set(&square->vertices[1].p, -0.5f, 1.0f, 0.5f);
        BrVector3Set(&square->vertices[2].p, 0.5f, 1.0f, 0.5f);
        BrVector3Set(&square->vertices[3].p, 0.5f, 1.0f, -0.5f);
        BrVector2Set(&square->vertices[0].map, 0.0f, 0.0f);
        BrVector2Set(&square->vertices[1].map, 0.0f, 1.0f);
        BrVector2Set(&square->vertices[2].map, 1.0f, 1.0f);
        BrVector2Set(&square->vertices[3].map, 1.0f, 0.0f);
        square->faces[0].vertices[0] = 0;
        square->faces[0].vertices[1] = 1;
        square->faces[0].vertices[2] = 2;
        square->faces[0].smoothing = 1;
        square->faces[1].vertices[0] = 0;
        square->faces[1].vertices[1] = 2;
        square->faces[1].vertices[2] = 3;
        square->faces[1].smoothing = 1;
        square->flags |= BR_MODF_KEEP_ORIGINAL;
        BrModelAdd(square);
        gSkids[skid].actor->model = square;
    }
}

// IDA: void __usercall HideSkid(int pSkid_num@<EAX>)
// FUNCTION: CARM95 0x40148d
void HideSkid(int pSkid_num) {
    LOG_TRACE("(%d)", pSkid_num);

    gSkids[pSkid_num].actor->render_style = BR_RSTYLE_NONE;
}

// IDA: void __cdecl HideSkids()
// FUNCTION: CARM95 0x4014ad
void HideSkids(void) {
    int skid;
    LOG_TRACE("()");

    for (skid = 0; skid < COUNT_OF(gSkids); skid++) {
        HideSkid(skid);
    }
}

// IDA: br_scalar __usercall SkidLen@<ST0>(int pSkid@<EAX>)
// FUNCTION: CARM95 0x4021f1
br_scalar SkidLen(int pSkid) {
    LOG_TRACE("(%d)", pSkid);

    return sqrtf(
        gSkids[pSkid].actor->t.t.mat.m[0][2] * gSkids[pSkid].actor->t.t.mat.m[0][2]
        + gSkids[pSkid].actor->t.t.mat.m[0][1] * gSkids[pSkid].actor->t.t.mat.m[0][1]
        + gSkids[pSkid].actor->t.t.mat.m[0][0] * gSkids[pSkid].actor->t.t.mat.m[0][0]);
}

// IDA: void __usercall SkidSection(tCar_spec *pCar@<EAX>, int pWheel_num@<EDX>, br_vector3 *pPos@<EBX>, int pMaterial_index@<ECX>)
// FUNCTION: CARM95 0x401a22
void SkidSection(tCar_spec* pCar, int pWheel_num, br_vector3* pPos, int pMaterial_index) {
    static tU16 skid;
    br_material* material;
    LOG_TRACE("(%p, %d, %p, %d)", pCar, pWheel_num, pPos, pMaterial_index);

    if (BrVector3Dot(&pCar->prev_nor[pWheel_num], &pCar->nor[pWheel_num]) < 0.99699998
        || fabs(BrVector3Dot(&pCar->nor[pWheel_num], pPos) - BrVector3Dot(&pCar->prev_skid_pos[pWheel_num], &pCar->nor[pWheel_num])) > 0.0099999998) {
        pCar->old_skidding &= ~(1 << pWheel_num);
        pCar->old_skid[pWheel_num] = -1;
        return;
    }

    material = MaterialFromIndex(pMaterial_index);
    if (pCar->old_skid[pWheel_num] >= COUNT_OF(gSkids)
        || gSkids[pCar->old_skid[pWheel_num]].actor->material != material
        || SkidLen(pCar->old_skid[pWheel_num]) > 0.5
        || FarFromLine2D(pPos, &pCar->skid_line_start[pWheel_num], &pCar->skid_line_end[pWheel_num])
        || Reflex2D(pPos, &pCar->skid_line_start[pWheel_num], &pCar->prev_skid_pos[pWheel_num])) {

        pCar->skid_line_start[pWheel_num] = pCar->prev_skid_pos[pWheel_num];
        pCar->skid_line_end[pWheel_num] = *pPos;
        gSkids[skid].actor->render_style = BR_RSTYLE_DEFAULT;
        gSkids[skid].actor->material = material;
        gSkids[skid].normal = pCar->nor[pWheel_num];
        StretchMark(&gSkids[skid], &pCar->prev_skid_pos[pWheel_num], pPos, pCar->total_length[pWheel_num]);
        PipeSingleSkidAdjustment(skid, &gSkids[skid].actor->t.t.mat, pMaterial_index);
        pCar->old_skid[pWheel_num] = skid;
        skid = (skid + 1) % COUNT_OF(gSkids);
    } else {
        StretchMark(&gSkids[pCar->old_skid[pWheel_num]], &pCar->skid_line_start[pWheel_num], pPos, pCar->total_length[pWheel_num]);
        PipeSingleSkidAdjustment(pCar->old_skid[pWheel_num], &gSkids[pCar->old_skid[pWheel_num]].actor->t.t.mat, pMaterial_index);
    }
}

// IDA: void __usercall SkidMark(tCar_spec *pCar@<EAX>, int pWheel_num@<EDX>)
// FUNCTION: CARM95 0x4014e5
void SkidMark(tCar_spec* pCar, int pWheel_num) {
    br_vector3 pos;
    br_vector3 world_pos;
    br_vector3 disp;
    br_vector3 spesh_to_wheel;
    int material_index;
    br_scalar dist;
    br_scalar dist2;
    int on_ground;
    br_material* material;
    LOG_TRACE("(%p, %d)", pCar, pWheel_num);

    on_ground = pCar->susp_height[pWheel_num >> 1] > pCar->oldd[pWheel_num];
    if (!on_ground) {
        pCar->special_start[pWheel_num].v[0] = FLT_MAX;
    }
    if (pCar->blood_remaining[pWheel_num] != 0 && on_ground) {
        pCar->new_skidding |= 1 << pWheel_num;
        material_index = -3;
    } else if (pCar->oil_remaining[pWheel_num] != 0 && on_ground) {
        pCar->new_skidding |= 1 << pWheel_num;
        material_index = -2;
    } else {
        material_index = pCar->material_index[pWheel_num];
        if (!gCurrent_race.material_modifiers[material_index].skid_mark_material) {
            pCar->old_skidding &= ~(1 << pWheel_num);
            return;
        }
    }

    if (((1 << pWheel_num) & pCar->new_skidding) != 0 || ((1 << pWheel_num) & pCar->old_skidding) != 0) {
        if ((pWheel_num & 1) != 0) {
            pos.v[0] = pCar->bounds[1].max.v[0] - 0.1725f;
        } else {
            pos.v[0] = pCar->bounds[1].min.v[0] + 0.1725f;
        }
        pos.v[1] = pCar->wpos[pWheel_num].v[1] - pCar->oldd[pWheel_num];
        pos.v[2] = pCar->wpos[pWheel_num].v[2];
        BrMatrix34ApplyP(&world_pos, &pos, &pCar->car_master_actor->t.t.mat);
        BrVector3InvScale(&world_pos, &world_pos, WORLD_SCALE);
        if (pCar->special_start[pWheel_num].v[0] != FLT_MAX) {

            BrVector3Sub(&spesh_to_wheel, &world_pos, &pCar->special_start[pWheel_num]);
            dist = BrVector3Length(&spesh_to_wheel);
            if (dist <= BR_SCALAR_EPSILON || (BrVector3Dot(&pCar->direction, &spesh_to_wheel) / dist < 0.70700002)) {
                return;
            }
            world_pos = pCar->special_start[pWheel_num];
            pCar->special_start[pWheel_num].v[0] = FLT_MAX;
        }
        if (((1 << pWheel_num) & pCar->new_skidding) != 0) {
            if (((1 << pWheel_num) & pCar->old_skidding) != 0) {
                BrVector3Sub(&disp, &world_pos, &pCar->prev_skid_pos[pWheel_num]);
                dist2 = BrVector3Length(&disp);
                if (dist2 < 0.05f) {
                    return;
                }
                SkidSection(pCar, pWheel_num, &world_pos, material_index);
                pCar->total_length[pWheel_num] = pCar->total_length[pWheel_num] + dist2;
                pCar->oil_remaining[pWheel_num] = pCar->oil_remaining[pWheel_num] - dist2;
                if (pCar->oil_remaining[pWheel_num] < 0.0f) {
                    pCar->oil_remaining[pWheel_num] = 0.0f;
                }
                pCar->blood_remaining[pWheel_num] = pCar->blood_remaining[pWheel_num] - dist2;
                if (pCar->blood_remaining[pWheel_num] < 0.0f) {
                    pCar->blood_remaining[pWheel_num] = 0.0f;
                }
            } else {
                pCar->old_skidding |= 1 << pWheel_num;
                pCar->total_length[pWheel_num] = 0.0f;
                pCar->old_skid[pWheel_num] = -1;
            }
        } else {
            pCar->old_skidding &= ~(1 << pWheel_num);
        }
        pCar->prev_skid_pos[pWheel_num] = world_pos;
        pCar->prev_nor[pWheel_num] = pCar->nor[pWheel_num];
    }
}

// IDA: void __usercall InitCarSkidStuff(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x402282
void InitCarSkidStuff(tCar_spec* pCar) {
    int wheel;
    LOG_TRACE("(%p)", pCar);

    pCar->old_skidding = 0;
    for (wheel = 0; wheel < 4; wheel++) {
        pCar->special_start[wheel].v[0] = FLT_MAX;
        pCar->blood_remaining[wheel] = 0.0f;
        pCar->oil_remaining[wheel] = 0.0f;
    }
}

// IDA: void __cdecl SkidsPerFrame()
// FUNCTION: CARM95 0x4022f1
void SkidsPerFrame(void) {
    int skid;
    LOG_TRACE("()");

    for (skid = 0; skid < COUNT_OF(gSkids); skid++) {
        if (gSkids[skid].actor->render_style != BR_RSTYLE_NONE) {
            EnsureGroundDetailVisible(&gSkids[skid].actor->t.t.translate.t, &gSkids[skid].normal, &gSkids[skid].pos);
        }
    }
}

// IDA: void __cdecl RemoveMaterialsFromSkidmarks()
void RemoveMaterialsFromSkidmarks(void) {
    int skid;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
