#include "controls.h"

#include "car.h"
#include "displays.h"
#include "flicplay.h"
#include "globvars.h"
#include "graphics.h"
#include "input.h"
#include "loadsave.h"
#include "mainloop.h"
#include "netgame.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "pratcam.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"
#include <stdlib.h>

tCheat gKev_keys[44] = {
    { .code = 0xA11EE75D, .code2 = 0xF805EDDD, .action_proc = SetFlag, .num = 0x0A11EE75D },
    { .code = 0x564E78B9, .code2 = 0x99155115, .action_proc = SetFlag, .num = 0x564E78B9 },
    { .code = 0x1F47E5E8, .code2 = 0xA715222C, .action_proc = SetFlag2, .num = 1 },
    { .code = 0x39D4C4C4, .code2 = 0xF0A2C5B0, .action_proc = FinishLap, .num = 0 },
    { .code = 0x2654216C, .code2 = 0xF8256D15, .action_proc = GetPowerup, .num = 1 },
    { .code = 0x4294EC89, .code2 = 0xC38AD60E, .action_proc = GetPowerup, .num = 2 },
    { .code = 0x2CEB2850, .code2 = 0xA0C2D27E, .action_proc = GetPowerup, .num = 3 },
    { .code = 0x2D5F3125, .code2 = 0x9CE8A541, .action_proc = GetPowerup, .num = 4 },
    { .code = 0x2169C78B, .code2 = 0x7F3C9229, .action_proc = GetPowerup, .num = 5 },
    { .code = 0x2203C0CB, .code2 = 0x79729BE4, .action_proc = GetPowerup, .num = 6 },
    { .code = 0x34F4E3EC, .code2 = 0x868C534D, .action_proc = GetPowerup, .num = 7 },
    { .code = 0x34010316, .code2 = 0x591D1EB2, .action_proc = GetPowerup, .num = 8 },
    { .code = 0x214FE3BC, .code2 = 0x87285111, .action_proc = GetPowerup, .num = 9 },
    { .code = 0x2FABC390, .code2 = 0xC93D9F7, .action_proc = GetPowerup, .num = 10 },
    { .code = 0x2902E890, .code2 = 0x40969F67, .action_proc = GetPowerup, .num = 11 },
    { .code = 0x2F458288, .code2 = 0x58074E2, .action_proc = GetPowerup, .num = 12 },
    { .code = 0x249DA152, .code2 = 0x8F287346, .action_proc = GetPowerup, .num = 13 },
    { .code = 0x23AAE08B, .code2 = 0xA668103D, .action_proc = GetPowerup, .num = 14 },
    { .code = 0x32130661, .code2 = 0x56F03459, .action_proc = GetPowerup, .num = 15 },
    { .code = 0x2734E944, .code2 = 0xFE1E4639, .action_proc = GetPowerup, .num = 16 },
    { .code = 0x28341139, .code2 = 0x355F6D02, .action_proc = GetPowerup, .num = 17 },
    { .code = 0x20508831, .code2 = 0x123D1961, .action_proc = GetPowerup, .num = 18 },
    { .code = 0x346B8BCB, .code2 = 0x4ABA696C, .action_proc = GetPowerup, .num = 19 },
    { .code = 0x3FC93DF0, .code2 = 0x29FA9EFB, .action_proc = GetPowerup, .num = 20 },
    { .code = 0x2A80B09B, .code2 = 0x58516F5, .action_proc = GetPowerup, .num = 21 },
    { .code = 0x2F548FD1, .code2 = 0x696744DA, .action_proc = GetPowerup, .num = 22 },
    { .code = 0x3CB74F32, .code2 = 0xB915D88D, .action_proc = GetPowerup, .num = 23 },
    { .code = 0x297B53BA, .code2 = 0x218D4D2D, .action_proc = GetPowerup, .num = 24 },
    { .code = 0x351BC37D, .code2 = 0xB2A63343, .action_proc = GetPowerup, .num = 25 },
    { .code = 0x2B624386, .code2 = 0x9BA6260E, .action_proc = GetPowerup, .num = 26 },
    { .code = 0x2BA4AE23, .code2 = 0xC163A76C, .action_proc = GetPowerup, .num = 27 },
    { .code = 0x2FB92DCA, .code2 = 0x4AD7D54E, .action_proc = GetPowerup, .num = 32 },
    { .code = 0x3A42191B, .code2 = 0xEFF70F4C, .action_proc = GetPowerup, .num = 35 },
    { .code = 0x2ACA3190, .code2 = 0xD9004F25, .action_proc = GetPowerup, .num = 36 },
    { .code = 0x37C1F613, .code2 = 0xB7FAF351, .action_proc = GetPowerup, .num = 37 },
    { .code = 0x2BA3F603, .code2 = 0x29F2425C, .action_proc = GetPowerup, .num = 38 },
    { .code = 0x416EFF61, .code2 = 0x2667DF4B, .action_proc = GetPowerup, .num = 39 },
    { .code = 0x2554125C, .code2 = 0x393CA35D, .action_proc = GetPowerup, .num = 41 },
    { .code = 0x3FFF84D5, .code2 = 0x84A42DF4, .action_proc = GetPowerup, .num = 41 },
    { .code = 0x37E83018, .code2 = 0xB609AEE6, .action_proc = GetPowerup, .num = 42 },
    { .code = 0x2DB03B19, .code2 = 0x924A84B7, .action_proc = GetPowerup, .num = 43 },
    { .code = 0x30A19FAB, .code2 = 0x2B0C2782, .action_proc = GetPowerup, .num = 44 },
    { .code = 0x0, .code2 = 0x0, .action_proc = 0x0, .num = 0x0 }
};
char* gAbuse_text[10];
tEdit_func* gEdit_funcs[10][18][8];
char* gEdit_mode_names[10];
tToggle_element _gToggle_array[] = {
    { 34, -2, 1, 1, 0, ConcussMe },
    { 56, -2, 1, 1, 0, ToggleMap },
    { 35, -2, 1, 1, 0, TogglePratcam },
    { 59, -2, 1, 1, 0, SetRecovery },
    { 4, 7, 1, 1, 0, AbortRace },
    { 45, -2, 1, 1, 0, ToggleMirror },
    { 50, -2, 1, 1, 0, LookLeft },
    { 51, -2, 1, 1, 0, LookRight },
    { 52, -2, 1, 1, 0, DamageTest },
    { 36, -2, 0, 1, 0, ToggleSoundEnable },
    { 5, 8, 0, 1, 0, PrintScreen },
    //{ 9, 7, 1, 1, 0, locret_1EF00 },  //DrawSomeText
    { 10, 7, 1, 1, 0, ToggleFlying },
    { 54, -2, 1, 1, 0, TogglePedestrians },
    { 17, -2, 0, 0, 0, F4Key },
    { 18, -2, 1, 0, 0, F5Key },
    { 19, -2, 1, 0, 0, F6Key },
    { 20, -2, 1, 0, 0, F7Key },
    { 21, -2, 1, 0, 0, F8Key },
    { 22, -2, 1, 0, 0, F10Key },
    { 23, -2, 1, 0, 0, F11Key },
    { 24, -2, 1, 0, 0, F12Key },
    { 14, -2, 1, 0, 0, NumberKey0 },
    { 37, -2, 1, 0, 0, NumberKey1 },
    { 38, -2, 1, 0, 0, NumberKey2 },
    { 39, -2, 1, 0, 0, NumberKey3 },
    { 40, -2, 1, 0, 0, NumberKey4 },
    { 41, -2, 1, 0, 0, NumberKey5 },
    { 42, -2, 1, 0, 0, NumberKey6 },
    { 43, -2, 1, 0, 0, NumberKey7 },
    { 15, -2, 1, 0, 0, NumberKey8 },
    { 16, -2, 1, 0, 0, NumberKey9 },
    { 60, -2, 1, 0, 0, ScreenLarger },
    { 61, -2, 1, 0, 0, ScreenSmaller },
    { 62, -2, 1, 0, 0, BuyArmour },
    { 63, -2, 1, 0, 0, BuyPower },
    { 64, -2, 1, 0, 0, BuyOffense },
    { 65, -2, 1, 0, 0, ViewNetPlayer },
    { 66, -2, 1, 0, 0, UserSendMessage },
    //{ 25, -2, 1, 1, 0, locret_2CDB4 },
    { 26, -2, 1, 1, 0, ToggleInfo },
    { 26, 8, 1, 1, 0, ToggleInfo },
    { 26, 7, 1, 1, 0, ToggleInfo }
};

