#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "dr_types.h"
extern tU32 gMess_max_flags;
extern tU32 gMess_mid_flags;
extern tU32 gMess_min_flags;
extern tU32 gGuarantee_number;
extern int gNet_service_disable;
extern int gIn_net_service;
extern int gPlayer_list_batch_number;
extern int gOnly_receive_guarantee_replies;
extern void* gMessage_to_free;
extern tNet_message* gBroadcast_stack;
extern tNet_message* gTo_host_stack;
extern tU32 gLast_flush_message;
extern int gRace_only_flags[33];
extern int gJoin_list_mode;
extern tNet_game_player_info gNew_net_players[6];
extern tGuaranteed_message gGuarantee_list[100];
extern tMid_message* gMid_messages;
extern tU32 gLast_player_list_received;
extern tMin_message* gMin_messages;
extern void (*gAdd_proc)(tNet_game_details*);
extern int gReceiving_batch_number;
extern int gReceiving_new_players;
extern tMax_message* gMax_messages;
extern int gNext_guarantee;
extern tU32 gAsk_time;
extern int gNet_initialised;
extern int gDont_allow_joiners;
extern tNet_game_details* gCurrent_join_poll_game;
extern int gMessage_header_size;
extern int gJoin_poll_index;
extern int gJoin_request_denied;
extern int gHost_died;
extern int gCar_was_taken;
extern int gBastard_has_answered;
extern int gTime_for_next_one;
extern int gReceived_game_scores;

int NetInitialise(void);

int NetShutdown(void);

void ShutdownNetIfRequired(void);

void DisableNetService(void);

void ReenableNetService(void);

int PermitNetServiceReentrancy(void);

void HaltNetServiceReentrancy(void);

void NetSendHeadupToAllPlayers(char* pMessage);

void NetSendHeadupToEverybody(char* pMessage);

void NetSendHeadupToPlayer(char* pMessage, tPlayer_ID pPlayer);

void InitialisePlayerStati(void);

void LeaveTempGame(void);

void DisposeCurrentJoinPollGame(void);

void DoNextJoinPoll(void);

void NetStartProducingJoinList(void (*pAdd_proc)(tNet_game_details*));

void NetEndJoinList(void);

void NetDisposePIDGameInfo(tNet_game_details* pDetails);

void NetDisposeGameDetails(tNet_game_details* pDetails);

tNet_game_details* NetAllocatePIDGameDetails(void);

void NetLeaveGameLowLevel(tNet_game_details* pDetails);

void NetLeaveGame(tNet_game_details* pNet_game);

void NetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address);

void NetDisposePlayer(tNet_game_player_info* pPlayer);

void FillInThisPlayer(tNet_game_details* pGame, tNet_game_player_info* pPlayer, int pCar_index, int pHost);

void LoadCarN(int pIndex, tNet_game_player_info* pPlayer);

void DisposeCarN(int pIndex);

void PlayerHasLeft(int pIndex);

void NetPlayersChanged(int pNew_count, tNet_game_player_info* pNew_players);

tNet_game_details* NetHostGame(tNet_game_type pGame_type, tNet_game_options* pOptions, int pStart_rank, char* pHost_name, int pCar_index);

int NetInitClient(tNet_game_details* pDetails);

int NetJoinGameLowLevel(tNet_game_details* pDetails, char* pPlayer_name);

int NetJoinGame(tNet_game_details* pDetails, char* pPlayer_name, int pCar_index);

void NetObtainSystemUserName(char* pName, int pMax_length);

tU32 NetExtractGameID(tNet_game_details* pDetails);

tPlayer_ID NetExtractPlayerID(tNet_game_details* pDetails);

int NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress);

int NetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer);

int NetSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage);

int NetReplyToMessage(tNet_game_details* pDetails, tNet_message* pIncoming_message, tNet_message* pReply_message);

int NetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage);

tU32 NetGetContentsSize(tNet_message_type pType, tS32 pSize_decider);

