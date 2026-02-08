// ChartObjectBaseData.cpp: implementation of the CMainBlockBaseData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartObjectBaseData.h"

#include "../Include_Chart/Dll_Load/IndicatorInfo.h"	// for CIndicatorInfo
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CMainAttributes

CMainAttributes& CMainAttributes::operator=(const CMainAttributes& data)
{
	if(this == &data)
		return *this;

	m_pMainBlockImpl = data.GetMainBlockImpl();
	m_pIChartCtrl = data.GetIChartCtrl();
	m_bDesignMode = data.IsDesignMode();
	m_strOCXPath = data.GetOCXPath();
	m_pPacketList = data.GetPacketList();
	m_pIndicatorList = data.GetIndicatorList();
	m_pMainChartFormulate = data.GetMainChartFormulate();
	m_pxScaleManager = data.GetXScaleManager();
	m_pPacketListManager = data.GetPacketListManager();
	m_pToolBarManager = data.GetToolBarManager();

	return *this;
}

CMainAttributes::CMainAttributes():
	m_pMainBlockImpl(NULL),
	m_pIChartCtrl(NULL),
	m_bDesignMode(false),
	m_pPacketList(NULL),
	m_pIndicatorList(NULL),
	m_pMainChartFormulate( NULL),
	m_pxScaleManager(NULL),
	m_pPacketListManager(NULL),
	m_pToolBarManager(NULL)	// 복수종목 관리 : 복수종목 - ojtaso (20070111)
{
}

void CMainAttributes::SetMainBlockImpl(CMainBlockImp* pMainBlockImpl)
{
	m_pMainBlockImpl = pMainBlockImpl;
}

void CMainAttributes::SetIChartCtrl( IChartCtrl* p_pIChartCtrl)
{
	m_pIChartCtrl = p_pIChartCtrl;
}

void CMainAttributes::SetDesignMode(const bool designMode)
{
	m_bDesignMode = designMode;
}

void CMainAttributes::SetOCXPath(const CString& strOCXPath)
{
	m_strOCXPath = strOCXPath;
}

void CMainAttributes::SetPacketList(CPacketList* pPacketList)
{
	m_pPacketList = pPacketList;
}

void CMainAttributes::SetIndicatorList(CIndicatorList* pIndicatorList)
{
	m_pIndicatorList = pIndicatorList;
}


void CMainAttributes::SetMainChartFormulate( CMainChartFormulate *p_pMainChartFormulate)
{
	m_pMainChartFormulate = p_pMainChartFormulate;
}

void CMainAttributes::SetXScaleManager(CXScaleManager* pxScaleManager)
{
	m_pxScaleManager = pxScaleManager;
}

// 복수종목 관리 : 복수종목 - ojtaso (20070111)
void CMainAttributes::SetPacketListManager(CPacketListManager* pPacketListManager)
{
	m_pPacketListManager = pPacketListManager;
}

void CMainAttributes::SetToolBarManager(IToolBarManager* pToolBarManager)
{
	m_pToolBarManager = pToolBarManager;
}

// --------------------------------------------------------------------------------------
CMainBlockImp* CMainAttributes::GetMainBlockImpl() const
{
	return m_pMainBlockImpl;
}

IChartCtrl *CMainAttributes::GetIChartCtrl() const
{
	return m_pIChartCtrl;
}

CWnd* CMainAttributes::GetParentWnd() const
{
	if(m_pIChartCtrl == NULL)
		return NULL;

	return m_pIChartCtrl->GetChartWindowPointer();
}

HWND CMainAttributes::GetParent_Handle() const
{
	if(m_pIChartCtrl == NULL)
		return NULL;

	return m_pIChartCtrl->GetWndHandle();
}

bool CMainAttributes::IsDesignMode() const
{
	return m_bDesignMode;
}

CString CMainAttributes::GetOCXPath() const
{
	return m_strOCXPath;
}

CPacketList* CMainAttributes::GetPacketList() const
{
	return m_pPacketList;
}

