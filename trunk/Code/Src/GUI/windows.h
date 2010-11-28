/****************************************************************************************
* �ļ�����WINDOWS.H
* ���ܣ����ڹ�����ͷ�ļ������ڶ���������ݽṹ��
* ���ߣ����ܱ�
* ���ڣ�2003.09.19
* ��ע��
****************************************************************************************/

#ifndef  WINDOWS_H
#define  WINDOWS_H

/* ���崰�����ݽṹ */
typedef  struct
{  uint32  x;			// ����λ��(���Ͻǵ�x����)
   uint32  y;			// ����λ��(���Ͻǵ�y����)
    
   uint32  with;		// ���ڿ���
   uint32  hight;		// ���ڸ߶�
   
   uint8   *title;		// ���������ָ�� (�����ַ�ΪASCII�ַ������������ܴ�������)
   uint8   *state;		// ����״̬��ָ�� (��Ϊ��ʱ����ʾ״̬��)   
} WINDOWS;



/****************************************************************************
* ���ƣ�GUI_WindowsDraw()
* ���ܣ���ʾ���ڡ������ṩ�Ĵ��ڲ������л�����
* ��ڲ�����dat		��������(���ںڰ�ɫLCM��Ϊ0�ĵ���Ϊ1�ĵ���ʾ)
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
extern  uint8  GUI_WindowsDraw(WINDOWS *win);



/****************************************************************************
* ���ƣ�GUI_WindowsHide()
* ���ܣ��������ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
extern uint8  GUI_WindowsHide(WINDOWS *win);



/****************************************************************************
* ���ƣ�GUI_WindowsClr()
* ���ܣ��������ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ�������
* ˵����ʹ�ô˺���ǰҪ��ʹ��GUI_WindowsDraw()�����ڻ�����
****************************************************************************/
extern  void  GUI_WindowsClr(WINDOWS *win);


#endif
