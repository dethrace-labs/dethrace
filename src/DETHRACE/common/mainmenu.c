#include "mainmenu.h"
#include "brender/brender.h"
#include "controls.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "intrface.h"
#include "loading.h"
#include "loadsave.h"
#include "network.h"
#include "newgame.h"
#include "options.h"
#include "pd/sys.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"
#include <stdlib.h>

char* gPalette_copy__mainmenu;    // suffix added to avoid duplicate symbol
int gPixel_buffer_size__mainmenu; // suffix added to avoid duplicate symbol
tInterface_spec* gMain_menu_spec;
int gMouse_was_started__mainmenu; // suffix added to avoid duplicate symbol
int gReplace_background;
char* gPixels_copy__mainmenu; // suffix added to avoid duplicate symbol

// IDA: int __usercall MainMenuDone1@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int MainMenuDone1(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        if (pCurrent_mode == 0) {
            if (pCurrent_choice == 7) {
                PreloadBunchOfFlics(7);
            }
        } else {
            return -1;
        }
        return pCurrent_choice;
    }

    if (pTimed_out) {
        return -1;
    }
    switch (pCurrent_choice) {
    case 1:
        PreloadBunchOfFlics(4);
        break;
    case 2:
        PreloadBunchOfFlics(5);
        break;
    case 3:
        PreloadBunchOfFlics(1);
        break;
    case 4:
        PreloadBunchOfFlics(3);
        break;
    case 5:
        PreloadBunchOfFlics(2);
        break;
    case 7:
        PreloadBunchOfFlics(7);
        break;
    default:
        return pCurrent_choice;
    }
    return pCurrent_choice;
}

// IDA: int __usercall MainMenuDone2@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int MainMenuDone2(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        if (pCurrent_mode == 0) {
            if (pCurrent_choice == 4) {
                PreloadBunchOfFlics(7);
            }
        } else {
            return -1;
        }
        return pCurrent_choice;
    }

    if (pTimed_out) {
        return -1;
    }
    switch (pCurrent_choice) {
    case 0:
        PreloadBunchOfFlics(4);
        break;
    case 1:
        PreloadBunchOfFlics(5);
        break;
    case 2:
        PreloadBunchOfFlics(1);
        break;
    case 3:
        PreloadBunchOfFlics(3);
        break;
    case 4:
        PreloadBunchOfFlics(7);
        break;
    }
    return pCurrent_choice;
}

// IDA: void __cdecl StartMainMenu()
void StartMainMenu(void) {
    LOG_TRACE("()");

    if (gFaded_palette) {
        PlayFlicsInDarkness();
        RunFlic(10);
    } else {
        RunFlic(11);
    }
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    if (!gProgram_state.racing) {
        DisableChoice(1);
        RunFlicAt(
            35,
            gMain_menu_spec->flicker_on_flics[1].x[gGraf_data_index],
            gMain_menu_spec->flicker_on_flics[1].y[gGraf_data_index]);
    }
    if (gDisallow_abandon_race) {
        DisableChoice(2);
        RunFlicAt(
            36,
            gMain_menu_spec->flicker_on_flics[2].x[gGraf_data_index],
            gMain_menu_spec->flicker_on_flics[2].y[gGraf_data_index]);
    }
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
    ReilluminateFlics();
}

