#ifndef _RACESTRT_H_
#define _RACESTRT_H_

#include "br_types.h"
#include "dr_types.h"

extern int gGrid_number_colour[4];
extern int gJust_bought_part;
extern tU32 gLast_host_query;
extern br_pixelmap* gDead_car;
extern int gFade_away_parts_shop;
extern tU32 gDare_start_time;
extern int gRefund_rate;
extern int gSwap_grid_2;
extern int gSwap_grid_1;
extern int gChange_race_net_mode;
extern tParts_category gPart_category;
extern tU32 gNet_synch_start;
extern tNet_game_details* gChoose_car_net_game;
extern int gPart_index;
extern int gChallenger_index__racestrt; // suffix added to avoid duplicate symbol
extern tGrid_draw gDraw_grid_status;
extern tNet_sequence_type gNet_race_sequence__racestrt; // suffix added to avoid duplicate symbol
extern br_pixelmap* gTaken_image;
extern int gGrid_number_x_coords[31];
extern int gGrid_transition_stage;
extern int gGrid_y_adjust;
extern br_pixelmap* gBullet_image;
extern br_pixelmap* gDeceased_image;
extern int gBest_pos_available;
extern int gChallenger_position;
extern int gOpponent_index;
extern int gChallenge_time;
extern int gOriginal_position;
extern int gCurrent_race_index;
extern tInterface_spec* gStart_interface_spec;
extern int gCurrent_car_index;
extern int gOur_starting_position;

void DrawRaceList(int pOffset);

void MoveRaceList(int pFrom, int pTo, tS32 pTime_to_move);

int UpRace(int* pCurrent_choice, int* pCurrent_mode);

int DownRace(int* pCurrent_choice, int* pCurrent_mode);

int ClickOnRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int UpClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int DownClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

void StartChangeRace();

int ChangeRace(int* pRace_index, int pNet_mode, tNet_sequence_type pNet_race_sequence);

void DoChangeRace();

void DrawCar(int pCurrent_choice, int pCurrent_mode);

void SetCarFlic();

int UpCar(int* pCurrent_choice, int* pCurrent_mode);

int DownCar(int* pCurrent_choice, int* pCurrent_mode);

int UpClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int DownClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int ChangeCarGoAhead(int* pCurrent_choice, int* pCurrent_mode);

int ChangeCar(int pNet_mode, int* pCar_index, tNet_game_details* pNet_game);

void DoChangeCar();

int PartsShopRecommended();

void CalcPartPrice(int pCategory, int pIndex, int* pPrice, int* pCost);

int BuyPart(int pCategory, int pIndex);

void DoAutoParts();

void DrawPartsLabel();

void ErasePartsText(int pTotal_as_well);

void DrawPartsText();

void SetPartsImage();

int GetPartsMax();

void CalcPartsIndex();

void DoExchangePart();

int PartsShopGoAhead(int* pCurrent_choice, int* pCurrent_mode);

int UpPart(int* pCurrent_choice, int* pCurrent_mode);

int DownPart(int* pCurrent_choice, int* pCurrent_mode);

int UpClickPart(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int DownClickPart(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int PartsArrowsOn(int* pCurrent_choice, int* pCurrent_mode);

int PartsArrowsOff(int* pCurrent_choice, int* pCurrent_mode);

void StartPartsShop();

int DonePartsShop(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

void DrawPartsShop(int pCurrent_choice, int pCurrent_mode);

void DoPartsShop(int pFade_away);

int AutoPartsDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

tSO_result DoAutoPartsShop();

void SetOpponentFlic();

void DrawSceneyMappyInfoVieweyThing();

void DismissSceneyMappyInfoVieweyThing();

int SelectRaceDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

int StartRaceGoAhead(int* pCurrent_choice, int* pCurrent_mode);

int TryToMoveToArrows(int* pCurrent_choice, int* pCurrent_mode);

int UpOpponent(int* pCurrent_choice, int* pCurrent_mode);

int DownOpponent(int* pCurrent_choice, int* pCurrent_mode);

int UpClickOpp(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int DownClickOpp(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

void SelectRaceStart();

int SuggestRace();

void SelectRaceDraw(int pCurrent_choice, int pCurrent_mode);

tSO_result DoSelectRace(int* pSecond_time_around);

void DrawGridCar(int pX, int pY, br_pixelmap* pImage);

void DrawGrid(int pOffset, int pDraw_it);

void MoveGrid(int pFrom, int pTo, tS32 pTime_to_move);

int CalcGridOffset(int pPosition);

void GridDraw(int pCurrent_choice, int pCurrent_mode);

void ActuallySwapOrder(int pFirst_index, int pSecond_index);

void DoGridTransition(int pFirst_index, int pSecond_index);

void ChallengeStart();

int CheckNextStage(int* pCurrent_choice, int* pCurrent_mode);

int ChallengeDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

void DoChallengeScreen();

int GridDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

void GridStart();

int GridMoveLeft(int* pCurrent_choice, int* pCurrent_mode);

int GridMoveRight(int* pCurrent_choice, int* pCurrent_mode);

int GridClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int GridClickNumbers(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int GridClickLeft(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int GridClickRight(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset);

int CheckChallenge(int* pCurrent_choice, int* pCurrent_mode);

int FindBestPos(int pOur_rank);

int SortGridFunction(const void* pFirst_one, const void* pSecond_one);

void SortOpponents();

tSO_result DoGridPosition();

void CheckPlayersAreResponding();

void NetSynchStartStart();

// Suffix added to avoid duplicate symbol
void DrawAnItem__racestrt(int pX, int pY_index, int pFont_index, char* pText);

void NetSynchStartDraw(int pCurrent_choice, int pCurrent_mode);

int NetSynchStartDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

int NetSynchStartGoAhead(int* pCurrent_choice, int* pCurrent_mode);

int ExitWhenReady(int* pCurrent_choice, int* pCurrent_mode);

tSO_result NetSynchRaceStart2(tNet_synch_mode pMode);

tSO_result NetSynchRaceStart();

#endif
