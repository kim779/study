// IWAddInToolMgr.cpp: implementation of the CIWAddInToolMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWAddInToolMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWAddInToolMgr)

//////////////////////////////////////////////////////////////////////
// (2006/2/12 - Seung-Won, Bae) Register/Unregister AddIn Event Map
//////////////////////////////////////////////////////////////////////

BOOL CIWAddInToolMgr::RegisterEventMap( const IChartAddIn::EAddInEventType p_eAddInEventType, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->RegisterEventMap( p_eAddInEventType, ( IAddInDll *)p_pIAddInDllBase, p_bOverExclusive);
}
BOOL CIWAddInToolMgr::UnregisterEventMap( const IChartAddIn::EAddInEventType p_eAddInEventType, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->UnregisterEventMap( p_eAddInEventType, ( IAddInDll *)p_pIAddInDllBase, p_bOverExclusive);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Keyboard & Mouse Exclusive Setting
//////////////////////////////////////////////////////////////////////

BOOL CIWAddInToolMgr::GetKeyboardExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->GetKeyboardExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::IsKeyboardExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->IsKeyboardExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::ReleaseKeyboardExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->ReleaseKeyboardExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::GetAllMouseExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->GetAllMouseExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::IsAllMouseExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->IsAllMouseExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::ReleaseAllMouseExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->ReleaseAllMouseExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::GetLButtonExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->GetLButtonExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::IsLButtonExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->IsLButtonExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::ReleaseLButtonExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->ReleaseLButtonExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::GetRButtonExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->GetRButtonExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::IsRButtonExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->IsRButtonExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::ReleaseRButtonExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->ReleaseRButtonExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::GetMouseMoveExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->GetMouseMoveExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::IsMouseMoveExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->IsMouseMoveExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::ReleaseMouseMoveExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->ReleaseMouseMoveExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::GetMouseWheelExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->GetMouseWheelExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::IsMouseWheelExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->IsMouseWheelExclusive( ( IAddInDll *)p_pIAddInDllBase);
}
BOOL CIWAddInToolMgr::ReleaseMouseWheelExclusive( IAddInDllBase *p_pIAddInDllBase)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->ReleaseMouseWheelExclusive( ( IAddInDll *)p_pIAddInDllBase);
}


//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL을 관리한다.
//////////////////////////////////////////////////////////////////////

// (2006/1/7 - Seung-Won, Bae) Interface for Spacial Interface of AddIn DLL
IUnknown *CIWAddInToolMgr::GetAddInCustomInterface( const char *p_szAddInModule)
{
	if( !m_pAddInManager) return NULL;
	return m_pAddInManager->GetAddInCustomInterface( p_szAddInModule);
}

// (2006/8/25 - Seung-Won, Bae) Check AddIn Loaded (DLL:IDS)
BOOL CIWAddInToolMgr::IsAddInLoaded( const char *p_szAddInItem)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->IsAddInLoaded( p_szAddInItem);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
//////////////////////////////////////////////////////////////////////

// (2006/10/4 - Seung-Won, Bae) On Real Data
void CIWAddInToolMgr::OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)
{
	if( m_pAddInManager) m_pAddInManager->OnRealData( p_szPacketName, p_dData, p_szRQ);
}
void CIWAddInToolMgr::OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData)
{
	if( m_pAddInManager) m_pAddInManager->OnIndexedRealData( p_szPacketName, p_nIndex, p_dData);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CIWAddInToolMgr::OnRealPacketEnd( const char *p_szRQ)
{
	if( m_pAddInManager) m_pAddInManager->OnRealPacketEnd( p_szRQ);
}

// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
void CIWAddInToolMgr::OnPacketTruncated( const int p_nDataCount)
{
	if( m_pAddInManager) m_pAddInManager->OnPacketTruncated( p_nDataCount);
}

//>> 갭보정 - vntsorl(20090217)
void CIWAddInToolMgr::OnPacketDataMultiItem( const char * p_szRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( m_pAddInManager) m_pAddInManager->OnPacketDataMultiItem( p_szRQ, p_szPacketNames, p_bWithReset);
}

// (2008/10/22 - Seung-Won, Bae) for Real Filtering
//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
//	You can change the Real Data Value. it will be set to chart.
BOOL CIWAddInToolMgr::OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ, BOOL p_bRealData)
{
	if( m_pAddInManager) return m_pAddInManager->OnPreRealData( p_szPacketName, p_dData, p_szRQ, p_bRealData);
	return TRUE;
}
BOOL CIWAddInToolMgr::OnPreIndexedRealData( const char *p_szPacketName, const int p_nIndex, double &p_dData, BOOL p_bRealData)
{
	if( m_pAddInManager) return m_pAddInManager->OnPreIndexedRealData( p_szPacketName, p_nIndex, p_dData, p_bRealData);
	return TRUE;
}
BOOL CIWAddInToolMgr::OnPreRealPacketEnd( const char *p_szRQ, BOOL p_bRealData)
{
	if( m_pAddInManager) return m_pAddInManager->OnPreRealPacketEnd( p_szRQ, p_bRealData);
	return TRUE;
}
BOOL CIWAddInToolMgr::OnPreNMTRClock( __int64 &p_nRealClockSecs)
{
	if( m_pAddInManager) return m_pAddInManager->OnPreNMTRClock( p_nRealClockSecs);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//////////////////////////////////////////////////////////////////////

BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID, p_hDC, nFlags, point, p_dwMouseActionID);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID, p_szParam1, p_szParam2);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID, p_szParam, p_lParam);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID, ( IAddInDll *)p_pParam);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID, p_lParam);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID, p_lParam, p_szParam1, p_szParam2, p_szParam3);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID, p_lParam1, p_lParam2);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_lResult, p_eCommandID);
}
BOOL CIWAddInToolMgr::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_lResult, p_eCommandID, p_lParam);
}
