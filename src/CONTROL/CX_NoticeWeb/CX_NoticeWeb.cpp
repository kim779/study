// CX_WebLink.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CX_WebLink.h"
#include "../../h/axisfire.h"
#include "WebLinkCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCX_WebLinkApp

BEGIN_MESSAGE_MAP(CCX_WebLinkApp, CWinApp)
	//{{AFX_MSG_MAP(CCX_WebLinkApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCX_WebLinkApp construction

CCX_WebLinkApp::CCX_WebLinkApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCX_WebLinkApp object

CCX_WebLinkApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCX_WebLinkApp initialization

BOOL CCX_WebLinkApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	AfxEnableControlContainer();

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

__declspec(dllexport) CWnd* APIENTRY axCreate(CWnd* parent, _param* param)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WebLinkCtrl* ctrl = new WebLinkCtrl;
	ctrl->Create(parent, param);
	return ctrl;
}
