#include "racestrt.h"
#include "brender.h"
#include "displays.h"
#include "drmem.h"
#include "flicplay.h"
#include "globvars.h"
#include "grafdata.h"
#include "graphics.h"
#include "input.h"
#include "intrface.h"
#include "loading.h"
#include "pd/sys.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"
#include <stdlib.h>

int gGrid_number_colour[4];
br_pixelmap* gDead_car;
int gFade_away_parts_shop;
tU32 gDare_start_time;
int gRefund_rate;
int gSwap_grid_2;
int gSwap_grid_1;
int gChange_race_net_mode;
tParts_category gPart_category;
tU32 gNet_synch_start;
tNet_game_details* gChoose_car_net_game;
int gPart_index;
int gChallenger_index;
tGrid_draw gDraw_grid_status;
tNet_sequence_type gNet_race_sequence;
tU32 gLast_host_query;
br_pixelmap* gTaken_image;
int gGrid_number_x_coords[31];
int gGrid_transition_stage;
int gJust_bought_part;
int gGrid_y_adjust;
br_pixelmap* gBullet_image;
br_pixelmap* gDeceased_image;
int gBest_pos_available;
int gChallenger_position;
int gOpponent_index;
int gChallenge_time;
int gOriginal_position;
int gCurrent_race_index;
tInterface_spec* gStart_interface_spec;
int gCurrent_car_index;
int gOur_starting_position;

