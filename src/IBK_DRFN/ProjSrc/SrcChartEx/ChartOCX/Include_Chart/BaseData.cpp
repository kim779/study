// BaseData.cpp: implementation of the BaseData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseData.h"

#include "./Dll_Load/IndicatorInfo.h"
#include "./Dll_Load/PacketList.h"				// 호가단위, 기준가 사용 - ojtaso (20070917)
#include "Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CDisplayAttributes

CDisplayAttributes& CDisplayAttributes::operator=(const CDisplayAttributes& data)
{
	if(this == &data)
		return *this;

	m_nDataStartInBlock = data.GetDataStartInBlock();
	m_nDataEndInBlock = data.GetDataEndInBlock();
	return *this;
}

bool CDisplayAttributes::operator==(const CDisplayAttributes& data)
{
	return (m_nDataStartInBlock == data.GetDataStartInBlock() && 
		m_nDataEndInBlock ==  data.GetDataEndInBlock());
}

// ----------------------------------------------------------------------------
CDisplayAttributes::CDisplayAttributes():
	m_nDataStartInBlock(-1),
	m_nDataEndInBlock(-1)
{
}

CDisplayAttributes::CDisplayAttributes(const int start, const int end):
	m_nDataStartInBlock(start),
	m_nDataEndInBlock(end)
{
}

void CDisplayAttributes::SetDataStartInBlock(const int start)
{
	m_nDataStartInBlock = start;
}

void CDisplayAttributes::SetDataEndInBlock(const int end)
{
	m_nDataEndInBlock = end;
}

// ----------------------------------------------------------------------------
int CDisplayAttributes::GetDataStartInBlock() const
{
	return m_nDataStartInBlock;
}

int CDisplayAttributes::GetDataEndInBlock() const
{
	return m_nDataEndInBlock;
}

int CDisplayAttributes::GetDataCountInBlock() const
{
	return (m_nDataEndInBlock - m_nDataStartInBlock +1);
}


///////////////////////////////////////////////////////////////////////////////
// class CScaleRegionAttributes

CScaleRegionAttributes::CScaleRegionAttributes():
	m_nMinHorzScaleRegionHeight(0),
	m_nMinVertScaleRegionWidth(0),
	m_nMaxVertScaleRegionWidth(0)
{
}

// 가로 scale의 최소 높이
void CScaleRegionAttributes::SetMinHorzScaleRegionHeight(const int minHorzScaleRegionHeight)
{
	m_nMinHorzScaleRegionHeight = minHorzScaleRegionHeight;
}

// 세로 scale의 최소 폭
void CScaleRegionAttributes::SetMinVertScaleRegionWidth(const int minVertScaleRegionWidth)
{
	m_nMinVertScaleRegionWidth = minVertScaleRegionWidth;
}

// 세로 scale의 최대 폭
void CScaleRegionAttributes::SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth)
{
	m_nMaxVertScaleRegionWidth = maxVertScaleRegionWidth;
}

// ----------------------------------------------------------------------------
int CScaleRegionAttributes::GetMinHorzScaleRegionHeight() const
{
	return m_nMinHorzScaleRegionHeight;
}

int CScaleRegionAttributes::GetMinVertScaleRegionWidth() const
{
	return m_nMinVertScaleRegionWidth;
}

