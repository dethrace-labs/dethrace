#ifndef _WORLD_H_
#define _WORLD_H_

#include "dr_types.h"

extern int gFunkotronics_array_size;
extern int gGroovidelics_array_size;
extern int gSpec_vol_mode;
extern tFunkotronic_spec* gFunkotronics_array;
extern tGroovidelic_spec* gGroovidelics_array;
extern char* gFunk_type_names[5];
extern char* gFunk_anim_names[2];
extern char* gTime_mode_names[2];
extern char* gFunk_move_names[6];
extern char* gFunk_nature_names[4];
extern char* gGroove_nature_names[2];
extern char* gAxis_names[3];
extern char* gLollipop_names[3];
extern char* gGroove_path_names[2];
extern char* gGroove_object_names[4];
extern char* gDepth_effect_names[2];
extern br_actor* gGroove_by_proxy_actor;
extern tRotate_mode gCurrent_rotate_mode;
extern tScale_mode gCurrent_scale_mode;
extern int gNumber_of_additional_models;
extern tRoad_texturing_level gRoad_texturing_level;
extern tWall_texturing_level gWall_texturing_level;
extern tCar_texturing_level gCar_texturing_level;
extern int gRendering_accessories;
extern int gNumber_of_actors;
extern int gNumber_of_lights;
extern br_actor* gActor_array[500];
extern float* gGroove_funk_bindings[960];
extern br_actor* gDelete_list[500];
extern br_actor* gLight_array[50];
extern br_model* gAdditional_models[1000];
extern br_actor* gSpec_vol_actors[100];
extern tPath_name gAdditional_actor_path;
extern tPath_name gAdditional_model_path;
extern tU32 gPrevious_groove_times[2];
extern int gRace_file_version;
extern br_vector3 gActor_centre;
extern float gTemp;
extern br_actor* gLast_actor;
extern br_actor* gKnown_actor;
extern br_actor* gAdditional_actors;
extern int gDelete_count;
extern br_scalar gNearest_distance;
extern br_actor* gNearest_actor;
extern br_actor* gStandard_lamp;
extern br_scalar gSight_distance_squared;

float MapSawToTriangle(float pNumber);

void SetSightDistance(br_scalar pYon);

br_actor* FindActorInArray(char* pThe_name);

br_actor* FindLightInArray(char* pThe_name);

br_actor* CloneActor(br_actor* pSource_actor);

void InitialiseStorageSpace(tBrender_storage* pStorage_space, int pMax_pixelmaps, int pMax_shade_tables, int pMax_materials, int pMax_models);

void DisposeStorageSpace(tBrender_storage* pStorage_space);

void ClearOutStorageSpace(tBrender_storage* pStorage_space);

tAdd_to_storage_result AddPixelmapToStorage(tBrender_storage* pStorage_space, br_pixelmap** pThe_pm);

tAdd_to_storage_result AddShadeTableToStorage(tBrender_storage* pStorage_space, br_pixelmap* pThe_st);

tAdd_to_storage_result AddMaterialToStorage(tBrender_storage* pStorage_space, br_material* pThe_mat);

tAdd_to_storage_result AddModelToStorage(tBrender_storage* pStorage_space, br_model* pThe_mod);

int LoadNPixelmaps(tBrender_storage* pStorage_space, FILE* pF, int pCount);

br_pixelmap* LoadSinglePixelmap(tBrender_storage* pStorage_space, char* pName);

br_material* LoadSingleMaterial(tBrender_storage* pStorage_space, char* pName);

int LoadNShadeTables(tBrender_storage* pStorage_space, FILE* pF, int pCount);

br_pixelmap* LoadSingleShadeTable(tBrender_storage* pStorage_space, char* pName);

int LoadNMaterials(tBrender_storage* pStorage_space, FILE* pF, int pCount);

int LoadNModels(tBrender_storage* pStorage_space, FILE* pF, int pCount);

void DodgyModelUpdate(br_model* pM);

br_material* SuffixedMaterial(br_material* pOld, char* pSuffix);

int FaceIsRoad(br_model* pModel, tU16 pFace);

br_material* RoadPerspToUntex(br_model* pModel, tU16 pFace);

