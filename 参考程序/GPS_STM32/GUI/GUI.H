//================================================================================================
// 文件名称: GUI.h 
// 芯片类型: STM32F103VBT6                                           
// 开发软件: Keil, Windows XP2 
// 开发时间: 2009年09月08日                                                               
// 程序功能: 液晶显示GUI 
// 编写人员: 崔超                                                                       
// 联系方式: 13686036830  cuichao@xrd-lcd.com 深圳市欣瑞达电子有限公司  
//================================================================================================ 
#ifndef _GUI_H
#define _GUI_H


//定义常量存储类型
#define code const   
#define _CONST_ const

#define GUI_LCM_XMAX  160
#define GUI_LCM_YMAX  160



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
#define En_8x8       (1)

//英文字体宽度定义
#define En_8x16_XSIZE  8
#define En_8x16_YSIZE  16
#define En_8x8_XSIZE   8
#define En_8x8_YSIZE   8

//中文字体宽度定义
#define Chinese_XSIZE  16
#define Chinese_YSIZE  16

//字体宽度定义
extern INT8U Char_XSIZE;
extern INT8U Char_YSIZE; 
extern	void GUI_PutChinese(INT16U UniCode, INT8U x, INT8U y);
extern INT8U GUI_SetTextMode(INT8U TextMode);
extern void GUI_SetEnFont(INT8U EnFont);
extern void GUI_DispChar(INT8S c, INT8U x, INT8U y); 
extern void GUI_Initialize(void);

extern void GUI_DispString(char *s,INT8U X,INT8U Y);
extern void GUI_DispDec(INT16U v, INT16U x, INT16U y, INT8U Len);
extern void GUI_DispHex(INT32U v, INT8U x, INT8U y, INT8U Len); 
extern void Display_Number(INT16U Number, INT8U X, INT8U Y, INT8U Lenth);
extern void Display_SignedNumber(int Number,INT8U X,INT16U Y,INT8U Lenth);


extern void  GUI_Line( INT16S x, INT16S y, INT16S x1, INT16S y1, TCOLOR color) ;
extern void  GUI_Circle(u32 x0, u32 y0, u32 r, TCOLOR color);
#endif
