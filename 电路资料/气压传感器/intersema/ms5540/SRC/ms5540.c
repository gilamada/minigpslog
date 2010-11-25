

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define	 dout	GpioDataRegs.GPADAT.bit.GPIO4

#define	 din_set   	GpioDataRegs.GPADAT.bit.GPIO3 = 1
#define	 din_clr   	GpioDataRegs.GPADAT.bit.GPIO3 = 0

#define	 clk_set	GpioDataRegs.GPADAT.bit.GPIO2 = 1
#define	 clk_clr	GpioDataRegs.GPADAT.bit.GPIO2 = 0



extern void gpio_init(void);

extern void pwm_init(void);

interrupt void timer0_isr(void);

interrupt void epwm_isr(void);



void ms_w(Uint16 dat);

Uint16 ms_r(void);

Uint16 ms_read_w(Uint16 sel);

Uint16 ms_read_d(Uint16 sel);

void ms_rst(void);

void trans_num(double *pressure, double *temperature, Uint16 d1_arg, Uint16 d2_arg);
Uint16 change_W_to_C(Uint16 ix,Uint16 W1,Uint16 W2,Uint16 W3,Uint16 W4);



double fc[7];



void main(void)
{

	
	Uint16 w1,w2,w3,w4;
	Uint16 d1,d2;

	Uint16 i;

	double pressure, temperature;

   InitSysCtrl();
	
   DINT;
// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;;

   InitPieCtrl();

   InitPieVectTable();


   gpio_init();

   pwm_init();

   	EALLOW;   	 
   	PieVectTable.TINT0 = &timer0_isr;
  	PieVectTable.EPWM1_INT = &epwm_isr;  	
   	EDIS;  

   	InitCpuTimers();  
   	ConfigCpuTimer(&CpuTimer0, 30, 5000);//定时1mS？？
   	StartCpuTimer0();
  
    IER |= (M_INT1|M_INT3); 	//开放中断

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

   	EINT;  
   	ERTM; 


	EALLOW;
    SysCtrlRegs.WDCR= 0x002f;//起动看门狗，设置看门狗时钟为WDCLK=OSCCLK/512/32
    EDIS;

	ms_rst();


	while(1)
	{	
		 w1 = ms_read_w(1);
		 w2 = ms_read_w(2);
		 w3 = ms_read_w(3);
		 w4 = ms_read_w(4);

		 d1=ms_read_d(1);
		 d2=ms_read_d(2);


		 for (i=1; i<=6; i++)
         {
            fc[i] = (double) change_W_to_C (i, w1, w2, w3, w4);
         }

		 trans_num(&pressure, &temperature, d1, d2);
    }

}



interrupt void timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

   // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    CpuTimer0Regs.TCR.bit.TIF=1;
    CpuTimer0Regs.TCR.bit.TRB=1;

	ServiceDog();//喂狗

}


Uint16 change_W_to_C (Uint16 ix, Uint16 W1, Uint16 W2, Uint16 W3, Uint16 W4)
{
    Uint16 c=0;
    Uint16 x, y;

    switch(ix)
    {
        case 1:	c =(W1>>1) & 0x7FFF;
        		break;

        case 2:	x =(W3<<6) & 0x0FC0;
        		y = W4 & 0x003F;
        		c = x|y;
        		break;

        case 3:	c =(W4>>6) & 0x03FF;
        		break;

        case 4: c =(W3>>6) & 0x03FF;
        		break;

        case 5: x =(W1<<10) & 0x0400;
        		y =(W2>>6) & 0x03FF;
        		c = x|y;
        		break;

        case 6: c = W2 & 0x003F;
        		break;
    }
    return(c);
}



void trans_num(double *pressure, double *temperature, Uint16 d1_arg, Uint16 d2_arg)
{
    double dt, off, sens;
    double fd1, fd2, x;

    d1_arg = d1_arg & 0xFFFF;
    d2_arg = d2_arg & 0xFFFF;

    fd1 = (double) d1_arg;
    fd2 = (double) d2_arg;

    dt = fd2 -((8.0 * fc[5]) + 20224.0);
    off = fc[2] * 4.0 + (((fc[4]-512.0) * dt)/4096.0);
    sens = 24576.0 +  fc[1] + ((fc[3]* dt)/1024.0);
    x = ((sens * (fd1-7168.0))/16384.0)-off;
	
    if (pressure!=0)
        *pressure = 250.0+ x/32;
    if (temperature!=0)
        *temperature =20.0+((dt * (fc[6]+50.0))/10240.0);
}


void ms_rst(void)
{
	ms_w(0x15);
	ms_w(0x55);
	ms_w(0x40);
}


Uint16 ms_read_w(Uint16 sel)//要返回16位数据长度
{
	Uint16 w;//16位数据长度

	switch(sel)
	{
		case 1:	ms_w(0x1d);
				ms_w(0x50);
				w= ms_r();
				break;

		case 2:	ms_w(0x1d);
				ms_w(0x60);
				w = ms_r();
				break;

		case 3:	ms_w(0x1d);
				ms_w(0x90);
				w = ms_r();
				break;

		case 4:	ms_w(0x1d);
				ms_w(0xa0);
				w = ms_r();
				break;

		default:break;
	}

	return(w);

}




Uint16 ms_read_d(Uint16 sel)
{
	Uint16 d;//16位数据长度

	switch(sel)
	{
		case 1:	ms_w(0x0f);
				ms_w(0x40);
				DELAY_US(20000);//延时35ms??
				d= ms_r();
				break;

		case 2:	ms_w(0x0f);
				ms_w(0x20);
				DELAY_US(20000);//延时35ms??
				d = ms_r();
				break;

		default:break;
	}

	return(d);


}


void ms_w(Uint16 dat)//发送8位命令	
{
	Uint16	i;

	for	(i=0;i<8;i++)
		{
			if ((dat & 0x80)==0)din_clr;//数据送0
			else	din_set;			//数据送1
			dat <<= 1;	
											
			clk_clr;	//CLK上升沿写命令??
			clk_set;		
		}
}



Uint16 ms_r(void)//读取16位数据
{
	Uint16	i;
	Uint16 dat=0;

	for	(i=0;i<16;i++)
		{
			clk_set;			//CLK下降沿读数据
			clk_clr;
			
			if (dout) dat|=1;			
			dat <<= 1;						
				
		}

	return(dat);
}

		
//===========================================================================
// No more.
//===========================================================================
