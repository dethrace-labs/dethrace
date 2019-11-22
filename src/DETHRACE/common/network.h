#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 429
int NetInitialise();

// Offset: 432
// Size: 103
int NetShutdown();

// Offset: 536
// Size: 58
void ShutdownNetIfRequired();

// Offset: 596
// Size: 44
void DisableNetService();

// Offset: 640
// Size: 44
void ReenableNetService();

// Offset: 684
// Size: 72
int PermitNetServiceReentrancy();

// Offset: 756
// Size: 44
void HaltNetServiceReentrancy();

// Offset: 800
// Size: 75
// EAX: pMessage
void NetSendHeadupToAllPlayers(char *pMessage);

// Offset: 876
// Size: 110
// EAX: pMessage
void NetSendHeadupToEverybody(char *pMessage);

// Offset: 988
// Size: 142
// EAX: pMessage
// EDX: pPlayer
void NetSendHeadupToPlayer(char *pMessage, tPlayer_ID pPlayer);

// Offset: 1132
// Size: 94
void InitialisePlayerStati();

// Offset: 1228
// Size: 73
void LeaveTempGame();

// Offset: 1304
// Size: 63
void DisposeCurrentJoinPollGame();

// Offset: 1368
// Size: 277
void DoNextJoinPoll();

// Offset: 1648
// Size: 100
// EAX: pAdd_proc
void NetStartProducingJoinList(void (*pAdd_proc)(tNet_game_details*));

// Offset: 1748
// Size: 59
void NetEndJoinList();

// Offset: 1808
// Size: 55
// EAX: pDetails
void NetDisposePIDGameInfo(tNet_game_details *pDetails);

// Offset: 1864
// Size: 51
// EAX: pDetails
void NetDisposeGameDetails(tNet_game_details *pDetails);

// Offset: 1916
// Size: 62
tNet_game_details* NetAllocatePIDGameDetails();

// Offset: 1980
// Size: 66
// EAX: pDetails
void NetLeaveGameLowLevel(tNet_game_details *pDetails);

// Offset: 2048
// Size: 382
// EAX: pNet_game
void NetLeaveGame(tNet_game_details *pNet_game);

// Offset: 2432
// Size: 49
// EAX: pPlayer
// EDX: pSender_address
void NetSetPlayerSystemInfo(tNet_game_player_info *pPlayer, void *pSender_address);

// Offset: 2484
// Size: 45
// EAX: pPlayer
void NetDisposePlayer(tNet_game_player_info *pPlayer);

// Offset: 2532
// Size: 234
// EAX: pGame
// EDX: pPlayer
// EBX: pCar_index
// ECX: pHost
void FillInThisPlayer(tNet_game_details *pGame, tNet_game_player_info *pPlayer, int pCar_index, int pHost);

// Offset: 2768
// Size: 206
// EAX: pIndex
// EDX: pPlayer
void LoadCarN(int pIndex, tNet_game_player_info *pPlayer);

// Offset: 2976
// Size: 266
// EAX: pIndex
void DisposeCarN(int pIndex);

// Offset: 3244
// Size: 154
// EAX: pIndex
void PlayerHasLeft(int pIndex);

// Offset: 3400
// Size: 1321
// EAX: pNew_count
// EDX: pNew_players
void NetPlayersChanged(int pNew_count, tNet_game_player_info *pNew_players);

// Offset: 4724
// Size: 386
// EAX: pGame_type
// EDX: pOptions
// EBX: pStart_rank
// ECX: pHost_name
tNet_game_details* NetHostGame(tNet_game_type pGame_type, tNet_game_options *pOptions, int pStart_rank, char *pHost_name, int pCar_index);

// Offset: 5112
// Size: 51
// EAX: pDetails
int NetInitClient(tNet_game_details *pDetails);

// Offset: 5164
// Size: 55
// EAX: pDetails
// EDX: pPlayer_name
int NetJoinGameLowLevel(tNet_game_details *pDetails, char *pPlayer_name);

// Offset: 5220
// Size: 392
// EAX: pDetails
// EDX: pPlayer_name
// EBX: pCar_index
int NetJoinGame(tNet_game_details *pDetails, char *pPlayer_name, int pCar_index);

// Offset: 5612
// Size: 56
// EAX: pName
// EDX: pMax_length
void NetObtainSystemUserName(char *pName, int pMax_length);

// Offset: 5668
// Size: 51
// EAX: pDetails
tU32 NetExtractGameID(tNet_game_details *pDetails);

// Offset: 5720
// Size: 51
// EAX: pDetails
tPlayer_ID NetExtractPlayerID(tNet_game_details *pDetails);

// Offset: 5772
// Size: 117
// EAX: pDetails
// EDX: pMessage
// EBX: pAddress
int NetSendMessageToAddress(tNet_game_details *pDetails, tNet_message *pMessage, void *pAddress);

// Offset: 5892
// Size: 170
// EAX: pDetails
// EDX: pMessage
// EBX: pPlayer
int NetSendMessageToPlayer(tNet_game_details *pDetails, tNet_message *pMessage, tPlayer_ID pPlayer);

