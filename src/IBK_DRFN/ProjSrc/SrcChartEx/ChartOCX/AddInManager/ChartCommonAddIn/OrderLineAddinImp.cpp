// OrderLineAddinImp.cpp: implementation of the COrderLineAddinImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderLineAddInImp.h"
#include "ChartCommonAddIn.h"
#include <math.h>


#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/MessageDefine.h"				// for UM_CURSOR_EXCHANGE
#include "../Include_AddIn/AddInCommand.h"					// for EAI_ALL_ON_CHANGE_CURSOR_MODE
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock

#define CONCLUSION_YES 0   //체결
#define CONCLUSION_NO  1   //미체결

#define ORDERTYPE_MARKET 0 //시장가
#define ORDERTYPE_ENTRY  1 //지정가
#define ORDERTYPE_CHANGE 2 //정정
#define ORDERTYPE_CANCEL 3 //취소

//////////////////////////////////////////////////////
///COrderLine
///////////////////////////////////////////////////////
COrderLine::COrderLine()
{
	m_nIndex        = 0;
	m_eLinePosition = LINE_POS_IN;
	m_bShow			= TRUE;
	m_bMouseOver	= FALSE;
	m_bSelected		= FALSE;
	m_ptStart		= CPoint(0,0);
	m_ptEnd			= CPoint(0,0);
	m_ptTableOffset = CPoint(0,0);

	m_ptStartChgLine= CPoint(0,0);
	m_ptEndChgLine  = CPoint(0,0);
	m_ptBackupTalbeOffset	= CPoint(0,0);
	m_ptLineOffset			= CPoint(0,0);
	m_dOrderPrice	= 0.0;
	m_nPanOffset	= 0;

	m_nSymbol			= 0;
	m_nFlagSellorBuy	= 0;
	m_dAmount			= 0.0;
	m_nOrderType		= 0;
	m_dRate				= 0.0;
	m_strDate			= "";
	m_strTime			= "";
	m_nCurClickedPos	= CLICKEDPOS_NONE;

	m_nPenStyle = PS_DOT;
	m_nPenThickness = 1;
	m_nPenClr = RGB(0, 0, 0);
	m_bCon = 0;   //미체결
	m_strOrderID = "";
}

COrderLine::COrderLine(CPoint ptPrev, CPoint ptEnd, double dOrderPrice)
{
	m_bShow			= TRUE;
	m_bSelected		= FALSE;
	m_bMouseOver	= FALSE;
	m_ptStart		= ptPrev;
	m_ptEnd			= ptEnd;
	m_dOrderPrice	= dOrderPrice;

	m_nSymbol			= 0;
	m_nFlagSellorBuy	= 0;
	m_dAmount			= 0;
	m_nOrderType		= 0;
	m_dRate				= 0.0;
	m_strDate			= "";
	m_strTime			= "";

	m_nPenStyle = PS_DOT;
	m_nPenThickness = 1;
	m_nPenClr = RGB(0, 0, 0);
}

COrderLine::~COrderLine()
{

}


CPoint COrderLine::GetPrevPoint()
{
	return (CPoint)m_ptStart;
}
CPoint COrderLine::GetEndPoint()
{
	return (CPoint)m_ptEnd;
}

void COrderLine::Draw(CDC* pDC)
{
	GetRect();
	if(!m_bShow)
		return;

	if(m_eLinePosition == LINE_POS_IN)
	{
	CPen pen(m_nPenStyle, m_nPenThickness, m_nPenClr);
	CPen *pOldPen = pDC->SelectObject( &pen);

	if(m_bMouseOver)
	{
		DrawOrderLineOnMouseOver(pDC);
	}
	else
	{
		DrawOrderLine(pDC);
	}
	pDC->SelectObject( pOldPen);

	CPen pen2(PS_SOLID, m_nPenThickness, m_nPenClr);
	CPen *pOldPen2 = pDC->SelectObject( &pen2);

	DrawChgLine(pDC);
	DrawOrderTable(pDC);
	DrawOrderText(pDC);

	pDC->SelectObject( pOldPen2);
	}
	else
	{	
		CPen pen(PS_SOLID, m_nPenThickness, COLOR_ORDERLINE);
		CPen *pOldPen = pDC->SelectObject( &pen);
		
		CBrush brush(COLOR_ORDERLINE);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		DrawEscapeTriangle(pDC);
	
		pDC->SelectObject( pOldPen);
		pDC->SelectObject( brush);
	}
	
	
}

void COrderLine::DrawOrderLine(CDC* pDC)
{
	pDC->MoveTo( m_ptStart);
	pDC->LineTo( m_ptEnd);	
}

