// Tool.cpp: implementation of the CTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tool.h"

#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CBaseTool

CBaseTool::CBaseTool():
	m_pIChartCtrl(NULL),
	m_bProcessEnded(true)
{
}

void CBaseTool::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_pIChartCtrl = p_pICharetCtrl;
}

IChartCtrl* CBaseTool::GetIChartCtrl() const
{
	return m_pIChartCtrl;
}

CWnd* CBaseTool::GetParent() const
{
	if(m_pIChartCtrl == NULL)
		return NULL;

	return m_pIChartCtrl->GetChartWindowPointer();
}

HWND CBaseTool::GetParent_Handle()
{
	if(m_pIChartCtrl == NULL)
		return NULL;

	return m_pIChartCtrl->GetWndHandle();
}


// virtual ====================================================================
// Tool의 작업 완료여부 (true = 작업완료)
bool CBaseTool::DoesProcessEnded() const
{
	return m_bProcessEnded;
}

void CBaseTool::InitializeAllData()
{
	if(m_pIChartCtrl != NULL && !m_bProcessEnded)
		m_pIChartCtrl->InvalidateOCX();

	ChangeCursor(CBaseToolEnumData::CTNONE);
	m_bProcessEnded = true;
}

// ----------------------------------------------------------------------------
bool CBaseTool::OnLButtonDown(CDC* /*pDC*/, CMainBlock* /*pMainBlock*/, const UINT /*nFlags*/, const CPoint& /*point*/)
{
	m_bProcessEnded = true;
	return false;
}

bool CBaseTool::OnLButtonUp(CDC* /*pDC*/, CMainBlock* /*pMainBlock*/, const CPoint& /*point*/)
{
	m_bProcessEnded = true;
	return false;
}

bool CBaseTool::OnLButtonDblClk(CDC* /*pDC*/, CMainBlock* /*pMainBlock*/, const CPoint& /*point*/)
{
	return false;
}

CMenuType::MENUTYPE CBaseTool::OnRButtonUp(CDC* /*pDC*/, CMainBlock* /*pMainBlock*/, const CPoint& /*point*/)
{
	InitializeAllData();
	return CMenuType::MENU_NONE;
}

void CBaseTool::OnMouseMove(CDC* /*pDC*/, CMainBlock* /*pMainBlock*/, const UINT /*nFlags*/, const CPoint& /*point*/)
{
}

bool CBaseTool::OnMenu(CMainBlock* /*pMainBlock*/, const CMenuType::ELEMENTMENU /*menuType*/, const WPARAM /*wParam*/)
{
	InitializeAllData();
	return false;
}

// 지표 뒤집기 - ojtaso (20071023)
bool CBaseTool::GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	return false;
}

void CBaseTool::OnDraw(CDC* /*pDC*/, CMainBlock* /*pMainBlock*/)
{
}


// protected ==================================================================
// mouse의 위치에 따라 커서의 모양을 변경한다.
/*void CBaseTool::ChangeCursor(const CBaseToolEnumData::CURSORTYPE& cursorType)
{
	if(m_pParent == NULL)
		return;
	
	if(cursorType == CBaseToolEnumData::SPLITE_ROW)
		m_pParent->SendMessage(UM_CURSOR_EXCHANGE, CCursorShape::BLOCKPOS_SPLITH);
	else if(cursorType == CBaseToolEnumData::SPLITE_COLUMN)
		m_pParent->SendMessage(UM_CURSOR_EXCHANGE, CCursorShape::BLOCKPOS_SPLITV);
	else if(cursorType == CBaseToolEnumData::SPLITE_ALL)
		m_pParent->SendMessage(UM_CURSOR_EXCHANGE, CCursorShape::BLOCKPOS_SPLITA);
	else if(cursorType == CBaseToolEnumData::SELECT)
		m_pParent->SendMessage(UM_CURSOR_EXCHANGE, CCursorShape::OBJECT_SELECT);
	else if(cursorType == CBaseToolEnumData::MOVE)
		m_pParent->SendMessage(UM_CURSOR_EXCHANGE, CCursorShape::OBJECT_MOVE);
	else if(cursorType == CBaseToolEnumData::NODROP)
		m_pParent->SendMessage(UM_CURSOR_EXCHANGE, CCursorShape::TOOL_NODROP);
	else
		m_pParent->SendMessage(UM_CURSOR_EXCHANGE, CCursorShape::OBJECT_NONE);

}*/

void CBaseTool::ChangeCursor(const CBaseToolEnumData::CURSORTYPE& cursorType)
{
	if(m_pIChartCtrl == NULL)
		return;
	
	if(cursorType == CBaseToolEnumData::SPLITE_ROW)
		m_pIChartCtrl->SetCursorExchange(CCursorShape::BLOCKPOS_SPLITH);
	else if(cursorType == CBaseToolEnumData::SPLITE_COLUMN)
		m_pIChartCtrl->SetCursorExchange(CCursorShape::BLOCKPOS_SPLITV);
	else if(cursorType == CBaseToolEnumData::SPLITE_ALL)
		m_pIChartCtrl->SetCursorExchange(CCursorShape::BLOCKPOS_SPLITA);
	else if(cursorType == CBaseToolEnumData::SELECT)
		m_pIChartCtrl->SetCursorExchange(CCursorShape::OBJECT_SELECT);
	else if(cursorType == CBaseToolEnumData::MOVE)
		m_pIChartCtrl->SetCursorExchange(CCursorShape::OBJECT_MOVE);
	else if(cursorType == CBaseToolEnumData::NODROP)
		m_pIChartCtrl->SetCursorExchange(CCursorShape::TOOL_NODROP);
	else if(cursorType == CBaseToolEnumData::REMOVE)
		m_pIChartCtrl->SetCursorExchange(CCursorShape::OBJECT_REMOVE);
	// Graph 분리/삽입 - ojtaso (20070731)
	else if(cursorType == CBaseToolEnumData::INSERT)
		m_pIChartCtrl->SetCursorExchange(CCursorShape::OBJECT_INSERT);
	else
		m_pIChartCtrl->SetCursorExchange(CCursorShape::OBJECT_NONE);

}

void CBaseTool::SetProcessEnded(const bool isProcessEnded)
{
	m_bProcessEnded = isProcessEnded;
}

// ----------------------------------------------------------------------------
void CBaseTool::Invalidate(const bool bErase)
{
	if(m_pIChartCtrl != NULL)
		m_pIChartCtrl->InvalidateOCX(bErase);
}

void CBaseTool::InvalidateRect(const CRect& region, const bool bErase)
{
	if(m_pIChartCtrl == NULL || region.IsRectEmpty())
		return;

	m_pIChartCtrl->InvalidateRect(region, bErase);
}

// ----------------------------------------------------------------------------
/*int CBaseTool::GetMargin() const
{
	if(m_pParent == NULL || m_pParent->m_hWnd == NULL)
		return -1;

	return (int) m_pParent->SendMessage(UDM_GET_USERMAGIN);
}*/

int CBaseTool::GetMargin() const
{
	if(m_pIChartCtrl == NULL)
		return -1;

	return m_pIChartCtrl->GetUserMargin();
}
