#include <F:\移动硬盘\资料\电子资料\单片机\C\机房远程温度监测-LCD+GPS\H\Options.H>
#include <F:\移动硬盘\资料\电子资料\单片机\C\机房远程温度监测-LCD+GPS\H\LCD12864.H>
#include <F:\移动硬盘\资料\电子资料\单片机\C\机房远程温度监测-LCD+GPS\H\TC1.H>
#include <F:\移动硬盘\资料\电子资料\单片机\C\机房远程温度监测-LCD+GPS\H\UART.H>
#include <F:\移动硬盘\资料\电子资料\单片机\C\机房远程温度监测-LCD+GPS\H\ADC.H>


void ADC_Vin(void)
{
  unsigned long int ADC_temp = 0;
  uchar tab[10]={'0','1','2','3','4','5','6','7','8','9'};
  unsigned long int qian,bai,shi,ge;
  
  	ADC_temp = ReadADC();
	ADC_temp *= 10000;
	ADC_temp=(ADC_temp/1024)*2494*1;

	qian=ADC_temp/10000000;
	bai=ADC_temp%10000000/1000000;
	shi=ADC_temp%1000000/100000;
	ge=ADC_temp%100000/10000;
	
    LCD12864_gotoXY(2,0);
    LCD12864_sendstr("ADC:");
	LCD12864_sendbyte(iDat, tab[qian]);
	LCD12864_sendstr(".");
	LCD12864_sendbyte(iDat, tab[bai]);
	LCD12864_sendbyte(iDat, tab[shi]);
	LCD12864_sendbyte(iDat, tab[ge]);
    LCD12864_sendstr("V");
    Delayms(100);
}





/*******************************************
函数名称: main
功    能: 12864液晶驱动主程序
参    数: 无
返回值  : 无
/********************************************/
void main(void)
{
  unsigned long int speed_now=0,speed_tmp=0,speed_max=0;
  uchar speed_str[7],max_str[7]; //288.62\0
  uint i;

  
  Board_init();            //初始所有IO口
  DingShiQi_TC1();         //初始化定时器TC1
  Uart_init(9600);		   //初始化串口，设置波特率
 
  InitADC();               //初始化ADC
  
  LCD12864_initial(); 	  //初始化12864液晶
  LCD12864_clear();        //12864液晶清屏

  
 	while(1)
	{
    LCD12864_gotoXY(1,0);                                      //第一行 左
    LCD12864_sendstr("卫星:");
    LCD12864_sendstr(use_sat);       //使用的卫星数
	LCD12864_sendstr("/");
	LCD12864_sendstr(total_sat);     //天空中总卫星数

    LCD12864_gotoXY(1,6);                                      //第一行 右
    LCD12864_sendbyte(iDat, DW);     //模式
	LCD12864_sendstr("/");
    LCD12864_sendbyte(iDat, dingwei);//定位方式

	
    LCD12864_gotoXY(2,1);                                      //第二行 左
	LCD12864_sendstr("速度");
    LCD12864_gotoXY(2,5);                                      //第二行 右
	LCD12864_sendstr("高度");
	

    if(buf_full&0x08)  //如果GPRMC处理完毕
    {
	    //海里转换为公里
	    switch(speed_num){  //按速度的位数换算
	        case 0:
		        speed_now = 0;  //节转换为公里 x.xx
	            break;
	        case 4:
		        speed_now = ((speed[0]-0x30)*100 + (speed[2]-0x30)*10 + (speed[3]-0x30)) * 1.852;  //节转换为公里 x.xx
	            break;
	        case 5:
		        speed_now = ((speed[0]-0x30)*1000 + (speed[1]-0x30)*100 + (speed[3]-0x30)*10 + (speed[4]-0x30)) * 1.852;  //节转换为公里 xx.xx
		        break;
	        case 6:
		        speed_now = ((speed[0]-0x30)*10000 + (speed[1]-0x30)*1000 + (speed[2]-0x30)*100 + (speed[4]-0x30)*10 + (speed[5]-0x30)) * 1.852;  //节转换为公里 xxx.xx
	            break;
        }
		speed_tmp=speed_now;
        //海里转换为公里 end
		

		//实时速度数据处理
		for(i=0;i<7;i++)  //把 实时速度 数值从低位到高位依次存入数组，倒数第三位加"."，不足6为前面补0，最后一位补\0。
		{
		  if(i!=2 && i!=6){
		      speed_str[5-i]=(speed_tmp%10)+0x30;  //从低位往高依次取余并写入对应位置
		      speed_tmp/=10;
          }
		  else if(i==2){
		      speed_str[5-i]='.';
		  }
		  else if(i==6){
		      speed_str[6]='\0';
		  }
		}
		LCD12864_gotoXY(3,0);                                  //第三行 左侧
		LCD12864_sendstr(speed_str);       //实时速度KM
		LCD12864_sendstr("KM");
	    //实时速度数据处理 end

		
		//最快速度数据处理
		if(speed_now>speed_max)  //比较速度大小
		{
		    speed_max=speed_now;  //记录最大速度
		}
		speed_tmp=speed_max;  //最大速度赋值给临时变量用于计算
		for(i=0;i<7;i++)    //把 最高速度 数值从低位到高位依次存入数组，倒数第三位加"."，不足6为前面补0，最后一位补\0。
		{
		  if(i!=2 && i!=6){
		      max_str[5-i]=(speed_tmp%10)+0x30;
		      speed_tmp/=10;
          }
		  else if(i==2){
		      max_str[5-i]='.';
		  }
		  else if(i==6){
		      max_str[6]='\0';
		  }
		}
		LCD12864_gotoXY(4,0);                                  //第四行 左侧
        LCD12864_sendstr(max_str);       //最大速度KM
		LCD12864_sendstr("KM");
		//最快速度数据处理 end
		
		
		//for(i=1;speed_tmp2/=10;i++); //去整数位数
		Uart_sentstr("实时速度:");
	    Uart_sentstr(speed_str);
		Uart_sentstr("\r\n");

		Uart_sentstr("最高速度:");
	    Uart_sentstr(max_str);
		Uart_sentstr("\r\n");
		Uart_sentstr("\r\n");
		
	    buf_full&=~0x08;
    }//如果GPRMC处理完毕 end
	
	
	
    if(buf_full&0x01)  //如果GPGGA处理完毕
    {
        LCD12864_gotoXY(4,4);  
	
	
	
	
	
	
	
	
	
	
	
        LCD12864_gotoXY(4,4);                                  //第三行 右侧
        LCD12864_sendstr(high);          //高度M
		LCD12864_sendstr("M");
	
	
	    buf_full&=~0x01;
    }//如果GPGGA处理完毕 end
    

	}//end while
}