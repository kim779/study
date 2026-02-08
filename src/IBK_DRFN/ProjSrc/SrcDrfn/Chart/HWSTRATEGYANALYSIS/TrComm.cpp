// DeskHelper.cpp : implementation file
//

#include "stdafx.h"
#include "TrComm.h"

//-----------------------------------------------------------------------------
// Author		: Young-Hee, Lee	Date :2005/4/13
// Reason		: 
//-----------------------------------------------------------------------------
//start
#include "../../../New/include/CommonDef.h"			//KJI 20030305 Add
#include "../../../New/CommonLib/ServerMsg.h"
//end

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		TID_WAITCHK				103
#define		CHECKSTATE(xxCallback, xxState)	if(xxCallback)	xxCallback->StatusChange(xxState);

/////////////////////////////////////////////////////////////////////////////
// CTRComm
BEGIN_MESSAGE_MAP(CTRComm, CWnd)
	//{{AFX_MSG_MAP(CTRComm)
	ON_WM_COPYDATA()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// * 파라미터 추가(2002/05/09, 최유강)
// * bWait : 종료 tr send시엔 기다리지 않고 바로 리턴
BOOL CTRComm::Send2Server(LPCSTR szData, int nLen, BOOL bWait/*=TRUE*/, LPCTSTR szHeader/*=""*/)
{
	if(!m_hWnd)
		return ICBS_CREATEERR;
	if(!m_hSocket)
		return ICBS_PARAMERR;
	
/*
	int nSize = sizeof(STTYPE400) -3 + nLen;

	STTYPE400* pTrData = (STTYPE400*)malloc(nSize+1); 
	memset(pTrData, 0x20, nSize);

	pTrData->bEnc = m_bEnc;
	pTrData->nLen = nSize;
	pTrData->trHeader.WindowID = (long)m_hWnd;
	memcpy(pTrData->trHeader.TRCode, m_szTR, m_szTR.GetLength());
	memcpy(pTrData->trHeader.TRData, szData, nLen);
	pTrData->trHeader.TRData[nLen] = 0;

	m_nWaitResult = WRET_READY;

	// TYPE400: 일반 처리	TYPE410 : 공인인증 거친다. 
	DWORD dwType = (m_bCert) ? TYPE410 : TYPE400;
*/

	CServerMsg msg;
	msg.AddString(m_szTR, QRYCODE_LEN);	// TRCode
	// 이전이후(1) + 암호와여부(1) + 목적지구분(1) + 0000
	// 목적지 구분  '1':서버 '2':원장서버 '3' 마스터서버
	if(strlen(szHeader) == 8)
		msg.AddString(szHeader, 8);	
	else
		msg.AddString("N0010000", 8);	

	msg.AddLong(nLen);	
	msg.AddString(szData, nLen);
	
	int nDataSize = msg.GetLength();
	LPBYTE pData = new BYTE[nDataSize+4];
	CopyMemory(pData, &m_hWnd, 4);
	CopyMemory(pData+4, msg.GetData(), nDataSize);

	BOOL bRet = RealSend(m_hSocket, (void*)pData, (LPARAM)nDataSize, 0);

	delete [] pData;
//	free(pTrData);

	if(bWait)
	{
		WaitResult();
	}

	return m_nWaitResult;
}

