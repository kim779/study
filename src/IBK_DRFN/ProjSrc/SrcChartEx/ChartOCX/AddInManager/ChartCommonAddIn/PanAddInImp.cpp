// PanAddInImp.cpp: implementation of the CPanAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "PanAddInImp.h"

#include <math.h>											// for pow()
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

CPanAddInImp::CPanAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = UINT_MAX;

	// Mouse가 처음 눌러졌을 때의 위치
	m_ptPanStart = CPoint(-1, -1);
	m_dPanStart = 0;

	// 현재 ADDIn이 실행되어야 되는지 확인하는 Flag 
	m_bRun = FALSE;
	m_bLButtonDown = FALSE;

	m_nStartIndex = 0;
	m_nEndIndex = 0;
	
	m_dBarWidth = 0;
	m_dViewMin = 0;
	m_dViewMax = 0;

	m_bIsLock = FALSE;
	
	m_rctGraphDrawingRegion = CRect(0,0,0,0);

	// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
	m_nRow = -1;
	m_nColumn = -1;
	m_ptYZoomStart = CPoint( -1, -1);
	m_nVScaleIndex = -1;
	m_nScaleHeight = 0;
}


CPanAddInImp::~CPanAddInImp()
{

}

// Multiple Item in DLL
CString CPanAddInImp::m_strAddInItemName = "PAN";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CPanAddInImp)
	ONADDINEVENT( OnToolCommand )
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnRButtonUp)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 24
// Return void	: 
// Param  const int p_nToolType : 
// Param  const int p_nCmdOption : 
// Comments		: 
//-----------------------------------------------------------------------------
void CPanAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;

	// 1. 모든 Tool을 삭제하라는 Message를 받으면 List를 초기화한다.
	if( eToolOption == CToolOptionInfo::T_DRAG_X_ZOOM || eToolOption == CToolOptionInfo::T_DRAG_Y_ZOOM )
		m_bRun = FALSE;
	else if( eToolOption == CToolOptionInfo::T_PAN )
		m_bRun = TRUE;
	// 2008.10.15 by LYH >> 패닝 
	else if( m_bRun) m_bRun = FALSE;
	else return;
	// 2008.10.15 by LYH <<

	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_PAN, m_bRun);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 22
// Return BOOL	: 
// Param  HDC p_hDC : 
// Param  UINT nFlags : 
// Param  POINT &point : 
// Param  const DWORD p_dwMouseActionID : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CPanAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID )
{
	// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
	if( OnLButtonDownForYZoom( p_hDC, nFlags, point, p_dwMouseActionID))
		return TRUE;

	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if( !m_bRun )
		return FALSE;
	
	// 2. Check Empty Chart!
	if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty())
		return FALSE;
	
	// 3. 가격차트의 정보를 얻어온다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
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

	if( !m_rcGraphRegion.PtInRect(point) )
		return FALSE;

	m_rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 10. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);

	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);

	pIBlock->Release();
	pIBlock = NULL;

	// 6. Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = p_dwMouseActionID;

	// 7. 현재의 point를 Drag의 시작 point로 저장한다.
	m_ptPanStart = point;

	// 1. 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
		return FALSE;

	int nDataOnPage = m_nEndIndex - m_nStartIndex + 1;
	m_dBarWidth = double( m_rctGraphDrawingRegion.Width() ) / double( nDataOnPage );

	m_dPanStart = m_pIChartOCX->ConvertYPositionToData( m_ptPanStart.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

#ifdef _DEBUG
	TRACE( "PAN : BarWidth %f, StartIndex : %d, EndIndex : %d, YPointPrice : %f\n",
			m_dBarWidth, m_nStartIndex, m_nEndIndex, m_dPanStart );
#endif

	m_bLButtonDown = TRUE;
	
	return TRUE;
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
BOOL CPanAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
	if( OnMouseMoveForYZoom( p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB))
		return TRUE;

	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if( !m_bRun && !m_bLButtonDown )
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
	}


	// 5. 현재의 point가 Drag를 시작했던 point와 같으면 return한다.
	if( m_ptPanStart == point )
		return TRUE;

	// 7. 가격차트가 포함된 Block의 Index를 구한다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return TRUE;

	// 8. Use Block Interface
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock) return TRUE;

	// 10. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
//	BOOL bLog, bReverse;
//	pIBlock->GetLogAndReverseScaleFlag( bLog, bReverse);

