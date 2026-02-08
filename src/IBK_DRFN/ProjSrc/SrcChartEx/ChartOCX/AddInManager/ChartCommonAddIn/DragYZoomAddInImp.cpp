// DragYZoomAddInImp.cpp: implementation of the CDragYZoomAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "DragYZoomAddInImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/MessageDefine.h"				// for UM_CURSOR_EXCHANGE
#include "../Include_AddIn/AddInCommand.h"					// for EAI_ALL_ON_CHANGE_CURSOR_MODE
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDragYZoomAddInImp::CDragYZoomAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_ptStartDragDrop = CPoint(-1, -1);
	m_ptCurrDragDrop = CPoint(-1, -1);
	m_rcPrevDrag = CRect(0,0,0,0);
	
	// Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = UINT_MAX;

	m_bRun = FALSE;
}

CDragYZoomAddInImp::~CDragYZoomAddInImp()
{

}


// Multiple Item in DLL
CString CDragYZoomAddInImp::m_strAddInItemName = "DRAG_Y_ZOOM";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CDragYZoomAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnToolCommand )
END_ADDIN_EVENT_MAP()


void CDragYZoomAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;

	// 1. 모든 Tool을 삭제하라는 Message를 받으면 List를 초기화한다.
	if( eToolOption == CToolOptionInfo::T_DRAG_X_ZOOM || eToolOption == CToolOptionInfo::T_PAN )
		m_bRun = FALSE;

	else if( eToolOption == CToolOptionInfo::T_DRAG_Y_ZOOM )
		m_bRun = TRUE;
	// 2008.10.15 by LYH >> 패닝
	else if( eToolOption == CToolOptionInfo::T_TOOL_SELECTION)
	{
		m_bRun = FALSE;
		return;
	}
	// 2008.10.15 by LYH <<
	else
		return;

	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_DRAG_Y_ZOOM, m_bRun);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 23