void COrderLine::DrawOrderLineOnMouseOver(CDC* pDC)
{
	CPen pen(m_nPenStyle, m_nPenThickness, m_nPenClr);
	CPen *pOldPen = pDC->SelectObject( &pen);

	pDC->MoveTo( m_ptStart);
	pDC->LineTo( m_ptEnd);

	CPoint ptShadowPrev = m_ptStart;
	CPoint ptShadowEnd  = m_ptEnd;
	
	ptShadowPrev.y -= 1;
	ptShadowEnd.y -= 1;

	pDC->MoveTo( ptShadowPrev);
	pDC->LineTo( ptShadowEnd);

	ptShadowPrev.y += 2;
	ptShadowEnd.y += 2;

	pDC->MoveTo( ptShadowPrev);
	pDC->LineTo( ptShadowEnd);

	pDC->SelectObject( pOldPen);
}

void COrderLine::DrawChgLine(CDC* pDC)
{
	pDC->MoveTo( m_ptStartChgLine);
	pDC->LineTo( m_ptEndChgLine);
}

void COrderLine::DrawOrderText(CDC* pDC)
{
	pDC->DrawText("Entry(B)(10-01 07:10, 43.1)",m_rctText,DT_LEFT| DT_VCENTER);
}

void COrderLine::DrawOrderTable(CDC* pDC)
{
	pDC->Rectangle(m_rctTable);
}

void COrderLine::DrawEscapeTriangle(CDC* pDC)
{
	CPoint ptTriangle[3];

	if(m_eLinePosition == LINE_POS_OUT_UP)
	{
		ptTriangle[0].x = m_ptEscape_Start.x;
		ptTriangle[0].y = m_ptEscape_Start.y + TRIANGLE_HEIGHT;

		ptTriangle[1].x = m_ptEscape_Start.x + TRIANGLE_SIDE / 2;
		ptTriangle[1].y = m_ptEscape_Start.y ;

		ptTriangle[2].x = m_ptEscape_Start.x + TRIANGLE_SIDE;
		ptTriangle[2].y = m_ptEscape_Start.y + TRIANGLE_HEIGHT;
	}
	else if(m_eLinePosition == LINE_POS_OUT_DOWN)
	{
		ptTriangle[0].x = m_ptEscape_Start.x;
		ptTriangle[0].y = m_ptEscape_Start.y - TRIANGLE_HEIGHT;

		ptTriangle[1].x = m_ptEscape_Start.x + TRIANGLE_SIDE / 2;
		ptTriangle[1].y = m_ptEscape_Start.y;

		ptTriangle[2].x = m_ptEscape_Start.x + TRIANGLE_SIDE;
		ptTriangle[2].y = m_ptEscape_Start.y - TRIANGLE_HEIGHT;
	}
	pDC->Polygon(ptTriangle, 3);
}

void COrderLine::ShowOrderLine(CDC* pDC)
{

}

void COrderLine::HideOrderLine(CDC* pDC)
{
}

BOOL COrderLine::IsInRegion(CPoint pt)  
{
	if(IsOnLine(pt) || IsOnTable(pt))
		return TRUE;
	else 
		return FALSE;
}
BOOL COrderLine::IsOnLine(CPoint pt)    
{
	if(m_rctLine.PtInRect(pt))
		return TRUE;
	else 
		return FALSE;
}
BOOL COrderLine::IsOnTable(CPoint pt)  
{
	if(m_rctTable.PtInRect(pt))
		return TRUE;
	else 
		return FALSE;
}

CRect COrderLine::GetRect()
{
/*	m_rctLine.left  = m_ptStart.x;
	m_rctLine.right = m_ptEnd.x;
	m_rctLine.top   = m_ptStart.y -1;
	m_rctLine.bottom   = m_ptStart.y + 2;
*/
	m_rctLine.left  = m_ptStart.x;
	m_rctLine.right = m_ptEnd.x;
	m_rctLine.top   = m_ptStart.y -1;
	m_rctLine.bottom   = m_ptStart.y + 2;

	m_ptStartChgLine.x  = m_ptStart.x + 10  +  m_ptTableOffset.x;
	m_ptStartChgLine.y  = m_ptStart.y;          
	m_ptEndChgLine.x    = m_ptStartChgLine.x;
	m_ptEndChgLine.y    = m_ptStartChgLine.y - 15 + m_ptTableOffset.y;

	
	m_rctTable.left   = m_ptEndChgLine.x;
	m_rctTable.right  = m_rctTable.left + 170;
	m_rctTable.bottom = m_ptEndChgLine.y + 1;
	m_rctTable.top    = m_rctTable.bottom - 15;

	m_rctText = m_rctTable;
	m_rctText.left += 2;
	m_rctText.right -= 2;
	m_rctText.top += 2;
	m_rctText.bottom -= 2;

	return m_rct;
}

CPoint COrderLine::GetPostion()
{
	CPoint pt(0, 0);
	return pt;
}

void COrderLine::SetPosition(CPoint pt, int nType)
{
	if(m_nCurClickedPos == CLICKEDPOS_ORDERLINE)
	{
		m_ptStart.y = pt.y ;
		m_ptEnd.y = pt.y ;
	}
	else if(m_nCurClickedPos == CLICKEDPOS_TABLE)
	{
		m_ptTableOffset.x = pt.x - m_ptLBtnDown.x + m_ptBackupTalbeOffset.x;
		m_ptTableOffset.y = pt.y - m_ptLBtnDown.y + m_ptBackupTalbeOffset.y;

	}
}

