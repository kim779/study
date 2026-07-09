// Xcomex.cpp : implementation file
//

#include "stdafx.h"
#include "Xcomex.h"
#include "xedit.h"
#include "MenuXP.h"

#include "../../h/axisvar.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	COLOR_BUTTON	::GetSysColor(COLOR_BTNFACE)
/////////////////////////////////////////////////////////////////////////////
// CXcomex

CXcomex::CXcomex(CString home, CWnd* parent)
{
	m_home = home;
	m_parent = parent;
	m_bRc = CRect(0);

	CString file;
	file.Format("%s\\image\\mb_menusel.bmp", m_home);
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	file.Format("%s\\image\\mb_menusel_dn.bmp", m_home);
	HBITMAP hBitmap_dn = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (m_arr.GetSafeHandle())
		m_arr.DeleteObject();
	if (hBitmap)	m_arr.Attach(hBitmap);
	if (m_arr_dn.GetSafeHandle())
		m_arr_dn.DeleteObject();
	if (hBitmap_dn)	m_arr_dn.Attach(hBitmap_dn);

	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_bPush = FALSE;
}

CXcomex::~CXcomex()
{
	if (m_pen.GetSafeHandle())	m_pen.DeleteObject();
	if (m_brush.GetSafeHandle())	m_brush.DeleteObject();

	m_arr.DeleteObject();
	m_arr_dn.DeleteObject();
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CXcomex, CWnd)
	//{{AFX_MSG_MAP(CXcomex)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXcomex message handlers

void CXcomex::OnPaint() 
{
	CPaintDC dc(this);

	CRect	cRc;
	GetClientRect(cRc);

	draw_Button(&dc);

	CFont*	font = dc.SelectObject(&m_font);
	cRc.left += 5;
	cRc.right -= (COMBTN_WIDTH-1);
	int mode = dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(COLOR_BARTEXT);
	if(m_gName == "공지/게시")
		dc.DrawText("공지", cRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
	else
		dc.DrawText(m_gName, cRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
	dc.SetBkMode(mode);
	dc.SelectObject(font);
}

void CXcomex::init()
{
	m_pen.CreatePen(PS_SOLID, 0, COLOR_COMLINE);
	//m_brush.CreateSolidBrush(COLOR_BUTTON);
	m_brush.CreateSolidBrush(COLOR_BARBACKGROUND);
}

void CXcomex::changepos()
{
	CRect	cRc;
	
	GetClientRect(cRc);
	m_bRc.CopyRect(&cRc);
#if 1	// new style
	m_bRc.DeflateRect(1, 1);
#else	// old style
	m_bRc.left = cRc.right - COMBTN_WIDTH;
	m_bRc.DeflateRect(0, 2);
	m_bRc.OffsetRect(-2, 0);
#endif
}

void CXcomex::draw_Button(CDC* pDC)
{
	if (m_bRc.IsRectEmpty())	return;

	BITMAP	bm;
	CDC	memDC;

	CRect rc;
	GetClientRect(rc);
//	rc.DeflateRect(1, 1);

	if (m_bPush)
	{
		m_arr_dn.GetBitmap(&bm);
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(&m_arr_dn);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(),
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			else	memDC.DeleteDC();
		}
	}
	else
	{
		m_arr.GetBitmap(&bm);
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(&m_arr);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(),
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			else	memDC.DeleteDC();
		}
	}
}

void CXcomex::set_gName(CString name)
{
	m_gName = name;
	Invalidate(FALSE);
}

void CXcomex::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bPush = TRUE;
	Invalidate();

	CWnd::OnLButtonDown(nFlags, point);
}

void CXcomex::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_bPush = FALSE;
	Invalidate();

	if (m_bRc.PtInRect(point))
	{
		const int	bufSize = 1024*4;
		char		buf[bufSize];
		CMenuXP		menu;
		CPoint		xpoint = m_bRc.BottomRight();
		CStringArray	ary;

		xpoint += CPoint(3, 3);
		ClientToScreen(&xpoint);
		menu.CreatePopupMenu();

		CString		file, text;
		int		dwRc, idx, uindex, tot;

		text = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
		idx  = text.Find('|');
		if (idx < 0)	
			text = _T("Anonymous");
		else		
			text = text.Mid(idx+1);

		if (text.IsEmpty())	
			text = _T("Anonymous");

		file.Format("%s\\user\\%s\\usertool.ini", m_home, text);
		dwRc = GetPrivateProfileString(USERSN, USEREN, "0, 0", buf, bufSize, file);

		text = CString(buf, dwRc);
		idx =text.Find(',');
		if (idx >= 0)	uindex = atoi(text.Left(idx++));
		tot = atoi(text.Mid(idx));

		CString code = _T("");
		CString gName = _T("");
		for (int ii = 0; ii < tot; ii++)
		{
			code.Format("%02d", ii);
			dwRc = GetPrivateProfileString(USERSN, code,
						"99=", buf, bufSize, file);
			if (dwRc <= 0 || !strcmp(buf, "99="))
				break;
			text = CString(buf, dwRc);
			//if(text == "공지/게시")
			//	text = "공지";
			menu.AppendMenuX(MF_STRING, COMM_BASE + ii+1, text);
		}
		
	//	menu.AppendMenuX(MF_SEPARATOR);
	//	menu.AppendMenuX(MF_STRING, COMM_BASE, "사용자 툴바편집");

		menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, xpoint.x, xpoint.y, m_parent);
	}

	Invalidate();
	
	CWnd::OnLButtonUp(nFlags, point);
}
