// SystemTradingSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SystemTradingSocket.h"
#include "SocketInterface.h"
#include "RcvSocket.h"
#include "../include_ST/MessageDefine.h"
#include "../include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingSocket

CSystemTradingSocket::CSystemTradingSocket(CSocketInterface* pInterface) :
	m_pInterface(pInterface)
{
}

CSystemTradingSocket::~CSystemTradingSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSystemTradingSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CSystemTradingSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingSocket member functions

void CSystemTradingSocket::OnConnect(int nErrorCode) 
{
	m_pInterface->Connected();
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CSystemTradingSocket::OnAccept(int nErrorCode) 
{
	CRcvSocket*	pRcvSocket = new CRcvSocket(m_pInterface);

	if(Accept(*pRcvSocket))
	{
		m_pInterface->Accepted(pRcvSocket);
	}
	else
		delete pRcvSocket;

	CAsyncSocket::OnAccept(nErrorCode);
}

void CSystemTradingSocket::OnReceive(int nErrorCode) 
{
/*
	if(nErrorCode == 0)
	{
		char szPacket[1024];
		int nLen = Receive(szPacket, 1024);

		if(nLen <= 0)
			return;

		// Command Ã³¸®
		if(::memcmp(szPacket, _T("#ST#"), 4) == 0)
		{
			if(::memcmp(szPacket, SOCK_CMD_START_RESPONSE, SIZE_SOCK_CMD_START_RESPONSE) == 0)
			{
//				m_pInterface->ReceivedCommand(this, 0, szPacket, nLen);
			}
		}
	}
*/

	CAsyncSocket::OnReceive(nErrorCode);
}

void CSystemTradingSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnClose(nErrorCode);
}
