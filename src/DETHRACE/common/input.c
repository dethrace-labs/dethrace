#include "input.h"

#include "brender/brender.h"
#include "errors.h"
#include "globvars.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "pd/sys.h"
#include "utility.h"
#include <stdlib.h>

int gEdge_trigger_mode;
tU32 gLast_poll_keys;
int gInsert_mode;
int gGo_ahead_keys[3] = { 51, 52, 106 }; // enter, return, space
tJoy_array gJoy_array;
tKey_array gKey_array;
int gKey_poll_counter;
tRolling_letter* gRolling_letters;
int gCurrent_cursor;
int gCurrent_position;
int gLetter_x_coords[15];
int gVisible_length;
int gLetter_y_coords[15];
int gThe_key;
tU32 gLast_key_down_time;
int gThe_length;
tU32 gLast_roll;
int gLast_key_down;
int gKey_mapping[67];
char gCurrent_typing[110];

#define NBR_ROLLING_LETTERS 500

// IDA: void __usercall SetJoystickArrays(int *pKeys@<EAX>, int pMark@<EDX>)
void SetJoystickArrays(int* pKeys, int pMark) {
    int i;
    tS32 joyX;
    tS32 joyY;
    static tS32 old_joy1X;
    static tS32 old_joy1Y;
    static tS32 old_joy2X;
    static tS32 old_joy2Y;
}

// IDA: void __cdecl PollKeys()
void PollKeys(void) {

    gKey_poll_counter++;
    PDSetKeyArray(gKey_array, gKey_poll_counter);
    SetJoystickArrays(gKey_array, gKey_poll_counter);
    gLast_poll_keys = PDGetTotalTime();
}

// IDA: void __cdecl CyclePollKeys()
void CyclePollKeys(void) {
    int i;
    for (i = 0; i < COUNT_OF(gKey_array); i++) {
        if (gKey_array[i] > gKey_poll_counter) {
            gKey_array[i] = 0;
            if (i > 115) {
                gJoy_array[i - 115] = -1; // yes this is a little weird I know...
            }
        }
    }
    gKey_poll_counter = 0;
}

// IDA: void __cdecl ResetPollKeys()
void ResetPollKeys(void) {
    int i;
    for (i = 0; i < COUNT_OF(gKey_array); i++) {
        gKey_array[i] = 0;
    }
    for (i = 0; i < COUNT_OF(gJoy_array); i++) {
        gJoy_array[i] = -1;
    }
}

// IDA: void __cdecl CheckKeysForMouldiness()
void CheckKeysForMouldiness(void) {
    LOG_TRACE9("()");

    if (PDGetTotalTime() - gLast_poll_keys > 500) {
        ResetPollKeys();
        CyclePollKeys();
        PollKeys();
    }
}

// IDA: int __cdecl EitherMouseButtonDown()
int EitherMouseButtonDown(void) {
    int but_1;
    int but_2;

    PDMouseButtons(&but_1, &but_2);
    return but_1 || but_2;
}

// IDA: tKey_down_result __usercall PDKeyDown2@<EAX>(int pKey_index@<EAX>)
tKey_down_result PDKeyDown2(int pKey_index) {
    tU32 the_time;

    CheckKeysForMouldiness();
    if (!gEdge_trigger_mode) {
        return gKey_array[pKey_index];
    }
    the_time = PDGetTotalTime();
    if (gKey_array[pKey_index]) {
        if (gLast_key_down == pKey_index) {
            if (the_time - gLast_key_down_time < 300) {
                return tKey_down_still;
            } else {
                gLast_key_down_time = the_time;
                return tKey_down_repeat;
            }
        } else {
            gLast_key_down_time = the_time;
            gLast_key_down = pKey_index;
            return tKey_down_yes;
        }
    }
    if (gLast_key_down == pKey_index) {
        gLast_key_down_time = 0;
        gLast_key_down = -1;
    }
    return tKey_down_no;
}

// IDA: int __usercall PDKeyDown@<EAX>(int pKey_index@<EAX>)
int PDKeyDown(int pKey_index) {
    tKey_down_result result;

    result = PDKeyDown2(pKey_index);
    if (!gEdge_trigger_mode || pKey_index <= 10) {
        return result != tKey_down_no;
    }
    return result == tKey_down_yes || result == tKey_down_repeat;
}

