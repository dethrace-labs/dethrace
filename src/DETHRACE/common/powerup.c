#include "powerup.h"
#include "brender/brender.h"
#include "car.h"
#include "controls.h"
#include "crush.h"
#include "displays.h"
#include "errors.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/trace.h"
#include "input.h"
#include "loading.h"
#include "netgame.h"
#include "network.h"
#include "opponent.h"
#include "pedestrn.h"
#include "piping.h"
#include "pratcam.h"
#include "sound.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>

tGot_proc* gGot_procs[34] = {
    GotCredits,
    SetPedSpeed,
    SetPedSize,
    SetPedExplode,
    PickAtRandom,
    SetInvulnerability,
    SetFreeRepairs,
    DoInstantRepair,
    FreezeTimer,
    SetEngineFactor,
    SetUnderwater,
    GotTimeOrPower,
    TrashBodywork,
    TakeDrugs,
    SetOpponentsSpeed,
    SetCopsSpeed,
    SetGravity,
    SetPinball,
    SetWallclimb,
    SetBouncey,
    SetSuspension,
    SetTyreGrip,
    SetDamageMultiplier,
    SetHades,
    SetBlindPedestrians,
    PedestrianRespawn,
    GotVouchers,
    SetMassMultiplier,
    SetInstantHandbrake,
    ShowPedestrians,
    HitMine,
    SetProximity,
    SetPedHarvest,
    SetVesuvianCorpses,
};
tLose_proc* gLose_procs[34] = {
    NULL,
    ResetPedSpeed,
    ResetPedSize,
    ResetPedExplode,
    NULL,
    ResetInvulnerability,
    ResetFreeRepairs,
    NULL,
    UnfreezeTimer,
    ResetEngineFactor,
    ResetUnderwater,
    NULL,
    NULL,
    PukeDrugsBackUp,
    ResetOpponentsSpeed,
    ResetCopsSpeed,
    ResetGravity,
    ResetPinball,
    ResetWallclimb,
    ResetBouncey,
    ResetSuspension,
    ResetTyreGrip,
    ResetDamageMultiplier,
    ResetHades,
    ResetBlindPedestrians,
    NULL,
    NULL,
    ResetMassMultiplier,
    ResetInstantHandbrake,
    HidePedestrians,
    NULL,
    ResetProximity,
    ResetPedHarvest,
    ResetVesuvianCorpses,
};
tPeriodic_proc* gPeriodic_procs[34] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    TheEffectsOfDrugs,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    DoBouncey,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MungeVouchers,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};
tU32* gReal_render_palette;
char* gFizzle_names[3] = { "CIRCLES.PIX", "SQUARES.PIX", "DIAMONDS.PIX" };
br_vector3 gZero_v__powerup; // suffix added to avoid duplicate symbol
int gPed_harvest_sounds[4] = { 4010, 4012, 4030, 4032 };
tHeadup_icon gIcon_list[20];
br_pixelmap* gFizzle_in[3];
int gNumber_of_powerups;
int gFizzle_height;
int gNumber_of_icons;
tPowerup* gPowerup_array;

#define GET_POWERUP_INDEX(POWERUP) (((POWERUP)-gPowerup_array) / sizeof(tPowerup))

// IDA: void __usercall LosePowerupX(tPowerup *pThe_powerup@<EAX>, int pTell_net_players@<EDX>)
void LosePowerupX(tPowerup* pThe_powerup, int pTell_net_players) {
    int i;
    tNet_message* the_message;
    LOG_TRACE("(%p, %d)", pThe_powerup, pTell_net_players);

    if (pThe_powerup->got_time != 0 && pThe_powerup->lose_proc != NULL) {
        pThe_powerup->lose_proc(pThe_powerup, pThe_powerup->car);
    }
    if (gNet_mode != eNet_mode_none) {
        the_message = NetBuildMessage(21, 0);
        the_message->contents.data.powerup.event = ePowerup_lost;
        the_message->contents.data.powerup.player = gLocal_net_ID;
        the_message->contents.data.powerup.event = GET_POWERUP_INDEX(pThe_powerup);
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, the_message, NULL);
    }
    for (i = 0; i < gNumber_of_icons; i++) {
        if (gIcon_list[i].powerup == pThe_powerup) {
            gIcon_list[i].fizzle_stage = 3;
            gIcon_list[i].fizzle_direction = -1;
            gIcon_list[i].fizzle_start = GetTotalTime();
            break;
        }
    }
    pThe_powerup->got_time = 0;
}

