#ifndef _INIT_H_
#define _INIT_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 115
void AllocateSelf();

// Offset: 116
// Size: 514
void AllocateCamera();

// Offset: 632
// Size: 582
void ReinitialiseForwardCamera();

// Offset: 1216
// Size: 217
void AllocateRearviewPixelmap();

// Offset: 1436
// Size: 169
void ReinitialiseRearviewCamera();

// Offset: 1608
// Size: 347
void ReinitialiseRenderStuff();

// Offset: 1956
// Size: 34
void InstallFindFailedHooks();

// Offset: 1992
// Size: 158
void AllocateStandardLamp();

// Offset: 2152
// Size: 342
void InitializeBRenderEnvironment();

// Offset: 2496
// Size: 79
void InitBRFonts();

// Offset: 2576
// Size: 196
void AustereWarning();

// Offset: 2772
// Size: 309
void InitLineStuff();

// Offset: 3084
// Size: 896
void InitSmokeStuff();

// Offset: 3980
// Size: 1350
void Init2DStuff();

// Offset: 5332
// Size: 671
// EAX: pArgc
// EDX: pArgv
void InitialiseApplication(int pArgc, char** pArgv);

// Offset: 6004
// Size: 64
// EAX: pArgc
// EDX: pArgv
void InitialiseDeathRace(int pArgc, char** pArgv);

// Offset: 6068
// Size: 427
// EAX: pStart_race
void InitGame(int pStart_race);

// Offset: 6496
// Size: 191
void DisposeGameIfNecessary();

// Offset: 6688
// Size: 54
void LoadInTrack();

// Offset: 6744
// Size: 44
void DisposeTrack();

// Offset: 6788
// Size: 97
// EAX: pMaterial
void CopyMaterialColourFromIndex(br_material* pMaterial);

// Offset: 6888
// Size: 1230
void InitRace();

// Offset: 8120
// Size: 208
void DisposeRace();

// Offset: 8328
// Size: 45
int GetScreenSize();

// Offset: 8376
// Size: 45
// EAX: pNew_size
void SetScreenSize(int pNew_size);

#endif
