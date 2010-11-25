#include "Magnetometer.h"
#include "TickGenerator.h"
#include "MKDebugMsg.h"


Magnetometer * magnetometer = NULL;



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   NONE														//
//  Description:	Create an instance of the onject.							//
//////////////////////////////////////////////////////////////////////////////////

Magnetometer::Magnetometer()
{
	magnetometer = this;
	mASIC = new ASIC11096SPI(3);
	mASIC->Init();
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   NONE														//
//  Description:	Removes an instance of the object.							//
//////////////////////////////////////////////////////////////////////////////////

Magnetometer::~Magnetometer()
{
	if(mASIC != NULL)
	{
		delete mASIC;
		mASIC = NULL;
	}
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		axis	-   The magnetic sensor to read.					//
//  Return Value:   The magnetic field on the desired axis.						//
//  Description:	Bitbangs the command to read the axis out to the asic, and  //
//					reads the magnetic field after the DRDY line has gone high. //
//////////////////////////////////////////////////////////////////////////////////

SInt16 Magnetometer::Sample(UInt8 axis)
{
	UInt8 cmd;							// the command to clock out
	UInt8 bit = 0x80;					// the bit in the command to shift out
	SInt16 magField = 0;				// the measured magnetic field
	UInt32 waitTime;					// time spent waiting for drdy to go high
	
	if(axis > kZ || mASIC == NULL)
	{
		return 0;
	}
	
										// format the command
	cmd = mASIC->PeriodCmdPart() | axis;
	
	mASIC->Select();					// select the asic for communication
	mASIC->Reset();						// reset the asic and get it ready to Rx
	
	// write out the command
	for(UInt32 count = 8; count > 0; --count)
	{
		// set the appropriate bit
		mASIC->SetMOSI((bool)(cmd &  bit));
		mASIC->Clock(true);				// set the clk high, asic should read
		mASIC->Clock(false);			// clock data out on falling edge
		bit >>= 1;
	}
	
	waitTime = Ticks() + 100;			// 1 sec should be more than enough time
										// to take the sample.  Loop until the
										// data is ready, or the time is up
	while(!mASIC->IsDRDYHigh())
	{
		if(Ticks() > waitTime)
		{
			mASIC->End();				// error reading sensor
			return 0;
		}
	}
	
	// read in the response
	for(UInt32 count = 16; count > 0; --count)
	{
		magField <<= 1;					// read next bit
		mASIC->Clock(true);				// read in on rising edge
		
		if(mASIC->MISO())
		{
			magField |= 1;
		}
		
		mASIC->Clock(false);			// next bit written on falling edge
	}
	
	mASIC->End();
	
	return magField;
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		period  -   The period to select.							//
//  Return Value:   NONE														//
//  Description:	Sets the ASIC to run at the desired period.					//
//////////////////////////////////////////////////////////////////////////////////

void Magnetometer::SetPeriod(UInt8 period)
{
	if(mASIC != NULL)
	{
		mASIC->SetPeriod(period);
	}
}



//////////////////////////////////////////////////////////////////////////////////
//  Parameters:		NONE														//
//  Return Value:   The period the ASIC is using to read the sensors.			//
//  Descritption:   Returns the period that the ASIC is using to read the		//
//					sensors.													//
//////////////////////////////////////////////////////////////////////////////////

UInt8 Magnetometer::Period()
{
	if(mASIC != NULL)
	{
		return mASIC->PeriodCmdPart() >> 4;
	}
	
	return 0xFF;							// error occured
}