CIndicatorList* CMainAttributes::GetIndicatorList() const
{
	return m_pIndicatorList;
}

CMainChartFormulate *CMainAttributes::GetMainChartFormulate( void) const
{
	return m_pMainChartFormulate;
}

CXScaleManager* CMainAttributes::GetXScaleManager() const
{
	return m_pxScaleManager;
}
 
// 복수종목 관리 : 복수종목 - ojtaso (20070111)
CPacketListManager* CMainAttributes::GetPacketListManager() const
{
	return m_pPacketListManager;
}

IToolBarManager* CMainAttributes::GetToolBarManager() const
{
	return m_pToolBarManager;
}

// --------------------------------------------------------------------------------------
void CMainAttributes::Invalidate()
{
	if(m_pIChartCtrl == NULL)
		return;

	m_pIChartCtrl->InvalidateOCX();
}

void CMainAttributes::SendMessageToParent(const CBlockBehaviorData& blockBehaviorData)
{
	if(m_pIChartCtrl == NULL || !blockBehaviorData.GetSourceBlockIndex().IsAllValueBiggerThanZero())
		return;

	//m_pIChartCtrl->SendMessage(UM_BLOCK_BEHAVIOR, (WPARAM)&blockBehaviorData);
	m_pIChartCtrl->BehaviorBlock(long(&blockBehaviorData));
}

/*
void CMainAttributes::SendMessageToParent(const CString& strGraphName, const bool bIsAddMargin)
{
	if(m_pIChartCtrl == NULL || strGraphName.IsEmpty())
		return;

	//GetParentWnd()->SendMessage(UM_GRAPH_MARGIN, (WPARAM)(LPCTSTR)strGraphName, bIsAddMargin);
	m_pIChartCtrl->SetGraphMargin(strGraphName, bIsAddMargin);
}
*/

void CMainAttributes::SendMessageToParent_TRData(const CString& strGraphName, const CString& strPacketNames)
{
	if(m_pIChartCtrl == NULL || strPacketNames.IsEmpty())
		return;

	m_pIChartCtrl->RequestTRPacket(strGraphName, strPacketNames);
}

void CMainAttributes::SendMessageToParent_TRData(const CString& strGraphName, const CString& strPacketNames, const int nRequestTRCount, const bool bIsNew)
{
	if(m_pIChartCtrl == NULL || strPacketNames.IsEmpty())
		return;

	m_pIChartCtrl->RequestTRPacket(strGraphName, strPacketNames, nRequestTRCount, bIsNew);
}

