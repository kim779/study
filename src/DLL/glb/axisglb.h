// axisglb.h : main header file for the AXISGLB DLL
//

#if !defined(AFX_AXISGLB_H__0AB52AED_E59F_40A4_9A0A_47559E4AD076__INCLUDED_)
#define AFX_AXISGLB_H__0AB52AED_E59F_40A4_9A0A_47559E4AD076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAxisglbApp
// See axisglb.cpp for the implementation of this class
//

class CAxisglbApp : public CWinApp
{
public:
	CAxisglbApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisglbApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisglbApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISGLB_H__0AB52AED_E59F_40A4_9A0A_47559E4AD076__INCLUDED_)
