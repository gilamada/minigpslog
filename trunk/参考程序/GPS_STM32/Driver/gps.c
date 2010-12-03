//=====================================================================
// �ļ�����: GPS.C
// оƬ����: STM32F103VCT6                                           
// �������: Keil, Windows XP2 
// ��������: 2010��09��06��                                                               
// ������: GPS��������
// ��д��Ա: �޳�                                                                       
// ��ϵ��ʽ: QQ:52020081 MSN:cui_chao@msn.com Email:soulcoffee@sohu.com  
//==============�汾��Ϣ===============================================
// ��    ��: V1.0	 
// ��    ��: 2010��09��06��	
// ��    ��: ԭʼ�汾 
//======================================================================


//============================= ͷ�ļ� =================================
#include "config.h"   

//============================= �궨�� =================================
#define SEMICOLON   ','    
#define ASTERISK    '*'    

#define USE_ATOI_ATOF    //ʹ�ÿ⺯���е� atoi atof ʹ�û�����4K������
                         //��������ʹ���Զ���� my_atoi my_atof

//============================= �������� ===============================

  GPSINFO  gps;    //GPS��Ϣ
  GPS_NEMA_STRUCT GPS_NEMA [NEMA_NUM_MAX] ;  //GPS��仺�� 
  char lasttime[20];  //����ʱ�䱣��
  BOOL isrunning;  //GPS���б�־
  BOOL  isFirmwareUpdate = FALSE; //GPS�̼����±�־
