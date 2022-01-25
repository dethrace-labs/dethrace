#include "brstdlib.h"
#include "harness/trace.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

// Global variables
// DOSPICK has "$Id: stdlib.c 2.4 1996/12/06 21:18:39 sam Exp $" for this. Do we want to implement these?

int BrMemCmp(void* s1, void* s2, size_t n) {
    return memcmp(s1, s2, n);
}

void* BrMemCpy(void* s1, void* s2, size_t n) {
    return memcpy(s1, s2, n);
}

void* BrMemSet(void* s, int c, size_t n) {
    return memset(s, c, n);
}

char* BrStrCat(char* s1, char* s2) {
    return strcat(s1, s2);
}

int BrStrCmp(char* s1, char* s2) {
    return strcmp(s1, s2);
}

int BrStrICmp(char* s1, char* s2) {
    // Windows is stricmp, while posix is strcasecmp
#ifdef _WIN32
    return stricmp(s1, s2);
#else
    return strcasecmp(s1, s2);
#endif
}

char* BrStrCpy(char* s1, const char* s2) {
    return strcpy(s1, s2);
}

br_size_t BrStrLen(char* s) {
    return strlen(s);
}

int BrStrNCmp(char* s1, char* s2, size_t n) {
    return strncmp(s1, s2, n);
}

int BrStrNICmp(char* s1, char* s2, size_t n) {
    // Windows is strnicmp, while posix is strncasecmp
#ifdef _WIN32
    return strnicmp(s1, s2, n);
#else
    return strncasecmp(s1, s2, n);
#endif
}

char* BrStrNCpy(char* s1, char* s2, size_t n) {
    return strncpy(s1, s2, n);
}

char* BrStrRChr(char* s1, char c) {
    return strrchr(s1, c);
}

void BrAbort(void) {
    abort();
}

char* BrGetEnv(char* name) {
    return getenv(name);
}

float BrStrToF(char* nptr, char** endptr) {
    return strtof(nptr, endptr);
}

double BrStrToD(char* nptr, char** endptr) {
    return strtod(nptr, endptr);
}

long BrStrToL(char* nptr, char** endptr, int base) {
    return strtol(nptr, endptr, base);
}

unsigned long BrStrToUL(char* nptr, char** endptr, int base) {
    return strtoul(nptr, endptr, base);
}

br_boolean BrIsAlpha(int c) {
    return isalpha(c);
}

br_boolean BrIsDigit(int c) {
    return isdigit(c);
}

br_boolean BrIsSpace(int c) {
    return isspace(c);
}

br_boolean BrIsPrint(int c) {
    return isprint(c);
}

br_int_32 BrVSprintf(char* buf, const char* fmt, va_list args) {
    return vsprintf(buf, fmt, args);
}

br_int_32 BrVSprintfN(char* buf, br_size_t buf_size, const char* fmt, va_list args) {
    // Most C libs have vsnprintf these days that can replace this.
    unsigned int n;
    char tmp[512];

    n = vsprintf(tmp, fmt, args);

    if (buf_size - 1 < n) {
        n = buf_size - 1;
    }

    strncpy(buf, tmp, n);
    buf[n] = '\0';
    return n;
}

br_int_32 BrVSScanf(char* buf, const char* fmt, va_list args) {
    return vsscanf(buf, fmt, args);
}
