#include "world.h"
#include <stdlib.h>

#include "brender/brender.h"
#include "brucetrk.h"
#include "car.h"
#include "depth.h"
#include "displays.h"
#include "drmem.h"
#include "errors.h"
#include "finteray.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/trace.h"
#include "input.h"
#include "loading.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "replay.h"
#include "spark.h"
#include "trig.h"
#include "utility.h"

#include <float.h>
#include <string.h>

int gFunkotronics_array_size;
int gGroovidelics_array_size;
int gSpec_vol_mode;
tFunkotronic_spec* gFunkotronics_array;
tGroovidelic_spec* gGroovidelics_array;
char* gFunk_type_names[] = { "spin", "rock", "throb", "slither", "roll" };
char* gFunk_anim_names[] = { "frames", "flic" };
char* gTime_mode_names[] = { "approximate", "accurate" };
char* gFunk_move_names[] = { "linear", "harmonic", "flash", "controlled", "absolute", "continuous" };
char* gFunk_nature_names[] = { "constant", "distance", "lastlap", "otherlaps" };
char* gGroove_nature_names[] = { "constant", "distance" };
char* gAxis_names[] = { "x", "y", "z" };
char* gLollipop_names[] = { "xlollipop", "ylollipop", "zlollipop" };
char* gGroove_path_names[] = { "straight", "circular" };
char* gGroove_object_names[] = { "spin", "rock", "throb", "shear" };
char* gDepth_effect_names[] = { "dark", "fog" };
br_actor* gGroove_by_proxy_actor;
tRotate_mode gCurrent_rotate_mode = eRotate_mode_y;
tScale_mode gCurrent_scale_mode = eScale_mode_all;
int gNumber_of_additional_models;
tRoad_texturing_level gRoad_texturing_level = eRTL_full;
tWall_texturing_level gWall_texturing_level = eWTL_full;
tCar_texturing_level gCar_texturing_level = eCTL_full;
int gRendering_accessories;
int gNumber_of_actors;
int gNumber_of_lights;
br_actor* gActor_array[500];
float* gGroove_funk_bindings[960];
br_actor* gDelete_list[500];
br_actor* gLight_array[50];
br_model* gAdditional_models[1000];
br_actor* gSpec_vol_actors[100];
tPath_name gAdditional_actor_path;
tPath_name gAdditional_model_path;
tU32 gPrevious_groove_times[2];
int gRace_file_version;
br_vector3 gActor_centre;
float gTemp;
br_actor* gLast_actor;
br_actor* gKnown_actor;
br_actor* gAdditional_actors;
int gDelete_count;
br_scalar gNearest_distance;
br_actor* gNearest_actor;
br_actor* gStandard_lamp;
br_scalar gSight_distance_squared;

// IDA: float __cdecl MapSawToTriangle(float pNumber)
float MapSawToTriangle(float pNumber) {
    LOG_TRACE("(%f)", pNumber);

    if (pNumber >= 0.5) {
        return 3.0 - pNumber * 4.0;
    } else {
        return pNumber * 4.0 - 1.0;
    }
}

// IDA: void __cdecl SetSightDistance(br_scalar pYon)
void SetSightDistance(br_scalar pYon) {
    gSight_distance_squared = pYon * 1.02f * (pYon * 1.02f);
}

// IDA: br_actor* __usercall FindActorInArray@<EAX>(char *pThe_name@<EAX>)
br_actor* FindActorInArray(char* pThe_name) {
    int i;
    LOG_TRACE("(\"%s\")", pThe_name);

    for (i = 0; i < gNumber_of_actors; i++) {
        if (strcmp(gActor_array[i]->identifier, pThe_name) == 0) {
            return gActor_array[i];
        }
    }
    return NULL;
}

// IDA: br_actor* __usercall FindLightInArray@<EAX>(char *pThe_name@<EAX>)
br_actor* FindLightInArray(char* pThe_name) {
    int i;
    LOG_TRACE("(\"%s\")", pThe_name);

    for (i = 0; i < gNumber_of_lights; i++) {
        if (strcmp(gLight_array[i]->identifier, pThe_name) == 0) {
            return gLight_array[i];
        }
    }
    return NULL;
}

// IDA: br_actor* __usercall CloneActor@<EAX>(br_actor *pSource_actor@<EAX>)
br_actor* CloneActor(br_actor* pSource_actor) {
    br_actor* new_actor;
    br_actor* child_actor;
    br_actor* new_child_actor;
    LOG_TRACE("(%p)", pSource_actor);

    new_actor = BrActorAllocate(pSource_actor->type, pSource_actor->type_data);
    new_actor->model = pSource_actor->model;
    new_actor->material = pSource_actor->material;
    if (pSource_actor->identifier != NULL) {
        if (new_actor->identifier != NULL) {
            BrResFree(new_actor->identifier);
        }
        new_actor->identifier = BrResStrDup(new_actor, pSource_actor->identifier);
    }
    new_actor->t = pSource_actor->t;
    for (child_actor = pSource_actor->children; child_actor != NULL; child_actor = child_actor->next) {
        new_child_actor = CloneActor(child_actor);
        BrActorAdd(new_actor, new_child_actor);
    }
    return new_actor;
}

// IDA: void __usercall InitialiseStorageSpace(tBrender_storage *pStorage_space@<EAX>, int pMax_pixelmaps@<EDX>, int pMax_shade_tables@<EBX>, int pMax_materials@<ECX>, int pMax_models)
void InitialiseStorageSpace(tBrender_storage* pStorage_space, int pMax_pixelmaps, int pMax_shade_tables, int pMax_materials, int pMax_models) {
    LOG_TRACE("(%p, %d, %d, %d, %d)", pStorage_space, pMax_pixelmaps, pMax_shade_tables, pMax_materials, pMax_models);

    pStorage_space->pixelmaps_count = 0;
    pStorage_space->shade_tables_count = 0;
    pStorage_space->materials_count = 0;
    pStorage_space->models_count = 0;
    pStorage_space->max_pixelmaps = pMax_pixelmaps;
    pStorage_space->max_shade_tables = pMax_shade_tables;
    pStorage_space->max_materials = pMax_materials;
    pStorage_space->max_models = pMax_models;
    pStorage_space->pixelmaps = BrMemCalloc(pMax_pixelmaps, sizeof(br_pixelmap*), kMem_stor_space_pix);
    pStorage_space->shade_tables = BrMemCalloc(pMax_shade_tables, sizeof(br_pixelmap*), kMem_stor_space_tab);
    pStorage_space->materials = BrMemCalloc(pMax_materials, sizeof(br_material*), kMem_stor_space_mat);
    pStorage_space->models = BrMemCalloc(pMax_models, sizeof(br_model*), kMem_stor_space_mod);
    pStorage_space->saved_colour_maps = BrMemCalloc(pMax_materials, sizeof(br_pixelmap*), kMem_stor_space_save);
}

// IDA: void __usercall DisposeStorageSpace(tBrender_storage *pStorage_space@<EAX>)
void DisposeStorageSpace(tBrender_storage* pStorage_space) {
    BrMemFree(pStorage_space->pixelmaps);
    BrMemFree(pStorage_space->shade_tables);
    BrMemFree(pStorage_space->materials);
    BrMemFree(pStorage_space->models);
    BrMemFree(pStorage_space->saved_colour_maps);
}

// IDA: void __usercall ClearOutStorageSpace(tBrender_storage *pStorage_space@<EAX>)
void ClearOutStorageSpace(tBrender_storage* pStorage_space) {
    int i;
    LOG_TRACE("(%p)", pStorage_space);

    for (i = 0; pStorage_space->pixelmaps_count > i; ++i) {
        if (pStorage_space->pixelmaps[i] != NULL) {
            BrMapRemove(pStorage_space->pixelmaps[i]);
            BrPixelmapFree(pStorage_space->pixelmaps[i]);
        }
    }
    pStorage_space->pixelmaps_count = 0;
    for (i = 0; pStorage_space->shade_tables_count > i; ++i) {
        if (pStorage_space->shade_tables[i] != NULL) {
            BrTableRemove(pStorage_space->shade_tables[i]);
            BrPixelmapFree(pStorage_space->shade_tables[i]);
        }
    }
    pStorage_space->shade_tables_count = 0;
    for (i = 0; pStorage_space->materials_count > i; ++i) {
        if (pStorage_space->materials[i] != NULL) {
            BrMaterialRemove(pStorage_space->materials[i]);
            BrMaterialFree(pStorage_space->materials[i]);
        }
    }
    pStorage_space->materials_count = 0;
    for (i = 0; pStorage_space->models_count > i; ++i) {
        if (pStorage_space->models[i] != NULL) {
            BrModelRemove(pStorage_space->models[i]);
            BrModelFree(pStorage_space->models[i]);
        }
    }
    pStorage_space->models_count = 0;
}

// IDA: tAdd_to_storage_result __usercall AddPixelmapToStorage@<EAX>(tBrender_storage *pStorage_space@<EAX>, br_pixelmap **pThe_pm@<EDX>)
// This seems like the signature should be `br_pixelmap* pThe_pm`
tAdd_to_storage_result AddPixelmapToStorage(tBrender_storage* pStorage_space, br_pixelmap** pThe_pm) {
    int i;
    LOG_TRACE("(%p, %p)", pStorage_space, pThe_pm);

    if (pStorage_space->pixelmaps_count >= pStorage_space->max_pixelmaps) {
        return eStorage_not_enough_room;
    }

    for (i = 0; i < pStorage_space->pixelmaps_count; i++) {
        if (pStorage_space->pixelmaps[i]->identifier
            && ((br_pixelmap*)pThe_pm)->identifier
            && strcmp(pStorage_space->pixelmaps[i]->identifier, ((br_pixelmap*)pThe_pm)->identifier) == 0) {
            return eStorage_duplicate;
        }
    }
    pStorage_space->pixelmaps[pStorage_space->pixelmaps_count] = (br_pixelmap*)pThe_pm;
    pStorage_space->pixelmaps_count++;
    return eStorage_allocated;
}

// IDA: tAdd_to_storage_result __usercall AddShadeTableToStorage@<EAX>(tBrender_storage *pStorage_space@<EAX>, br_pixelmap *pThe_st@<EDX>)
tAdd_to_storage_result AddShadeTableToStorage(tBrender_storage* pStorage_space, br_pixelmap* pThe_st) {
    int i;
    LOG_TRACE("(%p, %p)", pStorage_space, pThe_st);

    if (pStorage_space->shade_tables_count >= pStorage_space->max_shade_tables) {
        return eStorage_not_enough_room;
    }

    for (i = 0; i < pStorage_space->shade_tables_count; i++) {
        if (pStorage_space->shade_tables[i]->identifier
            && pThe_st->identifier
            && !strcmp(pStorage_space->shade_tables[i]->identifier, pThe_st->identifier)) {
            return eStorage_duplicate;
        }
    }
    pStorage_space->shade_tables[pStorage_space->shade_tables_count++] = pThe_st;
    return eStorage_allocated;
}

// IDA: tAdd_to_storage_result __usercall AddMaterialToStorage@<EAX>(tBrender_storage *pStorage_space@<EAX>, br_material *pThe_mat@<EDX>)
tAdd_to_storage_result AddMaterialToStorage(tBrender_storage* pStorage_space, br_material* pThe_mat) {
    int i;
    LOG_TRACE("(%p, %p)", pStorage_space, pThe_mat);

    if (pStorage_space->materials_count >= pStorage_space->max_materials) {
        return eStorage_not_enough_room;
    }
    for (i = 0; i < pStorage_space->materials_count; i++) {
        if (pStorage_space->materials[i]->identifier
            && pThe_mat->identifier
            && !strcmp(pStorage_space->materials[i]->identifier, pThe_mat->identifier)) {
            return eStorage_duplicate;
        }
    }
    pStorage_space->saved_colour_maps[pStorage_space->materials_count] = 0;
    pStorage_space->materials[pStorage_space->materials_count++] = pThe_mat;
    return eStorage_allocated;
}

// IDA: tAdd_to_storage_result __usercall AddModelToStorage@<EAX>(tBrender_storage *pStorage_space@<EAX>, br_model *pThe_mod@<EDX>)
tAdd_to_storage_result AddModelToStorage(tBrender_storage* pStorage_space, br_model* pThe_mod) {
    int i;
    LOG_TRACE("(%p, %p)", pStorage_space, pThe_mod);

    if (pStorage_space->materials_count >= pStorage_space->max_models) {
        return eStorage_not_enough_room;
    }
    for (i = 0; i < pStorage_space->models_count; i++) {
        if (pStorage_space->models[i]
            && pStorage_space->models[i]->identifier
            && pThe_mod->identifier
            && !strcmp(pStorage_space->models[i]->identifier, pThe_mod->identifier)) {
            return eStorage_duplicate;
        }
    }
    pStorage_space->models[pStorage_space->models_count] = pThe_mod;
    pStorage_space->models_count++;
    return eStorage_allocated;
}

// IDA: int __usercall LoadNPixelmaps@<EAX>(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>, int pCount@<EBX>)
int LoadNPixelmaps(tBrender_storage* pStorage_space, FILE* pF, int pCount) {
    tPath_name the_path;
    int i;
    int j;
    int new_ones;
    int total;
    char s[256];
    char* str;
    br_pixelmap* temp_array[200];
    LOG_TRACE("(%p, %p, %d)", pStorage_space, pF, pCount);

    new_ones = 0;
    for (i = 0; i < pCount; ++i) {
        PossibleService();
        GetALineAndDontArgue(pF, s);
        str = strtok(s, "\t ,/");
        PathCat(the_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
        PathCat(the_path, the_path, "PIXELMAP");
        PathCat(the_path, the_path, str);
        AllowOpenToFail();
        total = DRPixelmapLoadMany(the_path, temp_array, COUNT_OF(temp_array));
        if (total == 0) {
            PathCat(the_path, gApplication_path, "PIXELMAP");
            PathCat(the_path, the_path, str);
            total = DRPixelmapLoadMany(the_path, temp_array, COUNT_OF(temp_array));
            if (total == 0) {
                FatalError(kFatalError_LoadPixelmapFile_S, str);
            }
        }
        for (j = 0; j < total; j++) {
            if (temp_array[j] != NULL) {
                switch (AddPixelmapToStorage(pStorage_space, (br_pixelmap**)temp_array[j])) {
                case eStorage_not_enough_room:
                    FatalError(kFatalError_InsufficientPixelmapSlots);
                    break;
                case eStorage_duplicate:
                    BrPixelmapFree(temp_array[j]);
                    break;
                case eStorage_allocated:
                    BrMapAdd(temp_array[j]);
                    ++new_ones;
                    break;
                }
            }
        }
    }
    return new_ones;
}

// IDA: br_pixelmap* __usercall LoadSinglePixelmap@<EAX>(tBrender_storage *pStorage_space@<EAX>, char *pName@<EDX>)
br_pixelmap* LoadSinglePixelmap(tBrender_storage* pStorage_space, char* pName) {
    br_pixelmap* temp;
    LOG_TRACE("(%p, \"%s\")", pStorage_space, pName);

    temp = LoadPixelmap(pName);
    if (!temp) {
        return BrMapFind(pName);
    }

    switch (AddPixelmapToStorage(pStorage_space, (br_pixelmap**)temp)) {
    case eStorage_not_enough_room:
        FatalError(kFatalError_InsufficientPixelmapSlots);
        break;

    case eStorage_duplicate:
        BrPixelmapFree(temp);
        return BrMapFind(pName);

    case eStorage_allocated:
        BrMapAdd(temp);
        return temp;
    }

    return NULL;
}

// IDA: br_material* __usercall LoadSingleMaterial@<EAX>(tBrender_storage *pStorage_space@<EAX>, char *pName@<EDX>)
br_material* LoadSingleMaterial(tBrender_storage* pStorage_space, char* pName) {
    br_material* temp;
    LOG_TRACE("(%p, \"%s\")", pStorage_space, pName);

    temp = LoadMaterial(pName);
    if (!temp) {
        return BrMaterialFind(pName);
    }

    switch (AddMaterialToStorage(pStorage_space, temp)) {
    case eStorage_not_enough_room:
        FatalError(kFatalError_InsufficientMaterialSlots);
        break;

    case eStorage_duplicate:
        BrMaterialFree(temp);
        return BrMaterialFind(pName);

    case eStorage_allocated:
        BrMaterialAdd(temp);
        return temp;
    }

    return NULL;
}

// IDA: int __usercall LoadNShadeTables@<EAX>(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>, int pCount@<EBX>)
int LoadNShadeTables(tBrender_storage* pStorage_space, FILE* pF, int pCount) {
    tPath_name the_path;
    int i;
    int j;
    int new_ones;
    int total;
    char s[256];
    char* str;
    br_pixelmap* temp_array[50];
    LOG_TRACE("(%p, %p, %d)", pStorage_space, pF, pCount);

    new_ones = 0;
    for (i = 0; i < pCount; i++) {
        PossibleService();
        GetALineAndDontArgue(pF, s);
        str = strtok(s, "\t ,/");
        PathCat(the_path, gApplication_path, "SHADETAB");
        PathCat(the_path, the_path, str);
        total = DRPixelmapLoadMany(the_path, temp_array, 50);
        if (total == 0) {
            FatalError(kFatalError_LoadShadeTableFile_S, str);
        }
        for (j = 0; j < total; j++) {
            if (temp_array[j]) {
                switch (AddShadeTableToStorage(pStorage_space, temp_array[j])) {
                case eStorage_not_enough_room:
                    FatalError(kFatalError_InsufficientShadeTableSlots);
                    break;

                case eStorage_duplicate:
                    BrPixelmapFree(temp_array[j]);
                    break;
                case eStorage_allocated:
                    BrTableAdd(temp_array[j]);
                    new_ones++;
                    break;
                }
            }
        }
    }
    return new_ones;
}

// IDA: br_pixelmap* __usercall LoadSingleShadeTable@<EAX>(tBrender_storage *pStorage_space@<EAX>, char *pName@<EDX>)
br_pixelmap* LoadSingleShadeTable(tBrender_storage* pStorage_space, char* pName) {
    br_pixelmap* temp;
    LOG_TRACE("(%p, \"%s\")", pStorage_space, pName);

    temp = LoadShadeTable(pName);
    if (!temp) {
        return BrTableFind(pName);
    }

    switch (AddShadeTableToStorage(pStorage_space, temp)) {
    case eStorage_not_enough_room:
        FatalError(kFatalError_InsufficientShadeTableSlots);
        break;

    case eStorage_duplicate:
        BrPixelmapFree(temp);
        return BrTableFind(pName);

    case eStorage_allocated:
        BrTableAdd(temp);
        return temp;
    }

    return NULL;
}

// IDA: int __usercall LoadNMaterials@<EAX>(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>, int pCount@<EBX>)
int LoadNMaterials(tBrender_storage* pStorage_space, FILE* pF, int pCount) {
    tPath_name the_path;
    int i;
    int j;
    int new_ones;
    int total;
    char s[256];
    char* str;
    br_material* temp_array[200];
    LOG_TRACE("(%p, %p, %d)", pStorage_space, pF, pCount);

    new_ones = 0;
    for (i = 0; i < pCount; ++i) {
        PossibleService();
        GetALineAndDontArgue(pF, s);
        str = strtok(s, "\t ,/");
        PathCat(the_path, gApplication_path, "MATERIAL");
        PathCat(the_path, the_path, str);
        total = BrMaterialLoadMany(the_path, temp_array, 200);
        if (total == 0) {
            FatalError(kFatalError_LoadMaterialFile_S, str);
        }
        for (j = 0; j < total; j++) {
            if (temp_array[j]) {
                switch (AddMaterialToStorage(pStorage_space, temp_array[j])) {
                case eStorage_not_enough_room:
                    FatalError(kFatalError_InsufficientMaterialSlots);
                    break;
                case eStorage_duplicate:
                    BrMaterialFree(temp_array[j]);
                    break;
                case eStorage_allocated:
                    BrMaterialAdd(temp_array[j]);
                    new_ones++;
                }
            }
        }
    }
    return new_ones;
}

// IDA: int __usercall LoadNModels@<EAX>(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>, int pCount@<EBX>)
int LoadNModels(tBrender_storage* pStorage_space, FILE* pF, int pCount) {
    tPath_name the_path;
    int i;
    int j;
    int new_ones;
    int total;
    char s[256];
    char* str;
    br_model* temp_array[2000];
    v11model* prepared;
    int group;
    LOG_TRACE("(%p, %p, %d)", pStorage_space, pF, pCount);

    new_ones = 0;
    for (i = 0; i < pCount; i++) {
        PossibleService();
        GetALineAndDontArgue(pF, s);
        str = strtok(s, "\t ,/");
        PathCat(the_path, gApplication_path, "MODELS");
        PathCat(the_path, the_path, str);
        total = BrModelLoadMany(the_path, temp_array, 2000);
        if (total == 0) {
            FatalError(kFatalError_LoadModelFile_S, str);
        }
        for (j = 0; j < total; j++) {
            if (temp_array[j]) {
                switch (AddModelToStorage(pStorage_space, temp_array[j])) {
                case eStorage_not_enough_room:
                    FatalError(kFatalError_InsufficientModelSlots);
                    break;
                case eStorage_duplicate:
                    BrModelFree(temp_array[j]);
                    break;
                case eStorage_allocated:
                    temp_array[j]->flags |= 0x80u;
                    RemoveDoubleSided(temp_array[j]);
                    BrModelAdd(temp_array[j]);
                    ++new_ones;
                    break;
                }
            }
        }
    }

    return new_ones;
}

// IDA: void __usercall DodgyModelUpdate(br_model *pM@<EAX>)
void DodgyModelUpdate(br_model* pM) {
    LOG_TRACE("(%p)", pM);

    BrResFree(pM->faces);
    BrResFree(pM->vertices);
    pM->nfaces = 0;
    pM->nvertices = 0;
    pM->faces = NULL;
    pM->vertices = NULL;
}

// IDA: br_material* __usercall SuffixedMaterial@<EAX>(br_material *pOld@<EAX>, char *pSuffix@<EDX>)
br_material* SuffixedMaterial(br_material* pOld, char* pSuffix) {
    br_material* new_mat;
    char* new_id;
    LOG_TRACE("(%p, \"%s\")", pOld, pSuffix);

    new_id = BrMemAllocate(strlen(pOld->identifier) + strlen(pSuffix) + 1, kMem_new_mat_id);
    sprintf(new_id, "%s%s", pOld->identifier, pSuffix);
    new_mat = BrMaterialFind(new_id);
    if (new_mat == NULL) {
        new_mat = BrMaterialAllocate(NULL);
        MaterialCopy(new_mat, pOld);
        new_mat->identifier = new_id;
        BrMaterialAdd(new_mat);
    } else {
        BrMemFree(new_id);
    }
    return new_mat;
}

// IDA: int __usercall FaceIsRoad@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
int FaceIsRoad(br_model* pModel, tU16 pFace) {
    br_vector3 v0;
    br_vector3 v1;
    br_vector3 cross;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    BrVector3Sub(&v0, &pModel->vertices[pModel->faces[pFace].vertices[0]].p, &pModel->vertices[pModel->faces[pFace].vertices[1]].p);
    BrVector3Sub(&v1, &pModel->vertices[pModel->faces[pFace].vertices[1]].p, &pModel->vertices[pModel->faces[pFace].vertices[2]].p);
    BrVector3Cross(&cross, &v0, &v1);
    return sqrtf(cross.v[0] * cross.v[0] + cross.v[2] * cross.v[2]) < 0.7f * cross.v[1];
}

// IDA: br_material* __usercall RoadPerspToUntex@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* RoadPerspToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    old_mat = pModel->faces[pFace].material;
    if (old_mat->colour_map == NULL || !FaceIsRoad(pModel, pFace)) {
        return NULL;
    }
    new_mat = SuffixedMaterial(old_mat, ".road");
    if (new_mat->colour_map != NULL) {
        new_mat->colour_map = NULL;
        BrMaterialUpdate(new_mat, BR_MATU_ALL);
    }
    return new_mat;
}

// IDA: br_material* __usercall WallPerspToLinear@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallPerspToLinear(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    old_mat = pModel->faces[pFace].material;
    if (old_mat->colour_map == NULL || !(old_mat->flags & BR_MATF_PERSPECTIVE) || FaceIsRoad(pModel, pFace)) {
        return NULL;
    }
    new_mat = SuffixedMaterial(old_mat, ".pwall");
    if (new_mat->flags & BR_MATF_PERSPECTIVE) {
        new_mat->flags &= ~BR_MATF_PERSPECTIVE;
        BrMaterialUpdate(new_mat, BR_MATU_ALL);
    }
    return new_mat;
}

// IDA: br_material* __usercall WallPerspToUntex@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallPerspToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    old_mat = pModel->faces[pFace].material;
    if (old_mat->colour_map == NULL || FaceIsRoad(pModel, pFace)) {
        return NULL;
    }
    if (old_mat->flags & BR_MATF_PERSPECTIVE) {
        new_mat = SuffixedMaterial(old_mat, ".pwall");
    } else {
        new_mat = SuffixedMaterial(old_mat, ".lwall");
    }
    if (new_mat->colour_map != NULL) {
        new_mat->colour_map = NULL;
        BrMaterialUpdate(new_mat, BR_MATU_ALL);
    }
    return new_mat;
}

// IDA: void __usercall ProcessModelFaceMaterials2(br_model *pModel@<EAX>, tPMFM2CB pCallback@<EDX>)
void ProcessModelFaceMaterials2(br_model* pModel, tPMFM2CB pCallback) {
    tU16 f;
    tU16 group;
    br_material* old_mat;
    LOG_TRACE("(%p, %d)", pModel, pCallback);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ProcessModelFaceMaterials(br_model *pModel@<EAX>, tPMFMCB pCallback@<EDX>)
void ProcessModelFaceMaterials(br_model* pModel, tPMFMCB pCallback) {
    tU16 f;
    br_material* possible_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pCallback);

    new_mat = NULL;
    for (f = 0; f < pModel->nfaces; f++) {
        if (pModel->faces[f].material != NULL) {
            possible_mat = (*pCallback)(pModel, f);
            if (possible_mat != NULL) {
                pModel->faces[f].material = possible_mat;
                new_mat = possible_mat;
            }
        }
    }
    if (new_mat != NULL) {
        BrModelUpdate(pModel, BR_MATU_ALL);
    }
}

// IDA: int __usercall LoadNTrackModels@<EAX>(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>, int pCount@<EBX>)
int LoadNTrackModels(tBrender_storage* pStorage_space, FILE* pF, int pCount) {
    tPath_name the_path;
    int i;
    int j;
    int new_ones;
    int total;
    int group;
    char s[256];
    char* str;
    br_model* temp_array[2000];
    v11model* prepared;
    LOG_TRACE("(%p, %p, %d)", pStorage_space, pF, pCount);

    new_ones = 0;
    for (i = 0; i < pCount; i++) {
        GetALineAndDontArgue(pF, s);
        str = strtok(s, "\t ,/");
        PathCat(the_path, gApplication_path, "MODELS");
        PathCat(the_path, the_path, str);
        total = BrModelLoadMany(the_path, temp_array, 2000);
        if (total == 0) {
            FatalError(kFatalError_LoadModelFile_S, str);
        }
        for (j = 0; j < total; j++) {
            if (temp_array[j]) {
                switch (AddModelToStorage(pStorage_space, temp_array[j])) {
                case eStorage_not_enough_room:
                    FatalError(kFatalError_InsufficientModelSlots);
                    break;
                case eStorage_duplicate:
                    BrModelFree(temp_array[j]);
                    break;
                case eStorage_allocated:
                    temp_array[j]->flags |= BR_MODF_UPDATEABLE;
                    if (gRoad_texturing_level == eRTL_none) {
                        ProcessModelFaceMaterials(temp_array[j], RoadPerspToUntex);
                    }
                    switch (gWall_texturing_level) {
                    case eWTL_none:
                        ProcessModelFaceMaterials(temp_array[j], WallPerspToUntex);
                        break;
                    case eWTL_linear:
                        ProcessModelFaceMaterials(temp_array[j], WallPerspToLinear);
                        break;
                    default:
                        break;
                    }
                    RemoveDoubleSided(temp_array[j]);
                    BrModelAdd(temp_array[j]);
                    new_ones++;
                }
            }
        }
    }
    return new_ones;
}

// IDA: void __usercall LoadSomePixelmaps(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>)
void LoadSomePixelmaps(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_pixelmap* temp_array[200];
    LOG_TRACE("(%p, %p)", pStorage_space, pF);

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &count);
    LoadNPixelmaps(pStorage_space, pF, count);
}

