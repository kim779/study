// ConfigBar.cpp : implementation file
//

#include "stdafx.h"
#include "IB100000.h"
#include "ConfigBar.h"
#include "xtab.h"
#include "usetup.h"

#define	GAP		2

#define	STAT_COOL	0
#define	STAT_OVER	1
#define	STAT_DOWN	2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigBar

CConfigBar::CConfigBar(CWnd* base, CString home, CString user)
{
	m_base = base;
	m_home = home;
	m_user = user;
	m_mode = MODE_DEFAULT;

	m_mouseIN = false;
	m_pushON = false;
	m_menuS = true;
	m_hover = -1;
	m_push = -1;

	m_son.LoadBitmap(IDB_SON);
	//**m_soff.LoadBitmap(IDB_SOFF);
	m_bitmap[0].LoadBitmap(IDB_SETUP);
	m_bitmap[1].LoadBitmap(IDB_MENU1);
	m_bitmap[2].LoadBitmap(IDB_MENU2);

	m_bColor = RGB(181, 192, 218);

	m_xTab = NULL;
	m_arM.RemoveAll();

	m_sizeRate[0] = m_sizeRate[1] = 1.f;
}

CConfigBar::~CConfigBar()
{
	if (m_son.GetSafeHandle())	m_son.DeleteObject();
	if (m_soff.GetSafeHandle())	m_soff.DeleteObject();
	if (m_bitmap[0].GetSafeHandle())m_bitmap[0].DeleteObject();
	if (m_bitmap[1].GetSafeHandle())m_bitmap[1].DeleteObject();
	if (m_bitmap[2].GetSafeHandle())m_bitmap[2].DeleteObject();

	m_arM.RemoveAll();
}


BEGIN_MESSAGE_MAP(CConfigBar, CWnd)
	//{{AFX_MSG_MAP(CConfigBar)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConfigBar message handlers

void CConfigBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc;
	GetClientRect(cRc);
	CPen *open, pen;
	pen.CreatePen(PS_SOLID, 1, RGB(96, 118, 160));
	open = dc.SelectObject(&pen);
	dc.Rectangle(cRc); 

	cRc.DeflateRect(1,1,1,0); 
	dc.FillSolidRect(cRc, m_bColor); 

//	dc.SetBkColor(m_bColor);//RGB(96, 118, 160));
//	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);

	DrawImg();
	if (m_xTab)
	{
		CRect	xtRc;
		xtRc.SetRect(m_iRc[2].right + GAP*2, GAP, cRc.right - GAP*2, cRc.bottom - GAP);
		m_xTab->MoveWindow(xtRc);
	}

//	dc.MoveTo(CPoint(cRc.left, cRc.top));
//	dc.LineTo(CPoint(cRc.right, cRc.top));
	dc.SelectObject(open);

//	dc.FillSolidRect(cRc, 0x0000ff); 

}

BOOL CConfigBar::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	const int	comm = LOWORD(wParam);
	if (comm >= 0 && comm < 4) //**m_arM.GetSize())
	{
		CString	file, key;

		key.Format("%02d", comm + 2);
		file.Format("%s\\tab\\종합config.ini", m_home);

		WritePrivateProfileString("GENERAL", "option", "", file);

		CString item;
		if (m_menuS)
			item = "STOCK";
		else
			item = "FOPTION";
		WritePrivateProfileString("GENERAL", "section", item, file);

		char buf[256];
		GetPrivateProfileString(item, "00", "", buf, sizeof(buf), file);
		if (strlen(buf) > 0)
		{
			WritePrivateProfileString(buf, "type", key, file);
			GetParent()->SendMessage(WM_XMSG3, 0, 0);
			if (m_xTab)	m_xTab->ClearSelect();
		}
	}	

	return CWnd::OnCommand(wParam, lParam);
}

void CConfigBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for (int ii = 0; ii < 4; ii++)
	{
		if (!m_iRc[ii].PtInRect(point))
			continue;
		m_push = ii;
		DrawImg(ii, STAT_DOWN);
		SetCapture();
		break;
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CConfigBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	for (int ii = 0; ii < 4; ii++)
	{
		if (!m_iRc[ii].PtInRect(point))
			continue;
		if (ii == m_push)
		{
			DrawImg(ii);
			Operation(ii);
		}
		break;
	}

	m_push = -1;
	CWnd::OnLButtonUp(nFlags, point);
}

void CConfigBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME{};

	CWnd::OnMouseMove(nFlags, point);
	const CPoint	lpoint = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_mouseIN)
		{
			m_mouseIN = true;

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}
		HoverItem(lpoint);
	}
	else	m_mouseIN = false;
}

LRESULT CConfigBar::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_hover = -1;
	m_mouseIN = false;
	Invalidate(FALSE);
	return 0;
}

void CConfigBar::LoadInfo(bool clear)
{
	MakeXTab();
	if (m_xTab)
	{
		m_xTab->LoadInfo();
		if (clear)	m_xTab->ClearSelect();
	}
}

int CConfigBar::GetWidth()
{
	return m_iRc[3].Width() + GAP*2;
}

void CConfigBar::DrawImg()
{
	for (int ii = 0; ii < 4; ii++)
		DrawImg(ii);
}

/********************************************************
 * index 0 ~ 2 : setup, menu1, menu2					*
 * stat        : STAT_COOL, STAT_OVER, STAT_DOWN		*
 ********************************************************/
