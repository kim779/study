// colorBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "colorBox.h"

#include "../h/axisvar.h"
#include "../h/mainvar.h"

#define	winMARGIN	5
#define	winGAP		1
#define	itemWCOUNT	8
#define	itemHCOUNT	8
#define	itemWIDTH	17
#define	itemHEIGHT	15

// CColorBox

IMPLEMENT_DYNAMIC(CColorBox, CWnd)

CColorBox::CColorBox(CWnd* pParent, DWORD color, bool user /*=true*/, int offsetClr)
{
	CString	tmps;

	tmps = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	m_path.Format(_T("%s\\%s\\%s"), tmps, TABDIR, PALETTE);

	m_color = color;
	m_user  = user;
	m_offsetClr = offsetClr;
	m_path.Format(_T("%s\\%s\\workshop.ini"), tmps, RUNDIR);
}

CColorBox::~CColorBox()
{
}


BEGIN_MESSAGE_MAP(CColorBox, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
//	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CColorBox 메시지 처리기입니다.

void CColorBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect	rc;

	for (int ii = 0; ii < itemHCOUNT; ii++)
	{
		for (int jj = 0; jj < itemWCOUNT; jj++)
		{
			rc = makeRect(ii, jj);
			if (rc.PtInRect(point))
			{
				m_color = ii * itemWCOUNT + jj + m_offsetClr;
				GetParent()->PostMessage(WM_USER + 1, 0, m_color);
				break;
			}
		}
	}
	
	if (m_user)
	{
		rc = makeRect();
		if (rc.PtInRect(point))
			onUser();
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CColorBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect	rc;
	bool	usePAL = (m_color & userCOLOR) ? false : true;
	
	for (int ii = 0; ii < itemHCOUNT; ii++)
	{
		for (int jj = 0; jj < itemWCOUNT; jj++)
		{
			rc = makeRect(ii, jj);
			rc.InflateRect(1, 1);
			dc.FillSolidRect(rc, RGB(0, 0, 0));
			rc.DeflateRect(2, 2);
			dc.FillSolidRect(rc, (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, ii * itemWCOUNT + jj + m_offsetClr));

			if (usePAL && (int)m_color == ii * itemWCOUNT + jj + m_offsetClr)
			{
				dc.SelectStockObject(BLACK_PEN);
				dc.SelectStockObject(NULL_BRUSH);
				dc.Rectangle(rc);
				rc.DeflateRect(1, 1);
				dc.Rectangle(rc);
			}	
		}
	}

	rc = makeRect();
	if (m_color & userCOLOR)
		dc.FillSolidRect(rc, m_color);

	if (m_user)
		dc.SetTextColor(PALETTERGB(0, 0, 0));
	else
		dc.SetTextColor(PALETTERGB(128, 128, 128));

	dc.DrawEdge(&rc, EDGE_ETCHED, BF_RECT);
	dc.SelectStockObject(SYSTEM_FONT);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(_T("Others..."), &rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);	
}

void CColorBox::saveUserColor(COLORREF* ccolor)
{
	CString	tmps, keys;

	for (int ii = 0; ii < 16; ii++)
	{
		keys.Format(_T("CustomRGB%02d"), ii);
		tmps.Format(_T("%d"), ccolor[ii]);
		WritePrivateProfileString(GENERALSN, keys, tmps, m_path);
	}
}

void CColorBox::loadUserColor(COLORREF* ccolor)
{
	char	wb[64];
	CString	tmps;

	for (int ii = 0; ii < 16; ii++)
	{
		tmps.Format(_T("CustomRGB%02d"), ii);
		GetPrivateProfileString(GENERALSN, tmps, "", wb, sizeof(wb), m_path);
		tmps = wb;
		if (tmps.IsEmpty())
			ccolor[ii] = 0x00ffffff;
		else
			ccolor[ii] = atoi(tmps);
	}
}

void CColorBox::onUser()
{
	CColorDialog    dlg;
	COLORREF	color[16];

	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	if (m_color & userCOLOR)
		dlg.m_cc.rgbResult = RGB(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
	loadUserColor(color);
	dlg.m_cc.lpCustColors = color;

	if (dlg.DoModal() == IDOK)
	{
		COLORREF clr = dlg.GetColor();
		m_color = PALETTERGB(GetRValue(clr), GetGValue(clr), GetBValue(clr));
		Invalidate();
		COLORREF* cclr = dlg.m_cc.lpCustColors;
		saveUserColor(cclr);

		GetParent()->PostMessage(WM_USER + 1, 0, m_color);
	}
}

CRect CColorBox::makeRect(int row, int col)
{
	CRect	cRc;

	if (row == -1 && col == -1)
	{
		cRc.left   = winMARGIN;
		cRc.top    = (itemHEIGHT + winGAP) * itemHCOUNT + winGAP*4;
		cRc.right  = cRc.left + (itemWIDTH + winGAP)*itemWCOUNT - winGAP;
		cRc.bottom = (int)(cRc.top + itemHEIGHT*1.5);
	}
	else
	{
		cRc.left   = ((itemWIDTH  + winGAP) * col) + winMARGIN;
		cRc.top    = ((itemHEIGHT + winGAP) * row) + winMARGIN;
		cRc.right  = cRc.left + itemWIDTH;
		cRc.bottom = cRc.top + itemHEIGHT;
	}

	return cRc;
}