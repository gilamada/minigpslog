#include "ssd1332.h"
#include "delay.h"
#include "font.h"

#define OLED_CS_0() OLED_CS_PORT &= ~OLED_CS
#define OLED_CS_1() OLED_CS_PORT |=  OLED_CS
#define OLED_RD_0() OLED_RD_PORT &= ~OLED_RD
#define OLED_RD_1() OLED_RD_PORT |=  OLED_RD
#define OLED_DC_0() OLED_DC_PORT &= ~OLED_DC
#define OLED_DC_1() OLED_DC_PORT |=  OLED_DC
#define OLED_WR_0() OLED_WR_PORT &= ~OLED_WR
#define OLED_WR_1() OLED_WR_PORT |=  OLED_WR


void WriteCommand(unsigned com)
{
	OLED_CS_DDR|=OLED_CS;
	OLED_CS_1();

	OLED_RD_DDR|=OLED_RD;		
	OLED_RD_1();		        

	OLED_DC_DDR|=OLED_DC;		
	OLED_DC_0();		        //DC=0;  WRITE COMMAND 

	OLED_WR_DDR|=OLED_WR;		
	OLED_WR_0();		        //ENABLE EQUIL 0	

	OLED_CS_0();

	OLED_DATA_DDR=0xff;			//OUTPUT
	OLED_DATA_PORT=com;			//OUTPUT THE INFORMATION

	delay_nms(1);
	
	OLED_CS_1();
	OLED_WR_1();
	OLED_DC_1();	
}

void WriteData(unsigned dat)
{
	OLED_CS_DDR|=OLED_CS;
	OLED_CS_PORT|=OLED_CS;

	OLED_DC_DDR|=OLED_DC;		//USE AS OUT
	OLED_DC_PORT|=OLED_DC;		//DC=1;
	
	OLED_WR_DDR|=OLED_WR;		//READ SET TO 
	OLED_WR_PORT&=~OLED_WR;

	OLED_RD_DDR|=OLED_RD;
	OLED_RD_PORT|=OLED_RD;

	OLED_CS_PORT&=~OLED_CS;
	
	OLED_DATA_DDR=0XFF;			//USE AS OUT
	OLED_DATA_PORT=dat;			//SEND USEFUL INFORMATION

//	delay_nms(1);

	OLED_CS_PORT|=OLED_CS;
	OLED_WR_PORT|=OLED_WR;
	OLED_DC_PORT|=OLED_DC;
}

void esbusini(void)
{
	OLED_WR_DDR|=OLED_WR;
	OLED_WR_PORT&=~OLED_WR;
	OLED_RD_DDR|=OLED_RD;
	OLED_RD_PORT&=~OLED_RD;
	OLED_CS_DDR|=OLED_CS;
	OLED_CS_PORT&=~OLED_CS;
	OLED_RES_DDR|=OLED_RES;
	OLED_RES_PORT&=~OLED_RES;
	delay_nms(100);
	OLED_RES_PORT|=OLED_RES;
	delay_nms(100);
}


void OLED_init(void)
{

	esbusini();
	WriteCommand(0x15);	//SET BIAS
	WriteCommand(0x00);	//Set line0 to COM0
	WriteCommand(0x81); //set High Brightness
	WriteCommand(0xdf);
	WriteCommand(0x82); //set High Brightness
	WriteCommand(0x1f);
	WriteCommand(0x83); //set High Brightness
	WriteCommand(0xff);
	WriteCommand(0x87); //set High Brightness
	WriteCommand(0x0f);

	WriteCommand(0xa0);//Set Re-map & DataFormat  地址自动增加
	//WriteCommand(0x60); //set 65k color format   256c
    WriteCommand(0x71);

	WriteCommand(0xa4); //set Normal Display
	WriteCommand(0xa8); //set Multiplex Ratio
	WriteCommand(0x3f);

	WriteCommand(0xa9); //set Power Control  加电
	WriteCommand(0x03);

	WriteCommand(0xaf); //set Display on

	WriteCommand(0xb8);//Set Gray Scale Table
	WriteCommand(0x01);
	WriteCommand(0x05);
	WriteCommand(0x09);
	WriteCommand(0x0d);
	WriteCommand(0x11);
	WriteCommand(0x15);
	WriteCommand(0x19);
	WriteCommand(0x1d);
	WriteCommand(0x21);
	WriteCommand(0x25);
	WriteCommand(0x29);
	WriteCommand(0x2d);
	WriteCommand(0x31);
	WriteCommand(0x35);
	WriteCommand(0x39);
	WriteCommand(0x3d);
	WriteCommand(0x41);
	WriteCommand(0x45);
	WriteCommand(0x49);
	WriteCommand(0x4d);
	WriteCommand(0x51);
	WriteCommand(0x55);
	WriteCommand(0x59);
	WriteCommand(0x5d);
	WriteCommand(0x61);
	WriteCommand(0x65);
	WriteCommand(0x69);
	WriteCommand(0x6d);
	WriteCommand(0x71);
	WriteCommand(0x75);
	WriteCommand(0x79);
	WriteCommand(0x7d);

	WriteCommand(0xbb);
	WriteCommand(0x7f);
	WriteCommand(0xbc);
	WriteCommand(0x7f);
	WriteCommand(0xbd);
	WriteCommand(0x7f);
}


