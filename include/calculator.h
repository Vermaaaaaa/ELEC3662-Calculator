#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "token.h"

#define EPSILON 0.001

#define M_PI 3.14159265358979323846

#define EVAL_SUCCESS               0
#define EVAL_ERROR_STACK_UNDERFLOW -1
#define EVAL_ERROR_STACK_OVERFLOW  -2
#define EVAL_ERROR_DOMAIN          -3
#define EVAL_ERROR_UNKNOWN_TOKEN   -4
#define EVAL_ERROR_EXTRA_OPERANDS  -5



int evaluate_postfix(token_t *postfix, unsigned char postfix_count, double *result_out);


#endif