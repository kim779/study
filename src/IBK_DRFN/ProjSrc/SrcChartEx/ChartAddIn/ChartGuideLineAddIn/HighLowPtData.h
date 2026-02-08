#if !defined(AFX_HIGHLOWPTDATA_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_)
#define AFX_HIGHLOWPTDATA_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HighLowPtData.h : header file
//


// Init varialbe value with Zero
#define VALUE_ZERO				0


// Changing Point Satte
#define CHANGINGPT_HIGHTOLOW	0x0001		// 주요고점
#define CHANGINGPT_LOWTOHIGH	0x0002		// 주요저점

/////////////////////////////////////////////////////////////////////////////
// CCheChartData Object	-- 차트에 그려질 체결 리스트 
class CCheChartData : public CObject
{
public:	
	CString			m_strJmCode	;	//종목코드
	CString			m_strChetime	;	//체결시간	
	double			m_dChedanga	;	//체결단가	
	int				m_nMesuMedoGb;	//매도매수구분
	

// Construction, Destruction
	CCheChartData();
	virtual ~CCheChartData();
};


/////////////////////////////////////////////////////////////////////////////
// CHighLowPtData Object
class CHighLowPtData : public CObject
{
public:
	int				m_nPointGb;					// 고점 저점 구분

	int				m_nDataSequence;			// 데이터 순서
	double			m_dTime;					// 시간
	
	double			m_dOpen;					// 시가
	double			m_dHigh;					// 고가
	double			m_dLow;						// 저가
	double			m_dClose;					// 종가

// Construction, Destruction
	CHighLowPtData();
	virtual ~CHighLowPtData();
};


/////////////////////////////////////////////////////////////////////////////
// CMAData Object
class CMAData : public CObject
{
public:
	int				m_nDataSequence;			// 데이터 순서

	double			m_dMMa;						// 조정이동평균
	
// Construction, Destruction
	CMAData();
	virtual ~CMAData();
};


/////////////////////////////////////////////////////////////////////////////
// CRegistNSupportData Object
class CRegistNSupportData : public CObject
{
public:
	int				m_nPointGb;					// 고점, 저점 구분

	int				m_nDataSeq1;				// 첫번째 데이터 순서
	double			m_dYPoint1;					// 첫번째 데이터 값

	int				m_nDataSeq2;				// 두번째 데이터 순서
	double			m_dYPoint2;					// 두번째 데이터 값
		
// Construction, Destruction
	CRegistNSupportData();
	virtual ~CRegistNSupportData();
};


/////////////////////////////////////////////////////////////////////////////
// CPatternEnvData Object
class CPatternEnvData : public CObject
{
public:
	int				m_nMAPeriod;					// 이동평균 기간

	BOOL			m_bDrawMMa;						// 조정이동평균 라인
	BOOL			m_bDrawHighLowPtLine;			// 고점, 저점 연결라인
	
	BOOL			m_bDrawHighPtHLine;				// 고점 수평라인
	BOOL			m_bDrawLowPtHLine;				// 저점 수평라인

	BOOL			m_bDrawHighRegistLine;			// 고점 저항선
	BOOL			m_bDrawLowSupportLine;			// 저점 지지선

	double			m_dHLPtAllowalbeError;			// 주요고점, 저점 계산시 고점, 저점 차이가 n% 이내인 경우 데이터로 사용하지 않음
	double			m_dNextPtAllowableError;		// 두개의 주요점에서 기울기로구한 다음점과의 차이가 n%이내인 경우 데이터로 사용

	BOOL			m_bFilterSideWalkPeriod;		// 횡보구간 필터링 여부

	

// Construction, Destruction
	CPatternEnvData();
	virtual ~CPatternEnvData();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIGHLOWPTDATA_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_)
