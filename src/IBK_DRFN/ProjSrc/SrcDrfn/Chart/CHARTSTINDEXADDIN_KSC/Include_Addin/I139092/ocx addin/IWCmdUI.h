// IWCmdUI.h: interface for the CIWCmdUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCMDUI_H__ADA0E3C5_67D3_4DF1_81E6_1CD350EAE716__INCLUDED_)
#define AFX_IWCMDUI_H__ADA0E3C5_67D3_4DF1_81E6_1CD350EAE716__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"
#include "_ICmdUI.h"

class CIWCmdUI : public ICmdUI, public CIWUnknown
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( ICmdUI, CIWCmdUI, CCmdUI, m_pCmdUI)

public:
// Operations to do in ON_UPDATE_COMMAND_UI
	virtual void Enable( BOOL bOn = TRUE);
	virtual void SetCheck( int nCheck = 1); // 0, 1 or 2 (indeterminate)
	virtual void SetRadio( BOOL bOn = TRUE);
	virtual void SetText( LPCTSTR lpszText);

// Advanced operation
	virtual void ContinueRouting();

// Implementation
	virtual BOOL DoUpdate( LPDISPATCH dispTarget, BOOL bDisableIfNoHndler);
};


#endif // !defined(AFX_IWCMDUI_H__ADA0E3C5_67D3_4DF1_81E6_1CD350EAE716__INCLUDED_)
