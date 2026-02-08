// IWAddInDll.cpp: implementation of the CIWAddInDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWAddInDll.h"

#include "../../AddInManagerImp.h"						// for CAddInManagerImp
#include "_IChartAddIn.h"								// for IChartAddIn
#include "_IGraphInfo.h"								// for IGraphInfo
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
		CASE_ADDINEVENT( OnAddInMsg)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_CObject)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long)
		CASE_ADDINEVENT( OnCaptureChanged)
		CASE_ADDINEVENT( OnCmdMsg)
		CASE_ADDINEVENT( OnDrawAfterGraph)
		CASE_ADDINEVENT( OnDrawBeforeGraph)
		CASE_ADDINEVENT( OnGSharedDataChange)
		CASE_ADDINEVENT( OnLSharedDataChange)
		CASE_ADDINEVENT( OnKeyDown)
		CASE_ADDINEVENT( OnKeyUp)
		CASE_ADDINEVENT( OnMouseMove)
		CASE_ADDINEVENT( OnMouseWheel)
		CASE_ADDINEVENT( OnLButtonDown)
		CASE_ADDINEVENT( OnLButtonUp)
		CASE_ADDINEVENT( OnLButtonDblClk)
		CASE_ADDINEVENT( OnRButtonDblClk)
		CASE_ADDINEVENT( OnRButtonDown)
		CASE_ADDINEVENT( OnRButtonUp)
		CASE_ADDINEVENT( OnResetChart)
		CASE_ADDINEVENT( OnTimer)
		CASE_ADDINEVENT( OnResetMainBlock)
		CASE_ADDINEVENT( OnUserIndiatorConditionLoaded)
		CASE_ADDINEVENT( OnChartItemChanged)
		CASE_ADDINEVENT( OnGraphDataChanged)
		CASE_ADDINEVENT( OnBlockRemoved)
		CASE_ADDINEVENT( OnGraphAdded)
		CASE_ADDINEVENT( OnManualRealData)
		CASE_ADDINEVENT( OnContextMenu)
		CASE_ADDINEVENT( OnGraphDeleted)
		CASE_ADDINEVENT( OnPacketTruncated)
		CASE_ADDINEVENT( OnBlockChanged)
		CASE_ADDINEVENT( OnToolCommand)
		CASE_ADDINEVENT( OnRealData)
		CASE_ADDINEVENT( OnIndexedRealData)
		CASE_ADDINEVENT( OnRealPacketEnd)
		CASE_ADDINEVENT( OnLoadEnvironment)
		CASE_ADDINEVENT( OnSaveEnvironment)
		CASE_ADDINEVENT( DrawCursor)
		CASE_ADDINEVENT( OnRecalculationComplete)
		// RQ별 Packet : 복수종목 - ojtaso (20070111)
		CASE_ADDINEVENT( OnPacketDataMultiItem)
		// (2008/10/22 - Seung-Won, Bae) for Real Filtering
		//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
		CASE_ADDINEVENT( OnPreRealData)
		CASE_ADDINEVENT( OnPreIndexedRealData)
		CASE_ADDINEVENT( OnPreRealPacketEnd)
		CASE_ADDINEVENT( OnPreNMTRClock)

		case IChartAddIn::ADDINEVENT( OnUpdatePacketData):	AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnUserIndiatorConditionLoaded));
			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnPacketDataMultiItem));		// (2008/5/9 - Seung-Won, Bae) Delete OnPacketData. (It is not used any more.)
			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnManualRealData));
			AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType::ADDINEVENT( OnRealPacketEnd));
		break;
	}

	return eAddInEventType;
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
BOOL CIWAddInDll::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam)
{
	IGraphInfo *pIGraphInfo = CIWGraphInfo::CreateObject( p_pParam);
	BOOL bResult = m_pIChartAddIn->OnAddInToolCommand( p_lResult, p_eCommandID, pIGraphInfo);
	pIGraphInfo->Release();
	return bResult;
}
