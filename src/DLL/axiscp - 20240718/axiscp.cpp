// axiscp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HINSTANCE	hInstance = NULL;
static AFX_EXTENSION_MODULE axiscpDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("axiscp.DLL Initializing!\n");
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(axiscpDLL, hInstance))
			return 0;
#pragma warning(disable : 26409)
		new CDynLinkLibrary(axiscpDLL);
#pragma warning(default:26409)
		
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("axiscp.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(axiscpDLL);
	}
	return 1;   // ok
}
