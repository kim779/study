#if !defined(AFX_LOGIN_H__8450E74D_86C9_4B9A_8676_E88E9F17299B__INCLUDED_)
#define AFX_LOGIN_H__8450E74D_86C9_4B9A_8676_E88E9F17299B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// login.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogin command target

class CLogin : public CCmdTarget
{
	DECLARE_DYNCREATE(CLogin)

	CLogin();           // protected constructor used by dynamic creation
	virtual ~CLogin();

// Attributes
private:
	CString	m_returns;
	BOOL	m_order;

	CString	m_user;
	CString	m_name;
	CString	m_cogb;
	CString	m_brno;
	CString	m_cani;
	CString	m_utyp;
	CString	m_ctyp;
	CString	m_otpf;
	CString	m_misf;
	CString	m_juno;
	CString	m_lcog;
	CString	m_lpci;
	CString	m_lpui;
	CString	m_litm;
	CString	m_lotm;
	CString	m_day0;
	CString	m_day1;
	CString	m_day2;
	CString	m_nick;


// Operations
public:
	CString	SetLogin(char* data);
	CString	GetLogin(int id);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	clear();
	void	setDefault(char* data);

	// Generated message map functions
	//{{AFX_MSG(CLogin)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLogin)
	afx_msg BSTR _getUser();
	afx_msg BSTR _getName();
	afx_msg BOOL _getOrder();
	afx_msg BSTR _getCogb();
	afx_msg BSTR _getBrno();
	afx_msg BSTR _getCani();
	afx_msg BSTR _getUtyp();
	afx_msg BSTR _getCtyp();
	afx_msg BSTR _getOtpf();
	afx_msg BSTR _getJuno();
	afx_msg BSTR _getLcog();
	afx_msg BSTR _getLpci();
	afx_msg BSTR _getLpui();
	afx_msg BSTR _getLitm();
	afx_msg BSTR _getLotm();
	afx_msg BSTR _getDay0();
	afx_msg BSTR _getDay1();
	afx_msg BSTR _getDay2();
	afx_msg BSTR _getMisf();
	afx_msg BSTR _getNick();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__8450E74D_86C9_4B9A_8676_E88E9F17299B__INCLUDED_)
