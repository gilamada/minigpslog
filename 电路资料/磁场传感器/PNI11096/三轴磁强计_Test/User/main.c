/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.0
* Date               : 05/23/2008
* DescriptWHY8W-QT9DJ-P0VNX-L15PE-W4DCR-VP3FMion        : Main program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED 
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

/*********************user define***************************************************/
#include "..\FWLib\inc\main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define X_Dir 1
#define Y_Dir 2
#define Z_Dir 3

#define Set_MagnetoReset GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET); //将SPI1_Res拉高
#define Clr_MagnetoReset GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET); //将SPI1_Res拉低

/* 全局变量 -------------------------------------------------------------*/
volatile u32 TimingDelay;
u16 Magneto_xyz_Data[3]; 

/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
USART_InitTypeDef USART_InitStructure;
SPI_InitTypeDef SPI_InitStructure;

/* Private function prototypes 外设配置 -----------------------------------------------*/
void stm32_Init(void);
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void SysTick_Configuration(void);
void USART_Configuration(void);
void SPI_Configuration(void);

/*----------------------------- 外设初始化 --------------------------------------------*/
void Magneto_Enable(void); //三轴磁强计使能，即拉低SSNOT
void Magneto_Disable(void); //三轴磁强计失能，即拉高SSNOT
void Magneto_Data_Updata(u8 dir); //dir可取为为X_Dir,Y_Dir,Z_Dir

/* Private functions ---------------------------------------------------------*/
void Led_On(void);
void Led_Off(void);
void SysTick_Interrupt(void);
void Delay_ms(u32 ms);
void USART1_Interrupt(void);
void USART1_SendInt(u16 data);
//void SPI1_Interrupt(void);

/**************************************************************************
			  函数名：更新三轴磁强计的三轴数据
				功能：
				说明：
***************************************************************************/
//void Magneto_Data_Updata(u8 dir)
//{
//	u32 i;
	//u16 tmp1,tmp2;

	//给RES一个脉冲，使其复位，且每次测数据都要复位
		//GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET); //将SPI1_Res拉高	
//	for(i=0;i<500;i++);
		//Delay_ms(1);
		//GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET); //将SPI1_Res拉低

	/*SPI1->CR1 &=~(0x0040); //取消SPE位，失能SPI1
	SPI1->CR1 &=~(0x0800); //取消DFF位，设置为8位传输
	SPI1->CR1 |=0x0040; //置SPE位，使能能SPI1*/

		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==SET);	 //等待DRDY为低

	/****************************************************************
		要发送的命令数据格式:(BIT7-BIT0)DHST PS2 PS1 PS0 ODIR MOT AS1 ASO
		AS1 AS0决定所测轴：X 0 1 ,Y 1 0 ,Z 1 1  			
		MOT:0
		ODIR:0
		PS2 PS1 PS0 0 0 0    32分频
		DHSH 1
		即测X轴发：0xa1  测Y轴发：0xa2  测Z轴发：0xa3
	*****************************************************************/
		//SPI_I2S_SendData(SPI1,(0x20|dir));  //测dir轴
//	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);

	/*SPI1->CR1 &=~(0x0040); //取消SPE位，失能SPI1
	SPI1->CR1 |= (0x0800); //置DFF位，设置为16位传输
	SPI1->CR1 |=0x0040; //置SPE位，使能能SPI1*/
	//for(i=0;i<150;i++);
		//Delay_ms(1);
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==RESET);   //等待DRDY为高
	//Delay_ms(1);
	
//	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);
	//while((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==RESET)||(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET));	 //等待DRDY为高,接受缓存非空
	//SPI_I2S_SendData(SPI1, 0x00);  //测dir轴	
	//Magneto_xyz_Data[dir-1] =SPI_I2S_ReceiveData(SPI1);//接受dir轴的数据
	//SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
	//SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
		//tmp1 = SPI_I2S_ReceiveData(SPI1);//接受dir轴的数据
	//SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
//	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);
		//tmp2 = SPI_I2S_ReceiveData(SPI1);//接受dir轴的数据
		//Magneto_xyz_Data[dir-1] = ((tmp1<<8) |tmp2);

	/*SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
	tmp1 = SPI1->DR;
	SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
	tmp2 = SPI1->DR;
	Magneto_xyz_Data[dir-1]=((tmp1<<8)|tmp2);*/

	/*tmp  = (SPI_I2S_ReceiveData(SPI1)<<8);
	tmp |= SPI_I2S_ReceiveData(SPI1);
	Magneto_xyz_Data[dir-1]=tmp;*/

	/*tmp1 = SPI_I2S_ReceiveData(SPI1); //先得高8位
	tmp2 = SPI_I2S_ReceiveData(SPI1); //后得低8位;
	Magneto_xyz_Data[(dir-1)]=((tmp1<<8) | tmp2);*/