void COrderLine::MouseLButtonDown(const CPoint &pt)
{
	m_ptLBtnDown = pt;

	if(	IsOnTable(pt) )
		m_nCurClickedPos = CLICKEDPOS_TABLE;
	else if( IsOnLine(pt) )
		m_nCurClickedPos = CLICKEDPOS_ORDERLINE;
	else
		m_nCurClickedPos = CLICKEDPOS_NONE;
}

void COrderLine::MouseLButtonUp(const CPoint &pt)
{
	m_ptBackupTalbeOffset = m_ptTableOffset;
	m_ptLineOffset  = CPoint(0,0);
}

void COrderLine::MouseRButtonUp(const CPoint &pt)
{
	if(	IsOnTable(pt) )
		m_nCurClickedPos = CLICKEDPOS_TABLE;
	else if( IsOnLine(pt) )
		m_nCurClickedPos = CLICKEDPOS_ORDERLINE;
	else
		m_nCurClickedPos = CLICKEDPOS_NONE;
}


//////////////////////////////////////////////////////
///COrderLineAddInImp
///////////////////////////////////////////////////////
COrderLineAddInImp::COrderLineAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
:CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_bRun		= FALSE;
	m_hAPCursor = NULL;
	m_bIsLock = FALSE;
	m_bSplit	= FALSE;

	m_dViewMin = 0;
	m_dViewMax = 0;

	m_bOnSetCaprture = FALSE;
	m_pSelectedOrderLine = NULL;

	m_nCount_EscapeUp	 = 0;
	m_nCount_EscapeDown  = 0;


	//PAN
	m_dwMouseActionID = UINT_MAX;
	m_ptPanStart   = CPoint(-1, -1);
	m_dPanStart    = 0;
	m_nStartIndex  = 0;
	m_nEndIndex    = 0;
	m_bLButtonDown = FALSE;
	m_dBarWidth    = 0.0;

	//YZOOM
	m_nRow    = -1;
	m_nColumn = -1;
	m_ptYZoomStart = CPoint( -1, -1);
	m_nVScaleIndex = -1;
	m_nScaleHeight = 0;
}

COrderLineAddInImp::~COrderLineAddInImp()
{
	POSITION pos = m_OrderLineList.GetHeadPosition();
	while (pos)
	{
		COrderLine* pLineOrder = (COrderLine*)m_OrderLineList.GetNext(pos);
		delete pLineOrder;
		pLineOrder = NULL;
	}
	m_OrderLineList.RemoveAll();
}

// Multiple Item in DLL
CString COrderLineAddInImp::m_strAddInItemName = "ORDER_LINE";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( COrderLineAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnRButtonUp)
	ONADDINEVENT( OnToolCommand )
	ONADDINEVENT( OnResetChart )
	ONADDINEVENT( OnRealData)
	

END_ADDIN_EVENT_MAP()

void COrderLineAddInImp::OnDrawAfterGraph( HDC p_hDC )
{
	if( !m_bRun )	return;

	// 2. "특수차트"일 때에는 보이지 않게 한다.
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// 3. Mouse의 position으로 Block의 영역을 구한다.
//	GetGraphBlockRegion();
	GetGraphVScaleRegion();

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	POSITION pos;
	COrderLine* pOrderLine;
	for (pos = m_OrderLineList.GetHeadPosition(); pos != NULL;)
	{
		pOrderLine = (COrderLine*)m_OrderLineList.GetNext(pos);
		if(!pOrderLine) return;

		CheckIsLineInScreen(pOrderLine);
		pOrderLine->Draw(pDC);		
	}	
}

void COrderLineAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	if( !m_bRun )	return;	
}

RECT COrderLineAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rt;
	rt.SetRectEmpty();
	
	if( !m_bRun )	return rt;

	return rt;
}


BOOL COrderLineAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if(!m_bRun)		return FALSE;

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
	pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
	if( m_rctGraphRegion.IsRectEmpty() )
		return FALSE;

	// 6. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 7. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	

	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);		
	
	
	if( m_rctGraphRegion.PtInRect( point) ) //그래프 영역안에서 동작
	{			
		COrderLine* pOrderLine_Find = NULL;
		POSITION pos = m_OrderLineList.GetHeadPosition();
		BOOL bOnLine = FALSE;
		while(pos)
		{
			pOrderLine_Find = (COrderLine*)m_OrderLineList.GetNext(pos);
			if(pOrderLine_Find && pOrderLine_Find->IsInRegion(point))
			{
				bOnLine = TRUE;
				m_pSelectedOrderLine = pOrderLine_Find;
				m_pSelectedOrderLine->MouseLButtonDown(point);
				
				m_bOnSetCaprture = TRUE;
				SetMouseCursor(IDC_MOVE4WAY);

				if(m_pSelectedOrderLine)
					m_pSelectedOrderLine->m_bSelected = FALSE;
				POSITION posCur = m_OrderLineList.Find((CObject*)m_pSelectedOrderLine);
				m_OrderLineList.RemoveAt(posCur);
				m_OrderLineList.AddTail((CObject*)m_pSelectedOrderLine);
				m_pSelectedOrderLine->m_bSelected = TRUE;				
				break;
			}
		}		
		if(bOnLine)
		return TRUE;
	}	
	double dYValue;
	dYValue = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	m_nSelecteYValue = point.y;

	
