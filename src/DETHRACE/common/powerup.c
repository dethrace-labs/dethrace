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

// IDA: void __usercall LosePowerupX(tPowerup *pThe_powerup@<EAX>, int pTell_net_players@<EDX>)
void LosePowerupX(tPowerup* pThe_powerup, int pTell_net_players) {
    int i;
    tNet_message* the_message;
    LOG_TRACE("(%p, %d)", pThe_powerup, pTell_net_players);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LosePowerup(tPowerup *pThe_powerup@<EAX>)
void LosePowerup(tPowerup* pThe_powerup) {
    LOG_TRACE("(%p)", pThe_powerup);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoseAllSimilarPowerups(tPowerup *pThe_powerup@<EAX>)
void LoseAllSimilarPowerups(tPowerup* pThe_powerup) {
    int i;
    tPowerup* the_powerup;
    LOG_TRACE("(%p)", pThe_powerup);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GotPowerupX@<EAX>(tCar_spec *pCar@<EAX>, int pIndex@<EDX>, int pTell_net_players@<EBX>, int pDisplay_headup@<ECX>, tU32 pTime_left)
int GotPowerupX(tCar_spec* pCar, int pIndex, int pTell_net_players, int pDisplay_headup, tU32 pTime_left) {
    tPowerup* the_powerup;
    int i;
    int original_index;
    int icon_index;
    int ps_power;
    char s[256];
    char* s2;
    tNet_message* the_message;
    LOG_TRACE("(%p, %d, %d, %d, %d)", pCar, pIndex, pTell_net_players, pDisplay_headup, pTime_left);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GotPowerup@<EAX>(tCar_spec *pCar@<EAX>, int pIndex@<EDX>)
int GotPowerup(tCar_spec* pCar, int pIndex) {
    LOG_TRACE("(%p, %d)", pCar, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LoadPowerups()
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
            time = 1000 * GetAnInt(f);
            the_powerup->duration = time;
            if (time > 0) {
                the_powerup->type = ePowerup_timed;
            } else if (time == 0) {
                the_powerup->type = ePowerup_whole_race;
            } else {
                the_powerup->type = ePowerup_instantaneous;
            }
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
    fclose(f);
}

// IDA: void __cdecl InitPowerups()
void InitPowerups() {
    int i;
    tPowerup* the_powerup;
    LOG_TRACE("()");
    STUB();
}

// IDA: void __cdecl CloseDownPowerUps()
void CloseDownPowerUps() {
    int i;
    tPowerup* the_powerup;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawPowerups(tU32 pTime@<EAX>)
void DrawPowerups(tU32 pTime) {
    int i;
    int y;
    int timer;
    tPowerup* the_powerup;
    char s[8];
    tHeadup_icon* the_icon;
    br_pixelmap* fizzle_pix;
    LOG_TRACE("(%d)", pTime);
    SILENT_STUB();
}

// IDA: void __usercall DoPowerupPeriodics(tU32 pFrame_period@<EAX>)
void DoPowerupPeriodics(tU32 pFrame_period) {
    int i;
    tPowerup* the_powerup;
    tU32 the_time;
    LOG_TRACE("(%d)", pFrame_period);
    SILENT_STUB();
}

// IDA: void __usercall GotPowerupN(int pN@<EAX>)
void GotPowerupN(int pN) {
    int modifiers;
    LOG_TRACE("(%d)", pN);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup0()
void GotPowerup0() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup1()
void GotPowerup1() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup2()
void GotPowerup2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup3()
void GotPowerup3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup4()
void GotPowerup4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup5()
void GotPowerup5() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup6()
void GotPowerup6() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup7()
void GotPowerup7() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup8()
void GotPowerup8() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GotPowerup9()
void GotPowerup9() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GotCredits@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int GotCredits(tPowerup* pPowerup, tCar_spec* pCar) {
    int credits;
    char s[256];
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ImprovePSPowerup(tCar_spec *pCar@<EAX>, int pIndex@<EDX>)
void ImprovePSPowerup(tCar_spec* pCar, int pIndex) {
    tNet_message* the_message;
    LOG_TRACE("(%p, %d)", pCar, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GotTimeOrPower@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int GotTimeOrPower(tPowerup* pPowerup, tCar_spec* pCar) {
    int time;
    int index;
    int i;
    int not_allowed_power;
    char s[256];
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetPedSpeed@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetHades@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetHades(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetPedSize@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    br_scalar old_scale;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetPedExplode@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetInvulnerability@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetInvulnerability(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetFreeRepairs@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetFreeRepairs(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetBlindPedestrians@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetBlindPedestrians(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FreezeTimer@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int FreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UnfreezeTimer(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void UnfreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DoInstantRepair@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int DoInstantRepair(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetPedSpeed(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetPedSize(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetPedExplode(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetEngineFactor@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetUnderwater@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall TrashBodywork@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int TrashBodywork(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    tCar_spec* c;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall TakeDrugs@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int TakeDrugs(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PaletteFuckedUpByDrugs(br_pixelmap *pPixelmap@<EAX>, int pOffset@<EDX>)
void PaletteFuckedUpByDrugs(br_pixelmap* pPixelmap, int pOffset) {
    int i;
    LOG_TRACE("(%p, %d)", pPixelmap, pOffset);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall TheEffectsOfDrugs(tPowerup *pPowerup@<EAX>, tU32 pPeriod@<EDX>)
void TheEffectsOfDrugs(tPowerup* pPowerup, tU32 pPeriod) {
    LOG_TRACE("(%p, %d)", pPowerup, pPeriod);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetOpponentsSpeed@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetCopsSpeed@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetGravity@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetPinball@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetWallclimb@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetBouncey@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetSuspension@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetTyreGrip@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetDamageMultiplier@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetEngineFactor(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetUnderwater(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PukeDrugsBackUp(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void PukeDrugsBackUp(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetOpponentsSpeed(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetCopsSpeed(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetGravity(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetPinball(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetWallclimb(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetBouncey(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetSuspension(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetDamageMultiplier(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetTyreGrip(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PickAtRandom@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int PickAtRandom(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PedestrianRespawn@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int PedestrianRespawn(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GotVouchers@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int GotVouchers(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MungeVouchers(tPowerup *pPowerup@<EAX>, tU32 pPeriod@<EDX>)
void MungeVouchers(tPowerup* pPowerup, tU32 pPeriod) {
    LOG_TRACE("(%p, %d)", pPowerup, pPeriod);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetInstantHandbrake@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetInstantHandbrake(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoBouncey(tPowerup *pPowerup@<EAX>, tU32 pPeriod@<EDX>)
void DoBouncey(tPowerup* pPowerup, tU32 pPeriod) {
    LOG_TRACE("(%p, %d)", pPowerup, pPeriod);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall HitMine@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int HitMine(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    float fudge_multiplier;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetMassMultiplier@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetMassMultiplier(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ShowPedestrians@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int ShowPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall HidePedestrians(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void HidePedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetProximity@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetProximity(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetPedHarvest@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetPedHarvest(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SetVesuvianCorpses@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ResetVesuvianCorpses(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedPowerup(tNet_contents *pContents@<EAX>)
void ReceivedPowerup(tNet_contents* pContents) {
    tPowerup* powerup;
    tCar_spec* car;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SendCurrentPowerups()
void SendCurrentPowerups() {
    int i;
    int cat;
    int j;
    int car_count;
    tCar_spec* car;
    tNet_contents* the_contents;
    tPlayer_ID ID;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoseAllLocalPowerups(tCar_spec *pCar@<EAX>)
void LoseAllLocalPowerups(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}
