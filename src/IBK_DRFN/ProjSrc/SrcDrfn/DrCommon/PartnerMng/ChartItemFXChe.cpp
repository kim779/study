// ChartItemFXChe.cpp: implementation of the CChartItemFXChe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFXChe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//har* szRowFieldFX_S[]	= { "112302", "112034", "112025", "112129", "112130", "112131", NULL };
//char* szRowFieldFX_B[]	= { "112302", "112034", "112026", "112132", "112133", "112134", NULL };
//char* szMinRowFieldFX_B[]	= { "112302", "112034", "112025", "112026", "112129", "112130", "112131", "112132", "112133", "112134", NULL };
//char* szDayRowFieldFX[]	= { "112302", "112034", "112025", "112026", "112129", "112130", "112131", "112132", "112133", "112134", NULL };
//char* szWeekRowFieldFX[]	= { "112302", "112034", "112025", "112026", "112129", "112130", "112131", "112132", "112133", "112134", NULL };
/* 사용안함
//					      단축코드,   이름, 매도호가, 매수호가, 매도전일종가, 매수전일종가, 매도전일대비, 매수전일대비,
char* szFXGridSymbol[] = { "80301", "80022", "81025", "81026",  "80373",      "80374",      "81136",      "81137",
//							매도시가, 매도고가, 매도저가
							"81129",  "81130", "81131",
//							매수시가, 매수고가, 매수저가
							"81132",  "81133", "81134",
							NULL};
*/
CChartItemFXChe::CChartItemFXChe()
{
	m_bSet = FALSE;
	m_nReqFldCnt = 0;
	m_nFldCnt = 0;//m_nTickCnt = 0;	m_nSecCnt = 0;	m_nMinCnt = 0;	m_nDayCnt = 0;	m_nWeekCnt = 0;	m_nGridCnt=0;
//	while (szRowFieldFX_S[m_nFldCnt] != NULL)		m_nFldCnt++;
}

CChartItemFXChe::~CChartItemFXChe()
{
}

void CChartItemFXChe::SetInData(FX_CHE_RQ_INPUT* pData)
{
	try
	{
		//if(gDebugMng) gDebugMng->TraceTran('I', "SetInit", sizeof(FX_CHE_RQ_INPUT), (LPBYTE)pData);
		if(g_pDRDebuger) g_pDRDebuger->DRLog_RequestData("SetInit", (LPBYTE)pData, sizeof(FX_CHE_RQ_INPUT));
		memcpy(&m_InData, pData, sizeof(FX_CHE_RQ_INPUT));
		m_bSet = TRUE;
	}
	catch (CException* e)
	{
		char aBuff[MAX_PATH]={0,};
		e->GetErrorMessage(aBuff, MAX_PATH);
	}
}

