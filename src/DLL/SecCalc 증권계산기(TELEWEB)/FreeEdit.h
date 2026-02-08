#if !defined(AFX_FREEEDIT_H__0197597A_9255_49FB_AC54_61BE5C2E8B16__INCLUDED_)
#define AFX_FREEEDIT_H__0197597A_9255_49FB_AC54_61BE5C2E8B16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FreeEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFreeEdit window

class CFreeEdit : public CEdit
{
// Construction
public:
	CFreeEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreeEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextColor(COLORREF rgb);
	void SetBackColor(COLORREF rgb);
	virtual ~CFreeEdit();

	// Generated message map functions
protected:

	COLORREF	m_crText;
	COLORREF	m_crBack;
	CBrush		m_brBack;

	//{{AFX_MSG(CFreeEdit)
		afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREEEDIT_H__0197597A_9255_49FB_AC54_61BE5C2E8B16__INCLUDED_)