// Offset: 6064
// Size: 107
// EAX: pDetails
// EDX: pMessage
int NetSendMessageToHost(tNet_game_details *pDetails, tNet_message *pMessage);

// Offset: 6172
// Size: 62
// EAX: pDetails
// EDX: pIncoming_message
// EBX: pReply_message
int NetReplyToMessage(tNet_game_details *pDetails, tNet_message *pIncoming_message, tNet_message *pReply_message);

// Offset: 6236
// Size: 86
// EAX: pDetails
// EDX: pMessage
int NetSendMessageToAllPlayers(tNet_game_details *pDetails, tNet_message *pMessage);

// Offset: 6324
// Size: 642
// EAX: pType
// EDX: pSize_decider
tU32 NetGetContentsSize(tNet_message_type pType, tS32 pSize_decider);

// Offset: 6968
// Size: 60
// EAX: pType
// EDX: pSize_decider
tU32 NetGetMessageSize(tNet_message_type pType, tS32 pSize_decider);

// Offset: 7028
// Size: 111
// EAX: pContents
tS32 NetCalcSizeDecider(tNet_contents *pContents);

// Offset: 7140
// Size: 115
// EAX: pType
// EDX: pSize_decider
tNet_message* NetBuildMessage(tNet_message_type pType, tS32 pSize_decider);

// Offset: 7256
// Size: 230
// EAX: pType
// EDX: pSize_decider
tNet_contents* NetGetToHostContents(tNet_message_type pType, tS32 pSize_decider);

// Offset: 7488
// Size: 230
// EAX: pType
// EDX: pSize_decider
tNet_contents* NetGetBroadcastContents(tNet_message_type pType, tS32 pSize_decider);

// Offset: 7720
// Size: 114
void NetSendMessageStacks();

// Offset: 7836
// Size: 633
// EAX: pSize
tNet_message* NetAllocateMessage(int pSize);

// Offset: 8472
// Size: 96
void NetFreeExcessMemory();

// Offset: 8568
// Size: 73
// EAX: pDetails
// EDX: pMessage
int NetDisposeMessage(tNet_game_details *pDetails, tNet_message *pMessage);

// Offset: 8644
// Size: 55
// EAX: pDetails
// EDX: pSender_address
tNet_message* NetGetNextMessage(tNet_game_details *pDetails, void **pSender_address);

// Offset: 8700
// Size: 102
// EAX: pContents
// EDX: pSender_address
void ReceivedSendMeDetails(tNet_contents *pContents, void *pSender_address);

// Offset: 8804
// Size: 80
// EAX: pContents
void ReceivedDetails(tNet_contents *pContents);

// Offset: 8884
// Size: 259
void SendOutPlayerList();

// Offset: 9144
// Size: 600
// EAX: pContents
// EDX: pSender_address
void ReceivedJoin(tNet_contents *pContents, void *pSender_address);

// Offset: 9744
// Size: 364
// EAX: pID
void KickPlayerOut(tPlayer_ID pID);

// Offset: 10108
// Size: 49
// EAX: pContents
// EDX: pMessage
void ReceivedLeave(tNet_contents *pContents, tNet_message *pMessage);

// Offset: 10160
// Size: 474
// EAX: pStr_index
// EDX: pLeave_it_up_there
void NetFullScreenMessage(int pStr_index, int pLeave_it_up_there);

// Offset: 10636
// Size: 76
// EAX: pMessage_index
void HostHasBittenTheDust(int pMessage_index);

// Offset: 10712
// Size: 47
// EAX: pContents
void ReceivedHosticide(tNet_contents *pContents);

// Offset: 10760
// Size: 75
void ConfirmReceipt();

// Offset: 10836
// Size: 505
// EAX: pContents
// EDX: pM
void ReceivedNewPlayerList(tNet_contents *pContents, tNet_message *pM);

// Offset: 11344
// Size: 89
// EAX: pContents
void ReceivedRaceOver(tNet_contents *pContents);

// Offset: 11436
// Size: 250
// EAX: pContents
// EDX: pMessage
void ReceivedStatusReport(tNet_contents *pContents, tNet_message *pMessage);

// Offset: 11688
// Size: 692
// EAX: pContents
void ReceivedStartRace(tNet_contents *pContents);

// Offset: 12380
// Size: 99
// EAX: pContents
void ReceivedGuaranteeReply(tNet_contents *pContents);

// Offset: 12480
// Size: 75
// EAX: pContents
void ReceivedHeadup(tNet_contents *pContents);

// Offset: 12556
// Size: 129
// EAX: pContents
// EDX: pMessage
void ReceivedHostQuery(tNet_contents *pContents, tNet_message *pMessage);

// Offset: 12688
// Size: 140
// EAX: pContents
void ReceivedHostReply(tNet_contents *pContents);

// Offset: 12828
// Size: 91
// EAX: pMessage
// EDX: pSender_address
void SendGuaranteeReply(tNet_message *pMessage, void *pSender_address);

// Offset: 12920
// Size: 122
// EAX: pID
int PlayerIsInList(tPlayer_ID pID);

