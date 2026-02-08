// XScaleTimeDiffManager.h: interface for the CXScaleTimeDiffManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSCALETIMEDIFFMANAGER_H__22E3D5C0_B833_4D91_BAE7_D14101F05941__INCLUDED_)
#define AFX_XSCALETIMEDIFFMANAGER_H__22E3D5C0_B833_4D91_BAE7_D14101F05941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <vector>
#include <set>

#include "../Include_Chart/DataBlockList.h"
#include "../Include_Chart/BlockBaseEnum.h"

class CXScaleManagerImp;

class CXScaleTimeDiffManager  
{
public:
	CXScaleTimeDiffManager();
	CXScaleTimeDiffManager(CXScaleManagerImp* pXScaleManagerImp);
	virtual ~CXScaleTimeDiffManager();

public:
	friend class CXScaleManagerImp;
	void	SetXScaleManagerImp(CXScaleManagerImp* pXScaleManagerImp);

public:
	typedef map<string, int>						MAP_RQ_TIME_DIFF;		// 시차는 음수도 있을 수 있으므로 time 관련 structure는 사용 못함
	typedef pair<string, int>						PAIR_RQ_TIME_DIFF;

	typedef map<string, tm>							MAP_RQ_CURR_TIME;
	typedef pair<string, tm>						PAIR_RQ_CURR_TIME;

	typedef map<string, time_t>						MAP_RQ_TIME_MARKET;
	typedef pair<string, time_t>					PAIR_RQ_TIME_MARKET;
/*
	typedef map<string, CDataBlockList<time_t>*>	MAP_RQ_BASE_TIME;
	typedef pair<string, CDataBlockList<time_t>*>	PAIR_RQ_BASE_TIME;

	typedef map<string, CDataBlockList<time_t>*>	MAP_RQ_LOCAL_TIME;
	typedef pair<string, CDataBlockList<time_t>*>	PAIR_RQ_LOCAL_TIME;
*/

public:
	CXScaleManagerImp*				m_pXScaleManagerImp;

	MAP_RQ_TIME_DIFF				m_mtdRQ;				// RQ 문자열 KEY로 각 RQ별 시간의 기준시와의 시차를 보관
	MAP_RQ_CURR_TIME				m_mRQCurrTime;			// RQ 문자열 KEY로 각 RQ별 현재 날짜+시간을 보관
	MAP_RQ_TIME_MARKET				m_mRQMarketTime;		// RQ 문자열 KEY로 각 RQ별 장시작 시간을 보관
//	MAP_RQ_BASE_TIME				m_mblRQBaseTime;		// RQ별 XScale의 기준시간정보를 가지고 있습니다.( 리얼, 삭제를 보다 잘 반영하기 위해서 일목균형표는 반영 되지 않은 상태 )
//	MAP_RQ_LOCAL_TIME				m_mblRQLocalTime;		// RQ별 XScale의 현지시간(수신된시간)정보를 가지고 있습니다.( 리얼, 삭제를 보다 잘 반영하기 위해서 일목균형표는 반영 되지 않은 상태 )

//	CDataBlockList<time_t>			m_blTimefDRI;			// XScale의 기준 시간 정보 ( 각 RQ별로 통일된 Merge된 시계열 )

	CString							m_strScreenRQ;			// 현재 화면에 보여지고 있는 시간대의 RQ
	int								m_tmScreenTimeDiff;		// 현재 화면에 보여지고 있는 시간대의 기준시와의 시차 ( HHMMSS )

public:
	void		InitRQDiffTime();									// RQ별 기준시와의 시차 데이타 맵 초기화
	void		InitRQMarketTime();									// RQ별 장시작시간 맵 초기화

	void		SetRQDiffTime(CString strRQ, int tmDiffTime, __int64 nCurrTime);		// RQ별 기준시와의 시차 셋팅
	void		SetRQMarketTime( CString strRQ, int nMarketStartTime, int nMarketEndTime, CPacketType::MARKETDAYTYPE nMarketDayType);	// RQ별 장시작시간 셋팅
	void		SetScreenDiffTime(CString strRQ, int tmDiffTime);	// 현재 화면에 보여지고 있는 시간대의 기준시와의 시차를 셋팅
	void		SetCurrentRQ(CString strRQ);						// 현재 스케일의 기준이 되는 RQ

	time_t		GetRQScreenTime(CString strRQ, tm *timeRQ);	// RQ의 현지시간으로 들어온 시간을 현재 화면에 보여지는 시간대의 시간으로 구한다
	time_t		GetRQScreenDateTime(CString strRQ, time_t timeRQ);

	tm*			CalcCurrentDateTimToPacketTime(CString strRQ);
};

#endif // !defined(AFX_XSCALETIMEDIFFMANAGER_H__22E3D5C0_B833_4D91_BAE7_D14101F05941__INCLUDED_)
