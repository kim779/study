#if !defined(AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_)
#define AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatternMaker.h : header file
//

#include "./Include_AddIn.h"

#include "HighLowPtData.h"								// 주요고점, 주요저점 데이터 정보 클래스
#ifndef __COMMON_DEF_H
#include "./Client/commondef.h"
#endif

#include "./Include_PCTR/IPCTRMng.h"
#include "./Include_PCTR/IGateMng.h"
#include "./Include_PCTR/IAuTrCommMng.h"

/////////////////////////////////////////////////////////////////////////////
// CPatternMaker Object

extern CConfigSet g_clConfigSet;
class CPatternMaker : public CObject
{
// Construction
public:
	CPatternMaker();
	virtual ~CPatternMaker();


public:
	CStringArray m_ArrayFileChart;//삼성 데모용 
	void ReadOneLineForSamgungDemo(); //삼성 데모용....
	void SetChegyulList(D48112_OSUB* pD48112_OSub);
	void SetCode(CString strCode);	
	void GetPMMDanga();
	void SetMAInfo(D10418_O* pD10418);
	void RecvRealJango(CString sMMDanga);
	void RecvRealCurPrice(CString sEnd);	
	void RecvRealHigh(CString sHigh);
	void RecvRealOpen(CString sOpen);
	void RecvRealLow(CString sLow);




	
	void Init_GuideLineValue();
	void DeleteLinePen();
	void CreateLinePen();
	void	OnDraw( CDC *p_pDC);

	//int		GetXPositoin(int nDataOffset, double dCandleWidth, CRect rctGraphRegion);	// X좌표를 구한다
	int		GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion);	// X좌표를 구한다
		
	double	GetGradient(double dXPos1, double dYPos1, double dXPos2, double dYPos2);	// 두점을 가지고 기울기를 구한다

	void	SetPatternEnvData(CPatternEnvData *pPatternEnvData);						// 패턴 환경설정 데이터


	CArray <CHighLowPtData*, CHighLowPtData*>			m_aryHighLowPtData;			// 주요고점, 주요저점 데이터
	CArray <CMAData*, CMAData*>							m_aryMAData;				// 이동평균 데이터
	CArray <CRegistNSupportData*, CRegistNSupportData*>	m_aryRegistNSupportData;	// 저항, 지지선 데이터
	CArray <CCheChartData*, CCheChartData*>				m_aryCheChartData;			// 차트에 그려질 체결 데이터


	void	DeleteHighLowPtData();													// 주요고점, 주요저점 데이터 삭제 
	void	DeleteMAData();															// 이동평균 데이터 삭제
	void	DeleteRegistNSupportData();												// 저항, 지지선 데이터 삭제
	void	DeleteCheChartData();													// 차트에 그려질 체결 데이터 삭제								

	BOOL	m_bSpecialChart;		// Special 차트 일때 실시간 처리 안한다.
	BOOL	m_bDrawChegyulLine;		// 체결 라인 및 체결 지점 표시.

	void SetHwndCurChatItem(HWND hWnd) { m_hwndChartItem = hWnd; }
	void SetHwndSelChatItem(HWND hWnd) { m_hSelChartItem = hWnd; }
	
	HWND GetHwndCurChatItem() { return m_hwndChartItem; }
	HWND GetHwndSelChatItem() { return m_hSelChartItem; }


	void SetBWonChart(BOOL bWonChart) { m_bWonChart = bWonChart; }
	BOOL GetBWonChart() { return m_bWonChart; }
	CString GetCode(){return m_strCode;};


