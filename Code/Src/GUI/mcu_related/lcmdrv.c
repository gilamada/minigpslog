/****************************************************************************
* 文件名：LCMDRV.C
* 功能：ST7565S图形液晶模块驱动程序。使用STM32的GPIO口控制操作。
*       用于ZLG/GUI用户图形界面。
* 液晶模块与STM32的连接如下所示：
*	D7 		--		PD15
*	.		--		.
*	.		--		.
*	.		--		.
*	D0 		--		PD8
*
*  	/RD		--		PD7
*	/WR		--		PD6
*	D/I		--		PD5
*   /RST    --      PD4
*	/CS		--		PD3
*
****************************************************************************/
#include "stm32f10x.h"
#include "config.h"
#include "GUI.h"


/* 定义显示缓冲区 */
TCOLOR  gui_disp_buf[GUI_LCM_YMAX/8][GUI_LCM_XMAX];

//#define LCM_PORT	GPIOD

/* 定义总线起始的GPIO，即D0对应的GPIO值(PD8) */
//#define  BUS_NO		8

/* 输出总线数据宏定义 */
#define	 OutData(dat)	LCM_PORT->ODR = (dat<<BUS_NO)|0xff 

/* 定义R/W控制 */
//#define  LCM_RW			GPIO_Pin_6
#define  SetRW()		LCM_PORT->BSRR = LCM_RW	
#define  ClrRW()		LCM_PORT->BRR = LCM_RW 

/* 定义DI控制 */
//#define  LCM_DI			GPIO_Pin_5
#define  SetDI()		LCM_PORT->BSRR = LCM_DI	
#define  ClrDI()		LCM_PORT->BRR = LCM_DI 

/* 定义RST控制 */
//#define  LCM_RST		GPIO_Pin_4
#define  SetRST()		LCM_PORT->BSRR = LCM_RST	
#define  ClrRST()		LCM_PORT->BRR = LCM_RST 


/* 定义/En控制 */
//#define  LCM_EN			GPIO_Pin_3
#define  SetEN()		LCM_PORT->BSRR = LCM_EN
#define  ClrEN()		LCM_PORT->BRR = LCM_EN 

/* 定义LCM操作的命令字 */
#define	LCM_DISPON			0xaf	/* 打开LCM显示												*/
#define LCM_STARTROW		0xc0	/* 显示起始行0，可以用LCM_STARTROW+x设置起始行。(x<64) 		*/
#define	LCM_ADDRSTRX		0xb0	/* 页起始地址，可以用LCM_ADDRSTRX+x设置当前页(即X)。(x<8)	*/
#define	LCM_ADDRSTRY		0x40	/* 列起始地址，可以用LCM_ADDRSTRY+x设置当前列(即Y)。(x<64)	*/	


/*********************************************************************************
* 名称：DELAY5()
* 功能：软件延时函数。用于LCM显示输出时序控制。
* 入口参数：无
* 出口参数：无
**********************************************************************************/
void DELAY5(void)
{ int i;

  for(i=0; i<50; i++);
}


/***********************************************************************
* 名称：LCM_WrCommand()
* 功能：写命令子程序
* 入口参数：command  	要写入LCM的命令字
* 注：数据口为P0口(作IO口)
***********************************************************************/
void LCM_WrCommand(uint8 command) 
{ 
	ClrEN();
	ClrDI();
	OutData(command);
	ClrRW();
	DELAY5();
	SetEN();
}


/***********************************************************************
* 名称：LCM_WrData()
* 功能：写数据子程序
* 入口参数：wrdata  	要写入LCM的数据
***********************************************************************/
void LCM_WrData(uint8 wrdata) 
{ 
	ClrEN();
	SetDI();
	OutData(wrdata);
	ClrRW();
	DELAY5();
	SetEN();
}


/***********************************************************************
* 名称：LCM_WriteByte()
* 功能：向指定点写数据(一字节)。
* 入口参数：x 		x坐标值(0-127)
*	    	y       y坐标值(0-63)
*           wrdata	所要写的数据
* 说明：会重新设置CS1/CS2，及其内部指针
***********************************************************************/
void LCM_WriteByte(uint8 x, uint8 y, uint8 wrdata) 
{ 
	x = x&0x7f;				// 参数过滤
	y = y&0x3f;

  
	//更新显示缓冲区
	y = y>>3;
	gui_disp_buf[y][x] = wrdata;
   
	// 更新LCD显示
	LCM_WrCommand(y+0xB0);  //set page address   
	LCM_WrCommand(0x10+(x>>4));  //set Column address MSB   
	LCM_WrCommand(0x00+(x&0x0f));  //set column address LSB   

	LCM_WrData(wrdata);
}