// IDA: void __usercall LoadSomeShadeTables(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>)
void LoadSomeShadeTables(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_pixelmap* temp_array[50];
    LOG_TRACE("(%p, %p)", pStorage_space, pF);

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &count);
    LoadNShadeTables(pStorage_space, pF, count);
}

// IDA: void __usercall LoadSomeMaterials(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>)
void LoadSomeMaterials(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_material* temp_array[200];
    LOG_TRACE("(%p, %p)", pStorage_space, pF);

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &count);
    LoadNMaterials(pStorage_space, pF, count);
}

// IDA: void __usercall LoadSomeModels(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>)
void LoadSomeModels(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_model* temp_array[2000];
    LOG_TRACE("(%p, %p)", pStorage_space, pF);

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &count);
    LoadNModels(pStorage_space, pF, count);
}

// IDA: void __usercall LoadSomeTrackModels(tBrender_storage *pStorage_space@<EAX>, FILE *pF@<EDX>)
void LoadSomeTrackModels(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_model* temp_array[2000];
    LOG_TRACE("(%p, %p)", pStorage_space, pF);

    GetALineAndDontArgue(pF, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%d", &count);
    LoadNTrackModels(pStorage_space, pF, count);
}

// IDA: void __usercall AddFunkGrooveBinding(int pSlot_number@<EAX>, float *pPeriod_address@<EDX>)
void AddFunkGrooveBinding(int pSlot_number, float* pPeriod_address) {
    LOG_TRACE("(%d, %p)", pSlot_number, pPeriod_address);

    if (pSlot_number < 0 || pSlot_number >= COUNT_OF(gGroove_funk_bindings)) {
        FatalError(kFatalError_DefinedRefNumGrooveFunkOutOfRange);
    }

    gGroove_funk_bindings[pSlot_number] = pPeriod_address;
    *pPeriod_address = 0.0f;
}

// IDA: void __usercall ControlBoundFunkGroove(int pSlot_number@<EAX>, float pValue)
void ControlBoundFunkGroove(int pSlot_number, float pValue) {
    LOG_TRACE("(%d, %f)", pSlot_number, pValue);

    if (pSlot_number >= 0) {
        if (pSlot_number >= COUNT_OF(gGroove_funk_bindings)) {
            FatalError(kFatalError_UsedRefNumGrooveFunkOutOfRange);
        }
        *gGroove_funk_bindings[pSlot_number] = pValue;
    }
}

// IDA: float __usercall ControlBoundFunkGroovePlus@<ST0>(int pSlot_number@<EAX>, float pValue)
float ControlBoundFunkGroovePlus(int pSlot_number, float pValue) {
    LOG_TRACE("(%d, %f)", pSlot_number, pValue);

    if (pSlot_number < 0) {
        return 0.f;
    }
    if (pSlot_number >= COUNT_OF(gGroove_funk_bindings)) {
        FatalError(kFatalError_UsedRefNumGrooveFunkOutOfRange);
    }
    *gGroove_funk_bindings[pSlot_number] = fmodf(*gGroove_funk_bindings[pSlot_number] + pValue, 1.f);
    return *gGroove_funk_bindings[pSlot_number];
}

// IDA: void __usercall ShiftBoundGrooveFunks(char *pStart@<EAX>, char *pEnd@<EDX>, int pDelta@<EBX>)
void ShiftBoundGrooveFunks(char* pStart, char* pEnd, ptrdiff_t pDelta) {
    int i;
    LOG_TRACE("(\"%s\", \"%s\", %d)", pStart, pEnd, pDelta);

    for (i = 0; i < COUNT_OF(gGroove_funk_bindings); i++) {
        if (pStart <= (char*)gGroove_funk_bindings[i] && (char*)gGroove_funk_bindings[i] < pEnd) {
            gGroove_funk_bindings[i] = (float*)((char*)gGroove_funk_bindings[i] + (pDelta & ~(sizeof(void*) - 1))); // original code is (pDelta & 0xFFFFFFFC) but this caused problems;
        }
    }
}

// IDA: tFunkotronic_spec* __cdecl AddNewFunkotronic()
tFunkotronic_spec* AddNewFunkotronic() {
    void* new_array;
    int i;
    LOG_TRACE("()");

    for (i = 0; i < gFunkotronics_array_size; i++) {
        if (gFunkotronics_array[i].owner == -999) {
            memset(&gFunkotronics_array[i], 0, sizeof(tFunkotronic_spec));
            return &gFunkotronics_array[i];
        }
    }
    gFunkotronics_array_size += 16;
    new_array = BrMemCalloc(gFunkotronics_array_size, sizeof(tFunkotronic_spec), kMem_funk_spec);
    if (gFunkotronics_array != NULL) {
        memcpy(new_array, gFunkotronics_array, (gFunkotronics_array_size - 16) * sizeof(tFunkotronic_spec));
        ShiftBoundGrooveFunks(
            (char*)gFunkotronics_array,
            (char*)&gFunkotronics_array[gFunkotronics_array_size - 16],
            (char*)new_array - (char*)gFunkotronics_array);
        BrMemFree(gFunkotronics_array);
    }
    gFunkotronics_array = new_array;
    for (i = 0; i < 16; i++) {
        gFunkotronics_array[gFunkotronics_array_size - 16 + i].owner = -999;
    }
    return &gFunkotronics_array[gFunkotronics_array_size - 16];
}

// IDA: void __usercall DisposeFunkotronics(int pOwner@<EAX>)
void DisposeFunkotronics(int pOwner) {
    int i;
    tFunkotronic_spec* the_funk;
    LOG_TRACE("(%d)", pOwner);

    if (gFunkotronics_array == NULL) {
        return;
    }
    for (i = 0; i < gFunkotronics_array_size; i++) {
        the_funk = &gFunkotronics_array[i];
        PossibleService();
        if (the_funk->owner == pOwner) {
            the_funk->owner = -999;
            if (the_funk->proximity_array != NULL) {
                BrMemFree(the_funk->proximity_array);
            }
            if (the_funk->texture_animation_type == eTexture_animation_flic) {
                BrMemFree(the_funk->texture_animation_data.flic_info.flic_data);
                EndFlic(&the_funk->texture_animation_data.flic_info.flic_descriptor);
                BrMemFree(the_funk->material->colour_map->pixels);
                the_funk->material->colour_map->pixels = NULL;
                BrPixelmapFree(the_funk->material->colour_map);
                the_funk->material->colour_map = NULL;
            }
        }
    }
}

// IDA: void __usercall AddProximityVertex(br_vector3 *pV@<EAX>, tFunkotronic_spec *pThe_funk@<EDX>)
void AddProximityVertex(br_vector3* pV, tFunkotronic_spec* pThe_funk) {
    LOG_TRACE("(%p, %p)", pV, pThe_funk);

    pThe_funk->proximity_array[pThe_funk->proximity_count] = *pV;
    pThe_funk->proximity_count++;
}

// IDA: void __cdecl AddProximityVertexXYZ(br_scalar pX, br_scalar pY, br_scalar pZ, tFunkotronic_spec *pThe_funk)
void AddProximityVertexXYZ(br_scalar pX, br_scalar pY, br_scalar pZ, tFunkotronic_spec* pThe_funk) {
    br_vector3 v;
    LOG_TRACE("(%f, %f, %f, %p)", pX, pY, pZ, pThe_funk);

    v.v[0] = pX;
    v.v[1] = pY;
    v.v[2] = pZ;
    AddProximityVertex(&v, pThe_funk);
}

// IDA: br_uint_32 __usercall CalcProximities@<EAX>(br_actor *pActor@<EAX>, br_material *pMat@<EDX>, tFunkotronic_spec *pThe_funk@<EBX>)
br_uint_32 CalcProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk) {
    br_face* the_face;
    int i;
    LOG_TRACE("(%p, %p, %p)", pActor, pMat, pThe_funk);

    if (pActor->model != NULL) {
        if (pThe_funk->material == pMat) {
            pThe_funk->proximity_count += 8;
        } else {
            for (i = 0; i < pActor->model->nfaces; i++) {
                the_face = &pActor->model->faces[i];
                if (pThe_funk->material == the_face->material) {
                    pThe_funk->proximity_count += 3;
                }
            }
        }
    }
    return 0;
}

// IDA: br_uint_32 __usercall AddProximities@<EAX>(br_actor *pActor@<EAX>, br_material *pMat@<EDX>, tFunkotronic_spec *pThe_funk@<EBX>)
br_uint_32 AddProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk) {
    br_face* the_face;
    int i;
    LOG_TRACE("(%p, %p, %p)", pActor, pMat, pThe_funk);

    if (pActor->model != NULL) {
        if (pThe_funk->material == pMat) {
            AddProximityVertexXYZ(
                pActor->model->bounds.min.v[0],
                pActor->model->bounds.min.v[1],
                pActor->model->bounds.min.v[2],
                pThe_funk);
            AddProximityVertexXYZ(
                pActor->model->bounds.min.v[0],
                pActor->model->bounds.min.v[1],
                pActor->model->bounds.max.v[2],
                pThe_funk);
            AddProximityVertexXYZ(
                pActor->model->bounds.min.v[0],
                pActor->model->bounds.max.v[1],
                pActor->model->bounds.min.v[2],
                pThe_funk);
            AddProximityVertexXYZ(
                pActor->model->bounds.min.v[0],
                pActor->model->bounds.max.v[1],
                pActor->model->bounds.max.v[2],
                pThe_funk);
            AddProximityVertexXYZ(
                pActor->model->bounds.max.v[0],
                pActor->model->bounds.min.v[1],
                pActor->model->bounds.min.v[2],
                pThe_funk);
            AddProximityVertexXYZ(
                pActor->model->bounds.max.v[0],
                pActor->model->bounds.min.v[1],
                pActor->model->bounds.max.v[2],
                pThe_funk);
            AddProximityVertexXYZ(
                pActor->model->bounds.max.v[0],
                pActor->model->bounds.max.v[1],
                pActor->model->bounds.min.v[2],
                pThe_funk);
            AddProximityVertexXYZ(
                pActor->model->bounds.max.v[0],
                pActor->model->bounds.max.v[1],
                pActor->model->bounds.max.v[2],
                pThe_funk);
        } else {
            for (i = 0; i < pActor->model->nfaces; i++) {
                the_face = &pActor->model->faces[i];
                if (pThe_funk->material == the_face->material) {
                    AddProximityVertex(&pActor->model->vertices[the_face->vertices[0]].p, pThe_funk);
                    AddProximityVertex(&pActor->model->vertices[the_face->vertices[1]].p, pThe_funk);
                    AddProximityVertex(&pActor->model->vertices[the_face->vertices[2]].p, pThe_funk);
                }
            }
        }
    }
    return 0;
}

// IDA: void __usercall Adjust2FloatsForExceptions(float *pVictim1@<EAX>, float *pVictim2@<EDX>, br_pixelmap *pCulprit@<EBX>)
void Adjust2FloatsForExceptions(float* pVictim1, float* pVictim2, br_pixelmap* pCulprit) {
    tException_list e;
    LOG_TRACE("(%p, %p, %p)", pVictim1, pVictim2, pCulprit);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddFunkotronics(FILE *pF@<EAX>, int pOwner@<EDX>, int pRef_offset@<EBX>)
void AddFunkotronics(FILE* pF, int pOwner, int pRef_offset) {
    char s[256];
    char* str;
    int first_time;
    int i;
    int j;
    tFunkotronic_spec* the_funk;
    float speed1;
    float speed2;
    float a_min;
    float d_min;
    float s_min;
    float a_max;
    float d_max;
    float s_max;
    void* the_pixels;
    br_pixelmap* the_pixelmap;
    float x_0;
    float x_1;
    int d_0;
    int d_1;
    LOG_TRACE("(%p, %d, %d)", pF, pOwner, pRef_offset);

    first_time = 1;
    while (!feof(pF)) {
        PossibleService();
        GetALineAndDontArgue(pF, s);
        if (strcmp(s, "END OF FUNK") == 0) {
            break;
        }

        if (!first_time) {
            if (strcmp(s, "NEXT FUNK") != 0) {
                FatalError(kFatalError_FunkotronicFile);
            }
            GetALineAndDontArgue(pF, s);
        }
        first_time = 0;

        the_funk = AddNewFunkotronic();
        the_funk->owner = pOwner;
        str = strtok(s, "\t ,/");
        the_funk->material = BrMaterialFind(str);
        if (the_funk->material == NULL) {
            FatalError(kFatalError_FindMaterialUsedByFunkotronicFile_S, str);
        }
        the_funk->mode = GetALineAndInterpretCommand(pF, gFunk_nature_names, COUNT_OF(gFunk_nature_names));
        the_funk->matrix_mod_type = GetALineAndInterpretCommand(pF, gFunk_type_names, COUNT_OF(gFunk_type_names));
        if (the_funk->matrix_mod_type != eMatrix_mod_none) {
            the_funk->matrix_mode = GetALineAndInterpretCommand(pF, gFunk_move_names, COUNT_OF(gFunk_move_names));
        }
        switch (the_funk->matrix_mod_type) {
        case eMatrix_mod_spin:
            if (the_funk->matrix_mode == eMove_controlled || the_funk->matrix_mode == eMove_absolute) {
                i = GetAnInt(pF);
                AddFunkGrooveBinding(i + pRef_offset, &the_funk->matrix_mod_data.spin_info.period);
            } else {
                x_0 = GetAFloat(pF);
                the_funk->matrix_mod_data.spin_info.period = (x_0 == 0.0f) ? 0.0f : 1000.0f / x_0;
            }
            break;
        case eMatrix_mod_rock: // rock
            if (the_funk->matrix_mode == eMove_controlled || the_funk->matrix_mode == eMove_absolute) {
                d_0 = GetAnInt(pF);
                AddFunkGrooveBinding(d_0 + pRef_offset, &the_funk->matrix_mod_data.rock_info.period);
            } else {
                x_0 = GetAFloat(pF);
                the_funk->matrix_mod_data.rock_info.period = (x_0 == 0.0f) ? 0.0f : 1000.0f / x_0;
            }

            the_funk->matrix_mod_data.rock_info.rock_angle = GetAFloat(pF);
            GetPairOfFloats(pF, &x_0, &x_1);
            the_funk->matrix_mod_data.rock_info.x_centre = x_0 / 100.0f;
            the_funk->matrix_mod_data.rock_info.y_centre = x_1 / 100.0f;
            break;
        case eMatrix_mod_throb: // throb
            if (the_funk->matrix_mode == eMove_controlled || the_funk->matrix_mode == eMove_absolute) {
                GetPairOfInts(pF, &d_0, &d_1);
                if (d_0 >= 0) {
                    AddFunkGrooveBinding(d_0 + pRef_offset, &the_funk->matrix_mod_data.throb_info.x_period);
                }
                if (d_1 >= 0) {
                    AddFunkGrooveBinding(d_1 + pRef_offset, &the_funk->matrix_mod_data.throb_info.y_period);
                }
            } else {
                GetPairOfFloats(pF, &speed1, &speed2);
                the_funk->matrix_mod_data.throb_info.x_period = (speed1 == 0.0f) ? 0.0f : 1000.0f / speed1;
                the_funk->matrix_mod_data.throb_info.y_period = (speed2 == 0.0f) ? 0.0f : 1000.0f / speed2;
            }
            GetPairOfFloatPercents(
                pF,
                &the_funk->matrix_mod_data.throb_info.x_magnitude,
                &the_funk->matrix_mod_data.throb_info.y_magnitude);
            GetPairOfFloats(pF, &x_0, &x_1);
            the_funk->matrix_mod_data.throb_info.x_centre = x_0 / 100.0f;
            the_funk->matrix_mod_data.throb_info.y_centre = x_1 / 100.0f;
            if (the_funk->matrix_mode != eMove_controlled) {
                if (the_funk->matrix_mod_data.throb_info.x_period == 0.0f) {
                    the_funk->matrix_mod_data.throb_info.x_period = 1.0f;
                    the_funk->matrix_mod_data.throb_info.x_magnitude = 0.0f;
                }
                if (the_funk->matrix_mod_data.throb_info.y_period == 0.0f) {
                    the_funk->matrix_mod_data.throb_info.y_period = 1.0f;
                    the_funk->matrix_mod_data.throb_info.y_magnitude = 0.0f;
                }
            }
            break;
        case eMatrix_mod_slither: // slither
            if (the_funk->matrix_mode == eMove_controlled || the_funk->matrix_mode == eMove_absolute) {
                GetPairOfInts(pF, &d_0, &d_1);
                if (d_0 >= 0) {
                    AddFunkGrooveBinding(d_0 + pRef_offset, &the_funk->matrix_mod_data.slither_info.x_period);
                }
                if (d_1 >= 0) {
                    AddFunkGrooveBinding(d_1 + pRef_offset, &the_funk->matrix_mod_data.slither_info.y_period);
                }
            } else {
                GetPairOfFloats(pF, &speed1, &speed2);
                the_funk->matrix_mod_data.slither_info.x_period = (speed1 == 0.0f) ? 0.0f : 1000.0f / speed1;
                the_funk->matrix_mod_data.slither_info.y_period = (speed2 == 0.0f) ? 0.0f : 1000.0f / speed2;
            }
            GetPairOfFloatPercents(
                pF,
                &the_funk->matrix_mod_data.slither_info.x_magnitude,
                &the_funk->matrix_mod_data.slither_info.y_magnitude);
            if (the_funk->matrix_mode != eMove_controlled) {
                if (the_funk->matrix_mod_data.slither_info.x_period == 0.0f) {
                    the_funk->matrix_mod_data.slither_info.x_period = 1.0f;
                    the_funk->matrix_mod_data.slither_info.x_magnitude = 0.0f;
                }
                if (the_funk->matrix_mod_data.slither_info.y_period == 0.0f) {
                    the_funk->matrix_mod_data.slither_info.y_period = 1.0f;
                    the_funk->matrix_mod_data.slither_info.y_magnitude = 0.0f;
                }
            }
            break;
        case eMatrix_mod_roll: // roll
            if (the_funk->matrix_mode == eMove_controlled || the_funk->matrix_mode == eMove_absolute) {
                GetPairOfInts(pF, &d_0, &d_1);
                if (d_0 >= 0) {
                    AddFunkGrooveBinding(d_0 + pRef_offset, &the_funk->matrix_mod_data.roll_info.x_period);
                }
                if (d_1 >= 0) {
                    AddFunkGrooveBinding(d_1 + pRef_offset, &the_funk->matrix_mod_data.roll_info.y_period);
                }
            } else {
                GetPairOfFloats(pF, &speed1, &speed2);
                the_funk->matrix_mod_data.roll_info.x_period = speed1 == 0.0f ? 0.0f : 1000.0f / speed1;
                the_funk->matrix_mod_data.roll_info.y_period = speed2 == 0.0f ? 0.0f : 1000.0f / speed2;
            }
            break;
        default:
            break;
        }
        the_funk->lighting_animation_type = GetALineAndInterpretCommand(pF, gFunk_move_names, COUNT_OF(gFunk_move_names));
        if (the_funk->lighting_animation_type != eMove_none) {
            if (the_funk->lighting_animation_type == eMove_controlled || the_funk->lighting_animation_type == eMove_absolute) {
                d_0 = GetAnInt(pF);
                AddFunkGrooveBinding(d_0 + pRef_offset, &the_funk->lighting_animation_period);
            } else {
                x_0 = GetAFloat(pF);
                the_funk->lighting_animation_period = (x_0 == 0.0f) ? 0.0f : 1000.0f / x_0;
            }
            GetThreeFloatPercents(pF, &a_min, &d_min, &s_min);
            GetThreeFloatPercents(pF, &a_max, &d_max, &s_max);
            the_funk->ambient_base = (a_min + a_max) / 2.0f;
            the_funk->direct_base = (d_min + d_max) / 2.0f;
            the_funk->specular_base = (s_min + s_max) / 2.0f;
            the_funk->ambient_delta = (a_max - a_min) / 2.0f;
            the_funk->direct_delta = (d_max - d_min) / 2.0f;
            the_funk->specular_delta = (s_max - s_min) / 2.0f;
        }
        the_funk->texture_animation_type = GetALineAndInterpretCommand(pF, gFunk_anim_names, COUNT_OF(gFunk_anim_names));
        if (the_funk->texture_animation_type != eTexture_animation_none) {
            the_funk->time_mode = GetALineAndInterpretCommand(pF, gTime_mode_names, COUNT_OF(gTime_mode_names));
        }
        if (the_funk->texture_animation_type == eTexture_animation_flic && gAusterity_mode) {
            the_funk->texture_animation_type = eTexture_animation_none;
            GetALineAndDontArgue(pF, s);
        }
        the_funk->last_frame = 0.0f;

        if (the_funk->texture_animation_type == eTexture_animation_flic) {
            GetAString(pF, s);
            the_funk->texture_animation_data.flic_info.flic_data = 0;
            if (LoadFlicData(
                    s,
                    &the_funk->texture_animation_data.flic_info.flic_data,
                    &the_funk->texture_animation_data.flic_info.flic_data_length)) {
                the_funk->texture_animation_data.flic_info.flic_descriptor.data_start = NULL;
                StartFlic(
                    s,
                    -1,
                    &the_funk->texture_animation_data.flic_info.flic_descriptor,
                    the_funk->texture_animation_data.flic_info.flic_data_length,
                    (tS8*)the_funk->texture_animation_data.flic_info.flic_data,
                    0,
                    0,
                    0,
                    0);
                the_funk->last_frame = 0.0f;
                the_pixels = BrMemAllocate(
                    the_funk->texture_animation_data.flic_info.flic_descriptor.height
                        * ((the_funk->texture_animation_data.flic_info.flic_descriptor.width + 3) & ~3),
                    kMem_video_pixels);
                if (gScreen->row_bytes < 0) {
                    BrFatal(
                        "C:\\Msdev\\Projects\\DethRace\\World.c",
                        1729,
                        "Bruce bug at line %d, file C:\\Msdev\\Projects\\DethRace\\World.c",
                        193);
                }
                the_pixelmap = DRPixelmapAllocate(
                    gScreen->type,
                    the_funk->texture_animation_data.flic_info.flic_descriptor.width,
                    the_funk->texture_animation_data.flic_info.flic_descriptor.height,
                    the_pixels,
                    0);
                AssertFlicPixelmap(&the_funk->texture_animation_data.flic_info.flic_descriptor, the_pixelmap);
                the_funk->material->colour_map = the_pixelmap;
                BrMaterialUpdate(the_funk->material, BR_MATU_ALL);
            } else {
                the_funk->texture_animation_type = eTexture_animation_none;
            }
        } else if (the_funk->texture_animation_type == eTexture_animation_frames) {
            i = GetALineAndInterpretCommand(pF, gFunk_move_names, COUNT_OF(gFunk_move_names));
            the_funk->texture_animation_data.frames_info.mode = i;
            if (the_funk->texture_animation_data.frames_info.mode == eMove_controlled
                || the_funk->texture_animation_data.frames_info.mode == eMove_absolute) {
                d_0 = GetAnInt(pF);
                AddFunkGrooveBinding(d_0 + pRef_offset, &the_funk->texture_animation_data.frames_info.period);
            } else {
                x_0 = GetAFloat(pF);
                the_funk->texture_animation_data.frames_info.period = (x_0 == 0.0f) ? 0.0f : 1000.0F / x_0;
            }

            the_funk->texture_animation_data.frames_info.texture_count = (int)GetAFloat(pF);
            for (i = 0; i < the_funk->texture_animation_data.frames_info.texture_count; i++) {
                GetAString(pF, s);
                the_funk->texture_animation_data.frames_info.textures[i] = BrMapFind(s);
                if (the_funk->texture_animation_data.frames_info.textures[i] == NULL) {
                    FatalError(kFatalError_AnimationFramePixelmapUsedByFunkotronicFile);
                }
            }
        }
        the_funk->proximity_count = 0;
        the_funk->proximity_array = 0;
        if (the_funk->mode == eFunk_mode_distance) {
            DRActorEnumRecurseWithMat(gUniverse_actor, NULL, (recurse_with_mat_cbfn*)CalcProximities, the_funk);
            the_funk->proximity_array = BrMemAllocate(sizeof(br_vector3) * the_funk->proximity_count, kMem_funk_prox_array);
            the_funk->proximity_count = 0;
            DRActorEnumRecurseWithMat(gUniverse_actor, 0, (recurse_with_mat_cbfn*)AddProximities, the_funk);
            for (i = 0; i < the_funk->proximity_count; i++) {
                for (j = i + 1; j < the_funk->proximity_count; j++) {
                    if (the_funk->proximity_array[j].v[0] == the_funk->proximity_array[i].v[0]
                        && the_funk->proximity_array[j].v[1] == the_funk->proximity_array[i].v[1]
                        && the_funk->proximity_array[j].v[2] == the_funk->proximity_array[i].v[2]) {
                        memmove(
                            &the_funk->proximity_array[j],
                            &the_funk->proximity_array[j + 1],
                            sizeof(br_vector3) * (the_funk->proximity_count - j - 1));
                        the_funk->proximity_count--;
                        j--;
                    }
                }
            }
        }
    }
}

// IDA: void __usercall DisposeGroovidelics(int pOwner@<EAX>)
void DisposeGroovidelics(int pOwner) {
    int i;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%d)", pOwner);

    if (gGroovidelics_array == NULL) {
        return;
    }
    for (i = 0; i < gGroovidelics_array_size; i++) {
        the_groove = &gGroovidelics_array[i];
        PossibleService();
        if (the_groove->owner == pOwner) {
            the_groove->owner = -999;
        }
    }
}

// IDA: tGroovidelic_spec* __cdecl AddNewGroovidelic()
tGroovidelic_spec* AddNewGroovidelic() {
    void* new_array;
    int i;
    LOG_TRACE("()");

    for (i = 0; i < gGroovidelics_array_size; i++) {
        if (gGroovidelics_array[i].owner == -999) {
            memset(&gGroovidelics_array[i], 0, sizeof(tGroovidelic_spec));
            return &gGroovidelics_array[i];
        }
    }
    gGroovidelics_array_size += 16;
    new_array = BrMemCalloc(gGroovidelics_array_size, sizeof(tGroovidelic_spec), kMem_groove_spec);
    if (gGroovidelics_array != NULL) {
        memcpy(new_array, gGroovidelics_array, (gGroovidelics_array_size - 16) * sizeof(tGroovidelic_spec));
        ShiftBoundGrooveFunks(
            (char*)gGroovidelics_array,
            (char*)&gGroovidelics_array[gGroovidelics_array_size - 16],
            (char*)new_array - (char*)gGroovidelics_array);
        BrMemFree(gGroovidelics_array);
    }
    gGroovidelics_array = new_array;
    for (i = 0; i < 16; i++) {
        gGroovidelics_array[i + gGroovidelics_array_size - 16].owner = -999;
    }
    return &gGroovidelics_array[gGroovidelics_array_size - 16];
}

