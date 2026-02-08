// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartItemData.h"
#include "ChartItem.h"
#include "StdDialog.h"
#include "./define/ChartDef.h"
#include "./define/ChartMsgDef.h"
#include "./include/arMarketIndicator.h"

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

	m_pKB0602_ChartData = NULL;
	m_pKB0603_ChartData = NULL;
	m_pKB0604_ChartData = NULL;
	m_pKB1013_ChartData = NULL;
	m_pKB0606_ChartData = NULL;
	m_pKB1015_ChartData = NULL;
	m_pKB0515_ChartData = NULL;

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
}

// 선택된 차트의 시장 세팅 : 복수종목 - ojtaso (20070215)
void CChartItemData::SetTRCode(LPCTSTR lpszTRCode)
{
	if(strlen(lpszTRCode)==0 || m_strTRCode.Compare(lpszTRCode)==0)
		return;
	
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
	if(m_pKB0602_ChartData)
	{
		char *pData = (char*)m_pKB0602_ChartData;
		delete [] pData;
		m_pKB0602_ChartData = NULL;
	}
	if(m_pKB0603_ChartData)
	{
		char *pData = (char*)m_pKB0603_ChartData;
		delete [] pData;
		m_pKB0603_ChartData = NULL;
	}	

	if(m_pKB0604_ChartData)
	{
		char *pData = (char*)m_pKB0604_ChartData;
		delete [] pData;
		m_pKB0604_ChartData = NULL;
	}

	if(m_pKB1013_ChartData)
	{
		char *pData = (char*)m_pKB1013_ChartData;
		delete [] pData;
		m_pKB1013_ChartData = NULL;
	}

	if(m_pKB0606_ChartData)
	{
		char *pData = (char*)m_pKB0606_ChartData;
		delete [] pData;
		m_pKB0606_ChartData = NULL;
	}

	if(m_pKB1015_ChartData)
	{
		char *pData = (char*)m_pKB1015_ChartData;
		delete [] pData;
		m_pKB1015_ChartData = NULL;
	}

	if (m_pKB0515_ChartData)
	{
		char *pData = (char*)m_pKB0515_ChartData;
		delete [] pData;
		m_pKB0515_ChartData = NULL;
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
	if(bInitKey)
		memset(m_chkey,'0',sizeof(m_chkey));
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
CString CChartItemData::GetQueryData(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	m_lDataSize = 0;
	m_lCnt = 0;
	CString strTRCode = lpSelChart ? lpSelChart->m_strChartGubun : m_strTRCode;

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartItemData::GetQueryData  strTRCode(%s)\n", __LINE__, strTRCode);
// 		OutputDebugString(szDebug);
// 	}

	if(strTRCode == QUERY_STRJONGMOK)		
		return GetQueryData_KB_p0602(bNext, lpSelChart);
	else if(strTRCode == QUERY_STRUPJONG)
		return GetQueryData_KB_p0603(bNext, lpSelChart);	
	else if(strTRCode == QUERY_STRFUTOPT)
		return GetQueryData_KB_p0604(bNext, lpSelChart);
	else if(strTRCode == QUERY_STRSTOCKFUTOPT)
		return GetQueryData_KB_d1013(bNext, lpSelChart);
	else if(strTRCode == QUERY_STRFOREIGN)
		return GetQueryData_KB_p0606(bNext, lpSelChart);
	else if(strTRCode == QUERY_STRFOJIPYO)
		return GetQueryData_KB_p0515(bNext, lpSelChart);

	return "";
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0602(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0602_InRec1 kb0602InRec1;

	// 초기화
	memset(&kb0602InRec1, 0x20, sizeof(kb0602InRec1));

	// 종목코드
	memset(kb0602InRec1.shcode, 0x20, sizeof(kb0602InRec1.shcode));
	if(lpSelChart)
	{
		memcpy(kb0602InRec1.shcode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
		//KHD: 종목명 컨트롤은 주식때만 발생해야 하기때문에 여기다 넣음 : 
		((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_pInputBarDlg->SendSymbol(lpSelChart->m_strItemCode);
//		m_pChartItem->SetOldChartCode(lpSelChart->m_strItemCode);
	}
	else
	{
		memcpy(kb0602InRec1.shcode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
		((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_pInputBarDlg->SendSymbol(m_strCodeForRequest);
//		m_pChartItem->SetOldChartCode(m_strCodeForRequest);
	}

	// 검색시작일
	memcpy(kb0602InRec1.date, m_pChartItem->GetEndDateRequested(), sizeof(kb0602InRec1.date));

	// 조회건수
	char Temp[64];
	sprintf(Temp,"%04d",GetCntForQuery(bNext));
	memcpy(kb0602InRec1.num, Temp, sizeof(kb0602InRec1.num));

	// 조회단위
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if(lpSelChart)
		memcpy(kb0602InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
	else
		memcpy(kb0602InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());

	// 조회구분
	memcpy(kb0602InRec1.button,&chTypeRequested,sizeof(kb0602InRec1.button));

	// 수정주가
	char chUseRevised = '0';
	if(m_pChartItem->GetUseRevisedValue())
	{
		chUseRevised = '1';
	}
	memcpy(kb0602InRec1.modGb,&chUseRevised,sizeof(kb0602InRec1.modGb));

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
	memcpy(kb0602InRec1.nflag,&chFlag,sizeof(kb0602InRec1.nflag));

	// 검색키
	memset(kb0602InRec1.nkey,0x20,sizeof(kb0602InRec1.nkey));
	if(chFlag == '1')//다음 조회일 경우
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();
		int i = sizeof(KB_p0602_InRec1);
		memcpy(kb0602InRec1.nkey, strKey, min(sizeof(kb0602InRec1.nkey), strKey.GetLength()));

	}
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		if(sKey.GetLength()>0)
			memcpy(kb0602InRec1.nkey,m_chkey,sizeof(kb0602InRec1.nkey));
		else
			memset(kb0602InRec1.nkey,0x20,sizeof(kb0602InRec1.nkey));
	}
	else //단순 조회 일 경우.
		memset(kb0602InRec1.nkey,0x20,sizeof(kb0602InRec1.nkey));
*/

	// 가격구분
	char chUseRateValue = '0';
	int nNumericValueType = lpSelChart ? lpSelChart->m_nChartNumericValueType : m_pChartItem->GetChart_NumericValueType();

	if(nNumericValueType == WON_TYPE)	
		chUseRateValue = '0';
	else if(nNumericValueType == PERCENT_TYPE)	
//		chUseRateValue = '1';		// 비교차트와의 연관으로 5로 변경됨
		chUseRateValue = '5';
	else if(nNumericValueType == DOLLAR_TYPE)
	{
		chUseRateValue = '2';
		//@Solomon	091123SM072	-->
		CString strRate = ((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_strDollar;
		memcpy(kb0602InRec1.usRate, (LPTSTR)(LPCTSTR)strRate, min(strRate.GetLength(), sizeof(kb0602InRec1.usRate)));
		//@Solomon	091123SM072	<--
	}
	memcpy(kb0602InRec1.nDataGubun, &chUseRateValue, sizeof(kb0602InRec1.nDataGubun));

	// 봉구분
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())// TRUE이면 허봉 
		chUseFackedCandle = '0'; // '0':허수봉 '1':실봉           
	memcpy(kb0602InRec1.ImgRgb, &chUseFackedCandle, sizeof(kb0602InRec1.ImgRgb));

	// 복기구분값
	memcpy(kb0602InRec1.Irecordgb, m_pChartItem->GetReplayFlag(), sizeof(kb0602InRec1.Irecordgb));
	
	// 복기시작n일전
	memcpy(kb0602InRec1.nDayBefore, m_pChartItem->GetReplayDayBefore(),	sizeof(kb0602InRec1.nDayBefore));

	// 복기시간(hhmmss)
	CString strReplayTime = m_pChartItem->GetReplayStartTime();
	memcpy(kb0602InRec1.Itime, m_pChartItem->GetReplayStartTime(), strReplayTime.GetLength());
	
	// 당일조회
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	|| 
		chTypeRequested == WEEK_DATA_CHART	|| 
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
		else if (strQueryOpt == "2")
			chNowData = '2';
	}
	memcpy(kb0602InRec1.nowData, &chNowData, sizeof(kb0602InRec1.nowData));

	CString strKB0602Info;
	char* szKB0602Info = strKB0602Info.GetBuffer(sizeof(kb0602InRec1));
	memcpy(szKB0602Info, (char*)&kb0602InRec1, sizeof(kb0602InRec1));
	strKB0602Info.ReleaseBuffer();
	CString strKB0602InRec1;
	strKB0602InRec1 = strKB0602Info;
	return strKB0602InRec1;
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0603(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0603_InRec1 kb0603InRec1;

	// 초기화
	memset(&kb0603InRec1, 0x20, sizeof(kb0603InRec1));

	if(m_strCodeForRequest.GetLength() != UPJONG_LENGTH) 
	{
		ASSERT(0);
		return "";
	}

	// 장구분 / 업종코드
	if(lpSelChart)
	{
		kb0603InRec1.Sjanggb[0] = '1';		
		kb0603InRec1.Supcode[0] = lpSelChart->m_strItemCode[0];	
		kb0603InRec1.Supcode[1] = lpSelChart->m_strItemCode[1];	
		kb0603InRec1.Supcode[2] = lpSelChart->m_strItemCode[2];

//		m_pChartItem->SetOldChartCode(lpSelChart->m_strItemCode);
	}
	else
	{
		kb0603InRec1.Sjanggb[0] = '1';		
		kb0603InRec1.Supcode[0] = m_strCodeForRequest[0];	
		kb0603InRec1.Supcode[1] = m_strCodeForRequest[1];	
		kb0603InRec1.Supcode[2] = m_strCodeForRequest[2];

//		m_pChartItem->SetOldChartCode(m_strCodeForRequest);
	}
	// 봉구분
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())// TRUE이면 허봉 
		chUseFackedCandle = '0'; // '0':허수봉 '1':실봉           
	memcpy(kb0603InRec1.ImgRgb, &chUseFackedCandle, sizeof(kb0603InRec1.ImgRgb));

	// 검색시작일
	memcpy(kb0603InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(kb0603InRec1.Sdate));

	// 검색할데이터갯수
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(kb0603InRec1.Smaxbong, Temp, sizeof(kb0603InRec1.Smaxbong));
	
	// N봉(틱분시일)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if(lpSelChart)
		memcpy(kb0603InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
	else
		memcpy(kb0603InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());

	// 조회구분
	memcpy(kb0603InRec1.Sbtngb, &chTypeRequested, sizeof(kb0603InRec1.Sbtngb));

	// 다음/조회
	char chFlag;
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	if(bNext)
	{		
		chFlag = '1';
	}
	else
	{
		chFlag = '0';
	}
	memcpy(kb0603InRec1.Snextgb, &chFlag, sizeof(kb0603InRec1.Snextgb));

	// 페치키
	memset(kb0603InRec1.Snextkey, 0x20, sizeof(kb0603InRec1.Snextkey));
	if(chFlag == '1')	// 다음 조회일 경우
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(kb0603InRec1.Snextkey, strKey, min(sizeof(kb0603InRec1.Snextkey), strKey.GetLength()));
	}
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();

		if(sKey.GetLength()>0)
			memcpy(kb0603InRec1.Snextkey, m_chkey, sizeof(kb0603InRec1.Snextkey));
		else
			memset(kb0603InRec1.Snextkey, 0x20, sizeof(kb0603InRec1.Snextkey));
	}
	else				// 단순 조회일 경우.
		memset(kb0603InRec1.Snextkey, 0x20, sizeof(kb0603InRec1.Snextkey));
*/
	
	kb0603InRec1.SdataType[0] = '0';	//추가080521
	// 당일조회
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	|| 
		chTypeRequested == WEEK_DATA_CHART	|| 
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
		else if (strQueryOpt == "2")
			chNowData = '2';
	}

	memcpy(kb0603InRec1.nowData, &chNowData, sizeof(kb0603InRec1.nowData));

	CString strKB0603Info;
	char* szKB0603Info = strKB0603Info.GetBuffer(sizeof(kb0603InRec1));
	memcpy(szKB0603Info, (char*)&kb0603InRec1, sizeof(kb0603InRec1));
	strKB0603Info.ReleaseBuffer();
	CString strKB0603InRec1;
	strKB0603InRec1 = strKB0603Info;
	return strKB0603InRec1;
}

CString CChartItemData::GetQueryData_KB_p0604(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0604_InRec1 kb0604InRec1;

	// 초기화
	memset(&kb0604InRec1, 0x20, sizeof(kb0604InRec1));

	// 종목코드
	memset(kb0604InRec1.Scode, 0x20, sizeof(kb0604InRec1.Scode));
	if(lpSelChart)
	{
		memcpy(kb0604InRec1.Scode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
//		m_pChartItem->SetOldChartCode(lpSelChart->m_strItemCode);
	}
	else
	{
		memcpy(kb0604InRec1.Scode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
//		m_pChartItem->SetOldChartCode(m_strCodeForRequest);
	}

	// 검색시작일
	memcpy(kb0604InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(kb0604InRec1.Sdate));

	// 검색할데이터갯수
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(kb0604InRec1.Smaxbong, Temp, sizeof(kb0604InRec1.Smaxbong));

	// N봉(틱분시일)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if(lpSelChart)
		memcpy(kb0604InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
	else
		memcpy(kb0604InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());

	// 구분(틱분일주월)
	memcpy(kb0604InRec1.Sbtngb, &chTypeRequested, sizeof(kb0604InRec1.Sbtngb));

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
	memcpy(kb0604InRec1.Snextgb, &chFlag, sizeof(kb0604InRec1.Snextgb));

	// 페치키
	memset(kb0604InRec1.Snextkey, 0x20, sizeof(kb0604InRec1.Snextkey));
	if(chFlag == '1')	// 다음 조회일 경우
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(kb0604InRec1.Snextkey, strKey, min(sizeof(kb0604InRec1.Snextkey), strKey.GetLength()));
	}
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		if(sKey.GetLength()>0)
			memcpy(kb0604InRec1.Snextkey, m_chkey, sizeof(kb0604InRec1.Snextkey));
		else
			memset(kb0604InRec1.Snextkey, 0x20, sizeof(kb0604InRec1.Snextkey));
	}
	else				// 단순 조회일 경우
		memset(kb0604InRec1.Snextkey, 0x20, sizeof(kb0604InRec1.Snextkey));
*/

	kb0604InRec1.SdataType[0] = '0';	//추가080521

	// 봉구분(허수실봉)
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())
	{
		chUseFackedCandle = '0'; // '0':허수봉 '1':실봉           
	}
	memcpy(kb0604InRec1.Simgrgb, &chUseFackedCandle, sizeof(kb0604InRec1.Simgrgb));

	// 복기구분값
	memcpy(kb0604InRec1.Srecordgb, m_pChartItem->GetReplayFlag(), sizeof(kb0604InRec1.Srecordgb));

	// 복기시작n일전
	memcpy(kb0604InRec1.Sredate, m_pChartItem->GetReplayDayBefore(), sizeof(kb0604InRec1.Sredate));

	// 복기시간(hhmmss)
	memcpy(kb0604InRec1.Stime, m_pChartItem->GetReplayStartTime(), sizeof(kb0604InRec1.Stime));

	// 당일조회
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
		else if (strQueryOpt == "2")
			chNowData = '2';
	}

	memcpy(kb0604InRec1.nowData, &chNowData, sizeof(kb0604InRec1.nowData));	// 당일조회여부

//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
	kb0604InRec1.SgapRev[0] = '0';

	CString strCode;
	strCode.Format("%*.*s", sizeof(kb0604InRec1.Scode), sizeof(kb0604InRec1.Scode), kb0604InRec1.Scode);

	int nCodeType, nMarketType = ((CStdDialog*)m_pChartItem->m_pStdDialog)->GetMarketTypeFromCode(strCode, nCodeType);
	if (nMarketType == CMasterTypeInfo::COMMODITY_CHART)
	{
		if (m_pChartItem->m_bMonthGapRev)
			kb0604InRec1.SgapRev[0] = '1';
	}
//>> (Issue No : 0006572)

	CString strKB0604Info;
	char* szKB0604Info = strKB0604Info.GetBuffer(sizeof(kb0604InRec1));
	memcpy(szKB0604Info, (char*)&kb0604InRec1, sizeof(kb0604InRec1));
	strKB0604Info.ReleaseBuffer();
	CString strKB0604InRec1;
	strKB0604InRec1 = strKB0604Info;
	return strKB0604InRec1;
}

CString CChartItemData::GetQueryData_KB_d1013(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_d1013_InRec1 kb0605InRec1;

	// 초기화
	memset(&kb0605InRec1, 0x20, sizeof(kb0605InRec1));

	// 종목코드
	memset(kb0605InRec1.Scode, 0x20, sizeof(kb0605InRec1.Scode));
	if(lpSelChart)
	{
		memcpy(kb0605InRec1.Scode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
//		m_pChartItem->SetOldChartCode(lpSelChart->m_strItemCode);
	}
	else
	{
		memcpy(kb0605InRec1.Scode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
//		m_pChartItem->SetOldChartCode(m_strCodeForRequest);
	}

	// 검색시작일
	memcpy(kb0605InRec1.Sdate, (LPCTSTR)m_pChartItem->GetEndDateRequested(), sizeof(kb0605InRec1.Sdate));

	// 검색할데이터갯수
	char Temp[64];
	sprintf(Temp, "%04d", GetCntForQuery(bNext));
	memcpy(kb0605InRec1.Smaxbong, Temp, sizeof(kb0605InRec1.Smaxbong));

	// N봉(틱분시일)
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if(lpSelChart)
		memcpy(kb0605InRec1.Sunit, (LPCTSTR)lpSelChart->m_strTickMinDayNumber, lpSelChart->m_strTickMinDayNumber.GetLength());
	else
		memcpy(kb0605InRec1.Sunit, (LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(), m_pChartItem->GetTickMinDayNumberRequested().GetLength());

	// 구분(틱분일주월 초(01234 6))
	memcpy(kb0605InRec1.Sbtngb, &chTypeRequested, sizeof(kb0605InRec1.Sbtngb));

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
	memcpy(kb0605InRec1.Snextgb, &chFlag, sizeof(kb0605InRec1.Snextgb));

	// 페치키
	memset(kb0605InRec1.Snextkey, 0x20, sizeof(kb0605InRec1.Snextkey));
	if(chFlag == '1')	// 다음 조회일 경우
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(kb0605InRec1.Snextkey, strKey, min(sizeof(kb0605InRec1.Snextkey), strKey.GetLength()));
	}
/*
		CString sKey;
		sKey.Format("%21.21s", m_chkey); sKey.TrimLeft(); sKey.TrimRight();
		//if(atoi(sKey) > 0)
		if(sKey.GetLength()>0)
			memcpy(kb0605InRec1.Snextkey, m_chkey, sizeof(kb0605InRec1.Snextkey));
		else
			memset(kb0605InRec1.Snextkey, 0x20, sizeof(kb0605InRec1.Snextkey));
	}
	else				// 단순 조회일 경우
		memset(kb0605InRec1.Snextkey, 0x20, sizeof(kb0605InRec1.Snextkey));
*/

	kb0605InRec1.SdataType[0] = '0';	//추가080521

	// 봉구분(허수실봉)
	char chUseFackedCandle = '1';
	if(m_pChartItem->GetBNormalizedMinuteTimeRule())
	{
		chUseFackedCandle = '0'; // '0':허수봉 '1':실봉           
	}
	memcpy(kb0605InRec1.Simgrgb, &chUseFackedCandle, sizeof(kb0605InRec1.Simgrgb));

	// 복기구분값
	memcpy(kb0605InRec1.Srecordgb, m_pChartItem->GetReplayFlag(), sizeof(kb0605InRec1.Srecordgb));

	// 복기시작n일전
	memcpy(kb0605InRec1.Sredate, m_pChartItem->GetReplayDayBefore(), sizeof(kb0605InRec1.Sredate));

	// 복기시간(hhmmss)
	memcpy(kb0605InRec1.Stime, m_pChartItem->GetReplayStartTime(), sizeof(kb0605InRec1.Stime));

	// 당일조회
	char chNowData = '0';
	if( chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART )
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if(strQueryOpt == "1")
			chNowData = '1';
		else if (strQueryOpt == "2")
			chNowData = '2';
	}

	memcpy(kb0605InRec1.nowData, &chNowData, sizeof(kb0605InRec1.nowData));	// 당일조회여부

//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
	if (m_pChartItem->m_bMonthGapRev)
		kb0605InRec1.SgapRev[0] = '1';
	else
		kb0605InRec1.SgapRev[0] = '0';
//>> (Issue No : 0006572)

	CString strKB0605Info;
	char* szKB0605Info = strKB0605Info.GetBuffer(sizeof(kb0605InRec1));
	memcpy(szKB0605Info, (char*)&kb0605InRec1, sizeof(kb0605InRec1));
	strKB0605Info.ReleaseBuffer();
	CString strKB0605InRec1;
	strKB0605InRec1 = strKB0605Info;
	return strKB0605InRec1;
}

// 해외지수
// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
CString CChartItemData::GetQueryData_KB_p0606(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0606_InRec1 kb0606InRec1;

	// 초기화
	memset(&kb0606InRec1, 0x20, sizeof(kb0606InRec1));

	// 종목코드
	memset(kb0606InRec1.Shcode, 0x20, sizeof(kb0606InRec1.Shcode));
	if (lpSelChart)
		memcpy(kb0606InRec1.Shcode, (LPCTSTR)lpSelChart->m_strItemCode, lpSelChart->m_strItemCode.GetLength());
	else
		memcpy(kb0606InRec1.Shcode, (LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());

	// 검색시작일
	memcpy(kb0606InRec1.date, m_pChartItem->GetEndDateRequested(), sizeof(kb0606InRec1.date));

	// 조회건수
	char Temp[64];
	sprintf(Temp,"%04d",GetCntForQuery(bNext));
	memcpy(kb0606InRec1.num, Temp, sizeof(kb0606InRec1.num));

	// 조회단위
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	// 조회구분
	if(chTypeRequested == MIN_DATA_CHART || chTypeRequested == SEC_DATA_CHART || chTypeRequested == TICK_DATA_CHART)
	{
//		20121211 이상신 : 무한루프로 빠지지 않도록 수정.
//		CInputBarDlg::OnTimer() 의 if (nIDEvent == ID_SHARE_CODE_WAIT) 에서 수정.

//		((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_pInputBarDlg->SetChkType();// KHD : 해외는 분틱초 지원 안한다. 멀티차트에서 시장변경시 분타입이 들어갈땐 강제로 일로 변환.
		return "";
	}
	{
		if(lpSelChart)
			memcpy(kb0606InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
		else
			memcpy(kb0606InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	}


	memcpy(kb0606InRec1.button,&chTypeRequested,sizeof(kb0606InRec1.button));

	// 조회모드
	char chFlag;
	if (bNext)		chFlag = '1';
	else			chFlag = '0';

	memcpy(kb0606InRec1.nflag,&chFlag,sizeof(kb0606InRec1.nflag));

	// 검색키
	memset(kb0606InRec1.nkey,0x20,sizeof(kb0606InRec1.nkey));
	if (chFlag == '1')//다음 조회일 경우
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(kb0606InRec1.nkey, strKey, min(sizeof(kb0606InRec1.nkey), strKey.GetLength()));
	}

	// 당일조회
	char chNowData = '0';
	if (chTypeRequested == DAY_DATA_CHART	||
		chTypeRequested == WEEK_DATA_CHART	||
		chTypeRequested == MONTH_DATA_CHART ||
		chTypeRequested == YEAR_DATA_CHART)
		chNowData = '0';
	else
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if (strQueryOpt == "1")			chNowData = '1';
		else if (strQueryOpt == "2")	chNowData = '2';
	}
	memcpy(kb0606InRec1.nowData, &chNowData, sizeof(kb0606InRec1.nowData));

	if (m_pChartItem->GetScreenNo() == SCREENID_FOREIGN)
		memcpy(kb0606InRec1.timeGB, "0", sizeof(kb0606InRec1.timeGB));
	else
		memcpy(kb0606InRec1.timeGB, "1", sizeof(kb0606InRec1.timeGB));

	CString strKB0606Info;
	char* szKB0606Info = strKB0606Info.GetBuffer(sizeof(kb0606InRec1));
	memcpy(szKB0606Info, (char*)&kb0606InRec1, sizeof(kb0606InRec1));
	strKB0606Info.ReleaseBuffer();
	CString strKB0606InRec1;
	strKB0606InRec1 = strKB0606Info;
	return strKB0606InRec1;
}

CString CChartItemData::GetQueryData_KB_p0515(BOOL bNext, ST_SELCHART* lpSelChart/* = NULL*/)
{
	KB_p0515_InRec InRec;
	memset(&InRec, 0x20, sizeof(InRec));

	CString strData;
	//	종목코드
	if (lpSelChart)		strData = lpSelChart->m_strItemCode;
	else				strData = m_strCodeForRequest;
	memcpy(InRec.code, (LPCTSTR)strData, min(sizeof(InRec.code), strData.GetLength()));

	//	검색시작일
	strData = m_pChartItem->GetEndDateRequested();
	memcpy(InRec.date, (LPCTSTR)strData, min(sizeof(InRec.date), strData.GetLength()));

	//	조회건수
	strData.Format(_T("%04d"), GetCntForQuery(bNext));
	memcpy(InRec.maxbong, (LPCTSTR)strData, min(sizeof(InRec.maxbong), strData.GetLength()));

	// 조회단위
	if (lpSelChart)		strData = lpSelChart->m_strTickMinDayNumber;
	else				strData = m_pChartItem->GetTickMinDayNumberRequested();
	memcpy(InRec.unit, (LPCTSTR)strData, min(sizeof(InRec.unit), strData.GetLength()));

	// 조회구분
	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	if (chTypeRequested == SEC_DATA_CHART)
	{
		m_pChartItem->SetChartRequestType(DAY_DATA_CHART);
		chTypeRequested = DAY_DATA_CHART;
	}
	memcpy(InRec.btngb, &chTypeRequested, sizeof(InRec.btngb));

	// 다음조회구분
	char chFlag;
	if (bNext)		chFlag = '1';
	else			chFlag = '0';
	memcpy(InRec.nextgb, &chFlag, sizeof(InRec.nextgb));

	// 다음조회키
	if (chFlag == '1')
	{
		CString strKey;
		LPTSTR lpszKey = strKey.GetBuffer(sizeof(m_chkey) + 1);
		::memcpy(lpszKey, m_chkey, sizeof(m_chkey));
		strKey.TrimRight();
		strKey.ReleaseBuffer();

		memcpy(InRec.nextkey, strKey, min(sizeof(InRec.nextkey), strKey.GetLength()));
	}

	// 당일조회
/*	char chNowData = '0';
	if (chTypeRequested != DAY_DATA_CHART	&&
		chTypeRequested != WEEK_DATA_CHART	&&
		chTypeRequested != MONTH_DATA_CHART &&
		chTypeRequested != YEAR_DATA_CHART)
	{
		CString strQueryOpt = m_pChartItem->GetQueryOpt();
		if (strQueryOpt == "1")			chNowData = '1';
		else if (strQueryOpt == "2")	chNowData = '2';
	}
	memcpy(InRec.nowdata, &chNowData, sizeof(InRec.nowData));
*/
	CString strTemp;
	char* szTemp = strTemp.GetBuffer(sizeof(InRec));
	memcpy(szTemp, (char*)&InRec, sizeof(InRec));
	strTemp.ReleaseBuffer();
	CString strInRec;
	strInRec = strTemp;

	return strInRec;
}

//시장지표 TR을 날리기 전에 Input값을 셋팅힌다.
CString CChartItemData::GetMarketQueryData_ChartItemData(BOOL bNext, UINT nRqstCnt)
{
	m_lDataSize = 0;
	m_lCnt = 0;
	if(m_strTRCode == QUERY_STRMARKET)
		return GetQueryDataMarket(bNext, nRqstCnt);//return GetQueryDataMarket(bNext);		

	return "";
}

BOOL CChartItemData::MakeChartData(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	BOOL bRet = FALSE;
	if(m_strTRCode == QUERY_STRJONGMOK)
		bRet = MakeChartData_KB_p0602(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRUPJONG)
		bRet = MakeChartData_KB_p0603(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRFUTOPT)
		bRet = MakeChartData_KB_p0604(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRSTOCKFUTOPT)
		bRet = MakeChartData_KB_d1013(szData, nLength, lpSelChart);		
	else if(m_strTRCode == QUERY_STRFOREIGN)
		bRet = MakeChartData_KB_p0606(szData, nLength, lpSelChart);
	else if(m_strTRCode == QUERY_STRMARKET)
		bRet = MakeChartMarektData(szData, nLength);
	else if(m_strTRCode == QUERY_STRFOJIPYO)
		bRet = MakeChartData_KB_p0515(szData, nLength, lpSelChart);
/*
	if (g_pDebugMng)
	{
		CString strDebug;
		strDebug.Format(_T("MakeChartData() : start Date[%ld], end Date[%ld]\n"), m_lStartDate, m_lEndDate);
		g_pDebugMng->TraceTran('O', "CChartItemData::MakeChartData", strDebug.GetLength(), (LPBYTE)(LPCTSTR)strDebug);
	}
*/
	return bRet;
}

BOOL CChartItemData::MakeChartData_KB_p0602(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_p0602_OutRec1_EX* pKB0602_OutRec1 = (KB_p0602_OutRec1_EX*)szData;
	if(pKB0602_OutRec1 == NULL) return FALSE;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0602_OutRec1->nkey, sizeof(pKB0602_OutRec1->nkey));

//@Solomon-->
// 	m_pChartItem->EnableRequestMoreButton(TRUE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)
// 
// 	CString sKey;
// 	sKey.Format("%*.*s", sizeof(pKB0602_OutRec1->nkey), sizeof(pKB0602_OutRec1->nkey), pKB0602_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
// 
// 	if( atoi(sKey)  == 0)	memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0602_OutRec1->nkey), sizeof(pKB0602_OutRec1->nkey), pKB0602_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");

	ZeroMemory(m_chkey, sizeof(pKB0602_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pKB0602_ChartData)
	{
		m_pKB0602_ChartData = (KB_p0602_CHART*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	// 종목명
	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0602_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();

	// 차트카운트
	MEMCPYWITHNULL(szTemp, pKB0602_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;

	long lPosForChart = 0;
	if(nLength < (&pKB0602_OutRec1->fcnt[0] - &pKB0602_OutRec1->name[0]))
		return FALSE;

	// 차트데이터
	lPosForChart = nLength - (&pKB0602_OutRec1->fcnt[0] - &pKB0602_OutRec1->name[0]);
	memcpy(m_pKB0602_ChartData, &pKB0602_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB0602_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB0602_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_p0603(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_p0603_OutRec1_EX* pKB0603_OutRec1 = (KB_p0603_OutRec1_EX*)szData;
	if(pKB0603_OutRec1 == NULL) return FALSE;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0603_OutRec1->nkey, sizeof(pKB0603_OutRec1->nkey));

	m_pChartItem->EnableRequestMoreButton(TRUE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)

//@Solomon-->
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB0603_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0603_OutRec1->nkey), sizeof(pKB0603_OutRec1->nkey), pKB0603_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");

	ZeroMemory(m_chkey, sizeof(pKB0603_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pKB0603_ChartData)
	{
		m_pKB0603_ChartData = (KB_p0603_CHART*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lSize = 0;
	MEMCPYWITHNULL(szTemp, pKB0603_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	if(m_strCode[0]=='1')
	{
		m_strName = "KOSPI " + m_strName;
	}
	else if(m_strCode[0]=='2')
	{
		m_strName = "KOSDAQ " + m_strName;
	}

	MEMCPYWITHNULL(szTemp, pKB0603_OutRec1->fcnt);
	lSize = atol(szTemp);
	m_lCntTemp = lSize;

	long lPosForChart = 0;
	if(nLength < (&pKB0603_OutRec1->fcnt[0] - &pKB0603_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB0603_OutRec1->fcnt[0] - &pKB0603_OutRec1->name[0]);
	memcpy(m_pKB0603_ChartData, &pKB0603_OutRec1->fcnt[0],lPosForChart);
	if(!m_pChartItem->GetNext()&&!m_pChartItem->IsPrimaryChartItemData(this))
	{
		m_pKB0603_ChartData->bojomsg[7] = 'P';
	}

	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp,m_pKB0603_ChartData->chart[lSize-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp,m_pKB0603_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);
	
	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_p0604(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_p0604_OutRec1_EX* pKB0604_OutRec1 = (KB_p0604_OutRec1_EX*)szData;
	if(pKB0604_OutRec1 == NULL) return FALSE;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0604_OutRec1->nkey, sizeof(pKB0604_OutRec1->nkey));

	m_pChartItem->EnableRequestMoreButton(TRUE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)

//@Solomon-->
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB0604_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0604_OutRec1->nkey), sizeof(pKB0604_OutRec1->nkey), pKB0604_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0604_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	{ //100217-alzioyes.
		char *pData = (char*)m_pKB0604_ChartData;
		delete [] pData;
		m_pKB0604_ChartData = NULL;
	}

	if(!m_pKB0604_ChartData)
	{
		m_pKB0604_ChartData = (KB_p0604_CHART*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0604_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();

	MEMCPYWITHNULL(szTemp, pKB0604_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pKB0604_OutRec1->fcnt[0] - &pKB0604_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB0604_OutRec1->fcnt[0] - &pKB0604_OutRec1->name[0]);
	memcpy(m_pKB0604_ChartData, &pKB0604_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB0604_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB0604_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);

	if(m_pChartItem->GetScreenNo() == nScreenID_6401 && m_pChartItem->GetPrevDateRecvFlag())
	{
		CString strDate = m_pChartItem->GetEndDate();
		for(long l = 0; l < lCnt; l++)
			::memcpy(m_pKB0604_ChartData->chart[l].date, strDate.Right(4), 4);
	}
	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_d1013(void* szData, long nLength, ST_SELCHART* lpSelChart)
{
	KB_d1013_OutRec1_EX* pKB0605_OutRec1 = (KB_d1013_OutRec1_EX*)szData;
	if(pKB0605_OutRec1 == NULL) return FALSE;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0605_OutRec1->nkey, sizeof(pKB0605_OutRec1->nkey));

//	m_pChartItem->EnableRequestMoreButton(TRUE);	// 분차트 마지막 키인 경우 - ojtaso (20071226)

//@Solomon-->
// 	CString sKey;
// 	sKey.Format("%21.21s", pKB0605_OutRec1->nkey);
// 	sKey.TrimLeft(); sKey.TrimRight(); sKey.Replace(" ", "");
// 	if( atoi(sKey)  == 0)
// 		memset(m_chkey,0x00,sizeof(m_chkey));
// 
// 	if(sKey.GetLength() > 8)
// 	{
// 		sKey.Delete(0, 8);
// 		if( atoi(sKey)  == 0)
// 			memset(m_chkey,0x00,sizeof(m_chkey));
// 	}
// 
// 	if(atoi(sKey) == 0)
// 		m_pChartItem->EnableRequestMoreButton(FALSE);// 분차트 마지막 키인 경우 - ojtaso (20071226)

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0605_OutRec1->nkey), sizeof(pKB0605_OutRec1->nkey), pKB0605_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0605_OutRec1->nkey));
	if(sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
	{
		m_pChartItem->EnableRequestMoreButton(FALSE);
	}
	
//@Solomon<--

	if(lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if(!m_pKB1013_ChartData)
	{
		m_pKB1013_ChartData = (KB_d1013_CHART*)new char[nLength];
	}
	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0605_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	
	MEMCPYWITHNULL(szTemp, pKB0605_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if(nLength < (&pKB0605_OutRec1->fcnt[0] - &pKB0605_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB0605_OutRec1->fcnt[0] - &pKB0605_OutRec1->name[0]);
	memcpy(m_pKB1013_ChartData, &pKB0605_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB1013_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB1013_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);
	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_p0606(void* szData, long nLength, ST_SELCHART* lpSelChart) //해외종목
{
	KB_p0606_OutRec1_EX* pKB0606_OutRec1 = (KB_p0606_OutRec1_EX*)szData;
	if(pKB0606_OutRec1 == NULL) return FALSE;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pKB0606_OutRec1->nkey, sizeof(pKB0606_OutRec1->nkey));

	CString sKey;
	sKey.Format("%*.*s", sizeof(pKB0606_OutRec1->nkey), sizeof(pKB0606_OutRec1->nkey), pKB0606_OutRec1->nkey);
	sKey.TrimLeft(); sKey.TrimRight(); //sKey.Replace(" ", "");
	
	ZeroMemory(m_chkey, sizeof(pKB0606_OutRec1->nkey));
	if (sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
		m_pChartItem->EnableRequestMoreButton(FALSE);

	if (lpSelChart)
		lpSelChart->m_strKey = m_chkey;

	if (!m_pKB0606_ChartData)
		m_pKB0606_ChartData = (KB_p0606_CHART*)new char[nLength];

	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pKB0606_OutRec1->name);
	m_strName = szTemp;
	m_strName.TrimRight();
	((CStdDialog*)(m_pChartItem->m_pStdDialog))->m_pInputBarDlg->m_strJonMokName = m_strName;//IBK 종목코드에서 종목명을 주지 않아서 AP를 씀 
	MEMCPYWITHNULL(szTemp, pKB0606_OutRec1->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;
	
	long lPosForChart = 0;
	if (nLength < (&pKB0606_OutRec1->fcnt[0] - &pKB0606_OutRec1->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pKB0606_OutRec1->fcnt[0] - &pKB0606_OutRec1->name[0]);
	memcpy(m_pKB0606_ChartData, &pKB0606_OutRec1->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB0606_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB0606_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}

BOOL CChartItemData::MakeChartData_KB_p0515(void* szData, long nLength, ST_SELCHART* lpSelChart) //해외종목
{
	if ((nLength <= 0) || (szData == NULL))
		return FALSE;

	KB_p0515_OutRec* pOutRec = (KB_p0515_OutRec*)szData;

	// 이전키
	memset(m_chkeyOld, 0x00, sizeof(m_chkeyOld));	// 시장지표때문에 chkeyOld 저장
	memcpy(m_chkeyOld, m_chkey, sizeof(m_chkey));
	memset(m_chkey, 0x00, sizeof(m_chkey));
	memcpy(m_chkey, pOutRec->nkey, sizeof(pOutRec->nkey));

	CString sKey;
	sKey.Format("%*.*s", sizeof(pOutRec->nkey), sizeof(pOutRec->nkey), pOutRec->nkey);
	sKey.TrimLeft(); sKey.TrimRight();

	ZeroMemory(m_chkey, sizeof(m_chkey));
	if (sKey.GetLength()>0)
	{
		memcpy(m_chkey, (LPCSTR)sKey, sKey.GetLength());
		m_pChartItem->EnableRequestMoreButton(TRUE);
	}
	else
		m_pChartItem->EnableRequestMoreButton(FALSE);

	if (lpSelChart)
		lpSelChart->m_strKey = m_chkey;
	
	if (!m_pKB0515_ChartData)
		m_pKB0515_ChartData = (KB_p0515_CHART*)new char[nLength];

	m_strCode = m_strCodeForRequest;

	char szTemp[64];
	long lCnt = 0;
	MEMCPYWITHNULL(szTemp, pOutRec->name);
	m_strName = szTemp;
	m_strName.TrimRight();

	MEMCPYWITHNULL(szTemp, pOutRec->fcnt);
	lCnt = atol(szTemp);
	m_lCntTemp = lCnt;

	long lPosForChart = 0;
	if (nLength < (&pOutRec->fcnt[0] - &pOutRec->name[0]))
		return FALSE;

	lPosForChart = nLength - (&pOutRec->fcnt[0] - &pOutRec->name[0]);
	memcpy(m_pKB0515_ChartData, &pOutRec->fcnt[0], lPosForChart);
	m_lDataSize = lPosForChart;
	MEMCPYWITHNULL(szTemp, m_pKB0515_ChartData->chart[lCnt-1].date);
	m_lEndDate = atol(szTemp);
	MEMCPYWITHNULL(szTemp, m_pKB0515_ChartData->chart[0].date);
	m_lStartDate = atol(szTemp);

	return TRUE;
}

CString CChartItemData::GetTrCode()
{
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
	long lPos = 0;
	for( lPos=0;lPos<lSize;lPos++)
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
	MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE, lpszRQ);

	if(lpSelChart->m_strChartGubun == QUERY_STRJONGMOK)
	{
		CString strPackType = "";
		if (lpSelChart->m_nChartNumericValueType == PERCENT_TYPE)
			strPackType = g_strPacketTypeNumber[7];
		else if(lpSelChart->m_nChartNumericValueType == WON_TYPE)
			strPackType = g_strPacketTypeNumber[0];
		else if(lpSelChart->m_nChartNumericValueType == DOLLAR_TYPE)
			strPackType = g_strPacketTypeNumber[6];

		if (strPackType.GetLength() > 0)
		{
			MakePacketOption(g_strPacketName[1],10,strPackType,FALSE, lpszRQ);
			MakePacketOption(g_strPacketName[2],10,strPackType,FALSE, lpszRQ);
			MakePacketOption(g_strPacketName[3],10,strPackType,FALSE, lpszRQ);
			MakePacketOption(g_strPacketName[4],10,strPackType,TRUE, lpszRQ);
			MakePacketOption(g_strPacketName[5],10,strPackType,TRUE, lpszRQ);
			MakePacketOption(g_strPacketName[6],18,strPackType,FALSE, lpszRQ);
		}
		MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, lpszRQ);			//락유무
		MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE, lpszRQ);		//상하한 Sign
		MakePacketOption("시가GuideLine",1,g_strPacketTypeEtc[1],FALSE, lpszRQ);
		MakePacketOption("고가GuideLine",1,g_strPacketTypeEtc[1],FALSE, lpszRQ);
		MakePacketOption("저가GuideLine",1,g_strPacketTypeEtc[1],FALSE, lpszRQ);
	}
	else
	{
		MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE, lpszRQ);
		MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE, lpszRQ);
		MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE, lpszRQ);
		MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE, lpszRQ);
		MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE, lpszRQ);

		if (m_strTRCode != QUERY_STRFOREIGN)	// QUERY_STRUPJONG, QUERY_STRFUTOPT, QUERY_STRSTOCKFUTOPT, QUERY_STRFOJIPYO
		{
			MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE, lpszRQ);
			MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE, lpszRQ);			//락유무
			if (m_strTRCode == QUERY_STRFUTOPT || m_strTRCode == QUERY_STRSTOCKFUTOPT)
				MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE, lpszRQ);	//KHD : 미결제 약정
		}
	}

	return TRUE;
}

BOOL CChartItemData::AddPacketAndGraph(BOOL IsNoVolumeGraph /*= FALSE*/)
{	
	if(!m_bInitalChart)
	{
#ifdef _WRITELOG		// Log - ojtaso (20080107)
		m_pChartItem->WriteToStringLog(_T("OCX로 AddPacket/Graph::Begin"), m_strCodeForRequest, m_strCodeForRequest.GetLength());
#endif
		BOOL bPrice = FALSE, bPriceMA = FALSE, bVolume = FALSE, bVolumeMA = FALSE;

		m_strArrayPacketName.RemoveAll();
		BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
		if(m_pChartItem->GetPtrST())// it depends on System Trading Spec. gm0604 2005/10/04
		{	
			ASSERT(IsPrimaryChartItemData);
			if(m_strTRCode == QUERY_STRJONGMOK)
			{
				MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
				MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE);		//상하한 Sign
			}
			else if(m_strTRCode == QUERY_STRFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// 상품선물옵션차트 - ojtaso (20071001)
			{
				MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
				MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE);
				MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE);		//KHD : 미결제약정 
			}

			AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
			
			bPriceMA = bVolume = bVolumeMA = TRUE;
		}
		else
		{
			if(IsPrimaryChartItemData)
			{			
				if(m_strTRCode == QUERY_STRJONGMOK)
				{
					MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
					
					int nNumber = -1;
					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)		nNumber = 7;
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)		nNumber = 0;
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)	nNumber = 6;

					if (nNumber != -1)
					{
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[nNumber],TRUE);
					}

					MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE);
					MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE);		//상하한 Sign

					MakePacketOption("시가GuideLine",1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption("고가GuideLine",1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption("저가GuideLine",1,g_strPacketTypeEtc[1],FALSE);

					if(!IsNoVolumeGraph)
					{
						bPriceMA = bVolume = bVolumeMA = TRUE;
						AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart
					}
					else
						bPrice = bPriceMA = TRUE;
				}
				else
				{
					if( m_strTRCode == QUERY_STRSTOCKFUTOPT )
					{
						MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
						MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					}
					else
					{
						MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE);
						MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					}

					if (m_strTRCode != QUERY_STRFOREIGN)
					{
						MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);
					}

					if (m_strTRCode == QUERY_STRFUTOPT || m_strTRCode == QUERY_STRSTOCKFUTOPT)
						MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE);

					AddGraphPriceChart();	// JSR 20060123 Add : For Making a Price Chart

					if (m_strTRCode != QUERY_STRFOJIPYO)
					{
						if (m_strTRCode != QUERY_STRFUTOPT || m_pChartItem->GetScreenNo() != nScreenID_6401)
							bPriceMA = TRUE;

						bVolume = bVolumeMA = TRUE;
					}
				}
			}
			else
			{			
				if(m_strTRCode == QUERY_STRUPJONG)
				{
					MakePacketOption(m_strName + g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
					MakePacketOption(m_strName + g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName + g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName + g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName,						10,g_strPacketTypeNumber[4],TRUE);
					MakePacketOption(m_strName + g_strPacketName[5],10,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName + g_strPacketName[6],18,g_strPacketTypeNumber[4],FALSE);
					MakePacketOption(m_strName + g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);

					CStringArray arrayPacket;
					arrayPacket.Add(m_strName);
					m_strArrayGraphTitle.Add(m_strName);
					CString strGraphOption = MakeGraphOption(g_strChartType[1],m_strName,arrayPacket,m_strName);
					m_pChartItem->AddGraph(1,0,0,strGraphOption,TRUE,TRUE,TRUE);
				}
				else if(m_strTRCode == QUERY_STRMARKET)
				{
					m_strArrayPacketName.Add(m_strCodeForRequest); //패킷 명 추가 
					return TRUE;
				}
			}
		}
		m_bInitalChart = TRUE;

		if (bPrice)
		{
			m_strArrayGraphTitle.Add(g_strIndexName[0]);
			m_pChartItem->AddGraph(0,0,0,"가격차트",FALSE,TRUE,TRUE);
		}

		if (bPriceMA)
		{
			m_strArrayGraphTitle.Add("가격 이동평균");
			m_pChartItem->AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);
		}

		if (bVolume)
		{
			m_strArrayGraphTitle.Add(g_strIndexName[1]);
			m_pChartItem->AddGraph(1,0,0,"거래량차트",TRUE,TRUE,TRUE);
		}

		if (bVolumeMA)
		{
			m_strArrayGraphTitle.Add("거래량 이동평균");
			m_pChartItem->AddGraph(1,0,0,"거래량 이동평균",FALSE,FALSE,FALSE);
		}

		if (bPrice)
			m_pChartItem->HideScale(FALSE);

		if (m_pChartItem->GetPtrST())
			m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
		else if (IsPrimaryChartItemData)
		{
			if (!m_bResetChartCfgWith)
			{
				m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
				m_bResetChartCfgWith = TRUE;
			}
		}

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

	int nBlockCount = 0;
	if(bAddGraph == TRUE && bOverlapp == FALSE && !((CStdDialog*)(m_pChartItem->m_pStdDialog))->IsAvailableBlockCount(TRUE, nBlockCount))
		bAddGraph = FALSE;

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
			if(m_strTRCode == QUERY_STRJONGMOK)
			{
				MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
				MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
				MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE);		//상하한 Sign
			}
			else if(m_strTRCode == QUERY_STRFUTOPT/* || m_strTRCode == QUERY_STRCOMMODITY*/)		// 상품선물옵션차트 - ojtaso (20071001)
			{
				MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
				MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
				MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE);
				MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
				MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE);		//KHD : 미결제약정 
			}

			AddGraphPriceChart();	// JSR 20060122 Add : For Making a Price Chart

			m_strArrayGraphTitle.Add("가격 이동평균");
			m_pChartItem->AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);


			m_strArrayGraphTitle.Add(g_strIndexName[1]);
			m_pChartItem->AddGraph(1,0,0,"거래량차트",TRUE,TRUE,TRUE);

			m_strArrayGraphTitle.Add("거래량 이동평균");
			m_pChartItem->AddGraph(1,0,0,"거래량 이동평균",FALSE,FALSE,FALSE);
		}
		else
		{	
			if(IsPrimaryChartItemData)
			{
				if(m_strTRCode == QUERY_STRJONGMOK)
				{
					MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);

					int nNumber = -1;
					if(m_pChartItem->GetChart_NumericValueType() == PERCENT_TYPE)		nNumber = 7;
					else if(m_pChartItem->GetChart_NumericValueType() == WON_TYPE)		nNumber = 0;
					else if(m_pChartItem->GetChart_NumericValueType() == DOLLAR_TYPE)	nNumber = 6;

					if (nNumber != -1)
					{
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[nNumber],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[nNumber],TRUE);
					}

					MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE);
					MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption(g_strPacketName[9],1,g_strPacketTypeNumber[0],FALSE);		//상하한 Sign

					MakePacketOption("시가GuideLine",1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption("고가GuideLine",1,g_strPacketTypeEtc[1],FALSE);
					MakePacketOption("저가GuideLine",1,g_strPacketTypeEtc[1],FALSE);
				}
				else
				{
					if( m_strTRCode == QUERY_STRSTOCKFUTOPT )
					{
						MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
						MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					}
					else
					{
						MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
						MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[4],FALSE);
						MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[4],TRUE);
						MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);
					}

					if (m_strTRCode != QUERY_STRFOREIGN)
					{
						MakePacketOption(g_strPacketName[6],18,g_strPacketTypeNumber[0],FALSE);
						MakePacketOption(g_strPacketName[7],1,g_strPacketTypeEtc[1],FALSE);
					}

					if (m_strTRCode == QUERY_STRFUTOPT || m_strTRCode == QUERY_STRSTOCKFUTOPT)
						MakePacketOption(g_strPacketName[8],10,g_strPacketTypeNumber[0],TRUE);
				}

				// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
				int nRow, nCol;
				if(bOverlapp == FALSE)
					AddGraphPriceChart2NewItem(nRow, nCol, bAddGraph);
				else
					AddGraphPriceChart_Overlapp(nRow, nCol);

				if(!m_bResetChartCfgWith)
				{
					m_pChartItem->ResetChartCfgWith(0);	// JSR 20060119 Modify ( 1 -> 0)
					m_bResetChartCfgWith = TRUE;
				}
			}
			else
			{
				if(m_strTRCode == QUERY_STRUPJONG)
				{
					MakePacketOption(m_strName + g_strPacketName[0],  8, g_strPacketTypeDate[0], TRUE);
					MakePacketOption(m_strName + g_strPacketName[1], 10, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName + g_strPacketName[2], 10, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName + g_strPacketName[3], 10, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName,						 10, g_strPacketTypeNumber[4], TRUE);
					MakePacketOption(m_strName + g_strPacketName[5], 10, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName + g_strPacketName[6], 18, g_strPacketTypeNumber[4], FALSE);
					MakePacketOption(m_strName + g_strPacketName[7],  1, g_strPacketTypeEtc[1], FALSE);

					CStringArray arrayPacket;
					arrayPacket.Add(m_strName);
					m_strArrayGraphTitle.Add(m_strName);
					CString strGraphOption = MakeGraphOption(g_strChartType[1],m_strName,arrayPacket,m_strName);
					m_pChartItem->AddGraph(1,0,0,strGraphOption,TRUE,TRUE,TRUE);
				}
				else if(m_strTRCode == QUERY_STRMARKET)
				{
					m_strArrayPacketName.Add(m_strCodeForRequest); //패킷 명 추가 
					return TRUE;
				}
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
	//m_strArrayPacketName.RemoveAll();
	//m_strArrayGraphTitle.RemoveAll();
	//m_pChartItem->ResetOCX();


	//MakePacketOption(g_strPacketName[0],8,g_strPacketTypeDate[0],TRUE);
	//MakePacketOption(g_strPacketName[1],10,g_strPacketTypeNumber[0],FALSE);
	//MakePacketOption(g_strPacketName[2],10,g_strPacketTypeNumber[0],FALSE);
	//MakePacketOption(g_strPacketName[3],10,g_strPacketTypeNumber[0],FALSE);
	//MakePacketOption(g_strPacketName[4],10,g_strPacketTypeNumber[0],TRUE);
	//MakePacketOption(g_strPacketName[5],10,g_strPacketTypeNumber[0],TRUE);

	//CStringArray arrayPacket;
	//arrayPacket.Add(g_strPacketName[1]);	// 시가
	//arrayPacket.Add(g_strPacketName[2]);	// 고가
	//arrayPacket.Add(g_strPacketName[3]);	// 저가
	//arrayPacket.Add(g_strPacketName[4]);	// 종가
	//m_strArrayGraphTitle.Add(g_strIndexName[0]);
	//CString strGraphOption = MakeGraphOption(g_strChartType[0],g_strIndexName[0],arrayPacket,"가격");
	//m_pChartItem->AddGraph(0,0,0,strGraphOption,FALSE,FALSE,FALSE);
	////AddGraph(0,0,0,"가격차트",FALSE,FALSE,FALSE);

	//arrayPacket.RemoveAll();
	//arrayPacket.Add(g_strPacketName[5]);	// 거래량
	//m_strArrayGraphTitle.Add(g_strIndexName[1]);
	//strGraphOption = MakeGraphOption(g_strChartType[2],g_strIndexName[1],arrayPacket,"거래량",0
	//	,RGB(218,46,46),RGB(218,46,46),RGB(46,46,218),RGB(46,46,218),RGB(218,46,46),RGB(218,46,46)
	//	,1);				
	//m_pChartItem->AddGraph(1,0,0,strGraphOption,TRUE,TRUE,TRUE);
		//AddGraph(1,0,0,"거래량차트",TRUE,TRUE,TRUE);
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
		if(m_pChartItem->GetScreenNo() == nScreenID_6401)
			m_pChartItem->AddGraph(0,0,0,"선가격차트",FALSE,FALSE,FALSE);
		else
			m_pChartItem->AddGraph(0,0,0,"가격차트",FALSE,FALSE,FALSE);
	}
}
///////////////////////////////////////////////////////////////////////////////

// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
void CChartItemData::AddGraphPriceChart2NewItem(int& nRow, int& nCol, BOOL bAddGraph)
{
	m_strArrayGraphTitle.Add(g_strIndexName[0]);

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
			m_pChartItem->AddGraph(RowCount,0,0,"가격차트",TRUE,TRUE,TRUE);
		}

		nRow = RowCount;
		nCol = 0;

		m_strArrayGraphTitle.Add("가격 이동평균");
		m_pChartItem->AddGraph(nRow,nCol,0,"가격 이동평균",FALSE,FALSE,FALSE);
	}

}

// 새로운 차트 추가 : 복수종목 - ojtaso (20061224)
void CChartItemData::AddGraphPriceChart_Overlapp(int& nRow, int& nCol)
{
	int nVertScaleGroup = m_pChartItem->GetVertScaleGroupCount(0, 0);
	if(nVertScaleGroup < 0)
		return;

	m_strArrayGraphTitle.Add(g_strIndexName[0]);

	nRow = nCol = 0;

	if(m_pChartItem->GetScreenNo() == nScreenID_6401)
	{
		m_pChartItem->AddGraph(nRow,nCol,0,"선가격차트",FALSE,FALSE,TRUE);
	}
	else
	{
		m_pChartItem->AddGraph(nRow,nCol,nVertScaleGroup,"가격차트",FALSE,TRUE,TRUE);
		
		m_strArrayGraphTitle.Add("가격 이동평균");
		m_pChartItem->AddGraph(nRow,nCol,nVertScaleGroup,"가격 이동평균",FALSE,FALSE,FALSE);
	}
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
	KB_d1015_OutRec1* pOutRec = (KB_d1015_OutRec1*)szData;
//@유진삭제
	strPacketCnt.Format("%1.1s", pOutRec->inNumGb);	

	int nMarketJipyCnt = atoi(strPacketCnt);
	//AfxMessageBox(strPacketCnt);
	
	if(nMarketJipyCnt == 2) //지표수가 2개
	{
		KB_d1015_OutRec_1* pOutRec_1 = (KB_d1015_OutRec_1*)szData;

		if(nLength < (&pOutRec_1->d1015_CHART.chtCnt[0] - &pOutRec_1->winid[0]))
			return FALSE;

		CString sOriCode, sRecvCode;
		sRecvCode.Format("%8.8s", pOutRec_1->shcode); sRecvCode.TrimLeft(); sRecvCode.TrimRight();
		sOriCode = m_pChartItem->GetSelectedItemCode();
		CString strJipyo = pOutRec->dataNm;
		if (strJipyo.Find("ADR") == -1 && strJipyo.Find("ADL") == -1 )	// 서버에서 업종일때 종목코드가 코스피일때 001 인대 00을빼고 1로 주어서 이런 코드를 작성함.
		{
			if(sOriCode.CompareNoCase(sRecvCode) != 0)
			{		
				CString strTR;
				strTR = QUERY_STRMARKET;

				m_pChartItem->RemoveChartItem_OneData(m_strCodeForRequest, strTR);
				return FALSE;
			}
		}
		memset(m_chkey,0x00,sizeof(m_chkey));
		memcpy(m_chkey, pOutRec_1->nxtKey, sizeof(pOutRec_1->nxtKey));

		if(lpSelChart)
			lpSelChart->m_strKey = m_chkey;

		if(!m_pKB1015_ChartData)
		{
			//m_pKB1015_ChartData = (char*)(new KB_p0607_CHART_1);
			m_pKB1015_ChartData = new char[nLength];
			//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_1));
			memset(m_pKB1015_ChartData, 0x00, nLength);
		}
		else
		{
			//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_1));
			char* pData = m_pKB1015_ChartData;
			//delete m_pKB1015_ChartData;
			delete [] pData;
			m_pKB1015_ChartData = NULL;
		}
		m_strCode = m_strCodeForRequest;	
		m_strName = m_strCode;
		char szTemp[64];
		long lSize = 0;

		MEMCPYWITHNULL(szTemp, pOutRec_1->d1015_CHART.chtCnt);
		lSize = atol(szTemp);

		if(lSize <= 0) 
		{
			if( m_pKB1015_ChartData )
			{
				//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_1));
				char* pData = m_pKB1015_ChartData;
				//delete m_pKB1015_ChartData;
				delete [] pData;
				m_pKB1015_ChartData = NULL;
			}
//			AfxMessageBox("Err 2");
			return FALSE;
		}


		MEMCPYWITHNULL(szTemp, pOutRec_1->d1015_CHART.chart[0].date);
		m_lStartDate = atol(szTemp);
		m_lCntTemp = lSize;

		long lPosForChart = 0;
		lPosForChart = nLength - (&pOutRec_1->d1015_CHART.chtCnt[0] - &pOutRec_1->winid[0]);		
		memcpy(m_pKB1015_ChartData, &pOutRec_1->d1015_CHART.chtCnt[0], lPosForChart);
		
		m_lDataSize = lPosForChart;

		MEMCPYWITHNULL(szTemp, pOutRec_1->d1015_CHART.chart[lSize-1].date);
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
		KB_d1015_OutRec_3* pOutRec_3 = (KB_d1015_OutRec_3*)szData;

		if(nLength < (&pOutRec_3->d1015_CHART.chtCnt[0] - &pOutRec_3->winid[0]))
			return FALSE;

		memset(m_chkey, 0x00, sizeof(m_chkey));
		memcpy(m_chkey, pOutRec_3->nxtKey, sizeof(pOutRec_3->nxtKey));

		if(lpSelChart)
			lpSelChart->m_strKey = m_chkey;

		if(!m_pKB1015_ChartData)
		{
			//m_pKB1015_ChartData = (char*)(new KB_p0607_CHART_3);
			//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_3));
			m_pKB1015_ChartData = new char[nLength];
			memset(m_pKB1015_ChartData, 0x00, nLength);
		}
		else
		{
			//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_3));
			//delete m_pKB1015_ChartData;
			char* pData = m_pKB1015_ChartData;
			//delete m_pKB1015_ChartData;
			delete [] pData;
			m_pKB1015_ChartData = NULL;
		}

		m_strCode = m_strCodeForRequest;	
		m_strName = m_strCode;
		char szTemp[64];
		long lSize = 0;

		MEMCPYWITHNULL(szTemp, pOutRec_3->d1015_CHART.chtCnt);
		lSize = atol(szTemp);

		if(lSize <= 0) 
		{
			if( m_pKB1015_ChartData )
			{
				//memset(m_pKB1015_ChartData, 0x00, sizeof(KB_p0607_CHART_3));
				//delete m_pKB1015_ChartData;
				char* pData = m_pKB1015_ChartData;
				//delete m_pKB1015_ChartData;
				delete [] pData;
				m_pKB1015_ChartData = NULL;
			}
//			AfxMessageBox("Err 4");
			return FALSE;
		}


		MEMCPYWITHNULL(szTemp, pOutRec_3->d1015_CHART.chart[0].date);
		m_lStartDate = atol(szTemp);
		m_lCntTemp = lSize;

		long lPosForChart = 0;
		lPosForChart = nLength - (&pOutRec_3->d1015_CHART.chtCnt[0] - &pOutRec_3->nxtKey[0]);
		memcpy(m_pKB1015_ChartData, &pOutRec_3->d1015_CHART.chtCnt[0], lPosForChart);

		m_lDataSize = lPosForChart;

		MEMCPYWITHNULL(szTemp, pOutRec_3->d1015_CHART.chart[lSize-1].date);
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

CString CChartItemData::GetQueryDataMarket(BOOL bNext, int nRqstCnt)
{
	KB_d1015_InRec1 InRec1;

	char Temp[64];
	char chFlag;

	// 초기화
	memset(&InRec1, 0x20, sizeof(InRec1));
	
	// 윈도우아이디
	CString sWndID;
	sWndID.Format("%d", m_pChartItem->GetDlgCtrlID());
	memcpy(InRec1.winid, sWndID, sWndID.GetLength());

	// 데이타번호
	memcpy(InRec1.dataNm, (LPTSTR)(LPCTSTR)m_strCodeForRequest, m_strCodeForRequest.GetLength());
	
	// 봉구분(0:허/1:실봉)
	InRec1.img[0] = '1';//허봉(0) 실봉(1);
	if(m_pChartItem->m_CfgInfo.bRealCandle == false) //허봉이라면..
		InRec1.img[0] = '0';

	CString strPrimCode = m_pChartItem->GetSelectedItemCode();
	if(strPrimCode.GetLength() == UPJONG_LENGTH)// 업종
	{
		// 타입(0:종목/1:업종)
		InRec1.type [0]= '1';

		// 종목코드
		memcpy(InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(InRec1.shcode));
		InRec1.shcode[3] = ' ';	
		InRec1.shcode[4] = ' ';	
		InRec1.shcode[5] = ' ';	
		InRec1.shcode[6] = ' ';	
		InRec1.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == CODE_STOCK_LENGTH || strPrimCode.GetLength() == (CODE_STOCK_LENGTH-1))							// 종목
	{
		// 타입(0:종목/1:업종)
		InRec1.type[0] = '0';

		// 종목코드
		memcpy(InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(InRec1.shcode));
		InRec1.shcode[6] = ' ';
		InRec1.shcode[7] = ' ';
	}
	else if(strPrimCode.GetLength() == FUTOPT_LENGTH)// || strPrimCode.GetLength() == COMMODITY_LENGTH)							// 선물
	{
		// 타입(0:종목/1:업종)
		InRec1.type[0] = '0';

		// 종목코드
		memcpy(InRec1.shcode,(LPCTSTR)strPrimCode,sizeof(InRec1.shcode));
	}

	// 검색일자(최근일)
	memcpy(InRec1.edate,(LPCTSTR)m_pChartItem->GetEndDateRequested(),sizeof(InRec1.edate));
	if(nRqstCnt >= 10000)
	{
		AfxMessageBox("시장지표 최대 조회 갯수는 10000개 이하입니다");
		nRqstCnt = 9999;
	}

	// 검색건수
	m_lCntRequired = nRqstCnt; //조회할 갯수와 실제 받은 갯수를 비교하기 위해 저장해둔다.
	sprintf(Temp,"%04d",nRqstCnt);
	
	if(atoi(Temp) <= 0) return _T("");
	memcpy(InRec1.num,Temp,sizeof(InRec1.num));

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
	memcpy(InRec1.nxtGb,&chFlag,sizeof(InRec1.nxtGb));

	// 조회키(다음/이전)
	CString sKey;
	sKey.Format("%*.*s", sizeof(m_chkey), sizeof(m_chkey), m_chkey);
	sKey.TrimLeft(); sKey.TrimRight();

	if(sKey.GetLength()>0)
		memcpy(InRec1.nxtKey, m_chkey, sizeof(InRec1.nxtKey));
	else
		memset(InRec1.nxtKey, 0x20, sizeof(InRec1.nxtKey));

	//>>0000550: 0503_선옵종합차트의 시장지표의 미결제약정 지표가 일주기 이외의 모든 주기에서도 조회되도록 수정요청
	// 조회단위
//	char chTypeRequested = lpSelChart ? lpSelChart->m_cType : m_pChartItem->GetTypeRequested();
	char chTypeRequested = m_pChartItem->GetTypeRequested();
//	if(lpSelChart)
//		memcpy(InRec1.unit,(LPCTSTR)lpSelChart->m_strTickMinDayNumber,lpSelChart->m_strTickMinDayNumber.GetLength());
//	else
		memcpy(InRec1.unit,(LPCTSTR)m_pChartItem->GetTickMinDayNumberRequested(),m_pChartItem->GetTickMinDayNumberRequested().GetLength());
	
	// 조회구분
	memcpy(InRec1.button,&chTypeRequested,sizeof(InRec1.button));
	//<<0000550

	CString strTrInfo;
	char* pszTrInfo = strTrInfo.GetBuffer(sizeof(InRec1));
	memcpy(pszTrInfo, (char*)&InRec1, sizeof(InRec1));
	strTrInfo.ReleaseBuffer();
	CString strOutput;
	strOutput = strTrInfo;
	return strOutput;
}

long CChartItemData::GetMaximumRequestCnt()
{
	if( (m_strTRCode == QUERY_STRJONGMOK) ||
		(m_strTRCode == QUERY_STRUPJONG ) ||
		(m_strTRCode == QUERY_STRMARKET ) || 
		(m_strTRCode == QUERY_STRFUTOPT ) ||
		(m_strTRCode == QUERY_STRSTOCKFUTOPT ) ||
		(m_strTRCode == QUERY_STRFOREIGN) ||
		(m_strTRCode == QUERY_STRFOJIPYO)
	)
	{
		return (long)DEFAULTCNTATONCE;
	}
	
	return 0;
		
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

	if (!m_pChartItem->IsPrimaryChartItemData(this))	// 주 데이터가 아닌 경우
	{
		if (m_lCntRequiredByPrim >= lCntReceived)
			m_lCntRequiredByPrim -= lCntReceived;

////gm0604 2004.8.2
		if (!m_lCntRequiredByPrim)
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

	if (!lCntReceived)
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

CString CChartItemData::MakePacketOption(LPCTSTR lpPacketName, long lPacketSize, LPCTSTR lpPacketType, BOOL bUseInReal, CString strRQ)
{
	CString strPacket, strTemp;

	if (bUseInReal)		strTemp = "유;\r\n";
	else				strTemp = "무;\r\n";

	//2005. 05. 18 add by sy, nam =============================================================
	//중첩 모드인경우 패킷 내용을 다르게 하기 위해 다른 함수를 호출한다.	
	if (m_bIsFilled && m_bOverLapping)
		strPacket.Format(_T("1;ov%d%s;;ov%d%d;ov%d%s;ov%d%s"), m_nCntOverLap, lpPacketName, m_nCntOverLap, lPacketSize,
																m_nCntOverLap, lpPacketType, m_nCntOverLap, strTemp);
	//=========================================================================================
	else
		strPacket.Format(_T("1;%s;;%d;%s;%s"), lpPacketName, lPacketSize, lpPacketType, strTemp);

	if (strRQ.GetLength() <= 0)
		m_pChartItem->AddPacket(strPacket);
	else
		m_pChartItem->AddPacketRQ(strRQ, strPacket);

	m_strArrayPacketName.Add(lpPacketName);

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
	if(m_strTRCode == QUERY_STRJONGMOK)
		return (long)m_pKB0602_ChartData; 
	else if(m_strTRCode == QUERY_STRUPJONG)
		return (long)m_pKB0603_ChartData;		
	else if(m_strTRCode == QUERY_STRFUTOPT)
		return (long)m_pKB0604_ChartData;
	else if(m_strTRCode == QUERY_STRSTOCKFUTOPT)
		return (long)m_pKB1013_ChartData;
	else if(m_strTRCode == QUERY_STRFOREIGN)
		return (long)m_pKB0606_ChartData;
	else if(m_strTRCode == QUERY_STRMARKET)
		return (long)m_pKB1015_ChartData;
	else if(m_strTRCode == QUERY_STRFOJIPYO)
		return (long)m_pKB0515_ChartData;

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
	if(m_strTRCode==QUERY_STRJONGMOK)
	{
		if(nValue == PERCENT_TYPE)
		{
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[7]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[7]);
		}
		else if(nValue == WON_TYPE)
		{
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[0]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[0]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[0]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[0]);
		}
		else if(nValue == DOLLAR_TYPE)
		{
#ifdef _DEBUG
//				OutputDebugString("==============================================================");
//				AfxMessageBox(" CChartItemData::SetChartNumericValue 달러 타입 정의 필요	 ");
//				OutputDebugString("==============================================================");
#endif			
			m_pChartItem->ChangePacketType(g_strPacketName[1],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[2],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[3],g_strPacketTypeNumber[6]);
			m_pChartItem->ChangePacketType(g_strPacketName[4],g_strPacketTypeNumber[6]);
		}
	}	
}

void CChartItemData::AddRealItem()
{
	BOOL IsPrimaryChartItemData = m_pChartItem->IsPrimaryChartItemData(this);
	
	if(m_strTRCode == QUERY_STRJONGMOK)
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
			if(IS_FUT_CODE_1(strCode[0]) || IS_SPREAD_CODE_1(strCode[0]))	// Future // 0024696: 파생상품 코드체계 변경 제도개선
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
	}
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
BOOL CChartItemData::SetLoadedPacketData(LPCTSTR lpszRQ, ST_SELCHART* lpSelChart)
{
//#ifdef __MULTI_LANG__
//	return m_pChartItem->SetPacketDataRQ(lpszRQ, GetPacketName(),GetData(),GetDataSize(),FALSE);
//#else
 	BOOL bSetPacketDataRQ = m_pChartItem->SetPacketDataRQ(lpszRQ, GetPacketName(),GetData(),GetDataSize(),FALSE);
 	RemoveData();
 	return bSetPacketDataRQ;
//#endif
}

BOOL CChartItemData::SetPacketData(BOOL bReset)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartItemData::SetPacketData:Begin\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}
//	if(g_pDebugMng) 
//		g_pDebugMng->Trace("%s TR[%s]", "CChartItemData::SetPacketData(..)", "IN");

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
			m_pChartItem->SetNext(TRUE);
		}
	}
	else
	{
		m_bIsFilled = TRUE;
		m_pChartItem->SetNext(FALSE);
	}

	if(!ProcRequestCntAfterQuery(m_lCntTemp))
	{
		bRetValue = FALSE;
		goto ExitLoop;
	}	
	//20090922 SJ_KIM ViewCount <*
	m_pChartItem->SetViewCountFlag(TRUE);
	bRetValue =  m_pChartItem->SetPacketData(GetPacketName(),GetData(),GetDataSize(),bReset);
	m_pChartItem->SetViewCountFlag(FALSE);
	//20090922 SJ_KIM ViewCount *>

