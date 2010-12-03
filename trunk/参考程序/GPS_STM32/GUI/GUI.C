//================================================================================================
// 文件名称: GUI.C 
// 芯片类型: STM32F103VBT6                                           
// 开发软件: Keil, Windows XP2 
// 开发时间: 2009年09月08日                                                               
// 程序功能: 液晶显示GUI 
// 编写人员: 崔超                                                                       
// 联系方式: 13686036830  cuichao@xrd-lcd.com 深圳市欣瑞达电子有限公司  
//================================================================================================ 
 
#include "config.h"		 //头文件配置
#include "Chinese.h"
#include "ASCII.h"

//============================================================================= 
#define GUI_DISP_COLOR 1
#define GUI_BACK_COLOR 0 
//============================================================================= 
//   GUI相关全局变量
//============================================================================= 
INT8U  GUIEnFont = En_8x16;                //英文字体
INT8U  GUITextMode = GUI_TEXTMODE_NORMAL;  //显示模式

INT8U Char_XSIZE;   //英文字体X宽度
INT8U Char_YSIZE;   //英文字体Y宽度	  




//============================================================================= 
//函 数 名: GUI_PutChinese() 
//功 能：	指定位置显示一个16x16的汉字
//入口参数：UniCode:汉字内码 x:X轴坐标0--(160-16) Y轴坐标0--(160-16)
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void GUI_PutChinese(INT16U UniCode, INT8U x, INT8U y)
{
  INT16U CodeID;
  INT8U i,j,ImgData=0;
  _CONST_ INT8U *ImgDataPtr;

  // 在字库里搜索汉字
    for(CodeID=0; CodeID < hzNum; CodeID++){
        // 找到 指定汉字的 Index 后, 跳出循环
        if ( (hzIndex[2*CodeID] == UniCode/256) && (hzIndex[2*CodeID+1] == UniCode%256) ){
            break;
        }else if(CodeID == (hzNum-1)){//未检索到的汉字,显示个"##"提示吧
           GUI_DispChar('#',x,y);
           GUI_DispChar('#',x+Char_XSIZE,y);
            return;
        }
    }
//----------------------------------
    switch (GUITextMode) {
        case GUI_TEXTMODE_NORMAL :
//-------------------------------------------------------------------------
			ImgDataPtr = hzdot+CodeID*((Chinese_XSIZE+7)/8*Chinese_YSIZE);
   	   	   	for(i=0; i<Chinese_YSIZE; i++)						// 显示共16行
   	   	   	{  for(j=0; j<Chinese_XSIZE; j++)					// 每行共16点
   	   	   	   	{  //若当前点为0、8点，读取点阵数据
   	   	   	   	   	if( (j&0x07)==0 ) ImgData =*ImgDataPtr++;      
   	   	   	   	   	if( (ImgData&DCB_HEX_TAB[j&0x07])==0 ) GUI_BufferPoint(x, y,GUI_BACK_COLOR); 
   	   	   	   	   	else  GUI_BufferPoint(x, y,GUI_DISP_COLOR);    	   	   	   	   	 
   	   	   	   	    x++;
                }     
   	   	   	  y++;						// 指向下一行
      		  x -= Chinese_XSIZE;					// 恢复x值
            }
       
//-------------------------------------------------------------------------
             break;
        case GUI_TEXTMODE_REVERSE :
//-------------------------------------------------------------------------
			ImgDataPtr = hzdot+CodeID*((Chinese_XSIZE+7)/8*Chinese_YSIZE);
   	   	   	for(i=0; i<Chinese_YSIZE; i++)						// 显示共16行
   	   	   	{  for(j=0; j<Chinese_XSIZE; j++)					// 每行共16点
   	   	   	   	{  // 若当前点为0、8点，读取点阵数据 
   	   	   	   	   	if( (j&0x07)==0 ) ImgData =*ImgDataPtr++;     
   	   	   	   	   	if( (ImgData&DCB_HEX_TAB[j&0x07])==0 ) GUI_BufferPoint(x, y,GUI_DISP_COLOR);  //这里取前景色
   	   	   	   	   	else GUI_BufferPoint(x, y,GUI_BACK_COLOR);       
   	   	   	   	    x++;
                }     
   	   	   	  y++;						// 指向下一行
      		  x -= Chinese_XSIZE;					// 恢复x值
            }
//-------------------------------------------------------------------------
             break;
        case GUI_TEXTMODE_UNDERLINE :
//-------------------------------------------------------------------------
    
            ImgDataPtr = hzdot+CodeID*((Chinese_XSIZE+7)/8*Chinese_YSIZE);
   	   	   	for(i=0; i<Chinese_YSIZE-1; i++)     // 显示上面15行
   	   	   	{  for(j=0; j<Chinese_XSIZE; j++)    // 每行共16点
   	   	   	   	{  // 若当前点为0、8点，读取点阵数据 
   	   	   	   	   	if( (j&0x07)==0 ) ImgData =*ImgDataPtr++; 
					if(i ==  Chinese_XSIZE-1) ImgData = 0xff;  
   	   	   	   	   	if( (ImgData&DCB_HEX_TAB[j&0x07])==0 ) GUI_BufferPoint(x, y,GUI_BACK_COLOR); 
   	   	   	   	   	else   GUI_BufferPoint(x, y, GUI_DISP_COLOR);	   
   	   	   	   	    x++;
                }     
   	   	   	  y++;						// 指向下一行
      		  x -= Chinese_XSIZE;					// 恢复x值
            }//最后一行全部显示 
			
			for(j=0; j<Chinese_XSIZE; j++)             
  			{
			    GUI_BufferPoint(x, y, GUI_DISP_COLOR);
				x++;
			}
			x -= Chinese_XSIZE;					// 恢复x值
			y++;

//-------------------------------------------------------------------------
             break;
        default :
             break;
    }
	GUI_RefreshBlock(x,y-Chinese_YSIZE,x+Chinese_XSIZE,y);//刷新汉字的区域
}