// IDA: int __usercall PDKeyDown3@<EAX>(int pKey_index@<EAX>)
int PDKeyDown3(int pKey_index) {
    int last_key_down_time;
    int last_key_down;
    tKey_down_result result;
    LOG_TRACE("(%d)", pKey_index);

    last_key_down = gLast_key_down;
    last_key_down_time = gLast_key_down_time;
    result = PDKeyDown2(pKey_index);
    gLast_key_down_time = last_key_down_time;
    gLast_key_down = last_key_down;
    return result == tKey_down_yes || result == tKey_down_repeat;
}

// IDA: int __cdecl PDAnyKeyDown()
int PDAnyKeyDown(void) {
    int i;
    tKey_down_result result;

    CheckKeysForMouldiness();
    for (i = COUNT_OF(gKey_array) - 1; i >= 0; --i) {
        if (gKey_array[i]) {
            if (!gEdge_trigger_mode) {
                return i;
            }
            result = PDKeyDown2(i);
            switch (result) {
            case tKey_down_no:
            case tKey_down_still:
                return -1;
            case tKey_down_yes:
            case tKey_down_repeat:
                return i;
            }
        }
    }
    if (gEdge_trigger_mode) {
        gLast_key_down_time = 0;
        gLast_key_down = -1;
    }
    return -1;
}

// IDA: int __cdecl AnyKeyDown()
int AnyKeyDown(void) {
    int the_key;

    the_key = PDAnyKeyDown();
    if ((the_key != -1 && the_key != 4) || EitherMouseButtonDown() != 0) {
        return 1;
    }
    return 0;
}

// IDA: tU32* __cdecl KevKeyService()
tU32* KevKeyService(void) {
    static tU32 sum = 0;
    static tU32 code = 0;
    static tU32 code2 = 0;
    static int last_key = -1;
    static int last_single_key = -1;
    static tU32 last_time = 0;
    static tU32 return_val[2];
    tU32 keys;

    keys = gKeys_pressed;
    // printf("key: %d, %lx, %lx\n", sizeof(long), keys, code2);
    return_val[0] = 0;
    return_val[1] = 0;

    if (keys < 0x6B) {
        last_single_key = gKeys_pressed;
    } else {
        if (keys > 0x6b00) {
            sum = 0;
            code = 0;
            return return_val;
        }
        if ((keys & 0xff) != last_single_key && keys >> 8 != last_single_key) {
            sum = 0;
            code = 0;
            return return_val;
        }
        if (keys >> 8 != last_single_key) {
            sum = 0;
            code = 0;
            return return_val;
        }
        if ((keys & 0xff) == last_single_key) {
            keys = keys >> 8;
        }
        keys = keys & 0xff;
    }

    if (keys && keys != last_key) {
        sum += keys;
        code += keys << 11;
        code = (code >> 17) + (code << 4);
        code2 = (code2 >> 29) + keys * keys + (code2 << 3);
        // printf("accumulate: keys=%lx, sum=%lx, code=%lx, code2=%lx\n", keys, sum, code, code2);
        last_time = PDGetTotalTime();
    } else if (PDGetTotalTime() > (last_time + 1000)) {
        return_val[0] = ((code >> 11) + (sum << 21));
        return_val[1] = code2;
        // printf("final value: code=%lx, code2=%lx\n", return_val[0], return_val[1]);
        code = 0;
        code2 = 0;
        sum = 0;
    }
    last_key = keys;
    return return_val;
}

// IDA: int __usercall OldKeyIsDown@<EAX>(int pKey_index@<EAX>)
int OldKeyIsDown(int pKey_index) {
    int i;
    LOG_TRACE("(%d)", pKey_index);

    switch (pKey_index) {
    case -2:
        return 1;
    case -1:
        for (i = 0; i < COUNT_OF(gGo_ahead_keys); i++) {
            if (PDKeyDown(gGo_ahead_keys[i]) != 0) {
                return 1;
            }
        }
        return 0;
    default:
        return PDKeyDown(gKey_mapping[pKey_index]);
    }
}

// IDA: int __usercall KeyIsDown@<EAX>(int pKey_index@<EAX>)
int KeyIsDown(int pKey_index) {
    int i;

    CheckKeysForMouldiness();
    switch (pKey_index) {
    case -2:
        return 1;
    case -1:
        for (i = 0; i < COUNT_OF(gGo_ahead_keys); i++) {
            if (gKey_array[gGo_ahead_keys[i]]) {
                return 1;
            }
        }
        return 0;
    default:
        return gKey_array[gKey_mapping[pKey_index]];
    }
}

