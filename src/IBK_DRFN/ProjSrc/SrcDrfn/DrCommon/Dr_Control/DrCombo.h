#if !defined(AFX_DRCOMBO_H__818ADE24_AB0C_477B_B3F4_8F251C25AA90__INCLUDED_)
#define AFX_DRCOMBO_H__818ADE24_AB0C_477B_B3F4_8F251C25AA90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrCombo window

class CDrCombo : public CComboBox
{
// Construction
public:
	CDrCombo();

// Attributes
public:
	BOOL m_bSetFocus;
	BOOL m_bLButtonDown;

// Operations
public:
	void DrawComboBox(CDC* pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrCombo();

	void DrawDownArrow(CDC* pDC, CRect rcArrow);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrCombo)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetfocus();
	afx_msg void OnKillfocus();
	afx_msg void OnCloseup();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRCOMBO_H__818ADE24_AB0C_477B_B3F4_8F251C25AA90__INCLUDED_)
