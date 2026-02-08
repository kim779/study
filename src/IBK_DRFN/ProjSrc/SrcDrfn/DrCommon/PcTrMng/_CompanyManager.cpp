#include "stdafx.h"
#include "PcTrMng.h"
//#include "../CommonTR/SSSTOrderHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CCompanyManager
CCompanyManager::CCompanyManager()
{
//	m_SysWnd.m_nSysEde = -1;
	m_nSysEdu = -1;
}

CCompanyManager::~CCompanyManager()
{
}

int CCompanyManager::GetID()
{
	return gCURRENT_COMPANY_ID;
}

LPCSTR CCompanyManager::GetName(int nID)
{
	char *aIDName[]={"DAISHIN",
					"DAEWOO"	,	//
					"HYUNDAI"	,	//현대
					"WOORI"		,	//우리
					"HANWHA"	,	//한화
					"DONGBU"	,	//동부
					"SK	"		,	//SK	
					"KYOBO"		,	//교보
					"DONGWON"	,	//동원
					"KIWOOM"	,	//키움
					"SEOUL"		,	//서울
					"SAMSUNG"	,	//삼성
					"BOOKOOK"	,	//부국
					"HANTU"}	;	//한투

	return aIDName[nID];
}

// ---------------------------------------------------------------------------------------
//
//		아래 루틴은 삼성관련된 루틴임.	[START]
//		다른 사이트에서는 필요에따라 과감히 Delete
//
// ---------------------------------------------------------------------------------------
int CCompanyManager::GetAcctCode(CStringList& listAcct, CStringList& listAcctName)
{
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG) return GetSamsungAcctCode(listAcct, listAcctName);
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE) return GetEugeneAcctCode(listAcct, listAcctName);
	return 0;
}


//int CCompanyManager::GetAcctCodeEx(CStringList& listAcct, CStringList& listAcctName, CStringList& listAcctType, CStringList& listAcctTypeName)
int CCompanyManager::GetAcctCodeEx(LIST_STACC_SSDATA &ListAcctInfo, int nReqType)
{
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		return GetSamsungAcctCodeEx(ListAcctInfo, nReqType);
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
		return GetEugeneAcctCodeEx(ListAcctInfo, nReqType);

	return 0;
}

LPCSTR CCompanyManager::GetAcctInfo(LPCSTR szAcct, int nAcctSize, int nInfoType)
{
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		return GetSamsungAcctInfo(szAcct, nAcctSize, nInfoType);
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
		return GetEugeneAcctInfo(szAcct, nAcctSize, nInfoType);
	return "";
}

// 함  수  명  : KSLink_GetAccInfo
// 내      용  : 계좌정보를 구한다.
// 인      자  : CString strAccount : 계좌번호
//		int nType : 
//		#define		ACC_SERIAL			0		// 약정상품일련번호(3)
//		#define		ACC_PWD				1		// 계좌비밀번호  Passwd(8)
//		#define		ACC_CODE_NAME		2		// 계좌명(20)
//		#define		ACC_PRODUCTCODE		3		// 약정상품코드(3)
//		#define		ACC_PRODUCNAME		4		// 약정상품유형명(30)
//		#define		ACC_TYPECODE		5		// 계좌개설구분코드(3)
//		#define		ACC_LAPTYPECODE		6		// 랩구분코드(1)(주문을 직원에게 위임, 고객은 조회만 가능)
//		#define		ACC_OLDACCTYPECODE	7		// 구계좌구분상세코드(2)
//		#define     ACC_STIPYN          8		// 종합약관확인여부(1)
//		#define		ACC_ISALL			9		// 종합여부(1)
//		#define		ACC_ISCLOSE			10		// 폐쇄여부(1) 
//		#define		ACC_HTS				11		// HTS사용등급구분코드(1)
//		#define		ACC_ISPWD			12		// 비밀
int CCompanyManager::GetSamsungAcctCode(CStringList& listAcct, CStringList& listAcctName)
{
//@유진삭제, 유진의 계좌체계에 맞게 수정해야함.
/*	int nAccCount = theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
	CString		strInfo[ACC_ISPWD+1];
	CString		strAccName;
	for ( int i = 0; nAccCount > i ; i++)
	{
		CString		strAccCode;
		strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );

		if(strAccCode.GetLength() == 11)
		{
			strInfo[ACC_CODE_NAME] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , ACC_CODE_NAME);
			strInfo[ACC_PRODUCNAME] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , ACC_PRODUCNAME);


			strAccName = strInfo[ACC_CODE_NAME];

			strAccCode = strAccCode.Left(8) + "[" + strInfo[4] + "]" + strAccName;
			listAcctName.AddTail(strAccName);
			listAcct.AddTail(strAccCode);
		}
	}
	return listAcctName.GetCount();
*/ return 0;
}


