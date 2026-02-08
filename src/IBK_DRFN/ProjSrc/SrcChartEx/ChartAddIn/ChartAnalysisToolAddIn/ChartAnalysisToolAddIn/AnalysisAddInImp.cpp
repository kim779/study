// AnalysisAddInImp.cpp: implementation of the CAnalysisAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnalysisAddInImp.h"

#include "../Include_Chart/EnvironmentData.h"						// for CStyleData
#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"	// for IPacketListManager
#include "../Include_AddIn_133101/I133101/_ICmdUI.h"				// for ICmdUI

#include "../Include_Analysis/Dll_Load/MainAnalysisTool.h"			// for CMainAnalysisTool
#include "resource.h"												// for IDR_ANALYSIS

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnalysisAddInImp::CAnalysisAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	m_eTimeType = CScaleBaseData::HORZ_NOTYPE;

	m_hOcxWnd = ( m_pChartCWnd ? m_pChartCWnd->GetSafeHwnd() : NULL);

	// (2006/1/24 - Seung-Won, Bae) Manage Analysis Tool Interface.
	ML_SET_LANGUAGE_RES();
	m_pAnalysisTool = new CMainAnalysisTool( m_pChartCWnd, p_pIChartOCX->GetOcxHwnd(), ( CChartInfo::CHART_MODE)p_pIChartOCX->GetChartMode());
	// update ver 1.2 -> 11.5.2001
	m_pAnalysisTool->SetMainBlock( m_pIChartManager, p_pIChartOCX);
	
	ILPCSTR szUserPath = p_pIChartOCX->GetUserFilePathString();
	m_strUserPath = szUserPath;

	// (2006/2/16 - Seung-Won, Bae) Manage Flag for LButtonUp Case
	m_dwMouseActionID = UINT_MAX;
	
	// (2006/3/7 - Seung-Won, Bae) Manage All Exclusive Flag between OnLButtonDown and OnLButtonUp.
	m_bOnAllMouseExclusive = FALSE;

	// (2006/3/30 - Seung-Won, Bae) for ID_CAA_CROSS_FOLLOW_CLOSE
	m_bCrossLineFollowClose = FALSE;

	// (2006/7/3 - Seung-Won, Bae) With NIDLG
	m_bWithNIDLG	= FALSE;
	m_bWithTRACINGLG = FALSE; 
	m_eToolOption	= CToolOptionInfo::T_TOOL_SELECTION;
}

CAnalysisAddInImp::~CAnalysisAddInImp()
{
	if( m_pAnalysisTool)
	{
		delete m_pAnalysisTool;
		m_pAnalysisTool = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CAnalysisAddInImp::m_strAddInItemName = "ANALYSIS_TOOL";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CAnalysisAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnBlockChanged)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnKeyDown)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnLButtonDblClk)
	ONADDINEVENT( OnRButtonUp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

// (2006/6/28 - Seung-Won, Bae) with NIDLG
void CAnalysisAddInImp::SetCurrentAnalysisTool( const CAnalysis::TOOLTYPE eToolTYPE)
{
	if( m_pAnalysisTool) 
		m_pAnalysisTool->SetCurrentAnalysisTool( eToolTYPE);
}

// (2006/1/26 - Seung-Won, Bae) Draw Analysis Tool Object
void CAnalysisAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	if( m_pAnalysisTool) m_pAnalysisTool->OnDraw( CDC::FromHandle( p_hDC));
}

RECT CAnalysisAddInImp::DrawCursor(HDC p_hDC)
{
	RECT rct = { 0, 0, 0, 0};	

	CAnalysis::TOOLTYPE nType = m_pAnalysisTool->GetCurrentToolType();
	if( nType != CAnalysis::EN_ANALYSIS_NONE && nType != CAnalysis::EN_ANALYSIS_SELECTION)
	{
		CRect rect = m_pAnalysisTool->GetGraphTotalRegion(m_ptOldMouse);
		m_pAnalysisTool->DrawTool( CDC::FromHandle( p_hDC));
		rct = rect;
	}
	return rct;
}

