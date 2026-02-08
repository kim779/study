// ScaleDrawerSingleton.cpp: implementation of the CScaleDrawerSingleton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleDrawerSingleton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "BlockBaseData.h"
#include "PacketBase.h"											// PacketBase.dll
#include "PacketList.h"											// PacketBase.dll


////////////////////////////////////////////////////////////////////////////////////
// class CDataHorzScaleDrawerSingleton

CDataHorzScaleDrawerSingleton::CDataHorzScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: m_priceZoneScaleDrawer( p_hOcxWnd, p_eChartMode), m_etcHorzScaleDrawer( p_hOcxWnd, p_eChartMode),
		m_leftRightScaleDrawer( p_hOcxWnd, p_eChartMode), m_marketProfileHorzScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

CHorzScaleDrawer* CDataHorzScaleDrawerSingleton::GetDataHorzScaleDrawer(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType)
{
	switch(scaleDrawerType){
	case CScaleBaseData::HORZ_LEFTRIGHT: // "좌우식"
		return &m_leftRightScaleDrawer;
	case CScaleBaseData::HORZ_PRICEZONE: // "행사가"
		return &m_priceZoneScaleDrawer;
	case CScaleBaseData::HORZ_DATA:		 // "기타 데이타" (거래량 포함)
		return &m_etcHorzScaleDrawer;
	case CScaleBaseData::HORZ_MARKETPROFILE:	// "MarketProfile"
		return &m_marketProfileHorzScaleDrawer;
	};

	return NULL;
}


////////////////////////////////////////////////////////////////////////////////////
// class CScaleDrawerSingleton

CHorzScaleDrawerSingleton::CHorzScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: m_textHorzScaleDrawer( p_hOcxWnd, p_eChartMode), m_dataSingleton( p_hOcxWnd, p_eChartMode),
		m_fullTimeScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// 해당 scaleDrawer를 찾는다.
CHorzScaleDrawer* CHorzScaleDrawerSingleton::GetHorzScaleDrawer(CPacket* pPacket, const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType)
{
	if(pPacket == NULL)
		return NULL;

	// "자료일자"
	if( CScaleBaseData::HORZ_DAILY <= scaleDrawerType
		&& scaleDrawerType <= CScaleBaseData::HORZ_TIME_TWO
		|| scaleDrawerType == CScaleBaseData::HORZ_TIME_SECOND)
		return &m_fullTimeScaleDrawer;
	// "text"
	else if( pPacket->GetType() == _MTEXT( C6_CHARACTER))
		return &m_textHorzScaleDrawer;

	return m_dataSingleton.GetDataHorzScaleDrawer(scaleDrawerType);
}

CHorzScaleDrawer* CHorzScaleDrawerSingleton::GetHorzScaleDrawer(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType)
{
	if(scaleDrawerType >= CScaleBaseData::HORZ_DAILY && scaleDrawerType <= CScaleBaseData::HORZ_MONTHLY)
		return &m_fullTimeScaleDrawer;
	else if(scaleDrawerType == CScaleBaseData::HORZ_TICK || 
			scaleDrawerType == CScaleBaseData::HORZ_TIME_ONE ||
			scaleDrawerType == CScaleBaseData::HORZ_TIME_TWO ||
			scaleDrawerType == CScaleBaseData::HORZ_TIME_SECOND)
		return &m_fullTimeScaleDrawer;
	else if(scaleDrawerType == CScaleBaseData::HORZ_TEXT)
		return &m_textHorzScaleDrawer;

	return m_dataSingleton.GetDataHorzScaleDrawer(scaleDrawerType);
}

////////////////////////////////////////////////////////////////////////////////////
// class CVertScaleDrawerSingleton

CVertScaleDrawerSingleton::CVertScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: m_priceScaleDrawer( p_hOcxWnd, p_eChartMode), m_volumeVertScaleDrawer( p_hOcxWnd, p_eChartMode),
		m_saleScaleDrawer( p_hOcxWnd, p_eChartMode), m_marketProfileVertDrawer( p_hOcxWnd, p_eChartMode), 
		m_indicateScaleDrawer( p_hOcxWnd, p_eChartMode), m_packetDataScaleDrawer( p_hOcxWnd, p_eChartMode),
		m_textVertScaleDrawer( p_hOcxWnd, p_eChartMode), m_logScaleDrawer( p_hOcxWnd, p_eChartMode),
		m_percentScaleDrawer( p_hOcxWnd, p_eChartMode), m_topButtomPlusScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

CVertScaleDrawer* CVertScaleDrawerSingleton::GetVertScaleDrawer(const CScaleBaseData::VERTSCALEDRAWERTYPE drawerType)
{
	switch(drawerType){
		case CScaleBaseData::VERT_PRICE:	// 가격타입
			return &m_priceScaleDrawer;
		case CScaleBaseData::VERT_VOLUME:	// 거래량타입
			return &m_volumeVertScaleDrawer;
		case CScaleBaseData::VERT_SALE:		// 대기매물
			return &m_saleScaleDrawer;
		case CScaleBaseData::VERT_MARKETPROFILE: // MarketProfile
			return &m_marketProfileVertDrawer;
		case CScaleBaseData::VERT_BASELINE: // 기준선타입
			return &m_indicateScaleDrawer;
		case CScaleBaseData::VERT_TEXT:		// text
			return &m_textVertScaleDrawer;		
		case CScaleBaseData::VERT_PACKETDATA:// 순수 packet Data
			return &m_packetDataScaleDrawer;
		case CScaleBaseData::VERT_LOG:		// log
			return &m_logScaleDrawer;
		case CScaleBaseData::VERT_PERCENT:	// 백분율 (0% ~ 100%)
			return &m_percentScaleDrawer;
		case CScaleBaseData::VERT_TOPBUTTOM_PLUS:	// +상하식
			return &m_topButtomPlusScaleDrawer;
	}
	return NULL;
}
