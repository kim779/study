// Tips.cpp : implementation file
//

#include "stdafx.h"
#include "Tips.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_TIPS		100
#define	H_FONT		13

/////////////////////////////////////////////////////////////////////////////
// CTips

CTips::CTips()
{
	m_ClassName  = AfxRegisterWndClass(0);

	m_width      = 50;
	m_height     = H_FONT + 4;

	m_mouse	     = false;
	m_key        = -1;

	m_textRGB    = RGB(0,     0,   0);
	m_bkRGB      = RGB(255, 255, 255);
	m_frameRGB   = RGB(0,     0,   0);

}

CTips::~CTips()
{
	RemoveTips();
}


BEGIN_MESSAGE_MAP(CTips, CWnd)
	//{{AFX_MSG_MAP(CTips)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTips message handlers

int CTips::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_font.CreateFont(H_FONT, 0, 0, 0,
			FW_NORMAL, 0, false, false,
			HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			FIXED_PITCH | FF_DONTCARE, "±¼¸²");
	
	return 0;
}

void CTips::OnPaint() 
{
	CPaintDC dc(this);		// device context for painting

	CRect	clientRc;
	GetClientRect(&clientRc);
	
	dc.FillSolidRect(&clientRc, m_bkRGB);
	
	CBrush	frameBrush;
	frameBrush.CreateSolidBrush(m_frameRGB);
	dc.FrameRect(&clientRc, &frameBrush);
	
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_textRGB);

	clientRc.top += 2;
	CFont*	font = dc.SelectObject(&m_font);
	if (m_text.Replace("\n", "\n"))
	{
		clientRc.DeflateRect(2, 0, 0, 0);
		dc.DrawText(m_text, &clientRc, DT_LEFT|DT_NOPREFIX);
	}
	else
		dc.DrawText(m_text, &clientRc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	dc.SelectObject(font);
}

void CTips::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_TIPS)
	{
		KillTimer(TM_TIPS);
		if (m_mouse)
		{
			ShowWindow(SW_HIDE);
			m_mouse = false;
		}
	}
}

BOOL CTips::Create(CWnd* pWnd)
{
	BOOL bRet = CWnd::CreateEx(WS_EX_TOOLWINDOW, m_ClassName, NULL,
					WS_POPUP, 0, 0, m_width, m_height,
					/*pWnd->GetSafeHwnd()*/NULL, NULL, NULL);
	m_client = pWnd;
	if (bRet)
		SetOwner(pWnd);
	return bRet;
}


void CTips::RemoveTips()
{
	m_tips.RemoveAll();

	m_mouse = false;
	m_key    = -1;
	if (IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);
}

void CTips::RelayEvent(UINT msg, LPARAM lParam)
{
	if (m_tips.IsEmpty())
		return;

	switch (msg)
	{
	case WM_NCLBUTTONDOWN:
	case WM_NCMOUSEMOVE:
		m_mouse = false;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSELEAVE:
		if (IsWindowVisible())
			ShowWindow(SW_HIDE);
		break;

	case WM_MOUSEMOVE:
	{
		CPoint	point;
		CString	tips;

		if (AfxGetApp()->m_pMainWnd->GetFocus() == NULL)
			break;

		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);

		//point.y += H_FONT;
		m_client->ClientToScreen(&point);

		if (m_mouse)
		{
			/* TOOLTIPS moving mode ...
			if (IsWindowVisible())
			{
				SetWindowPos(&wndTop, point.x, point.y,
						m_width, m_height, SWP_NOACTIVATE);
				ShowWindow(SW_SHOWNOACTIVATE);
			}
			*/
			if (!m_currentRect.PtInRect(point))
			{
				m_mouse = false;
				ShowWindow(SW_HIDE);
			}
			break;
		}
	}
		break;
	default:
		break;
	}
}

void CTips::ShowTip(int key, CPoint mousePos)
{
	if (m_tips.IsEmpty() || key < 0)
		return;
	if (AfxGetApp()->m_pMainWnd->GetFocus() == NULL)
		return;
	if (m_mouse && m_key != key)
	{
		m_mouse = false;
		ShowWindow(SW_HIDE);
		return;
	}

	bool bRedraw = false;
	CString tips;
	CSize sz;
	mousePos.y += H_FONT + 15;
	if (m_tips.Lookup(key, tips))
	{
		if (m_text.Compare(tips))
		{
			m_text = tips;
			m_text.TrimRight();
			bRedraw = true;
		}

		if (m_text.IsEmpty())
			return;
		
		m_height = 2;
		CString sText = m_text;
		for (int ii = 0; ii <= m_text.Replace("\n", "\n"); ii++)
		{
			CString sTmp = Parser(sText, "\n");
			CDC	*pDC = GetDC();
			sz = pDC->GetTextExtent(sTmp);
			m_width = max(sz.cx, m_width);
			m_height += sz.cy;
			ReleaseDC(pDC);
		}
		if (!m_text.Replace("\n", "\n"))
			m_width = sz.cx + 10;
		
		SetWindowPos(&wndTopMost, mousePos.x, mousePos.y,
			m_width, m_height, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		//ShowWindow(SW_SHOWNOACTIVATE);
		if (bRedraw)
			RedrawWindow();
		m_mouse = true;
		m_key = key;
		
		//KillTimer(TM_TIPS);
		//SetTimer(TM_TIPS, time, NULL);
	}
}

void CTips::killMouse(int key)
{
	if (!IsWindow(m_hWnd))
		return;
	if (key == m_key || key == -1)
	{
		m_mouse = true;
		m_key	= -1;
		ShowWindow(SW_HIDE);
	}
}

CString CTips::Parser(CString &srcstr, CString substr)
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
	return _T("");
}
