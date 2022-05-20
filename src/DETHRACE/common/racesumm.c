#include "racesumm.h"
#include "brender/brender.h"
#include "crush.h"
#include "cutscene.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "intrface.h"
#include "loading.h"
#include "network.h"
#include "pd/sys.h"
#include "s3/s3.h"
#include "sound.h"
#include "utility.h"
#include <stdlib.h>

int gPlayer_lookup[6];
tMouse_area gOld_back_button;
tWreck_info gWreck_array[30];
br_actor* gWreck_root;
br_actor* gWreck_camera;
tU32 gWreck_start_zoom;
tU32 gWreck_gallery_start;
float gTemp_rank_increase;
float gRank_per_ms;
tU32 gLast_wreck_draw;
tS3_sound_tag gSumm_sound;
float gCredits_per_ms;
tMouse_area* gBack_button_ptr;
tU32 gSummary_start;
br_pixelmap* gWreck_z_buffer;
br_pixelmap* gWreck_render_area;
int gWreck_selected;
int gWreck_zoom_out;
br_pixelmap* gChrome_font;
int gWreck_zoom_in;
int gTemp_credits;
int gUser_interacted;
int gWreck_count;
int gRank_etc_munged;
int gRank_increase;
int gTemp_earned;
int gTemp_rank;
int gWreck_zoomed_in;
int gDone_initial;
int gTemp_lost;

// IDA: void __usercall MungeRankEtc(tProgram_state *pThe_state@<EAX>)
void MungeRankEtc(tProgram_state* pThe_state) {
    int i;
    int not_done_yet;
    LOG_TRACE("(%p)", pThe_state);

    not_done_yet = 0;
    if (!gRank_etc_munged) {
        gRank_etc_munged = 1;
        gRace_list[pThe_state->current_race_index].been_there_done_that++;
        pThe_state->rank -= gRank_increase;
        if (pThe_state->rank <= 0) {
            pThe_state->rank = 1;
            for (i = 0; i < gNumber_of_races; i++) {
                if (gRace_list[i].best_rank < 2 && !gRace_list[i].been_there_done_that) {
                    not_done_yet = 1;
                    break;
                }
            }

            pThe_state->game_completed = !not_done_yet;
        }
        pThe_state->credits += pThe_state->credits_earned - pThe_state->credits_lost;
        if (pThe_state->credits > 999999) {
            pThe_state->credits = 999999;
        }
        pThe_state->credits_earned = 0;
        pThe_state->credits_lost = 0;
        gRank_increase = 0;
        if (pThe_state->credits < 0) {
            pThe_state->credits = 0;
        }
    }
}

// IDA: void __cdecl CalcRankIncrease()
void CalcRankIncrease() {
    LOG_TRACE("()");

    if (gNet_mode == eNet_mode_none) {
        gRank_increase = gProgram_state.credits_earned / gProgram_state.credits_per_rank;
        if (gRank_increase > 5) {
            gRank_increase = 5;
        }
    } else {
        gRank_increase = 1;
    }
}

// IDA: int __usercall RaceSummaryDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int RaceSummaryDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);

    if (pTimed_out) {
        pCurrent_choice = 0;
    } else if (pEscaped) {
        pCurrent_choice = -1;
    }
    return pCurrent_choice;
}

// IDA: void __usercall DrawInBox(int pBox_left@<EAX>, int pText_left@<EDX>, int pTop@<EBX>, int pRight@<ECX>, int pBottom, int pColour, int pAmount)
void DrawInBox(int pBox_left, int pText_left, int pTop, int pRight, int pBottom, int pColour, int pAmount) {
    LOG_TRACE("(%d, %d, %d, %d, %d, %d, %d)", pBox_left, pText_left, pTop, pRight, pBottom, pColour, pAmount);

    BrPixelmapRectangleFill(gBack_screen,
        pBox_left,
        pTop,
        pRight - pBox_left,
        pBottom - pTop,
        0);
    if (pAmount >= 0) {
        BrPixelmapTextF(gBack_screen, pText_left, pTop - (TranslationMode() ? 2 : 0), pColour, gFont_7, "%d", pAmount);
    }
}