void CAnalysisAddInImp::OnResetChart(void)
{
	CAnalysis::TOOLTYPE nType = m_pAnalysisTool->GetCurrentToolType();
	if( nType != CAnalysis::EN_ANALYSIS_NONE && nType != CAnalysis::EN_ANALYSIS_SELECTION)
	{
		m_pIChartOCX->OnToolStateChanged(m_eToolOption,FALSE);
		// 툴바도 해제한다
		//m_pIChartOCX->ReleaseOtherToolUI( m_eToolOption);
		// 연속그리기 해제
		// (2006/6/28 - Seung-Won, Bae) with NIDLG
		SetCurrentAnalysisTool( CAnalysis::EN_ANALYSIS_SELECTION);

		// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
	}

	m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;	
}
//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CAnalysisAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( !m_pAnalysisTool) 
		return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
			case ID_CAA_CONFIG:
			case ID_CAA_FIBO_CONFIG:		// 2008.08.20 김진순 피보나치 확장
			case ID_CAA_DELETE:
			case ID_CAA_COPY:
			case ID_CAA_RELEASE_SERIESDRAW:
			case ID_CAA_DRAW_DRIFTLINE:
			case ID_CAA_DELETE_DRIFTLINE:		if( p_pICmdUI) p_pICmdUI->Enable( TRUE);
												break;

			// (2006/3/30 - Seung-Won, Bae) for ID_CAA_CROSS_FOLLOW_CLOSE
			case ID_CAA_CROSS_FOLLOW_CLOSE:		if( p_pICmdUI)
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
												break;
			default: return FALSE;
		}
		return TRUE;
	}
	
	if( CN_COMMAND == nCode)
	{
		ML_SET_LANGUAGE_RES();
		switch( nID)
		{
			case ID_CAA_CONFIG:	m_pAnalysisTool->OnMenu( CMenuType::MENU_SET, TRUE);
								break;
			// 2008.08.20 김진순 피보나치 확장<<
			case ID_CAA_FIBO_CONFIG: m_pAnalysisTool->OnMenu( CMenuType::MENU_FIBO_SET, TRUE);
								break;
			//>>
			case ID_CAA_DELETE:	m_pAnalysisTool->OnMenu( CMenuType::MENU_DELETE, TRUE);
								break;
			case ID_CAA_COPY:	m_pAnalysisTool->OnToolCopy();
								break;
			case ID_CAA_PROPERTY_CHANGE:		break;
			case ID_CAA_RELEASE_SERIESDRAW:
			{
				CAnalysis::TOOLTYPE nType = m_pAnalysisTool->GetCurrentToolType();
				if( nType != CAnalysis::EN_ANALYSIS_NONE && nType != CAnalysis::EN_ANALYSIS_SELECTION)
				{
					m_pIChartOCX->OnToolStateChanged(m_eToolOption,FALSE);
					// 툴바도 해제한다
					//m_pIChartOCX->ReleaseOtherToolUI( m_eToolOption);
					// 연속그리기 해제
					// (2006/6/28 - Seung-Won, Bae) with NIDLG
					SetCurrentAnalysisTool( CAnalysis::EN_ANALYSIS_SELECTION);

					// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
					m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
				}

				m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;
				break;
			}

			// (2006/6/28 - Seung-Won, Bae) with NIDLG
			case ID_CAA_DRAW_DRIFTLINE:	SetCurrentAnalysisTool( CAnalysis::EN_ANALYSIS_LINE);
										break;

			case ID_CAA_DELETE_DRIFTLINE:	m_pAnalysisTool->DeleteElement( CToolEnumData::Line);
											break;

			// (2006/3/30 - Seung-Won, Bae) for ID_CAA_CROSS_FOLLOW_CLOSE
			case ID_CAA_CROSS_FOLLOW_CLOSE:	m_bCrossLineFollowClose = !m_bCrossLineFollowClose;
											m_pAnalysisTool->SetAutoPriceTool( m_bCrossLineFollowClose ? true : false);
											return FALSE;	// for Other AddIn with AnalysisTool
			default: return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CAnalysisAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( !m_pAnalysisTool) 
		return FALSE;

	switch( p_eCommandID)
	{
		case EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR:
		{
			COLORREF clrAnalysisTool = m_pIChartOCX->GetStyleValueAnalysisToolColor();
			COLORREF clrAnalysisText = m_pIChartOCX->GetStyleValueAnalysisToolColor();//TextColor
			m_pAnalysisTool->SetBaseColor( clrAnalysisTool, clrAnalysisText);
			m_pAnalysisTool->ChangeAllElementsColor( clrAnalysisTool, clrAnalysisText);
			return FALSE;		// EAI_ALL_...
		}
		case EAI_ALL_APPLY_STYLE:
		{
			m_pAnalysisTool->SetBasePenColor( m_pIChartOCX->GetStyleValueAnalysisToolColor());
			return FALSE;
		}
		default:	
			return FALSE;
	}
	return TRUE;
}

void CAnalysisAddInImp::OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)
{
	if( !m_pAnalysisTool) return;

	switch( p_eChangeType)
	{
		case CBlockBaseData::BLOCK_INSERT:	m_pAnalysisTool->InsertElementTree( CBlockIndex( p_nSourceRow, p_nSourceColumn));
											break;
		case CBlockBaseData::BLOCK_MOVE:	m_pAnalysisTool->MoveElementTree( CBlockIndex( p_nSourceRow, p_nSourceColumn), CBlockIndex( p_nTargetRow, p_nTargetColumn));
											break;
		case CBlockBaseData::BLOCK_DELETE:	m_pAnalysisTool->DeleteAllElementInBlock( CBlockIndex( p_nSourceRow, p_nSourceColumn));
											break;
	}
}