int CScaleRegionAttributes::GetMaxVertScaleRegionWidth() const
{
	return m_nMaxVertScaleRegionWidth;
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphRegionsBase -> graph 영역 저장
CGraphRegionsBase::CGraphRegionsBase() :
	m_rctFrameRegion( 0, 0, 0, 0),
	m_rctNoFrameRegion( 0, 0, 0, 0),
	m_rctNoFrameAndMargin( 0, 0, 0, 0),
	m_nFrameThickness(0)
{
}

void CGraphRegionsBase::SetFrameRegion( const CRect& region, const int p_nFrameThickness, BOOL p_bReverse)
{
	m_nFrameThickness		= p_nFrameThickness;
	m_rctFrameRegion		= region;

	m_rctNoFrameRegion		= m_rctFrameRegion;
	m_rctNoFrameRegion.DeflateRect( m_nFrameThickness, m_nFrameThickness);
	
	m_rctNoFrameAndMargin	= m_rctNoFrameRegion;
	m_rctNoFrameAndMargin.DeflateRect( 1, 1, 2, 2);
	
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	if( m_rctNoFrameAndMargin.Width() > MARGIN * 5)
	{
		if( p_bReverse)		m_rctNoFrameAndMargin.left += MARGIN * 2 + 2;
		else				m_rctNoFrameAndMargin.right -= MARGIN * 2 + 2;
	}
}

///////////////////////////////////////////////////////////////////////////////
// class CGraphRegions  -> graph, scale 영역 저장

CGraphRegions::CGraphRegions() :
	m_nFontSize(0),
	m_nLineCount(1),	// 타이틀 줄 수 : 복수종목 - ojtaso (20070227)
	m_bIsTitle(false),
	m_bIsText(false),
	m_bIsVertScale(true),
	m_bBlockMoveDelete( FALSE)
{
}

bool CGraphRegions::IsTitle() const
{
	return m_bIsTitle;
}

bool CGraphRegions::IsText() const
{
	return m_bIsText;
}

void CGraphRegions::SetText(const bool bIsText)
{
	m_bIsText = bIsText;
}

void CGraphRegions::SetTitle(const bool bIsTitle)
{
	m_bIsTitle = bIsTitle;
}

void CGraphRegions::SetFontSize(const int nSize)
{
	m_nFontSize = nSize;
}

// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
void CGraphRegions::SetTitleLine(const int nLine)
{
	m_nLineCount = nLine;
}

// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
void CGraphRegions::OffsetTitleLine(const int nLine)
{
	m_nLineCount += nLine;
}

void CGraphRegions::SetVertScale(const bool bIsVertScale)
{
	m_bIsVertScale = bIsVertScale;
}

void CGraphRegions::OffsetFontSize(const int nSize)
{
	m_nFontSize += nSize;
}

CRect CGraphRegions::GetTitleRegion() const
{
	CRect titleRegion = GetNoFrameRegionAndMargin();
	titleRegion.bottom = titleRegion.top;
	if( m_bIsTitle) titleRegion.bottom += m_nFontSize;
	return titleRegion;
}

// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
int CGraphRegions::GetTitleLineCount() const
{
	return m_nLineCount;
}

CRect CGraphRegions::GetDrawingRegion(const CGraphBaseData::GRAPHTYPE eGraphType, const CGraphStyle& graphStyle) const
{
	CRect realRegion = GetNoFrameRegionAndMargin();

	if( m_bIsTitle && eGraphType != CGraphBaseData::Pie_Type) realRegion.top += m_nFontSize + 1;
	else if( m_bBlockMoveDelete)	realRegion.top += 11;

	switch( eGraphType)
	{
		case CGraphBaseData::Bar_Type:	switch( graphStyle.GetGraphBarStyle())
										{
											case CGraphBaseData::TextHorizontalBar:	realRegion.left--;
																					break;
											case CGraphBaseData::VerticalBar:	realRegion.bottom += 2;
																				break;
										}
										break;
		case CGraphBaseData::Pie_Type:	realRegion.right = GetNoFrameRegion().right - 2;
										break;
	}

	if( m_bIsText) realRegion.DeflateRect( 0, m_nFontSize - 1);

	return realRegion;
}

///////////////////////////////////////////////////////////////////////////////
// class CDrawingRegion  -> graph, scale 영역 저장

void CDrawingRegion::SetGraphRegions(const CGraphRegions& graphRegions)
{
	m_graphRegions = graphRegions;
}

void CDrawingRegion::SetHorzScaleRegion(const CRect& topRegion, const CRect& bottomRegion)
{
	m_horzScaleRegion.SetTopScaleRegion(topRegion);
	m_horzScaleRegion.SetBottomScaleRegion(bottomRegion);
}

void CDrawingRegion::SetVertScaleRegion(const CRect& leftRegion, const CRect& rightRegion)
{
	m_vertScaleRegion.SetLeftScaleRegion(leftRegion);
	m_vertScaleRegion.SetRightScaleRegion(rightRegion);
}

void CDrawingRegion::OffsetDrawingRegion(const int nFontSize)
{
	m_graphRegions.OffsetFontSize(nFontSize);
	m_vertScaleRegion.OffsetRegion(nFontSize, 0);
}

// ----------------------------------------------------------------------------
CGraphRegions& CDrawingRegion::GetGraphRegions()
{
	return m_graphRegions;
}

CGraphRegions CDrawingRegion::GetGraphRegions() const
{
	return m_graphRegions;
}

CRect CDrawingRegion::GetGraphFrameRegion( void) const
{
	return m_graphRegions.GetFrameRegion();
}

CHorzScaleRegion CDrawingRegion::GetHorzScaleRegion() const
{
	return m_horzScaleRegion;
}

CVertScaleRegion CDrawingRegion::GetVertScaleRegion() const
{
	return m_vertScaleRegion;
}


///////////////////////////////////////////////////////////////////////////////
// class CDrawingGraphData

// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 관리한다.
// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
CDrawingGraphData::CDrawingGraphData(CIndicatorList* pIndicatorList, 
		const CDrawingOption::DRAWINGORDER eDrawingOrderType, 
		const CGraphRegions& graphRegions,
		const CRect& scaleLeftTextRegion,		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
		const CRect& scaleRightTextRegion,		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
		const CDisplayDataMinMax& displayDataHorzMinMax, const CString& horzScalePacketName, 
		const CEtcGraphColor& etcGraphColor, const CDisplayAttributes& displayAttributes, 
		const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType, 
		const bool bIsConditionTitleDraw, const bool bIsDataTitleDraw, CPacketList *p_pPacketList,
		BOOL p_bWithBaseText,
		bool bDrawBaseLine,	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		HWND p_hOcxWnd,
		int	p_nColumn,
		CChartInfo::CHART_MODE p_eChartMode,	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
		CMainBlock *pMainBlock):	
	m_pIndicatorList(pIndicatorList), 
	m_eDrawingOrderType(eDrawingOrderType),
	m_graphRegions(graphRegions),
	m_scaleLeftTextRegion(scaleLeftTextRegion),		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	m_scaleRightTextRegion(scaleRightTextRegion),	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	m_displayDataHorzMinMax(displayDataHorzMinMax),
	m_strHorzScalePacketName(horzScalePacketName),
	m_etcGraphColor(etcGraphColor),
	m_displayAttributes(displayAttributes),
	m_bIsVertScaleLogType(bIsVertScaleLogType),
	m_bIsVertScaleInvertType(bIsVertScaleInvertType),
	m_bIsConditionTitleDraw(bIsConditionTitleDraw),
	m_bIsDataTitleDraw(bIsDataTitleDraw),
	m_pPriceChartColor(NULL),
	m_pIndiInfo(NULL),
	m_pSubGraphPacketList(NULL),
	m_pPacketList( p_pPacketList),
	m_eVertPos(CScaleBaseData::VERTSCALEPOSITION::VERT_RIGHT),	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	m_bTextDrawing(TRUE),
	m_bDrawBaseLine(bDrawBaseLine),	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	m_pMainBlock( pMainBlock)
{
	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
	m_eChartMode = p_eChartMode;

	// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
	m_bWithBaseText = p_bWithBaseText;

	// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
	m_hOcxWnd = p_hOcxWnd;

	// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
	m_nColumnIndex = p_nColumn;
}

void CDrawingGraphData::SetDisplayDataVertMinMax(const CDisplayDataMinMax& displayDataVertMinMax)
{
	m_displayDataVertMinMax = displayDataVertMinMax;
}

void CDrawingGraphData::SetGraphName(const CString& GraphName)
{
	m_strGraphName = GraphName;
}

void CDrawingGraphData::SetPacketNames(const CString& packetNames)
{
	m_strPacketNames = packetNames;
}

void CDrawingGraphData::SetTitleDraw(const bool bIsConditionTitleDraw, const bool bIsDataTitleDraw)
{
	m_bIsConditionTitleDraw = bIsConditionTitleDraw;
	m_bIsDataTitleDraw = bIsDataTitleDraw;
}

void CDrawingGraphData::SetConditionTitleDraw(const bool bIsConditionTitleDraw)
{
	m_bIsConditionTitleDraw = bIsConditionTitleDraw;
}

void CDrawingGraphData::SetDataTitleDraw(const bool bIsDataTitleDraw)
{
	m_bIsDataTitleDraw = bIsDataTitleDraw;
}

void CDrawingGraphData::SetGraphDataDrawingData(const CGraphDataDrawingData& graphDataDrawingData)
{
	m_drawingData = graphDataDrawingData;
}

void CDrawingGraphData::SetPriceChartColor(CGraphColor* pGraphColor)
{
	m_pPriceChartColor = pGraphColor;
}

void CDrawingGraphData::SetIndicatorInfo(CIndicatorInfo* pIndiInfo)
{
	m_pIndiInfo = pIndiInfo;
}

void CDrawingGraphData::SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList)
{
	m_pSubGraphPacketList = pSubGraphPacketList;
}

