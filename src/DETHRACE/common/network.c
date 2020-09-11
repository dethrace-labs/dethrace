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

int NetInitialise() {
    NOT_IMPLEMENTED();
}

int NetShutdown() {
    int i;
    NOT_IMPLEMENTED();
}

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

void DisableNetService() {
    NOT_IMPLEMENTED();
}

void ReenableNetService() {
    NOT_IMPLEMENTED();
}

int PermitNetServiceReentrancy() {
    NOT_IMPLEMENTED();
}

void HaltNetServiceReentrancy() {
    NOT_IMPLEMENTED();
}

void NetSendHeadupToAllPlayers(char* pMessage) {
    tNet_contents* the_contents;
    NOT_IMPLEMENTED();
}

void NetSendHeadupToEverybody(char* pMessage) {
    tNet_contents* the_contents;
    NOT_IMPLEMENTED();
}

void NetSendHeadupToPlayer(char* pMessage, tPlayer_ID pPlayer) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

void InitialisePlayerStati() {
    NOT_IMPLEMENTED();
}

void LeaveTempGame() {
    NOT_IMPLEMENTED();
}

void DisposeCurrentJoinPollGame() {
    NOT_IMPLEMENTED();
}

void DoNextJoinPoll() {
    NOT_IMPLEMENTED();
}

void NetStartProducingJoinList(void (*pAdd_proc)(tNet_game_details*)) {
    NOT_IMPLEMENTED();
}

void NetEndJoinList() {
    NOT_IMPLEMENTED();
}

