// SubGraph.cpp: implementation of the CSubGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SubGraph.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/PacketList.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/PacketBase.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/GraphDrawer.h"			// GraphDraw.dll
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"		// for CIndicatorInfo
#include "../Include_Chart/Dll_Load/PacketListManager.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/ViewData.h"
#include "../Include_Chart/SelectSubGraphData.h"
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "MainBlockImp.h"									// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
#include "MainBlockImp.h"									// for CMainBlockImp
#include "CalculatorData.h"
#include "ViewMinMaxCalculator.h"
#include "DrawingRegionCalculator.h"
#include "BlockImp.h"
#include "VertScaleGroup.h"
#include "GraphImp.h"
#include "PacketRQ.h"										// for CPacketRQ


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CSubGraph

// subGraph 객체로 만든다.
CSubGraph* CSubGraph::MakeSubGraph(CMainBlockBaseData* pMainBlockBaseData, CSubGraphList* pSubGraphList, const CSubGraphData& subGraphData)
{
	return new CSubGraph(pMainBlockBaseData, pSubGraphList, subGraphData);
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
// 기준선 두께, 모양 설정 - ojtaso (20071213)
CSubGraph* CSubGraph::MakeSubGraph(CMainBlockBaseData* pMainBlockBaseData, CSubGraphList* pSubGraphList, const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle)
{
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	return new CSubGraph(pMainBlockBaseData, pSubGraphList, type, style, drawStyle, name, title, hiding, color, penThickness, paintStyle, baseLineStyle);
}

// 객체를 삭제한다. Make에서 new를 했으므로...
void CSubGraph::Delete(CSubGraph* pSubGraph)
{
	delete pSubGraph;
}


/////////////////////////////////////////////////////////////////////////////////////////////
CSubGraph::CSubGraph(CMainBlockBaseData* pMainBlockBaseData, CSubGraphList* pSubGraphList, const CSubGraphData& subGraphData):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pSubGraphList(pSubGraphList),
	m_subGraphData(subGraphData),
	m_nVerticalScaleMaxWidth(-1)
{
//	SendMessage_SubGraph(true);
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
// 기준선 두께, 모양 설정 - ojtaso (20071213)
CSubGraph::CSubGraph(CMainBlockBaseData* pMainBlockBaseData, CSubGraphList* pSubGraphList, const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pSubGraphList(pSubGraphList),
	m_subGraphData(type, style, drawStyle, name, title, hiding, color, penThickness, paintStyle, baseLineStyle),	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	m_nVerticalScaleMaxWidth(-1)
{
//	SendMessage_SubGraph(true);
}

// public ==================================================================================
void CSubGraph::Delete()
{
//	SendMessage_SubGraph(false);
	CSubGraph::Delete(this);
}

// 단순히 subGraph가 삭제될 경우엔 호출할 필요가 없다.
void CSubGraph::DeleteAllPackets(const bool bIsOnlyNoTRData)
{
	if(m_pMainBlockBaseData == NULL)
		return;

	CPacketList* pPacketList = m_pMainBlockBaseData->GetPacketList();
	if(pPacketList == NULL)
		return;

	POSITION pos = m_subGraphPacketList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = m_subGraphPacketList.GetNext(pos);
		if(pPacket == NULL)
			continue;
		pPacketList->DeleteOrClearPacket(pPacket, true, bIsOnlyNoTRData);
	}
	m_subGraphPacketList.RemoveAll();
}

void CSubGraph::ClearAllPackets(const bool bIsOnlyNoTRData)
{
	if(m_pMainBlockBaseData == NULL)
		return;

	CPacketList* pPacketList = m_pMainBlockBaseData->GetPacketList();
	if(pPacketList == NULL)
		return;

	POSITION pos = m_subGraphPacketList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = m_subGraphPacketList.GetNext(pos);
		if(pPacket == NULL)
			continue;
		pPacketList->DeleteOrClearPacket(pPacket, false, bIsOnlyNoTRData);
	}
	m_subGraphPacketList.RemoveAll();
}

// ---------------------------------------------------------------------------------
void CSubGraph::SetSubGraphList(CSubGraphList* pSubGraphList)
{
	m_pSubGraphList = pSubGraphList;
}

void CSubGraph::SetSubGraphData(const CSubGraphData& sourceSubGraphData, const bool bIsAllChange)
{
	if(bIsAllChange)
		m_subGraphData = sourceSubGraphData;
	else
		m_subGraphData.ChangeKeyPart(sourceSubGraphData);
}

bool CSubGraph::SetHiding(const bool bIsHiding)
{
	if(m_subGraphData.GetHiding() == bIsHiding)
		return false;
	m_subGraphData.SetHiding( bIsHiding);

//	SendMessage_SubGraph(true);
	CalculateGraph();

	// (2007/1/19 - Seung-Won, Bae) Activate Vertical Scale Group.
	CGraphImp *pGraph = GetGraph();
	if( pGraph)
	{
		CVertScaleGroup *pVertScaleGroup = pGraph->GetVertScaleGroup();
		CVertScaleGroupList *pVertScaleGroupList = pVertScaleGroup->GetVertScaleGroupList();
		if( pVertScaleGroupList) pVertScaleGroupList->SetActivate( pVertScaleGroup);
	}

	return true;
}

// (2007/2/28 - Seung-Won, Bae) Support Graph Hiding
BOOL CSubGraph::GetHiding( void) const
{
	BOOL bHiding = m_subGraphData.GetHiding();
	if( bHiding) return TRUE;
	CGraphImp *pGraph = GetGraph();
	if( !pGraph) return TRUE;
	return pGraph->IsGraphHide();
}

// ---------------------------------------------------------------------------------
// 부모 주소
void CSubGraph::SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData)
{
	m_pMainBlockBaseData = pMainBlockBaseData;
}

