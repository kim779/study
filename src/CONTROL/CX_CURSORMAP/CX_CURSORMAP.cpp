// CX_CURSORMAP.cpp : Implementation of CCX_CURSORMAPApp and DLL registration.

#include "stdafx.h"
#include "CX_CURSORMAP.h"
#include "EXTCTRLCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCX_CURSORMAPApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
{ 0x24408bc4, 0xdef4, 0x4af8, {0x83, 0xdb, 0xcb, 0x36, 0x6e, 0x86, 0x69, 0x24}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CCX_CURSORMAPApp::InitInstance - DLL initialization

BOOL CCX_CURSORMAPApp::InitInstance()
{
	const	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CCX_CURSORMAPApp::ExitInstance - DLL termination

int CCX_CURSORMAPApp::ExitInstance()
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
// axCreate - Creates Ext Control

STDAPI_(CWnd*) axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	CEXTCTRLCtrl *m_pControlWnd = new CEXTCTRLCtrl();

	m_pControlWnd->m_bOleLock = false;
	AfxOleUnlockApp();	// Ole Control

	m_pControlWnd->m_pParent = parent;

	m_pControlWnd->SetParam((struct _param*)pParam);
	m_pControlWnd->Create(NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS, m_pControlWnd->m_Param.rect, parent, 100);
		
	return m_pControlWnd;

}
