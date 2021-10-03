#ifndef _STDDIAG_H_
#define _STDDIAG_H_

#include "brender/br_types.h"

extern br_diaghandler BrStdioDiagHandler;
extern br_diaghandler* _BrDefaultDiagHandler;

void BrStdioWarning(char* message);

void BrStdioFailure(char* message);

#endif
