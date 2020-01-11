#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "br_types.h"

// Offset: 13
// Size: 112
void BrTokenBegin();

// Offset: 139
// Size: 509
br_token BrTokenCreate(char* identifier, br_token type);

// Offset: 666
// Size: 92
char* BrTokenIdentifier(br_token t);

// Offset: 770
// Size: 92
br_token BrTokenType(br_token t);

// Offset: 875
// Size: 148
br_int_32 BrTokenCount(char* pattern);

// Offset: 1035
// Size: 103
br_token BrTokenFind(char* pattern);

// Offset: 1154
// Size: 127
br_int_32 BrTokenFindMany(char* pattern, br_token* tokens, br_int_32 max_tokens);

// Offset: 1297
// Size: 242
// EAX: ptype
// EDX: base
// EBX: types
// ECX: ntypes
br_token BrTokenFindType(br_token* ptype, char* base, br_token* types, br_int_32 ntypes);

#endif
