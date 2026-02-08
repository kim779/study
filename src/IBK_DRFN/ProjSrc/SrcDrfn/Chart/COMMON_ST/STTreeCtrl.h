#if !defined(AFX_STTREECTRL_H__60B05D0C_0948_4C5E_9370_11931912D4FB__INCLUDED_)
#define AFX_STTREECTRL_H__60B05D0C_0948_4C5E_9370_11931912D4FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STTreeCtrl.h : header file
//
#include "../Common_ST/LoadSTDir.h"

const UINT RMSG_STTREE_ITEMCLICK	= ::RegisterWindowMessage("RMSG_STTREE_ITEMCLICK");
const UINT RMSG_STTREE_ITEMDBLCLK	= ::RegisterWindowMessage("RMSG_STTREE_ITEMDBLCLK");

#define		SZSTKEY_USERINDEXN		"사용자지표"

/////////////////////////////////////////////////////////////////////////////
// CSTTreeCtrl window

class CSTTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CSTTreeCtrl();

// Attributes
public:
	HTREEITEM m_hSelTreeItem;
	LPCSTR	  m_szRootDir;
	HTREEITEM m_hTreeUser;		// 사용자전략저장폴더
	int		  m_nSTLevel;		// STLevel_Basic, STLevel_Ext;
	CFont	m_Font;

// Operations
public:
	virtual void LoadSTTree()=0;
	virtual void DeleteAllItemData(HTREEITEM hItem);
	virtual void DeleteInfo(STTREEBASE_INFO *pInfo);
	virtual void LoadSTFolderList(HTREEITEM hTreeItem, LPCSTR szDir, STTREEFOLDER_INFO* pFolder, int nType=0);
	//virtual void LoadSTFolderListSTB(HTREEITEM hTreeItem, LPCSTR szDir, STTREEFOLDER_INFO* pFolder);
	virtual HTREEITEM SearchFolderItem(LPCSTR szText);
	virtual HTREEITEM SearchItem(HTREEITEM hStart, LPCSTR szText);

	virtual BOOL DeleteSTItem(LPCSTR szFolder, LPCSTR szItem);
	virtual BOOL AddSTItem(LPCSTR szFolder, LPCSTR szItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTTreeCtrl();
	
	CString GetTrString(CString szText);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd * pParentWnd, UINT nID);

	// Generated message map functions
protected:
	//{{AFX_MSG(CSTTreeCtrl)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSTExtTree window
class CSTExtTree : public CSTTreeCtrl
{
// Construction
public:
	CSTExtTree();

// Attributes
public:

// Operations
public:
	virtual void LoadSTTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTExtTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTExtTree();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CSTExtTree)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSTBasicTree window
class CSTBasicTree : public CSTTreeCtrl
{
// Construction
public:
	CSTBasicTree();

// Attributes
public:

// Operations
public:
	virtual void LoadSTTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTBasicTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTBasicTree();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CSTBasicTree)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTREECTRL_H__60B05D0C_0948_4C5E_9370_11931912D4FB__INCLUDED_)
