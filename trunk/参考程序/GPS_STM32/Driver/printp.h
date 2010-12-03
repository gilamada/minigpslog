#ifndef _PRINTP_H_
#define _PRINTP_H_

#include "Config.h" 

void putchar1(INT8U ch);
INT8U getchar1(void);

//格式转换函数，十六进制转换为字符串  eg:输入0x5A，输出5A    
void PUT_INT8U_HexTOString(INT8U Hex);
//格式转换函数，十进制数据转换为字符串 eg：输入001234，串口输出1234(去掉无效0)
void PUT_INT32U_DecTOString(INT32U Dec);
//串口输入字符，转换成十进制，即"123456"--123456
INT32U StringTODec(void);

#define PRINTP_EN  			
#define CR_as_CRLF	TRUE         
// if true , you can use "\n" to act as CR/LF, 
// if false , you have to use "\n\r",but can get a higher speed

#ifdef PRINTP_EN
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

void printp( char* ctrl, ...);
INT8U USART_Scanf_Name(char * str);
INT8U USART_Scanf_Cmd(char * str);
INT8U Equal(void* A, void* B, INT8U SizeA, INT8U SizeB);

#endif


#endif  //_USART_PRINTP_H_



