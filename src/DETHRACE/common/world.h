#ifndef _WORLD_H_
#define _WORLD_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 88
float MapSawToTriangle(float pNumber);

// Offset: 88
// Size: 62
void SetSightDistance(br_scalar pYon);

// Offset: 152
// Size: 121
// EAX: pThe_name
br_actor* FindActorInArray(char* pThe_name);

// Offset: 276
// Size: 121
// EAX: pThe_name
br_actor* FindLightInArray(char* pThe_name);

// Offset: 400
// Size: 223
// EAX: pSource_actor
br_actor* CloneActor(br_actor* pSource_actor);

// Offset: 624
// Size: 247
// EAX: pStorage_space
// EDX: pMax_pixelmaps
// EBX: pMax_shade_tables
// ECX: pMax_materials
void InitialiseStorageSpace(tBrender_storage* pStorage_space, int pMax_pixelmaps, int pMax_shade_tables, int pMax_materials, int pMax_models);

// Offset: 872
// Size: 107
// EAX: pStorage_space
void DisposeStorageSpace(tBrender_storage* pStorage_space);

// Offset: 980
// Size: 463
// EAX: pStorage_space
void ClearOutStorageSpace(tBrender_storage* pStorage_space);

// Offset: 1444
// Size: 232
// EAX: pStorage_space
// EDX: pThe_pm
tAdd_to_storage_result AddPixelmapToStorage(tBrender_storage* pStorage_space, br_pixelmap** pThe_pm);

// Offset: 1676
// Size: 211
// EAX: pStorage_space
// EDX: pThe_st
tAdd_to_storage_result AddShadeTableToStorage(tBrender_storage* pStorage_space, br_pixelmap* pThe_st);

// Offset: 1888
// Size: 234
// EAX: pStorage_space
// EDX: pThe_mat
tAdd_to_storage_result AddMaterialToStorage(tBrender_storage* pStorage_space, br_material* pThe_mat);

// Offset: 2124
// Size: 231
// EAX: pStorage_space
// EDX: pThe_mod
tAdd_to_storage_result AddModelToStorage(tBrender_storage* pStorage_space, br_model* pThe_mod);

// Offset: 2356
// Size: 507
// EAX: pStorage_space
// EDX: pF
// EBX: pCount
int LoadNPixelmaps(tBrender_storage* pStorage_space, FILE* pF, int pCount);

// Offset: 2864
// Size: 195
// EAX: pStorage_space
// EDX: pName
br_pixelmap* LoadSinglePixelmap(tBrender_storage* pStorage_space, char* pName);

// Offset: 3060
// Size: 195
// EAX: pStorage_space
// EDX: pName
br_material* LoadSingleMaterial(tBrender_storage* pStorage_space, char* pName);

// Offset: 3256
// Size: 394
// EAX: pStorage_space
// EDX: pF
// EBX: pCount
int LoadNShadeTables(tBrender_storage* pStorage_space, FILE* pF, int pCount);

// Offset: 3652
// Size: 195
// EAX: pStorage_space
// EDX: pName
br_pixelmap* LoadSingleShadeTable(tBrender_storage* pStorage_space, char* pName);

// Offset: 3848
// Size: 414
// EAX: pStorage_space
// EDX: pF
// EBX: pCount
int LoadNMaterials(tBrender_storage* pStorage_space, FILE* pF, int pCount);

// Offset: 4264
// Size: 449
// EAX: pStorage_space
// EDX: pF
// EBX: pCount
int LoadNModels(tBrender_storage* pStorage_space, FILE* pF, int pCount);

// Offset: 4716
// Size: 103
// EAX: pM
void DodgyModelUpdate(br_model* pM);

// Offset: 4820
// Size: 197
// EAX: pOld
// EDX: pSuffix
br_material* SuffixedMaterial(br_material* pOld, char* pSuffix);

// Offset: 5020
// Size: 656
// EAX: pModel
// EDX: pFace
int FaceIsRoad(br_model* pModel, tU16 pFace);

// Offset: 5676
// Size: 161
// EAX: pModel
// EDX: pFace
br_material* RoadPerspToUntex(br_model* pModel, tU16 pFace);

// Offset: 5840
// Size: 169
// EAX: pModel
// EDX: pFace
br_material* WallPerspToLinear(br_model* pModel, tU16 pFace);

