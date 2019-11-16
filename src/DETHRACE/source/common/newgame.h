#ifndef _NEWGAME_H_
#define _NEWGAME_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 149
void StartRollingPlayerNamesIn();

// Offset: 152
// Size: 314
void FrankAnneStart1();

// Offset: 468
// Size: 150
void FrankAnneStart2();

// Offset: 620
// Size: 68
// EAX: pStarting_to_type
// EDX: pCurrent_choice
// EBX: pString
// ECX: pMax_length
void GetPlayerName(int pStarting_to_type, int pCurrent_choice, char *pString, int *pMax_length);

// Offset: 688
// Size: 203
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int FrankAnneDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 892
// Size: 139
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void FrankAnneDraw(int pCurrent_choice, int pCurrent_mode);

// Offset: 1032
// Size: 123
int FrankieOrAnnie();

// Offset: 1156
// Size: 88
int SelectSkillLevel();

// Offset: 1244
// Size: 323
int DoOnePlayerStart();

// Offset: 1568
// Size: 263
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NewNetGameUp(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 1832
// Size: 263
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NewNetGameDown(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 2096
// Size: 70
// EAX: pIndex
void DisposeJoinableGame(int pIndex);

// Offset: 2168
// Size: 123
// EAX: pX
// EDX: pY_index
// EBX: pFont_index
// ECX: pText
void DrawAnItem(int pX, int pY_index, int pFont_index, char *pText);

// Offset: 2292
// Size: 119
// EAX: pStr_index
// EDX: pX
void DrawColumnHeading(int pStr_index, int pX);

// Offset: 2412
// Size: 1821
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawGames(int pCurrent_choice, int pCurrent_mode);

// Offset: 4236
// Size: 85
void InitGamesToJoin();

// Offset: 4324
// Size: 95
// EAX: pExemption
void DisposeJoinList(int pExemption);

// Offset: 4420
// Size: 227
// EAX: pGame
void AddToJoinList(tNet_game_details *pGame);

// Offset: 4648
// Size: 111
void NewNetStart1();

// Offset: 4760
// Size: 61
void NewNetStart2();

// Offset: 4824
// Size: 62
// EAX: pStarting_to_type
// EDX: pCurrent_choice
// EBX: pString
// ECX: pMax_length
void NewNetGetName(int pStarting_to_type, int pCurrent_choice, char *pString, int *pMax_length);

// Offset: 4888
// Size: 72
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int NewNetDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 4960
// Size: 72
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NewNetGoAhead(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 5032
// Size: 287
// EAX: pGame_to_join
tJoin_or_host_result JoinOrHostGame(tNet_game_details **pGame_to_join);

// Offset: 5320
// Size: 223
// EAX: pGame_options
void GetNetOptions(tNet_game_options *pGame_options);

// Offset: 5544
// Size: 234
// EAX: pGame_options
void SetNetOptions(tNet_game_options *pGame_options);

// Offset: 5780
// Size: 72
// EAX: pIndex
void NetPlayCheckboxOn2(int pIndex);

// Offset: 5852
// Size: 72
// EAX: pIndex
void NetPlayCheckboxOff2(int pIndex);

// Offset: 5924
// Size: 75
// EAX: pIndex
void NetPlayCheckboxOn(int pIndex);

// Offset: 6000
// Size: 75
// EAX: pIndex
void NetPlayCheckboxOff(int pIndex);

// Offset: 6076
// Size: 81
// EAX: pIndex
// EDX: pValue
void NetPlayRadioOn2(int pIndex, int pValue);

// Offset: 6160
// Size: 81
// EAX: pIndex
// EDX: pValue
void NetPlayRadioOff2(int pIndex, int pValue);

// Offset: 6244
// Size: 79
// EAX: pIndex
// EDX: pValue
void NetPlayRadioOn(int pIndex, int pValue);

// Offset: 6324
// Size: 79
// EAX: pIndex
// EDX: pValue
void NetPlayRadioOff(int pIndex, int pValue);

// Offset: 6404
// Size: 229
void DrawNOptInitialRadios();

// Offset: 6636
// Size: 84
// EAX: pIndex
// EDX: pNew_value
void NetRadioChanged(int pIndex, int pNew_value);

// Offset: 6720
// Size: 116
// EAX: pIndex
void NetCheckboxChanged(int pIndex);

// Offset: 6836
// Size: 153
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NetOptLeft(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 6992
// Size: 157
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NetOptRight(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 7152
// Size: 148
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NetOptUp(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 7300
// Size: 148
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NetOptDown(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 7448
// Size: 213
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int NetRadioClick(int *pCurrent_choice, int *pCurrent_mode, int pX_offset, int pY_offset);

// Offset: 7664
// Size: 142
void RevertToDefaults();

// Offset: 7808
// Size: 156
void DefaultNetSettings();

// Offset: 7964
// Size: 98
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NetOptGoAhead(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 8064
// Size: 202
// EAX: pIndex
// EDX: pColour_value
void NetPlotAGraphBox(int pIndex, int pColour_value);

// Offset: 8268
// Size: 50
// EAX: pIndex
void NetDrawAGraphBox(int pIndex);

// Offset: 8320
// Size: 47
// EAX: pIndex
void NetEraseAGraphBox(int pIndex);

// Offset: 8368
// Size: 80
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawNetOptBox(int pCurrent_choice, int pCurrent_mode);

// Offset: 8448
// Size: 111
// EAX: pGame_options
void DoNetOptions(tNet_game_options *pGame_options);

// Offset: 8560
// Size: 190
// EAX: pGame_type
// EDX: pGame_options
void SetOptions(tNet_game_type pGame_type, tNet_game_options *pGame_options);

// Offset: 8992
// Size: 44
void DrawNetChooseInitial();

// Offset: 9036
// Size: 250
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NetChooseGoAhead(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 9288
// Size: 182
// EAX: pIndex
// EDX: pColour_value
void PlotAGraphBox(int pIndex, int pColour_value);

// Offset: 9472
// Size: 50
// EAX: pIndex
void DrawAGraphBox(int pIndex);

// Offset: 9524
// Size: 47
// EAX: pIndex
void EraseAGraphBox(int pIndex);

// Offset: 9572
// Size: 719
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawNetChoose(int pCurrent_choice, int pCurrent_mode);

// Offset: 10292
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NetChooseLR(int *pCurrent_choice, int *pCurrent_mode);

// Offset: 10356
// Size: 190
// EAX: pGame_type
// EDX: pGame_options
void SetGameTarget(tNet_game_type *pGame_type, tNet_game_options *pGame_options);

// Offset: 10548
// Size: 383
// EAX: pGame_type
// EDX: pGame_options
// EBX: pRace_index
int NetGameChoices(tNet_game_type *pGame_type, tNet_game_options *pGame_options, int *pRace_index);

// Offset: 10932
// Size: 95
// EAX: pGame_type
// EDX: pGame_options
// EBX: pRace_index
void ReadNetGameChoices(tNet_game_type *pGame_type, tNet_game_options *pGame_options, int *pRace_index);

// Offset: 11028
// Size: 56
// EAX: pRank
int ChooseStartRace(int *pRank);

// Offset: 11084
// Size: 63
// EAX: pNet_game
void SetUpOtherNetThings(tNet_game_details *pNet_game);

// Offset: 11148
// Size: 97
// EAX: pNet_game
void RequestCarDetails(tNet_game_details *pNet_game);

// Offset: 11248
// Size: 181
int PickARandomCar();

// Offset: 11432
// Size: 54
// EAX: pNet_game
void PollCarDetails(tNet_game_details *pNet_game);

// Offset: 11488
// Size: 278
// EAX: pOptions
void SetNetAvailability(tNet_game_options *pOptions);

// Offset: 11768
// Size: 396
// EAX: pNet_game
// EDX: pOptions
// EBX: pCar_index
// ECX: pIm_the_host_so_fuck_off
int ChooseNetCar(tNet_game_details *pNet_game, tNet_game_options *pOptions, int *pCar_index, int pIm_the_host_so_fuck_off);

// Offset: 12164
// Size: 115
void InitNetStorageSpace();

// Offset: 12280
// Size: 63
void DisposeNetStorageSpace();

// Offset: 12344
// Size: 793
int DoMultiPlayerStart();

#endif
