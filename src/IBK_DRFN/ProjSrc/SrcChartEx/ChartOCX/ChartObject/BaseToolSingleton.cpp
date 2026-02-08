// BaseToolSingleton.cpp: implementation of the CBaseToolSingleton class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseToolSingleton.h"

#include "../Include_Chart/Dll_Load/MainBlock.h"
// 블럭 최대화 추가 - ojtaso (20080519)
#include "../Include_Chart/IPropertyMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CBlockToolSingleton

// 블럭 최대화 추가 - ojtaso (20080519)
CBlockToolSingleton::CBlockToolSingleton(const bool bUsingMoveTool, const bool bUsingDeleteTool, const bool bUsingSizeTool):
	m_bUsingMoveTool(bUsingMoveTool),
	m_bUsingDeleteTool(bUsingDeleteTool),
	m_bUsingSizeTool(bUsingSizeTool)
{
}

void CBlockToolSingleton::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_blockMoveTool.SetIChartCtrl(p_pICharetCtrl);
	m_blockDeleteTool.SetIChartCtrl(p_pICharetCtrl);
	// 블럭 최대화 추가 - ojtaso (20080519)
	m_blockSizeTool.SetIChartCtrl(p_pICharetCtrl);
}

void CBlockToolSingleton::SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing)
{
	switch(eToolType){
	case CBlockBaseData::TOOL_BLOCKMOVE:	// block 이동 tool	
		m_bUsingMoveTool = bIsUsing;
		break;	
	case CBlockBaseData::TOOL_BLOCKDELETE:	// block 삭제 tool
		m_bUsingDeleteTool = bIsUsing;
		break;
	case CBlockBaseData::TOOL_BLOCKSIZE:	// 블럭 최대화 추가 - ojtaso (20080519)
		m_bUsingSizeTool = bIsUsing;
		break;
	}
}

// ----------------------------------------------------------------------------
CBlockTool* CBlockToolSingleton::GetTool(CMainBlock* pMainBlock, const CPoint& point)
{
	if(m_bUsingMoveTool && m_blockMoveTool.Find(pMainBlock, point))
		return &m_blockMoveTool;
	if(m_bUsingDeleteTool && m_blockDeleteTool.Find(pMainBlock, point))
		return &m_blockDeleteTool;
	// 블럭 최대화 추가 - ojtaso (20080519)
	if(m_bUsingSizeTool && m_blockSizeTool.Find(pMainBlock, point))
		return &m_blockSizeTool;

	return NULL;
}

// ----------------------------------------------------------------------------
void CBlockToolSingleton::OnDraw(CDC* pDC, CMainBlock* pMainBlock)
{
	if(pMainBlock == NULL)
		return;

	if(m_bUsingMoveTool)
		m_blockMoveTool.DrawSelectionButton(pDC, pMainBlock);

	if(m_bUsingDeleteTool)
		m_blockDeleteTool.DrawSelectionButton(pDC, pMainBlock);

	// 블럭 최대화 추가 - ojtaso (20080519)
	if(m_bUsingSizeTool)
		m_blockSizeTool.DrawSelectionButton(pDC, pMainBlock);
}


///////////////////////////////////////////////////////////////////////////////
// class CScaleToolSingleton

CScaleToolSingleton::CScaleToolSingleton(const bool bUsingHorzTool, const bool bUsingVertTool):
	m_bUsingHorzTool(bUsingHorzTool),
	m_bUsingVertTool(bUsingVertTool)
{
}

void CScaleToolSingleton::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_horzScaleTool.SetIChartCtrl(p_pICharetCtrl);
	m_vertScaleTool.SetIChartCtrl(p_pICharetCtrl);
}

void CScaleToolSingleton::SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing)
{
	switch(eToolType){
	case CBlockBaseData::TOOL_HORZSCALE:	// x축 scale tool
		m_bUsingHorzTool = bIsUsing;
		break;
	case CBlockBaseData::TOOL_VERTSCALE:	// y축 scale tool
		m_bUsingVertTool = bIsUsing;
		break;
	}
}

// ----------------------------------------------------------------------------
CScaleTool* CScaleToolSingleton::GetTool(CMainBlock* pMainBlock, const CPoint& point)
{
	if(m_bUsingHorzTool && m_horzScaleTool.Find(pMainBlock, point))
		return &m_horzScaleTool;
	if(m_bUsingVertTool && m_vertScaleTool.Find(pMainBlock, point))
		return &m_vertScaleTool;

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphToolSingleton

CGraphToolSingleton::CGraphToolSingleton(const bool bUsingTitleTool, const bool bUsingSubGraphTool):
	m_bUsingTitleTool(bUsingTitleTool),
	m_bUsingSubGraphTool(bUsingSubGraphTool)
{
}

void CGraphToolSingleton::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_subGraphTitleTool.SetIChartCtrl(p_pICharetCtrl);
	m_subGraphTool.SetIChartCtrl(p_pICharetCtrl);
}