// IDA: void __cdecl WaitForNoKeys()
void WaitForNoKeys(void) {
    LOG_TRACE("()");

    while (AnyKeyDown() || EitherMouseButtonDown()) {
        CheckQuit();
    }
    CheckQuit();
}

// IDA: void __cdecl WaitForAKey()
void WaitForAKey(void) {
    LOG_TRACE("()");

    while (1) {
        CheckQuit();
        if (AnyKeyDown()) {
            break;
        }
        if (EitherMouseButtonDown()) {
            break;
        }
    }
    CheckQuit();
    WaitForNoKeys();
}

// IDA: int __usercall CmdKeyDown@<EAX>(int pFKey_ID@<EAX>, int pCmd_key_ID@<EDX>)
int CmdKeyDown(int pFKey_ID, int pCmd_key_ID) {
    return KeyIsDown(pFKey_ID) || (KeyIsDown(KEYMAP_CONTROL_ANY) && KeyIsDown(pCmd_key_ID));
}

// IDA: void __usercall GetMousePosition(int *pX_coord@<EAX>, int *pY_coord@<EDX>)
void GetMousePosition(int* pX_coord, int* pY_coord) {
    int x_left_margin;
    int x_right_margin;
    int y_top_margin;
    int y_bottom_margin;
    LOG_TRACE("(%p, %p)", pX_coord, pY_coord);

    PDGetMousePosition(pX_coord, pY_coord);
    if (*pX_coord < 0) {
        *pX_coord = 0;
    } else if (gGraf_specs[gGraf_spec_index].total_width < *pX_coord) {
        *pX_coord = gGraf_specs[gGraf_spec_index].total_width;
    }
    if (*pY_coord < 0) {
        *pY_coord = 0;
    } else if (gGraf_specs[gGraf_spec_index].total_height < *pY_coord) {
        *pY_coord = gGraf_specs[gGraf_spec_index].total_height;
    }
}

// IDA: void __cdecl InitRollingLetters()
void InitRollingLetters(void) {
    int i;
    LOG_TRACE("()");

    gLast_roll = 0;
    gCurrent_cursor = -1;
    gRolling_letters = BrMemAllocate(NBR_ROLLING_LETTERS * sizeof(tRolling_letter), kMem_rolling_letters);
    for (i = 0; i < NBR_ROLLING_LETTERS; i++) {
        gRolling_letters[i].number_of_letters = -1;
    }
}

// IDA: void __cdecl EndRollingLetters()
void EndRollingLetters(void) {
    LOG_TRACE("()");

    BrMemFree(gRolling_letters);
}

// IDA: int __usercall AddRollingLetter@<EAX>(char pChar@<EAX>, int pX@<EDX>, int pY@<EBX>, tRolling_type rolling_type@<ECX>)
int AddRollingLetter(char pChar, int pX, int pY, tRolling_type rolling_type) {
    tRolling_letter* let;
    int i;
    int number_of_letters;
    LOG_TRACE("(%d, %d, %d, %d)", pChar, pX, pY, rolling_type);

    let = &gRolling_letters[0];
    for (i = 0; i < NBR_ROLLING_LETTERS; i++) {
        let = &gRolling_letters[i];
        if (let->number_of_letters < 0) {
            break;
        }
    }
    if (i == NBR_ROLLING_LETTERS) {
        LOG_WARN("no rolling slot available");
        return -1;
    }
    let->x_coord = pX;
    let->y_coord = pY;
    let->rolling_type = rolling_type;
    switch (rolling_type) {
    case eRT_looping_random:
        let->number_of_letters = 9;
        break;
    case eRT_looping_single:
        let->number_of_letters = 2;
        break;
    default:
        let->number_of_letters = IRandomBetween(3, 9);
        break;
    }

    let->current_offset = (gCurrent_graf_data->save_slot_letter_height * let->number_of_letters);
    for (i = 0; i < let->number_of_letters; i++) {
        if (rolling_type == eRT_numeric) {
            let->letters[i] = pChar;
        } else {
            let->letters[i] = IRandomBetween('A', 'Z' + 1);
        }
    }
    if (rolling_type != eRT_looping_random) {
        let->letters[0] = pChar;
    }

    return 0;
}

