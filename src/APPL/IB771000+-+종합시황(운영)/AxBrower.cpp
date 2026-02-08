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
#define IDC_PRC_BTN			8951
#define IDC_CT_BTN			8952
#define IDC_CP_BTN			8953
#define IDC_MMDH_BTN		8954
#define IDC_FAVG_BTN		8955


/////////////////////////////////////////////////////////////////////////////

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

	ON_COMMAND(IDC_PRC_BTN, OnPrcStock)
	ON_COMMAND(IDC_CT_BTN, OnCtStock)
	ON_COMMAND(IDC_CP_BTN, OnCpStock)
	ON_COMMAND(IDC_MMDH_BTN, OnMmdhStock)
	ON_COMMAND(IDC_FAVG_BTN, OnFavgStock)

	ON_COMMAND(IDC_BUY_STOCK,	OnBuyStock)
	ON_COMMAND(IDC_CHART_STOCK,	OnChartStock)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxBrower diagnostics

#ifdef _DEBUG
void CAxBrower::AssertValid() const
{
	//CHtmlView::AssertValid();
}

void CAxBrower::Dump(CDumpContext& dc) const
{
	//CHtmlView::Dump(dc);
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

void CAxBrower::OnPrcStock()
{
	CString strData;
	strData.Format("IB200100 /t/s/p5/d1301\t%s\n", m_strSelectedCod);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

void CAxBrower::OnCtStock()
{
	CString strData;
	strData.Format("IB711200 /t/s/p5/d1301\t%s\n", m_strSelectedCod);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

void CAxBrower::OnCpStock()
{
	CString strData;
	strData.Format("IB202300 /t/s/p5/d1301\t%s\n", m_strSelectedCod);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

void CAxBrower::OnMmdhStock()
{
	CString strData;
	strData.Format("IB202400 /t/s/p5/d1301\t%s\n", m_strSelectedCod);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

void CAxBrower::OnFavgStock()
{
	CString strData;
	strData.Format("IB240100 /t/s/p5/d1301\t%s\n", m_strSelectedCod);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

void CAxBrower::OnBuyStock()
{
}

void CAxBrower::OnChartStock()
{
	CString strData;
	strData.Format("IB711200 /t/S/d1301\t%s\n", m_strSelectedCod);
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

			return;
		}
	}	
	
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags,	lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CAxBrower::NavigateFromMemory( LPCSTR dat, int len )
{
	if (dat==NULL) return;
	
	HRESULT hr = S_OK;
	HGLOBAL hGlobal = NULL;
	IStream *pStream = NULL;
	ULONG lWritten{};
	
	do {
		HGLOBAL hGlobal = GlobalAlloc(GHND, len);
		if (hGlobal==NULL) break;
		
		hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
		if (FAILED(hr)) break;
		
		hr = pStream->Write(dat, len, &lWritten);
		if (SUCCEEDED(hr))
		{
			LARGE_INTEGER pos;
			pos.QuadPart = 0;
			pStream->Seek((LARGE_INTEGER)pos, STREAM_SEEK_SET, NULL);
		}
		else break;
		
		NavigateFromStream(pStream);
		
	} while(FALSE);
	
	if (pStream) pStream->Release();
	if (hGlobal) GlobalFree(hGlobal);
}

void CAxBrower::NavigateFromStream( IStream *pStream )
{
	if (pStream==NULL) return;
	
	HRESULT hr{};
	IDispatch* pHtmlDoc = NULL;
	IPersistStreamInit* pPersistStreamInit = NULL;
	
	do {
		
		hr = m_pBrowserApp->get_Document(&pHtmlDoc);
		if (FAILED(hr) || pHtmlDoc==NULL) break;
		
		hr = pHtmlDoc->QueryInterface(IID_IPersistStreamInit, (void**)&pPersistStreamInit);
		if (FAILED(hr) || pPersistStreamInit==NULL) break;
		
		hr = pPersistStreamInit->InitNew();
		if (FAILED(hr)) break;
		
		hr = pPersistStreamInit->Load( pStream );
		
	} while(FALSE);
	
	if (pPersistStreamInit) pPersistStreamInit->Release();
	if (pHtmlDoc) pHtmlDoc->Release();
}
