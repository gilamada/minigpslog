// ================================================================================================ 
// 文件名称：Usart.c  
// 芯片类型: STM32F103VBT6                                           
// 开发软件: Keil, Windows XP2 
// 开发时间：2010年01月14日                                                               
// 程序功能：STM32串口程序
// 编写人员：崔超                                                                       
// 联系方式：13686036830  cuichao@xrd-lcd.com   
// ================================================================================================

#include "config.h"

/*******************************************************************************
* Function Name  : 重定义系统putchar函数int fputc(int ch, FILE *f)
* Description    : 串口发一个字节
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
//函 数 名: USART1_IRQHandler()
//功	能：中断服务程序
//入口参数： 无
//出口参数： 无
//返 回 值： 无
//============================================================================= 

void USART1_IRQHandler(void)
{  

    u8 buffer;	 
	buffer =(u8) (USART_ReceiveData(USART1)); //获取数据	

   //GPS_usart(buffer);
   USART3->DR = buffer;  //转给usart3

   USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清楚中断标志
	
}






	
	


