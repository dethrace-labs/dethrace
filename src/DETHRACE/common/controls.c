#include "controls.h"

#include "brender/brender.h"
#include "car.h"
#include "constants.h"
#include "crush.h"
#include "depth.h"
#include "displays.h"
#include "finteray.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/trace.h"
#include "init.h"
#include "input.h"
#include "loadsave.h"
#include "mainloop.h"
#include "netgame.h"
#include "network.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "powerup.h"
#include "pratcam.h"
#include "replay.h"
#include "s3/s3sound.h"
#include "sound.h"
#include "spark.h"
#include "structur.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

tToggle_element gToggle_array[] = {
    { KEYMAP_MIRROR, -2, 1, 1, 0, ToggleMirror },
    { KEYMAP_MAP, -2, 1, 1, 0, ToggleMap },
    { KEYMAP_PRATCAM, -2, 1, 1, 0, TogglePratcam },
    { KEYMAP_RECOVERY, -2, 1, 1, 0, SetRecovery },
    { KEYMAP_CTRL_ABORT_RACE, 7, 1, 1, 0, AbortRace },
    { KEYMAP_COCKPIT, -2, 1, 1, 0, ToggleCockpit },
    { KEYMAP_LOOK_LEFT, -2, 1, 1, 0, LookLeft },
    { KEYMAP_LOOK_FORWARD, -2, 1, 1, 0, LookForward },
    { KEYMAP_LOOK_RIGHT, -2, 1, 1, 0, LookRight },
    { KEYMAP_SOUND, -2, 0, 1, 0, ToggleSoundEnable },
    { KEYMAP_SHIFT_DUMPSCREEN, KEYMAP_SHIFT_ANY, 0, 1, 0, PrintScreen },
    { KEYMAP_CTRL_DRAW_SOME_TEXT, KEYMAP_CONTROL_ANY, 1, 1, 0, DrawSomeText }, // commented out in original executable
    { KEYMAP_CTRL_FLY, KEYMAP_CONTROL_ANY, 1, 1, 0, ToggleFlying },
    { KEYMAP_PEDESTRIANS, -2, 1, 1, 0, TogglePedestrians },
    { KEYMAP_F4, -2, 0, 0, 0, F4Key },
    { KEYMAP_F5, -2, 1, 0, 0, F5Key },
    { KEYMAP_F6, -2, 1, 0, 0, F6Key },
    { KEYMAP_F7, -2, 1, 0, 0, F7Key },
    { KEYMAP_F8, -2, 1, 0, 0, F8Key },
    { KEYMAP_F10, -2, 1, 0, 0, F10Key },
    { KEYMAP_F11, -2, 1, 0, 0, F11Key },
    { KEYMAP_F12, -2, 1, 0, 0, F12Key },
    { KEYMAP_EDITMODE_0, -2, 1, 0, 0, NumberKey0 },
    { KEYMAP_EDITMODE_1, -2, 1, 0, 0, NumberKey1 },
    { KEYMAP_EDITMODE_2, -2, 1, 0, 0, NumberKey2 },
    { KEYMAP_EDITMODE_3, -2, 1, 0, 0, NumberKey3 },
    { KEYMAP_EDITMODE_4, -2, 1, 0, 0, NumberKey4 },
    { KEYMAP_EDITMODE_5, -2, 1, 0, 0, NumberKey5 },
    { KEYMAP_EDITMODE_6, -2, 1, 0, 0, NumberKey6 },
    { KEYMAP_EDITMODE_7, -2, 1, 0, 0, NumberKey7 },
    { KEYMAP_EDITMODE_8, -2, 1, 0, 0, NumberKey8 },
    { KEYMAP_EDITMODE_9, -2, 1, 0, 0, NumberKey9 },
    { KEYMAP_SCREEN_SMALLER, -2, 1, 0, 0, ScreenSmaller },
    { KEYMAP_SCREEN_LARGER, -2, 1, 0, 0, ScreenLarger },
    { KEYMAP_BUY_ARMOUR, -2, 1, 0, 0, BuyArmour },
    { KEYMAP_BUY_POWER, -2, 1, 0, 0, BuyPower },
    { KEYMAP_BUY_OFFENSE, -2, 1, 0, 0, BuyOffense },
    { KEYMAP_VIEW_NETPLAYER, -2, 1, 0, 0, ViewNetPlayer },
    { KEYMAP_SEND_MESSAGE, -2, 1, 0, 0, UserSendMessage },
    { KEYMAP_ARROW, -2, 1, 1, 0, ToggleArrow }, // commented out in original executable
    { KEYMAP_INFO, -2, 1, 1, 0, ToggleInfo },
    { KEYMAP_INFO, KEYMAP_SHIFT_ANY, 1, 1, 0, ToggleInfo },
    { KEYMAP_INFO, KEYMAP_CONTROL_ANY, 1, 1, 0, ToggleInfo }
};
int gRepair_last_time;
int gHad_auto_recover;
tU32 gLast_repair_time;
tEdit_mode gWhich_edit_mode = eEdit_mode_options;
char* gEdit_mode_names[10] = {
    "Cheat",
    "Accessories",
    "Special Volumes",
    "Pedestrians",
    "Opponents",
    "Prat-cam",
    "Depth effects",
    "Damage",
    "Bonnet",
    "Options",
};
// order is: { None, CTRL, ALT, CTRL+ALT, SHIFT, CTRL+SHIFT, ALT+SHIFT, CTRL+ALT+SHIFT }
tEdit_func* gEdit_funcs[10][18][8] = {
    {  },  // eEdit_mode_cheat
    {  },  // eEdit_mode_acc
    {  },  // eEdit_mode_spec_vol
    {  },  // eEdit_mode_ped
    {  },  // eEdit_mode_opp
    {  },  // eEdit_mode_pratcam
    {  },  // eEdit_mode_depth
    {  },  // eEdit_mode_damage
    {  },  // eEdit_mode_bonnet
    {  // eEdit_mode_options
        { },  // F5
        { },  // F6
        { },  // F7
        { },  // F8
        { },  // F10
        { },  // F11
        { },  // F12
        { },  // 0
        { CycleCarSimplificationLevel, NULL, NULL, NULL, CycleCarTexturingLevel, },  // 1
        { ToggleShadow, NULL, NULL, NULL, ToggleSmoke, },  // 2
        { CycleWallTexturingLevel, NULL, NULL, NULL, CycleRoadTexturingLevel, },  // 3
        { ToggleSky, NULL, NULL, NULL, ToggleDepthCueing, },  // 4
        { CycleYonFactor, NULL, NULL, NULL, ToggleAccessoryRendering, },  // 5
        { DecreaseYon, NULL, NULL, NULL, IncreaseYon,  },  // 6
        { CycleSoundDetailLevel,  },  // 7
        { },  // 8
        { },  // 9
        { },  // not used
    },
};
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
    { .code = 0x3FFF84D5, .code2 = 0x84A42DF4, .action_proc = GetPowerup, .num = 42 },
    { .code = 0x37E83018, .code2 = 0xB609AEE6, .action_proc = GetPowerup, .num = 43 },
    { .code = 0x2DB03B19, .code2 = 0x924A84B7, .action_proc = GetPowerup, .num = 44 },
    { .code = 0x30A19FAB, .code2 = 0x2B0C2782, .action_proc = GetPowerup, .num = 45 },
    { .code = 0x0, .code2 = 0x0, .action_proc = 0x0, .num = 0x0 }
};
int gAllow_car_flying;
int gEntering_message;
tU32 gPalette_fade_time; // was gRecover_timer
char* gAbuse_text[10];
char gString[84];
int gToo_late;
int gRecover_timer;
int gRecovery_voucher_count;
int gInstant_handbrake;
int gAuto_repair;

