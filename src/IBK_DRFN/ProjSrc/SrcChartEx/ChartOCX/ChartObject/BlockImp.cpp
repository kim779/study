// BlockImp.cpp: implementation of the CBlockImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlockImp.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/PacketList.h"			// PacketData.dll
#include "../Include_Chart/Dll_Load/GraphFrameDrawer.h"		// graph frame 을 그린다.// ViewDraw.dll
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/Dll_Load/PacketListManager.h"	// CPacketListManager
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "MainBlockImp.h"
#include "BlocksColumn.h"
#include "GraphImp.h"
#include "CalculatorData.h"
#include "ViewMinMaxCalculator.h"
#include "DrawingRegionCalculator.h"
#include "ScaleAndGraphDrawer.h"
#include "VertScaleGroup.h"				// for CVertScaleGroup
#include "GraphSearchInfo.h"			// for CGraphSearchInfo_IndicatorName
#include "SubGraph.h"					// for CSubGraph
#include "PacketRQ.h"					// for CPacketRQ
#include "XScaleManagerImp.h"				// for CXScaleManager

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

// public =====================================================================
CBlockImp::CBlockImp(CMainBlockBaseData* pMainBlockBaseData, CBlocksColumn* pBlocksColumn):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pBlocksColumn(pBlocksColumn),
	m_vertScaleGroupList( pMainBlockBaseData)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	m_bOnDestructor = FALSE;

	m_vertScaleGroupList.SetBlock( this);
}

///////////////////////////////////////////////////////////////////////////////
// Create
///////////////////////////////////////////////////////////////////////////////
	
// 객체(block)를 생성한다.
CBlockImp* CBlockImp::Make(CMainBlockBaseData* pMainBlockBaseData, CBlocksColumn* pBlocksColumn)
{
	return new CBlockImp( pMainBlockBaseData, pBlocksColumn);
}

