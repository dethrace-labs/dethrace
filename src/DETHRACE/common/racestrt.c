#include "racestrt.h"
#include "brender/brender.h"
#include "displays.h"
#include "drmem.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/trace.h"
#include "input.h"
#include "intrface.h"
#include "loading.h"
#include "newgame.h"
#include "opponent.h"
#include "pd/sys.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

int gGrid_number_colour[4] = { 49u, 201u, 1u, 201u };
int gJust_bought_part;
tU32 gLast_host_query;
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
int gChallenger_index__racestrt; // suffix added to avoid duplicate symbol
tGrid_draw gDraw_grid_status;
tNet_sequence_type gNet_race_sequence__racestrt; // suffix added to avoid duplicate symbol
br_pixelmap* gTaken_image;
int gGrid_number_x_coords[31];
int gGrid_transition_stage;
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

    left_most = (gCurrent_graf_data->choose_race_rank_right - 2 * gBig_font->width[48] + gCurrent_graf_data->choose_race_left) / 2;
    right_most = gCurrent_graf_data->choose_race_right - (left_most - gCurrent_graf_data->choose_race_left);
    BrPixelmapRectangleFill(gBack_screen,
        left_most, 
        gCurrent_graf_data->choose_race_y_top,
        gCurrent_graf_data->choose_race_right - gCurrent_graf_data->choose_race_left - 2 * (left_most - gCurrent_graf_data->choose_race_left),
        gCurrent_graf_data->choose_race_y_bottom - gCurrent_graf_data->choose_race_y_top,
        0);
    pitch = gCurrent_graf_data->choose_race_y_pitch;
    for (i = 0; i < gNumber_of_races; i++) {
        y = pitch * i + gCurrent_graf_data->choose_race_curr_y - pOffset;
        if (gCurrent_graf_data->choose_race_y_top <= (y - (TranslationMode() ? 2 : 0)) && (y + gBig_font->glyph_y) < gCurrent_graf_data->choose_race_line_y) {
            if ((gProgram_state.rank > gRace_list[i].rank_required || gProgram_state.rank < gRace_list[i].best_rank) && !gProgram_state.game_completed && !gChange_race_net_mode) {
                rank_colour = 44;
                text_colour = 49;
            } else if (gCurrent_graf_data->choose_race_curr_y == y) {
                rank_colour = 5;
                text_colour = 1;
            } else {
                rank_colour = 198;
                text_colour = 201;
            }
            if (!gChange_race_net_mode) {
                sprintf(rank_str, "%2d", gRace_list[i].rank_required);
                TransBrPixelmapText(gBack_screen,
                    gCurrent_graf_data->choose_race_rank_right - BrPixelmapTextWidth(gBack_screen, gBig_font, rank_str),
                    y,
                    rank_colour,
                    gBig_font,
                    (signed char*)rank_str);
            }
            TransBrPixelmapText(gBack_screen,
                gCurrent_graf_data->choose_race_name_left,
                y,
                text_colour,
                gBig_font,
                (signed char*)gRace_list[i].name);
            if (gRace_list[i].been_there_done_that && gBullet_image != NULL) {
                BrPixelmapRectangleCopy(gBack_screen,
                    gCurrent_graf_data->choose_race_bullet_left,
                    y + (gBig_font->glyph_y - gBullet_image->height) / 2,
                    gBullet_image,
                    0,
                    0,
                    gBullet_image->width,
                    gBullet_image->height);
            }
        }
    }
    if (gChange_race_net_mode) {
        strcpy(rank_str, GetMiscString(202 + (gNet_race_sequence__racestrt == 1 ? 1 : 0)));
        TransBrPixelmapText(gBack_screen,
            (right_most + left_most - BrPixelmapTextWidth(gBack_screen, gBig_font, rank_str)) / 2,
            gCurrent_graf_data->choose_race_current_text_y,
            5,
            gBig_font,
            (signed char*)rank_str);
    } else {
        sprintf(rank_str, "%s%d", GetMiscString(27), gProgram_state.rank);
        text_x = (gCurrent_graf_data->choose_race_left + gCurrent_graf_data->choose_race_right) / 2 - BrPixelmapTextWidth(gBack_screen, gBig_font, rank_str) / 2;
        TransBrPixelmapText(gBack_screen,
            text_x,
            gCurrent_graf_data->choose_race_current_text_y,
            3,
            gBig_font,
            (signed char*)GetMiscString(27));
        sprintf(rank_str, "%d", gProgram_state.rank);
        TransBrPixelmapText(gBack_screen,
            text_x + BrPixelmapTextWidth(gBack_screen, gBig_font, GetMiscString(27)),
            gCurrent_graf_data->choose_race_current_text_y,
            5,
            gBig_font,
            (signed char*)rank_str);
    }
    BrPixelmapLine(gBack_screen,
        left_most,
        gCurrent_graf_data->choose_race_line_y,
        right_most,
        gCurrent_graf_data->choose_race_line_y,
        6);
    DrawRectangle(gBack_screen,
        left_most,
        gCurrent_graf_data->choose_race_box_top - 1,
        right_most,
        2 * gCurrent_graf_data->choose_race_curr_y - gCurrent_graf_data->choose_race_box_top + gBig_font->glyph_y,
        3);
    PDScreenBufferSwap(0);
}

// IDA: void __usercall MoveRaceList(int pFrom@<EAX>, int pTo@<EDX>, tS32 pTime_to_move@<EBX>)
void MoveRaceList(int pFrom, int pTo, tS32 pTime_to_move) {
    tS32 start_time;
    tS32 the_time;
    int move_distance;
    int pitch;
    LOG_TRACE("(%d, %d, %d)", pFrom, pTo, pTime_to_move);

    pitch = gCurrent_graf_data->choose_race_y_pitch;
    start_time = PDGetTotalTime();
    while (1) {
        the_time = PDGetTotalTime();
        if (start_time + pTime_to_move <= the_time) break;
        DrawRaceList((the_time - start_time) * (pTo - pFrom) * pitch / pTime_to_move + pitch * pFrom);
    }
    DrawRaceList(pitch * pTo);
}

// IDA: int __usercall UpRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int UpRace(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    AddToFlicQueue(gStart_interface_spec->pushed_flics[2].flic_index,
        gStart_interface_spec->pushed_flics[2].x[gGraf_data_index],
        gStart_interface_spec->pushed_flics[2].y[gGraf_data_index],
        1);
    DRS3StartSound(gIndexed_outlets[0], 3000);
    if (gCurrent_race_index != 0 && (gRace_list[gCurrent_race_index - 1].best_rank <= gProgram_state.rank || gProgram_state.game_completed || gChange_race_net_mode)) {
        RemoveTransientBitmaps(1);
        MoveRaceList(gCurrent_race_index, gCurrent_race_index - 1, 150);
        gCurrent_race_index--;
    }
}

