// IB650001.h : main header file for the IB650001 DLL
//

#if !defined(AFX_IB650001_H__B4E77BD0_B5C8_4CA6_8CE5_1179AE36ECF9__INCLUDED_)
#define AFX_IB650001_H__B4E77BD0_B5C8_4CA6_8CE5_1179AE36ECF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB650001App
// See IB650001.cpp for the implementation of this class
//

class CIB650001App : public CWinApp
{
public:
	CIB650001App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB650001App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB650001App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IB650001_H__B4E77BD0_B5C8_4CA6_8CE5_1179AE36ECF9__INCLUDED_)
