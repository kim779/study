// wSock.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wSock.h"

#include "../h/mapvar.h"
#include "../h/mainvar.h"
//#include "../../h/axisfire.h"
//#include "../../h/axisfm.h"
//#include "../../h/axisvar.h"
#include "../../h/Sock.h"
//#include "../../h/msg.h"

// CwSock

#define	WRK_PORT	 15005//15105

IMPLEMENT_DYNAMIC(CwSock, CWnd)

CwSock* CwSock::m_wizard;

CwSock::CwSock(CWnd*pWnd)
{
	m_wnd   = pWnd;
	m_alive = false;

	m_recvB = NULL;
	m_sock  = NULL;
	m_iSession = 0;
	m_wizard = NULL;

	Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), pWnd, 0);
	m_wizard = this;
}

CwSock::~CwSock()
{
	if (m_sock)
		delete m_sock;

	if (m_recvB)
		delete[] m_recvB;
}


BEGIN_MESSAGE_MAP(CwSock, CWnd)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CwSock 메시지 처리기입니다.


void CwSock::OnDestroy()
{
	if (m_alive)
		m_sock->CloseSock(m_iSession);

	CWnd::OnDestroy();
}

void CwSock::sockEvent(int type, char* pBytes, int nBytes, int auxs)
{
	if (m_wizard == NULL)
		return;

	m_wizard->OnSockEvent(type, pBytes, nBytes, auxs);
}

void CwSock::OnSockEvent(int type, char* pBytes, int nBytes, int auxs)
{
	//Cmsg	msg;

	switch (type)
	{
	case smOPEN:
		if (nBytes == 0) //success
		{
			BOOL	retvalue = false;

			m_rtnStr = _T("AXIS");
			retvalue = m_sock->StartAxis(m_iSession, (void*)m_rtnStr.operator LPCTSTR(), modeWM);

			if (retvalue)
			{
				m_alive = true;
				m_wnd->PostMessage(WM_OPENEDSOCK, 0, 1);
			}
		}
		else
		{
			m_wnd->PostMessage(WM_OPENEDSOCK, 0, 0);
			m_rtnStr = _T("Socket Error!");
			SendMessage(WM_STATE, stIDLE, (LPARAM)(char *)m_rtnStr.operator LPCTSTR());
		}
		break;
	//case smCLOSE:
	//case smRUN:
	//case smSIZE:
	//case smVERS:
	//case smANM:
	case smAXIS:
		if (nBytes)
		{
			CopyMemory(m_recvB, pBytes, nBytes);
			m_wnd->SendMessage(WM_READDATA, (WPARAM)nBytes, (LPARAM)pBytes);
		}
		break;
	//case smSTAT:
	//case smPUSH:
	//case smERROR:
	}
}

bool CwSock::Open(CString ips, int port)
{
	CString	root;
	BOOL	retvalue = false;

	m_sock = new CSock();
	if (!(m_sock->Create(NULL, NULL, 0, CRect(0, 0, 0, 0), this, -2)))
	{
		AfxMessageBox(_T("Create axsock.dll : failed"));
		return false;
	}

	m_sock->SetCallback(&CwSock::sockEvent);

	if (ips.IsEmpty())
		ips = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
	if (port < 0)
		port = WRK_PORT;

	if (ips.IsEmpty())
	{
		m_wnd->SendMessage(WM_STATE, stADDRESS, (LPARAM) 0);
		return false;
	}

	root = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
	retvalue = m_sock->OpenSock(m_iSession, root, ips, port);
	if (!retvalue)
	{
		m_alive = false;
		return false;
	}

	if (m_recvB)
		delete[] m_recvB;

	m_recvB = new char[MAX_IO];
	ZeroMemory(m_recvB, MAX_IO);

	return true;
}


BOOL CwSock::Write(char* pBytes, int nBytes)
{
	if (!m_alive)
		return FALSE;

	BOOL	retvalue = false;

	ZeroMemory(m_recvB, MAX_IO);
	retvalue = m_sock->WriteStream(m_iSession, pBytes, nBytes);
	return retvalue;
}

void CwSock::Close()
{
	if (m_alive)
		m_sock->CloseSock(m_iSession);
}
