#include "sdl_network.h"
#include <brender/brender.h>
#include "common/errors.h"
#include "common/globvrpb.h"
#include "common/network.h"
#include "pd/net.h"
#include "pd/sys.h"
#include "harness.h"

#include "SDL.h"
#include "SDL_net.h"

#define JOINABLE_GAMES_CAPACITY     2

#define DEFAULT_PORT 6969
#define NBPORTS 10  // DEFAULT_PORT, DEFAULT_PORT+1, DEFAULT_PORT+2, ...

typedef struct {
    tU32 last_response;
    IPaddress addr;
} tPDSDLNetwork_net_game_info;

typedef struct {
    IPaddress addr;
} tPDSDL_game_details_addr;


static tPDSDLNetwork_net_game_info* gPDSDLNetwork_Joinable_games;
static int gPDSDLNetwork_Port;
static UDPsocket gPDSDLNetwork_Socket;
static UDPpacket *gPDSDLNetwork_SendPacket;
static UDPpacket *gPDSDLNetwork_RecvPacket;
static IPaddress gPDSDLNetwork_Interfaces[16];
static IPaddress gPDSDLNetwork_AddressBuffer;

static char* FormatIPaddress(IPaddress* address) {
    static char buffer[32];
    Uint32 host;
    Uint32 port;

    host = SDLNet_Read32(&address->host);
    port = SDLNet_Read16(&address->port);
    sprintf(buffer, "%d.%d.%d.%d:%d",
        (host >> 24) & 0xff,
        (host >> 16) & 0xff,
        (host >>  8) & 0xff,
        (host >>  0) & 0xff,
        port);
    return buffer;
}

static int CompareAddress(IPaddress* ipAddress1, IPaddress* ipAddress2) {
    return (ipAddress1->host == ipAddress2->host && ipAddress1->port == ipAddress2->port) ? 0 : 1;
}

int SDLNetwork_Init() {
    SDL_version compile_version;
    const SDL_version* link_version;
    int i;
    LOG_INFO("()");

    link_version = SDLNet_Linked_Version();
    SDL_NET_VERSION(&compile_version);
    LOG_INFO("Compiled with SDL_net version: %d.%d.%d",
        compile_version.major,
        compile_version.minor,
        compile_version.patch);
    LOG_INFO("Running with SDL_net version: %d.%d.%d",
        link_version->major,
        link_version->minor,
        link_version->patch);

    if (sizeof(((tPD_net_player_info*)NULL)->pd_addr) < sizeof(IPaddress)) {
        LOG_WARN("Not enough space to store an address (need %d bytes, have %d bytes)", sizeof(IPaddress), sizeof(((tPD_net_player_info*)NULL)->pd_addr));
        return -1;
    }

    // SDL is reference counted, so initializing multiple times is ok
    if (SDL_InitSubSystem(0) == -1) {
        LOG_WARN("SDL_InitSubSystem: %s", SDL_GetError());
        return -1;
    }

    if (SDLNet_Init() == -1) {
        LOG_WARN("SDLNet_Init: %s", SDLNet_GetError());
        goto fail_destroy_sdl;
    }

    for (i = DEFAULT_PORT; i < DEFAULT_PORT + NBPORTS; i++) {
        // UDP sockets created by SDL_net support broadcasting by default
        gPDSDLNetwork_Socket = SDLNet_UDP_Open(i);
        if (gPDSDLNetwork_Socket != NULL) {
            gPDSDLNetwork_Port = i;
            LOG_INFO("SDLNet_UDP_Open(%d) succeeded", i);
            break;
        }
        LOG_WARN("SDLNet_UDP_Open(%d) failed: %s", i, SDLNet_GetError());
    }
    if (gPDSDLNetwork_Socket == NULL) {
        LOG_WARN("Giving up trying to open a port");
        goto fail_destroy_data;
    }

    gNumber_of_networks = SDLNet_GetLocalAddresses(gPDSDLNetwork_Interfaces, COUNT_OF(gPDSDLNetwork_Interfaces));
    LOG_INFO("Found %d interfaces:", gNumber_of_networks);
    for (i = 0; i < gNumber_of_networks; i++) {
        SDLNet_Write16(gPDSDLNetwork_Port, &gPDSDLNetwork_Interfaces[i].port);
        LOG_INFO("[% 2d] %s %s", i, FormatIPaddress(&gPDSDLNetwork_Interfaces[i]), SDLNet_ResolveIP(&gPDSDLNetwork_Interfaces[i]));
    }
    dr_dprintf("Total networks = %d", gNumber_of_networks);

    gPDSDLNetwork_SendPacket = SDLNet_AllocPacket(512);
    if (gPDSDLNetwork_SendPacket == NULL) {
        LOG_WARN("SDLNet_AllocPacket() failed: %s", SDLNet_GetError());
        goto fail_destroy_data;
    }

    gPDSDLNetwork_RecvPacket = SDLNet_AllocPacket(512);
    if (gPDSDLNetwork_RecvPacket == NULL) {
        LOG_WARN("SDLNet_AllocPacket() failed: %s", SDLNet_GetError());
        goto fail_destroy_data;
    }

    gMsg_header_strlen = strlen(MESSAGE_HEADER_STR);

    return 0;
fail_destroy_data:
    if (gPDSDLNetwork_SendPacket != NULL) {
        SDLNet_FreePacket(gPDSDLNetwork_SendPacket);
        gPDSDLNetwork_SendPacket = NULL;
    }
    if (gPDSDLNetwork_RecvPacket != NULL) {
        SDLNet_FreePacket(gPDSDLNetwork_RecvPacket);
        gPDSDLNetwork_RecvPacket = NULL;
    }
    if (gPDSDLNetwork_Socket != NULL) {
        SDLNet_UDP_Close(gPDSDLNetwork_Socket);
        gPDSDLNetwork_Socket = NULL;
    }
    SDLNet_Quit();
fail_destroy_sdl:
    SDL_QuitSubSystem(0);
    return -1;
}

