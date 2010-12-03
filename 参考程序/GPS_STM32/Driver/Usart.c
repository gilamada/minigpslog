// ================================================================================================ 
// �ļ����ƣ�Usart.c  
// оƬ����: STM32F103VBT6                                           
// �������: Keil, Windows XP2 
// ����ʱ�䣺2010��01��14��                                                               
// �����ܣ�STM32���ڳ���
// ��д��Ա���޳�                                                                       
// ��ϵ��ʽ��13686036830  cuichao@xrd-lcd.com   
// ================================================================================================

#include "config.h"

/*******************************************************************************
* Function Name  : �ض���ϵͳputchar����int fputc(int ch, FILE *f)
* Description    : ���ڷ�һ���ֽ�
* Input          : int ch, FILE *f
* Output         : 
* Return         : int ch
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    //USART_SendData(USART1, (u8) ch);
    USART3->DR = (u8) ch;
    
    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
    {
    }

    return ch;
}
//============================================================================= 
//�� �� ��: USART1_IRQHandler()
//��	�ܣ��жϷ������
//��ڲ����� ��
//���ڲ����� ��
//�� �� ֵ�� ��
//============================================================================= 

void USART1_IRQHandler(void)
{  

    u8 buffer;	 
	buffer =(u8) (USART_ReceiveData(USART1)); //��ȡ����	

   //GPS_usart(buffer);
   USART3->DR = buffer;  //ת��usart3

   USART_ClearITPendingBit(USART1, USART_IT_RXNE);//����жϱ�־
	
}






	
	


