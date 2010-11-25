/*******************************************************************************
*
* 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "config.h"
#include "iodevice.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define	TRUE	1
#define	FAULT	0

#define	KEY_REPEAT_SPEED	15		/* 按键连续有效时识别的速度，
									   即每秒识别的次数为=1000/(tick*(KEY_REPEAT_SPEED-1)) */

#define OBJECT_PINS			0x3ff	/* PE0-PE10 */
#define OBJECT_PORT			GPIOE
#define OBJECT_PORT_CLK		RCC_APB2Periph_GPIOE
#define OBJECT_CLEAR_PIN	GPIO_Pin_10		/* 靶纸数据清零脚 */

/* Private macro -------------------------------------------------------------*/
#define	ReadObjectValue()	(OBJECT_PORT->IDR & 0x3ff)

/* Private variables ---------------------------------------------------------*/
static unsigned short OldKeyValue = 0;
static char SameKeyCount = 0;
unsigned char Flag_KeyRepeat = 0;	//连续按键是否有效标志

unsigned char leds_disp_buff[128][16];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : IO_Device_Init()
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void IO_Device_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(VOICE_PORT_CLK | KEY_PORT_CLK | 
						   LED_PORT_CLK | OBJECT_PORT_CLK |
						   DISP_ADDR_PORT_CLK | DISP_DATA_PORT_CLK, ENABLE);

	/* 语音 */
	GPIO_InitStructure.GPIO_Pin = VOICE_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(VOICE_PORT, &GPIO_InitStructure);	

	/* LED指示灯 */
	GPIO_InitStructure.GPIO_Pin = LED_PINS;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//开漏输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(LED_PORT, &GPIO_InitStructure);	

	/* LED显示板 */
	GPIO_InitStructure.GPIO_Pin = DISP_ADDR_PIN;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(DISP_ADDR_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DISP_DATA_PIN;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(DISP_DATA_PORT, &GPIO_InitStructure);

	/* 键盘 */
	GPIO_InitStructure.GPIO_Pin = KEY_BACK_PIN|KEY_SELECT_PIN|KEY_UP_PIN|KEY_DOWN_PIN|KEY_LIFT_PIN|KEY_RIGHT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);	

	/* 靶纸扫描  */
	GPIO_InitStructure.GPIO_Pin = OBJECT_PINS;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(OBJECT_PORT, &GPIO_InitStructure);	

	/* 靶纸数据清零  */
	GPIO_InitStructure.GPIO_Pin = OBJECT_CLEAR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(OBJECT_PORT, &GPIO_InitStructure);	
	GPIO_ResetBits(OBJECT_PORT,OBJECT_CLEAR_PIN);
	
}


/*******************************************************************************
* Function Name  : Voice_Play()
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void Voice_play(unsigned short sound)
{
	unsigned short	portVal;

	portVal = VOICE_PORT->ODR & VOICE_MASK;

	VOICE_PORT->ODR = portVal | (sound<<VOICE_START_BIT);
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
unsigned char Key_scan(void)
{
	unsigned short portVal;

	portVal = KEY_PORT->IDR & KEY_MASK;

	if(portVal == KEY_MASK)
	{
		OldKeyValue = 0;
		SameKeyCount = 0;
		return (0);
	}
		
	if(portVal != OldKeyValue)
	{
		OldKeyValue = portVal;
		SameKeyCount = 0;
	}
	else
	{
		if(SameKeyCount == 0)
		{
			SameKeyCount = 1;
			return (OldKeyValue>>KEY_START_BIT);
		}
		else if(Flag_KeyRepeat == TRUE)
		{
			if(++SameKeyCount == KEY_REPEAT_SPEED)
			{
				SameKeyCount = 1;
				return (OldKeyValue>>KEY_START_BIT);
			}
		}
	}

	return (0);
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void Led_flash_all(unsigned short bits)
{
	unsigned short	portVal;

	portVal = LED_PORT->ODR & LED_MASK;

	LED_PORT->ODR = portVal | (bits<<LED_START_BIT);
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void Led_flash_one(unsigned char bit,unsigned char val)
{
	if(val)
		LED_PORT->BSRR = 1<<(LED_START_BIT+bit-1);
	else
		LED_PORT->BRR = 1<<(LED_START_BIT+bit-1);
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
void Led_flash_non(unsigned char bit)
{  
	if(LED_PORT->ODR & (1<<(LED_START_BIT+bit-1)))
		LED_PORT->BRR = 1<<(LED_START_BIT+bit-1);
	else
		LED_PORT->BSRR = 1<<(LED_START_BIT+bit-1);
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
unsigned short ObjectValue[][8] = {
/*   0环-1 0环-2 0环-3 0环-4 0环-5 0环-6 Test1 Test2	*/
	{0x3d9,0x3e1,0x3e2,0x3e3,0x3e4,0x3e5,0x3e6,0x3e7},
