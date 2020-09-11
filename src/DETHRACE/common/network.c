#include "network.h"
#include "common/controls.h"
#include "common/netgame.h"
#include "globvars.h"
#include "globvrpb.h"
#include "pc-dos/dosnet.h"
#include <stdlib.h>

tNet_game_player_info gNew_net_players[6];
tGuaranteed_message gGuarantee_list[150];
int gRace_only_flags[33];
tU32 gLast_flush_message;
void* gMessage_to_free;
tMid_message* gMid_messages;
tU32 gLast_player_list_received;
tMin_message* gMin_messages;
void (*gAdd_proc)(tNet_game_details*);
int gNet_service_disable;
tNet_message* gTo_host_stack;
tNet_message* gBroadcast_stack;
int gReceiving_batch_number;
tU32 gMess_max_flags;
int gReceiving_new_players;
tMax_message* gMax_messages;
tU32 gGuarantee_number;
int gNext_guarantee;
tU32 gAsk_time;
int gIn_net_service;
int gNet_initialised = 0;
int gDont_allow_joiners;
tU32 gMess_mid_flags;
tNet_game_details* gCurrent_join_poll_game;
int gOnly_receive_guarantee_replies;
tU32 gMess_min_flags;
int gPlayer_list_batch_number;
int gMessage_header_size;
int gJoin_poll_index;
int gJoin_request_denied;
int gHost_died;
int gCar_was_taken;
int gBastard_has_answered;
int gTime_for_next_one;
int gJoin_list_mode;
int gReceived_game_scores;

// Offset: 0
// Size: 429
int NetInitialise() {
    NOT_IMPLEMENTED();
}

