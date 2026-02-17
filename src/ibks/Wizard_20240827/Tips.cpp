// Tips.cpp : implementation file
//

#include "pch.h"
#include "Wizard.h"
#include "Tips.h"
#include "Client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_TIPS		100
#define	TM_TIPX		101
#define	H_FONT		14

/////////////////////////////////////////////////////////////////////////////
// CTips

CTips::CTips(CGuard* guard)
{
	m_width   = 100;
	m_height  = H_FONT + 4;

	m_mouse	  = false;
	m_key     = -1;

	m_textRGB = PALETTERGB(  0,   0,   0);
	m_bkBrush.CreateSolidBrush(PALETTERGB(249, 254, 188));
	m_frBrush.CreateSolidBrush(PALETTERGB( 80,  80,  80));

	m_guard   = guard;
	m_works   = -1;
}

CTips::~CTips()
{
	m_font.DeleteObject();
	m_bkBrush.DeleteObject();
	m_frBrush.DeleteObject();
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
	
	LOGFONT	lf;

	lf.lfHeight         = 9 * 10;
	lf.lfWidth          = 0;
	lf.lfEscapement     = 0;
	lf.lfOrientation    = 0;
	lf.lfWeight         = FW_NORMAL;
	lf.lfItalic         = false;
	lf.lfUnderline      = false;
	lf.lfStrikeOut      = false;
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfQuality        = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(lf.lfFaceName, LF_FACESIZE, _T(""));

	m_font.CreatePointFontIndirect(&lf);
	return 0;
}

void CTips::OnPaint() 
{
	CPaintDC dc(this);		// device context for painting
	CFont*	font;
	CRect	clientRc;

	GetClientRect(&clientRc);
	dc.FillRect(&clientRc, &m_bkBrush);
	dc.FrameRect(&clientRc, &m_frBrush);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_textRGB);

	font = dc.SelectObject(&m_font);
	clientRc.DeflateRect(CSize(2, 2));
	dc.DrawText(m_text, &clientRc, 0);
	dc.SelectObject(font);
}

void CTips::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	switch (nIDEvent)
	{
	case TM_TIPS:
		setTips();
		break;

	case TM_TIPX:
		if (m_mouse)
		{
			ShowWindow(SW_HIDE);
			m_mouse = false;
		}
		break;
	}
}

BOOL CTips::Create(CWnd* pWnd)
{
	BOOL bRet = CWnd::CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(0), NULL, WS_POPUP, 0, 0, m_width, m_height,
					NULL /*pWnd->GetSafeHwnd()*/, NULL, NULL);	// parentWnd is set to NULL for top-level window & WS_EX_TOLLWINDOW
	if (bRet)
		SetOwner(pWnd);
	return bRet;
}

void CTips::RelayEvent(CClient* client, UINT msg, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KILLFOCUS:
	case WM_NCLBUTTONDOWN:
	case WM_NCMOUSEMOVE:
		m_mouse = false;
		[[fallthrough]];
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSELEAVE:
		if (IsWindowVisible())
			ShowWindow(SW_HIDE);
		break;

	case WM_MOUSEMOVE:
		if (AfxGetApp()->m_pMainWnd->GetFocus() == NULL || !client->m_focus)
			break;
		m_point.x = LOWORD(lParam);
		m_point.y = HIWORD(lParam);
		m_works   = client->m_key;
		KillTimer(TM_TIPS);
		SetTimer(TM_TIPS, 100, NULL);
		break;
	case WM_CLOSE:
		KillTimer(TM_TIPS);
		m_works = -1;
		[[fallthrough]];
	default:
		break;
	}
}

void CTips::setTips()
{
	CClient*	client;

	if (!m_guard->GetClient(m_works, (CWorks*&)client) || !(client->m_status & S_LOAD))
		return;

	int	form;
	CString	tips;
	CPoint	point = m_point;

	form = client->LookTips(point, tips);
	point.x += H_FONT;
	point.y += H_FONT;
	client->m_view->ClientToScreen(&point);

	if (m_mouse)
	{
		if (m_key != form)
		{
			m_mouse = false;
			ShowWindow(SW_HIDE);
		}
		return;
	}

	if (tips.IsEmpty())
	{
		m_key = -1;
		return;
	}

	if (m_key != form)
	{
		m_text = tips;
		m_text.Replace("\\n", "\n");
		m_text.TrimRight();
		if (m_text.IsEmpty())
			return;

		int	cx, cy, idx;
		CSize	size;
		CDC*	dc = GetDC();
		CFont*	font;
		CString	text;

		font = (CFont *)dc->SelectObject(&m_font);
		tips = m_text;
		for (cx = cy = 0; !tips.IsEmpty(); )
		{
			idx = tips.Find('\n');
			if (idx == -1)
			{
				text = tips;
				tips.Empty();
			}
			else
			{
				text = tips.Left(idx++);
				tips = tips.Mid(idx);
			}
			size = dc->GetOutputTextExtent(text);
			cx = max(cx, size.cx);
			cy += size.cy;
		}

		m_width  = cx + 4;
		m_height = cy + 4;
		dc->SelectObject(font);
		ReleaseDC(dc);

		CRect	cRc;
		client->m_view->GetWindowRect(cRc);
		if (cRc.right < point.x + m_width)
			point.x -= (point.x + m_width - cRc.right);
		point.x = max(point.x, cRc.left);

		SetWindowPos(&wndTopMost, point.x, point.y, m_width, m_height, SWP_NOACTIVATE);
		ShowWindow(SW_SHOWNOACTIVATE);
		m_mouse = true;
		m_key = form;

		TRACKMOUSEEVENT	csTME;

		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = client->m_view->GetSafeHwnd();
		::_TrackMouseEvent(&csTME);
		/*
		KillTimer(TM_TIPX);
		SetTimer(TM_TIPX, 3000, NULL);
		*/
	}
}
