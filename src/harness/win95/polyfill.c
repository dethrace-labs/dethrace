
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

uint32_t GetFileAttributesA_(char* lpFileName) {

    FILE* f = fopen(lpFileName, "r");
    if (!f) {
        return INVALID_FILE_ATTRIBUTES;
    }
    return FILE_ATTRIBUTE_NORMAL;
}

int SetFileAttributesA_(char* lpFileName, uint32_t dwFileAttributes) {
    // no-op for now
    return 0;
}

void* CreateFileA_(char* lpFileName, uint32_t dwDesiredAccess, uint32_t dwShareMode, void* lpSecurityAttributes, uint32_t dwCreationDisposition, uint32_t dwFlagsAndAttributes, void* hTemplateFile) {

    assert(dwDesiredAccess == GENERIC_READ);
    assert(lpSecurityAttributes == NULL);
    assert(dwCreationDisposition == OPEN_EXISTING);

    FILE* f = fopen(lpFileName, "r");
    if (!f) {
        return INVALID_HANDLE_VALUE;
    }

    return f;
}

uint32_t GetFileSize_(void* hFile, uint32_t* lpFileSizeHigh) {
    assert(lpFileSizeHigh == NULL);

    long current_offset = ftell(hFile);
    fseek(hFile, 0, SEEK_END);
    long size = ftell(hFile);
    fseek(hFile, current_offset, SEEK_SET);
    return size;
}

int CloseHandle_(void* hObject) {
    fclose(hObject);
    return 0;
}

void GlobalMemoryStatus_(MEMORYSTATUS_* lpBuffer) {
    lpBuffer->dwTotalPhys = 64000000;
    lpBuffer->dwAvailPhys = 64000000; // pretend we have a whole 64mb of ram!
    lpBuffer->dwAvailPageFile = 0;
    lpBuffer->dwTotalVirtual = 0;
    lpBuffer->dwAvailVirtual = 0;
    lpBuffer->dwMemoryLoad = 0;
    lpBuffer->dwTotalPageFile = 0;
}

int GetCursorPos_(POINT_* lpPoint) {
    int x, y;
    gHarness_platform.GetMousePosition(&x, &y);
    lpPoint->x = x;
    lpPoint->y = y;
    return 0;
}

int ScreenToClient_(void* hWnd, POINT_* lpPoint) {
    // no-op, we assume the point is already relative to client
    return 0;
}

uint32_t timeGetTime_() {
    return gHarness_platform.GetTicks();
}

uint32_t GetCurrentDirectoryA_(uint32_t nBufferLength, char* lpBuffer) {
    char* ret = getcwd(lpBuffer, nBufferLength);
    if (ret == NULL) {
        return 0;
    }
    return strlen(lpBuffer);
}

int SetCurrentDirectoryA_(char* lpPathName) {
    int ret = chdir(lpPathName);
    // chdir returning zero means success, SetCurrentDirectory returning non-zero means success
    if (ret == 0) {
        return 1;
    } else {
        return 0;
    }
}

void* FindFirstFileA_(char* lpFileName, WIN32_FIND_DATAA_* lpFindFileData) {
    assert(strcmp(lpFileName, "*.???") == 0);

#if defined(_WIN32) || defined(_WIN64)
    WIN32_FIND_DATA fd;
    void* hFile = FindFirstFile(lpFileName, &fd);
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

int FindNextFileA_(void* hFindFile, WIN32_FIND_DATAA_* lpFindFileData) {
#if defined(_WIN32) || defined(_WIN64)
    WIN32_FIND_DATA fd;
    int result = FindNextFile(hFindFile, &fd);
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

int FindClose_(void* hFindFile) {
#if defined(_WIN32) || defined(_WIN64)
    return FindClose(hFindFile);
#else
    return closedir(hFindFile);

#endif
}

void* CreateWindowExA_(uint32_t dwExStyle, char* lpClassName, char* lpWindowName, uint32_t dwStyle, int X, int Y, int nWidth, int nHeight, void* hWndParent, void* hMenu, void* hInstance, void* lpParam) {
    return gHarness_platform.CreateWindowAndRenderer(lpWindowName, X, Y, nWidth, nHeight);
}

int SetWindowPos_(void* hWnd, void* hWndInsertAfter, int X, int Y, int cx, int cy, unsigned int uFlags) {
    return gHarness_platform.SetWindowPos(hWnd, X, Y, cx, cy);
}

int ShowCursor_(int bShow) {
    gHarness_platform.ShowCursor(bShow);
    return 0;
}

int SendMessageA_(void* hWnd, unsigned int Msg, unsigned int wParam, long lParam) {
    return 0;
}

int MessageBoxA_(void* hWnd, char* lpText, char* lpCaption, unsigned int uType) {
    return 0;
}

int DestroyWindow_(void* hWnd) {
    gHarness_platform.DestroyWindow(hWnd);
    return 0;
}

void ExitProcess_(unsigned int uExitCode) {
    exit(uExitCode);
}

void TranslateMessage_(MSG_* lpMsg) {
    // no-op
}

void DispatchMessageA_(MSG_* lpMsg) {
    // no-op
}

int PeekMessageA_(MSG_* lpMsg, void* hWnd, unsigned int wMsgFilterMin, unsigned int wMsgFilterMax, unsigned int wRemoveMsg) {
    return gHarness_platform.ProcessWindowMessages(lpMsg);
}

int GetMessageA_(MSG_* lpMsg, void* hWnd, unsigned int wMsgFilterMin, unsigned int wMsgFilterMax) {
    return gHarness_platform.ProcessWindowMessages(lpMsg);
}

void Sleep_(uint32_t dwMilliseconds) {
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
