// TitleTip.cpp : implementation file
//

#include "stdafx.h"
#include "TitleTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleTip

CTitleTip::CTitleTip()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, TITLETIP_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style		= CS_SAVEBITS|CS_DBLCLKS;
		wndcls.lpfnWndProc	= ::DefWindowProc;
		wndcls.cbClsExtra	= wndcls.cbWndExtra = 0;
		wndcls.hInstance	= hInst;
		wndcls.hIcon		= NULL;
		wndcls.hCursor		= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName	= NULL;
		wndcls.lpszClassName	= TITLETIP_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}
}

CTitleTip::~CTitleTip()
{
}


BEGIN_MESSAGE_MAP(CTitleTip, CWnd)
	//{{AFX_MSG_MAP(CTitleTip)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTitleTip message handlers
BOOL CTitleTip::Create(CWnd* parentWnd)
{
	ASSERT_VALID(parentWnd);

	DWORD dwStyle = WS_BORDER|WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;
	m_parentWnd = parentWnd;

	return CreateEx(dwExStyle, TITLETIP_CLASSNAME, NULL, dwStyle, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL);
}

void CTitleTip::Show(CRect rectTitle, LPCTSTR lpszTitleText, int xoffset, LPRECT lpHoverRect, LPLOGFONT lpLogFont)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	if (rectTitle.IsRectEmpty()) return;

	m_rectHover = (lpHoverRect != NULL) ? lpHoverRect : rectTitle;

	m_parentWnd->ClientToScreen(m_rectHover);
	ScreenToClient(m_rectHover);

	if (IsWindowVisible()) 	return;
	if (!GetFocus())	return;

	// Define the rectangle outside which the titletip will be hidden.
	// We add a buffer of one pixel around the rectangle
	m_rectTitle.top    = -1;
	m_rectTitle.left   = -xoffset-1;
	m_rectTitle.right  = rectTitle.Width() - xoffset;
	m_rectTitle.bottom = rectTitle.Height() +1;

	// Determine the width of the text
	m_parentWnd->ClientToScreen(rectTitle);

	CClientDC dc(this);
	CString strTitle = _T("");
	strTitle += _T(" "); strTitle += lpszTitleText; strTitle += _T(" ");

	CFont font, *pOldFont = NULL;
	if (lpLogFont)
	{
		font.CreatePointFontIndirect(lpLogFont);
		pOldFont = dc.SelectObject(&font);
	}
	else
	{
		// use same font as ctrl
		pOldFont = dc.SelectObject(m_parentWnd->GetFont());
	}

	CSize size = dc.GetTextExtent(strTitle);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	size.cx += tm.tmOverhang;

	CRect rectDisplay = rectTitle;
	rectDisplay.left += xoffset;
	rectDisplay.right = rectDisplay.left + size.cx + xoffset;
    
	//if (rectDisplay.right > rectTitle.right-xoffset)
	{
		// Show the titletip
		POINT mousePos;
		GetCursorPos(&mousePos);
		mousePos.y += 10;
		SetWindowPos(&wndTop, mousePos.x, mousePos.y, rectDisplay.Width(),
					rectDisplay.Height(), SWP_SHOWWINDOW|SWP_NOACTIVATE);

		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(strTitle, CRect(0, 0, rectDisplay.Width(), rectDisplay.Height()),
							DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
//		dc.TextOut(0, 0, strTitle);
		SetCapture();
	}
    
	dc.SelectObject(pOldFont);
}

void CTitleTip::Hide()
{
  	if (!::IsWindow(GetSafeHwnd()))
	        return;

	if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
		ReleaseCapture();

	ShowWindow(SW_HIDE);
}

void CTitleTip::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_rectHover.PtInRect(point)) 
	{
		Hide();

		// Forward the message
		ClientToScreen(&point);
		CWnd *pWnd = WindowFromPoint(point);
		if (pWnd == this) pWnd = m_parentWnd;

		int hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
		if (hittest == HTCLIENT)
		{
			pWnd->ScreenToClient(&point);
			pWnd->PostMessage(WM_MOUSEMOVE, nFlags, MAKELONG(point.x,point.y));
		}
		else
		{
			pWnd->PostMessage(WM_NCMOUSEMOVE, hittest, MAKELONG(point.x,point.y));
		}
	}
}

BOOL CTitleTip::PreTranslateMessage(MSG* pMsg) 
{
	CWnd *pWnd;
	int hittest;

	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN: 
		{
			POINTS pts = MAKEPOINTS(pMsg->lParam);
			POINT  point;
			point.x = pts.x;
			point.y = pts.y;
			ClientToScreen(&point);
			pWnd = WindowFromPoint(point);
			if(pWnd == this) pWnd = m_parentWnd;

			hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));

			if (hittest == HTCLIENT)
			{
				pWnd->ScreenToClient( &point );
				pMsg->lParam = MAKELONG(point.x,point.y);
			}
			else
			{
				switch (pMsg->message)
				{
				case WM_LBUTTONDOWN: 
					pMsg->message = WM_NCLBUTTONDOWN;
					break;
				case WM_RBUTTONDOWN: 
					pMsg->message = WM_NCRBUTTONDOWN;
					break;
				case WM_MBUTTONDOWN: 
					pMsg->message = WM_NCMBUTTONDOWN;
					break;
				}
				pMsg->wParam = hittest;
				pMsg->lParam = MAKELONG(point.x,point.y);
			}
			Hide();
			pWnd->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;	
		}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		Hide();
		m_parentWnd->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	if (GetFocus() == NULL)
	{
		Hide();
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CTitleTip::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_rectHover.PtInRect(point))
	{
		ClientToScreen(&point);
		CWnd *pWnd = WindowFromPoint(point);
		if (pWnd == this) pWnd = m_parentWnd;
		pWnd->ScreenToClient(&point);
		pWnd->PostMessage(WM_LBUTTONDOWN, nFlags, MAKELONG(point.x,point.y));
		Hide();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CTitleTip::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_rectHover.PtInRect(point))
	{
		ClientToScreen(&point);
		CWnd *pWnd = WindowFromPoint(point);
		if (pWnd == this) pWnd = m_parentWnd;
		pWnd->ScreenToClient(&point);
		pWnd->PostMessage(WM_LBUTTONDOWN, nFlags, MAKELONG(point.x,point.y));
		Hide();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CTitleTip::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_rectHover.PtInRect(point))
	{
		ClientToScreen(&point);
		CWnd *pWnd = WindowFromPoint(point);
		if (pWnd == this) pWnd = m_parentWnd;
		pWnd->ScreenToClient(&point);
		pWnd->PostMessage(WM_LBUTTONDBLCLK, nFlags, MAKELONG(point.x,point.y));
		Hide();
	}	
	CWnd::OnLButtonDblClk(nFlags, point);
}
