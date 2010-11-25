/*******************************************
  文件：OK400C.H
  功能：定义一些OK400C开发板上应用程序用到的常量
/******************************************/

#include <iom16v.h>			//包含型号头文件
#include <macros.h>			//包含"位"操作头文件

/******************数据类型宏定义******************/
#define uchar unsigned char
#define uint unsigned int

/******************数码管和LED常量******************/
#define LEDLK PC5	//LED锁存器控制端
#define SEGLK PC6	//数码管段选锁存器控制端
#define BITLK PC7	//数码管位选锁存器控制端

#define TRUE 1
#define FALSE 0
#define ALLLED 0XFF
/******************LED点阵对应io口******************/
#define SDATA_595  PC5   //串行数据输入
#define SCLK_595   PC6   //移位时钟脉冲
#define RCK_595    PC7   //输出锁存器控制脉冲

#define sdat1_595  PC0
#define sclk1_595  PC1
#define rck1_595   PC2
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
/*******************************************
函数名称: Board_init
功    能: 初始化实验板，关闭全部功能
参    数: 无
返回值  : 无
/********************************************/
void Board_init(void)
{
	DDRA=0xFF;	//将四个IO口全部配置为输出
	DDRB=0xFF;
	DDRC=0xFF;
	DDRD=0xFF;

	SPCR=0x00;	//还原SPI口，使其为正常端口	

	PORTC|=BIT(PC3);//(BEEP)关闭蜂鸣器
	PORTC&=~BIT(PC1);	  //(RELAY)初始化继电器为常闭连接状态	
	PORTA&=~BIT(PA1);	//关闭SPI总线DA
	PORTB=0xFF;	//关闭LED
	PORTA|=BIT(LEDLK);	//锁存数据，使LED熄灭
 	Delayus(5);		
 	PORTA&=~BIT(LEDLK);
	
	PORTB=0x00;	   //输出段选
 	PORTA|=BIT(SEGLK);		  
 	Delayus(50);			  
 	PORTA&=~BIT(SEGLK);	    
 	
 	PORTB=0xFF;	   //输出位选
 	PORTA|=BIT(BITLK);		   
 	Delayus(50);				   
 	PORTA&=~BIT(BITLK);	   
	
	PORTD=0xFF;
	PORTA|=BIT(PA7);
	Delayus(50);
	PORTA&=~BIT(PA7);

	
}
/******************1602液晶常量及操作******************/
#define LCDa_CTRL	PORTD	//1602控制端口定义
#define LCDa_CTRL_DDR	DDRD	//控制端口方向寄存器定义
#define LCDa_RS	       	PD5	//定义三个控制引脚
#define LCDa_RW		PD6
#define LCDa_E	       	PD4

#define LCDa_L1		0x80	//第一行的地址：0x80+addr ,addr为列数
#define LCDa_L2		0xC0	//第二行的地址：0x80+0x40+addr

#define LCDa_CGRAM_ADDR	0x40	//CGRAM的开始地址
#define LCDa_CGMAX 	64	//CGRAM存储的最大字节数

#define LCDa_SET_RS		LCDa_CTRL|=BIT(LCDa_RS)	//三个控制管脚的控制操作
#define LCDa_SET_RW 		LCDa_CTRL|=BIT(LCDa_RW)
#define LCDa_SET_E  		LCDa_CTRL|=BIT(LCDa_E)
#define LCDa_CLR_RS 		LCDa_CTRL&=~BIT(LCDa_RS)
#define LCDa_CLR_RW 		LCDa_CTRL&=~BIT(LCDa_RW)
#define LCDa_CLR_E  		LCDa_CTRL&=~BIT(LCDa_E)

#define LCDa_DO		PORTA	//输出数据总线端口定义
#define LCDa_DI		PINA	//输入数据总线端口定义
#define LCDa_DATA_DDR	DDRA	//数据总线方向寄存器定义

#define LCDa_FUNCTION	0x38   	// 液晶模式为8位，2行，5*8字符

#define iDat	1		//数据标志
#define iCmd	0		//指令标志

