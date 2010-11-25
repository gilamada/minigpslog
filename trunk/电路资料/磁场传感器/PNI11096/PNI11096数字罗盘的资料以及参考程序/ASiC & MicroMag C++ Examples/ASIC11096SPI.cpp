/*
 *  ASIC.cpp
 *  ASIC_Example
 *
 *  Created by Jay Prunty on Wed Feb 11 2004.
 *  Copyright (c) 2004 PNI Corp. All rights reserved.
 *
 */

#include "ASIC11096SPI.h"


const UInt8 kMaxPeriod = 7;				// the maximum allowable period select.
const UInt8 kDefaultPeriod = 0x50;		// period defaults to 5 (period is
										// bits 4,5,6 in command byte



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		period  -   The period select used during the forward and   //
//								reverse biasing of the sensors.					//
//  Return Value:   NONE														//
//  Description:	Create an instance of the object.  The period must be		//
//					between 0 and 7 inclusive.									//
//////////////////////////////////////////////////////////////////////////////////

ASIC11096SPI::ASIC11096SPI(UInt8 period)
{
	SetPeriod(period);
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   NONE														//
//  Description:	Remove and instance of the object.							//
//////////////////////////////////////////////////////////////////////////////////

ASIC11096SPI::~ASIC11096SPI()
{
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   NONE														//
//  Description:	Initialize port directions and pin states for the ASIC.		//
//					The SSNOT, RESET, SCLK, and MOSI lines should all be		//
//					outputs, while the DRDY and MISO lines should be inputs.	//
//					The SSNOT line should be set high, while the RESET, SCLK	//
//					lines should be set low.									//
//////////////////////////////////////////////////////////////////////////////////

void ASIC11096SPI::Init()
{
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	// set up direction register
	CLRMASK(sysRegs->iop->IOPMOD, BIT(kDRDY) | BIT(kMISO));
	SETMASK(sysRegs->iop->IOPMOD, BIT(kSSNOT) | BIT(kRESET) | BIT(kSCLK)
			| BIT(kMOSI));
	
	// make sure the output pins are in the correct state
	SETMASK(sysRegs->iop->IOPDATA, BIT(kSSNOT));
	CLRMASK(sysRegs->iop->IOPDATA, BIT(kRESET) | BIT(kSCLK));
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   NONE														//
//  Description:	Selects the ASIC for communication by setting the SSNOT		//
//					line low.													//
//////////////////////////////////////////////////////////////////////////////////

void ASIC11096SPI::Select()
{
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	CLRMASK(sysRegs->iop->IOPDATA, BIT(kSSNOT));
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   NONE														//
//  Description:	End the communication with the ASIC by setting the SSNOT	//
//					line high.													//
//////////////////////////////////////////////////////////////////////////////////

void ASIC11096SPI::End()
{
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	SETMASK(sysRegs->iop->IOPDATA, BIT(kSSNOT));
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   NONE														//
//  Description:	Resets the ASIC by pulsing the RESET line high.  The line   //
//					must be returned to the low state.  This must also done		//
//					before every command/measurment.							//
//////////////////////////////////////////////////////////////////////////////////

void ASIC11096SPI::Reset()
{
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	SETMASK(sysRegs->iop->IOPDATA, BIT(kRESET));
	CLRMASK(sysRegs->iop->IOPDATA, BIT(kRESET));
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		high	-   True to set the clock high, false to set the	//
//								clock low.										//
//  Return Value:   NONE														//
//  Description:	Sets the clock line to the state indicated by high.			//
//////////////////////////////////////////////////////////////////////////////////

void ASIC11096SPI::Clock(bool high)
{
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	if(high)
	{
		SETMASK(sysRegs->iop->IOPDATA, BIT(kSCLK));
	}
	else
	{
		CLRMASK(sysRegs->iop->IOPDATA, BIT(kSCLK));
	}
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   True if the DRDY line is high, false if it is low.			//
//  Description:	Returns the state of the data ready line.					//
//////////////////////////////////////////////////////////////////////////////////

bool ASIC11096SPI::IsDRDYHigh()
{
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	return MASKHIGH(sysRegs->iop->IOPDATA, BIT(kDRDY));
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		high	-   True to set the MOSI line high, false to set	//
//								it low.											//
//  Return Value:   NONE														//
//  Description:	Sets the master out slave in line to the state indicated by //
//					high.														//
//////////////////////////////////////////////////////////////////////////////////

void ASIC11096SPI::SetMOSI(bool high)
{
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	if(high)
	{
		SETMASK(sysRegs->iop->IOPDATA, BIT(kMOSI));
	}
	else
	{
		CLRMASK(sysRegs->iop->IOPDATA, BIT(kMOSI));
	}
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   True if the MISO line his high, false if it is low.			//
//  Description:	Returns the state of the master out slave in line.			//
//////////////////////////////////////////////////////////////////////////////////

bool ASIC11096SPI::MISO()
{
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	return MASKHIGH(sysRegs->iop->IOPDATA, BIT(kMISO));
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		period  -   The new period select to send to  the ASIC.		//
//  Return Value:   NONE														//
//  Description:	The period must be between 0 and 7.  This is the period		//
//					select sent to the ASIC with every sensor command.			//
//////////////////////////////////////////////////////////////////////////////////

void ASIC11096SPI::SetPeriod(UInt8 period)
{
	if(period <= kMaxPeriod)
	{
		mPeriod = period << 4;
	}
	else
	{
		mPeriod = kDefaultPeriod;
	}
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   The period select part of the command.						//
//  Description:	Returns the period select shifted to it's position in the   //
//					command byte.												//
//////////////////////////////////////////////////////////////////////////////////

UInt8 ASIC11096SPI::PeriodCmdPart()
{
	return mPeriod;
}

