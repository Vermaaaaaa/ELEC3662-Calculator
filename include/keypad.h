#ifndef KEYPAD_H
#define KEYPAD_H

#include "bool.h"

// GPIO Port D Registers (Keypad Columns)
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_PDR_R        (*((volatile unsigned long *)0x40007514))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))

// GPIO Port E Registers (Keypad Rows)
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_PDR_R        (*((volatile unsigned long *)0x40024514))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

typedef enum {
    ZERO = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    PLUS = 10,
    MINUS = 11,
    DECIMAL = 12,
    SHIFT = 13,
    EDIT = 14,
    EQUALS = 15,
    SIN = 16,
    COS = 17,
    TAN = 18,
    MULT = 19,
    ARCSIN = 20,
    ARCCOS = 21,
    ARCTAN = 22,
    SQUARE = 23,
    SQRT = 24,
    SAVE = 25,
    DIV = 26,
    SCINOT = 27,
    UNSHIFT = 28,
    CLEAR_INPUT = 29,
    POWER = 30,
    PI = 31,
		UNARY_MINUS = 32, 
    NULL_VALUE = 33
} keypad_value;



keypad_value get_key();
int keypad_init();
char* unshiftedToString(char value);
const char *get_key_string(keypad_value key);


extern const keypad_value layout[4][4];

extern const keypad_value shifted_layout[4][4]; 
 

#endif