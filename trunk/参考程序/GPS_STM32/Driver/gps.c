//=====================================================================
// 文件名称: GPS.C
// 芯片类型: STM32F103VCT6                                           
// 开发软件: Keil, Windows XP2 
// 创建日期: 2010年09月06日                                                               
// 程序功能: GPS驱动程序
// 编写人员: 崔超                                                                       
// 联系方式: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com  
//==============版本信息===============================================
// 版    本: V1.0	 
// 日    期: 2010年09月06日	
// 描    述: 原始版本 
//======================================================================


//============================= 头文件 =================================
#include "config.h"   

//============================= 宏定义 =================================
#define SEMICOLON   ','    
#define ASTERISK    '*'    

#define USE_ATOI_ATOF    //使用库函数中的 atoi atof 使用会增加4K代码量
                         //不定义则使用自定义的 my_atoi my_atof

//============================= 变量定义 ===============================

  GPSINFO  gps;    //GPS信息
  GPS_NEMA_STRUCT GPS_NEMA [NEMA_NUM_MAX] ;  //GPS语句缓存 
  char lasttime[20];  //最后的时间保存
  BOOL isrunning;  //GPS运行标志
  BOOL  isFirmwareUpdate = FALSE; //GPS固件更新标志
//======================================================================
//函 数 名: GPS_Run() 
//功    能: GPS初始化运行
//入口参数: 无
//出口参数: 无
//返 回 值: TRUE 成功；FALSE：失败
//======================================================================
BOOL GPS_Run(void)   
{   
    if( isrunning)   
        return FALSE;
           
    isrunning  = TRUE;
    GPS_init();   
    GPS_BOOT_L(); 
    Delay_ms(100);
    GPS_POWER_ON();

    return isrunning;   
}   
//======================================================================
//函 数 名: GPS_Stop() 
//功    能: GPS关闭
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//======================================================================   
void GPS_Stop(void)   
{   

        GPS_POWER_OFF();  //关闭GPS电源
       // GPS_BOOT_H();
        isrunning=FALSE;   
     
}
//======================================================================
//函 数 名: GPS_FirmwareUpdate() 
//功    能: GPS固件更新
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//其他    : 目前有问题，后续计划改为是用USB虚拟串口更新 
//====================================================================== 
void GPS_FirmwareUpdate(void)   
{   
        isFirmwareUpdate = TRUE;
        GPS_POWER_OFF();  //关闭GPS电源
        Delay_ms(1000);
        GPS_BOOT_H();
        Delay_ms(1000);
        GPS_POWER_ON();  //开启GPS电源
         
        USART1_Configuration(38400);
	    USART3_Configuration(38400);
     
}  
//======================================================================
//函 数 名: GPS_IsRunning() 
//功    能: GPS运行状态检测
//入口参数: 无
//出口参数: 无
//返 回 值: TRUE：运行，FLASE: 停止
//======================================================================    
BOOL GPS_IsRunning(void)   
{   
    return isrunning;   
}   
//======================================================================
//函 数 名: GPS_IsLocationValid() 
//功    能: GPS定位状态检测
//入口参数: 无
//出口参数: 无
//返 回 值: TRUE：运行，FLASE: 停止
//======================================================================   
BOOL GPS_IsLocationValid(void)   
{   
    return (gps.isvalid != 0) ? TRUE : FALSE;   
}   
//======================================================================
//函 数 名: GPS_GetLongitude() 
//功    能: 获取经度
//入口参数: 无
//出口参数: 无
//返 回 值: 经度值，东经为正值，西经为负值
//======================================================================  
FP32 GPS_GetLongitude(void)   
{   
    
    return (gps.lgtEW == 'E') ? gps.longitude : -gps.longitude;   
}   
//======================================================================
//函 数 名: GPS_GetLatitude() 
//功    能: 获取纬度
//入口参数: 无
//出口参数: 无
//返 回 值: 纬度值，北纬为正值，南纬为负值
//======================================================================    
FP32 GPS_GetLatitude(void)   
{   
    return (gps.latNS == 'N') ? gps.latitude : -gps.latitude;   
}   
//======================================================================
//函 数 名: GPS_GetAltitude() 
//功    能: 获取海拔高度
//入口参数: 无
//出口参数: 无
//返 回 值: 海拔高度值，
//====================================================================== 
FP32 GPS_GetAltitude(void)   
{   
    return gps.altitude;   
}   
//======================================================================
//函 数 名: GPS_Getaltitudeunit() 
//功    能: 获取海拔高度单位
//入口参数: 无
//出口参数: 无
//返 回 值: 海拔高度单位，
//======================================================================    
char GPS_GetAltitudeUnit(void)   
{   
    return gps.altitudeunit;   
}   
//======================================================================
//函 数 名: GPS_GetSpeed() 
//功    能: 获取速度
//入口参数: 无
//出口参数: 无
//返 回 值: 速度，单位KM/h，
//======================================================================    
FP32 GPS_GetSpeed(void)   
{   
    return gps.speed;   
}   
//======================================================================
//函 数 名: GPS_GetDirection() 
//功    能: 获取方位角
//入口参数: 无
//出口参数: 无
//返 回 值: 方位角，度 ，以真北为参考
//======================================================================    
FP32 GPS_GetDirection(void)   
{   
    return gps.direction;   
}   
//======================================================================
//函 数 名: GPS_GetUserSatNum() 
//功    能: 获取有效卫星颗数
//入口参数: 无
//出口参数: 无
//返 回 值: 有效卫星颗数
//======================================================================     
int GPS_GetUserSatNum(void)   
{   
    return gps.usedsatnum;   
}   
//======================================================================
//函 数 名: GPS_GetError() 
//功    能: 获取综合定位误差
//入口参数: 无
//出口参数: 无
//返 回 值: 综合定位误差
//======================================================================     
FP32 GPS_GetError(void)   
{   
    return gps.PDOP;   
}  
//======================================================================
//函 数 名: GPS_DegreeToDMS() 
//功    能: 度转换为度分秒
//入口参数: FP32 deg：角度值
//出口参数: int *d,度 int *m,分 FP32 *s 秒
//返 回 值: 无
//======================================================================  
void GPS_DegreeToDMS(FP32 deg,int *d,int *m,FP32 *s)   
{  
   *d = (int)deg;   
   *m = (int)((deg - *d ) * 60);   
   *s = (((deg-*d)*60)-*m)*60;   
   if(60.0-*s<0.01)   
   {   
       *s = 0.0;   
       *m ++;   
       if(*m == 60)   
       {   
           *d++;   
           *m = 0;   
       }   
   }   
}
/*  
FP32 CCrd::DMSToRadian(FP32 dms)   
{   
    int d = (int)dms;   
    int m = (int)((dms -d) * 100);   
    FP32 s = ((dms - d) * 100 - m) * 100;   
    return ((FP32)d+(FP32)m/60+s/3600)*PI/180;   
} 
*/
#ifdef USE_ATOI_ATOF   
    #define my_atoi   atoi
    #define my_atof   atof