// IDA: void __usercall AddGroovidelics(FILE *pF@<EAX>, int pOwner@<EDX>, br_actor *pParent_actor@<EBX>, int pRef_offset@<ECX>, int pAllowed_to_be_absent)
void AddGroovidelics(FILE* pF, int pOwner, br_actor* pParent_actor, int pRef_offset, int pAllowed_to_be_absent) {
    char s[256];
    char* str;
    int first_time;
    int i;
    int j;
    tGroovidelic_spec* the_groove;
    float x_0;
    float x_1;
    float x_2;
    int d_0;
    int d_1;
    int d_2;
    LOG_TRACE("(%p, %d, %p, %d, %d)", pF, pOwner, pParent_actor, pRef_offset, pAllowed_to_be_absent);

    first_time = 1;

    while (!feof(pF)) {
        PossibleService();
        GetALineAndDontArgue(pF, s);
        if (strcmp(s, "END OF GROOVE") == 0) {
            break;
        }

        if (!first_time) {
            if (strcmp(s, "NEXT GROOVE") != 0) {
                FatalError(kFatalError_GroovidelicFile);
            }
            GetALineAndDontArgue(pF, s);
        }
        first_time = 0;
        str = strtok(s, "\t ,/");
        the_groove = AddNewGroovidelic();
        the_groove->owner = pOwner;
        the_groove->actor = DRActorFindRecurse(pParent_actor, str);

        if (the_groove->actor == NULL) {
            if (!pAllowed_to_be_absent && !gAusterity_mode) {
                FatalError(kFatalError_FindActorUsedByGroovidelicFile_S, str);
            }
            if (gGroove_by_proxy_actor == NULL) {
                gGroove_by_proxy_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
                gGroove_by_proxy_actor->model = LoadModel("PROXY.DAT");
                BrModelAdd(gGroove_by_proxy_actor->model);
                BrActorAdd(gDont_render_actor, gGroove_by_proxy_actor);
            }
            the_groove->actor = gGroove_by_proxy_actor;
        }
        the_groove->lollipop_mode = GetALineAndInterpretCommand(pF, gLollipop_names, COUNT_OF(gLollipop_names));
        the_groove->mode = GetALineAndInterpretCommand(pF, gGroove_nature_names, COUNT_OF(gGroove_nature_names));

        the_groove->path_type = GetALineAndInterpretCommand(pF, gGroove_path_names, COUNT_OF(gGroove_path_names));
        the_groove->path_interrupt_status = eInterrupt_none;
        the_groove->object_interrupt_status = eInterrupt_none;
        if (the_groove->path_type != eGroove_path_none) {
            the_groove->path_mode = GetALineAndInterpretCommand(pF, gFunk_move_names, COUNT_OF(gFunk_move_names));
        }

        if (the_groove->path_type == eGroove_path_circular) {
            TELL_ME_IF_WE_PASS_THIS_WAY();
            // GetThreeFloats(pF, &v90, &v89, &v88);
            // the_groove->path_data.circular_info.centre.v[0] = v90;
            // the_groove->path_data.circular_info.centre.v[1] = v89;
            // the_groove->path_data.circular_info.centre.v[2] = v88;
            // if (the_groove->path_data.circular_info.centre.v[2] == 0.0
            //     && *(float*)&the_groove->path_data.circular_info.axis == 0.0
            //     && *((float*)&the_groove->path_data.circular_info + 6) == 0.0) {
            //     v25 = (_DWORD*)the_groove->actor->t.t.translate.t.v;
            //     v26 = (_DWORD*)&the_groove->path_data.circular_info.centre.v[2];
            //     *v26 = *v25;
            //     v26[1] = v25[1];
            //     v26[2] = v25[2];
            // }
            // if (the_groove->path_mode != 3 && the_groove->path_mode != 4) {
            //     v29 = GetAFloat(pF);
            //     x_0 = v29;
            //     if (v31) {
            //         v62 = 0.0;
            //     } else {
            //         v62 = 1000.0 / x_0;
            //     }
            //     the_groove->path_data.circular_info.period = v62;
            // } else {
            //     v27 = &the_groove->path_data.circular_info.period;
            //     v28 = GetAnInt(pF);
            //     AddFunkGrooveBinding(v28 + pRef_offset, v27);
            // }
            // v32 = GetAFloat(pF);
            // the_groove->path_data.circular_info.radius = v32;
            // v33 = GetALineAndInterpretCommand(pF, gAxis_names, 3);
            // the_groove->path_data.circular_info.axis = v33;

        } else if (the_groove->path_type == eGroove_path_straight) {
            GetThreeFloats(pF,
                &the_groove->path_data.straight_info.centre.v[0],
                &the_groove->path_data.straight_info.centre.v[1],
                &the_groove->path_data.straight_info.centre.v[2]);

            if (Vector3IsZero(&the_groove->path_data.straight_info.centre)) {
                BrVector3Copy(&the_groove->path_data.straight_info.centre,
                    &the_groove->actor->t.t.translate.t);
            }
            if (the_groove->path_mode == eMove_controlled || the_groove->path_mode == eMove_absolute) {
                AddFunkGrooveBinding(pRef_offset + GetAnInt(pF), &the_groove->path_data.straight_info.period);
            } else {
                x_0 = GetAFloat(pF);
                the_groove->path_data.straight_info.period = x_0 == 0.0f ? 0.0f : 1000.0 / x_0;
            }
            GetThreeFloats(
                pF,
                &the_groove->path_data.straight_info.x_delta,
                &the_groove->path_data.straight_info.y_delta,
                &the_groove->path_data.straight_info.z_delta);
        }
        the_groove->object_type = GetALineAndInterpretCommand(pF, gGroove_object_names, COUNT_OF(gGroove_object_names));
        BrVector3Copy(&the_groove->object_position, &the_groove->actor->t.t.translate.t);
        if (the_groove->object_type != eGroove_object_none) {
            the_groove->object_mode = GetALineAndInterpretCommand(pF, gFunk_move_names, COUNT_OF(gFunk_move_names));
        }
        switch (the_groove->object_type) {
        case eGroove_object_spin:
            if (the_groove->object_mode == eMove_controlled || the_groove->object_mode == eMove_absolute) {
                AddFunkGrooveBinding(pRef_offset + GetAnInt(pF), &the_groove->object_data.spin_info.period);
            } else {
                x_0 = GetAFloat(pF);
                the_groove->object_data.spin_info.period = (x_0 == 0.0f) ? 0.0f : (1000.0f / x_0);
            }
            GetThreeFloats(pF,
                &the_groove->object_centre.v[0],
                &the_groove->object_centre.v[1],
                &the_groove->object_centre.v[2]);
            the_groove->object_data.spin_info.axis = GetALineAndInterpretCommand(pF, gAxis_names, COUNT_OF(gAxis_names));
            break;
        case eGroove_object_rock:
            if (the_groove->object_mode == eMove_controlled || the_groove->object_mode == eMove_absolute) {
                AddFunkGrooveBinding(pRef_offset + GetAnInt(pF), &the_groove->object_data.rock_info.period);
            } else {
                x_0 = GetAFloat(pF);
                the_groove->object_data.rock_info.period = (x_0 == 0.0f) ? 0.0f : (1000.0f / x_0);
            }
            GetThreeFloats(pF,
                &the_groove->object_centre.v[0],
                &the_groove->object_centre.v[1],
                &the_groove->object_centre.v[2]);
            the_groove->object_data.rock_info.axis = GetALineAndInterpretCommand(pF, gAxis_names, COUNT_OF(gAxis_names));
            the_groove->object_data.rock_info.max_angle = GetAFloat(pF);
            break;
        case eGroove_object_throb:
            if (the_groove->object_mode == eMove_controlled || the_groove->object_mode == eMove_absolute) {
                GetThreeInts(pF, &d_0, &d_1, &d_2);
                if (d_0 >= 0) {
                    AddFunkGrooveBinding(pRef_offset + d_0, &the_groove->object_data.throb_info.x_period);
                }
                if (d_1 >= 0) {
                    AddFunkGrooveBinding(pRef_offset + d_1, &the_groove->object_data.throb_info.y_period);
                }
                if (d_2 >= 0) {
                    AddFunkGrooveBinding(pRef_offset + d_2, &the_groove->object_data.throb_info.z_period);
                }
            } else {
                GetThreeFloats(pF, &x_0, &x_1, &x_2);
                the_groove->object_data.throb_info.x_period = (x_0 == 0.0f) ? 0.0f : (1000.0f / x_0);
                the_groove->object_data.throb_info.y_period = (x_1 == 0.0f) ? 0.0f : (1000.0f / x_1);
                the_groove->object_data.throb_info.z_period = (x_2 == 0.0f) ? 0.0f : (1000.0f / x_2);
            }
            GetThreeFloats(pF,
                &the_groove->object_centre.v[0],
                &the_groove->object_centre.v[1],
                &the_groove->object_centre.v[2]);
            GetThreeFloatPercents(
                pF,
                &the_groove->object_data.throb_info.x_magnitude,
                &the_groove->object_data.throb_info.y_magnitude,
                &the_groove->object_data.throb_info.z_magnitude);
            break;
        case eGroove_object_shear:
            if (the_groove->object_mode == eMove_controlled || the_groove->object_mode == eMove_absolute) {
                GetThreeInts(pF, &d_0, &d_1, &d_2);
                if (d_0 >= 0) {
                    AddFunkGrooveBinding(pRef_offset + d_0, &the_groove->object_data.shear_info.x_period);
                }
                if (d_1 >= 0) {
                    AddFunkGrooveBinding(pRef_offset + d_1, &the_groove->object_data.shear_info.y_period);
                }
                if (d_2 >= 0) {
                    AddFunkGrooveBinding(pRef_offset + d_2, &the_groove->object_data.shear_info.z_period);
                }
            } else {
                GetThreeFloats(pF, &x_0, &x_1, &x_2);
                the_groove->object_data.shear_info.x_period = x_0 == 0.0f ? 0.0f : 1000.0 / x_0;
                the_groove->object_data.shear_info.y_period = x_1 == 0.0f ? 0.0f : 1000.0 / x_1;
                the_groove->object_data.shear_info.z_period = x_2 == 0.0f ? 0.0f : 1000.0 / x_2;
            }
            GetThreeFloats(pF,
                &the_groove->object_centre.v[0],
                &the_groove->object_centre.v[1],
                &the_groove->object_centre.v[2]);
            GetThreeFloatPercents(
                pF,
                &the_groove->object_data.shear_info.x_magnitude,
                &the_groove->object_data.shear_info.y_magnitude,
                &the_groove->object_data.shear_info.z_magnitude);

            break;
        default:
            break;
        }
    }
}

// IDA: void __usercall KillGroovadelic(int pOwner@<EAX>)
void KillGroovadelic(int pOwner) {
    int i;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%d)", pOwner);

    if (gGroovidelics_array == NULL) {
        return;
    }
    for (i = 0; i < gGroovidelics_array_size; i++) {
        the_groove = &gGroovidelics_array[i];
        if (the_groove->owner != pOwner) {
            continue;
        }
        if (the_groove->path_mode == eMove_controlled) {
            continue;
        }
        if (the_groove->path_mode == eMove_absolute) {
            continue;
        }
        if (the_groove->object_mode == eMove_controlled) {
            continue;
        }
        if (the_groove->object_mode == eMove_absolute) {
            continue;
        }
        the_groove->owner = -999;
    }
}

// IDA: void __usercall KillFunkotronic(int pOwner@<EAX>)
void KillFunkotronic(int pOwner) {
    int i;
    tFunkotronic_spec* the_funk;
    LOG_TRACE("(%d)", pOwner);

    if (gFunkotronics_array == NULL) {
        return;
    }
    for (i = 0; i < gFunkotronics_array_size; i++) {
        the_funk = &gFunkotronics_array[i];
        if (the_funk->owner != pOwner) {
            continue;
        }
        if (the_funk->matrix_mode == eMove_controlled) {
            continue;
        }
        if (the_funk->matrix_mode == eMove_absolute) {
            continue;
        }
        if (the_funk->lighting_animation_type == eMove_controlled) {
            continue;
        }
        if (the_funk->lighting_animation_type == eMove_absolute) {
            continue;
        }
        if (the_funk->texture_animation_data.frames_info.mode == eMove_controlled && the_funk->texture_animation_type == eTexture_animation_frames) {
            continue;
        }
        the_funk->owner = -999;
    }
}

// IDA: br_uint_32 __usercall DeleteBastards@<EAX>(br_actor *pActor@<EAX>, br_matrix34 *pMatrix@<EDX>, void *pArg@<EBX>)
br_uint_32 DeleteBastards(br_actor* pActor, br_matrix34* pMatrix, void* pArg) {
    int i;
    int parent_already_doomed;
    LOG_TRACE("(%p, %p, %p)", pActor, pMatrix, pArg);

    if ((gAdditional_actors != pActor && (pActor->identifier == NULL || pActor->identifier[0] == '&') && Vector3IsZero((br_vector3*)pMatrix->m[3])) || (pActor->model == NULL && pActor->type == BR_ACTOR_MODEL)) {
        parent_already_doomed = 0;
        for (i = 0; i < gDelete_count; i++) {
            if (gDelete_list[i] == pActor) {
                parent_already_doomed = 1;
                break;
            }
        }
        if (!parent_already_doomed) {
            gDelete_list[gDelete_count] = pActor;
            gDelete_count++;
        }
    }
    return 0;
}

// IDA: void __cdecl DeleteAnyZeroBastards()
void DeleteAnyZeroBastards() {
    int i;
    LOG_TRACE("()");

    gDelete_count = 0;
    DRActorEnumRecurseWithTrans(gAdditional_actors, NULL, DeleteBastards, NULL);
    for (i = 0; i < gDelete_count; i++) {
        BrActorRemove(gDelete_list[i]);
    }
}

// IDA: br_uint_32 __usercall ApplyTransToModels@<EAX>(br_actor *pActor@<EAX>, br_matrix34 *pMatrix@<EDX>, void *pArg@<EBX>)
br_uint_32 ApplyTransToModels(br_actor* pActor, br_matrix34* pMatrix, void* pArg) {
    int i;
    br_vector3 temp_point;
    LOG_TRACE("(%p, %p, %p)", pActor, pMatrix, pArg);

    if (pActor->identifier == NULL || pActor->identifier[0] == '&') {
        return 0;
    }
    if (pActor->model != NULL) {
        for (i = 0; i < pActor->model->nvertices; i++) {
            BrVector3Copy(&temp_point, &pActor->model->vertices[i].p);
            BrMatrix34ApplyP(&pActor->model->vertices[i].p, &temp_point, pMatrix);
        }
        BrModelUpdate(pActor->model, BR_MATU_ALL);
    }
    BrMatrix34Identity(&pActor->t.t.mat);
    pActor->t.type = BR_TRANSFORM_IDENTITY;
    return 0;
}

// IDA: int __usercall FindSpecVolIndex@<EAX>(br_actor *pActor@<EAX>)
int FindSpecVolIndex(br_actor* pActor) {
    int i;
    tSpecial_volume* v;
    LOG_TRACE("(%p)", pActor);

    for (i = 0; i < gProgram_state.special_volume_count; i++) {
        if (gSpec_vol_actors[i] == pActor) {
            return i;
        }
    }
    return -1;
}

// IDA: void __usercall MungeMaterial(br_matrix34 *pMat@<EAX>, br_material *pMat_1@<EDX>, br_material *pMat_2@<EBX>, int pAxis_0@<ECX>, int pAxis_1)
void MungeMaterial(br_matrix34* pMat, br_material* pMat_1, br_material* pMat_2, int pAxis_0, int pAxis_1) {
    LOG_TRACE("(%p, %p, %p, %d, %d)", pMat, pMat_1, pMat_2, pAxis_0, pAxis_1);

    pMat_1->map_transform.m[0][0] = 6.f * BrVector3Length((br_vector3*)pMat->m[pAxis_0]);
    pMat_1->map_transform.m[1][1] = 6.f * BrVector3Length((br_vector3*)pMat->m[pAxis_1]);
    BrMatrix23Copy(&pMat_2->map_transform, &pMat_1->map_transform);
}

// IDA: void __usercall SetSpecVolMatSize(br_actor *pActor@<EAX>)
void SetSpecVolMatSize(br_actor* pActor) {
    br_model* model;
    LOG_TRACE("(%p)", pActor);

    model = pActor->model;
    MungeMaterial(&pActor->t.t.mat, model->faces[5].material, model->faces[17].material, 0, 1);
    MungeMaterial(&pActor->t.t.mat, model->faces[11].material, model->faces[23].material, 1, 2);
    MungeMaterial(&pActor->t.t.mat, model->faces[7].material, model->faces[19].material, 0, 2);
}

// IDA: void __usercall FindInverseAndWorldBox(tSpecial_volume *pSpec@<EAX>)
void FindInverseAndWorldBox(tSpecial_volume* pSpec) {
    br_bounds bnds;
    LOG_TRACE("(%p)", pSpec);

    bnds.min.v[0] = -1.0;
    bnds.min.v[1] = -1.0;
    bnds.min.v[2] = -1.0;
    bnds.max.v[0] = 1.0;
    bnds.max.v[1] = 1.0;
    bnds.max.v[2] = 1.0;
    GetNewBoundingBox(&pSpec->bounds, &bnds, &pSpec->mat);
    BrMatrix34Inverse(&pSpec->inv_mat, &pSpec->mat);
}

// IDA: void __cdecl UpdateSpecVol()
void UpdateSpecVol() {
    int index;
    tSpecial_volume* v;
    LOG_TRACE("()");

    index = FindSpecVolIndex(gLast_actor);
    if (index >= 0) {
        v = &gProgram_state.special_volumes[index];
        BrMatrix34Copy(&v->mat, &gLast_actor->t.t.mat);
        FindInverseAndWorldBox(v);
        SetSpecVolMatSize(gLast_actor);
    }
}

// IDA: void __cdecl SaveSpecialVolumes()
void SaveSpecialVolumes() {
    tPath_name the_path;
    FILE* f;
    int i;
    tSpecial_volume* v;
    LOG_TRACE("()");

    PathCat(the_path, gApplication_path, "SPECSAVE.TXT");
    f = DRfopen(the_path, "wt");
    if (f == NULL) {
        return;
    }
    fprintf(f, "// SPECIAL EFFECTS VOLUMES\n\n");
    fprintf(f, "%d\t\t\t\t// # special effects volumes\n\n", gProgram_state.special_volume_count);
    for (i = 0; i < gProgram_state.special_volume_count; i++) {
        v = &gProgram_state.special_volumes[i];
        if (v->no_mat) {
            fprintf(f, "%s\n", "DEFAULT WATER");
        } else {
            fprintf(f, "NEW IMPROVED!\n");
            fprintf(f, "%.3f, %.3f, %.3f\n", v->mat.m[0][0], v->mat.m[0][1], v->mat.m[0][2]);
            fprintf(f, "%.3f, %.3f, %.3f\n", v->mat.m[1][0], v->mat.m[1][1], v->mat.m[1][2]);
            fprintf(f, "%.3f, %.3f, %.3f\n", v->mat.m[2][0], v->mat.m[2][1], v->mat.m[2][2]);
            fprintf(f, "%.3f, %.3f, %.3f\n", v->mat.m[3][0], v->mat.m[3][1], v->mat.m[3][2]);
        }
        fprintf(f, "%.0f\t\t\t\t// gravity multiplier\n", v->gravity_multiplier);
        fprintf(f, "%.0f\t\t\t\t// viscosity multiplier\n", v->viscosity_multiplier);
        fprintf(f, "%.0f\t\t\t\t// Car damage per millisecond\n", v->car_damage_per_ms);
        fprintf(f, "%.0f\t\t\t\t// Pedestrian damage per millisecond\n", v->ped_damage_per_ms);
        fprintf(f, "%d\t\t\t\t\t// camera effect index\n", v->camera_special_effect_index);
        fprintf(f, "%d\t\t\t\t\t// sky colour\n", v->sky_col);
        fprintf(f, "%s\t\t\t\t// Windscreen material to use\n", (v->screen_material != NULL) ? v->screen_material->identifier : "none");
        fprintf(f, "%d\t\t\t\t\t// Sound ID of entry noise\n", v->entry_noise);
        fprintf(f, "%d\t\t\t\t\t// Sound ID of exit noise\n", v->exit_noise);
        fprintf(f, "%d\t\t\t\t\t// Engine noise index\n", v->engine_noise_index);
        fprintf(f, "%d\t\t\t\t\t// material index\n", v->material_modifier_index);
        fprintf(f, "\n");
    }
    fclose(f);
}

// IDA: void __cdecl SaveAdditionalStuff()
void SaveAdditionalStuff() {
    LOG_TRACE("()");

    if (gSpec_vol_mode) {
        UpdateSpecVol();
        SaveSpecialVolumes();
    } else {
        DeleteAnyZeroBastards();
        if (gLast_actor != NULL) {
            DRActorEnumRecurseWithTrans(gLast_actor, NULL, ApplyTransToModels, NULL);
        }
        BrActorSave(gAdditional_actor_path, gAdditional_actors);
        BrModelSaveMany(gAdditional_model_path, gAdditional_models, gNumber_of_additional_models);
    }
}

// IDA: br_uint_32 __cdecl ProcessMaterials(br_actor *pActor, tPMFM2CB pCallback)
br_uint_32 ProcessMaterials(br_actor* pActor, tPMFM2CB pCallback) {
    LOG_TRACE("(%p, %d)", pActor, pCallback);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl ProcessFaceMaterials2(br_actor *pActor, tPMFM2CB pCallback)
br_uint_32 ProcessFaceMaterials2(br_actor* pActor, tPMFM2CB pCallback) {
    LOG_TRACE("(%p, %d)", pActor, pCallback);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ChangePerspToSubdivCB(br_material *pMaterial@<EAX>)
void ChangePerspToSubdivCB(br_material* pMaterial) {
    LOG_TRACE("(%p)", pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ChangePerspToSubdiv()
void ChangePerspToSubdiv() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ChangeSubdivToPerspCB(br_material *pMaterial@<EAX>)
void ChangeSubdivToPerspCB(br_material* pMaterial) {
    LOG_TRACE("(%p)", pMaterial);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ChangeSubdivToPersp()
void ChangeSubdivToPersp() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl ProcessFaceMaterials(br_actor *pActor, tPMFMCB pCallback)
intptr_t ProcessFaceMaterials(br_actor* pActor, tPMFMCB pCallback) {
    LOG_TRACE("(%p, %d)", pActor, pCallback);

    if (pActor->identifier == NULL || pActor->identifier[0] != '&') {
        if (pActor->type == BR_ACTOR_MODEL && pActor->model != NULL) {
            ProcessModelFaceMaterials(pActor->model, pCallback);
        }
        return BrActorEnum(pActor, (br_actor_enum_cbfn*)ProcessFaceMaterials, pCallback);
    } else {
        return 0;
    }
}

// IDA: int __usercall DRPixelmapHasZeros@<EAX>(br_pixelmap *pm@<EAX>)
int DRPixelmapHasZeros(br_pixelmap* pm) {
    int x;
    int y;
    char* row_ptr;
    char* pp;
    int i;

    if (pm->flags & BR_PMF_NO_ACCESS) {
        return 1;
    }
    row_ptr = (char*)pm->pixels + (pm->row_bytes * pm->base_y) + pm->base_x;
    for (y = 0; y < pm->height; y++) {
        pp = row_ptr;
        for (x = 0; x < pm->width; x++) {
            if (!pp)
                return 1;
            pp++;
        }
        row_ptr += pm->row_bytes;
    }
    return 0;
}

// IDA: int __usercall StorageContainsPixelmap@<EAX>(tBrender_storage *pStorage@<EAX>, br_pixelmap *pMap@<EDX>)
int StorageContainsPixelmap(tBrender_storage* pStorage, br_pixelmap* pMap) {
    int i;

    for (i = 0; i < pStorage->pixelmaps_count; i++) {
        if (pMap == pStorage->pixelmaps[i]) {
            return 1;
        }
    }
    return 0;
}

// IDA: void __usercall HideStoredOpaqueTextures(tBrender_storage *pStorage@<EAX>)
void HideStoredOpaqueTextures(tBrender_storage* pStorage) {
    int i;

    for (i = 0; i < pStorage->materials_count; i++) {
        if (pStorage->materials[i]->colour_map && StorageContainsPixelmap(pStorage, pStorage->materials[i]->colour_map)) {
            if (!DRPixelmapHasZeros(pStorage->materials[i]->colour_map)) {
                pStorage->saved_colour_maps[i] = pStorage->materials[i]->colour_map;
                pStorage->materials[i]->colour_map = NULL;
                pStorage->materials[i]->flags &= 0xFDu;
                BrMaterialUpdate(pStorage->materials[i], BR_MATU_ALL);
            }
        }
    }
}

// IDA: void __usercall RevealStoredTransparentTextures(tBrender_storage *pStorage@<EAX>)
void RevealStoredTransparentTextures(tBrender_storage* pStorage) {
    int i;

    for (i = 0; i < pStorage->materials_count; i++) {
        if (pStorage->saved_colour_maps[i]) {
            if (DRPixelmapHasZeros(pStorage->saved_colour_maps[i])) {
                pStorage->materials[i]->colour_map = pStorage->saved_colour_maps[i];
                pStorage->saved_colour_maps[i] = NULL;
                pStorage->materials[i]->flags |= 2u;
                BrMaterialUpdate(pStorage->materials[i], BR_MATU_ALL);
            }
        }
    }
}

// IDA: void __usercall HideStoredTextures(tBrender_storage *pStorage@<EAX>)
void HideStoredTextures(tBrender_storage* pStorage) {
    int i;

    for (i = 0; i < pStorage->materials_count; i++) {
        if (pStorage->materials[i]->colour_map) {
            if (StorageContainsPixelmap(pStorage, pStorage->materials[i]->colour_map)) {
                pStorage->saved_colour_maps[i] = pStorage->materials[i]->colour_map;
                pStorage->materials[i]->colour_map = NULL;
                pStorage->materials[i]->flags &= 0xFDu;
                BrMaterialUpdate(pStorage->materials[i], BR_MATU_ALL);
            }
        }
    }
}

// IDA: void __usercall RevealStoredTextures(tBrender_storage *pStorage@<EAX>)
void RevealStoredTextures(tBrender_storage* pStorage) {
    int i;

    for (i = 0; i < pStorage->materials_count; i++) {
        if (pStorage->saved_colour_maps[i]) {
            pStorage->materials[i]->colour_map = pStorage->saved_colour_maps[i];
            pStorage->saved_colour_maps[i] = NULL;
            pStorage->materials[i]->flags |= BR_MATF_PRELIT;
            BrMaterialUpdate(pStorage->materials[i], BR_MATU_ALL);
        }
    }
}

// IDA: void __usercall SetCarStorageTexturingLevel(tBrender_storage *pStorage@<EAX>, tCar_texturing_level pNew@<EDX>, tCar_texturing_level pOld@<EBX>)
void SetCarStorageTexturingLevel(tBrender_storage* pStorage, tCar_texturing_level pNew, tCar_texturing_level pOld) {
    LOG_TRACE("(%p, %d, %d)", pStorage, pNew, pOld);

    switch (pNew) {
    case eCTL_none:
        HideStoredTextures(pStorage);
        break;
    case eCTL_transparent:
        switch (pOld) {
        case eCTL_none:
            RevealStoredTransparentTextures(pStorage);
            break;
        case eCTL_full:
            HideStoredOpaqueTextures(pStorage);
            break;
        default:
            break;
        }
        break;
    case eCTL_full:
        RevealStoredTextures(pStorage);
        break;
    default:
        break;
    }
}

// IDA: tCar_texturing_level __cdecl GetCarTexturingLevel()
tCar_texturing_level GetCarTexturingLevel() {
    LOG_TRACE("()");

    return gCar_texturing_level;
}

// IDA: void __usercall SetCarTexturingLevel(tCar_texturing_level pLevel@<EAX>)
void SetCarTexturingLevel(tCar_texturing_level pLevel) {
    LOG_TRACE("(%d)", pLevel);

    if (pLevel != gCar_texturing_level) {
        if (gOur_car_storage_space.models_count != 0) {
            SetCarStorageTexturingLevel(&gOur_car_storage_space, pLevel, gCar_texturing_level);
        }
        if (gTheir_cars_storage_space.models_count != 0) {
            SetCarStorageTexturingLevel(&gTheir_cars_storage_space, pLevel, gCar_texturing_level);
        }
        if (gNet_cars_storage_space.models_count != 0) {
            SetCarStorageTexturingLevel(&gTheir_cars_storage_space, pLevel, gCar_texturing_level);
        }
    }
    gCar_texturing_level = pLevel;
}

// IDA: int __usercall HasThisSuffix@<EAX>(char *pIdent@<EAX>, char *pSuffix@<EDX>)
int HasThisSuffix(char* pIdent, char* pSuffix) {
    size_t len_ident;
    size_t len_suffix;
    LOG_TRACE("(\"%s\", \"%s\")", pIdent, pSuffix);

    len_ident = strlen(pIdent);
    len_suffix = strlen(pSuffix);
    if (pIdent == NULL) {
        return 0;
    }
    if (pIdent < pSuffix) {
        return 0;
    }
    return strcmp(pIdent + len_ident - len_suffix, pSuffix) == 0;
}

// IDA: br_material* __usercall UnsuffixedMaterial@<EAX>(char *pOld_ident@<EAX>, char *pSuffix@<EDX>)
br_material* UnsuffixedMaterial(char* pOld_ident, char* pSuffix) {
    br_material* result;
    int unsuffixed_len;
    char* new_id;
    LOG_TRACE("(\"%s\", \"%s\")", pOld_ident, pSuffix);

    unsuffixed_len = strlen(pOld_ident) - strlen(pSuffix);
    new_id = BrMemAllocate(unsuffixed_len + 1, kMem_new_mat_id_2);
    sprintf(new_id, "%.*s", unsuffixed_len, pOld_ident);
    result = BrMaterialFind(new_id);
    BrMemFree(new_id);
    return result;
}

// IDA: br_material* __usercall RoadUntexToPersp@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* RoadUntexToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    old_mat = pModel->faces[pFace].material;
    if (HasThisSuffix(old_mat->identifier, ".road")) {
        new_mat = UnsuffixedMaterial(old_mat->identifier, ".road");
    } else {
        new_mat = NULL;
    }
    return new_mat;
}

// IDA: br_material* __usercall WallLinearToUntex@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallLinearToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    old_mat = pModel->faces[pFace].material;
    if (HasThisSuffix(old_mat->identifier, ".pwall")) {
        if (old_mat->colour_map != NULL) {
            old_mat->colour_map = NULL;
            BrMaterialUpdate(old_mat, BR_MATU_ALL);
        }
    } else {
        if (!FaceIsRoad(pModel, pFace) && old_mat->identifier != NULL && old_mat->colour_map != NULL) {
            old_mat = SuffixedMaterial(old_mat, ".lwall");
            if (old_mat->colour_map != NULL) {
                old_mat->colour_map = NULL;
                BrMaterialUpdate(old_mat, BR_MATU_ALL);
            }
        } else {
            old_mat = NULL;
        }
    }
    return old_mat;
}

// IDA: br_material* __usercall WallUntexToLinear@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallUntexToLinear(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    old_mat = pModel->faces[pFace].material;
    if (HasThisSuffix(old_mat->identifier, ".lwall")) {
        new_mat = UnsuffixedMaterial(old_mat->identifier, ".lwall");
    } else if (HasThisSuffix(old_mat->identifier, ".pwall")) {
        old_mat->colour_map = UnsuffixedMaterial(old_mat->identifier, ".pwall")->colour_map;
        old_mat->flags &= ~BR_MATF_PERSPECTIVE;
        BrMaterialUpdate(old_mat, BR_MATU_ALL);
        new_mat = NULL;
    } else {
        new_mat = NULL;
    }
    return new_mat;
}

// IDA: br_material* __usercall WallUntexToPersp@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallUntexToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    old_mat = pModel->faces[pFace].material;
    if (HasThisSuffix(old_mat->identifier, ".lwall")) {
        new_mat = UnsuffixedMaterial(old_mat->identifier, ".lwall");
    } else if (HasThisSuffix(old_mat->identifier, ".pwall")) {
        new_mat = UnsuffixedMaterial(old_mat->identifier, ".pwall");
    } else {
        new_mat = NULL;
    }
    return new_mat;
}

// IDA: br_material* __usercall WallLinearToPersp@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallLinearToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    old_mat = pModel->faces[pFace].material;
    if (HasThisSuffix(old_mat->identifier, ".pwall")) {
        new_mat = UnsuffixedMaterial(old_mat->identifier, ".pwall");
    } else {
        new_mat = NULL;
    }
    return new_mat;
}

