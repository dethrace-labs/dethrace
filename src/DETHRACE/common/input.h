#ifndef _INPUT_H_
#define _INPUT_H_

#include "br_types.h"
#include "dr_types.h"

extern int gEdge_trigger_mode;
extern tU32 gLast_poll_keys;
extern int gInsert_mode;
extern int gGo_ahead_keys[3];
extern tJoy_array gJoy_array;
extern tKey_array gKey_array;
extern int gKey_poll_counter;
extern tRolling_letter* gRolling_letters;
extern int gCurrent_cursor;
extern int gCurrent_position;
extern int gLetter_x_coords[15];
extern int gVisible_length;
extern int gLetter_y_coords[15];
extern int gThe_key;
extern tU32 gLast_key_down_time;
extern int gThe_length;
extern tU32 gLast_roll;
extern int gLast_key_down;
extern int gKey_mapping[67];
extern char gCurrent_typing[110];

void SetJoystickArrays(int* pKeys, int pMark);

void PollKeys();

void CyclePollKeys();

void ResetPollKeys();

void CheckKeysForMouldiness();

int EitherMouseButtonDown();

tKey_down_result PDKeyDown2(int pKey_index);

int PDKeyDown(int pKey_index);

int PDKeyDown3(int pKey_index);

int PDAnyKeyDown();

int AnyKeyDown();

tU32* KevKeyService();

int OldKeyIsDown(int pKey_index);

int KeyIsDown(int pKey_index);

void WaitForNoKeys();

void WaitForAKey();

int CmdKeyDown(int pFKey_ID, int pCmd_key_ID);

void GetMousePosition(int* pX_coord, int* pY_coord);

void InitRollingLetters();

void EndRollingLetters();

int AddRollingLetter(char pChar, int pX, int pY, tRolling_type rolling_type);

void AddRollingString(char* pStr, int pX, int pY, tRolling_type rolling_type);

void AddRollingNumber(tU32 pNumber, int pWidth, int pX, int pY);

void RollLettersIn();

int ChangeCharTo(int pSlot_index, int pChar_index, char pNew_char);

void ChangeTextTo(int pXcoord, int pYcoord, char* pNew_str, char* pOld_str);

void SetRollingCursor(int pSlot_index);

void BlankSlot(int pIndex, int pName_length, int pVisible_length);

void DoRLBackspace(int pSlot_index);

void DoRLDelete(int pSlot_index);

void DoRLInsert(int pSlot_index);

void DoRLCursorLeft(int pSlot_index);

void DoRLCursorRight(int pSlot_index);

void DoRLTypeLetter(int pChar, int pSlot_index);

void StopTyping(int pSlot_index);

void RevertTyping(int pSlot_index, char* pRevert_str);

void StartTyping(int pSlot_index, char* pText, int pVisible_length);

void TypeKey(int pSlot_index, char pKey);

void SetSlotXY(int pSlot_index, int pX_coord, int pY_coord);

void GetTypedName(char* pDestn, int pMax_length);

void KillCursor(int pSlot_index);

void EdgeTriggerModeOn();

void EdgeTriggerModeOff();

#endif