void CChartItemFXChe::SetTR(CString strTR)
{
	m_strTR = strTR;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
//    char    szAccNo[14];				/* 계좌번호 */
//    char    szAccNoPW[8];				/* 비밀번호 */
//    char    szItemCode[16];				/* 종목코드 */
//    char    szChGB[1];					/* 체결구분 */
//    char    szMMGB[1];					/* 매도/매수 구분 */
//    char	  gubun[1];					/* 연속구분 */

CString CChartItemFXChe::Convert()
{
	if(!m_bSet) return "";

	CString strData = "", strAccNo, strAccNoPW, strCode, strTemp, strMMGB, strChGB;

	strAccNo = CString(m_InData.szAccNo, sizeof(m_InData.szAccNo));
	strAccNo.TrimLeft(); strAccNo.TrimRight();

	strAccNoPW = CString(m_InData.szAccNoPW, sizeof(m_InData.szAccNoPW));
	strAccNoPW.TrimLeft(); strAccNoPW.TrimRight();

	strCode = CString(m_InData.szItemCode, sizeof(m_InData.szItemCode));
	strCode.TrimLeft(); strCode.TrimRight();

	strMMGB = CString(m_InData.szMMGB, sizeof(m_InData.szMMGB));
	strMMGB.TrimLeft(); strMMGB.TrimRight();

	strChGB = CString(m_InData.szChGB, sizeof(m_InData.szChGB));
	strChGB.TrimLeft();	strChGB.TrimRight();

	//	계좌번호 Input 심볼
	strData += "901310";	strData += sDS;	strData += strAccNo;	strData += sFS;
	//	계좌비밀번호 Input 심볼
	strData += "901311";	strData += sDS;	strData += strAccNoPW;	strData += sFS;
	// 종목코드 Input 심볼
	strData += "901301";	strData += sDS;	strData += strCode;		strData += sFS;
	//	체결구분 Input 심볼	==>  '1':체결, '2':미체결 => 따로 조회해야한다. 한꺼번에 조회하면 비어있는 값들이 있음
	strData += "901312";	strData += sDS;	strData += strChGB;		strData += sFS;
	//	매도매수구분 Input 심볼 ==> 안먹는듯. 어떤값을 줘도 조회결과가 같음...
	//strData += "901313";	strData += sDS;	strData += strMMGB;		strData += sFS;
	// 연속구분 Input 심볼
	strData += "901319";	strData += sDS;	strData += "0";		strData += sFS;
//	strData += "901319";	strData += sFS;



	//////////////////////////////////////////////////////////////////////////
	// 그리드성 데이터(배열성, 반복데이터) OutBound
	// 챠트 심볼
	strData += _T('$');
	if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// 해외선물
		strData += "901832";
	else
		strData += "901840";
	strData += sDS;

	//////////////////////////////////////////////////////////////////////////
	// 그리드 헤더
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));

	char szTemp[5];

	if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// 해외선물
		memcpy(grid_i.vrow, _T("13"), sizeof(grid_i.vrow));
	else
		memcpy(grid_i.vrow, _T("19"), sizeof(grid_i.vrow));

	sprintf(szTemp, _T("%04d"), 2000);	// 조회 데이터 수
	memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));

	grid_i.vflg[0] = _T('1');
	grid_i.gdir[0] = _T('1');	//	top.
	grid_i.sdir[0] = _T('0');

	if (m_InData.gubun[0] == '1')	grid_i.ikey[0] = _T('2');	//	Page Down
	else							grid_i.ikey[0] = _T('0');	//	일반(Next조회).

	memcpy(grid_i.page, m_InData.keyvalue, 4);
	memcpy(grid_i.save, m_InData.keyvalue+4, sizeof(grid_i.save));

	
	char szGrid_i[128];
	memset(szGrid_i, 0x00, sizeof(szGrid_i));
	memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));

	strData += szGrid_i;
	//////////////////////////////////////////////////////////////////////////
	// Row Data 구성
	m_nReqFldCnt = 0;