// Offset: 6012
// Size: 188
// EAX: pModel
// EDX: pFace
br_material* WallPerspToUntex(br_model* pModel, tU16 pFace);

// Offset: 6200
// Size: 260
// EAX: pModel
// EDX: pCallback
void ProcessModelFaceMaterials2(br_model* pModel, tPMFM2CB pCallback);

// Offset: 6460
// Size: 173
// EAX: pModel
// EDX: pCallback
void ProcessModelFaceMaterials(br_model* pModel, tPMFMCB pCallback);

// Offset: 6636
// Size: 554
// EAX: pStorage_space
// EDX: pF
// EBX: pCount
int LoadNTrackModels(tBrender_storage* pStorage_space, FILE* pF, int pCount);

// Offset: 7192
// Size: 107
// EAX: pStorage_space
// EDX: pF
void LoadSomePixelmaps(tBrender_storage* pStorage_space, FILE* pF);

// Offset: 7300
// Size: 107
// EAX: pStorage_space
// EDX: pF
void LoadSomeShadeTables(tBrender_storage* pStorage_space, FILE* pF);

// Offset: 7408
// Size: 107
// EAX: pStorage_space
// EDX: pF
void LoadSomeMaterials(tBrender_storage* pStorage_space, FILE* pF);

// Offset: 7516
// Size: 107
// EAX: pStorage_space
// EDX: pF
void LoadSomeModels(tBrender_storage* pStorage_space, FILE* pF);

// Offset: 7624
// Size: 107
// EAX: pStorage_space
// EDX: pF
void LoadSomeTrackModels(tBrender_storage* pStorage_space, FILE* pF);

// Offset: 7732
// Size: 87
// EAX: pSlot_number
// EDX: pPeriod_address
void AddFunkGrooveBinding(int pSlot_number, float* pPeriod_address);

// Offset: 7820
// Size: 81
// EAX: pSlot_number
void ControlBoundFunkGroove(int pSlot_number, float pValue);

// Offset: 7904
// Size: 130
// EAX: pSlot_number
float ControlBoundFunkGroovePlus(int pSlot_number, float pValue);

// Offset: 8036
// Size: 124
// EAX: pStart
// EDX: pEnd
// EBX: pDelta
void ShiftBoundGrooveFunks(char* pStart, char* pEnd, int pDelta);

// Offset: 8160
// Size: 361
tFunkotronic_spec* AddNewFunkotronic();

// Offset: 8524
// Size: 261
// EAX: pOwner
void DisposeFunkotronics(int pOwner);

// Offset: 8788
// Size: 76
// EAX: pV
// EDX: pThe_funk
void AddProximityVertex(br_vector3* pV, tFunkotronic_spec* pThe_funk);

// Offset: 8864
// Size: 65
void AddProximityVertexXYZ(br_scalar pX, br_scalar pY, br_scalar pZ, tFunkotronic_spec* pThe_funk);

// Offset: 8932
// Size: 167
// EAX: pActor
// EDX: pMat
// EBX: pThe_funk
br_uint_32 CalcProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk);

// Offset: 9100
// Size: 550
// EAX: pActor
// EDX: pMat
// EBX: pThe_funk
br_uint_32 AddProximities(br_actor* pActor, br_material* pMat, tFunkotronic_spec* pThe_funk);

// Offset: 9652
// Size: 152
// EAX: pVictim1
// EDX: pVictim2
// EBX: pCulprit
void Adjust2FloatsForExceptions(float* pVictim1, float* pVictim2, br_pixelmap* pCulprit);

// Offset: 9804
// Size: 3256
// EAX: pF
// EDX: pOwner
// EBX: pRef_offset
void AddFunkotronics(FILE* pF, int pOwner, int pRef_offset);

// Offset: 13060
// Size: 118
// EAX: pOwner
void DisposeGroovidelics(int pOwner);

// Offset: 13180
// Size: 349
tGroovidelic_spec* AddNewGroovidelic();

// Offset: 13532
// Size: 2614
// EAX: pF
// EDX: pOwner
// EBX: pParent_actor
// ECX: pRef_offset
void AddGroovidelics(FILE* pF, int pOwner, br_actor* pParent_actor, int pRef_offset, int pAllowed_to_be_absent);

