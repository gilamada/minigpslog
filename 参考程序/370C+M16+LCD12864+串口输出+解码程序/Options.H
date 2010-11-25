#include <iom16v.h>			//包含型号头文件
#include <macros.h>			//包含"位"操作头文件
#include <STDIO.H>


/******************数据类型宏定义******************/
#pragma interrupt_handler miao:9 //声明中断函数为miao 中断向量号为9
#define uchar unsigned char
#define uint unsigned int

#define TRUE 1
#define FALSE 0

uint temp;                 //实施温度值
uint eeprom_temp;          //设定温度值 
uint time_ms_Speaker;      //蜂鸣器计数器 ms
uint count;                //状态机流程
uint Speaker_delay;        //蜂鸣器延迟时间 ms
uint time_ms_Rxdata;       //通讯状态计数器 ms


/*******************************************
函数名称: Delayus
功    能: 延时指定微秒（8M晶振）
参    数: US--延时的微秒数(大约，不是很精确，MS越大越准确)
返回值  : 无
/********************************************/
void Delayus(uint US)		  
{
 uint i;
 US=US*5/4;		  	 //5/4是在8MHz晶振下，通过软件仿真反复实验得到的数值
 for( i=0;i<US;i++); 
}


/*******************************************
函数名称: Delayms
功    能: 延时指定毫秒（8M晶振）
参    数: MS--延时的毫秒数
返回值  : 无
/********************************************/
void Delayms(uint MS)		  
{
 uint i,j;
 for( i=0;i<MS;i++)
 for(j=0;j<1141;j++);	//1141是在8MHz晶振下，通过软件仿真反复实验得到的数值
}


/******************DS18B20常量******************/
#define DS18B20 PD6	//温度传感器管脚宏定义


/******************蜂鸣器、LED常量******************/
#define Speaker_IO PORTD	//蜂鸣器IO口定义
#define Speaker PD7	    //蜂鸣器管脚宏定义


/******************按键常量******************/
#define KEY_DDR DDRB			//按键方向定义
#define KEY_PORTO PORTB			//按键断口输出定义
#define KEY_PORTI PINB			//按键断口输入定义
#define OUT 0x03			    //输出常量 00000011
#define IN  0xFC			    //输入常量 11111100


/******************异步串口常量******************/
#define MCLK  8000000 	//主时钟频率宏定义


/******************12864液晶常量及操作******************/
#define iDat	1		//数据标志
#define iCmd	0		//指令标志
                                                                                                                                                                                                                                                                                                                                                                                                                                                         
#define LCDb_CTRL	    PORTD	//12864控制端口定义
#define LCDb_CTRL_DDR	DDRD	//控制端口方向寄存器定义
#define LCDb_RS	       	PD2	    //定义四个控制引脚
#define LCDb_RW		    PD3
#define LCDb_E	       	PD4
#define LCDb_RST	    PD5


#define LCDb_L1		    0x80	//第一行的地址
#define LCDb_L2		    0x90	//第二行的地址
#define LCDb_L3		    0x88	//第三行的地址
#define LCDb_L4		    0x98	//第四行的地址

#define LCDb_CGRAM_ADDR	0x40	//CGRAM的开始地址
#define LCDb_CGMAX 	    64	    //CGRAM存储的最大字节数

#define LCDb_SET_RS		LCDb_CTRL|=BIT(LCDb_RS)	//四个控制管脚的控制操作
#define LCDb_SET_RW 	LCDb_CTRL|=BIT(LCDb_RW)
#define LCDb_SET_E  	LCDb_CTRL|=BIT(LCDb_E)
#define LCDb_SET_RST  	LCDb_CTRL|=BIT(LCDb_RST)
#define LCDb_CLR_RS 	LCDb_CTRL&=~BIT(LCDb_RS)
#define LCDb_CLR_RW 	LCDb_CTRL&=~BIT(LCDb_RW)
#define LCDb_CLR_E  	LCDb_CTRL&=~BIT(LCDb_E)
#define LCDb_CLR_RST 	LCDb_CTRL&=~BIT(LCDb_RST)