int SDLNetwork_Shutdown() {

    SDLNet_FreePacket(gPDSDLNetwork_SendPacket);
    SDLNet_UDP_Close(gPDSDLNetwork_Socket);
    SDLNet_Quit();
    SDL_QuitSubSystem(0);
    return 0;
}

void SDLNetwork_StartProducingJoinList() {
    LOG_INFO("()");

    gNumber_of_hosts = 0;
    gPDSDLNetwork_Joinable_games = BrMemAllocate(JOINABLE_GAMES_CAPACITY * sizeof(tPDSDLNetwork_net_game_info), 0x80);
    if (gPDSDLNetwork_Joinable_games == NULL) {
        PDFatalError("Can't allocate memory for joinable games");
    }
}

void SDLNetwork_NetEndJoinList() {
    LOG_INFO("()");

    if (gPDSDLNetwork_Joinable_games != NULL) {
        BrMemFree(gPDSDLNetwork_Joinable_games);
    }
    gPDSDLNetwork_Joinable_games = NULL;
}

int SDLNetwork_NetGetNextJoinGame(tNet_game_details* pDetails, int pIndex) {
    static tU32 next_broadcast_time;
    int i;
    int j;
    // char str[256];
    LOG_TRACE9("(%p, %d)", pDetails, pIndex);

    if (pIndex == 0) {
        for (i = 0; i < gNumber_of_hosts; i++) {
            if ((int)(gPDSDLNetwork_Joinable_games[i].last_response + 10000) < PDGetTotalTime()) {
                for (j = i; j < gNumber_of_hosts - 1; j++) {
                    memcpy(&gPDSDLNetwork_Joinable_games[j], &gPDSDLNetwork_Joinable_games[j + 1], sizeof(tPDSDLNetwork_net_game_info));
                }
                gNumber_of_hosts--;
                i--;
            }
        }
        if ((int)next_broadcast_time < PDGetTotalTime()) {
            next_broadcast_time = PDGetTotalTime() + 3000;
            MakeMessageToSend(1);
            if (!BroadcastMessage()) {
                dr_dprintf("PDNetGetNextJoinGame(): Error on BroadcastMessage()");
            }
        }
    }
    ReceiveHostResponses();
    if (pIndex < gNumber_of_hosts) {
        dr_dprintf("PDNetGetNextJoinGame(): Adding game.");
        memcpy(pDetails->pd_net_info.pd_addr, &gPDSDLNetwork_Joinable_games->addr, sizeof(gPDSDLNetwork_Joinable_games->addr));
    }

    return pIndex < gNumber_of_hosts;
}