/***********************************************************************
* 名称：LCM_ReadByte()
* 功能：读取指定点上的数据。
* 入口参数：x 	  x坐标值(0-127)
*	   	    y     y坐标值(0-63)
* 出口参数：返回该点上的字节数据。
***********************************************************************/
uint8  LCM_ReadByte(uint8 x, uint8 y)
{ 
	x = x&0x7f;				// 参数过滤
	y = y&0x3f;

	y = y>>3;
	return(gui_disp_buf[y][x]);
}



/////////////////////////////////////////////////////////////////////////


/***********************************************************************
* 名称：LCM_DispFill()
* 功能：向显示缓冲区填充数据
* 入口参数：filldata  	要写入LCM的填充数据
* 注：此函数会设置显示起始行为0，且会自动选中CS1有效
***********************************************************************/
// 填充LCM
void LCM_DispFill(uint8 filldata)
{ 
	uint8  x,y;
	
	for(x=0xB0;x<=0xB7;x++)
	{
		LCM_WrCommand(x);  //set page address   
		LCM_WrCommand(0x10);  //set Column address MSB   
		LCM_WrCommand(0x00);  //set column address LSB   
		for(y=0;y<128;y++)  
			LCM_WrData(filldata);  	
	}
}

/****************************************************************************
* 名称：GUI_FillSCR()
* 功能：全屏填充。直接使用数据填充显示缓冲区。
* 入口参数：dat		填充的数据
* 出口参数：无
* 说明：用户根据LCM的实际情况编写此函数。
****************************************************************************/
void  GUI_FillSCR(TCOLOR dat)
{  uint32 i,j;
  
   // 填充缓冲区
   for(i=0; i<(GUI_LCM_YMAX/8); i++)
   {  for(j=0; j<GUI_LCM_XMAX; j++)
      {  gui_disp_buf[i][j] = dat;
      }
   }
   
   // 填充LCM
   LCM_DispFill(dat);
}    
/***********************************************************************
* 名称：LCM_DispIni()
* 功能：LCM显示初始化
* 入口参数：无
* 出口参数：无
* 注：初化显示后，清屏并设置显示起始行为0
*     会复位LCM_DISPCX，LCM_DISPCY.(并会只选中CS1)
***********************************************************************/
void LCM_DispIni(void)
{ 
	uint32  i;

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;  //GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//10M时钟速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	// 复位LCM
	ClrRST();				
	for(i=0; i<5000; i++);
	SetRST();  
	for(i=0; i<5000; i++); 

	LCM_WrCommand(0xA2);    //LCD Bias selection(1/65 Duty,1/9Bias)  
	LCM_WrCommand(0xA0);    //ADC selection(SEG0->SEG128)  
	LCM_WrCommand(0xC8);    //SHL selection(COM64->COM0)   
	LCM_WrCommand(0x26);    //Regulator Resistor Selection   
	LCM_WrCommand(0x81);    //Electronic Volume   
	LCM_WrCommand(0x8);    //Reference Register selection  Vo=(1+Rb/Ra)(1+a)*2.1=10  
	// LCM_WrCommand(VC_ON);    //Power Control(Vc=1;Vr=0;Vf=0)    
	// LCM_WrCommand(VC_ON|VR_ON);  

	LCM_WrCommand(0x2f);  
	LCM_WrCommand(0xF8); 
	LCM_WrCommand(0x00);  
	LCM_WrCommand(0xAF);    //Display on 

}



/////////////////////////////////////////////////////////////////////////////





/****************************************************************************
* 名称：GUI_Initialize()
* 功能：初始化GUI，包括初始化显示缓冲区，初始化LCM并清屏。
* 入口参数：无
* 出口参数：无
* 说明：用户根据LCM的实际情况编写此函数。
****************************************************************************/
void  GUI_Initialize(void)
{  LCM_DispIni();					// 初始化LCM模块工作模式，纯图形模式
   GUI_FillSCR(0x00);				// 初始化缓冲区为0x00，并输出屏幕(清屏)
}


