#if !defined(AFX_MENUSEARCH_H__1B1F8C2E_8F39_4EC5_BCFE_E5D1E274D38E__INCLUDED_)
#define AFX_MENUSEARCH_H__1B1F8C2E_8F39_4EC5_BCFE_E5D1E274D38E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuSearch.h : header file
//

#include "../ctrl/ExTreeCtrl.h"
#include "../ctrl/ExListCtrl.h"
#include "../ctrl/ExDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CMenuSearch dialog
#include "resource.h"
#include <afxtempl.h>

#define MODE_MENU	1
#define MODE_SEARCH	2

class CMenuSearch : public CExDialog
{
// Construction
public:
	CMenuSearch(class CTMenu* meun, CString home, bool (*axiscall)(int, WPARAM, LPARAM) = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMenuSearch();

	CString	getTR()	{ return m_trcode; };
	void	setPos(CPoint pos) { m_pos = pos; };
// Dialog Data
	//{{AFX_DATA(CMenuSearch)
	enum { IDD = IDD_MENUSEARCH };
	CStatic		m_Text;
	CExTreeCtrl	m_Tree;
	CExListCtrl	m_List;
	CString		m_Edit;
	CString		m_Result;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuSearch)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMenuSearch)
	afx_msg void OnAfter();
	afx_msg void OnBefore();
	afx_msg void OnSearch();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	class CTMenu*	m_menu;
	
	int		m_sel;
	int		m_cnt;
	int		m_count;
	CPoint		m_pos;
	
	CString		m_home;
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUSEARCH_H__1B1F8C2E_8F39_4EC5_BCFE_E5D1E274D38E__INCLUDED_)
