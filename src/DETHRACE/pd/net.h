#ifndef _PD_NET_H_
#define _PD_NET_H_

// Jeff:
// Header files are generated from information in the original symbol dump, but we don't exactly know what this used to look like or be called.
// Each platform build included a platform-dependant `sys` and `net` files. From the symbol dump and debug/error messages in the binaries,
// we know of at least `pc-dos/dossys.c`, `pc-dos/dosnet.c`, `Win95sys.c`.
// Functions contained within these files are prefixed with `PD` - we assume that is short for something like `Platform Dependant`.

#include "dr_types.h"

void ClearupPDNetworkStuff(void);

void MATTMessageCheck(char* pFunction_name, tNet_message* pMessage, int pAlleged_size);

int GetMessageTypeFromMessage(char* pMessage_str);

void MakeMessageToSend(int pMessage_type);

int ReceiveHostResponses(void);

int BroadcastMessage(void);

int PDNetInitialise(void);

int PDNetShutdown(void);

void PDNetStartProducingJoinList(void);

void PDNetEndJoinList(void);

int PDNetGetNextJoinGame(tNet_game_details* pGame, int pIndex);

void PDNetDisposeGameDetails(tNet_game_details* pDetails);

int PDNetHostGame(tNet_game_details* pDetails, char* pHost_name, void** pHost_address);

int PDNetJoinGame(tNet_game_details* pDetails, char* pPlayer_name);

void PDNetLeaveGame(tNet_game_details* pDetails);

void PDNetHostFinishGame(tNet_game_details* pDetails);

tU32 PDNetExtractGameID(tNet_game_details* pDetails);

tPlayer_ID PDNetExtractPlayerID(tNet_game_details* pDetails);

void PDNetObtainSystemUserName(char* pName, int pMax_length);

int PDNetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer);

int PDNetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage);

tNet_message* PDNetGetNextMessage(tNet_game_details* pDetails, void** pSender_address);

tNet_message* PDNetAllocateMessage(tU32 pSize, tS32 pSize_decider);

void PDNetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage);

void PDNetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address);

void PDNetDisposePlayer(tNet_game_player_info* pPlayer);

int PDNetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress);

int PDNetInitClient(tNet_game_details* pDetails);

int PDNetGetHeaderSize(void);

// dethrace added
void PDNetCopyFromNative(tCopyable_sockaddr_in* pAddress, struct sockaddr_in* sock);
void PDNetCopyToNative(struct sockaddr_in* sock, tCopyable_sockaddr_in* pAddress);

#endif
