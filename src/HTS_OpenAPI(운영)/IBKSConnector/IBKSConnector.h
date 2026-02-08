#if !defined(AFX_IBKSCONNECTOR_H__842F056B_ECB2_4D76_BEFD_549C6DB3B5EC__INCLUDED_)
#define AFX_IBKSCONNECTOR_H__842F056B_ECB2_4D76_BEFD_549C6DB3B5EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IBKSConnector.h : main header file for IBKSCONNECTOR.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIBKSConnectorApp : See IBKSConnector.cpp for implementation.

class CIBKSConnectorApp : public COleControlModule
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

#endif // !defined(AFX_IBKSCONNECTOR_H__842F056B_ECB2_4D76_BEFD_549C6DB3B5EC__INCLUDED)
