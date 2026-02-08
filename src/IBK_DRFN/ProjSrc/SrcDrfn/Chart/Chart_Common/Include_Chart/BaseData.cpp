// BaseData.cpp: implementation of the BaseData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseData.h"

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
// class CDataBarRegion  -> graph, scale 영역 저장

CDataBarRegion::CDataBarRegion():
	m_bIsTop(false),
	m_bIsBottom(false)
{
}

CDataBarRegion::CDataBarRegion(const bool bIsTop, const bool bIsBottom):
	m_bIsTop(bIsTop),
	m_bIsBottom(bIsBottom)
{
}

void CDataBarRegion::SetTop(const bool bIsTop)
{
	m_bIsTop = bIsTop;
}

void CDataBarRegion::SetBottom(const bool bIsBottom)
{
	m_bIsBottom = bIsBottom;
}

CRect CDataBarRegion::GetDrawingRegion_Center(const CRect& region, const int nHight) const
{
	int nTopHight = GetDataBarHight(m_bIsTop, region.Height(), nHight);
	int nBottomHight = GetDataBarHight(m_bIsBottom, region.Height(), nHight);
	CRect rect(region.left, region.top + nTopHight, region.right, region.bottom - nBottomHight);
	rect.NormalizeRect();
	return rect;
}

CRect CDataBarRegion::GetDrawingRegion_Top(const CRect& region, const int nHight) const
{
	int nTopHight = GetDataBarHight(m_bIsTop, region.Height(), nHight);
	return CRect(region.left, region.top, region.right, region.top + nTopHight);
}

CRect CDataBarRegion::GetDrawingRegion_Bottom(const CRect& region, const int nHight) const
{
	int nBottomHight = GetDataBarHight(m_bIsBottom, region.Height(), nHight);
	return CRect(region.left, region.bottom - nBottomHight, region.right, region.bottom);
}

bool CDataBarRegion::IsTopDataBar() const
{
	return m_bIsTop;
}

bool CDataBarRegion::IsBottomDataBar() const
{
	return m_bIsBottom;
}


