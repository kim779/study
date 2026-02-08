// CrossOverToolAddInImp.cpp: implementation of the CCrosshairsOverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartAnalysisToolAddIn.h"
#include "CrosshairsOverAddInImp.h"

#include "../Include_Chart/EnvironmentData.h"					// for CStyleData
#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT
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

CCrosshairsOverAddInImp::CCrosshairsOverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2006/1/24 - Seung-Won, Bae) Manage Analysis Tool Interface.
	m_pAnalysisTool = new CMainAnalysisTool( m_pChartCWnd, p_pIChartOCX->GetOcxHwnd(), ( CChartInfo::CHART_MODE)p_pIChartOCX->GetChartMode());
	// update ver 1.2 -> 11.5.2001
	m_pAnalysisTool->SetMainBlock( m_pIChartManager, p_pIChartOCX);
	
	// (2006/1/29 - Seung-Won, Bae) Reset Initial Color.
	OnAddInToolCommand( EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR);

	// (2006/3/30 - Seung-Won, Bae) for ID_CAA_CROSS_FOLLOW_CLOSE
	m_bCrossLineFollowClose = FALSE;

	// (2006/6/28 - Seung-Won, Bae) Manage Notify from CrossLine Object Tool to Over-Cross for avoiding the Multiple Cross Active.
	m_bObjectActivated = FALSE;

	// (2006/7/3 - Seung-Won, Bae) Manage Notify from Crosshairs Tool.
	m_bToolActivated = FALSE;
}

CCrosshairsOverAddInImp::~CCrosshairsOverAddInImp()
{
	if( m_pAnalysisTool)
	{
		delete m_pAnalysisTool;
		m_pAnalysisTool = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CCrosshairsOverAddInImp::m_strAddInItemName = "CROSS_OVER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CCrosshairsOverAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_long)
	ONADDINEVENT( OnMouseMove)
END_ADDIN_EVENT_MAP()

// (2006/1/26 - Seung-Won, Bae) Draw Analysis Tool Object
void CCrosshairsOverAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	if( m_pAnalysisTool) 
		m_pAnalysisTool->OnDraw( CDC::FromHandle( p_hDC));
}

void CCrosshairsOverAddInImp::OnResetChart(void)
{
	if( !m_pAnalysisTool) return;
}
//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CCrosshairsOverAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
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

BOOL CCrosshairsOverAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( !m_pAnalysisTool) return FALSE;

	switch( p_eCommandID)
	{
		case EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR:
		{
			COLORREF clrAnalysisTool = m_pIChartOCX->GetStyleValueAnalysisToolColor();
			m_pAnalysisTool->SetBaseColor( clrAnalysisTool, clrAnalysisTool);
			//sy 2004.12.28. 저장되어 있는 element도 색 변경.
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

BOOL CCrosshairsOverAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	// (2006/6/28 - Seung-Won, Bae) Manage Notify from CrossLine Object Tool to Over-Cross for avoiding the Multiple Cross Active.
	if( EAI_CROSSHAIRS_OVER_CROSSLINE_ACTIVATED == p_eCommandID) m_bObjectActivated = p_lParam;
	// (2006/7/3 - Seung-Won, Bae) Manage Notify from Crosshairs Tool.
	if( EAI_CROSSHAIRS_OVER_CROSSHAIRS_TOOL_ACTIVATED == p_eCommandID) m_bToolActivated = p_lParam;

	// (2006/6/29 - Seung-Won, Bae) when Activated, Clear shadow.
	if( m_bObjectActivated || m_bToolActivated) m_pAnalysisTool->OnMouseMove( NULL, CPoint( -1, -1),-1,NULL);

	return FALSE;
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CCrosshairsOverAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( !m_pAnalysisTool || m_pIChartManager->IsEmpty()) return;

	if( p_nToolType == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)		return;
	else if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)	return;

	if( CToolOptionInfo::T_CROSSHAIRS_OVER != p_nToolType)	return;

	m_pAnalysisTool->SetCoordinateType( CAnalysis::RELATIVE_POINT);	// 상대 좌표형 - data 형태
	if( 1 == p_nCmdOption)	m_pAnalysisTool->SetCurrentAnalysisTool( CAnalysis::EN_ANALYSIS_MOVE_CROSS);
	else					m_pAnalysisTool->SetCurrentAnalysisTool( CAnalysis::EN_ANALYSIS_SELECTION);

	// (2006/6/28 - Seung-Won, Bae) Manage Notify from Over-Cross to Drag-Cross for avoiding the Multiple Cross Active.
	m_pIAddInToolMgr->OnAddInToolCommand( EAI_CROSSHAIRS_DRAG_OVER_ACTIVATED, p_nCmdOption);
}

BOOL CCrosshairsOverAddInImp::OnMouseMove(  HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if( !m_pAnalysisTool) return FALSE;
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart
	if( m_pIChartManager->IsEmpty() || CAnalysis::EN_ANALYSIS_MOVE_CROSS != m_pAnalysisTool->GetCurrentToolType()) return FALSE;

	// (2006/6/28 - Seung-Won, Bae) Manage Notify from CrossLine Object Tool to Over-Cross for avoiding the Multiple Cross Active.
	// (2006/7/3 - Seung-Won, Bae) Manage Notify from Crosshairs Tool.
	if( !m_bObjectActivated && !m_bToolActivated) m_pAnalysisTool->OnMouseMove( CDC::FromHandle( p_hDC), point,nFlags,p_hbmpMainOSB);

	return TRUE;
}