// Offset: 16148
// Size: 161
// EAX: pOwner
void KillGroovadelic(int pOwner);

// Offset: 16312
// Size: 186
// EAX: pOwner
void KillFunkotronic(int pOwner);

// Offset: 16500
// Size: 236
// EAX: pActor
// EDX: pMatrix
// EBX: pArg
br_uint_32 DeleteBastards(br_actor* pActor, br_matrix34* pMatrix, void* pArg);

// Offset: 16736
// Size: 110
void DeleteAnyZeroBastards();

// Offset: 16848
// Size: 219
// EAX: pActor
// EDX: pMatrix
// EBX: pArg
br_uint_32 ApplyTransToModels(br_actor* pActor, br_matrix34* pMatrix, void* pArg);

// Offset: 17068
// Size: 120
// EAX: pActor
int FindSpecVolIndex(br_actor* pActor);

// Offset: 17188
// Size: 244
// EAX: pMat
// EDX: pMat_1
// EBX: pMat_2
// ECX: pAxis_0
void MungeMaterial(br_matrix34* pMat, br_material* pMat_1, br_material* pMat_2, int pAxis_0, int pAxis_1);

// Offset: 17432
// Size: 166
// EAX: pActor
void SetSpecVolMatSize(br_actor* pActor);

// Offset: 17600
// Size: 115
// EAX: pSpec
void FindInverseAndWorldBox(tSpecial_volume* pSpec);

// Offset: 17716
// Size: 109
void UpdateSpecVol();

// Offset: 17828
// Size: 834
void SaveSpecialVolumes();

// Offset: 18664
// Size: 137
void SaveAdditionalStuff();

// Offset: 18804
// Size: 110
br_uint_32 ProcessMaterials(br_actor* pActor, tPMFM2CB pCallback);

// Offset: 18916
// Size: 123
br_uint_32 ProcessFaceMaterials2(br_actor* pActor, tPMFM2CB pCallback);

// Offset: 19040
// Size: 91
// EAX: pMaterial
void ChangePerspToSubdivCB(br_material* pMaterial);

// Offset: 19132
// Size: 54
void ChangePerspToSubdiv();

// Offset: 19188
// Size: 78
// EAX: pMaterial
void ChangeSubdivToPerspCB(br_material* pMaterial);

// Offset: 19268
// Size: 54
void ChangeSubdivToPersp();

// Offset: 19324
// Size: 123
br_uint_32 ProcessFaceMaterials(br_actor* pActor, tPMFMCB pCallback);

// Offset: 19448
// Size: 236
// EAX: pm
int DRPixelmapHasZeros(br_pixelmap* pm);

// Offset: 19684
// Size: 125
// EAX: pStorage
// EDX: pMap
int StorageContainsPixelmap(tBrender_storage* pStorage, br_pixelmap* pMap);

// Offset: 19812
// Size: 266
// EAX: pStorage
void HideStoredOpaqueTextures(tBrender_storage* pStorage);

// Offset: 20080
// Size: 224
// EAX: pStorage
void RevealStoredTransparentTextures(tBrender_storage* pStorage);

// Offset: 20304
// Size: 236
// EAX: pStorage
void HideStoredTextures(tBrender_storage* pStorage);

// Offset: 20540
// Size: 197
// EAX: pStorage
void RevealStoredTextures(tBrender_storage* pStorage);

// Offset: 20740
// Size: 121
// EAX: pStorage
// EDX: pNew
// EBX: pOld
void SetCarStorageTexturingLevel(tBrender_storage* pStorage, tCar_texturing_level pNew, tCar_texturing_level pOld);

// Offset: 20864
// Size: 45
tCar_texturing_level GetCarTexturingLevel();

// Offset: 20912
// Size: 139
// EAX: pLevel
void SetCarTexturingLevel(tCar_texturing_level pLevel);

// Offset: 21052
// Size: 138
// EAX: pIdent
// EDX: pSuffix
int HasThisSuffix(char* pIdent, char* pSuffix);

// Offset: 21192
// Size: 145
// EAX: pOld_ident
// EDX: pSuffix
br_material* UnsuffixedMaterial(char* pOld_ident, char* pSuffix);

