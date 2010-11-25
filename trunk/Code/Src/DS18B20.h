#ifndef __DS18B20_H
#define __DS18B20_H

#define DS_PRECISION	0x7f   //�������üĴ��� 1f=9λ; 3f=10λ; 5f=11λ; 7f=12λ;
#define DS_AlarmTH		0x64
#define DS_AlarmTL		0x8a

void DS18B20_start(void);
float DS18b20_read(void);


#endif