void NetDisposePIDGameInfo(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

void NetDisposeGameDetails(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

tNet_game_details* NetAllocatePIDGameDetails() {
    NOT_IMPLEMENTED();
}

void NetLeaveGameLowLevel(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

void NetLeaveGame(tNet_game_details* pNet_game) {
    tNet_message* the_message;
    char s[256];
    char* s2;
    int i;
    int must_revert_reentrancy;
    NOT_IMPLEMENTED();
}

void NetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address) {
    NOT_IMPLEMENTED();
}

void NetDisposePlayer(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

void FillInThisPlayer(tNet_game_details* pGame, tNet_game_player_info* pPlayer, int pCar_index, int pHost) {
    NOT_IMPLEMENTED();
}

void LoadCarN(int pIndex, tNet_game_player_info* pPlayer) {
    int switched_res;
    NOT_IMPLEMENTED();
}

void DisposeCarN(int pIndex) {
    int i;
    int j;
    NOT_IMPLEMENTED();
}

void PlayerHasLeft(int pIndex) {
    NOT_IMPLEMENTED();
}

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

tNet_game_details* NetHostGame(tNet_game_type pGame_type, tNet_game_options* pOptions, int pStart_rank, char* pHost_name, int pCar_index) {
    tNet_game_details* game;
    void* host_address;
    tNet_game_player_info me;
    NOT_IMPLEMENTED();
}

int NetInitClient(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

int NetJoinGameLowLevel(tNet_game_details* pDetails, char* pPlayer_name) {
    NOT_IMPLEMENTED();
}

int NetJoinGame(tNet_game_details* pDetails, char* pPlayer_name, int pCar_index) {
    int result;
    tNet_message* the_message;
    tU32 start_time;
    NOT_IMPLEMENTED();
}

void NetObtainSystemUserName(char* pName, int pMax_length) {
    PDNetObtainSystemUserName(pName, pMax_length);
    pName[9] = 0;
}

tU32 NetExtractGameID(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

tPlayer_ID NetExtractPlayerID(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

int NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {
    NOT_IMPLEMENTED();
}

int NetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer) {
    int i;
    NOT_IMPLEMENTED();
}

int NetSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

int NetReplyToMessage(tNet_game_details* pDetails, tNet_message* pIncoming_message, tNet_message* pReply_message) {
    NOT_IMPLEMENTED();
}

int NetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

tU32 NetGetContentsSize(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    NOT_IMPLEMENTED();
}

tU32 NetGetMessageSize(tNet_message_type pType, tS32 pSize_decider) {
    NOT_IMPLEMENTED();
}

tS32 NetCalcSizeDecider(tNet_contents* pContents) {
    tS32 the_decider;
    NOT_IMPLEMENTED();
}

tNet_message* NetBuildMessage(tNet_message_type pType, tS32 pSize_decider) {
    tNet_message* the_message;
    tU32 the_size;
    NOT_IMPLEMENTED();
}

tNet_contents* NetGetToHostContents(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    tNet_contents* contents;
    NOT_IMPLEMENTED();
}

tNet_contents* NetGetBroadcastContents(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    tNet_contents* contents;
    NOT_IMPLEMENTED();
}

void NetSendMessageStacks() {
    NOT_IMPLEMENTED();
}

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

void NetFreeExcessMemory() {
    void* temp;
    NOT_IMPLEMENTED();
}

int NetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

tNet_message* NetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    NOT_IMPLEMENTED();
}

void ReceivedSendMeDetails(tNet_contents* pContents, void* pSender_address) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

void ReceivedDetails(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

void SendOutPlayerList() {
    int i;
    NOT_IMPLEMENTED();
}

void ReceivedJoin(tNet_contents* pContents, void* pSender_address) {
    int i;
    int new_player_count;
    int slot_index;
    tNet_message* message;
    tNet_game_player_info* new_players;
    NOT_IMPLEMENTED();
}

void KickPlayerOut(tPlayer_ID pID) {
    int i;
    int j;
    int new_player_count;
    tNet_game_player_info* new_players;
    NOT_IMPLEMENTED();
}

void ReceivedLeave(tNet_contents* pContents, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

void NetFullScreenMessage(int pStr_index, int pLeave_it_up_there) {
    tU32 start_time;
    char* s;
    int gPixel_buffer_size;
    int restore_screen;
    char* gPixels_copy;
    char* gPalette_copy;
    NOT_IMPLEMENTED();
}

void HostHasBittenTheDust(int pMessage_index) {
    NOT_IMPLEMENTED();
}

void ReceivedHosticide(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

void ConfirmReceipt() {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

void ReceivedNewPlayerList(tNet_contents* pContents, tNet_message* pM) {
    int i;
    NOT_IMPLEMENTED();
}

void ReceivedRaceOver(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

void ReceivedStatusReport(tNet_contents* pContents, tNet_message* pMessage) {
    int i;
    NOT_IMPLEMENTED();
}

void ReceivedStartRace(tNet_contents* pContents) {
    int i;
    int index;
    NOT_IMPLEMENTED();
}

void ReceivedGuaranteeReply(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

void ReceivedHeadup(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

void ReceivedHostQuery(tNet_contents* pContents, tNet_message* pMessage) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

void ReceivedHostReply(tNet_contents* pContents) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

void SendGuaranteeReply(tNet_message* pMessage, void* pSender_address) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

int PlayerIsInList(tPlayer_ID pID) {
    int i;
    NOT_IMPLEMENTED();
}

void ReceivedTimeSync(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    NOT_IMPLEMENTED();
}

void ReceivedConfirm(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

void ReceivedDisableCar(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

void ReceivedEnableCar(tNet_contents* pContents) {
    NOT_IMPLEMENTED();
}

void ReceivedScores(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

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

void ReceivedCarDetailsReq(tNet_contents* pContents, void* pSender_address) {
    tNet_message* message;
    int i;
    NOT_IMPLEMENTED();
}

void ReceivedCarDetails(tNet_contents* pContents) {
    int i;
    int j;
    NOT_IMPLEMENTED();
}

void ReceivedGameScores(tNet_contents* pContents) {
    int i;
    NOT_IMPLEMENTED();
}

void ReceivedMessage(tNet_message* pMessage, void* pSender_address, tU32 pReceive_time) {
    tNet_contents* contents;
    int i;
    NOT_IMPLEMENTED();
}

void NetReceiveAndProcessMessages() {
    void* sender_address;
    tU32 receive_time;
    int old_net_service;
    NOT_IMPLEMENTED();
}

void BroadcastStatus() {
    NOT_IMPLEMENTED();
}

void CheckForDisappearees() {
    int j;
    tU32 the_time;
    char s[256];
    char* s2;
    NOT_IMPLEMENTED();
}

void CheckForPendingStartRace() {
    NOT_IMPLEMENTED();
}

void NetService(int pIn_race) {
    tU32 time;
    static tU32 last_status_broadcast;
    SILENT_STUB();
}

void NetFinishRace(tNet_game_details* pDetails, tRace_over_reason pReason) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

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

tPlayer_status NetGetPlayerStatus() {
    NOT_IMPLEMENTED();
}

int NetGuaranteedSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    int i;
    int err;
    NOT_IMPLEMENTED();
}

int NetGuaranteedSendMessageToEverybody(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    NOT_IMPLEMENTED();
}

int NetGuaranteedSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    NOT_IMPLEMENTED();
}

int NetGuaranteedSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer, int (*pNotifyFail)(tU32, tNet_message*)) {
    int i;
    NOT_IMPLEMENTED();
}

int NetGuaranteedSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress, int (*pNotifyFail)(tU32, tNet_message*)) {
    NOT_IMPLEMENTED();
}

void ResendGuaranteedMessages() {
    int i;
    int j;
    tU32 time;
    NOT_IMPLEMENTED();
}

int SampleFailNotifier(tU32 pAge, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

void NetWaitForGuaranteeReplies() {
    tU32 start_time;
    NOT_IMPLEMENTED();
}

tNet_game_player_info* NetPlayerFromID(tPlayer_ID pPlayer) {
    int i;
    NOT_IMPLEMENTED();
}

tCar_spec* NetCarFromPlayerID(tPlayer_ID pPlayer) {
    int i;
    tNet_game_player_info* player;
    NOT_IMPLEMENTED();
}

tNet_game_player_info* NetPlayerFromCar(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

tU32 DoCheckSum(tNet_message* pMessage) {
    int i;
    int j;
    tU32 the_sum;
    tU32* p;
    tU8* q;
    NOT_IMPLEMENTED();
}

void GetCheckSum(tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

void CheckCheckSum(tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}
