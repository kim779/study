// ImgButton.cpp : implementation file
//

#include "stdafx.h"
#include "ImgButton.h"
#include "AxCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_TIPS		100
/////////////////////////////////////////////////////////////////////////////
// CImgButton

CImgButton::CImgButton(CFont* pFont/*=NULL*/)
{
	m_pFont = pFont;

	m_capture    = false;
	m_hBitmap    = NULL;
	m_hBitmap_dn = NULL;
	m_hBitmap_hv = NULL;

	m_bTracking = FALSE;
	m_bHover    = FALSE;
	m_bCheck    = false;
	m_bChecked  = false;
	
	m_bFocus    = false;
	m_bDrawFocus = false;
	m_pTipCtrl  = NULL;
	m_clText    = RGB(0,0,0);

	m_bStretch  = true;
}

CImgButton::~CImgButton()
{
}

BEGIN_MESSAGE_MAP(CImgButton, CWnd)
	//{{AFX_MSG_MAP(CImgButton)
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
	ON_WM_DRAWITEM()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(BM_SETCHECK, OnBmSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnBmGetCheck)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImgButton message handlers
BOOL CImgButton::Create(LPCTSTR caption, DWORD style, const RECT& rect, CWnd* parent, UINT nID, bool bCheck /*= FALSE*/, bool bStretch /*= FALSE*/)
{
	const DWORD dwStyle = WS_CHILD|WS_VISIBLE | style;
	m_bCheck = bCheck;
	//m_bStretch = bStretch;
	m_bStretch = true;
	const BOOL result = CWnd::Create(NULL, caption, dwStyle, rect, parent, nID);

	return result;
}

int CImgButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetCursor(LoadCursor(AfxGetApp()->m_hInstance, IDC_CROSS));//Axis::handCursor);
	CreateTooltip();

	return 0;
}

void CImgButton::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CImgButton::SetTextColor(COLORREF clText)
{
	m_clText = clText;
	RedrawWindow();
}

void CImgButton::SetImgBitmap(HBITMAP hBitmap, HBITMAP hBitmap_dn, HBITMAP hBitmap_hv)
{
	m_hBitmap    = hBitmap;
	m_hBitmap_dn = hBitmap_dn;
	m_hBitmap_hv = hBitmap_hv;
}

void CImgButton::SetFont(CFont* pFont, BOOL redraw)
{
	m_pFont = pFont;
	if (redraw) Invalidate();
}

void CImgButton::SetCheck(bool bCheck)
{
	m_bChecked = bCheck; Invalidate();
}

void CImgButton::SetDrawFocus(bool bDraw)
{
	m_bDrawFocus = bDraw;	Invalidate();
}

void CImgButton::OnPaint() 
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
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		
		/**
		if (m_bStretch)
			dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, BitmapSize(m_hBitmap).cx, BitmapSize(m_hBitmap).cy, SRCCOPY);
		else
			dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		**/

		dc.StretchBlt(3, 0, rc.Width() - 6, rc.Height(), &memDC, 3, 0, 3, stBitmapInfo.bmHeight, SRCCOPY);
		dc.StretchBlt(0, 0, 3, rc.Height(), &memDC, 0, 0, 3, stBitmapInfo.bmHeight, SRCCOPY);
		dc.StretchBlt(rc.Width() - 3, 0, 3, rc.Height(), &memDC, stBitmapInfo.bmWidth - 3, 0, 3, stBitmapInfo.bmHeight, SRCCOPY);
		
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	
	if (!m_hBitmap)
		dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover ? DFCS_PUSHED:NULL));
	
	CGdiObject* pOldFont{};
	if (m_pFont != NULL) pOldFont = dc.SelectObject(m_pFont);
	else pOldFont = dc.SelectStockObject(SYSTEM_FONT);

	GetWindowText(txt); 
	
	if (m_hBitmap && !m_bStretch)
	{
		rc.right = BitmapSize(m_hBitmap).cx;
		rc.bottom = BitmapSize(m_hBitmap).cy;
	}

	if (m_capture && m_bHover)
		rc.OffsetRect(1, 1);
	
	if (IsWindowEnabled())
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

void CImgButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CImgButton::OnMouseMove(UINT nFlags, CPoint point) 
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

LRESULT CImgButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover = TRUE; Invalidate();
	return 0;
}


LRESULT CImgButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;

	Invalidate(); return 0;
}

void CImgButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CImgButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_capture)
	{
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		if (rc.PtInRect(point))
		{
			const DWORD style = GetStyle();
			const BOOL autoRadio = (style & BS_AUTORADIOBUTTON) && (style & BS_PUSHLIKE);
			if (autoRadio)
			{
				if (!m_bCheck)
					m_bCheck = TRUE;
				
				m_bChecked = TRUE;

				CWnd* child = GetParent()->GetWindow(GW_CHILD);
				char className[MAX_PATH];
				::GetClassName(m_hWnd, className, MAX_PATH);
				while (child)
				{
					char buffer[MAX_PATH];
					GetClassName(child->m_hWnd, buffer, MAX_PATH);

					if (child->m_hWnd != m_hWnd && strcmp(className, buffer) == 0)
					{
						child->SendMessage(BM_SETCHECK, 0);
					}
					
					child = child->GetWindow(GW_HWNDNEXT);
				}
			}
			else
			{
				if (m_bCheck)
					m_bCheck = FALSE;
			}

			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		}
		Invalidate();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CImgButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

CSize CImgButton::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);
	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CImgButton::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	Invalidate();	
}

bool CImgButton::IsChecked()
{
	return m_bChecked;
}

LRESULT CImgButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;

	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		if (m_pTipCtrl)
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

CString CImgButton::Parser(CString &srcstr, CString substr)
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


void CImgButton::FitImageSize()
{
	if (m_hBitmap)
	{
		const CSize sz = BitmapSize(m_hBitmap);
		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOZORDER|SWP_NOMOVE);
	}
}

void CImgButton::CreateTooltip()
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

void CImgButton::PreSubclassWindow() 
{
	if (!m_pTipCtrl)
		CreateTooltip();	
	CWnd::PreSubclassWindow();
}

void CImgButton::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_bFocus = true;
	if (m_bDrawFocus)	Invalidate();
}

void CImgButton::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	m_bFocus = false;	
	if (m_bDrawFocus)	Invalidate();
}

LRESULT CImgButton::OnBmGetCheck(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_bChecked;
}

LRESULT CImgButton::OnBmSetCheck(WPARAM wParam, LPARAM lParam)
{
	m_bChecked = wParam != 0;
	if (wParam && !m_bCheck)
		m_bCheck = TRUE;
	Invalidate();
	return 0;
}


BOOL CImgButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetCursor(Axis::handCursor);	
	return TRUE;
}
