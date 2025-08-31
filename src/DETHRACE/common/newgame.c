#include "newgame.h"
#include "cutscene.h"
#include "displays.h"
#include "drmem.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "init.h"
#include "input.h"
#include "intrface.h"
#include "loading.h"
#include "network.h"
#include "pd/sys.h"
#include "racestrt.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>
#include <string.h>

// GLOBAL: CARM95 0x0051e910
tU8* gFrank_flic_data;

// GLOBAL: CARM95 0x0051e914
tU8* gAnne_flic_data;

// GLOBAL: CARM95 0x0051e918
int gNet_storage_allocated;
// clang-format off

// GLOBAL: CARM95 0x0051e920
tRadio_bastards gRadio_bastards__newgame[11] = { // suffix added to avoid duplicate symbol
    { 1,  32, 0, { 245,   0,   0,   0,   0, }, },
    { 1,  43, 0, { 245,   0,   0,   0,   0, }, },
    { 1,  54, 0, { 245,   0,   0,   0,   0, }, },
    { 1,  65, 0, { 245,   0,   0,   0,   0, }, },
    { 1,  76, 0, { 245,   0,   0,   0,   0, }, },
    { 2,  86, 0, {  81, 150,   0,   0,   0, }, },
    { 2,  97, 0, {  81, 150,   0,   0,   0, }, },
    { 2, 108, 0, {  81, 150,   0,   0,   0, }, },
    { 2, 119, 0, { 150, 201,   0,   0,   0, }, },
    { 4, 130, 0, { 115, 150, 183, 215,   0, }, },
    { 5, 141, 0, { 142, 160, 182, 204, 230, }, },
};
// clang-format on
char _name1[] = "xxxxxxxx.TXT"; // keep compiler happy
char _name2[] = "yyyyyyyy.TXT"; // keep compiler happy

// GLOBAL: CARM95 0x0051ea80
char* gBasic_car_names[2] = { _name1, _name2 };

// GLOBAL: CARM95 0x005500c0
tNet_game_options gNet_settings[8];

// GLOBAL: CARM95 0x0053e5c0
tJoinable_game gGames_to_join[6];

// GLOBAL: CARM95 0x0053e5b8
tNet_game_options* gOptions;

// GLOBAL: CARM95 0x005500a0
int gNet_target[7];

// GLOBAL: CARM95 0x0053e5f0
int gLast_graph_sel__newgame; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x0053e59c
tInterface_spec* gThe_interface_spec__newgame; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x0053e5a8
tNet_sequence_type gNet_race_sequence__newgame; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00550098
tNet_game_type gLast_game_type;

// GLOBAL: CARM95 0x0053e5ac
int gCurrent_net_game_count;

// GLOBAL: CARM95 0x0053e5f4
tU32 gAnne_flic_data_length;

// GLOBAL: CARM95 0x0053e5a0
int gShifted_default_yet;

char* gNet_name;

// GLOBAL: CARM95 0x0053e598
tU32 gFrank_flic_data_length;

// GLOBAL: CARM95 0x0053e5a4
int gLast_net_choose_box;

// GLOBAL: CARM95 0x0053e594
int gCurrent_game_selection;

// GLOBAL: CARM95 0x0053e5b4
int gRace_index;

// GLOBAL: CARM95 0x0053e590
int gRadio_selected;

// IDA: void __cdecl StartRollingPlayerNamesIn()
// FUNCTION: CARM95 0x004affc4
void StartRollingPlayerNamesIn(void) {
    int i;

    for (i = 0; i < COUNT_OF(gCurrent_graf_data->player_name_x); i++) {
        SetSlotXY(i, gCurrent_graf_data->player_name_x[i], gCurrent_graf_data->player_name_y);
        AddRollingString(gProgram_state.player_name[i], gCurrent_graf_data->player_name_x[i], gCurrent_graf_data->player_name_y, eRT_alpha);
    }
}

// IDA: void __cdecl FrankAnneStart1()
// FUNCTION: CARM95 0x004afe80
void FrankAnneStart1(void) {

    StartRollingPlayerNamesIn();
    if (gFrank_flic_data == NULL) {
        if (!LoadFlicData("FRANK.FLI", &gFrank_flic_data, &gFrank_flic_data_length)) {
            FatalError(kFatalError_LoadOpponentMugShotFile);
        }
    } else {
        MAMSLock((void**)&gFrank_flic_data);
    }
    if (!gAnne_flic_data) {
        if (!LoadFlicData("ANNIE.FLI", &gAnne_flic_data, &gAnne_flic_data_length)) {
            FatalError(kFatalError_LoadOpponentMugShotFile);
        }
    } else {
        MAMSLock((void**)&gAnne_flic_data);
    }
    InitialiseFlicPanel(0,
        gCurrent_graf_data->frank_panel_left,
        gCurrent_graf_data->frank_panel_top,
        gCurrent_graf_data->frank_panel_right - gCurrent_graf_data->frank_panel_left,
        gCurrent_graf_data->frank_panel_bottom - gCurrent_graf_data->frank_panel_top);
    InitialiseFlicPanel(1,
        gCurrent_graf_data->anne_panel_left,
        gCurrent_graf_data->anne_panel_top,
        gCurrent_graf_data->anne_panel_right - gCurrent_graf_data->anne_panel_left,
        gCurrent_graf_data->anne_panel_bottom - gCurrent_graf_data->anne_panel_top);
}

// IDA: void __cdecl FrankAnneStart2()
// FUNCTION: CARM95 0x004b0057
void FrankAnneStart2(void) {

    ChangePanelFlic(0, gFrank_flic_data, gFrank_flic_data_length);
    ChangePanelFlic(1, gAnne_flic_data, gAnne_flic_data_length);
    TellyInImage(GetPanelPixelmap(0), gCurrent_graf_data->frank_panel_left, gCurrent_graf_data->frank_panel_top);
    TellyInImage(GetPanelPixelmap(1), gCurrent_graf_data->anne_panel_left, gCurrent_graf_data->anne_panel_top);
}

// IDA: void __usercall GetPlayerName(int pStarting_to_type@<EAX>, int pCurrent_choice@<EDX>, char *pString@<EBX>, int *pMax_length@<ECX>)
// FUNCTION: CARM95 0x004b00e4
void GetPlayerName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {

    strcpy(pString, gProgram_state.player_name[pCurrent_choice]);
    *pMax_length = PLAYER_NAME_MAX_LENGTH;
}

// IDA: int __usercall FrankAnneDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
// FUNCTION: CARM95 0x004b012b
int FrankAnneDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {

    RemoveTransientBitmaps(1);
    TellyOutImage(GetPanelPixelmap(1), gCurrent_graf_data->anne_panel_left, gCurrent_graf_data->anne_panel_top);
    TellyOutImage(GetPanelPixelmap(0), gCurrent_graf_data->frank_panel_left, gCurrent_graf_data->frank_panel_top);
    if (gFrank_flic_data) {
        MAMSUnlock((void**)&gFrank_flic_data);
    }
    if (gAnne_flic_data) {
        MAMSUnlock((void**)&gAnne_flic_data);
    }
    gProgram_state.frank_or_anniness = pCurrent_choice;
    GetTypedName(gProgram_state.player_name[pCurrent_choice], PLAYER_NAME_MAX_LENGTH);
    return pCurrent_choice;
}

// IDA: void __usercall FrankAnneDraw(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b01fb
void FrankAnneDraw(int pCurrent_choice, int pCurrent_mode) {

    if (gTyping) {
        if (GetTotalTime() & 0x100) {
            if (pCurrent_choice < 2) {
                TransDRPixelmapText(
                    gBack_screen,
                    gCurrent_graf_data->enter_name_x[pCurrent_choice],
                    gCurrent_graf_data->enter_name_y,
                    &gFonts[FONT_GRNLIT],
                    GetMiscString(kMiscString_ENTER_NAME),
                    gBack_screen->width);
            }
        }
    }
}

