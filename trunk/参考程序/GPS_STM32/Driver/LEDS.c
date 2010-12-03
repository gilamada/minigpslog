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
#include "config.h"
//============================= �궨�� =================================


//============================= �������� ===============================

//======================================================================
//�� �� ��: LEDS_Configuration() 
//��    ��: ����LED�ܽ�
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//======================================================================
 void LEDS_Configuration(void)
{ 			
    GPIO_InitTypeDef GPIO_InitStructure;  
											   

	/* ���� PA.00 -PA.0 Ϊ���ģʽ | LED0~1 | ������� */

    GPIO_InitStructure.GPIO_Pin =  LED_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_LED_PORT, &GPIO_InitStructure); 
   
    GPIO_LED_PORT->BSRR = LED_PIN;		// �ø�IO(����)	

    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);


}

//======================================================================
//�� �� ��: KEY_Configuration() 
//��    ��: ����LED�ܽ�
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//======================================================================
 void KEY_Configuration(void)
{ 			
   GPIO_InitTypeDef GPIO_InitStructure;  
											   

	/* ���� PC.0 GPS_BOOT -PC.1 GPS_EN Ϊ���ģʽ*/
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 + GPIO_Pin_1+GPIO_Pin_2 + GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 


}
//======================================================================
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//======================================================================


//======================================================================
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//======================================================================