int CCompanyManager::GetSamsungAcctCodeEx(LIST_STACC_SSDATA &ListAcctInfo, int nReqType)
{
//@유진삭제, 유진의 계좌체계에 맞게 수정해야함.
/*	if(nReqType==-1)
		return theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();

	int nAccCount = theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
	CString		strInfo[ACC_ISPWD+1];
	CString		strAccName;
	for ( int i = 0; nAccCount > i ; i++)
	{
		STACC_SSDATA* pNewItem = new STACC_SSDATA;
		CString		strAccCode;
		strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );
		
		pNewItem->szAcct = strAccCode;
		for(int j=0; j<=ACC_ISPWD; j++)
			pNewItem->InData[j] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , j);

		ListAcctInfo.AddTail(pNewItem);
	}

	return nAccCount;
*/
	return 0;
}

LPCSTR CCompanyManager::GetSamsungAcctInfo(LPCSTR szAcct, int nAcctSize, int nInfoType)
{
//@유진삭제, 유진의 계좌체계에 맞게 수정해야함.
/*	if(nAcctSize==11)
	{
		theApp.m_szTemp = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( szAcct , nInfoType);
		return (LPCSTR)theApp.m_szTemp;
	}
	else	// 전체계좌에서 확인
	{
		int nAccCount = theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
		CString		strAccCode;
		for ( int i = 0; nAccCount > i ; i++)
		{
			strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );
			if(strncmp(strAccCode, szAcct, nAcctSize)==0)
			{
				theApp.m_szTemp = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , nInfoType);
				return (LPCSTR)theApp.m_szTemp;
			}
		}
	}
*/	return "";
}

CString CCompanyManager::GetPwd()
{
	return "";
//	return theApp.m_KSLibMng.Lib_KSLink_GetLoginInfo( "USER_PASSWORD" );
}

/*
long CCompanyManager::OnGetData(WPARAM wParam, LPARAM lParam)
{
	CRpDataParser rpParser;
	rpParser.OnGetData ( wParam, lParam );
	if(  rpParser.IsError() )
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//에러처리
		int nIndex   = rpParser.GetPacketID();
		// char *pErrMsg = rpParser.GetErrMsg();
		CString strError;
		strError.Format("[%s] ", rpParser.GetErrCode());
 		strError += rpParser.GetErrMsg();

		//CString strMsg = pErrMsg;
		//TRACE("\n(%s) %s\n", "CCompanyManager::OnGetData", strMsg);
		AfxMessageBox(strError);

		return 0L;
	}

	////////////////////////////////////////////////////////////////////////////
	//tr
	//종목잔고조회
	CString strTrCode = rpParser.GetTranCode();
	strTrCode.TrimRight();

	m_szLastMsgCode = rpParser.GetReciveCode();
	m_szLastMsg = CommGetMsgString(m_szLastMsgCode);
	// m_Form.SetFrameMsgText(strMsg);

	return 1L;
}

BOOL CCompanyManager::GetLastMessage(CString& szMsgCode, CString& szMsg)
{
	szMsg = m_szLastMsg;
	szMsgCode = m_szLastMsgCode;
	return szMsgCode.GetLength();
}
*/

//*****************************************************************************
// 함  수  명  : int CCompanyManager::GetSysEdu(LPCSTR szMainKey, long pThisPoint, long dwSubKey, SysEduCallbackProc pCallFunc)
// 내      용  : 시스템트레이딩 교육여부를 체크한다.
// 인      자  : 
// 호출  함수  : 리턴값 : 0(미교육), 1(교육완료), 0이하값(조회전상태)
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2007-01-22 오후 7:52:35
//*****************************************************************************
int CCompanyManager::GetSysEdu(LPCSTR szMainKey, long pThisPoint, long dwSubKey, SysEduCallbackProc pCallFunc)
{
	// 시스템트레이딩 교육여부. 0이상이면 세팅됨. 0교육안함. 1교육함.
	//if(m_SysWnd.m_nSysEde>=0)
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
	{
		gGetSamsungSysEdu(m_nSysEdu);
	}
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
	{
//		gGetEugeneSysEdu(m_nSysEdu);
		m_nSysEdu = 1;		// 우선 교육이수처리
//		m_nSysEdu = atoi(theApp.m_KSLibMng.Lib_KSLink_GetLoginInfo( "SYSTRAD_AGR" )); // 시스템 트레이딩사용동의여부
		CString szTmp; szTmp.Format("%d", m_nSysEdu);
		::SetEnvironmentVariable(SZEUGENE_SYSEDU, szTmp);
	}

	if(m_nSysEdu>=0)
	{
		//pCallFunc(pThisPoint, szMainKey, dwSubKey, m_SysWnd.m_nSysEde);
		//return m_SysWnd.m_nSysEde;
		return m_nSysEdu;
	}

/*
	if(m_SysWnd.m_hWnd==NULL)
	{
		if(!CreateChkSysWnd())
		{
			// 실패 Callback리턴
			//pCallFunc(pThisPoint, szMainKey, dwSubKey, m_SysWnd.m_nSysEde);
			//return m_SysWnd.m_nSysEde;
			return m_nSysEdu;
		}
	}
	m_SysWnd.SendV840Data();

	if(pCallFunc)
	{
		STSysCallProcItem* pItem = new STSysCallProcItem;
		pItem->szMainKey = szMainKey;
		pItem->pThisPoint = pThisPoint;
		pItem->dwSubKey = dwSubKey;
		pItem->dwNotifyData = (long)pCallFunc;
		
		m_SysWnd.m_ListCallback.AddTail(pItem);
	}
*/

	//return m_SysWnd.m_nSysEde;
	return m_nSysEdu;
}