//	BOOL bDoOrder = FALSE;	
	
	if(m_nSelecteYValue < m_rctGraphRegion.top || m_nSelecteYValue > m_rctGraphRegion.bottom)
		return FALSE;

	if(/*bDoOrder &&*/ (m_rctLVScaleRegion.PtInRect( point) || m_rctRVScaleRegion.PtInRect( point)))//주문시 라인 드로우
	{
		//신규 주문요청
//		m_pIChartOCX->OnOrderInfomationChanged(dYValue, 10, ORDERTYPE_ENTRY, "");

// 		COrderLine* pOrderLine = new COrderLine();
// 		pOrderLine->m_ptStart = CPoint(m_rctGraphRegion.left, m_nSelecteYValue);
// 		pOrderLine->m_ptEnd = CPoint(m_rctGraphRegion.right, m_nSelecteYValue);
// 		pOrderLine->m_dOrderPrice = dYValue;
// 
// 		if(m_pSelectedOrderLine)
// 			m_pSelectedOrderLine->m_bSelected = FALSE;
// 		m_pSelectedOrderLine = pOrderLine;
// 		m_OrderLineList.AddTail((CObject*)m_pSelectedOrderLine);
// 		m_pSelectedOrderLine->m_bSelected = TRUE;
// 	
//   		m_pIChartOCX->InvalidateControl();
	}

	pIBlock->Release();
	pIBlock = NULL;	


	//PAN//////////	
	m_bLButtonDown = TRUE;
	if( OnLButtonDownForYZoom( p_hDC, nFlags, point, p_dwMouseActionID))
		return TRUE;

	m_dwMouseActionID = p_dwMouseActionID;
	m_ptPanStart = point;
	// 1. 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex ))
		return FALSE;

	int nDataOnPage = m_nEndIndex - m_nStartIndex + 1;
	m_dBarWidth = double( m_rctGraphDrawingRegion.Width() ) / double( nDataOnPage );

	m_dPanStart = m_pIChartOCX->ConvertYPositionToData( m_ptPanStart.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	/////////////////

	return TRUE;
}

BOOL COrderLineAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if(!m_bRun)		return FALSE;	

	if(m_bOnSetCaprture)
	{	
		m_hAPCursor = NULL;	
		if( m_pIChartOCX) m_pIChartOCX->SetCursorHandle( m_hAPCursor);
		if(m_pSelectedOrderLine)
			m_pSelectedOrderLine->MouseLButtonUp(point);
		m_bOnSetCaprture = FALSE;
		m_pIChartOCX->InvalidateControl();

		SetOrderInfomation();		
	}
	
	// 5. Make Real Effect Gap 5 Pixel.
//	if( abs(m_ptPanStart.x - point.x) < m_dBarWidth )
//		return TRUE;

	// 6. Drag가 종료되었슴으로 Mouse Cursor를 변경한다.
//	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	m_ptPanStart = CPoint(-1, -1);	
	m_bLButtonDown = FALSE;

	if( OnLButtonUpForYZoom( p_hDC, nFlags, point, p_dwMouseActionID))
		return TRUE;

	return TRUE;
}

BOOL COrderLineAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if(!m_bRun)		return FALSE;

	if(m_bSplit)
	{
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::BLOCKPOS_SPLITH, NULL);
	}

	if( OnMouseMoveForYZoom( p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB))
		return TRUE;	

	BOOL	 bLineOver = FALSE;
