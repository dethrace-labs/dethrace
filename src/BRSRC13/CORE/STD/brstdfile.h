#ifndef _BRSTDFILE_H_
#define _BRSTDFILE_H_

#include "brender/br_types.h"

extern br_filesystem* _BrDefaultFilesystem;

br_uint_32 BrStdioAttributes();

void* BrStdioOpenRead(char* name, br_size_t n_magics, br_mode_test_cbfn* identify, int* mode_result);

void* BrStdioOpenWrite(char* name, int mode);

void BrStdioClose(void* f);

int BrStdioEof(void* f);

int BrStdioGetChar(void* f);

void BrStdioPutChar(int c, void* f);

br_size_t BrStdioRead(void* buf, br_size_t size, unsigned int n, void* f);

br_size_t BrStdioWrite(void* buf, br_size_t size, unsigned int n, void* f);

br_size_t BrStdioGetLine(char* buf, br_size_t buf_len, void* f);

void BrStdioPutLine(char* buf, void* f);

void BrStdioAdvance(br_size_t count, void* f);

#endif
