#include "loadsave.h"
#include "cutscene.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "init.h"
#include "input.h"
#include "intrface.h"
#include "loading.h"
#include "pd/sys.h"
#include "sound.h"
#include "structur.h"
#include "utility.h"
#include "world.h"
#include <brender/brender.h>
#include <stdlib.h>
#include <string.h>

tSave_game* gSaved_games[8];
int gStarted_typing;
int gSave_allowed;

#define SAVEGAME_VERSION 6

#define SWAP32_BE(V)       \
    do {                   \
        (V) = BrHtoNL(V); \
    } while (0)

// IDA: void __usercall CorrectLoadByteOrdering(int pIndex@<EAX>)
void CorrectLoadByteOrdering(int pIndex) {
    int i;
    LOG_TRACE("(%d)", pIndex);

    SWAP32_BE(gSaved_games[pIndex]->version);
    SWAP32_BE(gSaved_games[pIndex]->rank);
    SWAP32_BE(gSaved_games[pIndex]->credits);
    SWAP32_BE(gSaved_games[pIndex]->skill_level);
    SWAP32_BE(gSaved_games[pIndex]->frank_or_annitude);
    SWAP32_BE(gSaved_games[pIndex]->game_completed);
    SWAP32_BE(gSaved_games[pIndex]->current_race_index);
    SWAP32_BE(gSaved_games[pIndex]->number_of_cars);
    for (i = 0; i < COUNT_OF(gSaved_games[pIndex]->cars_available); i++) {
        SWAP32_BE(gSaved_games[pIndex]->cars_available[i]);
    }
    SWAP32_BE(gSaved_games[pIndex]->current_car_index);
    SWAP32_BE(gSaved_games[pIndex]->redo_race_index);
    for (i = 0; i < COUNT_OF(gSaved_games[pIndex]->power_up_levels); i++) {
        SWAP32_BE(gSaved_games[pIndex]->power_up_levels[i]);
    }
}

// IDA: tU32 __usercall CalcLSChecksum@<EAX>(tSave_game *pSaved_game@<EAX>)
tU32 CalcLSChecksum(tSave_game* pSaved_game) {
    tU32 checksum;
    tU32 checksum2;
    int i;
    tU8* ptr;
    LOG_TRACE("(%p)", pSaved_game);

#ifdef DETHRACE_FIX_BUGS
    if (sizeof(tSave_game) - sizeof(tU32) != offsetof(tSave_game, checksum)) {
        PDFatalError("checksum must be last element of savegame struct");
    }
#endif

    checksum = 0;
    for (i = 0, ptr = (tU8*)pSaved_game; i < (sizeof(tSave_game) - sizeof(tU32)); i++, ptr++) {
        checksum2 = (*ptr ^ 0xbd) + checksum;
        checksum = checksum ^ checksum2 << 25 ^ checksum2 >> 7;
    }
    return checksum;
}

// IDA: void __cdecl LoadSavedGames()
void LoadSavedGames(void) {
    tPath_name the_path;
    int i;
    FILE* f;
    tU32 the_size;
    LOG_TRACE("()");

#ifdef DETHRACE_FIX_BUGS
    if (sizeof(tSave_game) != 948) {
        PDFatalError("Size of tSave_game struct is not correct (should be 948 bytes)");
    }
#endif

    PathCat(the_path, gApplication_path, "SAVEGAME");
    PathCat(the_path, the_path, "SAVEx");
    for (i = 0; i < COUNT_OF(gSaved_games); i++) {
        the_path[strlen(the_path) - 1] = '0' + i;
        f = DRfopen(the_path, "rb");
        if (f == NULL) {
            continue;
        }
        the_size = GetFileLength(f);
        if (the_size == sizeof(tSave_game)) {
            gSaved_games[i] = BrMemCalloc(1, sizeof(tSave_game), kMem_saved_game);
            fread(gSaved_games[i], 1, the_size, f);
            CorrectLoadByteOrdering(i);
            if (CalcLSChecksum(gSaved_games[i]) != gSaved_games[i]->checksum || gSaved_games[i]->version != SAVEGAME_VERSION) {
                BrMemFree(gSaved_games[i]);
                gSaved_games[i] = NULL;
            }
        } else {
            gSaved_games[i] = NULL;
        }
        fclose(f);
    }
}

// IDA: void __cdecl DisposeSavedGames()
void DisposeSavedGames(void) {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gSaved_games); i++) {
        if (gSaved_games[i] != NULL) {
            BrMemFree(gSaved_games[i]);
        }
    }
}

