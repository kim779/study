// StdDialog.cpp : implementation file
//
#include "stdafx.h"
#include "MultiChart.h"
#include "StdDialog.h"
#include "TabCodeSearch.h"

#include "../../Chart/commontr/ChartRealDef.h"
#include "../../../SrcSite/include/axisfire.h"
// FX차트 주문 연동 - onlyojt (20091204)
#include "../CommonTR/SM_CheJanStruct.h"

void CStdDialog::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);

	if (m_hWinixWnd == NULL)
	{
		IGateManager* pGateMng;
		AfxGetIGateManager(pGateMng);
		if (pGateMng == NULL)
			return;

		IPartnerSvrManager* pMng = (IPartnerSvrManager*)pGateMng->GetInterface(UUID_IPartnerSvrManager);
		if (pMng)
			pMng->GetParentHandle(0, m_hWinixWnd);
	}
}

int CStdDialog::GetMarketTypeFromCode(CString strItemCode, int& nCodeType)
{
	int nRetType = CMasterTypeInfo::NONE_CHART;
	nCodeType = -1;
	if (strItemCode.IsEmpty())
		return -1;

	if (m_hWinixWnd)
	{
		nCodeType = (int)::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)GetRealItemCode(strItemCode));
		if (nCodeType == gfxType)
			nRetType = CMasterTypeInfo::FX_CHART;
	}

	return nRetType;
}

