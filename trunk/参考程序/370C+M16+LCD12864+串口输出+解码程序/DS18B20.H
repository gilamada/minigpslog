/*******************************************
函数名称: B20_init
功    能: 复位DS18B20
参    数: 无
返回值  : 无
/********************************************/
void B20_init(void)
{
 DDRD|=BIT(DS18B20);			//配置为输出			  
 PORTD&=~BIT(DS18B20);			//拉低
 Delayus(600);					//等待600微秒
 PORTD|=BIT(DS18B20);			//释放总线
 Delayus(60);					//等待60微秒
 DDRD&=~BIT(DS18B20);			//配置为输入 
 while((PIND&(BIT(DS18B20))));	//等待DS18B20拉低
 while(!(PIND&(BIT(DS18B20)))); //等待DS18B20释放总线
}
/*******************************************
函数名称: B20_readB
功    能: 读取一个字节的数据
参    数: 无
返回值  : retd--返回的一个字节数据
/********************************************/
uchar B20_readB(void)
{
 uchar i,retd=0;
 for(i=0;i<8;i++)	 			//位计数值
 {
  retd>>=1;						//右移，准备接受新的数据位
  DDRD|=BIT(DS18B20);			//配置为输出
  PORTD&=~BIT(DS18B20);			//拉低，启动读数据位
  PORTD|=BIT(DS18B20);			//释放总线
  Delayus(5);					//等待5微秒
  DDRD&=~BIT(DS18B20);			//配置为输入，开始读取数据位
  if(PIND&BIT(DS18B20))			//该位是否为高
  {
   retd|=0x80;					//是就将此位置高
  }
  Delayus(50);					//等待50微秒
 }
 return retd;					//将读到的一个字节返回
}
/*******************************************
函数名称: B20_writeB
功    能: 写入一个字节的数据
参    数: wrd--要写入的数据
返回值  : 无
/********************************************/
void B20_writeB(uchar wrd)
{
 uchar i;
 for(i=0;i<8;i++)	  	        //位计数值
 {
  DDRD|=BIT(DS18B20);			//配置为输出
  PORTD&=~BIT(DS18B20);			//拉低，启动写数据位
  Delayus(1);					//等待1微秒
  if(wrd&0x01)					//此位数据是否为高
  {
   PORTD|=BIT(DS18B20);			//是高则将单总线拉高
  }
  else
  {
   PORTD&=~BIT(DS18B20);		//是低则将单总线拉低
  }
  Delayus(50);					//等待50微秒
  PORTD|=BIT(DS18B20);			//释放总线
  wrd>>=1;						//右移，为写入新的数据位做准备
 }
 Delayus(50);					//等待50微秒
}
/*******************************************
函数名称: Read_temp
功    能: 读取温度值
参    数: 无
返回值  : rettemp--返回的温度值
/********************************************/
uint Read_temp(uchar x)
{
uchar templ,temph;
 uint temp;
 B20_init();	   //初始化，每次写命令都从初始化开始
 B20_writeB(0xcc); //跳过ROM
 B20_writeB(0x44); //启动温度转换
 B20_init();	   //初始化，每次写命令都从初始化开始
 B20_writeB(0xcc); //跳过ROM
 B20_writeB(0xbe); //读寄存器
 templ=B20_readB();//读温度低字节
 temph=B20_readB();//读温度高字节
 temp=templ+temph*256;//将温度整理成16位变量
 return temp;	   //返回16位变量
}