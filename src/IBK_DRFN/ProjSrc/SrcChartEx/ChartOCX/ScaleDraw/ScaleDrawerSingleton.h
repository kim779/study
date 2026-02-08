// ScaleDrawerSingleton.h: interface for the CScaleDrawerSingleton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEDRAWERSINGLETON_H__97C5F0A1_8E46_4950_A2D8_25EBB6C5E258__INCLUDED_)
#define AFX_SCALEDRAWERSINGLETON_H__97C5F0A1_8E46_4950_A2D8_25EBB6C5E258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TextScaleDrawer.h"
#include "VolumeScaleDrawer.h"
#include "PriceScaleDrawer.h"
#include "SpecialScaleDrawer.h"
#include "IndicateScaleDrawer.h"
#include "PacketDataScaleDrawer.h"
#include "LogScaleDrawer.h"
#include "DataHorzScaleDrawer.h"
#include "FullTimeScaleDrawer.h"		// for CFullTimeScaleDrawer

class CPacket;

// data 에 관한 scale
class CDataHorzScaleDrawerSingleton
{
public:
	CDataHorzScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
public:
	CHorzScaleDrawer* GetDataHorzScaleDrawer(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType);

private:
	CPriceZoneHorzScaleDrawer m_priceZoneScaleDrawer;	// "행사가"
	CEtcDataHorzScaleDrawer m_etcHorzScaleDrawer;		// "volume" & packet data
	CLeftRightScaleDrawer m_leftRightScaleDrawer;		// 좌우식
	CMarketProfileHorzScaleDrawer m_marketProfileHorzScaleDrawer; // "MarketProfile"
};

// 가로 scale
class CHorzScaleDrawerSingleton  
{
public:
	CHorzScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
public:
	CHorzScaleDrawer* GetHorzScaleDrawer(CPacket* pPacket, const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType);
	CHorzScaleDrawer* GetHorzScaleDrawer(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType);
	
private:
	CTextHorzScaleDrawer m_textHorzScaleDrawer; // "text"
	CDataHorzScaleDrawerSingleton m_dataSingleton;

protected:
	CFullTimeScaleDrawer	m_fullTimeScaleDrawer;
};


///////////////////////////////////////////////////////////////////////////////////////
// 세로 scale
class CVertScaleDrawerSingleton
{
public:
	CVertScaleDrawerSingleton( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:
	CVertScaleDrawer* GetVertScaleDrawer(const CScaleBaseData::VERTSCALEDRAWERTYPE drawerType);

private:
	CPriceScaleDrawer m_priceScaleDrawer;			// 가격 스케일 
	CVolumeVertScaleDrawer m_volumeVertScaleDrawer;	// 거래량 스케일
	CSaleScaleDrawer m_saleScaleDrawer;				// 대기매물차트 관련 스케일
	CMarketProfileVertScaleDrawer m_marketProfileVertDrawer;// MarketProfile 관련 스케일
	CIndicateScaleDrawer m_indicateScaleDrawer;		// 지표 데이타 (오른쪽 : 데이타 / 왼쪽 : 기준값)
	CPacketDataScaleDrawer m_packetDataScaleDrawer;	// 일반 데이타
	CTextVertScaleDrawer m_textVertScaleDrawer;		// text
	CLogScaleDrawer m_logScaleDrawer;				// log scale (가격차트, 보조차트)
	CPercentFullScaleDrawer m_percentScaleDrawer;	// 백분율(0%~100% 고정)
	CTopButtomPlusScaleDrawer m_topButtomPlusScaleDrawer;	// +상하식
};

#endif // !defined(AFX_SCALEDRAWERSINGLETON_H__97C5F0A1_8E46_4950_A2D8_25EBB6C5E258__INCLUDED_)
