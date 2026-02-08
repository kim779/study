// CX_Symbol.h : main header file for the CX_Symbol DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main CX_Symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_SymbolApp
// See CX_Symbol.cpp for the implementation of this class
//

class CCX_SymbolApp : public CWinApp
{
public:
	CCX_SymbolApp();

private:
	void	loadDefaultResource();
	void	freeDefaultResource();

	HINSTANCE m_hInst;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_SymbolApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_SymbolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};