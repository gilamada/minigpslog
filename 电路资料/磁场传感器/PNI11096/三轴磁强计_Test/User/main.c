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

#define Set_MagnetoReset GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET); //��SPI1_Res����
#define Clr_MagnetoReset GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET); //��SPI1_Res����

/* ȫ�ֱ��� -------------------------------------------------------------*/
volatile u32 TimingDelay;
u16 Magneto_xyz_Data[3]; 

/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
USART_InitTypeDef USART_InitStructure;
SPI_InitTypeDef SPI_InitStructure;

/* Private function prototypes �������� -----------------------------------------------*/
void stm32_Init(void);
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void SysTick_Configuration(void);
void USART_Configuration(void);
void SPI_Configuration(void);

/*----------------------------- �����ʼ�� --------------------------------------------*/
void Magneto_Enable(void); //�����ǿ��ʹ�ܣ�������SSNOT
void Magneto_Disable(void); //�����ǿ��ʧ�ܣ�������SSNOT
void Magneto_Data_Updata(u8 dir); //dir��ȡΪΪX_Dir,Y_Dir,Z_Dir

/* Private functions ---------------------------------------------------------*/
void Led_On(void);
void Led_Off(void);
void SysTick_Interrupt(void);
void Delay_ms(u32 ms);
void USART1_Interrupt(void);
void USART1_SendInt(u16 data);
//void SPI1_Interrupt(void);

/**************************************************************************
			  �����������������ǿ�Ƶ���������
				���ܣ�
				˵����
***************************************************************************/
//void Magneto_Data_Updata(u8 dir)
//{
//	u32 i;
	//u16 tmp1,tmp2;

	//��RESһ�����壬ʹ�临λ����ÿ�β����ݶ�Ҫ��λ
		//GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET); //��SPI1_Res����	
//	for(i=0;i<500;i++);
		//Delay_ms(1);
		//GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET); //��SPI1_Res����

	/*SPI1->CR1 &=~(0x0040); //ȡ��SPEλ��ʧ��SPI1
	SPI1->CR1 &=~(0x0800); //ȡ��DFFλ������Ϊ8λ����
	SPI1->CR1 |=0x0040; //��SPEλ��ʹ����SPI1*/

		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==SET);	 //�ȴ�DRDYΪ��

	/****************************************************************
		Ҫ���͵��������ݸ�ʽ:(BIT7-BIT0)DHST PS2 PS1 PS0 ODIR MOT AS1 ASO
		AS1 AS0���������᣺X 0 1 ,Y 1 0 ,Z 1 1  			
		MOT:0
		ODIR:0
		PS2 PS1 PS0 0 0 0    32��Ƶ
		DHSH 1
		����X�ᷢ��0xa1  ��Y�ᷢ��0xa2  ��Z�ᷢ��0xa3
	*****************************************************************/
		//SPI_I2S_SendData(SPI1,(0x20|dir));  //��dir��
//	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);

	/*SPI1->CR1 &=~(0x0040); //ȡ��SPEλ��ʧ��SPI1
	SPI1->CR1 |= (0x0800); //��DFFλ������Ϊ16λ����
	SPI1->CR1 |=0x0040; //��SPEλ��ʹ����SPI1*/
	//for(i=0;i<150;i++);
		//Delay_ms(1);
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==RESET);   //�ȴ�DRDYΪ��
	//Delay_ms(1);
	
//	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);
	//while((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==RESET)||(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET));	 //�ȴ�DRDYΪ��,���ܻ���ǿ�
	//SPI_I2S_SendData(SPI1, 0x00);  //��dir��	
	//Magneto_xyz_Data[dir-1] =SPI_I2S_ReceiveData(SPI1);//����dir�������
	//SPI_I2S_SendData(SPI1, 0x00);  //��dir��
	//SPI_I2S_SendData(SPI1, 0x00);  //��dir��
		//tmp1 = SPI_I2S_ReceiveData(SPI1);//����dir�������
	//SPI_I2S_SendData(SPI1, 0x00);  //��dir��
