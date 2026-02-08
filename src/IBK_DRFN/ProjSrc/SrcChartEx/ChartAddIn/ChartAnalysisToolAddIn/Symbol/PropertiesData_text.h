// PropertiesData_text.h: interface for the CPropertiesData_text class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTIESDATA_TEXT_H__400EB704_D0C0_405B_9D44_E91CBC19FA93__INCLUDED_)
#define AFX_PROPERTIESDATA_TEXT_H__400EB704_D0C0_405B_9D44_E91CBC19FA93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SymbolBaseData.h"

//도형
class CPropertiesData_Figure  
{
public:
	CPropertiesData_Figure();
	virtual ~CPropertiesData_Figure();

	COLORREF GetColor();
	void SetColor(const COLORREF& color);

	int GetStyle();
	void SetStyle(const int& style);

	int GetWeight();
	void SetWeight(const int& weight);

	bool GetFillState();
	void SetFillState(const bool bFill);

private:
	int m_Style;
	int m_Weight;
	COLORREF m_color;
	bool m_bFill;
};

//기호
class CPropertiesData_Symbol : public CPropertiesData_Figure
{
public:
	CPropertiesData_Symbol();
	virtual ~CPropertiesData_Symbol();

	LOGFONT GetLogFont() const;
	LOGFONT& GetLogFont();
	COLORREF GetTextColor() const;
	CSymbolBaseData::SIZETYPE GetSizeType() const;

	void SetLogFont(const LOGFONT& logfont);
	void SetTextColor(const COLORREF& textColor);
	void SetSizeType(const CSymbolBaseData::SIZETYPE sizetype);

private:
	LOGFONT m_logfont;
	COLORREF m_textColor; //sy 2005.03.30.
	CSymbolBaseData::SIZETYPE m_eSizeType;
};

//text
class CPropertiesData_text : public CPropertiesData_Symbol
{
public:
	CPropertiesData_text();
	virtual ~CPropertiesData_text();

	void SetText(const CString& text);
	CString GetText();

private:
	CString m_text;
};

#endif // !defined(AFX_PROPERTIESDATA_TEXT_H__400EB704_D0C0_405B_9D44_E91CBC19FA93__INCLUDED_)
