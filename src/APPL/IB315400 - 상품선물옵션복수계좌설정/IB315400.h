// IB315400.h : main header file for the IB315400 DLL
//

#if !defined(AFX_IB315400_H__F91ECC1B_6BD7_4157_90BF_70FDE0C4DEE6__INCLUDED_)
#define AFX_IB315400_H__F91ECC1B_6BD7_4157_90BF_70FDE0C4DEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB315400App
// See IB315400.cpp for the implementation of this class
//

class CIB315400App : public CWinApp
{
public:
	CIB315400App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB315400App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB315400App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IB315400_H__F91ECC1B_6BD7_4157_90BF_70FDE0C4DEE6__INCLUDED_)
