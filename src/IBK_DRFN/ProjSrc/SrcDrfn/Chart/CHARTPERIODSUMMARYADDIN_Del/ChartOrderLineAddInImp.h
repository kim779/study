// ChartOrderLineAddInImp.h: interface for the CChartOrderLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_Chart/IPropertyMap.h"
#include "LineObj.h"

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

#define OL_FILE	"OrderLine.cfg"	// 주문선 저장 파일명

class CChartOrderLineAddInImp : public CChartAddInBase
{
public:
	double m_prevLastData;
	static CString m_strAddInItemName;	// Add-in 이름
	virtual BOOL InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);

	CChartOrderLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartOrderLineAddInImp();

private:
	CString m_strDateType;			// 기준봉
	double m_dLastValue;	
	CString m_strToolLoading;
	CPoint m_ptInit;				// 초기 위치(마우스 클릭)
    CString m_strCode;              // 종목코드
	CString m_strCodeName;			// 종목명
	POINT m_ptOffSet;				// 그래프 영역 offset

    int m_nCommandType;             // 명령 타입
	BOOL m_bCentered;				// 캔들 가운데 정렬
	HMENU m_hPopupMenu;				// popup menu, 클릭 시 활성화되는 메뉴
	HDC m_hDC;
	BOOL m_bNewDraw;				// 그리기/수정 모드, TRUE:그리기, FALSE:수정
	POSITION m_PosForRemove;		// 삭제시 필요

	DWORD m_dwMouseActionID;
	
	CObList m_LineList;				// 주문선 리스트
	CLineObj* m_pCurSelectedLine;	// 선택된 주문선

	BOOL m_bLog;					// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	BOOL m_bReverse;				// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	double m_dViewMin;				// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	double m_dViewMax;				// View내에 대상 Graph된 View의 Maximum Scale 값이다.
	CRect m_rctBlockRegion, m_rctGraphRegion ;
	CRect m_rctGraphDrwingRegion;
	int m_nStartIndex, m_nEndIndex;
	CPoint m_ptCursor;				// 마우스 클릭 위치
	int m_nDateType;				// 날짜 타입, 기준봉
	CStringList m_AllOrderLines;	// 저장된 모든 주문선

	void LoadOrderLine();			// 저장된 주문선 불러오기
	void OnOLConfig();				// 주문선 설정
	void OnOLMan();					// 주문선 관리
	void OnDeleteLine();			// 주문선 파일 & 메모리(화면) 삭제
	void OnDeleteLine2();			// 주문선 메모리(화면) 삭제
	void OnReleaseDraw();			// 연속 그리기 해제
	int GetIndexFromValue(long lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT*/);
	int GetPointFromIndex(int x/*IN*/, int y/*IN*/, CPoint& ptIndex/*OUT*/);
	int GetValueFromIndex(int ix/*IN*/, int iy/*IN*/, long& lx/*OUT*/, double&dy/*OUT*/);
	int GetIndexFromPoint(CPoint& pt/*IN,OUT*/, int& x/*OUT*/, int&y/*OUT*/);
	BOOL GetValueFromPoint(CPoint& ptAt/*IN,OUT*/, long &x/*OUT*/, double &y/*OUT*/);
	int GetYPositionOnLine(int& x/*IN,OUT*/, CPoint pt1/*IN*/, CPoint pt2/*IN*/);
	void UnregPS(long pLine/*IN*/);				// addin manager에서 오브젝트 등록 해제
	void RegPS(long pLine/*IN*/);				// 오브젝트 addin manager에 등록

	BOOL GetChartBasicData(	int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion);
	BOOL IsValidIndex(int nIndex);	// 주어진 인덱스가 유효한 인덱스인지 여부 판별
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);	// 스트링의 추출
	long UPOrDown(CLineObj* pLine, double dData);

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()
protected:
	virtual void OnDrawAfterGraph(HDC p_hDC);
	virtual BOOL OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnResetChart(void);
	virtual void OnLoadEnvironment(IPropertyMap *p_pIAddInEnvMap);	// 열때 호출됨
	virtual void OnSaveEnvironment(IPropertyMap *p_pIAddInEnvMap);	// 닫을 때 호출됨
	virtual RECT DrawCursor(HDC p_hDC);								// 마우스로 이동 중에 (그리기 하는 동안) 호출됨
	virtual void OnPacketData(const char *p_szPacketNames, BOOL p_bWithReset);	// 데이터가 들어 올 때 호출됨
	virtual void OnUpdatePacketData(BOOL p_bTrUpdate);
	virtual BOOL OnAddInToolCommand(const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL OnAddInToolCommand(const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual void OnRealData( const char *p_szPacketName, double p_dData);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);
};

#endif // !defined(AFX_CHARTORDERLINEADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