// block에 Graph들을 생성한다.
// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
void CBlockImp::BuildGraphsAndScales( char *p_szBlockData, BOOL& bCalc)
{
	if( !p_szBlockData) return;
	if( !*p_szBlockData) return;

	char *szVertScaleGroupData = strstr( p_szBlockData, "\r\n");
	if( !szVertScaleGroupData) return;
	*szVertScaleGroupData = '\0';

	// block data setting.
	// 가로 scale
	// 사용할 Data;위치;grid타입;일자타입;시간타입;text타입;
	int nColonCount = 0;
	char *szLogType = p_szBlockData;
	while( szLogType && nColonCount < 6) if( *szLogType++ == ';') nColonCount++;

	BOOL bInvertType = FALSE;
	if( szLogType) *( szLogType - 1) = '\0';
	m_blockData.GetHorzScale().SetScaleData( p_szBlockData, m_pMainBlockBaseData->GetParent_Handle());
	if( szLogType)
	{
		*( szLogType - 1) = ';';

		// block data
		// log;차트뒤집기;2배block여부;
		char *szReverse = strchr( szLogType, ';');
		if( szReverse) *szReverse = '\0';
		m_blockData.SetVertScaleLogType( atoi( szLogType) != 0);
		if( szReverse)
		{
			*szReverse++ = ';';

			char *szDoubleSize = strchr( szReverse, ';');
			if( szDoubleSize) *szDoubleSize = '\0';
			bInvertType = ( atoi( szReverse) != 0);
			if( szDoubleSize)
			{
				*szDoubleSize++ = ';';

				// (2006/10/25 - Seung-Won, Bae) Double Size Data is not supported in Map CL(ChartList) Setting Value.
				//		Because, In Map Setting, Double Size Data is in the other property value.
				if( *szDoubleSize)
				{
					char *szEnd = strchr( szDoubleSize, ';');
					if( szEnd) *szEnd = '\0';
					m_blockData.SetDoubleSize( atoi( szDoubleSize) != 0);
					if( szEnd) *szEnd++ = ';';
				}
			}
		}
	}

	*szVertScaleGroupData = '\r';
	szVertScaleGroupData += 2;		// "\r\n"

	// block에 있는 VertScaleGroup들 생성.
	m_vertScaleGroupList.BuildVertScaleGroups( szVertScaleGroupData);
	// (2008/2/26 - Seung-Won, Bae) Set only true for Old Map Conversion. This CL Field is not used any more.
	if( bInvertType) m_vertScaleGroupList.SetAllVertScaleInvertType( TRUE);

	// Packet Name 다시 세팅 : 복수종목 - ojtaso (20070531)
//	CGraphSearchInfo_VisibleSubGraph gsi;
	// 종목중첩일때 특수차트 전환 버그 수정 - ojtaso (20090108)
	CPacketListManager* pPacketListManager = m_pMainBlockBaseData->GetPacketListManager();
	
	CGraphSearchInfo_AnyGraphWithRQ gsi;
	gsi.m_strRQ = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->GetCurrentRQ();
	if( m_vertScaleGroupList.FindGraph( &gsi))
	{
		CPacketRQ* lpPacketRQ = gsi.m_pGraphImp->GetPacketRQ();

		if(lpPacketRQ)
		{
			lpPacketRQ->SetPacketName(m_blockData.GetHorzScale().GetScaleData());
			bCalc = TRUE;
		}
	}
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
void CBlockImp::BuildGraphsAndScales(LPVOID pPacketRQ, char *p_szBlockData)
{
	if( !p_szBlockData) return;
	if( !*p_szBlockData) return;

	char *szVertScaleGroupData = strstr( p_szBlockData, "\r\n");
	if( !szVertScaleGroupData) return;
	*szVertScaleGroupData = '\0';

	// block data setting.
	// 가로 scale
	// 사용할 Data;위치;grid타입;일자타입;시간타입;text타입;
	int nColonCount = 0;
	char *szLogType = p_szBlockData;
	while( szLogType && nColonCount < 6) if( *szLogType++ == ';') nColonCount++;

	if( szLogType) *( szLogType - 1) = '\0';
	m_blockData.GetHorzScale().SetScaleData( p_szBlockData, m_pMainBlockBaseData->GetParent_Handle());
	BOOL bInvertType = FALSE;
	if( szLogType)
	{
		*( szLogType - 1) = ';';

		// block data
		// log;차트뒤집기;2배block여부;
		char *szReverse = strchr( szLogType, ';');
		if( szReverse) *szReverse = '\0';
		m_blockData.SetVertScaleLogType( atoi( szLogType) != 0);
		if( szReverse)
		{
			*szReverse++ = ';';

			char *szDoubleSize = strchr( szReverse, ';');
			if( szDoubleSize) *szDoubleSize = '\0';
			bInvertType = ( atoi( szReverse) != 0);
			if( szDoubleSize)
			{
				*szDoubleSize++ = ';';

				// (2006/10/25 - Seung-Won, Bae) Double Size Data is not supported in Map CL(ChartList) Setting Value.
				//		Because, In Map Setting, Double Size Data is in the other property value.
				if( *szDoubleSize)
				{
					char *szEnd = strchr( szDoubleSize, ';');
					if( szEnd) *szEnd = '\0';
					m_blockData.SetDoubleSize( atoi( szDoubleSize) != 0);
					if( szEnd) *szEnd++ = ';';
				}
			}
		}
	}

	*szVertScaleGroupData = '\r';
	szVertScaleGroupData += 2;		// "\r\n"

	// block에 있는 VertScaleGroup들 생성.
	m_vertScaleGroupList.BuildVertScaleGroups(pPacketRQ, szVertScaleGroupData);
	// (2008/2/26 - Seung-Won, Bae) Set only true for Old Map Conversion. This CL Field is not used any more.
	if( bInvertType) m_vertScaleGroupList.SetAllVertScaleInvertType( TRUE);
}

// block의 모든 data
void CBlockImp::GetBlockAllData( CStringList &p_slBlockData) const
{
	//>> JS.Kim_20100929  solomon.. PAIR RQ는 저장하지 않도록 수정..
	/*
	p_slBlockData.AddTail( "Block;\r\n");

	p_slBlockData.AddTail( CString());
	CString &strHorzScaleData = p_slBlockData.GetTail();
	m_blockData.GetHorzScale().GetScaleAllData( strHorzScaleData);

	p_slBlockData.AddTail( m_blockData.GetVertScaleLogType() ? '1' : '0');
	p_slBlockData.AddTail( ";");
	// (2008/2/26 - Seung-Won, Bae) Set false. This CL Field is not used any more.
	p_slBlockData.AddTail( '0');
	p_slBlockData.AddTail( ";");
	p_slBlockData.AddTail( m_blockData.GetDoubleSize() ? '1' : '0');
	p_slBlockData.AddTail( ";\r\n");

	m_vertScaleGroupList.GetVertScaleGroupsAllData( p_slBlockData);

	p_slBlockData.AddTail( "BlockEnd;\r\n");
	*/
	CStringList slBlockData;
	m_vertScaleGroupList.GetVertScaleGroupsAllData( slBlockData);
	if( slBlockData.IsEmpty() )		
	{
		return;
	}

	p_slBlockData.AddTail( "Block;\r\n");

	p_slBlockData.AddTail( CString());
	CString &strHorzScaleData = p_slBlockData.GetTail();
	m_blockData.GetHorzScale().GetScaleAllData( strHorzScaleData);

	p_slBlockData.AddTail( m_blockData.GetVertScaleLogType() ? '1' : '0');
	p_slBlockData.AddTail( ";");
	// (2008/2/26 - Seung-Won, Bae) Set false. This CL Field is not used any more.
	p_slBlockData.AddTail( '0');
	p_slBlockData.AddTail( ";");
	p_slBlockData.AddTail( m_blockData.GetDoubleSize() ? '1' : '0');
	p_slBlockData.AddTail( ";\r\n");

	p_slBlockData.AddTail(&slBlockData);

	p_slBlockData.AddTail( "BlockEnd;\r\n");
}

///////////////////////////////////////////////////////////////////////////////
// Delete
///////////////////////////////////////////////////////////////////////////////

// 객체를 삭제한다. Make에서 new를 했으므로..
void CBlockImp::Delete(CBlockImp* pBlock)
{
	// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
	if( !pBlock->m_bOnDestructor) delete pBlock;
}

void CBlockImp::DeleteFromParent()
{
	if(m_pBlocksColumn == NULL)
	{
		// ocx에 메시지 보내기
		CMainBlockImp* pMainBlock = GetMainBlock();
		if( pMainBlock) pMainBlock->GetMainBlockBaseData()->SendMessageToParent(CBlockBehaviorData(CBlockBaseData::BLOCK_DELETE, pMainBlock->FindBlockNumber(this)));
		// block 삭제
		CBlockImp::Delete( this);
		return;
	}

	CMainBlockImp *pMainBlockImp = GetMainBlock();
	if( pMainBlockImp) pMainBlockImp->DeleteBlock(this);
}

// (2006/8/8 - Seung-Won, Bae) Remove Graph
BOOL CBlockImp::DeleteGraph( int p_nV, int p_nG)
{
	CVertScaleGroup *pVertScaleGroup = m_vertScaleGroupList.GetVertScaleGroup( p_nV);
	if( !pVertScaleGroup) return FALSE;
	CGraphImp *pGraph = pVertScaleGroup->GetGraph( p_nG);
	if( !pGraph) return FALSE;
	pVertScaleGroup->DeleteGraphWithNotify( pGraph);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Draw
//////////////////////////////////////////////////////////////////////

// -> Block의 바탕만을 그리는 부분.
void CBlockImp::DrawBlockBackground(CDC* pDC, const CRect& blockRegion, const int interBlockHalfGap)
{
	//sy test
	CDataConversion::SelectClipRegion(pDC, blockRegion);

	// graph와 scale 그린다.
	if( !m_pMainBlockBaseData) return;
	// graph frame 그린다.
	CGraphFrameDrawer().DrawGraphFrame(pDC, m_vertScaleGroupList.GetDrawingRegion().GetGraphFrameRegion(), 
				m_pMainBlockBaseData->GetBlockCompartType(), interBlockHalfGap, 
				m_pMainBlockBaseData->GetGraphRegionFrameThickness(), 
				m_pMainBlockBaseData->GetGraphAttributes().GetGraphBackgroundGapX(), 
				m_pMainBlockBaseData->GetGraphAttributes().GetGraphBackgroundGapY(),  
				m_pMainBlockBaseData->GetGraphRegionColor());

	CDataConversion::SelectClipRegion(pDC);
}

// block HorzScale, VertScale을 그린다.
void CBlockImp::DrawHorzAndVertScale(CDC* pDC, const CRect& blockRegion)
{
	CGraphBaseData::GRAPHTYPE eGraphType = CGraphBaseData::GTNONE;
	CGraphStyle graphStyle;

	CSubGraph *pSubGraph = m_vertScaleGroupList.GetSubGraph( 0, 0, 0);
	if( pSubGraph)
	{
		CSubGraphData &subGraphData = pSubGraph->GetSubGraphData();
		eGraphType = subGraphData.GetType();
		graphStyle = subGraphData.GetStyle();
	}
	if( !m_pBlocksColumn) return;

	CDrawingRegion drawingRegion = m_vertScaleGroupList.GetDrawingRegion();
	// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
	CMainBlockImp *pMainBlock = GetMainBlock();
	CDrawingHorzScaleData drawingHorzScaleData( drawingRegion.GetGraphRegions().GetNoFrameRegionAndMargin(), eGraphType, graphStyle, 
		m_pMainBlockBaseData->GetGraphRegionColor(), m_pMainBlockBaseData->GetScaleAttributes().GetScaleColor(),
		m_pMainBlockBaseData->GetDisplayAttributes(), m_blockData.GetDisplayDataHorzMinMax(),
		GetHorzScaleDataPacket(), pMainBlock->GetDrawFutureScale(), m_pBlocksColumn->GetColumnIndex());

	// RQ별 Unit정보 관리 : 복수종목 - ojtaso (20070601)
  	CXScaleManager* pxScaleManager = GetMainBlock()->GetXScaleManager();
	CBlockScaleDrawer().DrawHorzScale(pDC, &drawingHorzScaleData, drawingRegion.GetHorzScaleRegion(), m_blockData.GetHorzScale(), pxScaleManager, GetHorzScaleDataRQ(), m_pMainBlockBaseData->GetParent_Handle(), m_pMainBlockBaseData->GetMainAttributes().GetChartMode());

	m_vertScaleGroupList.DrawVertScale(pDC, m_blockData);
}

void CBlockImp::CalculateDrawingRegion(CDC* pDC, const CRect& blockRegion, const CVertScaleRegionWidth& vertScaleRegionWidth)
{
	if( !m_pMainBlockBaseData) return;

	//sy test
	// horz Scale의 Min/Max 구하기
	m_blockData.SetDisplayDataHorzMinMax(GetViewHorzMinMax());

	// graph, scale의 영역을 구한다.
	// 가로 scale 높이
	int nTopHorzScaleRegionHeight = 0;
	int nBottomHorzScaleRegionHeight = 0;
	// scale position = 위쪽, 양쪽일 경우에만.
	if( !m_blockData.GetHorzScale().GetScaleData().IsEmpty())
	{
		CScaleBaseData::HORZSCALEPOSITION eHorzScalePosition = m_blockData.GetHorzScale().GetHorzScalePosition();
		switch( eHorzScalePosition)
		{
			case CScaleBaseData::HORZ_TOP:
			case CScaleBaseData::HORZ_BOTTOM:
			case CScaleBaseData::HORZ_BOTH:
			{
				// XScale 높이 지정 - ojtaso (20080429)
				int nHeight = m_blockData.GetHorizontalScaleHeight();
				int nHorzScaleRegionHeight = 0;
				if(nHeight < 0)
				{
					CHorzScaleMaxWidthCalculatorData calData( &m_blockData.GetHorzScale().GetScaleCompart(), GetHorzScaleDataPacket(),
																m_blockData.GetHorzScale().GetScaleData(), m_pMainBlockBaseData->GetDisplayAttributes());

					nHeight = CTextSizeCalculator( m_pMainBlockBaseData->GetParent_Handle(), m_pMainBlockBaseData->GetMainAttributes().GetChartMode()).GetMaxHorzScaleTextSize(pDC, calData).cy;
					if( nHeight > 0)	nHorzScaleRegionHeight = (nHeight + MARGIN *2);
					else				nHorzScaleRegionHeight = m_pMainBlockBaseData->GetScaleAttributes().GetMinHorzScaleRegionHeight();
				}
				else
					nHorzScaleRegionHeight = nHeight;

				if( eHorzScalePosition != CScaleBaseData::HORZ_BOTTOM)	nTopHorzScaleRegionHeight		= nHorzScaleRegionHeight;
				if( eHorzScalePosition != CScaleBaseData::HORZ_TOP)		nBottomHorzScaleRegionHeight	= nHorzScaleRegionHeight;
			}
		}
	}

	CHorzScaleRegionHeight horzScaleRegionHeight( nTopHorzScaleRegionHeight, nBottomHorzScaleRegionHeight);
	// graph, scale의 영역을 구한다.
	// CDrawingRegion을 구하기 위해 필요한 data 가져오기.
	bool bIsGraphTextDrawCheck = false, bIsGraphTextDraw = false;
	// Text가 그려지는 Graph가 있는지의 여부.
	// -> Text가 있는 경우 : <가격차트>, <Bar_Type : TextBar_Draw>
	// <가격차트>
	CMainBlockImp *pMainBlock = GetMainBlock();
	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
	if( m_vertScaleGroupList.FindGraph( &gsi))
	{
		// Sang-Woo, Cho		2006 / 09 / 26
		// 가격차트에서 최고, 최저값 Text를 보여줄 것인지 생략할 것인지 설정값을 확인한다.
		// Begin
		if( m_pBlocksColumn && pMainBlock)
			bIsGraphTextDrawCheck = bIsGraphTextDraw = ( pMainBlock->GetShowPriceMinMaxText() ? true : false);
	}
	// <Bar_Type : TextBar_Draw>
	else
	{
		CGraphSearchInfo_TypeStyleDrawStyle gsi;
		gsi.m_eGraphType	= CGraphBaseData::Bar_Type;
		gsi.m_nGraphStyle	= CGraphBaseData::FixedRateCombBar;
		gsi.m_nDrawStyle	= CGraphBaseData::TextBar_Draw;
		if( m_vertScaleGroupList.FindGraph( &gsi))
		{
			bIsGraphTextDrawCheck = false;
			bIsGraphTextDraw = true;
		}
	}

	// (2006/10/13 - Seung-Won, Bae) Set Block Move Delete Flag
	BOOL bBlockMoveDelete = FALSE;
	if( pMainBlock) bBlockMoveDelete = pMainBlock->GetBlockMoveDeleteFlag();

	// Graph Title 그릴 여부.
	// -> MainBlock의 CMainBlockBaseData의 "bIsAllTitleDrawHiding" 을 우선시 한다.
	bool bIsGraphTitleDrawingGraph = false;
	// graph title 모두 숨기기 : true 일 경우
	if( !m_pMainBlockBaseData->GetGraphAttributes().DoesAllTitleDrawHiding())
	{
		CGraphSearchInfo_VisibleGraphTitle gsi;
		bIsGraphTitleDrawingGraph = ( m_vertScaleGroupList.FindGraph( &gsi) ? true : false);
	}

	CGraphSearchInfo_HasVertScale gsi2;
	gsi2.m_eVertScalePosition = ( CScaleBaseData::VERTSCALEPOSITION)-1;
	bool bIsViewableVertScaleExist = ( m_vertScaleGroupList.FindGraph( &gsi2) ? true : false);

	CDrawingRegionCalData drawingRegionCalData(
							blockRegion, 
							m_pMainBlockBaseData->GetGraphRegionFrameThickness(),
							bIsGraphTextDrawCheck,
							bIsGraphTextDraw, 
							bIsGraphTitleDrawingGraph,
							bIsViewableVertScaleExist,
							horzScaleRegionHeight,
							vertScaleRegionWidth,
							bBlockMoveDelete
							);

	drawingRegionCalData.SetMaxVertScaleRegionWidth(m_pMainBlockBaseData->GetScaleAttributes().GetMaxVertScaleRegionWidth());
	drawingRegionCalData.SetDisplayAttributes(m_pMainBlockBaseData->GetDisplayAttributes());

	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
  	CXScaleManager* pxScaleManager = GetMainBlock()->GetXScaleManager();
	CDrawingRegion drawingRegion = CDrawingRegionCalculator().GetDrawingRegion( pDC, drawingRegionCalData, pxScaleManager->GetReverse());
	m_vertScaleGroupList.CalculateDrawingRegion(pDC, drawingRegion);
}

// block HorzScale, VertScale을 그린다.
void CBlockImp::DrawAllGraphs(CDC* pDC, const CRect& blockRegion)
{
	//sy test
	CDataConversion::SelectClipRegion(pDC, blockRegion);

	m_vertScaleGroupList.DrawGraphs( pDC, m_blockData);

	CDataConversion::SelectClipRegion(pDC);
}

///////////////////////////////////////////////////////////////////////////////
// Query & Update
///////////////////////////////////////////////////////////////////////////////

CMainBlockImp* CBlockImp::GetMainBlock() const
{
	if( !m_pMainBlockBaseData) return NULL;
	return m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl();
}


CBlocksColumn* CBlockImp::GetBlocksColumn()
{
	return m_pBlocksColumn;
}

// block data.
CBlockData &CBlockImp::GetBlockData( void)
{
	return m_blockData;
}
const CBlockData &CBlockImp::GetBlockData( void) const
{
	return m_blockData;
}

// vertScaleGroup의 list을 가져온다.
CVertScaleGroupList& CBlockImp::GetVertScaleGroupList()
{
	return m_vertScaleGroupList;
}
const CVertScaleGroupList& CBlockImp::GetVertScaleGroupList() const
{
	return m_vertScaleGroupList;
}

// (2006/6/15 - Seung-Won, Bae) Retrieve Region Info
BOOL CBlockImp::GetGraphRegions( CRect &p_rctNoFrame, CRect &p_rctNoFrameAndMargin)
{
	CGraphRegions gRegions	= GetVertScaleGroupList().GetDrawingRegion().GetGraphRegions();
	p_rctNoFrame			= gRegions.GetNoFrameRegion();
	p_rctNoFrameAndMargin	= gRegions.GetNoFrameRegionAndMargin();
	return TRUE;
}
BOOL CBlockImp::GetHorzScaleRegion( CRect &p_rctTopScale, CRect &p_rctBottomScale)
{
	CHorzScaleRegion gRegions	= GetVertScaleGroupList().GetDrawingRegion().GetHorzScaleRegion();
	p_rctTopScale				= gRegions.GetTopScaleRegion();
	p_rctBottomScale			= gRegions.GetBottomScaleRegion();
	return TRUE;
}
BOOL CBlockImp::GetVertScaleRegion( CRect &p_rctLeftScale, CRect &p_rctRightScale)
{
	CVertScaleRegion gRegions	= GetVertScaleGroupList().GetDrawingRegion().GetVertScaleRegion();
	p_rctLeftScale				= gRegions.GetLeftScaleRegion();
	p_rctRightScale				= gRegions.GetRightScaleRegion();
	return TRUE;
}
CRect CBlockImp::GetGraphDrwingRegion( const CBlock::REGIONPOSITION p_rgnType)
{
	return m_vertScaleGroupList.GetGraphRegions().GetDrawingRegion();
}

CString CBlockImp::GetDateCompart( void)
{
	return m_blockData.GetHorzScale().GetScaleCompart().GetDateCompart();
}

CString CBlockImp::GetTimeCompart( void)
{
	return m_blockData.GetHorzScale().GetScaleCompart().GetTimeCompart();
}

// XScale Text 툴팁표시 - ojtaso (20080429)
CScaleBaseData::TEXTCOMPARTTYPE CBlockImp::GetScaleTextType( void)
{
	return m_blockData.GetHorzScale().GetScaleCompart().GetScaleTextType();
}

// 가로 scale의 min/max
CDisplayDataMinMax CBlockImp::GetViewHorzMinMax( void)
{
	if(m_pMainBlockBaseData == NULL)
		return CDisplayDataMinMax();

	CString strScaleData;

	CHorzDisplayDataMinMaxInputData inputData( GetHorzScaleDataPacket(), 
		m_blockData.GetHorzScale().GetScaleData(), m_pMainBlockBaseData->GetDisplayAttributes());
	
	return CViewMinMaxCalculator().GetViewHorzMinMax(inputData);
}

// scale 관련부분 변경
void CBlockImp::SetAllScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType)
{
	// 가로 scale
	m_blockData.GetHorzScale().SetGridType(gridType);
	// 세로 scale
	m_vertScaleGroupList.SetScaleGridType(gridType);
}

// (2006/10/24 - Seung-Won, Bae) Retrieve the Scale Index of First Visible VScale.
int CBlockImp::GetFirstVisibleVScaleIndex( void)
{
	CGraphSearchInfo_VisibleSubGraph gsi;
	if( !m_vertScaleGroupList.FindGraph( &gsi)) return -1;
	return gsi.m_nVertScaleGroupIndex;
}

CScaleBaseData::VERTSCALEPOSITION CBlockImp::GetVScalePosition( const int p_nVScaleGroupIndex)
{
	if( m_vertScaleGroupList.GetCount() < 1) return CScaleBaseData::VERT_NONE;
	CVertScaleGroup *pVertScaleGroup = m_vertScaleGroupList.GetVertScaleGroup( p_nVScaleGroupIndex);
	if( !pVertScaleGroup) return CScaleBaseData::VERT_NONE;
	return pVertScaleGroup->GetVertScale().GetVertScalePosition();
}

BOOL CBlockImp::SetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL p_bIsLock, CScaleBaseData::VERTMINMAXTYPE p_eMinMaxType, double p_dMin, double p_dMax)
{
	CVertScaleGroup *pVScaleGroup = m_vertScaleGroupList.GetVertScaleGroup( p_nVScaleGroupIndex);
	if( !pVScaleGroup) return FALSE;
	CVerticalScale &vertScale = pVScaleGroup->GetVertScale();
	vertScale.SetVertScaleMinMaxLock( p_bIsLock);
	vertScale.SetVertScaleMinMaxType( p_eMinMaxType);
	vertScale.SetVertScaleMin( p_dMin);
	vertScale.SetVertScaleMax( p_dMax);
	return FALSE;
}

