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
            unsigned char digit = (unsigned char)(key - ZERO);
            if (!isDecimal) {
                // Building the integer part
                current_number = current_number * 10.0 + digit;
            } else {
                // Building the fractional part
                decimal_div *= 10.0;
                current_number += (digit / decimal_div);
            }
        }
        // ----------------------------------------------------------
        // Case 2: Decimal point
        // ----------------------------------------------------------
        else if (key == DECIMAL) {
            if (isDecimal) {
                // Already saw a decimal in this number => error
                return DECIMAL_ERROR;
            }
            isDecimal = true;
        }
        // ----------------------------------------------------------
        // Case 3: Operators (+, -, *, /, POWER, etc.)
        // ----------------------------------------------------------
        else if (is_operator(key)) {
            // Finalize current number, if any
            if (current_number != 0.0 || isDecimal) {
                tokens[index].token_type = TOKEN_NUM;
                tokens[index].data.value = current_number;
                tokens[index].token_index = index;
                index++;

                // Reset number accumulation
                current_number = 0.0;
                isDecimal = false;
                decimal_div = 1.0;
								prev_token = tokens[index - 1];
            }

            // **Important**: handle consecutive minus => plus, before unary check
            if (key == MINUS && index > 0
                && tokens[index - 1].token_type == TOKEN_OP
                && tokens[index - 1].data.op == MINUS)
            {
                // Turn the previous minus operator into plus
                tokens[index - 1].data.op = PLUS;
                // Skip adding a new token for the second minus
                // Move to the next iteration
                continue;
            }

            // Decide if minus is unary or binary
            if (key == MINUS &&
                (prev_token.token_type == NULL_TOKEN ||
                 prev_token.token_type == TOKEN_OP  ||
                 prev_token.token_type == TOKEN_FUNC))
            {
                // It's a unary minus
                tokens[index].token_type = TOKEN_FUNC;
                tokens[index].data.op = UNARY_MINUS;
                tokens[index].token_index = index;
                index++;
            }
            else {
                // Normal operator (e.g., plus, minus, multiply, etc.)
                tokens[index].token_type = TOKEN_OP;
                tokens[index].data.op = key;
                tokens[index].token_index = index;
                index++;
            }
        }
        // ----------------------------------------------------------
        // Case 4: Functions (SIN, COS, TAN, ARCSIN, etc.)
        // ----------------------------------------------------------
        else if (is_function(key)) {
            // Finalize current number, if any
            if (current_number != 0.0 || isDecimal) {
                tokens[index].token_type = TOKEN_NUM;
                tokens[index].data.value = current_number;
                tokens[index].token_index = index;
                index++;

                current_number = 0.0;
                isDecimal = false;
                decimal_div = 1.0;
            }

            // Add the function token
            tokens[index].token_type = TOKEN_FUNC;
            tokens[index].data.op = key;
            tokens[index].token_index = index;
            index++;
        }
        // ----------------------------------------------------------
        // Case 5: Any other key (SHIFT, CLEAR, etc.)
        // ----------------------------------------------------------
        else {
            // Finalize current number, if any
            if (current_number != 0.0 || isDecimal) {
                tokens[index].token_type = TOKEN_NUM;
                tokens[index].data.value = current_number;
                tokens[index].token_index = index;
                index++;

                current_number = 0.0;
                isDecimal = false;
                decimal_div = 1.0;
            }
            // You may choose to ignore the key or break out.
            // For now, let's just continue...
        }

        // Update prev_token if we have at least 1 token
        if (index > 0) {
            prev_token = tokens[index - 1];
        }

        // Check for token limit
        if (index >= MAX_TOKENS) {
            return TOKEN_LIMIT_ERROR;
        }
    }

    // If we ended while still building a number, finalize it
    if (current_number != 0.0 || isDecimal) {
        if (index >= MAX_TOKENS) {
            return TOKEN_LIMIT_ERROR;
        }
        tokens[index].token_type = TOKEN_NUM;
        tokens[index].data.value = current_number;
        tokens[index].token_index = index;
        index++;
    }

    // Return how many tokens we produced
    *token_count = index;
    return SUCCESS;
}

/*************************************************************************
 * Below are helper functions, same file or separate as you prefer
 *************************************************************************/

/**
 * @brief Checks if key is an operator like +, -, *, /, POWER, etc.
 */
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

/**
 * @brief Checks if key is a function like SIN, COS, TAN, ARCSIN, etc.
 */
static bool_t is_function(keypad_value key) {
    switch (key) {
        case SIN:
        case COS:
        case TAN:
        case ARCSIN:
        case ARCCOS:
        case ARCTAN:
        case SQUARE:
        case SQRT:
            return true;
        default:
            return false;
    }
}

/**
 * @brief Removes a token from the tokens array at the specified index.
 */
static void remove_token(token_t *tokens, unsigned char *token_count, unsigned char remove_index) {
    if (remove_index >= *token_count) {
        // Invalid index, do nothing
        return;
    }

    // Shift tokens left
    for (unsigned char i = remove_index; i < (*token_count) - 1; i++) {
        tokens[i] = tokens[i + 1];
    }

    // Decrease the token count
    (*token_count)--;
}
