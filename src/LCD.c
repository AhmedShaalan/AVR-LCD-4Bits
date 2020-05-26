/****************************************************************************
    LCD.c  - Use an LMB162ABC based LCD with an Atmel ATmega processor

    Copyright (C) 2014 Ahmed Shaalan    (ahmed_shaalan@icloud.com)

  ****************************************************************************
         File:    LCD.c
         Date:    October 28, 2014
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
 *  Function Name: LCD Put String
 *  ********************************
 *
 *	Description:
 *	************
 * 		- Cut strings into single char and send them one by one
 *
 *	Parameters:
 *	***********
 *		- Unsigned Character or Array of Characters "String"
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
 ***************************************************************************/

void lcd_putn(ubyte integer) {
    char buffer[6];
    sprintf(buffer, "%d", integer);
    lcd_puts(buffer);
}

/***************************************************************************
 *
 *  Function Name: LCD Move Cursor
 *  ********************************
 *
 *	Description:
 *	************
 * 		- Sets courser position
 *
 ***************************************************************************/

void lcd_move_cursor(ubyte x, ubyte y) {

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
 *s
 ***************************************************************************/

void lcd_clear_display(void) {
    lcd_exe_instruction(COMMAND_INSTRUCTION, LCD_HOME);

    lcd_exe_instruction(COMMAND_INSTRUCTION, LCD_CLR);
    _delay_ms(5);
}

/***************************************************************************
 *
 *  Function Name: LCD Cursor Home
 *  ********************************
 *
 *	Description:
 *	************
 * 		- move cursor to the first position.
 *
 ***************************************************************************/

void lcd_cursor_home(void) {
    lcd_exe_instruction(COMMAND_INSTRUCTION, LCD_HOME);
    _delay_ms(5);
}

/***************************************************************************
 *
 *  Function Name: LCD Display Control
 *  ********************************
 *
 *	Description:
 *	************
 * 		- Used to set display criteria, such as turning display on/off, curse on/off
 *        and if the cursor should blink.
 *
 *	Parameters:
 *	***********
 *		- display_status: display on/off.
 *		- cursor_status: cursor on/off.
 *		- cursor_blink: cursor blink on/off.
 *
 ***************************************************************************/

void lcd_display_control(LCD_DISPLAY display_status, LCD_CURSOR cursor_status, LCD_BLINK cursor_blink) {
    lcd_exe_instruction(COMMAND_INSTRUCTION, (display_status | cursor_status | cursor_blink));
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
 ***************************************************************************/

void lcd_init(void) {
    // wait for reset routine (some LCDs are 50ms)
    _delay_ms(17);

    LCD_DDR =
            ((1 << LCD_DB4) | (1 << LCD_DB5) | (1 << LCD_DB6) | (1 << LCD_DB7) | (1 << LCD_RS) | (1 << LCD_RW) |
             (1 << LCD_EN));

    _delay_ms(17);

    // Initialize LCD in 4-Bit mode
    lcd_exe_instruction(COMMAND_INSTRUCTION, 0x02);

    // Initialize LCD in 2 lines, 5*8 fonts and 4-Bit mode
    lcd_exe_instruction(COMMAND_INSTRUCTION, 0x28);

    lcd_display_control(LCD_DISPLAY_ON, LCD_CURSOR_OFF, LCD_BLINK_OFF);
    lcd_clear_display();

    lcd_move_cursor(5, 1);
    lcd_puts("Hello!");
}
