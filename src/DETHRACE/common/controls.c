#include "controls.h"

#include "common/displays.h"
#include "common/globvars.h"
#include "common/graphics.h"
#include "input.h"
#include "pc-dos/dossys.h"
#include "sound.h"
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
tToggle_element gToggle_array[43];
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

// Offset: 0
// Size: 53
void AbortRace() {
    NOT_IMPLEMENTED();
}

// Offset: 56
// Size: 271
void F4Key() {
    tEdit_mode old_edit_mode;
    NOT_IMPLEMENTED();
}

// Offset: 328
// Size: 87
// EAX: i
void SetFlag(int i) {
    NOT_IMPLEMENTED();
}

// Offset: 416
// Size: 42
// EAX: i
void FinishLap(int i) {
    NOT_IMPLEMENTED();
}

// Offset: 460
// Size: 48
void EnsureSpecialVolumesHidden() {
    NOT_IMPLEMENTED();
}

// Offset: 508
// Size: 48
void ShowSpecialVolumesIfRequ() {
    NOT_IMPLEMENTED();
}

// Offset: 556
// Size: 261
// EAX: pIndex
void DoEditModeKey(int pIndex) {
    int modifiers;
    NOT_IMPLEMENTED();
}

// Offset: 820
// Size: 41
void F5Key() {
    NOT_IMPLEMENTED();
}

// Offset: 864
// Size: 44
void F6Key() {
    NOT_IMPLEMENTED();
}

// Offset: 908
// Size: 44
void F7Key() {
    NOT_IMPLEMENTED();
}

// Offset: 952
// Size: 44
void F8Key() {
    NOT_IMPLEMENTED();
}

// Offset: 996
// Size: 44
void F10Key() {
    NOT_IMPLEMENTED();
}

// Offset: 1040
// Size: 44
void F11Key() {
    NOT_IMPLEMENTED();
}

// Offset: 1084
// Size: 44
void F12Key() {
    NOT_IMPLEMENTED();
}

// Offset: 1128
// Size: 44
void NumberKey0() {
    NOT_IMPLEMENTED();
}

// Offset: 1172
// Size: 44
void NumberKey1() {
    NOT_IMPLEMENTED();
}

// Offset: 1216
// Size: 44
void NumberKey2() {
    NOT_IMPLEMENTED();
}

// Offset: 1260
// Size: 44
void NumberKey3() {
    NOT_IMPLEMENTED();
}

// Offset: 1304
// Size: 44
void NumberKey4() {
    NOT_IMPLEMENTED();
}

// Offset: 1348
// Size: 44
void NumberKey5() {
    NOT_IMPLEMENTED();
}

// Offset: 1392
// Size: 44
void NumberKey6() {
    NOT_IMPLEMENTED();
}

// Offset: 1436
// Size: 44
void NumberKey7() {
    NOT_IMPLEMENTED();
}

// Offset: 1480
// Size: 44
void NumberKey8() {
    NOT_IMPLEMENTED();
}

// Offset: 1524
// Size: 44
void NumberKey9() {
    NOT_IMPLEMENTED();
}

// Offset: 1568
// Size: 175
void LookLeft() {
    NOT_IMPLEMENTED();
}

// Offset: 1744
// Size: 128
void LookForward() {
    NOT_IMPLEMENTED();
}

// Offset: 1872
// Size: 175
void LookRight() {
    NOT_IMPLEMENTED();
}

// Offset: 2048
// Size: 34
void DamageTest() {
    NOT_IMPLEMENTED();
}

// Offset: 2084
// Size: 44
void TDamageEngine() {
    NOT_IMPLEMENTED();
}

// Offset: 2128
// Size: 91
void TDamageDriver() {
    NOT_IMPLEMENTED();
}

// Offset: 2220
// Size: 44
void TDamageTrans() {
    NOT_IMPLEMENTED();
}

// Offset: 2264
// Size: 44
void TDamageSteering() {
    NOT_IMPLEMENTED();
}

// Offset: 2308
// Size: 44
void TDamageLFWheel() {
    NOT_IMPLEMENTED();
}

