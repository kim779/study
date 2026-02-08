// cx_image.h : main header file for the CX_IMAGE DLL
//

#if !defined(AFX_CX_IMAGE_H__089AB289_5AA3_4388_B1CA_DAE73BBA3D84__INCLUDED_)
#define AFX_CX_IMAGE_H__089AB289_5AA3_4388_B1CA_DAE73BBA3D84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCx_imageApp
// See cx_image.cpp for the implementation of this class
//

class CCx_imageApp : public CWinApp
{
public:
	CCx_imageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCx_imageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCx_imageApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_IMAGE_H__089AB289_5AA3_4388_B1CA_DAE73BBA3D84__INCLUDED_)
