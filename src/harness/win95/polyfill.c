
#include "harness/hooks.h"
#include "harness/win95_polyfill.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)

#include <direct.h>
#include <windows.h>
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getcwd-wgetcwd?view=msvc-170
#define getcwd _getcwd
#define chdir _chdir
#else
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#endif

uint32_t polyfill_first_clock_time = 0;

DWORD GetFileAttributesA_(char* lpFileName) {

    FILE* f = fopen(lpFileName, "r");
    if (!f) {
        return INVALID_FILE_ATTRIBUTES;
    }
    return FILE_ATTRIBUTE_NORMAL;
}

BOOL SetFileAttributesA_(char* lpFileName, DWORD dwFileAttributes) {
    // no-op for now
    return 0;
}

HANDLE CreateFileA_(char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, void* lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {

    assert(dwDesiredAccess == GENERIC_READ);
    assert(lpSecurityAttributes == NULL);
    assert(dwCreationDisposition == OPEN_EXISTING);

    FILE* f = fopen(lpFileName, "r");
    if (!f) {
        return INVALID_HANDLE_VALUE;
    }

    return f;
}

DWORD GetFileSize_(HANDLE hFile, DWORD* lpFileSizeHigh) {
    assert(lpFileSizeHigh == NULL);

    long current_offset = ftell(hFile);
    fseek(hFile, 0, SEEK_END);
    long size = ftell(hFile);
    fseek(hFile, current_offset, SEEK_SET);
    return size;
}

int CloseHandle_(HANDLE hObject) {
    fclose(hObject);
    return 0;
}

void GlobalMemoryStatus_(MEMORYSTATUS* lpBuffer) {
    lpBuffer->dwTotalPhys = 64000000;
    lpBuffer->dwAvailPhys = 64000000; // pretend we have a whole 64mb of ram!
    lpBuffer->dwAvailPageFile = 0;
    lpBuffer->dwTotalVirtual = 0;
    lpBuffer->dwAvailVirtual = 0;
    lpBuffer->dwMemoryLoad = 0;
    lpBuffer->dwTotalPageFile = 0;
}

BOOL GetCursorPos_(LPPOINT lpPoint) {
    int x, y;
    gHarness_platform.GetMousePosition(&x, &y);
    lpPoint->x = x;
    lpPoint->y = y;
    return 0;
}

BOOL ScreenToClient_(HWND hWnd, LPPOINT lpPoint) {
    // no-op, we assume the point is already relative to client
    return 0;
}

DWORD timeGetTime_() {
    return gHarness_platform.GetTicks();
}

DWORD GetCurrentDirectoryA_(DWORD nBufferLength, char* lpBuffer) {
    char* ret = getcwd(lpBuffer, nBufferLength);
    if (ret == NULL) {
        return 0;
    }
    return strlen(lpBuffer);
}

BOOL SetCurrentDirectoryA_(char* lpPathName) {
    int ret = chdir(lpPathName);
    // chdir returning zero means success, SetCurrentDirectory returning non-zero means success
    if (ret == 0) {
        return 1;
    } else {
        return 0;
    }
}

HANDLE FindFirstFileA_(char* lpFileName, WIN32_FIND_DATAA_* lpFindFileData) {
    assert(strcmp(lpFileName, "*.???") == 0);

#if defined(_WIN32) || defined(_WIN64)
    WIN32_FIND_DATA fd;
    HANDLE hFile = FindFirstFile(lpFileName, &fd);
    if (hFile != INVALID_HANDLE_VALUE) {
        strcpy(lpFindFileData->cFileName, fd.cFileName);
    }
    return hFile;
#else
    DIR* dir;
    strcpy(lpFileName, ".");
    dir = opendir(lpFileName);
    if (dir == NULL) {
        return INVALID_HANDLE_VALUE;
    }
    if (FindNextFileA_(dir, lpFindFileData)) {
        return dir;
    } else {
        closedir(dir);
        return INVALID_HANDLE_VALUE;
    }
#endif
}

BOOL FindNextFileA_(HANDLE hFindFile, WIN32_FIND_DATAA_* lpFindFileData) {
#if defined(_WIN32) || defined(_WIN64)
    WIN32_FIND_DATA fd;
    int result = FindNextFile(hFindFile, fd);
    if (result) {
        strcpy(lpFindFileData->cFileName, fd.cFileName);
    }
    return result;
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

BOOL FindClose_(HANDLE hFindFile) {
#if defined(_WIN32) || defined(_WIN64)
    FindClose(hFindFile);
#else
    return closedir(hFindFile);

#endif
}

HWND CreateWindowExA_(DWORD dwExStyle, char* lpClassName, char* lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, void* hMenu, void* hInstance, LPVOID lpParam) {
    return gHarness_platform.CreateWindow(lpWindowName, X, Y, nWidth, nHeight);
}

int SetWindowPos_(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) {
    return gHarness_platform.SetWindowPos(hWnd, X, Y, cx, cy);
}

int ShowCursor_(int bShow) {
    gHarness_platform.ShowCursor(bShow);
    return 0;
}

int SendMessageA_(HWND hWnd, UINT Msg, UINT wParam, long lParam) {
    return 0;
}

int MessageBoxA_(HWND hWnd, char* lpText, char* lpCaption, UINT uType) {
    return 0;
}

BOOL DestroyWindow_(HWND hWnd) {
    gHarness_platform.DestroyWindow(hWnd);
    return 0;
}

void ExitProcess_(UINT uExitCode) {
    exit(uExitCode);
}

void TranslateMessage_(MSG* lpMsg) {
    // no-op
}

void DispatchMessageA_(MSG* lpMsg) {
    // no-op
}

int PeekMessageA_(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {
    return gHarness_platform.GetMessage(lpMsg);
}

int GetMessageA_(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) {
    return gHarness_platform.GetMessage(lpMsg);
}

void Sleep_(DWORD dwMilliseconds) {
    gHarness_platform.Sleep(dwMilliseconds);
}

void DirectDraw_CreateSurface(int width, int height) {
    // no-op
}

void DirectInputDevice_GetDeviceState(unsigned int count, uint8_t* buffer) {
    gHarness_platform.GetKeyboardState(count, buffer);
}

void DirectDrawDevice_SetPaletteEntries(PALETTEENTRY_* palette, int pFirst_colour, int pCount) {
    assert(pFirst_colour == 0);
    assert(pCount == 256);
    gHarness_platform.Renderer_SetPalette(palette);
    Harness_RenderLastScreen();
}

void _splitpath_(char* path, char* drive, char* dir, char* fname, char* ext) {
#ifdef _WIN32
    _splitpath(path, NULL, NULL, fname, NULL);
#else
    char* base = basename(path);
    strcpy(fname, base);
#endif
}

int _CrtDbgReport_(int reportType, const char* filename, int linenumber, const char* moduleName, const char* format, ...) {
    printf("_CrtDbgReport: (TODO)\n");
    return 1;
}
