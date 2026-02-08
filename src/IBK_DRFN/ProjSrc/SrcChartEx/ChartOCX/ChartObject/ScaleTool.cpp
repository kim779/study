// ScaleTool.cpp: implementation of the CScaleTool class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartObject.h"
#include "ScaleTool.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/Dll_Load/PacketList.h"		// 호가단위, 기준가 사용 - ojtaso (20070917)
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl
#include "../Include_Chart/ScaleDataList.h"
#include "BlocksColumn.h"
#include "BlockImp.h"
#include "GraphSearchInfo.h"							// for CGraphSearchInfo_IndicatorName
#include "DlgHorzScaleSet.h"							// for CDlgHorzScaleSet
#include "DlgVertScaleSet.h"							// for CDlgVertScaleSet
#include "MainBlockImp.h"								// for CMainBlockImp


///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CScaleTool

CScaleTool::CScaleTool():
	m_pBlock(NULL)
{
}

// public =====================================================================
// 모든 data를 초기화 한다.
void CScaleTool::InitializeAllData()
{
	CBaseTool::InitializeAllData();
	m_pBlock = NULL;
}

// ----------------------------------------------------------------------------
bool CScaleTool::OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point)
{
	CBaseTool::OnLButtonDown(pDC, pMainBlock, nFlags, point);
	InitializeAllData();
	return false;
}

CMenuType::MENUTYPE CScaleTool::OnRButtonUp(CDC* /*pDC*/, CMainBlock* pMainBlock, const CPoint& point)
{
	if(!Find(pMainBlock, point))
		return CMenuType::MENU_NONE;

	CBaseTool::SetProcessEnded(false);
	return OnRButtonUp();
}

bool CScaleTool::OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	CBaseTool::SetProcessEnded(true);	
	return OnMenu(pMainBlock, menuType, (int) wParam);
}

// ----------------------------------------------------------------------------
// 해당 scale 찾는다.
bool CScaleTool::Find(CMainBlock* pMainBlock, const CPoint& point)
{
	InitializeAllData();
	if(pMainBlock == NULL) return false;

	m_pBlock = ( CBlockImp *)pMainBlock->GetBlock( pMainBlock->FindBlockIndex( point));
	if(m_pBlock == NULL) return false;

	return FindScale(m_pBlock, point);
}


// protected ==================================================================
CBlockImp* CScaleTool::GetBlock() const
{
	return m_pBlock;
}


// private ====================================================================
bool CScaleTool::OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const int nMenuData)
{
	if(pMainBlock == NULL || m_pBlock == NULL || nMenuData < 0)
		return false;

	switch(menuType)
	{
	case CMenuType::MENU_SET:	// 설정 Dlg
		return OnMenu_ScaleSet(pMainBlock, nMenuData);
	case CMenuType::MENU_HORZSCALE_POSITION:
		return OnMenu_HorzScalePosition(pMainBlock, (CScaleBaseData::HORZSCALEPOSITION) nMenuData);
	}

	return false;
}

bool CScaleTool::OnMenu_HorzScalePosition(CMainBlock* pMainBlock, const CScaleBaseData::HORZSCALEPOSITION horzPosition)
{
	if(m_pBlock == NULL)
		return false;

	// x축 위치 변경.
	// -> HORZ_HIDE(숨김) : block 갯수가 1개일 경우는 HORZ_NONE(없음)으로 변경한다.
	if(horzPosition == CScaleBaseData::HORZ_HIDE)
	{
		// block 갯수가 1개일 경우는 grid line도 숨김.
		if(pMainBlock != NULL && pMainBlock->GetRowCount() == 1)
		{
			// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
			//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
			m_pBlock->GetMainBlock()->SetHorzScalePosition( m_pBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_NONE);
			return true;
		}
	}

	// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
	//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	m_pBlock->GetMainBlock()->SetHorzScalePosition( m_pBlock->GetBlockData().GetHorzScale(), horzPosition);

	return true;
}


///////////////////////////////////////////////////////////////////////////////
// class CHorzScaleTool

CHorzScaleTool::CHorzScaleTool():
	CScaleTool()
{
}


// public =====================================================================
CBlockBaseData::TOOLTYPE CHorzScaleTool::GetToolType() const
{
	return CBlockBaseData::TOOL_HORZSCALE;
}


// protected ==================================================================
bool CHorzScaleTool::OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point)
{
	return OnMenu_ScaleSet(pMainBlock, CBaseTool::GetMargin());
}

