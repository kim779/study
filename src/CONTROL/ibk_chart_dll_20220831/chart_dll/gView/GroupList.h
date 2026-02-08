#if !defined(AFX_GROUPLIST_H__DD6956F0_BDD0_4B4A_BCC9_777B6861D838__INCLUDED_)
#define AFX_GROUPLIST_H__DD6956F0_BDD0_4B4A_BCC9_777B6861D838__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupList.h : header file
//

#define	GLST_CLASS_NAME	"GroupList"

typedef struct _codeinfo {
	char	code[12 + 1];
}CODEINFO, *PCODEINFO;
#define	sz_Code	sizeof(struct _codeinfo)

#define GROUP_FILE	"gropcd.ini"
#define THEME_FILE	"themcd.ini"

/////////////////////////////////////////////////////////////////////////////
// CGroupList window
class CCodeTreeCtrl : public CTreeCtrl
{
public:
	CCodeTreeCtrl(CWnd *pView, CString rootPath, CString user);
	virtual ~CCodeTreeCtrl();
	
public:
	//{{AFX_VIRTUAL(CCodeTreeCtrl)
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCodeTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()	

	void sortCode();
	void readCode();
	void setTree();	
	CString GetStrToken(int nIndex, CString data, char* token);
	HTREEITEM	AddItem(HTREEITEM hParent, CString input, DWORD nData, HTREEITEM hAt = TVI_LAST);
	CWnd		*m_pView;
	HWND		m_hCallWnd;
	CString		m_rootPath;
	CString		m_userName;
	HTREEITEM	m_itemSELECT;
	int		m_selectKIND;

private:
	template<class T>
	void resetArray(CArray<T, T>& arContent)
	{
		T	pObject = NULL;
		int	ii = 0, nSize = arContent.GetSize();

		for ( ii = 0 ; ii < nSize ; ii++ )
		{
			pObject = arContent.GetAt(ii);
			delete pObject;
			pObject = NULL;
		}

		arContent.RemoveAll();
	}

	DWORD	makeItemData(int nKind, int nSubKind, int nIndex);
	BYTE	getDataKind(DWORD input) { return HIBYTE(HIWORD(input)); }
	BYTE	getDataSubKind(DWORD input) { return LOBYTE(HIWORD(input)); }
	WORD	getDataIndex(DWORD input) { return LOWORD(input); }
	WORD	getDataInfo(int nWhat, DWORD input);
	void	setTreeList(HTREEITEM root, CString kind, CString fileName);
	bool	FillBuffer(HTREEITEM hItem);
	bool	loadHJCode();	
	bool	loadUPCode();

	CArray<struct hjcode*, struct hjcode*>							m_arHJCode;
	CArray<struct upcode*, struct upcode*>			m_arUPCode;
	CArray<struct _codeinfo*, struct _codeinfo*>	m_arBUFFER;
};

class CGroupList : public CWnd
{
// Construction
public:
	CGroupList(CWnd *pView, CWnd* pMain, CFont *pFont, CString rootPath, CString userName);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGroupList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGroupList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	
	void	Create();
	void	SetShow(bool bShow = true);
	void	SelectCode(CString sList, int kind);

private:
	CCodeTreeCtrl*	m_pTree;
	CWnd		*m_pView;
	CWnd		*m_pMain;
	CString		m_rootPath;
	CString		m_userName;
	CFont		*m_pFont;
	bool		m_bActivate;
public:
	CCodeTreeCtrl* GetTreeCtrl() const { return m_pTree; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPLIST_H__DD6956F0_BDD0_4B4A_BCC9_777B6861D838__INCLUDED_)
