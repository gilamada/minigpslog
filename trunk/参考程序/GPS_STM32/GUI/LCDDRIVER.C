//================================================================================================
// 文件名称: LCDDRIVE.C
// 芯片类型: STM32F103VBT6                                           
// 开发软件: Keil, Windows XP2 
// 开发时间: 2010年4月17日                                                               
// 程序功能: LCM点阵驱动程序,LCM为深圳市欣瑞达电子有限公司的XRD160160D
// 编写人员: 崔超                                                                       
// 联系方式: QQ:52020081 MSN:cui_chao@msn.com Email:cuichao@xrd-lcd.com 
// 说明：图形液晶采用UC1698为LCD控制芯片，内带负压产生器，单3.3伏供电，
// 并行接口(使用stm32f103rbt6驱动)。
// 硬件连接： D0--D7   <===>  GPIOA0--GPIOA7
//            /WR      <===>  GPIOB1
//            /RD      <===>  GPIOB6
//            /CS      <===>  GPIOB7
//            C/D      <===>  GPIOB8
//            /RST     <===>  GPIOB0 
//================================================================================================ 
#include "config.h"		 //头文件配置
/*
//========================================== 宏定义 ==============================================
// 定义LCM接口控制
#define UC1698U_Bus        	GPIOC->ODR	
#define UC1698U_RST_H()    {GPIOB->BSRR = GPIO_Pin_15;}
#define UC1698U_RST_L()    {GPIOB->BRR  = GPIO_Pin_15;}
#define UC1698U_WR_H()     {GPIOB->BSRR = GPIO_Pin_14;}                          
#define UC1698U_WR_L()     {GPIOB->BRR  = GPIO_Pin_14;}
#define UC1698U_RD_H()     {GPIOB->BSRR = GPIO_Pin_13;}                                      
#define UC1698U_RD_L()     {GPIOB->BRR  = GPIO_Pin_13;} 
#define UC1698U_CS_H()     {GPIOB->BSRR = GPIO_Pin_12;}
#define UC1698U_CS_L()     {GPIOB->BRR  = GPIO_Pin_12;}
#define UC1698U_CD_H()     {GPIOB->BSRR = GPIO_Pin_10;}
#define UC1698U_CD_L()     {GPIOB->BRR  = GPIO_Pin_10;} 
#define LCD_BL_ON()        {GPIOB->BSRR = GPIO_Pin_11;}                                  
#define LCD_BL_OFF()       {GPIOB->BRR  = GPIO_Pin_11;} 	*/

//======================================== 变量定义 ===============================================
TCOLOR  gui_disp_buf[GUI_LCM_YMAX][GUI_LCM_XMAX/8];	// 声明GUI显示缓冲区

INT8U const  DCB_HEX_TAB[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
/*
//======================================== 函数 ===============================================
//============================================================================= 
//函 数 名:  LCD_GPIO_Config() 
//功 能：	配置MCU的LCM显示接口，不通的MCU要更改此处配置
//入口参数： 无
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void LCD_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable GPIOC lcd data clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  0x000000ff;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
   // Enable GPIOA  clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  0x0000ff00;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//============================================================================= 
//函 数 名:  WriteCommandUC1698U() 
//功 能：	写命令到UC1698U
//入口参数： CmdData:命令码
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void WriteCommandUC1698U(INT8U CmdData)
{
	//StatusCheckUC1698U();
    UC1698U_CD_L();
	UC1698U_RD_H();		
    UC1698U_CS_L();	
    UC1698U_Bus =(INT16U) CmdData;     
    UC1698U_WR_L();    
    UC1698U_WR_H();		
    UC1698U_CS_H();	
}
//============================================================================= 
//函 数 名:  WriteDataUC1698U() 
//功 能：	写数据到UC1698U
//入口参数： Data:写入的数据
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void WriteDataUC1698U(INT8U Data)
{
    //StatusCheckUC1698U();
	UC1698U_CD_H();
	UC1698U_RD_H();	
    UC1698U_CS_L();	
    UC1698U_Bus =  (INT16U) Data;    
    UC1698U_WR_L();	 	
    UC1698U_WR_H();	
    UC1698U_CS_H();	 	

} 

*/
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR

#define WriteCommandUC1698U(CmdData)  *(__IO uint16_t *) (Bank1_LCD_C)=(INT16U)CmdData 
#define WriteDataUC1698U(Data)     *(__IO uint16_t *) (Bank1_LCD_D)=(INT16U)Data 
//写16位数据函数
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}
//写寄存器地址函数
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

} 