CMenuType::MENUTYPE CHorzScaleTool::OnRButtonUp()
{
	return CMenuType::MENU_HORZ_SCALE;
}

bool CHorzScaleTool::OnMenu_ScaleSet(CMainBlock* pMainBlock, const int nMarginCount)
{
	assert(pMainBlock != NULL);

	CHorizontalScalePtrDataList horzScalePtrDataList( pMainBlock);
	GetHorzScalePtrDataList(pMainBlock, nMarginCount, horzScalePtrDataList);
	if(horzScalePtrDataList.GetCount() <= 0)
		return false;

	// (2007/7/10 - Seung-Won, Bae) Use Horizontal Scale Setting UI Directly.
	//	CChartObjectWrapperHelper wrapperHelper(pMainBlock->GetOCXPath());
	//	// xScaleManager 추가 : xScaleManager - ojtaso (20070525)
	//	return wrapperHelper.SetHorzScale(CBaseTool::GetParent(), &horzScalePtrDataList, pMainBlock->GetXScaleManager());
	CWnd *pParent = CBaseTool::GetParent();
	if( pParent == NULL || horzScalePtrDataList.GetCount() <= 0) return false;
	_MHWND( m_pIChartCtrl->GetWndHandle());
	ML_SET_LANGUAGE_RES();
	CDlgHorzScaleSet dlg( pParent, pMainBlock, &horzScalePtrDataList, pMainBlock->GetXScaleManager());
	dlg.DoModal();
	return true;
}

bool CHorzScaleTool::FindScale(CBlockImp* pBlock, const CPoint& point)
{
	assert(pBlock != NULL);
	return pBlock->GetVertScaleGroupList().GetDrawingRegion().GetHorzScaleRegion().PointInRegion(point);
}


// private ====================================================================
void CHorzScaleTool::GetHorzScalePtrDataList(CMainBlock* pMainBlock, const int nMarginCount, CHorizontalScalePtrDataList& horzScalePtrDataList)
{
	horzScalePtrDataList.RemoveAll();

	CScaleColor* pScaleColor = &pMainBlock->GetScaleColor();
	if(pScaleColor == NULL || CScaleTool::GetBlock() == NULL || nMarginCount < 0)
		return;

	if(nMarginCount > 0)
		horzScalePtrDataList.SetMarginCount(nMarginCount);
	horzScalePtrDataList.SetScaleColor(&pScaleColor->GetHorzScaleColor(), &pScaleColor->GetScaleTextColor());
	CScaleTool::GetBlock()->GetBlocksColumn()->GetHorzScalePtrDataList(CScaleTool::GetBlock(), horzScalePtrDataList);
}


///////////////////////////////////////////////////////////////////////////////
// class CVertScaleTool

CVertScaleTool::CVertScaleTool():
	CScaleTool()
{
}

// public =====================================================================
CBlockBaseData::TOOLTYPE CVertScaleTool::GetToolType() const
{
	return CBlockBaseData::TOOL_VERTSCALE;
}


// protected ==================================================================
bool CVertScaleTool::OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point)
{
	// (2007/7/13 - Seung-Won, Bae) Reset Selected VScale with Double Click or Popup Menu.
	m_ptSelectedPoint = point;
	if( !OnMenu_ScaleSet( pMainBlock, 0)) return false;
	return true;
}

// (2007/7/13 - Seung-Won, Bae) Reset Selected VScale with Double Click or Popup Menu.
CMenuType::MENUTYPE CVertScaleTool::OnRButtonUp( CDC* pDC, CMainBlock* pMainBlock, const CPoint& point)
{
	m_ptSelectedPoint = point;
	return CScaleTool::OnRButtonUp( pDC, pMainBlock, point);
}

CMenuType::MENUTYPE CVertScaleTool::OnRButtonUp()
{
	assert(CScaleTool::GetBlock() != NULL);

	// x축 scale 이 <숨김>,<없음>일 경우 x축 scale 메뉴도 보임
	CScaleBaseData::HORZSCALEPOSITION horzScalePosition = CScaleTool::GetBlock()->GetBlockData().GetHorzScale().GetHorzScalePosition();
	if(horzScalePosition == CScaleBaseData::HORZ_HIDE || horzScalePosition == CScaleBaseData::HORZ_NONE)
		return CMenuType::MENU_VERT_HORZ_SCALE;

	return CMenuType::MENU_VERT_SCALE;
}

