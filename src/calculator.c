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

            if (double_stack_push_d(&val_stack, res) == ERROR) {
                return EVAL_ERROR_STACK_OVERFLOW;
            }
        }
       
        else if (t.token_type == TOKEN_OP) {
            double y, x;
          
            if (double_stack_pop_d(&val_stack, &y) == ERROR) {
                return EVAL_ERROR_STACK_UNDERFLOW;
            }
          
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
                        return EVAL_ERROR_DOMAIN;
                    }
                    res = x / y;
                    break;
                case POWER:

                    res = pow(x, y);
                    break;

                case SCINOT:
                    res = x * pow(10.0, y);
                    break;
                default:
                    return EVAL_ERROR_UNKNOWN_TOKEN;
            }

            if (double_stack_push_d(&val_stack, res) == ERROR) {
                return EVAL_ERROR_STACK_OVERFLOW;
            }
        }
        else {
            return EVAL_ERROR_UNKNOWN_TOKEN;
        }
    }


    if (val_stack.top != 0) {
      
        return EVAL_ERROR_EXTRA_OPERANDS;
    }

    double final_value;
    double_stack_pop_d(&val_stack, &final_value);
    *result_out = final_value; 
    return EVAL_SUCCESS;
}