void CMainAttributes::SendMessageToParent_CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList)
{
	if(m_pIChartCtrl == NULL || pIndicatorInfo == NULL) return;

#ifdef _DEBUG
	TRACE( "ST Indicator Called : %s (with %d Packets)\r\n", pIndicatorInfo->GetIndicatorName(), packetList.GetCount());
#endif

	m_pIChartCtrl->CalculateIndicatorInTRData(pIndicatorInfo, packetList);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
CChartInfo::CHART_MODE CMainAttributes::GetChartMode( void) const
{
	if( !m_pIChartCtrl) return CChartInfo::DEVELOPER;
	return m_pIChartCtrl->GetChartMode();
}

//////////////////////////////////////////////////////////////////////
// class CBlockAttributes

CBlockAttributes& CBlockAttributes::operator=(const CBlockAttributes& data)
{
	if(this == &data)
		return *this;

	m_eBlockType = data.GetBlockType();
	m_eBlockCompartType = data.GetBlockCompartType();
	m_nMaxBlocksCountInView = data.GetMaxBlocksCountInView();
	return *this;
}

CBlockAttributes::CBlockAttributes():
	m_eBlockType(CBlockBaseData::ROW),
	m_eBlockCompartType(CBlockBaseData::COMPARTLINE),
	m_nMaxBlocksCountInView(5)
{
}

void CBlockAttributes::SetBlockType(const CBlockBaseData::BLOCKTYPE eBlockType)
{
	m_eBlockType = eBlockType;
}

void CBlockAttributes::SetBlockCompartType(const CBlockBaseData::BLOCKCOMPART eBlockCompartType)
{
	m_eBlockCompartType = eBlockCompartType;
}

void CBlockAttributes::SetMaxBlocksCountInView(const int maxBlocksCountInView)
{
	m_nMaxBlocksCountInView = maxBlocksCountInView;
}

// --------------------------------------------------------------------------------------
CBlockBaseData::BLOCKTYPE CBlockAttributes::GetBlockType() const
{
	return m_eBlockType;
}

CBlockBaseData::BLOCKCOMPART CBlockAttributes::GetBlockCompartType() const
{
	return m_eBlockCompartType;
}

int CBlockAttributes::GetMaxBlocksCountInView() const
{
	return m_nMaxBlocksCountInView;
}


//////////////////////////////////////////////////////////////////////
// class CGraphAttributes

CGraphAttributes& CGraphAttributes::operator=(const CGraphAttributes& data)
{
	if(this == &data)
		return *this;

	m_bIsAllTitleDrawHiding = data.DoesAllTitleDrawHiding();
	m_bIsConditionTitleDraw = data.DoesConditionTitleDraw();
	m_bIsDataTitleDraw = data.DoesDataTitleDraw();
	m_bIsAutoDeleteIndicatorInfo = data.DoesAutoDeleteIndicatorInfo();
	m_nGraphBackgroundGapX = data.GetGraphBackgroundGapX();
	m_nGraphBackgroundGapY = data.GetGraphBackgroundGapY();
	m_nGraphRegionFrameThickness = data.GetGraphRegionFrameThickness();
	m_graphRegionColor = data.GetGraphRegionColor();
	m_etcGraphColor = data.GetEtcGraphColor();
	return *this;
}

CGraphAttributes::CGraphAttributes():
	m_bIsAllTitleDrawHiding(false), 
	m_bIsConditionTitleDraw(false),
	m_bIsDataTitleDraw(false),
	m_bIsAutoDeleteIndicatorInfo(false), 
	m_nGraphBackgroundGapX(0),
	m_nGraphBackgroundGapY(0),
	m_nGraphRegionFrameThickness(1)
{
}

void CGraphAttributes::SetAllGraphsTitleDrawHiding(const bool bIsAllTitleDrawHiding)
{
	m_bIsAllTitleDrawHiding = bIsAllTitleDrawHiding;
}

void CGraphAttributes::SetGraphTitleDraw(const bool bIsCondition, const bool bIsData)
{
	m_bIsConditionTitleDraw = bIsCondition;
	m_bIsDataTitleDraw = bIsData;
}

void CGraphAttributes::SetAutoDeleteIndicatorInfo(const bool bIsAutoDeleteIndicatorInfo)
{
	m_bIsAutoDeleteIndicatorInfo = bIsAutoDeleteIndicatorInfo;
}

void CGraphAttributes::SetGraphBackgroundGap(const int nGapX, const int nGapY)
{
	m_nGraphBackgroundGapX = nGapX;
	m_nGraphBackgroundGapY = nGapY;
}

void CGraphAttributes::SetGraphRegionFrameThickness(const int nThickness)
{
	m_nGraphRegionFrameThickness = nThickness;
}

void CGraphAttributes::SetGraphRegionColor(const CGraphRegionColor& graphRegionColor)
{
	m_graphRegionColor = graphRegionColor;
}

void CGraphAttributes::SetEtcGraphColor(const CEtcGraphColor& etcGraphColor)
{
	m_etcGraphColor = etcGraphColor;
}

// --------------------------------------------------------------------------------------
bool CGraphAttributes::DoesAllTitleDrawHiding() const
{
	return m_bIsAllTitleDrawHiding;
}

bool CGraphAttributes::DoesConditionTitleDraw() const
{
	return m_bIsConditionTitleDraw;
}

bool CGraphAttributes::DoesDataTitleDraw() const
{
	return m_bIsDataTitleDraw;
}

bool CGraphAttributes::DoesAutoDeleteIndicatorInfo() const
{
	return m_bIsAutoDeleteIndicatorInfo;
}

int CGraphAttributes::GetGraphBackgroundGapX() const
{
	return m_nGraphBackgroundGapX;
}

int CGraphAttributes::GetGraphBackgroundGapY() const
{
	return m_nGraphBackgroundGapY;
}

int CGraphAttributes::GetGraphRegionFrameThickness() const
{
	return m_nGraphRegionFrameThickness;
}

CGraphRegionColor CGraphAttributes::GetGraphRegionColor() const
{
	return m_graphRegionColor;
}

CEtcGraphColor CGraphAttributes::GetEtcGraphColor() const
{
	return m_etcGraphColor;
}


//////////////////////////////////////////////////////////////////////
// class CScaleAttributes

CScaleAttributes& CScaleAttributes::operator=(const CScaleAttributes& data)
{
	if(this == &data)
		return *this;

	m_bIsHorzScaleMessageBoxShow = data.DoesHorzScaleMessageBoxShow();
	m_bIsOnlyPriceVertScale = data.DoesOnlyPriceVertScale();
	m_scaleRegionAttributes = data.GetScaleRegionAttributes();
	m_scaleColor = data.GetScaleColor();
	return *this;
}

CScaleAttributes::CScaleAttributes():
	m_bIsHorzScaleMessageBoxShow(true),
	m_bIsOnlyPriceVertScale(false)
{
}

void CScaleAttributes::SetHorzScaleMessageBoxShow(const bool bHorzScaleMessageBoxShow)
{
	m_bIsHorzScaleMessageBoxShow = bHorzScaleMessageBoxShow;
}

void CScaleAttributes::SetOnlyPriceVertScale(const bool bIsOnlyPrice)
{
	m_bIsOnlyPriceVertScale = bIsOnlyPrice;
}

void CScaleAttributes::SetScaleRegionAttributes(const CScaleRegionAttributes& scaleRegionAttributes)
{
	m_scaleRegionAttributes = scaleRegionAttributes;
}

void CScaleAttributes::SetMinHorzScaleRegionHeight(const int minHorzScaleRegionHeight)
{
	m_scaleRegionAttributes.SetMinHorzScaleRegionHeight(minHorzScaleRegionHeight);
}

void CScaleAttributes::SetMinVertScaleRegionWidth(const int minVertScaleRegionWidth)
{
	m_scaleRegionAttributes.SetMinVertScaleRegionWidth(minVertScaleRegionWidth);
}

void CScaleAttributes::SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth)
{
	m_scaleRegionAttributes.SetMaxVertScaleRegionWidth(maxVertScaleRegionWidth);
}

