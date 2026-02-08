// GraphSearchInfo.cpp: implementation of the CGraphSearchInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphSearchInfo.h"

#include "GraphImp.h"					// for CGraphImp
#include "SubGraph.h"				// for CSubGraph
#include "VertScaleGroup.h"			// for CVertScaleGroup
#include "BlockImp.h"				// for CBlockImp
#include "BlocksColumn.h"			// for CBlocksColumn

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo
//////////////////////////////////////////////////////////////////////

CGraphSearchInfo::CGraphSearchInfo()
{
	ResetInfo();
}

void CGraphSearchInfo::ResetInfo( void)
{
	// Search Position and Index
	m_posColumnBlock		= NULL;
	m_posRowBlock			= NULL;
	m_posVertScaleGroup		= NULL;
	m_posGraph				= NULL;
	m_posSubGraph			= NULL;
	
	m_nColumnBlockIndex		= 0;
	m_nRowBlockIndex		= 0;
	m_nVertScaleGroupIndex	= 0;
	m_nGraphIndex			= 0;
	m_nSubGraphIndex		= 0;

	m_pBlocksColumn			= NULL;
	m_pBlockImp				= NULL;
	m_pVertScaleGroup		= NULL;
	m_pGraphImp				= NULL;
	m_pSubGraph				= NULL;
}

