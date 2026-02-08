// IndicatorExpHtmlView.cpp: implementation of the CIndicatorExpHtmlView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorExpHtmlView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyHtmlView

IMPLEMENT_DYNCREATE(CIndicatorExpHtmlView, CHtmlView)

CIndicatorExpHtmlView::CIndicatorExpHtmlView()
{
	//{{AFX_DATA_INIT(CIndicatorExpHtmlView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CIndicatorExpHtmlView::~CIndicatorExpHtmlView()
{
}

void CIndicatorExpHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndicatorExpHtmlView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndicatorExpHtmlView, CHtmlView)
	//{{AFX_MSG_MAP(CIndicatorExpHtmlView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndicatorExpHtmlView diagnostics

#ifdef _DEBUG
void CIndicatorExpHtmlView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CIndicatorExpHtmlView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIndicatorExpHtmlView message handlers
