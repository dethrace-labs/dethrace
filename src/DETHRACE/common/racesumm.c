#include "racesumm.h"
#include "brender/brender.h"
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
void CalcRankIncrease(void) {
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
void DrawSummaryItems(void) {
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
void StartSummary(void) {
    LOG_TRACE("()");

    DrawSummaryItems();
    gSummary_start = PDGetTotalTime();
}

// IDA: void __cdecl SetUpTemps()
void SetUpTemps(void) {
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
}

// IDA: tSO_result __cdecl DoEndRaceSummary1()
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

    max = 0.f;
    for (v = 0; v < model->nvertices; v++) {
        vp = &model->vertices[v];
        d = BrVector3LengthSquared(&vp->p);
        if (d > max) {
            max = d;
        }
    }
    d = sqrtf(max);
    model->radius = d;
}

// IDA: void __cdecl BuildWrecks()
void BuildWrecks(void) {
    int cat;
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;
    LOG_TRACE("()");

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
    gWreck_render_area = BrPixelmapAllocateSub(
        gBack_screen,
        gCurrent_graf_data->wreck_render_x,
        gCurrent_graf_data->wreck_render_y,
        gCurrent_graf_data->wreck_render_w,
        gCurrent_graf_data->wreck_render_h);
    gWreck_render_area->origin_x = gWreck_render_area->width / 2;
    gWreck_render_area->origin_y = gWreck_render_area->height / 2;
    gWreck_z_buffer = BrPixelmapAllocateSub(
        gDepth_buffer,
        gCurrent_graf_data->wreck_render_x,
        gCurrent_graf_data->wreck_render_y,
        gCurrent_graf_data->wreck_render_w,
        gCurrent_graf_data->wreck_render_h);
}

// IDA: void __cdecl DisposeWrecks()
void DisposeWrecks(void) {
    int cat;
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;
    LOG_TRACE("()");

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
    gWreck_z_buffer->pixels = NULL;
    BrPixelmapFree(gWreck_render_area);
    BrPixelmapFree(gWreck_z_buffer);
}

// IDA: int __usercall MatrixIsIdentity@<EAX>(br_matrix34 *pMat@<EAX>)
int MatrixIsIdentity(br_matrix34* pMat) {
    LOG_TRACE("(%p)", pMat);

    return (pMat->m[0][0] == 1.f && pMat->m[1][1] == 1.f && pMat->m[2][2] == 1.f && pMat->m[0][1] == 0.f && pMat->m[0][2] == 0.f && pMat->m[1][0] == 0.f && pMat->m[1][2] == 0.f && pMat->m[2][0] == 0.f && pMat->m[2][1] == 0.f);
}

// IDA: void __usercall SpinWrecks(tU32 pFrame_period@<EAX>)
void SpinWrecks(tU32 pFrame_period) {
    int i;
    br_vector3 translation;
    br_matrix34 old_mat;
    LOG_TRACE("(%d)", pFrame_period);

    for (i = 0; i < gWreck_count; i++) {
        if (gWreck_array[i].customised == 0) {
            BrMatrix34RotateY(&gWreck_array[i].rotation, BR_ANGLE_DEG(.05f * pFrame_period));
        }
        BrVector3Copy(&translation, &gWreck_array[i].actor->t.t.translate.t);
        BrVector3Set(&gWreck_array[i].actor->t.t.translate.t, 0.f, 0.f, 0.f);
        BrMatrix34Post(&gWreck_array[i].actor->t.t.mat, &gWreck_array[i].rotation);
        if (!MatrixIsIdentity(&gWreck_array[i].actor->t.t.mat)) {
            memcpy(&old_mat, &gWreck_array[i].actor->t.t.mat, sizeof(br_matrix34));
            BrMatrix34LPNormalise(&gWreck_array[i].actor->t.t.mat, &old_mat);
            BrMatrix34PostScale(&gWreck_array[i].actor->t.t.mat,
                gWreck_array[i].scaling_factor, gWreck_array[i].scaling_factor, gWreck_array[i].scaling_factor);
        }
        BrVector3Copy(&gWreck_array[i].actor->t.t.translate.t, &translation);
    }
}

// IDA: void __usercall ZoomInTo(int pIndex@<EAX>, int *pCurrent_choice@<EDX>, int *pCurrent_mode@<EBX>)
void ZoomInTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%d, %p, %p)", pIndex, pCurrent_choice, pCurrent_mode);

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
void ZoomOutTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%d, %p, %p)", pIndex, pCurrent_choice, pCurrent_mode);

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
int WreckPick(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pNear, br_scalar pFar, void* pArg) {
    int i;
    LOG_TRACE("(%p, %p, %p, %p, %p, %f, %f, %p)", pActor, pModel, pMaterial, pRay_pos, pRay_dir, pNear, pFar, pArg);

    for (i = 0; i < gWreck_count; i++) {
        if (gWreck_array[i].actor == pActor) {
            gWreck_selected = i;
            return 1;
        }
    }
    return 0;
}

