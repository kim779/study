#if !defined(AFX_USCODEDLG_H__CE670B0F_1879_4184_ACF8_AD86C90731B4__INCLUDED_)
#define AFX_USCODEDLG_H__CE670B0F_1879_4184_ACF8_AD86C90731B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// USCodeDlg.h : header file
//

#include "fxButton.h"

#define	SORT_LEN_CODE	16
#define	SORT_LEN_NAME	40
#define	SORT_LEN_CNAME	40

/////////////////////////////////////////////////////////////////////////////
// CUSCodeDlg dialog

class CUSCodeDlg : public CDialog
{
// Construction
public:
	CUSCodeDlg(CPoint pt, CWnd* pParent = NULL);   // standard constructor

public:
	HBRUSH	m_hBrush;
	CPoint	m_Point;
	CString	m_sRoot;

	struct _ListSort { 
		char*	Code[SORT_LEN_CODE];
		char*	Name[SORT_LEN_NAME];
		char*	CName[SORT_LEN_CNAME];
	};
	CArray <_ListSort*, _ListSort*> m_listSort;

	CFont	m_CtrlFont;
	bool	m_nameFlag;

	struct _CodeName {
		CString	symb;
		CString knam;
		CString enam;
		CString cnam;
	};
	CArray <_CodeName, _CodeName> m_CodeNameArray;

	//미국지수, 세계지수, 미국종목, 세계종목
	CArray <CString, CString> m_ListIndexUS;
	CArray <CString, CString> m_ListIndexGlobal;
	CArray <CString, CString> m_ListItemUS;
	CArray <CString, CString> m_ListItemGlobal;

	struct _exCode {
		CString code;
		CString name;
	};
	_exCode	m_rtnData;

	//Current Button State
	//전종목	= 0
	//미국지수	= 1		
	//세계지수	= 2
	//미국주요종목	= 3
	//세계주요종목	= 4
	//CME선물	= 5
	//해외		= 6
	int	m_nCurPos;
public:
	void	SetDlgPos(int x, int y);
	CString	GetRtnData(bool dialog = true, CString code = _T(""));
	void	ReSizeCodeList(int kind);
	CString	GetCountry(CString ncod);
	CString GetCountryCode(CString sCountry);
	void	AddCodeNameArray(_USCode* usCode);
	void	ToggleName(int kind);
	void	SetCodeList(int kind);
	int	findIndex(CString text, int len, int column = 1);
	void	killSelected(CListCtrl* ListCtrl);
	void	setSelected(CListCtrl* ListCtrl, int nItem, BOOL scroll = TRUE);
	void	SetBtn();
	void	AddListCtlCode(int nItem, CString szCode, CString szName, CString CName);
	void	ClearListCtlCode(int nItem = -1 );
	void	InitCtrl();
	void	InitList();
	void	LoadList();
	void	SaveList(CString sDir);
	int	GetItemUS(CString sItem);
	int	GetIndexUS(CString sIndex);
	CString	Parser(CString &srcstr, CString substr);
	LOGFONT	SetFont(int Size);

// Dialog Data
	//{{AFX_DATA(CUSCodeDlg)
	enum { IDD = IDD_DIALOG_USCODE };
	CfxButton	m_btn_mat;
	CfxButton	m_btn_lbi;
	CfxButton	m_btn_kai;
	CfxButton	m_btn_gof;
	CfxButton	m_btn_exr;
	CComboBox	m_cbo_slist;
	CEdit	m_edit_search;
	CfxButton	m_btn_eng;
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
	CfxButton	m_btn_all;
	CfxButton	m_btn_usi;
	CfxButton	m_btn_wri;
	CfxButton	m_btn_amq;
	CfxButton	m_btn_wmq;
	CfxButton	m_btn_cme;
	CfxButton	m_btn_adr;
	CSkinListCtrl	m_ListCtlCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUSCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditSearch();
	afx_msg void OnSelchangeCboSlist();
	afx_msg void OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnClickBtnKind(UINT nID);
	afx_msg void OnClickBtnSort(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USCODEDLG_H__CE670B0F_1879_4184_ACF8_AD86C90731B4__INCLUDED_)
