#define	 SetData	GpioDataRegs.GPADAT.bit.GPIO20 = 1
#define	 ClrData   	GpioDataRegs.GPADAT.bit.GPIO20 = 0
#define	 SetClk		GpioDataRegs.GPADAT.bit.GPIO22 = 1
#define	 ClrClk		GpioDataRegs.GPADAT.bit.GPIO22 = 0

#define	 SetRS		GpioDataRegs.GPADAT.bit.GPIO8 = 1
#define	 ClrRS		GpioDataRegs.GPADAT.bit.GPIO8 = 0
#define	 SetRW		GpioDataRegs.GPADAT.bit.GPIO9 = 1
#define	 ClrRW		GpioDataRegs.GPADAT.bit.GPIO9 = 0
#define	 SetE		GpioDataRegs.GPADAT.bit.GPIO10 = 1
#define	 ClrE		GpioDataRegs.GPADAT.bit.GPIO10 = 0

#define	 SetDACS1	0x01
#define  ClrDACS1	0xFE
#define	 SetDACS2	0x02
#define	 ClrDACS2	0xFD
#define	 SetDAOUT	0x04
#define	 ClrDAOUT	0xFB
#define	 SetDACLK	0x08
#define  ClrDACLK	0xF7
#define  SetLOE     0x20
#define  ClrLOE     0xBF

#define   OutReg	(*((volatile  Uint16 *)0x40FF))
#define	  LedReg	(*((volatile  Uint16 *)0x41FF))
#define   SysReg    (*((volatile  Uint16 *)0x42FF))    
#define   AicReg	(*((volatile  Uint16 *)0x43FF))    
#define   KeyReg    (*((volatile  Uint16 *)0x44FF))
#define   InlReg	(*((volatile  Uint16 *)0x45FF))
#define   InhReg    (*((volatile  Uint16 *)0x46FF))
#define   IntReg    (*((volatile  Uint16 *)0x47FF))

	