#define LCDa_CLS	0x01		// 清屏
#define LCDa_HOME	0x02		// 地址返回原点，不改变DDRAM内容
#define LCDa_ENTRY 	0x06		// 设定输入模式，光标加，屏幕不移动
#define LCDa_C2L	0x10		// 光标左移
#define LCDa_C2R	0x14		// 光标右移
#define LCDa_D2L	0x18		// 屏幕左移
#define LCDa_D2R	0x1C		// 屏幕又移

#define LCDa_ON		0x0C		// 打开显示
#define LCDa_OFF		0x08		// 关闭显示
#define LCDa_CURON		0x0E		// 显示光标
#define LCDa_CURFLA		0x0F		// 打开光标闪烁

/******************12864液晶常量及操作******************/
#define LCDb_CTRL	PORTD	//12864控制端口定义
#define LCDb_CTRL_DDR	DDRD	//控制端口方向寄存器定义
#define LCDb_RS	       	PD5	//定义四个控制引脚
#define LCDb_RW		PD6
#define LCDb_E	       	PD4
#define LCDb_RST	PD7


#define LCDb_L1		0x80	//第一行的地址
#define LCDb_L2		0x90	//第二行的地址
#define LCDb_L3		0x88	//第三行的地址
#define LCDb_L4		0x98	//第四行的地址

#define LCDb_CGRAM_ADDR	0x40	//CGRAM的开始地址
#define LCDb_CGMAX 	64	//CGRAM存储的最大字节数

#define LCDb_SET_RS		LCDb_CTRL|=BIT(LCDb_RS)	//四个控制管脚的控制操作
#define LCDb_SET_RW 		LCDb_CTRL|=BIT(LCDb_RW)
#define LCDb_SET_E  		LCDb_CTRL|=BIT(LCDb_E)
#define LCDb_SET_RST  		LCDb_CTRL|=BIT(LCDb_RST)
#define LCDb_CLR_RS 		LCDb_CTRL&=~BIT(LCDb_RS)
#define LCDb_CLR_RW 		LCDb_CTRL&=~BIT(LCDb_RW)
#define LCDb_CLR_E  		LCDb_CTRL&=~BIT(LCDb_E)
#define LCDb_CLR_RST 		LCDb_CTRL&=~BIT(LCDb_RST)

#define LCDb_DO		PORTA	//输出数据总线端口定义
#define LCDb_DI		PINA	//输入数据总线端口定义
#define LCDb_DATA_DDR	DDRA	//数据总线方向寄存器定义

#define LCDb_FUNCTION	0x38   	// 液晶模式为8位，2行，5*8字符

#define LCDb_BASCMD	0x30		// 基本指令集
#define LCDb_CLS	0x01		// 清屏
#define LCDb_HOME	0x02		// 地址返回原点，不改变DDRAM内容
#define LCDb_ENTRY 	0x06		// 设定输入模式，光标加，屏幕不移动
#define LCDb_C2L	0x10		// 光标左移
#define LCDb_C2R	0x14		// 光标右移
#define LCDb_D2L	0x18		// 屏幕左移
#define LCDb_D2R	0x1C		// 屏幕又移
#define LCDb_ON		0x0C		// 打开显示
#define LCDb_OFF		0x08		// 关闭显示

#define LCDb_EXTCMD1	0x34		// 扩充指令集，关闭绘图显示
#define LCDb_EXTCMD2	0x36		// 扩充指令集，打开绘图显示
#define LCDb_EXTCLS	0x01		// 清屏
#define LCDb_REVL1	0x04		// 反显第1行
#define LCDb_REVL2	0x05		// 反显第1行
#define LCDb_REVL3	0x06		// 反显第1行
#define LCDb_REVL4	0x07		// 反显第1行

/******************按键常量******************/
#define KEY_DDR DDRD			//按键方向定义
#define KEY_PORTO PORTD			//按键断口输出定义
#define KEY_PORTI PIND			//按键断口输入定义
#define OUT 0xFF			//输出常量
#define IN  0x0F			//输入常量

/******************按键外部中断常量******************/
#define GLOBAL 7	//全局中断位宏定义
#define EXTINT1 7	//外部中断1位宏定义
#define EXTINT0 6	//外部中断0位宏定义

/******************DA常量******************/

#define DAWR    PD6	//串行时钟线
#define DA_CS   PD2	//片选线

/******************DS18B20常量******************/
#define DS18B20 PC2

