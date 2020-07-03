#include "scratch.h"
#include "harness.h"

char scratchString[512];
char rscid[49];

// Offset: 18
// Size: 153
void* BrScratchAllocate(br_size_t size) {
    NOT_IMPLEMENTED();
}

// Offset: 185
// Size: 72
void BrScratchFree(void* scratch) {
    NOT_IMPLEMENTED();
}

// Offset: 272
// Size: 96
void BrScratchFlush() {
    NOT_IMPLEMENTED();
}

// Offset: 385
// Size: 41
br_size_t BrScratchInquire() {
    NOT_IMPLEMENTED();
}

// Offset: 442
// Size: 40
char* BrScratchString() {
    NOT_IMPLEMENTED();
}

// Offset: 502
// Size: 40
br_size_t BrScratchStringSize() {
    NOT_IMPLEMENTED();
}
