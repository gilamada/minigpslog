//=====================================================================
// �ļ�����: GPS.H
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
#ifndef __GPS_H_
#define __GPS_H_

//============================= �궨�� =================================
#define  NEMA_NUM_MAX   10     //�����NEMA�������
#define  NEMA_CHAR_MAX  150   //�����NEMA����ַ����� 

#define GPS_BOOT_H()    GPIOC->BSRR = GPIO_Pin_1
#define GPS_BOOT_L()    GPIOC->BRR  = GPIO_Pin_1
#define GPS_POWER_OFF()  GPIOC->BSRR = GPIO_Pin_0
#define GPS_POWER_ON()   GPIOC->BRR  = GPIO_Pin_0
//============================= �ṹ�嶨�� ===============================  
typedef	struct 
{ 
    int satid;      //������� 
    int elevation;  //�������ǣ�00 - 90����
    int azimuth;    //���Ƿ�λ�ǣ�00 - 359����
    int snr;        //����ȣ�00��99��dbHz 
} SatelliteInfo; 


typedef  struct
{

    char isvalid;  //GPS ��λ��־   0=δ��λ��1=�Ѷ�λ
    int hh,mm,ss,ms;

    int DD, MM, YY;
    FP32 latitude;
    u8 latNS;    
    FP32 longitude;
    u8   lgtEW;
    FP32 speed;       //�����ٶȣ�GPS�����λ��(Knots), �Ѿ�ת��λKM/H
    FP32 direction;   //��λ�ǣ��� �����汱Ϊ�ο�

    FP32  altitude;     //���θ߶�
    u8 altitudeunit;       //���ε�λ

    u8  FixMode;     //GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��3=��ЧPPS��6=���ڹ���
    u8 GSA_mode1;//��λģʽ��A=�Զ��ֶ�2D/3D��M=�ֶ�2D/3D 
    u8 GSA_mode2;//��λ���ͣ�1=δ��λ��2=2D��λ��3=3D��λ 

    FP32 PDOP;          //�ۺ�λ�þ�������
    FP32 HDOP;          //ˮƽ��������
    FP32 VDOP;          //��ֱ�������� 
    u32  ageOfDiff;//���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ� 
    u16  diffStationID;//���վID��0000 - 1023��ǰ��λ��������0��������ǲ�ֶ�λ��Ϊ�գ�
     
    u8 usedsat[12];//��������������������
    u8 usedsatnum;  //����ʹ�õ�����������00 - 12��
    u8 allsatnum;  //��ǰ�ɼ�����������00 - 12��
    SatelliteInfo satinfo[38];
}GPSINFO;


typedef struct
{
    u8   isupdated;  //NEMA ���������±�־
    char buffer[NEMA_CHAR_MAX]; //NEMA ������
}GPS_NEMA_STRUCT;


//============================= �������� ===============================
extern  GPS_NEMA_STRUCT GPS_NEMA [NEMA_NUM_MAX] ; 
extern  GPSINFO  gps;


extern void FATFS_WriteNEMA( char *ptr);  //

//============================= �������� ===============================
extern void GPS_init(void);
extern u8   GPS_parse(char *buf); 
extern void  GPS_usart(u8 buffer);  
extern u8 GPS_INFO_update(void);
extern BOOL GPS_Run(void); 
extern void GPS_Stop(void);
extern void GPS_FirmwareUpdate(void);
extern void GPS_DegreeToDMS(FP32 deg,int *d,int *m,FP32 *s)   ;
extern  BOOL GPS_IsRunning(void); 

extern BOOL GPS_IsLocationValid(void)  ;
extern FP32 GPS_GetLongitude(void) ;
extern FP32 GPS_GetLatitude(void);  
extern FP32 GPS_GetAltitude(void); 
extern char GPS_GetAltitudeUnit(void);   
extern FP32 GPS_GetSpeed(void); 
extern FP32 GPS_GetDirection(void); 
extern int GPS_GetUserSatNum(void);   
extern FP32 GPS_GetError(void);   
#endif   //__GPS_H_
//======================================================================
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//======================================================================
