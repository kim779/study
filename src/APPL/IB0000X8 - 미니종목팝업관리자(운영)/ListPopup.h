#pragma once
// ListPopup.h : header file
//

#include "ListBoxEx.h"

class CListPopup : public CWnd
{
// Construction
public:
	CListPopup(CEdit *pEdit);
	void SetCodeList(CStringArray *pCodeArray, CStringArray *pNameArray);
	void SelectKey(UINT nChar);
	void Select(UINT index);
	void GetSelection(CString &code, CString &name);

// Attributes
public:
	CEdit *m_pEdit;
	CListBoxEx m_listbox;

// Operations
public:
	void SetFont(CFont *pFont);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListPopup)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnListBoxKillFocus(WPARAM wParam, LPARAM lParam);
};

