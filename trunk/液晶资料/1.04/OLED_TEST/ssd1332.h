//Ŀ��оƬ : M16 
//���ߣ��³�
//�汾���ڶ���
//ʱ��: 8.0000Mhz 
#ifndef _SSD1332_H_
#define _SSD1332_H_

#include <iccioavr.h>
#include <macros.h>

#define uchar unsigned char
#define uint  unsigned int

#define OLED_RECTANGLE_FILL_ENABLE()    WriteCommand(0x26),WriteCommand(0x01)
#define OLED_RECTANGLE_FILL_DISABLE()   WriteCommand(0x26),WriteCommand(0x00)

/*--------------------------------------------------------------- 
������AVR��OLED������Ϣ 
PA->D0--D7
PC3->RD
PC2->WR
PC1->DC
PC0->CS
PD7->RES
Ҫʹ�ñ��������ı�����������Ϣ���� 
-----------------------------------------------------------------*/ 
#define OLED_RD_PORT    PORTC   //����4��Ҫ��Ϊͬһ���� 
#define OLED_RD_DDR     DDRC 
#define OLED_WR_PORT    PORTC   
#define OLED_WR_DDR     DDRC 
#define OLED_DC_PORT    PORTC    
#define OLED_DC_DDR     DDRC 
#define OLED_CS_PORT    PORTD   
#define OLED_CS_DDR     DDRD 
//����Ϊ�ĸ���Ҫ��������ض���
#define OLED_RES_PORT    PORTC   //RES��Ҫ��Ϊͬһ���� 
#define OLED_RES_DDR     DDRC 
//����Ϊ��λ����
#define OLED_DATA_PORT  PORTA   
#define OLED_DATA_DDR   DDRA    
#define OLED_DATA_PIN   PINA
//����Ϊ���ݶ˿ڶ���
#define OLED_RD         (1<<PC3) 
#define OLED_WR         (1<<PC2)
#define OLED_DC         (1<<PC7) 
#define OLED_RES        (1<<PC6)
#define OLED_CS         (1<<PD7) 
//����Ϊ�ӿڶ���


extern unsigned char ReadData(void);
extern unsigned char ReadCommand(void);
extern void WriteCommand(unsigned com);
extern void WriteData(unsigned dat);
extern void OLED_init(void);
extern void OLED_dot(uchar x,uchar y,uchar R,uchar G,uchar B);
extern void OLED_circle(uchar X,uchar Y,uchar R,uchar R1,uchar G1,uchar B1); 
extern void OLED_draw_rectangle(uchar x1,uchar y1,uchar x2,uchar y2,uchar R1,uchar G1,uchar B1,uchar R2,uchar G2,uchar B2);
extern void OLED_draw_line(uchar x1,uchar y1,uchar x2,uchar y2,uchar R,uchar G,uchar B);
extern void OLED_draw_img(uchar x,uchar y,uchar x_size,uchar y_size,__flash uchar image[]);
extern void OLED_putstring(uchar x,uchar y,uchar colorH,uchar colorL,uchar *text);
extern void OLED_write_chinese(uchar x,uchar y,uchar num,__flash uchar buff[][32]);

#endif
