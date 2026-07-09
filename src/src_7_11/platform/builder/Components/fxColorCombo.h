#if !defined(AFX_FXCOLORCOMBO_H__FEDD2354_B0F7_4BCE_AF78_E5CFB59E1AD4__INCLUDED_)
#define AFX_FXCOLORCOMBO_H__FEDD2354_B0F7_4BCE_AF78_E5CFB59E1AD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxColorCombo.h : header file
//

#include <afxtempl.h>

#define	userColor	0x02000000

/////////////////////////////////////////////////////////////////////////////
// CfxColorCombo window

class AFX_EXT_CLASS CfxColorCombo : public CComboBox
{
// Construction
public:
	CfxColorCombo();

// Attributes
protected:
	CString	m_path;
	COLORREF m_color;
	CArray	<COLORREF, COLORREF>	m_colors;

// Operations
public:
	COLORREF GetColor();
	void	SetColor(COLORREF clr);
	void	SetPalette(CString path);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxColorCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxColorCombo();

	// Generated message map functions
protected:
	bool	loadColor();
	bool	parseString(CString string, BYTE& rr, BYTE& gg, BYTE& bb);

	//{{AFX_MSG(CfxColorCombo)
	afx_msg void OnSelChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXCOLORCOMBO_H__FEDD2354_B0F7_4BCE_AF78_E5CFB59E1AD4__INCLUDED_)