// Offset: 432
// Size: 103
int NetShutdown() {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 536
// Size: 58
void ShutdownNetIfRequired() {
    if (gNet_initialised) {
        PDNetShutdown();
        DisposeAbuseomatic();
        //BrMemFree(dword_1380E8);
        //BrMemFree(dword_1380E0);
        //BrMemFree(dword_138100);
        DisposeNetHeadups();
        gNet_initialised = 0;
    }
}

// Offset: 596
// Size: 44
void DisableNetService() {
    NOT_IMPLEMENTED();
}

// Offset: 640
// Size: 44
void ReenableNetService() {
    NOT_IMPLEMENTED();
}

// Offset: 684
// Size: 72
int PermitNetServiceReentrancy() {
    NOT_IMPLEMENTED();
}

// Offset: 756
// Size: 44
void HaltNetServiceReentrancy() {
    NOT_IMPLEMENTED();
}

// Offset: 800
// Size: 75
void NetSendHeadupToAllPlayers(char* pMessage) {
    tNet_contents* the_contents;
    NOT_IMPLEMENTED();
}

// Offset: 876
// Size: 110
void NetSendHeadupToEverybody(char* pMessage) {
    tNet_contents* the_contents;
    NOT_IMPLEMENTED();
}

// Offset: 988
// Size: 142
void NetSendHeadupToPlayer(char* pMessage, tPlayer_ID pPlayer) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

// Offset: 1132
// Size: 94
void InitialisePlayerStati() {
    NOT_IMPLEMENTED();
}

// Offset: 1228
// Size: 73
void LeaveTempGame() {
    NOT_IMPLEMENTED();
}

// Offset: 1304
// Size: 63
void DisposeCurrentJoinPollGame() {
    NOT_IMPLEMENTED();
}

// Offset: 1368
// Size: 277
void DoNextJoinPoll() {
    NOT_IMPLEMENTED();
}

// Offset: 1648
// Size: 100
void NetStartProducingJoinList(void (*pAdd_proc)(tNet_game_details*)) {
    NOT_IMPLEMENTED();
}

// Offset: 1748
// Size: 59
void NetEndJoinList() {
    NOT_IMPLEMENTED();
}

// Offset: 1808
// Size: 55
void NetDisposePIDGameInfo(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 1864
// Size: 51
void NetDisposeGameDetails(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 1916
// Size: 62
tNet_game_details* NetAllocatePIDGameDetails() {
    NOT_IMPLEMENTED();
}

// Offset: 1980
// Size: 66
void NetLeaveGameLowLevel(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 2048
// Size: 382
void NetLeaveGame(tNet_game_details* pNet_game) {
    tNet_message* the_message;
    char s[256];
    char* s2;
    int i;
    int must_revert_reentrancy;
    NOT_IMPLEMENTED();
}

// Offset: 2432
// Size: 49
void NetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address) {
    NOT_IMPLEMENTED();
}

// Offset: 2484
// Size: 45
void NetDisposePlayer(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

// Offset: 2532
// Size: 234
void FillInThisPlayer(tNet_game_details* pGame, tNet_game_player_info* pPlayer, int pCar_index, int pHost) {
    NOT_IMPLEMENTED();
}

// Offset: 2768
// Size: 206
void LoadCarN(int pIndex, tNet_game_player_info* pPlayer) {
    int switched_res;
    NOT_IMPLEMENTED();
}

// Offset: 2976
// Size: 266
void DisposeCarN(int pIndex) {
    int i;
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 3244
// Size: 154
void PlayerHasLeft(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 3400
// Size: 1321
void NetPlayersChanged(int pNew_count, tNet_game_player_info* pNew_players) {
    int i;
    int j;
    int k;
    int switched_res;
    int new_player;
    int player_still_there;
    tPlayer_ID old_fox_it;
    NOT_IMPLEMENTED();
}

// Offset: 4724
// Size: 386
tNet_game_details* NetHostGame(tNet_game_type pGame_type, tNet_game_options* pOptions, int pStart_rank, char* pHost_name, int pCar_index) {
    tNet_game_details* game;
    void* host_address;
    tNet_game_player_info me;
    NOT_IMPLEMENTED();
}

// Offset: 5112
// Size: 51
int NetInitClient(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 5164
// Size: 55
int NetJoinGameLowLevel(tNet_game_details* pDetails, char* pPlayer_name) {
    NOT_IMPLEMENTED();
}

// Offset: 5220
// Size: 392
int NetJoinGame(tNet_game_details* pDetails, char* pPlayer_name, int pCar_index) {
    int result;
    tNet_message* the_message;
    tU32 start_time;
    NOT_IMPLEMENTED();
}

// Offset: 5612
// Size: 56
void NetObtainSystemUserName(char* pName, int pMax_length) {
    PDNetObtainSystemUserName(pName, pMax_length);
    pName[9] = 0;
}

// Offset: 5668
// Size: 51
tU32 NetExtractGameID(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 5720
// Size: 51
tPlayer_ID NetExtractPlayerID(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 5772
// Size: 117
int NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {
    NOT_IMPLEMENTED();
}

// Offset: 5892
// Size: 170
int NetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 6064
// Size: 107
int NetSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

// Offset: 6172
// Size: 62
int NetReplyToMessage(tNet_game_details* pDetails, tNet_message* pIncoming_message, tNet_message* pReply_message) {
    NOT_IMPLEMENTED();
}

// Offset: 6236
// Size: 86
int NetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

// Offset: 6324
// Size: 642
tU32 NetGetContentsSize(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    NOT_IMPLEMENTED();
}

// Offset: 6968
// Size: 60
tU32 NetGetMessageSize(tNet_message_type pType, tS32 pSize_decider) {
    NOT_IMPLEMENTED();
}

// Offset: 7028
// Size: 111
tS32 NetCalcSizeDecider(tNet_contents* pContents) {
    tS32 the_decider;
    NOT_IMPLEMENTED();
}

// Offset: 7140
// Size: 115
tNet_message* NetBuildMessage(tNet_message_type pType, tS32 pSize_decider) {
    tNet_message* the_message;
    tU32 the_size;
    NOT_IMPLEMENTED();
}

// Offset: 7256
// Size: 230
tNet_contents* NetGetToHostContents(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    tNet_contents* contents;
    NOT_IMPLEMENTED();
}

// Offset: 7488
// Size: 230
tNet_contents* NetGetBroadcastContents(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    tNet_contents* contents;
    NOT_IMPLEMENTED();
}

// Offset: 7720
// Size: 114
void NetSendMessageStacks() {
    NOT_IMPLEMENTED();
}

// Offset: 7836
// Size: 633
tNet_message* NetAllocateMessage(int pSize) {
    void* pointer;
    void* last_message;
    char* test;
    static int rr_min;
    static int rr_mid;
    static int rr_max;
    tNet_message* message;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 8472
// Size: 96
void NetFreeExcessMemory() {
    void* temp;
    NOT_IMPLEMENTED();
}

// Offset: 8568
// Size: 73
int NetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

// Offset: 8644
// Size: 55
tNet_message* NetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    NOT_IMPLEMENTED();
}

// Offset: 8700
// Size: 102
void ReceivedSendMeDetails(tNet_contents* pContents, void* pSender_address) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

// Offset: 8804
// Size: 80
void ReceivedDetails(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

// Offset: 8884
// Size: 259
void SendOutPlayerList() {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 9144
// Size: 600
void ReceivedJoin(tNet_contents* pContents, void* pSender_address) {
    int i;
    int new_player_count;
    int slot_index;
    tNet_message* message;
    tNet_game_player_info* new_players;
    NOT_IMPLEMENTED();
}

// Offset: 9744
// Size: 364
void KickPlayerOut(tPlayer_ID pID) {
    int i;
    int j;
    int new_player_count;
    tNet_game_player_info* new_players;
    NOT_IMPLEMENTED();
}

// Offset: 10108
// Size: 49
void ReceivedLeave(tNet_contents* pContents, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

// Offset: 10160
// Size: 474
void NetFullScreenMessage(int pStr_index, int pLeave_it_up_there) {
    tU32 start_time;
    char* s;
    int gPixel_buffer_size;
    int restore_screen;
    char* gPixels_copy;
    char* gPalette_copy;
    NOT_IMPLEMENTED();
}

// Offset: 10636
// Size: 76
void HostHasBittenTheDust(int pMessage_index) {
    NOT_IMPLEMENTED();
}

// Offset: 10712
// Size: 47
void ReceivedHosticide(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

// Offset: 10760
// Size: 75
void ConfirmReceipt() {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 10836
// Size: 505
void ReceivedNewPlayerList(tNet_contents* pContents, tNet_message* pM) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 11344
// Size: 89
void ReceivedRaceOver(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

// Offset: 11436
// Size: 250
void ReceivedStatusReport(tNet_contents* pContents, tNet_message* pMessage) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 11688
// Size: 692
void ReceivedStartRace(tNet_contents* pContents) {
    int i;
    int index;
    NOT_IMPLEMENTED();
}

// Offset: 12380
// Size: 99
void ReceivedGuaranteeReply(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 12480
// Size: 75
void ReceivedHeadup(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

// Offset: 12556
// Size: 129
void ReceivedHostQuery(tNet_contents* pContents, tNet_message* pMessage) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

// Offset: 12688
// Size: 140
void ReceivedHostReply(tNet_contents* pContents) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

// Offset: 12828
// Size: 91
void SendGuaranteeReply(tNet_message* pMessage, void* pSender_address) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

// Offset: 12920
// Size: 122
int PlayerIsInList(tPlayer_ID pID) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 13044
// Size: 82
void ReceivedTimeSync(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    NOT_IMPLEMENTED();
}

// Offset: 13128
// Size: 107
void ReceivedConfirm(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 13236
// Size: 37
void ReceivedDisableCar(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

// Offset: 13276
// Size: 37
void ReceivedEnableCar(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

// Offset: 13316
// Size: 103
void ReceivedScores(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 13420
// Size: 696
void ReceivedWasted(tNet_contents* pContents) {
    tNet_game_player_info* victim;
    tNet_game_player_info* culprit;
    char s[256];
    tCar_spec* car;
    static tS32 last_got_wasted_time;
    static tS32 last_wasted_em_time;
    static tS32 last_wasty_message_time;
    static tNet_game_player_info* last_culprit;
    static tNet_game_player_info* last_victim;
    NOT_IMPLEMENTED();
}

// Offset: 14116
// Size: 188
void ReceivedCarDetailsReq(tNet_contents* pContents, void* pSender_address) {
    tNet_message* message;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 14304
// Size: 183
void ReceivedCarDetails(tNet_contents* pContents) {
    int i;
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 14488
// Size: 146
void ReceivedGameScores(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 14636
// Size: 810
void ReceivedMessage(tNet_message* pMessage, void* pSender_address, tU32 pReceive_time) {
    tNet_contents* contents;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 15448
// Size: 177
void NetReceiveAndProcessMessages() {
    void* sender_address;
    tU32 receive_time;
    int old_net_service;
    NOT_IMPLEMENTED();
}

// Offset: 15628
// Size: 84
void BroadcastStatus() {
    NOT_IMPLEMENTED();
}

// Offset: 15712
// Size: 354
void CheckForDisappearees() {
    int j;
    tU32 the_time;
    char s[256];
    char* s2;
    NOT_IMPLEMENTED();
}

// Offset: 16068
// Size: 105
void CheckForPendingStartRace() {
    NOT_IMPLEMENTED();
}

// Offset: 16176
// Size: 223
void NetService(int pIn_race) {
    tU32 time;
    static tU32 last_status_broadcast;
    SILENT_STUB();
}

// Offset: 16400
// Size: 87
void NetFinishRace(tNet_game_details* pDetails, tRace_over_reason pReason) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// Offset: 16488
// Size: 157
void NetPlayerStatusChanged(tPlayer_status pNew_status) {
    LOG_TRACE("(%d)", pNew_status);
    tNet_message* the_message;

    if (gNet_mode && pNew_status != gNet_players[gThis_net_player_index].player_status) {
        gNet_players[gThis_net_player_index].player_status = pNew_status;
        the_message = NetBuildMessage(0xAu, 0);
        the_message->contents.data.report.status = gNet_players[gThis_net_player_index].player_status;
        NetSendMessageToAllPlayers(gCurrent_net_game, the_message);
        if (gProgram_state.current_car.disabled && pNew_status >= ePlayer_status_racing && pNew_status != ePlayer_status_recovering) {
            EnableCar(&gProgram_state.current_car);
        } else if (!gProgram_state.current_car.disabled && pNew_status < ePlayer_status_racing) {
            DisableCar(&gProgram_state.current_car);
        }
    }
}

// Offset: 16648
// Size: 56
tPlayer_status NetGetPlayerStatus() {
    NOT_IMPLEMENTED();
}

// Offset: 16704
// Size: 155
int NetGuaranteedSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    int i;
    int err;
    NOT_IMPLEMENTED();
}

// Offset: 16860
// Size: 133
int NetGuaranteedSendMessageToEverybody(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    NOT_IMPLEMENTED();
}

// Offset: 16996
// Size: 62
int NetGuaranteedSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    NOT_IMPLEMENTED();
}

// Offset: 17060
// Size: 243
int NetGuaranteedSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer, int (*pNotifyFail)(tU32, tNet_message*)) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 17304
// Size: 367
int NetGuaranteedSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress, int (*pNotifyFail)(tU32, tNet_message*)) {
    NOT_IMPLEMENTED();
}

// Offset: 17672
// Size: 536
void ResendGuaranteedMessages() {
    int i;
    int j;
    tU32 time;
    NOT_IMPLEMENTED();
}

// Offset: 18208
// Size: 66
int SampleFailNotifier(tU32 pAge, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

// Offset: 18276
// Size: 77
void NetWaitForGuaranteeReplies() {
    tU32 start_time;
    NOT_IMPLEMENTED();
}

// Offset: 18356
// Size: 114
tNet_game_player_info* NetPlayerFromID(tPlayer_ID pPlayer) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 18472
// Size: 78
tCar_spec* NetCarFromPlayerID(tPlayer_ID pPlayer) {
    int i;
    tNet_game_player_info* player;
    NOT_IMPLEMENTED();
}

// Offset: 18552
// Size: 114
tNet_game_player_info* NetPlayerFromCar(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 18668
// Size: 187
tU32 DoCheckSum(tNet_message* pMessage) {
    int i;
    int j;
    tU32 the_sum;
    tU32* p;
    tU8* q;
    NOT_IMPLEMENTED();
}

// Offset: 18856
// Size: 37
void GetCheckSum(tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

// Offset: 18896
// Size: 37
void CheckCheckSum(tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}