// IDA: void __usercall LoadTheGame(int pSlot_index@<EAX>)
void LoadTheGame(int pSlot_index) {
    int i;
    char the_car_name[14];
    LOG_TRACE("(%d)", pSlot_index);

    if (gProgram_state.car_name[0] != '\0') {
        DisposeCar(&gProgram_state.current_car, gProgram_state.current_car.index);
        ClearOutStorageSpace(&gOur_car_storage_space);
    }
    strcpy(gProgram_state.player_name[0], gSaved_games[pSlot_index]->player_name[0]);
    strcpy(gProgram_state.player_name[1], gSaved_games[pSlot_index]->player_name[1]);
    gProgram_state.current_car_index = gSaved_games[pSlot_index]->current_car_index;
    gProgram_state.redo_race_index = gSaved_games[pSlot_index]->redo_race_index;
    gProgram_state.frank_or_anniness = gSaved_games[pSlot_index]->frank_or_annitude;
    gProgram_state.car_name[0] = '\0';
    AboutToLoadFirstCar();
    SwitchToRealResolution();
    LoadCar(gSaved_games[pSlot_index]->car_name, eDriver_local_human,
        &gProgram_state.current_car, gProgram_state.current_car_index,
        gProgram_state.player_name[gProgram_state.frank_or_anniness],
        &gOur_car_storage_space);
    SwitchToLoresMode();
    SetCarStorageTexturingLevel(&gOur_car_storage_space, GetCarTexturingLevel(), eCTL_full);
    gProgram_state.skill_level = gSaved_games[pSlot_index]->skill_level;
    gProgram_state.current_race_index = gSaved_games[pSlot_index]->current_race_index;
    InitGame(gProgram_state.current_race_index);
    gProgram_state.number_of_cars = gSaved_games[pSlot_index]->number_of_cars;
    memcpy(gProgram_state.cars_available, gSaved_games[pSlot_index]->cars_available, sizeof(gProgram_state.cars_available));
    for (i = 0; i < gNumber_of_races; i++) {
        gRace_list[i].been_there_done_that = gSaved_games[pSlot_index]->race_info[i].been_there_done_that;
    }
    for (i = 0; i < gNumber_of_racers; i++) {
        gOpponents[i].dead = gSaved_games[pSlot_index]->opponent_info[i].dead;
    }
    gProgram_state.credits = gSaved_games[pSlot_index]->credits;
    gProgram_state.rank = gSaved_games[pSlot_index]->rank;
    for (i = 0; i < COUNT_OF(gProgram_state.current_car.power_up_levels); i++) {
        gProgram_state.current_car.power_up_levels[i] = gSaved_games[pSlot_index]->power_up_levels[i];
    }
    gProgram_state.game_completed = gSaved_games[pSlot_index]->game_completed;
    SetSoundVolumes();
    gProgram_state.saving = 0;
    SelectOpponents(&gCurrent_race);
}

// IDA: void __cdecl StartRollingSaveNamesIn()
void StartRollingSaveNamesIn(void) {
    int i;
    int save_slot_height;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gSaved_games); i++) {
        save_slot_height = gCurrent_graf_data->save_slot_y_offset + i * gCurrent_graf_data->rolling_letter_y_pitch;
        SetSlotXY(i, gCurrent_graf_data->save_slot_x_offset, save_slot_height);
        if (gSaved_games[i] != NULL) {
            AddRollingString(gSaved_games[i]->slot_name, gCurrent_graf_data->save_slot_x_offset, save_slot_height, eRT_alpha);
            AddRollingNumber(gSaved_games[i]->rank, 2, gCurrent_graf_data->save_slot_rank_x_offset, save_slot_height);
            AddRollingNumber(gSaved_games[i]->credits, 6, gCurrent_graf_data->save_slot_credits_x_offset, save_slot_height);
        } else {
            AddRollingString(GetMiscString(kMiscString_Empty), gCurrent_graf_data->save_slot_x_offset, save_slot_height, eRT_alpha);
        }
    }
}

// IDA: void __cdecl LoadStart()
void LoadStart(void) {
    LOG_TRACE("()");

    StartRollingSaveNamesIn();
}

