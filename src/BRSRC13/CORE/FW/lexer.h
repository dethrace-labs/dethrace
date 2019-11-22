#ifndef _LEXER_H_
#define _LEXER_H_

#include "br_types.h"

// Offset: 11
// Size: 30
void lexerError(br_lexer *l, char *string);

// Offset: 57
// Size: 167
br_lexer* BrLexerAllocate(br_lexer_keyword *keywords, int nkeywords);

// Offset: 242
// Size: 57
char BrLexerCommentSet(br_lexer *l, char eol_comment);

// Offset: 315
// Size: 94
br_lexer_error_cbfn* BrLexerErrorSet(br_lexer *l, br_lexer_error_cbfn *error);

// Offset: 421
// Size: 42
void BrLexerFree(br_lexer *l);

// Offset: 475
// Size: 52
void fileGetchar(br_lexer_source *source);

// Offset: 543
// Size: 211
br_error BrLexerPushFile(br_lexer *l, char *file);

// Offset: 768
// Size: 88
void stringGetchar(br_lexer_source *source);

// Offset: 874
// Size: 147
br_error BrLexerPushString(br_lexer *l, char *string, char *name);

// Offset: 1032
// Size: 83
br_lexer_source* BrLexerPop(br_lexer *l);

// Offset: 1128
// Size: 1264
// EAX: l
void lexerAdvance(br_lexer *l);

// Offset: 2409
// Size: 529
// EAX: l
void lexerAdvanceDump(br_lexer *l);

// Offset: 2953
// Size: 86
br_error BrLexerDumpSet(br_lexer *l, br_putline_cbfn *putline, void *putline_arg);

// Offset: 3057
// Size: 539
void BrLexerTokenError(br_lexer *l, br_lexer_token_id t);

// Offset: 3612
// Size: 126
void BrLexerPosition(br_lexer *l, char *buf, br_size_t buf_size);

// Offset: 3751
// Size: 213
br_scalar BrParseFixed(br_lexer *l);

// Offset: 3977
// Size: 209
br_float BrParseFloat(br_lexer *l);

// Offset: 4201
// Size: 170
br_int_32 BrParseInteger(br_lexer *l);

// Offset: 4390
// Size: 213
br_int_32 BrParseVectorFixed(br_lexer *l, br_fixed_ls *v, br_int_32 max);

// Offset: 4622
// Size: 213
br_int_32 BrParseVectorFloat(br_lexer *l, br_float *v, br_int_32 max);

// Offset: 4854
// Size: 218
br_int_32 BrParseMatrixFixed(br_lexer *l, br_fixed_ls *m, br_int_32 width, br_int_32 max_h);

// Offset: 5091
// Size: 218
br_int_32 BrParseMatrixFloat(br_lexer *l, br_float *m, br_int_32 width, br_int_32 max_h);

#endif
