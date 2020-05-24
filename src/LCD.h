/*
 * LCD.h
 *
 *	4-bit LCD Interface
 *  Date Created: 28.10.2014s
 * 		Hardware: LCD 16x2
 *        Author: Ahmed_Shaalan
 *
 * 		   Notes: The data pins should be PIN (4 - 7):
 *					P0 - No connect (NC)
 *					P1 - Enable (E)
 *					P2 - Read/Write_L (R/W_L)
 *					P3 - Register Select (RS)
 *					P4 - Data 4 (DB4)
 *					P5 - Data 5 (DB5)
 *					P6 - Data 6 (DB6)
 *					P7 - Data 7 (DB7)
 *
 */


#ifndef _LCD_H_
#define _LCD_H_

#include "StdTypes.h"

/* PORTs Direction	 									*/
#define LCD_PORT 		PORTB 		// LCD PORT
#define LCD_DDR 		DDRB 		// LCD DDR
/* PORTs Direction	 									*/

/* Controll  PINs	 									*/
#define LCD_RS				0 		// PORTA.1
#define LCD_RW				2 		// PORTA.2
#define LCD_EN				1		// PORTA.3
/* Controll  PINs	 									*/

/* Data PINs	 										*/
#define LCD_DB4				4		// PORTA.4
#define LCD_DB5				5		// PORTA.5
#define LCD_DB6				6		// PORTA.6
#define LCD_DB7 			7		// PORTA.7
/* Data PINs	 										*/

/* LCD Commands 										*/
#define LCD_CLR 		 	0x01 	// Clear Display
#define LCD_HOME		 	0x02	// Return Home
#define LCD_NSHIFT	 		0x04	// No Shift
#define LCD_RSHIFT	 		0x07 	// Right Shift ***
#define LCD_LSHIFT		 	0x05	// Left Shift ***
#define LCD_ON 	 	        0x0C	// LCD ON
#define LCD_OFF 		 	0x08    // LCD OFF
// #define LCD_CURSOR_ON	 	0x0E 	// Cursor ON
// #define LCD_CURSOR_OFF	 	0x0C	// Cursor OFF
// #define LCD_BLINK_ON	 	0x0F 	// Cursor Blink ON
// #define LCD_BLINK_OFF	 	0x0C 	// Cursor Blink OFF
#define LCD_LINE1           0x80    // Go to 1st line
#define LCD_LINE2           0xC0    // Go to 2nd line
#define LCD_CGRAM           0x40    // CGRAM start address
#define LCD_DDRAM           0x80    // DDRAM start address
/* LCD Commands 										*/

typedef enum  {
	DATA_INSTRUCTION = 1,
	COMMAND_INSTRUCTION = 0
}LCD_INSTRUCTION_TYPE;

typedef enum {
    LCD_DISPLAY_OFF = 0x00,
    LCD_DISPLAY_ON = 0x0C
} LCD_DISPLAY;

typedef enum {
    LCD_CURSOR_OFF = 0x00,
    LCD_CURSOR_ON = 0x0A
} LCD_CURSOR;

typedef enum {
    LCD_BLINK_OFF = 0x00,
    LCD_BLINK_ON = 0x09
} LCD_BLINK;

/* LCD Functions 										*/
void lcd_init (void);
void lcd_puts (char *string);
void lcd_putn (ubyte integer);
void lcd_exe_instruction(LCD_INSTRUCTION_TYPE instruction_type, char instruction);
void lcd_move_cursor(ubyte x, ubyte y);
void lcd_clear_display(void);
void lcd_cursor_home(void);
void lcd_display_control(LCD_DISPLAY display_status, LCD_CURSOR cursor_status, LCD_BLINK cursor_blink);
/* LCD Functions 										*/

#endif /* _LCD_H_ */
