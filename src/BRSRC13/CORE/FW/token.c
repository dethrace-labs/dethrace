#include "token.h"
#include "harness.h"

br_token_entry predefinedTokens[457];
char rscid[48];
token_type tokenTypes[37];

void BrTokenBegin() {
    STUB();
}

br_token BrTokenCreate(char* identifier, br_token type) {
    br_token_entry* te;
    int i;
    int l;
    NOT_IMPLEMENTED();
}

char* BrTokenIdentifier(br_token t) {
    br_token_entry* te;
    NOT_IMPLEMENTED();
}

br_token BrTokenType(br_token t) {
    br_token_entry* te;
    NOT_IMPLEMENTED();
}

br_int_32 BrTokenCount(char* pattern) {
    br_token_entry* te;
    int n;
    NOT_IMPLEMENTED();
}

br_token BrTokenFind(char* pattern) {
    br_token_entry* te;
    NOT_IMPLEMENTED();
}

br_int_32 BrTokenFindMany(char* pattern, br_token* tokens, br_int_32 max_tokens) {
    br_token_entry* te;
    int n;
    NOT_IMPLEMENTED();
}

br_token BrTokenFindType(br_token* ptype, char* base, br_token* types, br_int_32 ntypes) {
    br_token_entry* te;
    int l;
    int t;
    NOT_IMPLEMENTED();
}
