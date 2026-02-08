// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartItemData.h"
#include "ChartItem.h"
#include "./define/ChartDef.h"
#include "./define/ChartMsgDef.h"
#include "./include/arMarketIndicator.h"
#include "StdDialog.h"

//	추가... 솔라리스 환경에서 돌려서 보내야 한다. 고 병훈이가 써놓음
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")		// htonl, ntohl

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CChartItemData::CChartItemData(CChartItem *pChartItem, long lTrCode)
CChartItemData::CChartItemData(CChartItem *pChartItem, CString strTRCode)
{
	Init(pChartItem,strTRCode);
}

CChartItemData::~CChartItemData()
{
	ClearChartItemData();
}

void CChartItemData::Init(CChartItem *pChartItem, CString strTRCode)
{
	memset(m_chkey,'0',sizeof(m_chkey));

//@유진삭제	m_pSt33100Data = NULL;
//@유진삭제	m_pSt35100Data = NULL;
//@유진삭제	m_pSt36100Data = NULL;
	m_pEu0602_ChartData = NULL;
	m_pEu0603_ChartData = NULL;
	m_pEu0604_ChartData = NULL;
	m_pEu0605_ChartData = NULL;
	m_pEu0606_ChartData = NULL;
	m_pEu0607_ChartData = NULL;

//@유진삭제	m_pSt10450Data = NULL;
//@유진삭제	m_pSt10451Data = NULL;
//@유진삭제	m_pSt10452Data = NULL;
	m_pSt03040Data = NULL;
	m_pSt01007Data = NULL;

	m_lDataSize = 0;
	m_lCnt = 0;
	m_bInitalChart = FALSE;
	m_pChartItem = pChartItem;
	//m_lTrCode = lTrCode;	
	m_strTRCode = strTRCode;
	m_bIsFilled = FALSE;
	m_lStartDate = NULL; 
	m_lEndDate = NULL;
	m_lCntRequiredByPrim = 0;
	m_lCntTemp = 0;

	m_nCntOverLap = 0;
	m_bOverLapping = FALSE;

	m_bNextMarketData = FALSE;
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070529)
	m_bResetChartCfgWith = FALSE;

// --> [Edit]  강지원 2008/07/30
	m_strOpen		= "";			//	시가
	m_strHigh		= "";			//	고가
	m_strLow		= "";			//	저가
	m_strPrice		= "";			//	현재가
	m_strSign		= "";			//	부호
	m_strChange		= "";			//	대비
	m_strChrate		= "";			//	대비율
	m_strVolume		= "";			//	거래량
	m_strPreprice	= "";			//	기준가
	m_strKey		= "";			//	이전키
	m_strPosition	= "× 1";
// <-- [Edit]  강지원 2008/07/30
}

// 선택된 차트의 시장 세팅 : 복수종목 - ojtaso (20070215)
void CChartItemData::SetTRCode(LPCTSTR lpszTRCode)
{
	if(m_strTRCode.Compare(lpszTRCode))
		ChangePacketNameArray(lpszTRCode);

	m_strTRCode = lpszTRCode;
}


BOOL CChartItemData::ClearChartItemData()
{	
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::ClearChartItemData::Begin", _T("멀티차트 종료"), 13);
#endif
	RemoveRealItem();
	RemovePacketAndGraph();
	RemoveData();
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::ClearChartItemData::End", _T("멀티차트 종료"), 13);
#endif
	return TRUE;
}

BOOL CChartItemData::RemoveData()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveData::Begin", _T("멀티차트 종료"), 13);
#endif
	if(m_pEu0602_ChartData)
	{
		char *pData = (char*)m_pEu0602_ChartData;
		//delete [] m_pEu0602_ChartData;
		delete [] pData;
		m_pEu0602_ChartData = NULL;
	}
	if(m_pEu0603_ChartData)
	{
		char *pData = (char*)m_pEu0603_ChartData;
		//delete  m_pEu0603_ChartData;
		delete [] pData;
		m_pEu0603_ChartData = NULL;
	}	

	if(m_pEu0604_ChartData)
	{
		char *pData = (char*)m_pEu0604_ChartData;
		//delete  m_pEu0604_ChartData;
		delete [] pData;
		m_pEu0604_ChartData = NULL;
	}

	if(m_pEu0605_ChartData)
	{
		char *pData = (char*)m_pEu0605_ChartData;
		//delete  m_pEu0605_ChartData;
		delete [] pData;
		m_pEu0605_ChartData = NULL;
	}

	if(m_pEu0606_ChartData)
	{
		char *pData = (char*)m_pEu0606_ChartData;
		//delete  m_pEu0606_ChartData;
		delete [] pData;
		m_pEu0606_ChartData = NULL;
	}

	if(m_pEu0607_ChartData)
	{
		char *pData = (char*)m_pEu0607_ChartData;
		//delete  m_pEu0607_ChartData;
		delete [] pData;
		m_pEu0607_ChartData = NULL;
	}

//@유진삭제
//	if(m_pSt10450Data)
//	{
//		delete  m_pSt10450Data;
//		m_pSt10450Data = NULL;
//	}
//
//	if(m_pSt10451Data)
//	{
//		delete  m_pSt10451Data;
//		m_pSt10451Data = NULL;
//	}
//@유진삭제

	if(m_pSt03040Data)
	{
		delete m_pSt03040Data;
		m_pSt03040Data = NULL;
	}
	if(m_pSt01007Data)
	{
		delete m_pSt01007Data;
		m_pSt01007Data = NULL;
	}

	m_lDataSize = 0;

#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveData::End", _T("멀티차트 종료"), 13);
#endif
	return TRUE;
}