// Offset: 2352
// Size: 44
void TDamageLFBrake() {
    NOT_IMPLEMENTED();
}

// Offset: 2396
// Size: 44
void TDamageLRBrake() {
    NOT_IMPLEMENTED();
}

// Offset: 2440
// Size: 44
void TDamageLRWheel() {
    NOT_IMPLEMENTED();
}

// Offset: 2484
// Size: 44
void TDamageRFWheel() {
    NOT_IMPLEMENTED();
}

// Offset: 2528
// Size: 44
void TDamageRFBrake() {
    NOT_IMPLEMENTED();
}

// Offset: 2572
// Size: 44
void TDamageRRBrake() {
    NOT_IMPLEMENTED();
}

// Offset: 2616
// Size: 44
void TDamageRRWheel() {
    NOT_IMPLEMENTED();
}

// Offset: 2660
// Size: 76
void MoveBonnetForward() {
    NOT_IMPLEMENTED();
}

// Offset: 2736
// Size: 93
void SaveBonnet() {
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 2832
// Size: 73
void MoveBonnetBackward() {
    NOT_IMPLEMENTED();
}

// Offset: 2908
// Size: 76
void MoveBonnetLeft() {
    NOT_IMPLEMENTED();
}

// Offset: 2984
// Size: 73
void ShrinkBonnetX() {
    NOT_IMPLEMENTED();
}

// Offset: 3060
// Size: 73
void SwellBonnetX() {
    NOT_IMPLEMENTED();
}

// Offset: 3136
// Size: 73
void ShrinkBonnetY() {
    NOT_IMPLEMENTED();
}

// Offset: 3212
// Size: 73
void SwellBonnetY() {
    NOT_IMPLEMENTED();
}

// Offset: 3288
// Size: 73
void ShrinkBonnetZ() {
    NOT_IMPLEMENTED();
}

// Offset: 3364
// Size: 73
void SwellBonnetZ() {
    NOT_IMPLEMENTED();
}

// Offset: 3440
// Size: 73
void MoveBonnetDown() {
    NOT_IMPLEMENTED();
}

// Offset: 3516
// Size: 73
void MoveBonnetRight() {
    NOT_IMPLEMENTED();
}

// Offset: 3592
// Size: 76
void MoveBonnetUp() {
    NOT_IMPLEMENTED();
}

// Offset: 3668
// Size: 67
void TiltBonnetDownX() {
    NOT_IMPLEMENTED();
}

// Offset: 3736
// Size: 67
void TiltBonnetUpX() {
    NOT_IMPLEMENTED();
}

// Offset: 3804
// Size: 67
void TiltBonnetDownY() {
    NOT_IMPLEMENTED();
}

// Offset: 3872
// Size: 67
void TiltBonnetUpY() {
    NOT_IMPLEMENTED();
}

// Offset: 3940
// Size: 67
void TiltBonnetDownZ() {
    NOT_IMPLEMENTED();
}

// Offset: 4008
// Size: 67
void TiltBonnetUpZ() {
    NOT_IMPLEMENTED();
}

// Offset: 4076
// Size: 246
void ToggleCockpit() {
    NOT_IMPLEMENTED();
}

// Offset: 4324
// Size: 149
void ToggleMirror() {
    NOT_IMPLEMENTED();
}

// Offset: 4476
// Size: 130
void ConcussMe() {
    NOT_IMPLEMENTED();
}

// Offset: 4608
// Size: 34
void CheckHelp() {
    NOT_IMPLEMENTED();
}

// Offset: 4644
// Size: 313
void CheckLoadSave() {
    int switched_res;
    NOT_IMPLEMENTED();
}

// Offset: 4960
// Size: 364
// EAX: pRacing
void CheckToggles(int pRacing) {
    int i;
    int new_state;
    NOT_IMPLEMENTED();
}

// Offset: 5324
// Size: 444
// EAX: pCar
// EDX: pCheck_around
int CarWorldOffFallenCheckThingy(tCar_spec* pCar, int pCheck_around) {
    br_vector3 car_pos;
    br_vector3 offset_c;
    br_vector3 offset_w;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 5768
// Size: 56
// EAX: pCar
int HasCarFallenOffWorld(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 5824
// Size: 250
void CheckForBeingOutOfThisWorld() {
    static tU32 sLast_check;
    int time_step;
    NOT_IMPLEMENTED();
}

// Offset: 6076
// Size: 190
// EAX: pCar
void CheckHornLocal(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6268
// Size: 249
// EAX: pCar
void CheckHorn3D(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6520
// Size: 103
void CheckHorns() {
    NOT_IMPLEMENTED();
}

// Offset: 6624
// Size: 413
void SetRecovery() {
    NOT_IMPLEMENTED();
}

// Offset: 7040
// Size: 72
void RecoverCar() {
    NOT_IMPLEMENTED();
}

// Offset: 7112
// Size: 765
void CheckMapRenderMove() {
    int amount;
    float old_x;
    float old_y;
    NOT_IMPLEMENTED();
}

// Offset: 7880
// Size: 300
// EAX: pCar
void ExplodeCar(tCar_spec* pCar) {
    br_vector3 tv;
    br_vector3 pos;
    NOT_IMPLEMENTED();
}

// Offset: 8180
// Size: 448
// EAX: pEndFrameTime
void CheckRecoveryOfCars(tU32 pEndFrameTime) {
    int i;
    int time;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 8628
// Size: 107
// EAX: pNumber
void LoseSomePSPowerups(int pNumber) {
    int index;
    NOT_IMPLEMENTED();
}

// Offset: 8736
// Size: 1771
void CheckOtherRacingKeys() {
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
    NOT_IMPLEMENTED();
}

// Offset: 10508
// Size: 259
int CheckRecoverCost() {
    NOT_IMPLEMENTED();
}

// Offset: 10768
// Size: 279
// EAX: pCar
void SortOutRecover(tCar_spec* pCar) {
    int the_time;
    int val;
    static int old_time;
    NOT_IMPLEMENTED();
}

// Offset: 11048
// Size: 115
// EAX: pCar
void SetFlipUpCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 11164
// Size: 1385
// EAX: car
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
    NOT_IMPLEMENTED();
}

// Offset: 12552
// Size: 50
// EAX: pNum
void GetPowerup(int pNum) {
    _unittest_controls_lastGetPowerup = pNum;
    LOG_WARN("Not implemented");
}

// Offset: 12604
// Size: 112
// EAX: pRacing
void CheckSystemKeys(int pRacing) {
    tU32 start_menu_time;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 12716
// Size: 395
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

// Offset: 13112
// Size: 190
void BrakeInstantly() {
    NOT_IMPLEMENTED();
}

// Offset: 13304
// Size: 853
// EAX: pTime_difference
void PollCarControls(tU32 pTime_difference) {
    int decay_steering;
    int decay_speed;
    float decay_rate;
    tS32 joyX;
    tS32 joyY;
    tCar_controls keys;
    tJoystick joystick;
    tCar_spec* c;
    NOT_IMPLEMENTED();
}

// Offset: 14160
// Size: 846
// EAX: pTime_difference
void PollCameraControls(tU32 pTime_difference) {
    int flag;
    int left;
    int right;
    int swirl_mode;
    int up_and_down_mode;
    int going_up;
    static int last_swirl_mode;
    NOT_IMPLEMENTED();
}

// Offset: 15008
// Size: 62
// EAX: i
void SetFlag2(int i) {
    NOT_IMPLEMENTED();
}

// Offset: 15072
// Size: 164
void ToggleFlying() {
    NOT_IMPLEMENTED();
}

// Offset: 15236
// Size: 136
void ToggleInvulnerability() {
    NOT_IMPLEMENTED();
}

// Offset: 15372
// Size: 44
void MoreTime() {
    NOT_IMPLEMENTED();
}

// Offset: 15416
// Size: 44
void MuchMoreTime() {
    NOT_IMPLEMENTED();
}

// Offset: 15460
// Size: 139
void ToggleTimerFreeze() {
    NOT_IMPLEMENTED();
}

// Offset: 15600
// Size: 44
void EarnDosh() {
    NOT_IMPLEMENTED();
}

// Offset: 15644
// Size: 44
void LoseDosh() {
    NOT_IMPLEMENTED();
}

// Offset: 15688
// Size: 301
void ToggleMap() {
    static int old_indent;
    static int was_in_cockpit;
    NOT_IMPLEMENTED();
}

// Offset: 15992
// Size: 51
int HornBlowing() {
    NOT_IMPLEMENTED();
}

// Offset: 16044
// Size: 215
void ToggleArrow() {
    NOT_IMPLEMENTED();
}

// Offset: 16260
// Size: 45
int GetRecoverVoucherCount() {
    NOT_IMPLEMENTED();
}

// Offset: 16308
// Size: 46
// EAX: pCount
void AddVouchers(int pCount) {
    NOT_IMPLEMENTED();
}

// Offset: 16356
// Size: 44
void ResetRecoveryVouchers() {
    NOT_IMPLEMENTED();
}

// Offset: 16400
// Size: 194
void CycleCarTexturingLevel() {
    NOT_IMPLEMENTED();
}

// Offset: 16596
// Size: 202
void CycleWallTexturingLevel() {
    NOT_IMPLEMENTED();
}

// Offset: 16800
// Size: 157
void CycleRoadTexturingLevel() {
    NOT_IMPLEMENTED();
}

// Offset: 16960
// Size: 246
void CycleYonFactor() {
    char factor_str[5];
    NOT_IMPLEMENTED();
}

// Offset: 17208
// Size: 45
// EAX: pLevel
void SetSoundDetailLevel(int pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gSound_detail_level = pLevel;
}

// Offset: 17256
// Size: 65
// EAX: pLevel
void ReallySetSoundDetailLevel(int pLevel) {
    NOT_IMPLEMENTED();
}

// Offset: 17324
// Size: 45
int GetSoundDetailLevel() {
    NOT_IMPLEMENTED();
}

// Offset: 17372
// Size: 209
void CycleSoundDetailLevel() {
    NOT_IMPLEMENTED();
}

// Offset: 17584
// Size: 162
void CycleCarSimplificationLevel() {
    char* dst;
    NOT_IMPLEMENTED();
}

// Offset: 17748
// Size: 194
void ToggleAccessoryRendering() {
    NOT_IMPLEMENTED();
}

// Offset: 17944
// Size: 155
void ToggleSmoke() {
    NOT_IMPLEMENTED();
}

// Offset: 18100
// Size: 144
// EAX: pFont
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

// Offset: 18244
// Size: 104
void DrawSomeText() {
    DrawSomeText2(&gFonts[1]);
    DrawSomeText2(&gFonts[2]);
    DrawSomeText2(&gFonts[3]);
    DrawSomeText2(&gFonts[4]);
    DrawSomeText2(&gFonts[6]);
    DrawSomeText2(&gFonts[7]);
    DrawSomeText2(&gFonts[8]);
}

// Offset: 18348
// Size: 44
void SaySorryYouLittleBastard() {
    NOT_IMPLEMENTED();
}

// Offset: 18392
// Size: 66
void UserSendMessage() {
    NOT_IMPLEMENTED();
}

// Offset: 18460
// Size: 815
void EnterUserMessage() {
    static int about_to_die;
    static tU32 next_time;
    char* the_message;
    char* p;
    int len;
    int the_key;
    int abuse_num;
    NOT_IMPLEMENTED();
}

// Offset: 19276
// Size: 435
void DisplayUserMessage() {
    char* the_message;
    int len;
    tDR_font* font;
    NOT_IMPLEMENTED();
}

// Offset: 19712
// Size: 347
void InitAbuseomatic() {
    char s[256];
    FILE* f;
    int i;
    int len;
    NOT_IMPLEMENTED();
}

// Offset: 20060
// Size: 94
void DisposeAbuseomatic() {
    NOT_IMPLEMENTED();
}
