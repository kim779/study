#if !defined(AFX_ITEMCOMBO_H__834127E7_9297_11D5_8AFA_DAC6E5A1C54B__INCLUDED_)
#define AFX_ITEMCOMBO_H__834127E7_9297_11D5_8AFA_DAC6E5A1C54B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemCombo window

class CItemCombo : public CComboBox
{
// Construction
public:
	CItemCombo( int nItem, int nSubItem, CStringList *psList = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnDropdownComboGroup();
	virtual ~CItemCombo();

	// Generated message map functions
protected:
	CStringList m_sList;
	int m_nSubItem;
	int m_nItem;
	BOOL m_bVK_ESCAPE;
	//{{AFX_MSG(CItemCombo)
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCloseup();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMCOMBO_H__834127E7_9297_11D5_8AFA_DAC6E5A1C54B__INCLUDED_)
