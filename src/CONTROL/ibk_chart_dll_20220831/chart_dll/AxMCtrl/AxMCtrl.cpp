// AxMCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HINSTANCE self = NULL;
static AFX_EXTENSION_MODULE AxMCtrlDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("AXMCTRL.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(AxMCtrlDLL, hInstance))
			return 0;
		self = hInstance;

		new CDynLinkLibrary(AxMCtrlDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("AXMCTRL.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(AxMCtrlDLL);
	}
	return 1;   // ok
}
