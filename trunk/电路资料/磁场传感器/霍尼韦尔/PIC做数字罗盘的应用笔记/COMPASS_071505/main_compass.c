

/*****************************************************************************
*   Filename:   main_compass.c                                               *
******************************************************************************
*   Author:    Chris Valenti                                                 *
*   Title:     Senior Applications Engineer                                  *
*   Company:   Microchip Technology Incorporated                             *
*   Revision:  1.0                                                           *
*   Date:      7/15/05                                                       *
******************************************************************************
*   Include files:                                                           *
*      xlcd.h -	LCD header file												 *
*	   P18F2520.h - device header											 *
*      state.h - used for declination angles								 *
*																			 *
*	Source Files:									                         *
*		18ADOver.asm														 *
*		bin16_ASCII.asm														 *
*		openxlcd_test.c														 *
*		putrxlcd.c															 *
*		setcgram.c															 *
*		setddram.c															 *
*		wcmxlcd.c															 *
*		writdata.c														     *
*		state_select.c														 *
*																		     *
*	Linker file:															 *
*		18F2520i.lkr														 *
*																			 *
*      Compiled with Microchip C18 Compiler                                  *
******************************************************************************
*   This file contains the main code to run the digital compass.			 *
******************************************************************************
*   			                                                             *
*	Date		Rev			Changes											 *
*	-------		-----		----------------      							 *
*	7/15/05		1.0			Original release 								 *
*      								                                         *
*																			 *
*****************************************************************************/


#include	<P18F2520.h>
#include	<xlcd.h>
#include	<state.h>

// Config bit settings
#pragma config OSC = HS, FCMEN = OFF, IESO = OFF
#pragma config PWRT = ON, BOREN = OFF
#pragma config WDT = OFF
#pragma config MCLRE = ON, LPT1OSC = OFF
#pragma config STVREN = OFF, LVP = OFF
#pragma config XINST = OFF, DEBUG = OFF
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB = OFF


#pragma idata
unsigned char adH,adL;
#define		mLoadH(load)		(adH=load)				//macro for loading variable from C to ASM
#define		mLoadL(load)		(adL=load)				//macro for loading variable from C to ASM						

rom const char x_axis[] = 	"= X axis\n\r";				//Hypertermianl messages
rom const char y_axis[] =   "= Y axis\n\r";
rom const char z_axis[] =   "= Z axis\n\r";

unsigned int 	ADresult[4];							//array for the A/D results
#define 	ad_x_axis	0
#define 	ad_y_axis	1
#define 	ad_z_axis	2
#define 	ad_pr_temp	3

unsigned int 	AxisValue[3];							//array for axis value
#define 	X	0
#define 	Y	1
#define 	Z	2

#define		XCalVal 75									//axis's require PWM cal value
#define		YCalVal 255
#define		ZCalVal 255

#define		serial_x_axis								//if defined, the x axis A/D value will be TX on serial
//#define		serial_y_axis								//if defined, the y axis A/D value will be TX on serial
//#define		serial_z_axis								//if defined, the z axis A/D value will be TX on serial

			char 	PitchAngle, RollAngle;							//-70 to 70 degrees required
			char 	PRTempFactor;									//holds a correction factor for Pitch & Roll degrees
			float 	PitchCOS, PitchSIN, RollCOS, RollSIN;
			float 	Xheading, Yheading, XYHeading;
unsigned 	char 	TableIndex;


extern unsigned char BCDNumDig1;
extern unsigned char BCDNumDig2;
extern unsigned char BCDNumDig3;
extern unsigned char BCDNumDig4;
extern unsigned char BCDNumDig5;

extern unsigned char vADOverHigh;
extern unsigned char vADOverLow;

union									//this union maintains the Timer1 count
{
	struct
	{
		unsigned char TimerLow;
		unsigned char TimerHigh;
	}t1HiLow;	
		unsigned int TimrCnt;	
}timer;

union
{
	struct
	{
		unsigned char FhLo;
		unsigned char FhHi;
	}FhHiLo;
		unsigned int FinalHeading;
}Fh;

//COSINE Table 
const rom float cos_table[] = 				//angles 0 - 70 degrees "-" or "+" values
{
 1.000,1.000,0.999,0.999,0.998,0.996,0.995,0.993,0.990,0.988,0.985,0.982
,0.978,0.974,0.970,0.966,0.961,0.956,0.951,0.946,0.940,0.934,0.927
,0.921,0.914,0.906,0.899,0.891,0.883,0.875,0.866,0.857,0.848,0.839
,0.829,0.819,0.809,0.799,0.788,0.777,0.766,0.755,0.743,0.731,0.719
,0.707,0.695,0.682,0.669,0.656,0.643,0.629,0.616,0.602,0.588,0.574
,0.559,0.545,0.530,0.515,0.500,0.485,0.469,0.454,0.438,0.423,0.407
,0.391,0.375,0.358,0.342
};

//SIN Table
const rom float sin_table[] =				//angles 0 - 70 degrees, if angle is "-" then SIN = "-"
{
0.000,0.017,0.035,0.052,0.070,0.087,0.105,0.122,0.139,0.156,0.174,0.191
,0.208,0.225,0.242,0.259,0.276,0.292,0.309,0.326,0.342,0.358,0.375
,0.391,0.407,0.423,0.438,0.454,0.469,0.485,0.500,0.515,0.530,0.545
,0.559,0.574,0.588,0.602,0.616,0.629,0.643,0.656,0.669,0.682,0.695
,0.707,0.719,0.731,0.743,0.755,0.766,0.777,0.788,0.799,0.809,0.819
,0.829,0.839,0.848,0.857,0.866,0.875,0.883,0.891,0.899,0.906,0.914
,0.921,0.927,0.934,0.940
};

