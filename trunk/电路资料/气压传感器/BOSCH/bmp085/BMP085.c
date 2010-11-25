//By Wun@THU	2010.1
//头文件
//#include<reg52.h> 
#include<intrins.h>

#define uint unsigned int 		//占2个字节，0~65535
#define uchar unsigned char		//占1个字节，0~255

//位声明
sbit sda=P2^0; 				//P2^0：I2C的数据线
sbit scl=P2^1; 				//P2^1：I2C的时钟线
//sbit BMP085_EOC=P2^2;			
//sbit BMP085_XCLR=P2^3;

//变量声明
//BMP085中E2PROM存储的标定参数
int AC1=0;   				//(0xAA,0xAB)
int AC2=0;  				//(0xAC,0xAD)
int AC3=0;  				//(0xAE,0xAF)
uint AC4=0;				//(0xB0,0xB1)
uint AC5=0;				//(0xB2,0xB3)
uint AC6=0;				//(0xB4,0xB5)
int B1=0;				//(0xB6,0xB7)
int B2=0;				//(0xB8,0xB9)
int MB=0;				//(0xBA,0xBB)
int MC=0;				//(0xBC,0xBD)
int MD=0;				//(0xBE,0xBF)

long T=0;				//温度
long p=0;				//压力

//====================================================
//延时函数
//====================================================
void wait(){;;}				
void delay(uint z) 			//z近似毫秒（使用11.0592MHz晶振）
{
	uint x,y;
	for(x=z;x>0;x--){for(y=111;y>0;y--);}
}

//====================================================
//I2C总线通讯函数(读写设备号针对BMP085)
//====================================================
void I2Cinit() 				//I2C的初始化：SDA和SCL都为高电平
{
	sda=1;
	wait();
	scl=1;
	wait();
}
void I2Cstart() 			//开始信号
{	
	I2Cinit();
	sda=0;
	wait();
}
void I2Cstop() 				//停止信号
{
	sda=0;
	wait();
	scl=1;
	wait();
	sda=1;
	wait();
}
void I2Cresponse()  			//应答信号
{
	uchar i;
	scl=1;
	wait();
	while((sda==1)&&(i<250))i++;	
	scl=0;
	wait();
}
void I2Cwrite_byte(uchar dat) 		//写1个字节
{
	uchar i,temp;
	temp=dat;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;					
		scl=0;			
		wait();
		sda=CY;			
		wait();
		scl=1;			
		wait();
	}
	scl=0;				
	wait();
	sda=1;
	wait();
}
uchar I2Cread_byte() 			//读1个字节
{
	uchar i,k;
	scl=0;				
	wait();
	sda=1;
	wait();
	for(i=0;i<8;i++)
	{
		scl=1;
		wait();
		k=(k<<1)|sda;		
		scl=0;
		wait();
	}
	return k;
}
void I2Cwrite_add(uchar address,uchar dat) //写数据，1个字节地址位，1个字节数据
{
	I2Cstart();
	I2Cwrite_byte(0xEE);		// 16进制Oxa0转成8bit二进制数：前7位是设备号，末位"0"表示向设备写入。
	I2Cresponse();			
	I2Cwrite_byte(address);		//写地址
	I2Cresponse();			
	I2Cwrite_byte(dat);		//写数据
	I2Cresponse();			
	I2Cstop();
	delay(10);			//收到stop命令后，需twr时间来写数据，此期间不接收任何信息
}
uchar I2Cread_add(uchar address) 	//读数据，1个字节地址位，函数返回数据值
{
	uchar dat;
	I2Cstart();
	I2Cwrite_byte(0xEE); 		//16进制Oxa0转成8bit二进制数：前7位是设备号，末位"0"表示向设备写入。
	I2Cresponse();
	I2Cwrite_byte(address);		//向设备写入需要查找的地址（移动设备内指针）
	I2Cresponse();
	I2Cstart(); 			//刷新，再次查找设备
	I2Cwrite_byte(0xEF);		// 16进制Oxa0转成8bit二进制数：前7位是设备号，末位"1"表示从设备读取。
	I2Cresponse();
	dat=I2Cread_byte();		//读取数据
	I2Cstop();			//读完后主机返回"非应答"（高电平），并直接发出终止信号
	delay(1);
	return dat;
}

