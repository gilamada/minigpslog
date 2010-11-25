/*----------------------------------------------------------
	nokia 5110 driver program for 51 mcu
	11.0592MHz
	void L5_init(void);  //LCD5110初始化
	void L5_wrbyte(bit,uchar); //写数据到LCD 0传送命令|1传送数据
	void L5_setxy(uchar,uchar); //设置LCD坐标函数
	void L5_clear(void); //LCD5110清屏
----------------------------------------------------------*/
#include "_STC90.h"
#include "_CHAR_USER.h"

//端口定义
sbit L5_SCLK = P2^0; //时钟
sbit L5_SDIN = P2^2; //数据
sbit L5_DC   = P2^4; //1数据|0命令
sbit L5_RST  = P2^5; //复位
sbit L5_CE   = P2^7; //使能
sbit L5_LED  = P3^6; //背光灯

//使用SPI接口写数据到LCD--------------------------------------------
void L5_wrbyte(bit L5_com,uchar L5_dat)
{
    uchar i;	        
    L5_CE = 0; //关闭LCD
    L5_DC = L5_com; //0传送命令|1传送数据
	for(i=8;i>0;i--){
		if(L5_dat&0x80)	L5_SDIN = 1; //输出数据
		else L5_SDIN = 0;
		L5_SCLK = 0; L5_dat<<=1; L5_SCLK = 1; //时钟
	}
	//L5_CE = 1; //使能LCD
}

//设置LCD坐标函数------------------------------------------------
void L5_setxy(uchar x, uchar y)
{
	L5_wrbyte(0,0x40|y); //行0~5
	L5_wrbyte(0,0x80|x); //列0~83
	L5_CE = 1; //使能LCD
}

//LCD清屏函数--------------------------------------------------------
void L5_clear(void)
{
    uint j;
    L5_setxy(0,0); //初始位置
	for(j=504;j>0;j--)
		L5_wrbyte(1,0x00); //写0数据
	L5_CE = 1; //使能LCD
}

//LCD5110初始化-------------------------------------------------------
void L5_init(void)
{            
	L5_RST = 0; _nms(100); L5_RST = 1; //产生一个复位的低电平脉冲	 
	L5_CE = 1; //关闭LCD
    L5_wrbyte(0,0x21); //使用扩展命令设置LCD模式
	//Vlcd=3.06+(VOP)*0.06,本例VOP为0B0101 0000为十进制的80，Vlcd=7.86V
	L5_wrbyte(0,0xd0); //设置偏置电压 0xc8
//	L5_wrbyte(0,0x06); //温度校正
//	L5_wrbyte(0,0x13); //设置偏置系统1:48
    L5_wrbyte(0,0x20); //使用基本命令
    L5_wrbyte(0,0x0c); //设定显示模式,正常显示 0x0d反显
	L5_CE = 1; //使能LCD
	L5_clear();	//清屏
}

//显示汉字 s汉字点阵数组位---------------------------------------------                     
void L5_wrchr_cn(uchar x,uchar y,uchar s)
{
    uchar n;    
	L5_setxy(x,y); //设置初始位置
    for(n=0;n<24;n++){ //写一个汉字
		if(n==12) L5_setxy(x,y+1);
		L5_wrbyte(1,HZ12[s][n]);
	}	
}

//显示汉字串-----------------------------------------------------------                     
void L5_wrstr_cn(uchar x,uchar y,uchar s,uchar num,uchar lrow)
{//num 显示汉字的个数; s 汉字点阵数组中的起始行数; lrow 汉字显示的列间距
    uchar t;    
    for(t=0;t<num;t++) L5_wrchr_cn(x+(lrow+12)*t,y,s+t);
	L5_CE = 1; //使能LCD
}




