#include "pd/net.h"

#include "errors.h"
#include "harness/trace.h"
#include <stdlib.h>
#include <string.h>

tU32 gNetwork_init_flags;
tPD_net_game_info* gJoinable_games;
int gMatts_PC;
tU16 gSocket_number_pd_format;
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
tRM_info RMI;
_IPX_HEADER gLast_received_IPX_header;
tU16 gSocket_number_network_order;
USHORT gECB_offset;
tU16 gListen_selector;
tU16 gSend_selector;

// IDA: void __cdecl ClearupPDNetworkStuff()
void ClearupPDNetworkStuff() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MATTMessageCheck(char *pFunction_name@<EAX>, tNet_message *pMessage@<EDX>, int pAlleged_size@<EBX>)
void MATTMessageCheck(char* pFunction_name, tNet_message* pMessage, int pAlleged_size) {
    LOG_TRACE("(\"%s\", %p, %d)", pFunction_name, pMessage, pAlleged_size);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetProfileText@<EAX>(char *pDest@<EAX>, int pDest_len@<EDX>, char *pFname@<EBX>, char *pKeyname@<ECX>)
int GetProfileText(char* pDest, int pDest_len, char* pFname, char* pKeyname) {
    FILE* fp;
    char in_buf[256];
    int i;
    int j;
    int len;
    LOG_TRACE("(\"%s\", %d, \"%s\", \"%s\")", pDest, pDest_len, pFname, pKeyname);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl GetSocketNumberFromProfileFile()
int GetSocketNumberFromProfileFile() {
    char str[256];
    int sscanf_res;
    tU32 socknum;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall EthernetAddressToU32@<EAX>(_IPX_LOCAL_TARGET *pAddr_ipx@<EAX>)
tU32 EthernetAddressToU32(_IPX_LOCAL_TARGET* pAddr_ipx) {
    LOG_TRACE("(%p)", pAddr_ipx);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetNowIPXLocalTarget2String(char *pString@<EAX>, _IPX_LOCAL_TARGET *pSock_addr_ipx@<EDX>)
void NetNowIPXLocalTarget2String(char* pString, _IPX_LOCAL_TARGET* pSock_addr_ipx) {
    LOG_TRACE("(\"%s\", %p)", pString, pSock_addr_ipx);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall GetMessageTypeFromMessage@<EAX>(char *pMessage_str@<EAX>)
int GetMessageTypeFromMessage(char* pMessage_str) {
    char* real_msg;
    int msg_type_int;
    LOG_TRACE("(\"%s\")", pMessage_str);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SameEthernetAddress@<EAX>(_IPX_LOCAL_TARGET *pAddr_ipx1@<EAX>, _IPX_LOCAL_TARGET *pAddr_ipx2@<EDX>)
int SameEthernetAddress(_IPX_LOCAL_TARGET* pAddr_ipx1, _IPX_LOCAL_TARGET* pAddr_ipx2) {
    LOG_TRACE("(%p, %p)", pAddr_ipx1, pAddr_ipx2);
    NOT_IMPLEMENTED();
}

_IPX_LOCAL_TARGET* GetIPXAddrFromPlayerID(tPlayer_ID pPlayer_id) {
    int i;
    tU8* nodenum;
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MakeMessageToSend(int pMessage_type@<EAX>)
void MakeMessageToSend(int pMessage_type) {
    LOG_TRACE("(%d)", pMessage_type);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl ReceiveHostResponses()
int ReceiveHostResponses() {
    char str[256];
    int i;
    int already_registered;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl BroadcastMessage()
int BroadcastMessage() {
    int i;
    int errors;
    char broadcast_addr_string[32];
    char* real_msg;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: BOOL __usercall hmiIPXCloseSocket@<EAX>(W32 wSocket@<EAX>)
BOOL hmiIPXCloseSocket(W32 wSocket) {
    REGS regs;
    SREGS sregs;
    LOG_TRACE("(%d)", wSocket);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall hmiIPXListenForPacket(_IPX_ECB *pECB_ptr@<EAX>, tU32 pOffset@<EDX>)
void hmiIPXListenForPacket(_IPX_ECB* pECB_ptr, tU32 pOffset) {
    SREGS sregs;
    REGS regs;
    _IPX_HEADER* IPX_header_ptr;
    _IPX_ECB* sIPXECB;
    LOG_TRACE("(%p, %d)", pECB_ptr, pOffset);
    NOT_IMPLEMENTED();
}

// IDA: BOOL __usercall hmiIPXPostListen@<EAX>(_IPX_ECB *pECB_ptr@<EAX>, tU32 pOffset@<EDX>)
BOOL hmiIPXPostListen(_IPX_ECB* pECB_ptr, tU32 pOffset) {
    LOG_TRACE("(%p, %d)", pECB_ptr, pOffset);
    NOT_IMPLEMENTED();
}

// IDA: BOOL __cdecl hmiIPXGetData(PSTR pData, tU32 wDSize)
BOOL hmiIPXGetData(PSTR pData, tU32 wDSize) {
    tU32 packets_checked;
    tU32 full_packet_ooer_missus;
    _IPX_ECB* ECB_ptr;
    static tU32 round_robin_jobby;
    LOG_TRACE("(%d, %d)", pData, wDSize);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall hmiIPXSendPacket(_IPX_ECB *sECB@<EAX>, _IPX_ECB **pPacket@<EDX>, PSTR pHeader@<EBX>, W32 wSize@<ECX>)
void hmiIPXSendPacket(_IPX_ECB* sECB, _IPX_ECB** pPacket, PSTR pHeader, W32 wSize) {
    SREGS sregs;
    REGS regs;
    _IPX_HEADER* sIPXHeader;
    _IPX_ECB* sIPXECB;
    PSTR pIPXData;
    LOG_TRACE("(%p, %p, %d, %d)", sECB, pPacket, pHeader, wSize);
    NOT_IMPLEMENTED();
}

// IDA: BOOL __usercall hmiIPXSendDataDirect@<EAX>(PSTR pHeader@<EAX>, W32 wHSize@<EDX>, PSTR pData@<EBX>, W32 wDSize@<ECX>, _NETNOW_NODE_ADDR *sNode)
BOOL hmiIPXSendDataDirect(PSTR pHeader, W32 wHSize, PSTR pData, W32 wDSize, _NETNOW_NODE_ADDR* sNode) {
    W32 wIndex;
    _IPX_ELEMENT* sElement;
    W32* pSequence;
    LOG_TRACE("(%d, %d, %d, %d, %p)", pHeader, wHSize, pData, wDSize, sNode);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall hmiIPXGetInternetworkAddr(_IPX_INTERNET_ADDR *sInterworkAddr@<EAX>)
void hmiIPXGetInternetworkAddr(_IPX_INTERNET_ADDR* sInterworkAddr) {
    SREGS sregs;
    REGS regs;
    LOG_TRACE("(%p)", sInterworkAddr);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall hmiIPXGetLocalTarget(_IPX_LOCAL_TARGET *sNetworkAddr@<EAX>)
void hmiIPXGetLocalTarget(_IPX_LOCAL_TARGET* sNetworkAddr) {
    SREGS sregs;
    REGS regs;
    LOG_TRACE("(%p)", sNetworkAddr);
    NOT_IMPLEMENTED();
}

// IDA: BOOL __usercall AllocateRealMem@<EAX>(W32 wSize@<EAX>, PSTR *pPtr@<EDX>, W32 *pSegment@<EBX>, tU16 *pSelector@<ECX>)
BOOL AllocateRealMem(W32 wSize, PSTR* pPtr, W32* pSegment, tU16* pSelector) {
    REGS regs;
    SREGS sregs;
    W32 wAddress;
    W32 wHandle;
    W32 wSegment;
    W32 wLinear;
    LOG_TRACE("(%d, %p, %p, %p)", wSize, pPtr, pSegment, pSelector);
    NOT_IMPLEMENTED();
}

// IDA: BOOL __usercall FreeRealMem@<EAX>(tU16 pSelector@<EAX>)
BOOL FreeRealMem(tU16 pSelector) {
    REGS regs;
    SREGS sregs;
    W32 wAddress;
    W32 wHandle;
    W32 wSegment;
    W32 wLinear;
    LOG_TRACE("(%d)", pSelector);
    NOT_IMPLEMENTED();
}

// IDA: BOOL __cdecl hmiIPXInstalled()
BOOL hmiIPXInstalled() {
    SREGS sregs;
    REGS regs;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: BOOL __usercall hmiIPXOpenSocket@<EAX>(W32 wSocket@<EAX>)
BOOL hmiIPXOpenSocket(W32 wSocket) {
    SREGS sregs;
    REGS regs;
    LOG_TRACE("(%d)", wSocket);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GetLargestPacketSizeOoErBetterInsertLinfordChristieJokeHere()
void GetLargestPacketSizeOoErBetterInsertLinfordChristieJokeHere() {
    SREGS sregs;
    REGS regs;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: BOOL __usercall hmiIPXInitSystem@<EAX>(W32 wSocket@<EAX>)
BOOL hmiIPXInitSystem(W32 wSocket) {
    W32 wNIndex;
    W32 wIndex;
    W32 wMIndex;
    USHORT wSOffset;
    USHORT wLOffset;
    LOG_TRACE("(%d)", wSocket);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GetIPXToStickItsEarToTheGround()
void GetIPXToStickItsEarToTheGround() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDNetInitialise()
int PDNetInitialise() {
    tU32 timenow;
    char profile_string[32];
    char key_name[32];
    int sscanf_res;
    int i;
    tU32 netnum;
    char str[256];
    int mess_num;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDNetShutdown()
int PDNetShutdown() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDNetStartProducingJoinList()
void PDNetStartProducingJoinList() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDNetEndJoinList()
void PDNetEndJoinList() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDNetGetNextJoinGame@<EAX>(tNet_game_details *pGame@<EAX>, int pIndex@<EDX>)
int PDNetGetNextJoinGame(tNet_game_details* pGame, int pIndex) {
    static tU32 next_broadcast_time;
    int i;
    int j;
    int number_of_hosts_has_changed;
    char str[256];
    LOG_TRACE("(%p, %d)", pGame, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDNetDisposeGameDetails(tNet_game_details *pDetails@<EAX>)
void PDNetDisposeGameDetails(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDNetHostGame@<EAX>(tNet_game_details *pDetails@<EAX>, char *pHost_name@<EDX>, void **pHost_address@<EBX>)
int PDNetHostGame(tNet_game_details* pDetails, char* pHost_name, void** pHost_address) {
    LOG_TRACE("(%p, \"%s\", %p)", pDetails, pHost_name, pHost_address);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDNetJoinGame@<EAX>(tNet_game_details *pDetails@<EAX>, char *pPlayer_name@<EDX>)
int PDNetJoinGame(tNet_game_details* pDetails, char* pPlayer_name) {
    LOG_TRACE("(%p, \"%s\")", pDetails, pPlayer_name);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDNetLeaveGame(tNet_game_details *pDetails@<EAX>)
void PDNetLeaveGame(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDNetHostFinishGame(tNet_game_details *pDetails@<EAX>)
void PDNetHostFinishGame(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall PDNetExtractGameID@<EAX>(tNet_game_details *pDetails@<EAX>)
tU32 PDNetExtractGameID(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: tPlayer_ID __usercall PDNetExtractPlayerID@<EAX>(tNet_game_details *pDetails@<EAX>)
tPlayer_ID PDNetExtractPlayerID(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDNetObtainSystemUserName(char *pName@<EAX>, int pMax_length@<EDX>)
void PDNetObtainSystemUserName(char* pName, int pMax_length) {
#ifdef _WIN32
    int size;
    char buffer[16];
    BOOL result;
#endif

    dr_dprintf("PDNetObtainSystemUserName()\n");
#ifdef _WIN32
    size = COUNT_OF(buffer);
    result = GetComputerNameA(buffer, &size);
    if (result && size != 0) {
        strncpy(pName, buffer, pMax_length - 1);
        pName[pMax_length - 1] = '\0';
    }
    while (1) {
        pName = strpbrk(pName, "_=(){}[]<>!$%^&*/:@~;'#,?\\|`\"");
        if (pName == NULL || *pName == '\0') {
            break;
        }
        *pName = '-';
    }
#else
    strcpy(pName, "Ron Turn");
#endif
}

// IDA: int __usercall PDNetSendMessageToPlayer@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, tPlayer_ID pPlayer@<EBX>)
int PDNetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer) {
    char str[256];
    _IPX_LOCAL_TARGET* remote_addr_ipx;
    LOG_TRACE("(%p, %p, %d)", pDetails, pMessage, pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDNetSendMessageToAllPlayers@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
int PDNetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage) {
    char str[256];
    int i;
    LOG_TRACE("(%p, %p)", pDetails, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: tNet_message* __usercall PDNetGetNextMessage@<EAX>(tNet_game_details *pDetails@<EAX>, void **pSender_address@<EDX>)
tNet_message* PDNetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    char* receive_buffer;
    char str[256];
    int msg_type;
    LOG_TRACE("(%p, %p)", pDetails, pSender_address);
    NOT_IMPLEMENTED();
}

// IDA: tNet_message* __usercall PDNetAllocateMessage@<EAX>(tU32 pSize@<EAX>, tS32 pSize_decider@<EDX>)
tNet_message* PDNetAllocateMessage(tU32 pSize, tS32 pSize_decider) {
    LOG_TRACE("(%d, %d)", pSize, pSize_decider);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDNetDisposeMessage(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
void PDNetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {
    LOG_TRACE("(%p, %p)", pDetails, pMessage);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDNetSetPlayerSystemInfo(tNet_game_player_info *pPlayer@<EAX>, void *pSender_address@<EDX>)
void PDNetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address) {
    LOG_TRACE("(%p, %p)", pPlayer, pSender_address);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDNetDisposePlayer(tNet_game_player_info *pPlayer@<EAX>)
void PDNetDisposePlayer(tNet_game_player_info* pPlayer) {
    LOG_TRACE("(%p)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDNetSendMessageToAddress@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, void *pAddress@<EBX>)
int PDNetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {
    char str[256];
    LOG_TRACE("(%p, %p, %p)", pDetails, pMessage, pAddress);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDNetInitClient@<EAX>(tNet_game_details *pDetails@<EAX>)
int PDNetInitClient(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDNetGetHeaderSize()
int PDNetGetHeaderSize() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
