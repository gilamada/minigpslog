#include "_STC90.h"
#include "_LCD5110.h"
#include "_AT24C64.h"

#define Bat_full   10 //电量满
#define Bat_empt   11 //电量空
#define Signal     12 //信号
#define NoSignal   13 //信号
#define file       14 //文件
#define Sata_run   15 //状态运行
#define Sata_pause 16 //状态暂停
#define Ico_gps    17 //GPS
#define Ico_com    18 //数据通讯
#define Ico_del    19 //删除
#define Ico_upload 20 //上传
#define Ico_wait   21 //等待
#define chr_point  22 //大'.'
#define chr_degree 23 //度
#define chr_min    24 //分
#define Ico_point  25 //小度
#define Ico_degree 26 //小度

sbit Bat_Low = P1^4; //电池0低
sbit GPS_EN = P3^5;  //GPS使能

uchar buf_gps[96]={0};
uchar count_gps=0;
bit flg_sign=0;      
uchar var_seg[20]={0};
uchar ofs=0;          
uchar line=0x00; 

void get_cama(void)
{
 	uchar tp_c=0,cj;
	for(cj=0;cj<20;cj++) var_seg[cj] = 0;
	for(cj=0;cj<count_gps;cj++)
		if(buf_gps[cj]==','){ var_seg[tp_c] = cj; tp_c++;}
}

void Lwr_icob(uint nb){ for(;nb>0;nb--) L5_wrbyte(1,line);}

void Lwr_ico(uchar ic)
{
	uchar i0,i1;
 	switch(ic){
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			 	  i0 = (ic-48)*4; i1 = i0+4; break; 
		case 10: i0 = 65;  i1 = 75;  break; //电量满
		case 11: i0 = 75;  i1 = 85;  break; //电量空
		case 12: i0 = 40;  i1 = 50;  break; //信号有
		case 13: i0 = 50;  i1 = 60;  break; //信号无
		case 14: i0 = 60;  i1 = 65;  break; //文件
		case 15: i0 = 137; i1 = 143; break; //运行图标
		case 16: i0 = 143; i1 = 149; break; //暂停图标				
		case 17: i0 = 85;  i1 = 105; break; //GPS
		case 18: i0 = 164; i1 = 173; break; //数据传输
		case 19: i0 = 173; i1 = 180; break; //删除×
		case 20: i0 = 180; i1 = 187; break; //上传
		case 21: i0 = 187; i1 = 194; break; //等待
		case 'D': i0 = 131; i1 = 137; break; //'D'
		case ':': i0 = 115; i1 = 117; break; //':'
		case '/': i0 = 105; i1 = 109; break; //'/'
		case '.': i0 = 109; i1 = 111; break; //'.'
		case '-': i0 = 111; i1 = 115; break; //'-'
		case '%': i0 = 194; i1 = 202; break; //'%'
		case 'k': i0 = 117; i1 = 131; break; //'kmh'
		case 'm': i0 = 121; i1 = 127; break; //'m'
		case 's': i0 = 159; i1 = 164; break; //'s'
		case 'A': i0 = 202; i1 = 207; break; //'A'
		case 'H': i0 = 207; i1 = 212; break; //'H'
		case 'N': i0 = 212; i1 = 217; break; //'N'
		case 'S': i0 = 217; i1 = 222; break; //'S'
		case 'E': i0 = 222; i1 = 227; break; //'E'
		case 'W': i0 = 227; i1 = 232; break; //'N'		
		case 22: i0 = 149; i1 = 152; break; //大'.'
		case 23: i0 = 152; i1 = 156; break; // °
		case 24: i0 = 156; i1 = 159; break; //'
		case 25: i0 = 232; i1 = 235; break; //中点
		case 26: i0 = 235; i1 = 238; break; //小度
		default: i0 = ic*4; i1 = i0+4; break; //数字
	}
	for(;i0<i1;i0++) L5_wrbyte(1,(dip_ico[i0]<<ofs)|line);
}

void Lwr_icos(uchar cs,uchar cl)
{
 	uchar k;
	for(k=0;k<cl;k++) Lwr_ico(buf_gps[cs+k]);
}

