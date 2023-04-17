#ifndef _PD_NET_H_
#define _PD_NET_H_

// Jeff:
// Header files are generated from information in the original symbol dump, but we don't exactly know what this used to look like or be called.
// Each platform build included a platform-dependant `sys` and `net` files. From the symbol dump and debug/error messages in the binaries,
// we know of at least `pc-dos/dossys.c`, `pc-dos/dosnet.c`, `Win95sys.c`.
// Functions contained within these files are prefixed with `PD` - we assume that is short for something like `Platform Dependant`.

#include "brender/br_types.h"
#include "dr_types.h"

// void ClearupPDNetworkStuff(void);

// void MATTMessageCheck(char* pFunction_name, tNet_message* pMessage, int pAlleged_size);

// int GetProfileText(char* pDest, int pDest_len, char* pFname, char* pKeyname);

// int GetSocketNumberFromProfileFile(void);

// tU32 EthernetAddressToU32(_IPX_LOCAL_TARGET* pAddr_ipx);

// void NetNowIPXLocalTarget2String(char* pString, _IPX_LOCAL_TARGET* pSock_addr_ipx);

// int GetMessageTypeFromMessage(char* pMessage_str);

// int SameEthernetAddress(_IPX_LOCAL_TARGET* pAddr_ipx1, _IPX_LOCAL_TARGET* pAddr_ipx2);

// _IPX_LOCAL_TARGET* GetIPXAddrFromPlayerID(tPlayer_ID pPlayer_id);

// void MakeMessageToSend(int pMessage_type);

// int ReceiveHostResponses(void);

// int BroadcastMessage(void);

// BOOL hmiIPXCloseSocket(W32 wSocket);

// void hmiIPXListenForPacket(_IPX_ECB* pECB_ptr, tU32 pOffset);

// BOOL hmiIPXPostListen(_IPX_ECB* pECB_ptr, tU32 pOffset);

// BOOL hmiIPXGetData(PSTR pData, tU32 wDSize);

// void hmiIPXSendPacket(_IPX_ECB* sECB, _IPX_ECB** pPacket, PSTR pHeader, W32 wSize);

// BOOL hmiIPXSendDataDirect(PSTR pHeader, W32 wHSize, PSTR pData, W32 wDSize, _NETNOW_NODE_ADDR* sNode);

// void hmiIPXGetInternetworkAddr(_IPX_INTERNET_ADDR* sInterworkAddr);

// void hmiIPXGetLocalTarget(_IPX_LOCAL_TARGET* sNetworkAddr);

// BOOL AllocateRealMem(W32 wSize, PSTR* pPtr, W32* pSegment, tU16* pSelector);

// BOOL FreeRealMem(tU16 pSelector);

// BOOL hmiIPXInstalled(void);

// BOOL hmiIPXOpenSocket(W32 wSocket);

// void GetLargestPacketSizeOoErBetterInsertLinfordChristieJokeHere(void);

// BOOL hmiIPXInitSystem(W32 wSocket);

// void GetIPXToStickItsEarToTheGround(void);

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

#endif
