// NTab.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "NTab.h"
#include "axtabview.h"
#include "axMsg.hxx"
#include "resource.h"
#include "axMisc.h"
#include "NExpand.h"

#include "../dll/axissm/tmenu.h"
#include "../h/axisvar.h"

#include <afxtempl.h>

#define TW	8
#define RTW	12

#define	SELECT_NONE	0
#define	SELECT_MAP	1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNTab

CNTab::CNTab(CString home, CString key, class CTMenu* menu, 
	     CString aMapN, bool (*axiscall)(int, WPARAM, LPARAM), int winKey)
{
	m_sType  = SELECT_NONE;
	m_expand = false;
	m_index	 = 0;
	m_home   = home;
	m_key    = key;
	m_menu   = menu;
	m_axiscall = axiscall;
	m_winKey = winKey;

	m_axisres = NULL;
	m_exArr   = NULL;
	loadlist(aMapN);
}

CNTab::~CNTab()
{
	m_list.RemoveAll();
	if (m_exArr)	delete m_exArr;
}


BEGIN_MESSAGE_MAP(CNTab, CWnd)
	//{{AFX_MSG_MAP(CNTab)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EXPANDEVENT, OnExpand)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNTab message handlers

LRESULT CNTab::OnExpand(WPARAM wp, LPARAM lp)
{
	int	kind = (int) wp;
	if (kind == DIR_LEFT)	m_index--;
	if (kind == DIR_RIGHT)	m_index++;

	Draw();


	return 0;
}

void CNTab::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	Draw(&dc);
}

void CNTab::Draw(CDC* pDC)
{
	CDC		memDC;
	CRect		cRc;
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		memDC.SetBkColor(RGB(255, 0, 0));
		memDC.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);

		DrawTabImg(&memDC);
		pDC->BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
	else
	{
		pDC->SetBkColor(RGB(255, 0, 0));
		pDC->ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);

		DrawTabImg(pDC);
	}
}

