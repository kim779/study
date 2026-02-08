// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PartnerMng.h"

#include "../../Chart/commontr/TRDefine.h"
#include "../../../SrcSite/include/axisfire.h"
#include "../../../SrcSite/include/axisgrid.h"
#include "../../../SrcSite/include/axisgenv.h"
#include "../../inc/IMainInfoMng.h"	

const UINT RMSG_RECEIVE_DIRECT	= ::RegisterWindowMessage("RMSG_RECEIVE_DIRECT");

//---------------------------------------------------------------------------------------
//
//				class CRegisterCodeMng
//
//---------------------------------------------------------------------------------------
CRegisterCodeMng::~CRegisterCodeMng()
{
	m_CodeList.RemoveAll();
}

POSITION CRegisterCodeMng::FindCode(LPCSTR szCode)
{
	return m_CodeList.Find(szCode);
}

void CRegisterCodeMng::RemoveCode(LPCSTR szCode)
{
	POSITION pos = FindCode(szCode);
	if(pos) m_CodeList.RemoveAt(pos);
}

void CRegisterCodeMng::AddCode(LPCSTR szCode, int nIndex)
{
	POSITION pos = FindCode(szCode);
	if(pos==NULL) m_CodeList.AddHead(szCode);
}

//---------------------------------------------------------------------------------------
//
//				class CPartnerManager_SubData1
//
//---------------------------------------------------------------------------------------
CPartnerManagerSubDataA::CPartnerManagerSubDataA()
{
	for(int i=0; i<255-TRKEY_REALSTART; i++) {m_bRealKeyInfo[i]=0;}
}

//---------------------------------------------------------------------------------------
//
//				class CPartnerManager
//
//---------------------------------------------------------------------------------------
CPartnerManager::CPartnerManager()
{
	m_hSenComm = NULL;
	m_pPtSite = NULL;
	m_pMasterDataMng = NULL;

	//gDebugMng = (IDebugManagerLast*)AfxGetPctrInterface(UUID_IDebugManager);

	FID_SENDDATA item;
	item.hWnd = NULL;
	for (int i=TRKEY_VIRT_START; i<TRKEY_VIRT_END; i++)
		m_arrSend.Add(item);

	m_bUseRegisterCodeMng = FALSE;
}

HWND CPartnerManager::SetSendHandle(HWND hSend)
{
	m_hSenComm = hSend;
	return m_hSenComm;
}

void CPartnerManager::SetSite(IPartnerSite* pSite)
{
	m_pPtSite = pSite;
}

BOOL CPartnerManager::SendTrData(LPCSTR szTR, LPBYTE pData, int nLen, int &nKey, BOOL bAdd)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CPartnerManager::SendTrData: Entry szTR(%s), nLen(%d), nKey(%d)\n", __LINE__, szTR, nLen, nKey);
// 		OutputDebugString(szDebug);
// 	}

	//g_DRDebuger.DRLog_RequestData(szTR, pData, nLen);
//	if(gDebugMng)
//	{
//		gDebugMng->TraceCompData('I', szTR, nLen, pData);
//		gDebugMng->TraceTran('I', szTR, nLen, pData);
//	}

	m_CriticalSection.Lock();
	CString strTR = szTR;
/*
	if(strTR == TRNO_MSTCODE || strTR == TRNO_CONDIONECLK_RESULT || strTR == TRNO_ALERT_REALREG ||
		strTR == TRNO_CONDITION || strTR == TRNO_WORKDATE || strTR == TRNO_GROUPTOBASE || 
		strTR == TRNO_GROUPTOBASE_UP || strTR == TRNO_GROUP_RESULT || strTR == TRNO_DRAW_RESULT)
	{
		int nRet = SendTR(szTR, (char*)pData, nLen, nKey, US_KEY);
	}
*/
	int nRet = SendTR(szTR, (char*)pData, nLen, nKey, US_KEY);

	m_CriticalSection.Unlock();

	return FALSE;
}

BOOL CPartnerManager::SendFidData(LPCSTR szFid, LPBYTE pData, int nLen, int &nKey, BOOL bAdd)
{
// 	CString szDebug;
// 	szDebug.Format("[DRCS]CPartnerManager::SendFidData %s(%d)", szFid, nLen);
// 	OutputDebugString(szDebug);


	CString strTR = szFid;
	int nRet = FALSE;
	CString strSendData;
	
	m_CriticalSection.Lock();
	strSendData = m_DataConverter.ConvertToFid(strTR, pData, nLen);


	if (strTR == QUERY_STRMARKET)
		nKey = m_DataConverter.GetMarketTRKey();
	else
		nKey = GetTrKey(strTR);

	CString strPoop = _T("GOOPHOOP");
	//<<20100315_JS.Kim 솔로몬
	//if ((strTR == QUERY_STRFOFOREIGN) || (strTR == QUERY_STRFOFOREIGN_HOGA))
	//	strPoop = _T("pfbopoop");
	if ((strTR == QUERY_STRFOFOREIGN) || (strTR == QUERY_STRFOFOREIGN_HOGA)
			|| (strTR == QUERY_STRFX))
		strPoop = _T("pfbopoop");
	else if( strTR == QUERY_STRFX_CHE || strTR == QUERY_STRFX_CHE2 || strTR == QUERY_STRFX_JAN 
			|| (strTR == QUERY_STRFOFOREIGN_CHE2) || (strTR == QUERY_STRFOFOREIGN_JAN)		)
		strPoop = _T("pfboaoop");
	else if(strTR == QUERY_STRPLUS_STOCK)//KHD:  복수 종목 조회 
	{
		strPoop = _T("poop0090");
	}
	else if(strTR == QUERY_STRMARKET)//KHD: 시장 코드 조회
	{
		strPoop = _T("GOOPHOOP");//_T("POOP0200");
	}
	else if(strTR == QUERY_STRPLUS_KOSPI ||strTR == QUERY_STRPLUS_KOSDAK ||strTR == QUERY_STRPLUS_FO  )//KHD: Investor 코드 조회 : 동일한 In/Out 이다.
	{
		strPoop = _T("bo028011");
		nRet = SendTR(strPoop, (LPTSTR)(LPCTSTR)strSendData, strSendData.GetLength(), nKey, US_PASS|US_ENC);
		return nRet;
	}
	else if( strTR ==QUERY_STOCK_HOGA)
	{

		strPoop = _T("POOPPOOP");
	}
	
	//>>

	if (nKey != -1)
	{
		//For QCMonitor
		//if (gDebugMng)	gDebugMng->TraceTran('I', strTR, strSendData.GetLength(), (LPBYTE)(LPCTSTR)strSendData);

		nRet = SendTR(strPoop, (LPTSTR)(LPCTSTR)strSendData, strSendData.GetLength(), nKey, US_OOP);
	}
	
	return nRet;
}

int CPartnerManager::directSendFID(long pSendData)
{
	m_CriticalSection.Lock();

	FID_SENDDATA *pstSendData = (FID_SENDDATA *)pSendData;
	if (pstSendData == NULL)
	{
		m_CriticalSection.Unlock();
		return 0;
	}

	int nRet = FALSE;

	CString strSendData = m_DataConverter.ConvertToFid(pstSendData->szTrCode, pstSendData->pData, pstSendData->nLen);
	int nKey = GetTrKey(pstSendData->szTrCode, pstSendData);

	if (nKey != -1)
		nRet = SendTR("POOPPOOP", (LPTSTR)(LPCTSTR)strSendData, strSendData.GetLength(), nKey, US_OOP);

	m_CriticalSection.Unlock();
	return nRet;
}

