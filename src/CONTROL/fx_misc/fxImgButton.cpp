// fxImgButton.cpp : implementation file
//

#include "stdafx.h"
#include "fxImgButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_TIPS		100
/////////////////////////////////////////////////////////////////////////////
// CfxImgButton

CfxImgButton::CfxImgButton(CFont* pFont/*=nullptr*/)
{
	m_pFont = pFont;
	m_clText    = RGB(0,0,0);
	m_bDisableDraw = TRUE;
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
	ON_WM_DRAWITEM()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxImgButton message handlers
BOOL CfxImgButton::Create(LPCTSTR caption, const RECT& rect, CWnd* parent, UINT nID, bool bCheck /*= FALSE*/, bool bStretch /*= FALSE*/)
{
	const DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	m_bCheck = bCheck;
	m_bStretch = bStretch;
	return CWnd::Create(nullptr, caption, dwStyle, rect, parent, nID);
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
		m_pTipCtrl->DestroyWindow();
	CWnd::OnDestroy();
}

void CfxImgButton::SetTextColor(COLORREF clText)
{
	m_clText = clText;
	RedrawWindow();
}

void CfxImgButton::SetImgBitmap(HBITMAP hBitmap, HBITMAP hBitmap_dn, HBITMAP hBitmap_hv)
{
	m_hBitmap    = hBitmap;
	m_hBitmap_dn = hBitmap_dn;
	m_hBitmap_hv = hBitmap_hv;
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
	CPaintDC dc(this); // device context for painting
	
	CRect rc, rcFocus;
	CString txt;
	GetClientRect(&rc);
	rcFocus = rc;
	rcFocus.DeflateRect(2, 2);

	dc.SetBkMode(TRANSPARENT);

	if (m_hBitmap)
	{
		CDC		memDC;
		CBitmap* pOldBitmap{}, * pBitmap{};

		if (m_bCheck && m_bChecked && m_hBitmap_dn)
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		else if (m_capture && m_bHover && m_hBitmap_dn)
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		else if (m_bHover && m_hBitmap_hv)
			pBitmap = CBitmap::FromHandle(m_hBitmap_hv);
		else
			pBitmap = CBitmap::FromHandle(m_hBitmap);

		BITMAP stBitmapInfo ;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != nullptr) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		
		if (m_bStretch)
			dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, BitmapSize(m_hBitmap).cx, BitmapSize(m_hBitmap).cy, SRCCOPY);
		else
			dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	
	if (!m_hBitmap)
		dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover ? DFCS_PUSHED:NULL));
	
	CGdiObject* pOldFont{};
	if (m_pFont != nullptr) pOldFont = dc.SelectObject(m_pFont);
	else pOldFont = dc.SelectStockObject(SYSTEM_FONT);

	GetWindowText(txt); 
	
	if (m_hBitmap && !m_bStretch)
	{
		rc.right = BitmapSize(m_hBitmap).cx;
		rc.bottom = BitmapSize(m_hBitmap).cy;
	}

	if (m_capture && m_bHover)
		rc.OffsetRect(1, 1);
	
	if (IsWindowEnabled() || !m_bDisableDraw)
	{
		dc.SetTextColor(m_clText);
	}
	else 
	{
		dc.SetTextColor(RGB(255, 255, 255));
		rc.OffsetRect(1, 1);
		dc.DrawText(txt, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rc.OffsetRect(-1, -1);
		dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}

	dc.DrawText(txt, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (m_bDrawFocus && m_bFocus)
		dc.DrawFocusRect(rcFocus);

	dc.SelectObject(pOldFont);
	
	// Do not call CButton::OnPaint() for painting messages
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

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CfxImgButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover = TRUE; Invalidate();
	return 0;
}


LRESULT CfxImgButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
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
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;

	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		if (m_pTipCtrl && m_pTipCtrl->GetSafeHwnd())
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


void CfxImgButton::FitImageSize()
{
	if (m_hBitmap)
	{
		const CSize sz = BitmapSize(m_hBitmap);
		SetWindowPos(nullptr, 0, 0, sz.cx, sz.cy, SWP_NOZORDER|SWP_NOMOVE);
	}
}

void CfxImgButton::CreateTooltip()
{
	CString txt;
	GetWindowText(txt);

	if (txt.Find("\n") == -1)
		return;

	SetWindowText(Parser(txt, "\n"));
	
	CRect rc;
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

void CfxImgButton::SetDisableDraw(BOOL bDraw)
{
	m_bDisableDraw = bDraw;
}
