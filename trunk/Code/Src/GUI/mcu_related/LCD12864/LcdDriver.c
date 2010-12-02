//================================================================================================
// �ļ�����: LcdDriver.C 
// оƬ����: MSP430F149                                            
// ��������: IAR 3.42, Windows XP2 
// ����ʱ��: 2009��10��20��                                                               
// ������: Ks0108 Һ���������������� 
// ��д��Ա: �޳�                                                                       
// ��ϵ��ʽ: 13686036830  cuichao@xrd-lcd.com   
//================================================================================================ 
 
#include "config.h"	
#include "Chinese.C"
#include "ASCII.c"


//Һ��������

#define LCD_Bus         GPIOC->ODR	  

#define LCD_CS1_H()     {GPIOB->BSRR = GPIO_Pin_7;}
#define LCD_CS1_L()     {GPIOB->BRR = GPIO_Pin_7;}
#define LCD_CS2_H()     {GPIOB->BSRR = GPIO_Pin_8;}
#define LCD_CS2_L()     {GPIOB->BRR = GPIO_Pin_8;}
#define LCD_RS_H()      {GPIOB->BSRR = GPIO_Pin_0;}           
#define LCD_RS_L()      {GPIOB->BRR = GPIO_Pin_0;}                                     
#define LCD_RW_H()      {GPIOB->BSRR = GPIO_Pin_1;}                          
#define LCD_RW_L()      {GPIOB->BRR = GPIO_Pin_1;}                                    
#define LCD_E_H()       {GPIOB->BSRR = GPIO_Pin_6;}                                      
#define LCD_E_L()       {GPIOB->BRR = GPIO_Pin_6;}                                   
#define LCD_RST_H()     {GPIOB->BSRR = GPIO_Pin_9;}                                  
#define LCD_RST_L()     {GPIOB->BRR = GPIO_Pin_9;}                               

//3��LCDƬѡ�ź�

#define LCD_CSA()   {\
		LCD_CS1_H();   \
		LCD_CS2_L(); } 

#define LCD_CSB(){\
   		LCD_CS1_L();    \
      	LCD_CS2_H(); }  

#define LCD_CS_None() {\
 		LCD_CS1_L();    \
		LCD_CS2_L();} 


//------------------------------------------------------------------------------------------

/*
-------------------------------------------------------------------------
GUI���ȫ�ֱ���
-------------------------------------------------------------------------
*/
INT8U   GUIEnFont = En_8x16;                //Ӣ������
INT8U   GUITextMode = GUI_TEXTMODE_NORMAL;  //��ʾģʽ

INT8U  Char_XSIZE;   //Ӣ������X����
INT8U  Char_YSIZE;   //Ӣ������Y����

INT8U  PosX = 0;     //��ǰϵͳX����
INT8U  PosY = 0;     //��ǰϵͳY����

INT8U  NoXY = true;//�Ƿ�ʹ��PosX PosY�ı�־   1-->��ʹ��   0-->ʹ��
void SomeNop() 
 { 
 	uint32 i;
	for(i=0;i<1000;i++)
	{i++;i--;}
 }  

 
void LCD_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOC lcd data clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  0x0000ffff;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
   /* Enable GPIOA  clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  0x0000ffff;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*
*****************************************************************************
* WriteCommand_0108 - д���KS0108
* DESCRIPTION: -
* ���������ӹ�Ƭѡ�ź�
* @Param CmdData:������
* @Return :
* ----
*****************************************************************************
*/
void WriteCommand_0108(INT8U CmdData)
{
    //PUSH(IE);//����IE
    //EA = 0 ;//�ر��ж� ��ֹ�������	
    
    LCD_E_L();
	SomeNop();
    LCD_RS_L();
 	SomeNop();
	LCD_RW_L();
	SomeNop();
    LCD_Bus = (INT16U) CmdData;
    SomeNop();
    LCD_E_H();
    SomeNop();
    LCD_E_L();
	SomeNop();

    //POP(IE);//���»ָ�IE	


}


/*
*****************************************************************************
* WriteData_0108 - д���ݵ�KS0108
* DESCRIPTION: -
* ���������ӹ�Ƭѡ�ź�
* @Param Data:д�������
* @Return :
* ----
*****************************************************************************
*/
void WriteData_0108(INT8U Data)
{
	//PUSH(IE);//����IE
    //EA = 0 ;//�ر��ж� ��ֹ�������

    LCD_E_L();
	SomeNop();
    LCD_RS_H();
	SomeNop();
    LCD_RW_L();
	SomeNop();
    LCD_Bus = (INT16U) Data;
    SomeNop();
    LCD_E_H();
    SomeNop();
    LCD_E_L();
	SomeNop();
	
    //POP(IE);//���»ָ�IE	

}
/*-------------------��LCD����----------------------------*/
#if 0
INT8U ReadData_0108(void)
{   INT8U Temp;
    LCD_Bus = (INT16U) 0xFF;//�л�������
    LCD_RS_H();
    LCD_RW_H();
    SomeNop();
    LCD_E_H();
    SomeNop();
    Temp = LCD_Bus;
    return(Temp);

}

INT8U ReadState_0108(void)
{
    INT8U Temp;
    LCD_Bus = (INT16U) 0xFF;
    LCD_RS_L();
    LCD_RW_H();
    SomeNop();
    LCD_E_H();
    SomeNop();
    LCD_E_L();
    Temp = LCD_Bus;
    return (Temp);


}

INT8U Busy(void)
{
    INT16U TimeOut;
    INT8U Flag;
    TimeOut = 20;
    do{
        Flag = ReadState_0108();
        if (TimeOut-- == 0){
            return 0;
        }
    }while(Flag &0x80);
    return 1;
}
#endif
//Option = 1   --->Disp On
//Option = 0   --->Disp Off
void KS0108_DisplayOn(INT8U Option)
{
    if (Option == 1) {
        WriteCommand_0108(Ks0108_CMD_Disp_ON);
    }else{
        WriteCommand_0108(Ks0108_CMD_Disp_OFF);
    }
}

void KS0108_SetStartRow(INT8U Row)
{
    WriteCommand_0108(Ks0108_CMD_Disp_Start_Line + Row);
}
/*
*****************************************************************************
* Display_Locate - LCD��ָ��λ�� ��ʾָ������
* DESCRIPTION: -
*
* @Param x0:0--191��������
* @Param y0:0--7  ��������
* @Param DisplayData:д�������
* @Return :
* ----
*****************************************************************************
*/
void Display_Locate(INT8U DisplayData, INT8U x0, INT8U y0)
{

	#ifdef _DEBUG_
		Uart_PutString("Enter Display_Locate()\r\n");
	#endif

    if ((x0 <= (LCD_XSIZE-1))&(y0 <= (LCD_YSIZE-1))){
        if (x0 <= 63){                                //0--191
            LCD_CSA();
//           Busy();//��æ
            WriteCommand_0108(Ks0108_CMD_Set_Addr + x0);
        } else if (x0 <= 127){
            LCD_CSB();
//           Busy();//��æ
            WriteCommand_0108(Ks0108_CMD_Set_Addr + x0 - 64);
        } 
//       Busy();//��æ
        WriteCommand_0108(y0 + Ks0108_CMD_Set_Page);   //0--7
//        Busy();//��æ
        WriteData_0108(DisplayData);
        LCD_CS_None();//����Ƭѡ�κ�һ��Ks0108
    }

   #ifdef _DEBUG_
		Uart_PutString("Exit Display_Locate()\r\n");
    #endif
}