// IDA: void __usercall DrawChromeNumber(int pLeft_1@<EAX>, int pLeft_2@<EDX>, int pPitch@<EBX>, int pTop@<ECX>, int pAmount)
void DrawChromeNumber(int pLeft_1, int pLeft_2, int pPitch, int pTop, int pAmount) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pLeft_1, pLeft_2, pPitch, pTop, pAmount);

    if (pAmount < 10) {
        DRPixelmapRectangleMaskedCopy(gBack_screen,
            pLeft_1,
            pTop,
            gChrome_font,
            0,
            gChrome_font->height / 10 * pAmount,
            gChrome_font->width,
            gChrome_font->height / 10);
    } else {
        DrawChromeNumber(pLeft_2, 0, 0, pTop, pAmount / 10);
        DrawChromeNumber(pPitch + pLeft_2, 0, 0, pTop, pAmount % 10);
    }
}

// IDA: void __cdecl DrawSummaryItems()
void DrawSummaryItems() {
    LOG_TRACE("()");

    DrawInBox(
        gCurrent_graf_data->summ1_credits_box_left,
        gCurrent_graf_data->summ1_credits_left,
        gCurrent_graf_data->summ1_earned_top,
        gCurrent_graf_data->summ1_credits_right,
        gCurrent_graf_data->summ1_earned_bottom,
        2,
        gTemp_earned);
    DrawInBox(
        gCurrent_graf_data->summ1_credits_box_left,
        gCurrent_graf_data->summ1_credits_left,
        gCurrent_graf_data->summ1_lost_top,
        gCurrent_graf_data->summ1_credits_right,
        gCurrent_graf_data->summ1_lost_bottom,
        2,
        gTemp_lost);
    DrawInBox(
        gCurrent_graf_data->summ1_credits_box_left,
        gCurrent_graf_data->summ1_credits_left,
        gCurrent_graf_data->summ1_total_top,
        gCurrent_graf_data->summ1_credits_right,
        gCurrent_graf_data->summ1_total_bottom,
        2,
        (gTemp_credits <= 0) ? 0 : ((gTemp_credits >= 999999) ? 999999 : gTemp_credits));
    BrPixelmapRectangleFill(
        gBack_screen,
        gCurrent_graf_data->summ1_rank_inc_left,
        gCurrent_graf_data->summ1_rank_top,
        gCurrent_graf_data->summ1_rank_inc_right - gCurrent_graf_data->summ1_rank_inc_left,
        gCurrent_graf_data->summ1_rank_bot - gCurrent_graf_data->summ1_rank_top,
        0);
    BrPixelmapRectangleFill(
        gBack_screen,
        gCurrent_graf_data->summ1_rank_total_left,
        gCurrent_graf_data->summ1_rank_top,
        gCurrent_graf_data->summ1_rank_total_right - gCurrent_graf_data->summ1_rank_total_left,
        gCurrent_graf_data->summ1_rank_bot - gCurrent_graf_data->summ1_rank_top,
        0);
    DrawChromeNumber(
        gCurrent_graf_data->summ1_rank_inc_c,
        gCurrent_graf_data->summ1_rank_inc_l,
        gCurrent_graf_data->summ1_rank_x_pitch,
        gCurrent_graf_data->summ1_rank_y,
        (int)(gTemp_rank_increase + .5f));
    DrawChromeNumber(
        gCurrent_graf_data->summ1_rank_total_c,
        gCurrent_graf_data->summ1_rank_total_l,
        gCurrent_graf_data->summ1_rank_x_pitch,
        gCurrent_graf_data->summ1_rank_y,
        gTemp_rank);
}

// IDA: void __usercall RampUpRate(float *pRate@<EAX>, tU32 pTime@<EDX>)
void RampUpRate(float* pRate, tU32 pTime) {
    LOG_TRACE("(%p, %d)", pRate, pTime);

    if (pTime >= 6000) {
        *pRate = 10.f;
    } else if (pTime >= 4000) {
        *pRate = 5.f;
    } else if (pTime >= 3000) {
        *pRate = 1.f;
    } else if (pTime >= 2000) {
        *pRate = 0.5f;
    } else {
        *pRate = 0.1f;
    }
}

