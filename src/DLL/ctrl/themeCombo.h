#if !defined(AFX_THEMECOMBO_H__4F8D655F_0B25_4d62_B81B_DC624E368126__INCLUDED_)
#define AFX_THMECOMBO_H__4F8D655F_0B25_4d62_B81B_DC624E368126__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// themeCombo.h : header file
//

#include <afxtempl.h>

#define	userColor	0x02000000

/////////////////////////////////////////////////////////////////////////////
// CthemeCombo window

class CthemeCombo : public CComboBox
{
// Construction
public:
	CthemeCombo();

// Attributes
public:
	CString	m_path;
	COLORREF m_color;
	CArray	<COLORREF, COLORREF>	m_colors;
	CStringArray			m_themes;

	CString	m_strSkinName;

// Operations
public:
	COLORREF GetColor();
	void	SetColor(COLORREF clr);
	void	SetPalette(CString path);
	void	ChangePalette();

protected:
	bool	loadColor();
	bool	parseString(CString string, BYTE& rr, BYTE& gg, BYTE& bb);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CthemeCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CthemeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CthemeCombo)
	//}}AFX_MSG
	afx_msg BOOL OnSelChange();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMECOMBO_H__4F8D655F_0B25_4d62_B81B_DC624E368126__INCLUDED_)