int _unittest_controls_lastGetPowerup = 0;

// IDA: void __cdecl AbortRace()
void AbortRace() {
    LOG_TRACE("()");

    if (!gRace_finished) {
        gAbandon_game = 1;
    }
}

// IDA: void __cdecl F4Key()
void F4Key() {
    char s[256];
    tEdit_mode old_edit_mode;
    LOG_TRACE("()");

    old_edit_mode = gWhich_edit_mode;
    if (gI_am_cheating == 0xa11ee75d || (gI_am_cheating == 0xa11ee75d && gNet_mode == eNet_mode_none)) {
        if (PDKeyDown(KEY_SHIFT_ANY)) {
            gWhich_edit_mode--;
            if ((int)gWhich_edit_mode == -1) {
                gWhich_edit_mode = eEdit_mode_options;
            }
        } else {
            gWhich_edit_mode++;
            if (gWhich_edit_mode >= eEdit_mode_count) {
                gWhich_edit_mode = eEdit_mode_cheat;
            }
        }
        sprintf(s, "Edit mode: %s", gEdit_mode_names[gWhich_edit_mode]);
        NewTextHeadupSlot2(4, 0, 2000, -4, s, 0);
        if (gWhich_edit_mode == eEdit_mode_spec_vol && old_edit_mode != eEdit_mode_spec_vol) {
            ShowSpecialVolumes();
        } else if (gWhich_edit_mode != eEdit_mode_spec_vol && old_edit_mode == eEdit_mode_spec_vol) {
            HideSpecialVolumes();
        }
    } else {
        gWhich_edit_mode = eEdit_mode_options;
    }
}

// IDA: void __usercall SetFlag(int i@<EAX>)
void SetFlag(int i) {
    LOG_TRACE("(%d)", i);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FinishLap(int i@<EAX>)
void FinishLap(int i) {
    LOG_TRACE("(%d)", i);

    IncrementLap();
}

// IDA: void __cdecl EnsureSpecialVolumesHidden()
void EnsureSpecialVolumesHidden() {
    LOG_TRACE("()");
    STUB_ONCE();
}

// IDA: void __cdecl ShowSpecialVolumesIfRequ()
void ShowSpecialVolumesIfRequ() {
    LOG_TRACE("()");

    if (gWhich_edit_mode == eEdit_mode_spec_vol) {
        ShowSpecialVolumes();
    }
}

// IDA: void __usercall DoEditModeKey(int pIndex@<EAX>)
void DoEditModeKey(int pIndex) {
    int modifiers;
    LOG_TRACE("(%d)", pIndex);

    if (gI_am_cheating == 0xa11ee75d || (gI_am_cheating == 0x564e78b9 && gNet_mode == eNet_mode_none)) {
        modifiers = 0;
        if (PDKeyDown(KEY_SHIFT_ANY)) {
            modifiers |= 4;
        }
        if (PDKeyDown(KEY_ALT_ANY)) {
            modifiers |= 2;
        }
        if (PDKeyDown(KEY_CTRL_ANY)) {
            modifiers |= 1;
        }
        if (gEdit_funcs[gWhich_edit_mode][pIndex][modifiers] != NULL) {
            gEdit_funcs[gWhich_edit_mode][pIndex][modifiers]();
        }
    } else {
        gWhich_edit_mode = eEdit_mode_options;
    }
}

// IDA: void __cdecl F5Key()
void F5Key() {
    LOG_TRACE("()");

    DoEditModeKey(0);
}

// IDA: void __cdecl F6Key()
void F6Key() {
    LOG_TRACE("()");

    DoEditModeKey(1);
}

// IDA: void __cdecl F7Key()
void F7Key() {
    LOG_TRACE("()");

    DoEditModeKey(2);
}

// IDA: void __cdecl F8Key()
void F8Key() {
    LOG_TRACE("()");

    DoEditModeKey(3);
}

// IDA: void __cdecl F10Key()
void F10Key() {
    LOG_TRACE("()");

    DoEditModeKey(4);
}

// IDA: void __cdecl F11Key()
void F11Key() {
    LOG_TRACE("()");

    DoEditModeKey(5);
}

// IDA: void __cdecl F12Key()
void F12Key() {
    LOG_TRACE("()");

    DoEditModeKey(6);
}

// IDA: void __cdecl NumberKey0()
void NumberKey0() {
    LOG_TRACE("()");

    DoEditModeKey(7);
}

// IDA: void __cdecl NumberKey1()
void NumberKey1() {
    LOG_TRACE("()");

    DoEditModeKey(8);
}

// IDA: void __cdecl NumberKey2()
void NumberKey2() {
    LOG_TRACE("()");

    DoEditModeKey(9);
}

// IDA: void __cdecl NumberKey3()
void NumberKey3() {
    LOG_TRACE("()");

    DoEditModeKey(10);
}

// IDA: void __cdecl NumberKey4()
void NumberKey4() {
    LOG_TRACE("()");

    DoEditModeKey(11);
}

// IDA: void __cdecl NumberKey5()
void NumberKey5() {
    LOG_TRACE("()");

    DoEditModeKey(12);
}

// IDA: void __cdecl NumberKey6()
void NumberKey6() {
    LOG_TRACE("()");

    DoEditModeKey(13);
}

// IDA: void __cdecl NumberKey7()
void NumberKey7() {
    LOG_TRACE("()");

    DoEditModeKey(14);
}

// IDA: void __cdecl NumberKey8()
void NumberKey8() {
    LOG_TRACE("()");

    DoEditModeKey(15);
}

// IDA: void __cdecl NumberKey9()
void NumberKey9() {
    LOG_TRACE("()");

    DoEditModeKey(16);
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

    if ((&gProgram_state.current_car == gCar_to_view || gProgram_state.cockpit_on) && !gMap_mode) {
        if (!gAusterity_mode || gProgram_state.cockpit_on) {
            gProgram_state.cockpit_on = !gProgram_state.cockpit_on;
            if (gProgram_state.cockpit_on) {
                gCamera = gCamera_list[0];
            } else {
                gCamera = gCamera_list[1];
                InitialiseExternalCamera();
                PositionExternalCamera(&gProgram_state.current_car, 1);
            }
            AdjustRenderScreenSize();
            AdjustHeadups();
            MungeForwardSky();
        } else {
            NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(192));
        }
    }
}

