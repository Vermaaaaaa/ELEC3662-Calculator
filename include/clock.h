#ifndef CLOCK_H  
#define CLOCK_H

// PLL Registers
#define SYSCTL_RIS_R           (*((volatile unsigned long *)0x400FE050)) 
#define SYSCTL_RCC_R           (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R          (*((volatile unsigned long *)0x400FE070))

// SysTick Register 
#define NVIC_ST_CTRL_R         (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R       (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R      (*((volatile unsigned long *)0xE000E018))

void SysTick_init();
void PLL_init();
void SysTick_wait(unsigned int delay);
void delay_ms(unsigned char delay);
void delay_us(unsigned char delay);

#endif 