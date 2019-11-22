#ifndef _LOADING_H_
#define _LOADING_H_

#include "dr_types.h"
#include "br_types.h"

extern int gDecode_thing;

// Offset: 0
// Size: 67
// EAX: pF
tU32 ReadU32(FILE *pF);

// Offset: 68
// Size: 67
// EAX: pF
tU16 ReadU16(FILE *pF);

// Offset: 136
// Size: 67
// EAX: pF
tU8 ReadU8(FILE *pF);

// Offset: 204
// Size: 67
// EAX: pF
tS32 ReadS32(FILE *pF);

// Offset: 272
// Size: 67
// EAX: pF
tS16 ReadS16(FILE *pF);

// Offset: 340
// Size: 67
// EAX: pF
tS8 ReadS8(FILE *pF);

// Offset: 408
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU32L(FILE *pF, tU32 pNumber);

// Offset: 476
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU16L(FILE *pF, tU16 pNumber);

// Offset: 544
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteU8L(FILE *pF, tU8 pNumber);

// Offset: 612
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS32L(FILE *pF, tS32 pNumber);

// Offset: 680
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS16L(FILE *pF, tS16 pNumber);

// Offset: 748
// Size: 65
// EAX: pF
// EDX: pNumber
void WriteS8L(FILE *pF, tS8 pNumber);

// Offset: 816
// Size: 54
// EAX: pF
// EDX: pBytes_to_skip
void SkipBytes(FILE *pF, int pBytes_to_skip);

// Offset: 872
// Size: 70
// EAX: pPtr
tU32 MemReadU32(char **pPtr);

// Offset: 944
// Size: 70
// EAX: pPtr
tU16 MemReadU16(char **pPtr);

// Offset: 1016
// Size: 55
// EAX: pPtr
tU8 MemReadU8(char **pPtr);

// Offset: 1072
// Size: 70
// EAX: pPtr
tS32 MemReadS32(char **pPtr);

// Offset: 1144
// Size: 70
// EAX: pPtr
tS16 MemReadS16(char **pPtr);

// Offset: 1216
// Size: 55
// EAX: pPtr
tS8 MemReadS8(char **pPtr);

// Offset: 1272
// Size: 46
// EAX: pPtr
// EDX: pBytes_to_skip
void MemSkipBytes(char **pPtr, int pBytes_to_skip);

// Offset: 1320
// Size: 2149
void LoadGeneralParameters();

// Offset: 3472
// Size: 53
void FinishLoadingGeneral();

// Offset: 3528
// Size: 292
// EAX: pName
br_pixelmap* LoadPixelmap(char *pName);

// Offset: 3820
// Size: 222
// EAX: pFile_name
// EDX: pPixelmaps
// EBX: pNum
br_uint_32 LoadPixelmaps(char *pFile_name, br_pixelmap **pPixelmaps, br_uint_16 pNum);

// Offset: 4044
// Size: 99
// EAX: pName
br_pixelmap* LoadShadeTable(char *pName);

// Offset: 4144
// Size: 129
// EAX: pName
br_material* LoadMaterial(char *pName);

// Offset: 4276
// Size: 123
// EAX: pName
br_model* LoadModel(char *pName);

// Offset: 4400
// Size: 104
// EAX: pName
br_actor* LoadActor(char *pName);

// Offset: 4504
// Size: 78
// EAX: pPath_name
void DRLoadPalette(char *pPath_name);

// Offset: 4584
// Size: 78
// EAX: pPath_name
void DRLoadShadeTable(char *pPath_name);

// Offset: 4664
// Size: 84
// EAX: pPixelmap_array
// EDX: pCount
void RezeroPixelmaps(br_pixelmap **pPixelmap_array, int pCount);

// Offset: 4748
// Size: 97
// EAX: pPath_name
void DRLoadPixelmaps(char *pPath_name);

// Offset: 4848
// Size: 103
// EAX: pPath_name
void DRLoadMaterials(char *pPath_name);

// Offset: 4952
// Size: 103
// EAX: pPath_name
void DRLoadModels(char *pPath_name);