// IDA: int __cdecl FrankieOrAnnie()
// FUNCTION: CARM95 0x004b03cd
int FrankieOrAnnie(void) {
    static tFlicette flicker_on[3] = {
        { 83, { 61, 122 }, { 52, 125 } },
        { 83, { 184, 398 }, { 52, 125 } },
        { 43, { 215, 430 }, { 158, 379 } }
    };
    static tFlicette flicker_off[3] = {
        { 82, { 61, 122 }, { 52, 125 } },
        { 82, { 184, 398 }, { 52, 125 } },
        { 42, { 215, 430 }, { 158, 379 } }
    };
    static tFlicette push[3] = {
        { 83, { 61, 122 }, { 52, 125 } },
        { 83, { 184, 398 }, { 52, 125 } },
        { 45, { 215, 430 }, { 158, 379 } }
    };
    static tMouse_area mouse_areas[3] = {
        { { 55, 110 }, { 52, 125 }, { 161, 322 }, { 154, 370 }, 0, 0, 0, NULL },
        { { 178, 356 }, { 52, 125 }, { 295, 596 }, { 154, 370 }, 1, 0, 0, NULL },
        { { 215, 430 }, { 158, 379 }, { 278, 556 }, { 179, 430 }, 2, 1, 1, NULL }
    };
    static tRectile recopy_areas[2] = {
        { { 55, 110 }, { 132, 317 }, { 161, 322 }, { 154, 370 } },
        { { 178, 356 }, { 132, 317 }, { 295, 590 }, { 154, 370 } }
    };

    // GLOBAL: CARM95 0x0051EC18
    static tInterface_spec interface_spec = {
        0,               // initial_imode
        80,              // first_opening_flic
        0,               // second_opening_flic
        81,              // end_flic_go_ahead
        81,              // end_flic_escaped
        81,              // end_flic_otherwise
        0,               // flic_bunch_to_load
        { -1, -1 },      // move_left_new_mode
        { -1, 0 },       // move_left_delta
        { 0, 2 },        // move_left_min
        { 1, 2 },        // move_left_max
        { NULL, NULL },  // move_left_proc
        { -1, -1 },      // move_right_new_mode
        { 1, 0 },        // move_right_delta
        { 0, 2 },        // move_right_min
        { 1, 2 },        // move_right_max
        { NULL, NULL },  // move_right_proc
        { 1, 0 },        // move_up_new_mode
        { -2, -1 },      // move_up_delta
        { 2, 1 },        // move_up_min
        { 2, 1 },        // move_up_max
        { NULL, NULL },  // move_up_proc
        { 1, 0 },        // move_down_new_mode
        { 2, -1 },       // move_down_delta
        { 2, 1 },        // move_down_min
        { 2, 1 },        // move_down_max
        { NULL, NULL },  // move_down_proc
        { 1, 1 },        // go_ahead_allowed
        { NULL, NULL },  // go_ahead_proc
        { 1, 1 },        // escape_allowed
        { NULL, NULL },  // escape_proc
        NULL,            // exit_proc
        FrankAnneDraw,   // draw_proc
        0,               // time_out
        FrankAnneStart1, // start_proc1
        FrankAnneStart2, // start_proc2
        FrankAnneDone,   // done_proc
        1,               // font_needed
        { 1, 0 },        // typeable
        GetPlayerName,   // get_original_string
        2,               // escape_code
        1,               // dont_save_or_load
        3,               // number_of_button_flics
        flicker_on,      // flicker_on_flics
        flicker_off,     // flicker_off_flics
        push,            // pushed_flics
        3,               // number_of_mouse_areas
        mouse_areas,     // mouse_areas
        2,               // number_of_recopy_areas
        recopy_areas     // recopy_areas
    };

    int result;

    LoadFont(FONT_GRNLIT);
    result = DoInterfaceScreen(&interface_spec, 0, gProgram_state.frank_or_anniness);
    DisposeFlicPanel(1);
    DisposeFlicPanel(0);
    DisposeFont(FONT_GRNLIT);
    return result < 2;
}

// IDA: int __cdecl SelectSkillLevel()
// FUNCTION: CARM95 0x004b0436
int SelectSkillLevel(void) {
    static tFlicette flicker_on[4] = {
        { 116, { 38, 76 }, { 55, 132 } },
        { 119, { 36, 72 }, { 83, 199 } },
        { 121, { 38, 76 }, { 111, 266 } },
        { 43, { 227, 454 }, { 158, 379 } }
    };

    static tFlicette flicker_off[4] = {
        { 115, { 38, 76 }, { 55, 132 } },
        { 118, { 36, 72 }, { 83, 199 } },
        { 120, { 38, 76 }, { 111, 266 } },
        { 42, { 227, 454 }, { 158, 379 } }
    };

    static tFlicette push[4] = {
        { 117, { 38, 76 }, { 55, 132 } },
        { 117, { 36, 72 }, { 83, 199 } },
        { 117, { 38, 76 }, { 111, 266 } },
        { 45, { 227, 454 }, { 158, 379 } }
    };
    static tMouse_area mouse_areas[4] = {
        { { 38, 76 }, { 55, 132 }, { 205, 410 }, { 69, 166 }, 0, 0, 0, NULL },
        { { 36, 72 }, { 83, 199 }, { 205, 410 }, { 98, 235 }, 1, 0, 0, NULL },
        { { 38, 76 }, { 111, 266 }, { 205, 410 }, { 125, 300 }, 2, 0, 0, NULL },
        { { 227, 454 }, { 158, 379 }, { 290, 580 }, { 178, 427 }, 3, 0, 0, NULL }
    };

    static tInterface_spec interface_spec = {
        0,              // initial_imode
        110,            // first_opening_flic
        0,              // second_opening_flic
        -1,             // end_flic_go_ahead
        111,            // end_flic_escaped
        -1,             // end_flic_otherwise
        0,              // flic_bunch_to_load
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
        { -1, 0 },      // move_up_delta
        { 0, 0 },       // move_up_min
        { 3, 0 },       // move_up_max
        { NULL, NULL }, // move_up_proc
        { -1, 0 },      // move_down_new_mode
        { 1, 0 },       // move_down_delta
        { 0, 0 },       // move_down_min
        { 3, 0 },       // move_down_max
        { NULL, NULL }, // move_down_proc
        { 1, 1 },       // go_ahead_allowed
        { NULL, NULL }, // go_ahead_proc
        { 1, 1 },       // escape_allowed
        { NULL, NULL }, // escape_proc
        NULL,           // exit_proc
        NULL,           // draw_proc
        0u,             // time_out
        NULL,           // start_proc1
        NULL,           // start_proc2
        NULL,           // done_proc
        0,              // font_needed
        { 0, 0 },       // typeable
        NULL,           // get_original_string
        3,              // escape_code
        1,              // dont_save_or_load
        4,              // number_of_button_flics
        flicker_on,     // flicker_on_flics
        flicker_off,    // flicker_off_flics
        push,           // pushed_flics
        4,              // number_of_mouse_areas
        mouse_areas,    // mouse_areas
        0,              // number_of_recopy_areas
        NULL            // recopy_areas
    };

    int result;

    result = DoInterfaceScreen(&interface_spec, 0, gProgram_state.skill_level);
    if (result > 2) {
        return 0;
    }
    gProgram_state.skill_level = result;
    return 1;
}

// IDA: int __cdecl DoOnePlayerStart()
// FUNCTION: CARM95 0x004b027b
int DoOnePlayerStart(void) {
    int merrily_looping;
    tProgram_state saved_state;

    if (OriginalCarmaCDinDrive()) {
        memcpy(&saved_state, &gProgram_state, sizeof(tProgram_state));
        do {
            merrily_looping = FrankieOrAnnie();
            if (!merrily_looping) {
                memcpy(&gProgram_state, &saved_state, sizeof(tProgram_state));
                return 0;
            }
            if ((harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) && gProgram_state.frank_or_anniness != eFrankie) {
                DoFeatureUnavailableInDemo();
                memset(&gProgram_state, 0, sizeof(gProgram_state));
                return 0;
            }

            if (SelectSkillLevel()) {
                DoGoToRaceAnimation();
                StartLoadingScreen();
                AboutToLoadFirstCar();
                PrintMemoryDump(0, "JUST BEFORE LOADING YOUR CAR");
                SwitchToRealResolution();
                LoadCar(
                    gBasic_car_names[gProgram_state.frank_or_anniness],
                    eDriver_local_human,
                    &gProgram_state.current_car,
                    gProgram_state.frank_or_anniness,
                    gProgram_state.player_name[gProgram_state.frank_or_anniness],
                    &gOur_car_storage_space);
                SwitchToLoresMode();
                SetCarStorageTexturingLevel(&gOur_car_storage_space, GetCarTexturingLevel(), eCTL_full);
                PrintMemoryDump(0, "IMMEDIATELY AFTER LOADING YOUR CAR");
                gNet_mode = eNet_mode_none;
                InitGame(0);
                merrily_looping = 0;
            }
        } while (merrily_looping);
        UnlockBunchOfFlics(4);
        return 1;
    } else {
        DoErrorInterface(kMiscString_PLEASE_INSERT_THE_CARMAGEDDON_CD);
        return 0;
    }
}

// IDA: int __usercall NewNetGameUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b0484
int NewNetGameUp(int* pCurrent_choice, int* pCurrent_mode) {
    int new_sel;
    int i;

    DRS3StartSound(gEffects_outlet, 3000);
    if (*pCurrent_mode == 0) {
        gLast_graph_sel__newgame = COUNT_OF(gGames_to_join);
    }
    new_sel = -1;
    for (i = gLast_graph_sel__newgame - 1; i >= 0; i--) {
        gLast_graph_sel__newgame = i;
        if (gGames_to_join[i].game == NULL) {
            continue;
        }
        if (!gGames_to_join[i].game->options.open_game && !gGames_to_join[i].game->no_races_yet) {
            continue;
        }
        if (gGames_to_join[i].game->num_players > 5) {
            continue;
        }
        new_sel = i;
        break;
    }
    if (new_sel < 0) {
        gLast_graph_sel__newgame = -1;
        *pCurrent_choice = 0;
        *pCurrent_mode = 0;
    } else {
        gLast_graph_sel__newgame = new_sel;
        *pCurrent_choice = 2;
        *pCurrent_mode = 1;
    }
    return 1;
}

// IDA: int __usercall NewNetGameDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b0589
int NewNetGameDown(int* pCurrent_choice, int* pCurrent_mode) {
    int new_sel;
    int i;

    DRS3StartSound(gEffects_outlet, 3000);
    if (*pCurrent_mode == 0) {
        gLast_graph_sel__newgame = -1;
    }

    new_sel = -1;
    for (i = gLast_graph_sel__newgame - 1; i < COUNT_OF(gGames_to_join); i++) {
        gLast_graph_sel__newgame = i;
        if (gGames_to_join[i].game == NULL) {
            continue;
        }
        if (!gGames_to_join[i].game->options.open_game && !gGames_to_join[i].game->no_races_yet) {
            continue;
        }
        if (gGames_to_join[i].game->num_players > 5) {
            continue;
        }
        new_sel = i;
        break;
    }
    if (new_sel < 0) {
        gLast_graph_sel__newgame = -1;
        *pCurrent_choice = 0;
        *pCurrent_mode = 0;
    } else {
        gLast_graph_sel__newgame = new_sel;
        *pCurrent_choice = 2;
        *pCurrent_mode = 1;
    }
    return 1;
}