// IDA: void __usercall AddRollingString(char *pStr@<EAX>, int pX@<EDX>, int pY@<EBX>, tRolling_type rolling_type@<ECX>)
void AddRollingString(char* pStr, int pX, int pY, tRolling_type rolling_type) {
    int i;
    LOG_TRACE("(\"%s\", %d, %d, %d)", pStr, pX, pY, rolling_type);

    for (i = 0; i < strlen(pStr); i++) {
        AddRollingLetter(pStr[i], pX, pY, rolling_type);
        pX += gCurrent_graf_data->rolling_letter_x_pitch;
    }
}

// IDA: void __usercall AddRollingNumber(tU32 pNumber@<EAX>, int pWidth@<EDX>, int pX@<EBX>, int pY@<ECX>)
void AddRollingNumber(tU32 pNumber, int pWidth, int pX, int pY) {
    char the_string[32];
    LOG_TRACE("(%d, %d, %d, %d)", pNumber, pWidth, pX, pY);

    sprintf(the_string, VARLZEROINT, pWidth, pNumber);
    AddRollingString(the_string, pX, pY, eRT_numeric);
}

// IDA: void __cdecl RollLettersIn()
void RollLettersIn(void) {
    tU32 new_time;
    tU32 period;
    tRolling_letter* let;
    int i;
    int j;
    int k;
    int offset;
    int which_letter;
    int font_width;
    int letter_offset;
    int font_height;
    int the_row_bytes;
    tU8* char_ptr;
    tU8* saved_char_ptr;
    tU8* source_ptr;
    tU8 the_byte;
    LOG_TRACE9("()");

    new_time = PDGetTotalTime();
    if (gLast_roll) {
        period = new_time - gLast_roll;
    } else {
        period = 0;
    }
    font_height = gFonts[FONT_TYPEABLE].height;
    font_width = gFonts[FONT_TYPEABLE].width;
    the_row_bytes = gFonts[FONT_TYPEABLE].images->row_bytes;

    for (i = 0; i < NBR_ROLLING_LETTERS; i++) {
        let = &gRolling_letters[i];
        if (let->number_of_letters >= 0) {
            char_ptr = gBack_screen->pixels;
            char_ptr += let->y_coord * gBack_screen->row_bytes + let->x_coord;
            if (let->current_offset > 0.0f) {
                let->current_offset -= period * 0.18f;
                if (let->current_offset <= 0.0f) {
                    if (let->rolling_type == eRT_looping_random || let->rolling_type == eRT_looping_single) {
                        let->current_offset = (gCurrent_graf_data->save_slot_letter_height * let->number_of_letters) + let->current_offset;
                    } else {
                        let->current_offset = 0.0f;
                    }
                }
            }
            for (j = 0; j < gCurrent_graf_data->save_slot_height; j++) {
                offset = gCurrent_graf_data->save_slot_table[j] + let->current_offset;
                which_letter = offset / gCurrent_graf_data->save_slot_letter_height;
                letter_offset = offset % gCurrent_graf_data->save_slot_letter_height - (gCurrent_graf_data->save_slot_letter_height - font_height) / 2;
                saved_char_ptr = char_ptr;
                if (which_letter < let->number_of_letters && which_letter >= 0 && letter_offset >= 0 && letter_offset < font_height) {

                    // LOG_DEBUG("chars %d, %d, %d, %d", let->letters[0], let->letters[1], let->letters[2], let->letters[3]);
                    source_ptr = (tU8*)gFonts[FONT_TYPEABLE].images->pixels + (font_height * (let->letters[which_letter] - ' ') + letter_offset) * the_row_bytes;
                    for (k = 0; k < font_width; k++) {
                        the_byte = *source_ptr;
                        if (the_byte) {
                            *char_ptr = the_byte;
                        }
                        char_ptr++;
                        source_ptr++;
                    }
                }
                char_ptr = saved_char_ptr + gBack_screen->row_bytes;
            }
        }
    }
    gLast_roll = new_time;
}

