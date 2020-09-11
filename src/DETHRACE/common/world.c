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

// IDA: float __cdecl MapSawToTriangle(float pNumber)
float MapSawToTriangle(float pNumber) {
    LOG_TRACE("(%f)", pNumber);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetSightDistance(br_scalar pYon)
void SetSightDistance(br_scalar pYon) {
    gSight_distance_squared = pYon * 1.02f * (pYon * 1.02f);
}

// IDA: br_actor* __usercall FindActorInArray@<EAX>(char *pThe_name@<EAX>)
br_actor* FindActorInArray(char* pThe_name) {
    int i;
    LOG_TRACE("(\"%s\")", pThe_name);
    NOT_IMPLEMENTED();
}

// IDA: br_actor* __usercall FindLightInArray@<EAX>(char *pThe_name@<EAX>)
br_actor* FindLightInArray(char* pThe_name) {
    int i;
    LOG_TRACE("(\"%s\")", pThe_name);
    NOT_IMPLEMENTED();
}

// IDA: br_actor* __usercall CloneActor@<EAX>(br_actor *pSource_actor@<EAX>)
br_actor* CloneActor(br_actor* pSource_actor) {
    br_actor* new_actor;
    br_actor* child_actor;
    br_actor* new_child_actor;
    LOG_TRACE("(%p)", pSource_actor);
    NOT_IMPLEMENTED();
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
    pStorage_space->pixelmaps = BrMemCalloc(pMax_pixelmaps, 4, 0xD7u);
    pStorage_space->shade_tables = BrMemCalloc(pMax_shade_tables, 4, 0xD8u);
    pStorage_space->materials = BrMemCalloc(pMax_materials, 4, 0xD9u);
    pStorage_space->models = BrMemCalloc(pMax_models, 4, 0xDAu);
    pStorage_space->saved_colour_maps = BrMemCalloc(pMax_materials, 4, 0xDBu);
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
    NOT_IMPLEMENTED();
}

// IDA: tAdd_to_storage_result __usercall AddPixelmapToStorage@<EAX>(tBrender_storage *pStorage_space@<EAX>, br_pixelmap **pThe_pm@<EDX>)
tAdd_to_storage_result AddPixelmapToStorage(tBrender_storage* pStorage_space, br_pixelmap** pThe_pm) {
    int i;
    LOG_TRACE("(%p, %p)", pStorage_space, pThe_pm);
    NOT_IMPLEMENTED();
}

// IDA: tAdd_to_storage_result __usercall AddShadeTableToStorage@<EAX>(tBrender_storage *pStorage_space@<EAX>, br_pixelmap *pThe_st@<EDX>)
tAdd_to_storage_result AddShadeTableToStorage(tBrender_storage* pStorage_space, br_pixelmap* pThe_st) {
    int i;
    LOG_TRACE("(%p, %p)", pStorage_space, pThe_st);
    NOT_IMPLEMENTED();
}

// IDA: tAdd_to_storage_result __usercall AddMaterialToStorage@<EAX>(tBrender_storage *pStorage_space@<EAX>, br_material *pThe_mat@<EDX>)
tAdd_to_storage_result AddMaterialToStorage(tBrender_storage* pStorage_space, br_material* pThe_mat) {
    int i;
    LOG_TRACE("(%p, %p)", pStorage_space, pThe_mat);
    NOT_IMPLEMENTED();
}

// IDA: tAdd_to_storage_result __usercall AddModelToStorage@<EAX>(tBrender_storage *pStorage_space@<EAX>, br_model *pThe_mod@<EDX>)
tAdd_to_storage_result AddModelToStorage(tBrender_storage* pStorage_space, br_model* pThe_mod) {
    int i;
    LOG_TRACE("(%p, %p)", pStorage_space, pThe_mod);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall LoadSinglePixelmap@<EAX>(tBrender_storage *pStorage_space@<EAX>, char *pName@<EDX>)
br_pixelmap* LoadSinglePixelmap(tBrender_storage* pStorage_space, char* pName) {
    br_pixelmap* temp;
    LOG_TRACE("(%p, \"%s\")", pStorage_space, pName);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall LoadSingleMaterial@<EAX>(tBrender_storage *pStorage_space@<EAX>, char *pName@<EDX>)
br_material* LoadSingleMaterial(tBrender_storage* pStorage_space, char* pName) {
    br_material* temp;
    LOG_TRACE("(%p, \"%s\")", pStorage_space, pName);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __usercall LoadSingleShadeTable@<EAX>(tBrender_storage *pStorage_space@<EAX>, char *pName@<EDX>)
br_pixelmap* LoadSingleShadeTable(tBrender_storage* pStorage_space, char* pName) {
    br_pixelmap* temp;
    LOG_TRACE("(%p, \"%s\")", pStorage_space, pName);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DodgyModelUpdate(br_model *pM@<EAX>)
void DodgyModelUpdate(br_model* pM) {
    LOG_TRACE("(%p)", pM);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall SuffixedMaterial@<EAX>(br_material *pOld@<EAX>, char *pSuffix@<EDX>)
br_material* SuffixedMaterial(br_material* pOld, char* pSuffix) {
    br_material* new_mat;
    char* new_id;
    LOG_TRACE("(%p, \"%s\")", pOld, pSuffix);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FaceIsRoad@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
int FaceIsRoad(br_model* pModel, tU16 pFace) {
    br_vector3 v0;
    br_vector3 v1;
    br_vector3 cross;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall RoadPerspToUntex@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* RoadPerspToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall WallPerspToLinear@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallPerspToLinear(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall WallPerspToUntex@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallPerspToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddFunkGrooveBinding(int pSlot_number@<EAX>, float *pPeriod_address@<EDX>)
void AddFunkGrooveBinding(int pSlot_number, float* pPeriod_address) {
    LOG_TRACE("(%d, %p)", pSlot_number, pPeriod_address);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ControlBoundFunkGroove(int pSlot_number@<EAX>, float pValue)
void ControlBoundFunkGroove(int pSlot_number, float pValue) {
    LOG_TRACE("(%d, %f)", pSlot_number, pValue);
    NOT_IMPLEMENTED();
}

// IDA: float __usercall ControlBoundFunkGroovePlus@<ST0>(int pSlot_number@<EAX>, float pValue)
float ControlBoundFunkGroovePlus(int pSlot_number, float pValue) {
    LOG_TRACE("(%d, %f)", pSlot_number, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ShiftBoundGrooveFunks(char *pStart@<EAX>, char *pEnd@<EDX>, int pDelta@<EBX>)
void ShiftBoundGrooveFunks(char* pStart, char* pEnd, int pDelta) {
    int i;
    LOG_TRACE("(\"%s\", \"%s\", %d)", pStart, pEnd, pDelta);
    NOT_IMPLEMENTED();
}

// IDA: tFunkotronic_spec* __cdecl AddNewFunkotronic()
tFunkotronic_spec* AddNewFunkotronic() {
    void* new_array;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeFunkotronics(int pOwner@<EAX>)
void DisposeFunkotronics(int pOwner) {
    int i;
    tFunkotronic_spec* the_funk;
    LOG_TRACE("(%d)", pOwner);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddProximityVertex(br_vector3 *pV@<EAX>, tFunkotronic_spec *pThe_funk@<EDX>)
void AddProximityVertex(br_vector3* pV, tFunkotronic_spec* pThe_funk) {
    LOG_TRACE("(%p, %p)", pV, pThe_funk);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AddProximityVertexXYZ(br_scalar pX, br_scalar pY, br_scalar pZ, tFunkotronic_spec *pThe_funk)
void AddProximityVertexXYZ(br_scalar pX, br_scalar pY, br_scalar pZ, tFunkotronic_spec* pThe_funk) {
    br_vector3 v;
    LOG_TRACE("(%f, %f, %f, %p)", pX, pY, pZ, pThe_funk);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall CalcProximities@<EAX>(br_actor *pActor@<EAX>, br_material *pMat@<EDX>, tFunkotronic_spec *pThe_funk@<EBX>)
br_uint_32 CalcProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk) {
    br_face* the_face;
    int i;
    LOG_TRACE("(%p, %p, %p)", pActor, pMat, pThe_funk);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall AddProximities@<EAX>(br_actor *pActor@<EAX>, br_material *pMat@<EDX>, tFunkotronic_spec *pThe_funk@<EBX>)
br_uint_32 AddProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk) {
    br_face* the_face;
    int i;
    LOG_TRACE("(%p, %p, %p)", pActor, pMat, pThe_funk);
    NOT_IMPLEMENTED();
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
    LOG_TRACE("(%p, %d, %d)", pF, pOwner, pRef_offset);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeGroovidelics(int pOwner@<EAX>)
void DisposeGroovidelics(int pOwner) {
    int i;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%d)", pOwner);
    NOT_IMPLEMENTED();
}

// IDA: tGroovidelic_spec* __cdecl AddNewGroovidelic()
tGroovidelic_spec* AddNewGroovidelic() {
    void* new_array;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddGroovidelics(FILE *pF@<EAX>, int pOwner@<EDX>, br_actor *pParent_actor@<EBX>, int pRef_offset@<ECX>, int pAllowed_to_be_absent)
void AddGroovidelics(FILE* pF, int pOwner, br_actor* pParent_actor, int pRef_offset, int pAllowed_to_be_absent) {
    char s[256];
    char* str;
    int first_time;
    int i;
    int j;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%p, %d, %p, %d, %d)", pF, pOwner, pParent_actor, pRef_offset, pAllowed_to_be_absent);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall KillGroovadelic(int pOwner@<EAX>)
void KillGroovadelic(int pOwner) {
    int i;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%d)", pOwner);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall KillFunkotronic(int pOwner@<EAX>)
void KillFunkotronic(int pOwner) {
    int i;
    tFunkotronic_spec* the_funk;
    LOG_TRACE("(%d)", pOwner);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DeleteBastards@<EAX>(br_actor *pActor@<EAX>, br_matrix34 *pMatrix@<EDX>, void *pArg@<EBX>)
br_uint_32 DeleteBastards(br_actor* pActor, br_matrix34* pMatrix, void* pArg) {
    int i;
    int parent_already_doomed;
    LOG_TRACE("(%p, %p, %p)", pActor, pMatrix, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DeleteAnyZeroBastards()
void DeleteAnyZeroBastards() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall ApplyTransToModels@<EAX>(br_actor *pActor@<EAX>, br_matrix34 *pMatrix@<EDX>, void *pArg@<EBX>)
br_uint_32 ApplyTransToModels(br_actor* pActor, br_matrix34* pMatrix, void* pArg) {
    int i;
    br_vector3 temp_point;
    LOG_TRACE("(%p, %p, %p)", pActor, pMatrix, pArg);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FindSpecVolIndex@<EAX>(br_actor *pActor@<EAX>)
int FindSpecVolIndex(br_actor* pActor) {
    int i;
    tSpecial_volume* v;
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MungeMaterial(br_matrix34 *pMat@<EAX>, br_material *pMat_1@<EDX>, br_material *pMat_2@<EBX>, int pAxis_0@<ECX>, int pAxis_1)
void MungeMaterial(br_matrix34* pMat, br_material* pMat_1, br_material* pMat_2, int pAxis_0, int pAxis_1) {
    LOG_TRACE("(%p, %p, %p, %d, %d)", pMat, pMat_1, pMat_2, pAxis_0, pAxis_1);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetSpecVolMatSize(br_actor *pActor@<EAX>)
void SetSpecVolMatSize(br_actor* pActor) {
    br_model* model;
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FindInverseAndWorldBox(tSpecial_volume *pSpec@<EAX>)
void FindInverseAndWorldBox(tSpecial_volume* pSpec) {
    br_bounds bnds;
    LOG_TRACE("(%p)", pSpec);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl UpdateSpecVol()
void UpdateSpecVol() {
    int index;
    tSpecial_volume* v;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SaveSpecialVolumes()
void SaveSpecialVolumes() {
    tPath_name the_path;
    FILE* f;
    int i;
    tSpecial_volume* v;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SaveAdditionalStuff()
void SaveAdditionalStuff() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
br_uint_32 ProcessFaceMaterials(br_actor* pActor, tPMFMCB pCallback) {
    LOG_TRACE("(%p, %d)", pActor, pCallback);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: tCar_texturing_level __cdecl GetCarTexturingLevel()
tCar_texturing_level GetCarTexturingLevel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetCarTexturingLevel(tCar_texturing_level pLevel@<EAX>)
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

// IDA: int __usercall HasThisSuffix@<EAX>(char *pIdent@<EAX>, char *pSuffix@<EDX>)
int HasThisSuffix(char* pIdent, char* pSuffix) {
    size_t len_ident;
    size_t len_suffix;
    LOG_TRACE("(\"%s\", \"%s\")", pIdent, pSuffix);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall UnsuffixedMaterial@<EAX>(char *pOld_ident@<EAX>, char *pSuffix@<EDX>)
br_material* UnsuffixedMaterial(char* pOld_ident, char* pSuffix) {
    br_material* result;
    int unsuffixed_len;
    char* new_id;
    LOG_TRACE("(\"%s\", \"%s\")", pOld_ident, pSuffix);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall RoadUntexToPersp@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* RoadUntexToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall WallLinearToUntex@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallLinearToUntex(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall WallUntexToLinear@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallUntexToLinear(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall WallUntexToPersp@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallUntexToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall WallLinearToPersp@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* WallLinearToPersp(br_model* pModel, tU16 pFace) {
    br_material* old_mat;
    br_material* new_mat;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: tWall_texturing_level __cdecl GetWallTexturingLevel()
tWall_texturing_level GetWallTexturingLevel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetWallTexturingLevel(tWall_texturing_level pLevel@<EAX>)
void SetWallTexturingLevel(tWall_texturing_level pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gWall_texturing_level = pLevel;
}

// IDA: void __usercall ReallySetWallTexturingLevel(tWall_texturing_level pLevel@<EAX>)
void ReallySetWallTexturingLevel(tWall_texturing_level pLevel) {
    static tPMFMCB* tweaker[3][3];
    LOG_TRACE("(%d)", pLevel);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __usercall DisposeSuffixedMaterials@<EAX>(br_model *pModel@<EAX>, tU16 pFace@<EDX>)
br_material* DisposeSuffixedMaterials(br_model* pModel, tU16 pFace) {
    size_t max_suffix_len;
    br_material* mat;
    br_material* victim;
    static char* suffixes[3];
    int s;
    char* id;
    LOG_TRACE("(%p, %d)", pModel, pFace);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeTexturingMaterials()
void DisposeTexturingMaterials() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl SetAccessoryRenderingCB(br_actor *pActor, void *pFlag)
br_uint_32 SetAccessoryRenderingCB(br_actor* pActor, void* pFlag) {
    if (pActor->identifier && *pActor->identifier == 38) {
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
            style = 4;
        } else {
            style = 1;
        }
        DRActorEnumRecurse(gTrack_actor, (br_actor_enum_cbfn*)SetAccessoryRenderingCB, &style);
    }
    gRendering_accessories = pOn;
}

// IDA: int __cdecl GetAccessoryRendering()
int GetAccessoryRendering() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetCarSimplificationLevel(int pLevel@<EAX>)
void SetCarSimplificationLevel(int pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gCar_simplification_level = pLevel;
}

// IDA: int __cdecl GetCarSimplificationLevel()
int GetCarSimplificationLevel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl RemoveBounds(br_actor *pActor, void *pArg)
br_uint_32 RemoveBounds(br_actor* pActor, void* pArg) {
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RemoveBoundsStructures(br_actor *pActor@<EAX>)
void RemoveBoundsStructures(br_actor* pActor) {
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FreeTrack(tTrack_spec *pTrack_spec@<EAX>)
void FreeTrack(tTrack_spec* pTrack_spec) {
    int i;
    tNon_car_spec* non_car;
    LOG_TRACE("(%p)", pTrack_spec);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ProcessTrack(br_actor *pWorld@<EAX>, tTrack_spec *pTrack_spec@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world_transform@<ECX>, int pRender_blends)
void ProcessTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, int pRender_blends) {
    LOG_TRACE("(%p, %p, %p, %p, %d)", pWorld, pTrack_spec, pCamera, pCamera_to_world_transform, pRender_blends);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl NormaliseDegreeAngle(br_scalar pAngle)
br_scalar NormaliseDegreeAngle(br_scalar pAngle) {
    LOG_TRACE("(%f)", pAngle);
    NOT_IMPLEMENTED();
}

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
    br_pixelmap* old_colour_map;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CalcActorGlobalPos(br_vector3 *pResult@<EAX>, br_actor *pActor@<EDX>)
void CalcActorGlobalPos(br_vector3* pResult, br_actor* pActor) {
    LOG_TRACE("(%p, %p)", pResult, pActor);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PointOutOfSight@<EAX>(br_vector3 *pPoint@<EAX>, br_scalar pMax_distance)
int PointOutOfSight(br_vector3* pPoint, br_scalar pMax_distance) {
    br_vector3 distance_vector;
    LOG_TRACE("(%p, %f)", pPoint, pMax_distance);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PathGrooveBastard(tGroovidelic_spec *pGroove@<EAX>, tU32 pTime@<EDX>, br_matrix34 *pMat@<EBX>, int pInterrupt_it@<ECX>)
void PathGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it) {
    br_scalar pos;
    LOG_TRACE("(%p, %d, %p, %d)", pGroove, pTime, pMat, pInterrupt_it);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GrooveThisDelic(tGroovidelic_spec *pGroove@<EAX>, tU32 pTime@<EDX>, int pInterrupt_it@<EBX>)
void GrooveThisDelic(tGroovidelic_spec* pGroove, tU32 pTime, int pInterrupt_it) {
    br_actor* the_actor;
    br_vector3 actor_pos;
    br_matrix34* the_mat;
    tInterrupt_status old_path_interrupt;
    tInterrupt_status old_object_interrupt;
    LOG_TRACE("(%p, %d, %d)", pGroove, pTime, pInterrupt_it);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GrooveThoseDelics()
void GrooveThoseDelics() {
    int i;
    tGroovidelic_spec* the_groove;
    float f_the_time;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StopGroovidelic(br_actor *pActor@<EAX>)
void StopGroovidelic(br_actor* pActor) {
    int i;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetGrooveInterrupt(int pGroove_index@<EAX>, br_matrix34 *pMatrix@<EDX>, int pPath_interrupt@<EBX>, int pObject_interrupt@<ECX>, float pPath_resumption, float pObject_resumption)
void SetGrooveInterrupt(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption) {
    tGroovidelic_spec* the_groove;
    LOG_TRACE("(%d, %p, %d, %d, %f, %f)", pGroove_index, pMatrix, pPath_interrupt, pObject_interrupt, pPath_resumption, pObject_resumption);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetGrooveFlags()
void ResetGrooveFlags() {
    int i;
    tGroovidelic_spec* the_groove;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tSpecial_volume* __cdecl GetDefaultSpecialVolumeForWater()
tSpecial_volume* GetDefaultSpecialVolumeForWater() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tSpecial_volume* __usercall FindSpecialVolume@<EAX>(br_vector3 *pP@<EAX>, tSpecial_volume *pLast_vol@<EDX>)
tSpecial_volume* FindSpecialVolume(br_vector3* pP, tSpecial_volume* pLast_vol) {
    int i;
    tSpecial_volume* v;
    br_vector3 p;
    LOG_TRACE("(%p, %p)", pP, pLast_vol);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SaveAdditionalActors()
void SaveAdditionalActors() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall DistanceFromFace@<ST0>(br_vector3 *pPos@<EAX>, tFace_ref *pFace@<EDX>)
br_scalar DistanceFromFace(br_vector3* pPos, tFace_ref* pFace) {
    br_vector3 normal;
    LOG_TRACE("(%p, %p)", pPos, pFace);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl CalcHighestID(br_actor *pActor, int *pHighest)
br_uint_32 CalcHighestID(br_actor* pActor, int* pHighest) {
    char s[256];
    int number;
    LOG_TRACE("(%p, %p)", pActor, pHighest);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl SetID(br_actor *pActor, void *pArg)
br_uint_32 SetID(br_actor* pActor, void* pArg) {
    char s[256];
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UniquificateActorsName(br_actor *pUniverse_actor@<EAX>, br_actor *pActor@<EDX>)
void UniquificateActorsName(br_actor* pUniverse_actor, br_actor* pActor) {
    int highest;
    LOG_TRACE("(%p, %p)", pUniverse_actor, pActor);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AccessoryHeadup(br_actor *pActor@<EAX>, char *pPrefix@<EDX>)
void AccessoryHeadup(br_actor* pActor, char* pPrefix) {
    char s[256];
    int i;
    br_actor* original_actor;
    LOG_TRACE("(%p, \"%s\")", pActor, pPrefix);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl CalcHighestNonAmID(br_actor *pActor, int *pHighest)
br_uint_32 CalcHighestNonAmID(br_actor* pActor, int* pHighest) {
    char s[256];
    int number;
    LOG_TRACE("(%p, %p)", pActor, pHighest);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl SetIDAndDupModel(br_actor *pActor, void *pArg)
br_uint_32 SetIDAndDupModel(br_actor* pActor, void* pArg) {
    char s[256];
    char s2[256];
    br_model* new_model;
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DuplicateIfNotAmpersand(br_actor *pActor@<EAX>)
void DuplicateIfNotAmpersand(br_actor* pActor) {
    int highest;
    LOG_TRACE("(%p)", pActor);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor0()
void DropActor0() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor1()
void DropActor1() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor2()
void DropActor2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor3()
void DropActor3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor4()
void DropActor4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor5()
void DropActor5() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor6()
void DropActor6() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor7()
void DropActor7() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor8()
void DropActor8() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropActor9()
void DropActor9() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl IdentifyAccCB(br_actor *pActor, void *pArg)
br_uint_32 IdentifyAccCB(br_actor* pActor, void* pArg) {
    br_scalar distance;
    char s[256];
    br_vector3 v;
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl IdentifyAcc()
void IdentifyAcc() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl DelGrooveRef(br_actor *pActor, void *pArg)
br_uint_32 DelGrooveRef(br_actor* pActor, void* pArg) {
    tGroovidelic_spec* the_groove;
    int i;
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl DelReferencedModels(br_actor *pActor, void *pArg)
br_uint_32 DelReferencedModels(br_actor* pActor, void* pArg) {
    tGroovidelic_spec* the_groove;
    int i;
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DeleteAcc()
void DeleteAcc() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl OffsetModel(br_actor *pActor, void *pArg)
br_uint_32 OffsetModel(br_actor* pActor, void* pArg) {
    int i;
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall OffsetActor(br_actor *pActor@<EAX>, br_vector3 *pOffset@<EDX>)
void OffsetActor(br_actor* pActor, br_vector3* pOffset) {
    LOG_TRACE("(%p, %p)", pActor, pOffset);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CentreActor(br_actor *pActor@<EAX>, br_vector3 *pOffset@<EDX>)
void CentreActor(br_actor* pActor, br_vector3* pOffset) {
    LOG_TRACE("(%p, %p)", pActor, pOffset);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SnapAccToVertical()
void SnapAccToVertical() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RotateAccessory(br_angle pAngle@<EAX>)
void RotateAccessory(br_angle pAngle) {
    br_vector3 mr_offset;
    LOG_TRACE("(%d)", pAngle);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleAccessory(float pScaling_factor)
void ScaleAccessory(float pScaling_factor) {
    br_vector3 mr_offset;
    LOG_TRACE("(%f)", pScaling_factor);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveAccessory(br_scalar pX_shift, br_scalar pY_shift, br_scalar pZ_shift)
void MoveAccessory(br_scalar pX_shift, br_scalar pY_shift, br_scalar pZ_shift) {
    br_vector3 v;
    LOG_TRACE("(%f, %f, %f)", pX_shift, pY_shift, pZ_shift);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateAccL()
void RotateAccL() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateAccL2()
void RotateAccL2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateAccL3()
void RotateAccL3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateAccL4()
void RotateAccL4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateAccR()
void RotateAccR() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateAccR2()
void RotateAccR2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateAccR3()
void RotateAccR3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateAccR4()
void RotateAccR4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleAccRotate()
void CycleAccRotate() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleAccScale()
void CycleAccScale() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleAccUp2()
void ScaleAccUp2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleAccUp3()
void ScaleAccUp3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleAccUp4()
void ScaleAccUp4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleAccDown2()
void ScaleAccDown2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleAccDown3()
void ScaleAccDown3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleAccDown4()
void ScaleAccDown4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXAccL()
void MoveXAccL() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXAccL2()
void MoveXAccL2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXAccL3()
void MoveXAccL3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXAccL4()
void MoveXAccL4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXAccR()
void MoveXAccR() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXAccR2()
void MoveXAccR2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXAccR3()
void MoveXAccR3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXAccR4()
void MoveXAccR4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYAccL()
void MoveYAccL() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYAccL2()
void MoveYAccL2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYAccL3()
void MoveYAccL3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYAccL4()
void MoveYAccL4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYAccR()
void MoveYAccR() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYAccR2()
void MoveYAccR2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYAccR3()
void MoveYAccR3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYAccR4()
void MoveYAccR4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZAccL()
void MoveZAccL() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZAccL2()
void MoveZAccL2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZAccL3()
void MoveZAccL3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZAccL4()
void MoveZAccL4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZAccR()
void MoveZAccR() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZAccR2()
void MoveZAccR2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZAccR3()
void MoveZAccR3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZAccR4()
void MoveZAccR4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_material* __cdecl GetInternalMat()
br_material* GetInternalMat() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_material* __cdecl GetExternalMat()
br_material* GetExternalMat() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BuildSpecVolModel(tSpecial_volume *pSpec@<EAX>, int pIndex@<EDX>, br_material *pInt_mat@<EBX>, br_material *pExt_mat@<ECX>)
void BuildSpecVolModel(tSpecial_volume* pSpec, int pIndex, br_material* pInt_mat, br_material* pExt_mat) {
    int i;
    int j;
    int temp;
    tSpecial_volume* v;
    br_actor* actor;
    br_model* model;
    LOG_TRACE("(%p, %d, %p, %p)", pSpec, pIndex, pInt_mat, pExt_mat);
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol0()
void DropSpecVol0() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol1()
void DropSpecVol1() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol2()
void DropSpecVol2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol3()
void DropSpecVol3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol4()
void DropSpecVol4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol5()
void DropSpecVol5() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol6()
void DropSpecVol6() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol7()
void DropSpecVol7() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol8()
void DropSpecVol8() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DropSpecVol9()
void DropSpecVol9() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DelSpecVolumeGraph(int pIndex@<EAX>)
void DelSpecVolumeGraph(int pIndex) {
    br_actor* actor;
    br_model* model;
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DeleteSpecVol()
void DeleteSpecVol() {
    int index;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateSpecVolL()
void RotateSpecVolL() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateSpecVolL2()
void RotateSpecVolL2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateSpecVolL3()
void RotateSpecVolL3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateSpecVolL4()
void RotateSpecVolL4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateSpecVolR()
void RotateSpecVolR() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateSpecVolR2()
void RotateSpecVolR2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateSpecVolR3()
void RotateSpecVolR3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RotateSpecVolR4()
void RotateSpecVolR4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleSpecVolRotate()
void CycleSpecVolRotate() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleSpecVolScale()
void CycleSpecVolScale() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleSpecVolUp2()
void ScaleSpecVolUp2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleSpecVolUp3()
void ScaleSpecVolUp3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleSpecVolUp4()
void ScaleSpecVolUp4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleSpecVolDown2()
void ScaleSpecVolDown2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleSpecVolDown3()
void ScaleSpecVolDown3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleSpecVolDown4()
void ScaleSpecVolDown4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXSpecVolL()
void MoveXSpecVolL() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXSpecVolL2()
void MoveXSpecVolL2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXSpecVolL3()
void MoveXSpecVolL3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXSpecVolL4()
void MoveXSpecVolL4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXSpecVolR()
void MoveXSpecVolR() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXSpecVolR2()
void MoveXSpecVolR2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXSpecVolR3()
void MoveXSpecVolR3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveXSpecVolR4()
void MoveXSpecVolR4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYSpecVolL()
void MoveYSpecVolL() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYSpecVolL2()
void MoveYSpecVolL2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYSpecVolL3()
void MoveYSpecVolL3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYSpecVolL4()
void MoveYSpecVolL4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYSpecVolR()
void MoveYSpecVolR() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYSpecVolR2()
void MoveYSpecVolR2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYSpecVolR3()
void MoveYSpecVolR3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveYSpecVolR4()
void MoveYSpecVolR4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZSpecVolL()
void MoveZSpecVolL() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZSpecVolL2()
void MoveZSpecVolL2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZSpecVolL3()
void MoveZSpecVolL3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZSpecVolL4()
void MoveZSpecVolL4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZSpecVolR()
void MoveZSpecVolR() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZSpecVolR2()
void MoveZSpecVolR2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZSpecVolR3()
void MoveZSpecVolR3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveZSpecVolR4()
void MoveZSpecVolR4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SnapSpecVolToVertical()
void SnapSpecVolToVertical() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl HideSpecialVolumes()
void HideSpecialVolumes() {
    int i;
    tSpecial_volume* v;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
