// CX_FOMultiSel.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "../../h/axisfire.h"
#include "CX_FOMultiSel.h"
#include "FOMultiSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CCX_FOMultiSelApp

BEGIN_MESSAGE_MAP(CCX_FOMultiSelApp, CWinApp)
	//{{AFX_MSG_MAP(CCX_FOMultiSelApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCX_FOMultiSelApp construction

CCX_FOMultiSelApp::CCX_FOMultiSelApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCX_FOMultiSelApp object

CCX_FOMultiSelApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCX_FOMultiSelApp initialization

BOOL CCX_FOMultiSelApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CFOMultiSel *pCtrl = new CFOMultiSel(pParent, pParam);

	const	DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	if (!pCtrl->Create(NULL, NULL, style, pParam->rect, pParent, -1))
		return NULL;

	return pCtrl;
}

__declspec(dllexport) CWnd* WINAPI axCreateDLL(CWnd *pParent, _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return axCreate(pParent, pParam);
}