ExitLoop:
	RemoveData();

//	if(g_pDebugMng) 
//		g_pDebugMng->Trace("%s TR[%s]", "CChartItemData::SetPacketData(..)", "OUT-OK");

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CChartItemData::SetPacketData:End\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}
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

	//if(g_pDebugMng) g_pDebugMng->TraceTran('O', "ItemData", GetDataSize(), (LPBYTE)GetData());
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
	if(::lstrcmp(lpszTRCode, QUERY_STRFOREIGN))
	{
		m_strArrayPacketName.Add(g_strPacketName[6]);
		m_strArrayPacketName.Add(g_strPacketName[7]);
	}//KHD 
	if( ::lstrcmp(lpszTRCode, QUERY_STRFUTOPT))
		m_strArrayPacketName.Add(g_strPacketName[8]);
}

void CChartItemData::OnSelectChartItem(ST_SELCHART* lpSelChart)
{
	SetTRCode(lpSelChart->m_strChartGubun);
	m_strCodeForRequest = lpSelChart->m_strItemCode;
	memcpy(m_chkey, (LPCTSTR)lpSelChart->m_strKey, lpSelChart->m_strKey.GetLength());
}

CString CChartItemData::GetEndDate()
{
	if (m_lEndDate > 0)
	{
		CString strDate;
		strDate.Format(_T("%ld"), m_lEndDate);

		return strDate;
	}

	return _T("");
}
