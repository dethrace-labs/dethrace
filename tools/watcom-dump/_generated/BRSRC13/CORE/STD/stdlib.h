#ifndef _STDLIB_H_
#define _STDLIB_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 9
// Size: 50
int BrMemCmp(void *s1, void *s2, size_t n);

// Offset: 68
// Size: 50
void* BrMemCpy(void *s1, void *s2, size_t n);

// Offset: 127
// Size: 50
void* BrMemSet(void *s, int c, size_t n);

// Offset: 186
// Size: 47
char* BrStrCat(char *s1, char *s2);

// Offset: 242
// Size: 47
int BrStrCmp(char *s1, char *s2);

// Offset: 299
// Size: 47
int BrStrICmp(char *s1, char *s2);

// Offset: 355
// Size: 47
char* BrStrCpy(char *s1, char *s2);

// Offset: 411
// Size: 44
br_size_t BrStrLen(char *s);

// Offset: 465
// Size: 50
int BrStrNCmp(char *s1, char *s2, size_t n);

// Offset: 526
// Size: 50
int BrStrNICmp(char *s1, char *s2, size_t n);

// Offset: 586
// Size: 50
char* BrStrNCpy(char *s1, char *s2, size_t n);

// Offset: 646
// Size: 49
char* BrStrRChr(char *s1, char c);

// Offset: 703
// Size: 35
void BrAbort();

// Offset: 747
// Size: 44
char* BrGetEnv(char *name);

// Offset: 800
// Size: 58
float BrStrToF(char *nptr, char **endptr);

// Offset: 867
// Size: 67
double BrStrToD(char *nptr, char **endptr);

// Offset: 943
// Size: 50
long BrStrToL(char *nptr, char **endptr, int base);

// Offset: 1003
// Size: 50
unsigned long BrStrToUL(char *nptr, char **endptr, int base);

// Offset: 1063
// Size: 59
br_boolean BrIsAlpha(int c);

// Offset: 1132
// Size: 59
br_boolean BrIsDigit(int c);

// Offset: 1201
// Size: 59
br_boolean BrIsSpace(int c);

// Offset: 1270
// Size: 59
br_boolean BrIsPrint(int c);

// Offset: 1340
// Size: 50
br_int_32 BrVSprintf(char *buf, char *fmt, char **args);

// Offset: 1402
// Size: 101
br_int_32 BrVSprintfN(char *buf, br_size_t buf_size, char *fmt, char **args);

// Offset: 1513
// Size: 50
br_int_32 BrVSScanf(char *buf, char *fmt, char **args);

#endif
