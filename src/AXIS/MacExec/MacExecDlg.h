// MacExecDlg.h : header file
//

#if !defined(AFX_MACEXECDLG_H__38E15097_A3C4_4D2F_ABF6_A4CEEA220CE9__INCLUDED_)
#define AFX_MACEXECDLG_H__38E15097_A3C4_4D2F_ABF6_A4CEEA220CE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConfigDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMacExecDlg dialog

class CMacExecDlg : public CDialog
{
// Construction
public:
	CMacExecDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMacExecDlg)
	enum { IDD = IDD_MACEXEC_DIALOG };
	CListCtrl	m_list;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacExecDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMacExecDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Init();
	void Free();
	void Load();
	void Save();

	void DisplayListItem();
	void DisplayListItem(int row, Config* cfg, BOOL bAdd = TRUE);
	int  GetListIndex();

	void Execute(Config* cfg);
	void Execute();
private:
	CConfigList m_config;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACEXECDLG_H__38E15097_A3C4_4D2F_ABF6_A4CEEA220CE9__INCLUDED_)
