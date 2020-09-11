#include "lexer.h"
#include "harness.h"

void lexerError(br_lexer* l, char* string) {
    NOT_IMPLEMENTED();
}

br_lexer* BrLexerAllocate(br_lexer_keyword* keywords, int nkeywords) {
    br_lexer* l;
    NOT_IMPLEMENTED();
}

char BrLexerCommentSet(br_lexer* l, char eol_comment) {
    char old;
    NOT_IMPLEMENTED();
}

br_lexer_error_cbfn* BrLexerErrorSet(br_lexer* l, br_lexer_error_cbfn* error) {
    br_lexer_error_cbfn* old;
    NOT_IMPLEMENTED();
}

void BrLexerFree(br_lexer* l) {
    NOT_IMPLEMENTED();
}

void fileGetchar(br_lexer_source* source) {
    NOT_IMPLEMENTED();
}

br_error BrLexerPushFile(br_lexer* l, char* file) {
    br_lexer_source* s;
    void* f;
    int mode;
    NOT_IMPLEMENTED();
}

void stringGetchar(br_lexer_source* source) {
    char* cp;
    NOT_IMPLEMENTED();
}

br_error BrLexerPushString(br_lexer* l, char* string, char* name) {
    br_lexer_source* s;
    NOT_IMPLEMENTED();
}

br_lexer_source* BrLexerPop(br_lexer* l) {
    br_lexer_source* s;
    NOT_IMPLEMENTED();
}

void lexerAdvance(br_lexer* l) {
    int n;
    br_boolean got_point;
    NOT_IMPLEMENTED();
}

void lexerAdvanceDump(br_lexer* l) {
    char* tname;
    char* tvalue;
    char val[40];
    char tmp[256];
    NOT_IMPLEMENTED();
}

br_error BrLexerDumpSet(br_lexer* l, br_putline_cbfn* putline, void* putline_arg) {
    NOT_IMPLEMENTED();
}

void BrLexerTokenError(br_lexer* l, br_lexer_token_id t) {
    int i;
    char tmp[256];
    NOT_IMPLEMENTED();
}

void BrLexerPosition(br_lexer* l, char* buf, br_size_t buf_size) {
    NOT_IMPLEMENTED();
}

br_scalar BrParseFixed(br_lexer* l) {
    br_boolean neg;
    br_fixed_ls x;
    NOT_IMPLEMENTED();
}

br_float BrParseFloat(br_lexer* l) {
    br_boolean neg;
    br_float f;
    NOT_IMPLEMENTED();
}

br_int_32 BrParseInteger(br_lexer* l) {
    br_boolean neg;
    br_int_32 i;
    NOT_IMPLEMENTED();
}

br_int_32 BrParseVectorFixed(br_lexer* l, br_fixed_ls* v, br_int_32 max) {
    int n;
    NOT_IMPLEMENTED();
}

br_int_32 BrParseVectorFloat(br_lexer* l, br_float* v, br_int_32 max) {
    int n;
    NOT_IMPLEMENTED();
}

br_int_32 BrParseMatrixFixed(br_lexer* l, br_fixed_ls* m, br_int_32 width, br_int_32 max_h) {
    int n;
    NOT_IMPLEMENTED();
}

br_int_32 BrParseMatrixFloat(br_lexer* l, br_float* m, br_int_32 width, br_int_32 max_h) {
    int n;
    NOT_IMPLEMENTED();
}
