#include "conversion.h"

static int op_precedence(keypad_value op);

static bool_t is_right_associative(keypad_value op){
	switch(op){
		case(POWER):{ return true; }
		case(MINUS):{ return true; }
		default:{ return false; }
	}
}

static bool_t is_valid_sequence(token_t *infix, unsigned char infix_index);

bool_t is_operator(keypad_value op) {
    switch (op) {
        case(PLUS):{return true;}
        case (MINUS):{return true;}
        case (MULT):{return true;}
        case (DIV):{return true;}
        case (POWER):{return true;}
        default: {return false;}
    }
}


bool_t is_function(keypad_value op){
	switch (op) {
        case (SIN):{return true;}
        case (COS):{return true;}
        case (TAN):{return true;}
        case (SQRT):{return true;}
        case (SQUARE):{return true;}
        default:{return false;}   
    }
}

static bool_t is_unary_operator(token_t prev_token, token_t current_token) {
    if (current_token.token_type == TOKEN_OP && current_token.data.op == MINUS) {
        if (prev_token.token_type == TOKEN_OP || prev_token.token_type == TOKEN_FUNC) {
            return true; 
        }
    }
    return false; 
}


static void pop_operators_to_postfix(stack *operator_stack, token_t *postfix, unsigned char* postfix_index, keypad_value current_op);