uint8 const  DEC_HEX_TAB[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
/****************************************************************************
* 名称：GUI_Point()

* 功能：在指定位置上画点。
* 入口参数：x		指定点所在列的位置
*           y		指定点所在行的位置
*           color	显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：返回值为1时表示操作成功，为0时表示操作失败。
* 说明：操作失败原因是指定地址超出缓冲区范围。
****************************************************************************/
uint8  GUI_Point(uint8 x, uint8 y, TCOLOR color)
{  uint8   bak;
   
   // 参数过滤 
   if(x>=GUI_LCM_XMAX) return(0);
   if(y>=GUI_LCM_YMAX) return(0);
   
   // 设置相应的点为1或0 
   bak = LCM_ReadByte(x,y);
   if(0==color)
   {  bak &= (~DEC_HEX_TAB[y&0x07]);
   }
   else
   {  bak |= DEC_HEX_TAB[y&0x07];
   }
  
   // 刷新显示 
   LCM_WriteByte(x, y, bak);
   return(1);
}


/****************************************************************************
* 名称：GUI_ReadPoint()
* 功能：读取指定点的颜色。
* 入口参数：x		指定点所在列的位置
*           y		指定点所在行的位置
*           ret     保存颜色值的指针
* 出口参数：返回0表示指定地址超出缓冲区范围
* 说明：对于单色，设置ret的d0位为1或0，4级灰度则为d0、d1有效，8位RGB则d0--d7有效，
*      RGB结构则R、G、B变量有效。
****************************************************************************/
uint8  GUI_ReadPoint(uint8 x, uint8 y, TCOLOR *ret)
{  uint8  bak;

   // 参数过滤
   if(x>=GUI_LCM_XMAX) return(0);
   if(y>=GUI_LCM_YMAX) return(0);
  
   bak = LCM_ReadByte(x,y);
   if( (bak & (DEC_HEX_TAB[y&0x07])) == 0 ) *ret = 0x00;
     else  *ret = 0x01;
   
   return(1);
}


/****************************************************************************
* 名称：GUI_HLine()
* 功能：画水平线。
* 入口参数：x0		水平线起点所在列的位置
*           y0		水平线起点所在行的位置
*           x1      水平线终点所在列的位置
*           color	显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：无
* 说明：操作失败原因是指定地址超出缓冲区范围。
****************************************************************************/
void  GUI_HLine(uint8 x0, uint8 y0, uint8 x1, TCOLOR color) 
{  uint8  bak;

   if(x0>x1) 						// 对x0、x1大小进行排列，以便画图
   {  bak = x1;
      x1 = x0;
      x0 = bak;
   }
   
   do
   {  GUI_Point(x0, y0, color);		// 逐点显示，描出垂直线
      x0++;
   }while(x1>=x0);
}


/***********************************************************************
* 名称：GUI_RLine()
* 功能：画竖直线。根据硬件特点，实现加速。
* 入口参数：x0		垂直线起点所在列的位置
*           y0		垂直线起点所在行的位置
*           y1      垂直线终点所在行的位置
*           color	显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：	无
* 说明：操作失败原因是指定地址超出缓冲区范围。
***********************************************************************/
void  GUI_RLine(uint8 x0, uint8 y0, uint8 y1, TCOLOR color) 
{  uint8  bak;
   uint8  wr_dat;
  
   if(y0>y1) 		// 对y0、y1大小进行排列，以便画图
   {  bak = y1;
      y1 = y0;
      y0 = bak;
   }
   
   do
   {  // 先读取当前点的字节数据
      bak = LCM_ReadByte(x0,y0);	
      
      // 进行'与'/'或'操作后，将正确的数据写回LCM
      // 若y0和y1不是同一字节，则y0--当前字节结束，即(y0+8)&0x38，全写1，或者0。
      // 若y0和y1是同一字节，则y0--y1，要全写1，或者0。
      // 方法：dat=0xff，然后按y0清零dat低位，按y1清零高位。
      if((y0>>3) != (y1>>3))		// 竖直线是否跨越两个字节(或以上)
      {  wr_dat = 0xFF << (y0&0x07);// 清0低位
      
         if(color)					
         {  wr_dat = bak | wr_dat;	// 若color不为0，则显示
         }
         else
         {  wr_dat = ~wr_dat;		// 若color为0，则清除显示
            wr_dat = bak & wr_dat;
         }
         LCM_WriteByte(x0,y0, wr_dat);
         y0 = (y0+8)&0x38;
      }
      else
      {  wr_dat = 0xFF << (y0&0x07);
         wr_dat = wr_dat &  ( 0xFF >> (7-(y1&0x07)) );
              
         if(color)					
         {  wr_dat = bak | wr_dat;	// 若color不为0，则显示
         }
         else
         {  wr_dat = ~wr_dat;		// 若color为0，则清除显示
            wr_dat = bak & wr_dat;
         }
         LCM_WriteByte(x0,y0, wr_dat);
         return;
      } // end of if((y0>>3) != (y1>>3))... else...
   }while(y1>=y0);

}