void CScaleAttributes::SetScaleColor(const CScaleColor& scaleColor)
{
	m_scaleColor = scaleColor;
}

// --------------------------------------------------------------------------------------
bool CScaleAttributes::DoesHorzScaleMessageBoxShow() const
{
	return m_bIsHorzScaleMessageBoxShow;
}

bool CScaleAttributes::DoesOnlyPriceVertScale() const
{
	return m_bIsOnlyPriceVertScale;
}

CScaleRegionAttributes CScaleAttributes::GetScaleRegionAttributes() const
{
	return m_scaleRegionAttributes;
}

CScaleRegionAttributes& CScaleAttributes::GetScaleRegionAttributes()
{
	return m_scaleRegionAttributes;
}

CScaleColor CScaleAttributes::GetScaleColor() const
{
	return m_scaleColor;
}

CScaleColor& CScaleAttributes::GetScaleColor()
{
	return m_scaleColor;
}

int CScaleAttributes::GetMinHorzScaleRegionHeight() const
{
	return m_scaleRegionAttributes.GetMinHorzScaleRegionHeight();
}

int CScaleAttributes::GetMinVertScaleRegionWidth() const
{
	return m_scaleRegionAttributes.GetMinVertScaleRegionWidth();
}

int CScaleAttributes::GetMaxVertScaleRegionWidth() const
{
	return m_scaleRegionAttributes.GetMaxVertScaleRegionWidth();
}


