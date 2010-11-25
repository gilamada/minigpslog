#ifndef __DS18B20_H
#define __DS18B20_H

#define DS_PRECISION	0x7f   //精度配置寄存器 1f=9位; 3f=10位; 5f=11位; 7f=12位;
#define DS_AlarmTH		0x64
#define DS_AlarmTL		0x8a

void DS18B20_start(void);
float DS18b20_read(void);


#endif

