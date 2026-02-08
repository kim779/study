// PropertiesData_text.cpp: implementation of the CPropertiesData_text class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "PropertiesData_text.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPropertiesData_Figure::CPropertiesData_Figure()
{
	SetColor(RGB(255,255,0));
	SetStyle(PS_SOLID);
	SetWeight(0);
	m_bFill = true;
}

CPropertiesData_Figure::~CPropertiesData_Figure()
{

}

COLORREF CPropertiesData_Figure::GetColor()
{
	return m_color;
}

void CPropertiesData_Figure::SetColor(const COLORREF& color)
{
	m_color = color;
}

int CPropertiesData_Figure::GetStyle()
{
	return m_Style;
}

void CPropertiesData_Figure::SetStyle(const int& style)
{
	m_Style = style;
}

int CPropertiesData_Figure::GetWeight()
{
	return m_Weight;
}

void CPropertiesData_Figure::SetWeight(const int& weight)
{
	m_Weight = weight;
}

//±‚»£
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropertiesData_Symbol::CPropertiesData_Symbol()
{
	///////////////
	memset(&m_logfont, 0, sizeof(LOGFONT));       // zero out structure
	lstrcpy(m_logfont.lfFaceName, "±º∏≤"); //"±º∏≤"
	//m_logfont.lfWidth = 7;
	//m_logfont.lfHeight = 16;
	m_logfont.lfWidth = 10;
	m_logfont.lfHeight = 20;
	m_logfont.lfWeight = FW_BOLD;
	m_logfont.lfItalic = FALSE;
	m_logfont.lfUnderline = FALSE;
	m_logfont.lfStrikeOut = FALSE;

	m_logfont.lfPitchAndFamily = FIXED_PITCH;
	m_logfont.lfQuality = DEFAULT_QUALITY;
	m_logfont.lfCharSet = HANGUL_CHARSET;
	m_logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	/////////////////

	m_textColor = RGB(0, 0, 0);
	m_eSizeType = CSymbolBaseData::Small;
}

CPropertiesData_Symbol::~CPropertiesData_Symbol()
{

}

LOGFONT& CPropertiesData_Symbol::GetLogFont()
{
	return m_logfont;
}

LOGFONT CPropertiesData_Symbol::GetLogFont() const
{
	return m_logfont;
}

CSymbolBaseData::SIZETYPE CPropertiesData_Symbol::GetSizeType() const
{
	return m_eSizeType;
}

COLORREF CPropertiesData_Symbol::GetTextColor() const
{
	return m_textColor;
}

void CPropertiesData_Symbol::SetLogFont(const LOGFONT& logfont)
{
	m_logfont = logfont;
}

void CPropertiesData_Symbol::SetTextColor(const COLORREF& textColor)
{
	m_textColor = textColor;
}

void CPropertiesData_Symbol::SetSizeType(const CSymbolBaseData::SIZETYPE sizetype)
{
	m_eSizeType = sizetype;
}


//text
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropertiesData_text::CPropertiesData_text()
{
	m_text = "";
}

CPropertiesData_text::~CPropertiesData_text()
{

}

void CPropertiesData_text::SetText(const CString& text)
{
	m_text = text;
}

CString CPropertiesData_text::GetText()
{
	return m_text;
}

void CPropertiesData_Figure::SetFillState(const bool bFill)
{
	m_bFill = bFill;
}

bool CPropertiesData_Figure::GetFillState()
{
	return m_bFill;
}