// IDA: int __usercall DoMainMenuInterface@<EAX>(tU32 pTime_out@<EAX>, int pContinue_allowed@<EDX>)
int DoMainMenuInterface(tU32 pTime_out, int pContinue_allowed) {
    static tFlicette flicker_on1[8] = {
        { 14, { 64, 128 }, { 37, 89 } },
        { 25, { 59, 118 }, { 54, 130 } },
        { 27, { 55, 110 }, { 71, 170 } },
        { 29, { 52, 104 }, { 88, 211 } },
        { 23, { 53, 106 }, { 105, 252 } },
        { 21, { 54, 108 }, { 122, 293 } },
        { 17, { 58, 116 }, { 139, 334 } },
        { 19, { 64, 128 }, { 155, 372 } }
    };
    static tFlicette flicker_off1[8] = {
        { 13, { 64, 128 }, { 37, 89 } },
        { 24, { 59, 118 }, { 54, 130 } },
        { 26, { 55, 110 }, { 71, 170 } },
        { 28, { 52, 104 }, { 88, 211 } },
        { 22, { 53, 106 }, { 105, 252 } },
        { 20, { 54, 108 }, { 122, 293 } },
        { 16, { 58, 116 }, { 139, 334 } },
        { 18, { 64, 128 }, { 155, 372 } }
    };

    static tFlicette push1[8] = {
        { 15, { 63, 126 }, { 36, 86 } },
        { 15, { 58, 116 }, { 53, 127 } },
        { 15, { 54, 108 }, { 70, 168 } },
        { 15, { 51, 102 }, { 87, 209 } },
        { 15, { 52, 104 }, { 104, 250 } },
        { 15, { 53, 106 }, { 121, 290 } },
        { 15, { 57, 114 }, { 138, 331 } },
        { 15, { 63, 126 }, { 154, 370 } }
    };

    static tMouse_area mouse_areas1[8] = {
        { { 64, 128 }, { 37, 89 }, { 265, 530 }, { 47, 113 }, 0, 0, 0, NULL },
        { { 59, 118 }, { 54, 130 }, { 265, 530 }, { 64, 154 }, 1, 0, 0, NULL },
        { { 55, 110 }, { 71, 170 }, { 265, 530 }, { 81, 194 }, 2, 0, 0, NULL },
        { { 52, 104 }, { 88, 211 }, { 265, 530 }, { 98, 235 }, 3, 0, 0, NULL },
        { { 53, 106 }, { 105, 252 }, { 265, 530 }, { 115, 276 }, 4, 0, 0, NULL },
        { { 54, 108 }, { 122, 293 }, { 265, 530 }, { 132, 317 }, 5, 0, 0, NULL },
        { { 58, 116 }, { 139, 334 }, { 265, 530 }, { 149, 358 }, 6, 0, 0, NULL },
        { { 64, 128 }, { 155, 372 }, { 265, 530 }, { 165, 396 }, 7, 0, 0, NULL }
    };
    static tInterface_spec interface_spec1 = {
        0,                      // initial_imode
        0,                      // first_opening_flic
        0,                      // second_opening_flic
        0,                      // end_flic_go_ahead
        0,                      // end_flic_escaped
        0,                      // end_flic_otherwise
        0,                      // flic_bunch_to_load
        { -1, 0 },              // move_left_new_mode
        { 0, 0 },               // move_left_delta
        { 0, 0 },               // move_left_min
        { 0, 0 },               // move_left_max
        { NULL, NULL },         // move_left_proc
        { -1, 0 },              // move_right_new_mode
        { 0, 0 },               // move_right_delta
        { 0, 0 },               // move_right_min
        { 0, 0 },               // move_right_max
        { NULL, NULL },         // move_right_proc
        { -1, 0 },              // move_up_new_mode
        { -1, 0 },              // move_up_delta
        { 0, 0 },               // move_up_min
        { 7, 0 },               // move_up_max
        { NULL, NULL },         // move_up_proc
        { -1, 0 },              // move_down_new_mode
        { 1, 0 },               // move_down_delta
        { 0, 0 },               // move_down_min
        { 7, 0 },               // move_down_max
        { NULL, NULL },         // move_down_proc
        { 1, 1 },               // go_ahead_allowed
        { NULL, NULL },         // go_ahead_proc
        { 1, 1 },               // escape_allowed
        { NULL, NULL },         // escape_proc
        NULL,                   // exit_proc
        NULL,                   // draw_proc
        0u,                     // time_out
        StartMainMenu,          // start_proc1
        NULL,                   // start_proc2
        MainMenuDone1,          // done_proc
        0,                      // font_needed
        { 0, 0 },               // typeable
        NULL,                   // get_original_string
        0,                      // escape_code
        0,                      // dont_save_or_load
        COUNT_OF(flicker_on1),  // number_of_button_flics
        flicker_on1,            // flicker_on_flics
        flicker_off1,           // flicker_off_flics
        push1,                  // pushed_flics
        COUNT_OF(mouse_areas1), // number_of_mouse_areas
        mouse_areas1,           // mouse_areas
        0,                      // number_of_recopy_areas
        NULL                    // recopy_areas
    };

    static tFlicette flicker_on2[5] = {
        { 335, { 58, 116 }, { 51, 122 } },
        { 337, { 53, 106 }, { 74, 178 } },
        { 339, { 51, 102 }, { 96, 230 } },
        { 333, { 52, 104 }, { 119, 286 } },
        { 331, { 58, 116 }, { 142, 341 } }
    };
    static tFlicette flicker_off2[5] = {
        { 334, { 58, 116 }, { 51, 122 } },
        { 336, { 53, 106 }, { 74, 178 } },
        { 338, { 51, 102 }, { 96, 230 } },
        { 332, { 52, 104 }, { 119, 286 } },
        { 330, { 58, 116 }, { 142, 341 } }
    };
    static tFlicette push2[5] = {
        { 15, { 56, 112 }, { 50, 120 } },
        { 15, { 51, 102 }, { 73, 175 } },
        { 15, { 50, 100 }, { 95, 228 } },
        { 15, { 51, 102 }, { 118, 283 } },
        { 15, { 57, 114 }, { 141, 338 } }
    };
    static tMouse_area mouse_areas2[5] = {
        { { 57, 114 }, { 51, 122 }, { 265, 530 }, { 61, 146 }, 0, 0, 0, NULL },
        { { 52, 104 }, { 74, 178 }, { 265, 530 }, { 84, 202 }, 1, 0, 0, NULL },
        { { 51, 102 }, { 96, 230 }, { 265, 530 }, { 106, 254 }, 2, 0, 0, NULL },
        { { 52, 104 }, { 119, 286 }, { 265, 530 }, { 129, 310 }, 3, 0, 0, NULL },
        { { 58, 116 }, { 142, 341 }, { 265, 530 }, { 152, 365 }, 4, 0, 0, NULL }
    };
    static tInterface_spec interface_spec2 = {
        0,                      // initial_imode
        31,                     // first_opening_flic
        30,                     // second_opening_flic
        0,                      // end_flic_go_ahead
        0,                      // end_flic_escaped
        0,                      // end_flic_otherwise
        0,                      // flic_bunch_to_load
        { -1, 0 },              // move_left_new_mode
        { 0, 0 },               // move_left_delta
        { 0, 0 },               // move_left_min
        { 0, 0 },               // move_left_max
        { NULL, NULL },         // move_left_proc
        { -1, 0 },              // move_right_new_mode
        { 0, 0 },               // move_right_delta
        { 0, 0 },               // move_right_min
        { 0, 0 },               // move_right_max
        { NULL, NULL },         // move_right_proc
        { -1, 0 },              // move_up_new_mode
        { -1, 0 },              // move_up_delta
        { 0, 0 },               // move_up_min
        { 4, 0 },               // move_up_max
        { NULL, NULL },         // move_up_proc
        { -1, 0 },              // move_down_new_mode
        { 1, 0 },               // move_down_delta
        { 0, 0 },               // move_down_min
        { 4, 0 },               // move_down_max
        { NULL, NULL },         // move_down_proc
        { 1, 1 },               // go_ahead_allowed
        { NULL, NULL },         // go_ahead_proc
        { 1, 1 },               // escape_allowed
        { NULL, NULL },         // escape_proc
        NULL,                   // exit_proc
        NULL,                   // draw_proc
        0u,                     // time_out
        NULL,                   // start_proc1
        NULL,                   // start_proc2
        &MainMenuDone2,         // done_proc
        0,                      // font_needed
        { 0, 0 },               // typeable
        NULL,                   // get_original_string
        4,                      // escape_code
        0,                      // dont_save_or_load
        COUNT_OF(flicker_on2),  // number_of_button_flics
        flicker_on2,            // flicker_on_flics
        flicker_off2,           // flicker_off_flics
        push2,                  // pushed_flics
        COUNT_OF(mouse_areas2), // number_of_mouse_areas
        mouse_areas2,           // mouse_areas
        0,                      // number_of_recopy_areas
        NULL                    // recopy_areas
    };

    int result;
    LOG_TRACE("(%d, %d)", pTime_out, pContinue_allowed);

    if (pContinue_allowed) {
        gMain_menu_spec = &interface_spec1;
        result = DoInterfaceScreen(&interface_spec1, gFaded_palette | 2, 0);
        if (result == 0 || result == 1 || result == 2 || result == 7) {
            FadePaletteDown();
        } else {
            RunFlic(12);
        }
        switch (result) {
        case 0:
            return 0;
        case 1:
            return 8;
        case 2:
            return 9;
        case 3:
            return 6;
        case 4:
            return 3;
        case 5:
            return 2;
        case 6:
            return 1;
        case 7:
            return 7;
        default:
            break;
        }
        result = -1;
    } else {
        interface_spec2.time_out = pTime_out;
        result = DoInterfaceScreen(&interface_spec2, gFaded_palette, 0);
        if (result == -1 || result == 4) {
            FadePaletteDown();
        } else {
            RunFlic(32);
        }
        switch (result) {
        case 0:
            result = 4;
            break;
        case 1:
            result = 5;
            break;
        case 2:
            result = 6;
            break;
        case 3:
            result = 3;
            break;
        case 4:
            result = 7;
            break;
        default:
            result = -1;
            break;
        }
    }
    return result;
}

