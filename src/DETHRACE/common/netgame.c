#include "netgame.h"
#include "displays.h"
#include "globvars.h"
#include "globvrpb.h"
#include "harness/trace.h"
#include "network.h"
#include "opponent.h"
#include "powerup.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>

int gPowerup_cost[4] = { 1500, 2500, 4000, 6000 };
int gGame_scores[6];
int gPed_target;
int gNot_shown_race_type_headup;
tU32 gLast_it_change;
tU32 gTime_for_punishment;
tNet_game_player_info* gLast_lepper;
int gInitialised_grid;
int gIt_or_fox;

// IDA: void __usercall SendCarData(tU32 pNext_frame_time@<EAX>)
void SendCarData(tU32 pNext_frame_time) {
    tNet_contents* contents;
    tCar_spec* car;
    tCollision_info* ncar;
    int i;
    int j;
    static tU32 last_time;
    tU32 time;
    int damaged_wheels;
    LOG_TRACE("(%d)", pNext_frame_time);

    if (gNet_mode) {
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
}

// IDA: void __usercall ReceivedRecover(tNet_contents *pContents@<EAX>)
void ReceivedRecover(tNet_contents* pContents) {
    int i;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyMechanics(tCar_spec *pCar@<EAX>, tNet_contents *pContents@<EDX>)
void CopyMechanics(tCar_spec* pCar, tNet_contents* pContents) {
    int j;
    LOG_TRACE("(%p, %p)", pCar, pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedMechanics(tNet_contents *pContents@<EAX>)
void ReceivedMechanics(tNet_contents* pContents) {
    int i;
    tCar_spec* car;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedCopInfo(tNet_contents *pContents@<EAX>)
void ReceivedCopInfo(tNet_contents* pContents) {
    tCar_spec* c;
    int i;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SendAllNonCarPositions()
void SendAllNonCarPositions() {
    int i;
    br_actor** list;
    tNon_car_spec* non_car;
    tNet_contents* contents;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedNonCarPosition(tNet_contents *pContents@<EAX>)
void ReceivedNonCarPosition(tNet_contents* pContents) {
    br_actor* actor;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedNonCar(tNet_contents *pContents@<EAX>)
void ReceivedNonCar(tNet_contents* pContents) {
    br_actor* actor;
    br_vector3 tv;
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    tNon_car_spec* ncar;
    tCollision_info* c;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SignalToStartRace2(int pIndex@<EAX>)
void SignalToStartRace2(int pIndex) {
    tNet_message* the_message;
    int i;
    int j;
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SignalToStartRace()
void SignalToStartRace() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SetUpNetCarPositions()
void SetUpNetCarPositions() {
    int i;
    int j;
    int k;
    int grid_index;
    int racer_count;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReinitialiseCar(tCar_spec *pCar@<EAX>)
void ReinitialiseCar(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RepositionPlayer(int pIndex@<EAX>)
void RepositionPlayer(int pIndex) {
    tNet_message* the_message;
    tCar_spec* car;
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisableCar(tCar_spec *pCar@<EAX>)
void DisableCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    if (pCar->driver_name[0] != '\0') {
        if (!pCar->disabled) {
            pCar->disabled = 1;
            ForceRebuildActiveCarList();
        }
        if (pCar->car_master_actor->t.t.mat.m[3][0] < 500.0f) {
            pCar->car_master_actor->t.t.mat.m[3][0] = pCar->car_master_actor->t.t.mat.m[3][0] + 1000.0f;
            pCar->car_master_actor->t.t.mat.m[3][1] = pCar->car_master_actor->t.t.mat.m[3][1] + 1000.0f;
            pCar->car_master_actor->t.t.mat.m[3][2] = pCar->car_master_actor->t.t.mat.m[3][2] + 1000.0f;
            pCar->old_frame_mat.m[3][0] = pCar->car_master_actor->t.t.mat.m[3][0];
            pCar->old_frame_mat.m[3][1] = pCar->car_master_actor->t.t.mat.m[3][1];
            pCar->old_frame_mat.m[3][2] = pCar->car_master_actor->t.t.mat.m[3][2];
        }
    }
}

// IDA: void __usercall EnableCar(tCar_spec *pCar@<EAX>)
void EnableCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    if (pCar->driver_name[0] != '\0') {
        if (pCar->disabled) {
            pCar->disabled = 0;
            ForceRebuildActiveCarList();
        }
        if (pCar->car_master_actor->t.t.mat.m[3][0] > 500.0f) {
            pCar->car_master_actor->t.t.mat.m[3][0] = pCar->car_master_actor->t.t.mat.m[3][0] - 1000.0f;
            pCar->car_master_actor->t.t.mat.m[3][1] = pCar->car_master_actor->t.t.mat.m[3][1] - 1000.0f;
            pCar->car_master_actor->t.t.mat.m[3][2] = pCar->car_master_actor->t.t.mat.m[3][2] - 1000.0f;
            pCar->old_frame_mat.m[3][0] = pCar->car_master_actor->t.t.mat.m[3][0];
            pCar->old_frame_mat.m[3][1] = pCar->car_master_actor->t.t.mat.m[3][1];
            pCar->old_frame_mat.m[3][2] = pCar->car_master_actor->t.t.mat.m[3][2];
        }
    }
}

// IDA: void __usercall DoNetworkHeadups(int pCredits@<EAX>)
void DoNetworkHeadups(int pCredits) {
    char s[256];
    char s2[256];
    static tU32 last_flash;
    static int flash_state;
    LOG_TRACE("(%d)", pCredits);
    STUB_ONCE();
}

// IDA: int __usercall SortNetHeadAscending@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
int SortNetHeadAscending(void* pFirst_one, void* pSecond_one) {
    LOG_TRACE("(%p, %p)", pFirst_one, pSecond_one);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SortNetHeadDescending@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
int SortNetHeadDescending(void* pFirst_one, void* pSecond_one) {
    LOG_TRACE("(%p, %p)", pFirst_one, pSecond_one);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ClipName(char *pName@<EAX>, tDR_font *pFont@<EDX>, int pMax_width@<EBX>)
void ClipName(char* pName, tDR_font* pFont, int pMax_width) {
    LOG_TRACE("(\"%s\", %p, %d)", pName, pFont, pMax_width);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoNetScores2(int pOnly_sort_scores@<EAX>)
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
    LOG_TRACE("(%d)", pOnly_sort_scores);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoNetScores()
void DoNetScores() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitNetHeadups()
void InitNetHeadups() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeNetHeadups()
void DisposeNetHeadups() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EverybodysLost()
void EverybodysLost() {
    tNet_message* the_message;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DeclareWinner(int pWinner_index@<EAX>)
void DeclareWinner(int pWinner_index) {
    tNet_message* the_message;
    int i;
    int j;
    int best_score_index;
    char s[256];
    LOG_TRACE("(%d)", pWinner_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayerIsIt(tNet_game_player_info *pPlayer@<EAX>)
void PlayerIsIt(tNet_game_player_info* pPlayer) {
    int i;
    char s[256];
    LOG_TRACE("(%p)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FarEnoughAway@<EAX>(tNet_game_player_info *pPlayer_1@<EAX>, tNet_game_player_info *pPlayer_2@<EDX>)
int FarEnoughAway(tNet_game_player_info* pPlayer_1, tNet_game_player_info* pPlayer_2) {
    br_vector3 difference;
    LOG_TRACE("(%p, %p)", pPlayer_1, pPlayer_2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CarInContactWithItOrFox(tNet_game_player_info *pPlayer@<EAX>)
void CarInContactWithItOrFox(tNet_game_player_info* pPlayer) {
    LOG_TRACE("(%p)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SelectRandomItOrFox(int pNot_this_one@<EAX>)
void SelectRandomItOrFox(int pNot_this_one) {
    int i;
    int new_choice;
    LOG_TRACE("(%d)", pNot_this_one);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CalcPlayerScores()
void CalcPlayerScores() {
    int i;
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
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SendPlayerScores()
void SendPlayerScores() {
    tNet_contents* the_contents;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoNetGameManagement()
void DoNetGameManagement() {
    LOG_TRACE("()");
    STUB_ONCE();
}

// IDA: void __usercall InitialisePlayerScore(tNet_game_player_info *pPlayer@<EAX>)
void InitialisePlayerScore(tNet_game_player_info* pPlayer) {
    LOG_TRACE("(%p)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitPlayers()
void InitPlayers() {
    int i;
    LOG_TRACE("()");

    STUB();
}

// IDA: void __usercall BuyPSPowerup(int pIndex@<EAX>)
void BuyPSPowerup(int pIndex) {
    char s[256];
    char s2[256];
    LOG_TRACE("(%d)", pIndex);

    if (gNet_mode == eNet_mode_none) {
        NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(178));
    } else if (gProgram_state.current_car.power_up_levels[pIndex] < 4) {
        if (gNet_mode == eNet_mode_none || gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]] <= (gProgram_state.credits_earned - gProgram_state.credits_lost)) {
            SpendCredits(gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]]);
            ImprovePSPowerup(&gProgram_state.current_car, pIndex);
        } else {
            strcpy(s, GetMiscString(179));
            sprintf(s2, "%d", gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]]);
            SubsStringJob(s, s2);
            NewTextHeadupSlot(4, 0, 3008, -4, s);
        }
    } else {
        NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(174));
    }
}

// IDA: void __cdecl BuyArmour()
void BuyArmour() {
    LOG_TRACE("()");

    BuyPSPowerup(0);
}

// IDA: void __cdecl BuyPower()
void BuyPower() {
    LOG_TRACE("()");

    if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(214));
    } else if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(215));
    } else {
        BuyPSPowerup(1);
    }
}

// IDA: void __cdecl BuyOffense()
void BuyOffense() {
    LOG_TRACE("()");

    BuyPSPowerup(2);
}

// IDA: void __usercall UseGeneralScore(int pScore@<EAX>)
void UseGeneralScore(int pScore) {
    int i;
    LOG_TRACE("(%d)", pScore);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetSendEnvironmentChanges(tNet_game_player_info *pPlayer@<EAX>)
void NetSendEnvironmentChanges(tNet_game_player_info* pPlayer) {
    LOG_TRACE("(%p)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl UpdateEnvironments()
void UpdateEnvironments() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedGameplay(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>, tU32 pReceive_time@<EBX>)
void ReceivedGameplay(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    int must_revert_reentrancy;
    int gPixel_buffer_size;
    char* gPixels_copy;
    char* gPalette_copy;
    static int pause_semaphore;
    LOG_TRACE("(%p, %p, %d)", pContents, pMessage, pReceive_time);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SendGameplay(tPlayer_ID pPlayer@<EAX>, tNet_gameplay_mess pMess@<EDX>, int pParam_1@<EBX>, int pParam_2@<ECX>, int pParam_3, int pParam_4)
void SendGameplay(tPlayer_ID pPlayer, tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    LOG_TRACE("(%d, %d, %d, %d, %d, %d)", pPlayer, pMess, pParam_1, pParam_2, pParam_3, pParam_4);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SendGameplayToAllPlayers(tNet_gameplay_mess pMess@<EAX>, int pParam_1@<EDX>, int pParam_2@<EBX>, int pParam_3@<ECX>, int pParam_4)
void SendGameplayToAllPlayers(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    LOG_TRACE("(%d, %d, %d, %d, %d)", pMess, pParam_1, pParam_2, pParam_3, pParam_4);

    STUB_ONCE();
}

// IDA: void __usercall SendGameplayToHost(tNet_gameplay_mess pMess@<EAX>, int pParam_1@<EDX>, int pParam_2@<EBX>, int pParam_3@<ECX>, int pParam_4)
void SendGameplayToHost(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    LOG_TRACE("(%d, %d, %d, %d, %d)", pMess, pParam_1, pParam_2, pParam_3, pParam_4);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitNetGameplayStuff()
void InitNetGameplayStuff() {
    LOG_TRACE("()");
}

// IDA: void __cdecl DefaultNetName()
void DefaultNetName() {
    NetObtainSystemUserName(gNet_player_name, 32);
}

// IDA: void __usercall NetSendPointCrush(tCar_spec *pCar@<EAX>, tU16 pCrush_point_index@<EDX>, br_vector3 *pEnergy_vector@<EBX>)
void NetSendPointCrush(tCar_spec* pCar, tU16 pCrush_point_index, br_vector3* pEnergy_vector) {
    tNet_contents* contents;
    LOG_TRACE("(%p, %d, %p)", pCar, pCrush_point_index, pEnergy_vector);

    STUB_ONCE();
}

// IDA: void __usercall RecievedCrushPoint(tNet_contents *pContents@<EAX>)
void RecievedCrushPoint(tNet_contents* pContents) {
    tCar_spec* car;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetReducedMatrix(tReduced_matrix *m1@<EAX>, br_matrix34 *m2@<EDX>)
void GetReducedMatrix(tReduced_matrix* m1, br_matrix34* m2) {
    LOG_TRACE("(%p, %p)", m1, m2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetExpandedMatrix(br_matrix34 *m1@<EAX>, tReduced_matrix *m2@<EDX>)
void GetExpandedMatrix(br_matrix34* m1, tReduced_matrix* m2) {
    LOG_TRACE("(%p, %p)", m1, m2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetEarnCredits(tNet_game_player_info *pPlayer@<EAX>, tS32 pCredits@<EDX>)
void NetEarnCredits(tNet_game_player_info* pPlayer, tS32 pCredits) {
    LOG_TRACE("(%p, %d)", pPlayer, pCredits);
    NOT_IMPLEMENTED();
}
