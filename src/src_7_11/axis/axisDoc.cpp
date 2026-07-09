// axisDoc.cpp : implementation of the CAxisDoc class
//

#include "stdafx.h"
#include "axis.h"

#include "axisDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisDoc

IMPLEMENT_DYNCREATE(CAxisDoc, CDocument)

BEGIN_MESSAGE_MAP(CAxisDoc, CDocument)
	//{{AFX_MSG_MAP(CAxisDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisDoc construction/destruction

CAxisDoc::CAxisDoc()
{
}

CAxisDoc::~CAxisDoc()
{
}

BOOL CAxisDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAxisDoc serialization

void CAxisDoc::Serialize(CArchive& ar)
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
// CAxisDoc diagnostics

#ifdef _DEBUG
void CAxisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAxisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAxisDoc commands