/*
*****************************************************************************
* Display_Clear - ���LCD��ʾ
* DESCRIPTION: -
*
* @Param void:
* @Return :
* ----
*****************************************************************************
*/
void Display_Clear(void)
{
    INT8U  j,k;

    
	#ifdef _DEBUG_
		Uart_PutString("Enter Display_Clear()\r\n");
	#endif
//--------------------------------------
    LCD_CSA();
    for(k = 0; k < 8; k++){
        WriteCommand_0108(0 + Ks0108_CMD_Set_Addr); //0--7
        WriteCommand_0108(k + Ks0108_CMD_Set_Page); //0--63
        for(j = 0;j < 64;j++){
            WriteData_0108(0x00);
        }
    }
    KS0108_SetStartRow(0);
//--------------------------------------
    LCD_CSB();
    for(k = 0; k < 8; k++){
        WriteCommand_0108(0 + Ks0108_CMD_Set_Addr); //0--7
        WriteCommand_0108(k + Ks0108_CMD_Set_Page); //0--63
        for(j = 0;j < 64;j++){
            WriteData_0108(0x00);
        }
    }
    KS0108_SetStartRow(0);
//-------------------------------------
    LCD_CS_None();

    #ifdef _DEBUG_
		Uart_PutString("Exit Display_Clear()\r\n");
    #endif
}


/*
*****************************************************************************
* Display_Init - ��ʼ��LCD
* DESCRIPTION: -
*
* @Param void:
* @Return :
* ----
*****************************************************************************
*/
void Display_Init(void)				
{
	LCD_GPIO_Config();     //���ö˿����״̬

	LCD_RST_L();		 //Ӳ����λ
   	SomeNop();
    SomeNop();
	LCD_RST_H();
	SomeNop();
    SomeNop();


//---------------------------
    LCD_CSB();
  //  KS0108_DisplayOn(0);
    KS0108_SetStartRow(0);
 //   KS0108_SetStartRow(0);
    KS0108_DisplayOn(1);
//---------------------------
    LCD_CSA();
//    KS0108_DisplayOn(0);
    KS0108_SetStartRow(0);
 //   KS0108_SetStartRow(0);
    KS0108_DisplayOn(1);
//---------------------------
    LCD_CS_None();
}


/*
*****************************************************************************
* Display_Chinese - ָ��λ����ʾһ��16x16�ĺ���
* DESCRIPTION: -
*
* @Param x0:X������0--(192-16)
* @Param y0:Y������0--(8-2)
* @Param UniCode:��������
* @Return :
* ----
*****************************************************************************
*/
void Display_Chinese(INT16U UniCode, INT8U x0, INT8U y0)
{
  INT16U  CodeID;
  INT8U   i, ImgData;
  INT8U   HZ_X = x0;

  // ���ֿ�����������
    for(CodeID=0; CodeID < hzNum; CodeID++){
        // �ҵ� ָ�����ֵ� Index ��, ����ѭ��
        if ( (hzIndex[2*CodeID] == UniCode/256) && (hzIndex[2*CodeID+1] == UniCode%256) ){
            break;
        }else if(CodeID == (hzNum-1)){//δ�������ĺ���,��ʾ��"##"��ʾ��
            GUI_DispCharAt('#',x0,y0);
            GUI_DispCharAt('#',x0+Char_XSIZE,y0);
            return;
        }
    }
//----------------------------------
    switch (GUITextMode) {
        case GUI_TEXTMODE_NORMAL :
//-------------------------------------------------------------------------
            // д���ֵ��ϰ벿��  (һ������ 32 ���ֽ�,  ��ʾΪ2�� * 16��)
            for(i=0; i<16; i++){
                ImgData = hzdot[i+CodeID*32];
                Display_Locate(ImgData, HZ_X, y0);
                HZ_X++;
            }
            // д���ֵ��°벿��
            HZ_X = x0;
            for(i=16; i<32; i++){
                ImgData = hzdot[i+CodeID*32];
                Display_Locate(ImgData, HZ_X, y0 + 1);
                HZ_X++;
            }
//-------------------------------------------------------------------------
             break;
        case GUI_TEXTMODE_REVERSE :
//-------------------------------------------------------------------------
            // д���ֵ��ϰ벿��  (һ������ 32 ���ֽ�,  ��ʾΪ2�� * 16��)
            for(i=0; i<16; i++){
              ImgData = hzdot[i+CodeID*32]^0xFF;
              Display_Locate(ImgData, HZ_X, y0);
              HZ_X++;
            }
            // д���ֵ��°벿��
            HZ_X = x0;
            for(i=16; i<32; i++){
              ImgData = hzdot[i+CodeID*32]^0xFF;
              Display_Locate(ImgData, HZ_X, y0 + 1);
              HZ_X++;
            }
//-------------------------------------------------------------------------
             break;
        case GUI_TEXTMODE_UNDERLINE :
//-------------------------------------------------------------------------
            // д���ֵ��ϰ벿��  (һ������ 32 ���ֽ�,  ��ʾΪ2�� * 16��)
            for(i=0; i<16; i++){
              ImgData = hzdot[i+CodeID*32];
              Display_Locate(ImgData, HZ_X, y0);
              HZ_X++;
            }
            // д���ֵ��°벿��
            HZ_X = x0;
            for(i=16; i<32; i++){
              ImgData = hzdot[i+CodeID*32]|0x80;
              Display_Locate(ImgData, HZ_X, y0 + 1);
              HZ_X++;
            }
//-------------------------------------------------------------------------
             break;
        default :
             break;
    }
}


/*
*****************************************************************************
* Display_ASCII - ��ʾһ��ASCII
* DESCRIPTION: -
*
* @Param X:X������ 0--192  (ע��߽�)
* @Param Y:Y������ 0-- 7   (ע��߽�)
* @Param Char:ASCII��
* @Return :
* ----
*****************************************************************************
*/
void Display_ASCII(INT8U Char, INT8U X, INT8U Y)
{
    INT8U  i, ImgData;
    INT8U  HZ_X = X;
    if(GUIEnFont == En_5x8){
        Display_Locate(0x00, HZ_X, Y);
        HZ_X++;
        for(i = 0; i < 5; i++){
            ImgData = chardot_5x8[i+(Char-0x20)*5];
            Display_Locate(ImgData, HZ_X, Y);
            HZ_X++;
        }
        Display_Locate(0x00, HZ_X, Y);
        HZ_X++;
        Display_Locate(0x00, HZ_X, Y);
    } else {
        for(i=0;i<8;i++){
            ImgData = chardot_8x16[i+(Char-0x20)*16];
            Display_Locate(ImgData, HZ_X, Y);
            HZ_X++;
        }
        HZ_X=X;
        for(i=8;i<16;i++){
            ImgData = chardot_8x16[i+(Char-0x20)*16];
            Display_Locate(ImgData, HZ_X, Y + 1);
            HZ_X++;
      }

   }
}




