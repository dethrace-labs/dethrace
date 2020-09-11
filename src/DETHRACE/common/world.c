#include "world.h"
#include <stdlib.h>

#include "brender.h"
#include "car.h"
#include "globvars.h"
#include "loading.h"
#include "utility.h"

#include <string.h>

br_actor* gActor_array[500];
float* gGroove_funk_bindings[960];
br_actor* gDelete_list[500];
br_actor* gLight_array[50];
br_model* gAdditional_models[1000];
br_actor* gSpec_vol_actors[100];
char* gDepth_effect_names[2] = { "dark", "fog" };
char* gFunk_nature_names[4];
char* gGroove_object_names[4];
char* gGroove_path_names[2];
char* gGroove_nature_names[2];
char* gFunk_anim_names[2];
char* gFunk_move_names[6];
tPath_name gAdditional_actor_path;
char* gTime_mode_names[2];
tPath_name gAdditional_model_path;
tU32 gPrevious_groove_times[2];
int gRace_file_version;
br_vector3 gActor_centre;
tWall_texturing_level gWall_texturing_level = eWTL_full;
int gNumber_of_additional_models;
int gNumber_of_actors;
float gTemp;
br_actor* gLast_actor;
br_actor* gKnown_actor;
br_actor* gAdditional_actors;
br_actor* gGroove_by_proxy_actor;
int gRendering_accessories;
tScale_mode gCurrent_scale_mode;
int gNumber_of_lights;
tRoad_texturing_level gRoad_texturing_level = eRTL_full;
int gDelete_count;
br_scalar gNearest_distance;
br_actor* gNearest_actor;
br_actor* gStandard_lamp;
tRotate_mode gCurrent_rotate_mode = eRotate_mode_y;
tCar_texturing_level gCar_texturing_level = eCTL_full;
br_scalar gSight_distance_squared;
char* gLollipop_names[3];
char* gAxis_names[3];
int gGroovidelics_array_size;
tGroovidelic_spec* gGroovidelics_array;
int gSpec_vol_mode;
char* gFunk_type_names[5];
tFunkotronic_spec* gFunkotronics_array;
int gFunkotronics_array_size;

float MapSawToTriangle(float pNumber) {
    NOT_IMPLEMENTED();
}

void SetSightDistance(br_scalar pYon) {
    gSight_distance_squared = pYon * 1.02f * (pYon * 1.02f);
}

br_actor* FindActorInArray(char* pThe_name) {
    int i;
    NOT_IMPLEMENTED();
}

br_actor* FindLightInArray(char* pThe_name) {
    int i;
    NOT_IMPLEMENTED();
}

br_actor* CloneActor(br_actor* pSource_actor) {
    br_actor* new_actor;
    br_actor* child_actor;
    br_actor* new_child_actor;
    NOT_IMPLEMENTED();
}

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
    pStorage_space->pixelmaps = BrMemCalloc(pMax_pixelmaps, 4, 0xD7u);
    pStorage_space->shade_tables = BrMemCalloc(pMax_shade_tables, 4, 0xD8u);
    pStorage_space->materials = BrMemCalloc(pMax_materials, 4, 0xD9u);
    pStorage_space->models = BrMemCalloc(pMax_models, 4, 0xDAu);
    pStorage_space->saved_colour_maps = BrMemCalloc(pMax_materials, 4, 0xDBu);
}

void DisposeStorageSpace(tBrender_storage* pStorage_space) {
    BrMemFree(pStorage_space->pixelmaps);
    BrMemFree(pStorage_space->shade_tables);
    BrMemFree(pStorage_space->materials);
    BrMemFree(pStorage_space->models);
    BrMemFree(pStorage_space->saved_colour_maps);
}

void ClearOutStorageSpace(tBrender_storage* pStorage_space) {
    int i;
    NOT_IMPLEMENTED();
}

tAdd_to_storage_result AddPixelmapToStorage(tBrender_storage* pStorage_space, br_pixelmap** pThe_pm) {
    int i;
    NOT_IMPLEMENTED();
}

tAdd_to_storage_result AddShadeTableToStorage(tBrender_storage* pStorage_space, br_pixelmap* pThe_st) {
    int i;
    NOT_IMPLEMENTED();
}