// IDA: void __usercall DisposeJoinableGame(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004b0def
void DisposeJoinableGame(int pIndex) {

    NetDisposeGameDetails(gGames_to_join[pIndex].game);
    gGames_to_join[pIndex].game = NULL;
}

// IDA: void __usercall DrawAnItem(int pX@<EAX>, int pY_index@<EDX>, int pFont_index@<EBX>, char *pText@<ECX>)
// FUNCTION: CARM95 0x004b0e1b
void DrawAnItem__newgame(int pX, int pY_index, int pFont_index, char* pText) {

    TransDRPixelmapText(gBack_screen,
        pX,
        gCurrent_graf_data->joinable_games_y + gCurrent_graf_data->joinable_games_y_pitch * pY_index,
        &gFonts[pFont_index],
        pText,
        pX + DRTextWidth(&gFonts[pFont_index], pText));
}

// IDA: void __usercall DrawColumnHeading(int pStr_index@<EAX>, int pX@<EDX>)
// FUNCTION: CARM95 0x004b0e9c
void DrawColumnHeading__newgame(int pStr_index, int pX) {

    TransDRPixelmapText(gBack_screen,
        pX,
        gCurrent_graf_data->joinable_games_y - gCurrent_graf_data->joinable_games_y_pitch,
        &gFonts[kFont_GRYLIT],
        GetMiscString(pStr_index),
        pX + DRTextWidth(&gFonts[kFont_GRYLIT], GetMiscString(pStr_index)));
}

// IDA: void __usercall DrawGames(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b068e
void DrawGames(int pCurrent_choice, int pCurrent_mode) {
    int i;
    int font_index;
    int current_index;
    int x_coord;
    int y_coord;
    char s[256];
    char* s2;
    char* s3;

    current_index = 0;
    BrPixelmapRectangleFill(gBack_screen,
        gCurrent_graf_data->joinable_games_sel_left,
        gCurrent_graf_data->joinable_games_sel_top_marg + gCurrent_graf_data->joinable_games_y,
        gCurrent_graf_data->joinable_games_sel_right - gCurrent_graf_data->joinable_games_sel_left,
        (COUNT_OF(gGames_to_join) + 1) * gCurrent_graf_data->joinable_games_y_pitch + gCurrent_graf_data->joinable_games_sel_bot_marg,
        0);
    if (gMouse_in_use && pCurrent_mode) {
        GetMousePosition(&x_coord, &y_coord);
        gLast_graph_sel__newgame = -1;
    }
    DrawColumnHeading__newgame(kMiscString_NetworkGamesTableHeading_HOST, gCurrent_graf_data->joinable_games_x_1);
    DrawColumnHeading__newgame(kMiscString_NetworkGamesTableHeading_GAME_TYPE, gCurrent_graf_data->joinable_games_x_2);
    DrawColumnHeading__newgame(kMiscString_NetworkGamesTableHeading_PLAYERS, gCurrent_graf_data->joinable_games_x_3);
    DrawColumnHeading__newgame(kMiscString_NetworkGamesTableHeading_STATUS, gCurrent_graf_data->joinable_games_x_4);
    BrPixelmapLine(gBack_screen,
        gCurrent_graf_data->joinable_games_sel_left,
        gCurrent_graf_data->joinable_games_y + gFonts[kFont_GRYLIT].height + 1 - (TranslationMode() ? 2 : 0) - gCurrent_graf_data->joinable_games_y_pitch,
        gCurrent_graf_data->joinable_games_sel_right - 1,
        gCurrent_graf_data->joinable_games_y + gFonts[kFont_GRYLIT].height + 1 - (TranslationMode() ? 2 : 0) - gCurrent_graf_data->joinable_games_y_pitch,
        6);
    for (i = 0; i < COUNT_OF(gGames_to_join); i++) {
        if (gGames_to_join[i].game == NULL) {
            continue;
        }
        if (gGames_to_join[i].game->type < 0 || gGames_to_join[i].game->type >= eNet_game_type_count) {
            continue;
        }
        if ((PDGetTotalTime() - gGames_to_join[i].time) >= 15000) {
            DisposeJoinableGame(i);
            continue;
        }
        if (gMouse_in_use
            && pCurrent_mode != 0
            && gGames_to_join[i].game != NULL
            && (gGames_to_join[i].game->options.open_game || gGames_to_join[i].game->no_races_yet)
            && gGames_to_join[i].game->num_players <= 5
            && x_coord >= gCurrent_graf_data->joinable_games_sel_left
            && x_coord <= gCurrent_graf_data->joinable_games_sel_right
            && y_coord >= (gCurrent_graf_data->joinable_games_y + gCurrent_graf_data->joinable_games_sel_top_marg + current_index * gCurrent_graf_data->joinable_games_y_pitch)
            && y_coord <= (gCurrent_graf_data->joinable_games_y + gCurrent_graf_data->joinable_games_sel_bot_marg + current_index * gCurrent_graf_data->joinable_games_y_pitch - 1)) {
            gLast_graph_sel__newgame = i;
        }
        if (i == gLast_graph_sel__newgame) {
            font_index = 10;
        } else {
            font_index = 9;
        }
        sprintf(s, "%s", gGames_to_join[i].game->host_name);
        DrawAnItem__newgame(gCurrent_graf_data->joinable_games_x_1, current_index, font_index, s);
        sprintf(s, "%s", GetMiscString(kMiscString_NetworkGameTypeNames_START + gGames_to_join[i].game->type));
        DrawAnItem__newgame(gCurrent_graf_data->joinable_games_x_2, current_index, font_index, s);
        sprintf(s, "%d", gGames_to_join[i].game->num_players);
        DrawAnItem__newgame(gCurrent_graf_data->joinable_games_x_3, current_index, font_index, s);
        sprintf(s, "%s, %s",
            GetMiscString(kMiscString_NetworkGameStage_START + gGames_to_join[i].game->status.stage),
            GetMiscString(kMiscString_NetworkGameOpenGame_START + gGames_to_join[i].game->options.open_game));
        DrawAnItem__newgame(gCurrent_graf_data->joinable_games_x_4, current_index, font_index, s);
        if (i == gLast_graph_sel__newgame) {
            DrawRectangle(gBack_screen,
                gCurrent_graf_data->joinable_games_sel_left,
                gCurrent_graf_data->joinable_games_y + gCurrent_graf_data->joinable_games_sel_top_marg + gCurrent_graf_data->joinable_games_y_pitch * current_index,
                gCurrent_graf_data->joinable_games_sel_right - 1,
                gCurrent_graf_data->joinable_games_y + gCurrent_graf_data->joinable_games_sel_bot_marg + gCurrent_graf_data->joinable_games_y_pitch * current_index - 1,
                45);
        }
        current_index++;
    }

    if (current_index != 0 && (gShifted_default_yet == 0 || (gLast_graph_sel__newgame >= 0 && (gGames_to_join[gLast_graph_sel__newgame].game == NULL || (!gGames_to_join[gLast_graph_sel__newgame].game->options.open_game && !gGames_to_join[gLast_graph_sel__newgame].game->no_races_yet) || gGames_to_join[gLast_graph_sel__newgame].game->num_players > 5)))) {
        gShifted_default_yet = 1;
        for (i = 0; i < COUNT_OF(gGames_to_join); i++) {
            if (gGames_to_join[i].game != NULL && (gGames_to_join[i].game->options.open_game || gGames_to_join[i].game->no_races_yet) && gGames_to_join[i].game->num_players <= 5) {
                gLast_graph_sel__newgame = i;
                ChangeSelectionTo(2, 1);
                return;
            }
        }
    }
    if (pCurrent_mode != 0 &&
#if defined(DETHRACE_FIX_BUGS)
        (gLast_graph_sel__newgame >= 0) &&
#endif
        (current_index == 0
            || gGames_to_join[gLast_graph_sel__newgame].game == NULL
            || (!gGames_to_join[gLast_graph_sel__newgame].game->options.open_game && !gGames_to_join[gLast_graph_sel__newgame].game->no_races_yet)
            || gGames_to_join[gLast_graph_sel__newgame].game->num_players > 5)) {
        gLast_graph_sel__newgame = -1;
        ChangeSelectionTo(0, 0);
    }
}

// IDA: void __cdecl InitGamesToJoin()
// FUNCTION: CARM95 0x004b1253
void InitGamesToJoin(void) {
    int i;

    for (i = 0; i < COUNT_OF(gGames_to_join); i++) {
        gGames_to_join[i].game = NULL;
    }
    gLast_graph_sel__newgame = -1;
}

// IDA: void __usercall DisposeJoinList(int pExemption@<EAX>)
// FUNCTION: CARM95 0x004b1297
void DisposeJoinList(int pExemption) {
    int i;

    for (i = 0; i < COUNT_OF(gGames_to_join); i++) {
        if (i == pExemption) {
            continue;
        }
        if (gGames_to_join[i].game != NULL) {
            DisposeJoinableGame(i);
        }
    }
}

// IDA: void __usercall AddToJoinList(tNet_game_details *pGame@<EAX>)
// FUNCTION: CARM95 0x004b0f0f
void AddToJoinList(tNet_game_details* pGame) {
    int i;
    int slot_to_use;
    tU32 this_game_ID;

    slot_to_use = -1;
    this_game_ID = NetExtractGameID(pGame);
    for (i = 0; i < COUNT_OF(gGames_to_join); i++) {
        if (gGames_to_join[i].game != NULL) {
            if (NetExtractGameID(gGames_to_join[i].game) == this_game_ID) {
                DisposeJoinableGame(i);
                slot_to_use = i;
                break;
            }
        }
    }
    if (slot_to_use < 0) {
        for (i = 0; i < COUNT_OF(gGames_to_join); i++) {
            if (gGames_to_join[i].game == NULL) {
                slot_to_use = i;
                break;
            }
        }
    }
    if (slot_to_use >= 0) {
        gGames_to_join[slot_to_use].game = pGame;
        gGames_to_join[slot_to_use].time = PDGetTotalTime();
    }
}

