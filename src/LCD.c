/****************************************************************************
    LCD.c  - Use an LMB162ABC based LCD with an Atmel ATmega processor

    Copyright (C) 2014 Ahmed Shaalan    (ahmed.bytes@yahoo.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

  ****************************************************************************
         File:    LCD.c
         Date:    October 28, 2014
  Last Update:    June 19, 2015
       Target:    ATmega32A
     Compiler:    avr-gcc
       Author:    Ahmed Shaalan

      Summary:    4-bit data interface, busy flag not implemented.
                  Any LCD pin can be connected to any available I/O port.
                  Includes a simple write string routine.

  ******************************* Program Notes ******************************

            This program uses a 4-bit data interface but does not use the
              busy flag to determine when the LCD controller is ready.  The
              LCD RW line is not connected to the uP and it must be
              connected to GND for the program to function.

            All time delays are longer than those specified in most datasheets
              in order to accommodate slower than normal LCD modules.  This
              requirement is well documented but almost always ignored.  The
              information is in a note at the bottom of the right hand
              (Execution Time) column of the instruction set.

  **************************************************************************/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "LCD.h"

/***************************************************************************
 *
 *  Enable Signal Function
 *  ***********************
 *
 *	 Write description if you can :D
 ***************************************************************************/

static void lcd_enable_signal(void) {
    LCD_PORT |= (1 << LCD_EN);
    // asm volatile ("nop");
    // asm volatile ("nop");
    _delay_ms(1);
    LCD_PORT &= ~(1 << LCD_EN);
}

/***************************************************************************
 *
 *  Function Name: Send Command Signal
 *  **********************************
 *
 *	Description:
 *	************
 *		- Configure LCD to receive a command
 *			- Set 'R/W' to Low (To Write)
 *			- Set 'RS' to Low  (To Send Command)
 *			- Send 'EN' Signal
 *
 *	Parameters:
 *	***********
 *		- None
 *
 *	Returns:
 *	********
 *		- None
 *
 *	Notes:
 *	******
 *		- None
 *
 ***************************************************************************/

static void lcd_cmd_signal(void) {
    LCD_PORT &= ~((1 << LCD_RW) | (1 << LCD_RS));
    lcd_enable_signal();
}

/***************************************************************************
 *
 *  Function Name: LCD Send Command
 *  ********************************
 *
 *	Description:
 *	************
 * 		- Send a specific command to the LCD
 *			- Mask higher 4-bits
 *			- Send to the LCD port
 *			- Send enable signal
 *			- Mask lower 4-bits
 *			- Send to LCD port
 *			- Send enable signal
 *
 *	Parameters:
 *	***********
 *		- Command: Commands
 *
 *	Returns:
 *	********
 *		- None
 *
 *	Notes:
 *	******
 *		- This Optimized for LCD Data to be on pin (4 - 7)
 *
 ***************************************************************************/

void lcd_cmd(unsigned char command) {

    // ---------------------- Processing High Nibble ----------------------//

    LCD_PORT = (((command & 16) >= 1) << LCD_DB4) | (((command & 32) >= 1) << LCD_DB5) |
               (((command & 64) >= 1) << LCD_DB6) | (((command & 128) >= 1) << LCD_DB7);

    lcd_cmd_signal();

    // ---------------------- Processing Low Nibble ----------------------//

    LCD_PORT = (((command & 1) >= 1) << LCD_DB4) | (((command & 2) >= 1) << LCD_DB5) |
               (((command & 4) >= 1) << LCD_DB6) | (((command & 8) >= 1) << LCD_DB7);

    lcd_cmd_signal();

    _delay_us(200);
}

/***************************************************************************
 *
 *  Function Name: Send Data Signal
 *  **********************************
 *
 *	Description:
 *	************
 * 		- Configure the LCD to receive data
 *			- Set 'R/W' to Low (To Write Mode)
 *			- Set 'RS' to Low  (Select Command Register)
 *			- Send 'EN' Signal
 *
 *	Parameters:
 *	***********
 *		- None
 *
 *	Returns:
 *	********
 *		- None
 *
 *	Notes:
 *	******
 *		- None
 *
 ***************************************************************************/

static void lcd_data_signal(void) {
    LCD_PORT &= ~(1 << LCD_RW);
    LCD_PORT |= (1 << LCD_RS);
    lcd_enable_signal();
}