tAdd_to_storage_result AddMaterialToStorage(tBrender_storage* pStorage_space, br_material* pThe_mat) {
    int i;
    NOT_IMPLEMENTED();
}

tAdd_to_storage_result AddModelToStorage(tBrender_storage* pStorage_space, br_model* pThe_mod) {
    int i;
    NOT_IMPLEMENTED();
}

int LoadNPixelmaps(tBrender_storage* pStorage_space, FILE* pF, int pCount) {
    tPath_name the_path;
    int i;
    int j;
    int new_ones;
    int total;
    char s[256];
    char* str;
    br_pixelmap* temp_array[200];
    NOT_IMPLEMENTED();
}

br_pixelmap* LoadSinglePixelmap(tBrender_storage* pStorage_space, char* pName) {
    br_pixelmap* temp;
    NOT_IMPLEMENTED();
}

br_material* LoadSingleMaterial(tBrender_storage* pStorage_space, char* pName) {
    br_material* temp;
    NOT_IMPLEMENTED();
}

int LoadNShadeTables(tBrender_storage* pStorage_space, FILE* pF, int pCount) {
    tPath_name the_path;
    int i;
    int j;
    int new_ones;
    int total;
    char s[256];
    char* str;
    br_pixelmap* temp_array[50];
    NOT_IMPLEMENTED();
}

br_pixelmap* LoadSingleShadeTable(tBrender_storage* pStorage_space, char* pName) {
    br_pixelmap* temp;
    NOT_IMPLEMENTED();
}

