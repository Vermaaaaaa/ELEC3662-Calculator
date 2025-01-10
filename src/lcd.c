#include "lcd.h"
#include "clock.h"
#include "bool.h"
#include "utility.h"

static int PortA_B_init(){

    GPIO_PORTA_LOCK_R = 0x4C4F434B;      
    GPIO_PORTA_CR_R |= 0x0C;             
    GPIO_PORTA_AMSEL_R &= ~0x0C;         
    GPIO_PORTA_PCTL_R &= ~0x0000FF00;    
    GPIO_PORTA_DIR_R |= 0x0C;            
    GPIO_PORTA_AFSEL_R &= ~0x0C;        
    GPIO_PORTA_DEN_R |= 0x0C;            

	
    GPIO_PORTB_LOCK_R = 0x4C4F434B;      
    GPIO_PORTB_CR_R |= 0x0F;             
    GPIO_PORTB_AMSEL_R &= ~0x0F;         
    GPIO_PORTB_PCTL_R &= ~0x0000FFFF;    
    GPIO_PORTB_DIR_R |= 0x0F;            
    GPIO_PORTB_AFSEL_R &= ~0x0F;         
    GPIO_PORTB_DEN_R |= 0x0F;            
	
	return SUCCESS;
}


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
	
	delay_microsecond(50);
	
	return SUCCESS;
	
}

static int lcd_sendByte(command_t byte, bool_t isData){
	
	lcd_sendNibble(byte >> 4, isData);
	delay_microsecond(50);
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
	
	int ports = PortA_B_init();
	
	delay_millisecond(20);
	
	GPIO_PORTA_DATA_R &= ~(0x08 | 0x04);
	
	lcd_sendNibble(0x03, false);
	delay_millisecond(6);
	lcd_sendNibble(0x03, false);
	delay_microsecond(160);
	lcd_sendNibble(0x03, false);
	
	
	lcd_sendNibble(0x02, false);
	
	
	lcd_sendByte(FUNCTION_SET | 0x08, false);
	
	//Display OFF
	lcd_sendByte(DISPLAY_CONTROL & ~0x04, false); 
	
	lcd_sendByte(CLEAR, false);
	 
	lcd_sendByte(ENTRY_MODE_SET | 0x02 , false);
	
	lcd_sendByte(DISPLAY_CONTROL | 0x07, false);
	
	return SUCCESS;

}

void lcd_clear(){
	lcd_sendByte(CLEAR, false);
}




int lcd_print(const char* str){
	while(*str){
		lcd_sendByte(*str++, true);
	}
	return SUCCESS;
}


