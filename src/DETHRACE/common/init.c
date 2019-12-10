#include "init.h"

#include <time.h>

#include "pc-dos/dossys.h"
#include "common/globvars.h"
#include "common/errors.h"
#include "common/drmem.h"
#include "common/graphics.h"
#include "common/drdebug.h"
#include "common/loading.h"
#include "common/netgame.h"
#include "common/replay.h"
#include "common/grafdata.h"
#include "common/flicplay.h"
#include "common/sound.h"
#include "common/displays.h"
#include "common/powerup.h"
#include "common/raycast.h"
#include "common/depth.h"
#include "common/world.h"
#include "common/oil.h"
#include "common/skidmark.h"
#include "common/pedestrn.h"

#include "brender.h"

int gInitialisation_finished;
tU32 gAustere_time;
int gInitial_rank;
int gGame_initialized;
int gBr_initialized;
int gBrZb_initialized;
int gRender_indent;
int gCredits_per_rank[3];
int gInitial_credits[3];
int gNet_mode_of_last_game;
br_material *gDefault_track_material;

// Offset: 0
// Size: 115
void AllocateSelf() {
}

// Offset: 116
// Size: 514
void AllocateCamera() {
    int i;
}

// Offset: 632
// Size: 582
void ReinitialiseForwardCamera() {
    float the_angle;
    float d;
    float w;
}

// Offset: 1216
// Size: 217
void AllocateRearviewPixelmap() {
}

// Offset: 1436
// Size: 169
void ReinitialiseRearviewCamera() {
}

// Offset: 1608
// Size: 347
void ReinitialiseRenderStuff() {
    int y_diff;
}

// Offset: 1956
// Size: 34
void InstallFindFailedHooks() {
}

// Offset: 1992
// Size: 158
void AllocateStandardLamp() {
    br_actor *lamp;
    int i;
}

// Offset: 2152
// Size: 342
void InitializeBRenderEnvironment() {
}

// Offset: 2496
// Size: 79
void InitBRFonts() {
}

// Offset: 2576
// Size: 196
void AustereWarning() {
}

// Offset: 2772
// Size: 309
void InitLineStuff() {
}

// Offset: 3084
// Size: 896
void InitSmokeStuff() {
    tPath_name path;
}

// Offset: 3980
// Size: 1350
void Init2DStuff() {
    static br_token_value fadealpha[3];
    tPath_name path;
    br_scalar prat_u;
    br_scalar prat_v;
}

// Offset: 5332
// Size: 671
// EAX: pArgc
// EDX: pArgv
void InitialiseApplication(int pArgc, char **pArgv) {
  gProgram_state.sausage_eater_mode = gSausage_override;
  DrDebugLog(gSausage_override, *pArgv);
  if (gAustere_override || PDDoWeLeadAnAustereExistance() != 0) {
    gAusterity_mode = 1;
  }
  
  srand(time(NULL));
  BrV1dbBeginWrapper_Float();
  CreateStainlessClasses();
  InitWobbleStuff();
  LoadGeneralParameters();
  DefaultNetName();
  strcpy(gProgram_state.player_name[0], "MAX DAMAGE"); 
  strcpy(gProgram_state.player_name[0], "DIE ANNA"); 
  
  RestoreOptions();
  LoadKeyMapping();
  if (!PDInitScreenVars(pArgc, pArgv)) {
    FatalError(0);
  }
  CalcGrafDataIndex();
//   v26 = locret_A1004();
//   v27 = ((int (__fastcall *)(int))locret_A1040)(v26);
  InitializeBRenderEnvironment();
  InitDRFonts();
  InitBRFonts();
  LoadMiscStrings();
  LoadInRegistees();
  FinishLoadingGeneral();
  InitializePalettes();
  AustereWarning();
  LoadInterfaceStrings();
  InitializeActionReplay();
  FlicPaletteAllocate();
  InitInterfaceLoadState();
  InitTransientBitmaps();
  InitSound();
  InitHeadups();
  gDefault_track_material = BrMaterialAllocate("gDefault_track_material");
  //BYTE2(gDefault_track_material->map_transform.m[2][1]) = -29;
  //BYTE3(gDefault_track_material->map_transform.m[2][1]) = 1;
  BrMaterialAdd(gDefault_track_material);
  InitShadow();
  InitFlics();
  AllocateStandardLamp();
  InitAmbience();
  LoadOpponents();
  LoadPowerups();
  LoadRaces(gRace_list, &gNumber_of_races, -1);
  RevertPalette();
  InitRayCasting();
  InitDepthEffects();
  InitialiseStorageSpace(&gOur_car_storage_space, 40, 2, 40, 30);
  InitialiseStorageSpace(&gTheir_cars_storage_space, 300, 50, 500, 200);
  InitialiseStorageSpace(&gPedestrians_storage_space, 500, 10, 0, 0);
  InitialiseStorageSpace(&gTrack_storage_space, 400, 50, 400, 1000);
  InitOilSpills();
  if (gAustere_time) {
      while (PDGetTotalTime() - gAustere_time < 2000) {
      }
  }
  ClearEntireScreen();
  InitSkids();
  InitPeds();
  gProgram_state.cars_available[42] = 0;
  gCD_is_in_drive = TestForOriginalCarmaCDinDrive();
  SwitchToLoresMode();
  DrDebugLog(0, "AFTER APPLICATION INITIALISATION");
}

// Offset: 6004
// Size: 64
// EAX: pArgc
// EDX: pArgv
void InitialiseDeathRace(int pArgc, char **pArgv) {
    PDInitialiseSystem();
    InitialiseApplication(pArgc, pArgv);
    //dword_112DF8 = 1;  // never checked by game
}

// Offset: 6068
// Size: 427
// EAX: pStart_race
void InitGame(int pStart_race) {
    int i;
}

// Offset: 6496
// Size: 191
void DisposeGameIfNecessary() {
}

// Offset: 6688
// Size: 54
void LoadInTrack() {
}

// Offset: 6744
// Size: 44
void DisposeTrack() {
}

// Offset: 6788
// Size: 97
// EAX: pMaterial
void CopyMaterialColourFromIndex(br_material *pMaterial) {
}

// Offset: 6888
// Size: 1230
void InitRace() {
}

// Offset: 8120
// Size: 208
void DisposeRace() {
}

// Offset: 8328
// Size: 45
int GetScreenSize() {
}

// Offset: 8376
// Size: 45
// EAX: pNew_size
void SetScreenSize(int pNew_size) {
}

