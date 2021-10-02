#include "loader.h"
#include "harness/trace.h"

// IDA: br_image* __usercall ImageLoad@<EAX>(char *name@<EAX>)
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
    LOG_TRACE("(\"%s\")", name);
    NOT_IMPLEMENTED();
}
