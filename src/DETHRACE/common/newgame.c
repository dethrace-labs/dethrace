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
#include "harness/trace.h"
#include "init.h"
#include "input.h"
#include "intrface.h"
#include "loading.h"
#include "structur.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>
#include <string.h>

tU8* gFrank_flic_data;
tU8* gAnne_flic_data;
int gNet_storage_allocated;
tRadio_bastards gRadio_bastards__newgame[11]; // suffix added to avoid duplicate symbol
char _name1[] = "xxxxxxxx.TXT";               // keep compiler happy
char _name2[] = "yyyyyyyy.TXT";               // keep compiler happy
char* gBasic_car_names[2] = { _name1, _name2 };
tNet_game_options gNet_settings[8];
tJoinable_game gGames_to_join[6];
tNet_game_options* gOptions;
int gNet_target[7];
int gLast_graph_sel__newgame;                   // suffix added to avoid duplicate symbol
tInterface_spec* gThe_interface_spec__newgame;  // suffix added to avoid duplicate symbol
tNet_sequence_type gNet_race_sequence__newgame; // suffix added to avoid duplicate symbol
tNet_game_type gLast_game_type;
int gCurrent_net_game_count;
tU32 gAnne_flic_data_length;
int gShifted_default_yet;
char* gNet_name;
tU32 gFrank_flic_data_length;
int gLast_net_choose_box;
int gCurrent_game_selection;
int gRace_index;
int gRadio_selected;

// IDA: void __cdecl StartRollingPlayerNamesIn()
void StartRollingPlayerNamesIn(void) {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < 2; i++) {
        SetSlotXY(i, gCurrent_graf_data->player_name_x[i], gCurrent_graf_data->player_name_y);
        AddRollingString(gProgram_state.player_name[i], gCurrent_graf_data->player_name_x[i], gCurrent_graf_data->player_name_y, eRT_alpha);
    }
}

// IDA: void __cdecl FrankAnneStart1()
void FrankAnneStart1(void) {
    LOG_TRACE("()");
    StartRollingPlayerNamesIn();
    if (!gFrank_flic_data) {
        if (!LoadFlicData("FRANK.FLI", &gFrank_flic_data, &gFrank_flic_data_length)) {
            FatalError(kFatalError_LoadOpponentMugShotFile);
        }
    }
    if (!gAnne_flic_data) {
        if (!LoadFlicData("ANNIE.FLI", &gAnne_flic_data, &gAnne_flic_data_length)) {
            FatalError(kFatalError_LoadOpponentMugShotFile);
        }
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
void FrankAnneStart2(void) {
    LOG_TRACE("()");
    ChangePanelFlic(0, gFrank_flic_data, gFrank_flic_data_length);
    ChangePanelFlic(1, gAnne_flic_data, gAnne_flic_data_length);
    TellyInImage(GetPanelPixelmap(0), gCurrent_graf_data->frank_panel_left, gCurrent_graf_data->frank_panel_top);
    TellyInImage(GetPanelPixelmap(1), gCurrent_graf_data->anne_panel_left, gCurrent_graf_data->anne_panel_top);
}

// IDA: void __usercall GetPlayerName(int pStarting_to_type@<EAX>, int pCurrent_choice@<EDX>, char *pString@<EBX>, int *pMax_length@<ECX>)
void GetPlayerName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {
    LOG_TRACE("(%d, %d, \"%s\", %p)", pStarting_to_type, pCurrent_choice, pString, pMax_length);
    strcpy(pString, gProgram_state.player_name[pCurrent_choice]);
    *pMax_length = PLAYER_NAME_MAX_LENGTH;
}

// IDA: int __usercall FrankAnneDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int FrankAnneDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
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
void FrankAnneDraw(int pCurrent_choice, int pCurrent_mode) {
    LOG_TRACE9("(%d, %d)", pCurrent_choice, pCurrent_mode);
    if (gTyping) {
        if (GetTotalTime() & 0x100) {
            if (pCurrent_choice < 2) {
                TransDRPixelmapText(
                    gBack_screen,
                    gCurrent_graf_data->enter_name_x[pCurrent_choice],
                    gCurrent_graf_data->enter_name_y,
                    &gFonts[FONT_GRNLIT],
                    GetMiscString(191),
                    gBack_screen->width);
            }
        }
    }
}

// IDA: int __cdecl FrankieOrAnnie()
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
    LOG_TRACE("()");

    LoadFont(FONT_GRNLIT);
    result = DoInterfaceScreen(&interface_spec, 0, gProgram_state.frank_or_anniness);
    DisposeFlicPanel(1);
    DisposeFlicPanel(0);
    DisposeFont(FONT_GRNLIT);
    return result < 2;
}

// IDA: int __cdecl SelectSkillLevel()
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
    LOG_TRACE("()");

    result = DoInterfaceScreen(&interface_spec, 0, gProgram_state.skill_level);
    if (result > 2) {
        return 0;
    }
    gProgram_state.skill_level = result;
    return 1;
}
#include "harness/config.h"
#include "harness/hooks.h"