BOOL CTRComm::Send2Server_Buf(LPCSTR szData, int nLen, BOOL bWait/*=TRUE*/, LPCTSTR szHeader/*=""*/)
{
	if(!m_hWnd)
	return ICBS_CREATEERR;
	if(!m_hSocket)
	return ICBS_PARAMERR;

	/*
	int nSize = sizeof(STTYPE400) -3 + nLen;

	STTYPE400* pTrData = (STTYPE400*)malloc(nSize+1); 
	memset(pTrData, 0x20, nSize);

	pTrData->bEnc = m_bEnc;
	pTrData->nLen = nSize;
	pTrData->trHeader.WindowID = (long)m_hWnd;
	memcpy(pTrData->trHeader.TRCode, m_szTR, m_szTR.GetLength());
	memcpy(pTrData->trHeader.TRData, szData, nLen);
	pTrData->trHeader.TRData[nLen] = 0;

	m_nWaitResult = WRET_READY;

	// TYPE400: 일반 처리 TYPE410 : 공인인증 거친다. 
	DWORD dwType = (m_bCert) ? TYPE410 : TYPE400;
	*/

	CServerMsg msg;
	msg.AddString(m_szTR, QRYCODE_LEN); // TRCode
	// 이전이후(1) + 암호와여부(1) + 목적지구분(1) + 0000
	// 목적지 구분  '1':서버 '2':원장서버 '3' 마스터서버 '8':조건검색
	if(strlen(szHeader) == 8)
	msg.AddString(szHeader, 8); 
	else
	msg.AddString("N0010000", 8); 

	msg.AddLong(nLen); 

	int nDataSize;
	LPBYTE pData = NULL;

	int nMsgSize = msg.GetLength();
	nDataSize =  nMsgSize + nLen;
	pData = new BYTE[nDataSize+4];
	CopyMemory(pData, &m_hWnd, 4);
	CopyMemory(pData+4, msg.GetData(), nMsgSize);
	CopyMemory(pData+4+nMsgSize, szData, nLen);

	BOOL bRet = RealSend(m_hSocket, (void*)pData, (LPARAM)nDataSize, 0);

	//디버그
	CString strTemp;
	strTemp.Format("%s(%d): %s\r\n", m_szTR, nLen, szData);
	OutputDebugString(strTemp);
	delete [] pData;
	// free(pTrData);

	if(bWait)
	{
		WaitResult();
	}

	return m_nWaitResult;
}

#ifndef UDM_MAKE_AUTHDATA
	#define UDM_MAKE_AUTHDATA     WM_USER+606  // MakeAuthData
#endif

