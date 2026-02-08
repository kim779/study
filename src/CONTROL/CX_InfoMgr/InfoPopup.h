#pragma once
// InfoPopup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoPopup window

class CInfoPopup : public CWnd
{
// Construction
public:
	CInfoPopup(CWnd *pWnd);
	BOOL	addColumn(CListCtrl *list, LPCTSTR columns, int col, int width);
	BOOL	addItem(CListCtrl *list, CString items, int item, int subi = 0);

// Attributes
public:
	CWnd*	m_pParent;
	std::unique_ptr<CListCtrl> m_pInfoList;
	bool	m_fDone;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInfoPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInfoPopup)
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};