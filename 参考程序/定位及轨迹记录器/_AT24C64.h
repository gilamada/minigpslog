#include "_STC90.h"

sbit I2C_SDA = P1^5; //AT24C64数据
sbit I2C_SCL = P1^6; //AT24C64时钟
/*
16进制位存储
0: 0xff无数据 手动'H'72',自动A'65
1~2: 月、日
3~5: 时、分、秒
6: 78北纬; 83南纬
7~10: 度、分整数、分小数1、分小数2
11: 69东经; 87西经
12~15: 度、分整数、分小数1、分小数2
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