// ----------------------------------------------------------------------------
CDrawingOption::DRAWINGORDER CDrawingGraphData::GetDrawingOrderType() const
{
	return m_eDrawingOrderType;
}

bool CDrawingGraphData::DoesVertScaleLogType() const
{
	return m_bIsVertScaleLogType;
}

bool CDrawingGraphData::DoesVertScaleInvertType() const
{
	return m_bIsVertScaleInvertType;
}

// 지표 뒤집기 - ojtaso (20071023)
void CDrawingGraphData::SetVertScaleInvertType(const bool bIsVertScaleInvertType)
{
	m_bIsVertScaleInvertType = bIsVertScaleInvertType;
}

const CGraphRegions& CDrawingGraphData::GetGraphRegions() const
{
	return m_graphRegions;
}

CRect CDrawingGraphData::GetScaleTextRegion() const
{
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	if(m_eVertPos == CScaleBaseData::VERTSCALEPOSITION::VERT_RIGHT)
		return m_scaleRightTextRegion;
	else if(m_eVertPos == CScaleBaseData::VERTSCALEPOSITION::VERT_LEFT)
		return m_scaleLeftTextRegion;
	else
		return m_scaleRightTextRegion;
}

// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
CRect CDrawingGraphData::GetLeftScaleTextRegion() const
{
	return m_scaleLeftTextRegion;
}

// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
CRect CDrawingGraphData::GetRightScaleTextRegion() const
{
	return m_scaleRightTextRegion;
}

CDisplayDataMinMax CDrawingGraphData::GetDisplayDataHorzMinMax() const
{
	return m_displayDataHorzMinMax;
}

CDisplayDataMinMax CDrawingGraphData::GetDisplayDataVertMinMax() const
{
	return m_displayDataVertMinMax;
}

CString CDrawingGraphData::GetGraphName() const
{
	return m_strGraphName;
}

CString CDrawingGraphData::GetPacketName() const
{
	return m_strPacketNames;
}

CString CDrawingGraphData::GetHorzScalePacketName() const
{
	return m_strHorzScalePacketName;
}

bool CDrawingGraphData::DoesConditionTitleDraw() const
{
	return m_bIsConditionTitleDraw;
}

bool CDrawingGraphData::DoesDataTitleDraw() const
{
	return m_bIsDataTitleDraw;
}

CGraphDataDrawingData CDrawingGraphData::GetGraphDataDrawingData() const
{
	return m_drawingData;
}

CDisplayAttributes CDrawingGraphData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CEtcGraphColor CDrawingGraphData::GetEtcGraphColor() const
{
	return m_etcGraphColor;
}

CGraphColor* CDrawingGraphData::GetPriceChartColor() const
{
	return m_pPriceChartColor;
}

