// Compass1Doc.cpp : implementation of the CCompass1Doc class
//

#include "stdafx.h"
#include "Compass1.h"

#include "Compass1Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompass1Doc

IMPLEMENT_DYNCREATE(CCompass1Doc, CDocument)

BEGIN_MESSAGE_MAP(CCompass1Doc, CDocument)
	//{{AFX_MSG_MAP(CCompass1Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompass1Doc construction/destruction

CCompass1Doc::CCompass1Doc()
{
	// TODO: add one-time construction code here

}

CCompass1Doc::~CCompass1Doc()
{
}

BOOL CCompass1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCompass1Doc serialization

void CCompass1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCompass1Doc diagnostics

#ifdef _DEBUG
void CCompass1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCompass1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCompass1Doc commands
