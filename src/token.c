#include "token.h"
#include "bool.h"
#include "utility.h"



int tokenise(keypad_value *input, token_t *tokens, unsigned char* token_index){
	bool_t isDecimal = false;
	double current_number  = 0.0;
	unsigned char index = 0;
	double decimal_div = 1;
	
	unsigned char i;
	
	for(i = 0; i < MAX_INPUT_BUFFER; i++){
		keypad_value key = input[i];
		
		if(key >= ZERO && key <= NINE){
			unsigned char digit = key - ZERO;
			
			if(isDecimal == true){
				decimal_div *= 10;
				current_number += digit / decimal_div;
			}
			else{
				current_number = current_number * 10 + digit; 
			}
		}
		
		if(key == DECIMAL){
			if(isDecimal == true){ return DECIMAL_ERROR; }
			isDecimal = true;
			
		}
	}
	
}