/*
*****************************************************************************
* Display_InvASCII - ����ʾASCII
* DESCRIPTION: -
*
* @Param X:X������  0--191
* @Param Y:Y������  0--7
* @Param Char:ASCII��
* @Return :
* ----
*****************************************************************************
*/
void Display_InvASCII(INT8U Char, INT8U X, INT8U Y)
{

    INT8U  i, ImgData;
    INT8U  HZ_X = X;
    if(GUIEnFont == En_5x8){
        Display_Locate(0xFF, HZ_X, Y);
        HZ_X++;
        for(i = 0; i < 5; i++){
            ImgData = chardot_5x8[i+(Char-0x20)*5]^0xFF;
            Display_Locate(ImgData, HZ_X, Y);
            HZ_X++;
        }
        Display_Locate(0xFF, HZ_X, Y);
        HZ_X++;
        Display_Locate(0xFF, HZ_X, Y);
    } else {
        for(i=0;i<8;i++){
            ImgData = chardot_8x16[i+(Char-0x20)*16] ^ 0xFF;
            Display_Locate(ImgData, HZ_X, Y);
            HZ_X++;
        }
        HZ_X=X;
        for(i=8;i<16;i++){
            ImgData = chardot_8x16[i+(Char-0x20)*16] ^ 0xFF;
            Display_Locate(ImgData, HZ_X, Y + 1);
            HZ_X++;
        }
    }
}

