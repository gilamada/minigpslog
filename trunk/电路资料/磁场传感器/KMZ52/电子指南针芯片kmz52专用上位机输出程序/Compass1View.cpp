// Compass1View.cpp : implementation of the CCompass1View class
//

#include "stdafx.h"
#include "Compass1.h"

#include "Compass1Doc.h"
#include "Compass1View.h"

#include "StdAfx.h" //****************************************************
#include "CommThread\CommThread.h" //*************************************

#include "math.h"

#define PI 3.14159265358979323846
#define c_offset 320

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompass1View

IMPLEMENT_DYNCREATE(CCompass1View, CFormView)

BEGIN_MESSAGE_MAP(CCompass1View, CFormView)
	//{{AFX_MSG_MAP(CCompass1View)
	ON_BN_CLICKED(IDC_CONNECTOR, OnConnector)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_CHECK_COMPASS1, OnCheckCompass1)
	ON_BN_CLICKED(IDC_CHECK_COMPASS2, OnCheckCompass2)
	ON_BN_CLICKED(IDC_CHECK_COMPASS3, OnCheckCompass3)
	ON_BN_CLICKED(IDC_CHECK_COMPASS4, OnCheckCompass4)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_GRID, OnCheckGrid)
	ON_BN_CLICKED(IDC_CHECK_ANGLE, OnCheckAngle)
	ON_BN_CLICKED(IDC_CAL1, OnCal1)
	ON_MESSAGE( U_MSG_RECEIVE_COM1, OnReceiveData)	//<= com1 수신 이벤트 반응함수 ****************
	ON_BN_CLICKED(IDC_CAL2, OnCal2)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompass1View construction/destruction

CCompass1View::CCompass1View()
	: CFormView(CCompass1View::IDD)
{
	//{{AFX_DATA_INIT(CCompass1View)
	m_nCompass1X = 0;
	m_nCompass1Xin = 0;
	m_nCompass1Xmax = 0;
	m_nCompass1Xmin = 0;
	m_nCompass1Xref = 512;
	m_nCompass1Y = 0;
	m_nCompass1Yin = 0;
	m_nCompass1Ymax = 0;
	m_nCompass1Ymin = 0;
	m_nCompass1Yref = 512;
	m_nCompass1Z = 0;
	m_nCompass1Zmax = 0;
	m_nCompass1Zmin = 0;
	m_nCompass2X = 0;
	m_nCompass2Xin = 0;
	m_nCompass2Xmax = 0;
	m_nCompass2Xmin = 0;
	m_nCompass2Xref = 512;
	m_nCompass2Y = 0;
	m_nCompass2Yin = 0;
	m_nCompass2Ymax = 0;
	m_nCompass2Ymin = 0;
	m_nCompass2Yref = 512;
	m_nCompass2Z = 0;
	m_nCompass2Zmax = 0;
	m_nCompass2Zmin = 0;
	m_nCompass3X = 0;
	m_nCompass3Xin = 0;
	m_nCompass3Xmax = 0;
	m_nCompass3Xmin = 0;
	m_nCompass3Xref = 512;
	m_nCompass3Y = 0;
	m_nCompass3Yin = 0;
	m_nCompass3Ymax = 0;
	m_nCompass3Ymin = 0;
	m_nCompass3Yref = 512;
	m_nCompass3Z = 0;
	m_nCompass3Zmax = 0;
	m_nCompass3Zmin = 0;
	m_nCompass4X = 0;
	m_nCompass4Xin = 0;
	m_nCompass4Xmax = 0;
	m_nCompass4Xmin = 0;
	m_nCompass4Xref = 512;
	m_nCompass4Y = 0;
	m_nCompass4Yin = 0;
	m_nCompass4Ymax = 0;
	m_nCompass4Ymin = 0;
	m_nCompass4Yref = 512;
	m_nCompass4Z = 0;
	m_nCompass4Zmax = 0;
	m_nCompass4Zmin = 0;
	m_strCompass1Adeg = "0";
	m_strCompass1Arad = "0";
	m_strCompass1Err = "0";
	m_strCompass1Xerr = "0";
	m_strCompass1Yerr = "0";
	m_strCompass1Zerr = "0";
	m_strCompass2Adeg = "0";
	m_strCompass2Arad = "0";
	m_strCompass2Err = "0";
	m_strCompass2Xerr = "0";
	m_strCompass2Yerr = "0";
	m_strCompass3Adeg = "0";
	m_strCompass3Arad = "0";
	m_strCompass3Err = "0";
	m_strCompass3Xerr = "0";
	m_strCompass3Yerr = "0";
	m_strCompass3Zerr = "0";
	m_strCompass4Adeg = "0";
	m_strCompass4Arad = "0";
	m_strCompass4Err = "0";
	m_strCompass4Xerr = "0";
	m_strCompass4Yerr = "0";
	m_strCompass4Zerr = "0";
	m_strCompass2Zerr = "0";
	m_strEncoder1 = "0";
	m_strEncoder2 = "0";
	m_strEncoder3 = "0";
	m_strEncoder4 = "0";
	m_strEncoder1Ang = "0";
	m_strEncoder2Ang = "0";
	m_strEncoder3Ang = "0";
	m_strEncoder4Ang = "0";
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_bCheck1	  = FALSE;
	m_bCheck2	  = FALSE;
	m_bCheck3	  = FALSE;
	m_bCheck4	  = FALSE;
	m_bCheckGrid  = FALSE;
	m_bCheckAngle = FALSE;

	m_Pt1_1.x = c_offset;
	m_Pt1_1.y = c_offset;
	m_Pt1_2.x = c_offset;
	m_Pt1_2.y = c_offset;
	m_Pt2_1.x = c_offset;
	m_Pt2_1.y = c_offset;
	m_Pt2_2.x = c_offset;
	m_Pt2_2.y = c_offset;
	m_Pt3_1.x = c_offset;
	m_Pt3_1.y = c_offset;
	m_Pt3_2.x = c_offset;
	m_Pt3_2.y = c_offset;
	m_Pt4_1.x = c_offset;
	m_Pt4_1.y = c_offset;
	m_Pt4_2.x = c_offset;
	m_Pt4_2.y = c_offset;

	m_ecdPt1_1.x = c_offset;
	m_ecdPt1_1.y = c_offset;
	m_ecdPt1_2.x = c_offset;
	m_ecdPt1_2.y = c_offset;
	m_ecdPt2_1.x = c_offset;
	m_ecdPt2_1.y = c_offset;
	m_ecdPt2_2.x = c_offset;
	m_ecdPt2_2.y = c_offset;
	m_ecdPt3_1.x = c_offset;
	m_ecdPt3_1.y = c_offset;
	m_ecdPt3_2.x = c_offset;
	m_ecdPt3_2.y = c_offset;
	m_ecdPt4_1.x = c_offset;
	m_ecdPt4_1.y = c_offset;
	m_ecdPt4_2.x = c_offset;
	m_ecdPt4_2.y = c_offset;




	m_crCompass1 = RGB(255,0,0); // Red
	m_crCompass2 = RGB(0,0,255); // Blue
	m_crCompass3 = RGB(0,200,0); // Green
	m_crCompass4 = RGB(100,100,100); // Gray
}

CCompass1View::~CCompass1View()
{
}