// IDA: int __usercall DoLoadGame@<EAX>(int pSave_allowed@<EAX>)
int DoLoadGame(void) {
    static tFlicette flicker_on[9] = {
        { 74, { 47, 94 }, { 23, 55 } },
        { 74, { 47, 94 }, { 44, 110 } },
        { 74, { 47, 94 }, { 65, 156 } },
        { 74, { 47, 94 }, { 86, 206 } },
        { 74, { 47, 94 }, { 107, 257 } },
        { 74, { 47, 94 }, { 128, 307 } },
        { 74, { 47, 94 }, { 149, 358 } },
        { 74, { 47, 94 }, { 170, 408 } },
        { 57, { 255, 510 }, { 151, 362 } },
    };
    static tFlicette flicker_off[9] = {
        { 73, { 47, 94 }, { 23, 55 } },
        { 73, { 47, 94 }, { 44, 110 } },
        { 73, { 47, 94 }, { 65, 156 } },
        { 73, { 47, 94 }, { 86, 206 } },
        { 73, { 47, 94 }, { 107, 257 } },
        { 73, { 47, 94 }, { 128, 307 } },
        { 73, { 47, 94 }, { 149, 358 } },
        { 73, { 47, 94 }, { 170, 408 } },
        { 56, { 255, 510 }, { 151, 362 } },
    };
    static tFlicette push[9] = {
        { 74, { 47, 94 }, { 23, 55 } },
        { 74, { 47, 94 }, { 44, 110 } },
        { 74, { 47, 94 }, { 65, 156 } },
        { 74, { 47, 94 }, { 86, 206 } },
        { 74, { 47, 94 }, { 107, 257 } },
        { 74, { 47, 94 }, { 128, 307 } },
        { 74, { 47, 94 }, { 149, 358 } },
        { 74, { 47, 94 }, { 170, 408 } },
        { 59, { 255, 510 }, { 151, 362 } },
    };
    static tMouse_area mouse_areas[9] = {
        { { 48, 96 }, { 17, 41 }, { 250, 500 }, { 33, 79 }, 0, 0, 0, NULL },
        { { 48, 96 }, { 39, 94 }, { 250, 500 }, { 55, 132 }, 1, 0, 0, NULL },
        { { 48, 96 }, { 59, 142 }, { 250, 500 }, { 75, 180 }, 2, 0, 0, NULL },
        { { 48, 96 }, { 80, 192 }, { 250, 500 }, { 96, 230 }, 3, 0, 0, NULL },
        { { 48, 96 }, { 101, 242 }, { 250, 500 }, { 117, 281 }, 4, 0, 0, NULL },
        { { 48, 96 }, { 122, 293 }, { 250, 500 }, { 138, 331 }, 5, 0, 0, NULL },
        { { 48, 96 }, { 143, 343 }, { 250, 500 }, { 159, 382 }, 6, 0, 0, NULL },
        { { 48, 96 }, { 164, 394 }, { 250, 500 }, { 180, 432 }, 7, 0, 0, NULL },
        { { 152, 304 }, { 151, 362 }, { 299, 598 }, { 171, 410 }, 8, 1, 0, NULL },
    };
    static tRectile recopy_areas[24] = {
        {
            { 53, 106 },
            { 17, 41 },
            { 147, 294 },
            { 35, 84 },
        },
        {
            { 53, 106 },
            { 39, 94 },
            { 147, 294 },
            { 57, 137 },
        },
        {
            { 53, 106 },
            { 59, 142 },
            { 147, 294 },
            { 77, 185 },
        },
        {
            { 53, 106 },
            { 80, 192 },
            { 147, 294 },
            { 98, 235 },
        },
        {
            { 53, 106 },
            { 101, 242 },
            { 147, 294 },
            { 119, 286 },
        },
        {
            { 53, 106 },
            { 122, 293 },
            { 147, 294 },
            { 140, 336 },
        },
        {
            { 53, 106 },
            { 143, 343 },
            { 147, 294 },
            { 161, 386 },
        },
        {
            { 53, 106 },
            { 164, 394 },
            { 147, 294 },
            { 182, 437 },
        },
        {
            { 166, 332 },
            { 17, 41 },
            { 180, 360 },
            { 35, 84 },
        },
        {
            { 166, 332 },
            { 39, 94 },
            { 180, 360 },
            { 57, 137 },
        },
        {
            { 166, 332 },
            { 59, 142 },
            { 180, 360 },
            { 77, 185 },
        },
        {
            { 166, 332 },
            { 80, 192 },
            { 180, 360 },
            { 98, 235 },
        },
        {
            { 166, 332 },
            { 101, 242 },
            { 180, 360 },
            { 119, 286 },
        },
        {
            { 166, 332 },
            { 122, 293 },
            { 180, 360 },
            { 140, 336 },
        },
        {
            { 166, 332 },
            { 143, 343 },
            { 180, 360 },
            { 161, 386 },
        },
        {
            { 166, 332 },
            { 164, 394 },
            { 180, 360 },
            { 182, 370 },
        },
        {
            { 198, 396 },
            { 17, 41 },
            { 245, 490 },
            { 35, 84 },
        },
        {
            { 198, 396 },
            { 39, 94 },
            { 245, 490 },
            { 57, 137 },
        },
        {
            { 198, 396 },
            { 59, 142 },
            { 245, 490 },
            { 77, 185 },
        },
        {
            { 198, 396 },
            { 80, 192 },
            { 245, 490 },
            { 98, 235 },
        },
        {
            { 198, 396 },
            { 101, 242 },
            { 245, 490 },
            { 119, 286 },
        },
        {
            { 198, 396 },
            { 122, 293 },
            { 245, 490 },
            { 140, 336 },
        },
        {
            { 198, 396 },
            { 143, 343 },
            { 245, 490 },
            { 161, 386 },
        },
        {
            { 198, 396 },
            { 164, 394 },
            { 245, 490 },
            { 182, 437 },
        },
    };
    static tInterface_spec interface_spec = {
        0,
        71,
        70,
        0,
        0,
        0,
        3,
        { 1, 0 },
        { -1, 8 },
        { 8, 0 },
        { 8, 7 },
        { NULL, NULL },
        { 1, 0 },
        { 1, 0 },
        { 8, 0 },
        { 8, 7 },
        { NULL, NULL },
        { -1, -1 },
        { -1, 0 },
        { 0, 8 },
        { 7, 8 },
        { NULL, NULL },
        { -1, -1 },
        { 1, 0 },
        { 0, 8 },
        { 7, 8 },
        { NULL, NULL },
        { 1, 1 },
        { NULL, NULL },
        { 1, 1 },
        { NULL, NULL },
        NULL,
        NULL,
        0,
        LoadStart,
        NULL,
        NULL,
        1,
        { 0, 0 },
        NULL,
        8,
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
    LOG_TRACE("()");

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        DoFeatureUnavailableInDemo();
        return 0;
    }

    if (gNet_mode == eNet_mode_none) {
        if (!OriginalCarmaCDinDrive()) {
            DoErrorInterface(kMiscString_PLEASE_INSERT_THE_CARMAGEDDON_CD);
            return 0;
        }
        gProgram_state.loading = 1;
        LoadSavedGames();
        if (gGame_to_load >= 0) {
            gProgram_state.last_slot = gGame_to_load;
            gProgram_state.loaded = 1;
            LoadTheGame(gGame_to_load);
            gGame_to_load = -1;
            gProgram_state.prog_status = eProg_game_starting;
            DisposeSavedGames();
            gProgram_state.loading = 0;
            return 1;
        } else {
            result = DoInterfaceScreen(&interface_spec, gFaded_palette, gProgram_state.last_slot);
            if (result != 8 && gSaved_games[result] == NULL) {
                result = 8;
                DRS3StartSound(gEffects_outlet, 3100);
            }
            if (result != 8) {
                FadePaletteDown();
                if (gProgram_state.racing) {
                    gGame_to_load = result;
                    gProgram_state.prog_status = eProg_idling;
                } else {
                    gProgram_state.last_slot = result;
                    gProgram_state.loaded = 1;
                    LoadTheGame(result);
                }
            } else {
                if (gProgram_state.racing) {
                    FadePaletteDown();
                } else {
                    RunFlic(72);
                }
            }
            gProgram_state.loading = 0;
            DisposeSavedGames();
            return result != 8;
        }
    } else {
        SuspendPendingFlic();
        DoErrorInterface(kMiscString_CannotSaveGameInNetworkPlay);
        return 0;
    }
}