// IDA: tRoad_texturing_level __cdecl GetRoadTexturingLevel()
tRoad_texturing_level GetRoadTexturingLevel() {
    return gRoad_texturing_level;
}

// IDA: void __usercall SetRoadTexturingLevel(tRoad_texturing_level pLevel@<EAX>)
void SetRoadTexturingLevel(tRoad_texturing_level pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gRoad_texturing_level = pLevel;
}

// IDA: void __usercall ReallySetRoadTexturingLevel(tRoad_texturing_level pLevel@<EAX>)
void ReallySetRoadTexturingLevel(tRoad_texturing_level pLevel) {
    LOG_TRACE("(%d)", pLevel);

    if (pLevel != gRoad_texturing_level) {
        ProcessFaceMaterials(gProgram_state.track_spec.the_actor, (pLevel == eRTL_none) ? RoadUntexToPersp : RoadPerspToUntex);
    }
}

// IDA: tWall_texturing_level __cdecl GetWallTexturingLevel()
tWall_texturing_level GetWallTexturingLevel() {
    LOG_TRACE("()");

    return gWall_texturing_level;
}

// IDA: void __usercall SetWallTexturingLevel(tWall_texturing_level pLevel@<EAX>)
void SetWallTexturingLevel(tWall_texturing_level pLevel) {
    LOG_TRACE("(%d)", pLevel);

    gWall_texturing_level = pLevel;
}

// IDA: void __usercall ReallySetWallTexturingLevel(tWall_texturing_level pLevel@<EAX>)
void ReallySetWallTexturingLevel(tWall_texturing_level pLevel) {
    static tPMFMCB* tweaker[3][3] = {
        {
            NULL,
            WallUntexToLinear,
            WallUntexToPersp,
        },
        {
            WallLinearToUntex,
            NULL,
            WallLinearToPersp,
        },
        {
            WallPerspToUntex,
            WallPerspToLinear,
            NULL,
        },
    };
    LOG_TRACE("(%d)", pLevel);

    if (gWall_texturing_level != pLevel) {
        ProcessFaceMaterials(gProgram_state.track_spec.the_actor, tweaker[gWall_texturing_level][pLevel]);
    }
}

// IDA: br_material* __usercall DisposeSuffixedMaterials@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* DisposeSuffixedMaterials(br_model* pModel, tU16 pFace) {
    size_t max_suffix_len;
    br_material* mat;
    br_material* victim;
    static char* suffixes[3] = { ".road", ".pwall", ".lwall" };
    int s;
    char* id;
    LOG_TRACE("(%p, %d)", pModel, pFace);

    mat = pModel->faces[pFace].material;
    if (mat->identifier == NULL) {
        return NULL;
    }
    max_suffix_len = 0;
    for (s = 0; s < COUNT_OF(suffixes); s++) {
        if (max_suffix_len < strlen(suffixes[s])) {
            max_suffix_len = strlen(suffixes[s]);
        }
    }
    id = BrMemAllocate(strlen(mat->identifier) + max_suffix_len + 1, kMem_new_mat_id_3);
    for (s = 0; s < COUNT_OF(suffixes); s++) {
        sprintf(id, "%s%s", mat->identifier, suffixes[s]);
        victim = BrMaterialFind(id);
        if (victim != NULL) {
            BrMaterialRemove(victim);
            BrMaterialFree(victim);
        }
    }
    return NULL;
}

// IDA: void __cdecl DisposeTexturingMaterials()
void DisposeTexturingMaterials() {
    LOG_TRACE("()");

    switch (gWall_texturing_level) {
    case eWTL_linear:
        ProcessFaceMaterials(gProgram_state.track_spec.the_actor, WallLinearToPersp);
        break;
    case eWTL_none:
        ProcessFaceMaterials(gProgram_state.track_spec.the_actor, WallUntexToPersp);
        break;
    default:
        break;
    }

    switch (gRoad_texturing_level) {
    case eRTL_none:
        ProcessFaceMaterials(gProgram_state.track_spec.the_actor, RoadUntexToPersp);
        break;
    default:
        break;
    }

    if (gWall_texturing_level != eWTL_full || gRoad_texturing_level != eRTL_full) {
        ProcessFaceMaterials(gProgram_state.track_spec.the_actor, DisposeSuffixedMaterials);
    }
}

// IDA: br_uint_32 __cdecl SetAccessoryRenderingCB(br_actor *pActor, void *pFlag)
intptr_t SetAccessoryRenderingCB(br_actor* pActor, void* pFlag) {
    if (pActor->identifier && *pActor->identifier == '&') {
        pActor->render_style = *(br_uint_8*)pFlag;
    }
    return 0;
}

// IDA: void __usercall SetAccessoryRendering(int pOn@<EAX>)
void SetAccessoryRendering(int pOn) {
    int style;

    LOG_TRACE("(%d)", pOn);

    if (gTrack_actor) {
        if (pOn) {
            style = BR_RSTYLE_FACES;
        } else {
            style = BR_RSTYLE_NONE;
        }
        DRActorEnumRecurse(gTrack_actor, (br_actor_enum_cbfn*)SetAccessoryRenderingCB, &style);
    }
    gRendering_accessories = pOn;
}

// IDA: int __cdecl GetAccessoryRendering()
int GetAccessoryRendering() {
    LOG_TRACE("()");

    return gRendering_accessories;
}

// IDA: void __usercall SetCarSimplificationLevel(int pLevel@<EAX>)
void SetCarSimplificationLevel(int pLevel) {
    LOG_TRACE("(%d)", pLevel);

    gCar_simplification_level = pLevel;
}

// IDA: int __cdecl GetCarSimplificationLevel()
int GetCarSimplificationLevel() {
    LOG_TRACE("()");

    return gCar_simplification_level;
}

// IDA: void __usercall ParseSpecialVolume(FILE *pF@<EAX>, tSpecial_volume *pSpec@<EDX>, char *pScreen_name_str@<EBX>)
void ParseSpecialVolume(FILE* pF, tSpecial_volume* pSpec, char* pScreen_name_str) {
    char s[256];
    pSpec->gravity_multiplier = GetAScalar(pF);
    pSpec->viscosity_multiplier = GetAScalar(pF);
    pSpec->car_damage_per_ms = GetAScalar(pF);
    pSpec->ped_damage_per_ms = GetAScalar(pF);
    pSpec->camera_special_effect_index = GetAnInt(pF);
    pSpec->sky_col = GetAnInt(pF);

    GetAString(pF, s);
    if (pScreen_name_str) {
        strcpy(pScreen_name_str, s);
    } else {
        pSpec->screen_material = BrMaterialFind(s);
    }
    pSpec->entry_noise = GetAnInt(pF);
    pSpec->exit_noise = GetAnInt(pF);
    pSpec->engine_noise_index = GetAnInt(pF);
    pSpec->material_modifier_index = GetAnInt(pF);
}

