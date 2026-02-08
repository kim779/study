// ChartEnvSetupDlgAddInImp.cpp: implementation of the CChartEnvSetupDlgAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "ChartEnvSetupDlgAddInImp.h"

#include "GraphSetting.h"						// for CGraphSetting
#include "DlgEnvSetUpAll.h"						// for CDlgEnvSetUpAll
#include "DlgMainTabBaseEnvSetUp.h"				// for CDlgMainTabBaseEnvSetUp

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartEnvSetupDlgAddInImp::CChartEnvSetupDlgAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
	if( p_pIChartOCX) m_hOcxWnd = p_pIChartOCX->GetOcxHwnd();
// 2008.01.25 by LYH <<

	m_bEnableWithDefaultInfo = FALSE;

	// (2007/1/19 - Seung-Won, Bae) Check OS Version. (for excepting Win98)
	OSVERSIONINFO osVersionInfo;
	m_bWin98 = FALSE;
	osVersionInfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO);
	if( GetVersionEx( &osVersionInfo)) m_bWin98 = ( osVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT);

	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	m_pIChartOCX->SetMultipleSameNameIndicator( TRUE);

	// (2007/1/23 - Seung-Won, Bae) Main Indicator CFG Dialog for Deleting State Update.
	m_pMainIndicatorCFG = NULL;
	
	// (2007/10/31 - Seung-Won, Bae) for Hide Menu
	m_bShowMenu = TRUE;
}

CChartEnvSetupDlgAddInImp::~CChartEnvSetupDlgAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartEnvSetupDlgAddInImp::m_strAddInItemName = "ENV_SETUP";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartEnvSetupDlgAddInImp)
	ONADDINEVENT( OnGraphDeleted)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_long)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
BOOL CChartEnvSetupDlgAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	if( !p_szCommandName || !p_szData) return FALSE;

	if( !strcmp( p_szCommandName, "SHOW_MENU"))
	{
		m_bShowMenu = ( *p_szData != '0');
		return TRUE;
	}
	return FALSE;
}

// (2007/10/5 - Seung-Won, Bae) for the Revised Price of Map
BOOL CChartEnvSetupDlgAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	if( p_nCommandType != 1005 || !m_pMainIndicatorCFG) return FALSE;
	CDialog *pDlg = m_pMainIndicatorCFG->m_mtabChartEnvSetUpAll.GetTabPages( 1);
	if( !pDlg) return FALSE;
	if( !pDlg->IsKindOf( RUNTIME_CLASS( CDlgMainTabBaseEnvSetUp))) return FALSE;
	CDlgMainTabBaseEnvSetUp *pBaseEnvDlg = ( CDlgMainTabBaseEnvSetUp *)pDlg;
	pBaseEnvDlg->EnableRevisedPrice( p_lData);
	return TRUE;
}

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CChartEnvSetupDlgAddInImp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	if( !m_pMainIndicatorCFG) return;
	m_pMainIndicatorCFG->OnGraphDeleted( p_szGraphName, p_szIndicatorName);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CChartEnvSetupDlgAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( !m_bShowMenu) return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
			case ID_ESP_SHOW_ENVIRONMENT_DLG:	
			case ID_ESP_SHOW_INDICATOR_DLG:		if( p_pICmdUI)
												{
													// (2007/1/4 - Seung-Won, Bae) EmptyChart can not use the EnvSetupDlg.
													ILPCSTR szAllIndicatorName = m_pIChartManager->GetAllIndicatorNameString();
													if( !szAllIndicatorName.IsEmpty()) p_pICmdUI->Enable( !m_bWin98);
													return TRUE;
												}		
			default: return FALSE;
		}
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		switch( nID)
		{
			case ID_ESP_SHOW_ENVIRONMENT_DLG:
				OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, CToolOptionInfo::T_ENVIRONMENT_CFG, 1);
				break;
			case ID_ESP_SHOW_INDICATOR_DLG:
				OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, CToolOptionInfo::T_INDICATOR_CFG, 1);
				break;
			default:	return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}


BOOL CChartEnvSetupDlgAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( EAI_OOA_ON_TOOL_FUNCTION != p_eCommandID) return FALSE;

	int nActivePage = 0;
	// 환경설정 탭별 선택 가능 - ojtaso (20080109)
	int nShowItem = LOAD_OTHER_UI;

	switch( p_lParam1)
	{
		case CToolOptionInfo::T_ENVIRONMENT_CFG:	nActivePage = 1;
													break;
		// 지표설정
		case CToolOptionInfo::T_INDICATOR_LIST:
		case CToolOptionInfo::T_INDICATOR_CFG:		nActivePage = 0;
													break;
		// 환경설정 탭별 선택 가능 - ojtaso (20080109)
		case CToolOptionInfo::T_SELECT_ENV_TAB_INDEX:
			nActivePage = p_lParam2 - 1;
			nShowItem = LOAD_SEL_ITEM;
			break;
		default:	return FALSE;
	}

	if( p_lParam2 <= 0) return FALSE;

	if( m_bWin98)
	{
		ML_SET_LANGUAGE_RES();
		AfxMessageBox(IDS_MSG_SETTING_98);
		return FALSE;
	}

	if( m_pIChartOCX->IsOnSpecialChart())
	{
		ML_SET_LANGUAGE_RES();
		AfxMessageBox(IDS_MSG_ENV_SETTING);
		return FALSE;
	}

	// 0. Block에 가격차트가 등록되어 있지 않는 경우에는 제외한다.
	ILPCSTR szAllIndicatorName = m_pIChartManager->GetAllIndicatorNameString();
	if( szAllIndicatorName.IsEmpty()) return FALSE;

	m_pIChartOCX->ReleaseOtherToolUI( CToolOptionInfo::TOOLOPTION( -1));

	// (2006/3/29 - Seung-Won, Bae) Release AddIn Tool
	m_pIAddInToolMgr->OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, CToolOptionInfo::T_TOOL_SELECTION, 0);

	m_pIChartManager->ChangeAllGraphInfoFromGraphData();

	ML_SET_LANGUAGE_RES();
	// 환경설정 탭별 선택 가능 - ojtaso (20080109)
	CDlgEnvSetUpAll dlgEnvSetUpAll( m_pIChartOCX, nShowItem, nActivePage, this, m_pChartCWnd, m_bWin98);
	m_pMainIndicatorCFG = &dlgEnvSetUpAll;
	//{{2007.06.04 by LYH 설정창 띄웠을 경우 RQ를 지우지 않도록 세팅
	IPacketManager *pIPacketManager = m_pIChartOCX->GetIDefaultPacketManager();
	if( pIPacketManager)
		pIPacketManager->SetRemovablePacketRQ(FALSE);
	//}}
	dlgEnvSetUpAll.DoModal();
	//{{2007.06.04 by LYH 원상태로 돌려줌
	if( pIPacketManager)
	{
		pIPacketManager->SetRemovablePacketRQ(TRUE);
		pIPacketManager->Release();
	}
	//}}
	m_pMainIndicatorCFG = NULL;
		
	return TRUE;
}


BOOL CChartEnvSetupDlgAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, IGraphInfo *p_pIGraphInfo)
{
	if( EAI_ENVDLG_SHOW_SETUP_DLG != p_eCommandID) return FALSE;

	if( m_bWin98)
	{
		ML_SET_LANGUAGE_RES();
		AfxMessageBox(IDS_MSG_SETTING_98);
		return FALSE;
	}

	// 1. Initialize Return Value.
	p_lResult = IDCANCEL;

	// 2. Check Indicator Info
	if( !p_pIGraphInfo) return TRUE;
	if( p_pIGraphInfo->GetSubGraphCount() <= 0) return TRUE;
	IBlock *pIBlock = NULL;
	int nR = 0, nC = 0, nV = 0, nG = 0;
	ILPCSTR szGraphName = p_pIGraphInfo->GetGraphNameString();
	if( !szGraphName.IsEmpty())
	{
		if( !m_pIChartManager->GetIndexOfGraph( szGraphName, nR, nC, nV, nG)) return TRUE;
		pIBlock = m_pIChartManager->GetBlock( nR, nC);
	}

	// 3. Get Vertical Scale Type ~ Log and Reverse
	BOOL bLogVScale = FALSE, bReverseVScale = FALSE;
	if( pIBlock)
	{
		bLogVScale = pIBlock->IsLogScale();
		bReverseVScale = pIBlock->IsVScaleInvert( nV);
	}
	
	// 4. Get Delete All Flag
	//"가격차트", "특수형" 일 경우는 삭제해선 안된다.
	ILPCSTR szIndicatorName = p_pIGraphInfo->GetIndicatorNameString();
	BOOL bIsDeleteAll = strcmp( szIndicatorName, _MTEXT( C2_PRICE_CHART)) && !g_iMetaTable.IsSpecialGroupGraph( szIndicatorName);

	// 5. Check with Default Info
	BOOL bEnableWithDefaultInfo = ( m_bEnableWithDefaultInfo && !szGraphName.IsEmpty());
	// <가격차트> 이고 1틱일 경우는 무조건 false !!
	if( bEnableWithDefaultInfo)
	{
		ILPCSTR szGraphName = p_pIGraphInfo->GetGraphNameString();
		if( !strcmp( szGraphName, _MTEXT( C2_PRICE_CHART))
			&& m_pIDefaultPacketManager->GetBaseTimeUnit() == 1
			&& CScaleBaseData::HORZ_TICK == m_pIDefaultPacketManager->GetBaseTimeDataType())
				bEnableWithDefaultInfo = FALSE;
	}
	
	// 5. Show IndicatorInfo Setting Dlg
	ML_SET_LANGUAGE_RES();

	CGraphSetting dlgSetting( m_pIChartOCX, bEnableWithDefaultInfo);
	dlgSetting.SetData( p_pIGraphInfo, bLogVScale, bReverseVScale, bIsDeleteAll);
	dlgSetting.DoModal();
	p_lResult = dlgSetting.GetReturnValue();
	// <지표설정> 값을 적용.
	// <확인> 누른 경우
	if( IDOK == p_lResult)
	{
		// Default Info Update.
		if( dlgSetting.IsWithDefaultInfo() && !szGraphName.IsEmpty())
		{
			IGraphInfo *pDefaultInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER_DEFAULT, szGraphName);
			pDefaultInfo->GetCopyDataFrom( p_pIGraphInfo);
			pDefaultInfo->Release();
		}

		// 바꿀 값들을 changeData 에 넣어준다.
		if( pIBlock)
		{
			pIBlock->SetLogScale( dlgSetting.GetLogState());
			pIBlock->SetVScaleInvert( nV, dlgSetting.GetInvertScaleType());

			// (2006/11/28 - Seung-Won, Bae) Use new Interface without IGraphInfo.
			// (2007/1/25 - Seung-Won, Bae) Recalculate always.
			//		It updated in OCX directlry with IndicatorInfo. (So. How can I check about the Recalculation State.)
			pIBlock->UpdateGraphWithGraphInfo( nV, nG, TRUE);	//  dlgSetting.DoCalculate());
		}
	}
	// <삭제> 누른 경우
	else if( IDABORT == p_lResult)
	{
		if( pIBlock) pIBlock->RemoveGraph( nV, nG);
		m_pIChartOCX->InvalidateControl();
	}

	if( pIBlock) pIBlock->Release();
	return TRUE;
}


void CChartEnvSetupDlgAddInImp::BackUpIndicatorInfo( IStringList *pStringList )
{
	if( pStringList == NULL )
		return;

	int nCount = pStringList->GetCount();

	m_pIAddInToolMgr->OnAddInToolCommand( EAI_INDICATORINFO_UNDO_REDO_MANAGER_BACKUP_INDICATORINFO,
										  nCount,
										  (LPCTSTR)pStringList,
										  NULL,
										  NULL);
}