//}

void Magneto_Data_Updata(u8 dir)
{
	u16 tmp1,tmp2;	

	/*************重置磁强计******************/
	Set_MagnetoReset   	//将SPI1_Res拉高
	Delay_ms(1);
	Clr_MagnetoReset	//将SPI1_Res拉低

	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==SET);	 //等待DRDY为低

	/****************************************************************
		要发送的命令数据格式:(BIT7-BIT0)DHST PS2 PS1 PS0 ODIR MOT AS1 ASO
		AS1 AS0决定所测轴：X 0 1 ,Y 1 0 ,Z 1 1  			
		MOT:0
		ODIR:0
		PS2 PS1 PS0 0 0 0    32分频
		DHSH 1
		即测X轴发：0xa1  测Y轴发：0xa2  测Z轴发：0xa3
	*****************************************************************/
//	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == RESET);
	SPI_I2S_SendData(SPI1,(0x20|dir));  //测dir轴
	

	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==RESET);   //等待DRDY为高
	//while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);	

	//SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
	//tmp1 = SPI1->DR;
	//SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
	//tmp2 = SPI1->DR;
	//Magneto_xyz_Data[dir-1]=((tmp1<<8)|tmp2);

	SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
	tmp1 = SPI_I2S_ReceiveData(SPI1); //先得高8位
	//SPI_I2S_SendData(SPI1, 0x00);  //测dir轴
	tmp2 = SPI_I2S_ReceiveData(SPI1); //后得低8位;
	Magneto_xyz_Data[(dir-1)]=((tmp1<<8) | tmp2);
}

/**************************************************************************
			  函数名：LED0,LED1点亮
				功能：
				说明：
***************************************************************************/
void Led_On(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
}

/**************************************************************************
			  函数名：LED0,LED1熄灭
				功能：
				说明：
***************************************************************************/
void Led_Off(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
}

/**************************************************************************
			  函数名：SPI1的中断处理函数
				功能：
				说明：
***************************************************************************/
/*void SPI1_Interrupt(void)
{
	
} */

/**************************************************************************
			  函数名：三轴磁强计使能
				功能：
				说明：
***************************************************************************/
void Magneto_Enable(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET); //将SPI1_NSS拉低
	Delay_ms(1);//延时要大于100NS
}

/**************************************************************************
			  函数名：三轴磁强计失能
				功能：
				说明：
***************************************************************************/
void Magneto_Disable(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET); //将SPI1_NSS拉高
	Delay_ms(1);//延时要大于100NS
}

/**************************************************************************
			  函数名：串口1的中断处理函数
				功能：
				说明：
***************************************************************************/
void USART1_Interrupt(void)
{
	u8 RxData;
	u8 TxData;

	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET)
	{
		RxData = USART_ReceiveData(USART1);//接收8位的数据
		TxData=RxData;
		USART_SendData(USART1, (u8)TxData);//将接收到的数据发送出去
	}	
}

/**************************************************************************
			  函数名：串口1发送16位的数据
				功能：
				说明：
***************************************************************************/
void USART1_SendInt(u16 data)
{
	u8 tmp[2];
	tmp[0]=data;    //低8位
	tmp[1]=data>>8; //高8位
	while (!(USART1->SR & USART_FLAG_TXE)); //判断发送是否为空
  	USART1->DR = (tmp[1] & 0x1FF);	 //发送高字节
	while (!(USART1->SR & USART_FLAG_TXE)); //判断发送是否为空
    USART1->DR = (tmp[0] & 0x1FF);	 //发送低字节
}

/**************************************************************************
			  函数名：MS级延时函数
				功能：
				说明：
***************************************************************************/
void Delay_ms(u32 ms)
{
	/*u32 a;
	u8 b;
	for(a=0;a<(ms*1000);a++)
		for(b=0;b<72;b++);*/
	SysTick_CounterCmd(SysTick_Counter_Enable);
	TimingDelay=ms;
	while(TimingDelay != 0);
	SysTick_CounterCmd(SysTick_Counter_Disable);
	SysTick_CounterCmd(SysTick_Counter_Clear);
}

