#ifndef __IODEVICE_H__
#define __IODEVICE_H__

#define VOICE_STOP			0xf		//中断所有正在播出语音
#define	VOICE_DI_LIGHT		0xe		//嘀（小音量）,正常设置按键时的按键音
#define VOICE_DI_BIG		0xd		//嘀（大音量）,超声测距时
#define VOICE_START_MUSIC	0xc		//开机音乐
#define	VOICE_HALT_MUSIC	0xb		//关机音乐
#define	VOICE_TACH_5		0xa		//噹！+5环+加油！加油
#define	VOICE_TACH_6		0x9		//噹！+6环
#define	VOICE_TACH_7		0x8		//噹！+7环
#define	VOICE_TACH_8		0x7		//噹！+8环
#define	VOICE_TACH_9		0x6		//噹！+9环
#define	VOICE_TACH_10		0x5		//噹！+10环 + 棒极了
#define VOICE_DANG			0x4		//噹！,打在5环以下其它各区域
#define	VOICE_GOGO			0x3		//加油！加油！
#define	VOICE_BEGIN			0x2		//开始吧！
#define	VOICE_GOOD			0x1		//棒极了！
#define	VOICE_NULL			0X0		//常态,无声


#define	LED_1		1
#define	LED_2		2
#define	LED_3		3
#define	LED_4		4
#define	LED_ON		1
#define	LED_OFF		0

#define KEY_BACK	0x3E
#define KEY_SELECT	0x3D
#define KEY_UP		0x3B
#define KEY_DOWN	0x37
#define KEY_LEFT	0x2F
#define KEY_RIGHT	0x1F

#define COLOR_ORANGE	0
#define COLOR_RED		1
#define COLOR_GREEN		2
#define COLOR_NULL		3

extern unsigned char Flag_KeyRepeat;	//连续按键是否有效标志

extern void IO_Device_Init(void);
extern void Voice_play(unsigned short sound);
extern unsigned char Key_scan(void);
extern void Led_flash_all(unsigned short bits);
extern void Led_flash_one(unsigned char bit,unsigned char val);
extern void Led_flash_non(unsigned char bit);

extern void Leds_DispFill(unsigned char color);

#endif
