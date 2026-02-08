// PatternDBItems.cpp : implementation file
//

#include "stdafx.h"
#include "PatternDBItems.h"
#include "PatternDefine.h"
#include "Include/ColorTable.h"							// for Color


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// **************************************************************************
// CHighLowPtData
// **************************************************************************
CHighLowPtData::CHighLowPtData()
{
	m_nPointType	= ZERO;

	m_nDataSeq		= ZERO;
	m_dTime			= ZERO;		

	m_dOpen			= ZERO;		
	m_dHigh			= ZERO;		
	m_dLow			= ZERO;			
	m_dClose		= ZERO;		
}
CHighLowPtData::~CHighLowPtData()
{
}


// **************************************************************************
// CMAData
// **************************************************************************
CMAData::CMAData()
{
	m_nDataSeq		= ZERO;
	
	m_dMMa			= ZERO;
}
CMAData::~CMAData()
{
}


// **************************************************************************
// CRegistNSupportData
// **************************************************************************
CRegistNSupportData::CRegistNSupportData()
{
	m_nRSLineType	= ZERO;				// 저항선, 지지선 구분

	m_nDataSeq1		= ZERO;				// 첫번째 데이터 순서
	m_dYPoint1		= ZERO;				// 첫번째 데이터 값

	m_nDataSeq2		= ZERO;				// 두번째 데이터 순서
	m_dYPoint2		= ZERO;				// 두번째 데이터 값
}
CRegistNSupportData::~CRegistNSupportData()
{
}


// **************************************************************************
// CWavePatternData
// **************************************************************************
CWavePatternData::CWavePatternData()
{
	m_nPatternType	= ZERO;				// Pattern Type
	m_bIsSelect		= FALSE;			// Pattern Select

	m_nStartSeq		= ZERO;				// Start Sequence of a Pattern
	m_nEndSeq		= ZERO;				// End Sequence of a Pattern

	m_dDrawSeq1_1	= ZERO;				// First Draw Data
	m_dDrawYPt1_1	= ZERO;
	m_dDrawSeq1_2	= ZERO;				
	m_dDrawYPt1_2	= ZERO;				

	m_dDrawSeq2_1	= ZERO;				// Second Draw Data
	m_dDrawYPt2_1	= ZERO;
	m_dDrawSeq2_2	= ZERO;				
	m_dDrawYPt2_2	= ZERO;				

	m_dNeckSeq1		= ZERO;				// First Neck Data
	m_dNeckYPt1		= ZERO;
	m_dNeckSeq2		= ZERO;				// Second Neck Data
	m_dNeckYPt2		= ZERO;

	m_dNeckBaseYPt1	= ZERO;
	m_dNeckBaseYPt2	= ZERO;
		
}
CWavePatternData::~CWavePatternData()
{
}

// **************************************************************************
// CLineDrawData
// **************************************************************************
CLineDrawData::CLineDrawData()
{
	m_nWeight = 1;
	m_color = RGB(0,0,0);
}

CLineDrawData::~CLineDrawData()
{
}

// **************************************************************************
// CPatternEnvData
// **************************************************************************
CPatternEnvData::CPatternEnvData()
{
	m_bNotDrawAll			= FALSE;			// 모든 라인 그리지 않음

	m_nDrawMethod			= 0;				// 자동추세선 작도법(0: 조정이동평균법, 1:전후캔들)
	m_nMAPeriod				= 5;				// 이동평균 기간
	m_nCandleCnt			= 3;				// 전후 캔들 갯수
	m_nCandleAfterCnt		= 3;				// 후 캔들 갯수

	m_bDrawAutoPatternLine	= TRUE;				// 자동추세패턴
	m_bDrawHighLowPtLine	= TRUE;				// 파동선
	m_bDrawMMa				= FALSE;			// 조정이동평균 라인

	m_bDrawHighPtHLine		= FALSE;			// 고점 수평라인
	m_bDrawLowPtHLine		= FALSE;			// 저점 수평라인

	m_bDrawHighRegistLine	= TRUE;			// 고점 저항선
	m_bDrawLowSupportLine	= TRUE;			// 저점 지지선

	m_dHLPtAllowalbeError	= 0;				// 주요고점, 저점 계산시 고점, 저점 차이가 n% 이내인 경우 데이터로 사용하지 않음
	m_dNextPtAllowableError	= 1;				// 두 주요점을 지나는 직선과 다음 주요점과의 차이가 n% 이내인 경우 데이터로 사용

	m_bFilterSideWalkPeriod	= TRUE;				// 횡보구간 필터링 여부

	m_lineDataHighLowPt.m_color = COLOR_HIGHLOW_PTLINE;
	m_lineDataHighPtH.m_color = COLOR_HIGH_HORZLINE;			// 고점 수평라인
	m_lineDataLowPtH.m_color = COLOR_LOW_HORZLINE;				// 저점 수평라인
	m_lineDataHighRegist.m_color = RGB(100,100,100);			// 고점 저항선
	m_lineDataLowSupport.m_color = RGB(100,100,100);			// 저점 지지선

	m_bDrawPatternName = FALSE;
}
CPatternEnvData::~CPatternEnvData()
{
}

BOOL CPatternEnvData::IsDisplayCandle(int nCandleType)
{
	int nIndex;
	for(int i=0; i<m_arrNotDisplayCandleItems.GetSize(); i++)
	{
		nIndex = m_arrNotDisplayCandleItems.GetAt(i);
		if(nCandleType == nIndex)
			return FALSE;
	}
	return TRUE;
}
// **************************************************************************
// CSelPatternData4Draw Object
// **************************************************************************
CSelPatternData4Draw::CSelPatternData4Draw()
{
	m_bDraw			= FALSE;
	m_nPatternType	= ZERO;				// Pattern Type

	m_nStartSeq		= ZERO;
	m_nEndSeq		= ZERO;

	m_nX1			= ZERO;
	m_nY1			= ZERO;
	m_nX2			= ZERO;
	m_nY2			= ZERO;

	m_nX3			= ZERO;
	m_nY3			= ZERO;
	m_nX4			= ZERO;
	m_nY4			= ZERO;

	m_nNeckXPos1	= ZERO;
	m_nNeckYPos1	= ZERO;
	m_nNeckXPos2	= ZERO;
	m_nNeckYPos2	= ZERO;

	m_nNeckXLength	= ZERO;
	m_nNeckYLength1	= ZERO;
	m_nNeckYLength2	= ZERO;
}
CSelPatternData4Draw::~CSelPatternData4Draw()
{
}


// **************************************************************************
// CCandlePatternData Object
// **************************************************************************
CCandlePatternData::CCandlePatternData()
{
	m_dTime				= ZERO;			// Time
	m_nCandlePtnType	= ZERO;			// Candle Pattern Type
	
	m_nStartPos			= ZERO;
	m_nEndPos			= ZERO;

	memset( (void*)&m_dClose, ZERO, sizeof(m_dClose));	// Close Price for Accumulation rise and fall rate
}
CCandlePatternData::~CCandlePatternData()
{
}


// **************************************************************************
// CCandlePatternData Object
// **************************************************************************
CCandlePatternPosInfo::CCandlePatternPosInfo()
{
}
CCandlePatternPosInfo::~CCandlePatternPosInfo()
{
}
