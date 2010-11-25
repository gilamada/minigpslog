/*******************************************************************************
*
* 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "DS18B20.h"
#include "config.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define  SkipROM    0xCC     //跳过ROM
#define  SearchROM  0xF0	 //搜索ROM
#define  ReadROM    0x33	 //读ROM
#define  MatchROM   0x55	 //匹配ROM
#define  AlarmROM   0xEC	 //告警ROM

#define  StartConvert    0x44	 //开始温度转换，在温度转换期间总线上输出0，转换结束后输出1
#define  ReadScratchpad  0xBE	 //读暂存器的9个字节
#define  WriteScratchpad 0x4E	 //写暂存器的温度告警TH和TL
#define  CopyScratchpad  0x48	 //将暂存器的温度告警复制到EEPROM，在复制期间总线上输出0，复制完后输出1
#define  RecallEEPROM    0xB8    //将EEPROM的温度告警复制到暂存器中，复制期间输出0，复制完成后输出1
#define  ReadPower       0xB4    //读电源的供电方式：0为寄生电源供电；1为外部电源供电
 
#define DS_CONVERT_TICK	1000

#define WAIT_TIMEOUT 2000

/* Private macro -------------------------------------------------------------*/
#define EnableINT()  
#define DisableINT()

#define ResetDQ()	DS_PORT->BRR = DS_DQIO
#define SetDQ()		DS_PORT->BSRR = DS_DQIO
#define GetDQ()		(DS_PORT->IDR & DS_DQIO)?1:0
	
/* Private variables ---------------------------------------------------------*/	

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void Delay_us(u32 Nus) 
{  
/*	
	SysTick->LOAD=Nus*9;       			//时间加载       
	SysTick->CTRL|=0x01;            	//开始倒数     
	while(!(SysTick->CTRL&(1<<16)));	//等待时间到达  
	SysTick->CTRL=0X00000000;       	//关闭计数器 
	SysTick->VAL=0X00000000;        	//清空计数器  
*/
	u32	tick;

	tick = Nus*9;
	tick = SysTick->VAL;
	
}  


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void ResetDS18B20(void)
{
	unsigned int count = WAIT_TIMEOUT;
	
	SetDQ();
	Delay_us(50);
	
	ResetDQ();
	Delay_us(500);	 //500us （该时间的时间范围可以从480到960微秒）
	SetDQ();
	Delay_us(40);	 //40us
	while(GetDQ())
	{
		if(--count==0)
			break;
	}
	Delay_us(500);	 //500us
	SetDQ();
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void DS18B20WriteByte(unsigned char Dat)
{
	unsigned char i;
	
	for(i=8;i>0;i--)
	{
	 	ResetDQ();		  //在15u内送数到数据线上，DS18B20在15-60u读数
		Delay_us(5); 	  //5us
		if(Dat & 0x01)
			SetDQ();
		else
			ResetDQ();
		Delay_us(65); 	  //65us
		SetDQ();
		Delay_us(2); 	  //连续两位间应大于1us
		Dat >>= 1;	
	}	
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
unsigned char DS18B20ReadByte(void)
{
	unsigned char i,Dat;
	
	SetDQ();
	Delay_us(5);	
	for(i=8;i>0;i--)
	{
	 	Dat >>= 1;
	  	ResetDQ();		  //从读时序开始到采样信号线必须在15u内，且采样尽量安排在15u的最后
		Delay_us(5);	  //5us
		SetDQ();
		Delay_us(5);	  //5us
		if(GetDQ())
		 	Dat|=0x80;
		else
			Dat&=0x7f;		
		Delay_us(65);	  //65us
		SetDQ();
	}
	
	return Dat;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void ReadRom(unsigned char *Read_Addr)
{
	unsigned char i;

	DS18B20WriteByte(ReadROM);
		
	for(i=8;i>0;i--)
	{
		*Read_Addr=DS18B20ReadByte();
		Read_Addr++;
	}
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void DS18B20Init(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL)
{
	unsigned int count = WAIT_TIMEOUT;
	DisableINT();
	ResetDS18B20();
	DS18B20WriteByte(SkipROM);	
	DS18B20WriteByte(WriteScratchpad);
	DS18B20WriteByte(AlarmTL);
	DS18B20WriteByte(AlarmTH);
	DS18B20WriteByte(Precision);

	ResetDS18B20();
	DS18B20WriteByte(SkipROM);	
	DS18B20WriteByte(CopyScratchpad);
	EnableINT();

	while(!GetDQ())		 //等待复制完成
	{
		if(--count==0)
			break;
	}
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void DS18B20StartConvert(void)
{
	DisableINT();
	ResetDS18B20();
	DS18B20WriteByte(SkipROM);	
	DS18B20WriteByte(StartConvert);	
	EnableINT();
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void DS18B20_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DS_RCC_PORT, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DS_DQIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(DS_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void DS18B20_start(void)
{
	DS18B20_Configuration();
	DS18B20Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);
	DS18B20StartConvert();
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
float DS18B20_read(void)
{
	short  temperature;
	float fval;

	DisableINT();
 	ResetDS18B20();
	DS18B20WriteByte(SkipROM);	
	DS18B20WriteByte(ReadScratchpad);
	temperature = DS18B20ReadByte() + (DS18B20ReadByte()<<8);
	ResetDS18B20();
	EnableINT();

	DS18B20StartConvert();
	
	if(temperature & 0x8000)
	{
		temperature = ~temperature + 1;
		fval = 0 - temperature*0.0625;
	}
	else
		fval = temperature*0.0625;

	return 	fval;
}






