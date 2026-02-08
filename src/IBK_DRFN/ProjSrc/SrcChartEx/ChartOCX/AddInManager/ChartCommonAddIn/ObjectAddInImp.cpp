// ObjectAddInImp.cpp: implementation of the CObjectAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "ObjectAddInImp.h"

#include "../../Include_Chart/DLL_Load/IMetaTable.h"	// for ML_LANGUAGE_RES
#include "../../Include_Chart/BaseData.h"				// for CTimeDiffInfo  // 20081007 JS.Kim	사용자시간대 선택
#include "../../Include_Chart/MessageDefine.h"			// for UDM_CHANGE_TIMEDIFF // 20081007 JS.Kim	사용자시간대 선택
#include "../Include_AddIn/_resource.h"					// for ID_COA_...
#include "../Include_AddIn/AddInCommand.h"				// for EAI_ALL_RESET_CURRENT_TOOL
#include "../Include_AddIn/I000000/_IChartManager.h"	// for IChartManager
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"	// for IAddInToolMgr
#include "../Include_AddIn/I000000/_ICmdUI.h"			// for ICmdUI
#include "../Include_AddIn/I000000/_IChartOCX.h"		// for IChartOCX
#include "../Include_AddIn/I000000/_IString.h"			// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectAddInImp::CObjectAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2007/6/25 - Seung-Won, Bae) Support Dynamic Loading of ChartObject.dll
	m_pBlockTool = NULL;
	ILPCSTR szOCXPath = p_pIChartOCX->GetOCXPathString();
	m_hBlockTool = LoadLibraryEx( CString( szOCXPath) + "ChartObject.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if( m_hBlockTool)
	{
		FN_CreateBlockTool fnCreateBlockTool = ( FN_CreateBlockTool)GetProcAddress( m_hBlockTool, "CreateBlockTool");
		if( fnCreateBlockTool) m_pBlockTool = fnCreateBlockTool();
	}

	if( m_pBlockTool) m_pBlockTool->SetIChartCtrl( m_pIChartOCX->GetIChartCtrl());

	// 20081007 JS.Kim	사용자시간대 선택.	
	m_nCountTimeDiff = 0;	//사용자가 선택하게 될 시간대 갯수
	m_bReadInfo = false;
}

CObjectAddInImp::~CObjectAddInImp()
{
	// (2007/6/25 - Seung-Won, Bae) Support Dynamic Loading of ChartObject.dll
	if( m_pBlockTool)
	{
		delete m_pBlockTool;
		m_pBlockTool = NULL;
	}
	if( m_hBlockTool)
	{
		FreeLibrary( m_hBlockTool);
		m_hBlockTool = NULL;
	}

	// 20081007 JS.Kim	사용자시간대 선택
	CTimeDiffInfo *pTimeInfo;
	POSITION pos = m_lstTimeDiffInfo.GetHeadPosition();
	while( pos )
	{
		pTimeInfo = (CTimeDiffInfo *)m_lstTimeDiffInfo.GetNext(pos);
		if( pTimeInfo )		delete pTimeInfo;
	}
	m_lstTimeDiffInfo.RemoveAll();
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CObjectAddInImp::m_strAddInItemName = "OBJECT_TOOL";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CObjectAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnResetMainBlock)
	ONADDINEVENT( OnChartItemChanged)
	ONADDINEVENT( OnGraphDataChanged)
	ONADDINEVENT( OnBlockRemoved)
	ONADDINEVENT( OnGraphAdded)
	ONADDINEVENT( OnGraphDeleted)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_char)
	ONADDINEVENT( OnKeyDown)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnLButtonDblClk)
	ONADDINEVENT( OnRButtonUp)
END_ADDIN_EVENT_MAP()

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
IUnknown *CObjectAddInImp::GetAddInCustomInterface( void)
{
// (2006/1/23 - Seung-Won, Bae) Do not use it. Use AddIn Command!
/*
	IUnknown *pIUnknown = ( IUnknown *)( IChartObjectAddIn *)this;
	pIUnknown->AddRef();
	return pIUnknown;
*/
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
//////////////////////////////////////////////////////////////////////

// (2006/1/17 - Seung-Won, Bae) Drawing Interface
void CObjectAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	if( m_pBlockTool) m_pBlockTool->OnDraw( CDC::FromHandle( p_hDC), m_pIChartManager->GetObjectAddress());
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CObjectAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( m_pBlockTool) m_pBlockTool->ResetCurrentTool();
}

// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
void CObjectAddInImp::OnResetMainBlock( void)
{
	if( m_pBlockTool) m_pBlockTool->ResetCurrentTool();
}

// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
void CObjectAddInImp::OnChartItemChanged( void)
{
	if( m_pBlockTool) m_pBlockTool->ResetCurrentTool();
}

// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
void CObjectAddInImp::OnGraphDataChanged( void)
{
	if( m_pBlockTool) m_pBlockTool->ResetCurrentTool();
}

// (2006/9/25 - Seung-Won, Bae) On Block Removed.
void CObjectAddInImp::OnBlockRemoved( const int p_nRow, const int p_nColumn)
{
	if( m_pBlockTool) m_pBlockTool->ResetCurrentTool();
}

// (2006/9/25 - Seung-Won, Bae) On Graph Added.
void CObjectAddInImp::OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)
{
	if( m_pBlockTool) m_pBlockTool->ResetCurrentTool();
}

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CObjectAddInImp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	if( m_pBlockTool) m_pBlockTool->ResetCurrentTool();
}

// (2006/9/27 - Seung-Won, Bae) On Block Changed.
void CObjectAddInImp::OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)
{
	if( CBlockBaseData::BLOCK_DELETE == p_eChangeType && m_pBlockTool) m_pBlockTool->ResetCurrentTool();
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CObjectAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
			// (2009/6/5 - Seung-Won, Bae) Support on ENV_SETUP addin.
			case ID_COA_CONFIG:
				if( p_pICmdUI) p_pICmdUI->Enable( m_pIAddInToolMgr->IsAddInLoaded( "ChartEnvSetupDlgAddIn.dll:ENV_SETUP"));
				break;

			// (2009/6/5 - Seung-Won, Bae) Support invert on only complex r-button menu type.
			case ID_COA_INVERT:		// 지표 뒤집기 - ojtaso (20071023)
				if( m_pBlockTool && p_pICmdUI)
				{
					p_pICmdUI->SetCheck( m_pBlockTool->GetMenuCheck( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_INVERTSCALE,				TRUE));
					p_pICmdUI->Enable( m_pIChartOCX->GetGraphRButtonType());
				}
				break;

			// (2009/6/5 - Seung-Won, Bae) Support invert on only complex r-button menu type and on supporting mode
			case ID_COA_DELETE:
			case ID_COA_DELETE_ALL:
				if( p_pICmdUI) p_pICmdUI->Enable( m_pIChartOCX->GetGraphRButtonType() && m_pIChartOCX->CanGraphMoveAndDelete());
				break;

			case ID_COA_SCALE_X_SHOW:
			case ID_COA_SCALE_X_HIDE:
			// (2009/6/5 - Seung-Won, Bae) Use Other id for graph config option.
			case ID_COA_CONFIG_SCALE:
				if( p_pICmdUI) p_pICmdUI->Enable( TRUE);
				break;

			// 20081007 JS.Kim	사용자시간대 선택.	사용자가 선택하게 될 시간대 메뉴 구성을 위한 시작 ID
			//default: return FALSE;
			case ID_COA_TIMEDIFF_0:
				if( p_pICmdUI) p_pICmdUI->Enable( TRUE );
				break;
			default: 
				if( ( unsigned int)ID_COA_TIMEDIFF_0 < nID && nID <= ( unsigned int)( ID_COA_TIMEDIFF_0 + m_nCountTimeDiff))
				{
					if( p_pICmdUI) p_pICmdUI->Enable(TRUE);
					if( m_pBlockTool) 
					{
						POSITION pos = m_lstTimeDiffInfo.FindIndex(nID-ID_COA_TIMEDIFF_0 - 1);
						if( pos )
						{
							CTimeDiffInfo *pTimeInfo = (CTimeDiffInfo *)m_lstTimeDiffInfo.GetAt(pos);
							p_pICmdUI->SetCheck(m_pBlockTool->GetMenuCheck( m_pIChartManager->GetObjectAddress(), 
												CMenuType::MENU_TIME_DIFF, 
												WPARAM(pTimeInfo)));
						}
					}
					break;
				}
				return FALSE;
		}
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		switch( nID)
		{
			// (2009/6/5 - Seung-Won, Bae) Use Other id for graph config option.
			case ID_COA_CONFIG:
			case ID_COA_CONFIG_SCALE:	if( m_pBlockTool) m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_SET,					m_pIChartOCX->GetBongCountMargin());
										break;
			// 지표 뒤집기 - ojtaso (20071023)
			case ID_COA_INVERT:			if( m_pBlockTool) m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_INVERTSCALE,				TRUE);
										break;
			case ID_COA_DELETE:			if( m_pBlockTool) m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_DELETE,				TRUE);
										break;
			case ID_COA_DELETE_ALL:		if( m_pBlockTool) m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_DELETE_ALL,				TRUE);
										break;
			case ID_COA_SCALE_X_SHOW:	if( m_pBlockTool) m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_HORZSCALE_POSITION,	CScaleBaseData::HORZ_BOTTOM);
										break;
			case ID_COA_SCALE_X_HIDE:	if( m_pBlockTool) m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_HORZSCALE_POSITION,	CScaleBaseData::HORZ_HIDE);
										break;
			// 20081007 JS.Kim	사용자시간대 선택.	사용자가 선택하게 될 시간대 메뉴 구성을 위한 시작 ID
			//default:	return FALSE;
			case ID_COA_TIMEDIFF_0 :
				m_pIAddInToolMgr->OnAddInToolCommand( EAI_USER_CHANGE_TIMEDIFF, (const char *)"", (long)0);
				if( m_pBlockTool)  m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_TIME_DIFF, NULL);
				// 외부에 시간대 선택이 바뀌었음을 알린다
				if( m_pIChartOCX ) ::SendMessage( m_pIChartOCX->GetOcxHwnd(), UDM_NOTIFY_TIMEDIFF, -1, -1);
				break;
			default: 
				if( ( unsigned int)ID_COA_TIMEDIFF_0 < nID && nID <= ( unsigned int)( ID_COA_TIMEDIFF_0 + m_nCountTimeDiff))
				{
					POSITION pos = m_lstTimeDiffInfo.FindIndex(nID-ID_COA_TIMEDIFF_0 - 1);
					if( pos && m_pBlockTool )
					{
						CTimeDiffInfo *pTimeInfo = (CTimeDiffInfo *)m_lstTimeDiffInfo.GetAt(pos);
						m_pIAddInToolMgr->OnAddInToolCommand( EAI_USER_CHANGE_TIMEDIFF, (const char *)pTimeInfo->m_strName, (long)pTimeInfo->m_nDiffTime);
						m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_TIME_DIFF, WPARAM(pTimeInfo));
						// 외부에 시간대 선택이 바뀌었음을 알린다
						if( m_pIChartOCX ) ::SendMessage(m_pIChartOCX->GetOcxHwnd(), UDM_NOTIFY_TIMEDIFF, pTimeInfo->m_nIndex, -1);
					}
					return TRUE;
				}
				return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)
{
	if( EAI_ALL_RELEASE_SELECTION_EXCEPT_ONE == p_eCommandID && p_pParam != m_pIAddInDllBase && m_pBlockTool)
			m_pBlockTool->ResetCurrentTool();

	return FALSE;
}