int LoadNMaterials(tBrender_storage* pStorage_space, FILE* pF, int pCount) {
    tPath_name the_path;
    int i;
    int j;
    int new_ones;
    int total;
    char s[256];
    char* str;
    br_material* temp_array[200];
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void DodgyModelUpdate(br_model* pM) {
    NOT_IMPLEMENTED();
}

br_material* SuffixedMaterial(br_material* pOld, char* pSuffix) {
    br_material* new_mat;
    char* new_id;
    NOT_IMPLEMENTED();
}

int FaceIsRoad(br_model* pModel, tU16 pFace) {
    br_vector3 v0;
    br_vector3 v1;
    br_vector3 cross;
    NOT_IMPLEMENTED();
}

br_material* RoadPerspToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

br_material* WallPerspToLinear(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

br_material* WallPerspToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

void ProcessModelFaceMaterials2(br_model* pModel, tPMFM2CB pCallback) {
    tU16 f;
    tU16 group;
    br_material* old_mat;
    NOT_IMPLEMENTED();
}

void ProcessModelFaceMaterials(br_model* pModel, tPMFMCB pCallback) {
    tU16 f;
    br_material* possible_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void LoadSomePixelmaps(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_pixelmap* temp_array[200];
    NOT_IMPLEMENTED();
}

void LoadSomeShadeTables(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_pixelmap* temp_array[50];
    NOT_IMPLEMENTED();
}

void LoadSomeMaterials(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_material* temp_array[200];
    NOT_IMPLEMENTED();
}

void LoadSomeModels(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_model* temp_array[2000];
    NOT_IMPLEMENTED();
}

void LoadSomeTrackModels(tBrender_storage* pStorage_space, FILE* pF) {
    tPath_name the_path;
    int i;
    int j;
    int count;
    int new_ones;
    char s[256];
    char* str;
    br_model* temp_array[2000];
    NOT_IMPLEMENTED();
}

void AddFunkGrooveBinding(int pSlot_number, float* pPeriod_address) {
    NOT_IMPLEMENTED();
}

void ControlBoundFunkGroove(int pSlot_number, float pValue) {
    NOT_IMPLEMENTED();
}

float ControlBoundFunkGroovePlus(int pSlot_number, float pValue) {
    NOT_IMPLEMENTED();
}

void ShiftBoundGrooveFunks(char* pStart, char* pEnd, int pDelta) {
    int i;
    NOT_IMPLEMENTED();
}

tFunkotronic_spec* AddNewFunkotronic() {
    int i;
    NOT_IMPLEMENTED();
}

void DisposeFunkotronics(int pOwner) {
    int i;
    tFunkotronic_spec* the_funk;
    NOT_IMPLEMENTED();
}

void AddProximityVertex(br_vector3* pV, tFunkotronic_spec* pThe_funk) {
    NOT_IMPLEMENTED();
}

void AddProximityVertexXYZ(br_scalar pX, br_scalar pY, br_scalar pZ, tFunkotronic_spec* pThe_funk) {
    br_vector3 v;
    NOT_IMPLEMENTED();
}

br_uint_32 CalcProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk) {
    br_face* the_face;
    int i;
    NOT_IMPLEMENTED();
}

br_uint_32 AddProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk) {
    br_face* the_face;
    int i;
    NOT_IMPLEMENTED();
}

void Adjust2FloatsForExceptions(float* pVictim1, float* pVictim2, br_pixelmap* pCulprit) {
    tException_list e;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void DisposeGroovidelics(int pOwner) {
    int i;
    tGroovidelic_spec* the_groove;
    NOT_IMPLEMENTED();
}

tGroovidelic_spec* AddNewGroovidelic() {
    int i;
    NOT_IMPLEMENTED();
}

void AddGroovidelics(FILE* pF, int pOwner, br_actor* pParent_actor, int pRef_offset, int pAllowed_to_be_absent) {
    char s[256];
    char* str;
    int first_time;
    int i;
    int j;
    tGroovidelic_spec* the_groove;
    NOT_IMPLEMENTED();
}

void KillGroovadelic(int pOwner) {
    int i;
    tGroovidelic_spec* the_groove;
    NOT_IMPLEMENTED();
}

void KillFunkotronic(int pOwner) {
    int i;
    tFunkotronic_spec* the_funk;
    NOT_IMPLEMENTED();
}

br_uint_32 DeleteBastards(br_actor* pActor, br_matrix34* pMatrix, void* pArg) {
    int i;
    int parent_already_doomed;
    NOT_IMPLEMENTED();
}

void DeleteAnyZeroBastards() {
    NOT_IMPLEMENTED();
}

br_uint_32 ApplyTransToModels(br_actor* pActor, br_matrix34* pMatrix, void* pArg) {
    int i;
    br_vector3 temp_point;
    NOT_IMPLEMENTED();
}

int FindSpecVolIndex(br_actor* pActor) {
    int i;
    tSpecial_volume* v;
    NOT_IMPLEMENTED();
}

void MungeMaterial(br_matrix34* pMat, br_material* pMat_1, br_material* pMat_2, int pAxis_0, int pAxis_1) {
    NOT_IMPLEMENTED();
}

void SetSpecVolMatSize(br_actor* pActor) {
    br_model* model;
    NOT_IMPLEMENTED();
}

void FindInverseAndWorldBox(tSpecial_volume* pSpec) {
    br_bounds bnds;
    NOT_IMPLEMENTED();
}

void UpdateSpecVol() {
    int index;
    tSpecial_volume* v;
    NOT_IMPLEMENTED();
}

void SaveSpecialVolumes() {
    FILE* f;
    int i;
    tSpecial_volume* v;
    NOT_IMPLEMENTED();
}

void SaveAdditionalStuff() {
    NOT_IMPLEMENTED();
}

br_uint_32 ProcessMaterials(br_actor* pActor, tPMFM2CB pCallback) {
    NOT_IMPLEMENTED();
}

br_uint_32 ProcessFaceMaterials2(br_actor* pActor, tPMFM2CB pCallback) {
    NOT_IMPLEMENTED();
}

void ChangePerspToSubdivCB(br_material* pMaterial) {
    NOT_IMPLEMENTED();
}

void ChangePerspToSubdiv() {
    NOT_IMPLEMENTED();
}

void ChangeSubdivToPerspCB(br_material* pMaterial) {
    NOT_IMPLEMENTED();
}

void ChangeSubdivToPersp() {
    NOT_IMPLEMENTED();
}

br_uint_32 ProcessFaceMaterials(br_actor* pActor, tPMFMCB pCallback) {
    NOT_IMPLEMENTED();
}


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

int StorageContainsPixelmap(tBrender_storage* pStorage, br_pixelmap* pMap) {
    int i;

    for (i = 0; i < pStorage->pixelmaps_count; i++) {
        if (pMap == pStorage->pixelmaps[i]) {
            return 1;
        }
    }
    return 0;
}

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

void SetCarStorageTexturingLevel(tBrender_storage* pStorage, tCar_texturing_level pNew, tCar_texturing_level pOld) {
    NOT_IMPLEMENTED();
}

tCar_texturing_level GetCarTexturingLevel() {
    NOT_IMPLEMENTED();
}

void SetCarTexturingLevel(tCar_texturing_level pLevel) {
    LOG_TRACE("(%d)", pLevel);

    if (pLevel != gCar_texturing_level) {
        if (gOur_car_storage_space.models_count) {
            if (pLevel == eCTL_none) {
                HideStoredTextures(&gOur_car_storage_space);
            } else if (pLevel == eCTL_transparent) {
                if (gCar_texturing_level) {
                    if (gCar_texturing_level == 2)
                        HideStoredOpaqueTextures(&gOur_car_storage_space);
                } else {
                    RevealStoredTransparentTextures(&gOur_car_storage_space);
                }
            } else if (pLevel == eCTL_full) {
                RevealStoredTextures(&gOur_car_storage_space);
            }
        }
        if (gTheir_cars_storage_space.models_count) {
            if (pLevel == eCTL_none) {
                HideStoredTextures(&gTheir_cars_storage_space);
            } else if (pLevel == eCTL_transparent) {
                if (gCar_texturing_level) {
                    if (gCar_texturing_level == 2)
                        HideStoredOpaqueTextures(&gTheir_cars_storage_space);
                } else {
                    RevealStoredTransparentTextures(&gTheir_cars_storage_space);
                }
            } else if (pLevel == eCTL_full) {
                RevealStoredTextures(&gTheir_cars_storage_space);
            }
        }
        if (gNet_cars_storage_space.models_count) {
            if (pLevel == eCTL_none) {
                HideStoredTextures(&gTheir_cars_storage_space);
            } else if (pLevel == eCTL_transparent) {
                if (gCar_texturing_level) {
                    if (gCar_texturing_level == 2)
                        HideStoredOpaqueTextures(&gTheir_cars_storage_space);
                } else {
                    RevealStoredTransparentTextures(&gTheir_cars_storage_space);
                }
            } else if (pLevel == eCTL_full) {
                RevealStoredTextures(&gTheir_cars_storage_space);
            }
        }
    }
    gCar_texturing_level = pLevel;
}

int HasThisSuffix(char* pIdent, char* pSuffix) {
    size_t len_ident;
    size_t len_suffix;
    NOT_IMPLEMENTED();
}

br_material* UnsuffixedMaterial(char* pOld_ident, char* pSuffix) {
    br_material* result;
    int unsuffixed_len;
    char* new_id;
    NOT_IMPLEMENTED();
}

br_material* RoadUntexToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

br_material* WallLinearToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

br_material* WallUntexToLinear(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

br_material* WallUntexToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

br_material* WallLinearToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    NOT_IMPLEMENTED();
}

tRoad_texturing_level GetRoadTexturingLevel() {
    return gRoad_texturing_level;
}

void SetRoadTexturingLevel(tRoad_texturing_level pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gRoad_texturing_level = pLevel;
}

void ReallySetRoadTexturingLevel(tRoad_texturing_level pLevel) {
    NOT_IMPLEMENTED();
}

tWall_texturing_level GetWallTexturingLevel() {
    NOT_IMPLEMENTED();
}

void SetWallTexturingLevel(tWall_texturing_level pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gWall_texturing_level = pLevel;
}

void ReallySetWallTexturingLevel(tWall_texturing_level pLevel) {
    static tPMFMCB* tweaker[3][3];
    NOT_IMPLEMENTED();
}

br_material* DisposeSuffixedMaterials(br_model* pModel, tU16 pFace) {
    size_t max_suffix_len;
    br_material* mat;
    br_material* victim;
    static char* suffixes[3] = { ".road", ".pwall", ".lwall" };
    int s;
    char* id;
    NOT_IMPLEMENTED();
}

void DisposeTexturingMaterials() {
    NOT_IMPLEMENTED();
}

br_uint_32 SetAccessoryRenderingCB(br_actor* pActor, void* pFlag) {
    if (pActor->identifier && *pActor->identifier == 38) {
        pActor->render_style = *(br_uint_8*)pFlag;
    }
    return 0;
}

void SetAccessoryRendering(int pOn) {
    int style;

    LOG_TRACE("(%d)", pOn);

    if (gTrack_actor) {
        if (pOn) {
            style = 4;
        } else {
            style = 1;
        }
        DRActorEnumRecurse(gTrack_actor, (br_actor_enum_cbfn*)SetAccessoryRenderingCB, &style);
    }
    gRendering_accessories = pOn;
}

int GetAccessoryRendering() {
    NOT_IMPLEMENTED();
}

void SetCarSimplificationLevel(int pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gCar_simplification_level = pLevel;
}

int GetCarSimplificationLevel() {
    NOT_IMPLEMENTED();
}

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

void AddExceptionToList(tException_list* pDst, tException_list pNew) {
    NOT_IMPLEMENTED();
}

void LoadExceptionsFile(char* pName) {
    FILE* f;
    char line[256];
    char* tok;
    int file_version;
    tException_list e;
    char delimiters[4];
    NOT_IMPLEMENTED();
}

void LoadExceptionsFileForTrack(char* pTrack_file_name) {
    tPath_name exceptions_file_name;
    NOT_IMPLEMENTED();
}

void FreeExceptions() {
    tException_list next;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

br_uint_32 RemoveBounds(br_actor* pActor, void* pArg) {
    NOT_IMPLEMENTED();
}

void RemoveBoundsStructures(br_actor* pActor) {
    NOT_IMPLEMENTED();
}

void FreeTrack(tTrack_spec* pTrack_spec) {
    int i;
    tNon_car_spec* non_car;
    NOT_IMPLEMENTED();
}

void ProcessTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, int pRender_blends) {
    NOT_IMPLEMENTED();
}

br_scalar NormaliseDegreeAngle(br_scalar pAngle) {
    NOT_IMPLEMENTED();
}

void FunkThoseTronics() {
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
    br_pixelmap* old_colour_map;
    NOT_IMPLEMENTED();
}

void LollipopizeActor(br_actor* pSubject_actor, br_matrix34* ref_to_world, tLollipop_mode pWhich_axis) {
    br_vector3 ref_to_subject;
    br_vector3 fixed_axis;
    br_vector3 vector_a;
    br_vector3 vector_b;
    br_matrix34 subject_to_world;
    br_matrix34 mat;
    NOT_IMPLEMENTED();
}

void CalcActorGlobalPos(br_vector3* pResult, br_actor* pActor) {
    NOT_IMPLEMENTED();
}

int PointOutOfSight(br_vector3* pPoint, br_scalar pMax_distance) {
    br_vector3 distance_vector;
    NOT_IMPLEMENTED();
}

void PathGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it) {
    br_scalar pos;
    NOT_IMPLEMENTED();
}

void ObjectGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it) {
    int rock_it;
    br_scalar x_size;
    br_scalar y_size;
    br_scalar z_size;
    br_scalar pos;
    br_bounds* bounds;
    NOT_IMPLEMENTED();
}

