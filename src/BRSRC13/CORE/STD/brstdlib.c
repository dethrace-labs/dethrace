#include "brstdlib.h"
#include "harness.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

// Global variables
// DOSPICK has "$Id: stdlib.c 2.4 1996/12/06 21:18:39 sam Exp $" for this. Do we want to implement these?
char rscid[48];

// Offset: 9
// Size: 50
int BrMemCmp(void* s1, void* s2, size_t n) {
    return memcmp(s1, s2, n);
}

// Offset: 68
// Size: 50
void* BrMemCpy(void* s1, void* s2, size_t n) {
    memcpy(s1, s2, n);
}

// Offset: 127
// Size: 50
void* BrMemSet(void* s, int c, size_t n) {
    memset(s, c, n);
}

// Offset: 186
// Size: 47
char* BrStrCat(char* s1, char* s2) {
    return strcat(s1, s2);
}

// Offset: 242
// Size: 47
int BrStrCmp(char* s1, char* s2) {
    return strcmp(s1, s2);
}

// Offset: 299
// Size: 47
int BrStrICmp(char* s1, char* s2) {
    // Windows is stricmp, while posix is strcasecmp
#ifdef _WIN32
    return stricmp(s1, s2);
#else
    return strcasecmp(s1, s2);
#endif
}

// Offset: 355
// Size: 47
char* BrStrCpy(char* s1, const char* s2) {
    return strcpy(s1, s2);
}

// Offset: 411
// Size: 44
br_size_t BrStrLen(char* s) {
    return strlen(s);
}

// Offset: 465
// Size: 50
int BrStrNCmp(char* s1, char* s2, size_t n) {
    return strncmp(s1, s2, n);
}

// Offset: 526
// Size: 50
int BrStrNICmp(char* s1, char* s2, size_t n) {
    // Windows is strnicmp, while posix is strncasecmp
#ifdef _WIN32
    return strnicmp(s1, s2, n);
#else
    return strncasecmp(s1, s2, n);
#endif
}

// Offset: 586
// Size: 50
char* BrStrNCpy(char* s1, char* s2, size_t n) {
    return strncpy(s1, s2, n);
}

// Offset: 646
// Size: 49
char* BrStrRChr(char* s1, char c) {
    return strrchr(s1, c);
    NOT_IMPLEMENTED();
}

// Offset: 703
// Size: 35
void BrAbort(void) {
    abort();
    NOT_IMPLEMENTED();
}

// Offset: 747
// Size: 44
char* BrGetEnv(char* name) {
    return getenv(name);
    NOT_IMPLEMENTED();
}

// Offset: 800
// Size: 58
float BrStrToF(char* nptr, char** endptr) {
    return strtof(nptr, endptr);
    NOT_IMPLEMENTED();
}

// Offset: 867
// Size: 67
double BrStrToD(char* nptr, char** endptr) {
    return strtod(nptr, endptr);
    NOT_IMPLEMENTED();
}

// Offset: 943
// Size: 50
long BrStrToL(char* nptr, char** endptr, int base) {
    return strtol(nptr, endptr, base);
    NOT_IMPLEMENTED();
}

// Offset: 1003
// Size: 50
unsigned long BrStrToUL(char* nptr, char** endptr, int base) {
    return strtoul(nptr, endptr, base);
    NOT_IMPLEMENTED();
}

// Offset: 1063
// Size: 59
br_boolean BrIsAlpha(int c) {
    return isalpha(c);
    NOT_IMPLEMENTED();
}

// Offset: 1132
// Size: 59
br_boolean BrIsDigit(int c) {
    return isdigit(c);
    NOT_IMPLEMENTED();
}

// Offset: 1201
// Size: 59
br_boolean BrIsSpace(int c) {
    return isspace(c);
    NOT_IMPLEMENTED();
}

// Offset: 1270
// Size: 59
br_boolean BrIsPrint(int c) {
    return isprint(c);
    NOT_IMPLEMENTED();
}

// Offset: 1340
// Size: 50
br_int_32 BrVSprintf(char* buf, const char* fmt, va_list args) {
    return vsprintf(buf, fmt, args);
    NOT_IMPLEMENTED();
}

// Offset: 1402
// Size: 101
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
}

// Offset: 1513
// Size: 50
br_int_32 BrVSScanf(char* buf, const char* fmt, va_list args) {
    return vsscanf(buf, fmt, args);
    NOT_IMPLEMENTED();
}