// IDA: void __cdecl ToggleMirror()
void ToggleMirror() {
    LOG_TRACE("()");

    gProgram_state.mirror_on = !gProgram_state.mirror_on;
    ReinitialiseRearviewCamera();
    if (gProgram_state.mirror_on) {
        NewTextHeadupSlot(4, 0, 500, -4, GetMiscString(2));
    } else {
        NewTextHeadupSlot(4, 0, 500, -4, GetMiscString(3));
    }
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

    if (CmdKeyDown(KEYMAP_SAVE, KEYMAP_CTRL_SAVE)) {
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
    if (CmdKeyDown(KEYMAP_LOAD, KEYMAP_CTRL_LOAD)) {
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

    for (i = 0; i < COUNT_OF(gToggle_array); i++) {
        if ((!gToggle_array[i].in_game_only || pRacing)
            && ((!gTyping && !gEntering_message) || gToggle_array[i].key2 != -2)) {
            new_state = 0;
            if (gToggle_array[i].key1 == -2 || KeyIsDown(gToggle_array[i].key1)) {
                if (gToggle_array[i].key2 == -2 && gToggle_array[i].exact_modifiers) {
                    if (!PDKeyDown(KEY_SHIFT_ANY) && !PDKeyDown(KEY_ALT_ANY) && !PDKeyDown(KEY_CTRL_ANY) && !PDKeyDown(KEY_CTRL_ANY_2)) {
                        new_state = 1;
                    }
                } else {
                    if (KeyIsDown(gToggle_array[i].key2)) {
                        new_state = 1;
                    }
                }
            }
            if (gToggle_array[i].on_last_time != new_state) {
                gToggle_array[i].on_last_time = new_state;
                if (new_state) {
                    gToggle_array[i].action_proc();
                }
            }
        }
    }
}

// IDA: int __usercall CarWorldOffFallenCheckThingy@<EAX>(tCar_spec *pCar@<EAX>, int pCheck_around@<EDX>)
int CarWorldOffFallenCheckThingy(tCar_spec* pCar, int pCheck_around) {
    br_vector3 car_pos;
    br_vector3 offset_c;
    br_vector3 offset_w;
    int result;
    LOG_TRACE("(%p, %d)", pCar, pCheck_around);

    STUB_ONCE();
    return 0;
}

// IDA: int __usercall HasCarFallenOffWorld@<EAX>(tCar_spec *pCar@<EAX>)
int HasCarFallenOffWorld(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    return CarWorldOffFallenCheckThingy(pCar, 1);
}

// IDA: void __cdecl CheckForBeingOutOfThisWorld()
void CheckForBeingOutOfThisWorld() {
    static tU32 the_time;
    static tU32 sLast_check;
    int time_step;
    LOG_TRACE("()");

    the_time = PDGetTotalTime();

    if (gRecover_timer == 0 || ((gProgram_state.current_car.frame_collision_flag || gProgram_state.current_car.number_of_wheels_on_ground) && !IsCarInTheSea())) {
        gRecover_timer = 0;
        if ((the_time - sLast_check) > 200) {
            sLast_check = the_time;
            if (HasCarFallenOffWorld(&gProgram_state.current_car)) {
                gRecover_timer = 3000;
            }
        }
        if (IsCarInTheSea()) {
            if (!gRecover_timer) {
                gRecover_timer = 3000;
            }
        }
        return;
    }
    gRecover_timer -= gFrame_period;
    if (gRecover_timer <= 0 || IsCarInTheSea() == 2) {
        gRecover_timer = 0;
        RecoverCar();
        gHad_auto_recover = 1;
    }
}

// IDA: void __usercall CheckHornLocal(tCar_spec *pCar@<EAX>)
void CheckHornLocal(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    if (pCar->keys.horn && pCar->horn_sound_tag == 0) {
        pCar->horn_sound_tag = DRS3StartSound(gIndexed_outlets[0], 5209);
    } else if (!pCar->keys.horn && pCar->horn_sound_tag != 0) {
        while (S3SoundStillPlaying(pCar->horn_sound_tag)) {
            DRS3StopSound(pCar->horn_sound_tag);
            DRS3StopOutletSound(gIndexed_outlets[0]);
        }
        pCar->horn_sound_tag = 0;
    }
}

// IDA: void __usercall CheckHorn3D(tCar_spec *pCar@<EAX>)
void CheckHorn3D(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    STUB_ONCE();
}

// IDA: void __cdecl CheckHorns()
void CheckHorns() {
    int i;
    LOG_TRACE("()");

    if (gNet_mode) {
        for (i = 0; i < gNumber_of_net_players; ++i) {
            CheckHorn3D(gNet_players[i].car);
        }
    } else {
        CheckHornLocal(&gProgram_state.current_car);
    }
}

// IDA: void __cdecl SetRecovery()
void SetRecovery() {
    LOG_TRACE("()");

    if (gRace_finished
        || gProgram_state.current_car.knackered
        || gWait_for_it
        || gHad_auto_recover
        || gPalette_fade_time) {
        return;
    }

    if (gNet_mode == eNet_mode_none) {
        gRecover_car = 1;
        gRecover_timer = 0;
        return;
    }
    if (gProgram_state.current_car.time_to_recover) {
        if (GetRaceTime() + 600 >= gProgram_state.current_car.time_to_recover) {
            NewTextHeadupSlot2(4, 0, 2000, -4, GetMiscString(242), 1);
            gToo_late = 1;
        } else {
            gProgram_state.current_car.time_to_recover = 0;
            NewTextHeadupSlot2(4, 0, 2000, -4, GetMiscString(125), 0);
        }
        return;
    }
    if (!CheckRecoverCost()) {
        return;
    }
    if (gCurrent_net_game->type == eNet_game_type_foxy) {
        if (gThis_net_player_index == gIt_or_fox) {
            gProgram_state.current_car.time_to_recover = GetRaceTime() + 5000;
            gRecover_timer = 0;
            gToo_late = 0;
            return;
        }
    } else {
        if (gCurrent_net_game->type != eNet_game_type_tag) {
            gProgram_state.current_car.time_to_recover = GetRaceTime() + 3000;
            gRecover_timer = 0;
            gToo_late = 0;
            return;
        }
        if (gThis_net_player_index != gIt_or_fox) {
            gProgram_state.current_car.time_to_recover = GetRaceTime() + 5000;
            gRecover_timer = 0;
            gToo_late = 0;
            return;
        }
    }
    gProgram_state.current_car.time_to_recover = GetRaceTime() + 1000;
    gRecover_timer = 0;
    gToo_late = 0;
}

// IDA: void __cdecl RecoverCar()
void RecoverCar() {
    LOG_TRACE("()");

    if (gNet_mode == eNet_mode_none || !gPalette_fade_time) {
        gRecover_car = 1;
    }
    gProgram_state.current_car.time_to_recover = 0;
}

// IDA: void __cdecl CheckMapRenderMove()
void CheckMapRenderMove() {
    int shift_down;
    int amount;
    float old_x;
    float old_y;
    LOG_TRACE("()");

    old_y = gMap_render_y;
    old_x = gMap_render_x;
    if (gMap_mode) {
        amount = gFrame_period * .1f;
        if (KeyIsDown(30)) {
            gMap_render_y -= amount;
        } else if (KeyIsDown(31)) {
            gMap_render_y += amount;
        }
        if (KeyIsDown(32)) {
            gMap_render_x -= amount;
        } else if (KeyIsDown(33)) {
            gMap_render_x += amount;
        }
        if (gMap_render_x != old_x || gMap_render_y != old_y) {
            SetIntegerMapRenders();
            if (gMap_render_x_i < gCurrent_graf_data->map_render_x_marg) {
                if (gReal_graf_data_index == 0) {
                    gMap_render_x = (gCurrent_graf_data->map_render_x_marg + 3) & ~3;
                } else {
                    gMap_render_x = ((gCurrent_graf_data->map_render_x_marg + 3) & ~3) / 2;
                }
            }
            if (gMap_render_y_i < gCurrent_graf_data->map_render_y_marg) {
                if (gReal_graf_data_index == 0) {
                    gMap_render_y = (gCurrent_graf_data->map_render_y_marg + 1) & ~1;
                } else {
                    gMap_render_y = (((gCurrent_graf_data->map_render_y_marg + 1) & ~1) - 40) / 2;
                }
            }
            if (gBack_screen->width - gCurrent_graf_data->map_render_x_marg - gMap_render_width_i < gMap_render_x_i) {
                if (gReal_graf_data_index == 0) {
                    gMap_render_x = (gBack_screen->width - gCurrent_graf_data->map_render_x_marg - gMap_render_width_i) & ~3;
                } else {
                    gMap_render_x = ((gBack_screen->width - gCurrent_graf_data->map_render_x_marg - gMap_render_width_i) & ~3) / 2;
                }
            }
            if (gBack_screen->height - gCurrent_graf_data->map_render_y_marg - gMap_render_height_i < gMap_render_y_i) {
                if (gReal_graf_data_index == 0) {
                    gMap_render_y = (gBack_screen->height - gCurrent_graf_data->map_render_y_marg - gMap_render_height_i) & ~1;
                } else {
                    gMap_render_y = (((gBack_screen->height - gCurrent_graf_data->map_render_y_marg - gMap_render_height_i) & ~3) - 40 ) / 2;
                }
            }
            SetIntegerMapRenders();
            AdjustRenderScreenSize();
        }
    }
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

    if (gProgram_state.current_car.time_to_recover) {
        if (gProgram_state.current_car.knackered) {
            gProgram_state.current_car.time_to_recover = 0;
        } else {
            time = (gProgram_state.current_car.time_to_recover - pEndFrameTime + 1000) / 1000;
            sprintf(s, "%s %d %s", GetMiscString(97), time, time > 1 ? GetMiscString(99) : GetMiscString(98));
            if (!gToo_late) {
                NewTextHeadupSlot2(4, 0, 2000, -4, s, 0);
            }
            if (gProgram_state.current_car.time_to_recover <= pEndFrameTime) {
                RecoverCar();
            }
        }
    }
    if (gNet_mode) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gThis_net_player_index != i && gNet_players[i].car->time_to_recover && gNet_players[i].car->time_to_recover <= pEndFrameTime) {
                gNet_players[i].player_status = ePlayer_status_recovering;
                gNet_players[i].car->message.type = 32;
                gNet_players[i].car->message.time = pEndFrameTime;
                ExplodeCar(gNet_players[i].car);
                gNet_players[i].car->time_to_recover = 0;
            }
        }
    }
}