//TANGENT Table
const rom float tan_table[] = 				//angles 0 - 89 = 0 - 57.29 degrees
											//angles 91 - 180 = -57.29 - 0 degrees
{
0.02,0.03,0.05,0.07,0.09,0.11,0.12,0.14,0.16,0.18,0.19,0.21,0.23,0.25
,0.27,0.29,0.31,0.32,0.34,0.36,0.38,0.40,0.42,0.45,0.47,0.49,0.51,0.53
,0.55,0.58,0.60,0.62,0.65,0.67,0.70,0.73,0.75,0.78,0.81,0.84,0.87,0.90
,0.93,0.97,1.00,1.04,1.07,1.11,1.15,1.19,1.23,1.28,1.33,1.38,1.43,1.48
,1.54,1.60,1.66,1.73,1.80,1.88,1.96,2.05,2.14,2.25,2.36,2.48,2.61,2.75
,2.90,3.08,3.27,3.49,3.73,4.01,4.33,4.70,5.14,5.67,6.31,7.12,8.14,9.51
,11.43,14.30,19.08,28.64,57.29,76.39,
-57.29,-28.64,-19.08,-14.30,-11.43,-9.51,-8.14,-7.12,-6.31,-5.67,-5.14		
,-4.70,-4.33,-4.01,-3.73,-3.49,-3.27-3.08,-2.90,-2.75,-2.61,-2.48,-2.36
,-2.25,-2.14,-2.05,-1.96,-1.88,-1.80,-1.73,-1.66,-1.60,-1.54,-1.48,-1.43
,-1.38,-1.33,-1.28,-1.23,-1.19,-1.15,-1.11,-1.07,-1.04,-1.00,-0.97,-0.93
,-0.90,-0.87,-0.84,-0.81,-0.78,-0.75,-0.73,-0.70,-0.67,-0.65,-0.62,-0.60
,-0.58,-0.55,-0.53,-0.51,-0.49,-0.47,-0.45,-0.42,-0.40,-0.38,-0.36,-0.34
,-0.32,-0.31,-0.29,-0.27,-0.25,-0.23,-0.21,-0.19,-0.18,-0.16,-0.14,-0.12
,-0.11,-0.09,-0.07,-0.05,-0.03,-0.02,0.00
};
	
//-----PORTA CONFIGURATION-----
#define		out_x	PORTAbits.RA0		//x axis	
#define		out_y	PORTAbits.RA1 		//y axis
#define		out_z	PORTAbits.RA2 		//z axis
#define		pr_temp	PORTABITS.RA3 		//pitch/roll temperature
#define		gate	PORTAbits.RA4		//set/reset MOSFET control
#define		pitch 	PORTAbits.RA5		//roll axis 

//-----PORTB CONFIGURATION-----
//#define	data0			PORTBbits.RB0		//LCD DATA
//#define	data1			PORTBbits.RB1		//LCD DATA
//#define	data2			PORTBbits.RB2		//LCD DATA
//#define	data3			PORTBbits.RB3		//LCD DATA
#define		roll			PORTBbits.RB4 
#define		sw_led			PORTBbits.RB5
#define		sw_led_config	TRISBbits.TRISB5	
//#define	pgc				PORTBbits.RB6 		//ICD CLK
//#define	pgd				PORTBbits.RB7 		//ICD DATA
	
//-----PORTC CONFIGURATION-----
//#define	e		PORTCbits.RC0		//ENABLE SIGNAL
//#define	rs		PORTCbits.RC1		//DATA/INSTRUCTION
#define		pwm		PORTCbits.RC2 
#define		scl		PORTCbits.RC3	  
#define		sda		PORTCbits.RC4 
#define		sdo		PORTCbits.RC5 
#define		tx		PORTCbits.RC6 
#define		rx		PORTCbits.RC7 

#define		RCIF 	PIR1bits.RCIF
#define		TRMT 	TXSTAbits.TRMT
#define		GO		ADCON0bits.GO

#define		off		0
#define		on		1


//-----PROTOTYPES-----
#define MAIN_C
#include <main.h>
#undef MAIN_C

void	init(void);														//peripheral initalization
void	DelayFor18TCY(void);											//18TCY delay for LCD routines
void	DelayPORXLCD(void);												//15ms delay for LCD routines
void	DelayXLCD(void);												//5ms delay for LCD routines
void	CalXYZOut(unsigned char dutycycle);												//routine drives PWM to calibrate op amps
void 	DelayHalfSec(void);												//1/2 second delay
void	LCDByteAddr(unsigned char data, unsigned char DDaddr);
void	LCDWriteByte(unsigned char data);								//sends byte to LCD
void	AlignMagAxis(void);												//align the xyz devices
char	ReadUsart(void);
void	WriteStringUsart(const rom char *tx_data);
void	WriteByteUsart(unsigned int data);
void	Led(char led_status);
void	LedToggle(void);
char	MeasurePitch(void);
char	MeasureRoll(void);
void	A_DLoop(void);
void 	ASCIIConvWrite(unsigned char var);
void	Bin8_ToASCII(char dtemp);
void	Bin8_ToBCD(char dtemp);
float 	FindCOS(signed char DegreeToCOS);
float 	FindSIN(char DegreeToSIN);
void	GetArcTan(void);
char 	EERead(unsigned char EEAdress);
void	QuadrantDecode(void);
void 	PRDisplay(void);

