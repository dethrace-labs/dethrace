#include "tokenval.h"
#include "harness.h"
#include "resource.h"

char rscid[50];

br_tv_template* BrTVTemplateAllocate(void* res, br_tv_template_entry* entries, int n_entries) {
    br_tv_template* t;
    LOG_TRACE("(%p, %p, %d)", res, entries, n_entries);

    t = BrResAllocate(res, sizeof(br_tv_template), BR_MEMORY_TOKEN_TEMPLATE);
    t->res = t;
    t->entries = entries;
    t->n_entries = n_entries;
    return t;
}

void BrTVTemplateFree(br_tv_template* t) {
    NOT_IMPLEMENTED();
}

void templateResolveNames(br_tv_template* template) {
    br_tv_template_entry* tp;
    int n;
    NOT_IMPLEMENTED();
}

void templateMakeMap(br_tv_template* template) {
    br_tv_template_entry* tp;
    char* map;
    br_int_32 min;
    br_int_32 max;
    int i;
    int n;
    int e;
    NOT_IMPLEMENTED();
}

br_fixed_ls* ConvertFloatToFixed(br_fixed_ls** pextra, br_float* src, br_int_32 count, br_size_t* pextra_space) {
    br_fixed_ls* ret;
    NOT_IMPLEMENTED();
}

br_float* ConvertFixedToFloat(br_float** pextra, br_fixed_ls* src, br_int_32 count, br_size_t* pextra_space) {
    br_float* ret;
    NOT_IMPLEMENTED();
}

br_uint_32* ConvertLongCopy(br_uint_32** pextra, br_uint_32* src, br_int_32 count, br_size_t* pextra_space) {
    br_uint_32* ret;
    NOT_IMPLEMENTED();
}

br_error ValueQuery(br_token_value* tv, void** pextra, br_size_t* pextra_size, void* block, br_tv_template_entry* tep) {
    void* mem;
    br_uint_32* lp;
    int t;
    br_tv_custom* custp;
    NOT_IMPLEMENTED();
}

br_error ValueSet(void* block, br_token_value* tv, br_tv_template_entry* tep) {
    void* mem;
    int i;
    int t;
    br_tv_custom* custp;
    NOT_IMPLEMENTED();
}

br_size_t ValueExtraSize(void* block, br_tv_template_entry* tep) {
    void* mem;
    br_uint_32* lp;
    int t;
    br_tv_custom* custp;
    NOT_IMPLEMENTED();
}

br_error BrTokenValueQuery(br_uint_32* pvalue, br_uint_32* extra, br_size_t extra_size, br_token t, void* block, br_tv_template* template) {
    br_int_32 o;
    br_tv_template_entry* tep;
    br_token_value tv;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error BrTokenValueQueryMany(br_token_value* tv, void* extra, br_size_t extra_size, br_int_32* pcount, void* block, br_tv_template* template) {
    br_int_32 o;
    br_tv_template_entry* tep;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error BrTokenValueQueryManySize(br_size_t* psize, br_token_value* tv, void* block, br_tv_template* template) {
    br_int_32 o;
    br_size_t extra_size;
    NOT_IMPLEMENTED();
}

br_error BrTokenValueQueryAll(br_token_value* buffer, br_size_t buffer_size, void* block, br_tv_template* template) {
    br_tv_template_entry* tp;
    int n;
    int e;
    br_token_value* tv;
    char* extra;
    br_size_t extra_size;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error BrTokenValueQueryAllSize(br_size_t* psize, void* block, br_tv_template* template) {
    br_tv_template_entry* tp;
    int e;
    int n;
    br_size_t extra_size;
    NOT_IMPLEMENTED();
}

br_error BrTokenValueSet(void* mem, br_uint_32* pcombined_mask, br_token t, br_uint_32 value, br_tv_template* template) {
    br_int_32 o;
    br_error r;
    br_token_value tv;
    NOT_IMPLEMENTED();
}

br_error BrTokenValueSetMany(void* mem, br_int_32* pcount, br_uint_32* pcombined_mask, br_token_value* tv, br_tv_template* template) {
    br_int_32 o;
    br_int_32 n;
    br_error r;
    br_uint_32 cm;
    STUB();
}

void DumpMatrixInteger(br_int_32* ip, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    NOT_IMPLEMENTED();
}

void DumpMatrixFixed(br_fixed_ls* xp, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    NOT_IMPLEMENTED();
}

void DumpMatrixFloat(br_float* fp, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    NOT_IMPLEMENTED();
}

void DumpObject(br_object* h, char* prefix, char* info, br_putline_cbfn* putline, void* arg) {
    char value[128];
    char* dev_ident;
    char* h_ident;
    br_object* dev;
    NOT_IMPLEMENTED();
}

void BrTokenValueDump(br_token_value* tv, char* prefix, br_putline_cbfn* putline, void* arg) {
    char* id;
    char value[128];
    char tmp[128];
    char info_0[64];
    char* info_n;
    char* pvalue;
    int n;
    int i;
    br_object** pph;
    void** ppp;
    br_token* pt;
    NOT_IMPLEMENTED();
}

br_error BrStringToTokenValue(br_token_value* buffer, br_size_t buffer_size, char* str) {
    br_lexer* l;
    br_error r;
    static br_lexer_keyword keywords[4];
    NOT_IMPLEMENTED();
}

br_error parseTokenValue(br_lexer* l, br_token_value* tv, br_size_t size) {
    int len;
    char name[40];
    br_token type;
    static br_token real_types[2];
    br_error r;
    NOT_IMPLEMENTED();
}

br_boolean BrTokenValueCompare(br_token_value* tv1, br_token_value* tv2) {
    int i;
    NOT_IMPLEMENTED();
}

br_error BrTokenValueDup(br_token_value** dstp, br_token_value* src) {
    int i;
    int c;
    br_token_value* tv;
    NOT_IMPLEMENTED();
}
