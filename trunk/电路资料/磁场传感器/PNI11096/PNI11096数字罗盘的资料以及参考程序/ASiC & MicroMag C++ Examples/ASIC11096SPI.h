#pragma once


class ASIC11096SPI
{
public:
	// Commands to read the sensors
	// These are from the datasheet
	enum
	{
		kXAxis = 0x01,
		kYAxis = 0x10,
		kZAxis = 0x11,
	} Cmd;
	
	ASIC11096SPI(UInt8 period);
	~ASIC11096SPI();
	
	void Init();
	
	// Actions
	void Select();				// select the ASIC by driving SSNOT low
	void End();					// deselect the ASIC by driving SSNOT high
	void Reset();				// Pulse the reset line
	void Clock(bool high);		// set the state of the SCLK line
	bool IsDRDYHigh();			// Check to see if DRDY line is high
	void SetMOSI(bool high);	// set the state of the MOSI line
	bool MISO();				// get the state of the MISO line
	
	// member variable access
	void SetPeriod(UInt8 period);
	UInt8 PeriodCmdPart();		// returns the period part of the command byte
	
protected:
	// THESE NEED TO BE CHANGED TO MATCH YOUR OWN HARDWARE
	enum
	{
		kSSNOT = 14,			// Slave select not for the ASIC
		kRESET = 15,			// Reset line
		kDRDY  = 24,			// Data Ready line
		kSCLK  = 25,			// Serial Clock
		kMOSI  = 23,			// Master Out Slave In
		kMISO  = 22,			// Master In Slave Out  
	} SpiLines;

	UInt8 mPeriod;				// period select to send with the command
};