// IDA: tMM_result __usercall GetMainMenuOption@<EAX>(tU32 pTime_out@<EAX>, int pContinue_allowed@<EDX>)
tMM_result GetMainMenuOption(tU32 pTime_out, int pContinue_allowed) {
    int result;

    result = DoMainMenuInterface(pTime_out, pContinue_allowed);
    if (result < 0) {
        return eMM_timeout;
    }
    if (gProgram_state.prog_status == eProg_game_starting) {
        return eMM_continue;
    }
    switch (result) {
    case 1:
        return eMM_end_game;
    case 2:
        return eMM_save;
    case 3:
        return eMM_loaded;
    case 4:
        return eMM_1_start;
    case 5:
        return eMM_n_start;
    case 6:
        return eMM_options;
    case 7:
        return eMM_quit;
    case 8:
        return eMM_recover;
    case 9:
        return eMM_abort_race;
    default:
        return eMM_continue;
    }
}

// IDA: void __cdecl QuitVerifyStart()
void QuitVerifyStart(void) {
    gPixel_buffer_size__mainmenu = gBack_screen->height * gBack_screen->row_bytes;
    gPixels_copy__mainmenu = BrMemAllocate(gPixel_buffer_size__mainmenu, kMem_quit_vfy_pixels);
    gPalette_copy__mainmenu = BrMemAllocate(0x400u, kMem_quit_vfy_pal);
    memcpy(gPixels_copy__mainmenu, gBack_screen->pixels, gPixel_buffer_size__mainmenu);
    memcpy(gPalette_copy__mainmenu, gCurrent_palette_pixels, 0x400u);
    FadePaletteDown();
}