BOOL CBlockImp::GetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL &p_bIsLock, CScaleBaseData::VERTMINMAXTYPE &p_eMinMaxType, double &p_dMin, double &p_dMax)
{
	CVertScaleGroup *pVScaleGroup = m_vertScaleGroupList.GetVertScaleGroup( p_nVScaleGroupIndex);
	if( !pVScaleGroup) return FALSE;
	CVerticalScale &vertScale = pVScaleGroup->GetVertScale();
	p_bIsLock		= vertScale.IsVertScaleMinMaxLock();
	p_eMinMaxType	= vertScale.GetVertScaleMinMaxType();
	p_dMin			= vertScale.GetVertScaleMin();
	p_dMax			= vertScale.GetVertScaleMax();
	return TRUE;
}

// (2006/11/12 - Seung-Won, Bae) Get VScale Min/Max Type
CScaleBaseData::VERTMINMAXTYPE CBlockImp::GetVScaleMinMaxType( int p_nVScaleIndex)
{
	CVertScaleGroup *pVertScaleGruop = m_vertScaleGroupList.GetVertScaleGroup( p_nVScaleIndex);
	if( !pVertScaleGruop) return CScaleBaseData::VERT_MINMAX_OF_VIEW;
	return pVertScaleGruop->GetVertScale().GetVertScaleMinMaxType();
}

