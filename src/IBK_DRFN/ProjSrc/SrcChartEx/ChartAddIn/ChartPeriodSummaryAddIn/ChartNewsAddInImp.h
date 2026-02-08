// ChartNewsAddInImp.h: interface for the CChartNewsAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTNEWSADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTNEWSADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
// TR 통신 관련 포함 파일

#include "../../inc/IPCTRMng.h"
#include "../../inc/IGateMng.h"
#include "../../inc/IAuTrCommMng.h"

#include "NewsObj.h"

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

class CChartNewsAddInImp : public CChartAddInBase
{
public:
	POINT m_ptOffSet;
	HMENU m_hPopupMenu;	// 뉴스 아이콘 클릭 시 활성화되는 메뉴
	int m_nCommandType;	// invokeAddIn 의 명령 종류
	BOOL m_bCentered;	// Scale 의 중심에 위치 여부
//	CBitmap* m_pBmp;
	CNewsObj* m_pCurNewsObj;
	POSITION m_PosForRemove;
	
//	CDC m_dcMem;
//	CDC m_dcTranMem;	// 배경 지우기 위해
	DWORD m_dwMouseActionID;
	static CString m_strAddInItemName;
	CString m_strCode;
	long m_lCode;

	void OnNewsInfoRequest();	// news 일자 외부 전달
	int GetIndexFromValue(long lx/*IN*/, double dy/*IN*/, int& ix/*OUT*/, int& iy/*OUT*/);
	int GetPointFromIndex(int x/*IN*/, int y/*IN*/, CPoint& ptIndex/*OUT*/);
	int GetValueFromIndex(int ix/*IN*/, int iy/*IN*/, long& lx/*OUT*/, double&dy/*OUT*/);
	int GetIndexFromPoint(CPoint& pt/*IN,OUT*/, int& x/*OUT*/, int&y/*OUT*/);
	BOOL GetValueFromPoint(CPoint& ptAt/*IN,OUT*/, long &x/*OUT*/, double &y/*OUT*/);
	void ConvertBigEndian(long& in/*IN,OUT*/);
	virtual BOOL InvokeAddIn( int p_nCommandType, long p_lData);
	
	CChartNewsAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartNewsAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

	BOOL		GetChartBasicData(	int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion);

	//////////////////////////////////////////////////////////////////////////
	//{{ 통신(소켓) 처리 모듈
	//////////////////////////////////////////////////////////////////////////
public:
	IAUTrCommManager* m_pITrCommManager;
	ITrComm* m_TRComm;		
	CNewsObj* m_pNews;
	class CTrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData);
		STDMETHOD(StatusChange) (int nState);

	public:
		CChartNewsAddInImp* m_pTLMParent;
	} m_xTrCommSite;

	friend class CChartNewsAddInImp;
	//}}

	void RequestSiseData();
	void ReceiveSiseData(LPVOID	aTRData, long dwTRDateLen);
	//////////////////////////////////////////////////////////////////////////

protected:
	virtual void OnDrawAfterGraph( HDC p_hDC);
	virtual void OnDrawBeforeGraph( HDC p_hDC);
	virtual BOOL OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual void OnUpdatePacketData( BOOL p_bTrUpdate);
	virtual void OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual void OnResetChart( void);

	CNewsObjList m_NewsObjList;

private:
	BOOL		m_bLog;						// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	BOOL		m_bReverse;					// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	double		m_dViewMin;					// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	double		m_dViewMax;					// View내에 대상 Graph된 View의 Maximum Scale 값이다.
	CRect	rctBlockRegion, rctGraphRegion ;
	int		nStartIndex, nEndIndex;
};

#endif // !defined(AFX_CHARTNEWSADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