// Offset: 21340
// Size: 118
// EAX: pModel
// EDX: pFace
br_material* RoadUntexToPersp(br_model* pModel, tU16 pFace);

// Offset: 21460
// Size: 240
// EAX: pModel
// EDX: pFace
br_material* WallLinearToUntex(br_model* pModel, tU16 pFace);

// Offset: 21700
// Size: 203
// EAX: pModel
// EDX: pFace
br_material* WallUntexToLinear(br_model* pModel, tU16 pFace);

// Offset: 21904
// Size: 153
// EAX: pModel
// EDX: pFace
br_material* WallUntexToPersp(br_model* pModel, tU16 pFace);

// Offset: 22060
// Size: 112
// EAX: pModel
// EDX: pFace
br_material* WallLinearToPersp(br_model* pModel, tU16 pFace);

// Offset: 22172
// Size: 45
tRoad_texturing_level GetRoadTexturingLevel();

// Offset: 22220
// Size: 45
// EAX: pLevel
void SetRoadTexturingLevel(tRoad_texturing_level pLevel);

// Offset: 22268
// Size: 86
// EAX: pLevel
void ReallySetRoadTexturingLevel(tRoad_texturing_level pLevel);

// Offset: 22356
// Size: 45
tWall_texturing_level GetWallTexturingLevel();

// Offset: 22404
// Size: 45
// EAX: pLevel
void SetWallTexturingLevel(tWall_texturing_level pLevel);

// Offset: 22452
// Size: 82
// EAX: pLevel
void ReallySetWallTexturingLevel(tWall_texturing_level pLevel);

// Offset: 22536
// Size: 297
// EAX: pModel
// EDX: pFace
br_material* DisposeSuffixedMaterials(br_model* pModel, tU16 pFace);

// Offset: 22836
// Size: 161
void DisposeTexturingMaterials();

// Offset: 23000
// Size: 73
br_uint_32 SetAccessoryRenderingCB(br_actor* pActor, void* pFlag);

// Offset: 23076
// Size: 100
// EAX: pOn
void SetAccessoryRendering(int pOn);

// Offset: 23176
// Size: 45
int GetAccessoryRendering();

// Offset: 23224
// Size: 45
// EAX: pLevel
void SetCarSimplificationLevel(int pLevel);

// Offset: 23272
// Size: 45
int GetCarSimplificationLevel();

// Offset: 23320
// Size: 263
// EAX: pF
// EDX: pSpec
// EBX: pScreen_name_str
void ParseSpecialVolume(FILE* pF, tSpecial_volume* pSpec, char* pScreen_name_str);

// Offset: 23584
// Size: 56
// EAX: pDst
// EDX: pNew
void AddExceptionToList(tException_list* pDst, tException_list pNew);

// Offset: 23640
// Size: 527
// EAX: pName
void LoadExceptionsFile(char* pName);

// Offset: 24168
// Size: 91
// EAX: pTrack_file_name
void LoadExceptionsFileForTrack(char* pTrack_file_name);

// Offset: 24260
// Size: 104
void FreeExceptions();

// Offset: 24364
// Size: 7310
// EAX: pFile_name
// EDX: pTrack_spec
// EBX: pRace_info
void LoadTrack(char* pFile_name, tTrack_spec* pTrack_spec, tRace_info* pRace_info);

// Offset: 31676
// Size: 82
br_uint_32 RemoveBounds(br_actor* pActor, void* pArg);

// Offset: 31760
// Size: 52
// EAX: pActor
void RemoveBoundsStructures(br_actor* pActor);

// Offset: 31812
// Size: 439
// EAX: pTrack_spec
void FreeTrack(tTrack_spec* pTrack_spec);

// Offset: 32252
// Size: 94
// EAX: pWorld
// EDX: pTrack_spec
// EBX: pCamera
// ECX: pCamera_to_world_transform
void ProcessTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, int pRender_blends);

// Offset: 32348
// Size: 71
br_scalar NormaliseDegreeAngle(br_scalar pAngle);

// Offset: 32420
// Size: 9373
void FunkThoseTronics();

// Offset: 41796
// Size: 803
// EAX: pSubject_actor
// EDX: ref_to_world
// EBX: pWhich_axis
void LollipopizeActor(br_actor* pSubject_actor, br_matrix34* ref_to_world, tLollipop_mode pWhich_axis);