BOOL CBlockImp::GetYScaleMinMax( int p_nVScaleIndex, double &p_dMin, double &p_dMax)
{
	// 화면에 보인는 세로 min/max
	CDisplayDataMinMax minMax(0, 0);
	CVertScaleGroup *pVertScaleGroup = m_vertScaleGroupList.GetVertScaleGroup( p_nVScaleIndex);
	if( pVertScaleGroup) minMax = pVertScaleGroup->GetDisplayDataMinMax();
	p_dMin = minMax.GetDisplayMin();
	p_dMax = minMax.GetDisplayMax();
	return TRUE;
}

BOOL CBlockImp::IsLogScale( void)
{
	return m_blockData.GetVertScaleLogType();
}

BOOL CBlockImp::SetLogScale( BOOL p_bLogScale)
{
	m_blockData.SetVertScaleLogType( p_bLogScale ? true : false);
	return TRUE;
}

BOOL CBlockImp::IsVScaleInvert( const int p_nV)
{
	CVertScaleGroup *pVScaleGroup = m_vertScaleGroupList.GetVertScaleGroup( p_nV);
	if( !pVScaleGroup) return FALSE;
	return pVScaleGroup->GetVertScaleInvertType();
}

BOOL CBlockImp::SetVScaleInvert( const int p_nV, BOOL p_bInvertScale)
{
	CVertScaleGroup *pVScaleGroup = m_vertScaleGroupList.GetVertScaleGroup( p_nV);
	if( !pVScaleGroup) return FALSE;
	pVScaleGroup->SetVertScaleInvertType( p_bInvertScale);
	return TRUE;
}