CGraphImp* CSubGraph::GetGraph() const
{
	if(m_pSubGraphList == NULL)
		return NULL;

	return m_pSubGraphList->GetGraph();
}

CSubGraphList* CSubGraph::GetSubGraphList() const
{
	return m_pSubGraphList;
}

const CSubGraphData& CSubGraph::GetSubGraphData() const
{
	return m_subGraphData;
}

CSubGraphData& CSubGraph::GetSubGraphData()
{
	return m_subGraphData;
}

CSubGraphData* CSubGraph::GetSubGraphData_PointerType()
{
	return &m_subGraphData;
}

CSubGraphPacketList& CSubGraph::GetSubGraphPacketList()
{
	return m_subGraphPacketList;
}

// subGraph의 data
void CSubGraph::GetSubGraphStringData( CString &p_strSubGraphData) const
{
	m_subGraphData.GetStringData( p_strSubGraphData);
}

// ---------------------------------------------------------------------------------
// subGraph title region
CRect CSubGraph::GetSubGraphTitleRegion(CDC* pDC, const CRect& allTitleRegion, int& preTitleEndPosition) const
{
	if(m_subGraphData.GetHiding() || m_subGraphData.GetTitle().IsEmpty())
		return CRect(0, 0, 0, 0);

	CRect titleRegion;
	titleRegion.left = allTitleRegion.left + preTitleEndPosition + MARGIN;
	titleRegion.top = allTitleRegion.top + MARGIN;
	titleRegion.right = allTitleRegion.left + m_subGraphData.GetTitleEndPosition();
	titleRegion.bottom = allTitleRegion.bottom;

	preTitleEndPosition = m_subGraphData.GetTitleEndPosition();

	return titleRegion;
}

// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
CRect CSubGraph::GetSubGraphTitleRegion(CDC* pDC, const CRect& allTitleRegion, int& preTitleEndPosition, int& nPreLineCount) const
{
	if(m_subGraphData.GetHiding() || m_subGraphData.GetTitle().IsEmpty())
		return CRect(0, 0, 0, 0);

	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	int nLine = m_subGraphData.GetTitleEndLine();
	int nLeft = (nLine > nPreLineCount) ? VSGL_TITLE_START_POSITION : preTitleEndPosition;

	CRect titleRegion;
	titleRegion.left = allTitleRegion.left + nLeft + MARGIN;
	titleRegion.top = allTitleRegion.top + MARGIN;
	titleRegion.right = allTitleRegion.left + m_subGraphData.GetTitleEndPosition();
	titleRegion.bottom = allTitleRegion.bottom;

	if(nLine > 1)
	{
		nLeft = (titleRegion.Height() + MARGIN) * (nLine - 1);
		titleRegion.OffsetRect(0, nLeft);
	}

	preTitleEndPosition = m_subGraphData.GetTitleEndPosition();
	nPreLineCount = nLine;

	return titleRegion;
}

