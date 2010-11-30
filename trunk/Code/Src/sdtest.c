/******************************************************************************
* ����STM32��RTCģ����ʾ����
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "rtc.h"
#include "spi_sd.h"
#include "ff.h"
#include "diskio.h"
//#include <stdio.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
time_t current_time;
struct tm time_now;

//vu32 TimeDisplay;

u8 state;

FATFS fs;
FRESULT res;
UINT br;


/* Private function prototypes -----------------------------------------------*/
//extern void Init_Device(void);
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int SD_Test(void)
{
    u16 i;
	FIL file;
	char data[512];
	UINT number=0;
	char *s="hello,lightjpu\n";

    //Init_Device();
    //RTC_Start();
    SPI_Configuration();

    res = f_mount(0, &fs);

    //res = f_mkfs(0,0,0);
    // if(res!=FR_OK)
   // {
//		while(1);
   // }
	
    res = f_open(&file, "data.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
    
    if(res!=FR_OK)
    {
		while(1);
    }
    
    for(i=0;i<512;i++)
	{
		data[i] = 0x38;
	}

	res = f_write(&file, data, 512, &number);
	if(res!=FR_OK)
    {
		while(1);
    }
	
    while(1)
    {
        //if(fgets(data, sizeof(data), &file)==NULL)
        //{
            break;
        //}
        //prints(data);
    }

    f_close(&file);

     
   // write_time = Time_GetUnixTime();                            
    res = f_open(&file, "331.txt", FA_CREATE_ALWAYS | FA_WRITE);
    for(i=0;i<3;i++)                                            
    {                                                           
        res = f_write(&file, s, 14, &br);                   
        if(br<14)  //�ж��Ƿ����д��                          
        {                                                       
            break;                                              
        }                                                       
    }                                                           
   // write_time = Time_GetUnixTime() - write_time;               
        f_close(&file);                                         
    



    SD_PWR_OFF();
    while(1)
    {
        
        /* 
        if(TimeDisplay)                                                           
        {                                                                         
            current_time = Time_GetUnixTime();                                    
            time_now = Time_GetCalendarTime();                                    
                                                                                  
            USART_SendData(USART1, 0x0c);                                         
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);             
                                                                                  
            printf("\r\nUNIXʱ�䣺%d", current_time);                             
            printf("\t��ǰʱ�䣺%d-%d-%d %d %02d:%02d:%02d\t", time_now.tm_year, \
                   time_now.tm_mon+1, time_now.tm_mday, time_now.tm_wday+1,\      
                   time_now.tm_hour, time_now.tm_min, time_now.tm_sec);           
            TimeDisplay = 0;                                                      
        }                                                                         
        */
        
    }
}

