#ifndef _INIT_H_
#define _INIT_H_

#include "br_types.h"
#include "dr_types.h"

extern int gCredits_per_rank[3];
extern int gInitial_credits[3];
extern int gInitial_rank;
extern int gBrZb_initialized;

void AllocateSelf();

void AllocateCamera();

void ReinitialiseForwardCamera();

void AllocateRearviewPixelmap();

void ReinitialiseRearviewCamera();

void ReinitialiseRenderStuff();

void InstallFindFailedHooks();

void AllocateStandardLamp();

void InitializeBRenderEnvironment();

void InitBRFonts();

void AustereWarning();

void InitLineStuff();

void InitSmokeStuff();

void Init2DStuff();

void InitialiseApplication(int pArgc, char** pArgv);

void InitialiseDeathRace(int pArgc, char** pArgv);

void InitGame(int pStart_race);

void DisposeGameIfNecessary();

void LoadInTrack();

void DisposeTrack();

void CopyMaterialColourFromIndex(br_material* pMaterial);

void InitRace();

void DisposeRace();

int GetScreenSize();

void SetScreenSize(int pNew_size);

#endif