int SDLNetwork_NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {
    int r;

    memcpy(gPDSDLNetwork_SendPacket->data, pMessage, pMessage->overall_size);
    gPDSDLNetwork_SendPacket->len = pMessage->overall_size;
    r = SDLNet_UDP_Send(gPDSDLNetwork_Socket, -1, gPDSDLNetwork_SendPacket);
    if (r == -1) {
        dr_dprintf("PDNetSendMessageToAddress(): Error on send - %s", SDLNet_GetError());
        NetDisposeMessage(pDetails, pMessage);
        return 1;
    }
    NetDisposeMessage(pDetails, pMessage);
    return 0;
}

char* SDLNetwork_FormatAddress(void* pAddress) {

    return FormatIPaddress(pAddress);
}

int SDLNetwork_CompareAddresses(void* pAddress1, void* pAddress2) {

    return CompareAddress(pAddress1, pAddress2);
}

static IPaddress create_broadcast_address(Uint16 port) {
    Uint32 host_native;
    IPaddress result;

    // FIXME: don't assume 2nd element is network address (and not 127.0.0.1)
    host_native = SDLNet_Read32(&gPDSDLNetwork_Interfaces[1].host);
    host_native = host_native | 0xff;
    result.host = SDLNet_Read32(&host_native);
    result.port = SDLNet_Read16(&port);

    return result;
}

int SDLNetwork_BroadcastMessage() {
    int i;
    int res;

    // return 0 when error
    gPDSDLNetwork_SendPacket->len = strlen(gSend_buffer);
    memcpy(gPDSDLNetwork_SendPacket->data, gSend_buffer, gPDSDLNetwork_SendPacket->len);
    for (i = 0; i < NBPORTS; i++) {
        gPDSDLNetwork_SendPacket->address = create_broadcast_address(DEFAULT_PORT + i);
//        printf("addr: %s, len=%d\n", FormatIPaddress(&gPDSDLNetwork_SendPacket->address), gPDSDLNetwork_SendPacket->len);
        res = SDLNet_UDP_Send(gPDSDLNetwork_Socket, -1, gPDSDLNetwork_SendPacket);
        if (res == 0) {
            dr_dprintf("Failed to broadcast message to port %d.", DEFAULT_PORT + i);
        }
    }
    return 1;
}

static int AddressInInterfaces(IPaddress* pAddress) {
    int i;

    for (i = 0; i < gNumber_of_networks; i++) {
        if (CompareAddress(&gPDSDLNetwork_Interfaces[i], pAddress) == 0) {
            return 1;
        }
    }
    return 0;
}

int SDLNetwork_ReceiveHostResponses(void) {
    int r;
    int i;
    int already_registered;

    while (1) {
        r = SDLNet_UDP_Recv(gPDSDLNetwork_Socket, gPDSDLNetwork_RecvPacket);
        if (r == -1) {
            dr_dprintf("ReceiveHostResponses(SDLNet_UDP_Recv) failed: %s\n", SDLNet_GetError());
            return 1;
        } else if (r == 0) {
            return 0;
        }
        dr_dprintf("ReceiveHostResponses(): Received string '%s' from %s", gPDSDLNetwork_RecvPacket->data, FormatIPaddress(&gPDSDLNetwork_RecvPacket->address));
        if (AddressInInterfaces(&gPDSDLNetwork_RecvPacket->address)) {
            dr_dprintf("*** Discounting the above 'cos we sent it ***");
            continue;
        }
        if (GetMessageTypeFromMessage((char*)gPDSDLNetwork_RecvPacket->data) != 2) {
            dr_dprintf("*** Discounting the above 'cos it's not a host reply ***");
            continue;
        }
        already_registered = 0;
        for (i = 0; i < gNumber_of_hosts; i++) {
            if (CompareAddress(&gPDSDLNetwork_Joinable_games[i].addr, &gPDSDLNetwork_RecvPacket->address) == 0) {
                already_registered = 1;
                break;
            }
        }
        if (already_registered) {
            dr_dprintf("That game already registered");
            gPDSDLNetwork_Joinable_games[i].last_response = PDGetTotalTime();
        } else {
            dr_dprintf("Adding joinable game to slot #%d", gNumber_of_hosts);
            gPDSDLNetwork_Joinable_games[gNumber_of_hosts].addr = gPDSDLNetwork_RecvPacket->address;
            gPDSDLNetwork_Joinable_games[gNumber_of_hosts].last_response = PDGetTotalTime();
            gNumber_of_hosts++;
            dr_dprintf("Number of games found so far: %d", gNumber_of_hosts);
        }
        if (gNumber_of_hosts != 0) {
            dr_dprintf("Currently registered net games:");
            for (i = 0; i < gNumber_of_hosts; i++) {
                dr_dprintf("%d: Host addr %s", i, FormatIPaddress(&gPDSDLNetwork_Joinable_games[i].addr));
            }
        }
    }
}

