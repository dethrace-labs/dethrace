#ifndef _WORLD_H_
#define _WORLD_H_

#include "brender/br_types.h"
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

void ShiftBoundGrooveFunks(char* pStart, char* pEnd, int pDelta);

tFunkotronic_spec* AddNewFunkotronic();

void DisposeFunkotronics(int pOwner);

void AddProximityVertex(br_vector3* pV, tFunkotronic_spec* pThe_funk);

void AddProximityVertexXYZ(br_scalar pX, br_scalar pY, br_scalar pZ, tFunkotronic_spec* pThe_funk);

br_uint_32 CalcProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk);

br_uint_32 AddProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk);

void Adjust2FloatsForExceptions(float* pVictim1, float* pVictim2, br_pixelmap* pCulprit);

void AddFunkotronics(FILE* pF, int pOwner, int pRef_offset);

void DisposeGroovidelics(int pOwner);

tGroovidelic_spec* AddNewGroovidelic();

void AddGroovidelics(FILE* pF, int pOwner, br_actor* pParent_actor, int pRef_offset, int pAllowed_to_be_absent);

void KillGroovadelic(int pOwner);

void KillFunkotronic(int pOwner);

br_uint_32 DeleteBastards(br_actor* pActor, br_matrix34* pMatrix, void* pArg);

void DeleteAnyZeroBastards();

br_uint_32 ApplyTransToModels(br_actor* pActor, br_matrix34* pMatrix, void* pArg);

int FindSpecVolIndex(br_actor* pActor);

void MungeMaterial(br_matrix34* pMat, br_material* pMat_1, br_material* pMat_2, int pAxis_0, int pAxis_1);

void SetSpecVolMatSize(br_actor* pActor);

void FindInverseAndWorldBox(tSpecial_volume* pSpec);

void UpdateSpecVol();

void SaveSpecialVolumes();

void SaveAdditionalStuff();

br_uint_32 ProcessMaterials(br_actor* pActor, tPMFM2CB pCallback);

br_uint_32 ProcessFaceMaterials2(br_actor* pActor, tPMFM2CB pCallback);

void ChangePerspToSubdivCB(br_material* pMaterial);

void ChangePerspToSubdiv();

void ChangeSubdivToPerspCB(br_material* pMaterial);

void ChangeSubdivToPersp();

br_uint_32 ProcessFaceMaterials(br_actor* pActor, tPMFMCB pCallback);

int DRPixelmapHasZeros(br_pixelmap* pm);

int StorageContainsPixelmap(tBrender_storage* pStorage, br_pixelmap* pMap);

void HideStoredOpaqueTextures(tBrender_storage* pStorage);

void RevealStoredTransparentTextures(tBrender_storage* pStorage);

void HideStoredTextures(tBrender_storage* pStorage);

void RevealStoredTextures(tBrender_storage* pStorage);

void SetCarStorageTexturingLevel(tBrender_storage* pStorage, tCar_texturing_level pNew, tCar_texturing_level pOld);

tCar_texturing_level GetCarTexturingLevel();

void SetCarTexturingLevel(tCar_texturing_level pLevel);

int HasThisSuffix(char* pIdent, char* pSuffix);

br_material* UnsuffixedMaterial(char* pOld_ident, char* pSuffix);

br_material* RoadUntexToPersp(br_model* pModel, tU16 pFace);

br_material* WallLinearToUntex(br_model* pModel, tU16 pFace);

br_material* WallUntexToLinear(br_model* pModel, tU16 pFace);

br_material* WallUntexToPersp(br_model* pModel, tU16 pFace);

br_material* WallLinearToPersp(br_model* pModel, tU16 pFace);

tRoad_texturing_level GetRoadTexturingLevel();

void SetRoadTexturingLevel(tRoad_texturing_level pLevel);

void ReallySetRoadTexturingLevel(tRoad_texturing_level pLevel);

tWall_texturing_level GetWallTexturingLevel();

void SetWallTexturingLevel(tWall_texturing_level pLevel);

void ReallySetWallTexturingLevel(tWall_texturing_level pLevel);

br_material* DisposeSuffixedMaterials(br_model* pModel, tU16 pFace);

void DisposeTexturingMaterials();

/*br_uint_32*/ intptr_t SetAccessoryRenderingCB(br_actor* pActor, void* pFlag);

void SetAccessoryRendering(int pOn);

int GetAccessoryRendering();

void SetCarSimplificationLevel(int pLevel);

int GetCarSimplificationLevel();

void ParseSpecialVolume(FILE* pF, tSpecial_volume* pSpec, char* pScreen_name_str);

void AddExceptionToList(tException_list* pDst, tException_list pNew);

void LoadExceptionsFile(char* pName);

void LoadExceptionsFileForTrack(char* pTrack_file_name);

void FreeExceptions();

void LoadTrack(char* pFile_name, tTrack_spec* pTrack_spec, tRace_info* pRace_info);

br_uint_32 RemoveBounds(br_actor* pActor, void* pArg);

void RemoveBoundsStructures(br_actor* pActor);

void FreeTrack(tTrack_spec* pTrack_spec);

void ProcessTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, int pRender_blends);

br_scalar NormaliseDegreeAngle(br_scalar pAngle);

void FunkThoseTronics();

void LollipopizeActor(br_actor* pSubject_actor, br_matrix34* ref_to_world, tLollipop_mode pWhich_axis);

void CalcActorGlobalPos(br_vector3* pResult, br_actor* pActor);