void CConfigBar::DrawImg(int index, int stat)
{
	if(index > 2) return;

	CDC*	pDC = GetDC();
	CRect	cRc, iRc;
	CDC	memDC;
	BITMAP	bm{};

	GetClientRect(cRc);
	if (!memDC.CreateCompatibleDC(pDC))
	{
		ReleaseDC(pDC);
		return;
	}

	if (m_son.GetSafeHandle())
		m_son.GetBitmap(&bm);
	else
		ZeroMemory(&bm, sizeof(BITMAP));

	iRc.SetRect( GAP*3, GAP, 
		GAP*3, GAP+bm.bmHeight);
	
	CBitmap* oldbuffBitmap{};
	int	 bx{}, orgbx{}, by{}, orgby{}, ii{}, sx{};

	switch (index)
	{
	case 0:case 1: case 2:
		oldbuffBitmap = memDC.SelectObject(&m_bitmap[index]);
		bx = bm.bmWidth/3;	by = bm.bmHeight;

		iRc.SetRect(GAP*3, GAP, 
			 GAP*3, GAP+bm.bmHeight);
		for (ii = 0; ii <= index; ii++)
		{
			m_bitmap[ii].GetBitmap(&bm);
			orgbx = bx = bm.bmWidth/3;
			bx = (int)(float(bx) * m_sizeRate[0]);
			orgby = by = bm.bmHeight;
			by = (int)(float(by) * m_sizeRate[1]);

			iRc.right = iRc.left+ bx;
			if (ii != index)
				iRc.OffsetRect(bx, 0);
		}
		break;
	default:
		memDC.DeleteDC();
		ReleaseDC(pDC);
		return;
	}

	CRect rc;
	GetClientRect(&rc);
	const int igap = max(GAP, (rc.Height() - by) / 2 );
	iRc.top = igap;
	iRc.bottom = igap + by;
	

//	memDC.FillSolidRect(iRc,  0xff0000);
	m_iRc[index].CopyRect(&iRc);
	if (oldbuffBitmap == (CBitmap *) 0)
	{
		memDC.DeleteDC();
		ReleaseDC(pDC);
		return;
	}

	switch (stat)
	{
		default:
		case STAT_COOL:	sx = 0;		break;
		case STAT_OVER:	sx = orgbx;	break;
		case STAT_DOWN:	sx = orgbx*2;	break;
	}

	pDC->StretchBlt(iRc.left, iRc.top, iRc.Width(), iRc.Height(), &memDC, sx, 0, orgbx, orgby, SRCCOPY);
	memDC.SelectObject(oldbuffBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
}

void CConfigBar::HoverItem(CPoint point)
{
	int	tmpI = -1;
	for (int ii = 0; ii < 4; ii++)
	{
		if (!m_iRc[ii].PtInRect(point))
			continue;
		tmpI = ii;
	}

	if (tmpI == m_hover)	return;
	if (m_hover != -1)	DrawImg(m_hover);
	if (tmpI == -1)
	{
		m_hover = -1;
		return;
	}

	if (tmpI == m_push)
		DrawImg(tmpI, STAT_DOWN);
	else	DrawImg(tmpI, STAT_OVER);
	m_hover = tmpI;
}

void CConfigBar::Operation(int comm)
{
	CPoint	point(0, 0);
	CString	file, text, key, appS;

	switch (comm)
	{
	case 0:
		{
			CUSetup	dlg(m_home, m_user);
			dlg.DoModal();
			if (dlg.IsChange())
			{
				if (m_xTab)	m_xTab->LoadInfo();
				GetParent()->PostMessage(WM_XMSG3, 0, 0);
			}
		}
		break;
	case 1:	
		m_menuS = true;
		appS = _T("STOCK");
		point = CPoint(m_iRc[1].left, m_iRc[1].top);
		break;
	case 2:	
		m_menuS = false;
		appS = _T("FOPTION");
		point = CPoint(m_iRc[2].left, m_iRc[2].top);
		break;
	}
	
	
	if (!appS.IsEmpty())
	{
		char	buf[256];
		CMenu	menu;

		ClientToScreen(&point);
		m_arM.RemoveAll();
		menu.CreatePopupMenu();
		file.Format("%s\\tab\\종합config.ini", m_home);

		GetPrivateProfileString("GENERAL", "section", "STOCK", buf, sizeof(buf), file);
		CString currSection(buf);

		GetPrivateProfileString(currSection, "type",  "2", buf, sizeof(buf), file);
		CString currType(buf);


		CString str;
		for (char i = 'A'; i <= 'D'; i++)
		{
			str.Format("%c타입", i);
			menu.AppendMenu(MF_STRING, i - 'A', str);
		}

		int	meunHeight = GetSystemMetrics(SM_CYMENU)*m_arM.GetSize();
		meunHeight += GetSystemMetrics(SM_CYBORDER)*2;
		point.Offset(0, -meunHeight);
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
		menu.DestroyMenu();
	}
}

void CConfigBar::MakeXTab()
{
	if (m_xTab)	return;
	m_xTab = std::make_unique<CXTab>(m_base, m_home, m_user);
	if (!m_xTab->Create(NULL, "xtab", WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, 1))
	{
		m_xTab = nullptr;
	}
}

void CConfigBar::SetSkinColor(COLORREF clrBk, COLORREF clrTabBk, COLORREF clrTabS, bool draw)
{
	m_bColor = clrBk;
	if (m_xTab)	m_xTab->SetSkinColor(clrTabBk, clrTabS, draw);
	Invalidate();
}

void CConfigBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

void CConfigBar::ChangeFont(int pointfont, CString fontName)
{
	if (m_xTab)	
	{
		const int idx = pointfont - 8;
		m_sizeRate[0] = g_lratex[idx];
		m_sizeRate[1] = g_lratey[idx];
		DrawImg();
		m_xTab->ChangeFont(pointfont, fontName);

		/*
		_fontR font;
		font.name = "굴림";
		if (!fontName.IsEmpty())
			sprintf(font.name, fontName, fontName.GetLength());

		font.point = pointfont;
		font.italic = false;
		font.bold = FW_NORMAL;

	//	CFont* pFont = (CFont *) GetParent()->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (long) &font);
	//	m_xTab->SetFont(pFont);
	*/
	}
}

