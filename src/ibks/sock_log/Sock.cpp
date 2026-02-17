// Sock.cpp: CSockApp의 구현 및 DLL 등록입니다.

#include "pch.h"
#include "Sock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSockApp theApp;

const GUID CDECL _tlid = {0x2a62b8e8,0x6e92,0x4a7c,{0x8b,0x3f,0x4f,0x2f,0x87,0x34,0x43,0x80}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CSockApp::InitInstance - DLL 초기화

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
		// TODO: 여기에 직접 작성한 모듈 초기화 코드를 추가합니다.
	}

	return bInit;
}

// CSockApp::ExitInstance - DLL 종료
int CSockApp::ExitInstance()
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
