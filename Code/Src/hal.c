/***************************************************
**HAL.c
**��Ҫ����оƬӲ�����ڲ���Χ���ⲿ��Χ�ĳ�ʼ��������INIT����
**��MAIN�е��ã�ʹMAIN�����о�����Ӳ�����޹�
***************************************************/

#include "stm32f10x.h"



//�����ڲ�Ӳ��ģ������ú���
extern void RCC_Configuration(void);			//RCC
extern void USART_Configuration(void);			//����

/*******************************
**������:ChipHalInit()
**����:Ƭ��Ӳ����ʼ��
*******************************/
void  ChipHalInit(void)
{
	//��ʼ��ʱ��Դ
	//RCC_Configuration();
	SystemInit();
	
	
	//��ʼ������
	//USART_Configuration();
}


/*********************************
**������:ChipOutHalInit()
**����:Ƭ��Ӳ����ʼ��
*********************************/
void  ChipOutHalInit(void)
{
	//��ʼ��SD
	//SDConfig();

	//FSMC_SRAM_Init();
}