// IDA: int __usercall ChangeCharTo@<EAX>(int pSlot_index@<EAX>, int pChar_index@<EDX>, char pNew_char@<EBX>)
int ChangeCharTo(int pSlot_index, int pChar_index, char pNew_char) {
    int x_coord;
    int y_coord;
    int i;
    int j;
    tRolling_letter* let;
    tRolling_type new_type;
    LOG_TRACE("(%d, %d, %d)", pSlot_index, pChar_index, pNew_char);

    if (pChar_index >= gVisible_length || pChar_index < 0) {
        return -1;
    }
    y_coord = gLetter_y_coords[pSlot_index];
    x_coord = gCurrent_graf_data->rolling_letter_x_pitch * pChar_index + gLetter_x_coords[pSlot_index];

    if (pNew_char == ROLLING_LETTER_LOOP_RANDOM) {
        new_type = eRT_looping_random;
    } else if (pNew_char >= '0' && pNew_char <= '9') {
        new_type = eRT_numeric;
    } else {
        new_type = eRT_alpha;
    }

    for (i = 0; i < NBR_ROLLING_LETTERS; i++) {
        let = &gRolling_letters[i];
        if (let->number_of_letters >= 0 && x_coord == let->x_coord && y_coord == let->y_coord) {
            break;
        }
    }
    if (i >= NBR_ROLLING_LETTERS) {
        return AddRollingLetter(pNew_char, x_coord, y_coord, new_type);
    }
    if (pNew_char != ROLLING_LETTER_LOOP_RANDOM) {
        let->letters[0] = pNew_char;
    }
    if (pNew_char == ' ') {
        let->letters[0] = ' ';
    }
    let->rolling_type = new_type;
    let->current_offset = gCurrent_graf_data->save_slot_letter_height * let->number_of_letters;
    return i;
}

// IDA: void __usercall ChangeTextTo(int pXcoord@<EAX>, int pYcoord@<EDX>, char *pNew_str@<EBX>, char *pOld_str@<ECX>)
void ChangeTextTo(int pXcoord, int pYcoord, char* pNew_str, char* pOld_str) {
    int x_coord;
    int i;
    int len;
    int len2;
    int j;
    tRolling_letter* let;
    tRolling_type new_type;
    char new_char;
    LOG_TRACE("(%d, %d, \"%s\", \"%s\")", pXcoord, pYcoord, pNew_str, pOld_str);

    len = strlen(pOld_str);
    len2 = strlen(pNew_str);
#if defined(DETHRACE_FIX_BUGS)
    new_type = eRT_looping_random;
#endif

    for (i = 0; i < len; i++) {
        if (i < len2) {
            new_char = pNew_str[i];
        } else {
            new_char = ' ';
        }
        if (new_char == ROLLING_LETTER_LOOP_RANDOM) {
            new_type = eRT_looping_random;
        } else if (new_char >= '0' && new_char <= '9') {
            new_type = eRT_numeric;
        } else {
            new_type = eRT_alpha;
        }
        x_coord = gCurrent_graf_data->rolling_letter_x_pitch * i + pXcoord;
        for (j = 0, let = gRolling_letters; j < NBR_ROLLING_LETTERS; j++, let++) {
            if (let->number_of_letters >= 0 && let->x_coord == x_coord && let->y_coord == pYcoord) {
                if (new_char != ROLLING_LETTER_LOOP_RANDOM) {
                    let->letters[0] = new_char;
                }
                if (new_char == ' ') {
                    let->letters[0] = ' ';
                }
                let->current_offset = let->number_of_letters * gCurrent_graf_data->save_slot_letter_height;
                let->rolling_type = new_type;
            }
        }
    }
    for (i = len; i < len2; i++) {
        AddRollingLetter(pNew_str[i], gCurrent_graf_data->rolling_letter_x_pitch * i + pXcoord, pYcoord, new_type);
    }
}

// IDA: void __usercall SetRollingCursor(int pSlot_index@<EAX>)
void SetRollingCursor(int pSlot_index) {
    LOG_TRACE("(%d)", pSlot_index);

    gCurrent_cursor = ChangeCharTo(pSlot_index, gCurrent_position, ROLLING_LETTER_LOOP_RANDOM);
}

// IDA: void __usercall BlankSlot(int pIndex@<EAX>, int pName_length@<EDX>, int pVisible_length@<EBX>)
void BlankSlot(int pIndex, int pName_length, int pVisible_length) {
    int i;
    LOG_TRACE("(%d, %d, %d)", pIndex, pName_length, pVisible_length);

    gVisible_length = pVisible_length;
    for (i = 0; i < pName_length; i++) {
        ChangeCharTo(pIndex, i, ' ');
    }
}