// IDA: void __usercall LosePowerup(tPowerup *pThe_powerup@<EAX>)
void LosePowerup(tPowerup* pThe_powerup) {
    LOG_TRACE("(%p)", pThe_powerup);

    LosePowerupX(pThe_powerup, 1);
}

// IDA: void __usercall LoseAllSimilarPowerups(tPowerup *pThe_powerup@<EAX>)
void LoseAllSimilarPowerups(tPowerup* pThe_powerup) {
    int i;
    tPowerup* the_powerup;
    LOG_TRACE("(%p)", pThe_powerup);

    for (i = 0, the_powerup = gPowerup_array; i < gNumber_of_powerups; i++, the_powerup++) {
        if (the_powerup != pThe_powerup && the_powerup->got_time != 0) {
            if ((pThe_powerup->group_inclusion & the_powerup->group_inclusion) != 0) {
                LosePowerup(the_powerup);
            }
        }
    }
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

    if (pIndex < 0 || pIndex >= gNumber_of_powerups) {
        return -1;
    }
    the_powerup = &gPowerup_array[pIndex];
    if (the_powerup->type == ePowerup_dummy) {
        return -1;
    }
    if (the_powerup->got_proc == NULL) {
        NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(190));
        return -1;
    }
    original_index = pIndex;
    if (((gProgram_state.sausage_eater_mode || gTotal_peds < 2)
            && (strstr(the_powerup->message, "Ped") != NULL
                || strstr(the_powerup->message, "ped") != NULL
                || strstr(the_powerup->message, "corpses") != NULL))
        || (gNet_mode != eNet_mode_none && the_powerup->net_type == eNet_powerup_inappropriate)) {
        pIndex = 0;
        the_powerup = &gPowerup_array[pIndex];
    }
    the_powerup->current_value = -1;
    LoseAllSimilarPowerups(the_powerup);
    ps_power = gNet_mode != eNet_mode_none && the_powerup->got_proc == GotTimeOrPower;
    if (the_powerup->message[0] != '\0' && pDisplay_headup && !ps_power) {
        strcpy(s, the_powerup->message);
        s2 = s;
        if (the_powerup->got_proc == FreezeTimer) {
            s2 = strtok(s, "/");
            if (gFreeze_timer) {
                s2 = strtok(NULL, "/");
            }
        }
        NewTextHeadupSlot(4, 0, 3000, -4, s2);
    }
    the_powerup->car = pCar;
    if (the_powerup->got_proc != NULL) {
        pIndex = the_powerup->got_proc(the_powerup, pCar);
    }
    if (pCar->driver == eDriver_non_car_unused_slot || pCar->driver == eDriver_non_car) {
        return pIndex;
    }
    if (the_powerup->type == ePowerup_timed) {
        the_powerup->got_time = GetTotalTime();
        if (pTell_net_players) {
            the_powerup->lose_time = the_powerup->got_time + the_powerup->duration;
        } else {
            the_powerup->lose_time = the_powerup->got_time + pTime_left;
        }
        gProgram_state.current_car.powerups[pIndex] = the_powerup->lose_time;
    } else if (the_powerup->type == ePowerup_whole_race) {
        the_powerup->got_time = GetTotalTime();
        gProgram_state.current_car.powerups[pIndex] = -1;
    }
    if (the_powerup->prat_cam_event >= 0) {
        PratcamEvent(the_powerup->prat_cam_event);
    }
    if (gNet_mode != eNet_mode_none && pTell_net_players && pIndex == original_index && !ps_power) {
        the_message = NetBuildMessage(21, 0);
        the_message->contents.data.powerup.event = ePowerup_gained;
        the_message->contents.data.powerup.player = gLocal_net_ID;
        the_message->contents.data.powerup.powerup_index = pIndex;
        if (the_powerup->type == ePowerup_timed) {
            the_message->contents.data.powerup.time_left = the_powerup->duration;
        } else {
            the_message->contents.data.powerup.time_left = 0;
        }
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, the_message, NULL);
    }
    if (the_powerup->type != ePowerup_instantaneous && the_powerup->icon != NULL) {
        for (i = 0; i < gNumber_of_icons; i++) {
            if (gIcon_list[i].powerup == the_powerup) {
                gIcon_list[i].fizzle_stage = 4;
                return pIndex;
            }
        }
        if (gNumber_of_icons != COUNT_OF(gIcon_list)) {
            gIcon_list[gNumber_of_icons].powerup = the_powerup;
            gIcon_list[gNumber_of_icons].fizzle_stage = 0;
            gIcon_list[gNumber_of_icons].fizzle_direction = 1;
            gIcon_list[gNumber_of_icons].fizzle_start = GetTotalTime();
            gNumber_of_icons++;
        }
    }
    return pIndex;
}

