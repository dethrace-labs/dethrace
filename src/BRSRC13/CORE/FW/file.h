#ifndef _FILE_H_
#define _FILE_H_

#include "brender/br_types.h"

void _BrFileFree(void* res, br_uint_8 res_class, br_size_t size);

br_uint_32 BrFileAttributes(void);

void* BrFileOpenRead(char* name, br_size_t n_magics, br_mode_test_cbfn* mode_test, int* mode_result);

void* BrFileOpenWrite(char* name, int mode);

void BrFileClose(void* f);

int BrFileEof(void* f);

int BrFileGetChar(void* f);

void BrFilePutChar(int c, void* f);

int BrFileRead(void* buf, int size, int n, void* f);

int BrFileWrite(void* buf, int size, int n, void* f);

int BrFileGetLine(char* buf, br_size_t buf_len, void* f);

void BrFilePutLine(char* buf, void* f);

void BrFileAdvance(long count, void* f);

int BrFilePrintf(void* f, char* fmt, ...);

#endif
