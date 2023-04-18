#ifndef SSDX_H
#define SSDX_H

#include "harness/win95_polyfill.h"

// All we know about these functions are two names - "SSDXStart" and "SSDXLockAttachedSurface".
// SSDX stands for Stainless Software Direct X ?

int SSDXStart(void* hWnd, int windowed, int flags);
int SSDXInitDirectDraw(int width, int height, int* row_bytes);
void SSDXRelease();
void SSDXLockAttachedSurface();
void SSDXGetWindowRect(void* hWnd);
void SSDXHandleError(int error);

void SSDXSetPaleeteEntries(PALETTEENTRY_* palette, int pFirst_color, int pCount);

#endif
