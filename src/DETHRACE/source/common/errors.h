#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 207
void FatalError(int pStr_index, ...);

// Offset: 208
// Size: 172
void NonFatalError(int pStr_index, ...);

// Offset: 380
// Size: 34
void CloseDiagnostics();

// Offset: 416
// Size: 34
void OpenDiagnostics();

// Offset: 452
// Size: 34
// void dprintf(char *fmt_string);

// Offset: 488
// Size: 57
// EAX: pMisc_text_index
int DoErrorInterface(int pMisc_text_index);

#endif
