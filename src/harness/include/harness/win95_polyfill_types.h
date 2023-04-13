#ifndef WIN95_POLYFILL_TYPES_H
#define WIN95_POLYFILL_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef short SHORT;
typedef unsigned short USHORT;
typedef int INT;
typedef int BOOL;
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef long LONG;
typedef uint32_t DWORD;
typedef BYTE* PBYTE;
typedef char* PSTR;
typedef WORD* PWORD;
typedef unsigned short* PSHORT;
typedef LONG* PLONG;
typedef void* PVOID;
typedef BYTE* LPBYTE;
typedef BYTE* LPSTR;
typedef WORD* LPWORD;
typedef LONG* LPLONG;
typedef void* LPVOID;
typedef BYTE* HPBYTE;
typedef BYTE* HPSTR;
typedef LONG* HPLONG;
typedef void* HPVOID;
typedef void* HANDLE;

#define GENERIC_READ 0x80000000
#define OPEN_EXISTING 3
#define FILE_ATTRIBUTE_NORMAL 0x80
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define INVALID_FILE_ATTRIBUTES -1

#define FILE_ATTRIBUTE_READONLY 0x01
#define FILE_ATTRIBUTE_NORMAL 0x80

#define HWND_BROADCAST ((HWND)0xffff)

#define WM_QUIT 0x0012

typedef struct _MEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    size_t dwTotalPhys;
    size_t dwAvailPhys;
    size_t dwTotalPageFile;
    size_t dwAvailPageFile;
    size_t dwTotalVirtual;
    size_t dwAvailVirtual;
} MEMORYSTATUS;

typedef HANDLE HWND;

typedef struct tagPOINT {
    LONG x;
    LONG y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;

typedef struct tagPALETTEENTRY {
    BYTE peRed;
    BYTE peGreen;
    BYTE peBlue;
    BYTE peFlags;
} PALETTEENTRY;

typedef struct _WIN32_FIND_DATA {
    // DWORD dwFileAttributes;
    // FILETIME ftCreationTime;
    // FILETIME ftLastAccessTime;
    // FILETIME ftLastWriteTime;
    // DWORD nFileSizeHigh;
    // DWORD nFileSizeLow;
    // DWORD dwReserved0;
    // DWORD dwReserved1;
    char cFileName[1024];
    // char cAlternateFileName[14];
} WIN32_FIND_DATAA;

typedef struct tagMSG {
    HWND hwnd;
    UINT message;
    int wParam;
    long lParam;
    DWORD time;
    POINT pt;
    DWORD lPrivate;
} MSG, *PMSG, *NPMSG, *LPMSG;

#endif
