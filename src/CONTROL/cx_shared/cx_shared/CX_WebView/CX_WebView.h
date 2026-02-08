// CX_WebView.h : main header file for the CX_WEBVIEW DLL
//

#if !defined(AFX_CX_WEBVIEW_H__99DC32BA_24B1_43AA_9304_8986D9B9D37B__INCLUDED_)
#define AFX_CX_WEBVIEW_H__99DC32BA_24B1_43AA_9304_8986D9B9D37B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_WebViewApp
// See CX_WebView.cpp for the implementation of this class
//

class CCX_WebViewApp : public CWinApp
{
public:
	CCX_WebViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_WebViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_WebViewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CX_WEBVIEW_H__99DC32BA_24B1_43AA_9304_8986D9B9D37B__INCLUDED_)
