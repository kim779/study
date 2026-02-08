#if !defined(AFX_GLBDLG_H__78CFAA61_2C31_422B_99CB_C00E1AA53C48__INCLUDED_)
#define AFX_GLBDLG_H__78CFAA61_2C31_422B_99CB_C00E1AA53C48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// glbDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGlbDlg dialog

class CGlbDlg : public CDialog
{
// Construction
public:
	CGlbDlg(CWnd* pParent = NULL);   // standard constructor
	virtual	~CGlbDlg();
//	char* sndB{};
// Dialog Data
	//{{AFX_DATA(CGlbDlg)
	enum { IDD = IDD_GLB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	CString	m_returns;

protected:
	class	Csocket* m_sock;

	CString	m_data;
	CString	m_path;
	bool	m_staff;

	CStringArray	m_ips;
	CWordArray	m_ports;

	int	m_sip;		// start indev of IP
	int	m_iip;
	int	m_iport;

	int	m_wait;
	int	m_write;	// Sleep count (millisecond)
	CString	m_url;

	bool	m_connect;
	bool	m_on;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlbDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void	Init(char* path, char* sndB, int sndL, bool staff);
	void	SetIpIndex(int index){m_iip = index;};
protected:
	void	connect();
	BOOL	sendTR();
	void	dispatch(char* pBytes, int nBytes);
	void	clearSocket();

	bool	loadServer(CString path, bool staff = false);
	void	setGuide(CString guide);

	// Generated message map functions
	//{{AFX_MSG(CGlbDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	long	OnUser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLBDLG_H__78CFAA61_2C31_422B_99CB_C00E1AA53C48__INCLUDED_)
