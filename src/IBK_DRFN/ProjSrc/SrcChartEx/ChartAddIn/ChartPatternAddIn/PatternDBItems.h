#if !defined(AFX_PATTERNDBITEMS_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_)
#define AFX_PATTERNDBITEMS_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatternDBItems.h : header file
//


// Define Macro for CArray ===============================================>>
#include <afxtempl.h>			// for CArray
#define CArrayObj(CObject)		CArray <CObject*, CObject*>
#define CArrayObjPt(CObject)	CArray <CObject*, CObject*> *
// <<=======================================================================



// **************************************************************************
// CHighLowPtData Object				(주요고점, 저점 데이터)
// **************************************************************************
class CHighLowPtData : public CObject
{
// Construction
public:
	CHighLowPtData();
	virtual ~CHighLowPtData();

public:
	int				m_nPointType;				// 고점 저점 구분

	int				m_nDataSeq;					// 데이터 순서
	double			m_dTime;					// 시간
	
	double			m_dOpen;					// 시가
	double			m_dHigh;					// 고가
	double			m_dLow;						// 저가
	double			m_dClose;					// 종가
};


// **************************************************************************
// CMAData Object						(이동평균 데이터)
// **************************************************************************
class CMAData : public CObject
{
// Construction
public:
	CMAData();
	virtual ~CMAData();

public:
	int				m_nDataSeq;					// 데이터 순서

	double			m_dMMa;						// 조정이동평균
};


// **************************************************************************
// CRegistNSupportData Object				(지지선, 저항선 데이터)
// **************************************************************************
class CRegistNSupportData : public CObject
{
// Construction
public:
	CRegistNSupportData();
	virtual ~CRegistNSupportData();

public:
	int				m_nRSLineType;				// 저항선, 지지선 구분

	int				m_nDataSeq1;				// 첫번째 데이터 순서
	double			m_dYPoint1;					// 첫번째 데이터 값

	int				m_nDataSeq2;				// 두번째 데이터 순서
	double			m_dYPoint2;					// 두번째 데이터 값
};


// **************************************************************************
// CWavePatternData Object					(Wave Pattern Data)
// **************************************************************************
class CWavePatternData : public CObject
{
// Construction
public:
	CWavePatternData();
	virtual ~CWavePatternData();

public:
	int				m_nPatternType;				// Pattern Type
	BOOL			m_bIsSelect;				// Pattern Select

	int				m_nStartSeq;				// Start Sequence of a Pattern
	int				m_nEndSeq;					// End Sequence of a Pattern

	double			m_dDrawSeq1_1;				// First Draw Data
	double			m_dDrawYPt1_1;
	double			m_dDrawSeq1_2;				
	double			m_dDrawYPt1_2;

	double			m_dDrawSeq2_1;				// Second Draw Data
	double			m_dDrawYPt2_1;
	double			m_dDrawSeq2_2;
	double			m_dDrawYPt2_2;

	double			m_dNeckSeq1;				// First Neck Data
	double			m_dNeckYPt1;
	double			m_dNeckSeq2;				// Second Neck Data
	double			m_dNeckYPt2;
	
	double			m_dNeckBaseYPt1;
	double			m_dNeckBaseYPt2;
};

// **************************************************************************
// CLineDrawData Object					(Environment Setup of Line)
// **************************************************************************
class CLineDrawData : public CObject
{
// Construction
public:
	CLineDrawData();
	virtual ~CLineDrawData();

public:
	int m_nWeight;
	COLORREF m_color;
};


// **************************************************************************
// CPatternEnvData Object					(Environment Setup of Pattern)
// **************************************************************************
class CPatternEnvData : public CObject
{
// Construction
public:
	CPatternEnvData();
	virtual ~CPatternEnvData();
	BOOL IsDisplayCandle(int nCandleType);

public:
	BOOL			m_bNotDrawAll;					// 모든 라인 그리지 않음

	int				m_nDrawMethod;					// 자동추세선 작도법(0: 조정이동평균법, 1:전후캔들)
	int				m_nMAPeriod;					// 이동평균 기간
	int				m_nCandleCnt;					// 전후 캔들 갯수
	int				m_nCandleAfterCnt;				// 후 캔들 갯수

	BOOL			m_bDrawAutoPatternLine;			// 자동추세패턴
	BOOL			m_bDrawHighLowPtLine;			// 파동선
	BOOL			m_bDrawMMa;						// 조정이동평균 라인
	
	BOOL			m_bDrawHighPtHLine;				// 고점 수평라인
	BOOL			m_bDrawLowPtHLine;				// 저점 수평라인

	BOOL			m_bDrawHighRegistLine;			// 고점 저항선
	BOOL			m_bDrawLowSupportLine;			// 저점 지지선

	double			m_dHLPtAllowalbeError;			// 주요고점, 저점 계산시 고점, 저점 차이가 n% 이내인 경우 데이터로 사용하지 않음
	double			m_dNextPtAllowableError;		// 두 주요점을 지나는 직선과 다음 주요점과의 차이가 n% 이내인 경우 데이터로 사용

	BOOL			m_bFilterSideWalkPeriod;		// 횡보구간 필터링 여부

	CLineDrawData	m_lineDataHighLowPt;			// 파동선
	CLineDrawData	m_lineDataHighPtH;				// 고점 수평라인
	CLineDrawData	m_lineDataLowPtH;				// 저점 수평라인
	CLineDrawData	m_lineDataHighRegist;			// 고점 저항선
	CLineDrawData	m_lineDataLowSupport;			// 저점 지지선

	BOOL			m_bDrawPatternName;				// 캔들패턴 이름표시
	CArray<int, int>	m_arrNotDisplayCandleItems;		//캔들패턴 중 디스플레이 하지 않는 패턴 리스트
};

// **************************************************************************
// CSelPatternData4Draw Object				(Selected Pattern Data for Drawing)
// **************************************************************************
class CSelPatternData4Draw : public CObject
{
// Construction
public:
	CSelPatternData4Draw();
	virtual ~CSelPatternData4Draw();

public:
	BOOL			m_bDraw;
	int				m_nPatternType;

	int				m_nStartSeq;
	int				m_nEndSeq;

	int				m_nX1;
	int				m_nY1;
	int				m_nX2;
	int				m_nY2;
	
	int				m_nX3;
	int				m_nY3;
	int				m_nX4;
	int				m_nY4;

	int				m_nNeckXPos1;
	int				m_nNeckYPos1;
	int				m_nNeckXPos2;
	int				m_nNeckYPos2;

	int				m_nNeckXLength;
	int				m_nNeckYLength1;
	int				m_nNeckYLength2;
};


// **************************************************************************
// CCandlePatternData Object				(Candle Pattern Data)
// **************************************************************************
class CCandlePatternData : public CObject
{
// Construction
public:
	CCandlePatternData();
	virtual ~CCandlePatternData();

public:
	double			m_dTime;
	int				m_nCandlePtnType;
	
	int				m_nStartPos;
	int				m_nEndPos;

	double			m_dClose[6];		// Close Price for Accumulation rise and fall rate
	double			m_dHigh;			// 2006.07.21 Add by LYH for paint on chart candle.
	double			m_dLow;			// 2006.07.21 Add by LYH for paint on chart candle.
};

class CCandlePatternPosInfo : public CObject
{
	// Construction
public:
	CCandlePatternPosInfo();
	virtual ~CCandlePatternPosInfo();

public:
	CRect			m_rect;
	int				m_nCandlePtnType;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERNDBITEMS_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_)
