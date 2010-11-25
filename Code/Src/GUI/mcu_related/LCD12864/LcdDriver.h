//==============================================================================
// 文件名称: LcdDriver.H 
// 芯片类型: MSP430F149                                            
// 开发软件: IAR 3.42, Windows XP2 
// 开发时间: 2009年10月20日                                                               
// 程序功能: Ks0108 液晶控制器驱动程序 
// 编写人员: 崔超                                                                       
// 联系方式: 13686036830  cuichao@xrd-lcd.com   
//==============================================================================
#ifndef _LcdDriver_H
#define _LcdDriver_H

#define LCD_XSIZE  128
#define LCD_YSIZE  8

#define Ks0108_CMD_Disp_ON          0x3F
#define Ks0108_CMD_Disp_OFF         0x3E
#define Ks0108_CMD_Set_Addr         0x40
#define Ks0108_CMD_Set_Page         0xB8
#define Ks0108_CMD_Disp_Start_Line  0xC0

//显示模式
#define GUI_TEXTMODE_NORMAL      (0)
#define GUI_TEXTMODE_REVERSE     (1<<0)
#define GUI_TEXTMODE_UNDERLINE   (1<<1)
#define GUI_TEXTMODE_XOR         (1<<2)
//对齐方式
#define GUI_TA_LEFT              (0<<4)
#define GUI_TA_HCENTER           (1<<4)
#define GUI_TA_RIGHT             (2<<4)
#define GUI_JUST_TEXT            (1<<7)

//英文的两种字体
#define En_8x16      (0)
#define En_5x8       (1)

//字体宽度定义
#define En_8x16_XSIZE  8
#define En_8x16_YSIZE  2
#define En_5x8_XSIZE   6
#define En_5x8_YSIZE   1
//英文字体宽度定义

extern INT8U Char_XSIZE;
extern INT8U Char_YSIZE;

//中文字体宽度定义
#define Chinese_XSIZE  16
#define Chinese_YSIZE  2

//背光定义宏定义
#define GUI_BacklightOn()   	P12_OUT = 0;
#define GUI_BacklightOff()   	P12_OUT = 1; 

extern void GUI_GotoX(INT8U x);
extern void GUI_GotoY(INT8U y);
extern void GUI_GotoXY(INT8U x,INT8U y);
extern INT8U GUI_SetTextMode(INT8U TextMode);
extern void GUI_SetEnFont(INT8U EnFont);
extern void GUI_DispCharAt(INT8U c, INT8U x, INT8U y);
extern void GUI_DispChar(INT8U c);
extern void GUI_Init(void);
extern void GUI_Clear(void);
extern void GUI_DispCEOL(void);

extern void GUI_DispStringAt(INT8U  *s,INT8U X,INT8U Y);
extern void GUI_DispString(INT8U  *s);
extern void GUI_DispStringLen(INT8U *s, INT16U Len);
extern void GUI_DrawCursor(INT8U x, INT8U y);
extern void GUI_CursorOn(INT8U x, INT8U y);
extern void GUI_CursorOFF(INT8U x, INT8U y);
extern void GUI_DispDecAt(INT16U v, INT16U x, INT16U y, INT8U Len);
extern void GUI_DispHexAt(INT32U v, INT8U x, INT8U y, INT8U Len);
extern void GUI_DispStringAtBar(INT8U *s,INT8U x0, INT8U y0, INT8U x1,INT8U Mode);
extern void GUI_Delay(INT16U num);
extern void GUI_DrawIcon(INT8U *Ico,INT8U X,INT8U Y);
extern void GUI_DrawIcon16X16(INT8U  *Ico, INT8U x0, INT8U y0);
//extern void HBar(INT8U y, INT8U x0, INT8U x1,INT8U percent);
//extern void Bar(INT8U Item_,INT8U ItemNum_,INT8U BarPosX,INT8U BarNumPosX);
//extern void TipDisp( INT8U x0, INT8U y0, INT8U x1, INT8U y1);
//extern void TipClr( INT8U x0, INT8U y0, INT8U x1, INT8U y1);

#endif
//==============================================================================
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//==============================================================================

