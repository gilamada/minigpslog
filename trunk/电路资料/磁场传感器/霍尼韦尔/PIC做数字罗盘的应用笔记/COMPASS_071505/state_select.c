#define STATE_C
#include <state.h>
#undef STATE_C
#include <main.h>


/*-----------------------------------------------
Function: SelectState()
Overview: Configures the compass for 1 of 4 US time zones
Input: None
Output: None
-----------------------------------------------*/	
void SelectState(void)	
{	
	LCDMessageAddr(" Select ", 0);						//select state?
	LCDMessageAddr(" State ?", 0x40);
	Delay1Sec();
	
	if (Switch() == 0 )
	{
		char states = 0;
		
		while(states == 0)
		{
			if(states == 0)
			{
			LCDMessageAddr("Pacific ", 0);					//pacific zone states?
			LCDMessageAddr(" Zone ? ", 0x40);
			Delay1Sec();
				if (Switch() == 0 )
				{
						PacificStates();
						states = 1;
				}
			}
			
			if(states == 0)
			{		
			LCDMessageAddr("Mountain", 0);					//mountain zone states?
			Delay1Sec();
				if (Switch() == 0 )
				{
						MountainStates();
						states = 1;
				}
			}
		
			if(states == 0)
			{	
			LCDMessageAddr("Central ", 0);					//central zone states?
			Delay1Sec();
				if (Switch() == 0 )
				{
						CentralStates();
						states = 1;
				}
			}
		
			if(states == 0)
			{	
			LCDMessageAddr("Eastern ", 0);					//eastern zone states?
			Delay1Sec();
				if (Switch() == 0 )
				{
						EasternStates();
						states = 1;
				}
			}
		}
	}		
}

/*-----------------------------------------------
Function: PacificStates()
Overview: Provides state locations within the pacific time zone
Input: None
Output: None
-----------------------------------------------*/
void PacificStates(void)
{
	char region = 0;
	
	while(region == 0)
	{

		if(region == 0)
		{	
		LCDMessageAddr("No Angle", 0);					//No declination angle wanted?
		LCDMessageAddr(" Needed ", 0x40);
		Delay1Sec();
			if (Switch() == 0)
			{
				decl_angle = 0;
				region = 1;
			}
		}

		if(region == 0)
		{	
		LCDMessageAddr("WA      ", 0);					//Washington?
		LCDMessageAddr("Region ?", 0x40);
		Delay1Sec();
			if (Switch() == 0)
			{
				decl_angle = -21;
				region = 1;
			}
		}
		
		if(region == 0)
		{
		LCDMessageAddr("OR      ", 0);					//Oregon?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -19;
				region = 1;
				}
		}
		
		if(region == 0)
		{
		LCDMessageAddr("NV      ", 0);					//Nevada?
		Delay1Sec();
			if (Switch() == 0)
			{
				decl_angle = -16;
				region = 1;
			}
		}
		
		if(region == 0)
		{
		LCDMessageAddr("N. CA   ", 0);					//Northern Ca?
		Delay1Sec();
			if (Switch() == 0)
			{
				decl_angle = -18;
				region = 1;
			}
		}
		
		if(region == 0)
		{
		LCDMessageAddr("Mid CA  ", 0);					//Mid CA?
		Delay1Sec();
			if (Switch() == 0)
			{
				decl_angle = -16;
				region = 1;
			}
		}
		
		if(region == 0)
		{
		LCDMessageAddr("S. CA   ", 0);					//Southern Ca?
		Delay1Sec();
			if (Switch() == 0)
			{
				decl_angle = -14;
				region = 1;
			}
		}
			
		if(region == 0)
		{
		LCDMessageAddr(" Hawaii ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
			{
				decl_angle = -11;
				region = 1;
			}
		}		
	}
	EEWrite(DAngleAddr, decl_angle);					//write selected angle to EEPROM
}

