 // ViewWrap.cpp : implementation file
//

#include "stdafx.h"
#include "ViewWrap.h"

//#include "TrConVolView.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString		Glb_SubTrCode;
CString		Glb_MapFileName;
HWND		Glb_hFormMngCtrl = NULL;
IDispatch*	Glb_ScrFormDisp = NULL;


STDAPI CreateScreen( CWnd** ppWrapWnd, CWnd** ppScrWnd, CWnd* pParent,
					 LPCTSTR strSubTRCode, IDispatch * FormDisp, LPCTSTR strMapFileName, HWND hFormMng, CMapStringToOb * ArrCtrlName, long lBkColor, int nAddView)
{
	Glb_SubTrCode = strSubTRCode;
	Glb_MapFileName = strMapFileName;
	Glb_hFormMngCtrl = hFormMng;
	Glb_ScrFormDisp = FormDisp;
	
	CViewWrap* pWrapWnd = new CViewWrap();
	
	CRect rect;
	pParent->GetWindowRect( &rect );
	pParent->ScreenToClient( &rect );
	
	if( !pWrapWnd->Create( NULL, NULL, WS_CHILD|WS_VISIBLE, rect, pParent, 999 ) )
		return E_FAIL;
	
	if(nAddView == 1)
		pWrapWnd->AddViewForSingle();
	
	*ppWrapWnd = pWrapWnd;
	*ppScrWnd = pWrapWnd->m_pView;
	
	((CTrConVolView*)pWrapWnd->m_pView)->m_hFormMng = hFormMng;
	((CTrConVolView*)pWrapWnd->m_pView)->m_lBkColor = lBkColor;
	((CTrConVolView*)pWrapWnd->m_pView)->m_MapControlName = ArrCtrlName;
	((CTrConVolView*)pWrapWnd->m_pView)->m_strScreenNo = strSubTRCode;
	((CTrConVolView*)pWrapWnd->m_pView)->m_ScrFormDisp = FormDisp;
	((CTrConVolView*)pWrapWnd->m_pView)->m_strMapFileName = strMapFileName;
	((CTrConVolView*)pWrapWnd->m_pView)->m_pFormWnd = pParent;
	
	return NOERROR;
}

STDAPI DestroyScreen( CWnd* pWrapWnd )
{
	CViewWrap* pDelWnd = (CViewWrap*)pWrapWnd;
	pDelWnd->DestroyWindow();
	delete pDelWnd;

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////////////
// CViewWrap

CViewWrap::CViewWrap()
{
	m_pView = NULL;
}

CViewWrap::~CViewWrap()
{
}


BEGIN_MESSAGE_MAP(CViewWrap, CWnd)
	//{{AFX_MSG_MAP(CViewWrap)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CViewWrap message handlers

int CViewWrap::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if( !CreateView( RUNTIME_CLASS( CTrConVolView ) ) )
		return -1;


	return 0;
}

void CViewWrap::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if(((CTrConVolView*)m_pView)->m_nScreenType != 1)
	{
		if( m_pView )
		{			
			m_pView->SetWindowPos( NULL, 0, 0, cx, cy, 0 );
		}
	}
}

typedef	CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent);

BOOL CViewWrap::CreateView( CRuntimeClass * pViewClass )
{
    //CRect rect, client;
    ASSERT(pViewClass );

    CWnd *pWnd=(CWnd *)pViewClass->CreateObject();
    if (!pWnd) return FALSE;	

	int dwStyle = AFX_WS_DEFAULT_VIEW;
    dwStyle&=~WS_BORDER;
	dwStyle&=~WS_VISIBLE;

    if( !pWnd->Create(NULL, NULL, dwStyle, CRect(0,0,0,0), this, 999, NULL ) )
    {
//        TRACE0("Warning: couldn't create client area for tab view\n");
        // pWnd will be cleaned up by PostNcDestroy
		return FALSE;
    }

	//pWnd->ModifyStyleEx( 0, WS_EX_CLIENTEDGE);

	CRect rect;
	GetWindowRect( &rect );
	ScreenToClient( &rect );	

	pWnd->MoveWindow( rect );	
	m_pView = (CView*)pWnd;
	pWnd->ShowWindow( SW_SHOW );
	
    return TRUE;
}

void CViewWrap::AddViewForSingle( )
{
	CDocument* pDoc = ((CView*)GetParent()->GetParent())->GetDocument();
	pDoc->AddView((CView*)m_pView);
}

BOOL CViewWrap::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}