#include "devlist.h"
#include "harness.h"

char rscid[53];

br_error AddRequestedDrivers() {
    char devstr[256];
    static br_boolean bAlreadyDone;
    NOT_IMPLEMENTED();
}

br_error devAdd(br_device** pdev, br_device_begin_fn* dev_begin, char* args, br_image* image) {
    int i;
    br_device* dev;
    br_open_device* new_slots;
    NOT_IMPLEMENTED();
}

br_error BrDevAdd(br_device** pdev, char* image, char* args) {
    br_image* dev_image;
    br_device_begin_fn* dev_begin;
    NOT_IMPLEMENTED();
}

br_error BrDevAddStatic(br_device** pdev, br_device_begin_fn* dev_begin, char* args) {
    NOT_IMPLEMENTED();
}

br_error BrDevCheckAdd(br_device** pdev, char* name, char* args) {
    NOT_IMPLEMENTED();
}

br_error BrDevAddConfig(char* config) {
    char* end;
    char* dev;
    char* arg;
    char tmp[512];
    int n;
    NOT_IMPLEMENTED();
}

br_error BrDevRemove(br_device* dev) {
    int i;
    NOT_IMPLEMENTED();
}

br_error BrDevFind(br_device** pdev, char* pattern) {
    int i;
    int c;
    NOT_IMPLEMENTED();
}

br_error BrDevFindMany(br_device** devices, br_int_32* ndevices, br_int_32 max_devices, char* pattern) {
    int i;
    int c;
    NOT_IMPLEMENTED();
}

br_error BrDevCount(br_int_32* ndevices, char* pattern) {
    int i;
    int c;
    NOT_IMPLEMENTED();
}

br_error BrDevContainedFind(br_object** ph, br_token type, char* pattern, br_token_value* tv) {
    int i;
    NOT_IMPLEMENTED();
}

br_error BrDevContainedFindMany(br_object** objects, br_int_32 max_objects, br_int_32* pnum_objects, br_token type, char* pattern, br_token_value* tv) {
    int i;
    br_int_32 n;
    br_int_32 total;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error BrDevContainedCount(br_int_32* pcount, br_token type, char* pattern, br_token_value* tv) {
    int i;
    br_int_32 n;
    br_int_32 total;
    br_error r;
    NOT_IMPLEMENTED();
}
