#include "stdafx.h"
#include "iForm.h"
#include "draw.h"
#include "palette.h"

char*	LCcontrast[] = {"  ", "¡é", "¡å", "¡ã", "¡è"};

CiForm::CiForm(CString root)
{
	CString	path;

	m_root = root;
	m_userID   = _T("");
	m_userName = _T("");
	m_theme    = _T("Blue");	// _T("");

	path.Format(_T("%s/tab/palette.ini"), root);
	m_palette = new CAxPalette(path);
	m_draw = new CAxDraw;

	m_contrast.SetSize(5);
	m_langID = PRIMARYLANGID(GetSystemDefaultLangID());
	switch (m_langID)
	{
	case LANG_CHINESE:
		m_contrast[ct_uLimit]  = (TCHAR)0xa1;
		m_contrast[ct_uLimit] += (TCHAR)0xb6;
		m_contrast[ct_Rise]    = (TCHAR)0xa1;
		m_contrast[ct_Rise]   += (TCHAR)0xb6;
		m_contrast[ct_Fall]    = (TCHAR)0xa1;
		m_contrast[ct_Fall]   += (TCHAR)0xbf;
		m_contrast[ct_lLimit]  = (TCHAR)0xa1;
		m_contrast[ct_lLimit] += (TCHAR)0xbf;
		m_contrast.SetAt(ct_Flat, _T("  "));
		break;
	default:
		m_contrast.SetAt(ct_uLimit, LCcontrast[ct_uLimit]);
		m_contrast.SetAt(ct_Rise, LCcontrast[ct_Rise]);
		m_contrast.SetAt(ct_Fall, LCcontrast[ct_Fall]);
		m_contrast.SetAt(ct_lLimit, LCcontrast[ct_lLimit]);
		m_contrast.SetAt(ct_Flat, LCcontrast[ct_Flat]);
		break;
	}

	m_editMode = false;
}

CiForm::~CiForm(void)
{
	delete m_palette;
	delete m_draw;
}

void CiForm::SetTheme(CString theme)
{
	m_theme = theme;
	m_palette->SetPalette(theme);
}

COLORREF CiForm::GetRGB(int index)
{
	return m_palette->GetPaletteRGB(index);
}

int CiForm::GetPaletteIndex(COLORREF rgb)
{
	return m_palette->GetIndexRGB(rgb);
}

BOOL CiForm::Lookup(int index, COLORREF& rgb)
{
	return m_palette->Lookup(index, rgb);
}

CFont* CiForm::SetFont(CString fontN, int point, bool italic, int bold)
{
	return m_draw->SetFont(fontN, point, italic, bold);
}

CPen* CiForm::SetPen(COLORREF rgb, int width, int style)
{
	return m_draw->SetPen(rgb, width, style);
}

CBrush* CiForm::SetBrush(COLORREF rgb)
{
	return m_draw->SetBrush(rgb);
}

void* CiForm::SetImage(CString fileN)
{
	return m_draw->SetImage(fileN);
}

bool CiForm::IsContrast(int& value)
{
	switch (m_langID)
	{
	case LANG_CHINESE:
		switch (value)
		{
		case 0xa1b6:				// TriUp
		case 0xa1cf:				// '+'
			value = '+';
			return true;
		case 0xa1bf:				// TriDown
		case 0xa1d0:				// '-'
			value = '-';
			return true;
		}
		break;

	case LANG_KOREAN:
	default:
		switch (value)
		{
		case 0xa1e3:				// TriUp
		case 0xa1e8:				// Up
		case 0xa3ab:				// '+'
			value = '+';
			return true;
		case 0xa1e5:				// TriDown
		case 0xa1e9:				// Down
		case 0xa3ad:				// '-'
			value = '-';
			return true;
		}
		break;

	}
	return false;
}

int CiForm::GetContrast(int key, CString& text)
{
	switch (key)
	{
	case '1':
		text = m_contrast[ct_uLimit];
		return ct_uLimit;
	case '+':
	case '2':
		text = m_contrast[ct_Rise];
		return ct_Rise;
	case '4':
		text = m_contrast[ct_lLimit];
		return ct_lLimit;
	case '-':
	case '5':
		text = m_contrast[ct_Fall];
		return ct_Fall;
	}
	text  = _T("  ");
	return ct_Flat;
}

CString CiForm::TrimSign(CString& data)
{
	int	idx;
	CString	signs;

	signs.Empty();

	idx = 0;
	switch (data.FindOneOf("+-"))
	{
	case -1:
		if (data.Find(m_contrast[ct_uLimit]) && data.Find(m_contrast[ct_lLimit]) &&
				data.Find(m_contrast[ct_Rise]) && data.Find(m_contrast[ct_Fall]))
			break;
		signs += data.GetAt(idx++);
	case 0:
		signs += data.GetAt(idx++);
		data = data.Mid(idx);
		break;
	}
	return signs;
}

int CiForm::GetDouble(int value)
{
	if (value < 0x80)
	{
		switch (m_langID)
		{
		default:
		case LANG_KOREAN:
			if (value == ' ')
				return 0xa1a1;
			return (value + 0x80) + 0xa300;
		case LANG_CHINESE:
			break;
		}
	}
	return value;
}

int CiForm::GetSign(CString str, int& offs)
{
	switch (m_langID)
	{
	case LANG_KOREAN:
		offs = str.Find(m_contrast[ct_uLimit]);
		if (offs != -1)
			return ct_uLimit;

		offs = str.Find(m_contrast[ct_lLimit]);
		if (offs != -1)
			return ct_lLimit;

		offs = str.Find(m_contrast[ct_Rise]);
		if (offs != -1)
			return  ct_Rise;

		offs = str.Find(m_contrast[ct_Fall]);
		if (offs != -1)
			return ct_Fall;
		break;

	case LANG_CHINESE:
		offs = str.Find(m_contrast[ct_uLimit]);
		if (offs != -1)
			return ct_Rise;

		offs = str.Find(m_contrast[ct_lLimit]);
		if (offs != -1)
			return ct_Fall;

		offs = str.Find(m_contrast[ct_Rise]);
		if (offs != -1)
			return  ct_Rise;

		offs = str.Find(m_contrast[ct_Fall]);
		if (offs != -1)
			return ct_Fall;
		break;
	}
	return ct_Flat;
}