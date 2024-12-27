#include "clock.h"

void SysTick_Init(){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}

void PLL_Init(){
  
  SYSCTL_RCC2_R |=  0x80000000;  

  SYSCTL_RCC2_R |=  0x00000800;  
  
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~ 0x000007C0) + 0x00000540;
	
  SYSCTL_RCC2_R &= ~0x00000070;  
  
	SYSCTL_RCC2_R &= ~0x00002000;
  
  SYSCTL_RCC2_R |= 0x40000000;
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  + (4<<22);      
	
  while((SYSCTL_RIS_R&0x00000040)==0){};  

  SYSCTL_RCC2_R &= ~0x00000800;
}


void SysTick_Wait(unsigned int delay){
  NVIC_ST_RELOAD_R = delay-1;  
  NVIC_ST_CURRENT_R = 0;       
  while((NVIC_ST_CTRL_R&0x00010000)==0){};
}

void delay_ms(unsigned char delay){
	unsigned char i;
	
	for(i = 0; i < delay; i++){
		SysTick_Wait(80000);
	}
	
}
void delay_us(unsigned char delay){
	unsigned char i;
	
	for(i = 0; i < delay; i++){
		SysTick_Wait(80);
	}
	
}