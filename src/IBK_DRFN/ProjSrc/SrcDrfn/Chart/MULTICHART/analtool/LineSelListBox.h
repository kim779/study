#if !defined(AFX_LINESELLISTBOX_H__8ABF31B0_A4D4_46A1_B2C4_95DB69B58021__INCLUDED_)
#define AFX_LINESELLISTBOX_H__8ABF31B0_A4D4_46A1_B2C4_95DB69B58021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineSelListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineSelListBox window

class CLineSelListBox : public CListBox
{
// Construction
public:
	CLineSelListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineSelListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpdis);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpmis);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLineSelListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLineSelListBox)
	afx_msg void OnSelchange();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINESELLISTBOX_H__8ABF31B0_A4D4_46A1_B2C4_95DB69B58021__INCLUDED_)