// IDA: void __usercall DrawRaceList(int pOffset@<EAX>)
void DrawRaceList(int pOffset) {
    int i;
    int font_height;
    int pitch;
    int y;
    int left_most;
    int right_most;
    int rank_colour;
    int text_colour;
    int box_bot;
    int text_width;
    int text_x;
    char rank_str[256];
    LOG_TRACE("(%d)", pOffset);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MoveRaceList(int pFrom@<EAX>, int pTo@<EDX>, tS32 pTime_to_move@<EBX>)
void MoveRaceList(int pFrom, int pTo, tS32 pTime_to_move) {
    tS32 start_time;
    tS32 the_time;
    int move_distance;
    int pitch;
    LOG_TRACE("(%d, %d, %d)", pFrom, pTo, pTime_to_move);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall UpRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int UpRace(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DownRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DownRace(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ClickOnRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int ClickOnRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int x_coord;
    int y_coord;
    int race_delta;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall UpClickRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int UpClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DownClickRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int DownClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl StartChangeRace()
void StartChangeRace() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ChangeRace@<EAX>(int *pRace_index@<EAX>, int pNet_mode@<EDX>, tNet_sequence_type pNet_race_sequence@<EBX>)
int ChangeRace(int* pRace_index, int pNet_mode, tNet_sequence_type pNet_race_sequence) {
    static tFlicette flicker_on[4];
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[5];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("(%p, %d, %d)", pRace_index, pNet_mode, pNet_race_sequence);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoChangeRace()
void DoChangeRace() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawCar(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawCar(int pCurrent_choice, int pCurrent_mode) {
    char s[64];
    int text_x;
    int text_width;
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetCarFlic()
void SetCarFlic() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall UpCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int UpCar(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DownCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DownCar(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall UpClickCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int UpClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DownClickCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int DownClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ChangeCarGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int ChangeCarGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ChangeCar@<EAX>(int pNet_mode@<EAX>, int *pCar_index@<EDX>, tNet_game_details *pNet_game@<EBX>)
int ChangeCar(int pNet_mode, int* pCar_index, tNet_game_details* pNet_game) {
    static tFlicette flicker_on[4];
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[4];
    static tInterface_spec interface_spec;
    int i;
    int result;
    int power_up_levels[3];
    LOG_TRACE("(%d, %p, %p)", pNet_mode, pCar_index, pNet_game);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoChangeCar()
void DoChangeCar() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PartsShopRecommended()
int PartsShopRecommended() {
    int running_cost;
    int i;
    int current_index;
    int counter;
    LOG_TRACE("()");

    STUB();
    return 0;
}

// IDA: void __usercall CalcPartPrice(int pCategory@<EAX>, int pIndex@<EDX>, int *pPrice@<EBX>, int *pCost@<ECX>)
void CalcPartPrice(int pCategory, int pIndex, int* pPrice, int* pCost) {
    int current_value;
    LOG_TRACE("(%d, %d, %p, %p)", pCategory, pIndex, pPrice, pCost);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall BuyPart@<EAX>(int pCategory@<EAX>, int pIndex@<EDX>)
int BuyPart(int pCategory, int pIndex) {
    int price;
    int cost;
    LOG_TRACE("(%d, %d)", pCategory, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoAutoParts()
void DoAutoParts() {
    int i;
    int lowest_yet;
    int lowest_one;
    int price;
    int cost;
    int current_level;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DrawPartsLabel()
void DrawPartsLabel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ErasePartsText(int pTotal_as_well@<EAX>)
void ErasePartsText(int pTotal_as_well) {
    LOG_TRACE("(%d)", pTotal_as_well);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DrawPartsText()
void DrawPartsText() {
    int price;
    int cost;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetPartsImage()
void SetPartsImage() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetPartsMax()
int GetPartsMax() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CalcPartsIndex()
void CalcPartsIndex() {
    int current_index;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoExchangePart()
void DoExchangePart() {
    int price;
    int cost;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PartsShopGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int PartsShopGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    int flic_index;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall UpPart@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int UpPart(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DownPart@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DownPart(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall UpClickPart@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int UpClickPart(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DownClickPart@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int DownClickPart(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PartsArrowsOn@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int PartsArrowsOn(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PartsArrowsOff@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int PartsArrowsOff(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl StartPartsShop()
void StartPartsShop() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DonePartsShop@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int DonePartsShop(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawPartsShop(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawPartsShop(int pCurrent_choice, int pCurrent_mode) {
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoPartsShop(int pFade_away@<EAX>)
void DoPartsShop(int pFade_away) {
    static tFlicette flicker_on[7];
    static tFlicette flicker_off[7];
    static tFlicette push[7];
    static tMouse_area mouse_areas[7];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("(%d)", pFade_away);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall AutoPartsDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int AutoPartsDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
    NOT_IMPLEMENTED();
}

// IDA: tSO_result __cdecl DoAutoPartsShop()
tSO_result DoAutoPartsShop() {
    static tFlicette flicker_on[3];
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetOpponentFlic()
void SetOpponentFlic() {
    LOG_TRACE("()");
    return ChangePanelFlic(
        0,
        gOpponents[gCurrent_race.opponent_list[gOpponent_index].index].mug_shot_image_data,
        gOpponents[gCurrent_race.opponent_list[gOpponent_index].index].mug_shot_image_data_length);
}

// IDA: void __cdecl DrawSceneyMappyInfoVieweyThing()
void DrawSceneyMappyInfoVieweyThing() {
    LOG_TRACE("()");

    RemoveTransientBitmaps(1);
    if (gProgram_state.view_type) {
        if (gProgram_state.view_type == eVT_Info) {
            ChangePanelFlic(0, gCurrent_race.info_image_data, gCurrent_race.info_image_data_length);
        } else if (gProgram_state.view_type == eVT_Opponents) {
            SetOpponentFlic();
        }
    } else {
        ChangePanelFlic(0, gCurrent_race.scene_image_data, gCurrent_race.scene_image_data_length);
    }
    TellyInImage(GetPanelPixelmap(0), gCurrent_graf_data->start_race_panel_left, gCurrent_graf_data->start_race_panel_top);
}

// IDA: void __cdecl DismissSceneyMappyInfoVieweyThing()
void DismissSceneyMappyInfoVieweyThing() {
    LOG_TRACE("()");

    RemoveTransientBitmaps(1);
    TellyOutImage(GetPanelPixelmap(0), gCurrent_graf_data->start_race_panel_left, gCurrent_graf_data->start_race_panel_top);
}

// IDA: int __usercall SelectRaceDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int SelectRaceDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);

    DismissSceneyMappyInfoVieweyThing();
    if (pEscaped) {
        return -1;
    }
    return pCurrent_choice;
}

// IDA: int __usercall StartRaceGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int StartRaceGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (*pCurrent_choice != 1 || *pCurrent_mode) {
        return 1;
    }
    RemoveTransientBitmaps(1);
    DismissSceneyMappyInfoVieweyThing();
    if (++gProgram_state.view_type > eVT_Opponents) {
        gProgram_state.view_type = eVT_Scene;
    }
    if (gProgram_state.view_type) {
        if (gProgram_state.view_type == eVT_Info) {
            RunFlic(210);
        } else if (gProgram_state.view_type == eVT_Opponents) {
            RunFlic(212);
        }
    } else {
        RunFlic(213);
    }
    DrawSceneyMappyInfoVieweyThing();
    return 0;
}

// IDA: int __usercall TryToMoveToArrows@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int TryToMoveToArrows(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gProgram_state.view_type != eVT_Opponents) {
        return 0;
    }
    *pCurrent_choice = 5;
    *pCurrent_mode = 1;
    DRS3StartSound(gIndexed_outlets[0], 3000);
    return 1;
}

// IDA: int __usercall UpOpponent@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int UpOpponent(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DownOpponent@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DownOpponent(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall UpClickOpp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int UpClickOpp(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DownClickOpp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int DownClickOpp(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SelectRaceStart()
void SelectRaceStart() {
    LOG_TRACE("()");

    DrawSceneyMappyInfoVieweyThing();
    PrintMemoryDump(0, "INSIDE START RACE");
}

// IDA: int __cdecl SuggestRace()
int SuggestRace() {
    int i;
    int least_done;
    int suggested_so_far;
    int suggested_race;
    int new_suggestion;
    int number_of_visits;
    LOG_TRACE("()");

    suggested_so_far = 32767;
    suggested_race = 0;
    if (gProgram_state.game_completed) {
        return IRandomBetween(0, gNumber_of_races - 1);
    }
    if (gProgram_state.redo_race_index >= 0) {
        return gProgram_state.redo_race_index;
    }
    for (i = 0; i < gNumber_of_races; i++) {
        if (gRace_list[i].suggested_rank <= suggested_so_far && gRace_list[i].suggested_rank >= gProgram_state.rank) {
            suggested_so_far = gRace_list[i].suggested_rank;
            suggested_race = i;
        }
    }
    number_of_visits = gRace_list[suggested_race].been_there_done_that;
    new_suggestion = suggested_race;

    if (number_of_visits) {
        // Jeff: if we have already completed the suggested race, look backwards for a race that we haven't completed as many times
        for (i = suggested_race - 1; i >= 0 && i >= suggested_race - 5; i--) {
            if (gRace_list[i].rank_required < gProgram_state.rank || gRace_list[i].best_rank > gProgram_state.rank) {
                continue;
            }
            least_done = gRace_list[i].been_there_done_that < number_of_visits;
            if (!gRace_list[i].been_there_done_that
                || (least_done && suggested_race - 3 <= i)) {
                new_suggestion = i;
                number_of_visits = gRace_list[i].been_there_done_that;
            }
        }
        // Jeff: look forwards for a race that we haven't completed as many times as the previous suggestion
        for (i = suggested_race + 1; i < gNumber_of_races; i++) {
            least_done = gRace_list[i].been_there_done_that < number_of_visits;
            if (!least_done) {
                continue;
            }

            if ((gRace_list[i].rank_required >= gProgram_state.rank && gRace_list[i].best_rank <= gProgram_state.rank)
                || gProgram_state.game_completed) {
                new_suggestion = i;
                number_of_visits = gRace_list[i].been_there_done_that;
            }
        }
    }
    return new_suggestion;
}

// IDA: void __usercall SelectRaceDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void SelectRaceDraw(int pCurrent_choice, int pCurrent_mode) {
    tOpponent* the_opponent;
    tText_chunk* the_chunk;
    int j;
    int k;
    int y_coord;
    char s[256];
    char temp_str[256];
    char* sub_pt;
    char sub_str[16];
    tU32* test;
    static tU32 test2;
    LOG_TRACE8("(%d, %d)", pCurrent_choice, pCurrent_mode);

    if (gProgram_state.view_type == 2) {
        the_opponent = &gOpponents[gCurrent_race.opponent_list[gOpponent_index].index];
        the_chunk = the_opponent->text_chunks;
        for (j = 0; j < the_opponent->text_chunk_count; j++) {
            if (GetPanelFlicFrameIndex(0) >= the_chunk->frame_cue && GetPanelFlicFrameIndex(0) < the_chunk->frame_end) {
                y_coord = the_chunk->y_coord * gGraf_specs[gGraf_spec_index].total_height / 200
                    + gCurrent_graf_data->start_race_panel_top;
                for (k = 0; k < the_chunk->line_count; k++) {
                    TransBrPixelmapText(
                        gBack_screen,
                        the_chunk->x_coord * gGraf_specs[gGraf_spec_index].total_width / 320
                            + gCurrent_graf_data->start_race_panel_left,
                        y_coord,
                        0xC9u,
                        gFont_7,
                        (signed char*)the_chunk->text[j]);
                    y_coord += gFont_7->glyph_y + gFont_7->glyph_y / 2;
                }
            }
            ++the_chunk;
        }
    } else if (gProgram_state.view_type == 1) {
        the_chunk = gCurrent_race.text_chunks;
        for (j = 0; j < gCurrent_race.text_chunk_count; j++) {
            if (GetPanelFlicFrameIndex(0) >= the_chunk->frame_cue && GetPanelFlicFrameIndex(0) < the_chunk->frame_end) {
                y_coord = the_chunk->y_coord * gGraf_specs[gGraf_spec_index].total_height / 200
                    + gCurrent_graf_data->start_race_panel_top;
                for (k = 0; k < the_chunk->line_count; k++) {
                    TransBrPixelmapText(
                        gBack_screen,
                        the_chunk->x_coord * gGraf_specs[gGraf_spec_index].total_width / 320
                            + gCurrent_graf_data->start_race_panel_left,
                        y_coord,
                        0xC9u,
                        gFont_7,
                        (signed char*)the_chunk->text[k]);
                    y_coord += gFont_7->glyph_y + gFont_7->glyph_y / 2;
                }
            }
            ++the_chunk;
        }
    }
    test = KevKeyService();
    if (*test) {
        test2 = *test;
    }
    if (*test == 0x27645433 && test[1] == 0x758F0015) {
        gProgram_state.game_completed = 1;
        DRS3StartSound(gIndexed_outlets[0], 3202);
        DRS3StartSound(gIndexed_outlets[0], 3202);
    }
    if (*test == 0x33F75455 && test[1] == 0xC10AAAF2) {
        char s[128];
        FILE* f;
        int i;

        // Jeff
        assert(strlen(gApplication_path) < 120);

        PathCat(s, gApplication_path, "ACTORS");
        PathCat(s, s, "PROG.ACT");
        PDFileUnlock(s);
        f = fopen(s, "wb");
        if (f) {
            DRS3StartSound(gIndexed_outlets[0], 9000);
            if (gDecode_thing) {
                for (i = 0; i < strlen(gDecode_string); i++) {
                    gDecode_string[i] -= 50;
                }
                fputs(gDecode_string, f);
                for (i = 0; i < strlen(gDecode_string); i++) {
                    gDecode_string[i] += 50;
                }
            } else {
                for (i = 0; i < 20; i++) {
                    fputs("*************", f);
                }
            }
            gDecode_thing ^= 0x40u;
            fclose(f);
            EncodeAllFilesInDirectory("");
            EncodeAllFilesInDirectory("CARS");
            EncodeAllFilesInDirectory("NONCARS");
            EncodeAllFilesInDirectory("RACES");
            EncodeAllFilesInDirectory("32X20X8");
            EncodeAllFilesInDirectory("64X48X8");
            PathCat(s, "32X20X8", "CARS");
            EncodeAllFilesInDirectory(s);
            PathCat(s, "64X48X8", "CARS");
            EncodeAllFilesInDirectory(s);
            PathCat(s, "32X20X8", "FONTS");
            EncodeAllFilesInDirectory(s);
            PathCat(s, "64X48X8", "FONTS");
            EncodeAllFilesInDirectory(s);
        }
        DRS3StartSound(gIndexed_outlets[0], 9000);
    }
}

// IDA: tSO_result __usercall DoSelectRace@<EAX>(int *pSecond_time_around@<EAX>)
tSO_result DoSelectRace(int* pSecond_time_around) {
    static tFlicette flicker_on[7] = {
        { 43, { 224, 448 }, { 28, 67 } },
        { 43, { 224, 448 }, { 56, 134 } },
        { 43, { 224, 448 }, { 85, 204 } },
        { 43, { 224, 448 }, { 113, 271 } },
        { 43, { 224, 448 }, { 147, 353 } },
        { 221, { 30, 60 }, { 79, 190 } },
        { 221, { 30, 60 }, { 79, 190 } }
    };

    static tFlicette flicker_off[7] = {
        { 42, { 224, 448 }, { 28, 67 } },
        { 42, { 224, 448 }, { 56, 134 } },
        { 42, { 224, 448 }, { 85, 204 } },
        { 42, { 224, 448 }, { 113, 271 } },
        { 42, { 224, 448 }, { 147, 353 } },
        { 220, { 30, 60 }, { 79, 190 } },
        { 220, { 30, 60 }, { 79, 190 } }
    };

    static tFlicette push[7] = {
        { 195, { 224, 448 }, { 28, 67 } },
        { -1, { 224, 448 }, { 56, 134 } },
        { 200, { 224, 448 }, { 85, 204 } },
        { 202, { 224, 448 }, { 113, 271 } },
        { 201, { 224, 448 }, { 147, 353 } },
        { 222, { 30, 60 }, { 79, 190 } },
        { 225, { 30, 60 }, { 119, 286 } }
    };

    static tMouse_area mouse_areas[7] = {
        { { 224, 448 }, { 28, 67 }, { 287, 574 }, { 48, 115 }, 0, 0, 0, NULL },
        { { 224, 448 }, { 56, 134 }, { 287, 574 }, { 76, 182 }, 1, 0, 0, NULL },
        { { 224, 448 }, { 85, 204 }, { 287, 574 }, { 105, 252 }, 2, 0, 0, NULL },
        { { 224, 448 }, { 113, 271 }, { 287, 574 }, { 133, 319 }, 3, 0, 0, NULL },
        { { 224, 448 }, { 147, 353 }, { 287, 574 }, { 167, 401 }, 4, 0, 0, NULL },
        { { 30, 60 },
            { 79, 190 },
            { 45, 90 },
            { 106, 254 },
            -1,
            0,
            0,
            &UpClickOpp },
        { { 30, 60 },
            { 119, 286 },
            { 45, 90 },
            { 146, 350 },
            -1,
            0,
            0,
            &DownClickOpp }
    };

    static tInterface_spec interface_spec = {
        0, // initial_imode
        191, // first_opening_flic
        190, // second_opening_flic
        0, // end_flic_go_ahead
        0, // end_flic_escaped
        0, // end_flic_otherwise
        6, // flic_bunch_to_load
        { -1, 0 }, // move_left_new_mode
        { 0, -4 }, // move_left_delta
        { 0, 1 }, // move_left_min
        { 0, 1 }, // move_left_max
        { &TryToMoveToArrows, NULL }, // move_left_proc
        { -1, 0 }, // move_right_new_mode
        { 0, -4 }, // move_right_delta
        { 0, 1 }, // move_right_min
        { 0, 1 }, // move_right_max
        { &TryToMoveToArrows, NULL }, // move_right_proc
        { -1, -1 }, // move_up_new_mode
        { -1, 0 }, // move_up_delta
        { 0, 5 }, // move_up_min
        { 4, 5 }, // move_up_max
        { NULL, &UpOpponent }, // move_up_proc
        { -1, -1 }, // move_down_new_mode
        { 1, 0 }, // move_down_delta
        { 0, 5 }, // move_down_min
        { 4, 5 }, // move_down_max
        { NULL, &DownOpponent }, // move_down_proc
        { 1, 1 }, // go_ahead_allowed
        { &StartRaceGoAhead, NULL }, // go_ahead_proc
        { 1, 1 }, // escape_allowed
        { NULL, NULL }, // escape_proc
        NULL, // exit_proc
        &SelectRaceDraw, // draw_proc
        0u, // time_out
        NULL, // start_proc1
        &SelectRaceStart, // start_proc2
        &SelectRaceDone, // done_proc
        0, // font_needed
        { 0, 0 }, // typeable
        NULL, // get_original_string
        -1, // escape_code
        1, // dont_save_or_load
        7, // number_of_button_flics
        flicker_on, // flicker_on_flics
        flicker_off, // flicker_off_flics
        push, // pushed_flics
        7, // number_of_mouse_areas
        mouse_areas, // mouse_areas
        0, // number_of_recopy_areas
        NULL // recopy_areas
    };

    int result;
    int default_choice;
    int suggested;
    int old_current_race;
    LOG_TRACE("(%p)", pSecond_time_around);

    suggested = SuggestRace();
    if (!*pSecond_time_around) {
        gProgram_state.current_race_index = suggested;
        SelectOpponents(&gCurrent_race);
    }
    gProgram_state.parts_shop_visited = 0;
    gProgram_state.dont_load = 1;
    gDeceased_image = LoadPixelmap("DECEASED.PIX");
    InitialiseFlicPanel(
        0,
        gCurrent_graf_data->start_race_panel_left,
        gCurrent_graf_data->start_race_panel_top,
        gCurrent_graf_data->start_race_panel_right - gCurrent_graf_data->start_race_panel_left,
        gCurrent_graf_data->start_race_panel_bottom - gCurrent_graf_data->start_race_panel_top);
    LoadRaceInfo(gProgram_state.current_race_index, &gCurrent_race);
    do {
        gProgram_state.view_type = 0;
        gOpponent_index = 0;
        gStart_interface_spec = &interface_spec;
        if (gFaded_palette || gCurrent_splash) {
            result = DoInterfaceScreen(&interface_spec, 1, 4);
        } else {
            result = DoInterfaceScreen(&interface_spec, 0, 4);
        }

        if (!result || result == 2 || result == 3) {
            DisposeFlicPanel(0);

            if (result == 2) {
                RunFlic(192);
                DoPartsShop(0);
            } else if (result == 3) {
                RunFlic(192);
                DoChangeCar();
            } else if (result == 0) {
                RunFlic(192);
                old_current_race = gProgram_state.current_race_index;
                DoChangeRace();
                if (gProgram_state.current_race_index != old_current_race) {
                    DisposeRaceInfo(&gCurrent_race);
                    SelectOpponents(&gCurrent_race);
                    LoadRaceInfo(gProgram_state.current_race_index, &gCurrent_race);
                }
            }
            InitialiseFlicPanel(
                0,
                gCurrent_graf_data->start_race_panel_left,
                gCurrent_graf_data->start_race_panel_top,
                gCurrent_graf_data->start_race_panel_right - gCurrent_graf_data->start_race_panel_left,
                gCurrent_graf_data->start_race_panel_bottom - gCurrent_graf_data->start_race_panel_top);
        }
    } while (result >= 0 && result != 4);
    BrPixelmapFree(gDeceased_image);
    FillInRaceInfo(&gCurrent_race);
    DisposeRaceInfo(&gCurrent_race);
    DisposeFlicPanel(0);
    *pSecond_time_around = 1;
    gProgram_state.dont_load = 0;
    if (result >= 0) {
        *pSecond_time_around = 1;
        if (gProgram_state.parts_shop_visited || !PartsShopRecommended()) {
            FadePaletteDown();
            return eSO_continue;
        } else {
            RunFlic(192);
            return DoAutoPartsShop();
        }
    } else {
        RunFlic(192);
        gInterface_within_race_mode = 1;
        return eSO_main_menu_invoked;
    }
}

// IDA: void __usercall DrawGridCar(int pX@<EAX>, int pY@<EDX>, br_pixelmap *pImage@<EBX>)
void DrawGridCar(int pX, int pY, br_pixelmap* pImage) {
    LOG_TRACE("(%d, %d, %p)", pX, pY, pImage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawGrid(int pOffset@<EAX>, int pDraw_it@<EDX>)
void DrawGrid(int pOffset, int pDraw_it) {
    int i;
    int j;
    int y;
    int x;
    int str_x;
    int width_job;
    int done_highest;
    int str_index;
    int swap_1_x;
    int swap_1_y;
    int swap_2_x;
    int swap_2_y;
    br_pixelmap* the_image;
    br_pixelmap* swap_1_image;
    br_pixelmap* swap_2_image;
    char numbers_str[4][100];
    char total_str[256];
    tU32 the_time;
    LOG_TRACE("(%d, %d)", pOffset, pDraw_it);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MoveGrid(int pFrom@<EAX>, int pTo@<EDX>, tS32 pTime_to_move@<EBX>)
void MoveGrid(int pFrom, int pTo, tS32 pTime_to_move) {
    tS32 start_time;
    tS32 the_time;
    int move_distance;
    int pitch;
    LOG_TRACE("(%d, %d, %d)", pFrom, pTo, pTime_to_move);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CalcGridOffset@<EAX>(int pPosition@<EAX>)
int CalcGridOffset(int pPosition) {
    LOG_TRACE("(%d)", pPosition);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GridDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void GridDraw(int pCurrent_choice, int pCurrent_mode) {
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ActuallySwapOrder(int pFirst_index@<EAX>, int pSecond_index@<EDX>)
void ActuallySwapOrder(int pFirst_index, int pSecond_index) {
    tOpp_spec temp_opp;
    LOG_TRACE("(%d, %d)", pFirst_index, pSecond_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoGridTransition(int pFirst_index@<EAX>, int pSecond_index@<EDX>)
void DoGridTransition(int pFirst_index, int pSecond_index) {
    tU32 start_time;
    tU32 the_time;
    LOG_TRACE("(%d, %d)", pFirst_index, pSecond_index);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ChallengeStart()
void ChallengeStart() {
    br_pixelmap* the_map;
    int i;
    int j;
    int line_count;
    int dare_index;
    FILE* f;
    tPath_name the_path;
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CheckNextStage@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int CheckNextStage(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ChallengeDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int ChallengeDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoChallengeScreen()
void DoChallengeScreen() {
    static tFlicette flicker_on[2];
    static tFlicette flicker_off[2];
    static tFlicette push[2];
    static tMouse_area mouse_areas[2];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GridDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int GridDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GridStart()
void GridStart() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GridMoveLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GridMoveLeft(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GridMoveRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GridMoveRight(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GridClickCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int GridClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int rel_pos;
    int new_pos;
    int base_pos;
    int x_coord;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GridClickNumbers@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int GridClickNumbers(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int new_pos;
    int i;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GridClickLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int GridClickLeft(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GridClickRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int GridClickRight(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CheckChallenge@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int CheckChallenge(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FindBestPos@<EAX>(int pOur_rank@<EAX>)
int FindBestPos(int pOur_rank) {
    int i;
    LOG_TRACE("(%d)", pOur_rank);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SortGridFunction@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
int SortGridFunction(void* pFirst_one, void* pSecond_one) {
    LOG_TRACE("(%p, %p)", pFirst_one, pSecond_one);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SortOpponents()
void SortOpponents() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tSO_result __cdecl DoGridPosition()
tSO_result DoGridPosition() {
    static tFlicette flicker_on[3];
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[5];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckPlayersAreResponding()
void CheckPlayersAreResponding() {
    int i;
    tU32 time;
    tNet_message* message;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NetSynchStartStart()
void NetSynchStartStart() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetSynchStartDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void NetSynchStartDraw(int pCurrent_choice, int pCurrent_mode) {
    int i;
    int number_ready;
    char s[256];
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetSynchStartDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int NetSynchStartDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetSynchStartGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NetSynchStartGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ExitWhenReady@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int ExitWhenReady(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: tSO_result __usercall NetSynchRaceStart2@<EAX>(tNet_synch_mode pMode@<EAX>)
tSO_result NetSynchRaceStart2(tNet_synch_mode pMode) {
    static tFlicette flicker_on_hf[2];
    static tFlicette flicker_off_hf[2];
    static tFlicette push_hf[2];
    static tMouse_area mouse_areas_hf[2];
    static tInterface_spec interface_spec_hf;
    static tFlicette flicker_on_hs[1];
    static tFlicette flicker_off_hs[1];
    static tFlicette push_hs[1];
    static tMouse_area mouse_areas_hs[1];
    static tInterface_spec interface_spec_hs;
    static tFlicette flicker_on_c[1];
    static tFlicette flicker_off_c[1];
    static tFlicette push_c[1];
    static tMouse_area mouse_areas_c[1];
    static tInterface_spec interface_spec_c;
    int result;
    LOG_TRACE("(%d)", pMode);
    NOT_IMPLEMENTED();
}

// IDA: tSO_result __cdecl NetSynchRaceStart()
tSO_result NetSynchRaceStart() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