BOOL CPartnerManager::SendTrDataEx(long dwExInfo, LPBYTE pData, int nLen, int &nKey, BOOL bAdd)
{
	return FALSE;
}

BOOL CPartnerManager::SendFidDataEx(long dwExInfo, LPBYTE pData, int nLen, int &nKey, BOOL bAdd)
{
	return FALSE;
}

int CPartnerManager::GetTrKey(CString strTR, FID_SENDDATA* pSendData)
{
	if (pSendData == NULL)
	{
		if(strTR ==  QUERY_STRJONGMOK)		return  TRKEY_STRJONGMOK;
		if(strTR ==  QUERY_STRUPJONG)		return  TRKEY_STRUPJONG;
		if(strTR ==  QUERY_STRFUTOPT)		return  TRKEY_STRFUTOPT;
		if(strTR ==  QUERY_STRSTOCKFUTOPT)	return  TRKEY_STRSTOCKFUTOPT;
		if(strTR ==  QUERY_STRFOREIGN)		return  TRKEY_STRFOREIGN;
		if(strTR ==  QUERY_STRREPLAY)		return  TRKEY_STRREPLAY;
		if(strTR ==  QUERY_STOCK_HOGA)		return  TRKEY_STOCK_HOGA;
		if(strTR ==  QUERY_FUTOPT_HOGA)		return  TRKEY_FUTOPT_HOGA;
		if(strTR ==  QUERY_STOCKFUT_HOGA)	return  TRKEY_STOCKFUT_HOGA;
		if(strTR ==  QUERY_STOCKOPT_HOGA)	return  TRKEY_STOCKOPT_HOGA;
		if(strTR ==  QUERY_STRDOLLAR)		return  TRKEY_STRDOLLAR;
		if(strTR ==  QUERY_STRFOJIPYO)		return  TRKEY_STRFOJIPYO;
		if(strTR ==  QUERY_STRPREFUTOPT)	return  TRKEY_STRPREFUTOPT;
		if(strTR ==  QUERY_STRFOFOREIGN)		return  TRKEY_STRFOFOREIGN;
		if(strTR ==  QUERY_STRFOFOREIGN_HOGA)	return  TRKEY_STRFOFOREIGN_HOGA;
		if(strTR ==  QUERY_STRFOFOREIGN_CHE2)	return  TRKEY_STRFOFOREIGN_CHE2;	//20100315_JS.Kim 솔로몬
		if(strTR ==  QUERY_STRFOFOREIGN_JAN)	return  TRKEY_STRFOFOREIGN_JAN;	//20100315_JS.Kim 솔로몬
		if(strTR ==  QUERY_STRFX)				return  TRKEY_STRFX;		//20100315_JS.Kim 솔로몬
		if(strTR ==  QUERY_STRFX_CHE)			return  TRKEY_STRFX_CHE;	//20100315_JS.Kim 솔로몬
		if(strTR ==  QUERY_STRFX_CHE2)			return  TRKEY_STRFX_CHE2;	//20100315_JS.Kim 솔로몬
		if(strTR ==  QUERY_STRFX_JAN)			return  TRKEY_STRFX_JAN;	//20100315_JS.Kim 솔로몬
		if(strTR ==  QUERY_STRPLUS_STOCK)			return  TRKEY_PLUS_STOCK;	//20110415_KHD 솔로몬
		if(strTR ==  QUERY_STRPLUS_MARKET)			return  TRKEY_PLUS_MARKET;	//20110416_KHD 솔로몬
		if(strTR ==  QUERY_STRPLUS_KOSPI)			return  TRKEY_PLUS_KOSPI;	//20110416_KHD 솔로몬
		if(strTR ==  QUERY_STRPLUS_KOSDAK)			return  TRKEY_PLUS_KOSDAK;	//20110416_KHD 솔로몬
		if(strTR ==  QUERY_STRPLUS_FO)				return  TRKEY_PLUS_FO;	//20110416_KHD 솔로몬
		if(strTR ==  QUERY_STRPLUS_SIGNAL)				return  TRKEY_PLUS_SIGNAL;	//20110416_KHD 솔로몬
		
	}
	else
	{
		int nIndex = 0;
		FID_SENDDATA item;
		for (int i=TRKEY_VIRT_START; i<TRKEY_VIRT_END; i++)
		{
			nIndex = i-TRKEY_VIRT_START;
			item = m_arrSend.GetAt(nIndex);
			if (item.hWnd == NULL)
			{
				m_arrSend.SetAt(nIndex, *pSendData);
				return i;
			}
		}
	}

	return -1;
}

BOOL CPartnerManager::ReceiveTRData(LPCSTR szTR, LPBYTE pData, int nLen, int nKey)
{	
	ASSERT(m_pPtSite);
	if(m_pPtSite)
	{
		m_CriticalSection.Lock();

		CString strData;
		int nDataLen = 0, nGuideLine = 0;

		if(nKey >= TRKEY_STRJONGMOK && nKey <= TRKEY_FID_END || nKey >= TRKEY_STRMARKET_0 && nKey <= TRKEY_STRMARKET_END)
		{
			char* szData = m_DataConverter.Convert2Struct(pData, nLen, nKey, nDataLen);
			//@Solomon:091123SM068	-->
			char* pszGuideLine = m_DataConverter.GetConv2StructGuideLine(nKey, nGuideLine);
			if (nGuideLine > 0)
			{
				m_pPtSite->RecieveTRData(nKey, m_DataConverter.GetTrNo(nKey), QUERY_STRGUIDELINE, "", (LPVOID)pszGuideLine, nGuideLine);
				//if(gDebugMng) gDebugMng->TraceTran('O', szTR, nGuideLine, pszGuideLine);
				//g_DRDebuger.DRLog_ResponseData(m_DataConverter.GetTrNo(nKey), (LPBYTE)pszGuideLine, nGuideLine));
				if (pszGuideLine > 0)
				{
					delete[] pszGuideLine;
					pszGuideLine = NULL;
				}
			}
			//@Solomon:091123SM068	<--

			//20221209 이문수 szTRNo 문자열값이 깨지는 현상 수정(GetTrNo함수 리턴변수 CString은 지역변수로 리턴이 완료되면 사라지므로 그것의 포인터 내용도 깨지게 된다) >>
			//LPCSTR szTRNo = m_DataConverter.GetTrNo(nKey);
			CString szTRNo = m_DataConverter.GetTrNo(nKey);
			//20221209 이문수 szTRNo 문자열값이 깨지는 현상 수정(GetTrNo함수 리턴변수 CString은 지역변수로 리턴이 완료되면 사라지므로 그것의 포인터 내용도 깨지게 된다) <<
			m_pPtSite->RecieveTRData(nKey, szTRNo, "", "", (LPVOID)szData, nDataLen);
			if(nKey==TRKEY_STRDOLLAR) _DoPreReceiveData(QUERY_STRDOLLAR, pData, nLen);

			//g_DRDebuger.DRLog_ResponseData(szTRNo, (LPBYTE)szData, nDataLen);
			//if(gDebugMng) gDebugMng->TraceTran('O', szTRNo, nDataLen, (LPBYTE)szData);
			if(g_pDRDebuger) g_pDRDebuger->DRLog_ResponseData(szTRNo, (LPBYTE)szData, nDataLen);
			if (nDataLen > 0)
			{
				delete[] szData;
				szData = NULL;
			}
		}
		else if(nKey<= TRKEY_END)
		{
			if(nKey==24)// SYMBOLDATA : KHD 
			{
				szTR="SYMBOLDATA";
			}
// 			{
// 				CString szDebug;
// 				szDebug.Format("[DRCS][LOAD](%8d) CPartnerManager::ReceiveTRData: Entry\n", __LINE__);
// 				OutputDebugString(szDebug);
// 			}
			m_pPtSite->RecieveTRData(nKey, szTR, "", "", pData, nLen);
			if(nKey==TRKEY_MSTCODE)
				_DoPreReceiveData(TRNO_MSTCODE, pData, nLen);
			else if(nKey==TRKEY_WORKDATE)
				_DoPreReceiveData(TRNO_WORKDATE, pData, nLen);

			//if(gDebugMng)
			//{
			//	gDebugMng->TraceCompData('O', szTR, nLen, pData);
			//	gDebugMng->TraceTran('O', szTR, nLen, pData);
			//}
			//g_DRDebuger.DRLog_ResponseData(szTR, (LPBYTE)pData, nLen);
		}
		else if(nKey>= TRKEY_REALSTART)
		{
			//winix RealKey값 확인
//			m_pPtSite->RecieveRealKey(nKey, szTR, "", "", pData, nLen);
			//OutputDebugString();
		}
		else if (nKey >= TRKEY_VIRT_START && nKey <= TRKEY_VIRT_END)
		{
			int nIndex = nKey - TRKEY_VIRT_START;
			if (nIndex < m_arrSend.GetSize())
			{
				FID_SENDDATA item = m_arrSend.GetAt(nIndex);
				if (item.hWnd)
				{
					FID_RECEIVEDATA *pstData = new FID_RECEIVEDATA;
					pstData->pData = m_DataConverter.Convert2Struct(pData, nLen, item.szTrCode, nDataLen);
					pstData->nKey = nKey;
					pstData->nLen = nDataLen;
					pstData->szTrCode = item.szTrCode;
					::SendMessage(item.hWnd, RMSG_RECEIVE_DIRECT, (WPARAM)pstData, 0);
					if (nDataLen > 0)
					{
						delete[] pstData->pData;
						pstData->pData = NULL;
					}
					delete pstData;
					pstData = NULL;
				}
				item.hWnd = NULL;
				item.pData = NULL;
				m_arrSend.SetAt(nIndex, item);
			}
		}
		else
		{	
			m_CriticalSection.Unlock();
			return FALSE;
		}

		m_CriticalSection.Unlock();
	}

	return TRUE;
}

