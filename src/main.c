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
  token_t tokens[MAX_TOKENS];
  unsigned char token_index = 0;
	unsigned char input_buffer_index = 0;

    

	
	
	

	while (1) {
				
       keypad_value key = get_key(); 
       if(key == NULL_VALUE){ continue; }
			 
			 if(key == EQUALS){
				 input_buffer[input_buffer_index] = NULL_VALUE;
				 int tokenise_result = tokenise(input_buffer, tokens, &token_index);
				 
			 }
			 
			 input_buffer[input_buffer_index] = key;
			 input_buffer_index++;
			
			 const char *str = get_key_string(key);
				
				lcd_print(str);
			 
			 
		
		
				
				
		
		
	}
		
		
	
		return 0;
}
