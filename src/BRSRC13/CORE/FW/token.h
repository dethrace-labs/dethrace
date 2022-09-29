#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "brender/br_types.h"

void BrTokenBegin(void);

br_token BrTokenCreate(char* identifier, br_token type);

char* BrTokenIdentifier(br_token t);

br_token BrTokenType(br_token t);

br_int_32 BrTokenCount(char* pattern);

br_token BrTokenFind(char* pattern);

br_int_32 BrTokenFindMany(char* pattern, br_token* tokens, br_int_32 max_tokens);

br_token BrTokenFindType(br_token* ptype, char* base, br_token* types, br_int_32 ntypes);

#endif
