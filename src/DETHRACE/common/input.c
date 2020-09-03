#include "input.h"

#include "common/globvars.h"
#include "pc-dos/dossys.h"
#include "utility.h"
#include <stdlib.h>

tJoy_array gJoy_array;
tKey_array gKey_array;
int gKey_poll_counter;
tRolling_letter* gRolling_letters;
tU32 gLast_poll_keys = 0;
int gCurrent_cursor;
int gCurrent_position;
int gInsert_mode;
int gLetter_x_coords[15];
int gEdge_trigger_mode = 0;
int gVisible_length;
int gLetter_y_coords[15];
int gThe_key;
tU32 gLast_key_down_time;
int gThe_length;
tU32 gLast_roll;
int gLast_key_down;
int gGo_ahead_keys[3] = { 51, 52, 106 }; // enter, return, space
int gKey_mapping[67];
char gCurrent_typing[110];

// Offset: 0
// Size: 996
// EAX: pKeys
// EDX: pMark
void SetJoystickArrays(int* pKeys, int pMark) {
    int i;
    tS32 joyX;
    tS32 joyY;
    static tS32 old_joy1X;
    static tS32 old_joy1Y;
    static tS32 old_joy2X;
    static tS32 old_joy2Y;
}

// Offset: 996
// Size: 82
void PollKeys() {
    gKey_poll_counter++;
    PDSetKeyArray(gKey_array, gKey_poll_counter);
    SetJoystickArrays(gKey_array, gKey_poll_counter);
    gLast_poll_keys = PDGetTotalTime();
}

// Offset: 1080
// Size: 127
void CyclePollKeys() {
    int i;
    for (i = 0; i < 123; i++) {

        if (gKey_array[i] > gKey_poll_counter) {
            gKey_array[i] = 0;
            if (i > 115) {
                // TOOD: this does _something_, but we cannot figure out what..
                // eax+1361D4h = -1
                // gFonts[20].width_table[v1 + 141] = -1;
            }
        }
    }
    gKey_poll_counter = 0;
}

// Offset: 1208
// Size: 116
void ResetPollKeys() {
    memset(gKey_array, 0, sizeof(gKey_array));
    memset(gJoy_array, 0, sizeof(gJoy_array));
}

// Offset: 1324
// Size: 76
void CheckKeysForMouldiness() {
    NOT_IMPLEMENTED();
}

// Offset: 1400
// Size: 82
int EitherMouseButtonDown() {
    int but_2;
    return 0;
}

// Offset: 1484
// Size: 214
// EAX: pKey_index
tKey_down_result PDKeyDown2(int pKey_index) {
    tU32 the_time;
    if (/*!s3_timer_started_maybe[0] ||*/ (PDGetTotalTime() - gLast_poll_keys) > 500) {
        ResetPollKeys();
        CyclePollKeys();
        PollKeys();
    }

    if (!gEdge_trigger_mode) {
        return gKey_array[pKey_index];
    }
    the_time = PDGetTotalTime();
    if (gKey_array[pKey_index]) {
        if (pKey_index == gLast_key_down) {
            if ((the_time - gLast_key_down_time) < 300) {
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
    } else {
        if (pKey_index == gLast_key_down) {
            gLast_key_down_time = gKey_array[pKey_index];
            gLast_key_down = -1;
        }
        return tKey_down_no;
    }
}

// Offset: 1700
// Size: 130
// EAX: pKey_index
int PDKeyDown(int pKey_index) {
    tKey_down_result result;
    result = PDKeyDown2(pKey_index);
    if (!gEdge_trigger_mode || pKey_index <= 10) {
        return result != 0;
    }
    if (result != tKey_down_yes && result != tKey_down_repeat) {
        return 0;
    }
    return 1;
}

// Offset: 1832
// Size: 117
// EAX: pKey_index
int PDKeyDown3(int pKey_index) {
    int last_key_down_time;
    int last_key_down;
    tKey_down_result result;
    NOT_IMPLEMENTED();
}

// Offset: 1952
// Size: 208
int PDAnyKeyDown() {
    int i;
    tKey_down_result result;

    if (/*!s3_timer_started_maybe[0] ||*/ (PDGetTotalTime() - gLast_poll_keys) > 500) {
        ResetPollKeys();
        CyclePollKeys();
        PollKeys();
    }
    for (i = 122; i >= 0; i--) {
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
            default:
                break;
            }
        }
    }

    if (gEdge_trigger_mode) {
        gLast_key_down = -1;
        gLast_key_down_time = 0;
    }
    return -1;
}