BOOL CAnalysisAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)
{
	if( m_pAnalysisTool && EAI_ALL_RELEASE_SELECTION_EXCEPT_ONE == p_eCommandID && p_pParam != m_pIAddInDllBase)
	{
		// (2006/6/28 - Seung-Won, Bae) with NIDLG
		SetCurrentAnalysisTool( CAnalysis::EN_ANALYSIS_SELECTION);

		// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
	}

	return FALSE;
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CAnalysisAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( !m_pAnalysisTool) return;

	CAnalysis::TOOLTYPE nType = m_pAnalysisTool->GetCurrentToolType();				
	if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{
		if( nType != CAnalysis::EN_ANALYSIS_NONE && nType != CAnalysis::EN_ANALYSIS_SELECTION)
		{
			m_pIChartOCX->OnToolStateChanged(m_eToolOption,FALSE);
			// 툴바도 해제한다
			//m_pIChartOCX->ReleaseOtherToolUI( m_eToolOption);
			// 연속그리기 해제
			// (2006/6/28 - Seung-Won, Bae) with NIDLG
			SetCurrentAnalysisTool( CAnalysis::EN_ANALYSIS_SELECTION);

			// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
			m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
		}

		m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;	
		return;
	}
	
	if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		if( nType != CAnalysis::EN_ANALYSIS_NONE && nType != CAnalysis::EN_ANALYSIS_SELECTION)
			m_pIChartOCX->OnToolStateChanged(m_eToolOption,TRUE);
		return;
	}

	int nElementType = (int)p_nCmdOption;
	if( p_nToolType == CToolOptionInfo::T_TOOL_SELECTION &&
		m_pAnalysisTool->GetSelectedToolType() == (nElementType-10000))
	{ 
		m_pChartCWnd->Invalidate();
		return;
	}

	if( p_nToolType == CToolOptionInfo::T_TOOL_TRACEING_TT || 
		p_nToolType == CToolOptionInfo::T_NIDLG_OVER)
		return;

	if( p_nToolType == CToolOptionInfo::T_WHOLEVIEW)
		return;

	if( p_nToolType == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT ||
		p_nToolType == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT ||
		p_nToolType == CToolOptionInfo::T_ADDIN_AUTOSCROLL_STOP )
		return;

	if( p_nToolType == CToolOptionInfo::T_DELETE_INORDER || 
		p_nToolType == CToolOptionInfo::T_REMOVE_OBJ)
	{
		if( nType != CAnalysis::EN_ANALYSIS_NONE && nType != CAnalysis::EN_ANALYSIS_SELECTION)
			m_pIChartOCX->OnToolStateChanged(m_eToolOption,TRUE);
		return;
	}
	
	if(p_nToolType == CToolOptionInfo::T_DELETE_ALL)
	{
		if( nType != CAnalysis::EN_ANALYSIS_NONE && nType != CAnalysis::EN_ANALYSIS_SELECTION)
			m_pIChartOCX->OnToolStateChanged(m_eToolOption,TRUE);

		// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
		//		Delete with checking current state.
		//		See CKoscomChartCtrl::SetPacketData()
		if( 820 != p_nCmdOption)
		{
			m_pAnalysisTool->DeleteAllElement();
			return;
		}
		// 1. Check the ItemCode and time period.
		CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType = m_pIDefaultPacketManager->GetBaseTimeDataType();
		LPCTSTR szItemCode = NULL, szItemName = NULL;
		m_pIPacketListManager->GetItemCode( "DEFAULT", szItemCode, szItemName);
		CString strItemCode( szItemCode);
		if( m_eTimeType == eTimeType && m_strItemCode == strItemCode) return;
		m_pAnalysisTool->DeleteAllElement();
		m_eTimeType = eTimeType;
		m_strItemCode = strItemCode;
		return;
	}

	m_eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;
	CAnalysis::TOOLTYPE nTool = CAnalysis::EN_ANALYSIS_NONE;			
	switch(m_eToolOption)
	{
		case CToolOptionInfo::T_TOOL_LINE_EXT1:		//nTool = CAnalysis::EN_ANALYSIS_ADJUST_RATIO;
													nTool = CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE;	
													//nTool = CAnalysis::EN_ANALYSIS_PENCIL_LINE;
													//nTool = CAnalysis::EN_ANALYSIS_CANDLE_BONG;
													//nTool = CAnalysis::EN_ANALYSIS_CANDLE_LINE;
													//nTool = CAnalysis::EN_ANALYSIS_ANGLE_LINE;
													//nTool = CAnalysis::EN_ANALYSIS_RAFF_REGRESSION;
													//nTool = CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION;
													//nTool = CAnalysis::EN_ANALYSIS_CYCLE_LINES;
													//nTool = CAnalysis::EN_ANALYSIS_SPEED_ARC;
													//nTool = CAnalysis::EN_ANALYSIS_SPEED_LINE;
													//nTool = CAnalysis::EN_ANALYSIS_SPEED_FAN;
													//nTool = CAnalysis::EN_ANALYSIS_FIBO_CIRCLE;
													//nTool = CAnalysis::EN_ANALYSIS_TRACING_TT;
													break;
		case CToolOptionInfo::T_TOOL_LINE_PROFIT:
		case CToolOptionInfo::T_TOOL_LINE:			nTool = CAnalysis::EN_ANALYSIS_LINE;																										
													break;
		case CToolOptionInfo::T_TOOL_LINE_CROSS_WITH_NIDLG:	
		case CToolOptionInfo::T_TOOL_LINE_CROSS:	nTool = CAnalysis::EN_ANALYSIS_MOVE_CROSS; // <- TOOL_LINE_CROSS;
													break;
		case CToolOptionInfo::T_TOOL_LINE_VERT:		nTool = CAnalysis::EN_ANALYSIS_VERT_LINE;
													break;
		case CToolOptionInfo::T_TOOL_LINE_HORZ:		nTool = CAnalysis::EN_ANALYSIS_HORZ_LINE;
													break;
		case CToolOptionInfo::T_TOOL_LINE_TRISECT:	nTool = CAnalysis::EN_ANALYSIS_TRISECT;
													break;
		case CToolOptionInfo::T_TOOL_LINE_QUARTER:	nTool = CAnalysis::EN_ANALYSIS_QUARTER;
													break;
		case CToolOptionInfo::T_TOOL_FIBONACCI_ARC:	nTool = CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE;
													break;
		case CToolOptionInfo::T_TOOL_FIBO_CIRCLE:	nTool = CAnalysis::EN_ANALYSIS_FIBO_CIRCLE;
													break;
		case CToolOptionInfo::T_TOOL_FIBONACCI_FAN:	nTool = CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE;
													break;
		case CToolOptionInfo::T_TOOL_FIBO_RETRACE:	nTool = CAnalysis::EN_ANALYSIS_FIBO_RETRACE;
													break;
		case CToolOptionInfo::T_TOOL_FIBONACCI_TIME:nTool = CAnalysis::EN_ANALYSIS_FIBO_TIME;
													break;
		case CToolOptionInfo::T_TOOL_GANN_LINE45:	nTool = CAnalysis::EN_ANALYSIS_GANN_LINE45;
													break;
		case CToolOptionInfo::T_TOOL_GANN_LINE315:	nTool = CAnalysis::EN_ANALYSIS_GANN_LINE315;
													break;
		case CToolOptionInfo::T_TOOL_GANN_FAN45:	nTool = CAnalysis::EN_ANALYSIS_GANN_FAN45;
													break;
		case CToolOptionInfo::T_TOOL_GANN_FAN135:	nTool = CAnalysis::EN_ANALYSIS_GANN_FAN135;
													break;
		case CToolOptionInfo::T_TOOL_GANN_FAN225:	nTool = CAnalysis::EN_ANALYSIS_GANN_FAN225;
													break;
		case CToolOptionInfo::T_TOOL_GANN_FAN315:	nTool = CAnalysis::EN_ANALYSIS_GANN_FAN315;
													break;
		case CToolOptionInfo::T_TOOL_GANN_GRID:		nTool = CAnalysis::EN_ANALYSIS_GANN_GRID;
													break;
		case CToolOptionInfo::T_TOOL_ANDREWS_PITCHFORK:	nTool = CAnalysis::EN_ANALYSIS_ANDREWS_PITCHFORK;
													break;
		case CToolOptionInfo::T_TOOL_ELLIOT_WAVE_LINE:nTool = CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE;
													break;
		case CToolOptionInfo::T_TOOL_CYCLE_LINES:	nTool = CAnalysis::EN_ANALYSIS_CYCLE_LINES;
													break;
		case CToolOptionInfo::T_TOOL_SPEED_LINE:	nTool = CAnalysis::EN_ANALYSIS_SPEED_LINE;
													break;
		case CToolOptionInfo::T_TOOL_LINEAR_REGRESSION:	nTool = CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION;
													break;
		case CToolOptionInfo::T_TOOL_RAFF_REGRESSION:nTool = CAnalysis::EN_ANALYSIS_RAFF_REGRESSION;
													break;
		case CToolOptionInfo::T_TOOL_CANDLE_LINE:	nTool = CAnalysis::EN_ANALYSIS_CANDLE_LINE;
													break;
		case CToolOptionInfo::T_TOOL_CANDLE_BONG:	nTool = CAnalysis::EN_ANALYSIS_CANDLE_BONG;
													break;
		case CToolOptionInfo::T_TOOL_ANGLE_LINE:	nTool = CAnalysis::EN_ANALYSIS_ANGLE_LINE;
													break;
		case CToolOptionInfo::T_TOOL_SPEED_ARC:		nTool = CAnalysis::EN_ANALYSIS_SPEED_ARC;
													break;
		case CToolOptionInfo::T_TOOL_SPEED_FAN:		nTool = CAnalysis::EN_ANALYSIS_SPEED_FAN;
													break;
		case CToolOptionInfo::T_TOOL_ADJUST_RATIO:	nTool = CAnalysis::EN_ANALYSIS_ADJUST_RATIO;
													break;
		case CToolOptionInfo::T_TOOL_PENCIL_LINE:	nTool = CAnalysis::EN_ANALYSIS_PENCIL_LINE;
													break;
		case CToolOptionInfo::T_REMOVE_OBJ:			nTool = CAnalysis::EN_ANALYSIS_SELECTION;
													m_pAnalysisTool->DeleteCurrentElement();	// Capture the command in success.															
													break;
		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
		case CToolOptionInfo::T_TOOL_BALANCE_BASIC:	nTool = CAnalysis::EN_ANALYSIS_BALANCE_BASIC;
			break;
		// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<

		case CToolOptionInfo::T_TOOL_SELECTION:			
		default:									nTool = CAnalysis::EN_ANALYSIS_SELECTION;
													break;
	}

	//Property Loading
	if(	m_eToolOption != CToolOptionInfo::T_CROSSHAIRS_TOOL_WITH_NIDLG  &&
		m_eToolOption != CToolOptionInfo::T_TOOL_SELECTION  &&
		m_eToolOption != CToolOptionInfo::T_REMOVE_OBJ		&&
		m_eToolOption != CToolOptionInfo::T_DELETE_ALL)
	{
		ST_ANALYSIS_TOOL_INFO stAnalysisTool;
		if(nTool != CAnalysis::EN_ANALYSIS_DOWN_CROSS && LoadData(nTool,stAnalysisTool) == TRUE)
		{ 
			m_pAnalysisTool->SetLineDrawingInfo(nTool,stAnalysisTool.crLineColor,stAnalysisTool.nLineStyle,stAnalysisTool.nLineWeight);

			//Text표현
			if( nTool == CAnalysis::EN_ANALYSIS_MOVE_CROSS  ||						
				nTool == CAnalysis::EN_ANALYSIS_VERT_LINE	||
				nTool == CAnalysis::EN_ANALYSIS_HORZ_LINE	||
				nTool == CAnalysis::EN_ANALYSIS_LINE		||
				nTool == CAnalysis::EN_ANALYSIS_TRISECT		||
				nTool == CAnalysis::EN_ANALYSIS_QUARTER		||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_RETRACE||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_CIRCLE||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE ||
				nTool == CAnalysis::EN_ANALYSIS_GANN_FAN45	||
				nTool == CAnalysis::EN_ANALYSIS_GANN_FAN135	||
				nTool == CAnalysis::EN_ANALYSIS_GANN_FAN225	||
				nTool == CAnalysis::EN_ANALYSIS_GANN_FAN315)
			{
				m_pAnalysisTool->SetTextPosition(nTool,
							stAnalysisTool.nLeftText==1,stAnalysisTool.nRightText==1,
							stAnalysisTool.nTopText==1,stAnalysisTool.nBottomText==1, stAnalysisTool.nRateText);
			}

			if( nTool == CAnalysis::EN_ANALYSIS_LINE		||
				nTool == CAnalysis::EN_ANALYSIS_TRISECT		||
				nTool == CAnalysis::EN_ANALYSIS_QUARTER		||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_RETRACE)
			{
				m_pAnalysisTool->SetLineExtEnvironment(nTool,stAnalysisTool.nLeftExt,stAnalysisTool.nRightExt);
			}

			if( nTool == CAnalysis::EN_ANALYSIS_TRISECT		||
				nTool == CAnalysis::EN_ANALYSIS_QUARTER)
			{
				m_pAnalysisTool->SetEquiDivEnvironment(nTool,stAnalysisTool.nEquiDiv);
				// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
				m_pAnalysisTool->SetPriceRangeDivEnvironment(nTool,stAnalysisTool.nPriceRangeDiv);
			}

			// 2008.08.20 김진순 피보나치 확장	<<
			if( nTool == CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE ||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_CIRCLE		||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE	||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_RETRACE	||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_TIME		)
			{
				m_pAnalysisTool->SetFiboCoefficient(nTool,stAnalysisTool.strFiboCoefficient);
			}
			//>>

			// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
			if( nTool == CAnalysis::EN_ANALYSIS_BALANCE_BASIC )
				m_pAnalysisTool->SetBalanceCoefficient(nTool,stAnalysisTool.strFiboCoefficient);
			// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<
		}
		else	//default 표현
		{
			//Text표현
			if( nTool == CAnalysis::EN_ANALYSIS_MOVE_CROSS  ||
				nTool == CAnalysis::EN_ANALYSIS_DOWN_CROSS)
			{
				m_pAnalysisTool->SetTextPosition(nTool,false,true,false,true);
			}

			if(nTool == CAnalysis::EN_ANALYSIS_VERT_LINE)
			{
				m_pAnalysisTool->SetTextPosition(nTool,false,false,false,true);
			}

			if(nTool == CAnalysis::EN_ANALYSIS_HORZ_LINE)
			{
				m_pAnalysisTool->SetTextPosition(nTool,false,true,false,false);
			}

			if(	nTool == CAnalysis::EN_ANALYSIS_LINE)
			{
				m_pAnalysisTool->SetTextPosition(nTool,true,true,false,false);
				m_pAnalysisTool->SetLineExtEnvironment(nTool,0,0);
			}

			if( nTool == CAnalysis::EN_ANALYSIS_TRISECT		||
				nTool == CAnalysis::EN_ANALYSIS_QUARTER	)						
			{
				m_pAnalysisTool->SetTextPosition(nTool,true,true,false,false);
				m_pAnalysisTool->SetLineExtEnvironment(nTool,0,0);
				m_pAnalysisTool->SetEquiDivEnvironment(nTool,0);
				// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
				m_pAnalysisTool->SetPriceRangeDivEnvironment(nTool,0);
			}

			if( nTool == CAnalysis::EN_ANALYSIS_FIBO_RETRACE ||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_CIRCLE||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE)
			{
				m_pAnalysisTool->SetTextPosition(nTool,true,false,false,false);
				m_pAnalysisTool->SetLineExtEnvironment(nTool,0,0);
			}

			// 2008.08.20 김진순 피보나치 확장	<<
			if( nTool == CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE ||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_CIRCLE		||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE	||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_RETRACE	||
				nTool == CAnalysis::EN_ANALYSIS_FIBO_TIME		)
			{
				m_pAnalysisTool->SetFiboCoefficient(nTool,"");
			}
			//>>

			// (2010/4/17 - Seung-Won, Bae) Set Default LineWidth to 2.
			m_pAnalysisTool->SetLineDrawingInfo( nTool, m_pIChartOCX->GetStyleValueAnalysisToolColor(), PS_SOLID,2);
		}
	}				
	 
	m_pAnalysisTool->SetCoordinateType( CAnalysis::RELATIVE_POINT);	// 상대 좌표형 - data 형태

	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart with New Tool.
	if( p_nCmdOption == 0 || m_pIChartManager->IsEmpty())  
		nTool = CAnalysis::EN_ANALYSIS_SELECTION;

	// (2006/6/28 - Seung-Won, Bae) with NIDLG
	SetCurrentAnalysisTool(nTool);
	
	// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
	// (2006/2/1 - Seung-Won, Bae) Manage Serial Drawing Mode
	if( CAnalysis::EN_ANALYSIS_SELECTION == nTool || CAnalysis::EN_ANALYSIS_NONE == nTool)
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
	else	
		m_pIAddInToolMgr->GetAllMouseExclusive( m_pIAddInDllBase);			
}

