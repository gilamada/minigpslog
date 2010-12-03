//================================================================================================
// �ļ�����: BSP.H
// оƬ����: STM32F103VBT6                                           
// �������: Keil, Windows XP2 
// ����ʱ��: 2010��06��18��                                                               
// ������: STM32��Ӳ����ʼ������
// ��д��Ա: �޳�                                                                       
// ��ϵ��ʽ: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com   
//================================================================================================ 

#ifndef __BSP_H
#define __BSP_H

#define BACKLIGTH_PWM

extern u32 volatile Tick_s,Tick_ms;		   
/***************************************************************************************
** ��������: TargetResetInit
** ��������: Ŀ��������ʼ��
** ��    ��: None
** �� �� ֵ: None       
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
