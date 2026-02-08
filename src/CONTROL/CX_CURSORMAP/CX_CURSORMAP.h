#pragma once

// CX_CURSORMAP.h : main header file for CX_CURSORMAP.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_CURSORMAPApp : See CX_CURSORMAP.cpp for implementation.

class CCX_CURSORMAPApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

