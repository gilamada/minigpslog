//=====================================================================
// �ļ�����: Leds.C
// оƬ����: STM32F103VCT6                                           
// �������: Keil, Windows XP2 
// ��������: 2010��07��22��                                                               
// ������: STM32��led��������
// ��д��Ա: �޳�                                                                       
// ��ϵ��ʽ: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com  
//==============�汾��Ϣ===============================================
// ��    ��: V1.0	 
// ��    ��: 2010��07��22��	
// ��    ��: ԭʼ�汾 
//======================================================================
//============================= ͷ�ļ� =================================
#ifndef __LEDS_H_
#define __LEDS_H_



//============================= �궨�� =================================
#define GPIO_LED_PORT           GPIOA
#define RCC_APB2Periph_LED      RCC_APB2Periph_GPIOA	
#define LED_PIN                 GPIO_Pin_0+GPIO_Pin_1 
#define LED1                    GPIO_Pin_0
#define LED2                    GPIO_Pin_1

#define LED_Reverse(x)   GPIO_WriteBit(GPIO_LED_PORT, x, (BitAction)(1  \
                            - GPIO_ReadOutputDataBit(GPIO_LED_PORT, x)))
#define LED_ON(x)        GPIO_LED_PORT->BRR  = x
#define LED_OFF(x) 	     GPIO_LED_PORT->BSRR = x


#define KEY1       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)
#define KEY2       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1)
#define KEY3       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)
#define KEY4       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
//============================= �������� ===============================

//============================= �������� ===============================
extern  void LEDS_Configuration(void);
extern   void KEY_Configuration(void);
#endif
//======================================================================
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//======================================================================