// ---------------------------------------------------------------------------------
// view data setting
void CSubGraph::GetSubGraphViewDataList(const CString& GraphName, const CString& packetName, CSubGraphViewDataList& subGraphViewDataList) const
{
	if(m_subGraphData.GetHiding() || m_subGraphData.GetType() == CGraphBaseData::DataView_Type)
		return;
	
	CSubGraphViewData subGraphViewData = GetSubGraphViewData(GraphName, packetName);
	if(IsAddTail_SubGraphViewData(subGraphViewData.GetPacketNames(), subGraphViewDataList)){
		subGraphViewDataList.AddTail(subGraphViewData);
	}
}

// ---------------------------------------------------------------------------------------
// packet과 관련된 내용
bool CSubGraph::GetHeadPacketAndDataType(CPacket*& pPacket, double& dDataType) const
{
	pPacket = m_subGraphPacketList.GetHeadPacket();
	if(pPacket == NULL)
		return false;

	// (2007/2/24 - Seung-Won, Bae) Support ScaleType
	dDataType = pPacket->GetScaleTypeFormatDouble();
	return true;
}

bool CSubGraph::GetSubGraphPacketList(CList<CPacket*, CPacket*>& packetList) const
{
	bool bResult = false;
	POSITION pos = m_subGraphPacketList.GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = m_subGraphPacketList.GetNext(pos);
		if(pPacket == NULL)
			continue;

		packetList.AddTail(pPacket);
		bResult = true;
	}

	return bResult;
}

// ---------------------------------------------------------------------------------------
// point가 subGraph에 속하는지의 여부.
int CSubGraph::PointInSubGraph(const CPoint& point) const
{
	if(!IsPointInSubGraph(point))
		return -1;

	CSelectSubGraphData selectSubGraphData;
	if(!GetSelectSubGraphData(selectSubGraphData))
		return -1;

	return CGraphDrawer().PointInSubGraph(point, selectSubGraphData, NULL);
}

// ---------------------------------------------------------------------------------
// 같은 packet이 존재하는지 확인한다.
bool CSubGraph::IsExistenceSamePacketInGraph(CGraphImp* pTargetGraph) const
{
	if(pTargetGraph == NULL)
		return false;

	POSITION pos = m_subGraphPacketList.GetHeadPosition();
	while(pos != NULL)
	{
		CPacket* pPacket = m_subGraphPacketList.GetNext(pos);
		assert(pPacket != NULL);
		if(pTargetGraph->GetSubGraphList().IsExistenceSamePacket(pPacket))
			return true;
	}

	return false;
}

// ---------------------------------------------------------------------------------
// 세로 scale width 구하기.
int CSubGraph::GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData)
{
	if(m_subGraphData.GetHiding())
		return 0;

	pCalData->SetSubGraphName(m_subGraphData.GetName());
	pCalData->SetSubGraphPacketList(const_cast<CSubGraphPacketList*>(&m_subGraphPacketList));

	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		It is initialized in CSubGraph::GetDisplayDataMinMax() for CMainBlockImp::CalculateDataForDrawing().
	pCalData->SetSubGraphDisplayMinMax( m_displayDataMinMax);
	
	int nWidth = CTextSizeCalculator( m_pMainBlockBaseData->GetParent_Handle(), m_pMainBlockBaseData->GetMainAttributes().GetChartMode()).GetMaxVertScaleTextSize(pDC, *pCalData).cx;
	if(nWidth > 0)
	{
		if(m_subGraphData.GetName() == _MTEXT( C3_PRICE))
		{
			if(m_nVerticalScaleMaxWidth < nWidth)
				m_nVerticalScaleMaxWidth = nWidth;
			else if(m_nVerticalScaleMaxWidth > nWidth)
				nWidth = m_nVerticalScaleMaxWidth;
		}

		return (nWidth + MARGIN * 3);
	}

	// scale 영역을 못 구했을 경우에는 최소값으로 셋팅한다.
	return m_pMainBlockBaseData->GetScaleAttributes().GetMinVertScaleRegionWidth();
}

