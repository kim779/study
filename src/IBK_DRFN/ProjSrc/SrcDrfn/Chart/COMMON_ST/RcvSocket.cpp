// RcvSocket.cpp : implementation file
//

#include "stdafx.h"
#include "RcvSocket.h"
#include "SocketInterface.h"
#include "../include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRcvSocket

CRcvSocket::CRcvSocket(CSocketInterface* pInterface):
	m_pInterface(pInterface)
{
}

CRcvSocket::~CRcvSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CRcvSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CRcvSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CRcvSocket member functions

void CRcvSocket::OnReceive(int nErrorCode) 
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
			if(::memcmp(szPacket, SOCK_CMD_START, SIZE_SOCK_CMD_START) == 0)
			{

				m_pInterface->ReceivedCommand(this, 0, szPacket, nLen);
			}
		}

	}
*/

//	CAsyncSocket::OnReceive(nErrorCode);
}

void CRcvSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnClose(nErrorCode);
}
