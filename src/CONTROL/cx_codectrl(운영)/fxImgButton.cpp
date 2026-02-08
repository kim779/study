// fxImgButton.cpp : implementation file
//

#include "stdafx.h"
#include "fxImgButton.h"

#include "../../h/memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_TIPS		100
#define	TM_DRAG		105
/////////////////////////////////////////////////////////////////////////////
// CfxImgButton

CfxImgButton::CfxImgButton(CFont* pFont/*=NULL*/)
{
	m_pFont = pFont;

	m_capture    = false;
	m_bTracking = FALSE;
	m_bHover    = FALSE;
	m_bCheck    = false;
	m_bChecked  = false;
	m_bStretch  = false;
	m_bFocus    = false;
	m_bDrawFocus = false;
	m_pTipCtrl  = nullptr;
	m_clText    = RGB(0,0,0);

	m_bText	     = FALSE;
	m_bLClick    = FALSE;

	m_hCursor     = NULL;
}

CfxImgButton::~CfxImgButton()
{
}

BEGIN_MESSAGE_MAP(CfxImgButton, CWnd)
	//{{AFX_MSG_MAP(CfxImgButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_WM_DRAWITEM()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxImgButton message handlers
BOOL CfxImgButton::Create(LPCTSTR caption, const RECT& rect, CWnd* parent, UINT nID, bool bCheck /*= FALSE*/, bool bStretch /*= FALSE*/)
{
	m_bCheck = bCheck;
	m_bStretch = bStretch;
	return CWnd::Create(NULL, caption, WS_CHILD | WS_VISIBLE, rect, parent, nID);
}

int CfxImgButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateTooltip();
	return 0;
}

void CfxImgButton::OnDestroy() 
{
	if (m_pTipCtrl)
		m_pTipCtrl.reset();

	CWnd::OnDestroy();
}

void CfxImgButton::SetImgBitmap(CBitmap* pBitmap, CBitmap* pBitmap_dn, CBitmap* pBitmap_hv)
{
	m_Bitmap = pBitmap;
	m_Bitmapdn = pBitmap_dn;
	m_Bitmaphv = pBitmap_hv;
}

void CfxImgButton::SetFont(CFont* pFont, BOOL redraw)
{
	m_pFont = pFont;
	if (redraw) Invalidate();
}

void CfxImgButton::SetCheck(bool bCheck)
{
	m_bChecked = bCheck; Invalidate();
}

void CfxImgButton::SetDrawFocus(bool bDraw)
{
	m_bDrawFocus = bDraw;	Invalidate();
}

void CfxImgButton::OnPaint() 
{
	CPaintDC paintDC(this); // device context for painting	
	CRect btnRc;
	GetClientRect(&btnRc);
	
	if ((m_capture && m_bHover) || (m_bCheck && m_bChecked))	// pushed
		DrawStretchByMask(&paintDC, m_Bitmapdn, btnRc, RGB(255, 0, 255));
	else if (m_bHover)		// hover
		DrawStretchByMask(&paintDC, m_Bitmaphv, btnRc, RGB(255, 0, 255));
	else
		DrawStretchByMask(&paintDC, m_Bitmap, btnRc, RGB(255, 0, 255));
}

void CfxImgButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	if (m_capture)
		ReleaseCapture();

	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CfxImgButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (true || !m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	if (m_bLClick)
	{
		if ((m_posLClick.x - point.x > 2 || m_posLClick.x - point.x < -2) ||
			(m_posLClick.y - point.y > 2 || m_posLClick.y - point.y < -2))
		{
			KillTimer(TM_DRAG);
			m_capture = false;
			ReleaseCapture();
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_PUSHED), (LPARAM)GetSafeHwnd());
			m_bLClick = FALSE;
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CfxImgButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bHover = TRUE; 
	Invalidate();
	return 0;
}


LRESULT CfxImgButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
//	m_bTracking = FALSE;
	m_bHover=FALSE;

	Invalidate(); return 0;
}

void CfxImgButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CfxImgButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_capture)
	{
		if (m_bLClick)
		{
			m_bLClick = FALSE;
			KillTimer(TM_DRAG);
		}
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		if (rc.PtInRect(point))
		{
			if (m_bCheck)
				m_bChecked = !m_bChecked;
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		}
		Invalidate();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CfxImgButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

CSize CfxImgButton::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);
	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CfxImgButton::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	Invalidate();	
}

bool CfxImgButton::IsChecked()
{
	return m_bChecked;
}

