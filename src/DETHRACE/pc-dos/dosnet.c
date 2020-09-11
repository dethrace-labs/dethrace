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

void ClearupPDNetworkStuff() {
    NOT_IMPLEMENTED();
}

void MATTMessageCheck(char* pFunction_name, tNet_message* pMessage, int pAlleged_size) {
    NOT_IMPLEMENTED();
}

int GetProfileText(char* pDest, int pDest_len, char* pFname, char* pKeyname) {
    FILE* fp;
    char in_buf[256];
    int i;
    int j;
    int len;
    NOT_IMPLEMENTED();
}

int GetSocketNumberFromProfileFile() {
    int sscanf_res;
    tU32 socknum;
    NOT_IMPLEMENTED();
}

tU32 EthernetAddressToU32(_IPX_LOCAL_TARGET* pAddr_ipx) {
    NOT_IMPLEMENTED();
}

void NetNowIPXLocalTarget2String(char* pString, _IPX_LOCAL_TARGET* pSock_addr_ipx) {
    NOT_IMPLEMENTED();
}

int GetMessageTypeFromMessage(char* pMessage_str) {
    char* real_msg;
    int msg_type_int;
    NOT_IMPLEMENTED();
}

int SameEthernetAddress(_IPX_LOCAL_TARGET* pAddr_ipx1, _IPX_LOCAL_TARGET* pAddr_ipx2) {
    NOT_IMPLEMENTED();
}

_IPX_LOCAL_TARGET* GetIPXAddrFromPlayerID(tPlayer_ID pPlayer_id) {
    int i;
    tU8* nodenum;
    NOT_IMPLEMENTED();
}

void MakeMessageToSend(int pMessage_type) {
    NOT_IMPLEMENTED();
}

int ReceiveHostResponses() {
    int i;
    int already_registered;
    NOT_IMPLEMENTED();
}

int BroadcastMessage() {
    int errors;
    char broadcast_addr_string[32];
    char* real_msg;
    NOT_IMPLEMENTED();
}

BOOL hmiIPXCloseSocket(W32 wSocket) {
    REGS regs;
    SREGS sregs;
    NOT_IMPLEMENTED();
}

void hmiIPXListenForPacket(_IPX_ECB* pECB_ptr, tU32 pOffset) {
    SREGS sregs;
    REGS regs;
    _IPX_HEADER* IPX_header_ptr;
    _IPX_ECB* sIPXECB;
    NOT_IMPLEMENTED();
}

BOOL hmiIPXPostListen(_IPX_ECB* pECB_ptr, tU32 pOffset) {
    NOT_IMPLEMENTED();
}

BOOL hmiIPXGetData(PSTR pData, tU32 wDSize) {
    tU32 packets_checked;
    tU32 full_packet_ooer_missus;
    _IPX_ECB* ECB_ptr;
    static tU32 round_robin_jobby;
    NOT_IMPLEMENTED();
}

void hmiIPXSendPacket(_IPX_ECB* sECB, _IPX_ECB** pPacket, PSTR pHeader, W32 wSize) {
    SREGS sregs;
    REGS regs;
    _IPX_HEADER* sIPXHeader;
    _IPX_ECB* sIPXECB;
    PSTR pIPXData;
    NOT_IMPLEMENTED();
}

BOOL hmiIPXSendDataDirect(PSTR pHeader, W32 wHSize, PSTR pData, W32 wDSize, _NETNOW_NODE_ADDR* sNode) {
    W32 wIndex;
    _IPX_ELEMENT* sElement;
    W32* pSequence;
    NOT_IMPLEMENTED();
}

void hmiIPXGetInternetworkAddr(_IPX_INTERNET_ADDR* sInterworkAddr) {
    SREGS sregs;
    REGS regs;
    NOT_IMPLEMENTED();
}

void hmiIPXGetLocalTarget(_IPX_LOCAL_TARGET* sNetworkAddr) {
    SREGS sregs;
    REGS regs;
    NOT_IMPLEMENTED();
}

BOOL AllocateRealMem(W32 wSize, PSTR* pPtr, W32* pSegment, tU16* pSelector) {
    REGS regs;
    SREGS sregs;
    W32 wAddress;
    W32 wHandle;
    W32 wSegment;
    W32 wLinear;
    NOT_IMPLEMENTED();
}

BOOL FreeRealMem(tU16 pSelector) {
    REGS regs;
    SREGS sregs;
    W32 wAddress;
    W32 wHandle;
    W32 wSegment;
    W32 wLinear;
    NOT_IMPLEMENTED();
}

BOOL hmiIPXInstalled() {
    REGS regs;
    NOT_IMPLEMENTED();
}

BOOL hmiIPXOpenSocket(W32 wSocket) {
    SREGS sregs;
    REGS regs;
    NOT_IMPLEMENTED();
}

void GetLargestPacketSizeOoErBetterInsertLinfordChristieJokeHere() {
    REGS regs;
    NOT_IMPLEMENTED();
}

BOOL hmiIPXInitSystem(W32 wSocket) {
    W32 wNIndex;
    W32 wIndex;
    W32 wMIndex;
    USHORT wSOffset;
    USHORT wLOffset;
    NOT_IMPLEMENTED();
}

void GetIPXToStickItsEarToTheGround() {
    NOT_IMPLEMENTED();
}

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

int PDNetShutdown() {
    NOT_IMPLEMENTED();
}

void PDNetStartProducingJoinList() {
    NOT_IMPLEMENTED();
}

void PDNetEndJoinList() {
    NOT_IMPLEMENTED();
}

int PDNetGetNextJoinGame(tNet_game_details* pGame, int pIndex) {
    static tU32 next_broadcast_time;
    int i;
    int j;
    int number_of_hosts_has_changed;
    char str[256];
    NOT_IMPLEMENTED();
}

void PDNetDisposeGameDetails(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

int PDNetHostGame(tNet_game_details* pDetails, char* pHost_name, void** pHost_address) {
    NOT_IMPLEMENTED();
}

int PDNetJoinGame(tNet_game_details* pDetails, char* pPlayer_name) {
    NOT_IMPLEMENTED();
}

void PDNetLeaveGame(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

void PDNetHostFinishGame(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

tU32 PDNetExtractGameID(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

tPlayer_ID PDNetExtractPlayerID(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

void PDNetObtainSystemUserName(char* pName, int pMax_length) {
    dr_dprintf("PDNetObtainSystemUserName()\n");
    strcpy(pName, "Ron Turn");
}

int PDNetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer) {
    char str[256];
    _IPX_LOCAL_TARGET* remote_addr_ipx;
    NOT_IMPLEMENTED();
}

int PDNetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage) {
    char str[256];
    int i;
    NOT_IMPLEMENTED();
}

tNet_message* PDNetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    char* receive_buffer;
    char str[256];
    int msg_type;
    NOT_IMPLEMENTED();
}

tNet_message* PDNetAllocateMessage(tU32 pSize, tS32 pSize_decider) {
    NOT_IMPLEMENTED();
}

void PDNetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {
    NOT_IMPLEMENTED();
}

void PDNetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address) {
    NOT_IMPLEMENTED();
}

void PDNetDisposePlayer(tNet_game_player_info* pPlayer) {
    NOT_IMPLEMENTED();
}

int PDNetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {
    char str[256];
    NOT_IMPLEMENTED();
}

int PDNetInitClient(tNet_game_details* pDetails) {
    NOT_IMPLEMENTED();
}

int PDNetGetHeaderSize() {
    NOT_IMPLEMENTED();
}