//////////////////////////////////////////////////////////////////////
// class CViewAttributes

CViewAttributes& CViewAttributes::operator=(const CViewAttributes& data)
{
	if(this == &data)
		return *this;

	m_displayAttributes = data.GetDisplayAttributes();
	m_eDrawingOrderType = data.GetDrawingOrderType();
	m_bIsMessageBoxShow = data.DoesMessageBoxShow();
	m_backgroundColor = data.GetBackgroundColor();
	return *this;
}

CViewAttributes::CViewAttributes():
	m_eDrawingOrderType(CDrawingOption::LEFT),
	m_bIsMessageBoxShow(true),
	m_backgroundColor(RGB(255, 255, 255))
{
}

void CViewAttributes::SetDataRegionInBlock(const int start, const int end)
{
	m_displayAttributes.SetDataStartInBlock(start);
	m_displayAttributes.SetDataEndInBlock(end);
}

void CViewAttributes::SetDrawingOrderType(const CDrawingOption::DRAWINGORDER eDrawingOrderType)
{
	m_eDrawingOrderType = eDrawingOrderType;
}

void CViewAttributes::SetMessageBoxShow(const bool bIsShow)
{
	m_bIsMessageBoxShow = bIsShow;
}

void CViewAttributes::SetBackgroundColor(const COLORREF& color)
{
	m_backgroundColor = color;
}

// --------------------------------------------------------------------------------------
CDisplayAttributes CViewAttributes::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CDrawingOption::DRAWINGORDER CViewAttributes::GetDrawingOrderType() const
{
	return m_eDrawingOrderType;
}

CDrawingOption::DRAWINGORDER& CViewAttributes::GetDrawingOrderType()
{
	return m_eDrawingOrderType;
}

bool CViewAttributes::DoesMessageBoxShow() const
{
	return m_bIsMessageBoxShow;
}

COLORREF CViewAttributes::GetBackgroundColor() const
{
	return m_backgroundColor;
}


//////////////////////////////////////////////////////////////////////
// class CMainBlockBaseData

CMainBlockBaseData::CMainBlockBaseData()
{
	// (2006/12/12 - Seung-Won, Bae) Support Indicator Calculation Enabling Flag
	m_bEnableIndicatorCalculation = TRUE;

	// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
	m_bWithBaseText = TRUE;

	// (2007/6/28 - Seung-Won, Bae) Manage MessageBox Info
	m_bMessageBoxUsed = FALSE;
}

CMainBlockBaseData& CMainBlockBaseData::operator=(const CMainBlockBaseData& data)
{
	if(this == &data)
		return *this;

	m_mainAttributes = data.GetMainAttributes();
	m_blockAttributes = data.GetBlockAttributes();
	m_graphAttributes = data.GetGraphAttributes();
	m_scaleAttributes = data.GetScaleAttributes();
	m_viewAttributes = data.GetViewAttributes();

	// (2006/12/12 - Seung-Won, Bae) Support Indicator Calculation Enabling Flag
	m_bEnableIndicatorCalculation = data.m_bEnableIndicatorCalculation;

	// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
	m_bWithBaseText = data.GetWithBaseText();

	// (2007/6/28 - Seung-Won, Bae) Manage MessageBox Info
	m_bMessageBoxUsed = data.m_bMessageBoxUsed;

	return *this;
}

// --------------------------------------------------------------------------------------
// 가로 scale의 최소 높이
void CMainBlockBaseData::SetMinHorzScaleRegionHeight(const int minHorzScaleRegionHeight)
{
	m_scaleAttributes.GetScaleRegionAttributes().SetMinHorzScaleRegionHeight(minHorzScaleRegionHeight);
}