BOOL CAnalysisAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( !m_pAnalysisTool) 
		return FALSE;
 
	switch( p_eCommandID)
	{
		case EAI_BAS_OCXELEMENTMENU:	
			return m_pAnalysisTool->OnMenu( ( CMenuType::ELEMENTMENU)p_lParam1, p_lParam2);
	}
	return FALSE;	// for Crosshairs and NIDLG Tool Release
}

//OrderManager AddInToolCommand,UnDoRedoManager AddInToolCommand
BOOL CAnalysisAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	// 20081007 JS.Kim	사용자시간대 선택
	if( !m_pAnalysisTool) return FALSE;

	switch( p_eCommandID)
	{
		case EAI_TOOLORDERMGR_DELETE_THIS_TOOL:
			if( strcmp(p_szParam,m_strAddInItemName) == 0 )
				return m_pAnalysisTool->DeleteElementPos(p_lParam);
		case EAI_UNDO_REDO_MANAGER_UNDO_THIS_WORK:
			if( strcmp(p_szParam,m_strAddInItemName) == 0 )
				return m_pAnalysisTool->UndoElement(p_lParam);			
		case EAI_UNDO_REDO_MANAGER_REDO_THIS_WORK:
			if( strcmp(p_szParam,m_strAddInItemName) == 0 )
				return m_pAnalysisTool->RedoElement(p_lParam);
		// 20081007 JS.Kim	사용자시간대 선택
		case EAI_USER_CHANGE_TIMEDIFF:
			m_pAnalysisTool->RecalcElementXPoint(p_szParam, (int)p_lParam);
			return FALSE;
	}
	
	return FALSE;	
}

