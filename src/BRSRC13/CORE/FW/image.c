#include "image.h"
#include "harness_trace.h"


// IDA: br_boolean __cdecl BrImageAdd(br_image *img)
br_boolean BrImageAdd(br_image* img) {
    STUB();
}

// IDA: br_boolean __cdecl BrImageRemove(br_image *img)
br_boolean BrImageRemove(br_image* img) {
    LOG_TRACE("(%p)", img);
    NOT_IMPLEMENTED();
}

// IDA: br_image* __cdecl BrImageFind(char *pattern)
br_image* BrImageFind(char* pattern) {
    char* c;
    br_image* img;
    LOG_TRACE("(\"%s\")", pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_image* __usercall imageLoadHost@<EAX>(char *name@<EAX>)
br_image* imageLoadHost(char* name) {
    br_image* img;
    void* host_image;
    LOG_TRACE("(\"%s\")", name);
    NOT_IMPLEMENTED();
}

// IDA: br_image* __cdecl BrImageReference(char *name)
br_image* BrImageReference(char* name) {
    char* suffix;
    char* scratch;
    br_image* img;
    LOG_TRACE("(\"%s\")", name);
    NOT_IMPLEMENTED();
}

// IDA: void* __usercall imageLookupName@<EAX>(br_image *img@<EAX>, char *name@<EDX>, br_uint_32 hint@<EBX>)
void* imageLookupName(br_image* img, char* name, br_uint_32 hint) {
    int c;
    int limit;
    int base;
    LOG_TRACE("(%p, \"%s\", %d)", img, name, hint);
    NOT_IMPLEMENTED();
}

// IDA: void* __cdecl BrImageLookupName(br_image *img, char *name, br_uint_32 hint)
void* BrImageLookupName(br_image* img, char* name, br_uint_32 hint) {
    char* scratch;
    void* p;
    LOG_TRACE("(%p, \"%s\", %d)", img, name, hint);
    NOT_IMPLEMENTED();
}

// IDA: void* __cdecl BrImageLookupOrdinal(br_image *img, br_uint_32 ordinal)
void* BrImageLookupOrdinal(br_image* img, br_uint_32 ordinal) {
    LOG_TRACE("(%p, %d)", img, ordinal);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrImageDereference(br_image *image)
void BrImageDereference(br_image* image) {
    LOG_TRACE("(%p)", image);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrImageFree(br_image *image)
void BrImageFree(br_image* image) {
    int i;
    LOG_TRACE("(%p)", image);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl _BrImageFree(void *res, br_uint_8 res_class, br_size_t size)
void _BrImageFree(void* res, br_uint_8 res_class, br_size_t size) {
    LOG_TRACE("(%p, %d, %d)", res, res_class, size);
    NOT_IMPLEMENTED();
}
