#if !defined(AFX_TRSEARCH_H__8AF8D8AB_DB6F_4CF8_B6D6_5272A10101CB__INCLUDED_)
#define AFX_TRSEARCH_H__8AF8D8AB_DB6F_4CF8_B6D6_5272A10101CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrSearch.h : header file
//

#include "../ctrl/ExTreeCtrl.h"
#include "../ctrl/ExListCtrl.h"
#include "../ctrl/ExDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CTrSearch dialog
#include "resource.h"
#include <afxtempl.h>

#define MODE_MENU	1
#define MODE_SEARCH	2

#include "usedefine.h"

class AFX_EXT_CLASS CTrSearch : public CExDialog
{
// Construction
public:
	CTrSearch(class CTMenu* meun, bool (*axiscall)(int, WPARAM, LPARAM) = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrSearch();

	CString	getTR()	{ return m_trcode; };
	void	setPos(CPoint pos) { m_pos = pos; };
// Dialog Data
	//{{AFX_DATA(CTrSearch)
	enum { IDD = IDD_TRSEARCH };
	CExTreeCtrl	m_Tree;
	CExListCtrl	m_List;
	CString		m_Edit;
	CString		m_Result;
	//}}AFX_DATA


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
	afx_msg void OnAfter();
	afx_msg void OnBefore();
	afx_msg void OnSearch();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	class CTMenu*	m_menu;
	
	int		m_sel;
	int		m_cnt;
	int		m_count;
	CPoint		m_pos;
	
	CString		m_trcode;
	CString		m_treeN[MAX];
	HTREEITEM	m_itemN[MAX];
	CStringArray	m_arMap;
	CStringArray	m_arTr;

	CMapStringToString		m_arDesc;
	CArray <HTREEITEM, HTREEITEM>	m_arItem;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
protected:
	void	fit();
	void	setTree();
	void	initList();
	void	make_Tree(CString dat);
	void	setList(BOOL flag = TRUE);
	void	findItem(HTREEITEM item, CString comS);
	void	setCount(int cnt = -1, int tot = -1);
	
	void	treeSelect(int idx);
	void	listSelect(int idx);
	void	enableButton(bool bEnable);

/////////////////////////////////////////////////////////////////////////////
//	 특정사용자에게만 메뉴 오픈
// 2006 11 08
	CString	m_id;
/////////////////////////////////////////////////////////////////////////////
private:
	CBrush	m_ctlBrush;
#ifdef DF_USESTL_TRSEARCH
	std::unique_ptr<CControlChanger>m_changer;
#else
	CControlChanger* m_changer;
#endif
	int m_nMaxLen;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRSEARCH_H__8AF8D8AB_DB6F_4CF8_B6D6_5272A10101CB__INCLUDED_)