void CStdDialog::RecieveRealData(LPCTSTR pszCode, CMapStringToString* mapRealData)
{
	BOOL bSign = FALSE;
	CRealData_Common_FX RealData;

	CStringArray	arrFID;

	arrFID.Add("034");		// 체결시간
	// 매도
	arrFID.Add("025");		// 매도현재가
	arrFID.Add("136");		// 매도전일대비
	arrFID.Add("129");		// 매도시가
	arrFID.Add("130");		// 매도고가
	arrFID.Add("131");		// 매도저가
	// 매수
	arrFID.Add("026");		// 매수현재가
	arrFID.Add("137");		// 매수전일대비
	arrFID.Add("132");		// 매수시가
	arrFID.Add("133");		// 매수고가
	arrFID.Add("134");		// 매수저가


	RealData.m_strCode = RealData.m_RealData_S.m_strCode = RealData.m_RealData_B.m_strCode		= pszCode;

//체결시간
	mapRealData->Lookup(arrFID.GetAt(0), RealData.m_RealData_S.m_strTime);	

	if (RealData.m_RealData_S.m_strTime.CompareNoCase(_T("장종료")) == 0)
		RealData.m_RealData_S.m_strTime = _T("9999");
	else if (RealData.m_RealData_S.m_strTime.CompareNoCase(_T("장마감")) == 0)
		RealData.m_RealData_S.m_strTime = _T("8888");
	if (atoi(RealData.m_RealData_S.m_strTime) <= 0)
	{
		return;
	}
	RealData.m_strTime = RealData.m_RealData_B.m_strTime = RealData.m_RealData_S.m_strTime;

// 매도현재가
	mapRealData->Lookup(arrFID.GetAt(1), RealData.m_RealData_S.m_strPrice);
	RealData.m_RealData_S.m_strPrice.TrimLeft();
	RealData.m_RealData_S.m_strPrice.TrimRight();

	// 실시간 가격데이타는 앞에 1Byte가 등락구분이므로 1Byte를 빼준다.
	RealData.m_RealData_S.m_strPrice = RealData.m_RealData_S.m_strPrice.Mid(1);
			

// 매도전일대비
	//등락부호:1:상한,2:상승,3:보합,4:하한,5:하락
	mapRealData->Lookup(arrFID.GetAt(2), RealData.m_RealData_S.m_strChange);	//전일대비

	char cSign = RealData.m_RealData_S.m_strChange.GetAt(0);
	switch(cSign)
	{
	case ' ': RealData.m_RealData_S.m_strSign = "3"; break;
	case '+': RealData.m_RealData_S.m_strSign = "2"; break;
	case '-': RealData.m_RealData_S.m_strSign = "5"; break;
	}
	RealData.m_RealData_S.m_strChange = RealData.m_RealData_S.m_strChange.Mid(1);

// 매도시가
	mapRealData->Lookup(arrFID.GetAt(3), RealData.m_RealData_S.m_strOpen);	//시가
	RealData.m_RealData_S.m_strOpen = RealData.m_RealData_S.m_strOpen.Mid(1);
// 매도고가
	mapRealData->Lookup(arrFID.GetAt(4), RealData.m_RealData_S.m_strHigh);	//고가
	RealData.m_RealData_S.m_strHigh = RealData.m_RealData_S.m_strHigh.Mid(1);
// 매도저가
	mapRealData->Lookup(arrFID.GetAt(5), RealData.m_RealData_S.m_strLow);	//저가
	RealData.m_RealData_S.m_strLow = RealData.m_RealData_S.m_strLow.Mid(1);


// 매수현재가
	mapRealData->Lookup(arrFID.GetAt(6), RealData.m_RealData_B.m_strPrice);
	RealData.m_RealData_B.m_strPrice.TrimLeft();
	RealData.m_RealData_B.m_strPrice.TrimRight();

	// 실시간 가격데이타는 앞에 1Byte가 등락구분이므로 1Byte를 빼준다.
	RealData.m_RealData_B.m_strPrice = RealData.m_RealData_B.m_strPrice.Mid(1);
			

// 매수전일대비
	//등락부호:1:상한,2:상승,3:보합,4:하한,5:하락
	mapRealData->Lookup(arrFID.GetAt(7), RealData.m_RealData_B.m_strChange);	//전일대비

	cSign = RealData.m_RealData_B.m_strChange.GetAt(0);
	switch(cSign)
	{
	case ' ': RealData.m_RealData_B.m_strSign = "3"; break;
	case '+': RealData.m_RealData_B.m_strSign = "2"; break;
	case '-': RealData.m_RealData_B.m_strSign = "5"; break;
	}
	RealData.m_RealData_B.m_strChange = RealData.m_RealData_B.m_strChange.Mid(1);

// 매수시가
	mapRealData->Lookup(arrFID.GetAt(8), RealData.m_RealData_B.m_strOpen);	//시가
	RealData.m_RealData_B.m_strOpen = RealData.m_RealData_B.m_strOpen.Mid(1);
// 매수고가
	mapRealData->Lookup(arrFID.GetAt(9), RealData.m_RealData_B.m_strHigh);	//고가
	RealData.m_RealData_B.m_strHigh = RealData.m_RealData_B.m_strHigh.Mid(1);
// 매수저가
	mapRealData->Lookup(arrFID.GetAt(10), RealData.m_RealData_B.m_strLow);	//저가
	RealData.m_RealData_B.m_strLow = RealData.m_RealData_B.m_strLow.Mid(1);

	
	//pThis->RecieveRealData(dwKey, (LONG)&RealData);
	m_ChartMng.SetRealJPriceData((LPVOID)&RealData);
	return;
}

