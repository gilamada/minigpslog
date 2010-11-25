#include "delay.h"

void delay_1ms(void)//1ms延时函数
{
    unsigned int i;
  
    asm("nop");
    for(i=0;i<1139;i++)
    {  
    } 
}

void delay_nms(unsigned int n)//延时n毫秒
{
    unsigned int i;
    
    for(i=0;i<n;i++)//执行n次1毫秒延时
    {
        delay_1ms(); 
    }
}

void delay_nus(unsigned int n)//延时N微秒,N>=5  晶振:8MHz
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