// IDA: void __usercall LoseSomePSPowerups(int pNumber@<EAX>)
void LoseSomePSPowerups(int pNumber) {
    int index;
    LOG_TRACE("(%d)", pNumber);

    if (gNet_mode != eNet_mode_none && pNumber > 0) {
        while (pNumber--) {
            index = IRandomBetween(0, 2);
            if (gProgram_state.current_car.power_up_levels[index]) {
                gProgram_state.current_car.power_up_levels[index]--;
            }
        }
    }
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

    car = GetCarSpec(eVehicle_self, 0);
    CheckMapRenderMove();
    CheckHorns();
    CheckForBeingOutOfThisWorld();
    if (gPalette_fade_time) {
        SortOutRecover(car);
    } else if (gNet_mode && NetGetPlayerStatus() == ePlayer_status_recovering) {
        NetPlayerStatusChanged(ePlayer_status_racing);
    }
    if ((!gAuto_repair && !KeyIsDown(44))
        || gRace_finished
        || gProgram_state.current_car.knackered
        || gWait_for_it
        || gEntering_message) {
        gRepair_last_time = 0;
        stopped_repairing = 0;
        total_repair_cost = 0;
        total_difference = 0;
        if (sound_tag) {
            for (i = 0; i < 10 && S3SoundStillPlaying(sound_tag); ++i) {
                DRS3StopSound(sound_tag);
            }
            sound_tag = 0;
        }
    } else {
        if (!gAuto_repair && !gRepair_last_time && GetTotalTime() - gLast_repair_time < 1200) {
            gAuto_repair = 1;
        }
        gLast_repair_time = GetTotalTime();
        gRepair_last_time = 1;
        if (!NeedToExpandBoundingBox) {
            if (gFree_repairs
                || gNet_mode == eNet_mode_none
                || gProgram_state.credits_earned - gProgram_state.credits_lost >= 1) {
                bodywork_repair_amount = RepairCar(gProgram_state.current_car.car_ID, gFrame_period, &amount);
                NeedToExpandBoundingBox = bodywork_repair_amount > 0;
                cost = 0;
                for (j = 0; j < COUNT_OF(gProgram_state.current_car.damage_units); j++) {
                    old_level = gProgram_state.current_car.damage_units[j].damage_level;
                    if (amount == 0.0) {
                        new_level = 0;
                    } else {
                        new_level = ((double)gProgram_state.current_car.damage_units[j].damage_level
                            - floor(bodywork_repair_amount / amount * (double)gProgram_state.current_car.damage_units[j].damage_level));
                    }
                    if (new_level >= 0) {
                        if (new_level < 100) {
                            gProgram_state.current_car.damage_units[j].damage_level = new_level;
                        } else {
                            gProgram_state.current_car.damage_units[j].damage_level = 99;
                        }
                    } else {
                        gProgram_state.current_car.damage_units[j].damage_level = 0;
                    }
                    gProgram_state.current_car.damage_units[j].smoke_last_level = gProgram_state.current_car.damage_units[j].damage_level;
                    if (gNet_mode) {
                        ts = gNet_repair_cost[gCurrent_net_game->type];
                    } else {
                        ts = gRepair_cost[gProgram_state.skill_level];
                    }
                    cost = (old_level - gProgram_state.current_car.damage_units[j].damage_level) * ts + cost;
                    total_difference += old_level - new_level;
                }
                if (!gFree_repairs) {
                    LoseSomePSPowerups(total_difference / 100);
                }
                total_difference %= 100;
                cost = 10 * (cost / 10);
                if (((!total_repair_cost && cost) || bodywork_repair_amount != 0.0f) && !sound_tag) {
                    sound_tag = DRS3StartSound(gIndexed_outlets[1], 5200);
                }
                if (gProgram_state.current_car.num_smoke_columns) {
                    StopCarSmoking(&gProgram_state.current_car);
                }
                if (!cost && bodywork_repair_amount == 0.0) {
                    gAuto_repair = 0;
                }
                if (!gFree_repairs) {
                    cost += SpendCredits(cost);
                }
                total_repair_cost += cost;
                if (total_repair_cost) {
                    if (gFree_repairs) {
                        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(4));
                    } else {
                        sprintf(s, "%s %d", GetMiscString(5), total_repair_cost);
                        NewTextHeadupSlot(4, 0, 1000, -4, s);
                    }
                }
            } else {
                if (!stopped_repairing) {
                    NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(95));
                }
                gAuto_repair = 0;
                stopped_repairing = 1;
            }
        }
    }
    if (NeedToExpandBoundingBox) {
        NeedToExpandBoundingBox = ExpandBoundingBox(&gProgram_state.current_car) == 0;
    }
    if (!gRecover_car || gProgram_state.current_car.knackered) {
        gHad_auto_recover = 0;
    } else if (CheckRecoverCost()) {
        gRecover_timer = 0;
        SetFlipUpCar(car);
        if (gNet_mode != eNet_mode_none) {
            NewTextHeadupSlot(4, 0, 1500, -4, " ");
        }
        if (gRecovery_voucher_count != 0) {
            gRecovery_voucher_count--;
            sprintf(s, "%s", GetMiscString(48));
            NewTextHeadupSlot(4, 0, 1500, -4, s);
        } else {
            if (gNet_mode) {
                cost = gNet_recovery_cost[gCurrent_net_game->type];
            } else {
                cost = gRecovery_cost[gProgram_state.skill_level];
            }
            SpendCredits(cost);
            if (gNet_mode) {
                cost = gNet_recovery_cost[gCurrent_net_game->type];
            } else {
                cost = gRecovery_cost[gProgram_state.skill_level];
            }
            sprintf(s, "%s %d", GetMiscString(7), cost);
            NewTextHeadupSlot(4, 0, 1500, -4, s);
            LoseSomePSPowerups(2);
        }
        CancelPendingCunningStunt();
        PipeSingleSpecial(ePipe_special_fade);
    }
    gRecover_car = 0;
}