BOOL CGraphSearchInfo::FindGraph( const CBlocksColumn *p_pBlocksColumn)
{
	return p_pBlocksColumn ? p_pBlocksColumn->FindGraph( this) : FALSE;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CGraphSearchInfo::FindGraph(LPVOID pPacketRQ, const CBlockImp *p_pBlockImp)
{
	return p_pBlockImp ? p_pBlockImp->GetVertScaleGroupList().FindGraph(pPacketRQ, this) : FALSE;
}

BOOL CGraphSearchInfo::FindGraph( const CBlockImp *p_pBlockImp)
{
	return p_pBlockImp ? p_pBlockImp->GetVertScaleGroupList().FindGraph( this) : FALSE;
}

// (2008/9/15 - Seung-Won, Bae) RQ Search
BOOL CGraphSearchInfo::FindGraph(LPVOID pPacketRQ,  const CVertScaleGroup *p_pVertScaleGroup)
{
	return p_pVertScaleGroup ? p_pVertScaleGroup->FindGraph( pPacketRQ, this) : FALSE;
}

BOOL CGraphSearchInfo::FindGraph( const CVertScaleGroup *p_pVertScaleGroup)
{
	return p_pVertScaleGroup ? p_pVertScaleGroup->FindGraph( this) : FALSE;
}

BOOL CGraphSearchInfo::FindGraph( const CGraphImp *p_pGraph)
{
	return p_pGraph ? p_pGraph->GetSubGraphList().FindGraph( this) : FALSE;
}

BOOL CGraphSearchInfo::FindGraph( const CSubGraph *p_pSubGraph)
{
	return FALSE;
}

// (2007/4/12 - Seung-Won, Bae) Support Next Object Search.
void CGraphSearchInfo::SetNextStartColumnBlock( void)
{
	SetNextStartRowBlock();
	m_nRowBlockIndex = 0;

	m_posColumnBlock = NULL;
	m_nColumnBlockIndex++;
}
void CGraphSearchInfo::SetNextStartRowBlock( void)
{
	SetNextStartVerticalScaleGroup();
	m_nVertScaleGroupIndex = 0;

	m_posRowBlock = NULL;
	m_nRowBlockIndex++;
}
void CGraphSearchInfo::SetNextStartVerticalScaleGroup( void)
{
	SetNextStartGraph();
	m_nGraphIndex = 0;

	m_posVertScaleGroup = NULL;
	m_nVertScaleGroupIndex++;
}
void CGraphSearchInfo::SetNextStartGraph( void)
{
	SetNextStartSubGraph();
	m_nSubGraphIndex = 0;

	m_posGraph = NULL;
	m_nGraphIndex++;
}
void CGraphSearchInfo::SetNextStartSubGraph( void)
{
	m_posSubGraph = NULL;
	m_nSubGraphIndex++;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_BlocksColumn
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_BlocksColumn::FindGraph( const CBlocksColumn *p_pBlocksColumn)
{
	if( !p_pBlocksColumn) return FALSE;
	return ( p_pBlocksColumn == m_pBlocksColumn);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_Block
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_Block::FindGraph( const CBlockImp *p_pBlockImp)
{
	if( !p_pBlockImp) return FALSE;
	return ( p_pBlockImp == m_pBlockImp);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_OtherBlock
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_OtherBlock::FindGraph( const CBlockImp *p_pBlockImp)
{
	if( !p_pBlockImp) return FALSE;
	return ( p_pBlockImp != m_pBlockImp);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_DoubleSizeBlock
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_DoubleSizeBlock::FindGraph( const CBlockImp *p_pBlockImp)
{
	if( !p_pBlockImp) return FALSE;
	return p_pBlockImp->GetBlockData().GetDoubleSize();
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_VerticalScaleGroup
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_VerticalScaleGroup::FindGraph( const CVertScaleGroup *p_pVertScaleGroup)
{
	if( !p_pVertScaleGroup) return FALSE;
	return ( m_pVertScaleGroup == p_pVertScaleGroup);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_InvertVSCaleGroup
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_InvertVSCaleGroup::FindGraph( const CVertScaleGroup *p_pVertScaleGroup)
{
	if( !p_pVertScaleGroup) return FALSE;
	BOOL bInvertType = p_pVertScaleGroup->GetVertScaleInvertType();
	return ( m_bInvertType && bInvertType) || ( !m_bInvertType && !bInvertType);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_HasVertScale
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_HasVertScale::FindGraph( const CVertScaleGroup *p_pVertScaleGroup)
{
	if( !p_pVertScaleGroup) return FALSE;
	CScaleBaseData::VERTSCALEPOSITION eVertScalePosition = p_pVertScaleGroup->GetVertScale().GetVertScalePosition();
	if( eVertScalePosition == m_eVertScalePosition) return TRUE;
	if( m_eVertScalePosition == -1 &&  eVertScalePosition != CScaleBaseData::VERT_NONE) return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_HasVertScaleAndGridLine
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_HasVertScaleAndGridLine::FindGraph( const CVertScaleGroup *p_pVertScaleGroup)
{
	if( !CGraphSearchInfo_HasVertScale::FindGraph( p_pVertScaleGroup)) return FALSE;
	return ( p_pVertScaleGroup->GetVertScale().GetGridType() != CScaleBaseData::SLNONE);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_ActiveVertScaleAndGridLine
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_ActiveVertScaleAndGridLine::FindGraph( const CVertScaleGroup *p_pVertScaleGroup)
{
	if( !p_pVertScaleGroup) return FALSE;
	if( m_nActiveFlag < 0) return ( p_pVertScaleGroup->GetVertScale().GetActiveFlag() & VSGA_GRIDLINE);
	return ( p_pVertScaleGroup->GetVertScale().GetActiveFlag() & m_nActiveFlag);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_NotOneGraphVScaleGroup
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_NotOneGraphVScaleGroup::FindGraph(  const CVertScaleGroup *p_pVertScaleGroup)
{
	if( !p_pVertScaleGroup) return FALSE;
	return ( 1 != p_pVertScaleGroup->GetGraphCount());
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_AnyGraph
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_AnyGraph::FindGraph(  const CGraphImp *p_pGraph)
{
	if( !p_pGraph) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_AnyGraphWithRQ
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_AnyGraphWithRQ::FindGraph(  const CGraphImp *p_pGraph)
{
	if( !p_pGraph) return FALSE;
	return m_strRQ.IsEmpty() || ( p_pGraph->GetGraphRQ() == m_strRQ);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_IndicatorName
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_IndicatorName::FindGraph( const CGraphImp *p_pGraph)
{
	if( !p_pGraph) return FALSE;
	return p_pGraph->GetGraphData().GetIndicatorName() == m_strIndicatorName;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_IndicatorNameWithRQ
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_IndicatorNameWithRQ::FindGraph( const CGraphImp *p_pGraph)
{
	if( !p_pGraph) return FALSE;
	if( !CGraphSearchInfo_IndicatorName::FindGraph( p_pGraph)) return FALSE;
	return m_strRQ.IsEmpty() || ( p_pGraph->GetGraphRQ() == m_strRQ);
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_IndicatorObject
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_IndicatorObject::FindGraph( const CGraphImp *p_pGraph)
{
	if( !p_pGraph || m_pExceptGraph == p_pGraph) return FALSE;
	return p_pGraph->GetIndicatorInfo() == m_pIndicatorInfo;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_GraphName
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_GraphName::FindGraph( const CGraphImp *p_pGraph)
{
	if( !p_pGraph) return FALSE;
	return p_pGraph->GetGraphData().GetGraphName() == m_strGraphName;
}

// RQ별 찾기 : 복수종목 - ojtaso (20070306)
//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_GraphNameWithRQ
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_GraphNameWithRQ::FindGraph( const CGraphImp *p_pGraph)
{
	if( !p_pGraph) return FALSE;
	CString strName = p_pGraph->GetGraphData().GetIndicatorName();
	return strName == m_strGraphName && p_pGraph->GetGraphRQ() == m_strRQ;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_GraphNameAndSubGraphName
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_GraphNameAndSubGraphName::FindGraph( const CGraphImp *p_pGraph)
{
	if( !p_pGraph) return FALSE;
	if( p_pGraph->GetGraphData().GetGraphName() != m_strGraphName) return FALSE;
	return p_pGraph->GetSubGraphList().FindGraph( this);
}

BOOL CGraphSearchInfo_GraphNameAndSubGraphName::FindGraph( const CSubGraph *p_pSubGraph)
{
	if( !p_pSubGraph) return FALSE;
	return p_pSubGraph->GetSubGraphData().GetName() == m_strSubGraphName;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_GraphNameAndSubGraphIndex
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_GraphNameAndSubGraphIndex::FindGraph( const CGraphImp *p_pGraph)
{
	if( !p_pGraph) return FALSE;
	if( p_pGraph->GetGraphData().GetGraphName() != m_strGraphName) return FALSE;
	m_pSubGraph = p_pGraph->GetSubGraphList().FindSubGraph( m_nSubGraphIndex);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_SubGraphTitle
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_SubGraphTitle::FindGraph( const CSubGraph *p_pSubGraph)
{
	if( !p_pSubGraph) return FALSE;
	return p_pSubGraph->GetSubGraphData().GetTitle() == m_strSubGraphTitle;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_VisibleSubGraph
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_VisibleSubGraph::FindGraph( const CSubGraph *p_pSubGraph)
{
	if( !p_pSubGraph) return FALSE;
	return !p_pSubGraph->GetHiding();
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_VisibleGraphTitle
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_VisibleGraphTitle::FindGraph( const CSubGraph *p_pSubGraph)
{
	if( !p_pSubGraph) return FALSE;

	if( !CGraphSearchInfo_VisibleSubGraph::FindGraph( p_pSubGraph)) return FALSE;
	return !p_pSubGraph->GetSubGraphData().GetTitle().IsEmpty();
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_GraphType
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_GraphType::FindGraph( const CSubGraph *p_pSubGraph)
{
	if( !p_pSubGraph) return FALSE;

	const CSubGraphData &subGraphData = p_pSubGraph->GetSubGraphData();
	if( subGraphData.GetType() != m_eGraphType) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_TypeStyleDrawStyle
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_TypeStyleDrawStyle::FindGraph( const CSubGraph *p_pSubGraph)
{
	if( !p_pSubGraph) return FALSE;
	if( !CGraphSearchInfo_GraphType::FindGraph( p_pSubGraph)) return FALSE;

	const CSubGraphData &subGraphData = p_pSubGraph->GetSubGraphData();
	if( subGraphData.GetStyle().GetGraphStyle() != m_nGraphStyle) return FALSE;
	if( subGraphData.GetDrawStyle().GetDrawStyle() != m_nDrawStyle) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_VisibleTypeStyleDrawStyle
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_VisibleTypeStyleDrawStyle::FindGraph( const CSubGraph *p_pSubGraph)
{
	if( !CGraphSearchInfo_TypeStyleDrawStyle::FindGraph( p_pSubGraph)) return FALSE;
	return !p_pSubGraph->GetHiding();
}

// 한번에 그래프 속성 변경 가능한 메쏘드 추가 - ojtaso (20080602)
//////////////////////////////////////////////////////////////////////
// CGraphSearchInfo_SubGraphTitle
//////////////////////////////////////////////////////////////////////

BOOL CGraphSearchInfo_SubGraphTitleWithRQ::FindGraph( const CSubGraph *p_pSubGraph)
{
	if( !p_pSubGraph) return FALSE;
	return p_pSubGraph->GetSubGraphData().GetTitle() == m_strSubGraphTitle && p_pSubGraph->GetGraph()->GetGraphRQ() == m_strRQ;
}
