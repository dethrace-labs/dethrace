#ifndef WIN95_NET_TYPES_H
#define WIN95_NET_TYPES_H

// #include "dr_types.h"
#include "harness/win95_polyfill.h"
#include "harness/winsock.h"

// dethrace: have switched out IPX implementation for IP

// cannot be a regular sockaddr_in because it is transmitted between OS's
typedef struct tCopyable_sockaddr_in {
    uint64_t address;
    uint32_t port;
} tCopyable_sockaddr_in;

typedef struct tPD_net_player_info {
    // struct sockaddr_ipx addr_ipx;
    // cannot be a regular sockaddr_in because it is transmitted between machines
    tCopyable_sockaddr_in addr_in;
    // added by dethrace
    // tU32 pad0;

} tPD_net_player_info;

// has to match `tPD_net_player_info`
typedef struct tPD_net_game_info {
    // struct sockaddr_ipx addr_ipx;
    // cannot be a regular sockaddr_in because it is transmitted between machines
    tCopyable_sockaddr_in addr_in;

    tU32 last_response;
} tPD_net_game_info;

// typedef struct tIPX_netnum {
//     unsigned char bNetwork[4];
// } tIPX_netnum;

#endif
