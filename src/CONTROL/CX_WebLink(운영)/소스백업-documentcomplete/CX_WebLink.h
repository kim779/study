// CX_WebLink.h : main header file for the CX_WEBLINK DLL
//

#if !defined(AFX_CX_WEBLINK_H__13853D39_34B4_47F5_BD91_41BBEE834636__INCLUDED_)
#define AFX_CX_WEBLINK_H__13853D39_34B4_47F5_BD91_41BBEE834636__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_WebLinkApp
// See CX_WebLink.cpp for the implementation of this class
//

class CCX_WebLinkApp : public CWinApp
{
public:
	CCX_WebLinkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_WebLinkApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_WebLinkApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_WEBLINK_H__13853D39_34B4_47F5_BD91_41BBEE834636__INCLUDED_)
