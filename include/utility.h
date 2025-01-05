#ifndef UTILITY_H
#define UTILITY_H

#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
	
#define SUCCESS 0
#define ERROR   -1
#define TIMEOUT      1000 

int GPIO_Init();

#endif