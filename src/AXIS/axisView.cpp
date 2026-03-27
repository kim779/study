// axisView.cpp : implementation of the CAxisView class
//

#include "stdafx.h"
#include "axis.h"

#include "axisDoc.h"
#include "axisView.h"

#include "axmsg.hxx"
#include "axMisc.h"

#include <Winuser.h>
#include <Windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisView
#pragma warning (disable : 26409)
IMPLEMENT_DYNCREATE(CAxisView, CView)
#pragma warning (default : 26409)
BEGIN_MESSAGE_MAP(CAxisView, CView)
	//{{AFX_MSG_MAP(CAxisView)
	ON_WM_NCPAINT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(WM_CURSORMAP, OnSetCursorMap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisView construction/destruction

CAxisView::CAxisView()
{
	m_bTracking	= FALSE;
	m_bBkNotice	= FALSE;
	m_key		= 0;
	m_hCursor	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorHand = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));

	m_resourceHelper = ResourceHelper();
}

CAxisView::~CAxisView()
{
	RemoveAllMouseMap();
	if (m_hCursorHand)
		DestroyCursor(m_hCursorHand);
	CWnd* pMain = AfxGetMainWnd(); //test ¼öÁ¤ 20250613
	if(pMain)
		pMain->SendMessage(WM_AXIS, MAKEWPARAM(axCLOSEV, m_key), 0);
}

BOOL CAxisView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAxisView drawing

void CAxisView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CAxisView printing

BOOL CAxisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAxisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAxisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAxisView diagnostics

#ifdef _DEBUG
void CAxisView::AssertValid() const
{
	CView::AssertValid();
}

void CAxisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAxisDoc* CAxisView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAxisDoc)));
	return (CAxisDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAxisView message handlers

void CAxisView::SetInfo(int key)
{
	m_key = key;
}

void CAxisView::DrawEdge(CDC* pDC)
{
	CRect		winRc, rc;
	COLORREF	color{};

	GetWindowRect(winRc);
	winRc -= winRc.TopLeft();
	rc = winRc;

	color = m_resourceHelper->GetColor(ACTFLINE_4);
	pDC->Draw3dRect(rc, color, color);
	rc.DeflateRect(1, 1);

	color = m_resourceHelper->GetColor(ACTFLINE_5);
	pDC->Draw3dRect(rc, color, color);
}

void CAxisView::OnNcPaint() 
{
	CView::OnNcPaint();
	return;
	if (m_bBkNotice)
		return;
	CWindowDC dc(this);
	DrawEdge(&dc);
}

BOOL CAxisView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (nHitTest)
	{
	case HTCLIENT:
		if (!CView::OnSetCursor(pWnd, nHitTest, message))
		{
			SetCursor(m_hCursor);
			return TRUE;
		}
	case HTHSCROLL:
	case HTVSCROLL:
		break;
	}	
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CAxisView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

#ifdef DF_USE_CPLUS17
	for (const auto& [key, rc] : m_mapCursorMap)
	{
		if (rc)
		{
			if (rc->PtInRect(point))
			{
				m_hCursor = m_hCursorHand;
				break;
			}
		}
	}
	
#else
	POSITION pos = m_mapCursorMap.GetStartPosition();
	while (pos)
	{
		CString sKey;
		CRect* pRect;
		m_mapCursorMap.GetNextAssoc(pos, sKey, (void*&)pRect);
		if (pRect)
		{
			if (pRect->PtInRect(point))
			{
				m_hCursor = m_hCursorHand;
				break;
			}
		}
	}
#endif
	
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	
	CView::OnMouseMove(nFlags, point);
}

LRESULT CAxisView::OnSetCursorMap(WPARAM wParam, LPARAM lParam)
{
	struct _keyRect {
		RECT rect;
		char *pStr;
	};
	switch (LOWORD(wParam))
	{
	case SCM_SETCURSORMAP:
		{
			_keyRect *pData = (_keyRect *)lParam;
			CRect* pRect{};

#ifdef DF_USE_CPLUS17
			const auto ft = m_mapCursorMap.find(pData->pStr);
			const RECT rc = pData->rect;
			if (ft == m_mapCursorMap.end())
				m_mapCursorMap.emplace(std::make_pair<CString, std::shared_ptr<CRect>>(pData->pStr, std::make_shared<CRect>(rc)));
			else
				ft->second->SetRect(rc.left, rc.top, rc.right, rc.bottom);
#else
			if (!m_mapCursorMap.Lookup(pData->pStr, (void*&)pRect))
				pRect = new CRect(pData->rect);
			else
				*pRect = CRect(pData->rect);
			m_mapCursorMap.SetAt(pData->pStr, pRect);
#endif

		}
		break;
	case SCM_DELETECURSORMAP:
		{
			const LPSTR pStr = (LPSTR)lParam;
			CRect *pRect = NULL;

#ifdef DF_USE_CPLUS17
			const auto ft = m_mapCursorMap.find(pStr);
			if (ft != m_mapCursorMap.end())
				m_mapCursorMap.erase(ft);
#else
			if (m_mapCursorMap.Lookup(pStr, (void*&)pRect))
			{
				delete pRect;
				m_mapCursorMap.RemoveKey(pStr);
			}
#endif
			
		}
		break;
	case SCM_DELETEALLCURSORMAP:
		{
			RemoveAllMouseMap();
		}
		break;
	}
	return 0;
}

void CAxisView::RemoveAllMouseMap()
{
#ifdef DF_USE_CPLUS17
	m_mapCursorMap.clear();
#else
	POSITION pos = m_mapCursorMap.GetStartPosition();

	while (pos)
	{
		CString sKey;
		CRect* pRect;
		m_mapCursorMap.GetNextAssoc(pos, sKey, (void*&)pRect);
		if (pRect)
			delete pRect;
	}
	m_mapCursorMap.RemoveAll();
#endif
}

void CAxisView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{	
	CView::OnNcCalcSize(bCalcValidRects, lpncsp);
	/**if (m_bBkNotice)
	{
		lpncsp->rgrc[0].top  -= 3;
		lpncsp->rgrc[0].left -= 2;
		lpncsp->rgrc[0].right += 2;
		lpncsp->rgrc[0].bottom += 2;
	}
	**/
	const int VF = 2;
	lpncsp->rgrc[0].top  -= VF;
	lpncsp->rgrc[0].left -= VF;
	lpncsp->rgrc[0].right += VF;
	lpncsp->rgrc[0].bottom += VF;

}

LRESULT CAxisView::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_bTracking = FALSE;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return 0;
}

BOOL CAxisView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
//	return CView::OnEraseBkgnd(pDC);
}

LRESULT CAxisView::OnNcHitTest(CPoint point) 
{
	return CView::OnNcHitTest(point);
}

BOOL IsControlKey(UINT key)
{
	const UINT controlKeys[] = 
	{
		VK_RETURN, VK_BACK, VK_LEFT, VK_UP, VK_TAB, VK_RIGHT, VK_DOWN, NULL
	};

	for (int i = 0; controlKeys[i]; i++)
	{
		if (key == controlKeys[i])
			return TRUE;
	}
	return FALSE;
}

BOOL CAxisView::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN)
	{
		const CWnd *pWnd = GetWindow(GW_CHILD);
		if (pWnd && pWnd->m_hWnd)
		{
			const BOOL bRet = pWnd->SendMessage(WM_USER+1234, 0, LPARAM(pMsg)); // to WebBrowser
			if (bRet && IsControlKey(pMsg->wParam))
				return TRUE;
		}	
	}

	return CView::PreTranslateMessage(pMsg);
}
