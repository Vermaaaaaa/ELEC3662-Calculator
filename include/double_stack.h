#ifndef DOUBLE_STACK_H
#define DOUBLE_STACK_H

#include "utility.h" 


#define MAX_DOUBLE_STACK_SIZE 32

typedef struct {
    double stk[MAX_DOUBLE_STACK_SIZE];
    int top;
} double_stack;

int double_stack_init_d(double_stack *s);

int double_stack_isEmpty_d(double_stack *s);

int double_stack_push_d(double_stack *s, double value);

int double_stack_pop_d(double_stack *s, double *out_value);

#endif
