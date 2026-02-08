
/* *********************************** */
/* 제작자: 이일렬(yiilyoul@magicn.com) */
/* 제작일: 2007.03.29                  */
/* *********************************** */

#include "StdAfx.h"
#include "XScaleManagerImp.h"

#include "../Include/DumpLogger.h"							// for CDumpLogger
#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/PacketListManager.h"
#include "../Include_Chart/Dll_Load/IndicatorList.h"
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "PacketRQ.h"
#include "PacketListManagerImp.h"

CXScaleManagerImp::CXScaleManagerImp(void)
{
	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	m_eChartMode = CChartInfo::HTS;

	// (2009/6/4 - Seung-Won, Bae) for ceiled Minute Scale with second time data.
	m_bCeiling = 0;

	// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
	m_nCountOfBlockColumn = 0;				// 컬럼갯수 : xScaleManager - ojtaso (20071128)
	for( int i = 0; i < _MAX_COL; i++)
	{
		m_naScreenRegionLeft[	i] = 0;
		m_naScreenRegionWidth[	i] = 0;
		m_daItemWidth[			i] = 1.0;
		m_naItemWidth[			i] = 1;
		m_pblaAreafDTI[			i] = NULL;
		m_naScreenStart_Cur[	i] = 0;
		m_naScreenEnd_Cur[		i] = 0;
		m_naScaleType[			i] = -1;
	}

	// (2009/2/12 - Seung-Won, Bae) Support default data space.
	m_nDefaultDataSpace = 0;

	// (2009/1/19 - Seung-Won, Bae) for Scale Time List
	m_nSkipFactor = 1;
	m_eScaleDrawType = CScaleBaseData::HORZ_NOTYPE;

	// (2009/1/16 - Seung-Won, Bae) Support the Re-Calculation Mode on OCX OnDrawing
	m_nCalculationMode = 2;			// 0:No-Calc, 1:CalcByReal, 2:CalcAll

	// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
	m_rctClient.SetRectEmpty();
	m_bOnResizing = FALSE;
	m_bOnUserZoomChanged = FALSE;
	m_dLastItemWidth = 0.0;

	// (2009/1/13 - Seung-Won, Bae) for ReCalc AREA
	m_nNeedCalcArea			= 0;		// the Flag on by DRI() with full(2) or partial(1)
	m_nDrawCount_Pre		= 0;		// the data count on view
	m_nDRI_Start_Pre		= -1;		// the first data index on view
	m_bWeightedVolume_Pre	= FALSE;	// the Weighted Volume option.
	m_bReverse_Pre			= FALSE;	// the Reverse X option.
	m_nScreenLeft_Pre		= -1;		// the graph area left position on view
	m_nScreenWidth_Pre		= 0;		// the graph area width on view
	m_daItemWidth_Pre		= 0;		// the one data width on view

	// (2009/1/8 - Seung-Won, Bae) Manage the flags for recalc AREA
	m_bWholeViewPre = FALSE;

	// (2008/12/29 - Seung-Won, Bae) Manage previous number of RQ to delete.
	m_nCountOfRQ_Area = 0;	// It is previouis number of RQ for ReInitData_Area().

	// (2008/12/28 - Seung-Won, Bae) Manage temp pointer variable to check NULL from the CDataBlockList's NULL return.
	m_pTReturn = NULL;

	m_bApplyToolBar = TRUE;				// 툴바에 계산한 내용을 반영하도록 합니다.

	m_bReverse = FALSE;					// 뒤집기
	m_bWeightedVolume = FALSE;			// 거래량 가중치
	m_bUseOEBTMargin = FALSE;			// 일목균형표 전용여백(사용여부)
	m_nFutureTimeCount = 0;					// 미래영역 확보
	m_nLeftDataHideCount = 0;			// 좌측 데이터 숨기기
	m_nRightMarginWidth = 0;			// 우측 여백(pixel)
	m_nMinDataCountOnOnePage = 5;		// 최소 Count

	m_rstype = RSAT_ITEM_COUNT_FIX;		// 리사이즈 봉(ITEM) 처리 방법	- [봉(ITEM)의 개수 고정 기능]

	// (2009/2/8 - Seung-Won, Bae) Set initial of Fixed Interval to false. and Do not load previous user setting.
	m_bFixedInterval = FALSE;			// 봉간격 일정 : xScaleManager - ojtaso (20070528)
	m_nDRI_Start_Cur = 0;				// 그리기 시작 위치
	m_nDRI_End_Cur = 0;
	m_nDrawCount_Cur = 0;				// 그리기 갯수
	m_nItemCount_Total = 0;

	m_areaNONE = AREA(-5000, -5000, -5000);

	m_nCountOfRQ = 0;

	m_minScaleInterval_nRQ = 0;
	m_minScaleInterval_eType = CScaleBaseData::HORZ_DAILY;
	m_minScaleInterval_nUnit = 1;

	m_maxScaleInterval_nRQ = 0;
	m_maxScaleInterval_eType = CScaleBaseData::HORZ_DAILY;
	m_maxScaleInterval_nUnit = 1;

	// (2008/12/29 - Seung-Won, Bae) Do not use m_mifOEBT. Use m_pifaOEBT.
	//	m_mifOEBT.clear();

	m_pblaRQTime = NULL;
	m_pblaDRIfDTI = NULL;
	m_pblaDTIfDRI = NULL;
	m_pifaRQ = NULL;
	m_pifaOEBT = NULL;
	m_pamapSpecialTime = NULL;

	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
	m_TimeDiffManager.SetXScaleManagerImp( this);
	m_bTimeScale = false;

	m_nMinMaxRatio = 0;
}

CXScaleManagerImp::~CXScaleManagerImp(void)
{
	ClearData();
}

void CXScaleManagerImp::SetPacketListManager(CPacketListManager* ppacketListManager)
{
	m_ppacketListManager = ppacketListManager;
}

void CXScaleManagerImp::SetIndicatorList(CIndicatorList* pIndicatorList)
{
	m_pIndicatorList = pIndicatorList;
}

// 툴바에 반영할지의 여부를 임시 기록하기 위해서 사용
void CXScaleManagerImp::SetApplyToolBar(BOOL bApplyToolBar)
{
	m_bApplyToolBar = bApplyToolBar;
}

BOOL CXScaleManagerImp::GetApplyToolBar()
{
	return m_bApplyToolBar;
}

// 뒤집기(REVERSE) 기능
void CXScaleManagerImp::SetReverse( BOOL bReverse)
{
	if( !m_pIChartCtrl->IsRunningOneChart())
	{
		if(m_bReverse == TRUE && bReverse == FALSE)
			for( int i = 0; i < m_nCountOfBlockColumn; i++)
				m_nScreenLeft_Cur[ i] -= m_nRightMarginWidth;
		else if(m_bReverse == FALSE && bReverse == TRUE)
			for( int i = 0; i < m_nCountOfBlockColumn; i++)
				m_nScreenLeft_Cur[ i] += m_nRightMarginWidth;
	}

	m_bReverse = bReverse;
}

BOOL CXScaleManagerImp::GetReverse()
{
	return m_bReverse;
}

// 거래량 가중치 기능
void CXScaleManagerImp::SetWeightedVolume(BOOL bWeightedVolume, BOOL bCalc)
{
	m_bWeightedVolume = bWeightedVolume;
}

BOOL CXScaleManagerImp::GetWeightedVolume()
{
	return m_bWeightedVolume;
}

// 미래영역 확보 기능
/*
	DRI 데이터 영역에 가지고 있는 것이 아니라 화면을 그릴려고 할때 반영해 주도록 합니다. 
	해서 별도로 데이터를 가공하지 않도록 합니다.
*/
void CXScaleManagerImp::SetFutureTimeCount(int nFutureTimeCount)
{
	m_nFutureTimeCount = nFutureTimeCount;
}

int CXScaleManagerImp::GetFutureTimeCount()
{
	return m_nFutureTimeCount;
}

// 좌측 데이터 숨기기 기능
void CXScaleManagerImp::SetLeftDataHideCount(int nLeftDataHideCount, BOOL bCalc)
{
	m_nLeftDataHideCount = nLeftDataHideCount;
	// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI
	if( bCalc) Calc_DRI();
}

int CXScaleManagerImp::GetLeftDataHideCount()
{
	return m_nLeftDataHideCount;
}

// 일목균형표 전용여백(사용여부)
BOOL CXScaleManagerImp::GetUseOEBTMargin()
{
	return m_bUseOEBTMargin;
}

void CXScaleManagerImp::SetUseOEBTMargin(BOOL bUseOEBTMargin)
{
	m_bUseOEBTMargin = bUseOEBTMargin;
}

// 우측 여백 기능
void CXScaleManagerImp::SetRightMarginWidth(int nRightMarginWidth)
{
	m_nRightMarginWidth = nRightMarginWidth * 8;

	// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
	m_rctClient.InflateRect( 1, 1);

	for( int i = 0; i < m_nCountOfBlockColumn; i++)
		SetScreenRegion( i, m_naScreenRegionLeft[ i], m_naScreenRegionWidth[ i], TRUE);
}

// (2009/5/17 - Seung-Won, Bae) for Right Margin and Future Time
void CXScaleManagerImp::SetRightMarginAndFutureTime( int p_nRightMarginWidth, int p_nFutureTimeCount)
{
	m_nRightMarginWidth = p_nRightMarginWidth * 8;
	m_nFutureTimeCount = p_nFutureTimeCount;
}

// 우측 여백 동작 방식 설정
void CXScaleManagerImp::SetCandleTypeOnResize( RSA_TYPE rstype)
{
	m_rstype = rstype;
	if( m_nCountOfBlockColumn > 1) m_rstype = RSAT_ITEM_COUNT_FIX;
}

CXScaleManager::RSA_TYPE CXScaleManagerImp::GetCandleTypeOnResize()
{
	return m_rstype;
}

void CXScaleManagerImp::AddOEBTRQ(CString strRQ, CString strGraphName, CString strIndicatorName)
{
	// 선행 Span값을 가져온다.
	CIndicatorInfo* pInfo = m_pIndicatorList->GetGraphIndicatorInfo(EILT_USER, strGraphName, strIndicatorName, EIIGT_WITHOUT_DEFAULT);
	if(pInfo == NULL) return;

	CList<double, double>* pList = pInfo->GetCommonCalConditionList();
	if(pList == NULL) return;

	POSITION pos = pList->GetHeadPosition();
	if(pos == NULL) return;

	POEBT_INFO poebtinfo = new OEBT_INFO;
	poebtinfo->strGraphName = strGraphName;

	double dCalcCond = 0;
	dCalcCond = pList->GetNext(pos);
	poebtinfo->nFutureCount = int(dCalcCond);

	int nRQ = GetRQIndexFromString( strRQ);
	if( 0 <= nRQ) m_pifaOEBT[ nRQ] = poebtinfo;
	else delete poebtinfo;
}

void CXScaleManagerImp::DelOEBTRQ( CString strRQ)
{
	int nRQ = GetRQIndexFromString( strRQ);
	if( 0 <= nRQ)
	{
		delete m_pifaOEBT[ nRQ];
		m_pifaOEBT[ nRQ] = NULL;
	}
}

void CXScaleManagerImp::Clear_OEBTRQ()
{
	// (2008/12/29 - Seung-Won, Bae) Do not use m_mifOEBT. Use m_pifaOEBT.
	//	MAP_RQ_OEBT::iterator it = m_mifOEBT.begin();
	//	while(it != m_mifOEBT.end())
	//	{
	//		delete it->second;
	//		it++;
	//	}
	//	m_mifOEBT.clear();
	for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) if( m_pifaOEBT[ nRQ])
	{
		delete m_pifaOEBT[ nRQ];
		m_pifaOEBT[ nRQ] = NULL;
	}
}

// (2008/12/28 - Seung-Won, Bae) Comment.
void CXScaleManagerImp::Calc_DRI( BOOL p_bWithMerge)
{
	// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
	if( m_bTruncatedByReal) m_bTruncatedByReal = FALSE;

	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.  <<
	int nMarketStartTime, nMarketEndTime;
	m_TimeDiffManager.InitRQDiffTime();
	m_TimeDiffManager.InitRQMarketTime();
	m_bTimeScale = false;
	//>>

	// (2009/1/13 - Seung-Won, Bae) for ReCalc AREA
	m_nNeedCalcArea = 2;

	// 1. Destroy and Rebuild RQ Infos.
	// 1.1 Destroy All Data
	// 1.1.1 Destroy All RQ Data.
	m_midxRQ.clear();
	if(m_pifaRQ != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pifaRQ[ nRQ];
		delete [] m_pifaRQ;
		m_pifaRQ = NULL;
	}
	// 1.1.2 Destroy Data indexed DTI
	if( m_pblaRQTime != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pblaRQTime[ nRQ];
		delete [] m_pblaRQTime;
		m_pblaRQTime = NULL;
	}
	if( m_pblaDRIfDTI != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pblaDRIfDTI[ nRQ];
		delete [] m_pblaDRIfDTI;
		m_pblaDRIfDTI = NULL;
	}
	// 1.1.3 Destroy Data indexed DRI
	if( m_pblaDTIfDRI != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pblaDTIfDRI[ nRQ];
		delete [] m_pblaDTIfDRI;
		m_pblaDTIfDRI = NULL;
	}
	// 1.1.4 Destroy Data special time.
	if( m_pamapSpecialTime != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pamapSpecialTime[ nRQ];
		delete [] m_pamapSpecialTime;
		m_pamapSpecialTime = NULL;
	}
	// 1.2 Clear Data.
	// 1.2.1 Clear main time line.
	// X축 시간 정보 지우기
	m_blTimefDRI.Clear();
	// 1.2.2 Init min/max Scale Interval Info.
	m_minScaleInterval_nRQ = 0;
	m_maxScaleInterval_nRQ = 0;
	// 1.4 Create All Data (reverse order)
	// 1.4.0 Check no RQs.
	MAP_RQ_PACKETLIST* pmapPacketList = ( MAP_RQ_PACKETLIST*)m_ppacketListManager->GetMapPacketList();
	m_nCountOfRQ = pmapPacketList->size();
	if( 0 == m_nCountOfRQ)
	{
		for( int nRQ = 0; nRQ < m_saRQ.GetSize(); nRQ++)
			if( m_pifaOEBT[ nRQ]) delete m_pifaOEBT[ nRQ];
		delete [] m_pifaOEBT;
		m_pifaOEBT = NULL;
		m_saRQ.RemoveAll();
		return;
	}
	// 1.4.1 Create Data indexed DRI
	m_pblaDTIfDRI = (CDataBlockList<int>**)(new LPVOID[m_nCountOfRQ]);
	// 1.4.2 Create Data indexed DTI
	m_pblaRQTime = (CDataBlockList<time_t>**)(new LPVOID[m_nCountOfRQ]);
	m_pblaDRIfDTI = (CDataBlockList<int>**)(new LPVOID[m_nCountOfRQ]);
	m_pamapSpecialTime = ( CMapTimeToLong **)( new LPVOID[ m_nCountOfRQ]);
	// 1.4.3 Create all RQ Data
	m_pifaRQ = (RQ_INFO**)(new LPVOID[m_nCountOfRQ]);
	MAP_RQ_PACKETLIST::iterator it_packetRQ = pmapPacketList->begin();
	CStringArray saRQ;
	int nRQ = 0;
	for( nRQ = 0; it_packetRQ != pmapPacketList->end(); nRQ++)
	{
		m_midxRQ.insert( PAIR_RQ_INDEX( it_packetRQ->first, nRQ));
		saRQ.Add( CString( it_packetRQ->first.c_str()));
		
		m_pblaRQTime[nRQ] = new CDataBlockList<time_t>;
		m_pblaDRIfDTI[nRQ] = new CDataBlockList<int>;
		m_pblaDTIfDRI[nRQ] = new CDataBlockList<int>;
		m_pamapSpecialTime[ nRQ] = new CMapTimeToLong;

		m_pifaRQ[ nRQ] = new RQ_INFO;
		m_pifaRQ[ nRQ]->pPacketRQ = it_packetRQ->second;
		CPacketList *pPacketList = it_packetRQ->second->GetPacketList();
		m_pifaRQ[ nRQ]->pPacketList = pPacketList;
		m_pifaRQ[ nRQ]->pTimePacket = pPacketList->GetBaseData( it_packetRQ->second->GetPacketName());
		m_pifaRQ[ nRQ]->bRealReceived = TRUE;		// for Full calculation with Real Processing Logic.
		m_pifaRQ[ nRQ]->nLastRealDTI = -1;
		m_pifaRQ[ nRQ]->nUpdatedRealDRI = -1;
		m_pifaRQ[ nRQ]->eScaleUnit = pPacketList->GetDateUnitType();
		m_pifaRQ[ nRQ]->nScaleInterval_Unit = pPacketList->GetTimeInterval();
		// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
		m_pifaRQ[ nRQ]->m_bStringDataType = TRUE;
		if( m_pifaRQ[ nRQ]->pTimePacket)
			m_pifaRQ[ nRQ]->m_bStringDataType = ( !m_pifaRQ[ nRQ]->pTimePacket->GetnumericDataList()
													&& !m_pifaRQ[ nRQ]->pTimePacket->GetType().Compare( _MTEXT( C6_CHARACTER)));
		if( m_pifaRQ[ nRQ]->nScaleInterval_Unit <= 0) m_pifaRQ[ nRQ]->nScaleInterval_Unit = 1;	// 최소가 0인경우 1로 변경 : xScaleManager - ojtaso (20070709)
		m_pifaRQ[ nRQ]->nScaleInterval_Sec = 1;		// for Tick or ETC

		// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		CString strType;
		if (m_pifaRQ[ nRQ]->pTimePacket)
			strType = m_pifaRQ[ nRQ]->pTimePacket->GetType();
		// 2011.01.24 by SYS <<

		switch( m_pifaRQ[ nRQ]->eScaleUnit)
		{
			case CScaleBaseData::HORZ_MONTHLY:		m_pifaRQ[ nRQ]->nScaleInterval_Sec = (60 * 60 * 24 * 31) * m_pifaRQ[ nRQ]->nScaleInterval_Unit;
													break;
			case CScaleBaseData::HORZ_WEEKLEY:		m_pifaRQ[ nRQ]->nScaleInterval_Sec = (60 * 60 * 24 * 7) * m_pifaRQ[ nRQ]->nScaleInterval_Unit;
													break;
			case CScaleBaseData::HORZ_DAILY:		
													// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
													//if (strType == _T("YYYYMMDDHHMMSS"))
													//{
													//	__int64 nTime1 = __int64 (60 * 60 * 24) * m_pifaRQ[ nRQ]->nScaleInterval_Unit;
													//	m_pifaRQ[ nRQ]->nScaleInterval_Sec = __int64(nTime1 * 1000000);
													//}
									
				//else
													// 2011.01.24 by SYS <<
													m_pifaRQ[ nRQ]->nScaleInterval_Sec = (60 * 60 * 24) * m_pifaRQ[ nRQ]->nScaleInterval_Unit;
													break;
			case CScaleBaseData::HORZ_TIME_ONE:	
			case CScaleBaseData::HORZ_TIME_TWO:		if( m_pifaRQ[ nRQ]->pTimePacket)
														if( m_pifaRQ[ nRQ]->pTimePacket->GetType().Find( "SS") < 0)
															m_pifaRQ[nRQ]->nScaleInterval_Unit *= 60; 
														else 
															m_pifaRQ[nRQ]->nScaleInterval_Unit = ( m_pifaRQ[nRQ]->nScaleInterval_Unit / 100) * 60 + m_pifaRQ[nRQ]->nScaleInterval_Unit % 100;
													m_pifaRQ[ nRQ]->nScaleInterval_Sec = m_pifaRQ[ nRQ]->nScaleInterval_Unit;
													// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.
													m_bTimeScale = true;
													break;
			case CScaleBaseData::HORZ_TIME_SECOND:	m_pifaRQ[ nRQ]->nScaleInterval_Sec = m_pifaRQ[ nRQ]->nScaleInterval_Unit;
													break;
		}

		// (2009/5/15 - Seung-Won, Bae) Init with 0th RQ for Min/Max
		if( 0 == nRQ)
		{
			m_minScaleInterval_eType = m_maxScaleInterval_eType = m_pifaRQ[ nRQ]->eScaleUnit;
			m_minScaleInterval_nUnit = m_maxScaleInterval_nUnit = m_pifaRQ[ nRQ]->nScaleInterval_Unit;
		}

		// 1.4.4 Get min/max Scale Interval Info.
		if( m_pifaRQ[ m_minScaleInterval_nRQ]->nScaleInterval_Sec > m_pifaRQ[ nRQ]->nScaleInterval_Sec
			|| ( m_pifaRQ[ m_minScaleInterval_nRQ]->nScaleInterval_Sec == m_pifaRQ[ nRQ]->nScaleInterval_Sec
				&& ( ( m_pifaRQ[ m_minScaleInterval_nRQ]->eScaleUnit != CScaleBaseData::HORZ_TICK
						&& m_pifaRQ[ m_minScaleInterval_nRQ]->nScaleInterval_Unit < m_pifaRQ[ nRQ]->nScaleInterval_Unit)
					|| ( m_pifaRQ[ m_minScaleInterval_nRQ]->eScaleUnit == CScaleBaseData::HORZ_TICK
						&& m_pifaRQ[ m_minScaleInterval_nRQ]->nScaleInterval_Unit > m_pifaRQ[ nRQ]->nScaleInterval_Unit))))
		{
			m_minScaleInterval_nRQ = nRQ;
			m_minScaleInterval_eType = m_pifaRQ[ nRQ]->eScaleUnit;
			m_minScaleInterval_nUnit = m_pifaRQ[ nRQ]->nScaleInterval_Unit;
		}
		if( m_pifaRQ[ m_maxScaleInterval_nRQ]->nScaleInterval_Sec < m_pifaRQ[ nRQ]->nScaleInterval_Sec
			|| ( m_pifaRQ[ m_maxScaleInterval_nRQ]->nScaleInterval_Sec == m_pifaRQ[ nRQ]->nScaleInterval_Sec
				&& ( ( m_pifaRQ[ m_maxScaleInterval_nRQ]->eScaleUnit != CScaleBaseData::HORZ_TICK
						&& m_pifaRQ[ m_maxScaleInterval_nRQ]->nScaleInterval_Unit > m_pifaRQ[ nRQ]->nScaleInterval_Unit)
					|| ( m_pifaRQ[ m_maxScaleInterval_nRQ]->eScaleUnit == CScaleBaseData::HORZ_TICK
						&& m_pifaRQ[ m_maxScaleInterval_nRQ]->nScaleInterval_Unit < m_pifaRQ[ nRQ]->nScaleInterval_Unit))))
		{
			m_maxScaleInterval_nRQ = nRQ;
			m_maxScaleInterval_eType = m_pifaRQ[ nRQ]->eScaleUnit;
			m_maxScaleInterval_nUnit = m_pifaRQ[ nRQ]->nScaleInterval_Unit;
		}

		// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. <<
		m_TimeDiffManager.SetRQDiffTime( saRQ.GetAt(nRQ), pPacketList->GetBaseTimeDifference(), pPacketList->GetCurrDateTime());
		if( pPacketList->GetMarketTime( nMarketStartTime, nMarketEndTime) )
			m_TimeDiffManager.SetRQMarketTime( saRQ.GetAt(nRQ), nMarketStartTime, nMarketEndTime, pPacketList->GetMarketDayType());
		//>>

		it_packetRQ++;
	}

	// (2009/9/13 - Seung-Won, Bae) for Rearrange of m_pifaOEBT
	OEBT_INFO** pifaOEBT = ( OEBT_INFO**)( new LPVOID[ m_nCountOfRQ]);
	for( nRQ = 0; nRQ < saRQ.GetSize(); nRQ++)
	{
		pifaOEBT[ nRQ] = NULL;
		for( int nOldRQ = 0; nOldRQ < m_saRQ.GetSize(); nOldRQ++)
			if( m_saRQ[ nOldRQ] == saRQ[ nRQ])
			{
				pifaOEBT[ nRQ] = m_pifaOEBT[ nOldRQ];
				m_pifaOEBT[ nOldRQ] = NULL;
			}
	}
	for( nRQ = 0; nRQ < m_saRQ.GetSize(); nRQ++)
		if( m_pifaOEBT[ nRQ]) delete m_pifaOEBT[ nRQ];
	delete [] m_pifaOEBT;
	m_pifaOEBT = pifaOEBT;
	m_saRQ.RemoveAll();
	m_saRQ.Append( saRQ);

	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.
	m_TimeDiffManager.SetCurrentRQ( m_pIChartCtrl->GetCurrentRQ());	// 현재 스케일의 기준이 되는 RQ 셋팅

	// 2. Cacl_DRI_Merge
	if( p_bWithMerge) Calc_DRI_Merge();

	// (2009/6/4 - Seung-Won, Bae) for ceiled Minute Scale with second time data.
	// (2009/6/15 - Seung-Won, Bae) Koscom is new ceiling type.
	m_bCeiling = ( m_minScaleInterval_nUnit % 60 == 0 && CPacketType::FLOOR != m_pifaRQ[ 0]->pPacketList->GetRealType());
}

