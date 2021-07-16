#ifndef _NEWGAME_H_
#define _NEWGAME_H_

#include "br_types.h"
#include "dr_types.h"

extern char* gBasic_car_names[2];
extern tNet_game_options gNet_settings[8];
extern tRadio_bastards gRadio_bastards[11];
extern tJoinable_game gGames_to_join[6];
extern tNet_game_options* gOptions;
extern int gNet_target[7];
extern int gLast_graph_sel;
extern tInterface_spec* gThe_interface_spec;
extern tNet_sequence_type gNet_race_sequence;
extern tNet_game_type gLast_game_type;
extern int gCurrent_net_game_count;
extern tU32 gAnne_flic_data_length;
extern int gShifted_default_yet;
extern int gNet_storage_allocated;
extern tU8* gFrank_flic_data;
extern char* gNet_name;
extern tU32 gFrank_flic_data_length;
extern tU8* gAnne_flic_data;
extern int gLast_net_choose_box;
extern int gCurrent_game_selection;
extern int gRace_index;
extern int gRadio_selected;

void StartRollingPlayerNamesIn();

void FrankAnneStart1();

void FrankAnneStart2();

void GetPlayerName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length);

int FrankAnneDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

void FrankAnneDraw(int pCurrent_choice, int pCurrent_mode);

int FrankieOrAnnie();

int SelectSkillLevel();

int DoOnePlayerStart();

int NewNetGameUp(int* pCurrent_choice, int* pCurrent_mode);

int NewNetGameDown(int* pCurrent_choice, int* pCurrent_mode);

void DisposeJoinableGame(int pIndex);

void DrawAnItem(int pX, int pY_index, int pFont_index, char* pText);

void DrawColumnHeading(int pStr_index, int pX);

void DrawGames(int pCurrent_choice, int pCurrent_mode);

void InitGamesToJoin();

void DisposeJoinList(int pExemption);

void AddToJoinList(tNet_game_details* pGame);

void NewNetStart1();

void NewNetStart2();

void NewNetGetName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length);

int NewNetDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

int NewNetGoAhead(int* pCurrent_choice, int* pCurrent_mode);

tJoin_or_host_result JoinOrHostGame(tNet_game_details** pGame_to_join);

void GetNetOptions(tNet_game_options* pGame_options);

void SetNetOptions(tNet_game_options* pGame_options);

void NetPlayCheckboxOn2(int pIndex);

void NetPlayCheckboxOff2(int pIndex);

void NetPlayCheckboxOn(int pIndex);

void NetPlayCheckboxOff(int pIndex);

void NetPlayRadioOn2(int pIndex, int pValue);

void NetPlayRadioOff2(int pIndex, int pValue);

void NetPlayRadioOn(int pIndex, int pValue);

void NetPlayRadioOff(int pIndex, int pValue);

void DrawNOptInitialRadios();

void NetRadioChanged(int pIndex, int pNew_value);

void NetCheckboxChanged(int pIndex);

int NetOptLeft(int* pCurrent_choice, int* pCurrent_mode);

int NetOptRight(int* pCurrent_choice, int* pCurrent_mode);

int NetOptUp(int* pCurrent_choice, int* pCurrent_mode);

int NetOptDown(int* pCurrent_choice, int* pCurrent_mode);

int NetRadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

void RevertToDefaults();

void DefaultNetSettings();

int NetOptGoAhead(int* pCurrent_choice, int* pCurrent_mode);

void NetPlotAGraphBox(int pIndex, int pColour_value);

void NetDrawAGraphBox(int pIndex);

void NetEraseAGraphBox(int pIndex);

void DrawNetOptBox(int pCurrent_choice, int pCurrent_mode);

void DoNetOptions(tNet_game_options* pGame_options);

void PlayRadioOn_newgame(int pIndex);

void PlayRadioOff_newgame(int pIndex);

void SetOptions(tNet_game_type pGame_type, tNet_game_options* pGame_options);

void DrawNetChooseInitial();

int NetChooseGoAhead(int* pCurrent_choice, int* pCurrent_mode);

void PlotAGraphBox(int pIndex, int pColour_value);

void DrawAGraphBox(int pIndex);

void EraseAGraphBox(int pIndex);

void DrawNetChoose(int pCurrent_choice, int pCurrent_mode);

int NetChooseLR(int* pCurrent_choice, int* pCurrent_mode);

void SetGameTarget(tNet_game_type* pGame_type, tNet_game_options* pGame_options);

int NetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index);

void ReadNetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index);

int ChooseStartRace(int* pRank);

void SetUpOtherNetThings(tNet_game_details* pNet_game);

void RequestCarDetails(tNet_game_details* pNet_game);

int PickARandomCar();

void PollCarDetails(tNet_game_details* pNet_game);

void SetNetAvailability(tNet_game_options* pOptions);

int ChooseNetCar(tNet_game_details* pNet_game, tNet_game_options* pOptions, int* pCar_index, int pIm_the_host_so_fuck_off);

void InitNetStorageSpace();

void DisposeNetStorageSpace();

int DoMultiPlayerStart();

#endif
