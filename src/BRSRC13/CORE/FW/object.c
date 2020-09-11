#include "object.h"
#include "harness.h"

char rscid[48];

br_error _M_br_object_query(br_object* self, br_uint_32* pvalue, br_token t) {
    NOT_IMPLEMENTED();
}

br_error _M_br_object_queryBuffer(br_object* self, br_uint_32* pvalue, br_uint_32* buffer, br_size_t buffer_size, br_token t) {
    NOT_IMPLEMENTED();
}

br_error _M_br_object_queryMany(br_object* self, br_token_value* tv, void* extra, br_size_t extra_size, br_int_32* pcount) {
    NOT_IMPLEMENTED();
}

br_error _M_br_object_queryManySize(br_object* self, br_size_t* pextra_size, br_token_value* tv) {
    NOT_IMPLEMENTED();
}

br_error _M_br_object_queryAll(br_object* self, br_token_value* buffer, br_size_t buffer_size) {
    NOT_IMPLEMENTED();
}

br_error _M_br_object_queryAllSize(br_object* self, br_size_t* psize) {
    NOT_IMPLEMENTED();
}

void _BrObjectFree(void* res, br_uint_8 res_class, br_size_t size) {
    br_object* o;
    NOT_IMPLEMENTED();
}
