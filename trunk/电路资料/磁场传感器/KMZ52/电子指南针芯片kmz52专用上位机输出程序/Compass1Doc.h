// Compass1Doc.h : interface of the CCompass1Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPASS1DOC_H__7F290CE8_9BA4_42FD_A6E5_946867B0700D__INCLUDED_)
#define AFX_COMPASS1DOC_H__7F290CE8_9BA4_42FD_A6E5_946867B0700D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCompass1Doc : public CDocument
{
protected: // create from serialization only
	CCompass1Doc();
	DECLARE_DYNCREATE(CCompass1Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompass1Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCompass1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCompass1Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPASS1DOC_H__7F290CE8_9BA4_42FD_A6E5_946867B0700D__INCLUDED_)
