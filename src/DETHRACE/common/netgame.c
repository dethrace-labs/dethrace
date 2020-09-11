#include "netgame.h"
#include "globvars.h"
#include "network.h"
#include <stdlib.h>

int gGame_scores[6];
int gPowerup_cost[4];
int gPed_target;
int gNot_shown_race_type_headup;
tU32 gLast_it_change;
tU32 gTime_for_punishment;
tNet_game_player_info* gLast_lepper;
int gInitialised_grid;
int gIt_or_fox;

void SendCarData(tU32 pNext_frame_time) {
    tNet_contents* contents;
    tCar_spec* car;
    tCollision_info* ncar;
    int i;
    int j;
    static tU32 last_time;
    tU32 time;
    int damaged_wheels;
    NOT_IMPLEMENTED();
}

void ReceivedRecover(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

void CopyMechanics(tCar_spec* pCar, tNet_contents* pContents) {
    int j;
    NOT_IMPLEMENTED();
}

void ReceivedMechanics(tNet_contents* pContents) {
    int i;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void ReceivedCopInfo(tNet_contents* pContents) {
    tCar_spec* c;
    int i;
    NOT_IMPLEMENTED();
}

void SendAllNonCarPositions() {
    int i;
    br_actor** list;
    tNon_car_spec* non_car;
    tNet_contents* contents;
    NOT_IMPLEMENTED();
}

void ReceivedNonCarPosition(tNet_contents* pContents) {
    br_actor* actor;
    NOT_IMPLEMENTED();
}

void ReceivedNonCar(tNet_contents* pContents) {
    br_actor* actor;
    br_vector3 tv;
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    tNon_car_spec* ncar;
    tCollision_info* c;
    NOT_IMPLEMENTED();
}

void SignalToStartRace2(int pIndex) {
    tNet_message* the_message;
    int i;
    int j;
    NOT_IMPLEMENTED();
}

void SignalToStartRace() {
    NOT_IMPLEMENTED();
}

void SetUpNetCarPositions() {
    int j;
    int k;
    int grid_index;
    int racer_count;
    NOT_IMPLEMENTED();
}

void ReinitialiseCar(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

void RepositionPlayer(int pIndex) {
    tNet_message* the_message;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void DisableCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void EnableCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void DoNetworkHeadups(int pCredits) {
    char s[256];
    char s2[256];
    static tU32 last_flash;
    static int flash_state;
    NOT_IMPLEMENTED();
}

int SortNetHeadAscending(void* pFirst_one, void* pSecond_one) {
    NOT_IMPLEMENTED();
}

int SortNetHeadDescending(void* pFirst_one, void* pSecond_one) {
    NOT_IMPLEMENTED();
}

void ClipName(char* pName, tDR_font* pFont, int pMax_width) {
    NOT_IMPLEMENTED();
}

void DoNetScores2(int pOnly_sort_scores) {
    int i;
    int j;
    int score;
    int flags;
    int index;
    int right_edge;
    int x;
    int len;
    int ascending_order;
    char s[256];
    static tU32 last_flash;
    static int flash_state;
    tHeadup_pair headup_pairs[6];
    NOT_IMPLEMENTED();
}

void DoNetScores() {
    NOT_IMPLEMENTED();
}

void InitNetHeadups() {
    NOT_IMPLEMENTED();
}

void DisposeNetHeadups() {
    NOT_IMPLEMENTED();
}

void EverybodysLost() {
    int i;
    NOT_IMPLEMENTED();
}

void DeclareWinner(int pWinner_index) {
    tNet_message* the_message;
    int i;
    int j;
    int best_score_index;
    char s[256];
    NOT_IMPLEMENTED();
}

void PlayerIsIt(tNet_game_player_info* pPlayer) {
    int i;
    char s[256];
    NOT_IMPLEMENTED();
}

int FarEnoughAway(tNet_game_player_info* pPlayer_1, tNet_game_player_info* pPlayer_2) {
    br_vector3 difference;
    NOT_IMPLEMENTED();
}

void CarInContactWithItOrFox(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

void SelectRandomItOrFox(int pNot_this_one) {
    int i;
    int new_choice;
    NOT_IMPLEMENTED();
}

void CalcPlayerScores() {
    int j;
    int knock_out_bit;
    int e_dam;
    int t_dam;
    int d_dam;
    int w_dam;
    int cars_left;
    int car_left;
    int flags;
    int score;
    int highest;
    int next_highest;
    int lowest_score;
    int player_left;
    int new_choice;
    tCar_spec* car;
    tNet_message* message;
    tS32 time;
    char s[256];
    tNet_game_player_info* lowest_score_player;
    NOT_IMPLEMENTED();
}

void SendPlayerScores() {
    int i;
    NOT_IMPLEMENTED();
}

void DoNetGameManagement() {
    NOT_IMPLEMENTED();
}

void InitialisePlayerScore(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

void InitPlayers() {
    NOT_IMPLEMENTED();
}

void BuyPSPowerup(int pIndex) {
    char s[256];
    char s2[256];
    NOT_IMPLEMENTED();
}

void BuyArmour() {
    NOT_IMPLEMENTED();
}

void BuyPower() {
    NOT_IMPLEMENTED();
}

void BuyOffense() {
    NOT_IMPLEMENTED();
}

void UseGeneralScore(int pScore) {
    int i;
    NOT_IMPLEMENTED();
}

void NetSendEnvironmentChanges(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

void UpdateEnvironments() {
    NOT_IMPLEMENTED();
}

void ReceivedGameplay(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    int must_revert_reentrancy;
    int gPixel_buffer_size;
    char* gPixels_copy;
    char* gPalette_copy;
    static int pause_semaphore;
    NOT_IMPLEMENTED();
}

void SendGameplay(tPlayer_ID pPlayer, tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

void SendGameplayToAllPlayers(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

void SendGameplayToHost(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

void InitNetGameplayStuff() {
    NOT_IMPLEMENTED();
}

void DefaultNetName() {
    NetObtainSystemUserName(gNet_player_name, 32);
}

void NetSendPointCrush(tCar_spec* pCar, tU16 pCrush_point_index, br_vector3* pEnergy_vector) {
    tNet_contents* contents;
    NOT_IMPLEMENTED();
}

void RecievedCrushPoint(tNet_contents* pContents) {
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void GetReducedMatrix(tReduced_matrix* m1, br_matrix34* m2) {
    NOT_IMPLEMENTED();
}

void GetExpandedMatrix(br_matrix34* m1, tReduced_matrix* m2) {
    NOT_IMPLEMENTED();
}

void NetEarnCredits(tNet_game_player_info* pPlayer, tS32 pCredits) {
    NOT_IMPLEMENTED();
}
