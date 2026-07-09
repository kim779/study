#if !defined(AFX_HOTKEY_H__7D36B052_A145_44CF_B46C_BCB36308A3C3__INCLUDED_)
#define AFX_HOTKEY_H__7D36B052_A145_44CF_B46C_BCB36308A3C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Hotkey.h : header file
//
#include "../ctrl/ExTreeCtrl.h"
#include "../ctrl/ExListCtrl.h"
#include "../ctrl/ExDialog.h"

#include "resource.h"
#include <afxtempl.h>


#define MODE_MENU	1
#define MODE_SEARCH	2

/////////////////////////////////////////////////////////////////////////////
// CHotkey dialog

class AFX_EXT_CLASS CHotkey : public CExDialog
{
// Construction
public:
	CHotkey(class CTMenu* menu, CString home, CString user, CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotkey();

	CString	getTR()	{ return m_trcode; };
	void	setPos(CPoint pos) { m_pos = pos; };
	void	ApplySetup();
	void	DefaultSetup();

// Dialog Data
	//{{AFX_DATA(CHotkey)
	enum { IDD = IDD_HOTKEY };
	CExListCtrl	m_keyList;
	CExTreeCtrl	m_Tree;
	CExListCtrl	m_List;
	CString		m_Edit;
	CString		m_Result;
	CString		m_aEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotkey)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHotkey)
	virtual BOOL OnInitDialog();
	afx_msg void OnAfter();
	afx_msg void OnBefore();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearch();
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearchAll();
	afx_msg void OnViewAll();
	afx_msg void OnViewMenu();
	afx_msg void OnClickKey(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkKey(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	class CTMenu*	m_menu;
	CString		m_home;
	CString		m_user;
	CString		m_trcode;

	int	m_sel;
	int	m_cnt;
	int	m_mode;
	int	m_count;
	int	m_kIndex;
	UINT	m_nRes;
	CPoint	m_pos;

	CString		m_treeN[MAX];
	HTREEITEM	m_itemN[MAX];
	CStringArray	m_arMap;
	CStringArray	m_arTr;
	CArray	<int, int>	m_hlist;
	CMapStringToString	m_arDesc;
	CArray <HTREEITEM, HTREEITEM>	m_arItem;
protected:
	void	fit();
	void	initList();
	void	initkeyList();
	void	setList(BOOL flag = TRUE);
	void	setTree();
	void	make_Tree(CString dat);
	void	loadKey();

	void	findItem(HTREEITEM item, CString comS);
	void	setCount(int cnt = -1, int tot = -1);
	void	changeState(int index);
	void	writeKey();
	
	void	treeSelect(int idx);
	void	enableButton(bool bEnable);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTKEY_H__7D36B052_A145_44CF_B46C_BCB36308A3C3__INCLUDED_)