br_material* WallPerspToLinear(br_model* pModel, tU16 pFace);

br_material* WallPerspToUntex(br_model* pModel, tU16 pFace);

void ProcessModelFaceMaterials2(br_model* pModel, tPMFM2CB pCallback);

void ProcessModelFaceMaterials(br_model* pModel, tPMFMCB pCallback);

int LoadNTrackModels(tBrender_storage* pStorage_space, FILE* pF, int pCount);

void LoadSomePixelmaps(tBrender_storage* pStorage_space, FILE* pF);

void LoadSomeShadeTables(tBrender_storage* pStorage_space, FILE* pF);

void LoadSomeMaterials(tBrender_storage* pStorage_space, FILE* pF);

void LoadSomeModels(tBrender_storage* pStorage_space, FILE* pF);

void LoadSomeTrackModels(tBrender_storage* pStorage_space, FILE* pF);

void AddFunkGrooveBinding(int pSlot_number, float* pPeriod_address);

void ControlBoundFunkGroove(int pSlot_number, float pValue);

float ControlBoundFunkGroovePlus(int pSlot_number, float pValue);

void ShiftBoundGrooveFunks(char* pStart, char* pEnd, ptrdiff_t pDelta);

tFunkotronic_spec* AddNewFunkotronic(void);

void DisposeFunkotronics(int pOwner);

void AddProximityVertex(br_vector3* pV, tFunkotronic_spec* pThe_funk);

void AddProximityVertexXYZ(br_scalar pX, br_scalar pY, br_scalar pZ, tFunkotronic_spec* pThe_funk);

br_uint_32 CalcProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk);

br_uint_32 AddProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk);

void Adjust2FloatsForExceptions(float* pVictim1, float* pVictim2, br_pixelmap* pCulprit);

void AddFunkotronics(FILE* pF, int pOwner, int pRef_offset);

void DisposeGroovidelics(int pOwner);

tGroovidelic_spec* AddNewGroovidelic(void);

void AddGroovidelics(FILE* pF, int pOwner, br_actor* pParent_actor, int pRef_offset, int pAllowed_to_be_absent);

void KillGroovadelic(int pOwner);

void KillFunkotronic(int pOwner);

br_uint_32 DeleteBastards(br_actor* pActor, br_matrix34* pMatrix, void* pArg);

void DeleteAnyZeroBastards(void);

br_uint_32 ApplyTransToModels(br_actor* pActor, br_matrix34* pMatrix, void* pArg);

int FindSpecVolIndex(br_actor* pActor);

void MungeMaterial(br_matrix34* pMat, br_material* pMat_1, br_material* pMat_2, int pAxis_0, int pAxis_1);

void SetSpecVolMatSize(br_actor* pActor);

void FindInverseAndWorldBox(tSpecial_volume* pSpec);

void UpdateSpecVol(void);

void SaveSpecialVolumes(void);

void SaveAdditionalStuff(void);

br_uint_32 ProcessMaterials(br_actor* pActor, tPMFM2CB pCallback);

br_uint_32 ProcessFaceMaterials2(br_actor* pActor, tPMFM2CB pCallback);

void ChangePerspToSubdivCB(br_material* pMaterial);

void ChangePerspToSubdiv(void);

void ChangeSubdivToPerspCB(br_material* pMaterial);

void ChangeSubdivToPersp(void);

br_uintptr_t ProcessFaceMaterials(br_actor* pActor, tPMFMCB pCallback);

int DRPixelmapHasZeros(br_pixelmap* pm);

int StorageContainsPixelmap(tBrender_storage* pStorage, br_pixelmap* pMap);

void HideStoredOpaqueTextures(tBrender_storage* pStorage);

void RevealStoredTransparentTextures(tBrender_storage* pStorage);

void HideStoredTextures(tBrender_storage* pStorage);

void RevealStoredTextures(tBrender_storage* pStorage);

void SetCarStorageTexturingLevel(tBrender_storage* pStorage, tCar_texturing_level pNew, tCar_texturing_level pOld);

tCar_texturing_level GetCarTexturingLevel(void);

void SetCarTexturingLevel(tCar_texturing_level pLevel);

int HasThisSuffix(char* pIdent, char* pSuffix);

