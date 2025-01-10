#include "stack.h"
#include "utility.h"

int stack_init(stack *stk){
	stk->top = -1;
	return SUCCESS;
}

int stack_isEmpty(stack *stk){
	return stk->top == -1;
}

int stack_push(stack *stk, int value){
	if(stk->top < MAX_STACK_SIZE - 1){
		stk->stk[++stk->top] = value;
		return SUCCESS;
	}
	else{
		return ERROR;
	}
}

int stack_pop(stack *stk){
	if(!stack_isEmpty(stk)){
		return stk->stk[stk->top--];
	}
	else{
		return ERROR;
	}
}

int clear_stack(stack *stk){
	stk->top = -1;
	return SUCCESS;
}