// Offset: 2160
// Size: 88
int AnyKeyDown() {
    int the_key;
    the_key = PDAnyKeyDown();
    if ((the_key != -1 && the_key != 4) || EitherMouseButtonDown() != 0) {
        return 1;
    }
    return 0;
}

// Offset: 2248
// Size: 451
tU32* KevKeyService() {
    static tU32 sum = 0;
    static tU32 code = 0;
    static tU32 code2 = 0;
    static int last_key = -1;
    static int last_single_key = -1;
    static tU32 last_time = 0;
    static tU32 return_val[2];
    tU32 keys;

    keys = gKeys_pressed;
    //printf("key: %d, %lx, %lx\n", sizeof(long), keys, code2);
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
        code = (code >> 17) + 16 * code;
        code2 = (code2 >> 29) + keys * keys + 8 * code2;
        //printf("accumulate: keys=%lx, sum=%lx, code=%lx, code2=%lx\n", keys, sum, code, code2);
        last_time = PDGetTotalTime();
    } else if (PDGetTotalTime() > (last_time + 1000)) {
        return_val[0] = ((code >> 11) + (sum << 21));
        return_val[1] = code2;
        //printf("final value: code=%lx, code2=%lx\n", return_val[0], return_val[1]);
        code = 0;
        code2 = 0;
        sum = 0;
    }
    last_key = keys;
    return return_val;
}

// Offset: 2700
// Size: 145
// EAX: pKey_index
int OldKeyIsDown(int pKey_index) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 2848
// Size: 157
// EAX: pKey_index
int KeyIsDown(int pKey_index) {
    int i;

    if (PDGetTotalTime() - gLast_poll_keys > 500) {
        ResetPollKeys();
        CyclePollKeys();
        PollKeys();
    }
    if (pKey_index == -2) {
        return 1;
    }
    if (pKey_index != -1) {
        return gKey_array[gKey_mapping[pKey_index]];
    }

    for (i = 0; i < 3; i++) {
        if (gKey_array[gGo_ahead_keys[i]]) {
            return 1;
        }
    }

    return 0;
}

// Offset: 3008
// Size: 64
void WaitForNoKeys() {
    LOG_TRACE("()");

    int key_result = 0; //JeffH added

    while (1) {
        CheckQuit();
        key_result = PDAnyKeyDown();
        if ((key_result == -1 || key_result == 4) && !EitherMouseButtonDown()) {
            break;
        }
    }
    CheckQuit();
}

// Offset: 3072
// Size: 67
void WaitForAKey() {
    NOT_IMPLEMENTED();
}

// Offset: 3140
// Size: 103
// EAX: pFKey_ID
// EDX: pCmd_key_ID
int CmdKeyDown(int pFKey_ID, int pCmd_key_ID) {
    return KeyIsDown(pFKey_ID) || (KeyIsDown(KEYMAP_LCTRL) && KeyIsDown(pCmd_key_ID));
}

// Offset: 3244
// Size: 206
// EAX: pX_coord
// EDX: pY_coord
void GetMousePosition(int* pX_coord, int* pY_coord) {
    int x_left_margin;
    int x_right_margin;
    int y_top_margin;
    int y_bottom_margin;
    NOT_IMPLEMENTED();
}

// Offset: 3452
// Size: 125
void InitRollingLetters() {
    NOT_IMPLEMENTED();
}

// Offset: 3580
// Size: 48
void EndRollingLetters() {
    NOT_IMPLEMENTED();
}

// Offset: 3628
// Size: 384
// EAX: pChar
// EDX: pX
// EBX: pY
// ECX: rolling_type
int AddRollingLetter(char pChar, int pX, int pY, tRolling_type rolling_type) {
    tRolling_letter* let;
    int i;
    int number_of_letters;
    NOT_IMPLEMENTED();
}

// Offset: 4012
// Size: 111
// EAX: pStr
// EDX: pX
// EBX: pY
// ECX: rolling_type
void AddRollingString(char* pStr, int pX, int pY, tRolling_type rolling_type) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 4124
// Size: 85
// EAX: pNumber
// EDX: pWidth
// EBX: pX
// ECX: pY
void AddRollingNumber(tU32 pNumber, int pWidth, int pX, int pY) {
    char the_string[32];
    NOT_IMPLEMENTED();
}

