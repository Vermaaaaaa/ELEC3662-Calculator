#ifndef CONVERSION_H
#define CONVERSION_H

#include "token.h"
#include "keypad.h"
#include "stack.h"

#define SUCCESS 0
#define EMPTY_INFIX_ERROR -1
#define FIRST_TOKEN_OP_ERROR -2
#define RIGHT_OPERATOR_EMPTY_ERROR -3
#define RIGHT_OPERATOR_TOKEN_ERROR -4
#define DOUBLE_OPERATOR_ERROR -5
#define EMPTY_FUNC_ERROR -6
#define OP_FUNC_ERROR -7
#define LAST_TOKEN_OP_ERROR -8
#define ERROR -9


int infix_to_postfix(token_t *infix, unsigned char infix_index, token_t *postfix, unsigned char* postfix_index );

static int op_precedence(keypad_value op);

static bool_t is_right_associative(keypad_value op);

static int is_valid_sequence(token_t *infix, unsigned char infix_index);

bool_t is_operator(keypad_value op);

bool_t is_function(keypad_value op);

static bool_t is_unary_operator(token_t prev_tokem, token_t current_token);

static void pop_operators_to_postfix(stack *operator_stack, token_t *postfix, unsigned char* postfix_index, keypad_value current_op);



 




#endif