void Lwr_icoh(uchar *hs){ while(*hs){ Lwr_ico(*hs); hs++;}}

void L5_wr_chr(uchar nc)
{
	uchar l,n;
	switch(nc){
		case 'N': n = 19; break;
		case 'S': n = 22; break;
		case 'E': n = 16; break;
		case 'W': n = 24; break;
		case 'H': n = 18; break;
		case 'A': n = 14; break;
		case 'G': n = 17; break;
		case 'P': n = 20; break;
		case 'R': n = 21; break;
		case 'M': n = 18; break;
		case 'C': n = 15; break;
		case ',': n = 11; break;
		case '.': n = 13; break;
		case '-': n = 12; break;
		case '*': n = 10; break;		
		case 'V': n = 23; break;		
		default: n = nc-48; break;
	}
	for(l=0;l<6;l++) L5_wrbyte(1,ASCII326[n][l]);
}

void L5_wr_chrs(uchar ch,uchar lc)
{
 	uchar k;
	for(k=0;k<lc;k++) L5_wr_chr(buf_gps[ch+k]);
}

void L5_wr_chrh(uchar *hr){	while(*hr){ L5_wr_chr(*hr); hr++;}}

uchar chrtodec(uchar cd){return buf_gps[cd]-48;}
uchar chr_dec(uchar cf){ return chrtodec(cf)*10+chrtodec(cf+1);}
/void Lwr_dec(uchar dec){ Lwr_ico(dec/10); Lwr_ico(dec%10);}

