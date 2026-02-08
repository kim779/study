// IWAddInDll.cpp: implementation of the CIWAddInDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWAddInDll.h"

#include "../../../Include_Chart/BlockBaseData.h"		// for CBlockIndex
#include "../../AddInManagerImp.h"						// for CAddInManagerImp
#include "_IChartAddIn.h"								// for IChartAddIn
#include "_IGraphInfo.h"								// for IGraphInfo
#include "_AddInCommand.h"								// for EAI_ALL_ANALYSIS_RESET_TEXTPOSITION
#include "IWCmdUI.h"									// for CWICmdUI
#include "IWGraphInfo.h"								// for CIWGraphInfo

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// [04/10/07, 배승원] Addin DLL의 Module Name과 Handle을 관리하여 DLL Interface를 호출할 수 있도록 한다.
CIWAddInDll::CIWAddInDll( const char *p_szAddInModule, HMODULE p_hAddInDll, IChartCtrl *p_pIChartCtrl)
{
	m_hAddInDll		= p_hAddInDll;
	m_strModuleName	= p_szAddInModule;
	m_pIChartCtrl	= p_pIChartCtrl;
	if( !m_hAddInDll)
	{
		AfxMessageBox( "CIWAddInDll" _AIVERS " Null Wrapping Error!");
		return;
	}

	// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOCX Interface Gateway)를 전달하여
	//		DLL의 AddIn Interface를 확보한다.
	m_pIChartAddIn = NULL;

	// (2006/9/26 - Seung-Won, Bae) Additional Requesting Event Map
	m_nAdditionalEventCount = 0;
	m_eaAdditionalEvent[ m_nAdditionalEventCount] = IAddInDllBase::EAddInEventType::EAE_NOT_DEFINED;
}
CIWAddInDll::~CIWAddInDll()
{
	// (2004.10.08, 배승원) AddIn Object가 성공적으로 등록된 경우, AddIn Object를 Release한다.
	if( m_pIChartAddIn)
	{
		m_pIChartAddIn->Release();
		m_pIChartAddIn = NULL;
	}

	// (2004.10.07, 배승원) 등록된 AddIn DLL을 Free시킨다.
	// (2006/3/24 - Seung-Won, Bae) Do not free AddIn DLL for other Reference by GetAddInCustomInterface()
//	FreeLibrary( m_hAddInDll);
}
IAddInDll *CIWAddInDll::CreateObject( const char *p_szAddInModule, HMODULE p_hAddInDll, IChartCtrl *p_pIChartCtrl)
{
	IAddInDll *pIAddInDll = new CIWAddInDll( p_szAddInModule, p_hAddInDll, p_pIChartCtrl);
	pIAddInDll->AddRef();
	return pIAddInDll;
}


//[04/10/07] AddIn DLL을 초기화시키는 Interface를 제공한다.
// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOCX Interface Gateway)를 전달하여
//		DLL의 AddIn Interface를 확보한다.
typedef IChartAddIn *( *FN_InitAddInDll)( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDllBase *p_pIAddInDllBase);	
BOOL CIWAddInDll::InitAddInDll( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDll *p_pIAddInDLL)
{
	// 0. InitAddInDll Interface의 Process Address를 확인한다.
	FN_InitAddInDll fnInitAddInDll;
	fnInitAddInDll = ( FN_InitAddInDll)GetProcAddress( m_hAddInDll, "InitAddInDll");
	if( !fnInitAddInDll) return FALSE;

	m_pIChartAddIn = ( *fnInitAddInDll)( p_szAddInItem, p_pIAddInManager, p_pIAddInDLL);

	// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
	BOOL bResult = ( m_pIChartAddIn != NULL);
	if( bResult) m_strItemName = p_szAddInItem;

	return bResult;
}

// (2006/9/26 - Seung-Won, Bae) Additional Requesting Event Map
void CIWAddInDll::AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType p_eAddInEventType)
{
	if( _IWADLL_ADDITIONAL_EVENT_COUNT <= m_nAdditionalEventCount + 1) return;
	m_eaAdditionalEvent[ m_nAdditionalEventCount++] = p_eAddInEventType;
	m_eaAdditionalEvent[ m_nAdditionalEventCount] = IAddInDllBase::EAddInEventType::EAE_NOT_DEFINED;
}
void CIWAddInDll::ResetAdditionalRequestEvent( void)
{
	m_nAdditionalEventCount = 0;
	m_eaAdditionalEvent[ m_nAdditionalEventCount] = IAddInDllBase::EAddInEventType::EAE_NOT_DEFINED;
}

// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
int CIWAddInDll::RegisterEventMap( CAddInManagerImp *p_pAddInManager, const BOOL p_bOverExclusive)
{	
	ResetAdditionalRequestEvent();
	int nRegisteredCount = m_pIChartAddIn->RegisterEventMap( p_bOverExclusive);
	m_bOverExclusive = p_bOverExclusive;
	for( int i = 0; m_eaAdditionalEvent[ i] != IAddInDllBase::EAddInEventType::EAE_NOT_DEFINED; i++)
		if( p_pAddInManager->RegisterEventMap( m_eaAdditionalEvent[ i], this, m_bOverExclusive, TRUE)) nRegisteredCount++;
	return nRegisteredCount;
}
int CIWAddInDll::UnregisterEventMap( CAddInManagerImp *p_pAddInManager)
{	
	ResetAdditionalRequestEvent();
	int nUnregisteredCount = m_pIChartAddIn->UnregisterEventMap();				
	for( int i = 0; m_eaAdditionalEvent[ i] != IAddInDllBase::EAddInEventType::EAE_NOT_DEFINED; i++)
		if( p_pAddInManager->UnregisterEventMap( m_eaAdditionalEvent[ i], this, m_bOverExclusive, TRUE)) nUnregisteredCount++;
	return nUnregisteredCount;
}
#define CASE_ADDINEVENT_CONVERT( OnAddInEvent, OnOcxEvent)	case IChartAddIn::ADDINEVENT( OnAddInEvent):	eAddInEventType = IAddInDllBase::EAddInEventType::ADDINEVENT( OnOcxEvent);	break;
#define CASE_ADDINEVENT( OnAddInEvent)						CASE_ADDINEVENT_CONVERT( OnAddInEvent, OnAddInEvent)
IAddInDllBase::EAddInEventType	CIWAddInDll::GetGlobalAddInEventType( const long p_eAddInEventType)
{
	IAddInDllBase::EAddInEventType eAddInEventType = IAddInDllBase::EAddInEventType::EAE_NOT_DEFINED;
	switch( ( EAddInEventType)p_eAddInEventType)
	{
		CASE_ADDINEVENT_CONVERT( OnAddInToolCommand_EAICommandType_CBlockBehaviorData, OnBlockChanged)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long)
		CASE_ADDINEVENT_CONVERT( OnAddInToolCommand_EAICommandType_HWND_POINT, OnContextMenu)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char)
		CASE_ADDINEVENT_CONVERT( OnAddInToolCommand_long_EAICommandType_char_CObject, OnAddInToolCommand_long_EAICommandType_char_char_CObject)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long)
		CASE_ADDINEVENT( OnCaptureChanged)
		CASE_ADDINEVENT( OnCmdMsg)
		CASE_ADDINEVENT( OnDrawAfterGraph)
		CASE_ADDINEVENT( OnDrawBeforeGraph)
		CASE_ADDINEVENT_CONVERT( OnDrdsData, OnAddInToolCommand_EAICommandType_long_long)
		CASE_ADDINEVENT_CONVERT( OnDrdsPacketEnd, OnAddInToolCommand_EAICommandType_char_char)
		CASE_ADDINEVENT( OnGSharedDataChange)
		CASE_ADDINEVENT( OnKeyDown)
		CASE_ADDINEVENT( OnKeyUp)
		CASE_ADDINEVENT( OnLButtonDblClk)
		CASE_ADDINEVENT( OnLButtonDown)
		CASE_ADDINEVENT( OnLButtonUp)
		CASE_ADDINEVENT( OnLSharedDataChange)
		CASE_ADDINEVENT( OnMouseMove)
		CASE_ADDINEVENT( OnMouseWheel)
		CASE_ADDINEVENT( OnRButtonDblClk)
		CASE_ADDINEVENT( OnRButtonDown)
		CASE_ADDINEVENT( OnRButtonUp)
		CASE_ADDINEVENT( OnResetChart)
		// (2006/10/5 - Seung-Won, Bae) Not Used
		//	CASE_ADDINEVENT( OnSharedDataChange)
		CASE_ADDINEVENT( OnTimer)

		// (2006/9/26 - Seung-Won, Bae) ReMap EAI_ALL_APPLY_STYLE
		case IChartAddIn::ADDINEVENT( OnAddInToolCommand_EAICommandType):	eAddInEventType = IAddInDllBase::EAddInEventType::ADDINEVENT( OnAddInToolCommand_EAICommandType);
																			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnResetMainBlock));			// for EAI_ALL_RESET_CURRENT_TOOL
																			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnPacketDataMultiItem));		// for EAI_ALL_RESET_CURRENT_TOOL	// (2008/5/9 - Seung-Won, Bae) Delete OnPacketData. (It is not used any more.)
																			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnChartItemChanged));		// for EAI_ALL_RESET_CURRENT_TOOL
																			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnGraphDataChanged));		// for EAI_ALL_RESET_CURRENT_TOOL
																			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnBlockRemoved));			// for EAI_ALL_RESET_CURRENT_TOOL
																			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnGraphAdded));				// for EAI_ALL_RESET_CURRENT_TOOL
																			break;
		case IChartAddIn::ADDINEVENT( OnUpdatePacketData):	AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnUserIndiatorConditionLoaded));
															AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnPacketDataMultiItem));		// (2008/5/9 - Seung-Won, Bae) Delete OnPacketData. (It is not used any more.)
															AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnManualRealData));
															AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnRealPacketEnd));
															break;
		case IChartAddIn::ADDINEVENT( OnAddInToolCommand_EAICommandType_char):	eAddInEventType = IAddInDllBase::EAddInEventType::ADDINEVENT( OnAddInToolCommand_EAICommandType_char);
																				AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnGraphDeleted));				// for EAI_ALL_ON_DELETE_INDICATOR
																				break;
		case IChartAddIn::ADDINEVENT( OnAddInToolCommand_EAICommandType_long):	eAddInEventType = IAddInDllBase::EAddInEventType::ADDINEVENT( OnAddInToolCommand_EAICommandType_long);
																				AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnPacketTruncated));				// for EAI_ST_REMOVED_DATA_COUNT
																				break;
		case IChartAddIn::ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long):	eAddInEventType = IAddInDllBase::EAddInEventType::ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long);
																					AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnToolCommand));								// for EAI_OOA_ON_TOOL_FUNCTION
																					break;
		// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
		//CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject)
		// RQ별 Packet : 복수종목 - ojtaso (20070111)
		CASE_ADDINEVENT( OnPacketDataMultiItem)
		// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
		case IChartAddIn::ADDINEVENT( OnUpdatePacketDataMultiItem):	eAddInEventType = IAddInDllBase::EAddInEventType::ADDINEVENT( OnRealPacketEnd);
																	AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnPacketDataMultiItem));								// for EAI_OOA_ON_TOOL_FUNCTION
																	break;
		// (2008/5/9 - Seung-Won, Bae) Delete OnPacketData. (It is not used any more.)
		case IChartAddIn::ADDINEVENT( OnPacketData):	eAddInEventType = IAddInDllBase::EAddInEventType::ADDINEVENT( OnPacketDataMultiItem);
														break;
	}

	return eAddInEventType;
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CIWAddInDll::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	m_pIChartAddIn->OnPacketDataMultiItem(lpszRQ, p_szPacketNames, p_bWithReset);

	// (2008/5/9 - Seung-Won, Bae) Delete OnPacketData. (It is not used any more.)
	m_pIChartAddIn->OnPacketData( p_szPacketNames, p_bWithReset);

	OnAddInToolCommand( EAI_ALL_RESET_CURRENT_TOOL);

	// (2004.12.07, 배승원) Real의 반영에 따른 지표 계산을 싯점을 AddIn Module에게 알린다.
	OnUpdatePacketDataMultiItem(lpszRQ, TRUE);

	// (2008/5/9 - Seung-Won, Bae) Delete OnPacketData. (It is not used any more.)
	OnUpdatePacketData( TRUE);
}

// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
void CIWAddInDll::OnResetMainBlock( void)
{
	// (2006/1/24 - Seung-Won, Bae) Reset Text Position of Analysis Tool
	OnAddInToolCommand( EAI_ALL_ANALYSIS_RESET_TEXTPOSITION);

	OnAddInToolCommand( EAI_ALL_RESET_CURRENT_TOOL);
	// (2006/1/24 - Seung-Won, Bae) Reset Text Position of Analysis Tool
	OnAddInToolCommand( EAI_ALL_ENVIRONMENT_CHANGED);
}

// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
void CIWAddInDll::OnUserIndiatorConditionLoaded( void)
{
	OnUpdatePacketData( TRUE);
}

// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
void CIWAddInDll::OnChartItemChanged( void)
{
	OnAddInToolCommand( EAI_ALL_RESET_CURRENT_TOOL);
}

// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
void CIWAddInDll::OnGraphDataChanged( void)
{
	OnAddInToolCommand( EAI_ALL_RESET_CURRENT_TOOL);
}

// (2006/9/25 - Seung-Won, Bae) On Block Removed.
void CIWAddInDll::OnBlockRemoved( const int p_nRow, const int p_nColumn)
{
	OnAddInToolCommand( EAI_ALL_RESET_CURRENT_TOOL);
}