void CCompass1View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompass1View)
	DDX_Control(pDX, IDC_CHECK_COMPASS4, m_ctrCheckCompass4);
	DDX_Control(pDX, IDC_CHECK_COMPASS3, m_ctrCheckCompass3);
	DDX_Control(pDX, IDC_CHECK_COMPASS2, m_ctrCheckCompass2);
	DDX_Control(pDX, IDC_CHECK_COMPASS1, m_ctrCheckCompass1);
	DDX_Control(pDX, IDC_CHECK_GRID, m_ctrGrid);
	DDX_Control(pDX, IDC_CHECK_ANGLE, m_ctrAngle);
	DDX_Text(pDX, IDC_EDIT_COMPASS1X, m_nCompass1X);
	DDX_Text(pDX, IDC_EDIT_COMPASS1XIN, m_nCompass1Xin);
	DDX_Text(pDX, IDC_EDIT_COMPASS1XMAX, m_nCompass1Xmax);
	DDX_Text(pDX, IDC_EDIT_COMPASS1XMIN, m_nCompass1Xmin);
	DDX_Text(pDX, IDC_EDIT_COMPASS1XREF, m_nCompass1Xref);
	DDX_Text(pDX, IDC_EDIT_COMPASS1Y, m_nCompass1Y);
	DDX_Text(pDX, IDC_EDIT_COMPASS1YIN, m_nCompass1Yin);
	DDX_Text(pDX, IDC_EDIT_COMPASS1YMAX, m_nCompass1Ymax);
	DDX_Text(pDX, IDC_EDIT_COMPASS1YMIN, m_nCompass1Ymin);
	DDX_Text(pDX, IDC_EDIT_COMPASS1YREF, m_nCompass1Yref);
	DDX_Text(pDX, IDC_EDIT_COMPASS1Z, m_nCompass1Z);
	DDX_Text(pDX, IDC_EDIT_COMPASS1ZMAX, m_nCompass1Zmax);
	DDX_Text(pDX, IDC_EDIT_COMPASS1ZMIN, m_nCompass1Zmin);
	DDX_Text(pDX, IDC_EDIT_COMPASS2X, m_nCompass2X);
	DDX_Text(pDX, IDC_EDIT_COMPASS2XIN, m_nCompass2Xin);
	DDX_Text(pDX, IDC_EDIT_COMPASS2XMAX, m_nCompass2Xmax);
	DDX_Text(pDX, IDC_EDIT_COMPASS2XMIN, m_nCompass2Xmin);
	DDX_Text(pDX, IDC_EDIT_COMPASS2XREF, m_nCompass2Xref);
	DDX_Text(pDX, IDC_EDIT_COMPASS2Y, m_nCompass2Y);
	DDX_Text(pDX, IDC_EDIT_COMPASS2YIN, m_nCompass2Yin);
	DDX_Text(pDX, IDC_EDIT_COMPASS2YMAX, m_nCompass2Ymax);
	DDX_Text(pDX, IDC_EDIT_COMPASS2YMIN, m_nCompass2Ymin);
	DDX_Text(pDX, IDC_EDIT_COMPASS2YREF, m_nCompass2Yref);
	DDX_Text(pDX, IDC_EDIT_COMPASS2Z, m_nCompass2Z);
	DDX_Text(pDX, IDC_EDIT_COMPASS2ZMAX, m_nCompass2Zmax);
	DDX_Text(pDX, IDC_EDIT_COMPASS2ZMIN, m_nCompass2Zmin);
	DDX_Text(pDX, IDC_EDIT_COMPASS3X, m_nCompass3X);
	DDX_Text(pDX, IDC_EDIT_COMPASS3XIN, m_nCompass3Xin);
	DDX_Text(pDX, IDC_EDIT_COMPASS3XMAX, m_nCompass3Xmax);
	DDX_Text(pDX, IDC_EDIT_COMPASS3XMIN, m_nCompass3Xmin);
	DDX_Text(pDX, IDC_EDIT_COMPASS3XREF, m_nCompass3Xref);
	DDX_Text(pDX, IDC_EDIT_COMPASS3Y, m_nCompass3Y);
	DDX_Text(pDX, IDC_EDIT_COMPASS3YIN, m_nCompass3Yin);
	DDX_Text(pDX, IDC_EDIT_COMPASS3YMAX, m_nCompass3Ymax);
	DDX_Text(pDX, IDC_EDIT_COMPASS3YMIN, m_nCompass3Ymin);
	DDX_Text(pDX, IDC_EDIT_COMPASS3YREF, m_nCompass3Yref);
	DDX_Text(pDX, IDC_EDIT_COMPASS3Z, m_nCompass3Z);
	DDX_Text(pDX, IDC_EDIT_COMPASS3ZMAX, m_nCompass3Zmax);
	DDX_Text(pDX, IDC_EDIT_COMPASS3ZMIN, m_nCompass3Zmin);
	DDX_Text(pDX, IDC_EDIT_COMPASS4X, m_nCompass4X);
	DDX_Text(pDX, IDC_EDIT_COMPASS4XIN, m_nCompass4Xin);
	DDX_Text(pDX, IDC_EDIT_COMPASS4XMAX, m_nCompass4Xmax);
	DDX_Text(pDX, IDC_EDIT_COMPASS4XMIN, m_nCompass4Xmin);
	DDX_Text(pDX, IDC_EDIT_COMPASS4XREF, m_nCompass4Xref);
	DDX_Text(pDX, IDC_EDIT_COMPASS4Y, m_nCompass4Y);
	DDX_Text(pDX, IDC_EDIT_COMPASS4YIN, m_nCompass4Yin);
	DDX_Text(pDX, IDC_EDIT_COMPASS4YMAX, m_nCompass4Ymax);
	DDX_Text(pDX, IDC_EDIT_COMPASS4YMIN, m_nCompass4Ymin);
	DDX_Text(pDX, IDC_EDIT_COMPASS4YREF, m_nCompass4Yref);
	DDX_Text(pDX, IDC_EDIT_COMPASS4Z, m_nCompass4Z);
	DDX_Text(pDX, IDC_EDIT_COMPASS4ZMAX, m_nCompass4Zmax);
	DDX_Text(pDX, IDC_EDIT_COMPASS4ZMIN, m_nCompass4Zmin);
	DDX_Text(pDX, IDC_EDIT_COMPASS1ADEG, m_strCompass1Adeg);
	DDX_Text(pDX, IDC_EDIT_COMPASS1ARAD, m_strCompass1Arad);
	DDX_Text(pDX, IDC_EDIT_COMPASS1ERR, m_strCompass1Err);
	DDX_Text(pDX, IDC_EDIT_COMPASS1XERR, m_strCompass1Xerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS1YERR, m_strCompass1Yerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS1ZERR, m_strCompass1Zerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS2ADEG, m_strCompass2Adeg);
	DDX_Text(pDX, IDC_EDIT_COMPASS2ARAD, m_strCompass2Arad);
	DDX_Text(pDX, IDC_EDIT_COMPASS2ERR, m_strCompass2Err);
	DDX_Text(pDX, IDC_EDIT_COMPASS2XERR, m_strCompass2Xerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS2YERR, m_strCompass2Yerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS3ADEG, m_strCompass3Adeg);
	DDX_Text(pDX, IDC_EDIT_COMPASS3ARAD, m_strCompass3Arad);
	DDX_Text(pDX, IDC_EDIT_COMPASS3ERR, m_strCompass3Err);
	DDX_Text(pDX, IDC_EDIT_COMPASS3XERR, m_strCompass3Xerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS3YERR, m_strCompass3Yerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS3ZERR, m_strCompass3Zerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS4ADEG, m_strCompass4Adeg);
	DDX_Text(pDX, IDC_EDIT_COMPASS4ARAD, m_strCompass4Arad);
	DDX_Text(pDX, IDC_EDIT_COMPASS4ERR, m_strCompass4Err);
	DDX_Text(pDX, IDC_EDIT_COMPASS4XERR, m_strCompass4Xerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS4YERR, m_strCompass4Yerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS4ZERR, m_strCompass4Zerr);
	DDX_Text(pDX, IDC_EDIT_COMPASS2ZERR, m_strCompass2Zerr);
	DDX_Text(pDX, IDC_EDIT_ENCODER1, m_strEncoder1);
	DDX_Text(pDX, IDC_EDIT_ENCODER2, m_strEncoder2);
	DDX_Text(pDX, IDC_EDIT_ENCODER3, m_strEncoder3);
	DDX_Text(pDX, IDC_EDIT_ENCODER4, m_strEncoder4);
	DDX_Text(pDX, IDC_EDIT_ENCODER1ANG, m_strEncoder1Ang);
	DDX_Text(pDX, IDC_EDIT_ENCODER2ANG, m_strEncoder2Ang);
	DDX_Text(pDX, IDC_EDIT_ENCODER3ANG, m_strEncoder3Ang);
	DDX_Text(pDX, IDC_EDIT_ENCODER4ANG, m_strEncoder4Ang);
	//}}AFX_DATA_MAP
}

BOOL CCompass1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CCompass1View::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CCompass1View printing

BOOL CCompass1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCompass1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCompass1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCompass1View::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CCompass1View diagnostics

#ifdef _DEBUG
void CCompass1View::AssertValid() const
{
	CFormView::AssertValid();
}

