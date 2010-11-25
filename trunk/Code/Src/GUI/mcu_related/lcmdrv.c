/****************************************************************************
* �ļ�����LCMDRV.C
* ���ܣ�ST7565Sͼ��Һ��ģ����������ʹ��STM32��GPIO�ڿ��Ʋ�����
*       ����ZLG/GUI�û�ͼ�ν��档
* Һ��ģ����STM32������������ʾ��
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


/* ������ʾ������ */
TCOLOR  gui_disp_buf[GUI_LCM_YMAX/8][GUI_LCM_XMAX];

//#define LCM_PORT	GPIOD

/* ����������ʼ��GPIO����D0��Ӧ��GPIOֵ(PD8) */
//#define  BUS_NO		8

/* ����������ݺ궨�� */
#define	 OutData(dat)	LCM_PORT->ODR = (dat<<BUS_NO)|0xff 

/* ����R/W���� */
//#define  LCM_RW			GPIO_Pin_6
#define  SetRW()		LCM_PORT->BSRR = LCM_RW	
#define  ClrRW()		LCM_PORT->BRR = LCM_RW 

/* ����DI���� */
//#define  LCM_DI			GPIO_Pin_5
#define  SetDI()		LCM_PORT->BSRR = LCM_DI	
#define  ClrDI()		LCM_PORT->BRR = LCM_DI 

/* ����RST���� */
//#define  LCM_RST		GPIO_Pin_4
#define  SetRST()		LCM_PORT->BSRR = LCM_RST	
#define  ClrRST()		LCM_PORT->BRR = LCM_RST 


/* ����/En���� */
//#define  LCM_EN			GPIO_Pin_3
#define  SetEN()		LCM_PORT->BSRR = LCM_EN
#define  ClrEN()		LCM_PORT->BRR = LCM_EN 

/* ����LCM������������ */
#define	LCM_DISPON			0xaf	/* ��LCM��ʾ												*/
#define LCM_STARTROW		0xc0	/* ��ʾ��ʼ��0��������LCM_STARTROW+x������ʼ�С�(x<64) 		*/
#define	LCM_ADDRSTRX		0xb0	/* ҳ��ʼ��ַ��������LCM_ADDRSTRX+x���õ�ǰҳ(��X)��(x<8)	*/
#define	LCM_ADDRSTRY		0x40	/* ����ʼ��ַ��������LCM_ADDRSTRY+x���õ�ǰ��(��Y)��(x<64)	*/	


/*********************************************************************************
* ���ƣ�DELAY5()
* ���ܣ������ʱ����������LCM��ʾ���ʱ����ơ�
* ��ڲ�������
* ���ڲ�������
**********************************************************************************/
void DELAY5(void)
{ int i;

  for(i=0; i<50; i++);
}


/***********************************************************************
* ���ƣ�LCM_WrCommand()
* ���ܣ�д�����ӳ���
* ��ڲ�����command  	Ҫд��LCM��������
* ע�����ݿ�ΪP0��(��IO��)
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
* ���ƣ�LCM_WrData()
* ���ܣ�д�����ӳ���
* ��ڲ�����wrdata  	Ҫд��LCM������
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
* ���ƣ�LCM_WriteByte()
* ���ܣ���ָ����д����(һ�ֽ�)��
* ��ڲ�����x 		x����ֵ(0-127)
*	    	y       y����ֵ(0-63)
*           wrdata	��Ҫд������
* ˵��������������CS1/CS2�������ڲ�ָ��
***********************************************************************/
void LCM_WriteByte(uint8 x, uint8 y, uint8 wrdata) 
{ 
	x = x&0x7f;				// ��������
	y = y&0x3f;

  
	//������ʾ������
	y = y>>3;
	gui_disp_buf[y][x] = wrdata;
   
	// ����LCD��ʾ
	LCM_WrCommand(y+0xB0);  //set page address   
	LCM_WrCommand(0x10+(x>>4));  //set Column address MSB   
	LCM_WrCommand(0x00+(x&0x0f));  //set column address LSB   

	LCM_WrData(wrdata);
}