void CNTab::DrawTabImg(CDC* pDC)
{
	CRect	cRc, tRc(0,0,0,0), rRc, rc;
	CSize	size;
	CFont*	font;
	bool	expand = false;
	CString	mapName, text;
	COLORREF color;
	
	GetClientRect(cRc);
	cRc.bottom = TABH;

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(pDC, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		drawImg(&memDC, TAB_BKGROUND, cRc, true);	//	background
		for (int ii = m_index; ii < m_list.GetSize() && !expand; ii++)
		{
			mapName = m_list.GetAt(ii);
			text = GetTabString(mapName);
			if (text.IsEmpty())	text = m_menu->GetDesc(mapName);
			text = " " + text + " ";

			if (ii == m_activeN)
			{
				font  = memDC.SelectObject(m_axisres->GetBoldFont());
				color = memDC.SetTextColor(m_axisres->GetColor(26));
			}
			else
			{
				font  = memDC.SelectObject(m_axisres->GetFont());
				color = memDC.SetTextColor(m_axisres->GetColor(63));
			}
			
			size = memDC.GetTextExtent(text);
			tRc.SetRect(tRc.right, 0, tRc.right + size.cx + TW*2, cRc.bottom);
			
			if (tRc.right > cRc.right)
			{
				m_expand = true;
				expand = true;
			}
			rRc.CopyRect(&tRc);
			
			rRc.right = rRc.left + TW;
			drawImg(&memDC, ii == m_activeN ? TAB_SLEFT : TAB_LEFT, rRc);
			
			rRc.left  = rRc.right;
			rRc.right = rRc.left + size.cx;
			drawImg(&memDC, ii == m_activeN ? TAB_SCENTER : TAB_CENTER, rRc, true);
			
			memDC.SetBkMode(TRANSPARENT);
			rc.SetRect(rRc.left, rRc.top + 5, rRc.right, rRc.bottom - 4);
			memDC.DrawText(text, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			memDC.SelectObject(font);
			memDC.SetTextColor(color);

			rRc.left  = rRc.right;
			rRc.right = rRc.left + TW;
			drawImg(&memDC, ii == m_activeN ? TAB_SRIGHT : TAB_RIGHT, rRc);

			tRc.right = rRc.right;
		}
		
		cRc.bottom++;
		color = m_axisres->GetColor(67);
		memDC.Draw3dRect(cRc, color, color);
		pDC->BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
		bitmap.DeleteObject();

		if (!expand && m_index != 0)	m_expand = true;
		if (m_expand)
		{
			m_exArr->SetStatus(m_index == 0 ? STAT_DISABLE : STAT_ENABLE, expand ? STAT_ENABLE : STAT_DISABLE);
			m_exArr->GetWindowRect(&rc);
			m_exArr->SetWindowPos(NULL, cRc.right - rc.Width() - 1, 1, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
		}
		else	m_exArr->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE);
	}
}

void CNTab::drawImg(CDC* pDC, int index, CRect iRc, bool stretch)
{
	CDC	memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return;

	BITMAP	bm;
	CBitmap* bitmap	= m_axisres->GetTabImg(index);
	if (bitmap)
	{
		CBitmap* pbitmap= memDC.SelectObject(bitmap);
		bitmap->GetBitmap(&bm);
		
		if (stretch)
		{
			pDC->StretchBlt(iRc.left, iRc.top, iRc.Width(), iRc.Height(),
				&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		}
		else	pDC->BitBlt(iRc.left, iRc.top, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
		
		memDC.SelectObject(pbitmap);
	}
	memDC.DeleteDC();
}

void CNTab::ChangeSkin()
{
	Draw();
}

void CNTab::ChangeTab(CString mapN)
{
	for (int ii = 0; ii < m_list.GetSize(); ii++)
	{
		if (mapN != m_list.GetAt(ii))
			continue;
		m_activeN = ii;
		m_expand = false;
		m_index = 0;
		break;
	}

	while (!IsVisible(m_activeN))
	{
		if (m_index < m_list.GetSize()-1)
			m_index++;
		else
		{
			m_index = m_activeN;
			break;
		}
	}
	Draw();
}

void CNTab::loadlist(CString aMapN)
{
	CString	file, dat, string;
	char	buffer[1204];

	file.Format("%s\\%s\\axistab.ini", m_home, TABDIR);
	GetPrivateProfileString("tabview", m_key, "", buffer, sizeof(buffer), file);
	
	string = buffer;
	m_list.RemoveAll();
	while (!string.IsEmpty())
	{
		int pos = string.Find(',');
		if (pos == -1)
		{
			string.TrimLeft();
			string.TrimRight();
			if (string.GetLength() == L_MAPN)
			{
				if (!string.Compare(aMapN))
					m_activeN =(int) m_list.GetSize();
				if (m_menu->ExistMenu(string))
					m_list.Add(string);
			}
			break;
		}

		dat = string.Left(pos);
		string = string.Mid(pos+1);
		dat.TrimLeft();	dat.TrimRight();
		if (dat.GetLength() == L_MAPN)
		{
			if (!dat.Compare(aMapN))
				m_activeN = (int)m_list.GetSize();
			if (m_menu->ExistMenu(dat))
				m_list.Add(dat);
		}
	}
}

int CNTab::GetSelect(CPoint point)
{
	CClientDC	dc(this);
	CRect	cRc, tRc(0,0,0,0), rRc;
	CSize	size;
	CFont*	font;
	bool	expand = false;
	CString	mapName, text;
	
	GetClientRect(cRc);
	cRc.bottom = TABH;

	for (int ii = m_index; ii < m_list.GetSize() && !expand; ii++)
	{
		mapName = m_list.GetAt(ii);
		text = GetTabString(mapName);
		if (text.IsEmpty())	text = m_menu->GetDesc(mapName);
		text = " " + text + " ";

		if (ii == m_activeN)
			font  = dc.SelectObject(m_axisres->GetBoldFont());
		else	font  = dc.SelectObject(m_axisres->GetFont());
		size = dc.GetTextExtent(text);	
		dc.SelectObject(font);

		tRc.SetRect(tRc.right, 0, tRc.right + size.cx + TW*2, cRc.bottom);
		if (tRc.right > cRc.right)	expand = true;
		rRc.CopyRect(&tRc);		rRc.DeflateRect(0, 1, 0, 0);

		if (!rRc.PtInRect(point))	continue;
		if (ii == m_activeN)		return SELECT_NONE;
		
		m_mapN = m_list.GetAt(ii);
		return SELECT_MAP;
	}

	return SELECT_NONE;
}

void CNTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_sType = GetSelect(point);
	switch (m_sType)
	{
	default:	break;
	case SELECT_MAP:
		(*m_axiscall)(AXI_TABCHANGE, m_winKey, (LPARAM)(char *)m_mapN.operator LPCTSTR());
		break;
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}


bool CNTab::IsVisible(int index)
{
	if (m_list.GetSize() <= 0)	return true;
	CClientDC	dc(this);
	CRect		cRc, tRc(0,0,0,0);
	CSize		size;
	CFont*		font;
	CString		mapName, text;
	bool		visible = false;
	
	GetClientRect(cRc);
	cRc.bottom = TABH;

	for (int ii = m_index; ii < m_list.GetSize() && !visible; ii++)
	{
		mapName = m_list.GetAt(ii);
		text = GetTabString(mapName);
		if (text.IsEmpty())	text = m_menu->GetDesc(mapName);
		text = " " + text + " ";

		if (ii == m_activeN)
			font  = dc.SelectObject(m_axisres->GetBoldFont());
		else	font  = dc.SelectObject(m_axisres->GetFont());
		
		size = dc.GetTextExtent(text);	dc.SelectObject(font);
		tRc.SetRect(tRc.right, 0, tRc.right + size.cx + TW*2, cRc.bottom);

		if (ii + 1 < m_list.GetSize() && ii + 1 != m_activeN)
			tRc.right = tRc.left + size.cx + TW + RTW;

		if (ii == index && tRc.right < cRc.right)
			visible = true;
		dc.SelectObject(font);
	}
	return visible;
}

CString CNTab::GetTabString(CString mapName)
{
	CString	file;
	char	buf[64];

	file.Format("%s\\%s\\axistab.ini", m_home, TABDIR);
	DWORD dw = GetPrivateProfileString("tabcaption", mapName, "", buf, sizeof(buf), file);
	if (dw <= 0)	return _T("");
	return buf;
}

void CNTab::OnNcPaint() 
{
	CWnd::OnNcPaint();
	CWindowDC dc(this);
	DrawEdge(&dc);
}

void CNTab::SetInfo(class _axisRes* axisres)
{
	m_axisres = axisres;
}

void CNTab::DrawEdge(CDC* pDC)
{
	CRect		winRc, rc;
	COLORREF	color;

	GetWindowRect(winRc);
	winRc -= winRc.TopLeft();
	rc = winRc;

	if (m_axisres)
	{
		color = m_axisres->GetColor(ACTFLINE_4);
		pDC->Draw3dRect(rc, color, color);
		rc.DeflateRect(1, 1);

		color = m_axisres->GetColor(ACTFLINE_5);
		pDC->Draw3dRect(rc, color, color);
	}
}

int CNTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_exArr = new CNExpand(this, m_home);
	if (!m_exArr->Create(NULL, "NExpand", WS_CHILD, CRect(0, 0, 0, 0), this, (UINT) m_exArr))
	{
		delete m_exArr;
		m_exArr = NULL;
	}
	return 0;
}