BOOL CPartnerManager::SendTR(CString strTR, char *szData, int nLen, int nKey, int nStat)
{
	CString strTRData = "";

	struct _userTH user;
	strncpy(&user.trc[0], strTR, 8);

	//<<20100319_JS.Kim FX TR은 secondary session으로 요청한다.
	//user.path = ssMASTER;
//KHD	if( strTR.Left(2) == TRMODE_FX )
//		user.path = ssSEC1;
//	else
//		user.path = ssMASTER;
	//>>
	user.key  = nKey;
	user.stat = nStat;
//KHD	user.size = nLen;
	CString strTest;
	strTest.Format("KHD : strTR = %s nKey = %d, nStat = %d",strTR, nKey, nStat);
//	OutputDebugString(strTest);
//	strTRData = CString((char*)&user, sizeof(_userTH));
//	strTRData += CString(szData, nLen);
	//@100118-alzioyes
	int  nHead = sizeof(_userTH);
	long nSendLen = nHead+nLen;
	LPBYTE pSendBuffer = new BYTE[nSendLen+1];
	memcpy(pSendBuffer, (char*)&user, nHead);
	memcpy(pSendBuffer+nHead, szData, nLen);
	pSendBuffer[nSendLen]=NULL;

// 	CString szDebug;
// 	szDebug.Format("[DRCS]CPartnerManager::SendTR %s(%d)", strTR, nLen);
// 	OutputDebugString(szDebug);

	//BOOL bRet = ::SendMessage(m_hSenComm, WM_USER, MAKEWPARAM(invokeTRx, nLen), (LPARAM)(const char*)strTRData);
	BOOL bRet = ::SendMessage(m_hSenComm, WM_USER, MAKEWPARAM(invokeTRx, nLen), (LPARAM)(const char*)pSendBuffer);
	delete [] pSendBuffer;

	return bRet;
}

int CPartnerManager::GetRealKey(BYTE cKey, LPCSTR szCode, int nCodeType)
{
	//return TRKEY_REALSTART;

	CString szUseCode(szCode);
// 	CString rCode;
// 	if(theApp.m_PtUtilMng.GetSiteUpKode(NULL, szCode, rCode))
// 	{
// 		szUseCode="K";
// 		szUseCode += rCode;
// 	}

	int rValue;
	if(m_mapRealKey.Lookup(szUseCode, rValue)==TRUE)
	{
		if(cKey=='A')
		{

		}
		else	//'U'
		{
			m_subDataA.m_bRealKeyInfo[rValue-TRKEY_REALSTART] = 0;
			m_mapRealKey.RemoveKey(szUseCode);			
		}

		//if (g_pDRDebuger)
		//{
		//	CString strData;
		//	strData.Format(_T("**** Find RealKey : %d"), rValue);
		//	g_pDRDebuger->DRLog_Message(strData);
		//}

		return rValue;
	}
	else
	{
		if(cKey=='A')
		{
			for(int i=0; i<255-TRKEY_REALSTART; i++)
			{
				if(m_subDataA.m_bRealKeyInfo[i]==0)
				{
					m_subDataA.m_bRealKeyInfo[i] = 1;
					m_subDataA.m_bMarketInfo[i] = nCodeType;
					rValue = i+TRKEY_REALSTART;
					m_mapRealKey.SetAt(szUseCode, rValue);
					return rValue;
				}
			}
			{
				CString szMsg;
				szMsg.Format("RealKey Error(%s)[Ln%d]", __FILE__, __LINE__);
				AfxMessageBox(szMsg);
			}			
		}
		ASSERT(FALSE);
	}
	return -1;
}

int	CPartnerManager::FindRealKey(LPCSTR szCode)
{
	if (strlen(szCode) == 0)
		return -1;

	int rValue;
	if (m_mapRealKey.Lookup(szCode, rValue) == TRUE)
		return rValue;

//	지우지말것,DRFN Platform 로그기능추가, 필요시엔만 사용하고 배포할 때는 막을 것]
	//if (g_pDRDebuger)
	//{
	//	CString strTemp;	strTemp.Format(_T("*** not found REAL KEY : %s"), szCode);
	//	g_pDRDebuger->DRLog_Message(strTemp);
	//}

	return -1;
}