#ifdef __INFO
BOOL CTRComm::Send2Gateway(LPCSTR szData, int nLen, CDBMgr* pDBMgr, BOOL bWait/*=FALSE*/, BOOL bNext/*=FALSE*/)
{
	if(!m_hWnd)
		return ICBS_CREATEERR;
	if(!m_hSocket)
		return ICBS_PARAMERR;

	CServerMsg msg;

	msg.AddString(m_szTR, QRYCODE_LEN);	// TRCode
	// 수익증권여부(1) + 이전이후(1) + 암호와여부(1) + 목적지구분(1) + 0000
	// 목적지 구분  '1':서버 '2':원장서버 '3' 마스터서버
	BYTE ucFundAccount = 'N';
	msg.AddByte(ucFundAccount);
	if(bNext)
		msg.AddByte('2');
	else
		msg.AddByte('0');
	msg.AddString("060000", 6);		//조회+암호화+원장서버
	msg.AddLong(nLen);	
	msg.AddString(szData, nLen);

	int nDataSize = msg.GetLength();
	LPBYTE pData = new BYTE[nDataSize+4];
	CopyMemory(pData, &m_hWnd, 4);
	CopyMemory(pData+4, msg.GetData(), nDataSize);

	BOOL bRet = RealSend(m_hSocket, (void*)pData, (LPARAM)nDataSize, 0, TRUE);

	delete [] pData;
	if(bWait)
	{
		WaitResult();
	}
	return m_nWaitResult;
}
#else
BOOL CTRComm::Send2Gateway(LPCSTR szData, int nLen, CDBMgr* pDBMgr, BOOL bWait/*=FALSE*/, BOOL bNext/*=FALSE*/)
{
	if(!m_hWnd)
		return ICBS_CREATEERR;
	if(!m_hSocket)
		return ICBS_PARAMERR;

	CServerMsg msg;

	msg.AddString(m_szTR, QRYCODE_LEN);	// TRCode
	// 수익증권여부(1) + 이전이후(1) + 암호와여부(1) + 목적지구분(1) + 0000
	// 목적지 구분  '1':서버 '2':원장서버 '3' 마스터서버
	BYTE ucFundAccount = 'N';
	msg.AddByte(ucFundAccount);
	if(bNext)
		msg.AddByte('2');
	else
		msg.AddByte('0');
	msg.AddString("120000", 6);		//조회+암호화+원장서버
	msg.AddLong(nLen);	
	msg.AddString(szData, nLen);
	
//KJI 20021203 Add Start
	CLogUserInfo*  pLogUserInfo = NULL;
	pLogUserInfo = pDBMgr->GetLogUserData();
	
	if( pLogUserInfo->ucCertUse == 'Y')
	{
		CServerMsg* pTmp=NULL;
		
		pTmp = (CServerMsg*)AfxGetMainWnd()->SendMessage(UDM_MAKE_AUTHDATA , (WPARAM)(long)&msg ,(LPARAM)(long)m_hWnd);
		
		//if( pMain->MakeAuthData(&msg, FALSE, m_hWnd))
		if(pTmp)
		{
				int nDataSize = msg.GetLength();
				LPBYTE pData = new BYTE[nDataSize+4];
				CopyMemory(pData, &m_hWnd, 4);
				CopyMemory(pData+4, msg.GetData(), nDataSize);

				BOOL bRet = RealSend(m_hSocket, (void*)pData, (LPARAM)nDataSize, 0);
				delete [] pData;
		}
	}
	else
	{
		int nDataSize = msg.GetLength();
		LPBYTE pData = new BYTE[nDataSize+4];
		CopyMemory(pData, &m_hWnd, 4);
		CopyMemory(pData+4, msg.GetData(), nDataSize);

		BOOL bRet = RealSend(m_hSocket, (void*)pData, (LPARAM)nDataSize, 0);
		delete [] pData;
	}
	if(bWait)
	{
		WaitResult();
	}
	return m_nWaitResult;
}
#endif //__INFO
//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/13
// Comments		: 한화 스타일로 보낼때 쓴다.
//-----------------------------------------------------------------------------
// HanWha TR 이용시
BOOL CTRComm::Send2HanWha(void* pRecData, int nLen)
{
	if(!m_hWnd)
		return ICBS_CREATEERR;
	if(!m_hSocket)
		return ICBS_PARAMERR;
	
	CServerMsg* pMsg = NULL;
	pMsg = (CServerMsg*)pRecData;
	
	int nDataSize = pMsg->GetLength();
	int nSize = nDataSize + 4;
	LPBYTE pData = new BYTE[nSize];
	CopyMemory(pData, &m_hWnd, 4);
	CopyMemory(pData+4,pMsg->GetData(),nDataSize);
	
	BOOL bRet = RealSend(m_hSocket, (void*)pData, (LPARAM)nSize);
	free(pData);

/*	if(bWait)
	{
		WaitResult();
	}
*/
	return m_nWaitResult;
}

// 관심 데이터 
BOOL CTRComm::Send2IntData(void* pRecData, int nLen, CWnd* pSendWnd, int nType)
{
	if(!m_hWnd)
		return ICBS_CREATEERR;
	if(!m_hSocket)
		return ICBS_PARAMERR;
	
	CServerMsg* pMsg = NULL;

	pMsg = new CServerMsg;
	pMsg->AddString(m_szTR, QRYCODE_LEN);	// TRCode
	// 이전이후(1) + 암호와여부(1) + 목적지구분(1) + 0000
	// 목적지 구분  '1':서버 '2':원장서버 '3' 마스터서버
	pMsg->AddString("N0010000", 8);	
	pMsg->AddLong(nLen+4);	
	pMsg->AddLong((long)pSendWnd); // 빈공간 설정.
	LPCSTR strData = (LPCSTR)pRecData;
	pMsg->AddString((LPCSTR)strData,nLen);

	
	int nDataSize = pMsg->GetLength();
	int nSize = nDataSize + 4;
	LPBYTE pData = new BYTE[nSize];
	CopyMemory(pData, &m_hWnd, 4);
	CopyMemory(pData+4,pMsg->GetData(),nDataSize);

	//받을 아이디를 넘겨 준다.
	CopyMemory(pData+24, &pSendWnd, 4);
	
	BOOL bRet = RealSend(m_hSocket, (void*)pData, (LPARAM)nSize);
	free(pData);

	delete pMsg;
/*	if(bWait)
	{
		WaitResult();
	}
*/
	return m_nWaitResult;
}