/*
*****************************************************************************
* Display_ASCII_UnderLine - ��ʾһ�������»��ߵ�ASCII
* DESCRIPTION: -
*
* @Param X:X������
* @Param Y:Y������
* @Param Char:ASCII��
* @Return :
* ----
*****************************************************************************
*/
void Display_ASCII_UnderLine(INT8U Char, INT8U X, INT8U Y)
{
  //INT16U Position;
  INT8U  i, ImgData;
  INT8U  HZ_X = X;
    if(GUIEnFont == En_5x8){
        Display_Locate(0x80, HZ_X, Y);
        HZ_X++;
        for(i = 0; i < 5; i++){
          ImgData = chardot_5x8[i+(Char-0x20)*5] | 0x80;
          Display_Locate(ImgData, HZ_X, Y);
          HZ_X++;
        }
        Display_Locate(0x80, HZ_X, Y);
        HZ_X++;
        Display_Locate(0x80, HZ_X, Y);
    } else {
        for(i=0;i<8;i++){
            ImgData = chardot_8x16[i+(Char-0x20)*16];
            Display_Locate(ImgData, HZ_X, Y);
            HZ_X++;
        }
        HZ_X=X;
        for(i=8;i<16;i++){
            ImgData = chardot_8x16[i+(Char-0x20)*16] | 0x80;
            Display_Locate(ImgData, HZ_X, Y + 1);
            HZ_X++;
        }
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#if 0
/*
----------------------------------------------------------------------------------------------
//�ƶ���ǰY����
----------------------------------------------------------------------------------------------
*/
void GUI_GotoY(INT8U y)
{
    if (y > 7) {
        PosY = 7;
    } else {
        PosY = y;
    }
}
/*
----------------------------------------------------------------------------------------------
//�ƶ���ǰY����
----------------------------------------------------------------------------------------------
*/
void GUI_GotoX(INT8U x)
{
    if (x > 127) {
        PosX = 127;
    } else {
        PosX = x;
    }
}
/*
*****************************************************************************
* GUI_GotoXY - ϵͳ����ƶ���ָ��λ��
* DESCRIPTION: -
*
* @Param y:Y������
* @Param x:X������
* @Return :
* ----
*****************************************************************************
*/
void GUI_GotoXY(INT8U x,INT8U y)
{
    GUI_GotoX(x);
    GUI_GotoY(y);
}

#endif
/*
*****************************************************************************
* GUI_SetTextMode - ����������ʾģʽ
* DESCRIPTION: -
*
* @Param TextMode:ģʽ����
* @Return :ģʽ����
* ----
*****************************************************************************
*/

INT8U GUI_SetTextMode(INT8U TextMode)
{
    GUITextMode = TextMode;
    return GUITextMode;
}

/*
*****************************************************************************
* GUI_SetEnFont - ������ʾ��Ӣ������
* DESCRIPTION: -
* Ŀǰ֧����������8x16 5x8
* @Param EnFont:�������
* @Return :
* ----
*****************************************************************************
*/
void GUI_SetEnFont(INT8U EnFont)
{
  if (EnFont == En_5x8){
    GUIEnFont = En_5x8;
    Char_XSIZE = En_5x8_XSIZE;
    Char_YSIZE = En_5x8_YSIZE;
  } else {
    GUIEnFont = En_8x16;
    Char_XSIZE = En_8x16_XSIZE;
    Char_YSIZE = En_8x16_YSIZE;

  }

}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void GUI_Delay(INT16U num)
{ 
	INT8U   i;
	INT16U  j;
	for(j=0;j<num;j++)
	{
		for(i=0;i<140;i++);
	}
}
/*
*****************************************************************************
* GUI_DispCharAt - ָ��λ����ʾһ��ASCII��
* DESCRIPTION: - ����ʹ����ϵͳ�趨������
*
* @Param x:X������
* @Param y:Y������
* @Param c:ASCII��
* @Return :
* ----
*****************************************************************************
*/
void GUI_DispCharAt(INT8U c, INT8U x, INT8U y)
{
    switch (GUITextMode) {
        case GUI_TEXTMODE_NORMAL :
             Display_ASCII(c, x, y);
             break;
        case GUI_TEXTMODE_REVERSE :
             Display_InvASCII(c, x, y);
             break;
        case GUI_TEXTMODE_UNDERLINE :
             Display_ASCII_UnderLine(c, x, y);
             break;
        default :
             break;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#if 0
void GUI_DispChar(INT8U c)
{
    GUI_DispCharAt(c, PosX, PosY);
    PosX += 8;
}
#endif
/*
*****************************************************************************
* GUI_Init - ��ʼ��GUI��ʾ
* DESCRIPTION: -
*
* @Param void:
* @Return :
* ----
*****************************************************************************
*/
void GUI_Init(void)
{
	#ifdef _DEBUG_
		Uart_PutString("Enter GUI_Init()\r\n");
	#endif

    Display_Init();
    GUIEnFont = En_8x16;
    GUITextMode = GUI_TEXTMODE_NORMAL;
    Char_XSIZE = En_8x16_XSIZE;
    Char_YSIZE = En_8x16_YSIZE;
    PosX = 0;
    PosY = 0;
    NoXY = true;

    #ifdef _DEBUG_
	Uart_PutString("Exit GUI_Init()\r\n");
	#endif


}
void GUI_Clear(void)
{
    Display_Clear();
}
#if 0
void GUI_DispCEOL(void)
{
    INT8U  i;
    for(i = PosX; i <= LCD_XSIZE-8; i += 8) {
        GUI_DispCharAt(' ', i, PosY);

    }
}
#endif
/*
*****************************************************************************
* GUI_DispStringAt - ָ��λ����ʾһ���ַ���
* DESCRIPTION: -
* ֧����Ӣ��� ����Ӣ���ʱӢ������ǿ��Ϊ8x16 �������Ӣ�� �����趨����
* @Param x0:X������
* @Param y0:Y������
* @Param s:�ַ���
* @Return :
* ----
*****************************************************************************
*/
//void GUI_DispStringAt(INT8U  *s,INT8U x0,INT8U y0)
void GUI_DispStringAt(INT8U  *s,INT8U x0,INT8U y0)
{
    INT8U  i;
    if (NoXY == false){//û���ṩ����,��ʹ��ϵͳ����
        x0 = PosX;
        y0 = PosY;
    }
    i = 0;
    while(s[i]!='\0'){
        if (s[i] < 0x80) {//��ASCII
             if(s[i] == '\n'){
                    x0 = 0;
                    y0 += Char_YSIZE;
             } else {
                if (x0 > (LCD_XSIZE-1)) {//����
                    x0 = 0;
                    y0 += Char_YSIZE;
                }
                switch (GUITextMode) {
                    case GUI_TEXTMODE_NORMAL :
                         Display_ASCII(s[i], x0, y0);
                         break;
                    case GUI_TEXTMODE_REVERSE :
                         Display_InvASCII(s[i], x0, y0);
                         break;
                    case GUI_TEXTMODE_UNDERLINE :
                         Display_ASCII_UnderLine(s[i], x0, y0);
                         break;
                    default :
                         break;
                }
                x0 += Char_XSIZE;
             }
        } else {//������
            if (x0 > LCD_XSIZE-16){
                x0 = 0;
                y0 += Chinese_YSIZE;
            }
            Display_Chinese(s[i]*256+s[i+1],x0,y0);//����
            i++;
            x0 += Chinese_XSIZE;
       }
       s++;
    }
    if (NoXY == false){
        PosX = x0;
        PosY = y0;
    }

}
#if 0
/*
*****************************************************************************
* GUI_DispString - �ڵ�ǰϵͳ��������ʾ�ַ���
* DESCRIPTION: -
* �������겻��ָ���� ���ܸ�GUI_DispStringAtһ��
* @Param s:�ַ���
* @Return :
* ----
*****************************************************************************
*/
void GUI_DispString(INT8U  *s)
{
    NoXY = false;
    GUI_DispStringAt(s,PosX,PosY);
    NoXY = true;
}

void GUI_DispStringLen(INT8U  *s, INT16U Len)
{
    INT8U  i;
    i = 0;
    while(s[i]!='\0'){
        if (i >= Len ){
            return;
        }
        if (s[i] < 0x80) {//��ASCII
             if(s[i] == '\n'){
//                    PosX = 0;
//                    PosY += 2;
             } else {
               if (PosX > (LCD_XSIZE-1)) {//����
                    PosX = 0;
//                    PosY += 2;
                }
                switch (GUITextMode) {
                    case GUI_TEXTMODE_NORMAL :
                         Display_ASCII(s[i], PosX, PosY);
                         break;
                    case GUI_TEXTMODE_REVERSE :
                         Display_InvASCII(s[i], PosX, PosY);
                         break;
                    case GUI_TEXTMODE_UNDERLINE :
                         Display_ASCII_UnderLine(s[i], PosX, PosY);
                         break;
                    default :
                         break;
                }
                PosX += 8;
             }
        } else {//������
            if (PosX > LCD_XSIZE-16){
//                PosX = 0;
//                PosY += 2;
                return;
            }
            Display_Chinese(s[i]*256+s[i+1],PosX,PosY);//����
            i++;
            PosX += 16;
       }
       i++;
    }

}

void GUI_DrawCursor(INT8U x, INT8U y)
{
    if(GUITextMode == GUI_TEXTMODE_NORMAL){
        Display_Locate(0xFF,x,y);
        Display_Locate(0xFF,x,y+1);
    } else if (GUITextMode == GUI_TEXTMODE_REVERSE){
        Display_Locate(0x00,x,y);
        Display_Locate(0x00,x,y+1);
    } else {
        Display_Locate(0xFF,x,y);
        Display_Locate(0xFF,x,y+1);
    }
}

void GUI_CursorOn(INT8U x, INT8U y)
{
    Display_Locate(0xFF,x,y);
    Display_Locate(0xFF,x,y+1);
}

void GUI_CursorOFF(INT8U x, INT8U y)
{
    Display_Locate(0x00,x,y);
    Display_Locate(0x00,x,y+1);
}
#endif

/*
*****************************************************************************
* GUI_DispStringAtBar - ��ʾһ���ַ�����һ��BAR��
* DESCRIPTION: -
* �����趨���뷽ʽ
* @Param s:�ַ���
* @Param x0:BAR��ʼX������
* @Param x1:BAR�������X������
* @Param y0:BAR��Y������
* @Param Mode:���뷽ʽ ������BAR �����BAR �Ҷ���BAR
* @Return :
* ----
*****************************************************************************
*/
//void GUI_DispStringAtBar(INT8U  *s,INT8U x0, INT8U y0, INT8U x1,INT8U Mode)
void GUI_DispStringAtBar(INT8U  *s,INT8U x0, INT8U y0, INT8U x1,INT8U Mode)
{
    INT8U  ENCount;//Ӣ��ͳ��
    INT8U  CNCount;//����ͳ��
    INT8U   XLen;
    INT8U  X;
    INT8U  Y;
    INT8U   i;
    CNCount = 0;
    ENCount = 0;
    XLen = 0;
    i = 0;
    while(s[i]!='\0'){//ͳ����Ӣ�ĸ���
        if (s[i] > 0x80) {
            CNCount ++;
            i++;
        } else {
           ENCount++;
        }
        i++;
    }
        GUI_SetEnFont(En_8x16);
/*
    if(CNCount){
        GUI_SetEnFont(En_8x16);
    } else {
        GUI_SetEnFont(En_5x8);
    }
*/
    XLen = ENCount*8 + CNCount*16;

    if (XLen > 127) {//����ֻ����ʾһ��
        XLen = 127;
    }
//ʶ����䷽ʽ����
    switch (Mode&0x30) {//ֻ��������Bit���������
        case GUI_TA_LEFT: //�����
            XLen = 0;
            break;
        case GUI_TA_HCENTER:  //����
            XLen  = ((x1-x0)-XLen)/2;
            break;
        case GUI_TA_RIGHT:   //�Ҷ���
            XLen  = (x1-x0)-XLen;
            break;
        default:
            break;
    }
//--------------------------------------
//�õ���ʾ���������
    X = x0+XLen;
    Y = y0;

//---------------------
//����ַ�ǰ��Ŀհײ���
    if(Mode&GUI_JUST_TEXT){
    } else {
        x0 = X;
        if (GUITextMode == GUI_TEXTMODE_REVERSE){
            for(i = 0; i <x0; i++){
                Display_Locate(0xFF, i,y0);
                Display_Locate(0xFF, i,y0+1);
            }
        }else{
            for(i = 0; i <x0; i++){
                Display_Locate(0x00, i,y0);
                Display_Locate(0x00, i,y0+1);
            }
        }
    }

//-----------------------
    i = 0;
    while(s[i]!='\0'){
        if (s[i] < 0x80) {//��ASCII
            switch (GUITextMode) {
                case GUI_TEXTMODE_NORMAL :
                     Display_ASCII(s[i], X, Y);
                     break;
                case GUI_TEXTMODE_REVERSE :
                     Display_InvASCII(s[i], X, Y);
                     break;
                case GUI_TEXTMODE_UNDERLINE :
                     Display_ASCII_UnderLine(s[i], X, Y);
                     break;
                default :
                     break;
            }
            X += 8;
        } else {//������
            Display_Chinese(s[i]*256+s[i+1],X,Y);//����
            i++;
            X += 16;
       }
//       s++;
        i++;
    }
//����ַ�����Ŀհײ���
    if(Mode&GUI_JUST_TEXT){
        GUI_SetEnFont(En_5x8);
        return;
    } else {
        x0 = X;
        if (GUITextMode == GUI_TEXTMODE_REVERSE){
            for(i = 0; i <x1-x0; i++){
                Display_Locate(0xFF, x0+i,y0);
                Display_Locate(0xFF, x0+i,y0+1);
            }
        }else{
            for(i = 0; i <x1-x0; i++){
                Display_Locate(0x00, x0+i,y0);
                Display_Locate(0x00, x0+i,y0+1);
            }
        }
        //GUI_SetEnFont(En_5x8);
        return;
    }
//---------------------------------------
}

#if 0
//--------------------------
//��ʾASCII(16X32)
//Char �ַ�
//X 0--6
//Y 0--(128-8)

INT16U  Changedot[] =
{
0x0000, 0x0003, 0x000c, 0x000f, 0x0030, 0x0033, 0x003c, 0x003f, 0x00c0, 0x00c3,
0x00cc, 0x00cf, 0x00f0, 0x00f3, 0x00fc, 0x00ff, 0x0300, 0x0303, 0x030c, 0x030f,
0x0330, 0x0333, 0x033c, 0x033f, 0x03c0, 0x03c3, 0x03cc, 0x03cf, 0x03f0, 0x03f3,
0x03fc, 0x03ff, 0x0c00, 0x0c03, 0x0c0c, 0x0c0f, 0x0c30, 0x0c33, 0x0c3c, 0x0c3f,
0x0cc0, 0x0cc3, 0x0ccc, 0x0ccf, 0x0cf0, 0x0cf3, 0x0cfc, 0x0cff, 0x0f00, 0x0f03,
0x0f0c, 0x0f0f, 0x0f30, 0x0f33, 0x0f3c, 0x0f3f, 0x0fc0, 0x0fc3, 0x0fcc, 0x0fcf,
0x0ff0, 0x0ff3, 0x0ffc, 0x0fff, 0x3000, 0x3003, 0x300c, 0x300f, 0x3030, 0x3033,
0x303c, 0x303f, 0x30c0, 0x30c3, 0x30cc, 0x30cf, 0x30f0, 0x30f3, 0x30fc, 0x30ff,
0x3300, 0x3303, 0x330c, 0x330f, 0x3330, 0x3333, 0x333c, 0x333f, 0x33c0, 0x33c3,
0x33cc, 0x33cf, 0x33f0, 0x33f3, 0x33fc, 0x33ff, 0x3c00, 0x3c03, 0x3c0c, 0x3c0f,
0x3c30, 0x3c33, 0x3c3c, 0x3c3f, 0x3cc0, 0x3cc3, 0x3ccc, 0x3ccf, 0x3cf0, 0x3cf3,
0x3cfc, 0x3cff, 0x3f00, 0x3f03, 0x3f0c, 0x3f0f, 0x3f30, 0x3f33, 0x3f3c, 0x3f3f,
0x3fc0, 0x3fc3, 0x3fcc, 0x3fcf, 0x3ff0, 0x3ff3, 0x3ffc, 0x3fff, 0xc000, 0xc003,
0xc00c, 0xc00f, 0xc030, 0xc033, 0xc03c, 0xc03f, 0xc0c0, 0xc0c3, 0xc0cc, 0xc0cf,
0xc0f0, 0xc0f3, 0xc0fc, 0xc0ff, 0xc300, 0xc303, 0xc30c, 0xc30f, 0xc330, 0xc333,
0xc33c, 0xc33f, 0xc3c0, 0xc3c3, 0xc3cc, 0xc3cf, 0xc3f0, 0xc3f3, 0xc3fc, 0xc3ff,
0xcc00, 0xcc03, 0xcc0c, 0xcc0f, 0xcc30, 0xcc33, 0xcc3c, 0xcc3f, 0xccc0, 0xccc3,
0xcccc, 0xcccf, 0xccf0, 0xccf3, 0xccfc, 0xccff, 0xcf00, 0xcf03, 0xcf0c, 0xcf0f,
0xcf30, 0xcf33, 0xcf3c, 0xcf3f, 0xcfc0, 0xcfc3, 0xcfcc, 0xcfcf, 0xcff0, 0xcff3,
0xcffc, 0xcfff, 0xf000, 0xf003, 0xf00c, 0xf00f, 0xf030, 0xf033, 0xf03c, 0xf03f,
0xf0c0, 0xf0c3, 0xf0cc, 0xf0cf, 0xf0f0, 0xf0f3, 0xf0fc, 0xf0ff, 0xf300, 0xf303,
0xf30c, 0xf30f, 0xf330, 0xf333, 0xf33c, 0xf33f, 0xf3c0, 0xf3c3, 0xf3cc, 0xf3cf,
0xf3f0, 0xf3f3, 0xf3fc, 0xf3ff, 0xfc00, 0xfc03, 0xfc0c, 0xfc0f, 0xfc30, 0xfc33,
0xfc3c, 0xfc3f, 0xfcc0, 0xfcc3, 0xfccc, 0xfccf, 0xfcf0, 0xfcf3, 0xfcfc, 0xfcff,
0xff00, 0xff03, 0xff0c, 0xff0f, 0xff30, 0xff33, 0xff3c, 0xff3f, 0xffc0, 0xffc3,
0xffcc, 0xffcf, 0xfff0, 0xfff3, 0xfffc, 0xffff,
};
bit EnFont = 0;
void GUI_DispChar(INT8U Char,INT8U X,INT8U Y,bit Nor)
{
    INT8U  i;
    INT8U  dot;

    INT16U  temp;

    if(Y>(128-8)){//����
        X += 2;
        Y  = 0;
    }
    if(!Nor){
        for(i=0;i<8;i++){
            if(EnFont == 0){
                WriteByte(X,(Y+i),chardot[i+(Char-0x20)*16]);
            }else{
                dot = chardot[i+(Char-0x20)*16];
                temp = Changedot[dot];
//��δ������������Ŵ�2��,�ⲿ����ʱ������Ҫ�ӱ�
                WriteByte(X,(Y+i),(INT8U)(temp&0x00FF));
                WriteByte(X+1,(Y+i),(INT8U)((temp&0xFF00)>>8));

/*
//��δ��������Ŵ�4��,�����ں����ᱻ�Ŵ�,����ⲿ���ú���ʱҲ��Ҫ�ӱ�
                WriteByte(X,(Y+i*2),(INT8U)(temp&0x00FF));
                WriteByte(X,(Y+i*2+1),(INT8U)(temp&0x00FF));
                WriteByte(X+1,(Y+i*2),(INT8U)((temp&0xFF00)>>8));
                WriteByte(X+1,(Y+i*2+1),(INT8U)((temp&0xFF00)>>8));
*/
            }

        }
        for(i=8;i<16;i++){
            if(EnFont == 0){
                WriteByte(X+1,(Y+i-8),chardot[i+(Char-0x20)*16]);
            }else{
                dot = chardot[i+(Char-0x20)*16];
                temp = Changedot[dot];

                WriteByte(X+2,(Y+i-8),(INT8U)(temp&0x00FF));
                WriteByte(X+3,(Y+i-8),(INT8U)((temp&0xFF00)>>8));

/*
                WriteByte(X+2,(Y+(i-8)*2),(INT8U)(temp&0x00FF));
                WriteByte(X+2,(Y+(i-8)*2+1),(INT8U)(temp&0x00FF));
                WriteByte(X+3,(Y+(i-8)*2),(INT8U)((temp&0xFF00)>>8));
                WriteByte(X+3,(Y+(i-8)*2+1),(INT8U)((temp&0xFF00)>>8));
*/
            }
        }
    }else{
        for(i=0;i<8;i++){
            if(EnFont == 0){
                WriteByte(X,(Y+i),0xFF-chardot[i+(Char-0x20)*16]);
            }else{
                dot = 0xFF-chardot[i+(Char-0x20)*16];
                temp = Changedot[dot];
                WriteByte(X*2,(Y+i),(INT8U)(temp&0x00FF));
                WriteByte(X*2,(Y+i+1),(INT8U)(temp&0x00FF));
                WriteByte(X*2+1,(Y+i),(INT8U)((temp&0xFF00)>>8));
                WriteByte(X*2+1,(Y+i+1),(INT8U)((temp&0xFF00)>>8));

            }
        }
        for(i=8;i<16;i++){
            if(EnFont == 0){
                WriteByte(X+1,(Y+i-8),0xFF-chardot[i+(Char-0x20)*16]);
            }else{
                dot = 0xFF-chardot[i+(Char-0x20)*16];
                temp = Changedot[dot];
                WriteByte((X+1)*2,(Y+i-8+1),(INT8U)(temp&0x00FF));
                WriteByte((X+1)*2,(Y+i-8),(INT8U)(temp&0x00FF));
                WriteByte((X+1)*2+1,(Y+i-8+1),(INT8U)((temp&0xFF00)>>8));
                WriteByte((X+1)*2+1,(Y+i-8),(INT8U)((temp&0xFF00)>>8));
            }
        }
    }
}
#endif
//--------------------------

#if 0
//-------------��ʾ��ͨͼƬ
//------(ͼ�� ���� ��ʼλ��)
void Display_Image(INT8U  *IMG)
{
    INT8U   XAddr;
    INT8U   YAddr;
    INT16U  Count;
    Count = 0;
    for(XAddr=0;XAddr<8;XAddr++){//0--7
        for(YAddr=0;YAddr<128;YAddr++){//0--127
            WriteByte(XAddr,YAddr,IMG[Count++]);
        }
    }
}
#endif

/*
*****************************************************************************
* GUI_DrawIcon - �ƶ�λ����ʾһ��32x32��ͼ��
* DESCRIPTION: -
*
* @Param x0:X������
* @Param y0:Y������ ע��߽�!
* @Param Ico:ͼ������
* @Return :
* ----
*****************************************************************************
*/
void GUI_DrawIcon(INT8U   *Ico, INT8U x0, INT8U y0)
{
    INT8U   i;
    INT16U  Count;
    Count = 0;
    if( y0 > 4){//�߽籣��
        y0 = 4;
    }
    if(x0 >(LCD_XSIZE-32)){
        x0 = LCD_XSIZE-32;
    }
    for(i=0;i<32;i++){//��һ��
        Display_Locate(Ico[Count++],x0+i,y0);
    }
    for(i=0;i<32;i++){//�ڶ���
        Display_Locate(Ico[Count++],x0+i,y0+1);
    }
    for(i=0;i<32;i++){//������
        Display_Locate(Ico[Count++],x0+i,y0+2);
    }
    for(i=0;i<32;i++){//������
        Display_Locate(Ico[Count++],x0+i,y0+3);
    }
}
/*
*****************************************************************************
* GUI_DrawIcon - �ƶ�λ����ʾһ��16x16��ͼ��
* DESCRIPTION: -
*
* @Param x0:X������
* @Param y0:Y������ ע��߽�!
* @Param Ico:ͼ������
* @Return :
* ----
*****************************************************************************
*/
void GUI_DrawIcon16X16(INT8U   *Ico, INT8U x0, INT8U y0)
{
    INT8U   i;
    INT16U  Count;
    Count = 0;
    if( y0 > 6){//�߽籣��
        y0 = 4;
    }
    if(x0 >(LCD_XSIZE-16)){
        x0 = LCD_XSIZE-16;
    }
    for(i=0;i<16;i++){//��һ��
        Display_Locate(Ico[Count++],x0+i,y0);
    }
    for(i=0;i<16;i++){//�ڶ���
        Display_Locate(Ico[Count++],x0+i,y0+1);
    }
}
#if 1

/*
*****************************************************************************
* GUI_DispDecAt - ��ʾʮ������ֵ
* DESCRIPTION: -
* �����������5������(��ΪINT16U--->65536)
* @Param v:��ʾ������
* @Param x:X������
* @Param y:Y������  XY �������λ������ Ҳ������ֵ���λ������
* @Param Len:ָ������ʾ����1--5��
* @Return :
*
*****************************************************************************
*/
void GUI_DispDecAt(INT16U v, INT16U x, INT16U y, INT8U Len)
{
    INT8U  i;
    INT8U  CharBuf[5];
    INT8U  HighByte;

    HighByte = 0;
    for(i = 0; i < 5; i++){
        CharBuf[i] = (INT8U)(v%10);
        v = v/10;
        if(CharBuf[i]){
            HighByte = i;
        }
    }
    //��0λ�������Ҳ��ʾ
    i = 0;
    GUI_DispCharAt(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
    for(i = 1; i < Len; i++){
        if(CharBuf[i]){
            GUI_DispCharAt(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
        }else if(i > HighByte){
            GUI_DispCharAt(' ',x+((Len-1)-i)*Char_XSIZE,y);
        }
    }

}

/*
*****************************************************************************
* GUI_DispHexAt - ��ʾһ�����ݵ�ʮ������ֵ
* DESCRIPTION: -
* ��󳤶�4��
* @Param v:����
* @Param x:X������
* @Param y:Y������  XY����������� Ҳ������������ֽ�����
* @Param Len:����1--4
* @Return :
*
*****************************************************************************
*/
void GUI_DispHexAt(INT32U v, INT8U x, INT8U y, INT8U Len)
{
    INT8U  i;
    INT8U  HexData;
    if(Len > 8){//���Ʒ�Χ
        Len = 8;
    }
    for(i = 0; i < Len; i++){
        HexData = v&0x0F;
        v = v >>4;
        if(HexData < 0x0A){
            GUI_DispCharAt(HexData+'0',x+Char_XSIZE*(Len-1-i),y);
        }else{
            GUI_DispCharAt(HexData-0x0A+'A',x+Char_XSIZE*(Len-1-i),y);
        }
    }
}
/*
*****************************************************************************
* HBar - ��ʾһ��ˮƽ�Ľ�����
* DESCRIPTION: -
* �����аٷֱ���ʾ
* @Param x0:���������X������   0-->127
* @Param x1:������������X����   0-->127  �������x0  �ٷֱ���ʾ�ڸ�����֮��
* @Param y:������Y������        0--7
* @Param percent:��ǰ�ٷ�ֵ     0-->100
* @Return :
*
*****************************************************************************
*/
void HBar(INT8U y, INT8U x0, INT8U x1,INT8U percent)
{
    INT8U  U8Temp;
    INT8U  i;
    float  Center;
    Center = (x1-x0);
    Center *= percent;
    Center /= 100;
//    U8Temp = (x1-x0)*percent/100;//�������������430�����ã���C51���ƺ������ø�����
    U8Temp = (INT8U)Center;
    Display_Locate(0xFF, x0, y);
    Display_Locate(0xFF, x1, y);
    for(i = 1; i < U8Temp; i++){
        Display_Locate(0xBD, x0+i, y);
    }
    for(i = x0+U8Temp+1; i < x1; i++){
        Display_Locate(0x81, i, y);
    }
}
/*                  x1 +3
|-------------------|
|                   ||
|                   |||
|                   ||||
|                   ||||
--------------------
 -------------------
  ------------------
x0--->x1+3
y0--->y1


*/

void TipDisp( INT8U x0, INT8U y0, INT8U x1, INT8U y1)
{
    INT8U  i;
    for(i = 0; i < x1-x0+4; i++){
        Display_Locate(0x01, x0+i, y0);
        Display_Locate(0x0F, x0+i, y1);
    }
    Display_Locate(0x01, x0+0, y1);
    Display_Locate(0x01, x0+1, y1);
    Display_Locate(0x03, x0+2, y1);
    Display_Locate(0x03, x0+3, y1);
    Display_Locate(0x07, x0+4, y1);
    Display_Locate(0x07, x0+5, y1);

    for(i = 0; i < y1-y0; i++){
        Display_Locate(0xFF, x0, y0+i);
        Display_Locate(0xFF, x1, y0+i);
        Display_Locate(0xFF, x1+1, y0+i);
        Display_Locate(0xFF, x1+2, y0+i);
        Display_Locate(0xFF, x1+3, y0+i);
    }
    Display_Locate(0xFC, x1+1, y0);
    Display_Locate(0xF0, x1+2, y0);
    Display_Locate(0xC0, x1+3, y0);
}

/*
���Tip
����Ӧ�ø�TipDispһ��
*/
void TipClr( INT8U x0, INT8U y0, INT8U x1, INT8U y1)
{
    INT8U  i;
    INT8U  j;
    for(i = 0; i <= x1+3-x0; i++){
        for(j = 0; j <= y1-y0; j++){
            Display_Locate(0x00, x0+i, y0+j);
        }
    }

}
#endif
/*
// ---- ��ʾ�������ŵ�����  (���� ��ʼλ��XY,��ʾ����) -----------------------------
void Display_Number(INT16U Number, INT8U X, INT8U Y, INT8U Lenth)
{
  INT8U DispNum;

  X = ( X + Lenth * 8 - 8 );
  for(; Lenth>0; Lenth--)
  {
    DispNum = Number%10 + 0x30;
    Display_ASCII(DispNum, X, Y);
    X -= 8;
    Number = Number / 10;
  }
}

// ---- ��ʾ�����ŵ�����  (���� ��ʼλ��XY,��ʾ����) ---------------------------------
void Display_SignedNumber(int Number,INT8U X,INT16U Y,INT8U Lenth)
{
  if(Number < 0)
  {
    Display_ASCII('-', X, Y);
    Display_Number(-Number, X+8, Y, Lenth);
  }
  else
  {
    Display_ASCII(' ', X, Y);
    Display_Number(Number, X+8, Y, Lenth);
  }
}

// ---- ��ʾ�������ŵ�С�� (���� ��ʼλ��XY,����λ��,С��λ��) ------------------------------
void Display_Decimal(unsigned long int Number, char X, INT16U Y, INT8U INT, INT8U DEC)
{
  INT8U DispNum, Lenth;
  //Y = Y +(( X + INT * 8 + DEC * 8 ) / 84) * 2;
  X = ( X + ( INT + DEC ) *8);
  // ��ʾС������
  for(Lenth=DEC; Lenth>0; Lenth--)
  {
    DispNum = Number%10 + 0x30;
    Display_ASCII(DispNum, X, Y);
    //if (X < 8) {Y -= 2; X += 84;}
    X -= 8;
    Number = Number / 10;
  }
  // ��ʾС����
  Display_ASCII('.', X, Y);
  //if (X < 8) {Y -= 2; X += 84;}
    X -= 8;
  // ��ʾ��������
  for(Lenth=INT; Lenth>0; Lenth--)
  {
    DispNum = Number%10 + 0x30;
    Display_ASCII(DispNum, X, Y);
    //if (X < 8) {Y -= 2; X += 84;}
    X -= 8;
    Number = Number / 10;
  }
}

// ---- ��ʾ�����ŵ�С�� (���� ��ʼλ��XY,����λ��,С��λ��) ------------------------------
void Display_SignedDecimal(long int Number, char X, INT16U Y, INT8U INT, INT8U DEC)
{
  if(Number < 0)
  {
    Display_ASCII('-', X, Y);
    Display_Decimal(-Number, X+8, Y, INT, DEC);
  }
  else
  {
  Display_ASCII(' ',X,Y);
  Display_Decimal(Number, X+8, Y, INT, DEC);
  }
}
*/


//--------------
/*
Bar���㷨
    ___
 |   |
 |   |
| |<-|-----BarLen
 |   L
 |   |
 |   |
 |  _|_
Bar�Ļ���������L-BarLen
Ϊ������,�����ڿ�ʼ�ͽ�β���ֶ�������һЩ��,��ô��������Ҫ�۳���Щ��ĳ���,���ڼ�����
�õ�0��ʱ��,�������϶�Ҫ�������ĵ�BarRemainDot
2����ʾ��ʽ:
һ����BarLen�Ƕ�����,
һ��BarLen�Ǹ�����ʾ�ܹ���������������
*/

#if 0
//--------------
//Bar�ĳ���
//Ԥ�������ĵ�
#define BarRemainDot  3
//������ʾλ��
//#define BarNumPosX (128-8+2)
#define BarNumPosY (7)
//Bar����ʾ��ʼ/����λ��
//#define BarBeginPosX (126)
#define BarBeginPosY (0*8)
#define BarEndPosX (126)
#define BarEndPosY (6*8)

INT8U  BarCode0[]={0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00};
INT8U  BarCode1[]={0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
extern INT8U 	ItemBackup_i;
extern INT8U 	ItemBackup[];

void Bar(INT8U Item_,INT8U ItemNum_,INT8U BarPosX,INT8U BarNumPosX)
{
    INT8U U8_temp;
    INT8U  DispFlag;
    INT8U  YOffset;
    INT16U  Temp;
    INT8U  BarLen;
    INT8U  Y;
    INT8U  i;
//    INT8U CharBuf[5];
//    Bool HighBit;

    BarLen = (BarEndPosY-BarBeginPosY-BarRemainDot)/(ItemNum_);//BarLen����ItemNum_�õ�
    if (BarLen == 0) {
        BarLen = 5;
    }
    BarLen = 8;
    Temp = Item_*(BarEndPosY-BarBeginPosY-BarLen-BarRemainDot);//BarRemainDot�Ǳ��۳��Ĳ���
    Temp = Temp/(ItemNum_-1);
    YOffset = (INT8U)Temp;
    if(!Temp){//���ˣ���Ԥ���ļ���
        YOffset = BarRemainDot;
    }

    for(Y = 0;Y < BarEndPosY/8;Y++){
        if((Y != (YOffset/8))&&(Y != (YOffset/8+1))){
            Display_Locate(0x00,BarPosX,Y);//��� X=125 ��
            Display_Locate(0xFF,BarPosX+1,Y);//X=126�л���
            Display_Locate(0x00,BarPosX+2,Y);//��� X=127 ��
        }else{//Y = YOffset/8 Y = YOffset/8��1
            Display_Locate(BarCode0[YOffset%8],BarPosX,(YOffset/8));
            Display_Locate(0xFF-BarCode0[YOffset%8],BarPosX+1,(YOffset/8));
            Display_Locate(BarCode0[YOffset%8],BarPosX+2,(YOffset/8));
            if((YOffset/8)+1 < (BarEndPosY/8)){//��ֹ��Խ��
                Display_Locate(BarCode1[YOffset%8],BarPosX,(YOffset/8+1));
                Display_Locate(0xFF-BarCode1[YOffset%8],BarPosX+1,(YOffset/8+1));
                Display_Locate(BarCode1[YOffset%8],BarPosX+2,(YOffset/8+1));
            }
        }
    }

    GUI_SetEnFont(En_5x8);
    Item_ += 1;
    //��ʾBar����
/*
    for(i = 0; i < 5; i++){
        CharBuf[i] = (INT8U)(Item%10);
        Item = Item/10;
    }
    HighBit = false;
    for(i = 0; i < 5; i++){
        if(CharBuf[4-i]){//�����λ��ʼ��ʾ
            GUI_DispCharAt(CharBuf[4-i]+'0',x+8*i,y);
            HighBit = true;
        }else{
            if(HighBit == true){
                GUI_DispCharAt('0',x+Char_XSIZE*i,y);//�����λ��Ϊ0,��ǰֵΪ0 ,��ʾ0
            }
        }
    }
*/
    DispFlag = false;
    U8_temp = (INT8U)(Item_/100);//  ��λ
    if(U8_temp){
        GUI_DispCharAt(U8_temp+'0',BarNumPosX-12,BarNumPosY);
        DispFlag = true;//֪ͨ��λ��ʾ
    }else{
        GUI_DispCharAt(' ',BarNumPosX-12,BarNumPosY);
    }

    Item_ = (Item_-U8_temp*100);//�޳���λ
    U8_temp = (INT8U)(Item_/10);//  ʮλ
    if(U8_temp||(DispFlag == true)){//��λ��Ϊ0�����߸�λ�Ѿ���ʾ����ô������ʾ
        GUI_DispCharAt(U8_temp+'0',BarNumPosX-6,BarNumPosY);
        DispFlag = 1;
    }else{
        GUI_DispCharAt(' ',BarNumPosX-6,BarNumPosY);
    }

    U8_temp = (INT8U)(Item_%10);//  ��λ
    GUI_DispCharAt(U8_temp+'0',BarNumPosX,BarNumPosY);

//----------------------------------
//��ʾ��ʷ������
    if(ItemBackup_i > 1){//����1����
        for(i = 0; i <ItemBackup_i-1;i++){//���һ��λ��1��λ��
            Item_ = ItemBackup[ItemBackup_i-1-i]+1;  //�ӱ��������еõ���ţ�Ȼ���1��ʾ
            U8_temp = (INT8U)(Item_%10);//
            GUI_DispCharAt(U8_temp+'0',BarNumPosX-8*(i+1),BarNumPosY);
            Display_Locate(0x10, BarNumPosX-8*(i+1)+8-1, BarNumPosY); //��ָ���
            Display_Locate(0x10, BarNumPosX-8*(i+1)+8-0, BarNumPosY);
        }
    }
//----------------------------------
    GUI_SetEnFont(En_8x16);

   return;

}
 #endif

//================================================================================================= 
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//================================================================================================= 