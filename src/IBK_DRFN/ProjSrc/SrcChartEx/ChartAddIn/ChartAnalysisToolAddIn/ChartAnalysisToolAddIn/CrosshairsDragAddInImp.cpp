// CrosshairsDragAddInImp.cpp: implementation of the CCrosshairsDragAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrosshairsDragAddInImp.h"

#include "../Include_Chart/EnvironmentData.h"					// for CStyleData
#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_AddIn_133101/I133101/_ICmdUI.h"			// for ICmdUI

#include "../Include_Analysis/Dll_Load/MainAnalysisTool.h"		// for CMainAnalysisTool

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrosshairsDragAddInImp::CCrosshairsDragAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase),
	m_niDataQueryer( p_pIChartOCX) //20100219 이문수 >>
{
	// save point for inflate when mouse drag & drop
	// 09.21.2001
	m_ptStartDragDrop = CPoint(-1, -1);

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = UINT_MAX;

	// (2006/1/24 - Seung-Won, Bae) Manage Analysis Tool Interface.
	m_pAnalysisTool = new CMainAnalysisTool( m_pChartCWnd, p_pIChartOCX->GetOcxHwnd(), ( CChartInfo::CHART_MODE)p_pIChartOCX->GetChartMode());
	// update ver 1.2 -> 11.5.2001
	m_pAnalysisTool->SetMainBlock( m_pIChartManager, p_pIChartOCX);
	
	// (2006/1/29 - Seung-Won, Bae) Reset Initial Color.
	OnAddInToolCommand( EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR);

	// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed
	m_bCrossBtnExecuted		= FALSE;
	m_bWithNIDlgOnToolExec	= FALSE;

	// (2006/6/28 - Seung-Won, Bae) Manage Notify from Over-Cross to Drag-Cross for avoiding the Multiple Cross Active.
	m_bCrossCfgExecuted		= FALSE;

	m_bLButtonDown = FALSE;

	// (2006/3/30 - Seung-Won, Bae) for ID_CAA_CROSS_FOLLOW_CLOSE
	m_bCrossLineFollowClose = FALSE;

	//////////////////////////////////////////////////////////
	//by jwp(2007/01/02):MouseCrossCursor Part++
	m_bOnSetCaprture			= FALSE;
	m_rctBlockRegion = CRect(0, 0, 0, 0);
	m_rctTextLeft = CRect( 0, 0, 0, 0 );		
	m_rctTextRight = CRect( 0, 0, 0, 0 );		
	m_rctTextTop = CRect( 0, 0, 0, 0 );		
	m_rctTextBottom = CRect( 0, 0, 0, 0 );		
	m_ptOldMouse = CPoint(0,0);
	//by jwp(2007/01/02):MouseCrossCursor Part--
	//////////////////////////////////////////////////////////

	m_hOcxWnd = ( m_pChartCWnd ? m_pChartCWnd->GetSafeHwnd() : NULL);
}

CCrosshairsDragAddInImp::~CCrosshairsDragAddInImp()
{
	if( m_pAnalysisTool)
	{
		delete m_pAnalysisTool;
		m_pAnalysisTool = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CCrosshairsDragAddInImp::m_strAddInItemName = "CROSS_DRAG";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CCrosshairsDragAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnCaptureChanged)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
END_ADDIN_EVENT_MAP()

// (2006/1/26 - Seung-Won, Bae) Draw Analysis Tool Object
void CCrosshairsDragAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	// 04.18.2002  modified -- Mouse Drag Inflate 중이었으면 실행
	if( m_ptStartDragDrop != CPoint(-1, -1) && m_pAnalysisTool) 
		m_pAnalysisTool->OnDraw( CDC::FromHandle( p_hDC), TRUE);	
}

RECT CCrosshairsDragAddInImp::DrawCursor( HDC p_hDC)
{ 
	RECT rct = { 0, 0, 0, 0};
	if( m_bOnSetCaprture == TRUE && m_bLButtonDown == TRUE && 
		m_bCrossBtnExecuted == FALSE && m_bCrossCfgExecuted != FALSE)
	{		
		GetCurrentPosData(m_ptOldMouse);
		if(m_strTextXDate.IsEmpty() == FALSE && m_strTextYValue.IsEmpty() == FALSE)
		{
			DrawNewCross(p_hDC);
			return m_rctBlockRegion;
		}
	}

	return rct;	
}

