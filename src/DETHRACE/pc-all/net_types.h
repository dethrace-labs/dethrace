#ifndef PC_ALL_NET_TYPES_H
#define PC_ALL_NET_TYPES_H

#include <stdint.h>

// This file added dethrace
//  - have switched out IPX implementation for IP
//  - cross-platform instead of per-platform implementation

// cannot be a regular sockaddr_in because it is transmitted between OS's
typedef struct tCopyable_sockaddr_in {
    uint64_t address;
    uint32_t port;
} tCopyable_sockaddr_in;

typedef struct tPD_net_player_info {
    // cannot be a regular sockaddr_in because it is transmitted between OS's
    tCopyable_sockaddr_in addr_in;

} tPD_net_player_info;

// has to match `tPD_net_player_info` - see `PDNetGetNextJoinGame`
typedef struct tPD_net_game_info {
    // cannot be a regular sockaddr_in because it is transmitted between OS's
    tCopyable_sockaddr_in addr_in;

    uint32_t last_response;
} tPD_net_game_info;

#endif
