#ifndef _INIT_H_
#define _INIT_H_

#include "br_types.h"
#include "dr_types.h"

extern int gInitialisation_finished;
extern tU32 gAustere_time;
extern int gInitial_rank;
extern int gGame_initialized;
extern int gBr_initialized;
extern int gBrZb_initialized;
extern int gRender_indent;
extern int gCredits_per_rank[3];
extern int gInitial_credits[3];
extern int gNet_mode_of_last_game;
extern br_material* gDefault_track_material;

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
