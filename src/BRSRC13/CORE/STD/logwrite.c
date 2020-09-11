#include "logwrite.h"
#include "harness.h"

char rscid[50];

// IDA: int __cdecl BrLogWrite(void *buffer, br_size_t s, br_size_t n)
int BrLogWrite(void* buffer, br_size_t s, br_size_t n) {
    LOG_TRACE("(%p, %d, %d)", buffer, s, n);
    NOT_IMPLEMENTED();
}
