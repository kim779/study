// CalculatorData.cpp: implementation of the CCalculatorData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalculatorData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// class CHorzDisplayDataMinMaxInputData

CHorzDisplayDataMinMaxInputData::CHorzDisplayDataMinMaxInputData(CPacket* pPacket, 
		const CString& strPacketName, const CDisplayAttributes& displayAttributes):
	m_pPacket(pPacket),
	m_strPacketName(strPacketName),
	m_displayAttributes(displayAttributes)
{
}

void CHorzDisplayDataMinMaxInputData::SetPacket(CPacket* pPacket)
{
	m_pPacket = pPacket;
}

void CHorzDisplayDataMinMaxInputData::SetPacketName(const CString& strPacketName)
{
	m_strPacketName = strPacketName;
}

void CHorzDisplayDataMinMaxInputData::SetDisplayAttributes(const CDisplayAttributes& displayAttributes)
{
	m_displayAttributes = displayAttributes;
}

// ----------------------------------------------------------------------------
CPacket* CHorzDisplayDataMinMaxInputData::GetPacket() const
{
	return m_pPacket;
}

CString CHorzDisplayDataMinMaxInputData::GetPacketName() const
{
	return m_strPacketName;
}

CDisplayAttributes CHorzDisplayDataMinMaxInputData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}


///////////////////////////////////////////////////////////////////////////////
// class CVertDisplayDataMinMaxInputData

CVertDisplayDataMinMaxInputData& CVertDisplayDataMinMaxInputData::operator=(const CVertDisplayDataMinMaxInputData& data)
{
	if(this == &data)
		return *this;

	m_strIndicatorName = data.GetIndicatorName();
	m_strGraphName = data.GetGraphName();
	m_strScaleData = data.GetScaleData();
	m_displayAttributes = data.GetDisplayAttributes();
	m_pSubGraphData = data.GetSubGraphData();
	m_pSubGraphPacketList = data.GetSubGraphPacketList();
	return *this;
}

CVertDisplayDataMinMaxInputData::CVertDisplayDataMinMaxInputData():
	m_pSubGraphData(NULL),
	m_pSubGraphPacketList(NULL)
{
}

CVertDisplayDataMinMaxInputData::CVertDisplayDataMinMaxInputData( const CString &p_strIndicatorName, const CString& GraphName, const CString& scaleData, const CDisplayAttributes& displayAttributes):
	m_strIndicatorName( p_strIndicatorName),
	m_strGraphName(GraphName),
	m_strScaleData(scaleData),
	m_displayAttributes(displayAttributes),
	m_pSubGraphData(NULL),
	m_pSubGraphPacketList(NULL)
{
}

void CVertDisplayDataMinMaxInputData::SetSubGraphData(CSubGraphData* pSubGraphData)
{
	m_pSubGraphData = pSubGraphData;
}

void CVertDisplayDataMinMaxInputData::SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList)
{
	m_pSubGraphPacketList = pSubGraphPacketList;
}

// ----------------------------------------------------------------------------
CString CVertDisplayDataMinMaxInputData::GetIndicatorName() const
{
	return m_strIndicatorName;
}

CString CVertDisplayDataMinMaxInputData::GetGraphName() const
{
	return m_strGraphName;
}

CString CVertDisplayDataMinMaxInputData::GetScaleData() const
{
	return m_strScaleData;
}

CDisplayAttributes CVertDisplayDataMinMaxInputData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CGraphBaseData::GRAPHTYPE CVertDisplayDataMinMaxInputData::GetSubGraphType() const
{
	if(m_pSubGraphData == NULL)
		return CGraphBaseData::GTNONE;
	return m_pSubGraphData->GetType();
}

CGraphStyle CVertDisplayDataMinMaxInputData::GetSubGraphStyle() const
{
	if(m_pSubGraphData == NULL)
		return CGraphStyle();
	return m_pSubGraphData->GetStyle();
}

CSubGraphData* CVertDisplayDataMinMaxInputData::GetSubGraphData() const
{
	return m_pSubGraphData;
}

