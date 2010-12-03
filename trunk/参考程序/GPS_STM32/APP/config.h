//=====================================================================
// �ļ�����: config.h
// оƬ����: STM32F103VCT6                                           
// �������: Keil, Windows XP2 
// ��������: 2010��06��18��                                                               
// ������: STM32��Ӳ����ʼ������
// ��д��Ա: �޳�                                                                       
// ��ϵ��ʽ: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com  
//==============�汾��Ϣ===============================================
// ��    ��: V1.0	 
// ��    ��: 2010��06��18��	
// ��    ��: ԭʼ�汾 
//======================================================================
#ifndef __CONFIG_H
#define __CONFIG_H			  
 				 
/* Ĭ�ϱ�������---------------------------------------------------------------*/
#ifndef NULL
#define NULL    ((void *)0)
#endif

		



typedef unsigned char  INT8U; // �޷���8λ���ͱ��� // 
typedef signed char    INT8S; // �з���8λ���ͱ��� // 
typedef unsigned short INT16U; // �޷���16λ���ͱ��� // 
typedef signed short   INT16S; // �з���16λ���ͱ��� // 
typedef unsigned int   INT32U; // �޷���32λ���ͱ��� // 
typedef signed int     INT32S; // �з���32λ���ͱ��� // 
typedef float          FP32; // �����ȸ�����(32λ����) // 
typedef double         FP64; // ˫���ȸ�����(64λ����) //  
#define   BOOL     bool



/* ϵͳ�ļ�����---------------------------------------------------------------*/
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
