#ifndef SSDX_H
#define SSDX_H

#include "harness/win95_polyfill.h"

// All we know about these functions are two names - "SSDXStart" and "SSDXLockAttachedSurface".
// SSDX stands for Stainless Software Direct X ?

int SSDXStart(HWND hWnd, int windowed, int flags);
int SSDXInitDirectDraw(int width, int height, int* row_bytes);
void SSDXRelease();
void SSDXLockAttachedSurface();
void SSDXGetWindowRect(HWND hWnd);
void SSDXHandleError(int error);

void SSDXSetPaleeteEntries(PALETTEENTRY* palette, int pFirst_color, int pCount);

#endif
