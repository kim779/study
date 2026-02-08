// AxMSpeedButton.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#include "AxMToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* SpeedButtonClassName = "CAXSPEEDBUTTON";
/////////////////////////////////////////////////////////////////////////////
// CAxMSpeedButton

CAxMSpeedButton::CAxMSpeedButton()
{
	m_hoverBitmapPostfix = "_EN";
	m_downBitmapPostfix = "_DN";

	m_check = FALSE;
	m_down = m_downState = FALSE;
	m_hover = FALSE;

	m_stretchDraw = FALSE;
	m_textDraw = FALSE;

	m_bmp = m_bmpHover = m_bmpDown = NULL;
	m_font = NULL;

	m_messageHandler = NULL;
	m_toolTip = NULL;

	XRegisterWindowClass(SpeedButtonClassName);
}

CAxMSpeedButton::~CAxMSpeedButton()
{
	ClearBitmaps();
}


BEGIN_MESSAGE_MAP(CAxMSpeedButton, CWnd)
	//{{AFX_MSG_MAP(CAxMSpeedButton)
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMSpeedButton message handlers

void CAxMSpeedButton::ClearBitmaps()
{
	if (m_bmp)
		DeleteObject(m_bmp);
	if (m_bmpHover)
		DeleteObject(m_bmpHover);
	if (m_bmpDown)
		DeleteObject(m_bmpDown);

	m_bmp = m_bmpHover = m_bmpDown = FALSE;
}

void CAxMSpeedButton::SetHoverBitmapPostfix(LPCTSTR postfix)
{
	m_hoverBitmapPostfix = postfix;
}

void CAxMSpeedButton::SetDownBitmapPostfix(LPCTSTR postfix)
{
	m_downBitmapPostfix = postfix;
}

void CAxMSpeedButton::LoadBitmap(LPCTSTR bmpFile)
{
	CString file(bmpFile);

	CFileFind ff;
	if (!ff.FindFile(file)) return;

	ClearBitmaps();

	m_bmp = (HBITMAP)LoadImage(NULL, bmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bmpHover = (HBITMAP)LoadImage(NULL, GetEtcFileName(bmpFile, m_hoverBitmapPostfix), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bmpDown = (HBITMAP)LoadImage(NULL, GetEtcFileName(bmpFile, m_downBitmapPostfix), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	m_bmWidth = 172;
	m_bmHeight = 17;
	Invalidate();
}


void CAxMSpeedButton::OnPaint() 
{
	CPaintDC dc(this); 
	
	if (m_bmp) 
		DrawButton(&dc);
	else 
		CWnd::OnPaint();
}

BOOL CAxMSpeedButton::OnEraseBkgnd(CDC* pDC) 
{
	DrawButton(pDC);
	return TRUE;
}


BOOL CAxMSpeedButton::Create(LPCTSTR szText, LPRECT lpRect, CWnd* parent, UINT id)
{
	BOOL bResult = CWnd::Create(SpeedButtonClassName, szText, WS_VISIBLE | WS_CHILD, *lpRect, parent, id);

	if (bResult && strlen(szText) && szText[0] == '@') 
		MakeTooltip();

	return bResult;
}

void CAxMSpeedButton::MakeTooltip(CString sTip)
{
	if (!sTip.IsEmpty())
		SetWindowText(sTip);

	if (!m_toolTip)
	{
		m_toolTip = new CAxMToolTip;
		m_toolTip->Create();
	}
}

CString CAxMSpeedButton::GetEtcFileName(CString bmpFile, CString postfix)
{
	CString name, ext;
	int find = bmpFile.ReverseFind('.');

	if (find <= 1) return CString("");

	name = bmpFile.Left(find);
	ext = bmpFile.Right(bmpFile.GetLength() - (find + 1));

	CString s;
	s = name + postfix + '.' + ext;
	return s;
}


void CAxMSpeedButton::SetCheckStyle(BOOL bCheckStyle)
{
	m_check = bCheckStyle;
	m_down = FALSE;
	m_downState = FALSE;

	Invalidate();
}


void CAxMSpeedButton::DrawButton(CDC* dc)
{
	HBITMAP bmp, oldBmp;
	
	if (m_bmp) {
		if (m_down || m_downState) 
			bmp = m_bmpDown ? m_bmpDown: m_bmp;
		else if (m_hover)
			bmp = m_bmpHover ? m_bmpHover: m_bmp;
		else
			bmp = m_bmp;
		
		CRect rc;
		GetClientRect(&rc);
		CDC mdc;
		mdc.CreateCompatibleDC(dc);
		oldBmp = (HBITMAP)mdc.SelectObject(bmp);



		CFont* oldFont = NULL;
		
		if (m_textDraw) {
			if (m_font)
				oldFont = mdc.SelectObject(m_font);
			mdc.SetBkMode(TRANSPARENT);

			CString str;
			GetWindowText(str);
			mdc.DrawText(str, rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
		}
		
		if (m_stretchDraw) 
		{
			dc->StretchBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, m_bmWidth, m_bmHeight, SRCCOPY);
		}
		else 
		{
			dc->BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
		}
		
		if (oldFont)
			mdc.SelectObject(oldFont);
		mdc.SelectObject(oldBmp);
	}
}


void CAxMSpeedButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_downState = TRUE;
	Invalidate();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CAxMSpeedButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_downState) 
	{
		POINT p2 = point;
		ClientToScreen(&p2);
		CWnd* wndUnderMouse = WindowFromPoint(p2);

		if (wndUnderMouse && wndUnderMouse->m_hWnd != this->m_hWnd) 
		{
		}
		else 
		{
			if (m_check)
				m_down = !m_down;

			Notify(WM_AXITEMCLICK, m_down, (LPARAM)this);
			
			GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetWindowLong(m_hWnd, GWL_ID), BN_CLICKED));
		}
	}

	m_downState = FALSE;
	Invalidate();
	
	CWnd::OnLButtonUp(nFlags, point);
}


void CAxMSpeedButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd *wnd = GetActiveWindow(), *parent = GetOwner();
	CWnd::OnMouseMove(nFlags, point);

	if (GetCapture() != this && wnd && parent) 
	{
		m_hover = TRUE;
		if (m_toolTip) 
			ShowToolTip();

		SetCapture();
		Invalidate();
	}
	else 
	{
		POINT p2 = point;
		ClientToScreen(&p2);
		CWnd* wndUnderMouse = WindowFromPoint(p2);

		if (wndUnderMouse && wndUnderMouse->m_hWnd != this->m_hWnd) 
		{
			if (m_hover == TRUE) 
			{
				if (m_toolTip)
					HideToolTip();

				m_hover = FALSE;
				Invalidate();
			}

			if (!(nFlags & MK_LBUTTON))
				ReleaseCapture();
		}
	}
}

BOOL CAxMSpeedButton::GetDown()
{
	return m_down;
}

void CAxMSpeedButton::SetDown(BOOL bDown)
{
	if (m_check && bDown != m_down) 
	{
		m_down = bDown;
		Invalidate();
	}
}


LRESULT CAxMSpeedButton::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMSpeedButton::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_messageHandler;
}

LRESULT CAxMSpeedButton::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

void CAxMSpeedButton::StretchMode(BOOL bStretch)
{
	if (m_stretchDraw != bStretch) 
	{
		m_stretchDraw = bStretch;
		Invalidate();
	}
}

void CAxMSpeedButton::SetTextDraw(BOOL bDrawText)
{
	if (m_textDraw != bDrawText) 
	{
		m_textDraw = bDrawText;
		Invalidate();
	}
}

void CAxMSpeedButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	DrawButton(&dc);
	dc.Detach();
}

void CAxMSpeedButton::ShowToolTip()
{
	if (!m_toolTip) return;

	CString str;
	GetWindowText(str);
	CSize sz = m_toolTip->SetText(str);

	CPoint pt;
	::GetCursorPos(&pt);

	m_toolTip->WindowMove(pt.x, pt.y + 20, sz.cx, sz.cy);
	m_toolTip->ShowWindow(SW_SHOWNOACTIVATE);
//	Debug("rect = %d, %d, %d, %d", pt.x, pt.y + 20, sz.cx, sz.cy);
	m_toolTip->SetTimer(1, 200, NULL);
}

void CAxMSpeedButton::HideToolTip()
{
	if (!m_toolTip) return;

	m_toolTip->KillTimer(1);
	m_toolTip->ShowWindow(SW_HIDE);
}

void CAxMSpeedButton::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_toolTip) 
	{
		if (::IsWindow(m_toolTip->m_hWnd))
			m_toolTip->DestroyWindow();
		delete m_toolTip;
	}
}

