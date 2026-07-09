#if !defined(AFX_PASSDLG_H__B33599FF_02F3_4C06_954D_AA34014A40FC__INCLUDED_)
#define AFX_PASSDLG_H__B33599FF_02F3_4C06_954D_AA34014A40FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PassDlg.h : header file
//

#include <afxtempl.h>
#include "resource.h"

class	Caccount
{
public:
	CString	m_name;
	CString	m_account;
	CString	m_pass;
};

/////////////////////////////////////////////////////////////////////////////
// CPassDlg dialog

class CPassDlg : public CDialog
{
// Construction
public:
	CPassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPassDlg();

// Dialog Data
	//{{AFX_DATA(CPassDlg)
	enum { IDD = IDD_SAVEPASS };
	CListCtrl	m_list;
	//}}AFX_DATA
	CArray	< Caccount *, Caccount * > m_acs;
	char	m_sndB[128];

protected:
	int	m_select;
	bool	m_bNext;
	CWnd*	m_mainframe;
public:
	void	SetData(CString data);

protected:
	void	fit();
	bool	setAccountData(int sel);
	void	setStatus(CString status);
	CString	editAccount(CString account);

	BOOL	addColumn(LPCTSTR columns, int col, int width);
	BOOL	addItem(CString items, int item, int subi, bool press = false);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPassDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSave();
	afx_msg void OnListClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnCheckPass(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSDLG_H__B33599FF_02F3_4C06_954D_AA34014A40FC__INCLUDED_)