//	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);
		//tmp2 = SPI_I2S_ReceiveData(SPI1);//����dir�������
		//Magneto_xyz_Data[dir-1] = ((tmp1<<8) |tmp2);

	/*SPI_I2S_SendData(SPI1, 0x00);  //��dir��
	tmp1 = SPI1->DR;
	SPI_I2S_SendData(SPI1, 0x00);  //��dir��
	tmp2 = SPI1->DR;
	Magneto_xyz_Data[dir-1]=((tmp1<<8)|tmp2);*/

	/*tmp  = (SPI_I2S_ReceiveData(SPI1)<<8);
	tmp |= SPI_I2S_ReceiveData(SPI1);
	Magneto_xyz_Data[dir-1]=tmp;*/

	/*tmp1 = SPI_I2S_ReceiveData(SPI1); //�ȵø�8λ
	tmp2 = SPI_I2S_ReceiveData(SPI1); //��õ�8λ;
	Magneto_xyz_Data[(dir-1)]=((tmp1<<8) | tmp2);*/
//}

void Magneto_Data_Updata(u8 dir)
{
	u16 tmp1,tmp2;	

	/*************���ô�ǿ��******************/
	Set_MagnetoReset   	//��SPI1_Res����
	Delay_ms(1);
	Clr_MagnetoReset	//��SPI1_Res����

	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==SET);	 //�ȴ�DRDYΪ��

	/****************************************************************
		Ҫ���͵��������ݸ�ʽ:(BIT7-BIT0)DHST PS2 PS1 PS0 ODIR MOT AS1 ASO
		AS1 AS0���������᣺X 0 1 ,Y 1 0 ,Z 1 1  			
		MOT:0
		ODIR:0
		PS2 PS1 PS0 0 0 0    32��Ƶ
		DHSH 1
		����X�ᷢ��0xa1  ��Y�ᷢ��0xa2  ��Z�ᷢ��0xa3
	*****************************************************************/
//	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == RESET);
	SPI_I2S_SendData(SPI1,(0x20|dir));  //��dir��
	

	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)==RESET);   //�ȴ�DRDYΪ��
	//while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);	

	//SPI_I2S_SendData(SPI1, 0x00);  //��dir��
	//tmp1 = SPI1->DR;
	//SPI_I2S_SendData(SPI1, 0x00);  //��dir��
	//tmp2 = SPI1->DR;
	//Magneto_xyz_Data[dir-1]=((tmp1<<8)|tmp2);

	SPI_I2S_SendData(SPI1, 0x00);  //��dir��
	tmp1 = SPI_I2S_ReceiveData(SPI1); //�ȵø�8λ
	//SPI_I2S_SendData(SPI1, 0x00);  //��dir��
	tmp2 = SPI_I2S_ReceiveData(SPI1); //��õ�8λ;
	Magneto_xyz_Data[(dir-1)]=((tmp1<<8) | tmp2);
}

/**************************************************************************
			  ��������LED0,LED1����
				���ܣ�
				˵����
***************************************************************************/
void Led_On(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
}

/**************************************************************************
			  ��������LED0,LED1Ϩ��
				���ܣ�
				˵����
***************************************************************************/
void Led_Off(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
}

/**************************************************************************
			  ��������SPI1���жϴ�����
				���ܣ�
				˵����
***************************************************************************/
/*void SPI1_Interrupt(void)
{
	
} */

/**************************************************************************
			  �������������ǿ��ʹ��
				���ܣ�
				˵����
***************************************************************************/
void Magneto_Enable(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET); //��SPI1_NSS����
	Delay_ms(1);//��ʱҪ����100NS
}

/**************************************************************************
			  �������������ǿ��ʧ��
				���ܣ�
				˵����
***************************************************************************/
void Magneto_Disable(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET); //��SPI1_NSS����
	Delay_ms(1);//��ʱҪ����100NS
}

/**************************************************************************
			  ������������1���жϴ�����
				���ܣ�
				˵����
***************************************************************************/
void USART1_Interrupt(void)
{
	u8 RxData;
	u8 TxData;

	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET)
	{
		RxData = USART_ReceiveData(USART1);//����8λ������
		TxData=RxData;
		USART_SendData(USART1, (u8)TxData);//�����յ������ݷ��ͳ�ȥ
	}	
}

/**************************************************************************
			  ������������1����16λ������
				���ܣ�
				˵����
***************************************************************************/
void USART1_SendInt(u16 data)
{
	u8 tmp[2];
	tmp[0]=data;    //��8λ
	tmp[1]=data>>8; //��8λ
	while (!(USART1->SR & USART_FLAG_TXE)); //�жϷ����Ƿ�Ϊ��
  	USART1->DR = (tmp[1] & 0x1FF);	 //���͸��ֽ�
	while (!(USART1->SR & USART_FLAG_TXE)); //�жϷ����Ƿ�Ϊ��
    USART1->DR = (tmp[0] & 0x1FF);	 //���͵��ֽ�
}