#define nCS     GPIO_Pin_0
#define RS      GPIO_Pin_1
#define nWR     GPIO_Pin_2
#define nRD     GPIO_Pin_3
#define nReset  GPIO_Pin_3
//#define LCD_BK  GPIO_Pin_0
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void    LCD_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 
                         	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		 //LCD 背光控制
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  #ifndef BACKLIGTH_PWM
  GPIO_Init(GPIOD, &GPIO_InitStructure);					 
  #endif 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; 	 //LCD-RST
  GPIO_Init(GPIOD, &GPIO_InitStructure);  	
  
   /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */ 	
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 


 /* NE1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* RS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

  
  GPIO_SetBits(GPIOD, GPIO_Pin_7);			//CS=1 
  GPIO_SetBits(GPIOD, GPIO_Pin_14| GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1);  	 
  GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);   
  GPIO_ResetBits(GPIOE, GPIO_Pin_0);
  GPIO_ResetBits(GPIOD, GPIO_Pin_3);			//RESET=0
  GPIO_SetBits(GPIOD, GPIO_Pin_4);		    //RD=1
  GPIO_SetBits(GPIOD, GPIO_Pin_5);			//WR=1	  

  
}
/**
  * @brief  Configures the FSMC and GPIOs to interface with the SRAM memory.
  *         This function must be called before any write/read operation
  *         on the SRAM.
  * @param  None 
  * @retval : None
  */
void FSMC_LCD_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;


  p.FSMC_AddressSetupTime = 0x02;
  p.FSMC_AddressHoldTime = 0x00;
  p.FSMC_DataSetupTime = 0x05;
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_B;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  

  //打开FSMC时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
}





