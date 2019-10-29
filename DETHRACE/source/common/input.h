#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 996
// EAX: pKeys
// EDX: pMark
void SetJoystickArrays(int *pKeys);

// Offset: 996
// Size: 82
void PollKeys();

// Offset: 1080
// Size: 127
void CyclePollKeys();

// Offset: 1208
// Size: 116
void ResetPollKeys();

// Offset: 1324
// Size: 76
void CheckKeysForMouldiness();

// Offset: 1400
// Size: 82
int EitherMouseButtonDown();

// Offset: 1484
// Size: 214
// EAX: pKey_index
tKey_down_result PDKeyDown2(int pKey_index);

// Offset: 1700
// Size: 130
// EAX: pKey_index
int PDKeyDown(int pKey_index);

// Offset: 1832
// Size: 117
// EAX: pKey_index
int PDKeyDown3(int pKey_index);

// Offset: 1952
// Size: 208
int PDAnyKeyDown();

// Offset: 2160
// Size: 88
int AnyKeyDown();

// Offset: 2248
// Size: 451
tU32* KevKeyService();

// Offset: 2700
// Size: 145
// EAX: pKey_index
int OldKeyIsDown(int pKey_index);

// Offset: 2848
// Size: 157
// EAX: pKey_index
int KeyIsDown(int pKey_index);

// Offset: 3008
// Size: 64
void WaitForNoKeys();

// Offset: 3072
// Size: 67
void WaitForAKey();

// Offset: 3140
// Size: 103
// EAX: pFKey_ID
// EDX: pCmd_key_ID
int CmdKeyDown(int pFKey_ID, int pCmd_key_ID);

// Offset: 3244
// Size: 206
// EAX: pX_coord
// EDX: pY_coord
void GetMousePosition(int *pX_coord, int *pY_coord);

// Offset: 3452
// Size: 125
void InitRollingLetters();

// Offset: 3580
// Size: 48
void EndRollingLetters();

// Offset: 3628
// Size: 384
// EAX: pChar
// EDX: pX
// EBX: pY
// ECX: rolling_type
int AddRollingLetter(char pChar, int pX, int pY, tRolling_type rolling_type, int let);

// Offset: 4012
// Size: 111
// EAX: pStr
// EDX: pX
// EBX: pY
// ECX: rolling_type
void AddRollingString(char *pStr, int pX, int pY, tRolling_type rolling_type, int i);

// Offset: 4124
// Size: 85
// EAX: pNumber
// EDX: pWidth
// EBX: pX
// ECX: pY
void AddRollingNumber(tU32 pNumber, int pWidth);

// Offset: 4212
// Size: 651
void RollLettersIn();

// Offset: 4864
// Size: 345
// EAX: pSlot_index
// EDX: pChar_index
// EBX: pNew_char
int ChangeCharTo(int pSlot_index, int pChar_index, char pNew_char);

// Offset: 5212
// Size: 413
// EAX: pXcoord
// EDX: pYcoord
// EBX: pNew_str
// ECX: pOld_str
void ChangeTextTo(int pXcoord, int pYcoord, char *pNew_str, char *pOld_str);

// Offset: 5628
// Size: 61
// EAX: pSlot_index
void SetRollingCursor(int pSlot_index);

// Offset: 5692
// Size: 90
// EAX: pIndex
// EDX: pName_length
// EBX: pVisible_length
void BlankSlot(int pIndex, int pName_length, int pVisible_length);

// Offset: 5784
// Size: 232
// EAX: pSlot_index
void DoRLBackspace(int pSlot_index);

// Offset: 6016
// Size: 190
// EAX: pSlot_index
void DoRLDelete(int pSlot_index);

// Offset: 6208
// Size: 70
// EAX: pSlot_index
void DoRLInsert(int pSlot_index);

// Offset: 6280
// Size: 137
// EAX: pSlot_index
void DoRLCursorLeft(int pSlot_index);

// Offset: 6420
// Size: 101
// EAX: pSlot_index
void DoRLCursorRight(int pSlot_index);

// Offset: 6524
// Size: 323
// EAX: pChar
// EDX: pSlot_index
void DoRLTypeLetter(int pChar, int pSlot_index);

// Offset: 6848
// Size: 123
// EAX: pSlot_index
void StopTyping(int pSlot_index);

// Offset: 6972
// Size: 121
// EAX: pSlot_index
// EDX: pRevert_str
void RevertTyping(int pSlot_index, char *pRevert_str);

// Offset: 7096
// Size: 91
// EAX: pSlot_index
// EDX: pText
// EBX: pVisible_length
void StartTyping(int pSlot_index, char *pText, int pVisible_length);

// Offset: 7188
// Size: 176
// EAX: pSlot_index
// EDX: pKey
void TypeKey(int pSlot_index, char pKey);

// Offset: 7364
// Size: 69
// EAX: pSlot_index
// EDX: pX_coord
// EBX: pY_coord
void SetSlotXY(int pSlot_index, int pX_coord, int pY_coord);

// Offset: 7436
// Size: 93
// EAX: pDestn
// EDX: pMax_length
void GetTypedName(char *pDestn, int pMax_length);

// Offset: 7532
// Size: 212
// EAX: pSlot_index
void KillCursor(int pSlot_index);

// Offset: 7744
// Size: 44
void EdgeTriggerModeOn();

// Offset: 7788
// Size: 44
void EdgeTriggerModeOff();