// 현재 보이는 화면에서의 min, max값 구한다. ==> y축의 값
bool CSubGraph::GetDisplayDataMinMax(CVertDisplayDataMinMaxInputData* pInputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange)
{
	// dataview 형태나 "대기매물"일 경우에는 구하지 않는다.
	if(!IsDisplayDataMinMax())
		return false;

	pInputData->SetSubGraphData(const_cast<CSubGraphData*>(&m_subGraphData));
	pInputData->SetSubGraphPacketList(const_cast<CSubGraphPacketList*>(&m_subGraphPacketList));
	
//	CPacket* lpPacket = m_subGraphPacketList.GetHeadPacket();

	// yiilyoul@magicn.com - XScaleManager
	CXScaleManager* pxScaleManager = GetGraph()->GetMainBlock()->GetXScaleManager();
	CPacketRQ *pPacketRQ = this->GetGraph()->GetPacketRQ();
	// (2009/9/20 - Seung-Won, Bae) on deletion of graph, It can be NULL.
	if( !pPacketRQ) return false;
	CString strRQ = pPacketRQ->GetRQ();

	int nDRI_Start, nDRI_End;
	int nDTI_Start, nDTI_End;

	nDRI_Start = pInputData->GetDisplayAttributes().GetDataStartInBlock();
	nDRI_End = pInputData->GetDisplayAttributes().GetDataEndInBlock();

	CIndicatorInfo* pIndicatorInfo = this->GetGraph()->GetIndicatorInfo();

	if(pIndicatorInfo != NULL)
	{
		BOOL bOEBT = (pIndicatorInfo->GetIndicatorName() == _MTEXT( C2_OBVIOUSLY_BALANCE_CHART));
		pxScaleManager->GetDrawIndex(strRQ, FALSE, bOEBT, nDTI_Start, nDTI_End);
	}
	else
	{
		nDTI_Start = pxScaleManager->GetDTIFromDRI_AbleRange(strRQ, nDRI_Start, NULL);
		nDTI_End = pxScaleManager->GetDTIFromDRI_AbleRange(strRQ, nDRI_End, NULL);
	}

	// 앞쪽에 보이지 않는 데이터도 그려지도록 해야 정확한 정보가 그려진다.
	pInputData->SetDataStartEndInBlock(nDTI_Start, nDTI_End);

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool bResult = CViewMinMaxCalculator().GetViewVertMinMax(*pInputData, displayDataMinMax, p_bWithFullRange, m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl()->GetDrawBaseLine());

	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		It is initialized in CSubGraph::GetDisplayDataMinMax() for CMainBlockImp::CalculateDataForDrawing().
	m_displayDataMinMax = displayDataMinMax;

	pInputData->SetDataStartEndInBlock(nDRI_Start, nDRI_End);

	return bResult;
}

