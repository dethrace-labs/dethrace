#include "ssdx.h"
#include "errors.h"
#include "harness/hooks.h"

int gSSDX_windowed;
HWND gSSDX_hwnd;

int SSDXStart(HWND hWnd, int windowed, int flags) {
    int res = 0;
    dr_dprintf("SSDXStart(): START...");
    if (windowed) {
        gSSDX_windowed = 1;
    }
    gSSDX_hwnd = hWnd;

    // Not required

    // if ((flags & 1) != 0) {
    //     dr_dprintf("Calling DirectDrawCreate()...");
    //     res = DirectDrawCreate(0, &gDirect_draw, 0);
    //     if (!res) {
    //         dr_dprintf("Calling SetCooperativeLevel()...");
    //         if (gSSDX_windowed) {
    //             res = gDirect_draw->lpVtbl->SetCooperativeLevel(gDirect_draw, gSSDX_hwnd, 8);
    //         } else {
    //             res = gDirect_draw->lpVtbl->SetCooperativeLevel(gDirect_draw, gSSDX_hwnd, 83);
    //         }
    //     }
    // }
    // if (!res && (flags & 2) != 0) {
    //     dr_dprintf("Calling DirectSoundCreate()...");
    //     res = DirectSoundCreate(0, &gDirect_sound, 0);
    // }
    // if (res) {
    //     SSDXHandleError(res);
    // }
    dr_dprintf("SSDXStart(): END.");
    return res;
}

int SSDXInitDirectDraw(int width, int height, int* row_bytes) {
    DirectDraw_CreateSurface(width, height);
    *row_bytes = width;
    return 0;
}

void SSDXRelease() {}

void SSDXGetWindowRect(HWND hWnd) {
    // none of this is required

    // GetClientRect(hWnd, &gSSDX_rect);
    // ClientToScreen(hWnd, (LPPOINT)&gSSDX_rect);
    // ClientToScreen(hWnd, (LPPOINT)&gSSDX_rect.right);
    // dr_dprintf("New window rect: (%d,%d)(%d,%d)", gSSDX_rect.left, gSSDX_rect.top, gSSDX_rect.right, gSSDX_rect.bottom);
}

void SSDXHandleError(int error) {
    // no-op
}

void SSDXSetPaleeteEntries(PALETTEENTRY_* palette, int pFirst_color, int pCount) {
    DirectDrawDevice_SetPaletteEntries(palette, pFirst_color, pCount);
}
