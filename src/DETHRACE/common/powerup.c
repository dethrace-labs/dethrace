#include "powerup.h"
#include "brender.h"
#include "common/errors.h"
#include "common/globvars.h"
#include "common/loading.h"
#include "common/utility.h"
#include <stdlib.h>

int gPed_harvest_sounds[4];
tGot_proc* gGot_procs[34];
tLose_proc* gLose_procs[34];
tHeadup_icon gIcon_list[20];
tPeriodic_proc* gPeriodic_procs[34];
char* gFizzle_names[3] = { "CIRCLES.PIX", "SQUARES.PIX", "DIAMONDS.PIX" };
br_pixelmap* gFizzle_in[3];
int gNumber_of_powerups;
tU32* gReal_render_palette;
int gFizzle_height;
int gNumber_of_icons;
tPowerup* gPowerup_array;
br_vector3 gZero_v_powerup; // added _powerup suffix to avoid name collision

// Offset: 0
// Size: 303
void LosePowerupX(tPowerup* pThe_powerup, int pTell_net_players) {
    int i;
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 304
// Size: 50
void LosePowerup(tPowerup* pThe_powerup) {
    NOT_IMPLEMENTED();
}

// Offset: 356
// Size: 139
void LoseAllSimilarPowerups(tPowerup* pThe_powerup) {
    int i;
    tPowerup* the_powerup;
    NOT_IMPLEMENTED();
}

// Offset: 496
// Size: 1063
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
int GotPowerup(tCar_spec* pCar, int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 1628
// Size: 811
void LoadPowerups() {
    FILE* f;
    tPath_name the_path;
    char s[256];
    int i;
    int j;
    int time;
    int action_index;
    tPowerup* the_powerup;
    LOG_TRACE("()");

    for (i = 0; i < 3; i++) {
        gFizzle_in[i] = LoadPixelmap(gFizzle_names[i]);
    }

    gFizzle_height = gFizzle_in[0]->height / 4;
    PathCat(the_path, gApplication_path, "POWERUP.TXT");
    f = DRfopen(the_path, "rt");
    if (!f) {
        FatalError(25);
    }
    gNumber_of_powerups = GetAnInt(f);
    gPowerup_array = BrMemAllocate(sizeof(tPowerup) * gNumber_of_powerups, kMem_powerup_array);
    for (i = 0; i < gNumber_of_powerups; i++) {
        the_powerup = &gPowerup_array[i];

        GetALineAndDontArgue(f, the_powerup->message);
        if (strcmp(the_powerup->message, "dummy") == 0) {
            the_powerup->type = 0;
        } else {
            if (strcmp(the_powerup->message, "n/a") == 0) {
                the_powerup->message[0] = 0;
            }
            GetAString(f, s);
            the_powerup->icon = LoadPixelmap(s);
            the_powerup->fizzle_type = GetAnInt(f);
            the_powerup->duration = 1000 * GetAnInt(f);
            if (the_powerup->duration < 0) {
                the_powerup->type = ePowerup_instantaneous;
            } else if (the_powerup->duration == 0) {
                the_powerup->type = ePowerup_whole_race;
            } else {
                the_powerup->type = ePowerup_timed;
            }
            action_index = GetAnInt(f);
            if (action_index >= 0) {
                the_powerup->got_proc = gGot_procs[action_index];
                the_powerup->lose_proc = gLose_procs[action_index];
                the_powerup->periodic_proc = gPeriodic_procs[action_index];
            } else {
                the_powerup->lose_proc = NULL;
                the_powerup->periodic_proc = NULL;
                the_powerup->got_proc = NULL;
            }
            the_powerup->number_of_float_params = GetAnInt(f);
            the_powerup->float_params = BrMemAllocate(4 * the_powerup->number_of_float_params, kMem_powerup_float_parms);
            for (j = 0; j < the_powerup->number_of_float_params; j++) {
                the_powerup->float_params[j] = GetAFloat(f);
            }
            the_powerup->number_of_integer_params = GetAnInt(f);
            the_powerup->integer_params = BrMemAllocate(4 * the_powerup->number_of_integer_params, kMem_powerup_int_parms);
            for (j = 0; j < the_powerup->number_of_integer_params; j++) {
                the_powerup->integer_params[j] = GetAnInt(f);
            }
            the_powerup->group_inclusion = GetAnInt(f);
            the_powerup->prat_cam_event = GetAnInt(f);
            the_powerup->net_type = GetAnInt(f);
        }
    }
    fclose(f);
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
void DoPowerupPeriodics(tU32 pFrame_period) {
    int i;
    tPowerup* the_powerup;
    tU32 the_time;
    NOT_IMPLEMENTED();
}

// Offset: 3532
// Size: 158
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
int GotCredits(tPowerup* pPowerup, tCar_spec* pCar) {
    int credits;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 4296
// Size: 89
void ImprovePSPowerup(tCar_spec* pCar, int pIndex) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 4388
// Size: 397
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
int SetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 4868
// Size: 70
int SetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 4940
// Size: 43
void ResetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 4984
// Size: 187
int SetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    br_scalar old_scale;
    NOT_IMPLEMENTED();
}

// Offset: 5172
// Size: 75
int SetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5248
// Size: 78
int SetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5328
// Size: 51
void ResetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5380
// Size: 84
int SetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5464
// Size: 57
void ResetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5524
// Size: 75
int SetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5600
// Size: 48
void ResetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5648
// Size: 107
int FreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5756
// Size: 57
void UnfreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5816
// Size: 79
int DoInstantRepair(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5896
// Size: 48
void ResetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5944
// Size: 58
void ResetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6004
// Size: 48
void ResetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6052
// Size: 153
int SetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6208
// Size: 78
int SetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6288
// Size: 256
int TrashBodywork(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    tCar_spec* c;
    NOT_IMPLEMENTED();
}

// Offset: 6544
// Size: 141
int TakeDrugs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6688
// Size: 195
void PaletteFuckedUpByDrugs(br_pixelmap* pPixelmap, int pOffset) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 6884
// Size: 72
void TheEffectsOfDrugs(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

// Offset: 6956
// Size: 162
int SetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7120
// Size: 79
int SetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7200
// Size: 79
int SetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7280
// Size: 79
int SetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7360
// Size: 78
int SetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7440
// Size: 119
int SetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7560
// Size: 117
int SetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7680
// Size: 82
int SetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7764
// Size: 82
int SetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7848
// Size: 91
void ResetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7940
// Size: 51
void ResetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7992
// Size: 74
void PukeDrugsBackUp(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8068
// Size: 131
void ResetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 8200
// Size: 48
void ResetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8248
// Size: 50
void ResetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8300
// Size: 48
void ResetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8348
// Size: 51
void ResetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8400
// Size: 64
void ResetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8464
// Size: 65
void ResetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8532
// Size: 51
void ResetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8584
// Size: 51
void ResetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8636
// Size: 83
int PickAtRandom(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8720
// Size: 70
int PedestrianRespawn(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8792
// Size: 87
int GotVouchers(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8880
// Size: 49
void MungeVouchers(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

// Offset: 8932
// Size: 84
int SetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9016
// Size: 57
void ResetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9076
// Size: 137
void DoBouncey(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

// Offset: 9216
// Size: 450
int HitMine(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    float fudge_multiplier;
    NOT_IMPLEMENTED();
}

// Offset: 9668
// Size: 82
int SetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9752
// Size: 51
void ResetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9804
// Size: 84
int ShowPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9888
// Size: 57
void HidePedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 9948
// Size: 90
int SetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10040
// Size: 51
void ResetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10092
// Size: 75
int SetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10168
// Size: 120
void ResetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 10288
// Size: 75
int SetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10364
// Size: 48
void ResetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 10412
// Size: 423
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
void LoseAllLocalPowerups(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}
