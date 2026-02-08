#if !defined(AFX_POPUPMENU_H__7D1D9C59_D986_4DFB_8AEC_7D5B9F3FA2A3__INCLUDED_)
#define AFX_POPUPMENU_H__7D1D9C59_D986_4DFB_8AEC_7D5B9F3FA2A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PopUpMenu.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CPopUpMenu window
class CPopUpMenu : public CMenu
{
// Construction
public:
	CPopUpMenu();
	void	AddBmpCool(int nCool);
	void	AddBmpHot(int nHot);
	void	SetBmpTool(BOOL bBitMap)	{ m_bBitMap = bBitMap;	}
// Attributes
public:

protected:
	BOOL m_bBitMap;
	CArray<int, int>	m_CoolBmp;
	CArray<int, int>	m_HotBmp;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopUpMenu)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPopUpMenu();

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPMENU_H__7D1D9C59_D986_4DFB_8AEC_7D5B9F3FA2A3__INCLUDED_)