/*-----------------------------------------------
Function: MountainStates()
Overview: Provides state locations within the mountain time zone
Input: None
Output: None
-----------------------------------------------*/
void MountainStates(void)
{
	char region = 0;
	
	while(region == 0)
	{
		if(region == 0)
			{	
		LCDMessageAddr("Arizona ", 0);					//Arizona?
		LCDMessageAddr("Region ?", 0x40);
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -13;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("  Utah  ", 0);					//Utah?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -15;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("NewMexco", 0);					//New Mexico
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -11;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Colorado", 0);					//Colorado?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -12;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Wyoming ", 0);					//whyoming?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -14;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr(" Idaho  ", 0);					//Idaho?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -18;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Montana ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -16;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr(" Alaska ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -10;
				region = 1;
				}
			}
		}
	EEWrite(DAngleAddr, decl_angle);					//write selected angle to EEPROM
}
	
/*-----------------------------------------------
Function: CentralStates()
Overview: Provides state locations within the Central time zone
Input: None
Output: None
-----------------------------------------------*/	
void CentralStates(void)
{
	char region = 0;
	
	while(region == 0)
	{
		if(region == 0)
			{	
		LCDMessageAddr("N.Dakota ", 0);					//?
		LCDMessageAddr("Region ?", 0x40);
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -10;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("S.Dakota", 0);					//Utah?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -10;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Nebraska", 0);					//New Mexico
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -10;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr(" Kansas ", 0);					//Colorado?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -8;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr(" Texas  ", 0);					//whyoming?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -8;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Oklahoma", 0);					//Idaho?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -7;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Lusiana ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -5;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Arkansas", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -5;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Missouri", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -4;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("  Iowa  ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -5;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Minesota", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -5;
				region = 1;
				}
			}
		}
	EEWrite(DAngleAddr, decl_angle);					//write selected angle to EEPROM
}
	
/*-----------------------------------------------
Function: EasternStates()
Overview: Provides state locations within the eastern time zone
Input: None
Output: None
-----------------------------------------------*/
void EasternStates(void)
{
	char region = 0;
	
	while(region == 0)
	{
		if(region == 0)
			{	
		LCDMessageAddr("Wiscosin", 0);					//?
		LCDMessageAddr("Region ?", 0x40);
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -1;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Illinois", 0);					//Utah?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -1;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Missippi", 0);					//New Mexico
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -3;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Tennesee", 0);					//Colorado?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 0;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Alabama ", 0);					//whyoming?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = -1;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Kytucky ", 0);					//Idaho?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 1;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Indy", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 1;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Mischcan", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 4;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("  Ohio  ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 4;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Vaginia", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 7;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Georgia", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 1;
				region = 1;
				}
			}
			
		if(region == 0)
			{	
		LCDMessageAddr("Florida", 0);					//?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 2;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("WVaginia", 0);					//Utah?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 2;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("SCarolna", 0);					//New Mexico
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 4;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("NCarolna", 0);					//Colorado?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 6;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Pennvaya", 0);					//whyoming?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 10;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("NwJersey", 0);					//Idaho?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 12;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("NewYork ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 12;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("RdIsland", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 15;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("Connict", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 15;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr("NewHamp ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 16;
				region = 1;
				}
			}
			
		if(region == 0)
			{
		LCDMessageAddr(" Maine  ", 0);					//Montana?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 19;
				region = 1;
				}
			}
			
		if(region == 0)
			{	
		LCDMessageAddr("Vermont ", 0);					//?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 15;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("  Mass  ", 0);					//Utah?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 15;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Maryland", 0);					//New Mexico
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 10;
				region = 1;
				}
			}
		
		if(region == 0)
			{
		LCDMessageAddr("Delaware", 0);					//Colorado?
		Delay1Sec();
			if (Switch() == 0)
				{
				decl_angle = 11;
				region = 1;
				}
			}		
		}
	EEWrite(DAngleAddr, decl_angle);					//write selected angle to EEPROM
}
