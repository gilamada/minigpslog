#pragma once

#include "ASIC11096SPI.h"



class Magnetometer
{
public:
	enum
	{
		kX = ASIC11096SPI::kXAxis,
		kY = ASIC11096SPI::kYAxis,
		kZ = ASIC11096SPI::kZAxis
	} Cmd;
	
	Magnetometer();
	~Magnetometer();
	
	SInt16 Sample(UInt8 axis);
	void SetPeriod(UInt8 period);
	UInt8 Period();
	
protected:
	ASIC11096SPI * mASIC;
};

extern Magnetometer * magnetometer;