void CCompass1View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCompass1Doc* CCompass1View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCompass1Doc)));
	return (CCompass1Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCompass1View message handlers

void CCompass1View::OnConnector() 
{
	// TODO: Add your control notification handler code here
	g_hViewWnd = this->m_hWnd;
	g_pCPortCom1 = new CCommThread;
	CommPort.bInitComPort("COM1",CBR_9600,8,ONESTOPBIT,NOPARITY);
}

void CCompass1View::OnClose() 
{
	// TODO: Add your control notification handler code here
	CommPort.ClosePort();
}

long CCompass1View::OnReceiveData(WPARAM wParam, LPARAM lParam) //***********************
{
	UpdateData(TRUE);
	int nRcvSize = (INT)lParam;
	if(nRcvSize <= 0) return -1;

	strRcvData(PORT_COM1, nRcvSize);

	CommPort.CommPortBufferClear();
	return TRUE;
}

void CCompass1View::strRcvData(int xPortNo, int nRcvSize) //*****************************
{
	BYTE ch;
	static   int seq_no=0;
	int ID;

//------------------------------ 수신 받기 -----------------------------------
	for(int i=0; i<nRcvSize; i++)
	{
		ch = g_byRcvBuf[i];

		switch(seq_no)
		{
			case 0:
			{
				if(ch==0xff) seq_no=1;          // Start code가 0인가 Check
				else seq_no=0;
				break;
			}
			case 1:
			{
				if(ch<=4)						// 1,2,3,4 까지 ID를 허용
				{
					ID=ch;						// ID 등록
					seq_no=2;
				}
				else seq_no=0;
				break;
			}
			case 2:								// X data High
			{
				if(ch<=4)
				{
					if(ID==1) X1_data_H=ch;
					if(ID==2) X2_data_H=ch;
					if(ID==3) X3_data_H=ch;
					if(ID==4) X4_data_H=ch;
					seq_no=3;
				}
				else seq_no=0;
				break;
			}
			case 3:								// X data Low
			{
					if(ID==1) X1_data_L=ch;
					if(ID==2) X2_data_L=ch;
					if(ID==3) X3_data_L=ch;
					if(ID==4) X4_data_L=ch;
					seq_no=4;
					break;
			}
			case 4:								// Y data High
			{
				if(ch<=4)
				{
					if(ID==1) Y1_data_H=ch;
					if(ID==2) Y2_data_H=ch;
					if(ID==3) Y3_data_H=ch;
					if(ID==4) Y4_data_H=ch;
					seq_no=5;
				}
				else seq_no=0;
				break;
			}
			case 5:								// Y data Low
			{
					if(ID==1) Y1_data_L=ch;
					if(ID==2) Y2_data_L=ch;
					if(ID==3) Y3_data_L=ch;
					if(ID==4) Y4_data_L=ch;
					seq_no=6;
					break;
			}
			case 6:								// Encoder High
			{
					if(ID==1) Encoder1_H=ch;
					if(ID==2) Encoder2_H=ch;
					if(ID==3) Encoder3_H=ch;
					if(ID==4) Encoder4_H=ch;
					seq_no=7;
					break;
			}
			case 7:								// Encoder Low
			{
					if(ID==1)
					{
						Encoder1_L=ch;
						GridDraw();
						ID_1_Routine();
					}
					if(ID==2)
					{
						Encoder2_L=ch;
						ID_2_Routine();
					}
					if(ID==3)
					{
						Encoder3_L=ch;
						ID_3_Routine();
					}
					if(ID==4)
					{
						Encoder4_L=ch;
						ID_4_Routine();
					}
					seq_no=0;
					break;
			}
			default:
			{
					seq_no=0;
					break;
			}
		}

	}
	UpdateData(FALSE);
}

void CCompass1View::OnStart() 
{
	// TODO: Add your control notification handler code here
	BackgroundDraw();
	CircleDraw();
	GridDraw();

	OnCheckCompass1(); // Check 버튼 확인
	OnCheckCompass2();
	OnCheckCompass3();
	OnCheckCompass4();

	m_Pt1_1.x = c_offset;
	m_Pt1_1.y = c_offset;
	m_Pt1_2.x = c_offset;
	m_Pt1_2.y = c_offset;
	m_Pt2_1.x = c_offset;
	m_Pt2_1.y = c_offset;
	m_Pt2_2.x = c_offset;
	m_Pt2_2.y = c_offset;
	m_Pt3_1.x = c_offset;
	m_Pt3_1.y = c_offset;
	m_Pt3_2.x = c_offset;
	m_Pt3_2.y = c_offset;
	m_Pt4_1.x = c_offset;
	m_Pt4_1.y = c_offset;
	m_Pt4_2.x = c_offset;
	m_Pt4_2.y = c_offset;
	
	m_ecdPt1_1.x = c_offset;
	m_ecdPt1_1.y = c_offset;
	m_ecdPt1_2.x = c_offset;
	m_ecdPt1_2.y = c_offset;
	m_ecdPt2_1.x = c_offset;
	m_ecdPt2_1.y = c_offset;
	m_ecdPt2_2.x = c_offset;
	m_ecdPt2_2.y = c_offset;
	m_ecdPt3_1.x = c_offset;
	m_ecdPt3_1.y = c_offset;
	m_ecdPt3_2.x = c_offset;
	m_ecdPt3_2.y = c_offset;
	m_ecdPt4_1.x = c_offset;
	m_ecdPt4_1.y = c_offset;
	m_ecdPt4_2.x = c_offset;
	m_ecdPt4_2.y = c_offset;


	g_hViewWnd = this->m_hWnd;
	g_pCPortCom1 = new CCommThread;
	CommPort.bInitComPort("COM1",CBR_9600,8,ONESTOPBIT,NOPARITY);	
}

void CCompass1View::OnStop() 
{
	// TODO: Add your control notification handler code here
	CommPort.ClosePort();	
}

void CCompass1View::OnClear() 
{
	// TODO: Add your control notification handler code here
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();

	m_fCompass1Adeg = 0.0f;
	m_fCompass1Arad = 0.0f;
	m_fCompass1Err = 0.0f;
	m_nCompass1X = 0;
	m_fCompass1Xerr = 0.0f;
	m_nCompass1Xin = 0;
	m_nCompass1Xmax = 0;
	m_nCompass1Xmin = 0;
//	m_nCompass1Xref = 512;
	m_nCompass1Y = 0;
	m_fCompass1Yerr = 0.0f;
	m_nCompass1Yin = 0;
	m_nCompass1Ymax = 0;
	m_nCompass1Ymin = 0;
//	m_nCompass1Yref = 512;
	m_nCompass1Z = 0;
	m_fCompass1Zerr = 0.0f;
	m_nCompass1Zmax = 0;
	m_nCompass1Zmin = 0;
	m_fCompass2Adeg = 0.0f;
	m_fCompass2Arad = 0.0f;
	m_fCompass2Err = 0.0f;
	m_nCompass2X = 0;
	m_fCompass2Xerr = 0.0f;
	m_nCompass2Xin = 0;
	m_nCompass2Xmax = 0;
	m_nCompass2Xmin = 0;
//	m_nCompass2Xref = 512;
	m_nCompass2Y = 0;
	m_fCompass2Yerr = 0.0f;
	m_nCompass2Yin = 0;
	m_nCompass2Ymax = 0;
	m_nCompass2Ymin = 0;
//	m_nCompass2Yref = 512;
	m_nCompass2Z = 0;
	m_fCompass2Zerr = 0.0f;
	m_nCompass2Zmax = 0;
	m_nCompass2Zmin = 0;
	m_fCompass3Adeg = 0.0f;
	m_fCompass3Arad = 0.0f;
	m_fCompass3Err = 0.0f;
	m_nCompass3X = 0;
	m_fCompass3Xerr = 0.0f;
	m_nCompass3Xin = 0;
	m_nCompass3Xmax = 0;
	m_nCompass3Xmin = 0;
//	m_nCompass3Xref = 512;
	m_nCompass3Y = 0;
	m_fCompass3Yerr = 0.0f;
	m_nCompass3Yin = 0;
	m_nCompass3Ymax = 0;
	m_nCompass3Ymin = 0;
//	m_nCompass3Yref = 512;
	m_nCompass3Z = 0;
	m_fCompass3Zerr = 0.0f;
	m_nCompass3Zmax = 0;
	m_nCompass3Zmin = 0;
	m_fCompass4Adeg = 0.0f;
	m_fCompass4Arad = 0.0f;
	m_fCompass4Err = 0.0f;
	m_nCompass4X = 0;
	m_fCompass4Xerr = 0.0f;
	m_nCompass4Xin = 0;
	m_nCompass4Xmax = 0;
	m_nCompass4Xmin = 0;
//	m_nCompass4Xref = 512;
	m_nCompass4Y = 0;
	m_fCompass4Yerr = 0.0f;
	m_nCompass4Yin = 0;
	m_nCompass4Ymax = 0;
	m_nCompass4Ymin = 0;
//	m_nCompass4Yref = 512;
	m_nCompass4Z = 0;
	m_fCompass4Zerr = 0.0f;
	m_nCompass4Zmax = 0;
	m_nCompass4Zmin = 0;

	Invalidate();
	UpdateData(FALSE);

	m_Pt1_1.x = c_offset;
	m_Pt1_1.y = c_offset;
	m_Pt1_2.x = c_offset;
	m_Pt1_2.y = c_offset;
	m_Pt2_1.x = c_offset;
	m_Pt2_1.y = c_offset;
	m_Pt2_2.x = c_offset;
	m_Pt2_2.y = c_offset;
	m_Pt3_1.x = c_offset;
	m_Pt3_1.y = c_offset;
	m_Pt3_2.x = c_offset;
	m_Pt3_2.y = c_offset;
	m_Pt4_1.x = c_offset;
	m_Pt4_1.y = c_offset;
	m_Pt4_2.x = c_offset;
	m_Pt4_2.y = c_offset;

	m_ecdPt1_1.x = c_offset;
	m_ecdPt1_1.y = c_offset;
	m_ecdPt1_2.x = c_offset;
	m_ecdPt1_2.y = c_offset;
	m_ecdPt2_1.x = c_offset;
	m_ecdPt2_1.y = c_offset;
	m_ecdPt2_2.x = c_offset;
	m_ecdPt2_2.y = c_offset;
	m_ecdPt3_1.x = c_offset;
	m_ecdPt3_1.y = c_offset;
	m_ecdPt3_2.x = c_offset;
	m_ecdPt3_2.y = c_offset;
	m_ecdPt4_1.x = c_offset;
	m_ecdPt4_1.y = c_offset;
	m_ecdPt4_2.x = c_offset;
	m_ecdPt4_2.y = c_offset;


}

void CCompass1View::BackgroundDraw()
{
	CDC* pDC=GetDC(); // 배경색 지정 부분

	CRect rect;
	GetClientRect(rect);
	rect.right=640;
	rect.bottom=700;
	COLORREF m_crBackground;
	CBrush   m_wndbkBrush;
	m_crBackground=GetSysColor(4);
	m_wndbkBrush.CreateSolidBrush(m_crBackground);
	pDC->FillRect(&rect,&m_wndbkBrush);

	ReleaseDC(pDC);
}

void CCompass1View::CircleDraw()
{
	CDC* pDC=GetDC();

	CPen my,*old;		// 원을 그리는 부분
	my.CreatePen(PS_SOLID,1,RGB(220,220,220));
	old=pDC->SelectObject(&my);
	pDC->Arc(c_offset- 50,c_offset- 50,c_offset+ 50,c_offset+ 50,200,200,50,50);
	pDC->Arc(c_offset-100,c_offset-100,c_offset+100,c_offset+100,200,200,50,50);
	pDC->Arc(c_offset-150,c_offset-150,c_offset+150,c_offset+150,200,200,50,50);
	pDC->Arc(c_offset-200,c_offset-200,c_offset+200,c_offset+200,200,200,50,50);
	pDC->Arc(c_offset-250,c_offset-250,c_offset+250,c_offset+250,200,200,50,50);
	pDC->Arc(c_offset-300,c_offset-300,c_offset+300,c_offset+300,200,200,50,50);
	pDC->SelectObject(old);
	my.DeleteObject();

	ReleaseDC(pDC);
}

void CCompass1View::GridDraw()
{
	CDC* pDC=GetDC();

	CPen my,*old;		
	my.CreatePen(PS_SOLID,1,RGB(210,210,210)); // 회색 GRID 그리는 부분
	old=pDC->SelectObject(&my);
	pDC->MoveTo(c_offset-300,c_offset-300); pDC->LineTo(c_offset+300,c_offset-300);
	pDC->MoveTo(c_offset-300,c_offset-250); pDC->LineTo(c_offset+300,c_offset-250);
	pDC->MoveTo(c_offset-300,c_offset-200);	pDC->LineTo(c_offset+300,c_offset-200);
	pDC->MoveTo(c_offset-300,c_offset-150);	pDC->LineTo(c_offset+300,c_offset-150);
	pDC->MoveTo(c_offset-300,c_offset-100);	pDC->LineTo(c_offset+300,c_offset-100);
	pDC->MoveTo(c_offset-300,c_offset -50);	pDC->LineTo(c_offset+300,c_offset-50);
	pDC->MoveTo(c_offset-300,c_offset +50);	pDC->LineTo(c_offset+300,c_offset+50);
	pDC->MoveTo(c_offset-300,c_offset+100);	pDC->LineTo(c_offset+300,c_offset+100);
	pDC->MoveTo(c_offset-300,c_offset+150);	pDC->LineTo(c_offset+300,c_offset+150);
	pDC->MoveTo(c_offset-300,c_offset+200);	pDC->LineTo(c_offset+300,c_offset+200);
	pDC->MoveTo(c_offset-300,c_offset+250);	pDC->LineTo(c_offset+300,c_offset+250);
	pDC->MoveTo(c_offset-300,c_offset+300);	pDC->LineTo(c_offset+300,c_offset+300);
	
	pDC->MoveTo(c_offset-300,c_offset-300);	pDC->LineTo(c_offset-300,c_offset+300);
	pDC->MoveTo(c_offset-250,c_offset-300);	pDC->LineTo(c_offset-250,c_offset+300);
	pDC->MoveTo(c_offset-200,c_offset-300);	pDC->LineTo(c_offset-200,c_offset+300);
	pDC->MoveTo(c_offset-150,c_offset-300);	pDC->LineTo(c_offset-150,c_offset+300);
	pDC->MoveTo(c_offset-100,c_offset-300);	pDC->LineTo(c_offset-100,c_offset+300);
	pDC->MoveTo(c_offset -50,c_offset-300);	pDC->LineTo(c_offset -50,c_offset+300);
	pDC->MoveTo(c_offset +50,c_offset-300);	pDC->LineTo(c_offset +50,c_offset+300);
	pDC->MoveTo(c_offset+100,c_offset-300);	pDC->LineTo(c_offset+100,c_offset+300);
	pDC->MoveTo(c_offset+150,c_offset-300);	pDC->LineTo(c_offset+150,c_offset+300);
	pDC->MoveTo(c_offset+200,c_offset-300);	pDC->LineTo(c_offset+200,c_offset+300);
	pDC->MoveTo(c_offset+250,c_offset-300);	pDC->LineTo(c_offset+250,c_offset+300);
	pDC->MoveTo(c_offset+300,c_offset-300);	pDC->LineTo(c_offset+300,c_offset+300);
	pDC->SelectObject(old);
	my.DeleteObject();


	my.CreatePen(PS_SOLID,2,RGB(0,0,0)); // 검정색 LINE 그리는 부분
	old=pDC->SelectObject(&my);
 	pDC->MoveTo(c_offset-300,c_offset);
 	pDC->LineTo(c_offset+300,c_offset);
 	pDC->MoveTo(c_offset	,c_offset-300);
 	pDC->LineTo(c_offset	,c_offset+300);
 	pDC->MoveTo(c_offset	,c_offset);

 	pDC->MoveTo(c_offset-300,c_offset);	pDC->LineTo(c_offset-300,c_offset-6);
 	pDC->MoveTo(c_offset-275,c_offset);	pDC->LineTo(c_offset-275,c_offset-3);
 	pDC->MoveTo(c_offset-250,c_offset);	pDC->LineTo(c_offset-250,c_offset-6);
 	pDC->MoveTo(c_offset-225,c_offset);	pDC->LineTo(c_offset-225,c_offset-3);
 	pDC->MoveTo(c_offset-200,c_offset);	pDC->LineTo(c_offset-200,c_offset-6);
 	pDC->MoveTo(c_offset-175,c_offset);	pDC->LineTo(c_offset-175,c_offset-3);
 	pDC->MoveTo(c_offset-150,c_offset);	pDC->LineTo(c_offset-150,c_offset-6);
 	pDC->MoveTo(c_offset-125,c_offset);	pDC->LineTo(c_offset-125,c_offset-3);
 	pDC->MoveTo(c_offset-100,c_offset);	pDC->LineTo(c_offset-100,c_offset-6);
 	pDC->MoveTo(c_offset- 75,c_offset);	pDC->LineTo(c_offset- 75,c_offset-3);
 	pDC->MoveTo(c_offset- 50,c_offset);	pDC->LineTo(c_offset- 50,c_offset-6);
 	pDC->MoveTo(c_offset- 25,c_offset);	pDC->LineTo(c_offset- 25,c_offset-3);
 	pDC->MoveTo(c_offset+ 25,c_offset);	pDC->LineTo(c_offset+ 25,c_offset-3);
 	pDC->MoveTo(c_offset+ 50,c_offset);	pDC->LineTo(c_offset+ 50,c_offset-6);
 	pDC->MoveTo(c_offset+ 75,c_offset);	pDC->LineTo(c_offset+ 75,c_offset-3);
 	pDC->MoveTo(c_offset+100,c_offset);	pDC->LineTo(c_offset+100,c_offset-6);
 	pDC->MoveTo(c_offset+125,c_offset);	pDC->LineTo(c_offset+125,c_offset-3);
 	pDC->MoveTo(c_offset+150,c_offset);	pDC->LineTo(c_offset+150,c_offset-6);
 	pDC->MoveTo(c_offset+175,c_offset);	pDC->LineTo(c_offset+175,c_offset-3);
 	pDC->MoveTo(c_offset+200,c_offset);	pDC->LineTo(c_offset+200,c_offset-6);
 	pDC->MoveTo(c_offset+225,c_offset);	pDC->LineTo(c_offset+225,c_offset-3);
 	pDC->MoveTo(c_offset+250,c_offset);	pDC->LineTo(c_offset+250,c_offset-6);
 	pDC->MoveTo(c_offset+275,c_offset);	pDC->LineTo(c_offset+275,c_offset-3);
 	pDC->MoveTo(c_offset+300,c_offset);	pDC->LineTo(c_offset+300,c_offset-6);

 	pDC->MoveTo(c_offset,c_offset-300);	pDC->LineTo(c_offset+6,c_offset-300);
 	pDC->MoveTo(c_offset,c_offset-275);	pDC->LineTo(c_offset+3,c_offset-275);
 	pDC->MoveTo(c_offset,c_offset-250);	pDC->LineTo(c_offset+6,c_offset-250);
	pDC->MoveTo(c_offset,c_offset-225);	pDC->LineTo(c_offset+3,c_offset-225);
	pDC->MoveTo(c_offset,c_offset-200);	pDC->LineTo(c_offset+6,c_offset-200);
	pDC->MoveTo(c_offset,c_offset-175);	pDC->LineTo(c_offset+3,c_offset-175);
	pDC->MoveTo(c_offset,c_offset-150);	pDC->LineTo(c_offset+6,c_offset-150);
	pDC->MoveTo(c_offset,c_offset-125);	pDC->LineTo(c_offset+3,c_offset-125);
	pDC->MoveTo(c_offset,c_offset-100);	pDC->LineTo(c_offset+6,c_offset-100);
	pDC->MoveTo(c_offset,c_offset- 75);	pDC->LineTo(c_offset+3,c_offset- 75);
	pDC->MoveTo(c_offset,c_offset- 50);	pDC->LineTo(c_offset+6,c_offset- 50);
	pDC->MoveTo(c_offset,c_offset- 25);	pDC->LineTo(c_offset+3,c_offset- 25);
	pDC->MoveTo(c_offset,c_offset+ 25);	pDC->LineTo(c_offset+3,c_offset+ 25);
	pDC->MoveTo(c_offset,c_offset+ 50);	pDC->LineTo(c_offset+6,c_offset+ 50);
	pDC->MoveTo(c_offset,c_offset+ 75);	pDC->LineTo(c_offset+3,c_offset+ 75);
	pDC->MoveTo(c_offset,c_offset+100);	pDC->LineTo(c_offset+6,c_offset+100);
	pDC->MoveTo(c_offset,c_offset+125);	pDC->LineTo(c_offset+3,c_offset+125);
	pDC->MoveTo(c_offset,c_offset+150);	pDC->LineTo(c_offset+6,c_offset+150);
	pDC->MoveTo(c_offset,c_offset+175);	pDC->LineTo(c_offset+3,c_offset+175);
	pDC->MoveTo(c_offset,c_offset+200);	pDC->LineTo(c_offset+6,c_offset+200);
	pDC->MoveTo(c_offset,c_offset+225);	pDC->LineTo(c_offset+3,c_offset+225);
	pDC->MoveTo(c_offset,c_offset+250);	pDC->LineTo(c_offset+6,c_offset+250);
	pDC->MoveTo(c_offset,c_offset+275);	pDC->LineTo(c_offset+3,c_offset+275);
	pDC->MoveTo(c_offset,c_offset+300);	pDC->LineTo(c_offset+6,c_offset+300);


	CFont font;
	pDC->SelectObject(&font);
	pDC->SetBkMode(1); // 배경색과 같이 투명하게 설정

 	pDC->TextOut(c_offset-310,c_offset+5,"-300");
 	pDC->TextOut(c_offset-260,c_offset+5,"-250");
 	pDC->TextOut(c_offset-210,c_offset+5,"-200");
 	pDC->TextOut(c_offset-160,c_offset+5,"-150");
 	pDC->TextOut(c_offset-110,c_offset+5,"-100");
 	pDC->TextOut(c_offset- 60,c_offset+5,"-50");
 	pDC->TextOut(c_offset+ 40,c_offset+5,"50");
 	pDC->TextOut(c_offset+ 90,c_offset+5,"100");
 	pDC->TextOut(c_offset+140,c_offset+5,"150");
 	pDC->TextOut(c_offset+190,c_offset+5,"200");
 	pDC->TextOut(c_offset+240,c_offset+5,"250");
 	pDC->TextOut(c_offset+290,c_offset+5,"300");

 	pDC->TextOut(c_offset-30,c_offset-308,"-300");
 	pDC->TextOut(c_offset-30,c_offset-258,"-250");
 	pDC->TextOut(c_offset-30,c_offset-208,"-200");
 	pDC->TextOut(c_offset-30,c_offset-158,"-150");
 	pDC->TextOut(c_offset-30,c_offset-108,"-100");
 	pDC->TextOut(c_offset-22,c_offset- 58,"-50");
 	pDC->TextOut(c_offset-24,c_offset+ 42,"50");
 	pDC->TextOut(c_offset-30,c_offset+ 92,"100");
 	pDC->TextOut(c_offset-30,c_offset+142,"150");
 	pDC->TextOut(c_offset-30,c_offset+192,"200");
 	pDC->TextOut(c_offset-30,c_offset+242,"250");
 	pDC->TextOut(c_offset-30,c_offset+292,"300");

	pDC->SelectObject(old);
	my.DeleteObject();

	ReleaseDC(pDC);
}

void CCompass1View::ID_1_Routine()
{
	if(m_bCheck1==FALSE) return;

	i_X1_data  = X1_data_H << 8;
	i_X1_data += X1_data_L;
	i_Y1_data  = Y1_data_H << 8;
	i_Y1_data += Y1_data_L;

	i_X1_data = 1023 - i_X1_data;
	m_nCompass1Xin = i_X1_data;
	m_nCompass1Yin = i_Y1_data;

	i_X1_data = i_X1_data - m_nCompass1Xref;
	i_Y1_data = i_Y1_data - m_nCompass1Yref;

	f_X1_data = (float)(i_X1_data);
	f_Y1_data = (float)(i_Y1_data);

	if(i_X1_data == 0 && i_Y1_data > 0) f_angle1 = 270;
	if(i_X1_data == 0 && i_Y1_data < 0) f_angle1 =  90;
	if(i_X1_data >  0 && i_Y1_data > 0) f_angle1 = (float)(360 - (atan(f_Y1_data/f_X1_data))*180/PI);
	if(i_X1_data >  0 && i_Y1_data < 0) f_angle1 = (float)(  0 - (atan(f_Y1_data/f_X1_data))*180/PI);
	if(i_X1_data <  0				  ) f_angle1 = (float)(180 - (atan(f_Y1_data/f_X1_data))*180/PI);

	if(i_X1_data < m_nCompass1Xmin) m_nCompass1Xmin = i_X1_data;
	if(i_X1_data > m_nCompass1Xmax) m_nCompass1Xmax = i_X1_data;
	if(i_Y1_data < m_nCompass1Ymin) m_nCompass1Ymin = i_Y1_data;
	if(i_Y1_data > m_nCompass1Ymax) m_nCompass1Ymax = i_Y1_data;

	m_fCompass1Xerr = (float)(m_nCompass1Xmax + m_nCompass1Xmin)/2;
	m_strCompass1Xerr.Format("%1.0f",m_fCompass1Xerr);
	m_fCompass1Yerr = (float)(m_nCompass1Ymax + m_nCompass1Ymin)/2;
	m_strCompass1Yerr.Format("%1.0f",m_fCompass1Yerr);
	
	m_nCompass1X = i_X1_data;
	m_nCompass1Y = i_Y1_data;
	
	m_fCompass1Adeg = f_angle1;
	m_fCompass1Arad = (float)(f_angle1 * PI / 180);
	m_fCompass1Adeg = (float)((int)(m_fCompass1Adeg * 100 + 0.5)) / 100;
	m_fCompass1Arad = (float)((int)(m_fCompass1Arad * 100 + 0.5)) / 100;

	m_strCompass1Arad.Format("%1.02f",m_fCompass1Arad);
	m_strCompass1Adeg.Format("%1.02f",m_fCompass1Adeg);


	int i_Encoder1X;
	int i_Encoder1Y;
	
	i_Encoder1 = Encoder1_H << 8;
	i_Encoder1+= Encoder1_L;


//	i_Encoder1 = i_Encoder1;

	i_Encoder1 = i_Encoder1%2000;
	if(i_Encoder1==0) i_Encoder1=1;
	m_strEncoder1.Format("%d",i_Encoder1);


	
	f_Encoder1 = (float)(i_Encoder1);
	f_Encoder1 =  (float)(2 * PI / 2000 * f_Encoder1);
	m_strEncoder1Ang.Format("%1.01f",f_Encoder1*180/PI);

	i_Encoder1X = (int)(300*sin(f_Encoder1));
	i_Encoder1Y = (int)(300*cos(f_Encoder1));
	
	
	UpdateData(FALSE);

	CDC* pDC=GetDC();

	CPen my,*old;
	my.CreatePen(PS_SOLID,2,m_crCompass1); // RED
	old=pDC->SelectObject(&my);
	

	pDC->Rectangle(	i_Encoder1X+c_offset-2,	i_Encoder1Y+c_offset-2,	//*****
					i_Encoder1X+c_offset+2,	i_Encoder1Y+c_offset+2);//*****


	pDC->MoveTo(i_X1_data+c_offset-3,	i_Y1_data+c_offset-3);
	pDC->LineTo(i_X1_data+c_offset+3,	i_Y1_data+c_offset+3);
	pDC->MoveTo(i_X1_data+c_offset+3,	i_Y1_data+c_offset-3);
	pDC->LineTo(i_X1_data+c_offset-3,	i_Y1_data+c_offset+3);
	pDC->SelectObject(old);
	my.DeleteObject();


	if(m_bCheckAngle==TRUE)
	{
	my.CreatePen(PS_SOLID,2,m_crCompass1);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt1_1.x,m_Pt1_1.y);    // Vector(과거 선을 지움)
	pDC->LineTo(m_Pt1_2.x,m_Pt1_2.y);

	pDC->MoveTo(m_ecdPt1_1.x,m_ecdPt1_1.y);		//*****
	pDC->LineTo(m_ecdPt1_2.x,m_ecdPt1_2.y);		//*****
	
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	if(m_bCheckGrid==TRUE)
	{
	my.CreatePen(PS_DOT,1,m_crCompass1);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt1_2.x,c_offset-300); // Cross(과거 선을 지움)
	pDC->LineTo(m_Pt1_2.x,m_Pt1_2.y-10);
	pDC->MoveTo(m_Pt1_2.x,m_Pt1_2.y+10);
	pDC->LineTo(m_Pt1_2.x,c_offset+300);	
	pDC->MoveTo(c_offset-300,m_Pt1_2.y);
	pDC->LineTo(m_Pt1_2.x-10,m_Pt1_2.y);
	pDC->MoveTo(m_Pt1_2.x+10,m_Pt1_2.y);
	pDC->LineTo(c_offset+300,m_Pt1_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	m_Pt1_1.x = c_offset;
	m_Pt1_1.y = c_offset;
	m_Pt1_2.x = i_X1_data+c_offset;
	m_Pt1_2.y = i_Y1_data+c_offset;

	m_ecdPt1_1.x = c_offset;
	m_ecdPt1_1.y = c_offset;
	m_ecdPt1_2.x = i_Encoder1X+c_offset;
	m_ecdPt1_2.y = i_Encoder1Y+c_offset;

	if(m_bCheckAngle==TRUE)
	{
	my.CreatePen(PS_SOLID,2,m_crCompass1);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt1_1.x,m_Pt1_1.y);    // Vector
	pDC->LineTo(m_Pt1_2.x,m_Pt1_2.y);

	pDC->MoveTo(m_ecdPt1_1.x,m_ecdPt1_1.y);		//*****
	pDC->LineTo(m_ecdPt1_2.x,m_ecdPt1_2.y);		//*****
	
	pDC->SelectObject(old);
	my.DeleteObject();
	}
	
	if(m_bCheckGrid==TRUE)
	{
	my.CreatePen(PS_DOT,1,m_crCompass1);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt1_2.x,c_offset-300); // Cross
	pDC->LineTo(m_Pt1_2.x,m_Pt1_2.y-10);
	pDC->MoveTo(m_Pt1_2.x,m_Pt1_2.y+10);
	pDC->LineTo(m_Pt1_2.x,c_offset+300);	
	pDC->MoveTo(c_offset-300,m_Pt1_2.y);
	pDC->LineTo(m_Pt1_2.x-10,m_Pt1_2.y);
	pDC->MoveTo(m_Pt1_2.x+10,m_Pt1_2.y);
	pDC->LineTo(c_offset+300,m_Pt1_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	ReleaseDC(pDC);
}

void CCompass1View::ID_2_Routine()
{
	if(m_bCheck2==FALSE) return;

	i_X2_data  = X2_data_H << 8;
	i_X2_data += X2_data_L;
	i_Y2_data  = Y2_data_H << 8;
	i_Y2_data += Y2_data_L;

	i_X2_data = 1023 - i_X2_data;
	m_nCompass2Xin = i_X2_data;
	m_nCompass2Yin = i_Y2_data;

	i_X2_data = i_X2_data - m_nCompass2Xref;
	i_Y2_data = i_Y2_data - m_nCompass2Yref;

	f_X2_data = (float)(i_X2_data);
	f_Y2_data = (float)(i_Y2_data);

	if(i_X2_data == 0 && i_Y2_data > 0) f_angle2 = 270;
	if(i_X2_data == 0 && i_Y2_data < 0) f_angle2 =  90;
	if(i_X2_data >  0 && i_Y2_data > 0) f_angle2 = (float)(360 - (atan(f_Y2_data/f_X2_data))*180/PI);
	if(i_X2_data >  0 && i_Y2_data < 0) f_angle2 = (float)(  0 - (atan(f_Y2_data/f_X2_data))*180/PI);
	if(i_X2_data <  0			      ) f_angle2 = (float)(180 - (atan(f_Y2_data/f_X2_data))*180/PI);

	if(i_X2_data < m_nCompass2Xmin) m_nCompass2Xmin = i_X2_data;
	if(i_X2_data > m_nCompass2Xmax) m_nCompass2Xmax = i_X2_data;
	if(i_Y2_data < m_nCompass2Ymin) m_nCompass2Ymin = i_Y2_data;
	if(i_Y2_data > m_nCompass2Ymax) m_nCompass2Ymax = i_Y2_data;

	m_fCompass2Xerr = (float)(m_nCompass2Xmax + m_nCompass2Xmin)/2;
	m_strCompass2Xerr.Format("%1.0f",m_fCompass2Xerr);
	m_fCompass2Yerr = (float)(m_nCompass2Ymax + m_nCompass2Ymin)/2;
	m_strCompass2Yerr.Format("%1.0f",m_fCompass2Yerr);


	m_nCompass2X = i_X2_data;
	m_nCompass2Y = i_Y2_data;
	
	m_fCompass2Adeg = f_angle2;
	m_fCompass2Arad = (float)(f_angle2 * PI / 180);
	m_fCompass2Adeg = (float)((int)(m_fCompass2Adeg * 100 + 0.5)) / 100;
	m_fCompass2Arad = (float)((int)(m_fCompass2Arad * 100 + 0.5)) / 100;

	m_strCompass2Arad.Format("%1.02f",m_fCompass2Arad);
	m_strCompass2Adeg.Format("%1.02f",m_fCompass2Adeg);

	CDC* pDC=GetDC();

	CPen my,*old;
	my.CreatePen(PS_SOLID,2,m_crCompass2); // BLUE
	old=pDC->SelectObject(&my);

//	pDC->Rectangle(	i_X2_data+c_offset-2,	i_Y2_data+c_offset-2,
//					i_X2_data+c_offset+2,	i_Y2_data+c_offset+2);

	pDC->MoveTo(i_X2_data+c_offset-3,	i_Y2_data+c_offset-3);
	pDC->LineTo(i_X2_data+c_offset+3,	i_Y2_data+c_offset+3);
	pDC->MoveTo(i_X2_data+c_offset+3,	i_Y2_data+c_offset-3);
	pDC->LineTo(i_X2_data+c_offset-3,	i_Y2_data+c_offset+3);
	pDC->SelectObject(old);
	my.DeleteObject();

	if(m_bCheckAngle==TRUE)
	{
	my.CreatePen(PS_SOLID,2,m_crCompass2);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt2_1.x,m_Pt2_1.y);    // Vector(과거 선을 지움)
	pDC->LineTo(m_Pt2_2.x,m_Pt2_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	if(m_bCheckGrid==TRUE)
	{
	my.CreatePen(PS_DOT,1,m_crCompass2);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt2_2.x,c_offset-300); // Cross(과거 선을 지움)
	pDC->LineTo(m_Pt2_2.x,m_Pt2_2.y-10);
	pDC->MoveTo(m_Pt2_2.x,m_Pt2_2.y+10);
	pDC->LineTo(m_Pt2_2.x,c_offset+300);	
	pDC->MoveTo(c_offset-300,m_Pt2_2.y);
	pDC->LineTo(m_Pt2_2.x-10,m_Pt2_2.y);
	pDC->MoveTo(m_Pt2_2.x+10,m_Pt2_2.y);
	pDC->LineTo(c_offset+300,m_Pt2_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	m_Pt2_1.x = c_offset;
	m_Pt2_1.y = c_offset;
	m_Pt2_2.x = i_X2_data+c_offset;
	m_Pt2_2.y = i_Y2_data+c_offset;

	if(m_bCheckAngle==TRUE)
	{
	my.CreatePen(PS_SOLID,2,m_crCompass2);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt2_1.x,m_Pt2_1.y);    // Vector
	pDC->LineTo(m_Pt2_2.x,m_Pt2_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}
	
	if(m_bCheckGrid==TRUE)
	{
	my.CreatePen(PS_DOT,1,m_crCompass2);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt2_2.x,c_offset-300); // Cross
	pDC->LineTo(m_Pt2_2.x,m_Pt2_2.y-10);
	pDC->MoveTo(m_Pt2_2.x,m_Pt2_2.y+10);
	pDC->LineTo(m_Pt2_2.x,c_offset+300);	
	pDC->MoveTo(c_offset-300,m_Pt2_2.y);
	pDC->LineTo(m_Pt2_2.x-10,m_Pt2_2.y);
	pDC->MoveTo(m_Pt2_2.x+10,m_Pt2_2.y);
	pDC->LineTo(c_offset+300,m_Pt2_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	ReleaseDC(pDC);
}

void CCompass1View::ID_3_Routine()
{
	if(m_bCheck3==FALSE) return;

	i_X3_data  = X3_data_H << 8;
	i_X3_data += X3_data_L;
	i_Y3_data  = Y3_data_H << 8;
	i_Y3_data += Y3_data_L;

	i_X3_data = 1023 - i_X3_data;
	m_nCompass3Xin = i_X3_data;
	m_nCompass3Yin = i_Y3_data;

	i_X3_data = i_X3_data - m_nCompass3Xref;
	i_Y3_data = i_Y3_data - m_nCompass3Yref;

	f_X3_data = (float)(i_X3_data);
	f_Y3_data = (float)(i_Y3_data);

	if(i_X3_data == 0 && i_Y3_data > 0) f_angle3 = 270;
	if(i_X3_data == 0 && i_Y3_data < 0) f_angle3 =  90;
	if(i_X3_data >  0 && i_Y3_data > 0) f_angle3 = (float)(360 - (atan(f_Y3_data/f_X3_data))*180/PI);
	if(i_X3_data >  0 && i_Y3_data < 0) f_angle3 = (float)(  0 - (atan(f_Y3_data/f_X3_data))*180/PI);
	if(i_X3_data <  0			      ) f_angle3 = (float)(180 - (atan(f_Y3_data/f_X3_data))*180/PI);

	if(i_X3_data < m_nCompass3Xmin) m_nCompass3Xmin = i_X3_data;
	if(i_X3_data > m_nCompass3Xmax) m_nCompass3Xmax = i_X3_data;
	if(i_Y3_data < m_nCompass3Ymin) m_nCompass3Ymin = i_Y3_data;
	if(i_Y3_data > m_nCompass3Ymax) m_nCompass3Ymax = i_Y3_data;

	m_fCompass3Xerr = (float)(m_nCompass3Xmax + m_nCompass3Xmin)/2;
	m_strCompass3Xerr.Format("%1.0f",m_fCompass3Xerr);
	m_fCompass3Yerr = (float)(m_nCompass3Ymax + m_nCompass3Ymin)/2;
	m_strCompass3Yerr.Format("%1.0f",m_fCompass3Yerr);


	m_nCompass3X = i_X3_data;
	m_nCompass3Y = i_Y3_data;
	
	m_fCompass3Adeg = f_angle3;
	m_fCompass3Arad = (float)(f_angle3 * PI / 180);
	m_fCompass3Adeg = (float)((int)(m_fCompass3Adeg * 10 + 0.5)) / 10;
	m_fCompass3Arad = (float)((int)(m_fCompass3Arad * 100 + 0.5)) / 100;

	m_strCompass3Arad.Format("%1.02f",m_fCompass3Arad);
	m_strCompass3Adeg.Format("%1.01f",m_fCompass3Adeg);

	CDC* pDC=GetDC();

	CPen my,*old;
	my.CreatePen(PS_SOLID,3,m_crCompass3);
	old=pDC->SelectObject(&my);

//	pDC->Rectangle(	i_X3_data+c_offset-2,	i_Y3_data+c_offset-2,
//					i_X3_data+c_offset+2,	i_Y3_data+c_offset+2);

	pDC->MoveTo(i_X3_data+c_offset-3,	i_Y3_data+c_offset-3);
	pDC->LineTo(i_X3_data+c_offset+3,	i_Y3_data+c_offset+3);
	pDC->MoveTo(i_X3_data+c_offset+3,	i_Y3_data+c_offset-3);
	pDC->LineTo(i_X3_data+c_offset-3,	i_Y3_data+c_offset+3);
	pDC->SelectObject(old);
	my.DeleteObject();

	if(m_bCheckAngle==TRUE)
	{
	my.CreatePen(PS_SOLID,2,m_crCompass3);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt3_1.x,m_Pt3_1.y);    // Vector(과거 선을 지움)
	pDC->LineTo(m_Pt3_2.x,m_Pt3_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	if(m_bCheckGrid==TRUE)
	{
	my.CreatePen(PS_DOT,1,m_crCompass3);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt3_2.x,c_offset-300); // Cross(과거 선을 지움)
	pDC->LineTo(m_Pt3_2.x,m_Pt3_2.y-10);
	pDC->MoveTo(m_Pt3_2.x,m_Pt3_2.y+10);
	pDC->LineTo(m_Pt3_2.x,c_offset+300);	
	pDC->MoveTo(c_offset-300,m_Pt3_2.y);
	pDC->LineTo(m_Pt3_2.x-10,m_Pt3_2.y);
	pDC->MoveTo(m_Pt3_2.x+10,m_Pt3_2.y);
	pDC->LineTo(c_offset+300,m_Pt3_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	m_Pt3_1.x = c_offset;
	m_Pt3_1.y = c_offset;
	m_Pt3_2.x = i_X3_data+c_offset;
	m_Pt3_2.y = i_Y3_data+c_offset;

	if(m_bCheckAngle==TRUE)
	{
	my.CreatePen(PS_SOLID,2,m_crCompass3);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt3_1.x,m_Pt3_1.y);    // Vector
	pDC->LineTo(m_Pt3_2.x,m_Pt3_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}
	
	if(m_bCheckGrid==TRUE)
	{
	my.CreatePen(PS_DOT,1,m_crCompass3);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt3_2.x,c_offset-300); // Cross
	pDC->LineTo(m_Pt3_2.x,m_Pt3_2.y-10);
	pDC->MoveTo(m_Pt3_2.x,m_Pt3_2.y+10);
	pDC->LineTo(m_Pt3_2.x,c_offset+300);	
	pDC->MoveTo(c_offset-300,m_Pt3_2.y);
	pDC->LineTo(m_Pt3_2.x-10,m_Pt3_2.y);
	pDC->MoveTo(m_Pt3_2.x+10,m_Pt3_2.y);
	pDC->LineTo(c_offset+300,m_Pt3_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	ReleaseDC(pDC);
}

void CCompass1View::ID_4_Routine()
{
	if(m_bCheck4==FALSE) return;

	i_X4_data  = X4_data_H << 8;
	i_X4_data += X4_data_L;
	i_Y4_data  = Y4_data_H << 8;
	i_Y4_data += Y4_data_L;

	i_X4_data = 1023 - i_X4_data;
	m_nCompass4Xin = i_X4_data;
	m_nCompass4Yin = i_Y4_data;

	i_X4_data = i_X4_data - m_nCompass4Xref;
	i_Y4_data = i_Y4_data - m_nCompass4Yref;

	i_X4_data *= -1;

	f_X4_data = (float)(i_X4_data);
	f_Y4_data = (float)(i_Y4_data);

	if(i_X4_data == 0 && i_Y4_data > 0) f_angle4 = 270;
	if(i_X4_data == 0 && i_Y4_data < 0) f_angle4 =  90;
	if(i_X4_data >  0 && i_Y4_data > 0) f_angle4 = (float)(360 - (atan(f_Y4_data/f_X4_data))*180/PI);
	if(i_X4_data >  0 && i_Y4_data < 0) f_angle4 = (float)(  0 - (atan(f_Y4_data/f_X4_data))*180/PI);
	if(i_X4_data <  0			      ) f_angle4 = (float)(180 - (atan(f_Y4_data/f_X4_data))*180/PI);

	if(i_X4_data < m_nCompass4Xmin) m_nCompass4Xmin = i_X4_data;
	if(i_X4_data > m_nCompass4Xmax) m_nCompass4Xmax = i_X4_data;
	if(i_Y4_data < m_nCompass4Ymin) m_nCompass4Ymin = i_Y4_data;
	if(i_Y4_data > m_nCompass4Ymax) m_nCompass4Ymax = i_Y4_data;

	m_fCompass4Xerr = (float)(m_nCompass4Xmax + m_nCompass4Xmin)/2;
	m_strCompass4Xerr.Format("%1.0f",m_fCompass4Xerr);
	m_fCompass4Yerr = (float)(m_nCompass4Ymax + m_nCompass4Ymin)/2;
	m_strCompass4Yerr.Format("%1.0f",m_fCompass4Yerr);


	m_nCompass4X = i_X4_data;
	m_nCompass4Y = i_Y4_data;
	
	m_fCompass4Adeg = f_angle4;
	m_fCompass4Arad = (float)(f_angle4 * PI / 180);
	m_fCompass4Adeg = (float)((int)(m_fCompass4Adeg * 10 + 0.5)) / 10;
	m_fCompass4Arad = (float)((int)(m_fCompass4Arad * 100 + 0.5)) / 100;

	m_strCompass4Arad.Format("%1.02f",m_fCompass4Arad);
	m_strCompass4Adeg.Format("%1.01f",m_fCompass4Adeg);

	CDC* pDC=GetDC();

	CPen my,*old;
	my.CreatePen(PS_SOLID,2,m_crCompass4);
	old=pDC->SelectObject(&my);

//	pDC->Rectangle(	i_X4_data+c_offset-2,	i_Y4_data+c_offset-2,
//					i_X4_data+c_offset+2,	i_Y4_data+c_offset+2);

	pDC->MoveTo(i_X4_data+c_offset-3,	i_Y4_data+c_offset-3);
	pDC->LineTo(i_X4_data+c_offset+3,	i_Y4_data+c_offset+3);
	pDC->MoveTo(i_X4_data+c_offset+3,	i_Y4_data+c_offset-3);
	pDC->LineTo(i_X4_data+c_offset-3,	i_Y4_data+c_offset+3);
	pDC->SelectObject(old);
	my.DeleteObject();

	if(m_bCheckAngle==TRUE)
	{
	my.CreatePen(PS_SOLID,2,m_crCompass4);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt4_1.x,m_Pt4_1.y);    // Vector(과거 선을 지움)
	pDC->LineTo(m_Pt4_2.x,m_Pt4_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	if(m_bCheckGrid==TRUE)
	{
	my.CreatePen(PS_DOT,1,m_crCompass4);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt4_2.x,c_offset-300); // Cross(과거 선을 지움)
	pDC->LineTo(m_Pt4_2.x,m_Pt4_2.y-10);
	pDC->MoveTo(m_Pt4_2.x,m_Pt4_2.y+10);
	pDC->LineTo(m_Pt4_2.x,c_offset+300);	
	pDC->MoveTo(c_offset-300,m_Pt4_2.y);
	pDC->LineTo(m_Pt4_2.x-10,m_Pt4_2.y);
	pDC->MoveTo(m_Pt4_2.x+10,m_Pt4_2.y);
	pDC->LineTo(c_offset+300,m_Pt4_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	m_Pt4_1.x = c_offset;
	m_Pt4_1.y = c_offset;
	m_Pt4_2.x = i_X4_data+c_offset;
	m_Pt4_2.y = i_Y4_data+c_offset;

	if(m_bCheckAngle==TRUE)
	{	
	my.CreatePen(PS_SOLID,2,m_crCompass4);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt4_1.x,m_Pt4_1.y);    // Vector
	pDC->LineTo(m_Pt4_2.x,m_Pt4_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}
	
	if(m_bCheckGrid==TRUE)
	{
	my.CreatePen(PS_DOT,1,m_crCompass4);
	old=pDC->SelectObject(&my);
	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(m_Pt4_2.x,c_offset-300); // Cross
	pDC->LineTo(m_Pt4_2.x,m_Pt4_2.y-10);
	pDC->MoveTo(m_Pt4_2.x,m_Pt4_2.y+10);
	pDC->LineTo(m_Pt4_2.x,c_offset+300);	
	pDC->MoveTo(c_offset-300,m_Pt4_2.y);
	pDC->LineTo(m_Pt4_2.x-10,m_Pt4_2.y);
	pDC->MoveTo(m_Pt4_2.x+10,m_Pt4_2.y);
	pDC->LineTo(c_offset+300,m_Pt4_2.y);
	pDC->SelectObject(old);
	my.DeleteObject();
	}

	ReleaseDC(pDC);
}

void CCompass1View::OnCheckCompass1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  // 받기
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_COMPASS1))->GetCheck(); // 눌러졌나 확인?
	if(bCheck)
	{	
//		CDC* pDC=GetDC();
//		CFont font;
//		font.CreateFontIndirect(&m_logFont);
//		pDC->SelectObject(&font);
//		pDC->SetBkColor(RGB(255,0,0));

		m_bCheck1=TRUE;

		GetDlgItem(IDC_EDIT_COMPASS1ADEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1ARAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1ERR	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1X	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1XERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1XIN	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1XMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1XMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1XREF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1Y	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1YERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1YIN	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1YMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1YMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1YREF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1Z	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1ZERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1ZMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS1ZMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ENCODER1    )->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ENCODER1ANG )->EnableWindow(TRUE);

//		ReleaseDC(pDC);

	}
	else
	{
		m_bCheck1=FALSE;

		GetDlgItem(IDC_EDIT_COMPASS1ADEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1ARAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1ERR	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1X	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1XERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1XIN	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1XMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1XMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1XREF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1Y	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1YERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1YIN	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1YMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1YMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1YREF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1Z	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1ZERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1ZMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS1ZMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENCODER1    )->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENCODER1ANG )->EnableWindow(FALSE);

	}
	
}

void CCompass1View::OnCheckCompass2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  // 받기
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_COMPASS2))->GetCheck(); // 눌러졌나 확인?
	if(bCheck)
	{
		m_bCheck2=TRUE;
		GetDlgItem(IDC_EDIT_COMPASS2ADEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2ARAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2ERR	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2X	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2XERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2XIN	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2XMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2XMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2XREF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2Y	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2YERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2YIN	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2YMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2YMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2YREF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2Z	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2ZERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2ZMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS2ZMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ENCODER2    )->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ENCODER2ANG )->EnableWindow(TRUE);
	}
	else
	{
		m_bCheck2=FALSE;

		GetDlgItem(IDC_EDIT_COMPASS2ADEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2ARAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2ERR	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2X	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2XERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2XIN	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2XMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2XMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2XREF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2Y	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2YERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2YIN	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2YMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2YMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2YREF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2Z	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2ZERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2ZMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS2ZMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENCODER2    )->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENCODER2ANG )->EnableWindow(FALSE);

	}	
}

