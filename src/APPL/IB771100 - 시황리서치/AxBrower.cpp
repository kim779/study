// AxBrower.cpp : implementation file
//

#include "stdafx.h"
#include "AxBrower.h"
#include "shareMsg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_CUR_PRICE		9847
#define IDC_SELL_STOCK		9848
#define IDC_BUY_STOCK		8949
#define IDC_CHART_STOCK		8950


/////////////////////////////////////////////////////////////////////////////
// CAxBrower

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CAxBrower, CHtmlView)

CAxBrower::CAxBrower()
{
	m_pParent = NULL;
	//{{AFX_DATA_INIT(CAxBrower)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	kill_count = 0;
}

CAxBrower::~CAxBrower()
{

}

void CAxBrower::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxBrower)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxBrower, CHtmlView)
	//{{AFX_MSG_MAP(CAxBrower)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_COMMAND(IDC_CUR_PRICE,	OnCurPrice)
	ON_COMMAND(IDC_SELL_STOCK,	OnSellStock)
	ON_COMMAND(IDC_BUY_STOCK,	OnBuyStock)
	ON_COMMAND(IDC_CHART_STOCK,	OnChartStock)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxBrower diagnostics

#ifdef _DEBUG
void CAxBrower::AssertValid() const
{
//	CHtmlView::AssertValid();
}

void CAxBrower::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAxBrower message handlers

void CAxBrower::OnDestroy() 
{
	CWnd::OnDestroy();
	delete this;		// updateX_20050504
} 

BOOL CAxBrower::OnEraseBkgnd(CDC *pDC)
{

	return false;

}

void CAxBrower::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	if (::IsWindow(m_wndBrowser.m_hWnd))
	{
		CRect rect;
		GetClientRect(rect);
		::AdjustWindowRectEx(rect,
				::GetWindowLong(m_wndBrowser.m_hWnd,GWL_STYLE), FALSE, WS_EX_CLIENTEDGE);

		m_wndBrowser.SetWindowPos(&wndTop, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOACTIVATE );//| SWP_NOZORDER);

		GetParent()->SendMessage(WM_SIZE_WEBBROW , cx, cy);
	}
}

void CAxBrower::OnSizing( UINT nSide, LPRECT lpRect )
{	
	GetParent()->SendMessage(WM_SIZING_WEBBROW , (WPARAM)lpRect, 0);	
}

void CAxBrower::OnCurPrice()
{
	CString strData;
	strData.Format("IB200100 /t/S/d1301\t%s\n", m_strSelectedCod);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

void CAxBrower::OnSellStock()
{
	CString strData;
	strData.Format("IB100100 /t/s/p5/d1301\t%s\n", m_strSelectedCod);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

void CAxBrower::OnBuyStock()
{
}

void CAxBrower::OnChartStock()
{
	CString strData;
	strData.Format("IB700000 /t/S/d1301\t%s\n", m_strSelectedCod);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

int CAxBrower::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= WS_CLIPCHILDREN;
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


void CAxBrower::OnDocumentComplete(LPCTSTR lpszURL) 
{
	//KillTimer(ID_FINDMSGBOX);
	//SetTimer(ID_FINDMSGBOX, 500, NULL);

	CHtmlView::OnDocumentComplete(lpszURL);
}

void CAxBrower::OnTimer(UINT nIDEvent) 
{
/*	CWnd* hwd = NULL;
	
	try
	{
		switch (nIDEvent)
		{
		case ID_FINDMSGBOX:
			
			
			if (kill_count > 300)
			{
				KillTimer(nIDEvent);
				kill_count = 0;
			}		
			
			hwd = FindWindow(NULL, "Internet Explorer 스크립트 오류");
			if (!hwd) hwd = FindWindow(NULL, "Error");
			if (!hwd) hwd = FindWindow(NULL, "ERROR");
			if (!hwd) hwd = FindWindow(NULL, "오류");
			
			if(hwd)
			{
				//		    hwd->ShowWindow(SW_HIDE);
				//hwd->PostMessage(WM_COMMAND, MAKELONG(IDNO, BN_CLICKED), 0); 
				//if (hwd) hwd->PostMessage(WM_CLOSE, 0,0);
			}	
			
			kill_count++; 
			
			break;
		}
	}
	catch (CException* e)
	{
		
	}*/

	CHtmlView::OnTimer(nIDEvent);
}

void CAxBrower::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString url = lpszURL;

	CString keyword = url.Left(4);
	if( keyword == "axis" )
	{
		const int len = url.GetLength();
		
		CString str = url.Right( len-7 );
		if( str.Find( "menu/" ) >= 0 )
		{
			CString cod = str.Right( str.GetLength()-5 );
			m_strSelectedCod = cod;

			*pbCancel = TRUE;

			CString strData;
			strData.Format("IB200100 /t/S/d1301\t%s\n", m_strSelectedCod);
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);

			/* CMenuXP menu;

			menu.SetMenuStyle( CMenuXP::STYLE_OFFICE );
			menu.CreatePopupMenu();

			menu.AppendMenuX( MF_STRING, IDC_CUR_PRICE, "2001 현재가" );			
			menu.AppendMenuX( MF_STRING, IDC_SELL_STOCK, "1001 주문" );						
			menu.AppendMenuX( MF_STRING, IDC_CHART_STOCK, "7000 챠트" );	

			CPoint pt;
			GetCursorPos( &pt );
			
			menu.TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);		*/

			return;
		}
	}	
	
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags,	lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}
