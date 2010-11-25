//GPS数据存储数组
unsigned char JD[10]={"-----.----"}; //经度
unsigned char JD_a='-';			     //经度方向
unsigned char WD[9]={"----.----"};	 //纬度
unsigned char WD_a='-';			     //纬度方向

unsigned char date_y[2]={"--"};  //年
unsigned char date_m[2]={"--"};  //月
unsigned char date_d[2]={"--"};  //日
unsigned char time_s[2]={"--"};  //时
unsigned char time_f[2]={"--"};  //分
unsigned char time_m[2]={"--"};  //秒

unsigned char speed[7]={"------"};   //速度 x.xx\xx.xx\xxx.xx   节＝海里/小时,相当于1.852KM/H
unsigned char high[8]={"-------"};	 //高度 xx.x\xxx.x\-xxxx.x
unsigned char angle[5];		         //方位角
unsigned char use_sat[3]={"--"};	 //使用的卫星数
unsigned char total_sat[3]={"--"};	 //天空中总卫星数
unsigned char DW='-';                //定位标志 V未定位 A已定位
unsigned char dingwei='-';           //定位型式 1 = 未定位， 2 = 二维定位， 3 = 三维定位。 


//串口中断需要的变量
unsigned char seg_count;		     //逗号计数器
unsigned char byte_count;		     //位数计数器
unsigned char cmd_number;		     //命令类型
unsigned char mode;				     //0：结束模式，1：命令模式，2：数据模式
unsigned char buf_full;              //1：整句接收完成，相应数据有效。0：缓存数据无效。
unsigned char cmd[5];			     //命令类型存储数组


//显示需要的变量
unsigned char dsp_count;		     //刷新次数计数器
unsigned char time_count;

volatile unsigned char high_num,speed_num;