CIndicatorList* CDrawingGraphData::GetIndicatorList() const
{
	return m_pIndicatorList;
}

CIndicatorInfo* CDrawingGraphData::GetIndicatorInfo() const
{
	return m_pIndiInfo;
}

CSubGraphPacketList* CDrawingGraphData::GetSubGraphPacketList() const
{
	return m_pSubGraphPacketList;
}

CString CDrawingGraphData::GetIndicatorName() const
{
	if(m_pIndiInfo)
		return m_pIndiInfo->GetIndicatorName();
	return m_strGraphName;
}

// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
void CDrawingGraphData::SetVertScaleDisplayAttributes(CScaleBaseData::VERTSCALEPOSITION eVertPos, BOOL bTextDrawing)
{
	m_eVertPos = eVertPos;
	m_bTextDrawing = bTextDrawing;
}

///////////////////////////////////////////////////////////////////////////////
// class CDrawingHorzScaleData

// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
CDrawingHorzScaleData::CDrawingHorzScaleData(const CRect& drawingGraphRegion, const CGraphBaseData::GRAPHTYPE& eHeadGraphType, 
		const CGraphStyle& headGraphStyle, const CGraphRegionColor& graphRegionColor, 
		const CScaleColor& scaleColor, const CDisplayAttributes& displayAttributes, 
		const CDisplayDataMinMax& displayDataMinMax, CPacket* pPacket, BOOL bDrawFutureScale, int p_nColumn):
	m_drawingGraphRegion(drawingGraphRegion),
	m_eHeadGraphType(eHeadGraphType),
	m_headGraphStyle(headGraphStyle),
	m_TYPEScaleDrawer(CScaleBaseData::HORZ_NOTYPE),
	m_graphRegionColor(graphRegionColor),
	m_scaleColor(scaleColor),
	m_displayAttributes(displayAttributes),
	m_displayDataMinMax(displayDataMinMax),
	m_pPacket(pPacket),
	m_bDrawFutureScale(bDrawFutureScale)
{

	// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
	m_nColumnIndex = p_nColumn;
}

CRect CDrawingHorzScaleData::GetDrawingGraphRegion() const
{
	return m_drawingGraphRegion;
}

CGraphBaseData::GRAPHTYPE CDrawingHorzScaleData::GetHeadGraphType() const
{
	return m_eHeadGraphType;
}

CGraphStyle CDrawingHorzScaleData::GetHeadGraphStyle() const
{
	return m_headGraphStyle;
}

CScaleBaseData::HORZSCALEDRAWERTYPE CDrawingHorzScaleData::GetScaleDrawerType() const
{
	return m_TYPEScaleDrawer;
}

CScaleColor CDrawingHorzScaleData::GetScaleColor() const
{
	return m_scaleColor;
}

CGraphRegionColor CDrawingHorzScaleData::GetGraphRegionColor() const
{
	return m_graphRegionColor;
}

COLORREF CDrawingHorzScaleData::GetCompartLineColor() const
{
	return m_graphRegionColor.GetFrameColor();
}

CDisplayAttributes CDrawingHorzScaleData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CDisplayDataMinMax CDrawingHorzScaleData::GetDisplayDataMinMax() const
{
	return m_displayDataMinMax;
}

CPacket* CDrawingHorzScaleData::GetPacket() const
{
	return m_pPacket;
}

// ----------------------------------------------------------------------------
void CDrawingHorzScaleData::SetScaleDrawerType(const CScaleBaseData::HORZSCALEDRAWERTYPE& drawType)
{
	m_TYPEScaleDrawer = drawType;
}

// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
BOOL CDrawingHorzScaleData::GetDrawFutureScale() const
{
	return m_bDrawFutureScale;
}

// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
void CDrawingHorzScaleData::SetDrawFutureScale(const BOOL bDrawFutureScale)
{
	m_bDrawFutureScale = bDrawFutureScale;
}

///////////////////////////////////////////////////////////////////////////////
// class CDrawingVertScaleData

CDrawingVertScaleData::CDrawingVertScaleData(	const CRect& noFrameGraphRegion,
												const CRect& drawingGraphRegion,
												const CGraphRegionColor& graphRegionColor,
												const CScaleColor& scalrColor,
												const bool bIsVertScaleInvertType,
												const bool bIsVertScaleLogType,
												const CDisplayAttributes& displayAttributes,
												CPacketList* pPacketList) : 	// 호가단위, 기준가 사용 - ojtaso (20070917)
	m_noFrameGraphRegion(noFrameGraphRegion),
	m_drawingGraphRegion(drawingGraphRegion),
	m_TYPEScaleDrawer(CScaleBaseData::VERT_NOTYPE),
	m_graphRegionColor(graphRegionColor),
	m_scaleColor(scalrColor),
	m_bIsVertScaleInvertType(bIsVertScaleInvertType),
	m_displayAttributes(displayAttributes),
	m_pIndicatorInfo(NULL),
	m_pPacket(NULL),
	m_pPacketList(pPacketList)
{
	if(bIsVertScaleLogType)
		m_TYPEScaleDrawer = CScaleBaseData::VERT_LOG;
}

CRect CDrawingVertScaleData::GetNoFrameGraphRegion() const
{
	return m_noFrameGraphRegion;
}

CRect CDrawingVertScaleData::GetDrawingGraphRegion() const
{
	return m_drawingGraphRegion;
}

bool CDrawingVertScaleData::GetVertScaleInvertType() const
{
	return m_bIsVertScaleInvertType;
}

// 지표 뒤집기 - ojtaso (20071023)
void CDrawingVertScaleData::SetVertScaleInvertType(const bool bIsVertScaleInvertType)
{
	m_bIsVertScaleInvertType = bIsVertScaleInvertType;
}

bool CDrawingVertScaleData::GetVertScaleLogType() const
{
	return (m_TYPEScaleDrawer == CScaleBaseData::VERT_LOG ? true: false);
}

CString CDrawingVertScaleData::GetSubGraphNames() const
{
	return m_strSubGraphNames;
}

CString CDrawingVertScaleData::GetPacketNames() const
{
	return m_strPacketNames;
}

CString CDrawingVertScaleData::GetIndicatorName() const
{
	return m_strIndicatorName;
}

CScaleBaseData::VERTSCALEDRAWERTYPE CDrawingVertScaleData::GetScaleDrawerType() const
{
	return m_TYPEScaleDrawer;
}

CScaleColor CDrawingVertScaleData::GetScaleColor() const
{
	return m_scaleColor;
}

CGraphRegionColor CDrawingVertScaleData::GetGraphRegionColor() const
{
	return m_graphRegionColor;
}

COLORREF CDrawingVertScaleData::GetCompartLineColor() const
{
	return m_graphRegionColor.GetFrameColor();
}

CDisplayAttributes CDrawingVertScaleData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CDisplayDataMinMax CDrawingVertScaleData::GetDisplayDataMinMax() const
{
	return m_displayDataMinMax;
}

CIndicatorInfo* CDrawingVertScaleData::GetIndicatorInfo() const
{
	return m_pIndicatorInfo;
}

CPacket* CDrawingVertScaleData::GetPacket() const
{
	return m_pPacket;
}

