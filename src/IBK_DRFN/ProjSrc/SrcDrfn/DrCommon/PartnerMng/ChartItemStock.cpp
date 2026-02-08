// ChartItemStock.cpp: implementation of the CChartItemStock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemStock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szTickRowField[]	= { "2302", "2034", "2023", "2029", "2030", "2031", "2032", "2028", NULL };
char* szSecRowField[]	= { "6302", "6034", "6023", "6029", "6030", "6031", "6032", "6028", NULL };
char* szMinRowField[]	= { "4302", "4034", "4023", "4029", "4030", "4031", "4032", "1028", NULL };
char* szDayRowField[]	= { "5302", "5023", "5029", "5030", "5031", "5027", "5028", "5311", "5024", NULL };
char* szWeekRowField[]	= { "5302", "5023", "5029", "5030", "5031", "5027", "5028", NULL };
char* szYearRowField[]	= { "5302", "5023", "5029", "5030", "5031", "5027", "5028", NULL };

// 전일종가("2320") -> 기준가("1336") 으로의 변경.
//					            종목코드, 이름, 현재가, 전일종가, 증감, 등락율, 누적거래량, 전일거래량
//char* szStockGridSymbol[] = { "1301", "1022", "2023", "2320", "2024", "2033", "2027", "2321",
char* szStockGridSymbol[] = { "1301", "1022", "1023", "1336", "1024", "1033", "1027", "1314",
//			상한가,하한가 ,  시가,  고가,  저가,  전일시가, 전일고가, 전일저가,
			"1311", "1312", "1029", "1030", "1031", "1320", "1321", "1322",
			//장정보, 예상장시작시간, 예상장운영시간종료(HH::MM), 누적거래대금(일)
			"1307"	, "1900"		, "1901"					, "1028",			NULL};

CChartItemStock::CChartItemStock()
{
	m_bSet = FALSE;
	m_nTickCnt = 0;	m_nSecCnt = 0;	m_nMinCnt = 0;	m_nDayCnt = 0;	m_nWeekCnt = 0;	m_nYearCnt = 0; m_nGridCnt=0;
	while (szTickRowField[m_nTickCnt] != NULL)		m_nTickCnt++;
	while (szSecRowField[m_nSecCnt] != NULL)		m_nSecCnt++;
	while (szMinRowField[m_nMinCnt] != NULL)		m_nMinCnt++;
	while (szDayRowField[m_nDayCnt] != NULL)		m_nDayCnt++;
	while (szWeekRowField[m_nWeekCnt] != NULL)		m_nWeekCnt++;
	while (szYearRowField[m_nYearCnt] != NULL)		m_nYearCnt++;
	while (szStockGridSymbol[m_nGridCnt] != NULL)	m_nGridCnt++;
}

CChartItemStock::~CChartItemStock()
{
}