CSubGraphPacketList* CVertDisplayDataMinMaxInputData::GetSubGraphPacketList() const
{
	return m_pSubGraphPacketList;
}

void CVertDisplayDataMinMaxInputData::SetDataStartEndInBlock(int nStart, int nEnd)
{
	m_displayAttributes.SetDataStartInBlock(nStart);
	m_displayAttributes.SetDataEndInBlock(nEnd);
}

///////////////////////////////////////////////////////////////////////////////
// class CDrawingRegionCalData

CDrawingRegionCalData::CDrawingRegionCalData(
	const CRect& blockRegion,
	const int graphRegionFrameThickness,
	const bool bIsGraphTextDrawCheck,
	const bool bIsGraphTextDraw,
	const bool bIsGraphTitleDraw,
	const bool bIsVertScale,
	const CHorzScaleRegionHeight& horzScaleRegionHeight,
	const CVertScaleRegionWidth& vertScaleRegionWidth,
	const BOOL p_bBlockMoveDelete
	):
	m_blockRegion(blockRegion),
	m_nGraphRegionFrameThickness(graphRegionFrameThickness),
	m_bIsGraphTextDrawCheck(bIsGraphTextDrawCheck),
	m_bIsGraphTextDraw(bIsGraphTextDraw),
	m_bIsGraphTitleDraw(bIsGraphTitleDraw),
	m_bIsVertScale(bIsVertScale),
	m_horzScaleRegionHeight(horzScaleRegionHeight),
	m_vertScaleRegionWidth(vertScaleRegionWidth),
	m_nMaxVertScaleRegionWidth(0),
	m_bBlockMoveDelete( p_bBlockMoveDelete)
{
}

void CDrawingRegionCalData::SetGraphTitleDraw(const bool bIsGraphTitleDraw)
{
	m_bIsGraphTitleDraw = bIsGraphTitleDraw;
}

void CDrawingRegionCalData::SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth)
{
	m_nMaxVertScaleRegionWidth = maxVertScaleRegionWidth;
}

void CDrawingRegionCalData::SetDisplayAttributes(const CDisplayAttributes& displayAttributes)
{
	m_displayAttributes = displayAttributes;
}

// ----------------------------------------------------------------------------
CRect CDrawingRegionCalData::GetBlockRegion() const
{
	return m_blockRegion;
}

int CDrawingRegionCalData::GetGraphRegionFrameThickness() const
{
	return m_nGraphRegionFrameThickness;
}

bool CDrawingRegionCalData::IsGraphTextDrawCheck() const
{
	return m_bIsGraphTextDrawCheck;
}

bool CDrawingRegionCalData::IsGraphTextDraw() const
{
	return m_bIsGraphTextDraw;
}

bool CDrawingRegionCalData::IsGraphTitleDraw() const
{
	return m_bIsGraphTitleDraw;
}

bool CDrawingRegionCalData::IsVertScale() const
{
	return m_bIsVertScale;
}

int CDrawingRegionCalData::GetMaxVertScaleRegionWidth() const
{
	return m_nMaxVertScaleRegionWidth;
}

const CHorzScaleRegionHeight& CDrawingRegionCalData::GetHorzScaleRegionHeight() const
{
	return m_horzScaleRegionHeight;
}

const CVertScaleRegionWidth& CDrawingRegionCalData::GetVertScaleRegionWidth() const
{
	return m_vertScaleRegionWidth;
}

CDisplayAttributes CDrawingRegionCalData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}


///////////////////////////////////////////////////////////////////////////////
// class CHorzScaleMaxWidthCalculatorData

CHorzScaleMaxWidthCalculatorData::CHorzScaleMaxWidthCalculatorData(CScaleCompart* pScaleCompart, CPacket* pPacket, 
		const CString& strPacketName, const CDisplayAttributes& displayAttributes):
	m_pScaleCompart(pScaleCompart),
	m_pPacket(pPacket),
	m_strPacketName(strPacketName),
	m_displayAttributes(displayAttributes)
{
}