// IDA: void __cdecl NewNetStart1()
// FUNCTION: CARM95 0x004b0fff
void NewNetStart1(void) {

    gTyping = 1;
    SetSlotXY(0, gCurrent_graf_data->net_player_name_x, gCurrent_graf_data->net_player_name_y);
    AddRollingString(gNet_player_name,
        gCurrent_graf_data->net_player_name_x, gCurrent_graf_data->net_player_name_y, eRT_alpha);
}

// IDA: void __cdecl NewNetStart2()
// FUNCTION: CARM95 0x004b105d
void NewNetStart2(void) {

    gTyping_slot = 0;
    StartTyping(0, gNet_player_name, 11);
}

// IDA: void __usercall NewNetGetName(int pStarting_to_type@<EAX>, int pCurrent_choice@<EDX>, char *pString@<EBX>, int *pMax_length@<ECX>)
// FUNCTION: CARM95 0x004b1083
void NewNetGetName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {

    strcpy(pString, gNet_player_name);
    *pMax_length = 10;
}

// IDA: int __usercall NewNetDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
// FUNCTION: CARM95 0x004b10be
int NewNetDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {

    if (pCurrent_choice != 1) {
        GetTypedName(gNet_player_name, 10);
    }
    return pCurrent_choice;
}

// IDA: int __usercall NewNetGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b10ea
int NewNetGoAhead(int* pCurrent_choice, int* pCurrent_mode) {

    return gLast_graph_sel__newgame >= 0;
}

// IDA: tJoin_or_host_result __usercall JoinOrHostGame@<EAX>(tNet_game_details **pGame_to_join@<EAX>)
// FUNCTION: CARM95 0x004b1113
tJoin_or_host_result JoinOrHostGame(tNet_game_details** pGame_to_join) {
    static tFlicette flicker_on[2] = {
        { 43, { 41, 122 }, { 164, 370 } },
        { 43, { 230, 440 }, { 164, 370 } },
    };
    static tFlicette flicker_off[2] = {
        { 42, { 41, 122 }, { 164, 370 } },
        { 42, { 230, 440 }, { 164, 370 } },
    };
    static tFlicette push[2] = {
        { 90, { 41, 122 }, { 164, 370 } },
        { 45, { 230, 440 }, { 164, 370 } },
    };
    static tMouse_area mouse_areas[3] = {
        { { 41, 122 }, { 164, 370 }, { 104, 326 }, { 184, 422 }, 0, 0, 1, NULL },
        { { 230, 440 }, { 164, 370 }, { 293, 568 }, { 184, 422 }, 1, 0, 1, NULL },
        { { 42, 94 }, { 57, 137 }, { 290, 556 }, { 150, 341 }, 2, 1, 1, NULL },
    };
    static tRectile recopy_areas[1] = {
        {
            { 124, 110 },
            { 163, 317 },
            { 212, 322 },
            { 180, 370 },
        },
    };
    static tInterface_spec interface_spec = {
        0,
        100,
        0,
        101,
        101,
        101,
        -1,
        { 0, -1 },
        { -1, -1 },
        { 0, 2 },
        { 1, 2 },
        { NULL, NULL },
        { 0, -1 },
        { 1, -1 },
        { 0, 2 },
        { 1, 2 },
        { NULL, NULL },
        { 1, -1 },
        { 0, 0 },
        { 2, 0 },
        { 2, 0 },
        { NewNetGameUp, NewNetGameUp },
        { 1, -1 },
        { 0, 0 },
        { 2, 0 },
        { 2, 0 },
        { NewNetGameDown, NewNetGameDown },
        { 1, 1 },
        { NULL, NewNetGoAhead },
        { 1, 1 },
        { NULL, NULL },
        NULL,
        DrawGames,
        0,
        NewNetStart1,
        NewNetStart2,
        NewNetDone,
        1,
        { 1, 1 },
        NewNetGetName,
        1,
        1,
        COUNT_OF(flicker_on),
        flicker_on,
        flicker_off,
        push,
        COUNT_OF(mouse_areas),
        mouse_areas,
        COUNT_OF(recopy_areas),
        recopy_areas,
    };
    int result;

    gShifted_default_yet = 0;
    LoadFont(kFont_GRNDK);
    LoadFont(kFont_GRNLIT);
    LoadFont(kFont_GREENHED);
    LoadFont(kFont_GRYLIT);
    SetAlwaysTyping();
    InitGamesToJoin();
    NetStartProducingJoinList(AddToJoinList);
    result = DoInterfaceScreen(&interface_spec, 0, 0);
    ClearAlwaysTyping();
    NetEndJoinList();
    DisposeJoinList(gLast_graph_sel__newgame);
    DisposeFont(9);
    DisposeFont(10);
    DisposeFont(3);
    DisposeFont(12);
    strcpy(gProgram_state.player_name[0], gNet_player_name);
    SaveOptions();
    switch (result) {
    case 0:
        return eJoin_or_host_host;
    case 1:
        return eJoin_or_host_cancel;
    case 2:
        *pGame_to_join = gGames_to_join[gLast_graph_sel__newgame].game;
        return eJoin_or_host_join;
    default:
        return eJoin_or_host_cancel;
    }
}

// IDA: void __usercall GetNetOptions(tNet_game_options *pGame_options@<EAX>)
// FUNCTION: CARM95 0x004b2d9b
void GetNetOptions(tNet_game_options* pGame_options) {

    pGame_options->enable_text_messages = gRadio_bastards__newgame[0].current_value;
    pGame_options->show_players_on_map = gRadio_bastards__newgame[1].current_value;
    pGame_options->show_peds_on_map = gRadio_bastards__newgame[2].current_value;
    pGame_options->show_powerups_on_map = gRadio_bastards__newgame[3].current_value;
    pGame_options->powerup_respawn = gRadio_bastards__newgame[4].current_value;
    pGame_options->open_game = !gRadio_bastards__newgame[5].current_value;
    pGame_options->grid_start = !gRadio_bastards__newgame[6].current_value;
    pGame_options->race_sequence_type = gRadio_bastards__newgame[7].current_value ? eNet_sequence_sequential : eNet_sequence_random;
    pGame_options->random_car_choice = gRadio_bastards__newgame[8].current_value;
    pGame_options->car_choice = gRadio_bastards__newgame[9].current_value;
    pGame_options->starting_money_index = gRadio_bastards__newgame[10].current_value;
}

// IDA: void __usercall SetNetOptions(tNet_game_options *pGame_options@<EAX>)
// FUNCTION: CARM95 0x004b1b29
void SetNetOptions(tNet_game_options* pGame_options) {

    gRadio_bastards__newgame[0].current_value = pGame_options->enable_text_messages;
    gRadio_bastards__newgame[1].current_value = pGame_options->show_players_on_map;
    gRadio_bastards__newgame[2].current_value = pGame_options->show_peds_on_map;
    gRadio_bastards__newgame[3].current_value = pGame_options->show_powerups_on_map;
    gRadio_bastards__newgame[4].current_value = pGame_options->powerup_respawn;
    gRadio_bastards__newgame[5].current_value = !pGame_options->open_game;
    gRadio_bastards__newgame[6].current_value = !pGame_options->grid_start;
    gRadio_bastards__newgame[7].current_value = pGame_options->race_sequence_type == eNet_sequence_sequential;
    gRadio_bastards__newgame[8].current_value = pGame_options->random_car_choice;
    gRadio_bastards__newgame[9].current_value = pGame_options->car_choice;
    gRadio_bastards__newgame[10].current_value = pGame_options->starting_money_index;
}

// IDA: void __usercall NetPlayCheckboxOn2(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004b13e5
void NetPlayCheckboxOn2(int pIndex) {

    RunFlicAt(95,
        gRadio_bastards__newgame[pIndex].left[0],
        gRadio_bastards__newgame[pIndex].top + 1);
}

// IDA: void __usercall NetPlayCheckboxOff2(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004b1415
void NetPlayCheckboxOff2(int pIndex) {

    RunFlicAt(96,
        gRadio_bastards__newgame[pIndex].left[0],
        gRadio_bastards__newgame[pIndex].top + 1);
}

