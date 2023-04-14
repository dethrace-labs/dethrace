#ifndef _POLYFILL_H_
#define _POLYFILL_H_

#include "harness/compiler.h"
#include "win95_polyfill_defs.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

DWORD GetFileAttributesA(char* path);

BOOL SetFileAttributesA(char* lpFileName, DWORD dwFileAttributes);

HANDLE CreateFileA(
    char* lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    void* lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);

DWORD GetFileSize(
    HANDLE hFile,
    DWORD* lpFileSizeHigh);

BOOL CloseHandle(
    HANDLE hObject);

void GlobalMemoryStatus(MEMORYSTATUS* lpBuffer);

BOOL GetCursorPos(LPPOINT lpPoint);

BOOL ScreenToClient(HWND hWnd, LPPOINT lpPoint);

DWORD timeGetTime();

DWORD GetCurrentDirectoryA(DWORD nBufferLength, char* lpBuffer);

BOOL SetCurrentDirectoryA(char* lpPathName);

DWORD GetShortPathNameA(char* lpszLongPath, char* lpszShortPath, DWORD cchBuffer);

HANDLE FindFirstFileA(char* lpFileName, WIN32_FIND_DATAA* lpFindFileData);

BOOL FindNextFileA(HANDLE hFindFile, WIN32_FIND_DATAA* lpFindFileData);

BOOL FindClose(HANDLE hFindFile);

HWND CreateWindowExA(DWORD dwExStyle, char* lpClassName, char* lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, void* hMenu, void* hInstance, LPVOID lpParam);

BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

int ShowCursor(BOOL bShow);

int SendMessageA(HWND hWnd, UINT Msg, UINT wParam, long lParam);

int MessageBoxA(HWND hWnd, char* lpText, char* lpCaption, UINT uType);

BOOL DestroyWindow(HWND hWnd);

HARNESS_NORETURN void ExitProcess(UINT uExitCode);

void TranslateMessage(MSG* lpMsg);

void DispatchMessageA(MSG* lpMsg);

BOOL PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

BOOL GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);

void Sleep(DWORD dwMilliseconds);

void DirectDraw_CreateSurface();

void DirectDrawDevice_SetPaletteEntries(PALETTEENTRY* palette, int pFirst_colour, int pCount);

void DirectInputDevice_GetDeviceState(unsigned int keys, uint8_t* buffer);

void _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext);

int _CrtDbgReport(int reportType, const char* filename, int linenumber, const char* moduleName, const char* format, ...);

#endif
