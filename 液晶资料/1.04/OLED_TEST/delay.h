#ifndef _DELAY_H_
#define _DELAY_H_
//8MHz时钟下

#include <iccioavr.h>
#include <macros.h>

void delay_1us(void);
void delay_1ms(void);           //1ms延时函数
void delay_nms(unsigned int n); //延时n毫秒
void delay_nus(unsigned int n); //延时N微秒,N>=5  晶振:8MHz

#endif