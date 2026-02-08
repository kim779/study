#if !defined(AFX_REPLAYCHARTWND_H__FE2A2AD6_B7A8_4A1A_BB18_B32EB2048441__INCLUDED_)
#define AFX_REPLAYCHARTWND_H__FE2A2AD6_B7A8_4A1A_BB18_B32EB2048441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReplayChartWnd.h : header file
//

#include <afxtempl.h>

#include "../chart_common/ReplayChartInfoDef.h"
#include "../../Inc/IAuTrCommMng.h"
#include "../../Inc/IAuPartnerMng.h"
#include "../../Inc/IGateMng.h"

#include "../commontr/TRDefine.h"

#include "ReceivedTRData.h"

class CDlgRecvTRProgress;

/////////////////////////////////////////////////////////////////////////////
// CReplayChartWnd window
#define IMPLCTRLCLASS CReplayChartWnd

#define MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 10 / 18
// Explain		: 시간정보
// Using way	: 
//-----------------------------------------------------------------------------
typedef struct _TIME_INFO
{
	int m_nDD;	// 날짜 정보
	int m_nHH;	// 시간 정보
	int m_nMM;	// 분 정보
	int m_nSS;	// 초 정보
} TIME_INFO;

typedef struct {
	BOOL m_bRequested;
	int m_nChartCount;
} STTRINFO;

class CReceivedTRData;
class CReceivedEachTR;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
// Comments		: 
// Using way	: 
// See			: 복기용 Dll
//-----------------------------------------------------------------------------
class CReceivedTRData;
class CReplayChartWnd : public CWnd
{
// Construction
public:
	CReplayChartWnd();

// Attributes
public:
	HWND		m_hMainFrame;
	HWND		m_hParent;

	CDlgRecvTRProgress *m_pDlgRecvTRProgress;
	CReceivedEachTR		m_rcvTrData;

	int			m_nRemainTRCount;			// Receive해야 되는 TR의 개수
	int			m_nReplaySpeed;				// 복기를 할 속도
	int			m_nReplayDayCount;			// 복기를 할 날짜수.

	BOOL		m_bSendCurTime;			// MultiChart에 시간을 전송했는지 체크
	TIME_INFO	m_StartTime;			// 시작 시간 정보
	TIME_INFO	m_CurTime;			// 현재 진행중인 시간 정보
	TIME_INFO	m_MarketStartTime[3];	// 장이 시작하는 시간 정보
	TIME_INFO	m_MarketEndTime[3];	// 장이 종료하는 시간 정보

	float		m_fTimeInterval;			// 배속에 따른 증가시간 정보

	UINT		m_unTimerHandle;			// Timer Handle
	UINT		m_unTRWaitTimerHandle;		// Server에서 TR이 오는지를 기다리는 Timer Handle
	CString		m_szMarketID;			// 0 : 주식(s) , 2 : 선물(f)
	
	BOOL		m_bReceiveTR;				// Server에 TR을 전송하고 나면 FALSE, 받으면 TRUE;
	int			m_nRcvDate;
	TIME_INFO	m_sendTime;
	int			m_nTotalTime;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplayChartWnd)
	//}}AFX_VIRTUAL

	
// Implementation
public:
	virtual		~CReplayChartWnd();

	// About Create
	void		Init(void *pReplayInfo);
	void		GetReplayInfo(REPLAY_INFO *pRecvReplayInfo);			// MultiChart로 부터 받은 복기에 필요한 정보를 저장한다.

	// 서버로 TR정보를 요청한다.
	void		SendTRList2Server(CString strTime);

	// TR 조회 결과를 받는다.
	HRESULT		ReceiveData(LPVOID	aTRData, long dwTRDateLen);
	void		SendTickData2MultiChart();

	BOOL		Run(int nMode);				// 0 : Stop, 1 : Play, 2 : Pause
	void		ChangeSpeed(int nSpeed);

	// 복기 전체 시간을 계산한다.
	int			GetTotalTime();

	void		IncreaseTime();			// 시간을 증가시킨다.
	void		SendCurTime();			// MultiChart에 현재 시간을 전송한다.
	void		MoveTickIdx();			// 현재 복기일에 맞게 TickIndex를 조정한다.

	void		SendToChart();

	int			CalcCurTime(int nEndTime);
	int			CalcTotalTime();

	enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, ELW_CHART, FOREIGN_CHART};
	
// Generated message map functions
protected:
	//{{AFX_MSG(CReplayChartWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LONG OnRmsgReceiveReplayData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPLAYCHARTWND_H__FE2A2AD6_B7A8_4A1A_BB18_B32EB2048441__INCLUDED_)

