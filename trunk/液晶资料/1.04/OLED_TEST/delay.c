#include "delay.h"

void delay_1ms(void)//1ms��ʱ����
{
    unsigned int i;
  
    asm("nop");
    for(i=0;i<1139;i++)
    {  
    } 
}

void delay_nms(unsigned int n)//��ʱn����
{
    unsigned int i;
    
    for(i=0;i<n;i++)//ִ��n��1������ʱ
    {
        delay_1ms(); 
    }
}

void delay_nus(unsigned int n)//��ʱN΢��,N>=5  ����:8MHz
{
    unsigned int i;
    
    for(i=5;i<n;i++)
    {
        asm("nop");
        asm("nop");	 
    } 
}

void delay_1us(void)
{
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
}
