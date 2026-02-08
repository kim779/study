#if !defined(AFX_TSETUP_H__9739234C_C1D5_4DEC_8F85_611DEE367EEA__INCLUDED_)
#define AFX_TSETUP_H__9739234C_C1D5_4DEC_8F85_611DEE367EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tSetup.h : header file
//

#include "../ctrl/ExTreeCtrl.h"
#include "../ctrl/ExListCtrl.h"
//#include "../ctrl/ExDialog.h"
#include "../ctrl/ExCombo.h"
/////////////////////////////////////////////////////////////////////////////
// CTSetup dialog
#include "SetupDialog.h"
#include <afxtempl.h>

#define MODE_MENU	1
#define MODE_SEARCH	2

#define SUBGROUPNOEXIST	0
#define SUBGROUPEXIST	1

struct _subData
{
	CString	Code;
	CString	Name;
	CString	Short;
};
	
struct _gData	
{
	CString	Code;
	CString	Name;
	CString	Short;
	int	sGroup{};
};

class _subGroup
{
public:
	struct _subData	subData;
	CArray <_subData, _subData> m_sGroup;
};

class _Group	
{
public:
	_Group() { gData.sGroup = SUBGROUPNOEXIST; }
	struct _gData	gData;
	CArray <_gData, _gData> m_pGroup;
};

class AFX_EXT_CLASS CTSetup : public CSetupDialog
{
// Construction
public:
	CBitmap* m_bmpCombo_N;
	CBitmap* m_bmpCombo;
	void draw_groupBox(CPaintDC *pdc, int ids);
	CFont m_font;
	CTSetup(class CTMenu* menu, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTSetup();

	virtual void	ApplySetup();
	virtual void	DefaultSetup();

// Dialog Data
	//{{AFX_DATA(CTSetup)
	enum { IDD = IDD_TSETUP };
	CExTreeCtrl	m_usrtree;
	CExComboBox	m_Combo;
	CExTreeCtrl	m_Tree;
	CExListCtrl	m_List;
	CString		m_Edit;
	CString		m_Result;
	CString		m_editN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTSetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTSetup)
	afx_msg void OnAfter();
	afx_msg void OnBefore();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearch();
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnAddTR();
	afx_msg void OnDelTR();
	afx_msg void OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeGname();
	afx_msg void OnDelall();
	afx_msg void OnAddgroup();
	afx_msg void OnDelgroup();
	afx_msg void OnChangegroupname();
	virtual void OnOK();
	afx_msg void OnToolinit();
	afx_msg void OnAddsubgroup();
	afx_msg void OnDblclkUsertree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragUsertree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeSubName(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangesubgroup();
	afx_msg void OnDelsubgroup();
	afx_msg void OnToolview();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg	LONG OnUSER(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
	int	m_cnt;
	int	m_sel;
	int	m_uIndex;
	int	m_count;
	int	m_sIndex;
	int	m_tool2;

	bool		m_bDrag;
	CWnd*		m_pDragWnd;
	CImageList*	m_pDragImage;

	class CTMenu*	m_menu;

	CString		m_treeN[MAX];
	HTREEITEM	m_itemN[MAX];
	HTREEITEM	m_hCurItem;
	
	CStringArray	m_arMap;
	CStringArray	m_arTr;
	CStringArray	m_gName;
	
	CMapStringToOb		m_mGroup;
//	std::map<CString, std::shared_ptr <_Group>> m_mGroup;
	CMapStringToOb		m_subGroup;
	CMapStringToString	m_arDesc;
	CArray <HTREEITEM, HTREEITEM>	m_arItem;
protected:
	void	fit();
	void	initList();
	void	setList(BOOL flag = TRUE);
	void	setTree();
	void	make_Tree(CString dat);
	void	loadUserTree();
	void	redrawCombo();
	void	setCount(int cnt = -1, int tot = -1);
	
	void	findItem(HTREEITEM item, CString comS);
	void	treeSelect(int idx);
	void	listSelect(int idx);
	void	enableButton(bool bEnable);

	void	LoadInfo(CString setupfile = "");
	void	LoadSubInfo(CString setupfile = "");
	CImageList*	createDragImageEx(CListCtrl *pList, LPPOINT lpPoint);
	CImageList*	createDragImageEx(CTreeCtrl *pTree, LPPOINT lpPoint);

	void	SaveTreeData();
	void	getUserScreen(CString gName, char* tmp, int len, _Group* pGroup);
	void	getUserScreenSub(CString gName, char* tmp, int len, _subGroup* subGroup);
	CString	makeKey();
	void	saveData();
	void	saveScreen(CString key, _Group* pGroup);
	void	saveScreenSub(CString key);

	void	ChangeHangulMode(HWND hWnd);		// 한글 모드로
	void	ChangeEnglishMode(HWND hWnd);		// 영문 모드로

	BOOL	IsExistMap(CString mapN, CString subKey = _T(""));
	void	AddTreeItem(CString mapN, HTREEITEM sItem = NULL);
	void	AddTreeGroup();
	CString	GetSubKey(HTREEITEM item);
	void	delTree(HTREEITEM item);
	BOOL	IsUniqueName(CString name, HTREEITEM owner = NULL);
	void	SetGuide(UINT nID);
	_Group* GetCurGroup();
	CString	GetShortName(CString dat);

	void	MoveItem(HTREEITEM dragItem, HTREEITEM dropItem);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSETUP_H__9739234C_C1D5_4DEC_8F85_611DEE367EEA__INCLUDED_)