// IDA: int __usercall GotPowerup@<EAX>(tCar_spec *pCar@<EAX>, int pIndex@<EDX>)
int GotPowerup(tCar_spec* pCar, int pIndex) {
    LOG_TRACE("(%p, %d)", pCar, pIndex);

    return GotPowerupX(pCar, pIndex, 1, 1, 0);
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

    for (i = 0; i < COUNT_OF(gFizzle_in); i++) {
        gFizzle_in[i] = LoadPixelmap(gFizzle_names[i]);
    }

    gFizzle_height = gFizzle_in[0]->height / 4;
    PathCat(the_path, gApplication_path, "POWERUP.TXT");
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        FatalError(kFatalError_LoadResolutionIndependentFile);
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
        the_powerup->float_params = BrMemAllocate(sizeof(float) * the_powerup->number_of_float_params, kMem_powerup_float_parms);
        for (j = 0; j < the_powerup->number_of_float_params; j++) {
            the_powerup->float_params[j] = GetAFloat(f);
        }
        the_powerup->number_of_integer_params = GetAnInt(f);
        the_powerup->integer_params = BrMemAllocate(sizeof(int) * the_powerup->number_of_integer_params, kMem_powerup_int_parms);
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

    for (i = 0, the_powerup = gPowerup_array; i < gNumber_of_powerups; i++, the_powerup++) {
        the_powerup->got_time = 0;
        the_powerup->current_value = -1;
    }
}

// IDA: void __cdecl CloseDownPowerUps()
void CloseDownPowerUps() {
    int i;
    tPowerup* the_powerup;
    LOG_TRACE("()");

    for (i = 0, the_powerup = gPowerup_array; i < gNumber_of_powerups; i++, the_powerup++) {
        if (the_powerup->got_time != 0) {
            LosePowerup(the_powerup);
        }
    }
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

    y = gCurrent_graf_data->power_up_icon_y;
    for (i = 0; i < gNumber_of_icons && i < 5; i++) {
        the_icon = &gIcon_list[i];
        the_powerup = the_icon->powerup;
        if (the_powerup->icon == NULL) {
            continue;
        }
        y += gCurrent_graf_data->power_up_icon_y_pitch;
        if (the_icon->fizzle_stage < 4) {
            if (the_icon->fizzle_direction >= 0) {
                the_icon->fizzle_stage = (pTime - the_icon->fizzle_start) / 100;
            } else {
                the_icon->fizzle_stage = 3 - (pTime - the_icon->fizzle_start) / 100;
            }
            if (the_icon->fizzle_stage >= 5) {
                the_icon->fizzle_stage = 4;
            } else if (the_icon->fizzle_stage < 0) {
                memmove(the_icon, the_icon + 1, sizeof(tHeadup_icon) * (gNumber_of_icons - i - 1));
                gNumber_of_icons--;
                continue;
            }
        }
        if (the_icon->fizzle_stage >= 4) {
            DRPixelmapRectangleMaskedCopy(gBack_screen,
                gCurrent_graf_data->power_up_icon_x, y,
                the_powerup->icon, 0, 0, the_powerup->icon->width, the_powerup->icon->height);
            if (the_powerup->type == ePowerup_timed) {
                timer = the_powerup->lose_time - pTime;
                if (timer <= 0) {
                    timer = 0;
                }
                TimerString(timer, s, 0, 0);
                TransDRPixelmapText(gBack_screen,
                    gCurrent_graf_data->power_up_icon_countdown_x,
                    y + gCurrent_graf_data->power_up_icon_countdown_y_offset,
                    &gFonts[1], s, gCurrent_graf_data->power_up_icon_countdown_x + 30);
            } else if (the_powerup->current_value > 0) {
                sprintf(s, "%d", the_powerup->current_value);
                TransDRPixelmapText(gBack_screen,
                    gCurrent_graf_data->power_up_icon_countdown_x,
                    y + gCurrent_graf_data->power_up_icon_countdown_y_offset,
                    &gFonts[2], s, gCurrent_graf_data->power_up_icon_countdown_x + 30);
            }
        } else {
            fizzle_pix = gFizzle_in[the_powerup->fizzle_type];
            DRPixelmapRectangleMaskedCopy(gBack_screen,
                gCurrent_graf_data->power_up_icon_x, y, fizzle_pix, 0,
                the_icon->fizzle_stage * gFizzle_height, fizzle_pix->width, gFizzle_height);
        }
    }
}