/***************************************************************************
 *
 *  Function Name: LCD Display
 *  ********************************
 *
 *	Description:
 *	************
 * 		- Cut strings into single char and send it to the Display Helper
 *			- Loop for the Array of Characters (String)
 *			- Pass character by character to LCD_Display_Helper
 *
 *	Parameters:
 *	***********
 *		- Unsigned Character or Array of Characters "String"
 *
 *	Returns:
 *	********
 *		- None
 *
 *	Notes:
 *	******
 *		- None
 *
 ***************************************************************************/

void lcd_puts(char* string) {

    while (*string > 0)
        lcd_putc(*string++);
}

/***************************************************************************
 *
 *  Function Name: LCD Put Char
 *  ***************************
 *
 *	Description:
 *	************
 * 		- Processing chars and send it to the LCD to display them
 *			- Mask higher 4-bits
 *			- Send to the LCD port
 *			- Send enable signal
 *			- Mask lower 4-bits
 *			- Send to LCD port
 *			- Send enable signal
 *
 *	Parameters:
 *	***********
 *		- Unsigned Character or Array of Characters "String"
 *
 *	Returns:
 *	********
 *		- None
 *
 *	Notes:
 *	******
 *		- None
 *
 ***************************************************************************/

void lcd_putc(char aChar) {
    // char initData; // initialized Data Holder

    // ---------------------- Processing High Nibble ----------------------//

    LCD_PORT = (((aChar & 16) >= 1) << LCD_DB4) | (((aChar & 32) >= 1) << LCD_DB5) | (((aChar & 64) >= 1) << LCD_DB6) |
               (((aChar & 128) >= 1) << LCD_DB7);

    lcd_data_signal();

    // ---------------------- Processing Low Nibble ----------------------//

    LCD_PORT = (((aChar & 1) >= 1) << LCD_DB4) | (((aChar & 2) >= 1) << LCD_DB5) | (((aChar & 4) >= 1) << LCD_DB6) |
               (((aChar & 8) >= 1) << LCD_DB7);

    lcd_data_signal();

    _delay_us(200);
}

/***************************************************************************
 *
 *  Function Name: LCD Put Number
 *  *****************************
 *
 *	Description:
 *	************
 * 		- converts numbers to ASCII numbers
 *
 *	Parameters:
 *	***********
 *		- Unsigned Character or Array of Characters "String"
 *
 *	Returns:
 *	********
 *		- None
 *
 *	Notes:
 *	******
 *		- None
 *
 ***************************************************************************/

void lcd_putn(ubyte integer) {
    char buffer[6];
    sprintf(buffer, "%d", integer);
    lcd_puts(buffer);
}

/***************************************************************************
 *
 *  Function Name: LCD XY
 *  ********************************
 *
 *	Description:
 *	************
 * 		- Sets courser position
 *
 *	Parameters:
 *	***********
 *		- None
 *
 *	Returns:
 *	********
 *		- None
 *
 *	Notes:
 *	******
 *		- None
 *
 ***************************************************************************/

void lcd_xy(ubyte x, ubyte y) {

    switch (y) {
        case 1:
            lcd_cmd(LCD_LINE1 + x);
            break;

        case 2:
            lcd_cmd(LCD_LINE2 + x);
            break;

        default:
            break;
    }
}

/***************************************************************************
 *
 *  Function Name: LCD Initialize
 *  ********************************
 *
 *	Description:
 *	************
 * 		- Initialize the LCD to be ready for receiving data and commands
 *          - Wait for LCD to wakeup
 *          - Set Control PINs Output
 *          - Initialize LCD in 4-Bit mode
 *          - Initialize LCD in 2 lines, 5*8 fonts and 4-Bit mode
 *          - Reset DDRAM address to first line
 *          - LCD display ON
 *
 *	Parameters:
 *	***********
 *		- None
 *
 *	Returns:
 *	********
 *		- None
 *
 *	Notes:
 *	******
 *		- None
 *
 ***************************************************************************/

void lcd_init(void) {
    // wait for reset routine (some LCDs are 50ms)
    _delay_ms(17);

    LCD_DDR =
            ((1 << LCD_DB4) | (1 << LCD_DB5) | (1 << LCD_DB6) | (1 << LCD_DB7) | (1 << LCD_RS) | (1 << LCD_RW) |
             (1 << LCD_EN));

    lcd_cmd(0x02);   // Initialize LCD in 4-Bit mode
    lcd_cmd(0x28);   // Initialize LCD in 2 lines, 5*8 fonts and 4-Bit mode
    lcd_cmd(LCD_ON); // LCD display ON

    lcd_xy(4, 1);
    lcd_puts("Hello");
    lcd_xy(6, 2);
    lcd_puts("World!");
    _delay_ms(300);
}
