// ReplayChart.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "ReplayChartWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE ReplayChartDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("REPLAYCHART.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ReplayChartDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(ReplayChartDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("REPLAYCHART.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ReplayChartDLL);
	}
	return 1;   // ok
}

#define _EXPORT_DLL		extern "C" __declspec(dllexport)

_EXPORT_DLL CWnd* fnReplayDll_Load(HWND  hParentWnd)
{
	CString strWndClass = AfxRegisterWndClass(CS_DBLCLKS);
	CReplayChartWnd* pWnd = new CReplayChartWnd;
	if(pWnd)
	{
		
		pWnd->Create(	strWndClass,
						_T("ReplayChartWnd"),
						WS_CHILD | WS_VISIBLE,
						CRect(0,0,0,0),
						CWnd::FromHandle(hParentWnd), 8282);
		
		return pWnd;
	}

	return NULL;
}

_EXPORT_DLL void fnReplayDll_Unload(CWnd* pWnd)
{
	if (pWnd && pWnd->GetSafeHwnd())
	{
		pWnd->DestroyWindow();
		delete pWnd;
		pWnd = NULL;
	}
}

_EXPORT_DLL void fnReplayDll_SetInitInfo(CWnd* pWnd, void* pReplayInfo)
{
	if (pWnd && pWnd->GetSafeHwnd())
		((CReplayChartWnd*)pWnd)->Init(pReplayInfo);
}

_EXPORT_DLL BOOL fnReplayDll_Run(CWnd* pWnd, int nMode)
{
	if (pWnd && pWnd->GetSafeHwnd())
		return ((CReplayChartWnd*)pWnd)->Run(nMode);

	return FALSE;
}

//배속 조절
_EXPORT_DLL void fnReplayDll_ChangeSpeed(CWnd* pWnd, int nSpeed)
{
	if (pWnd && pWnd->GetSafeHwnd())
		((CReplayChartWnd*)pWnd)->ChangeSpeed(nSpeed);
}
