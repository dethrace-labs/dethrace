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
void PollKeys() {
    gKey_poll_counter++;
    PDSetKeyArray(gKey_array, gKey_poll_counter);
    SetJoystickArrays(gKey_array, gKey_poll_counter);
    gLast_poll_keys = PDGetTotalTime();
}

// IDA: void __cdecl CyclePollKeys()
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

// IDA: void __cdecl ResetPollKeys()
void ResetPollKeys() {
    memset(gKey_array, 0, sizeof(gKey_array));
    memset(gJoy_array, 0, sizeof(gJoy_array));
}

// IDA: void __cdecl CheckKeysForMouldiness()
void CheckKeysForMouldiness() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl EitherMouseButtonDown()
int EitherMouseButtonDown() {
    int but_2;
    return 0;
}

// IDA: tKey_down_result __usercall PDKeyDown2@<EAX>(int pKey_index@<EAX>)
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

// IDA: int __usercall PDKeyDown@<EAX>(int pKey_index@<EAX>)
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

// IDA: int __usercall PDKeyDown3@<EAX>(int pKey_index@<EAX>)
int PDKeyDown3(int pKey_index) {
    int last_key_down_time;
    int last_key_down;
    tKey_down_result result;
    LOG_TRACE("(%d)", pKey_index);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDAnyKeyDown()
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

// IDA: int __cdecl AnyKeyDown()
int AnyKeyDown() {
    int the_key;
    the_key = PDAnyKeyDown();
    if ((the_key != -1 && the_key != 4) || EitherMouseButtonDown() != 0) {
        return 1;
    }
    return 0;
}

// IDA: tU32* __cdecl KevKeyService()
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

// IDA: int __usercall OldKeyIsDown@<EAX>(int pKey_index@<EAX>)
int OldKeyIsDown(int pKey_index) {
    int i;
    LOG_TRACE("(%d)", pKey_index);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyIsDown@<EAX>(int pKey_index@<EAX>)
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

// IDA: void __cdecl WaitForNoKeys()
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

// IDA: void __cdecl WaitForAKey()
void WaitForAKey() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CmdKeyDown@<EAX>(int pFKey_ID@<EAX>, int pCmd_key_ID@<EDX>)
int CmdKeyDown(int pFKey_ID, int pCmd_key_ID) {
    return KeyIsDown(pFKey_ID) || (KeyIsDown(KEYMAP_LCTRL) && KeyIsDown(pCmd_key_ID));
}

// IDA: void __usercall GetMousePosition(int *pX_coord@<EAX>, int *pY_coord@<EDX>)
void GetMousePosition(int* pX_coord, int* pY_coord) {
    int x_left_margin;
    int x_right_margin;
    int y_top_margin;
    int y_bottom_margin;
    LOG_TRACE("(%p, %p)", pX_coord, pY_coord);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitRollingLetters()
void InitRollingLetters() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EndRollingLetters()
void EndRollingLetters() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall AddRollingLetter@<EAX>(char pChar@<EAX>, int pX@<EDX>, int pY@<EBX>, tRolling_type rolling_type@<ECX>)
int AddRollingLetter(char pChar, int pX, int pY, tRolling_type rolling_type) {
    tRolling_letter* let;
    int i;
    int number_of_letters;
    LOG_TRACE("(%d, %d, %d, %d)", pChar, pX, pY, rolling_type);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddRollingString(char *pStr@<EAX>, int pX@<EDX>, int pY@<EBX>, tRolling_type rolling_type@<ECX>)
void AddRollingString(char* pStr, int pX, int pY, tRolling_type rolling_type) {
    int i;
    LOG_TRACE("(\"%s\", %d, %d, %d)", pStr, pX, pY, rolling_type);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AddRollingNumber(tU32 pNumber@<EAX>, int pWidth@<EDX>, int pX@<EBX>, int pY@<ECX>)
void AddRollingNumber(tU32 pNumber, int pWidth, int pX, int pY) {
    char the_string[32];
    LOG_TRACE("(%d, %d, %d, %d)", pNumber, pWidth, pX, pY);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RollLettersIn()
void RollLettersIn() {
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
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetRollingCursor(int pSlot_index@<EAX>)
void SetRollingCursor(int pSlot_index) {
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BlankSlot(int pIndex@<EAX>, int pName_length@<EDX>, int pVisible_length@<EBX>)
void BlankSlot(int pIndex, int pName_length, int pVisible_length) {
    int i;
    LOG_TRACE("(%d, %d, %d)", pIndex, pName_length, pVisible_length);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoRLBackspace(int pSlot_index@<EAX>)
void DoRLBackspace(int pSlot_index) {
    int i;
    int new_len;
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoRLDelete(int pSlot_index@<EAX>)
void DoRLDelete(int pSlot_index) {
    int i;
    int new_len;
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoRLInsert(int pSlot_index@<EAX>)
void DoRLInsert(int pSlot_index) {
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoRLCursorLeft(int pSlot_index@<EAX>)
void DoRLCursorLeft(int pSlot_index) {
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoRLCursorRight(int pSlot_index@<EAX>)
void DoRLCursorRight(int pSlot_index) {
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoRLTypeLetter(int pChar@<EAX>, int pSlot_index@<EDX>)
void DoRLTypeLetter(int pChar, int pSlot_index) {
    int i;
    int new_len;
    LOG_TRACE("(%d, %d)", pChar, pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StopTyping(int pSlot_index@<EAX>)
void StopTyping(int pSlot_index) {
    int i;
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RevertTyping(int pSlot_index@<EAX>, char *pRevert_str@<EDX>)
void RevertTyping(int pSlot_index, char* pRevert_str) {
    int i;
    LOG_TRACE("(%d, \"%s\")", pSlot_index, pRevert_str);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StartTyping(int pSlot_index@<EAX>, char *pText@<EDX>, int pVisible_length@<EBX>)
void StartTyping(int pSlot_index, char* pText, int pVisible_length) {
    LOG_TRACE("(%d, \"%s\", %d)", pSlot_index, pText, pVisible_length);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall TypeKey(int pSlot_index@<EAX>, char pKey@<EDX>)
void TypeKey(int pSlot_index, char pKey) {
    LOG_TRACE("(%d, %d)", pSlot_index, pKey);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetSlotXY(int pSlot_index@<EAX>, int pX_coord@<EDX>, int pY_coord@<EBX>)
void SetSlotXY(int pSlot_index, int pX_coord, int pY_coord) {
    LOG_TRACE("(%d, %d, %d)", pSlot_index, pX_coord, pY_coord);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetTypedName(char *pDestn@<EAX>, int pMax_length@<EDX>)
void GetTypedName(char* pDestn, int pMax_length) {
    LOG_TRACE("(\"%s\", %d)", pDestn, pMax_length);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EdgeTriggerModeOn()
void EdgeTriggerModeOn() {
    gEdge_trigger_mode = 1;
}

// IDA: void __cdecl EdgeTriggerModeOff()
void EdgeTriggerModeOff() {
    gEdge_trigger_mode = 0;
}