//1. 마우스 오버시
//2. 영역 밖일 경우 

	if(m_bOnSetCaprture)
	{	
		if(m_pSelectedOrderLine)
			m_pSelectedOrderLine->SetPosition(point);
	}
	else if(m_bLButtonDown) //PAN
	{	
		if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
		{
			if( p_dwMouseActionID != m_dwMouseActionID)
				return FALSE;		
			if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase))
				return FALSE;
		}
		//현재의 point가 Drag를 시작했던 point와 같으면 return한다.
		if( m_ptPanStart == point )
			return TRUE;

		//가격차트가 포함된 Block의 Index를 구한다.
		int nR, nC, nV, nG;
		ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
		if( szGraphName.IsEmpty()) return TRUE;

		IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
		if( !pIBlock) return TRUE;

		CRect rctNoFrameAndMargin;
		pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
		if( m_rctGraphRegion.IsRectEmpty() )
			return FALSE;
		
		m_rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
		if( m_rctGraphDrawingRegion.IsRectEmpty() )
			return FALSE;

		// 4. GraphRegion이 넘어서면 영역을 GraphRegion으로 제한한다.
		if( m_rctGraphRegion.top >= point.y )
			point.y = m_rctGraphRegion.top;

		if( m_rctGraphRegion.bottom <= point.y )
			point.y = m_rctGraphRegion.bottom;

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
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_MOVE, NULL);
		
	}
	else if( m_rctLVScaleRegion.PtInRect( point) || m_rctRVScaleRegion.PtInRect( point))
	{
		SetMouseCursor(IDC_AP_INDICATOR);
	}
	else
	{
		POSITION pos = m_OrderLineList.GetHeadPosition();
		COrderLine* pOrderLine = NULL;
		while(pos)
		{
			pOrderLine = (COrderLine*)m_OrderLineList.GetNext(pos);
			if(pOrderLine) 
			{
				if(pOrderLine->IsInRegion(point))
				{
					bLineOver = TRUE;
					pOrderLine->m_bShow = TRUE;
					pOrderLine->m_bMouseOver = TRUE;
					break;
				}
			}
		}

		if(bLineOver)
		{			
			POSITION posOtherLine = m_OrderLineList.GetHeadPosition();
			while(posOtherLine)
			{
				COrderLine* potherOrderLine = (COrderLine*)m_OrderLineList.GetNext(posOtherLine);
				if(pos != posOtherLine && potherOrderLine ) 
				{
					potherOrderLine->m_bShow = FALSE;
					potherOrderLine->m_bMouseOver = FALSE;
				}			
			}	
		}
		else if(!m_bOnSetCaprture)
		{
			POSITION posOtherLine = m_OrderLineList.GetHeadPosition();
			while(posOtherLine)
			{
				COrderLine* potherOrderLine = (COrderLine*)m_OrderLineList.GetNext(posOtherLine);
				if(potherOrderLine ) 
				{
					potherOrderLine->m_bShow = TRUE;
					potherOrderLine->m_bMouseOver = FALSE;
				}
			}		
		}
		m_hAPCursor = NULL;	
		if( m_pIChartOCX) m_pIChartOCX->SetCursorHandle( m_hAPCursor);
	}	
		

	m_pIChartOCX->InvalidateControl();
	return TRUE;
}

BOOL COrderLineAddInImp::OnLButtonDownForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
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
	m_bSplit = TRUE;
//	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::BLOCKPOS_SPLITH, NULL);

	return TRUE;
	}	

BOOL COrderLineAddInImp::OnMouseMoveForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
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

BOOL COrderLineAddInImp::OnLButtonUpForYZoom( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( m_ptYZoomStart.x < 0 && m_ptYZoomStart.y < 0) return FALSE;

	m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase);
//	m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

	m_ptYZoomStart.x = -1;
	m_ptYZoomStart.y = -1;

	m_bSplit = FALSE;
	return TRUE;
}

BOOL COrderLineAddInImp::OnRButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
//	m_bRun = !m_bRun;
//	m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION, 1);

//	m_pIChartOCX->OnOrderInfomationChanged(0.0, 0, 0, "");

	// 1. 현재 실행해야될지 확인하고 실행해야 될 경우에만 실행한다.
	if(!m_bRun)		return FALSE;

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
	pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
	if( m_rctGraphRegion.IsRectEmpty() )
		return FALSE;

	// 6. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	if( m_rctGraphDrawingRegion.IsRectEmpty() )
		return FALSE;

	// 7. 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	

	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, m_dViewMin, m_dViewMax);	
	
	
	double dYValue;
	dYValue = m_pIChartOCX->ConvertYPositionToData( point.y, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);
	m_nSelecteYValue = point.y;

	