void CStdDialog::RecieveRealDataEx(LPCTSTR pszCode, DWORD* pArrData)
{
	BOOL bSign = FALSE;
	CRealData_Common_FX RealData;

	RealData.m_strCode = RealData.m_RealData_S.m_strCode = RealData.m_RealData_B.m_strCode		= pszCode;


//aaaaaaaaaaaaaaaaaaaaaaaaa디버기용 나중에 반드시 지우자...
	CString szFlag = (char*)pArrData[0];
	if( szFlag.GetLength() > 0 )
	{
 		char chGB = szFlag.GetAt(0);
		int abc = 0;
 		if( chGB == '3' )		
			abc ++;
		else
			abc --;
	}
//aaaaaaaaaaaaaaaaaaaaaaaaaaaa


//체결시간
	//mapRealData->Lookup(arrFID.GetAt(0), RealData.m_RealData_S.m_strTime);	
	RealData.m_RealData_S.m_strTime = (char*)pArrData[ 34];

	if (RealData.m_RealData_S.m_strTime.CompareNoCase(_T("장종료")) == 0)
		RealData.m_RealData_S.m_strTime = _T("9999");
	else if (RealData.m_RealData_S.m_strTime.CompareNoCase(_T("장마감")) == 0)
		RealData.m_RealData_S.m_strTime = _T("8888");
	if (atoi(RealData.m_RealData_S.m_strTime) <= 0)
	{
		return;
	}
	RealData.m_strTime = RealData.m_RealData_B.m_strTime = RealData.m_RealData_S.m_strTime;

// 매도현재가
	//mapRealData->Lookup(arrFID.GetAt(1), RealData.m_RealData_S.m_strPrice);
	RealData.m_RealData_S.m_strPrice = (char*)pArrData[ 25];
	RealData.m_RealData_S.m_strPrice.TrimLeft();
	RealData.m_RealData_S.m_strPrice.TrimRight();

	// 실시간 가격데이타는 앞에 1Byte가 등락구분이므로 1Byte를 빼준다.
	RealData.m_RealData_S.m_strPrice = RealData.m_RealData_S.m_strPrice.Mid(1);
			

// 매도전일대비
	//등락부호:1:상한,2:상승,3:보합,4:하한,5:하락
	//mapRealData->Lookup(arrFID.GetAt(2), RealData.m_RealData_S.m_strChange);	//전일대비
	RealData.m_RealData_S.m_strChange = (char*)pArrData[136];

	char cSign = RealData.m_RealData_S.m_strChange.GetAt(0);
	switch(cSign)
	{
	case ' ': RealData.m_RealData_S.m_strSign = "3"; break;
	case '+': RealData.m_RealData_S.m_strSign = "2"; break;
	case '-': RealData.m_RealData_S.m_strSign = "5"; break;
	}
	RealData.m_RealData_S.m_strChange = RealData.m_RealData_S.m_strChange.Mid(1);

// 매도시가
	//mapRealData->Lookup(arrFID.GetAt(3), RealData.m_RealData_S.m_strOpen);	//시가
	RealData.m_RealData_S.m_strOpen = (char*)pArrData[129];
	RealData.m_RealData_S.m_strOpen = RealData.m_RealData_S.m_strOpen.Mid(1);
// 매도고가
	//mapRealData->Lookup(arrFID.GetAt(4), RealData.m_RealData_S.m_strHigh);	//고가
	RealData.m_RealData_S.m_strHigh = (char*)pArrData[130];
	RealData.m_RealData_S.m_strHigh = RealData.m_RealData_S.m_strHigh.Mid(1);
// 매도저가
	//mapRealData->Lookup(arrFID.GetAt(5), RealData.m_RealData_S.m_strLow);	//저가
	RealData.m_RealData_S.m_strLow = (char*)pArrData[131];
	RealData.m_RealData_S.m_strLow = RealData.m_RealData_S.m_strLow.Mid(1);


// 매수현재가
	//mapRealData->Lookup(arrFID.GetAt(6), RealData.m_RealData_B.m_strPrice);
	RealData.m_RealData_B.m_strPrice = (char*)pArrData[ 26];
	RealData.m_RealData_B.m_strPrice.TrimLeft();
	RealData.m_RealData_B.m_strPrice.TrimRight();

	// 실시간 가격데이타는 앞에 1Byte가 등락구분이므로 1Byte를 빼준다.
	RealData.m_RealData_B.m_strPrice = RealData.m_RealData_B.m_strPrice.Mid(1);
			

// 매수전일대비
	//등락부호:1:상한,2:상승,3:보합,4:하한,5:하락
	//mapRealData->Lookup(arrFID.GetAt(7), RealData.m_RealData_B.m_strChange);	//전일대비
	RealData.m_RealData_B.m_strChange = (char*)pArrData[137];

	cSign = RealData.m_RealData_B.m_strChange.GetAt(0);
	switch(cSign)
	{
	case ' ': RealData.m_RealData_B.m_strSign = "3"; break;
	case '+': RealData.m_RealData_B.m_strSign = "2"; break;
	case '-': RealData.m_RealData_B.m_strSign = "5"; break;
	}
	RealData.m_RealData_B.m_strChange = RealData.m_RealData_B.m_strChange.Mid(1);

// 매수시가
	//mapRealData->Lookup(arrFID.GetAt(8), RealData.m_RealData_B.m_strOpen);	//시가
	RealData.m_RealData_B.m_strOpen = (char*)pArrData[132];
	RealData.m_RealData_B.m_strOpen = RealData.m_RealData_B.m_strOpen.Mid(1);
// 매수고가
	//mapRealData->Lookup(arrFID.GetAt(9), RealData.m_RealData_B.m_strHigh);	//고가
	RealData.m_RealData_B.m_strHigh = (char*)pArrData[133];
	RealData.m_RealData_B.m_strHigh = RealData.m_RealData_B.m_strHigh.Mid(1);
// 매수저가
	//mapRealData->Lookup(arrFID.GetAt(10), RealData.m_RealData_B.m_strLow);	//저가
	RealData.m_RealData_B.m_strLow = (char*)pArrData[134];
	RealData.m_RealData_B.m_strLow = RealData.m_RealData_B.m_strLow.Mid(1);

//	{
//		CString szDebug;
//		szDebug.Format("[SMalzio]S_close=%s, B_close=%s\n", RealData.m_RealData_S.m_strPrice, RealData.m_RealData_B.m_strPrice);
//		OutputDebugString(szDebug);
//	}

	//pThis->RecieveRealData(dwKey, (LONG)&RealData);
	m_ChartMng.SetRealJPriceData((LPVOID)&RealData);

	return;
}