// 주식			:  1301, 1021
// 업종			: 20301, 20021
// (주식)선옵	: 30301, 30021
int CPartnerManager::RequestMChartReal(BYTE cKey, LPCSTR szDrdsKey, LPCSTR szCode)
{
	SetCursor(LoadCursor(0,IDC_ARROW));
	int nCodeType = CMasterTypeInfo::NONE_CHART;

	BOOL bRealSend = TRUE;
	CString strData, strCode, strMarketSymbol;
	strCode = szCode;

	if (strcmp(szDrdsKey, "KXS3") == 0 || strcmp(szDrdsKey, "KXYS") == 0 || strcmp(szDrdsKey, "EWs3") == 0)
	{														// 거래소/코스닥/ELW
		nCodeType = CMasterTypeInfo::STOCK_CHART;
		strMarketSymbol = _T("1");
	}
	else if (strcmp(szDrdsKey, "IXIJ") == 0)				// 업종
	{
		nCodeType = CMasterTypeInfo::UPJONG_CHART;
		strMarketSymbol = _T("21");
	}
	else if (strcmp(szDrdsKey, "FFC0") == 0)		 		// 선물
	{
		nCodeType = CMasterTypeInfo::FUTOPT_CHART;
		strMarketSymbol = _T("31");
	}
	else if (strcmp(szDrdsKey, "OOC0") == 0)				// 옵션
	{
		nCodeType = CMasterTypeInfo::FUTOPT_CHART;
		strMarketSymbol = _T("41");
	}
	else if (strcmp(szDrdsKey, "JFC0") == 0)				// 주식선물
	{
		nCodeType = CMasterTypeInfo::STOCKFUTOPT_CHART;
		strMarketSymbol = _T("61");
	}
	else if (strcmp(szDrdsKey, "HWFJ")==0)					// 해외
	{
		nCodeType = CMasterTypeInfo::FOREIGN_CHART;
		strMarketSymbol = _T("51");
	}
	else if (strcmp(szDrdsKey, "GTFJ")==0)					// 상품선물
	{
		nCodeType = CMasterTypeInfo::COMMODITY_CHART;
		strMarketSymbol = _T("101");
	}
	else if (strcmp(szDrdsKey, "FOJS")==0)					// FO 지수, 솔로몬에서 차트전용으로 사용한것 같은데.. 확인이 필요함.
	{
		nCodeType = CMasterTypeInfo::FUOPT_JIPYO_CHART;
		bRealSend = FALSE;
	}
	else if (strcmp(szDrdsKey, "FOFR")==0)					// 해외선물, IBK 는 없음.
	{
		nCodeType = CMasterTypeInfo::FO_FOREIGN_CHART;
		bRealSend = FALSE;
	}
	else if (strcmp(szDrdsKey, "FX")==0)					// FX 지수, IBK 는 없음.
	{
		nCodeType = CMasterTypeInfo::FX_CHART;
		bRealSend = FALSE;
	}
	else if (strcmp(szDrdsKey, "JOC0") == 0)				// 주식옵션, IBK 는 없음.
	{
		nCodeType = CMasterTypeInfo::STOCKFUTOPT_CHART;
		bRealSend = FALSE;
	}
	else
		return -1;

	m_bUseRegisterCodeMng = TRUE;
	//100521-alzioyes. 리얼체크용 리스트에 추가.
	if (cKey=='A')
		m_RegisterCodeMng.AddCode(szCode);
	else
		m_RegisterCodeMng.RemoveCode(szCode);

	int nKey = GetRealKey(cKey, szCode, nCodeType);

	if (nKey<0 || cKey=='U')
		return nKey;

	if (!bRealSend)
		return 1;

	strData += strMarketSymbol + _T("301");
	strData += sDS;
	strData += strCode;
	strData += sCELL;
	strData += strMarketSymbol + _T("021");
	strData += sCELL;
	strData += strMarketSymbol + _T("023");
	strData += sCELL;

	//CString strTest;
	//strTest.Format("KHD : nKey = %d Code = %s  MarketType = %d  strData = %s", nKey, strCode, nCodeType, strData);
	//OutputDebugString("KHD : 리얼심볼키  = " + strTest);
	//if (g_pDRDebuger)
	//	g_pDRDebuger->DRLog_Message(strTest);

	return SendTR("GOOPHOOP", (LPSTR)(LPCTSTR)strData, strData.GetLength(), nKey, US_OOP);
}

#include "../../Chart/COMMONTR/ChartRealDef.h"
BOOL CPartnerManager::ReceiveRealDataEx(WPARAM wParam, LPARAM lParam)
{
	struct _alertR* alertR = (struct _alertR*)lParam;
	DWORD* pArrData = NULL;

	CString strCode = alertR->code;

/*	if (g_pDRDebuger)
	{
		CString strTemp;	strTemp += _T("ReceiveRealDataEx Code : ") + strCode;
		g_pDRDebuger->DRLog_Message(strTemp);
	}
*/

	//<<20100316_JS.Kim FX
	if (strCode.IsEmpty())
		return FALSE;

	int nCodeType = _DoGetCodeType(strCode);
	if (CMasterTypeInfo::FX_CHART != nCodeType && CMasterTypeInfo::FO_FOREIGN_CHART != nCodeType)
	{
		if (strCode[0] == 'A' || strCode[0] == 'J' || strCode[0] == 'Q')
			strCode = strCode.Mid(1);

		if (strCode[0] == 'K' || (strCode.GetLength() > 2 && ( strCode.Left(2) == "ET" || strCode.Left(2) == "ST" ) ) )
		{
			strCode = strCode.Mid(1);
			CString rCode;
			if (theApp.m_PtUtilMng.GetSiteUpKode(strCode, rCode))
				strCode = rCode;
		}

		if (strCode[0] == 'S' || strCode[0] == 'T')
			return FALSE;
	}

	int nKey = FindRealKey(strCode);
	if (nKey < 0)
		return FALSE;

// 	{	//For QCMonitor
// 		if(gDebugMng) gDebugMng->TraceReal(strCode, nSize, (LPBYTE)pData);
// 	}

	//@by alzioyes:100406 최근Real이 0번째.
	//for (int i = 0; i < alertR->size; i++)
	for (int i=alertR->size-1; i>=0; i--)
	{
		pArrData = (DWORD*)alertR->ptr[i];
		ReceiveRealDataEx_Sub(strCode, nCodeType, nKey, pArrData);
	}

	return TRUE;
}

