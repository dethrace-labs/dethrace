#include "skidmark.h"
#include "brender.h"
#include "common/globvars.h"
#include "common/globvrbm.h"
#include "common/loading.h"
#include <stdlib.h>

char* gMaterial_names[2] = { "OILSMEAR.MAT", "GIBSMEAR.MAT" };
char* gBoring_material_names[2] = { "OILSMEAR.MAT", "ROBSMEAR.MAT" };
tSkid gSkids[100];

// Offset: 0
// Size: 600
void StretchMark(tSkid* pMark, br_vector3* pFrom, br_vector3* pTo, br_scalar pTexture_start) {
    br_vector3 temp;
    br_vector3* rows;
    br_scalar len;
    br_model* model;
    NOT_IMPLEMENTED();
}

// Offset: 600
// Size: 81
br_material* MaterialFromIndex(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 684
// Size: 116
void AdjustSkid(int pSkid_num, br_matrix34* pMatrix, int pMaterial_index) {
    NOT_IMPLEMENTED();
}

// Offset: 800
// Size: 182
int FarFromLine2D(br_vector3* pPt, br_vector3* pL1, br_vector3* pL2) {
    br_vector2 line;
    br_vector2 to_pt;
    br_scalar line_len;
    br_scalar cross;
    NOT_IMPLEMENTED();
}

// Offset: 984
// Size: 137
int Reflex2D(br_vector3* pPt, br_vector3* pL1, br_vector3* pL2) {
    br_vector2 line;
    br_vector2 to_pt;
    NOT_IMPLEMENTED();
}

// Offset: 1124
// Size: 885
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

// Offset: 2012
// Size: 51
void HideSkid(int pSkid_num) {
    NOT_IMPLEMENTED();
}

// Offset: 2064
// Size: 67
void HideSkids() {
    NOT_IMPLEMENTED();
}

// Offset: 2132
// Size: 136
br_scalar SkidLen(int pSkid) {
    NOT_IMPLEMENTED();
}

// Offset: 2268
// Size: 949
void SkidSection(tCar_spec* pCar, int pWheel_num, br_vector3* pPos, int pMaterial_index) {
    static tU16 skid;
    br_material* material;
    NOT_IMPLEMENTED();
}

// Offset: 3220
// Size: 1175
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
    NOT_IMPLEMENTED();
}

// Offset: 4396
// Size: 130
void InitCarSkidStuff(tCar_spec* pCar) {
    int wheel;
    NOT_IMPLEMENTED();
}

// Offset: 4528
// Size: 121
void SkidsPerFrame() {
    NOT_IMPLEMENTED();
}

// Offset: 4652
// Size: 76
void RemoveMaterialsFromSkidmarks() {
    NOT_IMPLEMENTED();
}