// IDA: void __usercall DoRLBackspace(int pSlot_index@<EAX>)
void DoRLBackspace(int pSlot_index) {
    int i;
    int new_len;
    LOG_TRACE("(%d)", pSlot_index);

    if (gCurrent_position != 0) {
        if (strlen(gCurrent_typing) == gCurrent_position) {
            new_len = strlen(gCurrent_typing);
        } else {
            new_len = strlen(gCurrent_typing) - 1;
        }
        ChangeCharTo(pSlot_index, new_len, ' ');
        new_len = strlen(gCurrent_typing) - 1;
        for (i = gCurrent_position - 1; i < new_len; i++) {
            ChangeCharTo(pSlot_index, i, gCurrent_typing[i]);
            gCurrent_typing[i] = gCurrent_typing[i + 1];
        }
        gCurrent_typing[new_len] = 0;
        gCurrent_position = gCurrent_position - 1;
        SetRollingCursor(pSlot_index);
    }
}

// IDA: void __usercall DoRLDelete(int pSlot_index@<EAX>)
void DoRLDelete(int pSlot_index) {
    int i;
    int new_len;
    LOG_TRACE("(%d)", pSlot_index);

    if (gCurrent_position <= ((int)strlen(gCurrent_typing) - 1)) {
        new_len = strlen(gCurrent_typing) - 1;
        ChangeCharTo(pSlot_index, new_len, ' ');
        for (i = gCurrent_position; i < new_len; i++) {
            gCurrent_typing[i] = gCurrent_typing[i + 1];
            ChangeCharTo(pSlot_index, i, gCurrent_typing[i]);
        }
        gCurrent_typing[new_len] = '\0';
        SetRollingCursor(pSlot_index);
    }
}

// IDA: void __usercall DoRLInsert(int pSlot_index@<EAX>)
void DoRLInsert(int pSlot_index) {
    LOG_TRACE("(%d)", pSlot_index);

    gInsert_mode = !gInsert_mode;
}

// IDA: void __usercall DoRLCursorLeft(int pSlot_index@<EAX>)
void DoRLCursorLeft(int pSlot_index) {
    LOG_TRACE("(%d)", pSlot_index);
    if (gCurrent_position != 0) {
        if (strlen(gCurrent_typing) == gCurrent_position) {
            ChangeCharTo(pSlot_index, strlen(gCurrent_typing), ' ');
        } else {
            ChangeCharTo(pSlot_index, gCurrent_position, gCurrent_typing[gCurrent_position]);
        }

        gCurrent_position--;
        SetRollingCursor(pSlot_index);
    }
}

// IDA: void __usercall DoRLCursorRight(int pSlot_index@<EAX>)
void DoRLCursorRight(int pSlot_index) {
    LOG_TRACE("(%d)", pSlot_index);

    if (gCurrent_position < strlen(gCurrent_typing)) {
        ChangeCharTo(pSlot_index, gCurrent_position, gCurrent_typing[gCurrent_position]);
        gCurrent_position++;
        SetRollingCursor(pSlot_index);
    }
}

// IDA: void __usercall DoRLTypeLetter(int pChar@<EAX>, int pSlot_index@<EDX>)
void DoRLTypeLetter(int pChar, int pSlot_index) {
    int i;
    int new_len;
    LOG_TRACE("(%d, %d)", pChar, pSlot_index);

    // v2 = pSlot_index;
    if (pChar >= 32) {
        if (gInsert_mode) {
            new_len = strlen(gCurrent_typing) + 1;
            if (new_len > 100) {
                new_len = 100;
                DoErrorInterface(-1);
            }
            for (i = new_len - 1; i > gCurrent_position; i--) {
                gCurrent_typing[i] = gCurrent_typing[i - 1];
                ChangeCharTo(pSlot_index, i, gCurrent_typing[i]);
            }
        } else if (strlen(gCurrent_typing) == gCurrent_position) {
            new_len = strlen(gCurrent_typing) + 1;
        } else {
            new_len = strlen(gCurrent_typing);
        }
        if (new_len > 100) {
            new_len = 100;
            DoErrorInterface(-1);
        }

        gCurrent_typing[new_len] = 0;
        if (new_len - 1 < gCurrent_position) {
            gCurrent_position = new_len - 1;
        }
        gCurrent_typing[gCurrent_position] = pChar;
        ChangeCharTo(pSlot_index, gCurrent_position, pChar);
        gCurrent_position++;
        SetRollingCursor(pSlot_index);
    }
}

