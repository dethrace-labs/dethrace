#ifndef WIN95_POLYFILL_TYPES_H
#define WIN95_POLYFILL_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef void* HANDLE_;

#define GENERIC_READ 0x80000000
#define OPEN_EXISTING 3
#define FILE_ATTRIBUTE_NORMAL 0x80
#define INVALID_HANDLE_VALUE ((HANDLE_*)-1)
#define INVALID_FILE_ATTRIBUTES -1

#define FILE_ATTRIBUTE_READONLY 0x01
#define FILE_ATTRIBUTE_NORMAL 0x80

#define HWND_BROADCAST ((void*)0xffff)

#define _CRT_ASSERT 2

#define WM_QUIT 0x0012

#define MB_ICONERROR 0x00000010

typedef struct _MEMORYSTATUS_ {
    uint32_t dwLength;
    uint32_t dwMemoryLoad;
    size_t dwTotalPhys;
    size_t dwAvailPhys;
    size_t dwTotalPageFile;
    size_t dwAvailPageFile;
    size_t dwTotalVirtual;
    size_t dwAvailVirtual;
} MEMORYSTATUS_;

typedef struct tagPOINT_ {
    long x;
    long y;
} POINT_;

#pragma pack(push, 1)
typedef struct tagPALETTEENTRY_ {
    uint8_t peRed;
    uint8_t peGreen;
    uint8_t peBlue;
    uint8_t peFlags;
} PALETTEENTRY_;
#pragma pack(pop)

typedef struct _WIN32_FIND_DATA_ {
    // uint32_t dwFileAttributes;
    // FILETIME ftCreationTime;
    // FILETIME ftLastAccessTime;
    // FILETIME ftLastWriteTime;
    // uint32_t nFileSizeHigh;
    // uint32_t nFileSizeLow;
    // uint32_t dwReserved0;
    // uint32_t dwReserved1;
    char cFileName[1024];
    // char cAlternateFileName[14];
} WIN32_FIND_DATAA_;

typedef struct tagMSG_ {
    void* hwnd;
    unsigned int message;
    int wParam;
    long lParam;
    uint32_t time;
    POINT_ pt;
    uint32_t lPrivate;
} MSG_;

#endif