void CCrosshairsDragAddInImp::OnResetChart(void)
{
	if( !m_pAnalysisTool) return;

	m_bLButtonDown = FALSE;
	//m_bCrossBtnExecuted = FALSE;
	//m_bCrossCfgExecuted	= FALSE;

	m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_CROSSHAIRS_TOOL,FALSE);
	m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_CROSSHAIRS_TOOL_WITH_NIDLG,FALSE);
	
	/*if( m_bWithNIDlgOnToolExec == TRUE) 
	{
		m_bWithNIDlgOnToolExec = FALSE;		
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_NIDLG_DRAG_SHOWHIDE, ( long)0);
	}*/
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CCrosshairsDragAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( ID_CAA_CROSS_FOLLOW_CLOSE != nID) return FALSE;
	
	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		if( p_pICmdUI)
		{
			// (2009/6/5 - Seung-Won, Bae) Check close packet.
			IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
			p_pICmdUI->Enable( NULL != pIPacket);
			if( pIPacket)
			{
				pIPacket->Release();
				pIPacket = NULL;
			}
			p_pICmdUI->SetCheck( m_bCrossLineFollowClose);
		}
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		m_bCrossLineFollowClose = !m_bCrossLineFollowClose;
		m_pAnalysisTool->SetAutoPriceTool( m_bCrossLineFollowClose ? true : false);
		return FALSE;	// for Other AddIn with AnalysisTool
	}

	return FALSE;
}

BOOL CCrosshairsDragAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( !m_pAnalysisTool) return FALSE;

	switch( p_eCommandID)
	{
		case EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR:
		{
			COLORREF clrAnalysisTool = m_pIChartOCX->GetStyleValueAnalysisToolColor();
			m_pAnalysisTool->SetBaseColor( clrAnalysisTool, clrAnalysisTool);
			m_pAnalysisTool->ChangeAllElementsColor( clrAnalysisTool, clrAnalysisTool);
			break;
		}
		case EAI_ALL_APPLY_STYLE:
		{
			m_pAnalysisTool->SetBasePenColor( m_pIChartOCX->GetStyleValueAnalysisToolColor());
			return FALSE;
		}
	}
	return FALSE;
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CCrosshairsDragAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( !m_pAnalysisTool || m_pIChartManager->IsEmpty()) return;
	
	if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{
		if( !m_pAnalysisTool) return;

		m_bLButtonDown = FALSE;
		m_bCrossBtnExecuted = FALSE;
		m_bCrossCfgExecuted	= FALSE;

		m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_CROSSHAIRS_TOOL,FALSE);
		m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_CROSSHAIRS_TOOL_WITH_NIDLG,FALSE);
		
		if( m_bWithNIDlgOnToolExec == TRUE) 
		{
			m_bWithNIDlgOnToolExec = FALSE;		
			m_pIAddInToolMgr->OnAddInToolCommand( EAI_NIDLG_DRAG_SHOWHIDE, ( long)0);
		}
		return;
	}

	if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		if(m_bCrossBtnExecuted != FALSE) 
			m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_CROSSHAIRS_TOOL,TRUE);

		if( m_bWithNIDlgOnToolExec == TRUE) 
			m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_CROSSHAIRS_TOOL_WITH_NIDLG,( long)1);
		return;
	}
	
	// 0. Check New Tool State
	if( CToolOptionInfo::T_CROSSHAIRS_TOOL == p_nToolType || 
		CToolOptionInfo::T_CROSSHAIRS_TOOL_WITH_NIDLG == p_nToolType)
			m_bCrossBtnExecuted = p_nCmdOption;
	
	if( CToolOptionInfo::T_CROSSHAIRS_TOOL_WITH_NIDLG == p_nToolType)
	{
		m_bWithNIDlgOnToolExec = TRUE;
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_NIDLG_DRAG_SHOWHIDE, p_nCmdOption);
		// (2006/6/28 - Seung-Won, Bae) Notify from CrossLine Object Tool to Over-Cross for avoiding the Multiple Cross Active.
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_CROSSHAIRS_OVER_CROSSHAIRS_TOOL_ACTIVATED, p_nCmdOption);
	}
}

