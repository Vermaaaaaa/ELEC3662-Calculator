#ifndef LCD_H
#define LCD_H

#include "bool.h"

#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_PUR_R        (*((volatile unsigned long *)0x40004510))
#define GPIO_PORTA_PDR_R        (*((volatile unsigned long *)0x40004514))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))

#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_PDR_R        (*((volatile unsigned long *)0x40005514))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

typedef enum {
    CLEAR = 0x01,               // Clear Display
    RETURN_HOME = 0x02,         // Return Home
    ENTRY_MODE_SET = 0x04,      // Entry Mode Set (Add I/D and S bits for customization)
    DISPLAY_CONTROL = 0x08,     // Display ON/OFF Control (Add D, C, B bits for customization)
    CURSOR_DISPLAY_SHIFT = 0x10, // Cursor or Display Shift (Add S/C and R/L bits for customization)
    FUNCTION_SET = 0x20,        // Function Set (Add DL, N, F bits for customization)
    SET_CGRAM_ADDR = 0x40,      // Set CGRAM Address
    SET_DDRAM_ADDR = 0x80       // Set DDRAM Address
} command_t;


int lcd_init(void);
int lcd_print(const char *str);
void lcd_clear();


static int lcd_sendNibble(unsigned char nibble, bool_t isData);




#endif