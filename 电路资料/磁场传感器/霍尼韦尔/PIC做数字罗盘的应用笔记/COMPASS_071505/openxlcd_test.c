/* $Id: openxlcd.c,v 1.1 2003/12/09 22:52:09 GrosbaJ Exp $ */
#include <p18cxxx.h>
#include <xlcd.h>


/********************************************************************
*       Function Name:  OpenXLCD                                    *
*       Return Value:   void                                        *
*       Parameters:     lcdtype: sets the type of LCD (lines)       *
*       Description:    This routine configures the LCD. Based on   *
*                       the Hitachi HD44780 LCD controller. The     *
*                       routine will configure the I/O pins of the  *
*                       microcontroller, setup the LCD for 4- or    *
*                       8-bit mode and clear the display. The user  *
*                       must provide three delay routines:          *
*                       DelayFor18TCY() provides a 18 Tcy delay     *
*                       DelayPORXLCD() provides at least 15ms delay *
*                       DelayXLCD() provides at least 5ms delay     *
********************************************************************/
void OpenXLCD(unsigned char lcdtype)
{
        // The data bits must be either a 8-bit port or the upper or
        // lower 4-bits of a port. These pins are made into inputs                                

	DATA_PORT &= 0xf0;
	TRIS_DATA_PORT |= 0x0f;

	TRIS_RS = 0;
	TRIS_E = 0;
	RS_PIN = 0; 					// Register select pin made low
	E_PIN = 0;						// Clock pin made low
	
	DelayPORXLCD();					// Delay for 15ms to allow for LCD Power on reset

	// Setup interface to LCD
	TRIS_DATA_PORT &= 0xf0;
	DATA_PORT &= 0xf0;
	DATA_PORT |= 0b00000011;		// Function set cmd(4-bit interface)
	E_PIN = 1;						// Clock the cmd in
	DelayFor18TCY();
	E_PIN = 0;
	
	DelayXLCD();					// Delay for at least 4.1ms

	// Setup interface to LCD
	DATA_PORT &= 0xf0;				// Function set cmd(4-bit interface)
	DATA_PORT |= 0b00000011;
	E_PIN = 1;						// Clock the cmd in
	DelayFor18TCY();
	E_PIN = 0;

	DelayXLCD();					// Delay for at least 100us
	
	// Setup interface to LCD
	DATA_PORT &= 0xf0;				// Function set cmd(4-bit interface)
	DATA_PORT |= 0b00000011;
	E_PIN = 1;						// Clock cmd in
	DelayFor18TCY();
	E_PIN = 0;
	DelayXLCD();

	DATA_PORT &= 0xf0;				// Function set cmd(4-bit interface)
	DATA_PORT |= 0b00000010;
	E_PIN = 1;						// Clock cmd in
	DelayFor18TCY();
	E_PIN = 0;

	TRIS_DATA_PORT |= 0x0f;			// Make data nibble input

         	
    DelayXLCD();
    WriteCmdXLCD(lcdtype);          			// Set data interface width, # lines, font Function set cmd
   
 //   DelayXLCD();             					// Wait if LCD busy
 //   WriteCmdXLCD(DOFF&CURSOR_OFF&BLINK_OFF); 	// Display OFF/Blink OFF

    DelayXLCD();             		
    WriteCmdXLCD(DON&CURSOR_OFF&BLINK_OFF);   	// Display ON/Blink ON

    DelayXLCD();
    WriteCmdXLCD(0x01);             			// Clear display     
        
    DelayXLCD();
    WriteCmdXLCD(SHIFT_CUR_LEFT);   			// Set entry mode inc, no shift Entry Mode

    DelayXLCD();
    SetDDRamAddr(0);                			// Set Display data ram address to 0		
}

