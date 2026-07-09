#if !defined(AFX_SETUP_H__8BCB100B_A807_444A_AB71_D9978B3853A4__INCLUDED_)
#define AFX_SETUP_H__8BCB100B_A807_444A_AB71_D9978B3853A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setup.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSetup dialog

class CSetup : public CDialog
{
// Construction
public:
	CSetup(CString home, int connectBy, CWnd* pParent = NULL);   // standard constructor
	~CSetup();

	int	GetBy() { return m_by; }

protected:
	CWnd*	m_parent;
	int	m_by;
	CString	m_home;
	BOOL	m_bSDI;

	CStringArray	m_ips;

// Dialog Data
	//{{AFX_DATA(CSetup)
	enum { IDD = IDD_SETUP };
	CComboBox	m_server;
	BOOL	m_bReset;
	BOOL	m_bStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	loadServer(int type);
	void	getStrings(CString& str, CString& keys);

	// Generated message map functions
	//{{AFX_MSG(CSetup)
	afx_msg void OnTcp();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUdp();
	afx_msg void OnUdpd();
	afx_msg void OnIpinput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUP_H__8BCB100B_A807_444A_AB71_D9978B3853A4__INCLUDED_)
