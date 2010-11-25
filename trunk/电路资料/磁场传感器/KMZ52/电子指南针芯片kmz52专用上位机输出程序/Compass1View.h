// Compass1View.h : interface of the CCompass1View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPASS1VIEW_H__7769A459_70E7_4D37_B8E2_B6619338AA56__INCLUDED_)
#define AFX_COMPASS1VIEW_H__7769A459_70E7_4D37_B8E2_B6619338AA56__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCompass1View : public CFormView
{
protected: // create from serialization only
	CCompass1View();
	DECLARE_DYNCREATE(CCompass1View)

public:
	//{{AFX_DATA(CCompass1View)
	enum { IDD = IDD_COMPASS1_FORM };
	CButton	m_ctrCheckCompass4;
	CButton	m_ctrCheckCompass3;
	CButton	m_ctrCheckCompass2;
	CButton	m_ctrCheckCompass1;
	CButton	m_ctrGrid;
	CButton	m_ctrAngle;
	int		m_nCompass1X;
	int		m_nCompass1Xin;
	int		m_nCompass1Xmax;
	int		m_nCompass1Xmin;
	int		m_nCompass1Xref;
	int		m_nCompass1Y;
	int		m_nCompass1Yin;
	int		m_nCompass1Ymax;
	int		m_nCompass1Ymin;
	int		m_nCompass1Yref;
	int		m_nCompass1Z;
	int		m_nCompass1Zmax;
	int		m_nCompass1Zmin;
	int		m_nCompass2X;
	int		m_nCompass2Xin;
	int		m_nCompass2Xmax;
	int		m_nCompass2Xmin;
	int		m_nCompass2Xref;
	int		m_nCompass2Y;
	int		m_nCompass2Yin;
	int		m_nCompass2Ymax;
	int		m_nCompass2Ymin;
	int		m_nCompass2Yref;
	int		m_nCompass2Z;
	int		m_nCompass2Zmax;
	int		m_nCompass2Zmin;
	int		m_nCompass3X;
	int		m_nCompass3Xin;
	int		m_nCompass3Xmax;
	int		m_nCompass3Xmin;
	int		m_nCompass3Xref;
	int		m_nCompass3Y;
	int		m_nCompass3Yin;
	int		m_nCompass3Ymax;
	int		m_nCompass3Ymin;
	int		m_nCompass3Yref;
	int		m_nCompass3Z;
	int		m_nCompass3Zmax;
	int		m_nCompass3Zmin;
	int		m_nCompass4X;
	int		m_nCompass4Xin;
	int		m_nCompass4Xmax;
	int		m_nCompass4Xmin;
	int		m_nCompass4Xref;
	int		m_nCompass4Y;
	int		m_nCompass4Yin;
	int		m_nCompass4Ymax;
	int		m_nCompass4Ymin;
	int		m_nCompass4Yref;
	int		m_nCompass4Z;
	int		m_nCompass4Zmax;
	int		m_nCompass4Zmin;
	CString	m_strCompass1Adeg;
	CString	m_strCompass1Arad;
	CString	m_strCompass1Err;
	CString	m_strCompass1Xerr;
	CString	m_strCompass1Yerr;
	CString	m_strCompass1Zerr;
	CString	m_strCompass2Adeg;
	CString	m_strCompass2Arad;
	CString	m_strCompass2Err;
	CString	m_strCompass2Xerr;
	CString	m_strCompass2Yerr;
	CString	m_strCompass3Adeg;
	CString	m_strCompass3Arad;
	CString	m_strCompass3Err;
	CString	m_strCompass3Xerr;
	CString	m_strCompass3Yerr;
	CString	m_strCompass3Zerr;
	CString	m_strCompass4Adeg;
	CString	m_strCompass4Arad;
	CString	m_strCompass4Err;
	CString	m_strCompass4Xerr;
	CString	m_strCompass4Yerr;
	CString	m_strCompass4Zerr;
	CString	m_strCompass2Zerr;
	CString	m_strEncoder1;
	CString	m_strEncoder2;
	CString	m_strEncoder3;
	CString	m_strEncoder4;
	CString	m_strEncoder1Ang;
	CString	m_strEncoder2Ang;
	CString	m_strEncoder3Ang;
	CString	m_strEncoder4Ang;
	//}}AFX_DATA

// Attributes
public:
	CCompass1Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompass1View)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	float f_Encoder1;
	float f_Encoder2;
	float f_Encoder3;
	float f_Encoder4;

	BOOL m_bCheckAngle;
	BOOL m_bCheckGrid;
	
	unsigned int Encoder1_H;
	unsigned int Encoder1_L;
	unsigned int Encoder2_H;
	unsigned int Encoder2_L;
	unsigned int Encoder3_H;
	unsigned int Encoder3_L;
	unsigned int Encoder4_H;
	unsigned int Encoder4_L;

	int			 i_Encoder1;
	int			 i_Encoder2;
	int			 i_Encoder3;
	int			 i_Encoder4;

	float	m_fEncoder1;
	float	m_fEncoder2;
	float	m_fEncoder3;
	float	m_fEncoder4;
	float	m_fCompass1Arad;
	float	m_fCompass1Adeg;
	float	m_fCompass1Err;
	float	m_fCompass1Xerr;
	float	m_fCompass1Yerr;
	float	m_fCompass1Zerr;
	float	m_fCompass2Adeg;
	float	m_fCompass2Arad;
	float	m_fCompass2Err;
	float	m_fCompass2Xerr;
	float	m_fCompass2Yerr;
	float	m_fCompass2Zerr;
	float	m_fCompass3Adeg;
	float	m_fCompass3Arad;
	float	m_fCompass3Err;
	float	m_fCompass3Xerr;
	float	m_fCompass3Yerr;
	float	m_fCompass3Zerr;
	float	m_fCompass4Adeg;
	float	m_fCompass4Arad;
	float	m_fCompass4Err;
	float	m_fCompass4Xerr;
	float	m_fCompass4Yerr;
	float	m_fCompass4Zerr;

	unsigned int X1_data_H;
	unsigned int X1_data_L;
	unsigned int Y1_data_H;
	unsigned int Y1_data_L;
	unsigned int X2_data_H;
	unsigned int X2_data_L;
	unsigned int Y2_data_H;
	unsigned int Y2_data_L;
	unsigned int X3_data_H;
	unsigned int X3_data_L;
	unsigned int Y3_data_H;
	unsigned int Y3_data_L;
	unsigned int X4_data_H;
	unsigned int X4_data_L;
	unsigned int Y4_data_H;
	unsigned int Y4_data_L;

	int			 i_X1_data;
	int			 i_Y1_data;
	int			 i_X2_data;
	int			 i_Y2_data;
	int			 i_X3_data;
	int			 i_Y3_data;
	int			 i_X4_data;
	int			 i_Y4_data;
	
	float		 f_X1_data;
	float		 f_Y1_data;
	float		 f_X2_data;
	float		 f_Y2_data;
	float		 f_X3_data;
	float		 f_Y3_data;
	float		 f_X4_data;
	float		 f_Y4_data;

	float		 f_angle1;
	float		 f_angle2;
	float		 f_angle3;
	float		 f_angle4;

	BOOL m_bCheck1;
	BOOL m_bCheck2;
	BOOL m_bCheck3;
	BOOL m_bCheck4;

	void ID_4_Routine();
	void ID_3_Routine();
	void ID_2_Routine();
	void ID_1_Routine();
	void GridDraw();
	void CircleDraw();
	void BackgroundDraw();

	COLORREF m_crCompass1;
	COLORREF m_crCompass2;
	COLORREF m_crCompass3;
	COLORREF m_crCompass4;

	CPoint m_Pt1_1;	
	CPoint m_Pt1_2;
	CPoint m_Pt2_1;
	CPoint m_Pt2_2;
	CPoint m_Pt3_1;
	CPoint m_Pt3_2;
	CPoint m_Pt4_1;
	CPoint m_Pt4_2;

	CPoint m_ecdPt1_1;
	CPoint m_ecdPt1_2;
	CPoint m_ecdPt2_1;
	CPoint m_ecdPt2_2;
	CPoint m_ecdPt3_1;
	CPoint m_ecdPt3_2;
	CPoint m_ecdPt4_1;
	CPoint m_ecdPt4_2;


	void strRcvData(int xPortNo, int nRcvSize);
	long OnReceiveData(WPARAM wParam, LPARAM lParam);

	CCommThread CommPort; // *******************************************

	virtual ~CCompass1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCompass1View)
	afx_msg void OnConnector();
	afx_msg void OnClose();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnClear();
	afx_msg void OnCheckCompass1();
	afx_msg void OnCheckCompass2();
	afx_msg void OnCheckCompass3();
	afx_msg void OnCheckCompass4();
	afx_msg void OnPaint();
	afx_msg void OnCheckGrid();
	afx_msg void OnCheckAngle();
	afx_msg void OnCal1();
	afx_msg void OnCal2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Compass1View.cpp
inline CCompass1Doc* CCompass1View::GetDocument()
   { return (CCompass1Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPASS1VIEW_H__7769A459_70E7_4D37_B8E2_B6619338AA56__INCLUDED_)