// IDA: void __usercall DrawSummary(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawSummary(int pCurrent_choice, int pCurrent_mode) {
    tU32 the_time;
    static tU32 last_time;
    static tU32 last_change_time;
    int credit_delta;
    float old_temp_increase;
    float rank_delta;
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);

    the_time = PDGetTotalTime();
    if (the_time - gSummary_start > 3000) {
        if (gTemp_earned != 0) {
            ResetInterfaceTimeout();
            if (gSumm_sound == 0) {
                gSumm_sound = DRS3StartSound(gEffects_outlet, 3200);
            }
            credit_delta = (the_time - last_time) * gCredits_per_ms;
            gTemp_earned -= credit_delta;
            gTemp_credits += credit_delta;
            RampUpRate(&gCredits_per_ms, the_time - last_change_time + 1000);
            if (gTemp_earned <= 0) {
                gTemp_credits += gTemp_earned;
                S3StopOutletSound(gEffects_outlet);
                gSumm_sound = 0;
                gTemp_earned = 0;
                gCredits_per_ms = 0.1f;
                last_change_time = the_time;
            }
        } else if (gTemp_lost != 0) {
            ResetInterfaceTimeout();
            if (the_time - last_change_time > 1000) {
                if (gSumm_sound == 0) {
                    gSumm_sound = DRS3StartSound(gEffects_outlet, 3201);
                }
                credit_delta = (the_time - last_time) * gCredits_per_ms;
                gTemp_lost -= credit_delta;
                gTemp_credits -= credit_delta;
                RampUpRate(&gCredits_per_ms, the_time - last_change_time);
                if (gTemp_lost <= 0) {
                    gTemp_credits -= gTemp_lost;
                    S3StopOutletSound(gEffects_outlet);
                    gSumm_sound = 0;
                    gTemp_lost = 0;
                    last_change_time = the_time;
                }
            }
        } else if (gTemp_rank_increase != 0.f) {
            ResetInterfaceTimeout();
            if (the_time - last_change_time > 1000) {
                rank_delta = (the_time - last_time) * gRank_per_ms;
                old_temp_increase = gTemp_rank_increase;
                gTemp_rank_increase -= rank_delta;
                if ((int)(old_temp_increase + 0.5f) != (int)(gTemp_rank_increase + 0.5f)) {
                    if (gTemp_rank > 1) {
                        gTemp_rank -= 1;
                    }
                    gSumm_sound = DRS3StartSound(gEffects_outlet, 3202);
                }
                if (gTemp_rank_increase <= 0.f) {
                    S3StopOutletSound(gEffects_outlet);
                    gSumm_sound = 0;
                    gTemp_rank_increase = 0.f;
                    last_change_time = the_time;
                }
            }
        } else {
            S3StopOutletSound(gEffects_outlet);
            gSumm_sound = 0;
        }
    } else {
        last_change_time = the_time;
    }
    DrawSummaryItems();
    last_time = the_time;
}

// IDA: void __cdecl StartSummary()
void StartSummary() {
    LOG_TRACE("()");

    DrawSummaryItems();
    gSummary_start = PDGetTotalTime();
}

// IDA: void __cdecl SetUpTemps()
void SetUpTemps() {
    LOG_TRACE("()");

    gTemp_earned = gProgram_state.credits_earned;
    gTemp_lost = gProgram_state.credits_lost;
    gTemp_rank_increase = gRank_increase;
    gTemp_rank = gProgram_state.rank;
    gTemp_credits = gProgram_state.credits;
}

// IDA: int __usercall Summ1GoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int Summ1GoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    S3StopOutletSound(gEffects_outlet);
    MungeRankEtc(&gProgram_state);
    SetUpTemps();
    DrawSummaryItems();
    return 1;
}

// IDA: int __usercall SummCheckGameOver@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int SummCheckGameOver(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    tS3_sound_tag sound_tag;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gTemp_credits > 0) {
        return 0;
    }
    S3StopOutletSound(gEffects_outlet);
    RemoveTransientBitmaps(1);
    for (i = 0; i < 7; i++) {
        DrawInBox(
            gCurrent_graf_data->summ1_credits_box_left,
            gCurrent_graf_data->summ1_credits_left,
            gCurrent_graf_data->summ1_total_top,
            gCurrent_graf_data->summ1_credits_right,
            gCurrent_graf_data->summ1_total_bottom,
            2, -1);
        ProcessFlicQueue(gFrame_period);
        PDScreenBufferSwap(0);
        SoundService(0);
        WaitFor(300);
        DrawInBox(
            gCurrent_graf_data->summ1_credits_box_left,
            gCurrent_graf_data->summ1_credits_left,
            gCurrent_graf_data->summ1_total_top,
            gCurrent_graf_data->summ1_credits_right,
            gCurrent_graf_data->summ1_total_bottom,
            2, 0);
        ProcessFlicQueue(gFrame_period);
        PDScreenBufferSwap(0);
        SoundService(0);
        WaitFor(300);
    }
    S3StopAllOutletSounds();
    return 1;
}