LRESULT CfxImgButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd    = m_hWnd;
	msg.message = message;
	msg.wParam  = wParam;
	msg.lParam  = lParam;

	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		if (m_pTipCtrl && !IsWindow(m_pTipCtrl->m_hWnd))
			m_pTipCtrl->Create(this, TTS_NOPREFIX|TTS_ALWAYSTIP);

		if (m_pTipCtrl && IsWindow(m_pTipCtrl->m_hWnd))
			m_pTipCtrl->RelayEvent(&msg);
	}
	else if (message == WM_KEYUP && (wParam == VK_RETURN || wParam == VK_SPACE) && m_bDrawFocus)
	{
		if (m_bCheck)
			m_bChecked = !m_bChecked;
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

CString CfxImgButton::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CfxImgButton::CreateTooltip()
{
	CString txt;
	GetWindowText(txt);

	if (txt.Find("\n") == -1)
		return;

	SetWindowText(Parser(txt, "\n"));
	
	CRect	rc;

	GetClientRect(&rc);
	m_pTipCtrl = std::make_unique<CToolTipCtrl>();
	m_pTipCtrl->Create(this);
	m_pTipCtrl->AddTool(this, txt, &rc, GetDlgCtrlID());
	m_pTipCtrl->Activate(TRUE);
}

void CfxImgButton::PreSubclassWindow() 
{
	if (!m_pTipCtrl)
		CreateTooltip();

	CWnd::PreSubclassWindow();
}

void CfxImgButton::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_bFocus = true;
	if (m_bDrawFocus)	Invalidate();
}

void CfxImgButton::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	m_bFocus = false;	
	if (m_bDrawFocus)	Invalidate();
}

void CfxImgButton::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TM_DRAG:
		KillTimer(nIDEvent);
		ReleaseCapture();
		m_capture = false;
		m_bLClick = FALSE;
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_PUSHED), (LPARAM)GetSafeHwnd());
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CfxImgButton::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);	
}

void CfxImgButton::ShowText(BOOL bShowText)
{
	m_bText = bShowText;
}

BOOL CfxImgButton::IsShowText()
{
	return m_bText;
}

void CfxImgButton::SetText(LPCTSTR caption, LPCTSTR tooltip)
{
	SetWindowText(caption);

	CString tip(tooltip);
	if (!tip.IsEmpty()) 
	{
		CRect	rc;

		GetClientRect(&rc);
		if (m_pTipCtrl && !IsWindow(m_pTipCtrl->m_hWnd))
			m_pTipCtrl->Create(this, TTS_NOPREFIX|TTS_ALWAYSTIP);

		m_pTipCtrl->AddTool(this, tooltip, &rc, GetDlgCtrlID());
		SetFocus();
	}
}


#define SRCMASK   0x00220326    // mask
void CfxImgButton::DrawStretchByMask(CDC *pDC, CBitmap *pBitmap, CRect bRc, COLORREF maskcolor)
{
	 if (!pBitmap || !pBitmap->GetSafeHandle())
		return;

	 BITMAP  bm;
	 pBitmap->GetBitmap(&bm);

	 const	int	bx = bm.bmWidth;
	 const	int	by = bm.bmHeight;
	 CDC  buffDC, maskDC, memoryDC, copyDC;
	 CBitmap  buffBitmap, maskBitmap, copyBitmap;

	 buffDC.CreateCompatibleDC(pDC);
	 buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	 CBitmap *oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	 maskDC.CreateCompatibleDC(pDC);
	 maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	 CBitmap *oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	 const	CRect  maskRc( 0, 0, bx, by);
	 buffDC.FillSolidRect(&maskRc, maskcolor);

	 memoryDC.CreateCompatibleDC(pDC);
	 CBitmap *oldmemoryBitmap = memoryDC.SelectObject(pBitmap);
	 buffDC.BitBlt( 0, 0, bx, by, &memoryDC, 0, 0, SRCCOPY);
	 maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);

	 copyDC.CreateCompatibleDC(pDC);
	 copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	 CBitmap *oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	 copyDC.BitBlt( 0, 0, bx, by, pDC, bRc.left, bRc.top, SRCCOPY);
	 copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	 buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	 copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	 pDC->StretchBlt(bRc.left, bRc.top, bRc.Width(), bRc.Height(), &copyDC, 0, 0, bx, by, SRCCOPY);

	 copyDC.SelectObject(oldcopyBitmap);
	 memoryDC.SelectObject(oldmemoryBitmap);
	 maskDC.SelectObject(oldmaskBitmap);
	 buffDC.SelectObject(oldbuffBitmap);

	 buffDC.DeleteDC();
	 maskDC.DeleteDC();
	 memoryDC.DeleteDC();
	 copyDC.DeleteDC();
} 

BOOL CfxImgButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt;
	CRect rc;

	GetCursorPos(&pt);
	GetWindowRect(&rc);

	if (m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}