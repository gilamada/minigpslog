/**********************************************************************************************
***模块名称：                                                                               ***
***模块功能：                                                                               ***                                                                                                       
***作    者：                                                                               ***
***调用方式：                                                                               ***
***日    志： 日期        版本    修改人      事件                                          ***
***           2007/9/13   V1.00   METAL MAX   搭建程序框架                                  ***
***           2007/9/14   V1.01   METAL MAX   具体底层程序添加                              ***
***           2007/9/15   V1.02   METAL MAX   具体底层程序添加                              ***
***           2007/9/16   V1.03   METAL MAX   具体底层程序添加                              ***
***备    注：                                                                               ***
**********************************************************************************************/
#ifndef _PCF8583_H_
#define _PCF8583_H_ 

void ReadPCF8583(unsigned char addr, unsigned char num, unsigned char *buff);
void WritePCF8583(unsigned char addr, unsigned char num, unsigned char *buff);

#endif