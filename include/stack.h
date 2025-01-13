#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE 32

#define STACK_SUCCESS 0
#define STACK_ERROR -1


typedef struct{
	int stk[MAX_STACK_SIZE];
	int top;
}stack;

int stack_init(stack *stk);
int stack_isEmpty(stack *stk);
int stack_push(stack *stk, int value);
int stack_pop(stack* stk);
int clear_stack(stack *stk);







#endif