void CGraphToolSingleton::SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing)
{
	switch(eToolType){
	case CBlockBaseData::TOOL_GRAPHTITLE:	// graph title tool
		m_bUsingTitleTool = bIsUsing;
		break;
	case CBlockBaseData::TOOL_SUBGRAPH:		// subGraph 선택 tool
		m_bUsingSubGraphTool = bIsUsing;
		break;
	case CBlockBaseData::TOOL_GRAPHMOVE:	// graph title / subGraph tool 의 이동
		m_subGraphTitleTool.SetGraphMove(bIsUsing);
		m_subGraphTool.SetGraphMove(bIsUsing);
		break;
	case CBlockBaseData::TOOL_GRAPHDELETE:	// graph title / subGraph tool 의 삭제
		m_subGraphTitleTool.SetGraphDelete(bIsUsing);
		m_subGraphTool.SetGraphDelete(bIsUsing);
		break;
	}
}

// ----------------------------------------------------------------------------
// (2006/11/1 - Seung-Won, Bae) Support Graph Title First.
CGraphTool* CGraphToolSingleton::GetTitleTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useSubGraphTool)
{
	if( m_bUsingTitleTool && m_subGraphTitleTool.Find(pDC, pMainBlock, point)) return &m_subGraphTitleTool;
	return NULL;
}

CGraphTool* CGraphToolSingleton::GetShapeTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useSubGraphTool)
{
	if( m_bUsingSubGraphTool && useSubGraphTool && m_subGraphTool.Find(pDC, pMainBlock, point)) return &m_subGraphTool;
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// class CBaseToolSingleton

CBaseToolSingleton::CBaseToolSingleton():
	m_bUsingComparLineTool(true),
	// 블럭 최대화 추가 - ojtaso (20080519)
	m_blockTool(false, false, false),
	m_scaleTool(true, true),
	m_graphTool(false, false)
{
}

void CBaseToolSingleton::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_compartLineTool.SetIChartCtrl(p_pICharetCtrl);
	m_blockTool.SetIChartCtrl(p_pICharetCtrl);
	m_scaleTool.SetIChartCtrl(p_pICharetCtrl);
	m_graphTool.SetIChartCtrl(p_pICharetCtrl);
}

void CBaseToolSingleton::SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing)
{
	switch(eToolType){
	case CBlockBaseData::TOOL_COMPARTLINE:	// block 크기 tool
		m_bUsingComparLineTool = bIsUsing;
		break;
	case CBlockBaseData::TOOL_BLOCKMOVE:	// block 이동 tool		
	case CBlockBaseData::TOOL_BLOCKDELETE:	// block 삭제 tool
	case CBlockBaseData::TOOL_BLOCKSIZE:	// 블럭 최대화 추가 - ojtaso (20080519)
		m_blockTool.SetUsingTool(eToolType, bIsUsing);
		break;
	case CBlockBaseData::TOOL_HORZSCALE:	// x축 scale tool
	case CBlockBaseData::TOOL_VERTSCALE:	// y축 scale tool
		m_scaleTool.SetUsingTool(eToolType, bIsUsing);
		break;
	case CBlockBaseData::TOOL_GRAPHTITLE:	// graph title tool
	case CBlockBaseData::TOOL_SUBGRAPH:		// subGraph 선택 tool
	case CBlockBaseData::TOOL_GRAPHMOVE:	// graph title / subGraph tool 의 이동
	case CBlockBaseData::TOOL_GRAPHDELETE:	// graph title / subGraph tool 의 삭제
		m_graphTool.SetUsingTool(eToolType, bIsUsing);
		break;
	}
}

// ----------------------------------------------------------------------------
CBaseTool* CBaseToolSingleton::GetTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useScaleTool, const bool useSubGraphTool)
{
	if(pMainBlock == NULL)
		return NULL;

	if(m_bUsingComparLineTool && m_compartLineTool.Find(pMainBlock, point))
		return &m_compartLineTool;

	// 디자인 모드일 경우엔 compartLine Tool 만 사용된다!!
	// (2006/10/24 - Seung-Won, Bae) Support Graph Tool in Design Time.
	// (2006/11/1 - Seung-Won, Bae) Support Graph Title First in RunTime.
	CBaseTool* pBaseTool = m_graphTool.GetTitleTool(pDC, pMainBlock, point, FALSE);
	if( pBaseTool) return pBaseTool;
	if( pMainBlock->GetDesignMode()) return NULL;

	pBaseTool = m_blockTool.GetTool(pMainBlock, point);
	if( pBaseTool) return pBaseTool;
	
	// scale쪽은 mousemove 와 상관이 없음 -> RButton Click 시만 사용함
	if( useScaleTool)
	{
		pBaseTool = m_scaleTool.GetTool(pMainBlock, point);
		if( pBaseTool) return pBaseTool;
	}

	return m_graphTool.GetShapeTool(pDC, pMainBlock, point, useSubGraphTool);
}

// ----------------------------------------------------------------------------
void CBaseToolSingleton::OnDraw(CDC* pDC, CMainBlock* pMainBlock)
{
	if(pMainBlock == NULL)
		return;

	m_blockTool.OnDraw(pDC, pMainBlock);
}