//============================================================================= 
//函 数 名:  LCD_Initialize() 
//功 能：	 LCM初始化
//入口参数： 无
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void  LCD_Initialize(void)
{

#define BR		0X01	   	//Set LCD Bias Ratio:1/10 bias	  
#define PC1     0X03        //power control set as internal power
#define TC      0x00        //set temperate compensation as 0%
#define PM      172         //Set Vbias Potentiometer       
#define LC_210  0X05        //set LCD Control
#define LC_43   0x03
#define LC_5    0X01
#define LC_76   0x01
#define LC_8    0X00
#define NIV     0X10        //n-line inversion
#define CSF     0X02        //23:enable FRC,PWM,LRM sequence      
#define WPC0    0x25        //Starting Column Address
#define WPP0    0           //Starting Row Address
#define WPC1	0x5A		//Ending Column Address
#define WPP1	159		    //Ending Row Address
#define	AC    	0x01		//Address  Control	  
#define CEN 	159 		//COM scanning end (last COM with full line cycle,0 based index)

	
	LCD_GPIO_Configuration();// 配置LCM接口
    FSMC_LCD_Init();         //初始化FSMC 

	GPIO_ResetBits(GPIOD, GPIO_Pin_3);  //硬件复位
   	Delay_ms(10); 					   
    GPIO_SetBits(GPIOD, GPIO_Pin_3 );	
	Delay_ms(100);

	WriteCommandUC1698U(0xE8|(BR&0X03));			
	WriteCommandUC1698U(0x28|(PC1&0X03));		
	WriteCommandUC1698U(0x24|(TC&0x03));		
	WriteCommandUC1698U(0x81);		
	WriteCommandUC1698U(PM );  	
	WriteCommandUC1698U(0xc0|(LC_210&0X07) ); 				
	WriteCommandUC1698U(0xa0|(LC_43 &0X03) ); 					
	WriteCommandUC1698U(0xd0|(LC_5  &0X01) ); 				
	WriteCommandUC1698U(0xd4|(LC_76 &0X03) );				
	WriteCommandUC1698U(0x84|(LC_8  &0X01) );		
	WriteCommandUC1698U(0xc8 );				 			
	WriteCommandUC1698U( NIV );				 		
	WriteCommandUC1698U(0xd8|(CSF&0X07) );	 		
	WriteCommandUC1698U(0xf4 );				 	
	WriteCommandUC1698U(WPC0 );				 		
	WriteCommandUC1698U(0xf6 );				 		
	WriteCommandUC1698U(WPC1 );				 		
	WriteCommandUC1698U(0xf5 );				 	 	
	WriteCommandUC1698U(WPP0 );				 		
	WriteCommandUC1698U(0xf7 );				 	 	
	WriteCommandUC1698U(WPP1 ); 			 	 	
	WriteCommandUC1698U(0xf8|((AC>>3)&0x01) );	 										  
	WriteCommandUC1698U(0x88| (0x07&AC) );	 	
	WriteCommandUC1698U(0xf1 );			
	WriteCommandUC1698U(CEN );		 	
	WriteCommandUC1698U(0xad );		//18:display on,select on/off mode.Green Enhance mode disable 
			   			
} 
void LCD_SetWindowProgram(INT8U x1,INT8U y1,INT8U x2,INT8U y2)  
{
 	WriteCommandUC1698U(0xf4 );				 	
	WriteCommandUC1698U(0x25+x1/3 );				 		
	WriteCommandUC1698U(0xf6 );				 		
	WriteCommandUC1698U(0x25+(x2+2)/3);				 		
	WriteCommandUC1698U(0xf5 );				 	 	
	WriteCommandUC1698U(y1);				 		
	WriteCommandUC1698U(0xf7 );				 	 	
	WriteCommandUC1698U(y2 ); 
}
void LCD_SetAddress(INT8U x,INT8U y)
{  	
    x = 0x25+x/3;//计算出该点所在的列地址，注意该液晶每3个点共有一个地址
    WriteCommandUC1698U(0x60|(y&0x0f));		//写行地址低位 	
    WriteCommandUC1698U(0x70|(y>>4));		//写行地址高位	
    WriteCommandUC1698U(x&0x0f);		//写列地址低位
    WriteCommandUC1698U(0x10|(x>>4));	//写列地址高位
}
//============================================================================= 
//                   与LCM相关的GUI接口函数
//============================================================================= 
//============================================================================= 
//函 数 名: GUI_RefreshSCR() 
//功 能：	刷新全屏，将显示缓冲区全部数据送到模组显示
//入口参数： 无
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void GUI_RefreshSCR(void)
{

	INT8U  i,j;
	INT8U  TempData = 0;  	
	LCD_SetWindowProgram(0,0,159,159); 
	LCD_SetAddress(0,0)	; 
	for(i=0; i<GUI_LCM_YMAX; i++)		// 历遍所有列
   {  for(j=0; j<GUI_LCM_XMAX/8; j++)	// 历遍所有行
      { 
	   	TempData=0; 
		if(gui_disp_buf[i][j] & 0x80)
       		TempData=0xf0;
   		if(gui_disp_buf[i][j] & 0x40)
       		TempData|=0x0f;
   		WriteDataUC1698U(TempData);
		
		TempData=0; 
   		if(gui_disp_buf[i][j] & 0x20)
       		TempData=0xf0;
   		if(gui_disp_buf[i][j] & 0x10)
       		TempData|=0x0f;
   		WriteDataUC1698U(TempData);	  
		
		TempData=0; 
   		if(gui_disp_buf[i][j] & 0x08)
       		TempData=0xf0;
   		if(gui_disp_buf[i][j] & 0x04)
       		TempData|=0x0f;
   		WriteDataUC1698U(TempData);	
		
		TempData=0; 
   		if(gui_disp_buf[i][j] & 0x02)
       		TempData=0xf0;
   		if(gui_disp_buf[i][j] & 0x01)
       		TempData|=0x0f;
  		WriteDataUC1698U(TempData);	
	  }
	  WriteDataUC1698U(0x00);  //补全每行末尾的数据，使总点数能被三整除
   } 

}
//============================================================================= 
//函 数 名: GUI_FillSCR() 
//功 能：	全屏填充。直接使用数据填充显示缓冲区。
//入口参数： dat 填充的数据(对于黑白色LCM，为0的点灭，为1的点显示)
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void  GUI_FillSCR(INT8U dat)
{  INT8U  i, j;

   for(i=0; i<GUI_LCM_YMAX; i++)		// 历遍所有行
   {  for(j=0; j<GUI_LCM_XMAX/8; j++)	// 历遍所有行
      {  gui_disp_buf[i][j] = dat;		// 填充数据
	  }
   }
   GUI_RefreshSCR(); 
}
//============================================================================= 
//函 数 名: GUI_Refresh3Point()
//功 能：	横向刷新3个点
//入口参数： x		指定点所在列的位置
//           y		指定点所在行的位置 
//出口参数： 无
//返 回 值： 无
//备注    ： 列坐标以3个点为单位左对齐
//============================================================================= 
void GUI_Refresh3Point(INT8U x,INT8U y)
{
    INT8U DataTemp;

    x = (x/3)*3; //得到能被三整除的列坐标
    LCD_SetAddress(x,y)	; //设置显示RAM地址

    if(gui_disp_buf[y][x>>3]&DCB_HEX_TAB[x&0x07])  //获取第一个点的数据
        DataTemp = 0xf0;
    else 
        DataTemp = 0x00;
    x++;
    if(gui_disp_buf[y][x>>3]&DCB_HEX_TAB[x&0x07])  //获取第二个点的数据
        DataTemp |=0x0f;
    else 
        DataTemp &=0xf0;
    WriteDataUC1698U(DataTemp);  				   //发送数据

    x++;
    if(gui_disp_buf[y][x>>3]&DCB_HEX_TAB[x&0x07]) //获取第三个点的数据
        DataTemp = 0xf0;
    else 
        DataTemp = 0x00;	
    WriteDataUC1698U(DataTemp);  				   //发送数据	
}

