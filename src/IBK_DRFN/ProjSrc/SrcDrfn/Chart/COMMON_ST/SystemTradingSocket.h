#if !defined(AFX_SYSTEMTRADINGSOCKET_H__20061B01_2852_4F4E_BEA8_789D8EE51F0A__INCLUDED_)
#define AFX_SYSTEMTRADINGSOCKET_H__20061B01_2852_4F4E_BEA8_789D8EE51F0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemTradingSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingSocket command target

class CSocketInterface;
class CRcvSocket;
class CSystemTradingSocket : public CAsyncSocket
{
// Attributes
public:
	CSocketInterface*	m_pInterface;

// Operations
public:
	CSystemTradingSocket(CSocketInterface* pInterface);
	virtual ~CSystemTradingSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingSocket)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSystemTradingSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADINGSOCKET_H__20061B01_2852_4F4E_BEA8_789D8EE51F0A__INCLUDED_)
