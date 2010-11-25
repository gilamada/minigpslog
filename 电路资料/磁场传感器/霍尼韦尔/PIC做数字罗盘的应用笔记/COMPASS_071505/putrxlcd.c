/* $Id: putrxlcd.c,v 1.1 2003/12/09 22:52:09 GrosbaJ Exp $ */
#include <p18cxxx.h>
#include <xlcd.h>

// 3/30/05 This file has been modified form the original putrxlcd.c in the mcc18 library.


/********************************************************************
*       Function Name:  putrsXLCD
*       Return Value:   void
*       Parameters:     buffer: pointer to string
*       Description:    This routine writes a string of bytes to the
*                       Hitachi HD44780 LCD controller. The user
*                       must check to see if the LCD controller is
*                       busy before calling this routine. The data
*                       is written to the character generator RAM or
*                       the display data RAM depending on what the
*                       previous SetxxRamAddr routine was called.
********************************************************************/
void putrsXLCD(const rom char *buffer)
{
        while(*buffer)                  	// Write data to LCD up to null
        {
	        	//while(BusyXLCD()); 
                DelayXLCD();	     		// Wait while LCD is busy
                WriteDataXLCD(*buffer); 	// Write character to LCD
                buffer++;               	// Increment buffer
        }
        return;
}
