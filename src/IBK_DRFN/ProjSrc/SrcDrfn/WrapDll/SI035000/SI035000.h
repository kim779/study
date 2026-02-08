// SI035000.h : main header file for the SI035000 DLL
//

#if !defined(AFX_SI035000_H__8125C584_20CF_40C0_BE02_C1D1A7F489C0__INCLUDED_)
#define AFX_SI035000_H__8125C584_20CF_40C0_BE02_C1D1A7F489C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSI035000App
// See SI035000.cpp for the implementation of this class
//

class CSI035000App : public CWinApp
{
public:
	CSI035000App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSI035000App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSI035000App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SI035000_H__8125C584_20CF_40C0_BE02_C1D1A7F489C0__INCLUDED_)