BOOL CBlockImp::HasInvertScale( void)
{
	CGraphSearchInfo_InvertVSCaleGroup gsi;
	gsi.m_bInvertType = TRUE;
	return m_vertScaleGroupList.FindGraph( &gsi);
}

BOOL CBlockImp::SetAllInvertScale( BOOL p_bInvertScale)
{
	m_vertScaleGroupList.SetAllVertScaleInvertType( p_bInvertScale);
	return TRUE;
}

int	CBlockImp::GetActiveVScaleIndex( CScaleBaseData::VERTSCALEPOSITION p_eVScalePosition)
{
	return m_vertScaleGroupList.GetActiveVScaleIndex( p_eVScalePosition);
}

// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
BOOL CBlockImp::GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG)
{
	if( !p_szGraphName) return FALSE;
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = p_szGraphName;
	if( !m_vertScaleGroupList.FindGraph( &gsi)) return FALSE;
	p_nV = gsi.m_nVertScaleGroupIndex;
	p_nG = gsi.m_nGraphIndex;
	return TRUE;
}

// (2006/11/28 - Seung-Won, Bae) Update Graph with Graph Infos. (The Changing of IGraphInfo did not make the Graph Updated.)
BOOL CBlockImp::UpdateGraphWithGraphInfo( int p_nV, int p_nG, BOOL p_bWithCalculate)
{
	CGraphImp *pGraph = m_vertScaleGroupList.GetGraph( p_nV, p_nG);
	if( !pGraph) return FALSE;

	pGraph->ChangeGraphDataFromIndicatorInfo( p_bWithCalculate ? true : false);

	return TRUE;
}