// IDA: void __usercall NetPlayCheckboxOn(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004b167d
void NetPlayCheckboxOn(int pIndex) {

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    NetPlayCheckboxOn2(pIndex);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __usercall NetPlayCheckboxOff(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004b16b2
void NetPlayCheckboxOff(int pIndex) {

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    NetPlayCheckboxOff2(pIndex);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __usercall NetPlayRadioOn2(int pIndex@<EAX>, int pValue@<EDX>)
// FUNCTION: CARM95 0x004b1445
void NetPlayRadioOn2(int pIndex, int pValue) {

    RunFlicAt(288,
        gRadio_bastards__newgame[pIndex].left[pValue],
        gRadio_bastards__newgame[pIndex].top + 1);
}

// IDA: void __usercall NetPlayRadioOff2(int pIndex@<EAX>, int pValue@<EDX>)
// FUNCTION: CARM95 0x004b147c
void NetPlayRadioOff2(int pIndex, int pValue) {

    RunFlicAt(287,
        gRadio_bastards__newgame[pIndex].left[pValue],
        gRadio_bastards__newgame[pIndex].top + 1);
}

// IDA: void __usercall NetPlayRadioOn(int pIndex@<EAX>, int pValue@<EDX>)
// FUNCTION: CARM95 0x004b1598
void NetPlayRadioOn(int pIndex, int pValue) {

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    NetPlayRadioOn2(pIndex, pValue);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __usercall NetPlayRadioOff(int pIndex@<EAX>, int pValue@<EDX>)
// FUNCTION: CARM95 0x004b15d1
void NetPlayRadioOff(int pIndex, int pValue) {

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    NetPlayRadioOff2(pIndex, pValue);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __cdecl DrawNOptInitialRadios()
// FUNCTION: CARM95 0x004b12ec
void DrawNOptInitialRadios(void) {
    int i;
    int j;

    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    for (i = 0; i < COUNT_OF(gRadio_bastards__newgame); i++) {
        if (gRadio_bastards__newgame[i].count < 2) {
            if (gRadio_bastards__newgame[i].current_value) {
                NetPlayCheckboxOn2(i);
            } else {
                NetPlayCheckboxOff2(i);
            }
        } else {
            NetPlayRadioOn2(i, gRadio_bastards__newgame[i].current_value);
            for (j = 0; j < gRadio_bastards__newgame[i].count; j++) {
                if (j != gRadio_bastards__newgame[i].current_value) {
                    NetPlayRadioOff2(i, j);
                }
            }
        }
    }
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __usercall NetRadioChanged(int pIndex@<EAX>, int pNew_value@<EDX>)
// FUNCTION: CARM95 0x004b1555
void NetRadioChanged(int pIndex, int pNew_value) {

    NetPlayRadioOff(pIndex, gRadio_bastards__newgame[pIndex].current_value);
    NetPlayRadioOn(pIndex, pNew_value);
    gRadio_bastards__newgame[pIndex].current_value = pNew_value;
}

// IDA: void __usercall NetCheckboxChanged(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004b160a
void NetCheckboxChanged(int pIndex) {

    if (gRadio_bastards__newgame[pIndex].current_value) {
        NetPlayCheckboxOff(pIndex);
    } else {
        NetPlayCheckboxOn(pIndex);
    }
    gRadio_bastards__newgame[pIndex].current_value = !gRadio_bastards__newgame[pIndex].current_value;
}

// IDA: int __usercall NetOptLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b14b3
int NetOptLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;

    DRS3StartSound(gEffects_outlet, 3000);
    if (gRadio_bastards__newgame[*pCurrent_choice - 3].count < 2) {
        NetCheckboxChanged(*pCurrent_choice - 3);
    } else {
        new_value = gRadio_bastards__newgame[*pCurrent_choice - 3].current_value - 1;
        if (new_value < 0) {
            new_value = gRadio_bastards__newgame[*pCurrent_choice - 3].count - 1;
        }
        NetRadioChanged(*pCurrent_choice - 3, new_value);
    }
    return 1;
}

// IDA: int __usercall NetOptRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b16e7
int NetOptRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;

    DRS3StartSound(gEffects_outlet, 3000);
    if (gRadio_bastards__newgame[*pCurrent_choice - 3].count < 2) {
        NetCheckboxChanged(*pCurrent_choice - 3);
    } else {
        new_value = gRadio_bastards__newgame[*pCurrent_choice - 3].current_value + 1;
        if (new_value == gRadio_bastards__newgame[*pCurrent_choice - 3].count) {
            new_value = 0;
        }
        NetRadioChanged(*pCurrent_choice - 3, new_value);
    }
    return 1;
}

// IDA: int __usercall NetOptUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b178b
int NetOptUp(int* pCurrent_choice, int* pCurrent_mode) {

    if (*pCurrent_mode == 0) {
        *pCurrent_mode = 1;
        *pCurrent_choice = 13;
        DRS3StartSound(gEffects_outlet, 3000);
        return 1;
    } else if (*pCurrent_choice == 2) {
        *pCurrent_mode = 0;
        *pCurrent_choice = 0;
        DRS3StartSound(gEffects_outlet, 3000);
        return 1;
    } else {
        return 0;
    }
}

// IDA: int __usercall NetOptDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b181d
int NetOptDown(int* pCurrent_choice, int* pCurrent_mode) {

    if (*pCurrent_mode == 0) {
        *pCurrent_mode = 1;
        *pCurrent_choice = 3;
        DRS3StartSound(gEffects_outlet, 3000);
        return 1;
    } else if (*pCurrent_choice == 14) {
        *pCurrent_mode = 0;
        *pCurrent_choice = 0;
        DRS3StartSound(gEffects_outlet, 3000);
        return 1;
    } else {
        return 0;
    }
}

// IDA: int __usercall NetRadioClick@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
// FUNCTION: CARM95 0x004b18af
int NetRadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;

    if (gRadio_bastards__newgame[*pCurrent_choice - 3].count < 2) {
        NetCheckboxChanged(*pCurrent_choice - 3);
    } else {
        for (i = gRadio_bastards__newgame[*pCurrent_choice - 3].count - 1; i >= 0; i--) {
            if (gThe_interface_spec__newgame->mouse_areas[3].left[gGraf_data_index] + pX_offset + 3 >= gRadio_bastards__newgame[*pCurrent_choice - 3].left[i]) {
                DRS3StartSound(gEffects_outlet, 3000);
                NetRadioChanged(*pCurrent_choice - 3, i);
                break;
            }
        }
    }
    return 0;
}

// IDA: void __cdecl RevertToDefaults()
// FUNCTION: CARM95 0x004b1a8e
void RevertToDefaults(void) {
    tPath_name the_path;
    FILE* f;
    tNet_game_options net_options;
    int i;

    PathCat(the_path, gApplication_path, "NETDEFLT.TXT");
    f = DRfopen(the_path, "rt");
    if (f != NULL) {
        for (i = 0; i < gCurrent_game_selection + 1; i++) {
            ReadNetworkSettings(f, &net_options);
        }
        SetNetOptions(&net_options);
        DrawNOptInitialRadios();
        fclose(f);
    }
}

// IDA: void __cdecl DefaultNetSettings()
// FUNCTION: CARM95 0x004b1984
void DefaultNetSettings(void) {
    FILE* f;
    int i;
    tPath_name the_path; // added

    PathCat(the_path, gApplication_path, "NETDEFLT.TXT");
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        return;
    }
    ReadNetworkSettings(f, gNet_settings);
    rewind(f);
    for (i = 0; i < COUNT_OF(gNet_settings) - 1; i++) {
        ReadNetworkSettings(f, &gNet_settings[i + 1]);
    }
    fclose(f);
}

// IDA: int __usercall NetOptGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b1a34
int NetOptGoAhead(int* pCurrent_choice, int* pCurrent_mode) {

    if (*pCurrent_mode == 0) {
        if (*pCurrent_choice == 2) {
            RevertToDefaults();
            return 0;
        } else {
            return 1;
        }
    } else {
        NetOptRight(pCurrent_choice, pCurrent_mode);
        return 0;
    }
}

// IDA: void __usercall NetPlotAGraphBox(int pIndex@<EAX>, int pColour_value@<EDX>)
// FUNCTION: CARM95 0x004b1c58
void NetPlotAGraphBox(int pIndex, int pColour_value) {

    if (pIndex >= 0) {
        DrawRRectangle(gBack_screen,
            gThe_interface_spec__newgame->mouse_areas[pIndex + 3].left[gGraf_data_index],
            gThe_interface_spec__newgame->mouse_areas[pIndex + 3].top[gGraf_data_index],
            gThe_interface_spec__newgame->mouse_areas[pIndex + 3].right[gGraf_data_index],
            gThe_interface_spec__newgame->mouse_areas[pIndex + 3].bottom[gGraf_data_index],
            pColour_value);
    }
}

// IDA: void __usercall NetDrawAGraphBox(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004b1c3f
void NetDrawAGraphBox(int pIndex) {

    NetPlotAGraphBox(pIndex, 45);
}

// IDA: void __usercall NetEraseAGraphBox(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004b1d0a
void NetEraseAGraphBox(int pIndex) {

    NetPlotAGraphBox(pIndex, 0);
}

// IDA: void __usercall DrawNetOptBox(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b1bfd
void DrawNetOptBox(int pCurrent_choice, int pCurrent_mode) {

    if (gRadio_selected != pCurrent_choice) {
        NetEraseAGraphBox(gRadio_selected - 3);
        NetDrawAGraphBox(pCurrent_choice - 3);
        gRadio_selected = pCurrent_choice;
    }
}

// IDA: void __usercall DoNetOptions(tNet_game_options *pGame_options@<EAX>)
// FUNCTION: CARM95 0x004b2d37
void DoNetOptions(tNet_game_options* pGame_options) {
    static tFlicette flicker_on[14] = {
        { 43, { 169, 90 }, { 156, 398 } },
        { 43, { 236, 440 }, { 156, 398 } },
        { 43, { 37, 440 }, { 156, 398 } },
        { 68, { 81, 98 }, { 32, 86 } },
        { 75, { 81, 98 }, { 43, 108 } },
        { 77, { 81, 98 }, { 54, 130 } },
        { 79, { 81, 98 }, { 65, 161 } },
        { 112, { 81, 98 }, { 76, 182 } },
        { 127, { 81, 98 }, { 86, 214 } },
        { 129, { 81, 98 }, { 97, 235 } },
        { 134, { 81, 98 }, { 108, 266 } },
        { 137, { 81, 98 }, { 119, 288 } },
        { 139, { 81, 98 }, { 130, 310 } },
        { 143, { 81, 98 }, { 141, 331 } },
    };
    static tFlicette flicker_off[14] = {
        { 42, { 169, 90 }, { 156, 398 } },
        { 42, { 236, 440 }, { 156, 398 } },
        { 42, { 37, 440 }, { 156, 398 } },
        { 69, { 81, 98 }, { 32, 86 } },
        { 76, { 81, 98 }, { 43, 108 } },
        { 78, { 81, 98 }, { 54, 130 } },
        { 109, { 81, 98 }, { 65, 161 } },
        { 113, { 81, 98 }, { 76, 182 } },
        { 128, { 81, 98 }, { 86, 214 } },
        { 133, { 81, 98 }, { 97, 235 } },
        { 136, { 81, 98 }, { 108, 266 } },
        { 138, { 81, 98 }, { 119, 288 } },
        { 142, { 81, 98 }, { 130, 310 } },
        { 148, { 81, 98 }, { 141, 331 } },
    };
    static tFlicette push[14] = {
        { 154, { 169, 90 }, { 156, 398 } },
        { 45, { 236, 440 }, { 156, 398 } },
        { 67, { 37, 440 }, { 156, 398 } },
        { 68, { 81, 98 }, { 32, 86 } },
        { 75, { 81, 98 }, { 43, 108 } },
        { 77, { 81, 98 }, { 54, 130 } },
        { 79, { 81, 98 }, { 65, 161 } },
        { 112, { 81, 98 }, { 76, 182 } },
        { 127, { 81, 98 }, { 86, 214 } },
        { 129, { 81, 98 }, { 97, 235 } },
        { 134, { 81, 98 }, { 108, 266 } },
        { 137, { 81, 98 }, { 119, 288 } },
        { 139, { 81, 98 }, { 130, 310 } },
        { 143, { 81, 98 }, { 141, 331 } },
    };
    static tMouse_area mouse_areas[14] = {
        { { 169, 90 }, { 156, 396 }, { 232, 214 }, { 176, 444 }, 0, 0, 0, NULL },
        { { 236, 440 }, { 156, 396 }, { 299, 552 }, { 176, 444 }, 1, 0, 0, NULL },
        { { 37, 98 }, { 156, 84 }, { 100, 568 }, { 176, 103 }, 2, 0, 0, NULL },
        { { 61, 98 }, { 30, 106 }, { 270, 568 }, { 40, 125 }, 3, 1, 0, NetRadioClick },
        { { 61, 98 }, { 41, 127 }, { 270, 568 }, { 51, 146 }, 4, 1, 0, NetRadioClick },
        { { 61, 98 }, { 52, 158 }, { 270, 568 }, { 62, 178 }, 5, 1, 0, NetRadioClick },
        { { 61, 98 }, { 63, 180 }, { 270, 568 }, { 73, 199 }, 6, 1, 0, NetRadioClick },
        { { 61, 98 }, { 74, 192 }, { 270, 568 }, { 84, 230 }, 7, 1, 0, NetRadioClick },
        { { 61, 98 }, { 84, 233 }, { 270, 568 }, { 94, 252 }, 8, 1, 0, NetRadioClick },
        { { 61, 98 }, { 95, 264 }, { 270, 568 }, { 105, 283 }, 9, 1, 0, NetRadioClick },
        { { 61, 98 }, { 106, 286 }, { 270, 568 }, { 116, 305 }, 10, 1, 0, NetRadioClick },
        { { 61, 98 }, { 117, 307 }, { 270, 322 }, { 127, 326 }, 11, 1, 0, NetRadioClick },
        { { 61, 98 }, { 128, 329 }, { 270, 322 }, { 138, 348 }, 12, 1, 0, NetRadioClick },
        { { 61, 98 }, { 139, 358 }, { 270, 322 }, { 149, 377 }, 13, 1, 0, NetRadioClick },
    };
    static tInterface_spec interface_spec = {
        0, 65, 0, 66, 66, 66, -1,
        { -1, 0 }, { -1, 0 }, { 0, 3 }, { 2, 13 }, { NULL, NetOptLeft },
        { -1, 0 }, { 1, 0 }, { 0, 3 }, { 2, 13 }, { NULL, NetOptRight },
        { -1, -1 }, { 0, -1 }, { 0, 2 }, { 0, 13 }, { NetOptUp, NetOptUp },
        { -1, -1 }, { 1, 1 }, { 0, 3 }, { 0, 14 }, { NetOptDown, NetOptDown },
        { 1, 1 }, { NetOptGoAhead, NetOptGoAhead }, { 1, 1 }, { NULL, NULL },
        NULL, DrawNetOptBox, 0,
        NULL, DrawNOptInitialRadios, NULL, 0, { 0, 0 }, NULL, 1, 1,
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas,
        0, NULL
    };

    gThe_interface_spec__newgame = &interface_spec;
    gRadio_selected = -1;
    LoadFont(kFont_GRYLIT);
    SetNetOptions(pGame_options);
    if (DoInterfaceScreen(&interface_spec, 0, 0) == 0) {
        GetNetOptions(pGame_options);
    }
    DisposeFont(12);
}

// IDA: void __usercall PlayRadioOn(int pIndex@<EAX>)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004b1d3c
void PlayRadioOn__newgame(int pIndex) {

    RunFlicAt(288,
        gThe_interface_spec__newgame->pushed_flics[pIndex + 4].x[gGraf_data_index],
        gThe_interface_spec__newgame->pushed_flics[pIndex + 4].y[gGraf_data_index]);
}

// IDA: void __usercall PlayRadioOff(int pIndex@<EAX>)
//  Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004b1f33
void PlayRadioOff__newgame(int pIndex) {

    RunFlicAt(287,
        gThe_interface_spec__newgame->pushed_flics[pIndex + 4].x[gGraf_data_index],
        gThe_interface_spec__newgame->pushed_flics[pIndex + 4].y[gGraf_data_index]);
}

// IDA: void __usercall SetOptions(tNet_game_type pGame_type@<EAX>, tNet_game_options *pGame_options@<EDX>)
// FUNCTION: CARM95 0x004b1e95
void SetOptions(tNet_game_type pGame_type, tNet_game_options* pGame_options) {

    pGame_options->show_players_on_map = gNet_settings[0].show_players_on_map;
    pGame_options->show_peds_on_map = gNet_settings[pGame_type + 1].show_peds_on_map;
    pGame_options->enable_text_messages = gNet_settings[0].enable_text_messages;
    pGame_options->powerup_respawn = gNet_settings[0].powerup_respawn;
    pGame_options->show_powerups_on_map = gNet_settings[0].show_powerups_on_map;
    pGame_options->open_game = gNet_settings[pGame_type + 1].open_game;
    pGame_options->grid_start = gNet_settings[pGame_type + 1].grid_start;
    pGame_options->random_car_choice = gNet_settings[0].random_car_choice;
    pGame_options->car_choice = gNet_settings[0].car_choice;
    pGame_options->starting_money_index = gNet_settings[0].starting_money_index;
    pGame_options->race_sequence_type = gNet_settings[0].race_sequence_type;
}

// IDA: void __cdecl DrawNetChooseInitial()
// FUNCTION: CARM95 0x004b1d23
void DrawNetChooseInitial(void) {

    PlayRadioOn__newgame(gCurrent_game_selection);
}

// IDA: int __usercall NetChooseGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b1d9c
int NetChooseGoAhead(int* pCurrent_choice, int* pCurrent_mode) {

    if (*pCurrent_mode == 0) {
        return 1;
    } else {
        if (*pCurrent_choice - 4 != gCurrent_game_selection) {
            RemoveTransientBitmaps(1);
            DontLetFlicFuckWithPalettes();
            TurnFlicTransparencyOn();
            PlayRadioOff__newgame(gCurrent_game_selection);
            gCurrent_game_selection = *pCurrent_choice - 4;
            PlayRadioOn__newgame(gCurrent_game_selection);
            LoadRaces(gRace_list, &gNumber_of_races, gCurrent_game_selection);
            SetOptions(gCurrent_game_selection, gOptions);
            TurnFlicTransparencyOff();
            LetFlicFuckWithPalettes();
            if (gRace_index >= gNumber_of_races) {
                gRace_index = PickNetRace(-1, gNet_race_sequence__newgame);
            }
            if (!gMouse_in_use) {
                *pCurrent_mode = 0;
                *pCurrent_choice = 0;
            }
        }
        return 0;
    }
}

// IDA: void __usercall PlotAGraphBox(int pIndex@<EAX>, int pColour_value@<EDX>)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004b2277
void PlotAGraphBox__newgame(int pIndex, int pColour_value) {

    if (pIndex >= 0) {
        DrawRRectangle(gBack_screen,
            gThe_interface_spec__newgame->mouse_areas[pIndex].left[gGraf_data_index],
            gThe_interface_spec__newgame->mouse_areas[pIndex].top[gGraf_data_index],
            gThe_interface_spec__newgame->mouse_areas[pIndex].right[gGraf_data_index],
            gThe_interface_spec__newgame->mouse_areas[pIndex].bottom[gGraf_data_index],
            pColour_value);
    }
}

// IDA: void __usercall DrawAGraphBox(int pIndex@<EAX>)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004b225e
void DrawAGraphBox__newgame(int pIndex) {

    PlotAGraphBox__newgame(pIndex, 45);
}

// IDA: void __usercall EraseAGraphBox(int pIndex@<EAX>)
// Suffix added to avoid duplicate symbol
// FUNCTION: CARM95 0x004b2325
void EraseAGraphBox__newgame(int pIndex) {

    PlotAGraphBox__newgame(pIndex, 0);
}

// IDA: void __usercall DrawNetChoose(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b1f93
void DrawNetChoose(int pCurrent_choice, int pCurrent_mode) {
    char s[256];
    tU32* k;
    int i;

    if (gLast_net_choose_box >= 0) {
        EraseAGraphBox__newgame(gLast_net_choose_box);
    }
    if (pCurrent_mode != 0) {
        gLast_net_choose_box = pCurrent_choice;
        DrawAGraphBox__newgame(gLast_net_choose_box);
    } else {
        gLast_net_choose_box = -1;
    }
    BrPixelmapRectangleFill(gBack_screen,
        gCurrent_graf_data->net_descr_race_l,
        gCurrent_graf_data->net_choose_race_y,
        gCurrent_graf_data->net_descr_race_r - gCurrent_graf_data->net_descr_race_l,
        gFonts[kFont_GRNLIT].height + 1 - (TranslationMode() ? 2 : 0), 0);
    sprintf(s, "%s %s", GetMiscString(kMiscString_FIRST_RACE), gRace_list[gRace_index].name);
    DRPixelmapCentredText(gBack_screen,
        gCurrent_graf_data->net_choose_race_x,
        gCurrent_graf_data->net_choose_race_y,
        &gFonts[kFont_GRNLIT], s);
    BrPixelmapRectangleFill(gBack_screen,
        gCurrent_graf_data->net_descr_race_l,
        gCurrent_graf_data->net_descr_race_top - (TranslationMode() ? 2 : 0),
        gCurrent_graf_data->net_descr_race_r - gCurrent_graf_data->net_descr_race_l,
        gCurrent_graf_data->net_descr_race_bot - gCurrent_graf_data->net_descr_race_top,
        0);
    OoerrIveGotTextInMeBoxMissus(9,
        GetMiscString(kMiscString_NetworkGameTypeDescriptions_START + (pCurrent_mode ? pCurrent_choice - 4 : gCurrent_game_selection)),
        gBack_screen,
        gCurrent_graf_data->net_descr_race_l,
        gCurrent_graf_data->net_descr_race_top,
        gCurrent_graf_data->net_descr_race_r,
        gCurrent_graf_data->net_descr_race_bot,
        1);
    k = KevKeyService();
    if (k[0] == 0x2212d981 && k[1] == 0x90e8cf51) {
        DRS3StartSound(gEffects_outlet, 3202);
        DRS3StartSound(gEffects_outlet, 3202);
        for (i = 0; i < gNumber_of_racers; i++) {
            if (gOpponents[i].network_availability == eNet_avail_never) {
                gOpponents[i].network_availability = eNet_avail_all;
            }
        }
    }
}

// IDA: int __usercall NetChooseLR@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
// FUNCTION: CARM95 0x004b233e
int NetChooseLR(int* pCurrent_choice, int* pCurrent_mode) {

    *pCurrent_choice = gCurrent_game_selection + 4;
    return 0;
}

// IDA: void __usercall SetGameTarget(tNet_game_type *pGame_type@<EAX>, tNet_game_options *pGame_options@<EDX>)
// FUNCTION: CARM95 0x004b2e6f
void SetGameTarget(tNet_game_type* pGame_type, tNet_game_options* pGame_options) {

    pGame_options->race_end_target = gNet_target[*pGame_type];
    switch (*pGame_type) {
    case eNet_game_type_car_crusher:
        pGame_options->race_end_target = gNet_target[*pGame_type];
        break;
    case eNet_game_type_tag:
        pGame_options->race_end_target = 1000 * gNet_target[*pGame_type];
        break;
    case eNet_game_type_foxy:
        pGame_options->race_end_target = 1000 * gNet_target[*pGame_type];
        break;
    default:
        pGame_options->race_end_target = gNet_target[*pGame_type];
        break;
    }
}

// IDA: int __usercall NetGameChoices@<EAX>(tNet_game_type *pGame_type@<EAX>, tNet_game_options *pGame_options@<EDX>, int *pRace_index@<EBX>)
// FUNCTION: CARM95 0x004b2ba3
int NetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index) {
    static tFlicette flicker_on[11] = {
        { 43, { 226, 90 }, { 117, 398 } },
        { 43, { 226, 440 }, { 148, 398 } },
        { 43, { 226, 98 }, { 42, 86 } },
        { 43, { 226, 98 }, { 74, 108 } },
        { 162, { 74, 98 }, { 73, 130 } },
        { 163, { 74, 98 }, { 83, 161 } },
        { 164, { 74, 98 }, { 93, 182 } },
        { 165, { 74, 98 }, { 103, 214 } },
        { 166, { 74, 98 }, { 113, 235 } },
        { 167, { 74, 98 }, { 123, 266 } },
        { 168, { 74, 98 }, { 133, 288 } },
    };
    static tFlicette flicker_off[11] = {
        { 42, { 226, 90 }, { 117, 398 } },
        { 42, { 226, 440 }, { 148, 398 } },
        { 42, { 226, 98 }, { 42, 86 } },
        { 42, { 226, 98 }, { 74, 108 } },
        { 182, { 74, 98 }, { 73, 130 } },
        { 183, { 74, 98 }, { 83, 161 } },
        { 184, { 74, 98 }, { 93, 182 } },
        { 185, { 74, 98 }, { 103, 214 } },
        { 186, { 74, 98 }, { 113, 235 } },
        { 187, { 74, 98 }, { 123, 266 } },
        { 188, { 74, 98 }, { 133, 288 } },
    };
    static tFlicette push[11] = {
        { 88, { 227, 90 }, { 117, 398 } },
        { 45, { 226, 440 }, { 148, 398 } },
        { 195, { 227, 98 }, { 42, 86 } },
        { 124, { 226, 98 }, { 74, 108 } },
        { 162, { 74, 98 }, { 73, 130 } },
        { 163, { 74, 98 }, { 83, 161 } },
        { 164, { 74, 98 }, { 93, 182 } },
        { 165, { 74, 98 }, { 103, 214 } },
        { 166, { 74, 98 }, { 113, 235 } },
        { 167, { 74, 98 }, { 123, 266 } },
        { 168, { 74, 98 }, { 133, 288 } },
    };
    static tMouse_area mouse_areas[11] = {
        { { 226, 90 }, { 117, 396 }, { 290, 214 }, { 137, 444 }, 0, 0, 0, NULL },
        { { 226, 440 }, { 148, 396 }, { 290, 552 }, { 168, 444 }, 1, 0, 0, NULL },
        { { 226, 98 }, { 42, 84 }, { 290, 568 }, { 62, 103 }, 2, 0, 0, NULL },
        { { 226, 98 }, { 74, 106 }, { 290, 568 }, { 94, 125 }, 3, 0, 0, NULL },
        { { 53, 98 }, { 71, 127 }, { 211, 568 }, { 79, 146 }, 4, 1, 0, NULL },
        { { 53, 98 }, { 81, 158 }, { 211, 568 }, { 89, 178 }, 5, 1, 0, NULL },
        { { 53, 98 }, { 91, 180 }, { 211, 568 }, { 99, 199 }, 6, 1, 0, NULL },
        { { 53, 98 }, { 101, 192 }, { 211, 568 }, { 109, 230 }, 7, 1, 0, NULL },
        { { 53, 98 }, { 111, 233 }, { 211, 568 }, { 119, 252 }, 8, 1, 0, NULL },
        { { 53, 98 }, { 121, 264 }, { 211, 568 }, { 129, 283 }, 9, 1, 0, NULL },
        { { 53, 98 }, { 131, 264 }, { 211, 568 }, { 139, 283 }, 10, 1, 0, NULL },
    };

    // GLOBAL: CARM95 0x0051FE48
    static tInterface_spec interface_spec = {
        0, 122, 0, 0, 0, 0, -1,
        { 1, 0 }, { 4, -10 }, { 4, 0 }, { 4, 0 }, { NetChooseLR, NULL },
        { 1, 0 }, { 3, -10 }, { 4, 0 }, { 4, 0 }, { NetChooseLR, NULL },
        { -1, -1 }, { -1, -1 }, { 0, 4 }, { 3, 10 }, { NULL, NULL },
        { -1, -1 }, { 1, 1 }, { 0, 4 }, { 3, 10 }, { NULL, NULL },
        { 1, 1 }, { NetChooseGoAhead, NetChooseGoAhead }, { 1, 1 }, { NULL, NULL },
        NULL, DrawNetChoose, 0, NULL, DrawNetChooseInitial, NULL, 0, { 0, 0 }, NULL,
        1, 1,
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas,
        0, NULL

    };
    int result;

    gRace_index = *pRace_index;
    gCurrent_game_selection = *pGame_type;
    do {
        LoadFont(kFont_GRNLIT);
        LoadFont(kFont_GRNDK);
        gThe_interface_spec__newgame = &interface_spec;
        gLast_net_choose_box = -1;
        gOptions = pGame_options;
        gNet_race_sequence__newgame = pGame_options->race_sequence_type;
        result = DoInterfaceScreen(&interface_spec, 0, 0);
        if (result || !pGame_options->random_car_choice) {
            RunFlic(123);
        } else {
            FadePaletteDown();
        }
        DisposeFont(9);
        DisposeFont(10);
        switch (result) {
        case 0:
            *pGame_type = gCurrent_game_selection;
            *pRace_index = gRace_index;
            break;
        case 2:
            ChangeRace(&gRace_index, 1, pGame_options->race_sequence_type);
            break;
        case 3:
            DoNetOptions(pGame_options);
            break;
        }
    } while (result > 1);
    gLast_game_type = *pGame_type;
    gNet_settings[0] = *pGame_options;
    gNet_settings[gLast_game_type + 1] = *pGame_options;
    SaveOptions();
    SetGameTarget(pGame_type, pGame_options);
    return result == 0;
}

// IDA: void __usercall ReadNetGameChoices(tNet_game_type *pGame_type@<EAX>, tNet_game_options *pGame_options@<EDX>, int *pRace_index@<EBX>)
// FUNCTION: CARM95 0x004b2f38
void ReadNetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index) {

    *pGame_type = gLast_game_type;
    SetOptions(*pGame_type, pGame_options);
    SetGameTarget(pGame_type, pGame_options);
    *pRace_index = PickNetRace(-1, pGame_options->race_sequence_type);
}

