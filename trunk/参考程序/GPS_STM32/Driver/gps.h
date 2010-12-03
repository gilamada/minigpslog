//=====================================================================
// 文件名称: GPS.H
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
#ifndef __GPS_H_
#define __GPS_H_

//============================= 宏定义 =================================
#define  NEMA_NUM_MAX   10     //缓冲的NEMA语句数量
#define  NEMA_CHAR_MAX  150   //缓冲的NEMA语句字符数量 

#define GPS_BOOT_H()    GPIOC->BSRR = GPIO_Pin_1
#define GPS_BOOT_L()    GPIOC->BRR  = GPIO_Pin_1
#define GPS_POWER_OFF()  GPIOC->BSRR = GPIO_Pin_0
#define GPS_POWER_ON()   GPIOC->BRR  = GPIO_Pin_0
//============================= 结构体定义 ===============================  
typedef	struct 
{ 
    int satid;      //卫星序号 
    int elevation;  //卫星仰角（00 - 90）度
    int azimuth;    //卫星方位角（00 - 359）度
    int snr;        //信噪比（00－99）dbHz 
} SatelliteInfo; 


typedef  struct
{

    char isvalid;  //GPS 定位标志   0=未定位，1=已定位
    int hh,mm,ss,ms;

    int DD, MM, YY;
    FP32 latitude;
    u8 latNS;    
    FP32 longitude;
    u8   lgtEW;
    FP32 speed;       //地面速度，GPS输出单位节(Knots), 已经转化位KM/H
    FP32 direction;   //方位角，度 ，以真北为参考

    FP32  altitude;     //海拔高度
    u8 altitudeunit;       //海拔单位

    u8  FixMode;     //GPS状态，0=未定位，1=非差分定位，2=差分定位，3=无效PPS，6=正在估算
    u8 GSA_mode1;//定位模式，A=自动手动2D/3D，M=手动2D/3D 
    u8 GSA_mode2;//定位类型，1=未定位，2=2D定位，3=3D定位 

    FP32 PDOP;          //综合位置精度因子
    FP32 HDOP;          //水平精度因子
    FP32 VDOP;          //垂直精度因子 
    u32  ageOfDiff;//差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空） 
    u16  diffStationID;//差分站ID号0000 - 1023（前导位数不足则补0，如果不是差分定位将为空）
     
    u8 usedsat[12];//正在用来解算的卫星序号
    u8 usedsatnum;  //正在使用的卫星数量（00 - 12）
    u8 allsatnum;  //当前可见卫星总数（00 - 12）
    SatelliteInfo satinfo[38];
}GPSINFO;


typedef struct
{
    u8   isupdated;  //NEMA 缓冲区更新标志
    char buffer[NEMA_CHAR_MAX]; //NEMA 缓冲区
}GPS_NEMA_STRUCT;


//============================= 变量声明 ===============================
extern  GPS_NEMA_STRUCT GPS_NEMA [NEMA_NUM_MAX] ; 
extern  GPSINFO  gps;


extern void FATFS_WriteNEMA( char *ptr);  //

//============================= 函数声明 ===============================
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