// Offset: 42600
// Size: 140
// EAX: pResult
// EDX: pActor
void CalcActorGlobalPos(br_vector3* pResult, br_actor* pActor);

// Offset: 42740
// Size: 320
// EAX: pPoint
int PointOutOfSight(br_vector3* pPoint, br_scalar pMax_distance);

// Offset: 43060
// Size: 3696
// EAX: pGroove
// EDX: pTime
// EBX: pMat
// ECX: pInterrupt_it
void PathGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it);

// Offset: 46756
// Size: 9418
// EAX: pGroove
// EDX: pTime
// EBX: pMat
// ECX: pInterrupt_it
void ObjectGrooveBastard(tGroovidelic_spec* pGroove, tU32 pTime, br_matrix34* pMat, int pInterrupt_it);

// Offset: 56176
// Size: 611
// EAX: pGroove
// EDX: pTime
// EBX: pInterrupt_it
void GrooveThisDelic(tGroovidelic_spec* pGroove, tU32 pTime, int pInterrupt_it);

// Offset: 56788
// Size: 191
void GrooveThoseDelics();

// Offset: 56980
// Size: 137
// EAX: pActor
void StopGroovidelic(br_actor* pActor);

// Offset: 57120
// Size: 117
// EAX: pGroove_index
// EDX: pMatrix
// EBX: pPath_interrupt
// ECX: pObject_interrupt
void SetGrooveInterrupt(int pGroove_index, br_matrix34* pMatrix, int pPath_interrupt, int pObject_interrupt, float pPath_resumption, float pObject_resumption);

// Offset: 57240
// Size: 92
void ResetGrooveFlags();

// Offset: 57332
// Size: 45
tSpecial_volume* GetDefaultSpecialVolumeForWater();

// Offset: 57380
// Size: 342
// EAX: pP
// EDX: pLast_vol
tSpecial_volume* FindSpecialVolume(br_vector3* pP, tSpecial_volume* pLast_vol);

// Offset: 57724
// Size: 48
void SaveAdditionalActors();

// Offset: 57772
// Size: 89
// EAX: pPos
// EDX: pFace
br_scalar DistanceFromFace(br_vector3* pPos, tFace_ref* pFace);

// Offset: 57864
// Size: 166
br_uint_32 CalcHighestID(br_actor* pActor, int* pHighest);

// Offset: 58032
// Size: 182
br_uint_32 SetID(br_actor* pActor, void* pArg);

// Offset: 58216
// Size: 100
// EAX: pUniverse_actor
// EDX: pActor
void UniquificateActorsName(br_actor* pUniverse_actor, br_actor* pActor);

// Offset: 58316
// Size: 107
// EAX: pActor
// EDX: pPrefix
void AccessoryHeadup(br_actor* pActor, char* pPrefix);

// Offset: 58424
// Size: 132
br_uint_32 CalcHighestNonAmID(br_actor* pActor, int* pHighest);

// Offset: 58556
// Size: 443
br_uint_32 SetIDAndDupModel(br_actor* pActor, void* pArg);

// Offset: 59000
// Size: 100
// EAX: pActor
void DuplicateIfNotAmpersand(br_actor* pActor);

// Offset: 59100
// Size: 1246
// EAX: pIndex
void DropActor(int pIndex);

// Offset: 60348
// Size: 41
void DropActor0();

// Offset: 60392
// Size: 44
void DropActor1();

// Offset: 60436
// Size: 44
void DropActor2();

// Offset: 60480
// Size: 44
void DropActor3();

// Offset: 60524
// Size: 44
void DropActor4();

// Offset: 60568
// Size: 44
void DropActor5();

// Offset: 60612
// Size: 44
void DropActor6();

// Offset: 60656
// Size: 44
void DropActor7();

// Offset: 60700
// Size: 44
void DropActor8();

// Offset: 60744
// Size: 44
void DropActor9();

// Offset: 60788
// Size: 273
br_uint_32 IdentifyAccCB(br_actor* pActor, void* pArg);

// Offset: 61064
// Size: 105
void IdentifyAcc();

// Offset: 61172
// Size: 108
br_uint_32 DelGrooveRef(br_actor* pActor, void* pArg);

