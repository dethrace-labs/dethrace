#include "dr_types.h"
#include "br_types.h"

extern br_filesystem *_BrDefaultFilesystem;

// Offset: 18
// Size: 40
br_uint_32 BrStdioAttributes();

// Offset: 74
// Size: 595
void* BrStdioOpenRead(char *name, br_size_t n_magics, br_mode_test_cbfn *identify, int *mode_result);

// Offset: 686
// Size: 82
void* BrStdioOpenWrite(char *name, int mode);

// Offset: 781
// Size: 38
void BrStdioClose(void *f);

// Offset: 830
// Size: 45
int BrStdioEof(void *f);

// Offset: 890
// Size: 117
int BrStdioGetChar(void *f);

// Offset: 1022
// Size: 41
void BrStdioPutChar(int c, void *f);

// Offset: 1075
// Size: 53
br_size_t BrStdioRead(void *buf, br_size_t size, unsigned int n, void *f);

// Offset: 1141
// Size: 53
br_size_t BrStdioWrite(void *buf, br_size_t size, unsigned int n, void *f);

// Offset: 1209
// Size: 141
br_size_t BrStdioGetLine(char *buf, br_size_t buf_len, void *f);

// Offset: 1365
// Size: 54
void BrStdioPutLine(char *buf, void *f);

// Offset: 1434
// Size: 46
void BrStdioAdvance(br_size_t count, void *f);

