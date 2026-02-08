// socket.cpp : implementation file
//

#include "stdafx.h"
#include "axisglb.h"
#include "socket.h"

#include "../../h/axisfm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Csocket

Csocket::Csocket(CWnd* parent)
{
	m_parent = parent;
	m_idx    = 0;
	m_rcvL   = 0;
	m_alive  = false;
	m_flow   = flowS;
	m_lock  = new CSingleLock(&m_event);
}

Csocket::~Csocket()
{
	delete m_lock;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(Csocket, CAsyncSocket)
	//{{AFX_MSG_MAP(Csocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// Csocket member functions

void Csocket::OnConnect(int nErrorCode) 
{
	if (nErrorCode == 0)	// success
	{
		m_alive = true;
		m_parent->SendMessage(WM_USER, MAKEWPARAM(sm_CONNECT, 0));
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void Csocket::OnClose(int nErrorCode) 
{
	m_alive = false;
	m_parent->PostMessage(WM_USER, MAKEWPARAM(sm_CLOSE, 0));	
	CAsyncSocket::OnClose(nErrorCode);
}

void Csocket::OnSend(int nErrorCode) 
{
	if (nErrorCode)
	{
		OnClose(nErrorCode);
		return;
	}
	m_event.SetEvent();	
	CAsyncSocket::OnSend(nErrorCode);
}

void Csocket::OnReceive(int nErrorCode) 
{
	if (nErrorCode)
	{
		OnClose(nErrorCode);
		return;
	}

	char	frameB[maxBUFs];
	int	frameL;
	struct	_fmH*	fmH;

	frameL = Receive(frameB, sizeof(frameB));
	switch (frameL)
	{
	case 0:
		OnClose(0);
		return;
	case SOCKET_ERROR:			// maybe does not happened
		return;
	default:
		break;
	}
	
	for (int ii = 0; ii < frameL; ii++)
	{
		switch (m_flow)
		{
		case flowS:			// start flag
			switch ((BYTE)frameB[ii])
			{
			case fmF_FS:
				m_flow = (_flow)m_idx;
				m_rcvB[m_idx++] = frameB[ii];
				break;
			default:
				m_idx = 0;
				break;
			}
			continue;

		case flowH:			// fmH
			m_rcvB[m_idx++] = frameB[ii];
			if (m_idx < L_fmH)
				continue;

			fmH = (struct _fmH *)m_rcvB;
			m_rcvL = atoi(CString(fmH->datL, sizeof(fmH->datL)));
			if (m_rcvL == 0)
			{
				m_flow = flowE;
				break;
			}

			m_flow = flowD;
			break;

		case flowD:			// Data
			m_rcvB[m_idx++] = frameB[ii];
			if ((int)(m_idx - L_fmH) >= m_rcvL)
				m_flow = flowE;
			break;
		}
		if (m_flow == flowE)		// end
		{
			m_flow = flowS;
			m_idx  = 0;
			if (m_rcvL > 0)
				m_parent->SendMessage(WM_USER, MAKEWPARAM(sm_RECEIVE, L_fmH+m_rcvL), (LPARAM)m_rcvB);
			break;
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

BOOL Csocket::Open(CString ips, int port)
{
	if (m_alive)
		return TRUE;

	if (Create())
	{
		int	value;

		value = TRUE;
		SetSockOpt(SO_KEEPALIVE, &value, sizeof(value));
		value = TRUE;
		SetSockOpt(SO_REUSEADDR, &value, sizeof(value));
		value = maxBUFs;
		SetSockOpt(SO_SNDBUF, &value, sizeof(value));
		value = maxBUFs;
		SetSockOpt(SO_RCVBUF, &value, sizeof(value));
		if (!Connect((LPCTSTR)ips, (UINT)port))
		{
			int	err = GetLastError();
			switch (err)
		//	switch (GetLastError())
			{
			case WSAEINPROGRESS:
			case WSAEWOULDBLOCK:
				break;
			default:
				ips.Format("ErrorCode = %d", err);
				AfxMessageBox(ips);
				return FALSE;
			}
		}
		return TRUE;
	}

	AfxMessageBox("fail to Create()");
	return FALSE;
}

void Csocket::Close()
{
	m_alive = false;

	ShutDown(2);
	CAsyncSocket::Close();
}

BOOL Csocket::Write(char* sndB, int sndL)
{
	if (!m_alive)
		return FALSE;

	for (int ii = 0; ;)
	{
		int nWritten = Send(&sndB[ii], sndL);
		if (nWritten == SOCKET_ERROR)
		{
			switch (GetLastError())
			{
			case WSAEINPROGRESS:
			case WSAEWOULDBLOCK:
				break;
			default:
				return FALSE;
			}
		}
		else
		{
			sndL -= nWritten;
			if (sndL == 0)
				break;
			ii += nWritten;
		}

		BOOL success = m_lock->Lock();
		m_lock->Unlock();
		if (!success)
			return FALSE;
	}

	return TRUE;
}