// IDA: int __usercall DownRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DownRace(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    AddToFlicQueue(gStart_interface_spec->pushed_flics[3].flic_index,
        gStart_interface_spec->pushed_flics[3].x[gGraf_data_index],
        gStart_interface_spec->pushed_flics[3].y[gGraf_data_index],
        1);
    DRS3StartSound(gIndexed_outlets[0], 3000);
    if (gCurrent_race_index < gNumber_of_races - 1 && (gProgram_state.rank <= gRace_list[gCurrent_race_index + 1].rank_required || gProgram_state.game_completed || gChange_race_net_mode)) {
        RemoveTransientBitmaps(1);
        MoveRaceList(gCurrent_race_index, gCurrent_race_index + 1, 150);
        gCurrent_race_index++;
    }
}

// IDA: int __usercall ClickOnRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int ClickOnRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int x_coord;
    int y_coord;
    int race_delta;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);

    GetMousePosition(&x_coord, &y_coord);
    race_delta = (y_coord - (gCurrent_graf_data->choose_race_curr_y - 5 * (gCurrent_graf_data->choose_race_y_pitch / 2) + gBig_font->glyph_y / 2)) / gCurrent_graf_data->choose_race_y_pitch - 2;
    if (race_delta >= -2 && race_delta <= -1) {
        do {
            UpRace(pCurrent_choice, pCurrent_mode);
            race_delta++;
        } while (race_delta != 0);
    } else if (race_delta > 0 && race_delta < 3) {
        do {
            DownRace(pCurrent_choice, pCurrent_mode);
            race_delta--;
        } while (race_delta != 0);
    }
    return 0;
}

// IDA: int __usercall UpClickRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int UpClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);

    UpRace(pCurrent_choice, pCurrent_mode);
    return 0;
}

// IDA: int __usercall DownClickRace@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int DownClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);

    DownRace(pCurrent_choice, pCurrent_mode);
    return 0;
}

// IDA: void __cdecl StartChangeRace()
void StartChangeRace() {
    LOG_TRACE("()");

    MoveRaceList(-3, gCurrent_race_index, 400);
}

// IDA: int __usercall ChangeRace@<EAX>(int *pRace_index@<EAX>, int pNet_mode@<EDX>, tNet_sequence_type pNet_race_sequence@<EBX>)
int ChangeRace(int* pRace_index, int pNet_mode, tNet_sequence_type pNet_race_sequence) {
    static tFlicette flicker_on[4] = {
        {  43, {  60, 120 }, { 154, 370 } },
        {  43, { 221, 442 }, { 154, 370 } },
        { 221, {  30,  60 }, {  78, 187 } },
        { 221, {  30,  60 }, {  78, 187 } },
    };
    static tFlicette flicker_off[4] = {
        {  42, {  60, 120 }, { 154, 370 } },
        {  42, { 221, 442 }, { 154, 370 } },
        { 220, {  30,  60 }, {  78, 187 } },
        { 220, {  30,  60 }, {  78, 187 } },
    };
    static tFlicette push[4] = {
        { 154, {  60, 120 }, { 154, 370 } },
        {  45, { 221, 442 }, { 154, 370 } },
        { 222, {  30,  60 }, {  78, 187 } },
        { 225, {  30,  60 }, { 118, 283 } },
    };
    static tMouse_area mouse_areas[5] = {
        { {  60, 120 }, { 154, 370 }, { 125, 250 }, { 174, 418 },   0,   0,   0, NULL },
        { { 221, 442 }, { 154, 370 }, { 286, 572 }, { 174, 418 },   1,   0,   0, NULL },
        { {  30,  60 }, {  78, 187 }, {  45,  90 }, { 104, 250 },  -1,   0,   0, UpClickRace },
        { {  30,  60 }, { 118, 283 }, {  45,  90 }, { 145, 348 },  -1,   0,   0, DownClickRace },
        { {  66, 132 }, {  33,  79 }, { 278, 556 }, { 144, 346 },  -1,   0,   0, ClickOnRace },
    };
    static tInterface_spec interface_spec = {
        0, 230, 60, 231, 231, 231, 6,
        { -1,  0}, { -1,  0}, {  0,  0 }, {  1,  0}, { NULL, NULL },
        { -1,  0}, {  1,  0}, {  0,  0 }, {  1,  0}, { NULL, NULL },
        { -1,  0}, {  0,  0}, {  0,  0 }, {  0,  0}, { UpRace, NULL },
        { -1,  0}, {  0,  0}, {  0,  0 }, {  0,  0}, { DownRace, NULL },
        { 1, 1 }, { NULL, NULL }, { 1, 1 }, { NULL, NULL },
        NULL, NULL, 0, NULL, StartChangeRace, NULL, 0, {0,0},
        NULL, 1, 1, COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas, 0, NULL
    };
    int result;
    LOG_TRACE("(%p, %d, %d)", pRace_index, pNet_mode, pNet_race_sequence);

    gNet_race_sequence__racestrt = pNet_race_sequence;
    gChange_race_net_mode = pNet_mode;
    gStart_interface_spec = &interface_spec;
    gCurrent_race_index = *pRace_index;
    if (pNet_mode == 0) {
        gBullet_image = LoadPixelmap("BULLET.PIX");
    } else {
        gBullet_image = NULL;
    }
    result = DoInterfaceScreen(&interface_spec, pNet_mode != 0, 0);
    if (result == 0) {
        *pRace_index = gCurrent_race_index;
    }
    if (gBullet_image != NULL) {
        BrPixelmapFree(gBullet_image);
    }
    return result == 0;
}

// IDA: void __cdecl DoChangeRace()
void DoChangeRace() {
    LOG_TRACE("()");

    if (ChangeRace(&gProgram_state.current_race_index, 0, eNet_sequence_sequential) != 0) {
        gProgram_state.current_race_index = gCurrent_race_index;
    }
}