// IDA: void __usercall CorrectSaveByteOrdering(int pIndex@<EAX>)
void CorrectSaveByteOrdering(int pIndex) {
    int i;
    LOG_TRACE("(%d)", pIndex);

    SWAP32_BE(gSaved_games[pIndex]->version);
    SWAP32_BE(gSaved_games[pIndex]->rank);
    SWAP32_BE(gSaved_games[pIndex]->credits);
    SWAP32_BE(gSaved_games[pIndex]->skill_level);
    SWAP32_BE(gSaved_games[pIndex]->frank_or_annitude);
    SWAP32_BE(gSaved_games[pIndex]->game_completed);
    SWAP32_BE(gSaved_games[pIndex]->current_race_index);
    SWAP32_BE(gSaved_games[pIndex]->number_of_cars);
    for (i = 0; i < COUNT_OF(gSaved_games[pIndex]->cars_available); i++) {
        SWAP32_BE(gSaved_games[pIndex]->cars_available[i]);
    }
    SWAP32_BE(gSaved_games[pIndex]->current_car_index);
    SWAP32_BE(gSaved_games[pIndex]->redo_race_index);
    for (i = 0; i < COUNT_OF(gSaved_games[pIndex]->power_up_levels); i++) {
        SWAP32_BE(gSaved_games[pIndex]->power_up_levels[i]);
    }
}