/******************TWI(IIC)常量******************/
#define START 0x08		//START信号发送完毕状态
#define MT_SLA_ACK 0x18		//从器件地址发送，返回ACK
#define MT_SLA_NOACK 0x20	//从器件地址发送，但是返回NOACK
#define MT_DATA_ACK  0x28	//数据已发送，返回ACK
#define MT_DATA_NOACK 0x30	//数据已发送，返回NOACK

#define RD_DEVICE_ADDR 0xA1   //前4位器件固定,后三位看连线,最后1位是读写指令位
#define WD_DEVICE_ADDR 0xA0

#define Start() (TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN))	//产生START信号
#define Stop() (TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN))	//产生STOP信号
#define Wait() while(!(TWCR&(1<<TWINT)))		//等待当前操作完成
#define TestACK() (TWSR&0xF8)				//取出状态码
#define SetACK() (TWCR|=(1<<TWEA))			//产生ACK
#define Twi()    (TWCR=(1<<TWINT)|(1<<TWEN))            //启动I2C
#define Writebyte(twi_d) {TWDR=(twi_d);TWCR=(1<<TWINT)|(1<<TWEN);}	//发送一个字节（twi_d为写入的数据）

/******************蜂鸣器音乐常量******************/
#define BEEP     PC3
//      计时值=65536-8000000/8/2/频率  
//      音名     计时值       频率Hz
#define DO_L     63627       //262
#define DOA_L    63731       //277
#define RE_L     63835       //294
#define REA_L    63928       //311
#define MI_L     64021       //330 
#define FA_L     64103       //349 
#define FAA_L    64185       //370
#define SO_L     64270       //392
#define SOA_L    64331       //415 
#define LA_L     64400       //440
#define LAA_L    64463       //466
#define TI_L     64524       //494 
#define DO       64580       //523
#define DOA      64633       //554
#define RE       64684       //587 
#define REA      64732       //622
#define MI       64777       //659 
#define FA       64820       //698 
#define FAA      64860       //740
#define SO       64898       //784 
#define SOA      64934       //831
#define LA       64968       //880  
#define LAA      65000       //932
#define TI       65030       //988                                               
#define DO_H     65058       //1046
#define DOA_H    65085       //1109
#define RE_H     65110       //1175
#define REA_H    65134       //1245 
#define MI_H     65157       //1318 
#define FA_H     65178       //1397
#define FAA_H    65198       //1480
#define SO_H     65217       //1568 
#define SOA_H    65235       //1661
#define LA_H     65252       //1760 
#define LAA_H    65268       //1865
#define TI_H     65283       //1976 
#define ZERO     0           //休止符
//十进制:     0
//休止符:     0
 
//十进制:     1     2     3     4     5     6     7     8     9     10    11     12
//低音  :     1     #1    2     #2    3     4     #4    5     #5    6     #6     7
 
//十进制:     13    14    15    16    17    18    19    20    21    22    23     24
// 中音 :     1     #1    2     #2    3     4     #4    5     #5    6     #6     7

//十进制:     25    26    27    28    29    30    31    32    33    34    35     36
// 高音 :     1     #1    2     #2    3     4     #4    5     #5    6     #6     7

/******************继电器常量******************/
#define RELAY PC1	//继电器管脚宏定义

/******************异步串口常量******************/
#define MCLK  8000000 	//主时钟频率宏定义
/******************DS12C887常量******************/
#define  RTC_CS      PC0
#define  RTC_DS      PD7
#define  RTC_RW      PD6
#define  RTC_AS      PD5


/******************通用子函数******************/
void Delayms(uint MS);

/******************LED子函数******************/
void One_led(uchar number,uchar light);
void All_led(uchar light);
void Flow_LED(uchar MS);
void Flash_LED(uchar number,uchar MS);

/******************数码管子函数******************/
void One_smg_display(uchar data,uchar number);
void All_smg_display(uchar *pdata);
void Cycle_smg_display(uchar *pdata);

/******************1602液晶管子函数******************/
void LCD1602_portini(void);
uchar LCD1602_readbyte(uchar DatCmd);
void LCD1602_sendbyte(uchar DatCmd, uchar dByte);
void LCD1602_sendstr(uchar *ptString);
void LCD1602_clear(void);
uchar LCD1602_readBF(void);
void LCD1602_gotoXY(uchar Row, uchar Col);
void LCD1602_initial(void);

