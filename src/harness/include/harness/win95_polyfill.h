#ifndef _POLYFILL_H_
#define _POLYFILL_H_

#include "harness/compiler.h"
#include "win95_polyfill_defs.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// All functions have a "_" suffix to avoid collisions with <windows.h>-defined types

uint32_t GetFileAttributesA_(char* path);

int SetFileAttributesA_(char* lpFileName, uint32_t dwFileAttributes);

void* CreateFileA_(
    char* lpFileName,
    uint32_t dwDesiredAccess,
    uint32_t dwShareMode,
    void* lpSecurityAttributes,
    uint32_t dwCreationDisposition,
    uint32_t dwFlagsAndAttributes,
    void* hTemplateFile);

uint32_t GetFileSize_(
    void* hFile,
    uint32_t* lpFileSizeHigh);

int CloseHandle_(
    void* hObject);

void GlobalMemoryStatus_(MEMORYSTATUS_* lpBuffer);

int GetCursorPos_(POINT_* lpPoint);

int ScreenToClient_(void* hWnd, POINT_* lpPoint);

uint32_t timeGetTime_();

uint32_t GetCurrentDirectoryA_(uint32_t nBufferLength, char* lpBuffer);

int SetCurrentDirectoryA_(char* lpPathName);

uint32_t GetShortPathNameA_(char* lpszLongPath, char* lpszShortPath, uint32_t cchBuffer);

HANDLE_ FindFirstFileA_(char* lpFileName, WIN32_FIND_DATAA_* lpFindFileData);

int FindNextFileA_(HANDLE_ hFindFile, WIN32_FIND_DATAA_* lpFindFileData);

int FindClose_(HANDLE_ hFindFile);

void* CreateWindowExA_(uint32_t dwExStyle, char* lpClassName, char* lpWindowName, uint32_t dwStyle, int X, int Y, int nWidth, int nHeight, void* hWndParent, void* hMenu, void* hInstance, void* lpParam);

int SetWindowPos_(void* hWnd, void* hWndInsertAfter, int X, int Y, int cx, int cy, unsigned int uFlags);

int ShowCursor_(int bShow);

int SendMessageA_(void* hWnd, unsigned int Msg, unsigned int wParam, long lParam);

int MessageBoxA_(void* hWnd, char* lpText, char* lpCaption, unsigned int uType);

int DestroyWindow_(void* hWnd);

HARNESS_NORETURN void ExitProcess_(unsigned int uExitCode);

void TranslateMessage_(MSG_* lpMsg);

void DispatchMessageA_(MSG_* lpMsg);

int PeekMessageA_(MSG_* lpMsg, void* hWnd, unsigned int wMsgFilterMin, unsigned int wMsgFilterMax, unsigned int wRemoveMsg);

int GetMessageA_(MSG_* lpMsg, void* hWnd, unsigned int wMsgFilterMin, unsigned int wMsgFilterMax);

void Sleep_(uint32_t dwMilliseconds);

void DirectDraw_CreateSurface();

void DirectDrawDevice_SetPaletteEntries(PALETTEENTRY_* palette, int pFirst_colour, int pCount);

void DirectInputDevice_GetDeviceState(unsigned int keys, uint8_t* buffer);

void _splitpath_(char* path, char* drive, char* dir, char* fname, char* ext);

int _CrtDbgReport_(int reportType, const char* filename, int linenumber, const char* moduleName, const char* format, ...);

#endif