// IDA: void __usercall DoPowerupPeriodics(tU32 pFrame_period@<EAX>)
void DoPowerupPeriodics(tU32 pFrame_period) {
    int i;
    tPowerup* the_powerup;
    tU32 the_time;
    LOG_TRACE("(%d)", pFrame_period);

    the_time = GetTotalTime();
    for (i = 0, the_powerup = gPowerup_array; i < gNumber_of_powerups; i++, the_powerup++) {
        if (the_powerup->got_time != 0) {
            if (the_powerup->type == ePowerup_timed && the_powerup->lose_time <= the_time) {
                LosePowerup(the_powerup);
            } else if (the_powerup->current_value == 0) {
                LosePowerup(the_powerup);
            } else if (the_powerup->periodic_proc != NULL) {
                the_powerup->periodic_proc(the_powerup, pFrame_period);
            }
        }
    }
}

// IDA: void __usercall GotPowerupN(int pN@<EAX>)
void GotPowerupN(int pN) {
    int modifiers;
    LOG_TRACE("(%d)", pN);

    modifiers = 0;
    if (PDKeyDown(KEY_SHIFT_ANY) != 0) {
        modifiers += 10;
    }
    if (PDKeyDown(KEY_ALT_ANY) != 0) {
        modifiers += 20;
    }
    if (PDKeyDown(KEY_CTRL_ANY) != 0) {
        modifiers += 40;
    }
    GotPowerup(&gProgram_state.current_car, modifiers + pN);
}

// IDA: void __cdecl GotPowerup0()
void GotPowerup0() {
    LOG_TRACE("()");

    GotPowerupN(0);
}

// IDA: void __cdecl GotPowerup1()
void GotPowerup1() {
    LOG_TRACE("()");

    GotPowerupN(1);
}

// IDA: void __cdecl GotPowerup2()
void GotPowerup2() {
    LOG_TRACE("()");

    GotPowerupN(2);
}

// IDA: void __cdecl GotPowerup3()
void GotPowerup3() {
    LOG_TRACE("()");

    GotPowerupN(3);
}

// IDA: void __cdecl GotPowerup4()
void GotPowerup4() {
    LOG_TRACE("()");

    GotPowerupN(4);
}

// IDA: void __cdecl GotPowerup5()
void GotPowerup5() {
    LOG_TRACE("()");

    GotPowerupN(5);
}

// IDA: void __cdecl GotPowerup6()
void GotPowerup6() {
    LOG_TRACE("()");

    GotPowerupN(6);
}

// IDA: void __cdecl GotPowerup7()
void GotPowerup7() {
    LOG_TRACE("()");

    GotPowerupN(7);
}

// IDA: void __cdecl GotPowerup8()
void GotPowerup8() {
    LOG_TRACE("()");

    GotPowerupN(8);
}

// IDA: void __cdecl GotPowerup9()
void GotPowerup9() {
    LOG_TRACE("()");

    GotPowerupN(9);
}

// IDA: int __usercall GotCredits@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int GotCredits(tPowerup* pPowerup, tCar_spec* pCar) {
    int credits;
    char s[256];
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        strcpy(s, pPowerup->message);
        strcat(s, " ");
        EarnCredits2((IRandomBetween(pPowerup->integer_params[0], pPowerup->integer_params[1]) / 100) * 100, s);
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ImprovePSPowerup(tCar_spec *pCar@<EAX>, int pIndex@<EDX>)
void ImprovePSPowerup(tCar_spec* pCar, int pIndex) {
    tNet_message* the_message;
    LOG_TRACE("(%p, %d)", pCar, pIndex);

    pCar->power_up_levels[pIndex]++;
    NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(175 + pIndex));
}

