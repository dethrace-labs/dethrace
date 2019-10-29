#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 201
void ClearupPDNetworkStuff();

// Offset: 204
// Size: 39
// EAX: pFunction_name
// EDX: pMessage
// EBX: pAlleged_size
void MATTMessageCheck(char *pFunction_name, tNet_message *pMessage, int pAlleged_size, unsigned short __unk3__);

// Offset: 244
// Size: 484
// EAX: pDest
// EDX: pDest_len
// EBX: pFname
// ECX: pKeyname
int GetProfileText(char *pDest, int pDest_len, char *pFname, char *pKeyname);

// Offset: 728
// Size: 187
int GetSocketNumberFromProfileFile();

// Offset: 916
// Size: 104
// EAX: pAddr_ipx
tU32 EthernetAddressToU32(_IPX_LOCAL_TARGET *pAddr_ipx);

// Offset: 1020
// Size: 175
// EAX: pString
// EDX: pSock_addr_ipx
void NetNowIPXLocalTarget2String(char *pString, _IPX_LOCAL_TARGET *pSock_addr_ipx, signed char __unk2__);

// Offset: 1196
// Size: 157
// EAX: pMessage_str
int GetMessageTypeFromMessage(char *pMessage_str);

// Offset: 1356
// Size: 95
// EAX: pAddr_ipx1
// EDX: pAddr_ipx2
int SameEthernetAddress(_IPX_LOCAL_TARGET *pAddr_ipx1, _IPX_LOCAL_TARGET *pAddr_ipx2);

// Offset: 1452
// Size: 194
// EAX: pPlayer_id
_IPX_LOCAL_TARGET* GetIPXAddrFromPlayerID(tPlayer_ID pPlayer_id);

// Offset: 1648
// Size: 67
// EAX: pMessage_type
void MakeMessageToSend(int pMessage_type);

// Offset: 1716
// Size: 514
int ReceiveHostResponses();

// Offset: 2232
// Size: 229
int BroadcastMessage();

// Offset: 2464
// Size: 174
// EAX: wSocket
BOOL hmiIPXCloseSocket(W32 wSocket);

// Offset: 2640
// Size: 337
// EAX: pECB_ptr
// EDX: pOffset
void hmiIPXListenForPacket(_IPX_ECB *pECB_ptr, tU32 pOffset);

// Offset: 2980
// Size: 59
// EAX: pECB_ptr
// EDX: pOffset
BOOL hmiIPXPostListen(_IPX_ECB *pECB_ptr, tU32 pOffset);

// Offset: 3040
// Size: 278
BOOL hmiIPXGetData(PSTR pData, tU32 wDSize);

// Offset: 3320
// Size: 423
// EAX: sECB
// EDX: pPacket
// EBX: pHeader
// ECX: wSize
void hmiIPXSendPacket(_IPX_ECB *sECB, _IPX_ECB **pPacket, PSTR pHeader, W32 wSize);

// Offset: 3744
// Size: 497
// EAX: pHeader
// EDX: wHSize
// EBX: pData
// ECX: wDSize
BOOL hmiIPXSendDataDirect(PSTR pHeader, W32 wHSize, PSTR pData, W32 wDSize, _NETNOW_NODE_ADDR *sNode);

// Offset: 4244
// Size: 183
// EAX: sInterworkAddr
void hmiIPXGetInternetworkAddr(_IPX_INTERNET_ADDR *sInterworkAddr);

// Offset: 4428
// Size: 211
// EAX: sNetworkAddr
void hmiIPXGetLocalTarget(_IPX_LOCAL_TARGET *sNetworkAddr);

// Offset: 4640
// Size: 224
// EAX: wSize
// EDX: pPtr
// EBX: pSegment
// ECX: pSelector
BOOL AllocateRealMem(W32 wSize, PSTR *pPtr, W32 *pSegment, tU16 *pSelector);

// Offset: 4864
// Size: 165
// EAX: pSelector
BOOL FreeRealMem(tU16 pSelector);

