


#undef EXT
#ifdef 	MAIN_C
#define EXT

#else 
#define	EXT	extern
#endif



//EEPROM Addresses
#define 	DAngleAddr	0

//Variables
EXT	unsigned 	char 	decl_angle;

//Function Prototypes
EXT	void	LCDMessageAddr(const rom char *buffer, unsigned char DDaddr); 	//Pass a LCD message w/ line location
EXT	void	Delay1Sec(void);												//2 second delay
EXT	char	Switch(void);
EXT void 	EEWrite(unsigned char EEAdress, char EEData);