void OLED_dot(uchar x,uchar y,uchar R,uchar G,uchar B)
{
    WriteCommand(0x15);     //设置列地址
	WriteCommand(x);        //列地址起始地址0
	WriteCommand(x+1);      //列地址结束地址95
	WriteCommand(0x75);     //设置行地址
	WriteCommand(y);        //行地址起始地址0
	WriteCommand(y+1);      //行地址结束地址63
    WriteData((B<<3)|(G>>3));
    WriteData((G<<5)|R);
}

void OLED_circle(uchar X,uchar Y,uchar R,uchar R1,uchar G1,uchar B1) 
{//Bresenham算法 
    uchar a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        OLED_dot(X+a,Y+b,R1,G1,B1);     //        7 
        OLED_dot(X-a,Y+b,R1,G1,B1);                                                        //        6 
        OLED_dot(X+a,Y-b,R1,G1,B1);                                                        //        2 
        OLED_dot(X-a,Y-b,R1,G1,B1);                                                        //        3 
        OLED_dot(X+b,Y+a,R1,G1,B1);                                                        //        8 
        OLED_dot(X-b,Y+a,R1,G1,B1);                                                        //        5 
        OLED_dot(X+b,Y-a,R1,G1,B1);                                                        //        1 
        OLED_dot(X-b,Y-a,R1,G1,B1);                                                        //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        OLED_dot(X+a,Y+b,R1,G1,B1); 
        OLED_dot(X+a,Y+b,R1,G1,B1); 
        OLED_dot(X+a,Y-b,R1,G1,B1); 
        OLED_dot(X-a,Y-b,R1,G1,B1); 
        OLED_dot(X+b,Y+a,R1,G1,B1); 
        OLED_dot(X-b,Y+a,R1,G1,B1); 
        OLED_dot(X+b,Y-a,R1,G1,B1); 
        OLED_dot(X-b,Y-a,R1,G1,B1); 
    } 
} 

void OLED_draw_line(uchar x1,uchar y1,uchar x2,uchar y2,uchar R,uchar G,uchar B)
{
    WriteCommand(0x21);     //DRAW LINE COMMAND
    WriteCommand(x1);       //column start address 
    WriteCommand(y1);       //row start address
    WriteCommand(x2);       //column end address
    WriteCommand(y2);       //row end address
    WriteCommand(B);
    WriteCommand(G);
    WriteCommand(R);
}

//If fill color option is disabled, the enclosed area will not be filled.
void OLED_draw_rectangle(uchar x1,uchar y1,uchar x2,uchar y2,uchar R1,uchar G1,uchar B1,uchar R2,uchar G2,uchar B2)
{
    WriteCommand(0x22);     //DRAW RECTANGLE COMMAND
    WriteCommand(x1);       //column start address 
    WriteCommand(y1);       //row start address
    WriteCommand(x2);       //column end address
    WriteCommand(y2);       //row end address
    WriteCommand(B1);       //line color
    WriteCommand(G1);
    WriteCommand(R1);
    WriteCommand(B2);       //fill color
    WriteCommand(G2);
    WriteCommand(R2);   
}

void OLED_draw_img(uchar x,uchar y,uchar x_size,uchar y_size,__flash uchar image[])
{
    uint i;
    
    WriteCommand(0x15);         //设置列地址
	WriteCommand(x);            //列地址起始地址0
	WriteCommand(x+x_size-1);   //列地址结束地址
	WriteCommand(0x75);         //设置行地址
	WriteCommand(y);            //行地址起始地址0
	WriteCommand(y+y_size-1);   //行地址结束地址
	for(i=0;i<(x_size*y_size*2);i++)
	{
        WriteData(image[i]);
	}
}

void OLED_putstring(uchar x,uchar y,uchar colorH,uchar colorL,uchar *text)
{
    uchar i;
    
    WriteCommand(0x15);     //设置列地址
	WriteCommand(x);        //列地址起始地址
	WriteCommand(95);       //列地址结束地址
	WriteCommand(0x75);     //设置行地址
	WriteCommand(y);        //行地址起始地址
	WriteCommand(y+7);      //行地址结束地址 

    while(*text)
    {
        for(i=0;i<8;i++)    //画8个点空一列,减小字模所占空间
        {
            WriteData(0x00);
            WriteData(0x00);
        }
	    for(i=0;i<40;i++)   //5*8 40个点为一个字母
	    {
            if(ascii[(*text-32)*5+i/8]&(0x80>>(i%8)))
		    {
                WriteData(colorH);
                WriteData(colorL);
		    }
            else
            {
                WriteData(0x00);
                WriteData(0x00);
            }
        }
        text++;
    }
}

void OLED_write_chinese(uchar x,uchar y,uchar num,__flash uchar buff[][32])
{
    uchar j;
    uint i;
    WriteCommand(0x15);     //设置列地址
    WriteCommand(x);        //列地址起始地址
    WriteCommand(95);       //列地址结束地址
    WriteCommand(0x75);     //设置行地址
    WriteCommand(y);        //行地址起始地址
    WriteCommand(y+15);     //行地址结束地址    
    
    for(j=0;j<num;j++)
    {
	    for(i=0;i<256;i++)
	    {
            if(buff[j][i/8]&(0x80>>(i%8)))
		    {
                WriteData(0x07);
                WriteData(0xe0);
		    }
            else
            {
                WriteData(0x00);
                WriteData(0x00);
            }
        }
    }
}