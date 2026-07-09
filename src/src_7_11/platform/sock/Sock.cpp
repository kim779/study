// Sock.cpp : Implementation of CSockApp and DLL registration.

#include "stdafx.h"
#include "Sock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSockApp NEAR theApp;

//const GUID CDECL BASED_CODE _tlid =
//		{ 0x1fe24ef3, 0x2a3d, 0x4bc7, { 0xb2, 0x2a, 0xbc, 0x77, 0x91, 0xf6, 0xe, 0x86 } };

// {4240D683-6BA4-4626-B0D9-27E13ACBE944}
const GUID CDECL BASED_CODE _tlid =
		{ 0x4240d683, 0x6ba4, 0x4626, { 0xb0, 0xd9, 0x27, 0xe1, 0x3a, 0xcb, 0xe9, 0x44 } };


const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CSockApp::InitInstance - DLL initialization

BOOL CSockApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(CG_IDS_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CSockApp::ExitInstance - DLL termination

int CSockApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
