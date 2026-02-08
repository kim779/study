#if !defined(AFX_CLIENTSOCKET_H__CD1F510F_3923_4372_B322_63AFAC4ECE9E__INCLUDED_)
#define AFX_CLIENTSOCKET_H__CD1F510F_3923_4372_B322_63AFAC4ECE9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CSocket
{
// Attributes
public:
	LPSTR	m_lpszPacketData;
	LPSTR	m_lpszSendData;
	LPSTR	m_lpszTempPacketData;
	BOOL	m_bDownloadingFile;
	BOOL	m_bDownloadingFileUF;
	BOOL	m_bDownloadingFileZip;
	int		m_nPacketIndex;
	BOOL	m_bStopCompiling;

// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();

	void	SendToServer(int nCommand, LPVOID lpData, int nDataSize, UINT nStrategyType = 0);
	void	ProceedPacketData();
	void	WriteDllFile();
	void	WriteDllFileUF();
	void	WriteDllFileZip();
	BOOL	IsEndPacket(int nIndex);
	void	StopCompiling();
	CString		GetUserName();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__CD1F510F_3923_4372_B322_63AFAC4ECE9E__INCLUDED_)