// IDA: int __usercall ChooseStartRace@<EAX>(int *pRank@<EAX>)
// FUNCTION: CARM95 0x004b235d
int ChooseStartRace(int* pRank) {

    *pRank = 0;
    return 1;
}

// IDA: void __usercall SetUpOtherNetThings(tNet_game_details *pNet_game@<EAX>)
// FUNCTION: CARM95 0x004b237b
void SetUpOtherNetThings(tNet_game_details* pNet_game) {

    StartLoadingScreen();
    gProgram_state.skill_level = 1;
    InitGame(pNet_game->start_race);
}

// IDA: void __usercall RequestCarDetails(tNet_game_details *pNet_game@<EAX>)
// FUNCTION: CARM95 0x004b246e
void RequestCarDetails(tNet_game_details* pNet_game) {
    tNet_message* message;

    gReceived_car_details = 0;
    if (gNet_mode == eNet_mode_none) {
        gNet_mode = eNet_mode_thinking_about_it;
    }
    message = NetBuildMessage(NETMSGID_CARDETAILSREQ, 0);
    NetGuaranteedSendMessageToAddress(pNet_game, message, &pNet_game->pd_net_info, NULL);
}

// IDA: int __cdecl PickARandomCar()
// FUNCTION: CARM95 0x004b23a4
int PickARandomCar(void) {
    int i;
    int cars[120];
    int array_size;

    array_size = 0;
    for (i = 0; i < gNumber_of_racers; i++) {
        if (gCar_details[i].ownership == eCar_owner_none) {
            cars[array_size] = i;
            array_size++;
            if (gOpponents[i].network_availability == eNet_avail_all) {
                cars[array_size] = i;
                array_size++;
            }
        }
    }
    return cars[IRandomBetween(0, array_size - 1)];
}