BOOL CPartnerManager::ReceiveRealDataEx_Sub(CString& strCode, int nCodeType, int nKey, DWORD* pArrData)
{
	CString szDebug;
	m_pPtSite->RecieveRealDataEx(1, strCode, (LONG)pArrData);
	//<<20100316_JS.Kim FX
	if (CMasterTypeInfo::FX_CHART == nCodeType)
		return TRUE;
	//>>

 	BOOL bSign = FALSE;
 	CRealData_Common RealData;

	if (bSign)	RealData.m_strPrice = (char*)pArrData[111];
	else		RealData.m_strPrice = (char*)pArrData[ 23];

	//거래량이 없으면 리얼처리를 하지않는다.
	//mapRealData.Lookup("027", RealData.m_strVolume);	//누적거래량
	//<<20100419_JS.Kim 해외선물은 체결량을 사용한다.
	//RealData.m_strVolume = (char*)pArrData[ 27];
	if( nCodeType == CMasterTypeInfo::FO_FOREIGN_CHART )
	{
		//<<20100421_JS.Kim. 솔로몬 해외선물. 플래그 값이 '1' : 체결, '2' : 호가
		CString szFlag = (char*)pArrData[0];
		if (szFlag.IsEmpty() || szFlag.GetAt(0) != '1')
		{
//	지우지말것,DRFN Platform 로그기능추가, 필요시엔만 사용하고 배포할 때는 막을 것]
			//if (g_pDRDebuger)
			//{
			//	CString strTemp;	strTemp.Format(_T("*** not contract data : code[%s], price[%s]"), strCode, RealData.m_strPrice);
			//	g_pDRDebuger->DRLog_Message(strTemp);
			//}

			return TRUE;
		}

		//alzioyes:100701 김진순부장은 왜 누적거래량으로 하지 않았지?
		//RealData.m_strVolume = (char*)pArrData[ 32];
		RealData.m_strVolume = (char*)pArrData[ 27];
		RealData.m_strVolume.Remove('-');
		RealData.m_strVolume.Remove('+');
	}
	else
	{
		RealData.m_strVolume = (char*)pArrData[ 27];
	}
	//>>

	if ((nCodeType == CMasterTypeInfo::STOCK_CHART) && RealData.m_strVolume.IsEmpty())
	{
		RealData.m_strVolume = (char*)pArrData[112];	//누적거래량
		bSign = TRUE;
	}

	//@solomon	FO 지표차트는 거래량이 없음.
	if (nCodeType != CMasterTypeInfo::FUOPT_JIPYO_CHART)
	{
		if (RealData.m_strVolume.IsEmpty() == TRUE)
		{
//	지우지말것,DRFN Platform 로그기능추가, 필요시엔만 사용하고 배포할 때는 막을 것]
			//if (g_pDRDebuger)
			//{
			//	CString strTemp;	strTemp.Format(_T("*** no volume : code[%s], price[%s]"), strCode, RealData.m_strPrice);
			//	g_pDRDebuger->DRLog_Message(strTemp);
			//}

			return TRUE;
		}
	}

	if (bSign)	RealData.m_strPrice = (char*)pArrData[111];
	else		RealData.m_strPrice = (char*)pArrData[ 23];

	RealData.m_strPrice.TrimLeft();
	RealData.m_strPrice.TrimRight();
	if (RealData.m_strPrice.GetLength() == 0)
		bSign = FALSE;

	//20110124_alzioyes:FO차트(FUOPT_JIPYO_CHART) 리얼처리루틴 수정.
	if( nCodeType == CMasterTypeInfo::FUOPT_JIPYO_CHART )
		RealData.m_strPrice.Format("%.2f", (atof(RealData.m_strPrice)/100));

	RealData.m_bIsRealJang	= !bSign;
	RealData.m_strCode		= strCode;

	CString szUpLimit, szDnLimit;
	szUpLimit = (char*)pArrData[302];	//상한가.
	szDnLimit = (char*)pArrData[303];	//하한가.

	// -------------------------------------------------------
	RealData.m_strTime = (char*)pArrData[ 34];	//체결시간

//@solomon	현지시간(확인되면 다시 셋팅..)
//	mapRealData.Lookup("035", RealData.m_strTime2);	//현지체결시간(해외의 경우)

	if (RealData.m_strTime.CompareNoCase(_T("장종료")) == 0)
		RealData.m_strTime = _T("9999");
	else if (RealData.m_strTime.CompareNoCase(_T("장마감")) == 0)
		RealData.m_strTime = _T("8888");

	if (!bSign && atoi(RealData.m_strTime) <= 0)
		return TRUE;

	if (nCodeType != CMasterTypeInfo::FO_FOREIGN_CHART)
		RealData.m_strPrice.Remove('.');

	// 종목코드로 시장정보를 얻어와도 스프레드선물이 선물로 취급되어 케이스별 예외처리.(위닉스플랫폼...)
	// 스프레드 선물일 경우 앞자리의 부호는 색상을 나타냄 (+, -, 0)상승/하락/보합
	// 스프레드 선물 현재가는 음수를 갖을 수 있음. 뒷자리 부호는 실제 현재가격의 부호.
	// 상승/하락 색상은 등락률 부호를 사용하므로 따로 처리 안함.
	// 	if( RealData.m_strPrice.Find("0-") > -1 || RealData.m_strPrice.Find("0+") > -1 || RealData.m_strPrice.Find("--") > -1 || RealData.m_strPrice.Find("-+") > -1 ||
	// 		RealData.m_strPrice.Find("+-") > -1 || RealData.m_strPrice.Find("++") > -1)	
	if( ( nCodeType == CMasterTypeInfo::FUOPT_JIPYO_CHART || nCodeType == CMasterTypeInfo::FUTOPT_CHART || nCodeType == CMasterTypeInfo::STOCKFUTOPT_CHART ) 
		&& !RealData.m_strPrice.IsEmpty() )
	{
		if(IS_SPREAD_CODE_1(RealData.m_strCode.Left(1)))	//스프레드선물인경우 // 0024696: 파생상품 코드체계 변경 제도개선
			RealData.m_strPrice = RealData.m_strPrice.Mid( 1 );
		else
			RealData.m_strPrice.Remove('-');
	}
	else
		RealData.m_strPrice.Remove('-');

	//등락부호:1:상한,2:상승,3:보합,4:하한,5:하락
	if (bSign)	RealData.m_strChange = (char*)pArrData[115];	//전일대비
	else		RealData.m_strChange = (char*)pArrData[ 24];	//전일대비
	RealData.m_strChange = RealData.m_strChange.Mid(1);

	if (bSign)	RealData.m_strChrate = (char*)pArrData[116];	//등락률
	else		RealData.m_strChrate = (char*)pArrData[ 33];	//등락률
	if (nCodeType != CMasterTypeInfo::FO_FOREIGN_CHART)
			RealData.m_strChrate.Remove('.');
	char cSign = RealData.m_strChrate[0];

	switch (cSign)
	{
	case ' ': RealData.m_strSign = "3"; break;
	case '+': RealData.m_strSign = "2"; break;
	case '-': RealData.m_strSign = "5"; break;
	}

	theApp.DoGetMapStrObj_UpDnInfo(strCode, szUpLimit, szDnLimit);
	
	if (szUpLimit.GetLength() > 0 && atof(szUpLimit) <= atof(RealData.m_strPrice))
		RealData.m_strSign = "1";
	else if (szDnLimit.GetLength() > 0 && atof(szDnLimit) >= atof(RealData.m_strPrice))
		RealData.m_strSign = "4";

	RealData.m_strOpen = (char*)pArrData[ 29];	//시가
	RealData.m_strHigh = (char*)pArrData[ 30];	//고가
	RealData.m_strLow = (char*)pArrData[ 31];	//저가
	if( nCodeType != CMasterTypeInfo::FO_FOREIGN_CHART )
	{
		RealData.m_strOpen.Remove('.');
		RealData.m_strHigh.Remove('.');
		RealData.m_strLow.Remove('.');
	}

	if( nCodeType == CMasterTypeInfo::FUOPT_JIPYO_CHART || nCodeType == CMasterTypeInfo::FUTOPT_CHART || nCodeType == CMasterTypeInfo::STOCKFUTOPT_CHART )
	{
		if(IS_SPREAD_CODE_1(RealData.m_strCode.Left(1)))	//스프레드선물인경우 // 0024696: 파생상품 코드체계 변경 제도개선
		{		
			if( RealData.m_strOpen.GetLength() > 0 )
				RealData.m_strOpen = RealData.m_strOpen.Mid( 1 );
			if( RealData.m_strHigh.GetLength() > 0 )
				RealData.m_strHigh = RealData.m_strHigh.Mid( 1 );
			if( RealData.m_strLow.GetLength() > 0 )
				RealData.m_strLow = RealData.m_strLow.Mid( 1 );
		}
		else
		{
			RealData.m_strOpen.Remove('-');
			RealData.m_strHigh.Remove('-');
			RealData.m_strLow.Remove('-');
		}		
	}
	else
	{
		RealData.m_strOpen.Remove('-');
		RealData.m_strHigh.Remove('-');
		RealData.m_strLow.Remove('-');
	}


	RealData.m_strValue = (char*)pArrData[ 28];	//거래대금

	RealData.m_nMarketType=nCodeType;

	RealData.m_strOpenyak = (char*)pArrData[201];	//미결제약정
	// -------------------------------------------------------	

	if (bSign)
		RealData.m_strOpen = RealData.m_strHigh = RealData.m_strLow = "";

	//<<20100421_JS.Kim 솔로몬 해외선물. 진법 자리수 맞추기
	if (nCodeType == CMasterTypeInfo::FO_FOREIGN_CHART)
	{
		int nPcnt;
		double dRate;
		CString strTextType;

		CString strGB = (char*)(char*)pArrData[309];	//	해외선물종목 시세표시구분
		char cType = CDataConverter::GetDecimalRate(strGB, nPcnt, dRate, strTextType);

		RealData.m_strPrice.Remove('+');
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, RealData.m_strPrice))
			RealData.m_strPrice.Format(_T("%lf"), CDataConverter::DataSeparation(RealData.m_strPrice, nPcnt));

		RealData.m_strOpen.Remove('+');
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, RealData.m_strOpen))
			RealData.m_strOpen.Format(_T("%lf"), CDataConverter::DataSeparation(RealData.m_strOpen, nPcnt));

		RealData.m_strHigh.Remove('+');
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, RealData.m_strHigh))
			RealData.m_strHigh.Format(_T("%lf"), CDataConverter::DataSeparation(RealData.m_strHigh, nPcnt));

		RealData.m_strLow.Remove('+');
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, RealData.m_strLow))
			RealData.m_strLow.Format(_T("%lf"), CDataConverter::DataSeparation(RealData.m_strLow, nPcnt));

		RealData.m_strChange.Remove('+');
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, RealData.m_strChange))
			RealData.m_strChange.Format(_T("%lf"), CDataConverter::DataSeparation(RealData.m_strChange, nPcnt));
	}

	m_pPtSite->RecieveRealData(nKey, (LONG)&RealData);

	return TRUE;
}