//====================================================
//压力传感器驱动函数 （BMP085）
//====================================================
void BMP085_init()//读取BMP085中E2PROM的参数，并赋给全局变量
{		
	uchar memo[22];
	uchar i,*px;
	void *pf;
	//从EEPROM中读取预设参数
	for(i=0;i<22;i++){memo[i]=I2Cread_add(0xAA+i);}
	px=memo;
	pf=&AC1;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+i);}
	pf=&AC2;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+2+i);}
	pf=&AC3;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+4+i);}
	pf=&AC4;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+6+i);}
	pf=&AC5;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+8+i);}
	pf=&AC6;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+10+i);}
	pf=&B1;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+12+i);}
	pf=&B2;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+14+i);}
	pf=&MB;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+16+i);}
	pf=&MC;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+18+i);}
	pf=&MD;		
	for(i=0;i<2;i++){*((char *)pf+i)=*(px+20+i);}
}

void BMP085_sample(uchar oss)//温度、压力采样
//oss为工作方式：
//0-ultra low power;
//1-standard;
//2-high resolution;
//3-ultra high resolution
{
	long UT=0;			//温度原始值	
	long UP=0;			//压力原始值
	long X1,X2,X3;
	long B3,B5,B6,B7;
	unsigned long B4;
	uchar i,*px,BMPmemo[3];
	void *pf;

	I2Cwrite_add(0xF4,0x2E);	//发命令采温度
	delay(10);							//等待AD，延迟4.5ms以上
	for(i=0;i<2;i++){BMPmemo[i]=I2Cread_add(0xF6+i);}//读取结果
	px=BMPmemo;	   						
	pf=&UT;								
	for(i=0;i<2;i++){*((char *)pf+2+i)=*(px+i);}

	I2Cwrite_add(0xF4,0x34+(oss<<6));//发命令采压力
	delay(30);							//延迟时间视工作方式而定，具体查手册
	for(i=0;i<3;i++){BMPmemo[i]=I2Cread_add(0xF6+i);}//读取结果
	px=BMPmemo;	   						
	pf=&UP;								
	for(i=0;i<3;i++){*((char *)pf+1+i)=*(px+i);}
	UP=UP>>(8-oss);

	X1=(UT-AC6)*AC5/32768; 		//计算温度
	X2=MC;
	X2=X2*2048/(X1+MD);
	B5=X1+X2;
	T=(B5+8)/16;

	B6=B5-4000;			//计算压力
	X1=B2;
	X1=(X1*(B6*B6/4096))/2048;
	X2=AC2;
	X2=X2*B6/2048;
	X3=X1+X2;
	B3=AC1;
	B3=(((B3*4+X3)<<oss)+2)/4;	
	X1=AC3;
	X1=X1*B6/8192;
	X2=B1;
	X2=(X2*(B6*B6/4096))/65536;
	X3=((X1+X2)+2)/4;
	B4=AC4;
	B4=B4*(unsigned long)(X3+32768)/32768;
	B7=((unsigned long)UP-B3)*(50000>>oss);	
	if(B7<0x80000000){p=(B7*2)/B4;}
	else {p=(B7/B4)*2;}
	X1=(p/256)*(p/256);
	X1=(X1*3038)/65536;
	X2=(-7357*p)/65536;
	p=p+(X1+X2+3791)/16;
}
//====================================================
//主函数
//====================================================
void main()
{
	float temperature=0;
	float pressure=0;

	LCDinit();				//液晶初始化
	I2Cinit(); 				//I2C总线初始化
	delay(20);
	BMP085_init();				//读取BMP085的E2PROM中的11个标定参数

	while(1)
	{	
		BMP085_sample(3);
		temperature=T;
		pressure=p;
		temperature=temperature/10;	//单位：摄氏度
		pressure=pressure/1000;		//单位：kPa
		delay(100);
		//这里就可以调用显示程序，驱动LED或者LCM显示结果了   	
	}		
}





















