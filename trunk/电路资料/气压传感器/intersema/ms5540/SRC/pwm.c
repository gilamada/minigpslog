//###########################################################################
//
// FILE:    pwm.c
//
// TITLE:   PWM发生器配置程序
//
// DESCRIPTION:
//			配置PWM寄存器
//			ePWM：EPWM1产生PWM波形控制泵，EPWM2定时触发ADC中断
//
//###########################################################################
//  Release: 
//  Date:		 2008.11.15
//	Modified:	 2008.11.15
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


/////////////////////////////////////////////////////////////
/////PWM寄存器初值	/////

// Configure the period for each timer
#define EPWM1_TIMER_TBPRD  230  //PWM频率7.6KHz，30MHz系统时钟
#define EPWM1_MAX_CMPA      229
#define EPWM1_MIN_CMPA        1
#define EPWM1_MAX_CMPB      229
#define EPWM1_MIN_CMPB        1

// To keep track of which way the compare value is moving
#define EPWM_CMP_UP   1
#define EPWM_CMP_DOWN 0

Uint16 pwm_m=115;


typedef struct
{
   volatile struct EPWM_REGS *EPwmRegHandle;
   Uint16 EPwm_CMPA_Direction;
   Uint16 EPwm_CMPB_Direction;
   Uint16 EPwmTimerIntCount;
   Uint16 EPwmMaxCMPA;
   Uint16 EPwmMinCMPA;
   Uint16 EPwmMaxCMPB;
   Uint16 EPwmMinCMPB;
}EPWM_INFO;


void update_compare(EPWM_INFO*);

EPWM_INFO epwm1_info;


//////////////////////////////////////////////////////
//////PWM中断	////////

interrupt void epwm_isr(void)
{
   // Update the CMPA and CMPB values
   update_compare(&epwm1_info);

   // Clear INT flag for this timer
   EPwm1Regs.ETCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 3
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

/////////////////////////////////////////////////////
/////初始化PWM寄存器	/////

void pwm_init(void)
{

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   // Setup TBCLK
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
   EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;   // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;	//寄存器值，要查对应的分频凳趴梢运鉚BCLK的值

 // Setup shadow register load on ZERO
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   // Set Compare values
   EPwm1Regs.CMPA.half.CMPA = EPWM1_MIN_CMPA;    // Set compare A value
   EPwm1Regs.CMPB = EPWM1_MIN_CMPB;              // Set Compare B value

   // Set actions
   EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;            // Set PWM1A on Zero
   EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;          // Clear PWM1A on event A, up count

   EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;            // Set PWM1B on Zero
   EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Clear PWM1B on event B, up count

   // Interrupt where we will change the Compare Values
   EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
   EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
   EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

   // Information this example uses to keep track
   // of the direction the CMPA/CMPB values are
   // moving, the min and max allowed values and
   // a pointer to the correct ePWM registers
   epwm1_info.EPwm_CMPA_Direction = EPWM_CMP_UP; // Start by increasing CMPA & CMPB
   epwm1_info.EPwm_CMPB_Direction = EPWM_CMP_UP;
   epwm1_info.EPwmTimerIntCount = 0;             // Zero the interrupt counter
   epwm1_info.EPwmRegHandle = &EPwm1Regs;        // Set the pointer to the ePWM module
   epwm1_info.EPwmMaxCMPA = EPWM1_MAX_CMPA;      // Setup min/max CMPA/CMPB values
   epwm1_info.EPwmMinCMPA = EPWM1_MIN_CMPA;
   epwm1_info.EPwmMaxCMPB = EPWM1_MAX_CMPB;
   epwm1_info.EPwmMinCMPB = EPWM1_MIN_CMPB;

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;

}


////////////////////////////////////////////////////
/////更新PWM占空比	///////

void update_compare(EPWM_INFO *epwm_info)
{
   if(epwm_info->EPwmTimerIntCount == 10)
   {
       epwm_info->EPwmTimerIntCount = 0;

	   EPwm1Regs.CMPA.half.CMPA = pwm_m;//更改小频PWM
   }
   else
   {
      epwm_info->EPwmTimerIntCount++;
   }

   EPwm1Regs.CMPB=150;

/*
///////////////////////////////////////////////////////////////////
//////	PID运算	///////

	Up = ref - feb;		//差值运算
	if(Up>=Pmax)Up=Pmax;	//差值限幅
	if(Up<=-Pmax)Up=-Pmax;	//差值限幅

	Uo = Kp*Up + Ki*Ui + Kd*Ud;		//PID调节
	Ud = Ui;
	Ui = Up;

	err=(int)Uo;	//强制转换成整数进行下面的计算
	PWM+=err;		//调整PWM输出

	if(PWM>=Umax)PWM=Umax;		//输出值限幅，防止积分饱和
	if(PWM<=-Umax)PWM=-Umax;	//输出值限幅，防止积分饱和

///////////////////////////////////////////////////////////////////
*/

/*
///////////////////////////////////////////////////////////////////
//////	模糊运算	//////

	Up = ref - feb;					//差值运算

	if(Up>p1)pwm+=q1;				//最大波动
	else if(Up<-p1)pwm-=q1;

	if(Up<=p1&&Up>=p2)pwm+=q2;		//一级波动
	else if(Up>=-p1&&Up<=-p2)pwm-=q2;

	if(Up<=p2&&Up>=p3)pwm+=q3;		//二级波动
	else if(Up>=-p2&&Up<=-p3)pwm-=q3;

	if(Up<=p3&&Up>=p4)pwm+=q4;		//三级波动
	else if(Up>=-p3&&Up<=-p4)pwm-=q4;

	if(Up<=p4&&Up>=p5)pwm+=q5;		//最小波动
	else if(Up>=-p4&&Up<=-p5)pwm-=q5;

///////////////////////////////////////////////////////////////////
*/

	/*if(epwm_info->EPwm_CMPA_Direction == EPWM_CMP_UP)
	   {
	       if(epwm_info->EPwmRegHandle->CMPA.half.CMPA < epwm1_info.EPwmMaxCMPA)
	       {
	          epwm_info->EPwmRegHandle->CMPA.half.CMPA++;
	       }
	       else
	       {
	          epwm_info->EPwm_CMPA_Direction = EPWM_CMP_DOWN;
              epwm_info->EPwmRegHandle->CMPA.half.CMPA--;
	       }
	   }
	   else
	   {
	       if(epwm_info->EPwmRegHandle->CMPA.half.CMPA == epwm1_info.EPwmMinCMPA)
	       {
	          epwm_info->EPwm_CMPA_Direction = EPWM_CMP_UP;
	          epwm_info->EPwmRegHandle->CMPA.half.CMPA++;
	       }
	       else
	       {
	          epwm_info->EPwmRegHandle->CMPA.half.CMPA--;
	       }
	   }*/
   return;

}

