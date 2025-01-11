#include "utility.h"
#include "clock.h"
#include "lcd.h"
#include "keypad.h"
#include "token.h"






int main() {
   
	int pll = PLL_init();
	int tick = SysTick_init();
	int gpio = GPIO_init();	
	int lcd = lcd_init();
	int keypad = keypad_init();
	
	keypad_value input_buffer[MAX_INPUT_BUFFER];
	unsigned char buffer_index = 0;
	
	

	while (1) {
        keypad_value key = get_key(); 
        if(key == NULL_VALUE){
					continue;
				}
				
				if(buffer_index > MAX_INPUT_BUFFER){					
					continue;
				}
				
				input_buffer[buffer_index] = key;
				buffer_index++;
				
				const char *str = get_key_string(key);
				lcd_print(str);
    }
	
		return 0;
}
