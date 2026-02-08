#pragma once
// BondDlg.h : header file
//

#include <afxtempl.h>
#include "skinlistctrl.h"
#include "MyEdit.h"

typedef struct CJcode{
	CString code;
	char    gubn{};
	char	ngub{};	// √÷±Ÿ
	CString name;
} CJCODE;

static	BOOL	m_gsort;

/////////////////////////////////////////////////////////////////////////////
// CBondDlg dialog

class CBondDlg : public CDialog
{
// Construction
public:
	int GetCodeType();
	CBondDlg(CPoint pt, int nType = 0, CWnd* pParent = nullptr);   // standard constructor

	void	Set_KS_HanMode();
	void	SetDlgPos(int x, int y);
	BOOL	FindCode(CString szCode);
	void	AddRow(int sort);
	CString	GetCode(){ return m_szCode; }
	CString	GetName(){ return m_szName; }
	void	SetName(CString szName){ m_szName = szName; }
	void    listsort(int column);

// Dialog Data
	//{{AFX_DATA(CBondDlg)
	enum { IDD = IDD_DIALOG_BOND };
	CSkinListCtrl	m_ListCtrl;
	int		m_nRadio{};
	CString	m_szEditSearch;	
	CMyEdit	m_edSearch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBondDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont	m_Font;
	CPoint	m_point;
	CString	m_szCode;
	CString	m_szName;
	int m_nType{};

	std::vector<std::tuple<CString, CString, char, char>>& _vABond = ((CAxisCodeApp *)AfxGetApp())->_vBond;
	std::vector<std::pair<CString, CString>> _vBond;

	// Generated message map functions
	//{{AFX_MSG(CBondDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListBond(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListBond(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditSearch();
	afx_msg void OnChklast();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetdispinfoListBond(NMHDR* pNMHDR, LRESULT* pResult);
};

