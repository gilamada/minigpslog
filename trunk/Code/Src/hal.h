#ifndef HAL_H
#define HAL_H

//硬件定义
//TFT硬件定义
#define TFT_TYPE	2		//1是一号,2是二号屏

//触摸屏定义
#define TOUCH_TYPE	2		//1是旧版,2是新版



//输出宏定义
#define LED1_ON		GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define LED1_OFF	GPIO_SetBits(GPIOA, GPIO_Pin_8)

#define LED2_ON		GPIO_ResetBits(GPIOD, GPIO_Pin_7)
#define LED2_OFF	GPIO_SetBits(GPIOD, GPIO_Pin_7)

#define LED3_ON		GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define LED3_OFF	GPIO_SetBits(GPIOC, GPIO_Pin_7)

#define LED4_ON		GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define LED4_OFF	GPIO_SetBits(GPIOC, GPIO_Pin_5)

#define LED5_ON		GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define LED5_OFF	GPIO_SetBits(GPIOB, GPIO_Pin_9)

#define LED6_ON		GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define LED6_OFF	GPIO_SetBits(GPIOB, GPIO_Pin_8)

#define LED7_ON		GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define LED7_OFF	GPIO_SetBits(GPIOB, GPIO_Pin_5)

#define LED8_ON		GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define LED8_OFF	GPIO_SetBits(GPIOB, GPIO_Pin_0)


//背光开关
#define BLIGHT_OFF		GPIOC->BSRR=GPIO_Pin_5
#define BLIGHT_ON      GPIOC->BRR=GPIO_Pin_5


//输入宏定义
#define GET_LEFT()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3))
#define GET_UP()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4))
#define GET_RIGHT()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5))
#define GET_DOWN()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6))


//硬件初始化
extern void  ChipHalInit(void);
extern void  ChipOutHalInit(void);

//系统时钟延迟
extern void SysTickDelay(u16);

//串口
extern void USART1_Putc(u8 c);
extern void USART1_Puts(char * str);
extern u8 WaitUsart1(void);
extern void USART2_Putc(u8 c);
extern void USART2_Puts(char * str);

extern u8 Uart2_Get_Flag;
extern u8 Uart2_Get_Data;

//SD
extern void TurnToSD(void);						//SD
extern void SDConfig(void);
extern void WriteFlash(void);

//25F
extern void SST25ChipErase(void);							//全刷除
extern void SST25SectorErase(u32 addr);						//扇区刷除
extern void SST25Write(u32 addr,u8* p_data,u32 no);			//写
extern void SST25Read(u32 addr,u8* p_data,u32 no);			//读
extern void TurnToSST25(void);								//转到SD卡

#endif