// IDA: int __cdecl CheckRecoverCost()
int CheckRecoverCost() {
    LOG_TRACE("()");

    if (gProgram_state.current_car.knackered
        || gNet_mode == eNet_mode_none
        || (gProgram_state.credits_earned - gProgram_state.credits_lost) >= gNet_recovery_cost[gCurrent_net_game->type]
        || gRecovery_voucher_count) {
        return 1;
    }
    gProgram_state.credits_earned = 0;
    gProgram_state.credits_lost = 0;
    NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(96));
    DoFancyHeadup(18);
    KnackerThisCar(&gProgram_state.current_car);
    SendGameplayToHost(eNet_gameplay_suicide, 0, 0, 0, 0);
    return 0;
}

// IDA: void __usercall SortOutRecover(tCar_spec *pCar@<EAX>)
void SortOutRecover(tCar_spec* pCar) {
    int the_time;
    int val;
    static int old_time;
    LOG_TRACE("(%p)", pCar);

    the_time = GetRaceTime() - gPalette_fade_time;
    if (the_time < 0) {
        gPalette_fade_time = 0;
        old_time = 0;
    }
    if (the_time < 500) {
        val = 256 - (the_time * 256) / 500;
    } else {
        if (old_time < 500) {
            FlipUpCar(pCar);
            PipeSingleSpecial(ePipe_special_fade);
        }
        pCar->doing_nothing_flag = 1;
        val = ((the_time - 1000) * 256) / 500;
        if (val >= 256) {
            val = 256;
            gPalette_fade_time = 0;
            old_time = 0;
            pCar->doing_nothing_flag = 0;
        }
    }
    if (val <= 0) {
        val = 0;
    }
    SetFadedPalette(val);
    old_time = the_time;
}