//============================================================================= 
//函 数 名: GUI_RefreshBlock()
//功 能：	区域刷新显示
//入口参数： x1		区域左上角的x坐标值
//           y1		区域左上角的y坐标值
//           x2     区域右下角的x坐标值
//           y2     区域右下角的y坐标值
//出口参数： 无
//返 回 值： 0:范围超出显示区域 1：刷新成功
//============================================================================= 
INT8U GUI_RefreshBlock(INT8U x1,INT8U y1,INT8U x2,INT8U y2)
{ INT8U i,j;
    //参数过滤 
  if(x2>GUI_LCM_XMAX) return(0);
  if(y2>GUI_LCM_YMAX) return(0); 
	x2 += 2;
	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j+=3)
			GUI_Refresh3Point(j,i);	

   return(1);
}
//============================================================================= 
//函 数 名: GUI_ClearSCR() 
//功 能：	清屏
//入口参数： 无
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void  GUI_ClearSCR(void)
{  GUI_FillSCR(0x00);
}

//============================================================================= 
//函 数 名: GUI_Point() 
//功 能：	在指定位置上画点
//入口参数：  x		指定点所在列的位置
//            y		指定点所在行的位置
//            color	显示颜色(为0时灭，为1时显示)
//出口参数： 无
//返 回 值： 返回值为1时表示操作成功，为0时表示操作失败。(操作失败原因是指定地址超出有
//           效范围)
//说明    ： 使用此函数画点时，LCM显示和缓冲区数据同时改变，
//           因此液晶模组本身特性所致，此函数的效率较低，每次更新需要三个点同时更新
//============================================================================= 
INT8U  GUI_Point(INT8U x, INT8U y, TCOLOR color)
{ 
    INT8U DataTemp;
    //参数过滤 
    if(x>GUI_LCM_XMAX) return(0);
    if(y>GUI_LCM_YMAX) return(0);
   
    // 设置缓冲区相应的点
    if(color) gui_disp_buf[y][x>>3] |= DCB_HEX_TAB[x&0x07];
    else  gui_disp_buf[y][x>>3] &= (~DCB_HEX_TAB[x&0x07]); 
   
    x = (x/3)*3; //得到能被三整除的列坐标
	LCD_SetAddress(x,y)	; //设置显示RAM地址

    if(gui_disp_buf[y][x>>3]&DCB_HEX_TAB[x&0x07])  //获取第一个点的数据
        DataTemp = 0xf0;
    else 
        DataTemp = 0x00;
    x++;
    if(gui_disp_buf[y][x>>3]&DCB_HEX_TAB[x&0x07])  //获取第二个点的数据
        DataTemp |=0x0f;
    else 
        DataTemp &=0xf0;
    WriteDataUC1698U(DataTemp);  				   //发送数据

    x++;
    if(gui_disp_buf[y][x>>3]&DCB_HEX_TAB[x&0x07]) //获取第三个点的数据
        DataTemp = 0xf0;
    else 
        DataTemp = 0x00;	
    WriteDataUC1698U(DataTemp);  				   //发送数据		  
   return(1);
}

