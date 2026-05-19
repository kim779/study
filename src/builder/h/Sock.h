#pragma once

////////////////////////////////////////////////////////////////////////////////
//
//	Sock Event
//	void OnSockEvent(int type, char* pBytes, int nBytes, int auxs)
//		type   : event type
//		pBytes : data
//		nBytes : detail information (length, code, flag, etc)
//		auxs   : auxiliary information (session, etc)
//

// type
//
#define	smOPEN		0		// axis connected
					// wParam : session id (0 : ssMASTER, 1~7 : ssSEC1~7)
					// lParam : zero  : success
					//          other : fail code
#define	smCLOSE		1		// axis closed
					// wParam : session id
#define	smRUN		2		// axis/workstation start
					// wParam : session id
					// lParam : true  : axis update
					//          false : axis start
#define	smSIZE		3		// file size information
#define	smVERS		4		// screen version table

#define	smANM		5		// alert message(tick, flash)
#define	smAXIS		6		// axis message
					// LOWORD(wParam) : key
					// HIWORD(wParam) : ...
#define	smSTAT		7		// progress stat
					// resource name, progress stat
#define	smPUSH		8		// push message

#define	smERROR		9		// error, string

//
//	StartAxis.mode
//
#define	modeWM			0		// workshop manager
#define	modeWS			-1		// workstation mode
#define	modeRM			-2		// resource/version control manager
#define	modeWTS			-3		// wts manager
#define	modeRMx			-4		// resource/version control manager (only P/F files)

//
//	GetEnum.mode
//
#define	enumRTS			0		// get queuing rts data count
#define	enumIP			1		// get PC IP


// CSock

class AFX_EXT_CLASS CSock : public CWnd
{
	DECLARE_DYNAMIC(CSock)

public:
	CSock();
	virtual ~CSock();

private:
	class CDgram*	m_dgram;
	class CWsh*	m_rsm;
	class CWsh*	m_wsh;
	CMap	<int, int, class CStream*, CStream* > m_streams;

	bool	m_startup;
	CString	m_returns;

	void	(*m_callback)(int, char*, int, int);		// OnSockEvent(int type, char* pBytes, int nBytes, int auxs)

public:
	void	SetCallback(LPVOID lpCallback);
	BOOL	OpenSock(int session, CString home, CString ip, int port);
	void	CloseSock(int session);
	BOOL	WriteStream(int session, char* pBytes, int nBytes);
	BOOL	OpenDgram(int port);
	void	CloseDgram();
	BOOL	WriteDgram(CString ip, char* pBytes, int nBytes, bool route);
	BOOL	StartAxis(int session, void* key, int mode);
	LRESULT	GetEnum(char* pBytes, int nBytes, int mode);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWrite(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVers(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSockOpen(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSockClose(WPARAM wParam, LPARAM lParam);
};