void GrooveThisDelic(tGroovidelic_spec* pGroove, tU32 pTime, int pInterrupt_it) {
    br_actor* the_actor;
    br_vector3 actor_pos;
    br_matrix34* the_mat;
    tInterrupt_status old_path_interrupt;
    tInterrupt_status old_object_interrupt;
    NOT_IMPLEMENTED();
}

void GrooveThoseDelics() {
    tGroovidelic_spec* the_groove;
    float f_the_time;
    NOT_IMPLEMENTED();
}

void StopGroovidelic(br_actor* pActor) {
    int i;
    tGroovidelic_spec* the_groove;
    NOT_IMPLEMENTED();
}

void SetGrooveInterrupt(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    tGroovidelic_spec* the_groove;
    NOT_IMPLEMENTED();
}

void ResetGrooveFlags() {
    int i;
    tGroovidelic_spec* the_groove;
    NOT_IMPLEMENTED();
}

tSpecial_volume* GetDefaultSpecialVolumeForWater() {
    NOT_IMPLEMENTED();
}

tSpecial_volume* FindSpecialVolume(br_vector3* pP, tSpecial_volume* pLast_vol) {
    int i;
    tSpecial_volume* v;
    br_vector3 p;
    NOT_IMPLEMENTED();
}

void SaveAdditionalActors() {
    NOT_IMPLEMENTED();
}