// IDA: void __usercall PollCarDetails(tNet_game_details *pNet_game@<EAX>)
// FUNCTION: CARM95 0x004b244a
void PollCarDetails(tNet_game_details* pNet_game) {

    if (gReceived_car_details) {
        RequestCarDetails(pNet_game);
    }
}

// IDA: void __usercall SetNetAvailability(tNet_game_options *pOptions@<EAX>)
// FUNCTION: CARM95 0x004b24c2
void SetNetAvailability(tNet_game_options* pOptions) {
    int i;

    for (i = 0; i < gNumber_of_racers; i++) {
        switch (gOpponents[i].network_availability) {
        case eNet_avail_never:
            gCar_details[i].ownership = eCar_owner_not_allowed;
            break;
        case eNet_avail_eagle:
            if (pOptions->car_choice == eNet_car_annie) {
                gCar_details[i].ownership = eCar_owner_not_allowed;
            } else {
                gCar_details[i].ownership = eCar_owner_none;
            }
            break;
        case eNet_avail_hawk:
            if (pOptions->car_choice == eNet_car_frankie) {
                gCar_details[i].ownership = eCar_owner_not_allowed;
            } else {
                gCar_details[i].ownership = eCar_owner_none;
            }
            break;
        case eNet_avail_all:
            if (pOptions->car_choice == eNet_car_all) {
                gCar_details[i].ownership = eCar_owner_none;
            } else {
                gCar_details[i].ownership = eCar_owner_not_allowed;
            }
        }
    }
}

