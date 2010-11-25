//By Wun@THU	2010.1
//ͷ�ļ�
//#include<reg52.h> 
#include<intrins.h>

#define uint unsigned int 		//ռ2���ֽڣ�0~65535
#define uchar unsigned char		//ռ1���ֽڣ�0~255

//λ����
sbit sda=P2^0; 				//P2^0��I2C��������
sbit scl=P2^1; 				//P2^1��I2C��ʱ����
//sbit BMP085_EOC=P2^2;			
//sbit BMP085_XCLR=P2^3;

//��������
//BMP085��E2PROM�洢�ı궨����
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

long T=0;				//�¶�
long p=0;				//ѹ��

//====================================================
//��ʱ����
//====================================================
void wait(){;;}				
void delay(uint z) 			//z���ƺ��루ʹ��11.0592MHz����
{
	uint x,y;
	for(x=z;x>0;x--){for(y=111;y>0;y--);}
}

//====================================================
//I2C����ͨѶ����(��д�豸�����BMP085)
//====================================================
void I2Cinit() 				//I2C�ĳ�ʼ����SDA��SCL��Ϊ�ߵ�ƽ
{
	sda=1;
	wait();
	scl=1;
	wait();
}
void I2Cstart() 			//��ʼ�ź�
{	
	I2Cinit();
	sda=0;
	wait();
}
void I2Cstop() 				//ֹͣ�ź�
{
	sda=0;
	wait();
	scl=1;
	wait();
	sda=1;
	wait();
}
void I2Cresponse()  			//Ӧ���ź�
{
	uchar i;
	scl=1;
	wait();
	while((sda==1)&&(i<250))i++;	
	scl=0;
	wait();
}
void I2Cwrite_byte(uchar dat) 		//д1���ֽ�
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
uchar I2Cread_byte() 			//��1���ֽ�
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
void I2Cwrite_add(uchar address,uchar dat) //д���ݣ�1���ֽڵ�ַλ��1���ֽ�����
{
	I2Cstart();
	I2Cwrite_byte(0xEE);		// 16����Oxa0ת��8bit����������ǰ7λ���豸�ţ�ĩλ"0"��ʾ���豸д�롣
	I2Cresponse();			
	I2Cwrite_byte(address);		//д��ַ
	I2Cresponse();			
	I2Cwrite_byte(dat);		//д����
	I2Cresponse();			
	I2Cstop();
	delay(10);			//�յ�stop�������twrʱ����д���ݣ����ڼ䲻�����κ���Ϣ
}
uchar I2Cread_add(uchar address) 	//�����ݣ�1���ֽڵ�ַλ��������������ֵ
{
	uchar dat;
	I2Cstart();
	I2Cwrite_byte(0xEE); 		//16����Oxa0ת��8bit����������ǰ7λ���豸�ţ�ĩλ"0"��ʾ���豸д�롣
	I2Cresponse();
	I2Cwrite_byte(address);		//���豸д����Ҫ���ҵĵ�ַ���ƶ��豸��ָ�룩
	I2Cresponse();
	I2Cstart(); 			//ˢ�£��ٴβ����豸
	I2Cwrite_byte(0xEF);		// 16����Oxa0ת��8bit����������ǰ7λ���豸�ţ�ĩλ"1"��ʾ���豸��ȡ��
	I2Cresponse();
	dat=I2Cread_byte();		//��ȡ����
	I2Cstop();			//�������������"��Ӧ��"���ߵ�ƽ������ֱ�ӷ�����ֹ�ź�
	delay(1);
	return dat;
}

//====================================================
//ѹ���������������� ��BMP085��
//====================================================
void BMP085_init()//��ȡBMP085��E2PROM�Ĳ�����������ȫ�ֱ���
{		
	uchar memo[22];
	uchar i,*px;
	void *pf;
	//��EEPROM�ж�ȡԤ�����
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

void BMP085_sample(uchar oss)//�¶ȡ�ѹ������
//ossΪ������ʽ��
//0-ultra low power;
//1-standard;
//2-high resolution;
//3-ultra high resolution
{
	long UT=0;			//�¶�ԭʼֵ	
	long UP=0;			//ѹ��ԭʼֵ
	long X1,X2,X3;
	long B3,B5,B6,B7;
	unsigned long B4;
	uchar i,*px,BMPmemo[3];
	void *pf;

	I2Cwrite_add(0xF4,0x2E);	//��������¶�
	delay(10);							//�ȴ�AD���ӳ�4.5ms����
	for(i=0;i<2;i++){BMPmemo[i]=I2Cread_add(0xF6+i);}//��ȡ���
	px=BMPmemo;	   						
	pf=&UT;								
	for(i=0;i<2;i++){*((char *)pf+2+i)=*(px+i);}

	I2Cwrite_add(0xF4,0x34+(oss<<6));//�������ѹ��
	delay(30);							//�ӳ�ʱ���ӹ�����ʽ������������ֲ�
	for(i=0;i<3;i++){BMPmemo[i]=I2Cread_add(0xF6+i);}//��ȡ���
	px=BMPmemo;	   						
	pf=&UP;								
	for(i=0;i<3;i++){*((char *)pf+1+i)=*(px+i);}
	UP=UP>>(8-oss);

	X1=(UT-AC6)*AC5/32768; 		//�����¶�
	X2=MC;
	X2=X2*2048/(X1+MD);
	B5=X1+X2;
	T=(B5+8)/16;

	B6=B5-4000;			//����ѹ��
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
//������
//====================================================
void main()
{
	float temperature=0;
	float pressure=0;

	LCDinit();				//Һ����ʼ��
	I2Cinit(); 				//I2C���߳�ʼ��
	delay(20);
	BMP085_init();				//��ȡBMP085��E2PROM�е�11���궨����

	while(1)
	{	
		BMP085_sample(3);
		temperature=T;
		pressure=p;
		temperature=temperature/10;	//��λ�����϶�
		pressure=pressure/1000;		//��λ��kPa
		delay(100);
		//����Ϳ��Ե�����ʾ��������LED����LCM��ʾ�����   	
	}		
}





















