// wsock.cpp: implementation of the CwSock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wsock.h"
#include "../mapvar.h"
#include "../../h/axisfire.h"
#include "../../h/axisfm.h"
#include "../../h/axisvar.h"
#include "../mainvar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CwSock, CWnd)
	//{{AFX_MSG_MAP(CwSock)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CwSock, CWnd)
	//{{AFX_EVENTSINK_MAP(CWnd)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CwSock, -1, 1, OnFireEvent, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CwSock::CwSock(CWnd* pWnd)
{
	m_wnd  = pWnd;
	m_sock = -1;

	m_sendB = (char *) NULL;
	m_recvB = (char *) NULL;

	m_ips  = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
	m_sRoot = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
	m_port = WRK_PORT;
	Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), pWnd, 0);
}

CwSock::~CwSock()
{
	if (m_pSockCtrl)
		delete m_pSockCtrl;

	if (m_sendB)	delete[] m_sendB;
	if (m_recvB)	delete[] m_recvB;
}

bool CwSock::Open(CString ips, int port)
{
	BOOL retvalue =false;
	m_pSockCtrl = new CWnd();
	//if (!m_pSockCtrl->CreateControl("AxisSock.SockCtrl.WOW10", NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 1))
	if (!m_pSockCtrl->CreateControl("AxisSock.SockCtrl.AX2010", NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 1))
	{
		AfxMessageBox("Create AxSock.ocx : failed");
		return false;
	}

	if (!ips.IsEmpty())
		m_ips = ips;
	m_port = (port < 0) ? m_port : port;

	if (m_ips.IsEmpty())
	{
		m_wnd->SendMessage(WM_STATE, stADDRESS, (LPARAM) 0);
		return false;
	}

	m_pSockCtrl->InvokeHelper(DI_OPEN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_BSTR VTS_BSTR VTS_I4), m_sRoot, m_ips, m_port);
	if (!retvalue)
	{
		m_sock = -1;
		return false;
	}
	m_sock = 1;

	if (m_sendB)
		delete[] m_sendB;
	if (m_recvB)
		delete[] m_recvB;

	m_sendB = new char[MAX_IO];
	m_recvB = new char[MAX_IO];
	ZeroMemory(m_sendB, MAX_IO);
	ZeroMemory(m_recvB, MAX_IO);

	return true;
}

bool CwSock::Write(int size)
{
	if (m_sock < 0)
		return false;
	BOOL retvalue;
	ZeroMemory(m_recvB, MAX_IO);

	m_pSockCtrl->InvokeHelper(DI_WRITE, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4), (long)m_sendB, size);
	return (retvalue?true:false);
}

void CwSock::Close()
{
	if (m_sock >= 0)
		m_pSockCtrl->InvokeHelper(DI_CLOSE, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, NULL);
}

void CwSock::OnFireEvent(long type, long pBytes, long nBytes)
{
	switch(type)
	{
	case FEV_OPEN:
		if (nBytes == 0) //success
		{
			BOOL retvalue;
			m_rtnStr = "AXIS";
			m_pSockCtrl->InvokeHelper(DI_AXIS, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4), (long)m_rtnStr.operator LPCTSTR(), modeWM);

			if (retvalue)
				m_wnd->PostMessage(WM_OPENEDSOCK, 0, 1);
		}
		else
		{
			m_wnd->PostMessage(WM_OPENEDSOCK, 0, 0);
			m_rtnStr = "Socket Error!";
			SendMessage(WM_STATE, stIDLE, (LPARAM)(char *)m_rtnStr.operator LPCTSTR());
		}
		break;

	case FEV_RUN:
		break;
	case FEV_AXIS:
		if (nBytes)
		{
			CopyMemory(m_recvB, (char*)pBytes, nBytes);
			ZeroMemory(m_sendB, MAX_IO);
			m_wnd->SendMessage(WM_READDATA);
		}
		break;
	case FEV_CLOSE:
		break;
	case FEV_ERROR:
		break;
	}
}

void CwSock::OnDestroy()
{
	if (m_sock >= 0)
		m_pSockCtrl->InvokeHelper(DI_CLOSE, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, NULL);
	CWnd::OnDestroy();
}