//============================================================================= 
//函 数 名: Display_ASCII() 
//功 能：	指定位置显示一个ASCII
//入口参数：Char:ASCII码 x:X轴坐标0--(160-8) Y轴坐标0--(160-16)
//出口参数： 无
//返 回 值： 无
//说明    ： 函数显示的文字列地址是8的整数倍，参数不是8的整数倍时自动向左调整
//============================================================================= 
void Display_ASCII(INT8S Char, INT8U x, INT8U y)
{
    INT8U i;
	INT8U _CONST_ *ImgData; 
	x = x>>3;
    if(GUIEnFont == En_8x8){        
          ImgData = chardot_8x8+(Char-0x20)*8;
        for(i = 0; i < En_8x8_YSIZE; i++){
			gui_disp_buf[y++][x] = *ImgData++;            
		}
		x = x<<3 ;
		GUI_RefreshBlock(x,y-En_8x8_YSIZE,x+En_8x8_XSIZE,y);	  //刷新字符的区域
    } else {
		ImgData = chardot_8x16+(Char-0x20)*16;
        for(i=0;i<En_8x16_YSIZE;i++){
		   	gui_disp_buf[y++][x] = *ImgData++;
           	         
        } 
		x = x<<3 ;
      	GUI_RefreshBlock(x,y-En_8x16_YSIZE,x+En_8x16_XSIZE,y);	  //刷新字符的区域
   }
}