// IDA: void __usercall StopTyping(int pSlot_index@<EAX>)
void StopTyping(int pSlot_index) {
    int i;
    LOG_TRACE("(%d)", pSlot_index);

    for (i = 0; i < gThe_length; i++) {
        if (i < (strlen(gCurrent_typing) - 1)) {
            ChangeCharTo(pSlot_index, i, gCurrent_typing[i]);
        } else {
            ChangeCharTo(pSlot_index, i, ' ');
        }
    }
}

// IDA: void __usercall RevertTyping(int pSlot_index@<EAX>, char *pRevert_str@<EDX>)
void RevertTyping(int pSlot_index, char* pRevert_str) {
    int i;
    LOG_TRACE("(%d, \"%s\")", pSlot_index, pRevert_str);

    for (i = 0; i < gThe_length; i++) {
        ChangeCharTo(pSlot_index, i, i >= strlen(pRevert_str) ? ' ' : pRevert_str[i]);
    }
}

// IDA: void __usercall StartTyping(int pSlot_index@<EAX>, char *pText@<EDX>, int pVisible_length@<EBX>)
void StartTyping(int pSlot_index, char* pText, int pVisible_length) {
    LOG_TRACE("(%d, \"%s\", %d)", pSlot_index, pText, pVisible_length);

    gThe_length = pVisible_length;
    strcpy(gCurrent_typing, pText);
    gVisible_length = pVisible_length;
    gCurrent_position = strlen(gCurrent_typing);
    SetRollingCursor(pSlot_index);
}

// IDA: void __usercall TypeKey(int pSlot_index@<EAX>, char pKey@<EDX>)
void TypeKey(int pSlot_index, char pKey) {
    LOG_TRACE("(%d, %d)", pSlot_index, pKey);

    switch (pKey) {
    case KEY_GRAVE:
        break;
    case KEY_BACKSPACE:
        DoRLBackspace(pSlot_index);
        break;
    case KEY_INSERT:
        DoRLInsert(pSlot_index);
        break;
    case KEY_DELETE:
        DoRLDelete(pSlot_index);
        break;
    case KEY_LEFT:
        DoRLCursorLeft(pSlot_index);
        break;
    case KEY_RIGHT:
        DoRLCursorRight(pSlot_index);
        break;
    default:
        DoRLTypeLetter(PDGetASCIIFromKey(pKey), pSlot_index);
        break;
    }
}

// IDA: void __usercall SetSlotXY(int pSlot_index@<EAX>, int pX_coord@<EDX>, int pY_coord@<EBX>)
void SetSlotXY(int pSlot_index, int pX_coord, int pY_coord) {
    LOG_TRACE("(%d, %d, %d)", pSlot_index, pX_coord, pY_coord);

    gLetter_x_coords[pSlot_index] = pX_coord;
    gLetter_y_coords[pSlot_index] = pY_coord;
}

// IDA: void __usercall GetTypedName(char *pDestn@<EAX>, int pMax_length@<EDX>)
void GetTypedName(char* pDestn, int pMax_length) {
    LOG_TRACE("(\"%s\", %d)", pDestn, pMax_length);

    if (strlen(gCurrent_typing) <= pMax_length) {
        strcpy(pDestn, gCurrent_typing);
    } else {
        memcpy(pDestn, gCurrent_typing, pMax_length);
        pDestn[pMax_length] = 0;
    }
}

// IDA: void __usercall KillCursor(int pSlot_index@<EAX>)
void KillCursor(int pSlot_index) {
    int x_coord;
    int y_coord;
    int i;
    int j;
    tRolling_letter* let;
    tRolling_type new_type;
    LOG_TRACE("(%d)", pSlot_index);

    if (gCurrent_position < gVisible_length && gCurrent_position >= 0) {
        y_coord = gLetter_y_coords[pSlot_index];
        x_coord = gCurrent_graf_data->rolling_letter_x_pitch * gCurrent_position + gLetter_x_coords[pSlot_index];
        for (i = 0; i < NBR_ROLLING_LETTERS; i++) {
            let = &gRolling_letters[i];
            if (let->number_of_letters >= 0 && x_coord == let->x_coord && y_coord == let->y_coord) {
                gRolling_letters[i].number_of_letters = -1;
                break;
            }
        }
    }
}

// IDA: void __cdecl EdgeTriggerModeOn()
void EdgeTriggerModeOn(void) {
    gEdge_trigger_mode = 1;
}

// IDA: void __cdecl EdgeTriggerModeOff()
void EdgeTriggerModeOff(void) {
    gEdge_trigger_mode = 0;
}
