#include "keypad.h"
#include "clock.h"
#include "utility.h"
#include "bool.h"

// Define layouts here
const keypad_value layout[4][4] = {
    {ONE, TWO, THREE, PLUS},
    {FOUR, FIVE, SIX, MINUS},
    {SEVEN, EIGHT, NINE, DECIMAL},
    {EQUALS, ZERO, EDIT, SHIFT}
};

const keypad_value shifted_layout[4][4] = {
    {SIN, COS, TAN, MULT},
    {ARCSIN, ARCCOS, ARCTAN, DIV},
    {PI, SQUARE, SQRT, SCINOT},
    {SAVE, CLEAR_INPUT, POWER, UNSHIFT}
};

const char *key_strings[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "+", "-", ".", "", "EDIT", "=", "SIN", "COS", "TAN", "x",
    "ARCSIN", "ARCCOS", "ARCTAN", "^2", "SQRT", "M+", "/",
    "E10", "", "CLEAR_INPUT", "^", "PI"
};


static int shiftstate = 0;


int keypad_init(){
	
    GPIO_PORTD_LOCK_R = 0x4C4F434B;      // Unlock GPIOCR register
    GPIO_PORTD_CR_R |= 0x0F;  // Allow changes to PD3-PD0
    GPIO_PORTD_AMSEL_R &= ~0x0F; // Disable analogue functionality
    GPIO_PORTD_PCTL_R &= ~0x0000FFFF;    // Set PD0-PD3 for GPIO function
    GPIO_PORTD_DIR_R |= 0x0F; // Set PD0-PD3 as outputs
    GPIO_PORTD_AFSEL_R &= ~0x0F; // Disable alternate functions
    GPIO_PORTD_DEN_R |= 0x0F; // Enable digital functionality for PD0-PD3

    
    GPIO_PORTE_LOCK_R = 0x4C4F434B;      // Unlock GPIOCR register
    GPIO_PORTE_CR_R |= 0x0F;  // Allow changes to PE3-PE0
    GPIO_PORTE_AMSEL_R &= ~0x0F; // Disable analogue functionality
    GPIO_PORTE_PCTL_R &= ~0x0000FFFF;    // Set PE0-PE3 for GPIO function
    GPIO_PORTE_DIR_R &= ~0x0F; // Set PE0-PE3 as inputs
    GPIO_PORTE_AFSEL_R &= ~0x0F; // Disable alternate functions
    GPIO_PORTE_PUR_R |= 0x0F;  // Enable internal pull-up resistors
    GPIO_PORTE_DEN_R |= 0x0F;  // Enable digital functionality for PE0-PE3
	
	return SUCCESS;
}




keypad_value get_key(void)
{
    unsigned char col, row;

    for (col = 0; col < 4; col++) {
        // Drive all columns high
        GPIO_PORTD_DATA_R |= 0x0F;
        // Pull this column low
        GPIO_PORTD_DATA_R &= ~(1 << col);

        delay_microsecond(2);

        for (row = 0; row < 4; row++) {
            if (!(GPIO_PORTE_DATA_R & (1 << row))) {
                // Debounce
                delay_millisecond(10);
                if (!(GPIO_PORTE_DATA_R & (1 << row))) {
                    // Wait for release
                    while (!(GPIO_PORTE_DATA_R & (1 << row))) {}
											
										if(shiftstate){
											keypad_value key = shifted_layout[row][col];
											if(key == UNSHIFT){
												shiftstate = !shiftstate;
                        return NULL_VALUE; 
											}
											return key;
										}
										else{
											keypad_value key = layout[row][col];
											if(key == SHIFT){
												shiftstate = !shiftstate;
                        return NULL_VALUE; 
											}
											return key;
										}
                }
            }
        }
    }
    return NULL_VALUE; 
}

const char *get_key_string(keypad_value key)
{
    if (key == NULL_VALUE) {
        return " ";
    }
    return key_strings[key];
}





































