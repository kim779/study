// colorCombo.cpp

#include "stdafx.h"
#include "colorCombo.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	maxCOLOR	32
COLORREF defColor[maxCOLOR] = {
	PALETTERGB(  0,   0,   0),	// Windows Default Color
	PALETTERGB(128,   0,   0),
	PALETTERGB(  0, 128,   0),
	PALETTERGB(128, 128,   0),
	PALETTERGB(  0,   0, 128),
	PALETTERGB(128,   0, 128),
	PALETTERGB(  0, 128, 128),
	PALETTERGB(192, 192, 192),
	PALETTERGB(128, 128, 128),
	PALETTERGB(255,   0,   0),
	PALETTERGB(  0, 255,   0),
	PALETTERGB(255, 255,   0),
	PALETTERGB(  0,   0, 255),
	PALETTERGB(255,   0, 255),
	PALETTERGB(  0, 255, 255),
	PALETTERGB(255, 255, 255),

	PALETTERGB(186, 195, 224),	// User Customized Color
	PALETTERGB(230, 236, 248),
	PALETTERGB(  0, 178, 235),
	PALETTERGB( 30,  30, 255),
	PALETTERGB( 28,  97, 174),
	PALETTERGB( 20,  65, 174),
	PALETTERGB(  0,   0, 128),
	PALETTERGB(128,   0, 128),
	PALETTERGB(146,   0, 123),
	PALETTERGB(128,   0,   0),
	PALETTERGB(116,  74,   0),
	PALETTERGB(128, 128,   0),
	PALETTERGB( 38,   7,  81),
	PALETTERGB(192, 192, 192),
	PALETTERGB(255, 255, 255),
	PALETTERGB(  0,   0,   0),
};

// CColorCombo

CColorCombo::CColorCombo()
{
	m_colors.RemoveAll();
	for (int ii = 0; ii < maxCOLOR; ii++)
		m_colors.Add(defColor[ii]);

	m_colors.Add(::GetSysColor(COLOR_WINDOW));	// Others
	m_color = 0;
	m_path  = _T("");
}

CColorCombo::~CColorCombo()
{
	m_colors.RemoveAll();
}


BEGIN_MESSAGE_MAP(CColorCombo, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCombo message handlers

void CColorCombo::PreSubclassWindow() 
{
	for (int ii = 0; ii < m_colors.GetSize(); ii++)
		AddString(_T(""));
	
	SetCurSel(0);
	m_color = 0;
	CComboBox::PreSubclassWindow();
}

void CColorCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC	dc;
	CRect	rect;
	CBrush	brush;

	dc.Attach(lpDrawItemStruct->hDC);
	rect.CopyRect(&lpDrawItemStruct->rcItem);
	rect.DeflateRect(2, 2);
	dc.FillSolidRect(rect, m_colors.GetAt(lpDrawItemStruct->itemID));
	
	switch (lpDrawItemStruct->itemID)
	{
	case maxCOLOR:	// The automatic case
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		dc.DrawText(_T("Others.."), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

	default:
		break;
	}
	
	brush.CreateSolidBrush(RGB(0, 0, 0));
	dc.FrameRect(rect, &brush);
	brush.DeleteObject();
	rect.InflateRect(1, 1);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		dc.DrawFocusRect(rect);

	dc.Detach();
}

void CColorCombo::OnSelChange() 
{
	int	sel = GetCurSel();
	if (sel == CB_ERR || sel >= m_colors.GetUpperBound())
	{
		CColorDialog dlg;
		
		dlg.m_cc.Flags |= CC_RGBINIT|CC_FULLOPEN;
		if (m_color & userColor)
			dlg.m_cc.rgbResult = RGB(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
		if (dlg.DoModal() == IDOK)
		{
			m_color = dlg.GetColor();
			m_color = PALETTERGB(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
		}
	}
	else
		m_color = sel;
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetSafeHwnd());
}

COLORREF CColorCombo::GetColor()
{
	if ((int)m_color < m_colors.GetUpperBound())
		return m_colors.GetAt(m_color);
	else
		return m_colors.GetAt(m_colors.GetUpperBound());
}

void CColorCombo::SetColor(COLORREF clr)
{
	bool	find = false;

	if (clr & userColor)
	{
		for (int ii = 0; ii < m_colors.GetUpperBound(); ii++)
		{
			if (m_colors.GetAt(ii) == clr)
			{
				m_color = ii;
				find    = true;
				break;
			}
		}
	}
	else if ((int)clr < m_colors.GetUpperBound())
	{
		m_color = clr;
		find    = true;
	}

	if (!find)
	{
		m_color = (int)m_colors.GetUpperBound();
		m_colors.SetAt(m_color, clr);
	}

	SetCurSel(m_color);
}

void CColorCombo::SetPalette(CString path)
{
	m_path.Format(_T("%s\\%s\\%s"), path, TABDIR, PALETTE);
	m_colors.RemoveAll();
	if (!loadColor())
	{
		m_colors.RemoveAll();
		for (int ii = 0; ii < maxCOLOR; ii++)
			m_colors.Add(defColor[ii]);
	}
	m_colors.Add(::GetSysColor(COLOR_WINDOW));
}

bool CColorCombo::loadColor()
{
	char	wb[128];
	DWORD	rc;
	CString	tmps, section;
	
	rc   = GetPrivateProfileString(GENERALSN, _T("Palette"), _T(""), wb, sizeof(wb), m_path);
	if (!rc || !strlen(wb))
		return false;

	section = wb;
	section.TrimLeft();
	section.TrimRight();
	if (section.IsEmpty())
		return false;

	BYTE	rr, gg, bb;
	for (int ii = 0; ii < maxCOLOR; ii++)
	{
		tmps.Format(_T("RGB%03d"), ii);
		rc = GetPrivateProfileString(section, tmps, _T(""), wb, sizeof(wb), m_path);
		if (!rc || !strlen(wb))
			return false;

		tmps = wb;
		if (!parseString(tmps, rr, gg, bb))
			return false;

		m_colors.Add(PALETTERGB(rr, gg, bb));
	}

	return true;
}

bool CColorCombo::parseString(CString string, BYTE& rr, BYTE& gg, BYTE& bb)
{
	int	pos;
	CString	tmps;

	for (int ii = 0; ii < 3; ii++)
	{
		string.TrimLeft();
		string.TrimRight();
		if (string.IsEmpty())
			return false;

		pos = string.Find(_T(','));
		if (pos > 0)
		{
			tmps   = string.Left(pos);
			string = string.Mid(pos+1);
		}
		else
		{
			tmps   = string;
			string = _T("");
		}

		tmps.TrimRight();
		switch (ii)
		{
		case 0:
			rr = atoi(tmps);	break;
		case 1:
			gg = atoi(tmps);	break;
		case 2:
			bb = atoi(tmps);	break;
		}
	}

	return true;
}
