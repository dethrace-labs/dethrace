#include "network.h"
#include "brender.h"
#include "controls.h"
#include "displays.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "netgame.h"
#include "pd/net.h"
#include "pd/sys.h"
#include "utility.h"
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

// IDA: int __cdecl NetInitialise()
int NetInitialise() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl NetShutdown()
int NetShutdown() {
    int err;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ShutdownNetIfRequired()
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

// IDA: void __cdecl DisableNetService()
void DisableNetService() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ReenableNetService()
void ReenableNetService() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PermitNetServiceReentrancy()
int PermitNetServiceReentrancy() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl HaltNetServiceReentrancy()
void HaltNetServiceReentrancy() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetSendHeadupToAllPlayers(char *pMessage@<EAX>)
void NetSendHeadupToAllPlayers(char* pMessage) {
    tNet_contents* the_contents;
    LOG_TRACE("(\"%s\")", pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetSendHeadupToEverybody(char *pMessage@<EAX>)
void NetSendHeadupToEverybody(char* pMessage) {
    tNet_contents* the_contents;
    LOG_TRACE("(\"%s\")", pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetSendHeadupToPlayer(char *pMessage@<EAX>, tPlayer_ID pPlayer@<EDX>)
void NetSendHeadupToPlayer(char* pMessage, tPlayer_ID pPlayer) {
    tNet_message* message;
    LOG_TRACE("(\"%s\", %d)", pMessage, pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitialisePlayerStati()
void InitialisePlayerStati() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LeaveTempGame()
void LeaveTempGame() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeCurrentJoinPollGame()
void DisposeCurrentJoinPollGame() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoNextJoinPoll()
void DoNextJoinPoll() {
    tNet_message* the_message;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetStartProducingJoinList(void (*pAdd_proc)(tNet_game_details*)@<EAX>)
void NetStartProducingJoinList(void (*pAdd_proc)(tNet_game_details*)) {
    LOG_TRACE("(%p)", pAdd_proc);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NetEndJoinList()
void NetEndJoinList() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetDisposePIDGameInfo(tNet_game_details *pDetails@<EAX>)
void NetDisposePIDGameInfo(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetDisposeGameDetails(tNet_game_details *pDetails@<EAX>)
void NetDisposeGameDetails(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: tNet_game_details* __cdecl NetAllocatePIDGameDetails()
tNet_game_details* NetAllocatePIDGameDetails() {
    tNet_game_details* game;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetLeaveGameLowLevel(tNet_game_details *pDetails@<EAX>)
void NetLeaveGameLowLevel(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetLeaveGame(tNet_game_details *pNet_game@<EAX>)
void NetLeaveGame(tNet_game_details* pNet_game) {
    tNet_message* the_message;
    char s[256];
    char* s2;
    int i;
    int must_revert_reentrancy;
    LOG_TRACE("(%p)", pNet_game);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetSetPlayerSystemInfo(tNet_game_player_info *pPlayer@<EAX>, void *pSender_address@<EDX>)
void NetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address) {
    LOG_TRACE("(%p, %p)", pPlayer, pSender_address);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetDisposePlayer(tNet_game_player_info *pPlayer@<EAX>)
void NetDisposePlayer(tNet_game_player_info* pPlayer) {
    LOG_TRACE("(%p)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FillInThisPlayer(tNet_game_details *pGame@<EAX>, tNet_game_player_info *pPlayer@<EDX>, int pCar_index@<EBX>, int pHost@<ECX>)
void FillInThisPlayer(tNet_game_details* pGame, tNet_game_player_info* pPlayer, int pCar_index, int pHost) {
    LOG_TRACE("(%p, %p, %d, %d)", pGame, pPlayer, pCar_index, pHost);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LoadCarN(int pIndex@<EAX>, tNet_game_player_info *pPlayer@<EDX>)
void LoadCarN(int pIndex, tNet_game_player_info* pPlayer) {
    int switched_res;
    LOG_TRACE("(%d, %p)", pIndex, pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeCarN(int pIndex@<EAX>)
void DisposeCarN(int pIndex) {
    int i;
    int j;
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayerHasLeft(int pIndex@<EAX>)
void PlayerHasLeft(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayersChanged(int pNew_count@<EAX>, tNet_game_player_info *pNew_players@<EDX>)
void NetPlayersChanged(int pNew_count, tNet_game_player_info* pNew_players) {
    int i;
    int j;
    int k;
    int switched_res;
    int new_player;
    int player_still_there;
    tPlayer_ID old_fox_it;
    LOG_TRACE("(%d, %p)", pNew_count, pNew_players);
    NOT_IMPLEMENTED();
}

// IDA: tNet_game_details* __usercall NetHostGame@<EAX>(tNet_game_type pGame_type@<EAX>, tNet_game_options *pOptions@<EDX>, int pStart_rank@<EBX>, char *pHost_name@<ECX>, int pCar_index)
tNet_game_details* NetHostGame(tNet_game_type pGame_type, tNet_game_options* pOptions, int pStart_rank, char* pHost_name, int pCar_index) {
    tNet_game_details* game;
    void* host_address;
    tNet_game_player_info me;
    LOG_TRACE("(%d, %p, %d, \"%s\", %d)", pGame_type, pOptions, pStart_rank, pHost_name, pCar_index);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetInitClient@<EAX>(tNet_game_details *pDetails@<EAX>)
int NetInitClient(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetJoinGameLowLevel@<EAX>(tNet_game_details *pDetails@<EAX>, char *pPlayer_name@<EDX>)
int NetJoinGameLowLevel(tNet_game_details* pDetails, char* pPlayer_name) {
    LOG_TRACE("(%p, \"%s\")", pDetails, pPlayer_name);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetJoinGame@<EAX>(tNet_game_details *pDetails@<EAX>, char *pPlayer_name@<EDX>, int pCar_index@<EBX>)
int NetJoinGame(tNet_game_details* pDetails, char* pPlayer_name, int pCar_index) {
    int result;
    tNet_message* the_message;
    tU32 start_time;
    LOG_TRACE("(%p, \"%s\", %d)", pDetails, pPlayer_name, pCar_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetObtainSystemUserName(char *pName@<EAX>, int pMax_length@<EDX>)
void NetObtainSystemUserName(char* pName, int pMax_length) {
    PDNetObtainSystemUserName(pName, pMax_length);
    pName[9] = 0;
}

// IDA: tU32 __usercall NetExtractGameID@<EAX>(tNet_game_details *pDetails@<EAX>)
tU32 NetExtractGameID(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: tPlayer_ID __usercall NetExtractPlayerID@<EAX>(tNet_game_details *pDetails@<EAX>)
tPlayer_ID NetExtractPlayerID(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetSendMessageToAddress@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, void *pAddress@<EBX>)
int NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {
    LOG_TRACE("(%p, %p, %p)", pDetails, pMessage, pAddress);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetSendMessageToPlayer@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, tPlayer_ID pPlayer@<EBX>)
int NetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer) {
    int i;
    LOG_TRACE("(%p, %p, %d)", pDetails, pMessage, pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetSendMessageToHost@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
int NetSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage) {
    LOG_TRACE("(%p, %p)", pDetails, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetReplyToMessage@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pIncoming_message@<EDX>, tNet_message *pReply_message@<EBX>)
int NetReplyToMessage(tNet_game_details* pDetails, tNet_message* pIncoming_message, tNet_message* pReply_message) {
    LOG_TRACE("(%p, %p, %p)", pDetails, pIncoming_message, pReply_message);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetSendMessageToAllPlayers@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
int NetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage) {
    LOG_TRACE("(%p, %p)", pDetails, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall NetGetContentsSize@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
tU32 NetGetContentsSize(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    LOG_TRACE("(%d, %d)", pType, pSize_decider);
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall NetGetMessageSize@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
tU32 NetGetMessageSize(tNet_message_type pType, tS32 pSize_decider) {
    LOG_TRACE("(%d, %d)", pType, pSize_decider);
    NOT_IMPLEMENTED();
}

// IDA: tS32 __usercall NetCalcSizeDecider@<EAX>(tNet_contents *pContents@<EAX>)
tS32 NetCalcSizeDecider(tNet_contents* pContents) {
    tS32 the_decider;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: tNet_message* __usercall NetBuildMessage@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
tNet_message* NetBuildMessage(tNet_message_type pType, tS32 pSize_decider) {
    tNet_message* the_message;
    tU32 the_size;
    LOG_TRACE("(%d, %d)", pType, pSize_decider);
    NOT_IMPLEMENTED();
}

// IDA: tNet_contents* __usercall NetGetToHostContents@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
tNet_contents* NetGetToHostContents(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    tNet_contents* contents;
    LOG_TRACE("(%d, %d)", pType, pSize_decider);
    NOT_IMPLEMENTED();
}

// IDA: tNet_contents* __usercall NetGetBroadcastContents@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
tNet_contents* NetGetBroadcastContents(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    tNet_contents* contents;
    LOG_TRACE("(%d, %d)", pType, pSize_decider);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NetSendMessageStacks()
void NetSendMessageStacks() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tNet_message* __usercall NetAllocateMessage@<EAX>(int pSize@<EAX>)
tNet_message* NetAllocateMessage(int pSize) {
    void* pointer;
    void* last_message;
    char* test;
    static int rr_min;
    static int rr_mid;
    static int rr_max;
    tNet_message* message;
    int i;
    LOG_TRACE("(%d)", pSize);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NetFreeExcessMemory()
void NetFreeExcessMemory() {
    void* temp;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetDisposeMessage@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
int NetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {
    LOG_TRACE("(%p, %p)", pDetails, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: tNet_message* __usercall NetGetNextMessage@<EAX>(tNet_game_details *pDetails@<EAX>, void **pSender_address@<EDX>)
tNet_message* NetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    LOG_TRACE("(%p, %p)", pDetails, pSender_address);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedSendMeDetails(tNet_contents *pContents@<EAX>, void *pSender_address@<EDX>)
void ReceivedSendMeDetails(tNet_contents* pContents, void* pSender_address) {
    tNet_message* message;
    LOG_TRACE("(%p, %p)", pContents, pSender_address);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedDetails(tNet_contents *pContents@<EAX>)
void ReceivedDetails(tNet_contents* pContents) {
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SendOutPlayerList()
void SendOutPlayerList() {
    tNet_message* message;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedJoin(tNet_contents *pContents@<EAX>, void *pSender_address@<EDX>)
void ReceivedJoin(tNet_contents* pContents, void* pSender_address) {
    int i;
    int new_player_count;
    int slot_index;
    tNet_message* message;
    tNet_game_player_info* new_players;
    LOG_TRACE("(%p, %p)", pContents, pSender_address);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall KickPlayerOut(tPlayer_ID pID@<EAX>)
void KickPlayerOut(tPlayer_ID pID) {
    int i;
    int j;
    int new_player_count;
    tNet_game_player_info* new_players;
    LOG_TRACE("(%d)", pID);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedLeave(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>)
void ReceivedLeave(tNet_contents* pContents, tNet_message* pMessage) {
    LOG_TRACE("(%p, %p)", pContents, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetFullScreenMessage(int pStr_index@<EAX>, int pLeave_it_up_there@<EDX>)
void NetFullScreenMessage(int pStr_index, int pLeave_it_up_there) {
    tU32 start_time;
    char* s;
    // Jeff: added underscore suffix to avoid collisions with samed-named globals
    int gPixel_buffer_size_;
    char* gPixels_copy_;
    char* gPalette_copy_;
    int restore_screen;

    LOG_TRACE("(%d, %d)", pStr_index, pLeave_it_up_there);

    if (pLeave_it_up_there || (gProgram_state.racing && !gInterface_within_race_mode)) {
        restore_screen = 0;
    } else {
        gPixel_buffer_size_ = gBack_screen->height * gBack_screen->row_bytes;
        gPixels_copy_ = BrMemAllocate(gPixel_buffer_size_, 0xB0u);
        gPalette_copy_ = BrMemAllocate(0x400u, 0xB1u);
        memcpy(gPixels_copy_, gBack_screen->pixels, gPixel_buffer_size_);
        memcpy(gPalette_copy_, gCurrent_palette_pixels, 0x400u);
        restore_screen = 1;
    }
    FadePaletteDown();
    LoadFont(FONT_MEDIUMHD);
    ClearEntireScreen();
    if (pStr_index <= 0) {
        s = "FIXED THAT YOU TWISTED BASTARDS";
    } else {
        s = GetMiscString(pStr_index);
    }
    OoerrIveGotTextInMeBoxMissus(
        FONT_MEDIUMHD,
        s,
        gBack_screen,
        0,
        gGraf_specs[gGraf_spec_index].total_height / 2 - gFonts[4].height,
        gGraf_specs[gGraf_spec_index].total_width,
        gGraf_specs[gGraf_spec_index].total_height,
        1);
    PDScreenBufferSwap(0);
    EnsureRenderPalette();
    EnsurePaletteUp();
    if (!pLeave_it_up_there) {
        start_time = PDGetTotalTime();
        while (PDGetTotalTime() - start_time < 3000) {
            ;
        }
        FadePaletteDown();
        if (restore_screen) {
            memcpy(gBack_screen->pixels, gPixels_copy_, gPixel_buffer_size_);
            memcpy(gCurrent_palette_pixels, gPalette_copy_, 0x400u);
            BrMemFree(gPixels_copy_);
            BrMemFree(gPalette_copy_);
            PDScreenBufferSwap(0);
            FadePaletteUp();
        } else {
            ClearEntireScreen();
        }
    }
}

// IDA: void __usercall HostHasBittenTheDust(int pMessage_index@<EAX>)
void HostHasBittenTheDust(int pMessage_index) {
    LOG_TRACE("(%d)", pMessage_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedHosticide(tNet_contents *pContents@<EAX>)
void ReceivedHosticide(tNet_contents* pContents) {
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ConfirmReceipt()
void ConfirmReceipt() {
    tNet_message* the_message;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedNewPlayerList(tNet_contents *pContents@<EAX>, tNet_message *pM@<EDX>)
void ReceivedNewPlayerList(tNet_contents* pContents, tNet_message* pM) {
    int i;
    LOG_TRACE("(%p, %p)", pContents, pM);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedRaceOver(tNet_contents *pContents@<EAX>)
void ReceivedRaceOver(tNet_contents* pContents) {
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedStatusReport(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>)
void ReceivedStatusReport(tNet_contents* pContents, tNet_message* pMessage) {
    int i;
    LOG_TRACE("(%p, %p)", pContents, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedStartRace(tNet_contents *pContents@<EAX>)
void ReceivedStartRace(tNet_contents* pContents) {
    int i;
    int index;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedGuaranteeReply(tNet_contents *pContents@<EAX>)
void ReceivedGuaranteeReply(tNet_contents* pContents) {
    int i;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedHeadup(tNet_contents *pContents@<EAX>)
void ReceivedHeadup(tNet_contents* pContents) {
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedHostQuery(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>)
void ReceivedHostQuery(tNet_contents* pContents, tNet_message* pMessage) {
    tNet_message* message;
    LOG_TRACE("(%p, %p)", pContents, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedHostReply(tNet_contents *pContents@<EAX>)
void ReceivedHostReply(tNet_contents* pContents) {
    tNet_message* message;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SendGuaranteeReply(tNet_message *pMessage@<EAX>, void *pSender_address@<EDX>)
void SendGuaranteeReply(tNet_message* pMessage, void* pSender_address) {
    tNet_message* message;
    LOG_TRACE("(%p, %p)", pMessage, pSender_address);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PlayerIsInList@<EAX>(tPlayer_ID pID@<EAX>)
int PlayerIsInList(tPlayer_ID pID) {
    int i;
    LOG_TRACE("(%d)", pID);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedTimeSync(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>, tU32 pReceive_time@<EBX>)
void ReceivedTimeSync(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    LOG_TRACE("(%p, %p, %d)", pContents, pMessage, pReceive_time);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedConfirm(tNet_contents *pContents@<EAX>)
void ReceivedConfirm(tNet_contents* pContents) {
    int i;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedDisableCar(tNet_contents *pContents@<EAX>)
void ReceivedDisableCar(tNet_contents* pContents) {
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedEnableCar(tNet_contents *pContents@<EAX>)
void ReceivedEnableCar(tNet_contents* pContents) {
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedScores(tNet_contents *pContents@<EAX>)
void ReceivedScores(tNet_contents* pContents) {
    int i;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedWasted(tNet_contents *pContents@<EAX>)
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
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedCarDetailsReq(tNet_contents *pContents@<EAX>, void *pSender_address@<EDX>)
void ReceivedCarDetailsReq(tNet_contents* pContents, void* pSender_address) {
    tNet_message* message;
    int i;
    LOG_TRACE("(%p, %p)", pContents, pSender_address);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedCarDetails(tNet_contents *pContents@<EAX>)
void ReceivedCarDetails(tNet_contents* pContents) {
    int i;
    int j;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedGameScores(tNet_contents *pContents@<EAX>)
void ReceivedGameScores(tNet_contents* pContents) {
    int i;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedMessage(tNet_message *pMessage@<EAX>, void *pSender_address@<EDX>, tU32 pReceive_time@<EBX>)
void ReceivedMessage(tNet_message* pMessage, void* pSender_address, tU32 pReceive_time) {
    tNet_contents* contents;
    int i;
    LOG_TRACE("(%p, %p, %d)", pMessage, pSender_address, pReceive_time);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NetReceiveAndProcessMessages()
void NetReceiveAndProcessMessages() {
    tNet_message* message;
    void* sender_address;
    tU32 receive_time;
    int old_net_service;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BroadcastStatus()
void BroadcastStatus() {
    tNet_message* message;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckForDisappearees()
void CheckForDisappearees() {
    int i;
    int j;
    tU32 the_time;
    char s[256];
    char* s2;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckForPendingStartRace()
void CheckForPendingStartRace() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetService(int pIn_race@<EAX>)
void NetService(int pIn_race) {
    tU32 time;
    static tU32 last_status_broadcast;
    SILENT_STUB();
}

// IDA: void __usercall NetFinishRace(tNet_game_details *pDetails@<EAX>, tRace_over_reason pReason@<EDX>)
void NetFinishRace(tNet_game_details* pDetails, tRace_over_reason pReason) {
    tNet_message* the_message;
    LOG_TRACE("(%p, %d)", pDetails, pReason);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetPlayerStatusChanged(tPlayer_status pNew_status@<EAX>)
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

// IDA: tPlayer_status __cdecl NetGetPlayerStatus()
tPlayer_status NetGetPlayerStatus() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetGuaranteedSendMessageToAllPlayers@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, int (*pNotifyFail)(tU32, tNet_message*)@<EBX>)
int NetGuaranteedSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    int i;
    int err;
    LOG_TRACE("(%p, %p, %p)", pDetails, pMessage, pNotifyFail);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetGuaranteedSendMessageToEverybody@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, int (*pNotifyFail)(tU32, tNet_message*)@<EBX>)
int NetGuaranteedSendMessageToEverybody(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    LOG_TRACE("(%p, %p, %p)", pDetails, pMessage, pNotifyFail);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetGuaranteedSendMessageToHost@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, int (*pNotifyFail)(tU32, tNet_message*)@<EBX>)
int NetGuaranteedSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    LOG_TRACE("(%p, %p, %p)", pDetails, pMessage, pNotifyFail);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetGuaranteedSendMessageToPlayer@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, tPlayer_ID pPlayer@<EBX>, int (*pNotifyFail)(tU32, tNet_message*)@<ECX>)
int NetGuaranteedSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer, int (*pNotifyFail)(tU32, tNet_message*)) {
    int i;
    LOG_TRACE("(%p, %p, %d, %p)", pDetails, pMessage, pPlayer, pNotifyFail);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NetGuaranteedSendMessageToAddress@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, void *pAddress@<EBX>, int (*pNotifyFail)(tU32, tNet_message*)@<ECX>)
int NetGuaranteedSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress, int (*pNotifyFail)(tU32, tNet_message*)) {
    LOG_TRACE("(%p, %p, %p, %p)", pDetails, pMessage, pAddress, pNotifyFail);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResendGuaranteedMessages()
void ResendGuaranteedMessages() {
    int i;
    int j;
    tU32 time;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SampleFailNotifier@<EAX>(tU32 pAge@<EAX>, tNet_message *pMessage@<EDX>)
int SampleFailNotifier(tU32 pAge, tNet_message* pMessage) {
    LOG_TRACE("(%d, %p)", pAge, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl NetWaitForGuaranteeReplies()
void NetWaitForGuaranteeReplies() {
    tU32 start_time;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tNet_game_player_info* __usercall NetPlayerFromID@<EAX>(tPlayer_ID pPlayer@<EAX>)
tNet_game_player_info* NetPlayerFromID(tPlayer_ID pPlayer) {
    int i;
    LOG_TRACE("(%d)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: tCar_spec* __usercall NetCarFromPlayerID@<EAX>(tPlayer_ID pPlayer@<EAX>)
tCar_spec* NetCarFromPlayerID(tPlayer_ID pPlayer) {
    int i;
    tNet_game_player_info* player;
    LOG_TRACE("(%d)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: tNet_game_player_info* __usercall NetPlayerFromCar@<EAX>(tCar_spec *pCar@<EAX>)
tNet_game_player_info* NetPlayerFromCar(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall DoCheckSum@<EAX>(tNet_message *pMessage@<EAX>)
tU32 DoCheckSum(tNet_message* pMessage) {
    int i;
    int j;
    tU32 the_sum;
    tU32* p;
    tU8* q;
    LOG_TRACE("(%p)", pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetCheckSum(tNet_message *pMessage@<EAX>)
void GetCheckSum(tNet_message* pMessage) {
    LOG_TRACE("(%p)", pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CheckCheckSum(tNet_message *pMessage@<EAX>)
void CheckCheckSum(tNet_message* pMessage) {
    LOG_TRACE("(%p)", pMessage);
    NOT_IMPLEMENTED();
}
