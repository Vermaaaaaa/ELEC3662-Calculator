#include "token.h"
#include "bool.h"
#include "utility.h"
#include "conversion.h"

static bool_t is_operator(keypad_value key);
static bool_t is_function(keypad_value key);
static void remove_token(token_t *tokens, unsigned char *token_count, unsigned char remove_index);


int tokenise(keypad_value *input, token_t *tokens, unsigned char *token_count) {
    bool_t isDecimal = false;     
    double current_number = 0.0; 
    double decimal_div = 1.0;      
    token_t prev_token = { .token_type = NULL_TOKEN };
    unsigned char index = 0;       

   
    for (unsigned char i = 0; i < MAX_INPUT_BUFFER; i++) {
        keypad_value key = input[i];

      
        if (key == NULL_VALUE) {
            break;
        }

       
        if (key >= ZERO && key <= NINE) {
            unsigned char digit = (unsigned char)(key - 0);
            if (!isDecimal) {
                current_number = current_number * 10.0 + digit;
            } else {
   
                decimal_div *= 10.0;
                current_number += (digit / decimal_div);
            }
        }
    
        else if (key == DECIMAL) {
            if (isDecimal) {

                return DECIMAL_ERROR;
            }
            isDecimal = true;
        }

        else if (is_operator(key)) {

            if (current_number != 0.0) {
                tokens[index].token_type = TOKEN_NUM;
                tokens[index].data.value = current_number;
                tokens[index].token_index = index;
                index++;

          
                current_number = 0.0;
                isDecimal = false;
                decimal_div = 1.0;
								prev_token = tokens[index - 1];
            }

            if (key == MINUS && index > 0 && tokens[index - 1].token_type == TOKEN_OP&& tokens[index - 1].data.op == MINUS){
                tokens[index - 1].data.op = PLUS;
                continue;
            }

            
            if (key == MINUS && (prev_token.token_type == NULL_TOKEN ||prev_token.token_type == TOKEN_OP  ||prev_token.token_type == TOKEN_FUNC)){
                tokens[index].token_type = TOKEN_FUNC;
                tokens[index].data.op = UNARY_MINUS;
                tokens[index].token_index = index;
                index++;
            }
            else {
                tokens[index].token_type = TOKEN_OP;
                tokens[index].data.op = key;
                tokens[index].token_index = index;
                index++;
            }
        }

        else if (is_function(key)) {
            if (current_number != 0.00) {
                tokens[index].token_type = TOKEN_NUM;
                tokens[index].data.value = current_number;
                tokens[index].token_index = index;
                index++;

                current_number = 0.0;
                isDecimal = false;
                decimal_div = 1.0;
            }


            tokens[index].token_type = TOKEN_FUNC;
            tokens[index].data.op = key;
            tokens[index].token_index = index;
            index++;
        }

        else {

            if (current_number != 0.0) {
                tokens[index].token_type = TOKEN_NUM;
                tokens[index].data.value = current_number;
                tokens[index].token_index = index;
                index++;

                current_number = 0.0;
                isDecimal = false;
                decimal_div = 1.0;
            }

        }

        if (index > 0) {
            prev_token = tokens[index - 1];
        }
        if (index >= MAX_TOKENS) {
            return TOKEN_LIMIT_ERROR;
        }
    }


    if (current_number != 0.0) {
        if (index >= MAX_TOKENS) {
            return TOKEN_LIMIT_ERROR;
        }
        tokens[index].token_type = TOKEN_NUM;
        tokens[index].data.value = current_number;
        tokens[index].token_index = index;
        index++;
    }

    *token_count = index;
    return SUCCESS;
}



static bool_t is_operator(keypad_value key) {
    switch (key) {
        case PLUS:
        case MINUS:
        case MULT:
        case DIV:
        case POWER:
        case SCINOT:
            return true;
        default:
            return false;
    }
}


static bool_t is_function(keypad_value key) {
    switch (key) {
        case SIN: return true;
        case COS: return true;
        case TAN: return true;
        case ARCSIN: return true;
        case ARCCOS: return true;
        case ARCTAN: return true;
        case SQUARE: return true;
        case SQRT: return true;
        default: return false;
    }
}


static void remove_token(token_t *tokens, unsigned char *token_count, unsigned char remove_index) {
    if (remove_index >= *token_count) {
        return;
    }

    for (unsigned char i = remove_index; i < (*token_count) - 1; i++) {
        tokens[i] = tokens[i + 1];
    }


    (*token_count)--;
}