void CChartItemStock::SetInData(KB_p0602_InRec1* pData)
{
	try
	{
		//if(gDebugMng) gDebugMng->TraceTran('I', "SetInit", sizeof(KB_p0602_InRec1), (LPBYTE)pData);
		if(g_pDRDebuger) g_pDRDebuger->DRLog_RequestData("SetInit", (LPBYTE)pData, sizeof(KB_p0602_InRec1));
		int i = sizeof(KB_p0602_InRec1);
		memcpy(&m_InData, pData, sizeof(KB_p0602_InRec1));
		m_bSet = TRUE;
	}
	catch (CException* e)
	{
		char aBuff[MAX_PATH]={0,};
		e->GetErrorMessage(aBuff, MAX_PATH);
	}
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemStock::Convert()
{
	if(!m_bSet) 
		return "";

	CString strData, strCode, strTemp, strDate, strNow, strUnit, strTime, strImgRgb;
	int i, nCount, nUnit, nType, nFlag;
	double dUSDollar=1;

	strCode	= CDataConverter::Char2String(m_InData.shcode, sizeof(m_InData.shcode));
	strDate	= CDataConverter::Char2String(m_InData.date, sizeof(m_InData.date));
	strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	strTime	= CDataConverter::Char2String(m_InData.Itime, sizeof(m_InData.Itime));
	nCount	= CDataConverter::Char2Int(m_InData.num, sizeof(m_InData.num));
	nUnit	= CDataConverter::Char2Int(m_InData.unit, sizeof(m_InData.unit));
	nType	= CDataConverter::Char2Int(m_InData.button, sizeof(m_InData.button));	// 틱(0)/분(1)/일(2)/주(3)/월(4)
	strImgRgb = CDataConverter::Char2String(m_InData.ImgRgb, sizeof(m_InData.ImgRgb));
	nFlag     = CDataConverter::Char2Int(m_InData.nflag, sizeof(m_InData.nflag));	// 0:
	//strDate = "20120531";

	strUnit.Format(_T("%d"), nUnit);
	
	//////////////////////////////////////////////////////////////////////////
	//	Inbound Data

	//	종목코드 Input 심볼
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	strData += "1301";	strData += sDS;	strData += strCode;	strData += sCELL;//KHD sFS;
	CString stRowData;
	CString stNextData, stNextKey, stNextKey2;

	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.nkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}
	//환율
	strData += "51305"; strData += sDS;	strData += "USDKRWSMBS";	strData += sCELL; //입력 OOP 
	//END 
	//////////////////////////////////////////////////////////////////////////

	stNextData.Format("%28.28s", m_InData.nkey);

	switch (nType)
	{
	case 0:		// 
			Datah.dindex = GI_TICK;
			Datah.dunit = GI_TICK;
			stRowData = _T("?2500");
			stNextKey = "2324";
			// 		// 당일만조회 Input 심볼 (1:당일) 
			strData += "2309";	strData += sDS;	strData += strNow;	strData += sCELL;
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
// 		//	복기차트 조회 시간
 		if (m_InData.Irecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.nDayBefore, sizeof(m_InData.nDayBefore));	// 0:
			CString stBefore;
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : 당일 , 1: 전
 			strData += "2341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
//END
		break;

	case 1:		// 분
			Datah.dindex = GI_MINUTE;
			Datah.dunit = GI_MINUTE;
			stRowData = _T("?4500");
		
//KHD
		stNextKey = "4324";
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
// 		// 실/허봉 구분 (0:허봉, 1:실봉)
 		strData += "4308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		// 		// 당일만조회 Input 심볼 (1:당일) 
 		strData += "4309";	strData += sDS;	strData += strNow;	strData += sCELL;

		// 수정주가
		strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
		strData += "4339";	strData += sDS;	strData += strTemp;	strData += sCELL;

		if (m_InData.Irecordgb[0] == '1')	//	복기차트 조회 시간
 		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.nDayBefore, sizeof(m_InData.nDayBefore));	// 0:
			CString stBefore;
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : 당일 , 1: 전
 			strData += "4341";	strData += sDS;	strData += strTime;	strData += sCELL;
 		}
//END
		break;

	case 2:		// 일
			Datah.dindex = GI_DAY;
			 nUnit = 1;
			stRowData = _T("?5500");
			stNextKey = "5324";
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
			//KHD : 수정주가
			strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
			strData += "5339";	strData += sDS;	strData += strTemp;	strData += sCELL;
			
		break;
	case 3:		// 주
			Datah.dindex = GI_WEEK;
			 nUnit = 1;
			stRowData = _T("?5500");
			stNextKey = "5324";
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
			//KHD : 수정주가
			strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
			strData += "5339";	strData += sDS;	strData += strTemp;	strData += sCELL;
			
		break;
	case 4:		// 월
			Datah.dindex = GI_MONTH;
			 nUnit = 1;
			stRowData = _T("?5500");
			stNextKey = "5324";
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
			//KHD : 수정주가
			strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
			strData += "5339";	strData += sDS;	strData += strTemp;	strData += sCELL;

		break;

	case 5:		// 년
			Datah.dindex = GI_YEAR;
			 nUnit = 1;
			stRowData = _T("?5500");
			stNextKey = "5324";
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
			//KHD : 수정주가
			strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
			strData += "5339";	strData += sDS;	strData += strTemp;	strData += sCELL;

		break;

	case 6:		// 초
		Datah.dindex = GI_SEC;
		Datah.dunit = GI_SEC;
		stRowData = _T("?6500");
	
		stNextKey = "6324";
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL;	// 넥스트 키값 : KHD 
 		strData += "6308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;		// 실/허봉 구분 (0:허봉, 1:실봉)
 		strData += "6309";	strData += sDS;	strData += strNow;	strData += sCELL;			// 당일만조회 Input 심볼 (1:당일)

		break;
	}

	//////////////////////////////////////////////////////////////////////////
	//	OutBound Data
	for (i=0; i<m_nGridCnt; i++)
	{
		strData += szStockGridSymbol[i];
		strData += sCELL;//KHD sFS;
	}
	strData += stNextKey;
	strData += sCELL;