void CCompanyManager::RemoveSysEdu(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc)
{
//	m_SysWnd.RemoveSysEdu(szKey, pThisPoint, dwKey, pCallFunc);
}

BOOL CCompanyManager::CreateChkSysWnd()
{
	return FALSE;
//	return m_SysWnd.OpenWindow();
}

// ---------------------------------------------------------------------------------------
//
//		여기까지는 삼성관련된 루틴임.	[END]
//
// ---------------------------------------------------------------------------------------


// 함  수  명  : KSLink_GetAccInfo
// 내      용  : 계좌정보를 구한다.
// 인      자  : CString strAccount : 계좌번호
//		int nType : 
//		#define		ACC_SERIAL			0		// 약정상품일련번호(3)
//		#define		ACC_PWD				1		// 계좌비밀번호  Passwd(8)
//		#define		ACC_CODE_NAME		2		// 계좌명(20)
//		#define		ACC_PRODUCTCODE		3		// 약정상품코드(3)
//		#define		ACC_PRODUCNAME		4		// 약정상품유형명(30)
//		#define		ACC_TYPECODE		5		// 계좌개설구분코드(3)
//		#define		ACC_LAPTYPECODE		6		// 랩구분코드(1)(주문을 직원에게 위임, 고객은 조회만 가능)
//		#define		ACC_OLDACCTYPECODE	7		// 구계좌구분상세코드(2)
//		#define     ACC_STIPYN          8		// 종합약관확인여부(1)
//		#define		ACC_ISALL			9		// 종합여부(1)
//		#define		ACC_ISCLOSE			10		// 폐쇄여부(1) 
//		#define		ACC_HTS				11		// HTS사용등급구분코드(1)
//		#define		ACC_ISPWD			12		// 비밀
int CCompanyManager::GetEugeneAcctCode(CStringList& listAcct, CStringList& listAcctName)
{
	int nAccCount = 0;//theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
	CString		strInfo[ACC_ISPWD+1];
	CString		strAccName;
	for ( int i = 0; nAccCount > i ; i++)
	{
		CString		strAccCode;
		strAccCode	= "";//theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );

		if(strAccCode.GetLength() == 11)
		{
/*
			strInfo[ACC_CODE_NAME] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , ACC_CODE_NAME);
			strInfo[ACC_PRODUCNAME] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , ACC_PRODUCNAME);
*/


			strAccName = strInfo[ACC_CODE_NAME];

			strAccCode = strAccCode.Left(8) + "[" + strInfo[4] + "]" + strAccName;
			listAcctName.AddTail(strAccName);
			listAcct.AddTail(strAccCode);
		}
	}
	return listAcctName.GetCount();
}


int CCompanyManager::GetEugeneAcctCodeEx(LIST_STACC_SSDATA &ListAcctInfo, int nReqType)
{
//	if(nReqType==-1)
//		return theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();

	int nAccCount = 0;//theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
	CString		strInfo[ACC_ISPWD+1];
	CString		strAccName;
	for ( int i = 0; nAccCount > i ; i++)
	{
		STACC_SSDATA* pNewItem = new STACC_SSDATA;
		CString		strAccCode;
//		strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );
		
		pNewItem->szAcct = strAccCode;
//		for(int j=0; j<=ACC_ISPWD; j++)
//			pNewItem->InData[j] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , j);

		ListAcctInfo.AddTail(pNewItem);
	}

	return nAccCount;
}

LPCSTR CCompanyManager::GetEugeneAcctInfo(LPCSTR szAcct, int nAcctSize, int nInfoType)
{
	if(nAcctSize==11)
	{
//		theApp.m_szTemp = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( szAcct , nInfoType);
		return (LPCSTR)theApp.m_szTemp;
	}
	else	// 전체계좌에서 확인
	{
		int nAccCount = 0;//theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
		CString		strAccCode;
		for ( int i = 0; nAccCount > i ; i++)
		{
//			strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );
			if(strncmp(strAccCode, szAcct, nAcctSize)==0)
			{
//				theApp.m_szTemp = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , nInfoType);
				return (LPCSTR)theApp.m_szTemp;
			}
		}
	}
	return "";
}