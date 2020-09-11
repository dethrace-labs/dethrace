#include "token.h"
#include "harness.h"

br_token_entry predefinedTokens[457];
char rscid[48];
token_type tokenTypes[37];

// IDA: void __cdecl BrTokenBegin()
void BrTokenBegin() {
    STUB();
}

// IDA: br_token __cdecl BrTokenCreate(char *identifier, br_token type)
br_token BrTokenCreate(char* identifier, br_token type) {
    br_token_entry* te;
    int i;
    int l;
    LOG_TRACE("(\"%s\", %d)", identifier, type);
    NOT_IMPLEMENTED();
}

// IDA: char* __cdecl BrTokenIdentifier(br_token t)
char* BrTokenIdentifier(br_token t) {
    br_token_entry* te;
    LOG_TRACE("(%d)", t);
    NOT_IMPLEMENTED();
}

// IDA: br_token __cdecl BrTokenType(br_token t)
br_token BrTokenType(br_token t) {
    br_token_entry* te;
    LOG_TRACE("(%d)", t);
    NOT_IMPLEMENTED();
}

// IDA: br_int_32 __cdecl BrTokenCount(char *pattern)
br_int_32 BrTokenCount(char* pattern) {
    br_token_entry* te;
    int n;
    LOG_TRACE("(\"%s\")", pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_token __cdecl BrTokenFind(char *pattern)
br_token BrTokenFind(char* pattern) {
    br_token_entry* te;
    LOG_TRACE("(\"%s\")", pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_int_32 __cdecl BrTokenFindMany(char *pattern, br_token *tokens, br_int_32 max_tokens)
br_int_32 BrTokenFindMany(char* pattern, br_token* tokens, br_int_32 max_tokens) {
    br_token_entry* te;
    int n;
    LOG_TRACE("(\"%s\", %p, %d)", pattern, tokens, max_tokens);
    NOT_IMPLEMENTED();
}

// IDA: br_token __usercall BrTokenFindType@<EAX>(br_token *ptype@<EAX>, char *base@<EDX>, br_token *types@<EBX>, br_int_32 ntypes@<ECX>)
br_token BrTokenFindType(br_token* ptype, char* base, br_token* types, br_int_32 ntypes) {
    br_token_entry* te;
    int l;
    int t;
    LOG_TRACE("(%p, \"%s\", %p, %d)", ptype, base, types, ntypes);
    NOT_IMPLEMENTED();
}
