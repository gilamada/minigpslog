#ifndef __RTC_H__
#define __RTC_H__

#include <time.h>

extern struct tm SysTime;

void RTC_Start(void);
struct tm Time_UnixToCalendar(time_t t);		//����һ��Unixʱ�����time_t��������Calendar��ʽ����
time_t Time_CalendarToUnix(struct tm t);		//����һ��Calendar��ʽ���ڣ�����Unixʱ�����time_t��
time_t Time_GetUnixTime(void);					//��RTCȡ��ǰʱ���Unixʱ���ֵ
struct tm Time_GetCalendarTime(void);			//��RTCȡ��ǰʱ�������ʱ��
void Time_SetUnixTime(time_t);					//����UNIXʱ�����ʽʱ�䣬����Ϊ��ǰRTCʱ��
void Time_SetCalendarTime(struct tm t_tm);	//����Calendar��ʽʱ�䣬����Ϊ��ǰRTCʱ��


#endif