BOOL CPartnerManager::ReceiveRealData(WPARAM wParam, LPARAM lParam)
{
	//@100521-by alzioyes.  더이상 사용안함.
	return FALSE;
}

int CPartnerManager::_DoRealDataParsing(LPTSTR pData, CMapStringToString *pMap)
{
	int nIndex=0, nDataIdx=0, nDataCount=0;
	CString strData, strFID;
	char aKey[10+1], aValue[50+1];
	memset(aKey, 0x00, sizeof(aKey));
	memset(aValue, 0x00, sizeof(aValue));

	char cParser = '\t';
	int nPos=0, nEndPos=strlen(pData);

	while(nPos<nEndPos)
	{
		_DoParser(cParser, pData, nPos, nEndPos, aKey);
		_DoParser(cParser, pData, nPos, nEndPos, aValue);

		pMap->SetAt(aKey, aValue);
	}
	
	return 1;
}

int CPartnerManager::_DoParser(char cParser, LPTSTR pData, int &nPos, int nEndPos, char* aString)
{
	int nDataIdx=0;
	while (pData[nPos] != 0x00)
	{
		if (pData[nPos] == cParser || nPos>=nEndPos)
		{
			nPos++;
			aString[nDataIdx] = 0;
			return nDataIdx;
		}
		else
			aString[nDataIdx++] = pData[nPos++];
	}
	return -1;
}

int CPartnerManager::_DoGetCodeType(LPCSTR szCode)
{
	CString strCode;
	strCode = szCode;
	if(strCode[0]=='K'|| strCode[0]=='A' || strCode[0] == 'Q'  )
	{
		strCode = strCode.Mid(1);
	}
	else if( strCode.GetLength() > 2 && (strCode.Left(2) == "ET" || strCode.Left(2) == "ST" ) )
	{
		strCode = strCode.Mid(1);
		CString rCode;
		if (theApp.m_PtUtilMng.GetSiteUpKode(strCode, rCode))
				strCode = rCode;
	}

	if(!m_pMasterDataMng) m_pMasterDataMng = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	return (int)m_pMasterDataMng->GetMarketTypeFromCode(strCode);
}

