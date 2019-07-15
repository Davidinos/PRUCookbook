#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

#pragma DATA_SECTION(init_pins, ".init_pins")
#pragma RETAIN(init_pins)
const char init_pins[] =  
	"/sys/class/leds/beaglebone:green:usr1/trigger\0none\0" \
	"/sys/class/leds/beaglebone:green:usr2/trigger\0none\0" \
	"/sys/class/leds/beaglebone:green:usr3/trigger\0none\0" \
	"\0\0";

volatile register unsigned int __R30;
volatile register unsigned int __R31;

void main(void) {
	int i;

	// uint32_t *gpio1 = (uint32_t *)GPIO1;
	// uint32_t *gpio2 = (uint32_t *)GPIO2;
	uint32_t *gpio3 = (uint32_t *)GPIO3;
	// uint32_t *gpio4 = (uint32_t *)GPIO4;
	uint32_t *gpio5 = (uint32_t *)GPIO5;
	// uint32_t *gpio6 = (uint32_t *)GPIO6;
	// uint32_t *gpio7 = (uint32_t *)GPIO7;
	// uint32_t *gpio8 = (uint32_t *)GPIO8;
	
	// Select which pins to toggle.  These are all on pru1_1
	uint32_t gpio = P9_14 | P9_16 | P8_15 | P8_16 | P8_26;

	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

	for(i=0; i<10; i++) {
		gpio5[GPIO_SETDATAOUT]   = USR1;			// The the USR3 LED on
		gpio3[GPIO_CLEARDATAOUT] = USR2;
		// gpio4[GPIO_SETDATAOUT]   = (1<<29);
		// gpio8[GPIO_SETDATAOUT]   = (1<<18);
		// gpio4[GPIO_SETDATAOUT]   = (1<<28);
		// gpio6[GPIO_SETDATAOUT]   = (1<<17);
		
		__R30 |= gpio;		// Set the GPIO pin to 1

		__delay_cycles(500000000/5);    // Wait 1/2 second

		gpio5[GPIO_CLEARDATAOUT] = USR1;
        gpio3[GPIO_SETDATAOUT]   = USR2;
		// gpio4[GPIO_CLEARDATAOUT] = (1<<29);
		// gpio8[GPIO_CLEARDATAOUT] = (1<<18);
		// gpio4[GPIO_CLEARDATAOUT] = (1<<28);
		// gpio6[GPIO_CLEARDATAOUT] = (1<<17);
		
		__R30 &= ~gpio;		// Clearn the GPIO pin

		__delay_cycles(500000000/5); 
		
		if((__R31&P8_19) == P8_19) {
            gpio3[GPIO_CLEARDATAOUT]   = USR3;      // Turn on LED
        } else
            gpio3[GPIO_SETDATAOUT]     = USR3;      // Turn off LED
	}
	__halt();
}
