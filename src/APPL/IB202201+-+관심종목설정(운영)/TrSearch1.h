#pragma once
// TrSearch1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrSearch dialog
#define  MAX 5
#include "resource.h"
#include <afxtempl.h>

#define MODE_MENU	1
#define MODE_SEARCH	2

class CPage2;

class CTrSearch : public CDialog
{
// Construction
public:
	CTrSearch(CWnd* pParent = nullptr, CWnd* mainWnd = nullptr);   // standard constructor
	~CTrSearch();

	void	Init();
	void	loadTRInfo();
	void	make_Tree(CString dat);
	CString	getTR()	{ return m_trcode; };
	CString	getName()	{ return m_trname; };
	void	setPos(CPoint pos) { m_pos = pos; };
	void	initList();
	void	setList(BOOL flag = TRUE);
	void	findItem(HTREEITEM item, CString comS);
	void	setCount(int cnt = -1, int tot = -1);
	void	fit();
	void	treeSelect(int idx);
	void	listSelect(int idx);
	void	enableButton(bool bEnable);
	BOOL	AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width);
	void	AppendItemToList(int ipos, CString code, CString name);
	int		AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem = -1);

	CString	Variant(int comm, CString data = "");
	CString Parser(CString &srcstr, CString substr);
	int		GetUnitInList(CMapStringToString& mapList, CString dat, CString deli = ",", CString start = "", CString end = "");
// Dialog Data
	//{{AFX_DATA(CTrSearch)
	enum { IDD = IDD_TRSEARCH };
	CListCtrl	m_List;
	CTreeCtrl	m_Tree;
	CString		m_Edit;
	CString		m_Result;
	//}}AFX_DATA	
	

	CWnd*			m_pMainWnd;
	CPage2*			m_pParent;

	CString			m_root;
	CString			m_name;
	CString			m_user;

	int		m_sel;
	int		m_cnt;
	int		m_count;
	CPoint		m_pos;
	
	CString		m_trcode;
	CString		m_trname;
	CString		m_treeN[MAX];
	HTREEITEM	m_itemN[MAX];
	CStringArray	m_arMap;
	CStringArray	m_arTr;
	
	CString	m_id;

	CMapStringToString		m_arDesc;
	CArray <HTREEITEM, HTREEITEM>	m_arItem;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrSearch)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnAfter();
	afx_msg void OnBefore();
	afx_msg void OnSearch();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

