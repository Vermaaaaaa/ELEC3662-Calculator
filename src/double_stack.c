#include "double_stack.h"

int double_stack_init_d(double_stack *s) {
    s->top = -1;
    return SUCCESS;
}

int double_stack_isEmpty_d(double_stack *s) {
    return (s->top == -1);
}

int double_stack_push_d(double_stack *s, double value) {
    if (s->top < MAX_DOUBLE_STACK_SIZE - 1) {
        s->stk[++(s->top)] = value;
        return SUCCESS;
    }
    return ERROR; // stack overflow
}

int double_stack_pop_d(double_stack *s, double *out_value) {
    if (!double_stack_isEmpty_d(s)) {
        *out_value = s->stk[s->top--];
        return SUCCESS;
    }
    return ERROR; // stack underflow
}