// IDA: int __usercall GotTimeOrPower@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int GotTimeOrPower(tPowerup* pPowerup, tCar_spec* pCar) {
    int time;
    int index;
    int i;
    int not_allowed_power;
    char s[256];
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (gNet_mode == eNet_mode_none) {
        if (pCar->driver == eDriver_local_human) {
            time = IRandomBetween(pPowerup->integer_params[0], pPowerup->integer_params[1]);
            AwardTime(time);
        }
    } else {
        not_allowed_power = (gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox)
            || (gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox);
        if (pCar->power_up_levels[0] < 4 || (pCar->power_up_levels[1] < 4 && !not_allowed_power) || pCar->power_up_levels[2] < 4) {
            for (i = 0; i < 50; i++) {
                if (not_allowed_power) {
                    index = PercentageChance(50) ? 0 : 2;
                } else {
                    index = IRandomBetween(0, 2);
                }
                if (pCar->power_up_levels[index] < 4) {
                    ImprovePSPowerup(pCar, index);
                    break;
                }
            }
        } else {
            NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(174));
        }
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetPedSpeed@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gPedestrian_speed_factor = pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetHades@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    PedMaterialFromHell();
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetHades(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetHades(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    ResetPedMaterial();
}

// IDA: int __usercall SetPedSize@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    br_scalar old_scale;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    old_scale = gPed_scale_factor;
    if (old_scale != pPowerup->float_params[0]) {
        if (gPed_scale_factor > 1.f) {
            gPed_scale_factor = pPowerup->float_params[0];
            PipeSingleSpecial(ePipe_special_giant_ped_off);
        } else {
            gPed_scale_factor = pPowerup->float_params[0];
            if (old_scale < 1.f) {
                PipeSingleSpecial(ePipe_special_min_ped_off);
            }
        }
        if (gPed_scale_factor > 1.f) {
            PipeSingleSpecial(ePipe_special_giant_ped_on);
            old_scale = gPed_scale_factor;
        } else {
            old_scale = gPed_scale_factor;
            if (gPed_scale_factor < 1.f) {
                PipeSingleSpecial(ePipe_special_min_ped_on);
                old_scale = gPed_scale_factor;
            }
        }
    }
    gPed_scale_factor = old_scale;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetPedExplode@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gExploding_pedestrians = 1;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetInvulnerability@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->invulnerable = 1;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetInvulnerability(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetInvulnerability(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->invulnerable = 0;
}

// IDA: int __usercall SetFreeRepairs@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gFree_repairs = 1;
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetFreeRepairs(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetFreeRepairs(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gFree_repairs = 0;
    }
}

// IDA: int __usercall SetBlindPedestrians@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gBlind_pedestrians = 1;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetBlindPedestrians(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetBlindPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gBlind_pedestrians = 0;
}

// IDA: int __usercall FreezeTimer@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int FreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gFreeze_timer = !gFreeze_timer;
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall UnfreezeTimer(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void UnfreezeTimer(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gFreeze_timer = 0;
    }
}

// IDA: int __usercall DoInstantRepair@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int DoInstantRepair(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        TotallyRepairCar();
    }
    return GET_POWERUP_INDEX(gPowerup_array);
}

// IDA: void __usercall ResetPedSpeed(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPedSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gPedestrian_speed_factor = 1.f;
}

// IDA: void __usercall ResetPedSize(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPedSize(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gPed_scale_factor = 1.f;
    PipeSingleSpecial(ePipe_special_giant_ped_off);
}

// IDA: void __usercall ResetPedExplode(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPedExplode(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gExploding_pedestrians = 0;
}