// IDA: void __usercall SetFlipUpCar(tCar_spec *pCar@<EAX>)
void SetFlipUpCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    if (gNet_mode != eNet_mode_none && pCar->driver == eDriver_local_human) {
        DisableCar(pCar);
        gPalette_fade_time = GetRaceTime();
        NetPlayerStatusChanged(ePlayer_status_recovering);
    } else {
        FlipUpCar(pCar);
    }
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

    count = 0;
    if (car->driver == eDriver_local_human && gNet_mode == eNet_mode_none) {
        FadePaletteDown();
        while (KeyIsDown(44)) {
            ;
        }
    }
    car->doing_nothing_flag = 0;
    EnableCar(car);
    new_pos = 1;
    for (i = 0; i < 4; ++i) {
        if (car->susp_height[i >> 1] <= car->oldd[i]) {
            new_pos = 0;
        }
    }
    do {
        tv.v[0] = car->car_master_actor->t.t.mat.m[3][0] - car->last_safe_positions[0].m[3][0];
        tv.v[1] = car->car_master_actor->t.t.mat.m[3][1] - car->last_safe_positions[0].m[3][1];
        tv.v[2] = car->car_master_actor->t.t.mat.m[3][2] - car->last_safe_positions[0].m[3][2];
        if (BrVector3LengthSquared(&tv) > 8.3015966) {
            new_pos = 0;
        }
        BrMatrix34Copy(&car->car_master_actor->t.t.mat, &car->last_safe_positions[new_pos]);
        BrMatrix34Copy(&car->oldmat, &car->last_safe_positions[new_pos]);
        BrMatrix34Copy(&car->old_frame_mat, &car->oldmat);
        car->oldmat.m[3][0] = car->oldmat.m[3][0] * WORLD_SCALE;
        car->oldmat.m[3][1] = car->oldmat.m[3][1] * WORLD_SCALE;
        car->oldmat.m[3][2] = car->oldmat.m[3][2] * WORLD_SCALE;
        dir.v[0] = 0.0;
        dir.v[1] = 0.28985506;
        dir.v[2] = 0.0;
        FindFace(&car->car_master_actor->t.t.euler.t, &dir, &tv, &t, &material);
        if (t > 1.0) {
            car->car_master_actor->t.t.mat.m[3][0] += dir.v[0];
            car->car_master_actor->t.t.mat.m[3][1] += dir.v[1];
            car->car_master_actor->t.t.mat.m[3][2] += dir.v[2];
            car->oldmat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * WORLD_SCALE;
            car->oldmat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * WORLD_SCALE;
            car->oldmat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * WORLD_SCALE;
            car->old_frame_mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0];
            car->old_frame_mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1];
            car->old_frame_mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2];
        }
        tv.v[0] = 0.0;
        tv.v[1] = 0.0;
        tv.v[2] = -0.001;
        BrMatrix34ApplyV(&car->v, &tv, &car->car_master_actor->t.t.mat);
        car->omega.v[0] = 0.0;
        car->omega.v[1] = 0.0;
        car->omega.v[2] = 0.0;
        car->direction.v[0] = -car->oldmat.m[2][0];
        car->direction.v[1] = -car->oldmat.m[2][1];
        car->direction.v[2] = -car->oldmat.m[2][2];
        for (i = 0; i <= new_pos; i++) {
            for (j = 0; j < 4; j++) {
                BrMatrix34Copy(&car->last_safe_positions[j], &car->last_safe_positions[j + 1]);
            }
        }
        for (l = 0; l < 10; l++) {
            BrVector3Scale(&car->old_norm, &car->old_norm, 0.072463766);
            BrMatrix34ApplyV(&tv, &car->old_norm, &car->car_master_actor->t.t.mat);
            car->car_master_actor->t.t.mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] + tv.v[0];
            car->car_master_actor->t.t.mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] + tv.v[1];
            car->car_master_actor->t.t.mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] + tv.v[2];
            car->oldmat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * WORLD_SCALE;
            car->oldmat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * WORLD_SCALE;
            car->oldmat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * WORLD_SCALE;
            car->old_frame_mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0];
            car->old_frame_mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1];
            car->old_frame_mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2];
            if (TestForCarInSensiblePlace(car)) {
                break;
            }
        }
        count++;
    } while (l == 10 && count < 3);
    car->oldmat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * WORLD_SCALE;
    car->oldmat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * WORLD_SCALE;
    car->oldmat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * WORLD_SCALE;
    car->curvature = 0.0;
    for (j = 0; j < 4; ++j) {
        car->oldd[j] = car->ride_height;
    }
    car->revs = 0.0;
    car->gear = 0;
    car->auto_special_volume = 0;
    if (car->driver == eDriver_local_human) {
        InitialiseExternalCamera();
        PositionExternalCamera(car, 100u);
    }
}

