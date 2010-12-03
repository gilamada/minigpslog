//================================================================================================
// 文件名称: BSP.C
// 芯片类型: STM32F103VBT6                                           
// 开发软件: Keil, Windows XP2 
// 开发时间: 2010年06月18日                                                               
// 程序功能: STM32的硬件初始化程序
// 编写人员: 崔超                                                                       
// 联系方式: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com   
//================================================================================================ 

#include "config.h"

u32 volatile Tick_ms=0,Tick_s=0;

/***************************************************************************************
** 函数名称: GPIOX_Configuration
** 功能描述: 特殊的GPIO管脚配置
** 参    数: None
** 返 回 值: None       
****************************************************************************************/
static void GPIOX_Configuration(void)
{



}



/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : Configures the used Timers.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM_Configuration(void)
{ 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

  /* TIM2 configuration */			   //定时1s
  TIM_TimeBaseStructure.TIM_Period = 24-1;          
  TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  TIM_OCStructInit(&TIM_OCInitStructure);
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;                   
  TIM_OCInitStructure.TIM_Pulse = 0x0;  
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);


  /* Immediate load of TIM2 Precaler value */
  TIM_PrescalerConfig(TIM2, 1000-1, TIM_PSCReloadMode_Immediate);


  /* Clear TIM2 update pending flag */
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);


  /* Enable TIM2 Update interrupt */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

}






void PWM_Configuration(u16 DutyCycle) 
{ 


      GPIO_InitTypeDef GPIO_InitStructure;  	
 
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure; 
    TIM_OCInitTypeDef TIM_OCInitStructure; 
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure; 



     GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);//以前没注意这个，没这个就不行 
 		
//PD13 复用输出
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
GPIO_Init(GPIOD, &GPIO_InitStructure); 

      //启动TIM1 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

    //TIM1基本计数器设置（设置PWM频率） 
    //频率=TIM1_CLK/(ARR+1) 
    TIM_BaseInitStructure.TIM_Period = 1000-1; 
    TIM_BaseInitStructure.TIM_Prescaler = 24-1; 
    TIM_BaseInitStructure.TIM_ClockDivision = 0; 
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0; 
    TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure); 
    //启用ARR的影子寄存器（直到产生更新事件才更改设置） 
    TIM_ARRPreloadConfig(TIM4, ENABLE); 

   
    //TIM1_OC1模块设置（设置1通道占空比） 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; 
    TIM_OCInitStructure.TIM_Pulse = DutyCycle; 
  //  TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
    //启用CCR1寄存器的影子寄存器（直到产生更新事件才更改设置） 
  //  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
     

    //TIM2_OC2模块设置（设置2通道占空比） 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = DutyCycle; 
    TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
    //启用CCR2寄存器的影子寄存器（直到产生更新事件才更改设置） 
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
    
    //死区设置 
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable; 
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable; 
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF; 
    TIM_BDTRInitStructure.TIM_DeadTime = 0x90;  //这里调整死区大小0-0xff 
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable; 
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High; 
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; 
    TIM_BDTRConfig(TIM4, &TIM_BDTRInitStructure); 
    
    //TIM1开启 
    TIM_Cmd(TIM4, ENABLE); 
    //TIM1_OC通道输出PWM（一定要加） 
    TIM_CtrlPWMOutputs(TIM4, ENABLE); 

} 
  

/***************************************************************************************
** 函数名称: NVIC_Configuration
** 功能描述: stm32f10x配置基本向量中断
** 参    数: None
** 返 回 值: None       
****************************************************************************************/
static void NVIC_Configuration(void)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	
	#ifdef  VECT_TAB_RAM  											/* 重映射向量表        */
  	/* Set the Vector Table base location at 0x20000000 */ 
  	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x200); 
	#else  /* VECT_TAB_FLASH  */
 	 /* Set the Vector Table base location at 0x08000000 */ 
 	 NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		/* 主从优先级配置(4*4) */


  /* Enable the TIM2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  

  /* Enable the TIM4 Interrupt */
 /* 
 	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
 */	 
	// Enable the USART1 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the USART3 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

       NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 }
