#include "tokenval.h"
#include "harness.h"
#include "resource.h"

char rscid[50];

// Offset: 21
// Size: 88
br_tv_template* BrTVTemplateAllocate(void* res, br_tv_template_entry* entries, int n_entries) {
    br_tv_template* t;
    LOG_TRACE("(%p, %p, %d)", res, entries, n_entries);

    t = BrResAllocate(res, sizeof(br_tv_template), BR_MEMORY_TOKEN_TEMPLATE);
    t->res = t;
    t->entries = entries;
    t->n_entries = n_entries;
    return t;
}

// Offset: 126
// Size: 42
void BrTVTemplateFree(br_tv_template* t) {
    NOT_IMPLEMENTED();
}

// Offset: 189
// Size: 114
// EAX: template
void templateResolveNames(br_tv_template* template) {
    br_tv_template_entry* tp;
    int n;
    NOT_IMPLEMENTED();
}

// Offset: 319
// Size: 640
// EAX: template
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

// Offset: 979
// Size: 171
// EAX: pextra
// EDX: src
// EBX: count
// ECX: pextra_space
br_fixed_ls* ConvertFloatToFixed(br_fixed_ls** pextra, br_float* src, br_int_32 count, br_size_t* pextra_space) {
    br_fixed_ls* ret;
    NOT_IMPLEMENTED();
}

// Offset: 1170
// Size: 160
// EAX: pextra
// EDX: src
// EBX: count
// ECX: pextra_space
br_float* ConvertFixedToFloat(br_float** pextra, br_fixed_ls* src, br_int_32 count, br_size_t* pextra_space) {
    br_float* ret;
    NOT_IMPLEMENTED();
}

// Offset: 1346
// Size: 154
// EAX: pextra
// EDX: src
// EBX: count
// ECX: pextra_space
br_uint_32* ConvertLongCopy(br_uint_32** pextra, br_uint_32* src, br_int_32 count, br_size_t* pextra_space) {
    br_uint_32* ret;
    NOT_IMPLEMENTED();
}

// Offset: 1511
// Size: 1807
// EAX: tv
// EDX: pextra
// EBX: pextra_size
// ECX: block
br_error ValueQuery(br_token_value* tv, void** pextra, br_size_t* pextra_size, void* block, br_tv_template_entry* tep) {
    void* mem;
    br_uint_32* lp;
    int t;
    br_tv_custom* custp;
    NOT_IMPLEMENTED();
}

// Offset: 3327
// Size: 948
// EAX: block
// EDX: tv
// EBX: tep
br_error ValueSet(void* block, br_token_value* tv, br_tv_template_entry* tep) {
    void* mem;
    int i;
    int t;
    br_tv_custom* custp;
    NOT_IMPLEMENTED();
}

// Offset: 4290
// Size: 488
// EAX: block
// EDX: tep
br_size_t ValueExtraSize(void* block, br_tv_template_entry* tep) {
    void* mem;
    br_uint_32* lp;
    int t;
    br_tv_custom* custp;
    NOT_IMPLEMENTED();
}

// Offset: 4796
// Size: 160
br_error BrTokenValueQuery(br_uint_32* pvalue, br_uint_32* extra, br_size_t extra_size, br_token t, void* block, br_tv_template* template) {
    br_int_32 o;
    br_tv_template_entry* tep;
    br_token_value tv;
    br_error r;
    NOT_IMPLEMENTED();
}

// Offset: 4978
// Size: 183
br_error BrTokenValueQueryMany(br_token_value* tv, void* extra, br_size_t extra_size, br_int_32* pcount, void* block, br_tv_template* template) {
    br_int_32 o;
    br_tv_template_entry* tep;
    br_error r;
    NOT_IMPLEMENTED();
}

// Offset: 5187
// Size: 172
br_error BrTokenValueQueryManySize(br_size_t* psize, br_token_value* tv, void* block, br_tv_template* template) {
    br_int_32 o;
    br_size_t extra_size;
    NOT_IMPLEMENTED();
}

// Offset: 5380
// Size: 317
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

// Offset: 5722
// Size: 169
br_error BrTokenValueQueryAllSize(br_size_t* psize, void* block, br_tv_template* template) {
    br_tv_template_entry* tp;
    int e;
    int n;
    br_size_t extra_size;
    NOT_IMPLEMENTED();
}

// Offset: 5907
// Size: 203
br_error BrTokenValueSet(void* mem, br_uint_32* pcombined_mask, br_token t, br_uint_32 value, br_tv_template* template) {
    br_int_32 o;
    br_error r;
    br_token_value tv;
    NOT_IMPLEMENTED();
}

// Offset: 6130
// Size: 261
br_error BrTokenValueSetMany(void* mem, br_int_32* pcount, br_uint_32* pcombined_mask, br_token_value* tv, br_tv_template* template) {
    br_int_32 o;
    br_int_32 n;
    br_error r;
    br_uint_32 cm;
    STUB();
}

// Offset: 6409
// Size: 269
// EAX: ip
// EDX: rows
// EBX: cols
// ECX: prefix
void DumpMatrixInteger(br_int_32* ip, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    NOT_IMPLEMENTED();
}

// Offset: 6694
// Size: 293
// EAX: xp
// EDX: rows
// EBX: cols
// ECX: prefix
void DumpMatrixFixed(br_fixed_ls* xp, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    NOT_IMPLEMENTED();
}

// Offset: 7003
// Size: 287
// EAX: fp
// EDX: rows
// EBX: cols
// ECX: prefix
void DumpMatrixFloat(br_float* fp, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    NOT_IMPLEMENTED();
}

// Offset: 7301
// Size: 260
// EAX: h
// EDX: prefix
// EBX: info
// ECX: putline
void DumpObject(br_object* h, char* prefix, char* info, br_putline_cbfn* putline, void* arg) {
    char value[128];
    char* dev_ident;
    char* h_ident;
    br_object* dev;
    NOT_IMPLEMENTED();
}

// Offset: 7578
// Size: 2901
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

// Offset: 10500
// Size: 156
br_error BrStringToTokenValue(br_token_value* buffer, br_size_t buffer_size, char* str) {
    br_lexer* l;
    br_error r;
    static br_lexer_keyword keywords[4];
    NOT_IMPLEMENTED();
}

// Offset: 10672
// Size: 1301
// EAX: l
// EDX: tv
// EBX: size
br_error parseTokenValue(br_lexer* l, br_token_value* tv, br_size_t size) {
    int len;
    char name[40];
    br_token type;
    static br_token real_types[2];
    br_error r;
    NOT_IMPLEMENTED();
}

// Offset: 11993
// Size: 952
br_boolean BrTokenValueCompare(br_token_value* tv1, br_token_value* tv2) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 12961
// Size: 88
// EAX: dstp
// EDX: src
br_error BrTokenValueDup(br_token_value** dstp, br_token_value* src) {
    int i;
    int c;
    br_token_value* tv;
    NOT_IMPLEMENTED();
}