BOOL CObjectAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( EAI_OBJECT_SET_USING_TOOL1 == p_eCommandID)
	{
		if( m_pBlockTool) m_pBlockTool->SetUsingTool( ( CBlockBaseData::TOOLTYPE)p_lParam1, ( 0 == p_lParam2 ? false : true));
		return TRUE;
	}
	// return FALSE to pass the Event to a next AddIn Routine.
	return FALSE;
}

BOOL CObjectAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)
{
	if( EAI_OBJECT_SET_USING_TOOL2 == p_eCommandID)
	{
		if( m_pBlockTool) m_pBlockTool->SetUsingTool(	( 0 == p_lParam1 ? false : true),
									( 0 == p_lParam2 ? false : true),
									( 0 == p_lParam3 ? false : true),
									( 0 == p_lParam4 ? false : true),
									( 0 == p_lParam5 ? false : true),
									( 0 == p_lParam6 ? false : true),
									( 0 == p_lParam7 ? false : true),
									( 0 == p_lParam8 ? false : true),
									( 0 == p_lParam9 ? false : true),
									( 0 == p_lParam9 ? false : true));	// 블럭 최대화 추가 - ojtaso (20080519)
		return TRUE;
	}
	// return FALSE to pass the Event to a next AddIn Routine.
	return FALSE;
}

