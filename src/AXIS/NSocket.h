#if !defined(AFX_NSOCKET_H__F9B0FA99_58F3_4C71_83B5_F1CF6434D026__INCLUDED_)
#define AFX_NSOCKET_H__F9B0FA99_58F3_4C71_83B5_F1CF6434D026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NSocket.h : header file
//
#define	STX	0xfe

struct _mid {
	char	pcip[16];
	char	bpip[16];
};

struct _mod {
	char	retc[1];
	char	bpip[16];
};

#define L_mid	sizeof(struct _mid)
#define L_mod	sizeof(struct _mod)
/////////////////////////////////////////////////////////////////////////////
// CNSocket command target

class CNSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CNSocket(CWnd* parent);
	virtual ~CNSocket();
	CWnd*	m_parent;

	int	m_nBytes{};
	char	m_pBytes[1024]{};
	enum	{flowSTX = 0, flowETX}	m_flow;
	enum	{flowSTX1 = 0, flowSTX2} m_step;
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CNSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NSOCKET_H__F9B0FA99_58F3_4C71_83B5_F1CF6434D026__INCLUDED_)