// Offset: 61280
// Size: 177
br_uint_32 DelReferencedModels(br_actor* pActor, void* pArg);

// Offset: 61460
// Size: 135
void DeleteAcc();

// Offset: 61596
// Size: 170
br_uint_32 OffsetModel(br_actor* pActor, void* pArg);

// Offset: 61768
// Size: 54
// EAX: pActor
// EDX: pOffset
void OffsetActor(br_actor* pActor, br_vector3* pOffset);

// Offset: 61824
// Size: 286
// EAX: pActor
// EDX: pOffset
void CentreActor(br_actor* pActor, br_vector3* pOffset);

// Offset: 62112
// Size: 156
void SnapAccToVertical();

// Offset: 62268
// Size: 280
// EAX: pAngle
void RotateAccessory(br_angle pAngle);

// Offset: 62548
// Size: 377
void ScaleAccessory(float pScaling_factor);

// Offset: 62928
// Size: 110
void MoveAccessory(br_scalar pX_shift, br_scalar pY_shift, br_scalar pZ_shift);

// Offset: 63040
// Size: 44
void RotateAccL();

// Offset: 63084
// Size: 44
void RotateAccL2();

// Offset: 63128
// Size: 44
void RotateAccL3();

// Offset: 63172
// Size: 44
void RotateAccL4();

// Offset: 63216
// Size: 44
void RotateAccR();

// Offset: 63260
// Size: 44
void RotateAccR2();

// Offset: 63304
// Size: 44
void RotateAccR3();

// Offset: 63348
// Size: 44
void RotateAccR4();

// Offset: 63392
// Size: 181
void CycleAccRotate();

// Offset: 63576
// Size: 227
void CycleAccScale();

// Offset: 63804
// Size: 44
void ScaleAccUp2();

// Offset: 63848
// Size: 44
void ScaleAccUp3();

// Offset: 63892
// Size: 44
void ScaleAccUp4();

// Offset: 63936
// Size: 44
void ScaleAccDown2();

// Offset: 63980
// Size: 44
void ScaleAccDown3();

// Offset: 64024
// Size: 44
void ScaleAccDown4();

// Offset: 64068
// Size: 48
void MoveXAccL();

// Offset: 64116
// Size: 48
void MoveXAccL2();

// Offset: 64164
// Size: 48
void MoveXAccL3();

// Offset: 64212
// Size: 48
void MoveXAccL4();

// Offset: 64260
// Size: 48
void MoveXAccR();

// Offset: 64308
// Size: 48
void MoveXAccR2();

// Offset: 64356
// Size: 48
void MoveXAccR3();

// Offset: 64404
// Size: 48
void MoveXAccR4();

// Offset: 64452
// Size: 48
void MoveYAccL();

// Offset: 64500
// Size: 48
void MoveYAccL2();

// Offset: 64548
// Size: 48
void MoveYAccL3();

// Offset: 64596
// Size: 48
void MoveYAccL4();

// Offset: 64644
// Size: 48
void MoveYAccR();

// Offset: 64692
// Size: 48
void MoveYAccR2();

// Offset: 64740
// Size: 48
void MoveYAccR3();

// Offset: 64788
// Size: 48
void MoveYAccR4();

// Offset: 64836
// Size: 48
void MoveZAccL();

// Offset: 64884
// Size: 48
void MoveZAccL2();

// Offset: 64932
// Size: 48
void MoveZAccL3();

// Offset: 64980
// Size: 48
void MoveZAccL4();

// Offset: 65028
// Size: 48
void MoveZAccR();

// Offset: 65076
// Size: 48
void MoveZAccR2();

// Offset: 65124
// Size: 48
void MoveZAccR3();

// Offset: 65172
// Size: 48
void MoveZAccR4();

// Offset: 65220
// Size: 54
br_material* GetInternalMat();

// Offset: 65276
// Size: 54
br_material* GetExternalMat();

// Offset: 65332
// Size: 2149
// EAX: pSpec
// EDX: pIndex
// EBX: pInt_mat
// ECX: pExt_mat
void BuildSpecVolModel(tSpecial_volume* pSpec, int pIndex, br_material* pInt_mat, br_material* pExt_mat);