// ---------------------------------------------------------------------------------
// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
bool CSubGraph::DrawSubGraphTitle(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, const int& nSubGraphIndex)
{
	if(m_subGraphData.GetHiding())
		return false;

	pDrawingGraphData->SetSubGraphPacketList(&m_subGraphPacketList);
	
	// CPacketListManager 추가 : 복수종목 - ojtaso (20070705)
	CPacketListManager* lpPacketListManager = GetGraph()->GetMainBlock()->GetPacketListManager();
//	// 선택된 그래프의 타이틀 표시 : 복수종목 - ojtaso (20070705)
//	if(lpPacketListManager->GetPacketListCount() > 1)
//	{
//		if( !strcmp( this->GetGraph()->GetGraphRQ(), lpPacketListManager->GetCurrentRQ()))
//		{
//			CBlockImp* pBlock = GetBlock();
//			CRect region = pBlock->GetVertScaleGroupList().GetSubGraphTitleRegion(pDC, this);
//			
//			pDC->MoveTo(region.left, region.bottom - 1);
//			pDC->LineTo(region.right, region.bottom - 1);
//			pDC->LineTo(region.right, region.top);
//		}
//	}
	
	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	m_subGraphData.SetSubGraphIndex( nSubGraphIndex);

	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	bool bResult = CGraphDrawer().DrawGraphTitle(pDC, *pDrawingGraphData, m_subGraphData, graphTitlePosition, graphTitleLine);
	if(bResult)
	{
		m_subGraphData.SetTitleEndPosition( graphTitlePosition, graphTitleLine);

		// 선택된 그래프의 타이틀 표시 : 복수종목 - ojtaso (20070705)
		if(lpPacketListManager->GetPacketListCount() > 1)
		{
			if( !strcmp( this->GetGraph()->GetGraphRQ(), m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->GetCurrentRQ()))
			{
				CBlockImp* pBlock = GetBlock();
				CRect region = pBlock->GetVertScaleGroupList().GetSubGraphTitleRegion(pDC, this);
				
				pDC->MoveTo(region.left, region.bottom - 1);
				pDC->LineTo(region.right, region.bottom - 1);
				pDC->LineTo(region.right, region.top);
			}
		}
	}
	return bResult;
}

// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
bool CSubGraph::DrawSubGraph(CDC* pDC, CDrawingGraphData* pDrawingGraphData)
{
	if(m_subGraphData.GetHiding())
		return false;

	pDrawingGraphData->SetSubGraphPacketList(&m_subGraphPacketList);
	
	// yiilyoul@magicn.com - XScaleManager
	CString strRQ = this->GetGraph()->GetGraphRQ();
	CXScaleManager* pxScaleManager = GetGraph()->GetMainBlock()->GetXScaleManager();
	
	return CGraphDrawer().DrawGraph(pDC, *pDrawingGraphData, m_subGraphData, strRQ, pxScaleManager);
}

void CSubGraph::DrawGripedSubGraphTitle(CDC* pDC)
{
	CBlockImp* pBlock = GetBlock();
	if(pBlock == NULL)
		return;

	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);
	int oldDrawMode = pDC->SetROP2(R2_NOT);

	CRect region = pBlock->GetVertScaleGroupList().GetSubGraphTitleRegion(pDC, this);
	region.InflateRect(2, 2);
	region.bottom -= 1;
	pDC->Rectangle(region);

	pDC->SetROP2(oldDrawMode);
	pDC->SelectObject(pOldBrush);
}

bool CSubGraph::DrawGrip(CDC* pDC, const int nDataIndex)
{
	if(nDataIndex < -1 || m_subGraphData.GetHiding())
		return false;

	CSelectSubGraphData selectSubGraphData;
	if(!GetSelectSubGraphData(selectSubGraphData))
		return false;

	CDataConversion::SelectClipRegion(pDC, selectSubGraphData.GetGraphRegions().GetNoFrameRegionAndMargin());
	bool bResult = CGraphDrawer().DrawGrip(pDC, nDataIndex, selectSubGraphData);
	CDataConversion::SelectClipRegion(pDC);
	if(!bResult)
		return false;

	DrawGripedSubGraphTitle(pDC);
	return true;
}


// private =========================================================================
// 지표를 계산한다.
void CSubGraph::CalculateGraph()
{
	// "보임"이고 packet* = null 닌 경우 공식 호출
	if(m_subGraphData.GetHiding() || m_subGraphPacketList.GetHeadPacket() != NULL)
		return;

	CGraphImp* pGraph = GetGraph();
	if(pGraph != NULL)
		pGraph->Initialize_AllPackets(true);
}

