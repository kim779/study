// TechnicalIndexFunc.h : main header file for the TECHNICALINDEXFUNC DLL
//

#if !defined(AFX_TECHNICALINDEXFUNC_H__00E5535D_1753_475D_9E2B_909C424DD1CE__INCLUDED_)
#define AFX_TECHNICALINDEXFUNC_H__00E5535D_1753_475D_9E2B_909C424DD1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTechnicalIndexFuncApp
// See TechnicalIndexFunc.cpp for the implementation of this class
//
#include "../Include_ST/InnerFunction.h"


class CTechnicalIndexFuncApp : public CWinApp
{
public:
	CTechnicalIndexFuncApp();
	CInnerFunction* LoadInnerFunction(long lIndex);
	BOOL GetInputs(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTechnicalIndexFuncApp)
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTechnicalIndexFuncApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHNICALINDEXFUNC_H__00E5535D_1753_475D_9E2B_909C424DD1CE__INCLUDED_)
