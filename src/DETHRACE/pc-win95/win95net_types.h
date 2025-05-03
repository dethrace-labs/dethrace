#ifndef WIN95_NET_TYPES_H
#define WIN95_NET_TYPES_H

// #include "dr_types.h"
#include "harness/win95_polyfill.h"
#include "harness/winsock.h"

// dethrace: have switched out IPX implementation for IP

typedef struct tSockaddr_in {
    uint64_t address;
    uint32_t port;
} tSockaddr_in;

typedef struct tPD_net_player_info {
    // struct sockaddr_ipx addr_ipx;

    // struct sockaddr_in addr_in;
    //  these fields used only for sending between peers to avoid sending platform-specific sockaddr_in
    // uint64_t address_tx;
    // uint32_t port_tx;

    // cannot be a regular sockaddr_in because it is transmitted between machines
    tSockaddr_in addr_in;
    // added by dethrace
    // tU32 pad0;

} tPD_net_player_info;

typedef struct tPD_net_game_info {
    // has to match `tPD_net_player_info`
    // struct sockaddr_ipx addr_ipx;

    // struct sockaddr_in addr_in;
    // uint64_t address_tx;
    // uint32_t port_tx;
    tSockaddr_in addr_in;

    tU32 last_response;
} tPD_net_game_info;

// typedef struct tIPX_netnum {
//     unsigned char bNetwork[4];
// } tIPX_netnum;

#endif