CScaleCompart* CHorzScaleMaxWidthCalculatorData::GetScaleCompart() const
{
	return m_pScaleCompart;
}

CPacket* CHorzScaleMaxWidthCalculatorData::GetPacket() const
{
	return m_pPacket;
}

CString CHorzScaleMaxWidthCalculatorData::GetPacketName() const
{
	return m_strPacketName;
}

CDisplayAttributes CHorzScaleMaxWidthCalculatorData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}


///////////////////////////////////////////////////////////////////////////////
// class CVertScaleMaxWidthCalculatorData

CVertScaleMaxWidthCalculatorData::CVertScaleMaxWidthCalculatorData(const bool bIsRight, const CDisplayAttributes& displayAttributes, const CVerticalScale& vertScale, BOOL p_bIsDualScale):
	m_bIsRight(bIsRight),
	m_nCalcRiseFallRate_CurData(0),
	m_vertScale(vertScale),
	m_displayAttributes(displayAttributes),
	m_pIndicatorInfo(NULL),
	m_pSubGraphPacketList(NULL),
	m_bIsDualScale( p_bIsDualScale)		// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
{
}

void CVertScaleMaxWidthCalculatorData::SetCalcRiseFallRate_CurData(const int nCalcRiseFallRate_CurData)
{
	m_nCalcRiseFallRate_CurData = nCalcRiseFallRate_CurData;
}

void CVertScaleMaxWidthCalculatorData::SetGraphName(const CString GraphName)
{
	m_strGraphName = GraphName;
}

void CVertScaleMaxWidthCalculatorData::SetSubGraphName(const CString& subGraphName)
{
	m_strSubGraphName = subGraphName;
}

void CVertScaleMaxWidthCalculatorData::SetIndicatorInfo(CIndicatorInfo* pIndiInfo)
{
	m_pIndicatorInfo = pIndiInfo;
}

void CVertScaleMaxWidthCalculatorData::SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList)
{
	m_pSubGraphPacketList = pSubGraphPacketList;
}

// ----------------------------------------------------------------------------
bool CVertScaleMaxWidthCalculatorData::DoesRight() const
{
	return m_bIsRight;
}

int CVertScaleMaxWidthCalculatorData::GetCalcRiseFallRate_CurData() const
{
	return m_nCalcRiseFallRate_CurData;
}

CVerticalScale CVertScaleMaxWidthCalculatorData::GetVerticalScale() const
{
	return m_vertScale;
}

CVerticalScale& CVertScaleMaxWidthCalculatorData::GetVerticalScale()
{
	return m_vertScale;
}

CString CVertScaleMaxWidthCalculatorData::GetGraphName() const
{
	return m_strGraphName;
}

CString CVertScaleMaxWidthCalculatorData::GetSubGraphName() const
{
	return m_strSubGraphName;
}

CString CVertScaleMaxWidthCalculatorData::GetVertScalePacketName() const
{
	return m_vertScale.GetScaleData();
}

CDisplayAttributes CVertScaleMaxWidthCalculatorData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CIndicatorInfo* CVertScaleMaxWidthCalculatorData::GetIndicatorInfo() const
{
	return m_pIndicatorInfo;
}

CSubGraphPacketList* CVertScaleMaxWidthCalculatorData::GetSubGraphPacketList() const
{
	return m_pSubGraphPacketList;
}

// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
void CVertScaleMaxWidthCalculatorData::SetDualScale( BOOL p_bIsDualScale)
{
	m_bIsDualScale = p_bIsDualScale;
}
BOOL CVertScaleMaxWidthCalculatorData::IsDualScale( void) const
{
	return m_bIsDualScale;
}

// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		It is initialized in CSubGraph::GetDisplayDataMinMax() for CMainBlockImp::CalculateDataForDrawing().
void CVertScaleMaxWidthCalculatorData::SetSubGraphDisplayMinMax( const CDisplayDataMinMax &p_displayDataMinMax)
{
	m_displayDataMinMax = p_displayDataMinMax;
}
