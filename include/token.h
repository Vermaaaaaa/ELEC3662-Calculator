#ifndef TOKEN_H
#define TOKEN_H

#include "keypad.h"

#define MAX_TOKENS 32
#define MAX_INPUT_BUFFER 32


#define SUCCESS          0
#define TOKEN_LIMIT_ERROR   1
#define DECIMAL_ERROR    2
#define UNKNOWN_ERROR    3

typedef enum {
    TOKEN_NUM,
    TOKEN_OP,
    TOKEN_FUNC,
    NULL_TOKEN
} token_type_t;

typedef struct {
    token_type_t token_type;
    union {
        double value;       
        keypad_value op;   
    } data;
    unsigned char token_index;
} token_t;


int tokenise(keypad_value *input, token_t *tokens, unsigned char *token_count);


static void remove_token(token_t *tokens, unsigned char *token_count, unsigned char remove_index);

#endif 
