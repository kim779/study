// CX_ContractX.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CX_ContractX.h"
#include "ControlWnd.h"

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
// CCX_ContractXApp

BEGIN_MESSAGE_MAP(CCX_ContractXApp, CWinApp)
	//{{AFX_MSG_MAP(CCX_ContractXApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCX_ContractXApp construction

CCX_ContractXApp::CCX_ContractXApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCX_ContractXApp object

CCX_ContractXApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCX_ContractXApp initialization

BOOL CCX_ContractXApp::InitInstance()
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

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, struct _param* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *pControlWnd = new CControlWnd(parent, pParam);
	if(!pControlWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, pParam->rect, parent, 100))
	{
		TRACE("Window Create Failed!!!\n");
		return 0;
	}
	pControlWnd->Init();

	return pControlWnd;
}

COLORREF CCX_ContractXApp::GetColor(CWnd *pWizard, int color)
{
	if (color & 0x02000000)
		return color;

	return pWizard->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CPen* CCX_ContractXApp::GetPen(CWnd *pWizard, int color, int width)
{
	struct	_penR	penR;

	penR.clr   = color;
	penR.width = 1;
	penR.style = PS_SOLID;
	return (CPen *)pWizard->SendMessage(WM_USER, MAKEWPARAM(getPEN, 0), (LPARAM)&penR);
}

CFont* CCX_ContractXApp::GetFont(CWnd *pWizard, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pWizard->SendMessage(WM_USER, getFONT, long(&rFont));
}
