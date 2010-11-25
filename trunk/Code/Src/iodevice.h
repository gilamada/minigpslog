#ifndef __IODEVICE_H__
#define __IODEVICE_H__

#define VOICE_STOP			0xf		//�ж��������ڲ�������
#define	VOICE_DI_LIGHT		0xe		//�֣�С������,�������ð���ʱ�İ�����
#define VOICE_DI_BIG		0xd		//�֣���������,�������ʱ
#define VOICE_START_MUSIC	0xc		//��������
#define	VOICE_HALT_MUSIC	0xb		//�ػ�����
#define	VOICE_TACH_5		0xa		//����+5��+���ͣ�����
#define	VOICE_TACH_6		0x9		//����+6��
#define	VOICE_TACH_7		0x8		//����+7��
#define	VOICE_TACH_8		0x7		//����+8��
#define	VOICE_TACH_9		0x6		//����+9��
#define	VOICE_TACH_10		0x5		//����+10�� + ������
#define VOICE_DANG			0x4		//����,����5����������������
#define	VOICE_GOGO			0x3		//���ͣ����ͣ�
#define	VOICE_BEGIN			0x2		//��ʼ�ɣ�
#define	VOICE_GOOD			0x1		//�����ˣ�
#define	VOICE_NULL			0X0		//��̬,����


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

extern unsigned char Flag_KeyRepeat;	//���������Ƿ���Ч��־

extern void IO_Device_Init(void);
extern void Voice_play(unsigned short sound);
extern unsigned char Key_scan(void);
extern void Led_flash_all(unsigned short bits);
extern void Led_flash_one(unsigned char bit,unsigned char val);
extern void Led_flash_non(unsigned char bit);

extern void Leds_DispFill(unsigned char color);

#endif
