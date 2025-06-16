#include "pd/net.h"

#include "brender.h"
#include "dr_types.h"
#include "errors.h"
#include "globvrpb.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/os.h"
#include "harness/trace.h"
#include "net_types.h"
#include "network.h"
#include "pd/net.h"
#include "pd/sys.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>

#define EWOULDBLOCK WSAEWOULDBLOCK

#else /* Assume posix style sockets on non-windows */

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#endif

// dethrace: have switched out IPX implementation for IP

tPD_net_game_info* gJoinable_games;
int gMatts_PC;
tU16 gSocket_number_pd_format;
char gLocal_addr_string[32];
char gReceive_buffer[512];
tPD_net_player_info gRemote_net_player_info;
tPD_net_player_info gLocal_net_player_info;
char gSend_buffer[512];
tU8* gSend_packet;
tU8* gSend_packet_ptr;
tU8* gListen_packet;
tU8* gListen_packet_ptr;
size_t gMsg_header_strlen;
int gNumber_of_networks;
int gNumber_of_hosts;

struct sockaddr_in gAny_addr;
struct sockaddr_in gLocal_addr;
struct sockaddr_in gRemote_addr;
struct sockaddr_in gBroadcast_addr;
struct sockaddr_in gLast_received_addr;

tCopyable_sockaddr_in gLocal_addr_copyable;
tCopyable_sockaddr_in gLast_received_addr_copyable;

int gSocket;
int gPlayer_id;

#define MESSAGE_HEADER_STR "CW95MSG"
#define JOINABLE_GAMES_CAPACITY 16
#define PORT 12286

DR_STATIC_ASSERT(offsetof(tNet_message, pd_stuff_so_DO_NOT_USE) == 0);
DR_STATIC_ASSERT(offsetof(tNet_message, magic_number) == 4);
DR_STATIC_ASSERT(offsetof(tNet_message, guarantee_number) == 8);
DR_STATIC_ASSERT(offsetof(tNet_message, sender) == 12);
DR_STATIC_ASSERT(offsetof(tNet_message, version) == 16);
DR_STATIC_ASSERT(offsetof(tNet_message, senders_time_stamp) == 20);
DR_STATIC_ASSERT(offsetof(tNet_message, num_contents) == 24);
DR_STATIC_ASSERT(offsetof(tNet_message, overall_size) == 26);
DR_STATIC_ASSERT(offsetof(tNet_message, contents) == 28);

// dethrace added
void PDNetCopyFromNative(tCopyable_sockaddr_in* pAddress, struct sockaddr_in* sock);
void PDNetCopyToNative(struct sockaddr_in* sock, tCopyable_sockaddr_in* pAddress);

