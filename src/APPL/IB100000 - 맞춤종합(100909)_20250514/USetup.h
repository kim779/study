#pragma once
// USetup.h : header file
//
#include "BtnST.h"
#include <afxcoll.h>

#define	TYPE_NONE	0
#define	TYPE_1		1
#define	TYPE_2		2
#define	TYPE_3		3
#define	TYPE_4		4
#define	TYPE_5		5
#define	TYPE_6		6
#define	TYPE_7		7
/////////////////////////////////////////////////////////////////////////////
// CUSetup dialog

class CUSetup : public CDialog
{
// Construction
public:
	CUSetup(CString home, CString user, CWnd* pParent = NULL);   // standard constructor
	virtual ~CUSetup();

	BOOL	IsChange() {	return m_bChange; }
// Dialog Data
	//{{AFX_DATA(CUSetup)
	enum { IDD = IDD_USERSETUP };
	CListCtrl	m_mlist;
	CListCtrl	m_slist;
	CListCtrl	m_list;
	CButtonST	m_type7;
	CButtonST	m_type6;
	CButtonST	m_type5;
	CButtonST	m_type4;
	CButtonST	m_type3;
	CButtonST	m_type2;
	CButtonST	m_type1;
	//}}AFX_DATA

protected:
	int		m_selectRes;
	int		m_selectType;
	int		m_selectA;
	BOOL		m_bChange;
	CRect		m_vRc[4];
	CString		m_home;
	CString		m_user;
	CString		m_section;

	CBrush		m_brushC;

	CStringArray	m_arInfo;
	CStringArray	m_arS[4];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	init_Ctrl();
	void	init_Button();
	void	init_List();
	void	fill_List(char type = 0);
	void	fill_UList();
	void	ChangeView(CPoint point);
	void	UpdateView();
	void	countView(int selectA);
	void	saveViewPos();
	void	positionView(int type = TYPE_NONE);
	void	fit(CListCtrl& list);
	void	fit2(CListCtrl& list);
	void	load_Slist();
	void	change_UserConfig();
	BOOL	addColumn(CListCtrl& list, LPCTSTR columns, int col, int width);
	BOOL	addItem(CListCtrl& list, CString items, int item, int subi = 0);
	CString	mapInfo(CString mapN);

	// Generated message map functions
	//{{AFX_MSG(CUSetup)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnType1();
	afx_msg void OnType2();
	afx_msg void OnType3();
	afx_msg void OnType4();
	afx_msg void OnType5();
	afx_msg void OnType6();
	afx_msg void OnType7();
	afx_msg void OnStock();
	afx_msg void OnFoption();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSave();
	afx_msg void OnDelete();
	afx_msg void OnDblclkSlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedUsermaplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickUsermaplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnAddall();
	afx_msg void OnBtnDelall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

