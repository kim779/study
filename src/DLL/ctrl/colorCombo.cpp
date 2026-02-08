// colorCombo.cpp : implementation file
//

#include "stdafx.h"
#include "colorCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	maxCOLOR	32
COLORREF defColor[maxCOLOR] = {		
	// 0 ~ 15 : Windows Default Color
	PALETTERGB(  0,   0,   0),	//  0. black
	PALETTERGB(128,   0,   0),	//  1. dark red
	PALETTERGB(  0, 128,   0),	//  2. dark green
	PALETTERGB(128, 128,   0),	//  3. dark yellow
	PALETTERGB(  0,   0, 128),	//  4. dark blue
	PALETTERGB(128,   0, 128),	//  5. dark magenta
	PALETTERGB(  0, 128, 128),	//  6. dark cyan
	PALETTERGB(192, 192, 192),	//  7. light gray
	PALETTERGB(128, 128, 128),	//  8. medium gray
	PALETTERGB(255,   0,   0),	//  9. red
	PALETTERGB(  0, 255,   0),	// 10. green
	PALETTERGB(255, 255,   0),	// 11. yellow
	PALETTERGB(  0,   0, 255),	// 12. blue
	PALETTERGB(255,   0, 255),	// 13. magenta
	PALETTERGB(  0, 255, 255),	// 14. cyan
	PALETTERGB(255, 255, 255),	// 15. white

	// 16 ~ 31 : User Customized Color
	PALETTERGB( 70,  70,  70),
	PALETTERGB( 50,  50,  50),
	PALETTERGB(130, 130, 130),
	PALETTERGB(140, 140, 140),
	PALETTERGB( 30,  30,  30),
	PALETTERGB(200, 200, 200),
	PALETTERGB(  0,   0, 100),
	PALETTERGB(110, 110, 110),
	PALETTERGB( 37, 127,  19),
	PALETTERGB(244,   0,   0),
	PALETTERGB(216, 208, 194),
	PALETTERGB(206, 196, 176),
	PALETTERGB(202, 214, 228),
	PALETTERGB(152, 171, 193),
	PALETTERGB(235, 239, 245),
	PALETTERGB(244, 239, 255),	// pane background color
};

/////////////////////////////////////////////////////////////////////////////
// CcolorCombo

CcolorCombo::CcolorCombo()
{
	m_colors.RemoveAll();
	for (int ii = 0; ii < maxCOLOR; ii++)
		m_colors.Add(defColor[ii]);

	m_colors.Add(::GetSysColor(COLOR_WINDOW));	// Others
	m_color = 0;
	m_path  = _T("");
}

CcolorCombo::~CcolorCombo()
{
	m_colors.RemoveAll();
}


BEGIN_MESSAGE_MAP(CcolorCombo, CComboBox)
	//{{AFX_MSG_MAP(CcolorCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcolorCombo message handlers

void CcolorCombo::PreSubclassWindow() 
{
	for (int ii = 0; ii < m_colors.GetSize(); ii++)
		AddString(_T(""));

	SetCurSel(0);
	m_color = 0;
	CComboBox::PreSubclassWindow();
}

void CcolorCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC	dc;
	CRect	rect;
	CBrush	brush;

	dc.Attach(lpDrawItemStruct->hDC);
	rect.CopyRect(&lpDrawItemStruct->rcItem);
	rect.DeflateRect(1, 1);
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		rect.InflateRect(1, 1);
		dc.FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));
		rect.DeflateRect(1, 1);
	}
	else
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
	
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DSHADOW));
	else
		brush.CreateSolidBrush(RGB(0, 0, 0));
	dc.FrameRect(rect, &brush);
	brush.DeleteObject();
	rect.DeflateRect(1, 1);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		dc.DrawFocusRect(rect);

	dc.Detach();
}

BOOL CcolorCombo::OnSelChange()
{
	const int	sel = GetCurSel();
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
			m_colors.SetAt(m_colors.GetUpperBound(), m_color);
		}
	}
	else
		m_color = sel;

	return FALSE;
}

COLORREF CcolorCombo::GetColor()
{
	if (!(m_color & userColor) && (int)m_color >= 0 && (int)m_color < m_colors.GetUpperBound())
		return m_colors.GetAt(m_color);
	else
		return m_colors.GetAt(m_colors.GetUpperBound());
}

void CcolorCombo::SetColor(COLORREF clr)
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
		m_color = m_colors.GetUpperBound();
		m_colors.SetAt(m_color, clr);
	}

	SetCurSel(m_color);
}

void CcolorCombo::SetPalette(CString path)
{
	m_path.Format("%s\\%s\\%s", path, "tab", "palette.ini");
	m_colors.RemoveAll();
	if (!loadColor())
	{
		m_colors.RemoveAll();
		for (int ii = 0; ii < maxCOLOR; ii++)
			m_colors.Add(defColor[ii]);
	}
	m_colors.Add(::GetSysColor(COLOR_WINDOW));
}

bool CcolorCombo::loadColor()
{
	char	wb[128];
	DWORD	rc;
	CString	tmps, section;
	
	rc   = GetPrivateProfileString("General", "Palette", "", wb, sizeof(wb), m_path);
	if (!rc || !strlen(wb))
		return false;

	section = wb; section.TrimLeft(); section.TrimRight();
	if (section.IsEmpty())
		return false;

	BYTE	 rr{}, gg{}, bb{};
	for (int ii = 0; ii < maxCOLOR; ii++)
	{
		tmps.Format("RGB%03d", ii);
		rc = GetPrivateProfileString(section, tmps, "", wb, sizeof(wb), m_path);
		if (!rc || !strlen(wb))
			return false;

		tmps = wb;
		if (!parseString(tmps, rr, gg, bb))
			return false;

		m_colors.Add(PALETTERGB(rr, gg, bb));
	}

	return true;
}

bool CcolorCombo::parseString(CString string, BYTE& rr, BYTE& gg, BYTE& bb)
{
	int	pos{};
	CString	tmps;

	for (int ii = 0; ii < 3; ii++)
	{
		string.TrimLeft();
		string.TrimRight();
		if (string.IsEmpty())
			return false;

		pos = string.Find(',');
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


