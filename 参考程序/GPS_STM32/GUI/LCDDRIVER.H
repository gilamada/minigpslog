//================================================================================================
// 文件名称: LCDDRIVE.H
// 芯片类型: STM32F103VBT6                                           
// 开发软件: Keil, Windows XP2 
// 开发时间: 2010年4月17日                                                               
// 程序功能: LCM点阵驱动程序,LCM为深圳市欣瑞达电子有限公司的XRD160160D
// 编写人员: 崔超                                                                       
// 联系方式: QQ:52020081 MSN:cui_chao@msn.com Email:cuichao@xrd-lcd.com 
// 说明：图形液晶采用UC1698为LCD控制芯片，内带负压产生器，单3.3伏供电，
// 并行接口(使用stm32f103rbt6驱动)。
// 硬件连接： D0--D7   <===>  GPIOA0--GPIOA7
//            /WR      <===>  GPIOB1
//            /RD      <===>  GPIOB6
//            /CS      <===>  GPIOB7
//            C/D      <===>  GPIOB8
//            /RST     <===>  GPIOB0 
//================================================================================================ ****************************************************************************************/
#ifndef  LCDDRIVE_H
#define  LCDDRIVE_H

//========================================== 宏定义 ==============================================

/* 定义颜色数据类型(可以是数据结构) */
#define  TCOLOR				INT8U  
#define  GUI_LCM_XMAX		160	/* 定义液晶x轴的点数 */
#define  GUI_LCM_YMAX		160	/* 定义液晶y轴的点数 */

#ifndef BACKLIGTH_PWM
    #define  GUI_BacklightON()  GPIO_SetBits(GPIOD, GPIO_Pin_13);			//LIGHT ON
    #define  GUI_BacklightOFF()  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			//LIGHT Off
#else 
    #define  GUI_BacklightON()    PWM_Configuration(12);
    #define  GUI_BacklightOFF()   PWM_Configuration(0);
#endif
//======================================== 变量声明 ===============================================
extern TCOLOR  gui_disp_buf[GUI_LCM_YMAX][GUI_LCM_XMAX/8];	// 声明GUI显示缓冲区
extern INT8U const  DCB_HEX_TAB[8];

//======================================== 函数声明 ===============================================
extern void  LCD_Initialize(void);
extern void GUI_RefreshSCR(void);
extern void GUI_FillSCR(TCOLOR dat);
extern INT8U GUI_RefreshBlock(INT8U x1,INT8U y1,INT8U x2,INT8U y2);
extern void  GUI_ClearSCR(void);
extern INT8U  GUI_Point(INT8U x, INT8U y, TCOLOR color);
extern INT8U  GUI_BufferPoint(INT8U x, INT8U y, TCOLOR color) ;
extern INT8U  GUI_ReadPoint(INT8U x, INT8U y, TCOLOR *ret);
#endif
//================================================================================================= 
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//================================================================================================= 

