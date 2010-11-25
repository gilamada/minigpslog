// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__EF0C08FE_8791_4310_A37F_FD27B3F0268F__INCLUDED_)
#define AFX_STDAFX_H__EF0C08FE_8791_4310_A37F_FD27B3F0268F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//**********************************************************************************
#include ".\CommThread\CommThread.h" 

typedef struct
{
	int xPort;

	int xBaud;
	int xData;
	int xStop;
	int xParity;

} sComPort;		//시리얼 포트의 설정값을 저장할 구조체..

//--> 사용자 이벤트 정의.. Com 1 에서 수신..
#define  U_MSG_RECEIVE_COM1		(WM_USER + 10)

//--> 사용자 이벤트 정의.. Com 2 에서 수신..
#define  U_MSG_RECEIVE_COM2		(WM_USER + 11)


//--> 최대 수신 바이트수..버퍼 한계치..
#define  MAX_RECEIVE			(4096 + 1)

//--> 최대 송신 바이트수..버퍼 한계치..
#define  MAX_SEND				(256 + 1)


//--> 포트 번호.
#define  PORT_COM1				0
#define  PORT_COM2				1


//===== 전역변수 선언 =====
extern HWND g_hViewWnd;				//View 의 핸들

extern sComPort g_sPortCom1;		// Com 1 Port의 설정 상태 저장..
extern sComPort g_sPortCom2;		// Com 1 Port의 설정 상태 저장..


extern CCommThread *g_pCPortCom1;	//Com 1 Port 를 감시할 쓰레드.
extern CCommThread *g_pCPortCom2;	//Com 2 Port 를 감시할 쓰레드.	


//extern BYTE g_byRcvBuf[2][MAX_RECEIVE];	//수신 버퍼
//extern BYTE g_bySendBuf[2][MAX_SEND];	//송신 버퍼
extern BYTE g_byRcvBuf[MAX_RECEIVE];	//수신 버퍼
extern BYTE g_bySendBuf[MAX_SEND];	//송신 버퍼
//**************************************************************************************


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__EF0C08FE_8791_4310_A37F_FD27B3F0268F__INCLUDED_)
