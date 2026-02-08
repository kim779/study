// IBKSConnector.cpp : Implementation of CIBKSConnectorApp and DLL registration.

#include "stdafx.h"
#include "IBKSConnector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CIBKSConnectorApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
{ 0x90685967, 0x8f5e, 0x491a, { 0x8c, 0xca, 0x42, 0x20, 0x47, 0x9a, 0xc1, 0x89 } };

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CIBKSConnectorApp::InitInstance - DLL initialization

BOOL CIBKSConnectorApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	AfxOleInitModule();

	if (bInit)
	{
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CIBKSConnectorApp::ExitInstance - DLL termination

int CIBKSConnectorApp::ExitInstance()
{
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
