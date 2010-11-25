/*******************************************
函数名称: LCD12864_portini
功    能: 初始化12864液晶用到的IO口
参    数: 无
返回值  : 无
/********************************************/
void LCD12864_portini(void)
{
	LCDb_CTRL_DDR |= BIT(LCDb_RS)|BIT(LCDb_RW)|BIT(LCDb_E)|BIT(LCDb_RST);//配置控制管脚为输出
	LCDb_DATA_DDR |= 0xFF;  //配置数据管脚为输出
	LCDb_SET_RST;		//不使能复位端
}
/*******************************************
函数名称: LCD12864_readbyte
功    能: 从12864液晶读出一个字节数据或者指令
参    数: DatCmd--为iDat时是数据，为iCmd时是指令
返回值  : dByte--读回的数据或者指令
/********************************************/
uchar LCD12864_readbyte(uchar DatCmd)
{
 	uchar dByte;
	if (DatCmd == iCmd)		 //指令操作
		LCDb_CLR_RS;
	else
		LCDb_SET_RS;
		
	LCDb_SET_RW;	   	  	 //读操作
	LCDb_SET_E;
	LCDb_DATA_DDR=0x00;	 //数据总线定义为输入
	dByte=LCDb_DI;			 //读数据或者指令
	Delayms(1);				 //时序调整
	LCDb_CLR_E;	
	LCDb_DATA_DDR|=0xff;	 //数据总线还原为输出
	return dByte;
}
/*******************************************
函数名称: LCD12864_sendbyte
功    能: 向12864液晶写入一个字节数据或者指令
参    数: DatCmd--为iDat时是数据，为iCmd时是指令
	  	  dByte--为写入12864的数据或者指令
返回值  : 无
/********************************************/
void LCD12864_sendbyte(uchar DatCmd, uchar dByte)
{
	if (DatCmd == iCmd)		//指令操作
		LCDb_CLR_RS;
	else
		LCDb_SET_RS;
		
	LCDb_CLR_RW;			//写操作
	LCDb_SET_E;
	LCDb_DO = dByte;		//写入数据
	Delayus(100);
	LCDb_CLR_E;	
}
/*******************************************
函数名称: LCD12864_sendstr
功    能: 向12864液晶写入一个字符串
参    数: ptString--字符串指针
返回值  : 无
/********************************************/
void LCD12864_sendstr(uchar *ptString)
{
	while((*ptString)!='\0')		 //字符串未结束一直写
	{
		LCD12864_sendbyte(iDat, *ptString++);
	}
}
/*******************************************
函数名称: LCD12864_clear
功    能: 12864液晶清屏
参    数: 无
返回值  : 无
/********************************************/
void LCD12864_clear(void)
{
	LCD12864_sendbyte(iCmd,LCDb_CLS);
	Delayms(2);// 清屏指令写入后，2ms 的延时是很必要的!!!
}
/*******************************************
函数名称: LCD12864_readBF
功    能: 12864液晶清屏
参    数: 无
返回值  : busy--为1时忙，为0时可以接收指令
/********************************************/
uchar LCD12864_readBF(void)
{	  
	uchar busy;
	busy=LCD12864_readbyte(iCmd);		//读回BF标志和地址
	if(busy&0x80)						//如果忙，返回忙状态
		busy=1;
	else	   							//不忙，可以写入
		busy=0;
	return busy;
}
/*******************************************
函数名称: LCD12864_gotoXY
功    能: 移动到指定位置
参    数: Row--指定的行
	  	  Col--指定的列
返回值  : 无
/********************************************/
void LCD12864_gotoXY(uchar Row, uchar Col)
{	
	switch (Row)		  //选择行
	{
		case 2:
			LCD12864_sendbyte(iCmd, LCDb_L2 + Col); break;	//写入第2行的指定列
		case 3:
			LCD12864_sendbyte(iCmd, LCDb_L3 + Col); break;	//写入第3行的指定列
		case 4:
			LCD12864_sendbyte(iCmd, LCDb_L4 + Col); break;	//写入第4行的指定列	
		default:
			LCD12864_sendbyte(iCmd, LCDb_L1 + Col); break;	//写入第1行的指定列	
	}
}
/*******************************************
函数名称: LCD12864_initial
功    能: 12864液晶初始化
参    数: 无
返回值  : 无
/********************************************/
void LCD12864_initial(void)
{
	Delayms(100);				// 等待内部复位
	LCD12864_portini();			//端口初始化
	
	LCD12864_sendbyte(iCmd, LCDb_FUNCTION);	//功能、模式设定
	while(LCD12864_readBF());
	LCD12864_sendbyte(iCmd, LCDb_ON);		//打开显示
	while(LCD12864_readBF());
	LCD12864_clear();						//清屏
	while(LCD12864_readBF());
	LCD12864_sendbyte(iCmd, LCDb_ENTRY);	// 输入模式设定		
}
/*******************************************
函数名称: LCD12864_opGDRAM
功    能: 向GDRAM写入数据,并显示（完成绘图功能）
参    数: ptGDRAM--GDRAM数据指针
返回值  : 无
/********************************************/
void LCD12864_opGDRAM(const uchar *ptGDRAM)
{
	uchar i,j;
	uint x=0;
	while(LCD12864_readBF());	  //读忙标志
	LCD12864_sendbyte(iCmd,LCDb_EXTCMD1);		//扩充指令集，并关闭绘图显示	
	//写入并显示上半屏幕
	for(i=0;i<32;i++)            				//行地址计数
     {	  	   	   
           for(j=0;j<8;j++)      				//列地址计数
           {            		   		 
		   		 LCD12864_sendbyte(iCmd,0x80|i);//输入行地址
				 Delayus(10);
		   		 LCD12864_sendbyte(iCmd,0x80|j);//输入列地址
				 Delayus(10);
                 LCD12864_sendbyte(iDat,ptGDRAM[x++]); //输入高位字节数据 
				 Delayus(10);                
				 LCD12864_sendbyte(iDat,ptGDRAM[x++]); //输入低位字节数据
				 Delayus(10); 
           }
     }
	 //写入并显示下半屏幕
	 for(i=0;i<32;i++)            
     {	  	   	   
           for(j=8;j<16;j++)      
           {            
		   		 LCD12864_sendbyte(iCmd,0x80|i);//输入行地址
				 Delayus(10);
		   		 LCD12864_sendbyte(iCmd,0x80|j);//输入列地址
				 Delayus(10);
                 LCD12864_sendbyte(iDat,ptGDRAM[x++]); //输入高位字节数据      
				 Delayus(10);
				 LCD12864_sendbyte(iDat,ptGDRAM[x++]); //输入低位字节数据
				 Delayus(10); 
           }
     }
	 Delayus(10);
	 LCD12864_sendbyte(iCmd,LCDb_EXTCMD2);			   //打开绘图显示
	 LCD12864_sendbyte(iCmd,LCDb_BASCMD);			   //还原回基本指令集模式
}
 