extern 	void Bin16_2BCD(void);
extern 	void Bin16_2BCD4LCD(void);
extern	void ADOverInit(void);
extern	void ADOverGetData(void);

#pragma code
//___________________________________________ MAIN __________________________________________________
void	main(void)
{
	init();											//initalization routines
	
	LCDMessageAddr(" Hello! ", 0);					//Power up Greeting
	LCDMessageAddr("Compass ",0x40);
	Delay1Sec();
	Delay1Sec();
	Delay1Sec();
	WriteCmdXLCD(0x01);             				//Clear display
	
	SelectState();									//select state for declination angle...?
	decl_angle = EERead(DAngleAddr);				//read EEPROM to retrieve declination angle for specific state		
													//	if no state was selected, EEPROM will have value from last selection

	LCDMessageAddr("PitchRol", 0);					//Dispaly the Pitch & Roll values for calibration 
	LCDMessageAddr("Display?",0x40);
	Delay1Sec();
	
	if (Switch() == 0 )
	{
		Delay1Sec();
		Delay1Sec();
		while(Switch() == 1)
		{
			PRDisplay();
		}
	}

	WriteStringUsart("Start Test");					//write Start message to PC...testing purposes
	WriteByteUsart(0x0D);							//carriage return and line feed
	WriteByteUsart(0x0A);
	
	WriteCmdXLCD(0x01);             				//Clear LCD
	
while(1)
	{		
	LedToggle();									//status indicator

	MeasurePitch();									//measure pitch angle			
	MeasureRoll();									//measure roll angle
	
	PitchCOS 	= FindCOS(PitchAngle);				//find the SIN & COS of Pitch & Roll angles
	RollCOS 	= FindCOS(RollAngle);
	PitchSIN 	= FindSIN(PitchAngle);
	RollSIN 	= FindSIN(RollAngle);
	if(PitchAngle < 0)								//if pitchangle is negative then SIN is negative
	PitchSIN = -PitchSIN;
	if(RollAngle < 0)								//if rollangle is negative then SIN is negative
	RollSIN = -RollSIN;
	
	AlignMagAxis();									//re-align the magnetic strip in the X/Y & Z devices
	
	A_DLoop();										//convert analog inputs: X, Y, Z & Pitch/Roll temperature
		
	if((PitchAngle == 0) && (RollAngle == 0))
	{
		Xheading =  AxisValue[X];					//Pitch & Roll = 0, therefore simpler X/Y calcs
		Yheading =  AxisValue[Y];						//no Z axis value is required
	}			 
	else
	{
		Xheading =  (AxisValue[X] * PitchCOS)		//Xh (magnetic component) heading for arcTAN calc
		+ (AxisValue[Y] * RollSIN * PitchSIN) 
		- (AxisValue[Z] * RollCOS * PitchSIN);
	
		Yheading = (AxisValue[Y] * RollCOS)			//Yh (magnetic component) heading for arcTAN calc
				 + (AxisValue[Z] * RollSIN);
	}
					
	XYHeading = Yheading/Xheading;					//Yh/Xh value			1st step for heading
	GetArcTan();									//find arcTAN(Yh/Xh)	2nd step for heading
	
	QuadrantDecode();								//measurement is in quadrant 1,2,3 or 4 ?
		
	Fh.FinalHeading = Fh.FinalHeading + decl_angle;	// calculate "True North" Heading		
		
	mLoadH(Fh.FhHiLo.FhHi);							//write FinalHeading to LCD
	mLoadL(Fh.FhHiLo.FhLo);
	Bin16_2BCD4LCD();
	BCDNumDig5 = BCDNumDig5 + '0';
    BCDNumDig4 = BCDNumDig4 + '0';
    BCDNumDig3 = BCDNumDig3 + '0';
	BCDNumDig2 = BCDNumDig2 + '0';
	BCDNumDig1 = BCDNumDig1 + '0';

	LCDByteAddr(BCDNumDig3, 0x00);					//start at address 00 on LCD
	LCDWriteByte(BCDNumDig2);						// only digits 1, 2 and 3 need to be displayed 
	LCDWriteByte(BCDNumDig1);						//	for a 1 - 360 degree heading

//	mLoadH(Fh.FhHiLo.FhHi);		
//	mLoadL(Fh.FhHiLo.FhLo);
//	Bin16_2BCD();								//convert result to BCD for PC display
//	ASCIIConvWrite(BCDNumDig4);
//	ASCIIConvWrite(BCDNumDig2);	
//	WriteStringUsart(" Heading");
//	WriteByteUsart(0x0D);						//carriage return and line feed
//	WriteByteUsart(0x0A);

	DelayHalfSec();

	}
}	

//________________________END OF MAIN____________________________________________________________
	

/*-----------------------------------------------
Function: Led
Overview: Controls LED value based on passed value
Input: char On or Off LED value
Output: None
-----------------------------------------------*/
void	Led(char led_status)
{
		if(led_status)
		{
			sw_led_config = 0;		//make RB5 an output
			sw_led = 1;				//LED on
		}
		else
			sw_led = 0;				//LED off
}