//	// 5. Block에서 GraphResion을 얻어온다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions(m_rcGraphRegion, rctNoFrameAndMargin);
	if( m_rcGraphRegion.IsRectEmpty() )
		return FALSE;
	
	m_rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 4. GraphRegion이 넘어서면 영역을 GraphRegion으로 제한한다.
	if( m_rcGraphRegion.top >= point.y )
		point.y = m_rcGraphRegion.top;

	if( m_rcGraphRegion.bottom <= point.y )
		point.y = m_rcGraphRegion.bottom;

	// point와의 차이값으로 이동을 결정한다.
	int nXGap = m_ptPanStart.x - point.x;
	BOOL bInvalidate = FALSE;
	int nXMove = 0;
	if( abs(nXGap) > m_dBarWidth )
	{
		nXMove = (int)(nXGap / m_dBarWidth);

		int nLast;
		m_pIChartOCX->GetMaxScrollRange(nLast);

		if( (m_nStartIndex + nXMove >= 0) && (m_nEndIndex + nXMove <= nLast ) )
			m_pIChartOCX->SetViewStartEndIndex( m_nStartIndex + nXMove, m_nEndIndex + nXMove );
		bInvalidate = TRUE;
	}

	// Block에 설정값을 전달한다.
	if( !m_bIsLock )
	{
		double dDragCur = 0;
		dDragCur = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

		if( ( m_dViewMin + ( m_dPanStart - dDragCur ) > 0 ) && ( m_dPanStart - dDragCur ) != 0 )
		{
			m_dViewMin = m_dViewMin + ( m_dPanStart - dDragCur );
			m_dViewMax = m_dViewMax + ( m_dPanStart - dDragCur );
			pIBlock->SetVScaleMinMaxSetting( nV, m_bIsLock,
											 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
											 m_dViewMin, m_dViewMax);

			bInvalidate = TRUE;
		}
	}
		
	if( pIBlock )
		pIBlock->Release();

	pIBlock = NULL;

	// 6. Drag가 종료되었슴으로 Mouse Cursor를 변경한다.
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_MOVE, NULL);
	
#ifdef _DEBUG
	TRACE( "PAN-MOVE: nXMove:%d, StartPoint.x:%d, StartPoint.y:%d, point.x:%d, point.y:%d\n",
			nXMove, m_ptPanStart.x, m_ptPanStart.y, point.x, point.y );
#endif

	if( bInvalidate )
		m_pIChartOCX->InvalidateControl();

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
BOOL CPanAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
	if( OnLButtonUpForYZoom( p_hDC, nFlags, point, p_dwMouseActionID))
		return TRUE;

	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if( !m_bRun )
		return FALSE;

	// 2. DC를 얻어온다.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// 3. Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase))
		return FALSE;

	// 5. Make Real Effect Gap 5 Pixel.
	if( abs(m_ptPanStart.x - point.x) < m_dBarWidth )
		return TRUE;

	// 6. Drag가 종료되었슴으로 Mouse Cursor를 변경한다.
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	m_ptPanStart = CPoint(-1, -1);
	
	m_bLButtonDown = FALSE;

	return TRUE;
}

// (2008/12/11 - Seung-Won, Bae) for Y Zoom.
BOOL CPanAddInImp::OnLButtonDownForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( 0 < m_ptYZoomStart.x && 0 < m_ptYZoomStart.y) return FALSE;

	if( !m_pIChartManager->GetBlockIndex( point, m_nRow, m_nColumn)) return FALSE;
	IBlock *pIBlock = m_pIChartManager->GetBlock( m_nRow, m_nColumn);
	if( !pIBlock) return FALSE;

	CRect rctLeftScale, rctRightScale;
	pIBlock->GetVertScaleRegion( rctLeftScale, rctRightScale);

	m_nVScaleIndex = pIBlock->GetActiveVScaleIndex( rctLeftScale.PtInRect( point) ? CScaleBaseData::VERT_LEFT : CScaleBaseData::VERT_RIGHT);
	if( 0 <= m_nVScaleIndex)
	{
		CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
		pIBlock->GetVScaleMinMaxSetting( m_nVScaleIndex, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);
	}
	pIBlock->Release();

	if( m_bIsLock || m_nVScaleIndex < 0 || ( !rctLeftScale.PtInRect( point) && !rctRightScale.PtInRect( point))) return FALSE;
	m_nScaleHeight = rctLeftScale.Height();
	m_ptYZoomStart = point;

	m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase);
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::BLOCKPOS_SPLITH, NULL);

	return TRUE;
}
BOOL CPanAddInImp::OnMouseMoveForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if( m_ptYZoomStart.x < 0 && m_ptYZoomStart.y < 0) return FALSE;
	if( m_nRow < 0 || m_nColumn < 0) return TRUE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( m_nRow, m_nColumn);
	if( !pIBlock) return TRUE;

	double dViewMin = m_dViewMin;
	double dViewMax = m_dViewMax;
	if( m_dViewMin != m_dViewMax && point.y != m_ptYZoomStart.y)
	{
		double dViewRange = ( m_dViewMax - m_dViewMin) / 2;
		double dFactor = pow( 10, ( double( abs(point.y - m_ptYZoomStart.y)) / m_nScaleHeight) / ( 200. / 100.));
		if( m_ptYZoomStart.y < point.y) dViewRange /= dFactor;
		else							dViewRange *= dFactor;

		dViewMin = ( m_dViewMax + m_dViewMin) / 2  - dViewRange;
		dViewMax = ( m_dViewMax + m_dViewMin) / 2  + dViewRange;
	}

	pIBlock->SetVScaleMinMaxSetting( m_nVScaleIndex, m_bIsLock,
									 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
									 dViewMin, dViewMax);
	pIBlock->Release();

	m_pIChartOCX->InvalidateControl();	
	return TRUE;
}
BOOL CPanAddInImp::OnLButtonUpForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( m_ptYZoomStart.x < 0 && m_ptYZoomStart.y < 0) return FALSE;

	m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase);
	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	m_ptYZoomStart.x = -1;
	m_ptYZoomStart.y = -1;
	return TRUE;
}
// 2008.10.20 by LYH >> 오른쪽 마우스 눌르면 패닝 기능 풀어줌
BOOL CPanAddInImp::OnRButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if( !m_bRun) return FALSE;
	m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION, 1);
	return FALSE;
}
// 2008.10.20 by LYH <<
