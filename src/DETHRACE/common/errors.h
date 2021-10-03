#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern char* gError_messages[126];
extern int gError_code;
extern char* gPalette_copy__errors; // suffix added to avoid duplicate symbol
extern int gPixel_buffer_size__errors; // suffix added to avoid duplicate symbol
extern int gMouse_was_started__errors; // suffix added to avoid duplicate symbol
extern char* gPixels_copy__errors; // suffix added to avoid duplicate symbol

void FatalError(int pStr_index, ...);

void NonFatalError(int pStr_index, ...);

void CloseDiagnostics();

void OpenDiagnostics();

void dr_dprintf(char* fmt_string, ...);

int DoErrorInterface(int pMisc_text_index);

#endif