void CXScaleManagerImp::Calc_DRI_Merge( void)
{
	// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
	int nItemCount_Prev = m_nItemCount_Total;
	if( m_bTruncatedByReal) Calc_DRI();

	// (2009/5/17 - Seung-Won, Bae) for Future Time Count of Compare Chart with OEBT.
	int nAddedFutureTimeCount = 0;

	// 2. Make the time line for All RQs.
	int nLastRealDRI = INT_MAX;
	int nRQ = 0;
	for( nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
	{
		if( !m_pifaRQ[ nRQ]->pTimePacket) continue;
		// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
		if( m_pifaRQ[ nRQ]->m_bStringDataType)
		{
			if( !m_pifaRQ[ nRQ]->bRealReceived) continue;
			// Clear Old DRI/DTI Info.
			m_pblaDRIfDTI[ nRQ]->DeleteBack( m_pblaDRIfDTI[ nRQ]->GetItemCount());

			CString strTime;
			time_t timeCur = 0;
			CList<CString, CString>* plTime = m_pifaRQ[ nRQ]->pTimePacket->GetstrDataList();
			POSITION posTime = plTime->GetHeadPosition();
			while( posTime)
			{
				strTime = plTime->GetNext( posTime);
				timeCur++;
				m_pblaRQTime[ nRQ]->PushBack( timeCur);
			}

			m_pifaRQ[ nRQ]->bRealReceived = FALSE;
		}
		else
		{
			// 시간 데이터를 만들어 줍니다.
			CList<double, double>* plTime = m_pifaRQ[ nRQ]->pTimePacket->GetnumericDataList();
			if( !plTime) return;
			CString strTimeType = m_pifaRQ[ nRQ]->pTimePacket->GetType();

			// (2009/5/17 - Seung-Won, Bae) for Future Time Count of Compare Chart with OEBT.
			// 1. Get Time Interval to add the future time.
			
			// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
			//int nScaleInterval_Sec = m_pifaRQ[ nRQ]->nScaleInterval_Sec;
			__int64 nScaleInterval_Sec = m_pifaRQ[ nRQ]->nScaleInterval_Sec;
			// 2011.01.24 by SYS <<

			if( m_pifaRQ[ nRQ]->eScaleUnit == CScaleBaseData::HORZ_TICK && 0 < m_minScaleInterval_nUnit)
				nScaleInterval_Sec = m_pifaRQ[ nRQ]->nScaleInterval_Unit / m_minScaleInterval_nUnit;

			// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
			CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = m_pifaRQ[ nRQ]->eScaleUnit;
			// 2011.01.27 by SYS <<

			if( m_pifaRQ[ nRQ]->bRealReceived)
			{
// (2009/3/10 - Seung-Won, Bae) Special Chart X Scale Error TEST
#ifdef _DEBUG
//				CString strLog;
//				CString strValue;
//
//				strValue.Format( "nPacketDataCount : %d\r\n", plTime->GetCount());
//				strLog += strValue;
//
//				strValue.Format( "nPreLastRealDTI : %d\r\n", m_pifaRQ[ nRQ]->nLastRealDTI);
//				strLog += strValue;
//
//				strValue.Format( "nPreRQTimeCount : %d\r\n", m_pblaRQTime[ nRQ]->GetItemCount());
//				strLog += strValue;
//
//				strValue.Format( "Added Time :\r\n");
//				strLog += strValue;
//
//				CStringList slValues;
#endif

				// (2009/3/11 - Seung-Won, Bae) Check exception for new small data of special chart.
				if( plTime->GetCount() <= m_pifaRQ[ nRQ]->nLastRealDTI)
				{
					m_pifaRQ[ nRQ]->nLastRealDTI = -1;
					m_pblaRQTime[ nRQ]->DeleteBack( m_pblaRQTime[ nRQ]->GetItemCount());
					m_pamapSpecialTime[ nRQ]->RemoveAll();
				}

				// Get the minimum DRI for update.
				// and delete last real time data (it maybe changed with new time)
				int nDRI = INT_MAX;
				if( 0 <= m_pifaRQ[ nRQ]->nLastRealDTI)
				{
					m_pIReturn = m_pblaDRIfDTI[ nRQ]->GetAt( m_pifaRQ[ nRQ]->nLastRealDTI);
					if( m_pIReturn) nDRI = *m_pIReturn;
					for( int i = m_pifaRQ[ nRQ]->nLastRealDTI; i < m_pblaRQTime[ nRQ]->GetItemCount(); i++)
					{
						m_pTReturn = m_pblaRQTime[ nRQ]->GetAt( i);
						if( m_pTReturn) m_pamapSpecialTime[ nRQ]->RemoveKey( *m_pTReturn);
					}
					// Delete time of Last Real DTI and after
					m_pblaRQTime[ nRQ]->DeleteBack( m_pblaRQTime[ nRQ]->GetItemCount() - m_pifaRQ[ nRQ]->nLastRealDTI);
				}
				if( nDRI < nLastRealDRI) nLastRealDRI = nDRI;

				// Generate new real time data.			
				POSITION posTime = NULL;
				if( 0 <= m_pifaRQ[ nRQ]->nLastRealDTI)	posTime = plTime->FindIndex( m_pifaRQ[ nRQ]->nLastRealDTI);
				else									posTime = plTime->GetHeadPosition();
				time_t timeCur = 0;
				double dTimeCur = 0.0;
				tm tmResult = { 0, 0, 0, 0, 0, 0, 0, 0, -1};	//[A00000554]alzioyes:20110530 시간셋팅 초기값변경.
				if( 0 < m_pifaRQ[ nRQ]->nLastRealDTI)
				{
					m_pTReturn = m_pblaRQTime[ nRQ]->GetAt( m_pifaRQ[ nRQ]->nLastRealDTI - 1);
					if( m_pTReturn)
					{
						tm *ptm = localtime( m_pTReturn);
						if( ptm) tmResult = *ptm;
					}
				}

				while( posTime)
				{
					dTimeCur = plTime->GetNext( posTime);

// (2009/3/10 - Seung-Won, Bae) Special Chart X Scale Error TEST
#ifdef _DEBUG
//					strValue.Format( "\t%f\r\n", dTimeCur);
//					slValues.AddTail( strValue);
#endif
					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.

					// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
					//timeCur = GetTimeFromTimeValue( ( __int64)dTimeCur, strTimeType, m_pifaRQ[ nRQ], tmResult, m_saRQ.GetAt( nRQ), m_pamapSpecialTime[ nRQ]);
					timeCur = GetTimeFromTimeValue( ( __int64)dTimeCur, strTimeType, m_pifaRQ[ nRQ], tmResult, m_saRQ.GetAt( nRQ), m_pamapSpecialTime[ nRQ]);
					// 2011.01.27 by SYS <<

					m_pblaRQTime[ nRQ]->PushBack( timeCur);
				}
				m_pifaRQ[ nRQ]->bRealReceived = FALSE;
				m_pifaRQ[ nRQ]->nLastRealDTI = m_pblaRQTime[ nRQ]->GetItemCount() - 1;


				// (2009/5/28 - Seung-Won, Bae) the future can be steady special time.
				m_pifaRQ[ nRQ]->m_lEndSpecialTimeCode = 0;
				if( 0 <= m_pifaRQ[ nRQ]->nLastRealDTI)
				{
					m_pTReturn = m_pblaRQTime[ nRQ]->GetAt( m_pifaRQ[ nRQ]->nLastRealDTI);
					if( m_pTReturn) if( !m_pamapSpecialTime[ nRQ]->Lookup( *m_pTReturn, m_pifaRQ[ nRQ]->m_lEndSpecialTimeCode))
						m_pifaRQ[ nRQ]->m_lEndSpecialTimeCode = 0;
				}

				// (2009/5/17 - Seung-Won, Bae) for Future Time Count of Compare Chart with OEBT.
				// 2. Get the future time count of OEBT.
				if( m_pifaOEBT[ nRQ])
				{
					for( int nCount = 0; nCount < m_pifaOEBT[ nRQ]->nFutureCount; nCount++)
					{
						timeCur += nScaleInterval_Sec;
						m_pblaRQTime[ nRQ]->PushBack( timeCur);

						// (2009/5/28 - Seung-Won, Bae) the future can be steady special time.
						// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
						if( m_pifaRQ[ nRQ]->m_lEndSpecialTimeCode && CChartInfo::WORLD_ON != m_eChartMode)
							m_pamapSpecialTime[ nRQ]->SetAt( timeCur, m_pifaRQ[ nRQ]->m_lEndSpecialTimeCode);
					}
				}
			}

// (2009/3/10 - Seung-Won, Bae) Special Chart X Scale Error TEST
#ifdef _DEBUG
//			int nLen = 0;
//			POSITION psn = slValues.GetHeadPosition();
//			while( psn)
//			{
//				CString &strValues2 = slValues.GetNext( psn);
//				nLen += strValues2.GetLength();
//			}
//			char *szValues = strValue.GetBuffer( nLen);
//			psn = slValues.GetHeadPosition();
//			while( psn)
//			{
//				CString &strValues2 = slValues.GetNext( psn);
//				strcpy( szValues, strValues2);
//				szValues += strValues2.GetLength();
//			}
//			strValue.ReleaseBuffer();
//			strLog += strValue;
//
//			strValue.Format( "nNewLastRealDTI : %d\r\n", m_pifaRQ[ nRQ]->nLastRealDTI);
//			strLog += strValue;
//
//			strValue.Format( "nNewRQTimeCount : %d\r\n", m_pblaRQTime[ nRQ]->GetItemCount());
//			strLog += strValue;
//
//			CString strLogKey;
//			strLogKey.Format( "XScale[%08X]", ( long)this);
//
//			CDumpLogger dLogger( strLogKey);
//			dLogger.WriteToStringLog( "Calc_DRI_Merge", strLog, strLog.GetLength());
#endif

			// (2009/5/17 - Seung-Won, Bae) for Future Time Count of Compare Chart with OEBT.
			// 3. Get the future time count with user setting.
			if( nRQ == m_maxScaleInterval_nRQ && !m_pIChartCtrl->IsRunningOneChart())
			{
				nAddedFutureTimeCount = m_nFutureTimeCount;
				if( m_pifaOEBT[ nRQ])
				{
					if( nAddedFutureTimeCount < m_pifaOEBT[ nRQ]->nFutureCount)
							nAddedFutureTimeCount = 0;
					else	nAddedFutureTimeCount -= m_pifaOEBT[ nRQ]->nFutureCount;
				}
				if( m_pblaRQTime[ nRQ]->GetItemCount() + nAddedFutureTimeCount < m_nDefaultDataSpace)
					nAddedFutureTimeCount = m_nDefaultDataSpace - m_pblaRQTime[ nRQ]->GetItemCount();
				// 4. Add Future Time.
				if( 0 < nAddedFutureTimeCount)
				{
					time_t timeCur = 0;
					if( 0 < m_pblaRQTime[ nRQ]->GetItemCount())
					{
						m_pTReturn = m_pblaRQTime[ nRQ]->GetAt( m_pblaRQTime[ nRQ]->GetItemCount() - 1);
						if( m_pTReturn) timeCur = *m_pTReturn;
					}
					else	time( &timeCur);
#ifdef _DEBUG
//tm *pTM = localtime( &timeCur);
#endif
					for( int nCount = 0; nCount < nAddedFutureTimeCount; nCount++)
					{
						timeCur += nScaleInterval_Sec;
#ifdef _DEBUG
//tm *pTM = localtime( &timeCur);
#endif
						m_pblaRQTime[ nRQ]->PushBack( timeCur);

						// (2009/5/28 - Seung-Won, Bae) the future can be steady special time.
						// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
						if( m_pifaRQ[ nRQ]->m_lEndSpecialTimeCode && CChartInfo::WORLD_ON != m_eChartMode)
							m_pamapSpecialTime[ nRQ]->SetAt( timeCur, m_pifaRQ[ nRQ]->m_lEndSpecialTimeCode);
					}
				}
			}
		}
	}

	// (2010/4/4 - Seung-Won, Bae) for Ceiling Minute Merge Mode.
	nLastRealDRI = INT_MAX;

	// Delete main time DRI for last real DRI.
	if( nLastRealDRI == INT_MAX) nLastRealDRI = 0;
	else if( m_blTimefDRI.GetItemCount() <= nLastRealDRI) return;
	m_blTimefDRI.DeleteBack( m_blTimefDRI.GetItemCount() - nLastRealDRI);

	int *naDTI_Cur = new int[ m_nCountOfRQ];
	for( nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
	{
		// naDTI_Cur is the Index array of the DTI that merged completly.
		if( 0 <= nLastRealDRI - 1 && nLastRealDRI - 1 < m_pblaDTIfDRI[ nRQ]->GetItemCount())
		{
				m_pIReturn = m_pblaDTIfDRI[ nRQ]->GetAt( nLastRealDRI - 1);
				if( m_pIReturn)
				{
					naDTI_Cur[ nRQ] = *m_pIReturn;

					// Delete ALL DTItoDRI with LastRealDRI.
					if( -1 <= naDTI_Cur[ nRQ])
						m_pblaDRIfDTI[ nRQ]->DeleteBack( m_pblaDRIfDTI[ nRQ]->GetItemCount() - ( naDTI_Cur[ nRQ] + 1));
				}
		}
		else
		{
			naDTI_Cur[ nRQ] = -1;
			// (2009/3/4 - Seung-Won, Bae) Delete End Mark.
			m_pblaDRIfDTI[ nRQ]->DeleteBack( m_pblaDRIfDTI[ nRQ]->GetItemCount());
		}

		m_pblaDTIfDRI[ nRQ]->DeleteBack( m_pblaDTIfDRI[ nRQ]->GetItemCount() - nLastRealDRI);
	}

	BOOL bNewTime = FALSE;
	time_t timeCur = 0;
	// 3. Do merge to the full time line.
	int nIRQ_Count = 0;
	int* naIRQ	= new int[ m_nCountOfRQ];	// 현재 시점과 일치하는(같은 시간) RQ들을 가지고 있습니다.
	memset( naIRQ, 0, sizeof( int) * m_nCountOfRQ);
	int nRQi = 0;
	int nDRI = 0;
	// 3.1 Loop with increasing DRI until the Index(naDTI_Cur) of All RQs is end.
	for(  nDRI = nLastRealDRI; !EndOfRQs( naDTI_Cur); nDRI++)
	{
		// 3.1.1 Get the minimum time value between the next time value of all RQs.
		//		and Increase the DTI of RQs that have the minimum time value.
		//		and naIRQ has the RQ Index that have the minimum time value.
		//		and nIRQ_Count is number of RQs that have the minimum time value.
		timeCur = GetLeastTime( naDTI_Cur, naIRQ, &nIRQ_Count);

		// 3.1.2 Skip Left Hide.
		// 좌측 데이터 숨기기 적용
		if( nDRI < m_nLeftDataHideCount)
		{
			// 현재 보이지 않는 경우에는 -1 로 채워 주도록 합니다.
			for( int nCount = 0; nCount < nIRQ_Count; nCount++)
				m_pblaDRIfDTI[ naIRQ[ nCount]]->PushBack( -1);
			continue;
		}

		// 3.1.3 Add data to the full time line for new DRI
		m_blTimefDRI.PushBack( timeCur);

		// 3.1.4 Add DTI for new DRI
		// DTI From DRI 설정
		for( nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
			// (2010/4/4 - Seung-Won, Bae) for Ceiling Minute Merge Mode.
			// 1. Check Minute
			switch( m_pifaRQ[ nRQ]->eScaleUnit)
		{
			case CScaleBaseData::HORZ_TIME_ONE:			// 시간-한줄
			case CScaleBaseData::HORZ_TICK:				// 틱
			case CScaleBaseData::HORZ_TIME_TWO:			// 시간-두줄
			case CScaleBaseData::HORZ_TIME_SECOND:		// 초
			{
						// if it is matched. naDTI_Cur[ nRQ] was increased already.
						BOOL bMatched = FALSE;
						for( nRQi = 0; nRQi < nIRQ_Count; nRQi++) if( nRQ == naIRQ[ nRQi]) bMatched = TRUE;
						m_pblaDTIfDRI[ nRQ]->PushBack( naDTI_Cur[ nRQ] + ( bMatched || m_pblaRQTime[ nRQ]->GetItemCount() == 0 ? 0 : 1));
						break;
			}
			default:	// 3.1.4 Add DTI for new DRI
						// DTI From DRI 설정
						m_pblaDTIfDRI[ nRQ]->PushBack( naDTI_Cur[ nRQ]);
						break;
		}

		// 3.1.5 Set DRI for matched time DTI.
		// 현재 시점과 일치하는(같은 시간) RQ들의 경우에는 해당 데이터 위치, 그리기 위치를 저장해 줍니다.
		// DRI From DTI 설정
		for( nRQi = 0; nRQi < nIRQ_Count; nRQi++)
			// (2010/4/4 - Seung-Won, Bae) for Ceiling Minute Merge Mode.
			// 1. Check Minute
			switch( m_pifaRQ[ naIRQ[ nRQi]]->eScaleUnit)
		{
			case CScaleBaseData::HORZ_TIME_ONE:			// 시간-한줄
			case CScaleBaseData::HORZ_TICK:				// 틱
			case CScaleBaseData::HORZ_TIME_TWO:			// 시간-두줄
			case CScaleBaseData::HORZ_TIME_SECOND:		// 초
						// if it is matched. push the next DRI for next DTI.
						if( m_pblaDRIfDTI[ naIRQ[ nRQi]]->GetItemCount() == 0)
							m_pblaDRIfDTI[ naIRQ[ nRQi]]->PushBack( nDRI - m_nLeftDataHideCount);
						m_pblaDRIfDTI[ naIRQ[ nRQi]]->PushBack( nDRI - m_nLeftDataHideCount + 1);
						break;
			default:	m_pblaDRIfDTI[ naIRQ[ nRQi]]->PushBack( nDRI - m_nLeftDataHideCount);
						break;
		}
	}
	delete [] naIRQ;

	// 3.3 Set time item count of the full time line. (DRI counts)
	m_nItemCount_Total = m_blTimefDRI.GetItemCount();

	// (2009/8/15 - Seung-Won, Bae) After Calc_DRI, we need to calculate the Item Width.
	for( int i = 0; i < _MAX_COL; i++) m_naScreenRegionWidth[ i] = 0;

	// 3.4 Add End Mark DTI for Calc_AREA()
	//			Cacl_Area() calculate the Area from start DTI to end DTI of view with next DTI.
	//		and Set DTI or none-DTI mark for future DRI
	//		but Do not add time item to a time line of RQs.
	// DTI의 마지막 아이템(봉)의 폭을 구하기 위해서 가짜 DTI를 하나 만들어 줍니다.
	for( nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
	{
		// (2009/5/17 - Seung-Won, Bae) for Future Time Count of Compare Chart with OEBT.
		//		Remove future time count data.
		if( nRQ == m_maxScaleInterval_nRQ && 0 < nAddedFutureTimeCount)
		{
			m_pblaRQTime[ nRQ]->DeleteBack( nAddedFutureTimeCount);
			m_pblaDRIfDTI[ nRQ]->DeleteBack( nAddedFutureTimeCount);
			naDTI_Cur[ nRQ] = m_pblaDRIfDTI[ nRQ]->GetItemCount() - 1;
		}

		// 3.4.1 Add end-mark DTI.
		m_pblaDRIfDTI[ nRQ]->PushBack( -1);

		// 3.4.2 Get last and next DRI for future.
		int nDTI_Count = m_pblaDRIfDTI[ nRQ]->GetItemCount();
		if( nDTI_Count == 1) continue;
		int nDRI_Last = -1;
		m_pIReturn = m_pblaDRIfDTI[ nRQ]->GetAt( nDTI_Count - 2);
		if( m_pIReturn) nDRI_Last = *m_pIReturn;
		if( nDRI_Last == -1) continue;
		int nDRI_Next = GetNextDRI_ByScaleInterval( m_pifaRQ[ nRQ]->eScaleUnit, m_pifaRQ[ nRQ]->nScaleInterval_Unit, nDRI_Last);

		// 3.4.3 Reset end-mark DTI with future DRI.
		//		DRI of end-mark can be nDRI_Last + 1 (no future) or m_nItemCount_Total (small future).
		m_pblaDRIfDTI[ nRQ]->SetAt( nDTI_Count - 1, nDRI_Next);

		// (2009/5/15 - Seung-Won, Bae) for future DRI.
		//		Check ItemCount to PushBack().
		int nDTIfDRICount = m_pblaDTIfDRI[ nRQ]->GetItemCount();
		if( 0 <= naDTI_Cur[ nRQ])
		{
			// 3.4.4 Reset DTI for last DRIs with added new future.
			for( nDRI = nDRI_Last; nDRI < nDTIfDRICount; nDRI++)
				m_pblaDTIfDRI[ nRQ]->SetAt( nDRI, naDTI_Cur[ nRQ]);
			for( nDRI = nDTIfDRICount; nDRI < nDRI_Next; nDRI++)
				m_pblaDTIfDRI[ nRQ]->PushBack( naDTI_Cur[ nRQ]);

			// 3.4.5 Reset DTI for end-mark DTI for added new future.
			int nDTI = ( ( naDTI_Cur[ nRQ]) + 3) * -1;
			for( nDRI = nDRI_Next; nDRI < nDTIfDRICount; nDRI++)	// do you know why? (nDRI <= m_nItemCount_Total)
				m_pblaDTIfDRI[ nRQ]->SetAt( nDRI, nDTI);
			for( nDRI = nDTIfDRICount; nDRI <= m_nItemCount_Total; nDRI++)	// do you know why? (nDRI <= m_nItemCount_Total)
				m_pblaDTIfDRI[ nRQ]->PushBack( nDTI);
		}
		else
		{
			// 3.4.5 Reset DTI for end-mark DTI for added new future.
			for( nDRI = nDRI_Last; nDRI < nDTIfDRICount; nDRI++)	// do you know why? (nDRI <= m_nItemCount_Total)
				m_pblaDTIfDRI[ nRQ]->SetAt( nDRI, naDTI_Cur[ nRQ]);
			for( nDRI = nDTIfDRICount; nDRI <= m_nItemCount_Total; nDRI++)	// do you know why? (nDRI <= m_nItemCount_Total)
				m_pblaDTIfDRI[ nRQ]->SetAt( nDRI, naDTI_Cur[ nRQ]);
		}
	}
	delete [] naDTI_Cur;

	// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
	if( m_bScrollByReal)
	{
		int nItemCount_Pre = m_nItemCount_Total;
		int nDRI_Start_Pre = m_nDRI_Start_Cur;
		int nDrawCount_Pre = m_nDrawCount_Cur;

		m_bScrollByReal = FALSE;
		if( m_pIChartCtrl->IsWholeView())
		{
			m_nDrawCount_Cur = m_nItemCount_Total;
			m_nDRI_End_Cur = m_nDrawCount_Cur - 1;
		}
		else
		{
			// (2009/1/14 - Seung-Won, Bae) Support Pixel Arranging.
			if( m_nCountOfBlockColumn == 1) if( 1 < m_nScreenWidth_Cur[ 0] && m_nScreenWidth_Cur[ 0] < m_nDrawCount_Cur && !m_bWeightedVolume)
			{
				m_nScreenWidth_Cur[ 0]--;	// leave the one pixel for real area.
				int nOnOnePixel = m_nDrawCount_Cur;
				if( 0 < m_nScreenWidth_Cur[ 0]) nOnOnePixel /= m_nScreenWidth_Cur[ 0];
				nOnOnePixel++;			// the number of max data count for one pixel.
				int nLeftDataCountOverAverage = m_nDrawCount_Cur % m_nScreenWidth_Cur[ 0];
				int nLeftDataOnRealArea = nLeftDataCountOverAverage % nOnOnePixel + 1;
				int nNewLeftDataOnRealArea = ( nLeftDataOnRealArea + m_nItemCount_Total - nItemCount_Prev) % nOnOnePixel;
				if( 0 == nNewLeftDataOnRealArea) nNewLeftDataOnRealArea = nOnOnePixel;
				m_nDrawCount_Cur += ( nNewLeftDataOnRealArea - nLeftDataOnRealArea);
				m_nScreenWidth_Cur[ 0]++;	// leave the one pixel for real area.
			}
			
			m_nDRI_End_Cur += m_nItemCount_Total - nItemCount_Prev;
		}
		
		m_nDRI_Start_Cur = m_nDRI_End_Cur - m_nDrawCount_Cur + 1;
		if( m_nDRI_Start_Cur < 0)
		{
			m_nDRI_End_Cur = m_nDrawCount_Cur - 1;
			m_nDRI_Start_Cur = 0;
		}

		// (2009/1/8 - Seung-Won, Bae) Update Count and Start to avoid the right indent too large.
		if( m_nItemCount_Total <= m_nDRI_End_Cur)
		{
			m_nDrawCount_Cur = m_nItemCount_Total;
			m_nDRI_End_Cur = m_nDrawCount_Cur - 1;
			m_nDRI_Start_Cur = m_nDRI_End_Cur - m_nDrawCount_Cur + 1;
		}
		
		if( m_nDrawCount_Cur != m_nDrawCount_Pre
			|| m_pIChartCtrl->IsWholeView() && !m_bWholeViewPre
			|| !m_pIChartCtrl->IsWholeView() && m_bWholeViewPre)
			for( int i = 0; i < m_nCountOfBlockColumn; i++) Calc_ItemWidth( i, m_nDrawCount_Cur);

		// 3. Update ScrollBar and Zoom SliderBar
		if( nItemCount_Pre != m_nItemCount_Total
			|| nDRI_Start_Pre != m_nDRI_Start_Cur
			|| nDrawCount_Pre != m_nDrawCount_Cur)
			m_pIChartCtrl->UpdateZoomAndScrollInfo( m_nItemCount_Total, m_nDRI_Start_Cur, m_nDrawCount_Cur);
	}
}

// (2009/1/19 - Seung-Won, Bae) for Scale Time List
inline void CXScaleManagerImp::PopLastScaleDataTime( void)
{
	signed short nTimeDiff = 0;
	CXScaleGridTime *pXScaleGridTime = NULL;
	POSITION psnXGridTime = m_mapXGridTime.GetStartPosition();
	while( psnXGridTime)
	{
		m_mapXGridTime.GetNextAssoc( psnXGridTime, nTimeDiff, pXScaleGridTime);
		if( !pXScaleGridTime) continue;
		pXScaleGridTime->PopLastScaleDataTime();
	}
}

inline void CXScaleManagerImp::PushScaleDataTime( int p_nDRI, time_t p_timeDRI, BOOL p_bEndDRI)
{
	if( p_timeDRI < 0)
	{
//		ASSERT( 0 <= p_timeDRI);
		return;
	}

	// (2009/1/19 - Seung-Won, Bae) for Scale Time List
	tm* ptmCur = localtime( &p_timeDRI);
	if( !ptmCur) return;
	tm tmDRI = *ptmCur;

	// (2009/6/4 - Seung-Won, Bae) for ceiled Minute Scale with second time data.
	if( m_bCeiling && 0 < tmDRI.tm_sec)
	{
		tmDRI.tm_sec += 60 - tmDRI.tm_sec;
		mktime( &tmDRI);
	}

	signed short nTimeDiff = 0;
	CXScaleGridTime *pXScaleGridTime = NULL;
	POSITION psnXGridTime = m_mapXGridTime.GetStartPosition();
	while( psnXGridTime)
	{
		m_mapXGridTime.GetNextAssoc( psnXGridTime, nTimeDiff, pXScaleGridTime);
		if( !pXScaleGridTime) continue;
		pXScaleGridTime->PushScaleDataTime( nTimeDiff, p_nDRI, tmDRI, p_bEndDRI);
	}
}

// (2008/12/28 - Seung-Won, Bae) Comment
//	Check current DTI of All RQ's are end.
BOOL CXScaleManagerImp::EndOfRQs( int* naDTI_Cur)
{
	for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
		if( m_pblaRQTime[ nRQ]->GetItemCount() != ( naDTI_Cur[ nRQ] + 1)
			&& -1 <= naDTI_Cur[ nRQ])
			return FALSE;

	return TRUE;
}

// (2008/12/28 - Seung-Won, Bae) Comment.
//	1. Get the minimum time value between the next time value of all RQs.
//	2. Increase the DTI of RQs that have the minimum time value.
time_t CXScaleManagerImp::GetLeastTime( int* naDTI_Cur, int* naIRQ, int* pnIRQ_Count)
{
	*pnIRQ_Count = -1;
	time_t timeTemp = 0;
	time_t timeResult = INT_MAX;
	// 1. Search the minimum time value and RQ that has it.
	for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
	{
		// 1.1 Check current DTI is on end.
		if( m_pblaRQTime[ nRQ]->GetItemCount() == ( naDTI_Cur[ nRQ] + 1)
			|| naDTI_Cur[ nRQ] < -1)
			continue;

		// 1.2 Retrieve the time value of the next DTI.
		m_pTReturn = m_pblaRQTime[ nRQ]->GetAt( naDTI_Cur[ nRQ] + 1);
		if( m_pTReturn) timeTemp = *m_pTReturn;

		// 1.3 Get the minimum time value.
		if( timeResult < timeTemp) continue;
		if( timeTemp < timeResult)
		{
			timeResult = timeTemp;
			*pnIRQ_Count = 0;
		}

		// 1.4 Get the RQs that have the time value with same to the minimum time value.
		naIRQ[ *pnIRQ_Count] = nRQ;
		( *pnIRQ_Count)++;
	}

	// 현재 시점에 일치하는 RQ
	for( int nCount = 0; nCount < *pnIRQ_Count; nCount++)
		naDTI_Cur[ naIRQ[ nCount]]++;

	return timeResult;
}

// REAL DATA 수신시에 호출
void CXScaleManagerImp::RealDataReceived( CString strRQ, BOOL p_bIndexedReal)
{
	if( 0 == m_nCountOfRQ)
	{
		// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI
		Calc_DRI();
		if( 0 == m_nCountOfRQ) return;
	}

	// Set Real Flag.
	int nRQ = GetRQIndexFromString( strRQ);
	if( nRQ < 0) return;

	if( p_bIndexedReal)
	{
		m_nNeedCalcArea = 2;
		m_pifaRQ[ nRQ]->nUpdatedRealDRI = -1;
	}
	else
	{
		if( 0 == m_nNeedCalcArea) m_nNeedCalcArea = 1;	// the Flag on by DRI() with full(2) or partial(1)
														// Do not reset 2 with 1.

		if( m_pifaRQ[ nRQ]->nUpdatedRealDRI != INT_MAX)
		{
			m_pIReturn = m_pblaDRIfDTI[ nRQ]->GetAt( m_pifaRQ[ nRQ]->nLastRealDTI);
			if( m_pIReturn) m_pifaRQ[ nRQ]->nUpdatedRealDRI = *m_pIReturn;
		}

		m_pifaRQ[ nRQ]->bRealReceived = TRUE;
	}
}


// 블록컬럼의 갯수를 설정합니다.
void CXScaleManagerImp::SetBlockColumnCount(int nBlockColumn_Count)
{
	m_nCountOfBlockColumn = nBlockColumn_Count;

//	int nBC, nCountOfBC;
/*
	VECTOR_BC_INFO::iterator itBC = m_vBCInfo.begin();
	while(itBC != m_vBCInfo.end())
	{
		delete *itBC;
	}

	m_vBCInfo.clear();


	nCountOfBC = nBlockColumn_Count;
	for(nBC = 0; nBC < nCountOfBC; nBC++)
	{
		PBC_INFO pbc_info = new BC_INFO;
		m_vBCInfo.push_back(pbc_info);
	}

	if(m_nCountOfBlockColumn > 1) m_rmtype = RMAT_RIGHT_MARGIN_FIX;
*/
}

/*
// 컬럼갯수 : xScaleManager - ojtaso (20071128)
int CXScaleManagerImp::GetCountOfBlockColumn()
{
	return m_nCountOfBlockColumn;
}
*/

// 다른 함수(스케일, 그래프 관련 함수)들이 기본으로 참고할 블록컬럼을 설정합니다.
void CXScaleManagerImp::SetBlockColumn_Cur(int nBlockColumn)
{
//	m_pbcinfo_cur = m_vBCInfo.at(nBlockColumn);
}

// 전체 갯수를 얻어옵니다.
int CXScaleManagerImp::GetCount()
{
	return m_nItemCount_Total;	// the future count is included in total..
}

// 현재 화면에 보여지는 갯수를 얻어옵니다.
int CXScaleManagerImp::GetDrawCount_Cur()
{
	return m_nDrawCount_Cur;
}

/*
// 현재 화면에 보여지는 영역의 시작 위치를 설정합니다.
void CXScaleManagerImp::SetStartDrawIdx_Cur(int nStartDrawIdx_Cur)
{
	m_nDRI_Start_Cur = nStartDrawIdx_Cur;
	m_nDRI_End_Cur = m_nDRI_Start_Cur + m_nDrawCount_Cur - 1;
}
*/

// 현재 화면에 보여지는 영역의 시작 위치를 얻어옵니다.
int CXScaleManagerImp::GetStartDrawIdx_Cur()
{
	return m_nDRI_Start_Cur;
}

/*
// 현재 화면에 보여지는 영역의 끝 위치를 설정합니다.
void CXScaleManagerImp::SetEndDrawIdx_Cur(int nEndDrawIdx_Cur, BOOL bCalc)
{
	m_nDRI_End_Cur = nEndDrawIdx_Cur;
	m_nDRI_Start_Cur = m_nDRI_End_Cur - m_nDrawCount_Cur + 1;

	if(m_nDRI_Start_Cur < 0)
	{
		m_nDRI_Start_Cur = 0;
		m_nDRI_End_Cur = m_nDRI_Start_Cur + m_nDrawCount_Cur - 1;
	}
}
*/

// 현재 화면에 보여지는 영역의 끝 위치를 얻어옵니다.
int CXScaleManagerImp::GetEndDrawIdx_Cur()
{
	return m_nDRI_End_Cur;
}

void CXScaleManagerImp::GetStartEndDrawIdx_Cur(int& nStartDrawIdx, int& nEndDrawIdx)
{
	nStartDrawIdx = m_nDRI_Start_Cur;
	nEndDrawIdx = m_nDRI_End_Cur;
}

void CXScaleManagerImp::SetStartEndDrawIdx_Cur(int nStartDrawIdx, int nEndDrawIdx, BOOL p_bMoreZoomIn)
{
	int nDrawCount = (nEndDrawIdx - nStartDrawIdx + 1);
	if( nDrawCount < m_nMinDataCountOnOnePage) nDrawCount = m_nMinDataCountOnOnePage;

	m_nDRI_End_Cur		= nEndDrawIdx;
	m_nDRI_Start_Cur	= nEndDrawIdx - nDrawCount + 1;

	// (2009/1/18 - Seung-Won, Bae) Check Limit.
	if( m_nDRI_Start_Cur < 0)
	{
		m_nDRI_Start_Cur = 0;
		m_nDRI_End_Cur = m_nDRI_Start_Cur + nDrawCount - 1;
	}
	// (2009/2/19 - Seung-Won, Bae) If no-data, Set view range to (0:ZoomValue) for accepting of ZoomValue.
	if( 0 < m_nItemCount_Total && m_nItemCount_Total <= m_nDRI_End_Cur)
	{
		m_nDRI_End_Cur = m_nItemCount_Total - 1;
		m_nDRI_Start_Cur = m_nDRI_End_Cur - nDrawCount + 1;
	}
	if( m_nDRI_Start_Cur < 0)
	{
		m_nDRI_Start_Cur = 0;
		m_nDRI_End_Cur = m_nItemCount_Total - 1;
		nDrawCount = m_nItemCount_Total;
	}
		
	if( m_nDrawCount_Cur != nDrawCount
		|| m_pIChartCtrl->IsWholeView() && !m_bWholeViewPre
		|| !m_pIChartCtrl->IsWholeView() && m_bWholeViewPre)
		for( int i = 0; i < m_nCountOfBlockColumn; i++) Calc_ItemWidth( i, nDrawCount, p_bMoreZoomIn);

	// 3. Update ScrollBar and Zoom SliderBar
	m_pIChartCtrl->UpdateZoomAndScrollInfo( m_nItemCount_Total, m_nDRI_Start_Cur, m_nDrawCount_Cur);
}

void CXScaleManagerImp::Calc_ItemWidth( int p_nColumn, int nDrawCount, BOOL p_bMoreZoomIn)
{
	int nPreDrawCount = m_nDrawCount_Cur;
	if( nDrawCount < m_nMinDataCountOnOnePage) nDrawCount = m_nMinDataCountOnOnePage;
	m_nDrawCount_Cur = nDrawCount;

	// (2009/1/19 - Seung-Won, Bae) Do you know why do check after setting the draw count.
	if( m_nScreenWidth_Cur[ p_nColumn] < 5) return;

	// (2009/1/14 - Seung-Won, Bae) Support Pixel Arranging.
	int nLeftDataOnRealArea = -1;
	if( 1 == m_nCountOfBlockColumn && 1 < m_nScreenWidth_Cur[ p_nColumn] && m_nScreenWidth_Cur[ p_nColumn] < m_nDrawCount_Cur && !m_bWeightedVolume)
	{
		m_nScreenWidth_Cur[ p_nColumn]--;	// leave the one pixel for real area.
		int nOnOnePixel = m_nDrawCount_Cur;
		if( 0 < m_nScreenWidth_Cur[ p_nColumn]) nOnOnePixel /= m_nScreenWidth_Cur[ p_nColumn];
		nOnOnePixel++;			// the number of max data count for one pixel.
		int nLeftDataCountOverAverage = m_nDrawCount_Cur % m_nScreenWidth_Cur[ p_nColumn];
		nLeftDataOnRealArea = nLeftDataCountOverAverage % nOnOnePixel + 1;
		m_nDrawCount_Cur -= nLeftDataOnRealArea;
	}

	m_daItemWidth[ p_nColumn] = ( double)m_nScreenWidth_Cur[ p_nColumn] / ( double)max( m_nDrawCount_Cur, m_nMinDataCountOnOnePage);

	// (2009/1/14 - Seung-Won, Bae) Support Pixel Arranging.
	if( 0 <= nLeftDataOnRealArea)
	{
		m_nScreenWidth_Cur[ p_nColumn]++;
		m_nDrawCount_Cur += nLeftDataOnRealArea;
	}
	else	// Do not support fixed interval for small size then one pixel.
	// 우측 여백 동작 방식
	if( 1 == m_nCountOfBlockColumn && m_bFixedInterval && !m_pIChartCtrl->IsWholeView())
	{
		if( 0 < m_daItemWidth[ p_nColumn] && m_daItemWidth[ p_nColumn] < 1)
				m_daItemWidth[ p_nColumn] = 1.0 / ceil( 1 / m_daItemWidth[ p_nColumn]);
		else	m_daItemWidth[ p_nColumn] = ( int)m_daItemWidth[ p_nColumn];
		// (2009/1/8 - Seung-Won, Bae) Update Count and Start to avoid the right indent too large.
		if( 0 < m_daItemWidth[ p_nColumn]) m_nDrawCount_Cur = ( int)( ( double)m_nScreenWidth_Cur[ p_nColumn] / m_daItemWidth[ p_nColumn]);

		// (2009/1/8 - Seung-Won, Bae) Zoom More !
		if( p_bMoreZoomIn && m_nDrawCount_Cur == nPreDrawCount && m_nDrawCount_Cur != nDrawCount)
		{
			if( m_nDrawCount_Cur < nDrawCount)
			{
				if( m_daItemWidth[ p_nColumn] < 2)		m_daItemWidth[ p_nColumn]--;
				else if( 0 < m_daItemWidth[ p_nColumn])	m_daItemWidth[ p_nColumn] = 1.0 / ( ceil( 1 / m_daItemWidth[ p_nColumn]) + 1);
			}
			else
			{
				if( 1 <= m_daItemWidth[ p_nColumn])		m_daItemWidth[ p_nColumn]++;
				else if( 0 < m_daItemWidth[ p_nColumn])	m_daItemWidth[ p_nColumn] = 1.0 / ( ceil( 1 / m_daItemWidth[ p_nColumn]) - 1);
			}
			// (2009/1/8 - Seung-Won, Bae) Update Count and Start to avoid the right indent too large.
			if( 0 < m_daItemWidth[ p_nColumn]) m_nDrawCount_Cur = ( int)( ( double)m_nScreenWidth_Cur[ p_nColumn] / m_daItemWidth[ p_nColumn]);
		}

		if( m_nItemCount_Total < m_nDrawCount_Cur)
		{
			m_nDrawCount_Cur = m_nItemCount_Total;
			m_daItemWidth[ p_nColumn] = ( double)m_nScreenWidth_Cur[ p_nColumn] / ( double)max( m_nDrawCount_Cur, m_nMinDataCountOnOnePage);
			if( 0 < m_daItemWidth[ p_nColumn] && m_daItemWidth[ p_nColumn] < 1)
					m_daItemWidth[ p_nColumn] = 1.0 / ( ceil( 1 / m_daItemWidth[ p_nColumn]) - 1);
			else	m_daItemWidth[ p_nColumn] = int( m_daItemWidth[ p_nColumn]) + 1;
			if( 0 < m_daItemWidth[ p_nColumn]) m_nDrawCount_Cur = ( int)( ( double)m_nScreenWidth_Cur[ p_nColumn] / m_daItemWidth[ p_nColumn]);
		}
	}

	m_naItemWidth[ p_nColumn] = (int)m_daItemWidth[ p_nColumn];
	m_nScreenRight_Cur[ p_nColumn] = m_nScreenLeft_Cur[ p_nColumn] + m_nScreenWidth_Cur[ p_nColumn];
	if(m_bReverse == FALSE)
	{
		m_naScreenStart_Cur[ p_nColumn] = m_nScreenLeft_Cur[ p_nColumn];
		m_naScreenEnd_Cur[ p_nColumn] = m_nScreenRight_Cur[ p_nColumn];

		m_nScreenStartiM_Cur[ p_nColumn] = m_nScreenLeftiM_Cur[ p_nColumn];
		m_nScreenEndiM_Cur[ p_nColumn] = m_nScreenRightiM_Cur[ p_nColumn];
	}
	else
	{
		m_naScreenStart_Cur[ p_nColumn] = m_nScreenRight_Cur[ p_nColumn];
		m_naScreenEnd_Cur[ p_nColumn] = m_nScreenLeft_Cur[ p_nColumn];

		m_nScreenStartiM_Cur[ p_nColumn] = m_nScreenRightiM_Cur[ p_nColumn];
		m_nScreenEndiM_Cur[ p_nColumn] = m_nScreenLeftiM_Cur[ p_nColumn];
	}

	m_nDRI_Start_Cur =  m_nDRI_End_Cur - m_nDrawCount_Cur + 1;
	if( m_nDRI_Start_Cur < 0)
	{
		m_nDRI_Start_Cur = 0;
		m_nDRI_End_Cur = m_nDRI_Start_Cur + m_nDrawCount_Cur - 1;
	}

	m_bWholeViewPre = m_pIChartCtrl->IsWholeView();
}

// 그래프가 현재 보여지는 영역에서 그려야할 영역 정보를 넘겨줍니다.
void CXScaleManagerImp::GetDrawIndex(CString strRQ, BOOL bIncludePrev, BOOL bOEBT, int& nDTI_Start, int& nDTI_End)
{
	string stringRQ((LPSTR)(LPCSTR)strRQ);
	
	int nDTI_Start_Org, nDTI_End_Org;

	nDTI_Start = nDTI_End = -1;

	int nRQ = GetRQIndexFromString( strRQ);
	if( nRQ < 0) return;
	CDataBlockList<int>* blDTIfDRI = m_pblaDTIfDRI[ nRQ];

	nDTI_Start	= GetDTIFromDRI_AbleRange(blDTIfDRI, m_nDRI_Start_Cur, &nDTI_Start_Org);
	nDTI_End	= GetDTIFromDRI_AbleRange(blDTIfDRI, m_nDRI_End_Cur, &nDTI_End_Org);

	if(nDTI_Start_Org >= 0 || nDTI_Start_Org != nDTI_End_Org)
	{
		// 일목 균형표를 포함하는 RQ인지 체크합니다.
		// (2008/12/29 - Seung-Won, Bae) Do not use m_mifOEBT. Use m_pifaOEBT.
		//	MAP_RQ_OEBT::iterator it = m_mifOEBT.find(stringRQ);
		//	BOOL bOEBT_RQ = (it != m_mifOEBT.end());
		// 일목 균형표를 포함하는 RQ의 그래프중 일목 균형표가 아닌 그래프이면
		// 해당 그래프는 일목균형표 우측 여백 만큼 그리지 않도록 해야 한다.
		if( m_pifaOEBT[ nRQ] && bOEBT == FALSE && m_bUseOEBTMargin == TRUE)
			nDTI_End -= m_pifaOEBT[ nRQ]->nFutureCount;

		// 선을 그릴때는 앞쪽에 보이지 않는 데이터를 얻어야 정확한 선을 그릴수 있습니다.
		if(bIncludePrev == TRUE) nDTI_Start--;

		if(nDTI_Start < 0) nDTI_Start = 0;

		if(nDTI_End < nDTI_Start)
		{
			nDTI_Start = nDTI_End = -1;
		}
	}
	else
	{
		nDTI_Start = nDTI_End = -1;
	}
}

// 현재 선택된 RQ를 자동으로 호출 : xScaleManager - ojtaso (20070409)
void CXScaleManagerImp::GetDrawIndex(BOOL bIncludePrev, BOOL bOEBT, int& nDTI_Start, int& nDTI_End)
{
	if(m_ppacketListManager)	
		GetDrawIndex( m_pIChartCtrl->GetCurrentRQ(), bIncludePrev, bOEBT, nDTI_Start, nDTI_End);
	else
		GetDrawIndex(_T("DEFAULT"), bIncludePrev, bOEBT, nDTI_Start, nDTI_End);
}

int CXScaleManagerImp::GetDTIFromDRI(CString strRQ, int nDRI)
{
	int nRQ = GetRQIndexFromString( strRQ);
	if( nRQ < 0) return -1;
	return GetDTIFromDRI( m_pblaDTIfDRI[ nRQ], nDRI);
}

// (2009/1/7 - Seung-Won, Bae) Comment.
//	Get DTI Value from DRI.
//		if No DTI for past. -1
//		if No DTI for future. End Mark DTI.
int CXScaleManagerImp::GetDTIFromDRI( CDataBlockList<int>* pblDTIfDRI, int nDRI)
{
	// (2009/4/10 - Seung-Won, Bae) check NULL.
	if( !pblDTIfDRI) return -1;

	int nItemCount = pblDTIfDRI->GetItemCount();
	if( nItemCount == 0) return -1;

	if( 0 <= nDRI && nDRI < nItemCount)
	{
		m_pIReturn = pblDTIfDRI->GetAt(nDRI);
		if( m_pIReturn) return *m_pIReturn;
		else return -1;
	}
	if( nItemCount <= nDRI)
	{
		m_pIReturn = pblDTIfDRI->GetAt( nItemCount - 1);	// end mark.
		if( m_pIReturn) return *m_pIReturn;
		else return -1;
	}
	return -1;															// no matched.
}

int CXScaleManagerImp::GetDTIFromDRI_AbleRange(CString strRQ, int nDRI, int* pnDTI_Org)
{
	int nRQ = GetRQIndexFromString( strRQ);
	if( nRQ < 0) return -1;
	return GetDTIFromDRI_AbleRange( m_pblaDTIfDRI[ nRQ], nDRI, pnDTI_Org);
}

// (2009/1/7 - Seung-Won, Bae) Comment.
//	Get DTI Index from DRI.
//		if No DTI for past.		0
//		if No DTI for future.	Last DTI. (can be calculated by EndMark.)
int CXScaleManagerImp::GetDTIFromDRI_AbleRange( CDataBlockList<int>* pblDTIfDRI, int nDRI, int* pnDTI_Org)
{
	int nDTI = GetDTIFromDRI(pblDTIfDRI, nDRI);
	if( pnDTI_Org != NULL) *pnDTI_Org = nDTI;
	if( nDTI == -1)	nDTI = 0;
	if( nDTI < -1)	nDTI = (nDTI * -1) - 3;
	return nDTI;
}

// (2008/12/27 - Seung-Won, Bae) Comment.
//		Retrieve the RQ Index. (RQ is indexed in m_midxRQ. but why?)
int CXScaleManagerImp::GetRQIndexFromString(CString strRQ)
{
	string stringRQ((LPSTR)(LPCSTR)strRQ);

	MAP_RQ_INDEX::iterator it = m_midxRQ.find(stringRQ);
	if( it != m_midxRQ.end()) return it->second;

	return -1;
}

// 스케일을 그릴때 사용할 Unit정보입니다.
// RQ별 Unit정보 관리 : 복수종목 - ojtaso (20070601)
CScaleBaseData::HORZSCALEDRAWERTYPE CXScaleManagerImp::GetScaleUnit_Max(LPCTSTR lpszRQ/* = NULL*/)
{
	if( lpszRQ) if( !*lpszRQ) lpszRQ = NULL;
	int nRQ = GetRQIndexFromString( lpszRQ ? lpszRQ : m_pIChartCtrl->GetCurrentRQ());
	if( 0 <= nRQ)	return m_pifaRQ[ nRQ]->eScaleUnit;
	else			return m_maxScaleInterval_eType;
}

// 스케일을 그릴때 사용할 Interval정보입니다.
int CXScaleManagerImp::GetScaleInterval_Unit_Max()
{
	return m_maxScaleInterval_nUnit;
}

// 스케일 정보를 얻으려고 하는 위치를 설정합니다.
void CXScaleManagerImp::SetCurScaleInfo(int nDRI)
{
	m_blTimefDRI.SetIndex(nDRI);
	m_blAreafDRI[ 0].SetIndex(nDRI - m_nDRI_Start_Cur);
}
																																																																																																																																																																		
// 스케일 정보를 얻으려고 하는 위치를 얻어옵니다.
void CXScaleManagerImp::GetCurScaleInfo(int& nDRI)
{
	nDRI = m_blAreafDRI[ 0].GetIndex();
}

// 다음 스케일 위치의 시간 정보를 구합니다.
BOOL CXScaleManagerImp::NextScaleTime( time_t* ptimeScale, PAREA* pparea)
{
	int nItemCount = m_blTimefDRI.GetItemCount();
	if( 0 == nItemCount) return FALSE;

	if( m_blTimefDRI.GetIndex() < nItemCount)
	{
		m_pTReturn = m_blTimefDRI.GetNext();
		if( m_pTReturn) *ptimeScale = *m_pTReturn;
		*pparea = (PAREA)m_blAreafDRI[ 0].GetNextUNK();
		return TRUE;
	}

	m_pTReturn = m_blTimefDRI.GetAt(m_blTimefDRI.GetItemCount() - 1);
	if( m_pTReturn) *ptimeScale = *m_pTReturn;
	*pparea = (PAREA)m_blAreafDRI[ 0].GetAtUNK( m_blAreafDRI[ 0].GetItemCount() - 1 - 1);	// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle
	return TRUE;
}

// 이전 스케일 위치의 시간 정보를 구합니다.
void CXScaleManagerImp::PrevScaleTime(time_t* ptimeScale, PAREA* pparea)
{
	m_pTReturn = m_blTimefDRI.GetPrev();
	if( m_pTReturn) *ptimeScale = *m_pTReturn;
	*pparea = (PAREA)m_blAreafDRI[ 0].GetPrevUNK();
}

void CXScaleManagerImp::GetAt_ScaleTime(int nDRI, time_t* ptimeScale, PAREA* pparea)
{
	m_pTReturn = m_blTimefDRI.GetAt( nDRI);
	if( m_pTReturn) *ptimeScale = *m_pTReturn;
	*pparea = (PAREA)m_blAreafDRI[ 0].GetAtUNK(nDRI);
}

// (2009/1/3 - Seung-Won, Bae) Comment.
void CXScaleManagerImp::GetTimeFromDRI( int nDRI, time_t* ptimeScale)
{
	// 0. Check exception.
	if( m_nItemCount_Total == 0)
	{
		*ptimeScale = 0;
		return;
	}

	int nDRITimeCount = m_blTimefDRI.GetItemCount();
	// 1. In Range of full time line or past.
	if( nDRI < 0) nDRI = 0;
	if( 0 <= nDRI && nDRI < nDRITimeCount)
	{
		m_pTReturn = m_blTimefDRI.GetAt( nDRI);
		if( m_pTReturn) *ptimeScale = *m_pTReturn;
		return;
	}

	// 2. for Future.
	int nDRI_Gap = nDRI - nDRITimeCount + 1;
	m_pTReturn = m_blTimefDRI.GetAt( nDRITimeCount - 1);
	if( m_pTReturn) *ptimeScale = *m_pTReturn;
	tm* ptmCur = localtime( ptimeScale);
	if( !ptmCur) return;
	switch( m_minScaleInterval_eType)
	{
		case CScaleBaseData::HORZ_MONTHLY:		ptmCur->tm_mon	+= ( m_minScaleInterval_nUnit * nDRI_Gap);
												break;
		case CScaleBaseData::HORZ_WEEKLEY:		ptmCur->tm_mday += ( m_minScaleInterval_nUnit * 7 * nDRI_Gap);
												break;
		case CScaleBaseData::HORZ_DAILY:		ptmCur->tm_mday += ( m_minScaleInterval_nUnit * nDRI_Gap);
												break;
		case CScaleBaseData::HORZ_TIME_ONE:
		case CScaleBaseData::HORZ_TIME_TWO:
		case CScaleBaseData::HORZ_TIME_SECOND:	ptmCur->tm_sec	+= ( m_minScaleInterval_nUnit * nDRI_Gap);
												break;
		case CScaleBaseData::HORZ_TICK:			ptmCur->tm_sec	+= nDRI_Gap;
												break;
	}
	*ptimeScale = mktime(ptmCur);
}


// 추세선에서 특정 RQ의 DTI에 해당하는 영역 정보를 구하고자 할때 사용합니다.
BOOL CXScaleManagerImp::GetAREAfRQnDTI( CString strRQ, int nDTI, int nBlockColumn, PAREA parea)
{
	*parea = m_areaNONE;

	int nDRI_Cur, nDRI_Next;

	CDataBlockList<int>* pblDRIfDTI = NULL;
	int	nRQ = GetRQIndexFromString( strRQ);
	if( 0 <= nRQ) pblDRIfDTI = m_pblaDRIfDTI[ nRQ];

	RQ_INFO* pifRQ = NULL;
	if( 0 <= nRQ) pifRQ = m_pifaRQ[ nRQ];
	
	int nDTI_Total = pblDRIfDTI->GetItemCount();
	if( 0 <= nDTI && nDTI < nDTI_Total - 1)	// (2009/2/20 - Seung-Won, Bae) -1 is end mark.
	{
		if( pifRQ->nDTI_Start_Data <= nDTI && nDTI <= pifRQ->nDTI_End_Data)
		{
			CDataBlockList< AREA> *pblAreafDTI = NULL;
			if( 0 <= nRQ) pblAreafDTI = m_pblaAreafDTI[ nBlockColumn][ nRQ];

			int nDTI_View = nDTI - pifRQ->nDTI_Start_Data;

			m_pAReturn = ( PAREA)pblAreafDTI->GetAtUNK( nDTI_View);
			if( m_pAReturn) *parea = *m_pAReturn;
		}
		else if( nDTI < nDTI_Total - 1)
		{
			m_pIReturn = pblDRIfDTI->GetAt( nDTI);
			if( m_pIReturn) nDRI_Cur = *m_pIReturn;
			m_pIReturn = pblDRIfDTI->GetAt( nDTI + 1);
			if( m_pIReturn) nDRI_Next = *m_pIReturn;

			if(nDRI_Cur < 0 && nDRI_Next < 0)
				return FALSE;

			PAREA parea_Cur = NULL, parea_Next = NULL;
			GetAreaFromDRI( nBlockColumn, nDRI_Cur, &parea_Cur);
			GetAreaFromDRI( nBlockColumn, nDRI_Next, &parea_Next);
			if( !parea_Cur) return FALSE;

			if(m_bReverse == TRUE)
			{
				//parea->left = parea_Next->right;
				if(parea_Next)
					parea->left = parea_Next->right;
				else
					parea->left = parea_Cur->left;
				parea->right = parea_Cur->right;
				parea->center = ( parea->left + parea->right) / 2;
				ItemIntervalApply( nBlockColumn, *parea, nDRI_Next - nDRI_Cur);
			}
			else
			{
				parea->left = parea_Cur->left;
				//parea->right = parea_Next->left;
				if(parea_Next)
					parea->right = parea_Next->left;
				else
					parea->right = parea_Cur->right;
				parea->center = ( parea->left + parea->right) / 2;
				ItemIntervalApply_Reverse( nBlockColumn, *parea, nDRI_Next - nDRI_Cur);
			}
		}
	}
	// (2009/2/20 - Seung-Won, Bae) for out-side.
	else if( nDTI < 0)
	{
		m_pIReturn = pblDRIfDTI->GetAt( 0);
		if( m_pIReturn) nDRI_Cur = *m_pIReturn;
		nDRI_Cur += nDTI;

		PAREA parea_Cur = NULL;
		GetAreaFromDRI( nBlockColumn, nDRI_Cur, &parea_Cur);
		if( !parea_Cur) return FALSE;
		*parea = *parea_Cur;
	}
	else
	{
		m_pIReturn = pblDRIfDTI->GetAt( nDTI_Total - 1);
		if( m_pIReturn) nDRI_Cur = *m_pIReturn;
		nDRI_Cur += nDTI - nDTI_Total + 1;

		PAREA parea_Cur = NULL;
		GetAreaFromDRI( nBlockColumn, nDRI_Cur, &parea_Cur);
		if( !parea_Cur) return FALSE;
		*parea = *parea_Cur;
	}

	return TRUE;
}

// 현재 선택된 RQ를 자동으로 호출 : xScaleManager - ojtaso (20070409)
BOOL CXScaleManagerImp::GetAREAfRQnDTI(int nDTI, int nBlockColumn, PAREA parea)
{
	if(m_ppacketListManager)
		return GetAREAfRQnDTI( m_pIChartCtrl->GetCurrentRQ(), nDTI, nBlockColumn, parea);
	else
		return GetAREAfRQnDTI(_T("DEFAULT"), nDTI, nBlockColumn, parea);
}

// 십자선과 같은 곳에서 Pixel값에 RQ들, DTI들, DRI_AREA의 정보를 구하고자 할때 사용합니다.
BOOL CXScaleManagerImp::GetRQInfosFromPt(int nCursor_x, int& nBlockColumn, CStringArray& saRQ, CArray<int, int>& naDTI, PAREA parea)
{
	saRQ.RemoveAll();
	naDTI.RemoveAll();

	int nDRI = GetDRInFromPt( nCursor_x, &nBlockColumn);
	GetAreaFromDRI_Copy( nBlockColumn, nDRI, *parea);

	for(int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
	{
		int nDTI, nDTI_Org;

		m_pIReturn = m_pblaDTIfDRI[nRQ]->GetAt( nDRI);
		if( m_pIReturn) nDTI = nDTI_Org = *m_pIReturn;

		if(nDTI >= 0)
		{
			saRQ.Add(m_saRQ.GetAt(nRQ));
			naDTI.Add(nDTI);
		}
	}
	
	return (saRQ.GetSize() > 0);
}

time_t CXScaleManagerImp::GetTimeFromX(int nCursor_x, int* pnBlockColumn, PAREA* pparea)
{
	time_t timeResult;

	int nDRI = GetDRInFromPt(nCursor_x, pnBlockColumn, pparea);
	GetTimeFromDRI(nDRI, &timeResult);

	return timeResult;
}

// 우측여백도 포함된 시간예상 : xScaleManager - ojtaso (20070608)
time_t CXScaleManagerImp::GetTimeFromXWithRightMargin(int nCursor_x, int* pnBlockColumn, PAREA* pparea)
{
	time_t timeResult;

	int nDRI = GetDRInFromPt(nCursor_x, pnBlockColumn, pparea, TRUE);
	GetTimeFromDRI(nDRI, &timeResult);

	return timeResult;
}

// 패킷타입 : xScaleManager - ojtaso (20070703)
CString CXScaleManagerImp::GetPacketType(LPCTSTR lpszRQ)
{
	CString strRQ( lpszRQ);
	if( strRQ.IsEmpty()) strRQ = m_pIChartCtrl->GetCurrentRQ();
	CPacketRQ* lpPacketRQ = (CPacketRQ*)m_ppacketListManager->GetPacketRQ( strRQ);

	CPacketList* pPacketList = m_ppacketListManager->GetPacketList( strRQ);
	CPacket* pPacket = pPacketList->GetBaseData(lpPacketRQ->GetPacketName());
	if(pPacket == NULL) 
		return _T("MMDDHHMM");

	return pPacket->GetType();
}

// double값으로 변환 : xScaleManager - ojtaso (20070402)
// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
double CXScaleManagerImp::GetDoubleFromX( const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, PAREA* pparea, const char *p_szRQ)
{
	LPCTSTR lpszRQ = p_szRQ;
	if( lpszRQ) if( !*lpszRQ) lpszRQ = NULL;
	if( !lpszRQ) lpszRQ = m_pIChartCtrl->GetMasterValueRQ( nRow, nCol);

	CPacketRQ* lpPacketRQ = (CPacketRQ*)m_ppacketListManager->GetPacketRQ(lpszRQ);
	if(::lstrcmp(lpPacketRQ->GetPacketName(), _MTEXT( C0_DATE_TIME)))
		return 0.;

	CPacketList* pPacketList = m_ppacketListManager->GetPacketList(lpszRQ);
	CPacket* pPacket = pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if(pPacket == NULL) 
		return 0.;

	int nDRI = GetDRInFromPt( nCursor_x, pnBlockColumn, pparea, TRUE);
	// (2009/5/15 - Seung-Won, Bae) for future DRI.
	if( nDRI < 0) nDRI = 0;
	else if( m_blTimefDRI.GetItemCount() <= nDRI) nDRI = m_blTimefDRI.GetItemCount() - 1;
	time_t timeDRI = 0;
	m_pTReturn = m_blTimefDRI.GetAt( nDRI);
	if( m_pTReturn) timeDRI = *m_pTReturn;
	if( 0 == timeDRI) return 0;
	return GetTimeValueFromTime( timeDRI, pPacket->GetType());
}


// double값으로 변환 : xScaleManager - ojtaso (20070402)
// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
int CXScaleManagerImp::GetXFromDouble( const int nRow, const int nCol, double dblFind, PAREA* pparea)
{
	LPCTSTR lpszRQ = m_pIChartCtrl->GetMasterValueRQ(nRow, nCol);

	CPacketRQ* lpPacketRQ = (CPacketRQ*)m_ppacketListManager->GetPacketRQ(lpszRQ);
	if(::lstrcmp(lpPacketRQ->GetPacketName(), _MTEXT( C0_DATE_TIME)))
		return 0;

	CPacketList* pPacketList = m_ppacketListManager->GetPacketList(lpszRQ);
	CPacket* pPacket = pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if(pPacket == NULL) 
		return 0;

	CString strType = pPacket->GetType();

	int	nRQ = GetRQIndexFromString( lpszRQ);
	tm tmResult = { 0, 0, 0, 0, 0, 0, 0, 0, -1};	//[A00000554]alzioyes:20110530 시간셋팅 초기값변경.
	
	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//time_t timeFind = GetTimeFromTimeValue( ( __int64)dblFind, strType, 0 <= nRQ ? m_pifaRQ[ nRQ] : NULL, tmResult, lpszRQ);
	//CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pPacket->GetDateUnitType();
	time_t timeFind = GetTimeFromTimeValue( ( __int64)dblFind, strType, 0 <= nRQ ? m_pifaRQ[ nRQ] : NULL, tmResult, lpszRQ);
	// 2011.01.27 by SYS <<


	return GetXFromTime( nCol, timeFind, pparea, TRUE);
}

int CXScaleManagerImp::GetScaleUnitAlphaWeight(CString strRQ)
{
	int	nRQ = GetRQIndexFromString( strRQ);
	if( nRQ < 0) return 1;
	switch( m_pifaRQ[ nRQ]->eScaleUnit)
	{
		case CScaleBaseData::HORZ_MONTHLY:		return 160;
		case CScaleBaseData::HORZ_WEEKLEY:		return 160;
		case CScaleBaseData::HORZ_DAILY:		return 200;
		case CScaleBaseData::HORZ_TIME_ONE:		return 160;
		case CScaleBaseData::HORZ_TIME_TWO:		return 160;
		case CScaleBaseData::HORZ_TIME_SECOND:	return 160;
		case CScaleBaseData::HORZ_TICK:			return 160;
	}
	return 160;
}

// (2008/12/28 - Seung-Won, Bae) Comment.
//	Get the DRI of the next time with time unit of RQ.
//	How dare you believe the time value of nDRI_Cur is on time unit ?
int CXScaleManagerImp::GetNextDRI_ByScaleInterval(CScaleBaseData::HORZSCALEDRAWERTYPE ScaleUnit, int nScaleInterval_Unit, int nDRI_Cur)
{
	// 1. Get next DRI for tick.
	if( ScaleUnit == CScaleBaseData::HORZ_TICK)
	{
		// ? what is m_minScaleInterval_nUnit ?
		int nDRI_Next = nDRI_Cur;
		if( 0 < m_minScaleInterval_nUnit) nDRI_Next += int( nScaleInterval_Unit / m_minScaleInterval_nUnit);
		if( nDRI_Next > m_nItemCount_Total) nDRI_Next = m_nItemCount_Total;
		return nDRI_Next;
	}

	// 2. Get next DRI for other time type.
	// 2.1 Get current time value for Current DRI in full time line.
	int nDRI_Next = nDRI_Cur + 1;
	time_t timeCur = { 0,};
	m_pTReturn = m_blTimefDRI.GetAt( nDRI_Cur);
	if( m_pTReturn) timeCur = *m_pTReturn;
	// (2009/6/4 - Seung-Won, Bae) Check Exception.
	if( timeCur < 0) return nDRI_Next;
	// 2.2 Get next time vlaue with type unit of RQ.
	tm* ptmCur = localtime( &timeCur);

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//if( !ptmCur) return nDRI_Next;
	if(ptmCur == NULL)
	{
		return INT_MIN;
	}
	// 2011.01.06 by SYS <<


	switch( ScaleUnit)
	{
		default: return nDRI_Next;

		case CScaleBaseData::HORZ_MONTHLY:		ptmCur->tm_mon	+= nScaleInterval_Unit;			break;
		case CScaleBaseData::HORZ_WEEKLEY:		ptmCur->tm_mday += (nScaleInterval_Unit * 7);	break;
		case CScaleBaseData::HORZ_DAILY:		ptmCur->tm_mday += nScaleInterval_Unit;			break;
		case CScaleBaseData::HORZ_TIME_ONE:		ptmCur->tm_sec	+= nScaleInterval_Unit;			break;
		case CScaleBaseData::HORZ_TIME_TWO:		ptmCur->tm_sec	+= nScaleInterval_Unit;			break;
		case CScaleBaseData::HORZ_TIME_SECOND:	ptmCur->tm_sec	+= nScaleInterval_Unit;			break;
	}
	time_t timeNext = mktime( ptmCur);
	// 2.3 Search the DRI for next time in full time line.
	if( timeCur != timeNext) while( m_nItemCount_Total > nDRI_Next)
	{
		GetTimeFromDRI( nDRI_Next, &timeCur);
		if( timeNext <= timeCur) break;
		nDRI_Next++;
	}
	return nDRI_Next;
}


int CXScaleManagerImp::GetPageCountNextStep(const int nSliderPos_Prev, const int nSliderPos_New, const int nSBCode, const int nMin, const int nMax)
{
	if( 1 != m_nCountOfBlockColumn || m_nCountOfRQ == 0 || !m_bFixedInterval) return nSliderPos_New;

	int nCount = GetCount();
	int nPreDrawCount = m_nDrawCount_Cur;
	int nDrawCount_Cur = nSliderPos_New;
	double dItemWidth = ( double)m_nScreenWidth_Cur[ 0] / ( double)max( nDrawCount_Cur, m_nMinDataCountOnOnePage);

	// 우측 여백 동작 방식
	if( 0 < dItemWidth && dItemWidth < 1)
			dItemWidth = 1.0 / ceil( 1 / dItemWidth);
	else	dItemWidth = ( int)dItemWidth;
	// (2009/1/8 - Seung-Won, Bae) Update Count and Start to avoid the right indent too large.
	if( 0 < dItemWidth) nDrawCount_Cur = ( int)( ( double)m_nScreenWidth_Cur[ 0] / dItemWidth);

	if( m_nItemCount_Total < nDrawCount_Cur)
	{
		nDrawCount_Cur = m_nItemCount_Total;
		dItemWidth = ( double)m_nScreenWidth_Cur[ 0] / ( double)max( nDrawCount_Cur, m_nMinDataCountOnOnePage);
		if( 0 < dItemWidth && dItemWidth < 1 && ceil( 1 / dItemWidth) != 1)
				dItemWidth = 1.0 / ( ceil( 1 / dItemWidth) - 1);
		else	dItemWidth = int( dItemWidth) + 1;
		if( 0 < dItemWidth) nDrawCount_Cur = ( int)( ( double)m_nScreenWidth_Cur[ 0] / dItemWidth);
	}
	return nDrawCount_Cur;
}


void CXScaleManagerImp::ClearData()
{
	// (2008/12/29 - Seung-Won, Bae) Do not use m_mifOEBT. Use m_pifaOEBT.
	//	// OEBT 초기화
	//	MAP_RQ_OEBT::iterator it_oebt = m_mifOEBT.begin();
	//	while(it_oebt != m_mifOEBT.end())
	//	{
	//		delete it_oebt->second;
	//		it_oebt++;
	//	}

	if( m_pblaRQTime != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pblaRQTime[ nRQ];
		delete [] m_pblaRQTime;
		m_pblaRQTime = NULL;
	}
	if(m_pblaDRIfDTI != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pblaDRIfDTI[ nRQ];
		delete [] m_pblaDRIfDTI;
		m_pblaDRIfDTI = NULL;
	}
	if(m_pblaDTIfDRI != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pblaDTIfDRI[ nRQ];
		delete [] m_pblaDTIfDRI;
		m_pblaDTIfDRI = NULL;
	}
	if(m_pifaRQ != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pifaRQ[ nRQ];
		delete [] m_pifaRQ;
		m_pifaRQ = NULL;
	}
	for( int i = 0; i < m_nCountOfBlockColumn; i ++) if( m_pblaAreafDTI[ i] != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pblaAreafDTI[ i][ nRQ];
		delete [] m_pblaAreafDTI[ i];
		m_pblaAreafDTI[ i] = NULL;
	}
	if(m_pifaOEBT != NULL)
	{
		// (2008/12/29 - Seung-Won, Bae) Do not use m_mifOEBT. Use m_pifaOEBT.
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pifaOEBT[ nRQ];
		delete [] m_pifaOEBT;
		m_pifaOEBT = NULL;
	}
	if( m_pamapSpecialTime != NULL)
	{
		for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++) delete m_pamapSpecialTime[ nRQ];
		delete [] m_pamapSpecialTime;
		m_pamapSpecialTime = NULL;
	}

	// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
	MAP_RQ_VERTBOX::iterator it = m_mifVertBox.begin();
	while(it != m_mifVertBox.end())
	{
		delete it->second;
		it++;
	}

	m_mifVertBox.clear();
}

// time_t를 double로 변환 : xScaleManager - ojtaso (20070405)
double CXScaleManagerImp::GetTimeValueFromTime(time_t tmValue, CString strType)
{
	CString strResult;
	CTime tmResult(tmValue);

	if(strType == "YYYYMM")
		strResult = tmResult.Format(_T("%Y%m"));
	else if(strType == "YYYYMMDD")
		strResult = tmResult.Format(_T("%Y%m%d"));
	else if(strType == "MMDDHHMM")
		strResult = tmResult.Format(_T("%m%d%H%M"));
	else if(strType == "HHMMSS")
		strResult = tmResult.Format(_T("%H%M%S"));
	else if(strType == "DDHHMMSS")
		strResult = tmResult.Format(_T("%d%H%M%S"));
	// 2011.01.11 by SYS >> 분차트 YYYY(년) 추가
	else if(strType == "YYYYMMDDHHMMSS")
		strResult = tmResult.Format(_T("%Y%m%d%H%M%S"));
	// 2011.01.11 by SYS <<
	else
		strResult = _T("0");

	return atof(strResult);
}

// (2008/12/28 - Seung-Won, Bae) Comment.
//	Get the time_t(second value) for YYYYMMDDhhmmss type.
// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
// time_t CXScaleManagerImp::GetTimeFromTimeValue( unsigned __int64 lTime, CString strType, RQ_INFO* pifRQ, tm &tmResult, const char *p_szRQ, CMapTimeToLong *p_pamapSpecialTime)
// {
// 	time_t timeResult = 0;
// 
// 	// 2011.01.20 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	// 날짜/시간이 14Byte 형태이면...
// 	if (strType == "YYYYMMDDHHMMSS" || lTime > 10000000000000)
// 	{
// 		time_t timeCur;
// 		tm* ptmCur;
// 		time(&timeCur);
// 		ptmCur = localtime(&timeCur);
// 		
// 		if (lTime > 10000000000000)
// 		{
// 			// 날짜Data가 YYYYMMDDhhmmss 인 경우..
// 			ptmCur->tm_year = (lTime / 10000000000) - 1900;
// 			ptmCur->tm_mon  = (lTime % 10000000000) / 100000000 - 1;
// 			ptmCur->tm_mday = (lTime % 100000000) / 1000000;
// 			ptmCur->tm_hour = (lTime % 1000000) / 10000;
// 			ptmCur->tm_min	= (lTime % 10000) / 100;
// 			ptmCur->tm_sec	= (lTime % 100);
// 			tmResult = *ptmCur;	
// 		}
// 		else if (lTime > 10000000)
// 		{
// 			// 날짜Data가 YYYYMMDD 인 경우..
// 			memset(&tmResult, 0, sizeof(tm));
// 			if(lTime)
// 			{
// 				tmResult.tm_year = (lTime / 10000) - 1900;
// 				tmResult.tm_mon  = (lTime % 10000) / 100 - 1;
// 				tmResult.tm_mday = (lTime % 100);
// 			}
// 		}
// 		else if (lTime > 100000)
// 		{
// 			// 날짜Data가 YYYYMM 인 경우..
// 			memset(&tmResult, 0, sizeof(tm));
// 			if(lTime)
// 			{
// 				tmResult.tm_year = (lTime / 100) - 1900;
// 				tmResult.tm_mon = (lTime % 100) - 1;
// 				tmResult.tm_mday = 1;
// 			}
// 		}
// 	}
// 	else
// 	// 2011.01.20 by SYS <<
// 	{
// 		// 1. if Monthly
// 		if( strType == "YYYYMM")
// 		{
// 			memset(&tmResult, 0, sizeof(tm));
// 
// 			if(lTime)
// 			{
// 				tmResult.tm_year = (lTime / 100) - 1900;
// 				tmResult.tm_mon = (lTime % 100) - 1;
// 				tmResult.tm_mday = 1;
// 			}
// 		}
// 		// 2. if Daily
// 		else if(strType == "YYYYMMDD")
// 		{
// 			memset(&tmResult, 0, sizeof(tm));
// 
// 			if(lTime)
// 			{
// 				tmResult.tm_year = (lTime / 10000) - 1900;
// 				tmResult.tm_mon  = (lTime % 10000) / 100 - 1;
// 				tmResult.tm_mday = (lTime % 100);
// 			}
// 		}
// 		// 3. if Minute unit
// 		else if(strType == "MMDDHHMM")
// 		{
// 			// (2009/1/25 - Seung-Won, Bae) Check special time.
// 			// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
// 			if( CChartInfo::WORLD_ON != m_eChartMode && p_pamapSpecialTime && g_iMetaTable.IsSpecialTime( ( lTime % 10000) * 100))
// 			{
// 				if( 0 == tmResult.tm_year)
// 				{
// 					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
// 					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
// 					if( ptmCur) tmResult = *ptmCur;
// 				}
// 				tmResult.tm_min++;
// 				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
// 				p_pamapSpecialTime->SetAt( timeResult, lTime % 10000);
// 			}
// 			else
// 			{
// 				// (2009/5/12 - Seung-Won, Bae) Do not check local time for speed.
// 				int nMon = ( lTime / 1000000) - 1;
// 				if( 0 == tmResult.tm_year)
// 				{
// 					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
// 					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
// 					if( ptmCur) tmResult = *ptmCur;
// 					if( lTime && nMon > tmResult.tm_mon) tmResult.tm_year--;
// 					tmResult.tm_mon = nMon;
// 				}
// 				if( lTime)
// 				{
// 					if( nMon < tmResult.tm_mon) tmResult.tm_year++;
// 					tmResult.tm_mon = nMon;
// 					tmResult.tm_mday = (lTime % 1000000) / 10000;
// 					tmResult.tm_hour = (lTime % 10000) / 100;
// 					tmResult.tm_min	= (lTime % 100);
// 				}
// 				tmResult.tm_sec = 0;
// 			}
// 		}
// 		// 4. if Second unit
// 		else if(strType == "HHMMSS")
// 		{
// 			// (2009/1/25 - Seung-Won, Bae) Check special time.
// 			// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
// 			if( CChartInfo::WORLD_ON != m_eChartMode && p_pamapSpecialTime && g_iMetaTable.IsSpecialTime( lTime))
// 			{
// 				if( 0 == tmResult.tm_year)
// 				{
// 					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
// 					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
// 					if( ptmCur) tmResult = *ptmCur;
// 				}
// 				tmResult.tm_sec++;
// 				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
// 				p_pamapSpecialTime->SetAt( timeResult, lTime);
// 			}
// 			else
// 			{
// 				// (2009/5/12 - Seung-Won, Bae) Do not check local time for speed.
// 				int nHour = (lTime / 10000);
// 				if( 0 == tmResult.tm_year)
// 				{
// 					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
// 					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
// 					if( ptmCur) tmResult = *ptmCur;
// 					if( lTime && nHour > tmResult.tm_hour) tmResult.tm_mday--;
// 					tmResult.tm_hour = nHour;
// 				}
// 				if( lTime)
// 				{
// 					if( nHour < tmResult.tm_hour) tmResult.tm_mday++;
// 					tmResult.tm_hour = nHour;
// 					tmResult.tm_min	= (lTime % 10000) / 100;
// 					tmResult.tm_sec	= (lTime % 100);
// 				}
// 
// 				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
// 			}
// 
// 			// ? why do you return directly ?
// 			//		Oh, it has second!
// 			return timeResult;
// 		}
// 		// 5. Second unit with day.
// 		else if (strType == "DDHHMMSS")
// 		{
// 			// (2009/1/25 - Seung-Won, Bae) Check special time.
// 			// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
// 			if( CChartInfo::WORLD_ON != m_eChartMode && p_pamapSpecialTime && g_iMetaTable.IsSpecialTime( lTime % 1000000))
// 			{
// 				if( 0 == tmResult.tm_year)
// 				{
// 					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
// 					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
// 					if( ptmCur) tmResult = *ptmCur;
// 				}
// 				tmResult.tm_sec++;
// 				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
// 				p_pamapSpecialTime->SetAt( timeResult, lTime % 1000000);
// 			}
// 			else
// 			{
// 				// (2009/5/12 - Seung-Won, Bae) Do not check local time for speed.
// 				int nDay = (lTime / 1000000);
// 				if( 0 == tmResult.tm_year)
// 				{
// 					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
// 					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
// 					if( ptmCur) tmResult = *ptmCur;
// 					if( lTime && nDay > tmResult.tm_mday) tmResult.tm_mon--;
// 					tmResult.tm_mday = nDay;
// 				}
// 				if(lTime)
// 				{
// 					if( nDay < tmResult.tm_mday) tmResult.tm_mon++;
// 					tmResult.tm_mday = nDay;
// 					tmResult.tm_hour = (lTime % 1000000) / 10000;
// 					tmResult.tm_min	= (lTime % 10000) / 100;
// 					tmResult.tm_sec	= (lTime % 100);
// 				}
// 
// 				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
// 			}
// 
// 			return timeResult;
// 		}
// 		else
// 		{
// 			// 6. ETC (current pc time ^^;)
// 			// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
// 			tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
// 			tmResult = *ptmCur;
// 		}
// 	}
// 
// 	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// //	timeResult = mktime(&tmResult);
// 	switch(pifRQ->eScaleUnit)
// 	{
// 		case CScaleBaseData::HORZ_MONTHLY:
// 		{
// 											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 일주월인 경우 날자계산 먼저 한 후 시차 처리한다.
// 											timeResult = mktime(&tmResult);
// 
// 											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. <<
// 											//	데이타처리 방식이 다름 => 2008년 8월 : 200808 으로 온다
// 											/*
// 											timeResult -= ((3600 * 24) * (tmResult.tm_mday - 1));
// 											timeResult -= (3600 * tmResult.tm_hour);
// 											timeResult -= tmResult.tm_sec;
// 											timeResult += 1;
// 											*/
// 											//>>
// 
// 											tm* ptmCur = localtime(&timeResult);
// 
// 											// 달의 시작이 일요일이면 월요일로 만들어 줍니다.
// 											if( ptmCur) if(ptmCur->tm_wday == 0) timeResult += (3600 * 24);
// 
// 											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 분봉을 함께 보는 경우 Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 											if( m_bTimeScale) timeResult = m_TimeDiffManager.GetRQScreenDateTime( p_szRQ, timeResult);
// 											break;
// 		}
// 
// 		case CScaleBaseData::HORZ_WEEKLEY:	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 일주월인 경우 날자계산 먼저 한 후 시차 처리한다.
// 											timeResult = mktime(&tmResult);
// 
// 											// 그 주의 월요일로 만들어 줍니다.
// 											if( tmResult.tm_wday < 1) tmResult.tm_wday += 7;
// 											timeResult -= ((3600 * 24) * (tmResult.tm_wday - 1));
// 
// 											// ? do you believe the hour, sec is not 0 ?
// 											//	why not minute?
// 											timeResult -= (3600 * tmResult.tm_hour);
// 											timeResult -= tmResult.tm_sec;
// 											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 
// 											//	분봉과 같이 비교할 때 봉 위치가 어긋나는 현상 발생하여 막음
// 											//timeResult += 1;
// 
// 											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 분봉을 함께 보는 경우 Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 											if( m_bTimeScale) timeResult = m_TimeDiffManager.GetRQScreenDateTime( p_szRQ, timeResult);
// 											break;
// 
// 		case CScaleBaseData::HORZ_DAILY:	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 일주월인 경우 날자계산 먼저 한 후 시차 처리한다.
// 											timeResult = mktime(&tmResult);
// 											// ? do you believe the hour, sec is not 0 ?
// 											//	why not minute?
// 											timeResult -= (3600 * tmResult.tm_hour);
// 											timeResult -= tmResult.tm_sec;
// 
// 											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 
// 											//	분봉과 같이 비교할 때 봉 위치가 어긋나는 현상 발생하여 막음
// 											//timeResult += 1;
// 
// 											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 분봉을 함께 보는 경우 Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 											if( m_bTimeScale) timeResult = m_TimeDiffManager.GetRQScreenDateTime( p_szRQ, timeResult);
// 											break;
// 
// 		case CScaleBaseData::HORZ_TIME_ONE:	
// 		case CScaleBaseData::HORZ_TIME_TWO:	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
// 											timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
// 											timeResult -= tmResult.tm_sec;
// 											break;
// 
// 		case CScaleBaseData::HORZ_TIME_SECOND:
// 		case CScaleBaseData::HORZ_TICK:
// 			// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 			if (strType == "YYYYMMDDHHMMSS" || lTime > 10000000000000)
// 			{
// 				//timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
// 				timeResult = mktime(&tmResult);
// 			}
// 			else
// 			{
// 				// it was returned directly!
// 			}
// 			// 2011.01.24 by SYS <<
// 			break;
// 	}
// 
// 	return timeResult;
// }

// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
time_t CXScaleManagerImp::GetTimeFromTimeValue( unsigned __int64 lTime, CString strType, RQ_INFO* pifRQ, tm &tmResult, const char *p_szRQ, CMapTimeToLong *p_pamapSpecialTime)
{
	time_t timeResult = 0;

	// 2011.01.20 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	// 날짜/시간이 14Byte 형태이면...
	if (strType == "YYYYMMDDHHMMSS" || lTime > 10000000000000)
	{
		time_t timeCur;
		tm* ptmCur;
		time(&timeCur);
		ptmCur = localtime(&timeCur);
		
		int nYear	= 0;
		int nMon	= 0;
		int nDay	= 0;
		int nHour	= 0;
		int nMin	= 0;
		int nSec	= 0;
		
		if (lTime > 10000000000000)
		{
			// 날짜Data가 YYYYMMDDhhmmss 인 경우..
			nYear	= (lTime / 10000000000) - 1900;
			nMon	= (lTime % 10000000000) / 100000000 - 1;
			nDay	= (lTime % 100000000) / 1000000;
			nHour	= (lTime % 1000000) / 10000;
			nMin	= (lTime % 10000) / 100;
			nSec	= (lTime % 100);
			
			switch(pifRQ->eScaleUnit)
			{
			case CScaleBaseData::HORZ_DAILY:		// 일
			case CScaleBaseData::HORZ_WEEKLEY:		// 주 
				memset(&tmResult, 0, sizeof(tm));
				tmResult.tm_year = nYear;
				tmResult.tm_mon  = nMon;
				tmResult.tm_mday = nDay;
				break;
			case CScaleBaseData::HORZ_MONTHLY:		// 월 
				memset(&tmResult, 0, sizeof(tm));
				tmResult.tm_year = nYear;
				tmResult.tm_mon  = nMon;
				tmResult.tm_mday = 1;
				break;
			case CScaleBaseData::HORZ_TIME_ONE:		// 분
			case CScaleBaseData::HORZ_TIME_TWO:
			case CScaleBaseData::HORZ_TICK:			// 틱 
			case CScaleBaseData::HORZ_TIME_SECOND:	// 초
				if (lTime > 10000000000000)
				{
					// 날짜Data가 YYYYMMDDhhmmss 인 경우..
					ptmCur->tm_year = nYear;
					ptmCur->tm_mon  = nMon;
					ptmCur->tm_mday = nDay;
					ptmCur->tm_hour = nHour;
					ptmCur->tm_min	= nMin;
					ptmCur->tm_sec	= nSec;
					tmResult = *ptmCur;	
				}			
				break;
			}
		}
		else
		{
			switch(pifRQ->eScaleUnit)
			{
			case CScaleBaseData::HORZ_DAILY:		// 일
			case CScaleBaseData::HORZ_WEEKLEY:		// 주 
				if (lTime > 10000000)
				{
					// 날짜Data가 YYYYMMDD 인 경우..
					memset(&tmResult, 0, sizeof(tm));
					if(lTime)
					{
						tmResult.tm_year = (lTime / 10000) - 1900;
						tmResult.tm_mon  = (lTime % 10000) / 100 - 1;
						tmResult.tm_mday = (lTime % 100);
					}
				}
				break;
			case CScaleBaseData::HORZ_MONTHLY:		// 월 
// vntsorl_20110407:[A00000378] Global 맵차트에서 월/분/틱 추세선이 안 그려지는 오류개선
				if (lTime > 10000000)
				{
					// 날짜Data가 YYYYMMDD 인 경우..
					memset(&tmResult, 0, sizeof(tm));
					if(lTime)
					{
						tmResult.tm_year = (lTime / 10000) - 1900;
						tmResult.tm_mon  = (lTime % 10000) / 100 - 1;
						tmResult.tm_mday = (lTime % 100);
					}
				}
				else if (lTime > 100000)
				{
					// 날짜Data가 YYYYMM 인 경우..
					memset(&tmResult, 0, sizeof(tm));
					if(lTime)
					{
						tmResult.tm_year = (lTime / 100) - 1900;
						tmResult.tm_mon = (lTime % 100) - 1;
						tmResult.tm_mday = 1;
					}
				}
				break;
			case CScaleBaseData::HORZ_TIME_ONE:		// 분
			case CScaleBaseData::HORZ_TIME_TWO:
// vntsorl_20110407:[A00000378] Global 맵차트에서 월/분/틱 추세선이 안 그려지는 오류개선
				// 날짜Data가 MMDDHHMM 인 경우..
				if (lTime > 1000000)
				{
					// 날짜Data가 YYYYMMDD 인 경우..
					memset(&tmResult, 0, sizeof(tm));
					if(lTime)
					{
						ptmCur->tm_mon  = (lTime / 1000000) - 1;
						ptmCur->tm_mday = (lTime / 10000) % 100;
						ptmCur->tm_hour = (lTime % 10000) / 100;
						ptmCur->tm_min	= (lTime % 100);
						ptmCur->tm_sec	= 0;
						tmResult = *ptmCur;	
					}
				}
				break;
			case CScaleBaseData::HORZ_TICK:			// 틱 
			case CScaleBaseData::HORZ_TIME_SECOND:	// 초
// vntsorl_20110407:[A00000378] Global 맵차트에서 월/분/틱 추세선이 안 그려지는 오류개선
				// 날짜Data가 DDHHMMSS 인 경우..
				if (lTime > 1000000)
				{
						ptmCur->tm_mday  = (lTime / 1000000);
						ptmCur->tm_hour = (lTime / 10000) % 100;
						ptmCur->tm_min = (lTime % 10000) / 100;
						ptmCur->tm_sec	= (lTime % 100);
						tmResult = *ptmCur;	
				}
				break;
				// 분초틱의 경우는 날짜시간 포맷이 14Byte로 와야 계산이 가능하다.
				// 만약, 이 경우라면 날짜시간에 문제가 있는 경우이다.
				// 로컬시간으로 셋팅해버리자.
// 				if (lTime > 10000000000000)
// 				{
// 					// 날짜Data가 YYYYMMDDhhmmss 인 경우..
// 					ptmCur->tm_year = (lTime / 10000000000) - 1900;
// 					ptmCur->tm_mon  = (lTime % 10000000000) / 100000000 - 1;
// 					ptmCur->tm_mday = (lTime % 100000000) / 1000000;
// 					ptmCur->tm_hour = (lTime % 1000000) / 10000;
// 					ptmCur->tm_min	= (lTime % 10000) / 100;
// 					ptmCur->tm_sec	= (lTime % 100);
//  					tmResult = *ptmCur;	
// 				}			
// 				break;
			}
		}
	}
	else
	// 2011.01.20 by SYS <<
	{
		// 1. if Monthly
		if( strType == "YYYYMM")
		{
			memset(&tmResult, 0, sizeof(tm));

			if(lTime)
			{
				tmResult.tm_year = (lTime / 100) - 1900;
				tmResult.tm_mon = (lTime % 100) - 1;
				tmResult.tm_mday = 1;
			}
		}
		// 2. if Daily
		else if(strType == "YYYYMMDD")
		{
			memset(&tmResult, 0, sizeof(tm));

			if(lTime)
			{
				tmResult.tm_year = (lTime / 10000) - 1900;
				tmResult.tm_mon  = (lTime % 10000) / 100 - 1;
				tmResult.tm_mday = (lTime % 100);
			}
		}
		// 3. if Minute unit
		else if(strType == "MMDDHHMM")
		{
			// (2009/1/25 - Seung-Won, Bae) Check special time.
			// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
			if( CChartInfo::WORLD_ON != m_eChartMode && p_pamapSpecialTime && g_iMetaTable.IsSpecialTime( ( lTime % 10000) * 100))
			{
				if( 0 == tmResult.tm_year)
				{
					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
					if( ptmCur) tmResult = *ptmCur;
				}
				tmResult.tm_min++;
				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
				p_pamapSpecialTime->SetAt( timeResult, lTime % 10000);
			}
			else
			{
				// (2009/5/12 - Seung-Won, Bae) Do not check local time for speed.
				int nMon = ( lTime / 1000000) - 1;
				if( 0 == tmResult.tm_year)
				{
					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
					if( ptmCur) tmResult = *ptmCur;
					if( lTime && nMon > tmResult.tm_mon) tmResult.tm_year--;
					tmResult.tm_mon = nMon;
				}
				if( lTime)
				{
					if( nMon < tmResult.tm_mon) tmResult.tm_year++;
					tmResult.tm_mon = nMon;
					tmResult.tm_mday = (lTime % 1000000) / 10000;
					tmResult.tm_hour = (lTime % 10000) / 100;
					tmResult.tm_min	= (lTime % 100);
				}
				tmResult.tm_sec = 0;
			}
		}
		// 4. if Second unit
		else if(strType == "HHMMSS")
		{
			// (2009/1/25 - Seung-Won, Bae) Check special time.
			// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
			if( CChartInfo::WORLD_ON != m_eChartMode && p_pamapSpecialTime && g_iMetaTable.IsSpecialTime( lTime))
			{
				if( 0 == tmResult.tm_year)
				{
					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
					if( ptmCur) tmResult = *ptmCur;
				}
				tmResult.tm_sec++;
				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
				p_pamapSpecialTime->SetAt( timeResult, lTime);
			}
			else
			{
				// (2009/5/12 - Seung-Won, Bae) Do not check local time for speed.
				int nHour = (lTime / 10000);
				if( 0 == tmResult.tm_year)
				{
					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
					if( ptmCur) tmResult = *ptmCur;
					if( lTime && nHour > tmResult.tm_hour) tmResult.tm_mday--;
					tmResult.tm_hour = nHour;
				}
				if( lTime)
				{
					if( nHour < tmResult.tm_hour) tmResult.tm_mday++;
					tmResult.tm_hour = nHour;
					tmResult.tm_min	= (lTime % 10000) / 100;
					tmResult.tm_sec	= (lTime % 100);
				}

				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
			}

			// ? why do you return directly ?
			//		Oh, it has second!
			return timeResult;
		}
		// 5. Second unit with day.
		else if (strType == "DDHHMMSS")
		{
			// (2009/1/25 - Seung-Won, Bae) Check special time.
			// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
			if( CChartInfo::WORLD_ON != m_eChartMode && p_pamapSpecialTime && g_iMetaTable.IsSpecialTime( lTime % 1000000))
			{
				if( 0 == tmResult.tm_year)
				{
					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
					if( ptmCur) tmResult = *ptmCur;
				}
				tmResult.tm_sec++;
				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
				p_pamapSpecialTime->SetAt( timeResult, lTime % 1000000);
			}
			else
			{
				// (2009/5/12 - Seung-Won, Bae) Do not check local time for speed.
				int nDay = (lTime / 1000000);
				if( 0 == tmResult.tm_year)
				{
					// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
					tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
					if( ptmCur) tmResult = *ptmCur;
					if( lTime && nDay > tmResult.tm_mday) tmResult.tm_mon--;
					tmResult.tm_mday = nDay;
				}
				if(lTime)
				{
					if( nDay < tmResult.tm_mday) tmResult.tm_mon++;
					tmResult.tm_mday = nDay;
					tmResult.tm_hour = (lTime % 1000000) / 10000;
					tmResult.tm_min	= (lTime % 10000) / 100;
					tmResult.tm_sec	= (lTime % 100);
				}

				// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
				timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
			}

			return timeResult;
		}
		else
		{
			// 6. ETC (current pc time ^^;)
			// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리.Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
			tm* ptmCur = m_TimeDiffManager.CalcCurrentDateTimToPacketTime( p_szRQ);
			tmResult = *ptmCur;
		}
	}

	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
//	timeResult = mktime(&tmResult);
	switch(pifRQ->eScaleUnit)
	{
		case CScaleBaseData::HORZ_MONTHLY:
		{
											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 일주월인 경우 날자계산 먼저 한 후 시차 처리한다.
											timeResult = mktime(&tmResult);

											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. <<
											//	데이타처리 방식이 다름 => 2008년 8월 : 200808 으로 온다
											/*
											timeResult -= ((3600 * 24) * (tmResult.tm_mday - 1));
											timeResult -= (3600 * tmResult.tm_hour);
											timeResult -= tmResult.tm_sec;
											timeResult += 1;
											*/
											//>>

											tm* ptmCur = localtime(&timeResult);

											// 달의 시작이 일요일이면 월요일로 만들어 줍니다.
											if( ptmCur) if(ptmCur->tm_wday == 0) timeResult += (3600 * 24);

											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 분봉을 함께 보는 경우 Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
											if( m_bTimeScale) timeResult = m_TimeDiffManager.GetRQScreenDateTime( p_szRQ, timeResult);
											break;
		}

		case CScaleBaseData::HORZ_WEEKLEY:	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 일주월인 경우 날자계산 먼저 한 후 시차 처리한다.
											timeResult = mktime(&tmResult);

											// 그 주의 월요일로 만들어 줍니다.
											if( tmResult.tm_wday < 1) tmResult.tm_wday += 7;
											timeResult -= ((3600 * 24) * (tmResult.tm_wday - 1));

											// ? do you believe the hour, sec is not 0 ?
											//	why not minute?
											timeResult -= (3600 * tmResult.tm_hour);
											timeResult -= tmResult.tm_sec;
											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 
											//	분봉과 같이 비교할 때 봉 위치가 어긋나는 현상 발생하여 막음
											//timeResult += 1;

											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 분봉을 함께 보는 경우 Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
											if( m_bTimeScale) timeResult = m_TimeDiffManager.GetRQScreenDateTime( p_szRQ, timeResult);
											break;

		case CScaleBaseData::HORZ_DAILY:	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 일주월인 경우 날자계산 먼저 한 후 시차 처리한다.
											timeResult = mktime(&tmResult);
											// ? do you believe the hour, sec is not 0 ?
											//	why not minute?
											timeResult -= (3600 * tmResult.tm_hour);
											timeResult -= tmResult.tm_sec;

											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 
											//	분봉과 같이 비교할 때 봉 위치가 어긋나는 현상 발생하여 막음
											//timeResult += 1;

											// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 분봉을 함께 보는 경우 Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
											if( m_bTimeScale) timeResult = m_TimeDiffManager.GetRQScreenDateTime( p_szRQ, timeResult);
											break;

		case CScaleBaseData::HORZ_TIME_ONE:	
		case CScaleBaseData::HORZ_TIME_TWO:	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. Packet의 현지시간으로 현재 화면에 보여지는 시간대의 시간을 구한다
											timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
											timeResult -= tmResult.tm_sec;
											break;

		case CScaleBaseData::HORZ_TIME_SECOND:
		case CScaleBaseData::HORZ_TICK:
			// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
			if (strType == "YYYYMMDDHHMMSS" || lTime > 10000000000000)
			{
				//timeResult = m_TimeDiffManager.GetRQScreenTime( p_szRQ, &tmResult);
				timeResult = mktime(&tmResult);
			}
			else
			{
				// it was returned directly!
			}
			// 2011.01.24 by SYS <<
			break;
	}

	return timeResult;
}
// 2011.01.27 by SYS <<

BOOL CXScaleManagerImp::IsUseable()
{
	return (m_nCountOfRQ != 0);
}

// OCX 세팅 : xScaleManager - ojtaso (20070504)
void CXScaleManagerImp::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_pIChartCtrl = p_pICharetCtrl;
	m_eChartMode = m_pIChartCtrl->GetChartMode();
}

// 봉간격 일정 : xScaleManager - ojtaso (20070528)
void CXScaleManagerImp::SetFixedInterval( BOOL bFixedInterval, BOOL bCalc)
{
	if( m_bFixedInterval && bFixedInterval
		|| !m_bFixedInterval && !bFixedInterval) return;

	m_bFixedInterval = bFixedInterval;
	if( bCalc && !m_pIChartCtrl->IsWholeView())
	{
		m_pIChartCtrl->ZoomTo( m_pIChartCtrl->GetOnePageDataCount(), FALSE);

		for( int i = 0; i < m_nCountOfBlockColumn; i++) Calc_ItemWidth( i, m_nDrawCount_Cur);

		// 3. Update ScrollBar and Zoom SliderBar
		m_pIChartCtrl->UpdateZoomAndScrollInfo( m_nItemCount_Total, m_nDRI_Start_Cur, m_nDrawCount_Cur);
	}
}

BOOL CXScaleManagerImp::IsFixedInterval()
{
	return m_bFixedInterval;
}

// xScale 초기화 : xScaleManager - ojtaso (20070628)
void CXScaleManagerImp::Initialize(BOOL bCalc/* = FALSE*/)
{
	m_bReverse = FALSE;					// 뒤집기
	m_bWeightedVolume = FALSE;			// 거래량 가중치
	m_bUseOEBTMargin = FALSE;			// 일목균형표 전용여백(사용여부)
	m_nLeftDataHideCount = 0;			// 좌측 데이터 숨기기
	// (2009/2/8 - Seung-Won, Bae) Set initial of Fixed Interval to false. and Do not load previous user setting.
	m_bFixedInterval = FALSE;			// 봉간격 일정 : xScaleManager - ojtaso (20070528)
	// (2009/2/11 - Seung-Won, Bae) Init Resize Type.
	m_rstype = RSAT_ITEM_COUNT_FIX;		// 리사이즈 봉(ITEM) 처리 방법	- [봉(ITEM)의 개수 고정 기능]
}

/*
// 문자형 데이타인지 : xScaleManager - ojtaso (20070703)
BOOL CXScaleManagerImp::IsStringDataType()
{
	// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
	if( !m_pifaRQ) return FALSE;
	if( !m_pifaRQ[ 0]) return FALSE;
	return m_pifaRQ[ 0]->m_bStringDataType;
}
*/

void CXScaleManagerImp::SetThreadFlag(int nFlag)
{
	m_nThread_Flag = nFlag;
}

int CXScaleManagerImp::GetThreadFlag()
{
	return m_nThread_Flag;
}

BOOL CXScaleManagerImp::IsSpecialTime( const char *p_szRQ, time_t p_time, long &p_lSpecialTimeCode)
{
	p_lSpecialTimeCode = 0;
	int nRQ = GetRQIndexFromString( p_szRQ);
	if( nRQ < 0) return FALSE;
	return m_pamapSpecialTime[ nRQ]->Lookup( p_time, p_lSpecialTimeCode);
}


// (2009/2/12 - Seung-Won, Bae) Support default data space.
void CXScaleManagerImp::SetDefaultDataSpace( int p_nDefaultDataSpace, int p_nMinDataCountOnOnePage)
{
	m_nDefaultDataSpace = p_nDefaultDataSpace;
	m_nMinDataCountOnOnePage = p_nMinDataCountOnOnePage;
}

//////////////////////////////////////////////////////////////////////
// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
//////////////////////////////////////////////////////////////////////


// (2009/1/7 - Seung-Won, Bae) Comment.
void CXScaleManagerImp::Calc_AREA( int p_nColumnCount)
{
	m_nCountOfBlockColumn = p_nColumnCount;

	// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
	if( m_bOnUserZoomChanged)
	{
		m_dLastItemWidth = m_daItemWidth[ 0];
		m_bOnUserZoomChanged = FALSE;
	}

	// (2009/1/13 - Seung-Won, Bae) for ReCalc AREA
	// (2009/1/16 - Seung-Won, Bae) Support the Re-Calculation Mode on OCX OnDrawing
	m_nCalculationMode = 2;					// 0:No-Calc, 1:CalcByReal, 2:CalcAll
	if( 1 == m_nCountOfBlockColumn && 1 == m_nCountOfRQ)
	{
		if( m_nDRI_Start_Pre == m_nDRI_Start_Cur
			&& m_nScreenLeft_Pre == m_nScreenLeft_Cur[ 0]
			&& m_nScreenWidth_Pre == m_nScreenWidth_Cur[ 0]
			&& m_daItemWidth_Pre == m_daItemWidth[ 0]
			&& ( m_bReverse_Pre && m_bReverse
				|| !m_bReverse_Pre && !m_bReverse))
				m_nCalculationMode = 1;			// 0:No-Calc, 1:CalcByReal, 2:CalcAll
		if( 1 == m_nCalculationMode				// 0:No-Calc, 1:CalcByReal, 2:CalcAll
			&& m_nDrawCount_Pre == m_nDrawCount_Cur
			&& 0 == m_nNeedCalcArea
			&& ( m_bWeightedVolume_Pre && m_bWeightedVolume
				|| !m_bWeightedVolume_Pre && !m_bWeightedVolume))
		{
			m_nCalculationMode = 0;				// 0:No-Calc, 1:CalcByReal, 2:CalcAll
			return;
		}
		if( 1 == m_nCalculationMode)			// 0:No-Calc, 1:CalcByReal, 2:CalcAll
			if( 1 != m_nNeedCalcArea
				|| m_bWeightedVolume
				|| m_pblaDRIfDTI[ 0]->GetItemCount() < m_nDefaultDataSpace)
					m_nCalculationMode = 2;			// 0:No-Calc, 1:CalcByReal, 2:CalcAll
		m_nDrawCount_Pre		= m_nDrawCount_Cur;		// the data count on view
		m_nDRI_Start_Pre		= m_nDRI_Start_Cur;		// the first data index on view
		m_nScreenLeft_Pre		= m_nScreenLeft_Cur[ 0];	// the graph area left position on view
		m_nScreenWidth_Pre		= m_nScreenWidth_Cur[ 0];	// the graph area width on view
		m_daItemWidth_Pre		= m_daItemWidth[ 0];			// the one data width on view
		m_bWeightedVolume_Pre	= m_bWeightedVolume;	// the Weighted Volume option.
		m_bReverse_Pre			= m_bReverse;			// the Reverse X option.
	}
	m_nNeedCalcArea			= 0;					// the Flag on by DRI() with full(2) or partial(1)

	int i = 0;
	// 1.3 Clear full Area Info.
	if( 1 != m_nCalculationMode)			// 0:No-Calc, 1:CalcByReal, 2:CalcAll
	{
		// 1. Recreate Area Info of all RQs
		// 1.1 Destroy old data.
		// 자료 설정
		for(  i = 0; i < m_nCountOfBlockColumn; i++)
		{
			if( m_pblaAreafDTI[ i])
			{
				for( int nRQ = 0; nRQ < m_nCountOfRQ_Area; nRQ++) delete m_pblaAreafDTI[ i][ nRQ];
				delete [] m_pblaAreafDTI[ i];
				m_pblaAreafDTI[ i] = NULL;
			}
			// 1.2 Create new data.
			m_pblaAreafDTI[ i] = ( CDataBlockList<AREA>**)(new LPVOID[ m_nCountOfRQ]);
			for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
				m_pblaAreafDTI[ i][ nRQ] = new CDataBlockList<AREA>;

			m_blAreafDRI[ i].Clear();
		}
		m_nCountOfRQ_Area = m_nCountOfRQ;

		// (2009/1/19 - Seung-Won, Bae) for Scale Time List
		signed short nTimeDiff = 0;
		CXScaleGridTime *pXScaleGridTime = NULL;
		POSITION psnXGridTime = m_mapXGridTime.GetStartPosition();
		while( psnXGridTime)
		{
			m_mapXGridTime.GetNextAssoc( psnXGridTime, nTimeDiff, pXScaleGridTime);
			if( !pXScaleGridTime) continue;
			delete pXScaleGridTime;
		}
		m_mapXGridTime.RemoveAll();
		m_mapXGridTime.SetAt( 0, new CXScaleGridTime);
		const char *szCurrentRQ = m_pIChartCtrl->GetCurrentRQ();
		if( szCurrentRQ) if( *szCurrentRQ)
		{
			CPacketList *pPacketListMain = m_ppacketListManager->GetPacketList( szCurrentRQ);
			if( pPacketListMain) for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
			{
				CString strRQ = m_saRQ.GetAt( nRQ);
				if( strRQ.IsEmpty()) continue;
				CPacketList *pPacketList = m_ppacketListManager->GetPacketList( strRQ);
				if( !pPacketList) continue;
				WORD wTimeDiff = pPacketList->GetBaseTimeDifference() - pPacketListMain->GetBaseTimeDifference();
				if( m_mapXGridTime.Lookup( wTimeDiff, pXScaleGridTime)) continue;
				m_mapXGridTime.SetAt( wTimeDiff, new CXScaleGridTime);
			}
		}
	}

	// (2009/2/8 - Seung-Won, Bae) Clear Scale Draw Info for Mode 1 and 2.
	for(  i = 0; i < m_nCountOfBlockColumn; i++)
	{
		m_blScaleTime[ i].Clear();
		m_blScaleDRI[ i].Clear();
	}

	if( m_nItemCount_Total == 0) return;

	// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
	for( i = 0; i < m_nCountOfBlockColumn; i++) Calc_AREAofColumn( i);
}


void CXScaleManagerImp::Calc_AREAofColumn( int p_nColumn)
{
	// (2009/2/19 - Seung-Won, Bae) Check No area.
	if( m_nScreenWidth_Cur[ p_nColumn] < 1) return;

	//	Calc_Area_AreafDRI();
	// 각 DRI별로 소유하는 영역 정보를 구하도록 합니다.
	// (2009/6/24 - Seung-Won, Bae) Support volume weighted x scale for whole view. (not special chart)
	if( ( m_bWeightedVolume && 0 <= m_minScaleInterval_nRQ && !m_pIChartCtrl->IsRunningOneChart()))
	{
		CPacket* pVolumePacket = m_pifaRQ[ m_minScaleInterval_nRQ]->pPacketRQ->GetPacketList()->GetBaseData( _MTEXT( C0_VOLUME));
		if( !pVolumePacket) return;
		CList< double, double>* pListVolume = pVolumePacket->GetnumericDataList();
		if( !pListVolume) return;
		CDataBlockList<int> *pblDTIfDRI = m_pblaDTIfDRI[ m_minScaleInterval_nRQ];
		CDataBlockList<int> *pblDRIfDTI = m_pblaDRIfDTI[ m_minScaleInterval_nRQ];
		int nDTI_Start_Cur_Org = 0, nDTI_End_Cur_Org = 0;
		int nDTI_Start_Cur	= GetDTIFromDRI_AbleRange( pblDTIfDRI, m_nDRI_Start_Cur, &nDTI_Start_Cur_Org);
		int nDTI_End_Cur	= GetDTIFromDRI_AbleRange( pblDTIfDRI, m_nDRI_End_Cur, &nDTI_End_Cur_Org);
		double dVolumeFactor = 0.;

		// Check there is any DTI in view.
		POSITION posVolume = NULL;
		if( !( nDTI_Start_Cur_Org < -1			// Graph is gone to left.
				|| -1 == nDTI_End_Cur_Org))		// Graph is gone to right.
		{
			double dWidth_Total = 0, dVolume_Total = 0;
			int nNoVolumeCount = 0;

			// Sum of all volume in view.
			posVolume = pListVolume->FindIndex( nDTI_Start_Cur);
			for( int nDTI = nDTI_Start_Cur; nDTI <= nDTI_End_Cur; nDTI++)
			{
				if( posVolume)	dVolume_Total += pListVolume->GetNext( posVolume);
				else			nNoVolumeCount++;
			}

			// Graph Drawing Area ?
			int nDRI_Start	= -1;
			m_pIReturn = pblDRIfDTI->GetAt( nDTI_Start_Cur);

			if (m_pIReturn)	//tour2k
			nDRI_Start = *m_pIReturn;
			int nDRI_End	= -1;
			m_pIReturn = pblDRIfDTI->GetAt( nDTI_End_Cur);

			if (m_pIReturn)//tour2k
			nDRI_End = *m_pIReturn;
			if( nDRI_Start < m_nDRI_Start_Cur)	nDRI_Start	= m_nDRI_Start_Cur;
			if( m_nDRI_End_Cur < nDRI_End)		nDRI_End	= m_nDRI_End_Cur;
			dWidth_Total = ( nDRI_End - nDRI_Start + 1 - nNoVolumeCount) * m_daItemWidth[ p_nColumn];

			if( 0 < dVolume_Total) dVolumeFactor = dWidth_Total / dVolume_Total;
		}

		int nDTI = 0;
		double dItemWidth_Cur = 0;
		int nDRICount = pblDTIfDRI->GetItemCount();
		BOOL bNoVolume = ( nDRICount == 0 || dVolumeFactor == 0.0);
		int nVolumeCount = pListVolume->GetCount();
		posVolume = NULL;
		double dVolume = 0.0;
		int nPreDTI = -2;		// do you know why -2 ?
		if( !m_bReverse)
		{
			AREA area( 0, 0, m_nScreenLeft_Cur[ p_nColumn]);
			double dLeft_Cur = m_nScreenLeft_Cur[ p_nColumn];
			m_blTimefDRI.SetIndex( m_nDRI_Start_Cur);
			int nRealDRIEnd = min( m_blTimefDRI.GetItemCount() - 1, m_nDRI_End_Cur);	// for scroll to pass or future
			for( int nDRI = m_nDRI_Start_Cur; nDRI <= m_nDRI_End_Cur + 1; nDRI++)	// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle
			{
				nDTI = -1;
				if( !bNoVolume && 0 <= nDRI && nDRI < nDRICount)
				{
					m_pIReturn = pblDTIfDRI->GetAt( nDRI);
					if( m_pIReturn) nDTI = *m_pIReturn;
				}

				if( bNoVolume || nDTI < 0 || nVolumeCount <= nDTI)
						dItemWidth_Cur = m_daItemWidth[ p_nColumn];
				else
				{
					if( nPreDTI != nDTI)
					{
						if( nPreDTI + 1 != nDTI) posVolume = pListVolume->FindIndex( nDTI);
						dVolume = pListVolume->GetNext( posVolume);
						nPreDTI = nDTI;
					}
					dItemWidth_Cur = dVolume * dVolumeFactor;
				}

				area.left = area.right;
				dLeft_Cur += dItemWidth_Cur;
				area.right = int(dLeft_Cur);
				area.center = (area.left + area.right) / 2;

				m_blAreafDRI[ p_nColumn].PushBack(area);

				// (2009/1/19 - Seung-Won, Bae) for Scale Time List
				if( 0 == p_nColumn)
				{
					m_pTReturn = m_blTimefDRI.GetNext();
					if( m_pTReturn) PushScaleDataTime( nDRI, *m_pTReturn, nDRI == nRealDRIEnd);
				}
			}
		}
		else
		{
			AREA area( m_nScreenLeft_Cur[ p_nColumn] + m_nScreenWidth_Cur[ p_nColumn], 0, 0);
			double dLeft_Cur = m_nScreenLeft_Cur[ p_nColumn] + m_nScreenWidth_Cur[ p_nColumn];
			m_blTimefDRI.SetIndex( m_nDRI_Start_Cur);
			int nRealDRIEnd = min( m_blTimefDRI.GetItemCount() - 1, m_nDRI_End_Cur);	// for scroll to pass or future
			for( int nDRI = m_nDRI_Start_Cur; nDRI <= m_nDRI_End_Cur + 1; nDRI++)	// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle
			{
				nDTI = -1;
				if( !bNoVolume && 0 <= nDRI && nDRI < nDRICount)
				{
					m_pIReturn = pblDTIfDRI->GetAt( nDRI);
					if( m_pIReturn) nDTI = *m_pIReturn;
				}

				if( bNoVolume || nDTI < 0 || nVolumeCount <= nDTI)
						dItemWidth_Cur = m_daItemWidth[ p_nColumn];
				else
				{
					if( nPreDTI != nDTI)
					{
						if( nPreDTI + 1 != nDTI) posVolume = pListVolume->FindIndex( nDTI);
						dVolume = pListVolume->GetNext( posVolume);
						nPreDTI = nDTI;
					}
					dItemWidth_Cur = dVolume * dVolumeFactor;
				}

				area.right = area.left;
				dLeft_Cur -= dItemWidth_Cur;
				area.left = int(dLeft_Cur);
				area.center = (area.left + area.right) / 2;

				m_blAreafDRI[ p_nColumn].PushBack(area);

				// (2009/1/19 - Seung-Won, Bae) for Scale Time List
				if( 0 == p_nColumn)
				{
					m_pTReturn = m_blTimefDRI.GetNext();
					if( m_pTReturn) PushScaleDataTime( nDRI, *m_pTReturn, nDRI == nRealDRIEnd);
				}
			}
		}
	}
	else
	{
		// (2009/2/12 - Seung-Won, Bae) Support default data space.
		int nPreAreaCount = m_blAreafDRI[ 0].GetItemCount();
		if( 1 == m_nCalculationMode
			&& m_blTimefDRI.GetItemCount() <= m_nDRI_End_Cur + 1
			&& m_nDRI_End_Cur - m_nDRI_Start_Cur + 1 + 1 == nPreAreaCount) // (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle
		{
			int nDRI_Start_Cur = 0;
			CXScaleGridTime *pXScaleGridTime = NULL;
			if( m_mapXGridTime.Lookup( 0, pXScaleGridTime)) nDRI_Start_Cur = pXScaleGridTime->GetLastDRI();
			PopLastScaleDataTime();
			m_blTimefDRI.SetIndex( nDRI_Start_Cur);
			int nRealDRIEnd = min( m_blTimefDRI.GetItemCount() - 1, m_nDRI_End_Cur);	// for scroll to pass or future
			for( int nDRI = nDRI_Start_Cur; nDRI <= m_nDRI_End_Cur; nDRI++)
			{
				// (2009/1/19 - Seung-Won, Bae) for Scale Time List
				m_pTReturn = m_blTimefDRI.GetNext();
				if( m_pTReturn) PushScaleDataTime( nDRI, *m_pTReturn, nDRI == nRealDRIEnd);
			}
		}
		else if( !m_bReverse)
		{
			double dLeft_Cur = m_nScreenLeft_Cur[ p_nColumn];
			// (2009/1/14 - Seung-Won, Bae) Support Pixel Arranging.
			int nDRI_Start_Cur = m_nDRI_Start_Cur;
			if( 1 == m_nCalculationMode)		// 0:No-Calc, 1:CalcByReal, 2:CalcAll
			{
				// (2009/2/11 - Seung-Won, Bae) Recalc pre-last one for updated new time.
				dLeft_Cur += m_daItemWidth[ 0] * ( nPreAreaCount - 1 - 1);	// (2009/3/2 - Seung-Won, Bae) -1 for right of Last Candle
				nDRI_Start_Cur = m_nDRI_End_Pre;
				m_blAreafDRI[ 0].DeleteBack( 1 + 1);						// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle

				// (2009/1/19 - Seung-Won, Bae) for Scale Time List
				PopLastScaleDataTime();
			}
			AREA area( 0, 0, dLeft_Cur);
			m_blTimefDRI.SetIndex( nDRI_Start_Cur);
			int nRealDRIEnd = min( m_blTimefDRI.GetItemCount() - 1, m_nDRI_End_Cur);	// for scroll to pass or future
			for( int nDRI = nDRI_Start_Cur; nDRI <= m_nDRI_End_Cur + 1; nDRI++)		// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle
			{
				area.left = area.right;
				dLeft_Cur += m_daItemWidth[ p_nColumn];
				area.right = int(dLeft_Cur);
				area.center = (area.left + area.right) / 2;
				m_blAreafDRI[ p_nColumn].PushBack(area);

				// (2009/1/19 - Seung-Won, Bae) for Scale Time List
				if( 0 == p_nColumn && nDRI <= m_nDRI_End_Cur)
				{
					m_pTReturn = m_blTimefDRI.GetNext();
					if( m_pTReturn) PushScaleDataTime( nDRI, *m_pTReturn, nDRI == nRealDRIEnd);
				}
			}
		}
		else
		{
			double dLeft_Cur = m_nScreenLeft_Cur[ p_nColumn] + m_nScreenWidth_Cur[ p_nColumn];
			// (2009/1/14 - Seung-Won, Bae) Support Pixel Arranging.
			int nDRI_Start_Cur = m_nDRI_Start_Cur;
			if( 1 == m_nCalculationMode)		// 0:No-Calc, 1:CalcByReal, 2:CalcAll
			{
				// (2009/2/11 - Seung-Won, Bae) Recalc pre-last one for updated new time.
				dLeft_Cur -= m_daItemWidth[ 0] * ( nPreAreaCount - 1 - 1);	// (2009/3/2 - Seung-Won, Bae) -1 for right of Last Candle
				nDRI_Start_Cur = m_nDRI_End_Pre;
				m_blAreafDRI[ 0].DeleteBack( 1 + 1);						// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle

				// (2009/1/19 - Seung-Won, Bae) for Scale Time List
				PopLastScaleDataTime();
			}
			AREA area( dLeft_Cur, 0, 0);
			m_blTimefDRI.SetIndex( nDRI_Start_Cur);
			int nRealDRIEnd = min( m_blTimefDRI.GetItemCount() - 1, m_nDRI_End_Cur);	// for scroll to pass or future
			for( int nDRI = nDRI_Start_Cur; nDRI <= m_nDRI_End_Cur + 1; nDRI++)		// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle
			{
				area.right = area.left;
				dLeft_Cur -= m_daItemWidth[ p_nColumn];
				area.left = int(dLeft_Cur);
				area.center = (area.left + area.right) / 2;
				m_blAreafDRI[ p_nColumn].PushBack(area);

				// (2009/1/19 - Seung-Won, Bae) for Scale Time List
				if( 0 == p_nColumn && nDRI <= m_nDRI_End_Cur)
				{
					m_pTReturn = m_blTimefDRI.GetNext();
					if( m_pTReturn) PushScaleDataTime( nDRI, *m_pTReturn, nDRI == nRealDRIEnd);
				}
			}
		}
		m_nDRI_End_Pre = m_nDRI_End_Cur;
	}
	
	for( int nRQ = 0; nRQ < m_nCountOfRQ; nRQ++)
	{
		int nDTI_Start_Cur_Org = 0, nDTI_End_Cur_Org = 0;
		int nDTI_Start_Cur	= GetDTIFromDRI_AbleRange( m_pblaDTIfDRI[nRQ], m_nDRI_Start_Cur,	&nDTI_Start_Cur_Org);
		int nDTI_End_Cur	= GetDTIFromDRI_AbleRange( m_pblaDTIfDRI[nRQ], m_nDRI_End_Cur,		&nDTI_End_Cur_Org);
		
		if( nDTI_Start_Cur_Org < -1		// Graph is gone to left.
			|| -1 == nDTI_End_Cur_Org)	// Graph is gone to right.
				continue;

		if(nDTI_Start_Cur < 1) nDTI_Start_Cur = 0;
		else				   nDTI_Start_Cur--;

		// (2009/1/14 - Seung-Won, Bae) Support Pixel Arranging.
		int nDTI_End_Pre	= m_pifaRQ[nRQ]->nDTI_End_Data;
		if( 2 == m_nCalculationMode)		// 0:No-Calc, 1:CalcByReal, 2:CalcAll
			nDTI_End_Pre	= nDTI_Start_Cur;

		m_pifaRQ[nRQ]->nDTI_Start_Data = nDTI_Start_Cur;
		m_pifaRQ[nRQ]->nDTI_End_Data = nDTI_End_Cur;

		int nDRI = -1;
		m_pIReturn = m_pblaDRIfDTI[ nRQ]->GetAt( nDTI_End_Pre);
		if( m_pIReturn) nDRI = *m_pIReturn;

		// (2009/1/3 - Seung-Won, Bae) for Exception. (no data?)
		if( nDRI < 0) continue;

		// (2009/1/14 - Seung-Won, Bae) Support Pixel Arranging.
		if( 1 == m_nCalculationMode)		// 0:No-Calc, 1:CalcByReal, 2:CalcAll
			m_pblaAreafDTI[ 0][ nRQ]->DeleteBack( 1);		// for update last DTI AREA.

		AREA area( 0, 0, 0);
		GetAreaFromDRI_Copy( p_nColumn, nDRI, area);

		int nDRI_Next = 0;
		AREA area_Next( 0, 0, 0);
		for( int nDTI = nDTI_End_Pre; nDTI <= nDTI_End_Cur; nDTI++)
		{
			m_pIReturn = m_pblaDRIfDTI[ nRQ]->GetAt( nDTI + 1);
			if( m_pIReturn) nDRI_Next = *m_pIReturn;

			GetAreaFromDRI_Copy( p_nColumn, nDRI_Next, area_Next);

			if( !m_bReverse)
			{
				area.right = area_Next.left;
				ItemIntervalApply( p_nColumn, area, nDRI_Next - nDRI);
			}
			else
			{
				area.left = area_Next.right;
				ItemIntervalApply_Reverse( p_nColumn, area, nDRI_Next - nDRI);
			}

			area.center = (area.left + area.right) / 2;

			m_pblaAreafDTI[ p_nColumn][ nRQ]->PushBack(area);

			area = area_Next;
			nDRI = nDRI_Next;
		}
	}
}


// (2009/1/3 - Seung-Won, Bae) Comment.
// 좌측데이터 숨기기를 할경우에 숨겨진 데이터의 DRI는 -1(음수)가 될수 있고
// 그럴경우 해당 데이터의 영역정보도 넘겨줄수 있도록 하기 위해서
// 음수에 대해서 따로 막는 작업을 하지 않습니다.
void CXScaleManagerImp::GetAreaFromDRI_Copy( int p_nColumn, int nDRI, AREA &area)
{
	// on View
	if( m_nDRI_Start_Cur <= nDRI && nDRI <= m_nDRI_End_Cur + 1)		// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle
	{
		m_pAReturn = m_blAreafDRI[ p_nColumn].GetAt(nDRI - m_nDRI_Start_Cur);
		if( m_pAReturn) area = *m_pAReturn;
		return;
	}

	// left on view.
	int nDRI_Gap = 0;
	if( nDRI < m_nDRI_Start_Cur)
	{
		m_pAReturn = m_blAreafDRI[ p_nColumn].GetAt( 0);
		if( m_pAReturn) area = *m_pAReturn;

		nDRI_Gap = m_nDRI_Start_Cur - nDRI;
	}
	else
	{
		// right on view.
		m_pAReturn = m_blAreafDRI[ p_nColumn].GetAt( m_nDrawCount_Cur - 1);
		if( m_pAReturn) area = *m_pAReturn;

		nDRI_Gap = nDRI - m_nDRI_End_Cur;
	}

	double dGap = nDRI_Gap * m_daItemWidth[ p_nColumn];
	if( dGap < SHRT_MIN) dGap = SHRT_MIN;
	if( SHRT_MAX < dGap) dGap = SHRT_MAX;
	if( ( nDRI < m_nDRI_Start_Cur && !m_bReverse) || ( m_nDRI_End_Cur < nDRI && m_bReverse))
	{
		area.right = area.left + m_naItemWidth[ p_nColumn];
		area.Offset( ROUND( -dGap));
	}
	else
	{
		area.left = area.right - m_naItemWidth[ p_nColumn];
		area.Offset( ROUND( dGap));
	}
}


void CXScaleManagerImp::ItemIntervalApply( int p_nColumn, AREA& area, int nDRI_Count)
{
	int nItemWidth_Cur = area.right - area.left;

	if( m_bWeightedVolume == FALSE && nDRI_Count == 1)
	{
		switch( m_naItemWidth[ p_nColumn])
		{
		case 1:
			{
				if(nItemWidth_Cur == 2) area.left++;
			}
			break;

		case 2:
			{
				if(nItemWidth_Cur == 2) area.right++;
			}
			break;

		case 3:
			{
				if(nItemWidth_Cur == 4) area.left++;
			}
			break;

		default:
			{
				if( ( m_naItemWidth[ p_nColumn] % 2) == 1)
				{
					if( ( nItemWidth_Cur % 2) == 1)	area.left+=2;
					else							area.left+=3;
				}
				else
				{
					if( ( nItemWidth_Cur % 2) == 1)	area.left+=2;
					else							area.left+=1;
					if( area.right < area.left)		area.left = area.right = area.center;
				}
			}
			break;
		}
	}
	else
	{
		if( nItemWidth_Cur > 1)
		{
			if( ( nItemWidth_Cur % 2) == 1)	area.left+=2;
			else							area.left+=1;
		}
	}
}


void CXScaleManagerImp::ItemIntervalApply_Reverse( int p_nColumn, AREA& area, int nDRI_Count)
{
	int nItemWidth_Cur = area.right - area.left;

	if( m_bWeightedVolume == FALSE && nDRI_Count == 1)
	{
		switch( m_naItemWidth[ p_nColumn])
		{ 
		case 1:
			{
				if(nItemWidth_Cur == 2) area.right--;
			}
			break;

		case 2:
			{
				if(nItemWidth_Cur == 2) area.left--;
			}
			break;

		case 3:
			{
				if(nItemWidth_Cur == 4) area.right--;
			}
			break;

		default:
			{
				if( ( m_naItemWidth[ p_nColumn] % 2) == 1)
				{
					if( ( nItemWidth_Cur % 2) == 1)	area.right-=2;
					else							area.right-=3;
				}
				else
				{
					if( ( nItemWidth_Cur % 2) == 1)	area.right-=2;
					else							area.right-=1;
					if( area.right < area.left)		area.left = area.right = area.center;
				}
			}
			break;
		}
	}
	else
	{
		if( nItemWidth_Cur > 1)
		{
			if( ( nItemWidth_Cur % 2) == 1)	area.right-=2;
			else							area.right-=1;
		}
	}
}


// 우측여백도 포함된 시간예상 : xScaleManager - ojtaso (20070608)
int CXScaleManagerImp::GetDRInFromPt( int nCursor_x, int *pnBlockColumn, PAREA* pparea, BOOL bIncludeRightMargin, BOOL p_bWithBoundCheck)
{
	int nColumn = 0;
	for( int i = 0; i < m_nCountOfBlockColumn; i++)
		if( m_naScreenRegionLeft[ i] < nCursor_x && nCursor_x < m_naScreenRegionLeft[ i] + m_naScreenRegionWidth[ i])
	{
		nColumn = i;
		break;
	}

	int nDRI;
	int nDRI_End = m_nItemCount_Total - 1;
	int nDRI_EndiM = nDRI_End;

	PAREA parea = &m_areaNONE;

	if(m_bReverse == FALSE)
	{
		int nX = nCursor_x - m_naScreenStart_Cur[ nColumn];
		if( 0 < m_daItemWidth[ nColumn]) nDRI = ceil((double)nX / m_daItemWidth[ nColumn]) - 1;
	}
	else
	{
		int nX = m_naScreenStart_Cur[ nColumn] - nCursor_x;
		if( 0 < m_daItemWidth[ nColumn]) nDRI = ceil((double)nX / m_daItemWidth[ nColumn]) - 1;
	}
	
	// 우측여백 포함 계산 : xScaleManager - ojtaso (20070611)
	if( bIncludeRightMargin && !m_pIChartCtrl->IsRunningOneChart() && m_nRightMarginWidth && 0 < m_daItemWidth[ nColumn])
		nDRI_End += ceil((double)m_nRightMarginWidth / m_daItemWidth[ nColumn]);

	if(m_bReverse == FALSE)
	{
		nDRI = m_nDRI_Start_Cur + nDRI;	// DRI 위치를 계산으로 추측합니다.

		// (2008/8/14 - Seung-Won, Bae) Support Checking of DRI Bound
		if( p_bWithBoundCheck && ( nDRI < 0 || nDRI > nDRI_End)) return -1;

		if(nDRI < 0)		nDRI = 0;
		if(nDRI > nDRI_End)	nDRI = nDRI_End;

		GetAreaFromDRI( nColumn, nDRI, &parea);

		// (2008/9/8 - Seung-Won, Bae) Check first for nDRI of bound.
		if(parea) while(nDRI > 0 && nCursor_x < parea->left)
		{
			nDRI--;
			GetAreaFromDRI( nColumn, nDRI, &parea);
			if( !parea) break;
		}
		if(parea) while(nDRI < nDRI_End && nCursor_x >= parea->right)
		{
			nDRI++;
			GetAreaFromDRI( nColumn, nDRI, &parea);
			if( !parea) break;
		}
	}
	else
	{
		nDRI = m_nDRI_End_Cur - nDRI;	// DRI 위치를 계산으로 추측합니다.

		// (2008/8/14 - Seung-Won, Bae) Support Checking of DRI Bound
		if( p_bWithBoundCheck && ( nDRI < 0 || nDRI > nDRI_End)) return -1;
		
		if(nDRI < 0)		nDRI = 0;
		if(nDRI > nDRI_End)	nDRI = nDRI_End;

		GetAreaFromDRI( nColumn, nDRI, &parea);

		if(parea)
		{
			// (2008/9/8 - Seung-Won, Bae) Check first for nDRI of bound.
			while(nDRI < nDRI_End && nCursor_x < parea->left)
			{
				nDRI++;
				GetAreaFromDRI( nColumn, nDRI, &parea);
			}
			while(nDRI > 0 && nCursor_x >= parea->right)
			{
				nDRI--;
				GetAreaFromDRI( nColumn, nDRI, &parea);
			} 
		}
	}

	if( pnBlockColumn) *pnBlockColumn = nColumn;
	if( pparea != NULL) *pparea = parea;
	return nDRI;
}


int CXScaleManagerImp::GetGraphXArea_End( int p_nColumn, BOOL bIncludeMargin)
{
	if(bIncludeMargin == FALSE)	return m_naScreenEnd_Cur[ p_nColumn];
	else						return m_nScreenEndiM_Cur[ p_nColumn];
}

int CXScaleManagerImp::GetGraphXArea_Left( int p_nColumn, BOOL bIncludeMargin)
{
	if(bIncludeMargin == FALSE)	return m_nScreenLeft_Cur[ p_nColumn];
	else						return m_nScreenLeftiM_Cur[ p_nColumn];
}


int CXScaleManagerImp::GetGraphXArea_Right( int p_nColumn, BOOL bIncludeMargin)
{
	if(bIncludeMargin == FALSE)	return m_nScreenRight_Cur[ p_nColumn];
	else						return m_nScreenRightiM_Cur[ p_nColumn];
}


int CXScaleManagerImp::GetGraphXArea_Start( int p_nColumn, BOOL bIncludeMargin)
{
	if(bIncludeMargin == FALSE) return m_naScreenStart_Cur[ p_nColumn];
	else						return m_nScreenStartiM_Cur[ p_nColumn];
}


int CXScaleManagerImp::GetScaleDRIandTime( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
				const int p_nScaleRegionWidth, const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const int p_nTimeInterval,
				const char *p_szRQ, int &p_nSkipFactor, const CString &p_strPacketType)
{
	// 1. Get X Scale Grid Object.
	CXScaleGridTime *pXScaleGridTime = NULL;
	int nTimeDiff = GetTimeDiffWithCurrentRQ( p_szRQ);
	if( !m_mapXGridTime.Lookup( nTimeDiff, pXScaleGridTime)) return -1;

	p_pblScaleDRI = &m_blScaleDRI[ p_nColumn];			// DRI value for scale of YYYY/MM/DD/HH/mm/SS
	p_pblScaleTime = &m_blScaleTime[ p_nColumn];		// Time value for scale of YYYY/MM/DD/HH/mm/SS

	int nRQ = GetRQIndexFromString( p_szRQ);
	if( nRQ < 0) return -1;

//#ifndef _DEBUG
//	if( 0 < m_blScaleDRI[ p_nColumn].GetItemCount()
//		&& p_eScaleDrawType == m_eScaleDrawType
//		&& p_szRQ == m_strScaleRQ)
//	{
//		p_nSkipFactor = m_nSkipFactor;
//		return m_naScaleType[ p_nColumn];
//	}
//#endif

	m_blScaleDRI[ p_nColumn].Clear();		// DRI value for scale of YYYY/MM/DD/HH/mm/SS
	m_blScaleTime[ p_nColumn].Clear();		// Time value for scale of YYYY/MM/DD/HH/mm/SS

	// (2009/5/28 - Seung-Won, Bae) Do not draw scale on no data.
	if( IsEmptyData()) return -1;

	int nStartDRIofRQ = -1;
	m_pIReturn = m_pblaDRIfDTI[ nRQ]->GetAt( m_pifaRQ[ nRQ]->nDTI_Start_Data);
	if( m_pIReturn) nStartDRIofRQ = *m_pIReturn;
	int nEndDRIofRQ = -1;
	m_pIReturn = m_pblaDRIfDTI[ nRQ]->GetAt( m_pifaRQ[ nRQ]->nDTI_End_Data);
	if( m_pIReturn) nEndDRIofRQ = *m_pIReturn;

	p_nSkipFactor = 1;	// p_nSkipFactor 개수 중에 하나면 출력됨
	m_naScaleType[ p_nColumn] = pXScaleGridTime->RecalculateScaleType( p_strPacketType, p_eScaleDrawType, p_nScaleRegionWidth, p_nSkipFactor, p_nTimeInterval, m_eChartMode);
	pXScaleGridTime->MakeScaleData( m_naScaleType[ p_nColumn], m_blScaleTime[ p_nColumn], m_blScaleDRI[ p_nColumn], p_nSkipFactor, m_eChartMode);

	m_nSkipFactor = p_nSkipFactor;
	m_eScaleDrawType = p_eScaleDrawType;
	m_strScaleRQ = p_szRQ;

	return m_naScaleType[ p_nColumn];
}


int	CXScaleManagerImp::GetScreenWidth( int p_nColumn)
{
	if( p_nColumn < 0 && m_nCountOfBlockColumn <= p_nColumn) return 0;
	return m_nScreenWidth_Cur[ p_nColumn];
}


// 우측여백도 포함된 X예상 : xScaleManager - ojtaso (20070608)
int CXScaleManagerImp::GetXFromTime( int p_nColumn, time_t timeFind, PAREA* pparea, BOOL bIncludeRightMargin)
{
	int nDRI;
	int nDRI_Prev, nDRI_Next;
	time_t timePrev, timeNext;
	int nTimeSpan_Prev, nTimeSpan_Next;

	int nCountOfDRI = m_blTimefDRI.GetItemCount();

	time_t timeFirst, timeLast, timeDRI_Start, timeTemp;
	m_pTReturn = m_blTimefDRI.GetAt( 0);
	if( m_pTReturn) timeFirst = *m_pTReturn;
	m_pTReturn = m_blTimefDRI.GetAt( nCountOfDRI - 1);
	timeLast = *m_pTReturn;

	int nTimeSpan = timeLast - timeFirst;
	int nTimeSpan_Find = timeFind - timeFirst;
	double dTimeSpan_Item = (double)nTimeSpan;
	if( 0 < nCountOfDRI) dTimeSpan_Item /= (double)nCountOfDRI;

	if( 0 < dTimeSpan_Item) nDRI = ceil((double)nTimeSpan_Find / dTimeSpan_Item);

	if(nDRI < 0)				nDRI = 0;
	if(nDRI >= nCountOfDRI)		nDRI = nCountOfDRI - 1;

	m_pTReturn = m_blTimefDRI.GetAt( nDRI);
	if( m_pTReturn) timeDRI_Start = *m_pTReturn;

	m_blTimefDRI.SetIndex(nDRI);

	if(timeDRI_Start > timeFind)
	{
		do
		{
			m_pTReturn = m_blTimefDRI.GetPrev();
			if( m_pTReturn) timeTemp = *m_pTReturn;
		} while( timeTemp >= timeFind && m_blTimefDRI.GetIndex() >= 0);
		
		if(timeTemp >= timeFind && m_blTimefDRI.GetIndex() < 0)
		{
			nDRI = 0;
		}
		else
		{
			// timeFind보다 바로 작은거
			nDRI_Prev = m_blTimefDRI.GetIndex() + 1;
			nDRI_Next = nDRI_Prev + 1;

			// 가장 가까운 DRI를 찾습니다.
			m_pTReturn = m_blTimefDRI.GetAt(nDRI_Prev);
			if( m_pTReturn) timePrev = *m_pTReturn;
			m_pTReturn = m_blTimefDRI.GetAt(nDRI_Next);
			if( m_pTReturn) timeNext = *m_pTReturn;

			nTimeSpan_Prev = abs(timeFind - timePrev);
			nTimeSpan_Next = abs(timeFind - timeNext);
			
			if(nTimeSpan_Prev < nTimeSpan_Next)	nDRI = nDRI_Prev;
			else								nDRI = nDRI_Next;
		}
	}
	else if(timeDRI_Start < timeFind)
	{
		do
		{
			m_pTReturn = m_blTimefDRI.GetNext();
			if( m_pTReturn) timeTemp = *m_pTReturn;
		} while(timeTemp < timeFind && m_blTimefDRI.GetIndex() < nCountOfDRI);

		if(timeTemp < timeFind && m_blTimefDRI.GetIndex() >= nCountOfDRI)
		{
			nDRI = nCountOfDRI - 1;

			// 우측여백 포함 계산 : xScaleManager - ojtaso (20070611)
			if( bIncludeRightMargin && !m_pIChartCtrl->IsRunningOneChart() && m_nRightMarginWidth)
			{
				int nDRI_End = -1;
				if( 0 < m_daItemWidth[ p_nColumn]) ceil( ( double)m_nRightMarginWidth / m_daItemWidth[ p_nColumn]);
				time_t timeEnd;
				GetTimeFromDRI(nCountOfDRI + nDRI_End - 1, &timeEnd);

				nTimeSpan = timeEnd - timeLast;
				nTimeSpan_Find = timeFind - timeLast;
				if( 0 < nDRI_End) dTimeSpan_Item = (double)nTimeSpan / (double)nDRI_End;

				if( 0 < dTimeSpan_Item) nDRI += ceil((double)nTimeSpan_Find / dTimeSpan_Item);
			}
		}
		else
		{
			// timeFind랑 같은거나 바로 큰거
			nDRI_Next = m_blTimefDRI.GetIndex() - 1;
			nDRI_Prev = nDRI_Next - 1;

			// 가장 가까운 DRI를 찾습니다.
			m_pTReturn = m_blTimefDRI.GetAt(nDRI_Prev);
			if( m_pTReturn) timePrev = *m_pTReturn;
			m_pTReturn = m_blTimefDRI.GetAt(nDRI_Next);
			if( m_pTReturn) timeNext = *m_pTReturn;

			nTimeSpan_Prev = abs(timeFind - timePrev);
			nTimeSpan_Next = abs(timeFind - timeNext);
			
			if(nTimeSpan_Prev < nTimeSpan_Next)	nDRI = nDRI_Prev;
			else								nDRI = nDRI_Next;
		}
	}
	else if(timeDRI_Start == timeFind)
	{
		do
		{
			m_pTReturn = m_blTimefDRI.GetPrev();
			if( m_pTReturn) timeTemp = *m_pTReturn;
		} while( timeTemp >= timeFind && m_blTimefDRI.GetIndex() >= 0);
		
		if(timeTemp >= timeFind && m_blTimefDRI.GetIndex() < 0)
		{
			nDRI = 0;
		}
		else
		{
			// timeFind보다 바로 작은거 + 1 은 같은 시간인데 가장 먼저 나온 시간을 의미 합니다.
			nDRI = m_blTimefDRI.GetIndex() + 1 + 1;
		}
	}

	PAREA parea = NULL;
	GetAreaFromDRI( p_nColumn, nDRI, &parea);
	if(pparea != NULL) *pparea = parea;

	if( !parea) return -1;
	return parea->center;
}


// 현재 선택된 RQ를 자동으로 호출 : xScaleManager - ojtaso (20070409)
BOOL CXScaleManagerImp::GetDTInAREAfRQnPt(int nCursor_x, int& nBlockColumn, int* pnDTI, PAREA parea)
{
	if(m_ppacketListManager)
		return GetDTInAREAfRQnPt( m_pIChartCtrl->GetCurrentRQ(), nCursor_x, nBlockColumn, pnDTI, parea);
	else
		return GetDTInAREAfRQnPt(_T("DEFAULT"), nCursor_x, nBlockColumn, pnDTI, parea);
}


// 추세선에서 Pixel값에 해당하는 Data Index와 영역 정보를 구하고자 할때 사용합니다.
BOOL CXScaleManagerImp::GetDTInAREAfRQnPt( CString strRQ, int nCursor_x, int &nBlockColumn, int* pnDTI, PAREA parea, BOOL p_bWithBoundCheck)
{
	*pnDTI = -1;

	// 비어있는 차트인지 : xScaleManager - ojtaso (20070907)
	if(!IsUseable() || strRQ.IsEmpty()) return FALSE;

	//	string stringRQ((LPSTR)(LPCSTR)strRQ);

	int nDTI_Org, nDRI;

	nDRI = GetDRInFromPt( nCursor_x, &nBlockColumn, NULL, FALSE, p_bWithBoundCheck);

	// (2008/8/14 - Seung-Won, Bae) Support Checking of DRI Bound
	if( nDRI < 0) return FALSE;
	
	RQ_INFO* pifRQ = NULL;
	int nRQ = GetRQIndexFromString( strRQ);
	if( nRQ < 0) return FALSE;
	pifRQ = m_pifaRQ[ nRQ];

	CDataBlockList<int>* pblDTIfDRI = m_pblaDTIfDRI[ nRQ];
	if( !pblDTIfDRI) return FALSE;

	int nCountOfDRI_RQ = pblDTIfDRI->GetItemCount();
	if(nCountOfDRI_RQ <= nDRI) nDRI = nCountOfDRI_RQ - 1;

	m_pIReturn = pblDTIfDRI->GetAt(nDRI);
	if( m_pIReturn) *pnDTI = nDTI_Org = *m_pIReturn;

	TRACE("\nx:%d, DRI:%d, DTI:%d", nCursor_x, nDRI, *pnDTI);

	if(parea != NULL)
	{
		*parea = m_areaNONE;

		if(*pnDTI >= 0)
		{
			CDataBlockList< AREA> *pblAreafDTI = NULL;
			if( 0 <= nRQ && m_pblaAreafDTI[ nBlockColumn] && m_nCountOfRQ_Area == m_nCountOfRQ)
				pblAreafDTI = m_pblaAreafDTI[ nBlockColumn][ nRQ];

			int nDTI_View = *pnDTI - pifRQ->nDTI_Start_Data;

			if( pblAreafDTI) if( nDTI_View >= 0 && nDTI_View < pblAreafDTI->GetItemCount())
				*parea = *((PAREA)pblAreafDTI->GetAtUNK(nDTI_View));
			// (2009/1/10 - Seung-Won, Bae) why do you check the m_drawinfo ? 
			//	{
			//		if(m_drawinfo.pAreafDTI != NULL)
			//		{
			//			*parea = *((PAREA)pblAreafDTI->GetAtUNK(nDTI_View));
			//			TRACE(" AREA:(%d,%d,%d)", parea->left, parea->center, parea->right);
			//		}	
			//	}
		}
	}

	TRACE("\n");
	
	return (*pnDTI >= 0);
}


BOOL CXScaleManagerImp::IsAllOnePixel( int p_nColumn, const char *p_szRQ)
{
	if( m_nCountOfBlockColumn < 0) return FALSE;
	if( p_nColumn < 0 || m_nCountOfBlockColumn <= p_nColumn) return FALSE;

	int nRQ = GetRQIndexFromString( p_szRQ);
	if( nRQ < 0) return FALSE;
	if( m_minScaleInterval_eType != m_pifaRQ[ nRQ]->eScaleUnit
		|| m_minScaleInterval_nUnit != m_pifaRQ[ nRQ]->nScaleInterval_Unit) return FALSE;
	
	return m_daItemWidth[ p_nColumn] < 1 && !m_bWeightedVolume;
}


BOOL CXScaleManagerImp::IsInGraphXArea( int p_nColumn, int nX, BOOL bIncludeEqual)
{
	if( bIncludeEqual == FALSE)	return ( m_nScreenLeft_Cur[ p_nColumn] < nX && m_nScreenRight_Cur[ p_nColumn] > nX);
	else						return ( m_nScreenLeft_Cur[ p_nColumn] <= nX && m_nScreenRight_Cur[ p_nColumn] >= nX);
}


void CXScaleManagerImp::GetAreaFromDRI( int p_nColumn, int nDRI, PAREA* pparea)
{
	if( !pparea) return;
	*pparea = NULL;
	if( nDRI < 0) return;

	// on View
	if( m_nDRI_Start_Cur <= nDRI && nDRI <= m_nDRI_End_Cur + 1)		// (2009/3/2 - Seung-Won, Bae) +1 for right of Last Candle
	{
		*pparea = m_blAreafDRI[ p_nColumn].GetAt( nDRI - m_nDRI_Start_Cur);
		return;
	}

	*pparea = &m_areaTEMP;

	// left on view.
	int nDRI_Gap = 0;
	if( nDRI < m_nDRI_Start_Cur)
	{
		m_pAReturn = m_blAreafDRI[ p_nColumn].GetAt( 0);
		if( m_pAReturn) m_areaTEMP = *m_pAReturn;

		nDRI_Gap = m_nDRI_Start_Cur - nDRI;
		if( !m_bReverse) nDRI_Gap = -nDRI_Gap;
	}
	else
	{
		// right on view.
		m_pAReturn = m_blAreafDRI[ p_nColumn].GetAt( m_nDrawCount_Cur - 1);
		if( m_pAReturn) m_areaTEMP = *m_pAReturn;

		nDRI_Gap = nDRI - m_nDRI_End_Cur;
		if( m_bReverse) nDRI_Gap = -nDRI_Gap;
	}

	m_areaTEMP.Offset( ROUND( nDRI_Gap * m_daItemWidth[ p_nColumn]));
}


void CXScaleManagerImp::GetGraphXAreaLR( int p_nColumn, int& nLeft, int& nRight, BOOL bIncludeMargin)
{
	if(bIncludeMargin == FALSE)
	{
		nLeft = m_nScreenLeft_Cur[ p_nColumn];
		nRight = m_nScreenRight_Cur[ p_nColumn];
	}
	else
	{
		nLeft = m_nScreenLeftiM_Cur[ p_nColumn];
		nRight = m_nScreenRightiM_Cur[ p_nColumn];
	}
}


void CXScaleManagerImp::GetGraphXAreaSE( int p_nColumn, int& nStart, int& nEnd, BOOL bIncludeMargin)
{
	if(bIncludeMargin == FALSE)
	{
		nStart = m_naScreenStart_Cur[ p_nColumn];
		nEnd = m_naScreenEnd_Cur[ p_nColumn];
	}
	else
	{
		nStart = m_nScreenStartiM_Cur[ p_nColumn];
		nEnd = m_nScreenEndiM_Cur[ p_nColumn];
	}
}


// 화면 폭을 설정합니다.
void CXScaleManagerImp::SetScreenRegion( int p_nColumn, int nLeft, int nWidth, BOOL p_bReCalc)
{
	if( p_nColumn < 0 || m_nCountOfBlockColumn <= p_nColumn) return;

	// (2009/1/8 - Seung-Won, Bae) Do not calculate again with same environment.
	if( m_naScreenRegionLeft[ p_nColumn] == nLeft && m_naScreenRegionWidth[ p_nColumn] == nWidth && !p_bReCalc) return;

	// (2009/1/8 - Seung-Won, Bae) Manage the Screen Region Info
	m_naScreenRegionLeft[ p_nColumn]		= nLeft;
	m_naScreenRegionWidth[ p_nColumn]	= nWidth;

	// Left : 1, Right : 2
	nLeft += 1;	nWidth -=2;
	if( nWidth > MARGIN * 5)
	{
		nWidth -= MARGIN * 2 + 2;
		if(m_bReverse) nLeft += MARGIN * 2 + 2;
	}

	m_nScreenLeftiM_Cur[ p_nColumn] = nLeft;
	m_nScreenRightiM_Cur[ p_nColumn] = nLeft + nWidth;

	m_nScreenLeft_Cur[ p_nColumn] = nLeft;
	m_nScreenWidth_Cur[ p_nColumn] = nWidth;

	if( !m_pIChartCtrl->IsRunningOneChart())
	{
		if( m_bReverse) m_nScreenLeft_Cur[ p_nColumn] += m_nRightMarginWidth;
		m_nScreenWidth_Cur[ p_nColumn] -= m_nRightMarginWidth;
	}

	CRect rctClient = m_pIChartCtrl->GetAllBlockRegion();
	if( m_nItemCount_Total == 0)
	{
		m_rctClient = rctClient;
		return;
	}

	// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
	BOOL bItemWidthFix = ( m_rstype == RSAT_ITEM_WIDTH_FIX && !m_pIChartCtrl->IsWholeView());
	if( bItemWidthFix) bItemWidthFix = ( !m_rctClient.IsRectEmpty() && m_rctClient != rctClient);
	m_rctClient = rctClient;
	if( bItemWidthFix && 0 < m_dLastItemWidth)
	{
		m_bOnResizing = TRUE;
		double dDrawCount = m_nScreenWidth_Cur[ 0] / m_dLastItemWidth;
		m_pIChartCtrl->ZoomTo( dDrawCount, FALSE);
		if( m_nMinDataCountOnOnePage == m_nDrawCount_Cur || m_nItemCount_Total == m_nDrawCount_Cur)
		{
			// (2009/1/11 - Seung-Won, Bae) Use Calc_ItemWidth();
			Calc_ItemWidth( p_nColumn, m_nDrawCount_Cur);

			// 3. Update ScrollBar and Zoom SliderBar
			// (2009/1/14 - Seung-Won, Bae) Do not call on No-Recalc for avoding the deadlock.
			m_pIChartCtrl->UpdateZoomAndScrollInfo( m_nItemCount_Total, m_nDRI_Start_Cur, m_nDrawCount_Cur);
		}
		m_bOnResizing = FALSE;
	}
	else
	{
		int nZoomValue = m_pIChartCtrl->GetOnePageDataCount();
		if( m_pIChartCtrl->IsWholeView())					nZoomValue = m_nDrawCount_Cur;
		else if( nZoomValue < m_nMinDataCountOnOnePage)	nZoomValue = m_nMinDataCountOnOnePage;
		else if( m_nItemCount_Total < nZoomValue)		nZoomValue = m_nItemCount_Total;

		// (2009/1/11 - Seung-Won, Bae) Use Calc_ItemWidth();
		Calc_ItemWidth( p_nColumn, nZoomValue);

		// 3. Update ScrollBar and Zoom SliderBar
		// (2009/1/14 - Seung-Won, Bae) Do not call on No-Recalc for avoding the deadlock.
		m_pIChartCtrl->UpdateZoomAndScrollInfo( m_nItemCount_Total, m_nDRI_Start_Cur, m_nDrawCount_Cur);
	}
}


// (2009/1/10 - Seung-Won, Bae) Support Area array of DTI for drawing.
CDataBlockList< CXScaleManager::AREA> *CXScaleManagerImp::GetDTIArea( int p_nColumn, const char *p_szRQ, int &p_nZeroDTI)
{
	int nRQ = GetRQIndexFromString( p_szRQ);
	if( nRQ < 0) return NULL;
	p_nZeroDTI = m_pifaRQ[ nRQ]->nDTI_Start_Data;
	if( !m_pblaAreafDTI[ p_nColumn]) return NULL;
	return m_pblaAreafDTI[ p_nColumn][ nRQ];
}

// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
void CXScaleManagerImp::SetFlagForCalcDRIandScrollByReal( BOOL p_bWithTrucated)
{
	m_bScrollByReal = TRUE;
	if( p_bWithTrucated) m_bTruncatedByReal = TRUE;
}

// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
void CXScaleManagerImp::OnUserZoomChanged( void)
{
	if( m_bOnResizing) return;
	m_bOnUserZoomChanged = TRUE;
}

// (2009/5/28 - Seung-Won, Bae) Check! Is Data Empty?
BOOL CXScaleManagerImp::IsEmptyData( void)
{
	for( int i = 0; i < m_nCountOfRQ; i++)
		if( 0 <= m_pifaRQ[ i]->nLastRealDTI) return FALSE;
	return TRUE;
}

// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
void CXScaleManagerImp::AddVerticalBoxInfo(LPCTSTR lpszRQ, double dHorzValue, COLORREF clrColor, BOOL bFill)
{
	MAP_RQ_VERTBOX::iterator it = m_mifVertBox.find(lpszRQ);
	if(it != m_mifVertBox.end())
	{
		it->second->dHorzValue = dHorzValue;
		it->second->clrColor = clrColor;
		it->second->bFill = bFill;
	}
	else
	{
		VERTBOX_INFO* pMapVerBox = new VERTBOX_INFO;

		pMapVerBox->dHorzValue = dHorzValue;
		pMapVerBox->clrColor = clrColor;
		pMapVerBox->bFill = bFill;

		m_mifVertBox.insert(PAIR_RQ_VERTBOX(lpszRQ, pMapVerBox));
	}
}

// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
void CXScaleManagerImp::GetVerticalBoxInfo(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, CRect& rcBox, COLORREF& clrColor, BOOL& bFill)
{
	MAP_RQ_VERTBOX::iterator itVertBox = m_mifVertBox.find((LPSTR)lpszRQ);
	if(itVertBox != m_mifVertBox.end())
	{
		CPacketList* pPacketList = m_ppacketListManager->GetPacketList(lpszRQ);
		CPacket* pPacket = pPacketList->GetBaseData(lpszPacketName);
		if(!pPacket)
			return;

		CList<double, double>* pDataList = pPacket->GetnumericDataList();
		if( !pDataList) return;

		POSITION pos = pDataList->GetHeadPosition();
		int nDTI = -1;
		while(pos)
		{
			nDTI++;
			if(pDataList->GetNext(pos) == itVertBox->second->dHorzValue)
				break;
		}

		if(nDTI < 0)
			return;

		AREA area;
		if(!GetAREAfRQnDTI(lpszRQ, nDTI, 0, &area))
			return;
		
		rcBox.left = area.left;
		rcBox.right = area.right;
		
		clrColor = itVertBox->second->clrColor;
		bFill = itVertBox->second->bFill;
	}
}

int CXScaleManagerImp::GetTimeDiffWithCurrentRQ( const CString p_strRQ)
{
	const char *szCurrentRQ = m_pIChartCtrl->GetCurrentRQ();
	if( !szCurrentRQ) return 0;
	if( p_strRQ == szCurrentRQ) return 0;

	CPacketList *pPacketListMain = m_ppacketListManager->GetPacketList( szCurrentRQ);
	CPacketList *pPacketList = m_ppacketListManager->GetPacketList( p_strRQ);
	if( !pPacketListMain || !pPacketList) return 0;

	return pPacketList->GetBaseTimeDifference() - pPacketListMain->GetBaseTimeDifference();
}


int	CXScaleManagerImp::GetScaleDRIandTimeAllData( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
	const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType,	const char *p_szRQ, int &p_nSkipFactor)
{
	// 1. Get X Scale Grid Object.
	CXScaleGridTime *pXScaleGridTime = NULL;
	int nTimeDiff = GetTimeDiffWithCurrentRQ( p_szRQ);
	if( !m_mapXGridTime.Lookup( nTimeDiff, pXScaleGridTime)) return -1;

	p_pblScaleDRI = &m_blScaleDRI2[ p_nColumn];			// DRI value for scale of YYYY/MM/DD/HH/mm/SS
	p_pblScaleTime = &m_blScaleTime2[ p_nColumn];		// Time value for scale of YYYY/MM/DD/HH/mm/SS


	int nRQ = GetRQIndexFromString( p_szRQ);
	if( nRQ < 0) return -1;

	m_blScaleDRI2[ p_nColumn].Clear();		// DRI value for scale of YYYY/MM/DD/HH/mm/SS
	m_blScaleTime2[ p_nColumn].Clear();		// Time value for scale of YYYY/MM/DD/HH/mm/SS

	// (2009/5/28 - Seung-Won, Bae) Do not draw scale on no data.
	if( IsEmptyData()) return -1;

	p_nSkipFactor = 1;
	pXScaleGridTime->MakeScaleData( 2, m_blScaleTime2[ p_nColumn], m_blScaleDRI2[ p_nColumn], p_nSkipFactor, m_eChartMode);
	
	m_nSkipFactor = p_nSkipFactor;
	m_eScaleDrawType = p_eScaleDrawType;
	m_strScaleRQ = p_szRQ;

	return m_naScaleType[ p_nColumn];
}
