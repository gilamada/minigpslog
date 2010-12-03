//=====================================================================
// 文件名称: Leds.C
// 芯片类型: STM32F103VCT6                                           
// 开发软件: Keil, Windows XP2 
// 创建日期: 2010年07月22日                                                               
// 程序功能: STM32的led驱动程序
// 编写人员: 崔超                                                                       
// 联系方式: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com  
//==============版本信息===============================================
// 版    本: V1.0	 
// 日    期: 2010年07月22日	
// 描    述: 原始版本 
//======================================================================
//============================= 头文件 =================================
#include "config.h"
//============================= 宏定义 =================================


//============================= 变量定义 ===============================

//======================================================================
//函 数 名: LEDS_Configuration() 
//功    能: 配置LED管脚
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//======================================================================
 void LEDS_Configuration(void)
{ 			
    GPIO_InitTypeDef GPIO_InitStructure;  
											   

	/* 配置 PA.00 -PA.0 为输出模式 | LED0~1 | 推挽输出 */

    GPIO_InitStructure.GPIO_Pin =  LED_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_LED_PORT, &GPIO_InitStructure); 
   
    GPIO_LED_PORT->BSRR = LED_PIN;		// 置高IO(灯灭)	

    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);


}

//======================================================================
//函 数 名: KEY_Configuration() 
//功    能: 配置LED管脚
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//======================================================================
 void KEY_Configuration(void)
{ 			
   GPIO_InitTypeDef GPIO_InitStructure;  
											   

	/* 配置 PC.0 GPS_BOOT -PC.1 GPS_EN 为输出模式*/
    
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
