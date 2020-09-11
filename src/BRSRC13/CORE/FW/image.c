#include "image.h"
#include "harness.h"

char rscid[50];

br_boolean BrImageAdd(br_image* img) {
    STUB();
}

br_boolean BrImageRemove(br_image* img) {
    NOT_IMPLEMENTED();
}

br_image* BrImageFind(char* pattern) {
    char* c;
    br_image* img;
    NOT_IMPLEMENTED();
}

br_image* imageLoadHost(char* name) {
    br_image* img;
    void* host_image;
    NOT_IMPLEMENTED();
}

br_image* BrImageReference(char* name) {
    char* suffix;
    char* scratch;
    br_image* img;
    NOT_IMPLEMENTED();
}

void* imageLookupName(br_image* img, char* name, br_uint_32 hint) {
    int c;
    int limit;
    int base;
    NOT_IMPLEMENTED();
}

void* BrImageLookupName(br_image* img, char* name, br_uint_32 hint) {
    char* scratch;
    void* p;
    NOT_IMPLEMENTED();
}

void* BrImageLookupOrdinal(br_image* img, br_uint_32 ordinal) {
    NOT_IMPLEMENTED();
}

void BrImageDereference(br_image* image) {
    NOT_IMPLEMENTED();
}

void BrImageFree(br_image* image) {
    int i;
    NOT_IMPLEMENTED();
}

void _BrImageFree(void* res, br_uint_8 res_class, br_size_t size) {
    NOT_IMPLEMENTED();
}
