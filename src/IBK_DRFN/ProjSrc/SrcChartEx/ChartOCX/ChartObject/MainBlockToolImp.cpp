// MainBlockToolImp.cpp: implementation of the CMainBlockToolImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainBlockToolImp.h"

#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CMainBlockToolImp

CMainBlockToolImp::CMainBlockToolImp(IChartCtrl* p_pICharetCtrl):
	m_pIChartCtrl(p_pICharetCtrl),
	m_pBaseTool(NULL),
	m_pOldBaseTool(NULL)
{
}

// public =====================================================================
void CMainBlockToolImp::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_pIChartCtrl = p_pICharetCtrl;
	m_toolSingleton.SetIChartCtrl(p_pICharetCtrl);
}

// tool을 사용할지 여부
// 블럭 최대화 추가 - ojtaso (20080519)
void CMainBlockToolImp::SetUsingTool(const bool bIsUsing_compartLine, const bool bIsUsing_blockMove, const bool bIsUsing_blockDelete, 
		const bool bIsUsing_horzScale, const bool bIsUsing_vertScale, const bool bIsUsing_graphTitle, const bool bIsUsing_subGraph, 
		const bool bIsUsing_graphMove, const bool bIsUsing_graphDelete, const bool bIsUsing_blockSize)
{
	// block 크기 tool
	SetUsingTool(CBlockBaseData::TOOL_COMPARTLINE, bIsUsing_compartLine);
	// block 이동 tool
	SetUsingTool(CBlockBaseData::TOOL_BLOCKMOVE, bIsUsing_blockMove);
	// block 삭제 tool
	SetUsingTool(CBlockBaseData::TOOL_BLOCKDELETE, bIsUsing_blockDelete);
	// x축 scale tool
	SetUsingTool(CBlockBaseData::TOOL_HORZSCALE, bIsUsing_horzScale);
	// y축 scale tool
	SetUsingTool(CBlockBaseData::TOOL_VERTSCALE, bIsUsing_vertScale);
	// graph title tool (선택, 설정)
	SetUsingTool(CBlockBaseData::TOOL_GRAPHTITLE, bIsUsing_graphTitle);
	// sugGraph tool	(선택, 설정, 강조)
	SetUsingTool(CBlockBaseData::TOOL_SUBGRAPH, bIsUsing_subGraph);
	// graph title / subGraph tool 의 이동
	SetUsingTool(CBlockBaseData::TOOL_GRAPHMOVE, bIsUsing_graphMove);
	// graph title / subGraph tool 의 삭제
	SetUsingTool(CBlockBaseData::TOOL_GRAPHDELETE, bIsUsing_graphDelete);
	// 블럭 최대화 추가 - ojtaso (20080519)
	SetUsingTool(CBlockBaseData::TOOL_BLOCKSIZE, bIsUsing_blockSize);
}

void CMainBlockToolImp::SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing)
{
	m_toolSingleton.SetUsingTool(eToolType, bIsUsing);
}

// 선택된 Tool의 내용을 Reset하고 선택해지를 한다.
void CMainBlockToolImp::ResetCurrentTool()
{
	if(m_pBaseTool != NULL)
		m_pBaseTool->InitializeAllData();
	
	m_pBaseTool = NULL;
	m_pOldBaseTool = NULL;
}

// ----------------------------------------------------------------------------
CBlockBaseData::TOOLTYPE CMainBlockToolImp::GetCurrentToolType() const
{
	if(m_pBaseTool == NULL)
		return CBlockBaseData::TOOL_NONE;
	return m_pBaseTool->GetToolType();
}

// ----------------------------------------------------------------------------
// block 이동
bool CMainBlockToolImp::OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	SetCurrentBaseTool(pDC, pMainBlock, point, false);
	if(!IsActionOfTool(pMainBlock))
		return false;

	if(!m_pBaseTool) return true;

	// 블럭 최대화 추가 - ojtaso (20080519)
	if( m_pBaseTool->GetToolType() == CBlockBaseData::TOOL_COMPARTLINE && pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero())
		return false;

	bool bResult = m_pBaseTool->OnLButtonDown(pDC, pMainBlock, nFlags, point);
	if(bResult)
		Invalidate();

	return bResult;
}

bool CMainBlockToolImp::OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	if(!m_pBaseTool) return true;

	// 블럭 최대화 추가 - ojtaso (20080519)
	if( m_pBaseTool->GetToolType() == CBlockBaseData::TOOL_COMPARTLINE && pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero())
		return false;

	bool result = false;
	if(IsActionOfTool(pMainBlock)){
		result = m_pBaseTool->OnLButtonUp(pDC, pMainBlock, point);
		Invalidate();
	}
	
	InitializeAllData();
	return result;
}

bool CMainBlockToolImp::OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	SetCurrentBaseTool(pDC, pMainBlock, point, true);
	if(!IsActionOfTool(pMainBlock))
		return false;

	if(!m_pBaseTool) return true;
	bool bResult = m_pBaseTool->OnLButtonDblClk(pDC, pMainBlock, point);
	if(bResult)
		Invalidate();
	InitializeAllData();
	// (2007/1/3 - Seung-Won, Bae) for deny Next AddIn LButtonDlbClick Process
	return true;
}