// Return void	: 
// Param  HDC p_hDC : 
// Comments		: Drag시 Rect를 그려준다.
//-----------------------------------------------------------------------------
void CDragYZoomAddInImp::OnDrawAfterGraph( HDC p_hDC )
{
	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if( !m_bRun )
		return;

	// 2. DC를 얻어온다.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC ) return;

	// 3. Mouse Drag Inflate 중이었으면 실행
	if( m_ptStartDragDrop != CPoint(-1, -1) )
		pDC->DrawDragRect( m_rcPrevDrag, CSize( 4, 4), NULL, CSize( 4, 4));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 23
// Return BOOL	: 
// Param  CDC * pdc : 
// Param  CPoint ptFirst : 
// Param  CPoint ptPrev : 
// Param  CPoint ptCurr : 
// Param  int nWeight : 
// Param  BOOL bMouseMove : 
// Comments		: Draw Trace Rectangle
//-----------------------------------------------------------------------------
BOOL CDragYZoomAddInImp::DrawDragRect(CDC * pdc, CPoint ptFirst, CPoint ptPrev, CPoint ptCurr, int nWeight, BOOL bMouseMove)
{
	// 1. ptFirst와 ptPrev의 값이 초기값 상태이면 동작하지 않는다.
	if( ptFirst == CPoint(-1, -1) || ptPrev == CPoint(-1, -1) )
		 return FALSE;

	// 2. 제대로 된 사각형 만들기
	CPoint ptStart, ptEnd;
	ptStart.x = ( ptFirst.x < ptCurr.x ) ? ptFirst.x : ptCurr.x;
	ptStart.y = ( ptFirst.y < ptCurr.y ) ? ptFirst.y : ptCurr.y;
	ptEnd.x = ( ptFirst.x > ptCurr.x ) ? ptFirst.x : ptCurr.x;
	ptEnd.y = ( ptFirst.y > ptCurr.y ) ? ptFirst.y : ptCurr.y;

	// 3. 조정한 point로 사각형을 만들어준다. YZoom은 Left와 Right를 OCX Region과 같이 한다.
	CRect rcCurr = CRect( ptStart, ptEnd );
	CRect rc = m_pIChartOCX->GetOcxRegion();
	rcCurr.left = rc.left;
	rcCurr.right = rc.right;

	// 4. MouseMove가 아니면 Rect를 0으로 만든다.
	if( bMouseMove == FALSE )
		rcCurr = CRect(0,0,0,0);
	
	// 5. Rect를 그려준다.
	SIZE size;
	size.cx = nWeight; size.cy = nWeight;
	pdc->DrawDragRect(rcCurr, size, m_rcPrevDrag, size);
	
	m_rcPrevDrag = rcCurr;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 22
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CDragYZoomAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if( !m_bRun )
		return FALSE;
	
	// 2. Check Empty Chart!
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty())
		return FALSE;
	
	// 3. 가격차트의 정보를 얻어온다.
	int nR, nC, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, m_nVertScaleGroupIndex, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	// 4. Block 정보를 얻어온다.
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
	
	// 5. Block에서 GraphResion을 얻어온다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions(m_rcGraphRegion, rctNoFrameAndMargin);
	if( m_rcGraphRegion.IsRectEmpty() )
		return FALSE;

	pIBlock->Release();
	pIBlock = NULL;

	if( !m_rcGraphRegion.PtInRect(point) )
		return FALSE;

	// 6. Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = p_dwMouseActionID;

	// 7. 현재의 point를 Drag의 시작 point로 저장한다.
	m_ptStartDragDrop = point;

	return FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 22
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Param  const HBITMAP p_hbmpMainOSB : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CDragYZoomAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if( !m_bRun )
		return FALSE;

	// 2. DC를 얻어온다.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// 3. Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
		if( p_dwMouseActionID != m_dwMouseActionID)
			return FALSE;
		
		if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase))
			return FALSE;
		
		m_ptCurrDragDrop = m_ptStartDragDrop;
	}

	// 4. GraphRegion이 넘어서면 영역을 GraphRegion으로 제한한다.
	if( m_rcGraphRegion.top >= point.y )
		point.y = m_rcGraphRegion.top;

	if( m_rcGraphRegion.bottom <= point.y )
		point.y = m_rcGraphRegion.bottom;

	// 5. 현재의 point가 Drag를 시작했던 point와 같으면 return한다.
	if( m_ptStartDragDrop == point )
		return TRUE;

	// 6. Drag 중이므로 Rect를 그려준다.
	DrawDragRect( pDC, m_ptStartDragDrop, m_ptCurrDragDrop, point, 4 );
	
	// 7. 현재의 point를 저장한다.
	m_ptCurrDragDrop = point;
	
	// 8. 현재의 MousePoint에 따라 Cursor의 모양을 변경한다.
	if( m_ptStartDragDrop.x > point.x)	
	{
		// 전체보기모양 마우스커서 보이기.
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, (WPARAM)CCursorShape::WHOLE_VIEW, NULL );
	}
	
	else	
	{
		// 확대모양 마우스커서 보이기.
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, (WPARAM)CCursorShape::VIEW_ZOOM_IN, NULL );	// 확대커서.
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 23
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CDragYZoomAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if( !m_bRun )
		return FALSE;

	// 2. DC를 얻어온다.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// 3. Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase))
		return FALSE;

	// 4. Drag하고 있는 Rect를 그려준다.
	DrawDragRect( pDC, m_ptStartDragDrop, m_ptCurrDragDrop, point, 4, FALSE);

	// 5. Make Real Effect Gap 5 Pixel.
	if( abs(m_ptStartDragDrop.x - point.x) < 5)
		return TRUE;

	// 6. Drag가 종료되었슴으로 Mouse Cursor를 변경한다.
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	// 7. 가격차트가 포함된 Block의 Index를 구한다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return TRUE;

	// 8. Use Block Interface
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock) return TRUE;
		
	BOOL bIsLock;
	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	double dViewMin, dViewMax;
	pIBlock->GetVScaleMinMaxSetting( nV, bIsLock, eMinMaxType, dViewMin, dViewMax);
	
	CRect rctGraphRegion, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( rctGraphRegion, rctNoFrameAndMargin );

	// 10. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	BOOL bLog = pIBlock->IsLogScale();
	BOOL bReverse = pIBlock->IsVScaleInvert( m_nVertScaleGroupIndex);
	
	// 11. 
	CRect rctGraphDrawingRegion;
	rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
	
	double dDragStart, dDragEnd;
	dDragStart = m_pIChartOCX->ConvertYPositionToData( m_ptStartDragDrop.y, rctGraphDrawingRegion.top, rctGraphDrawingRegion.bottom, dViewMax, dViewMin, bLog, bReverse);
	dDragEnd = m_pIChartOCX->ConvertYPositionToData( m_ptCurrDragDrop.y, rctGraphDrawingRegion.top, rctGraphDrawingRegion.bottom, dViewMax, dViewMin, bLog, bReverse);

	// Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	if( bReverse)
	{
		double nTemp	= dDragStart;
		dDragStart		= dDragEnd;
		dDragEnd		= nTemp;
	}
	
	// Block에 설정값을 전달한다.
	if( !bIsLock )
	{
		pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, bIsLock,
										 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
										 dDragEnd, dDragStart);
	}

	pIBlock->Release();
	pIBlock = NULL;

	m_pIChartOCX->InvalidateControl();

	m_ptStartDragDrop = CPoint(-1, -1);
	m_ptCurrDragDrop = CPoint(-1, -1);

	return TRUE;
}
