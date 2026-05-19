#pragma once

#define	stIDLE		0
#define	stADDRESS	1
#define	stCONNECTING	2
#define	stCONNECTED	3
#define	stRECEIVING	4
#define	stWAIT		5
#define	stCOMPLETE	6
#define	stSTOP		7

#define	MAPEN		"sform"
#define	UOBEN		"uform"

// CwSock

class AFX_EXT_CLASS CwSock : public CWnd
{
	DECLARE_DYNAMIC(CwSock)

public:
	CwSock(CWnd* pWnd);
	virtual ~CwSock();

	char*	m_recvB;

	bool	m_alive;
	CWnd*	m_wnd;
	class CSock*	m_sock;

	CString m_rtnStr;
	int	m_iSession;
	static CwSock* m_wizard;

protected:
	DECLARE_MESSAGE_MAP()

public:
	bool	Open(CString ips = _T(""), int port = -1);
	BOOL	Write(char* pBytes, int nBytes);
	void	Close();
	void	static sockEvent(int type, char* pBytes, int nBytes, int auxs);
	void	OnSockEvent(int type, char* pBytes, int nBytes, int auxs);

	afx_msg void OnDestroy();
};


