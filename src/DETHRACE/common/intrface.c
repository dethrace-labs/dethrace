#include "intrface.h"
#include "brender.h"
#include "flicplay.h"
#include "globvars.h"
#include "grafdata.h"
#include "graphics.h"
#include "input.h"
#include "loading.h"
#include "main.h"
#include "pc-dos/dossys.h"
#include "sound.h"
#include <stdlib.h>

int gDisabled_choices[10];
int gCurrent_mode;
tU32 gStart_time;
int gCurrent_choice;
tInterface_spec* gSpec;
int gAlways_typing;
int gDisabled_count;

// Offset: 0
// Size: 44
void SetAlwaysTyping() {
    NOT_IMPLEMENTED();
}

// Offset: 44
// Size: 44
void ClearAlwaysTyping() {
    NOT_IMPLEMENTED();
}

// Offset: 88
// Size: 103
// EAX: pChoice
int ChoiceDisabled(int pChoice) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 192
// Size: 44
void ResetInterfaceTimeout() {
    NOT_IMPLEMENTED();
}

// Offset: 236
// Size: 627
// EAX: pSpec
// EDX: pOld_selection
// EBX: pNew_selection
// ECX: pMode
void ChangeSelection(tInterface_spec* pSpec, int* pOld_selection, int* pNew_selection, int pMode, int pSkip_disabled) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 864
// Size: 292
// EAX: pSpec
// EDX: pCopy_areas
void RecopyAreas(tInterface_spec* pSpec, br_pixelmap** pCopy_areas) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 1156
// Size: 111
// EAX: pChoice
void DisableChoice(int pChoice) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 1268
// Size: 140
// EAX: pChoice
void EnableChoice(int pChoice) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 1408
// Size: 5354
// EAX: pSpec
// EDX: pOptions
// EBX: pCurrent_choice
int DoInterfaceScreen(tInterface_spec* pSpec, int pOptions, int pCurrent_choice) {
    tProg_status entry_status; //
    int x_coord; //
    int y_coord; //
    int mouse_in_somewhere;
    int i; //
    int key2;
    int mouse_was_started;
    int last_choice; //
    int escaped; //
    int timed_out; //
    int go_ahead; //
    int last_mode;
    int result; //
    int the_key; //
    int the_max;
    int mouse_down; //
    int new_mouse_down; //
    int last_mouse_down;
    int defeat_mode_change; //
    int selection_changed; //
    char the_str[256];
    tU32 last_press; //
    tU32 last_left_press;
    tU32 last_right_press;
    tU32 last_up_press;
    tU32 last_down_press;
    br_pixelmap** copy_areas; //
    br_pixelmap* old_current_splash; //
    void* pixels_copy;
    void* palette_copy;
    LOG_TRACE("(%p, %d, %d)", pSpec, pOptions, pCurrent_choice);

    //added
    int last_choice_2;

    entry_status = gProgram_state.prog_status;
    gTyping_slot = -1;
    EdgeTriggerModeOn();
    gSpec = pSpec;
    gDisabled_count = 0;
    LoadInterfaceStuff(gProgram_state.racing);
    gProgram_state.dont_save_or_load = pSpec->dont_save_or_load;
    for (i = 0; i < pSpec->number_of_button_flics; i++) {
        LoadFlic(pSpec->flicker_on_flics[i].flic_index);
        LoadFlic(pSpec->flicker_off_flics[i].flic_index);
        LoadFlic(pSpec->pushed_flics[i].flic_index);
    }

    InitFlicQueue();
    gStart_time = PDGetTotalTime();
    gCurrent_choice = pCurrent_choice;
    gCurrent_mode = pSpec->initial_imode;
    StartMouseCursor();
    if (pSpec->font_needed) {
        InitRollingLetters();
        LoadFont(0);
    }
    old_current_splash = gCurrent_splash;
    KillSplashScreen();
    if (pSpec->start_proc1) {
        pSpec->start_proc1();
    }
    if (!gFaded_palette && !old_current_splash && !(pOptions & 2)) {
        DRS3StartSound(gIndexed_outlets[0], 3006);
    }
    if (pOptions & 1) {
        if (pSpec->second_opening_flic > 0) {
            RunFlic(pSpec->second_opening_flic);
        }
    } else if (pSpec->first_opening_flic > 0) {
        RunFlic(pSpec->first_opening_flic);
    }

    if (pSpec->start_proc2) {
        pSpec->start_proc2();
    }

    if (pSpec->number_of_recopy_areas) {
        copy_areas = BrMemAllocate(sizeof(void*) * pSpec->number_of_recopy_areas, kMem_intf_copy_areas);
        for (i = 0; i < pSpec->number_of_recopy_areas; i++) {
            copy_areas[i] = BrPixelmapAllocate(BR_MEMORY_PIXELS,
                ((pSpec->recopy_areas[i].right[gGraf_data_index] - pSpec->recopy_areas[i].left[gGraf_data_index]) + 3) & 0xFC,
                pSpec->recopy_areas[i].bottom[gGraf_data_index] - pSpec->recopy_areas[i].top[gGraf_data_index],
                0, 0);
        }
    }

    last_choice = -1;
    ChangeSelection(pSpec, &last_choice, &gCurrent_choice, gCurrent_mode, 0);
    WaitForNoKeys();
    for (i = 0; i < pSpec->number_of_recopy_areas; i++) {
        BrPixelmapRectangleCopy(copy_areas[i], 0, 0, gBack_screen, pSpec->recopy_areas[i].left[gGraf_data_index], pSpec->recopy_areas[i].top[gGraf_data_index],
            pSpec->recopy_areas[i].right[gGraf_data_index] - pSpec->recopy_areas[i].left[gGraf_data_index],
            pSpec->recopy_areas[i].bottom[gGraf_data_index] - pSpec->recopy_areas[i].top[gGraf_data_index]);
    }

    timed_out = -1;
    last_choice = gCurrent_choice;
    do {
        if (last_choice != gCurrent_choice) {
            ChangeSelection(pSpec, &last_choice, &gCurrent_choice, gCurrent_mode, 1);
        }
        last_choice_2 = gCurrent_choice;
        PollKeys();
        EdgeTriggerModeOff();
        the_key = PDAnyKeyDown();
        if (the_key != -1 && the_key != KEY_CAPSLOCK) {
            gMouse_in_use = 0;
            gStart_time = PDGetTotalTime();
        }
        EdgeTriggerModeOn();
        if ((gTyping_slot < 0 || gAlways_typing) && (PDKeyDown(KEY_LEFT) || PDKeyDown(KEY_KP_4) || last_press == KEY_LEFT)) {
            last_press = -1;
            if (pSpec->move_left_delta[gCurrent_mode]) {
                gCurrent_choice += pSpec->move_left_delta[gCurrent_mode];
                if (gCurrent_choice < pSpec->move_left_min[gCurrent_mode]) {
                    gCurrent_choice = pSpec->move_left_max[gCurrent_mode];
                }
                if (gCurrent_choice > pSpec->move_left_max[gCurrent_mode]) {
                    gCurrent_choice = pSpec->move_left_min[gCurrent_mode];
                }
                DRS3StartSound(gIndexed_outlets[0], 3000);
            }
            if (pSpec->move_left_proc[gCurrent_mode]) {
                defeat_mode_change = (pSpec->move_left_proc[gCurrent_mode])(&gCurrent_choice, &gCurrent_mode);
            } else {
                defeat_mode_change = 0;
            }
            if (pSpec->move_left_new_mode[gCurrent_mode] >= 0 && !defeat_mode_change) {
                gCurrent_mode = pSpec->move_left_new_mode[gCurrent_mode];
            }
        }
        if ((gTyping_slot < 0 || gAlways_typing) && (PDKeyDown(KEY_RIGHT) || PDKeyDown(KEY_KP_6) || last_press == KEY_RIGHT)) {
            last_press = -1;
            if (pSpec->move_right_delta[gCurrent_mode]) {
                gCurrent_choice += pSpec->move_right_delta[gCurrent_mode];
                if (gCurrent_choice < pSpec->move_right_min[gCurrent_mode]) {
                    gCurrent_choice = pSpec->move_right_max[gCurrent_mode];
                }
                if (gCurrent_choice > pSpec->move_right_max[gCurrent_mode]) {
                    gCurrent_choice = pSpec->move_right_min[gCurrent_mode];
                }
                DRS3StartSound(gIndexed_outlets[0], 3000);
            }
            if (pSpec->move_right_proc[gCurrent_mode]) {
                defeat_mode_change = (pSpec->move_right_proc[gCurrent_mode])(&gCurrent_choice, &gCurrent_mode);
            } else {
                defeat_mode_change = 0;
            }
            if (pSpec->move_right_new_mode[gCurrent_mode] >= 0 && !defeat_mode_change) {
                gCurrent_mode = pSpec->move_right_new_mode[gCurrent_mode];
            }
        }
        if (PDKeyDown(KEY_UP) || PDKeyDown(KEY_KP_8) || last_press == KEY_UP) {
            last_press = -1;
            if (pSpec->move_up_delta[gCurrent_mode]) {
                gCurrent_choice += pSpec->move_up_delta[gCurrent_mode];
                if (gCurrent_choice < pSpec->move_up_min[gCurrent_mode]) {
                    gCurrent_choice = pSpec->move_up_max[gCurrent_mode];
                }
                if (gCurrent_choice > pSpec->move_up_max[gCurrent_mode]) {
                    gCurrent_choice = pSpec->move_up_min[gCurrent_mode];
                }
                DRS3StartSound(gIndexed_outlets[0], 3000);
            }
            if (pSpec->move_up_proc[gCurrent_mode]) {
                defeat_mode_change = (pSpec->move_up_proc[gCurrent_mode])(&gCurrent_choice, &gCurrent_mode);
            } else {
                defeat_mode_change = 0;
            }
            if (pSpec->move_up_new_mode[gCurrent_mode] >= 0 && !defeat_mode_change) {
                gCurrent_mode = pSpec->move_up_new_mode[gCurrent_mode];
            }
        }
        if (PDKeyDown(KEY_DOWN) || PDKeyDown(KEY_KP_2) || last_press == KEY_DOWN) {
            last_press = -1;
            if (pSpec->move_down_delta[gCurrent_mode]) {
                gCurrent_choice += pSpec->move_down_delta[gCurrent_mode];
                if (gCurrent_choice < pSpec->move_down_min[gCurrent_mode]) {
                    gCurrent_choice = pSpec->move_down_max[gCurrent_mode];
                }
                if (gCurrent_choice > pSpec->move_down_max[gCurrent_mode]) {
                    gCurrent_choice = pSpec->move_down_min[gCurrent_mode];
                }
                DRS3StartSound(gIndexed_outlets[0], 3000);
            }
            if (pSpec->move_down_proc[gCurrent_mode]) {
                defeat_mode_change = (pSpec->move_down_proc[gCurrent_mode])(&gCurrent_choice, &gCurrent_mode);
            } else {
                defeat_mode_change = 0;
            }
            if (pSpec->move_down_new_mode[gCurrent_mode] >= 0 && !defeat_mode_change) {
                gCurrent_mode = pSpec->move_down_new_mode[gCurrent_mode];
            }
        }
        if (gTyping_slot >= 0 && pSpec->typeable[gCurrent_mode] && gTyping_slot != gCurrent_choice && !gAlways_typing) {
            gCurrent_choice = gTyping_slot;
        }
        if (last_choice_2 == gCurrent_choice) {
            selection_changed = 0;
        } else {
            ChangeSelection(pSpec, &last_choice_2, &gCurrent_choice, gCurrent_mode, 1);
            selection_changed = 1;
        }
        timed_out = pSpec->time_out && PDGetTotalTime() >= pSpec->time_out + gStart_time;
        RemoveTransientBitmaps(1);
        RecopyAreas(pSpec, copy_areas);
        if (pSpec->font_needed) {
            RollLettersIn();
        }
        ServicePanelFlics(1);
        if (pSpec->draw_proc) {
            pSpec->draw_proc(gCurrent_choice, gCurrent_mode);
        }
        ProcessFlicQueue(gFrame_period);
        if (DoMouseCursor() || EitherMouseButtonDown()) {
            gStart_time = PDGetTotalTime();
        }
        PDScreenBufferSwap(0);
        if (gMouse_in_use && !selection_changed) {
            GetMousePosition(&x_coord, &y_coord);
            mouse_down = EitherMouseButtonDown();
            last_choice = 0;
            new_mouse_down = mouse_down && !new_mouse_down;
            for (i = 0; i < pSpec->number_of_mouse_areas; i++) {
                if (x_coord >= pSpec->mouse_areas[i].left[gGraf_data_index]
                    && y_coord >= pSpec->mouse_areas[i].top[gGraf_data_index]
                    && x_coord <= pSpec->mouse_areas[i].right[gGraf_data_index]
                    && y_coord <= pSpec->mouse_areas[i].bottom[gGraf_data_index]
                    && (gTyping_slot < 0 || gAlways_typing || pSpec->mouse_areas[i].available_when_typing || gTyping_slot == pSpec->mouse_areas[i].new_choice)) {

                    if (pSpec->mouse_areas[i].new_choice >= 0 && pSpec->mouse_areas[i].new_choice != gCurrent_choice) {
                        gCurrent_choice = pSpec->mouse_areas[i].new_choice;
                        ChangeSelection(pSpec, &last_choice_2, &gCurrent_choice, gCurrent_mode, 0);
                    }
                    if (pSpec->mouse_areas[i].new_mode >= 0) {
                        gCurrent_mode = pSpec->mouse_areas[i].new_mode;
                    }

                    if (pSpec->mouse_areas[i].new_mode >= 0 || pSpec->mouse_areas[i].new_choice >= 0) {
                        last_choice_2 = 1;
                    }
                    if (mouse_down) {
                        if (pSpec->mouse_areas[i].mouse_click) {
                            mouse_down = pSpec->mouse_areas[i].mouse_click(&gCurrent_choice, &gCurrent_mode,
                                x_coord - pSpec->mouse_areas[i].left[gGraf_data_index],
                                y_coord - pSpec->mouse_areas[i].top[gGraf_data_index]);
                        }
                    }
                    break;
                }
            }
        }

        if (PDKeyDown(KEY_RETURN) || PDKeyDown(KEY_KP_ENTER) || (gTyping_slot < 0 && PDKeyDown(KEY_SPACE)) || (gMouse_in_use && last_choice && new_mouse_down)) {
            DRS3StartSound(gIndexed_outlets[0], 3004);
            go_ahead = pSpec->go_ahead_allowed[gCurrent_mode];
            if (pSpec->go_ahead_proc[gCurrent_mode]) {
                go_ahead = (pSpec->go_ahead_proc[gCurrent_mode])(&gCurrent_choice, &gCurrent_mode);
            }
        } else {
            go_ahead = 0;
        }
        if (PDKeyDown(KEY_ESCAPE)) {
            DRS3StartSound(gIndexed_outlets[0], 3005);
            escaped = pSpec->escape_allowed[gCurrent_mode];
            if (pSpec->escape_proc[gCurrent_mode]) {
                escaped = (pSpec->escape_proc[gCurrent_mode])(&gCurrent_choice, &gCurrent_mode);
            }
        } else {
            escaped = 0;
        }
        if (escaped && gTyping_slot >= 0 && !gAlways_typing) {
            LOG_PANIC("not implemented");
            //pSpec->get_original_string(&pVisible_length, gTyping_slot);
            escaped = 0;
            //RevertTyping(gTyping_slot, &v106);
            gTyping = 0;
            gTyping_slot = -1;
        }
        if (go_ahead) {
            if (gCurrent_choice >= 0 && gCurrent_choice < pSpec->number_of_button_flics) {
                //v84 = &pSpec->pushed_flics[gCurrent_choice];
                if (pSpec->pushed_flics[gCurrent_choice].flic_index >= 0) {
                    AddToFlicQueue(pSpec->pushed_flics[gCurrent_choice].flic_index, pSpec->pushed_flics[gCurrent_choice].x[gGraf_data_index], pSpec->pushed_flics[gCurrent_choice].y[gGraf_data_index], 1);
                }
            }
            if (pSpec->typeable[gCurrent_mode]) {
                if (gTyping_slot >= 0 || gAlways_typing) {
                    KillCursor(gCurrent_choice);
                    RecopyAreas(pSpec, copy_areas);
                    RollLettersIn();
                    PDScreenBufferSwap(0);
                } else {
                    LOG_PANIC("not implemented");
                    gTyping_slot = gCurrent_choice;
                    //(pSpec->get_original_string)(&pVisible_length, gCurrent_choice);
                    //StartTyping(gTyping_slot, &v106, pVisible_length);
                    go_ahead = 0;
                    gTyping = 1;
                }
            }
        } else if (gTyping_slot >= 0 && !escaped) {
            last_press = PDAnyKeyDown();
            if (last_press != -1 && (!gAlways_typing || (last_press != KEY_LEFT && last_press != KEY_RIGHT && last_press != KEY_UP && last_press != KEY_DOWN))) {
                if (gCurrent_choice != gTyping_slot && !gAlways_typing) {
                    ChangeSelection(pSpec, &gCurrent_choice, &gTyping_slot, gCurrent_mode, gAlways_typing);
                    for (i = 0; i < 2; i++) {
                        if (pSpec->typeable[i]) {
                            gCurrent_mode = i;
                            break;
                        }
                    }
                }
                TypeKey(gAlways_typing ? 0 : gCurrent_choice, last_press);
            }
        }
        ServiceGame();
        if (last_choice_2 != gCurrent_choice) {
            ChangeSelection(pSpec, &last_choice_2, &gCurrent_choice, gCurrent_mode, 1);
        }
        if (entry_status != eProg_idling && gProgram_state.prog_status == eProg_idling) {
            escaped = 1;
        }
    } while ((!pSpec->exit_proc || !(pSpec->exit_proc)(&gCurrent_choice, &gCurrent_mode)) && !go_ahead && !last_press && !escaped);
    gTyping = 0;
    if (pSpec->font_needed) {
        EndRollingLetters();
        DisposeFont(0);
    }
    for (i = 0; i < pSpec->number_of_recopy_areas; i++) {
        BrPixelmapFree(copy_areas[i]);
    }
    BrMemFree(copy_areas);

    RemoveTransientBitmaps(1);
    FlushFlicQueue();
    for (i = 0; i < pSpec->number_of_button_flics; i++) {
        UnlockFlic(pSpec->flicker_on_flics[i].flic_index);
        UnlockFlic(pSpec->flicker_off_flics[i].flic_index);
        UnlockFlic(pSpec->pushed_flics[i].flic_index);
    }

    //v100 = gCurrent_choice;
    if (gCurrent_choice == pSpec->escape_code) {
        escaped = 1;
        go_ahead = 0;
    }
    if (escaped) {
        //v100 = pSpec->escape_code;
        gCurrent_choice = pSpec->escape_code;
    }
    if (pSpec->done_proc) {
        result = (pSpec->done_proc)(gCurrent_choice, gCurrent_mode, go_ahead, escaped, timed_out);
    } else {
        result = gCurrent_choice;
    }
    if (!go_ahead) {
        if (!escaped) {
            if (pSpec->end_flic_otherwise > 0) {
                DRS3StartSound(gIndexed_outlets[0], 3007);
                RunFlic(pSpec->end_flic_otherwise);
            } else if (pSpec->end_flic_otherwise < 0) {
                FadePaletteDown(pSpec->end_flic_otherwise);
            }
            goto LABEL_230;
        } else {
            if (pSpec->end_flic_escaped > 0) {
                DRS3StartSound(gIndexed_outlets[0], 3007);
                RunFlic(pSpec->end_flic_escaped);
            } else if (pSpec->end_flic_escaped < 0) {
                FadePaletteDown(pSpec->end_flic_escaped);
            }
            goto LABEL_230;
        }
    }
    if (pSpec->end_flic_go_ahead > 0) {
        DRS3StartSound(gIndexed_outlets[0], 3007);
        RunFlic(pSpec->end_flic_go_ahead);
    } else if (pSpec->end_flic_go_ahead < 0) {
        FadePaletteDown(pSpec->end_flic_go_ahead);
    }

LABEL_230:
    gProgram_state.dont_save_or_load = 0;
    EndMouseCursor();
    UnlockInterfaceStuff();
    EdgeTriggerModeOff();
    return result;
}

// Offset: 6764
// Size: 88
// EAX: pNew_choice
// EDX: pNew_mode
void ChangeSelectionTo(int pNew_choice, int pNew_mode) {
    int last_choice;
    NOT_IMPLEMENTED();
}