//======================================================================
//�� �� ��: GPS_Run() 
//��    ��: GPS��ʼ������
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: TRUE �ɹ���FALSE��ʧ��
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
//�� �� ��: GPS_Stop() 
//��    ��: GPS�ر�
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//======================================================================   
void GPS_Stop(void)   
{   

        GPS_POWER_OFF();  //�ر�GPS��Դ
       // GPS_BOOT_H();
        isrunning=FALSE;   
     
}
//======================================================================
//�� �� ��: GPS_FirmwareUpdate() 
//��    ��: GPS�̼�����
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//����    : Ŀǰ�����⣬�����ƻ���Ϊ����USB���⴮�ڸ��� 
//====================================================================== 
void GPS_FirmwareUpdate(void)   
{   
        isFirmwareUpdate = TRUE;
        GPS_POWER_OFF();  //�ر�GPS��Դ
        Delay_ms(1000);
        GPS_BOOT_H();
        Delay_ms(1000);
        GPS_POWER_ON();  //����GPS��Դ
         
        USART1_Configuration(38400);
	    USART3_Configuration(38400);
     
}  
//======================================================================
//�� �� ��: GPS_IsRunning() 
//��    ��: GPS����״̬���
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: TRUE�����У�FLASE: ֹͣ
//======================================================================    
BOOL GPS_IsRunning(void)   
{   
    return isrunning;   
}   
//======================================================================
//�� �� ��: GPS_IsLocationValid() 
//��    ��: GPS��λ״̬���
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: TRUE�����У�FLASE: ֹͣ
//======================================================================   
BOOL GPS_IsLocationValid(void)   
{   
    return (gps.isvalid != 0) ? TRUE : FALSE;   
}   
//======================================================================
//�� �� ��: GPS_GetLongitude() 
//��    ��: ��ȡ����
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ����ֵ������Ϊ��ֵ������Ϊ��ֵ
//======================================================================  
FP32 GPS_GetLongitude(void)   
{   
    
    return (gps.lgtEW == 'E') ? gps.longitude : -gps.longitude;   
}   
//======================================================================
//�� �� ��: GPS_GetLatitude() 
//��    ��: ��ȡγ��
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: γ��ֵ����γΪ��ֵ����γΪ��ֵ
//======================================================================    
FP32 GPS_GetLatitude(void)   
{   
    return (gps.latNS == 'N') ? gps.latitude : -gps.latitude;   
}   
//======================================================================
//�� �� ��: GPS_GetAltitude() 
//��    ��: ��ȡ���θ߶�
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ���θ߶�ֵ��
//====================================================================== 
FP32 GPS_GetAltitude(void)   
{   
    return gps.altitude;   
}   
//======================================================================
//�� �� ��: GPS_Getaltitudeunit() 
//��    ��: ��ȡ���θ߶ȵ�λ
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ���θ߶ȵ�λ��
//======================================================================    
char GPS_GetAltitudeUnit(void)   
{   
    return gps.altitudeunit;   
}   
//======================================================================
//�� �� ��: GPS_GetSpeed() 
//��    ��: ��ȡ�ٶ�
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: �ٶȣ���λKM/h��
//======================================================================    
FP32 GPS_GetSpeed(void)   
{   
    return gps.speed;   
}   
//======================================================================
//�� �� ��: GPS_GetDirection() 
//��    ��: ��ȡ��λ��
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��λ�ǣ��� �����汱Ϊ�ο�
//======================================================================    
FP32 GPS_GetDirection(void)   
{   
    return gps.direction;   
}   
//======================================================================
//�� �� ��: GPS_GetUserSatNum() 
//��    ��: ��ȡ��Ч���ǿ���
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��Ч���ǿ���
//======================================================================     
int GPS_GetUserSatNum(void)   
{   
    return gps.usedsatnum;   
}   
//======================================================================
//�� �� ��: GPS_GetError() 
//��    ��: ��ȡ�ۺ϶�λ���
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: �ۺ϶�λ���
//======================================================================     
FP32 GPS_GetError(void)   
{   
    return gps.PDOP;   
}  
//======================================================================
//�� �� ��: GPS_DegreeToDMS() 
//��    ��: ��ת��Ϊ�ȷ���
//��ڲ���: FP32 deg���Ƕ�ֵ
//���ڲ���: int *d,�� int *m,�� FP32 *s ��
//�� �� ֵ: ��
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
//�� �� ��: my_atoi() 
//��    ��: �ַ���ת��Ϊ����
//��ڲ���: *str���ַ����׵�ַ
//���ڲ���: ��
//�� �� ֵ: int ����
//====================================================================== 
int my_atoi(const char *str)
{
    int result = 0;
    int signal = 1; /* Ĭ��Ϊ���� */
    if((*str>='0'&&*str<='9')||*str=='-'||*str=='+')
    {
        if(*str=='-'||*str=='+')
        {   if(*str=='-')
	        signal = -1; /* ���븺�� */ 
	        str++;
         }
    }
    else return 0; 
   
   /* ��ʼת�� */
   while(*str>='0'&&*str<='9')
        result = result*10+(*str++ -'0'); 

   return signal*result;
}