// messageType == 1 ==> request to join
// messageType == 2 ==> answer to joiner
// everything else

tNet_message* SDLNetwork_PDNetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {
    tNet_message* message;
    int r;
    int messageType;

    r = SDLNet_UDP_Recv(gPDSDLNetwork_Socket, gPDSDLNetwork_RecvPacket);
    if (r == -1) {
        dr_dprintf("SDLNet_UDP_Recv() failed: %s", SDLNet_GetError());
        return NULL;
    } else if (r == 0) {
        return NULL;
    }

    printf("Received message from %s\n", FormatIPaddress(&gPDSDLNetwork_RecvPacket->address));
    if (AddressInInterfaces(&gPDSDLNetwork_RecvPacket->address)) {
        return NULL;
    }
    messageType = GetMessageTypeFromMessage((char*)gPDSDLNetwork_RecvPacket->data);
    if (messageType == NETMSGID_DETAILS) {
        if (gNet_mode == eNet_mode_host) {
            dr_dprintf("PDNetGetNextMessage(): Received '%s' from '%s', replying to joiner", gPDSDLNetwork_RecvPacket->data, FormatIPaddress(&gPDSDLNetwork_RecvPacket->address));
            MakeMessageToSend(NETMSGID_JOIN);
            gPDSDLNetwork_SendPacket->maxlen = COUNT_OF(gSend_buffer);
            gPDSDLNetwork_SendPacket->len = strlen(gSend_buffer);
            memcpy(gPDSDLNetwork_SendPacket->data, gSend_buffer, gPDSDLNetwork_SendPacket->len);
            gPDSDLNetwork_SendPacket->address = gPDSDLNetwork_RecvPacket->address;
            r = SDLNet_UDP_Send(gPDSDLNetwork_Socket, -1, gPDSDLNetwork_SendPacket);
            if (r == -1) {
                dr_dprintf("SDLNet_UDP_Send failed: %s", SDLNet_GetError());
            }
            return NULL;
        }
    } else if (messageType != 2) {
        message = NetAllocateMessage(gPDSDLNetwork_RecvPacket->len);
        memcpy(message, gPDSDLNetwork_RecvPacket->data, gPDSDLNetwork_RecvPacket->len);
        dr_dprintf("PDNetGetNextMessage(): res is %d, received message type %d from '%s', passing up", r, message->contents.header.type, FormatIPaddress(&gPDSDLNetwork_RecvPacket->address));
        gPDSDLNetwork_AddressBuffer = gPDSDLNetwork_RecvPacket->address;
        *pSender_address = (void*)&gPDSDLNetwork_AddressBuffer;
        return message;
    }
    return NULL;
}

int SDLNetwork_PDNetHostGame(tNet_game_details* pDetails, char* pHost_name, void** pHost_address) {
    if (gNumber_of_networks == 0) {
        return 0;
    }
    // FIXME: auto-select correct host address (avoid 127.0.0.1)
    *pHost_address = &gPDSDLNetwork_Interfaces[1];
    memcpy(pDetails->pd_net_info.pd_addr, &gPDSDLNetwork_Interfaces[1], sizeof(IPaddress));
    return 1;
}

tPlayer_ID SDLNetwork_PDNetExtractPlayerID(tNet_game_details* pDetails) {
    IPaddress* addr = (IPaddress*)&pDetails->pd_net_info;
    // FIXME: Improve hash function. Include host name?
    return addr->host ^ addr->port;
}

int SDLNetwork_NetSendto(char* message, int size, void* pAddress) {
    NOT_IMPLEMENTED();
    return -1;
}