//	BOOL bDoOrder = FALSE;	
	
	if(m_nSelecteYValue < m_rctGraphRegion.top || m_nSelecteYValue > m_rctGraphRegion.bottom)
		return FALSE;

	if(/*bDoOrder &&*/ (m_rctLVScaleRegion.PtInRect( point) || m_rctRVScaleRegion.PtInRect( point)))//주문시 라인 드로우
	{
		CMenu	menu;
		menu.CreatePopupMenu();

		int nID=100;
		menu.AppendMenu(MF_STRING, nID++, "Create Market Order"); 
		menu.AppendMenu(MF_STRING, nID++, "Create Entry  Order"); 

		UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
		int nRet = menu.TrackPopupMenu(flags, point.x + 115, point.y + 260, m_pChartCWnd);
		menu.DestroyMenu();

		switch(nRet) {
		case 100:
			{						
				m_pIChartOCX->OnOrderInfomationChanged(0, 10, ORDERTYPE_MARKET, "", "", "", "");
				break;
			}
		case 101:
			m_pIChartOCX->OnOrderInfomationChanged(dYValue, 10, ORDERTYPE_ENTRY, "", "", "", "");
			break;
		}
		
	}
	else
	{
		if( m_rctGraphRegion.PtInRect( point) ) //그래프 영역안에서 동작
		{	
			COrderLine* pOrderLine_Find = NULL;
			POSITION pos = m_OrderLineList.GetHeadPosition();
			BOOL bOnLine = FALSE;
			while(pos)
			{
				pOrderLine_Find = (COrderLine*)m_OrderLineList.GetNext(pos);
				if(pOrderLine_Find && pOrderLine_Find->IsInRegion(point))
				{
					bOnLine = TRUE;
					m_pSelectedOrderLine = pOrderLine_Find;			

					if(m_pSelectedOrderLine)
						m_pSelectedOrderLine->m_bSelected = FALSE;
					POSITION posCur = m_OrderLineList.Find((CObject*)m_pSelectedOrderLine);
					m_OrderLineList.RemoveAt(posCur);
					m_OrderLineList.AddTail((CObject*)m_pSelectedOrderLine);
					m_pSelectedOrderLine->m_bSelected = TRUE;				
					break;
				}
			}			
			
			if(!m_pSelectedOrderLine)
				return TRUE;

			m_pSelectedOrderLine->MouseRButtonUp(point);
	
//			if(m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_TABLE)
//			{			
				CMenu	menu;
				menu.CreatePopupMenu();

				int nID=102;
				menu.AppendMenu(MF_STRING, nID++, "Change Order"); 
				menu.AppendMenu(MF_STRING, nID++, "Remove Order"); 

				UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
				int nRet = menu.TrackPopupMenu(flags, point.x + 40, point.y + 250, m_pChartCWnd);
				menu.DestroyMenu();

				switch(nRet) {
				case 102:
					{
						m_pIChartOCX->OnOrderInfomationChanged(dYValue, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_CHANGE, 
												m_pSelectedOrderLine->m_strOrderID,  "", "", "");
						break;
					}
				case 103:
					m_pIChartOCX->OnOrderInfomationChanged(dYValue, (int)m_pSelectedOrderLine->m_dAmount, ORDERTYPE_CANCEL, 
												m_pSelectedOrderLine->m_strOrderID, "", "", "");
					break;
							}
//			}
		}

	}
	return FALSE;
}

void COrderLineAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;


//	if( eToolOption == CToolOptionInfo::T_DRAG_X_ZOOM || eToolOption == CToolOptionInfo::T_DRAG_Y_ZOOM )
//		m_bRun = FALSE;
//	else if( eToolOption == CToolOptionInfo::T_TOOL_SELECTION)
//	{
//		m_bRun = FALSE;
//		return;
//	}
//	else
		return;

//	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_DRAG_Y_ZOOM, m_bRun);
	
}

BOOL COrderLineAddInImp::GetGraphVScaleRegion( void )
{
	// 1. "일목균형표"가 포함된 Block의 Index를 구한다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
		
	// 3. Vertical Scale Region을 구한다.
	pIBlock->GetVertScaleRegion(m_rctLVScaleRegion, m_rctRVScaleRegion);
	pIBlock->Release();
	pIBlock = NULL;
	
	return TRUE;	
}

BOOL COrderLineAddInImp::GetGraphBlockRegion( void )
{
	// 1. "일목균형표"가 포함된 Block의 Index를 구한다.
	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return FALSE;
		
	// 3. Block의 Region을 구한다.
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( m_rctGraphRegion, rctNoFrameAndMargin);

	// 4. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;

	// 5. Block Region을 조정한다.
	m_rctGraphRegion.left = m_rctGraphDrawingRegion.left;
	m_rctGraphRegion.right = m_rctGraphDrawingRegion.right;
	
	return TRUE;
}

void COrderLineAddInImp::OnResetChart()
{
//	OnAddInToolCommand( EAI_CUSTOM_ENABLE_BOUNDMARK, TRUE);
	SetRemove_OrderLine();
}


void COrderLineAddInImp::SetRemove_OrderLine()
{
	POSITION pos = m_OrderLineList.GetHeadPosition();
	while (pos)
	{
		COrderLine* pLineOrder = (COrderLine*)m_OrderLineList.GetNext(pos);
		delete pLineOrder;
		pLineOrder = NULL;
	}
	m_OrderLineList.RemoveAll();
	m_pSelectedOrderLine = NULL;
}

void COrderLineAddInImp::SetMouseCursor(int nType)
{
	ML_SET_DEFAULT_RES();
	m_hAPCursor = AfxGetApp()->LoadCursor( nType);
	ASSERT( m_hAPCursor);
	if( m_pIChartOCX) m_pIChartOCX->SetCursorHandle( m_hAPCursor);

}

void COrderLineAddInImp::OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)
{
	
}

