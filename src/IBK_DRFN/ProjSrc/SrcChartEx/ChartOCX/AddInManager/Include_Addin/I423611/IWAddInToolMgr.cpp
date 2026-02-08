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
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//////////////////////////////////////////////////////////////////////

BOOL CIWAddInToolMgr::OnAddInToolCommand( const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_eCommandID, pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID);
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
BOOL CIWAddInToolMgr::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
{
	if( !m_pAddInManager) return FALSE;
	return m_pAddInManager->OnAddInToolCommand( p_lResult, p_eCommandID, p_lParam);
}
