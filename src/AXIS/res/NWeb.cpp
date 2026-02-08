// NWeb.cpp : implementation file
//

#include "stdafx.h"
//#include "axis.h"
#include "NWeb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNWeb

//IMPLEMENT_DYNCREATE(CNWeb, CHtmlView)

CNWeb::CNWeb()
{
	//{{AFX_DATA_INIT(CNWeb)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CNWeb::~CNWeb()
{
}

void CNWeb::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNWeb)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNWeb, CHtmlView)
	//{{AFX_MSG_MAP(CNWeb)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNWeb diagnostics

#ifdef _DEBUG
void CNWeb::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CNWeb::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNWeb message handlers
