#include "calculator.h"
#include <math.h>
#include "double_stack.h"




static double degToRad(double deg){
    return deg*(M_PI/180.0);
}

int evaluate_postfix(token_t *postfix, unsigned char postfix_count, double *result_out)
{
    
    double_stack val_stack;
    double_stack_init_d(&val_stack);

    
    for (unsigned char i = 0; i < postfix_count; i++) {
        token_t t = postfix[i];

        if (t.token_type == TOKEN_NUM) {
           
            if (double_stack_push_d(&val_stack, t.data.value) == ERROR) {
                return EVAL_ERROR_STACK_OVERFLOW;
            }
        }

        else if (t.token_type == TOKEN_FUNC) {
            double x;
            if (double_stack_pop_d(&val_stack, &x) == ERROR) {
                return EVAL_ERROR_STACK_UNDERFLOW;
            }

            double res = 0.0;
            switch (t.data.op) {
                case UNARY_MINUS:
                    res = -x;
                    break;
                case SIN:
                    x = degToRad(x);
                    res = sin(x);
                    break;
                case COS:
										if(x < EPSILON){ return EVAL_ERROR_DOMAIN; }
										 x = degToRad(x);
                    res = cos(x);
                    break;
                case TAN:
										 x = degToRad(x);
                    res = tan(x);
                    break;
                case SQRT:
                    if (x < 0.0) {
                        return EVAL_ERROR_DOMAIN;
                    }
                    res = sqrt(x);
                    break;
                case SQUARE:
                    res = x * x;
                    break;
                case ARCSIN:
										x = degToRad(x);
										res = asin(x);
										break;
								case ARCCOS:
										x = degToRad(x);
										res = acos(x);
										break;
								case ARCTAN:
										x = degToRad(x);
										res = atan(x);
										break;
								
                default:
                    return EVAL_ERROR_UNKNOWN_TOKEN; 
            }

            // push result
            if (double_stack_push_d(&val_stack, res) == ERROR) {
                return EVAL_ERROR_STACK_OVERFLOW;
            }
        }
        // CASE C: If it's an operator (binary) => pop 2 operands
        else if (t.token_type == TOKEN_OP) {
            double y, x;
            // pop y
            if (double_stack_pop_d(&val_stack, &y) == ERROR) {
                return EVAL_ERROR_STACK_UNDERFLOW;
            }
            // pop x
            if (double_stack_pop_d(&val_stack, &x) == ERROR) {
                return EVAL_ERROR_STACK_UNDERFLOW;
            }

            double res = 0.0;
            switch (t.data.op) {
                case PLUS:
                    res = x + y;
                    break;
                case MINUS:
                    res = x - y;
                    break;
                case MULT:
                    res = x * y;
                    break;
                case DIV:
                    if (fabs(y) < 1e-12) {
                        // potential divide by zero
                        return EVAL_ERROR_DOMAIN;
                    }
                    res = x / y;
                    break;
                case POWER:
                    // x^y
                    // watch out for x=0 and y<=0 => domain error?
                    // or negative x with fractional y => domain error
                    // for now assume user is only using integer exponents or valid domain
                    res = pow(x, y);
                    break;
                // SCINOT => interpret as x * 10^y, if that's how you do it
                case SCINOT:
                    // x E y => x * 10^y
                    res = x * pow(10.0, y);
                    break;
                default:
                    return EVAL_ERROR_UNKNOWN_TOKEN;
            }

            // push result
            if (double_stack_push_d(&val_stack, res) == ERROR) {
                return EVAL_ERROR_STACK_OVERFLOW;
            }
        }
        else {
            // Unknown token type
            return EVAL_ERROR_UNKNOWN_TOKEN;
        }
    }

    // 3) After processing all tokens, we should have exactly one item in the stack
    if (val_stack.top != 0) {
        // If we have >1, there's leftover. If we have <0, we already returned error.
        return EVAL_ERROR_EXTRA_OPERANDS;
    }

    double final_value;
    double_stack_pop_d(&val_stack, &final_value);
    *result_out = final_value; // Put the final result out
    return EVAL_SUCCESS;
}