/*-----------------------------------------------
Function: LedToggle
Overview: Toggles LED on RB5
Input: None
Output: None
-----------------------------------------------*/
void	LedToggle(void)
{
_asm
		btg		PORTB,5,0
_endasm
}


/*-----------------------------------------------
Function: Switch
Overview: Enable switch for operation
Input: None
Output: Returns a char value to indicate if the switch 
		has been pressed.
-----------------------------------------------*/
char	Switch(void)				//switch is normally high, sw_led = 1, no press
{
		sw_led_config = 1;			//make RB5 an input
		DelayHalfSec();
		if(sw_led)
		{
			sw_led_config = 0;		//make RB5 an output
			return 1;				//switch not pressed
		}
		else
			sw_led_config = 0;		//make RB5 an output
			return 0;				//switch pressed, grounded
}
		

/*-----------------------------------------------
Function: ReadUsart
Overview: waits for data from the PC
Input: None
Output: Returns char RX value from PC
-----------------------------------------------*/		
char	ReadUsart(void)
{
		unsigned char rx_data;
		if(RCIF)
		rx_data = RCREG;
		return rx_data;
}


/*-----------------------------------------------
Function: WriteStringUsart
Overview: Writes a string to the PC
Input: const ROM char Pointer to string for TX out
Output: None
-----------------------------------------------*/
void	WriteStringUsart(const rom char *tx_data)
{
		while(*tx_data)
		{
			while(!TRMT);
			TXREG = *tx_data;
			tx_data++;
		}
}


/*-----------------------------------------------
Function: WriteByteUsart
Overview: Writes a byte to the serial port
Input: unsigned int to be transmitted
Output: None
-----------------------------------------------*/
void	WriteByteUsart(unsigned int data)
{
		while(!TRMT);
		TXREG = data;
}	
			
				
/*-----------------------------------------------
Function: LCDMessageAddr
Overview: Writes a string to the LCD
Input: char Pointer to LCD string and unsigned char address for LCD
Output: None
-----------------------------------------------*/
void	LCDMessageAddr(const rom char *buffer, unsigned char DDaddr) //sends LCD message and line location
{
	SetDDRamAddr(DDaddr);
	putrsXLCD(buffer);		
}


/*-----------------------------------------------
Function: LCDByteAddr
Overview: Writes a single byte to the LCD
Input: unsigned char data and unsigned char address for LCD
Output: None
-----------------------------------------------*/
void	LCDByteAddr(unsigned char data, unsigned char DDaddr) //sends byte and line location to LCD
{
	SetDDRamAddr(DDaddr);
	WriteDataXLCD(data);			
}


/*-----------------------------------------------
Function: LCDWriteByte
Overview: Writes byte to LCD, no address specified
Input: unsigned char
Output: None
-----------------------------------------------*/
void	LCDWriteByte(unsigned char data)		//sends byte to LCD
{
	WriteDataXLCD(data);		
}


/*-----------------------------------------------
Function: ASCIIConvWrite
Overview: Converts hex value to ASCII and transmits converted
			value
Input: unsigned char hex value to be converted
Output: None
-----------------------------------------------*/	
void 	ASCIIConvWrite(unsigned char var)
{
		char varl;
		char varh;
		varl = ((var & 0x0F) + 0x30);
		varh = ((var & 0xF0)>>4) + 0x30;
		WriteByteUsart(varh);
		WriteByteUsart(varl);
}


/*-----------------------------------------------
Function: Bin8_ToASCII
Overview: Converts hex value to ASCII
Input: char 
Output: None
-----------------------------------------------*/
void	Bin8_ToASCII(char dtemp)					//8 bit(1 byte) to ASCII conversion
{		
	BCDNumDig3 = dtemp / 0x64;
	BCDNumDig2 = ( dtemp - (BCDNumDig3*0x64) )/10;
	BCDNumDig1 = ( dtemp - (BCDNumDig3*0x64)-(BCDNumDig2*10) );
	
	BCDNumDig3 = BCDNumDig3 + '0';
	BCDNumDig2 = BCDNumDig2 + '0';
	BCDNumDig1 = BCDNumDig1 + '0';
}


