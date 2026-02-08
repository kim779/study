#pragma once
// UpCodeDlg.h : header file
//

#include "BtnST.h"
#include "SkinListCtrl.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CUpCodeDlg dialog

class CUpCodeDlg : public CDialog
{
// Construction
public:
	CUpCodeDlg(CWnd* pParent = nullptr);   // standard constructor

public:
	CFont	m_CtrlFont;

	CString	m_sRoot;
	CArray <upcode , upcode> m_aUPCode;

	struct _exCode {
		CString code;
		CString name;
	};
	CArray <_exCode, _exCode> m_exCode[2];

	_exCode	m_rtnData;

	int	m_btnSel;
	HBRUSH	m_hBrush;
	
public:
	int InsertListCtlCode(int nIdx, CString szCode, CString szName);
	CString	GetRtnData();
	void	GetExCode();
	void	ClearListCtlCode(int nItem = -1);
	void	AddListCtlCode(CString szCode, CString szName);
	BOOL	loadUPCode();
	LOGFONT	SetFont(int Size);
	void	InitListCtrl();
	void	SetBtn();

// Dialog Data
	//{{AFX_DATA(CUpCodeDlg)
	enum { IDD = IDD_DIALOG_UPCODE };
	CSkinListCtrl	m_ListCtlCode;
	CButtonST	m_btn_sec;
	CButtonST	m_btn_kpu;
	CButtonST	m_btn_kdu;
	CButtonST	m_btn_fus;
	CButtonST	m_btn_free;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpCodeDlg)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnClickBtnKind(UINT nID);
	DECLARE_MESSAGE_MAP()
};