// IDA: void __usercall GetPowerup(int pNum@<EAX>)
void GetPowerup(int pNum) {
    LOG_TRACE("()");

    // FIXME: remove unittest variables from dethrace
    _unittest_controls_lastGetPowerup = pNum;

    GotPowerup(&gProgram_state.current_car, pNum);
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

    if (gKev_keys[i].action_proc) {
        if (gNet_mode) {
            if (gKev_keys[i].num == 0xA11EE75D) {
                strcpy(s, gNet_players[gThis_net_player_index].player_name);
                strcat(s, " ");
                strcat(s, GetMiscString(225));
                NetSendHeadupToEverybody(s);
                gKev_keys[i].action_proc(gKev_keys[i].num);
            } else {
                strcpy(s, gNet_players[gThis_net_player_index].player_name);
                strcat(s, " ");
                strcat(s, GetMiscString(224));
                NetSendHeadupToAllPlayers(s);
            }
        } else {
            gKev_keys[i].action_proc(gKev_keys[i].num);
        }
    }
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

    c = &gProgram_state.current_car;

    memset(&keys, 0, sizeof(tCar_controls));
    joystick.left = -1;
    joystick.right = -1;
    joystick.acc = -1;
    joystick.dec = -1;
    if (gEntering_message) {
        memset(&c->keys, 0, sizeof(tCar_controls));
        c->joystick.left = -1;
        c->joystick.right = -1;
        c->joystick.acc = -1;
        c->joystick.dec = -1;
    } else {
        if (gKey_mapping[46] >= 115 || gKey_mapping[47] >= 115) {
            joystick.left = gJoy_array[gKey_mapping[46] - 115];
            joystick.right = gJoy_array[gKey_mapping[47] - 115];
            if (joystick.left < 0 && joystick.right < 0) {
                joystick.left = 0;
            }
        } else {
            if (KeyIsDown(46)) {
                keys.left = 1;
            }
            if (KeyIsDown(47)) {
                keys.right = 1;
            }
        }
        if (KeyIsDown(12)) {
            keys.holdw = 1;
        }
        if (KeyIsDown(53) || gRace_finished) {
            if (!gInstant_handbrake || gRace_finished) {
                keys.brake = 1;
            } else {
                BrakeInstantly();
            }
        }
        if (gKey_mapping[48] < 115) {
            if (KeyIsDown(48) && !gRace_finished && !c->knackered && !gWait_for_it) {
                keys.acc = 1;
            }
        } else {
            joystick.acc = gJoy_array[gKey_mapping[48] - 115];
            if (joystick.acc > 0xFFFF) {
                joystick.acc = 0xFFFF;
            }
        }
        if (gKey_mapping[49] < 115) {
            if (KeyIsDown(49) && !gRace_finished && !c->knackered && !gWait_for_it) {
                keys.dec = 1;
            }
        } else {
            joystick.dec = gJoy_array[gKey_mapping[49] - 115];
            if (joystick.dec > 0xFFFF) {
                joystick.dec = 0xFFFF;
            }
        }
        if (KeyIsDown(55) && c->gear >= 0) {
            keys.change_down = 1;
            c->just_changed_gear = 1;
            if (keys.acc || joystick.acc > 32000) {
                c->traction_control = 0;
            } else if (c->gear > 1 && !c->keys.change_down) {
                --c->gear;
            }
            if (gCountdown && !c->keys.change_down) {
                JumpTheStart();
            }
        }
        if (gCar_flying) {
            if (KeyIsDown(13)) {
                keys.up = 1;
            }
            if (KeyIsDown(11)) {
                keys.down = 1;
            }
        }
        if (KeyIsDown(58)) {
            if (!gEntering_message) {
                keys.horn = 1;
            }
        }
        c->keys = keys;
        c->joystick = joystick;
    }
}

// IDA: void __usercall PollCameraControls(tU32 pTime_difference@<EAX>)
void PollCameraControls(tU32 pTime_difference) {
    int flag;
    int left;
    int right;
    int swirl_mode;
    int up_and_down_mode;
    int going_up;
    static int last_swirl_mode = 0;
    LOG_TRACE("(%d)", pTime_difference);

    flag = 0;
    swirl_mode = gRace_finished && !gAction_replay_mode && (&gProgram_state.current_car == gCar_to_view || gCar_to_view->knackered);
    up_and_down_mode = swirl_mode && !gCamera_has_collided;
    going_up = gCamera_zoom > 1.0;
    if (last_swirl_mode != swirl_mode) {
        if (swirl_mode) {
            SaveCameraPosition(0);
        } else {
            RestoreCameraPosition(0);
        }
        last_swirl_mode = swirl_mode;
    }
    if (!gMap_mode && !gProgram_state.cockpit_on && (!gAction_replay_mode || gAction_replay_camera_mode <= eAction_replay_standard)) {
        if (KeyIsDown(31) || (up_and_down_mode && !going_up)) {
            gCamera_zoom = (double)pTime_difference * TIME_CONV_THING / (double)(2 * swirl_mode + 1) + gCamera_zoom;
            if (gCamera_zoom > 2.0f) {
                gCamera_zoom = 2.0f;
            }
            if (up_and_down_mode && gCamera_zoom > 1.0f) {
                gCamera_zoom = 1.0f;
            }
        }
        if (KeyIsDown(30) || (up_and_down_mode && going_up)) {
            gCamera_zoom = gCamera_zoom - (double)pTime_difference * TIME_CONV_THING / (double)(2 * swirl_mode + 1);
            if (gCamera_zoom < 0.1) {
                gCamera_zoom = 0.1;
                if (up_and_down_mode) {
                    if (gCamera_zoom < 1.0f) {
                        gCamera_zoom = 1.0f;
                    }
                }
            }
        }
        if (swirl_mode && gProgram_state.current_car.speedo_speed < 0.001449275362318841) {
            left = 1;
            right = 0;
        } else {
            left = KeyIsDown(32);
            right = KeyIsDown(33);
        }

        if ((gCamera_sign ? left : right)) {
            if (!gCamera_reset) {
                gCamera_yaw += BrDegreeToAngle(pTime_difference * 0.05f);
            }
            flag = 1;
        }
        if ((gCamera_sign ? right : left)) {
            if (!gCamera_reset) {
                gCamera_yaw -= BrDegreeToAngle(pTime_difference * 0.05f);
            }
            if (flag) {
                gCamera_yaw = 0;
                gCamera_reset = 1;
            }
        } else if (!flag) {
            gCamera_reset = 0;
        }
    }
}

