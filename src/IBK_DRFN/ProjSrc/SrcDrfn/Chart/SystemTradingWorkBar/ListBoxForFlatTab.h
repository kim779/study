#if !defined(AFX_LISTBOXFORFLATTAB_H__0824BB8A_1071_41A7_861F_E40C3EA31DF9__INCLUDED_)
#define AFX_LISTBOXFORFLATTAB_H__0824BB8A_1071_41A7_861F_E40C3EA31DF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListBoxForFlatTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListBoxForFlatTab window

class CListBoxForFlatTab : public CListBox
{
// Construction
public:
	CListBoxForFlatTab();

// Attributes
public:
	CFont	m_font;
	CMenu	m_mnuPopup;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxForFlatTab)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListBoxForFlatTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxForFlatTab)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClear();
	afx_msg void OnCopytext();
	afx_msg void OnUpdateCopytext(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXFORFLATTAB_H__0824BB8A_1071_41A7_861F_E40C3EA31DF9__INCLUDED_)