// 세로 scale의 최소 폭
void CMainBlockBaseData::SetMinVertScaleRegionWidth(const int minVertScaleRegionWidth)
{
	m_scaleAttributes.GetScaleRegionAttributes().SetMinVertScaleRegionWidth(minVertScaleRegionWidth);
}

// 세로 scale의 최대 폭
void CMainBlockBaseData::SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth)
{
	m_scaleAttributes.GetScaleRegionAttributes().SetMaxVertScaleRegionWidth(maxVertScaleRegionWidth);
}

// --------------------------------------------------------------------------------------
CMainAttributes CMainBlockBaseData::GetMainAttributes() const
{
	return m_mainAttributes;
}

CMainAttributes& CMainBlockBaseData::GetMainAttributes()
{
	return m_mainAttributes;
}

CBlockAttributes CMainBlockBaseData::GetBlockAttributes() const
{
	return m_blockAttributes;
}

CBlockAttributes& CMainBlockBaseData::GetBlockAttributes()
{
	return m_blockAttributes;
}

CGraphAttributes CMainBlockBaseData::GetGraphAttributes() const
{
	return m_graphAttributes;
}

CGraphAttributes& CMainBlockBaseData::GetGraphAttributes()
{
	return m_graphAttributes;
}

CScaleAttributes CMainBlockBaseData::GetScaleAttributes() const
{
	return m_scaleAttributes;
}

CScaleAttributes& CMainBlockBaseData::GetScaleAttributes()
{
	return m_scaleAttributes;
}

CViewAttributes CMainBlockBaseData::GetViewAttributes() const
{
	return m_viewAttributes;
}

CViewAttributes& CMainBlockBaseData::GetViewAttributes()
{
	return m_viewAttributes;
}

// --------------------------------------------------------------------------------------
CWnd* CMainBlockBaseData::GetParentWnd() const
{
	return m_mainAttributes.GetParentWnd();
}

HWND CMainBlockBaseData::GetParent_Handle() const
{
	return m_mainAttributes.GetParent_Handle();
}

CPacketList* CMainBlockBaseData::GetPacketList() const
{
	return m_mainAttributes.GetPacketList();
}

CIndicatorList* CMainBlockBaseData::GetIndicatorList() const
{
	return m_mainAttributes.GetIndicatorList();
}

CXScaleManager* CMainBlockBaseData::GetXScaleManager() const
{
	return m_mainAttributes.GetXScaleManager();
}

// 복수종목 관리 : 복수종목 - ojtaso (20070111)
CPacketListManager* CMainBlockBaseData::GetPacketListManager() const
{
	return m_mainAttributes.GetPacketListManager();
}

IToolBarManager* CMainBlockBaseData::GetToolBarManager() const
{
	return m_mainAttributes.GetToolBarManager();
}

bool CMainBlockBaseData::DoesAutoDeleteIndicatorInfo() const
{
	return m_graphAttributes.DoesAutoDeleteIndicatorInfo();
}

int CMainBlockBaseData::GetGraphRegionFrameThickness() const
{
	return m_graphAttributes.GetGraphRegionFrameThickness();
}

int CMainBlockBaseData::GetMinHorzScaleRegionHeight() const
{
	return m_scaleAttributes.GetScaleRegionAttributes().GetMinHorzScaleRegionHeight();
}

int CMainBlockBaseData::GetMinVertScaleRegionWidth() const
{
	return m_scaleAttributes.GetScaleRegionAttributes().GetMinVertScaleRegionWidth();
}

int CMainBlockBaseData::GetMaxVertScaleRegionWidth() const
{
	return m_scaleAttributes.GetScaleRegionAttributes().GetMaxVertScaleRegionWidth();
}

CBlockBaseData::BLOCKTYPE CMainBlockBaseData::GetBlockType() const
{
	return m_blockAttributes.GetBlockType();
}

CBlockBaseData::BLOCKCOMPART CMainBlockBaseData::GetBlockCompartType() const
{
	return m_blockAttributes.GetBlockCompartType();
}

int CMainBlockBaseData::GetMaxBlocksCountInView() const
{
	return m_blockAttributes.GetMaxBlocksCountInView();
}

