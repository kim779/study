#if !defined(AFX_STREAM_H__86E647CF_8F00_459B_9975_7EF8551FA219__INCLUDED_)
#define AFX_STREAM_H__86E647CF_8F00_459B_9975_7EF8551FA219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Stream.h : header file
//
#include <afxmt.h>
#include "../h/axisfm.h"

/////////////////////////////////////////////////////////////////////////////
// CStream command target

class CStream : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CStream(class CWsh* rsm, class CWsh* wsh);
	virtual ~CStream();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStream)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CStream)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
public:
	bool	OpenStream(char* ip, int port);
	void	CloseStream();
	bool	RunStream(int ssm);
	bool	RunVersion(char* keys, int size);
	bool	WriteData(char* data, int ndat, bool wait = false);

private:
	bool	ParseFrame();
	bool	ResponseFM(unsigned char fmC);
	bool	WriteBytes(char* pBytes, int nBytes);

protected:
	bool	m_alive;
	int	m_chainL;
	char	*m_chain;

	class CWsh*	m_rsm;
	class CWsh*	m_wsh;
	class CCompress* m_zip;

#ifdef	_DEBUG
	class CLog*	m_log;
#endif

private:
	int	m_nBytesToResponse;	// frame response (ACK, NAK, POLL, etc)
	char	m_pBytesToResponse[L_fmH+1];

	int	m_nBytesToData;		// frame data
	char	m_pBytesToData[L_fmH+maxIOs+1];

	int	m_rcvL;			// frame data length
	char	m_rcvB[L_fmH+maxIOs+1];	// fmH + data + BCC

	int	m_frameL;
	char	m_frameB[4096];		// receive packet frame buffer

	int	m_idx;			// receive buffer index
	char	*m_ptr;			// receive buffer pointer
	_fmH	*fmH;			// frame header
	int	m_ssm;			// session

	enum	fmFlow
		{_frameF_ = 0, _frameH_, _frameD_, _frameBCC_, _frameE_};
	fmFlow	m_flow;			// frame flow

	CEvent	m_event;
	CSingleLock*	m_lock;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STREAM_H__86E647CF_8F00_459B_9975_7EF8551FA219__INCLUDED_)