// private ====================================================================
int CDataBarRegion::GetDataBarHight(const bool bIsDataBar, const nMaxHight, const int nMinHight) const
{
	if(!bIsDataBar || nMinHight <= 0 || nMaxHight < nMinHight * 3)
		return 0;

	return nMinHight;
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphRegions  -> graph, scale 영역 저장

CGraphRegions::CGraphRegions():
	m_noFrameRegion(CRect(0, 0, 0, 0)),
	m_nFontSize(0),
	m_bIsTitle(false),
	m_bIsText(false),
	m_bIsVertScale(true)
{
}

CGraphRegions::CGraphRegions(const CRect& noFrameRegion, const int nFontSize, const bool bIsTitle, const bool bIsText, 
	const bool bIsVertScale, const bool bIsTopDataBar, const bool bIsBottomDataBar):
	m_noFrameRegion(noFrameRegion),
	m_nFontSize(nFontSize),
	m_bIsTitle(bIsTitle),
	m_bIsText(bIsText),
	m_bIsVertScale(bIsVertScale),
	m_dataBar(CDataBarRegion(bIsTopDataBar, bIsBottomDataBar))
{
}

void CGraphRegions::SetNoFrameRegion(const CRect& region)
{
	m_noFrameRegion = region;
}

void CGraphRegions::SetFontSize(const int nSize)
{
	m_nFontSize = nSize;
}

void CGraphRegions::OffsetFontSize(const int nSize)
{
	m_nFontSize += nSize;
}

void CGraphRegions::SetTitle(const bool bIsTitle)
{
	m_bIsTitle = bIsTitle;
}

void CGraphRegions::SetText(const bool bIsText)
{
	m_bIsText = bIsText;
}

void CGraphRegions::SetVertScale(const bool bIsVertScale)
{
	m_bIsVertScale = bIsVertScale;
}

// ----------------------------------------------------------------------------
bool CGraphRegions::IsTitle() const
{
	return m_bIsTitle;
}

bool CGraphRegions::IsText() const
{
	return m_bIsText;
}

CDataBarRegion& CGraphRegions::GetDataBarRegion()
{
	return m_dataBar;
}

CRect CGraphRegions::GetFrameRegion(const int frameThickness) const
{
	CRect graphFrameRegion = m_noFrameRegion;
	graphFrameRegion.InflateRect(frameThickness, frameThickness);
	return graphFrameRegion;
}

CRect CGraphRegions::GetNoFrameRegion() const
{
	return m_noFrameRegion;
}

CRect CGraphRegions::GetNoFrameRegionAndMargin() const
{
	CRect realRegion = m_noFrameRegion;
	if(realRegion.Width() > MARGIN*5)
		realRegion.right -= MARGIN*2;

	return realRegion;
}

CRect CGraphRegions::GetTitleRegion() const
{
	CRect titleRegion(m_noFrameRegion.left, m_noFrameRegion.top, m_noFrameRegion.right, m_noFrameRegion.top);
	if(m_bIsTitle)
		titleRegion.bottom += m_nFontSize;

	return titleRegion;
}

// title과 textHeight 와 margin을 제외한 영역
CRect CGraphRegions::GetDrawingRegion(const CGraphRegions::REGIONPOSITION position) const
{
	CRect region = GetNoTitleRegion(GetRealDrawingRegion());

	switch(position){
	case CENTER:
		return GetNoTextCenterRegion(m_dataBar.GetDrawingRegion_Center(region, m_nFontSize));
	case TOP:
		return m_dataBar.GetDrawingRegion_Top(region, m_nFontSize);
	case BOTTOM:
		return m_dataBar.GetDrawingRegion_Bottom(region, m_nFontSize);
	}
	return region;
}

CRect CGraphRegions::GetDrawingRegion(const CGraphBaseData::GRAPHTYPE eGraphType, const CGraphStyle& graphStyle) const
{
	if(eGraphType != CGraphBaseData::DataView_Type)
		return GetDrawingRegion(CENTER);

	// 상단식 바형
	if(graphStyle.GetGraphDataViewStyle() == CGraphBaseData::TopDataBar)
		return GetDrawingRegion(TOP);
	// 하단식 바형
	else if(graphStyle.GetGraphDataViewStyle() == CGraphBaseData::BottomDataBar)
		return GetDrawingRegion(BOTTOM);

	return GetDrawingRegion(CENTER);
}


// private ====================================================================
// title가 없고 세로 scale 있을 경우에만 top에 margin을 준다.
// 오른쪽에 margin을 준다.
CRect CGraphRegions::GetRealDrawingRegion() const
{
	CRect realRegion = GetNoFrameRegionAndMargin();
	if(m_bIsTitle || !m_bIsVertScale)
		return realRegion;

	// Title 이 없고 상단에 DataBar 가 없는 경우 여백을 준다.
	if(!m_dataBar.IsTopDataBar())
		realRegion.top += MARGIN;

	return realRegion;
}

CRect CGraphRegions::GetNoTitleRegion(const CRect& region) const
{
	if(!m_bIsTitle){
		return GetRegionInDataBar(region);
	}

	return CRect(region.left, region.top + m_nFontSize, region.right, region.bottom);
}

CRect CGraphRegions::GetNoTextCenterRegion(const CRect& region) const
{
	if(!m_bIsText){
		return GetRegionInDataBar(region);
	}

	return CRect(region.left, region.top + m_nFontSize, region.right, region.bottom - m_nFontSize);
}

CRect CGraphRegions::GetRegionInDataBar(const CRect& region) const
{
	CRect rect = region;
	if(m_dataBar.IsTopDataBar())
		rect.top += MARGIN;
	
	if(m_dataBar.IsBottomDataBar())
		rect.bottom -= MARGIN;

	return rect;
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

CRect CDrawingRegion::GetGraphFrameRegion(const int graphRegionFrameThickness) const
{
	return m_graphRegions.GetFrameRegion(graphRegionFrameThickness);
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
// class CDisplayDataMinMax

CDisplayDataMinMax& CDisplayDataMinMax::operator=(const CDisplayDataMinMax& displayDataMinMax)
{
	if(this == &displayDataMinMax)
		return *this;

	m_dMin = displayDataMinMax.GetDisplayMin();
	m_dMax = displayDataMinMax.GetDisplayMax();
	return *this;
}

CDisplayDataMinMax::CDisplayDataMinMax():
	m_dMin(0.0),
	m_dMax(0.0)
{
}

CDisplayDataMinMax::CDisplayDataMinMax(const double& dMin, const double& dMax):
	m_dMin(dMin),
	m_dMax(dMax)
{
}

void CDisplayDataMinMax::SetDisplayData(const double& dMin, const double& dMax)
{
	m_dMin = dMin;
	m_dMax = dMax;
}

void CDisplayDataMinMax::SetDisplayMin(const double& dMin)
{
	m_dMin = dMin;
}

void CDisplayDataMinMax::SetDisplayMax(const double& dMax)
{
	m_dMax = dMax;
}

void CDisplayDataMinMax::Inflate(const CDisplayDataMinMax& displayDataMinMax)
{
	if(displayDataMinMax.GetDisplayMin() < m_dMin)
		m_dMin = displayDataMinMax.GetDisplayMin();
	if(displayDataMinMax.GetDisplayMax() > m_dMax)
		m_dMax = displayDataMinMax.GetDisplayMax();
}

void CDisplayDataMinMax::Inflate(const double& dMinGap, const double& dMaxGap)
{
	m_dMin -= dMinGap;
	m_dMax += dMaxGap;
}

// ----------------------------------------------------------------------------
double& CDisplayDataMinMax::GetDisplayMin()
{
	return m_dMin;
}

double& CDisplayDataMinMax::GetDisplayMax()
{
	return m_dMax;
}

double CDisplayDataMinMax::GetDisplayMin() const
{
	return m_dMin;
}

double CDisplayDataMinMax::GetDisplayMax() const
{
	return m_dMax;
}

double CDisplayDataMinMax::GetCount() const
{
	// 두 차가 0.xxx 일 경우에 그 차가 정확하지 않다.
	// -> 소숫점 부분의 제일 마지막 자릿수(10으로 함)에서 반올림한다.
	double dDifference = m_dMax - m_dMin;
	if(dDifference > 0 && dDifference < 1){
		dDifference = CMath::Round(dDifference, 10);
	}
	return dDifference;
}

double CDisplayDataMinMax::GetMarginDisplayMin() const
{
	return (m_dMin - (GetCount() * 0.06));
}

double CDisplayDataMinMax::GetMarginDisplayMax() const
{
	return (m_dMax + (GetCount() * 0.06));
}

bool CDisplayDataMinMax::IsAllZero() const
{
	return (m_dMax == 0.0 && m_dMin == 0.0);
}

bool CDisplayDataMinMax::IsEqual() const
{
	return (m_dMax == m_dMin);
}

// 현재값이 최대값에 가까운지의 여부
bool CDisplayDataMinMax::IsCloseToMaxValue(const double& dCur, const bool bIsInvert) const
{
	// "뒤집기"인 경우 
	if(bIsInvert)
		return (fabs(m_dMax - dCur) <= fabs(m_dMin - dCur));
	
	return (fabs(m_dMax - dCur) >= fabs(m_dMin - dCur));
}


///////////////////////////////////////////////////////////////////////////////
// class CDrawingGraphData

// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 관리한다.
CDrawingGraphData::CDrawingGraphData(CIndicatorList* pIndicatorList, 
		const CDrawingOption::DRAWINGORDER eDrawingOrderType, 
		const CGraphRegions& graphRegions, const CRect& scaleTextRegion, 
		const CDisplayDataMinMax& displayDataHorzMinMax, const CString& horzScalePacketName, 
		const CEtcGraphColor& etcGraphColor, const CDisplayAttributes& displayAttributes, 
		const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType, 
		const bool bIsConditionTitleDraw, const bool bIsDataTitleDraw):
	m_pIndicatorList(pIndicatorList), 
	m_eDrawingOrderType(eDrawingOrderType),
	m_graphRegions(graphRegions),
	m_scaleTextRegion(scaleTextRegion),
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
	m_pSubGraphPacketList(NULL)
{
}

void CDrawingGraphData::SetDisplayDataVertMinMax(const CDisplayDataMinMax& displayDataVertMinMax)
{
	m_displayDataVertMinMax = displayDataVertMinMax;
}

void CDrawingGraphData::SetFunctionName(const CString& functionName)
{
	m_strFunctionName = functionName;
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

const CGraphRegions& CDrawingGraphData::GetGraphRegions() const
{
	return m_graphRegions;
}

CRect CDrawingGraphData::GetScaleTextRegion() const
{
	return m_scaleTextRegion;
}

CDisplayDataMinMax CDrawingGraphData::GetDisplayDataHorzMinMax() const
{
	return m_displayDataHorzMinMax;
}

CDisplayDataMinMax CDrawingGraphData::GetDisplayDataVertMinMax() const
{
	return m_displayDataVertMinMax;
}

CString CDrawingGraphData::GetFunctionName() const
{
	return m_strFunctionName;
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


///////////////////////////////////////////////////////////////////////////////
// class CDrawingHorzScaleData

CDrawingHorzScaleData::CDrawingHorzScaleData(const CRect& drawingGraphRegion, const CGraphBaseData::GRAPHTYPE& eHeadGraphType, 
		const CGraphStyle& headGraphStyle, const CGraphRegionColor& graphRegionColor, 
		const CScaleColor& scaleColor, const CDisplayAttributes& displayAttributes, 
		const CDisplayDataMinMax& displayDataMinMax, CPacket* pPacket):
	m_drawingGraphRegion(drawingGraphRegion),
	m_eHeadGraphType(eHeadGraphType),
	m_headGraphStyle(headGraphStyle),
	m_TYPEScaleDrawer(CScaleBaseData::HORZ_NOTYPE),
	m_graphRegionColor(graphRegionColor),
	m_scaleColor(scaleColor),
	m_displayAttributes(displayAttributes),
	m_displayDataMinMax(displayDataMinMax),
	m_pPacket(pPacket)
{
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


///////////////////////////////////////////////////////////////////////////////
// class CDrawingVertScaleData

CDrawingVertScaleData::CDrawingVertScaleData(const CRect& noFrameGraphRegion, const CRect& drawingGraphRegion, 
		const CGraphRegionColor& graphRegionColor, const CScaleColor& scalrColor, 
		const bool bIsVertScaleInvertType, const bool bIsVertScaleLogType, const CDisplayAttributes& displayAttributes):
	m_noFrameGraphRegion(noFrameGraphRegion),
	m_drawingGraphRegion(drawingGraphRegion),
	m_TYPEScaleDrawer(CScaleBaseData::VERT_NOTYPE),
	m_graphRegionColor(graphRegionColor),
	m_scaleColor(scalrColor),
	m_bIsVertScaleInvertType(bIsVertScaleInvertType),
	m_displayAttributes(displayAttributes),
	m_pIndicatorInfo(NULL),
	m_pPacket(NULL)
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

CString CDrawingVertScaleData::GetFunctionName() const
{
	return m_strFunctionName;
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

// ----------------------------------------------------------------------------
void CDrawingVertScaleData::SetSubGraphNames(const CString& subGraphNames)
{
	m_strSubGraphNames = subGraphNames;
}

void CDrawingVertScaleData::SetPacketNames(const CString& packetNames)
{
	m_strPacketNames = packetNames;
}

void CDrawingVertScaleData::SetFunctionName(const CString& functionName)
{
	m_strFunctionName = functionName;
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

CTextInRightScaleRegionDrawer::CTextInRightScaleRegionDrawer(const CString& strFunctionName, CSubGraphPacketList* pSubGraphPacketList, 
		const CRect& region, const int nCurDataType, const int nDataEnd):
	m_strFunctionName(strFunctionName), 
	m_pSubGraphPacketList(pSubGraphPacketList), 
	m_region(region), 
	m_nCurDataType(nCurDataType), 
	m_nDataEnd(nDataEnd)
{
}

void CTextInRightScaleRegionDrawer::SetFunctionName(const CString& strFunctionName)
{
	m_strFunctionName = strFunctionName;
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

CString CTextInRightScaleRegionDrawer::GetFunctionName() const
{
	return m_strFunctionName;
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