int PointOutOfSight(br_vector3* pPoint, br_scalar pMax_distance);

void PathGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it);

void ObjectGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it);

void GrooveThisDelic(tGroovidelic_spec* pGroove, tU32 pTime, int pInterrupt_it);

void GrooveThoseDelics();

void StopGroovidelic(br_actor* pActor);

void SetGrooveInterrupt(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption);

void ResetGrooveFlags();

tSpecial_volume* GetDefaultSpecialVolumeForWater();

tSpecial_volume* FindSpecialVolume(br_vector3* pP, tSpecial_volume* pLast_vol);

void SaveAdditionalActors();

br_scalar DistanceFromFace(br_vector3* pPos, tFace_ref* pFace);

br_uint_32 CalcHighestID(br_actor* pActor, int* pHighest);

br_uint_32 SetID(br_actor* pActor, void* pArg);

void UniquificateActorsName(br_actor* pUniverse_actor, br_actor* pActor);

void AccessoryHeadup(br_actor* pActor, char* pPrefix);

br_uint_32 CalcHighestNonAmID(br_actor* pActor, int* pHighest);

br_uint_32 SetIDAndDupModel(br_actor* pActor, void* pArg);

void DuplicateIfNotAmpersand(br_actor* pActor);

void DropActor(int pIndex);

void DropActor0();

void DropActor1();

void DropActor2();

void DropActor3();

void DropActor4();

void DropActor5();

void DropActor6();

void DropActor7();

void DropActor8();

void DropActor9();

br_uint_32 IdentifyAccCB(br_actor* pActor, void* pArg);

void IdentifyAcc();

br_uint_32 DelGrooveRef(br_actor* pActor, void* pArg);

br_uint_32 DelReferencedModels(br_actor* pActor, void* pArg);

void DeleteAcc();

br_uint_32 OffsetModel(br_actor* pActor, void* pArg);

void OffsetActor(br_actor* pActor, br_vector3* pOffset);

void CentreActor(br_actor* pActor, br_vector3* pOffset);

void SnapAccToVertical();

void RotateAccessory(br_angle pAngle);

void ScaleAccessory(float pScaling_factor);

void MoveAccessory(br_scalar pX_shift, br_scalar pY_shift, br_scalar pZ_shift);

void RotateAccL();

void RotateAccL2();

void RotateAccL3();

void RotateAccL4();

void RotateAccR();

void RotateAccR2();

void RotateAccR3();

void RotateAccR4();

void CycleAccRotate();

void CycleAccScale();

void ScaleAccUp2();

void ScaleAccUp3();

void ScaleAccUp4();

void ScaleAccDown2();

void ScaleAccDown3();

void ScaleAccDown4();

void MoveXAccL();

void MoveXAccL2();

void MoveXAccL3();

void MoveXAccL4();

void MoveXAccR();

void MoveXAccR2();

void MoveXAccR3();

void MoveXAccR4();

void MoveYAccL();

void MoveYAccL2();

void MoveYAccL3();

void MoveYAccL4();

void MoveYAccR();

void MoveYAccR2();

void MoveYAccR3();

void MoveYAccR4();

void MoveZAccL();

void MoveZAccL2();

void MoveZAccL3();

void MoveZAccL4();

void MoveZAccR();

void MoveZAccR2();

void MoveZAccR3();

void MoveZAccR4();

br_material* GetInternalMat();

br_material* GetExternalMat();

void BuildSpecVolModel(tSpecial_volume* pSpec, int pIndex, br_material* pInt_mat, br_material* pExt_mat);

void DropSpecVol(int pIndex);

void DropSpecVol0();

void DropSpecVol1();

void DropSpecVol2();

void DropSpecVol3();

void DropSpecVol4();

void DropSpecVol5();

void DropSpecVol6();

void DropSpecVol7();

void DropSpecVol8();

void DropSpecVol9();

void IdentifySpecVol();

void DelSpecVolumeGraph(int pIndex);

void DeleteSpecVol();

void RotateSpecVolL();

void RotateSpecVolL2();

void RotateSpecVolL3();

void RotateSpecVolL4();

void RotateSpecVolR();

void RotateSpecVolR2();

void RotateSpecVolR3();

void RotateSpecVolR4();

void CycleSpecVolRotate();

void CycleSpecVolScale();

void ScaleSpecVolUp2();

void ScaleSpecVolUp3();

void ScaleSpecVolUp4();

void ScaleSpecVolDown2();

void ScaleSpecVolDown3();

void ScaleSpecVolDown4();

void MoveXSpecVolL();

void MoveXSpecVolL2();

void MoveXSpecVolL3();

void MoveXSpecVolL4();

void MoveXSpecVolR();

void MoveXSpecVolR2();

void MoveXSpecVolR3();

void MoveXSpecVolR4();

void MoveYSpecVolL();

void MoveYSpecVolL2();

void MoveYSpecVolL3();

void MoveYSpecVolL4();

void MoveYSpecVolR();

void MoveYSpecVolR2();

void MoveYSpecVolR3();

void MoveYSpecVolR4();

void MoveZSpecVolL();

void MoveZSpecVolL2();

void MoveZSpecVolL3();

void MoveZSpecVolL4();

void MoveZSpecVolR();

void MoveZSpecVolR2();

void MoveZSpecVolR3();

void MoveZSpecVolR4();

void SnapSpecVolToVertical();

void ShowSpecialVolumes();

void HideSpecialVolumes();

#endif
