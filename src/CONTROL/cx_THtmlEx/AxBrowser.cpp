// AxBrower.cpp : implementation file
//

#include "stdafx.h"
#include "AxBrowser.h"

#include "../../h/axisfire.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxBrowser
[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CAxBrowser, CHtmlView)

CAxBrowser::CAxBrowser()
{
	//{{AFX_DATA_INIT(CAxBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_scrGap = 0;
	m_parentView = NULL;
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
//		::AdjustWindowRectEx(rect,
//				::GetWindowLong(m_wndBrowser.m_hWnd,GWL_STYLE), FALSE, WS_EX_CLIENTEDGE);

		m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top,
			rect.Width() + m_scrGap, rect.Height() + m_scrGap, SWP_NOACTIVATE | SWP_NOZORDER); 
	}

}

int CAxBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= WS_CLIPCHILDREN;
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CAxBrowser::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	CString url(lpszURL);

	/** test
	if (url.Find("hts_index.do") < 0)
		url = "hts://IB200100";
	**/

	url.MakeUpper();
	const	int	find = url.Find("HTS://");
	if (m_parentView && find == 0) //** && url.GetLength() >= 14 /** 6 + 8 **/)
	{
		CString	map;

		map = url.Mid(6, url.GetLength() - 6);
		map.Remove('/');
		if (map.GetLength() == 4 && atoi(map) > 0)
		{
			map.Insert(0, "IB");
			map += "00";
		}

		m_parentView->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)map.GetString());
		*pbCancel = TRUE;
		return;
	}

	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CAxBrowser::SetParentView(CWnd* parentView)
{
	m_parentView = parentView;
}