BOOL CTRComm::Send2Direct(LPCSTR szData, LPCSTR szHeader, int nLen, BOOL bWait/*=TRUE*/)
{
	if(!m_hWnd)
		return ICBS_CREATEERR;
	if(!m_hSocket)
		return ICBS_PARAMERR;

	CServerMsg msg;
	msg.AddString(m_szTR, QRYCODE_LEN);	// TRCode
	// 이전이후(1) + 암호와여부(1) + 목적지구분(1) + 0000
	// 목적지 구분  '1':서버 '2':원장서버 '3' 마스터서버
	msg.AddString(szHeader, 8);	
	msg.AddLong(nLen);	
	msg.AddString(szData, nLen);
	
	int nDataSize = msg.GetLength();
	LPBYTE pData = new BYTE[nDataSize+4];
	CopyMemory(pData, &m_hWnd, 4);
	CopyMemory(pData+4, msg.GetData(), nDataSize);

	BOOL bRet = RealSend(m_hSocket, (void*)pData, (LPARAM)nDataSize, 0);

	delete [] pData;
//	free(pTrData);

	if(bWait)
	{
		WaitResult();
	}

	return m_nWaitResult;
}

BOOL CTRComm::RealSend(HWND hWnd, LPVOID pData, int nLen, DWORD dwType/*=TYPE400*/, BOOL bBPTrData/*=FALSE*/)
{
/*
	COPYDATASTRUCT cds;
	if( m_szTR == "15110" )
		cds.dwData = TYPE410;
	else
		cds.dwData = dwType;
	cds.cbData = nLen;// + sizeof(DWORD);
	cds.lpData = pData;
*/

	m_nWaitResult = WRET_READY;


	COPYDATASTRUCT copyData;
	if(	bBPTrData)
		copyData.dwData = PID_TRDATA_TOBP;
	else
		copyData.dwData = PID_TRDATA;
	copyData.cbData = nLen+4;
	copyData.lpData = pData;

	CHECKSTATE(m_pCallback, ICBS_SSENDBEFORE);
	BOOL m_RTn = ::SendMessage(hWnd, WM_COPYDATA, 0,(LPARAM)&copyData);
	CHECKSTATE(m_pCallback, ICBS_SSENDAFTER);
	CHECKSTATE(m_pCallback, ICBS_SREADY);
	
	StartTRTimer();
			
	return TRUE;
}

// 보조메시지를 사용하느냐, AP데이타가 있는냐에 따라 TRData 부분을 달리처리한다.
// TRData중에서 보조메시지 만큼을 먼저 사용하고 다음은 APData
// 마지막으로 화면에 전해질 데이타로 사용한다.
BOOL CTRComm::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
//	switch(pCopyDataStruct->dwData)
//	{
//	case TYPE400:	// Socket --> Map
//	case TYPE410:	// Socket --> Map
//		{
			m_nWaitResult = WRET_RECEIVE;
			CHECKSTATE(m_pCallback, ICBS_SRECEIVE);
			if(m_hWnd && IsWindow(m_hWnd))
				OnCopyData_400(pCopyDataStruct->lpData, pCopyDataStruct->cbData);
//		}
//		break;
//	}

	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}

