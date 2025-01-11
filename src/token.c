#include "token.h"
#include "bool.h"
#include "utility.h"



int tokenise(keypad_value *input, token_t *tokens, unsigned char* token_index) {
    bool_t isDecimal = false;
    double current_number = 0.0;
    double decimal_div = 1.0;
    unsigned char index = 0;
		unsigned char i;

    for (i = 0; i < MAX_INPUT_BUFFER; i++) {
        keypad_value key = input[i];

        // Case 1: Digit
        if (key >= ZERO && key <= NINE) {
            unsigned char digit = key - ZERO;

            if (isDecimal) {
                decimal_div *= 10.0;
                current_number += digit / decimal_div;
            } else {
                current_number = current_number * 10.0 + digit;
            }
        }
        // Case 2: Decimal Point
        else if (key == DECIMAL) {
            if (isDecimal) {return DECIMAL_ERROR;}
            isDecimal = true;
        }
        // Case 3: Operator or End of Input
        else {
            if (current_number != 0.0 || isDecimal) {
                if (index >= MAX_TOKENS) {return TOKEN_LIMIT_ERROR;}
                tokens[index++] = (token_t){
                    .token_type = TOKEN_NUM,
                    .data.value = current_number
                };
                current_number = 0.0;
                isDecimal = false;
                decimal_div = 1.0;
            }

          
            if (key != NULL_VALUE) { 
                if (index >= MAX_TOKENS) {return TOKEN_LIMIT_ERROR;}
                tokens[index++] = (token_t){
                    .token_type = TOKEN_OP,
                    .data.op = key
                };
            }
        }
    }

   
    if (current_number != 0.0 || isDecimal) {
        if (index >= MAX_TOKENS) {return TOKEN_LIMIT_ERROR;}
        tokens[index++] = (token_t){
            .token_type = TOKEN_NUM,
            .data.value = current_number
        };
    }

    *token_index = index; 
    return SUCCESS;
}