// IDA: int __usercall SetEngineFactor@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->engine_power_multiplier = pPowerup->float_params[0];
    pCar->grip_multiplier = pPowerup->float_params[1];
    SetCarSuspGiveAndHeight(pCar, pPowerup->float_params[2], pPowerup->float_params[3],
        pPowerup->float_params[6], pPowerup->float_params[4], pPowerup->float_params[5]);
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetUnderwater@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->underwater_ability = 1;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall TrashBodywork@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int TrashBodywork(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    tCar_spec* c;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    for (i = 0; i < pCar->car_actor_count; i++) {
        TotallySpamTheModel(pCar, i, pCar->car_model_actors[i].actor, &pCar->car_model_actors[i].crush_data, 0.5f);
    }
    if (pCar->driver == eDriver_local_human) {
        DRS3StartSound2(gCar_outlet, 5000, 1, 255, 255, -1, -1);
        DRS3StartSound2(gCar_outlet, 5001, 1, 255, 255, -1, -1);
        DRS3StartSound2(gCar_outlet, 5002, 1, 255, 255, -1, -1);
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall TakeDrugs@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int TakeDrugs(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        if (gReal_render_palette == NULL) {
            gReal_render_palette = BrMemAllocate(sizeof(tU32) * 256, kMem_drugs_palette);
            memcpy(gReal_render_palette, gRender_palette->pixels, sizeof(tU32) * 256);
        }
        gOn_drugs = 1;
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall PaletteFuckedUpByDrugs(br_pixelmap *pPixelmap@<EAX>, int pOffset@<EDX>)
void PaletteFuckedUpByDrugs(br_pixelmap* pPixelmap, int pOffset) {
    int i;
    LOG_TRACE("(%p, %d)", pPixelmap, pOffset);

    ((tU32*)gRender_palette->pixels)[0] = gReal_render_palette[0];
    for (i = 1; i < 224; i++) {
        ((tU32*)gRender_palette->pixels)[i] = gReal_render_palette[(i + pOffset) & 0xff];
    }
    for (i = 224; i < 256; i++) {
        ((tU32*)gRender_palette->pixels)[i] = gReal_render_palette[i];
    }
}

// IDA: void __usercall TheEffectsOfDrugs(tPowerup *pPowerup@<EAX>, tU32 pPeriod@<EDX>)
void TheEffectsOfDrugs(tPowerup* pPowerup, tU32 pPeriod) {
    LOG_TRACE("(%p, %d)", pPowerup, pPeriod);

    PaletteFuckedUpByDrugs(gRender_palette, GetTotalTime() / 100);
    ResetPalette();
}

// IDA: int __usercall SetOpponentsSpeed@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    for (i = 0; i < gCurrent_race.number_of_racers && gCurrent_race.opponent_list[i].index != 29; i++) {
    }
    if (i < gCurrent_race.number_of_racers) {
        SetEngineFactor(gPowerup_array + 5, gCurrent_race.opponent_list[i].car_spec);
    }
    gOpponent_speed_factor = pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetCopsSpeed@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gCop_speed_factor = pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetGravity@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gGravity_multiplier = pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetPinball@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gPinball_factor = pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetWallclimb@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->wall_climber_mode = 1;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetBouncey@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->bounce_rate = 1000.f / pPowerup->float_params[0];
    pCar->bounce_amount = pPowerup->float_params[1];
    pCar->last_bounce = 0;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetSuspension@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    SetCarSuspGiveAndHeight(pCar,
        pPowerup->float_params[0], pPowerup->float_params[1], pPowerup->float_params[4],
        pPowerup->float_params[2], pPowerup->float_params[3]);
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetTyreGrip@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->grip_multiplier = pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetDamageMultiplier@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->damage_multiplier = pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetEngineFactor(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetEngineFactor(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->engine_power_multiplier = 1.f;
    pCar->grip_multiplier = 1.f;
    SetCarSuspGiveAndHeight(pCar, 1.f, 1.f, 1.f, 0.f, 0.f);
}

// IDA: void __usercall ResetUnderwater(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetUnderwater(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->underwater_ability = 0;
}

// IDA: void __usercall PukeDrugsBackUp(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void PukeDrugsBackUp(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gOn_drugs = 0;
        PaletteFuckedUpByDrugs(gRender_palette, 0);
        ResetPalette();
    }
}

// IDA: void __usercall ResetOpponentsSpeed(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetOpponentsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    for (i = 0; i < gCurrent_race.number_of_racers && gCurrent_race.opponent_list[i].index != 29; i++) {
    }
    if (i < gCurrent_race.number_of_racers) {
        ResetEngineFactor(&gPowerup_array[5], gCurrent_race.opponent_list[i].car_spec);
    }
    gOpponent_speed_factor = 1.f;
}

// IDA: void __usercall ResetCopsSpeed(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetCopsSpeed(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gCop_speed_factor = 1.f;
}

// IDA: void __usercall ResetGravity(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetGravity(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gGravity_multiplier = gDefault_gravity;
}

// IDA: void __usercall ResetPinball(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPinball(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gPinball_factor = 0.f;
}

// IDA: void __usercall ResetWallclimb(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetWallclimb(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->wall_climber_mode = 0;
}

// IDA: void __usercall ResetBouncey(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetBouncey(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->bounce_rate = 0.f;
    pCar->bounce_amount = 0.f;
}

// IDA: void __usercall ResetSuspension(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetSuspension(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    SetCarSuspGiveAndHeight(pCar, 1.f, 1.f, 1.f, 0.f, 0.f);
}

