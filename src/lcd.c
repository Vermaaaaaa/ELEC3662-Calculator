#include "lcd.h"
#include "clock.h"
#include "bool.h"
#include "utility.h"


// Define global variables to track cursor position
unsigned char current_row = 1;
unsigned char current_column = 0;

// Define row start addresses for a 20x4 LCD
static const unsigned char row_start_addresses[4] = { 0x00, 0x40, 0x14, 0x54 };


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
	current_row = 1;
	current_column = 0;
}

void lcd_setCursor(unsigned char row, unsigned char col)
{
    if (row < 1) row = 1;      
    if (row > 4) row = 4;
    if (col > 19) col = 19;    

    unsigned char base = row_start_addresses[row - 1];
    unsigned char addr = base + col;  

    lcd_sendByte(0x80 | addr, false);
}


int lcd_print(const char* str)
{
    while (*str) {
        lcd_sendByte(*str, true);
        current_column++;
        if (current_column >= 20) {
           
            current_column = 0;
            current_row++;
            if (current_row > 4) {
                current_row = 1;
            }
            lcd_setCursor(current_row, current_column);
        }
        str++;
    }
    return SUCCESS;
}