// IDA: void __usercall SaveTheGame(int pSlot_number@<EAX>)
void SaveTheGame(int pSlot_number) {
    tPath_name the_path;
    FILE* f;
    LOG_TRACE("(%d)", pSlot_number);

    gSaved_games[pSlot_number]->checksum = CalcLSChecksum(gSaved_games[pSlot_number]);
    PathCat(the_path, gApplication_path, "SAVEGAME");
    PathCat(the_path, the_path, "SAVEx");
    the_path[strlen(the_path) - 1] = '0' + pSlot_number;
    PDFileUnlock(the_path);
    f = DRfopen(the_path, "wb");
    if (f != NULL) {
        CorrectSaveByteOrdering(pSlot_number);
        fwrite(gSaved_games[pSlot_number], 1, sizeof(tSave_game), f);
        fclose(f);
        CorrectLoadByteOrdering(pSlot_number);
    }
}

// IDA: int __cdecl ConfirmMidGameSave()
int ConfirmMidGameSave(void) {
    static tFlicette flicker_on[2] = {
        { 43, { 84, 168 }, { 124, 298 } },
        { 43, { 181, 362 }, { 124, 298 } },
    };
    static tFlicette flicker_off[2] = {
        { 42, { 84, 168 }, { 124, 298 } },
        { 42, { 181, 362 }, { 124, 298 } },
    };
    static tFlicette push[2] = {
        { 44, { 84, 168 }, { 124, 298 } },
        { 45, { 181, 362 }, { 124, 298 } },
    };
    static tMouse_area mouse_areas[2] = {
        { { 84, 168 }, { 124, 298 }, { 147, 294 }, { 144, 346 }, 0, 0, 0, NULL },
        { { 181, 362 }, { 124, 298 }, { 244, 488 }, { 144, 346 }, 1, 0, 0, NULL },
    };
    static tInterface_spec interface_spec = {
        0, 40, 0, 41, -1, -1, 0,
        { -1, 0 }, { -1, 0 }, { 0, 0 }, { 1, 0 }, { NULL, NULL },
        { -1, 0 }, { 1, 0 }, { 0, 0 }, { 1, 0 }, { NULL, NULL },
        { -1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { NULL, NULL },
        { -1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { NULL, NULL },
        { 1, 1 }, { NULL, NULL }, { 1, 1 }, { NULL, NULL },
        NULL, NULL, 0, NULL, NULL, NULL, 0, { 0, 0 },
        NULL, 1, 1,
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas,
        0, NULL
    };
    LOG_TRACE("()");

    return DoInterfaceScreen(&interface_spec, 0, 0) == 0;
}

// IDA: void __usercall MakeSavedGame(tSave_game **pSave_record@<EAX>)
void MakeSavedGame(tSave_game** pSave_record) {
    int i;
    LOG_TRACE("(%p)", pSave_record);

    if (*pSave_record == NULL) {
        *pSave_record = BrMemCalloc(1, sizeof(tSave_game), kMem_new_save_game);
    }
    (*pSave_record)->skill_level = gProgram_state.skill_level;
    (*pSave_record)->version = SAVEGAME_VERSION;
    (*pSave_record)->frank_or_annitude = gProgram_state.frank_or_anniness;
    (*pSave_record)->game_completed = gProgram_state.game_completed;
    (*pSave_record)->current_race_index = gProgram_state.current_race_index;
    for (i = 0; i < gNumber_of_races; i++) {
        (*pSave_record)->race_info[i].been_there_done_that = gRace_list[i].been_there_done_that;
    }
    for (i = 0; i < gNumber_of_racers; i++) {
        (*pSave_record)->opponent_info[i].dead = gOpponents[i].dead;
    }
    (*pSave_record)->credits = gProgram_state.credits;
    (*pSave_record)->rank = gProgram_state.rank;
    strcpy((*pSave_record)->car_name, gProgram_state.car_name);
    strcpy((*pSave_record)->player_name[0], gProgram_state.player_name[0]);
    strcpy((*pSave_record)->player_name[1], gProgram_state.player_name[1]);

    (*pSave_record)->number_of_cars = gProgram_state.number_of_cars;
    (*pSave_record)->current_car_index = gProgram_state.current_car_index;
    (*pSave_record)->redo_race_index = gProgram_state.redo_race_index;
    for (i = 0; i < COUNT_OF(gProgram_state.cars_available); i++) {
        (*pSave_record)->cars_available[i] = gProgram_state.cars_available[i];
    }
    for (i = 0; i < COUNT_OF(gProgram_state.current_car.power_up_levels); i++) {
        (*pSave_record)->power_up_levels[i] = gProgram_state.current_car.power_up_levels[i];
    }
}

// IDA: void __cdecl SaveStart()
void SaveStart(void) {
    LOG_TRACE("()");

    StartRollingSaveNamesIn();
}

// IDA: void __usercall GetSaveName(int pStarting_to_type@<EAX>, int pCurrent_choice@<EDX>, char *pString@<EBX>, int *pMax_length@<ECX>)
void GetSaveName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {
    LOG_TRACE("(%d, %d, \"%s\", %p)", pStarting_to_type, pCurrent_choice, pString, pMax_length);

    if (gSaved_games[pCurrent_choice] != NULL) {
        strcpy(pString, gSaved_games[pCurrent_choice]->slot_name);
    } else {
        if (pStarting_to_type) {
            BlankSlot(pCurrent_choice, strlen(GetMiscString(kMiscString_Empty)), 12);
            *pString = '\0';
        } else {
            strcpy(pString, GetMiscString(kMiscString_Empty));
        }
    }
    *pMax_length = 12;
}

// IDA: int __usercall SaveDone@<EAX>(int pCurrent_choice@<EAX>, int pCurrent_mode@<EDX>, int pGo_ahead@<EBX>, int pEscaped@<ECX>, int pTimed_out)
int SaveDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    LOG_TRACE("(%d, %d, %d, %d, %d)", pCurrent_choice, pCurrent_mode, pGo_ahead, pEscaped, pTimed_out);

    if (!pEscaped && pCurrent_choice < 8) {
        gProgram_state.last_slot = pCurrent_choice;
        MakeSavedGame(&gSaved_games[pCurrent_choice]);
        if (!gSave_allowed && gPre_race_saved_game != NULL) {
            memcpy(gSaved_games[pCurrent_choice], gPre_race_saved_game, sizeof(tSave_game));
        }
        GetTypedName(gSaved_games[pCurrent_choice]->slot_name, 12);
        gSaved_games[pCurrent_choice]->slot_name[12] = '\0';
        SaveTheGame(pCurrent_choice);
    }
    return pCurrent_choice;
}

// IDA: int __usercall SaveGoAhead@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int SaveGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    char s1[256];
    char s2[256];
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gTyping_slot != 0) {
        sprintf(s1, VARLZEROINT, 2, gProgram_state.rank);
        if (gSaved_games[*pCurrent_choice] == NULL) {
            s2[0] = '\0';
        } else {
            sprintf(s2, VARLZEROINT, 2, gSaved_games[*pCurrent_choice]->rank);
        }
        ChangeTextTo(gCurrent_graf_data->save_slot_rank_x_offset,
            gCurrent_graf_data->save_slot_y_offset + *pCurrent_choice * gCurrent_graf_data->rolling_letter_y_pitch,
            s1, s2);
        sprintf(s1, VARLZEROINT, 6, gProgram_state.credits);
        if (gSaved_games[*pCurrent_choice] == NULL) {
            s2[0] = '\0';
        } else {
            sprintf(s2, VARLZEROINT, 6, gSaved_games[*pCurrent_choice]->credits);
        }
        ChangeTextTo(gCurrent_graf_data->save_slot_credits_x_offset,
            gCurrent_graf_data->save_slot_y_offset + *pCurrent_choice * gCurrent_graf_data->rolling_letter_y_pitch,
            s1, s2);
        gStarted_typing = 1;
    }
    return 1;
}