// IDA: void __usercall DrawCar(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawCar(int pCurrent_choice, int pCurrent_mode) {
    char s[64];
    int text_x;
    int text_width;
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);

    PollCarDetails(gChoose_car_net_game);
    if (gChange_race_net_mode == 0) {
        if (gProgram_state.number_of_cars == 1) {
            sprintf(s, GetMiscString(34));
        } else if (gProgram_state.cars_available[gCurrent_car_index] == gProgram_state.frank_or_anniness) {
            sprintf(s, GetMiscString(36));
        } else {
            sprintf(s, "%s %s", GetMiscString(35), gOpponents[gProgram_state.cars_available[gCurrent_car_index]].name);
        }
    } else if (gCar_details[gProgram_state.cars_available[gCurrent_car_index]].ownership == eCar_owner_someone) {
        sprintf(s, "%s %s", GetMiscString(204), gCar_details[gProgram_state.cars_available[gCurrent_car_index]].name);
    } else {
        sprintf(s, GetMiscString(205));
    }
    text_width = BrPixelmapTextWidth(gBack_screen, gFont_7, s);
    text_x = (gCurrent_graf_data->change_car_line_right + gCurrent_graf_data->change_car_line_left - text_width) / 2;
    BrPixelmapRectangleFill(gBack_screen,
        gCurrent_graf_data->change_car_line_left,
        gCurrent_graf_data->change_car_text_y,
        gCurrent_graf_data->change_car_line_right - gCurrent_graf_data->change_car_line_left,
        gFont_7->glyph_y,
        0);
    TransBrPixelmapText(gBack_screen,
        text_x,
        gCurrent_graf_data->change_car_text_y,
        3,
        gFont_7,
        (signed char*)s);
    BrPixelmapLine(gBack_screen,
        gCurrent_graf_data->change_car_line_left,
        gCurrent_graf_data->change_car_line_y,
        gCurrent_graf_data->change_car_line_right,
        gCurrent_graf_data->change_car_line_y,
        6);
    if (gChange_race_net_mode && gCar_details[gProgram_state.cars_available[gCurrent_car_index]].ownership == eCar_owner_someone) {
        DRPixelmapRectangleMaskedCopy(
            gBack_screen,
            (gCurrent_graf_data->change_car_panel_left + gCurrent_graf_data->change_car_panel_right - gTaken_image->width) / 2,
            (gCurrent_graf_data->change_car_panel_bottom + gCurrent_graf_data->change_car_panel_bottom - gTaken_image->height) / 2,
            gTaken_image,
            0,
            0,
            gTaken_image->width,
            gTaken_image->height);
    }
}

// IDA: void __cdecl SetCarFlic()
void SetCarFlic() {
    LOG_TRACE("()");

    ChangePanelFlic(0,
        gOpponents[gProgram_state.cars_available[gCurrent_car_index]].stolen_car_image_data,
        gOpponents[gProgram_state.cars_available[gCurrent_car_index]].stolen_car_image_data_length);
}

// IDA: int __usercall UpCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int UpCar(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    AddToFlicQueue(gStart_interface_spec->pushed_flics[2].flic_index,
        gStart_interface_spec->pushed_flics[2].x[gGraf_data_index],
        gStart_interface_spec->pushed_flics[2].y[gGraf_data_index],
        1);
    DRS3StartSound(gIndexed_outlets[0], 3000);
    if (gCurrent_car_index != 0) {
        RemoveTransientBitmaps(1);
        DropOutImageThruBottom(GetPanelPixelmap(0),
            gCurrent_graf_data->change_car_panel_left,
            gCurrent_graf_data->change_car_panel_top,
            gCurrent_graf_data->change_car_panel_top_clip,
            gCurrent_graf_data->change_car_panel_bottom_clip);
        gCurrent_car_index--;
        SetCarFlic();
        DropInImageFromTop(GetPanelPixelmap(0),
            gCurrent_graf_data->change_car_panel_left,
            gCurrent_graf_data->change_car_panel_top,
            gCurrent_graf_data->change_car_panel_top_clip,
            gCurrent_graf_data->change_car_panel_bottom_clip);
    }
    return 0;
}

// IDA: int __usercall DownCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int DownCar(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    AddToFlicQueue(gStart_interface_spec->pushed_flics[3].flic_index,
        gStart_interface_spec->pushed_flics[3].x[gGraf_data_index],
        gStart_interface_spec->pushed_flics[3].y[gGraf_data_index],
        1);
    DRS3StartSound(gIndexed_outlets[0], 3000);
    if (gCurrent_car_index < gProgram_state.number_of_cars - 1) {
        RemoveTransientBitmaps(1);
        DropOutImageThruTop(GetPanelPixelmap(0),
            gCurrent_graf_data->change_car_panel_left,
            gCurrent_graf_data->change_car_panel_top,
            gCurrent_graf_data->change_car_panel_top_clip,
            gCurrent_graf_data->change_car_panel_bottom_clip);
        gCurrent_car_index++;
        SetCarFlic();
        DropInImageFromBottom(GetPanelPixelmap(0),
            gCurrent_graf_data->change_car_panel_left,
            gCurrent_graf_data->change_car_panel_top,
            gCurrent_graf_data->change_car_panel_top_clip,
            gCurrent_graf_data->change_car_panel_bottom_clip);
    }
    return 0;
}

// IDA: int __usercall UpClickCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int UpClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);

    UpCar(pCurrent_choice, pCurrent_mode);
    return 0;
}

// IDA: int __usercall DownClickCar@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int DownClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);

    DownCar(pCurrent_choice, pCurrent_mode);
    return 0;
}

// IDA: int __usercall ChangeCarGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int ChangeCarGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gChange_race_net_mode == 0 || gCar_details[gProgram_state.cars_available[gCurrent_car_index]].ownership != eCar_owner_someone) {
        return 1;
    } else {
        DRS3StartSound(gIndexed_outlets[0], 3100);
        return 0;
    }
}

