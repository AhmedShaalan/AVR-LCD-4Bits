#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"

#define F_CPU 8000000UL // 8 MHz

void initIO(void) {
}

int main(void) {
    initIO();

    /* Initialize LCD 					*/
    lcd_init();

    while (1) {
    }
    return 0; // never reached
}