CGraphRegionColor CMainBlockBaseData::GetGraphRegionColor() const
{
	return m_graphAttributes.GetGraphRegionColor();
}

CDisplayAttributes CMainBlockBaseData::GetDisplayAttributes() const
{
	return m_viewAttributes.GetDisplayAttributes();
}

// --------------------------------------------------------------------------------------
void CMainBlockBaseData::Invalidate()
{
	m_mainAttributes.Invalidate();
}

void CMainBlockBaseData::SendMessageToParent(const CBlockBehaviorData& blockBehaviorData)
{
	m_mainAttributes.SendMessageToParent(blockBehaviorData);
}
/*
void CMainBlockBaseData::SendMessageToParent(const CString& strGraphName, const bool bIsAddMargin)
{
	m_mainAttributes.SendMessageToParent(strGraphName, bIsAddMargin);
}
*/
void CMainBlockBaseData::SendMessageToParent_TRData(const CString& strGraphName, const CString& strPacketNames)
{
	m_mainAttributes.SendMessageToParent_TRData(strGraphName, strPacketNames);
}

void CMainBlockBaseData::SendMessageToParent_TRData(const CString& strGraphName, const CString& strPacketNames, const int nRequestTRCount, const bool bIsNew)
{
	m_mainAttributes.SendMessageToParent_TRData(strGraphName, strPacketNames, nRequestTRCount, bIsNew);
}

void CMainBlockBaseData::SendMessageToParent_CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList)
{
	m_mainAttributes.SendMessageToParent_CalculateIndicatorInTRData(pIndicatorInfo, packetList);
}

// (2007/6/28 - Seung-Won, Bae) Manage MessageBox Info
BOOL CMainBlockBaseData::DoesMessageBoxShow( void) const
{
	if( !GetScaleAttributes().DoesHorzScaleMessageBoxShow()) return false;
	return GetViewAttributes().DoesMessageBoxShow();
}

///////////////////////////////////////////////////////////////////////////////
// class CBlockData

CBlockData::CBlockData():
	m_bIsDoubleSize(false),
	m_bIsVertScaleLogType(false)
{
}

void CBlockData::SetDoubleSize(const bool bIsDoubleSize)
{
	m_bIsDoubleSize = bIsDoubleSize;
}

void CBlockData::SetVertScaleLogType(const bool bIsVertScaleLogType)
{
	m_bIsVertScaleLogType = bIsVertScaleLogType;
}

void CBlockData::SetHorzScale(const CHorizontalScale& horzScale)
{
	m_horzScale = horzScale;
}

void CBlockData::SetDisplayDataHorzMinMax(const CDisplayDataMinMax& displayDataHorzMinMax)
{
	m_displayDataHorzMinMax = displayDataHorzMinMax;
}

// XScale 높이 지정 - ojtaso (20080429)
void CBlockData::SetHorizontalScaleHeight(const int nHeight)
{
	m_horzScale.SetScaleHeight(nHeight);
}

// ----------------------------------------------------------------------------
bool CBlockData::GetDoubleSize() const
{
	return m_bIsDoubleSize;
}

bool CBlockData::GetVertScaleLogType() const
{
	return m_bIsVertScaleLogType;
}

bool& CBlockData::GetVertScaleLogType()
{
	return m_bIsVertScaleLogType;
}

CHorizontalScale CBlockData::GetHorzScale() const
{
	return m_horzScale;
}

CHorizontalScale& CBlockData::GetHorzScale()
{
	return m_horzScale;
}

CDisplayDataMinMax CBlockData::GetDisplayDataHorzMinMax() const
{
	return m_displayDataHorzMinMax;
}

// XScale 높이 지정 - ojtaso (20080429)
int CBlockData::GetHorizontalScaleHeight() const
{
	return m_horzScale.GetScaleHeight();
}

CDisplayDataMinMax& CBlockData::GetDisplayDataHorzMinMax()
{
	return m_displayDataHorzMinMax;
}
