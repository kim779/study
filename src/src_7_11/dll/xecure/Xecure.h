#if !defined(AFX_XECURE_H__F7D452D2_82EF_4B0D_964B_7C28BD2FD046__INCLUDED_)
#define AFX_XECURE_H__F7D452D2_82EF_4B0D_964B_7C28BD2FD046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Xecure.h : main header file for XECURE.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CXecureApp : See Xecure.cpp for implementation.

class CXecureApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XECURE_H__F7D452D2_82EF_4B0D_964B_7C28BD2FD046__INCLUDED)
