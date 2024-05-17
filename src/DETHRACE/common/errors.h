#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "dr_types.h"
#include "harness/compiler.h"

extern char* gError_messages[126];
extern int gError_code;
extern char* gPalette_copy__errors;    // suffix added to avoid duplicate symbol
extern int gPixel_buffer_size__errors; // suffix added to avoid duplicate symbol
extern int gMouse_was_started__errors; // suffix added to avoid duplicate symbol
extern char* gPixels_copy__errors;     // suffix added to avoid duplicate symbol

HARNESS_NORETURN void FatalError(int pStr_index, ...);

void NonFatalError(int pStr_index, ...);

void CloseDiagnostics(void);

void OpenDiagnostics(void);

void dr_dprintf(char* fmt_string, ...);

int DoErrorInterface(int pMisc_text_index);

#endif
