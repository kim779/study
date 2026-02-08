#include "stdafx.h"
#include <initguid.h>
#include "IB192100_i.c"
#include "IB192100.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CIB192100App, CWinApp)
	//{{AFX_MSG_MAP(CIB192100App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CIB192100App::CIB192100App()
{
}

CIB192100App theApp;

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	std::unique_ptr<CMapWnd> pWnd = std::make_unique<CMapWnd>(pParent);
	
	if (!pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, CRect(0, 0, 310, 650), pParent, 0))
	{
		pWnd = nullptr;
	}
	
	pWnd->Init();
	
	return pWnd.release();
}

	
CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount() == 0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}
/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}
/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer(TRUE); //TRUE indicates that typelib is unreg'd
	return S_OK;
}

BOOL CIB192100App::InitInstance()
{
	if (!InitATL())
		return FALSE;

	return CWinApp::InitInstance();

}

int CIB192100App::ExitInstance()
{
	_Module.Term();

	return CWinApp::ExitInstance();

}

BOOL CIB192100App::InitATL()
{
	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	return TRUE;

}