br_material* UnsuffixedMaterial(char* pOld_ident, char* pSuffix);

br_material* RoadUntexToPersp(br_model* pModel, tU16 pFace);

br_material* WallLinearToUntex(br_model* pModel, tU16 pFace);

br_material* WallUntexToLinear(br_model* pModel, tU16 pFace);

br_material* WallUntexToPersp(br_model* pModel, tU16 pFace);

br_material* WallLinearToPersp(br_model* pModel, tU16 pFace);

tRoad_texturing_level GetRoadTexturingLevel(void);

void SetRoadTexturingLevel(tRoad_texturing_level pLevel);

void ReallySetRoadTexturingLevel(tRoad_texturing_level pLevel);

tWall_texturing_level GetWallTexturingLevel(void);

void SetWallTexturingLevel(tWall_texturing_level pLevel);

void ReallySetWallTexturingLevel(tWall_texturing_level pLevel);

br_material* DisposeSuffixedMaterials(br_model* pModel, tU16 pFace);

void DisposeTexturingMaterials(void);

/*br_uint_32*/ br_uintptr_t SetAccessoryRenderingCB(br_actor* pActor, void* pFlag);

void SetAccessoryRendering(int pOn);

int GetAccessoryRendering(void);

void SetCarSimplificationLevel(int pLevel);

int GetCarSimplificationLevel(void);

void ParseSpecialVolume(FILE* pF, tSpecial_volume* pSpec, char* pScreen_name_str);

void AddExceptionToList(tException_list* pDst, tException_list pNew);

void LoadExceptionsFile(char* pName);

void LoadExceptionsFileForTrack(char* pTrack_file_name);

void FreeExceptions(void);

void LoadTrack(char* pFile_name, tTrack_spec* pTrack_spec, tRace_info* pRace_info);

/*br_uint_32*/ br_uintptr_t RemoveBounds(br_actor* pActor, void* pArg);

void RemoveBoundsStructures(br_actor* pActor);

void FreeTrack(tTrack_spec* pTrack_spec);

void ProcessTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, int pRender_blends);

br_scalar NormaliseDegreeAngle(br_scalar pAngle);

void FunkThoseTronics(void);

void LollipopizeActor(br_actor* pSubject_actor, br_matrix34* ref_to_world, tLollipop_mode pWhich_axis);

void CalcActorGlobalPos(br_vector3* pResult, br_actor* pActor);

int PointOutOfSight(br_vector3* pPoint, br_scalar pMax_distance);

void PathGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it);

void ObjectGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it);

void GrooveThisDelic(tGroovidelic_spec* pGroove, tU32 pTime, int pInterrupt_it);

void GrooveThoseDelics(void);

void StopGroovidelic(br_actor* pActor);

void SetGrooveInterrupt(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption);

void ResetGrooveFlags(void);

tSpecial_volume* GetDefaultSpecialVolumeForWater(void);

tSpecial_volume* FindSpecialVolume(br_vector3* pP, tSpecial_volume* pLast_vol);

void SaveAdditionalActors(void);

br_scalar DistanceFromFace(br_vector3* pPos, tFace_ref* pFace);

br_uint_32 CalcHighestID(br_actor* pActor, int* pHighest);

br_uint_32 SetID(br_actor* pActor, void* pArg);

void UniquificateActorsName(br_actor* pUniverse_actor, br_actor* pActor);

void AccessoryHeadup(br_actor* pActor, char* pPrefix);

br_uint_32 CalcHighestNonAmID(br_actor* pActor, int* pHighest);

br_uint_32 SetIDAndDupModel(br_actor* pActor, void* pArg);

void DuplicateIfNotAmpersand(br_actor* pActor);

void DropActor(int pIndex);

void DropActor0(void);

void DropActor1(void);

void DropActor2(void);

void DropActor3(void);

void DropActor4(void);

void DropActor5(void);

void DropActor6(void);

void DropActor7(void);

void DropActor8(void);

void DropActor9(void);

br_uint_32 IdentifyAccCB(br_actor* pActor, void* pArg);

void IdentifyAcc(void);

br_uint_32 DelGrooveRef(br_actor* pActor, void* pArg);

br_uint_32 DelReferencedModels(br_actor* pActor, void* pArg);

