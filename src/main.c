#include "utility.h"
#include "clock.h"



int main(){
	PLL_init();
	SysTick_init();
	
	int gpio = GPIO_Init();
	if(gpio == ERROR){
		
	}
	return 0;
}