protected:
	CString		m_strCode;				// 종목코드
	CPatternEnvData	*m_pPatternEnvData;	// 패턴 환경설정 데이터	
	D10418_O	m_MAvgInfo;				// 이동평균 정보
	CPen		m_pnHighLowLinePen;		// 주요고점 저점 연결라인 색상
	CPen		m_pnMaPen;				// 이동평균 색상
	CPen		m_pnHighLinePen;		// 주요고점 라인 색상
	CPen		m_pnLowLinePen;			// 주요저점 라인 색상
	
	BOOL		m_bLog;					// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	BOOL		m_bReverse;				// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	
	double		m_dViewMin;				// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	double		m_dViewMax;				// View내에 대상 Graph된 View의 Maximum Scale 값이다.
	//=========================================================================================
	//실제 가지고 있는 데이터 갯수보다 설정된 값이 클 경우 그리지 않는다.

	BOOL		m_bDraw2Pyung1;			// 이동평균 1번째 그릴것 인지..
	BOOL		m_bDraw2Pyung2;			// 이동평균 2번째 그릴것 인지..
	BOOL		m_bDraw2Pyung3;			// 이동평균 3번째 그릴것 인지..
	BOOL		m_bDraw2Pyung4;			// 이동평균 4번째 그릴것 인지..
	//=========================================================================================
	
	

	double m_dYSprice;	//전일가격 시가
	double m_dYHprice;	//전일가격 고가
	double m_dYLprice;	//전일가격 저가
	double m_dYEprice;	//전일가격 종가
	double m_dYSHL;		//전일가격 (시+고+저)/3
	double m_dTSprice;	//당일가격 시가
	double m_dTHprice;	//당일가격 고가
	double m_dTLprice;	//당일가격 저가
	double m_dTEprice;	//당일가격 종가
	double m_dTHL;		//당일가격 (고+저)/2
	double m_dTSHL;		//당일가격 (시+고+저)/3
	double m_dTHLE;		//당일가격 (고+저+(종*2))/4
	double m_dHighLimit;//상한가
	double m_dLowLimit;	//하한가
	double m_dPivot2Rst;	// 피봇 2차 저항
	double m_dPivot1Rst;	// 피봇 1차 저항
	double m_dPivotGLine;	// 피봇 기준선
	double m_dPivot1GG;	// 피봇 1차 지지
	double m_dPivot2GG;	// 피봇 2차 지지
	double m_dDMKPreHPrice;	// Demark 예상고가
	double m_dDMKGLine;		// Demark 기준선
	double m_dDMKPreLPrice;	// Demark 예상저가

	double m_dMAPeriod1; //이동평균 1번째 값
	double m_dMAPeriod2; //이동평균 2번째 값
	double m_dMAPeriod3; //이동평균 3번째 값
	double m_dMAPeriod4; //이동평균 4번째 값

	double m_dMASum1; //당일을 제외한 이동평균 1번째 합계 
	double m_dMASum2; //당일을 제외한 이동평균 2번째 합계
	double m_dMASum3; //당일을 제외한 이동평균 3번째 합계
	double m_dMASum4; //당일을 제외한 이동평균 4번째 합계

	double m_dPmmDanga ; //평균매입단가
	double m_dPmmPDanga; //단가 + n%
	double m_dPmmMDanga; //단가 - n%

	//==========================================================================
	CPen m_penYSprice;			//전일가격 시가				펜
	CPen m_penYHprice;			//전일가격 고가				펜
	CPen m_penYLprice;			//전일가격 저가				펜
	CPen m_penYEprice;			//전일가격 종가				펜
	CPen m_penYSHL;				//전일가격 (시+고+저)/3		펜
	
	CPen m_penTSprice;			//당일가격 시가				펜
	CPen m_penTHprice;			//당일가격 고가				펜
	CPen m_penTLprice;			//당일가격 저가				펜
	CPen m_penTEprice;			//당일가격 종가				펜
	CPen m_penTHL;				//당일가격 (고+저)/2		펜
	CPen m_penTSHL;				//당일가격 (시+고+저)/3		펜
	CPen m_penTHLE;				//당일가격 (고+저+(종*2))/4 펜
	CPen m_penHighLimit;		//상한가					펜
	CPen m_penLowLimit;			//하한가					펜
	
	CPen m_penPivot2Rst;		// 피봇 2차 저항 펜 
	CPen m_penPivot1Rst;		// 피봇 1차 저항 펜
	CPen m_penPivotGLine;		// 피봇 기준선  펜
	CPen m_penPivot1GG;			// 피봇 1차 지지 펜
	CPen m_penPivot2GG;			// 피봇 2차 지지 펜
	
	CPen m_penDMKPreHPrice;		// Demark 예상고가  펜
	CPen m_penDMKGLine;			// Demark 기준선	펜
	CPen m_penDMKPreLPrice;		// Demark 예상저가  펜
	
	CPen m_pen2Pyung1;			// 이동평균 첫번째 입력값 펜
	CPen m_pen2Pyung2;			// 이동평균 두번째 입력값 펜
	CPen m_pen2Pyung3;			// 이동평균 세번째 입력값 펜
	CPen m_pen2Pyung4;			// 이동평균 네번째 입력값 펜
	
	CPen m_penPmmDanga;			// 평균매입 단가 펜
	CPen m_penPmmPDanga;		// 평균매입 단가 +n% 펜
	CPen m_penPmmMDanga;		// 평균매입 단가 -n% 펜

	CPen m_penChegyul;			// 체결 시간표시	펜 
	CPen m_penSell;				// 매도 체결 표시	펜
	CPen m_penBuy;				// 매수 체결 표시	펜
	//==========================================================================
	
	HWND m_hwndChartItem;
	HWND m_hSelChartItem;


	BOOL m_bWonChart;

	IChartManager *m_pIChartManager;
	IChartOCX *m_pIChartOCX;
	IPacketManager *m_pIPacketManager;
	void RefreshChart();
public:
	void			SetIChartOCX( IChartManager *p_pIChartManager, IChartOCX *p_pIChartOCX, IPacketManager *p_pIPacketManager);
	//{{ 통신(소켓) 처리 모듈
	IAUTrCommManager*		m_pITrCommManager;
	ITrComm*		m_TRComm;		
	
	class CTrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDataLen);
		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData);
		STDMETHOD(StatusChange) (int nState);

	public:
		CPatternMaker* m_pTLMParent;
	} m_xTrCommSite;
	friend class CPatternMaker;
	//}}

	void RequestSiseData();
	void ReceiveSiseData(LPVOID	aTRData, long dwTRDataLen);

	CString	m_strRQ;		// 사용되고 있는 RQ :복수종목 - ojtaso (20070307)
	void	SetRQ(LPCTSTR lpszRQ) {m_strRQ = lpszRQ; }
	LPCTSTR GetRQ() { return m_strRQ; };

// (2008/9/9 - Seung-Won, Bae) Request MA Data
protected:
	BYTE	m_nCodeGubun;
public:
	void RequestMaData( void);
	void ReceiveMaData( LPVOID aTRData, long dwTRDataLen);

// (2008/10/26 - Seung-Won, Bae) Draw DO/DH/DL/DC
protected:
	BOOL	m_bDrawOHLC;
	COLORREF	m_clrO;
	COLORREF	m_clrH;
	COLORREF	m_clrL;
	COLORREF	m_clrC;
public:
	void	SetDrawOHLC( BOOL p_bDrawOHLC, COLORREF p_clrUp, COLORREF p_clrDown);

// (2009/9/12 - Seung-Won, Bae) for OHLC title
protected:
	static CString	m_strTitleO;
	static CString	m_strTitleH;
	static CString	m_strTitleL;
	static CString	m_strTitleC;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERNMAKER_H__C8385516_C663_49EB_BC25_79CAEC4882FA__INCLUDED_)
