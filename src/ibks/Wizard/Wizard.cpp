// Wizard.cpp: CWizardApp의 구현 및 DLL 등록입니다.

#include "pch.h"
#include "framework.h"
#include "Wizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CWizardApp theApp;

const GUID CDECL _tlid = {0x18c830e8,0x7bd4,0x4d87,{0xb3,0x2e,0x6f,0xdb,0x4e,0xdd,0x61,0x07}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CWizardApp::InitInstance - DLL 초기화

BOOL CWizardApp::InitInstance()
{
	AfxEnableControlContainer();

	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 여기에 직접 작성한 모듈 초기화 코드를 추가합니다.
	}

	return bInit;
}



// CWizardApp::ExitInstance - DLL 종료

int CWizardApp::ExitInstance()
{
	// TODO: 여기에 직접 작성한 모듈 종료 코드를 추가합니다.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 시스템 레지스트리에 항목을 추가합니다.

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 시스템 레지스트리에서 항목을 제거합니다.

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
