#include "utility.h"
#include "clock.h"

int GPIO_init(){
	volatile unsigned long delay;
	unsigned int counter  = 0;
	
	SYSCTL_RCGCGPIO_R |= 0x1B;
	delay = SYSCTL_RCGCGPIO_R;
	
	
	while((SYSCTL_PRGPIO_R & 0x1B) != 0x1B){
		delay_microsecond(5);
		counter += 5;
		if(counter >= TIMEOUT){
			return ERROR;
		}
	}
	
	return SUCCESS;
	
}

static void triggerSystemReset() {
    // Trigger a system reset by writing to NVIC_APINT_R
    NVIC_APINT_R = 0x05FA0004; // Write key (0x05FA) with SysResetReq (bit 8) set
}

void init(){
	unsigned char retries = 0;
  unsigned char counter = 0;

    while (retries < MAX_RETRIES) {
        counter = 0; 

        // Initialize PLL
        int pll = PLL_init();
        if (pll == ERROR) {
            counter++;
        }

        // Initialize SysTick
        int systick = SysTick_init();
        if (systick == ERROR) {
            counter++;
        }

        // Initialize GPIO
        int gpio = GPIO_init();
        if (gpio == ERROR) {
            counter++;
        }

        // Check if all initializations succeeded
        if (counter == 0) {
            break;
        }

        retries++;

        if (retries >= MAX_RETRIES) {
            while (1) {
                // Infinite loop to halt the program
            }
        }

        // Trigger a system reset to try again
        triggerSystemReset();
    }
}