//============================================================================= 
//函 数 名: Display_InvASCII() 
//功 能：	指定位置反显示一个ASCII
//入口参数：Char:ASCII码 x:X轴坐标0--(160-8) Y轴坐标0--(160-16)
//出口参数： 无
//返 回 值： 无
//说明    ： 函数显示的文字列地址是8的整数倍，参数不是8的整数倍时自动向左调整
//============================================================================= 
void Display_InvASCII(INT8S Char, INT8U x, INT8U y)
{
    INT8U i;
	INT8U _CONST_ *ImgData; 

	x = x>>3;
    if(GUIEnFont == En_8x8){        
         ImgData = chardot_8x8+(Char-0x20)*8;
        for(i = 0; i < En_8x8_YSIZE; i++){
			gui_disp_buf[y++][x] = *ImgData++^0xff;            
		}
		x = x<<3 ;
		GUI_RefreshBlock(x,y-En_8x8_YSIZE,x+En_8x8_XSIZE,y);	  //刷新字符的区域
    } else {
		ImgData = chardot_8x16+(Char-0x20)*16;
        for(i=0;i<En_8x16_YSIZE;i++){
		   	gui_disp_buf[y++][x] = *ImgData++^0xff;            	         
        } 
		x = x<<3 ;
      	GUI_RefreshBlock(x,y-En_8x16_YSIZE,x+En_8x16_XSIZE,y);	  //刷新字符的区域
   }
}
//============================================================================= 
//函 数 名: Display_InvASCII() 
//功 能：	指定位置显示一个带有下划线的ASCII
//入口参数：Char:ASCII码 x:X轴坐标0--(160-8) Y轴坐标0--(160-16)
//出口参数： 无
//返 回 值： 无
//说明    ： 函数显示的文字列地址是8的整数倍，参数不是8的整数倍时自动向左调整
//============================================================================= 
void Display_ASCII_UnderLine(INT8S Char, INT8U x, INT8U y)
{
    INT8U i;
	INT8U _CONST_ *ImgData; 
	x = x>>3;
    if(GUIEnFont == En_8x8){        
          ImgData = chardot_8x8+(Char-0x20)*8;
        for(i = 0; i < En_8x8_YSIZE-1; i++){
			gui_disp_buf[y++][x] = *ImgData++;            
		}
		gui_disp_buf[y++][x] = 0xff;  
		x = x<<3 ;
		GUI_RefreshBlock(x,y-En_8x8_YSIZE,x+En_8x8_XSIZE,y);	  //刷新字符的区域
    } else {
		ImgData = chardot_8x16+(Char-0x20)*16;
        for(i=0;i<En_8x16_YSIZE-1;i++){
		   	gui_disp_buf[y++][x] = *ImgData++;            	         
        }
		gui_disp_buf[y++][x] = 0xff;   
		x = x<<3 ;
      	GUI_RefreshBlock(x,y-En_8x16_YSIZE,x+En_8x16_XSIZE,y);	  //刷新字符的区域
   }
}