BOOL CObjectAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( EAI_BAS_OCXELEMENTMENU == p_eCommandID && m_pBlockTool) return m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), ( CMenuType::ELEMENTMENU)p_lParam1, p_lParam2);

	// 20081007 JS.Kim	사용자시간대 선택.	// 주어진 시간대 Index 로 CTimeDiffInfo pointer를 구한다
	else if( EAI_USER_CHANGE_TIMEDIFF == p_eCommandID)
	{
		const int nID = (const int)p_lParam1;
		if( nID < 0 )
		{
			p_lResult = 0;
		}
		else
		{
			if( !m_bReadInfo )			LoadTimeDiffConf();

			CTimeDiffInfo *pTimeInfo;
			POSITION pos = m_lstTimeDiffInfo.GetHeadPosition();
			while(pos)
			{
				pTimeInfo = (CTimeDiffInfo *)m_lstTimeDiffInfo.GetNext(pos);
				if( pTimeInfo && pTimeInfo->m_nIndex == nID )
				{
					p_lResult = (long)pTimeInfo;
					break;
				}
			}
		}
		return TRUE;
	}
	
	// return FALSE to pass the Event to a next AddIn Routine.
	return FALSE;
}

BOOL CObjectAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)
{
	if( EAI_USER_CHANGE_TIMEDIFF == p_eCommandID)		// 주어진 시간대 도시 이름으로 시간대 Index를 구한다.
	{
		if( strlen(p_szParam) == 0 )		
		{
			p_lResult = -1;
			return TRUE;
		}

		if( !m_bReadInfo )			LoadTimeDiffConf();

		CTimeDiffInfo *pTimeInfo;
		POSITION pos = m_lstTimeDiffInfo.GetHeadPosition();
		while(pos)
		{
			pTimeInfo = (CTimeDiffInfo *)m_lstTimeDiffInfo.GetNext(pos);
			if( pTimeInfo && pTimeInfo->m_strName.CollateNoCase(p_szParam) == 0 )
			{
				p_lResult = pTimeInfo->m_nIndex;
				break;
			}
		}
		return TRUE;
	}

	// return FALSE to pass the Event to a next AddIn Routine.
	return FALSE;
}

BOOL CObjectAddInImp::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// (2006/10/26 - Seung-Won, Bae) Deny in DesignTime.
	if( !m_pIChartOCX->IsRuntimeMode()) return FALSE;

	// (2006/1/22 - Seung-Won, Bae) Delete Selected Object
	if( VK_DELETE == nChar && m_pBlockTool) return m_pBlockTool->OnMenu( m_pIChartManager->GetObjectAddress(), CMenuType::MENU_DELETE, TRUE);
	return FALSE;
}

BOOL CObjectAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)		// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC || !m_pBlockTool) return FALSE;

	// (2006/1/18 - Seung-Won, Bae) Do not check Exclusive for Object Search!
	//	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase)) return TRUE;
	// (2006/1/17 - Seung-Won, Bae) Support Design Mode & RunTime
	return m_pBlockTool->OnMouseMove( pDC, m_pIChartManager->GetObjectAddress(), nFlags, point);
}

BOOL CObjectAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC || !m_pBlockTool) return FALSE;

	// (2006/1/17 - Seung-Won, Bae) Support Design & RunMode
	// if( m_pIChartOCX->IsRuntimeMode()) return FALSE;
	if( !m_pBlockTool->OnLButtonDown( pDC, m_pIChartManager->GetObjectAddress(), nFlags, point)) return FALSE;
	m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase);
	
	// (2006/2/1 - Seung-Won, Bae) Release Other Tools Selection
	m_pIAddInToolMgr->OnAddInToolCommand( EAI_ALL_RELEASE_SELECTION_EXCEPT_ONE, m_pIAddInDllBase);

	// (2006/3/21 - Seung-Won, Bae) Tooltip must work in Only Graph. (not GraphTitle)
	if( m_pBlockTool->GetCurrentToolType() == CBlockBaseData::TOOL_SUBGRAPH)
	{
		// (2006/2/15 - Seung-Won, Bae) Show Tooltip
		m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLTIP_ONLBUTTONDOWN, p_hDC, nFlags, point, p_dwMouseActionID);
	}

	return TRUE;
}

