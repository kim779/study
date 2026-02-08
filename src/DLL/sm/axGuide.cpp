// axGuide.cpp : implementation file
//

#include "stdafx.h"
#include "axGuide.h"
#include "resource.h"

#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxGuide

CAxGuide::CAxGuide(int wndPOS, CString rootDir)
{
	m_pWnd       = NULL;

	m_textRGB    = PALETTERGB( 47,  55,  68);
	m_bkRGB      = PALETTERGB(255, 255, 255);
	m_frameRGB   = PALETTERGB( 47,  55,  68);

	m_fontSize.cx = 8;
	m_fontSize.cy = 16;
	const int width = GetSystemMetrics(SM_CXFULLSCREEN);
	if (width < 900)
		m_fontSize -= CSize(2, 3);
	if (width < 800)
		m_fontSize -= CSize(2, 3);
	m_height = m_fontSize.cy * 2 + m_fontSize.cy / 2;

	m_wndPOS = (_wndPOS) wndPOS;

	m_bitmap.m_hObject = NULL;

	CString	string;
	string.Format("%s\\%s\\%s", rootDir, IMAGEDIR, "error.bmp");
	HBITMAP	bitmap = (HBITMAP) LoadImage(NULL, string, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (bitmap)
	{
		m_bitmap.Attach((HBITMAP) bitmap);
		BITMAP	bm;
		m_bitmap.GetBitmap(&bm);
		m_szBitmap = CSize(bm.bmWidth, bm.bmHeight);
		m_height = bm.bmHeight;
	}
}

CAxGuide::~CAxGuide()
{
}


BEGIN_MESSAGE_MAP(CAxGuide, CWnd)
	//{{AFX_MSG_MAP(CAxGuide)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAxGuide message handlers

int CAxGuide::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreateFont(m_fontSize.cy, 0, 0, 0,
			m_fontSize.cx < 8 ? FW_NORMAL : FW_SEMIBOLD, 0,
			false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, fontName);
	return 0;
}

void CAxGuide::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect	clientRC;
	GetClientRect(&clientRC);

	if (m_bitmap.m_hObject == NULL)
	{
		CRect	effectRC = clientRC;
		effectRC.OffsetRect(-1, -1);
		
		CBrush	bkBrush;
		bkBrush.CreateSolidBrush(m_bkRGB);

		CBrush	frameBrush;
		frameBrush.CreateSolidBrush(m_frameRGB);

		dc.FillRect(&clientRC, &bkBrush);
		dc.FrameRect(&clientRC, &frameBrush);
		dc.FrameRect(&effectRC, &frameBrush);
	}
	else
	{
		CDC mdc;
		mdc.CreateCompatibleDC(&dc);
		CBitmap* oldBitmap = mdc.SelectObject(&m_bitmap);
		const int oldBkMode = mdc.SetBkMode(TRANSPARENT);

		dc.StretchBlt(clientRC.left, clientRC.top, clientRC.right, clientRC.bottom, 
			&mdc, 0, 0, m_szBitmap.cx, m_szBitmap.cy, SRCCOPY);
		
		mdc.SetBkMode(oldBkMode);
		mdc.SelectObject(oldBitmap);
	}

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_textRGB);

	clientRC.top += 1;
	CFont	*pFont = dc.SelectObject(&m_font);
	if (m_text.Find(0x0a) == -1)
		dc.DrawText(m_text, &clientRC, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	else
	{
		clientRC.OffsetRect(0, m_fontSize.cy/2);
		dc.DrawText(m_text, &clientRC, DT_CENTER);
	}

	dc.SelectObject(pFont);
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CAxGuide::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_text = _T("");
	ShowWindow(SW_HIDE);
	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CAxGuide::Create(CWnd *pWnd)
{
	const BOOL bRet = CWnd::CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(0), NULL,
					WS_POPUP, 0, 0, 0, m_height,
					pWnd->GetSafeHwnd(), NULL, NULL);
	if (bRet) SetOwner(pWnd);
	m_pWnd = pWnd;
	return bRet;
}