void DeleteAcc(void);

br_uint_32 OffsetModel(br_actor* pActor, void* pArg);

void OffsetActor(br_actor* pActor, br_vector3* pOffset);

void CentreActor(br_actor* pActor, br_vector3* pOffset);

void SnapAccToVertical(void);

void RotateAccessory(br_angle pAngle);

void ScaleAccessory(float pScaling_factor);

void MoveAccessory(br_scalar pX_shift, br_scalar pY_shift, br_scalar pZ_shift);

void RotateAccL(void);

void RotateAccL2(void);

void RotateAccL3(void);

void RotateAccL4(void);

void RotateAccR(void);

void RotateAccR2(void);

void RotateAccR3(void);

void RotateAccR4(void);

void CycleAccRotate(void);

void CycleAccScale(void);

void ScaleAccUp2(void);

void ScaleAccUp3(void);

void ScaleAccUp4(void);

void ScaleAccDown2(void);

void ScaleAccDown3(void);

void ScaleAccDown4(void);

void MoveXAccL(void);

void MoveXAccL2(void);

void MoveXAccL3(void);

void MoveXAccL4(void);

void MoveXAccR(void);

void MoveXAccR2(void);

void MoveXAccR3(void);

void MoveXAccR4(void);

void MoveYAccL(void);

void MoveYAccL2(void);

void MoveYAccL3(void);

void MoveYAccL4(void);

void MoveYAccR(void);

void MoveYAccR2(void);

void MoveYAccR3(void);

void MoveYAccR4(void);

void MoveZAccL(void);

void MoveZAccL2(void);

void MoveZAccL3(void);

void MoveZAccL4(void);

void MoveZAccR(void);

void MoveZAccR2(void);

void MoveZAccR3(void);

void MoveZAccR4(void);

br_material* GetInternalMat(void);

br_material* GetExternalMat(void);

void BuildSpecVolModel(tSpecial_volume* pSpec, int pIndex, br_material* pInt_mat, br_material* pExt_mat);

void DropSpecVol(int pIndex);

void DropSpecVol0(void);

void DropSpecVol1(void);

void DropSpecVol2(void);

void DropSpecVol3(void);

void DropSpecVol4(void);

void DropSpecVol5(void);

void DropSpecVol6(void);

void DropSpecVol7(void);

void DropSpecVol8(void);

void DropSpecVol9(void);

void IdentifySpecVol(void);

void DelSpecVolumeGraph(int pIndex);

void DeleteSpecVol(void);

void RotateSpecVolL(void);

void RotateSpecVolL2(void);

void RotateSpecVolL3(void);

void RotateSpecVolL4(void);

void RotateSpecVolR(void);

void RotateSpecVolR2(void);

void RotateSpecVolR3(void);

void RotateSpecVolR4(void);

void CycleSpecVolRotate(void);

void CycleSpecVolScale(void);

void ScaleSpecVolUp2(void);

void ScaleSpecVolUp3(void);

void ScaleSpecVolUp4(void);

void ScaleSpecVolDown2(void);

void ScaleSpecVolDown3(void);

void ScaleSpecVolDown4(void);

void MoveXSpecVolL(void);

void MoveXSpecVolL2(void);

void MoveXSpecVolL3(void);

void MoveXSpecVolL4(void);

void MoveXSpecVolR(void);

void MoveXSpecVolR2(void);

void MoveXSpecVolR3(void);

void MoveXSpecVolR4(void);

void MoveYSpecVolL(void);

void MoveYSpecVolL2(void);

void MoveYSpecVolL3(void);

void MoveYSpecVolL4(void);

void MoveYSpecVolR(void);

void MoveYSpecVolR2(void);

void MoveYSpecVolR3(void);

void MoveYSpecVolR4(void);

void MoveZSpecVolL(void);

void MoveZSpecVolL2(void);

void MoveZSpecVolL3(void);

void MoveZSpecVolL4(void);

void MoveZSpecVolR(void);

void MoveZSpecVolR2(void);

void MoveZSpecVolR3(void);

void MoveZSpecVolR4(void);

void SnapSpecVolToVertical(void);

void ShowSpecialVolumes(void);

void HideSpecialVolumes(void);

#endif