// IDA: void __usercall AddExceptionToList(tException_list *pDst@<EAX>, tException_list pNew@<EDX>)
void AddExceptionToList(tException_list* pDst, tException_list pNew) {
    LOG_TRACE("(%p, %d)", pDst, pNew);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoadExceptionsFile(char *pName@<EAX>)
void LoadExceptionsFile(char* pName) {
    FILE* f;
    char line[256];
    char* tok;
    int file_version;
    tException_list e;
    char delimiters[4];
    LOG_TRACE("(\"%s\")", pName);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoadExceptionsFileForTrack(char *pTrack_file_name@<EAX>)
void LoadExceptionsFileForTrack(char* pTrack_file_name) {
    tPath_name exceptions_file_name;
    LOG_TRACE("(\"%s\")", pTrack_file_name);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl FreeExceptions()
void FreeExceptions() {
    tException_list list;
    tException_list next;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoadTrack(char *pFile_name@<EAX>, tTrack_spec *pTrack_spec@<EDX>, tRace_info *pRace_info@<EBX>)
void LoadTrack(char* pFile_name, tTrack_spec* pTrack_spec, tRace_info* pRace_info) {
    char temp_name[14];
    FILE* f;
    FILE* non_car_f;
    FILE* g;
    int i;
    int j;
    int k;
    int group;
    int needs_updating;
    int killed_sky;
    int line_count;
    int ped_subs_index;
    int sl;
    int num_materials;
    int count;
    int num_non_cars;
    int style;
    int cp_rect_w[2];
    int cp_rect_h[2];
    tPath_name the_path;
    tPath_name general_file_path;
    char s[256];
    char* str;
    float temp_float;
    br_actor* new_root;
    br_vector3 a;
    br_vector3 b;
    tU16 sky_pixels_high;
    tNon_car_spec* non_car;
    tSpecial_volume* spec;
    br_vector3 p[3];
    br_vector3 v1;
    br_vector3 v2;
    br_vector3 temp_v;
    br_bounds temp_bounds;
    tPed_subs* ped_subs;
    br_pixelmap* sky;
    br_material* material;
    LOG_TRACE("(\"%s\", %p, %p)", pFile_name, pTrack_spec, pRace_info);

    killed_sky = 0;
    PathCat(the_path, gApplication_path, "RACES");
    PathCat(the_path, the_path, pFile_name);
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        FatalError(kFatalError_OpenRacesFile);
    }
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    if (strcmp(str, "VERSION") == 0) {
        str = strtok(NULL, "\t ,/");
        sscanf(str, "%d", &gRace_file_version);
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
    } else {
        gRace_file_version = 0;
    }
    sscanf(str, "%f", &temp_float);
    pRace_info->initial_position.v[0] = temp_float;
    str = strtok(0, "\t ,/");
    sscanf(str, "%f", &temp_float);
    pRace_info->initial_position.v[1] = temp_float;
    str = strtok(0, "\t ,/");
    sscanf(str, "%f", &temp_float);
    pRace_info->initial_position.v[2] = temp_float;
    PossibleService();
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    sscanf(str, "%f", &temp_float);
    pRace_info->initial_yaw = temp_float;
    GetThreeInts(f, pRace_info->initial_timer, &pRace_info->initial_timer[1], &pRace_info->initial_timer[2]);
    pRace_info->total_laps = GetAnInt(f);
    GetThreeInts(f, &pRace_info->bonus_score[0][0], &pRace_info->bonus_score[0][1], &pRace_info->bonus_score[0][2]);
    GetThreeInts(f, &pRace_info->bonus_score[1][0], &pRace_info->bonus_score[1][1], &pRace_info->bonus_score[1][2]);
    GetThreeInts(f, &pRace_info->bonus_score[2][0], &pRace_info->bonus_score[2][1], &pRace_info->bonus_score[2][2]);
    if (gRace_file_version > 1) {
        GetPairOfInts(f, &cp_rect_w[0], &cp_rect_w[1]);
        GetPairOfInts(f, &cp_rect_h[0], &cp_rect_h[1]);
    }
    PossibleService();
    pRace_info->check_point_count = GetAnInt(f);
    for (i = 0; i < pRace_info->check_point_count; i++) {
        PossibleService();
        if (gProgram_state.sausage_eater_mode) {
            GetALineAndDontArgue(f, s);
            GetThreeInts(
                f,
                &pRace_info->checkpoints[i].time_value[0],
                &pRace_info->checkpoints[i].time_value[1],
                &pRace_info->checkpoints[i].time_value[2]);
        } else {
            GetThreeInts(
                f,
                &pRace_info->checkpoints[i].time_value[0],
                &pRace_info->checkpoints[i].time_value[1],
                &pRace_info->checkpoints[i].time_value[2]);
            GetALineAndDontArgue(f, s);
        }
        pRace_info->checkpoints[i].quad_count = GetAnInt(f);
        for (j = 0; j < pRace_info->checkpoints[i].quad_count; j++) {
            for (k = 0; k < 4; ++k) {
                GetThreeScalars(
                    f,
                    &pRace_info->checkpoints[i].vertices[j][k].v[0],
                    &pRace_info->checkpoints[i].vertices[j][k].v[1],
                    &pRace_info->checkpoints[i].vertices[j][k].v[2]);
            }
            p[0] = pRace_info->checkpoints[i].vertices[j][0];
            p[1] = pRace_info->checkpoints[i].vertices[j][1];
            p[2] = pRace_info->checkpoints[i].vertices[j][2];
            pRace_info->checkpoints[i].normal[j].v[0] = (p[2].v[2] - p[0].v[2]) * (p[1].v[1] - p[0].v[1]) - (p[1].v[2] - p[0].v[2]) * (p[2].v[1] - p[0].v[1]);
            pRace_info->checkpoints[i].normal[j].v[1] = (p[1].v[2] - p[0].v[2]) * (p[2].v[0] - p[0].v[0]) - (p[2].v[2] - p[0].v[2]) * (p[1].v[0] - p[0].v[0]);
            pRace_info->checkpoints[i].normal[j].v[2] = (p[2].v[1] - p[0].v[1]) * (p[1].v[0] - p[0].v[0]) - (p[1].v[1] - p[0].v[1]) * (p[2].v[0] - p[0].v[0]);
        }
        if (gRace_file_version > 0) {
            if (gRace_file_version == 1) {
                GetPairOfInts(f, pRace_info->checkpoints[i].map_left, &pRace_info->checkpoints[i].map_left[1]);
                GetPairOfInts(f, pRace_info->checkpoints[i].map_top, &pRace_info->checkpoints[i].map_top[1]);
                GetPairOfInts(f, pRace_info->checkpoints[i].map_right, &pRace_info->checkpoints[i].map_right[1]);
                GetPairOfInts(f, pRace_info->checkpoints[i].map_bottom, &pRace_info->checkpoints[i].map_bottom[1]);
            } else {
                GetPairOfInts(f, pRace_info->checkpoints[i].map_left, &pRace_info->checkpoints[i].map_left[1]);
                GetPairOfInts(f, pRace_info->checkpoints[i].map_top, &pRace_info->checkpoints[i].map_top[1]);
                pRace_info->checkpoints[i].map_right[0] = cp_rect_w[0] + pRace_info->checkpoints[i].map_left[0];
                pRace_info->checkpoints[i].map_right[1] = cp_rect_w[0] + pRace_info->checkpoints[i].map_left[1];
                pRace_info->checkpoints[i].map_bottom[0] = cp_rect_h[0] + pRace_info->checkpoints[i].map_top[0];
                pRace_info->checkpoints[i].map_bottom[1] = cp_rect_h[0] + pRace_info->checkpoints[i].map_top[1];
            }
        }
    }
    if (gRace_file_version <= 3) {
        LoadSomePixelmaps(&gTrack_storage_space, f);
    } else if (gAusterity_mode) {
        SkipNLines(f);
        LoadSomePixelmaps(&gTrack_storage_space, f);
    } else {
        LoadSomePixelmaps(&gTrack_storage_space, f);
        SkipNLines(f);
    }
    LoadSomeShadeTables(&gTrack_storage_space, f);
    if (gRace_file_version <= 3) {
        LoadSomeMaterials(&gTrack_storage_space, f);
    } else if (gAusterity_mode) {
        SkipNLines(f);
        LoadSomeMaterials(&gTrack_storage_space, f);
    } else {
        LoadSomeMaterials(&gTrack_storage_space, f);
        SkipNLines(f);
    }
    for (i = 0; gTrack_storage_space.materials_count > i; ++i) {
        PossibleService();
        if (gTrack_storage_space.materials[i]->flags & (BR_MATF_LIGHT | BR_MATF_PRELIT | BR_MATF_SMOOTH)) {
            gTrack_storage_space.materials[i]->flags &= ~(BR_MATF_LIGHT | BR_MATF_PRELIT | BR_MATF_SMOOTH);
            if (gTrack_storage_space.materials[i]->flags & BR_MATF_TWO_SIDED) {
                gTrack_storage_space.materials[i]->user = DOUBLESIDED_USER_FLAG;
                gTrack_storage_space.materials[i]->flags &= ~BR_MATF_TWO_SIDED;
            }
            BrMaterialUpdate(gTrack_storage_space.materials[i], BR_MATU_RENDERING);
        }
    }
    if (gRace_file_version <= 5) {
        LoadSomeTrackModels(&gTrack_storage_space, f);
    } else if (gAusterity_mode) {
        SkipNLines(f);
        LoadSomeTrackModels(&gTrack_storage_space, f);
    } else {
        LoadSomeTrackModels(&gTrack_storage_space, f);
        SkipNLines(f);
    }

    PrintMemoryDump(0, "JUST LOADED IN TEXTURES/MATS/MODELS FOR TRACK");
    if (gRace_file_version <= 5) {
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        PathCat(the_path, gApplication_path, "ACTORS");
        PathCat(the_path, the_path, str);
    } else if (gAusterity_mode) {
        GetALineAndDontArgue(f, s);
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        PathCat(the_path, gApplication_path, "ACTORS");
        PathCat(the_path, the_path, str);
    } else {
        GetALineAndDontArgue(f, s);
        str = strtok(s, "\t ,/");
        PathCat(the_path, gApplication_path, "ACTORS");
        PathCat(the_path, the_path, str);
        GetALineAndDontArgue(f, s);
    }
    pTrack_spec->the_actor = BrActorLoad(the_path);
    if (gRace_file_version <= 6) {
        gDefault_blend_pc = 75;
    } else {
        GetAString(f, s);
        if (!sscanf(s, "%d", &gDefault_blend_pc) || gDefault_blend_pc < 0 || gDefault_blend_pc > 100) {
            PDFatalError("Wanted a default blend percentage. Didn't get one. Old data file?");
        }
    }
    PossibleService();
    ExtractColumns(pTrack_spec);
    for (i = 0; gTrack_storage_space.models_count > i; ++i) {
        PossibleService();
        if (gTrack_storage_space.models[i] && gTrack_storage_space.models[i]->flags & 0x82) {
            gTrack_storage_space.models[i]->flags &= 0xFF7Du;
            for (group = 0; group < V11MODEL(gTrack_storage_space.models[i])->ngroups; group++) {
                int f = V11MODEL(gTrack_storage_space.models[i])->groups[group].face_user[0];
                material = gTrack_storage_space.models[i]->faces[f].material;
                V11MODEL(gTrack_storage_space.models[i])->groups[group].face_colours_material = material;
                if (material && !material->index_shade) {
                    material->index_shade = BrTableFind("DRRENDER.TAB");
                    BrMaterialUpdate(material, 0x7FFFu);
                }
            }
            DodgyModelUpdate(gTrack_storage_space.models[i]);
        }
    }
    PrintMemoryDump(0, "JUST LOADED IN TRACK ACTOR AND PROCESSED COLUMNS");
    gTrack_actor = pTrack_spec->the_actor;
    if (!gRendering_accessories && !gNet_mode) {
        PossibleService();
        style = BR_RSTYLE_NONE;
        DRActorEnumRecurse(gTrack_actor, (br_actor_enum_cbfn*)SetAccessoryRenderingCB, &style);
    }
    BrActorAdd(gUniverse_actor, pTrack_spec->the_actor);
    GetALineAndDontArgue(f, s);
    str = strtok(s, "\t ,/");
    str = strtok(str, ".");
    strcat(str, ".DAT");
    PathCat(gAdditional_model_path, gApplication_path, "MODELS");
    PathCat(gAdditional_model_path, gAdditional_model_path, str);
    gNumber_of_additional_models = 0;
    PossibleService();
    str = strtok(s, "\t ,/");
    str = strtok(str, ".");
    strcat(str, ".ACT");
    PathCat(gAdditional_actor_path, gApplication_path, "ACTORS");
    PathCat(gAdditional_actor_path, gAdditional_actor_path, str);
    gAdditional_actors = BrActorAllocate(0, 0);
    BrActorAdd(gUniverse_actor, gAdditional_actors);
    gLast_actor = NULL;
    SaveAdditionalStuff();
    GetAString(f, s);
    sky = BrMapFind(s);
    if (gAusterity_mode && sky) {
        for (i = 0; gTrack_storage_space.pixelmaps_count > i; ++i) {
            if (gTrack_storage_space.pixelmaps[i] == sky) {
                BrMapRemove(gTrack_storage_space.pixelmaps[i]);
                BrPixelmapFree(gTrack_storage_space.pixelmaps[i]);
                gTrack_storage_space.pixelmaps[i] = gTrack_storage_space.pixelmaps[gTrack_storage_space.pixelmaps_count - 1];
                gTrack_storage_space.pixelmaps_count--;
                break;
            }
        }
        sky = 0;
        killed_sky = 1;
    }
    gProgram_state.default_depth_effect.sky_texture = sky;
    if (sky) {
        sky_pixels_high = gProgram_state.default_depth_effect.sky_texture->height;
    } else {
        sky_pixels_high = 100;
    }
    PossibleService();

    gSky_image_width = (360.0 / BrDegreeToAngle(GetAnInt(f)));
    gSky_image_height = BrDegreeToAngle(GetAScalar(f));
    gSky_image_underground = gSky_image_height * (sky_pixels_high - GetAnInt(f)) / sky_pixels_high;

    MungeRearviewSky();
    PossibleService();

    gProgram_state.default_depth_effect.type = GetALineAndInterpretCommand(f, gDepth_effect_names, 2);
    GetPairOfInts(f, &gProgram_state.default_depth_effect.start, &gProgram_state.default_depth_effect.end);
    if (killed_sky && gProgram_state.default_depth_effect.type != eDepth_effect_fog) {
        gProgram_state.default_depth_effect.type = eDepth_effect_fog;
        gProgram_state.default_depth_effect.start = 7;
        gProgram_state.default_depth_effect.end = 0;
    }
    InstantDepthChange(
        gProgram_state.default_depth_effect.type,
        gProgram_state.default_depth_effect.sky_texture,
        gProgram_state.default_depth_effect.start,
        gProgram_state.default_depth_effect.end);
    gSwap_sky_texture = 0;
    if (!GetSkyTextureOn()) {
        ToggleSkyQuietly();
    }
    gSwap_depth_effect_type = -1;
    if (!GetDepthCueingOn()) {
        ToggleDepthCueingQuietly();
    }
    PossibleService();
    gDefault_engine_noise_index = GetAnInt(f);
    gDefault_water_spec_vol = &gDefault_default_water_spec_vol;
    gProgram_state.special_volume_count = GetAnInt(f);
    if (gProgram_state.special_volume_count) {
        gProgram_state.special_volumes = BrMemAllocate(sizeof(tSpecial_volume) * gProgram_state.special_volume_count, kMem_special_volume);
        i = 0;
        spec = gProgram_state.special_volumes;
        for (i = 0; i < gProgram_state.special_volume_count; i++) {
            PossibleService();
            spec->no_mat = 0;
            GetALineAndDontArgue(f, s);
            if (strcmp(s, "NEW IMPROVED!") == 0) {
                GetThreeScalars(f, &spec->mat.m[0][0], &spec->mat.m[0][1], &spec->mat.m[0][2]);
                GetThreeScalars(f, &spec->mat.m[1][0], &spec->mat.m[1][1], &spec->mat.m[1][2]);
                GetThreeScalars(f, &spec->mat.m[2][0], &spec->mat.m[2][1], &spec->mat.m[2][2]);
                GetThreeScalars(f, &spec->mat.m[3][0], &spec->mat.m[3][1], &spec->mat.m[3][2]);
                FindInverseAndWorldBox(spec);
                ParseSpecialVolume(f, spec, NULL);
            } else if (strcmp(s, "DEFAULT WATER") == 0) {
                spec->bounds.min.v[0] = 0.0;
                spec->bounds.min.v[1] = 0.0;
                spec->bounds.min.v[2] = 0.0;
                spec->bounds.max.v[0] = 0.0;
                spec->bounds.max.v[1] = 0.0;
                spec->bounds.max.v[2] = 0.0;
                ParseSpecialVolume(f, spec, NULL);
                gDefault_water_spec_vol = spec;
                spec->no_mat = 1;
            } else {
                TELL_ME_IF_WE_PASS_THIS_WAY();
                spec->no_mat = 0;
                str = strtok(s, "\t ,/");
                sscanf(str, "%f", &spec->bounds.min.v[0]);
                str = strtok(0, "\t ,/");
                sscanf(str, "%f", &spec->bounds.min.v[1]);
                str = strtok(0, "\t ,/");
                sscanf(str, "%f", &spec->bounds.min.v[2]);
                GetThreeScalars(f, &spec->bounds.max.v[0], &spec->bounds.max.v[1], &spec->bounds.max.v[2]);
                BrMatrix34Identity(&spec->mat);
                for (k = 0; k < 3; ++k) {
                    // FIXME: not 100% sure this is correct
                    spec->mat.m[3][k] = (spec->bounds.max.v[k] + spec->bounds.min.v[k]) / 2.f;
                    spec->mat.m[k][k] = spec->bounds.max.v[k] - spec->bounds.min.v[k];
                }
                ParseSpecialVolume(f, spec, NULL);
            }
            spec++;
        }
    }
    GetAString(f, s);
    gProgram_state.standard_screen = BrMaterialFind(s);
    GetAString(f, s);
    gProgram_state.standard_screen_dark = BrMaterialFind(s);
    GetAString(f, s);
    gProgram_state.standard_screen_fog = BrMaterialFind(s);
    gProgram_state.special_screens_count = GetAnInt(f);
    if (gProgram_state.special_screens_count) {
        gProgram_state.special_screens = BrMemAllocate(sizeof(tSpecial_screen) * gProgram_state.special_screens_count, kMem_special_screen);
        for (i = 0; i < gProgram_state.special_screens_count; i++) {
            GetFourScalars(
                f,
                &gProgram_state.special_screens[i].min_x,
                &gProgram_state.special_screens[i].min_z,
                &gProgram_state.special_screens[i].max_x,
                &gProgram_state.special_screens[i].max_z);
            GetAString(f, s);
            material = BrMaterialFind(s);
            gProgram_state.special_screens[i].material = material;
        }
    }
    PossibleService();
    GetAString(f, s);
    pRace_info->map_image = LoadPixelmap(s);
    PrintMemoryDump(0, "JUST LOADING SKY/SPEC VOLS/SCREENS/MAP");
    for (i = 0; i < 4; ++i) {
        GetThreeScalars(
            f,
            &pRace_info->map_transformation.m[i][0],
            &pRace_info->map_transformation.m[i][1],
            &pRace_info->map_transformation.m[i][2]);
    }
    GetALineAndDontArgue(f, s);
    AddFunkotronics(f, -2, 720);
    GetALineAndDontArgue(f, s);
    AddGroovidelics(f, -2, gUniverse_actor, 720, 0);
    PossibleService();
    PrintMemoryDump(0, "JUST LOADING IN FUNKS AND GROOVES");
    ped_subs = NULL;
    count = 0;
    if (gRace_file_version > 3) {
        line_count = GetAnInt(f);
        if (gAusterity_mode) {
            if (line_count >= 0) {
                PathCat(the_path, gApplication_path, "PEDSUBS.TXT");
                g = DRfopen(the_path, "rt");
                if (g == NULL) {
                    FatalError(kFatalError_OpenRacesFile);
                }
                for (i = 0; i < line_count; ++i) {
                    SkipNLines(g);
                }
                count = GetAnInt(g);
                ped_subs = BrMemAllocate(sizeof(tPed_subs) * count, kMem_misc);
                for (ped_subs_index = 0; ped_subs_index < count; ped_subs_index++) {
                    GetPairOfInts(g, &ped_subs[i].orig, &ped_subs[i].subs);
                }
                fclose(g);
            }
        }
    }
    PossibleService();
    LoadInPedestrians(f, count, ped_subs);
    if (ped_subs != NULL) {
        BrMemFree(ped_subs);
    }
    PrintMemoryDump(0, "JUST LOADED IN PEDS");
    LoadInOppoPaths(f);
    PrintMemoryDump(0, "JUST LOADED IN OPPO PATHS");
    num_materials = GetAnInt(f);
    for (i = 0; i < num_materials; i++) {
        PossibleService();
        pRace_info->material_modifiers[i].car_wall_friction = GetAScalar(f);
        pRace_info->material_modifiers[i].tyre_road_friction = GetAScalar(f);
        pRace_info->material_modifiers[i].down_force = GetAScalar(f);
        pRace_info->material_modifiers[i].bumpiness = GetAScalar(f);
        pRace_info->material_modifiers[i].tyre_noise_index = GetAnInt(f);
        pRace_info->material_modifiers[i].crash_noise_index = GetAnInt(f);
        pRace_info->material_modifiers[i].scrape_noise_index = GetAnInt(f);
        pRace_info->material_modifiers[i].sparkiness = GetAScalar(f);
        pRace_info->material_modifiers[i].smoke_type = GetAnInt(f);
        GetAString(f, s);
        str = strtok(s, ".");

        if (!strcmp(s, "none") || !strcmp(s, "NONE") || !strcmp(s, "0") || !strcmp(s, "1")) {
            pRace_info->material_modifiers[i].skid_mark_material = NULL;
        } else {
            sl = strlen(str);
            strcat(str, ".PIX");
            LoadSinglePixelmap(&gTrack_storage_space, str);
            str[sl] = 0;
            strcat(str, ".MAT");
            material = LoadSingleMaterial(&gTrack_storage_space, str);
            pRace_info->material_modifiers[i].skid_mark_material = material;
        }
    }
    for (i = num_materials; i < 10; ++i) {
        pRace_info->material_modifiers[i].car_wall_friction = 1.0;
        pRace_info->material_modifiers[i].tyre_road_friction = 1.0;
        pRace_info->material_modifiers[i].down_force = 1.0;
        pRace_info->material_modifiers[i].bumpiness = 0.0;
        pRace_info->material_modifiers[i].tyre_noise_index = 0;
        pRace_info->material_modifiers[i].crash_noise_index = 0;
        pRace_info->material_modifiers[i].scrape_noise_index = 0;
        pRace_info->material_modifiers[i].sparkiness = 1.0;
        pRace_info->material_modifiers[i].smoke_type = 1;
        pRace_info->material_modifiers[i].skid_mark_material = 0;
    }
    i = 10;
    pRace_info->material_modifiers[i].car_wall_friction = 1.0;
    pRace_info->material_modifiers[i].tyre_road_friction = 1.0;
    pRace_info->material_modifiers[i].down_force = 0.0;
    pRace_info->material_modifiers[i].bumpiness = 0.0;
    pRace_info->material_modifiers[i].tyre_noise_index = -1;
    pRace_info->material_modifiers[i].crash_noise_index = 0;
    pRace_info->material_modifiers[i].scrape_noise_index = 0;
    pRace_info->material_modifiers[i].sparkiness = 0.0;
    pRace_info->material_modifiers[i].smoke_type = 1;
    pRace_info->material_modifiers[i].skid_mark_material = 0;
    gDefault_water_spec_vol->material_modifier_index = i;
    num_non_cars = GetAnInt(f);
    non_car = BrMemCalloc(num_non_cars + 5, sizeof(tNon_car_spec), kMem_non_car_spec);
    if (!non_car && num_non_cars) {
        FatalError(kFatalError_OpenRacesFile);
    }
    gProgram_state.non_cars = non_car;
    gProgram_state.num_non_car_spaces = num_non_cars + NONCAR_UNUSED_SLOTS;
    for (i = 0; i < COUNT_OF(gNon_car_spec_list); ++i) {
        gNon_car_spec_list[i] = '\0';
    }
    for (i = 0; i < NONCAR_UNUSED_SLOTS; i++) {
        non_car->collision_info.driver = eDriver_non_car_unused_slot;
        non_car++;
    }
    i = 0;
    for (i = 0; i < num_non_cars; i++) {
        PossibleService();
        GetAString(f, s);
        PathCat(the_path, gApplication_path, "NONCARS");
        PathCat(the_path, the_path, s);
        non_car_f = DRfopen(the_path, "rt");
        if (non_car_f == NULL) {
            FatalError(kFatalError_Open_S, the_path);
        }
        ReadNonCarMechanicsData(non_car_f, non_car);
        PossibleService();
        ReadShrapnelMaterials(non_car_f, &non_car->collision_info);
        gNon_car_spec_list[non_car->collision_info.index] = i + 1;
        fclose(non_car_f);
        non_car++;
    }
    GetSmokeShadeTables(f);
    pRace_info->number_of_net_start_points = GetAnInt(f);
    for (i = 0; i < pRace_info->number_of_net_start_points; i++) {
        GetThreeScalars(
            f,
            &pRace_info->net_starts[i].pos.v[0],
            &pRace_info->net_starts[i].pos.v[1],
            &pRace_info->net_starts[i].pos.v[2]);
        pRace_info->net_starts[i].yaw = GetAScalar(f);
    }
    if (gRace_file_version <= 2) {
        LoadInKevStuff(NULL);
    } else {
        LoadInKevStuff(f);
    }
    if (gRace_file_version < 5) {
        gYon_multiplier = 1.0;
    } else {
        gYon_multiplier = GetAScalar(f);
    }
    GetAString(f, s);
    if (strcmp(s, pFile_name) != 0) {
        FatalError(kFatalError_FileCorrupt_S, pFile_name);
    }
    fclose(f);
}

// IDA: br_uint_32 __cdecl RemoveBounds(br_actor *pActor, void *pArg)
intptr_t RemoveBounds(br_actor* pActor, void* pArg) {
    LOG_TRACE("(%p, %p)", pActor, pArg);

    if (pActor->type == BR_ACTOR_BOUNDS || pActor->type == BR_ACTOR_BOUNDS_CORRECT) {
        BrResFree(pActor->type_data);
        pActor->type_data = NULL;
    }
    return 0;
}

// IDA: void __usercall RemoveBoundsStructures(br_actor *pActor@<EAX>)
void RemoveBoundsStructures(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);

    DRActorEnumRecurse(pActor, RemoveBounds, NULL);
}

// IDA: void __usercall FreeTrack(tTrack_spec *pTrack_spec@<EAX>)
void FreeTrack(tTrack_spec* pTrack_spec) {
    int i;
    tNon_car_spec* non_car;
    LOG_TRACE("(%p)", pTrack_spec);

    if (gAdditional_actors != NULL) {
        BrActorRemove(gAdditional_actors);
        BrActorFree(gAdditional_actors);
    }
    PossibleService();
    DisposeTexturingMaterials();
    PossibleService();
    DisposeColumns(pTrack_spec);
    PossibleService();
    RemoveBoundsStructures(pTrack_spec->the_actor);
    BrActorRemove(pTrack_spec->the_actor);
    BrActorFree(pTrack_spec->the_actor);
    pTrack_spec->the_actor = NULL;
    gTrack_actor = NULL;
    PossibleService();
    DisposeFunkotronics(-2);
    PossibleService();
    ClearOutStorageSpace(&gTrack_storage_space);
    PossibleService();
    DisposeGroovidelics(-2);
    PossibleService();
    DisposeOpponentPaths();
    PossibleService();
    DisposeKevStuff();
    PossibleService();
    if (gCurrent_race.map_image != NULL) {
        BrPixelmapFree(gCurrent_race.map_image);
    }
    if (gProgram_state.special_screens_count != 0) {
        BrMemFree(gProgram_state.special_screens);
    }
    PossibleService();
    for (i = 0, non_car = gProgram_state.non_cars; i < gProgram_state.num_non_car_spaces; i++, non_car++) {
        PossibleService();
        if (non_car->collision_info.driver == eDriver_non_car && non_car->collision_info.car_master_actor != NULL) {
            BrActorRemove(non_car->collision_info.car_master_actor);
            BrActorFree(non_car->collision_info.car_master_actor);
        }
    }
    if (gProgram_state.non_cars != NULL) {
        BrMemFree(gProgram_state.non_cars);
    }
    FreeSmokeShadeTables();
}

// IDA: void __usercall ProcessTrack(br_actor *pWorld@<EAX>, tTrack_spec *pTrack_spec@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world_transform@<ECX>, int pRender_blends)
void ProcessTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, int pRender_blends) {
    LOG_TRACE("(%p, %p, %p, %p, %d)", pWorld, pTrack_spec, pCamera, pCamera_to_world_transform, pRender_blends);

    RenderTrack(pWorld, pTrack_spec, pCamera, pCamera_to_world_transform, pRender_blends);
    if (gAdditional_actors) {
        if (!pRender_blends) {
            BrZbSceneRenderAdd(gAdditional_actors);
        }
    }
}

// IDA: br_scalar __cdecl NormaliseDegreeAngle(br_scalar pAngle)
br_scalar NormaliseDegreeAngle(br_scalar pAngle) {
    LOG_TRACE("(%f)", pAngle);

    while (pAngle < .0f) {
        pAngle += 360.f;
    }
    return pAngle;
}

#define SAW(T, PERIOD) (fmodf((T), (PERIOD)) / (PERIOD))

#define MOVE_FUNK_PARAMETER(DEST, MODE, PERIOD, AMPLITUDE, FLASH_VALUE)                   \
    do {                                                                                  \
        switch (MODE) {                                                                   \
        case eMove_continuous:                                                            \
            if ((PERIOD) == 0.f) {                                                        \
                DEST = 0.f;                                                               \
            } else {                                                                      \
                DEST = (AMPLITUDE)*SAW(f_the_time, (PERIOD));                             \
            }                                                                             \
            break;                                                                        \
        case eMove_controlled:                                                            \
            DEST = (PERIOD) * (AMPLITUDE);                                                \
            break;                                                                        \
        case eMove_absolute:                                                              \
            DEST = (PERIOD);                                                              \
            break;                                                                        \
        case eMove_linear:                                                                \
            if ((PERIOD) == 0.f) {                                                        \
                DEST = 0.f;                                                               \
            } else {                                                                      \
                DEST = (AMPLITUDE)*MapSawToTriangle(SAW(f_the_time, (PERIOD)));           \
            }                                                                             \
            break;                                                                        \
        case eMove_flash:                                                                 \
            if (2 * fmodf(f_the_time, (PERIOD)) > (PERIOD)) {                             \
                DEST = (FLASH_VALUE);                                                     \
            } else {                                                                      \
                DEST = -(FLASH_VALUE);                                                    \
            }                                                                             \
            break;                                                                        \
        case eMove_harmonic:                                                              \
            if ((PERIOD) == 0.f) {                                                        \
                DEST = 0.f;                                                               \
            } else {                                                                      \
                DEST = (AMPLITUDE)*BR_SIN(BR_ANGLE_DEG(SAW(f_the_time, (PERIOD)) * 360)); \
            }                                                                             \
            break;                                                                        \
        default:                                                                          \
            TELL_ME_IF_WE_PASS_THIS_WAY();                                                \
        }                                                                                 \
    } while (0)

// IDA: void __cdecl FunkThoseTronics()
void FunkThoseTronics() {
    int i;
    int j;
    int iteration_count;
    int finished;
    tFunkotronic_spec* the_funk;
    br_vector3* the_proximity;
    tS32 the_time;
    tS32 old_last_time;
    tS32 time_diff;
    br_matrix23* mat_matrix;
    br_material* the_material;
    float f_the_time;
    float rot_amount;
    float f_time_diff;
    br_vector2 tmp_v2;
    br_pixelmap* old_colour_map;
    LOG_TRACE("()");

    if (gFunkotronics_array == NULL) {
        return;
    }
    DontLetFlicFuckWithPalettes();
    the_time = GetTotalTime();
    f_the_time = (float)the_time;
    for (i = 0; i < gFunkotronics_array_size; i++) {
        the_funk = &gFunkotronics_array[i];
        if (the_funk->owner == -999) {
            continue;
        }
        j = 0;
        if (the_funk->mode == eFunk_mode_distance && the_funk->proximity_array != NULL) {
            j = -2;
            for (j = 0; j < the_funk->proximity_count; j++) {
                if (Vector3DistanceSquared(&the_funk->proximity_array[j], gOur_pos) <= gSight_distance_squared) {
                    j = -1;
                    break;
                }
            }
        }
        if (j == -1 || (j != -2 && (the_funk->mode != eFunk_mode_last_lap_only || gLap >= gTotal_laps) && (the_funk->mode != eFunk_mode_all_laps_but_last || gLap < gTotal_laps))) {
            the_material = the_funk->material;
            mat_matrix = &the_material->map_transform;
            if (!gAction_replay_mode || !ReplayIsPaused() || the_funk->matrix_mode == eMove_controlled || the_funk->matrix_mode == eMove_absolute) {
                switch (the_funk->matrix_mod_type) {
                case eMatrix_mod_spin:

                    BrMatrix23Identity(mat_matrix);
                    the_material->map_transform.m[2][0] -= .5f;
                    the_material->map_transform.m[2][1] -= .5f;
                    if (the_funk->matrix_mod_data.spin_info.period > 0.f) {
                        f_time_diff = 1.f - fmodf(the_funk->matrix_mod_data.spin_info.period, 1.f);
                    } else {
                        f_time_diff = fmodf(-the_funk->matrix_mod_data.spin_info.period, 1.f);
                    }

                    MOVE_FUNK_PARAMETER(rot_amount, the_funk->matrix_mode, f_time_diff, 65536.f, 7.5f);
                    DRMatrix23PostRotate(mat_matrix, (br_angle)rot_amount);

                    the_material->map_transform.m[2][0] += .5f;
                    the_material->map_transform.m[2][1] += .5f;
                    break;
                case eMatrix_mod_rock:
                    BrMatrix23Identity(mat_matrix);
                    the_material->map_transform.m[2][0] -= the_funk->matrix_mod_data.rock_info.x_centre;
                    the_material->map_transform.m[2][1] -= the_funk->matrix_mod_data.rock_info.y_centre;

                    MOVE_FUNK_PARAMETER(rot_amount, the_funk->matrix_mode, the_funk->matrix_mod_data.rock_info.period, the_funk->matrix_mod_data.rock_info.rock_angle, the_funk->matrix_mod_data.rock_info.rock_angle);
                    DRMatrix23PostRotate(mat_matrix, BrDegreeToAngle(NormaliseDegreeAngle(rot_amount)));

                    the_material->map_transform.m[2][0] += the_funk->matrix_mod_data.rock_info.x_centre;
                    the_material->map_transform.m[2][1] += the_funk->matrix_mod_data.rock_info.y_centre;
                    break;
                case eMatrix_mod_throb:
                    BrMatrix23Identity(mat_matrix);
                    the_material->map_transform.m[2][0] -= the_funk->matrix_mod_data.throb_info.x_centre;
                    the_material->map_transform.m[2][1] -= the_funk->matrix_mod_data.throb_info.y_centre;

                    MOVE_FUNK_PARAMETER(tmp_v2.v[1], the_funk->matrix_mode, the_funk->matrix_mod_data.throb_info.y_period, the_funk->matrix_mod_data.throb_info.y_magnitude, the_funk->matrix_mod_data.throb_info.y_magnitude);
                    MOVE_FUNK_PARAMETER(tmp_v2.v[0], the_funk->matrix_mode, the_funk->matrix_mod_data.throb_info.x_period, the_funk->matrix_mod_data.throb_info.x_magnitude, the_funk->matrix_mod_data.throb_info.x_magnitude);
                    BrMatrix23PostScale(mat_matrix, tmp_v2.v[0] + 1.f, tmp_v2.v[1] + 1.f);

                    the_material->map_transform.m[2][0] += the_funk->matrix_mod_data.throb_info.x_centre;
                    the_material->map_transform.m[2][1] += the_funk->matrix_mod_data.throb_info.y_centre;
                    break;
                case eMatrix_mod_slither:
                    MOVE_FUNK_PARAMETER(the_material->map_transform.m[2][0], the_funk->matrix_mode, the_funk->matrix_mod_data.slither_info.x_period, the_funk->matrix_mod_data.slither_info.x_magnitude, the_funk->matrix_mod_data.slither_info.x_magnitude);
                    MOVE_FUNK_PARAMETER(the_material->map_transform.m[2][1], the_funk->matrix_mode, the_funk->matrix_mod_data.slither_info.y_period, the_funk->matrix_mod_data.slither_info.y_magnitude, the_funk->matrix_mod_data.slither_info.y_magnitude);
                    break;
                case eMatrix_mod_roll:
                    LOG_DEBUG("roll");
                    MOVE_FUNK_PARAMETER(the_material->map_transform.m[2][0], the_funk->matrix_mode, the_funk->matrix_mod_data.roll_info.x_period, 1.f, 1.875f);
                    MOVE_FUNK_PARAMETER(the_material->map_transform.m[2][1], the_funk->matrix_mode, the_funk->matrix_mod_data.roll_info.y_period, 1.f, 1.875f);
                    break;
                case eMatrix_mod_none:
                    break;
                }
                if (the_funk->matrix_mod_type != eMatrix_mod_none) {
                    BrMaterialUpdate(the_funk->material, BR_MATU_MAP_TRANSFORM);
                }
            }
            if (the_funk->lighting_animation_type != eMove_none) {
                MOVE_FUNK_PARAMETER(the_material->ka, the_funk->lighting_animation_type, the_funk->lighting_animation_period, the_funk->ambient_delta, the_funk->ambient_delta);
                the_material->ka += the_funk->ambient_base;

                MOVE_FUNK_PARAMETER(the_material->kd, the_funk->lighting_animation_type, the_funk->lighting_animation_period, the_funk->direct_delta, the_funk->direct_delta);
                the_material->kd += the_funk->direct_base;

                MOVE_FUNK_PARAMETER(the_material->ks, the_funk->lighting_animation_type, the_funk->lighting_animation_period, the_funk->specular_delta, the_funk->specular_delta);
                the_material->ks += the_funk->specular_base;
            }
            if (the_funk->texture_animation_type == eTexture_animation_frames) {
                if (!gAction_replay_mode || !ReplayIsPaused() || the_funk->mode == eFunk_mode_all_laps_but_last || the_funk->mode == 4) {
                    old_colour_map = the_material->colour_map;
                    if (the_funk->time_mode == eTime_mode_accurate) {
                        MOVE_FUNK_PARAMETER(rot_amount, the_funk->texture_animation_data.frames_info.mode, the_funk->texture_animation_data.frames_info.period, the_funk->texture_animation_data.frames_info.texture_count, the_funk->texture_animation_data.frames_info.texture_count);
                        the_material->colour_map = the_funk->texture_animation_data.frames_info.textures[(int)rot_amount];
                    } else {
                        if (the_funk->texture_animation_data.frames_info.period <= fabsf(f_the_time - the_funk->last_frame)) {
                            the_funk->last_frame = f_the_time;
                            the_funk->texture_animation_data.frames_info.current_frame++;
                            if (the_funk->texture_animation_data.frames_info.current_frame >= the_funk->texture_animation_data.frames_info.texture_count) {
                                the_funk->texture_animation_data.frames_info.current_frame = 0;
                            }
                            the_material->colour_map = the_funk->texture_animation_data.frames_info.textures[(int)rot_amount];
                        }
                    }
                    if (the_material->colour_map != old_colour_map) {
                        BrMaterialUpdate(the_funk->material, BR_MATU_COLOURMAP);
                    }
                }
            } else if (the_funk->texture_animation_type == eTexture_animation_flic && (!gAction_replay_mode || !ReplayIsPaused())) {
                f_time_diff = f_the_time;
                if (f_time_diff < the_funk->last_frame) {
                    f_time_diff = 2 * the_funk->last_frame - f_time_diff;
                }
                if (the_funk->time_mode == eTime_mode_accurate) {
                    if (the_funk->last_frame) {
                        iteration_count = (f_time_diff - the_funk->last_frame) / the_funk->texture_animation_data.flic_info.flic_descriptor.frame_period;
                    } else {
                        iteration_count = 1;
                    }
                } else {
                    if (f_time_diff - the_funk->last_frame > the_funk->texture_animation_data.flic_info.flic_descriptor.frame_period) {
                        iteration_count = 1;
                    } else {
                        iteration_count = 0;
                    }
                }
                for (j = 0; j < iteration_count; j++) {
                    finished = PlayNextFlicFrame(&the_funk->texture_animation_data.flic_info.flic_descriptor);
                    BrMapUpdate(the_funk->material->colour_map, BR_MAPU_ALL);
                    BrMaterialUpdate(the_funk->material, BR_MATU_COLOURMAP);
                    if (finished) {
                        EndFlic(&the_funk->texture_animation_data.flic_info.flic_descriptor);
                        StartFlic(
                            the_funk->texture_animation_data.flic_info.flic_descriptor.file_name,
                            -1,
                            &the_funk->texture_animation_data.flic_info.flic_descriptor,
                            the_funk->texture_animation_data.flic_info.flic_data_length,
                            (tS8*)the_funk->texture_animation_data.flic_info.flic_data,
                            the_material->colour_map, 0, 0, 0);
                    }
                    the_funk->last_frame = f_the_time;
                }
            }
        }
    }
    LetFlicFuckWithPalettes();
}

// IDA: void __usercall LollipopizeActor(br_actor *pSubject_actor@<EAX>, br_matrix34 *ref_to_world@<EDX>, tLollipop_mode pWhich_axis@<EBX>)
void LollipopizeActor(br_actor* pSubject_actor, br_matrix34* ref_to_world, tLollipop_mode pWhich_axis) {
    br_vector3 ref_to_subject;
    br_vector3 fixed_axis;
    br_vector3 vector_a;
    br_vector3 vector_b;
    br_matrix34 subject_to_world;
    br_matrix34 mat;
    LOG_TRACE("(%p, %p, %d)", pSubject_actor, ref_to_world, pWhich_axis);

    BrActorToActorMatrix34(&subject_to_world, pSubject_actor, gNon_track_actor);
    BrVector3Sub(&ref_to_subject, (br_vector3*)ref_to_world->m[3], (br_vector3*)subject_to_world.m[3]);
    switch (pWhich_axis) {
    case eLollipop_none:
        TELL_ME_IF_WE_PASS_THIS_WAY();
        break;
    case eLollipop_x_match:
        BrVector3SetFloat(&vector_a, 1.f, 0.f, 0.f);
        break;
    case eLollipop_y_match:
        BrVector3SetFloat(&vector_a, 0.f, 1.f, 0.f);
        break;
    case eLollipop_z_match:
        BrVector3SetFloat(&vector_a, 0.f, 0.f, 1.f);
        break;
    }
    BrVector3Cross(&vector_b, &ref_to_subject, &vector_a);
    BrVector3Normalise(&vector_b, &vector_b);

    BrVector3Cross(&fixed_axis, &vector_a, &vector_b);

    switch (pWhich_axis) {
    case eLollipop_none:
        break;
    case eLollipop_x_match:
        mat.m[0][0] = vector_a.v[0];
        mat.m[1][0] = vector_a.v[1];
        mat.m[2][0] = vector_a.v[2];
        mat.m[0][1] = vector_b.v[0];
        mat.m[1][1] = vector_b.v[1];
        mat.m[2][1] = vector_b.v[2];
        mat.m[0][2] = fixed_axis.v[0];
        mat.m[1][2] = fixed_axis.v[1];
        mat.m[2][2] = fixed_axis.v[2];
        mat.m[3][0] = 0.f;
        mat.m[3][1] = 0.f;
        mat.m[3][2] = 0.f;
        break;
    case eLollipop_y_match:
        mat.m[0][0] = vector_b.v[0];
        mat.m[1][0] = vector_b.v[1];
        mat.m[2][0] = vector_b.v[2];
        mat.m[0][1] = vector_a.v[0];
        mat.m[1][1] = vector_a.v[1];
        mat.m[2][1] = vector_a.v[2];
        mat.m[0][2] = fixed_axis.v[0];
        mat.m[1][2] = fixed_axis.v[1];
        mat.m[2][2] = fixed_axis.v[2];
        mat.m[3][0] = 0.f;
        mat.m[3][1] = 0.f;
        mat.m[3][2] = 0.f;
        break;
    case eLollipop_z_match:
        mat.m[0][0] = vector_b.v[0];
        mat.m[1][0] = vector_b.v[1];
        mat.m[2][0] = vector_b.v[2];
        mat.m[0][1] = vector_a.v[0];
        mat.m[1][1] = vector_a.v[1];
        mat.m[2][1] = vector_a.v[2];
        mat.m[0][2] = fixed_axis.v[0];
        mat.m[1][2] = fixed_axis.v[1];
        mat.m[2][2] = fixed_axis.v[2];
        mat.m[3][0] = 0.f;
        mat.m[3][1] = 0.f;
        mat.m[3][2] = 0.f;
        break;
    }
    BrMatrix34Pre(&pSubject_actor->t.t.mat, &mat);
}

// IDA: void __usercall CalcActorGlobalPos(br_vector3 *pResult@<EAX>, br_actor *pActor@<EDX>)
void CalcActorGlobalPos(br_vector3* pResult, br_actor* pActor) {
    LOG_TRACE("(%p, %p)", pResult, pActor);

    pResult->v[0] = 0.0;
    pResult->v[1] = 0.0;
    pResult->v[2] = 0.0;
    while (pActor && pActor != gNon_track_actor) {
        pResult->v[0] = pActor->t.t.mat.m[3][0] + pResult->v[0];
        pResult->v[1] = pActor->t.t.mat.m[3][1] + pResult->v[1];
        pResult->v[2] = pActor->t.t.mat.m[3][2] + pResult->v[2];
        pActor = pActor->parent;
    }
}

// IDA: int __usercall PointOutOfSight@<EAX>(br_vector3 *pPoint@<EAX>, br_scalar pMax_distance)
int PointOutOfSight(br_vector3* pPoint, br_scalar pMax_distance) {
    br_vector3 distance_vector;
    LOG_TRACE("(%p, %f)", pPoint, pMax_distance);

    if (gMirror_on__graphics) {
        distance_vector.v[0] = pPoint->v[0] - gRearview_camera_to_world.m[3][0];
        distance_vector.v[1] = pPoint->v[1] - gRearview_camera_to_world.m[3][1];
        distance_vector.v[2] = pPoint->v[2] - gRearview_camera_to_world.m[3][2];
        if (distance_vector.v[0] * distance_vector.v[0]
                    + distance_vector.v[1] * distance_vector.v[1]
                    + distance_vector.v[2] * distance_vector.v[2]
                < pMax_distance
            && gRearview_camera_to_world.m[2][2] * distance_vector.v[2]
                    + gRearview_camera_to_world.m[2][1] * distance_vector.v[1]
                    + gRearview_camera_to_world.m[2][0] * distance_vector.v[0]
                < 0.0) {
            return 0;
        }
    }
    distance_vector.v[0] = pPoint->v[0] - gCamera_to_world.m[3][0];
    distance_vector.v[1] = pPoint->v[1] - gCamera_to_world.m[3][1];
    distance_vector.v[2] = pPoint->v[2] - gCamera_to_world.m[3][2];
    return distance_vector.v[0] * distance_vector.v[0] + distance_vector.v[1] * distance_vector.v[1] + distance_vector.v[2] * distance_vector.v[2] >= pMax_distance
        || gCamera_to_world.m[2][2] * distance_vector.v[2] + gCamera_to_world.m[2][1] * distance_vector.v[1] + gCamera_to_world.m[2][0] * distance_vector.v[0] >= 0.0;
}

// IDA: void __usercall PathGrooveBastard(tGroovidelic_spec *pGroove@<EAX>, tU32 pTime@<EDX>, br_matrix34 *pMat@<EBX>, int pInterrupt_it@<ECX>)
void PathGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it) {
    br_scalar pos;
    LOG_TRACE("(%p, %d, %p, %d)", pGroove, pTime, pMat, pInterrupt_it);

    pos = 0;
    if (pGroove->path_type == eGroove_path_straight) {
        if (pGroove->path_data.straight_info.x_delta != 0.0) {

            switch (pGroove->path_mode) {
            case eMove_continuous:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period * pGroove->path_data.straight_info.x_delta;
                }
                break;
            case eMove_controlled:
                pos = pGroove->path_data.straight_info.period * pGroove->path_data.straight_info.x_delta;
                break;

            case eMove_absolute:
                pos = pGroove->path_data.straight_info.period;
                break;
            case eMove_flash:
                if (fmod(pTime, pGroove->path_data.straight_info.period) * 2.0 <= pGroove->path_data.straight_info.period) {
                    pos = pGroove->path_data.straight_info.x_delta;
                } else {
                    pos = -pGroove->path_data.straight_info.x_delta;
                }
                break;
            case eMove_harmonic:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = sin(
                              BrAngleToRadian(
                                  BrDegreeToAngle(
                                      fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period * 360.0)))
                        * pGroove->path_data.straight_info.x_delta;
                }
                break;
            case eMove_linear:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = MapSawToTriangle(fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period)
                        * pGroove->path_data.straight_info.x_delta;
                }
                break;
            case eMove_none:
                break;
            }

            pos = pGroove->path_data.straight_info.centre.v[0] + pos;
            if (pInterrupt_it) {
                pGroove->path_resumption_value = pos;
                if (pMat->m[3][0] <= pos) {
                    pGroove->path_interrupt_status = eInterrupt_greater_than;
                } else {
                    pGroove->path_interrupt_status = eInterrupt_less_than;
                }
            } else if (pGroove->path_interrupt_status) {
                if (pGroove->path_interrupt_status == eInterrupt_less_than) {
                    if (pGroove->path_resumption_value > pos) {
                        pGroove->path_interrupt_status = eInterrupt_none;
                        pGroove->actor->t.t.translate.t = pGroove->path_data.straight_info.centre;
                        pMat->m[3][0] = pos;
                    }
                } else if (pGroove->path_resumption_value < pos) {
                    pGroove->path_interrupt_status = eInterrupt_none;
                    pGroove->actor->t.t.euler.t = pGroove->path_data.straight_info.centre;
                    pMat->m[3][0] = pos;
                }
            }
        }

        if (pGroove->path_data.straight_info.y_delta != 0.0) {
            pos = 0;
            switch (pGroove->path_mode) {
            case eMove_continuous:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period * pGroove->path_data.straight_info.y_delta;
                }
                break;
            case eMove_controlled:
                pos = pGroove->path_data.straight_info.period * pGroove->path_data.straight_info.y_delta;
                break;

            case eMove_absolute:
                pos = pGroove->path_data.straight_info.period;
                break;
            case eMove_flash:
                if (fmod(pTime, pGroove->path_data.straight_info.period) * 2.0 <= pGroove->path_data.straight_info.period) {
                    pos = pGroove->path_data.straight_info.y_delta;
                } else {
                    pos = -pGroove->path_data.straight_info.y_delta;
                }
                break;
            case eMove_harmonic:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = sin(
                              BrAngleToRadian(
                                  BrDegreeToAngle(
                                      fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period * 360.0)))
                        * pGroove->path_data.straight_info.y_delta;
                }
                break;
            case eMove_linear:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = MapSawToTriangle(fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period)
                        * pGroove->path_data.straight_info.y_delta;
                }
                break;
            case eMove_none:
                break;
            }

            pos = pGroove->path_data.straight_info.centre.v[1] + pos;
            if (pInterrupt_it) {
                pGroove->path_resumption_value = pos;
                if (pMat->m[3][1] <= pos) {
                    pGroove->path_interrupt_status = eInterrupt_greater_than;
                } else {
                    pGroove->path_interrupt_status = eInterrupt_less_than;
                }
            } else if (pGroove->path_interrupt_status) {
                if (pGroove->path_interrupt_status == eInterrupt_less_than) {
                    if (pGroove->path_resumption_value > pos) {
                        pGroove->path_interrupt_status = eInterrupt_none;
                        pGroove->actor->t.t.translate.t = pGroove->path_data.straight_info.centre;
                        pMat->m[3][1] = pos;
                    }
                } else if (pGroove->path_resumption_value < pos) {
                    pGroove->path_interrupt_status = eInterrupt_none;
                    pGroove->actor->t.t.euler.t = pGroove->path_data.straight_info.centre;
                    pMat->m[3][1] = pos;
                }
            }
        }

        if (pGroove->path_data.straight_info.z_delta != 0.0) {
            pos = 0;
            switch (pGroove->path_mode) {
            case eMove_continuous:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period * pGroove->path_data.straight_info.z_delta;
                }
                break;
            case eMove_controlled:
                pos = pGroove->path_data.straight_info.period * pGroove->path_data.straight_info.z_delta;
                break;
            case eMove_absolute:
                pos = pGroove->path_data.straight_info.period;
                break;
            case eMove_flash:
                if (fmod(pTime, pGroove->path_data.straight_info.period) * 2.0 <= pGroove->path_data.straight_info.period) {
                    pos = pGroove->path_data.straight_info.z_delta;
                } else {
                    pos = -pGroove->path_data.straight_info.z_delta;
                }
                break;
            case eMove_harmonic:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = sin(
                              BrAngleToRadian(
                                  BrDegreeToAngle(
                                      fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period * 360.0)))
                        * pGroove->path_data.straight_info.z_delta;
                }
                break;
            case eMove_linear:
                if (pGroove->path_data.straight_info.period != 0.0) {
                    pos = MapSawToTriangle(fmod(pTime, pGroove->path_data.straight_info.period) / pGroove->path_data.straight_info.period)
                        * pGroove->path_data.straight_info.z_delta;
                }
                break;
            case eMove_none:
                break;
            }

            pos = pGroove->path_data.straight_info.centre.v[1] + pos;
            if (pInterrupt_it) {
                pGroove->path_resumption_value = pos;
                if (pMat->m[3][2] <= pos) {
                    pGroove->path_interrupt_status = eInterrupt_greater_than;
                } else {
                    pGroove->path_interrupt_status = eInterrupt_less_than;
                }
            } else if (pGroove->path_interrupt_status) {
                if (pGroove->path_interrupt_status == eInterrupt_less_than) {
                    if (pGroove->path_resumption_value > pos) {
                        pGroove->path_interrupt_status = eInterrupt_none;
                        pGroove->actor->t.t.translate.t = pGroove->path_data.straight_info.centre;
                        pMat->m[3][2] = pos;
                    }
                } else if (pGroove->path_resumption_value < pos) {
                    pGroove->path_interrupt_status = eInterrupt_none;
                    pGroove->actor->t.t.euler.t = pGroove->path_data.straight_info.centre;
                    pMat->m[3][2] = pos;
                }
            }
        }
        pGroove->object_position = pGroove->actor->t.t.translate.t;
    } else if (pGroove->path_type == eGroove_path_circular) {
        pGroove->actor->t.t.translate.t = pGroove->path_data.circular_info.centre;
        if (pGroove->path_data.circular_info.axis == eGroove_axis_y) {
            if (pGroove->path_data.circular_info.period != 0.0) {
                pos = cos(BrAngleToRadian(BrDegreeToAngle(fmod(pTime, pGroove->path_data.circular_info.period) / pGroove->path_data.circular_info.period * 360.0))) * pGroove->path_data.circular_info.radius;
                pMat->m[3][0] = pGroove->path_data.circular_info.centre.v[0] + pos;
            }

        } else if (pGroove->path_data.circular_info.axis == eGroove_axis_z) {
            if (pGroove->path_data.circular_info.period != 0.0) {
                pos = sin(BrAngleToRadian(BrDegreeToAngle(fmod(pTime, pGroove->path_data.circular_info.period) / pGroove->path_data.circular_info.period * 360.0))) * pGroove->path_data.circular_info.radius;
                pMat->m[3][0] = pGroove->path_data.circular_info.centre.v[0] + pos;
            }
        }
        if (pGroove->path_data.circular_info.axis) {
            if (pGroove->path_data.circular_info.axis == eGroove_axis_z) {
                if (pGroove->path_data.circular_info.period != 0.0) {
                    pos = cos(BrAngleToRadian(BrDegreeToAngle(fmod(pTime, pGroove->path_data.circular_info.period) / pGroove->path_data.circular_info.period * 360.0))) * pGroove->path_data.circular_info.radius;
                    pMat->m[3][1] = pGroove->path_data.circular_info.centre.v[1] + pos;
                }
            } else {
                if (pGroove->path_data.circular_info.period != 0.0) {
                    pos = sin(BrAngleToRadian(BrDegreeToAngle(fmod(pTime, pGroove->path_data.circular_info.period) / pGroove->path_data.circular_info.period * 360.0))) * pGroove->path_data.circular_info.radius;
                    pMat->m[3][1] = pGroove->path_data.circular_info.centre.v[1] + pos;
                }
            }
        }
        if (pGroove->path_data.circular_info.axis) {
            if (pGroove->path_data.circular_info.axis == eGroove_axis_y) {
                if (pGroove->path_data.circular_info.period != 0.0) {
                    pos = sin(BrAngleToRadian(BrDegreeToAngle(fmod(pTime, pGroove->path_data.circular_info.period) / pGroove->path_data.circular_info.period * 360.0))) * pGroove->path_data.circular_info.radius;
                    pMat->m[3][2] = pGroove->path_data.circular_info.centre.v[2] + pos;
                }
            } else {
                if (pGroove->path_data.circular_info.period != 0.0) {
                    pos = cos(BrAngleToRadian(BrDegreeToAngle(fmod(pTime, pGroove->path_data.circular_info.period) / pGroove->path_data.circular_info.period * 360.0))) * pGroove->path_data.circular_info.radius;
                    pMat->m[3][2] = pGroove->path_data.circular_info.centre.v[2] + pos;
                }
            }
            pGroove->object_position = pGroove->actor->t.t.translate.t;
        }
    }
}