// ---------------------------------------------------------------------------------
// min/max 구할지의 여부 (graph단계가 아닌 subGraph단계에서의 검증)
// 구하지 않는 경우1 : hide
// 구하지 않는 경우2 : subGraphName = <대기매물>
// 구하지 않는 경우3 : type = "데이터 표현형"(DataView_Type)
// 구하지 않는 경우4 : style = "수평식"(HorizontalLine), "수직식"(VerticalLine)
bool CSubGraph::IsDisplayDataMinMax() const
{
	if(m_subGraphData.GetHiding())
		return false;

	if(m_subGraphData.GetType() == CGraphBaseData::DataView_Type)
		return false;

	if(m_subGraphData.GetType() == CGraphBaseData::Line_Type)
	{
		CGraphBaseData::LINESTYLE eLineStyle = m_subGraphData.GetStyle().GetGraphLineStyle();
		return (eLineStyle != CGraphBaseData::HorizontalLine && eLineStyle != CGraphBaseData::VerticalLine);
	}

	return true;
}

// view data list에 추가할지 결정 --> 내용이 이미 있을 경우엔 추가 안함
bool CSubGraph::IsAddTail_SubGraphViewData(const CString& packetName, CSubGraphViewDataList& subGraphViewDataList) const
{ 
	// RQ검사추가 : 복수종목 - ojtaso (20070701)
	CString strRQ = GetGraph()->GetGraphRQ();
	for(int nIndex = 0; nIndex < subGraphViewDataList.GetCount(); nIndex++){
		CSubGraphViewData subGraphViewData = subGraphViewDataList.GetAt(nIndex);
		if(strRQ == subGraphViewData.GetRQ() && subGraphViewData.GetTitle() == m_subGraphData.GetTitle() && subGraphViewData.GetPacketNames() == packetName)
			return false;
	}
	return true;
}

bool CSubGraph::IsPointInSubGraph(const CPoint& point) const
{
	// graph 숨김일땐 찾지 않는다!!
	if(m_subGraphData.GetHiding())
		return false;

	CVertScaleGroupList* pVertScaleGroupList = GetVertScaleGroupList();
	if(pVertScaleGroupList == NULL)
		return false;

	CRect drawRegion = pVertScaleGroupList->GetDrawingRegion().GetGraphRegions().GetDrawingRegion(m_subGraphData.GetType(), m_subGraphData.GetStyle());
	return drawRegion.PtInRect(point) ? true : false;
}

// ---------------------------------------------------------------------------------
// subGraph 를 select에 관한 data 가져오기.
bool CSubGraph::GetSelectSubGraphData(CSelectSubGraphData& selectSubGraphData) const
{
	// type / style / draw style
	selectSubGraphData.SetSubGraphType(m_subGraphData.GetType());
	selectSubGraphData.SetSubGraphStyle(m_subGraphData.GetStyle());
	selectSubGraphData.SetSubGraphDrawStyle(m_subGraphData.GetDrawStyle());
	// vertScaleGroup에서 가져올 data
	if(!GetSelectSubGraphData_InVertScaleGroupData(selectSubGraphData))
		return false;
	// block에서 가져올 data
	if(!GetSelectSubGraphData_InBlock(selectSubGraphData))
		return false;
	// 기타 data
	return GetSelectSubGraphData_Etc(selectSubGraphData);
}

// vertScaleGroup에서 가져올 data
bool CSubGraph::GetSelectSubGraphData_InVertScaleGroupData(CSelectSubGraphData& selectSubGraphData) const
{
	CVertScaleGroup* pVertScaleGroup = GetVertScaleGroup(GetGraph());
	if(pVertScaleGroup == NULL)
		return false;

	// 순수 graph 그리는 영역.
	CVertScaleGroupList* pVertScaleGroupList = pVertScaleGroup->GetVertScaleGroupList();
	assert(pVertScaleGroupList != NULL);
	selectSubGraphData.SetGraphRegions(pVertScaleGroupList->GetGraphRegions());
	// vert scale min/max
	selectSubGraphData.SetDisplayDataVertMinMax(pVertScaleGroup->GetDisplayDataMinMax());
	return true;
}

