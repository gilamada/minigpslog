#ifndef HAL_H
#define HAL_H

//Ӳ������
//TFTӲ������
#define TFT_TYPE	2		//1��һ��,2�Ƕ�����

//����������
#define TOUCH_TYPE	2		//1�Ǿɰ�,2���°�



//����궨��
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


//���⿪��
#define BLIGHT_OFF		GPIOC->BSRR=GPIO_Pin_5
#define BLIGHT_ON      GPIOC->BRR=GPIO_Pin_5


//����궨��
#define GET_LEFT()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3))
#define GET_UP()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4))
#define GET_RIGHT()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5))
#define GET_DOWN()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6))


//Ӳ����ʼ��
extern void  ChipHalInit(void);
extern void  ChipOutHalInit(void);

//ϵͳʱ���ӳ�
extern void SysTickDelay(u16);

//����
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
extern void SST25ChipErase(void);							//ȫˢ��
extern void SST25SectorErase(u32 addr);						//����ˢ��
extern void SST25Write(u32 addr,u8* p_data,u32 no);			//д
extern void SST25Read(u32 addr,u8* p_data,u32 no);			//��
extern void TurnToSST25(void);								//ת��SD��

#endif