// IDA: int __usercall ChangeCar@<EAX>(int pNet_mode@<EAX>, int *pCar_index@<EDX>, tNet_game_details *pNet_game@<EBX>)
int ChangeCar(int pNet_mode, int* pCar_index, tNet_game_details* pNet_game) {
    static tFlicette flicker_on[4] = {
        {  43, {  60, 120 }, { 154, 370 } },
        {  43, { 221, 442 }, { 154, 370 } },
        { 221, {  30,  60 }, {  78, 187 } },
        { 221, {  30,  60 }, {  78, 187 } },
    };
    static tFlicette flicker_off[4] = {
        {  42, {  60, 120 }, { 154, 370 } },
        {  42, { 221, 442 }, { 154, 370 } },
        { 220, {  30,  60 }, {  78, 187 } },
        { 220, {  30,  60 }, {  78, 187 } },
    };
    static tFlicette push[4] = {
        { 154, {  60, 120 }, { 154, 370 } },
        {  45, { 221, 442 }, { 154, 370 } },
        { 222, {  30,  60 }, {  78, 187 } },
        { 225, {  30,  60 }, { 118, 283 } },
    };
    static tMouse_area mouse_areas[4] = {
        { {  60, 120 }, { 154, 370 }, { 125, 250 }, { 174, 418 },   0,   0,   0, NULL },
        { { 221, 442 }, { 154, 370 }, { 286, 572 }, { 174, 418 },   1,   0,   0, NULL },
        { {  30,  60 }, {  78, 187 }, {  45,  90 }, { 104, 250 },  -1,   0,   0, UpClickCar },
        { {  30,  60 }, { 118, 283 }, {  45,  90 }, { 145, 348 },  -1,   0,   0, DownClickCar },
    };
    static tInterface_spec interface_spec = {
        0, 236, 62, 0, 0, 0, -1,
        { -1,  0 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, { NULL, NULL },
        { -1,  0 }, {  1,  0 }, {  0,  0 }, {  1,  0 }, { NULL, NULL },
        { -1,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, { UpCar, NULL },
        { -1,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, { DownCar, NULL },
        {  1,  1 }, { ChangeCarGoAhead, NULL},
        {  1,  1 }, { NULL, NULL },
        NULL, DrawCar, 0, NULL, NULL, NULL, 0, {  0,  0 }, NULL, 1, 1,
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas,
        0, NULL,
    };
    int i;
    int result;
    int power_up_levels[3];
    LOG_TRACE("(%d, %p, %p)", pNet_mode, pCar_index, pNet_game);

    gChoose_car_net_game = pNet_game;
    gChange_race_net_mode = pNet_mode;
    gStart_interface_spec = &interface_spec;
    for (i = 0; i < gProgram_state.number_of_cars; i++) {
        if (gProgram_state.cars_available[i] == *pCar_index) {
            gCurrent_car_index = i;
            break;
        }
    }
    if (gProgram_state.game_completed) {
        gProgram_state.number_of_cars = gNumber_of_racers;
        for (i = 0; i < gProgram_state.number_of_cars; i++) {
            gProgram_state.cars_available[i] = i;
        }
    }
    for (i = 0; i < gProgram_state.number_of_cars; i++) {
        if (gOpponents[gProgram_state.cars_available[i]].stolen_car_image_data == NULL) {
            LoadFlicData(gOpponents[gProgram_state.cars_available[i]].stolen_car_flic_name,
                &gOpponents[gProgram_state.cars_available[i]].stolen_car_image_data,
                &gOpponents[gProgram_state.cars_available[i]].stolen_car_image_data_length);
        } else {
            MAMSLock((void**)&gOpponents[gProgram_state.cars_available[i]].stolen_car_image_data);
        }
    }
    InitialiseFlicPanel(0,
        gCurrent_graf_data->change_car_panel_left,
        gCurrent_graf_data->change_car_panel_top,
        gCurrent_graf_data->change_car_panel_right - gCurrent_graf_data->change_car_panel_left,
        gCurrent_graf_data->change_car_panel_bottom - gCurrent_graf_data->change_car_panel_top);
    SetCarFlic();
    if (pNet_mode) {
        gTaken_image = LoadPixelmap("TAKEN.PIX");
    }
    result = DoInterfaceScreen(&interface_spec, pNet_mode, 0);
    if (pNet_mode) {
        FadePaletteDown();
    } else {
        RunFlic(237);
    }
    for (i = 0; i < gProgram_state.number_of_cars; i++) {
        MAMSUnlock((void**)&gOpponents[gProgram_state.cars_available[i]].stolen_car_image_data);
    }
    DisposeFlicPanel(0);
    if (pNet_mode) {
        BrPixelmapFree(gTaken_image);
    }
    if (result == 0) {
        if (pNet_mode) {
            *pCar_index = gProgram_state.cars_available[gCurrent_race_index];
        } else {
            AboutToLoadFirstCar();
            SwitchToRealResolution();
            for (i = 0; i < COUNT_OF(power_up_levels); i++) {
                power_up_levels[i] = gProgram_state.current_car.power_up_levels[i];
            }
            LoadCar(gOpponents[gProgram_state.cars_available[gCurrent_car_index]].car_file_name,
                eDriver_local_human,
                &gProgram_state.current_car,
                gProgram_state.cars_available[gCurrent_car_index],
                gProgram_state.player_name[gProgram_state.frank_or_anniness],
                &gOur_car_storage_space);
            for (i = 0; i < COUNT_OF(power_up_levels); i++) {
                gProgram_state.current_car.power_up_levels[i] = power_up_levels[i];
            }
            SwitchToLoresMode();
            SetCarStorageTexturingLevel(&gOur_car_storage_space, GetCarTexturingLevel(), eCTL_full);
            DisposeRaceInfo(&gCurrent_race);
            SelectOpponents(&gCurrent_race);
            LoadRaceInfo(gProgram_state.current_race_index, &gCurrent_race);
        }
        return 1;
    } else {
        return 0;
    }
}

// IDA: void __cdecl DoChangeCar()
void DoChangeCar() {
    LOG_TRACE("()");

    ChangeCar(0, &gProgram_state.current_car.index, NULL);
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
    ChangePanelFlic(0, gOpponents[gCurrent_race.opponent_list[gOpponent_index].index].mug_shot_image_data, gOpponents[gCurrent_race.opponent_list[gOpponent_index].index].mug_shot_image_data_length);
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
    gProgram_state.view_type++;
    if (gProgram_state.view_type > eVT_Opponents) {
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
        // cheat code: "KEVWOZEAR"
        gProgram_state.game_completed = 1;
        DRS3StartSound(gIndexed_outlets[0], 3202);
        DRS3StartSound(gIndexed_outlets[0], 3202);
    }
    if (*test == 0x33F75455 && test[1] == 0xC10AAAF2) {
        // cheat code: "IWANTTOFIDDLE"

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
        gDisallow_abandon_race = 1;
        return eSO_main_menu_invoked;
    }
}

// IDA: void __usercall DrawGridCar(int pX@<EAX>, int pY@<EDX>, br_pixelmap *pImage@<EBX>)
void DrawGridCar(int pX, int pY, br_pixelmap* pImage) {
    LOG_TRACE("(%d, %d, %p)", pX, pY, pImage);

    if (gCurrent_graf_data->grid_left_clip <= pX && pX + pImage->width < gCurrent_graf_data->grid_right_clip) {
        DRPixelmapRectangleMaskedCopy(gBack_screen, pX, pY, pImage, 0, 0, pImage->width, pImage->height);
    }
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
    int swap_1_x = 0;
    int swap_1_y = 0;
    int swap_2_x = 0;
    int swap_2_y = 0;
    br_pixelmap* the_image = NULL;
    br_pixelmap* swap_1_image = NULL;
    br_pixelmap* swap_2_image = NULL;
    char numbers_str[4][100];
    char total_str[256];
    tU32 the_time;
    LOG_TRACE("(%d, %d)", pOffset, pDraw_it);

    done_highest = 0;
    str_index = 0;

    memset(numbers_str, 0, 4 * 100 * sizeof(char));
    memset(total_str, 0, sizeof(total_str));

    the_time = PDGetTotalTime();
    BrPixelmapRectangleFill(
        gBack_screen,
        gCurrent_graf_data->grid_left_clip,
        gCurrent_graf_data->grid_top_clip,
        gCurrent_graf_data->grid_right_clip - gCurrent_graf_data->grid_left_clip,
        gCurrent_graf_data->grid_bottom_clip - gCurrent_graf_data->grid_top_clip,
        0);
    for (i = 0; i < gCurrent_race.number_of_racers; i++) {
        if (gCurrent_race.opponent_list[i].index == -1) {
            the_image = gProgram_state.current_car.grid_icon_image;
        } else {
            the_image = gCurrent_race.opponent_list[i].car_spec->grid_icon_image;
        }
        y = gCurrent_graf_data->grid_top_y
            + i % 2 * gCurrent_graf_data->grid_y_pitch
            - the_image->height / 2
            - gGrid_y_adjust;
        x = gCurrent_graf_data->grid_left_x
            + i % 2 * gCurrent_graf_data->grid_x_stagger
            + i / 2 * gCurrent_graf_data->grid_x_pitch
            - pOffset;
        if (i == gSwap_grid_1) {
            swap_1_x = x;
            swap_1_y = y;
            swap_1_image = the_image;
        } else if (i == gSwap_grid_2) {
            swap_2_x = x;
            swap_2_y = y;
            swap_2_image = the_image;
        } else if (gCurrent_race.opponent_list[i].index != -1 || the_time % 900 / 300) {
            DrawGridCar(x, y, the_image);
        }
        if (!done_highest && gCurrent_race.opponent_list[i].ranking >= gProgram_state.rank) {
            done_highest = 1;
            if (x - gCurrent_graf_data->grid_marker_margin >= gCurrent_graf_data->grid_left_clip
                && x + the_image->width < gCurrent_graf_data->grid_right_clip) {
                BrPixelmapLine(
                    gBack_screen,
                    x - gCurrent_graf_data->grid_marker_margin,
                    y - gCurrent_graf_data->grid_marker_margin,
                    x - gCurrent_graf_data->grid_marker_margin,
                    y + gCurrent_graf_data->grid_marker_margin + the_image->height,
                    45);
                BrPixelmapLine(
                    gBack_screen,
                    x - gCurrent_graf_data->grid_marker_margin,
                    y - gCurrent_graf_data->grid_marker_margin,
                    x + gCurrent_graf_data->grid_marker_x_len,
                    y - gCurrent_graf_data->grid_marker_margin,
                    45);
                BrPixelmapLine(
                    gBack_screen,
                    x - gCurrent_graf_data->grid_marker_margin,
                    y + gCurrent_graf_data->grid_marker_margin + the_image->height,
                    x + gCurrent_graf_data->grid_marker_x_len,
                    y + gCurrent_graf_data->grid_marker_margin + the_image->height,
                    45);
            }
        }
        if (gCurrent_race.opponent_list[i].index >= 0) {
            if (gCurrent_race.opponent_list[i].ranking >= gProgram_state.rank) {
                if (str_index >= 2) {
                    str_index = 3;
                } else {
                    str_index = 1;
                }
            }
        } else {
            str_index = 2;
        }
        if (gCurrent_race.opponent_list[i].index >= 0) {
            if (gOpponents[gCurrent_race.opponent_list[i].index].car_number <= 0
                || gOpponents[gCurrent_race.opponent_list[i].index].car_number >= 100) {
                if (gOpponents[gCurrent_race.opponent_list[i].index].car_number < 0) {
                    sprintf(&numbers_str[str_index][strlen(numbers_str[str_index])], "%c ", ':');
                }
            } else {
                sprintf(
                    &numbers_str[str_index][strlen(numbers_str[str_index])],
                    "%d ",
                    gOpponents[gCurrent_race.opponent_list[i].index].car_number);
            }
        } else {
            if (gProgram_state.frank_or_anniness) {
                sprintf(&numbers_str[str_index][strlen(numbers_str[str_index])], "%c ", '<');
            } else {
                sprintf(&numbers_str[str_index][strlen(numbers_str[str_index])], "%c ", ';');
            }
        }
    }
    if (gSwap_grid_1 >= 0) {
        DrawGridCar(
            gGrid_transition_stage * (swap_2_x - swap_1_x) / 100 + swap_1_x,
            gGrid_transition_stage * (swap_2_y - swap_1_y) / 100 + swap_1_y,
            swap_1_image);
        DrawGridCar(
            gGrid_transition_stage * (swap_1_x - swap_2_x) / 100 + swap_2_x,
            gGrid_transition_stage * (swap_1_y - swap_2_y) / 100 + swap_2_y,
            swap_2_image);
    }
    if (gDraw_grid_status == eGrid_draw_all) {
        if (strlen(numbers_str[3])) {
            numbers_str[2][strlen(numbers_str[3]) + 99] = 0;
        } else {
            numbers_str[1][strlen(numbers_str[2]) + 99] = 0;
        }
        strcpy(total_str, (const char*)numbers_str);
        for (i = 1; i <= 3; i++) {
            strcat(total_str, numbers_str[i]);
        }
        str_x = (gCurrent_graf_data->grid_numbers_left + gCurrent_graf_data->grid_numbers_right) / 2
            - (BrPixelmapTextWidth(gBack_screen, gBig_font, total_str) / 2);
        BrPixelmapRectangleFill(
            gBack_screen,
            gCurrent_graf_data->grid_numbers_left,
            gCurrent_graf_data->grid_numbers_top,
            gCurrent_graf_data->grid_numbers_right - gCurrent_graf_data->grid_numbers_left,
            gBig_font->glyph_y,
            0);
        gGrid_number_x_coords[0] = str_x - 3 - gCurrent_graf_data->grid_numbers_left;
        for (i = 0; i < 4; ++i) {
            if (i != 2 || the_time % 900 / 300) {
                TransBrPixelmapText(
                    gBack_screen,
                    str_x,
                    gCurrent_graf_data->grid_numbers_top,
                    gGrid_number_colour[i],
                    gBig_font,
                    (signed char*)numbers_str[i]);
            }
            str_x += BrPixelmapTextWidth(gBack_screen, gBig_font, numbers_str[i]);
        }
        for (i = gCurrent_race.number_of_racers - 1; i > 0; i--) {
            for (j = strlen(total_str) - 2; j >= 0; j--) {
                if (total_str[j] == ' ') {
                    total_str[j + 1] = 0;
                    break;
                }
            }
            gGrid_number_x_coords[i] = gGrid_number_x_coords[0] + BrPixelmapTextWidth(gBack_screen, gBig_font, total_str);
        }
    }
    if (pDraw_it) {
        PDScreenBufferSwap(0);
    }
}

// IDA: void __usercall MoveGrid(int pFrom@<EAX>, int pTo@<EDX>, tS32 pTime_to_move@<EBX>)
void MoveGrid(int pFrom, int pTo, tS32 pTime_to_move) {
    tS32 start_time;
    tS32 the_time;
    int move_distance;
    int pitch;
    LOG_TRACE("(%d, %d, %d)", pFrom, pTo, pTime_to_move);

    pitch = gCurrent_graf_data->grid_x_pitch;
    start_time = PDGetTotalTime();
    while (1) {
        the_time = PDGetTotalTime();
        if (start_time + pTime_to_move <= the_time) {
            break;
        }
        DrawGrid(pitch * pFrom + pitch * (pTo - pFrom) * (the_time - start_time) / pTime_to_move, 1);
    }
    DrawGrid(pitch * pTo, 1);
}

// IDA: int __usercall CalcGridOffset@<EAX>(int pPosition@<EAX>)
int CalcGridOffset(int pPosition) {
    LOG_TRACE("(%d)", pPosition);

    return pPosition / 2 - 1;
}

// IDA: void __usercall GridDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void GridDraw(int pCurrent_choice, int pCurrent_mode) {
    LOG_TRACE8("(%d, %d)", pCurrent_choice, pCurrent_mode);

    if (gDraw_grid_status > eGrid_draw_none) {
        DrawGrid(gCurrent_graf_data->grid_x_pitch * CalcGridOffset(gOur_starting_position), 0);
    }
}

// IDA: void __usercall ActuallySwapOrder(int pFirst_index@<EAX>, int pSecond_index@<EDX>)
void ActuallySwapOrder(int pFirst_index, int pSecond_index) {
    tOpp_spec temp_opp;
    LOG_TRACE("(%d, %d)", pFirst_index, pSecond_index);

    temp_opp = gCurrent_race.opponent_list[pFirst_index];
    gCurrent_race.opponent_list[pFirst_index] = gCurrent_race.opponent_list[pSecond_index];
    gCurrent_race.opponent_list[pSecond_index] = temp_opp;
    gOur_starting_position = pSecond_index;

    //LOG_DEBUG("first %d, second %d", gCurrent_race.opponent_list[pFirst_index].index, gCurrent_race.opponent_list[pSecond_index].index);
}

// IDA: void __usercall DoGridTransition(int pFirst_index@<EAX>, int pSecond_index@<EDX>)
void DoGridTransition(int pFirst_index, int pSecond_index) {
    tU32 start_time;
    tU32 the_time;
    LOG_TRACE("(%d, %d)", pFirst_index, pSecond_index);

    if (pFirst_index != pSecond_index) {
        start_time = PDGetTotalTime();
        gSwap_grid_1 = pFirst_index;
        gSwap_grid_2 = pSecond_index;
        while (1) {
            the_time = PDGetTotalTime();
            if (start_time + 150 <= the_time) {
                break;
            }
            RemoveTransientBitmaps(1);
            gGrid_transition_stage = 100 * (the_time - start_time) / 150;
            DrawGrid(gCurrent_graf_data->grid_x_pitch * CalcGridOffset(gOur_starting_position), 0);
            ProcessFlicQueue(gFrame_period);
            DoMouseCursor();
            PDScreenBufferSwap(0);
        }
        gSwap_grid_1 = -1;
        gSwap_grid_2 = -1;
        ActuallySwapOrder(pFirst_index, pSecond_index);
        MoveGrid(CalcGridOffset(pFirst_index), CalcGridOffset(pSecond_index), 150);
    }
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

    InitialiseFlicPanel(
        0,
        gCurrent_graf_data->start_race_panel_left,
        gCurrent_graf_data->start_race_panel_top,
        gCurrent_graf_data->start_race_panel_right - gCurrent_graf_data->start_race_panel_left,
        gCurrent_graf_data->start_race_panel_bottom - gCurrent_graf_data->start_race_panel_top);
    ChangePanelFlic(
        0,
        gOpponents[gChallenger_index__racestrt].mug_shot_image_data,
        gOpponents[gChallenger_index__racestrt].mug_shot_image_data_length);
    if (gScreen->row_bytes < 0) {
        BrFatal("C:\\Msdev\\Projects\\DethRace\\Racestrt.c", 2610, "Bruce bug at line %d, file C:\\Msdev\\Projects\\DethRace\\Racestrt.c", 50);
    }
    the_map = DRPixelmapAllocate(
        gScreen->type,
        gCurrent_graf_data->dare_mugshot_width,
        gCurrent_graf_data->dare_mugshot_height,
        0,
        0);

    BrPixelmapRectangleCopy(the_map, 0, 0, GetPanelPixelmap(0), gCurrent_graf_data->dare_mug_left_margin, gCurrent_graf_data->dare_mug_top_margin, gCurrent_graf_data->dare_mugshot_width, gCurrent_graf_data->dare_mugshot_height);
    DisposeFlicPanel(0);
    TellyInImage(the_map, gCurrent_graf_data->dare_mugshot_left, gCurrent_graf_data->dare_mugshot_top);
    BrPixelmapFree(the_map);
    the_map = DRPixelmapAllocate(gScreen->type, gCurrent_graf_data->dare_text_width, gCurrent_graf_data->dare_mugshot_height, 0, 0);
    BrPixelmapFill(the_map, 0);
    TransBrPixelmapText(the_map, 0, 0, 1u, gBig_font, (signed char*)gOpponents[gChallenger_index__racestrt].abbrev_name);
    PathCat(the_path, gApplication_path, "DARES.TXT");
    f = DRfopen(the_path, "rt");
    if (!f) {
        FatalError(100);
    }

    dare_index = IRandomBetween(0, GetAnInt(f) - 1);
    for (i = 0; i < dare_index; i++) {
        line_count = GetAnInt(f);
        for (j = 0; j < line_count; j++) {
            GetALineAndDontArgue(f, s);
        }
    }
    line_count = GetAnInt(f);
    for (i = 0; i < line_count; i++) {
        GetALineAndDontArgue(f, s);
        TransBrPixelmapText(the_map, 0, 2 * (i + 1) * gBig_font->glyph_y, 0x86u, gBig_font, (signed char*)s);
    }
    fclose(f);
    BrPixelmapLine(the_map, 0, gBig_font->glyph_y + 2, the_map->width, gBig_font->glyph_y + 2, 45);
    TellyInImage(the_map, gCurrent_graf_data->dare_text_left, gCurrent_graf_data->dare_mugshot_top);
    BrPixelmapFree(the_map);
    UnlockOpponentMugshot(gChallenger_index__racestrt);
    gDare_start_time = PDGetTotalTime();
}

// IDA: int __usercall CheckNextStage@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int CheckNextStage(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gDare_start_time && (unsigned int)(PDGetTotalTime() - gDare_start_time) >= 7500) {
        BrPixelmapRectangleFill(
            gBack_screen,
            gCurrent_graf_data->grid_left_clip,
            gCurrent_graf_data->dare_mugshot_top,
            gCurrent_graf_data->grid_right_clip - gCurrent_graf_data->grid_left_clip,
            gCurrent_graf_data->dare_mugshot_height,
            0);
        DoGridTransition(gOur_starting_position, gChallenger_position);
        gDraw_grid_status = eGrid_draw_icons_only;
        gDare_start_time = 0;
    }
    return 0;
}