// IDA: int __usercall QuitVerifyDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int QuitVerifyDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    FadePaletteDown();
    TurnOnPanelFlics();
    if (gMouse_was_started__mainmenu) {
        RemoveTransientBitmaps(1);
    }
    if (gReplace_background) {
        if (pCurrent_choice) {
            memcpy(gBack_screen->pixels, gPixels_copy__mainmenu, gPixel_buffer_size__mainmenu);
            memcpy(gCurrent_palette_pixels, gPalette_copy__mainmenu, 0x400u);
        } else {
            ClearEntireScreen();
        }
        PDScreenBufferSwap(0);
        FadePaletteUp();
    }
    BrMemFree(gPixels_copy__mainmenu);
    BrMemFree(gPalette_copy__mainmenu);
    return pCurrent_choice == 0;
}

// IDA: int __usercall DoVerifyQuit@<EAX>(int pReplace_background@<EAX>)
int DoVerifyQuit(int pReplace_background) {
    static tFlicette flicker_on[2] = { { 43, { 181, 362 }, { 124, 298 } }, { 43, { 84, 168 }, { 124, 298 } } };
    static tFlicette flicker_off[2] = { { 42, { 181, 362 }, { 124, 298 } }, { 42, { 84, 168 }, { 124, 298 } } };
    static tFlicette push[2] = { { 135, { 181, 362 }, { 124, 298 } }, { 45, { 84, 168 }, { 124, 298 } } };
    static tMouse_area mouse_areas[2] = {
        { { 181, 362 }, { 124, 298 }, { 244, 488 }, { 144, 346 }, 0, 0, 0, NULL },
        { { 84, 168 }, { 124, 298 }, { 147, 294 }, { 144, 346 }, 1, 0, 0, NULL }
    };
    static tInterface_spec interface_spec = {
        0,                 // initial_imode
        0,                 // first_opening_flic
        0,                 // second_opening_flic
        0,                 // end_flic_go_ahead
        0,                 // end_flic_escaped
        0,                 // end_flic_otherwise
        0,                 // flic_bunch_to_load
        { 0xFFFFFFFF, 0 }, // move_left_new_mode
        { 0xFFFFFFFF, 0 }, // move_left_delta
        { 0, 0 },          // move_left_min
        { 1, 0 },          // move_left_max
        { NULL, NULL },    // move_left_proc
        { 0xFFFFFFFF, 0 }, // move_right_new_mode
        { 1, 0 },          // move_right_delta
        { 0, 0 },          // move_right_min
        { 1, 0 },          // move_right_max
        { NULL, NULL },    // move_right_proc
        { 0xFFFFFFFF, 0 }, // move_up_new_mode
        { 0, 0 },          // move_up_delta
        { 0, 0 },          // move_up_min
        { 0, 0 },          // move_up_max
        { NULL, NULL },    // move_up_proc
        { 0xFFFFFFFF, 0 }, // move_down_new_mode
        { 0, 0 },          // move_down_delta
        { 0, 0 },          // move_down_min
        { 0, 0 },          // move_down_max
        { NULL, NULL },    // move_down_proc
        { 1, 1 },          // go_ahead_allowed
        { NULL, NULL },    // go_ahead_proc
        { 1, 1 },          // escape_allowed
        { NULL, NULL },    // escape_proc
        NULL,              // exit_proc
        NULL,              // draw_proc
        0u,                // time_out
        QuitVerifyStart,   // start_proc1
        NULL,              // start_proc2
        QuitVerifyDone,    // done_proc
        0,                 // font_needed
        { 0, 0 },          // typeable
        NULL,              // get_original_string
        1,                 // escape_code
        1,                 // dont_save_or_load
        2,                 // number_of_button_flics
        flicker_on,        // flicker_on_flics
        flicker_off,       // flicker_off_flics
        push,              // pushed_flics
        2,                 // number_of_mouse_areas
        mouse_areas,       // mouse_areas
        0,                 // number_of_recopy_areas
        NULL               // recopy_areas
    };
    int result;
    int switched_res;
    int woz_in_race;

    woz_in_race = 0;
    if (gAusterity_mode) {
        return 1;
    }
    if (gProgram_state.racing) {
        FadePaletteDown();
        ClearEntireScreen();
        GoingToInterfaceFromRace();
        woz_in_race = 1;
        gProgram_state.racing = 0;
    }
    result = IRandomBetween(1, 3);
    switch (result) {
    case 1:
        interface_spec.first_opening_flic = 130;
        break;
    case 2:
        interface_spec.first_opening_flic = 131;
        break;
    case 3:
        interface_spec.first_opening_flic = 132;
        break;
    }
    gMouse_was_started__mainmenu = gMouse_started;
    if (gMouse_started) {
        RemoveTransientBitmaps(1);
        EndMouseCursor();
    }
    TurnOffPanelFlics();
    gReplace_background = pReplace_background;

    DRS3StopOutletSound(gEffects_outlet);
    LoadInterfaceStuff(woz_in_race);
    result = DoInterfaceScreen(&interface_spec, 0, 0);
    if (gMouse_was_started__mainmenu) {
        LoadInterfaceStuff(woz_in_race);
        StartMouseCursor();
    }
    if (woz_in_race) {
        GoingBackToRaceFromInterface();
        gProgram_state.racing = 1;
    }
    return result;
}

