#ifndef PC_ALL_NET_TYPES_H
#define PC_ALL_NET_TYPES_H

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

    uint32_t last_response;
} tPD_net_game_info;

// typedef struct tIPX_netnum {
//     unsigned char bNetwork[4];
// } tIPX_netnum;

#endif