void COrderLineAddInImp::SetOpenPosition(FX_CHE_STRUCT* pStOpenPosition)
{
	POSITION pos = m_OrderLineList.GetHeadPosition();
	POSITION pos2;

	while(pos)
	{
		pos2 = pos;
		COrderLine* pOpenPosition = (COrderLine*)m_OrderLineList.GetNext(pos);
		if(!::memcmp(pStOpenPosition->szOrgCustItem2, pOpenPosition->m_strOrderID, sizeof(pStOpenPosition->szOrgCustItem2)))
		{
			if(pStOpenPosition->fAmount == 0.)
			{
				delete pOpenPosition;
				pOpenPosition = NULL;

				m_OrderLineList.RemoveAt(pos2);
			}
			else
			{
				CString strOrderPrice(pStOpenPosition->szRate, sizeof(pStOpenPosition->szRate));

				pOpenPosition->m_dOrderPrice = atof(strOrderPrice);
				int nYPoint = m_pIChartOCX->ConvertDataToYPosition( pOpenPosition->m_dOrderPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);

				pOpenPosition->m_strRate = strOrderPrice;
				pOpenPosition->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
				pOpenPosition->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);
			}
			
			return;
		}
	}

	AddOpenPosition(pStOpenPosition);
}

void COrderLineAddInImp::AddOpenPosition(FX_CHE_STRUCT* pStOpenPosition)
{	
	COrderLine* pOrderLine = new COrderLine();

	SetData(pOrderLine, pStOpenPosition, CONCLUSION_YES);
	m_OrderLineList.AddTail((CObject*)pOrderLine);
}

void COrderLineAddInImp::SetPendingPosition(FX_CHE_STRUCT*	pStPendingPosition)
{
	POSITION pos = m_OrderLineList.GetHeadPosition();
	POSITION pos2;

	while(pos)
	{
		pos2 = pos;
		COrderLine* pPendingPosition = (COrderLine*)m_OrderLineList.GetNext(pos);
		if(!::memcmp(pStPendingPosition->szOrgCustItem2, pPendingPosition->m_strOrderID, sizeof(pStPendingPosition->szOrgCustItem2)))
		{
			if(pStPendingPosition->fOrderAmt == 0.)
			{
				delete pPendingPosition;
				pPendingPosition = NULL;

				m_OrderLineList.RemoveAt(pos2);
			}
			else
			{
				CString strOrderPrice(pStPendingPosition->szRate, sizeof(pStPendingPosition->szRate));

				pPendingPosition->m_dOrderPrice = atof(strOrderPrice);
				int nYPoint = m_pIChartOCX->ConvertDataToYPosition( pPendingPosition->m_dOrderPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);

				pPendingPosition->m_strRate = strOrderPrice;
				pPendingPosition->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
				pPendingPosition->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);
			}
			
			return;
		}
	}

	AddPendingPosition(pStPendingPosition);
/*
	CPendingOrder clPendingOrder;
	clPendingOrder.Setdata(stPendingOrder);
*/
}

void COrderLineAddInImp::AddPendingPosition(FX_CHE_STRUCT* pStPendingPosition)
{
	COrderLine* pOrderLine = new COrderLine();
	SetData(pOrderLine, pStPendingPosition, CONCLUSION_NO);
	m_OrderLineList.AddTail((CObject*)pOrderLine);
/*
	int nYPoint = m_pIChartOCX->ConvertDataToYPosition( stOpenPosition.m_dOpenPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	
	COrderLine* pOrderLine = new COrderLine();
	pOrderLine->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
	pOrderLine->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);
	pOrderLine->m_dOrderPrice = stOpenPosition.m_dOpenPrice;
	pOrderLine->m_dAmount = stOpenPosition.m_dAmount;
	m_OrderLineList.AddTail((CObject*)pOrderLine);
*/
}

void COrderLineAddInImp::CheckIsLineInScreen(COrderLine* pOrderLine)
{	
	CPoint ptLeftTop(m_rctGraphRegion.left, m_rctGraphRegion.top);       //화면 좌측상단 좌표
	CPoint ptLeftBottom(m_rctGraphRegion.left, m_rctGraphRegion.bottom); //화면 좌측하단 좌표

	int nR, nC, nV, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");

	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock )
		return;
	
	double dViewMin, dViewMax;
	CScaleBaseData::VERTMINMAXTYPE eMinMaxType;
	pIBlock->GetVScaleMinMaxSetting( nV, m_bIsLock, eMinMaxType, dViewMin, dViewMax);

	if(!m_bOnSetCaprture)
	{	
		int nYPoint = m_pIChartOCX->ConvertDataToYPosition(pOrderLine->m_dOrderPrice, dViewMax, dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
		pOrderLine->m_ptStart.y = nYPoint;
		pOrderLine->m_ptEnd.y   = nYPoint;
	}
	
	if(pOrderLine->m_dOrderPrice > dViewMax)      //OrderLine이 화면 위 영역에 존재
	{		
		if(pOrderLine->m_eLinePosition == LINE_POS_OUT_UP)
			return;
		else if(pOrderLine->m_eLinePosition == LINE_POS_OUT_DOWN)
		{
			m_nCount_EscapeDown--;			
		}	
		pOrderLine->m_eLinePosition = LINE_POS_OUT_UP;
		m_nCount_EscapeUp++;	
		//화면 영역 벗어날시 삼각형 시작 좌표 설정
		pOrderLine->m_ptEscape_Start = CPoint(m_rctGraphRegion.left + TRIANGLE_SIDE * m_nCount_EscapeUp, m_rctGraphRegion.top);
	}
	else if(pOrderLine->m_dOrderPrice < dViewMin)  //OrderLine이 화면 아래 영역에 존재
	{
		if(pOrderLine->m_eLinePosition == LINE_POS_OUT_UP)
		{
			m_nCount_EscapeUp--;
		}
		else if(pOrderLine->m_eLinePosition == LINE_POS_OUT_DOWN)
			return;
		
		pOrderLine->m_eLinePosition = LINE_POS_OUT_DOWN;
		m_nCount_EscapeDown++;
		//화면 영역 벗어날시 삼각형 시작 좌표 설정
		pOrderLine->m_ptEscape_Start = CPoint(m_rctGraphRegion.left + TRIANGLE_SIDE * m_nCount_EscapeDown, m_rctGraphRegion.bottom);
	}
	else									  ////OrderLine이 화면 영역에 존재
	{
		if(pOrderLine->m_eLinePosition == LINE_POS_OUT_UP)
			m_nCount_EscapeUp--;
		else if(pOrderLine->m_eLinePosition == LINE_POS_OUT_DOWN)
			m_nCount_EscapeDown--;
		pOrderLine->m_eLinePosition = LINE_POS_IN;
	}
}

