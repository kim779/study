// SignalSet.h : main header file for the SIGNALSET DLL
//

#if !defined(AFX_SIGNALSET_H__DC97C226_273E_4C2F_9B7D_EF379201B427__INCLUDED_)
#define AFX_SIGNALSET_H__DC97C226_273E_4C2F_9B7D_EF379201B427__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSignalSetApp
// See SignalSet.cpp for the implementation of this class
//

class CSignalSetApp : public CWinApp
{
public:
	CSignalSetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSignalSetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSignalSetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNALSET_H__DC97C226_273E_4C2F_9B7D_EF379201B427__INCLUDED_)
