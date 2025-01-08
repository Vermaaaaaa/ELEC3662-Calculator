#include "utility.h"
#include "clock.h"
#include "lcd.h"



int main() {
   
	int pll = PLL_init();
	int tick = SysTick_init();
	int gpio = GPIO_init();	
	int lcd = lcd_init();
	
	lcd_print("Hello World");
	
	
	

  

    return 0;
}
