//================================================================================================
// �ļ�����: main.c 
// оƬ����: STM32103VCT6                                            
// �������: Keil v3.90, Windows XP2 
// ����ʱ��: 2010��01��05��                                                               
// ������: 
// ��д��Ա: �޳�                                                                       
// ��ϵ��ʽ: 13686036830  soulcoffee@sohu.com   
//================================================================================================ 

//========================================= ͷ�ļ� ================================================ 	
#include "config.h"
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

//======================================== �������� =============================================== 
 void GPS_Display(void);
//======================================== �������� =============================================== 

char GPRMC[]="$GPRMC,014638.000,A,2241.3323,N,11356.9003,E,0.00,,160810,,,A*78\r\n";
char GPGGA[]="$GPGGA,014639.000,2241.3323,N,11356.9003,E,1,04,6.8,98.7,M,-2.8,M,,0000*70\r\n";
char GPGSA[]="$GPGSA,A,3,22,06,14,29,,,,,,,,,7.4,6.8,2.8*3D\r\n";
char GPGSV1[]="$GPGSV,3,1,11,14,56,098,39,16,53,228,17,31,47,002,15,32,36,294,*75\r\n";
char GPGSV2[]="$GPGSV,3,2,11,29,24,072,27,20,18,306,,22,15,169,28,06,13,184,30*71\r\n";
char GPGSV3[]="$GPGSV,3,3,11,30,12,039,,03,02,197,,24,00,198,*4B\r\n";

//SD_Error Status = SD_OK;
//SD_CardInfo SDCardInfo;

/* Private functions ---------------------------------------------------------*/
    FATFS fs;            // Work area (file system object) for logical drive
    FIL fil;      // file objects
    //BYTE buffer[512]; // file copy buffer
    FRESULT res;         // FatFs function common result code
    UINT br, bw;         // File R/W count

    char filName[15];

 void FATFS_WriteNEMA( char *ptr)
 {
   
  static u16 count = 0;

  f_write(&fil,ptr,strlen(ptr),&bw);

  if(count++ > 20)
    {
        f_sync(&fil);
        count = 0; 
    }
  //f_close(&fil); 
  //f_mount(0, NULL); 
 }
                              
void FATFS_CreateFile(void)
{
  
  disk_initialize(0);    
  f_mount(0, &fs);

 if(FR_NO_FILE ==f_open(&fil,filName, FA_WRITE ))
    f_open(&fil,filName, FA_WRITE | FA_CREATE_ALWAYS);
  
  
  res = f_lseek(&fil,fil.fsize);
   
  //res = f_write(&fil,GPRMC,sizeof(GPRMC),&bw); 
  //res = f_write(&fil,GPGSA,sizeof(GPGSA),&bw); 
 // FATFS_WriteNEMA(GPGGA);
 // f_close(&fil); 
 // f_mount(0, NULL); 

}
//============================================================================= 
//�� �� ��: USB_SDCardRead() 
//��    ��: ��ʼUSB������  
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//============================================================================= 
int USB_SDCardRead(void)
{
  Set_System();
  Set_USBClock();
  Led_Config();
  USB_Interrupts_Config();
  USB_Init();
  while (bDeviceState != CONFIGURED); 
  USB_Configured_LED();

  while (1)
  {}
}
//============================================================================= 
//�� �� ��: main() 
//��    ��: C��ں���  
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//============================================================================= 
int main (void)
{

    u8 bl_level=0;

	SystemInit(); 

  	TargetResetInit(); //Ŀ��������ʼ��  
    LEDS_Configuration();
    KEY_Configuration();
    USB_CheckPlugInit();
    GPS_init();  

   GUI_Initialize();// ��ʼ��LCM 
   GUI_BacklightOFF();   
   GUI_ClearSCR();     
   GUI_SetTextMode(GUI_TEXTMODE_NORMAL); 
   
  // printp("\nSystemCoreClock = %dM\n",SystemCoreClock_Mhz );
    if(Bit_RESET == KEY2)
    { 
      GUI_DispString("GPS --> PC",0,0);
      USB_SDCardRead();
    }   

    GPS_Run();
    GPS_Display();
    Delay_ms(3000);
    GPS_INFO_update(); 
    GPS_INFO_update();
    GPS_INFO_update(); 
    sprintf(filName,"GPS%02d%02d.txt\0",gps.MM,gps.DD);


  if (MAL_Init(0) == MAL_OK)
  { 
	printp("\nSD Card OK!\n");
  } 
   FATFS_CreateFile();  //���ɼ�¼�ļ�
  
   

//	 GUI_SetEnFont(En_8x8);
  /*  GPS_parse(GPRMC);
    GPS_parse(GPGGA);
    GPS_parse(GPGSA);
    GPS_parse(GPGSV1);
    GPS_parse(GPGSV2);
    GPS_parse(GPGSV3);   */
    
    while(1){ 
     /*   if(TRUE == USB_CheckPlug())        //�������� ���������������
        { 
           GUI_DispString("GPS --> PC",0,0);
           USB_SDCardRead();
        }  */

         GPS_Display() ;
    
        if(Bit_RESET == KEY2)
          {
       
                GPS_Stop();
          }

        if(Bit_RESET == KEY4)
        {

            GPS_Run();
          if(bl_level<160)
          {  
             bl_level += 40;
             PWM_Configuration( bl_level);
          }else{
             bl_level = 0;
             PWM_Configuration( bl_level);
          }
        }
    }
}


