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
        lcd_exe_instruction(DATA_INSTRUCTION, *string++);
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
            lcd_exe_instruction(COMMAND_INSTRUCTION, LCD_LINE1 + x);
            break;

        case 2:
            lcd_exe_instruction(COMMAND_INSTRUCTION, LCD_LINE2 + x);
            break;

        default:
            break;
    }
}

/***************************************************************************
 *
 *  Function Name: LCD Clear Display
 *  ********************************
 *
 *	Description:
 *	************
 * 		- This function used for clearing the display
 *        and returning the cursor home.
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

void lcd_clear_display() {
    lcd_exe_instruction(COMMAND_INSTRUCTION, LCD_HOME); // LCD display ON

    lcd_exe_instruction(COMMAND_INSTRUCTION, LCD_CLR); // LCD display ON
    _delay_ms(50);
}

/***************************************************************************
 *
 *  Function Name: LCD Execute Instruction
 *  ********************************
 *
 *	Description:
 *	************
 * 		- Used to send instructions (Data/Commands) to the LCD.
 *
 *	Parameters:
 *	***********
 *		- instruction_type: data or command.
 *      - Instruction: the data/command to excute.
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

void lcd_exe_instruction(LCD_INSTRUCTION_TYPE instruction_type, char instruction) {

    // processing high nibble
    LCD_PORT = (1 << LCD_EN) | (instruction_type << LCD_RS) | (((instruction & 16) >= 1) << LCD_DB4) |
               (((instruction & 32) >= 1) << LCD_DB5) | (((instruction & 64) >= 1) << LCD_DB6) |
               (((instruction & 128) >= 1) << LCD_DB7);

    LCD_PORT &= ~(1 << LCD_EN);

    // processing low nibble
    LCD_PORT = (1 << LCD_EN) | (instruction_type << LCD_RS) | (((instruction & 1) >= 1) << LCD_DB4) |
               (((instruction & 2) >= 1) << LCD_DB5) | (((instruction & 4) >= 1) << LCD_DB6) |
               (((instruction & 8) >= 1) << LCD_DB7);

    LCD_PORT &= ~(1 << LCD_EN);
    _delay_ms(1);
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

    _delay_ms(17);

    lcd_exe_instruction(COMMAND_INSTRUCTION, 0x02);   // Initialize LCD in 4-Bit mode
    lcd_exe_instruction(COMMAND_INSTRUCTION, 0x28);   // Initialize LCD in 2 lines, 5*8 fonts and 4-Bit mode
    lcd_exe_instruction(COMMAND_INSTRUCTION, LCD_ON); // LCD display ON

    lcd_clear_display();

    lcd_xy(5, 1);
    lcd_puts("Hello");
}
