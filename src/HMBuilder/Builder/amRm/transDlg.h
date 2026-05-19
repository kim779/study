#if !defined(AFX_TRANSDLG_H__9F2F50A0_D239_44D7_B283_5BC463FEAA35__INCLUDED_)
#define AFX_TRANSDLG_H__9F2F50A0_D239_44D7_B283_5BC463FEAA35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// transDlg.h : header file
//

#define	szMAX	1024*32

#define	modeNONE	-1
#define	modeUSER	0
#define	modeADMIN	100

#define	kindNONE	-1
#define	kindFORMB	1	// form binary
#define	kindFORMS	2	// form source
#define	kindIMAGE	3	// image
#define	kindTRLY	4	// layout
#define	kindINFO	5	// tab information

/////////////////////////////////////////////////////////////////////////////
// CTransDlg dialog

class CTransDlg : public CDialog
{
// Construction
public:
	CTransDlg(CWnd* pParent, int kind, CString id, CString desc);   // standard constructor
	virtual	~CTransDlg();

// Dialog Data
	//{{AFX_DATA(CTransDlg)
	enum { IDD = IDD_TRANSMIT };
	CProgressCtrl	m_progress;
	CIPAddressCtrl	m_addr;
	//}}AFX_DATA

public:
	CStringArray	m_files;

protected:
	int	m_mode;		// run mode
	int	m_kind;		// combol select
	CString	m_id;		// login id
	CString	m_desc;		// modify description

	enum	{ sNONE, sCREATE, sOPEN } m_state;
	class	CwSock*	m_socket;
	CString	m_ips;
	int	m_port;

	char*	m_sendB;
	char*	m_recvB;
	struct	_rsmH*	m_SRH;
	struct	_rsmH*	m_RRH;
	struct	_saveform* m_saveF;

	CString	m_dir;
	CFile	m_file;
	int	m_size;
	bool	m_finish;

	int	m_index;
	int	m_sendL;
	int	m_offset;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool	sendData();
	int	makeData();
	void	close();
	void	updateProgress(int offset = 0);
	void	enableSend(bool enable = true);
	CString	makePath(CString name);
	int	toInt(char* pBytes, int plen);

	// Generated message map functions
	//{{AFX_MSG(CTransDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTransmit();
	virtual void OnCancel();
	afx_msg void OnAddrFieldChanged(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnSocketSession(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSocketData(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSDLG_H__9F2F50A0_D239_44D7_B283_5BC463FEAA35__INCLUDED_)
