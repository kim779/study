#pragma once
// FindEdit.h : header file
//

#include "Common.h"
#include "ListPopup.h"

class CFindEdit : public CEdit
{
// Construction
public:
	CFindEdit();

// Attributes
public:
	void SetFont(CFont *pFont);
	void SetCodeList(CStringArray *pCodeArray, CStringArray *pNameArray);

// Operations
protected:
	CString m_text;
	BOOL m_bImeNoInsert, m_bImeNoRemove, m_bComposition;
	std::unique_ptr <CListPopup> m_pList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindEdit)
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFindEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFindEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