//---------------------------------------------------------------------------
// Function name	: ConfirmMap
// Description		: 주문확인창
// Return type		: CString 
// Argument		:
//	CString strAccount		: 계좌번호
//  CString strAccountName	: 계좌명
//	CString strCodeName		: 종목
//	CString strFDM 			: FDM 
//	CString strOrderType	: 주문구분(보통/시장가 등...)
//	CString strQuantity		: 수량
//	CString strPrice		: 가격
//	CString strJmNum		: 주문번호
//	int nOrderType			: 0-매도 1-매수 2-정정 3-취소 4-T.STOP 5-청산 6-STOP/LIMIT
//
//[FX미니주문 연동하기- SI907000]
//
//도미노데이터 2개
//mmgb : 0-매도 1-매수 2-정정 3-취소
//domino : (계좌번호|비밀번호|종목코드|주문단가|주문수량)
//ex)
//Dim sdata
//
//sdata = "03954700084|1234|EUR_USD.H|1234|5"
//
//SCREEN.CreateWindow 1, "SI907000 /p5 /S/t/d", "mmgb"&vbTab&"0"&vbLf&"domino"&vbTab&sdata&vbLf 
//---------------------------------------------------------------------------
BOOL CStdDialog::ConfirmMap(CString strAccount, CString strAccountName, CString strPassword, CString strCodeName, 
							CString strFDM , CString strOrderType,  CString strQuantity, 
							CString strPrice, CString strJmNum,  CString strStopPrice,
							int nOrderType, int nMapType/*=0*/)
{
//FX 미니주문창
	CString strMapNo;//(_T("SI907000"));

	if( nMapType == 1 )					// double click한 경우
		strMapNo = _T("SI907000");
	else if( nOrderType == 0 )			// 매도
		strMapNo = _T("SI9065Z1");
	else if( nOrderType == 1 )			// 매수
		strMapNo = _T("SI9065Z0");
	else
		strMapNo = _T("SI907000");

	CString strParam(_T(""));
	CString strKey(_T("stock_order_"));
	
	strQuantity.TrimLeft("0");
	strPrice.TrimLeft("0");
	
	 
	switch (nOrderType)
	{
	case 0: //매도
	case 1: //매수
		strParam.Format("%s /p5 /S/t/d mmgb\t%d\ndomino\t%s|%s|%s|%s|%s|\n",
			strMapNo, nOrderType, strAccount, strPassword, strCodeName, strPrice, strQuantity);
		break;
		
	case 2: //정정
	case 3:	//취소	
		strParam.Format("%s /p5 /S/t/d mmgb\t%d\ndomino\t%s|%s|%s|%s|%s|%s|\n",
			strMapNo, nOrderType, strAccount, strPassword, strCodeName, strPrice, strQuantity, strJmNum);
		break;

	case 4: //T.Stop
	case 5:	//청산	
	case 6:	//STOP/LIMIT
		strParam.Format("%s /p5 /S/t/d mmgb\t%d\ndomino\t%s|%s|%s|%s|%s|%s|\n",
			strMapNo, nOrderType, strAccount, strPassword, strCodeName, strPrice, strQuantity, strJmNum);
		break;

	}
	
	::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(viewDLL, typeMODAL), (LPARAM)(LPCTSTR)strParam);