BOOL CObjectAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC || !m_pBlockTool) return FALSE;

	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase)) return FALSE;
	// (2006/1/17 - Seung-Won, Bae) Support Design & Run Mode
	CBlockBaseData::TOOLTYPE eToolType = m_pBlockTool->GetCurrentToolType();
	BOOL bResult = m_pBlockTool->OnLButtonUp( pDC, m_pIChartManager->GetObjectAddress(), nFlags, point);
	if(bResult)
	{
		if(eToolType == CBlockBaseData::TOOL_COMPARTLINE)
		{
			if(!m_pIChartOCX->IsRuntimeMode())
			{
				ILPCSTR szBlockPosition = m_pIChartManager->GetBlocksPositionString();
				m_pIChartOCX->SetStrBlockRegion( szBlockPosition);
			}
			// (2006/1/20 - Seung-Won, Bae) Support RunTime Mode
			else if( !m_pIChartOCX->IsOnSpecialChart()) m_pIChartOCX->SaveChartCFG_SaveNow();
		}
		else if(eToolType == CBlockBaseData::TOOL_SUBGRAPH)
		{
			// (2006/2/15 - Seung-Won, Bae) Show Tooltip
			m_pIAddInToolMgr->OnAddInToolCommand( EAI_TOOLTIP_ONLBUTTONUP, p_hDC, nFlags, point, p_dwMouseActionID);
		}
	}

	m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase);
	return bResult;
}

BOOL CObjectAddInImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC || !m_pBlockTool) return FALSE;

	// (2004.06.28, 배승원) Graph가 등록되어 있지 않는 상황에서는,
	//		분석 도구를 제공하지 않는다.
	// (2004.07.07. 배승원) 가격차트로 Graph가 등록되어 있지 않는 경우를 판단하는 것은,
	//		DLL Container가 있는 경우로 제한한다.
	//		(일반 Map에서는 가격차트 대신 다른 Graph가 있는 경우가 가능하다.)
// (2006/1/22 - Seung-Won, Bae) why not ? 
//	if( m_hContainerDllWnd && !m_pMainBlock->IsExistenceGraph( _CHARTDLL_CHARTTYPE_PRICE))
//		return FALSE;

	m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase);
	return m_pBlockTool->OnLButtonDblClk( pDC, m_pIChartManager->GetObjectAddress(), nFlags, point);
}

// (2006/1/21 - Seung-Won, Bae) Show Popup Menu
BOOL CObjectAddInImp::OnRButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC || !m_pBlockTool) return FALSE;

	// (2006/10/30 - Seung-Won, Bae) Support Design Time Graph Menu.
	if( m_pIChartOCX->IsRuntimeMode() && !m_pIChartOCX->IsGraphMenuShow()) return FALSE;

	CMenuType::MENUTYPE nMenuType = m_pBlockTool->OnRButtonUp( pDC, m_pIChartManager->GetObjectAddress(), 0, point);
	// scale menu
	CMenu Context;
	BOOL bResult = FALSE;
	// (2009/6/5 - Seung-Won, Bae) Set resource handle after CMainBlockTool::OnRButtonUp().
	ML_SET_LANGUAGE_RES();
	if( nMenuType == CMenuType::MENU_HORZ_SCALE) bResult = Context.LoadMenu(IDR_SCALE_X);
	else if(nMenuType == CMenuType::MENU_VERT_SCALE) bResult = Context.LoadMenu(IDR_SCALE_Y);
	// 04.03.2002  x축 scale 없는 block에서만
	// (2004.07.28, 배승원) 원래의 ID인 IDR_SCALE_NONE_X의 원형을 설정한다.
	else if(nMenuType == CMenuType::MENU_VERT_HORZ_SCALE) bResult = Context.LoadMenu(IDR_SCALE_Y_NONE_X);
	// 20081007 JS.Kim	사용자시간대 선택
	else if( nMenuType != CMenuType::MENU_NONE) bResult = MakeConfigMenu(&Context);
	if( !bResult) return FALSE;

	CMenu * pPopup = Context.GetSubMenu( 0);

	// (2009/6/5 - Seung-Won, Bae) Delete disabled menu item.
	int nMenuID = 0;
	int nMenuCount = pPopup->GetMenuItemCount();
	for( int i = nMenuCount - 1; 0 <= i; i--)
	{
		nMenuID = pPopup->GetMenuItemID( i);
		if( nMenuID < 0) continue;
		if( ID_PLM_ADD_MENU <= nMenuID && nMenuID <= ID_PLM_ADD_MENU_END) continue;
		if( !( pPopup->GetMenuState( i, MF_BYPOSITION) & MF_DISABLED)) continue;

		m_pIChartOCX->OnCmdMsg( pPopup->GetSafeHmenu(), nMenuID, i, nMenuCount, CN_UPDATE_COMMAND_UI);
		MENUITEMINFO mii;
		mii.cbSize = sizeof( MENUITEMINFO); // must fill up this field
		mii.fMask = MIIM_DATA;             // get the state of the menu item
		pPopup->GetMenuItemInfo( i, &mii, TRUE);
		if( mii.dwItemData != 9999)
		{
			pPopup->DeleteMenu( i, MF_BYPOSITION);
			nMenuCount--;
		}
	}

	m_pChartCWnd->ClientToScreen( &point);
	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, m_pChartCWnd, NULL);
	Context.DestroyMenu();
	return TRUE;
}