void CAxGuide::SetGuide(CString guide, CWnd* owner)
{
#if 1
	//TRACE("[%s]\n",guide);
	m_text = guide;
	m_text.TrimRight();

	if (m_pWnd == NULL)	return;

	if (m_text.IsEmpty())
	{
		ShowWindow(SW_HIDE);
		return;
	}
	
	  

	CPaintDC dc(this);
	CFont*	pFont = dc.SelectObject(&m_font);
	int	width  = dc.GetTextExtent(m_text + "    ").cx;
	dc.SelectObject(pFont);

//	width.cx += m_fontSize.cx * 2;
//	if (width.cx < m_fontSize.cx * 30)
//		width.cx = m_fontSize.cx * 30;
	/*
	if (!m_text.IsEmpty())
	{
		CString tmp; tmp.Format("%d, %d",width, m_szBitmap.cx);
		AfxMessageBox(tmp);
	}
	*/
	if (m_bitmap.m_hObject != NULL)
	if (width <= m_szBitmap.cx)
		width = m_szBitmap.cx;
	else
	{
		width += m_szBitmap.cx/5;
	}
	

	CRect	cRc;
	if (owner == NULL)
		::GetWindowRect(m_pWnd->m_hWnd, &cRc);
	else	owner->GetWindowRect(&cRc);

	CPoint	point(cRc.left, cRc.bottom);
	if (cRc.Width() > width)
	{
		point.x += (cRc.Width() - width) / 2;
	}

	point.y -= m_height;
	if (cRc.Height() > m_height * 2)
	{
		switch (m_wndPOS)
		{
		case wndTOP:
			point.y = cRc.top;
			break;
		case wndBOTTOM:
			point.y = cRc.bottom - m_height;
			break;
		case wndCENTER:
		default:
			point.y -= cRc.Height() / 2;
			break;
		}
	}
	SetWindowPos(&wndTop, point.x, point.y,	width, m_height, SWP_NOACTIVATE);
	ShowWindow(SW_SHOWNOACTIVATE); Invalidate(FALSE);
#else
	m_text = guide;
	m_text.TrimRight();

	if (m_pWnd == NULL)	return;

	if (m_text.IsEmpty())
	{
		ShowWindow(SW_HIDE);
		return;
	}

	bool	multi = false;
	if (guide.Find(0x0a) != -1)
		multi = true;

	CPaintDC dc(this);
	CFont	*pFont = dc.SelectObject(&m_font);
	CSize	width = dc.GetTextExtent("MSG");
	int	cx, lineN = 0;
	if (multi)
	{
		int	pos;
		CString	tmps, dat = m_text;

		while (!dat.IsEmpty())
		{
			pos = dat.Find("\r\n");
			if (pos == -1)
			{
				cx = dc.GetTextExtent(dat).cx;
				if (cx > width.cx)
					width.cx = cx;
				lineN++;
				break;
			}
			tmps = dat.Left(pos++);
			dat = dat.Mid(++pos);
			
			cx = dc.GetTextExtent(tmps).cx;
			if (cx > width.cx)
				width.cx = cx;
			lineN++;
		}
	}
	else	width = dc.GetTextExtent(m_text);
	dc.SelectObject(pFont);

	width.cx += m_fontSize.cx * 2;
	if (width.cx < m_fontSize.cx * 30)
		width.cx = m_fontSize.cx * 30;
	if (!lineN)	lineN = 1;

	width.cy = width.cy*(lineN + 1);
	CRect	cRc;
	if (owner == NULL)
		::GetWindowRect(m_pWnd->m_hWnd, &cRc);
	else	owner->GetWindowRect(&cRc);

	::GetWindowRect(m_pWnd->m_hWnd, &cRc);
	CPoint	point(cRc.left, cRc.bottom);
	if (cRc.Width() > width.cx)
	{
		point.x += (cRc.Width() - width.cx) / 2;
	}

	point.y -= m_height;
	if (cRc.Height() > m_height * 2)
	{
		switch (m_wndPOS)
		{
		case wndTOP:
			point.y = cRc.top;
			break;
		case wndBOTTOM:
			point.y = cRc.bottom - m_height;
			break;
		case wndCENTER:
		default:
			point.y -= cRc.Height() / 2;
			break;
		}
	}
	if (multi)
		SetWindowPos(&wndTop, point.x, point.y,	width.cx, width.cy, SWP_NOACTIVATE);
	else	SetWindowPos(&wndTop, point.x, point.y,	width.cx, m_height, SWP_NOACTIVATE);
	ShowWindow(SW_SHOWNOACTIVATE); Invalidate(FALSE);
#endif
}

void CAxGuide::SetPos(int pos)
{
	switch (pos)
	{
	case 1:	m_wndPOS = wndTOP;	break;
	case 2: m_wndPOS = wndBOTTOM;	break;
	default: m_wndPOS = wndCENTER;	break;
	}
}