#ifndef _RTC_time_H_
#define _RTC_time_H_
#include <time.h>

#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

extern char *weekday[];

struct tm Time_ConvUnixToCalendar(time_t t);
time_t Time_ConvCalendarToUnix(struct tm t);
time_t Time_GetUnixTime(void);
struct tm Time_GetCalendarTime(void);
void Time_SetUnixTime(time_t);
void Time_SetCalendarTime(struct tm t);

void RTC_Configuration(void);
void Setup_RTC(void);

#endif
