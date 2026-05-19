#pragma once

// colorCombo.h : header file

#define	userColor	0x02000000


// CColorCombo window

class CColorCombo : public CComboBox
{
public:
	CColorCombo();
	virtual ~CColorCombo();

protected:
	CString	m_path;
	COLORREF m_color;
	CArray	<COLORREF, COLORREF>	m_colors;


public:
	COLORREF GetColor();
	void	SetColor(COLORREF clr);
	void	SetPalette(CString path);

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	virtual void PreSubclassWindow();
	bool	loadColor();
	bool	parseString(CString string, BYTE& rr, BYTE& gg, BYTE& bb);

	afx_msg void OnSelChange();

	DECLARE_MESSAGE_MAP()
};