/*
	if( m_strTR != QUERY_STRFOFOREIGN_CHE2 )		// FX
	{
		strData += "901918";	strData += sCOL;	m_nReqFldCnt ++;		// 거래유형(11:진입, 12:청산, 13:마진콜)
	}
	strData += "901912";	strData += sCOL;	m_nReqFldCnt ++;		// 매도/매수 구분 "-"/"+"
	strData += "901988";	strData += sCOL;	m_nReqFldCnt ++;		// 주문상태정보
	strData += "901927";	strData += sCOL;	m_nReqFldCnt ++;		// 주문유형(1:시장가   2:지정가 3:역지정가 4:STOP-LIMIT 5:OCO V:추적손실)
	strData += "901904";	strData += sCOL;	m_nReqFldCnt ++;		// 주문번호
	strData += "901941";	strData += sCOL;	m_nReqFldCnt ++;		// 주문일자
	strData += "901947";	strData += sCOL;	m_nReqFldCnt ++;		// 주문시간
	strData += "901909";	strData += sCOL;	m_nReqFldCnt ++;		// 주문수량
	strData += "901910";	strData += sCOL;	m_nReqFldCnt ++;		// 주문가격

	strData += "901948";	strData += sCOL;	m_nReqFldCnt ++;		// 체결번호
	strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// 체결일자
	strData += "901923";	strData += sCOL;	m_nReqFldCnt ++;		// 체결시간
	strData += "901931";	strData += sCOL;	m_nReqFldCnt ++;		// 누적체결수량
	strData += "901916";	strData += sCOL;	m_nReqFldCnt ++;		// 체결가격(평균)
	if( m_strTR != QUERY_STRFOFOREIGN_CHE2 )		// FX
	{
	   strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// 잔고수량(미결제약정 수량)
	}
	strData += "901921";	strData += sCOL;	m_nReqFldCnt ++;		// 미체결수량
	if( m_strTR != QUERY_STRFOFOREIGN_CHE2 )		// FX
	{
		strData += "901318";	strData += sCOL;	m_nReqFldCnt ++;		// 
	}

	strData += "901945";	strData += sCOL;	m_nReqFldCnt ++;		// STOP가격
	strData += "901950";	strData += sCOL;	m_nReqFldCnt ++;		// 포지션번호
	strData += "901975";	strData += sCOL;	m_nReqFldCnt ++;		// 거부사유
*/
	if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// 해외선물
	{
		strData += "901912";	strData += sCOL;	m_nReqFldCnt ++;		// 매도/매수 구분 "-"/"+"
		strData += "901916";	strData += sCOL;	m_nReqFldCnt ++;		// 체결가격(평균)
		strData += "901936";	strData += sCOL;	m_nReqFldCnt ++;		// 체결수량
		strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// 체결일자
		strData += "901923";	strData += sCOL;	m_nReqFldCnt ++;		// 체결시간
	}
	else
	{
		strData += "901918";	strData += sCOL;	m_nReqFldCnt ++;		// 거래유형(11:진입, 12:청산, 13:마진콜)

		strData += "901912";	strData += sCOL;	m_nReqFldCnt ++;		// 매도/매수 구분 "-"/"+"
		strData += "901988";	strData += sCOL;	m_nReqFldCnt ++;		// 주문상태정보
		strData += "901927";	strData += sCOL;	m_nReqFldCnt ++;		// 주문유형(1:시장가   2:지정가 3:역지정가 4:STOP-LIMIT 5:OCO V:추적손실)
		strData += "901904";	strData += sCOL;	m_nReqFldCnt ++;		// 주문번호
		strData += "901941";	strData += sCOL;	m_nReqFldCnt ++;		// 주문일자
		strData += "901947";	strData += sCOL;	m_nReqFldCnt ++;		// 주문시간
		strData += "901909";	strData += sCOL;	m_nReqFldCnt ++;		// 주문수량
		strData += "901910";	strData += sCOL;	m_nReqFldCnt ++;		// 주문가격

		strData += "901948";	strData += sCOL;	m_nReqFldCnt ++;		// 체결번호
		strData += "901940";	strData += sCOL;	m_nReqFldCnt ++;		// 체결일자
		strData += "901923";	strData += sCOL;	m_nReqFldCnt ++;		// 체결시간
		strData += "901931";	strData += sCOL;	m_nReqFldCnt ++;		// 누적체결수량
		strData += "901916";	strData += sCOL;	m_nReqFldCnt ++;		// 체결가격(평균)
		strData += "901921";	strData += sCOL;	m_nReqFldCnt ++;		// 미체결수량

		strData += "901945";	strData += sCOL;	m_nReqFldCnt ++;		// STOP가격
		strData += "901950";	strData += sCOL;	m_nReqFldCnt ++;		// 포지션번호
		strData += "901975";	strData += sCOL;	m_nReqFldCnt ++;		// 거부사유
	}
	strData += sFS;

	return strData;
}

char* CChartItemFXChe::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	FX_CHE_STRUCT	*pOut;

	int nIndex = 0;

	char *pszBuff = NULL, *pszData=NULL;
	pszBuff = new char[nLen+1];
	memset(pszBuff, 0x00, nLen+1);
	memcpy(pszBuff, pData, nLen);
	
	pszData = pszBuff;

	CString strNode, strOpenDate, strOpenTime, strExeDate, strExeTime;
	double	fOrder = 0.;


	// 그리드 헤더
	GRAPH_IO gridOut;

	nIndex = sizeof(gridOut);
	memcpy(&gridOut, pszData, nIndex);