//환율 데이터 
	strData += "51331";
	strData += sCELL;

	// Row Data 구성
	strData += stRowData;
	strData += sDS;

	CString temp;
	//조회데이터 수 
	int GrpHLen = 0;
	temp.Format("%06d", nCount);
	CopyMemory(Datah.count, temp, temp.GetLength());
	Datah.dkey = 0 + 0x30;
	
	CopyMemory(Datah.pday, strDate, strDate.GetLength());

	temp.Format("%0*d", sizeof(Datah.lgap), nUnit);// N분봉 
	CopyMemory(Datah.lgap, temp, temp.GetLength() );
	
	char szData_i[sz_DATAH];
	memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
 	CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
	szData_i[sz_DATAH-1] = 0x00; // KHD : 맨끝에 0x00문자를 집어 넣는다.
	strData += szData_i;

	switch (nType)
	{
	case 0:		// 틱
		for (i = 0; i < m_nTickCnt; i++)
		{
			strData += szTickRowField[i];
			strData += sCOL;
		}
		break;

	case 1:		// 분
		for(i = 0; i < m_nMinCnt; i++)
		{
			strData += szMinRowField[i];
			strData += sCOL;
		}
		break;

	case 2:		// 일
		for(i = 0; i < m_nDayCnt; i++)
		{
			strData += szDayRowField[i];
			strData += sCOL;
		}
		break;
	case 3:		// 주
	case 4:		// 월
		for(i = 0; i < m_nWeekCnt; i++)
		{
			strData += szWeekRowField[i];
			strData += sCOL;
		}
		break;
	case 6:		// 초
		for(i = 0; i < m_nSecCnt; i++)
		{
			strData += szSecRowField[i];
			strData += sCOL;
		}
		break;
	case 5:		// 년
		for(i = 0; i < m_nYearCnt; i++)
		{
			strData += szYearRowField[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}

char* CChartItemStock::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0602_OutRec1_EX stOut;
	CString strSrc = (char*)pData;
	CString strNode, strCode, strLock;
	CString strPrice, strPrePrice, strVolume, strPreVolume;
	CString strHigh, strLow, strOpen, strHighLimit, strLowLimit, strWonDollar;
	CString strPreHigh, strPreLow, strPreOpen;
	CString strBaseTime, strMarketTime;
	int nPrice;
	double dChRate=0;

	memset(&stOut, 0x20, sizeof(stOut));

	strCode = CDataConverter::Parser(strSrc, sCELL); // 종목코드

	strNode = CDataConverter::Parser(strSrc, sCELL); // 종목명

	//100120-alzioyes. 오늘부로 서버와 얘기된 내용.
	//strNode = strNode.Mid(1);
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sCELL); // 현재가
	nPrice = atoi(strPrice);
	strPrice.Remove(_T('+')); strPrice.Remove(_T('-'));
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));

	
	strPrePrice = CDataConverter::Parser(strSrc, sCELL); // 전일종가
	memcpy(stOut.preprice, strPrePrice, min(sizeof(stOut.preprice), strPrePrice.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sCELL); // 전일증감
	strNode = strNode.Mid(1);
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL); // 등락율
	dChRate = atof(strNode);
	strNode.Remove(_T('+'));/*strNode.Remove(_T('-'));*/
	strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // 거래량
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	strPreVolume = CDataConverter::Parser(strSrc, sCELL); // 전일거래량

	// Make sign value
	int nHigh, nLow;
	strHighLimit = CDataConverter::Parser(strSrc, sCELL); // 상한가
	nHigh = atoi(strHighLimit);

	strLowLimit = CDataConverter::Parser(strSrc, sCELL); // 하한가
	nLow = atoi(strLowLimit);

	theApp.DoMapStrObj_UpDnInfo(strCode, strHighLimit, strLowLimit);

	if(abs(nPrice) == nHigh)		stOut.sign[0] = '1'; // UP_LIMIT
	else if(abs(nPrice) == nLow)	stOut.sign[0] = '4'; // DOWN_LIMIT
	else if(dChRate < 0)			stOut.sign[0] = '5'; // DOWN
	else if(dChRate == 0)			stOut.sign[0] = '3'; // SAME (BOHAP)
	else							stOut.sign[0] = '2'; // UP


	strOpen = CDataConverter::Parser(strSrc, sCELL); // 시가
	strOpen.Remove(_T('+')); strOpen.Remove(_T('-'));
	strHigh = CDataConverter::Parser(strSrc, sCELL); // 고가
	strHigh.Remove(_T('+')); strHigh.Remove(_T('-'));
	strLow = CDataConverter::Parser(strSrc, sCELL); // 저가
	strLow.Remove(_T('+')); strLow.Remove(_T('-'));

	strPreOpen = CDataConverter::Parser(strSrc, sCELL);		// 전일시가
	strPreOpen.Remove(_T('+')); strPreOpen.Remove(_T('-'));
	strPreHigh = CDataConverter::Parser(strSrc, sCELL);		// 전일고가
	strPreHigh.Remove(_T('+')); strPreHigh.Remove(_T('-'));
	strPreLow = CDataConverter::Parser(strSrc, sCELL);		// 전일저가
	strPreLow.Remove(_T('+')); strPreLow.Remove(_T('-'));

	CString strRealState = CDataConverter::Parser(strSrc, sCELL);	
	/* 0:장중 */
	/* 2:장마감 */
	/* 3:장마감 */
	/* 4:거래정지 */
	/* 5:거래재개 */
	/* 6:장종료후시간외종가매매개시 */
	/* 7:장종료후시간외종가매매및 */
	/* 시간외단일가매매개시 */
	/* 8:CB발동 9:CB발동해제 */
	/* 10:장개시전 시간외매매개시 */
	/* 11:장개시전 시간외매매종료 */
	/* 12:시가범위연장 */
	/* 13:시가임의종료 */
	/* 14:종가범위연장 */
	/* 15:종가임의종료 */
	/* 16:시간외단일가RANDOMEND개시 */
	/* 17:시간외단일가RANDOMEND종료 */
	/* 18:시간외단일가매매마감 */
	/* 19:임시정지 20:임시정지후재개*/
	/* 21:시간외단일가 개시 */
	/* 22:예상체결가산출 개시 */

