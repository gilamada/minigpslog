#include "_STC90.h"
#include "_GPS.h"

sbit C_LED = P0^4; //数据通讯指示
sbit Key0 = P3^3;
sbit Key1 = P1^2;
sbit Key2 = P1^0;

bit flg_gsv=0;
bit flg_mode=0;  
bit flg_upload=0; 
bit flg_rx=0;
bit flg_rxf=0;
uchar flg_rmcgga=0;      //1rmc 2gga

void get_gpsmode(void)
{
	if(!flg_mode){
		if(buf_gps[3]=='M' && buf_gps[4]=='C') flg_rmcgga = 1;     //RMC
		else{
			if(buf_gps[3]=='G' && buf_gps[4]=='A') flg_rmcgga = 2; //GGA
			else flg_rx = 0; 
		}
	}
	else{
		if(buf_gps[3]=='S' && buf_gps[4]=='V') flg_rmcgga = 3; //GGA
		else flg_rx = 0;
	}
}

void UART_RX(void) interrupt 4
{ 
	uchar tp_rx;
	while(!RI); 
	tp_rx = SBUF; 
	if(!flg_rxf){
		if(flg_rx){                       
			buf_gps[count_gps] = tp_rx;  
			if(count_gps==4) get_gpsmode(); 	
			if(tp_rx=='*'){ flg_rx = 0; flg_rxf = 1;} 
			else count_gps++;             
		}
		else{                          
			if(tp_rx=='$'){ flg_rx = 1; count_gps = 0; flg_rmcgga = 0;}
		}
	}
	if(tp_rx=='#') flg_upload = 1;
	RI = 0;
}

void UART_TX(uchar tp_tx)
{	
	SBUF = tp_tx;
	while(!TI); 
	TI = 0; _nus(100);
}

uchar var_js7=36;
uchar var_js5=0;
uchar var_js4=0;
uint var_jsrec=0;
//60ms
void TIMER2_SEV(void) interrupt 5
{
	TF2 = 0;
	if(var_js7>0) var_js7--;
	if(var_js5>0) var_js5--;
	if(var_js4>0) var_js4--;
	if(var_jsrec>0) var_jsrec--;
}

void initial(void)
{
	C_LED  = 0;
	GPS_EN = 0; 
	EA = 0; 	
	AUXR = 0x01;
	IPH = 0x0c;	IP = 0x14;
	SCON = 0x50;	
	PCON = 0x00; 
	RI = 0;	TI = 0; ES = 1;

	TCON = 0x00;
	TMOD = 0x21;
	TH1 = 256-6; TL1 = 256-6; ET1 = 0; TR1 = 1; 
	T2CON = 0x00; T2MOD = 0x00;
	RCAP2H = (65536-55296)/256; RCAP2L = (65536-55296)%256;
	ET2 = 1; TR2 = 1;
	I2C_Init();
	L5_init();
	L5_wrstr_cn(24,2,0,4,4); //卫星定位
	L5_wrstr_cn(24,4,4,4,4); //轨迹记录
	EA = 1;	
	while(var_js7>0);
	Lgps_disp_init(); 	           
	C_LED = 1; 
}

void Key_deal(void)
{
	uchar jk=255;
	C_LED = 0; EA = 0; flg_rx = 0; flg_rxf = 0;
	if(!Key0){
		do{ _nms(8); jk--;}while(!Key0 && jk>0);
		if(jk>0){
			flg_mode = !flg_mode;
			L5_setxy(0,1); ofs = 0; line = 0x00; Lwr_icob(336);
			if(!flg_mode) Lgps_disp_dt();				
		}
		else{ 
			flg_sata = !flg_sata; L5_SATA();
			GPS_EN = flg_sata; 
		}
		do{	while(!Key0); _nms(5);}while(!Key0);
	}
	if(!Key1){
		do{ _nms(20); jk--;}while(!Key1 && jk>0);
		if(jk>0){ at_onewr('H'); var_js4 = 10;} 
		else at_format(); 			
		do{	while(!Key1); _nms(5);}while(!Key1);
	}
	if(!Key2){
		do{ _nms(8); jk--;}while(!Key2 && jk>0);
		if(jk>0) L5_LED = !L5_LED;
		else{
			while(!Key2){
				rec_add();
				_nms(255); _nms(255); _nms(255); _nms(255);
			}
			var_jsrec = var_rec*50; var_jsrec/=3;
		}
	}
	C_LED = 1; EA = 1;
}


void main(void) 
{
	initial();
	while(1){
		if(flg_rxf){
			if(flg_sign) C_LED = 0;
			L5_ComData(1); var_js5 = 6;				
			get_cama();  
			if(!flg_mode){
				if(flg_rmcgga==1) L5_RMC();
				else if(flg_rmcgga==2) L5_GGA();
			}
			else{
				if(flg_rmcgga==3){ 
					if(!flg_gsv){ 
						L5_GSV();
						if(buf_gps[8]=='3') flg_gsv = 1;
					}
					else{
						if(buf_gps[8]=='2'){ L5_GSV(); flg_gsv = 0;}
					}
				}
			}		
			for(count_gps=0;count_gps<96;count_gps++) 
				buf_gps[count_gps]=0;
			C_LED = 1; flg_rxf = 0;
		}
		if(var_js5==1){ L5_ComData(0); var_js5 = 0;}
		if(var_js7==0){	L5_BAT(); var_js7 = 16;}
		if(!(Key0&Key1&Key2)){ _nms(5); Key_deal();}
		if(var_js4==1){ L5_AtWr(0); var_js4 = 0;}
		if(!flg_sata){
			if(var_rec>0){
				if(var_jsrec==0){
					at_onewr('A'); var_js4 = 10; 
					var_jsrec = var_rec*50; var_jsrec/=3;
				}
			}
		}
		if(flg_upload){
			if(flg_sata){
				uint k; uchar r;
				C_LED = 0; EA = 0; flg_rx = 0; flg_rxf = 0;
				for(k=0;k<at_id;k++){
					AT24C64_RdPage(k*16);
					UART_TX(k/256); UART_TX(k%256); 
					for(r=0;r<16;r++) UART_TX(C64_buf[r]);				
					L5_sch(1,1,k,at_id);
				}			
				EA = 1; L5_sch(0,0,0,0); C_LED = 1; 
			}
			flg_upload = 0;
		}
	}
}
