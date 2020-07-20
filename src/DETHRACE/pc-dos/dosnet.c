#include "dosnet.h"

#include "common/errors.h"
#include <stdlib.h>
#include <string.h>

_IPX_ELEMENT gListen_elements[16];
char gLocal_ipx_addr_string[32];
_IPX_ELEMENT gSend_elements[16];
_IPX_LOCAL_TARGET gLocal_addr_ipx;
char gReceive_buffer[512];
tPD_net_player_info gRemote_net_player_info;
_IPX_LOCAL_TARGET gBroadcast_addr_ipx;
tPD_net_player_info gLocal_net_player_info;
char gSend_buffer[512];
tIPX_netnum gNetworks[16];
_IPX_LOCAL_TARGET gRemote_addr_ipx;
tU8* gSend_packet;
W32 gListen_segment;
tU8* gSend_packet_ptr;
W32 gSend_segment;
tU8* gListen_packet;
tU8* gListen_packet_ptr;
size_t gMsg_header_strlen;
int gNumber_of_networks;
int gNumber_of_hosts;
tU32 gNetwork_init_flags;
tPD_net_game_info* gJoinable_games;
tRM_info RMI;
int gMatts_PC;
_IPX_HEADER gLast_received_IPX_header;
tU16 gSocket_number_network_order;
tU16 gSocket_number_pd_format;
USHORT gECB_offset;
tU16 gListen_selector;
tU16 gSend_selector;

// Offset: 0
// Size: 201
void ClearupPDNetworkStuff() {
    NOT_IMPLEMENTED();
}

// Offset: 204
// Size: 39
// EAX: pFunction_name
// EDX: pMessage
// EBX: pAlleged_size
void MATTMessageCheck(char* pFunction_name, tNet_message* pMessage, int pAlleged_size) {
    NOT_IMPLEMENTED();
}

// Offset: 244
// Size: 484
// EAX: pDest
// EDX: pDest_len
// EBX: pFname
// ECX: pKeyname
int GetProfileText(char* pDest, int pDest_len, char* pFname, char* pKeyname) {
    FILE* fp;
    char in_buf[256];
    int i;
    int j;
    int len;
    NOT_IMPLEMENTED();
}

// Offset: 728
// Size: 187
int GetSocketNumberFromProfileFile() {
    int sscanf_res;
    tU32 socknum;
    NOT_IMPLEMENTED();
}

// Offset: 916
// Size: 104
// EAX: pAddr_ipx
tU32 EthernetAddressToU32(_IPX_LOCAL_TARGET* pAddr_ipx) {
    NOT_IMPLEMENTED();
}

// Offset: 1020
// Size: 175
// EAX: pString
// EDX: pSock_addr_ipx
void NetNowIPXLocalTarget2String(char* pString, _IPX_LOCAL_TARGET* pSock_addr_ipx) {
    NOT_IMPLEMENTED();
}

// Offset: 1196
// Size: 157
// EAX: pMessage_str
int GetMessageTypeFromMessage(char* pMessage_str) {
    char* real_msg;
    int msg_type_int;
    NOT_IMPLEMENTED();
}

// Offset: 1356
// Size: 95
// EAX: pAddr_ipx1
// EDX: pAddr_ipx2
int SameEthernetAddress(_IPX_LOCAL_TARGET* pAddr_ipx1, _IPX_LOCAL_TARGET* pAddr_ipx2) {
    NOT_IMPLEMENTED();
}

// Offset: 1452
// Size: 194
// EAX: pPlayer_id
_IPX_LOCAL_TARGET* GetIPXAddrFromPlayerID(tPlayer_ID pPlayer_id) {
    int i;
    tU8* nodenum;
    NOT_IMPLEMENTED();
}

// Offset: 1648
// Size: 67
// EAX: pMessage_type
void MakeMessageToSend(int pMessage_type) {
    NOT_IMPLEMENTED();
}

// Offset: 1716
// Size: 514
int ReceiveHostResponses() {
    int i;
    int already_registered;
    NOT_IMPLEMENTED();
}

// Offset: 2232
// Size: 229
int BroadcastMessage() {
    int errors;
    char broadcast_addr_string[32];
    char* real_msg;
    NOT_IMPLEMENTED();
}

