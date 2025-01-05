#include "utility.h"
#include "clock.h"




int GPIO_Init(){
	volatile unsigned long delay;
	unsigned int counter  = 0;
	
	SYSCTL_RCGCGPIO_R |= 0x1B;
	delay = SYSCTL_RCGCGPIO_R;
	
	
	while((SYSCTL_PRGPIO_R & 0x1B) != 0x1B){
		delay_us(5);
		counter += 5;
		if(counter >= TIMEOUT){
			return ERROR;
		}
	}
	
	return SUCCESS;
	
}