tU32 NetGetMessageSize(tNet_message_type pType, tS32 pSize_decider);

tS32 NetCalcSizeDecider(tNet_contents* pContents);

tNet_message* NetBuildMessage(tNet_message_type pType, tS32 pSize_decider);

tNet_contents* NetGetToHostContents(tNet_message_type pType, tS32 pSize_decider);

tNet_contents* NetGetBroadcastContents(tNet_message_type pType, tS32 pSize_decider);

void NetSendMessageStacks(void);

tNet_message* NetAllocateMessage(int pSize);

void NetFreeExcessMemory(void);

int NetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage);

tNet_message* NetGetNextMessage(tNet_game_details* pDetails, void** pSender_address);

void ReceivedSendMeDetails(tNet_contents* pContents, void* pSender_address);

void ReceivedDetails(tNet_contents* pContents);

void SendOutPlayerList(void);

void ReceivedJoin(tNet_contents* pContents, void* pSender_address);

void KickPlayerOut(tPlayer_ID pID);

void ReceivedLeave(tNet_contents* pContents, tNet_message* pMessage);

void NetFullScreenMessage(int pStr_index, int pLeave_it_up_there);

void HostHasBittenTheDust(int pMessage_index);

void ReceivedHosticide(tNet_contents* pContents);

void ConfirmReceipt(void);

void ReceivedNewPlayerList(tNet_contents* pContents, tNet_message* pM);

void ReceivedRaceOver(tNet_contents* pContents);

void ReceivedStatusReport(tNet_contents* pContents, tNet_message* pMessage);

void ReceivedStartRace(tNet_contents* pContents);

void ReceivedGuaranteeReply(tNet_contents* pContents);

void ReceivedHeadup(tNet_contents* pContents);

void ReceivedHostQuery(tNet_contents* pContents, tNet_message* pMessage);

void ReceivedHostReply(tNet_contents* pContents);

void SendGuaranteeReply(tNet_message* pMessage, void* pSender_address);

int PlayerIsInList(tPlayer_ID pID);

void ReceivedTimeSync(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time);

void ReceivedConfirm(tNet_contents* pContents);

void ReceivedDisableCar(tNet_contents* pContents);

void ReceivedEnableCar(tNet_contents* pContents);

void ReceivedScores(tNet_contents* pContents);

void ReceivedWasted(tNet_contents* pContents);

void ReceivedCarDetailsReq(tNet_contents* pContents, void* pSender_address);

void ReceivedCarDetails(tNet_contents* pContents);

void ReceivedGameScores(tNet_contents* pContents);

void ReceivedMessage(tNet_message* pMessage, void* pSender_address, tU32 pReceive_time);

void NetReceiveAndProcessMessages(void);

void BroadcastStatus(void);

void CheckForDisappearees(void);

void CheckForPendingStartRace(void);

void NetService(int pIn_race);

void NetFinishRace(tNet_game_details* pDetails, tRace_over_reason pReason);

void NetPlayerStatusChanged(tPlayer_status pNew_status);

tPlayer_status NetGetPlayerStatus(void);

int NetGuaranteedSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*));

int NetGuaranteedSendMessageToEverybody(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*));

int NetGuaranteedSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*));

int NetGuaranteedSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer, int (*pNotifyFail)(tU32, tNet_message*));

int NetGuaranteedSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress, int (*pNotifyFail)(tU32, tNet_message*));

void ResendGuaranteedMessages(void);

int SampleFailNotifier(tU32 pAge, tNet_message* pMessage);

void NetWaitForGuaranteeReplies(void);

tNet_game_player_info* NetPlayerFromID(tPlayer_ID pPlayer);

tCar_spec* NetCarFromPlayerID(tPlayer_ID pPlayer);

tNet_game_player_info* NetPlayerFromCar(tCar_spec* pCar);

tU32 DoCheckSum(tNet_message* pMessage);

void GetCheckSum(tNet_message* pMessage);

void CheckCheckSum(tNet_message* pMessage);

#endif