// IDA: tMM_result __usercall DoMainMenu@<EAX>(tU32 pTime_out@<EAX>, int pSave_allowed@<EDX>, int pContinue_allowed@<EBX>)
tMM_result DoMainMenu(tU32 pTime_out, int pSave_allowed, int pContinue_allowed) {
    tMM_result the_result;
    LOG_TRACE("(%d, %d, %d)", pTime_out, pSave_allowed, pContinue_allowed);

    if (gProgram_state.racing) {
        FadePaletteDown();
    }
    the_result = GetMainMenuOption(pTime_out, pContinue_allowed);
    switch (the_result) {
    case eMM_end_game:
        if (gNet_mode) {
            gProgram_state.prog_status = eProg_idling;
        }
        break;
    case eMM_1_start:
        if (pContinue_allowed || gAusterity_mode) {
            PlayFlicsFromMemory();
        }
        if (!DoOnePlayerStart()) {
            the_result = eMM_none;
        }
        if (pContinue_allowed || gAusterity_mode) {
            PlayFlicsFromDisk();
        }
        break;
    case eMM_n_start:
        if (!DoMultiPlayerStart()) {
            LoadRaces(gRace_list, &gNumber_of_races, -1);
            the_result = eMM_none;
        }
        break;
    case eMM_loaded:
        if (!DoLoadGame()) {
            the_result = eMM_none;
        }
        break;
    case eMM_save:
        DoSaveGame(pSave_allowed);
        the_result = eMM_none;
        break;
    case eMM_options:
        LoadSoundOptionsData();
        DoOptions();
        FreeSoundOptionsData();
        break;
    case eMM_quit:
        if (!DoVerifyQuit(0)) {
            the_result = eMM_none;
        }
        break;
    case eMM_recover:
        SetRecovery();
        break;
    case eMM_abort_race:
        gAbandon_game = 1;
        break;
    default:
        return the_result;
    }
    return the_result;
}