// IDA: int __usercall SaveEscape@<EAX>(int *pCurrent_choice@<EAX>, int *pCurrent_mode@<EDX>)
int SaveEscape(int* pCurrent_choice, int* pCurrent_mode) {
    char s1[256];
    char s2[256];
    LOG_TRACE("(%p, %p)", pCurrent_choice, pCurrent_mode);

    if (gStarted_typing != 0) {
        sprintf(s1, VARLZEROINT, 2, gProgram_state.rank);
        if (gSaved_games[*pCurrent_choice] == NULL) {
            s2[0] = '\0';
        } else {
            sprintf(s2, VARLZEROINT, 2, gSaved_games[*pCurrent_choice]->rank);
        }
        ChangeTextTo(gCurrent_graf_data->save_slot_rank_x_offset,
            gCurrent_graf_data->save_slot_y_offset + *pCurrent_choice * gCurrent_graf_data->rolling_letter_y_pitch,
            s2, s1);
        sprintf(s1, VARLZEROINT, 6, gProgram_state.credits);
        if (gSaved_games[*pCurrent_choice] == NULL) {
            s2[0] = '\0';
        } else {
            sprintf(s2, VARLZEROINT, 6, gSaved_games[*pCurrent_choice]->credits);
        }
        ChangeTextTo(gCurrent_graf_data->save_slot_credits_x_offset,
            gCurrent_graf_data->save_slot_y_offset + *pCurrent_choice * gCurrent_graf_data->rolling_letter_y_pitch,
            s2, s1);
        gStarted_typing = 0;
    }
    return 1;
}