// Offset: 5032
// Size: 210
BOOL hmiIPXInstalled();

// Offset: 5244
// Size: 204
// EAX: wSocket
BOOL hmiIPXOpenSocket(W32 wSocket);

// Offset: 5448
// Size: 203
void GetLargestPacketSizeOoErBetterInsertLinfordChristieJokeHere();

// Offset: 5652
// Size: 510
// EAX: wSocket
BOOL hmiIPXInitSystem(W32 wSocket);

// Offset: 6164
// Size: 86
void GetIPXToStickItsEarToTheGround();

// Offset: 6252
// Size: 342
int PDNetInitialise();

// Offset: 6596
// Size: 63
int PDNetShutdown();

// Offset: 6660
// Size: 101
void PDNetStartProducingJoinList();

// Offset: 6764
// Size: 81
void PDNetEndJoinList();

// Offset: 6848
// Size: 369
// EAX: pGame
// EDX: pIndex
int PDNetGetNextJoinGame(tNet_game_details *pGame);

// Offset: 7220
// Size: 51
// EAX: pDetails
void PDNetDisposeGameDetails(tNet_game_details *pDetails);

// Offset: 7272
// Size: 72
// EAX: pDetails
// EDX: pHost_name
// EBX: pHost_address
int PDNetHostGame(tNet_game_details *pDetails, char *pHost_name);

// Offset: 7344
// Size: 62
// EAX: pDetails
// EDX: pPlayer_name
int PDNetJoinGame(tNet_game_details *pDetails);

// Offset: 7408
// Size: 51
// EAX: pDetails
void PDNetLeaveGame(tNet_game_details *pDetails);

// Offset: 7460
// Size: 51
// EAX: pDetails
void PDNetHostFinishGame(tNet_game_details *pDetails);

// Offset: 7512
// Size: 74
// EAX: pDetails
tU32 PDNetExtractGameID(tNet_game_details *pDetails);

// Offset: 7588
// Size: 78
// EAX: pDetails
tPlayer_ID PDNetExtractPlayerID(tNet_game_details *pDetails);

// Offset: 7668
// Size: 65
// EAX: pName
// EDX: pMax_length
void PDNetObtainSystemUserName(char *pName, int pMax_length);

// Offset: 7736
// Size: 223
// EAX: pDetails
// EDX: pMessage
// EBX: pPlayer
int PDNetSendMessageToPlayer(tNet_game_details *pDetails, tNet_message *pMessage, tPlayer_ID pPlayer);

// Offset: 7960
// Size: 274
// EAX: pDetails
// EDX: pMessage
int PDNetSendMessageToAllPlayers(tNet_game_details *pDetails, tNet_message *pMessage);

// Offset: 8236
// Size: 420
// EAX: pDetails
// EDX: pSender_address
tNet_message* PDNetGetNextMessage(tNet_game_details *pDetails, void **pSender_address);

// Offset: 8656
// Size: 76
// EAX: pSize
// EDX: pSize_decider
tNet_message* PDNetAllocateMessage(tU32 pSize, tS32 pSize_decider);

// Offset: 8732
// Size: 63
// EAX: pDetails
// EDX: pMessage
void PDNetDisposeMessage(tNet_game_details *pDetails, tNet_message *pMessage);

// Offset: 8796
// Size: 62
// EAX: pPlayer
// EDX: pSender_address
void PDNetSetPlayerSystemInfo(tNet_game_player_info *pPlayer);

// Offset: 8860
// Size: 51
// EAX: pPlayer
void PDNetDisposePlayer(tNet_game_player_info *pPlayer);

// Offset: 8912
// Size: 202
// EAX: pDetails
// EDX: pMessage
// EBX: pAddress
int PDNetSendMessageToAddress(tNet_game_details *pDetails, tNet_message *pMessage);

// Offset: 9116
// Size: 61
// EAX: pDetails
int PDNetInitClient(tNet_game_details *pDetails);

// Offset: 9180
// Size: 44
int PDNetGetHeaderSize();