int CTRComm::OnCopyData_400(LPVOID lpData, DWORD cbData)
{
	CServerMsg	msg;
	msg.SetBuffer((LPBYTE)lpData, cbData);

	// TR 헤더 데이터를 얻는다.
	CString strQryCode = msg.GetString(QRYCODE_LEN);	// Trcode : BYTE[8]
	msg.GetByte();							//filler
	m_ucPrevNext = msg.GetByte();		//이전.이후
	m_ucErrCode = msg.GetByte();			//에러 구분 : BYTE
	CString strMsgCode = msg.GetString(MSGCODE_LEN);	//메시지 코드 : BYTE[5]
	DWORD dwLength = msg.GetLong();				//데이타 길이 : LONG
	StopTRTimer();
	
	if( m_ucErrCode == '0')
	{	
		
		int nLen = (int)dwLength;
		if(nLen == 0)
		{
			TRACE("Error");
			return 0;
		}

		char *szBuf = new char[nLen + 1];
		memcpy(szBuf, msg.GetBuffer(), nLen);
		szBuf[nLen] = 0;

		CString strMessage = strMsgCode;
		NotifyParent(	strQryCode,
					strMsgCode,
					strMessage,
					szBuf,
					nLen);
		delete[] szBuf;
	}
	else if( m_ucErrCode == '1')
	{	
		int nLen = 0;
		TRACE("Error");

		char *szBuf = new char[nLen + 1];
		memcpy(szBuf, msg.GetBuffer(), nLen);
		szBuf[nLen] = 0;

		CString strMessage = strMsgCode;
		strMessage.Format("[%s]비정상 처리되었습니다.", strQryCode);
		NotifyParent(	strQryCode,
					strMsgCode,
					strMessage,
					szBuf,
					nLen);
		delete[] szBuf;
	}
	else if(m_ucErrCode == '2')
	{
		CString strMessage = msg.GetString(80);	//에러메세지 80byte 고정
		int nLen = (int)dwLength;
		if(nLen == 0)
		{
			TRACE("Error");
			return 0;
		}
		
		nLen -= 80; 
		char *szBuf = new char[nLen + 1];
		memcpy(szBuf, msg.GetBuffer(), nLen);
		szBuf[nLen] = 0;
		
		NotifyParent(	strQryCode,
					strMsgCode,
					strMessage,
					szBuf,
					nLen);
		delete[] szBuf;
	}
	else if(m_ucErrCode == '3')
	{
		CString strMessage = msg.GetString(80);	//에러메세지 80byte 고정
		int nLen = (int)dwLength;
		if(nLen == 0)
		{
			TRACE("Error");
			return 0;
		}
		
		//TRACE2("Error %d-%d\r\n", nLen, strTRData.GetLength());
		NotifyParent(	strQryCode,
					strMsgCode,
					strMessage,
					"",
					0);
	}
	else
	{
		int nLen = (int)dwLength;
		if(nLen == 0)
		{
			TRACE("Error");
			return 0;
		}
		
		char *szBuf = new char[nLen + 1];
		memcpy(szBuf, msg.GetBuffer(), nLen);
		szBuf[nLen] = 0;

		CString strMessage = strMsgCode;
		NotifyParent(	strQryCode,
					strMsgCode,
					strMessage,
					szBuf,
					nLen);
		delete[] szBuf;
	}
	//::SendMessage(m_hWnd, WMU_SET_MESSAGE, 0, (LPARAM)(LPCTSTR)errMesg);
	/*
	STTRHEADER* trHeader = (STTRHEADER*) lpData;

	int nCDSLen = cbData;
	char  aTrCode[5+1]={0,},
		aMsgCode[6+1]={0,},
		aMessage[82+1]={0,},
		*aTRData=NULL;
	memcpy(aTrCode, trHeader->TRCode, 5);
	memcpy(aMessage, trHeader->Message, 82);
	memcpy(aMsgCode, trHeader->MsgCode, 6);

	int	nTRDataLen = nCDSLen-(STTRHEADER_TRDATAHEAD);
	
	StopTRTimer();

	int nOffset=0;
	aTRData = new char[nTRDataLen+1];
	aTRData[nTRDataLen]=0;
	memcpy(aTRData, &trHeader->TRData+nOffset, nTRDataLen);
*/

	/*NotifyParent(	aTrCode,
					aMsgCode,
					aMessage,
					aTRData,
					nTRDataLen);

	if(aTRData)		delete aTRData;		aTRData=NULL;
*/
	return 0;
}


