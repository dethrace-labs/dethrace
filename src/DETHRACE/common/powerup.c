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

void LosePowerupX(tPowerup* pThe_powerup, int pTell_net_players) {
    int i;
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

void LosePowerup(tPowerup* pThe_powerup) {
    NOT_IMPLEMENTED();
}

void LoseAllSimilarPowerups(tPowerup* pThe_powerup) {
    int i;
    tPowerup* the_powerup;
    NOT_IMPLEMENTED();
}

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

int GotPowerup(tCar_spec* pCar, int pIndex) {
    NOT_IMPLEMENTED();
}

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

void InitPowerups() {
    tPowerup* the_powerup;
    NOT_IMPLEMENTED();
}

void CloseDownPowerUps() {
    tPowerup* the_powerup;
    NOT_IMPLEMENTED();
}

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

void DoPowerupPeriodics(tU32 pFrame_period) {
    int i;
    tPowerup* the_powerup;
    tU32 the_time;
    NOT_IMPLEMENTED();
}

void GotPowerupN(int pN) {
    int modifiers;
    NOT_IMPLEMENTED();
}

void GotPowerup0() {
    NOT_IMPLEMENTED();
}

void GotPowerup1() {
    NOT_IMPLEMENTED();
}

void GotPowerup2() {
    NOT_IMPLEMENTED();
}

void GotPowerup3() {
    NOT_IMPLEMENTED();
}

void GotPowerup4() {
    NOT_IMPLEMENTED();
}

void GotPowerup5() {
    NOT_IMPLEMENTED();
}

void GotPowerup6() {
    NOT_IMPLEMENTED();
}

void GotPowerup7() {
    NOT_IMPLEMENTED();
}

void GotPowerup8() {
    NOT_IMPLEMENTED();
}

void GotPowerup9() {
    NOT_IMPLEMENTED();
}

int GotCredits(tPowerup* pPowerup, tCar_spec* pCar) {
    int credits;
    char s[256];
    NOT_IMPLEMENTED();
}

void ImprovePSPowerup(tCar_spec* pCar, int pIndex) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

int GotTimeOrPower(tPowerup* pPowerup, tCar_spec* pCar) {
    int time;
    int index;
    int i;
    int not_allowed_power;
    char s[256];
    NOT_IMPLEMENTED();
}

int SetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    br_scalar old_scale;
    NOT_IMPLEMENTED();
}

int SetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int FreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void UnfreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int DoInstantRepair(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int TrashBodywork(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    tCar_spec* c;
    NOT_IMPLEMENTED();
}

int TakeDrugs(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void PaletteFuckedUpByDrugs(br_pixelmap* pPixelmap, int pOffset) {
    int i;
    NOT_IMPLEMENTED();
}

void TheEffectsOfDrugs(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

int SetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

int SetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void PukeDrugsBackUp(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

void ResetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int PickAtRandom(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int PedestrianRespawn(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int GotVouchers(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void MungeVouchers(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

int SetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void DoBouncey(tPowerup* pPowerup, tU32 pPeriod) {
    NOT_IMPLEMENTED();
}

int HitMine(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    float fudge_multiplier;
    NOT_IMPLEMENTED();
}

int SetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int ShowPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void HidePedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

int SetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

int SetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ResetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void ReceivedPowerup(tNet_contents* pContents) {
    tPowerup* powerup;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void SendCurrentPowerups() {
    int cat;
    int j;
    int car_count;
    tCar_spec* car;
    tNet_contents* the_contents;
    tPlayer_ID ID;
    NOT_IMPLEMENTED();
}

void LoseAllLocalPowerups(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}
