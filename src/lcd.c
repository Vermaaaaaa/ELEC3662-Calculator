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
			delay_microsecond(50);
			break;
		}
		
	}
	
	return SUCCESS;
	
}

int lcd_init(void){
	
	delay_millisecond(60);
	
	GPIO_PORTA_DATA_R &= ~(0x08 | 0x04);
	
	lcd_sendNibble(0x03, false);
	delay_millisecond(5);
	lcd_sendNibble(0x03, false);
	delay_microsecond(150);
	lcd_sendNibble(0x03, false);
	delay_millisecond(5);
	
	lcd_sendNibble(0x02, false);
	delay_millisecond(1);
	
	lcd_sendByte(FUNCTION_SET | 0x08, false);
	delay_millisecond(1);
	
	lcd_sendByte(DISPLAY_CONTROL, false);
	delay_millisecond(1);
	
	lcd_sendByte(CLEAR, false);
	delay_millisecond(2);
	
	lcd_sendByte(ENTRY_MODE_SET | 0x02, false);
	delay_millisecond(1);
	
	lcd_sendByte(DISPLAY_CONTROL | 0x04, false);
	delay_millisecond(1);

	//TEMP
	lcd_sendByte(0x80, false);
	delay_millisecond(1);
	
	return SUCCESS;

	
}



