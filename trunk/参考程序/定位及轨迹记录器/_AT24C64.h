#include "_STC90.h"

sbit I2C_SDA = P1^5; //AT24C64����
sbit I2C_SCL = P1^6; //AT24C64ʱ��
/*
16����λ�洢
0: 0xff������ �ֶ�'H'72',�Զ�A'65
1~2: �¡���
3~5: ʱ���֡���
6: 78��γ; 83��γ
7~10: �ȡ�����������С��1����С��2
11: 69����; 87����
12~15: �ȡ�����������С��1����С��2
*/
uchar C64_buf[16]={0};
     
void I2C_Init(void){ I2C_SDA = 1; I2C_SCL = 1;}

void I2C_Start(void) 
{ 
	I2C_SDA = 1; I2C_SCL = 1; _nus(5); 
 	I2C_SDA = 0; _nus(5); I2C_SCL = 0; _nus(5);
}

void I2C_Stop(void) 
{ 
	I2C_SDA = 0; I2C_SCL = 1; _nus(5);
	I2C_SDA = 1; _nus(5);
}

void I2C_SendAck(bit at_ack) //0Ack,1Nack
{ 
	I2C_SDA = at_ack; I2C_SCL = 1; _nus(5);
	I2C_SCL = 0; _nus(5);
}

bit I2C_Send_Byte(uchar at_snddat) 
{ 
	uchar i;
	bit R_ack;
	for (i=8; i>0; i--){    	
		if(at_snddat&0x80) I2C_SDA = 1;
		else I2C_SDA = 0;
        	I2C_SCL = 1; _nus(5); 
		I2C_SCL = 0; _nus(4); at_snddat<<=1;
    	}
	I2C_SDA = 1; I2C_SCL = 1; _nus(5);
	R_ack = I2C_SDA; 
	I2C_SCL =0; _nus(5);
	return R_ack; 
}

uchar I2C_Recv_Byte(void) 
{ 
	uchar i, at_recdat; 
	I2C_SDA = 1; 
	for(i=8;i>0;i--){ 
		at_recdat<<=1; I2C_SCL =1; _nus(5);		 
		if(I2C_SDA) at_recdat|=0x01; 
		I2C_SCL = 0; _nus(5);
	} 
	return at_recdat; 
} 

void AT24C64_WR(uint at_addr,uchar at_data) 
{ 
	I2C_Start(); 
	I2C_Send_Byte(0xa0); 
	I2C_Send_Byte(at_addr/256); I2C_Send_Byte(at_addr%256); 
	I2C_Send_Byte(at_data); 
	I2C_Stop(); _nms(5);
}

uchar AT24C64_RD(uint at_addr) 
{ 
	uchar at_data;
	I2C_Start();              
	I2C_Send_Byte(0xa0);        
	I2C_Send_Byte(at_addr/256); I2C_Send_Byte(at_addr%256); 
	I2C_Start();               
	I2C_Send_Byte(0xa1);      
	at_data = I2C_Recv_Byte(); 
	I2C_SendAck(1); I2C_Stop();
	return at_data;
}

void AT24C64_WrPage(uint at_addr) 
{ 
	uchar c;
	I2C_Start();               
	I2C_Send_Byte(0xa0);         
	I2C_Send_Byte(at_addr/256); I2C_Send_Byte(at_addr%256); 
	for(c=0;c<16;c++)
		I2C_Send_Byte(C64_buf[c]); 
	I2C_Stop(); _nms(5);          

} 

void AT24C64_RdPage(uint at_addr) 
{ 
	uchar c;
	I2C_Start();                     
	I2C_Send_Byte(0xa0);              
	I2C_Send_Byte(at_addr/256); I2C_Send_Byte(at_addr%256); 
	I2C_Start();                    
	I2C_Send_Byte(0xa1);           
	for(c=0;c<16;c++){
		C64_buf[c] = I2C_Recv_Byte();
		if(c==15) I2C_SendAck(1);     
		else I2C_SendAck(0);          
	}  	
	I2C_Stop();                     
}

