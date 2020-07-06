#include "token.h"
#include "harness.h"

br_token_entry predefinedTokens[457];
char rscid[48];
token_type tokenTypes[37];

// Offset: 13
// Size: 112
void BrTokenBegin() {
    LOG_WARN("Not implemented");
}

// Offset: 139
// Size: 509
br_token BrTokenCreate(char* identifier, br_token type) {
    br_token_entry* te;
    int i;
    int l;
    NOT_IMPLEMENTED();
}

// Offset: 666
// Size: 92
char* BrTokenIdentifier(br_token t) {
    br_token_entry* te;
    NOT_IMPLEMENTED();
}

// Offset: 770
// Size: 92
br_token BrTokenType(br_token t) {
    br_token_entry* te;
    NOT_IMPLEMENTED();
}

// Offset: 875
// Size: 148
br_int_32 BrTokenCount(char* pattern) {
    br_token_entry* te;
    int n;
    NOT_IMPLEMENTED();
}

// Offset: 1035
// Size: 103
br_token BrTokenFind(char* pattern) {
    br_token_entry* te;
    NOT_IMPLEMENTED();
}

// Offset: 1154
// Size: 127
br_int_32 BrTokenFindMany(char* pattern, br_token* tokens, br_int_32 max_tokens) {
    br_token_entry* te;
    int n;
    NOT_IMPLEMENTED();
}

// Offset: 1297
// Size: 242
// EAX: ptype
// EDX: base
// EBX: types
// ECX: ntypes
br_token BrTokenFindType(br_token* ptype, char* base, br_token* types, br_int_32 ntypes) {
    br_token_entry* te;
    int l;
    int t;
    NOT_IMPLEMENTED();
}