void CChartItemData::SetCodeForQuery(LPCTSTR lpCode, BOOL bInitKey/* = TRUE*/)
{
	ASSERT(lpCode);
	m_strCodeForRequest = lpCode;
	//if(m_strCodeForRequest.GetLength() == CODE_STOCK_LENGTH2)
	//	m_strCodeForRequest.Format("%s ", lpCode);
	if(bInitKey)
		memset(m_chkey,'0',sizeof(m_chkey));
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
stSend2Server* CChartItemData::GetQueryData(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	m_lDataSize = 0;
	m_lCnt = 0;
	CString strTRCode = lpSelChart ? lpSelChart->m_strChartGubun : m_strTRCode;

	if (strTRCode.IsEmpty())
		strTRCode = m_strTRCode;

	if (strTRCode == QUERY_STRFOFOREIGN)
		return GetQueryData_EU_p0602(bNext, lpSelChart);
//	else if (strTRCode == QUERY_STRFOFOREIGN_HOGA)
//		return GetQueryData_EU_p0603(bNext, lpSelChart);

	return NULL;
}

// --> [Edit]  강지원 2008/07/22		( 시간구분 )
#define		UD_DATAKIND_TICK	"1"
#define		UD_DATAKIND_MIN		"2"
#define		UD_DATAKIND_DAY		"3"
#define		UD_DATAKIND_WEEK	"4"
#define		UD_DATAKIND_MONTH	"5"
#define		UD_DATAKIND_HOUR	"6"

#include "../../inc/IKSLibMng.h"
// <-- [Edit]  강지원 2008/07/22

stSend2Server* CChartItemData::GetQueryData_EU_p0602(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	int nID = 50;

	IGateManager* pGateMng;
	AfxGetIGateManager(pGateMng);
	if(pGateMng == NULL)
		return NULL;

	CString strKeyValue = "";
	CString strJMCode = "";
	CString strDate = "";
	CString strTime = "";
	CString strFirstGu = "";
	CString strTimeGu = "";
	CString strSelectMin = "";
	CString strDataCnt = "";
	CString strQtyCd = "1";

	strKeyValue		= "";				/* key 일자+시간(18)	: key 일자+시간 */
	strJMCode		= "";				/* 종목코드(10)			: 종목코드 */
	strDate			= "99999999";		/* 일자(8)				: 일자 */
	strTime			= "9999999999";		/* 시간(10)				: 시간 */
	strFirstGu		= "0";				/* 구분(1)				: 0:처음1:다음 */
	strTimeGu		= UD_DATAKIND_DAY;	/* 데이터구분(1)		: 1:Tick,2:분,3:일,4:주,5:월 */
	strSelectMin	= "1";				/* N분구분(2)			: 0:30sec,1:1M,2:2M,... */
	strDataCnt		= "400";				/* 데이터건수(5)		: 데이터건수 */


	HCQ03010_IN*	input = new HCQ03010_IN;
	memset(input , 0x20 , HCQ03010_IN_SZ);

	strKeyValue = lpSelChart ? lpSelChart->m_strKey : m_pChartItem->m_strNextKey;
	if ( bNext )
		memcpy(input->keyvalue,	(LPCTSTR)strKeyValue,	strKeyValue.GetLength());	/* key 일자+시간	: key 일자+시간 */

	//	종목코드
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	if(lpSelChart)	strJMCode = CStdDialog::GetRealItemCode( lpSelChart->m_strItemCode);
	else			strJMCode = CStdDialog::GetRealItemCode( m_strCodeForRequest);

	memcpy(input->jmcode,	(LPCTSTR)strJMCode,		strJMCode.GetLength());	/* 종목코드			: 종목코드 */

	// 조회모드
	if(bNext)
		strFirstGu = "1";
	else
		strFirstGu = "0";
	memcpy(input->gubun,		(LPCTSTR)strFirstGu,	strFirstGu.GetLength());	/* 구분				: 0:처음1:다음 */

	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	strTimeGu.Format("%c", chTypeRequested);
	memcpy(input->dgubun,	(LPCTSTR)strTimeGu,		strTimeGu.GetLength());	/* 데이터구분		: 1:Tick,2:분,3:일,:주,5:월 */

	m_pChartItem->SetTypeExInputBarDlg(chTypeRequested);

	if ( chTypeRequested == TICK_DATA_CHART )		//	틱차트일경우 일자를 초기화해서 준다... 예전일자가 들어갈경우 틱차트 조회가 안되기때문.
	{
		memcpy(input->date,		(LPCTSTR)strDate,		strDate.GetLength());		/* 일자				: 일자 */
		memcpy(input->chetime,	(LPCTSTR)strTime,		strTime.GetLength());		/* 시간				: 시간 */
	}
	else
	{
		memcpy(input->date,		(LPCTSTR)m_pChartItem->GetCtlTime(), sizeof(input->date) + sizeof(input->chetime));		/* 일자				: 일자 */
		//	주기가 분일경우 설정된 시간을 서버시간(시카고)로 변경해서 조회로 올려준다. 
		if ( chTypeRequested == MIN_DATA_CHART )		//	틱차트일경우 일자를 초기화해서 준다... 예전일자가 들어갈경우 틱차트 조회가 안되기때문.
		{
			CString strTemp = m_pChartItem->GetCtlTime();
			int		nTimeIndex = m_pChartItem->GetExInputBarDlg();

			strDate = strTemp.Left(8);
			strTemp = strTemp.Mid(8);
			strTemp.TrimLeft();		strTemp.TrimRight();

			if ( lpSelChart && strTemp != "9999999999" )
			{
				int nGab(0), nServerGab(0), nSummerTime(0);
				nServerGab = atoi(((CStdDialog*)m_pChartItem->m_pStdDialog)->GetTimeIndexInfo(nTimeIndex/*lpSelChart->m_nTimeIndex*/, 1));
				nGab = atoi(((CStdDialog*)m_pChartItem->m_pStdDialog)->GetTimeIndexInfo(lpSelChart->m_nOrgTimeIndex, 1));

				strTemp = m_pChartItem->GetCalcTimeGab(strDate, strTemp, ( nGab - nServerGab ));
				strTemp.Remove(':');

				memcpy(input->date,		(LPCTSTR)strDate,		strDate.GetLength());		/* 일자				: 일자 */
				memcpy(input->chetime,	(LPCTSTR)strTemp,		strTemp.GetLength());		/* 시간				: 시간 */
			}
		}
	}

	//	분 구분
	strSelectMin = lpSelChart ? lpSelChart->m_strTickMinDayNumber : m_pChartItem->GetTickMinDayNumberRequested();
	int nSelMin = atoi(strSelectMin);
	if(lpSelChart)
		m_pChartItem->SetTickMinDayNumber(nSelMin);

	if ( strSelectMin == SEC30_STRING )
		strSelectMin = "0";
	//{{JS.Kim_20100903 Solomon 99분 이상 데이타 처리
	//if (nSelMin < 100)
	//	strSelectMin.Format(_T("%d"), nSelMin);
	//else
	//	strSelectMin = "1";
	strSelectMin.Format(_T("%d"), nSelMin);
	//}}

	memcpy(input->nsel,		(LPCTSTR)strSelectMin,	strSelectMin.GetLength());	/* N분구분			: 0:30sec,1:1M,2:2M,... */

	// 조회건수
	int nQueryCnt = GetCntForQuery(bNext);
	if ( nQueryCnt == 0 )	nQueryCnt = DEFAULTCNTATONCE;
	strDataCnt.Format("%04d", nQueryCnt);
	memcpy(input->dcnt,		(LPCTSTR)strDataCnt,	strDataCnt.GetLength());	/* 데이터건수		: 데이터건수 */

	//{{JS.Kim_20100906 데이타추가부르기
	// 조회모드
	char chFlag;
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(input->nflag,&chFlag,sizeof(input->nflag));

	// 검색키
	memset(input->nkey,0x20,sizeof(input->nkey));
	if(chFlag == '1')//다음 조회일 경우
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(input->nkey, strKey, min(sizeof(input->nkey), strKey.GetLength()));
	}
	//}}

	if ( m_pChartItem->GetBNormalizedMinuteTimeRule() )			//	TRUE : 허수봉 포함, FALSE : 실봉만
		strQtyCd = "0";
	else
		strQtyCd = "1";

	//	이 구조체의 메모리 해제는 통신을 보낸다음에 한다.
	stSend2Server* st = new stSend2Server;
	st->m_pBuffer = (void*)input;
	st->m_lLength= sizeof(HCQ03010_IN);

	return st;
}
/*
// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
stSend2Server* CChartItemData::GetQueryData_EU_p0603(BOOL bNext, ST_SELCHART* lpSelChart)
{
	int nID = 62;

	IGateManager* pGateMng;
	AfxGetIGateManager(pGateMng);
	if (pGateMng == NULL)
		return NULL;

	CString strKeyValue = "";
	CString strJMCode = "";
	CString strDate = "";
	CString strTime = "";
	CString strDataCnt = "";

	strKeyValue		= "";				// key 일자+시간(18)	: key 일자+시간
	strJMCode		= "";				// 종목코드(10)			: 종목코드
	strDate			= "99999999";		// 일자(8)				: 일자
	strTime			= "9999999999";		// 시간(10)				: 시간
	strDataCnt		= "400";			// 데이터건수(5)		: 데이터건수


	HCQ03011_IN*	input = new HCQ03011_IN;
	memset(input , 0x20 , HCQ03011_IN_SZ);

	strKeyValue = lpSelChart ? lpSelChart->m_strKey : m_pChartItem->m_strNextKey;
	if ( bNext )
		memcpy(input->keyvalue,	(LPCTSTR)m_pChartItem->m_strNextKey,	m_pChartItem->m_strNextKey.GetLength());	// key 일자+시간	: key 일자+시간

	//	종목코드
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	if(lpSelChart)	strJMCode = CStdDialog::GetRealItemCode( lpSelChart->m_strItemCode);
	else			strJMCode = CStdDialog::GetRealItemCode( m_strCodeForRequest);

	memcpy(input->jmcode,	(LPCTSTR)strJMCode,		strJMCode.GetLength());						// 종목코드			: 종목코드
	memcpy(input->date,		(LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(input->date));	// 일자				: 일자
	memcpy(input->chetime,	(LPCTSTR)strTime,		strTime.GetLength());						// 시간				: 시간

	// 조회건수
	memcpy(input->dcnt,		(LPCTSTR)strDataCnt,	strDataCnt.GetLength());					// 데이터건수		: 데이터건수
	
	//	이 구조체의 메모리 해제는 통신을 보낸다음에 한다.
	stSend2Server* st = new stSend2Server;
	st->m_pBuffer = (void*)input;
	st->m_lLength= sizeof(HCQ03011_IN);

	return st;
}

CString CChartItemData::GetQueryData_EU_p0604(BOOL bNext, ST_SELCHART* lpSelChart)
{
	EU_p0604_InRec1 eu0604InRec1;

	// 초기화
	memset(&eu0604InRec1, 0x00, sizeof(eu0604InRec1));

	// 종목코드
	memset(eu0604InRec1.Scode, 0x20, sizeof(eu0604InRec1.Scode));
	if(lpSelChart)
		memcpy(eu0604InRec1.Scode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(eu0604InRec1.Scode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());

	// 검색시작일
	memcpy(eu0604InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(eu0604InRec1.Sdate));

	// 검색할데이터갯수
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(eu0604InRec1.Smaxbong, Temp, sizeof(eu0604InRec1.Smaxbong));

	// N봉(틱분시일)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
//	if(chTypeRequested == SEC10_DATA_CHART)
//		memcpy(eu0604InRec1.Sunit,"10s", 3);
//	else if (chTypeRequested == SEC30_DATA_CHART)
//		memcpy(eu0604InRec1.Sunit,"30s", 3);
//	else
	{
		if(lpSelChart)
			memcpy(eu0604InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
		else
			memcpy(eu0604InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	}

	// 구분(틱분일주월)
	memcpy(eu0604InRec1.Sbtngb, &chTypeRequested, sizeof(eu0604InRec1.Sbtngb));

	// 조회구분(다음조회)
	char chFlag;
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(eu0604InRec1.Snextgb, &chFlag, sizeof(eu0604InRec1.Snextgb));

	// 페치키
	if(chFlag == '1')	// 다음 조회일 경우
	{
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		//if(atoi(sKey) > 0)
		if(sKey.GetLength()>0)
			memcpy(eu0604InRec1.Snextkey, m_chkey, sizeof(eu0604InRec1.Snextkey));
		else
			memset(eu0604InRec1.Snextkey, 0x20, sizeof(eu0604InRec1.Snextkey));
	}
	else				// 단순 조회일 경우
		memset(eu0604InRec1.Snextkey, 0x20, sizeof(eu0604InRec1.Snextkey));

	eu0604InRec1.SdataType[0] = '0';	//추가080521

	// 봉구분(허수실봉)
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())
	{
		chUseFackedCandle = '0'; // '0':허수봉 '1':실봉           
	}
	memcpy(eu0604InRec1.Simgrgb, &chUseFackedCandle, sizeof(eu0604InRec1.Simgrgb));

	// 복기구분값
	memcpy(eu0604InRec1.Srecordgb, m_pChartItem->GetReplayFlag(), sizeof(eu0604InRec1.Srecordgb));

	// 복기시작n일전
	memcpy(eu0604InRec1.Sredate, m_pChartItem->GetReplayDayBefore(), sizeof(eu0604InRec1.Sredate));

	// 복기시간(hhmmss)
	memcpy(eu0604InRec1.Stime, m_pChartItem->GetReplayStartTime(), sizeof(eu0604InRec1.Stime));

	// 당일조회
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
	}

	memcpy(eu0604InRec1.nowData, &chNowData, sizeof(eu0604InRec1.nowData));	// 당일조회여부

	CString strRecordInfo("0                                                                               ");
	CString strEU0604Info;
	char* szEU0604Info = strEU0604Info.GetBuffer(sizeof(eu0604InRec1));
	memcpy(szEU0604Info, (char*)&eu0604InRec1, sizeof(eu0604InRec1));
	strEU0604Info.ReleaseBuffer();
	CString strFidInfo("1               000002003004005006009010012013|2               000001002003004005;");
	CString strEU0604InRec1;
	strEU0604InRec1 = strRecordInfo + strEU0604Info + strFidInfo;
	return strEU0604InRec1;
}

CString CChartItemData::GetQueryData_EU_p0605(BOOL bNext, ST_SELCHART* lpSelChart)
{
	EU_p0605_InRec1 eu0605InRec1;

	// 초기화
	memset(&eu0605InRec1, 0x00, sizeof(eu0605InRec1));

	// 종목코드
	memset(eu0605InRec1.Scode, 0x20, sizeof(eu0605InRec1.Scode));
	if(lpSelChart)
		memcpy(eu0605InRec1.Scode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(eu0605InRec1.Scode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());

	// 검색시작일
	memcpy(eu0605InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(eu0605InRec1.Sdate));

	// 검색할데이터갯수
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(eu0605InRec1.Smaxbong, Temp, sizeof(eu0605InRec1.Smaxbong));

	// N봉(틱분시일)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
//	if(chTypeRequested == SEC10_DATA_CHART)
//		memcpy(eu0605InRec1.Sunit,"10s", 3);
//	else if (chTypeRequested == SEC30_DATA_CHART)
//		memcpy(eu0605InRec1.Sunit,"30s", 3);
//	else
	{
		if(lpSelChart)
			memcpy(eu0605InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
		else
			memcpy(eu0605InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	}

	// 구분(틱분일주월)
	memcpy(eu0605InRec1.Sbtngb, &chTypeRequested, sizeof(eu0605InRec1.Sbtngb));

	// 조회구분(다음조회)
	char chFlag;
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(eu0605InRec1.Snextgb, &chFlag, sizeof(eu0605InRec1.Snextgb));

	// 페치키
	if(chFlag == '1')	// 다음 조회일 경우
	{
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		//if(atoi(sKey) > 0)
		if(sKey.GetLength()>0)
			memcpy(eu0605InRec1.Snextkey, m_chkey, sizeof(eu0605InRec1.Snextkey));
		else
			memset(eu0605InRec1.Snextkey, 0x20, sizeof(eu0605InRec1.Snextkey));
	}
	else				// 단순 조회일 경우
		memset(eu0605InRec1.Snextkey, 0x20, sizeof(eu0605InRec1.Snextkey));

	eu0605InRec1.SdataType[0] = '0';	//추가080521

	// 봉구분(허수실봉)
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())
	{
		chUseFackedCandle = '0'; // '0':허수봉 '1':실봉           
	}
	memcpy(eu0605InRec1.Simgrgb, &chUseFackedCandle, sizeof(eu0605InRec1.Simgrgb));

	// 복기구분값
	memcpy(eu0605InRec1.Srecordgb, m_pChartItem->GetReplayFlag(), sizeof(eu0605InRec1.Srecordgb));

	// 복기시작n일전
	memcpy(eu0605InRec1.Sredate, m_pChartItem->GetReplayDayBefore(), sizeof(eu0605InRec1.Sredate));

	// 복기시간(hhmmss)
	memcpy(eu0605InRec1.Stime, m_pChartItem->GetReplayStartTime(), sizeof(eu0605InRec1.Stime));

	// 당일조회
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
	}

	memcpy(eu0605InRec1.nowData, &chNowData, sizeof(eu0605InRec1.nowData));	// 당일조회여부

	CString strRecordInfo("0                                                                               ");
	CString strEU0605Info;
	char* szEU0605Info = strEU0605Info.GetBuffer(sizeof(eu0605InRec1));
	memcpy(szEU0605Info, (char*)&eu0605InRec1, sizeof(eu0605InRec1));
	strEU0605Info.ReleaseBuffer();
	CString strFidInfo("1               000002003004005006009010012013|2               000001002003004005;");
	CString strEU0605InRec1;
	strEU0605InRec1 = strRecordInfo + strEU0605Info + strFidInfo;
	return strEU0605InRec1;
}

// 해외지수
// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
CString CChartItemData::GetQueryData_EU_p0606(BOOL bNext, ST_SELCHART* lpSelChart)
{
	EU_p0606_InRec1 eu0606InRec1;

	// 초기화
	memset(&eu0606InRec1, 0x00, sizeof(eu0606InRec1));

	// 종목코드
	memset(eu0606InRec1.Shcode, 0x20, sizeof(eu0606InRec1.Shcode));
	if(lpSelChart)
		memcpy(eu0606InRec1.Shcode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(eu0606InRec1.Shcode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());

	// 검색시작일
	memcpy(eu0606InRec1.date, m_pChartItem->GetEndDateRequested(), sizeof(eu0606InRec1.date));

	// 조회건수
	char Temp[64];
	sprintf(Temp,"%04d",GetCntForQuery(bNext));
	memcpy(eu0606InRec1.num, Temp, sizeof(eu0606InRec1.num));

	// 조회단위
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	{
		if(lpSelChart)
			memcpy(eu0606InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
		else
			memcpy(eu0606InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	}

	// 조회구분
	if(chTypeRequested == MIN_DATA_CHART || chTypeRequested == SEC_DATA_CHART)
	{
		m_pChartItem->SetChartRequestType(DAY_DATA_CHART);
		chTypeRequested = DAY_DATA_CHART;
	}
	memcpy(eu0606InRec1.button,&chTypeRequested,sizeof(eu0606InRec1.button));

	// 조회모드
	char chFlag;
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(eu0606InRec1.nflag,&chFlag,sizeof(eu0606InRec1.nflag));

	// 검색키
	if(chFlag == '1')//다음 조회일 경우
	{
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		if(sKey.GetLength()>0)
			memcpy(eu0606InRec1.nkey,m_chkey,sizeof(eu0606InRec1.nkey));
		else
			memset(eu0606InRec1.nkey,0x20,sizeof(eu0606InRec1.nkey));
	}
	else //단순 조회 일 경우.
		memset(eu0606InRec1.nkey,0x20,sizeof(eu0606InRec1.nkey));

	// 당일조회
	char chNowData = '0';
//	if( chTypeRequested == DAY_DATA_CHART	|| 
//		chTypeRequested == WEEK_DATA_CHART	|| 
//		chTypeRequested == MONTH_DATA_CHART )
//		chNowData = '0';
//	else
//	{
//		CString strQueryOpt = m_pChartItem->GetQueryOpt();
//		if(strQueryOpt == "1")
//			chNowData = '1';
//	}
	memcpy(eu0606InRec1.nowData, &chNowData, sizeof(eu0606InRec1.nowData));

	CString strRecordInfo("0                                                                               ");
	CString strEU0606Info;
	char* szEU0606Info = strEU0606Info.GetBuffer(sizeof(eu0606InRec1));
	memcpy(szEU0606Info, (char*)&eu0606InRec1, sizeof(eu0606InRec1));
	strEU0606Info.ReleaseBuffer();
	CString strFidInfo("1               000001002003004005008009011012|2               000001002003004005;");
	CString strEU0606InRec1;
	strEU0606InRec1 = strRecordInfo + strEU0606Info + strFidInfo;
	return strEU0606InRec1;
}
*/

//시장지표 TR을 날리기 전에 Input값을 셋팅힌다.
CString CChartItemData::GetMarketQueryData_ChartItemData(BOOL bNext, UINT nRqstCnt)
{
	m_lDataSize = 0;
	m_lCnt = 0;
//	if(		(m_strTRCode == QUERY_STRMARKET_10450)	||
//			(m_strTRCode == QUERY_STRMARKET_10451)	||
//			(m_strTRCode == QUERY_STRMARKET_10452)
//		)
	if(m_strTRCode == QUERY_STRMARKET)
		return GetQueryDataMarket(bNext, nRqstCnt);//return GetQueryDataMarket(bNext);		

	return "";
}

BOOL CChartItemData::MakeChartData(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	BOOL r = FALSE;
	if(m_strTRCode == QUERY_STRFOFOREIGN)
		return MakeChartData_EU_p0602(szData, nLength, lpSelChart);		
//	else if(m_strTRCode == QUERY_STRFOFOREIGN_HOGA)
//		return MakeChartData_EU_p0603(szData, nLength, lpSelChart);		
/*
	else if(m_strTRCode == QUERY_STRFUTOPT)
		return MakeChartData_EU_p0604(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRSTOCKFUTOPT)
		return MakeChartData_EU_p0605(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRFOREIGN)
		return MakeChartData_EU_p0606(szData, nLength, lpSelChart);
	else if(m_strTRCode == QUERY_STRMARKET)
		return MakeChartMarektData(szData, nLength);		
*/
	return FALSE;
}

