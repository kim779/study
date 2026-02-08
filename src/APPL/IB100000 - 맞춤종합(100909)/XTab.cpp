// XTab.cpp : implementation file
//

#include "stdafx.h"
#include "IB100000.h"
#include "XTab.h"

#include "base.h"

#define	BHEIGTH		20
#define	BWIDTH		60
#define GAP		2
#define	EXPANDW		8

#define LT_COLOR	RGB(171, 168, 153)
#define RB_COLOR	RGB(115, 113, 100)

#define DIRECT_NONE	0
#define DIRECT_LEFT	1
#define DIRECT_RIGHT	2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTab

CXTab::CXTab(CWnd* base, CString home, CString user)
{
	m_base = (CBase*)base;
	m_cur = -1;
	m_index = 0;
	m_dRc = CRect(0, 0, 0, 0);
	m_sColor = RGB(0, 0, 255);
	m_font.CreatePointFont(90, "굴림");
	
	m_home = home;
	m_user = user;

}

CXTab::~CXTab()
{
	m_font.DeleteObject();
	m_list.RemoveAll();
}

BEGIN_MESSAGE_MAP(CXTab, CWnd)
	//{{AFX_MSG_MAP(CXTab)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTab message handlers

void CXTab::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawButton();
}

void CXTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	cRc, bRc, eRc;
	CString	string, file;

	GetClientRect(cRc);
	bRc.CopyRect(&cRc);
	eRc.CopyRect(&cRc);
	
	eRc.DeflateRect(0, GAP);
	eRc.OffsetRect(-GAP, 0);
	
	eRc.left = eRc.right - EXPANDW*2;

	int	exN = m_list.GetSize();
	bool	expand = false;

	for (int jj = m_index, ii = 0; jj < m_list.GetSize(); ii++, jj++)
	{
		bRc.SetRect(BWIDTH*ii, 0, BWIDTH*(ii+1), cRc.bottom);
		bRc.right += GAP+1;
		bRc.DeflateRect(GAP, GAP);
		if (bRc.right >= eRc.left)
		{
			if (jj+1 < m_list.GetSize() || m_index > 0 || bRc.right >= eRc.right)
			{
				exN = ii;
				expand = true;
				break;
			}
		}
		if (!bRc.PtInRect(point))
			continue;
//		if (jj != m_cur)
//		{
			m_cur = jj;
			DrawButton();

			file.Format("%s\\tab\\종합config.ini", m_home);
			WritePrivateProfileString("GENERAL", "option", "user", file);

			file.Format("%s\\user\\%s\\종합.ini", m_home, m_user);
			string.Format("%02d", jj);
			WritePrivateProfileString("GENERAL", "key", string, file);
			m_base->PostMessage(WM_XMSG3, 0, 0);
//		}
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	
	bRc.CopyRect(&eRc);
	if (!expand && m_index)	expand = true;
	if (expand)
	{
		bool	match = false;
		bRc.right = bRc.left + EXPANDW;
		if (bRc.PtInRect(point) && !match)
		{
			if (m_index > 0)
			{
				m_index--;
				DrawButton(false);
			}
			Drawdirect(bRc, DIRECT_LEFT);
			m_dir = DIRECT_LEFT;
			m_dRc.CopyRect(&bRc);
			match = true;
		}
		bRc.OffsetRect(EXPANDW-1, 0);
		bRc.right = bRc.left + EXPANDW;
		if (bRc.PtInRect(point) && !match)
		{
			if (m_index+1 <= m_list.GetSize() - exN)
			{
				m_index++;
				DrawButton(false);
			}
			Drawdirect(bRc, DIRECT_RIGHT);
			m_dir = DIRECT_RIGHT;
			m_dRc.CopyRect(&bRc);
		}
		bRc.OffsetRect(EXPANDW/2, 0);
	}

	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

void CXTab::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	CRect	cRc, bRc, eRc;
	CString	string;

	GetClientRect(cRc);
	bRc.CopyRect(&cRc);
	eRc.CopyRect(&cRc);
	
	eRc.DeflateRect(0, GAP);
	eRc.OffsetRect(-GAP, 0);
	
	eRc.left = eRc.right - EXPANDW*2;

	bool	expand = false;
	for (int ii = 0; ii < m_list.GetSize(); ii++)
	{
		bRc.SetRect(BWIDTH*ii, 0, BWIDTH*(ii+1), cRc.bottom);
		bRc.right += GAP+1;
		bRc.DeflateRect(GAP, GAP);
		if (bRc.right >= eRc.left)
		{
			if (ii+1 < m_list.GetSize() || m_index > 0 || bRc.right >= eRc.right)
			{
				expand = true;
				break;
			}
		}
		if (!bRc.PtInRect(point))
			continue;
		break;
	}

	bRc.CopyRect(&eRc);
	if (expand && m_dir != DIRECT_NONE)
		Drawdirect(m_dRc, m_dir, false);
	m_dir = DIRECT_NONE;

	
	CWnd::OnLButtonUp(nFlags, point);
}

