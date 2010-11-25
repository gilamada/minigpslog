/**********************************************************************************************
***�ļ����ƣ� PINDEFIEN.H                                                                   ***
***��    �ܣ� ���Ŷ���                                                                      ***																				                       
***��    �ߣ� METAL MAX  ��liyangbbs@126.com��                                              ***
***��    ע��                                                                               ***
**********************************************************************************************/
#ifndef _LCMPort_h_
#define _LCMPort_h_

sbit WR_LCM	   =  P2^0;                                                 /* LCM�˿ڶ���                                      */
sbit RD_LCM	   =  P2^1;
sbit CE_LCM	   =  P2^2;
sbit CD_LCM	   =  P2^3;
sbit RST_LCM   =  P2^7;
sbit FS_LCM	   =  P2^6;
#define DPORT_LCM  P0

sbit SDA       =  P1^1;                                                 /* IIC�˿ڶ���                                      */
sbit SCL       =  P1^0;

//sbit CLK_SPI   =  P3^6;                                                 /* SPI�˿ڶ���                                      */
//sbit MOSI      =  P3^4;
//sbit MISO      =  P3^5;
//sbit SSNOT     =  P3^7;

#endif