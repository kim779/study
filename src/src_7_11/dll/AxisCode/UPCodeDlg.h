#if !defined(AFX_UPCODEDLG_H__939F43AD_D60C_4779_9DF2_538DA1D6A8C4__INCLUDED_)
#define AFX_UPCODEDLG_H__939F43AD_D60C_4779_9DF2_538DA1D6A8C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UPCodeDlg.h : header file
//

#include "fxButton.h"
/////////////////////////////////////////////////////////////////////////////
// CUPCodeDlg dialog

class CUPCodeDlg : public CDialog
{
// Construction
public:
	CUPCodeDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CFont	m_CtrlFont;
	CString	m_sRoot;

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
	LOGFONT	SetFont(int Size);
	void	InitListCtrl();
	void	SetBtn();
	
// Dialog Data
	//{{AFX_DATA(CUPCodeDlg)
	enum { IDD = IDD_DIALOG_UPCODE };
	CfxButton	m_btn_free;
	CfxButton	m_btn_sec;
	CfxButton	m_btn_kpu;
	CfxButton	m_btn_kdu;
	CfxButton	m_btn_fus;
	CSkinListCtrl	m_ListCtlCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUPCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUPCodeDlg)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnClickBtnKind(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPCODEDLG_H__939F43AD_D60C_4779_9DF2_538DA1D6A8C4__INCLUDED_)
