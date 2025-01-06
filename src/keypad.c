#include "keypad.h"
#include "clock.h"
#include "utility.h"


void keypad_init(){
	
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
}


































