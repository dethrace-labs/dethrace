#ifndef _DRFILE_H_
#define _DRFILE_H_

#include "dr_types.h"

extern br_filesystem gFilesystem;
extern br_filesystem* gOld_file_system;

void* DRStdioOpenRead(char* name, br_size_t n_magics, br_mode_test_cbfn* identify, int* mode_result);

void* DRStdioOpenWrite(char* name, int mode);

void DRStdioClose(void* f);

br_size_t DRStdioRead(void* buf, br_size_t size, unsigned int n, void* f);

br_size_t DRStdioWrite(void* buf, br_size_t size, unsigned int n, void* f);

void InstallDRFileCalls(void);

#endif
