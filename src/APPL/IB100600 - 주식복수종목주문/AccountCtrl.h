#pragma once
// Symbol.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSymbol window

class CAccountCtrl : public CWnd
{
// Construction
public:
	CAccountCtrl(CWnd* pView, CWnd* parent, CString home);

// Attributes
public:
	CString		m_home;
	CWnd*		m_pView;
	CWnd*		m_pParent;

	HINSTANCE	m_hAccountDll;
	CWnd*		m_pAccountCtrl;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void createAccountCtrl(CString MapName, int nkey, DWORD BackRGB);
	virtual ~CAccountCtrl();

	long	OnMessage(WPARAM wParam, LPARAM lParam);
	long	OnMessage1(WPARAM wParam, LPARAM lParam);
	CString	GetAccNo();
	CString GetAccName();
	CString GetPswd();
//	bool ConfirmPswd(CString pswd);
	void	TriggerAccount(CString acc);

	CString Parser(CString &srcstr, CString substr);
	void ChangeFont(int ipoint, LPCTSTR lszname);

	// Generated message map functions
protected:
	//{{AFX_MSG(CSymbol)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