/*******************************************
函数名称: Get_Data
功    能: GPS报文解码
参    数: 无
返回值  : 无
/********************************************/
void Get_Data(void)
{
 uchar tmp;
 tmp=UDR;
 
	switch(tmp){
		case '$':
			cmd_number=0;		//命令类型清空
			mode=1;				//接收命令模式
			byte_count=0;		//接收位数清空
			break;
		case ',':
			seg_count++;		//逗号计数加1
			byte_count=0;
			break;
		case '*':
			switch(cmd_number){
				case 1:
					buf_full|=0x01; //GPGGA
					break;
				case 2:
					buf_full|=0x02; //GPGSV
					break;
				case 3:
					buf_full|=0x04; //GPGSA
					break;
				case 4:
					buf_full|=0x08; //GPRMC
					break;
			}
			mode=0;
			break;
		default:
			if(mode==1){
				//命令种类判断 0：结束模式，1：命令模式，2：数据模式
				cmd[byte_count]=tmp;			//接收字符放入类型缓存
				if(byte_count>=4){				//如果类型数据接收完毕，判断类型
					if(cmd[0]=='G'){
						if(cmd[1]=='P'){
							if(cmd[2]=='G'){
								if(cmd[3]=='G'){
									if(cmd[4]=='A'){  //GPGGA GPS定位信息
										cmd_number=1;
										mode=2;
										seg_count=0;
										byte_count=0;
										high_num=0;
									}
								}
								else if(cmd[3]=='S'){
									if(cmd[4]=='V'){  //GPGSV 可见卫星信息
										cmd_number=2;
										mode=2;
										seg_count=0;
										byte_count=0;
										
									}
									else if(cmd[4]=='A'){  //GPGSA 当前卫星信息
										cmd_number=3;
										mode=2;
										seg_count=0;
										byte_count=0;
									}
								}
							}
							else if(cmd[2]=='R'){     //GPRMC 推荐定位信息
								if(cmd[3]=='M'){
									if(cmd[4]=='C'){
										cmd_number=4;
										mode=2;
										seg_count=0;
										byte_count=0;
										speed_num=0;
									}
								}
							}
						}
					}
				}
			}
			else if(mode==2){
				//接收数据处理
				switch (cmd_number){
/*********************************************************************************************
$GPGGA,121252.000,3937.3032,N,11611.6046,E,1,05,2.0,45.9,M,-5.7,M,,0000*77
<1> UTC时间，hhmmss（时分秒）格式 
<2> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输） 
<3> 纬度半球N（北半球）或S（南半球） 
<4> 经度dddmm.mmmm（度分）格式（前面的0也将被传输） 
<5> 经度半球E（东经）或W（西经） 
<6> GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算 
<7> 正在使用解算位置的卫星数量（00~12）（前面的0也将被传输） 
<8> HDOP水平精度因子（0.5~99.9） 
<9> 海拔高度（-9999.9~99999.9） 
<10> 地球椭球面相对大地水准面的高度 
<11> 差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空） 
<12> 差分站ID号0000~1023（前面的0也将被传输，如果不是差分定位将为空）
***********************************************************************************************/
					case 1:				//类型1数据接收。GPGGA
						switch(seg_count){
							case 2:								//纬度处理
								if(byte_count<9){
									WD[byte_count]=tmp;
								}
								break;
							case 3:								//纬度方向处理
								if(byte_count<1){
									WD_a=tmp;
								}
								break;
							case 4:								//经度处理
								if(byte_count<10){
									JD[byte_count]=tmp;
								}
								break;
							case 5:								//经度方向处理
								if(byte_count<1){
									JD_a=tmp;
								}
								break;
							case 7:								//定位使用的卫星数
								if(byte_count<2){
									use_sat[byte_count]=tmp;
								}
								break;
							case 9:								//高度处理
								if(byte_count<6){
									high[byte_count]=tmp;
									high_num++;
								}
								break;
						}
						break;
						
/*********************************************************************************************
$GPGSV,3,1,12,16,65,013,43,06,48,143,,03,42,167,,23,39,323,49*74
1) 天空中收到讯号的卫星总数。
2) 定位的卫星总数。
3) 天空中的卫星总数，00 至 12。
4) 卫星编号， 01 至 32。
5) 卫星仰角， OO 至 90 度。
6) 卫星方位角， OOO 至 359 度。实际值。
7) 讯号噪声比（C/No）， 00 至 99 dB；无表未接收到讯号。
8) Checksum.(检查位).
**********************************************************************************************/	
					case 2:				//类型2数据接收。GPGSV
						switch(seg_count){
							case 3:								//天空中的卫星总数
								if(byte_count<2){
									total_sat[byte_count]=tmp;
								}
								break;
						}
						break;

/*********************************************************************************************
$GPGSA,A,3,14,15,05,22,18,26,,,,,,,2.1,1.2,1.7*3D
$GPGSA,<1>,<2>,<3>,<3>,,,,,<3>,<3>,<3>,<4>,<5>,<6>,<7><CR><LF> 
$GPGSA,A,3,16,23,31,32,14,,,,,,,,1.53,1.20,0.95*0A
<1> 模式 ：M = 手动， A = 自动。 
<2> 定位型式 1 = 未定位， 2 = 二维定位， 3 = 三维定位。 
<3> PRN 数字：01 至 32 表天空使用中的卫星编号，最多可接收12颗卫星信息。 
<4> PDOP位置精度因子（0.5~99.9） 
<5> HDOP水平精度因子（0.5~99.9） 
<6> VDOP垂直精度因子（0.5~99.9） 
<7> Checksum.(检查位). 	
**********************************************************************************************/	
					case 3:				//类型2数据接收。GPGSA
						switch(seg_count){
							case 2:								//定位形式
								if(byte_count<1){
									dingwei=tmp;
								}
								break;
						}
						break;
						
/*********************************************************************************************
$GPRMC,050458.000,A,2459.8991,N,10242.8698,E,0.37,130.03,190808,,,A*66 
<1> UTC时间，hhmmss（时分秒）格式 
<2> 定位状态，A=有效定位，V=无效定位 
<3> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输） 
<4> 纬度半球N（北半球）或S（南半球） 
<5> 经度dddmm.mmmm（度分）格式（前面的0也将被传输） 
<6> 经度半球E（东经）或W（西经） 
<7> 地面速率（000.0~999.9节，前面的0也将被传输） 
<8> 地面航向（000.0~359.9度，以真北为参考基准，前面的0也将被传输） 
<9> UTC日期，ddmmyy（日月年）格式 
<10> 磁偏角（000.0~180.0度，前面的0也将被传输） 
<11> 磁偏角方向，E（东）或W（西） 
**********************************************************************************************/
					case 4:				//类型3数据接收。GPRMC
						switch(seg_count){
                            case 1:                             //时间处理
                                if(byte_count<2){    //时
                                    time_s[byte_count]=tmp; 
                                break;
                                }
                                if(byte_count<4){    //分
                                    time_f[byte_count-2]=tmp; 
                                break;
                                }
                                if(byte_count<6){    //秒
                                    time_m[byte_count-4]=tmp; 
                                }
                                break;
                            case 2 :                            //定位标志
                                if(byte_count<1){
                                    DW=tmp;
                                }
                                break;
							case 7:								//速度处理
								if(byte_count<6){
									speed[byte_count]=tmp;
									speed_num++;
								}
								break;
							case 8:								//方位角处理
								if(byte_count<5){
									angle[byte_count]=tmp;
								}
								break;
                            case 9:                             //时间处理
                                if(byte_count<2){    //日
                                    date_d[byte_count]=tmp; 
                                break;
                                }
                                if(byte_count<4){    //月
                                    date_m[byte_count-2]=tmp; 
                                break;
                                }
                                if(byte_count<6){    //年
                                    date_y[byte_count-4]=tmp;
                                break;
                                }
                                break;
						}
						break;
				}//switch (cmd_number) end
			}//if(mode==1) end
			
			
			byte_count++;		//接收数位加1
			break;
	}//switch(tmp) end
}