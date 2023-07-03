#include "dr_types.h"

#ifdef __DOS__

typedef struct _tagIPX_HEADER {
    unsigned short wChecksum;
    unsigned short wLength;
    unsigned char bTransportControl;
    unsigned char bPacketType;
    unsigned char bDestNetworkNumber[4];
    unsigned char bDestNetworkNode[6];
    unsigned short wDestNetworkSocket;
    unsigned char bSourceNetworkNumber[4];
    unsigned char bSourceNetworkNode[6];
    unsigned short wSourceNetworkSocket;
} _IPX_HEADER;

typedef struct _PACKET {
    char* pData;
    unsigned short wLength;
} _PACKET;

typedef struct _REAL_PACKET {
    short wOffset;
    short wSegment;
    unsigned short wLength;
} _REAL_PACKET;

typedef struct _ECB_PACKET {
    _PACKET sPacket;
    _REAL_PACKET sRealPacket;
} _ECB_PACKET;

typedef struct _tagIPX_ECB {
    char* pLinkAddress;
    char* pESRRoutine;
    unsigned char bInUse;
    unsigned char bCompletionCode;
    unsigned short wSocket;
    unsigned short wConnectionID;
    unsigned short wWorkSpace;
    unsigned char bDriverWorkSpace[12];
    unsigned char bImmediateAddress[6];
    unsigned short wPacketCount;
    _ECB_PACKET sPacket[2];
} _IPX_ECB;

typedef struct _tagIPX_INTERNET_ADDR {
    unsigned char bNetwork[4];
    unsigned char bNode[6];
} _IPX_INTERNET_ADDR;

typedef struct _tagIPX_NETWORK_ADDR {
    _IPX_INTERNET_ADDR sInternetAddr;
    unsigned char bSocket[2];
} _IPX_NETWORK_ADDR;

typedef struct _tagIPX_LOCAL_TARGET {
    _IPX_INTERNET_ADDR sInternetAddr;
    unsigned char bImmediate[6];
} _IPX_LOCAL_TARGET;

typedef struct _tagIPX_ELEMENT {
    unsigned short wFlags;
    unsigned short wOffset;
    _IPX_HEADER sHeader;
    _IPX_ECB sECB;
    _IPX_ECB* pECB;
    _IPX_HEADER* pIPXHeader;
    char* pHeader;
    unsigned short wHSize;
} _IPX_ELEMENT;

typedef struct _PROT_PTR {
    char* pData;
} _PROT_PTR;

typedef struct _REAL_PTR {
    short wOffset;
    short wSegment;
} _REAL_PTR;

typedef struct _PTR {
    _PROT_PTR sPointer;
    _REAL_PTR sRealPtr;
} _PTR;

typedef struct _tag_NETBIOS_NCB {
    unsigned char bCommand;
    unsigned char bReturnCode;
    unsigned char bLocalSession;
    unsigned char bNetworkNameNumber;
    _PTR sPtr;
    unsigned short wLength;
    unsigned char bCallName[16];
    unsigned char bName[16];
    unsigned char bReceiveTimeOut;
    unsigned char bSendTimeOut;
    char* pPostFunction;
    unsigned char bAdapter;
    unsigned char bCompletionCode;
    unsigned char bReserve[14];
} _NETBIOS_NCB;

typedef struct _tagNETBIOS_ADAPTER_STATUS {
    unsigned char bCardID[6];
    unsigned char bReleaseLevel;
    unsigned char bReserved1;
    unsigned char bTypeOfAdapter;
    unsigned char bOldOrNewParameters;
    unsigned short wReportingPeriodMinutes;
    unsigned short wFrameRejectReceivedCount;
    unsigned short wFrameRejectSentCount;
    unsigned short wReceivedDataFrameErrors;
    unsigned short wUnsuccessfulTransmissions;
    long dwGoodTransmissions;
    long dwGoodReceptions;
    unsigned short wRetransmissions;
    unsigned short wExhaustedResourceCount;
    unsigned short wT1TimerExpiredCount;
    unsigned short wTITimerExpiredCount;
    unsigned char bReserved2[4];
    unsigned short wAvailableNCBS;
    unsigned short wMaxNCBSConfigured;
    unsigned short wMaxNCBSPossible;
    unsigned short wBufferOrStationBusyCount;
    unsigned short wMaxDatagramSize;
    unsigned short wPendingSessions;
    unsigned short wMaxSessionsConfigured;
    unsigned short wMaxSessionsPossible;
    unsigned short wMaxFrameSize;
    unsigned short wNameCount;
    struct {
        unsigned char bName[16];
        unsigned char bNameNumber;
        unsigned char bNameStatus;
    } sNameTable[20];
} _NETBIOS_ADAPTER_STATUS;

typedef struct _tagNETBIOS_ELEMENT {
    unsigned short wFlags;
    unsigned short wOffset;
    _NETBIOS_NCB sNCB;
    _NETBIOS_NCB* pNCB;
    char* pHeader;
    unsigned short wHSize;
} _NETBIOS_ELEMENT;

typedef struct _tagNETBIOS_LOCAL_TARGET {
    unsigned char bNode[16];
} _NETBIOS_LOCAL_TARGET;

typedef struct _tagXFER_BLOCK_HEADER {
    unsigned int wSequence;
    unsigned int wType;
    unsigned int wID;
    unsigned int wLength;
    unsigned int wNode;
    unsigned int wUser1;
} _XFER_BLOCK_HEADER;

typedef struct _NETNOW_NODE_ADDR {
    _IPX_LOCAL_TARGET sIPX;
    _NETBIOS_LOCAL_TARGET sNETBIOS;
} _NETNOW_NODE_ADDR;

typedef struct _tagRMI_REGS _RMI_REGS;
typedef struct _tagBREGS _HMI_BREGS;
typedef struct _tagWREGS _HMI_WREGS;
typedef struct _tagDREGS _HMI_DREGS;
typedef struct _tagSREGS _HMI_SREGS;
typedef struct _tagIPX_HEADER _IPX_HEADER;
typedef struct _tagIPX_ECB _IPX_ECB;
typedef struct _tagIPX_INTERNET_ADDR _IPX_INTERNET_ADDR;
typedef struct _tagIPX_NETWORK_ADDR _IPX_NETWORK_ADDR;
typedef struct _tagIPX_LOCAL_TARGET _IPX_LOCAL_TARGET;
typedef struct _tagIPX_ELEMENT _IPX_ELEMENT;
typedef struct _tag_NETBIOS_NCB _NETBIOS_NCB;
typedef struct _tagNETBIOS_ADAPTER_STATUS _NETBIOS_ADAPTER_STATUS;
typedef struct _tagNETBIOS_ELEMENT _NETBIOS_ELEMENT;
typedef struct _tagNETBIOS_LOCAL_TARGET _NETBIOS_LOCAL_TARGET;
typedef struct _tagXFER_BLOCK_HEADER _XFER_BLOCK_HEADER;

typedef struct tPD_net_player_info {
    _IPX_LOCAL_TARGET addr_ipx;
} tPD_net_player_info;

typedef struct tPD_net_game_info {
    _IPX_LOCAL_TARGET addr_ipx;
    tU32 last_response;
} tPD_net_game_info;

#endif
