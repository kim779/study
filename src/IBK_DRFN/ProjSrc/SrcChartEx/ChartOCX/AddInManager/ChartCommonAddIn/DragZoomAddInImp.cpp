// DragZoomAddInImp.cpp: implementation of the CDragZoomAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "DragZoomAddInImp.h"

#include "../../Include_Chart/MessageDefine.h"				// for UM_CURSOR_EXCHANGE
#include "../Include_AddIn/AddInCommand.h"					// for EAI_ALL_ON_CHANGE_CURSOR_MODE
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDragZoomAddInImp::CDragZoomAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// save point for inflate when mouse drag & drop
	// 09.21.2001
	m_ptStartDragDrop = CPoint(-1, -1);
	m_ptCurrDragDrop = CPoint(-1, -1);

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = UINT_MAX;

	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) ADDIN의 실행여부.
	m_bRun = FALSE;
}

CDragZoomAddInImp::~CDragZoomAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CDragZoomAddInImp::m_strAddInItemName = "DRAG_ZOOM";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CDragZoomAddInImp)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////

// (2006/2/6 - Seung-Won, Bae) Draw Trace Rectangle
CRect CDragZoomAddInImp::DrawDragRect(CDC * pdc, CPoint ptFirst, CPoint ptCurr, int nWeight)
{
	CRect rccurr;
	rccurr.SetRectEmpty();
	if( ptCurr == CPoint(-1, -1)) return rccurr;

	// 제대로 된 사각형 만들기
	CPoint ptStart, ptEnd;
	ptStart.x = ptFirst.x < ptCurr.x ? ptFirst.x : ptCurr.x;
	ptStart.y = ptFirst.y < ptCurr.y ? ptFirst.y : ptCurr.y;
	ptEnd.x = ptFirst.x > ptCurr.x ? ptFirst.x : ptCurr.x;
	ptEnd.y = ptFirst.y > ptCurr.y ? ptFirst.y : ptCurr.y;

	rccurr = CRect(ptStart, ptEnd);
	CRect rc = m_pIChartOCX->GetOcxRegion();
	rccurr.top = rc.top;
	rccurr.bottom = rc.bottom;

	SIZE size;
	size.cx = nWeight; size.cy = nWeight;

	pdc->DrawDragRect(rccurr, size, NULL, size);
	
	return rccurr;
}

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
RECT CDragZoomAddInImp::DrawCursor( HDC p_hDC)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	return DrawDragRect( pDC, m_ptStartDragDrop, m_ptCurrDragDrop, 4);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

void CDragZoomAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;

	if( eToolOption == CToolOptionInfo::T_DRAG_Y_ZOOM || eToolOption == CToolOptionInfo::T_PAN )
		m_bRun = FALSE;
	else if( eToolOption == CToolOptionInfo::T_DRAG_X_ZOOM)
		m_bRun = TRUE;
	// 2008.10.15 by LYH >> 패닝 (PAN, Y_ZOOM 해제 안 되는 현상때문에 SELECTION 될 경우 무조건 TRUE)
	else if( eToolOption == CToolOptionInfo::T_TOOL_SELECTION)
		m_bRun = TRUE;
	// 2008.10.15 by LYH <<
	else
		return;
	
	// (2006/11/25 - Seung-Won, Bae) Reset UI
	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_DRAG_X_ZOOM, m_bRun);
}



BOOL CDragZoomAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	if( !m_bRun ) return FALSE;

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
		if( p_dwMouseActionID != m_dwMouseActionID) return FALSE;
		if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
		m_ptCurrDragDrop = m_ptStartDragDrop;
	}

	if( m_ptStartDragDrop == point)	return TRUE;

	// Drag중
	m_ptCurrDragDrop = point;
	if( m_ptStartDragDrop.x > point.x)	// 전체보기모양 마우스커서 보이기.
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::WHOLE_VIEW, NULL);
	else	// 확대모양 마우스커서 보이기.
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::VIEW_ZOOM_IN, NULL);	// 확대커서.

	return TRUE;
}

BOOL CDragZoomAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	if( !m_bRun )
		return FALSE;

	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart!
	if( m_pIChartOCX->IsOnNoZoomChart()
		|| m_pIChartManager->IsEmpty()
		|| m_pIChartManager->IsEmptyData()) return FALSE;
	CRect rctRegion = m_pIChartManager->GetGraphRegionSumInColumn( point);
	if( rctRegion.IsRectEmpty()) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = p_dwMouseActionID;

	m_ptStartDragDrop = point;
	m_ptCurrDragDrop = CPoint(-1, -1);

	return FALSE;
}

BOOL CDragZoomAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	if( !m_bRun )
		return FALSE;

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase)) return FALSE;

	// save point & draw for inflate when mouse drag & drop
	// 09.21.2001	
	// MODIFY: 종찬(04/04/18) DoubleClick-Drag시 ZoomIn기능을 제거.
	// (2006/3/4 - Seung-Won, Bae) Make Real Effect Gap 5 Pixel.
	if( 5 <= abs(m_ptStartDragDrop.x - point.x))
	{
		//nami 8.30
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

		m_pIChartOCX->DragZoomIn( m_ptStartDragDrop, point);
	}

	m_ptStartDragDrop = CPoint(-1, -1);
	m_ptCurrDragDrop = CPoint(-1, -1);

	return TRUE;
}

