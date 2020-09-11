#ifndef _LOADSAVE_H_
#define _LOADSAVE_H_

#include "br_types.h"
#include "dr_types.h"

void CorrectLoadByteOrdering(int pIndex);

tU32 CalcLSChecksum(tSave_game* pSaved_game);

void LoadSavedGames();

void DisposeSavedGames();

void LoadTheGame(int pSlot_index);

void StartRollingSaveNamesIn();

void LoadStart();

int DoLoadGame();

void CorrectSaveByteOrdering(int pIndex);

void SaveTheGame(int pSlot_number);

int ConfirmMidGameSave();

void MakeSavedGame(tSave_game** pSave_record);

void SaveStart();

void GetSaveName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length);

int SaveDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

int SaveGoAhead(int* pCurrent_choice, int* pCurrent_mode);

int SaveEscape(int* pCurrent_choice, int* pCurrent_mode);

int SaveGameInterface(int pDefault_choice);

void DoSaveGame(int pSave_allowed);

#endif