CMenuType::MENUTYPE CMainBlockToolImp::OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const UINT /*nFlags*/, const CPoint& point)
{
	SetCurrentBaseTool_OnRButton(pDC, pMainBlock, point, true);
	if(!IsActionOfTool(pMainBlock)){
		InitializeAllData(true);
		return CMenuType::MENU_NONE;
	}

	if(!m_pBaseTool) return CMenuType::MENU_NONE;
	CMenuType::MENUTYPE result = m_pBaseTool->OnRButtonUp(pDC, pMainBlock, point);
	InitializeAllData();
	return result;
}

bool CMainBlockToolImp::OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	if(!IsSelectingTool(pMainBlock))
		return false;
	
	if(m_pBaseTool == NULL)
		return OnMouseMove_ProcessFind(pDC, pMainBlock, nFlags, point);

	m_pOldBaseTool = NULL;
	
	// 블럭 최대화 추가 - ojtaso (20080519)
	if( m_pBaseTool->GetToolType() == CBlockBaseData::TOOL_COMPARTLINE && pMainBlock->GetMaximizedBlock().IsAllValueBiggerThanZero())
		return false;

	m_pBaseTool->OnMouseMove(pDC, pMainBlock, nFlags, point);
	return false;
}

bool CMainBlockToolImp::OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	bool result = false;
	if(IsActionOfTool(pMainBlock)){
		result = m_pBaseTool->OnMenu(pMainBlock, menuType, wParam);
		if(result)
			Invalidate();
	}

	InitializeAllData();
	return result;
}

// 지표 뒤집기 - ojtaso (20071023)
bool CMainBlockToolImp::GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	bool result = false;
	if(IsActionOfTool(pMainBlock)){
		result = m_pBaseTool->GetMenuCheck(pMainBlock, menuType, wParam);
	}

	InitializeAllData();
	return result;
}

void CMainBlockToolImp::OnDraw(CDC* pDC, CMainBlock* pMainBlock)
{
	// tool과 관련된 기본 그림 그리기
	m_toolSingleton.OnDraw(pDC, pMainBlock);

	// 현재 실행되고 있는 Tool 이 있을 경우.
	if(IsActionOfTool(pMainBlock))
		m_pBaseTool->OnDraw(pDC, pMainBlock);

	if(m_pOldBaseTool != NULL)
		m_pOldBaseTool->OnDraw(pDC, pMainBlock);
}


// private ====================================================================
void CMainBlockToolImp::Invalidate()
{
	if(m_pIChartCtrl != NULL)
		m_pIChartCtrl->InvalidateOCX();
}

// 모든 tool의 값을 초기화 한다.
void CMainBlockToolImp::InitializeAllData(const bool bInProcessEnded)
{
	if(m_pBaseTool != NULL){
		if(!bInProcessEnded && !m_pBaseTool->DoesProcessEnded())
			return;
		m_pBaseTool->InitializeAllData();
	}
		
	m_pBaseTool = NULL;
	m_pOldBaseTool = NULL;
}

// tool 을 실행하다.
bool CMainBlockToolImp::IsActionOfTool(CMainBlock* pMainBlock) const
{
	return (pMainBlock != NULL && m_pBaseTool != NULL && m_pBaseTool->GetIChartCtrl() != NULL);
}

// tool 선택에 관한 유무.
bool CMainBlockToolImp::IsSelectingTool(CMainBlock* pMainBlock) const
{
	if(pMainBlock == NULL)
		return false;

	if(!pMainBlock->GetDesignMode())
		return true;

	return (pMainBlock->GetBlockCompartType() == CBlockBaseData::COMPARTLINE);
}

// 현재의 Tool이 scale Tool 인지의 유무
bool CMainBlockToolImp::IsScaleTool(CBaseTool* pBaseTool) const
{
	if(pBaseTool == NULL)
		return false;
	
	return (pBaseTool->GetToolType() == CBlockBaseData::TOOL_HORZSCALE || pBaseTool->GetToolType() == CBlockBaseData::TOOL_VERTSCALE);
}

// ----------------------------------------------------------------------------
// 현재 사용되는 CBaseTool을 m_pBaseTool에 setting.
void CMainBlockToolImp::SetCurrentBaseTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useScaleTool)
{
	if(!IsSelectingTool(pMainBlock))
		return;

	InitializeAllData(true);

	m_pBaseTool = m_toolSingleton.GetTool(pDC, pMainBlock, point, useScaleTool);
}

void CMainBlockToolImp::SetCurrentBaseTool_OnRButton(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useScaleTool)
{
	if(m_pBaseTool != NULL && !IsScaleTool(m_pBaseTool))
		return;

	SetCurrentBaseTool(pDC, pMainBlock, point, useScaleTool);
}

// ----------------------------------------------------------------------------
bool CMainBlockToolImp::OnMouseMove_ProcessFind(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	CBaseTool* pBaseTool = m_toolSingleton.GetTool(pDC, pMainBlock, point, false, false);

	if(m_pOldBaseTool != pBaseTool)
		Invalidate();

	m_pOldBaseTool = pBaseTool;
	if(pBaseTool == NULL)
		return false;

	pBaseTool->OnMouseMove(pDC, pMainBlock, nFlags, point);
	return true;
}