BOOL COrderLineAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	// Load Chart Data Info I
	if( stricmp(p_szData, "ON") == 0 )
	{
		m_bRun = TRUE;
		// (2008/9/13 - Seung-Won, Bae) Set Mouse Exclusive
		m_pIAddInToolMgr->GetAllMouseExclusive( m_pIAddInDllBase);
	}
	else
	{
		m_bRun = FALSE;
		// (2008/9/13 - Seung-Won, Bae) Set Mouse Exclusive
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
	}

	m_pIChartOCX->RunToolCommand( CToolOptionInfo::T_TOOL_SELECTION, 1);
	return TRUE;
}

BOOL COrderLineAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	if(!p_lData) return FALSE;

	FX_CHE_STRUCT* pDataPosition = (FX_CHE_STRUCT*)p_lData;

	if(p_nCommandType == 1)			// ADD OPEN_POSITION after Seach
		SetOpenPosition(pDataPosition);
	else if(p_nCommandType == 2)	// ADDPENDING_ORDER after Seach
		SetPendingPosition(pDataPosition);	
	else if(p_nCommandType == 3)	// ADD OPEN_POSITION
		AddOpenPosition(pDataPosition);	
	else if(p_nCommandType == 4)	// ADD PENDING_ORDER
		AddPendingPosition(pDataPosition);	
	else if(p_nCommandType == 5)	// reset
	{

	}
	return TRUE;
}

void COrderLineAddInImp::SetData(COrderLine* pOrderLine, FX_CHE_STRUCT* pDataPosition, int nType)
{
	CString strOrderPrice(pDataPosition->szRate, sizeof(pDataPosition->szRate));
	CString strOrderID(pDataPosition->szOrgCustItem2, sizeof(pDataPosition->szOrgCustItem2));

	pOrderLine->m_dOrderPrice = atof(strOrderPrice);


	if(nType == CONCLUSION_YES)
	{
		pOrderLine->m_dAmount = pDataPosition->fAmount;
		pOrderLine->m_bCon = TRUE;
	}
	else
	{
		pOrderLine->m_dAmount = pDataPosition->fOrderAmt;
		pOrderLine->m_bCon = FALSE;
	}

	int nYPoint = m_pIChartOCX->ConvertDataToYPosition( pOrderLine->m_dOrderPrice, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);

	pOrderLine->m_strRate = strOrderPrice;
	pOrderLine->m_strOrderID = strOrderID;
	pOrderLine->m_ptStart = CPoint(m_rctGraphRegion.left, nYPoint);
	pOrderLine->m_ptEnd = CPoint(m_rctGraphRegion.right, nYPoint);

}


void COrderLineAddInImp::SetOrderInfomation()
{
	return ;
	if(m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_NONE)
		return;

	int nOrderType;
	if(	m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_ORDERLINE)
	{
		if(m_pSelectedOrderLine->m_ptStart != m_pSelectedOrderLine->m_ptStartChgLine
			&& m_pSelectedOrderLine->m_ptEnd != m_pSelectedOrderLine->m_ptEndChgLine)
			nOrderType = ORDERTYPE_CHANGE;
		else
			return;
	}
	else if(m_pSelectedOrderLine->m_nCurClickedPos == CLICKEDPOS_TABLE)
	{
		if(m_pSelectedOrderLine->m_ptTableOffset == m_pSelectedOrderLine->m_ptBackupTalbeOffset)
			nOrderType = ORDERTYPE_CANCEL;
		else 
			return;
	}


	m_pIChartOCX->OnOrderInfomationChanged(m_pSelectedOrderLine->m_dOrderPrice, (int)m_pSelectedOrderLine->m_dAmount, nOrderType,  
										 m_pSelectedOrderLine->m_strOrderID, "", "", "");
	
}