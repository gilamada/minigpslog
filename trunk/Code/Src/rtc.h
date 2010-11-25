#ifndef __RTC_H__
#define __RTC_H__

#include <time.h>

extern struct tm SysTime;

void RTC_Start(void);
struct tm Time_UnixToCalendar(time_t t);		//输入一个Unix时间戳（time_t），返回Calendar格式日期
time_t Time_CalendarToUnix(struct tm t);		//输入一个Calendar格式日期，返回Unix时间戳（time_t）
time_t Time_GetUnixTime(void);					//从RTC取当前时间的Unix时间戳值
struct tm Time_GetCalendarTime(void);			//从RTC取当前时间的日历时间
void Time_SetUnixTime(time_t);					//输入UNIX时间戳格式时间，设置为当前RTC时间
void Time_SetCalendarTime(struct tm t_tm);	//输入Calendar格式时间，设置为当前RTC时间


#endif