BOOL CAnalysisAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	m_ptOldMouse = point;
	if( !m_pAnalysisTool) return FALSE;

	// (2006/7/3 - Seung-Won, Bae) With NIDLG
	//if(m_bWithNIDLG || m_bWithTRACINGLG) 
	//	m_pIAddInToolMgr->OnAddInToolCommand( EAI_NIDLG_DRAG_ONMOUSEMOVE, pDC, nFlags, point, p_dwMouseActionID);
	
	//return m_pAnalysisTool->OnMouseMove( pDC, point,nFlags,p_hbmpMainOSB);
	m_pAnalysisTool->OnMouseMove( CDC::FromHandle( p_hDC), point,nFlags,p_hbmpMainOSB);
	return FALSE;
}

BOOL CAnalysisAddInImp::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( VK_DELETE == nChar && m_pAnalysisTool) 
	{		
		return m_pAnalysisTool->DeleteCurrentElement();
	}
	return FALSE;
}

BOOL CAnalysisAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pAnalysisTool) return FALSE;
	if( !m_pAnalysisTool->OnLButtonDown( CDC::FromHandle( p_hDC), point)) return FALSE;

	// (2006/3/7 - Seung-Won, Bae) Manage All Exclusive Flag between OnLButtonDown and OnLButtonUp.
	m_bOnAllMouseExclusive = m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase);
	if( !m_bOnAllMouseExclusive) 
		m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase);
	
	// (2006/2/1 - Seung-Won, Bae) Release Other Tools Selection
	m_pIAddInToolMgr->OnAddInToolCommand( EAI_ALL_RELEASE_SELECTION_EXCEPT_ONE, m_pIAddInDllBase);

	// (2006/2/16 - Seung-Won, Bae) Manage Flag for LButtonUp Case
	m_dwMouseActionID = p_dwMouseActionID;

	return TRUE;
}

