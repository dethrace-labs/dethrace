#include "skidmark.h"
#include "brender/brender.h"
#include "common/globvars.h"
#include "common/globvrbm.h"
#include "common/loading.h"
#include <stdlib.h>

char* gBoring_material_names[2] = { "OILSMEAR.MAT", "ROBSMEAR.MAT" };
char* gMaterial_names[2] = { "OILSMEAR.MAT", "GIBSMEAR.MAT" };
tSkid gSkids[100];

// IDA: void __usercall StretchMark(tSkid *pMark@<EAX>, br_vector3 *pFrom@<EDX>, br_vector3 *pTo@<EBX>, br_scalar pTexture_start)
void StretchMark(tSkid* pMark, br_vector3* pFrom, br_vector3* pTo, br_scalar pTexture_start) {
    br_vector3 temp;
    br_vector3* rows;
    br_scalar len;
    br_model* model;
    LOG_TRACE("(%p, %p, %p, %f)", pMark, pFrom, pTo, pTexture_start);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall MaterialFromIndex@<EAX>(int pIndex@<EAX>)
br_material* MaterialFromIndex(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AdjustSkid(int pSkid_num@<EAX>, br_matrix34 *pMatrix@<EDX>, int pMaterial_index@<EBX>)
void AdjustSkid(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    LOG_TRACE("(%d, %p, %d)", pSkid_num, pMatrix, pMaterial_index);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FarFromLine2D@<EAX>(br_vector3 *pPt@<EAX>, br_vector3 *pL1@<EDX>, br_vector3 *pL2@<EBX>)
int FarFromLine2D(br_vector3* pPt, br_vector3* pL1, br_vector3* pL2) {
    br_vector2 line;
    br_vector2 to_pt;
    br_scalar line_len;
    br_scalar cross;
    LOG_TRACE("(%p, %p, %p)", pPt, pL1, pL2);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall Reflex2D@<EAX>(br_vector3 *pPt@<EAX>, br_vector3 *pL1@<EDX>, br_vector3 *pL2@<EBX>)
int Reflex2D(br_vector3* pPt, br_vector3* pL1, br_vector3* pL2) {
    br_vector2 line;
    br_vector2 to_pt;
    LOG_TRACE("(%p, %p, %p)", pPt, pL1, pL2);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitSkids()
void InitSkids() {
    int skid;
    int mat;
    int sl;
    br_model* square;
    char* str;
    LOG_TRACE("()");

    for (mat = 0; mat < 2; mat++) {
        if (gProgram_state.sausage_eater_mode) {
            str = gBoring_material_names[mat];
        } else {
            str = gMaterial_names[mat];
        }
        gMaterial[mat] = BrMaterialFind(str);
        if (!gMaterial[mat]) {
            if (gProgram_state.sausage_eater_mode) {
                str = gBoring_material_names[mat];
            } else {
                str = gMaterial_names[mat];
            }

            sl = strlen(strtok(str, "."));
            strcpy(str + sl, ".PIX");
            BrMapAdd(LoadPixelmap(str));
            strcpy(str + sl, ".MAT");
            gMaterial[mat] = LoadMaterial(str);
            if (gMaterial[mat]) {
                BrMaterialAdd(gMaterial[mat]);
            } else {
                BrFatal("..\\..\\source\\common\\skidmark.c", 207, "Couldn't find %s", gMaterial_names[mat]);
            }
        }
    }

    for (skid = 0; skid < 100; skid++) {
        gSkids[skid].actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        BrActorAdd(gNon_track_actor, gSkids[skid].actor);
        gSkids[skid].actor->t.t.mat.m[1][1] = 0.0099999998;
        gSkids[skid].actor->render_style = 1;
        square = BrModelAllocate(NULL, 4, 2);
        square->vertices[0].p.v[0] = -0.5;
        square->vertices[0].p.v[1] = 1.0;
        square->vertices[0].p.v[2] = -0.5;
        square->vertices[1].p.v[0] = -0.5;
        square->vertices[1].p.v[1] = 1.0;
        square->vertices[1].p.v[2] = 0.5;
        square->vertices[2].p.v[0] = 0.5;
        square->vertices[2].p.v[1] = 1.0;
        square->vertices[2].p.v[2] = 0.5;
        square->vertices[3].p.v[0] = 0.5;
        square->vertices[3].p.v[1] = 1.0;
        square->vertices[3].p.v[2] = -0.5;
        square->vertices[0].map.v[0] = 0.0;
        square->vertices[0].map.v[1] = 0.0;
        square->vertices[1].map.v[0] = 0.0;
        square->vertices[1].map.v[1] = 1.0;
        square->vertices[2].map.v[0] = 1.0;
        square->vertices[2].map.v[1] = 1.0;
        square->vertices[3].map.v[0] = 1.0;
        square->vertices[3].map.v[1] = 0.0;
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
void HideSkid(int pSkid_num) {
    LOG_TRACE("(%d)", pSkid_num);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl HideSkids()
void HideSkids() {
    int skid;
    LOG_TRACE("()");

    STUB();
}

// IDA: br_scalar __usercall SkidLen@<ST0>(int pSkid@<EAX>)
br_scalar SkidLen(int pSkid) {
    LOG_TRACE("(%d)", pSkid);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SkidSection(tCar_spec *pCar@<EAX>, int pWheel_num@<EDX>, br_vector3 *pPos@<EBX>, int pMaterial_index@<ECX>)
void SkidSection(tCar_spec* pCar, int pWheel_num, br_vector3* pPos, int pMaterial_index) {
    static tU16 skid;
    br_material* material;
    LOG_TRACE("(%p, %d, %p, %d)", pCar, pWheel_num, pPos, pMaterial_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SkidMark(tCar_spec *pCar@<EAX>, int pWheel_num@<EDX>)
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

    STUB_ONCE();
}

// IDA: void __usercall InitCarSkidStuff(tCar_spec *pCar@<EAX>)
void InitCarSkidStuff(tCar_spec* pCar) {
    int wheel;
    LOG_TRACE("(%p)", pCar);
    STUB();
}

// IDA: void __cdecl SkidsPerFrame()
void SkidsPerFrame() {
    int skid;
    LOG_TRACE("()");
    STUB_ONCE();
}

// IDA: void __cdecl RemoveMaterialsFromSkidmarks()
void RemoveMaterialsFromSkidmarks() {
    int skid;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