// Offset: 13044
// Size: 82
// EAX: pContents
// EDX: pMessage
// EBX: pReceive_time
void ReceivedTimeSync(tNet_contents *pContents, tNet_message *pMessage, tU32 pReceive_time);

// Offset: 13128
// Size: 107
// EAX: pContents
void ReceivedConfirm(tNet_contents *pContents);

// Offset: 13236
// Size: 37
// EAX: pContents
void ReceivedDisableCar(tNet_contents *pContents);

// Offset: 13276
// Size: 37
// EAX: pContents
void ReceivedEnableCar(tNet_contents *pContents);

// Offset: 13316
// Size: 103
// EAX: pContents
void ReceivedScores(tNet_contents *pContents);

// Offset: 13420
// Size: 696
// EAX: pContents
void ReceivedWasted(tNet_contents *pContents);

// Offset: 14116
// Size: 188
// EAX: pContents
// EDX: pSender_address
void ReceivedCarDetailsReq(tNet_contents *pContents, void *pSender_address);

// Offset: 14304
// Size: 183
// EAX: pContents
void ReceivedCarDetails(tNet_contents *pContents);

// Offset: 14488
// Size: 146
// EAX: pContents
void ReceivedGameScores(tNet_contents *pContents);

// Offset: 14636
// Size: 810
// EAX: pMessage
// EDX: pSender_address
// EBX: pReceive_time
void ReceivedMessage(tNet_message *pMessage, void *pSender_address, tU32 pReceive_time);

// Offset: 15448
// Size: 177
void NetReceiveAndProcessMessages();

// Offset: 15628
// Size: 84
void BroadcastStatus();

// Offset: 15712
// Size: 354
void CheckForDisappearees();

// Offset: 16068
// Size: 105
void CheckForPendingStartRace();

// Offset: 16176
// Size: 223
// EAX: pIn_race
void NetService(int pIn_race);

// Offset: 16400
// Size: 87
// EAX: pDetails
// EDX: pReason
void NetFinishRace(tNet_game_details *pDetails, tRace_over_reason pReason);

// Offset: 16488
// Size: 157
// EAX: pNew_status
void NetPlayerStatusChanged(tPlayer_status pNew_status);

// Offset: 16648
// Size: 56
tPlayer_status NetGetPlayerStatus();

// Offset: 16704
// Size: 155
// EAX: pDetails
// EDX: pMessage
// EBX: pNotifyFail
int NetGuaranteedSendMessageToAllPlayers(tNet_game_details *pDetails, tNet_message *pMessage, int (*pNotifyFail)(tU32, tNet_message*));

// Offset: 16860
// Size: 133
// EAX: pDetails
// EDX: pMessage
// EBX: pNotifyFail
int NetGuaranteedSendMessageToEverybody(tNet_game_details *pDetails, tNet_message *pMessage, int (*pNotifyFail)(tU32, tNet_message*));

// Offset: 16996
// Size: 62
// EAX: pDetails
// EDX: pMessage
// EBX: pNotifyFail
int NetGuaranteedSendMessageToHost(tNet_game_details *pDetails, tNet_message *pMessage, int (*pNotifyFail)(tU32, tNet_message*));

// Offset: 17060
// Size: 243
// EAX: pDetails
// EDX: pMessage
// EBX: pPlayer
// ECX: pNotifyFail
int NetGuaranteedSendMessageToPlayer(tNet_game_details *pDetails, tNet_message *pMessage, tPlayer_ID pPlayer, int (*pNotifyFail)(tU32, tNet_message*));

// Offset: 17304
// Size: 367
// EAX: pDetails
// EDX: pMessage
// EBX: pAddress
// ECX: pNotifyFail
int NetGuaranteedSendMessageToAddress(tNet_game_details *pDetails, tNet_message *pMessage, void *pAddress, int (*pNotifyFail)(tU32, tNet_message*));

// Offset: 17672
// Size: 536
void ResendGuaranteedMessages();

// Offset: 18208
// Size: 66
// EAX: pAge
// EDX: pMessage
int SampleFailNotifier(tU32 pAge, tNet_message *pMessage);

// Offset: 18276
// Size: 77
void NetWaitForGuaranteeReplies();

// Offset: 18356
// Size: 114
// EAX: pPlayer
tNet_game_player_info* NetPlayerFromID(tPlayer_ID pPlayer);

// Offset: 18472
// Size: 78
// EAX: pPlayer
tCar_spec* NetCarFromPlayerID(tPlayer_ID pPlayer);

// Offset: 18552
// Size: 114
// EAX: pCar
tNet_game_player_info* NetPlayerFromCar(tCar_spec *pCar);

// Offset: 18668
// Size: 187
// EAX: pMessage
tU32 DoCheckSum(tNet_message *pMessage);

// Offset: 18856
// Size: 37
// EAX: pMessage
void GetCheckSum(tNet_message *pMessage);

// Offset: 18896
// Size: 37
// EAX: pMessage
void CheckCheckSum(tNet_message *pMessage);

#endif