void L5_RMC(void)
{	
	uchar tp_h,tp_y,tp_m,tp_d; 
	bit flg_d=0;              
 	L5_setxy(56,5);	ofs = 3; line = 0x02;
	tp_h = chr_dec(6)+8;
	if(tp_h>23){ tp_h%=24; flg_d = 1;}   
	Lwr_dec(tp_h); Lwr_ico(':');              
	Lwr_icos(8,2); Lwr_ico(':'); Lwr_icos(10,2);
	L5_CE = 1;
	C64_buf[3] = tp_h;
	C64_buf[4] = chr_dec(8); C64_buf[5] = chr_dec(10); 

	L5_setxy(32,5);
	tp_m = chr_dec(var_seg[8]+3);          
	tp_d = chr_dec(var_seg[8]+1); 
	if(flg_d){
		switch(tp_m){
			case 4: case 6: case 9: case 11:
					tp_h = 30; break;
			case 2: tp_y = chr_dec(var_seg[8]+5);
			 		if(tp_y%4==0) tp_h = 29;
					else tp_h = 28;
					break;
			default: tp_h = 31; break;         
		}		
		if(tp_d<tp_h) tp_d++;
		else{
			tp_d = 1;
			if(tp_m<12) tp_m++;
			else tp_m = 1;
		}
		Lwr_dec(tp_m); Lwr_ico('-'); Lwr_dec(tp_d); 		
	}
	else{
		Lwr_icos(var_seg[8]+3,2); Lwr_ico('-');   
		Lwr_icos(var_seg[8]+1,2);                   
	}
	L5_CE = 1;
	C64_buf[1] = tp_m; C64_buf[2] = tp_d;	
	
	L5_setxy(0,0); ofs = 0; line = 0x40;
	if(buf_gps[17]=='A'){ Lwr_ico(Signal); flg_sign = 1;} 
	else{ Lwr_ico(NoSignal); flg_sign = 0;}               
	L5_CE = 1;

	if((var_seg[4]-var_seg[2])==12){
		L5_setxy(0,3); ofs = 0; line = 0x00; 
		L5_wr_chr(buf_gps[29]);	
		L5_setxy(12,3);						  
		L5_wr_chrs(19,2); Lwr_ico(chr_degree);
		L5_wr_chrs(21,2); Lwr_ico(chr_point);
		L5_wr_chrs(24,4);                     
		L5_CE = 1;
		if(buf_gps[29]=='N') C64_buf[6] = 0x00; //N78 S83
		else C64_buf[6] = 0x10;
		C64_buf[7] = chr_dec(19); C64_buf[8] = chr_dec(21);
		C64_buf[9] = chr_dec(24); C64_buf[10] = chr_dec(26);
	}
	if(var_seg[6]>41){ 
		L5_setxy(0,4); 
		tp_y = buf_gps[var_seg[5]+1]; L5_wr_chr(tp_y); //E/W	  	
		tp_h = chr_dec(31);
		if(var_seg[5]>40){	      
			L5_wr_chrs(31,3);
			tp_h = tp_h*10+chrtodec(33);
		}
		else{ Lwr_icob(6); L5_wr_chrs(31,2);} 
		Lwr_ico(chr_degree);                           
		L5_wr_chrs(var_seg[5]-7,2); Lwr_ico(chr_point); 
		L5_wr_chrs(var_seg[5]-4,4);	 
		L5_CE = 1;
		if(tp_y=='E') C64_buf[6]&=0xf0;     //E69 W87
		else C64_buf[6]|=0x01;
		C64_buf[11] = tp_h; C64_buf[12] = chr_dec(var_seg[5]-7);
		C64_buf[13] = chr_dec(var_seg[5]-4); C64_buf[14] = chr_dec(var_seg[5]-2);		
	}

	L5_setxy(52,2); 
	tp_h = var_seg[7]-var_seg[6];    
	if(tp_h>4){		
		uint tp_v=0;
		if(tp_h==7) tp_y = 6;        
		else tp_y = tp_h;
		for(tp_m=1;tp_m<tp_y;tp_m++){
			if(tp_m==3) tp_m++;
			tp_v = tp_v+chrtodec(var_seg[7]-tp_m)*185;
			if(tp_m<5){
				if(tp_v%10>4) tp_v = tp_v/10+1;
				else tp_v/=10;
			}
		}
		if(tp_h==7) tp_v = tp_v+chrtodec(var_seg[6]+1)*1850;
		if(tp_v<100) Lwr_icob(8);    
		else{
			if(tp_v<1000) Lwr_icob(4);
			else{ Lwr_ico(tp_v/1000); tp_v%=1000;}
			Lwr_ico(tp_v/100); tp_v%=100;
		}
		Lwr_ico(tp_v/10); Lwr_ico('.'); Lwr_ico(tp_v%10);
	}
	else Lwr_icoh("---.-"); //Lwr_ico('k'); -.-kmh
	L5_CE = 1;

	L5_setxy(24,2); 
	tp_h = var_seg[8]-var_seg[7];   
	if(tp_h>4){
		uint tp_dg=0;
		tp_h-=4;
		for(tp_y=0;tp_y<tp_h;tp_y++)
			tp_dg = tp_dg*10+buf_gps[var_seg[7]+1+tp_y]-48;
		if(tp_dg>90 && tp_dg<270){
			Lwr_ico('S'); Lwr_ico(Ico_point);
			if(tp_dg<180){ Lwr_ico('E'); tp_dg = 180-tp_dg;}
			else{ Lwr_ico('W'); tp_dg = tp_dg-180;} 
		}
		else{
			Lwr_ico('N'); Lwr_ico(Ico_point); 
			if(tp_dg<180) Lwr_ico('E');
			else{ Lwr_ico('W'); tp_dg = 360-tp_dg;} 
		}
		Lwr_dec(tp_dg);	Lwr_ico(Ico_degree); 
	}
	else Lwr_icob(24);
	L5_CE = 1;		
}


void L5_GGA(void)
{
	uchar tp_h;	
	L5_setxy(10,0); ofs = 0; line = 0x40;
	Lwr_icos(var_seg[6]+1,2);	
	L5_CE = 1;

 	L5_setxy(74,4); ofs = 0; line = 0x00;
	Lwr_ico(buf_gps[var_seg[5]+1]); 
	L5_CE = 1;

	L5_setxy(74,3);
	if((var_seg[8]-var_seg[7])==4){
		Lwr_icos(var_seg[7]+1,3);
		tp_h = chrtodec(var_seg[7]+1)*10 +chrtodec(var_seg[7]+3);
	}
	else{ Lwr_icoh("-.-"); tp_h = 0;}   
	L5_CE = 1;
	C64_buf[15] = tp_h;

	L5_setxy(56,1);
	tp_h = var_seg[9]-var_seg[8]; 
	if(tp_h>3){
		Lwr_icob(28-tp_h*4);        
		Lwr_icos(var_seg[8]+1,tp_h-1); 
	}
	else Lwr_icoh("----.-"); 	
	L5_CE = 1;
}