//주문확인창
/*	
	CString strMapNo(_T("SI9064H"));
	CString strParam(_T(""));
	CString strKey(_T("stock_order_"));
	
	strQuantity.TrimLeft("0");
	strPrice.TrimLeft("0");
	
	 
	strMapNo.Format("SI9064H%d",  nOrderType); // 1매수 2매도 3정정 4취소 
	switch (nOrderType)
	{
	case 1:
	case 2:
		strParam.Format("%s/s/p5/t/d o_acno\t%s\no_acnm\t%s\no_hnam\t%s\no_fdm\t%s\no_hogb\t%s\no_jqty\t%s\no_jprc\t%s\no_stopjprc\t%s",
			strMapNo, strAccount, strAccountName, strCodeName, strFDM, strOrderType, strQuantity, strPrice, strStopPrice);
		break;
		
	case 3:
		strParam.Format("%s/s/p5/t/d o_acno\t%s\no_acnm\t%s\no_hnam\t%s\no_fdm\t%s\no_ogno\t%s\no_jprc\t%s\n",
			strMapNo, strAccount, strAccountName, strCodeName, strFDM, strJmNum, strPrice);
		break;
		
	case 4:		
		strParam.Format("%s/s/p5/t/d o_acno\t%s\no_acnm\t%s\no_hnam\t%s\no_fdm\t%s\no_ogno\t%s\n",
			strMapNo, strAccount, strAccountName, strCodeName, strFDM, strJmNum);
		break;
	}
	
	::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(viewDLL, typeMODAL), (LPARAM)(LPCTSTR)strParam);
	if (nOrderType < 5)
		strKey += (char)(nOrderType + 0x30);
	else
		strKey += (char)(nOrderType - 4 + 0x30);
	
//	CString strResult = m_pApp->Variant(popCC, strKey);
//	return (strResult == _T("OK")) ? TRUE : FALSE; 
*/
	return TRUE;
}