// IDA: void __usercall DoMainMenuScreen(tU32 pTime_out@<EAX>, int pSave_allowed@<EDX>, int pContinue_allowed@<EBX>)
void DoMainMenuScreen(tU32 pTime_out, int pSave_allowed, int pContinue_allowed) {
    tPlayer_status old_status;
    LOG_TRACE("(%d, %d, %d)", pTime_out, pSave_allowed, pContinue_allowed);

    if (pContinue_allowed || gAusterity_mode) {
        PlayFlicsFromDisk();
    }
    old_status = gNet_players[gThis_net_player_index].player_status;
    NetPlayerStatusChanged(ePlayer_status_main_menu);
    PreloadBunchOfFlics(0);
    switch (DoMainMenu(pTime_out, pSave_allowed, pContinue_allowed)) {
    case eMM_end_game:
        gProgram_state.prog_status = eProg_idling;
        break;
    case eMM_1_start:
    case eMM_n_start:
        gProgram_state.prog_status = eProg_game_starting;
        break;
    case eMM_loaded:
        if (gGame_to_load < 0) {
            gProgram_state.prog_status = eProg_game_starting;
        }
        break;
    case eMM_quit:
        gProgram_state.prog_status = eProg_quit;
        break;
    case eMM_timeout:
        gProgram_state.prog_status = eProg_demo;
        break;
    default:
        break;
    }
    UnlockBunchOfFlics(0);
    if (gNet_players[gThis_net_player_index].player_status == ePlayer_status_main_menu) {
        NetPlayerStatusChanged(old_status);
    }
    if (pContinue_allowed || gAusterity_mode) {
        PlayFlicsFromMemory();
    }
}
