#if !defined(AFX_SCREENLIST_H__EBB3AD9B_A919_4374_8ED4_712BBE17A39B__INCLUDED_)
#define AFX_SCREENLIST_H__EBB3AD9B_A919_4374_8ED4_712BBE17A39B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenList.h : header file
//
#include "resource.h"
#include <afxcoll.h>

#define	HISTORY		1
#define	SEARCH		2
#define	L_MAPN		8
#define	LBM_BASE	100
#define	LBM_SELECTION	(LBM_BASE +1)
#define	LBM_NOSELECTION	(LBM_BASE +2)
/////////////////////////////////////////////////////////////////////////////
// CScreenList dialog

class CScreenList : public CDialog
{
// Construction
public:
	CScreenList(bool (*listcall)(int, WPARAM, LPARAM), CWnd* pParentWnd = NULL);   // standard constructor

	void add_History(CString dat);
	void copy_History(CStringArray& list);
	void show_Histroy();
	bool updateCtrl(const CStringArray& array, int type = HISTORY, bool bParse = true);

public:
	BOOL	m_bAscending{};
	struct HSORTITEM{
		CListCtrl* pSortItem;
		int iColumn;
	};
// Dialog Data
	//{{AFX_DATA(CScreenList)
	enum { IDD = IDD_SCREENLIST };
	CListCtrl	m_listctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool	(*m_listcall)(int, WPARAM, LPARAM);
	CStringArray	m_history;
	int nMaxW;

	void	fit(bool bSize = TRUE);
	void	Init();
	// Generated message map functions
	//{{AFX_MSG(CScreenList)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	static int CALLBACK SortFunc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENLIST_H__EBB3AD9B_A919_4374_8ED4_712BBE17A39B__INCLUDED_)
