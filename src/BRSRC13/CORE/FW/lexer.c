#include "lexer.h"
#include "harness.h"

// IDA: void __cdecl lexerError(br_lexer *l, char *string)
void lexerError(br_lexer* l, char* string) {
    LOG_TRACE("(%p, \"%s\")", l, string);
    NOT_IMPLEMENTED();
}

// IDA: br_lexer* __cdecl BrLexerAllocate(br_lexer_keyword *keywords, int nkeywords)
br_lexer* BrLexerAllocate(br_lexer_keyword* keywords, int nkeywords) {
    br_lexer *l;
    LOG_TRACE("(%p, %d)", keywords, nkeywords);
    NOT_IMPLEMENTED();
}

// IDA: char __cdecl BrLexerCommentSet(br_lexer *l, char eol_comment)
char BrLexerCommentSet(br_lexer* l, char eol_comment) {
    char old;
    LOG_TRACE("(%p, %d)", l, eol_comment);
    NOT_IMPLEMENTED();
}

// IDA: br_lexer_error_cbfn* __cdecl BrLexerErrorSet(br_lexer *l, br_lexer_error_cbfn *error)
br_lexer_error_cbfn* BrLexerErrorSet(br_lexer* l, br_lexer_error_cbfn* error) {
    br_lexer_error_cbfn *old;
    LOG_TRACE("(%p, %p)", l, error);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrLexerFree(br_lexer *l)
void BrLexerFree(br_lexer* l) {
    LOG_TRACE("(%p)", l);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl fileGetchar(br_lexer_source *source)
void fileGetchar(br_lexer_source* source) {
    LOG_TRACE("(%p)", source);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrLexerPushFile(br_lexer *l, char *file)
br_error BrLexerPushFile(br_lexer* l, char* file) {
    br_lexer_source *s;
    void *f;
    int mode;
    LOG_TRACE("(%p, \"%s\")", l, file);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl stringGetchar(br_lexer_source *source)
void stringGetchar(br_lexer_source* source) {
    char *cp;
    LOG_TRACE("(%p)", source);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrLexerPushString(br_lexer *l, char *string, char *name)
br_error BrLexerPushString(br_lexer* l, char* string, char* name) {
    br_lexer_source *s;
    LOG_TRACE("(%p, \"%s\", \"%s\")", l, string, name);
    NOT_IMPLEMENTED();
}

// IDA: br_lexer_source* __cdecl BrLexerPop(br_lexer *l)
br_lexer_source* BrLexerPop(br_lexer* l) {
    br_lexer_source *s;
    LOG_TRACE("(%p)", l);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall lexerAdvance(br_lexer *l@<EAX>)
void lexerAdvance(br_lexer* l) {
    int n;
    br_boolean got_point;
    LOG_TRACE("(%p)", l);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall lexerAdvanceDump(br_lexer *l@<EAX>)
void lexerAdvanceDump(br_lexer* l) {
    char *tname;
    char *tvalue;
    char val[40];
    char tmp[256];
    LOG_TRACE("(%p)", l);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrLexerDumpSet(br_lexer *l, br_putline_cbfn *putline, void *putline_arg)
br_error BrLexerDumpSet(br_lexer* l, br_putline_cbfn* putline, void* putline_arg) {
    LOG_TRACE("(%p, %p, %p)", l, putline, putline_arg);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrLexerTokenError(br_lexer *l, br_lexer_token_id t)
void BrLexerTokenError(br_lexer* l, br_lexer_token_id t) {
    int i;
    char tmp[256];
    LOG_TRACE("(%p, %d)", l, t);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrLexerPosition(br_lexer *l, char *buf, br_size_t buf_size)
void BrLexerPosition(br_lexer* l, char* buf, br_size_t buf_size) {
    LOG_TRACE("(%p, \"%s\", %d)", l, buf, buf_size);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl BrParseFixed(br_lexer *l)
br_scalar BrParseFixed(br_lexer* l) {
    br_boolean neg;
    br_fixed_ls x;
    LOG_TRACE("(%p)", l);
    NOT_IMPLEMENTED();
}

// IDA: br_float __cdecl BrParseFloat(br_lexer *l)
br_float BrParseFloat(br_lexer* l) {
    br_boolean neg;
    br_float f;
    LOG_TRACE("(%p)", l);
    NOT_IMPLEMENTED();
}

// IDA: br_int_32 __cdecl BrParseInteger(br_lexer *l)
br_int_32 BrParseInteger(br_lexer* l) {
    br_boolean neg;
    br_int_32 i;
    LOG_TRACE("(%p)", l);
    NOT_IMPLEMENTED();
}

// IDA: br_int_32 __cdecl BrParseVectorFixed(br_lexer *l, br_fixed_ls *v, br_int_32 max)
br_int_32 BrParseVectorFixed(br_lexer* l, br_fixed_ls* v, br_int_32 max) {
    int n;
    LOG_TRACE("(%p, %p, %d)", l, v, max);
    NOT_IMPLEMENTED();
}

// IDA: br_int_32 __cdecl BrParseVectorFloat(br_lexer *l, br_float *v, br_int_32 max)
br_int_32 BrParseVectorFloat(br_lexer* l, br_float* v, br_int_32 max) {
    int n;
    LOG_TRACE("(%p, %p, %d)", l, v, max);
    NOT_IMPLEMENTED();
}

// IDA: br_int_32 __cdecl BrParseMatrixFixed(br_lexer *l, br_fixed_ls *m, br_int_32 width, br_int_32 max_h)
br_int_32 BrParseMatrixFixed(br_lexer* l, br_fixed_ls* m, br_int_32 width, br_int_32 max_h) {
    int n;
    LOG_TRACE("(%p, %p, %d, %d)", l, m, width, max_h);
    NOT_IMPLEMENTED();
}

// IDA: br_int_32 __cdecl BrParseMatrixFloat(br_lexer *l, br_float *m, br_int_32 width, br_int_32 max_h)
br_int_32 BrParseMatrixFloat(br_lexer* l, br_float* m, br_int_32 width, br_int_32 max_h) {
    int n;
    LOG_TRACE("(%p, %p, %d, %d)", l, m, width, max_h);
    NOT_IMPLEMENTED();
}