void CXTab::DrawButton(bool calculate)
{
	CDC*	pDC = GetDC();
	CRect	cRc, bRc, eRc, xRc;
	CString	str;

	GetClientRect(cRc);
	bRc.CopyRect(&cRc);
	eRc.CopyRect(&cRc);
	
	eRc.DeflateRect(0, GAP);
	eRc.OffsetRect(-GAP, 0);
	
	eRc.left = eRc.right - EXPANDW*2;

	pDC->SetBkColor(m_bColor);
	pDC->ExtTextOut(bRc.left, bRc.top, ETO_OPAQUE, &bRc, "", 0, NULL);
	pDC->SetBkMode(TRANSPARENT);
	CFont*	font = pDC->SelectObject(&m_font);

	int	visibleN = 0;
	bool	expand = CalculateHideButton(visibleN);
	if (m_index != 0 && !expand && calculate) 
	{
		while (m_list.GetSize() < visibleN+m_index)
			m_index--;
		if (m_index < 0)	m_index = 0;
	}

	expand = false;
	for (int jj = m_index, ii = 0 ; jj < m_list.GetSize(); ii++, jj++)
	{
		bRc.SetRect(BWIDTH*ii, 0, BWIDTH*(ii+1), cRc.bottom);
		bRc.right += GAP+1;
		bRc.DeflateRect(GAP, GAP/2);
		if (bRc.right >= eRc.left)
		{
			if (jj+1 < m_list.GetSize() || m_index > 0 || bRc.right >= eRc.right)
			{
				expand = true;
				break;
			}
		}
		str = m_list.GetAt(jj);
		if (jj == m_cur)
		{
			xRc.CopyRect(&bRc);
			pDC->FillSolidRect(xRc, m_sColor);
			const COLORREF color = pDC->SetTextColor(RGB(255, 255, 255));
			pDC->Draw3dRect(xRc, RB_COLOR, RB_COLOR);
			pDC->DrawText(str, bRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			pDC->SetTextColor(color);
		}
		else
		{
			xRc.CopyRect(&bRc);
			pDC->Draw3dRect(xRc, RB_COLOR, RB_COLOR);
			xRc.left++;	xRc.top++;
			pDC->Draw3dRect(xRc, RGB(255, 255, 255), RB_COLOR);
			pDC->DrawText(str, bRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}

	if (!expand && m_index)	
		expand = true;

	DrawExpand(eRc, expand);
	pDC->SelectObject(font);

	ReleaseDC(pDC);
}

void CXTab::DrawExpand(CRect eRc, bool expand)
{
	CDC*	pDC = GetDC();
	CRect	bRc, xRc;
	CString	str;

	pDC->SetBkMode(TRANSPARENT);
	CFont*	font = pDC->SelectObject(&m_font);

	bRc.CopyRect(&eRc);
	if (expand)
	{
		bRc.right = bRc.left + EXPANDW;
		pDC->FillSolidRect(bRc, GetSysColor(COLOR_BTNFACE));

		xRc.CopyRect(&bRc);
		pDC->Draw3dRect(xRc, RB_COLOR, RB_COLOR);
		xRc.left++;	xRc.top++;
		pDC->Draw3dRect(xRc, RGB(255, 255, 255), RB_COLOR);
		CPoint	cPoint = bRc.CenterPoint();
		pDC->MoveTo(cPoint.x-1, cPoint.y-1);
		pDC->LineTo(cPoint.x-2, cPoint.y-1);

		pDC->MoveTo(cPoint.x, cPoint.y - 2);
		pDC->LineTo(cPoint.x, cPoint.y + 1);

		pDC->MoveTo(cPoint.x+1, cPoint.y-3);
		pDC->LineTo(cPoint.x+1, cPoint.y+2);
		bRc.OffsetRect(EXPANDW-1, 0);

		bRc.right = bRc.left + EXPANDW;
		pDC->FillSolidRect(bRc, GetSysColor(COLOR_BTNFACE));

		xRc.CopyRect(&bRc);
		pDC->Draw3dRect(xRc, RB_COLOR, RB_COLOR);
		xRc.left++;	xRc.top++;
		pDC->Draw3dRect(xRc, RGB(255, 255, 255), RB_COLOR);
		
		cPoint = bRc.CenterPoint();
		pDC->MoveTo(cPoint.x - 1, cPoint.y - 3);
		pDC->LineTo(cPoint.x - 1, cPoint.y + 2);

		pDC->MoveTo(cPoint.x, cPoint.y - 2);
		pDC->LineTo(cPoint.x, cPoint.y + 1);

		pDC->MoveTo(cPoint.x-1, cPoint.y-1);
		pDC->LineTo(cPoint.x+2, cPoint.y-1);
		bRc.OffsetRect(EXPANDW, 0);
	}

	pDC->SelectObject(font);
	ReleaseDC(pDC);
}

void CXTab::Drawdirect(CRect eRc, int dir, bool push)
{
	CDC*	pDC = GetDC();
	CRect	xRc;
	CFont*	font = pDC->SelectObject(&m_font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(eRc, GetSysColor(COLOR_BTNFACE));

	xRc.CopyRect(&eRc);//	xRc.left--;
	pDC->Draw3dRect(xRc, RB_COLOR, RB_COLOR);
	xRc.left++;	xRc.top++;

	if (push)
	{
		xRc.right--;	xRc.bottom--;
		pDC->Draw3dRect(xRc, RGB(148, 146, 131), RGB(255, 255, 255));
	}
	else
	{
		pDC->Draw3dRect(xRc, RGB(255, 255, 255), RB_COLOR);
	}

	const CPoint	cPoint = eRc.CenterPoint();
	if (dir == DIRECT_LEFT)
	{
		pDC->MoveTo(cPoint.x-1, cPoint.y-1);
		pDC->LineTo(cPoint.x-2, cPoint.y-1);

		pDC->MoveTo(cPoint.x, cPoint.y - 2);
		pDC->LineTo(cPoint.x, cPoint.y + 1);

		pDC->MoveTo(cPoint.x+1, cPoint.y-3);
		pDC->LineTo(cPoint.x+1, cPoint.y+2);
	}
	else
	{
		pDC->MoveTo(cPoint.x - 1, cPoint.y - 3);
		pDC->LineTo(cPoint.x - 1, cPoint.y + 2);

		pDC->MoveTo(cPoint.x, cPoint.y - 2);
		pDC->LineTo(cPoint.x, cPoint.y + 1);

		pDC->MoveTo(cPoint.x-1, cPoint.y-1);
		pDC->LineTo(cPoint.x+2, cPoint.y-1);
	}
	pDC->SelectObject(font);
	ReleaseDC(pDC);
}

void CXTab::SetSkinColor(COLORREF bColor, COLORREF sColor, bool draw)
{
	m_bColor = bColor;
	m_sColor = sColor;
	if (draw)	
		DrawButton();
}

void CXTab::LoadInfo()
{
	char		buf[1024]{};
	char		sec[32]{};
	DWORD		dw;
	CString		file, keys, tmpS;

	file.Format("%s\\tab\\종합config.ini", m_home);
	dw = GetPrivateProfileString("GENERAL", "option", "", buf, sizeof(buf), file);
	if (dw <= 0)	m_cur = -1;
	else
	{
		file.Format("%s\\user\\%s\\종합.ini", m_home, m_user);
		dw = GetPrivateProfileString("GENERAL", "key", "00", buf, sizeof(buf), file);
		m_cur = atoi(CString(buf, dw));
	}

	file.Format("%s\\user\\%s\\종합.ini", m_home, m_user);
	dw = GetPrivateProfileString("GENERAL", "section", "", sec, sizeof(sec), file);
	if (dw <= 0)	return;
	
	m_list.RemoveAll();
	for (int ii = 0;; ii++)
	{
		keys.Format("%02d", ii);
		dw = GetPrivateProfileString(sec, keys, "", buf, sizeof(buf), file);
		if (dw <= 0)	
			break;

		m_list.Add(CString(buf, dw));
	}

	Invalidate();
}

void CXTab::ClearSelect()
{
	m_cur = -1;
	if (IsWindow(m_hWnd))
		Invalidate(FALSE);
}

bool CXTab::CalculateHideButton(int& visibleN)
{
	CRect	cRc, bRc, eRc;

	GetClientRect(cRc);
	bRc.CopyRect(&cRc);
	eRc.CopyRect(&cRc);
	
	eRc.DeflateRect(0, GAP);
	eRc.OffsetRect(-GAP, 0);
	
	eRc.left = eRc.right - EXPANDW*2;

	bool	expand = false;
	int	ii=0, jj=0;
	for (ii = 0 ; ii < m_list.GetSize(); ii++)
	{
		bRc.SetRect(BWIDTH*ii, 0, BWIDTH*(ii+1), cRc.bottom);
		bRc.right += GAP+1;
		bRc.DeflateRect(GAP, GAP/2);
		if (bRc.right >= eRc.left)
		{
			if (ii+1 < m_list.GetSize() || bRc.right >= eRc.right)
				break;
		}
	}

	if (ii > m_list.GetSize())
		ii = m_list.GetSize();
	visibleN = ii;
	for (ii = m_index, jj = 0; ii < m_list.GetSize(); ii++, jj++)
	{
		bRc.SetRect(BWIDTH*jj, 0, BWIDTH*(jj+1), cRc.bottom);
		bRc.right += GAP+1;
		bRc.DeflateRect(GAP, GAP/2);
		if (bRc.right >= eRc.left)
		{
			if (ii+1 < m_list.GetSize() || bRc.right >= eRc.right)
			{
				expand = true;
				break;
			}
		}
	}
	return expand;
}

void CXTab::ChangeFont(int ifontsize, CString sFont)
{
	if(m_font.GetSafeHandle() )
	{
		m_font.DeleteObject();
		
	}

	m_font.CreatePointFont(ifontsize*10, sFont);
	Invalidate(FALSE);
}