// Offset: 67484
// Size: 427
// EAX: pIndex
void DropSpecVol(int pIndex);

// Offset: 67912
// Size: 34
void DropSpecVol0();

// Offset: 67948
// Size: 44
void DropSpecVol1();

// Offset: 67992
// Size: 44
void DropSpecVol2();

// Offset: 68036
// Size: 44
void DropSpecVol3();

// Offset: 68080
// Size: 44
void DropSpecVol4();

// Offset: 68124
// Size: 44
void DropSpecVol5();

// Offset: 68168
// Size: 44
void DropSpecVol6();

// Offset: 68212
// Size: 44
void DropSpecVol7();

// Offset: 68256
// Size: 44
void DropSpecVol8();

// Offset: 68300
// Size: 44
void DropSpecVol9();

// Offset: 68344
// Size: 344
void IdentifySpecVol();

// Offset: 68688
// Size: 349
// EAX: pIndex
void DelSpecVolumeGraph(int pIndex);

// Offset: 69040
// Size: 250
void DeleteSpecVol();

// Offset: 69292
// Size: 39
void RotateSpecVolL();

// Offset: 69332
// Size: 39
void RotateSpecVolL2();

// Offset: 69372
// Size: 39
void RotateSpecVolL3();

// Offset: 69412
// Size: 39
void RotateSpecVolL4();

// Offset: 69452
// Size: 39
void RotateSpecVolR();

// Offset: 69492
// Size: 39
void RotateSpecVolR2();

// Offset: 69532
// Size: 39
void RotateSpecVolR3();

// Offset: 69572
// Size: 39
void RotateSpecVolR4();

// Offset: 69612
// Size: 39
void CycleSpecVolRotate();

// Offset: 69652
// Size: 39
void CycleSpecVolScale();

// Offset: 69692
// Size: 39
void ScaleSpecVolUp2();

// Offset: 69732
// Size: 39
void ScaleSpecVolUp3();

// Offset: 69772
// Size: 39
void ScaleSpecVolUp4();

// Offset: 69812
// Size: 39
void ScaleSpecVolDown2();

// Offset: 69852
// Size: 39
void ScaleSpecVolDown3();

// Offset: 69892
// Size: 39
void ScaleSpecVolDown4();

// Offset: 69932
// Size: 39
void MoveXSpecVolL();

// Offset: 69972
// Size: 39
void MoveXSpecVolL2();

// Offset: 70012
// Size: 39
void MoveXSpecVolL3();

// Offset: 70052
// Size: 39
void MoveXSpecVolL4();

// Offset: 70092
// Size: 39
void MoveXSpecVolR();

// Offset: 70132
// Size: 39
void MoveXSpecVolR2();

// Offset: 70172
// Size: 39
void MoveXSpecVolR3();

// Offset: 70212
// Size: 39
void MoveXSpecVolR4();

// Offset: 70252
// Size: 39
void MoveYSpecVolL();

// Offset: 70292
// Size: 39
void MoveYSpecVolL2();

// Offset: 70332
// Size: 39
void MoveYSpecVolL3();

// Offset: 70372
// Size: 39
void MoveYSpecVolL4();

// Offset: 70412
// Size: 39
void MoveYSpecVolR();

// Offset: 70452
// Size: 39
void MoveYSpecVolR2();

// Offset: 70492
// Size: 39
void MoveYSpecVolR3();

// Offset: 70532
// Size: 39
void MoveYSpecVolR4();

// Offset: 70572
// Size: 39
void MoveZSpecVolL();

// Offset: 70612
// Size: 39
void MoveZSpecVolL2();

// Offset: 70652
// Size: 39
void MoveZSpecVolL3();

// Offset: 70692
// Size: 39
void MoveZSpecVolL4();

// Offset: 70732
// Size: 39
void MoveZSpecVolR();

// Offset: 70772
// Size: 39
void MoveZSpecVolR2();

// Offset: 70812
// Size: 39
void MoveZSpecVolR3();

// Offset: 70852
// Size: 39
void MoveZSpecVolR4();

// Offset: 70892
// Size: 39
void SnapSpecVolToVertical();

// Offset: 70932
// Size: 165
void ShowSpecialVolumes();

// Offset: 71100
// Size: 112
void HideSpecialVolumes();

#endif