void CCompass1View::OnCheckCompass3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  // 받기
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_COMPASS3))->GetCheck(); // 눌러졌나 확인?
	if(bCheck)
	{
		m_bCheck3=TRUE;
		GetDlgItem(IDC_EDIT_COMPASS3ADEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3ARAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3ERR	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3X	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3XERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3XIN	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3XMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3XMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3XREF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3Y	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3YERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3YIN	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3YMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3YMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3YREF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3Z	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3ZERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3ZMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS3ZMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ENCODER3    )->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ENCODER3ANG )->EnableWindow(TRUE);
	}
	else
	{
		m_bCheck3=FALSE;

		GetDlgItem(IDC_EDIT_COMPASS3ADEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3ARAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3ERR	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3X	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3XERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3XIN	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3XMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3XMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3XREF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3Y	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3YERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3YIN	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3YMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3YMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3YREF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3Z	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3ZERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3ZMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS3ZMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENCODER3    )->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENCODER3ANG )->EnableWindow(FALSE);

	}	
}

void CCompass1View::OnCheckCompass4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  // 받기
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_COMPASS4))->GetCheck(); // 눌러졌나 확인?
	if(bCheck)
	{
		m_bCheck4=TRUE;
		GetDlgItem(IDC_EDIT_COMPASS4ADEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4ARAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4ERR	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4X	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4XERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4XIN	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4XMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4XMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4XREF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4Y	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4YERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4YIN	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4YMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4YMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4YREF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4Z	)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4ZERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4ZMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMPASS4ZMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ENCODER4    )->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ENCODER4ANG )->EnableWindow(TRUE);
	}
	else
	{
		m_bCheck4=FALSE;
		GetDlgItem(IDC_EDIT_COMPASS4ADEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4ARAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4ERR	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4X	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4XERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4XIN	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4XMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4XMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4XREF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4Y	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4YERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4YIN	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4YMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4YMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4YREF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4Z	)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4ZERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4ZMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMPASS4ZMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENCODER4    )->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENCODER4ANG )->EnableWindow(FALSE);

	}	
}