// Offset: 5056
// Size: 134
// EAX: pPath_name
void DRLoadActors(char *pPath_name);

// Offset: 5192
// Size: 134
// EAX: pPath_name
void DRLoadLights(char *pPath_name);

// Offset: 5328
// Size: 70
// EAX: pThe_base_path
// EDX: pThe_dir_name
// EBX: pLoad_routine
void LoadInFiles(char *pThe_base_path, char *pThe_dir_name, void (*pLoad_routine)(char*));

// Offset: 5400
// Size: 208
// EAX: pThe_dir_path
void LoadInRegisteeDir(char *pThe_dir_path);

// Offset: 5608
// Size: 44
void LoadInRegistees();

// Offset: 5652
// Size: 182
void LoadKeyMapping();

// Offset: 5836
// Size: 269
// EAX: pWithin_race
void LoadInterfaceStuff(int pWithin_race);

// Offset: 6108
// Size: 272
void UnlockInterfaceStuff();

// Offset: 6380
// Size: 75
void InitInterfaceLoadState();

// Offset: 6456
// Size: 627
// EAX: pBr_map
tS8* ConvertPixTo16BitStripMap(br_pixelmap *pBr_map);

// Offset: 7084
// Size: 578
// EAX: pThe_br_map
tS8* ConvertPixToStripMap(br_pixelmap *pThe_br_map);

// Offset: 7664
// Size: 147
// EAX: pModel
// EDX: pMaterial
void KillWindscreen(br_model *pModel, br_material *pMaterial);

// Offset: 7812
// Size: 103
// EAX: pCar
void DropOffDyingPeds(tCar_spec *pCar);

// Offset: 7916
// Size: 1113
// EAX: pCar_spec
// EDX: pOwner
void DisposeCar(tCar_spec *pCar_spec, int pOwner);

// Offset: 9032
// Size: 626
// EAX: pCar
void AdjustCarCoordinates(tCar_spec *pCar);

// Offset: 9660
// Size: 929
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadSpeedo(FILE *pF, int pIndex, tCar_spec *pCar_spec);

// Offset: 10592
// Size: 764
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadTacho(FILE *pF, int pIndex, tCar_spec *pCar_spec);

// Offset: 11356
// Size: 816
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadHeadups(FILE *pF, int pIndex, tCar_spec *pCar_spec);

// Offset: 12172
// Size: 1819
// EAX: pF
// EDX: non_car
void ReadNonCarMechanicsData(FILE *pF, tNon_car_spec *non_car);

// Offset: 13992
// Size: 3476
// EAX: pF
// EDX: c
void ReadMechanicsData(FILE *pF, tCar_spec *c);

// Offset: 17468
// Size: 196
// EAX: pF
// EDX: pIndex
// EBX: pCar_spec
void LoadGear(FILE *pF, int pIndex, tCar_spec *pCar_spec);

// Offset: 17664
// Size: 56
// EAX: pRef_holder
void AddRefOffset(int *pRef_holder);

// Offset: 17720
// Size: 697
// EAX: pF
// EDX: pCar_spec
// EBX: pImpact_location
void GetDamageProgram(FILE *pF, tCar_spec *pCar_spec, tImpact_location pImpact_location);

// Offset: 18420
// Size: 178
br_uint_32 LinkModel(br_actor *pActor, tModel_pool *pModel_pool);

// Offset: 18600
// Size: 198
// EAX: pModel_array
// EDX: pModel_count
void FreeUpBonnetModels(br_model **pModel_array, int pModel_count);

// Offset: 18800
// Size: 67
// EAX: pActor
// EDX: pModel_array
// EBX: pModel_count
void LinkModelsToActor(br_actor *pActor, br_model **pModel_array, int pModel_count);

// Offset: 18868
// Size: 132
// EAX: pF
// EDX: pCar_spec
void ReadShrapnelMaterials(FILE *pF, tCollision_info *pCar_spec);

