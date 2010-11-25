/***************************************************
**HAL.c
**主要用于芯片硬件的内部外围和外部外围的初始化，两大INIT函数
**在MAIN中调用，使MAIN函数中尽量与硬件库无关
***************************************************/

#include "stm32f10x.h"



//各个内部硬件模块的配置函数
extern void RCC_Configuration(void);			//RCC
extern void USART_Configuration(void);			//串口

/*******************************
**函数名:ChipHalInit()
**功能:片内硬件初始化
*******************************/
void  ChipHalInit(void)
{
	//初始化时钟源
	//RCC_Configuration();
	SystemInit();
	
	
	//初始化串口
	//USART_Configuration();
}


/*********************************
**函数名:ChipOutHalInit()
**功能:片外硬件初始化
*********************************/
void  ChipOutHalInit(void)
{
	//初始化SD
	//SDConfig();

	//FSMC_SRAM_Init();
}