// IDA: int __cdecl DoOnePlayerStart()
int DoOnePlayerStart(void) {
    int merrily_looping;
    tProgram_state saved_state;
    LOG_TRACE("()");

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
                gNet_mode = 0;
                InitGame(0);
                merrily_looping = 0;
            }
        } while (merrily_looping);
        UnlockBunchOfFlics(4);
        return 1;
    } else {
        DoErrorInterface(223);
        return 0;
    }
}

// IDA: int __usercall NewNetGameUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NewNetGameUp(int* pCurrent_choice, int* pCurrent_mode) {
    int new_sel;
    int i;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NewNetGameDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NewNetGameDown(int* pCurrent_choice, int* pCurrent_mode) {
    int new_sel;
    int i;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeJoinableGame(int pIndex@<EAX>)
void DisposeJoinableGame(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawAnItem(int pX@<EAX>, int pY_index@<EDX>, int pFont_index@<EBX>, char *pText@<ECX>)
void DrawAnItem__newgame(int pX, int pY_index, int pFont_index, char* pText) {
    LOG_TRACE("(%d, %d, %d, \"%s\")", pX, pY_index, pFont_index, pText);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawColumnHeading(int pStr_index@<EAX>, int pX@<EDX>)
void DrawColumnHeading__newgame(int pStr_index, int pX) {
    LOG_TRACE("(%d, %d)", pStr_index, pX);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawGames(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawGames(int pCurrent_choice, int pCurrent_mode) {
    int i;
    int font_index;
    int current_index;
    int x_coord;
    int y_coord;
    char s[256];
    char* s2;
    char* s3;
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitGamesToJoin()
void InitGamesToJoin(void) {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeJoinList(int pExemption@<EAX>)
void DisposeJoinList(int pExemption) {
    int i;
    LOG_TRACE("(%d)", pExemption);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddToJoinList(tNet_game_details *pGame@<EAX>)
void AddToJoinList(tNet_game_details* pGame) {
    int i;
    int slot_to_use;
    tU32 this_game_ID;
    LOG_TRACE("(%p)", pGame);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewNetStart1()
void NewNetStart1(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NewNetStart2()
void NewNetStart2(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NewNetGetName(int pStarting_to_type@<EAX>, int pCurrent_choice@<EDX>, char *pString@<EBX>, int *pMax_length@<ECX>)
void NewNetGetName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {
    LOG_TRACE("(%d, %d, \"%s\", %p)", pStarting_to_type, pCurrent_choice, pString, pMax_length);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NewNetDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int NewNetDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NewNetGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NewNetGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: tJoin_or_host_result __usercall JoinOrHostGame@<EAX>(tNet_game_details **pGame_to_join@<EAX>)
tJoin_or_host_result JoinOrHostGame(tNet_game_details** pGame_to_join) {
    static tFlicette flicker_on[2];
    static tFlicette flicker_off[2];
    static tFlicette push[2];
    static tMouse_area mouse_areas[3];
    static tRectile recopy_areas[1];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("(%p)", pGame_to_join);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetNetOptions(tNet_game_options *pGame_options@<EAX>)
void GetNetOptions(tNet_game_options* pGame_options) {
    LOG_TRACE("(%p)", pGame_options);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetNetOptions(tNet_game_options *pGame_options@<EAX>)
void SetNetOptions(tNet_game_options* pGame_options) {
    LOG_TRACE("(%p)", pGame_options);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayCheckboxOn2(int pIndex@<EAX>)
void NetPlayCheckboxOn2(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayCheckboxOff2(int pIndex@<EAX>)
void NetPlayCheckboxOff2(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayCheckboxOn(int pIndex@<EAX>)
void NetPlayCheckboxOn(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayCheckboxOff(int pIndex@<EAX>)
void NetPlayCheckboxOff(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayRadioOn2(int pIndex@<EAX>, int pValue@<EDX>)
void NetPlayRadioOn2(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayRadioOff2(int pIndex@<EAX>, int pValue@<EDX>)
void NetPlayRadioOff2(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayRadioOn(int pIndex@<EAX>, int pValue@<EDX>)
void NetPlayRadioOn(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayRadioOff(int pIndex@<EAX>, int pValue@<EDX>)
void NetPlayRadioOff(int pIndex, int pValue) {
    LOG_TRACE("(%d, %d)", pIndex, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DrawNOptInitialRadios()
void DrawNOptInitialRadios(void) {
    int i;
    int j;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetRadioChanged(int pIndex@<EAX>, int pNew_value@<EDX>)
void NetRadioChanged(int pIndex, int pNew_value) {
    LOG_TRACE("(%d, %d)", pIndex, pNew_value);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetCheckboxChanged(int pIndex@<EAX>)
void NetCheckboxChanged(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetOptLeft@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NetOptLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetOptRight@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NetOptRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetOptUp@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NetOptUp(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetOptDown@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NetOptDown(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetRadioClick@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>, int pX_offset@<EBX>, int pY_offset@<ECX>)
int NetRadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    LOG_TRACE("(%p, %p, %d, %d)", pCurrent_choice, pCurrent_mode, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RevertToDefaults()
void RevertToDefaults(void) {
    tPath_name the_path;
    FILE* f;
    tNet_game_options net_options;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DefaultNetSettings()
void DefaultNetSettings(void) {
    FILE* f;
    int i;
    STUB();
}

// IDA: int __usercall NetOptGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NetOptGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlotAGraphBox(int pIndex@<EAX>, int pColour_value@<EDX>)
void NetPlotAGraphBox(int pIndex, int pColour_value) {
    LOG_TRACE("(%d, %d)", pIndex, pColour_value);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetDrawAGraphBox(int pIndex@<EAX>)
void NetDrawAGraphBox(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetEraseAGraphBox(int pIndex@<EAX>)
void NetEraseAGraphBox(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawNetOptBox(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawNetOptBox(int pCurrent_choice, int pCurrent_mode) {
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoNetOptions(tNet_game_options *pGame_options@<EAX>)
void DoNetOptions(tNet_game_options* pGame_options) {
    static tFlicette flicker_on[14];
    static tFlicette flicker_off[14];
    static tFlicette push[14];
    static tMouse_area mouse_areas[14];
    static tInterface_spec interface_spec;
    LOG_TRACE("(%p)", pGame_options);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayRadioOn(int pIndex@<EAX>)
//  Suffix added to avoid duplicate symbol
void PlayRadioOn__newgame(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayRadioOff(int pIndex@<EAX>)
//  Suffix added to avoid duplicate symbol
void PlayRadioOff__newgame(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetOptions(tNet_game_type pGame_type@<EAX>, tNet_game_options *pGame_options@<EDX>)
void SetOptions(tNet_game_type pGame_type, tNet_game_options* pGame_options) {
    LOG_TRACE("(%d, %p)", pGame_type, pGame_options);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DrawNetChooseInitial()
void DrawNetChooseInitial(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetChooseGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NetChooseGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlotAGraphBox(int pIndex@<EAX>, int pColour_value@<EDX>)
// Suffix added to avoid duplicate symbol
void PlotAGraphBox__newgame(int pIndex, int pColour_value) {
    LOG_TRACE("(%d, %d)", pIndex, pColour_value);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawAGraphBox(int pIndex@<EAX>)
// Suffix added to avoid duplicate symbol
void DrawAGraphBox__newgame(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall EraseAGraphBox(int pIndex@<EAX>)
// Suffix added to avoid duplicate symbol
void EraseAGraphBox__newgame(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawNetChoose(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>)
void DrawNetChoose(int pCurrent_choice, int pCurrent_mode) {
    char s[256];
    tU32* k;
    int i;
    LOG_TRACE("(%d, %d)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetChooseLR@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int NetChooseLR(int* pCurrent_choice, int* pCurrent_mode) {
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetGameTarget(tNet_game_type *pGame_type@<EAX>, tNet_game_options *pGame_options@<EDX>)
void SetGameTarget(tNet_game_type* pGame_type, tNet_game_options* pGame_options) {
    LOG_TRACE("(%p, %p)", pGame_type, pGame_options);

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
int NetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index) {
    static tFlicette flicker_on[11];
    static tFlicette flicker_off[11];
    static tFlicette push[11];
    static tMouse_area mouse_areas[11];
    static tInterface_spec interface_spec;
    int result;
    LOG_TRACE("(%p, %p, %p)", pGame_type, pGame_options, pRace_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReadNetGameChoices(tNet_game_type *pGame_type@<EAX>, tNet_game_options *pGame_options@<EDX>, int *pRace_index@<EBX>)
void ReadNetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index) {
    LOG_TRACE("(%p, %p, %p)", pGame_type, pGame_options, pRace_index);

    *pGame_type = gLast_game_type;
    SetOptions(*pGame_type, pGame_options);
    SetGameTarget(pGame_type, pGame_options);
    *pRace_index = PickNetRace(-1, pGame_options->race_sequence_type);
}

// IDA: int __usercall ChooseStartRace@<EAX>(int *pRank@<EAX>)
int ChooseStartRace(int* pRank) {
    LOG_TRACE("(%p)", pRank);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetUpOtherNetThings(tNet_game_details *pNet_game@<EAX>)
void SetUpOtherNetThings(tNet_game_details* pNet_game) {
    LOG_TRACE("(%p)", pNet_game);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RequestCarDetails(tNet_game_details *pNet_game@<EAX>)
void RequestCarDetails(tNet_game_details* pNet_game) {
    tNet_message* message;
    LOG_TRACE("(%p)", pNet_game);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PickARandomCar()
int PickARandomCar(void) {
    int i;
    int cars[120];
    int array_size;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PollCarDetails(tNet_game_details *pNet_game@<EAX>)
void PollCarDetails(tNet_game_details* pNet_game) {
    LOG_TRACE("(%p)", pNet_game);

    if (gReceived_car_details != 0) {
        RequestCarDetails(pNet_game);
    }
}

// IDA: void __usercall SetNetAvailability(tNet_game_options *pOptions@<EAX>)
void SetNetAvailability(tNet_game_options* pOptions) {
    int i;
    LOG_TRACE("(%p)", pOptions);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ChooseNetCar@<EAX>(tNet_game_details *pNet_game@<EAX>, tNet_game_options *pOptions@<EDX>, int *pCar_index@<EBX>, int pIm_the_host_so_fuck_off@<ECX>)
int ChooseNetCar(tNet_game_details* pNet_game, tNet_game_options* pOptions, int* pCar_index, int pIm_the_host_so_fuck_off) {
    tS32 start_time;
    int i;
    int result;
    int car_index;
    int the_car_index;
    LOG_TRACE("(%p, %p, %p, %d)", pNet_game, pOptions, pCar_index, pIm_the_host_so_fuck_off);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitNetStorageSpace()
void InitNetStorageSpace(void) {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < 6; i++) {
        gCurrent_race.opponent_list[i].car_spec = NULL;
    }
    gNet_storage_allocated = 1;
    InitialiseStorageSpace(&gNet_cars_storage_space, 500, 50, 500, 300);
}

// IDA: void __cdecl DisposeNetStorageSpace()
void DisposeNetStorageSpace(void) {
    LOG_TRACE("()");

    if (gNet_storage_allocated) {
        DisposeStorageSpace(&gNet_cars_storage_space);
    }
    gNet_storage_allocated = 0;
}

// IDA: int __cdecl DoMultiPlayerStart()
int DoMultiPlayerStart(void) {
    tJoin_or_host_result result;
    tNet_game_details* game_to_join;
    char s[256];
    char* s2;
    tNet_game_type new_game_type;
    tNet_game_options new_game_options;
    int start_rank;
    int car_index;
    LOG_TRACE("()");

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        SuspendPendingFlic();
        DoFeatureUnavailableInDemo();
        return 0;
    }
    NOT_IMPLEMENTED();
}
