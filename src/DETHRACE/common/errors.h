#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "br_types.h"
#include "dr_types.h"

void FatalError(int pStr_index, ...);

void NonFatalError(int pStr_index, ...);

void CloseDiagnostics();

void OpenDiagnostics();

void dr_dprintf(char* fmt_string, ...);

int DoErrorInterface(int pMisc_text_index);

#endif