//	memcpy(stOut1.nkey, gridOut.page, sizeof(gridOut.page));

	pszData += nIndex;

	int nSize, nCnt;
	nSize = m_nReqFldCnt;
	CStringArray arrNode;
	int nNodeIndex = 1;
	while ((pszData - pszBuff) < nLen-1)
	{
		pszData += CDataConverter::Parser2(pszData, (nNodeIndex == nSize)?sROW:sCELL, strNode) + 1;
		arrNode.Add(strNode);
		
		if (nNodeIndex == nSize)		nNodeIndex = 1;
		else							nNodeIndex++;
	}

	// 전체 데이터 수 계산
	nCnt = arrNode.GetSize() / nSize;
	pOut = new FX_CHE_STRUCT[nCnt+1];

	nIndex = 0;
	for(int nRow = 0; nRow < nCnt; nRow++)
	{
		memset((char *)&pOut[nRow], 0x20, sizeof(FX_CHE_STRUCT));
/*
		if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// 해외선물
		{
			memcpy(pOut[nRow].szTradeType, "91", 2);
		}
		else
		{
			// 거래유형(11:진입, 12:청산, 13:마진콜)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			memcpy(pOut[nRow].szTradeType, strNode, min(sizeof(pOut[nRow].szTradeType), strNode.GetLength()));
		}


		// 매도/매수 구분 "-"/"+"
		strNode = arrNode.GetAt(nIndex++);
		if( strNode == "+매수" )			memcpy(pOut[nRow].szSide, "079", 3);
		else								memcpy(pOut[nRow].szSide, "081", 3);

		// 주문상태정보
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		//if( strNode == "체결" )				memcpy(pOut[nRow].szKeyType, "CH01", 4);
		//else								memcpy(pOut[nRow].szKeyType, "OR01", 4);
		//memcpy(pOut[nRow].szKeyType, "OR01", 4);//미체결만 조회한 경우
		if( m_InData.szChGB[0] == '1' )		memcpy(pOut[nRow].szKeyType, "CH01", 4);
		else								memcpy(pOut[nRow].szKeyType, "OR01", 4);

		// 주문유형(1:시장가   2:지정가 3:역지정가 4:STOP-LIMIT 5:OCO V:추적손실)
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		memcpy(pOut[nRow].szOrdType, strNode, min(sizeof(pOut[nRow].szOrdType), strNode.GetLength()));

		// 주문번호
		strNode = arrNode.GetAt(nIndex++);
		memcpy(pOut[nRow].szCustItem2, strNode, min(sizeof(pOut[nRow].szCustItem2), strNode.GetLength()));

		// 주문일자
		strNode = arrNode.GetAt(nIndex++);
		strOpenDate = strNode;
		strOpenDate.TrimLeft();		strOpenDate.TrimRight();

		// 주문시간
		strNode = arrNode.GetAt(nIndex++);
		strOpenTime = strNode;
		strOpenTime.TrimLeft();		strOpenTime.TrimRight();
		memcpy(pOut[nRow].szOpenTime, strOpenTime, min(sizeof(pOut[nRow].szOpenTime), strOpenTime.GetLength()));

		// 주문수량
		strNode = arrNode.GetAt(nIndex++);
		fOrder = atof(strNode);

		// 주문가격
		strNode = arrNode.GetAt(nIndex++);
		memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

		// 체결번호. 체결이 되었는데 체결번호가 없는 경우는 주문번호를 체결번호로 간주한다.
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		if( (strNode.IsEmpty() || strNode == "0") &&  memcmp(pOut[nRow].szKeyType, "CH01", 4) == 0 )
			memcpy(pOut[nRow].szFXCMPosID, pOut[nRow].szCustItem2, min(sizeof(pOut[nRow].szFXCMPosID), sizeof(pOut[nRow].szCustItem2)));
		else
			memcpy(pOut[nRow].szFXCMPosID, strNode, min(sizeof(pOut[nRow].szFXCMPosID), strNode.GetLength()));

		// 체결일자
		strNode = arrNode.GetAt(nIndex++);
		strExeDate = strNode;
		strExeDate.TrimLeft();		strExeDate.TrimRight();

		// 체결시간
		strNode = arrNode.GetAt(nIndex++);
		strExeTime = strNode;
		strExeTime.TrimLeft();		strExeTime.TrimRight();
		strExeTime.Remove(':');
		strExeTime = strExeDate + strExeTime;
		memcpy(pOut[nRow].szExeTime, strExeTime, min(sizeof(pOut[nRow].szExeTime), strExeTime.GetLength()));

		// 누적체결수량
		strNode = arrNode.GetAt(nIndex++);
		pOut[nRow].fAmount = atof(strNode);						//체결수량

		// 체결가격(평균)
		strNode = arrNode.GetAt(nIndex++);
		strNode.TrimLeft();		strNode.TrimRight();
		if( strNode.GetLength() > 0 && atof(strNode) != 0.0)
			memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

		
		// 미체결수량
		strNode = arrNode.GetAt(nIndex++);
		pOut[nRow].fOrderAmt = atof(strNode);		

		if( m_strTR != QUERY_STRFOFOREIGN_CHE2 )		// FX
		{
			// 몰라
			strNode = arrNode.GetAt(nIndex++);
		}

		// STOP가격
		strNode = arrNode.GetAt(nIndex++);
		memcpy(pOut[nRow].szStop, strNode, min(sizeof(pOut[nRow].szStop), strNode.GetLength()));

		// 포지션번호
		strNode = arrNode.GetAt(nIndex++);
		memcpy(pOut[nRow].szPositionID, strNode, min(sizeof(pOut[nRow].szPositionID), strNode.GetLength()));

		// 거부사유
		strNode = arrNode.GetAt(nIndex++);
*/
		if( m_strTR == QUERY_STRFOFOREIGN_CHE2 )		// 해외선물
		{
			memcpy(pOut[nRow].szTradeType, "91", 2);
			memcpy(pOut[nRow].szKeyType, "CH01", 4);

			// 매도/매수 구분 "-"/"+"
			strNode = arrNode.GetAt(nIndex++);
			if( strNode == "+매수" )			memcpy(pOut[nRow].szSide, "079", 3);
			else								memcpy(pOut[nRow].szSide, "081", 3);

			// 체결가격(평균)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			if( strNode.GetLength() > 0 && atof(strNode) != 0.0)
				memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

			// 체결수량
			strNode = arrNode.GetAt(nIndex++);
			pOut[nRow].fAmount = atof(strNode);						//체결수량

			// 체결일자
			strNode = arrNode.GetAt(nIndex++);
			strExeDate = strNode;
			strExeDate.TrimLeft();		strExeDate.TrimRight();

			// 체결시간
			strNode = arrNode.GetAt(nIndex++);
			strExeTime = strNode;
			strExeTime.TrimLeft();		strExeTime.TrimRight();
			strExeTime.Remove(':');
			strExeTime = strExeDate + strExeTime;
			memcpy(pOut[nRow].szExeTime, strExeTime, min(sizeof(pOut[nRow].szExeTime), strExeTime.GetLength()));
		}
		else		// FX
		{
			// 거래유형(11:진입, 12:청산, 13:마진콜)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			memcpy(pOut[nRow].szTradeType, strNode, min(sizeof(pOut[nRow].szTradeType), strNode.GetLength()));


			// 매도/매수 구분 "-"/"+"
			strNode = arrNode.GetAt(nIndex++);
			if( strNode == "+매수" )			memcpy(pOut[nRow].szSide, "079", 3);
			else								memcpy(pOut[nRow].szSide, "081", 3);

			// 주문상태정보
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			//if( strNode == "체결" )				memcpy(pOut[nRow].szKeyType, "CH01", 4);
			//else								memcpy(pOut[nRow].szKeyType, "OR01", 4);
			//memcpy(pOut[nRow].szKeyType, "OR01", 4);//미체결만 조회한 경우
			if( m_InData.szChGB[0] == '1' )		memcpy(pOut[nRow].szKeyType, "CH01", 4);
			else								memcpy(pOut[nRow].szKeyType, "OR01", 4);

			// 주문유형(1:시장가   2:지정가 3:역지정가 4:STOP-LIMIT 5:OCO V:추적손실)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			memcpy(pOut[nRow].szOrdType, strNode, min(sizeof(pOut[nRow].szOrdType), strNode.GetLength()));

			// 주문번호
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szCustItem2, strNode, min(sizeof(pOut[nRow].szCustItem2), strNode.GetLength()));

			// 주문일자
			strNode = arrNode.GetAt(nIndex++);
			strOpenDate = strNode;
			strOpenDate.TrimLeft();		strOpenDate.TrimRight();

			// 주문시간
			strNode = arrNode.GetAt(nIndex++);
			strOpenTime = strNode;
			strOpenTime.TrimLeft();		strOpenTime.TrimRight();
			memcpy(pOut[nRow].szOpenTime, strOpenTime, min(sizeof(pOut[nRow].szOpenTime), strOpenTime.GetLength()));

			// 주문수량
			strNode = arrNode.GetAt(nIndex++);
			fOrder = atof(strNode);

			// 주문가격
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));

			// 체결번호. 체결이 되었는데 체결번호가 없는 경우는 주문번호를 체결번호로 간주한다.
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			if( (strNode.IsEmpty() || strNode == "0") &&  memcmp(pOut[nRow].szKeyType, "CH01", 4) == 0 )
				memcpy(pOut[nRow].szFXCMPosID, pOut[nRow].szCustItem2, min(sizeof(pOut[nRow].szFXCMPosID), sizeof(pOut[nRow].szCustItem2)));
			else
				memcpy(pOut[nRow].szFXCMPosID, strNode, min(sizeof(pOut[nRow].szFXCMPosID), strNode.GetLength()));

			// 체결일자
			strNode = arrNode.GetAt(nIndex++);
			strExeDate = strNode;
			strExeDate.TrimLeft();		strExeDate.TrimRight();

			// 체결시간
			strNode = arrNode.GetAt(nIndex++);
			strExeTime = strNode;
			strExeTime.TrimLeft();		strExeTime.TrimRight();
			strExeTime.Remove(':');
			strExeTime = strExeDate + strExeTime;
			memcpy(pOut[nRow].szExeTime, strExeTime, min(sizeof(pOut[nRow].szExeTime), strExeTime.GetLength()));

			// 누적체결수량
			strNode = arrNode.GetAt(nIndex++);
			pOut[nRow].fAmount = atof(strNode);						//체결수량

			// 체결가격(평균)
			strNode = arrNode.GetAt(nIndex++);
			strNode.TrimLeft();		strNode.TrimRight();
			if( strNode.GetLength() > 0 && atof(strNode) != 0.0)
				memcpy(pOut[nRow].szRate, strNode, min(sizeof(pOut[nRow].szRate), strNode.GetLength()));
			
			// 미체결수량
			strNode = arrNode.GetAt(nIndex++);
			pOut[nRow].fOrderAmt = atof(strNode);		

			// STOP가격
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szStop, strNode, min(sizeof(pOut[nRow].szStop), strNode.GetLength()));

			// 포지션번호
			strNode = arrNode.GetAt(nIndex++);
			memcpy(pOut[nRow].szPositionID, strNode, min(sizeof(pOut[nRow].szPositionID), strNode.GetLength()));

			// 거부사유
			strNode = arrNode.GetAt(nIndex++);
		}

	}

	delete []pszBuff;
	pszBuff = NULL;

	nDataLen = sizeof(FX_CHE_STRUCT) * nCnt;
	return (char *)pOut;
}
