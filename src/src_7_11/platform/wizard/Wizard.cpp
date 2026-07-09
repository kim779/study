// Wizard.cpp : Implementation of CWizardApp and DLL registration.

#include "stdafx.h"
#include "Wizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWizardApp NEAR theApp;

//const GUID CDECL BASED_CODE _tlid =
//		{ 0xdb13014c, 0x73d9, 0x4d8e, { 0x84, 0xdb, 0xe5, 0x9e, 0xee, 0xf0, 0xb5, 0x5d } };

// {95840AF6-B82B-42DE-94C9-2039367689CB}
const GUID CDECL BASED_CODE _tlid =
		{ 0x95840af6, 0xb82b, 0x42de, { 0x94, 0xc9, 0x20, 0x39, 0x36, 0x76, 0x89, 0xcb } };

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CWizardApp::InitInstance - DLL initialization

BOOL CWizardApp::InitInstance()
{
	AfxEnableControlContainer();

	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CWizardApp::ExitInstance - DLL termination

int CWizardApp::ExitInstance()
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
