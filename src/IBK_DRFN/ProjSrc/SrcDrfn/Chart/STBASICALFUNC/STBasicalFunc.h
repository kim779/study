// STBasicalFunc.h : main header file for the STBASICALFUNC DLL
//

#if !defined(AFX_STBASICALFUNC_H__4F248EE4_748D_449D_80A1_0B4F90C852C8__INCLUDED_)
#define AFX_STBASICALFUNC_H__4F248EE4_748D_449D_80A1_0B4F90C852C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSTBasicalFuncApp
// See STBasicalFunc.cpp for the implementation of this class
//
#include "../Include_ST/InnerFunction.h"
class CSTBasicalFuncApp : public CWinApp
{
public:
	CSTBasicalFuncApp();
	CInnerFunction* LoadInnerFunction(long lIndex);
	BOOL GetInputs(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTBasicalFuncApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTBasicalFuncApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STBASICALFUNC_H__4F248EE4_748D_449D_80A1_0B4F90C852C8__INCLUDED_)