// 호가단위, 기준가 사용 - ojtaso (20070917)
double CDrawingVertScaleData::GetHogaUnit( BOOL bVolumeForSale) const
{
	if(!m_pPacketList)
		return 0.;

	// (2008/4/28 - Seung-Won, Bae) for VolumeForSafe
	CString strMsgKey;
	if( bVolumeForSale)	strMsgKey = "HBPRICE";
	else				strMsgKey = "HOGAUNIT";

	CString strValue;
	if(m_pPacketList->GetHelpMessageItem( strMsgKey, strValue))
		return atof(strValue);
	else
		return 0.;
}

// 호가단위, 기준가 사용 - ojtaso (20070917)
double CDrawingVertScaleData::GetStandardValue() const
{
	if(!m_pPacketList)
		return 0.;

	CPacket* pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_CLOSE));

	if(pPacket)
		return pPacket->GetStandardValue();
	else
		return 0.;
}

// ----------------------------------------------------------------------------
void CDrawingVertScaleData::SetSubGraphNames(const CString& subGraphNames)
{
	m_strSubGraphNames = subGraphNames;
}

void CDrawingVertScaleData::SetPacketNames(const CString& packetNames)
{
	m_strPacketNames = packetNames;
}

void CDrawingVertScaleData::SetIndicatorName(const CString& IndicatorName)
{
	m_strIndicatorName = IndicatorName;
}

void CDrawingVertScaleData::SetScaleDrawerType(const CScaleBaseData::VERTSCALEDRAWERTYPE& drawType)
{
	m_TYPEScaleDrawer = drawType;
}

void CDrawingVertScaleData::SetDisplayDataMinMax(const CDisplayDataMinMax& displayDataMinMax)
{
	m_displayDataMinMax = displayDataMinMax;
}

void CDrawingVertScaleData::SetIndicatorInfo(CIndicatorInfo* pIndicatorInfo)
{
	m_pIndicatorInfo = pIndicatorInfo;
}

void CDrawingVertScaleData::SetPacket(CPacket* pPacket)
{
	m_pPacket = pPacket;
}

///////////////////////////////////////////////////////////////////////////////
// class CTextInRightScaleRegionDrawer - 오른쪽 스케일 영역에 그릴 text에 대한 정보

CTextInRightScaleRegionDrawer::CTextInRightScaleRegionDrawer(const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList, 
		const CRect& region, const int nCurDataType, const int nDataEnd):
	m_strGraphName(strGraphName), 
	m_pSubGraphPacketList(pSubGraphPacketList), 
	m_region(region), 
	m_nCurDataType(nCurDataType), 
	m_nDataEnd(nDataEnd)
{
}

void CTextInRightScaleRegionDrawer::SetGraphName(const CString& strGraphName)
{
	m_strGraphName = strGraphName;
}

void CTextInRightScaleRegionDrawer::SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList)
{
	m_pSubGraphPacketList = pSubGraphPacketList;
}

void CTextInRightScaleRegionDrawer::SetRegion(const CRect& region)
{
	m_region = region;
}

void CTextInRightScaleRegionDrawer::SetCurDataType(const int nCurDataType)
{
	m_nCurDataType = nCurDataType;
}

void CTextInRightScaleRegionDrawer::SetDataEnd(const int nDataEnd)
{
	m_nDataEnd = nDataEnd;
}

CString CTextInRightScaleRegionDrawer::GetGraphName() const
{
	return m_strGraphName;
}

CSubGraphPacketList* CTextInRightScaleRegionDrawer::GetSubGraphPacketList() const
{
	return m_pSubGraphPacketList;
}

CRect CTextInRightScaleRegionDrawer::GetRegion() const
{
	return m_region;
}

int CTextInRightScaleRegionDrawer::GetCurDataType() const
{
	return m_nCurDataType;
}

int CTextInRightScaleRegionDrawer::GetDataEnd() const
{
	return m_nDataEnd;
}

bool CTextInRightScaleRegionDrawer::IsTextDrawing() const
{
	if(m_pSubGraphPacketList == NULL)
		return false;

	return (m_nCurDataType > 0);
}
