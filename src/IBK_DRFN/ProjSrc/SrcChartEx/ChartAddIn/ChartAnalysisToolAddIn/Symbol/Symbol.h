// Symbol.h : main header file for the SYMBOL DLL
//

#if !defined(AFX_SYMBOL_H__D1C3D2F2_C397_4ED6_8EB1_C79F71D15E10__INCLUDED_)
#define AFX_SYMBOL_H__D1C3D2F2_C397_4ED6_8EB1_C79F71D15E10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSymbolApp
// See Symbol.cpp for the implementation of this class
//

class CSymbolApp : public CWinApp
{
public:
	CSymbolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSymbolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOL_H__D1C3D2F2_C397_4ED6_8EB1_C79F71D15E10__INCLUDED_)