// IDA: tSO_result __cdecl DoEndRaceSummary1()
tSO_result DoEndRaceSummary1() {
    static tFlicette flicker_on[1] = {
        { 43, { 218, 436 }, { 147, 353 } },
    };
    static tFlicette flicker_off[1] = {
        { 42, { 218, 436 }, { 147, 353 } },
    };
    static tFlicette push[1] = {
        { 154, { 218, 436 }, { 147, 353 } },
    };
    static tMouse_area mouse_areas[1] = {
        { { 218, 436 }, { 147, 353 }, { 281, 562 }, { 167, 401 }, 0, 0, 0, NULL },
    };
    static tInterface_spec interface_spec = {
        0,
        310,
        0,
        0,
        0,
        0,
        -1,
        { -1, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { NULL, NULL },
        { -1, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { NULL, NULL },
        { -1, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { NULL, NULL },
        { -1, 0 },
        { 1, 0 },
        { 0, 0 },
        { 0, 0 },
        { NULL, NULL },
        { 1, 1 },
        { Summ1GoAhead, NULL },
        { 1, 1 },
        { NULL, NULL },
        SummCheckGameOver,
        DrawSummary,
        20000,
        NULL,
        StartSummary,
        RaceSummaryDone,
        0,
        { 0, 0 },
        NULL,
        -1,
        1,
        COUNT_OF(flicker_on),
        flicker_on,
        flicker_off,
        push,
        COUNT_OF(mouse_areas),
        mouse_areas,
        0,
        NULL,
    };
    int result;
    int completed_already;
    LOG_TRACE("()");

    NetPlayerStatusChanged(ePlayer_status_summary);
    completed_already = gProgram_state.game_completed;
    gSumm_sound = 0;
    CalcRankIncrease();
    SetUpTemps();
    gCredits_per_ms = 0.1f;
    gRank_per_ms = 0.003f;
    gChrome_font = LoadChromeFont();
    result = DoInterfaceScreen(&interface_spec, 0, 0);
    MungeRankEtc(&gProgram_state);
    NetPlayerStatusChanged(ePlayer_status_loading);
    DisposeChromeFont(gChrome_font);
    DRS3StartSound(gEffects_outlet, 3004);
    if (result < 0) {
        DRS3StartSound(gEffects_outlet, 3007);
        RunFlic(311);
        result = eSO_main_menu_invoked;
    } else if (gTemp_credits <= 0) {
        FadePaletteDown();
        result = eSO_game_over;
    } else if (gProgram_state.game_completed && !completed_already) {
        FadePaletteDown();
        result = eSO_game_completed;
    } else {
        DRS3StartSound(gEffects_outlet, 3007);
        FadePaletteDown();
        result = eSO_continue;
    }

    return result;
}

// IDA: void __usercall PrepareBoundingRadius(br_model *model@<EAX>)
//  Suffix added to avoid duplicate symbol
void PrepareBoundingRadius__racesumm(br_model* model) {
    float d;
    float max;
    int v;
    br_vertex* vp;
    LOG_TRACE("(%p)", model);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BuildWrecks()
void BuildWrecks() {
    int cat;
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeWrecks()
void DisposeWrecks() {
    int cat;
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall MatrixIsIdentity@<EAX>(br_matrix34 *pMat@<EAX>)
int MatrixIsIdentity(br_matrix34* pMat) {
    LOG_TRACE("(%p)", pMat);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SpinWrecks(tU32 pFrame_period@<EAX>)
void SpinWrecks(tU32 pFrame_period) {
    int i;
    br_vector3 translation;
    br_matrix34 old_mat;
    LOG_TRACE("(%d)", pFrame_period);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ZoomInTo(int pIndex@<EAX>, int *pCurrent_choice@<EDX>, int *pCurrent_mode@<EBX>)
void ZoomInTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%d, %p, %p)", pIndex, pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ZoomOutTo(int pIndex@<EAX>, int *pCurrent_choice@<EDX>, int *pCurrent_mode@<EBX>)
void ZoomOutTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%d, %p, %p)", pIndex, pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl WreckPick(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pNear, br_scalar pFar, void *pArg)
int WreckPick(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pNear, br_scalar pFar, void* pArg) {
    int i;
    LOG_TRACE("(%p, %p, %p, %p, %p, %f, %f, %p)", pActor, pModel, pMaterial, pRay_pos, pRay_dir, pNear, pFar, pArg);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CastSelectionRay@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int CastSelectionRay(int* pCurrent_choice, int* pCurrent_mode) {
    int mouse_x;
    int mouse_y;
    int i;
    int result;
    br_scalar inv_wreck_pick_scale_factor;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DamageScrnExit@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnExit(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageScrnDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DamageScrnDraw(int pCurrent_choice, int pCurrent_mode) {
    tU32 the_time;
    br_vector3 camera_movement;
    int finished;
    int h;
    int v;
    int rows;
    int columns;
    float spacing;
    br_actor* sel_actor;
    char* name;
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DamageScrnLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DamageScrnRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnRight(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DamageScrnUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnUp(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DamageScrnDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnDown(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DamageScrnGoHead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnGoHead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ClickDamage@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int ClickDamage(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int mouse_x;
    int mouse_y;
    int old_mouse_x;
    int old_mouse_y;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DamageScrnDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int DamageScrnDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
    NOT_IMPLEMENTED();
}

// IDA: tSO_result __cdecl DoEndRaceSummary2()
tSO_result DoEndRaceSummary2() {
    static tFlicette flicker_on[3];
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("()");

    STUB();
    return eSO_continue;
}

// IDA: void __usercall DrawAnItem(int pX@<EAX>, int pY_index@<EDX>, int pFont_index@<EBX>, char *pText@<ECX>)
//  Suffix added to avoid duplicate symbol
void DrawAnItem__racesumm(int pX, int pY_index, int pFont_index, char* pText) {
    LOG_TRACE("(%d, %d, %d, \"%s\")", pX, pY_index, pFont_index, pText);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawColumnHeading(int pStr_index@<EAX>, int pX@<EDX>)
//  Suffix added to avoid duplicate symbol
void DrawColumnHeading__racesumm(int pStr_index, int pX) {
    LOG_TRACE("(%d, %d)", pStr_index, pX);
}

// IDA: int __usercall SortScores@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
int SortScores(void* pFirst_one, void* pSecond_one) {
    LOG_TRACE("(%p, %p)", pFirst_one, pSecond_one);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SortGameScores()
void SortGameScores() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetSumDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void NetSumDraw(int pCurrent_choice, int pCurrent_mode) {
    int i;
    char s[256];
    tNet_game_player_info* player;
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoNetRaceSummary()
void DoNetRaceSummary() {
    static tFlicette flicker_on[1];
    static tFlicette flicker_off[1];
    static tFlicette push[1];
    static tMouse_area mouse_areas[1];
    static tInterface_spec interface_spec;
    int i;
    int result;
    tS32 start_time;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tSO_result __usercall DoEndRaceSummary@<EAX>(int *pFirst_summary_done@<EAX>, tRace_result pRace_result@<EDX>)
tSO_result DoEndRaceSummary(int* pFirst_summary_done, tRace_result pRace_result) {
    tSO_result result;
    LOG_TRACE("(%p, %d)", pFirst_summary_done, pRace_result);

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_carmageddon_demo) {
        gRank_etc_munged = 1;
        DoEndRaceSummary2();
        return eSO_continue;
    }
    gRank_etc_munged = 0;
    if (!*pFirst_summary_done && pRace_result != eRace_timed_out) {
        if (gNet_mode != eNet_mode_none) {
            CalcRankIncrease();
            MungeRankEtc(&gProgram_state);
            DoNetRaceSummary();
            return eSO_continue;
        }
        result = DoEndRaceSummary1();
    } else {
        result = eSO_continue;
    }
    *pFirst_summary_done = 1;
    if (result == eSO_game_over) {
        DoGameOverAnimation();
        gProgram_state.prog_status = eProg_opening;
        result = eSO_continue;
    } else if (result == eSO_main_menu_invoked) {
        *pFirst_summary_done = 0;
        result = eSO_main_menu_invoked;
    } else {
        if (result == eSO_game_completed) {
            DoGameCompletedAnimation();
        }
        result = DoEndRaceSummary2();
        if (result != eSO_main_menu_invoked) {
            TotallyRepairCar();
            result = eSO_continue;
        }
    }
    return result;
}
