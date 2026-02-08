// IWCmdUI.cpp: implementation of the CIWCmdUI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWCmdUI.h"

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWCmdUI)

//////////////////////////////////////////////////////////////////////
// (2006/7/12 - Seung-Won, Bae) Overrides
//////////////////////////////////////////////////////////////////////

// Operations to do in ON_UPDATE_COMMAND_UI
void CIWCmdUI::Enable( BOOL bOn)
{
	if( m_pCmdUI) m_pCmdUI->Enable( bOn);
}
void CIWCmdUI::SetCheck( int nCheck)
{
	if( m_pCmdUI) m_pCmdUI->SetCheck( nCheck);
}
void CIWCmdUI::SetRadio( BOOL bOn)
{
	if( m_pCmdUI) m_pCmdUI->SetRadio( bOn);
}
void CIWCmdUI::SetText( LPCTSTR lpszText)
{
	if( m_pCmdUI) m_pCmdUI->SetText( lpszText);
}

// Advanced operation
void CIWCmdUI::ContinueRouting()
{
	if( m_pCmdUI) m_pCmdUI->ContinueRouting();
}

// Implementation
BOOL CIWCmdUI::DoUpdate( LPDISPATCH dispTarget, BOOL bDisableIfNoHndler)
{
	if( !m_pCmdUI) return FALSE;
	CCmdTarget *pTarget = CCmdTarget::FromIDispatch( dispTarget);
	return m_pCmdUI->DoUpdate( pTarget, bDisableIfNoHndler);
}
