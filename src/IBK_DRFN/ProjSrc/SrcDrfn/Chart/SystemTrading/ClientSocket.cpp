// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "systemtrading.h"
#include "ClientSocket.h"
#include "MainFrm.h"

#include "../Include_ST/SocketDefine.h"
#include "../Include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket() :
	m_lpszPacketData(NULL),
	m_lpszSendData(NULL),
	m_lpszTempPacketData(NULL),
	m_bDownloadingFile(FALSE),
	m_bDownloadingFileUF(FALSE),
	m_bDownloadingFileZip(FALSE),
	m_nPacketIndex(0),
	m_bStopCompiling(FALSE)
{
}

CClientSocket::~CClientSocket()
{
	if(m_lpszPacketData)
	{
		delete [] m_lpszPacketData;
		m_lpszPacketData = NULL;
	}

	if(m_lpszSendData)
	{
		delete [] m_lpszSendData;
		m_lpszSendData = NULL;
	}

	if(m_lpszTempPacketData)
	{
		delete [] m_lpszTempPacketData;
		m_lpszTempPacketData = NULL;
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::SendToServer(int nCommand, LPVOID lpData, int nDataSize, UINT nStrategyType/* = 0*/)
{
	if(!m_lpszSendData)
		m_lpszSendData = new char[MAX_FT_PACKET_LEN];

	ST_SENDDATA* pSendData = (ST_SENDDATA*)m_lpszSendData;
	pSendData->m_nCommand = nCommand;
	pSendData->m_nStrategyType = nStrategyType;
	::memcpy(pSendData->m_szData, lpData, nDataSize);
	pSendData->m_nDataSize = nDataSize;

	int nSize = sizeof(ST_SENDDATA) - sizeof(char) + pSendData->m_nDataSize + 5;
	::memcpy(m_lpszSendData + nSize - 5, ETX_PREV, 4);
	m_lpszSendData[nSize - 1] = ETX;

	int nOffset = 0;
	int nSendLen = 0;
	while(nSize > 0)
	{
		nSendLen = Send(m_lpszSendData + nOffset, nSize);
		if(nSendLen >= 0)
		{
			nSize -= nSendLen;
			nOffset += nSendLen;
		}
		else if(nSendLen == SOCKET_ERROR)
		{
			if(GetLastError() == WSAEWOULDBLOCK)
				Sleep(100);
		}
	}
}

void CClientSocket::OnReceive(int nErrorCode) 
{
	if(!m_lpszPacketData)
		m_lpszPacketData = new char[MAX_FT_PACKET_LEN];

	if(!m_lpszTempPacketData)
		m_lpszTempPacketData = new char[MAX_FT_PACKET_LEN];

	int nSize = Receive(m_lpszTempPacketData, MAX_FT_PACKET_LEN);

	for(int i = 0; i < nSize; i++)
	{
		if(IsEndPacket(i))
		{
			if(m_bDownloadingFile)
				WriteDllFile();
			else if(m_bDownloadingFileUF)
				WriteDllFileUF();
			else if(m_bDownloadingFileZip)
				WriteDllFileZip();
			else
				ProceedPacketData();

			m_nPacketIndex = 0;
		}
		else
			m_lpszPacketData[m_nPacketIndex++] = m_lpszTempPacketData[i];
	}

	CSocket::OnReceive(nErrorCode);
}

BOOL CClientSocket::IsEndPacket(int nIndex)
{
	if(nIndex < 5)
		return FALSE;

	if(m_lpszTempPacketData[nIndex] == ETX)
	{
		if(!::memcmp(m_lpszTempPacketData + nIndex - 5 + 1, ETX_PREV, 4))
			return TRUE;
	}

	return FALSE;
}

// 임시용
// 아이디로 변경요
CString CClientSocket::GetUserName()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	CString szIP;
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD( 2, 0 );
	
	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{
		
        if( gethostname ( name, sizeof(name)) == 0)
        {
			if((hostinfo = gethostbyname(name)) != NULL)
				// 외부 IP:로그인 할때 필요한 IP
				szIP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
		}
  		
        WSACleanup( );
	}
	return szIP;
}

void CClientSocket::ProceedPacketData()
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;

	ST_SENDDATA* pSendData = (ST_SENDDATA*)m_lpszPacketData;

	switch(pSendData->m_nCommand)
	{
	case SOCK_CMD_READYTOCOMPILE:
		{
			pMainFrame->SetConnectionStatusTest(_T("온라인"));
			CString strName = GetUserName();
			SendToServer(SOCK_CMD_READYTOCOMPILE_RESPONSE, (LPVOID)(LPCSTR)strName, strName.GetLength(), pSendData->m_nStrategyType);
		}
		break;
	case SOCK_CMD_EJECT_OVERUSER:
		pMainFrame->SetConnectionStatusTest(_T("오프라인"), RGB(255, 0, 0));
		AfxMessageBox(_T("사용자 초과"));
		break;
	case SOCK_CMD_START_RESPONSE:
		{
			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_STARTDLL);
			
			pMainFrame->SetConnectionStatusTest(_T("컴파일중"));
			pMainFrame->UploadCPPFile(pSendData->m_nStrategyType);

			pMainFrame->StartCompileProgress();
		}
		break;
	case SOCK_CMD_START_RESPONSE_DEBUG:
		{
			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_STARTDLL);
			
			pMainFrame->SetConnectionStatusTest(_T("컴파일중"));
			pMainFrame->UploadCPPFile_Debug(pSendData->m_nStrategyType);

			pMainFrame->StartCompileProgress();
		}
		break;
	case SOCK_CMD_DNLOAD_START:
		{
			pMainFrame->StopCompileProgress();

			pMainFrame->SetConnectionStatusTest(_T("수신중"));
			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->CreateNewDll(szFileName, pSendData->m_nStrategyType);

			m_bDownloadingFile = TRUE;

			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_CREATEFILE);
			SendToServer(SOCK_CMD_DNLOAD_START_RESPONSE, pSendData->m_szData, pSendData->m_nDataSize, pSendData->m_nStrategyType);
		}
		break;
	case SOCK_CMD_DNLOAD_START_DEBUG:
		{
			pMainFrame->StopCompileProgress();

			pMainFrame->SetConnectionStatusTest(_T("수신중"));
			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->CreateNewDll_Debug(szFileName, pSendData->m_nStrategyType);

			m_bDownloadingFile = TRUE;

			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_CREATEFILE);
			SendToServer(SOCK_CMD_DNLOAD_START_RESPONSE_DEBUG, pSendData->m_szData, pSendData->m_nDataSize, pSendData->m_nStrategyType);
		}
		break;
	case SOCK_CMD_DNLOAD_FILE:
		{
//			m_bDownloadingFile = FALSE;
//			pMainFrame->WriteEndDllFile();
//			pMainFrame->WriteDllFile(pSendData->m_szData, pSendData->m_nDataSize);
			pMainFrame->SetConnectionStatusTest(_T("온라인"));
		}
		break;
	case SOCK_CMD_TIMEOUT:
		break;
	case SOCK_CMD_COMPILE_ERROR:
		{
			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetOutputText(CM_MAKE_FAILED_STRATEGY, szFileName);
			pMainFrame->SetConnectionStatusTest(_T("온라인"));

			pMainFrame->StopCompileProgress();
			pMainFrame->m_bCompiling = FALSE;

			pMainFrame->m_ctrlCompileProgress.ShowWindow(SW_HIDE);
			SendToServer(SOCK_CMD_END_COMPILE, NULL, 0);
		}
		break;
	case SOCK_CMD_START_RESPONSE_UF:
		{
			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_STARTDLL);

			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetConnectionStatusTest(_T("컴파일중"));
			pMainFrame->UploadUserFunctionCPPFile(szFileName);

			pMainFrame->StartCompileProgress();
		}
		break;
	case SOCK_CMD_START_RESPONSE_UF_DEBUG:
		{
			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_STARTDLL);

			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetConnectionStatusTest(_T("컴파일중"));
			pMainFrame->UploadUserFunctionCPPFile(szFileName);

			pMainFrame->StartCompileProgress();
		}
		break;
	case SOCK_CMD_DNLOAD_START_UF:
		{
			pMainFrame->StopCompileProgress();

			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetConnectionStatusTest(_T("수신중"));
			pMainFrame->CreateNewDllFunc(szFileName);

			m_bDownloadingFileUF = TRUE;

			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_CREATEFILE);
			SendToServer(SOCK_CMD_DNLOAD_START_RESPONSE_UF, pSendData->m_szData, pSendData->m_nDataSize, pSendData->m_nStrategyType);
		}
		break;
	case SOCK_CMD_DNLOAD_START_UF_DEBUG:
		{
			pMainFrame->StopCompileProgress();

			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetConnectionStatusTest(_T("수신중"));
			pMainFrame->CreateNewDllFunc_Debug(szFileName);

			m_bDownloadingFileUF = TRUE;

			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_CREATEFILE);
			SendToServer(SOCK_CMD_DNLOAD_START_RESPONSE_UF_DEBUG, pSendData->m_szData, pSendData->m_nDataSize, pSendData->m_nStrategyType);
		}
		break;
	case SOCK_CMD_DNLOAD_FILE_UF:
		{
//			pMainFrame->WriteDllFileFunc(pSendData->m_szData, pSendData->m_nDataSize);
			pMainFrame->SetConnectionStatusTest(_T("온라인"));
		}
		break;
	case SOCK_CMD_TIMEOUT_UF:
		break;
	case SOCK_CMD_COMPILE_ERROR_UF:
		{
			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetOutputText(CM_MAKE_FAILED_FUNCTION, szFileName);
			pMainFrame->SetConnectionStatusTest(_T("온라인"));

			pMainFrame->StopCompileProgress();
			pMainFrame->m_bCompiling = FALSE;

			pMainFrame->m_ctrlCompileProgress.ShowWindow(SW_HIDE);
			SendToServer(SOCK_CMD_END_COMPILE, NULL, 0);
		}
		break;
	case SOCK_CMD_START_RESPONSE_ALL:
		{
			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_STARTDLL_ZIP);

			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetConnectionStatusTest(_T("컴파일중"));
			pMainFrame->UploadZipFile(szFileName);

			pMainFrame->StartCompileProgressZip();
		}
		break;
	case SOCK_CMD_DNLOAD_START_ALL:
		{
			pMainFrame->StopCompileProgressZip();

			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetConnectionStatusTest(_T("수신중"));
			pMainFrame->CreateNewZip(szFileName);

			m_bDownloadingFileZip = TRUE;
			pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_CREATEFILE);

			SendToServer(SOCK_CMD_DNLOAD_START_RESPONSE_ALL, pSendData->m_szData, pSendData->m_nDataSize, pSendData->m_nStrategyType);
		}
		break;
	case SOCK_CMD_EXTRACT_ERROR_ALL:
	case SOCK_CMD_COMPRESS_ERROR_ALL:
		{
			char szFileName[MAX_PATH] = {NULL, };
			::memcpy(szFileName, pSendData->m_szData, pSendData->m_nDataSize);

			pMainFrame->SetConnectionStatusTest(_T("온라인"));
			pMainFrame->DisplayZipError(szFileName);

			pMainFrame->StopCompileProgressZip();
			pMainFrame->m_bCompiling = FALSE;

			pMainFrame->m_ctrlCompileProgress.ShowWindow(SW_HIDE);
			SendToServer(SOCK_CMD_END_COMPILE, NULL, 0);
		}
		break;
	case SOCK_CMD_STOPCOMPILING_RESPONSE:
			pMainFrame->SetConnectionStatusTest(_T("온라인"));
			SendToServer(SOCK_CMD_END_COMPILE, NULL, 0);
		break;	
	case SOCK_CMD_SERVERMESSAGE:
		{
			char szMessage[101] = {NULL, };

			char szLevel[2] = {NULL, };
			*szLevel = *pSendData->m_szData;
			UINT nLevel = atoi(szLevel);
			::memcpy(szMessage, pSendData->m_szData + 1, pSendData->m_nDataSize - 1);

			pMainFrame->SetServerMessage(szMessage, nLevel);
		}
	case SOCK_CMD_POLLINGMASSAGE:
			SendToServer(SOCK_CMD_POLLINGMASSAGE, NULL, 0);
		break;
	case SOCK_CMD_CLOSE_IDLETIME:
			pMainFrame->SetServerMessage(_T("1시간동안 아무런 동작이 없어서 서버와의 연결을 종료합니다."), SVRMSG_ALERT);
		break;
	}
}