void CCompass1View::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	OnCheckCompass1(); // Check 버튼 확인
	OnCheckCompass2();
	OnCheckCompass3();
	OnCheckCompass4();
	
	BackgroundDraw();
	CircleDraw();
	GridDraw();	
	// Do not call CFormView::OnPaint() for painting messages
}

void CCompass1View::OnCheckGrid() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  // 받기
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_GRID))->GetCheck(); // 눌러졌나 확인?
	if(bCheck)
	{
		m_bCheckGrid=TRUE;

	}
	else
	{
		m_bCheckGrid=FALSE;
	}
	m_Pt1_1.x = c_offset;
	m_Pt1_1.y = c_offset;
	m_Pt1_2.x = c_offset;
	m_Pt1_2.y = c_offset;
	m_Pt2_1.x = c_offset;
	m_Pt2_1.y = c_offset;
	m_Pt2_2.x = c_offset;
	m_Pt2_2.y = c_offset;
	m_Pt3_1.x = c_offset;
	m_Pt3_1.y = c_offset;
	m_Pt3_2.x = c_offset;
	m_Pt3_2.y = c_offset;
	m_Pt4_1.x = c_offset;
	m_Pt4_1.y = c_offset;
	m_Pt4_2.x = c_offset;
	m_Pt4_2.y = c_offset;	
}

