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
#ifndef __LEDS_H_
#define __LEDS_H_



//============================= 宏定义 =================================
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
//============================= 变量定义 ===============================

//============================= 函数声明 ===============================
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