void GPS_Display(void)
{    int dd,mm;
    FP32 ss;
    static int last_sec = 0; 
	char str[80]; 
    static u32 update_times=0;

     
     GPS_INFO_update(); 

     if(last_sec != gps.ss)   //�������� ��ˢ����ʾ
     {
      
       last_sec = gps.ss;//����gps��������
  

        GPS_DegreeToDMS(gps.latitude, &dd, &mm,&ss);
        sprintf(str,"%s:%3d %2d'%05.2f\"  ",(gps.latNS == 'N')?"��γ":"��γ",dd, mm, ss);
        // sprintf(str,"%s: %.6f��    ",(gps.latNS == 'N')?"��γ":"��γ",gps.latitude);
        GUI_DispString(str,0,0); 

        GPS_DegreeToDMS(gps.longitude, &dd, &mm,&ss);
        sprintf(str,"%s:%3d %2d'%05.2f\"  ",(gps.lgtEW == 'E')?"����":"����",dd, mm, ss);
        GUI_DispString(str,0,16);
        sprintf(str,"����:%.1f%c    ",gps.altitude,gps.altitudeunit);
        GUI_DispString(str,0,32);
        sprintf(str,"�ٶ�:%.1fkm/h    ",gps.speed);
        GUI_DispString(str,0,48);
        sprintf(str,"����:%.1f��    ",gps.direction);
        GUI_DispString(str,0,64);
        sprintf(str,"ʱ��:%2d:%02d:%02d ",(gps.hh<16)?gps.hh+8:gps.hh-16,gps.mm,gps.ss);
        GUI_DispString(str,0,80);    
        sprintf(str,"����:20%02d-%d-%d  ",gps.YY,gps.MM,gps.DD);
        GUI_DispString(str,0,96);  
        sprintf(str,"����:%2d/%2d",gps.usedsatnum,gps.allsatnum);
        GUI_DispString(str,0,112); 

        switch(gps.FixMode)
        {
            case 0: sprintf(str,"GPS״̬:δ��λ   ");break;
            case 1: sprintf(str,"GPS״̬:%dD SPS  ",gps.GSA_mode2);break;
            case 2: sprintf(str,"GPS״̬:%dD DGPS ",gps.GSA_mode2);break;
            case 6: sprintf(str,"GPS״̬:������   ");break;
            default :break;                                                 
         }
        GUI_DispString(str,0,128);     
        sprintf(str,"Update %4d times!",update_times++);
        GUI_DispString(str,0,144);
   }

}

   /*  {
		printp("\r");
		printp("\r");
		printp("\r                          \\\\\\|///                       ");  
        printp("\r                        \\\\  - -  //                      ");
        printp("\r                         (  @ @  )                         ");
        printp("\r +---------------------oOOo-(_)-oOOo----------------------+");
        printp("\r |                                                        |");
        printp("\r |           Welcome to use STM32F103V                    |");
        printp("\r |                --Designer: CC--                        |");
        printp("\r |               --Date:2010.08.05--                      |");
        printp("\r |                    QQ:52020081                         |");
        printp("\r |         USART Config--COM:115200bps,8bool,NP            |");
        printp("\r |                             Oooo                       |");
        printp("\r +-----------------------oooO--(   )----------------------+");
        printp("\r                        (   )   ) / 					   ");
        printp("\r                         \\ (   (_/ 				       ");
        printp("\r                          \\_)      					   ");
		printp("\r");
		printp("\r");
        Delay_s(1);
    } 
    */


     /*  //����Ƿ������̼��������������⣬�����ȹص�
    if(Bit_RESET == KEY2)
    {
      Delay_ms(10);
      if(Bit_RESET == KEY2)
      {
        GPS_FirmwareUpdate();
        while(1)
        {
         LED_Reverse(LED2);
         Delay_ms(200);
        }
        
      }

    }  */
//================================================================================================= 
//                                   0ooo
//                          ooo0     (   ) 
//                          (   )     ) /
//                           \ (     (_/
//                            \_) 
//================================================================================================= 
