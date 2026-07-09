#if !defined(AFX_ADDRDLG_H__C7D40460_7A9A_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_ADDRDLG_H__C7D40460_7A9A_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// addrDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAddrDlg dialog

class AFX_EXT_CLASS CAddrDlg : public CDialog
{
// Construction
public:
	CAddrDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_currentPath;

// Dialog Data
	//{{AFX_DATA(CAddrDlg)
	enum { IDD = IDD_ADDR };
	CIPAddressCtrl	m_addr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CString	m_ips;

// Implementation
protected:
	void	getAddress(BYTE& field0, BYTE& field1, BYTE& field2, BYTE& field3);

	// Generated message map functions
	//{{AFX_MSG(CAddrDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRDLG_H__C7D40460_7A9A_11D4_A024_00001CD7F9BE__INCLUDED_)