/*-----------------------------------------------
Function: MeasurePitch
Overview: Measures the pitch value of the acceleromter
		  by timing the output signal with Timer1
Input: None
Output: Char pitch value
-----------------------------------------------*/
char	MeasurePitch(void)
{
	do
	{
		int  TableIndex;
		while(pitch);									//wait until HIGH pulse is over
		while(!pitch);									//wait until LOW pulse is over
		TMR1H = 0;										//start counter (0000) on start of high
		TMR1L = 0;
		while(pitch);									//wait until HIGH is done
		timer.t1HiLow.TimerHigh = TMR1H;				//move the High pulse count
		timer.t1HiLow.TimerLow = TMR1L;	
	
		if(timer.TimrCnt > 5000)						//if timer count > 5000 then angle = POSITIVE
		{
			TableIndex = (timer.TimrCnt - 5000) / 33;	//calculate angle based on T1 counts
			PitchAngle = TableIndex;					//angle = POSITIVE
		}
	
		if(timer.TimrCnt == 5000)						//if timer count = 5000 then angle = 0
		{
			PitchAngle = 0;
		}
		if(timer.TimrCnt < 5000)
		{												//angle is positive
			TableIndex = (timer.TimrCnt - 2690) / 33;	//calculate angle based on T1 counts					
			PitchAngle = (70 - TableIndex);				//angle = ***NEGATIVE***
			PitchAngle = -PitchAngle;					//complement for ***NEGATIVE*** angle
		}

		PitchAngle = PitchAngle + PRTempFactor;			//Apply Pitch/Roll temperature factor
		
		if(PitchAngle > 40)								//if angle is excessive then display warning
		{
		LCDMessageAddr(" Pitch  ", 0x0);
		LCDMessageAddr("^^^^^^^^",0x40);
		Delay1Sec();	
		}

		if(PitchAngle < -40)							//if angle is excessive then display warning
		{
		LCDMessageAddr(" Pitch  ", 0x0);
		LCDMessageAddr("  BACK  ",0x40);
		Delay1Sec();	
		}
	}	
	while( (PitchAngle > 40) || (PitchAngle < -40) );	//measure angle until within limits
		
		WriteCmdXLCD(0x01);             				//Clear display
		return PitchAngle;
}	


/*-----------------------------------------------
Function: MeasureRoll
Overview:Measures the roll value of the acceleromter
		  by timing the output signal with Timer1
Input: None
Output: Char roll value
-----------------------------------------------*/
char	MeasureRoll(void)
{
	do
	{
		char TableIndex;	
		while(roll);									//wait until HIGH pulse is over
		while(!roll);									//wait until LOW pulse is over
		TMR1H = 0;										//start counter (0000) on start of high
		TMR1L = 0;
		while(roll);									//wait until HIGH is done
		timer.t1HiLow.TimerHigh = TMR1H;				//move the High pulse count
		timer.t1HiLow.TimerLow = TMR1L;
		Nop();		//testing
		
		if(timer.TimrCnt > 5000)
		{
			TableIndex = (timer.TimrCnt - 5000) / 33;	//calculate angle based on T1 counts
			RollAngle = TableIndex;						//angle = POSITIVE
		}
	
		if(timer.TimrCnt == 5000)
		{
			RollAngle = 0;								//if timer count = 5000 then angle = 0
		}
	
		if(timer.TimrCnt < 5000)
		{
			TableIndex = (timer.TimrCnt - 2690) / 33;	//calculate angle based on T1 counts					
			RollAngle = (70 - TableIndex);				//angle = ***NEGATIVE***
			RollAngle = -RollAngle;						//complement for ***NEGATIVE*** angle
		}
	
		RollAngle = RollAngle + PRTempFactor;			//Apply Pitch/Roll temperature factor
		
		if(RollAngle > 40)								//if angle is excessive then display warning
		{
		LCDMessageAddr("  Roll  ", 0x0);
		LCDMessageAddr("<<<<----",0x40);
		Delay1Sec();	
		}
		
		if(RollAngle < -40)								//if angle is excessive then display warning
		{
		LCDMessageAddr("  Roll  ", 0x0);
		LCDMessageAddr("---->>>>",0x40);	
		Delay1Sec();
		}
	}		
	while( (RollAngle > 40) || (RollAngle < -40) );		//measure angle until within limits
	
		WriteCmdXLCD(0x01);             				//Clear display
		return RollAngle;	
}	

/*-----------------------------------------------
Function: FindCOS
Overview: Converts the pitch and roll values to a 
			cosine value
Input: signed char
Output: float cosine value
-----------------------------------------------*/
float 	FindCOS(signed char DegreeToCOS)		//converts pitch/roll degrees to COSINE value for Xh & Yh calcs
{
	float cos_value;
	if(DegreeToCOS < 0)
	{	
		DegreeToCOS = -DegreeToCOS;				//complement for ***NEGATIVE*** angle to get correct value from table
	}		
	cos_value = cos_table[DegreeToCOS];
	return	cos_value;
}	

/*-----------------------------------------------
Function: FindSIN
Overview:Converts the pitch and roll values to a 
			sine value
Input: signed char
Output: float sine value
-----------------------------------------------*/	
float 	FindSIN(char DegreeToSIN)				//converts pitch/roll degrees to SINE value for Xh & Yh calcs
{
	float sin_value;
	if(DegreeToSIN < 0)
	{	
		DegreeToSIN = -DegreeToSIN;				//complement for ***NEGATIVE*** angle to get correct value from table
	}
	sin_value = sin_table[DegreeToSIN];
	return	sin_value;
}


/*-----------------------------------------------
Function: AlignMagAxis
Overview: Re-aligns the magnetic field of the HMC swensors 
Input: None
Output: None
-----------------------------------------------*/
void	AlignMagAxis(void)						//used to reset the 1-axis & 2-axis magnetic fields
{
	gate = 1;									//send high pulse for ~1us
	gate = 0;
_asm
	BTG 	PORTC,3,0
_endasm

}

/*-----------------------------------------------
Function: CalXYZOut
Overview: Drives the magnetic sensor op amps for calibration purposes
Input: unsigned char
Output: None
-----------------------------------------------*/		
void	CalXYZOut(unsigned char dutycycle)			//drives the Vref on the Op amps
{
	PR2 = 150;										//configure PWM period @4MHz, 3.9kHz 
	CCPR1L = dutycycle;								//load with specific duty cycle for each axis
//	CCPR1L = 75;									//configure PWM DC xx% 126uS 
}