// (2007/1/20 - Seung-Won, Bae) Search Indicator
BOOL CBlockImp::FindFirstIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)
{
	p_nV = p_nG = -1;
	p_strGraphName.Empty();

	CGraphSearchInfo_AnyGraph					gsi;

	CGraphSearchInfo_AnyGraphWithRQ				gsiRQ;
	gsiRQ.m_strRQ								= p_szRQ;

	CGraphSearchInfo_IndicatorName				gsiIndicatorName;
	gsiIndicatorName.m_strIndicatorName			= p_szIndicatorName;

	CGraphSearchInfo_IndicatorNameWithRQ		gsiIndicatorNameWithRQ;
	gsiIndicatorNameWithRQ.m_strRQ				= p_szRQ;
	gsiIndicatorNameWithRQ.m_strIndicatorName	= p_szIndicatorName;
	
	CGraphSearchInfo *pGSI = &gsi;
	if( p_szIndicatorName && p_szRQ)		if( *p_szIndicatorName && *p_szRQ)
												pGSI = &gsiIndicatorNameWithRQ;
	if( pGSI == &gsi && p_szIndicatorName)	if( *p_szIndicatorName)
												pGSI = &gsiIndicatorName;
	if( pGSI == &gsi && p_szRQ)				if( *p_szRQ)
												pGSI = &gsiRQ;

	if( !m_vertScaleGroupList.FindGraph( pGSI)) return FALSE;
	if( !pGSI->m_pGraphImp) return FALSE;

	p_nV			= pGSI->m_nVertScaleGroupIndex;
	p_nG			= pGSI->m_nGraphIndex;
	p_strGraphName	= pGSI->m_pGraphImp->GetGraphData().GetGraphName();
	return TRUE;
}
BOOL CBlockImp::FindNextIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)
{
	p_strGraphName.Empty();

	CGraphSearchInfo_AnyGraph					gsi;

	CGraphSearchInfo_AnyGraphWithRQ				gsiRQ;
	gsiRQ.m_strRQ								= p_szRQ;

	CGraphSearchInfo_IndicatorName				gsiIndicatorName;
	gsiIndicatorName.m_strIndicatorName			= p_szIndicatorName;

	CGraphSearchInfo_IndicatorNameWithRQ		gsiIndicatorNameWithRQ;
	gsiIndicatorNameWithRQ.m_strRQ				= p_szRQ;
	gsiIndicatorNameWithRQ.m_strIndicatorName	= p_szIndicatorName;
	
	CGraphSearchInfo *pGSI = &gsi;
	if( p_szIndicatorName && p_szRQ)		if( *p_szIndicatorName && *p_szRQ)
												pGSI = &gsiIndicatorNameWithRQ;
	if( pGSI == &gsi && p_szIndicatorName)	if( *p_szIndicatorName)
												pGSI = &gsiIndicatorName;
	if( pGSI == &gsi && p_szRQ)				if( *p_szRQ)
												pGSI = &gsiRQ;

	pGSI->m_nVertScaleGroupIndex	= p_nV;
	pGSI->m_nGraphIndex				= p_nG;
	pGSI->SetNextStartGraph();

	if( !m_vertScaleGroupList.FindGraph( pGSI)) return FALSE;
	if( !pGSI->m_pGraphImp) return FALSE;

	p_nV			= pGSI->m_nVertScaleGroupIndex;
	p_nG			= pGSI->m_nGraphIndex;
	p_strGraphName	= pGSI->m_pGraphImp->GetGraphData().GetGraphName();
	return TRUE;
}

