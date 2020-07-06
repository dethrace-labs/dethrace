#include "loader.h"
#include "harness.h"

char rscid[49];

// Offset: 10
// Size: 2401
// EAX: name
br_image* ImageLoad(char* name) {
    void* fh;
    int mode;
    msdos_header dos_header;
    coff_header coff_header;
    nt_optional_header nt_header;
    section_header section_header;
    br_uint_32 pe;
    br_image* img;
    br_uint_8* arena_base;
    int arena_size;
    int arena_align;
    int i;
    br_uint_32 offset;
    unsigned int n;
    NOT_IMPLEMENTED();
}
