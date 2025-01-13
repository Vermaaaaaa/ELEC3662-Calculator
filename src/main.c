#include "utility.h"
#include "clock.h"
#include "lcd.h"
#include "keypad.h"
#include "token.h"
#include <stdio.h>
#include "conversion.h"
#include "calculator.h"

static void clear_all_buffers(keypad_value *input_buffer, unsigned char *input_count, token_t *infix_tokens, unsigned char *infix_count, token_t *postfix_tokens,unsigned char *postfix_count){
    for (int i = 0; i < MAX_INPUT_BUFFER; i++) {
        input_buffer[i] = NULL_VALUE;
    }
    for (int i = 0; i < MAX_TOKENS; i++) {
        infix_tokens[i].token_type = NULL_TOKEN;
        infix_tokens[i].data.value = 0.0;
    }
    for (int i = 0; i < MAX_TOKENS; i++) {
        postfix_tokens[i].token_type = NULL_TOKEN;
        postfix_tokens[i].data.value = 0.0;
    }

    *input_count   = 0;
    *infix_count   = 0;
    *postfix_count = 0;
		
		lcd_clear();
		
		lcd_setCursor(1,0);
}

static void reprint_input_buffer(const keypad_value *input_buffer,unsigned char input_count){
    lcd_clear();

    for (unsigned char i = 0; i < input_count; i++){
        const char* token_str = get_key_string(input_buffer[i]);
       
        lcd_print(token_str);
    }
}


static void remove_last_input(keypad_value *input_buffer, unsigned char *input_count){
    if (*input_count > 0) {
        *input_count -= 1;
        input_buffer[*input_count] = NULL_VALUE;
    }
		reprint_input_buffer(input_buffer, *input_count);
}


int main() {
  
	/*
	int pll = PLL_init();
	int tick = SysTick_init();
	int gpio = GPIO_init();	
	int lcd = lcd_init();
	int keypad = keypad_init();
	*/
	
	init();
	

	keypad_value  input_buffer[MAX_INPUT_BUFFER];
  token_t       infix_tokens[MAX_TOKENS];
  token_t       postfix_tokens[MAX_TOKENS];
  unsigned char input_count   = 0;
  unsigned char infix_count   = 0;
  unsigned char postfix_count = 0;


	while (1) {
				
				
       keypad_value key = get_key(); 
       if(key == NULL_VALUE){ continue; }
			 
			 if(key == CLEAR_INPUT){
				 clear_all_buffers(input_buffer, &input_count, infix_tokens, &infix_count, postfix_tokens, &postfix_count);
				 lcd_clear();
				 lcd_print("CLEARED");
				 delay_millisecond(500);
				 lcd_clear();
			 }
			 
			 if(key == EDIT){
				 if(input_count == 0){continue;}
				 remove_last_input(input_buffer, &input_count);
				 
			 }
			 
			 if(key == EQUALS){
				 if(input_count >= MAX_INPUT_BUFFER){
					 clear_all_buffers(input_buffer, &input_count, infix_tokens, &infix_count, postfix_tokens, &postfix_count);
					 lcd_clear();
					 lcd_print("Too many inputs");
					 delay_millisecond(500);
					 lcd_clear();
					 lcd_print("CLEARED");
					 delay_millisecond(500);
					 lcd_clear();
				 }
				
				 input_buffer[input_count] = NULL_VALUE;
				 unsigned char token_count = 0;
				 int tokenise_result = tokenise(input_buffer, infix_tokens, &token_count); 
				  if (tokenise_result != SUCCESS){
                lcd_clear();
                char buf[16];
                snprintf(buf, sizeof(buf), "TokErr:%d", tokenise_result);
                lcd_print(buf);
                delay_millisecond(1000);
								lcd_clear();
                clear_all_buffers(input_buffer, &input_count,infix_tokens, &infix_count,postfix_tokens, &postfix_count);
                continue;
            }
					
					
					infix_count = token_count;
					
					
					unsigned char pcount = 0;
          int conversion_result = infix_to_postfix(infix_tokens, infix_count,postfix_tokens, &pcount);
            if (conversion_result != SUCCESS){
                lcd_clear();
                char buf[16];
                snprintf(buf, sizeof(buf), "ConvErr:%d", conversion_result);
                lcd_print(buf);
                delay_millisecond(1000);
                lcd_clear();
                clear_all_buffers(input_buffer, &input_count,infix_tokens, &infix_count,postfix_tokens, &postfix_count);
                continue;
            }
					
            postfix_count = pcount;
						
						
						
						
						double result = 0.0;
            int eval_result = evaluate_postfix(postfix_tokens, postfix_count, &result);
            if (eval_result != EVAL_SUCCESS){
                lcd_clear();
                char buf[16];
                snprintf(buf, sizeof(buf), "EvalErr:%d", eval_result);
                lcd_print(buf);
                delay_millisecond(1000);
                lcd_clear();
                clear_all_buffers(input_buffer, &input_count,infix_tokens, &infix_count,postfix_tokens, &postfix_count);
                continue;
            }
						
						
            lcd_print("=");
            char rbuf[16];
            snprintf(rbuf, sizeof(rbuf), "%.4f", result);
            lcd_print(rbuf);
						continue;
						
						
				 }
			 
				 if (input_count < MAX_INPUT_BUFFER){
                input_buffer[input_count] = key;
                input_count++;
								lcd_print(get_key_string(key));
            }
				 
				  
			 }
			 
		
		return 0;
}