// Offset: 19000
// Size: 200
// EAX: pOutput_car
// EDX: pInput_car
void CloneCar(tCar_spec **pOutput_car, tCar_spec *pInput_car);

// Offset: 19200
// Size: 65
// EAX: pCar
void DisposeClonedCar(tCar_spec *pCar);

// Offset: 19268
// Size: 523
// EAX: pModel
int RemoveDoubleSided(br_model *pModel);

// Offset: 19792
// Size: 201
// EAX: pModel
void MungeWindscreen(br_model *pModel);

// Offset: 19996
// Size: 141
// EAX: pModel
// EDX: pOwner
void SetModelFlags(br_model *pModel, int pOwner);

// Offset: 20140
// Size: 8202
// EAX: pCar_name
// EDX: pDriver
// EBX: pCar_spec
// ECX: pOwner
void LoadCar(char *pCar_name, tDriver pDriver, tCar_spec *pCar_spec, int pOwner, char *pDriver_name, tBrender_storage *pStorage_space);

// Offset: 28344
// Size: 186
void LoadHeadupImages();

// Offset: 28532
// Size: 94
void DisposeHeadupImages();

// Offset: 28628
// Size: 109
FILE* OpenRaceFile();

// Offset: 28740
// Size: 219
// EAX: pF
void SkipRestOfRace(FILE *pF);

// Offset: 28960
// Size: 411
// EAX: pRace_list
// EDX: pCount
// EBX: pRace_type_index
void LoadRaces(tRace_list_spec *pRace_list, int *pCount, int pRace_type_index);

// Offset: 29372
// Size: 88
// EAX: pIndex
void UnlockOpponentMugshot(int pIndex);

// Offset: 29460
// Size: 168
// EAX: pIndex
void LoadOpponentMugShot(int pIndex);

// Offset: 29628
// Size: 136
// EAX: pRace_info
// EDX: pIndex
void DisposeOpponentGridIcon(tRace_info *pRace_info, int pIndex);

// Offset: 29764
// Size: 155
// EAX: pRace_info
// EDX: pIndex
void LoadOpponentGridIcon(tRace_info *pRace_info, int pIndex);

// Offset: 29920
// Size: 804
// EAX: pRace_index
// EDX: pRace_info
void LoadRaceInfo(int pRace_index, tRace_info *pRace_info);

// Offset: 30724
// Size: 351
// EAX: pRace_info
void DisposeRaceInfo(tRace_info *pRace_info);

// Offset: 31076
// Size: 122
// EAX: pRace_info
void LoadGridIcons(tRace_info *pRace_info);

// Offset: 31200
// Size: 119
// EAX: pRace_info
void DisposeGridIcons(tRace_info *pRace_info);

// Offset: 31320
// Size: 1075
void LoadOpponents();

// Offset: 32396
// Size: 647
// EAX: pName
br_font* LoadBRFont(char *pName);

// Offset: 33044
// Size: 262
void LoadParts();

// Offset: 33308
// Size: 149
void UnlockParts();

// Offset: 33460
// Size: 72
br_pixelmap* LoadChromeFont();

// Offset: 33532
// Size: 49
// EAX: pThe_font
void DisposeChromeFont(br_pixelmap *pThe_font);

// Offset: 33584
// Size: 140
// EAX: pF
// EDX: pString_list
// EBX: pCount
int GetALineAndInterpretCommand(FILE *pF, char **pString_list, int pCount);

// Offset: 33724
// Size: 101
// EAX: pF
int GetAnInt(FILE *pF);

// Offset: 33828
// Size: 101
// EAX: pF
float GetAFloat(FILE *pF);

// Offset: 33932
// Size: 107
// EAX: pF
float GetAFloatPercent(FILE *pF);

// Offset: 34040
// Size: 131
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfFloats(FILE *pF, float *pF1, float *pF2);

// Offset: 34172
// Size: 169
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeFloats(FILE *pF, float *pF1, float *pF2, float *pF3);

// Offset: 34344
// Size: 131
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfInts(FILE *pF, int *pF1, int *pF2);