#else
//======================================================================
//函 数 名: my_atoi() 
//功    能: 字符串转化为整数
//入口参数: *str：字符串首地址
//出口参数: 无
//返 回 值: int 整数
//====================================================================== 
int my_atoi(const char *str)
{
    int result = 0;
    int signal = 1; /* 默认为正数 */
    if((*str>='0'&&*str<='9')||*str=='-'||*str=='+')
    {
        if(*str=='-'||*str=='+')
        {   if(*str=='-')
	        signal = -1; /* 输入负数 */ 
	        str++;
         }
    }
    else return 0; 
   
   /* 开始转换 */
   while(*str>='0'&&*str<='9')
        result = result*10+(*str++ -'0'); 

   return signal*result;
}


//======================================================================
//函 数 名: my_strtod() 
//功    能: 字符串转换为浮点数
//入口参数: *str：字符串首地址
//出口参数: 无
//返 回 值: FP32 浮点数
//====================================================================== 
/*

   这个函数是把浮点数字符串转换为浮点数的函数。

   函数将会跳过字符串中的空格字符和不是'+'、'-'、'.'、

   数字的字符。如果字符串是空的或者都是由空格组成，将不会

   做任何转换，仅仅是把字符串的结束地址赋给endptr。如果字

   符串合法，将会进行转换，并把字符串最后的NULL的地址给

   endptr。如果你想使用endptr参数，那么赋一个NULL值就

   可以了。

*/
FP32 my_strtod(const char* s, char** endptr)
{
 const char*  p = s;

    FP32  value = 0.L;

    int  sign  = 0;

    FP32  factor;

    unsigned int expo;

    while ( isspace(*p) )//跳过前面的空格
    p++;
    
    if(*p == '-' || *p == '+')

      sign = *p++;//把符号赋给字符sign，指针后移。

   //处理数字字符
    while ( (unsigned int)(*p - '0') < 10u )//转换整数部分

      value = value*10 + (*p++ - '0');

   //如果是正常的表示方式（如：1234.5678）

   if ( *p == '.' ) 
     {
      factor = 1.; 
      
      p++;

        while ( (unsigned int)(*p - '0') < 10u ) 

      {

         factor *= 0.1;

            value  += (*p++ - '0') * factor;

        }

    }

   //如果是IEEE754标准的格式（如：1.23456E+3）

    if ( (*p | 32) == 'e' ) 

   {

      expo   = 0;

        factor = 10.L;

        switch (*++p)

      { 

        case '-': 

           factor = 0.1;

        case '+': 

           p++;

           break;

        case '0': 

        case '1': 

        case '2':

        case '3':

        case '4': 

        case '5': 

        case '6': 

        case '7': 

        case '8': 

        case '9':

           break;

        default : 

           value = 0.L;

           p     = s;

           goto done;

        }

        while ( (unsigned int)(*p - '0') < 10u )

            expo = 10 * expo + (*p++ - '0');

        while ( 1 )

      {

        if ( expo & 1 )

           value *= factor;

            if ( (expo >>= 1) == 0 )

                break;

            factor *= factor;

        }

    }

done:

    if ( endptr != 0 )

        *endptr = (char*)p;

 

    return (sign == '-' ? -value : value);

}
//======================================================================
//函 数 名: my_atof() 
//功    能: 字符串转换为浮点数
//入口参数: *str：字符串首地址
//出口参数: 无
//返 回 值: FP32 浮点数
//====================================================================== 
FP32 my_atof(char *str)

