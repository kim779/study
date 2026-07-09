#if !defined(__MYBAR_H__)
#define __MYBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// mybar.h : header file
//
#include "../ChatTree.h"     
#include "NFBtn.h"
#include "ToolTipListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMyBar window

typedef struct stUser {
	CString strgubn;
	CString strName;
} _stUser;

class CMyBar : public CSizingControlBarG
{
// Construction
public:
	CMyBar();

// Attributes
public:
	void		initialize();
	void		CheckTreeList();
	BOOL	addColumn(CListCtrl *LC, LPCTSTR str, int item, int width);
	void		AddItem(CListCtrl *LC, int item, stUser* pdata);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBar();

	CChatTree		m_TreeChat;
	CToolTipListCtrl	m_List_user;
	//CListCtrl	m_List_user;
	
protected:
	CEdit	m_wndChild;
	CFont		m_font, m_tabFont;
	CString		m_path;
	CImageList*	m_pDragImg, m_tabImg;

	CNFBtn		m_treeBtn;
	//CNFBtn		m_listBtn;
	CNFBtn		m_ReloadBtn;

	HBITMAP		m_hTreeBitmap;
	HBITMAP		m_hListBitmap;
	HBITMAP		m_hReloadBitmap;

	TCHAR* m_pchTip ;
	WCHAR* m_pwchTip;
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyBar)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTreeBtnClick();
	afx_msg void OnListDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__MYBAR_H__)
