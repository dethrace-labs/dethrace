#ifndef PC_ALL_NET_TYPES_H
#define PC_ALL_NET_TYPES_H

#include <dr_types.h>

#if _MSC_VER == 1020
typedef struct sockaddr_ipx {
    short sa_family;          // AF_IPX = 6
    char sa_netnum[4];        // network number
    char sa_nodenum[6];       // node number (MAC addr)
    unsigned short sa_socket; // socket number
} SOCKADDR_IPX, *PSOCKADDR_IPX;
#endif

// This file added dethrace
//  - have switched out IPX implementation for IP
//  - cross-platform instead of per-platform implementation
// cannot be a regular sockaddr_in because it is transmitted between OS's
typedef struct tCopyable_sockaddr_in {
#if _MSC_VER == 1020
    SOCKADDR_IPX addr_ipx;
#else
    uint64_t address;
    uint32_t port;
#endif

} tCopyable_sockaddr_in;

typedef struct tPD_net_player_info {
    // cannot be a regular sockaddr_in because it is transmitted between OS's
    tCopyable_sockaddr_in addr_in;

} tPD_net_player_info;

// has to match `tPD_net_player_info` - see `PDNetGetNextJoinGame`
typedef struct tPD_net_game_info {
    // cannot be a regular sockaddr_in because it is transmitted between OS's
    tCopyable_sockaddr_in addr_in;

    unsigned int last_response;
} tPD_net_game_info;

#endif
