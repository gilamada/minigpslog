//================================================================================================
// 文件名称: BSP.H
// 芯片类型: STM32F103VBT6                                           
// 开发软件: Keil, Windows XP2 
// 开发时间: 2010年06月18日                                                               
// 程序功能: STM32的硬件初始化程序
// 编写人员: 崔超                                                                       
// 联系方式: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com   
//================================================================================================ 

#ifndef __BSP_H
#define __BSP_H

#define BACKLIGTH_PWM

extern u32 volatile Tick_s,Tick_ms;		   
/***************************************************************************************
** 函数名称: TargetResetInit
** 功能描述: 目标板基本初始化
** 参    数: None
** 返 回 值: None       
****************************************************************************************/
extern void TargetResetInit (void);	

extern void PWM_Configuration(u16 DutyCycle) ;

extern void USART1_Configuration(u32 BaudRate);
extern void USART3_Configuration(u32 BaudRate);
extern void TimingDelay_Decrement(void); 

extern void Delay_ms(u32 ms) ; 
extern void Delay_s(u16 s);
#endif /* _TARGET_H */

/***************************************************************************************
**  End Of File                                                     
****************************************************************************************/
