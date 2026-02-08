// ChartStockOpinionAddInImp.h: interface for the CChartPeriodSummaryAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_Chart/IPropertyMap.h"

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
	int m_nDateType;
	HDC m_hDC;
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
	BOOL IsValidIndex(int nIndex/*IN*/);		// 의미 있는 인덱스 여부 판단
	int GetDateType();							// 일자 종류 얻기, 삼성증권 맞춤임
	void OnDeleteLine();						// 기간 요약 삭제
	void OnPSConfig(CPeriodObj* pLine/*IN*/);	// 기간요약 설정 명령
	void OnReleaseDraw();						// 연속 그리기 해제 명령
	int GetPriceInfo(CPeriodObj::CPrice* pPriceInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);				// 가격정보 얻기
	int GetCandleInfo(CPeriodObj::CCandle* pCandleInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);			//본 정보 얻기
	int GetAmountInfo(CPeriodObj::CAmount* pAmountInfo/*OUT*/, int nSIndex/*IN*/, int nEIndex/*IN*/);			//거래량 정보 얻기
	int GetIndexFromValue(long lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT, N/A*/);		// 값 --> 인덱스
	int GetPointFromIndex(int x/*IN*/, int y/*IN*/, CPoint& ptIndex/*OUT*/);						// 인덱스  --> 위치 정보 
	int GetValueFromIndex(int ix/*IN*/, int iy/*IN*/, long& lx/*OUT*/, double&dy/*OUT*/);			// 인덱스 --> 값
	int GetIndexFromPoint(CPoint& pt/*IN,OUT*/, int& x/*OUT*/, int&y/*OUT*/);						// 위치 정보 --> 인덱스
	//BOOL GetValueFromPoint(CPoint& ptAt/*IN,OUT*/, long &x/*OUT*/, double &y/*OUT*/);				// 위치 정보 --> 값
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN,OUT*/);	// 스트링 tokenizing, 가능하다면 MFC 또는 SDK 함수로 대체
	BOOL GetChartBasicData(	int &p_nStartIndex/*OUT*/, int &p_nEndIndex/*OUT*/, int &p_nR/*OUT*/, int &p_nC/*OUT*/, CRect &p_rctBlockRegion/*OUT*/, CRect &p_rctGraphRegion/*OUT*/);	// 차트의 기본 정보 얻기
	BOOL GetSpanTime( CPoint ptPoint/*IN*/);	
	BOOL GetStringfromDataIdx( int nDataIdx/*IN*/, CString &strSpan/*OUT*/);

	
	CChartPeriodSummaryAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartPeriodSummaryAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

	virtual BOOL InvokeAddIn( int p_nCommandType, long p_lData);	// 사용하지 않음
		
protected:
	virtual void OnDrawAfterGraph( HDC p_hDC);		// 차트보다 앞 쪽에 그릴 때
	virtual void OnDrawBeforeGraph( HDC p_hDC);		// 차트보다 뒤 쪽에 그릴 때
	virtual BOOL OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI);	// ON COMMAND와 관련된 액션 처리
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);	// 데이터가 들어 올 때 호출됨
	virtual void OnResetChart( void);	// 차트가 리셋될 때 호출됨
	virtual BOOL OnAddInToolCommand(const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual void OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);	// 열때 호출됨
	virtual void OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);	// 닫을 때 호출됨
	virtual RECT DrawCursor( HDC p_hDC);	// 마우스로 그리기 하는 동안 호출됨
	virtual void OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);


	CObList m_LineList;

	int GetData_X(const CPoint& point, long& lx);	// XScale : 관련 Interface로 사용 - vntsorl(20070521)
private:
	BOOL m_bLog;						// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	BOOL m_bReverse;					// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	CRect m_rctBoundMark;
	double m_dViewMin;					// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	double m_dViewMax;					// View내에 대상 Graph된 View의 Maximum Scale 값이다.
	CRect m_rctBlockRegion, m_rctGraphRegion ;	// 그리기 영역 괸련
	CRect m_rctGraphDrwingRegion;
	int m_nStartIndex, m_nEndIndex;		// 화면 상의 처음 끝 인덱스
	CPoint m_ptPrevSpan;					// "선행 Span"을 표시할 영역의 중심 Point
	CPoint m_ptAfterSpan;					// "후행 Span"을 표시할 영역의 중심 Point

	int m_nPrevSpanIndex;					// "선행 Span"의 Index
	int m_nAfterSpanIndex;					// "후행 Span"의 Index
	int m_nPrevSpan;						// "선행 Span" 값
	int m_nAfterSpan;						// "후행 Span" 값

	CString m_strDateCompartment;			// XScale의 자료일자 구분자
	CString m_strTimeCompartment;			// XScale의 시간 구분자

};

#endif // !defined(AFX_CHARTPERIODSUMMARYADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