char gString[84];
int gToo_late;
int gAllow_car_flying;
int gRecover_timer;
tU32 gLast_repair_time;
int gHad_auto_recover;
tEdit_mode gWhich_edit_mode;
int gRepair_last_time;
int gRecovery_voucher_count;
int gInstant_handbrake;
int gAuto_repair;
tU32 gPalette_fade_time;
int gEntering_message;

int _unittest_controls_lastGetPowerup = 0;

// IDA: void __cdecl AbortRace()
void AbortRace() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl F4Key()
void F4Key() {
    char s[256];
    tEdit_mode old_edit_mode;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetFlag(int i@<EAX>)
void SetFlag(int i) {
    LOG_TRACE("(%d)", i);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FinishLap(int i@<EAX>)
void FinishLap(int i) {
    LOG_TRACE("(%d)", i);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EnsureSpecialVolumesHidden()
void EnsureSpecialVolumesHidden() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ShowSpecialVolumesIfRequ()
void ShowSpecialVolumesIfRequ() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoEditModeKey(int pIndex@<EAX>)
void DoEditModeKey(int pIndex) {
    int modifiers;
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl F5Key()
void F5Key() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl F6Key()
void F6Key() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl F7Key()
void F7Key() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl F8Key()
void F8Key() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl F10Key()
void F10Key() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl F11Key()
void F11Key() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl F12Key()
void F12Key() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey0()
void NumberKey0() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey1()
void NumberKey1() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey2()
void NumberKey2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey3()
void NumberKey3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey4()
void NumberKey4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey5()
void NumberKey5() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey6()
void NumberKey6() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey7()
void NumberKey7() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey8()
void NumberKey8() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NumberKey9()
void NumberKey9() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LookLeft()
void LookLeft() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LookForward()
void LookForward() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LookRight()
void LookRight() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DamageTest()
void DamageTest() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageEngine()
void TDamageEngine() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageDriver()
void TDamageDriver() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageTrans()
void TDamageTrans() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageSteering()
void TDamageSteering() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageLFWheel()
void TDamageLFWheel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageLFBrake()
void TDamageLFBrake() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageLRBrake()
void TDamageLRBrake() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageLRWheel()
void TDamageLRWheel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageRFWheel()
void TDamageRFWheel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageRFBrake()
void TDamageRFBrake() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageRRBrake()
void TDamageRRBrake() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TDamageRRWheel()
void TDamageRRWheel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveBonnetForward()
void MoveBonnetForward() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SaveBonnet()
void SaveBonnet() {
    br_actor* bonny;
    tPath_name the_path;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveBonnetBackward()
void MoveBonnetBackward() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveBonnetLeft()
void MoveBonnetLeft() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ShrinkBonnetX()
void ShrinkBonnetX() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SwellBonnetX()
void SwellBonnetX() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ShrinkBonnetY()
void ShrinkBonnetY() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SwellBonnetY()
void SwellBonnetY() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ShrinkBonnetZ()
void ShrinkBonnetZ() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SwellBonnetZ()
void SwellBonnetZ() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveBonnetDown()
void MoveBonnetDown() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveBonnetRight()
void MoveBonnetRight() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoveBonnetUp()
void MoveBonnetUp() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TiltBonnetDownX()
void TiltBonnetDownX() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TiltBonnetUpX()
void TiltBonnetUpX() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TiltBonnetDownY()
void TiltBonnetDownY() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TiltBonnetUpY()
void TiltBonnetUpY() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TiltBonnetDownZ()
void TiltBonnetDownZ() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TiltBonnetUpZ()
void TiltBonnetUpZ() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleCockpit()
void ToggleCockpit() {
    br_scalar ts;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleMirror()
void ToggleMirror() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ConcussMe()
void ConcussMe() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckHelp()
void CheckHelp() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckLoadSave()
void CheckLoadSave() {
    int save_load_allowed;
    int switched_res;
    LOG_TRACE8("()");

    save_load_allowed = !gProgram_state.saving && !gProgram_state.loading && gProgram_state.prog_status == eProg_game_ongoing && !gProgram_state.dont_save_or_load;

    if (CmdKeyDown(KEYMAP_F2, KEYMAP_S)) {
        if (save_load_allowed) {
            FadePaletteDown();
            ClearEntireScreen();
            if (gProgram_state.racing) {
                GoingToInterfaceFromRace();
            }
            DoSaveGame(gProgram_state.racing == 0);
            if (gProgram_state.racing) {
                GoingBackToRaceFromInterface();
            }
        }
        WaitForNoKeys();
    }
    if (CmdKeyDown(KEYMAP_F3, KEYMAP_L)) {
        if (save_load_allowed && !gProgram_state.dont_load) {
            FadePaletteDown();
            ClearEntireScreen();
            if (gProgram_state.racing) {
                GoingToInterfaceFromRace();
            }
            if (DoLoadGame() && !gProgram_state.racing) {
                gProgram_state.prog_status = eProg_game_starting;
            }
            if (gProgram_state.racing) {
                GoingBackToRaceFromInterface();
            }
            PlayFlicsFromMemory();
        }
        WaitForNoKeys();
    }
}

// IDA: void __usercall CheckToggles(int pRacing@<EAX>)
void CheckToggles(int pRacing) {
    int i;
    int new_state;
    SILENT_STUB();
}

// IDA: int __usercall CarWorldOffFallenCheckThingy@<EAX>(tCar_spec *pCar@<EAX>, int pCheck_around@<EDX>)
int CarWorldOffFallenCheckThingy(tCar_spec* pCar, int pCheck_around) {
    br_vector3 car_pos;
    br_vector3 offset_c;
    br_vector3 offset_w;
    int result;
    LOG_TRACE("(%p, %d)", pCar, pCheck_around);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall HasCarFallenOffWorld@<EAX>(tCar_spec *pCar@<EAX>)
int HasCarFallenOffWorld(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckForBeingOutOfThisWorld()
void CheckForBeingOutOfThisWorld() {
    static tU32 the_time;
    static tU32 sLast_check;
    int time_step;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CheckHornLocal(tCar_spec *pCar@<EAX>)
void CheckHornLocal(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CheckHorn3D(tCar_spec *pCar@<EAX>)
void CheckHorn3D(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckHorns()
void CheckHorns() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetRecovery()
void SetRecovery() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RecoverCar()
void RecoverCar() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckMapRenderMove()
void CheckMapRenderMove() {
    int shift_down;
    int amount;
    float old_x;
    float old_y;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ExplodeCar(tCar_spec *pCar@<EAX>)
void ExplodeCar(tCar_spec* pCar) {
    br_vector3 tv;
    br_vector3 pos;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CheckRecoveryOfCars(tU32 pEndFrameTime@<EAX>)
void CheckRecoveryOfCars(tU32 pEndFrameTime) {
    int i;
    int time;
    char s[256];
    LOG_TRACE("(%d)", pEndFrameTime);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoseSomePSPowerups(int pNumber@<EAX>)
void LoseSomePSPowerups(int pNumber) {
    int index;
    LOG_TRACE("(%d)", pNumber);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckOtherRacingKeys()
void CheckOtherRacingKeys() {
    int i;
    int j;
    int new_level;
    int old_level;
    char s[256];
    tU32 cost;
    br_scalar ts;
    br_vector3 tv;
    int flip_up_flag;
    tCar_spec* car;
    float bodywork_repair_amount;
    static tU32 total_repair_cost;
    static tS3_sound_tag sound_tag;
    static br_scalar amount;
    static int NeedToExpandBoundingBox;
    static int total_difference;
    static int stopped_repairing;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl CheckRecoverCost()
int CheckRecoverCost() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SortOutRecover(tCar_spec *pCar@<EAX>)
void SortOutRecover(tCar_spec* pCar) {
    int the_time;
    int val;
    static int old_time;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetFlipUpCar(tCar_spec *pCar@<EAX>)
void SetFlipUpCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FlipUpCar(tCar_spec *car@<EAX>)
void FlipUpCar(tCar_spec* car) {
    br_vector3 tv;
    br_vector3 dir;
    int new_pos;
    int i;
    int j;
    int l;
    int count;
    br_scalar dist;
    br_material* material;
    br_scalar t;
    LOG_TRACE("(%p)", car);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetPowerup(int pNum@<EAX>)
void GetPowerup(int pNum) {
    _unittest_controls_lastGetPowerup = pNum;
    STUB();
}

// IDA: void __usercall CheckSystemKeys(int pRacing@<EAX>)
void CheckSystemKeys(int pRacing) {
    tU32 start_menu_time;
    int i;

    start_menu_time = PDGetTotalTime();
    CheckQuit();
    if (!gAction_replay_mode) {
        CheckLoadSave();
    }
    AddLostTime(PDGetTotalTime() - start_menu_time);
    CheckToggles(pRacing);
    if (pRacing & !gAction_replay_mode) {
        CheckOtherRacingKeys();
    }
}

// IDA: void __cdecl CheckKevKeys()
void CheckKevKeys() {
    int i;
    tU32* value;
    char s[128];

    value = KevKeyService();
    if (value[0] == 0) {
        return;
    }

    for (i = 0; gKev_keys[i].action_proc != 0; i++) {
        if (gKev_keys[i].code == value[0] && gKev_keys[i].code2 == value[1]) {
            break;
        }
    }

    if (gKev_keys[i].action_proc == 0) {
        return;
    }
    gKev_keys[i].action_proc(gKev_keys[i].num);
}

// IDA: void __cdecl BrakeInstantly()
void BrakeInstantly() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PollCarControls(tU32 pTime_difference@<EAX>)
void PollCarControls(tU32 pTime_difference) {
    int decay_steering;
    int decay_speed;
    float decay_rate;
    tS32 joyX;
    tS32 joyY;
    tCar_controls keys;
    tJoystick joystick;
    tCar_spec* c;
    LOG_TRACE("(%d)", pTime_difference);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PollCameraControls(tU32 pTime_difference@<EAX>)
void PollCameraControls(tU32 pTime_difference) {
    int flag;
    int left;
    int right;
    int swirl_mode;
    int up_and_down_mode;
    int going_up;
    static int last_swirl_mode;
    LOG_TRACE("(%d)", pTime_difference);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetFlag2(int i@<EAX>)
void SetFlag2(int i) {
    LOG_TRACE("(%d)", i);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleFlying()
void ToggleFlying() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleInvulnerability()
void ToggleInvulnerability() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MoreTime()
void MoreTime() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MuchMoreTime()
void MuchMoreTime() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleTimerFreeze()
void ToggleTimerFreeze() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EarnDosh()
void EarnDosh() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LoseDosh()
void LoseDosh() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleMap()
void ToggleMap() {
    static int old_indent;
    static int was_in_cockpit;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl HornBlowing()
int HornBlowing() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleArrow()
void ToggleArrow() {
    static br_actor* old_actor;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetRecoverVoucherCount()
int GetRecoverVoucherCount() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddVouchers(int pCount@<EAX>)
void AddVouchers(int pCount) {
    LOG_TRACE("(%d)", pCount);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetRecoveryVouchers()
void ResetRecoveryVouchers() {
    LOG_TRACE("()");

    gRecovery_voucher_count = 0;
}

// IDA: void __cdecl CycleCarTexturingLevel()
void CycleCarTexturingLevel() {
    tCar_texturing_level new_level;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleWallTexturingLevel()
void CycleWallTexturingLevel() {
    tWall_texturing_level new_level;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleRoadTexturingLevel()
void CycleRoadTexturingLevel() {
    tRoad_texturing_level new_level;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleYonFactor()
void CycleYonFactor() {
    br_scalar new_factor;
    char factor_str[5];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetSoundDetailLevel(int pLevel@<EAX>)
void SetSoundDetailLevel(int pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gSound_detail_level = pLevel;
}

// IDA: void __usercall ReallySetSoundDetailLevel(int pLevel@<EAX>)
void ReallySetSoundDetailLevel(int pLevel) {
    LOG_TRACE("(%d)", pLevel);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetSoundDetailLevel()
int GetSoundDetailLevel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleSoundDetailLevel()
void CycleSoundDetailLevel() {
    int new_level;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CycleCarSimplificationLevel()
void CycleCarSimplificationLevel() {
    char* src;
    char* dst;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleAccessoryRendering()
void ToggleAccessoryRendering() {
    int on;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleSmoke()
void ToggleSmoke() {
    int on;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawSomeText2(tDR_font *pFont@<EAX>)
void DrawSomeText2(tDR_font* pFont) {
    int y;
    int i;
    char* txt[15] = {
        "Cops    Show all racers on map    Show peds on map",
        "Random pick-up generation    Pick-up respawn",
        "Open game    Closed game",
        "Grid start    Random start",
        "Random races    Sequential races",
        "Include opponents' cars in car choices",
        "Choose cars    manually    randomly    include Big APC",
        "Starting credits    0    2000    5000    10000    20000",
        "Driven to Destruction",
        "Car Crusher",
        "Carnage Accumulator",
        "Checkpoint Stampede",
        "Sudden Death",
        "Terminal Tag",
        "Fox 'n' Hounds"
    };

    ClearEntireScreen();
    y = 0;
    for (i = 0; i < 15; i++) {
        TransDRPixelmapText(gBack_screen, 0, y, pFont, txt[i], 320);
        y += pFont->height + 1;
    }

    PDScreenBufferSwap(0);
    //PrintScreen();
}

// IDA: void __cdecl DrawSomeText()
void DrawSomeText() {
    DrawSomeText2(&gFonts[1]);
    DrawSomeText2(&gFonts[2]);
    DrawSomeText2(&gFonts[3]);
    DrawSomeText2(&gFonts[4]);
    DrawSomeText2(&gFonts[6]);
    DrawSomeText2(&gFonts[7]);
    DrawSomeText2(&gFonts[8]);
}

// IDA: void __cdecl SaySorryYouLittleBastard()
void SaySorryYouLittleBastard() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl UserSendMessage()
void UserSendMessage() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EnterUserMessage()
void EnterUserMessage() {
    static int last_key;
    static int about_to_die;
    static tU32 next_time;
    char* the_message;
    char* p;
    int len;
    int the_key;
    int abuse_num;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisplayUserMessage()
void DisplayUserMessage() {
    char* the_message;
    int len;
    tDR_font* font;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitAbuseomatic()
void InitAbuseomatic() {
    char path[256];
    char s[256];
    FILE* f;
    int i;
    int len;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeAbuseomatic()
void DisposeAbuseomatic() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