BOOL CCrosshairsDragAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{ 
	// (2006/6/28 - Seung-Won, Bae) Manage Notify from Over-Cross to Drag-Cross for avoiding the Multiple Cross Active.
	if( EAI_CROSSHAIRS_DRAG_OVER_ACTIVATED == p_eCommandID)
		return FALSE;				// for CrossLine Object Tool AddIn
	
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart
	if( !m_pAnalysisTool || m_pIChartManager->IsEmpty() || 
		EAI_ALL_ON_CHANGE_CURSOR_MODE != p_eCommandID) return FALSE;
	
	// (2006/2/13 - Seung-Won, Bae) Manage the Flag of Environment Setting
	m_bCrossCfgExecuted		= ( p_lParam & CROSSLINEVIEW);
	m_bWithNIDlgOnToolExec	= ( p_lParam & NUMERICALINQUIRYDLG);	// (2006/2/28 - Seung-Won, Bae) for Active without NIDLG

	return FALSE;	// for Other Tool
}

BOOL CCrosshairsDragAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	m_bLButtonDown = FALSE;
	if(nFlags != MK_LBUTTON) return FALSE;

	m_bLButtonDown = TRUE;

	// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed
	if( !m_bCrossCfgExecuted || m_bCrossBtnExecuted) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
//		if( m_ptStartDragDrop == point)	
//			return FALSE;				// It is not moved.
		if( p_dwMouseActionID != m_dwMouseActionID) 
			return FALSE;	// It is not one action.

		GetGraphTotalRegion(point);
		if( m_rctBlockRegion.PtInRect( point ) && m_rctBlockRegion.IsRectEmpty() == FALSE)
		{		
			if(!m_bOnSetCaprture)
				SetCaptureWithFlag();
		}
	}

	if( m_bOnSetCaprture)
	{
		m_ptOldMouse = point;

		if( m_bWithNIDlgOnToolExec) 
			m_pIAddInToolMgr->OnAddInToolCommand( EAI_NIDLG_DRAG_ONMOUSEMOVE, p_hDC, nFlags, point, p_dwMouseActionID);

		if( !m_rctBlockRegion.PtInRect( point ) || m_rctBlockRegion.IsRectEmpty() != FALSE)
			ReleaseCaptureWithFlag();
	}

	return TRUE;
}

BOOL CCrosshairsDragAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{ 
	//by jwp(2006/12/05) - On SpecialChart CrossLine Is enable to move crossline
	//m_pIChartOCX->IsOnSpecialChart() ||

	if( m_pIChartManager->IsEmpty() || CRect(0, 0, 0, 0) == m_pIChartManager->GetGraphRegionSumInColumn( point)) 
		return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = p_dwMouseActionID;
	m_ptStartDragDrop = point;	
	
	return FALSE;	// return FALSE. OnLButtonDown is not start time of this tool.
}

BOOL CCrosshairsDragAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed
	if( !m_bCrossCfgExecuted || m_bCrossBtnExecuted) return FALSE;

	if( m_bOnSetCaprture) ReleaseCaptureWithFlag();

	return FALSE;
}

void CCrosshairsDragAddInImp::SetCaptureWithFlag()
{
	if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) 
		return;

	// 1. Mouse Event를 Capturing한다.
	// (2004.11.02, 배승원) Mouse Capturing이 중첩적으로 처리되도록, 중첩 Count와 별도의 Capturing Interface를 이용한다.
	m_pIChartOCX->AddSetCapture();

	// 2. Mouse Capturing Flag을 TRUE로 설정한다.
	m_bOnSetCaprture = TRUE;	

	if( m_bWithNIDlgOnToolExec) 
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_NIDLG_DRAG_SHOWHIDE, ( long)1);
}

void CCrosshairsDragAddInImp::ReleaseCaptureWithFlag()
{
	// 1. Mouse Capturing Flag을 FALSE로 설정한다.
	m_bOnSetCaprture = FALSE;
	
	m_pIChartOCX->RemoveSetCapture();

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase)) return;

	m_ptStartDragDrop = CPoint(-1, -1);

	if( m_bWithNIDlgOnToolExec) 
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_NIDLG_DRAG_SHOWHIDE, ( long)0);
}