BOOL CBlockImp::GetHeadPacketAndDataType( int p_nVertScaleGroupIndex, CPacket *&p_pPacket, double &p_dDataType)
{
	p_pPacket	= NULL;
	p_dDataType = 1.0l;

	CGraphImp *pGraph = m_vertScaleGroupList.GetGraph( 0, 0);
	if( !pGraph) return FALSE;

	return pGraph->GetSubGraphList().GetHeadPacketAndDataType(p_pPacket, p_dDataType);
}

// (2009/10/2 - Seung-Won, Bae) Get Horizontal Scale RQ for X Scale of each block with different time scale.
CString CBlockImp::GetHorzScaleDataRQ( void)
{
	return m_pBlocksColumn->GetHorzScaleDataRQ( this);
}

// (2009/10/1 - Seung-Won, Bae) Get Horizontal Scale Packet for X Scale of each block with different time scale.
CPacket *CBlockImp::GetHorzScaleDataPacket( void)
{
	CString strPacketName = m_blockData.GetHorzScale().GetScaleData();
	if( strPacketName.IsEmpty()) return NULL;

	CString strRQ = GetHorzScaleDataRQ();
	if( strRQ.IsEmpty()) return NULL;

	CPacketListManager *pPacketListManager = m_pMainBlockBaseData->GetPacketListManager();
	if( !pPacketListManager) return NULL;

	CPacketList *pPacketList = pPacketListManager->GetPacketList( strRQ);
	if( !pPacketList) return NULL;
	
	return pPacketList->GetBaseData( strPacketName);
}