// IDA: void __usercall ObjectGrooveBastard(tGroovidelic_spec *pGroove@<EAX>, tU32 pTime@<EDX>, br_matrix34 *pMat@<EBX>, int pInterrupt_it@<ECX>)
void ObjectGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it) {
    int rock_it;
    br_scalar x_size;
    br_scalar y_size;
    br_scalar z_size;
    br_scalar pos;
    br_bounds* bounds;
    LOG_TRACE("(%p, %d, %p, %d)", pGroove, pTime, pMat, pInterrupt_it);

    x_size = 0;
    y_size = 0;
    z_size = 0;
    pos = 0;

    switch (pGroove->object_type) {
    case eGroove_object_spin:
        if (pGroove->object_data.spin_info.axis == eGroove_axis_y) {
            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period * 360.0;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                pos = pGroove->object_data.spin_info.period * 360.0;
            } else if (pGroove->object_mode == eMove_absolute) {
                pos = pGroove->object_data.spin_info.period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.spin_info.period) * 2.0 <= pGroove->object_data.spin_info.period) {
                    pos = 3.85156;
                } else {
                    pos = -3.85156;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = sin(
                              BrAngleToRadian(
                                  BrDegreeToAngle(
                                      fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period * 360.0)))
                        * 360.0;
                }
            } else {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period;
                    pos = MapSawToTriangle(pos) * 360.0;
                }
            }
            DRMatrix34PostRotateY(pMat, BrDegreeToAngle(pos));
        } else if (pGroove->object_data.spin_info.axis == eGroove_axis_z) {
            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period * 360.0;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                pos = pGroove->object_data.spin_info.period * 360.0;
            } else if (pGroove->object_mode == eMove_absolute) {
                pos = pGroove->object_data.spin_info.period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.spin_info.period) * 2.0 <= pGroove->object_data.spin_info.period) {
                    pos = 3.85156;
                } else {
                    pos = -3.85156;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = sin(
                              BrAngleToRadian(
                                  BrDegreeToAngle(
                                      fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period * 360.0)))
                        * 360.0;
                }
            } else {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period;
                    pos = MapSawToTriangle(pos) * 360.0;
                }
            }
            DRMatrix34PostRotateZ(pMat, BrDegreeToAngle(pos));
        } else if (pGroove->object_data.spin_info.axis == eGroove_axis_x) {
            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period * 360.0;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                pos = pGroove->object_data.spin_info.period * 360.0;
            } else if (pGroove->object_mode == eMove_absolute) {
                pos = pGroove->object_data.spin_info.period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.spin_info.period) * 2.0 <= pGroove->object_data.spin_info.period) {
                    pos = 3.85156;
                } else {
                    pos = -3.85156;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = sin(
                              BrAngleToRadian(
                                  BrDegreeToAngle(
                                      fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period * 360.0)))
                        * 360.0;
                }
            } else {
                if (pGroove->object_data.spin_info.period != 0.0) {
                    pos = fmod(pTime, pGroove->object_data.spin_info.period) / pGroove->object_data.spin_info.period;
                    pos = MapSawToTriangle(pos) * 360.0;
                }
            }
            DRMatrix34PostRotateX(pMat, BrDegreeToAngle(pos));
        }
        break;
    case eGroove_object_rock:
        rock_it = 1;
        if (pGroove->object_mode == eMove_continuous) {
            if (pGroove->object_data.rock_info.period != 0.0) {
                pos = fmod(pTime, pGroove->object_data.rock_info.period) / pGroove->object_data.rock_info.period * pGroove->object_data.rock_info.max_angle;
            }
        } else if (pGroove->object_mode == eMove_controlled) {
            pos = pGroove->object_data.rock_info.period * pGroove->object_data.rock_info.max_angle;
        } else if (pGroove->object_mode == eMove_absolute) {
            pos = pGroove->object_data.rock_info.period;
        } else if (pGroove->object_mode == eMove_flash) {
            if (fmod(pTime, pGroove->object_data.rock_info.period) * 2.0 <= pGroove->object_data.rock_info.period) {
                pos = pGroove->object_data.rock_info.max_angle;
            } else {
                pos = -pGroove->object_data.rock_info.max_angle;
            }
        } else if (pGroove->object_mode == eMove_harmonic) {
            if (pGroove->object_data.rock_info.period != 0.0) {
                pos = sin(
                          BrAngleToRadian(
                              BrDegreeToAngle(
                                  fmod(pTime, pGroove->object_data.rock_info.period) / pGroove->object_data.rock_info.period * 360.0)))
                    * pGroove->object_data.rock_info.max_angle;
            }
        } else {
            if (pGroove->object_data.rock_info.period != 0.0) {
                pos = fmod(pTime, pGroove->object_data.rock_info.period) / pGroove->object_data.rock_info.period;
                pos = MapSawToTriangle(pos) * pGroove->object_data.rock_info.max_angle;
            }
        }

        if (pInterrupt_it) {
            pGroove->object_resumption_value = pos;
            if (pGroove->object_data.rock_info.current_angle <= pos) {
                pGroove->object_interrupt_status = eInterrupt_greater_than;
            } else {
                pGroove->object_interrupt_status = eInterrupt_less_than;
            }
        } else if (pGroove->object_interrupt_status == eInterrupt_less_than) {
            if (pGroove->object_resumption_value <= pos || gAction_replay_mode) {
                rock_it = 0;
            } else {
                pGroove->object_interrupt_status = eInterrupt_none;
            }
        } else if (pGroove->object_interrupt_status == eInterrupt_greater_than) {
            if (pGroove->object_resumption_value >= pos || gAction_replay_mode) {
                rock_it = 0;
            } else {
                pGroove->object_interrupt_status = eInterrupt_none;
            }
        }
        if (rock_it) {
            pGroove->object_data.rock_info.current_angle = pos;
        }

        if (pGroove->object_data.rock_info.axis == eGroove_axis_y) {
            DRMatrix34PostRotateY(pMat, BrDegreeToAngle(pGroove->object_data.rock_info.current_angle));
        } else if (pGroove->object_data.rock_info.axis == eGroove_axis_z) {
            DRMatrix34PostRotateZ(pMat, BrDegreeToAngle(pGroove->object_data.rock_info.current_angle));
        } else if (pGroove->object_data.rock_info.axis == eGroove_axis_x) {
            DRMatrix34PostRotateX(pMat, BrDegreeToAngle(pGroove->object_data.rock_info.current_angle));
        }
        break;
    case eGroove_object_throb:
        if (pGroove->object_mode == eMove_continuous) {
            if (pGroove->object_data.throb_info.z_period != 0.0) {
                z_size = fmod(pTime, pGroove->object_data.throb_info.z_period) / pGroove->object_data.throb_info.z_period * pGroove->object_data.throb_info.z_magnitude;
            }
        } else if (pGroove->object_mode == eMove_controlled) {
            z_size = pGroove->object_data.throb_info.z_magnitude * pGroove->object_data.throb_info.z_period;
        } else if (pGroove->object_mode == eMove_absolute) {
            z_size = pGroove->object_data.throb_info.z_period;
        } else if (pGroove->object_mode == eMove_flash) {
            if (fmod(pTime, pGroove->object_data.throb_info.z_period) * 2.0 <= pGroove->object_data.throb_info.z_period) {
                z_size = pGroove->object_data.throb_info.z_magnitude;
            } else {
                z_size = -pGroove->object_data.throb_info.z_magnitude;
            }
        } else if (pGroove->object_mode == eMove_harmonic) {
            if (pGroove->object_data.throb_info.z_period != 0.0) {
                z_size = sin(
                             BrAngleToRadian(
                                 BrDegreeToAngle(
                                     fmod(pTime, pGroove->object_data.throb_info.z_period) / pGroove->object_data.throb_info.z_period * 360.0)))
                    * pGroove->object_data.throb_info.z_magnitude;
            }
        } else {
            if (pGroove->object_data.throb_info.z_period != 0.0) {
                z_size = fmod(pTime, pGroove->object_data.throb_info.z_period) / pGroove->object_data.throb_info.z_period;
                z_size = MapSawToTriangle(z_size) * pGroove->object_data.throb_info.z_magnitude;
            }
        }

        if (pGroove->object_mode == eMove_continuous) {
            if (pGroove->object_data.throb_info.x_period != 0.0) {
                x_size = fmod(pTime, pGroove->object_data.throb_info.x_period) / pGroove->object_data.throb_info.x_period * pGroove->object_data.throb_info.x_magnitude;
            }
        } else if (pGroove->object_mode == eMove_controlled) {
            x_size = pGroove->object_data.throb_info.x_magnitude * pGroove->object_data.throb_info.x_period;
        } else if (pGroove->object_mode == eMove_absolute) {
            x_size = pGroove->object_data.throb_info.x_period;
        } else if (pGroove->object_mode == eMove_flash) {
            if (fmod(pTime, pGroove->object_data.throb_info.x_period) * 2.0 <= pGroove->object_data.throb_info.x_period) {
                x_size = pGroove->object_data.throb_info.x_magnitude;
            } else {
                x_size = -pGroove->object_data.throb_info.x_magnitude;
            }
        } else if (pGroove->object_mode == eMove_harmonic) {
            if (pGroove->object_data.throb_info.x_period != 0.0) {
                x_size = sin(
                             BrAngleToRadian(
                                 BrDegreeToAngle(
                                     fmod(pTime, pGroove->object_data.throb_info.x_period) / pGroove->object_data.throb_info.x_period * 360.0)))
                    * pGroove->object_data.throb_info.x_magnitude;
            }
        } else {
            if (pGroove->object_data.throb_info.x_period != 0.0) {
                x_size = fmod(pTime, pGroove->object_data.throb_info.x_period) / pGroove->object_data.throb_info.x_period;
                x_size = MapSawToTriangle(x_size) * pGroove->object_data.throb_info.x_magnitude;
            }
        }

        if (pGroove->object_mode == eMove_continuous) {
            if (pGroove->object_data.throb_info.y_period != 0.0) {
                y_size = fmod(pTime, pGroove->object_data.throb_info.y_period) / pGroove->object_data.throb_info.y_period * pGroove->object_data.throb_info.y_magnitude;
            }
        } else if (pGroove->object_mode == eMove_controlled) {
            y_size = pGroove->object_data.throb_info.y_magnitude * pGroove->object_data.throb_info.y_period;
        } else if (pGroove->object_mode == eMove_absolute) {
            y_size = pGroove->object_data.throb_info.y_period;
        } else if (pGroove->object_mode == eMove_flash) {
            if (fmod(pTime, pGroove->object_data.throb_info.y_period) * 2.0 <= pGroove->object_data.throb_info.y_period) {
                y_size = pGroove->object_data.throb_info.y_magnitude;
            } else {
                y_size = -pGroove->object_data.throb_info.y_magnitude;
            }
        } else if (pGroove->object_mode == eMove_harmonic) {
            if (pGroove->object_data.throb_info.y_period != 0.0) {
                y_size = sin(
                             BrAngleToRadian(
                                 BrDegreeToAngle(
                                     fmod(pTime, pGroove->object_data.throb_info.y_period) / pGroove->object_data.throb_info.y_period * 360.0)))
                    * pGroove->object_data.throb_info.y_magnitude;
            }
        } else {
            if (pGroove->object_data.throb_info.y_period != 0.0) {
                y_size = fmod(pTime, pGroove->object_data.throb_info.y_period) / pGroove->object_data.throb_info.y_period;
                y_size = MapSawToTriangle(y_size) * pGroove->object_data.throb_info.y_magnitude;
            }
        }

        BrMatrix34PostScale(pMat, x_size + 1.0, y_size + 1.0, z_size + 1.0);
        /* FALLTHROUGH */
    case eGroove_object_shear:
        bounds = &pGroove->actor->model->bounds;
        if (pGroove->object_data.shear_info.x_magnitude == 0.0) {
            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.shear_info.z_period != 0.0) {
                    z_size = fmod(pTime, pGroove->object_data.shear_info.z_period) / pGroove->object_data.shear_info.z_period * pGroove->object_data.shear_info.z_magnitude;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                z_size = pGroove->object_data.shear_info.z_magnitude * pGroove->object_data.shear_info.z_period;
            } else if (pGroove->object_mode == eMove_absolute) {
                z_size = pGroove->object_data.shear_info.z_period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.shear_info.z_period) * 2.0 <= pGroove->object_data.shear_info.z_period) {
                    z_size = pGroove->object_data.shear_info.z_magnitude;
                } else {
                    z_size = -pGroove->object_data.shear_info.z_magnitude;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.shear_info.z_period != 0.0) {
                    z_size = sin(
                                 BrAngleToRadian(
                                     BrDegreeToAngle(
                                         fmod(pTime, pGroove->object_data.shear_info.z_period) / pGroove->object_data.shear_info.z_period * 360.0)))
                        * pGroove->object_data.shear_info.z_magnitude;
                }
            } else {
                if (pGroove->object_data.shear_info.z_period != 0.0) {
                    z_size = fmod(pTime, pGroove->object_data.shear_info.z_period) / pGroove->object_data.shear_info.z_period;
                    z_size = MapSawToTriangle(z_size) * pGroove->object_data.shear_info.z_magnitude;
                }
            }

            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.shear_info.y_period != 0.0) {
                    y_size = fmod(pTime, pGroove->object_data.shear_info.y_period) / pGroove->object_data.shear_info.y_period * pGroove->object_data.shear_info.y_magnitude;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                y_size = pGroove->object_data.shear_info.y_magnitude * pGroove->object_data.shear_info.y_period;
            } else if (pGroove->object_mode == eMove_absolute) {
                y_size = pGroove->object_data.shear_info.y_period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.shear_info.y_period) * 2.0 <= pGroove->object_data.shear_info.y_period) {
                    y_size = pGroove->object_data.shear_info.y_magnitude;
                } else {
                    y_size = -pGroove->object_data.shear_info.y_magnitude;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.shear_info.y_period != 0.0) {
                    y_size = sin(
                                 BrAngleToRadian(
                                     BrDegreeToAngle(
                                         fmod(pTime, pGroove->object_data.shear_info.y_period) / pGroove->object_data.shear_info.y_period * 360.0)))
                        * pGroove->object_data.shear_info.y_magnitude;
                }
            } else {
                if (pGroove->object_data.shear_info.y_period != 0.0) {
                    y_size = fmod(pTime, pGroove->object_data.shear_info.y_period) / pGroove->object_data.shear_info.y_period;
                    y_size = MapSawToTriangle(y_size) * pGroove->object_data.shear_info.y_magnitude;
                }
            }
            BrMatrix34PostShearX(pMat, y_size / (bounds->max.v[1] - bounds->min.v[1]), z_size / bounds->max.v[2] - bounds->min.v[2]);
        } else if (pGroove->object_data.shear_info.y_magnitude == 0.0) {

            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.shear_info.z_period != 0.0) {
                    z_size = fmod(pTime, pGroove->object_data.shear_info.z_period) / pGroove->object_data.shear_info.z_period * pGroove->object_data.shear_info.z_magnitude;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                z_size = pGroove->object_data.shear_info.z_magnitude * pGroove->object_data.shear_info.z_period;
            } else if (pGroove->object_mode == eMove_absolute) {
                z_size = pGroove->object_data.shear_info.z_period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.shear_info.z_period) * 2.0 <= pGroove->object_data.shear_info.z_period) {
                    z_size = pGroove->object_data.shear_info.z_magnitude;
                } else {
                    z_size = -pGroove->object_data.shear_info.z_magnitude;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.shear_info.z_period != 0.0) {
                    z_size = sin(
                                 BrAngleToRadian(
                                     BrDegreeToAngle(
                                         fmod(pTime, pGroove->object_data.shear_info.z_period) / pGroove->object_data.shear_info.z_period * 360.0)))
                        * pGroove->object_data.shear_info.z_magnitude;
                }
            } else {
                if (pGroove->object_data.shear_info.z_period != 0.0) {
                    z_size = fmod(pTime, pGroove->object_data.shear_info.z_period) / pGroove->object_data.shear_info.z_period;
                    z_size = MapSawToTriangle(z_size) * pGroove->object_data.shear_info.z_magnitude;
                }
            }

            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.shear_info.x_period != 0.0) {
                    x_size = fmod(pTime, pGroove->object_data.shear_info.x_period) / pGroove->object_data.shear_info.x_period * pGroove->object_data.shear_info.x_magnitude;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                x_size = pGroove->object_data.shear_info.x_magnitude * pGroove->object_data.shear_info.x_period;
            } else if (pGroove->object_mode == eMove_absolute) {
                x_size = pGroove->object_data.shear_info.x_period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.shear_info.x_period) * 2.0 <= pGroove->object_data.shear_info.x_period) {
                    x_size = pGroove->object_data.shear_info.x_magnitude;
                } else {
                    x_size = -pGroove->object_data.shear_info.x_magnitude;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.shear_info.x_period != 0.0) {
                    x_size = sin(
                                 BrAngleToRadian(
                                     BrDegreeToAngle(
                                         fmod(pTime, pGroove->object_data.shear_info.x_period) / pGroove->object_data.shear_info.x_period * 360.0)))
                        * pGroove->object_data.shear_info.x_magnitude;
                }
            } else { // linear
                if (pGroove->object_data.shear_info.x_period != 0.0) {
                    x_size = fmod(pTime, pGroove->object_data.shear_info.x_period) / pGroove->object_data.shear_info.x_period;
                    x_size = MapSawToTriangle(x_size) * pGroove->object_data.shear_info.x_magnitude;
                }
            }

            BrMatrix34PostShearY(pMat, x_size / (bounds->max.v[0] - bounds->min.v[0]), z_size / (bounds->max.v[2] - bounds->min.v[2]));
        } else { // x_magnitude == 0
            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.shear_info.y_period != 0.0) {
                    y_size = fmod(pTime, pGroove->object_data.shear_info.y_period) / pGroove->object_data.shear_info.y_period * pGroove->object_data.shear_info.y_magnitude;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                y_size = pGroove->object_data.shear_info.y_magnitude * pGroove->object_data.shear_info.y_period;
            } else if (pGroove->object_mode == eMove_absolute) {
                y_size = pGroove->object_data.shear_info.y_period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.shear_info.y_period) * 2.0 <= pGroove->object_data.shear_info.y_period) {
                    y_size = pGroove->object_data.shear_info.y_magnitude;
                } else {
                    y_size = -pGroove->object_data.shear_info.y_magnitude;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.shear_info.y_period != 0.0) {
                    y_size = sin(
                                 BrAngleToRadian(
                                     BrDegreeToAngle(
                                         fmod(pTime, pGroove->object_data.shear_info.y_period) / pGroove->object_data.shear_info.y_period * 360.0)))
                        * pGroove->object_data.shear_info.y_magnitude;
                }
            } else {
                if (pGroove->object_data.shear_info.y_period != 0.0) {
                    y_size = fmod(pTime, pGroove->object_data.shear_info.y_period) / pGroove->object_data.shear_info.y_period;
                    y_size = MapSawToTriangle(y_size) * pGroove->object_data.shear_info.y_magnitude;
                }
            }

            if (pGroove->object_mode == eMove_continuous) {
                if (pGroove->object_data.shear_info.x_period != 0.0) {
                    x_size = fmod(pTime, pGroove->object_data.shear_info.x_period) / pGroove->object_data.shear_info.x_period * pGroove->object_data.shear_info.x_magnitude;
                }
            } else if (pGroove->object_mode == eMove_controlled) {
                x_size = pGroove->object_data.shear_info.x_magnitude * pGroove->object_data.shear_info.x_period;
            } else if (pGroove->object_mode == eMove_absolute) {
                x_size = pGroove->object_data.shear_info.x_period;
            } else if (pGroove->object_mode == eMove_flash) {
                if (fmod(pTime, pGroove->object_data.shear_info.x_period) * 2.0 <= pGroove->object_data.shear_info.x_period) {
                    x_size = pGroove->object_data.shear_info.x_magnitude;
                } else {
                    x_size = -pGroove->object_data.shear_info.x_magnitude;
                }
            } else if (pGroove->object_mode == eMove_harmonic) {
                if (pGroove->object_data.shear_info.x_period != 0.0) {
                    x_size = sin(
                                 BrAngleToRadian(
                                     BrDegreeToAngle(
                                         fmod(pTime, pGroove->object_data.shear_info.x_period) / pGroove->object_data.shear_info.x_period * 360.0)))
                        * pGroove->object_data.shear_info.x_magnitude;
                }
            } else {
                if (pGroove->object_data.shear_info.x_period != 0.0) {
                    x_size = fmod(pTime, pGroove->object_data.shear_info.x_period) / pGroove->object_data.shear_info.x_period;
                    x_size = MapSawToTriangle(x_size) * pGroove->object_data.shear_info.x_magnitude;
                }
            }
            BrMatrix34PostShearZ(pMat, x_size / (bounds->max.v[0] - bounds->min.v[0]), y_size / (bounds->max.v[1] - bounds->min.v[1]));
        }
        break;
    default:
        return;
    }
}