// block에서 가져올 data
bool CSubGraph::GetSelectSubGraphData_InBlock(CSelectSubGraphData& selectSubGraphData) const
{
	if(m_pMainBlockBaseData == NULL)
		return false;

	CBlockImp* pBlock = GetBlock();
	if(pBlock == NULL)
		return false;

	// draw order
	selectSubGraphData.SetDrawingOrderType(m_pMainBlockBaseData->GetViewAttributes().GetDrawingOrderType());
	// log
	selectSubGraphData.SetVertScaleLogType(pBlock->GetBlockData().GetVertScaleLogType());
	// invert
	// 지표 뒤집기 - ojtaso (20071023)
	selectSubGraphData.SetVertScaleInvertType(GetVertScaleGroup(GetGraph())->GetVertScaleInvertType());
	// hors scale min/max
	selectSubGraphData.SetDisplayDataHorzMinMax(pBlock->GetViewHorzMinMax());
	return true;
}

// 기타 data
bool CSubGraph::GetSelectSubGraphData_Etc(CSelectSubGraphData& selectSubGraphData) const
{
	if(m_pMainBlockBaseData == NULL)
		return false;

	CGraphImp* pGraph = GetGraph();
	if(pGraph == NULL)
		return false;

	// CIndicatorInfo
	selectSubGraphData.SetIndicatorInfo(pGraph->GetIndicatorInfo());
	// 함수명
	selectSubGraphData.SetGraphName(		pGraph->GetGraphData().GetGraphName());
	selectSubGraphData.SetIndicatorName(	pGraph->GetGraphData().GetIndicatorName());
	// 화면에 보이는 start/end
	selectSubGraphData.SetDisplayAttributes(m_pMainBlockBaseData->GetDisplayAttributes());
	// 사용할 packet 들.
	selectSubGraphData.SetSubGraphPacketList(const_cast<CSubGraphPacketList*>(&m_subGraphPacketList));
	// XScale 정보 Set.
	selectSubGraphData.SetXScaleManager(m_pMainBlockBaseData->GetMainAttributes().GetXScaleManager());
	// RQ 정보 Set
	CString strRQ = GetGraph()->GetGraphRQ();
	selectSubGraphData.SetRQ(strRQ);

	CPacket* pPacket = m_subGraphPacketList.GetHeadPacket();
	return (pPacket != NULL && pPacket->GetCount() > 0);
}

// ---------------------------------------------------------------------------------
// subGraphViewData 가져오기.
CSubGraphViewData CSubGraph::GetSubGraphViewData(const CString& GraphName, const CString& packetName) const
{
	CString packetNames;
	POSITION pos = m_subGraphPacketList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = m_subGraphPacketList.GetNext(pos);
		assert(pPacket != NULL);
		packetNames += (pPacket->GetName() + ',');
	}

	return CSubGraphViewData(m_subGraphData.GetType(), m_subGraphData.GetStyle(), m_subGraphData.GetTitle(), packetNames, GetGraph()->GetGraphRQ());
}

// ---------------------------------------------------------------------------------
// 해당 block을 가져오기.
CBlockImp* CSubGraph::GetBlock() const
{
	CVertScaleGroup* pVertScaleGroup = GetVertScaleGroup(GetGraph());
	if(pVertScaleGroup == NULL)
		return NULL;

	return pVertScaleGroup->GetVertScaleGroupList()->GetBlock();
}

CVertScaleGroupList* CSubGraph::GetVertScaleGroupList() const
{
	CGraphImp* pGraph = GetGraph();
	if(pGraph == NULL)
		return false;

	return pGraph->GetVertScaleGroupList();
}

CVertScaleGroup* CSubGraph::GetVertScaleGroup(CGraphImp* pGraph) const
{
	if(pGraph == NULL)
		return NULL;

	return pGraph->GetVertScaleGroup();
}

/*
void CSubGraph::SendMessage_SubGraph(const bool bIsMaking)
{
	if(m_pMainBlockBaseData == NULL)
		return;

	if(m_subGraphData.GetName() != _MTEXT( C3_PRESPAN))
		return; 

	bool bIsAddMargin = false;
	if(bIsMaking && !m_subGraphData.GetHiding())
		bIsAddMargin = true;

	m_pMainBlockBaseData->SendMessageToParent( _MTEXT( C2_OBVIOUSLY_BALANCE_CHART), bIsAddMargin);
}
*/


void CSubGraph::ResetMaxVerticalScaleWidth()
{
	m_nVerticalScaleMaxWidth = -1;
}