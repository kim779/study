#if !defined(AFX_REGMAP_H__64B3215B_1B9F_4637_A985_FA60952B8E57__INCLUDED_)
#define AFX_REGMAP_H__64B3215B_1B9F_4637_A985_FA60952B8E57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegMap.h : header file
//
#include "../ctrl/ExTreeCtrl.h"
#include "../ctrl/ExListCtrl.h"
#include "../ctrl/ExDialog.h"
#include "../ctrl/ExCombo.h"
/////////////////////////////////////////////////////////////////////////////
// CRegMap dialog
#include "resource.h"
#include <afxtempl.h>

#define MODE_MENU	1
#define MODE_SEARCH	2
#define	SCREENREG	"screenreg.ini"
#define	CODEREG		"codereg"
/////////////////////////////////////////////////////////////////////////////
// CRegMap dialog

#include "RegMap.h"
class CRegMap : public CExDialog
{
// Construction
public:
	CRegMap(class CTMenu* menu, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegMap();

//	CString	getTR()	{ return m_trcode; };
// Dialog Data
	//{{AFX_DATA(CRegMap)
	enum { IDD = IDD_REGISTER_MAP };
	CExListCtrl	m_gList;
	CExTreeCtrl	m_Tree;
	CExListCtrl	m_List;
	CString		m_Edit;
	CString		m_Result;
	CString		m_editN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegMap)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegMap)
	afx_msg void OnAfter();
	afx_msg void OnBefore();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearch();
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnAddTR();
	afx_msg void OnDelTR();
	afx_msg void OnDblclkGlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelall();
	afx_msg void OnAddgroup();
	afx_msg void OnDelgroup();
	afx_msg void OnChangegroupname();
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int	m_sel;
	int	m_uIndex;
	int	m_cnt;
	int	m_count;
	CPoint	m_pos;

	bool		m_bDrag;
	CWnd*		m_pDragWnd;
	HTREEITEM	m_hCurItem;
	CImageList*	m_pDragImage;

	class CTMenu*	m_menu;

	CString		m_treeN[MAX];
	HTREEITEM	m_itemN[MAX];
	CStringArray	m_arMap;
	CStringArray	m_arTr;
	CMapStringToString	m_arDesc;
	CArray <HTREEITEM, HTREEITEM>	m_arItem;
protected:
	void	fit();
	void	fitG();
	void	initList();
	void	setList(BOOL flag = TRUE);
	void	make_Tree(CString dat);
	void	setTree();
	void	redrawGList();
	void	setCount(int cnt = -1, int tot = -1);
	
	void	findItem(HTREEITEM item, CString comS);
	void	treeSelect(int idx);
	void	listSelect(int idx);
	void	enableButton(bool bEnable);

	void	LoadInfo();
	void	SaveInfo();
	CImageList*	createDragImageEx(CListCtrl *pList, LPPOINT lpPoint);
	CImageList*	createDragImageEx(CTreeCtrl *pTree, LPPOINT lpPoint);

	
/////////////////////////////////////////////////////////////////////////////
//	 특정사용자에게만 메뉴 오픈
// 2006 11 08
	CString	m_id;
/////////////////////////////////////////////////////////////////////////////
	
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGMAP_H__64B3215B_1B9F_4637_A985_FA60952B8E57__INCLUDED_)
