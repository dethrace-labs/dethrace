#include "racesumm.h"
#include "brender.h"
#include "crush.h"
#include "cutscene.h"
#include "displays.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "input.h"
#include "intrface.h"
#include "loading.h"
#include "main.h"
#include "network.h"
#include "opponent.h"
#include "pd/sys.h"
#include "raycast.h"
#include "s3/s3.h"
#include "sound.h"
#include "utility.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x00530e38
int gPlayer_lookup[6];

// GLOBAL: CARM95 0x00530e58
tMouse_area gOld_back_button;

// GLOBAL: CARM95 0x00530ec0
tWreck_info gWreck_array[30];

// GLOBAL: CARM95 0x00530e94
br_actor* gWreck_root;

// GLOBAL: CARM95 0x00530e2c
br_actor* gWreck_camera;

// GLOBAL: CARM95 0x00530eac
tU32 gWreck_start_zoom;

// GLOBAL: CARM95 0x00530ea4
tU32 gWreck_gallery_start;

// GLOBAL: CARM95 0x00530e28
float gTemp_rank_increase;

// GLOBAL: CARM95 0x00530eb8
float gRank_per_ms;

// GLOBAL: CARM95 0x00530eb0
tU32 gLast_wreck_draw;

// GLOBAL: CARM95 0x00530ea0
tS3_sound_tag gSumm_sound;

// GLOBAL: CARM95 0x00530e8c
float gCredits_per_ms;

// GLOBAL: CARM95 0x00530e10
tMouse_area* gBack_button_ptr;

// GLOBAL: CARM95 0x00530e18
tU32 gSummary_start;

// GLOBAL: CARM95 0x00530e98
br_pixelmap* gWreck_z_buffer;

// GLOBAL: CARM95 0x00530e08
br_pixelmap* gWreck_render_area;

// GLOBAL: CARM95 0x00531d48
int gWreck_selected;

// GLOBAL: CARM95 0x00531d4c
int gWreck_zoom_out;

// GLOBAL: CARM95 0x00530e90
br_pixelmap* gChrome_font;

// GLOBAL: CARM95 0x00530eb4
int gWreck_zoom_in;

// GLOBAL: CARM95 0x00530ea8
int gTemp_credits;

// GLOBAL: CARM95 0x00530e50
int gUser_interacted;

// GLOBAL: CARM95 0x00530e00
int gWreck_count;

// GLOBAL: CARM95 0x00530e88
int gRank_etc_munged;

// GLOBAL: CARM95 0x00530e0c
int gRank_increase;

// GLOBAL: CARM95 0x00530e14
int gTemp_earned;

// GLOBAL: CARM95 0x00530e30
int gTemp_rank;

// GLOBAL: CARM95 0x00530e24
int gWreck_zoomed_in;

// GLOBAL: CARM95 0x00530e04
int gDone_initial;

// GLOBAL: CARM95 0x00530e20
int gTemp_lost;

