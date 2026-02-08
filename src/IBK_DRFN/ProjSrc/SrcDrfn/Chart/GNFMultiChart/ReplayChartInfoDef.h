//=========================================================
// 2005. 08. 02 by sy, nam
// 복기차트 용 구조 
// Lib와 쓰고자 하는 Dll이 공히 Include 
// ReplayChartInfoDef.h
//=========================================================

#ifndef __HANHWA_CHART_REPLAY_INFO_H
#define __HANHWA_CHART_REPLAY_INFO_H

#define QUERY_STRREPLAY			"p0608"

	enum {STOP, PLAY, PAUSE};

	typedef struct REPLAY_INFO
	{
		void*			pRecvDBMgr;			//CDBMgr*
		HWND			hRecvMainFrame;		//MainFrame Handle
		HWND			hCallHwnd;			//복기를 요청하고 있는 Wnd's Handle
		int				nDataKind;			//주식, 선물 구분
		CStringArray	arrStrCode;			//종목코드
		char			cGubun;				//틱('0')/분('1') 구별 
		int				nUnit;				//조회 단위 1,2,3...........
		int				nSpeed;				//배속	
		int				nDayBefore;			//n일전
		CString			strStartDate;		//복기 시작시간 YYYYMMDDHHMM		
		CString			strEndDate;			//복기 종료시간 YYYYMMDDHHMM
	} REPLAY_INFO;

	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
	// Comments		: Tick 데이터
	// Using way	: 서버에서 내려오는 Tick 데이터
	//-----------------------------------------------------------------------------
	typedef struct _TICK_DATA
	{
		char chDate[6];				// 날짜 + 시간( DDHHMMSS )
		char chClosingPrice[10];	// 종가
		char chVolume[10];			// 체결량
	} TICK_DATA;

	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
	// Comments		: 
	// Using way	: 
	// See			: 서버에서 내려오는 전체 Tick 데이터
	//-----------------------------------------------------------------------------

	#define MAX_COUNT 50000
	
	typedef struct _TR01002_I
	{
		char chStockCode[16];		// 종목코드		// s : 주식 , f : 선물
		char chDayBefore[2];		// n일전 data
		char chTimeData[6];			// HHMMSS
	} TR01002_I;


	typedef struct _TR01002_CHECK
	{
		char chDayBefore[2];
		char chTickCount[5];					// Tick Data Count
		char chSubMsgLength[4];					// 차트 보조 메세지의 길이
		//TICK_DATA chTickData[ MAX_COUNT ];		// 전체 Tick Data
	} TR01002_CHECK;
	
	typedef struct _TR01002_O
	{
		TICK_DATA chTickData[ MAX_COUNT ];		// 전체 Tick Data

	}TR01002_O;
/*
	class CChangeDate
	{
	public:	
		CChangeDate(){};
		virtual ~CChangeDate(){};
	
		CString m_strSubMsg;						// 차트 보조 메세지		// Date=YYYYMMDDHHMMSS@JG=S(or F)@SC=15( Stock Code )
	};
*/
	
	class CSendRealReplay
	{
	public:	
		CSendRealReplay()
		{
		}
		virtual void OnReplayDrds(CString strCode, TICK_DATA* pReplayData) {};
	};

	const UINT RMSG_REPLAY_REAL			= ::RegisterWindowMessage(_T("RMSG_REPLAY_REAL"));
	const UINT RMSG_REPLAY_FAIL			= ::RegisterWindowMessage(_T("RMSG_REPLAY_FAIL"));
	const UINT RMSG_REPLAY_CHANGE_DATE	= ::RegisterWindowMessage(_T("RMSG_REPLAY_CHANGE_DATE"));
	const UINT RMSG_REPLAY_END			= ::RegisterWindowMessage(_T("RMSG_REPLAY_END"));
	const UINT RMSG_REPLAY_INC_SEC = ::RegisterWindowMessage(_T("RMSG_REPLAY_INC_SEC"));			// 1초가 증가 될때마다 전송
	const UINT RMSG_REPLAY_TOTAL_SEC = ::RegisterWindowMessage(_T("RMSG_REPLAY_TOTAL_SEC"));		// 전체 시간(초단위)를 전송
#endif