void CCrosshairsDragAddInImp::DrawNewCross( HDC p_hDC)
{
	// Manage State for Resource Loading of Cursor 
	ML_SET_LANGUAGE_RES();

	// 1. DC의 Handle을 구한다.
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	bool bLeftExist = true;
	bool bRightExist = true;
	bool bTopExist = true;
	bool bBottomExist = true;

	CRect rcTop;
	CRect rcBottom;
	CRect rcLeft;
	CRect rcRight;
	
	rcLeft.SetRectEmpty();
	rcRight.SetRectEmpty();
	IBlock *pBlock = m_pIChartManager->GetBlock(0,0);
	pBlock->GetVertScaleRegion(rcLeft,rcRight);	
	pBlock->Release();

	rcTop.SetRectEmpty();
	rcBottom.SetRectEmpty();
	CRect rcTopTemp;
	CRect rcBottomTemp;	
	int nRowCount = m_pIChartManager->GetRowCount();
	int nColCount = m_pIChartManager->GetColumnCount();
	for(int i=0;i<nRowCount;i++)
	{
		for(int j=0;j<nColCount;j++)
		{
			IBlock *pBlock = m_pIChartManager->GetBlock(i,j);
			pBlock->GetHorzScaleRegion(rcTopTemp,rcBottomTemp);	
			pBlock->Release();

			if(rcTopTemp.IsRectEmpty() == FALSE)
				rcTop.CopyRect(rcTopTemp);
			
			if(rcBottomTemp.IsRectEmpty() == FALSE)
				rcBottom.CopyRect(rcBottomTemp);			
		}
	}

	if(rcLeft.IsRectEmpty())
		bLeftExist = false;
	
	if(rcRight.IsRectEmpty())
		bRightExist = false;

	if(rcTop.IsRectEmpty())
		bTopExist = false;
	
	if(rcBottom.IsRectEmpty())
		bBottomExist = false;

	if(bLeftExist == true)
	{
		CSize szTextLeft = pDC->GetTextExtent( m_strTextYValue );
		m_rctTextLeft.top = m_ptOldMouse.y - szTextLeft.cy/2;
		m_rctTextLeft.bottom = m_ptOldMouse.y + szTextLeft.cy/2+3;
		if((rcLeft.right - szTextLeft.cx-5) <= rcLeft.left)
		{
			m_rctTextLeft.right = rcLeft.left + szTextLeft.cx+5;
			m_rctTextLeft.left = rcLeft.left;
		}
		else
		{
			m_rctTextLeft.left = rcLeft.right - szTextLeft.cx-5;
			m_rctTextLeft.right = rcLeft.right;
		}	
	}

	if(bRightExist == true)
	{
		CSize szTextRight = pDC->GetTextExtent( m_strTextYValue );
		m_rctTextRight.top = m_ptOldMouse.y - szTextRight.cy/2;
		m_rctTextRight.bottom = m_ptOldMouse.y + szTextRight.cy/2+3;
		if((rcRight.left + szTextRight.cx+5) >= rcRight.right)
		{
			m_rctTextRight.right = rcRight.right;
			m_rctTextRight.left = rcRight.right - szTextRight.cx-5;
		}
		else
		{
			m_rctTextRight.right = rcRight.left + szTextRight.cx+5;
			m_rctTextRight.left = rcRight.left;
		}
	}
	
	if(bTopExist == true)
	{
		CSize szTextTop = pDC->GetTextExtent( m_strTextXDate );		
		m_rctTextTop.top = rcTop.top;
		m_rctTextTop.bottom = rcTop.top + szTextTop.cy+2;
		m_rctTextTop.right = m_ptOldMouse.x + szTextTop.cx/2 + 5;
		m_rctTextTop.left = m_ptOldMouse.x - szTextTop.cx/2;
	}

	if(bBottomExist == true)
	{
		CSize szTextBottom = pDC->GetTextExtent( m_strTextXDate );		
		m_rctTextBottom.top = rcBottom.top;
		m_rctTextBottom.bottom = rcBottom.top + szTextBottom.cy+2;
		m_rctTextBottom.right = m_ptOldMouse.x + szTextBottom.cx/2 + 5;
		m_rctTextBottom.left = m_ptOldMouse.x - szTextBottom.cx/2;
	}

	// 5. 현 위치에 호가를 표시한다.
	// 5.1 NOT Line을 출력한다.
	int nPreROP = pDC->SetROP2( R2_NOT);
	pDC->MoveTo( m_rctBlockRegion.left,	m_ptOldMouse.y);
	pDC->LineTo( m_rctBlockRegion.right,m_ptOldMouse.y);

	pDC->MoveTo( m_ptOldMouse.x,m_rctBlockRegion.top);
	pDC->LineTo( m_ptOldMouse.x,m_rctBlockRegion.bottom);
	pDC->SetROP2( nPreROP);
		
	CPen *	pOldPen		= ( CPen *)pDC->SelectStockObject( BLACK_PEN);
	CBrush *pOldBrush	= ( CBrush *)pDC->SelectStockObject( WHITE_BRUSH);

	if(bLeftExist == true)
	{
		pDC->Rectangle( &m_rctTextLeft);
		pDC->TextOut( m_rctTextLeft.left + 3, m_rctTextLeft.top + 2, m_strTextYValue);
	}

	if(bRightExist == true)
	{
		pDC->Rectangle( &m_rctTextRight);
		pDC->TextOut( m_rctTextRight.left + 3, m_rctTextRight.top + 2, m_strTextYValue);
	}

	if(bTopExist == true)
	{
		pDC->Rectangle( &m_rctTextTop);
		pDC->TextOut( m_rctTextTop.left + 3, m_rctTextTop.top + 2, m_strTextXDate);
	}

	if(bBottomExist == true)
	{
		pDC->Rectangle( &m_rctTextBottom);
		pDC->TextOut( m_rctTextBottom.left + 3, m_rctTextBottom.top/* + 2*/, m_strTextXDate);
	}

	pDC->SelectObject( pOldBrush);
	pDC->SelectObject( pOldPen);
}

