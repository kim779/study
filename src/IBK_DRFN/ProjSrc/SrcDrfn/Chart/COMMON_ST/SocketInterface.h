// SocketInterface.h: interface for the CSokcetInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETINTERFACE_H__7FA0D7B6_02C2_495A_9CEB_22BE7DED93B9__INCLUDED_)
#define AFX_SOCKETINTERFACE_H__7FA0D7B6_02C2_495A_9CEB_22BE7DED93B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocketInterface  
{
public:
	CSocketInterface();
	virtual ~CSocketInterface();

	virtual void Connected() {};

	virtual void Accepted(CAsyncSocket* pSocket = NULL) {};
	virtual void ReceivedCommand(CAsyncSocket* pSocket, UINT nCommand, LPSTR lpszPacket = NULL, UINT nLen = 0) {};
	virtual void Received(LPVOID pBuffer, int nSize) {};

};

#endif // !defined(AFX_SOCKETINTERFACE_H__7FA0D7B6_02C2_495A_9CEB_22BE7DED93B9__INCLUDED_)