//============================================================================= 
//函 数 名: GUI_BufferPoint() 
//功 能：	更改指定点的缓冲区的数据，并不更改模块显示内容
//入口参数：  x		指定点所在列的位置
//            y		指定点所在行的位置
//            color	显示颜色(为0时灭，为1时显示)
//出口参数： 无
//返 回 值： 返回值为1时表示操作成功，为0时表示操作失败。(操作失败原因是指定地址超出有
//           效范围)
//说明    ： 使用此函数画点时，只更改缓冲区数据，需要配合区域刷新函数使用，
//           增加此函数的目的是为了加快显示汉或图标显示速度
//============================================================================= 
INT8U  GUI_BufferPoint(INT8U x, INT8U y, TCOLOR color)
{ 
    //参数过滤 
    if(x>GUI_LCM_XMAX) return(0);
    if(y>GUI_LCM_YMAX) return(0);
	   
    // 设置缓冲区相应的点
    if(color) gui_disp_buf[y][x>>3] |= DCB_HEX_TAB[x&0x07];
    else  gui_disp_buf[y][x>>3] &= (~DCB_HEX_TAB[x&0x07]); 
   
   return(1);
}
//============================================================================= 
//函 数 名: GUI_ReadPoint() 
//功 能：	读取指定点的颜色。
//入口参数：  x		指定点所在列的位置
//            y		指定点所在行的位置//            
//出口参数： ret		保存颜色值的指针
//返 回 值： 返回值为1时表示操作成功，为0时表示操作失败。(操作失败原因是指定地址超出有
//           效范围)
//============================================================================= 
INT8U  GUI_ReadPoint(INT8U x, INT8U y, TCOLOR *ret)
{  TCOLOR  bak;

   // 参数过滤 
   if(x>=GUI_LCM_XMAX) return(0);
   if(y>=GUI_LCM_YMAX) return(0);
  
   // 取得该点颜色
   bak = gui_disp_buf[y][x>>3];
   if( (bak&DCB_HEX_TAB[x&0x07])!=0 ) *ret = 1;
      else  *ret = 0;
   
   return(1);
}



//================================================================================================= 
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//================================================================================================= 














