#if !defined(AFX_COMBOINLISTVIEW_H__60972C62_A33C_11D1_9798_002018026B76__INCLUDED_)
#define AFX_COMBOINLISTVIEW_H__60972C62_A33C_11D1_9798_002018026B76__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ComboInListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboInListView window

class AFX_EXT_CLASS CComboInListView : public CComboBox
{
// Construction
public:
	CComboInListView(int iItem, int iSubItem, CStringArray *plstItems, CDWordArray *parrIDs=NULL);	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboInListView)
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboInListView();
private:
	int 	m_iItem;
	int 	m_iSubItem;
	CStringArray m_lstItems;
	CDWordArray	m_arrIDs;
	BOOL	m_bVK_ESCAPE;
	// Generated message map functions
protected:
	//{{AFX_MSG(CComboInListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnCloseup();
	afx_msg void OnSelchange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOINLISTVIEW_H__60972C62_A33C_11D1_9798_002018026B76__INCLUDED_)