/*-----------------------------------------------
Function: GetArcTan
Overview: Get the ArcTan value  of the inital heading calculation
Input: None
Output: None
-----------------------------------------------*/
void	GetArcTan(void)
{
	unsigned char TableIndex;
	if(XYHeading > 0 && XYHeading < 1)						//0 to 0.99 divided Tan table into lower half (45 values)
	{	
		TableIndex = 0;	
		while(XYHeading > tan_table[TableIndex])
		{
			TableIndex++;									//move to next position in table
		}								
	}
	
	 if (XYHeading >= 1 && XYHeading <= 76.39)
	{		
		TableIndex = 45;									// 1 to 57.29 go to middle of table to access upper half
		while(XYHeading > tan_table[TableIndex])
		{
			++TableIndex;									//move to next position in table
		}
	}
	
	if (XYHeading < 0)										// 0 to -57 access -Tangent values, greater then -57.
	{
		TableIndex = 90;									
		while(XYHeading > tan_table[TableIndex])
		{
			++TableIndex;
		}	
		TableIndex = 1 + TableIndex;						//offset the array by 1	
	}
	
	if (XYHeading > 76.39 || XYHeading <-57.29)
	{
		TableIndex = 90;
	}	
	
		Fh.FinalHeading = TableIndex;
}		


/*-----------------------------------------------
Function: A_DLoop
Overview: Measures the magnetic sensor outputs and
			converts it to a digital value 
Input: None
Output: None
-----------------------------------------------*/
void	A_DLoop(void)
{
// _________________________X axis conv __________________________________	
	ADCON0 = 0b00000001;						//select AN0, out_x
	CalXYZOut(XCalVal);							//PWM op amp calibration
	DelayXLCD();								//delay for 5ms @ 4MHz
	ADOverGetData();							//start coversion w/oversampling
	ADresult[ad_x_axis] = ((int)(vADOverHigh)<<8) + ((int)(vADOverLow));
	
	if(ADresult[ad_x_axis] > 2048)				// need to find value from 2048 A/D midpoint
		AxisValue[X] = ADresult[ad_x_axis] - 2048;
	else
		AxisValue[X] = 2048 - ADresult[ad_x_axis];

#ifdef	serial_x_axis		
	mLoadH(vADOverHigh);		
	mLoadL(vADOverLow);
	Bin16_2BCD();
	ASCIIConvWrite(BCDNumDig5);					//write X axis value to serial
	ASCIIConvWrite(BCDNumDig4);
	ASCIIConvWrite(BCDNumDig2);					//convert result to BCD for PC display
//	WriteByteUsart('X');						//Print "X"
	WriteByteUsart(0x0D);						//carriage return and line feed
	WriteByteUsart(0x0A);
#endif

// __________________________Y axis conv __________________________________		
	ADCON0 = 0b00000101;						//select AN1, out_y
	CalXYZOut(YCalVal);							//PWM op amp calibration
	DelayXLCD();								//delay for 5ms @ 4MHz
	ADOverGetData();							//start coversion w/oversampling
	ADresult[ad_y_axis] = ((int)(vADOverHigh)<<8) + ((int)(vADOverLow));
	
	if(ADresult[ad_y_axis] > 2048)				// need to find value from 2048 midpoint
		AxisValue[Y] = ADresult[ad_y_axis] - 2048;
	else
		AxisValue[Y] = 2048 - ADresult[ad_y_axis];

#ifdef	serial_y_axis
	mLoadH(vADOverHigh);		
	mLoadL(vADOverLow);
	Bin16_2BCD();								//convert result to BCD for PC display
	ASCIIConvWrite(BCDNumDig5);					//write Y axis value to serial
	ASCIIConvWrite(BCDNumDig4);
	ASCIIConvWrite(BCDNumDig2);	
//	WriteByteUsart('Y');						//Print "Y"
	WriteByteUsart(0x0D);						//carriage return and line feed
	WriteByteUsart(0x0A);
#endif	
	
// __________________________Z axis conv __________________________________				
	ADCON0 = 0b00001001;						//select AN2, out_z
	CalXYZOut(ZCalVal);							//PWM op amp calibration
	DelayXLCD();								//delay for 5ms @ 4MHz
	ADOverGetData();							//start coversion w/oversampling
	ADresult[ad_z_axis] = ((int)(vADOverHigh)<<8) + ((int)(vADOverLow));
	
	if(ADresult[ad_z_axis] > 2048)				// need to find value from 2048 midpoint
		AxisValue[Z] = ADresult[ad_z_axis] - 2048;
	else
		AxisValue[Z] = 2048 - ADresult[ad_z_axis];
	
#ifdef	serial_z_axis	
	mLoadH(vADOverHigh);		
	mLoadL(vADOverLow);
	Bin16_2BCD();								//convert result to BCD for PC display
	ASCIIConvWrite(BCDNumDig5);					//write Z axis value to serial
	ASCIIConvWrite(BCDNumDig4);
	ASCIIConvWrite(BCDNumDig2);	
	WriteByteUsart('Z');						//Print "Z"
	WriteByteUsart(0x0D);						//carriage return and line feed
	WriteByteUsart(0x0A);
	
	WriteByteUsart(0x0D);						//carriage return and line feed
	WriteByteUsart(0x0A); 
	WriteByteUsart(0x0D);						//carriage return and line feed
	WriteByteUsart(0x0A);	
#endif

// __________________________Pitch/Roll Temp conv__________________________
	ADCON0 = 0b00001101;						//AN3, pitch/roll temp, does not require oversampling
	GO = 1;										//start conversion
	while(GO);									//wait for conversion to end
	ADresult[ad_pr_temp] = ((int)(ADRESH)<<8) + ((int)(ADRESL));		

#ifdef	PR_temp	
	mLoadH(ADRESH);		
	mLoadL(ADRESL);
	Bin16_2BCD();								//convert result to BCD for PC display
	ASCIIConvWrite(BCDNumDig5);					//write P/R temp value to serial
	ASCIIConvWrite(BCDNumDig4);
	ASCIIConvWrite(BCDNumDig2);	
	WriteByteUsart(0x0D);						//carriage return and line feed
	WriteByteUsart(0x0A); 
	WriteByteUsart(0x0D);						//carriage return and line feed
	WriteByteUsart(0x0A);
#endif
	PRTempFactor = -43.28 + (.111 * ADresult[ad_pr_temp]);	//caluclate temperature correction factor
															//value is added to pitch & roll angles	
}