// (2006/9/25 - Seung-Won, Bae) On Graph Added.
void CIWAddInDll::OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)
{
	OnAddInToolCommand( EAI_ALL_RESET_CURRENT_TOOL);
}

// (2006/9/25 - Seung-Won, Bae) On Graph Added.
void CIWAddInDll::OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption)
{
	OnUpdatePacketData();
}

// (2006/9/27 - Seung-Won, Bae) On Context Menu.
void CIWAddInDll::OnContextMenu( HWND p_hWnd, POINT &p_pt)
{
	OnAddInToolCommand( EAI_ALL_CONTEXT_MENU, p_hWnd, p_pt);
}

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CIWAddInDll::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	OnAddInToolCommand( EAI_ALL_ON_DELETE_INDICATOR, p_szIndicatorName);
}

// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
void CIWAddInDll::OnPacketTruncated( const int p_nDataCount)
{
	OnAddInToolCommand( EAI_ST_REMOVED_DATA_COUNT, p_nDataCount);
}

// (2006/9/27 - Seung-Won, Bae) On Block Changed.
void CIWAddInDll::OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)
{
	CBlockIndex sourceindex( p_nSourceRow, p_nSourceColumn);
	CBlockIndex targetindex( p_nTargetRow, p_nTargetColumn);
	CBlockBehaviorData bbdData( p_eChangeType, sourceindex, targetindex);
	OnAddInToolCommand( EAI_ALL_BLOCK_BEHAVIOR, &bbdData);
}

// (2006/9/27 - Seung-Won, Bae) On Block Changed.
void CIWAddInDll::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, p_nToolType, p_nCmdOption);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CIWAddInDll::OnRealPacketEnd( const char *p_szRQ)
{
	OnUpdatePacketData();
	OnUpdatePacketDataMultiItem( p_szRQ);
}

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
BOOL CIWAddInDll::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	ICmdUI *pICmdUI = CIWCmdUI::CreateObject( ( CCmdUI *)pExtra);
	BOOL bResult = m_pIChartAddIn->OnCmdMsg( nID, nCode, pICmdUI);
	pICmdUI->Release();
	return bResult;
}
BOOL CIWAddInDll::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)
{
	if( EAI_DRDS_PACKET_END == p_eCommandID) OnDrdsPacketEnd( ( WPARAM)p_szParam1, ( LPARAM)p_szParam2);
	return FALSE;
}
BOOL CIWAddInDll::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( EAI_DRDS_DATA == p_eCommandID)
	{
		OnDrdsData( ( WPARAM)p_lParam1, ( LPARAM)p_lParam2);
		return FALSE;
	}
	return m_pIChartAddIn->OnAddInToolCommand( p_eCommandID, p_lParam1, p_lParam2);
}
BOOL CIWAddInDll::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam)
{
	IGraphInfo *pIGraphInfo = CIWGraphInfo::CreateObject( p_pParam);
	BOOL bResult = m_pIChartAddIn->OnAddInToolCommand( p_lResult, p_eCommandID, pIGraphInfo);
	pIGraphInfo->Release();
	return bResult;
}