//<<20100412_JS.Kim FX. 체결데이타 실시간 수신
BOOL CPartnerManager::ReceiveNoticeData(WPARAM wParam, LPARAM lParam)
{
	CString strNotice = (char*)lParam;

	if (strNotice.IsEmpty()) 
		return FALSE;
	
	CString strFormCode = (LPCTSTR)wParam;

	CStringArray arList;	
	CString strAccount	= _T("");
	CString strID		= _T("");
	CString strJmNum	= _T("");			//주문 번호
	CString strRtsCode	= _T("");				
	CString strSymbol	= _T("");
	CString strData		= _T("");
			
	int nContract = 0;		//1/2: 잔고컨트롤에서 오는 데이터(1:FX,2:해외선물), 그외: 체결데이터
			
	while (!strNotice.IsEmpty())	
	{
		//arList.Add(m_pApp->Parser(strNotice, sROW));
		arList.Add(CDataConverter::Parser(strNotice, sROW));
	}	
	
	CString sMajorInfo, sMinerInfo;		
	sMajorInfo = CDataConverter::Parser(arList[0], '\r');	
	while(!sMajorInfo.IsEmpty())
	{
		strSymbol = CDataConverter::Parser(sMajorInfo, '\t');
		strData	  = CDataConverter::Parser(sMajorInfo, '\t');
		
		switch (atoi(strSymbol))
		{
		case 901:   //계좌번호
			strAccount = strData;
			break;
		case 902:   //주문ID
			strID = strData;
			break;		
		case 830:	// 해외선물 체결 TR
			nContract = 2;
			break;				
		case 840:	// FX 체결 TR
			nContract = 1;
			break;				
		}
	}

	if( nContract != 1 && nContract != 2 )			return	FALSE;
	if( nContract == 1 && strFormCode != "9041" )	return	FALSE;
	if( nContract == 2 && strFormCode != "8041" )	return	FALSE;

	FX_CHE_STRUCT OrderInfo;
	memset(&OrderInfo, ' ', sizeof(FX_CHE_STRUCT));

	memcpy(OrderInfo.szAccNo, (LPCTSTR)strAccount, min(strAccount.GetLength(), sizeof(OrderInfo.szAccNo)));
	

	CString symbol, value;
	sMinerInfo	= arList[0];
	strSymbol	= CDataConverter::Parser(sMinerInfo,	'\t');	
	strData		= CDataConverter::Parser(sMinerInfo,	'\t');	
	switch (atoi(strSymbol))
	{
	case 903:	//Rts코드(잔고tr)
		strRtsCode  = strData;
		break;
	case 904:	//주문번호(주문tr)
		strJmNum.Format("%06d", atoi(strData));
		memcpy(OrderInfo.szCustItem2, (LPCTSTR)strJmNum, min(strJmNum.GetLength(), sizeof(OrderInfo.szCustItem2)));
		break;
	}	
	//----------------------------첫번째줄 파싱끝			


	CString strType = "", strMGB="";
	strNotice = arList[1];
	while(!strNotice.IsEmpty())
	{
		strSymbol = CDataConverter::Parser(strNotice, '\t');
		strData	  = CDataConverter::Parser(strNotice, '\t');
		
		switch (atoi(strSymbol))
		{
		case 21:   //종목코드
			memcpy(OrderInfo.SHCODE, (LPCTSTR)strData, min(strData.GetLength(), sizeof(OrderInfo.SHCODE)));
			break;
		case 905:	//원주문번호
			strData.TrimLeft();		strData.TrimRight();
			memcpy(OrderInfo.szOrgCustItem2, (LPCTSTR)strData, min(strData.GetLength(), sizeof(OrderInfo.szOrgCustItem2)));
			break;
		case 948:	//체결번호
			strData.TrimLeft();		strData.TrimRight();
			memcpy(OrderInfo.szFXCMPosID, (LPCTSTR)strData, min(strData.GetLength(), sizeof(OrderInfo.szFXCMPosID)));
			break;
		case 950:	//포지션번호
			strData.TrimLeft();		strData.TrimRight();
			memcpy(OrderInfo.szPositionID, (LPCTSTR)strData, min(strData.GetLength(), sizeof(OrderInfo.szPositionID)));
			break;
		case 918:   //거래유형코드 11:진입, 12:청산, 13:마진콜
			strType = strData;
			strType.TrimLeft();		strType.TrimRight();
			memcpy(OrderInfo.szTradeType, strType, min(sizeof(OrderInfo.szTradeType), strType.GetLength()));
			break;		
		case 912:   //매도/매수 구분 "-"/"+"
			strMGB = strData;
			strMGB.TrimLeft();		strMGB.TrimRight();
			//if( strData == "+매수" )			memcpy(OrderInfo.szSide, "079", 3);
			//else								memcpy(OrderInfo.szSide, "081", 3);
			break;		
		case 996:	// 주문상태정보. 
			// 01:주문접수,02:체결중,03:체결완료,04:정정접수,05:정정완료(원주문),06:취소접수,07:취소완료(원주문),08:거부,15:정정완료(정정주문),17:취소완료(취소주문)
			strData.TrimLeft();		strData.TrimRight();
			if( strData == "08" )		return TRUE;

			if( strData == "03" )			memcpy(OrderInfo.szKeyType, "CH01", 4);	//체결
			else							memcpy(OrderInfo.szKeyType, "OR01", 4);	//주문

			memcpy(OrderInfo.szStatus, strData, min(sizeof(OrderInfo.szStatus), strData.GetLength()));
			break;				
		case 927:	// 주문유형.	(1:시장가   2:지정가 3:역지정가 4:STOP-LIMIT 5:OCO V:추적손실)
			memcpy(OrderInfo.szOrdType, strData, min(sizeof(OrderInfo.szOrdType), strData.GetLength()));
			break;
		case 941:	// 주문일자
			if( nContract == 2 )	//해외선물
				memcpy(OrderInfo.szExeTime, strData, min(sizeof(OrderInfo.szExeTime), strData.GetLength()));
			break;
		case 947:	// 주문시간
			memcpy(OrderInfo.szOpenTime, strData, min(sizeof(OrderInfo.szOpenTime), strData.GetLength()));
			break;
		case 909:	// 주문수량
			OrderInfo.fOrderAmt = atof(strData);
			break;
		case 910:	// 주문가격
			strData.TrimLeft();		strData.TrimRight();
			if( !strData.IsEmpty() && atof(strData) != 0.0 )
				memcpy(OrderInfo.szRate, strData, min(sizeof(OrderInfo.szRate), strData.GetLength()));
			break;
		case 940:	// 체결일자
			memcpy(OrderInfo.szExeTime, strData, min(sizeof(OrderInfo.szExeTime), strData.GetLength()));
			break;
		case 923:	// 체결시간
			memcpy(&OrderInfo.szExeTime[8], strData, min(sizeof(OrderInfo.szExeTime)-8, strData.GetLength()));
			break;
		case 936:	// 체결수량
			OrderInfo.fAmount = atof(strData);
			break;
		case 917:	// 체결가격
			if( !strData.IsEmpty() && atof(strData) != 0.0 )
				memcpy(OrderInfo.szRate, strData, min(sizeof(OrderInfo.szRate), strData.GetLength()));
			break;
		case 945:	// STOP가격
			memcpy(OrderInfo.szStop, strData, min(sizeof(OrderInfo.szStop), strData.GetLength()));
			break;
		case 956:	// LIMIT가격
			memcpy(OrderInfo.szLimit, strData, min(sizeof(OrderInfo.szLimit), strData.GetLength()));
			break;
		}
	}

	if( strMGB == "+매수" )			
	{
		if( strType == "12" )		memcpy(OrderInfo.szSide, "082", 3); //"전매"
		else						memcpy(OrderInfo.szSide, "079", 3);	//"매수"
	}
	else							
	{
		if( strType == "12" )		memcpy(OrderInfo.szSide, "080", 3);	//"환매"
		else						memcpy(OrderInfo.szSide, "081", 3); //"매도"
	}

	// 청산에 대한 STOP/LIMIT 주문인 경우
	if( memcmp(OrderInfo.szKeyType, "OR01", 4) == 0				// 주문
			&& OrderInfo.szOrdType[0] == '5'					// OCO
			&& memcmp(OrderInfo.szTradeType, "12", 2) == 0 )	// 청산
		memcpy(OrderInfo.szKeyType, "CLSL", 4);

	m_pPtSite->ReceiveNoticeData("", (LONG)&OrderInfo);

	return TRUE;
}
//>>

// 100521 alzioyes.
BOOL CPartnerManager::IsRegisterCode(LPCSTR szCode)
{
	//매니저를 사용안할 때는 TRUE리턴.
	if(!m_bUseRegisterCodeMng) return TRUE;
	//return (BOOL)m_RegisterCodeMng.FindCode(szCode);

	//100614-alzioyes.test
	//BOOL bRet = (BOOL)m_RegisterCodeMng.FindCode(szCode);
	BOOL bRet;
	CString strCode = szCode;
	if(szCode[0]=='K' && strlen(szCode)==5 )
	{
		CString strCode(szCode), rCode;
		strCode = strCode.Mid(1);
		if(theApp.m_PtUtilMng.GetSiteUpKode(strCode, rCode))
			strCode = rCode;

		bRet = (BOOL)m_RegisterCodeMng.FindCode(strCode);
// 		{
// 			CString szDebug;
// 			szDebug.Format("%s -> K code\n", szCode);
// 			OutputDebugString(szDebug);
// 			return TRUE;
// 		}
	}
	else if( strCode.GetLength() > 2 && (strCode.Left(2) == "ET" || strCode.Left(2) == "ST" ) )
	{
		CString strCode(szCode), rCode;
		strCode = strCode.Mid(1);
		if(theApp.m_PtUtilMng.GetSiteUpKode(strCode, rCode))
			strCode = rCode;
		
		bRet = (BOOL)m_RegisterCodeMng.FindCode(strCode);
	}
	else
	{
		bRet = (BOOL)m_RegisterCodeMng.FindCode(szCode);
	}

	//100614-alzioyes.test
// 	if(!bRet)
// 	{
// 		CString szDebug;
// 		szDebug.Format("%s -> skip \n", szCode);
// 		OutputDebugString(szDebug);
// 	}
	return bRet;
}

