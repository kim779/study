#pragma once
// colorCombo.h : header file
//

#include <afxtempl.h>

#define	userColor	0x02000000

/////////////////////////////////////////////////////////////////////////////
// CcolorCombo window

class CcolorCombo : public CComboBox
{
// Construction
public:
	CcolorCombo();

// Attributes
public:
	CString	m_path;
	COLORREF m_color;
	CArray	<COLORREF, COLORREF>	m_colors;

// Operations
public:
	COLORREF GetColor();
	void	SetColor(COLORREF clr);
	void	SetPalette(CString path);

protected:
	bool	loadColor();
	bool	parseString(CString string, BYTE& rr, BYTE& gg, BYTE& bb);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcolorCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CcolorCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CcolorCombo)
	//}}AFX_MSG
	afx_msg BOOL OnSelChange();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