void CClientSocket::WriteDllFile()
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;

	pMainFrame->m_ctrlCompileProgress.SetPos(COMPILE_PROGRESS_COMPLETE);
	pMainFrame->WriteDllFile(m_lpszPacketData, m_nPacketIndex);
//	pMainFrame->WriteEndDllFile();

	m_bDownloadingFile = FALSE;
	SendToServer(SOCK_CMD_END_COMPILE, NULL, 0);
}

void CClientSocket::WriteDllFileUF()
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
	pMainFrame->WriteDllFileFunc(m_lpszPacketData, m_nPacketIndex);
//	pMainFrame->WriteEndDllFile();

	m_bDownloadingFileUF = FALSE;
	SendToServer(SOCK_CMD_END_COMPILE, NULL, 0);
}

void CClientSocket::WriteDllFileZip()
{
	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
	pMainFrame->WriteZipFile(m_lpszPacketData, m_nPacketIndex);
//	pMainFrame->WriteEndDllFile();

	m_bDownloadingFileZip = FALSE;
	SendToServer(SOCK_CMD_END_COMPILE, NULL, 0);
}

void CClientSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnConnect(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode) 
{
	CSocket::OnClose(nErrorCode);

	CMainFrame* pMainFrame = (CMainFrame*)theApp.m_pMainWnd;
	pMainFrame->CloseClientSocket();
}

void CClientSocket::StopCompiling()
{
	m_bDownloadingFile = m_bDownloadingFileUF = m_bDownloadingFileZip = FALSE;
	m_nPacketIndex = 0;
}
