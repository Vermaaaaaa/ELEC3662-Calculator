#include "conversion.h"

int infix_to_postfix(token_t *infix, unsigned char infix_index, token_t *postfix, unsigned char *postfix_index){
	int valid_sequence = is_valid_sequence(infix, infix_index);
	
	if(valid_sequence != SUCCESS) { return valid_sequence; }
	
	stack op_stack;
	stack_init(&op_stack);
	
	*postfix_index = 0;
	
	for(unsigned char i = 0; i < infix_index; i++){
		token_t current_token = infix[i];
		
		if(current_token.token_type == TOKEN_NUM){
			postfix[*postfix_index] = current_token;
			(*postfix_index)++;
		}
		else if(current_token.token_type == TOKEN_FUNC){stack_push(&op_stack, current_token.data.op);}
		
		else if(current_token.token_type == TOKEN_OP){
			keypad_value current_token_op = current_token.data.op;
			
			pop_to_postfix(&op_stack, postfix, postfix_index, current_token_op);
			
			stack_push(&op_stack, current_token_op);
		}
		else{return ERROR;}
	}
	while(!stack_isEmpty(&op_stack)){
		int top_op = stack_pop(&op_stack);
		if(top_op == STACK_ERROR) {return ERROR;}
		
		 token_t op_token;
     op_token.token_type = is_function((keypad_value)top_op) ? TOKEN_FUNC : TOKEN_OP;
     op_token.data.op = (keypad_value)top_op;
     postfix[*postfix_index] = op_token;
     (*postfix_index)++;

     if (*postfix_index >= MAX_TOKENS) {return ERROR;} 
	}
	return SUCCESS;
}

static int op_precedence(keypad_value op) {
    switch (op) {
        case POWER: return 3;
        case DIV:	return 3;
				case MULT: return 2;
        case PLUS: return 2;
        case MINUS: return 1;
        default:    return 0; 
    }
}

static bool_t is_right_associative(keypad_value op){
	switch(op){
		case(POWER):{ return true; }
		default:{ return false; }
	}
}

static int is_valid_sequence(token_t *infix, unsigned char infix_index){
	if(infix_index == 0) {return EMPTY_INFIX_ERROR;}
	
	token_t prev_token = {.token_type = NULL_TOKEN};	
	
	for(unsigned char i = 0; i < infix_index; i++){
		token_t current_token = infix[i];
		token_t next_token = {.token_type = NULL_TOKEN};
		
		if(i + 1 < infix_index) {next_token = infix[i+1];}
		
		if(current_token.token_type == TOKEN_OP){
			if(i == 0) { 
				if(current_token.token_type == TOKEN_OP){ return FIRST_TOKEN_OP_ERROR; }
			}			
		}
		if(current_token.token_type == TOKEN_OP){
				keypad_value op = current_token.data.op;
				
				if(op == POWER || op == SCINOT || op == SQUARE){
					if(i+1 >= infix_index) { return RIGHT_OPERATOR_EMPTY_ERROR; }  
					 if (!( next_token.token_type == TOKEN_NUM || (next_token.token_type == TOKEN_FUNC) || (next_token.token_type == TOKEN_OP && next_token.data.op == MINUS))){
						 return RIGHT_OPERATOR_TOKEN_ERROR;
					 }
				}
			}
		
		if(current_token.token_type == TOKEN_OP && (prev_token.token_type == TOKEN_OP && prev_token.data.op != SQUARE)) {return DOUBLE_OPERATOR_ERROR;}
		
		if(current_token.token_type == TOKEN_FUNC && current_token.data.op != SQUARE){
				if( i + 1 >= infix_index) {return EMPTY_FUNC_ERROR;}
				if(next_token.token_type == TOKEN_OP){ return OP_FUNC_ERROR; }
			}
		prev_token = current_token;
		
	}
	
	token_t last_token = infix[infix_index - 1];
	if(last_token.token_type == TOKEN_OP) { return LAST_TOKEN_OP_ERROR;}
	
	return SUCCESS;
}

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
				case (ARCSIN):{return true;}
				case (ARCCOS):{return true;}
				case (ARCTAN):{return true;}
        case (SQRT):{return true;}
        case (SQUARE):{return true;}
				case (UNARY_MINUS): {return true;}
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


static void pop_to_postfix(stack *op_stack, token_t *postfix, unsigned char* postfix_index, keypad_value current_op){
    while (!stack_isEmpty(op_stack)) {
        int top_op = op_stack->stk[op_stack->top];

       
        bool_t top_is_func = is_function((keypad_value)top_op);
        bool_t top_is_op   = is_operator((keypad_value)top_op);

        if (!top_is_func && !top_is_op) {break;}

      
        if (top_is_func) {
            top_op = stack_pop(op_stack);
            token_t token = {.token_type = TOKEN_FUNC, token.data.op = top_op};
            postfix[*postfix_index] = token;
            (*postfix_index)++;
            continue;
        }
        if (top_is_op) {

            int top_prec  = op_precedence((keypad_value)top_op);
            int curr_prec = op_precedence(current_op);

            bool_t must_pop = false;
            if (top_prec > curr_prec) {must_pop = true;}
            else if (top_prec == curr_prec && !is_right_associative(current_op)) {must_pop = false;}

            if (must_pop) {
                top_op = stack_pop(op_stack);
                token_t t;
                t.token_type = TOKEN_OP;
                t.data.op = (keypad_value)top_op;
                postfix[*postfix_index] = t;
                (*postfix_index)++;
            }
            else {break;}
        }
    }
}