BOOL CAnalysisAddInImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pAnalysisTool) return FALSE;
	
	CAnalysis::TOOLTYPE nType = m_pAnalysisTool->GetCurrentToolType();
	if( nType != CAnalysis::EN_ANALYSIS_NONE && nType != CAnalysis::EN_ANALYSIS_SELECTION)
		return FALSE;

	ML_SET_LANGUAGE_RES();	// to Load Menu in this Module

	if( !m_pAnalysisTool->OnLButtonDbClk( CDC::FromHandle( p_hDC), point)) return FALSE;

	// (2006/2/16 - Seung-Won, Bae) Manage Flag for LButtonUp Case
	m_dwMouseActionID = p_dwMouseActionID;

	return TRUE;
}
	
BOOL CAnalysisAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pAnalysisTool || p_dwMouseActionID != m_dwMouseActionID) return FALSE;

	// (2006/3/7 - Seung-Won, Bae) Manage All Exclusive Flag between OnLButtonDown and OnLButtonUp.
	if( !m_bOnAllMouseExclusive) m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase);

	CAnalysis::TOOLTYPE nCurr = m_pAnalysisTool->GetCurrentToolType();
	BOOL bResult = m_pAnalysisTool->OnLButtonUp( CDC::FromHandle( p_hDC), point);
	if( nCurr != m_pAnalysisTool->GetCurrentToolType())
	{
		// (2006/6/28 - Seung-Won, Bae) Why?
		// m_pIChartOCX->ReleaseToolUI( GetToolType( nCurr));

		// (2006/3/7 - Seung-Won, Bae) Tool Change on LButtonUp means Releasing of Tool.
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
	}
	// 20080916 김진순  분석도구 그리고 나면 무조건 연속그리기해제
	else if( nCurr != CAnalysis::EN_ANALYSIS_MOVE_CROSS			// 십자선(mouse move 시)
		&& nCurr !=	CAnalysis::EN_ANALYSIS_DOWN_CROSS		// 십자선(mouse down 시)
		&& nCurr !=	CAnalysis::EN_ANALYSIS_VERT_LINE		// 수직선
		&& nCurr !=	CAnalysis::EN_ANALYSIS_HORZ_LINE		// 수평선
		&& nCurr !=	CAnalysis::EN_ANALYSIS_LINE			// 추세선
		&& nCurr !=	CAnalysis::EN_ANALYSIS_PENCIL_LINE		// 연필추세선
		&& !m_pAnalysisTool->DoesAction() )
	{
		m_pIChartOCX->OnToolStateChanged(m_eToolOption,FALSE);
		SetCurrentAnalysisTool( CAnalysis::EN_ANALYSIS_SELECTION);
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
		m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;
	}
	
	return bResult;
}

