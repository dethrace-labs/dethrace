#ifndef _LOADSAVE_H_
#define _LOADSAVE_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 647
// EAX: pIndex
void CorrectLoadByteOrdering(int pIndex);

// Offset: 648
// Size: 128
// EAX: pSaved_game
tU32 CalcLSChecksum(tSave_game* pSaved_game);

// Offset: 776
// Size: 391
void LoadSavedGames();

// Offset: 1168
// Size: 94
void DisposeSavedGames();

// Offset: 1264
// Size: 701
// EAX: pSlot_index
void LoadTheGame(int pSlot_index);

// Offset: 1968
// Size: 331
void StartRollingSaveNamesIn();

// Offset: 2300
// Size: 39
void LoadStart();

// Offset: 2340
// Size: 415
// EAX: pSave_allowed
int DoLoadGame(int pSave_allowed);

// Offset: 2756
// Size: 647
// EAX: pIndex
void CorrectSaveByteOrdering(int pIndex);

// Offset: 3404
// Size: 229
// EAX: pSlot_number
void SaveTheGame(int pSlot_number);

// Offset: 3636
// Size: 77
int ConfirmMidGameSave();

// Offset: 3716
// Size: 492
// EAX: pSave_record
void MakeSavedGame(tSave_game** pSave_record);

// Offset: 4208
// Size: 39
void SaveStart();

// Offset: 4248
// Size: 152
// EAX: pStarting_to_type
// EDX: pCurrent_choice
// EBX: pString
// ECX: pMax_length
void GetSaveName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length);

// Offset: 4400
// Size: 188
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int SaveDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 4588
// Size: 368
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int SaveGoAhead(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 4956
// Size: 368
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int SaveEscape(int* pCurrent_choice, int* pCurrent_mode);

// Offset: 5324
// Size: 92
// EAX: pDefault_choice
int SaveGameInterface(int pDefault_choice);

// Offset: 5416
// Size: 181
// EAX: pSave_allowed
void DoSaveGame(int pSave_allowed);

#endif