// Offset: 2464
// Size: 174
// EAX: wSocket
BOOL hmiIPXCloseSocket(W32 wSocket) {
    REGS regs;
    SREGS sregs;
    NOT_IMPLEMENTED();
}

// Offset: 2640
// Size: 337
// EAX: pECB_ptr
// EDX: pOffset
void hmiIPXListenForPacket(_IPX_ECB* pECB_ptr, tU32 pOffset) {
    SREGS sregs;
    REGS regs;
    _IPX_HEADER* IPX_header_ptr;
    _IPX_ECB* sIPXECB;
    NOT_IMPLEMENTED();
}

// Offset: 2980
// Size: 59
// EAX: pECB_ptr
// EDX: pOffset
BOOL hmiIPXPostListen(_IPX_ECB* pECB_ptr, tU32 pOffset) {
    NOT_IMPLEMENTED();
}

// Offset: 3040
// Size: 278
BOOL hmiIPXGetData(PSTR pData, tU32 wDSize) {
    tU32 packets_checked;
    tU32 full_packet_ooer_missus;
    _IPX_ECB* ECB_ptr;
    static tU32 round_robin_jobby;
    NOT_IMPLEMENTED();
}

// Offset: 3320
// Size: 423
// EAX: sECB
// EDX: pPacket
// EBX: pHeader
// ECX: wSize
void hmiIPXSendPacket(_IPX_ECB* sECB, _IPX_ECB** pPacket, PSTR pHeader, W32 wSize) {
    SREGS sregs;
    REGS regs;
    _IPX_HEADER* sIPXHeader;
    _IPX_ECB* sIPXECB;
    PSTR pIPXData;
    NOT_IMPLEMENTED();
}

// Offset: 3744
// Size: 497
// EAX: pHeader
// EDX: wHSize
// EBX: pData
// ECX: wDSize
BOOL hmiIPXSendDataDirect(PSTR pHeader, W32 wHSize, PSTR pData, W32 wDSize, _NETNOW_NODE_ADDR* sNode) {
    W32 wIndex;
    _IPX_ELEMENT* sElement;
    W32* pSequence;
    NOT_IMPLEMENTED();
}

// Offset: 4244
// Size: 183
// EAX: sInterworkAddr
void hmiIPXGetInternetworkAddr(_IPX_INTERNET_ADDR* sInterworkAddr) {
    SREGS sregs;
    REGS regs;
    NOT_IMPLEMENTED();
}

// Offset: 4428
// Size: 211
// EAX: sNetworkAddr
void hmiIPXGetLocalTarget(_IPX_LOCAL_TARGET* sNetworkAddr) {
    SREGS sregs;
    REGS regs;
    NOT_IMPLEMENTED();
}

// Offset: 4640
// Size: 224
// EAX: wSize
// EDX: pPtr
// EBX: pSegment
// ECX: pSelector
BOOL AllocateRealMem(W32 wSize, PSTR* pPtr, W32* pSegment, tU16* pSelector) {
    REGS regs;
    SREGS sregs;
    W32 wAddress;
    W32 wHandle;
    W32 wSegment;
    W32 wLinear;
    NOT_IMPLEMENTED();
}

// Offset: 4864
// Size: 165
// EAX: pSelector
BOOL FreeRealMem(tU16 pSelector) {
    REGS regs;
    SREGS sregs;
    W32 wAddress;
    W32 wHandle;
    W32 wSegment;
    W32 wLinear;
    NOT_IMPLEMENTED();
}

// Offset: 5032
// Size: 210
BOOL hmiIPXInstalled() {
    REGS regs;
    NOT_IMPLEMENTED();
}

// Offset: 5244
// Size: 204
// EAX: wSocket
BOOL hmiIPXOpenSocket(W32 wSocket) {
    SREGS sregs;
    REGS regs;
    NOT_IMPLEMENTED();
}

// Offset: 5448
// Size: 203
void GetLargestPacketSizeOoErBetterInsertLinfordChristieJokeHere() {
    REGS regs;
    NOT_IMPLEMENTED();
}

