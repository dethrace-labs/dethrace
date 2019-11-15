#include "scratch.h"

char scratchString[512];
char rscid[49];

// Offset: 18
// Size: 153
void* BrScratchAllocate(br_size_t size) {
}

// Offset: 185
// Size: 72
void BrScratchFree(void *scratch) {
}

// Offset: 272
// Size: 96
void BrScratchFlush() {
}

// Offset: 385
// Size: 41
br_size_t BrScratchInquire() {
}

// Offset: 442
// Size: 40
char* BrScratchString() {
}

// Offset: 502
// Size: 40
br_size_t BrScratchStringSize() {
}

