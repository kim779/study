#if !defined(AFX_TRANSDLG_H__9F2F50A0_D239_44D7_B283_5BC463FEAA35__INCLUDED_)
#define AFX_TRANSDLG_H__9F2F50A0_D239_44D7_B283_5BC463FEAA35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// transDlg.h : header file
//

#define	verPORT	15207
#define	szMAX	1024*16

#define	rsADMIN	100
#define	rsNONE	-1

#define	rsAXIS	0
#define	rsDEV	1
#define	rsTAB	2
#define	rsIMAGE	3
#define	rsMTBL	4
#define	rsHELP	5
#define	rsMAP	6
#define	rsEXE	7
#define	rsROP	8
#define	rsMENU	9
#define	rsGEX	10


/////////////////////////////////////////////////////////////////////////////
// CTransDlg dialog

class CTransDlg : public CDialog
{
// Construction
public:
	CTransDlg(CWnd* pParent, int kind);   // standard constructor
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
	int	m_kind;

	enum	{ sNONE, sCREATE, sOPEN } m_state;
	CWnd*	m_socket;
	CString	m_ips;

	char*	m_sendB;
	char*	m_recvB;
	struct	_rsmH*	m_SRH;
	struct	_rsmH*	m_RRH;

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

	// Generated message map functions
	//{{AFX_MSG(CTransDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTransmit();
	virtual void OnCancel();
	afx_msg void OnAddrFieldChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg	int  OnFire(int type, char* pBytes, int nBytes);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSDLG_H__9F2F50A0_D239_44D7_B283_5BC463FEAA35__INCLUDED_)