/***********************************************************************
* ���ƣ�LCM_ReadByte()
* ���ܣ���ȡָ�����ϵ����ݡ�
* ��ڲ�����x 	  x����ֵ(0-127)
*	   	    y     y����ֵ(0-63)
* ���ڲ��������ظõ��ϵ��ֽ����ݡ�
***********************************************************************/
uint8  LCM_ReadByte(uint8 x, uint8 y)
{ 
	x = x&0x7f;				// ��������
	y = y&0x3f;

	y = y>>3;
	return(gui_disp_buf[y][x]);
}



/////////////////////////////////////////////////////////////////////////


/***********************************************************************
* ���ƣ�LCM_DispFill()
* ���ܣ�����ʾ�������������
* ��ڲ�����filldata  	Ҫд��LCM���������
* ע���˺�����������ʾ��ʼ��Ϊ0���һ��Զ�ѡ��CS1��Ч
***********************************************************************/
// ���LCM
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
* ���ƣ�GUI_FillSCR()
* ���ܣ�ȫ����䡣ֱ��ʹ�����������ʾ��������
* ��ڲ�����dat		��������
* ���ڲ�������
* ˵�����û�����LCM��ʵ�������д�˺�����
****************************************************************************/
void  GUI_FillSCR(TCOLOR dat)
{  uint32 i,j;
  
   // ��仺����
   for(i=0; i<(GUI_LCM_YMAX/8); i++)
   {  for(j=0; j<GUI_LCM_XMAX; j++)
      {  gui_disp_buf[i][j] = dat;
      }
   }
   
   // ���LCM
   LCM_DispFill(dat);
}    
/***********************************************************************
* ���ƣ�LCM_DispIni()
* ���ܣ�LCM��ʾ��ʼ��
* ��ڲ�������
* ���ڲ�������
* ע��������ʾ��������������ʾ��ʼ��Ϊ0
*     �ḴλLCM_DISPCX��LCM_DISPCY.(����ֻѡ��CS1)
***********************************************************************/
void LCM_DispIni(void)
{ 
	uint32  i;

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;  //GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//10Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	// ��λLCM
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
* ���ƣ�GUI_Initialize()
* ���ܣ���ʼ��GUI��������ʼ����ʾ����������ʼ��LCM��������
* ��ڲ�������
* ���ڲ�������
* ˵�����û�����LCM��ʵ�������д�˺�����
****************************************************************************/
void  GUI_Initialize(void)
{  LCM_DispIni();					// ��ʼ��LCMģ�鹤��ģʽ����ͼ��ģʽ
   GUI_FillSCR(0x00);				// ��ʼ��������Ϊ0x00���������Ļ(����)
}


uint8 const  DEC_HEX_TAB[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
/****************************************************************************
* ���ƣ�GUI_Point()

* ���ܣ���ָ��λ���ϻ��㡣
* ��ڲ�����x		ָ���������е�λ��
*           y		ָ���������е�λ��
*           color	��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* ���ڲ���������ֵΪ1ʱ��ʾ�����ɹ���Ϊ0ʱ��ʾ����ʧ�ܡ�
* ˵��������ʧ��ԭ����ָ����ַ������������Χ��
****************************************************************************/
uint8  GUI_Point(uint8 x, uint8 y, TCOLOR color)
{  uint8   bak;
   
   // �������� 
   if(x>=GUI_LCM_XMAX) return(0);
   if(y>=GUI_LCM_YMAX) return(0);
   
   // ������Ӧ�ĵ�Ϊ1��0 
   bak = LCM_ReadByte(x,y);
   if(0==color)
   {  bak &= (~DEC_HEX_TAB[y&0x07]);
   }
   else
   {  bak |= DEC_HEX_TAB[y&0x07];
   }
  
   // ˢ����ʾ 
   LCM_WriteByte(x, y, bak);
   return(1);
}


/****************************************************************************
* ���ƣ�GUI_ReadPoint()
* ���ܣ���ȡָ�������ɫ��
* ��ڲ�����x		ָ���������е�λ��
*           y		ָ���������е�λ��
*           ret     ������ɫֵ��ָ��
* ���ڲ���������0��ʾָ����ַ������������Χ
* ˵�������ڵ�ɫ������ret��d0λΪ1��0��4���Ҷ���Ϊd0��d1��Ч��8λRGB��d0--d7��Ч��
*      RGB�ṹ��R��G��B������Ч��
****************************************************************************/
uint8  GUI_ReadPoint(uint8 x, uint8 y, TCOLOR *ret)
{  uint8  bak;

   // ��������
   if(x>=GUI_LCM_XMAX) return(0);
   if(y>=GUI_LCM_YMAX) return(0);
  
   bak = LCM_ReadByte(x,y);
   if( (bak & (DEC_HEX_TAB[y&0x07])) == 0 ) *ret = 0x00;
     else  *ret = 0x01;
   
   return(1);
}


/****************************************************************************
* ���ƣ�GUI_HLine()
* ���ܣ���ˮƽ�ߡ�
* ��ڲ�����x0		ˮƽ����������е�λ��
*           y0		ˮƽ����������е�λ��
*           x1      ˮƽ���յ������е�λ��
*           color	��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* ���ڲ�������
* ˵��������ʧ��ԭ����ָ����ַ������������Χ��
****************************************************************************/
void  GUI_HLine(uint8 x0, uint8 y0, uint8 x1, TCOLOR color) 
{  uint8  bak;

   if(x0>x1) 						// ��x0��x1��С�������У��Ա㻭ͼ
   {  bak = x1;
      x1 = x0;
      x0 = bak;
   }
   
   do
   {  GUI_Point(x0, y0, color);		// �����ʾ�������ֱ��
      x0++;
   }while(x1>=x0);
}


/***********************************************************************
* ���ƣ�GUI_RLine()
* ���ܣ�����ֱ�ߡ�����Ӳ���ص㣬ʵ�ּ��١�
* ��ڲ�����x0		��ֱ����������е�λ��
*           y0		��ֱ����������е�λ��
*           y1      ��ֱ���յ������е�λ��
*           color	��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* ���ڲ�����	��
* ˵��������ʧ��ԭ����ָ����ַ������������Χ��
***********************************************************************/
void  GUI_RLine(uint8 x0, uint8 y0, uint8 y1, TCOLOR color) 
{  uint8  bak;
   uint8  wr_dat;
  
   if(y0>y1) 		// ��y0��y1��С�������У��Ա㻭ͼ
   {  bak = y1;
      y1 = y0;
      y0 = bak;
   }
   
   do
   {  // �ȶ�ȡ��ǰ����ֽ�����
      bak = LCM_ReadByte(x0,y0);	
      
      // ����'��'/'��'�����󣬽���ȷ������д��LCM
      // ��y0��y1����ͬһ�ֽڣ���y0--��ǰ�ֽڽ�������(y0+8)&0x38��ȫд1������0��
      // ��y0��y1��ͬһ�ֽڣ���y0--y1��Ҫȫд1������0��
      // ������dat=0xff��Ȼ��y0����dat��λ����y1�����λ��
      if((y0>>3) != (y1>>3))		// ��ֱ���Ƿ��Խ�����ֽ�(������)
      {  wr_dat = 0xFF << (y0&0x07);// ��0��λ
      
         if(color)					
         {  wr_dat = bak | wr_dat;	// ��color��Ϊ0������ʾ
         }
         else
         {  wr_dat = ~wr_dat;		// ��colorΪ0���������ʾ
            wr_dat = bak & wr_dat;
         }
         LCM_WriteByte(x0,y0, wr_dat);
         y0 = (y0+8)&0x38;
      }
      else
      {  wr_dat = 0xFF << (y0&0x07);
         wr_dat = wr_dat &  ( 0xFF >> (7-(y1&0x07)) );
              
         if(color)					
         {  wr_dat = bak | wr_dat;	// ��color��Ϊ0������ʾ
         }
         else
         {  wr_dat = ~wr_dat;		// ��colorΪ0���������ʾ
            wr_dat = bak & wr_dat;
         }
         LCM_WriteByte(x0,y0, wr_dat);
         return;
      } // end of if((y0>>3) != (y1>>3))... else...
   }while(y1>=y0);

}