//============================================================================= 
//函 数 名: Display_InvASCII() 
//功 能：	指定位置显示一个ASCII码
//入口参数：Char:ASCII码 x:X轴坐标0--(160-8) Y轴坐标0--(160-16)
//出口参数： 无
//返 回 值： 无
//说明    ： 字体使用是系统设定的字体
//============================================================================= 
void GUI_DispChar(INT8S c, INT8U x, INT8U y)
{
   if(c < ' ') return;
   if(c > '~'+1) return;
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

//============================================================================= 
//函 数 名: GUI_Initialize() 
//功 能：	初始化GUI，包括初始化显示缓冲区，初始化LCM并清屏。
//入口参数： 无
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void  GUI_Initialize(void)
{  
    LCD_Initialize();  // 初始化LCM模块工作模式，纯图形模式
    GUI_FillSCR(0x00); // 初始化缓冲区为0x00，并输出屏幕(清屏)
	
    GUIEnFont = En_8x16;
    GUITextMode = GUI_TEXTMODE_NORMAL;
    Char_XSIZE = En_8x16_XSIZE;
    Char_YSIZE = En_8x16_YSIZE;

}
//============================================================================= 
//函 数 名: GUI_SetTextMode() 
//功 能：	设置文字显示模式
//入口参数： TextMode:模式代码
//出口参数： 无
//返 回 值： 模式代码
//============================================================================= 
INT8U GUI_SetTextMode(INT8U TextMode)
{
    GUITextMode = TextMode;
    return GUITextMode;
}

//============================================================================= 
//函 数 名: GUI_SetEnFont()
//功 能：	设置显示的英文字体
//入口参数： EnFont:字体代码   目前支持两种字体8x16 8x8
//出口参数： 无
//返 回 值： 无
//============================================================================= 
void GUI_SetEnFont(INT8U EnFont)
{
  if (EnFont == En_8x8){
    GUIEnFont = En_8x8;
    Char_XSIZE = En_8x8_XSIZE;
    Char_YSIZE = En_8x8_YSIZE;
  } else {
    GUIEnFont = En_8x16;
    Char_XSIZE = En_8x16_XSIZE;
    Char_YSIZE = En_8x16_YSIZE;

  }	
}

//============================================================================= 
//函 数 名: GUI_DispString()
//功 能：	指定位置显示一个字符串
//入口参数： s:字符串 x0:X轴坐标 y0:Y轴坐标
//出口参数： 无
//返 回 值： 无
//说明 :支持中英混合 当中英混合时英文字体强制为8x16 如果单英文 按照设定字体
//============================================================================= 
void GUI_DispString(char *s,INT8U x0,INT8U y0)
{
    INT8U i;

	i = 0;
	 while(s[i]!='\0')//遍历字符串看是否有中文。
       { if (s[i] > 0x80) 	//字符内码大于0x80的是中文
            {	GUI_SetEnFont(En_8x16);
			    break;
			}
			i++;
        }    
    i = 0;
    while(s[i]!='\0'){
        if (s[i] < 0x80) {//是ASCII
             if(s[i] == '\n'){
                    x0 = 0;
                    y0 += Char_YSIZE;
             } else {
                if (x0 > (GUI_LCM_XMAX-8)) {//换行
                    x0 = 0;
                    y0 += Char_YSIZE;
					if(y0 > GUI_LCM_XMAX - Char_YSIZE)
					y0 = 0;
                }

                 GUI_DispChar(s[i], x0, y0);              
                
                x0 += Char_XSIZE;
             }
        } else {//是中文
		
            if (x0 > GUI_LCM_XMAX-Chinese_XSIZE){
                x0 = 0;
                y0 += Chinese_YSIZE;
				if(y0 > GUI_LCM_XMAX - Chinese_YSIZE)
					y0 = 0;
            }
            GUI_PutChinese(s[i]*256+s[i+1],x0,y0);//中文
            i++;
            x0 += Chinese_XSIZE;
       }
       s++;
    }
}
//============================================================================= 
//函 数 名: GUI_DispDec()
//功 能：	显示十进制数值
//入口参数： v:显示的数据 x:X轴坐标 y:Y轴坐标 Len:长度1--5
//出口参数： 无
//返 回 值： 无
//说明 :处理长度最多5个数字(因为INT16U--->65536)
//      XY 均是起点位置坐标 也就是数值最高位的坐标
//=============================================================================  
void GUI_DispDec(INT16U v, INT16U x, INT16U y, INT8U Len)
{
    INT8U i;
    INT8U CharBuf[5];
    INT8U HighByte;

    HighByte = 0;
    for(i = 0; i < 5; i++){
        CharBuf[i] = (INT8U)(v%10);
        v = v/10;
        if(CharBuf[i]){
            HighByte = i;
        }
    }
    //第0位无论如何也显示
    i = 0;
    GUI_DispChar(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
    for(i = 1; i < Len; i++){
        if(CharBuf[i]){
            GUI_DispChar(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
        }else if(i > HighByte){
            GUI_DispChar(' ',x+((Len-1)-i)*Char_XSIZE,y);
        }
    }

}
//============================================================================= 
//函 数 名: GUI_DispHex()
//功 能：	显示一个数据的十六进制值
//入口参数： v:显示的数据 x:X轴坐标 y:Y轴坐标 Len:长度1--4
//出口参数： 无
//返 回 值： 无
//说明 :处理长度最多4个数字(因为INT16U--->0xFFFF)
//      XY 均是起点位置坐标 也就是数值最高位的坐标
//============================================================================= 
void GUI_DispHex(INT32U v, INT8U x, INT8U y, INT8U Len)
{
    INT8U i;
    INT8U HexData;
    if(Len > 8){//限制范围
        Len = 8;
    }
    for(i = 0; i < Len; i++){
        HexData = v&0x0F;
        v = v >>4;
        if(HexData < 0x0A){
            GUI_DispChar(HexData+'0',x+Char_XSIZE*(Len-1-i),y);
        }else{
            GUI_DispChar(HexData-0x0A+'A',x+Char_XSIZE*(Len-1-i),y);
        }
    }
}

//============================================================================= 
//函 数 名: Display_Number()
//功 能：	显示不带符号的整数  
//入口参数： v:显示的数据 x:X轴坐标 y:Y轴坐标 Len:长度1--5
//出口参数： 无
//返 回 值： 无
//说明 :处理长度最多5个数字(因为INT16U--->65535)
//      XY 均是起点位置坐标 也就是数值最高位的坐标
//============================================================================= 
void Display_Number(INT16U Number, INT8U X, INT8U Y, INT8U Lenth)
{
  INT8U DispNum;

  X = ( X + Lenth * 8 - 8 );
  for(; Lenth>0; Lenth--)
  {
    DispNum = Number%10 + 0x30;
    GUI_DispChar(DispNum, X, Y);
    X -= 8;
    Number = Number / 10;
  }
}
//============================================================================= 
//函 数 名: Display_Number()
//功 能：	显示带符号的整数  
//入口参数： v:显示的数据 x:X轴坐标 y:Y轴坐标 Len:长度1--5
//出口参数： 无
//返 回 值： 无
//说明 :处理长度最多5个数字(因为INT16U--->32768)
//      XY 均是起点位置坐标 也就是数值最高位的坐标
//============================================================================= 
void Display_SignedNumber(int Number,INT8U X,INT16U Y,INT8U Lenth)
{
  if(Number < 0)
  {
    GUI_DispChar('-', X, Y);
    Display_Number(-Number, X+8, Y, Lenth);
  }
  else
  {
    GUI_DispChar(' ', X, Y);
    Display_Number(Number, X+8, Y, Lenth);
  }
}

 //============================================================================= 
//函 数 名: GUI_Line() 
//功 能:    画线子程序:为了回避复杂的浮点运算,采用了整数数字微分的方法
//			以(x列,y行)为起点,以(x1列,y1行)为终点画一条直线
//入口参数: x,y	起始坐标	x1,y1终点坐标	color:0-1 0:白线 1:黑线
//	 ┏━━━━━━━━┓
//	 ┃ x,y ------->x1 ┃
//	 ┃	|╲			   ┃
//   ┃ |  ╲		   ┃	
//	 ┃	|	 ╲		   ┃
//	 ┃	y1	  x1,y1	   ┃
//	 ┗━━━━━━━━┛
//		   zuo
//出口参数: 无 
//返 回 值: 无
//============================================================================= 

void GUI_Line( INT16S x, INT16S y, INT16S x1, INT16S y1, TCOLOR color) 
{ 


    INT16S dx,dy,err = 0;
   
 
    if(y != y1)
	{
		if(y1 < y) /*这个判断语句保证了y1>=y这个前提*/
		{
			dx = y1; dy = x1;
			y1 = y;  x1 = x;
			y = dx;  x = dy;
		}
		dy = y1 - y;
		dx = x1 - x;
		GUI_Point(x ,y,color);
		
		if((x1 >= x)&&((y1 - y) <= (x1 - x)))
		{
			for(;x != x1;)
			{
				if(err < 0)
				{
					GUI_Point(x + 1,y,color);
					x += 1;
					err += dy;
				}
				else
				{
					GUI_Point(x + 1,y + 1,color);
					x += 1;
					y += 1;
					err += (dy - dx);
				}
			}
		}
		else
			if((x1 >= x)&&((y1 - y) > (x1 - x)))
			{
				for(;y != y1;)
				{
					if(err < 0)
					{
						GUI_Point(x + 1,y + 1,color);
						x += 1;
						y += 1;
						err += (dy - dx);
					}
					else
					{
						GUI_Point(x,y + 1,color);
						y += 1;
						err -= dx;
					}
				}
			}
			else
				if((x1 < x)&&((y1 - y) <= (x - x1)))
				{
					for(;x != x1;)
					{
						if(err < 0)
						{
							GUI_Point(x - 1,y,color);
							x -= 1;
							err += dy;
						}
						else
						{
							GUI_Point(x - 1,y + 1,color);
							x -= 1;
							y += 1;
							err += (dy + dx);
						}
					}
				}
				else
					if((x1 < x)&&((y1 - y) > (x1 - x)))
					{
						for(;y != y1;)
						{
							if(err < 0)
							{
								GUI_Point(x - 1,y + 1,color);
								x -= 1;
								y += 1;
								err += (dy + dx);
							}
							else
							{
								GUI_Point(x,y + 1,color);
								y += 1;
								err += dx;
							}
						}
					}
	}	
	else
	{
		dx = x; dy = x1;
		if(x < x1)
		{
			dx = x1;
			dy = x;
		}
		for(;dy < dx;dy++)
			GUI_Point(dy,y,color);

			GUI_Point(x1 ,y1,color);
	}
}
 //============================================================================= 
//函 数 名: GUI_Circle() 
//功 能:    指定圆心位置及半径，画圆
//入口参数: x0		圆心的x坐标值
//          y0		圆心的y坐标值
//          r       圆的半径
//          color	显示颜色
//出口参数: 无 
//返 回 值: 无
//============================================================================= 
void  GUI_Circle(u32 x0, u32 y0, u32 r, TCOLOR color)
{  s32  draw_x0, draw_y0;			// 刽图点坐标变量
   s32  draw_x1, draw_y1;	
   s32  draw_x2, draw_y2;	
   s32  draw_x3, draw_y3;	
   s32  draw_x4, draw_y4;	
   s32  draw_x5, draw_y5;	
   s32  draw_x6, draw_y6;	
   s32  draw_x7, draw_y7;	
   s32  xx, yy;					// 画圆控制变量
 
   s32  di;						// 决策变量
   
   /* 参数过滤 */
   if(0==r) return;
   
   /* 计算出8个特殊点(0、45、90、135、180、225、270度)，进行显示 */
   draw_x0 = draw_x1 = x0;
   draw_y0 = draw_y1 = y0 + r;
   if(draw_y0<GUI_LCM_YMAX) GUI_Point(draw_x0, draw_y0, color);	// 90度
	
   draw_x2 = draw_x3 = x0;
   draw_y2 = draw_y3 = y0 - r;
   if(draw_y2>=0) GUI_Point(draw_x2, draw_y2, color);			// 270度
   
	
   draw_x4 = draw_x6 = x0 + r;
   draw_y4 = draw_y6 = y0;
   if(draw_x4<GUI_LCM_XMAX) GUI_Point(draw_x4, draw_y4, color);	// 0度
   
   draw_x5 = draw_x7 = x0 - r;
   draw_y5 = draw_y7 = y0;
   if(draw_x5>=0) GUI_Point(draw_x5, draw_y5, color);			// 180度   
   if(1==r) return;					// 若半径为1，则已圆画完
   
   
   /* 使用Bresenham法进行画圆 */
   di = 3 - 2*r;					// 初始化决策变量
   
   xx = 0;
   yy = r;	
   while(xx<yy)
   {  if(di<0)
	  {  di += 4*xx + 6;	      
	  }
	  else
	  {  di += 4*(xx - yy) + 10;
	  
	     yy--;	  
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
		 draw_x4--;
		 draw_x5++;
		 draw_x6--;
		 draw_x7++;	 	
	  }
	  
	  xx++;   
	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;
	  draw_y4++;
	  draw_y5++;
	  draw_y6--;
	  draw_y7--;
		
	
	  /* 要判断当前点是否在有效范围内 */
	  if( (draw_x0<=GUI_LCM_XMAX)&&(draw_y0>=0) )	
	  {  GUI_Point(draw_x0, draw_y0, color);
	  }	    
	  if( (draw_x1>=0)&&(draw_y1>=0) )	
	  {  GUI_Point(draw_x1, draw_y1, color);
	  }
	  if( (draw_x2<=GUI_LCM_XMAX)&&(draw_y2<=GUI_LCM_YMAX) )	
	  {  GUI_Point(draw_x2, draw_y2, color);   
	  }
	  if( (draw_x3>=0)&&(draw_y3<=GUI_LCM_YMAX) )	
	  {  GUI_Point(draw_x3, draw_y3, color);
	  }
	  if( (draw_x4<=GUI_LCM_XMAX)&&(draw_y4>=0) )	
	  {  GUI_Point(draw_x4, draw_y4, color);
	  }
	  if( (draw_x5>=0)&&(draw_y5>=0) )	
	  {  GUI_Point(draw_x5, draw_y5, color);
	  }
	  if( (draw_x6<=GUI_LCM_XMAX)&&(draw_y6<=GUI_LCM_YMAX) )	
	  {  GUI_Point(draw_x6, draw_y6, color);
	  }
	  if( (draw_x7>=0)&&(draw_y7<=GUI_LCM_YMAX) )	
	  {  GUI_Point(draw_x7, draw_y7, color);
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