bit flg_lowbat=0;
void L5_BAT(void)
{
	L5_setxy(74,0); ofs = 0; line = 0x40;
	if(!Bat_Low){
		if(flg_lowbat) Lwr_icob(10);
		else Lwr_ico(Bat_empt);
		flg_lowbat = !flg_lowbat;
	}
	else Lwr_ico(Bat_full); 
	L5_CE = 1;
}

bit flg_sata=0;
void L5_SATA(void)
{
	L5_setxy(22,5); ofs = 0; line = 0x02;
	if(!flg_sata) Lwr_ico(Sata_run);         
	else{ Lwr_ico(Sata_pause); flg_sign = 0;} 
	L5_CE = 1;	
}

void L5_ComData(bit tp_cd)
{
	L5_setxy(20,0); ofs = 0; line = 0x40;
	if(tp_cd) Lwr_ico(Ico_com); 
	else Lwr_icob(10);
	L5_CE = 1;				
}

void L5_sch(bit sen,uchar stype,uint s0,uint s1)
{
	L5_setxy(0,1); ofs = 0; line = 0x00;
	if(sen){
		Lwr_ico(file);
		if(stype==0) Lwr_ico(Ico_wait);
		else{
			if(stype==1) Lwr_ico(Ico_upload);
			else Lwr_ico(Ico_del);
		}
		s0 = s0*100/s1; s1 = s0/5; //20份'%'
		if(s0<10) Lwr_icob(8); 
		else{
			if(s0<100){Lwr_icob(4); Lwr_ico(s0/10);}
			else Lwr_dec(s0/10);
		}
		Lwr_ico(s0%10); Lwr_ico('%');		
		line = 0x1f; Lwr_icob(1+s1); line = 0x11;
		Lwr_icob(20-s1); line = 0x1f; Lwr_icob(1);
	}
	else{ _nms(255); _nms(255); _nms(255); Lwr_icob(54);}
	L5_CE = 1;
}

uint at_id=0; 
void L5_FileN(void)
{
	L5_setxy(42,0); ofs = 0; line = 0x40;
	Lwr_dec(at_id/10); Lwr_ico(at_id%10); 
	L5_CE = 1;
}

void at_getid(void)
{
	uchar tp_r;
	at_id = 0;
	do{
		tp_r = AT24C64_RD(at_id*16);
		if(tp_r==0xff) break;
		at_id++;
		L5_sch(1,0,at_id,512);  
	}while(at_id<512);
	L5_FileN();	L5_sch(0,0,0,0);
}

void at_format(void)
{
	uint tp_f;
	if(flg_sata){                          
		if(at_id>0){
			for(tp_f=0;tp_f<at_id;tp_f++){
				AT24C64_WR(tp_f*16,0xff);	
				L5_sch(1,2,tp_f,at_id); 
			}
			at_id = 0; L5_FileN(); L5_sch(0,0,0,0);
		}
	}
}

void L5_AtWr(uchar wen)
{
	L5_setxy(30,0); ofs = 0; line = 0x40;
	if(wen==1) Lwr_ico(Ico_wait);
	else{
		if(wen==2) Lwr_ico(Ico_del);
		else Lwr_icob(7);
	} 
	L5_CE = 1;
}

void at_onewr(uchar hdat)
{
	if(flg_sign && at_id<512){	
		L5_AtWr(1);
		C64_buf[0] = hdat;  
		AT24C64_WrPage(at_id*16); 
		at_id++; L5_FileN();
	}
	else L5_AtWr(2);
}

