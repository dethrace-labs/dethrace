#include "powerup.h"
#include <stdlib.h>

int gPed_harvest_sounds[4];
tGot_proc* gGot_procs[34];
tLose_proc* gLose_procs[34];
tHeadup_icon gIcon_list[20];
tPeriodic_proc* gPeriodic_procs[34];
char* gFizzle_names[3];
br_pixelmap* gFizzle_in[3];
int gNumber_of_powerups;
tU32* gReal_render_palette;
int gFizzle_height;
int gNumber_of_icons;
tPowerup* gPowerup_array;
br_vector3 gZero_v;

// Offset: 0
// Size: 303
// EAX: pThe_powerup
// EDX: pTell_net_players
void LosePowerupX(tPowerup* pThe_powerup, int pTell_net_players) {
    int i;
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 304
// Size: 50
// EAX: pThe_powerup
void LosePowerup(tPowerup* pThe_powerup) {
    NOT_IMPLEMENTED();
}

// Offset: 356
// Size: 139
// EAX: pThe_powerup
void LoseAllSimilarPowerups(tPowerup* pThe_powerup) {
    int i;
    tPowerup* the_powerup;
    NOT_IMPLEMENTED();
}

// Offset: 496
// Size: 1063
// EAX: pCar
// EDX: pIndex
// EBX: pTell_net_players
// ECX: pDisplay_headup
int GotPowerupX(tCar_spec* pCar, int pIndex, int pTell_net_players, int pDisplay_headup, tU32 pTime_left) {
    tPowerup* the_powerup;
    int i;
    int original_index;
    int icon_index;
    int ps_power;
    char s[256];
    char* s2;
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 1560
// Size: 67
// EAX: pCar
// EDX: pIndex
int GotPowerup(tCar_spec* pCar, int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 1628
// Size: 811
void LoadPowerups() {
    tPath_name the_path;
    char s[256];
    int i;
    int j;
    int time;
    int action_index;
    tPowerup* the_powerup;
    NOT_IMPLEMENTED();
}

// Offset: 2440
// Size: 112
void InitPowerups() {
    tPowerup* the_powerup;
    NOT_IMPLEMENTED();
}

// Offset: 2552
// Size: 99
void CloseDownPowerUps() {
    tPowerup* the_powerup;
    NOT_IMPLEMENTED();
}

// Offset: 2652
// Size: 701
// EAX: pTime
void DrawPowerups(tU32 pTime) {
    int i;
    int y;
    int timer;
    tPowerup* the_powerup;
    char s[8];
    tHeadup_icon* the_icon;
    br_pixelmap* fizzle_pix;
    NOT_IMPLEMENTED();
}

// Offset: 3356
// Size: 176
// EAX: pFrame_period
void DoPowerupPeriodics(tU32 pFrame_period) {
    int i;
    tPowerup* the_powerup;
    tU32 the_time;
    NOT_IMPLEMENTED();
}

// Offset: 3532
// Size: 158
// EAX: pN
void GotPowerupN(int pN) {
    int modifiers;
    NOT_IMPLEMENTED();
}

// Offset: 3692
// Size: 41
void GotPowerup0() {
    NOT_IMPLEMENTED();
}

// Offset: 3736
// Size: 44
void GotPowerup1() {
    NOT_IMPLEMENTED();
}

// Offset: 3780
// Size: 44
void GotPowerup2() {
    NOT_IMPLEMENTED();
}

// Offset: 3824
// Size: 44
void GotPowerup3() {
    NOT_IMPLEMENTED();
}

// Offset: 3868
// Size: 44
void GotPowerup4() {
    NOT_IMPLEMENTED();
}

// Offset: 3912
// Size: 44
void GotPowerup5() {
    NOT_IMPLEMENTED();
}

// Offset: 3956
// Size: 44
void GotPowerup6() {
    NOT_IMPLEMENTED();
}

// Offset: 4000
// Size: 44
void GotPowerup7() {
    NOT_IMPLEMENTED();
}

// Offset: 4044
// Size: 44
void GotPowerup8() {
    NOT_IMPLEMENTED();
}

// Offset: 4088
// Size: 44
void GotPowerup9() {
    NOT_IMPLEMENTED();
}

// Offset: 4132
// Size: 163
// EAX: pPowerup
// EDX: pCar
int GotCredits(tPowerup* pPowerup, tCar_spec* pCar) {
    int credits;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 4296
// Size: 89
// EAX: pCar
// EDX: pIndex
void ImprovePSPowerup(tCar_spec* pCar, int pIndex) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 4388
// Size: 397
// EAX: pPowerup
// EDX: pCar
int GotTimeOrPower(tPowerup* pPowerup, tCar_spec* pCar) {
    int time;
    int index;
    int i;
    int not_allowed_power;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 4788
// Size: 79
// EAX: pPowerup
// EDX: pCar
int SetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 4868
// Size: 70
// EAX: pPowerup
// EDX: pCar
int SetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 4940
// Size: 43
// EAX: pPowerup
// EDX: pCar
void ResetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 4984
// Size: 187
// EAX: pPowerup
// EDX: pCar
int SetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    br_scalar old_scale;
    NOT_IMPLEMENTED();
}

// Offset: 5172
// Size: 75
// EAX: pPowerup
// EDX: pCar
int SetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5248
// Size: 78
// EAX: pPowerup
// EDX: pCar
int SetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5328
// Size: 51
// EAX: pPowerup
// EDX: pCar
void ResetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5380
// Size: 84
// EAX: pPowerup
// EDX: pCar
int SetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5464
// Size: 57
// EAX: pPowerup
// EDX: pCar
void ResetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5524
// Size: 75
// EAX: pPowerup
// EDX: pCar
int SetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5600
// Size: 48
// EAX: pPowerup
// EDX: pCar
void ResetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5648
// Size: 107
// EAX: pPowerup
// EDX: pCar
int FreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5756
// Size: 57
// EAX: pPowerup
// EDX: pCar
void UnfreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5816
// Size: 79
// EAX: pPowerup
// EDX: pCar
int DoInstantRepair(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5896
// Size: 48
// EAX: pPowerup
// EDX: pCar
void ResetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5944
// Size: 58
// EAX: pPowerup
// EDX: pCar
void ResetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6004
// Size: 48
// EAX: pPowerup
// EDX: pCar
void ResetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6052
// Size: 153
// EAX: pPowerup
// EDX: pCar
int SetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6208
// Size: 78
// EAX: pPowerup
// EDX: pCar
int SetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6288
// Size: 256
// EAX: pPowerup
// EDX: pCar
int TrashBodywork(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    tCar_spec* c;
    NOT_IMPLEMENTED();
}

// Offset: 6544
// Size: 141
// EAX: pPowerup
// EDX: pCar
int TakeDrugs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6688
// Size: 195
// EAX: pPixelmap
// EDX: pOffset
void PaletteFuckedUpByDrugs(br_pixelmap* pPixelmap, int pOffset) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 6884
// Size: 72
// EAX: pPowerup
// EDX: pPeriod
void TheEffectsOfDrugs(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

// Offset: 6956
// Size: 162
// EAX: pPowerup
// EDX: pCar
int SetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7120
// Size: 79
// EAX: pPowerup
// EDX: pCar
int SetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7200
// Size: 79
// EAX: pPowerup
// EDX: pCar
int SetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7280
// Size: 79
// EAX: pPowerup
// EDX: pCar
int SetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7360
// Size: 78
// EAX: pPowerup
// EDX: pCar
int SetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7440
// Size: 119
// EAX: pPowerup
// EDX: pCar
int SetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7560
// Size: 117
// EAX: pPowerup
// EDX: pCar
int SetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7680
// Size: 82
// EAX: pPowerup
// EDX: pCar
int SetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7764
// Size: 82
// EAX: pPowerup
// EDX: pCar
int SetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7848
// Size: 91
// EAX: pPowerup
// EDX: pCar
void ResetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7940
// Size: 51
// EAX: pPowerup
// EDX: pCar
void ResetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7992
// Size: 74
// EAX: pPowerup
// EDX: pCar
void PukeDrugsBackUp(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8068
// Size: 131
// EAX: pPowerup
// EDX: pCar
void ResetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 8200
// Size: 48
// EAX: pPowerup
// EDX: pCar
void ResetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8248
// Size: 50
// EAX: pPowerup
// EDX: pCar
void ResetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8300
// Size: 48
// EAX: pPowerup
// EDX: pCar
void ResetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8348
// Size: 51
// EAX: pPowerup
// EDX: pCar
void ResetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8400
// Size: 64
// EAX: pPowerup
// EDX: pCar
void ResetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8464
// Size: 65
// EAX: pPowerup
// EDX: pCar
void ResetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8532
// Size: 51
// EAX: pPowerup
// EDX: pCar
void ResetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8584
// Size: 51
// EAX: pPowerup
// EDX: pCar
void ResetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8636
// Size: 83
// EAX: pPowerup
// EDX: pCar
int PickAtRandom(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8720
// Size: 70
// EAX: pPowerup
// EDX: pCar
int PedestrianRespawn(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8792
// Size: 87
// EAX: pPowerup
// EDX: pCar
int GotVouchers(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8880
// Size: 49
// EAX: pPowerup
// EDX: pPeriod
void MungeVouchers(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

// Offset: 8932
// Size: 84
// EAX: pPowerup
// EDX: pCar
int SetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9016
// Size: 57
// EAX: pPowerup
// EDX: pCar
void ResetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9076
// Size: 137
// EAX: pPowerup
// EDX: pPeriod
void DoBouncey(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

// Offset: 9216
// Size: 450
// EAX: pPowerup
// EDX: pCar
int HitMine(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    float fudge_multiplier;
    NOT_IMPLEMENTED();
}

// Offset: 9668
// Size: 82
// EAX: pPowerup
// EDX: pCar
int SetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9752
// Size: 51
// EAX: pPowerup
// EDX: pCar
void ResetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9804
// Size: 84
// EAX: pPowerup
// EDX: pCar
int ShowPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9888
// Size: 57
// EAX: pPowerup
// EDX: pCar
void HidePedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9948
// Size: 90
// EAX: pPowerup
// EDX: pCar
int SetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10040
// Size: 51
// EAX: pPowerup
// EDX: pCar
void ResetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10092
// Size: 75
// EAX: pPowerup
// EDX: pCar
int SetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10168
// Size: 120
// EAX: pPowerup
// EDX: pCar
void ResetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 10288
// Size: 75
// EAX: pPowerup
// EDX: pCar
int SetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10364
// Size: 48
// EAX: pPowerup
// EDX: pCar
void ResetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10412
// Size: 423
// EAX: pContents
void ReceivedPowerup(tNet_contents* pContents) {
    tPowerup* powerup;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

// Offset: 10836
// Size: 354
void SendCurrentPowerups() {
    int cat;
    int j;
    int car_count;
    tCar_spec* car;
    tNet_contents* the_contents;
    tPlayer_ID ID;
    NOT_IMPLEMENTED();
}

// Offset: 11192
// Size: 137
// EAX: pCar
void LoseAllLocalPowerups(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}
