// AxMSpeedButton.cpp : implementation file
//

#include "stdafx.h"
#include "SpeedButton.h"
#include "CX_Category.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_AXUSER		WM_USER+10000
#define AXM_MESSAGE			(WM_USER + 30000)
#define AXM_SETMSGHANDLER	(AXM_MESSAGE + 1)
#define AXM_GETMSGHANDLER	(AXM_MESSAGE + 2)
#define WM_AXITEMCLICK  WM_AXUSER+11

const char* SpeedButtonClassName = "CAXSPEEDBUTTON";
/////////////////////////////////////////////////////////////////////////////
// CSpeedButton
BOOL XRegisterWindowClass(LPCTSTR lpszClassName)
{
	HBRUSH hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	
	if (!(::GetClassInfo(hInst, lpszClassName, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = hbrBackground;
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = lpszClassName;
		
		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	
	return TRUE;
}


CSpeedButton::CSpeedButton()
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

	XRegisterWindowClass(SpeedButtonClassName);
}

CSpeedButton::~CSpeedButton()
{
	ClearBitmaps();
}


BEGIN_MESSAGE_MAP(CSpeedButton, CWnd)
	//{{AFX_MSG_MAP(CSpeedButton)
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
// CSpeedButton message handlers

void CSpeedButton::ClearBitmaps()
{
	if (m_bmp)
		DeleteObject(m_bmp);
	if (m_bmpHover)
		DeleteObject(m_bmpHover);
	if (m_bmpDown)
		DeleteObject(m_bmpDown);

	m_bmp = m_bmpHover = m_bmpDown = FALSE;
}

void CSpeedButton::SetHoverBitmapPostfix(LPCTSTR postfix)
{
	m_hoverBitmapPostfix = postfix;
}

void CSpeedButton::SetDownBitmapPostfix(LPCTSTR postfix)
{
	m_downBitmapPostfix = postfix;
}

void CSpeedButton::LoadBitmap(LPCTSTR bmpFile)
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


void CSpeedButton::OnPaint() 
{
	CPaintDC dc(this); 
	
	if (m_bmp) 
		DrawButton(&dc);
	else 
		CWnd::OnPaint();
}

BOOL CSpeedButton::OnEraseBkgnd(CDC* pDC) 
{
	DrawButton(pDC);
	return TRUE;
}


BOOL CSpeedButton::Create(LPCTSTR szText, LPRECT lpRect, CWnd* parent, UINT id)
{
	BOOL bResult = CWnd::Create(SpeedButtonClassName, szText, WS_VISIBLE | WS_CHILD, *lpRect, parent, id);

	if (bResult && strlen(szText) && szText[0] == '@') 
		MakeTooltip();

	return bResult;
}

void CSpeedButton::MakeTooltip(CString sTip)
{
	if (!sTip.IsEmpty())
		SetWindowText(sTip);
}

CString CSpeedButton::GetEtcFileName(CString bmpFile, CString postfix)
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


void CSpeedButton::SetCheckStyle(BOOL bCheckStyle)
{
	m_check = bCheckStyle;
	m_down = FALSE;
	m_downState = FALSE;

	Invalidate();
}


void CSpeedButton::DrawButton(CDC* dc)
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


void CSpeedButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_downState = TRUE;
	Invalidate();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CSpeedButton::OnLButtonUp(UINT nFlags, CPoint point) 
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


void CSpeedButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd *wnd = GetActiveWindow(), *parent = GetOwner();
	CWnd::OnMouseMove(nFlags, point);

	if (GetCapture() != this && wnd && parent) 
	{
		m_hover = TRUE;

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
				m_hover = FALSE;
				Invalidate();
			}

			if (!(nFlags & MK_LBUTTON))
				ReleaseCapture();
		}
	}
}

BOOL CSpeedButton::GetDown()
{
	return m_down;
}

void CSpeedButton::SetDown(BOOL bDown)
{
	if (m_check && bDown != m_down) 
	{
		m_down = bDown;
		Invalidate();
	}
}


LRESULT CSpeedButton::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CSpeedButton::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_messageHandler;
}

LRESULT CSpeedButton::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_messageHandler && ::IsWindow(m_messageHandler->m_hWnd)) 
		return m_messageHandler->SendMessage(msg, wParam, lParam);
	return 0;
}

void CSpeedButton::StretchMode(BOOL bStretch)
{
	if (m_stretchDraw != bStretch) 
	{
		m_stretchDraw = bStretch;
		Invalidate();
	}
}

void CSpeedButton::SetTextDraw(BOOL bDrawText)
{
	if (m_textDraw != bDrawText) 
	{
		m_textDraw = bDrawText;
		Invalidate();
	}
}

void CSpeedButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	DrawButton(&dc);
	dc.Detach();
}

void CSpeedButton::OnDestroy() 
{
	CWnd::OnDestroy();

}