// IDA: void __usercall GrooveThisDelic(tGroovidelic_spec *pGroove@<EAX>, tU32 pTime@<EDX>, int pInterrupt_it@<EBX>)
void GrooveThisDelic(tGroovidelic_spec* pGroove, tU32 pTime, int pInterrupt_it) {
    br_actor* the_actor;
    br_vector3 actor_pos;
    br_matrix34* the_mat;
    tInterrupt_status old_path_interrupt;
    tInterrupt_status old_object_interrupt;
    LOG_TRACE8("(%p, %d, %d)", pGroove, pTime, pInterrupt_it);

    old_path_interrupt = pGroove->path_interrupt_status;
    old_object_interrupt = pGroove->object_interrupt_status;
    the_actor = pGroove->actor;
    pGroove->done_this_frame = 1;
    CalcActorGlobalPos(&actor_pos, the_actor);
    if (PointOutOfSight(&actor_pos, pGroove->mode == eGroove_mode_distance ? gYon_squared : 36.f)) {
        return;
    }

    the_mat = &the_actor->t.t.mat;
    if (!gAction_replay_mode
        || !ReplayIsPaused()
        || pGroove->path_mode == eMove_controlled
        || pGroove->path_mode == eMove_absolute) {
        PathGrooveBastard(pGroove, pTime, the_mat, pInterrupt_it);
    }
    if ((pGroove->object_type != eGroove_object_none || pGroove->lollipop_mode != eLollipop_none)
        && (!gAction_replay_mode
            || !ReplayIsPaused()
            || pGroove->object_mode == eMove_controlled
            || pGroove->object_mode == eMove_absolute)) {
        the_mat->m[0][0] = 1.0f;
        the_mat->m[0][1] = 0.0f;
        the_mat->m[0][2] = 0.0f;
        the_mat->m[1][0] = 0.0f;
        the_mat->m[1][1] = 1.0f;
        the_mat->m[1][2] = 0.0f;
        the_mat->m[2][0] = 0.0f;
        the_mat->m[2][1] = 0.0f;
        the_mat->m[2][2] = 1.0f;
        the_mat->m[3][0] = -pGroove->object_centre.v[0];
        the_mat->m[3][1] = -pGroove->object_centre.v[1];
        the_mat->m[3][2] = -pGroove->object_centre.v[2];
        ObjectGrooveBastard(pGroove, pTime, the_mat, pInterrupt_it);
        the_actor->t.t.mat.m[3][0] += pGroove->object_position.v[0] + pGroove->object_centre.v[0];
        the_actor->t.t.mat.m[3][1] += pGroove->object_position.v[1] + pGroove->object_centre.v[1];
        the_actor->t.t.mat.m[3][2] += pGroove->object_position.v[2] + pGroove->object_centre.v[2];
        if (pGroove->lollipop_mode != eLollipop_none) {
            LollipopizeActor(pGroove->actor, &gCamera_to_world, pGroove->lollipop_mode);
        }
    }
    if (pGroove->path_interrupt_status != old_path_interrupt || pGroove->object_interrupt_status != old_object_interrupt) {
        PipeSingleGrooveStop(
            pGroove - gGroovidelics_array,
            the_mat,
            pGroove->path_interrupt_status,
            pGroove->object_interrupt_status,
            pGroove->path_resumption_value,
            pGroove->object_resumption_value);
    }
}

// IDA: void __cdecl GrooveThoseDelics()
void GrooveThoseDelics() {
    int i;
    tGroovidelic_spec* the_groove;
    float f_the_time;
    LOG_TRACE("()");

    if (gGroovidelics_array != NULL) {
        f_the_time = (double)GetTotalTime();
        gPrevious_groove_times[1] = gPrevious_groove_times[0];
        gPrevious_groove_times[0] = f_the_time;

        for (i = 0; i < gGroovidelics_array_size; i++) {
            the_groove = &gGroovidelics_array[i];
            if (the_groove->owner != -999 && !the_groove->done_this_frame) {
                GrooveThisDelic(the_groove, f_the_time, 0);
            }
        }
    }
}

// IDA: void __usercall StopGroovidelic(br_actor *pActor@<EAX>)
void StopGroovidelic(br_actor* pActor) {
    int i;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%p)", pActor);

    STUB();
}

// IDA: void __usercall SetGrooveInterrupt(int pGroove_index@<EAX>, br_matrix34 *pMatrix@<EDX>, int pPath_interrupt@<EBX>, int pObject_interrupt@<ECX>, float pPath_resumption, float pObject_resumption)
void SetGrooveInterrupt(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%d, %p, %d, %d, %f, %f)", pGroove_index, pMatrix, pPath_interrupt, pObject_interrupt, pPath_resumption, pObject_resumption);

    the_groove = &gGroovidelics_array[pGroove_index];
    the_groove->path_interrupt_status = pPath_interrupt;
    the_groove->object_interrupt_status = pObject_interrupt;
    the_groove->path_resumption_value = pPath_resumption;
    the_groove->object_resumption_value = pObject_resumption;
    BrMatrix34Copy(&the_groove->actor->t.t.mat, pMatrix);
}

// IDA: void __cdecl ResetGrooveFlags()
void ResetGrooveFlags() {
    int i;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("()");

    the_groove = gGroovidelics_array;
    for (i = 0; i < gGroovidelics_array_size; i++) {
        the_groove->done_this_frame = 0;
        the_groove++;
    }
}

// IDA: tSpecial_volume* __cdecl GetDefaultSpecialVolumeForWater()
tSpecial_volume* GetDefaultSpecialVolumeForWater() {
    LOG_TRACE("()");

    return gDefault_water_spec_vol;
}

// IDA: tSpecial_volume* __usercall FindSpecialVolume@<EAX>(br_vector3 *pP@<EAX>, tSpecial_volume *pLast_vol@<EDX>)
tSpecial_volume* FindSpecialVolume(br_vector3* pP, tSpecial_volume* pLast_vol) {
    int i;
    tSpecial_volume* v;
    br_vector3 p;
    LOG_TRACE("(%p, %p)", pP, pLast_vol);

    for (i = 0, v = gProgram_state.special_volumes; i < gProgram_state.special_volume_count; i++, v++) {
        if (!v->no_mat && v->bounds.min.v[0] < pP->v[0] && pP->v[0] < v->bounds.max.v[0] && v->bounds.min.v[1] < pP->v[1] && pP->v[1] < v->bounds.max.v[1] && v->bounds.min.v[2] < pP->v[2] && pP->v[2] < v->bounds.max.v[2]) {
            BrMatrix34ApplyP(&p, pP, &v->inv_mat);
            if (-1.f < p.v[0] && p.v[0] < 1.f && -1.f < p.v[1] && p.v[1] < 1.f && -1.f < p.v[2] && p.v[2] < 1.f) {
                return v;
            }
        }
    }
    return NULL;
}

// IDA: void __cdecl SaveAdditionalActors()
void SaveAdditionalActors() {
    LOG_TRACE("()");

    if (gAdditional_actors != NULL) {
        SaveAdditionalStuff();
    }
}

// IDA: br_scalar __usercall DistanceFromFace@<ST0>(br_vector3 *pPos@<EAX>, tFace_ref *pFace@<EDX>)
br_scalar DistanceFromFace(br_vector3* pPos, tFace_ref* pFace) {
    br_vector3 normal;
    LOG_TRACE("(%p, %p)", pPos, pFace);

    return BrVector3Dot(&pFace->normal, pPos) - pFace->d;
}

// IDA: br_uint_32 __cdecl CalcHighestID(br_actor *pActor, int *pHighest)
br_uint_32 CalcHighestID(br_actor* pActor, int* pHighest) {
    char s[256];
    int number;
    LOG_TRACE("(%p, %p)", pActor, pHighest);

    if (pActor->identifier == NULL || pActor->identifier[0] == '@') {
        return 0;
    }
    strcpy(s, &pActor->identifier[4]);
    s[4] = '\0';
    sscanf(s, "%d", &number);
    if (*pHighest < number) {
        *pHighest = number;
    }
    return 0;
}

// IDA: br_uint_32 __cdecl SetID(br_actor *pActor, void *pArg)
br_uint_32 SetID(br_actor* pActor, void* pArg) {
    char s[256];
    LOG_TRACE("(%p, %p)", pActor, pArg);

    if (pActor->identifier == NULL) {
        return 0;
    }
    strcpy(s, pActor->identifier);
    strtok(s, ".");
    strcat(s, "0000");
    sprintf(&s[4], "%04d", (int)(intptr_t)pArg);
    strcat(s, ".ACT");
    BrResFree(pActor->identifier);
    pActor->identifier = BrResStrDup(pActor, s);
    return 0;
}

// IDA: void __usercall UniquificateActorsName(br_actor *pUniverse_actor@<EAX>, br_actor *pActor@<EDX>)
void UniquificateActorsName(br_actor* pUniverse_actor, br_actor* pActor) {
    int highest;
    LOG_TRACE("(%p, %p)", pUniverse_actor, pActor);

    if (pActor->identifier == NULL || pActor->identifier[0] == '@') {
        return;
    }
    highest = 0;
    DRActorEnumRecurse(pUniverse_actor, (br_actor_enum_cbfn*)CalcHighestNonAmID, &highest);
    DRActorEnumRecurse(pActor, (br_actor_enum_cbfn*)SetID, (void*)(uintptr_t)(highest + 1));
}

// IDA: void __usercall AccessoryHeadup(br_actor *pActor@<EAX>, char *pPrefix@<EDX>)
void AccessoryHeadup(br_actor* pActor, char* pPrefix) {
    char s[256];
    int i;
    br_actor* original_actor;
    LOG_TRACE("(%p, \"%s\")", pActor, pPrefix);

    strcpy(s, pPrefix);
    if (pActor->identifier != NULL) {
        strcat(s, pActor->identifier);
    }
    NewTextHeadupSlot(4, 0, 2000, -2, s);
}

// IDA: br_uint_32 __cdecl CalcHighestNonAmID(br_actor *pActor, int *pHighest)
br_uint_32 CalcHighestNonAmID(br_actor* pActor, int* pHighest) {
    char s[256];
    int number;
    LOG_TRACE("(%p, %p)", pActor, pHighest);

    if (pActor->identifier == NULL || pActor->identifier[0] == '&') {
        return 0;
    }
    if (strlen(pActor->identifier) == 12) {
        strcpy(s, &pActor->identifier[4]);
        strtok(s, ".");
        sscanf(s, "%d", &number);
    } else {
        number = 0;
    }
    if (*pHighest < number) {
        *pHighest = number;
    }
    return 0;
}

// IDA: br_uint_32 __cdecl SetIDAndDupModel(br_actor *pActor, void *pArg)
br_uint_32 SetIDAndDupModel(br_actor* pActor, void* pArg) {
    char s[256];
    char s2[256];
    br_model* new_model;
    LOG_TRACE("(%p, %p)", pActor, pArg);

    if (pActor->identifier == NULL || pActor->identifier[0] == '@') {
        return 0;
    }
    *(int*)(uintptr_t)pArg = *(int*)(uintptr_t)pArg + 1;
    strcpy(s, pActor->identifier);
    s[0] = '@';
    strtok(s, ".");
    strcat(s, "0000");
    sprintf(&s[4], "%04d", *(int*)(uintptr_t)pArg);
    strcpy(s2, s);
    strcat(s, ".ACT");
    BrResFree(pActor->identifier);
    pActor->identifier = BrResStrDup(pActor, s);
    if (pActor->model != NULL) {
        strcat(s2, ".DAT");
        new_model = BrModelAllocate(s2, pActor->model->nvertices, pActor->model->nfaces);
        memcpy(new_model->vertices, pActor->model->vertices, pActor->model->nvertices * sizeof(br_vertex));
        memcpy(new_model->faces, pActor->model->faces, pActor->model->nfaces * sizeof(br_face));
        new_model->flags |= 0x80; // FIXME: unknown model flag
        BrModelAdd(new_model);
        BrModelUpdate(new_model, BR_MODU_ALL);
        pActor->model = new_model;
        gAdditional_models[gNumber_of_additional_models] = new_model;
        gNumber_of_additional_models++;
    }
    return 0;
}

// IDA: void __usercall DuplicateIfNotAmpersand(br_actor *pActor@<EAX>)
void DuplicateIfNotAmpersand(br_actor* pActor) {
    int highest;
    LOG_TRACE("(%p)", pActor);

    if (pActor->identifier != NULL && pActor->identifier[0] != '&') {
        highest = 0;
        DRActorEnumRecurse(gUniverse_actor, (br_actor_enum_cbfn*)CalcHighestID, &highest);
        DRActorEnumRecurse(pActor, (br_actor_enum_cbfn*)SetIDAndDupModel, &highest);
    }
}

// IDA: void __usercall DropActor(int pIndex@<EAX>)
void DropActor(int pIndex) {
    FILE* f;
    tPath_name the_path;
    char s[256];
    int i;
    int face_bastard;
    int face_count;
    tBounds kev_bounds;
    tFace_ref the_list[50];
    br_scalar nearest_bastard;
    br_scalar distance_bastard;
    br_scalar ts;
    br_vector3 side_vector;
    br_angle phi;
    br_matrix34 mat;
    br_transform new_transform;
    br_actor* a;
    br_actor* last_non_ampersand;
    LOG_TRACE("(%d)", pIndex);

    if (PDKeyDown(KEY_CTRL_ANY)) {
        pIndex += 20;
    }
    if (PDKeyDown(KEY_ALT_ANY)) {
        pIndex += 10;
    }
    PathCat(the_path, gApplication_path, "ACCESSRY.TXT");
    f = DRfopen(the_path, "rt");
    for (i = 0; i <= pIndex; i++) {
        if (!feof(f)) {
            GetAString(f, s);
        } else {
            s[0] = '\0';
        }
    }
    if (s[0] != '\0') {
        gLast_actor = LoadActor(s);
        if (gLast_actor != NULL && gLast_actor->model != NULL) {
            BrVector3Set(&kev_bounds.original_bounds.min, -.05f, -.05f, -.05f);
            BrVector3Set(&kev_bounds.original_bounds.max, .05f, .05f, .05f);
            kev_bounds.mat = &gProgram_state.current_car.car_master_actor->t.t.mat;

            do {
                face_count = FindFacesInBox(&kev_bounds, the_list, COUNT_OF(the_list));
                BrVector3Scale(&kev_bounds.original_bounds.min, &kev_bounds.original_bounds.min, 2.f);
                BrVector3Scale(&kev_bounds.original_bounds.max, &kev_bounds.original_bounds.max, 2.f);
            } while (face_count == 0);

            nearest_bastard = FLT_MAX;
            face_bastard = -1;
            for (i = 0; i < face_count; i++) {
                distance_bastard = DistanceFromFace(gOur_pos, &the_list[i]);
                if (distance_bastard < nearest_bastard) {
                    nearest_bastard = distance_bastard;
                    face_bastard = i;
                }
            }

            if (face_bastard >= 0) {
                BrVector3Scale(&gLast_actor->t.t.translate.t, &the_list[face_bastard].normal, -nearest_bastard);
                BrVector3Accumulate(&gLast_actor->t.t.translate.t, gOur_pos);
                if (!PDKeyDown(KEY_SHIFT_ANY)) {
                    if (the_list[face_bastard].normal.v[1] > the_list[face_bastard].normal.v[0] && the_list[face_bastard].normal.v[2] > the_list[face_bastard].normal.v[0]) {
                        BrVector3Set(&side_vector, -1.f, 0.f, 0.f);
                    } else if (the_list[face_bastard].normal.v[0] > the_list[face_bastard].normal.v[1] && the_list[face_bastard].normal.v[2] > the_list[face_bastard].normal.v[1]) {
                        BrVector3Set(&side_vector, 0.f, -1.f, 0.f);
                    } else {
                        BrVector3Set(&side_vector, 0.f, 0.f, -1.f);
                    }
                    new_transform.type = BR_TRANSFORM_LOOK_UP;
                    BrVector3Cross(&new_transform.t.look_up.look, &the_list[face_bastard].normal, &side_vector);
                    BrVector3Copy(&new_transform.t.look_up.up, &the_list[face_bastard].normal);
                    BrVector3Copy(&new_transform.t.look_up.t, &gLast_actor->t.t.translate.t);
                    BrTransformToTransform(&gLast_actor->t, &new_transform);
                }

                gKnown_actor = gLast_actor;
                BrVector3Copy(&gActor_centre, &gLast_actor->t.t.translate.t);
                DuplicateIfNotAmpersand(gLast_actor);
                UniquificateActorsName(gUniverse_actor, gLast_actor);
                gLast_actor->model->flags |= 0x80; // FIXME: unknown flag
                if (gLast_actor->identifier == NULL || gLast_actor->identifier[0] == '&') {
                    last_non_ampersand = gAdditional_actors;
                    for (a = gAdditional_actors->children; a != NULL; a = a->next) {
                        if (a->identifier != NULL && a->identifier[0] != '&') {
                            last_non_ampersand = a;
                        }
                    }
                    BrActorAdd(last_non_ampersand, gLast_actor);
                } else {
                    BrActorAdd(gAdditional_actors, gLast_actor);
                }
                SaveAdditionalStuff();
                AccessoryHeadup(gLast_actor, "Shat out ");
            }
        }
    }
    fclose(f);
}

// IDA: void __cdecl DropActor0()
void DropActor0() {
    LOG_TRACE("()");

    DropActor(0);
}

// IDA: void __cdecl DropActor1()
void DropActor1() {
    LOG_TRACE("()");

    DropActor(1);
}

// IDA: void __cdecl DropActor2()
void DropActor2() {
    LOG_TRACE("()");

    DropActor(2);
}

// IDA: void __cdecl DropActor3()
void DropActor3() {
    LOG_TRACE("()");

    DropActor(3);
}

// IDA: void __cdecl DropActor4()
void DropActor4() {
    LOG_TRACE("()");

    DropActor(4);
}

// IDA: void __cdecl DropActor5()
void DropActor5() {
    LOG_TRACE("()");

    DropActor(5);
}

// IDA: void __cdecl DropActor6()
void DropActor6() {
    LOG_TRACE("()");

    DropActor(6);
}

// IDA: void __cdecl DropActor7()
void DropActor7() {
    LOG_TRACE("()");

    DropActor(7);
}

// IDA: void __cdecl DropActor8()
void DropActor8() {
    LOG_TRACE("()");

    DropActor(8);
}

// IDA: void __cdecl DropActor9()
void DropActor9() {
    LOG_TRACE("()");

    DropActor(9);
}

// IDA: br_uint_32 __cdecl IdentifyAccCB(br_actor *pActor, void *pArg)
br_uint_32 IdentifyAccCB(br_actor* pActor, void* pArg) {
    br_scalar distance;
    char s[256];
    br_vector3 v;
    LOG_TRACE("(%p, %p)", pActor, pArg);

    if (pActor == NULL || pActor->model == NULL) {
        return 0;
    }
    BrVector3Add(&v, &pActor->model->bounds.max, &pActor->model->bounds.min);
    BrVector3InvScale(&v, &v, 2.f);
    BrVector3Accumulate(&v, &pActor->t.t.translate.t);
    BrVector3Sub(&v, &v, gOur_pos);
    distance = BrVector3LengthSquared(&v);
    if (distance < gNearest_distance) {
        gNearest_actor = pActor;
        gNearest_distance = distance;
    }
    return 0;
}

// IDA: void __cdecl IdentifyAcc()
void IdentifyAcc() {
    LOG_TRACE("()");

    gNearest_distance = FLT_MAX;
    gNearest_actor = NULL;
    DRActorEnumRecurse(gAdditional_actors, (br_actor_enum_cbfn*)IdentifyAccCB, NULL);
    if (gNearest_actor != NULL) {
        gLast_actor = gNearest_actor;
        AccessoryHeadup(gNearest_actor, "Locked onto ");
    }
}

// IDA: br_uint_32 __cdecl DelGrooveRef(br_actor *pActor, void *pArg)
br_uint_32 DelGrooveRef(br_actor* pActor, void* pArg) {
    tGroovidelic_spec* the_groove;
    int i;
    LOG_TRACE("(%p, %p)", pActor, pArg);

    for (i = 0; i < gGroovidelics_array_size; i++) {
        the_groove = &gGroovidelics_array[i];
        if (the_groove->actor == pActor) {
            the_groove->owner = -999;
        }
    }
    return 0;
}

// IDA: br_uint_32 __cdecl DelReferencedModels(br_actor *pActor, void *pArg)
br_uint_32 DelReferencedModels(br_actor* pActor, void* pArg) {
    tGroovidelic_spec* the_groove;
    int i;
    LOG_TRACE("(%p, %p)", pActor, pArg);

    for (i = 0; i < gNumber_of_additional_models; i++) {
        if (pActor->model == gAdditional_models[i]) {
            BrModelRemove(pActor->model);
            BrModelFree(pActor->model);
            memmove(&gAdditional_models[i], &gAdditional_models[i + 1], (gNumber_of_additional_models - i - 1) * sizeof(br_model*));
            gNumber_of_additional_models--;
        }
    }
    return 0;
}

// IDA: void __cdecl DeleteAcc()
void DeleteAcc() {
    LOG_TRACE("()");

    if (gLast_actor == NULL) {
        return;
    }
    AccessoryHeadup(gLast_actor, "Murdered ");
    DRActorEnumRecurse(gLast_actor, (br_actor_enum_cbfn*)DelReferencedModels, NULL);
    DRActorEnumRecurse(gLast_actor, (br_actor_enum_cbfn*)DelGrooveRef, NULL);
    BrActorRemove(gLast_actor);
    BrActorFree(gLast_actor);
    gLast_actor = NULL;
    SaveAdditionalStuff();
}

// IDA: br_uint_32 __cdecl OffsetModel(br_actor *pActor, void *pArg)
br_uint_32 OffsetModel(br_actor* pActor, void* pArg) {
    int i;
    LOG_TRACE("(%p, %p)", pActor, pArg);

    if (pActor->model == NULL) {
        return 0;
    }
    for (i = 0; i < pActor->model->nvertices; i++) {
        BrVector3Accumulate(&pActor->model->vertices[i].p, (br_vector3*)pArg);
    }
    return 0;
}

// IDA: void __usercall OffsetActor(br_actor *pActor@<EAX>, br_vector3 *pOffset@<EDX>)
void OffsetActor(br_actor* pActor, br_vector3* pOffset) {
    LOG_TRACE("(%p, %p)", pActor, pOffset);

    DRActorEnumRecurse(pActor, (br_actor_enum_cbfn*)OffsetModel, pOffset);
}

// IDA: void __usercall CentreActor(br_actor *pActor@<EAX>, br_vector3 *pOffset@<EDX>)
void CentreActor(br_actor* pActor, br_vector3* pOffset) {
    LOG_TRACE("(%p, %p)", pActor, pOffset);

    if (pActor->model == NULL) {
        BrVector3Set(pOffset, 0.f, 0.f, 0.f);
    } else if (gKnown_actor == gLast_actor) {
        BrVector3Scale(pOffset, &gActor_centre, -1.f);
    } else {
        BrVector3Add(pOffset, &pActor->model->bounds.max, &pActor->model->bounds.min);
        BrVector3Scale(pOffset, pOffset, -2.f);
    }
    DRActorEnumRecurse(pActor, (br_actor_enum_cbfn*)OffsetModel, pOffset);
    BrVector3Scale(pOffset, pOffset, -1.f);
}

// IDA: void __cdecl SnapAccToVertical()
void SnapAccToVertical() {
    LOG_TRACE("()");

    if (gLast_actor == NULL) {
        return;
    }
    BrVector3Set((br_vector3*)gLast_actor->t.t.mat.m[0], 1.f, 0.f, 0.f);
    BrVector3Set((br_vector3*)gLast_actor->t.t.mat.m[1], 0.f, 1.f, 0.f);
    BrVector3Set((br_vector3*)gLast_actor->t.t.mat.m[2], 0.f, 0.f, 1.f);
    SaveAdditionalStuff();
}

// IDA: void __usercall RotateAccessory(br_angle pAngle@<EAX>)
void RotateAccessory(br_angle pAngle) {
    br_vector3 mr_offset;
    LOG_TRACE("(%d)", pAngle);

    if (gLast_actor == NULL) {
        return;
    }
    if (!gSpec_vol_mode && gLast_actor->identifier != NULL && gLast_actor->identifier[0] == '@') {
        CentreActor(gLast_actor, &mr_offset);
    }
    switch (gCurrent_rotate_mode) {
    case eRotate_mode_x:
        BrMatrix34PreRotateX(&gLast_actor->t.t.mat, pAngle);
        break;
    case eRotate_mode_y:
        BrMatrix34PreRotateY(&gLast_actor->t.t.mat, pAngle);
        break;
    case eRotate_mode_z:
        BrMatrix34PreRotateZ(&gLast_actor->t.t.mat, pAngle);
        break;
    }
    if (!gSpec_vol_mode && gLast_actor->identifier != NULL && gLast_actor->identifier[0] == '@') {
        DRActorEnumRecurseWithTrans(gLast_actor, NULL, ApplyTransToModels, NULL);
        OffsetActor(gLast_actor, &mr_offset);
    }
    SaveAdditionalStuff();
}