void CCrosshairsDragAddInImp::OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{	
	if( m_bOnSetCaprture)
		ReleaseCaptureWithFlag();
}

BOOL CCrosshairsDragAddInImp::GetCurrentPosData( CPoint ptPoint )
{
	m_strTextXDate.Empty();	
	m_strTextYValue.Empty();	

	int nStartIndex = 0;
	int nEndIndex = 0;
	// 1. 현재 화면에 보이는 Data의 Start/End Index를 구한다.

	// XScale : 영역의 시작/끝 인덱스 구하기 - vntsorl(20070327)
//	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex ))
	if( !m_pIChartManager->GetStartEndDrawIdx_Cur("DEFAULT", nStartIndex, nEndIndex))
		return FALSE;

	m_pAnalysisTool->GetCurrentPosData(ptPoint,m_strTextYValue,m_strTextXDate);

	//20100219 이문수 >>	
	if( m_niDataQueryer.ParsingInquiryData("", ptPoint))
	{
		SetInquiryXData(m_strTextXDate, &m_niDataQueryer.m_listInquiryData,&m_niDataQueryer.m_listPrevInquiryData);
	}
	//20100219 이문수 <<
	return TRUE;
}

//20100219 이문수 >>
void CCrosshairsDragAddInImp::SetInquiryXData(CString & strXData, CInquiryData *pInquiryData,CInquiryData *pInquiryPrevData)
{
	if( pInquiryData->IsEmpty()) return;
	if( pInquiryPrevData->IsEmpty()) return;

	// _LISTDATA는 하나만 사용한다. 그러므로 미리 가져오는게 좋다! (Multi Line은 여러개사용하니까 For문돌리고!)
	CInquiryItem * plistData	= pInquiryData->GetHead();	
	CInquiryItem* plistPrevData = pInquiryPrevData->GetHead();	
	
	// 데이타를 ListCtrl에 삽입.
	LV_ITEM ltc;
	ltc.mask = LVIF_TEXT;
	
	// (2006/7/2 - Seung-Won, Bae) Using Virtual Draw. 0:Title, 1:Data
	int nIndex = 0;
	CRowData *pRowData = NULL;
	CRowData *pRowPrevData = NULL;
	CString strItemData;
	double dItemData = 0.0;
	double dPrevItemData = 0.0;
	double dItemRatio = 0.0;
	CString strTempData;
	POSITION posRow = plistData->GetHeadPosition();
	POSITION posPrevRow = plistPrevData->GetHeadPosition();
	if( posPrevRow)
	{
		pRowPrevData= plistPrevData->GetAt( posPrevRow);
		strXData	= pRowPrevData->m_strData;
	}

	if( posRow )
	{
		pRowData = plistData->GetAt(posRow);
		strXData = pRowData->m_strData;
	}
}
//20100219 이문수 <<

BOOL CCrosshairsDragAddInImp::GetGraphTotalRegion(CPoint point)
{
	if(m_pAnalysisTool == NULL)
	{
		return FALSE;
	}

	m_rctBlockRegion = m_pAnalysisTool->GetGraphTotalRegion(point);		
	return TRUE;
}