#pragma once
// USCodeDlg.h : header file
//

#include "skinlistctrl.h"
#include "JongmukDlg.h"
#include "btnst.h"

#define	SORT_LEN_CODE	16
#define	SORT_LEN_NAME	40
#define	SORT_LEN_ENAME	40

/////////////////////////////////////////////////////////////////////////////
// CUSCodeDlg dialog

class CUSCodeDlg : public CDialog
{
// Construction
public:
	CUSCodeDlg(CPoint pt, CWnd* pParent = nullptr);   // standard constructor

public:
	CPoint	m_point;
	CString	m_root;
	CArray<_USCode*, _USCode*> m_aUSCode;
	std::vector<_USCode*> _vUSCode;

	enum _codeKind {ckAll = -1, ckJisu, ckItem, ckFuture, ckRate, ckDebt, ckExchange, ckTop/*, ckEtc*/} m_CodeKind;

	
	CFont	m_CtrlFont;
	bool	m_nameFlag;


	struct _exCode {
		CString code;
		CString name;
	};
	_exCode	m_rtnData;

public:
	void	SetDlgPos(int x, int y);
	CString	GetRtnData(bool dialog = true, CString code = _T(""));
	void	ReSizeCodeList(int kind);
	CString	GetCountry(CString ncod);
	void	SetCodeList(int kind);
	int	findIndex(CString text, int len, int column = 1);
	void	killSelected(CListCtrl* ListCtrl);
	void	setSelected(CListCtrl* ListCtrl, int nItem, BOOL scroll = TRUE);
	void	SetBtn();
	BOOL	LoadUSCode();
	void	InitCtrl();
	LOGFONT	SetFont(int Size);
	void	ResetCboItem(enum _codeKind codeKind);
	void	SelectCode(enum _codeKind codeKind, short subKind);
	BOOL    LoadTopCode();

// Dialog Data
	//{{AFX_DATA(CUSCodeDlg)
	enum { IDD = IDD_DIALOG_USCODE };
	CButtonST	m_btn_top;
	CButtonST	m_btn_rate;
	CButtonST	m_btn_jisu;
	CButtonST	m_btn_item;
	CButtonST	m_btn_future;
	CButtonST	m_btn_exchange;
	CButtonST	m_btn_eng;
	CButtonST	m_btn_debt;
	CComboBox	m_cbo_slist;
	CEdit		m_edit_search;
	CButtonST	m_btn_sort14;
	CButtonST	m_btn_sort13;
	CButtonST	m_btn_sort12;
	CButtonST	m_btn_sort11;
	CButtonST	m_btn_sort10;
	CButtonST	m_btn_sort09;
	CButtonST	m_btn_sort08;
	CButtonST	m_btn_sort07;
	CButtonST	m_btn_sort06;
	CButtonST	m_btn_sort05;
	CButtonST	m_btn_sort04;
	CButtonST	m_btn_sort03;
	CButtonST	m_btn_sort02;
	CButtonST	m_btn_sort01;
	CButtonST	m_btn_all;
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
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditSearch();
	afx_msg void OnSelchangeCboSlist();
	afx_msg void OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnClickBtnKind(UINT nID);
	afx_msg void OnClickBtnSort(UINT nID);
	DECLARE_MESSAGE_MAP()
private:

	CString _sBuffer;
	std::vector<std::tuple<CString, CString, CString, CString>> _vData, _vSelect;
	std::map<CString, CString> _mTop;

	void listsort(int column);

public:
	afx_msg void OnGetdispinfoListCode(NMHDR* pNMHDR, LRESULT* pResult);
};