BOOL CChartItemData::MakeChartData_EU_p0602(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
// --> [Edit]  강지원 2008/07/28		( 서버에서 받은 차트 데이터를 차트에 넘겨주기 위해 데이터를 가공하는 부분 )
	m_strOpen	= "";			//	시가
	m_strHigh	= "";			//	고가
	m_strLow	= "";			//	저가

	if (szData == NULL)
		return FALSE;

	//	데이터 카운트 구하기
	CString strDataCnt = "", strTemp = "";
	long nDataCnt = 0;

	strDataCnt = CString((char*)szData/*+sizeof(m_pEu0602_ChartData->nkey)*/, sizeof(m_pEu0602_ChartData->fcnt));
	nDataCnt = atol(strDataCnt);

	if (m_pEu0602_ChartData)
	{
		char *pData = (char*)m_pEu0602_ChartData;
		delete [] pData;
		m_pEu0602_ChartData = NULL;
	}

	long nDataSize = /*sizeof(m_pEu0602_ChartData->nkey) + */sizeof(m_pEu0602_ChartData->fcnt) + sizeof(m_pEu0602_ChartData->bojomsg) + (nDataCnt * sizeof(EU_p0602_OutRec2));
	m_pEu0602_ChartData = (EU_p0602_CHART*)new char[nDataSize*2];

	m_strName = m_strCode = m_strCodeForRequest;


	// 종목명
	long lCnt = 0;

	// 차트카운트
	lCnt = m_lCntTemp = nDataCnt;

	long lPosForChart = 0, nBojoMsgLen = 0;

	// 차트데이터
	strDataCnt = CString((char*)szData + /*sizeof(m_pEu0602_ChartData->nkey) +*/ sizeof(m_pEu0602_ChartData->fcnt), BOJO_MSG_LENGTH_SIZE);
	nBojoMsgLen = atoi(strDataCnt) + BOJO_MSG_LENGTH_SIZE;

	m_lDataSize = lPosForChart = nLength;// - nBojoMsgLen - sizeof(m_pEu0602_ChartData->fcnt);	//	nLength - (&pChartData->fcnt[0] - &pChartData->name[0]);
	memcpy(m_pEu0602_ChartData, (char*)szData , lPosForChart);

//@Solomon-->
	m_pChartItem->EnableRequestMoreButton(TRUE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)

	CString sKey = "";
	sKey = lpSelChart ? lpSelChart->m_strKey : m_pChartItem->m_strNextKey;
	sKey.TrimLeft();	sKey.TrimRight();

	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, (LPCTSTR)sKey, sKey.GetLength());

//	sKey.Format("%18.18s", pChartData->nkey);
//	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
//	if( atoi(sKey)  == 0)
//		memset(m_chkey,0x00,sizeof(m_chkey));
//
//	if(sKey.GetLength() > 8)
//	{
//		sKey.Delete(0, 8);
//		if( atoi(sKey)  == 0)
//			memset(m_chkey,0x00,sizeof(m_chkey));
//	}

	if (sKey.IsEmpty() || atol(sKey) == 0)
		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

//	if(atoi(sKey) == 0)
//		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

//	if(lpSelChart)
//		// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
//		lpSelChart->m_strKey.Format("%18.18s", m_chkey);

//@Solomon<--

//	char *pszStart = strstr((char*)m_pEu0602_ChartData, _T("HBPRICE="));
	char *pszStart = strstr((char*)m_pEu0602_ChartData, _T("DECTYPE="));
	if (pszStart != NULL)
	{
		char szTemp[64];
		memset(szTemp, 0x00, sizeof(szTemp));
//		pszStart += sizeof(_T("HBPRICE="));
		pszStart += strlen(_T("DECTYPE="));
		char *pszEnd = strstr(pszStart, _T("@"));
		if (pszEnd != NULL)
		{
			memcpy(szTemp, pszStart, pszEnd-pszStart);
			m_strPosition.Format(_T("%s"), szTemp);
		}
	}
//	m_strPosition = m_pChartItem->GetCodePrecisionFormat(m_strCode);




	return TRUE;
}
/*
BOOL CChartItemData::MakeChartData_EU_p0603(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	m_strOpen	= "";			//	시가
	m_strHigh	= "";			//	고가
	m_strLow	= "";			//	저가

	if (szData == NULL)
		return FALSE;

	//	데이터 카운트 구하기
	CString strDataCnt = "", strTemp = "";
	long nDataCnt = 0;

	strDataCnt = CString((char*)szData, sizeof(m_pEu0603_ChartData->fcnt));
	nDataCnt = atol(strDataCnt);

	m_pChartItem->EnableRequestMoreButton(FALSE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)

	if (m_pEu0603_ChartData)
	{
		char *pData = (char*)m_pEu0603_ChartData;
		delete [] pData;
		m_pEu0603_ChartData = NULL;
	}

	long nDataSize = sizeof(m_pEu0603_ChartData->fcnt) + sizeof(m_pEu0603_ChartData->bojomsg) + ( nDataCnt * sizeof(EU_p0602_OutRec2) );
	m_pEu0603_ChartData = (EU_p0603_CHART*)new char[nDataSize*2];

	m_strName = m_strCode = m_strCodeForRequest;

	long lCnt = 0;
	lCnt = m_lCntTemp = nDataCnt;

	long lPosForChart = 0, nBojoMsgLen = 0;

	strDataCnt = CString((char*)szData + sizeof(m_pEu0603_ChartData->fcnt), BOJO_MSG_LENGTH_SIZE);
	nBojoMsgLen = atoi(strDataCnt) + BOJO_MSG_LENGTH_SIZE;

	m_lDataSize = lPosForChart = nLength;// - nBojoMsgLen - sizeof(m_pEu0603_ChartData->fcnt);	//	nLength - (&pChartData->fcnt[0] - &pChartData->name[0]);
	memcpy(m_pEu0603_ChartData, (char*)szData , lPosForChart);

	char *pszStart = strstr((char*)m_pEu0603_ChartData, _T("HBPRICE="));
	if (pszStart != NULL)
	{
		char szTemp[64];
		memset(szTemp, 0x00, sizeof(szTemp));
		pszStart += sizeof(_T("HBPRICE="));
		char *pszEnd = strstr(pszStart, _T("@"));
		if (pszEnd != NULL)
		{
			memcpy(szTemp, pszStart, pszEnd-pszStart);
			m_strPosition.Format(_T("%s"), szTemp);
		}
	}
//	m_strPosition = m_pChartItem->GetCodePrecisionFormat(m_strCode);

	return TRUE;
}

BOOL CChartItemData::MakeChartData_EU_p0604(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	EU_p0604_OutRec1_EX* pEu0604_OutRec1 = (EU_p0604_OutRec1_EX*)szData;
	if(pEu0604_OutRec1 == NULL) return FALSE;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pEu0604_OutRec1->nkey, sizeof(pEu0604_OutRec1->nkey));

	m_pChartItem->EnableRequestMoreButton(TRUE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)

	CString sKey;
	sKey.Format("%21.21s", pEu0604_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
	if( atoi(sKey)  == 0)
		memset(m_chkey,0x00,sizeof(m_chkey));

	if(sKey.GetLength() > 8)
	{
		sKey.Delete(0, 8);
		if( atoi(sKey)  == 0)
			memset(m_chkey,0x00,sizeof(m_chkey));
	}

	if(atoi(sKey) == 0)
		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

	if(lpSelChart)
		// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
		lpSelChart->m_strKey.Format("%18.18s", m_chkey);

	if(!m_pEu0604_ChartData)
	{
		//m_pEu0604_ChartData = new EU_p0604_CHART;
		m_pEu0604_ChartData = (EU_p0604_CHART*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pEu0604_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp, pEu0604_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pEu0604_OutRec1->fcnt[0] - &pEu0604_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pEu0604_OutRec1->fcnt[0] - &pEu0604_OutRec1->name[0]);
	memcpy(m_pEu0604_ChartData, &pEu0604_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pEu0604_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pEu0604_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);
	return TRUE;
}

BOOL CChartItemData::MakeChartData_EU_p0605(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	EU_p0605_OutRec1_EX* pEu0605_OutRec1 = (EU_p0605_OutRec1_EX*)szData;
	if(pEu0605_OutRec1 == NULL) return FALSE;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pEu0605_OutRec1->nkey, sizeof(pEu0605_OutRec1->nkey));

	m_pChartItem->EnableRequestMoreButton(TRUE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)

	CString sKey;
	sKey.Format("%21.21s", pEu0605_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
	if( atoi(sKey)  == 0)
		memset(m_chkey,0x00,sizeof(m_chkey));

	if(sKey.GetLength() > 8)
	{
		sKey.Delete(0, 8);
		if( atoi(sKey)  == 0)
			memset(m_chkey,0x00,sizeof(m_chkey));
	}

	if(atoi(sKey) == 0)
		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

	if(lpSelChart)
		// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
		lpSelChart->m_strKey.Format("%18.18s", m_chkey);

	if(!m_pEu0605_ChartData)
	{
		//m_pEu0605_ChartData = new EU_p0605_CHART;
		m_pEu0605_ChartData = (EU_p0605_CHART*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pEu0605_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp, pEu0605_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pEu0605_OutRec1->fcnt[0] - &pEu0605_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pEu0605_OutRec1->fcnt[0] - &pEu0605_OutRec1->name[0]);
	memcpy(m_pEu0605_ChartData, &pEu0605_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pEu0605_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pEu0605_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);
	return TRUE;
}

BOOL CChartItemData::MakeChartData_EU_p0606(void* szData, long nLength, ST_SELCHART* lpSelChart) //해외종목
{
	EU_p0606_OutRec1_EX* pEu0606_OutRec1 = (EU_p0606_OutRec1_EX*)szData;
	if(pEu0606_OutRec1 == NULL) return FALSE;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pEu0606_OutRec1->nkey, sizeof(pEu0606_OutRec1->nkey));

	m_pChartItem->EnableRequestMoreButton(TRUE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)

	CString sKey;
	sKey.Format("%21.21s", pEu0606_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
	if( atoi(sKey)  == 0)
		memset(m_chkey,0x00,sizeof(m_chkey));

	if(sKey.GetLength() > 8)
	{
		sKey.Delete(0, 8);
		if( atoi(sKey)  == 0)
			memset(m_chkey,0x00,sizeof(m_chkey));
	}

	if(atoi(sKey) == 0)
		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

	if(lpSelChart)
		// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
		lpSelChart->m_strKey.Format("%18.18s", m_chkey);

	if(!m_pEu0606_ChartData)
	{
		//m_pEu0606_ChartData = new EU_p0606_CHART;
		m_pEu0606_ChartData = (EU_p0606_CHART*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pEu0606_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp, pEu0606_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pEu0606_OutRec1->fcnt[0] - &pEu0606_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pEu0606_OutRec1->fcnt[0] - &pEu0606_OutRec1->name[0]);
	memcpy(m_pEu0606_ChartData, &pEu0606_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pEu0606_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pEu0606_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}
*/

CString CChartItemData::GetTrCode()
{
//	CString strTrCode;
//	strTrCode.Format("%d",m_lTrCode);
//	return strTrCode;
	return m_strTRCode;
}


//---------------------------------------------
// 한화에서는 안 쓴다.
// 05. 05. 06 
// comment by sy, nam
//---------------------------------------------
long CChartItemData::GetLTrCode()
{
	return 0;//m_lTrCode;
}


