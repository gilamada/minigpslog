/*----------------------------------------------------------
	nokia 5110 driver program for 51 mcu
	11.0592MHz
	void L5_init(void);  //LCD5110��ʼ��
	void L5_wrbyte(bit,uchar); //д���ݵ�LCD 0��������|1��������
	void L5_setxy(uchar,uchar); //����LCD���꺯��
	void L5_clear(void); //LCD5110����
----------------------------------------------------------*/
#include "_STC90.h"
#include "_CHAR_USER.h"

//�˿ڶ���
sbit L5_SCLK = P2^0; //ʱ��
sbit L5_SDIN = P2^2; //����
sbit L5_DC   = P2^4; //1����|0����
sbit L5_RST  = P2^5; //��λ
sbit L5_CE   = P2^7; //ʹ��
sbit L5_LED  = P3^6; //�����

//ʹ��SPI�ӿ�д���ݵ�LCD--------------------------------------------
void L5_wrbyte(bit L5_com,uchar L5_dat)
{
    uchar i;	        
    L5_CE = 0; //�ر�LCD
    L5_DC = L5_com; //0��������|1��������
	for(i=8;i>0;i--){
		if(L5_dat&0x80)	L5_SDIN = 1; //�������
		else L5_SDIN = 0;
		L5_SCLK = 0; L5_dat<<=1; L5_SCLK = 1; //ʱ��
	}
	//L5_CE = 1; //ʹ��LCD
}

//����LCD���꺯��------------------------------------------------
void L5_setxy(uchar x, uchar y)
{
	L5_wrbyte(0,0x40|y); //��0~5
	L5_wrbyte(0,0x80|x); //��0~83
	L5_CE = 1; //ʹ��LCD
}

//LCD��������--------------------------------------------------------
void L5_clear(void)
{
    uint j;
    L5_setxy(0,0); //��ʼλ��
	for(j=504;j>0;j--)
		L5_wrbyte(1,0x00); //д0����
	L5_CE = 1; //ʹ��LCD
}

//LCD5110��ʼ��-------------------------------------------------------
void L5_init(void)
{            
	L5_RST = 0; _nms(100); L5_RST = 1; //����һ����λ�ĵ͵�ƽ����	 
	L5_CE = 1; //�ر�LCD
    L5_wrbyte(0,0x21); //ʹ����չ��������LCDģʽ
	//Vlcd=3.06+(VOP)*0.06,����VOPΪ0B0101 0000Ϊʮ���Ƶ�80��Vlcd=7.86V
	L5_wrbyte(0,0xd0); //����ƫ�õ�ѹ 0xc8
//	L5_wrbyte(0,0x06); //�¶�У��
//	L5_wrbyte(0,0x13); //����ƫ��ϵͳ1:48
    L5_wrbyte(0,0x20); //ʹ�û�������
    L5_wrbyte(0,0x0c); //�趨��ʾģʽ,������ʾ 0x0d����
	L5_CE = 1; //ʹ��LCD
	L5_clear();	//����
}

//��ʾ���� s���ֵ�������λ---------------------------------------------                     
void L5_wrchr_cn(uchar x,uchar y,uchar s)
{
    uchar n;    
	L5_setxy(x,y); //���ó�ʼλ��
    for(n=0;n<24;n++){ //дһ������
		if(n==12) L5_setxy(x,y+1);
		L5_wrbyte(1,HZ12[s][n]);
	}	
}

//��ʾ���ִ�-----------------------------------------------------------                     
void L5_wrstr_cn(uchar x,uchar y,uchar s,uchar num,uchar lrow)
{//num ��ʾ���ֵĸ���; s ���ֵ��������е���ʼ����; lrow ������ʾ���м��
    uchar t;    
    for(t=0;t<num;t++) L5_wrchr_cn(x+(lrow+12)*t,y,s+t);
	L5_CE = 1; //ʹ��LCD
}