/**************************************************************************
			  函数名：SysTick中断函数
				功能：
				说明：
***************************************************************************/
void SysTick_Interrupt(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

/**************************************************************************
			  函数名：串口初始化
				功能：
				说明：
***************************************************************************/
void USART_Configuration(void)
{
	USART_InitStructure.USART_BaudRate=38400;//波特率为38400		
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8位数据位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//失能硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);//配置串口1
	//USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能串口1接受中断
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//使能串口2接受中断

	USART_Cmd(USART1, ENABLE);//使能外设串口1
	//USART_Cmd(USART2, ENABLE);//使能外设串口2
}

/**************************************************************************
			  函数名：SPI1初始化
				功能：
				说明：
***************************************************************************/
void SPI_Configuration(void)
{
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;//主SPI
	SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;//收发数据为8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //时钟悬空高
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;   //数据捕获于第一个时钟沿
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	 //内部NSS信号有SSI位控制
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//时钟64分频，即（72/128）M，SPI时钟最大值为18M，PNI11096的时钟最大值是1M
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_SSOutputCmd(SPI1, ENABLE);

//	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);	 //开启接受非空中断
	SPI_Cmd(SPI1, ENABLE);
}

/**************************************************************************
			  函数名：SysTick初始化
				功能：
				说明：
***************************************************************************/
void SysTick_Configuration(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //SysTick时钟源为AHB时钟除以8，及为9M
	NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 1, 0);
	SysTick_SetReload(9000);
	SysTick_ITConfig(ENABLE);
}

void stm32_Init(void)
{
	RCC_Configuration();
	NVIC_Configuration();
	GPIO_Configuration();
	SysTick_Configuration();
	USART_Configuration();
	SPI_Configuration();
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
#ifdef DEBUG
    debug();
#endif
    
	stm32_Init();

	Magneto_Enable();//三轴磁强计使能，即拉低SSNOT

    /* Infinite loop */
    while (1)
    {
		Magneto_Data_Updata(X_Dir);
		Magneto_Data_Updata(Y_Dir);
		Magneto_Data_Updata(Z_Dir);
		USART1_SendInt(0x0000);
		USART1_SendInt(Magneto_xyz_Data[0]);
		USART1_SendInt(Magneto_xyz_Data[1]);
		USART1_SendInt(Magneto_xyz_Data[2]);
		USART1_SendInt(0x7f7f);
		Delay_ms(100);

		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		Delay_ms(200);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
    }
}
/**************************************************************************
			  函数名：时钟初始化
				功能：
				说明：
***************************************************************************/
void RCC_Configuration(void)
{
	RCC_DeInit();//将RCC外设寄存器设置为缺省值
	RCC_HSEConfig(RCC_HSE_ON);//设置外部晶振
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //使能预取缓冲区
    	FLASH_SetLatency(FLASH_Latency_2); 	//Flash2个等待周期   
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHB时钟 = 系统时钟
		RCC_PCLK1Config(RCC_HCLK_Div2);//APB1时钟 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div1);//APB2时钟 = HCLK
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//设置PLL时钟源为外部的8M，倍频9倍即为72M	
		RCC_PLLCmd(ENABLE);//使能PLL时钟

		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);//等待PLL准备好
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);// 选择PLL做为系统时钟源    
		while(RCC_GetSYSCLKSource() != 0x08);//
	}

	/************************使能外设时钟********************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);//使能UART1,SPI1(36M),GPIOA时钟	，复用IO时钟
}

/**************************************************************************
			  函数名：嵌套中断设置
				功能：
				说明：
***************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif		

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置优先级分组形式1，即抢占级占一位，优先级占3位
	
	/****************UART1全局中断使能***********************/
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);				 

	/****************SPI1全局中断使能***********************/
	/*NVIC_InitStructure.NVIC_IRQChannel=SPI1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);				 */

}

/**************************************************************************
			  函数名：GPIO初始化
				功能：
				说明：
***************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*********************************配置PA口********************************
	PA0:LED D2 	  （推挽式输出）
	PA1:LED D3    （推挽式输出）
	PA4:SPI1_NSS  （复用功能推挽式输出)
	PA5:SPI1_SCK   (复用功能推挽式输出)
	PA6:SPI1_MISO  (复用功能推挽式输出)
	PA7:SPI1_MOSI  (复用功能推挽式输出)
	PA9: USART1发送端口（复用功能推挽式输出）
	PA10: USART1接收端口（浮空输入模式）
	PA11:SPI1_Res  （推挽式输出）
	PA12:SPI1_DRDY （浮空输入模式）
	*************************************************************************/	

	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //推挽式输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