// Offset: 4212
// Size: 651
void RollLettersIn() {
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
    NOT_IMPLEMENTED();
}

// Offset: 4864
// Size: 345
// EAX: pSlot_index
// EDX: pChar_index
// EBX: pNew_char
int ChangeCharTo(int pSlot_index, int pChar_index, char pNew_char) {
    int x_coord;
    int y_coord;
    int i;
    int j;
    tRolling_letter* let;
    tRolling_type new_type;
    NOT_IMPLEMENTED();
}

// Offset: 5212
// Size: 413
// EAX: pXcoord
// EDX: pYcoord
// EBX: pNew_str
// ECX: pOld_str
void ChangeTextTo(int pXcoord, int pYcoord, char* pNew_str, char* pOld_str) {
    int x_coord;
    int i;
    int len;
    int len2;
    int j;
    tRolling_letter* let;
    tRolling_type new_type;
    char new_char;
    NOT_IMPLEMENTED();
}

// Offset: 5628
// Size: 61
// EAX: pSlot_index
void SetRollingCursor(int pSlot_index) {
    NOT_IMPLEMENTED();
}

// Offset: 5692
// Size: 90
// EAX: pIndex
// EDX: pName_length
// EBX: pVisible_length
void BlankSlot(int pIndex, int pName_length, int pVisible_length) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 5784
// Size: 232
// EAX: pSlot_index
void DoRLBackspace(int pSlot_index) {
    int i;
    int new_len;
    NOT_IMPLEMENTED();
}

// Offset: 6016
// Size: 190
// EAX: pSlot_index
void DoRLDelete(int pSlot_index) {
    int i;
    int new_len;
    NOT_IMPLEMENTED();
}

// Offset: 6208
// Size: 70
// EAX: pSlot_index
void DoRLInsert(int pSlot_index) {
    NOT_IMPLEMENTED();
}

// Offset: 6280
// Size: 137
// EAX: pSlot_index
void DoRLCursorLeft(int pSlot_index) {
    NOT_IMPLEMENTED();
}

// Offset: 6420
// Size: 101
// EAX: pSlot_index
void DoRLCursorRight(int pSlot_index) {
    NOT_IMPLEMENTED();
}

// Offset: 6524
// Size: 323
// EAX: pChar
// EDX: pSlot_index
void DoRLTypeLetter(int pChar, int pSlot_index) {
    int i;
    int new_len;
    NOT_IMPLEMENTED();
}

// Offset: 6848
// Size: 123
// EAX: pSlot_index
void StopTyping(int pSlot_index) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 6972
// Size: 121
// EAX: pSlot_index
// EDX: pRevert_str
void RevertTyping(int pSlot_index, char* pRevert_str) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7096
// Size: 91
// EAX: pSlot_index
// EDX: pText
// EBX: pVisible_length
void StartTyping(int pSlot_index, char* pText, int pVisible_length) {
    NOT_IMPLEMENTED();
}

// Offset: 7188
// Size: 176
// EAX: pSlot_index
// EDX: pKey
void TypeKey(int pSlot_index, char pKey) {
    NOT_IMPLEMENTED();
}

// Offset: 7364
// Size: 69
// EAX: pSlot_index
// EDX: pX_coord
// EBX: pY_coord
void SetSlotXY(int pSlot_index, int pX_coord, int pY_coord) {
    NOT_IMPLEMENTED();
}

// Offset: 7436
// Size: 93
// EAX: pDestn
// EDX: pMax_length
void GetTypedName(char* pDestn, int pMax_length) {
    NOT_IMPLEMENTED();
}

// Offset: 7532
// Size: 212
// EAX: pSlot_index
void KillCursor(int pSlot_index) {
    int x_coord;
    int y_coord;
    int i;
    int j;
    tRolling_letter* let;
    tRolling_type new_type;
    NOT_IMPLEMENTED();
}

// Offset: 7744
// Size: 44
void EdgeTriggerModeOn() {
    gEdge_trigger_mode = 1;
}

// Offset: 7788
// Size: 44
void EdgeTriggerModeOff() {
    gEdge_trigger_mode = 0;
}