// (2006/1/26 - Seung-Won, Bae) Show Popup Menu
BOOL CAnalysisAddInImp::OnRButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pAnalysisTool) return FALSE;

	ML_SET_LANGUAGE_RES();	// to Load Menu in this Module

	// 1. Check Menu
	BOOL bMenuLoaded = FALSE;
	CMenu Context;
	CAnalysis::TOOLTYPE nType = m_pAnalysisTool->GetCurrentToolType();
	if( nType != CAnalysis::EN_ANALYSIS_NONE && 
		nType != CAnalysis::EN_ANALYSIS_SELECTION &&
		nType != CAnalysis::EN_ANALYSIS_DOWN_CROSS)						
		bMenuLoaded = Context.LoadMenu( IDR_ANALYSIS);
	
	BOOL bVirtualCandle = FALSE;
	if( !bMenuLoaded) 
	{
		if( m_pAnalysisTool->OnRButtonUp( CDC::FromHandle( p_hDC), point) == CMenuType::MENU_ANALYSTOOL)	
			bMenuLoaded = Context.LoadMenu( IDR_ANALYSIS_CONFIG);

		if(bMenuLoaded == TRUE && m_pAnalysisTool->GetSelectedToolType() == CToolEnumData::Candle_Bong)
			bVirtualCandle = TRUE;
	}

	if( !bMenuLoaded) 
		return FALSE;

	//>> (2008/12/16 - Seung-Lyong Park) 분석도구 복사
	BOOL bPencilLine = FALSE;
	if( m_pAnalysisTool->GetSelectedToolType() == CToolEnumData::Pencil_Line)
		bPencilLine = TRUE;
	//<< (2008/12/16 - Seung-Lyong Park) 분석도구 복사

	// 2. Show Menu
	CMenu * pPopup = Context.GetSubMenu( 0);
	if( pPopup)
	{
		if(bVirtualCandle == TRUE)
			pPopup->RemoveMenu(ID_CAA_CONFIG,MF_BYCOMMAND);

	//>> (2008/12/16 - Seung-Lyong Park) 분석도구 복사
		if(bPencilLine == TRUE)
			pPopup->RemoveMenu(ID_CAA_COPY,MF_BYCOMMAND);
	//<< (2008/12/16 - Seung-Lyong Park) 분석도구 복사

		CPoint pt( point);
		m_pChartCWnd->ClientToScreen( &pt);
		pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_pChartCWnd, NULL);
	}
	Context.DestroyMenu();
	return TRUE;
}