#include "../../inc/IMainExInfoMng.h"
#include "../../inc/IMainInfoMng.h"
#include "../../inc/Condition_MngDef.h"
void CPartnerManager::_DoPreReceiveData(LPCSTR szTRKey, LPBYTE pData, int nSize)
{
	if( strcmp(szTRKey, QUERY_STRDOLLAR)!=0 &&
		strcmp(szTRKey, TRNO_WORKDATE)!=0 &&
		strcmp(szTRKey, TRNO_MSTCODE) !=0 )
		return;

	IMainInfoManagerLast* pMng = (IMainInfoManagerLast*)AfxGetPctrInterface(UUID_IMainInfoManager);
	
	CString szDataDir;
//	szDataDir.Format("%s\\icss\\%s", pMng->GetDataDir(), gszCacheDir);
	szDataDir.Format("%s\\FindData\\%s", pMng->GetDataDir(), gszCacheDir);
	
	CString szIni;
	szIni.Format("%s\\%s", szDataDir, "RequestToday.ini");
	
	CTime tm = CTime::GetCurrentTime();
	CString szKey;
	szKey.Format("%04d%02d%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay());

	if(strcmp(szTRKey, QUERY_STRDOLLAR)==0)
	{
	}
	else if(strcmp(szTRKey, TRNO_WORKDATE)==0)
	{
		IMainExInfoManagerLast* pMng = (IMainExInfoManagerLast*)AfxGetPctrInterface(UUID_IMainExInfoManager);
		if(pMng)
		{
			CString szData;
			szData.Format("%-*.*s", nSize, nSize, pData);
			pMng->SetUserStringData(gszDataKey_WorkDate, (LPSTR)(LPCSTR)szData);
			
			CString szFile;
			szFile.Format("%s\\%s", szDataDir, "WorkDate.dat");
			
			FILE* fp = fopen(szFile, "w");
			if(fp)
			{
				fwrite(pData, 1, nSize, fp);
				fclose(fp);
				
				::WritePrivateProfileString(gszDataKey_WorkDate, "Today", szKey, szIni);
			}
		}
	}
	else if(strcmp(szTRKey, TRNO_MSTCODE)==0)
	{
		CString szFile;
		szFile.Format("%s\\%s", szDataDir, "MstCode.dat");

		char buf[8];
		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, pData, 4);
		int nTotCnt = atoi(buf);

		nSize = nTotCnt * sizeof(MSTCode) + 4;

		FILE* fp = fopen(szFile, "w");
		if(fp)
		{
			fwrite(pData, 1, nSize, fp);
			fclose(fp);
			
			::WritePrivateProfileString(gszDataKey_MstCode, "Today", szKey, szIni);
		}
	}
	
	// gszDataKey_USDollar -> DrDataMng\ChartItemDollar.cpp
	// 달러에 대한 처리는 CChartItemDollar::TRDataConvertSvrToCli(..)에서 하고 있음.
}

//---------------------------------------------------------------------------------------
//
//				class CPartnerSvrManager
//
//---------------------------------------------------------------------------------------
CPartnerSvrManager::CPartnerSvrManager()
{

}
CPartnerSvrManager::~CPartnerSvrManager()
{

}

//int CPartnerSvrManager::RemoveItem(IPartnerManager* pKey)
// {
// 	POSITION tPos;
// 	for(POSITION pos=m_List.GetHeadPosition(); pos;)
// 	{
// 		tPos = pos;
// 		IPartnerManager* pManager = m_List.GetNext(pos);
// 		if(pManager && pManager==pKey)
// 		{
// 			delete pManager;			
// 			m_List.RemoveAt(tPos);
// 			return m_List.GetCount();
// 		}
// 	}
// 	return m_List.GetCount();
// }
int CPartnerSvrManager::RemoveItem(HWND _hWinixWnd, HWND _hChart)
{
	CString szHandle;
	//szHandle.Format("08X", _hWnd);
	szHandle.Format("%08X_%08X", _hWinixWnd, _hChart);

	LPVOID rValue=NULL;
	if(m_mapList.Lookup(szHandle, rValue)==TRUE)
	{
		IPartnerManager* pManager = (IPartnerManager*)rValue;
		if(pManager) delete pManager;
		m_mapList.RemoveKey(szHandle);
	}
	return m_mapList.GetCount();
}

// void CPartnerSvrManager::RemoveAll()
// {
// 	for(POSITION pos=m_List.GetHeadPosition(); pos;)
// 	{
// 		IPartnerManager* pManager = m_List.GetNext(pos);
// 		if(pManager) delete pManager;
// 	}
// 	m_List.RemoveAll();	
// }
void CPartnerSvrManager::RemoveAll()
{
	CString rKey;
	LPVOID rValue=NULL;
	for(POSITION pos=m_mapList.GetStartPosition(); pos;)
	{
		m_mapList.GetNextAssoc(pos, rKey, rValue);
		IPartnerManager* pManager = (IPartnerManager*)rValue;
		if(pManager) delete pManager;
	}
	m_mapList.RemoveAll();
}

IPartnerManager* CPartnerSvrManager::AdviseManager(HWND _hWinixWnd, HWND _hChart)
{
	CPartnerManager *pNewItem= new CPartnerManager;
	pNewItem->SetSendHandle(_hWinixWnd);

	CString szHandle;
	//szHandle.Format("08X", _hWnd);
	szHandle.Format("%08X_%08X", _hWinixWnd, _hChart);

	//m_List.AddTail((IPartnerManager*)pNewItem);
	m_mapList.SetAt(szHandle, pNewItem);

	SetParentHandle('A', _hWinixWnd, _hChart);

	return (IPartnerManager*)pNewItem;
}

//int CPartnerSvrManager::UnAdviseManager(IPartnerManager* pPtMng)
int CPartnerSvrManager::UnAdviseManager(HWND _hWinixWnd, HWND _hChart)
{
	SetParentHandle('U', _hWinixWnd, _hChart);
	return RemoveItem(_hWinixWnd, _hChart);
}

BOOL CPartnerSvrManager::SetParentHandle(BYTE cKey, HWND _hWinixWnd, HWND _hChart)
{
	CString szKey;
	szKey.Format("%08X_%08X", _hWinixWnd, _hChart);

	if(cKey=='A')
	{
		//m_mapHandleList.Add((LPVOID)_hWnd);
		m_mapHandleList.Add(szKey);
		return TRUE;
	}
	else if(cKey=='U')
	{
		CString szItem;
		//HWND hWnd;
		for(int i=0; i<m_mapHandleList.GetSize(); i++)
		{
			//hWnd = (HWND)m_mapHandleList.GetAt(i);
			szItem = m_mapHandleList.GetAt(i);
			//if(hWnd==_hWnd)
			if(szItem.Compare(szKey)==0)
			{
				m_mapHandleList.RemoveAt(i);
				return TRUE;
			}
		}
	}
	return FALSE;
}

int CPartnerSvrManager::GetParentHandle(int nIndex, HWND &_hWinixWnd)
{
	_hWinixWnd = NULL;
	int nAllCnt = m_mapHandleList.GetSize();
	CString szItem;
	HWND hWinixWnd, hChart;

	if(nIndex>=0 && nAllCnt>nIndex) 
	{
		//_hWnd = (HWND)m_mapHandleList.GetAt(nIndex);
		szItem = m_mapHandleList.GetAt(nIndex);
		sscanf((LPCSTR)szItem, "%08X_%08X", &hWinixWnd, &hChart);

		if(::IsWindow(hWinixWnd)==TRUE)
		{
			_hWinixWnd = hWinixWnd;
			return nAllCnt;
		}
//		if(::IsWindow(hWinixWnd)==FALSE)
//		{
//			hWinixWnd = NULL;
//			//SetParentHandle('U', _hWinixWnd, NULL);
//		}
	}

	return nAllCnt;
}

LONG CPartnerSvrManager::GetInerInterface(int nID, long dwKey)
{
	return theApp.m_GateMng.GetInterface(nID, dwKey);
}

