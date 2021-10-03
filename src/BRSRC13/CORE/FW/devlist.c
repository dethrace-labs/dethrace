#include "devlist.h"
#include "harness/trace.h"

// IDA: br_error __cdecl AddRequestedDrivers()
br_error AddRequestedDrivers() {
    char devstr[256];
    static br_boolean bAlreadyDone;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall devAdd@<EAX>(br_device **pdev@<EAX>, br_device_begin_fn *dev_begin@<EDX>, char *args@<EBX>, br_image *image@<ECX>)
br_error devAdd(br_device** pdev, br_device_begin_fn* dev_begin, char* args, br_image* image) {
    int i;
    br_device* dev;
    br_open_device* new_slots;
    LOG_TRACE("(%p, %p, \"%s\", %p)", pdev, dev_begin, args, image);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevAdd(br_device **pdev, char *image, char *args)
br_error BrDevAdd(br_device** pdev, char* image, char* args) {
    br_image* dev_image;
    br_device_begin_fn* dev_begin;
    LOG_TRACE("(%p, \"%s\", \"%s\")", pdev, image, args);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevAddStatic(br_device **pdev, br_device_begin_fn *dev_begin, char *args)
br_error BrDevAddStatic(br_device** pdev, br_device_begin_fn* dev_begin, char* args) {
    LOG_TRACE("(%p, %p, \"%s\")", pdev, dev_begin, args);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevCheckAdd(br_device **pdev, char *name, char *args)
br_error BrDevCheckAdd(br_device** pdev, char* name, char* args) {
    LOG_TRACE("(%p, \"%s\", \"%s\")", pdev, name, args);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevAddConfig(char *config)
br_error BrDevAddConfig(char* config) {
    char* end;
    char* dev;
    char* arg;
    char tmp[512];
    int n;
    LOG_TRACE("(\"%s\")", config);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevRemove(br_device *dev)
br_error BrDevRemove(br_device* dev) {
    int i;
    LOG_TRACE("(%p)", dev);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevFind(br_device **pdev, char *pattern)
br_error BrDevFind(br_device** pdev, char* pattern) {
    int i;
    int c;
    LOG_TRACE("(%p, \"%s\")", pdev, pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevFindMany(br_device **devices, br_int_32 *ndevices, br_int_32 max_devices, char *pattern)
br_error BrDevFindMany(br_device** devices, br_int_32* ndevices, br_int_32 max_devices, char* pattern) {
    int i;
    int c;
    LOG_TRACE("(%p, %p, %d, \"%s\")", devices, ndevices, max_devices, pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevCount(br_int_32 *ndevices, char *pattern)
br_error BrDevCount(br_int_32* ndevices, char* pattern) {
    int i;
    int c;
    LOG_TRACE("(%p, \"%s\")", ndevices, pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevContainedFind(br_object **ph, br_token type, char *pattern, br_token_value *tv)
br_error BrDevContainedFind(br_object** ph, br_token type, char* pattern, br_token_value* tv) {
    int i;
    LOG_TRACE("(%p, %d, \"%s\", %p)", ph, type, pattern, tv);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevContainedFindMany(br_object **objects, br_int_32 max_objects, br_int_32 *pnum_objects, br_token type, char *pattern, br_token_value *tv)
br_error BrDevContainedFindMany(br_object** objects, br_int_32 max_objects, br_int_32* pnum_objects, br_token type, char* pattern, br_token_value* tv) {
    int i;
    br_int_32 n;
    br_int_32 total;
    br_error r;
    LOG_TRACE("(%p, %d, %p, %d, \"%s\", %p)", objects, max_objects, pnum_objects, type, pattern, tv);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevContainedCount(br_int_32 *pcount, br_token type, char *pattern, br_token_value *tv)
br_error BrDevContainedCount(br_int_32* pcount, br_token type, char* pattern, br_token_value* tv) {
    int i;
    br_int_32 n;
    br_int_32 total;
    br_error r;
    LOG_TRACE("(%p, %d, \"%s\", %p)", pcount, type, pattern, tv);
    NOT_IMPLEMENTED();
}