BOOL CAnalysisAddInImp::LoadData(int nTool,ST_ANALYSIS_TOOL_INFO &stAnalysisTool)
{
	CString strFileName;
	strFileName.Format( "%s%s", m_strUserPath ,G_ANALYSIS_PROPERTY_INFO);
	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	finder.Close();
	
	stAnalysisTool.nLineStyle = -1;
	if(bFind == FALSE)
		return FALSE;
	
	COLORREF crDefault = m_pIChartOCX->GetStyleValueAnalysisToolColor();
	CString strSection,strKey,strData,strName;	
	//strSection = m_strANSTitle;	
	strSection.Format("%d",nTool);
	
	// (2008/9/8 - Seung-Won, Bae) Check Section Existing
	char szSectionNames[ 256];
	GetPrivateProfileSectionNames( szSectionNames, 256, strFileName);
	char *szSectionName = szSectionNames;
	while( *szSectionName)
	{
		if( !strcmp( szSectionName, strSection)) break;
		szSectionName += strlen(szSectionName) + 1;
	}
	if( !*szSectionName) return FALSE;

	strKey = _T("TopText");
	stAnalysisTool.nTopText = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("BottomText");
	stAnalysisTool.nBottomText = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("LeftText");
	stAnalysisTool.nLeftText = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("RightText");
	stAnalysisTool.nRightText = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("LeftExt");
	stAnalysisTool.nLeftExt = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("RightExt");
	stAnalysisTool.nRightExt = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("EquiDiv");
	stAnalysisTool.nEquiDiv = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	strKey = _T("PriceRangeDiv");
	stAnalysisTool.nPriceRangeDiv = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("HLPercent");
	stAnalysisTool.nHLPercent = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("FillRegion");
	stAnalysisTool.nFillRegion = GetPrivateProfileInt(strSection,strKey,0,strFileName);
	
	strKey = _T("LineColor");
	stAnalysisTool.crLineColor = (COLORREF)GetPrivateProfileInt(strSection,strKey,crDefault,strFileName);

	strKey = _T("LineStyle");
	stAnalysisTool.nLineStyle = GetPrivateProfileInt(strSection,strKey,PS_SOLID,strFileName);

	strKey = _T("LineWeight");
	stAnalysisTool.nLineWeight = GetPrivateProfileInt(strSection,strKey,1,strFileName);

	// 2008.08.20 김진순 피보나치 확장	<<	피보계수
	char	tmp[128];
	strKey = _T("FiboCoefficient");
	GetPrivateProfileString(strSection,strKey,"",tmp,sizeof(tmp)-1,strFileName);
	stAnalysisTool.strFiboCoefficient = tmp;
	//>>

	strKey = _T("RightRateText");
	stAnalysisTool.nRateText = GetPrivateProfileInt(strSection,strKey,1,strFileName);

	return TRUE;
}

void CAnalysisAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	if(p_pIAddInEnvMap->GetCount() <= 0)
		return;

	const char *chToolLoading;
	CString strToolLoading;	
	p_pIAddInEnvMap->Lookup(m_strAddInItemName,chToolLoading);
	strToolLoading.Format("%s",chToolLoading);
	if(strToolLoading.IsEmpty() == TRUE)
		return;

	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	if( strToolLoading.Left( 5) == "CODE:")
	{
		int nItemInfoLen = strToolLoading.Find( "\r\n");
		CString strItemInfo = strToolLoading.Mid( 5, nItemInfoLen - 5);
		strToolLoading = strToolLoading.Mid( nItemInfoLen + 2);

		nItemInfoLen = strItemInfo.Find( " TIME:");
		m_strItemCode = strItemInfo.Left( nItemInfoLen);
		CString strTimeType = strItemInfo.Mid( nItemInfoLen + 6);
		m_eTimeType = ( CScaleBaseData::HORZSCALEDRAWERTYPE)atoi( strTimeType);
	}

	m_pAnalysisTool->BuildElementsFromString(strToolLoading);	
}

void CAnalysisAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	CString strSaveToolList = m_pAnalysisTool->GetSavedElementsData();

	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	if( !strSaveToolList.IsEmpty())
	{
		CString strItemInfo;
		strItemInfo.Format( "CODE:%s TIME:%d\r\n", m_strItemCode, m_eTimeType);
		strSaveToolList = strItemInfo + strSaveToolList;
	}

	p_pIAddInEnvMap->SetAt(m_strAddInItemName,strSaveToolList);
}