// IDA: int __usercall ChooseNetCar@<EAX>(tNet_game_details *pNet_game@<EAX>, tNet_game_options *pOptions@<EDX>, int *pCar_index@<EBX>, int pIm_the_host_so_fuck_off@<ECX>)
// FUNCTION: CARM95 0x004b25fa
int ChooseNetCar(tNet_game_details* pNet_game, tNet_game_options* pOptions, int* pCar_index, int pIm_the_host_so_fuck_off) {
    tS32 start_time;
    int i;
    int result;
    int car_index;
    int the_car_index;

    if (!pOptions->random_car_choice || pIm_the_host_so_fuck_off) {
        gNet_options = pOptions;
        if (pIm_the_host_so_fuck_off) {
            SetNetAvailability(pOptions);
        } else {
            RequestCarDetails(pNet_game);
            start_time = PDGetTotalTime();
            while (!gReceived_car_details && PDGetTotalTime() - start_time < 10000) {
                NetService(0);
            }
            gNet_mode = eNet_mode_none;
            if (!gReceived_car_details) {
                gNet_mode = eNet_mode_none;
                return 0;
            }
        }
        if (pOptions->random_car_choice) {
            *pCar_index = PickARandomCar();
            if (pIm_the_host_so_fuck_off && *pCar_index >= 0) {
                gCar_details[*pCar_index].ownership = eCar_owner_someone;
            }
            result = 1;
        } else {
            if (*pCar_index < 0) {
                *pCar_index = PickARandomCar();
                car_index = 0;
                for (i = 0; i < gNumber_of_racers; i++) {
                    if (gCar_details[i].ownership < eCar_owner_not_allowed) {
                        gProgram_state.cars_available[car_index] = i;
                        car_index++;
                    }
                }
                gProgram_state.number_of_cars = car_index;
            }
            result = ChangeCar(1, pCar_index, pNet_game);
            gNet_mode = eNet_mode_none;
            if (pIm_the_host_so_fuck_off) {
                gCar_details[*pCar_index].ownership = eCar_owner_someone;
            }
        }
    } else {
        result = 1;
    }
    return result;
}

// IDA: void __cdecl InitNetStorageSpace()
// FUNCTION: CARM95 0x004b2f85
void InitNetStorageSpace(void) {
    int i;

    for (i = 0; i < COUNT_OF(gNet_players); i++) {
        gCurrent_race.opponent_list[i].car_spec = NULL;
    }
    gNet_storage_allocated = 1;
    InitialiseStorageSpace(&gNet_cars_storage_space, 500, 50, 500, 300);
}

// IDA: void __cdecl DisposeNetStorageSpace()
// FUNCTION: CARM95 0x004b2fe9
void DisposeNetStorageSpace(void) {

    if (gNet_storage_allocated) {
        DisposeStorageSpace(&gNet_cars_storage_space);
    }
    gNet_storage_allocated = 0;
}

// IDA: int __cdecl DoMultiPlayerStart()
// FUNCTION: CARM95 0x004b27a7
int DoMultiPlayerStart(void) {
    tJoin_or_host_result result;
    tNet_game_details* game_to_join;
    char s[256];
    char* s2;
    tNet_game_type new_game_type;
    tNet_game_options new_game_options;
    int start_rank;
    int car_index;
    int race_index; // added by dethrace

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        SuspendPendingFlic();
        DoFeatureUnavailableInDemo();
        return 0;
    }

    if (gAusterity_mode) {
        NetFullScreenMessage(kMiscString_NOT_ENOUGH_MEMORY, 0);
        return 0;
    }
    if (NetInitialise()) {
        SuspendPendingFlic();
        DoErrorInterface(kMiscString_NetworkPlayIsNotAvailable);
        return 0;
    }
    gSynch_race_start = 0;
    gPending_race = -1;
    gCurrent_race.number_of_racers = 0;
    gAsk_time = 0;
    AboutToLoadFirstCar();
    result = JoinOrHostGame(&game_to_join);
    switch (result) {
    case eJoin_or_host_cancel:
        return 0;
    case eJoin_or_host_join:
        gProgram_state.frank_or_anniness = eFrankie;
        if (!CarmaCDinDriveOrFullGameInstalled()) {
            DoErrorInterface(kMiscString_PLEASE_INSERT_THE_CARMAGEDDON_CD);
            return 0;
        }
        car_index = -1;
        // Abusing 'start_rank' here, it's probably better to introduce a new variable name (e.g. join_result)
        start_rank = -4;
        while (start_rank == -4) {
            start_rank = 0;
            if (!ChooseNetCar(game_to_join, &game_to_join->options, &car_index, 0)) {
                continue;
            }
            StartLoadingScreen();
            InitNetStorageSpace();
            start_rank = NetJoinGame(game_to_join, gProgram_state.player_name[0], car_index);
            if (start_rank == 0) {
                LoadRaces(gRace_list, &gNumber_of_races, gCurrent_net_game->type);
                SetUpOtherNetThings(game_to_join);
                ReenableNetService();
                strcpy(s, gProgram_state.player_name[0]);
                strcat(s, " ");
                strcat(s, GetMiscString(kMiscString_HasJoinedTheGame));
                NetSendHeadupToAllPlayers(s);
                start_rank = 1;
            } else {
                DisposeNetStorageSpace();
            }
        }
        if (start_rank <= 0) {
            NetDisposeGameDetails(game_to_join);
            return 0;
        } else {
            return 1;
        }
    case eJoin_or_host_host:
#if defined(DETHRACE_FIX_BUGS)
        /* Don't allow hosting a game when the game is launched with --no-bind */
        if (harness_game_config.no_bind) {
            DoErrorInterface(76);
            return 0;
        }
#endif
        gProgram_state.frank_or_anniness = eFrankie;
        if (!OriginalCarmaCDinDrive()) {
            DoErrorInterface(kMiscString_PLEASE_INSERT_THE_CARMAGEDDON_CD);
            return 0;
        }
        car_index = -1;
        ReadNetGameChoices(&new_game_type, &new_game_options, &start_rank);
        LoadRaces(gRace_list, &gNumber_of_races, new_game_type);
        start_rank = PickNetRace(-1, new_game_options.race_sequence_type);
        if (NetGameChoices(&new_game_type, &new_game_options, &start_rank) != 0
            && ChooseNetCar(gCurrent_net_game, &new_game_options, &car_index, 1) != 0) {
            StartLoadingScreen();
            InitNetStorageSpace();
            if (NetHostGame(new_game_type, &new_game_options, start_rank, gProgram_state.player_name[0], car_index) != NULL) {
                SetUpOtherNetThings(gCurrent_net_game);
                ReenableNetService();
                return 1;
            } else {
                DisposeNetStorageSpace();
                ReenableNetService();
                NetLeaveGame(gCurrent_net_game);
                return 0;
            }
        }
    default:
        return 0;
    }
}
