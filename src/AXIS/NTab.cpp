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

#include "../dll/sm/tmenu.h"


//#define	GAP	4
#define TW	8
#define RTW	8
//#define	TGAP	12//16

#define	SELECT_NONE	0
#define	SELECT_MAP	1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNTab

CNTab::CNTab(CString key, class CTMenu* menu, 
	     CString aMapN, bool (*axiscall)(int, WPARAM, LPARAM), int winKey)
{
	m_sType  = SELECT_NONE;
	m_expand = false;
	m_index	 = 0;
	m_key    = key;
	m_menu   = menu;
	m_axiscall = axiscall;
	m_winKey = winKey;

	m_resourceHelper = ResourceHelper();

	m_exArr   = NULL;
	loadlist(aMapN);
}

CNTab::~CNTab()
{
	m_list.RemoveAll();
#ifndef DF_USE_CPLUS17
	if (m_exArr)	delete m_exArr;
#endif
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
	const int	kind = (int)
		wp;
	if (kind == DIR_LEFT)	
		m_index--;
	else if (kind == DIR_RIGHT)	
		m_index++;

	TRACE("OnExpand........\n");

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
	CFont* font{};
	bool	expand = false;
	CString	mapName, text;
	COLORREF color{};
	
	GetClientRect(cRc);
	cRc.bottom = TABH;

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap	bitmap;

		bitmap.CreateCompatibleBitmap(pDC, cRc.Width(), cRc.Height());

		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		CBrush br;
		br.CreateSolidBrush(Axis::GetColor(181));//64));
		
		memDC.FillRect(cRc, &br);
		
		for (int ii = m_index; ii < m_list.GetSize() && !expand; ii++)
		{
			mapName = m_list.GetAt(ii);
			text = GetTabString(mapName);
			if (text.IsEmpty())	text = m_menu->GetDesc(mapName);
			const int find = text.ReverseFind(':');
			if (find > 0)
				text = text.Right(text.GetLength() - (find + 1));
		
			text = " " + text + " ";

			if (ii == m_activeN)
			{
				font  = memDC.SelectObject(m_resourceHelper->GetBoldFont());
				color = memDC.SetTextColor(Axis::GetColor(71));
			}
			else
			{
				font  = memDC.SelectObject(m_resourceHelper->GetFont());
				color = memDC.SetTextColor(Axis::GetColor(70));
			}
			
			size = memDC.GetTextExtent(text);
			tRc.SetRect(tRc.right, 0, tRc.right + size.cx + TW*2, cRc.bottom);
			
			if (tRc.right > cRc.right)
			{
				m_expand = true;
				expand = true;
			}

			rRc.CopyRect(&tRc);
			//rRc.OffsetRect(0, 1);
			
			if (!ii)
				rRc.left ++;
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
			rRc.right = rRc.left + RTW+2;		
			drawImg(&memDC, ii == m_activeN ? TAB_SRIGHT : TAB_RIGHT, rRc);

			tRc.right = rRc.right;
			
		}
		color = m_resourceHelper->GetColor(64);
		CPen pt; pt.CreatePen(PS_SOLID, 1, color);
		memDC.SelectObject(&pt);
		memDC.MoveTo(0, 0);
		memDC.LineTo(0, cRc.bottom-1);
		memDC.MoveTo(cRc.right-1, 0);
		memDC.LineTo(cRc.right-1, cRc.bottom-1);


		color = m_resourceHelper->GetColor(44);
		CPen pn; pn.CreatePen(PS_SOLID, 1, color);//  pn.CreateStockObject(241);// .CreateSolidBrush(color);
		memDC.SelectObject(&pn);
		memDC.MoveTo(tRc.right, tRc.bottom-1);
		memDC.LineTo(cRc.right, cRc.bottom-1);
		cRc.bottom++;
		
		
		//memDC.Draw3dRect(cRc, color, color);
		
		pDC->BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		/*
		CBrush cbr; cbr.CreateSolidBrush(m_resourceHelper->GetColor(214));
		pDC->SelectObject(&cbr);
		tRc.left = tRc.right;
		tRc.right = cRc.right;
		pDC->FillRect(tRc, &cbr);
		*/
		/*
		CString tmp; tmp.Format("Width : %d", cRc.Height());
		AfxMessageBox(tmp);
		*/
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
		else	
			m_exArr->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE);
	}
}