// Offset: 34476
// Size: 169
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeInts(FILE *pF, int *pF1, int *pF2, int *pF3);

// Offset: 34648
// Size: 197
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeIntsAndAString(FILE *pF, int *pF1, int *pF2, int *pF3, char *pS);

// Offset: 34848
// Size: 208
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFourInts(FILE *pF, int *pF1, int *pF2, int *pF3, int *pF4);

// Offset: 35056
// Size: 51
// EAX: pF
br_scalar GetAScalar(FILE *pF);

// Offset: 35108
// Size: 69
// EAX: pF
// EDX: pS1
// EBX: pS2
void GetPairOfScalars(FILE *pF, br_scalar *pS1, br_scalar *pS2);

// Offset: 35180
// Size: 81
// EAX: pF
// EDX: pS1
// EBX: pS2
// ECX: pS3
void GetThreeScalars(FILE *pF, br_scalar *pS1, br_scalar *pS2, br_scalar *pS3);

// Offset: 35264
// Size: 240
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFourScalars(FILE *pF, br_scalar *pF1, br_scalar *pF2, br_scalar *pF3, br_scalar *pF4);

// Offset: 35504
// Size: 285
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetFiveScalars(FILE *pF, br_scalar *pF1, br_scalar *pF2, br_scalar *pF3, br_scalar *pF4, br_scalar *pF5);

// Offset: 35792
// Size: 152
// EAX: pF
// EDX: pNumber
// EBX: pScalars
void GetNScalars(FILE *pF, int pNumber, br_scalar *pScalars);

// Offset: 35944
// Size: 157
// EAX: pF
// EDX: pF1
// EBX: pF2
void GetPairOfFloatPercents(FILE *pF, float *pF1, float *pF2);

// Offset: 36104
// Size: 208
// EAX: pF
// EDX: pF1
// EBX: pF2
// ECX: pF3
void GetThreeFloatPercents(FILE *pF, float *pF1, float *pF2, float *pF3);

// Offset: 36312
// Size: 82
// EAX: pF
// EDX: pString
void GetAString(FILE *pF, char *pString);

// Offset: 36396
// Size: 49
void AboutToLoadFirstCar();

// Offset: 36448
// Size: 275
// EAX: pRace_info
void LoadOpponentsCars(tRace_info *pRace_info);

// Offset: 36724
// Size: 184
// EAX: pRace_info
void DisposeOpponentsCars(tRace_info *pRace_info);

// Offset: 36908
// Size: 211
void LoadMiscStrings();

// Offset: 37120
// Size: 53
// EAX: pThe_race
void FillInRaceInfo(tRace_info *pThe_race);

// Offset: 37176
// Size: 1156
// EAX: pFilename
// EDX: pMode
FILE* OldDRfopen(char *pFilename, char *pMode);

// Offset: 38332
// Size: 44
void AllowOpenToFail();

// Offset: 38376
// Size: 34
void DoNotAllowOpenToFail();

// Offset: 38412
// Size: 189
// EAX: pFilename
// EDX: pMode
FILE* DRfopen(char *pFilename, char *pMode);

// Offset: 38604
// Size: 178
// EAX: pPath_name
int GetCDPathFromPathsTxtFile(char *pPath_name);

// Offset: 38784
// Size: 44
int TestForOriginalCarmaCDinDrive();

// Offset: 38828
// Size: 45
int OriginalCarmaCDinDrive();

// Offset: 38876
// Size: 63
int CarmaCDinDriveOrFullGameInstalled();

// Offset: 38940
// Size: 191
// EAX: pF
// EDX: pOptions
void ReadNetworkSettings(FILE *pF, tNet_game_options *pOptions);

// Offset: 39132
// Size: 378
// EAX: pF
// EDX: pIndex
int PrintNetOptions(FILE *pF, int pIndex);

// Offset: 39512
// Size: 1398
int SaveOptions();

// Offset: 40912
// Size: 1643
int RestoreOptions();

// Offset: 42556
// Size: 75
void InitFunkGrooveFlags();

#endif