/*-----------------------------------------------
Function: QuadrantDecode
Overview: Determines which quadrant the magnetic sensor
			values are in
Input: None
Output: None
-----------------------------------------------*/
void	QuadrantDecode(void)
{
	unsigned char QuadDecodeDone;
	
	if((ADresult[ad_x_axis] < 2048) && (ADresult[ad_y_axis] > 2048))		//quadrant #1
		Fh.FinalHeading = 270 + Fh.FinalHeading;
	
	if((ADresult[ad_x_axis] > 2048) && (ADresult[ad_y_axis] > 2048))		//quadrant #2
		Fh.FinalHeading = 90 - Fh.FinalHeading;
	
	if((ADresult[ad_x_axis] > 2048) && (ADresult[ad_y_axis] < 2048))		//quadrant #3
		Fh.FinalHeading = 90 + Fh.FinalHeading;
	
	if((ADresult[ad_x_axis] < 2048) && (ADresult[ad_y_axis] < 2048))		//quadrant #4
		Fh.FinalHeading = 270 - Fh.FinalHeading;
}
	
	
/*-----------------------------------------------
Function: Bin8_ToBCD
Overview: convert hex values to BCD
Input: char data
Output: None
-----------------------------------------------*/
void	Bin8_ToBCD(char dtemp)
{			
	BCDNumDig3 = dtemp / 0x64;
	BCDNumDig2 = ( dtemp - (BCDNumDig3*0x64) )/10;
	BCDNumDig1 = ( dtemp - (BCDNumDig3*0x64)-(BCDNumDig2*10) );
	
	BCDNumDig2 = BCDNumDig2<<4;
	BCDNumDig1 = BCDNumDig2|BCDNumDig1;
}


/*-----------------------------------------------
Function: DelayFor18TCY
Overview: 18 TCY delay required for LCD
Input: None
Output: None
-----------------------------------------------*/
void DelayFor18TCY(void)						//delay for 18 TCY cyclces
{
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();	
}

/*-----------------------------------------------
Function: DelayPORXLCD
Overview: 15ms delay for LCD
Input: None
Output: None
-----------------------------------------------*/
void DelayPORXLCD(void)					//delay for 15ms @ 4MHz
{
	int i;
	for ( i = 0; i < 748; i++ );
}

/*-----------------------------------------------
Function: DelayXLCD
Overview: 5ms delay for LCD
Input: None
Output: None
-----------------------------------------------*/
void DelayXLCD(void)					//delay for 5ms @ 4MHz
{
	int i;
	for ( i = 0; i < 248; i++ );
}


/*-----------------------------------------------
Function: Delay1Sec
Overview: 1 second delay
Input: None
Output: None
-----------------------------------------------*/
void Delay1Sec(void)
{
	unsigned int i;
	for ( i = 0; i < 45000; i++ );
}

/*-----------------------------------------------
Function: DelayHalfSec
Overview: half second delay
Input: None
Output: None
-----------------------------------------------*/
void DelayHalfSec(void)
{
	unsigned int i;
	for ( i = 0; i < 22500; i++ );
}

/*-----------------------------------------------
Function: EERead
Overview: Reads EEPROM
Input: unsigned char - EEPROM address to be read
Output: char retrieved data
-----------------------------------------------*/	
char EERead(unsigned char EEAdress)
{
	char ReadData;
	EEADR = EEAdress;				//load EEADR with value to be read
	EECON1bits.EEPGD = 0;			//Point to DATA memory
	EECON1bits.CFGS = 0;			//Point to DATA memory
	EECON1bits.RD = 1;				//Read address in EEPROM
	ReadData = EEDATA;
	return	ReadData;
}

