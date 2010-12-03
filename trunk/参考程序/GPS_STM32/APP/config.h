//=====================================================================
// 文件名称: config.h
// 芯片类型: STM32F103VCT6                                           
// 开发软件: Keil, Windows XP2 
// 创建日期: 2010年06月18日                                                               
// 程序功能: STM32的硬件初始化程序
// 编写人员: 崔超                                                                       
// 联系方式: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com  
//==============版本信息===============================================
// 版    本: V1.0	 
// 日    期: 2010年06月18日	
// 描    述: 原始版本 
//======================================================================
#ifndef __CONFIG_H
#define __CONFIG_H			  
 				 
/* 默认变量定义---------------------------------------------------------------*/
#ifndef NULL
#define NULL    ((void *)0)
#endif

		



typedef unsigned char  INT8U; // 无符号8位整型变量 // 
typedef signed char    INT8S; // 有符号8位整型变量 // 
typedef unsigned short INT16U; // 无符号16位整型变量 // 
typedef signed short   INT16S; // 有符号16位整型变量 // 
typedef unsigned int   INT32U; // 无符号32位整型变量 // 
typedef signed int     INT32S; // 有符号32位整型变量 // 
typedef float          FP32; // 单精度浮点数(32位长度) // 
typedef double         FP64; // 双精度浮点数(64位长度) //  
#define   BOOL     bool



/* 系统文件包含---------------------------------------------------------------*/
#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "BSP.h" 
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>  
#include "printp.h" 
#include "rtc_time.h" 
#include "leds.h" 
#include "LCDDRIVER.h"
#include "GUI.h" 
#include "GPS.h"
#include "ff.h"			/* FatFs configurations and declarations */
#include "sdcard.h"
#include "diskio.h"
extern FATFS  fs;            // Work area (file system object) for logical drive
extern FIL    fil;      // file objects
#endif /* __CONFIG_H */	
/*******************************************************************************
**  End Of File                                                     
*******************************************************************************/
