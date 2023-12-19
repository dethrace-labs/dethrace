#ifndef WIN95_NET_TYPES_H
#define WIN95_NET_TYPES_H

// #include "dr_types.h"
#include "harness/win95_polyfill.h"
#include "harness/winsock.h"

// dethrace: have switched out IPX implementation for IP

typedef struct tPD_net_player_info {
    // struct sockaddr_ipx addr_ipx;
    struct sockaddr_in addr_in;
} tPD_net_player_info;

typedef struct tPD_net_game_info {
    // struct sockaddr_ipx addr_ipx;
    struct sockaddr_in addr_in;
    tU32 last_response;
} tPD_net_game_info;

// typedef struct tIPX_netnum {
//     unsigned char bNetwork[4];
// } tIPX_netnum;

#endif
