/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/main.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

#define ADC_Filter_Num		10	// AD滤波的次数

#define ADC_Used_Num		3	//总共使用的通道数

//#define USE_REFINT_ADJUST		//使用17通道的参考值校准

#define ADC_Channel_Damp 	ADC_Channel_14
#define ADC_DAMP_SEQUENCE	1
#define	ADC_DAMP_GPIO_PIN	GPIO_Pin_4
#define	ADC_DAMP_GPIO_PORT	GPIOC

#define ADC_Channel_InsideTemp 	ADC_Channel_16
#define ADC_INTEMP_SEQUENCE		2

#define ADC_Channel_Refint 	ADC_Channel_17
#define ADC_REFINT_SEQUENCE	3

#define	ADC_RCC_ALL_PORT	RCC_APB2Periph_GPIOC

#define ADC_DMA_CHAN		DMA1_Channel1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t ADCConvertedValue[ADC_Filter_Num][ADC_Used_Num];

float ADCValue[ADC_Used_Num] = {0.0};

uint16_t temp_dr=0;
    
/* Private function prototypes -----------------------------------------------*/
  
/* Private functions ---------------------------------------------------------*/


void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/* Enable ADC1 and ADC_GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | ADC_RCC_ALL_PORT, ENABLE);

	/* ADCCLK = PCLK2/6 = 72M/6 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 

	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* Configure ADC_GPIOC as analog input -------------------------*/
#ifdef	ADC_DAMP_GPIO_PORT
	GPIO_InitStructure.GPIO_Pin = ADC_DAMP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_DAMP_GPIO_PORT, &GPIO_InitStructure);
#endif

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(ADC_DMA_CHAN);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_Filter_Num*ADC_Used_Num;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(ADC_DMA_CHAN, &DMA_InitStructure);
  
	/* Enable DMA1 channel1 */
	DMA_Cmd(ADC_DMA_CHAN, ENABLE);
     
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//在扫描模式（多通道）or 单次（单通道）模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//连续模式 or 单次模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据向右边对齐
	ADC_InitStructure.ADC_NbrOfChannel = ADC_Used_Num;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel configuration */ 	
#ifdef	ADC_Channel_Damp
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Damp, ADC_DAMP_SEQUENCE, ADC_SampleTime_55Cycles5);
#endif
#ifdef	ADC_Channel_InsideTemp
	ADC_RegularChannelConfig(ADC1, ADC_Channel_InsideTemp, ADC_INTEMP_SEQUENCE, ADC_SampleTime_55Cycles5);
#endif
#ifdef	ADC_Channel_Refint
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Refint, ADC_REFINT_SEQUENCE, ADC_SampleTime_55Cycles5);
#endif
  
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* 开启温度传感器和内部参考电压通道 */
//#ifdef	(ADC_Channel_Refint || ADC_Channel_InsideTemp)
	ADC_TempSensorVrefintCmd(ENABLE);
//#endif

	/* 重置指定的ADC的校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* 开始指定ADC的校准状态 */
	ADC_StartCalibration(ADC1);
	
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
    
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


void ADC_Start(void)
{
	ADC_Cmd(ADC1, ENABLE);
	DMA_Cmd(ADC_DMA_CHAN, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC_Stop(void)
{
	DMA_Cmd(ADC_DMA_CHAN, DISABLE);
	ADC_Cmd(ADC1, DISABLE);	
}

void ADC_Read(void)
{
	unsigned short i,j,minVal,maxVal,tmpVal[ADC_Used_Num];
	unsigned int Sum;
	
	ADC_Stop();

	for(i=0; i<ADC_Used_Num; i++)
	{
		Sum = 0;
		minVal = 4096;
		maxVal = 0;
		for(j=0; j<ADC_Filter_Num; j++)
		{
			
			Sum += ADCConvertedValue[j][i];
			if(minVal > ADCConvertedValue[j][i])
				minVal = ADCConvertedValue[j][i];

			if(maxVal < ADCConvertedValue[j][i])
				maxVal = ADCConvertedValue[j][i];
		}

		tmpVal[i] = (Sum - minVal - maxVal)/(ADC_Filter_Num - 2);
	}

#ifdef USE_REFINT_ADJUST
		for(i=0; i<ADC_Used_Num; i++)
			ADCValue[i] = 1.2 * (float)(tmpVal[i]/tmpVal[ADC_Used_Num-1]);
#else
		for(i=0; i<ADC_Used_Num; i++)
			ADCValue[i] = 3.33 * (float)(tmpVal[i]/4096);
#endif
}


/***********************************************************************
* 内部温度传感器: 连接到ADC_Channel_16
* 内部的参照电压: 连接到ADC_Channel_17
*
* 为修正参考电压波动带来的偏差，测量温度电压值之前，先读出参照电压的ADC
* 测量数值，记为ADrefint；再读出内部温度通道的ADC转换数值，记为ADtemp；
* 则内部温度通道的电压为：
*  Vsense = Vrefint * (ADtemp/ADrefint)  其中Vrefint为参照电压=1.20V。
*
* 温度公式: Temperature (in ℃) = {(V25 - VSENSE) / Avg_Slope} + 25
* 在STM32F103xx的数据手册中分别给出了V25和Avg_Slope的值：
* V25       最小=1.34V       典型=1.43V        最大=1.52V
* Avg_Slope 最小=4.0mV/℃    典型=4.3mV/℃     最大=4.6mV/℃   
*
* 例如读到Vsense = 1.30V。分别取V25和Avg_Slope的典型值，计算得到：
* (1.43 - 1.30)/0.0043 + 25 = 55.23
* 所以温度大约为 55℃(得到的温度只能是CPU的大概温度)
***********************************************************************/
#ifdef ADC_Channel_InsideTemp
float CPU_Inside_Temperature(unsigned short ADtemp, unsigned short ADrefint)
{
	float Vsense,InTemp;

	Vsense = 1.2 * (ADtemp/ADrefint);
	InTemp = (1.43 - Vsense)/0.0043 + 25;

	return InTemp;
}
#endif