/***************************************************************************************
** 函数名称: RCC_Configuration
** 功能描述: stm32f10x时钟系统配置
** 参    数: None
** 返 回 值: None       
****************************************************************************************/

 void RCC_Configuration(void)
{
  
  

    //下面是给各模块开启时钟
    //启动GPIO
    RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA |
                             RCC_APB2Periph_GPIOB | 
                             RCC_APB2Periph_GPIOC | 
                             RCC_APB2Periph_GPIOD, ENABLE);     
    //启动AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
   /*
    //启动DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    //启动ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	  	// Enable TIM2, TIM3 and TIM4 clocks 
  	RCC_APB1PeriphClockCmd(  RCC_APB1Periph_TIM2 |
                             RCC_APB1Periph_TIM3 |
                             RCC_APB1Periph_TIM4, ENABLE);


   */

}





 /*******************************************************************************
* Function Name  : USART1_Configuration
* Description    : NUSART1设置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_Configuration(u32 BaudRate)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

     	  /* Enable GPIO  clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    	//USART1_TX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

        //启动USART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
   
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    USART_Cmd(USART1, ENABLE);
}

 /*******************************************************************************
* Function Name  : USART1_Configuration
* Description    : NUSART1设置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_Configuration(u32 BaudRate)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

     	  /* Enable GPIO  clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    	//USART3_TX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //USART3_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

//	printf("\r");	//不知道为什么这个加上第1个字节才能发出去. 据说是内部移位寄存器的毛病?


        //启动USART3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	/*
		USART3 configured as follow:
	    - BaudRate = 4800 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);

	// Enable the USART Receive interrupt: this interrupt is generated when the
	// USART1 receive data register is not empty
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);
}
/***************************************************************************************
** 函数名称: SysTick_Init
** 功能描述: 系统定时器初始化函数
** 参    数: 装载值
** 返 回 值: None       
****************************************************************************************/

void SysTick_Init(void)
{
  if (SysTick_Config(SystemCoreClock_Mhz*1000)) /* SystemFrequency is defined in “system_stm32f10x.h” and equal to HCLK frequency */
  {
     while (1);
  }
}
/***************************************************************************************
** 函数名称: TargetResetInit
** 功能描述: 目标板基本初始化
** 参    数: None
** 返 回 值: None       
****************************************************************************************/
void TargetResetInit (void)
{
    	

	RCC_Configuration();  
    NVIC_Configuration();
		/* GPIO端口配置 */
	GPIOX_Configuration();

     #ifdef BACKLIGTH_PWM	

	PWM_Configuration(0);
    #endif 	

	USART1_Configuration(9600);
	USART3_Configuration(9600);
//	TIM_Configuration(); 
							   
    SysTick_Init();
}

/***************************************************************************************
** 函数名称: Delay_ms
** 功能描述: 延时函数
** 参    数: 延时毫秒数
** 返 回 值: None       
****************************************************************************************/

void Delay_ms(u32 ms)
{

  u32 counter;
  
  counter=Tick_ms;
  while( Tick_ms-counter < ms);	 

}
/***************************************************************************************
** 函数名称: Delay_s
** 功能描述: 延时函数
** 参    数: 延时秒数
** 返 回 值: None       
****************************************************************************************/

void Delay_s(u16 s)
{

  Delay_ms(1000*s); 

}
/*******************************************************************************
* Function Name  : TimingDelay_Decrement
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void TimingDelay_Decrement(void)
{
  Tick_ms++;
  if(Tick_ms%500 == 0) 
  {
     LED_Reverse(LED1);
  }
  if(Tick_ms%1000 == 0) 
  Tick_s++;


 
}
   
/***************************************************************************************
**  End Of File                                                     
***************************************************************************************/
