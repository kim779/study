// AxBrower.cpp : implementation file
//

#include "stdafx.h"
#include "AxBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxBrowser

IMPLEMENT_DYNCREATE(CAxBrowser, CHtmlView)

CAxBrowser::CAxBrowser()
{
	//{{AFX_DATA_INIT(CAxBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAxBrowser::~CAxBrowser()
{
}

void CAxBrowser::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxBrowser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxBrowser, CHtmlView)
	//{{AFX_MSG_MAP(CAxBrowser)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxBrowser diagnostics

#ifdef _DEBUG
void CAxBrowser::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CAxBrowser::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAxBrowser message handlers

void CAxBrowser::OnDestroy() 
{
	CWnd::OnDestroy();
} 


void CAxBrowser::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	if (::IsWindow(m_wndBrowser.m_hWnd))
	{
		CRect rect;
		GetClientRect(rect);
		::AdjustWindowRectEx(rect,
				::GetWindowLong(m_wndBrowser.m_hWnd,GWL_STYLE), FALSE, WS_EX_CLIENTEDGE);

		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

int CAxBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= WS_CLIPCHILDREN;
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
