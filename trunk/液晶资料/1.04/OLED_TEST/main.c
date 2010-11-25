#include <iccioavr.h>
#include <macros.h>
#include "ssd1332.h"
#include "delay.h"
#include "img.h"

//M16@8M
#define uchar unsigned char


//指针时钟坐标

const unsigned char sec_x[]={
            32,35,38,41,43,
            45,48,50,52,54,
            56,58,59,60,60,
            60,60,60,59,58,
            56,54,52,50,48,
            45,43,41,38,35,
            32,29,26,23,21,
            19,16,14,12,10,
             8, 6, 5, 4, 4,
             4, 4, 4, 5, 6,
             8,10,12,14,16,
            19,21,23,26,29,
           };

const unsigned char sec_y[]={
             4, 4, 4, 5, 6,
             8,10,12,14,16,
            19,21,23,26,29,
            32,35,38,41,43,
            45,48,50,52,54,
            56,58,59,60,60,
            60,60,60,59,58,
            56,54,52,50,48,
            45,43,41,38,35,
            32,29,26,23,21,
            19,16,14,12,10,
             8, 6, 5, 4, 4,
           };
		  
const unsigned char min_x[]={
            32,34,36,38,40,
            41,42,44,46,48,
            49,50,51,52,53,
            54,53,52,51,50,
            49,46,45,44,42,
            40,39,38,37,35,
            32,29,27,26,25,
            24,22,20,19,18,
            15,14,13,12,11,
            10,11,12,13,14,
            15,16,18,20,22,
            23,24,26,28,30,
          };

const unsigned char min_y[]={
            10,10,10,11,13,
            14,15,17,19,21,
            23,24,26,28,30,
            32,34,36,38,40,
            41,44,45,46,48,
            49,50,51,52,53,
            54,53,52,51,50,
            49,48,46,45,44,
            41,40,38,36,34,
            32,30,28,26,24,
            23,21,19,17,15,
            14,13,11,10,10,
          };


const unsigned char hour_x[]={ 
             32,38,44,47,44,38,
             32.26,20,15,20,26,
           };


const unsigned char hour_y[]={ 
            17,19,26,32,38,45,
            47,45,38,32,26,19,
           };




unsigned char RGB[]={0x00,0x1f,0x07,0xe0,0xf8,0x00,0x00,0x00,0xff,0xff};
unsigned char ColorBar[]={0xff,0xff,0xf8,0x00,0xff,0xe0,0x07,0xe0,0x07,0xff,0x00,0x1f,0x00,0x00};


__flash unsigned char table1[][32]=
{

{0x10,0x20,0x8C,0x3F,0x61,0xC0,0x06,0x00,0x00,0x01,0x7F,0xE2,0x40,0x0C,0x4F,0xF0,0x40,0x08,0x7F,0xE6,0x00,0x00,0x1F,0xE0,0x00,0x02,0x00,0x01,0xFF,0xFE,0x00,0x00},/*"测",0*/

{0x02,0x00,0x42,0x00,0x3B,0xFE,0x10,0x04,0x00,0x08,0x09,0x04,0x09,0x04,0x09,0xF8,0x09,0x08,0x09,0x08,0xFF,0x80,0x08,0x60,0x48,0x18,0x38,0x04,0x08,0x1E,0x00,0x00},/*"试",1*/

{0x00,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x03,0x00,0x01,0x00,0x00,0x00},/*"一",2*/

{0x00,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x7F,0xFE,0x44,0x00,0x46,0x00,0x43,0x80,0x41,0x00,0x40,0x00,0xC0,0x00,0x40,0x00,0x00,0x00},/*"下",3*/

{0x02,0x00,0x42,0x00,0x3B,0xFE,0x10,0x04,0x00,0x08,0x09,0x04,0x09,0x04,0x09,0xF8,0x09,0x08,0x09,0x08,0xFF,0x80,0x08,0x60,0x48,0x18,0x38,0x04,0x08,0x1E,0x00,0x00},/*"试",4*/

{0x02,0x00,0x42,0x00,0x3B,0xFE,0x10,0x04,0x00,0x08,0x09,0x04,0x09,0x04,0x09,0xF8,0x09,0x08,0x09,0x08,0xFF,0x80,0x08,0x60,0x48,0x18,0x38,0x04,0x08,0x1E,0x00,0x00},/*"试",5*/

};

void disp_all(unsigned char data1,unsigned char data2)
{
	uchar ii,jj;
	WriteCommand(0x15);     //设置列地址
	WriteCommand(0x00);     //列地址起始地址0
	WriteCommand(0x5f);     //列地址结束地址95
	WriteCommand(0x75);     //设置行地址
	WriteCommand(0x00);     //行地址起始地址0
	WriteCommand(0x3f);     //行地址结束地址63
	for(ii=0;ii<64;ii++)
	{
		for(jj=0;jj<96;jj++)
		{
            WriteData(data1);
            WriteData(data2);
		}
	}
}
    
void main()
{
    unsigned int i=0,j=0;
	OLED_init();
	OLED_init();
	delay_nms(100);
    
    disp_all(0x00,0x00);    //black
    

    
    /*
    OLED_draw_line(0,0,30,0,20,30,30);
    OLED_draw_line(0,5,30,5,10,0,0);
    OLED_draw_line(0,10,30,10,60,0,0);
    OLED_draw_rectangle(30,40,70,50,30,30,0,0,0,40);  
    OLED_RECTANGLE_FILL_ENABLE();
    OLED_RECTANGLE_FILL_DISABLE();
    OLED_draw_rectangle(80,0,90,40,30,30,0,0,10,0);
    */
    
    //OLED_draw_rectangle(40,40,50,50,30,30,0,0,0,0);
    //OLED_draw_line(50,50,60,62,0,30,30);    
    //      
    //
    
    OLED_draw_img(0,0,71,64,gImage_1);
    //OLED_putstring(0,20,0x07,0xf8,"litchiate @YLEEE"); 
    //OLED_write_chinese(0,0,6,table1);
/*
    OLED_draw_rectangle(0,0,60,60,0,30,0,0,0,0);
    OLED_draw_line(30,0,30,30,0,0,30);
    OLED_draw_line(30,30,60,30,0,0,30);
    OLED_draw_line(30,30,30,60,0,0,30);  
    OLED_draw_line(30,30,0,30,0,0,30);

    OLED_dot(2,2,30,0,0);
    
    OLED_dot(5,5,0,30,0);
    
    OLED_dot(10,10,0,0,30);
    
    
    OLED_circle(30,30,30,30,0,0); 
    OLED_circle(30,30,20,30,0,0); 
    OLED_circle(30,30,10,30,0,0); 
    OLED_circle(60,30,30,30,0,30); 
*/    
	while(1)
    {
       
    }
        
}