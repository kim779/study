// ICmdUI.h: interface for the ICmdUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICMDUI_H__1F2508E4_461D_4E6A_B806_7C1A4060940E__INCLUDED_)
#define AFX_ICMDUI_H__1F2508E4_461D_4E6A_B806_7C1A4060940E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"					// for AddIn Version Define
#include "../_IWUnknown.h"				// for CIWUnknown

interface ICmdUI : public IUnknown
{
protected:
	virtual ~ICmdUI()	{}

public:
// Operations to do in ON_UPDATE_COMMAND_UI
	virtual void Enable( BOOL bOn = TRUE)	= 0;
	virtual void SetCheck( int nCheck = 1)	= 0;   // 0, 1 or 2 (indeterminate)
	virtual void SetRadio( BOOL bOn = TRUE)	= 0;
	virtual void SetText( LPCTSTR lpszText)	= 0;

// Advanced operation
	virtual void ContinueRouting() = 0;

// Implementation
	virtual BOOL DoUpdate( LPDISPATCH dispTarget, BOOL bDisableIfNoHndler) = 0;
};

#endif // !defined(AFX_ICMDUI_H__1F2508E4_461D_4E6A_B806_7C1A4060940E__INCLUDED_)