// IDA: int __usercall ChallengeDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int ChallengeDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);

    if (!pEscaped || gDare_start_time) {
        if (!pEscaped && gDare_start_time) {
            ActuallySwapOrder(gOur_starting_position, gChallenger_position);
            ChallengeOccurred(gChallenger_index__racestrt, 1);
        }
    } else {
        DoGridTransition(gOur_starting_position, gOriginal_position);
        ActuallySwapOrder(gOur_starting_position, gOriginal_position);
        ChallengeOccurred(gChallenger_index__racestrt, 0);
    }
    ChallengeOccurred(gChallenger_index__racestrt, pEscaped == 0);
    if (pTimed_out) {
        return 0;
    } else {
        return pCurrent_choice;
    }
}

// IDA: void __cdecl DoChallengeScreen()
void DoChallengeScreen() {
    static tFlicette flicker_on[2] = { { 43, { 54, 108 }, { 157, 377 } }, { 43, { 218, 436 }, { 157, 377 } } };
    static tFlicette flicker_off[2] = { { 42, { 54, 108 }, { 157, 377 } }, { 42, { 218, 436 }, { 157, 377 } } };
    static tFlicette push[2] = { { 304, { 54, 108 }, { 157, 377 } }, { 305, { 218, 436 }, { 157, 377 } } };
    static tMouse_area mouse_areas[2] = {
        { { 54, 108 }, { 157, 377 }, { 117, 234 }, { 178, 427 }, 0, 0, 0, NULL },
        { { 218, 436 }, { 157, 377 }, { 281, 562 }, { 178, 427 }, 1, 0, 0, NULL }
    };
    static tInterface_spec interface_spec = {
        0, // initial_imode
        301, // first_opening_flic
        0, // second_opening_flic
        -1, // end_flic_go_ahead
        -1, // end_flic_escaped
        -1, // end_flic_otherwise
        0, // flic_bunch_to_load
        { -1, 0 }, // move_left_new_mode
        { -1, 0 }, // move_left_delta
        { 0, 0 }, // move_left_min
        { 1, 0 }, // move_left_max
        { NULL, NULL }, // move_left_proc
        { -1, 0 }, // move_right_new_mode
        { 1, 0 }, // move_right_delta
        { 0, 0 }, // move_right_min
        { 1, 0 }, // move_right_max
        { NULL, NULL }, // move_right_proc
        { -1, 0 }, // move_up_new_mode
        { 0, 0 }, // move_up_delta
        { 0, 0 }, // move_up_min
        { 0, 0 }, // move_up_max
        { NULL, NULL }, // move_up_proc
        { -1, 0 }, // move_down_new_mode
        { 0, 0 }, // move_down_delta
        { 0, 0 }, // move_down_min
        { 0, 0 }, // move_down_max
        { NULL, NULL }, // move_down_proc
        { 1, 1 }, // go_ahead_allowed
        { NULL, NULL }, // go_ahead_proc
        { 1, 1 }, // escape_allowed
        { NULL, NULL }, // escape_proc
        &CheckNextStage, // exit_proc
        &GridDraw, // draw_proc
        0u, // time_out
        NULL, // start_proc1
        &ChallengeStart, // start_proc2
        &ChallengeDone, // done_proc
        0, // font_needed
        { 0, 0 }, // typeable
        NULL, // get_original_string
        1, // escape_code
        1, // dont_save_or_load
        2, // number_of_button_flics
        flicker_on, // flicker_on_flics
        flicker_off, // flicker_off_flics
        push, // pushed_flics
        2, // number_of_mouse_areas
        mouse_areas, // mouse_areas
        0, // number_of_recopy_areas
        NULL // recopy_areas
    };

    int result;
    LOG_TRACE("()");

    gOriginal_position = gOur_starting_position;
    gChallenger_position = IRandomBetween(0, 1);
    if (gOpponents[gCurrent_race.opponent_list[gChallenger_position].index].car_number < 0) {
        gChallenger_position ^= 1u;
    }
    gChallenger_index__racestrt = gCurrent_race.opponent_list[gChallenger_position].index;
    LoadOpponentMugShot(gChallenger_index__racestrt);
    gDraw_grid_status = eGrid_draw_none;
    gGrid_y_adjust = gCurrent_graf_data->dare_y_adjust;
    DoInterfaceScreen(&interface_spec, 0, 0);
}