void CNTab::drawImg(CDC* pDC, int index, CRect iRc, bool stretch)
{
	//stretch = true;
	CDC	memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return;

	CBitmap* bitmap	= m_resourceHelper->GetTabImg(index);
	if (!bitmap)
		return;

	BITMAP	bm{};

	CBitmap* pbitmap= memDC.SelectObject(bitmap);
	bitmap->GetBitmap(&bm);

	if (stretch)
	{
		pDC->StretchBlt(iRc.left, iRc.top, iRc.Width(), iRc.Height(),
				&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	}
	else	
		pDC->BitBlt(iRc.left, iRc.top, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pbitmap);
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
		// 20070102 cej tab view resize½Ã ¹«ÇÑ loop
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
	CString	dat, string;

	CProfile profile(pkAxisTab);
	string = profile.GetString(szTabView, m_key);
	m_list.RemoveAll();

	while (!string.IsEmpty())
	{
		const int pos = string.Find(',');
		if (pos == -1)
		{
			string.TrimLeft();
			string.TrimRight();
			if (string.GetLength() == L_MAPN)
			{
				if (!string.Compare(aMapN))
					m_activeN = m_list.GetSize();
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
				m_activeN = m_list.GetSize();
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
	CFont* font{};
	bool	expand = false;
	CString	mapName, text;
	
	GetClientRect(cRc);
	cRc.bottom = TABH;

// 	CString tmsg;
// 	tmsg.Format("ClientRect (%d/%d/%d/%d)\n", cRc.left, cRc.top, cRc.right, cRc.bottom);
// 	OutputDebugString(tmsg);

	for (int ii = m_index; ii < m_list.GetSize() && !expand; ii++)
	{
		mapName = m_list.GetAt(ii);
		text = GetTabString(mapName);
		if (text.IsEmpty())	
			text = m_menu->GetDesc(mapName);

		const int find = text.ReverseFind(':');
		if (find > 0)
			text = text.Right(text.GetLength() - (find + 1));
		
		text = " " + text + " ";

		if (ii == m_activeN)
			font  = dc.SelectObject(m_resourceHelper->GetBoldFont());
		else	font  = dc.SelectObject(m_resourceHelper->GetFont());
		
		size = dc.GetTextExtent(text);	dc.SelectObject(font);
		tRc.SetRect(tRc.right, 0, tRc.right + size.cx + TW*2, cRc.bottom);
#if 0
		if (tRc.right > cRc.right)	expand = true;
		rRc.CopyRect(&tRc);		rRc.DeflateRect(0, 6, 0, 0);
		if (!rRc.PtInRect(point))	continue;
		if (ii == m_activeN)		return SELECT_NONE;
#else
		if (tRc.right > cRc.right)	expand = true;
		rRc.CopyRect(&tRc);		rRc.DeflateRect(0, 6, 0, 0);

		if (ii + 1 < m_list.GetSize() && ii + 1 != m_activeN)
			rRc.right = rRc.left + size.cx + TW + RTW;
		tRc.right = rRc.right;
		
// 		tmsg.Format("GetSelect %02d = CRect(%d/%d/%d/%d) CPoint(%d/%d) => %s\n", ii, rRc.left, rRc.top, rRc.right, rRc.bottom,
// 			point.x, point.y, rRc.PtInRect(point) ? "TRUE" : "FALSE");
// 		OutputDebugString(tmsg);

		if (!rRc.PtInRect(point))	continue;
		if (ii == m_activeN)		return SELECT_NONE;
#endif		
		m_mapN = m_list.GetAt(ii);
		return SELECT_MAP;
	}
	return SELECT_NONE;
}

void CNTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	OutputDebugString("NTab OnLButtonDown - Start\n");

	m_sType = GetSelect(point);
	switch (m_sType)
	{
	default:	break;
	case SELECT_MAP:
		(*m_axiscall)(AXI_TABCHANGE, m_winKey, (LPARAM)(const char*)m_mapN);
		break;
	}
	
//	OutputDebugString("NTab OnLButtonDown - End\n");
	CWnd::OnLButtonDown(nFlags, point);
}


bool CNTab::IsVisible(int index)
{
	if (m_list.GetSize() <= 0)	return true;
	CClientDC	dc(this);
	CRect		cRc, tRc(0,0,0,0);
	CSize		size;
	CFont* font{};
	CString		mapName, text;
	bool		visible = false;
	
	GetClientRect(cRc);
	cRc.bottom = TABH;

	for (int ii = m_index; ii < m_list.GetSize() && !visible; ii++)
	{
		mapName = m_list.GetAt(ii);
		text = GetTabString(mapName);
		if (text.IsEmpty())	text = m_menu->GetDesc(mapName);
		const int find = text.ReverseFind(':');
		if (find > 0)
			text = text.Right(text.GetLength() - (find + 1));
		
		text = " " + text + " ";

		if (ii == m_activeN)
			font  = dc.SelectObject(m_resourceHelper->GetBoldFont());
		else	font  = dc.SelectObject(m_resourceHelper->GetFont());
		
		size = dc.GetTextExtent(text);	dc.SelectObject(font);
		tRc.SetRect(tRc.right, 0, tRc.right + size.cx + TW*2, cRc.bottom);

		if (ii + 1 < m_list.GetSize() && ii + 1 != m_activeN)
			tRc.right = tRc.left + size.cx + TW + RTW;

		if (ii == index && tRc.left < cRc.right-100)	visible = true;
		dc.SelectObject(font);
	}
	return visible;
}

CString CNTab::GetTabString(CString mapName)
{
	CProfile profile(pkAxisTab);
	return profile.GetString("TabCaption", mapName);
}

void CNTab::OnNcPaint() 
{
	CWnd::OnNcPaint();
	CWindowDC dc(this);
	DrawEdge(&dc);
}

void CNTab::DrawEdge(CDC* pDC)
{
	CRect		winRc, rc;
	COLORREF	color{};

	GetWindowRect(winRc);
	winRc -= winRc.TopLeft();
	rc = winRc;

	color = m_resourceHelper->GetColor(ACTFLINE_4);
	pDC->Draw3dRect(rc, color, color);
	rc.DeflateRect(1, 1);

	color = m_resourceHelper->GetColor(ACTFLINE_5);
	pDC->Draw3dRect(rc, color, color);
}

int CNTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
#ifdef DF_USE_CPLUS17
	m_exArr = std::make_unique<CNExpand>(this);
	if (!m_exArr->Create(NULL, "NExpand", WS_CHILD, CRect(0, 0, 0, 0), this, (UINT)m_exArr.get()))
		m_exArr = nullptr;
#else
	m_exArr = new CNExpand(this);
	if (!m_exArr->Create(NULL, "NExpand", WS_CHILD, CRect(0, 0, 0, 0), this, (UINT) m_exArr))
	{
		delete m_exArr;
		m_exArr = NULL;
	}
#endif
	return 0;
}
