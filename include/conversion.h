#ifndef CONVERSION_H
#define CONVERSION_H

#include "token.h"
#include "keypad.h"
#include "stack.h"

int infix_to_postfix(token_t *infix, unsigned char infix_index, token_t *postfix, unsigned char* postfix_index );

static int op_precedence(keypad_value op);

static bool_t is_right_associative(keypad_value op);

static bool_t is_valid_sequence(token_t *infix, unsigned char infix_index);

bool_t is_operator(keypad_value op);

bool_t is_function(keypad_value op);

static bool_t is_unary_operator(token_t prev_tokem, token_t current_token);

static void pop_operators_to_postfix(stack *operator_stack, token_t *postfix, unsigned char* postfix_index, keypad_value current_op);



 




#endif