//	CString strRequestTime = CDataConverter::Parser(strSrc, sCELL);	// "1390" 현재장시간.
//	CString strMarketStartTime = CDataConverter::Parser(strSrc, sFS); // "1389" 예상장시작시간(HHMM)
//	CString strMarketStartTime = CDataConverter::Parser(strSrc, sFS); // "1600" 장상태 코드(정규장/시간외단일가등..)
// 	if(strMarketStartTime.GetLength() == 4) // "0900" -> "090000"
// 		strMarketStartTime += "00";

	//장운영시간을 체크해서 장이 시작했으면 보조메시지에서 장시간을 셋팅하지 않도록 한다.
	CString strJangStart = CDataConverter::Parser(strSrc, sCELL); //"1391" 장운영시간시작(HHMM)
	CString strJangEnd   = CDataConverter::Parser(strSrc, sCELL); //"1392" 장운영시간종료(HHMM)
	strJangStart.TrimRight();
	strJangEnd.TrimRight();

	BOOL bSetReal = FALSE;
	if(strRealState.Compare("10") == 0 || strRealState.Compare("11") == 0 || strRealState.Compare("22") == 0)
		bSetReal = TRUE;

	if(strJangStart.GetLength() == 4)	strJangStart += "00";

	//>>0001067: [차트] 장시작 시각이 9시가 아닌 경우(ex : 수능) N분봉이 변경된 시간기준으로 생성되고 있지않은 현상
	//수능의 경우 9시가 아닌 경우에 시작하므로 장시작정보가 바뀌어야 함.
	//BASETIME(HHMMSS)과 MARKETTIME(HHMMSS|HHMMSS) 둘다 셋팅되어야 함.
	//if(strJangStart.Compare("090000") != 0)
	CString strResetTime = "";
	if(bSetReal)
	{
		strResetTime = strJangStart;
	}
	strBaseTime = strJangStart;
	strMarketTime.Format("%4.4s00|%4.4s00", strJangStart, strJangEnd);
	//<<

	//if(strJangStart.Compare("0000")!=0)		strMarketStartTime.Empty();

	//[A00000651][A00000652]누적거래대금.
	CString strVolAmt = CDataConverter::Parser(strSrc, sCELL); //"2028" 누적거래대금
	strVolAmt.TrimRight();
	strVolAmt += _T("000000");

	//KHD : NextKey
	CString strNextKey = CDataConverter::Parser(strSrc, sCELL);
	//KHD : 
	CString strUSDollor = CDataConverter::Parser(strSrc, sCELL);
	memcpy(m_InData.usRate, (LPTSTR)(LPCTSTR)strUSDollor, min(strUSDollor.GetLength(), sizeof(m_InData.usRate)));
	
	int nSize, nCnt, nType;
	strNode = CString(m_InData.button, sizeof(m_InData.button)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	nType = atoi(strNode);
	if (nType == 0)			nSize = m_nTickCnt;	// 틱
	else if (nType == 1)	nSize = m_nMinCnt;	// 분
	else if (nType == 6)	nSize = m_nSecCnt;	// 초
	else if (nType == 2)	nSize = m_nDayCnt;	// 일
	else if (nType == 5)	nSize = m_nYearCnt;	// 년
	else					nSize = m_nWeekCnt;	// 주, 월

//	종목명 정보구하기.	
// 	CString szTTT;
//	szTTT = CDataConverter::Parser(strSrc, sFS); // 17408 종목명 정보구하기.

	// 그리드 헤더
	//GRAPH_IO gridOut;
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));
	
	if (strNextKey =="99999999999999" || atoi(strNextKey) == 0)// 9가 14자리면 다음키가 없는것.  
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));
	//END

	CStringArray arrNode;
	int nNodeIndex = 1;
	strSrc.TrimLeft(); strSrc.TrimRight();
	strSrc+=sCELL;
	strSrc+=sFS;
	strSrc+=sROW;
	strSrc+=sCELL;
	while(strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);
	
		arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// 전체 데이터 수 계산
	nCnt = arrNode.GetSize()/ nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// 차트 봉 데이터
	int nBongSize = sizeof(KB_p0602_OutRec2);
	int nBufSize = sizeof(KB_p0602_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0602_OutRec1_EX);

	nDataLen = nBufSize;

	CString strDate, strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_p0602_OutRec2 stFirstBong, stBong;

	CStringArray arrTest;
	double dValue=0;
	int nIndex = 0;
	int nEmptyIndex = 0;
	int nRow = 0;
	for ( nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);
		int Price = atoi(strDate);
		
		// time : 틱,초,분일 때
		if (nType < 2 || nType == 6) 
			strTime = arrNode.GetAt(nIndex++); 
		
		if (nType == 0) // DDHHMMSS
			strNode = strDate.Right(2) + strTime;
		else if (nType == 1) //MMDDHHMM
			strNode = strDate.Right(4) + strTime.Left(4);
		else if (nType == 4) //YYYYMM
			strNode = strDate.Left(6);
		else if (nType == 6) //DDHHMMSS
			strNode = strDate.Right(2) + strTime.Left(6);
		else if (nType == 5) // 년.
		{
			if (nRow != nCnt-1)
				strNode.Format(_T("%s12"), strDate.Left(4));
			else
				strNode = strDate.Left(6);
		}
		else
			strNode = strDate;

		memcpy(stBong.date, strNode, min(sizeof(stBong.date), strNode.GetLength()));
	
		// 종가/체결가
		strNode = arrNode.GetAt(nIndex++);
	
		memcpy(stBong.close, strNode, min(sizeof(stBong.close), strNode.GetLength()));
		// 시가
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.open, strNode, min(sizeof(stBong.open), strNode.GetLength()));
		// 고가
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.high, strNode, min(sizeof(stBong.high), strNode.GetLength()));
		// 저가
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.low, strNode, min(sizeof(stBong.low), strNode.GetLength()));
		// 거래량
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.volume, strNode, min(sizeof(stBong.volume), strNode.GetLength()));
		// 거래대금
		//@Solomon	091202SM123	-->
		strNode = arrNode.GetAt(nIndex++);
		//dValue = atof(strNode)*1000000;
		//strNode.Format(_T("%.0f"), dValue);
		if(nType < 2 || nType == 6)	//틱초분.
		{			
			//	strNode += "000000";//KHD
		}
		else
		{
			// 20121024 이상신	마지막 데이터에 sCELL 값이 붙어 있는 경우 .. sCELL 이전 값으로 다시 셋팅. --->>>>
			if (nRow == nCnt - 1)
			{
				int nFind = strNode.Find(sCELL);
				if (nFind > 0)
					strNode = strNode.Left(nFind);
			}
			//	---------<<<<<<

			strNode += "000000";
		}

		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--

		// 권리락
		if (nType == 2)
		{
			strNode = arrNode.GetAt(nIndex++);
			memcpy(stBong.rights, strNode, min(sizeof(stBong.rights), strNode.GetLength()));

			strNode = arrNode.GetAt(nIndex++);
			memcpy(stBong.sign, strNode, min(sizeof(stBong.sign), strNode.GetLength()));
		}

		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);

		// 첫 데이터를 저장해 놓는다.
	    if (nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	// 20130222 이상신 : 주봉일경우 마지막 데이터의 일자를 설정해 준다.(주 첫 영업일 관련)
	if (nCnt > 0 && nType == 3 && m_InData.nflag[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = strDate;
	else
		m_bojoInput.m_sLastDataTime = _T("");

//	nCnt-=nEmptyIndex;
	double dUSDollar=0;
	// 데이터 가공 : 타사이트는 서버쪽에서 처리
	int nDataType = int(m_InData.nDataGubun[0] - '0'); 
	if(nDataType > 0)
	{
		CString strData(stFirstBong.close, sizeof(stFirstBong.close));
		int nFirstClose = atoi(strData);
		double dDenom; //분모
		KB_p0602_OutRec2 stBong;
		//@Solomon	091123SM072
		double dData;
		double dUSDollar = CDataConverter::Char2Double(m_InData.usRate, sizeof(m_InData.usRate));

		switch(nDataType)
		{
		case 1:	dDenom = nFirstClose;		break;	// 첫종가 기준
		case 3:	dDenom = atof(strOpen);		break;	// 당일시가
		case 4:	dDenom = nPrice;			break;	// 현재가
		case 5:	dDenom = atof(strPrePrice);	break;	// 전일종가
		case 2:
			dDenom = dUSDollar;

			dData = atof(strPrePrice)*100 / dUSDollar;
			strPrePrice.Format(_T("%ld"), (long)dData);
			dData = atof(strLowLimit)*100 / dUSDollar;
			strLowLimit.Format(_T("%ld"), (long)dData);

			dData = atof(strHighLimit)*100 / dUSDollar;
			strHighLimit.Format(_T("%ld"), (long)dData);
 ////KHD : 상단바 깨지는 현상 수정			
 		dData = atof(strOpen)*100 ;// dUSDollar;
 			strOpen.Format(_T("%ld"), (long)dData);
 			dData = atof(strHigh)*100 ;// dUSDollar;
 			strHigh.Format(_T("%ld"), (long)dData);
 			dData = atof(strLow)*100 ;// dUSDollar;
 			strLow.Format(_T("%ld"), (long)dData);
//END 
			dData = atof(strPrice)*100 / dUSDollar;
			strPrice.Format(_T("%ld"), (long)dData);

			strWonDollar.Format(_T("%ld"), (long)dUSDollar);
			break;
		}

//		if (dDenom == 0)
//			dDenom = atoi(strOpen);

		if (nDataType == 2)
		{
			for (nRow = 0; nRow < nCnt; nRow++)
			{
				memcpy(&stBong, &pDataBuf[nPosBong + nRow*nBongSize], nBongSize);
				// 시가
				strData = CDataConverter::Get_RateData(stBong.open, dDenom, sizeof(stBong.open));
				memcpy(stBong.open, strData, min(strData.GetLength(), sizeof(stBong.open)));
				// 고가
				strData = CDataConverter::Get_RateData(stBong.high, dDenom, sizeof(stBong.high));
				memcpy(stBong.high, strData, min(strData.GetLength(), sizeof(stBong.high)));
				// 저가
				strData = CDataConverter::Get_RateData(stBong.low, dDenom, sizeof(stBong.low));
				memcpy(stBong.low, strData, min(strData.GetLength(), sizeof(stBong.low)));
				// 종가
				strData = CDataConverter::Get_RateData(stBong.close, dDenom, sizeof(stBong.close));
				memcpy(stBong.close, strData, min(strData.GetLength(), sizeof(stBong.close)));
				
				memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
			}
		}
		else
		{
			for (nRow = 0; nRow < nCnt; nRow++)
			{
				memcpy(&stBong, &pDataBuf[nPosBong + nRow*nBongSize], nBongSize);
				// 시가
				strData = CDataConverter::Get_PercentData(stBong.open, dDenom, sizeof(stBong.open));
				memcpy(stBong.open, strData, min(strData.GetLength(), sizeof(stBong.open)));
				// 고가
				strData = CDataConverter::Get_PercentData(stBong.high, dDenom, sizeof(stBong.high));
				memcpy(stBong.high, strData, min(strData.GetLength(), sizeof(stBong.high)));
				// 저가
				strData = CDataConverter::Get_PercentData(stBong.low, dDenom, sizeof(stBong.low));
				memcpy(stBong.low, strData, min(strData.GetLength(), sizeof(stBong.low)));
				// 종가
				strData = CDataConverter::Get_PercentData(stBong.close, dDenom, sizeof(stBong.close));
				memcpy(stBong.close, strData, min(strData.GetLength(), sizeof(stBong.close)));
				
				memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 차트보조 메시지 만들기
	CChartBozoManager bojoMng;
	m_bojoInput.m_sShcode = strCode;
	m_bojoInput.m_sPrice = strPrice;		// 현재가
	m_bojoInput.m_sVolume = strVolume;		// 거래량
	m_bojoInput.m_sUpLmtPrice = strHighLimit;	// 상한가
	m_bojoInput.m_sDnLmtPrice = strLowLimit;	// 하한가
	m_bojoInput.m_sOpen = strOpen;		// 시가
	m_bojoInput.m_sHigh = strHigh;		// 고가
	m_bojoInput.m_sLow = strLow;			// 저가
//@Solomon:091123SM068 -->
	m_bojoInput.m_sPreOpen = strPreOpen;		// 전일시가
	m_bojoInput.m_sPreHigh = strPreHigh;		// 전일고가
	m_bojoInput.m_sPreLow = strPreLow;		// 전일저가
	m_bojoInput.m_sWonDollor = strWonDollar;	// 원달러 환률
//@Solomon:091123SM068 <--
	m_bojoInput.m_sPreprice = strPrePrice;	// 전일종가
	m_bojoInput.m_sPreVolume = strPreVolume;	// 전일거래량
	m_bojoInput.m_sStartTime = strResetTime;//"090000";	// 거래시작시간(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close));	// 조회중 첫봉의 종가.

	m_bojoInput.m_sDataType.Format(_T("%c"), m_InData.nDataGubun[0]);
	m_bojoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.
	m_bojoInput.m_sNxtFlg = m_InData.nflag[0];

	m_bojoInput.m_sTerm.Format("%d", nType);	//	일,주,월
	m_bojoInput.m_sTick = CString(m_InData.unit, sizeof(m_InData.unit));
	if (nType == 1)
	{
		//{{JS.Kim_20100903 Solomon 60분봉 이상인 경우 시간으로 단위 변경하여야 함
		int nTick = atoi(m_bojoInput.m_sTick);
		if( nTick >= 60 )
			m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
		//}}
	}
	else if (nType >= 2 && nType <= 5)		//	일,주,월,년
		m_bojoInput.m_sTick = _T("1");

	m_bojoInput.m_sVolAmt = strVolAmt;	//[A00000651][A00000652]누적거래대금.

	m_bojoInput.m_szMarketTime	= strMarketTime;	//장운영정보.
	m_bojoInput.m_szBaseTime	= strBaseTime;		//분봉일 때 봉의 시작기준 시간.
	m_bojoInput.m_strHogaTick	= CDataConverter::GetHogaTick(strCode, strPrice); //호가단위

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 101); // 101은 주식차트
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0496", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}
