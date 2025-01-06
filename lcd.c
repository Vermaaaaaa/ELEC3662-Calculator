#include "lcd.h"
#include "clock.h"
#include "bool.h"
#include "utility.h"


static int lcd_sendNibble(unsigned char nibble, bool_t isData){
	switch(isData){
		case true:{
			GPIO_PORTA_DATA_R |= 0x08;
			break;
		}
		case false:{
			GPIO_PORTA_DATA_R &= ~0x08;
			break;
		}
		default:{
			return ERROR;
			break;
		}
	}
	
	GPIO_PORTB_DATA_R &= ~0x0F;
	
	GPIO_PORTB_DATA_R |= (nibble & 0x0F);
	
	delay_microsecond(1);
	
	GPIO_PORTA_DATA_R |= 0x04;
	delay_microsecond(1);
	GPIO_PORTA_DATA_R &= ~0x04;
	
	delay_microsecond(37);
	
	return SUCCESS;
	
}

static int lcd_sendByte(unsigned char byte, bool_t isData){
	
	lcd_sendNibble(byte >> 4, isData);
	lcd_sendNibble(byte & 0x0F, isData);
	
	switch(byte){
		case(CLEAR):{
			delay_millisecond(2);
			break;
		}
		case(RETURN_HOME):{
			delay_millisecond(2);
			break;
		}
		default:{
			delay_microsecond(40);
			break;
		}
		
	}
	
}