// IDA: int __usercall GridDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int GridDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);

    if (pTimed_out) {
        return 0;
    }
    if (pEscaped) {
        return -1;
    }
    return pCurrent_choice;
}

// IDA: void __cdecl GridStart()
void GridStart() {
    LOG_TRACE("()");

    MoveGrid(-2, CalcGridOffset(gOur_starting_position), 400);
    PrintMemoryDump(0, "IN GRID SCREEN");
}

// IDA: int __usercall GridMoveLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GridMoveLeft(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gOur_starting_position
        && gCurrent_race.opponent_list[gOur_starting_position - 1].ranking >= gProgram_state.rank) {
        AddToFlicQueue(
            gStart_interface_spec->pushed_flics[1].flic_index,
            gStart_interface_spec->pushed_flics[1].x[gGraf_data_index],
            gStart_interface_spec->pushed_flics[1].y[gGraf_data_index],
            1);
        DRS3StartSound(gIndexed_outlets[0], 3000);
        DoGridTransition(gOur_starting_position, gOur_starting_position - 1);
    }
    return 0;
}

// IDA: int __usercall GridMoveRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int GridMoveRight(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gOur_starting_position < gCurrent_race.number_of_racers - 1) {
        AddToFlicQueue(
            gStart_interface_spec->pushed_flics[2].flic_index,
            gStart_interface_spec->pushed_flics[2].x[gGraf_data_index],
            gStart_interface_spec->pushed_flics[2].y[gGraf_data_index],
            1);
        DRS3StartSound(gIndexed_outlets[0], 3000);
        DoGridTransition(gOur_starting_position, gOur_starting_position + 1);
    }
    return 0;
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
    LOG_TRACE8("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (!gChallenge_time || PDGetTotalTime() < gChallenge_time) {
        return 0;
    }
    *pCurrent_choice = -2;
    return 1;
}