// IDA: void __usercall SetFlag2(int i@<EAX>)
void SetFlag2(int i) {
    LOG_TRACE("(%d)", i);

    gAllow_car_flying = 1;
    ToggleFlying();
    gAllow_car_flying = gCar_flying;
}

// IDA: void __cdecl ToggleFlying()
void ToggleFlying() {
    LOG_TRACE("()");

    if (gAllow_car_flying && gNet_mode == eNet_mode_none) {
        gCar_flying = !gCar_flying;
        if (gCar_flying) {
            NewTextHeadupSlot(4, 0, 500, -4, "We have lift off!!");
        } else {
            NewTextHeadupSlot(4, 0, 500, -4, "Back down to Earth");
        }
    } else {
        gCar_flying = 0;
    }
}

// IDA: void __cdecl ToggleInvulnerability()
void ToggleInvulnerability() {
    LOG_TRACE("()");

    gProgram_state.current_car.invulnerable = !gProgram_state.current_car.invulnerable;
    if (gProgram_state.current_car.invulnerable) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(0));
    } else {
        NewTextHeadupSlot(4, 0, 1000, -4, "Vulnerability returns!");
    }
}

// IDA: void __cdecl MoreTime()
void MoreTime() {
    LOG_TRACE("()");

    AwardTime(30);
}

// IDA: void __cdecl MuchMoreTime()
void MuchMoreTime() {
    LOG_TRACE("()");

    AwardTime(300);
}

// IDA: void __cdecl ToggleTimerFreeze()
void ToggleTimerFreeze() {
    LOG_TRACE("()");
    
    gFreeze_timer = !gFreeze_timer;
    if (gFreeze_timer) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(1));
    } else {
        NewTextHeadupSlot(4, 0, 1000, -4, "Timer thawed out");
    }
}

// IDA: void __cdecl EarnDosh()
void EarnDosh() {
    LOG_TRACE("()");

    EarnCredits(5000);
}

// IDA: void __cdecl LoseDosh()
void LoseDosh() {
    LOG_TRACE("()");

    EarnCredits(-5000);
}

// IDA: void __cdecl ToggleMap()
void ToggleMap() {
    static int old_indent;
    static int was_in_cockpit;
    LOG_TRACE("()");

    if (!gMap_mode) {
        if (!gAction_replay_mode) {
            if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox) {
                NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(214));
            } else if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox) {
                NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(215));
            } else {
                old_indent = gRender_indent;
                gRender_indent = 0;
                was_in_cockpit = gProgram_state.cockpit_on;
                if (gProgram_state.cockpit_on) {
                    ToggleCockpit();
                }
                gMap_mode = PDGetTotalTime();
            }
        }
    } else {
        gMap_mode = 0;
        gRender_indent = old_indent;
        if (was_in_cockpit) {
            ToggleCockpit();
        }
    }
    AdjustRenderScreenSize();
}

// IDA: int __cdecl HornBlowing()
int HornBlowing() {
    LOG_TRACE("()");

    return gProgram_state.current_car.keys.horn;
}

// IDA: void __cdecl ToggleArrow()
void ToggleArrow() {
    static br_actor* old_actor;
    LOG_TRACE("()");

    return;

    if (gArrow_mode) {
        gProgram_state.current_car.car_model_actors[gProgram_state.current_car.principal_car_actor].actor = old_actor;
        BrActorRemove(gArrow_actor);
        BrActorAdd(gProgram_state.current_car.car_master_actor, old_actor);
        gArrow_mode = 0;
        if (gInfo_on) {
            ToggleInfo();
        }
    } else {
        old_actor = gProgram_state.current_car.car_model_actors[gProgram_state.current_car.principal_car_actor].actor;
        BrActorRemove(old_actor);
        BrActorAdd(gProgram_state.current_car.car_master_actor, gArrow_actor);
        gProgram_state.current_car.car_model_actors[gProgram_state.current_car.principal_car_actor].actor = gArrow_actor;
        gArrow_mode = 1;
        if (!gInfo_on) {
            ToggleInfo();
        }
    }
}

// IDA: int __cdecl GetRecoverVoucherCount()
int GetRecoverVoucherCount() {
    LOG_TRACE("()");

    return gRecovery_voucher_count;
}

// IDA: void __usercall AddVouchers(int pCount@<EAX>)
void AddVouchers(int pCount) {
    LOG_TRACE("(%d)", pCount);

    gRecovery_voucher_count += pCount;
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

    DRS3StopAllOutletSounds();
    DisposeSoundSources();
    gSound_detail_level = pLevel;
    InitSound();
    InitSoundSources();
}

// IDA: int __cdecl GetSoundDetailLevel()
int GetSoundDetailLevel() {
    LOG_TRACE("()");

    return gSound_detail_level;
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
    PrintScreen();
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
    STUB_ONCE();
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

    for (i = 0; i < COUNT_OF(gAbuse_text); i++) {
        if (gAbuse_text[i] != NULL) {
            BrMemFree(gAbuse_text[i]);
        }
    }
}
