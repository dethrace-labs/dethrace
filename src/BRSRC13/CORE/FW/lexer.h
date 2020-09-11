#ifndef _LEXER_H_
#define _LEXER_H_

#include "br_types.h"

void lexerError(br_lexer* l, char* string);

br_lexer* BrLexerAllocate(br_lexer_keyword* keywords, int nkeywords);

char BrLexerCommentSet(br_lexer* l, char eol_comment);

br_lexer_error_cbfn* BrLexerErrorSet(br_lexer* l, br_lexer_error_cbfn* error);

void BrLexerFree(br_lexer* l);

void fileGetchar(br_lexer_source* source);

br_error BrLexerPushFile(br_lexer* l, char* file);

void stringGetchar(br_lexer_source* source);

br_error BrLexerPushString(br_lexer* l, char* string, char* name);

br_lexer_source* BrLexerPop(br_lexer* l);

void lexerAdvance(br_lexer* l);

void lexerAdvanceDump(br_lexer* l);

br_error BrLexerDumpSet(br_lexer* l, br_putline_cbfn* putline, void* putline_arg);

void BrLexerTokenError(br_lexer* l, br_lexer_token_id t);

void BrLexerPosition(br_lexer* l, char* buf, br_size_t buf_size);

br_scalar BrParseFixed(br_lexer* l);

br_float BrParseFloat(br_lexer* l);

br_int_32 BrParseInteger(br_lexer* l);

br_int_32 BrParseVectorFixed(br_lexer* l, br_fixed_ls* v, br_int_32 max);

br_int_32 BrParseVectorFloat(br_lexer* l, br_float* v, br_int_32 max);

br_int_32 BrParseMatrixFixed(br_lexer* l, br_fixed_ls* m, br_int_32 width, br_int_32 max_h);

br_int_32 BrParseMatrixFloat(br_lexer* l, br_float* m, br_int_32 width, br_int_32 max_h);

#endif