HRESULT CStdDialog::CPartnerSite::RecieveFidData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveTRData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	CString strTR(szTR);
//	if (dwKey == TRKEY_WORKDATE)		strTR = TRNO_WORKDATE;
//	else if (dwKey == TRKEY_STOCK_HOGA)	strTR = QUERY_STOCK_HOGA;
	
	pThis->ReceiveData(dwKey, strTR, szMsgCode, szMessage, aTRData, dwTRDateLen);

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealData(long dwKey, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

//	pThis->m_ChartMng.SetRealJPriceData((LPVOID)dwData);
	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveKeyData(int nKey, LPCSTR _szData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	switch(nKey)
	{
		case IPartnerSite::IPS_RK_shareCode:
			//조회중이면 return;
			if (pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;
			else			//WPARAM 종목코드, LPARAM MarketType	
			{
				CString szData(_szData), szCode,szSymbol;
				int nPos = szData.Find('\t');
				if(nPos<0) return FALSE;

				szSymbol = szData.Left(nPos);
				szCode = szData.Mid(nPos+1);
				szCode.TrimRight();

				int nCodeType;
				pThis->GetMarketTypeFromCode(szCode, nCodeType);
				if (nCodeType == gfxType)
				{
//					pThis->DoNotifyChangeCode(FALSE);
					pThis->ChangeCode(szCode);
//					pThis->DoNotifyChangeCode(TRUE);
				}
			}
			break;
		case IPartnerSite::IPS_RK_codeDlg: //종목Dialog에서 선택한 Code
			if (pThis->m_pInputBarDlg->IsWindowEnabled())
			{
				CString szCode(_szData);
				int nPos = szCode.Find('\t');
				if (nPos > 0)
					szCode = szCode.Left(nPos);

				int nCodeType;
				pThis->GetMarketTypeFromCode(szCode, nCodeType);
				if (nCodeType == gfxType)
				{
					pThis->ChangeCode(szCode);
					if (g_pPtUtilMng)
						g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
				}
			}
			break;

		case IPartnerSite::IPS_RK_codeHidDlg:
			break;
		case IPartnerSite::IPS_RK_gwansim:
			break;
	}

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealDataEx(long dwKey, LPCTSTR pszCode, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	if( dwKey == 0 )
	{
		CMapStringToString *mapRealData = (CMapStringToString *)dwData;
		pThis->RecieveRealData(pszCode, mapRealData);
	}
	else
	{
		pThis->RecieveRealDataEx(pszCode, (DWORD*)dwData);
	}
/*
	CMapStringToString *mapRealData = (CMapStringToString *)dwData;
	BOOL bSign = FALSE;
	CRealData_Common_FX RealData;

	CStringArray	arrFID;

	arrFID.Add("034");		// 체결시간
	// 매도
	arrFID.Add("025");		// 매도현재가
	arrFID.Add("136");		// 매도전일대비
	arrFID.Add("129");		// 매도시가
	arrFID.Add("130");		// 매도고가
	arrFID.Add("131");		// 매도저가
	// 매수
	arrFID.Add("026");		// 매수현재가
	arrFID.Add("137");		// 매수전일대비
	arrFID.Add("132");		// 매수시가
	arrFID.Add("133");		// 매수고가
	arrFID.Add("134");		// 매수저가


	RealData.m_RealData_S.m_strCode = RealData.m_RealData_B.m_strCode		= pszCode;

//체결시간
	mapRealData->Lookup(arrFID.GetAt(0), RealData.m_RealData_S.m_strTime);	

	if (RealData.m_RealData_S.m_strTime.CompareNoCase(_T("장종료")) == 0)
		RealData.m_RealData_S.m_strTime = _T("9999");
	else if (RealData.m_RealData_S.m_strTime.CompareNoCase(_T("장마감")) == 0)
		RealData.m_RealData_S.m_strTime = _T("8888");
	if (atoi(RealData.m_RealData_S.m_strTime) <= 0)
	{
		return -1;
	}
	RealData.m_RealData_B.m_strTime = RealData.m_RealData_S.m_strTime;

// 매도현재가
	mapRealData->Lookup(arrFID.GetAt(1), RealData.m_RealData_S.m_strPrice);
	RealData.m_RealData_S.m_strPrice.TrimLeft();
	RealData.m_RealData_S.m_strPrice.TrimRight();

	// 실시간 가격데이타는 앞에 1Byte가 등락구분이므로 1Byte를 빼준다.
	RealData.m_RealData_S.m_strPrice = RealData.m_RealData_S.m_strPrice.Mid(1);
			

// 매도전일대비
	//등락부호:1:상한,2:상승,3:보합,4:하한,5:하락
	mapRealData->Lookup(arrFID.GetAt(2), RealData.m_RealData_S.m_strChange);	//전일대비

	char cSign = RealData.m_RealData_S.m_strChange.GetAt(0);
	switch(cSign)
	{
	case ' ': RealData.m_RealData_S.m_strSign = "3"; break;
	case '+': RealData.m_RealData_S.m_strSign = "2"; break;
	case '-': RealData.m_RealData_S.m_strSign = "5"; break;
	}
	RealData.m_RealData_S.m_strChange = RealData.m_RealData_S.m_strChange.Mid(1);

// 매도시가
	mapRealData->Lookup(arrFID.GetAt(3), RealData.m_RealData_S.m_strOpen);	//시가
	RealData.m_RealData_S.m_strOpen = RealData.m_RealData_S.m_strOpen.Mid(1);
// 매도고가
	mapRealData->Lookup(arrFID.GetAt(4), RealData.m_RealData_S.m_strHigh);	//고가
	RealData.m_RealData_S.m_strHigh = RealData.m_RealData_S.m_strHigh.Mid(1);
// 매도저가
	mapRealData->Lookup(arrFID.GetAt(5), RealData.m_RealData_S.m_strLow);	//저가
	RealData.m_RealData_S.m_strLow = RealData.m_RealData_S.m_strLow.Mid(1);


// 매수현재가
	mapRealData->Lookup(arrFID.GetAt(6), RealData.m_RealData_B.m_strPrice);
	RealData.m_RealData_B.m_strPrice.TrimLeft();
	RealData.m_RealData_B.m_strPrice.TrimRight();

	// 실시간 가격데이타는 앞에 1Byte가 등락구분이므로 1Byte를 빼준다.
	RealData.m_RealData_B.m_strPrice = RealData.m_RealData_B.m_strPrice.Mid(1);
			

// 매수전일대비
	//등락부호:1:상한,2:상승,3:보합,4:하한,5:하락
	mapRealData->Lookup(arrFID.GetAt(7), RealData.m_RealData_B.m_strChange);	//전일대비

	cSign = RealData.m_RealData_B.m_strChange.GetAt(0);
	switch(cSign)
	{
	case ' ': RealData.m_RealData_B.m_strSign = "3"; break;
	case '+': RealData.m_RealData_B.m_strSign = "2"; break;
	case '-': RealData.m_RealData_B.m_strSign = "5"; break;
	}
	RealData.m_RealData_B.m_strChange = RealData.m_RealData_B.m_strChange.Mid(1);

// 매수시가
	mapRealData->Lookup(arrFID.GetAt(8), RealData.m_RealData_B.m_strOpen);	//시가
	RealData.m_RealData_B.m_strOpen = RealData.m_RealData_B.m_strOpen.Mid(1);
// 매수고가
	mapRealData->Lookup(arrFID.GetAt(9), RealData.m_RealData_B.m_strHigh);	//고가
	RealData.m_RealData_B.m_strHigh = RealData.m_RealData_B.m_strHigh.Mid(1);
// 매수저가
	mapRealData->Lookup(arrFID.GetAt(10), RealData.m_RealData_B.m_strLow);	//저가
	RealData.m_RealData_B.m_strLow = RealData.m_RealData_B.m_strLow.Mid(1);

	
	//pThis->RecieveRealData(dwKey, (LONG)&RealData);
	pThis->m_ChartMng.SetRealJPriceData((LPVOID)&RealData);
*/
	return -1;
}

HRESULT CStdDialog::CPartnerSite::ReceiveOnUser(WPARAM wParam, LPARAM lParam)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

//<<20100412_JS.Kim FX. 체결데이타 실시간 수신
HRESULT CStdDialog::CPartnerSite::ReceiveNoticeData(LPCTSTR pszCode, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	pThis->ReceiveNoticeData(pszCode, dwData);

	return S_OK;
}

void CStdDialog::ReceiveNoticeData(LPCTSTR pszCode, LONG dwData)
{
	FX_CHE_STRUCT* pOrderInfomation = (FX_CHE_STRUCT*)dwData;
	if(!pOrderInfomation)
		return;
			
	CString strCodeInputCode = m_pInputBarDlg->m_pCodeInput->GetCode();

	if(strCodeInputCode.IsEmpty())
		return;

	CString strAccount = "", strAccName = "", strAccPW = "";
	if( m_pExtInputBarDlg )	m_pExtInputBarDlg->GetAccountInfo(strAccount, strAccName, strAccPW);
			
	CString strItemCode (pOrderInfomation->SHCODE,	sizeof(pOrderInfomation->SHCODE));
	CString strAccNo (pOrderInfomation->szAccNo,	sizeof(pOrderInfomation->szAccNo));

	strItemCode.TrimRight ();
	strCodeInputCode.TrimRight ();
	strAccount.TrimRight ();
	strAccNo.TrimRight ();
			
	if (strCodeInputCode == strItemCode && strAccount == strAccNo)
	{
		CString strKeyType(pOrderInfomation->szKeyType, sizeof(pOrderInfomation->szKeyType));

		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		if(pChartItem)
		{
			if(strKeyType == "CH01" || strKeyType == "CLSL")		// 체결
			{
				pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 3, (long)pOrderInfomation);
				//pChartItem->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 3, (long)pOrderInfomation);				
			}
			else if(strKeyType == "OR01" )	// 미체결
			{
				pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 4, (long)pOrderInfomation);
				//pChartItem->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 4, (long)pOrderInfomation);
			}
		}
	}
}
//>>