br_scalar DistanceFromFace(br_vector3* pPos, tFace_ref* pFace) {
    br_vector3 normal;
    NOT_IMPLEMENTED();
}

br_uint_32 CalcHighestID(br_actor* pActor, int* pHighest) {
    char s[256];
    int number;
    NOT_IMPLEMENTED();
}

br_uint_32 SetID(br_actor* pActor, void* pArg) {
    char s[256];
    NOT_IMPLEMENTED();
}

void UniquificateActorsName(br_actor* pUniverse_actor, br_actor* pActor) {
    int highest;
    NOT_IMPLEMENTED();
}

void AccessoryHeadup(br_actor* pActor, char* pPrefix) {
    char s[256];
    int i;
    br_actor* original_actor;
    NOT_IMPLEMENTED();
}

br_uint_32 CalcHighestNonAmID(br_actor* pActor, int* pHighest) {
    char s[256];
    int number;
    NOT_IMPLEMENTED();
}

br_uint_32 SetIDAndDupModel(br_actor* pActor, void* pArg) {
    char s[256];
    char s2[256];
    br_model* new_model;
    NOT_IMPLEMENTED();
}

void DuplicateIfNotAmpersand(br_actor* pActor) {
    int highest;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void DropActor0() {
    NOT_IMPLEMENTED();
}

void DropActor1() {
    NOT_IMPLEMENTED();
}

void DropActor2() {
    NOT_IMPLEMENTED();
}

void DropActor3() {
    NOT_IMPLEMENTED();
}

void DropActor4() {
    NOT_IMPLEMENTED();
}

void DropActor5() {
    NOT_IMPLEMENTED();
}

void DropActor6() {
    NOT_IMPLEMENTED();
}

void DropActor7() {
    NOT_IMPLEMENTED();
}

void DropActor8() {
    NOT_IMPLEMENTED();
}

void DropActor9() {
    NOT_IMPLEMENTED();
}

br_uint_32 IdentifyAccCB(br_actor* pActor, void* pArg) {
    br_scalar distance;
    char s[256];
    br_vector3 v;
    NOT_IMPLEMENTED();
}

void IdentifyAcc() {
    NOT_IMPLEMENTED();
}

br_uint_32 DelGrooveRef(br_actor* pActor, void* pArg) {
    tGroovidelic_spec* the_groove;
    int i;
    NOT_IMPLEMENTED();
}

br_uint_32 DelReferencedModels(br_actor* pActor, void* pArg) {
    tGroovidelic_spec* the_groove;
    int i;
    NOT_IMPLEMENTED();
}

void DeleteAcc() {
    NOT_IMPLEMENTED();
}

br_uint_32 OffsetModel(br_actor* pActor, void* pArg) {
    int i;
    NOT_IMPLEMENTED();
}

void OffsetActor(br_actor* pActor, br_vector3* pOffset) {
    NOT_IMPLEMENTED();
}

void CentreActor(br_actor* pActor, br_vector3* pOffset) {
    NOT_IMPLEMENTED();
}

void SnapAccToVertical() {
    NOT_IMPLEMENTED();
}

void RotateAccessory(br_angle pAngle) {
    br_vector3 mr_offset;
    NOT_IMPLEMENTED();
}

void ScaleAccessory(float pScaling_factor) {
    br_vector3 mr_offset;
    NOT_IMPLEMENTED();
}

void MoveAccessory(br_scalar pX_shift, br_scalar pY_shift, br_scalar pZ_shift) {
    br_vector3 v;
    NOT_IMPLEMENTED();
}

void RotateAccL() {
    NOT_IMPLEMENTED();
}

void RotateAccL2() {
    NOT_IMPLEMENTED();
}

void RotateAccL3() {
    NOT_IMPLEMENTED();
}

void RotateAccL4() {
    NOT_IMPLEMENTED();
}

void RotateAccR() {
    NOT_IMPLEMENTED();
}

void RotateAccR2() {
    NOT_IMPLEMENTED();
}

void RotateAccR3() {
    NOT_IMPLEMENTED();
}

void RotateAccR4() {
    NOT_IMPLEMENTED();
}

void CycleAccRotate() {
    NOT_IMPLEMENTED();
}

void CycleAccScale() {
    NOT_IMPLEMENTED();
}

void ScaleAccUp2() {
    NOT_IMPLEMENTED();
}

void ScaleAccUp3() {
    NOT_IMPLEMENTED();
}

void ScaleAccUp4() {
    NOT_IMPLEMENTED();
}

void ScaleAccDown2() {
    NOT_IMPLEMENTED();
}

void ScaleAccDown3() {
    NOT_IMPLEMENTED();
}

void ScaleAccDown4() {
    NOT_IMPLEMENTED();
}

void MoveXAccL() {
    NOT_IMPLEMENTED();
}

void MoveXAccL2() {
    NOT_IMPLEMENTED();
}

void MoveXAccL3() {
    NOT_IMPLEMENTED();
}

void MoveXAccL4() {
    NOT_IMPLEMENTED();
}

void MoveXAccR() {
    NOT_IMPLEMENTED();
}

void MoveXAccR2() {
    NOT_IMPLEMENTED();
}

void MoveXAccR3() {
    NOT_IMPLEMENTED();
}

void MoveXAccR4() {
    NOT_IMPLEMENTED();
}

void MoveYAccL() {
    NOT_IMPLEMENTED();
}

void MoveYAccL2() {
    NOT_IMPLEMENTED();
}

void MoveYAccL3() {
    NOT_IMPLEMENTED();
}

void MoveYAccL4() {
    NOT_IMPLEMENTED();
}

void MoveYAccR() {
    NOT_IMPLEMENTED();
}

void MoveYAccR2() {
    NOT_IMPLEMENTED();
}

void MoveYAccR3() {
    NOT_IMPLEMENTED();
}

void MoveYAccR4() {
    NOT_IMPLEMENTED();
}

void MoveZAccL() {
    NOT_IMPLEMENTED();
}

void MoveZAccL2() {
    NOT_IMPLEMENTED();
}

void MoveZAccL3() {
    NOT_IMPLEMENTED();
}

void MoveZAccL4() {
    NOT_IMPLEMENTED();
}

void MoveZAccR() {
    NOT_IMPLEMENTED();
}

void MoveZAccR2() {
    NOT_IMPLEMENTED();
}

void MoveZAccR3() {
    NOT_IMPLEMENTED();
}

void MoveZAccR4() {
    NOT_IMPLEMENTED();
}

br_material* GetInternalMat() {
    NOT_IMPLEMENTED();
}

br_material* GetExternalMat() {
    NOT_IMPLEMENTED();
}

void BuildSpecVolModel(tSpecial_volume* pSpec, int pIndex, br_material* pInt_mat, br_material* pExt_mat) {
    int i;
    int j;
    int temp;
    tSpecial_volume* v;
    br_actor* actor;
    br_model* model;
    NOT_IMPLEMENTED();
}

void DropSpecVol(int pIndex) {
    FILE* f;
    tPath_name the_path;
    int i;
    int spec_count;
    tSpecial_volume spec;
    tSpecial_volume* new_specs;
    char s[256];
    NOT_IMPLEMENTED();
}

void DropSpecVol0() {
    NOT_IMPLEMENTED();
}

void DropSpecVol1() {
    NOT_IMPLEMENTED();
}

void DropSpecVol2() {
    NOT_IMPLEMENTED();
}

void DropSpecVol3() {
    NOT_IMPLEMENTED();
}

void DropSpecVol4() {
    NOT_IMPLEMENTED();
}

void DropSpecVol5() {
    NOT_IMPLEMENTED();
}

void DropSpecVol6() {
    NOT_IMPLEMENTED();
}

void DropSpecVol7() {
    NOT_IMPLEMENTED();
}

void DropSpecVol8() {
    NOT_IMPLEMENTED();
}

void DropSpecVol9() {
    NOT_IMPLEMENTED();
}

void IdentifySpecVol() {
    int min_index;
    tSpecial_volume* v;
    br_scalar min_d;
    br_scalar d;
    br_vector3* p;
    char s[256];
    NOT_IMPLEMENTED();
}

void DelSpecVolumeGraph(int pIndex) {
    br_actor* actor;
    br_model* model;
    NOT_IMPLEMENTED();
}

void DeleteSpecVol() {
    NOT_IMPLEMENTED();
}

void RotateSpecVolL() {
    NOT_IMPLEMENTED();
}

void RotateSpecVolL2() {
    NOT_IMPLEMENTED();
}

void RotateSpecVolL3() {
    NOT_IMPLEMENTED();
}

void RotateSpecVolL4() {
    NOT_IMPLEMENTED();
}

void RotateSpecVolR() {
    NOT_IMPLEMENTED();
}

void RotateSpecVolR2() {
    NOT_IMPLEMENTED();
}

void RotateSpecVolR3() {
    NOT_IMPLEMENTED();
}

void RotateSpecVolR4() {
    NOT_IMPLEMENTED();
}

void CycleSpecVolRotate() {
    NOT_IMPLEMENTED();
}

void CycleSpecVolScale() {
    NOT_IMPLEMENTED();
}

void ScaleSpecVolUp2() {
    NOT_IMPLEMENTED();
}

void ScaleSpecVolUp3() {
    NOT_IMPLEMENTED();
}

void ScaleSpecVolUp4() {
    NOT_IMPLEMENTED();
}

void ScaleSpecVolDown2() {
    NOT_IMPLEMENTED();
}

void ScaleSpecVolDown3() {
    NOT_IMPLEMENTED();
}

void ScaleSpecVolDown4() {
    NOT_IMPLEMENTED();
}

void MoveXSpecVolL() {
    NOT_IMPLEMENTED();
}

void MoveXSpecVolL2() {
    NOT_IMPLEMENTED();
}

void MoveXSpecVolL3() {
    NOT_IMPLEMENTED();
}

void MoveXSpecVolL4() {
    NOT_IMPLEMENTED();
}

void MoveXSpecVolR() {
    NOT_IMPLEMENTED();
}

void MoveXSpecVolR2() {
    NOT_IMPLEMENTED();
}

void MoveXSpecVolR3() {
    NOT_IMPLEMENTED();
}

void MoveXSpecVolR4() {
    NOT_IMPLEMENTED();
}

void MoveYSpecVolL() {
    NOT_IMPLEMENTED();
}

void MoveYSpecVolL2() {
    NOT_IMPLEMENTED();
}

void MoveYSpecVolL3() {
    NOT_IMPLEMENTED();
}

void MoveYSpecVolL4() {
    NOT_IMPLEMENTED();
}

void MoveYSpecVolR() {
    NOT_IMPLEMENTED();
}

void MoveYSpecVolR2() {
    NOT_IMPLEMENTED();
}

void MoveYSpecVolR3() {
    NOT_IMPLEMENTED();
}

void MoveYSpecVolR4() {
    NOT_IMPLEMENTED();
}

void MoveZSpecVolL() {
    NOT_IMPLEMENTED();
}

void MoveZSpecVolL2() {
    NOT_IMPLEMENTED();
}

void MoveZSpecVolL3() {
    NOT_IMPLEMENTED();
}

void MoveZSpecVolL4() {
    NOT_IMPLEMENTED();
}

void MoveZSpecVolR() {
    NOT_IMPLEMENTED();
}

void MoveZSpecVolR2() {
    NOT_IMPLEMENTED();
}

void MoveZSpecVolR3() {
    NOT_IMPLEMENTED();
}

void MoveZSpecVolR4() {
    NOT_IMPLEMENTED();
}

void SnapSpecVolToVertical() {
    NOT_IMPLEMENTED();
}

void ShowSpecialVolumes() {
    int j;
    int temp;
    tSpecial_volume* v;
    br_actor* actor;
    br_model* model;
    br_material* internal_mat;
    br_material* external_mat;
    NOT_IMPLEMENTED();
}

void HideSpecialVolumes() {
    tSpecial_volume* v;
    NOT_IMPLEMENTED();
}