// IDA: int __usercall CastSelectionRay@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int CastSelectionRay(int* pCurrent_choice, int* pCurrent_mode) {
    int mouse_x;
    int mouse_y;
    int i;
    int result;
    br_scalar inv_wreck_pick_scale_factor;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (!gMouse_in_use) {
        return 0;
    }
    if (*pCurrent_choice != 0) {
        return 0;
    }
    if (*pCurrent_mode != 0) {
        return 0;
    }
    if (gWreck_zoomed_in >= 0) {
        return 0;
    }
    if (gWreck_zoom_out >= 0) {
        return 0;
    }
    if (gWreck_zoom_in >= 0) {
        return 0;
    }

    inv_wreck_pick_scale_factor = 0.5f;
    GetMousePosition(&mouse_x, &mouse_y);
    if (gReal_graf_data_index != 0) {
        mouse_x = 2 * mouse_x;
        mouse_y = 2 * mouse_y + 40;
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
}

// IDA: int __usercall DamageScrnExit@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnExit(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gProgram_state.prog_status == eProg_idling) {
        return 1;
    } else {
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

    if (((pCurrent_choice == 0 && pCurrent_mode == 0) || !gDone_initial) && (gWreck_zoomed_in < 0 && gWreck_selected >= 0)) {
        sel_actor = BrActorAllocate(BR_ACTOR_MODEL, NULL);
        sel_actor->render_style = BR_RSTYLE_BOUNDING_EDGES;
        sel_actor->render_style = BR_RSTYLE_NONE; // FIXME: remove this line once BR_RSTYLE_BOUNDING_EDGES rener style has been implemente
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
        finished = the_time - gWreck_start_zoom > 1000;
        if (finished) {
            the_time = gWreck_start_zoom + 1000;
        }
        if (gWreck_zoom_out < 0) {
            camera_movement.v[0] = (1.f - (the_time - gWreck_start_zoom) / 1000.f) * gWreck_array[gWreck_zoom_in].actor->t.t.translate.t.v[0];
            camera_movement.v[1] = (1.f - (the_time - gWreck_start_zoom) / 1000.f) * gWreck_array[gWreck_zoom_in].actor->t.t.translate.t.v[1];
            camera_movement.v[2] = (1.f - (the_time - gWreck_start_zoom) / 1000.f) * -1.45f;
            if (finished) {
                gWreck_zoom_in = -1;
                gWreck_zoomed_in = -1;
            }
        } else {
            camera_movement.v[0] = (the_time - gWreck_start_zoom) / 1000.f * gWreck_array[gWreck_zoom_out].actor->t.t.translate.t.v[0];
            camera_movement.v[1] = (the_time - gWreck_start_zoom) / 1000.f * gWreck_array[gWreck_zoom_out].actor->t.t.translate.t.v[1];
            camera_movement.v[2] = (the_time - gWreck_start_zoom) / 1000.f * -1.45f;
            if (finished) {
                gWreck_zoomed_in = gWreck_zoom_out;
                gWreck_zoom_out = -1;
            }
        }
        gWreck_camera->t.t.translate.t.v[0] = camera_movement.v[0];
        gWreck_camera->t.t.translate.t.v[1] = camera_movement.v[1];
        gWreck_camera->t.t.translate.t.v[2] = camera_movement.v[2] + 2.2f;
    }
    EnsureRenderPalette();
    EnsurePaletteUp();
    BrPixelmapFill(gWreck_z_buffer, 0xffffffff);
    BrPixelmapFill(gWreck_render_area, BR_COLOUR_RGBA(0xb0, 0xb0, 0xb0, 0xb0));

    rows = gWreck_render_area->height / 15.f;
    columns = gWreck_render_area->width / 15.f;
    for (v = 0; v <= rows; v++) {
        BrPixelmapLine(gWreck_render_area,
            -gWreck_render_area->origin_x,
            gWreck_render_area->height / 2.f - 15.f * (rows / 2.f - v) - gWreck_render_area->origin_y,
            gWreck_render_area->width - gWreck_render_area->origin_x,
            gWreck_render_area->height / 2.f - 15.f * (rows / 2.f - v) - gWreck_render_area->origin_y,
            8);
    }
    for (h = 0; h <= columns; h++) {
        BrPixelmapLine(gWreck_render_area,
            gWreck_render_area->width / 2.f - 15.f * (columns / 2.f - h) - gWreck_render_area->origin_x,
            -gWreck_render_area->origin_y,
            gWreck_render_area->width / 2.f - 15.f * (columns / 2.f - h) - gWreck_render_area->origin_x,
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
}

// IDA: int __usercall DamageScrnLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    gDone_initial = 1;
    DRS3StartSound(gEffects_outlet, 3000);
    if (*pCurrent_mode == 0 && gWreck_zoomed_in < 0) {
        if (gWreck_selected < 0) {
            gWreck_selected = gWreck_count - 1;
        } else if (gWreck_selected != 0 && gWreck_array[gWreck_selected - 1].pos_y == gWreck_array[gWreck_selected].pos_y) {
            gWreck_selected--;
        } else {
            for (i = gWreck_count - 1; i >= 0; i--) {
                if (gWreck_array[i].pos_y == gWreck_array[gWreck_selected].pos_y) {
                    gWreck_selected = i;
                    break;
                }
            }
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
int DamageScrnRight(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    gDone_initial = 1;
    DRS3StartSound(gEffects_outlet, 3000);
    if (*pCurrent_mode == 0 && gWreck_zoomed_in < 0) {
        if (gWreck_selected < 0) {
            gWreck_selected = 0;
        } else if (gWreck_selected - 1 != gWreck_count && gWreck_array[gWreck_selected + 1].pos_y == gWreck_array[gWreck_selected].pos_y) {
            gWreck_selected++;
        } else {
            for (i = 0; i < gWreck_count; i++) {
                if (gWreck_array[i].pos_y == gWreck_array[gWreck_selected].pos_y) {
                    gWreck_selected = i;
                    break;
                }
            }
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
int DamageScrnUp(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

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
            new_difference = 1000;
            new_selection = gWreck_selected;
            for (i = 0; i < gWreck_count; i++) {
                if (gWreck_array[gWreck_selected].pos_y - 1.f == gWreck_array[i].pos_y) {
                    if (gWreck_array[i].pos_x == gWreck_array[gWreck_selected].pos_x) {
                        new_selection = i;
                        break;
                    }
                    difference = abs((int)(gWreck_array[i].pos_x - gWreck_array[gWreck_selected].pos_x));
                    if (difference < new_difference) {
                        new_selection = i;
                        new_difference = difference;
                    }
                }
            }
            gWreck_selected = new_selection;
        }
    }
    return 1;
}

// IDA: int __usercall DamageScrnDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnDown(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

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
            new_difference = 1000;
            new_selection = gWreck_selected;
            for (i = 0; i < gWreck_count; i++) {
                if (gWreck_array[gWreck_selected].pos_y + 1.f == gWreck_array[i].pos_y) {
                    if (gWreck_array[i].pos_x == gWreck_array[gWreck_selected].pos_x) {
                        new_selection = i;
                        break;
                    }
                    difference = abs((int)(gWreck_array[i].pos_x - gWreck_array[gWreck_selected].pos_x));
                    if (difference < new_difference) {
                        new_selection = i;
                        new_difference = difference;
                    }
                }
            }
            gWreck_selected = new_selection;
        }
    }
    return 1;
}

// IDA: int __usercall DamageScrnGoHead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DamageScrnGoHead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    gDone_initial = 1;
    if (*pCurrent_choice == 2) {
        return 1;
    } else if (gWreck_zoomed_in < 0) {
        if (*pCurrent_choice == 0 && gWreck_selected >= 0) {
            ZoomOutTo(gWreck_selected, pCurrent_choice, pCurrent_mode);
            gUser_interacted = 1;
        }
    } else if (*pCurrent_choice == 1) {
        ZoomInTo(gWreck_selected, pCurrent_choice, pCurrent_mode);
    }
    gDone_initial = 1;
    return 0;
}

// IDA: int __usercall ClickDamage@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int ClickDamage(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int mouse_x;
    int mouse_y;
    int old_mouse_x;
    int old_mouse_y;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);

#if defined(DETHRACE_FIX_BUGS)
    old_mouse_x = 0; // Fixes warning caused by -Wsometimes-uninitialized
    old_mouse_y = 0; // Fixes warning caused by -Wsometimes-uninitialized
#endif
    GetMousePosition(&old_mouse_y, &old_mouse_y);
    if (gWreck_zoomed_in < 0) {
        if (CastSelectionRay(pCurrent_choice, pCurrent_mode)) {
            gUser_interacted = 1;
            return 1;
        } else {
            return 0;
        }
    } else {
        while (1) {
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
            if (!EitherMouseButtonDown()) {
                break;
            }
        }
        return 0;
    }
}

// IDA: int __usercall DamageScrnDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int DamageScrnDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);

    if (pTimed_out) {
        pCurrent_choice = 2;
    }
    return pCurrent_choice;
}

// IDA: tSO_result __cdecl DoEndRaceSummary2()
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
    LOG_TRACE("()");

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
void SortGameScores(void) {
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
void DoNetRaceSummary(void) {
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