// IDA: void __usercall MungeRankEtc(tProgram_state *pThe_state@<EAX>)
// FUNCTION: CARM95 0x00415740
void MungeRankEtc(tProgram_state* pThe_state) {
    int i;
    int not_done_yet;

    not_done_yet = 0;
    if (!gRank_etc_munged) {
        gRank_etc_munged = 1;
        gRace_list[pThe_state->current_race_index].been_there_done_that++;
        pThe_state->rank -= gRank_increase;
        if (pThe_state->rank < 1) {
            pThe_state->rank = 1;
            for (i = 0; i < gNumber_of_races; i++) {
                if (gRace_list[i].best_rank <= 1 && !gRace_list[i].been_there_done_that) {
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
// FUNCTION: CARM95 0x0041631f
void CalcRankIncrease(void) {

    if (gNet_mode != eNet_mode_none) {
        gRank_increase = 1;
    } else {
        gRank_increase = gProgram_state.credits_earned / gProgram_state.credits_per_rank;
        if (gRank_increase > 5) {
            gRank_increase = 5;
        }
    }
}

// IDA: int __usercall RaceSummaryDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
// FUNCTION: CARM95 0x0041587e
int RaceSummaryDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {

    if (pTimed_out) {
        return 0;
    } else if (pEscaped) {
        return -1;
    } else {
        return pCurrent_choice;
    }
}

// IDA: void __usercall DrawInBox(int pBox_left@<EAX>, int pText_left@<EDX>, int pTop@<EBX>, int pRight@<ECX>, int pBottom, int pColour, int pAmount)
// FUNCTION: CARM95 0x00415e44
void DrawInBox(int pBox_left, int pText_left, int pTop, int pRight, int pBottom, int pColour, int pAmount) {

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
// FUNCTION: CARM95 0x00415ebd
void DrawChromeNumber(int pLeft_1, int pLeft_2, int pPitch, int pTop, int pAmount) {

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
// FUNCTION: CARM95 0x00415beb
void DrawSummaryItems(void) {

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
        (int)(gTemp_rank_increase + 0.5));
    DrawChromeNumber(
        gCurrent_graf_data->summ1_rank_total_c,
        gCurrent_graf_data->summ1_rank_total_l,
        gCurrent_graf_data->summ1_rank_x_pitch,
        gCurrent_graf_data->summ1_rank_y,
        gTemp_rank);
}

// IDA: void __usercall RampUpRate(float *pRate@<EAX>, tU32 pTime@<EDX>)
// FUNCTION: CARM95 0x00415f72
void RampUpRate(float* pRate, tU32 pTime) {

    if (pTime > 6000) {
        *pRate = 10.f;
    } else if (pTime > 4000) {
        *pRate = 5.f;
    } else if (pTime > 3000) {
        *pRate = 1.f;
    } else if (pTime > 2000) {
        *pRate = 0.5f;
    } else {
        *pRate = 0.1f;
    }
}

// IDA: void __usercall DrawSummary(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004158c0
void DrawSummary(int pCurrent_choice, int pCurrent_mode) {
    tU32 the_time;
    // GLOBAL: CARM95 0x530e1c
    static tU32 last_time;
    // GLOBAL: CARM95 0x530e9c
    static tU32 last_change_time;
    int credit_delta;
    float old_temp_increase;
    float rank_delta;

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
                last_change_time = the_time;
                gCredits_per_ms = 0.1f;
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
                if ((int)(old_temp_increase + 0.5) != (int)(gTemp_rank_increase + 0.5)) {
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
        last_time = the_time;
        last_change_time = the_time;
    }
    DrawSummaryItems();
    last_time = the_time;
}

// IDA: void __cdecl StartSummary()
// FUNCTION: CARM95 0x00415ff2
void StartSummary(void) {

    DrawSummaryItems();
    gSummary_start = PDGetTotalTime();
}

// IDA: void __cdecl SetUpTemps()
// FUNCTION: CARM95 0x00416046
void SetUpTemps(void) {

    gTemp_earned = gProgram_state.credits_earned;
    gTemp_lost = gProgram_state.credits_lost;
    gTemp_credits = gProgram_state.credits;
    gTemp_rank_increase = gRank_increase;
    gTemp_rank = gProgram_state.rank;
}

// IDA: int __usercall Summ1GoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x0041600c
int Summ1GoAhead(int* pCurrent_choice, int* pCurrent_mode) {

    S3StopOutletSound(gEffects_outlet);
    MungeRankEtc(&gProgram_state);
    SetUpTemps();
    DrawSummaryItems();
    return 1;
}

// IDA: int __usercall SummCheckGameOver@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x0041608d
int SummCheckGameOver(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    tS3_sound_tag sound_tag;

    if (gTemp_credits <= 0) {
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
            SoundService();
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
            SoundService();
            WaitFor(300);
        }
        S3StopAllOutletSounds();
        return 1;
    } else {
        return 0;
    }
}

// IDA: tSO_result __cdecl DoEndRaceSummary1()
// FUNCTION: CARM95 0x004161dd
tSO_result DoEndRaceSummary1(void) {
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

    // GLOBAL: CARM95 0x00509C38
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
        return eSO_main_menu_invoked;
    } else if (gTemp_credits <= 0) {
        FadePaletteDown();
        return eSO_game_over;
    } else if (gProgram_state.game_completed && !completed_already) {
        FadePaletteDown();
        return eSO_game_completed;
    } else {
        DRS3StartSound(gEffects_outlet, 3007);
        FadePaletteDown();
        return eSO_continue;
    }
}

// IDA: void __usercall PrepareBoundingRadius(br_model *model@<EAX>)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x00417e27
void PrepareBoundingRadius__racesumm(br_model* model) {
    float d;
    float max;
    int v;
    br_vertex* vp;

    max = 0.f;
    for (v = 0; v < model->nvertices; v++) {
        vp = &model->vertices[v];
        d = BrVector3LengthSquared(&vp->p);
        if (d > max) {
            max = d;
        }
    }
    d = sqrt(max);
    model->radius = d;
}

// IDA: void __cdecl BuildWrecks()
// FUNCTION: CARM95 0x00417a91
void BuildWrecks(void) {
    int cat;
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;

    gWreck_count = 0;
    position = 0;
    gWreck_root = BrActorAllocate(BR_ACTOR_NONE, NULL);
    gWreck_camera = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    BrActorAdd(gUniverse_actor, gWreck_root);
    BrActorAdd(gWreck_root, gWreck_camera);
    memcpy(gWreck_camera->type_data, gCamera_list[1]->type_data, sizeof(br_camera));
    ((br_camera*)gWreck_camera->type_data)->aspect = 2.f;
    ((br_camera*)gWreck_camera->type_data)->field_of_view = BR_ANGLE_DEG(55);
    BrMatrix34Identity(&gWreck_camera->t.t.mat);
    BrVector3SetFloat(&gWreck_camera->t.t.translate.t, 0.f, 0.f, 2.2f);
    for (cat = eVehicle_self; cat < eVehicle_rozzer; cat++) {
        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (i = 0; i < car_count; i++) {
            if (cat == eVehicle_self) {
                the_car = &gProgram_state.current_car;
            } else {
                the_car = GetCarSpec(cat, i);
            }
            this_car = the_car->car_model_actors[the_car->principal_car_actor].actor;
            memcpy(&gWreck_array[gWreck_count].original_matrix, &this_car->t.t.mat, sizeof(br_matrix34));
            BrActorRelink(gWreck_root, this_car);
            this_car->render_style = BR_RSTYLE_FACES;
            gWreck_array[gWreck_count].customised = 0;
            gWreck_array[gWreck_count].actor = this_car;
            PrepareBoundingRadius__racesumm(this_car->model);
            gWreck_array[gWreck_count].scaling_factor = .47f / this_car->model->radius;
            gWreck_array[gWreck_count].pos_x = (position % 3) - 1.0f;
            gWreck_array[gWreck_count].pos_y = (position / 3) - 0.5f;
            this_car->t.t.translate.t.v[0] = 1.5f * gWreck_array[gWreck_count].pos_x;
            this_car->t.t.translate.t.v[1] = -1.2f * gWreck_array[gWreck_count].pos_y;
            this_car->t.t.translate.t.v[2] = 0.f;
            gWreck_array[gWreck_count].car_type = cat;
            gWreck_array[gWreck_count].car_index = i;
            gWreck_count += 1;
            position += 1;
        }
    }
#ifdef DETHRACE_3DFX_PATCH
    if (gScreen->type == BR_PMT_INDEX_8) {
#endif
        gWreck_render_area = BrPixelmapAllocateSub(
            gBack_screen,
            gCurrent_graf_data->wreck_render_x,
            gCurrent_graf_data->wreck_render_y,
            gCurrent_graf_data->wreck_render_w,
            gCurrent_graf_data->wreck_render_h);
#ifdef DETHRACE_3DFX_PATCH
    } else {
        gWreck_render_area = BrPixelmapAllocateSub(
            gReal_back_screen,
            gCurrent_graf_data->wreck_render_x * 2,
            gCurrent_graf_data->wreck_render_y * 2 + HIRES_Y_OFFSET,
            gCurrent_graf_data->wreck_render_w * 2,
            gCurrent_graf_data->wreck_render_h * 2);
    }
#endif
    gWreck_render_area->origin_x = gWreck_render_area->width / 2;
    gWreck_render_area->origin_y = gWreck_render_area->height / 2;

#ifdef DETHRACE_3DFX_PATCH
    if (gScreen->type == BR_PMT_INDEX_8) {
#endif
        gWreck_z_buffer = BrPixelmapAllocateSub(
            gDepth_buffer,
            gCurrent_graf_data->wreck_render_x,
            gCurrent_graf_data->wreck_render_y,
            gCurrent_graf_data->wreck_render_w,
            gCurrent_graf_data->wreck_render_h);
#ifdef DETHRACE_3DFX_PATCH
    } else {
        gWreck_z_buffer = gDepth_buffer;
    }
#endif
}

// IDA: void __cdecl DisposeWrecks()
// FUNCTION: CARM95 0x00417eba
void DisposeWrecks(void) {
    int cat;
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;

    for (cat = eVehicle_self; cat < eVehicle_rozzer; cat++) {
        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (i = 0; i < car_count; i++) {
            if (cat == eVehicle_self) {
                the_car = &gProgram_state.current_car;
            } else {
                the_car = GetCarSpec(cat, i);
            }
            this_car = the_car->car_model_actors[the_car->principal_car_actor].actor;
            BrActorRelink(the_car->car_master_actor, this_car);
            this_car->render_style = BR_RSTYLE_NONE;
        }
    }
    for (i = 0; i < gWreck_count; i++) {
        memcpy(&gWreck_array[i].actor->t.t.mat, &gWreck_array[i].original_matrix, sizeof(br_matrix34));
    }
    BrActorRemove(gWreck_root);
    BrActorRemove(gWreck_camera);
    BrActorFree(gWreck_root);
    BrActorFree(gWreck_camera);
    gWreck_render_area->pixels = NULL;
    BrPixelmapFree(gWreck_render_area);

#ifdef DETHRACE_3DFX_PATCH
    if (gScreen->type == BR_PMT_INDEX_8) {
        gWreck_z_buffer->pixels = NULL;
        BrPixelmapFree(gWreck_z_buffer);
    }
#endif
}

// IDA: int __usercall MatrixIsIdentity@<EAX>(br_matrix34 *pMat@<EAX>)
// FUNCTION: CARM95 0x00416f4e
int MatrixIsIdentity(br_matrix34* pMat) {

    return (pMat->m[0][0] == 1.f && pMat->m[1][1] == 1.f && pMat->m[2][2] == 1.f && pMat->m[0][1] == 0.f && pMat->m[0][2] == 0.f && pMat->m[1][0] == 0.f && pMat->m[1][2] == 0.f && pMat->m[2][0] == 0.f && pMat->m[2][1] == 0.f);
}

// IDA: void __usercall SpinWrecks(tU32 pFrame_period@<EAX>)
// FUNCTION: CARM95 0x00416d47
void SpinWrecks(tU32 pFrame_period) {
    int i;
    br_vector3 translation;
    br_matrix34 old_mat;

    for (i = 0; i < gWreck_count; i++) {
        if (gWreck_array[i].customised == 0) {
            BrMatrix34RotateY(&gWreck_array[i].rotation, BrDegreeToAngle(0.05 * pFrame_period));
        }
        memcpy(&translation, &gWreck_array[i].actor->t.t.translate.t, sizeof(br_vector3));
        BrVector3Set(&gWreck_array[i].actor->t.t.translate.t, 0.f, 0.f, 0.f);
        BrMatrix34Post(&gWreck_array[i].actor->t.t.mat, &gWreck_array[i].rotation);
        if (!MatrixIsIdentity(&gWreck_array[i].actor->t.t.mat)) {
            memcpy(&old_mat, &gWreck_array[i].actor->t.t.mat, sizeof(br_matrix34));
            BrMatrix34LPNormalise(&gWreck_array[i].actor->t.t.mat, &old_mat);
            BrMatrix34PostScale(&gWreck_array[i].actor->t.t.mat,
                gWreck_array[i].scaling_factor, gWreck_array[i].scaling_factor, gWreck_array[i].scaling_factor);
        }
        memcpy(&gWreck_array[i].actor->t.t.translate.t, &translation, sizeof(br_vector3));
    }
}

// IDA: void __usercall ZoomInTo(int pIndex@<EAX>, int *pCurrent_choice@<EDX>, int *pCurrent_mode@<EBX>)
// FUNCTION: CARM95 0x0041778f
void ZoomInTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {

    gWreck_zoom_in = pIndex;
    gWreck_zoom_out = -1;
    gWreck_start_zoom = PDGetTotalTime();
    if (gWreck_selected == 0 && !gUser_interacted) {
        *pCurrent_choice = 2;
        *pCurrent_mode = 1;
    } else {
        *pCurrent_choice = 0;
        *pCurrent_mode = 0;
    }
    RemoveTransientBitmaps(1);
    TurnFlicTransparencyOn();
    RunFlicAt(325, 9, 174);
    TurnFlicTransparencyOff();
    gBack_button_ptr->new_choice = -1;
    gBack_button_ptr->new_mode = -1;
}

// IDA: void __usercall ZoomOutTo(int pIndex@<EAX>, int *pCurrent_choice@<EDX>, int *pCurrent_mode@<EBX>)
// FUNCTION: CARM95 0x00416413
void ZoomOutTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {

    gWreck_zoom_out = pIndex;
    gWreck_zoom_in = -1;
    gWreck_start_zoom = PDGetTotalTime();
    RemoveTransientBitmaps(1);
    TurnFlicTransparencyOn();
    RunFlicAt(326, 9, 174);
    TurnFlicTransparencyOff();
    memcpy(gBack_button_ptr, &gOld_back_button, sizeof(tMouse_area));
    *pCurrent_choice = 1;
    *pCurrent_mode = 1;
}

// IDA: int __cdecl WreckPick(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pNear, br_scalar pFar, void *pArg)
// FUNCTION: CARM95 0x0041660d
int WreckPick(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pNear, br_scalar pFar, void* pArg) {
    int i;

    for (i = 0; i < gWreck_count; i++) {
        if (gWreck_array[i].actor == pActor) {
            gWreck_selected = i;
            return 1;
        }
    }
    return 0;
}

// IDA: int __usercall CastSelectionRay@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x00416486
int CastSelectionRay(int* pCurrent_choice, int* pCurrent_mode) {
    int mouse_x;
    int mouse_y;
    int i;
    int result;
    br_scalar inv_wreck_pick_scale_factor;

    if (gMouse_in_use
        && *pCurrent_choice == 0
        && *pCurrent_mode == 0
        && gWreck_zoomed_in < 0
        && gWreck_zoom_out < 0
        && gWreck_zoom_in < 0) {

        inv_wreck_pick_scale_factor = 0.5f;
        GetMousePosition(&mouse_x, &mouse_y);
        if (gReal_graf_data_index != 0) {
            mouse_x = 2 * mouse_x;
            mouse_y = 2 * mouse_y + HIRES_Y_OFFSET;
        }
        for (i = 0; i < gWreck_count; i++) {
            BrMatrix34PreScale(&gWreck_array[i].actor->t.t.mat, 2.f, 2.f, 2.f);
        }
        result = DRScenePick2DXY(gWreck_root, gWreck_camera, gRender_screen,
            mouse_x - gBack_screen->width / 2,
            mouse_y - gBack_screen->height / 2,
            WreckPick,
            NULL);
        for (i = 0; i < gWreck_count; i++) {
            BrMatrix34PreScale(&gWreck_array[i].actor->t.t.mat,
                inv_wreck_pick_scale_factor, inv_wreck_pick_scale_factor, inv_wreck_pick_scale_factor);
        }
        return result;
    } else {
        return 0;
    }
}

// IDA: int __usercall DamageScrnExit@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x0041636e
int DamageScrnExit(int* pCurrent_choice, int* pCurrent_mode) {

    if (gProgram_state.prog_status == eProg_idling) {
        return 1;
    }
    if (gWreck_gallery_start == 0) {
        gWreck_gallery_start = PDGetTotalTime();
    } else if (!gDone_initial && gWreck_selected == 0) {
        if (PDGetTotalTime() - gWreck_gallery_start > 1500) {
            ZoomOutTo(gWreck_selected, pCurrent_choice, pCurrent_mode);
            gDone_initial = 1;
        }
    }
    CastSelectionRay(pCurrent_choice, pCurrent_mode);
    return 0;
}

// IDA: void __usercall DamageScrnDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x00416671
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

    if (((pCurrent_choice == 0 && pCurrent_mode == 0) || !gDone_initial) && (gWreck_zoomed_in < 0 && gWreck_selected >= 0)) {
        sel_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        sel_actor->render_style = BR_RSTYLE_BOUNDING_EDGES;
        sel_actor->model = gWreck_array[gWreck_selected].actor->model;
        BrActorAdd(gWreck_array[gWreck_selected].actor, sel_actor);
    } else {
        sel_actor = NULL;
    }
    the_time = PDGetTotalTime();
    SpinWrecks(the_time - gLast_wreck_draw);
    gLast_wreck_draw = the_time;
    if (gWreck_zoom_out >= 0 || gWreck_zoom_in >= 0) {
        if (gWreck_start_zoom == 0) {
            gWreck_start_zoom = the_time;
        }

        if (the_time - gWreck_start_zoom > 1000) {
            the_time = gWreck_start_zoom + 1000;
            finished = 1;
        } else {
            finished = 0;
        }
        if (gWreck_zoom_out >= 0) {
            memcpy(&camera_movement, &gWreck_array[gWreck_zoom_out].actor->t.t.translate.t, sizeof(br_vector3));
            camera_movement.v[2] = -1.45f;
            BrVector3Scale(&camera_movement, &camera_movement, (the_time - gWreck_start_zoom) / 1000.f);
            if (finished) {
                gWreck_zoomed_in = gWreck_zoom_out;
                gWreck_zoom_out = -1;
            }
        } else {
            memcpy(&camera_movement, &gWreck_array[gWreck_zoom_in].actor->t.t.translate.t, sizeof(br_vector3));
            camera_movement.v[2] = -1.45f;
            BrVector3Scale(&camera_movement, &camera_movement, 1.f - (the_time - gWreck_start_zoom) / 1000.f);
            if (finished) {
                gWreck_zoom_in = -1;
                gWreck_zoomed_in = -1;
            }
        }
        camera_movement.v[2] += 2.2f;
        memcpy(&gWreck_camera->t.t.translate.t, &camera_movement, sizeof(br_vector3));
    }
    EnsureRenderPalette();
    EnsurePaletteUp();
#ifdef DETHRACE_3DFX_PATCH
    if (gScreen->type == BR_PMT_INDEX_8) {
#endif
        BrPixelmapFill(gWreck_z_buffer, 0xffffffff);
        BrPixelmapFill(gWreck_render_area, BR_COLOUR_RGBA(0xb0, 0xb0, 0xb0, 0xb0));
        spacing = 15.f;
        rows = gWreck_render_area->height / spacing;
        columns = gWreck_render_area->width / spacing;
        for (v = 0; v <= rows; v++) {
            BrPixelmapLine(gWreck_render_area,
                -gWreck_render_area->origin_x,
                gWreck_render_area->height / 2.0 - spacing * (rows / 2.0 - v) - gWreck_render_area->origin_y,
                gWreck_render_area->width - gWreck_render_area->origin_x,
                gWreck_render_area->height / 2.0 - spacing * (rows / 2.0 - v) - gWreck_render_area->origin_y,
                8);
        }
        for (h = 0; h <= columns; h++) {
            BrPixelmapLine(gWreck_render_area,
                gWreck_render_area->width / 2.0 - spacing * (columns / 2.0 - h) - gWreck_render_area->origin_x,
                -gWreck_render_area->origin_y,
                gWreck_render_area->width / 2.0 - spacing * (columns / 2.0 - h) - gWreck_render_area->origin_x,
                gWreck_render_area->height - gWreck_render_area->origin_y,
                8);
        }
        BrZbSceneRenderBegin(gUniverse_actor, gWreck_camera, gWreck_render_area, gWreck_z_buffer);
        BrZbSceneRenderAdd(gWreck_root);
        BrZbSceneRenderEnd();
        if (sel_actor != NULL) {
            BrActorRemove(sel_actor);
            sel_actor->model = NULL;
            BrActorFree(sel_actor);
        }
        BrPixelmapRectangleFill(gBack_screen,
            gCurrent_graf_data->wreck_name_left,
            gCurrent_graf_data->wreck_name_top,
            gCurrent_graf_data->wreck_name_right - gCurrent_graf_data->wreck_name_left,
            gCurrent_graf_data->wreck_name_bottom - gCurrent_graf_data->wreck_name_top,
            0);
        if (gWreck_selected >= 0 && (gWreck_zoomed_in >= 0 || pCurrent_mode == 0)) {
            name = GetDriverName(gWreck_array[gWreck_selected].car_type,
                gWreck_array[gWreck_selected].car_index);
            TransBrPixelmapText(gBack_screen,
                (gCurrent_graf_data->wreck_name_left + gCurrent_graf_data->wreck_name_right - BrPixelmapTextWidth(gBack_screen, gFont_7, name)) / 2,
                gCurrent_graf_data->wreck_name_base_line,
                84,
                gFont_7,
                name);
        }
#ifdef DETHRACE_3DFX_PATCH
    } else {
        rows = gCurrent_graf_data->wreck_render_h * 0.0666666f;
        columns = (double)gCurrent_graf_data->wreck_render_w * 0.0666666f;

        BrPixelmapRectangleFill(gBack_screen, gCurrent_graf_data->wreck_render_x, gCurrent_graf_data->wreck_render_y, gCurrent_graf_data->wreck_render_w, gCurrent_graf_data->wreck_render_h, 0xB0B0B0B0);

        for (v = 0; v <= rows; v++) {
            BrPixelmapLine(
                gBack_screen,
                gCurrent_graf_data->wreck_render_x,
                gCurrent_graf_data->wreck_render_h / 2.0f + gCurrent_graf_data->wreck_render_y - (rows / 2.0f - v) * 15.0f,
                gCurrent_graf_data->wreck_render_w + gCurrent_graf_data->wreck_render_x,
                gCurrent_graf_data->wreck_render_h / 2.0f + gCurrent_graf_data->wreck_render_y - (rows / 2.0f - v) * 15.0f,
                8);
        }
        for (h = 0; h <= columns; h++) {
            BrPixelmapLine(gBack_screen,
                gCurrent_graf_data->wreck_render_w / 2.0f + gCurrent_graf_data->wreck_render_x - (columns / 2.0f - h) * 15.0,
                gCurrent_graf_data->wreck_render_y,
                gCurrent_graf_data->wreck_render_w / 2.0f + gCurrent_graf_data->wreck_render_x - (columns / 2.0f - h) * 15.0,
                gCurrent_graf_data->wreck_render_h + gCurrent_graf_data->wreck_render_y,
                8);
        }
        if (sel_actor) {
            BrActorRemove(sel_actor);
            sel_actor->model = NULL;
            BrActorFree(sel_actor);
        }
        BrPixelmapRectangleFill(
            gBack_screen,
            gCurrent_graf_data->wreck_name_left,
            gCurrent_graf_data->wreck_name_top,
            gCurrent_graf_data->wreck_name_right - gCurrent_graf_data->wreck_name_left,
            gCurrent_graf_data->wreck_name_bottom - gCurrent_graf_data->wreck_name_top,
            0);
        if (gWreck_selected >= 0 && (gWreck_zoomed_in >= 0 || pCurrent_mode == 0)) {
            name = GetDriverName(gWreck_array[gWreck_selected].car_type, gWreck_array[gWreck_selected].car_index);
            TransBrPixelmapText(gBack_screen,
                (gCurrent_graf_data->wreck_name_left + gCurrent_graf_data->wreck_name_right - BrPixelmapTextWidth(gBack_screen, gFont_7, name)) / 2,
                gCurrent_graf_data->wreck_name_base_line,
                84,
                gFont_7,
                name);
        }
        CopyBackScreen(0);
        BrPixelmapFill(gWreck_z_buffer, 0xFFFFFFFF);
        PDUnlockRealBackScreen(1);

        // Added by dethrace
        // 3d scene is drawn on top of the 2d hud, so we must ensure that all the 2d pixel
        // writes have been flushed to the framebuffer first
        BrPixelmapFlush(gReal_back_screen);
        // -

        BrZbSceneRenderBegin(gUniverse_actor, gWreck_camera, gWreck_render_area, gWreck_z_buffer);
        BrZbSceneRenderAdd(gWreck_root);
        BrZbSceneRenderEnd();
        PDLockRealBackScreen(1);
    }
#endif
}

// IDA: int __usercall DamageScrnLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x00417038
int DamageScrnLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int i;

    gDone_initial = 1;
    DRS3StartSound(gEffects_outlet, 3000);
    if (*pCurrent_mode == 0 && gWreck_zoomed_in < 0) {
        if (gWreck_selected < 0) {
            gWreck_selected = gWreck_count - 1;
        } else if (gWreck_selected == 0 || gWreck_array[gWreck_selected - 1].pos_y != gWreck_array[gWreck_selected].pos_y) {
            for (i = gWreck_count - 1; i >= 0; i--) {
                if (gWreck_array[i].pos_y == gWreck_array[gWreck_selected].pos_y) {
                    gWreck_selected = i;
                    break;
                }
            }
        } else {
            gWreck_selected--;
        }
    } else if (gWreck_zoomed_in >= 0) {
        *pCurrent_choice = *pCurrent_choice + 1;
        if (*pCurrent_choice == 3) {
            *pCurrent_choice = 1;
        }
    }
    return 1;
}

// IDA: int __usercall DamageScrnRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x00417175
int DamageScrnRight(int* pCurrent_choice, int* pCurrent_mode) {
    int i;

    gDone_initial = 1;
    DRS3StartSound(gEffects_outlet, 3000);
    if (*pCurrent_mode == 0 && gWreck_zoomed_in < 0) {
        if (gWreck_selected < 0) {
            gWreck_selected = 0;
        } else if (gWreck_count - 1 == gWreck_selected || gWreck_array[gWreck_selected + 1].pos_y != gWreck_array[gWreck_selected].pos_y) {
            for (i = 0; i < gWreck_count; i++) {
                if (gWreck_array[i].pos_y == gWreck_array[gWreck_selected].pos_y) {
                    gWreck_selected = i;
                    break;
                }
            }
        } else {
            gWreck_selected++;
        }
    } else if (gWreck_zoomed_in >= 0) {
        *pCurrent_choice = *pCurrent_choice + 1;
        if (*pCurrent_choice == 3) {
            *pCurrent_choice = 1;
        }
    }
    return 1;
}

// IDA: int __usercall DamageScrnUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004172b9
int DamageScrnUp(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;

    gDone_initial = 1;
    DRS3StartSound(gEffects_outlet, 3000);
    if (*pCurrent_mode != 0) {
        *pCurrent_mode = 0;
        *pCurrent_choice = 0;
        if (gWreck_zoomed_in < 0) {
            gWreck_selected = gWreck_count + -1;
        }
    } else {
        if (gWreck_selected < 0) {
            if (gWreck_zoomed_in < 0) {
                gWreck_selected = gWreck_count - 1;
            }
        } else if (gWreck_zoomed_in >= 0) {
            *pCurrent_mode = 1;
            *pCurrent_choice = 1;
        } else if (gWreck_array[gWreck_selected].pos_y == gWreck_array[0].pos_y) {
            *pCurrent_mode = 1;
            *pCurrent_choice = 2;
        } else {
            difference = 1000;
            new_selection = gWreck_selected;
            for (i = 0; i < gWreck_count; i++) {
                if (gWreck_array[gWreck_selected].pos_y - 1.f == gWreck_array[i].pos_y) {
                    if (gWreck_array[i].pos_x == gWreck_array[gWreck_selected].pos_x) {
                        new_selection = i;
                        break;
                    } else {
                        new_difference = abs((int)(gWreck_array[i].pos_x - gWreck_array[gWreck_selected].pos_x));
                        if (new_difference < difference) {
                            difference = new_difference;
                            new_selection = i;
                        }
                    }
                }
            }
            gWreck_selected = new_selection;
        }
    }
    return 1;
}

// IDA: int __usercall DamageScrnDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004174a9
int DamageScrnDown(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;

    gDone_initial = 1;
    DRS3StartSound(gEffects_outlet, 3000);
    if (*pCurrent_mode != 0) {
        *pCurrent_mode = 0;
        *pCurrent_choice = 0;
        if (gWreck_zoomed_in < 0) {
            for (i = gWreck_count - 1; i >= 0; i--) {
                if (gWreck_array[i].pos_y == gWreck_array[0].pos_y) {
                    gWreck_selected = i;
                    break;
                }
            }
        }
    } else {
        if (gWreck_selected < 0) {
            gWreck_selected = 0;
        } else if (gWreck_zoomed_in >= 0) {
            *pCurrent_mode = 1;
            *pCurrent_choice = 1;
        } else if (gWreck_array[gWreck_selected].pos_y == gWreck_array[gWreck_count - 1].pos_y) {
            *pCurrent_mode = 1;
            *pCurrent_choice = 2;
        } else {
            difference = 1000;
            new_selection = gWreck_selected;
            for (i = 0; i < gWreck_count; i++) {
                if (gWreck_array[gWreck_selected].pos_y + 1.f == gWreck_array[i].pos_y) {
                    if (gWreck_array[i].pos_x == gWreck_array[gWreck_selected].pos_x) {
                        new_selection = i;
                        break;
                    } else {
                        new_difference = abs((int)(gWreck_array[i].pos_x - gWreck_array[gWreck_selected].pos_x));
                        if (new_difference < difference) {
                            difference = new_difference;
                            new_selection = i;
                        }
                    }
                }
            }
            gWreck_selected = new_selection;
        }
    }
    return 1;
}

// IDA: int __usercall DamageScrnGoHead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004176dd
int DamageScrnGoHead(int* pCurrent_choice, int* pCurrent_mode) {

    gDone_initial = 1;
    if (*pCurrent_choice == 2) {
        return 1;
    } else if (gWreck_zoomed_in >= 0) {
        if (*pCurrent_choice == 1) {
            ZoomInTo(gWreck_selected, pCurrent_choice, pCurrent_mode);
        }
    } else if (*pCurrent_choice == 1) {
        if (*pCurrent_choice == 0 && gWreck_selected >= 0) {
            ZoomOutTo(gWreck_selected, pCurrent_choice, pCurrent_mode);
            gUser_interacted = 1;
        }
    }
    return 0;
}

// IDA: int __usercall ClickDamage@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
// FUNCTION: CARM95 0x00417839
int ClickDamage(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int mouse_x;
    int mouse_y;
    int old_mouse_x;
    int old_mouse_y;

#if defined(DETHRACE_FIX_BUGS)
    old_mouse_x = 0; // Fixes warning caused by -Wsometimes-uninitialized
    old_mouse_y = 0; // Fixes warning caused by -Wsometimes-uninitialized
#endif
    GetMousePosition(&old_mouse_x, &old_mouse_y);
    if (gWreck_zoomed_in >= 0) {
        do {
            GetMousePosition(&mouse_x, &mouse_y);
            BrMatrix34RollingBall(&gWreck_array[gWreck_zoomed_in].rotation, mouse_x - old_mouse_x, old_mouse_y - mouse_y, 50);
            old_mouse_x = mouse_x;
            old_mouse_y = mouse_y;
            gWreck_array[gWreck_zoomed_in].customised = 1;
            RemoveTransientBitmaps(1);
            DamageScrnDraw(0, 0);
            ProcessFlicQueue(gFrame_period);
            DoMouseCursor();
            PDScreenBufferSwap(0);
            ServiceGame();
        } while (EitherMouseButtonDown());
        return 0;
    } else {
        if (CastSelectionRay(pCurrent_choice, pCurrent_mode)) {
            gUser_interacted = 1;
            return 1;
        } else {
            return 0;
        }
    }
}

// IDA: int __usercall DamageScrnDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
// FUNCTION: CARM95 0x0041794f
int DamageScrnDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {

    if (pTimed_out) {
        return 2;
    } else {
        return pCurrent_choice;
    }
}

// IDA: tSO_result __cdecl DoEndRaceSummary2()
// FUNCTION: CARM95 0x0041797b
tSO_result DoEndRaceSummary2(void) {
    static tFlicette flicker_on[3] = {
        { -1, { 0, 0 }, { 0, 0 } },
        { 321, { 9, 18 }, { 174, 418 } },
        { 321, { 247, 494 }, { 174, 418 } },
    };
    static tFlicette flicker_off[3] = {
        { -1, { 0, 0 }, { 0, 0 } },
        { 322, { 9, 18 }, { 174, 418 } },
        { 322, { 247, 494 }, { 174, 418 } },
    };
    static tFlicette push[3] = {
        { -1, { 0, 0 }, { 0, 0 } },
        { 324, { 9, 18 }, { 174, 418 } },
        { 323, { 247, 494 }, { 174, 418 } },
    };
    static tMouse_area mouse_areas[3] = {
        { { 11, 22 }, { 20, 48 }, { 309, 618 }, { 169, 406 }, 0, 0, 0, ClickDamage },
        { { 9, 18 }, { 174, 418 }, { 72, 144 }, { 194, 466 }, 1, 1, 0, NULL },
        { { 247, 494 }, { 174, 418 }, { 310, 620 }, { 194, 466 }, 2, 1, 0, NULL },
    };
    static tInterface_spec interface_spec = {
        1, 320, 0, -1, -1, -1, -1,
        { -1, -1 }, { 0, 0 }, { 0, 0 }, { 0, 2 }, { DamageScrnLeft, DamageScrnLeft },
        { -1, -1 }, { 0, 0 }, { 0, 0 }, { 0, 2 }, { DamageScrnRight, DamageScrnRight },
        { 1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 }, { DamageScrnUp, DamageScrnUp },
        { 1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 }, { DamageScrnDown, DamageScrnDown },
        { 1, 1 }, { DamageScrnGoHead, DamageScrnGoHead },
        { 1, 1 }, { NULL, NULL },
        DamageScrnExit, DamageScrnDraw,
        20000, NULL, NULL, DamageScrnDone, 0,
        { 0, 0 }, NULL, 2, 1,
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas,
        0, NULL
    };
    int result;

    if (gAusterity_mode) {
        return eSO_continue;
    }
    NetPlayerStatusChanged(ePlayer_status_wrecks_gallery);
    gBack_button_ptr = &mouse_areas[1];
    memcpy(&gOld_back_button, &mouse_areas[1], sizeof(tMouse_area));
    gOld_back_button.new_choice = 1;
    gOld_back_button.new_mode = 1;
    memcpy(gBack_button_ptr, &gOld_back_button, sizeof(tMouse_area));
    BuildWrecks();
    gWreck_zoom_out = -1;
    gWreck_zoom_in = -1;
    gWreck_start_zoom = 0;
    gWreck_zoomed_in = -1;
    gWreck_selected = 0;
    gUser_interacted = 0;
    gDone_initial = 0;
    TurnOnPaletteConversion();
    gWreck_gallery_start = 0;
    result = DoInterfaceScreen(&interface_spec, 0, 2);
    NetPlayerStatusChanged(ePlayer_status_loading);
    TurnOffPaletteConversion();
    DisposeWrecks();
    if (result < 0) {
        return eSO_main_menu_invoked;
    }
    return eSO_continue;
}

// IDA: void __usercall DrawAnItem(int pX@<EAX>, int pY_index@<EDX>, int pFont_index@<EBX>, char *pText@<ECX>)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004183be
void DrawAnItem__racesumm(int pX, int pY_index, int pFont_index, char* pText) {

    TransBrPixelmapText(gBack_screen,
        pX,
        gCurrent_graf_data->net_sum_headings_y + gCurrent_graf_data->net_sum_y_pitch * pY_index,
        pFont_index,
        gFont_7,
        pText);
}

// IDA: void __usercall DrawColumnHeading(int pStr_index@<EAX>, int pX@<EDX>)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x00418405
void DrawColumnHeading__racesumm(int pStr_index, int pX) {

    TransBrPixelmapText(gBack_screen,
        pX,
        gCurrent_graf_data->net_sum_headings_y - gCurrent_graf_data->net_sum_y_pitch,
        250,
        gFont_7,
        GetMiscString(pStr_index));
}

// IDA: int __usercall SortScores@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
// FUNCTION: CARM95 0x0041853a
int SortScores(const void* pFirst_one, const void* pSecond_one) {

    return gNet_players[*(int*)pSecond_one].games_score - gNet_players[*(int*)pFirst_one].games_score;
}

// IDA: void __cdecl SortGameScores()
// FUNCTION: CARM95 0x00418515
void SortGameScores(void) {
    qsort(gPlayer_lookup, gNumber_of_net_players, sizeof(gPlayer_lookup[0]), SortScores);
}

// IDA: void __usercall NetSumDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x0041804b
void NetSumDraw(int pCurrent_choice, int pCurrent_mode) {
    int i;
    char s[256];
    tNet_game_player_info* player;

    DrawColumnHeading__racesumm(kMiscString_PLAYED, gCurrent_graf_data->net_sum_x_3);
    DrawColumnHeading__racesumm(kMiscString_WON, gCurrent_graf_data->net_sum_x_4);
    DrawColumnHeading__racesumm(kMiscString_SCORE, gCurrent_graf_data->net_sum_x_5);
    BrPixelmapLine(gBack_screen,
        gCurrent_graf_data->net_sum_x_1,
        gCurrent_graf_data->net_sum_headings_y + 1 + gFont_7->glyph_y - gCurrent_graf_data->net_sum_y_pitch,
        gBack_screen->width - gCurrent_graf_data->net_sum_x_1,
        gCurrent_graf_data->net_sum_headings_y + 1 + gFont_7->glyph_y - gCurrent_graf_data->net_sum_y_pitch,
        252);

    for (i = 0; i < gNumber_of_net_players; i++) {
        player = &gNet_players[gPlayer_lookup[i]];

        strcpy(s, player->player_name);
        if (player->host) {
            strcat(s, " -");
            strcat(s, GetMiscString(kMiscString_HOST));
            strcat(s, "-");
        }
        TurnOffPaletteConversion();
        DRPixelmapRectangleMaskedCopy(gBack_screen,
            gCurrent_graf_data->net_sum_x_1,
            gCurrent_graf_data->net_sum_headings_y + 1 + i * gCurrent_graf_data->net_sum_y_pitch,
            gIcons_pix_low_res, /* DOS version uses low res, Windows version uses normal res */
            0,
            gCurrent_graf_data->net_head_icon_height * player->car_index,
            gIcons_pix_low_res->width, /* DOS version uses low res, Windows version uses normal res */
            gCurrent_graf_data->net_head_icon_height);
        TurnOnPaletteConversion();
        DrawAnItem__racesumm(gCurrent_graf_data->net_sum_x_2, i, 83, s);
        sprintf(s, "%d", player->played);
        DrawAnItem__racesumm(gCurrent_graf_data->net_sum_x_3, i, 83, s);
        sprintf(s, "%d", player->won);
        DrawAnItem__racesumm(gCurrent_graf_data->net_sum_x_4, i, 83, s);
        sprintf(s, "%d", player->games_score);
        DrawAnItem__racesumm(gCurrent_graf_data->net_sum_x_5, i, 83, s);
    }
}

// IDA: void __cdecl DoNetRaceSummary()
// FUNCTION: CARM95 0x00418452
void DoNetRaceSummary(void) {
    static tFlicette flicker_on[1] = { { 321, { 219, 112 }, { 172, 362 } } };
    static tFlicette flicker_off[1] = { { 322, { 219, 112 }, { 172, 362 } } };
    static tFlicette push[1] = { { 323, { 219, 112 }, { 172, 362 } } };
    static tMouse_area mouse_areas[1] = { { { 219, 112 }, { 172, 362 }, { 282, 182 }, { 192, 379 }, 0, 0, 0, NULL } };
    static tInterface_spec interface_spec = {
        0,              // initial_imode
        63,             // first_opening_flic
        0,              // second_opening_flic
        -1,             // end_flic_go_ahead
        -1,             // end_flic_escaped
        -1,             // end_flic_otherwise
        8,              // flic_bunch_to_load
        { -1, 0 },      // move_left_new_mode
        { 0, 0 },       // move_left_delta
        { 0, 0 },       // move_left_min
        { 0, 0 },       // move_left_max
        { NULL, NULL }, // move_left_proc
        { -1, 0 },      // move_right_new_mode
        { 0, 0 },       // move_right_delta
        { 0, 0 },       // move_right_min
        { 0, 0 },       // move_right_max
        { NULL, NULL }, // move_right_proc
        { -1, 0 },      // move_up_new_mode
        { 0, 0 },       // move_up_delta
        { 0, 0 },       // move_up_min
        { 0, 0 },       // move_up_max
        { NULL, NULL }, // move_up_proc
        { -1, 0 },      // move_down_new_mode
        { 0, 0 },       // move_down_delta
        { 0, 0 },       // move_down_min
        { 0, 0 },       // move_down_max
        { NULL, NULL }, // move_down_proc
        { 1, 1 },       // go_ahead_allowed
        { NULL, NULL }, // go_ahead_proc
        { 1, 1 },       // escape_allowed
        { NULL, NULL }, // escape_proc
        NULL,           // exit_proc
        &NetSumDraw,    // draw_proc
        10000,          // time_out
        NULL,           // start_proc1
        NULL,           // start_proc2
        NULL,           // done_proc
        0,              // font_needed
        { 0, 0 },       // typeable
        NULL,           // get_original_string
        0,              // escape_code
        1,              // dont_save_or_load
        1,              // number_of_button_flics
        flicker_on,     // flicker_on_flics
        flicker_off,    // flicker_off_flics
        push,           // pushed_flics
        1,              // number_of_mouse_areas
        mouse_areas,    // mouse_areas
        0,              // number_of_recopy_areas
        NULL            // recopy_areas
    };
    int i;
    int result;
    tS32 start_time;

    NetPlayerStatusChanged(ePlayer_status_summary);
    start_time = PDGetTotalTime();
    while (!gReceived_game_scores && PDGetTotalTime() - start_time < 20000) {
        NetService(0);
    }
    if (gReceived_game_scores) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            gPlayer_lookup[i] = i;
        }
        SortGameScores();
        TurnOnPaletteConversion();
        DoInterfaceScreen(&interface_spec, 0, 0);
        NetPlayerStatusChanged(ePlayer_status_loading);
        TurnOffPaletteConversion();
        FadePaletteDown();
    }
}

// IDA: tSO_result __usercall DoEndRaceSummary@<EAX>(int *pFirst_summary_done@<EAX>, tRace_result pRace_result@<EDX>)
// FUNCTION: CARM95 0x00418590
tSO_result DoEndRaceSummary(int* pFirst_summary_done, tRace_result pRace_result) {
    tSO_result result;

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
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
