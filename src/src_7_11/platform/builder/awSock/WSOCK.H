// wsock.h: interface for the CwSock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSOCK_H__A52CBB3D_AFFB_42A1_B790_212545EC96FE__INCLUDED_)
#define AFX_WSOCK_H__A52CBB3D_AFFB_42A1_B790_212545EC96FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	WRK_PORT	15105
#define	US_PORT		15206

#define	stIDLE		0
#define	stADDRESS	1
#define	stCONNECTING	2
#define	stCONNECTED	3
#define	stRECEIVING	4
#define	stWAIT		5
#define	stCOMPLETE	6
#define	stSTOP		7

#define	DN_FILE		"resource.ini"
#define	RESSN		"ROOT"
#define	MAPEN		"sform"
#define	UOBEN		"uform"


class AFX_EXT_CLASS CwSock : public CWnd 
{
public:
	CwSock(CWnd* pWnd);
	virtual ~CwSock();

public:
	char*	m_sendB;
	char*	m_recvB;
	struct	_rsmH*	m_sRSM;
	struct	_rsmH*	m_rRSM;

	int	m_sock;
	CWnd*	m_wnd;
	CWnd	*m_pSockCtrl;

	CString m_rtnStr;

protected:
	CString	m_ips, m_sRoot;
	int	m_port;

public:
	bool	Open(CString ips = _T(""), int port = -1);
	bool	Write(int size);
	void	Close();

	// Generated message map functions
protected:
	//{{AFX_MSG(CwSock)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnFireEvent(long type, long pBytes, long nBytes);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

#endif // !defined(AFX_WSOCK_H__A52CBB3D_AFFB_42A1_B790_212545EC96FE__INCLUDED_)
