#ifndef _DELAY_H_
#define _DELAY_H_
//8MHzʱ����

#include <iccioavr.h>
#include <macros.h>

void delay_1us(void);
void delay_1ms(void);           //1ms��ʱ����
void delay_nms(unsigned int n); //��ʱn����
void delay_nus(unsigned int n); //��ʱN΢��,N>=5  ����:8MHz

#endif