uchar var_rec=0;
void L5_recty(void)
{
	L5_setxy(0,2); ofs = 0; 
	if(var_rec==0){ line = 0x00; Lwr_icob(6); Lwr_ico('H'); Lwr_icob(7);}
	else{ line = 0x20; Lwr_ico('A'); Lwr_dec(var_rec); Lwr_ico('s');}
	L5_CE = 1; 
}

void rec_add(void)
{
	switch(var_rec){
		case 0: var_rec = 90; break;
		case 90: var_rec = 60; break;
		case 60: var_rec = 30; break;
		case 30: var_rec = 10; break;
		default: var_rec = 0; break;
	}
	L5_recty(); 
}


void Lgps_disp_dt(void)
{
	//第1行	
	L5_setxy(56,1); ofs = 0; line = 0x00; Lwr_icoh("----.-m"); L5_CE = 1;	
	//第2行
	L5_setxy(52,2); Lwr_icoh("---.-k");  L5_CE = 1;
	//第3行
	L5_setxy(0,3); L5_wr_chr('N');
	L5_setxy(12,3); L5_wr_chrh("--"); Lwr_ico(chr_degree); L5_wr_chrh("--"); 
	Lwr_ico(chr_point); L5_wr_chrh("----"); Lwr_ico(chr_min); L5_CE = 1;
	L5_setxy(74,3); Lwr_icoh("-.-"); L5_CE = 1;
	//第4行
	L5_setxy(0,4); L5_wr_chrh("E---"); Lwr_ico(chr_degree); L5_wr_chrh("--");
	Lwr_ico(chr_point);	L5_wr_chrh("----"); Lwr_ico(chr_min); L5_CE = 1;
	L5_setxy(74,4); Lwr_icoh("0D");	L5_CE = 1;
	//第2行
	L5_recty();
}

void Lgps_disp_init(void)
{
	L5_clear();
	L5_setxy(0,0); ofs = 0; line = 0x40;
	Lwr_ico(NoSignal); Lwr_icoh("00"); Lwr_icob(19);
	Lwr_ico(file); Lwr_icoh("000/512"); Lwr_icob(4);
	Lwr_ico(Bat_empt); L5_CE = 1;
	Lgps_disp_dt();
	L5_setxy(0,5); ofs = 0; line = 0x02; Lwr_ico(Ico_gps); 
	Lwr_icob(2); Lwr_ico(Sata_run);	Lwr_icob(4); ofs = 3;
	Lwr_icoh("00-00"); Lwr_icob(4); Lwr_icoh("--:--:--");
	L5_CE = 1;
	at_getid();
}

unsigned char code g_sign[] = {0x00,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff};
void L5_GSV(void)
{				  	
	uchar tp_j,tp_jk,tp_g,tp_n;
	tp_n = buf_gps[8]-48; 
	ofs = 2; line = 0x01; 
	for(tp_j=0;tp_j<4;tp_j++){
		if(tp_n==1) L5_setxy(tp_j*14,2);
		else{
			if(tp_n==2){
				if(tp_j<2) L5_setxy((tp_j+4)*14,2);
				else L5_setxy((tp_j-2)*14,4);
			}
			else L5_setxy((tp_j+2)*14,4);
		}
		Lwr_icob(2);
		tp_jk = tp_j*4+3;
		if(var_seg[tp_jk+1]>(var_seg[tp_jk]+1)) Lwr_icos(var_seg[tp_jk]+1,2);
		else Lwr_icoh("--");
		Lwr_icob(4);
	}
	for(tp_j=0;tp_j<4;tp_j++){
		tp_jk = tp_j*4+6;
		if(var_seg[tp_jk+1]>(var_seg[tp_jk]+1)) tp_g = chr_dec(var_seg[tp_jk]+1);
		else tp_g = 0;
		if(tp_n==1) L5_setxy(tp_j*14,1);
		else{
			if(tp_n==2){
				if(tp_j<2) L5_setxy((tp_j+4)*14,1);
				else L5_setxy((tp_j-2)*14,3);
			}
			else L5_setxy((tp_j+2)*14,3);
		}		
		line = g_sign[tp_g/12]; Lwr_icob(4);
		line = 0x00; Lwr_dec(tp_g);
	}
	L5_CE = 1;
}
