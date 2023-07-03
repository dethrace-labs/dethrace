typedef unsigned int W32;

typedef struct _tag_sos_evds_struct _SOS_EVDS_STRUCT;
typedef struct _tag_sos_vds_struct _SOS_VDS_STRUCT;
typedef struct _tag_sos_sample _SOS_SAMPLE;
typedef _SOS_SAMPLE* PSOSSAMPLE;
typedef struct _tagCAPABILITIES _SOS_CAPABILITIES;
typedef _SOS_CAPABILITIES* PSOSCAPABILITIES;
typedef struct _SOS_HARDWARE* PSOSHARDWARE;
typedef struct _tag_sos_driver _SOS_DIGI_DRIVER;
typedef _SOS_DIGI_DRIVER* PSOSDIGIDRIVER;
typedef struct _SOS_DRV_FILEHEADER* PSOSDRVFILEHEADER;
typedef struct _SOS_DRV_DRIVERHEADER* PSOSDRVDRIVERHEADER;
typedef struct _tag_sos_system _SOS_SYSTEM;
typedef _SOS_SYSTEM* PSOSSYSTEM;
typedef struct _tag_sos_det_system _SOS_DET_SYSTEM;
typedef _SOS_DET_SYSTEM* PSOSDETSYSTEM;
typedef struct _tag_sos_timer_system _SOS_TIMER_SYSTEM;
typedef _SOS_TIMER_SYSTEM* PSOSTIMERSYSTEM;

typedef struct _tag_sos_evds_struct {
    unsigned int region_size;
    unsigned int offset;
    unsigned int segment;
    unsigned short number_available;
    unsigned short number_used;
    unsigned int page0;
} _SOS_EVDS_STRUCT;

typedef struct _tag_sos_vds_struct {
    unsigned int region_size;
    unsigned int offset;
    unsigned short segment;
    unsigned short ID;
    unsigned int physical;
} _SOS_VDS_STRUCT;

typedef struct _tag_sos_sample {
    char* pSample;
    char* pSampleCurrent;
    char* pSampleLoop;
    unsigned long wLength;
    unsigned long wLoopLength;
    unsigned long wLoopEndLength;
    unsigned long wLoopStage;
    unsigned long wID;
    unsigned long wFlags;
    unsigned long wPriority;
    unsigned long hSample;
    unsigned long wVolume;
    unsigned long wLoopCount;
    unsigned long wRate;
    unsigned long wBitsPerSample;
    unsigned long wChannels;
    unsigned long wFormat;
    unsigned long wPanPosition;
    unsigned long wPanSpeed;
    unsigned long wPanStart;
    unsigned long wPanEnd;
    unsigned long wPanMode;
    unsigned long wTotalBytesProcessed;
    void (*pfnSampleProcessed)(PSOSSAMPLE*);
    void (*pfnSampleDone)(PSOSSAMPLE*);
    void (*pfnSampleLoop)(PSOSSAMPLE*);
    unsigned long wSystem[16];
    unsigned long wUser[16];
    PSOSSAMPLE* pLink;
    PSOSSAMPLE* pNext;
} _SOS_SAMPLE;

typedef struct _tagCAPABILITIES {
    unsigned char szDeviceName[32];
    W32 wDeviceVersion;
    W32 wBitsPerSample;
    W32 wChannels;
    W32 wMinRate;
    W32 wMaxRate;
    W32 wMixerOnBoard;
    W32 wMixerFlags;
    W32 wFlags;
    short* lpPortList;
    short* lpDMAList;
    short* lpIRQList;
    short* lpRateList;
    W32 fBackground;
    W32 wID;
    W32 wTimerID;
} _SOS_CAPABILITIES;

typedef struct _SOS_HARDWARE {
    W32 wPort;
    W32 wIRQ;
    W32 wDMA;
    W32 wParam;
} _SOS_HARDWARE;

typedef struct _tag_sos_driver {
    W32 wFlags;
    unsigned long wDriverRate;
    unsigned long wDriverChannels;
    unsigned long wDriverBitsPerSample;
    unsigned long wDriverFormat;
    unsigned long wMixerChannels;
    unsigned long wDMACountRegister;
    unsigned long wDMAPosition;
    unsigned long wDMALastPosition;
    unsigned long wDMADistance;
    char* pXFERPosition;
    unsigned long wXFERJumpAhead;
    _SOS_SAMPLE* pSampleList;
    void (*pfnPseudoDMAFunction)(void);
    char* pDMABuffer;
    char* pDMABufferEnd;
    unsigned long wDMABufferSize;
    char* pMixingBuffer;
    char* pMixingBufferEnd;
    unsigned long wMixingBufferSize;
    unsigned long wActiveChannels;
    _SOS_SAMPLE* pSamples;
    _SOS_HARDWARE sHardware;
    _SOS_CAPABILITIES sCaps;
    char* lpDriverDS;
    char* lpDriverCS;
    W32 wSize;
    unsigned long dwLinear;
    unsigned long dwDMAPhysical;
    char* lpDMABuffer;
    W32 hMemory;
    W32 wDMARealSeg;
    W32 wID;
    void (*pfnMixFunction)(void);
} _SOS_DIGI_DRIVER;

typedef struct _SOS_DRV_FILEHEADER {
    unsigned char szName[32];
    W32 wDrivers;
    W32 lOffset;
    W32 lFileSize;
} _SOS_DRV_FILEHEADER;

typedef struct _SOS_DRV_DRIVERHEADER {
    unsigned char szName[32];
    W32 lNextDriver;
    W32 wSize;
    W32 wDeviceID;
    W32 wExtenderType;
} _SOS_DRV_DRIVERHEADER;

typedef struct _tag_sos_system {
    W32 wFlags;
    unsigned char szDriverPath[128];
    unsigned char szTempDriverPath[128];
    PSOSDIGIDRIVER pDriver[5];
    _SOS_VDS_STRUCT sVDS;
    _SOS_DRV_FILEHEADER sFileHeader;
    _SOS_DRV_DRIVERHEADER sDriverHeader;
    char* (*pMemAllocFunction)(unsigned long);
    void (*pMemFreeFunction)(char*, W32);
} _SOS_SYSTEM;

typedef struct _tag_sos_det_system {
    W32 wFlags;
    unsigned char szDriverPath[128];
    unsigned char szTempDriverPath[128];
    _SOS_DRV_FILEHEADER sFileHeader;
    _SOS_DRV_DRIVERHEADER sDriverHeader;
    _SOS_CAPABILITIES sCaps;
    PSOSCAPABILITIES pCaps;
    char* lpBufferDS;
    char* lpBufferCS;
    W32 hFile;
    unsigned long dwDriverIndex;
    W32 wDriverIndexCur;
    W32 hMemory;
    unsigned long dwLinear;
} _SOS_DET_SYSTEM;

typedef struct _tag_sos_timer_system {
    W32 wFlags;
    W32 wChipDivisor;
    void (*pfnEvent[16])(void);
    W32 wEventRate[16];
    unsigned long dwAdditiveFraction[16];
    unsigned long dwCurrentSummation[16];
    W32 wMIDIEventSongHandle[16];
    W32 wMIDIActiveSongHandle;
} _SOS_TIMER_SYSTEM;