#define LCDb_DO		    PORTC	//输出数据总线端口定义
#define LCDb_DI		    PINC	//输入数据总线端口定义
#define LCDb_DATA_DDR	DDRC	//数据总线方向寄存器定义

#define LCDb_FUNCTION	0x38   	// 液晶模式为8位，2行，5*8字符

#define LCDb_BASCMD	    0x30	// 基本指令集
#define LCDb_CLS	    0x01	// 清屏
#define LCDb_HOME	    0x02	// 地址返回原点，不改变DDRAM内容
#define LCDb_ENTRY 	    0x06	// 设定输入模式，光标加，屏幕不移动
#define LCDb_C2L	    0x10	// 光标左移
#define LCDb_C2R	    0x14	// 光标右移
#define LCDb_D2L	    0x18	// 屏幕左移
#define LCDb_D2R	    0x1C	// 屏幕又移
#define LCDb_ON		    0x0C	// 打开显示
#define LCDb_OFF		0x08	// 关闭显示

#define LCDb_EXTCMD1	0x34	// 扩充指令集，关闭绘图显示
#define LCDb_EXTCMD2	0x36	// 扩充指令集，打开绘图显示
#define LCDb_EXTCLS	    0x01	// 清屏
#define LCDb_REVL1	    0x04	// 反显第1行
#define LCDb_REVL2	    0x05	// 反显第1行
#define LCDb_REVL3	    0x06	// 反显第1行
#define LCDb_REVL4	    0x07	// 反显第1行


/*******************************************
函数名称: miao
功    能: 定期TC1中断
参    数: 无
返回值  : 无
/********************************************/
void miao(void)
{
	time_ms_Speaker++;    //蜂鸣器打开或者关闭时间计数
    time_ms_Rxdata++;     //串口空闲时间计数
	  
    TCNT1H=0XFF;//设置 TC1 的输入捕获寄存器 高8位值
    TCNT1L=0X82;//设置 TC1 的输入捕获寄存器 低8位值
}


/*******************************************
函数名称: Speaker_Set
功    能: 蜂鸣器报警音
参    数: Open--发音时间
          Close--停止时间
          Num--发音次数
返回值  : 无
/********************************************/
void Speaker_Set(uint Open, uint Close, uint Num)  
{
 uint i;
 for(i=0;i<Num;i++)
    {
        Speaker_IO&=~BIT(Speaker);
        Delayms(Open);
        Speaker_IO|=BIT(Speaker);
        Delayms(Close);
    }
}


/*******************************************
函数名称: Speaker_TC1_Set
功    能: 蜂鸣器报警音
参    数: Num--开关
返回值  : 无
/********************************************/
void Speaker_TC1_Set(uint Num)  
{
 uint i;
 if(Num==0)
 { 
     Speaker_IO|=BIT(Speaker);  //关闭蜂鸣器
 }
 else
 {
     switch(count)
     {
         case 1:
             if(time_ms_Speaker>=500)//关闭时间
             {
                 time_ms_Speaker=0;  //计数器变量置0
                 Speaker_IO&=~BIT(Speaker);  //打开蜂鸣器
                 count=2;
             }
             break;
         case 2:
             if(time_ms_Speaker>=500)//打开时间
             {
                 time_ms_Speaker=0;  //计数器变量置0
                 Speaker_IO|=BIT(Speaker);  //关闭蜂鸣器
                 count=1;
             }
             break;
         default:
             count=1;
     }
 }
}


/*******************************************
函数名称: Board_init
功    能: 初始所有IO口，关闭全部功能
参    数: 无
返回值  : 无
/********************************************/
void Board_init(void)
{
	DDRA=0xFF;	//将四个IO口全部配置为输出
	DDRB=0xFF;
	DDRC=0xFF;
	DDRD=0xFF;
	
	Speaker_IO|=BIT(Speaker);//关闭蜂鸣器
	//Speaker_IO&=~BIT(Speaker);//打开蜂鸣器
	
}