// XScale 높이 지정 - ojtaso (20080429)
int CBlockImp::GetHorizontalScaleHeight() const
{
	return m_blockData.GetHorizontalScaleHeight();
}

// (2008/10/22 - Seung-Won, Bae) Retreive RQ
const char *CBlockImp::GetMasterValueRQ( void)
{
	CVertScaleGroup* pVertScaleGroup = GetVertScaleGroupList().GetVertScaleGroup( GetFirstVisibleVScaleIndex());
	if( !pVertScaleGroup) return _T("DEFAULT");
	CGraphImp* pGraph = pVertScaleGroup->GetGraph( 0);
	if(!pGraph) return _T("DEFAULT");
	return pGraph->GetGraphRQ();
}

// (2009/10/15 - Seung-Won, Bae) Retrieve Coordinate Infos.
CString CBlockImp::GetCoordinateInfos( CPacket *&p_pPacketX, CPacket *&p_pPacketY, double &p_dMinY, double &p_dMaxY, double &p_dDataType, BOOL &p_bLog, BOOL &p_bInvert)
{
	CGraphSearchInfo_VisibleSubGraph gsi;
	if( !m_vertScaleGroupList.FindGraph( &gsi)) return "";

	// 1. Get Select Graph in first visible VScale Group.
	if( !gsi.m_pVertScaleGroup) return "";
	CGraphImp *pGraph = NULL;
	// 1.1. Find Selected RQ's graph first.
	CString strRQ = m_pMainBlockBaseData->GetMainAttributes().GetIChartCtrl()->GetCurrentRQ();
	CGraphSearchInfo_AnyGraphWithRQ gsi3;
	gsi3.m_strRQ = strRQ;
	if( gsi.m_pVertScaleGroup->FindGraph( &gsi3))
		pGraph = gsi3.m_pGraphImp;
	if( !pGraph)
	{
		pGraph = gsi.m_pVertScaleGroup->GetSelectedGraph();
		// 1.2. if no selected grpah, no selected rq graph, then first graph.
		if( !pGraph) pGraph = gsi.m_pGraphImp;
	}
	if( !pGraph) return "";

	// 2. Get X, Y Packet.
	strRQ = pGraph->GetGraphRQ();
	CPacketListManager *pPacketListManager = m_pMainBlockBaseData->GetPacketListManager();
	if( !pPacketListManager) return "";
	CPacketList *pPacketList = pPacketListManager->GetPacketList( strRQ);
	if( !pPacketList) return "";
	// 2.1. Get X Packet.
	CString strPacketName = m_blockData.GetHorzScale().GetScaleData();
	if( !strPacketName.IsEmpty()) p_pPacketX = pPacketList->GetBaseData( strPacketName);
	// 2.2. Get Y Packet
	p_pPacketY = pGraph->GetSubGraphList().GetHeadPacket();
	// 2.2.1. Use close packet with price chart.
	CGraphSearchInfo_IndicatorNameWithRQ gsi2;
	gsi2.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
	gsi2.m_strRQ = strRQ;
	if( gsi.m_pVertScaleGroup->FindGraph( &gsi2))
	{
		// 가격차트이면서 종가가 없는 차트 세팅이 있는 경우때문에.. - onlyojt (20100129)
//		p_pPacketY = pPacketList->GetBaseData( _MTEXT( C0_CLOSE));
		p_pPacketY = pPacketList->GetBaseData( _MTEXT( C0_CLOSE), false);
	}
	// 2.2.2. Get Data type.
	//alzioyes091217:조회된 값이 없을 때는 p_pPacketY값이 NULL임.
	if(p_pPacketY) p_dDataType = p_pPacketY->GetScaleTypeFormatDouble();

	// 3. Get VScale Info.
	CDisplayDataMinMax ddMinMax = gsi.m_pVertScaleGroup->GetDisplayDataMinMax();
	p_dMaxY = ddMinMax.GetDisplayMax();
	p_dMinY = ddMinMax.GetDisplayMin();
	CVerticalScale &vScale = gsi.m_pVertScaleGroup->GetVertScale();
	p_bLog = m_blockData.GetVertScaleLogType();
	p_bInvert = vScale.GetVertScaleInvertType();

	return strRQ;
}
