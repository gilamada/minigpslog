// stdafx.cpp : source file that includes just the standard includes
//	Compass1.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

//********************************************************************
// 전역 변수의 원형 선언
sComPort g_sPortCom1;
sComPort g_sPortCom2;

HWND g_hViewWnd;

CCommThread *g_pCPortCom1;
CCommThread *g_pCPortCom2;
BYTE g_byRcvBuf[MAX_RECEIVE];
BYTE g_bySendBuf[MAX_SEND];
//*********************************************************************