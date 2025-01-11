#ifndef TOKEN_H
#define TOKEN_H

#include "keypad.h"

#define MAX_TOKENS 128

typedef struct{
	enum {TOKEN_NUM, TOKEN_OP, TOKEN_FUNC} type;
	double value;
	keypad_value op;
} token_t;

int tokenise(keypad_value *input, token_t *tokens, unsigned char* token_index);





#endif
