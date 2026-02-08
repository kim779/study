// DummyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DummyWnd.h"
#include "fnda.h"
#include "fnds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Lib_SignalMng.h"
/////////////////////////////////////////////////////////////////////////////
// CDummyWnd

CDummyWnd::CDummyWnd()
{
}

CDummyWnd::~CDummyWnd()
{
}


BEGIN_MESSAGE_MAP(CDummyWnd, CWnd)
	//{{AFX_MSG_MAP(CDummyWnd)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_RP_RECEIVED, OnGetData)
//	ON_MESSAGE(WM_GET_BROD, OnGetBroad)
	ON_MESSAGE(WM_BC_DATA_RECEIVED, OnGetBroad)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDummyWnd message handlers

long CDummyWnd::OnGetBroad(WPARAM wParam, LPARAM lParam)
{
	
	ST_REALITEM* pRealItem = (ST_REALITEM*)lParam;
	
	CString strTemp;
	strTemp.Format("%s", pRealItem->szDataLength);
	int nDataLength = _ttoi(strTemp.Mid(0, 4));
	
	CString strType;
	strType.Format("%s", pRealItem->szPacketKey);
	strType = strType.Mid(0, 4);
	
	int nType;
	if(strType == "FNDS")
		nType = TYPE_SIGNAL_SYS;
	else if(strType == "FNDA")
		nType = TYPE_SIGNAL_ALERT;

	m_ptheApp->GetRealData(nType, pRealItem->szBcData, nDataLength, "");

	return 0;
}

BOOL CDummyWnd::SetSearch_NewsAll(Signal_NewsInfo stNews)
{
//@유진 : 이 루틴이 불리는 지 확인하기 위해 메시지 박스 넣음.
#ifdef __DEBUG		
	AfxMessageBox(__FILE__);
#endif

	int nSize = sizeof(Signal_NewsInfo);

	char* lpData = new char[nSize + 1];
	memset(lpData, 0x00, nSize + 1);
	memcpy(lpData, &stNews, nSize);
	int nRet = Modual_SendTr((CString)TRCODE_NEWS_ALL, nSize, (BYTE *)lpData);
			
	if (lpData)
		delete[] lpData;

	return (nRet > 0) ? TRUE:FALSE;
}

int CDummyWnd::Modual_SendTr(CString TrNo, int nTrLen, BYTE *TrData)
{
//@유진 : 이 루틴이 불리는 지 확인하기 위해 메시지 박스 넣음.
#ifdef __DEBUG		
	AfxMessageBox(__FILE__);
#endif
	
//@유진삭제	CCommOptionInfo	option;
//@유진삭제	option.m_strTrCode   = TrNo;
//@유진삭제	option.m_Destination = 'U';

	int nState=0;
//@유진삭제	nState = CommRequestDataOption(GetSafeHwnd(), (BYTE *)TrData , nTrLen, (BYTE *)&option);

	return nState;
}

BOOL CDummyWnd::SetSearch_NewsAll2(Signal_NewsInfo stNews, LPCSTR strScreenCode)
{
	int nSize = sizeof(Signal_NewsInfo);

	char* lpData = new char[nSize + 1];
	memset(lpData, 0x00, nSize + 1);
	memcpy(lpData, &stNews, nSize);
	int nRet = Modual_SendTr2((CString)TRCODE_NEWS_ALL, nSize, (BYTE *)lpData, strScreenCode);
			
	if (lpData)
		delete[] lpData;

	return (nRet > 0) ? TRUE:FALSE;
}

int CDummyWnd::Modual_SendTr2(CString TrNo, int nTrLen, BYTE *TrData, LPCSTR strScreenCode)
{
#ifdef __DEBUG		
	AfxMessageBox("Lib_SignalMng.dll [CDummyWnd::Modual_SendTr2] 루틴 불리네");
#endif

//@유진.. 아래 루틴은 사용하지 않는 듯함. 위의 메시지가 불리는 지 확인할 것...	
//@유진삭제	CCommOptionInfo	option;
//@유진삭제	option.m_strTrCode   = TrNo;
//@유진삭제	option.m_Destination = 'U';
//@유진삭제	option.m_strScreenCode = strScreenCode;

	int nState=0;
//@유진삭제	nState = CommRequestDataOption(GetSafeHwnd(), (BYTE *)TrData , nTrLen, (BYTE *)&option);

	return nState;
}

long CDummyWnd::OnGetData(WPARAM wParam, LPARAM lParam)
{
	int nResult = -1;
#ifdef __DEBUG		
	AfxMessageBox("Lib_SignalMng[CDummyWnd::OnGetData] 처리중...^^");
#endif

//@유진삭제
/*	CRpDataParser rpParser;
	rpParser.OnGetData(wParam, lParam);
	if (!rpParser.IsError())
	{
		CString strTrCode;
		strTrCode = rpParser.GetTranCode();
		strTrCode.TrimRight(" ");
		
		if ((CString) == strTrCode)
		{
			char* szTrData = new char[rpParser.GetDataSize() + 1];
			::memset(szTrData, 0x00, rpParser.GetDataSize() + 1);
			::memcpy(szTrData, rpParser.GetData(), rpParser.GetDataSize());
			
			m_ptheApp->GetCheck_News(szTrData, rpParser.GetDataSize());
			delete [] szTrData;
		}
	}
	else
	{
		char *pErrMsg = rpParser.GetErrMsg();
		
		CString strMsg;
		if (pErrMsg)
			strMsg.Format("ErrorCode[%s]", rpParser.GetErrCode());
		else
			strMsg.Format("ErrorCode[%s]=[%s]", rpParser.GetErrCode(), pErrMsg);
		
		TRACE(strMsg);
	}
*/	
	return nResult;
}
