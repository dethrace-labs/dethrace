#ifndef _INIT_H_
#define _INIT_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern int gGame_initialized;
extern int gBr_initialized;
extern int gBrZb_initialized;
extern int gInitialisation_finished;
extern int gRender_indent;
extern tU32 gAustere_time;
extern int gInitial_rank;
extern int gCredits_per_rank[3];
extern int gInitial_credits[3];
extern int gNet_mode_of_last_game;
extern br_material* gDefault_track_material;

void AllocateSelf(void);

void AllocateCamera(void);

void ReinitialiseForwardCamera(void);

void AllocateRearviewPixelmap(void);

void ReinitialiseRearviewCamera(void);

void ReinitialiseRenderStuff(void);

void InstallFindFailedHooks(void);

void AllocateStandardLamp(void);

void InitializeBRenderEnvironment(void);

void InitBRFonts(void);

void AustereWarning(void);

void InitLineStuff(void);

void InitSmokeStuff(void);

void Init2DStuff(void);

void InitialiseApplication(int pArgc, char** pArgv);

void InitialiseDeathRace(int pArgc, char** pArgv);

void InitGame(int pStart_race);

void DisposeGameIfNecessary(void);

void LoadInTrack(void);

void DisposeTrack(void);

void CopyMaterialColourFromIndex(br_material* pMaterial);

void InitRace(void);

void DisposeRace(void);

int GetScreenSize(void);

void SetScreenSize(int pNew_size);

#endif
