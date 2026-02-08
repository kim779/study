// SelectSubGraphData.cpp: implementation of the CSelectSubGraphData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectSubGraphData.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CSelectSubGraphData

CSelectSubGraphData::CSelectSubGraphData():
	m_eDrawingOrderType(CDrawingOption::LEFT),
	m_bIsVertScaleLogType(false),
	m_bIsVertScaleInvertType(false),
	m_TYPESubGraphType(CGraphBaseData::GTNONE),
	m_pSubGraphPacketList(NULL),
	m_pIndicatorInfo(NULL)
{
}

void CSelectSubGraphData::SetDrawingOrderType(const CDrawingOption::DRAWINGORDER eDrawingOrderType)
{
	m_eDrawingOrderType = eDrawingOrderType;
}

void CSelectSubGraphData::SetGraphFunctionName(const CString& functionName)
{
	m_strFunctionName = functionName;
}

void CSelectSubGraphData::SetVertScaleLogType(const bool bIsVertScaleLogType)
{
	m_bIsVertScaleLogType = bIsVertScaleLogType;
}

void CSelectSubGraphData::SetVertScaleInvertType(const bool bIsVertScaleInvertType)
{
	m_bIsVertScaleInvertType = bIsVertScaleInvertType;
}

void CSelectSubGraphData::SetGraphRegions(const CGraphRegions& graphRegions)
{
	m_graphRegions = graphRegions;
}

void CSelectSubGraphData::SetSubGraphType(const CGraphBaseData::GRAPHTYPE subGraphType)
{
	m_TYPESubGraphType = subGraphType;
}

void CSelectSubGraphData::SetSubGraphStyle(const CGraphStyle& subGraphStyle)
{
	m_nSubGraphStyle = subGraphStyle;
}

void CSelectSubGraphData::SetSubGraphDrawStyle(const CGraphDrawStyle& subGraphDrawStyle)
{
	m_nSubGraphDrawStyle = subGraphDrawStyle;
}

void CSelectSubGraphData::SetDisplayDataHorzMinMax(const CDisplayDataMinMax& displayDataMinMax)
{
	m_displayDataHorzMinMax = displayDataMinMax;
}

void CSelectSubGraphData::SetDisplayDataVertMinMax(const CDisplayDataMinMax& displayDataMinMax)
{
	m_displayDataVertMinMax = displayDataMinMax;
}

void CSelectSubGraphData::SetDisplayAttributes(const CDisplayAttributes& displayAttributes)
{
	m_displayAttributes = displayAttributes;
}

void CSelectSubGraphData::SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList)
{
	m_pSubGraphPacketList = pSubGraphPacketList;
}

void CSelectSubGraphData::SetIndicatorInfo(CIndicatorInfo* pIndicatorInfo)
{
	m_pIndicatorInfo = pIndicatorInfo;
}

// ----------------------------------------------------------------------------
CDrawingOption::DRAWINGORDER CSelectSubGraphData::GetDrawingOrderType() const
{
	return m_eDrawingOrderType;
}

CString CSelectSubGraphData::GetGraphFunctionName() const
{
	return m_strFunctionName;
}

bool CSelectSubGraphData::GetVertScaleLogType() const
{
	return m_bIsVertScaleLogType;
}

bool CSelectSubGraphData::GetVertScaleInvertType() const
{
	return m_bIsVertScaleInvertType;
}

CGraphRegions CSelectSubGraphData::GetGraphRegions() const
{
	return m_graphRegions;
}

CGraphBaseData::GRAPHTYPE CSelectSubGraphData::GetSubGraphType() const
{
	return m_TYPESubGraphType;
}

CGraphStyle CSelectSubGraphData::GetSubGraphStyle() const
{
	return m_nSubGraphStyle;
}

CGraphDrawStyle CSelectSubGraphData::GetSubGraphDrawStyle() const
{
	return m_nSubGraphDrawStyle;
}

CDisplayDataMinMax CSelectSubGraphData::GetDisplayDataHorzMinMax() const
{
	return m_displayDataHorzMinMax;
}

CDisplayDataMinMax CSelectSubGraphData::GetDisplayDataVertMinMax() const
{
	return m_displayDataVertMinMax;
}

CDisplayAttributes CSelectSubGraphData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CSubGraphPacketList* CSelectSubGraphData::GetSubGraphPacketList() const
{
	return m_pSubGraphPacketList;
}

CIndicatorInfo* CSelectSubGraphData::GetIndicatorInfo() const
{
	return m_pIndicatorInfo;
}