// (2009/9/2 - Seung-Won, Bae) Check Market Index with search.
int CObjectAddInImp::GetSubMenuIndex( CMenu *p_pMainMenu, const char *p_szMenuString)
{
	if( !p_pMainMenu || !p_szMenuString) return -1;
	int nMenuItemCount = p_pMainMenu->GetMenuItemCount();
	if( nMenuItemCount <= 0 || !*p_szMenuString) return -1;

	CString strMenuString;
	for( int i = 0; i < nMenuItemCount; i++)
	{
		p_pMainMenu->GetMenuString( i, strMenuString, MF_BYPOSITION);
		if( strMenuString == p_szMenuString) return i;
	}
	return -1;
}
CMenu *CObjectAddInImp::GetSubMenu( CMenu *p_pMainMenu, const char *p_szMenuString)
{
	int nMenuIndex = GetSubMenuIndex( p_pMainMenu, p_szMenuString);
	if( nMenuIndex <= 0) return NULL;
	return p_pMainMenu->GetSubMenu( nMenuIndex);
}


// 20081007 JS.Kim	사용자시간대 선택
BOOL CObjectAddInImp::MakeConfigMenu(CMenu *pMenu)
{
	if( !pMenu) return FALSE;
	pMenu->LoadMenu( IDR_OBJECT_CONFIG);

	CMenu * pSettingPopup = pMenu->GetSubMenu( 0);
	if( pSettingPopup == NULL )	return FALSE;

	ML_SET_LANGUAGE_RES();
	CString strUserTime;
	strUserTime.LoadString( IDS_USER_TIME);
	int nMenuIndex = GetSubMenuIndex( pSettingPopup, strUserTime);
	if( nMenuIndex < 0) return FALSE;

	// (2009/9/30 - Seung-Won, Bae) for WorldOn
	if( CChartInfo::HTS == m_eChartMode || CChartInfo::FX == m_eChartMode
		// 시간대 Configure 파일이 없거나 시간대 정보가 없는 경우는 [시간대 설정] 메뉴 삭제
		|| ( m_bReadInfo && m_nCountTimeDiff == 0 ))
	{
		pSettingPopup->DeleteMenu( nMenuIndex, MF_BYPOSITION);	// Remove User Time Scale.
		return TRUE;
	}

	// 시간대 설정 메뉴
	CMenu* pPopup = pSettingPopup->GetSubMenu( nMenuIndex);
	if( pPopup == NULL )	return FALSE;

	// 원래시간대로.. 메뉴 텍스트 편집
	//CMenu* pOrgMenu = pPopup->GetSubMenu(0);
	if( m_pBlockTool )
	{
		CTimeDiffInfo TimeInfo("", 0);
		m_pBlockTool->GetMenuCheck( m_pIChartManager->GetObjectAddress(), 
														CMenuType::MENU_TIME_DIFF, 
														WPARAM(&TimeInfo));
		CString strTimeInfo, strMenuItem;
		pPopup->GetMenuString(0, strMenuItem, MF_BYPOSITION);
		strTimeInfo.Format("%s(GMT%+03d:00)", strMenuItem, TimeInfo.m_nDiffTime);
		pPopup->ModifyMenu(0, MF_BYPOSITION, ID_COA_TIMEDIFF_0, (LPCTSTR)strTimeInfo);
	}

	if( m_bReadInfo )
	{
		POSITION pos;
		CTimeDiffInfo *pTimeInfo = NULL;
		for( int i=0; i<m_nCountTimeDiff; i++ )
		{
			pos = m_lstTimeDiffInfo.FindIndex(i);
			if( !pos )
				break;

			pTimeInfo = (CTimeDiffInfo *)m_lstTimeDiffInfo.GetAt(pos);
			if( pTimeInfo )
				pPopup->AppendMenu(MF_STRING, ID_COA_TIMEDIFF_0+1 + i, pTimeInfo->m_strName);
		}

		return TRUE;
	}

	LoadTimeDiffConf();

	if( m_nCountTimeDiff == 0 )
	{
		pSettingPopup->DeleteMenu( 1, MF_BYPOSITION);
	}
	else
	{
		CTimeDiffInfo *pTimeInfo;
		int nCnt = 0;
		POSITION pos = m_lstTimeDiffInfo.GetHeadPosition();
		while(pos)
		{
			pTimeInfo = (CTimeDiffInfo *)m_lstTimeDiffInfo.GetNext(pos);
			if( pTimeInfo )
			{
				nCnt ++;
				pPopup->AppendMenu(MF_STRING, ID_COA_TIMEDIFF_0 + nCnt, pTimeInfo->m_strName);
			}
		}
	}

	return TRUE;
}

