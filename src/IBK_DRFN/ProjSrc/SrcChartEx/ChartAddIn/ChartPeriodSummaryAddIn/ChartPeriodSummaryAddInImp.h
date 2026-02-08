// ChartStockOpinionAddInImp.h: interface for the CChartPeriodSummaryAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_Chart/IPropertyMap.h"
#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_AddIn/I133691/_IPacketListManager.h"			// for IPacketListManager


#include "PeriodObj.h"

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

class CChartPeriodSummaryAddInImp : public CChartAddInBase
{
public:
	CString m_strToolLoading;
	BOOL m_bLoadData;
	int m_nMouseClickStatus;
	CPoint m_ptInit;
	BOOL m_bDrag;
	BOOL m_bMouseCaptured;
	CPoint m_ptCursor;
	POINT m_ptOffSet;
//	int m_nDateType;
	HMENU m_hPopupMenu;				// 기간요약선 클릭 시 활성화되는 메뉴 핸들
	int m_nCommandType;				// 명령 종류
	BOOL m_bCentered;				// 봉의 가운데 정렬 여부
	BOOL m_bAttachData;				// N/A
	BOOL m_bNewDraw;				// 그리기 모드에서 마우스 L 버튼 클릭 시 TREU, 그 외는 FALSE
	CPeriodObj* m_pCurSelectedLine;	//현재 선택된 기간요약 오브젝트
	POSITION m_PosForRemove;
	DWORD m_dwMouseActionID;
	static CString m_strAddInItemName;

//	CBitmap* m_pBmp;
//	CDC m_dcMem;
//	CDC m_dcTranMem;	// 배경 지우기 위해
//	CDC m_dcMemText;	// 텍스트용

	void UnregPS(long pLine/*IN*/);				// 오브젝트 addin manager에 등록
	void RegPS(long pLine/*IN*/);				// addin manager에 등록된 오브젝트 등록 해제
	BOOL IsValidIndex( const char *p_szRQ, int nIndex/*IN*/);		// 의미 있는 인덱스 여부 판단
	int GetDateType( CPeriodObj *p_pLine);		// 일자 종류 얻기, 삼성증권 맞춤임
	void OnDeleteLine();						// 기간 요약 삭제
	void OnPSConfig(CPeriodObj* pLine/*IN*/);	// 기간요약 설정 명령
	void OnReleaseDraw();						// 연속 그리기 해제 명령
	int GetPriceInfo( const char *p_szRQ, CPeriodObj::CPrice* pPriceInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);				// 가격정보 얻기
	int GetCandleInfo( const char *p_szRQ, CPeriodObj::CCandle* pCandleInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);			//본 정보 얻기
	int GetAmountInfo( const char *p_szRQ, CPeriodObj::CAmount* pAmountInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);			//거래량 정보 얻기
	
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//int GetIndexFromValue( const char *p_szRQ, int p_nEndIndex, long lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT, N/A*/);		// 값 --> 인덱스
	int GetIndexFromValue( const char *p_szRQ, int p_nEndIndex, __int64 lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT, N/A*/);		// 값 --> 인덱스
	// 2011.01.06 by SYS <<

	int GetPointFromIndex( const char *p_szRQ, int x/*IN*/, int y/*IN*/, CPoint& ptIndex/*OUT*/);						// 인덱스  --> 위치 정보 
//	int GetValueFromIndex(int ix/*IN*/, int iy/*IN*/, long& lx/*OUT*/, double&dy/*OUT*/);			// 인덱스 --> 값
	int GetIndexFromPoint( const char *p_szRQ, CPoint& pt/*IN,OUT*/, int& x/*OUT*/, int&y/*OUT*/);						// 위치 정보 --> 인덱스
	//BOOL GetValueFromPoint(CPoint& ptAt/*IN,OUT*/, long &x/*OUT*/, double &y/*OUT*/);				// 위치 정보 --> 값
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN,OUT*/);	// 스트링 tokenizing, 가능하다면 MFC 또는 SDK 함수로 대체
	BOOL GetChartBasicData(	CPeriodObj* p_pLine);	// 차트의 기본 정보 얻기
//	BOOL GetSpanTime( CPoint ptPoint/*IN*/);	
//	BOOL GetStringfromDataIdx( const char *p_szRQ, int nDataIdx/*IN*/, CString &strSpan/*OUT*/);

	
	CChartPeriodSummaryAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartPeriodSummaryAddInImp();

	CObList m_LineList;

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//int GetData_X(const CPoint& point, long& lx);	// XScale : 관련 Interface로 사용 - vntsorl(20070521)
	int GetData_X(const CPoint& point, __int64& lx);	// XScale : 관련 Interface로 사용 - vntsorl(20070521)
	CString DoubleToString(const double& Num);
	// 2011.01.06 by SYS <<

private:
//	BOOL m_bLog;						// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
//	BOOL m_bReverse;					// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	CRect m_rctBoundMark;
//	double m_dViewMin;					// View내에 대상 Graph된 View의 Minimun Scale 값이다.
//	double m_dViewMax;					// View내에 대상 Graph된 View의 Maximum Scale 값이다.
//	CRect m_rctBlockRegion, m_rctGraphRegion ;	// 그리기 영역 괸련
//	CRect m_rctGraphDrwingRegion;
//	int m_nStartIndex, m_nEndIndex;		// 화면 상의 처음 끝 인덱스
	CPoint m_ptPrevSpan;					// "선행 Span"을 표시할 영역의 중심 Point
	CPoint m_ptAfterSpan;					// "후행 Span"을 표시할 영역의 중심 Point

	int m_nPrevSpanIndex;					// "선행 Span"의 Index
	int m_nAfterSpanIndex;					// "후행 Span"의 Index
	int m_nPrevSpan;						// "선행 Span" 값
	int m_nAfterSpan;						// "후행 Span" 값

	CString m_strDateCompartment;			// XScale의 자료일자 구분자
	CString m_strTimeCompartment;			// XScale의 시간 구분자

public:
	// 20081009 박동관 >>
	IPacketListManager *	GetPacketListManager( void)
							{
								if( !m_pIPacketListManager) return NULL;
								m_pIPacketListManager->AddRef();
								return m_pIPacketListManager;
							}
	// 20081009 박동관 <<
	// 20081007 JS.Kim	사용자시간대 선택
	bool	RecalcLineXPoint( const char* szName, int nTimeDiff);


// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()
		
// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
	//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
	//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
	virtual void	OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