{

   return my_strtod(str,0);

}
#endif // USE_ATOI_ATOF
/*
char *match(char *str,char *patten)   
{   
    while(*str != 0 && *patten != 0 && *str == *patten)   
    {   
        str++;   
        patten++;   
    }   
   
    if(*patten == 0)   
        return str+1;   
    else   
        return NULL;   
}   
   
char *next_fld(char *str)   
{   
    char *ts=str;   
   
    while(*ts != ',' && *ts != 0)   
        ts++;   
    *ts='\0';   
   
    return str;   
} */
//======================================================================
//函 数 名: split() 
//功    能: 字符串分解
//入口参数: *buf：字符串首地址  s：分割标识字  char **left  分解后的开始地址
//出口参数: 无
//返 回 值: 分解出的字符串地址 
//====================================================================== 
char *split(char *buf,char s,char **left)   
{   

    char *p=buf,*ret=buf;
      
    if(buf == NULL || buf[0] == 0)   
    {   
        *left=NULL;   
        return NULL;   
    }   
   
 
    while(*p != 0 && *p != s && *p != '\r' && *p != '\n')   
    {   
        p++;   
    }   
   
    if(*p != 0)   
    {   
        *left=p+1;   
        *p=0;   
    }   
    else   
    {   
        *left=NULL;   
    }   
   
    return ret;   
}   
//======================================================================
//函 数 名: check() 
//功    能: 字符串校验
//入口参数: char *sentence：字符串首地址 char *cksum 校验码地址
//出口参数: 无
//返 回 值: 0 ：不正确; 1: 正确
//======================================================================    
const unsigned char digits[16]={ '0','1','2','3','4','5','6','7','8','9',   
                           'A','B','C','D','E','F' };   
int check(char *sentence,char *cksum)   
{   

    unsigned char *p=(unsigned char *)sentence,sum=0; 
    if(sentence == NULL || cksum == NULL)   
        return 0;    
  
    for(; *p != 0; p++)   
    {   
        sum ^= *p;   
    }   
   
    if(digits[sum >> 4] == cksum[0] && digits[sum & 0x0f] == cksum[1])   
        return 1;   
  
    return 0;   
}   