// IDA: void __cdecl ClearupPDNetworkStuff()
void ClearupPDNetworkStuff(void) {
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
int GetSocketNumberFromProfileFile(void) {
    char str[256];
    int sscanf_res;
    tU32 socknum;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// in dossys, this is called `NetNowIPXLocalTarget2String`
void SockAddrToString(char* pString, struct sockaddr_in* pSock_addr) {
    LOG_TRACE("(\"%s\", %p)", pString, pSock_addr);

    char portbuf[10];

    inet_ntop(AF_INET, &pSock_addr->sin_addr, pString, 32);
    sprintf(portbuf, ":%d", ntohs(pSock_addr->sin_port));
    strcat(pString, portbuf);
}

// added by dethrace
void CopyableSockAddrToString(char* pString, tCopyable_sockaddr_in* pSock_addr) {
    LOG_TRACE("(\"%s\", %p)", pString, pSock_addr);

    char portbuf[10];

    inet_ntop(AF_INET, &pSock_addr->address, pString, 32);
    sprintf(portbuf, ":%d", ntohs(pSock_addr->port));
    strcat(pString, portbuf);
}

// IDA: int __usercall GetMessageTypeFromMessage@<EAX>(char *pMessage_str@<EAX>)
int GetMessageTypeFromMessage(char* pMessage_str) {
    char* real_msg;
    int msg_type_int;
    LOG_TRACE("(\"%s\")", pMessage_str);

    real_msg = &pMessage_str[4];
    msg_type_int = 0;

    // FIXME: "CW95MSG" value is used in and depends on platform
    if (strncmp(real_msg, MESSAGE_HEADER_STR, gMsg_header_strlen) == 0) {
        if (isdigit(real_msg[gMsg_header_strlen])) {
            msg_type_int = real_msg[gMsg_header_strlen] - '0';
        }
        if (msg_type_int != 0 && msg_type_int < 3) {
            return msg_type_int;
        }
    }
    return 999;
}

int SameEthernetAddress(struct sockaddr_in* pAddr_1, struct sockaddr_in* pAddr_2) {
    LOG_TRACE("(%p, %p)", pAddr_1, pAddr_2);

    return memcmp(pAddr_1, pAddr_2, sizeof(struct sockaddr_in)) == 0;
}

// added by dethrace
int SameEthernetAddress2(tCopyable_sockaddr_in* pAddr_1, struct sockaddr_in* pAddr_2) {
    LOG_TRACE("(%p, %p)", pAddr_1, pAddr_2);

    return pAddr_1->port == pAddr_2->sin_port && pAddr_1->address == pAddr_2->sin_addr.s_addr;
}

/*SOCKADDR_IPX_* */ void GetIPXAddrFromPlayerID(tPlayer_ID pPlayer_id) {
    int i;
    tU8* nodenum;
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MakeMessageToSend(int pMessage_type@<EAX>)
void MakeMessageToSend(int pMessage_type) {
    LOG_TRACE("(%d)", pMessage_type);

#ifdef DETHRACE_FIX_BUGS
    sprintf(gSend_buffer, "XXXX%s%1d", MESSAGE_HEADER_STR, pMessage_type);
#else
    sprintf(gSend_buffer, "XXXX%s%0.1d", MESSAGE_HEADER_STR, pMessage_type);
#endif
}

// IDA: int __cdecl ReceiveHostResponses()
int ReceiveHostResponses(void) {
    char str[256];
    int i;
    int already_registered;
    LOG_TRACE("()");

    char addr_string[32];
    unsigned int sa_len;
    int error;

    sa_len = sizeof(gRemote_addr);
    while (1) {
        if (recvfrom(gSocket, gReceive_buffer, sizeof(gReceive_buffer), 0, (struct sockaddr*)&gRemote_addr, &sa_len) == -1) {
            break;
        }
        SockAddrToString(addr_string, &gRemote_addr);
        dr_dprintf("ReceiveHostResponses(): Received string '%s' from %s", gReceive_buffer, addr_string);

        if (SameEthernetAddress(&gLocal_addr, &gRemote_addr)) {
            dr_dprintf("*** Discounting the above 'cos we sent it ***");
            continue;
        }
        if (GetMessageTypeFromMessage(gReceive_buffer) != 2) {
            dr_dprintf("*** Discounting the above 'cos it's not a host reply ***");
            continue;
        }

        dr_dprintf("*** It's a host reply! ***");
        already_registered = 0;
        for (i = 0; i < gNumber_of_hosts; i++) {
            if (SameEthernetAddress2(&gJoinable_games[i].addr_in, &gRemote_addr)) {
                already_registered = 1;
                break;
            }
        }
        if (already_registered) {
            dr_dprintf("That game already registered");
            gJoinable_games[i].last_response = PDGetTotalTime();
        } else {
            dr_dprintf("Adding joinable game to slot #%d", gNumber_of_hosts);
            PDNetCopyFromNative(&gJoinable_games[gNumber_of_hosts].addr_in, &gRemote_addr);
            gJoinable_games[gNumber_of_hosts].last_response = PDGetTotalTime();
            gNumber_of_hosts++;
            dr_dprintf("Number of games found so far: %d", gNumber_of_hosts);
        }
        if (gNumber_of_hosts) {
            dr_dprintf("Currently registered net games:");
            for (i = 0; i < gNumber_of_hosts; i++) {
                CopyableSockAddrToString(str, &gJoinable_games[i].addr_in);
                dr_dprintf("%d: Host addr %s", i, str);
            }
        }
    }
    error = OS_GetLastSocketError() != EWOULDBLOCK;
    if (error == 0) {
        return 1;
    }
    dr_dprintf("ReceiveHostResponses(): Error on recvfrom() - WSAGetLastError=%d", error);
    return 0;
}

// IDA: int __cdecl BroadcastMessage()
int BroadcastMessage(void) {
    int i;
    int errors;
    char broadcast_addr_string[32];
    char* real_msg;
    LOG_TRACE("()");

    errors = 0;
    for (i = 0; i < gNumber_of_networks; i++) {
        SockAddrToString(broadcast_addr_string, &gBroadcast_addr);
        dr_dprintf("Broadcasting on address '%s'", broadcast_addr_string);
        if (sendto(gSocket, gSend_buffer, strlen(gSend_buffer) + 1, 0, (struct sockaddr*)&gBroadcast_addr, sizeof(gBroadcast_addr)) == -1) {
            dr_dprintf("BroadcastMessage(): Error on sendto() - WSAGetLastError=%d", OS_GetLastSocketError());
            errors = 1;
        }
    }
    return errors == 0;
}

// IDA: int __cdecl PDNetInitialise()
int PDNetInitialise(void) {
    tU32 timenow;
    char profile_string[32];
    char key_name[32];
    int sscanf_res;
    int i;
    tU32 netnum;
    char str[256];
    int mess_num;

    struct linger so_linger;
    unsigned int sa_len;

    LOG_TRACE("()");

    gPlayer_id = PDGetTotalTime();
    gNumber_of_networks = 1;
    sa_len = sizeof(struct sockaddr_in);
    dr_dprintf("PDNetInitialise()");
    gNumber_of_networks = 1;

    memset(&gAny_addr, 0, sizeof(gAny_addr));
    memset(&gLocal_addr, 0, sizeof(gLocal_addr));
    memset(&gRemote_addr, 0, sizeof(gRemote_addr));
    memset(&gBroadcast_addr, 0, sizeof(gBroadcast_addr));

    gAny_addr.sin_family = AF_INET;
    gAny_addr.sin_addr.s_addr = INADDR_ANY;
    gAny_addr.sin_port = htons(PORT);

    int found = OS_GetAdapterAddress(harness_game_config.network_adapter_name, &gLocal_addr);
    if (!found) {
        gLocal_addr.sin_addr.s_addr = INADDR_LOOPBACK;
    }

    gLocal_addr.sin_port = htons(PORT);

    // advertise that we are connectable on this address
    PDNetCopyFromNative(&gLocal_addr_copyable, &gLocal_addr);

    SockAddrToString(str, &gLocal_addr);
    LOG_INFO("Advertising on %s", str);

    // actually listen on any address
    gLocal_addr.sin_addr.s_addr = INADDR_ANY;

    SockAddrToString(str, &gLocal_addr);
    LOG_INFO("Listening on %s", str);

    gRemote_addr.sin_family = AF_INET;
    gRemote_addr.sin_port = htons(PORT);
    gBroadcast_addr.sin_family = AF_INET;
    gBroadcast_addr.sin_port = htons(PORT);
    gBroadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;

    if (OS_InitSockets() == -1) {
        dr_dprintf("PDNetInitialise(): WSAStartup() failed");
        return -1;
    }

    gSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (gSocket == -1) {
        dr_dprintf("PDNetInitialise(): Failed to create socket - WSAGetLastError=%d", OS_GetLastSocketError());
        OS_CleanupSockets();
        return -1;
    }
    int broadcast = 1;
    setsockopt(gSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    setsockopt(gSocket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

    unsigned long nobio = 1;
    if (OS_SetSocketNonBlocking(gSocket) == -1) {
        dr_dprintf("Error on ioctlsocket() - WSAGetLastError=%d", OS_GetLastSocketError());
        OS_CloseSocket(gSocket);
        OS_CleanupSockets();
        return -1;
    }

    if (harness_game_config.no_bind == 0) {
        if (bind(gSocket, (struct sockaddr*)&gAny_addr, sizeof(gAny_addr)) == -1) {
            dr_dprintf("Error on bind() - WSAGetLastError=%d", OS_GetLastSocketError());
            OS_CloseSocket(gSocket);
            OS_CleanupSockets();
            return -1;
        }
    }

    SockAddrToString(gLocal_addr_string, &gLocal_addr);

    dr_dprintf("Socket bound OK; local address is '%s'", gLocal_addr_string);
    gMsg_header_strlen = 7;
    if (strstr(gLocal_addr_string, "00a0240f9fac")) {
        gMatts_PC = 1;
    }
    return 0;
}

// IDA: int __cdecl PDNetShutdown()
int PDNetShutdown(void) {
    LOG_TRACE("()");

    dr_dprintf("PDNetShutdown()");
    if (gSocket != -1) {
        OS_CloseSocket(gSocket);
    }
    gSocket = -1;
    return 0;
}

// IDA: void __cdecl PDNetStartProducingJoinList()
void PDNetStartProducingJoinList(void) {
    LOG_TRACE("()");

    dr_dprintf("PDNetStartProducingJoinList()");
    gNumber_of_hosts = 0;
    gJoinable_games = BrMemAllocate(sizeof(tPD_net_game_info) * JOINABLE_GAMES_CAPACITY, 0x80u);
    if (gJoinable_games == NULL) {
        PDFatalError("Can't allocate memory for joinable games");
    }
}

// IDA: void __cdecl PDNetEndJoinList()
void PDNetEndJoinList(void) {
    LOG_TRACE("()");

    dr_dprintf("PDNetEndJoinList()");
    if (gJoinable_games) {
        BrMemFree(gJoinable_games);
    }
    gJoinable_games = NULL;
}

// IDA: int __usercall PDNetGetNextJoinGame@<EAX>(tNet_game_details *pGame@<EAX>, int pIndex@<EDX>)
int PDNetGetNextJoinGame(tNet_game_details* pGame, int pIndex) {
    static tU32 next_broadcast_time = 0;
    int i;
    int j;
    int number_of_hosts_has_changed;
    char str[256];
    LOG_TRACE9("(%p, %d)", pGame, pIndex);

    dr_dprintf("PDNetGetNextJoinGame(): pIndex is %d", pIndex);
    if (pIndex == 0) {
        do {
            number_of_hosts_has_changed = 0;
            for (i = 0; i < gNumber_of_hosts; i++) {
                if (gJoinable_games[i].last_response + 10000 < PDGetTotalTime()) {
                    number_of_hosts_has_changed = 1;
                    for (j = i; j < gNumber_of_hosts - 1; j++) {
                        memcpy(&gJoinable_games[j], &gJoinable_games[j + 1], sizeof(tPD_net_game_info));
                    }
                }
            }
            if (number_of_hosts_has_changed) {
                gNumber_of_hosts--;
            }
        } while (number_of_hosts_has_changed);
        if (PDGetTotalTime() > next_broadcast_time) {
            next_broadcast_time = PDGetTotalTime() + 3000;
            MakeMessageToSend(1);
            if (BroadcastMessage() == 0) {
                dr_dprintf("PDNetGetNextJoinGame(): Error on BroadcastMessage()");
            }
        }
    }
    ReceiveHostResponses();
    if (gNumber_of_hosts <= pIndex) {
        return 0;
    }
    dr_dprintf("PDNetGetNextJoinGame(): Adding game.");
    // danger: copying a tPD_net_game_info into a tPD_net_player_info
    pGame->pd_net_info = *(tPD_net_player_info*)&gJoinable_games[pIndex];
    return 1;
}

// IDA: void __usercall PDNetDisposeGameDetails(tNet_game_details *pDetails@<EAX>)
void PDNetDisposeGameDetails(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDNetHostGame@<EAX>(tNet_game_details *pDetails@<EAX>, char *pHost_name@<EDX>, void **pHost_address@<EBX>)
int PDNetHostGame(tNet_game_details* pDetails, char* pHost_name, void** pHost_address) {
    LOG_TRACE("(%p, \"%s\", %p)", pDetails, pHost_name, pHost_address);

    dr_dprintf("PDNetHostGame()");
    //*pHost_address = &gLocal_addr;
    *pHost_address = &gLocal_addr_copyable;
    return 1;
}

// IDA: int __usercall PDNetJoinGame@<EAX>(tNet_game_details *pDetails@<EAX>, char *pPlayer_name@<EDX>)
int PDNetJoinGame(tNet_game_details* pDetails, char* pPlayer_name) {
    LOG_TRACE("(%p, \"%s\")", pDetails, pPlayer_name);

    dr_dprintf("PDNetJoinGame()");
    return 0;
}

// IDA: void __usercall PDNetLeaveGame(tNet_game_details *pDetails@<EAX>)
void PDNetLeaveGame(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);

    dr_dprintf("PDNetLeaveGame()");
}

// IDA: void __usercall PDNetHostFinishGame(tNet_game_details *pDetails@<EAX>)
void PDNetHostFinishGame(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);

    dr_dprintf("PDNetHostFinishGame()");
}

// IDA: tU32 __usercall PDNetExtractGameID@<EAX>(tNet_game_details *pDetails@<EAX>)
tU32 PDNetExtractGameID(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);

    dr_dprintf("PDNetExtractGameID()");
    // dethrace changed
    // return ntohs(gLocal_addr.sin_addr.s_addr);
    return gPlayer_id;
}

// IDA: tPlayer_ID __usercall PDNetExtractPlayerID@<EAX>(tNet_game_details *pDetails@<EAX>)
tPlayer_ID PDNetExtractPlayerID(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);

    dr_dprintf("PDNetExtractPlayerID()");
    // dethrace changed
    // return ntohs(gLocal_addr.sin_addr.s_addr);
    return gPlayer_id;
}

// IDA: void __usercall PDNetObtainSystemUserName(char *pName@<EAX>, int pMax_length@<EDX>)
void PDNetObtainSystemUserName(char* pName, int pMax_length) {
    int result;
    char* found;
    dr_dprintf("PDNetObtainSystemUserName()");

    result = gethostname(pName, pMax_length);
    if (result == 0) {
    }

    while (1) {
        found = strpbrk(pName, "_=(){}[]<>!$%^&*/:@~;'#,?\\|`\"");
        if (found == NULL) {
            break;
        }
        *found = '-';
    }
}

// IDA: int __usercall PDNetSendMessageToPlayer@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, tPlayer_ID pPlayer@<EBX>)
int PDNetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer) {
    char str[256];
    LOG_TRACE("(%p, %p, %d)", pDetails, pMessage, pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDNetSendMessageToAllPlayers@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
int PDNetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage) {
    char str[256];
    int i;
    LOG_TRACE("(%p, %p)", pDetails, pMessage);

    struct sockaddr_in someaddr;

    for (i = 0; i < gNumber_of_net_players; ++i) {
        if (i == gThis_net_player_index) {
            continue;
        }

        PDNetCopyToNative(&someaddr, &gNet_players[i].pd_net_info.addr_in);
        SockAddrToString(str, &someaddr);
        LOG_DEBUG(str);
        if (sendto(gSocket, (const char*)pMessage, pMessage->overall_size, 0, (struct sockaddr*)&someaddr, sizeof(someaddr)) == -1) {
            dr_dprintf("PDNetSendMessageToAllPlayers(): Error on sendto() - WSAGetLastError=%d", OS_GetLastSocketError());
            NetDisposeMessage(pDetails, pMessage);
            return 1;
        }
    }
    NetDisposeMessage(pDetails, pMessage);
    return 0;
}

// IDA: tNet_message* __usercall PDNetGetNextMessage@<EAX>(tNet_game_details *pDetails@<EAX>, void **pSender_address@<EDX>)
tNet_message* PDNetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    char* receive_buffer;
    char str[256];
    int msg_type;
    LOG_TRACE("(%p, %p)", pDetails, pSender_address);

    char addr_str[32];
    unsigned int sa_len;
    int res;
    tNet_message* msg;

    sa_len = sizeof(gRemote_addr);
    msg = NetAllocateMessage(512);
    receive_buffer = (char*)msg;
    res = recvfrom(gSocket, receive_buffer, 512, 0, (struct sockaddr*)&gRemote_addr, &sa_len);
    res = res != -1;
    if (res == 0) {
        res = OS_GetLastSocketError() != EWOULDBLOCK;
        if (res) {
            sprintf(str, "PDNetGetNextMessage(): Error on recvfrom() - WSAGetLastError=%d", res);
            PDFatalError(str);
        }
    } else {
        SockAddrToString(addr_str, &gRemote_addr);
        if (!SameEthernetAddress(&gLocal_addr, &gRemote_addr)) {
            msg_type = GetMessageTypeFromMessage(receive_buffer);
            switch (msg_type) {
            case 1:
                if (gNet_mode == eNet_mode_host) {
                    dr_dprintf("PDNetGetNextMessage(): Received '%s' from '%s', replying to joiner", receive_buffer, addr_str);
                    MakeMessageToSend(2);
                    if (sendto(gSocket, gSend_buffer, strlen(gSend_buffer) + 1, 0, (struct sockaddr*)&gRemote_addr, sizeof(gRemote_addr)) == -1) {
                        dr_dprintf("PDNetGetNextMessage(): Error on sendto() - WSAGetLastError=%d", OS_GetLastSocketError());
                    }
                }
                break;
            case 2:
                // no-op
                break;
            default:
                dr_dprintf("PDNetGetNextMessage(): res is %d, received message type %d from '%s', passing up", res, msg->contents.header.type, addr_str);
                memcpy(&gLast_received_addr, &gRemote_addr, sizeof(gLast_received_addr));

                // Changed by dethrace
                // *pSender_address = &gLast_received_addr;
                PDNetCopyFromNative(&gLast_received_addr_copyable, &gLast_received_addr);
                *pSender_address = &gLast_received_addr_copyable;
                return msg;
            }
        }
    }
    msg->guarantee_number = 0;
    NetDisposeMessage(pDetails, msg);
    return 0;
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

    dr_dprintf("PDNetSetPlayerSystemInfo()");
    memcpy(&pPlayer->pd_net_info, pSender_address, sizeof(pPlayer->pd_net_info));
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
    struct sockaddr_in someaddr;

    PDNetCopyToNative(&someaddr, pAddress);

    SockAddrToString(str, &someaddr);

    if (sendto(gSocket, (const char*)pMessage, pMessage->overall_size, 0, (struct sockaddr*)&someaddr, sizeof(someaddr)) == -1) {
        dr_dprintf("PDNetSendMessageToAddress(): Error on sendto() - WSAGetLastError=%d", OS_GetLastSocketError());
        NetDisposeMessage(pDetails, pMessage);
        return 1;
    }

    NetDisposeMessage(pDetails, pMessage);
    return 0;
}

// IDA: int __usercall PDNetInitClient@<EAX>(tNet_game_details *pDetails@<EAX>)
int PDNetInitClient(tNet_game_details* pDetails) {
    LOG_TRACE("(%p)", pDetails);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDNetGetHeaderSize()
int PDNetGetHeaderSize(void) {
    LOG_TRACE("()");

    return 0;
}

void PDNetCopyFromNative(tCopyable_sockaddr_in* pAddress, struct sockaddr_in* sock) {
    pAddress->port = sock->sin_port;
    pAddress->address = sock->sin_addr.s_addr;
}

void PDNetCopyToNative(struct sockaddr_in* sock, tCopyable_sockaddr_in* pAddress) {
    sock->sin_addr.s_addr = pAddress->address;
    sock->sin_port = pAddress->port;
    sock->sin_family = AF_INET;
}