bool CVertScaleTool::OnMenu_ScaleSet(CMainBlock* pMainBlock, const int /*nMarginCount*/)
{
	if( !pMainBlock) return false;

	CBlockImp *pBlock = ( CBlockImp *)pMainBlock->GetBlock( pMainBlock->FindBlockIndex( m_ptSelectedPoint));
	if( pBlock == NULL) return false;
	CVertScaleGroupList &vertScaleGroupList = pBlock->GetVertScaleGroupList();

	CGraphSearchInfo_ActiveVertScaleAndGridLine gsi;
	gsi.m_nActiveFlag = ( vertScaleGroupList.GetDrawingRegion().GetVertScaleRegion().GetLeftScaleRegion().PtInRect( m_ptSelectedPoint) ? VSGA_LEFTSCALE : VSGA_RIGHTSCALE);

	// Y Scale없음인 경우 Y Scale 설정창 안나오던 버그 수정 - ojtaso (20071115)
	bool bFind = vertScaleGroupList.FindGraph( &gsi);
	ShowVertScaleSettingDlg( pMainBlock, gsi.m_nVertScaleGroupIndex);

	return bFind;
}

bool CVertScaleTool::ShowVertScaleSettingDlg( CMainBlock* pMainBlock, const int p_nSelectedIndex)
{
	assert(pMainBlock != NULL);

	CVerticalScalePtrDataList vertScalePtrDataList;
	GetVertScalePtrDataList(pMainBlock, vertScalePtrDataList);
	if(vertScalePtrDataList.GetCount() <= 0)
		return false;

	// (2007/7/13 - Seung-Won, Bae) Reset Selected VScale with Double Click or Popup Menu.
	if( 0 <= p_nSelectedIndex) vertScalePtrDataList.SetFirstIndex( p_nSelectedIndex);

	// (2007/7/10 - Seung-Won, Bae) Use Horizontal Scale Setting UI Directly.
	//	CChartObjectWrapperHelper wrapperHelper(pMainBlock->GetOCXPath());
	//	if(!wrapperHelper.SetVertScale(CBaseTool::GetParent(), &vertScalePtrDataList)) return false;
	CWnd *pParent = CBaseTool::GetParent();
	if( pParent == NULL || vertScalePtrDataList.GetCount() <= 0) return false;

	// 호가단위, 기준가 사용 - ojtaso (20070917)
	BOOL bHogaUnit = FALSE;
	CString strValue;
	bHogaUnit = pMainBlock->GetPacketList()->GetHelpMessageItem(_T("HOGAUNIT"), strValue);

	_MHWND( pParent->GetSafeHwnd());
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	ML_SET_LANGUAGE_RES();
	CDlgVertScaleSet dlg( pParent, &vertScalePtrDataList, bHogaUnit);
	dlg.DoModal();

	SendMessage_VertScaleSetting();
	return true;
}

bool CVertScaleTool::FindScale(CBlockImp* pBlock, const CPoint& point)
{
	assert(pBlock != NULL);
	return pBlock->GetVertScaleGroupList().GetDrawingRegion().GetVertScaleRegion().PointInRegion(point);
}


// private ====================================================================
void CVertScaleTool::GetVertScalePtrDataList(CMainBlock* pMainBlock, CVerticalScalePtrDataList& vertScalePtrDataList)
{
	vertScalePtrDataList.RemoveAll();
	
	CScaleColor* pScaleColor = &pMainBlock->GetScaleColor();
	if(pScaleColor == NULL || CScaleTool::GetBlock() == NULL)
		return;

	vertScalePtrDataList.SetScaleColor( &pScaleColor->GetVertScaleColor(), &pScaleColor->GetScaleTextColor());
	vertScalePtrDataList.SetLogType( &CScaleTool::GetBlock()->GetBlockData().GetVertScaleLogType());
	CScaleTool::GetBlock()->GetVertScaleGroupList().GetVertScalePtrDataList(vertScalePtrDataList);
}

// ----------------------------------------------------------------------------
// 선택된 Graph의 Type을 ocx에 메시지로 보낸다.
// -> "가격차트" 인 경우만 보낸다.
void CVertScaleTool::SendMessage_VertScaleSetting()
{
	if(CBaseTool::GetIChartCtrl() == NULL || CScaleTool::GetBlock() == NULL)
		return;

	// "가격차트" 인 경우만 보낸다.
	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
	if( !CScaleTool::GetBlock()->GetVertScaleGroupList().FindGraph( &gsi)) return;

	bool bLog = CScaleTool::GetBlock()->GetBlockData().GetVertScaleLogType();
	CBaseTool::GetIChartCtrl()->OnScaleSettingMessage( CCommonInterfaceEnum::PRICE_LOG, (int)bLog);
}