// IDA: void __cdecl ScaleAccessory(float pScaling_factor)
void ScaleAccessory(float pScaling_factor) {
    br_vector3 mr_offset;
    LOG_TRACE("(%f)", pScaling_factor);

    if (gLast_actor == NULL) {
        return;
    }
    if (!gSpec_vol_mode && gLast_actor->identifier != NULL && gLast_actor->identifier[0] == '@') {
        CentreActor(gLast_actor, &mr_offset);
    }
    switch (gCurrent_scale_mode) {
    case eScale_mode_all:
        BrMatrix34PreScale(&gLast_actor->t.t.mat, pScaling_factor, pScaling_factor, pScaling_factor);
        break;
    case eScale_mode_x:
        BrMatrix34PreScale(&gLast_actor->t.t.mat, pScaling_factor, 1.f, 1.f);
        break;
    case eScale_mode_y:
        BrMatrix34PreScale(&gLast_actor->t.t.mat, 1.f, pScaling_factor, 1.f);
        break;
    case eScale_mode_z:
        BrMatrix34PreScale(&gLast_actor->t.t.mat, 1.f, 1.f, pScaling_factor);
        break;
    }
    if (!gSpec_vol_mode && gLast_actor->identifier != NULL && gLast_actor->identifier[0] == '@') {
        DRActorEnumRecurseWithTrans(gLast_actor, NULL, ApplyTransToModels, NULL);
        OffsetActor(gLast_actor, &mr_offset);
    }
    SaveAdditionalStuff();
}

// IDA: void __cdecl MoveAccessory(br_scalar pX_shift, br_scalar pY_shift, br_scalar pZ_shift)
void MoveAccessory(br_scalar pX_shift, br_scalar pY_shift, br_scalar pZ_shift) {
    br_vector3 v;
    LOG_TRACE("(%f, %f, %f)", pX_shift, pY_shift, pZ_shift);

    if (gLast_actor == NULL) {
        return;
    }
    BrVector3SetFloat(&v, pX_shift, pY_shift, pZ_shift);
    BrVector3Accumulate(&gLast_actor->t.t.translate.t, &v);
    SaveAdditionalStuff();
}

// IDA: void __cdecl RotateAccL()
void RotateAccL() {
    LOG_TRACE("()");

    RotateAccessory(BrDegreeToAngle(90));
}

// IDA: void __cdecl RotateAccL2()
void RotateAccL2() {
    LOG_TRACE("()");

    RotateAccessory(BrDegreeToAngle(15));
}

// IDA: void __cdecl RotateAccL3()
void RotateAccL3() {
    LOG_TRACE("()");

    RotateAccessory(BrDegreeToAngle(5));
}

// IDA: void __cdecl RotateAccL4()
void RotateAccL4() {
    LOG_TRACE("()");

    RotateAccessory(BrDegreeToAngle(1));
}

// IDA: void __cdecl RotateAccR()
void RotateAccR() {
    LOG_TRACE("()");

    RotateAccessory(BrDegreeToAngle(270));
}

// IDA: void __cdecl RotateAccR2()
void RotateAccR2() {
    LOG_TRACE("()");

    RotateAccessory(BrDegreeToAngle(345));
}

// IDA: void __cdecl RotateAccR3()
void RotateAccR3() {
    LOG_TRACE("()");

    RotateAccessory(BrDegreeToAngle(355));
}

// IDA: void __cdecl RotateAccR4()
void RotateAccR4() {
    LOG_TRACE("()");

    RotateAccessory(BrDegreeToAngle(359));
}

// IDA: void __cdecl CycleAccRotate()
void CycleAccRotate() {
    LOG_TRACE("()");

    gCurrent_rotate_mode = (gCurrent_rotate_mode == eRotate_mode_z) ? eRotate_mode_x : (gCurrent_rotate_mode + 1);
    switch (gCurrent_rotate_mode) {
    case eRotate_mode_x:
        NewTextHeadupSlot(4, 0, 2000, -2, "Rotate mode: X");
        break;
    case eRotate_mode_y:
        NewTextHeadupSlot(4, 0, 2000, -2, "Rotate mode: Y");
        break;
    case eRotate_mode_z:
        NewTextHeadupSlot(4, 0, 2000, -2, "Rotate mode: Z");
        break;
    }
}

// IDA: void __cdecl CycleAccScale()
void CycleAccScale() {
    LOG_TRACE("()");

    gCurrent_scale_mode = (gCurrent_scale_mode == eScale_mode_z) ? eScale_mode_all : (gCurrent_scale_mode + 1);
    switch (gCurrent_scale_mode) {
    case eScale_mode_all:
        NewTextHeadupSlot(4, 0, 2000, -2, "Scale mode: ALL");
        break;
    case eScale_mode_x:
        NewTextHeadupSlot(4, 0, 2000, -2, "Scale mode: X");
        break;
    case eScale_mode_y:
        NewTextHeadupSlot(4, 0, 2000, -2, "Scale mode: Y");
        break;
    case eScale_mode_z:
        NewTextHeadupSlot(4, 0, 2000, -2, "Scale mode: Z");
        break;
    }
}

// IDA: void __cdecl ScaleAccUp2()
void ScaleAccUp2() {
    LOG_TRACE("()");

    ScaleAccessory(1.2f);
}

// IDA: void __cdecl ScaleAccUp3()
void ScaleAccUp3() {
    LOG_TRACE("()");

    ScaleAccessory(1.05f);
}

// IDA: void __cdecl ScaleAccUp4()
void ScaleAccUp4() {
    LOG_TRACE("()");

    ScaleAccessory(1.002f);
}

// IDA: void __cdecl ScaleAccDown2()
void ScaleAccDown2() {
    LOG_TRACE("()");

    ScaleAccessory(1 / 1.2f);
}

// IDA: void __cdecl ScaleAccDown3()
void ScaleAccDown3() {
    LOG_TRACE("()");

    ScaleAccessory(1 / 1.05f);
}

// IDA: void __cdecl ScaleAccDown4()
void ScaleAccDown4() {
    LOG_TRACE("()");

    ScaleAccessory(1 / 1.002f);
}

// IDA: void __cdecl MoveXAccL()
void MoveXAccL() {
    LOG_TRACE("()");

    MoveAccessory(1.f, 0.f, 0.f);
}

// IDA: void __cdecl MoveXAccL2()
void MoveXAccL2() {
    LOG_TRACE("()");

    MoveAccessory(.2f, 0.f, 0.f);
}

// IDA: void __cdecl MoveXAccL3()
void MoveXAccL3() {
    LOG_TRACE("()");

    MoveAccessory(.02f, 0.f, 0.f);
}

// IDA: void __cdecl MoveXAccL4()
void MoveXAccL4() {
    LOG_TRACE("()");

    MoveAccessory(.002f, 0.f, 0.f);
}

// IDA: void __cdecl MoveXAccR()
void MoveXAccR() {
    LOG_TRACE("()");

    MoveAccessory(-1.f, 0.f, 0.f);
}

// IDA: void __cdecl MoveXAccR2()
void MoveXAccR2() {
    LOG_TRACE("()");

    MoveAccessory(-.2f, 0.f, 0.f);
}

// IDA: void __cdecl MoveXAccR3()
void MoveXAccR3() {
    LOG_TRACE("()");

    MoveAccessory(-.02f, 0.f, 0.f);
}

// IDA: void __cdecl MoveXAccR4()
void MoveXAccR4() {
    LOG_TRACE("()");

    MoveAccessory(-.002f, 0.f, 0.f);
}

// IDA: void __cdecl MoveYAccL()
void MoveYAccL() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 1.f, 0.f);
}

// IDA: void __cdecl MoveYAccL2()
void MoveYAccL2() {
    LOG_TRACE("()");

    MoveAccessory(0.f, .2f, 0.f);
}

// IDA: void __cdecl MoveYAccL3()
void MoveYAccL3() {
    LOG_TRACE("()");

    MoveAccessory(0.f, .02f, 0.f);
}

// IDA: void __cdecl MoveYAccL4()
void MoveYAccL4() {
    LOG_TRACE("()");

    MoveAccessory(0.f, .002f, 0.f);
}

// IDA: void __cdecl MoveYAccR()
void MoveYAccR() {
    LOG_TRACE("()");

    MoveAccessory(0.f, -1.f, 0.f);
}

// IDA: void __cdecl MoveYAccR2()
void MoveYAccR2() {
    LOG_TRACE("()");

    MoveAccessory(0.f, -.2f, 0.f);
}

// IDA: void __cdecl MoveYAccR3()
void MoveYAccR3() {
    LOG_TRACE("()");

    MoveAccessory(0.f, -.02f, 0.f);
}

// IDA: void __cdecl MoveYAccR4()
void MoveYAccR4() {
    LOG_TRACE("()");

    MoveAccessory(0.f, -.002f, 0.f);
}

// IDA: void __cdecl MoveZAccL()
void MoveZAccL() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 0.f, 1.f);
}

// IDA: void __cdecl MoveZAccL2()
void MoveZAccL2() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 0.f, .2f);
}

// IDA: void __cdecl MoveZAccL3()
void MoveZAccL3() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 0.f, .02f);
}

// IDA: void __cdecl MoveZAccL4()
void MoveZAccL4() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 0.f, .002f);
}

// IDA: void __cdecl MoveZAccR()
void MoveZAccR() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 0.f, -1.f);
}

// IDA: void __cdecl MoveZAccR2()
void MoveZAccR2() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 0.f, -.2f);
}

// IDA: void __cdecl MoveZAccR3()
void MoveZAccR3() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 0.f, -.02f);
}

// IDA: void __cdecl MoveZAccR4()
void MoveZAccR4() {
    LOG_TRACE("()");

    MoveAccessory(0.f, 0.f, -.002f);
}

// IDA: br_material* __cdecl GetInternalMat()
br_material* GetInternalMat() {
    LOG_TRACE("()");

    return BrMaterialFind("SPECVOL.MAT");
}

// IDA: br_material* __cdecl GetExternalMat()
br_material* GetExternalMat() {
    LOG_TRACE("()");

    return BrMaterialFind("SPECVOL2.MAT");
}

#define DrVertexSet(V, X, Y, Z) \
    do {                        \
        (V)[0] = (X);           \
        (V)[1] = (Y);           \
        (V)[2] = (Z);           \
    } while (0)

// IDA: void __usercall BuildSpecVolModel(tSpecial_volume *pSpec@<EAX>, int pIndex@<EDX>, br_material *pInt_mat@<EBX>, br_material *pExt_mat@<ECX>)
void BuildSpecVolModel(tSpecial_volume* pSpec, int pIndex, br_material* pInt_mat, br_material* pExt_mat) {
    int i;
    int j;
    int temp;
    tSpecial_volume* v;
    br_actor* actor;
    br_model* model;
    LOG_TRACE("(%p, %d, %p, %p)", pSpec, pIndex, pInt_mat, pExt_mat);

    actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
    BrMatrix34Copy(&actor->t.t.mat, &pSpec->mat);
    actor->render_style = BR_RSTYLE_FACES;
    model = BrModelAllocate("", 12, 24);
    model->flags |= BR_MODF_KEEP_ORIGINAL | BR_MODF_DONT_WELD;
    BrVector3Set(&model->vertices[0].p, 1.f, -1.f, -1.f);
    BrVector2Set(&model->vertices[0].map, 1.f, 0.f);
    BrVector3Set(&model->vertices[1].p, -1.f, -1.f, -1.f);
    BrVector2Set(&model->vertices[1].map, 0.f, 0.f);
    BrVector3Set(&model->vertices[2].p, -1.f, 1.f, -1.f);
    BrVector2Set(&model->vertices[2].map, 0.f, 1.f);
    BrVector3Set(&model->vertices[3].p, 1.f, 1.f, -1.f);
    BrVector2Set(&model->vertices[3].map, 1.f, 1.f);
    BrVector3Set(&model->vertices[4].p, 1.f, -1.f, 1.f);
    BrVector2Set(&model->vertices[4].map, 1.f, 1.f);
    BrVector3Set(&model->vertices[5].p, -1.f, -1.f, 1.f);
    BrVector2Set(&model->vertices[5].map, 0.f, 1.f);
    BrVector3Set(&model->vertices[6].p, -1.f, 1.f, 1.f);
    BrVector2Set(&model->vertices[6].map, 0.f, 0.f);
    BrVector3Set(&model->vertices[7].p, 1.f, 1.f, 1.f);
    BrVector2Set(&model->vertices[7].map, 1.f, 0.f);
    BrVector3Set(&model->vertices[8].p, 1.f, -1.f, -1.f);
    BrVector2Set(&model->vertices[8].map, 0.f, 1.f);
    BrVector3Set(&model->vertices[9].p, 1.f, -1.f, 1.f);
    BrVector2Set(&model->vertices[9].map, 0.f, 0.f);
    BrVector3Set(&model->vertices[10].p, -1.f, -1.f, -1.f);
    BrVector2Set(&model->vertices[10].map, 1.f, 1.f);
    BrVector3Set(&model->vertices[11].p, -1.f, -1.f, 1.f);
    BrVector2Set(&model->vertices[11].map, 1.f, 0.f);

    DrVertexSet(model->faces[0].vertices, 0, 3, 1);
    DrVertexSet(model->faces[1].vertices, 1, 3, 2);
    DrVertexSet(model->faces[2].vertices, 3, 7, 2);
    DrVertexSet(model->faces[3].vertices, 2, 7, 6);
    DrVertexSet(model->faces[4].vertices, 6, 7, 4);
    DrVertexSet(model->faces[5].vertices, 6, 4, 5);
    DrVertexSet(model->faces[6].vertices, 0, 5, 4);
    DrVertexSet(model->faces[7].vertices, 1, 5, 0);
    DrVertexSet(model->faces[8].vertices, 9, 7, 8);
    DrVertexSet(model->faces[9].vertices, 8, 7, 3);
    DrVertexSet(model->faces[10].vertices, 11, 2, 6);
    DrVertexSet(model->faces[11].vertices, 11, 10, 2);

    memcpy(&model->faces[12], model->faces, 12 * sizeof(br_face));
    for (i = 12; i < 24; i++) {
        temp = model->faces[i].vertices[0];
        model->faces[i].vertices[0] = model->faces[i].vertices[1];
        model->faces[i].vertices[1] = temp;
    }
    model->faces[5].material = model->faces[4].material = model->faces[1].material = model->faces[0].material = DRMaterialClone(pExt_mat);
    model->faces[11].material = model->faces[10].material = model->faces[9].material = model->faces[8].material = DRMaterialClone(pExt_mat);
    model->faces[7].material = model->faces[6].material = model->faces[3].material = model->faces[2].material = DRMaterialClone(pExt_mat);
    model->faces[17].material = model->faces[16].material = model->faces[13].material = model->faces[12].material = DRMaterialClone(pInt_mat);
    model->faces[23].material = model->faces[22].material = model->faces[21].material = model->faces[20].material = DRMaterialClone(pInt_mat);
    model->faces[19].material = model->faces[18].material = model->faces[15].material = model->faces[14].material = DRMaterialClone(pInt_mat);
    BrModelUpdate(model, BR_MODU_ALL);
    actor->model = model;
    BrActorAdd(gNon_track_actor, actor);
    gSpec_vol_actors[pIndex] = actor;
    SetSpecVolMatSize(actor);
}

#undef DrVertexSet

// IDA: void __usercall DropSpecVol(int pIndex@<EAX>)
void DropSpecVol(int pIndex) {
    FILE* f;
    tPath_name the_path;
    int i;
    int spec_count;
    tSpecial_volume spec;
    tSpecial_volume* new_specs;
    char s[256];
    LOG_TRACE("(%d)", pIndex);

    PathCat(the_path, gApplication_path, "SPECVOL.TXT");
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        return;
    }
    spec_count = GetAnInt(f);
    // pIndex = 1 means first special volume
    if (pIndex > spec_count) {
        fclose(f);
        return;
    }
    for (i = 0; i < pIndex; i++) {
        ParseSpecialVolume(f, &spec, NULL);
    }
    spec.no_mat = 0;
    BrMatrix34Copy(&spec.mat, &gProgram_state.current_car.car_master_actor->t.t.mat);
    new_specs = BrMemAllocate((gProgram_state.special_volume_count + 1) * sizeof(tSpecial_volume), kMem_new_special_vol);
    memcpy(new_specs, gProgram_state.special_volumes, gProgram_state.special_volume_count * sizeof(tSpecial_volume));
    memcpy(&new_specs[gProgram_state.special_volume_count], &spec, sizeof(tSpecial_volume));
    gProgram_state.special_volume_count++;
    BrMemFree(gProgram_state.special_volumes);
    gProgram_state.special_volumes = new_specs;
    BuildSpecVolModel(&spec, gProgram_state.special_volume_count - 1, GetInternalMat(), GetExternalMat());
    gLast_actor = gSpec_vol_actors[gProgram_state.special_volume_count - 1];
    UpdateSpecVol();
    sprintf(s, "Shat out special volume #%d (type %d)", gProgram_state.special_volume_count - 1, pIndex);
    NewTextHeadupSlot(4, 0, 2000, -2, s);
    SaveSpecialVolumes();
    fclose(f);
}

// IDA: void __cdecl DropSpecVol0()
void DropSpecVol0() {
    LOG_TRACE("()");
}

// IDA: void __cdecl DropSpecVol1()
void DropSpecVol1() {
    LOG_TRACE("()");

    DropSpecVol(1);
}

// IDA: void __cdecl DropSpecVol2()
void DropSpecVol2() {
    LOG_TRACE("()");

    DropSpecVol(2);
}

// IDA: void __cdecl DropSpecVol3()
void DropSpecVol3() {
    LOG_TRACE("()");

    DropSpecVol(3);
}

// IDA: void __cdecl DropSpecVol4()
void DropSpecVol4() {
    LOG_TRACE("()");

    DropSpecVol(4);
}

// IDA: void __cdecl DropSpecVol5()
void DropSpecVol5() {
    LOG_TRACE("()");

    DropSpecVol(5);
}

// IDA: void __cdecl DropSpecVol6()
void DropSpecVol6() {
    LOG_TRACE("()");

    DropSpecVol(6);
}

// IDA: void __cdecl DropSpecVol7()
void DropSpecVol7() {
    LOG_TRACE("()");

    DropSpecVol(7);
}

// IDA: void __cdecl DropSpecVol8()
void DropSpecVol8() {
    LOG_TRACE("()");

    DropSpecVol(8);
}

// IDA: void __cdecl DropSpecVol9()
void DropSpecVol9() {
    LOG_TRACE("()");

    DropSpecVol(9);
}

// IDA: void __cdecl IdentifySpecVol()
void IdentifySpecVol() {
    int i;
    int min_index;
    tSpecial_volume* v;
    br_scalar min_d;
    br_scalar d;
    br_vector3* p;
    char s[256];
    LOG_TRACE("()");

    min_d = FLT_MAX;
    min_index = -1;
    p = &gProgram_state.current_car.pos;
    for (i = 0; i < gProgram_state.special_volume_count; i++) {
        v = &gProgram_state.special_volumes[i];
        d = Vector3DistanceSquared((br_vector3*)v->mat.m[3], p);
        if (d < min_d) {
            min_index = i;
            min_d = d;
        }
    }
    if (min_index < 0) {
        gLast_actor = NULL;
        NewTextHeadupSlot(4, 0, 2000, -2, "No special volumes to lock onto");
    } else {
        sprintf(s, "Locked onto Special Volume #%d", min_index);
        NewTextHeadupSlot(4, 0, 2000, -2, s);
        gLast_actor = gSpec_vol_actors[min_index];
    }
}

// IDA: void __usercall DelSpecVolumeGraph(int pIndex@<EAX>)
void DelSpecVolumeGraph(int pIndex) {
    br_actor* actor;
    br_model* model;
    LOG_TRACE("(%d)", pIndex);

    actor = gSpec_vol_actors[pIndex];
    model = actor->model;

    BrMaterialRemove(model->faces[5].material);
    BrMaterialRemove(model->faces[11].material);
    BrMaterialRemove(model->faces[7].material);
    BrMaterialRemove(model->faces[17].material);
    BrMaterialRemove(model->faces[23].material);
    BrMaterialRemove(model->faces[19].material);
    BrMaterialFree(model->faces[5].material);
    BrMaterialFree(model->faces[11].material);
    BrMaterialFree(model->faces[7].material);
    BrMaterialFree(model->faces[17].material);
    BrMaterialFree(model->faces[23].material);
    BrMaterialFree(model->faces[19].material);
    BrModelRemove(model);
    BrModelFree(model);
    BrActorRemove(actor);
    BrActorFree(actor);
}

// IDA: void __cdecl DeleteSpecVol()
void DeleteSpecVol() {
    int index;
    LOG_TRACE("()");

    index = FindSpecVolIndex(gLast_actor);
    if (index < 0) {
        return;
    }
    DelSpecVolumeGraph(index);
    memmove(&gProgram_state.special_volumes[index], &gProgram_state.special_volumes[index + 1], (gProgram_state.special_volume_count - index - 1) * sizeof(tSpecial_volume));
    memmove(&gSpec_vol_actors[index], &gSpec_vol_actors[index + 1], (gProgram_state.special_volume_count - index - 1) * sizeof(br_actor*));
    gProgram_state.special_volume_count--;
    NewTextHeadupSlot(4, 0, 2000, -2, "There's been a special volumes MURDER!!");
    gLast_actor = NULL;
    if (&gProgram_state.special_volumes[index] < gDefault_water_spec_vol) {
        gDefault_water_spec_vol--;
    }
    SaveSpecialVolumes();
}

// IDA: void __cdecl RotateSpecVolL()
void RotateSpecVolL() {
    LOG_TRACE("()");

    RotateAccL();
}

// IDA: void __cdecl RotateSpecVolL2()
void RotateSpecVolL2() {
    LOG_TRACE("()");

    RotateAccL2();
}

// IDA: void __cdecl RotateSpecVolL3()
void RotateSpecVolL3() {
    LOG_TRACE("()");

    RotateAccL3();
}

// IDA: void __cdecl RotateSpecVolL4()
void RotateSpecVolL4() {
    LOG_TRACE("()");

    RotateAccL4();
}

// IDA: void __cdecl RotateSpecVolR()
void RotateSpecVolR() {
    LOG_TRACE("()");

    RotateAccR();
}

// IDA: void __cdecl RotateSpecVolR2()
void RotateSpecVolR2() {
    LOG_TRACE("()");

    RotateAccR2();
}

// IDA: void __cdecl RotateSpecVolR3()
void RotateSpecVolR3() {
    LOG_TRACE("()");

    RotateAccR3();
}

// IDA: void __cdecl RotateSpecVolR4()
void RotateSpecVolR4() {
    LOG_TRACE("()");

    RotateAccR4();
}

// IDA: void __cdecl CycleSpecVolRotate()
void CycleSpecVolRotate() {
    LOG_TRACE("()");

    CycleAccRotate();
}

// IDA: void __cdecl CycleSpecVolScale()
void CycleSpecVolScale() {
    LOG_TRACE("()");

    CycleAccScale();
}

// IDA: void __cdecl ScaleSpecVolUp2()
void ScaleSpecVolUp2() {
    LOG_TRACE("()");

    ScaleAccUp2();
}

// IDA: void __cdecl ScaleSpecVolUp3()
void ScaleSpecVolUp3() {
    LOG_TRACE("()");

    ScaleAccUp3();
}

// IDA: void __cdecl ScaleSpecVolUp4()
void ScaleSpecVolUp4() {
    LOG_TRACE("()");

    ScaleAccUp4();
}

// IDA: void __cdecl ScaleSpecVolDown2()
void ScaleSpecVolDown2() {
    LOG_TRACE("()");

    ScaleAccDown2();
}

// IDA: void __cdecl ScaleSpecVolDown3()
void ScaleSpecVolDown3() {
    LOG_TRACE("()");

    ScaleAccDown3();
}

// IDA: void __cdecl ScaleSpecVolDown4()
void ScaleSpecVolDown4() {
    LOG_TRACE("()");

    ScaleAccDown4();
}

// IDA: void __cdecl MoveXSpecVolL()
void MoveXSpecVolL() {
    LOG_TRACE("()");

    MoveXAccL();
}

// IDA: void __cdecl MoveXSpecVolL2()
void MoveXSpecVolL2() {
    LOG_TRACE("()");

    MoveXAccL2();
}

// IDA: void __cdecl MoveXSpecVolL3()
void MoveXSpecVolL3() {
    LOG_TRACE("()");

    MoveXAccL3();
}

// IDA: void __cdecl MoveXSpecVolL4()
void MoveXSpecVolL4() {
    LOG_TRACE("()");

    MoveXAccL4();
}

// IDA: void __cdecl MoveXSpecVolR()
void MoveXSpecVolR() {
    LOG_TRACE("()");

    MoveXAccR();
}

// IDA: void __cdecl MoveXSpecVolR2()
void MoveXSpecVolR2() {
    LOG_TRACE("()");

    MoveXAccR2();
}

// IDA: void __cdecl MoveXSpecVolR3()
void MoveXSpecVolR3() {
    LOG_TRACE("()");

    MoveXAccR3();
}

// IDA: void __cdecl MoveXSpecVolR4()
void MoveXSpecVolR4() {
    LOG_TRACE("()");

    MoveXAccR4();
}

// IDA: void __cdecl MoveYSpecVolL()
void MoveYSpecVolL() {
    LOG_TRACE("()");

    MoveYAccL();
}

// IDA: void __cdecl MoveYSpecVolL2()
void MoveYSpecVolL2() {
    LOG_TRACE("()");

#if defined(DETHRACE_FIX_BUGS)
    MoveYAccL2();
#else
    MoveYAccL3();
#endif
}

// IDA: void __cdecl MoveYSpecVolL3()
void MoveYSpecVolL3() {
    LOG_TRACE("()");

    MoveYAccL3();
}

// IDA: void __cdecl MoveYSpecVolL4()
void MoveYSpecVolL4() {
    LOG_TRACE("()");

    MoveYAccL4();
}

// IDA: void __cdecl MoveYSpecVolR()
void MoveYSpecVolR() {
    LOG_TRACE("()");

    MoveYAccR();
}

// IDA: void __cdecl MoveYSpecVolR2()
void MoveYSpecVolR2() {
    LOG_TRACE("()");

    MoveYAccR2();
}

// IDA: void __cdecl MoveYSpecVolR3()
void MoveYSpecVolR3() {
    LOG_TRACE("()");

    MoveYAccR3();
}

// IDA: void __cdecl MoveYSpecVolR4()
void MoveYSpecVolR4() {
    LOG_TRACE("()");

    MoveYAccR4();
}

// IDA: void __cdecl MoveZSpecVolL()
void MoveZSpecVolL() {
    LOG_TRACE("()");

    MoveZAccL();
}

// IDA: void __cdecl MoveZSpecVolL2()
void MoveZSpecVolL2() {
    LOG_TRACE("()");

    MoveZAccL2();
}

// IDA: void __cdecl MoveZSpecVolL3()
void MoveZSpecVolL3() {
    LOG_TRACE("()");

    MoveZAccL3();
}

// IDA: void __cdecl MoveZSpecVolL4()
void MoveZSpecVolL4() {
    LOG_TRACE("()");

    MoveZAccL4();
}

// IDA: void __cdecl MoveZSpecVolR()
void MoveZSpecVolR() {
    LOG_TRACE("()");

    MoveZAccR();
}

// IDA: void __cdecl MoveZSpecVolR2()
void MoveZSpecVolR2() {
    LOG_TRACE("()");

    MoveZAccR2();
}

// IDA: void __cdecl MoveZSpecVolR3()
void MoveZSpecVolR3() {
    LOG_TRACE("()");

    MoveZAccR3();
}

// IDA: void __cdecl MoveZSpecVolR4()
void MoveZSpecVolR4() {
    LOG_TRACE("()");

    MoveZAccR4();
}

// IDA: void __cdecl SnapSpecVolToVertical()
void SnapSpecVolToVertical() {
    LOG_TRACE("()");

    SnapAccToVertical();
}

// IDA: void __cdecl ShowSpecialVolumes()
void ShowSpecialVolumes() {
    int i;
    int j;
    int temp;
    tSpecial_volume* v;
    br_actor* actor;
    br_model* model;
    br_material* internal_mat;
    br_material* external_mat;
    LOG_TRACE("()");

    gLast_actor = NULL;
    gSpec_vol_mode = 1;
    internal_mat = GetInternalMat();
    external_mat = GetExternalMat();
    for (i = 0; i < gProgram_state.special_volume_count; i++) {
        v = &gProgram_state.special_volumes[i];
        if (!v->no_mat) {
            BuildSpecVolModel(v, i, internal_mat, external_mat);
        } else {
            gSpec_vol_actors[i] = NULL;
        }
    }
}

// IDA: void __cdecl HideSpecialVolumes()
void HideSpecialVolumes() {
    int i;
    tSpecial_volume* v;
    LOG_TRACE("()");

    gSpec_vol_mode = 0;
    for (i = 0; i < gProgram_state.special_volume_count; i++) {
        v = &gProgram_state.special_volumes[i];
        if (!v->no_mat) {
            DelSpecVolumeGraph(i);
        }
    }
}