/*-----------------------------------------------
Function: EEWrite
Overview: Writes data to specific EEPROM address
Input: unsigned char EEPROM address,  char data
Output: None
-----------------------------------------------*/	
void EEWrite(unsigned char EEAdress, char EEData)
{
	EEADR = EEAdress;				//load EEADR with value to be written
	EEDATA = EEData;				//move write value
	EECON1bits.EEPGD = 0;			//Point to DATA memory
	EECON1bits.CFGS = 0;			//Point to DATA memory
	EECON1bits.WREN = 1;			//Enable Writes
	INTCONbits.GIE = 0;				//disable interrupts
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;				//beign writes
//	INTCONbits.GIE = 1;				//enable interrupts
		
	while(PIR2bits.EEIF == 0);    	//wait until write completes
	EECON1bits.WREN = 0;			//disable Writes
	PIR2bits.EEIF = 0;				//clear write flag
}

/*-----------------------------------------------
Function: PRDisplay
Overview: Displays Pitch and Roll values on LCD for calibration
Input: None
Output: None
-----------------------------------------------*/
void PRDisplay(void)									
{
	MeasurePitch();									//measure pitch angle			
	MeasureRoll();									//measure roll angle

	DelayXLCD();									//short delay here
	Bin8_ToASCII(PitchAngle);
	LCDByteAddr(BCDNumDig3, 0);						//write Pitch angle to LCD
	LCDWriteByte(BCDNumDig2);
	LCDWriteByte(BCDNumDig1);
	LCDMessageAddr("Pitch", 3);						// "xxxPitch"
	DelayPORXLCD();	
	DelayPORXLCD();
	DelayPORXLCD();

	Bin8_ToASCII(RollAngle);
	LCDByteAddr(BCDNumDig3, 0x40);					//write Roll angle to LCD
	LCDWriteByte(BCDNumDig2);
	LCDWriteByte(BCDNumDig1);
	LCDMessageAddr("Roll", 0x43);					// "xxx Roll"
	DelayPORXLCD();
	DelayPORXLCD();
	DelayPORXLCD();
}


/*-----------------------------------------------
Function: init
Overview: initalization
Input: None
Output: None
-----------------------------------------------*/
void	init(void)						//device initalization
{

//----- PORT configuration
	PORTA = 0b00000000;
	TRISA = 0b11101111;
	PORTB = 0b00000000;
	TRISB = 0b00010000;
	PORTC = 0b00000000;
	TRISC = 0b00000000;
	
	INTCON2bits.RBPU = 0;				//turn on PORTB pull-ups
	
//----- A/D configuration
	ADOverInit();						//A/D oversampling initalization
	
	ADCON1bits.PCFG0 = 1;				//Vss & Vdd, AN0 - AN3
	ADCON1bits.PCFG1 = 1;
	ADCON1bits.PCFG2 = 0;
	ADCON1bits.PCFG3 = 1;
	ADCON1bits.VCFG0 = 0;
	ADCON1bits.VCFG1 = 0;
	
	ADCON2bits.ACQT0 = 0;				//acquisition time = 0 Tad
	ADCON2bits.ACQT1 = 1;
	ADCON2bits.ACQT2 = 1;

//----- TIMER1 configuration			//used as a counter 
	T1CONbits.RD16  = 	0;				//1:1 prescale
	T1CONbits.T1RUN  = 	0;
	T1CONbits.T1CKPS1 = 0;				
	T1CONbits.T1CKPS0 = 0;
	T1CONbits.T1OSCEN = 0;
	T1CONbits.T1SYNC = 	0;
	T1CONbits.TMR1CS = 	0;
	T1CONbits.TMR1ON = 	1;				//T1 enabled
		
//----- TIMER2 configuration
	T2CONbits.T2CKPS0  = 0;				//1:1 prescale, 1:1 postscale
	T2CONbits.T2CKPS1  = 0;
	T2CONbits.TMR2ON   = 1;				//T2 enabled
	T2CONbits.T2OUTPS0 = 0;
	T2CONbits.T2OUTPS1 = 0;
	T2CONbits.T2OUTPS2 = 0;
	T2CONbits.T2OUTPS3 = 0;
	
//----- PWM configuration
	CCP1CONbits.CCP1M0 = 1;
	CCP1CONbits.CCP1M1 = 1;
	CCP1CONbits.CCP1M2 = 1;
	CCP1CONbits.CCP1M3 = 1;
	CCP1CONbits.DC1B0  = 0;
	CCP1CONbits.DC1B1  = 0;	

//----- USART configuration
//	TXSTAbits.TX9D  = 1;
//	TXSTAbits.TRMT  = 1;
	TXSTAbits.BRGH  = 1;
//	TXSTAbits.SENDB = 1;
	TXSTAbits.SYNC  = 0;
	TXSTAbits.TXEN  = 1;
	TXSTAbits.TX9   = 0;
	TXSTAbits.CSRC  = 1;
	
//	RCSTAbits.RX9D  = 1;
//	RCSTAbits.OERR  = 1;
//	RCSTAbits.FERR  = 1;
//	RCSTAbits.ADEN  = 0;
	RCSTAbits.CREN  = 1;
//	RCSTAbits.SREN  = 1;
	RCSTAbits.RX9   = 0;
	RCSTAbits.SPEN  = 1;
	
	BAUDCONbits.ABDEN  = 0;
	BAUDCONbits.WUE    = 0;
	BAUDCONbits.BRG16  = 0;
//	BAUDCONbits.SCKP   = 1;
//	BAUDCONbits.RCIDL  = 1;
	BAUDCONbits.ABDOVF = 0;
	
	SPBRG = 0x0C;
	
//----- LCD configuration

	OpenXLCD(FOUR_BIT & LINES_5X7);		//initalize LCD
}