// 20081031 JS.Kim	사용자시간대 파일(timesetup.ini) 읽어서 List 생성
void CObjectAddInImp::LoadTimeDiffConf()
{
	m_bReadInfo = true;

	// DATA 폴더에서 System 폴더로 변경
	//ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
	//CString strPath = CString( szDataPath);
	ILPCSTR szDataPath = m_pIChartOCX->GetHtsPathString();
	CString strPath = CString( szDataPath);

//	소리마치
//	strPath += "\\System";
	strPath += "\\Icss";	//	WINIX.

	m_lstTimeDiffInfo.RemoveAll();
	m_nCountTimeDiff = 0;

	int nTimeDiff = 0, nSTime = 0, nIndex = -1;
	CTimeDiffInfo	*pTimeInfo;
	unsigned int m_nTestEnvBufferSize = 1024 * 10;
	char szBuffer[ 1024 * 10];

	ML_SET_LANGUAGE_RES();
	CString strSection;
	strSection.LoadString( IDS_SET_TIMEDIFF);
	DWORD dwRead = GetPrivateProfileSection( strSection, szBuffer, m_nTestEnvBufferSize, strPath + "\\timesetup.ini");
	char *pszKey = szBuffer;
	if( 0 < dwRead && dwRead < m_nTestEnvBufferSize - 2)
	{
		// 읽어온 Info를 Parsing한다.
		char *pszRTrim = NULL, *pszNextKey = NULL, *pszValue = NULL, *p = NULL;
		while( *pszKey)
		{
			pszNextKey = pszKey + strlen(pszKey) + 1;

			pszRTrim = pszNextKey - 2;
			while(*pszRTrim == ' ' || *pszRTrim == '\t') pszRTrim--;
			*(pszRTrim + 1) = '\0';
			
			nTimeDiff = 0;
			pszValue = strstr( pszKey, "=");
			if( pszValue)
			{
				*pszValue = '\0';
				nIndex = atoi(pszKey);	// Index

				pszValue ++;
				pszKey = pszValue;		// 도시 이름
				pszValue = strchr(pszKey, ',');
				if( pszValue )
				{
					*pszValue = '\0';
					pszValue++;
					p = strchr(pszValue, ',');
					if( p )		
					{
						*p = '\0';
						nTimeDiff = atoi(pszValue);
						
						pszValue = p+1;
						p = strchr(pszValue, ',');
						if( p )		*p = '\0';
						nSTime = atoi(pszValue);

						nTimeDiff += nSTime;
					}
					pTimeInfo = new CTimeDiffInfo;
					pTimeInfo->m_nIndex = nIndex;
					pTimeInfo->m_strName = pszKey;
					if( nSTime == 1 )	pTimeInfo->m_strName = pTimeInfo->m_strName + "  섬머타임적용";		// Summer Time
					pTimeInfo->m_nDiffTime = nTimeDiff;
					m_lstTimeDiffInfo.AddTail(pTimeInfo);

					m_nCountTimeDiff++;
				}
			}
			pszKey = pszNextKey;
		}
	}
}

