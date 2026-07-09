#if !defined(AFX_FUNDDLG_H__49F05DB7_0A7A_48E1_859D_670477F0248E__INCLUDED_)
#define AFX_FUNDDLG_H__49F05DB7_0A7A_48E1_859D_670477F0248E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FundDlg.h : header file
//
#include "fxButton.h"
/////////////////////////////////////////////////////////////////////////////
// CFundDlg dialog

#define	FD_LEN_CODE	5 + 1
#define	FD_LEN_NAME	30 + 1


class CFundDlg : public CDialog
{
// Construction
public:
	CFundDlg(CPoint pt = CPoint(0, 0), CWnd* pParent = NULL);   // standard constructor
public:
	CPoint	m_Point;
	CString	m_sRoot;
	CFont	m_CtrlFont;

	struct _ListSort { 
		char*	Code[FD_LEN_CODE];
		char*	Name[FD_LEN_NAME];
	};
	CArray <_ListSort*, _ListSort*> m_listSort;

	struct _CodeName {
		CString	code;
		CString name;
	};
	CArray <_CodeName, _CodeName> m_CodeNameArray;

	_CodeName	m_rtnData;
	HBRUSH		m_hBrush;

public:
	void	SetDlgPos(int x, int y);
	CString	GetRtnData(bool dialog = true, CString code = _T(""));
	void	AddCodeNameArray(_FDCode* fdCode);
	void	SetCodeList(int kind);
	int	findIndex(CString text, CString nxText, int len, int column = 1);
	void	killSelected(CListCtrl* ListCtrl);
	void	setSelected(CListCtrl* ListCtrl, int nItem, BOOL scroll = TRUE);
	void	SetBtn();
	void	ClearListCtlCode(int nItem = -1);
	void	AddListCtlCode(int nItem, CString szCode, CString szName);
	void	InitCtrl();
	BOOL	FindCode(int kind, CString szCode);
	CString		GetCode();
	CString		GetName();
	LOGFONT	SetFont(int Size);

// Dialog Data
	//{{AFX_DATA(CFundDlg)
	enum { IDD = IDD_DIALOG_FUND };
	CfxButton	m_btn_sort16;
	CfxButton	m_btn_sort15;
	CfxButton	m_btn_sort14;
	CfxButton	m_btn_sort13;
	CfxButton	m_btn_sort12;
	CfxButton	m_btn_sort11;
	CfxButton	m_btn_sort10;
	CfxButton	m_btn_sort09;
	CfxButton	m_btn_sort08;
	CfxButton	m_btn_sort07;
	CfxButton	m_btn_sort06;
	CfxButton	m_btn_sort05;
	CfxButton	m_btn_sort04;
	CfxButton	m_btn_sort03;
	CfxButton	m_btn_sort02;
	CfxButton	m_btn_sort01;
	CfxButton	m_btn_bmx;
	CfxButton	m_btn_all;
	CEdit	m_edit_search;
	CfxButton	m_btn_stk;
	CfxButton	m_btn_smx;
	CfxButton	m_btn_sfund;
	CfxButton	m_btn_mmf;
	CfxButton	m_btn_bond;
	CSkinListCtrl	m_ListCtlCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFundDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditSearch();
	afx_msg void OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnClickBtnKind(UINT nID);
	afx_msg void OnClickBtnSort(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNDDLG_H__49F05DB7_0A7A_48E1_859D_670477F0248E__INCLUDED_)