BOOL CChartItemData::RemovePacketAndGraph()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemovePacketAndGraph::Begin", _T("멀티차트 종료"), 13);
#endif
	CString strTitle;
	long lSize = m_strArrayGraphTitle.GetSize();
	for(long lPos=0;lPos<lSize;lPos++)
	{
		strTitle = m_strArrayGraphTitle.GetAt(lPos);
		m_pChartItem->RemoveChart(strTitle);
	}
	m_strArrayGraphTitle.RemoveAll();
	lSize = m_strArrayPacketName.GetSize();
	for(lPos=0;lPos<lSize;lPos++)
	{
		strTitle = m_strArrayPacketName.GetAt(lPos);
		m_pChartItem->RemovePacket(strTitle);
	}
	m_strArrayPacketName.RemoveAll();
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemovePacketAndGraph::End", _T("멀티차트 종료"), 13);
#endif
	return TRUE;
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
BOOL CChartItemData::AddLoadedPacket(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart)
{
	m_strArrayPacketName.RemoveAll();
	if(lpSelChart->m_strChartGubun == QUERY_STRFOFOREIGN)
	{
		// 2011.01.13 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		// 2011.01.13 by SYS <<

		m_strArrayPacketName.Add(g_strPacketName[0]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[1]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[2]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[3]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[4]);

		
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("시가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("시가GuideLine");				
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("고가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("고가GuideLine");				
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption("저가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add("저가GuideLine");				
	}
/*	else if(lpSelChart->m_strChartGubun == QUERY_STRFOFOREIGN_HOGA)
	{
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[0]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[1]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[2]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[3]);
		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[4]);

		m_pChartItem->AddPacketRQ(lpszRQ, MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
		m_strArrayPacketName.Add(g_strPacketName[5]);
	}
*/

	return TRUE;
}

BOOL CChartItemData::AddPacketAndGraph(BOOL IsNoVolumeGraph /*= FALSE*/)
{	
	if(!m_bInitalChart)
	{
#ifdef _WRITELOG		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX로 AddPacket/Graph::Begin"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
		m_strArrayPacketName.RemoveAll();
		BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
		if(m_pChartItem->GetPtrST())// it depends on System Trading Spec. gm0604 2005/10/04
		{	
			ASSERT(IsPrimaryChartItemData);
			if(m_strTRCode == QUERY_STRFOFOREIGN)
			{
				// 2011.01.13 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
				//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				// 2011.01.13 by SYS <<

				m_strArrayPacketName.Add(g_strPacketName[0]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[1]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[2]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[3]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[4]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[5]);		
			}
/*			else if(m_strTRCode == QUERY_STRFOFOREIGN_HOGA)
			{
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[0]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[1]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[2]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[3]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[4]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[5]);		
			}
*/
			AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
			
			m_strArrayGraphTitle.Add("가격 이동평균");
			m_pChartItem->AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);

			m_strArrayGraphTitle.Add(g_strIndexName[1]);
			m_pChartItem->AddGraph(1,0,0,"거래량차트",TRUE,TRUE,TRUE);

			m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
		}
		else
		{	
			if(IsPrimaryChartItemData)
			{			
				if(m_strTRCode == QUERY_STRFOFOREIGN)
				{
					// 2011.01.13 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					// 2011.01.13 by SYS 
					m_strArrayPacketName.Add(g_strPacketName[0]);

					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
#ifdef _DEBUG
//						AfxMessageBox("CChartItemData::AddPacketAndGraph의 \n Dollar 로 넘어가는 부분  처리 해라...");
#endif
					}

					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[5]);

					m_pChartItem->AddPacket(MakePacketOption("시가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("시가GuideLine");				
					m_pChartItem->AddPacket(MakePacketOption("고가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("고가GuideLine");				
					m_pChartItem->AddPacket(MakePacketOption("저가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("저가GuideLine");				

					if(!IsNoVolumeGraph)
					{
						AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
						m_strArrayGraphTitle.Add("가격 이동평균");
						m_pChartItem->AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);

						m_strArrayGraphTitle.Add(g_strIndexName[1]);
						m_pChartItem->AddGraph(1,0,0,"거래량차트",TRUE,TRUE,TRUE);
					}
					else
					{
						m_strArrayGraphTitle.Add(g_strIndexName[0]);
						m_pChartItem->AddGraph(0,0,0,"가격차트",FALSE,TRUE,TRUE);

						m_strArrayGraphTitle.Add("가격 이동평균");
						m_pChartItem->AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);

						m_pChartItem->HideScale(FALSE);
					}					

					// 복수종목 불러오기 : 복수종목 - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}
				}
/*				else if(m_strTRCode == QUERY_STRFOFOREIGN_HOGA)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);
					
					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
#ifdef _DEBUG
//						AfxMessageBox("CChartItemData::AddPacketAndGraph의 \n Dollar 로 넘어가는 부분  처리 해라...");
#endif
					}

					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[5]);

					if(!IsNoVolumeGraph)
					{
						AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
						m_strArrayGraphTitle.Add("가격 이동평균");
						m_pChartItem->AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);

						m_strArrayGraphTitle.Add(g_strIndexName[1]);
						m_pChartItem->AddGraph(1,0,0,"거래량차트",TRUE,TRUE,TRUE);
					}
					else
					{
						m_strArrayGraphTitle.Add(g_strIndexName[0]);
						m_pChartItem->AddGraph(0,0,0,"가격차트",FALSE,TRUE,TRUE);

						m_strArrayGraphTitle.Add("가격 이동평균");
						m_pChartItem->AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);

						m_pChartItem->HideScale(FALSE);
					}					

					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}
				}
*/
			}
		}
		m_bInitalChart = TRUE;
	
#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX로 AddPacket/Graph::End"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
	}
	return TRUE;
}

// 새로운 패킷,차트 추가 : 복수종목 - ojtaso (20061224)
BOOL CChartItemData::AddPacketAndGraph2(BOOL bAddGraph /*= TRUE*/, BOOL bOverlapp /*= FALSE*/)
{
	if(!bAddGraph)
	{
		CString strTitle;
		long lSize = m_strArrayPacketName.GetSize();
		for(long lPos=0;lPos<lSize;lPos++)
		{
			strTitle = m_strArrayPacketName.GetAt(lPos);
			m_pChartItem->RemovePacket(strTitle);
		}
		m_strArrayPacketName.RemoveAll();
	}

//	if(!m_bInitalChart)
	{
#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX로 AddPacket/Graph::Begin"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
		m_strArrayPacketName.RemoveAll();
		BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
		if(m_pChartItem->GetPtrST())// it depends on System Trading Spec. gm0604 2005/10/04
		{	
			ASSERT(IsPrimaryChartItemData);
			if(m_strTRCode == QUERY_STRFOFOREIGN)
			{
				// 2011.01.13 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
				//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				// 2011.01.13 by SYS 

				m_strArrayPacketName.Add(g_strPacketName[0]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[1]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[2]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[3]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[4]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[5]);		
			}
/*			else if(m_strTRCode == QUERY_STRFOFOREIGN_HOGA)
			{
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[0]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[1]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[2]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[3]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[4]);
				m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
				m_strArrayPacketName.Add(g_strPacketName[5]);		
			}
*/
			AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart

			m_strArrayGraphTitle.Add("가격 이동평균");
			m_pChartItem->AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);


			m_strArrayGraphTitle.Add(g_strIndexName[1]);
			m_pChartItem->AddGraph(1,0,0,"거래량차트",TRUE,TRUE,TRUE);
		}
		else
		{	
			if(IsPrimaryChartItemData)
			{			
				if(m_strTRCode == QUERY_STRFOFOREIGN)
				{
					// 2011.01.13 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
					//m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],14,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					// 2011.01.13 by SYS 
					m_strArrayPacketName.Add(g_strPacketName[0]);

					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
#ifdef _DEBUG
//						AfxMessageBox("CChartItemData::AddPacketAndGraph의 \n Dollar 로 넘어가는 부분  처리 해라...");
#endif
					}

					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[5]);

					m_pChartItem->AddPacket(MakePacketOption("시가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("시가GuideLine");				
					m_pChartItem->AddPacket(MakePacketOption("고가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("고가GuideLine");				
					m_pChartItem->AddPacket(MakePacketOption("저가GuideLine",1,g_strPacketTypeEtc[1],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add("저가GuideLine");				

					// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
					int nRow, nCol;
					if(bOverlapp == FALSE)
						AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);
					else
						AddGraphPriceChart_Overlapp(nRow, nCol);

					// 복수종목 불러오기 : 복수종목 - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}
				}
/*				else if(m_strTRCode == QUERY_STRFOFOREIGN_HOGA)
				{
					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[0]);

					//if(m_pChartItem->GetUseRateValue())
					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[7],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[7],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,m_strPosition,FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,m_strPosition,TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
					}
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)
					{
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[1]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[2]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[6],FALSE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[3]);
						m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[6],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
						m_strArrayPacketName.Add(g_strPacketName[4]);
#ifdef _DEBUG
//						AfxMessageBox("CChartItemData::AddPacketAndGraph의 \n Dollar 로 넘어가는 부분  처리 해라...");
#endif
					}

					m_pChartItem->AddPacket(MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, m_nCntOverLap, m_bIsFilled, m_bOverLapping));
					m_strArrayPacketName.Add(g_strPacketName[5]);

					// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
					int nRow, nCol;
					if(bOverlapp == FALSE)
						AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);
					else
						AddGraphPriceChart_Overlapp(nRow, nCol);

					// 복수종목 불러오기 : 복수종목 - ojtaso (20070529)
					//m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					if(!m_bResetChartCfgWith)
					{
						m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
						m_bResetChartCfgWith = TRUE;
					}
				}				
*/
			}
		}
		m_bInitalChart = TRUE;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	m_pChartItem->WriteToStringLog(_T("OCX로 AddPacket/Graph::End"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
	return TRUE;
}

//KHD : 추가 ChartItem클래스에서 AddPacketAndGraphAnal 호출된다. 
BOOL CChartItemData::AddPacketAndGraphAnal()
{
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Function Name		: AddGraphPriceChart()
// Function description : 가격차트를 블럭에 추가한다
// Author				: 유제선 (Yoo Je Sun)
// Made Date			: 2006. 01. 22
///////////////////////////////////////////////////////////////////////////////
void CChartItemData::AddGraphPriceChart(void)
{
	int RowCount = (int)m_pChartItem->GetRowBlockCount();
	int ColumnCount = (int)m_pChartItem->GetColumnBlockCount();

	m_strArrayGraphTitle.Add(g_strIndexName[0]);
	if( RowCount <= 0 || ColumnCount <= 0 )
	{
		m_pChartItem->AddGraph(0,0,0,"가격차트",TRUE,TRUE,TRUE);
	}
	else
	{
		m_pChartItem->AddGraph(0,0,0,"가격차트",FALSE,FALSE,FALSE);
	}
}
///////////////////////////////////////////////////////////////////////////////

// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
// (2008/9/14 - Seung-Won, Bae) for Pair Tick
void CChartItemData::AddGraphPriceChart2NewItem(int& nRow, int& nCol, BOOL bAddGraph)
{
	m_strArrayGraphTitle.Add(g_strIndexName[0]);

	int nMainRow = -1;
	CString strPairRQ = m_pChartItem->GetSelectedRQ();
	CString strMainRQ( strPairRQ);
	if( strPairRQ.Right( 5) != "_PAIR")	strPairRQ = strPairRQ + "_PAIR";
	else								strMainRQ = strMainRQ.Left( strMainRQ.GetLength() - 5);
	int nRequestPairTickState = ( ( CStdDialog *)m_pChartItem->m_pStdDialog)->GetRequestPairTickState();
	switch( nRequestPairTickState)
	{
		case E_RPTS_PAIR:
		case E_RPTS_LOOP_PAIR:
		case E_PRTS_ROTATE_PAIR:
			bAddGraph = !m_pChartItem->HasIndicatorWithRQ( "가격차트", strPairRQ);
			if( bAddGraph) nMainRow = m_pChartItem->GetRowOfGraphWithRQ( "가격차트", strMainRQ);
			break;
	}
	if(bAddGraph)
	{
		int RowCount = (int)m_pChartItem->GetRowBlockCount();
		int ColumnCount = (int)m_pChartItem->GetColumnBlockCount();

		if( RowCount <= 0 || ColumnCount <= 0 )
		{
			m_pChartItem->AddGraph(RowCount,0,0,"가격차트",TRUE,TRUE,TRUE);
		}
		else
		{
			switch( nRequestPairTickState)
			{
				case E_RPTS_PAIR:
				case E_RPTS_LOOP_PAIR:
				case E_PRTS_ROTATE_PAIR:
					RowCount = nMainRow;
					m_pChartItem->AddGraph(RowCount,0,0,"가격차트",FALSE,FALSE,TRUE);
					break;
				default:
					m_pChartItem->AddGraph(RowCount,0,0,"가격차트",TRUE,TRUE,TRUE);
					break;
			}
		}

		nRow = RowCount;
		nCol = 0;
	}

	if( nRequestPairTickState != E_RPTS_PAIR
		&& nRequestPairTickState != E_RPTS_LOOP_PAIR
		&& nRequestPairTickState != E_PRTS_ROTATE_PAIR)
	{
//		m_strArrayGraphTitle.Add("가격 이동평균");
//		m_pChartItem->AddGraph(nRow,nCol,0,"가격 이동평균",FALSE,FALSE,FALSE);
	}
}

// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
void CChartItemData::AddGraphPriceChart_Overlapp(int& nRow, int& nCol)
{
// --> [Edit]  강지원 2008/08/26	( 일단 막기 ) 우리선물용 차트로 바꾸면서 함수가 사라짐... 
//	int nVertScaleGroup = m_pChartItem->GetVertScaleGroupCount(0, 0);
//	if(nVertScaleGroup < 0)
//		return;
//
//	m_strArrayGraphTitle.Add(g_strIndexName[0]);
//
//	nRow = nCol = 0;
//	m_pChartItem->AddGraph(nRow,nCol,nVertScaleGroup,"가격차트",FALSE,TRUE,TRUE);
//
//	m_strArrayGraphTitle.Add("가격 이동평균");
//	m_pChartItem->AddGraph(nRow,nCol,nVertScaleGroup,"가격 이동평균",FALSE,FALSE,FALSE);
// <-- [Edit]  강지원 2008/08/26
}

CString* CChartItemData::GetPstrPacketName(long lPosByOutput, long lPosByInput,CString *pstrPacketName,CString *pstrPacketName1)
{
	CString *pStr = NULL;
	long lSize = sizeof(g_lpszInputMarketIndicator)/sizeof(LPCTSTR) - 1;
	if( lPosByInput < lSize  && pstrPacketName1->IsEmpty()
		&&m_strName==g_lpszInputMarketIndicator[lPosByInput]
		&&g_lpszInputMarketIndicator[lPosByInput]==g_lpszOutputMarketIndicator[lPosByOutput]
		)
	{
		pStr = pstrPacketName1;
		*pStr = g_lpszOutputMarketIndicator[lPosByOutput];
	}
	else if( lPosByInput < lSize  
			&& m_strName==g_lpszInputMarketIndicator[lPosByInput]
			&& g_lpszInputMarketIndicator[lPosByInput]!=g_lpszOutputMarketIndicator[lPosByOutput]
		)
	{
		pStr = pstrPacketName1;
		*pStr = g_lpszOutputMarketIndicator[lPosByOutput];
	}
	else
	{
		CString strNum;
		strNum.Format("tmp%d",lPosByOutput);
		pStr = pstrPacketName;
		*pStr = m_strName + strNum; 
	}			
	return pStr;
}

CString CChartItemData::GetPacketName()
{
	CString strPacketName;
	CString strPacketNameTemp;
	long lSize = m_strArrayPacketName.GetSize();
	for(long lPos=0;lPos<lSize;lPos++)
	{
		strPacketNameTemp = m_strArrayPacketName.GetAt(lPos);
		if(strPacketNameTemp.Find("GuideLine")==-1)
		{
			strPacketName += strPacketNameTemp;
			strPacketName += ";";
		}
	}
	return strPacketName;
}

BOOL CChartItemData::MakeChartMarektData(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	CString strPacketCnt;

//@유진삭제
	EU_p0607_OutRec_1* pEu0607OutRec = (EU_p0607_OutRec_1*)szData;
//@유진삭제
	strPacketCnt.Format("%1.1s", pEu0607OutRec->inNumGb);	

	int nMarketJipyCnt = atoi(strPacketCnt);
	//AfxMessageBox(strPacketCnt);
	
	if(nMarketJipyCnt == 2) //지표수가 2개
	{
		EU_p0607_OutRec_1* pEu0607_OutRec_1 = (EU_p0607_OutRec_1*)szData;

		if(nLength < (&pEu0607_OutRec_1->eu0607_CHART.chtCnt[0] - &pEu0607_OutRec_1->winid[0]))
			return FALSE;

		CString sOriCode, sRecvCode;
		sRecvCode.Format("%8.8s", pEu0607_OutRec_1->shcode); sRecvCode.TrimLeft(); sRecvCode.TrimRight();
		sOriCode = m_pChartItem->GetSelectedItemCode();
		if(sOriCode.CompareNoCase(sRecvCode) != 0)
		{		
			CString strTR;
			strTR = QUERY_STRMARKET;

			m_pChartItem->RemoveChartItem_OneData(m_strCodeForRequest, strTR);
			return FALSE;
		}

		memset(m_chkey,0x00,sizeof(m_chkey));
		memcpy(m_chkey, pEu0607_OutRec_1->nxtKey, sizeof(pEu0607_OutRec_1->nxtKey));

		if(lpSelChart)
			// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
			lpSelChart->m_strKey.Format("%18.18s", m_chkey);

		if(!m_pEu0607_ChartData)
		{
			//m_pEu0607_ChartData = (char*)(new EU_p0607_CHART_1);
			m_pEu0607_ChartData = new char[nLength];
			//memset(m_pEu0607_ChartData, 0x00, sizeof(EU_p0607_CHART_1));
			memset(m_pEu0607_ChartData, 0x00, nLength);
		}
		else
		{
			//memset(m_pEu0607_ChartData, 0x00, sizeof(EU_p0607_CHART_1));
			char* pData = m_pEu0607_ChartData;
			//delete m_pEu0607_ChartData;
			delete [] pData;
			m_pEu0607_ChartData = NULL;
		}
		m_strCode = m_strCodeForRequest;	
		m_strName = m_strCode;
		char szTemp[64];
		long lSize = 0;

		MEMCPYWITHNULL(szTemp, pEu0607_OutRec_1->eu0607_CHART.chtCnt);
		lSize = atol(szTemp);

		if(lSize <= 0) 
		{
			if( m_pEu0607_ChartData )
			{
				//memset(m_pEu0607_ChartData, 0x00, sizeof(EU_p0607_CHART_1));
				char* pData = m_pEu0607_ChartData;
				//delete m_pEu0607_ChartData;
				delete [] pData;
				m_pEu0607_ChartData = NULL;
			}
//			AfxMessageBox("Err 2");
			return FALSE;
		}


		MEMCPYWITHNULL(szTemp, pEu0607_OutRec_1->eu0607_CHART.chart[0].date);
		m_lStartDate = atol(szTemp);
		m_lCntTemp = lSize;

		long lPosForChart = 0;
		lPosForChart = nLength - (&pEu0607_OutRec_1->eu0607_CHART.chtCnt[0] - &pEu0607_OutRec_1->winid[0]);		
		memcpy(m_pEu0607_ChartData, &pEu0607_OutRec_1->eu0607_CHART.chtCnt[0], lPosForChart);
		
		m_lDataSize = lPosForChart;

		MEMCPYWITHNULL(szTemp, pEu0607_OutRec_1->eu0607_CHART.chart[lSize-1].date);
		m_lEndDate = atol(szTemp);	

		
//			CString strData;			
//			strData.Format("Query__%.*s", 100, m_pSt10450Data);
//			OutputDebugString(strData);
			
//			AfxMessageBox(strData);

		long lSizeOfChtPacket = SZ_TR_MARKET_1_OS;
	
	
/*		char szTempBig[512];
		int lPos = 0;
		int lPos2 = 0;
		for(lPos = 0;lPos<lSize;lPos++)
		{
			memset(szTempBig,0x00,sizeof(szTempBig));
			memcpy(szTempBig,&stOutput.os_buf[lPos],lSizeOfChtPacket);
			for(lPos2 = 8;lPos2<lSizeOfChtPacket;lPos2++) //lPos2 = 8 인 이유는 날짜 부분은 건너뛰기 위해 
			{
				if(szTempBig[lPos2]>' ')
				{
					MEMCPYWITHNULL(szTemp,stOutput.os_buf[lPos].date);
					m_lStartDate = atol(szTemp);
					m_lCntTemp = lSize-lPos;
					return TRUE;
				}
			}		
		}
*/
		return TRUE;
	}
	else if(nMarketJipyCnt == 4)//지표수가 
	{
		EU_p0607_OutRec_3* pEu0607_OutRec_3 = (EU_p0607_OutRec_3*)szData;

		if(nLength < (&pEu0607_OutRec_3->eu0607_CHART.chtCnt[0] - &pEu0607_OutRec_3->winid[0]))
			return FALSE;

		memset(m_chkey, 0x00, sizeof(m_chkey));
		memcpy(m_chkey, pEu0607_OutRec_3->nxtKey, sizeof(pEu0607_OutRec_3->nxtKey));

		if(lpSelChart)
			// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
			lpSelChart->m_strKey.Format("%18.18s", m_chkey);

		if(!m_pEu0607_ChartData)
		{
			//m_pEu0607_ChartData = (char*)(new EU_p0607_CHART_3);
			//memset(m_pEu0607_ChartData, 0x00, sizeof(EU_p0607_CHART_3));
			m_pEu0607_ChartData = new char[nLength];
			memset(m_pEu0607_ChartData, 0x00, nLength);
		}
		else
		{
			//memset(m_pEu0607_ChartData, 0x00, sizeof(EU_p0607_CHART_3));
			//delete m_pEu0607_ChartData;
			char* pData = m_pEu0607_ChartData;
			//delete m_pEu0607_ChartData;
			delete [] pData;
			m_pEu0607_ChartData = NULL;
		}

		m_strCode = m_strCodeForRequest;	
		m_strName = m_strCode;
		char szTemp[64];
		long lSize = 0;

		MEMCPYWITHNULL(szTemp, pEu0607_OutRec_3->eu0607_CHART.chtCnt);
		lSize = atol(szTemp);

		if(lSize <= 0) 
		{
			if( m_pEu0607_ChartData )
			{
				//memset(m_pEu0607_ChartData, 0x00, sizeof(EU_p0607_CHART_3));
				//delete m_pEu0607_ChartData;
				char* pData = m_pEu0607_ChartData;
				//delete m_pEu0607_ChartData;
				delete [] pData;
				m_pEu0607_ChartData = NULL;
			}
//			AfxMessageBox("Err 4");
			return FALSE;
		}


		MEMCPYWITHNULL(szTemp, pEu0607_OutRec_3->eu0607_CHART.chart[0].date);
		m_lStartDate = atol(szTemp);
		m_lCntTemp = lSize;

		long lPosForChart = 0;
		lPosForChart = nLength - (&pEu0607_OutRec_3->eu0607_CHART.chtCnt[0] - &pEu0607_OutRec_3->nxtKey[0]);
		memcpy(m_pEu0607_ChartData, &pEu0607_OutRec_3->eu0607_CHART.chtCnt[0], lPosForChart);

		m_lDataSize = lPosForChart;

		MEMCPYWITHNULL(szTemp, pEu0607_OutRec_3->eu0607_CHART.chart[lSize-1].date);
		m_lEndDate = atol(szTemp);	

		
		long lSizeOfChtPacket = SZ_TR_MARKET_3_OS;
	
	
	/*	char szTempBig[512];
		int lPos = 0;
		int lPos2 = 0;
		for(lPos = 0;lPos<lSize;lPos++)
		{
			memset(szTempBig,0x00,sizeof(szTempBig));
			memcpy(szTempBig,&stOutput.os_buf[lPos],lSizeOfChtPacket);
			for(lPos2 = 8;lPos2<lSizeOfChtPacket;lPos2++) //lPos2 = 8 인 이유는 날짜 부분은 건너뛰기 위해 
			{
				if(szTempBig[lPos2]>' ')
				{
					MEMCPYWITHNULL(szTemp,stOutput.os_buf[lPos].date);
					m_lStartDate = atol(szTemp);
					m_lCntTemp = lSize-lPos;
					return TRUE;
				}
			}		
		}
	*/
		return TRUE;
	}
	
//	AfxMessageBox("Err 5");
	return FALSE;
}

//DEL BOOL CChartItemData::MakeChartData10451(void* szData, long nLength)
//DEL {
//DEL //	char pCnt;
//DEL //	memcpy(&pCnt, (char*)szData, 1);
//DEL //	int nMarketJipyCnt = atoi(&pCnt);
//DEL 
//DEL //	if(nMarketJipyCnt == 2) //지표수가 2개
//DEL 	{
//DEL 		TR_10451_O stOutput;
//DEL 
//DEL 		if(nLength>sizeof(stOutput))
//DEL 		{			
//DEL 			ASSERT(0);
//DEL 			return FALSE;
//DEL 		}
//DEL 
//DEL 		memcpy(&stOutput,(void*)szData,nLength);
//DEL 		
//DEL 
//DEL 		//=============================================
//DEL 		//1. 종목 비교 해서 틀리면 처리 안한다.
//DEL 		//2. Array에서 지워버린다.
//DEL 		//=============================================
//DEL 		//1.
//DEL 			CString sOriCode, sRecvCode;
//DEL 			sRecvCode.Format("%7.7s", stOutput.shcode); sRecvCode.TrimLeft(); sRecvCode.TrimRight();
//DEL 			sOriCode = m_pChartItem->GetDataCode();
//DEL 			if(sOriCode.CompareNoCase(sRecvCode) != 0)
//DEL 			{	
//DEL 				m_pChartItem->RemoveChartItem_OneData(m_strCodeForRequest, QUERY_STRMARKET_NEWS);
//DEL 				return FALSE;
//DEL 			}
//DEL 		//=============================================
//DEL 
//DEL 			
//DEL 		memset(m_chkey,0x00,sizeof(m_chkey));
//DEL 		memcpy(m_chkey,stOutput.nxtKey,sizeof(stOutput.nxtKey));
//DEL 		if(!m_pSt10451Data)
//DEL 		{
//DEL 			m_pSt10451Data = new TR_10451_CHART;
//DEL 		}
//DEL 		m_strCode = m_strCodeForRequest;	
//DEL 		m_strName = m_strCode;
//DEL 		char szTemp[64];
//DEL 		long lSize = 0;
//DEL 
//DEL 		MEMCPYWITHNULL(szTemp,stOutput.chtCnt);
//DEL 		lSize = atol(szTemp);
//DEL 		
//DEL 		if(lSize <= 0)
//DEL 		{		
//DEL 			if( m_pSt10451Data )
//DEL 			{
//DEL 				delete m_pSt10451Data;
//DEL 				m_pSt10451Data = NULL;
//DEL 			}
//DEL 
//DEL 			return FALSE;
//DEL 		}
//DEL 
//DEL 		m_lCntTemp = lSize;
//DEL 
//DEL 		
//DEL 
//DEL 		MEMCPYWITHNULL(szTemp,stOutput.os_buf[0].date);
//DEL 		m_lStartDate = atol(szTemp);
//DEL 		
//DEL 
//DEL 		
//DEL 
//DEL 		long lPosForChart = 0;
//DEL 
//DEL 		lPosForChart = nLength - sizeof(stOutput.nxtKey);		
//DEL 		memcpy(m_pSt10451Data,&stOutput.chtCnt[0],lPosForChart);
//DEL 		
//DEL 		
//DEL //		if(!m_pChartItem->GetNext())
//DEL //		{
//DEL //			((TR_10451_1_CHART*)m_pSt10451Data)->Cbojo[7] = 'P';
//DEL //		}	
//DEL 		m_lDataSize = lPosForChart;
//DEL 
//DEL 		MEMCPYWITHNULL(szTemp,stOutput.os_buf[lSize-1].date);
//DEL 		m_lEndDate = atol(szTemp);	
//DEL 
//DEL 		
//DEL /*		long lSizeOfChtPacket = SZ_TR_10451_1_OS;
//DEL 	
//DEL 	
//DEL 		char szTempBig[512];
//DEL 		int lPos = 0;
//DEL 		int lPos2 = 0;
//DEL 		for(lPos = 0;lPos<lSize;lPos++)
//DEL 		{
//DEL 			memset(szTempBig,0x00,sizeof(szTempBig));
//DEL 			memcpy(szTempBig,&stOutput.os_buf[lPos],lSizeOfChtPacket);
//DEL 			for(lPos2 = 8;lPos2<lSizeOfChtPacket;lPos2++) //lPos2 = 8 인 이유는 날짜 부분은 건너뛰기 위해 
//DEL 			{
//DEL 				if(szTempBig[lPos2]>' ')
//DEL 				{
//DEL 					MEMCPYWITHNULL(szTemp,stOutput.os_buf[lPos].date);
//DEL 					m_lStartDate = atol(szTemp);
//DEL 					m_lCntTemp = lSize-lPos;
//DEL 					return TRUE;
//DEL 				}
//DEL 			}		
//DEL 		}
//DEL */
//DEL 		return TRUE;
//DEL 	}
//DEL 
//DEL 	if( m_pSt10451Data )
//DEL 	{
//DEL 		delete m_pSt10451Data;
//DEL 		m_pSt10451Data = NULL;
//DEL 	}
//DEL 
//DEL 	return FALSE;
//DEL 	
//DEL }

CString CChartItemData::GetQueryDataMarket(BOOL bNext, int nRqstCnt)
{
//@유진삭제
//	TR_MARKET_I stOutput;
//@유진삭제
//@유진추가
	EU_p0607_InRec1 eu0607InRec1;
//@유진추가

	char Temp[64];
	char chFlag;

	// 초기화
	memset(&eu0607InRec1, 0x20, sizeof(eu0607InRec1));
	
	// 윈도우아이디
	CString sWndID;
	sWndID.Format("%d", m_pChartItem->GetDlgCtrlID());
	memcpy(eu0607InRec1.winid, sWndID, sWndID.GetLength());

	// 데이타번호
	memcpy(eu0607InRec1.dataNm, (LPTSTR)(LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
	
	// 봉구분(0:허/1:실봉)
	eu0607InRec1.img[0] = '0';//허봉(0) 실봉(1);
	if(m_pChartItem->m_CfgInfo.bImaginaryCandle == false) //실봉이라면..
		eu0607InRec1.img[0] = '1';

	CString strPrimCode = m_pChartItem->GetSelectedItemCode();
	if(strPrimCode.GetLength() == UPJONG_LENGTH)// 업종
	{
		// 타입(0:종목/1:업종)
		eu0607InRec1.type [0]= '1';

		// 종목코드
		memcpy(eu0607InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(eu0607InRec1.shcode));
		eu0607InRec1.shcode[4] = ' ';	
		eu0607InRec1.shcode[5] = ' ';	
		eu0607InRec1.shcode[6] = ' ';	
		eu0607InRec1.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == CODE_STOCK_LENGTH || strPrimCode.GetLength() == (CODE_STOCK_LENGTH-1))							// 종목
	{
		// 타입(0:종목/1:업종)
		eu0607InRec1.type[0] = '0';

		// 종목코드
		memcpy(eu0607InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(eu0607InRec1.shcode));
		eu0607InRec1.shcode[6] = ' ';
		eu0607InRec1.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == FUTOPT_LENGTH)// || strPrimCode.GetLength() == COMMODITY_LENGTH)							// 선물
	{
		// 타입(0:종목/1:업종)
		eu0607InRec1.type[0] = '0';

		// 종목코드
		memcpy(eu0607InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(eu0607InRec1.shcode));
	}

	// 검색일자(최근일)
	memcpy(eu0607InRec1.edate,(LPCTSTR)m_pChartItem->GetEndDateRequested(),sizeof(eu0607InRec1.edate));
	if(nRqstCnt >= 10000)
	{
		AfxMessageBox("시장지표 최대 조회 갯수는 10000개 이하입니다");
		nRqstCnt = 9999;
	}

	// 검색건수
	m_lCntRequired = nRqstCnt; //조회할 갯수와 실제 받은 갯수를 비교하기 위해 저장해둔다.
	sprintf(Temp,"%04d",nRqstCnt);
	
	if(atoi(Temp) <= 0) return _T("");
	memcpy(eu0607InRec1.num,Temp,sizeof(eu0607InRec1.num));

	// 구분(0:조회/1:다음)
	if(bNext)
	{		
		chFlag = '1';
		memcpy(m_chkey, m_pChartItem->GetNextKey(TRUE), sizeof(m_chkey)); //기본 차트 데이터의 연속키를 가져온다.
	}
	else
	{
		chFlag = '0';
		memset(m_chkey, 0x00, sizeof(m_chkey));
	}
	memcpy(eu0607InRec1.nxtGb,&chFlag,sizeof(eu0607InRec1.nxtGb));

	// 조회키(다음/이전)
	memcpy(eu0607InRec1.nxtKey,m_chkey,sizeof(eu0607InRec1.nxtKey));

//@유진추가
	CString strFidInfo("0                                                                               ");
	CString strTrInfo;
	char* pszTrInfo = strTrInfo.GetBuffer(sizeof(eu0607InRec1));
	memcpy(pszTrInfo, (char*)&eu0607InRec1, sizeof(eu0607InRec1));
	strTrInfo.ReleaseBuffer();
	CString strRecordInfo("999999999999991               000001002003004005006007|2               000001002003;");
	CString strOutput;
	strOutput = strFidInfo + strTrInfo + strRecordInfo;
	return strOutput;
//@유진추가
//
//	return (char*)&eu0607InRec1;
}

//DEL CString CChartItemData::GetQueryData10451(BOOL bNext)
//DEL {
//DEL 	TR_10451_I stOutput;
//DEL 	char Temp[64];
//DEL 	char chFlag;
//DEL 
//DEL 	memset(&stOutput,0x20,sizeof(stOutput)); 
//DEL 
//DEL 	CString sWndID;
//DEL 	sWndID.Format("%d", m_pChartItem->GetDlgCtrlID());
//DEL 	memcpy(stOutput.wndID, sWndID, sWndID.GetLength());
//DEL 
//DEL 	memcpy( stOutput.pkName, (LPTSTR)(LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
//DEL 
//DEL 
//DEL 	//================================================================
//DEL 	// 허봉 실봉 구분.
//DEL 	stOutput.imgr_Gb[0] = '1';
//DEL 	if(m_pChartItem->m_CfgInfo.bImaginaryCandle == true)
//DEL 		stOutput.imgr_Gb[0] = '0'; //허봉(0) 실봉(1);
//DEL 
//DEL 	if(bNext)
//DEL 	{		
//DEL 		chFlag = '1';
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		chFlag = '0';
//DEL 	}
//DEL 
//DEL 	CString strPrimCode = m_pChartItem->GetPrimaryChartItemData()->GetCode();
//DEL 	if(strPrimCode.GetLength() == UPJONG_LENGTH)// 업종
//DEL 	{
//DEL 		stOutput.type[0] = '1';
//DEL 		memcpy(stOutput.shcode,(LPCTSTR)strPrimCode,sizeof(stOutput.shcode));
//DEL 		stOutput.shcode[4] = ' ';	
//DEL 		stOutput.shcode[5] = ' ';	
//DEL 		stOutput.shcode[6] = ' ';	
//DEL 	}
//DEL 	else if(strPrimCode.GetLength() == CODE_STOCK_LENGTH)							// 종목
//DEL 	{
//DEL 		stOutput.type[0] = '0';
//DEL 		memcpy(stOutput.shcode,(LPCTSTR)strPrimCode,sizeof(stOutput.shcode));
//DEL 		stOutput.shcode[6] = ' ';
//DEL 	}
//DEL 	memcpy(stOutput.edate,(LPCTSTR)m_pChartItem->GetEndDateRequested(),sizeof(stOutput.edate));	
//DEL 	sprintf(Temp,"%04d",GetCntForQuery(bNext));
//DEL 	
//DEL 	
//DEL 	if(atoi(Temp) <= 0) return _T("");
//DEL 	
//DEL 	memcpy(stOutput.num,Temp,sizeof(stOutput.num));
//DEL 	memcpy(stOutput.nxtGb,&chFlag,sizeof(stOutput.nxtGb));
//DEL 	memcpy(stOutput.nxtKey,m_chkey,sizeof(stOutput.nxtKey));
//DEL 
//DEL   
//DEL 	return (char*)&stOutput;	
//DEL }


// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
BOOL CChartItemData::MakeChartData03040(void* szData, long nLength, ST_SELCHART* lpSelChart) //해외종목
{
	D03040_O stOutput;
	memset(&stOutput, 0x20, sizeof(D03040_O));
	if(nLength>sizeof(stOutput))
	{			
		ASSERT(0);
		return FALSE;
	}
	D03040_O *pRecvData = NULL;
	pRecvData = (D03040_O *)szData;
	if(pRecvData == NULL) return FALSE;

	//시장지표때문에 chkeyOld 저장
	memset(m_chkeyOld,0x00,sizeof(m_chkeyOld));
	memcpy(m_chkeyOld,m_chkey,sizeof(pRecvData->nkey));

	memset(m_chkey,0x00,sizeof(m_chkey));
	memcpy(m_chkey,pRecvData->nkey,sizeof(pRecvData->nkey));

	// 분차트 마지막 키인 경우 - ojtaso (20071226)
	m_pChartItem->EnableRequestMoreButton(TRUE);

	//빌어 05.09.09 ================================================
	CString sKey;
	sKey.Format("%21.21s", pRecvData->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
	if( atoi(sKey)  == 0)
		memset(m_chkey,0x00,sizeof(m_chkey));
	//===============================================================
	// 분차트 마지막 키인 경우 - ojtaso (20071226)
	if(sKey.GetLength() > 8)
	{
		sKey.Delete(0, 8);
		if( atoi(sKey)  == 0)
			memset(m_chkey,0x00,sizeof(m_chkey));
	}

	// 분차트 마지막 키인 경우 - ojtaso (20071226)
	if(atoi(sKey) == 0)
		m_pChartItem->EnableRequestMoreButton(FALSE);

	if(lpSelChart)
		// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
		lpSelChart->m_strKey.Format("%18.18s", m_chkey);

	if(!m_pSt03040Data)
	{
		m_pSt03040Data = new S03040_CHART;
	}
	m_strCode = m_strCodeForRequest;
	
	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp,pRecvData->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp,pRecvData->tmp);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pRecvData->tmp[0] - &pRecvData->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pRecvData->tmp[0] - &pRecvData->name[0]);
	memcpy(m_pSt03040Data,&pRecvData->tmp[0],lPosForChart);

	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp,pRecvData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp,pRecvData->chart[0].date);
	m_lStartDate = atol(szTemp);
	
	return TRUE;
}

long CChartItemData::GetMaximumRequestCnt()
{
	if ((m_strTRCode == QUERY_STRFOFOREIGN) ||
		(m_strTRCode == QUERY_STRFOFOREIGN_HOGA))
	{
		return (long)DEFAULTCNTATONCE;
	}
	
	return DEFAULTCNTATONCE;
}

long CChartItemData::GetCntForQuery(BOOL bNext)
{
	//-->@Solomon:100311-Next조회이더라도 기존 요청한 갯수만큰 요청하도록 수정.
	bNext = FALSE;
	//<--

	if(m_pChartItem->IsPrimaryChartItemData(this))	// 주 데이터의 경우
	{
// gm0604 2004/08/20 Due to problem that Cnt become zero
		if(!bNext)
		{
			m_lCntRequired = m_pChartItem->GetCntForRequest();
			if(m_lCntRequired<5)
			{
				m_lCntRequired = 5;
			}
		}
		else
		{
			m_lCntRequired = GetMaximumRequestCnt();
		}		
	}
	else		// 보조데이터의 경우..
	{
		if(!bNext)	// 처음의 경우 요구되어질 총 데이터를 세팅한다.
		{
			m_lCntRequiredByPrim = m_pChartItem->GetAllCnt();
		}
		// 최대요청될수있는수량보다 전체데이터수이 작은경우 전체데이터수로 데이터수를 요청한다.
/*		if(GetMaximumRequestCnt()>m_lCntRequiredByPrim) 
		{
			m_lCntRequired = m_lCntRequiredByPrim;
		}
		else
		{
			m_lCntRequired = GetMaximumRequestCnt();
		}		
*/
		m_lCntRequired = m_lCntRequiredByPrim;
	}

	return m_lCntRequired;	
}

BOOL CChartItemData::ProcRequestCntAfterQuery(long lCntReceived)
{
//	if(m_lCntRequired<lCntReceived)
//	{
//		if (m_strCodeForRequest.Find("선물순매수") < 0 && m_strCodeForRequest.Find("콜옵션순매수") < 0 && m_strCodeForRequest.Find("풋옵션순매수") < 0 && m_strCodeForRequest.Find("기관순매수누적") < 0)
//		{
//	#ifdef _DEBUG
//			CString strMsg;
//			strMsg.Format("서버와 데이터수가 맞지 않습니다...[TR:%s][Code:%s]\
//						  요구한 데이터수:%d, 받은 데이터수:%d",\
//						  m_strTRCode,m_strCode,m_lCntRequired,lCntReceived);
//			AfxMessageBox(strMsg);
//	#endif
//			return FALSE;
//		}
//	}
	if(m_pChartItem->IsPrimaryChartItemData(this))	// 주 데이터의 경우
	{
	}
	else
	{
		m_lCntRequiredByPrim -= lCntReceived;
////gm0604 2004.8.2
		if(!m_lCntRequiredByPrim)
		{	
			m_pChartItem->SetNext(FALSE);
			
			m_bIsFilled = TRUE;
#ifdef _DEBUG
			OutputDebugString("CChartItemData::ProcRequest .... m_bIsFillded = TRUE");
#endif
			return TRUE;
		}
////
	}
	if(!lCntReceived)
	{
		m_pChartItem->SetNext(FALSE);

		//===
		SetBNextMarketData(FALSE);
		return TRUE;
	}	
	else
	{
		m_lCnt = lCntReceived;
	}
	return TRUE;
}

// 해외지수
// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
CString CChartItemData::GetQueryData03040(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	D03040_I stOutput;
	char Temp[64];
	char chFlag;

	memset(&stOutput,0x00,sizeof(stOutput));// stOutput.chNull = NULL;
	memset(stOutput.codegb, 0x20, sizeof(stOutput.codegb));
	memset(stOutput.jmcode, 0x20, sizeof(stOutput.jmcode));
	if(lpSelChart)
		memcpy(stOutput.jmcode,(LPCTSTR)lpSelChart->m_strItemCode,lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(stOutput.jmcode,(LPCTSTR)m_strCodeForRequest,m_strCodeForRequest.GetLength());
	//int nCodeLength = m_strCodeForRequest.GetLength();
	//stOutput.jmcode[nCodeLength] = ' ';	
	memcpy(stOutput.date,m_pChartItem->GetEndDateRequested(),sizeof(stOutput.date));

/*
	if(lpSelChart)
		sprintf(Temp,"%04d",lpSelChart->m_lCount);
	else
*/
		sprintf(Temp,"%04d",GetCntForQuery(bNext));
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}	
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	
	memcpy(stOutput.num,Temp,sizeof(stOutput.num));
	
//	if(chTypeRequested == SEC10_DATA_CHART)
//		memcpy(stOutput.unit,"10s", 3);
//	else if (chTypeRequested == SEC30_DATA_CHART)
//		memcpy(stOutput.unit,"30s", 3);
//	else
	{
		if(lpSelChart)
			memcpy(stOutput.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
		else
			memcpy(stOutput.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	}

	memcpy(stOutput.button,&chTypeRequested,sizeof(stOutput.button));
	memcpy(stOutput.nflag,&chFlag,sizeof(stOutput.nflag));
	// 2005. 09. 21 by sy, nam ==============================================================
	// memset 0x00으로 하니 '\0'로 인해 뒤 데이터가 잘려서 
	// 실봉 허봉 이 구분이 제대로 안 되는 경우가 생겼다.
	if(chFlag == '1')//다음 조회일 경우
	{
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		//if(atoi(sKey) > 0)
		if(sKey.GetLength()>0)
			memcpy(stOutput.nkey,m_chkey,sizeof(stOutput.nkey));
		else
			memset(stOutput.nkey,0x20,sizeof(stOutput.nkey));
	}
	else //단순 조회 일 경우.
		memset(stOutput.nkey,0x20,sizeof(stOutput.nkey));

	//memcpy(stOutput.nkey,m_chkey,sizeof(stOutput.nkey)); <== original	
	//=======================================================================================
	
//	// 당일조회
//	char chOpt = '0';
//	if(chTypeRequested == DAY_DATA_CHART || chTypeRequested == WEEK_DATA_CHART || chTypeRequested == MONTH_DATA_CHART)
//		chOpt = '0';
//	else
//		chOpt = '1';
//
//	memcpy(stOutput.nOpt, &chOpt, sizeof(stOutput.nOpt));	// 당일조회여부

	
	return (char*)&stOutput;
}


CString CChartItemData::MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled, BOOL bOverLapping)
{
	//2005. 05. 18 add by sy, nam =============================================================
	//중첩 모드인경우 패킷 내용을 다르게 하기 위해 다른 함수를 호출한다.	
	if(bChartFilled && bOverLapping)
		return OverLapping_MakePacketOption(lpPacketName,lPacketSize,lpPacketType,bUseInReal, nCntOverLap, bChartFilled, bOverLapping);
	//=========================================================================================

	char szTemp[64];
	CString strPacket;

	strPacket = "1;";
	strPacket += lpPacketName; // 사용할 Packet명
	strPacket += ";;";
	sprintf(szTemp,"%d",lPacketSize);
	strPacket += szTemp;
	strPacket += ";";
	strPacket += lpPacketType;
	strPacket += ";";
	if(bUseInReal)
	{
		strPacket += "유;\r\n";
	}
	else
	{
		strPacket += "무;\r\n";
	}
//	OutputDebugString(strPacket);
	return strPacket;
}



CString CChartItemData::OverLapping_MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal, int nCntOverLap, BOOL bChartFilled, BOOL bOverLapping )
{
	char szTemp[64];
	CString strPacket;
	CString strOverLapCnt;
	strOverLapCnt.Format("ov%d", nCntOverLap);

	strPacket += strOverLapCnt;
	strPacket = "1;";
	
	strPacket += strOverLapCnt;
	strPacket += lpPacketName; // 사용할 Packet명
	strPacket += ";;";
	sprintf(szTemp,"%d",lPacketSize);

	strPacket += strOverLapCnt;
	strPacket += szTemp;
	strPacket += ";";

	strPacket += strOverLapCnt;
	strPacket += lpPacketType;
	strPacket += ";";
	if(bUseInReal)
	{
		strPacket += strOverLapCnt;
		strPacket += "유;\r\n";
	}
	else
	{
		strPacket += strOverLapCnt;
		strPacket += "무;\r\n";
	}
	return strPacket;
}

CString CChartItemData::MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle /*=0*/,
									COLORREF clrGrp1 /*= RGB(218,46,46)*/, COLORREF clrGrp2 /*= RGB(218,46,46)*/,
									COLORREF clrGrp3 /*= RGB(46,46,218)*/, COLORREF clrGrp4 /*= RGB(46,46,218)*/,
									COLORREF clrGrp5 /*= RGB(218,46,46)*/, COLORREF clrGrp6 /*= RGB(218,46,46)*/,
									BOOL bUpFilled /*= 0*/, BOOL bDownFilled /*= 1*/,
									long lUpLineWidth /*= 1*/, long lDownLineWidth /*= 1*/, BOOL bHideGrp /*= 0*/)
{
	CString strChartType = lpChartType;
	CString tempStr;
	CString strGraphOption("Graph;\r\n");
	strGraphOption += lpChartType;
	strGraphOption += ";";
	strGraphOption +=lpChartName;
	strGraphOption += ";";
	LONG lSize = rArrayPacketList.GetSize();
	for(long lIndex=0;lIndex<lSize;lIndex++)
	{
		if(lIndex)	strGraphOption += ",";
		strGraphOption += rArrayPacketList.GetAt(lIndex);		
	}
	if(strChartType==g_strChartType[0])
	{
		strGraphOption += ";0;0;0;0;1;0";
	}
	strGraphOption += ";\r\n";

	// "그래프타입;그래프스타일;그리는스타일;이름;title;숨김;색1;색2;색3;색4;색5;색6;
	//  상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n"
	
	if(strChartType==g_strChartType[0])
	{
		strGraphOption += "0;";
	}
	else if(strChartType==g_strChartType[1])
	{
		strGraphOption += "1;";
	}
	else if(strChartType==g_strChartType[2])
	{
		strGraphOption += "2;";
	}
	else
	{
		ASSERT(0); // Error !! 
	}
	tempStr.Format("%d;",lChartStyle);
	strGraphOption += tempStr;
	strGraphOption += "0;"; 
	strGraphOption += lpChartName;
	strGraphOption += ";";
	strGraphOption += lpChartTitle;
	tempStr.Format(";0;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d;%d;%d;%d;%d;\r\n",
		GetRValue(clrGrp1),GetGValue(clrGrp1), GetBValue(clrGrp1),
		GetRValue(clrGrp2),GetGValue(clrGrp2), GetBValue(clrGrp2),
		GetRValue(clrGrp3),GetGValue(clrGrp3), GetBValue(clrGrp3),
		GetRValue(clrGrp4),GetGValue(clrGrp4), GetBValue(clrGrp4),
		GetRValue(clrGrp5),GetGValue(clrGrp5), GetBValue(clrGrp5),
		GetRValue(clrGrp6),GetGValue(clrGrp6), GetBValue(clrGrp6),
		bUpFilled,bDownFilled,
		lUpLineWidth,lDownLineWidth,bHideGrp);
	strGraphOption += tempStr;
	
	strGraphOption += "GraphEnd;\r\n";	
	return strGraphOption;
}

long CChartItemData::GetData()
{
	if (m_strTRCode == QUERY_STRFOFOREIGN)
		return (long)m_pEu0602_ChartData;
//	else if(m_strTRCode == QUERY_STRFOFOREIGN_HOGA)
//		return (long)m_pEu0603_ChartData;

	return 0L;
}

void CChartItemData::RemoveRealItem()
{
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveRealItem::Begin", _T("멀티차트 종료"), 13);
#endif
	CString strRealItemRegistered;
	int nSize = m_strArrayRealItemRegistered.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		strRealItemRegistered = m_strArrayRealItemRegistered.GetAt(nPos);
		m_pChartItem->RemoveRealItem(strRealItemRegistered);
	}
	m_strArrayRealItemRegistered.RemoveAll();
#ifdef _WRITELOG
	m_pChartItem->WriteToStringLog("CChartItemData::RemoveRealItem::End", _T("멀티차트 종료"), 13);
#endif
}

void CChartItemData::SetChartNumericValue(int nValue)
{
/*@Solomon	20100125	사용하지 않음.
	if(m_strTRCode==QUERY_STRFOFOREIGN)
	{
		CString strCode = m_pChartItem->GetSelectedItemCode();
		if(m_pChartItem->m_lpSelChart && m_pChartItem->m_lpSelChart->m_strItemCode.IsEmpty() )	strCode = CStdDialog::GetRealItemCode( m_pChartItem->m_lpSelChart->m_strItemCode);
		else			strCode = CStdDialog::GetRealItemCode( m_strCodeForRequest );
		m_strPosition = m_pChartItem->GetCodePrecisionFormat(strCode);

		if(nValue == PERCENT_TYPE)
		{
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[7]);
		}
		else if(nValue == WON_TYPE)
		{
			m_pChartItem->ChangePacketType(g_strPacketName[1],m_strPosition);
			m_pChartItem->ChangePacketType(g_strPacketName[2],m_strPosition);
			m_pChartItem->ChangePacketType(g_strPacketName[3],m_strPosition);
			m_pChartItem->ChangePacketType(g_strPacketName[4],m_strPosition);
		}
		else if(nValue == DOLLAR_TYPE)
		{
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[6]);
		}
	}
*/
}

void CChartItemData::AddRealItem()
{
	BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
	
	if(m_strTRCode == QUERY_STRFOFOREIGN)
	{
			CString strRealItem;	
			if(IsPrimaryChartItemData)
			{
				// 자료일자
				strRealItem = g_strPacketName[0];
				strRealItem += '.';
				strRealItem += "S31.time.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);
				m_strArrayRealItemRegistered.Add(strRealItem);

				// 거래량
				strRealItem = g_strPacketName[5];
				strRealItem += '.';
				strRealItem += "S31.volume.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);		
				m_strArrayRealItemRegistered.Add(strRealItem);

				//if(m_pChartItem->GetUseRateValue())
				if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
				{
					// 종가
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "S31.chgrate.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
				else
				{
					// 종가
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "S31.price.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);

/*
					if(m_pChartItem->GetType() == DAY_DATA_CHART)
					{	
						// 시가
						strRealItem = "시가GuideLine";
						strRealItem += '.';
						strRealItem += "58.23.";
						strRealItem += m_strCode;
						strRealItem += ';';
						m_pChartItem->AddRealItem(strRealItem);
						m_strArrayRealItemRegistered.Add(strRealItem);
						// 고가 
						strRealItem = "고가GuideLine";
						strRealItem += '.';
						strRealItem += "58.7.";
						strRealItem += m_strCode;
						strRealItem += ';';
						m_pChartItem->AddRealItem(strRealItem);
						m_strArrayRealItemRegistered.Add(strRealItem);
						// 저가
						strRealItem = "저가GuideLine";
						strRealItem += '.';
						strRealItem += "58.8.";
						strRealItem += m_strCode;
						strRealItem += ';';
						m_pChartItem->AddRealItem(strRealItem);
						m_strArrayRealItemRegistered.Add(strRealItem);
					}*/


				}
			}
	}		
	else if(m_strTRCode == QUERY_STRUPJONG)
	{
			CString strRealItem;
			if(IsPrimaryChartItemData)
			{
				// 자료일자
				strRealItem = g_strPacketName[0];
				strRealItem += '.';
				strRealItem += "JS0.time.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);
				m_strArrayRealItemRegistered.Add(strRealItem);
				// 거래량
				strRealItem = g_strPacketName[5];
				strRealItem += '.';
				strRealItem += "JS0.volume.";
				strRealItem += m_strCode;
				strRealItem += ';';
				m_pChartItem->AddRealItem(strRealItem);		
				m_strArrayRealItemRegistered.Add(strRealItem);
				if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
				{
					// 종가
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "JS0.chgrate.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
				else
				{
					// 종가
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "JS0.jisu.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
			}
			else
			{
				//if(m_pChartItem->GetUseRateValue())
				if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)
				{
					// 종가
					strRealItem = GetChartItemDataTitle();
					strRealItem += '.';
					strRealItem += "JS0.chgrate.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
				else
				{
					// 종가
					strRealItem = GetChartItemDataTitle();
					strRealItem += '.';
					strRealItem += "JS0.jisu.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}

			}
	}		
	else if(m_strTRCode == QUERY_STRFUTOPT)
	{
			CString strRealItem;
			if(IsPrimaryChartItemData)
			{
				CString strCode = GetCode();
				if(strCode[0] == '1' || strCode[0] == '4')	// Future
				{
					// 자료일자
					strRealItem = g_strPacketName[0];
					strRealItem += '.';
					strRealItem += "SC0.time.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
					// 거래량
					strRealItem = g_strPacketName[5];
					strRealItem += '.';
					strRealItem += "SC0.volume.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);		
					m_strArrayRealItemRegistered.Add(strRealItem);
					// 종가
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "SC0.price.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
				else
				{
					// 자료일자
					strRealItem = g_strPacketName[0];
					strRealItem += '.';
					strRealItem += "OC0.time.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
					// 거래량
					strRealItem = g_strPacketName[5];
					strRealItem += '.';
					strRealItem += "OC0.volume.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);		
					m_strArrayRealItemRegistered.Add(strRealItem);
					// 종가
					strRealItem = g_strPacketName[4];
					strRealItem += '.';
					strRealItem += "OC0.price.";
					strRealItem += m_strCode;
					strRealItem += ';';
					m_pChartItem->AddRealItem(strRealItem);
					m_strArrayRealItemRegistered.Add(strRealItem);
				}
			}
//			CString strRealItem;
//			if(IsPrimaryChartItemData)
//			{
//				//=============================================================
//				// 선물, 옵션을  구분한다.
//				CString sPacketName = _T("");
//				m_strCode.Remove(' ');
//
//				DWORD dw = m_pChartItem->GetCodeType_By_DBMgr(m_strCode);					
//				switch(dw)
//				{ 	
//					case CODE_QFUTURE	:
//					case CODE_FSPREAD	:
//					case CODE_QFUTURES	:
//					case CODE_FUTURE	: //선물 				
//						sPacketName = "65";
//						break;
//					case CODE_OPTION	: //옵션	
//					case CODE_QOPTION	:						
//					case CODE_QOPTIONS	: //코스닥 옵션 
//					case CODE_JOPTION	: //주식 옵션
//						sPacketName = "67";									
//						break;
//					case CODE_FSTAR		:
//					case CODE_FSSPREAD	:
//						sPacketName = "90";
//						break;
//					default:
//						return;
//						break;
//				}
//				//=============================================================
//				// 자료일자
//				strRealItem = g_strPacketName[0];
//				strRealItem += '.';
//				strRealItem += sPacketName + ".2.";
//				strRealItem += m_strCode;
//				strRealItem += ';';
//				m_pChartItem->AddRealItem(strRealItem);
//				m_strArrayRealItemRegistered.Add(strRealItem);
//				// 거래량
//				strRealItem = g_strPacketName[5];
//				strRealItem += '.';
//				strRealItem += sPacketName + ".10.";
//				strRealItem += m_strCode;
//				strRealItem += ';';
//				m_pChartItem->AddRealItem(strRealItem);		
//				m_strArrayRealItemRegistered.Add(strRealItem);
//				// 종가
//				strRealItem = g_strPacketName[4];
//				strRealItem += '.';
//				strRealItem += sPacketName + ".3.";
//				strRealItem += m_strCode;
//				strRealItem += ';';
//				m_pChartItem->AddRealItem(strRealItem);
//				m_strArrayRealItemRegistered.Add(strRealItem);
//			}
	}
//	else if(m_strTRCode == QUERY_STRCOMMODITY)		// 상품선물옵션차트 - ojtaso (20071001)
//	{
//		CString strRealItem;
//		if(IsPrimaryChartItemData)
//		{
//			CString strCode = GetCode();
//			// 자료일자
//			strRealItem = g_strPacketName[0];
//			strRealItem += '.';
//			strRealItem += "OC0.time.";
//			strRealItem += m_strCode;
//			strRealItem += ';';
//			m_pChartItem->AddRealItem(strRealItem);
//			m_strArrayRealItemRegistered.Add(strRealItem);
//			// 거래량
//			strRealItem = g_strPacketName[5];
//			strRealItem += '.';
//			strRealItem += "OC0.volume.";
//			strRealItem += m_strCode;
//			strRealItem += ';';
//			m_pChartItem->AddRealItem(strRealItem);		
//			m_strArrayRealItemRegistered.Add(strRealItem);
//			// 종가
//			strRealItem = g_strPacketName[4];
//			strRealItem += '.';
//			strRealItem += "OC0.price.";
//			strRealItem += m_strCode;
//			strRealItem += ';';
//			m_pChartItem->AddRealItem(strRealItem);
//			m_strArrayRealItemRegistered.Add(strRealItem);
//		}
//	}
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
BOOL CChartItemData::SetLoadedPacketData(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart)
{
	return m_pChartItem->SetPacketDataRQ(lpszRQ, GetPacketName(),GetData(),GetDataSize(),FALSE);
}

BOOL CChartItemData::SetPacketData(BOOL bReset)
{
	BOOL bRetValue = FALSE;
	if(!m_pChartItem->IsPrimaryChartItemData(this)&&m_lCntRequiredByPrim)
	{		
		if(m_lCntRequired>m_lCntTemp)
		{
			m_bIsFilled = TRUE;
			m_pChartItem->SetNext(FALSE);
			bRetValue = m_pChartItem->SetPacketData(GetPacketName(),GetData(),GetDataSize(),bReset);
			long lSize = GetCntChartItemDataTitle();
			if(m_lStartDate)
			{
				for(long lPos=0;lPos<lSize;lPos++)
				{
					m_pChartItem->SetStartPeriodOfPacket(GetChartItemDataTitle(lPos),m_lStartDate,TRUE);
				}			
			}
			goto ExitLoop;
		}
		else
		{
			m_bIsFilled = FALSE;
// --> [Edit]  강지원 2008/12/12	( 데이터 추가조회 플래그 설정 )
//			m_pChartItem->SetNext(TRUE);
// <-- [Edit]  강지원 2008/12/12
		}
	}
	else
	{
		m_bIsFilled = TRUE;
// --> [Edit]  강지원 2008/12/12	( 데이터 추가조회 플래그 설정 )
//		m_pChartItem->SetNext(FALSE);
// <-- [Edit]  강지원 2008/12/12
	}

	if(!ProcRequestCntAfterQuery(m_lCntTemp))
	{
		bRetValue = FALSE;
		goto ExitLoop;
	}


	bRetValue =  m_pChartItem->SetPacketData(GetPacketName(),GetData(),GetDataSize(),bReset);	


ExitLoop:
	RemoveData();
	return bRetValue;
}

BOOL CChartItemData::SetPacketDataInAdditionPacket(BOOL bReset)
{
	BOOL bRetValue = FALSE;
	if(!m_pChartItem->IsPrimaryChartItemData(this)&&m_lCntRequiredByPrim)
	{		
		if(m_lCntRequired>m_lCntTemp)
		{
			m_bIsFilled = TRUE;
#ifdef _DEBUG
			OutputDebugString("CChartItemData::SetPacketDataInAdditionPacket .... m_bIsFillded = TRUE");
#endif 
			m_pChartItem->SetNext(FALSE);
			bRetValue = m_pChartItem->SetPacketDataInAdditionPacket(GetPacketName(),GetData(),GetDataSize(),bReset);
			long lSize = GetCntChartItemDataTitle();
			if(m_lStartDate)
			{
				for(long lPos=0;lPos<lSize;lPos++)
				{
					m_pChartItem->SetStartPeriodOfPacket(GetChartItemDataTitle(lPos),m_lStartDate,TRUE);
				}			
			}
			goto ExitLoop;
		}
		else
		{
			m_bIsFilled = FALSE;
#ifdef _DEBUG
			OutputDebugString("CChartItemData::SetPacketData .... m_bIsFillded = FALSE#2");
#endif 

			m_pChartItem->SetNext(TRUE);
		}
	}
	else
	{
		m_bIsFilled = TRUE;
#ifdef _DEBUG
			OutputDebugString("CChartItemData::SetPacketData .... m_bIsFillded = TRUE#3");
#endif 

		m_pChartItem->SetNext(FALSE);
	}

//	m_bIsFilled = TRUE;	

	if(!ProcRequestCntAfterQuery(m_lCntTemp))
	{
		bRetValue = FALSE;
		goto ExitLoop;
	}


	//bRetValue =  m_pChartItem->SetPacketDataInAdditionPacket(GetPacketName(),GetData(),GetDataSize(),bReset);	
	bRetValue =  m_pChartItem->SetPacketDataInAdditionPacket(GetPacketName(),GetData(),GetDataSize(),!m_bNextMarketData);
	
ExitLoop:
	RemoveData();
	
	SetBNextMarketData(FALSE);

	return bRetValue;
}





BOOL CChartItemData::IsFilled()
{
	return m_bIsFilled;
}

long CChartItemData::GetCntChartItemDataTitle()
{
	return m_strArrayGraphTitle.GetSize();
}

CString CChartItemData::GetChartItemDataTitle(long nIndex /*= 0*/)
{
	if(m_strArrayGraphTitle.GetSize()>nIndex)
	{
		return m_strArrayGraphTitle.GetAt(nIndex);
	}
	return "";
}

void CChartItemData::ClearData() 
{ 
	m_bIsFilled = FALSE;
	
#ifdef _DEBUG
			OutputDebugString("CChartItemData::ClearData .... m_bIsFillded = FALSE");
#endif 

}

// ELW INPUT ====================================================================
//    char    Scode    [15];       /* 종목코드                             */
//    char    Sdate    [ 8];       /* 검색 시작일                          */
//    char    Smaxbong [ 4];       /* 검색할 데이터 갯수                   */
//    char    Sunit    [ 3];       /* Tick, Sec(10s, 30s), Min, Day Number */
//    char    Sbtngb   [ 1];       /* 틱(0)/분(1)/일(2)/주(3)/월(4)        */
//    char    Snextgb  [ 1];       /* 다음(1) / 조회(0) 구분자             */
//    char    Snextkey [14];       /* 다음 / 이전 Fetch Key                */
//================================================================================
// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
//CString CChartItemData::GetQueryDataELW(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
//{
////@유진삭제
////	S01007_I stOutput;
////@유진삭제
////@유진추가
//	p0605InBlock stOutput;
////@유진추가
//
//	char Temp[64];
//	char chFlag;
//
//	memset(&stOutput,0x00,sizeof(stOutput)); 	stOutput.chNull = NULL;
//	memset(stOutput.Scode, 0x20, sizeof(stOutput.Scode));
//	if(lpSelChart)
//		memcpy(stOutput.Scode,(LPCTSTR)lpSelChart->m_strItemCode,lpSelChart->m_strItemCode.GetLength());
//	else
//		memcpy(stOutput.Scode,(LPCTSTR)m_strCodeForRequest,m_strCodeForRequest.GetLength());
//	//	memcpy(stOutput.shcode,(LPCTSTR)m_strCodeForRequest,sizeof(stOutput.shcode));
//	//	stOutput.shcode[7] = ' ';	
//	memcpy(stOutput.Sdate, m_pChartItem->GetEndDateRequested(), sizeof(stOutput.Sdate));
//
///*
//	if(lpSelChart)
//		sprintf(Temp,"%04d",lpSelChart->m_lCount);
//	else
//*/
//		sprintf(Temp,"%04d",GetCntForQuery(bNext));
//	if(bNext)
//	{		
//		chFlag = '1';
//	}
//	else
//	{
//		chFlag = '0';
//	}	
//	char chUseRevised = '0';
//	if(m_pChartItem->GetUseRevisedValue())
//	{
//		chUseRevised = '1';
//	}
//	//05.06.22 add by sy, nam =====================================================
//	//	if(m_pChartItem->GetUseRateValue())
//	//	{
//	//		chUseRateValue = '1';
//	//	}	
//	char chUseRateValue = '0';
//	int nNumericValueType = lpSelChart ? lpSelChart->m_nChartNumericValueType : m_pChartItem->GetChart_NumericValueType();
//	if(nNumericValueType == WON_TYPE)	
//		chUseRateValue = '0';
//	else if(nNumericValueType == PERCENT_TYPE)	
//		chUseRateValue = '1';
//	else if(nNumericValueType == DOLLAR_TYPE)	
//		chUseRateValue = '2';
//	//===============================================================================
//
//	char chUseFackedCandle = '1';
//	if(m_pChartItem->GetBNormalizedMinuteTimeRule())// TRUE이면 허봉 
//		chUseFackedCandle = '0'; // '0':허수봉 '1':실봉           
//
//
//
//	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
//
//
//
//	memcpy(stOutput.Smaxbong, Temp, sizeof(stOutput.Smaxbong));
//
//	//==================================================================================================================================
//	// 틱/분/일... 등의 구분값이 별도로 가기 때문에.. tick/min이라는 요소를 없애고
//	// unit에 틱/분/일 의 단위를 입력한다.
//	// 2005. 05. 23 by sy, nam
//	//==================================================================================================================================
//	if(chTypeRequested == SEC10_DATA_CHART)
//		memcpy(stOutput.Sunit, "10s", 3);
//	else if (chTypeRequested == SEC30_DATA_CHART)
//		memcpy(stOutput.Sunit, "30s", 3);
//	else
//	{
//		if(lpSelChart)
//			memcpy(stOutput.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
//		else
//			memcpy(stOutput.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());
//	}
//
//	//==================================================================================================================================
//	memcpy(stOutput.Sbtngb, &chTypeRequested, sizeof(stOutput.Sbtngb));
//	//	memcpy(stOutput.modGb,&chUseRevised,sizeof(stOutput.modGb));
//	memcpy(stOutput.Snextgb, &chFlag, sizeof(stOutput.Snextgb));
//
//	// 2005. 09. 21 by sy, nam ==============================================================
//	// memset 0x00으로 하니 '\0'로 인해 뒤 데이터가 잘려서 
//	// 실봉 허봉 이 구분이 제대로 안 되는 경우가 생겼다.
//	if(chFlag == '1')//다음 조회일 경우
//	{
//		CString sKey;
//		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
//		//if(atoi(sKey) > 0)
//		if(sKey.GetLength()>0)
//			memcpy(stOutput.Snextkey, m_chkey, sizeof(stOutput.Snextkey));
//		else
//			memset(stOutput.Snextkey, 0x20, sizeof(stOutput.Snextkey));
//	}
//	else //단순 조회 일 경우.
//		memset(stOutput.Snextkey, 0x20, sizeof(stOutput.Snextkey));
//
//	//memcpy(stOutput.nkey,m_chkey,sizeof(stOutput.nkey)); <== original
//	//=======================================================================================
//
//
//
//	//	memcpy(stOutput.nDataGubun,&chUseRateValue,sizeof(stOutput.nDataGubun));
//	//	memcpy(stOutput.ImgRgb,&chUseFackedCandle,sizeof(stOutput.ImgRgb));
//	//==================================================================================================================================
//	// 복기 관련 정보를 넣는다.
//	// 2005. 08. 11 by sy, nam
//	//==================================================================================================================================
//	//	CString strReplayFlag = m_pChartItem->GetReplayFlag();
//	//	CString strBeforeDay  = m_pChartItem->GetReplayDayBefore();
//	//	CString strReplayTime = m_pChartItem->GetReplayStartTime();
//	//
//	//	memset(stOutput.Itime	 ,  0x00			,   sizeof(stOutput.Itime)		);
//	//
//	//	memcpy(stOutput.Irecordgb, 	strReplayFlag	, 	sizeof(stOutput.Irecordgb)	);/* 복기 구분값(1)		*///<========2005.08.11
//	//
//	//	memcpy(stOutput.nDayBefore, strBeforeDay	,	sizeof(stOutput.nDayBefore)	);/* 복시 시작 n일전	*/
//	//
//	//	memcpy(stOutput.Itime	 , 	strReplayTime	,	strReplayTime.GetLength()	);/* 복기시간 (hhmmss)	*///<========2005.08.11
//
////@유진추가
//	CString strFidInfo("0                                                                               ");
//	CString strTrInfo;
//	char* pszTrInfo = strTrInfo.GetBuffer(sizeof(stOutput));
//	memcpy(pszTrInfo, (char*)&stOutput, sizeof(stOutput));
//	strTrInfo.ReleaseBuffer();
//	CString strRecordInfo("1               000002003004005009010012013|2               000001002003004005007;");
//	CString strOutput;
//	strOutput = strFidInfo + strTrInfo + strRecordInfo;
//	return strOutput;
////@유진추가
//
//	return (char*)&stOutput;
//}

//OUTPUT ELW
BOOL CChartItemData::MakeChartDataELW(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	S01007_O stOutput;
	memset(&stOutput, 0x20, sizeof(S01007_O));
	if(nLength>sizeof(stOutput))
	{			
		ASSERT(0);
		return FALSE;
	}
	S01007_O *pRecvData = NULL;
	pRecvData = (S01007_O *)szData;
	if(pRecvData == NULL) return FALSE;

	//시장지표때문에 chkeyOld 저장
	memset(m_chkeyOld,0x00,sizeof(m_chkeyOld));
	memcpy(m_chkeyOld,m_chkey,sizeof(pRecvData->nkey));

	memset(m_chkey,0x00,sizeof(m_chkey));
	memcpy(m_chkey,pRecvData->nkey,sizeof(pRecvData->nkey));

	// 분차트 마지막 키인 경우 - ojtaso (20071226)
	m_pChartItem->EnableRequestMoreButton(TRUE);

	//빌어 05.09.09 ================================================
	CString sKey;
	sKey.Format("%21.21s", pRecvData->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
	if( atoi(sKey)  == 0)
		memset(m_chkey,0x00,sizeof(m_chkey));
	//===============================================================
	// 분차트 마지막 키인 경우 - ojtaso (20071226)
	if(sKey.GetLength() > 8)
	{
		sKey.Delete(0, 8);
		if( atoi(sKey)  == 0)
			memset(m_chkey,0x00,sizeof(m_chkey));
	}

	// 분차트 마지막 키인 경우 - ojtaso (20071226)
	if(atoi(sKey) == 0)
		m_pChartItem->EnableRequestMoreButton(FALSE);

	if(lpSelChart)
		// (2008/9/28 - Seung-Won, Bae) Use Format for Non Null-Terminated.
		lpSelChart->m_strKey.Format("%18.18s", m_chkey);

	if(!m_pSt01007Data)
	{
		m_pSt01007Data = new S01007_CHART;
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp,pRecvData->name);
	m_strName = szTemp;
	m_strName.TrimRight();

	MEMCPYWITHNULL(szTemp,pRecvData->tmp);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;

	long lPosForChart = 0;
	if(nLength < (&pRecvData->tmp[0] - &pRecvData->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pRecvData->tmp[0] - &pRecvData->name[0]);
	memcpy(m_pSt01007Data,&pRecvData->tmp[0],lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp,pRecvData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp,pRecvData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}

// 시장이 바뀌면 PacketName도 바꾼다 : 복수종목 - ojtaso (20070711)
void CChartItemData::ChangePacketNameArray(LPCTSTR lpszTRCode)
{
	m_strArrayPacketName.RemoveAll();

	m_strArrayPacketName.Add(g_strPacketName[0]);
	m_strArrayPacketName.Add(g_strPacketName[1]);
	m_strArrayPacketName.Add(g_strPacketName[2]);
	m_strArrayPacketName.Add(g_strPacketName[3]);
	m_strArrayPacketName.Add(g_strPacketName[4]);
	m_strArrayPacketName.Add(g_strPacketName[5]);
	
	if(!::lstrcmp(lpszTRCode, QUERY_STRFOFOREIGN))
		m_strArrayPacketName.Add(g_strPacketName[7]);
}

void CChartItemData::OnSelectChartItem(ST_SELCHART* lpSelChart)
{
	SetTRCode(lpSelChart->m_strChartGubun);
	m_strCodeForRequest = lpSelChart->m_strItemCode;
//	memcpy(m_chkey, (LPCTSTR)lpSelChart->m_strKey, lpSelChart->m_strKey.GetLength());
}

void CChartItemData::GetOHLValue(CString &strOpen, CString &strHigh, CString &strLow)
{
	strOpen = m_strOpen;			//	시가
	strHigh = m_strHigh;			//	고가
	strLow = m_strLow;			//	저가
}

CString CChartItemData::GetReciveValue(short nType)
{
	CString strTemp = "";
	switch ( nType )
	{
	case 0:
		return m_strName;
	case 1:
		return m_strPrice;
	case 2:
		return m_strOpen;
	case 3:
		return m_strHigh;
	case 4:
		return m_strLow;
	case 5:
		return m_strSign;
	case 6:
		return m_strChange;
	case 7:
		return m_strChrate;
	case 8:
		return m_strVolume;
	case 9:
		return m_strPreprice;
	case 10:
		return m_strKey;
	case 11:
		{
			strTemp.Format("%ld", m_lStartDate);
			return strTemp;
		}
		break;
	case 12:
		{
			strTemp.Format("%ld", m_lEndDate);
			return strTemp;
		}
		break;
	case 13:
		{
			strTemp.Format("%ld", m_lDataSize);
			return strTemp;
		}
		break;
	}
	return "";
}

void CChartItemData::SetReciveValue(CString strValue, short nType)
{
	CString strTemp = "";
	switch ( nType )
	{
	case 0:	m_strName = strValue;	break;
	case 1: m_strPrice = strValue;	break;
	case 2: m_strOpen = strValue;	break;
	case 3: m_strHigh = strValue;	break;
	case 4: m_strLow = strValue;	break;
	case 5: m_strSign = strValue;	break;
	case 6: m_strChange = strValue;	break;
	case 7: m_strChrate = strValue;	break;
	case 8: m_strVolume = strValue;	break;
	case 9: m_strPreprice = strValue;	break;
	case 10: m_strKey = strValue;	break;
	case 11: m_lStartDate = atol(strValue);	break;
	case 12: m_lEndDate = atol(strValue);	break;
	case 13:m_lDataSize = atol(strValue);	break;
	}
}