// Offset: 5652
// Size: 510
// EAX: wSocket
BOOL hmiIPXInitSystem(W32 wSocket) {
    W32 wNIndex;
    W32 wIndex;
    W32 wMIndex;
    USHORT wSOffset;
    USHORT wLOffset;
    NOT_IMPLEMENTED();
}

// Offset: 6164
// Size: 86
void GetIPXToStickItsEarToTheGround() {
    NOT_IMPLEMENTED();
}

// Offset: 6252
// Size: 342
int PDNetInitialise() {
    char profile_string[32];
    char key_name[32];
    int sscanf_res;
    int i;
    tU32 netnum;
    char str[256];
    int mess_num;
    NOT_IMPLEMENTED();
}

// Offset: 6596
// Size: 63
int PDNetShutdown() {
    NOT_IMPLEMENTED();
}

// Offset: 6660
// Size: 101
void PDNetStartProducingJoinList() {
    NOT_IMPLEMENTED();
}

// Offset: 6764
// Size: 81
void PDNetEndJoinList() {
    NOT_IMPLEMENTED();
}

// Offset: 6848
// Size: 369
// EAX: pGame
// EDX: pIndex
int PDNetGetNextJoinGame(tNet_game_details* pGame, int pIndex) {
    static tU32 next_broadcast_time;
    int i;
    int j;
    int number_of_hosts_has_changed;
    char str[256];
    NOT_IMPLEMENTED();
}

// Offset: 7220
// Size: 51
// EAX: pDetails
void PDNetDisposeGameDetails(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 7272
// Size: 72
// EAX: pDetails
// EDX: pHost_name
// EBX: pHost_address
int PDNetHostGame(tNet_game_details* pDetails, char* pHost_name, void** pHost_address) {
    NOT_IMPLEMENTED();
}

// Offset: 7344
// Size: 62
// EAX: pDetails
// EDX: pPlayer_name
int PDNetJoinGame(tNet_game_details* pDetails, char* pPlayer_name) {
    NOT_IMPLEMENTED();
}

// Offset: 7408
// Size: 51
// EAX: pDetails
void PDNetLeaveGame(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 7460
// Size: 51
// EAX: pDetails
void PDNetHostFinishGame(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 7512
// Size: 74
// EAX: pDetails
tU32 PDNetExtractGameID(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 7588
// Size: 78
// EAX: pDetails
tPlayer_ID PDNetExtractPlayerID(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 7668
// Size: 65
// EAX: pName
// EDX: pMax_length
void PDNetObtainSystemUserName(char* pName, int pMax_length) {
    dr_dprintf("PDNetObtainSystemUserName()\n");
    strcpy(pName, "Ron Turn");
}

// Offset: 7736
// Size: 223
// EAX: pDetails
// EDX: pMessage
// EBX: pPlayer
int PDNetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer) {
    char str[256];
    _IPX_LOCAL_TARGET* remote_addr_ipx;
    NOT_IMPLEMENTED();
}

// Offset: 7960
// Size: 274
// EAX: pDetails
// EDX: pMessage
int PDNetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage) {
    char str[256];
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 8236
// Size: 420
// EAX: pDetails
// EDX: pSender_address
tNet_message* PDNetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    char* receive_buffer;
    char str[256];
    int msg_type;
    NOT_IMPLEMENTED();
}

// Offset: 8656
// Size: 76
// EAX: pSize
// EDX: pSize_decider
tNet_message* PDNetAllocateMessage(tU32 pSize, tS32 pSize_decider) {
    NOT_IMPLEMENTED();
}

// Offset: 8732
// Size: 63
// EAX: pDetails
// EDX: pMessage
void PDNetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

// Offset: 8796
// Size: 62
// EAX: pPlayer
// EDX: pSender_address
void PDNetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address) {
    NOT_IMPLEMENTED();
}

// Offset: 8860
// Size: 51
// EAX: pPlayer
void PDNetDisposePlayer(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

// Offset: 8912
// Size: 202
// EAX: pDetails
// EDX: pMessage
// EBX: pAddress
int PDNetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {
    char str[256];
    NOT_IMPLEMENTED();
}

// Offset: 9116
// Size: 61
// EAX: pDetails
int PDNetInitClient(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

// Offset: 9180
// Size: 44
int PDNetGetHeaderSize() {
    NOT_IMPLEMENTED();
}
