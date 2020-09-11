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

// Offset: 0
// Size: 2343
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

// Offset: 2344
// Size: 137
void ReceivedRecover(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 2484
// Size: 117
void CopyMechanics(tCar_spec* pCar, tNet_contents* pContents) {
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 2604
// Size: 1378
void ReceivedMechanics(tNet_contents* pContents) {
    int i;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

// Offset: 3984
// Size: 576
void ReceivedCopInfo(tNet_contents* pContents) {
    tCar_spec* c;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 4560
// Size: 250
void SendAllNonCarPositions() {
    int i;
    br_actor** list;
    tNon_car_spec* non_car;
    tNet_contents* contents;
    NOT_IMPLEMENTED();
}

// Offset: 4812
// Size: 118
void ReceivedNonCarPosition(tNet_contents* pContents) {
    br_actor* actor;
    NOT_IMPLEMENTED();
}

// Offset: 4932
// Size: 842
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

// Offset: 5776
// Size: 617
void SignalToStartRace2(int pIndex) {
    tNet_message* the_message;
    int i;
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 6396
// Size: 56
void SignalToStartRace() {
    NOT_IMPLEMENTED();
}

// Offset: 6452
// Size: 776
void SetUpNetCarPositions() {
    int j;
    int k;
    int grid_index;
    int racer_count;
    NOT_IMPLEMENTED();
}

// Offset: 7228
// Size: 93
void ReinitialiseCar(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7324
// Size: 285
void RepositionPlayer(int pIndex) {
    tNet_message* the_message;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

// Offset: 7612
// Size: 200
void DisableCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 7812
// Size: 222
void EnableCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8036
// Size: 893
void DoNetworkHeadups(int pCredits) {
    char s[256];
    char s2[256];
    static tU32 last_flash;
    static int flash_state;
    NOT_IMPLEMENTED();
}

// Offset: 8932
// Size: 167
int SortNetHeadAscending(void* pFirst_one, void* pSecond_one) {
    NOT_IMPLEMENTED();
}

// Offset: 9100
// Size: 167
int SortNetHeadDescending(void* pFirst_one, void* pSecond_one) {
    NOT_IMPLEMENTED();
}

// Offset: 9268
// Size: 72
void ClipName(char* pName, tDR_font* pFont, int pMax_width) {
    NOT_IMPLEMENTED();
}

// Offset: 9340
// Size: 2219
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

// Offset: 11560
// Size: 41
void DoNetScores() {
    NOT_IMPLEMENTED();
}

// Offset: 11604
// Size: 156
void InitNetHeadups() {
    NOT_IMPLEMENTED();
}

// Offset: 11760
// Size: 146
void DisposeNetHeadups() {
    NOT_IMPLEMENTED();
}

// Offset: 11908
// Size: 130
void EverybodysLost() {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 12040
// Size: 676
void DeclareWinner(int pWinner_index) {
    tNet_message* the_message;
    int i;
    int j;
    int best_score_index;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 12716
// Size: 476
void PlayerIsIt(tNet_game_player_info* pPlayer) {
    int i;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 13192
// Size: 195
int FarEnoughAway(tNet_game_player_info* pPlayer_1, tNet_game_player_info* pPlayer_2) {
    br_vector3 difference;
    NOT_IMPLEMENTED();
}

// Offset: 13388
// Size: 144
void CarInContactWithItOrFox(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

// Offset: 13532
// Size: 276
void SelectRandomItOrFox(int pNot_this_one) {
    int i;
    int new_choice;
    NOT_IMPLEMENTED();
}

// Offset: 13808
// Size: 2570
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

// Offset: 16380
// Size: 183
void SendPlayerScores() {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 16564
// Size: 53
void DoNetGameManagement() {
    NOT_IMPLEMENTED();
}

// Offset: 16620
// Size: 218
void InitialisePlayerScore(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

// Offset: 16840
// Size: 142
void InitPlayers() {
    NOT_IMPLEMENTED();
}

// Offset: 16984
// Size: 343
void BuyPSPowerup(int pIndex) {
    char s[256];
    char s2[256];
    NOT_IMPLEMENTED();
}

// Offset: 17328
// Size: 41
void BuyArmour() {
    NOT_IMPLEMENTED();
}

// Offset: 17372
// Size: 188
void BuyPower() {
    NOT_IMPLEMENTED();
}

// Offset: 17560
// Size: 44
void BuyOffense() {
    NOT_IMPLEMENTED();
}

// Offset: 17604
// Size: 247
void UseGeneralScore(int pScore) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 17852
// Size: 53
void NetSendEnvironmentChanges(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

// Offset: 17908
// Size: 152
void UpdateEnvironments() {
    NOT_IMPLEMENTED();
}

// Offset: 18060
// Size: 635
void ReceivedGameplay(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    int must_revert_reentrancy;
    int gPixel_buffer_size;
    char* gPixels_copy;
    char* gPalette_copy;
    static int pause_semaphore;
    NOT_IMPLEMENTED();
}

// Offset: 18696
// Size: 120
void SendGameplay(tPlayer_ID pPlayer, tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 18816
// Size: 117
void SendGameplayToAllPlayers(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 18936
// Size: 126
void SendGameplayToHost(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 19064
// Size: 42
void InitNetGameplayStuff() {
    NOT_IMPLEMENTED();
}

// Offset: 19108
// Size: 49
void DefaultNetName() {
    NetObtainSystemUserName(gNet_player_name, 32);
}

// Offset: 19160
// Size: 116
void NetSendPointCrush(tCar_spec* pCar, tU16 pCrush_point_index, br_vector3* pEnergy_vector) {
    tNet_contents* contents;
    NOT_IMPLEMENTED();
}

// Offset: 19276
// Size: 290
void RecievedCrushPoint(tNet_contents* pContents) {
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

// Offset: 19568
// Size: 144
void GetReducedMatrix(tReduced_matrix* m1, br_matrix34* m2) {
    NOT_IMPLEMENTED();
}

// Offset: 19712
// Size: 238
void GetExpandedMatrix(br_matrix34* m1, tReduced_matrix* m2) {
    NOT_IMPLEMENTED();
}

// Offset: 19952
// Size: 38
void NetEarnCredits(tNet_game_player_info* pPlayer, tS32 pCredits) {
    NOT_IMPLEMENTED();
}
