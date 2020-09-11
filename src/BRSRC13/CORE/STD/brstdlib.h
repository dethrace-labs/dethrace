/*
 * Renamed from stdlib.h to avoid collisions with c std lib
*/

#ifndef _BRSTDLIB_H_
#define _BRSTDLIB_H_

#include "br_types.h"

#include <stdarg.h>
#include <stdlib.h>

int BrMemCmp(void* s1, void* s2, size_t n);

void* BrMemCpy(void* s1, void* s2, size_t n);

void* BrMemSet(void* s, int c, size_t n);

char* BrStrCat(char* s1, char* s2);

int BrStrCmp(char* s1, char* s2);

int BrStrICmp(char* s1, char* s2);

char* BrStrCpy(char* s1, const char* s2);

br_size_t BrStrLen(char* s);

int BrStrNCmp(char* s1, char* s2, size_t n);

int BrStrNICmp(char* s1, char* s2, size_t n);

char* BrStrNCpy(char* s1, char* s2, size_t n);

char* BrStrRChr(char* s1, char c);

void BrAbort(void);

char* BrGetEnv(char* name);

float BrStrToF(char* nptr, char** endptr);

double BrStrToD(char* nptr, char** endptr);

long BrStrToL(char* nptr, char** endptr, int base);

unsigned long BrStrToUL(char* nptr, char** endptr, int base);

br_boolean BrIsAlpha(int c);

br_boolean BrIsDigit(int c);

br_boolean BrIsSpace(int c);

br_boolean BrIsPrint(int c);

br_int_32 BrVSprintf(char* buf, const char* fmt, va_list args);

br_int_32 BrVSprintfN(char* buf, br_size_t buf_size, const char* fmt, va_list args);

br_int_32 BrVSScanf(char* buf, const char* fmt, va_list args);

#endif