void CCompass1View::OnCheckAngle() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  // 받기
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_ANGLE))->GetCheck(); // 눌러졌나 확인?
	if(bCheck)
	{
		m_bCheckAngle=TRUE;
	}
	else
	{
		m_bCheckAngle=FALSE;
	}
	m_Pt1_1.x = c_offset;
	m_Pt1_1.y = c_offset;
	m_Pt1_2.x = c_offset;
	m_Pt1_2.y = c_offset;
	m_Pt2_1.x = c_offset;
	m_Pt2_1.y = c_offset;
	m_Pt2_2.x = c_offset;
	m_Pt2_2.y = c_offset;
	m_Pt3_1.x = c_offset;
	m_Pt3_1.y = c_offset;
	m_Pt3_2.x = c_offset;
	m_Pt3_2.y = c_offset;
	m_Pt4_1.x = c_offset;
	m_Pt4_1.y = c_offset;
	m_Pt4_2.x = c_offset;
	m_Pt4_2.y = c_offset;
}

void CCompass1View::OnCal1() 
{
	// TODO: Add your control notification handler code here
	m_nCompass1Xref += (int)(m_fCompass1Xerr);
	m_nCompass1Yref += (int)m_fCompass1Yerr;
	m_nCompass3Xref += (int)m_fCompass3Xerr;
	m_nCompass3Yref += (int)m_fCompass3Yerr;
	UpdateData(0);
}

void CCompass1View::OnCal2() 
{
	// TODO: Add your control notification handler code here
	m_nCompass2Xref += (int)m_fCompass2Xerr;
	m_nCompass2Yref += (int)m_fCompass2Yerr;
	m_nCompass4Xref += (int)m_fCompass4Xerr;
	m_nCompass4Yref += (int)m_fCompass4Yerr;
	UpdateData(0);	
}