//======================================================================
//�� �� ��: my_strtod() 
//��    ��: �ַ���ת��Ϊ������
//��ڲ���: *str���ַ����׵�ַ
//���ڲ���: ��
//�� �� ֵ: FP32 ������
//====================================================================== 
/*

   ��������ǰѸ������ַ���ת��Ϊ�������ĺ�����

   �������������ַ����еĿո��ַ��Ͳ���'+'��'-'��'.'��

   ���ֵ��ַ�������ַ����ǿյĻ��߶����ɿո���ɣ�������

   ���κ�ת���������ǰ��ַ����Ľ�����ַ����endptr�������

   �����Ϸ����������ת���������ַ�������NULL�ĵ�ַ��

   endptr���������ʹ��endptr��������ô��һ��NULLֵ��

   �����ˡ�

*/
FP32 my_strtod(const char* s, char** endptr)
{
 const char*  p = s;

    FP32  value = 0.L;

    int  sign  = 0;

    FP32  factor;

    unsigned int expo;

    while ( isspace(*p) )//����ǰ��Ŀո�
    p++;
    
    if(*p == '-' || *p == '+')

      sign = *p++;//�ѷ��Ÿ����ַ�sign��ָ����ơ�

   //���������ַ�
    while ( (unsigned int)(*p - '0') < 10u )//ת����������

      value = value*10 + (*p++ - '0');

   //����������ı�ʾ��ʽ���磺1234.5678��

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

   //�����IEEE754��׼�ĸ�ʽ���磺1.23456E+3��

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
//�� �� ��: my_atof() 
//��    ��: �ַ���ת��Ϊ������
//��ڲ���: *str���ַ����׵�ַ
//���ڲ���: ��
//�� �� ֵ: FP32 ������
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
//�� �� ��: split() 
//��    ��: �ַ����ֽ�
//��ڲ���: *buf���ַ����׵�ַ  s���ָ��ʶ��  char **left  �ֽ��Ŀ�ʼ��ַ
//���ڲ���: ��
//�� �� ֵ: �ֽ�����ַ�����ַ 
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
//�� �� ��: check() 
//��    ��: �ַ���У��
//��ڲ���: char *sentence���ַ����׵�ַ char *cksum У�����ַ
//���ڲ���: ��
//�� �� ֵ: 0 ������ȷ; 1: ��ȷ
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
//�� �� ��: GPS_parse() 
//��    ��: GPS������
//��ڲ���: char *buf �ַ����׵�ַ
//���ڲ���: ��
//�� �� ֵ: 0 ����䲻��ȷ; 1: �����ȷ
//====================================================================== 
u8 GPS_parse(char *buf)   
{   
   int d,m,mm;
   u8 i;
   char *word,*left=buf+1;    
   static u8 msgcount=0,msgid=0,satcount=0;   //����GSV�õ��ı���
           //��ͨ�����õ����Ǳ��    
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
        //ʱ���    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(strncmp(lasttime,word,20))   
            {   
                sscanf(word,"%2d%2d%2d.%4d",&gps.hh,&gps.mm,&gps.ss,&gps.ms);   
                strncpy(lasttime,word,20);   
            }   
        }   
   
        //��λ״̬    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(word[0] == 'A')   
                gps.isvalid=1;   
            else   
                gps.isvalid=0;   
        }   
   
        //γ��    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
             
            sscanf(word,"%2d%2d.%4d",&d,&m,&mm);   
            gps.latitude=(float)d+(float)m/60.0+(float)mm/600000.0;   
        }   
   
        //�ϱ������־    
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

   
        //����    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            int d,m,mm;   
            sscanf(word,"%3d%2d.%4d",&d,&m,&mm);   
            gps.longitude = (float)d+(float)m/60.0+(float)mm/600000.0;   
        }   
   
        //���������־    
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
   
        //�Ե��˶��ٶ�    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.speed=my_atof(word)*1.852;   
        }   
   
        //�Ե��˶�����    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.direction=my_atof(word);   
        } 
        
         //utc ����    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        { 
          sscanf(word,"%2d%2d%2d",&gps.DD,&gps.MM,&gps.YY);    
        }  
    }   
    else if(!strcmp(word,"GPGGA"))   
    {   
        //ʱ���    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            if(strncmp(lasttime,word,20))   
            {   
                sscanf(word,"%2d%2d%2d.%4d",&gps.hh,&gps.mm,&gps.ss,&gps.ms);   
                strncpy(lasttime,word,20);   
            }   
        }   
   
        //γ��    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            int d,m,mm;   
            sscanf(word,"%2d%2d.%4d",&d,&m,&mm);   
            gps.latitude=(float)d+(float)m/60.0+(float)mm/600000.0;   
        }   
   
        //�ϱ������־    
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
   
        //����    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            int d,m,mm;   
            sscanf(word,"%3d%2d.%4d",&d,&m,&mm);   
            gps.longitude=(float)d+(float)m/60.0+(float)mm/600000.0;   
        }   
   
        //���������־    
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
   
        //��λ��Ч�Լ���ʽ    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.FixMode=my_atoi(word);   
        }   
   
        //��׽��������    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.usedsatnum=my_atoi(word);   
        }   
   
        //�������    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.HDOP=my_atof(word); 
            
            //sscanf(word,"%f",&gps.HDOP);  
        }   
   
        //���θ߶�    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.altitude=my_atof(word);   
        }   
   
        //�߶ȵ�λ    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.altitudeunit=word[0];   
        }   
    }   
    else if(!strcmp(word,"GPGSV"))   
    {   
        
   
        //��Ϣ����    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            msgcount=my_atoi(word);   
        }   
   
        //��Ϣ���    
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
   
        //��������    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.allsatnum=my_atoi(word);   
        }   
   
       // printf("%s\n",left);   
        for(i=0;i<4;i++)   
        {   
            //���Ǳ��    
            word=split(left,SEMICOLON,&left);   
            if(word != NULL)   
            {   
                gps.satinfo[satcount].satid=my_atoi(word);   
            }   
   
            //��������    
            word=split(left,SEMICOLON,&left);   
            if(word != NULL)   
            {   
                gps.satinfo[satcount].elevation=my_atoi(word);   
            }   
   
            //���Ƿ�λ��    
            word=split(left,SEMICOLON,&left);   
            if(word != NULL)   
            {   
                gps.satinfo[satcount].azimuth=my_atoi(word);   
            }   
   
            //�����ź������    
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
        //��λģʽ1    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.GSA_mode1=my_atoi(word);   
        }   
   
        //��λģʽ2    
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
   
        //λ�þ���ֵ    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.PDOP=(float)my_atof(word);   
        }   
   
        //ˮƽ����ֵ    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.HDOP=(float)my_atof(word);   
        }   
   
        //�߶Ⱦ���ֵ    
        word=split(left,SEMICOLON,&left);   
        if(word != NULL)   
        {   
            gps.VDOP=(float)my_atof(word);   
        }       
    }  
    //==============����ֵ���� =================
    if(gps.latitude > 90.0)
      gps.latitude = 0.0;

        if(gps.longitude > 180.0)
      gps.longitude = 0.0;
   //=============����ֵ����ԭ����============
    return 1;   
}
//======================================================================
//�� �� ��: USART3_IRQHandler() 
//��    ��: GPS�жϸ�λ����
//��ڲ����� ��
//���ڲ����� ��
//�� �� ֵ�� ��
//====================================================================== 
void USART3_IRQHandler(void)
{    
    u8 buffer;
    static u8 NEMA_count = 0;
    static u8 char_count = 0;

   // if(isFirmwareUpdate == 1)     //���¹̼�
   //   return;

   if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
   	buffer =(u8) (USART_ReceiveData(USART3)); //��ȡ����
    	
   USART1->DR = buffer;  //ת��usart1

    if(buffer == '$')
      {  
         GPS_NEMA[NEMA_count].isupdated = 1;  //������һ�������ϸ��±�־
         NEMA_count++;
         if(NEMA_count > (NEMA_NUM_MAX-1))
         {
            NEMA_count = 0;
         }
         GPS_NEMA[NEMA_count].isupdated = 0;  //������������δ���±�־
         GPS_NEMA[NEMA_count].buffer[0] = '$';
         char_count = 1; 
         
        }else{

         if(char_count < NEMA_CHAR_MAX-1)
         GPS_NEMA[NEMA_count].buffer[char_count++] = buffer;
         }   
  }
 USART_ClearITPendingBit(USART3, USART_IT_RXNE);//����жϱ�־ 	
}
//======================================================================
//�� �� ��: GPS_INFO_update() 
//��    ��: GPS���ݸ���
//��ڲ����� ��
//���ڲ����� ��
//�� �� ֵ�� 0 δ���� 1 ����
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
//�� �� ��: GPS_init() 
//��    ��: ����GPS�ܽ�
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//======================================================================
 void GPS_init(void)
{ 			
    GPIO_InitTypeDef GPIO_InitStructure;  
											   

	/* ���� PC.0 GPS_BOOT -PC.1 GPS_EN Ϊ���ģʽ*/
    
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