// IDA: int __usercall FindBestPos@<EAX>(int pOur_rank@<EAX>)
int FindBestPos(int pOur_rank) {
    int i;
    LOG_TRACE("(%d)", pOur_rank);

    for (i = gCurrent_race.number_of_racers - 1; i >= 0; i--) {
        if (gCurrent_race.opponent_list[i].ranking < pOur_rank) {
            return i + 1;
        }
    }
    return 0;
}

// IDA: int __usercall SortGridFunction@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
int SortGridFunction(const void* pFirst_one, const void* pSecond_one) {
    LOG_TRACE("(%p, %p)", pFirst_one, pSecond_one);

    return ((tOpp_spec*)pFirst_one)->ranking - ((tOpp_spec*)pSecond_one)->ranking;
}

// IDA: void __cdecl SortOpponents()
void SortOpponents() {
    int i;
    LOG_TRACE("()");

    for (i = 0; gCurrent_race.number_of_racers > i; ++i) {
        if (gCurrent_race.opponent_list[i].index < 0) {
            return;
        }
    }
    qsort(gCurrent_race.opponent_list, gCurrent_race.number_of_racers, sizeof(tOpp_spec), SortGridFunction);
    gBest_pos_available = FindBestPos(gProgram_state.rank);
    gOur_starting_position = gCurrent_race.number_of_racers - 70 * (gCurrent_race.number_of_racers - gBest_pos_available) / 100;
    for (i = gCurrent_race.number_of_racers; i > gOur_starting_position; i--) {
        gCurrent_race.opponent_list[i] = gCurrent_race.opponent_list[i - 1];
    }
    gCurrent_race.opponent_list[gOur_starting_position].index = -1;
    gCurrent_race.opponent_list[gOur_starting_position].ranking = gProgram_state.rank;
    gCurrent_race.opponent_list[gOur_starting_position].car_spec = &gProgram_state.current_car;
    gCurrent_race.number_of_racers++;
}

