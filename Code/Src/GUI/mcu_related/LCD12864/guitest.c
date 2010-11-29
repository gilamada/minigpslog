//================================================================================================
// �ļ�����:  main.c 
// оƬ����: STM32103RBT6                                            
// ��������: Keil v3.90, Windows XP2 
// ����ʱ��: 2010��01��05��                                                               
// ������: ��STM32����ֲZLG_GUI������ʹ��UC1698оƬ��160160��ɫ����ģ�飬��ģ��㷺Ӧ���ڵ����ز��ն� 
// ��д��Ա: �޳�                                                                       
// ��ϵ��ʽ: 13686036830  soulcoffee@sohu.com   
//================================================================================================ 

//========================================= ͷ�ļ� ================================================ 	
#include "config.h"

//======================================== �������� =============================================== 

// һ����ɫͼ������ݣ�����x�߶�=40x40
uint8 const ICO1[] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x07,0x00,0xE0,0x00,0x00,0x18,
0x00,0x18,0x00,0x00,0x30,0x00,0x0C,0x00,0x00,0x40,0x00,0x02,0x00,0x00,0x80,0x00,
0x01,0x00,0x01,0x00,0x00,0x00,0x80,0x03,0x00,0x00,0x00,0xC0,0x02,0x01,0x81,0x80,
0x40,0x04,0x03,0xC3,0xC0,0x20,0x04,0x03,0xC3,0xC0,0x20,0x04,0x03,0xC3,0xC0,0x20,
0x08,0x03,0xC3,0xC0,0x10,0x08,0x03,0xC3,0xC0,0x10,0x08,0x01,0x81,0x80,0x10,0x08,
0x08,0x00,0x10,0x10,0x08,0x08,0x00,0x10,0x10,0x08,0x10,0x00,0x08,0x10,0x08,0xF0,
0x00,0x0F,0x10,0x08,0x08,0x00,0x10,0x10,0x04,0x04,0x00,0x20,0x20,0x04,0x06,0x00,
0x60,0x20,0x04,0x03,0x81,0xC0,0x20,0x02,0x01,0x7E,0x80,0x40,0x03,0x00,0x81,0x00,
0xC0,0x01,0x00,0x42,0x00,0x80,0x00,0x80,0x3C,0x01,0x00,0x00,0x40,0x00,0x02,0x00,
0x00,0x30,0x00,0x0C,0x00,0x00,0x18,0x00,0x18,0x00,0x00,0x07,0x00,0xE0,0x00,0x00,
0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// �������ߵ�4���˵�
PointXY log_pin[] = { {80,  30},
                      {120, 60},
                      {160, 30},
                      {200, 60}
                    };

// ��ն���εĸ�������                    
uint32  const poly6[] = {150,110, 170,80, 180,95, 200,85, 230,110, 150,110};
                      

uint8  str[256];
uint32 Time_count = 0;
uint32 rr;
void delay()
{
  Delay_ms(1);
}  

WINDOWS Win_Main[] = {0,0,160,160,"ZLG_GUI","Programme by CuiChao" }  ;

MMENU  Menu_Main = {Win_Main ,5,{"File","Edit","View","Tool","Help" } };

SMENU  Menu_Slave = { Win_Main,0,5,{"New","Open","Close","Save","Exit"},0,{delay,delay,delay,delay,delay, } } ;


//============================================================================= 
//�� �� ��: main() 
//��    ��: C��ں���  
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//============================================================================= 
int main (void)
{			 
	uint32 Time_count_old = 100;


  	TargetInit();			/* Ӧ�ó����ʼ��   */ 

	GUI_Initialize();				// ��ʼ��LCM 

   GUI_SetColor(1, 0);				// ����ǰ��ɫ������ɫ

   
  

  
  // GUI_Spline(log_pin, 4, 1); 					// ����������
  // GUI_LineS(poly6, 6, 1);						// ����յĶ����
 //  GUI_FloodFill(159,100, 1);					// �������

   GUI_WindowsDraw(Win_Main);
   GUI_MMenuDraw(&Menu_Main);
   // GUI_PutString(10,20,"WWW.ZLGMCU.COM");		// ��ʾ�ַ���
   GUI_MMenuSelect(&Menu_Main,0);
   GUI_SMenuDraw(&Menu_Slave);

	GUI_LoadPic(80,60, (uint8 *)ICO1, 40, 40);	// ��ʾ40��40��ͼ��		 
	while(1)
	{
		// ÿ�����ֵ��һ�Σ�������TIMER.C��
		if(Time_count != Time_count_old)
		{
		 ;
 
		
		}
	}
}

//================================================================================================= 
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//================================================================================================= 