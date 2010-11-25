//###########################################################################
//
// FILE:    gpio.c
//
// TITLE:   初始化GPIO程序
//
// DESCRIPTION:
//			配置各个外围器件使用的IO，配置PWM输出IO
//
//
//###########################################################################
//  Release: 
//  Date:		 2008.11.15
//	Modified:	 2008.11.15
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


void gpio_init(void)
{

/////////////////////////////////////////////////////////
/////初始化ePWM的GPIO	/////

    EALLOW;   

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // Enable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;    // Enable pull-up on GPIO1 (EPWM1B)      
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B 

    //GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;   //dst
    //GpioCtrlRegs.GPADIR.bit.GPIO1 = 0; 
	//GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;    // Enable pull-up 

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;   //rst
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; 
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;    // Enable pull-up  

	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;   //cs
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1; 
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;    // Enable pull-up

	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;   //a0
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 0; 
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    // Enable pull-up

	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;   //clk
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1; 
	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up

	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;   //18b20
    GpioCtrlRegs.GPADIR.bit.GPIO6 = 1; 
	GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up


/////初始化其他IO	/////
	//GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;   //方便检测使用
    //GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;

    //GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;   //k1
    //GpioCtrlRegs.GPADIR.bit.GPIO20 = 1; 

	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;   //k1
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0; 
	GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;    // Enable pull-up

    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;   //k2
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0; 
	GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;    // Enable pull-up 

    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;   //k3
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;    // Enable pull-up  

	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;   //k4
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0; 
	GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;    // Enable pull-up
/*
	GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 0;   //k4
    GpioCtrlRegs.GPCDIR.bit.GPIO80 = 1; 
	GpioCtrlRegs.GPCPUD.bit.GPIO80 = 0;    // Enable pull-up

	GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 0;   //k4
    GpioCtrlRegs.GPCDIR.bit.GPIO79 = 1; 
	GpioCtrlRegs.GPCPUD.bit.GPIO79 = 0;    // Enable pull-up
*/
    EDIS;
    
}