// IDA: tSO_result __cdecl DoGridPosition()
tSO_result DoGridPosition() {
    static tFlicette flicker_on[3] = {
        { 43, { 240, 480 }, { 158, 379 } },
        { 293, { 56, 112 }, { 151, 362 } },
        { 296, { 136, 272 }, { 151, 362 } }
    };

    static tFlicette flicker_off[3] = {
        { 42, { 240, 480 }, { 158, 379 } },
        { 292, { 56, 112 }, { 151, 362 } },
        { 295, { 136, 272 }, { 151, 362 } }
    };

    static tFlicette push[3] = {
        { 154, { 240, 480 }, { 158, 379 } },
        { 294, { 56, 112 }, { 151, 362 } },
        { 297, { 136, 272 }, { 151, 362 } }
    };
    static tMouse_area mouse_areas[5] = {
        { { 240, 480 }, { 158, 379 }, { 305, 610 }, { 178, 427 }, 0, 0, 0, NULL },
        { { 56, 112 },
            { 151, 362 },
            { 91, 182 },
            { 158, 379 },
            -1,
            0,
            0,
            GridClickLeft },
        { { 136, 272 },
            { 151, 362 },
            { 171, 342 },
            { 158, 379 },
            -1,
            0,
            0,
            GridClickRight },
        { { 52, 104 },
            { 56, 134 },
            { 269, 538 },
            { 141, 338 },
            -1,
            0,
            0,
            GridClickCar },
        { { 60, 120 },
            { 163, 391 },
            { 235, 470 },
            { 176, 422 },
            -1,
            0,
            0,
            GridClickNumbers }
    };

    static tInterface_spec interface_spec = {
        0, // initial_imode
        290, // first_opening_flic
        0, // second_opening_flic
        0, // end_flic_go_ahead
        0, // end_flic_escaped
        0, // end_flic_otherwise
        8, // flic_bunch_to_load
        { -1, 0 }, // move_left_new_mode
        { 0, 0 }, // move_left_delta
        { 0, 0 }, // move_left_min
        { 0, 0 }, // move_left_max
        { GridMoveLeft, NULL }, // move_left_proc
        { -1, 0 }, // move_right_new_mode
        { 0, 0 }, // move_right_delta
        { 0, 0 }, // move_right_min
        { 0, 0 }, // move_right_max
        { GridMoveRight, NULL }, // move_right_proc
        { -1, 0 }, // move_up_new_mode
        { 0, 0 }, // move_up_delta
        { 0, 0 }, // move_up_min
        { 0, 0 }, // move_up_max
        { GridMoveLeft, NULL }, // move_up_proc
        { -1, 0 }, // move_down_new_mode
        { 0, 0 }, // move_down_delta
        { 0, 0 }, // move_down_min
        { 0, 0 }, // move_down_max
        { GridMoveRight, NULL }, // move_down_proc
        { 1, 1 }, // go_ahead_allowed
        { NULL, NULL }, // go_ahead_proc
        { 1, 1 }, // escape_allowed
        { NULL, NULL }, // escape_proc
        CheckChallenge, // exit_proc
        GridDraw, // draw_proc
        0u, // time_out
        NULL, // start_proc1
        GridStart, // start_proc2
        GridDone, // done_proc
        0, // font_needed
        { 0, 0 }, // typeable
        NULL, // get_original_string
        -1, // escape_code
        1, // dont_save_or_load
        3, // number_of_button_flics
        flicker_on, // flicker_on_flics
        flicker_off, // flicker_off_flics
        push, // pushed_flics
        5, // number_of_mouse_areas
        mouse_areas, // mouse_areas
        0, // number_of_recopy_areas
        NULL // recopy_areas
    };

    int result;
    LOG_TRACE("()");

    gStart_interface_spec = &interface_spec;
    if (!gAusterity_mode) {
        LoadGridIcons(&gCurrent_race);
    }
    gSwap_grid_1 = -1;
    gSwap_grid_2 = -1;
    gDraw_grid_status = eGrid_draw_all;
    gGrid_y_adjust = 0;
    SortOpponents();
    if (!gAusterity_mode) {
        if (gBest_pos_available > 1
            && (gOpponents[gCurrent_race.opponent_list[0].index].car_number >= 0
                || gOpponents[gCurrent_race.opponent_list[1].index].car_number >= 0)
            && PercentageChance(10)) {
            gChallenge_time = PDGetTotalTime() + IRandomBetween(1000, 10000);
        } else {
            gChallenge_time = 0;
        }
        result = DoInterfaceScreen(&interface_spec, 0, 0);
        if (result >= 0) {
            FadePaletteDown();
        } else {
            RunFlic(291);
        }
        if (result == -1) {
            return eSO_main_menu_invoked;
        }
        if (result == -2) {
            DoChallengeScreen();
        }
        DisposeGridIcons(&gCurrent_race);
    }
    return eSO_continue;
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

//IDA: void __usercall DrawAnItem(int pX@<EAX>, int pY_index@<EDX>, int pFont_index@<EBX>, char *pText@<ECX>)
// Suffix added to avoid duplicate symbol
void DrawAnItem__racestrt(int pX, int pY_index, int pFont_index, char* pText) {
    LOG_TRACE("(%d, %d, %d, \"%s\")", pX, pY_index, pFont_index, pText);
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
