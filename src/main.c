#include "utility.h"
#include "clock.h"
#include "lcd.h"
#include "keypad.h"
#include "token.h"


#define NULL 0


int main() {
   
	int pll = PLL_init();
	int tick = SysTick_init();
	int gpio = GPIO_init();	
	int lcd = lcd_init();
	int keypad = keypad_init();
	

	
	
	
	while (1) {
        keypad_value key = get_key(); 
        if(key == NULL_VALUE){
					continue;
				}
				const char *str = get_key_string(key);
				lcd_print(str);
    }
	
		return 0;
}