/**************************************************************************
			  ��������MS����ʱ����
				���ܣ�
				˵����
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
			  ��������SysTick�жϺ���
				���ܣ�
				˵����
***************************************************************************/
void SysTick_Interrupt(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

/**************************************************************************
			  �����������ڳ�ʼ��
				���ܣ�
				˵����
***************************************************************************/
void USART_Configuration(void)
{
	USART_InitStructure.USART_BaudRate=38400;//������Ϊ38400		
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8λ����λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ʧ��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);//���ô���1
	//USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܴ���1�����ж�
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//ʹ�ܴ���2�����ж�

	USART_Cmd(USART1, ENABLE);//ʹ�����贮��1
	//USART_Cmd(USART2, ENABLE);//ʹ�����贮��2
}

/**************************************************************************
			  ��������SPI1��ʼ��
				���ܣ�
				˵����
***************************************************************************/
void SPI_Configuration(void)
{
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;//��SPI
	SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;//�շ�����Ϊ8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //ʱ�����ո�
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;   //���ݲ����ڵ�һ��ʱ����
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	 //�ڲ�NSS�ź���SSIλ����
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//ʱ��64��Ƶ������72/128��M��SPIʱ�����ֵΪ18M��PNI11096��ʱ�����ֵ��1M
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_SSOutputCmd(SPI1, ENABLE);

//	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);	 //�������ܷǿ��ж�
	SPI_Cmd(SPI1, ENABLE);
}

/**************************************************************************
			  ��������SysTick��ʼ��
				���ܣ�
				˵����
***************************************************************************/
void SysTick_Configuration(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //SysTickʱ��ԴΪAHBʱ�ӳ���8����Ϊ9M
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

	Magneto_Enable();//�����ǿ��ʹ�ܣ�������SSNOT

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
			  ��������ʱ�ӳ�ʼ��
				���ܣ�
				˵����
***************************************************************************/
void RCC_Configuration(void)
{
	RCC_DeInit();//��RCC����Ĵ�������Ϊȱʡֵ
	RCC_HSEConfig(RCC_HSE_ON);//�����ⲿ����
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //ʹ��Ԥȡ������
    	FLASH_SetLatency(FLASH_Latency_2); 	//Flash2���ȴ�����   
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHBʱ�� = ϵͳʱ��
		RCC_PCLK1Config(RCC_HCLK_Div2);//APB1ʱ�� = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div1);//APB2ʱ�� = HCLK
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//����PLLʱ��ԴΪ�ⲿ��8M����Ƶ9����Ϊ72M	
		RCC_PLLCmd(ENABLE);//ʹ��PLLʱ��

		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);//�ȴ�PLL׼����
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);// ѡ��PLL��Ϊϵͳʱ��Դ    
		while(RCC_GetSYSCLKSource() != 0x08);//
	}

	/************************ʹ������ʱ��********************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);//ʹ��UART1,SPI1(36M),GPIOAʱ��	������IOʱ��
}

/**************************************************************************
			  ��������Ƕ���ж�����
				���ܣ�
				˵����
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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�������ȼ�������ʽ1������ռ��ռһλ�����ȼ�ռ3λ
	
	/****************UART1ȫ���ж�ʹ��***********************/
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);				 

	/****************SPI1ȫ���ж�ʹ��***********************/
	/*NVIC_InitStructure.NVIC_IRQChannel=SPI1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);				 */

}

/**************************************************************************
			  ��������GPIO��ʼ��
				���ܣ�
				˵����
***************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*********************************����PA��********************************
	PA0:LED D2 	  ������ʽ�����
	PA1:LED D3    ������ʽ�����
	PA4:SPI1_NSS  �����ù�������ʽ���)
	PA5:SPI1_SCK   (���ù�������ʽ���)
	PA6:SPI1_MISO  (���ù�������ʽ���)
	PA7:SPI1_MOSI  (���ù�������ʽ���)
	PA9: USART1���Ͷ˿ڣ����ù�������ʽ�����
	PA10: USART1���ն˿ڣ���������ģʽ��
	PA11:SPI1_Res  ������ʽ�����
	PA12:SPI1_DRDY ����������ģʽ��
	*************************************************************************/	

	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //����ʽ���
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
