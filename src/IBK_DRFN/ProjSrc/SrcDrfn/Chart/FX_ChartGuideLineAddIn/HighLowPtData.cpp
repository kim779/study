// HighLowPtData.cpp : implementation file
//

#include "stdafx.h"
#include "HighLowPtData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCheChartData
CCheChartData::CCheChartData()
{
	m_strJmCode		= _T("")	;	//종목코드
	m_strChetime	= _T("")	;	//체결시간		
	m_dChedanga		= VALUE_ZERO;	//체결단가	
	m_nMesuMedoGb	= VALUE_ZERO;	//매도매수구분
}

CCheChartData::~CCheChartData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CHighLowPtData
CHighLowPtData::CHighLowPtData()
{
	m_nPointGb		= VALUE_ZERO;

	m_nDataSequence = VALUE_ZERO;
	m_dTime			= VALUE_ZERO;		

	m_dOpen			= VALUE_ZERO;		
	m_dHigh			= VALUE_ZERO;		
	m_dLow			= VALUE_ZERO;			
	m_dClose		= VALUE_ZERO;		
}

CHighLowPtData::~CHighLowPtData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMAData
CMAData::CMAData()
{
	m_nDataSequence = VALUE_ZERO;
	
	m_dMMa			= VALUE_ZERO;
}

CMAData::~CMAData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRegistNSupportData
CRegistNSupportData::CRegistNSupportData()
{
	m_nPointGb		= VALUE_ZERO;				// 고점, 저점 구분

	m_nDataSeq1		= VALUE_ZERO;				// 첫번째 데이터 순서
	m_dYPoint1		= VALUE_ZERO;				// 첫번째 데이터 값

	m_nDataSeq2		= VALUE_ZERO;				// 두번째 데이터 순서
	m_dYPoint2		= VALUE_ZERO;				// 두번째 데이터 값
}

CRegistNSupportData::~CRegistNSupportData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRegistNSupportData
CPatternEnvData::CPatternEnvData()
{
	m_nMAPeriod				= 5;				// 이동평균 기간

	m_bDrawMMa				= FALSE;			// 조정이동평균 라인
	m_bDrawHighLowPtLine	= TRUE;				// 고점, 저점 연결라인

	m_bDrawHighPtHLine		= FALSE;			// 고점 수평라인
	m_bDrawLowPtHLine		= FALSE;			// 저점 수평라인

	m_bDrawHighRegistLine	= TRUE;				// 고점 저항선
	m_bDrawLowSupportLine	= TRUE;				// 저점 지지선

	m_dHLPtAllowalbeError	= 0;				// 주요고점, 저점 계산시 고점, 저점 차이가 n% 이내인 경우 데이터로 사용하지 않음
	m_dNextPtAllowableError	= 1;				// 두개의 주요점에서 기울기로구한 다음점과의 차이가 n%이내인 경우 데이터로 사용

	m_bFilterSideWalkPeriod	= TRUE;				// 횡보구간 필터링 여부
}

CPatternEnvData::~CPatternEnvData()
{
}