// IDA: void __usercall ResetDamageMultiplier(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetDamageMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->damage_multiplier = 1.f;
}

// IDA: void __usercall ResetTyreGrip(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetTyreGrip(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->grip_multiplier = 1.f;
    ;
}

// IDA: int __usercall PickAtRandom@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int PickAtRandom(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    return GotPowerup(pCar, pPowerup->integer_params[IRandomBetween(0, pPowerup->number_of_integer_params - 1)]);
}

// IDA: int __usercall PedestrianRespawn@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int PedestrianRespawn(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    RespawnPedestrians();
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall GotVouchers@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int GotVouchers(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        AddVouchers(pPowerup->integer_params[0]);
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall MungeVouchers(tPowerup *pPowerup@<EAX>, tU32 pPeriod@<EDX>)
void MungeVouchers(tPowerup* pPowerup, tU32 pPeriod) {
    LOG_TRACE("(%p, %d)", pPowerup, pPeriod);

    pPowerup->current_value = GetRecoverVoucherCount();
}

// IDA: int __usercall SetInstantHandbrake@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gInstant_handbrake = 1;
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetInstantHandbrake(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetInstantHandbrake(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gInstant_handbrake = 0;
    }
}

// IDA: void __usercall DoBouncey(tPowerup *pPowerup@<EAX>, tU32 pPeriod@<EDX>)
void DoBouncey(tPowerup* pPowerup, tU32 pPeriod) {
    LOG_TRACE("(%p, %d)", pPowerup, pPeriod);

    if (gProgram_state.current_car.bounce_rate <= GetTotalTime() - gProgram_state.current_car.last_bounce && gProgram_state.current_car.number_of_wheels_on_ground > 2) {
        PratcamEvent(42);
        gProgram_state.current_car.last_bounce = GetTotalTime();
        gProgram_state.current_car.v.v[1] += gProgram_state.current_car.bounce_amount;
        DRS3StartSound(gCar_outlet, 9010);
    }
}

// IDA: int __usercall HitMine@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int HitMine(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    float fudge_multiplier;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->v.v[1] = FRandomBetween(pPowerup->float_params[0], pPowerup->float_params[1]) / pCar->M + pCar->v.v[1];
    pCar->omega.v[2] = FRandomPosNeg(pPowerup->float_params[2]) * TAU / pCar->M + pCar->omega.v[2];
    pCar->omega.v[0] = FRandomPosNeg(pPowerup->float_params[3]) * TAU / pCar->M + pCar->omega.v[0];
    if (pCar->driver != eDriver_non_car_unused_slot && !pCar->invulnerable) {
        fudge_multiplier = pCar->car_model_actors[pCar->principal_car_actor].crush_data.softness_factor / .7f;
        for (i = 0; i < pCar->car_actor_count; i++) {
            TotallySpamTheModel(pCar, i, pCar->car_model_actors[i].actor,
                &pCar->car_model_actors[i].crush_data, fudge_multiplier * .1f);
        }
        for (i = 0; i < 12; i++) {
            DamageUnit(pCar, i, IRandomBetween(0, fudge_multiplier * 15.f));
        }
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: int __usercall SetMassMultiplier@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->collision_mass_multiplier = pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetMassMultiplier(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetMassMultiplier(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->collision_mass_multiplier = 1.f;
}

// IDA: int __usercall ShowPedestrians@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int ShowPedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gShow_peds_on_map = 1;
    }
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall HidePedestrians(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void HidePedestrians(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    if (pCar->driver == eDriver_local_human) {
        gShow_peds_on_map = 0;
    }
}

// IDA: int __usercall SetProximity@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->proxy_ray_distance = pPowerup->float_params[0] * pPowerup->float_params[0];
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetProximity(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetProximity(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    pCar->proxy_ray_distance = 0.f;
}

// IDA: int __usercall SetPedHarvest@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gPedestrian_harvest = 1;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetPedHarvest(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetPedHarvest(tPowerup* pPowerup, tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gPedestrian_harvest = 0;
    for (i = 0; i < COUNT_OF(gPed_harvest_sounds); i++) {
        DRS3StartSound3D(gPedestrians_outlet, gPed_harvest_sounds[i], &pCar->pos,
            &gZero_v__powerup, 1, 255, -1, -1);
    }
}

// IDA: int __usercall SetVesuvianCorpses@<EAX>(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
int SetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gVesuvian_corpses = 1;
    return GET_POWERUP_INDEX(pPowerup);
}

// IDA: void __usercall ResetVesuvianCorpses(tPowerup *pPowerup@<EAX>, tCar_spec *pCar@<EDX>)
void ResetVesuvianCorpses(tPowerup* pPowerup, tCar_spec* pCar) {
    LOG_TRACE("(%p, %p)", pPowerup, pCar);

    gVesuvian_corpses = 0;
}

// IDA: void __usercall ReceivedPowerup(tNet_contents *pContents@<EAX>)
void ReceivedPowerup(tNet_contents* pContents) {
    tPowerup* powerup;
    tCar_spec* car;
    LOG_TRACE("(%p)", pContents);

    if (gProgram_state.racing && pContents->data.powerup.powerup_index >= 0 && pContents->data.powerup.powerup_index < gNumber_of_powerups) {
        powerup = &gPowerup_array[pContents->data.powerup.powerup_index];
        if (pContents->data.powerup.event == ePowerup_gained || pContents->data.powerup.event == ePowerup_ongoing) {
            if (powerup->net_type == eNet_powerup_global) {
                GotPowerupX(&gProgram_state.current_car, pContents->data.powerup.powerup_index, 0,
                    pContents->data.powerup.event == ePowerup_gained, pContents->data.powerup.time_left);
            } else if (powerup->net_type == eNet_powerup_local && pContents->data.powerup.player != gLocal_net_ID && powerup->got_proc != NULL) {
                car = NetCarFromPlayerID(pContents->data.powerup.player);
                if (car != NULL) {
                    powerup->got_proc(powerup, car);
                    switch (powerup->type) {
                    case ePowerup_timed:
                        car->powerups[pContents->data.powerup.powerup_index] = GetTotalTime() + pContents->data.powerup.time_left;
                        break;
                    case ePowerup_whole_race:
                        car->powerups[pContents->data.powerup.powerup_index] = -1;
                        break;
                    default:
                        break;
                    }
                }
            }
        } else if (powerup->net_type == eNet_powerup_local && powerup->lose_proc != NULL) {
            car = NetCarFromPlayerID(pContents->data.powerup.player);
            if (car != NULL) {
                powerup->lose_proc(powerup, car);
                car->powerups[pContents->data.powerup.powerup_index] = 0;
            }
        }
    }
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

    for (cat = eVehicle_self; cat < eVehicle_net_player; cat++) {
        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (i = 0; i < car_count; i++) {
            if (cat == eVehicle_self) {
                car = &gProgram_state.current_car;
            } else {
                car = GetCarSpec(cat, i);
            }
#if defined(DETHRACE_FIX_BUGS)
            ID = gNet_players[0].ID;
#endif
            for (j = 0; j < gNumber_of_net_players; j++) {
                if (gNet_players[j].car == car) {
                    ID = gNet_players[j].ID;
                    break;
                }
            }
            for (j = 0; j < gNumber_of_powerups; j++) {
                if (car->powerups[j] != 0) {
                    the_contents = NetGetBroadcastContents(21, 0);
                    the_contents->data.powerup.event = ePowerup_ongoing;
                    the_contents->data.powerup.player = ID;
                    the_contents->data.powerup.powerup_index = j;
                    the_contents->data.powerup.time_left = car->powerups[i] - GetTotalTime();
                }
            }
        }
    }
}

// IDA: void __usercall LoseAllLocalPowerups(tCar_spec *pCar@<EAX>)
void LoseAllLocalPowerups(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);

    if (pCar->driver == eDriver_local_human) {
        for (i = 0; i < gNumber_of_powerups; i++) {
            if (pCar->powerups[i] != 0 && gPowerup_array[i].net_type == eNet_powerup_local) {
                LosePowerup(&gPowerup_array[i]);
            }
        }
    }
}

// Added by dethrace
void GetPowerupMessage(int pN, char* pMessage) {
    switch (pN) {
    case 0:
        strcpy(pMessage, "Bonus");
        break;
    case 1:
        strcpy(pMessage, "Mega Bonus");
        break;
    case 14:
    case 46:
        strcpy(pMessage, "Mine");
        break;
    case 28:
    case 29:
    case 30:
    case 31:
        strcpy(pMessage, "Random");
        break;
    default:
        strcpy(pMessage, gPowerup_array[pN].message);
        break;
    }
}
