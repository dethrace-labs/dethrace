#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern tGot_proc* gGot_procs[34];
extern tLose_proc* gLose_procs[34];
extern tPeriodic_proc* gPeriodic_procs[34];
extern tU32* gReal_render_palette;
extern char* gFizzle_names[3];
extern br_vector3 gZero_v__powerup; // suffix added to avoid duplicate symbol
extern int gPed_harvest_sounds[4];
extern tHeadup_icon gIcon_list[20];
extern br_pixelmap* gFizzle_in[3];
extern int gNumber_of_powerups;
extern int gFizzle_height;
extern int gNumber_of_icons;
extern tPowerup* gPowerup_array;

void LosePowerupX(tPowerup* pThe_powerup, int pTell_net_players);

void LosePowerup(tPowerup* pThe_powerup);

void LoseAllSimilarPowerups(tPowerup* pThe_powerup);

int GotPowerupX(tCar_spec* pCar, int pIndex, int pTell_net_players, int pDisplay_headup, tU32 pTime_left);

int GotPowerup(tCar_spec* pCar, int pIndex);

void LoadPowerups();

void InitPowerups();

void CloseDownPowerUps();

void DrawPowerups(tU32 pTime);

void DoPowerupPeriodics(tU32 pFrame_period);

void GotPowerupN(int pN);

void GotPowerup0();

void GotPowerup1();

void GotPowerup2();

void GotPowerup3();

void GotPowerup4();

void GotPowerup5();

void GotPowerup6();

void GotPowerup7();

void GotPowerup8();

void GotPowerup9();

int GotCredits(tPowerup* pPowerup, tCar_spec* pCar);

void ImprovePSPowerup(tCar_spec* pCar, int pIndex);

int GotTimeOrPower(tPowerup* pPowerup, tCar_spec* pCar);

int SetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar);

int SetHades(tPowerup* pPowerup, tCar_spec* pCar);

void ResetHades(tPowerup* pPowerup, tCar_spec* pCar);

int SetPedSize(tPowerup* pPowerup, tCar_spec* pCar);

int SetPedExplode(tPowerup* pPowerup, tCar_spec* pCar);

int SetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar);

void ResetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar);

int SetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar);

void ResetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar);

int SetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar);

void ResetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar);

int FreezeTimer(tPowerup* pPowerup, tCar_spec* pCar);

void UnfreezeTimer(tPowerup* pPowerup, tCar_spec* pCar);

int DoInstantRepair(tPowerup* pPowerup, tCar_spec* pCar);

void ResetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar);

void ResetPedSize(tPowerup* pPowerup, tCar_spec* pCar);

void ResetPedExplode(tPowerup* pPowerup, tCar_spec* pCar);

int SetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar);

int SetUnderwater(tPowerup* pPowerup, tCar_spec* pCar);

int TrashBodywork(tPowerup* pPowerup, tCar_spec* pCar);

int TakeDrugs(tPowerup* pPowerup, tCar_spec* pCar);

void PaletteFuckedUpByDrugs(br_pixelmap* pPixelmap, int pOffset);

void TheEffectsOfDrugs(tPowerup* pPowerup, tU32 pPeriod);

int SetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar);

int SetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar);

int SetGravity(tPowerup* pPowerup, tCar_spec* pCar);

int SetPinball(tPowerup* pPowerup, tCar_spec* pCar);

int SetWallclimb(tPowerup* pPowerup, tCar_spec* pCar);

int SetBouncey(tPowerup* pPowerup, tCar_spec* pCar);

int SetSuspension(tPowerup* pPowerup, tCar_spec* pCar);

int SetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar);

int SetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar);

void ResetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar);

void ResetUnderwater(tPowerup* pPowerup, tCar_spec* pCar);

void PukeDrugsBackUp(tPowerup* pPowerup, tCar_spec* pCar);

void ResetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar);

void ResetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar);

void ResetGravity(tPowerup* pPowerup, tCar_spec* pCar);

void ResetPinball(tPowerup* pPowerup, tCar_spec* pCar);

void ResetWallclimb(tPowerup* pPowerup, tCar_spec* pCar);

void ResetBouncey(tPowerup* pPowerup, tCar_spec* pCar);

void ResetSuspension(tPowerup* pPowerup, tCar_spec* pCar);

void ResetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar);

void ResetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar);

int PickAtRandom(tPowerup* pPowerup, tCar_spec* pCar);

int PedestrianRespawn(tPowerup* pPowerup, tCar_spec* pCar);

int GotVouchers(tPowerup* pPowerup, tCar_spec* pCar);

void MungeVouchers(tPowerup* pPowerup, tU32 pPeriod);

int SetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar);

void ResetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar);

void DoBouncey(tPowerup* pPowerup, tU32 pPeriod);

int HitMine(tPowerup* pPowerup, tCar_spec* pCar);

int SetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar);

void ResetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar);

int ShowPedestrians(tPowerup* pPowerup, tCar_spec* pCar);

void HidePedestrians(tPowerup* pPowerup, tCar_spec* pCar);

int SetProximity(tPowerup* pPowerup, tCar_spec* pCar);

void ResetProximity(tPowerup* pPowerup, tCar_spec* pCar);

int SetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar);

void ResetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar);

int SetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar);

void ResetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar);

void ReceivedPowerup(tNet_contents* pContents);

void SendCurrentPowerups();

void LoseAllLocalPowerups(tCar_spec* pCar);

#endif
