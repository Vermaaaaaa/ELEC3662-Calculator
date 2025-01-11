#ifndef UTILITY_H
#define UTILITY_H

#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
	
#define NVIC_APINT_R (*((volatile unsigned long *)0xE000ED0C))
	
#define SUCCESS 0
#define ERROR   -1
#define TIMEOUT      1000 
#define MAX_RETRIES 3
#define MAX_INPUT_BUFFER 128

#define DECIMAL_ERROR -2

int GPIO_init();
static void triggerSystemReset();

void init();

#endif