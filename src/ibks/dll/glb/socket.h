#if !defined(AFX_SOCKET_H__F4276DA0_4208_4E81_80F3_532AF9227F75__INCLUDED_)
#define AFX_SOCKET_H__F4276DA0_4208_4E81_80F3_532AF9227F75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// socket.h : header file
//

#include <afxmt.h>

#define	maxBUFs		2*1024

#define	sm_CLOSE	0
#define	sm_CONNECT	1
#define	sm_RECEIVE	2
#define	sm_NEXTSOCK	3

/////////////////////////////////////////////////////////////////////////////
// Csocket command target

class Csocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	Csocket(CWnd* parent = NULL);
	virtual ~Csocket();

// Overrides
public:
	BOOL	Open(CString ips, int port);
	void	Close();
	BOOL	Write(char* sndB, int sndL);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Csocket)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(Csocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CWnd*	m_parent;

	bool	m_alive;
	CEvent	m_event;
	CSingleLock* m_lock;

	char	m_rcvB[maxBUFs];
	int	m_rcvL;

	int	m_idx;
	enum	_flow { flowS = 0, flowH, flowD, flowE } m_flow;	// frame flow
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKET_H__F4276DA0_4208_4E81_80F3_532AF9227F75__INCLUDED_)