/*   5环-1 5环-2 5环-3 5环-4	*/
	{0x3d5,0x3d6,0x3d7,0x3d8},
/*	 6环-1 6环-2 6环-3 6环-4 6环-5 6环-6 6环-7 6环-8	*/
	{0x3b6,0x3b7,0x3b8,0x3b9,0x3d1,0x3d2,0x3d3,0x3d4},
/*	 7环-1 7环-2 7环-3 7环-4 7环-5 7环-6 7环-7 7环-8	*/
	{0x377,0x378,0x379,0x3b1,0x3b2,0x3b3,0x3b4,0x3b5},
/*	 8环-1 8环-2 8环-3 8环-4 8环-5 8环-6 8环-7 8环-8	*/
	{0x2f8,0x2f9,0x371,0x372,0x373,0x374,0x375,0x376},
/*	 9环-1 9环-2 9环-3 9环-4 9环-5 9环-6 9环-7 9环-8	*/
	{0x1f9,0x2f1,0x2f2,0x2f3,0x2f4,0x2f5,0x2f6,0x2f7},
/*	10环-1 10环-2 10环-3 10环-4 10环-5 10环-6 10环-7 10环-8	*/
	{0x1f1, 0x1f2,0x1f3, 0x1f4, 0x1f5, 0x1f6, 0x1f7, 0x1f8},
};

unsigned char Object_scan(void)
{
	unsigned short objVal,i,j;
	
	objVal = ReadObjectValue();

	if((objVal & 0x3f0) == 0)
		return	(0);

	for(i=0;i<7;i++)
	{
		for(j=0;i<8;j++)
		{
			if(objVal == ObjectValue[i][j])
				break;
		}
	}

	if(i == 7)
		return (0);
		
	if(i != 0)
		i += 4;

	return ((i<<4)+j);
}

 /*******************************************************************************
* Function Name  : 
* Description    : 
* Input 		 : None
* Output		 : None
* Return		 : None
*******************************************************************************/
unsigned char H_Addr[129] = {
0x00,0x08,0x04,0x0C,0x02,0x0A,0x06,0x0E,0x01,0x09,0x05,0x0D,0x03,0x0B,0x07,0x0F,
0x80,0x88,0x84,0x8C,0x82,0x8A,0x86,0x8E,0x81,0x89,0x85,0x8D,0x83,0x8B,0x87,0x8F,
0x40,0x48,0x44,0x4C,0x42,0x4A,0x46,0x4E,0x41,0x49,0x45,0x4D,0x43,0x4B,0x47,0x4F,
0xC0,0xC8,0xC4,0xCC,0xC2,0xCA,0xC6,0xCE,0xC1,0xC9,0xC5,0xCD,0xC3,0xCB,0xC7,0xCF,
0x20,0x28,0x24,0x2C,0x22,0x2A,0x26,0x2E,0x21,0x29,0x25,0x2D,0x23,0x2B,0x27,0x2F,
0xA0,0xA8,0xA4,0xAC,0xA2,0xAA,0xA6,0xAE,0xA1,0xA9,0xA5,0xAD,0xA3,0xAB,0xA7,0xAF,
0x60,0x68,0x64,0x6C,0x62,0x6A,0x66,0x6E,0x61,0x69,0x65,0x6D,0x63,0x6B,0x67,0x6F,
0xE0,0xE8,0xE4,0xEC,0xE2,0xEA,0xE6,0xEE,0xE1,0xE9,0xE5,0xED,0xE3,0xEB,0xE7,0xEF,
0XF0
};

unsigned char L_Addr[4][16] = {
{0x00,0x08,0x04,0x0C,0x02,0x0A,0x06,0x0E,0x01,0x09,0x05,0x0D,0x03,0x0B,0x07,0x0F},
{0x20,0x28,0x24,0x2C,0x22,0x2A,0x26,0x2E,0x21,0x29,0x25,0x2D,0x23,0x2B,0x27,0x2F},
{0x10,0x18,0x14,0x1C,0x12,0x1A,0x16,0x1E,0x11,0x19,0x15,0x1D,0x13,0x1B,0x17,0x1F},
{0x30}
};

void Leds_DispFill(unsigned char color)
{
	unsigned short addr,data,x,y;

	for(x=0; x<128; x++)
	{	
		for(y=0; y<16; y++)
		{
			addr = ~((H_Addr[x]<<8) + L_Addr[color][y]);
			DISP_ADDR_PORT->ODR = addr;
			data = DISP_DATA_PORT->ODR & 0x00ff;
			DISP_DATA_PORT->ODR = data | ((~leds_disp_buff[x][y])<<8);

			//vTaskDelay(1);
		}
	}
}