// 받은 정보를 넘겨준다.
// TR코드명(5)
// 메시지코드(6)
// 메시지(82)
// 에러일때 커서위치(2)
// 보조메시지 (보조메시지길이)
// TR데이타
// TR데이타길이
BOOL CTRComm::NotifyParent(
				LPCSTR szTRName,
				LPCSTR szMsgCode,
				LPCSTR szMessage,
				LPVOID	aTRData,
				long	dwTRDateLen )
{
//	m_sect.Lock();
	HRESULT hr = 0;
	if(m_pCallback)
		hr = m_pCallback->ReceiveData(m_hWnd,
										szTRName,
										szMsgCode,
										szMessage,
										aTRData,
										dwTRDateLen);
//	m_sect.Unlock();

	if(SUCCEEDED(hr)) return TRUE;
	else return FALSE;
}

void CTRComm::OnTimer(UINT nIDEvent) 
{
	StopTRTimer();

	CHECKSTATE(m_pCallback, ICBS_TIMEOUT);
	CWnd::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
// 만든이            : 이준옥(JunOk, Lee)
// Email             : alzio@dooriic.co.kr
// 만든 날짜         : 2001/8/17
// 함수 이름         : CTRComm::StartTRTimer
// 리턴 타입         : void
// 파라미터 없음     : 
// 함수 설명         : TR 타이머 시작
///////////////////////////////////////////////////////////////////////////////
void CTRComm::StartTRTimer()
{
	if(m_nTRTimer>0) KillTimer(m_nTRTimer);
	m_nTRTimer = SetTimer(TID_WAITCHK, m_nTRTimeout, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// 만든이            : 이준옥(JunOk, Lee)
// Email             : alzio@dooriic.co.kr
// 만든 날짜         : 2001/8/17
// 함수 이름         : CTRComm::StopTRTimer
// 리턴 타입         : void
// 파라미터 없음     : 
// 함수 설명         : TR 타이머 끝
///////////////////////////////////////////////////////////////////////////////
void CTRComm::StopTRTimer()
{
	if(m_nTRTimer>0) KillTimer(m_nTRTimer);
	m_nTRTimer = -1;
}

BOOL CTRComm::Create(CWnd* pParentWnd)
{
	if(!pParentWnd) 
		pParentWnd = AfxGetMainWnd();
	else
		m_pParent = pParentWnd;
	
	// Get the class name and create the window
	CString szClassName = AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS, 
		LoadCursor(NULL, IDC_ARROW));
	
	// Create the window - just don't show it yet.
	if (!CWnd::CreateEx(WS_EX_TOPMOST, szClassName, _T(""), 
		WS_POPUP, 
		0, 0, 0, 0, // size & position updated when needed
		NULL, 0, NULL))
	{
		return FALSE;
	}
	
	return TRUE;
}

int CTRComm::WaitResult()
{
	DWORD dwStartTime = GetCurrentTime();	
	DWORD dwTimeOut = 1000*30;	// TimeOut : 30초
	MSG		msg;
	while( (GetCurrentTime() - dwStartTime) < dwTimeOut )
	{
		if(m_nWaitResult>0) {
//			switch(m_nWaitResult) {		// OnCopyData()에서 m_nWaitResult의 값을 세팅할 때까지 기다림
//				case WRET_SUCCESS:
//					// 등록성공
//					break;
//				case WRET_FAILED:
//					// 등록실패 
//					break;
//			}
			return m_nWaitResult;
		}

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE )) 
		{
			if (!m_hWnd || msg.message == WM_QUIT || msg.message == WM_CLOSE) 
			{
    			PostQuitMessage(msg.wParam) ;
				m_nWaitResult = WRET_QUIT;
				return m_nWaitResult;
       		}

   			TranslateMessage( &msg );
   			DispatchMessage( &msg );
	    }
	}
	m_nWaitResult = WRET_TIMEOUT;			// 시간초과 
	return m_nWaitResult;
}

