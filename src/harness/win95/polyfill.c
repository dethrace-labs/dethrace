#include "harness/hooks.h"
#include "harness/os.h"
#include "harness/win95_polyfill.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)

LARGE_INTEGER qpc_start_time, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER qpc_ticks_per_sec;

#include <direct.h>
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
#define getcwd _getcwd
#define chdir _chdir
extern HANDLE Real_FindFirstFile(char* lpFileName, char* lpFindFileData_filename);
extern HANDLE Real_FindNextFile(HANDLE hFindFile, char* lpFindFileData_filename);
extern BOOL Real_FindClose(HANDLE hFindFile);
#else
#include <dirent.h>
#include <unistd.h>
#endif

uint32_t polyfill_first_clock_time = 0;

DWORD GetFileAttributesA(char* lpFileName) {

    FILE* f = fopen(lpFileName, "r");
    if (!f) {
        return INVALID_FILE_ATTRIBUTES;
    }
    return FILE_ATTRIBUTE_NORMAL;
}

BOOL SetFileAttributesA(char* lpFileName, DWORD dwFileAttributes) {
    // no-op for now
    return 0;
}

HANDLE CreateFileA(char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, void* lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {

    assert(dwDesiredAccess == GENERIC_READ);
    assert(lpSecurityAttributes == NULL);
    assert(dwCreationDisposition == OPEN_EXISTING);

    FILE* f = fopen(lpFileName, "r");
    if (!f) {
        return INVALID_HANDLE_VALUE;
    }

    return f;
}

DWORD GetFileSize(HANDLE hFile, DWORD* lpFileSizeHigh) {
    assert(lpFileSizeHigh == NULL);

    long current_offset = ftell(hFile);
    fseek(hFile, 0, SEEK_END);
    long size = ftell(hFile);
    fseek(hFile, current_offset, SEEK_SET);
    return size;
}

int CloseHandle(HANDLE hObject) {
    fclose(hObject);
    return 0;
}

void GlobalMemoryStatus(MEMORYSTATUS* lpBuffer) {
    lpBuffer->dwTotalPhys = 64000000;
    lpBuffer->dwAvailPhys = 64000000; // pretend we have a whole 64mb of ram!
    lpBuffer->dwAvailPageFile = 0;
    lpBuffer->dwTotalVirtual = 0;
    lpBuffer->dwAvailVirtual = 0;
    lpBuffer->dwMemoryLoad = 0;
    lpBuffer->dwTotalPageFile = 0;
}

BOOL GetCursorPos(LPPOINT lpPoint) {
    return gHarness_platform.GetCursorPos(lpPoint);
}

BOOL ScreenToClient(HWND hWnd, LPPOINT lpPoint) {
    // no-op, we assume the point is already relative to client
}

DWORD timeGetTime() {
    return gHarness_platform.GetTicks();
}

DWORD GetCurrentDirectoryA(DWORD nBufferLength, char* lpBuffer) {
    char* ret = getcwd(lpBuffer, nBufferLength);
    if (ret == NULL) {
        return 0;
    }
    return strlen(lpBuffer);
}

BOOL SetCurrentDirectoryA(char* lpPathName) {
    int ret = chdir(lpPathName);
    // chdir returning zero means success, SetCurrentDirectory returning non-zero means success
    if (ret == 0) {
        return 1;
    } else {
        return 0;
    }
}

HANDLE FindFirstFileA(char* lpFileName, WIN32_FIND_DATAA* lpFindFileData) {
    assert(strcmp(lpFileName, "*.???") == 0);

#if defined(_WIN32) || defined(_WIN64)
    return Real_FindFirstFile(lpFileName, lpFindFileData.cFileName);
#else
    DIR* dir;
    strcpy(lpFileName, ".");
    dir = opendir(lpFileName);
    if (dir == NULL) {
        return INVALID_HANDLE_VALUE;
    }
    if (FindNextFileA(dir, lpFindFileData)) {
        return dir;
    } else {
        closedir(dir);
        return INVALID_HANDLE_VALUE;
    }
#endif
}

BOOL FindNextFileA(HANDLE hFindFile, WIN32_FIND_DATAA* lpFindFileData) {
#if defined(_WIN32) || defined(_WIN64)
    return Real_FindNextFile(hFindFile, lpFindFileData.cFileName);
#else
    struct dirent* entry;

    if (hFindFile == NULL) {
        return 0;
    }
    while ((entry = readdir(hFindFile)) != NULL) {
        if (entry->d_type == DT_REG) {
            strcpy(lpFindFileData->cFileName, entry->d_name);
            return 1;
        }
    }
    return 0;
#endif
}

BOOL FindClose(HANDLE hFindFile) {
#if defined(_WIN32) || defined(_WIN64)
    Real_FindClose(hFindFile);
#else
    return closedir(hFindFile);

#endif
}

HWND CreateWindowExA(DWORD dwExStyle, char* lpClassName, char* lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, void* hMenu, void* hInstance, LPVOID lpParam) {
    return gHarness_platform.CreateWindow(lpWindowName, X, Y, nWidth, nHeight);
}

int SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) {
    return gHarness_platform.SetWindowPos(hWnd, X, Y, cx, cy);
}

int ShowCursor(int bShow) {
    gHarness_platform.ShowCursor(bShow);
    return 0;
}

int SendMessageA(HWND hWnd, UINT Msg, UINT wParam, long lParam) {
    return 0;
}

int MessageBoxA(HWND hWnd, char* lpText, char* lpCaption, UINT uType) {
    return 0;
}

BOOL DestroyWindow(HWND hWnd) {
    gHarness_platform.DestroyWindow(hWnd);
    return 0;
}

void ExitProcess(UINT uExitCode) {
    exit(uExitCode);
}

void TranslateMessage(MSG* lpMsg) {
    // no-op
}

void DispatchMessageA(MSG* lpMsg) {
    // no-op
}

int PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {
    return gHarness_platform.GetMessage(lpMsg);
}

int GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) {
    return gHarness_platform.GetMessage(lpMsg);
}

void Sleep(DWORD dwMilliseconds) {
    gHarness_platform.Sleep(dwMilliseconds);
    // #if defined(_WIN32) || defined(_WIN64)
    //     LARGE_INTEGER now;
    //     if (qpc_start_time.QuadPart == 0) {
    //         QueryPerformanceFrequency(&qpc_ticks_per_sec);
    //         QueryPerformanceCounter(&qpc_start_time);
    //     }

    //     QueryPerformanceCounter(&now);
    //     return (uint32_t)(((now.QuadPart - qpc_start_time.QuadPart) * 1000) / qpc_ticks_per_sec.QuadPart);
    // #else
    //     struct timespec ts;
    //     ts.tv_sec = dwMilliseconds / 1000;
    //     ts.tv_nsec = (dwMilliseconds % 1000) * 1000000;
    //     nanosleep(&ts, &ts);
    // #endif
}

void DirectDraw_CreateSurface(int width, int height) {
    // no-op
}

void DirectInputDevice_GetDeviceState(unsigned int count, uint8_t* buffer) {
    gHarness_platform.GetKeyboardState(count, buffer);
}

void DirectDrawDevice_SetPaletteEntries(PALETTEENTRY* palette, int pFirst_colour, int pCount) {
    assert(pFirst_colour == 0);
    assert(pCount == 256);
    gHarness_platform.SetPalette(palette);
    Harness_RenderLastScreen();
}