//======================================================================
//函 数 名: GPS_parse() 
//功    能: GPS语句分析
//入口参数: char *buf 字符串首地址
//出口参数: 无
//返 回 值: 0 ：语句不正确; 1: 语句正确
//====================================================================== 
u8 GPS_parse(char *buf)   
{   
   int d,m,mm;
   u8 i;
   char *word,*left=buf+1;    
   static u8 msgcount=0,msgid=0,satcount=0;   //解析GSV用到的变量
           //各通道采用的卫星编号    
   u8 usedsatcount=0;

    if(buf[0] != '$')   
        return 0;  

    word=split(left,ASTERISK,&left);   
    if(check(word,left) != 1)   
        return 0; 

    left=word;  
       
    word=split(left,SEMICOLON,&left);   
    if(!strcmp(word,"GPRMC"))   
    {   
        //时间戳    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(strncmp(lasttime,word,20))   
            {   
                sscanf(word,"%2d%2d%2d.%4d",&gps.hh,&gps.mm,&gps.ss,&gps.ms);   
                strncpy(lasttime,word,20);   
            }   
        }   
   
        //定位状态    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(word[0] == 'A')   
                gps.isvalid=1;   
            else   
                gps.isvalid=0;   
        }   
   
        //纬度    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
             
            sscanf(word,"%2d%2d.%4d",&d,&m,&mm);   
            gps.latitude=(float)d+(float)m/60.0+(float)mm/600000.0;   
        }   
   
        //南北半球标志    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(word[0] == 'S')   
              { // gps.latitude=-gps.latitude; 
                 gps.latNS = 'S';
              }else{
                 gps.latNS = 'N';
              } 
         }       

   
        //经度    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            int d,m,mm;   
            sscanf(word,"%3d%2d.%4d",&d,&m,&mm);   
            gps.longitude = (float)d+(float)m/60.0+(float)mm/600000.0;   
        }   
   
        //东西半球标志    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(word[0] == 'W')
            {   
                gps.lgtEW = 'W';
               // gps.longitude=-gps.longitude;
            } else{
                gps.lgtEW = 'E';
            }  
        }   
   
        //对地运动速度    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.speed=my_atof(word)*1.852;   
        }   
   
        //对地运动方向    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.direction=my_atof(word);   
        } 
        
         //utc 日期    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        { 
          sscanf(word,"%2d%2d%2d",&gps.DD,&gps.MM,&gps.YY);    
        }  
    }   
    else if(!strcmp(word,"GPGGA"))   
    {   
        //时间戳    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(strncmp(lasttime,word,20))   
            {   
                sscanf(word,"%2d%2d%2d.%4d",&gps.hh,&gps.mm,&gps.ss,&gps.ms);   
                strncpy(lasttime,word,20);   
            }   
        }   
   
        //纬度    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            int d,m,mm;   
            sscanf(word,"%2d%2d.%4d",&d,&m,&mm);   
            gps.latitude=(float)d+(float)m/60.0+(float)mm/600000.0;   
        }   
   
        //南北半球标志    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(word[0] == 'S')   
              { // gps.latitude=-gps.latitude; 
                 gps.latNS = 'S';
              }else{
                 gps.latNS = 'N';
              } 
         }    
   
        //经度    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            int d,m,mm;   
            sscanf(word,"%3d%2d.%4d",&d,&m,&mm);   
            gps.longitude=(float)d+(float)m/60.0+(float)mm/600000.0;   
        }   
   
        //东西半球标志    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(word[0] == 'W')
            {   
                gps.lgtEW = 'W';
               // gps.longitude=-gps.longitude;
            } else{
                gps.lgtEW = 'E';
            }  
        }   
   
        //定位有效性及格式    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.FixMode=my_atoi(word);   
        }   
   
        //捕捉卫星数量    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.usedsatnum=my_atoi(word);   
        }   
   
        //估计误差    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.HDOP=my_atof(word); 
            
            //sscanf(word,"%f",&gps.HDOP);  
        }   
   
        //海拔高度    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.altitude=my_atof(word);   
        }   
   
        //高度单位    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.altitudeunit=word[0];   
        }   
    }   
    else if(!strcmp(word,"GPGSV"))   
    {   
        
   
        //消息总数    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            msgcount=my_atoi(word);   
        }   
   
        //消息编号    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            msgid=my_atoi(word);   
        }   
   
        if(msgid == 1)   
        {   
            satcount=0;   
            //memset(satinfo,0,sizeof(SatelliteInfo)*38);   
        }   
   
        //卫星总数    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.allsatnum=my_atoi(word);   
        }   
   
       // printf("%s\n",left);   
        for(i=0;i<4;i++)   
        {   
            //卫星编号    
            word=split(left,SEMICOLON,&left);   
            if(word != NULL)   
            {   
                gps.satinfo[satcount].satid=my_atoi(word);   
            }   
   
            //卫星仰角    
            word=split(left,SEMICOLON,&left);   
            if(word != NULL)   
            {   
                gps.satinfo[satcount].elevation=my_atoi(word);   
            }   
   
            //卫星方位角    
            word=split(left,SEMICOLON,&left);   
            if(word != NULL)   
            {   
                gps.satinfo[satcount].azimuth=my_atoi(word);   
            }   
   
            //卫星信号信噪比    
            word=split(left,SEMICOLON,&left);   
            if(word != NULL)   
            {   
                gps.satinfo[satcount].snr=my_atoi(word);   
            }   
   
            satcount++;   
   
            if(word == NULL)   
                break;   
        }   
   
        if(msgid == msgcount )   
          ;   
    }   
    else if(!strcmp(word,"GPGSA"))   
    {   
        //定位模式1    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.GSA_mode1=my_atoi(word);   
        }   
   
        //定位模式2    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.GSA_mode2=my_atoi(word);   
        }   
   

          
        for(i=0;i<12;i++)   
        {   
            word=split(left,SEMICOLON,&left);   
            if(word != NULL)   
            {   
                gps.usedsat[i]=my_atoi(word);   
                usedsatcount++;   
            }   
        }   
   
        //位置精度值    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.PDOP=(float)my_atof(word);   
        }   
   
        //水平精度值    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.HDOP=(float)my_atof(word);   
        }   
   
        //高度精度值    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.VDOP=(float)my_atof(word);   
        }       
    }  
    //==============错误值过滤 =================
    if(gps.latitude > 90.0)
      gps.latitude = 0.0;

        if(gps.longitude > 180.0)
      gps.longitude = 0.0;
   //=============错误值产生原因不详============
    return 1;   
}
//======================================================================
//函 数 名: USART3_IRQHandler() 
//功    能: GPS中断复位函数
//入口参数： 无
//出口参数： 无
//返 回 值： 无
//====================================================================== 
void USART3_IRQHandler(void)
{    
    u8 buffer;
    static u8 NEMA_count = 0;
    static u8 char_count = 0;

   // if(isFirmwareUpdate == 1)     //更新固件
   //   return;

   if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
   	buffer =(u8) (USART_ReceiveData(USART3)); //获取数据
    	
   USART1->DR = buffer;  //转给usart1

    if(buffer == '$')
      {  
         GPS_NEMA[NEMA_count].isupdated = 1;  //将上面一条语句打上更新标志
         NEMA_count++;
         if(NEMA_count > (NEMA_NUM_MAX-1))
         {
            NEMA_count = 0;
         }
         GPS_NEMA[NEMA_count].isupdated = 0;  //将本条语句打上未更新标志
         GPS_NEMA[NEMA_count].buffer[0] = '$';
         char_count = 1; 
         
        }else{

         if(char_count < NEMA_CHAR_MAX-1)
         GPS_NEMA[NEMA_count].buffer[char_count++] = buffer;
         }   
  }
 USART_ClearITPendingBit(USART3, USART_IT_RXNE);//清楚中断标志 	
}
//======================================================================
//函 数 名: GPS_INFO_update() 
//功    能: GPS数据更新
//入口参数： 无
//出口参数： 无
//返 回 值： 0 未更新 1 更新
//====================================================================== 
u8 GPS_INFO_update(void)
{ 
    u8 i;
    u8 temp = 0;
    for(i=0;i<NEMA_NUM_MAX;i++)
    {
      if(GPS_NEMA[i].isupdated == 1)
        { 
          if('$' == GPS_NEMA[i].buffer[0] )
              {
                  FATFS_WriteNEMA(GPS_NEMA[i].buffer);
              }
              
            temp += GPS_parse(GPS_NEMA[i].buffer); 
            GPS_NEMA[i].isupdated = 0  ;
            memset(GPS_NEMA[i].buffer,0x00,NEMA_CHAR_MAX);
        }       
    }
    
  return (temp != 0)? 1 : 0;
}



//======================================================================
//函 数 名: GPS_init() 
//功    能: 配置GPS管脚
//入口参数: 无
//出口参数: 无
//返 回 值: 无
//======================================================================
 void GPS_init(void)
{ 			
    GPIO_InitTypeDef GPIO_InitStructure;  
											   

	/* 配置 PC.0 GPS_BOOT -PC.1 GPS_EN 为输出模式*/
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 + GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);     
    
    GPS_Stop();  


}
 //======================================================================
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//======================================================================