// IDA: int __usercall SaveGameInterface@<EAX>(int pDefault_choice@<EAX>)
int SaveGameInterface(int pDefault_choice) {
    static tFlicette flicker_on[9] = {
        { 74, { 47, 94 }, { 23, 55 } },
        { 74, { 47, 94 }, { 44, 106 } },
        { 74, { 47, 94 }, { 65, 156 } },
        { 74, { 47, 94 }, { 86, 206 } },
        { 74, { 47, 94 }, { 107, 257 } },
        { 74, { 47, 94 }, { 128, 307 } },
        { 74, { 47, 94 }, { 149, 358 } },
        { 74, { 47, 94 }, { 170, 408 } },
        { 57, { 255, 510 }, { 151, 362 } },
    };
    static tFlicette flicker_off[9] = {
        { 73, { 47, 94 }, { 23, 55 } },
        { 73, { 47, 94 }, { 44, 106 } },
        { 73, { 47, 94 }, { 65, 156 } },
        { 73, { 47, 94 }, { 86, 206 } },
        { 73, { 47, 94 }, { 107, 257 } },
        { 73, { 47, 94 }, { 128, 307 } },
        { 73, { 47, 94 }, { 149, 358 } },
        { 73, { 47, 94 }, { 170, 408 } },
        { 56, { 255, 510 }, { 151, 362 } },
    };
    static tFlicette push[9] = {
        { 74, { 47, 94 }, { 23, 55 } },
        { 74, { 47, 94 }, { 44, 106 } },
        { 74, { 47, 94 }, { 65, 156 } },
        { 74, { 47, 94 }, { 86, 206 } },
        { 74, { 47, 94 }, { 107, 257 } },
        { 74, { 47, 94 }, { 128, 307 } },
        { 74, { 47, 94 }, { 149, 358 } },
        { 74, { 47, 94 }, { 170, 408 } },
        { 59, { 255, 510 }, { 151, 362 } },
    };
    static tMouse_area mouse_areas[9] = {
        { { 48, 96 }, { 17, 41 }, { 250, 500 }, { 33, 79 }, 0, 0, 0, NULL },
        { { 48, 96 }, { 39, 94 }, { 250, 500 }, { 55, 132 }, 1, 0, 0, NULL },
        { { 48, 96 }, { 59, 142 }, { 250, 500 }, { 75, 180 }, 2, 0, 0, NULL },
        { { 48, 96 }, { 80, 192 }, { 250, 500 }, { 96, 230 }, 3, 0, 0, NULL },
        { { 48, 96 }, { 101, 242 }, { 250, 500 }, { 117, 281 }, 4, 0, 0, NULL },
        { { 48, 96 }, { 122, 293 }, { 250, 500 }, { 138, 331 }, 5, 0, 0, NULL },
        { { 48, 96 }, { 143, 343 }, { 250, 500 }, { 159, 382 }, 6, 0, 0, NULL },
        { { 48, 96 }, { 164, 394 }, { 250, 500 }, { 180, 432 }, 7, 0, 0, NULL },
        { { 152, 304 }, { 151, 362 }, { 299, 598 }, { 171, 410 }, 8, 1, 0, NULL },
    };
    static tRectile recopy_areas[24] = {
        {
            { 53, 106 },
            { 17, 41 },
            { 147, 294 },
            { 35, 84 },
        },
        {
            { 53, 106 },
            { 39, 94 },
            { 147, 294 },
            { 57, 137 },
        },
        {
            { 53, 106 },
            { 59, 142 },
            { 147, 294 },
            { 77, 185 },
        },
        {
            { 53, 106 },
            { 80, 192 },
            { 147, 294 },
            { 98, 235 },
        },
        {
            { 53, 106 },
            { 101, 242 },
            { 147, 294 },
            { 119, 286 },
        },
        {
            { 53, 106 },
            { 122, 293 },
            { 147, 294 },
            { 140, 336 },
        },
        {
            { 53, 106 },
            { 143, 343 },
            { 147, 294 },
            { 161, 386 },
        },
        {
            { 53, 106 },
            { 164, 394 },
            { 147, 294 },
            { 182, 437 },
        },
        {
            { 166, 332 },
            { 17, 41 },
            { 180, 360 },
            { 35, 84 },
        },
        {
            { 166, 332 },
            { 39, 94 },
            { 180, 360 },
            { 57, 137 },
        },
        {
            { 166, 332 },
            { 59, 142 },
            { 180, 360 },
            { 77, 185 },
        },
        {
            { 166, 332 },
            { 80, 192 },
            { 180, 360 },
            { 98, 235 },
        },
        {
            { 166, 332 },
            { 101, 242 },
            { 180, 360 },
            { 119, 286 },
        },
        {
            { 166, 332 },
            { 122, 293 },
            { 180, 360 },
            { 140, 336 },
        },
        {
            { 166, 332 },
            { 143, 343 },
            { 180, 360 },
            { 161, 386 },
        },
        {
            { 166, 332 },
            { 164, 394 },
            { 180, 360 },
            { 182, 437 },
        },
        {
            { 198, 396 },
            { 17, 41 },
            { 245, 490 },
            { 35, 84 },
        },
        {
            { 198, 396 },
            { 39, 94 },
            { 245, 490 },
            { 57, 137 },
        },
        {
            { 198, 396 },
            { 59, 142 },
            { 245, 490 },
            { 77, 185 },
        },
        {
            { 198, 396 },
            { 80, 192 },
            { 245, 490 },
            { 98, 235 },
        },
        {
            { 198, 396 },
            { 101, 242 },
            { 245, 490 },
            { 119, 286 },
        },
        {
            { 198, 396 },
            { 122, 293 },
            { 245, 490 },
            { 140, 336 },
        },
        {
            { 198, 396 },
            { 143, 343 },
            { 245, 490 },
            { 161, 386 },
        },
        {
            { 198, 396 },
            { 164, 394 },
            { 245, 490 },
            { 182, 437 },
        },
    };
    static tInterface_spec interface_spec = {
        0, 50, 0, 0, 0, 0, 2,
        { 1, 0 }, { -1, 8 }, { 8, 0 }, { 8, 7 }, { NULL, NULL },
        { 1, 0 }, { 1, 0 }, { 8, 0 }, { 8, 7 }, { NULL, NULL },
        { -1, -1 }, { -1, 0 }, { 0, 8 }, { 7, 8 }, { NULL, NULL },
        { -1, -1 }, { 1, 0 }, { 0, 8 }, { 7, 8 }, { NULL, NULL },
        { 1, 1 }, { SaveGoAhead, NULL },
        { 1, 1 }, { SaveEscape, NULL },
        NULL, NULL, 0, SaveStart, NULL, SaveDone, 1,
        { 1, 0 }, GetSaveName, 8, 1,
        COUNT_OF(flicker_on), flicker_on, flicker_off, push,
        COUNT_OF(mouse_areas), mouse_areas,
        COUNT_OF(recopy_areas), recopy_areas
    };
    LOG_TRACE("(%d)", pDefault_choice);

    gStarted_typing = 0;
    return DoInterfaceScreen(&interface_spec, 0, pDefault_choice) < 8;
}

// IDA: void __usercall DoSaveGame(int pSave_allowed@<EAX>)
void DoSaveGame(int pSave_allowed) {
    LOG_TRACE("()");

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        DoFeatureUnavailableInDemo();
        return;
    }

    if (gNet_mode == eNet_mode_none) {
        DRS3StopOutletSound(gEffects_outlet);
        gProgram_state.saving = 1;
        gSave_allowed = pSave_allowed;
        LoadSavedGames();
        LoadFont(kFont_TYPEABLE);
        if (!pSave_allowed && !ConfirmMidGameSave()) {
            gProgram_state.saving = 0;
            return;
        }
        SaveGameInterface(gProgram_state.last_slot);
        if (pSave_allowed) {
            RunFlic(51);
        } else {
            FadePaletteDown();
        }
        DisposeSavedGames();
        gProgram_state.saving = 0;
    } else {
        SuspendPendingFlic();
        DoErrorInterface(kMiscString_CannotSaveGameInNetworkPlay);
    }
}
