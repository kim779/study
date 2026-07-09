// themeCombo.cpp : implementation file
//

#include "stdafx.h"
#include "themeCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	maxCOLOR	7
COLORREF themeColor[maxCOLOR] = {		
	// 0 ~ 15 : Windows Default Color
	PALETTERGB( 83, 123, 195),
	PALETTERGB(104, 171, 170),
	PALETTERGB(123, 161, 113),
	PALETTERGB(140, 138,  92),
	PALETTERGB(144, 103,  98),
	PALETTERGB(146, 122, 164),
	PALETTERGB(158, 172, 198),
};

CString	theme[maxCOLOR] = { "Blue", "Mint", "Green", "Brown", "Garnet", "Violet", "BlueGray" };

/////////////////////////////////////////////////////////////////////////////
// CthemeCombo

CthemeCombo::CthemeCombo()
{
	m_colors.RemoveAll();
	for (int ii = 0; ii < maxCOLOR; ii++)
	{
		m_colors.Add(themeColor[ii]);
		m_themes.Add(theme[ii]);
	}

	m_color = 0;
	m_path  = _T("");
}

CthemeCombo::~CthemeCombo()
{
	m_colors.RemoveAll();
}


BEGIN_MESSAGE_MAP(CthemeCombo, CComboBox)
	//{{AFX_MSG_MAP(CthemeCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CthemeCombo message handlers

void CthemeCombo::PreSubclassWindow() 
{
	for (int ii = 0; ii < m_colors.GetSize(); ii++)
		AddString(_T(""));

	SetCurSel(0);
	m_color = 0;
	CComboBox::PreSubclassWindow();
}

void CthemeCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC	dc;
	CRect	rect, tRc, tRc2;
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
	{
		tRc.SetRect(rect.left, rect.top, rect.left + rect.Width() / 2, rect.bottom);
		dc.FillSolidRect(tRc, m_colors.GetAt(lpDrawItemStruct->itemID));

		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

		tRc2.SetRect(tRc.right + 5, tRc.top, rect.right, tRc.bottom);
		dc.DrawText(m_themes.GetAt(lpDrawItemStruct->itemID), tRc2, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	}
	
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DSHADOW));
	else
		brush.CreateSolidBrush(RGB(0, 0, 0));
	
	dc.FrameRect(tRc, &brush);
	brush.DeleteObject();
	tRc.DeflateRect(1, 1);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		dc.DrawFocusRect(tRc);

	dc.Detach();
}

BOOL CthemeCombo::OnSelChange()
{
	int	sel = GetCurSel();

	m_color = sel;

	return FALSE;
}

COLORREF CthemeCombo::GetColor()
{
	if (!(m_color & userColor) && (int)m_color >= 0 && (int)m_color < m_colors.GetUpperBound())
		return m_colors.GetAt(m_color);
	else
		return m_colors.GetAt(m_colors.GetUpperBound());
}

void CthemeCombo::SetColor(COLORREF clr)
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
		m_color = (COLORREF)m_colors.GetUpperBound();
		m_colors.SetAt(m_color, clr);
	}

	SetCurSel(m_color);
}

void CthemeCombo::SetPalette(CString path)
{
	m_path.Format("%s\\%s\\%s", path, "tab", "palette.ini");
	m_colors.RemoveAll();
	if (!loadColor())
	{
		m_colors.RemoveAll();
		for (int ii = 0; ii < maxCOLOR; ii++)
			m_colors.Add(themeColor[ii]);
	}
}

bool CthemeCombo::loadColor()
{
#if 0
	char	wb[128];
	DWORD	rc;
	CString	tmps, section;
	
	rc   = GetPrivateProfileString("General", "Palette", "", wb, sizeof(wb), m_path);
	if (!rc || !strlen(wb))
		return false;

	section = wb; section.TrimLeft(); section.TrimRight();
	if (section.IsEmpty())
		return false;

	BYTE	rr, gg, bb;
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
#else
	int	pos;
	char	buffer[128];
	CString	skin, tmpS, dat;

	m_themes.RemoveAll();
	DWORD dw = GetPrivateProfileString("General", "Palette", "", buffer, sizeof(buffer), m_path);
	if (dw <= 0)	return false;
	skin = buffer;		skin.TrimLeft();	skin.TrimRight();
	dw = GetPrivateProfileString("General", "Palettes", "", buffer, sizeof(buffer), m_path);
	if (dw <= 0)	return false;

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			dat.TrimLeft();		dat.TrimRight();
			m_themes.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		tmpS.TrimLeft();	tmpS.TrimRight();
		m_themes.Add(tmpS);
	}

	BYTE	rr, gg, bb;
	for (int ii = 0; ii < m_themes.GetSize() && ii < maxCOLOR; ii++)
	{
		dat = m_themes.GetAt(ii);
		if (dat == skin)
			SetCurSel(ii);

		DWORD dw = GetPrivateProfileString(dat, "RGB120", "", buffer, sizeof(buffer), m_path);
		if (dw <= 0)	return false;
	
		tmpS = buffer;
		if (!parseString(tmpS, rr, gg, bb))
			return false;

		m_colors.Add(PALETTERGB(rr, gg, bb));
	}
#endif
	return true;
}

bool CthemeCombo::parseString(CString string, BYTE& rr, BYTE& gg, BYTE& bb)
{
	int	pos;
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

void CthemeCombo::ChangePalette()
{
	CString skinN = m_themes.GetAt(GetCurSel());
	
	WritePrivateProfileString("GENERAL", "Palette", (char *)skinN.operator LPCTSTR(), m_path);
}