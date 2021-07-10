#include "tokenval.h"
#include "harness_trace.h"
#include "resource.h"

char rscid[50];

// IDA: br_tv_template* __cdecl BrTVTemplateAllocate(void *res, br_tv_template_entry *entries, int n_entries)
br_tv_template* BrTVTemplateAllocate(void* res, br_tv_template_entry* entries, int n_entries) {
    br_tv_template* t;
    LOG_TRACE("(%p, %p, %d)", res, entries, n_entries);

    t = BrResAllocate(res, sizeof(br_tv_template), BR_MEMORY_TOKEN_TEMPLATE);
    t->res = t;
    t->entries = entries;
    t->n_entries = n_entries;
    return t;
}

// IDA: void __cdecl BrTVTemplateFree(br_tv_template *t)
void BrTVTemplateFree(br_tv_template* t) {
    LOG_TRACE("(%p)", t);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall templateResolveNames(br_tv_template *template@<EAX>)
void templateResolveNames(br_tv_template* template) {
    br_tv_template_entry* tp;
    int n;
    LOG_TRACE("(%p)", template);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall templateMakeMap(br_tv_template *template@<EAX>)
void templateMakeMap(br_tv_template* template) {
    br_tv_template_entry* tp;
    char* map;
    br_int_32 min;
    br_int_32 max;
    int i;
    int n;
    int e;
    LOG_TRACE("(%p)", template);
    NOT_IMPLEMENTED();
}

// IDA: br_fixed_ls* __usercall ConvertFloatToFixed@<EAX>(br_fixed_ls **pextra@<EAX>, br_float *src@<EDX>, br_int_32 count@<EBX>, br_size_t *pextra_space@<ECX>)
br_fixed_ls* ConvertFloatToFixed(br_fixed_ls** pextra, br_float* src, br_int_32 count, br_size_t* pextra_space) {
    br_fixed_ls* ret;
    LOG_TRACE("(%p, %p, %d, %p)", pextra, src, count, pextra_space);
    NOT_IMPLEMENTED();
}

// IDA: br_float* __usercall ConvertFixedToFloat@<EAX>(br_float **pextra@<EAX>, br_fixed_ls *src@<EDX>, br_int_32 count@<EBX>, br_size_t *pextra_space@<ECX>)
br_float* ConvertFixedToFloat(br_float** pextra, br_fixed_ls* src, br_int_32 count, br_size_t* pextra_space) {
    br_float* ret;
    LOG_TRACE("(%p, %p, %d, %p)", pextra, src, count, pextra_space);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32* __usercall ConvertLongCopy@<EAX>(br_uint_32 **pextra@<EAX>, br_uint_32 *src@<EDX>, br_int_32 count@<EBX>, br_size_t *pextra_space@<ECX>)
br_uint_32* ConvertLongCopy(br_uint_32** pextra, br_uint_32* src, br_int_32 count, br_size_t* pextra_space) {
    br_uint_32* ret;
    LOG_TRACE("(%p, %p, %d, %p)", pextra, src, count, pextra_space);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall ValueQuery@<EAX>(br_token_value *tv@<EAX>, void **pextra@<EDX>, br_size_t *pextra_size@<EBX>, void *block@<ECX>, br_tv_template_entry *tep)
br_error ValueQuery(br_token_value* tv, void** pextra, br_size_t* pextra_size, void* block, br_tv_template_entry* tep) {
    void* mem;
    br_uint_32* lp;
    int t;
    br_tv_custom* custp;
    LOG_TRACE("(%p, %p, %p, %p, %p)", tv, pextra, pextra_size, block, tep);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall ValueSet@<EAX>(void *block@<EAX>, br_token_value *tv@<EDX>, br_tv_template_entry *tep@<EBX>)
br_error ValueSet(void* block, br_token_value* tv, br_tv_template_entry* tep) {
    void* mem;
    int i;
    int t;
    br_tv_custom* custp;
    LOG_TRACE("(%p, %p, %p)", block, tv, tep);
    NOT_IMPLEMENTED();
}

// IDA: br_size_t __usercall ValueExtraSize@<EAX>(void *block@<EAX>, br_tv_template_entry *tep@<EDX>)
br_size_t ValueExtraSize(void* block, br_tv_template_entry* tep) {
    void* mem;
    br_uint_32* lp;
    int t;
    br_tv_custom* custp;
    LOG_TRACE("(%p, %p)", block, tep);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrTokenValueQuery(br_uint_32 *pvalue, br_uint_32 *extra, br_size_t extra_size, br_token t, void *block, br_tv_template *template)
br_error BrTokenValueQuery(br_uint_32* pvalue, br_uint_32* extra, br_size_t extra_size, br_token t, void* block, br_tv_template* template) {
    br_int_32 o;
    br_tv_template_entry* tep;
    br_token_value tv;
    br_error r;
    LOG_TRACE("(%p, %p, %d, %d, %p, %p)", pvalue, extra, extra_size, t, block, template);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrTokenValueQueryMany(br_token_value *tv, void *extra, br_size_t extra_size, br_int_32 *pcount, void *block, br_tv_template *template)
br_error BrTokenValueQueryMany(br_token_value* tv, void* extra, br_size_t extra_size, br_int_32* pcount, void* block, br_tv_template* template) {
    br_int_32 o;
    br_tv_template_entry* tep;
    br_error r;
    LOG_TRACE("(%p, %p, %d, %p, %p, %p)", tv, extra, extra_size, pcount, block, template);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrTokenValueQueryManySize(br_size_t *psize, br_token_value *tv, void *block, br_tv_template *template)
br_error BrTokenValueQueryManySize(br_size_t* psize, br_token_value* tv, void* block, br_tv_template* template) {
    br_int_32 o;
    br_size_t extra_size;
    LOG_TRACE("(%p, %p, %p, %p)", psize, tv, block, template);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrTokenValueQueryAll(br_token_value *buffer, br_size_t buffer_size, void *block, br_tv_template *template)
br_error BrTokenValueQueryAll(br_token_value* buffer, br_size_t buffer_size, void* block, br_tv_template* template) {
    br_tv_template_entry* tp;
    int n;
    int e;
    br_token_value* tv;
    char* extra;
    br_size_t extra_size;
    br_error r;
    LOG_TRACE("(%p, %d, %p, %p)", buffer, buffer_size, block, template);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrTokenValueQueryAllSize(br_size_t *psize, void *block, br_tv_template *template)
br_error BrTokenValueQueryAllSize(br_size_t* psize, void* block, br_tv_template* template) {
    br_tv_template_entry* tp;
    int e;
    int n;
    br_size_t extra_size;
    LOG_TRACE("(%p, %p, %p)", psize, block, template);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrTokenValueSet(void *mem, br_uint_32 *pcombined_mask, br_token t, br_uint_32 value, br_tv_template *template)
br_error BrTokenValueSet(void* mem, br_uint_32* pcombined_mask, br_token t, br_uint_32 value, br_tv_template* template) {
    br_int_32 o;
    br_error r;
    br_token_value tv;
    LOG_TRACE("(%p, %p, %d, %d, %p)", mem, pcombined_mask, t, value, template);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrTokenValueSetMany(void *mem, br_int_32 *pcount, br_uint_32 *pcombined_mask, br_token_value *tv, br_tv_template *template)
br_error BrTokenValueSetMany(void* mem, br_int_32* pcount, br_uint_32* pcombined_mask, br_token_value* tv, br_tv_template* template) {
    br_int_32 o;
    br_int_32 n;
    br_error r;
    br_uint_32 cm;
    STUB();
}

// IDA: void __usercall DumpMatrixInteger(br_int_32 *ip@<EAX>, int rows@<EDX>, int cols@<EBX>, char *prefix@<ECX>, char *info_0, char *info_n, br_putline_cbfn *putline, void *arg)
void DumpMatrixInteger(br_int_32* ip, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    LOG_TRACE("(%p, %d, %d, \"%s\", \"%s\", \"%s\", %p, %p)", ip, rows, cols, prefix, info_0, info_n, putline, arg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DumpMatrixFixed(br_fixed_ls *xp@<EAX>, int rows@<EDX>, int cols@<EBX>, char *prefix@<ECX>, char *info_0, char *info_n, br_putline_cbfn *putline, void *arg)
void DumpMatrixFixed(br_fixed_ls* xp, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    LOG_TRACE("(%p, %d, %d, \"%s\", \"%s\", \"%s\", %p, %p)", xp, rows, cols, prefix, info_0, info_n, putline, arg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DumpMatrixFloat(br_float *fp@<EAX>, int rows@<EDX>, int cols@<EBX>, char *prefix@<ECX>, char *info_0, char *info_n, br_putline_cbfn *putline, void *arg)
void DumpMatrixFloat(br_float* fp, int rows, int cols, char* prefix, char* info_0, char* info_n, br_putline_cbfn* putline, void* arg) {
    int i;
    int j;
    char* cp;
    char value[128];
    LOG_TRACE("(%p, %d, %d, \"%s\", \"%s\", \"%s\", %p, %p)", fp, rows, cols, prefix, info_0, info_n, putline, arg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DumpObject(br_object *h@<EAX>, char *prefix@<EDX>, char *info@<EBX>, br_putline_cbfn *putline@<ECX>, void *arg)
void DumpObject(br_object* h, char* prefix, char* info, br_putline_cbfn* putline, void* arg) {
    char value[128];
    char* dev_ident;
    char* h_ident;
    br_object* dev;
    LOG_TRACE("(%p, \"%s\", \"%s\", %p, %p)", h, prefix, info, putline, arg);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrTokenValueDump(br_token_value *tv, char *prefix, br_putline_cbfn *putline, void *arg)
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
    LOG_TRACE("(%p, \"%s\", %p, %p)", tv, prefix, putline, arg);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrStringToTokenValue(br_token_value *buffer, br_size_t buffer_size, char *str)
br_error BrStringToTokenValue(br_token_value* buffer, br_size_t buffer_size, char* str) {
    br_lexer* l;
    br_error r;
    static br_lexer_keyword keywords[4];
    LOG_TRACE("(%p, %d, \"%s\")", buffer, buffer_size, str);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall parseTokenValue@<EAX>(br_lexer *l@<EAX>, br_token_value *tv@<EDX>, br_size_t size@<EBX>)
br_error parseTokenValue(br_lexer* l, br_token_value* tv, br_size_t size) {
    int len;
    char name[40];
    br_token type;
    static br_token real_types[2];
    br_error r;
    LOG_TRACE("(%p, %p, %d)", l, tv, size);
    NOT_IMPLEMENTED();
}

// IDA: br_boolean __cdecl BrTokenValueCompare(br_token_value *tv1, br_token_value *tv2)
br_boolean BrTokenValueCompare(br_token_value* tv1, br_token_value* tv2) {
    int i;
    LOG_TRACE("(%p, %p)", tv1, tv2);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall BrTokenValueDup@<EAX>(br_token_value **dstp@<EAX>, br_token_value *src@<EDX>)
br_error BrTokenValueDup(br_token_value** dstp, br_token_value* src) {
    int i;
    int c;
    br_token_value* tv;
    LOG_TRACE("(%p, %p)", dstp, src);
    NOT_IMPLEMENTED();
}
