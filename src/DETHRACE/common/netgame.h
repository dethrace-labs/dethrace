#ifndef _NETGAME_H_
#define _NETGAME_H_

#include "br_types.h"
#include "dr_types.h"

extern int gGame_scores[6];
extern int gPowerup_cost[4];
extern int gPed_target;
extern int gNot_shown_race_type_headup;
extern tU32 gLast_it_change;
extern tU32 gTime_for_punishment;
extern tNet_game_player_info* gLast_lepper;
extern int gInitialised_grid;
extern int gIt_or_fox;

void SendCarData(tU32 pNext_frame_time);

void ReceivedRecover(tNet_contents* pContents);

void CopyMechanics(tCar_spec* pCar, tNet_contents* pContents);

void ReceivedMechanics(tNet_contents* pContents);

void ReceivedCopInfo(tNet_contents* pContents);

void SendAllNonCarPositions();

void ReceivedNonCarPosition(tNet_contents* pContents);

void ReceivedNonCar(tNet_contents* pContents);

void SignalToStartRace2(int pIndex);

void SignalToStartRace();

void SetUpNetCarPositions();

void ReinitialiseCar(tCar_spec* pCar);

void RepositionPlayer(int pIndex);

void DisableCar(tCar_spec* pCar);

void EnableCar(tCar_spec* pCar);

void DoNetworkHeadups(int pCredits);

int SortNetHeadAscending(void* pFirst_one, void* pSecond_one);

int SortNetHeadDescending(void* pFirst_one, void* pSecond_one);

void ClipName(char* pName, tDR_font* pFont, int pMax_width);

void DoNetScores2(int pOnly_sort_scores);

void DoNetScores();

void InitNetHeadups();

void DisposeNetHeadups();

void EverybodysLost();

void DeclareWinner(int pWinner_index);

void PlayerIsIt(tNet_game_player_info* pPlayer);

int FarEnoughAway(tNet_game_player_info* pPlayer_1, tNet_game_player_info* pPlayer_2);

void CarInContactWithItOrFox(tNet_game_player_info* pPlayer);

void SelectRandomItOrFox(int pNot_this_one);

void CalcPlayerScores();

void SendPlayerScores();

void DoNetGameManagement();

void InitialisePlayerScore(tNet_game_player_info* pPlayer);

void InitPlayers();

void BuyPSPowerup(int pIndex);

void BuyArmour();

void BuyPower();

void BuyOffense();

void UseGeneralScore(int pScore);

void NetSendEnvironmentChanges(tNet_game_player_info* pPlayer);

void UpdateEnvironments();

void ReceivedGameplay(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time);

void SendGameplay(tPlayer_ID pPlayer, tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4);

void SendGameplayToAllPlayers(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4);

void SendGameplayToHost(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4);

void InitNetGameplayStuff();

void DefaultNetName();

void NetSendPointCrush(tCar_spec* pCar, tU16 pCrush_point_index, br_vector3* pEnergy_vector);

void RecievedCrushPoint(tNet_contents* pContents);

void GetReducedMatrix(tReduced_matrix* m1, br_matrix34* m2);

void GetExpandedMatrix(br_matrix34* m1, tReduced_matrix* m2);

void NetEarnCredits(tNet_game_player_info* pPlayer, tS32 pCredits);

#endif
