#ifndef _POLYFILL_H_
#define _POLYFILL_H_

#include "harness/compiler.h"
#include "win95_polyfill_defs.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

DWORD GetFileAttributesA_(char* path);

BOOL SetFileAttributesA_(char* lpFileName, DWORD dwFileAttributes);

HANDLE CreateFileA_(
    char* lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    void* lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);

DWORD GetFileSize_(
    HANDLE hFile,
    DWORD* lpFileSizeHigh);

BOOL CloseHandle_(
    HANDLE hObject);

void GlobalMemoryStatus_(MEMORYSTATUS* lpBuffer);

BOOL GetCursorPos_(LPPOINT lpPoint);

BOOL ScreenToClient_(HWND hWnd, LPPOINT lpPoint);

DWORD timeGetTime_();

DWORD GetCurrentDirectoryA_(DWORD nBufferLength, char* lpBuffer);

BOOL SetCurrentDirectoryA_(char* lpPathName);

DWORD GetShortPathNameA_(char* lpszLongPath, char* lpszShortPath, DWORD cchBuffer);

HANDLE FindFirstFileA_(char* lpFileName, WIN32_FIND_DATAA_* lpFindFileData);

BOOL FindNextFileA_(HANDLE hFindFile, WIN32_FIND_DATAA_* lpFindFileData);

BOOL FindClose_(HANDLE hFindFile);

HWND CreateWindowExA_(DWORD dwExStyle, char* lpClassName, char* lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, void* hMenu, void* hInstance, LPVOID lpParam);

BOOL SetWindowPos_(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

int ShowCursor_(BOOL bShow);

int SendMessageA_(HWND hWnd, UINT Msg, UINT wParam, long lParam);

int MessageBoxA_(HWND hWnd, char* lpText, char* lpCaption, UINT uType);

BOOL DestroyWindow_(HWND hWnd);

HARNESS_NORETURN void ExitProcess_(UINT uExitCode);

void TranslateMessage_(MSG* lpMsg);

void DispatchMessageA_(MSG* lpMsg);

BOOL PeekMessageA_(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

BOOL GetMessageA_(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);

void Sleep_(DWORD dwMilliseconds);

void DirectDraw_CreateSurface();

void DirectDrawDevice_SetPaletteEntries(PALETTEENTRY_* palette, int pFirst_colour, int pCount);

void DirectInputDevice_GetDeviceState(unsigned int keys, uint8_t* buffer);

// renamed from _splitpath to avoid conflict with stdlib
void _splitpath_(char* path, char* drive, char* dir, char* fname, char* ext);

int _CrtDbgReport_(int reportType, const char* filename, int linenumber, const char* moduleName, const char* format, ...);

#endif
