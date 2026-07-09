// NSocket.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "NSocket.h"
#include "axmsg.hxx"
#include "../h/axis.h"
#include "../h/axisfm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNSocket

CNSocket::CNSocket(CWnd* parent)
{
	m_parent = parent;
	m_flow = flowSTX;
	m_step = flowSTX1;
}

CNSocket::~CNSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CNSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CNSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CNSocket member functions

void CNSocket::OnClose(int nErrorCode) 
{
	m_parent->PostMessage(WM_AXIS, MAKEWPARAM(axCLOSESOCK, 0));
	CAsyncSocket::OnClose(nErrorCode);
}

void CNSocket::OnConnect(int nErrorCode) 
{
	if (nErrorCode == ERROR_SUCCESS)
	{
		m_nBytes = 0;
		m_parent->PostMessage(WM_AXIS, MAKEWPARAM(axCONNECTSOCK, 0));
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void CNSocket::OnReceive(int nErrorCode) 
{
	unsigned char fflag[2];
	char	frameB[4096];
	int	frameL;

	frameL = Receive(frameB, sizeof(frameB));
	switch (frameL)
	{
	case 0:
		OnClose(0);
		return;
	case SOCKET_ERROR:
		return;
	default:
		break;
	}

	int	len = L_fmH + L_axisH + L_mod;
	for (int ii = 0; ii < frameL; ii++)
	{
		
		m_pBytes[m_nBytes++] = frameB[ii];
/*		if (m_nBytes >= len)
		{
			m_flow = flowSTX;
			m_parent->SendMessage(WM_AXIS, MAKEWPARAM(axRECEIVESOCK, 0));
		}*/
	
		if (m_nBytes >= 2 && m_flow == flowSTX)
		{
			fflag[0] = m_pBytes[0];
			fflag[1] = m_pBytes[1];

			if (fflag[0] == 0xfe && fflag[1] == 0xfe)
				m_flow = flowETX;
			else	m_nBytes = 0;
		}

		if (m_nBytes >= len && m_flow == flowETX)
		{
			m_flow = flowSTX;
			m_parent->SendMessage(WM_AXIS, MAKEWPARAM(axRECEIVESOCK, 0));
			break;
		}
		continue;

		switch (m_flow)
		{
		case flowSTX:
			if (m_pBytes[0] != 0xfe)
			{
				m_nBytes = 0;
				continue;
			}
			else if (m_pBytes[1] != 0xfe)
			{
				m_nBytes = 0;
				continue;
			}
			else	m_flow   = flowETX;
			break;
		case flowETX:
			if (m_nBytes >= len)
			{
				m_flow = flowSTX;
				m_parent->SendMessage(WM_AXIS, MAKEWPARAM(axRECEIVESOCK, 0));
			}
			break;
		}
		
/*		switch (m_flow)
		{
		case flowSTX:
			if (frameB[ii] == STX)
			{
				m_nBytes = 0;
				m_flow   = flowETX;
			}
			break;
		case flowETX:
			switch (frameB[ii])
			{
			case ETX:
				m_parent->SendMessage(WM_AXIS, MAKEWPARAM(axRECEIVESOCK, 0));
				m_flow = flowSTX;
				break;
			case STX:
				m_nBytes = 0;
				break;
			default:
				m_pBytes[m_nBytes++] = frameB[ii];
				break;
			}
			break;
		}//*/
	}
	
	CAsyncSocket::OnReceive(nErrorCode);
}
